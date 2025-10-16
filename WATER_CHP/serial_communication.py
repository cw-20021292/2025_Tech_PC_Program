import tkinter as tk
from tkinter import ttk, scrolledtext, messagebox
import serial
import serial.tools.list_ports
import threading
import time
from datetime import datetime
import queue


class ProtocolHandler:
    """프로토콜 데이터 처리 클래스"""
    STX = 0x02  # Start of Text
    ETX = 0x03  # End of Text
    PACKET_SIZE = 20  # 고정 패킷 크기
    
    def __init__(self):
        self.receive_buffer = bytearray()
    
    def create_packet(self, data_bytes):
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
    
    def process_received_data(self, new_data):
        """수신된 데이터를 버퍼에 추가하고 완성된 패킷 추출"""
        self.receive_buffer.extend(new_data)
        packets = []
        
        while len(self.receive_buffer) >= self.PACKET_SIZE:
            # STX 찾기
            stx_index = -1
            for i in range(len(self.receive_buffer)):
                if self.receive_buffer[i] == self.STX:
                    stx_index = i
                    break
            
            if stx_index == -1:
                # STX를 찾을 수 없으면 버퍼 클리어
                self.receive_buffer.clear()
                break
            
            # STX 이전 데이터 제거
            if stx_index > 0:
                self.receive_buffer = self.receive_buffer[stx_index:]
            
            # 완전한 패킷이 있는지 확인
            if len(self.receive_buffer) >= self.PACKET_SIZE:
                # 패킷 추출
                packet = self.receive_buffer[:self.PACKET_SIZE]
                
                # ETX 확인
                if packet[-1] == self.ETX:
                    # 유효한 패킷
                    data_part = packet[1:-1]  # STX, ETX 제외
                    # 패딩(0x00) 제거
                    data_part = data_part.rstrip(b'\x00')
                    packets.append(bytes(data_part))
                
                # 처리된 패킷 제거
                self.receive_buffer = self.receive_buffer[self.PACKET_SIZE:]
            else:
                break
        
        return packets
    
    def is_protocol_data(self, data):
        """데이터가 프로토콜 형식인지 확인"""
        return (len(data) >= 2 and 
                data[0] == self.STX and 
                (len(data) == self.PACKET_SIZE and data[-1] == self.ETX))
    
    def format_packet_display(self, packet_data, is_full_packet=False):
        """패킷 데이터를 표시용으로 포맷"""
        if is_full_packet:
            # 전체 패킷 (STX + 데이터 + ETX)
            hex_str = " ".join([f"{b:02X}" for b in packet_data])
            return f"[프로토콜] {hex_str}"
        else:
            # 데이터 부분만
            hex_str = " ".join([f"{b:02X}" for b in packet_data])
            ascii_str = ""
            for b in packet_data:
                if 32 <= b <= 126:  # 출력 가능한 ASCII
                    ascii_str += chr(b)
                else:
                    ascii_str += "."
            return f"[프로토콜 데이터] HEX: {hex_str} | ASCII: {ascii_str}"


