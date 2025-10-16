"""
USB TO SERIAL 통신 모듈 - 프로토콜 적용
"""
import serial
import serial.tools.list_ports
import threading
import time
import queue
import struct
from datetime import datetime


class ProtocolHandler:
    """프로토콜 데이터 처리 클래스"""
    
    # 프로토콜 상수
    STX = 0x02
    ETX = 0x03
    
    # Device IDs
    PC_ID = 0x01
    MAIN_ID = 0x02
    FRONT_ID = 0x03
    
    # CMD와 DATA FIELD LENGTH 매핑
    CMD_LENGTH_MAP = {
        0x0F: 0,   # Heartbeat
        0xA0: 20,  # 밸브 제어 (NOS 1~5: 5바이트, FEED 1~15: 15바이트)
        0xA1: 1,
        0xB0: 5,
        0xB1: 5,   # 냉각 제어 (ON온도 정수부, ON온도 소수부, OFF온도 정수부, OFF온도 소수부, 추가시간)
        0xB2: 5,
        0xB3: 48,
        0xC0: 7
    }
    
    def __init__(self):
        self.receive_buffer = bytearray()
    
    def calculate_crc16(self, data):
        """CRC16-CCITT 계산 (STX ~ DATA FIELD)"""
        crc = 0xFFFF
        for byte in data:
            crc ^= byte << 8
            for _ in range(8):
                if crc & 0x8000:
                    crc = (crc << 1) ^ 0x1021
                else:
                    crc = crc << 1
                crc &= 0xFFFF
        return crc
    
    def create_packet(self, tx_id, rx_id, cmd, data_field=None):
        """
        프로토콜 패킷 생성
        STX + TX ID + RX ID + CMD + DATA FIELD LENGTH + DATA FIELD + CRC16 + ETX
        """
        if cmd not in self.CMD_LENGTH_MAP:
            raise ValueError(f"지원하지 않는 CMD: 0x{cmd:02X}")
        
        expected_length = self.CMD_LENGTH_MAP[cmd]
        
        # DATA FIELD 검증
        if data_field is None:
            data_field = bytes(expected_length)
        elif len(data_field) != expected_length:
            raise ValueError(f"CMD 0x{cmd:02X}의 DATA FIELD는 {expected_length}바이트여야 합니다 (현재: {len(data_field)}바이트)")
        
        # 패킷 구성
        packet = bytearray()
        packet.append(self.STX)
        packet.append(tx_id)
        packet.append(rx_id)
        packet.append(cmd)
        packet.append(expected_length)
        packet.extend(data_field)
        
        # CRC16 계산 (STX ~ DATA FIELD)
        crc = self.calculate_crc16(packet)
        packet.extend(struct.pack('>H', crc))  # Big-endian 2바이트
        
        packet.append(self.ETX)
        
        return bytes(packet)
    
    def create_heartbeat_packet(self, tx_id=PC_ID, rx_id=MAIN_ID):
        """CMD 0x0F (Heartbeat) 패킷 생성"""
        return self.create_packet(tx_id, rx_id, 0x0F)
    
    def parse_packet(self, packet_data):
        """패킷 파싱"""
        if len(packet_data) < 8:  # 최소 패킷 크기
            return None
        
        try:
            stx = packet_data[0]
            tx_id = packet_data[1]
            rx_id = packet_data[2]
            cmd = packet_data[3]
            data_length = packet_data[4]
            
            expected_total = 7 + data_length  # STX + IDs + CMD + LEN + DATA + CRC16 + ETX
            
            if len(packet_data) != expected_total:
                return None
            
            data_field = packet_data[5:5+data_length]
            crc_received = struct.unpack('>H', packet_data[5+data_length:7+data_length])[0]
            etx = packet_data[-1]
            
            # 검증
            if stx != self.STX or etx != self.ETX:
                return None
            
            # CRC 검증
            crc_data = packet_data[:-3]  # STX ~ DATA FIELD
            crc_calculated = self.calculate_crc16(crc_data)
            
            if crc_received != crc_calculated:
                return None
            
            return {
                'tx_id': tx_id,
                'rx_id': rx_id,
                'cmd': cmd,
                'data_length': data_length,
                'data_field': data_field,
                'crc': crc_received
            }
            
        except Exception:
            return None
    
    def process_received_data(self, new_data):
        """수신 버퍼에서 패킷 추출"""
        self.receive_buffer.extend(new_data)
        packets = []
        
        while len(self.receive_buffer) > 0:
            # STX 찾기
            stx_index = -1
            for i in range(len(self.receive_buffer)):
                if self.receive_buffer[i] == self.STX:
                    stx_index = i
                    break
            
            if stx_index == -1:
                self.receive_buffer.clear()
                break
            
            # STX 이전 데이터 제거
            if stx_index > 0:
                self.receive_buffer = self.receive_buffer[stx_index:]
            
            # 최소 헤더 크기 확인 (STX + TX + RX + CMD + LEN)
            if len(self.receive_buffer) < 5:
                break
            
            data_length = self.receive_buffer[4]
            expected_total = 7 + data_length
            
            if len(self.receive_buffer) < expected_total:
                break
            
            # 패킷 추출 시도
            packet_candidate = bytes(self.receive_buffer[:expected_total])
            parsed = self.parse_packet(packet_candidate)
            
            if parsed:
                packets.append(parsed)
                self.receive_buffer = self.receive_buffer[expected_total:]
            else:
                # 파싱 실패시 STX 다음부터 다시 시도
                self.receive_buffer = self.receive_buffer[1:]
        
        return packets


