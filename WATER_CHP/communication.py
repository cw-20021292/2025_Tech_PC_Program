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
    
    # CMD와 DATA FIELD LENGTH 매핑 (PC → 메인)
    CMD_LENGTH_MAP = {
        0x0F: 0,   # Heartbeat
        0xF0: 0,   # 상태조회 (POLLING)
        0xA0: 20,  # 밸브 부하 변경 (DATA1~5: NOS 밸브, DATA6~20: FEED 밸브)
        0xA1: 1,   # 드레인 펌프 출력 변경
        0xB0: 4,   # 공조시스템
        0xB1: 4,   # 냉각운전 변경 (TARGET RPS, TARGET TEMP, 냉각 동작, 냉각 ON 온도)
        0xB2: 7,   # 제빙운전 변경 (TARGET RPS, TARGET TEMP, 제빙 동작, 제빙시간, 입수용량)
        0xB3: 93,  # 제빙테이블 적용 (DATA1: 행인덱스 1바이트, DATA2~93: 테이블 46개x2바이트)
        0xB4: 4,   # 보냉운전 변경 (TARGET RPS, TARGET TEMP, TARGET TEMP FIRST, TRAY POSITION)
        0xC0: 7    # 센서값 변경
    }
    
    def __init__(self):
        self.receive_buffer = bytearray()
    
    @staticmethod
    def int_to_signed_byte(value):
        """
        정수를 signed byte로 변환 (부호-크기 표현)
        1바이트에서 최상위 비트(MSB)가 1이면 음수, 0이면 양수
        범위: -127 ~ 127 (0은 양수로 처리)
        부호-크기(sign-magnitude) 표현 사용
        """
        if value < -127 or value > 127:
            raise ValueError(f"값이 signed byte 범위(-127~127)를 벗어났습니다: {value}")
        
        if value < 0:
            # 음수: MSB를 1로 설정하고 절대값을 하위 7비트에 저장
            # 예: -1 -> 0x81 (1000 0001) = 128 | 1
            # 예: -40 -> 0xA8 (1010 1000) = 128 | 40
            # 예: -80 -> 0xF0 (1111 0000) = 128 | 80
            return 0x80 | abs(value)
        else:
            # 양수: MSB가 0이므로 그대로 반환
            # 예: 1 -> 0x01 (0000 0001)
            # 예: 80 -> 0x50 (0101 0000)
            return value
    
    @staticmethod
    def signed_byte_to_int(byte_value):
        """
        signed byte를 정수로 변환 (부호-크기 표현)
        최상위 비트(MSB)가 1이면 음수, 0이면 양수
        """
        if byte_value & 0x80:
            # MSB가 1이면 음수: 하위 7비트의 절대값에 음수 부호 적용
            # 예: 0x81 -> -1, 0xA8 -> -40, 0xF0 -> -80
            return -(byte_value & 0x7F)
        else:
            # MSB가 0이면 양수: 그대로 반환
            # 예: 0x01 -> 1, 0x50 -> 80
            return byte_value
    
    def calculate_crc16(self, data):
        """
        CRC16-CCITT 계산 (STX ~ DATA FIELD)
        초기값: 0x0000
        다항식: 0x1021
        """
        wCRCin = 0x0000  # 초기값 변경: 0xFFFF -> 0x0000
        wCPoly = 0x1021
        
        for wChar in data:
            wCRCin ^= (wChar << 8)
            for i in range(8):
                if wCRCin & 0x8000:
                    wCRCin = (wCRCin << 1) ^ wCPoly
                else:
                    wCRCin = wCRCin << 1
                wCRCin &= 0xFFFF
        return wCRCin
    
    def create_packet(self, tx_id, cmd, data_field=None):
        """
        프로토콜 패킷 생성
        STX + TX ID + CMD + DATA FIELD LENGTH + DATA FIELD + CRC16 + ETX
        """
        if cmd not in self.CMD_LENGTH_MAP:
            raise ValueError(f"지원하지 않는 CMD: 0x{cmd:02X}")
        
        expected_length = self.CMD_LENGTH_MAP[cmd]
        
        # DATA FIELD 검증
        if data_field is None:
            data_field = bytes(expected_length)
        elif len(data_field) != expected_length:
            raise ValueError(f"CMD 0x{cmd:02X}의 DATA FIELD는 {expected_length}바이트여야 합니다 (현재: {len(data_field)}바이트)")
        
        # 패킷 구성 (RX ID 제거)
        packet = bytearray()
        packet.append(self.STX)
        packet.append(tx_id)
        packet.append(cmd)
        packet.append(expected_length)
        packet.extend(data_field)
        
        # CRC16 계산 (STX ~ DATA FIELD)
        crc = self.calculate_crc16(packet)
        # CRC를 ETX 앞에 CRC_HIGHBYTE와 CRC_LOWBYTE 순서로 추가
        crc_high = (crc >> 8) & 0xFF  # CRC_HIGHBYTE
        crc_low = crc & 0xFF          # CRC_LOWBYTE
        packet.append(crc_high)
        packet.append(crc_low)
        
        packet.append(self.ETX)
        
        return bytes(packet)
    
    def create_heartbeat_packet(self, tx_id=PC_ID):
        """CMD 0x0F (Heartbeat) 패킷 생성 (RX ID 제거)"""
        return self.create_packet(tx_id, 0x0F)
    
    def parse_packet(self, packet_data):
        """패킷 파싱 (RX ID 제거) - 에러 정보 포함"""
        if len(packet_data) < 7:  # 최소 패킷 크기 (RX ID 제거로 1바이트 감소)
            return {
                'error': 'PACKET_TOO_SHORT',
                'detail': f'패킷 길이 부족: {len(packet_data)}바이트 (최소 7바이트 필요)',
                'raw_data': ' '.join([f'{b:02X}' for b in packet_data])
            }
        
        try:
            stx = packet_data[0]
            tx_id = packet_data[1]
            cmd = packet_data[2]
            data_length = packet_data[3]
            
            expected_total = 6 + data_length  # STX + TX_ID + CMD + LEN + DATA + CRC16 + ETX
            
            if len(packet_data) != expected_total:
                return {
                    'error': 'LENGTH_MISMATCH',
                    'detail': f'패킷 길이 불일치: 예상 {expected_total}바이트, 실제 {len(packet_data)}바이트 (DATA_LEN={data_length})',
                    'raw_data': ' '.join([f'{b:02X}' for b in packet_data]),
                    'stx': f'0x{stx:02X}',
                    'tx_id': f'0x{tx_id:02X}',
                    'cmd': f'0x{cmd:02X}'
                }
            
            data_field = packet_data[4:4+data_length]
            # CRC_HIGHBYTE와 CRC_LOWBYTE를 읽어서 CRC 값 구성
            crc_high = packet_data[4+data_length]
            crc_low = packet_data[4+data_length+1]
            crc_received = (crc_high << 8) | crc_low
            etx = packet_data[-1]
            
            # STX 검증
            if stx != self.STX:
                return {
                    'error': 'INVALID_STX',
                    'detail': f'잘못된 STX: 0x{stx:02X} (예상: 0x{self.STX:02X})',
                    'raw_data': ' '.join([f'{b:02X}' for b in packet_data])
                }
            
            # ETX 검증
            if etx != self.ETX:
                return {
                    'error': 'INVALID_ETX',
                    'detail': f'잘못된 ETX: 0x{etx:02X} (예상: 0x{self.ETX:02X})',
                    'raw_data': ' '.join([f'{b:02X}' for b in packet_data])
                }
            
            # CRC 검증
            crc_data = packet_data[:-3]  # STX ~ DATA FIELD
            crc_calculated = self.calculate_crc16(crc_data)
            
            if crc_received != crc_calculated:
                return {
                    'error': 'CRC_MISMATCH',
                    'detail': f'CRC 불일치: 수신 0x{crc_received:04X}, 계산 0x{crc_calculated:04X}',
                    'raw_data': ' '.join([f'{b:02X}' for b in packet_data]),
                    'tx_id': f'0x{tx_id:02X}',
                    'cmd': f'0x{cmd:02X}',
                    'data_field': ' '.join([f'{b:02X}' for b in data_field])
                }
            
            # 성공
            return {
                'tx_id': tx_id,
                'cmd': cmd,
                'data_length': data_length,
                'data_field': data_field,
                'crc': crc_received
            }
            
        except Exception as e:
            return {
                'error': 'PARSE_EXCEPTION',
                'detail': f'파싱 중 예외 발생: {str(e)}',
                'raw_data': ' '.join([f'{b:02X}' for b in packet_data])
            }
    
    def process_received_data(self, new_data):
        """수신 버퍼에서 패킷 추출 - 엄격한 프로토콜 검증"""
        self.receive_buffer.extend(new_data)
        packets = []
        
        while len(self.receive_buffer) > 0:
            # 1단계: STX(0x02) 확인 - 첫 바이트가 STX가 아니면 버퍼 초기화
            if self.receive_buffer[0] != self.STX:
                invalid_byte = self.receive_buffer[0]
                buffer_preview = self.receive_buffer[:min(10, len(self.receive_buffer))]
                packets.append({
                    'error': 'INVALID_START',
                    'detail': f'통신 시작 오류: 첫 바이트가 STX(0x02)가 아님 (수신: 0x{invalid_byte:02X})',
                    'raw_data': ' '.join([f'{b:02X}' for b in buffer_preview])
                })
                # 버퍼 초기화
                self.receive_buffer.clear()
                break
            
            # 최소 헤더 크기 확인 (STX + TX_ID + CMD + DATA_LEN = 4바이트)
            if len(self.receive_buffer) < 4:
                # 아직 헤더가 다 안 옴, 대기
                break
            
            # 2단계: CMD 확인 - 세 번째 데이터가 정의된 CMD인지 확인
            stx = self.receive_buffer[0]
            tx_id = self.receive_buffer[1]
            cmd = self.receive_buffer[2]
            data_length = self.receive_buffer[3]
            
            if cmd not in self.CMD_LENGTH_MAP:
                buffer_preview = self.receive_buffer[:min(10, len(self.receive_buffer))]
                packets.append({
                    'error': 'UNDEFINED_CMD',
                    'detail': f'정의되지 않은 CMD: 0x{cmd:02X} (TX_ID: 0x{tx_id:02X})',
                    'raw_data': ' '.join([f'{b:02X}' for b in buffer_preview])
                })
                # 버퍼 초기화
                self.receive_buffer.clear()
                break
            
            # 3단계: DATA_LENGTH로 전체 패킷 길이 계산
            # 패킷 구조: STX(1) + TX_ID(1) + CMD(1) + DATA_LEN(1) + DATA(N) + CRC_HIGH(1) + CRC_LOW(1) + ETX(1)
            # 전체 길이 = 4 + DATA_LEN + 3 = 7 + DATA_LEN
            expected_total = 7 + data_length
            
            # CRC와 ETX 위치 예상
            crc_high_pos = 4 + data_length
            crc_low_pos = 5 + data_length
            etx_pos = 6 + data_length
            
            # 패킷이 완전히 도착했는지 확인
            if len(self.receive_buffer) < expected_total:
                # 아직 패킷이 다 안 옴, 대기
                break
            
            # 4단계: ETX 위치 확인
            actual_etx = self.receive_buffer[etx_pos]
            if actual_etx != self.ETX:
                packets.append({
                    'error': 'ETX_POSITION_MISMATCH',
                    'detail': f'ETX 위치 오류: 예상 위치[{etx_pos}]에 ETX(0x03) 없음 (수신: 0x{actual_etx:02X})',
                    'raw_data': ' '.join([f'{b:02X}' for b in self.receive_buffer[:expected_total]]),
                    'expected_length': expected_total,
                    'cmd': f'0x{cmd:02X}',
                    'data_length': data_length
                })
                # 버퍼 초기화
                self.receive_buffer.clear()
                break
            
            # CRC 계산 및 확인
            packet_data = self.receive_buffer[:expected_total]
            crc_data = packet_data[:crc_high_pos]  # STX ~ DATA FIELD
            crc_calculated = self.calculate_crc16(crc_data)
            
            crc_high_received = self.receive_buffer[crc_high_pos]
            crc_low_received = self.receive_buffer[crc_low_pos]
            crc_received = (crc_high_received << 8) | crc_low_received
            
            if crc_received != crc_calculated:
                packets.append({
                    'error': 'CRC_MISMATCH',
                    'detail': f'CRC 불일치: 수신 0x{crc_received:04X}, 계산 0x{crc_calculated:04X}',
                    'raw_data': ' '.join([f'{b:02X}' for b in packet_data]),
                    'tx_id': f'0x{tx_id:02X}',
                    'cmd': f'0x{cmd:02X}',
                    'data_length': data_length
                })
                # 버퍼 초기화
                self.receive_buffer.clear()
                break
            
            # 정상 패킷 - DATA FIELD 추출
            data_field = packet_data[4:4+data_length]
            
            # 성공 패킷 추가
            packets.append({
                'tx_id': tx_id,
                'cmd': cmd,
                'data_length': data_length,
                'data_field': data_field,
                'crc': crc_received
            })
            
            # 처리한 패킷만큼 버퍼에서 제거
            self.receive_buffer = self.receive_buffer[expected_total:]
        
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
        self.heartbeat_paused = False  # Heartbeat 일시 중지 플래그
    
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
                baudrate=9600,
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
    
    def pause_heartbeat(self):
        """Heartbeat 일시 중지 (CMD 0xB3 전송 중 사용)"""
        self.heartbeat_paused = True
    
    def resume_heartbeat(self):
        """Heartbeat 재개"""
        self.heartbeat_paused = False
    
    def _heartbeat_worker(self):
        """Heartbeat 전송 작업자"""
        while self.heartbeat_active and self.is_connected:
            try:
                # Heartbeat가 일시 중지되었으면 대기
                if not self.heartbeat_paused:
                    packet = self.protocol.create_heartbeat_packet()
                    self.send_queue.put(packet)
                time.sleep(self.heartbeat_interval)
            except Exception as e:
                if self.heartbeat_active:
                    self.status_queue.put(('ERROR', f"Heartbeat 오류: {str(e)}"))
                break
    
    def send_packet(self, cmd, data_field=None, tx_id=None):
        """프로토콜 패킷 전송 (RX ID 제거)"""
        if not self.is_connected:
            return False, "연결되지 않음"
        
        try:
            if tx_id is None:
                tx_id = self.protocol.PC_ID
            
            # 패킷 생성 (STX와 ETX 포함, RX ID 제거)
            packet = self.protocol.create_packet(tx_id, cmd, data_field)
            
            # 패킷 검증: STX와 ETX가 포함되어 있는지 확인
            if len(packet) < 2:
                return False, "패킷이 너무 짧습니다"
            if packet[0] != self.protocol.STX:
                return False, f"패킷 시작이 STX(0x{self.protocol.STX:02X})가 아닙니다: 0x{packet[0]:02X}"
            if packet[-1] != self.protocol.ETX:
                return False, f"패킷 끝이 ETX(0x{self.protocol.ETX:02X})가 아닙니다: 0x{packet[-1]:02X}"
            
            # 전송 대기열에 추가
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
                    
                    # RAW 데이터 로깅 (디버그용)
                    if len(data) > 0:
                        raw_hex = ' '.join([f'{b:02X}' for b in data])
                        self.receive_queue.put(('RAW_DATA', {
                            'data': data.hex().upper(),
                            'length': len(data),
                            'bytes': raw_hex
                        }))
                    
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
                    # 패킷 검증: STX와 ETX가 포함되어 있는지 확인
                    if len(data) >= 2:
                        if data[0] != self.protocol.STX:
                            self.receive_queue.put(('ERROR', f"전송 패킷 시작이 STX가 아닙니다: 0x{data[0]:02X}"))
                        elif data[-1] != self.protocol.ETX:
                            self.receive_queue.put(('ERROR', f"전송 패킷 끝이 ETX가 아닙니다: 0x{data[-1]:02X}"))
                        else:
                            # STX와 ETX가 포함된 전체 패킷 전송
                            self.serial_connection.write(data)
                            self.receive_queue.put(('SENT', data))
                    else:
                        self.receive_queue.put(('ERROR', f"전송 패킷이 너무 짧습니다: {len(data)}바이트"))
                
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
        """
        CRC16-CCITT 계산 (STX ~ DATA FIELD)
        초기값: 0x0000
        다항식: 0x1021
        """
        wCRCin = 0x0000  # 초기값 변경: 0xFFFF -> 0x0000
        wCPoly = 0x1021
        
        for wChar in data:
            wCRCin ^= (wChar << 8)
            for i in range(8):
                if wCRCin & 0x8000:
                    wCRCin = (wCRCin << 1) ^ wCPoly
                else:
                    wCRCin = wCRCin << 1
                wCRCin &= 0xFFFF
        return wCRCin
    
    def create_packet(self, tx_id, cmd, data_field=None):
        """
        프로토콜 패킷 생성
        STX + TX ID + CMD + DATA FIELD LENGTH + DATA FIELD + CRC16 + ETX
        """
        if cmd not in self.CMD_LENGTH_MAP:
            raise ValueError(f"지원하지 않는 CMD: 0x{cmd:02X}")
        
        expected_length = self.CMD_LENGTH_MAP[cmd]
        
        # DATA FIELD 검증
        if data_field is None:
            data_field = bytes(expected_length)
        elif len(data_field) != expected_length:
            raise ValueError(f"CMD 0x{cmd:02X}의 DATA FIELD는 {expected_length}바이트여야 합니다")
        
        # 패킷 구성 (RX ID 제거)
        packet = bytearray()
        packet.append(self.STX)
        packet.append(tx_id)
        packet.append(cmd)
        packet.append(expected_length)
        packet.extend(data_field)
        
        # CRC16 계산 (STX ~ DATA FIELD)
        crc = self.calculate_crc16(packet)
        # CRC를 ETX 앞에 CRC_HIGHBYTE와 CRC_LOWBYTE 순서로 추가
        crc_high = (crc >> 8) & 0xFF  # CRC_HIGHBYTE
        crc_low = crc & 0xFF          # CRC_LOWBYTE
        packet.append(crc_high)
        packet.append(crc_low)
        
        packet.append(self.ETX)
        
        return bytes(packet)
    
    def create_heartbeat_packet(self, tx_id=PC_ID):
        """CMD 0x0F (Heartbeat) 패킷 생성 (RX ID 제거)"""
        return self.create_packet(tx_id, 0x0F)
    
    def parse_packet(self, packet_data):
        """패킷 파싱 (RX ID 제거)"""
        if len(packet_data) < 7:  # 최소 패킷 크기 (RX ID 제거로 1바이트 감소)
            return None
        
        try:
            stx = packet_data[0]
            tx_id = packet_data[1]
            cmd = packet_data[2]
            data_length = packet_data[3]
            
            expected_total = 6 + data_length  # STX + TX_ID + CMD + LEN + DATA + CRC16 + ETX
            
            if len(packet_data) != expected_total:
                return None
            
            data_field = packet_data[4:4+data_length]
            # CRC_HIGHBYTE와 CRC_LOWBYTE를 읽어서 CRC 값 구성
            crc_high = packet_data[4+data_length]
            crc_low = packet_data[4+data_length+1]
            crc_received = (crc_high << 8) | crc_low
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
        self.heartbeat_paused = False  # Heartbeat 일시 중지 플래그
    
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
    
    def pause_heartbeat(self):
        """Heartbeat 일시 중지 (CMD 0xB3 전송 중 사용)"""
        self.heartbeat_paused = True
    
    def resume_heartbeat(self):
        """Heartbeat 재개"""
        self.heartbeat_paused = False
    
    def _heartbeat_worker(self):
        """Heartbeat 전송 작업자"""
        while self.heartbeat_active and self.is_connected:
            try:
                # Heartbeat가 일시 중지되었으면 대기
                if not self.heartbeat_paused:
                    packet = self.protocol.create_heartbeat_packet()
                    self.send_queue.put(packet)
                time.sleep(self.heartbeat_interval)
            except Exception as e:
                if self.heartbeat_active:
                    self.status_queue.put(('ERROR', f"Heartbeat 오류: {str(e)}"))
                break
    
    def send_packet(self, cmd, data_field=None, tx_id=None):
        """프로토콜 패킷 전송 (RX ID 제거)"""
        if not self.is_connected:
            return False, "연결되지 않음"
        
        try:
            if tx_id is None:
                tx_id = self.protocol.PC_ID
            
            # 패킷 생성 (STX와 ETX 포함, RX ID 제거)
            packet = self.protocol.create_packet(tx_id, cmd, data_field)
            
            # 패킷 검증: STX와 ETX가 포함되어 있는지 확인
            if len(packet) < 2:
                return False, "패킷이 너무 짧습니다"
            if packet[0] != self.protocol.STX:
                return False, f"패킷 시작이 STX(0x{self.protocol.STX:02X})가 아닙니다: 0x{packet[0]:02X}"
            if packet[-1] != self.protocol.ETX:
                return False, f"패킷 끝이 ETX(0x{self.protocol.ETX:02X})가 아닙니다: 0x{packet[-1]:02X}"
            
            # 전송 대기열에 추가
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
                    
                    # RAW 데이터 로깅 (디버그용)
                    if len(data) > 0:
                        raw_hex = ' '.join([f'{b:02X}' for b in data])
                        self.receive_queue.put(('RAW_DATA', {
                            'data': data.hex().upper(),
                            'length': len(data),
                            'bytes': raw_hex
                        }))
                    
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
                    # 패킷 검증: STX와 ETX가 포함되어 있는지 확인
                    if len(data) >= 2:
                        if data[0] != self.protocol.STX:
                            self.receive_queue.put(('ERROR', f"전송 패킷 시작이 STX가 아닙니다: 0x{data[0]:02X}"))
                        elif data[-1] != self.protocol.ETX:
                            self.receive_queue.put(('ERROR', f"전송 패킷 끝이 ETX가 아닙니다: 0x{data[-1]:02X}"))
                        else:
                            # STX와 ETX가 포함된 전체 패킷 전송
                            self.serial_connection.write(data)
                            self.receive_queue.put(('SENT', data))
                    else:
                        self.receive_queue.put(('ERROR', f"전송 패킷이 너무 짧습니다: {len(data)}바이트"))
                
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