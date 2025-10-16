"""
USB TO SERIAL 통신 모듈
"""
import serial
import serial.tools.list_ports
import threading
import time
import queue
import re
from datetime import datetime


class SerialCommunication:
    def __init__(self):
        self.serial_connection = None
        self.is_connected = False
        self.receive_thread = None
        self.stop_thread = False
        
        # 데이터 큐들 (스레드 간 안전한 통신)
        self.receive_queue = queue.Queue()  # 수신 데이터
        self.send_queue = queue.Queue()     # 송신 대기 데이터
        self.status_queue = queue.Queue()   # 연결 상태 변경
        
        # 현재 연결 정보
        self.current_port = None
        self.current_baudrate = None
        
        # 주기적 전송 관련
        self.periodic_send_thread = None
        self.periodic_send_active = False
        self.periodic_send_interval = 0.1  # 100ms
        
        # 프로토콜 설정
        self.STX = 0x02  # Start of Text
        self.ETX = 0x03  # End of Text
        self.PACKET_SIZE = 20  # 고정 패킷 크기
    
    def get_available_ports(self):
        """사용 가능한 시리얼 포트 목록 반환"""
        ports = serial.tools.list_ports.comports()
        return [f"{port.device} - {port.description}" for port in ports]
    
    def check_port_availability(self, port_info):
        """포트 사용 가능성 확인"""
        try:
            port = port_info.split(" - ")[0] if " - " in port_info else port_info
            
            # 임시로 포트를 열어서 사용 가능한지 확인
            test_connection = serial.Serial(
                port=port,
                baudrate=115200,  # 기본값으로 테스트
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
            
            # 포트명 추출
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
            
            # 상태 변경 알림
            self.status_queue.put(('CONNECTED', f"{port} ({baudrate} bps)"))
            
            # 1초 후 주기적 전송 시작
            # threading.Timer(1.0, self.start_periodic_send).start()
            
            return True, "연결 성공"
            
        except PermissionError as e:
            self.is_connected = False
            error_msg = f"포트 접근 권한 오류: {port}\n다른 프로그램에서 사용 중이거나 권한이 없습니다.\n해결 방법:\n1. 다른 시리얼 프로그램을 종료하세요\n2. 케이블을 재연결하세요\n3. 관리자 권한으로 실행하세요"
            self.status_queue.put(('ERROR', error_msg))
            return False, error_msg
        except serial.SerialException as e:
            self.is_connected = False
            if "Access is denied" in str(e) or "Permission denied" in str(e):
                error_msg = f"포트 접근 거부: {port}\n포트가 이미 사용 중입니다.\n다른 프로그램을 종료한 후 다시 시도하세요."
            elif "cannot find" in str(e).lower() or "not found" in str(e).lower():
                error_msg = f"포트를 찾을 수 없음: {port}\n장치가 연결되어 있는지 확인하고 포트를 새로고침하세요."
            elif "exists" in str(e).lower():
                error_msg = f"잘못된 포트: {port}\n올바른 포트를 선택하세요."
            else:
                error_msg = f"시리얼 포트 오류: {str(e)}"
            self.status_queue.put(('ERROR', error_msg))
            return False, error_msg
        except ValueError as e:
            self.is_connected = False
            error_msg = f"설정 값 오류: {str(e)}\n통신속도를 확인하세요."
            self.status_queue.put(('ERROR', error_msg))
            return False, error_msg
        except Exception as e:
            self.is_connected = False
            error_msg = f"예상치 못한 연결 오류: {str(e)}"
            self.status_queue.put(('ERROR', error_msg))
            return False, error_msg
    
    def disconnect(self):
        """시리얼 포트 연결 해제"""
        try:
            self.stop_thread = True
            self.is_connected = False
            
            # 주기적 전송 중지
            # self.stop_periodic_send()
            
            if self.serial_connection and self.serial_connection.is_open:
                self.serial_connection.close()
            
            self.current_port = None
            self.current_baudrate = None
            
            # 상태 변경 알림
            self.status_queue.put(('DISCONNECTED', "연결 해제됨"))
            
            return True, "연결 해제 성공"
            
        except Exception as e:
            error_msg = f"연결 해제 오류: {str(e)}"
            self.status_queue.put(('ERROR', error_msg))
            return False, error_msg
    
    def send_data(self, data, hex_mode=True):
        """데이터 송신 큐에 추가"""
        if not self.is_connected:
            return False, "연결되지 않음"
        
        try:
            if isinstance(data, bytes):
                # 이미 바이트 데이터인 경우 그대로 사용
                bytes_data = data
            elif hex_mode:
                # HEX 모드
                hex_values = data.replace(" ", "")
                if len(hex_values) % 2 != 0:
                    return False, "HEX 데이터는 짝수 개의 문자여야 합니다"
                bytes_data = bytes.fromhex(hex_values)
            else:
                # 텍스트 모드 (문자열을 바이트로 변환)
                bytes_data = data.encode('utf-8')
            
            self.send_queue.put(bytes_data)
            return True, "송신 대기열에 추가됨"
            
        except Exception as e:
            return False, f"송신 데이터 처리 오류: {str(e)}"
    
    def _receive_worker(self):
        """데이터 수신 스레드"""
        while not self.stop_thread and self.is_connected:
            try:
                if self.serial_connection and self.serial_connection.in_waiting > 0:
                    data = self.serial_connection.read(self.serial_connection.in_waiting)
                    
                    # 수신 큐에 데이터 추가
                    self.receive_queue.put(('DATA', data))
                
                time.sleep(0.01)  # CPU 사용률 조절
                
            except Exception as e:
                if self.is_connected:
                    self.receive_queue.put(('ERROR', f"수신 오류: {str(e)}"))
                break
    
    def _send_worker(self):
        """데이터 송신 스레드"""
        while not self.stop_thread and self.is_connected:
            try:
                # 송신 큐에서 데이터 가져오기 (타임아웃 설정)
                data = self.send_queue.get(timeout=0.1)
                
                if self.serial_connection and self.serial_connection.is_open:
                    self.serial_connection.write(data)
                    
                    # 송신 완료 알림
                    self.receive_queue.put(('SENT', data))
                
            except queue.Empty:
                continue
            except Exception as e:
                if self.is_connected:
                    self.receive_queue.put(('ERROR', f"송신 오류: {str(e)}"))
                break
    
    def get_received_data(self):
        """수신된 데이터 가져오기 (논블로킹)"""
        received_data = []
        try:
            while True:
                data = self.receive_queue.get_nowait()
                received_data.append(data)
        except queue.Empty:
            pass
        
        return received_data
    
    def get_status_updates(self):
        """상태 업데이트 가져오기 (논블로킹)"""
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
        """밸브 상태 파싱 (NOS 1~5, FEED 1~15)"""
        valve_updates = {
            'nos_valves': {},
            'feed_valves': {}
        }
        try:
            data_upper = data_string.upper()
            
            # NOS 밸브 패턴: NOS1:1, NOS2:0 형태 (1=CLOSE, 0=OPEN)
            nos_pattern = r'NOS(\d+):([01])'
            nos_matches = re.findall(nos_pattern, data_upper)
            for valve_num, state in nos_matches:
                valve_num = int(valve_num)
                if 1 <= valve_num <= 5:
                    valve_updates['nos_valves'][valve_num] = (state == '1')  # 1이면 True(CLOSE)
            
            # FEED 밸브 패턴: FEED1:1, FEED2:0 형태 (1=OPEN, 0=CLOSE)
            feed_pattern = r'FEED(\d+):([01])'
            feed_matches = re.findall(feed_pattern, data_upper)
            for valve_num, state in feed_matches:
                valve_num = int(valve_num)
                if 1 <= valve_num <= 15:
                    valve_updates['feed_valves'][valve_num] = (state == '1')  # 1이면 True(OPEN)
        
        except Exception:
            pass
        
        return valve_updates
    
    @staticmethod
    def parse_system_status(data_string):
        """시스템 상태 파싱 (공조시스템, 냉각, 제빙, 드레인)"""
        system_updates = {
            'hvac': {},
            'cooling': {},
            'icemaking': {},
            'drain_tank': {},
            'drain_pump': {}
        }
        try:
            data_upper = data_string.upper()
            
            # 공조시스템 (HVAC) 패턴들
            hvac_patterns = [
                (r'HVAC_REFRIGERANT_VALVE_STATE:(핫가스|냉각|제빙)', 'refrigerant_valve_state'),
                (r'HVAC_REFRIGERANT_VALVE_TARGET:(핫가스|냉각|제빙)', 'refrigerant_valve_target'),
                (r'HVAC_COMPRESSOR_STATE:(동작중|미동작)', 'compressor_state'),
                (r'HVAC_CURRENT_RPS:(\d+)', 'current_rps'),
                (r'HVAC_TARGET_RPS:(\d+)', 'target_rps'),
                (r'HVAC_ERROR_CODE:(\d+)', 'error_code'),
                (r'HVAC_DC_FAN1:(ON|OFF)', 'dc_fan1'),
                (r'HVAC_DC_FAN2:(ON|OFF)', 'dc_fan2')
            ]
            
            for pattern, key in hvac_patterns:
                match = re.search(pattern, data_upper)
                if match:
                    value = match.group(1)
                    if key in ['current_rps', 'target_rps', 'error_code']:
                        value = int(value)
                    system_updates['hvac'][key] = value
            
            # 냉각시스템 패턴들
            cooling_patterns = [
                (r'COOLING_OPERATION_STATE:(GOING|STOP)', 'operation_state'),
                (r'COOLING_ON_TEMP:(\d+)', 'on_temp'),
                (r'COOLING_OFF_TEMP:(\d+)', 'off_temp'),
                (r'COOLING_COOLING_ADDITIONAL_TIME:(\d+)', 'cooling_additional_time')
            ]
            
            for pattern, key in cooling_patterns:
                match = re.search(pattern, data_upper)
                if match:
                    value = match.group(1)
                    if key in ['on_temp', 'off_temp', 'cooling_additional_time']:
                        value = int(value)
                    system_updates['cooling'][key] = value
            
            # 제빙시스템 패턴들
            icemaking_patterns = [
                (r'ICE_OPERATION:(.+)', 'operation'),
                (r'ICE_ICEMAKING_TIME:(\d+)', 'icemaking_time'),
                (r'ICE_WATER_CAPACITY:(\d+)', 'water_capacity'),
                (r'ICE_SWING_ON_TIME:(\d+)', 'swing_on_time'),
                (r'ICE_SWING_OFF_TIME:(\d+)', 'swing_off_time')
            ]
            
            for pattern, key in icemaking_patterns:
                match = re.search(pattern, data_upper)
                if match:
                    value = match.group(1)
                    if key in ['icemaking_time', 'water_capacity', 'swing_on_time', 'swing_off_time']:
                        value = int(value)
                    system_updates['icemaking'][key] = value
            
            # 드레인탱크 패턴들
            drain_tank_patterns = [
                (r'DRAIN_TANK_LOW_LEVEL:(감지|미감지)', 'low_level'),
                (r'DRAIN_TANK_HIGH_LEVEL:(감지|미감지)', 'high_level'),
                (r'DRAIN_TANK_WATER_LEVEL_STATE:(만수위|저수위|비어있음)', 'water_level_state')
            ]
            
            for pattern, key in drain_tank_patterns:
                match = re.search(pattern, data_upper)
                if match:
                    system_updates['drain_tank'][key] = match.group(1)
            
            # 드레인펌프 패턴들
            pump_pattern = r'DRAIN_PUMP_OPERATION_STATE:(ON|OFF)'
            pump_match = re.search(pump_pattern, data_upper)
            if pump_match:
                system_updates['drain_pump']['operation_state'] = pump_match.group(1)
        
        except Exception:
            pass
        
        return system_updates
    
    @staticmethod
    def parse_sensor_data(data_string):
        """센서 데이터 파싱 (온도, 압력 등 그래프용)"""
        sensor_data = {}
        try:
            data_upper = data_string.upper()
            
            # 온도 센서들
            temp_patterns = [
                (r'OUTDOOR_TEMP1:(-?\d+\.?\d*)', 'outdoor_temp1'),
                (r'OUTDOOR_TEMP2:(-?\d+\.?\d*)', 'outdoor_temp2'),
                (r'PURIFIED_TEMP:(-?\d+\.?\d*)', 'purified_temp'),
                (r'COLD_TEMP:(-?\d+\.?\d*)', 'cold_temp'),
                (r'HOT_INLET_TEMP:(-?\d+\.?\d*)', 'hot_inlet_temp'),
                (r'HOT_INTERNAL_TEMP:(-?\d+\.?\d*)', 'hot_internal_temp'),
                (r'HOT_OUTLET_TEMP:(-?\d+\.?\d*)', 'hot_outlet_temp'),
                # 그래프용 기존 패턴들도 유지
                (r'TEMP1:(-?\d+\.?\d*)', 'temp1'),
                (r'TEMP2:(-?\d+\.?\d*)', 'temp2'),
                (r'HOT_TEMP:(-?\d+\.?\d*)', 'hot_temp'),
            ]
            
            for pattern, key in temp_patterns:
                match = re.search(pattern, data_upper)
                if match:
                    sensor_data[key] = float(match.group(1))
            
            # 압력 센서들
            pressure_patterns = [
                (r'PRESSURE1:(-?\d+\.?\d*)', 'pressure1'),
                (r'PRESSURE2:(-?\d+\.?\d*)', 'pressure2'),
            ]
            
            for pattern, key in pressure_patterns:
                match = re.search(pattern, data_upper)
                if match:
                    sensor_data[key] = float(match.group(1))
        
        except Exception:
            pass
        
        return sensor_data
    
    def create_protocol_packet(self, data_bytes):
        """프로토콜 패킷 생성 (STX + 데이터 + ETX)"""
        if len(data_bytes) > self.PACKET_SIZE - 2:  # STX, ETX 제외
            raise ValueError(f"데이터 크기가 {self.PACKET_SIZE - 2}바이트를 초과합니다.")
        
        # 패킷 구성: STX + 데이터 + 패딩 + ETX
        packet = bytearray()
        packet.append(self.STX)
        packet.extend(data_bytes)
        
        # 패딩 추가 (20바이트 맞추기)
        padding_size = self.PACKET_SIZE - len(packet) - 1  # ETX 공간 제외
        packet.extend([0x00] * padding_size)
        packet.append(self.ETX)
        
        return bytes(packet)
    
    def generate_test_data(self):
        """테스트용 센서 데이터 생성"""
        import random
        
        # 밸브 상태 (NOS: 1~5, FEED: 1~15)
        nos_states = [random.randint(0, 1) for _ in range(5)]
        feed_states = [random.randint(0, 1) for _ in range(15)]
        
        # 센서 데이터
        outdoor_temp1 = random.uniform(-10, 40)
        outdoor_temp2 = random.uniform(-10, 40)
        cold_temp = random.uniform(0, 10)
        hot_temp = random.uniform(70, 95)
        
        # 냉각 시스템 상태
        cooling_state = random.choice(['STOP', 'GOING'])
        cooling_on_temp = random.uniform(5, 15)
        cooling_off_temp = random.uniform(3, 10)
        
        # 제빙 시스템 상태
        ice_operation = random.choice(['대기', '제빙', '탈빙'])
        ice_time = random.randint(0, 1800)
        ice_capacity = random.randint(0, 3000)
        
        # 드레인 시스템
        drain_low = random.choice(['감지', '미감지'])
        drain_high = random.choice(['감지', '미감지'])
        drain_pump = random.choice(['ON', 'OFF'])
        
        # 데이터 문자열 구성
        data_str = f"NOS:{','.join(map(str, nos_states))};FEED:{','.join(map(str, feed_states))};"
        data_str += f"TEMP1:{outdoor_temp1:.1f};TEMP2:{outdoor_temp2:.1f};COLD:{cold_temp:.1f};HOT:{hot_temp:.1f};"
        data_str += f"COOL:{cooling_state};CON:{cooling_on_temp:.1f};COFF:{cooling_off_temp:.1f};"
        data_str += f"ICE:{ice_operation};ITIME:{ice_time};ICAP:{ice_capacity};"
        data_str += f"DLOW:{drain_low};DHIGH:{drain_high};DPUMP:{drain_pump}"
        
        return data_str.encode('utf-8')
    
    def start_periodic_send(self):
        """주기적 전송 시작"""
        if not self.is_connected:
            return
            
        self.periodic_send_active = True
        self.periodic_send_thread = threading.Thread(target=self._periodic_send_worker, daemon=True)
        self.periodic_send_thread.start()
        
        # 시작 알림
        self.status_queue.put(('SYSTEM', "주기적 데이터 전송 시작 (100ms 간격)"))
    
    def stop_periodic_send(self):
        """주기적 전송 중지"""
        self.periodic_send_active = False
        if self.periodic_send_thread and self.periodic_send_thread.is_alive():
            self.periodic_send_thread.join(timeout=1.0)
        
        # 중지 알림
        if hasattr(self, 'status_queue'):
            self.status_queue.put(('SYSTEM', "주기적 데이터 전송 중지"))
    
    def send_test_data(self):
        """테스트 데이터 수동 전송"""
        if not self.is_connected:
            return False, "포트가 연결되지 않았습니다"
        
        try:
            # 테스트 데이터 생성
            data_bytes = self.generate_test_data()
            
            # 데이터 내용 미리보기 (처음 50자만)
            data_preview = data_bytes.decode('utf-8', errors='replace')[:50]
            if len(data_bytes) > 50:
                data_preview += "..."
            
            # 프로토콜 패킷 생성
            packet = self.create_protocol_packet(data_bytes)
            
            # 전송 큐에 추가
            self.send_queue.put(packet)
            
            # 상세한 성공 알림
            self.status_queue.put(('SYSTEM', f"테스트 데이터 전송: {len(packet)}바이트"))
            self.status_queue.put(('SYSTEM', f"데이터 내용: {data_preview}"))
            
            return True, f"테스트 데이터 전송 완료 ({len(packet)}바이트)"
            
        except Exception as e:
            error_msg = f"테스트 데이터 전송 오류: {str(e)}"
            self.status_queue.put(('ERROR', error_msg))
            return False, error_msg
    
    def _periodic_send_worker(self):
        """주기적 전송 작업자 스레드"""
        while self.periodic_send_active and self.is_connected:
            try:
                # 테스트 데이터 생성
                data_bytes = self.generate_test_data()
                
                # 프로토콜 패킷 생성
                packet = self.create_protocol_packet(data_bytes)
                
                # 전송 큐에 추가
                self.send_queue.put(packet)
                
                # 100ms 대기
                time.sleep(self.periodic_send_interval)
                
            except Exception as e:
                if self.periodic_send_active:  # 활성 상태에서만 오류 보고
                    self.status_queue.put(('ERROR', f"주기적 전송 오류: {str(e)}"))
                break