class SerialCommunication:
    def __init__(self):
        self.serial_connection = None
        self.is_connected = False
        self.receive_thread = None
        self.send_thread = None
        self.heartbeat_thread = None
        self.stop_thread = False
        
        # 데이터 큐들
        self.receive_queue = queue.Queue()
        self.send_queue = queue.Queue()
        self.status_queue = queue.Queue()
        
        # 연결 정보
        self.current_port = None
        self.current_baudrate = None
        
        # 프로토콜 핸들러
        self.protocol = ProtocolHandler()
        
        # Heartbeat 설정
        self.heartbeat_interval = 0.1  # 100ms
        self.heartbeat_active = False
    
    def get_available_ports(self):
        """사용 가능한 시리얼 포트 목록 반환"""
        ports = serial.tools.list_ports.comports()
        return [f"{port.device} - {port.description}" for port in ports]
    
    def check_port_availability(self, port_info):
        """포트 사용 가능성 확인"""
        try:
            port = port_info.split(" - ")[0] if " - " in port_info else port_info
            
            test_connection = serial.Serial(
                port=port,
                baudrate=115200,
                timeout=0.1
            )
            test_connection.close()
            return True, "포트 사용 가능"
            
        except PermissionError:
            return False, "포트가 다른 프로그램에서 사용 중입니다"
        except serial.SerialException as e:
            if "Access is denied" in str(e) or "Permission denied" in str(e):
                return False, "포트 접근이 거부되었습니다"
            elif "cannot find" in str(e).lower() or "not found" in str(e).lower():
                return False, "포트를 찾을 수 없습니다"
            else:
                return False, f"포트 오류: {str(e)}"
        except Exception as e:
            return False, f"알 수 없는 오류: {str(e)}"
    
    def connect(self, port_info, baudrate):
        """시리얼 포트 연결"""
        try:
            if self.is_connected:
                self.disconnect()
            
            port = port_info.split(" - ")[0] if " - " in port_info else port_info
            
            self.serial_connection = serial.Serial(
                port=port,
                baudrate=int(baudrate),
                bytesize=serial.EIGHTBITS,
                parity=serial.PARITY_NONE,
                stopbits=serial.STOPBITS_ONE,
                timeout=1
            )
            
            self.is_connected = True
            self.stop_thread = False
            self.current_port = port
            self.current_baudrate = baudrate
            
            # 수신 스레드 시작
            self.receive_thread = threading.Thread(target=self._receive_worker, daemon=True)
            self.receive_thread.start()
            
            # 송신 스레드 시작
            self.send_thread = threading.Thread(target=self._send_worker, daemon=True)
            self.send_thread.start()
            
            # Heartbeat 스레드 시작
            self.start_heartbeat()
            
            self.status_queue.put(('CONNECTED', f"{port} ({baudrate} bps)"))
            
            return True, "연결 성공"
            
        except Exception as e:
            self.is_connected = False
            error_msg = f"연결 오류: {str(e)}"
            self.status_queue.put(('ERROR', error_msg))
            return False, error_msg
    
    def disconnect(self):
        """시리얼 포트 연결 해제"""
        try:
            self.stop_thread = True
            self.is_connected = False
            
            # Heartbeat 중지
            self.stop_heartbeat()
            
            if self.serial_connection and self.serial_connection.is_open:
                self.serial_connection.close()
            
            self.current_port = None
            self.current_baudrate = None
            
            self.status_queue.put(('DISCONNECTED', "연결 해제됨"))
            
            return True, "연결 해제 성공"
            
        except Exception as e:
            error_msg = f"연결 해제 오류: {str(e)}"
            self.status_queue.put(('ERROR', error_msg))
            return False, error_msg
    
    def start_heartbeat(self):
        """Heartbeat 전송 시작 (CMD 0x0F)"""
        self.heartbeat_active = True
        self.heartbeat_thread = threading.Thread(target=self._heartbeat_worker, daemon=True)
        self.heartbeat_thread.start()
        self.status_queue.put(('SYSTEM', "Heartbeat 시작 (CMD 0x0F, 100ms 간격)"))
    
    def stop_heartbeat(self):
        """Heartbeat 전송 중지"""
        self.heartbeat_active = False
        if self.heartbeat_thread and self.heartbeat_thread.is_alive():
            self.heartbeat_thread.join(timeout=1.0)
        self.status_queue.put(('SYSTEM', "Heartbeat 중지"))
    
    def _heartbeat_worker(self):
        """Heartbeat 전송 작업자"""
        while self.heartbeat_active and self.is_connected:
            try:
                packet = self.protocol.create_heartbeat_packet()
                self.send_queue.put(packet)
                time.sleep(self.heartbeat_interval)
            except Exception as e:
                if self.heartbeat_active:
                    self.status_queue.put(('ERROR', f"Heartbeat 오류: {str(e)}"))
                break
    
    def send_packet(self, cmd, data_field=None, tx_id=None, rx_id=None):
        """프로토콜 패킷 전송"""
        if not self.is_connected:
            return False, "연결되지 않음"
        
        try:
            if tx_id is None:
                tx_id = self.protocol.PC_ID
            if rx_id is None:
                rx_id = self.protocol.MAIN_ID
            
            packet = self.protocol.create_packet(tx_id, rx_id, cmd, data_field)
            self.send_queue.put(packet)
            return True, "패킷 전송 대기열 추가"
        except Exception as e:
            return False, f"패킷 생성 오류: {str(e)}"
    
    def _receive_worker(self):
        """데이터 수신 스레드"""
        while not self.stop_thread and self.is_connected:
            try:
                if self.serial_connection and self.serial_connection.in_waiting > 0:
                    data = self.serial_connection.read(self.serial_connection.in_waiting)
                    
                    # 프로토콜 패킷 파싱
                    packets = self.protocol.process_received_data(data)
                    
                    for packet_info in packets:
                        self.receive_queue.put(('PACKET', packet_info))
                
                time.sleep(0.01)
                
            except Exception as e:
                if self.is_connected:
                    self.receive_queue.put(('ERROR', f"수신 오류: {str(e)}"))
                break
    
    def _send_worker(self):
        """데이터 송신 스레드"""
        while not self.stop_thread and self.is_connected:
            try:
                data = self.send_queue.get(timeout=0.1)
                
                if self.serial_connection and self.serial_connection.is_open:
                    self.serial_connection.write(data)
                    self.receive_queue.put(('SENT', data))
                
            except queue.Empty:
                continue
            except Exception as e:
                if self.is_connected:
                    self.receive_queue.put(('ERROR', f"송신 오류: {str(e)}"))
                break
    
    def get_received_data(self):
        """수신된 데이터 가져오기"""
        received_data = []
        try:
            while True:
                data = self.receive_queue.get_nowait()
                received_data.append(data)
        except queue.Empty:
            pass
        return received_data
    
    def get_status_updates(self):
        """상태 업데이트 가져오기"""
        status_updates = []
        try:
            while True:
                status = self.status_queue.get_nowait()
                status_updates.append(status)
        except queue.Empty:
            pass
        return status_updates