class SerialCommunicationApp:
    def __init__(self, root):
        self.root = root
        self.root.title("USB TO SERIAL 통신 프로그램")
        self.root.geometry("1000x600")
        self.root.resizable(True, True)
        
        # 시리얼 통신 관련 변수
        self.serial_connection = None
        self.is_connected = False
        self.receive_thread = None
        self.stop_thread = False
        
        # 데이터 큐 (스레드 간 안전한 통신)
        self.data_queue = queue.Queue()
        
        # 밸브 상태 관리 (1~10번 밸브, 기본값: CLOSE)
        self.valve_states = {i: False for i in range(1, 11)}  # False=CLOSE, True=OPEN
        self.valve_labels = {}
        
        # 프로토콜 핸들러 초기화
        self.protocol_handler = ProtocolHandler()
        
        # GUI 구성
        self.create_widgets()
        self.refresh_ports()
        
        # 큐 모니터링 시작
        self.monitor_queue()
    
    def create_widgets(self):
        """GUI 위젯들을 생성하고 배치"""
        # 메인 프레임
        main_frame = ttk.Frame(self.root, padding="10")
        main_frame.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))
        
        # 밸브 상태 프레임 (왼쪽)
        valve_frame = ttk.LabelFrame(main_frame, text="밸브 상태", padding="10")
        valve_frame.grid(row=0, column=0, rowspan=3, sticky=(tk.W, tk.N, tk.S), padx=(0, 10))
        
        # 밸브 상태 위젯들 생성
        self.create_valve_widgets(valve_frame)
        
        # 통신 관련 프레임들을 오른쪽으로 이동
        comm_frame = ttk.Frame(main_frame)
        comm_frame.grid(row=0, column=1, sticky=(tk.W, tk.E, tk.N, tk.S))
        
        # 연결 설정 프레임
        connection_frame = ttk.LabelFrame(comm_frame, text="연결 설정", padding="10")
        connection_frame.grid(row=0, column=0, sticky=(tk.W, tk.E), pady=(0, 10))
        
        # 포트 선택
        ttk.Label(connection_frame, text="포트:").grid(row=0, column=0, sticky=tk.W, padx=(0, 5))
        self.port_var = tk.StringVar()
        self.port_combo = ttk.Combobox(connection_frame, textvariable=self.port_var, 
                                      state="readonly", width=15)
        self.port_combo.grid(row=0, column=1, padx=(0, 10))
        
        # 포트 새로고침 버튼
        self.refresh_btn = ttk.Button(connection_frame, text="새로고침", 
                                     command=self.refresh_ports)
        self.refresh_btn.grid(row=0, column=2, padx=(0, 20))
        
        # 통신속도 선택
        ttk.Label(connection_frame, text="통신속도:").grid(row=0, column=3, sticky=tk.W, padx=(0, 5))
        self.baudrate_var = tk.StringVar(value="9600")
        self.baudrate_combo = ttk.Combobox(connection_frame, textvariable=self.baudrate_var,
                                          values=["9600", "19200", "38400", "57600", "115200"],
                                          state="readonly", width=10)
        self.baudrate_combo.grid(row=0, column=4, padx=(0, 20))
        
        # 연결/연결해제 버튼
        self.connect_btn = ttk.Button(connection_frame, text="연결", 
                                     command=self.toggle_connection)
        self.connect_btn.grid(row=0, column=5)
        
        # 데이터 송신 프레임
        send_frame = ttk.LabelFrame(comm_frame, text="데이터 송신", padding="10")
        send_frame.grid(row=1, column=0, sticky=(tk.W, tk.E), pady=(0, 10))
        
        # 송신 데이터 입력
        ttk.Label(send_frame, text="송신 데이터:").grid(row=0, column=0, sticky=tk.W, padx=(0, 5))
        self.send_entry = ttk.Entry(send_frame, width=50)
        self.send_entry.grid(row=0, column=1, padx=(0, 10), sticky=(tk.W, tk.E))
        self.send_entry.bind('<Return>', lambda e: self.send_data())
        
        # 송신 버튼
        self.send_btn = ttk.Button(send_frame, text="송신", command=self.send_data)
        self.send_btn.grid(row=0, column=2)
        self.send_btn.config(state="disabled")
        
        # 송신 모드 선택
        mode_frame = ttk.Frame(send_frame)
        mode_frame.grid(row=1, column=0, columnspan=4, pady=(5, 0), sticky=tk.W)
        
        self.send_mode_var = tk.StringVar(value="text")
        ttk.Radiobutton(mode_frame, text="텍스트", variable=self.send_mode_var, 
                       value="text").pack(side=tk.LEFT, padx=(0, 10))
        ttk.Radiobutton(mode_frame, text="HEX", variable=self.send_mode_var, 
                       value="hex").pack(side=tk.LEFT, padx=(0, 10))
        ttk.Radiobutton(mode_frame, text="프로토콜", variable=self.send_mode_var, 
                       value="protocol").pack(side=tk.LEFT)
        
        # 송신 프레임 컬럼 설정
        send_frame.columnconfigure(1, weight=1)
        
        # 통신 로그 프레임
        log_frame = ttk.LabelFrame(comm_frame, text="통신 로그", padding="10")
        log_frame.grid(row=2, column=0, sticky=(tk.W, tk.E, tk.N, tk.S), pady=(0, 10))
        
        # 로그 텍스트 영역
        self.log_text = scrolledtext.ScrolledText(log_frame, height=20, width=80)
        self.log_text.grid(row=0, column=0, columnspan=3, sticky=(tk.W, tk.E, tk.N, tk.S))
        
        # 로그 제어 버튼들
        log_btn_frame = ttk.Frame(log_frame)
        log_btn_frame.grid(row=1, column=0, columnspan=3, pady=(10, 0))
        
        self.clear_btn = ttk.Button(log_btn_frame, text="로그 지우기", 
                                   command=self.clear_log)
        self.clear_btn.pack(side=tk.LEFT, padx=(0, 10))
        
        self.autoscroll_var = tk.BooleanVar(value=True)
        self.autoscroll_check = ttk.Checkbutton(log_btn_frame, text="자동 스크롤", 
                                               variable=self.autoscroll_var)
        self.autoscroll_check.pack(side=tk.LEFT)
        
        # 테스트 버튼들 (개발용)
        self.test_valve_btn = ttk.Button(log_btn_frame, text="밸브 테스트", 
                                        command=self.test_valve_status)
        self.test_valve_btn.pack(side=tk.LEFT, padx=(20, 0))
        
        self.test_protocol_btn = ttk.Button(log_btn_frame, text="프로토콜 테스트", 
                                           command=self.test_protocol_data)
        self.test_protocol_btn.pack(side=tk.LEFT, padx=(10, 0))
        
        # 로그 프레임 확장 설정
        log_frame.columnconfigure(0, weight=1)
        log_frame.rowconfigure(0, weight=1)
        
        # 통신 프레임 확장 설정
        comm_frame.columnconfigure(0, weight=1)
        comm_frame.rowconfigure(2, weight=1)
        
        # 메인 프레임 확장 설정
        main_frame.columnconfigure(1, weight=1)
        main_frame.rowconfigure(0, weight=1)
        
        # 루트 윈도우 확장 설정
        self.root.columnconfigure(0, weight=1)
        self.root.rowconfigure(0, weight=1)
    
    def create_valve_widgets(self, parent_frame):
        """밸브 상태 표시 위젯들을 생성"""
        # 밸브 상태 설명
        desc_frame = ttk.Frame(parent_frame)
        desc_frame.grid(row=0, column=0, columnspan=2, pady=(0, 10))
        
        open_label = tk.Label(desc_frame, text="● OPEN", fg="green", font=("Arial", 10, "bold"))
        open_label.pack(side=tk.LEFT, padx=(0, 20))
        
        close_label = tk.Label(desc_frame, text="● CLOSE", fg="red", font=("Arial", 10, "bold"))
        close_label.pack(side=tk.LEFT)
        
        # 밸브 1~10 상태 표시
        for i in range(1, 11):
            row = (i - 1) // 2 + 1  # 2열로 배치
            col = (i - 1) % 2
            
            valve_frame = ttk.Frame(parent_frame)
            valve_frame.grid(row=row, column=col, padx=5, pady=2, sticky=tk.W)
            
            # 밸브 번호 라벨
            valve_num_label = tk.Label(valve_frame, text=f"밸브 {i:2d}:", 
                                     font=("Arial", 10), width=8)
            valve_num_label.pack(side=tk.LEFT)
            
            # 상태 표시 라벨 (기본값: CLOSE/빨간색)
            status_label = tk.Label(valve_frame, text="CLOSE", 
                                  fg="red", bg="white",
                                  font=("Arial", 10, "bold"),
                                  width=6, relief="sunken", bd=1)
            status_label.pack(side=tk.LEFT, padx=(5, 0))
            
            # 라벨 참조 저장
            self.valve_labels[i] = status_label
    
    def update_valve_status(self, valve_num, is_open):
        """밸브 상태 업데이트"""
        if valve_num in self.valve_labels:
            self.valve_states[valve_num] = is_open
            label = self.valve_labels[valve_num]
            
            if is_open:
                label.config(text="OPEN", fg="white", bg="green")
            else:
                label.config(text="CLOSE", fg="white", bg="red")
    
    def refresh_ports(self):
        """사용 가능한 시리얼 포트를 새로고침"""
        ports = serial.tools.list_ports.comports()
        port_list = [f"{port.device} - {port.description}" for port in ports]
        
        self.port_combo['values'] = port_list
        if port_list:
            self.port_combo.set(port_list[0])
        else:
            self.port_combo.set("")
            self.log_message("사용 가능한 시리얼 포트가 없습니다.", "SYSTEM")
    
    def toggle_connection(self):
        """연결/연결해제 토글"""
        if not self.is_connected:
            self.connect_serial()
        else:
            self.disconnect_serial()
    
    def connect_serial(self):
        """시리얼 포트 연결"""
        try:
            port_info = self.port_var.get()
            if not port_info:
                messagebox.showerror("오류", "포트를 선택해주세요.")
                return
            
            # 포트명 추출 (예: "COM3 - USB Serial Port" -> "COM3")
            port = port_info.split(" - ")[0]
            baudrate = int(self.baudrate_var.get())
            
            # 시리얼 연결
            self.serial_connection = serial.Serial(
                port=port,
                baudrate=baudrate,
                bytesize=serial.EIGHTBITS,
                parity=serial.PARITY_NONE,
                stopbits=serial.STOPBITS_ONE,
                timeout=1
            )
            
            self.is_connected = True
            self.stop_thread = False
            
            # 수신 스레드 시작
            self.receive_thread = threading.Thread(target=self.receive_data, daemon=True)
            self.receive_thread.start()
            
            # UI 업데이트
            self.connect_btn.config(text="연결해제")
            self.send_btn.config(state="normal")
            self.port_combo.config(state="disabled")
            self.baudrate_combo.config(state="disabled")
            self.refresh_btn.config(state="disabled")
            
            self.log_message(f"포트 {port} ({baudrate} bps)에 연결되었습니다.", "SYSTEM")
            
        except Exception as e:
            messagebox.showerror("연결 오류", f"시리얼 포트 연결에 실패했습니다:\n{str(e)}")
            self.log_message(f"연결 실패: {str(e)}", "ERROR")
    
    def disconnect_serial(self):
        """시리얼 포트 연결해제"""
        try:
            self.stop_thread = True
            self.is_connected = False
            
            if self.serial_connection and self.serial_connection.is_open:
                self.serial_connection.close()
            
            # UI 업데이트
            self.connect_btn.config(text="연결")
            self.send_btn.config(state="disabled")
            self.port_combo.config(state="readonly")
            self.baudrate_combo.config(state="readonly")
            self.refresh_btn.config(state="normal")
            
            self.log_message("시리얼 포트 연결이 해제되었습니다.", "SYSTEM")
            
        except Exception as e:
            self.log_message(f"연결해제 오류: {str(e)}", "ERROR")
    
    def send_data(self):
        """데이터 송신"""
        if not self.is_connected or not self.serial_connection:
            messagebox.showwarning("경고", "시리얼 포트가 연결되지 않았습니다.")
            return
        
        try:
            data = self.send_entry.get()
            if not data:
                return
            
            send_mode = self.send_mode_var.get()
            
            if send_mode == "hex":
                # HEX 모드: 공백으로 구분된 HEX 값들을 바이트로 변환
                hex_values = data.replace(" ", "")
                if len(hex_values) % 2 != 0:
                    messagebox.showerror("오류", "HEX 데이터는 짝수 개의 문자여야 합니다.")
                    return
                bytes_data = bytes.fromhex(hex_values)
                display_data = " ".join([f"{b:02X}" for b in bytes_data])
                
            elif send_mode == "protocol":
                # 프로토콜 모드: STX + 데이터 + ETX 패킷 생성
                if data.startswith("HEX:"):
                    # HEX 데이터로 입력된 경우
                    hex_str = data[4:].replace(" ", "")
                    if len(hex_str) % 2 != 0:
                        messagebox.showerror("오류", "HEX 데이터는 짝수 개의 문자여야 합니다.")
                        return
                    data_bytes = bytes.fromhex(hex_str)
                else:
                    # 텍스트 데이터
                    data_bytes = data.encode('utf-8')
                
                if len(data_bytes) > 18:  # 20바이트 - STX - ETX
                    messagebox.showerror("오류", "프로토콜 데이터는 18바이트를 초과할 수 없습니다.")
                    return
                
                bytes_data = self.protocol_handler.create_packet(data_bytes)
                display_data = self.protocol_handler.format_packet_display(bytes_data, is_full_packet=True)
                
            else:
                # 텍스트 모드: 문자열을 바이트로 변환
                bytes_data = data.encode('utf-8')
                display_data = data
            
            # 데이터 송신
            self.serial_connection.write(bytes_data)
            
            # 로그에 기록
            self.log_message(f"송신: {display_data}", "SEND")
            
            # 입력 필드 클리어
            self.send_entry.delete(0, tk.END)
            
        except Exception as e:
            messagebox.showerror("송신 오류", f"데이터 송신에 실패했습니다:\n{str(e)}")
            self.log_message(f"송신 오류: {str(e)}", "ERROR")
    
    def receive_data(self):
        """데이터 수신 스레드"""
        while not self.stop_thread and self.is_connected:
            try:
                if self.serial_connection and self.serial_connection.in_waiting > 0:
                    data = self.serial_connection.read(self.serial_connection.in_waiting)
                    
                    # 프로토콜 데이터 처리
                    protocol_packets = self.protocol_handler.process_received_data(data)
                    
                    if protocol_packets:
                        # 프로토콜 패킷이 있는 경우
                        for packet_data in protocol_packets:
                            self.data_queue.put(('PROTOCOL', packet_data))
                    else:
                        # 일반 데이터로 처리
                        self.data_queue.put(('RECEIVE', data))
                
                time.sleep(0.01)  # CPU 사용률 조절
                
            except Exception as e:
                if self.is_connected:  # 연결이 끊어진 경우가 아니라면 오류 로그
                    self.data_queue.put(('ERROR', f"수신 오류: {str(e)}"))
                break
    
    def monitor_queue(self):
        """큐 모니터링 및 데이터 처리 (메인 스레드에서 실행)"""
        try:
            while True:
                msg_type, data = self.data_queue.get_nowait()
                
                if msg_type == 'RECEIVE':
                    # 일반 수신 데이터 처리
                    try:
                        display_data = data.decode('utf-8', errors='replace')
                    except:
                        display_data = " ".join([f"{b:02X}" for b in data])
                    
                    # 밸브 상태 파싱 및 업데이트
                    self.parse_valve_data(display_data)
                    
                    self.log_message(f"수신: {display_data}", "RECEIVE")
                
                elif msg_type == 'PROTOCOL':
                    # 프로토콜 데이터 처리
                    display_data = self.protocol_handler.format_packet_display(data)
                    
                    # 프로토콜 데이터에서 밸브 상태 파싱
                    try:
                        text_data = data.decode('utf-8', errors='replace')
                        self.parse_valve_data(text_data)
                    except:
                        pass
                    
                    self.log_message(f"수신: {display_data}", "PROTOCOL")
                
                elif msg_type == 'ERROR':
                    self.log_message(data, "ERROR")
                    
        except queue.Empty:
            pass
        
        # 100ms 후 다시 확인
        self.root.after(100, self.monitor_queue)
    
    def parse_valve_data(self, data_string):
        """수신된 데이터에서 밸브 상태를 파싱"""
        try:
            # 밸브 상태 파싱 패턴들
            # 예시 패턴: "VALVE1:OPEN", "VALVE2:CLOSE", "V1:1", "V2:0" 등
            import re
            
            # 패턴 1: VALVE숫자:OPEN/CLOSE
            pattern1 = r'VALVE(\d+):(OPEN|CLOSE)'
            matches1 = re.findall(pattern1, data_string.upper())
            for valve_num, state in matches1:
                valve_num = int(valve_num)
                if 1 <= valve_num <= 10:
                    is_open = (state == 'OPEN')
                    self.update_valve_status(valve_num, is_open)
            
            # 패턴 2: V숫자:1/0
            pattern2 = r'V(\d+):([01])'
            matches2 = re.findall(pattern2, data_string.upper())
            for valve_num, state in matches2:
                valve_num = int(valve_num)
                if 1 <= valve_num <= 10:
                    is_open = (state == '1')
                    self.update_valve_status(valve_num, is_open)
            
            # 패턴 3: 간단한 숫자 패턴 예: "1:OPEN 2:CLOSE"
            pattern3 = r'(\d+):(OPEN|CLOSE)'
            matches3 = re.findall(pattern3, data_string.upper())
            for valve_num, state in matches3:
                valve_num = int(valve_num)
                if 1 <= valve_num <= 10:
                    is_open = (state == 'OPEN')
                    self.update_valve_status(valve_num, is_open)
                    
        except Exception as e:
            # 파싱 오류는 조용히 무시 (로그에 기록하지 않음)
            pass
    
    def test_valve_status(self):
        """밸브 상태 테스트용 함수 (개발/디버깅용)"""
        import random
        for i in range(1, 11):
            is_open = random.choice([True, False])
            self.update_valve_status(i, is_open)
    
    def test_protocol_data(self):
        """프로토콜 데이터 테스트용 함수 (개발/디버깅용)"""
        import random
        
        # 테스트 데이터 생성
        test_messages = [
            "VALVE1:OPEN",
            "VALVE2:CLOSE", 
            "V3:1 V4:0",
            "TEMP:25.5",
            "STATUS:OK"
        ]
        
        # 랜덤 메시지 선택
        message = random.choice(test_messages)
        
        try:
            # 프로토콜 패킷 생성
            data_bytes = message.encode('utf-8')
            packet = self.protocol_handler.create_packet(data_bytes)
            
            # 수신된 것처럼 처리
            protocol_packets = self.protocol_handler.process_received_data(packet)
            
            if protocol_packets:
                for packet_data in protocol_packets:
                    self.data_queue.put(('PROTOCOL', packet_data))
            
            self.log_message(f"프로토콜 테스트 데이터 생성: {message}", "SYSTEM")
            
        except Exception as e:
            self.log_message(f"프로토콜 테스트 오류: {str(e)}", "ERROR")
    
    def log_message(self, message, msg_type="INFO"):
        """로그 메시지 추가"""
        timestamp = datetime.now().strftime("%H:%M:%S.%f")[:-3]
        
        # 메시지 타입별 색상 설정
        color_map = {
            "SEND": "blue",
            "RECEIVE": "green", 
            "PROTOCOL": "orange",
            "ERROR": "red",
            "SYSTEM": "purple",
            "INFO": "black"
        }
        
        color = color_map.get(msg_type, "black")
        
        # 로그 텍스트에 추가
        self.log_text.config(state="normal")
        self.log_text.insert(tk.END, f"[{timestamp}] {message}\n")
        
        # 색상 적용
        line_start = self.log_text.index("end-2l")
        line_end = self.log_text.index("end-1l")
        self.log_text.tag_add(msg_type, line_start, line_end)
        self.log_text.tag_config(msg_type, foreground=color)
        
        self.log_text.config(state="disabled")
        
        # 자동 스크롤
        if self.autoscroll_var.get():
            self.log_text.see(tk.END)
    
    def clear_log(self):
        """로그 지우기"""
        self.log_text.config(state="normal")
        self.log_text.delete(1.0, tk.END)
        self.log_text.config(state="disabled")
    
    def on_closing(self):
        """프로그램 종료 시 처리"""
        if self.is_connected:
            self.disconnect_serial()
        self.root.destroy()


def main():
    root = tk.Tk()
    app = SerialCommunicationApp(root)
    
    # 프로그램 종료 시 정리 작업
    root.protocol("WM_DELETE_WINDOW", app.on_closing)
    
    root.mainloop()


if __name__ == "__main__":
    main()