class DataParser:
    """시리얼 데이터 파싱 클래스"""
    # 프로토콜 상수
    STX = 0x02
    ETX = 0x03
    
    # Device IDs
    PC_ID = 0x01
    MAIN_ID = 0x02
    FRONT_ID = 0x03
    @staticmethod
    def parse_valve_status(data_string):
        """밸브 상태 파싱"""
        valve_updates = {
            'nos_valves': {},
            'feed_valves': {}
        }
        try:
            import re
            data_upper = data_string.upper()
            
            nos_pattern = r'NOS(\d+):([01])'
            nos_matches = re.findall(nos_pattern, data_upper)
            for valve_num, state in nos_matches:
                valve_num = int(valve_num)
                if 1 <= valve_num <= 5:
                    valve_updates['nos_valves'][valve_num] = (state == '1')
            
            feed_pattern = r'FEED(\d+):([01])'
            feed_matches = re.findall(feed_pattern, data_upper)
            for valve_num, state in feed_matches:
                valve_num = int(valve_num)
                if 1 <= valve_num <= 15:
                    valve_updates['feed_valves'][valve_num] = (state == '1')
        
        except Exception:
            pass
        
        return valve_updates
    
    @staticmethod
    def parse_system_status(data_string):
        """시스템 상태 파싱"""
        system_updates = {
            'hvac': {},
            'cooling': {},
            'icemaking': {},
            'drain_tank': {},
            'drain_pump': {}
        }
        # 파싱 로직은 기존과 동일
        return system_updates
    
    @staticmethod
    def parse_sensor_data(data_string):
        """센서 데이터 파싱"""
        sensor_data = {}
        # 파싱 로직은 기존과 동일
        return sensor_data
    
    def __init__(self):
        self.receive_buffer = bytearray()
    
    def calculate_crc16(self, data):
        """CRC16-CCITT 계산 (STX ~ DATA FIELD)"""
        crc = 0xFFFF
        for byte in data:
            crc ^= byte << 8
            for _ in range(8):
                if crc & 0x8000:
                    crc = (crc << 1) ^ 0x1021
                else:
                    crc = crc << 1
                crc &= 0xFFFF
        return crc
    
    def create_packet(self, tx_id, rx_id, cmd, data_field=None):
        """
        프로토콜 패킷 생성
        STX + TX ID + RX ID + CMD + DATA FIELD LENGTH + DATA FIELD + CRC16 + ETX
        """
        if cmd not in self.CMD_LENGTH_MAP:
            raise ValueError(f"지원하지 않는 CMD: 0x{cmd:02X}")
        
        expected_length = self.CMD_LENGTH_MAP[cmd]
        
        # DATA FIELD 검증
        if data_field is None:
            data_field = bytes(expected_length)
        elif len(data_field) != expected_length:
            raise ValueError(f"CMD 0x{cmd:02X}의 DATA FIELD는 {expected_length}바이트여야 합니다")
        
        # 패킷 구성
        packet = bytearray()
        packet.append(self.STX)
        packet.append(tx_id)
        packet.append(rx_id)
        packet.append(cmd)
        packet.append(expected_length)
        packet.extend(data_field)
        
        # CRC16 계산 (STX ~ DATA FIELD)
        crc = self.calculate_crc16(packet)
        packet.extend(struct.pack('>H', crc))  # Big-endian 2바이트
        
        packet.append(self.ETX)
        
        return bytes(packet)
    
    def create_heartbeat_packet(self, tx_id=PC_ID, rx_id=MAIN_ID):
        """CMD 0x0F (Heartbeat) 패킷 생성"""
        return self.create_packet(tx_id, rx_id, 0x0F)
    
    def parse_packet(self, packet_data):
        """패킷 파싱"""
        if len(packet_data) < 8:  # 최소 패킷 크기
            return None
        
        try:
            stx = packet_data[0]
            tx_id = packet_data[1]
            rx_id = packet_data[2]
            cmd = packet_data[3]
            data_length = packet_data[4]
            
            expected_total = 7 + data_length  # STX + IDs + CMD + LEN + DATA + CRC16 + ETX
            
            if len(packet_data) != expected_total:
                return None
            
            data_field = packet_data[5:5+data_length]
            crc_received = struct.unpack('>H', packet_data[5+data_length:7+data_length])[0]
            etx = packet_data[-1]
            
            # 검증
            if stx != self.STX or etx != self.ETX:
                return None
            
            # CRC 검증
            crc_data = packet_data[:-3]  # STX ~ DATA FIELD
            crc_calculated = self.calculate_crc16(crc_data)
            
            if crc_received != crc_calculated:
                return None
            
            return {
                'tx_id': tx_id,
                'rx_id': rx_id,
                'cmd': cmd,
                'data_length': data_length,
                'data_field': data_field,
                'crc': crc_received
            }
            
        except Exception:
            return None
    
    def process_received_data(self, new_data):
        """수신 버퍼에서 패킷 추출"""
        self.receive_buffer.extend(new_data)
        packets = []
        
        while len(self.receive_buffer) > 0:
            # STX 찾기
            stx_index = -1
            for i in range(len(self.receive_buffer)):
                if self.receive_buffer[i] == self.STX:
                    stx_index = i
                    break
            
            if stx_index == -1:
                self.receive_buffer.clear()
                break
            
            # STX 이전 데이터 제거
            if stx_index > 0:
                self.receive_buffer = self.receive_buffer[stx_index:]
            
            # 최소 헤더 크기 확인 (STX + TX + RX + CMD + LEN)
            if len(self.receive_buffer) < 5:
                break
            
            data_length = self.receive_buffer[4]
            expected_total = 7 + data_length
            
            if len(self.receive_buffer) < expected_total:
                break
            
            # 패킷 추출 시도
            packet_candidate = bytes(self.receive_buffer[:expected_total])
            parsed = self.parse_packet(packet_candidate)
            
            if parsed:
                packets.append(parsed)
                self.receive_buffer = self.receive_buffer[expected_total:]
            else:
                # 파싱 실패시 STX 다음부터 다시 시도
                self.receive_buffer = self.receive_buffer[1:]
        
        return packets


class SerialCommunication:
    def __init__(self):
        self.serial_connection = None
        self.is_connected = False
        self.receive_thread = None
        self.send_thread = None
        self.heartbeat_thread = None
        self.stop_thread = False
        
        # 데이터 큐들
        self.receive_queue = queue.Queue()
        self.send_queue = queue.Queue()
        self.status_queue = queue.Queue()
        
        # 연결 정보
        self.current_port = None
        self.current_baudrate = None
        
        # 프로토콜 핸들러
        self.protocol = ProtocolHandler()
        
        # Heartbeat 설정
        self.heartbeat_interval = 0.1  # 100ms
        self.heartbeat_active = False
    
    def get_available_ports(self):
        """사용 가능한 시리얼 포트 목록 반환"""
        ports = serial.tools.list_ports.comports()
        return [f"{port.device} - {port.description}" for port in ports]
    
    def check_port_availability(self, port_info):
        """포트 사용 가능성 확인"""
        try:
            port = port_info.split(" - ")[0] if " - " in port_info else port_info
            
            test_connection = serial.Serial(
                port=port,
                baudrate=115200,
                timeout=0.1
            )
            test_connection.close()
            return True, "포트 사용 가능"
            
        except PermissionError:
            return False, "포트가 다른 프로그램에서 사용 중입니다"
        except serial.SerialException as e:
            if "Access is denied" in str(e) or "Permission denied" in str(e):
                return False, "포트 접근이 거부되었습니다"
            elif "cannot find" in str(e).lower() or "not found" in str(e).lower():
                return False, "포트를 찾을 수 없습니다"
            else:
                return False, f"포트 오류: {str(e)}"
        except Exception as e:
            return False, f"알 수 없는 오류: {str(e)}"
    
    def connect(self, port_info, baudrate):
        """시리얼 포트 연결"""
        try:
            if self.is_connected:
                self.disconnect()
            
            port = port_info.split(" - ")[0] if " - " in port_info else port_info
            
            self.serial_connection = serial.Serial(
                port=port,
                baudrate=int(baudrate),
                bytesize=serial.EIGHTBITS,
                parity=serial.PARITY_NONE,
                stopbits=serial.STOPBITS_ONE,
                timeout=1
            )
            
            self.is_connected = True
            self.stop_thread = False
            self.current_port = port
            self.current_baudrate = baudrate
            
            # 수신 스레드 시작
            self.receive_thread = threading.Thread(target=self._receive_worker, daemon=True)
            self.receive_thread.start()
            
            # 송신 스레드 시작
            self.send_thread = threading.Thread(target=self._send_worker, daemon=True)
            self.send_thread.start()
            
            # Heartbeat 스레드 시작
            self.start_heartbeat()
            
            self.status_queue.put(('CONNECTED', f"{port} ({baudrate} bps)"))
            
            return True, "연결 성공"
            
        except Exception as e:
            self.is_connected = False
            error_msg = f"연결 오류: {str(e)}"
            self.status_queue.put(('ERROR', error_msg))
            return False, error_msg
    
    def disconnect(self):
        """시리얼 포트 연결 해제"""
        try:
            self.stop_thread = True
            self.is_connected = False
            
            # Heartbeat 중지
            self.stop_heartbeat()
            
            if self.serial_connection and self.serial_connection.is_open:
                self.serial_connection.close()
            
            self.current_port = None
            self.current_baudrate = None
            
            self.status_queue.put(('DISCONNECTED', "연결 해제됨"))
            
            return True, "연결 해제 성공"
            
        except Exception as e:
            error_msg = f"연결 해제 오류: {str(e)}"
            self.status_queue.put(('ERROR', error_msg))
            return False, error_msg
    
    def start_heartbeat(self):
        """Heartbeat 전송 시작 (CMD 0x0F)"""
        self.heartbeat_active = True
        self.heartbeat_thread = threading.Thread(target=self._heartbeat_worker, daemon=True)
        self.heartbeat_thread.start()
        self.status_queue.put(('SYSTEM', "Heartbeat 시작 (CMD 0x0F, 100ms 간격)"))
    
    def stop_heartbeat(self):
        """Heartbeat 전송 중지"""
        self.heartbeat_active = False
        if self.heartbeat_thread and self.heartbeat_thread.is_alive():
            self.heartbeat_thread.join(timeout=1.0)
        self.status_queue.put(('SYSTEM', "Heartbeat 중지"))
    
    def _heartbeat_worker(self):
        """Heartbeat 전송 작업자"""
        while self.heartbeat_active and self.is_connected:
            try:
                packet = self.protocol.create_heartbeat_packet()
                self.send_queue.put(packet)
                time.sleep(self.heartbeat_interval)
            except Exception as e:
                if self.heartbeat_active:
                    self.status_queue.put(('ERROR', f"Heartbeat 오류: {str(e)}"))
                break
    
    def send_packet(self, cmd, data_field=None, tx_id=None, rx_id=None):
        """프로토콜 패킷 전송"""
        if not self.is_connected:
            return False, "연결되지 않음"
        
        try:
            if tx_id is None:
                tx_id = self.protocol.PC_ID
            if rx_id is None:
                rx_id = self.protocol.MAIN_ID
            
            packet = self.protocol.create_packet(tx_id, rx_id, cmd, data_field)
            self.send_queue.put(packet)
            return True, "패킷 전송 대기열 추가"
        except Exception as e:
            return False, f"패킷 생성 오류: {str(e)}"
    
    def _receive_worker(self):
        """데이터 수신 스레드"""
        while not self.stop_thread and self.is_connected:
            try:
                if self.serial_connection and self.serial_connection.in_waiting > 0:
                    data = self.serial_connection.read(self.serial_connection.in_waiting)
                    
                    # 프로토콜 패킷 파싱
                    packets = self.protocol.process_received_data(data)
                    
                    for packet_info in packets:
                        self.receive_queue.put(('PACKET', packet_info))
                
                time.sleep(0.01)
                
            except Exception as e:
                if self.is_connected:
                    self.receive_queue.put(('ERROR', f"수신 오류: {str(e)}"))
                break
    
    def _send_worker(self):
        """데이터 송신 스레드"""
        while not self.stop_thread and self.is_connected:
            try:
                data = self.send_queue.get(timeout=0.1)
                
                if self.serial_connection and self.serial_connection.is_open:
                    self.serial_connection.write(data)
                    self.receive_queue.put(('SENT', data))
                
            except queue.Empty:
                continue
            except Exception as e:
                if self.is_connected:
                    self.receive_queue.put(('ERROR', f"송신 오류: {str(e)}"))
                break
    
    def get_received_data(self):
        """수신된 데이터 가져오기"""
        received_data = []
        try:
            while True:
                data = self.receive_queue.get_nowait()
                received_data.append(data)
        except queue.Empty:
            pass
        return received_data
    
    def get_status_updates(self):
        """상태 업데이트 가져오기"""
        status_updates = []
        try:
            while True:
                status = self.status_queue.get_nowait()
                status_updates.append(status)
        except queue.Empty:
            pass
        return status_updates


class DataParser:
    """시리얼 데이터 파싱 클래스"""
    
    @staticmethod
    def parse_valve_status(data_string):
        """밸브 상태 파싱"""
        valve_updates = {
            'nos_valves': {},
            'feed_valves': {}
        }
        try:
            import re
            data_upper = data_string.upper()
            
            nos_pattern = r'NOS(\d+):([01])'
            nos_matches = re.findall(nos_pattern, data_upper)
            for valve_num, state in nos_matches:
                valve_num = int(valve_num)
                if 1 <= valve_num <= 5:
                    valve_updates['nos_valves'][valve_num] = (state == '1')
            
            feed_pattern = r'FEED(\d+):([01])'
            feed_matches = re.findall(feed_pattern, data_upper)
            for valve_num, state in feed_matches:
                valve_num = int(valve_num)
                if 1 <= valve_num <= 15:
                    valve_updates['feed_valves'][valve_num] = (state == '1')
        
        except Exception:
            pass
        
        return valve_updates
    
    @staticmethod
    def parse_system_status(data_string):
        """시스템 상태 파싱"""
        system_updates = {
            'hvac': {},
            'cooling': {},
            'icemaking': {},
            'drain_tank': {},
            'drain_pump': {}
        }
        # 파싱 로직은 기존과 동일
        return system_updates
    
    @staticmethod
    def parse_sensor_data(data_string):
        """센서 데이터 파싱"""
        sensor_data = {}
        # 파싱 로직은 기존과 동일
        return sensor_data