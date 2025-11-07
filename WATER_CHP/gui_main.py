"""
메인 GUI 모듈 - 프로토콜 적용 (전체 기능 유지)
"""
import tkinter as tk
from tkinter import ttk, messagebox
import threading
import time
from datetime import datetime
from collections import deque

# matplotlib 설정
import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure

plt.rcParams['font.family'] = ['Malgun Gothic', 'DejaVu Sans', 'sans-serif']
plt.rcParams['axes.unicode_minus'] = False

from communication import SerialCommunication, DataParser
from systems import RefrigerationSystem
import constants


class MainGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("코웨이 정수기 시스템 검토 프로그램 - 프로토콜 적용")
        self.root.geometry("1000x850")
        self.root.resizable(True, True)
        
        # 통신 모듈 초기화
        self.comm = SerialCommunication()
        self.data_parser = DataParser()
        
        # 데이터 저장소
        self.nos_valve_states = {i: False for i in range(1, 6)}
        self.feed_valve_states = {i: False for i in range(1, 16)}
        
        self.sensor_data = {
            'outdoor_temp1': 0, 'outdoor_temp2': 0,
            'purified_temp': 0, 'cold_temp': 0,
            'hot_inlet_temp': 0, 'hot_internal_temp': 0,
            'hot_outlet_temp': 0
        }
        
        # 공조시스템
        self.hvac_data = {
            'refrigerant_valve_state': '핫가스',
            'refrigerant_valve_target': '핫가스',
            'compressor_state': '미동작',
            'current_rps': 0,
            'error_code': 0,
            'dc_fan1': 'OFF',
            'dc_fan2': 'OFF'
        }

        # 공조시스템 입력 모드 상태
        self.hvac_edit_mode = False  # 입력 모드 활성화 여부
        self.hvac_temp_data = {
            'refrigerant_valve_target': '핫가스',  # 냉각/제빙/핫가스
            'compressor_state': '미동작',  # 동작/미동작
            'dc_fan1': 'OFF',  # ON/OFF
            'dc_fan2': 'OFF'   # ON/OFF
        }
        
        # 냉각시스템
        self.cooling_data = {
            'operation_state': 'STOP',
            'target_rps': 0,               # 목표 RPS (냉각시스템)
            'on_temp': 0,
            'off_temp': 0,
            'cooling_additional_time': 0
        }

        # 냉각 입력 모드 상태 추가
        self.cooling_edit_mode = False  # 입력 모드 활성화 여부
        
        # 냉각 입력 필드가 포커스 중인지 추적
        self.cooling_input_focus = {
            'on_temp': False,
            'off_temp': False,
            'cooling_additional_time': False
        }
        
        # 제빙시스템
        self.icemaking_data = {
            'operation': '대기',
            'target_rps': 0,               # 목표 RPS
            'icemaking_time': 0,
            'water_capacity': 0,
            'swing_on_time': 0,
            'swing_off_time': 0
        }

        # 제빙 입력 모드 상태 추가 (냉각 입력 모드 변수 뒤에)
        self.icemaking_edit_mode = False  # 입력 모드 활성화 여부
        self.icemaking_temp_data = {
            'operation': '대기',  # 대기/동작
            'target_rps': 0,      # 목표 RPS
            'icemaking_time': 0,      # ms 단위 (0~65535)
            'water_capacity': 0,      # Hz 단위 (0~65535)
            'swing_on_time': 0,       # ms 단위 (0~255)
            'swing_off_time': 0       # ms 단위 (0~255)
        }
        
        # 드레인탱크
        self.drain_tank_data = {
            'low_level': '미감지',
            'high_level': '미감지',
            'water_level_state': '비어있음'
        }
        
        # 드레인펌프
        self.drain_pump_data = {
            'operation_state': 'OFF'
        }
        
        # 보냉시스템 모듈 초기화
        self.refrigeration_system = RefrigerationSystem(self.root, self.comm, self.log_communication)
        
        # 그래프 데이터
        self.graph_data = {
            'time': deque(maxlen=100),
            'temp1': deque(maxlen=100),
            'temp2': deque(maxlen=100),
            'cold_temp': deque(maxlen=100),
            'hot_temp': deque(maxlen=100),
            'pressure1': deque(maxlen=100),
            'pressure2': deque(maxlen=100)
        }
        
        # 그래프 토글 상태
        self.graph1_active_items = set()
        self.graph2_active_items = set()
        
        # 모든 데이터 항목의 그래프 데이터
        self.all_graph_data = {
            'time': deque(maxlen=100),
            'outdoor_temp1': deque(maxlen=100),
            'outdoor_temp2': deque(maxlen=100),
            'purified_temp': deque(maxlen=100),
            'cold_temp_sensor': deque(maxlen=100),
            'hot_inlet_temp': deque(maxlen=100),
            'hot_internal_temp': deque(maxlen=100),
            'hot_outlet_temp': deque(maxlen=100),
            **{f'nos_valve_{i}': deque(maxlen=100) for i in range(1, 6)},
            **{f'feed_valve_{i}': deque(maxlen=100) for i in range(1, 16)},
            'cooling_operation': deque(maxlen=100),
            'cooling_on_temp': deque(maxlen=100),
            'cooling_off_temp': deque(maxlen=100),
            'icemaking_time': deque(maxlen=100),
            'icemaking_capacity': deque(maxlen=100),
            'drain_tank_level': deque(maxlen=100),
            'drain_pump_state': deque(maxlen=100)
        }
        
        # GUI 위젯 참조
        self.nos_valve_labels = {}
        self.feed_valve_labels = {}
        # 냉동검토용 탭 전용 라벨
        self.nos_valve_labels_freezing = {}
        self.feed_valve_labels_freezing = {}
        # 제어검토용 탭 전용 라벨
        self.nos_valve_labels_control = {}
        self.feed_valve_labels_control = {}
        
        self.sensor_labels = {}
        self.hvac_labels = {}
        self.cooling_labels = {}
        self.icemaking_labels = {}
        self.drain_tank_labels = {}
        self.drain_pump_labels = {}
        self.status_label = None
        self.comm_text = None
        
        # GUI 생성
        self.create_widgets()
        
        # 데이터 모니터링 스레드 시작
        self.monitoring_active = True
        self.monitor_thread = threading.Thread(target=self.monitor_data, daemon=True)
        self.monitor_thread.start()
        
        # GUI 업데이트 시작
        self.update_gui()
    
    def create_widgets(self):
        """탭 기반 GUI 위젯들을 생성하고 배치"""
        main_container = ttk.Frame(self.root)
        main_container.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)
        
        # 탭 노트북 (상단)
        self.notebook = ttk.Notebook(main_container)
        self.notebook.pack(fill=tk.BOTH, expand=True, pady=(0, 5))
        
        # 탭 변경 이벤트 바인딩
        self.notebook.bind("<<NotebookTabChanged>>", self.on_tab_changed)
        
        # 공통 통신부 (하단 고정)
        self.create_shared_communication_area(main_container)
        
        # 냉동검토용 탭 생성
        self.create_freezing_tab()
        
        # 제어검토용 탭 생성  
        self.create_control_tab()
    
    def create_freezing_tab(self):
        """냉동검토용 탭 생성"""
        freezing_frame = ttk.Frame(self.notebook)
        self.notebook.add(freezing_frame, text="냉동검토용")
        
        main_frame = ttk.Frame(freezing_frame, padding="2")
        main_frame.pack(fill=tk.BOTH, expand=True)
        
        # 상단 영역
        top_frame = ttk.Frame(main_frame)
        top_frame.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S), pady=(0, 2))
        
        # 중단 영역
        middle_frame = ttk.Frame(main_frame)
        middle_frame.grid(row=1, column=0, sticky=(tk.W, tk.E, tk.N, tk.S), pady=(0, 2))
        
        # 하단 영역
        bottom_frame = ttk.Frame(main_frame)
        bottom_frame.grid(row=2, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))
        
        # 상단 영역 레이아웃
        self.create_cooling_area(top_frame)
        self.create_hvac_area(top_frame)
        self.create_icemaking_area(top_frame)
        self.refrigeration_system.create_widgets(top_frame)  # 보냉시스템 추가
        self.create_graph_areas(top_frame)
        
        # 중단 영역 레이아웃
        self.create_valve_area(middle_frame)
        self.create_sensor_area(middle_frame)
        
        # 하단 영역 레이아웃
        self.create_drain_tank_area(bottom_frame)
        self.create_drain_pump_area(bottom_frame)
        
        # 프레임 확장 설정 (보냉시스템 추가로 5개로 변경)
        for i in range(5):
            top_frame.columnconfigure(i, weight=1)
        top_frame.rowconfigure(0, weight=1)
        
        middle_frame.columnconfigure(0, weight=1)
        middle_frame.columnconfigure(1, weight=1)
        middle_frame.rowconfigure(0, weight=1)
        
        bottom_frame.columnconfigure(0, weight=1)
        bottom_frame.columnconfigure(1, weight=1)
        bottom_frame.rowconfigure(0, weight=1)
        
        main_frame.columnconfigure(0, weight=1)
        main_frame.rowconfigure(0, weight=3)
        main_frame.rowconfigure(1, weight=4)
        main_frame.rowconfigure(2, weight=1)
    
    def create_control_tab(self):
        """제어검토용 탭 생성"""
        control_frame = ttk.Frame(self.notebook)
        self.notebook.add(control_frame, text="제어검토용")
        
        main_frame = ttk.Frame(control_frame, padding="2")
        main_frame.pack(fill=tk.BOTH, expand=True)
        
        # 상단 영역 (그래프)
        top_frame = ttk.Frame(main_frame)
        top_frame.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S), pady=(0, 2))
        
        # 중단 영역 (밸브류)
        middle_frame = ttk.Frame(main_frame)
        middle_frame.grid(row=1, column=0, sticky=(tk.W, tk.E, tk.N, tk.S), pady=(0, 2))
        
        # 하단 영역 (제어 관련)
        bottom_frame = ttk.Frame(main_frame)
        bottom_frame.grid(row=2, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))
        
        # 상단 영역 레이아웃
        self.create_graph_areas(top_frame)
        
        # 중단 영역 레이아웃
        self.create_valve_area(middle_frame)
        
        # 하단 영역 레이아웃
        self.create_control_sections(bottom_frame)
        
        # 프레임 확장 설정
        main_frame.columnconfigure(0, weight=1)
        main_frame.rowconfigure(0, weight=2)
        main_frame.rowconfigure(1, weight=4)
        main_frame.rowconfigure(2, weight=3)
    
    def create_cooling_area(self, parent):
        """냉각 섹션 생성"""
        cooling_frame = ttk.LabelFrame(parent, text="냉각", padding="2")
        cooling_frame.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S), padx=(0, 1))
        
        # 운전 상태
        state_frame = ttk.Frame(cooling_frame)
        state_frame.grid(row=0, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(state_frame, text="운전 상태:", font=("Arial", 8), width=8).pack(side=tk.LEFT)
        self.cooling_labels['operation_state'] = tk.Label(state_frame, text="STOP", 
                                                        fg="white", bg="red", font=("Arial", 7, "bold"),
                                                        width=8, relief="raised")
        self.cooling_labels['operation_state'].pack(side=tk.LEFT, padx=(2, 0))
        
        # 목표 RPS (입력 가능)
        target_rps_frame = ttk.Frame(cooling_frame)
        target_rps_frame.grid(row=2, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(target_rps_frame, text="목표 RPS:", font=("Arial", 8), width=8).pack(side=tk.LEFT)
        vcmd_rps = (self.root.register(self.validate_rps), '%P')
        self.cooling_labels['target_rps'] = tk.Entry(target_rps_frame, font=("Arial", 8), 
                                             width=6, validate='key', validatecommand=vcmd_rps,
                                             state='readonly')
        self.cooling_labels['target_rps'].insert(0, "0")
        self.cooling_labels['target_rps'].pack(side=tk.LEFT, padx=(2, 0))
        
        # ON 온도 (입력 가능)
        on_temp_frame = ttk.Frame(cooling_frame)
        on_temp_frame.grid(row=3, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(on_temp_frame, text="ON 온도:", font=("Arial", 8), width=8).pack(side=tk.LEFT)
        
        vcmd_temp = (self.root.register(self.validate_number), '%P')
        self.cooling_labels['on_temp'] = tk.Entry(on_temp_frame, font=("Arial", 8), 
                                                width=6, validate='key', validatecommand=vcmd_temp,
                                                state='readonly')  # 기본 읽기 전용
        self.cooling_labels['on_temp'].insert(0, "0")
        self.cooling_labels['on_temp'].pack(side=tk.LEFT, padx=(2, 0))
        ttk.Label(on_temp_frame, text="℃", font=("Arial", 8)).pack(side=tk.LEFT)
        
        # OFF 온도 (입력 가능)
        off_temp_frame = ttk.Frame(cooling_frame)
        off_temp_frame.grid(row=4, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(off_temp_frame, text="OFF 온도:", font=("Arial", 8), width=8).pack(side=tk.LEFT)
        self.cooling_labels['off_temp'] = tk.Entry(off_temp_frame, font=("Arial", 8), 
                                                width=6, validate='key', validatecommand=vcmd_temp,
                                                state='readonly')  # 기본 읽기 전용
        self.cooling_labels['off_temp'].insert(0, "0")
        self.cooling_labels['off_temp'].pack(side=tk.LEFT, padx=(2, 0))
        ttk.Label(off_temp_frame, text="℃", font=("Arial", 8)).pack(side=tk.LEFT)
        
        # 냉각 추가시간 (입력 가능)
        add_time_frame = ttk.Frame(cooling_frame)
        add_time_frame.grid(row=5, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(add_time_frame, text="추가시간:", font=("Arial", 8), width=8).pack(side=tk.LEFT)
        self.cooling_labels['cooling_additional_time'] = tk.Entry(add_time_frame, font=("Arial", 8), 
                                                                width=6, validate='key', validatecommand=vcmd_temp,
                                                                state='readonly')  # 기본 읽기 전용
        self.cooling_labels['cooling_additional_time'].insert(0, "0")
        self.cooling_labels['cooling_additional_time'].pack(side=tk.LEFT, padx=(2, 0))
        ttk.Label(add_time_frame, text="초", font=("Arial", 8)).pack(side=tk.LEFT)
        
        # CMD 0xB1 전송 버튼
        send_btn_frame = ttk.Frame(cooling_frame)
        send_btn_frame.grid(row=6, column=0, sticky=(tk.W, tk.E), pady=(5, 1))
        self.cooling_send_btn = ttk.Button(send_btn_frame, text="입력모드",
                                        command=self.send_cooling_control, state="disabled")
        self.cooling_send_btn.pack(fill=tk.X)
        
        cooling_frame.columnconfigure(0, weight=1)
    
    def validate_number(self, value):
        """숫자와 소숫점 입력 가능하도록 검증"""
        if value == "":
            return True
        try:
            # 소숫점이 2개 이상이면 안됨
            if value.count('.') > 1:
                return False
            float(value)
            return True
        except ValueError:
            return False
    
    def validate_rps(self, value):
        """RPS 범위 검증 (37~75) - 입력 중에는 숫자만 허용"""
        if value == "":
            return True
        try:
            # 숫자인지 확인 (입력 중에는 범위 체크하지 않음)
            int(value)
            return True
        except (ValueError, OverflowError):
            return False
    
    def send_cooling_control(self):
        """냉각 제어 CMD 0xB1 전송 - 입력 모드 토글 방식"""
        if not self.comm.is_connected:
            messagebox.showwarning("경고", "시리얼 포트가 연결되지 않았습니다.")
            return
        
        if not self.cooling_edit_mode:
            # ========== 입력 모드 활성화 ==========
            self.cooling_edit_mode = True
            
            # Entry 위젯들을 편집 가능하게 설정
            self.cooling_labels['target_rps'].config(state='normal', bg='lightyellow')
            self.cooling_labels['on_temp'].config(state='normal', bg='lightyellow')
            self.cooling_labels['off_temp'].config(state='normal', bg='lightyellow')
            self.cooling_labels['cooling_additional_time'].config(state='normal', bg='lightyellow')
            
            # 버튼 텍스트 변경
            self.cooling_send_btn.config(text="설정 완료 (CMD 0xB1 전송)")
            
            self.log_communication("냉각 설정 입력 모드 활성화", "purple")
        
        else:
            # ========== 입력 모드 비활성화 및 데이터 전송 ==========
            try:
                # 입력 값 가져오기
                target_rps_str = self.cooling_labels['target_rps'].get()
                on_temp_str = self.cooling_labels['on_temp'].get()
                
                # 빈 값 체크
                if not target_rps_str or not on_temp_str:
                    messagebox.showwarning("경고", "모든 값을 입력해주세요.")
                    return
                
                # 정수로 변환
                target_rps = int(float(target_rps_str))
                on_temp = int(float(on_temp_str))
                cooling_operation = 1 if self.cooling_data['operation_state'] == 'GOING' else 0
                
                # 범위 체크
                if not (constants.RPS_MIN <= target_rps <= constants.RPS_MAX):
                    messagebox.showwarning("경고", f"목표 RPS는 {constants.RPS_MIN}~{constants.RPS_MAX} 범위여야 합니다.")
                    return
                
                if not (-127 <= on_temp <= 127):
                    messagebox.showwarning("경고", "냉각 ON 온도는 -127~127℃ 범위여야 합니다.")
                    return
                
                # TARGET TEMP는 on_temp를 사용 (임시)
                target_temp = on_temp
                
                # DATA FIELD 구성 (4바이트 - 새로운 프로토콜)
                data_field = bytearray(4)
                data_field[0] = target_rps  # TARGET RPS
                data_field[1] = self.comm.protocol.int_to_signed_byte(target_temp)  # TARGET TEMP (signed byte)
                data_field[2] = cooling_operation  # 냉각 동작 (0: STOP, 1: GOING)
                data_field[3] = self.comm.protocol.int_to_signed_byte(on_temp)  # 냉각 ON 온도 (signed byte)
                
                # 로그 출력
                hex_data = " ".join([f"{b:02X}" for b in data_field])
                self.log_communication(f"[냉각 제어] CMD 0xB1 전송 (4바이트)", "blue")
                self.log_communication(f"  목표 RPS: {target_rps}", "gray")
                self.log_communication(f"  목표 온도: {target_temp}℃", "gray")
                self.log_communication(f"  냉각 동작: {'GOING' if cooling_operation == 1 else 'STOP'}", "gray")
                self.log_communication(f"  냉각 ON 온도: {on_temp}℃", "gray")
                self.log_communication(f"  DATA FIELD (HEX): {hex_data}", "gray")
                
                # CMD 0xB1 패킷 전송
                success, message = self.comm.send_packet(0xB1, bytes(data_field))
                
                if success:
                    self.log_communication(f"  전송 성공 (CMD 0xB1, 4바이트)", "green")
                    
                    # 입력 모드 비활성화
                    self.cooling_edit_mode = False
                    
                    # Entry 위젯들을 읽기 전용으로 설정
                    self.cooling_labels['target_rps'].config(state='readonly', bg='white')
                    self.cooling_labels['on_temp'].config(state='readonly', bg='white')
                    self.cooling_labels['off_temp'].config(state='readonly', bg='white')
                    self.cooling_labels['cooling_additional_time'].config(state='readonly', bg='white')
                    
                    # 버튼 텍스트 변경
                    self.cooling_send_btn.config(text="입력모드")
                    
                else:
                    self.log_communication(f"  전송 실패: {message}", "red")
                    
            except ValueError:
                messagebox.showerror("오류", "올바른 숫자를 입력해주세요.")
            except Exception as e:
                self.log_communication(f"냉각 제어 오류: {str(e)}", "red")

    # ============================================
    # 4. 제빙 제어 CMD 0xB2 전송 함수 (7바이트 버전)
    # ============================================
    def send_icemaking_control(self):
        """제빙 제어 CMD 0xB2 전송 - 입력 모드 토글 방식 (7바이트)"""
        if not self.comm.is_connected:
            messagebox.showwarning("경고", "시리얼 포트가 연결되지 않았습니다.")
            return
        
        if not self.icemaking_edit_mode:
            # ========== 입력 모드 활성화 ==========
            self.icemaking_edit_mode = True
            
            # 현재 값을 임시 저장소에 복사
            self.icemaking_temp_data['operation'] = self.icemaking_data['operation']
            self.icemaking_temp_data['target_rps'] = self.icemaking_data['target_rps']
            self.icemaking_temp_data['icemaking_time'] = self.icemaking_data['icemaking_time']
            self.icemaking_temp_data['water_capacity'] = self.icemaking_data['water_capacity']
            self.icemaking_temp_data['swing_on_time'] = self.icemaking_data['swing_on_time']
            self.icemaking_temp_data['swing_off_time'] = self.icemaking_data['swing_off_time']
            
            # Entry 위젯들을 편집 가능하게 설정
            self.icemaking_labels['target_rps'].config(state='normal', bg='lightyellow')
            self.icemaking_labels['icemaking_time'].config(state='normal', bg='lightyellow')
            self.icemaking_labels['water_capacity'].config(state='normal', bg='lightyellow')
            self.icemaking_labels['swing_on_time'].config(state='normal', bg='lightyellow')
            self.icemaking_labels['swing_off_time'].config(state='normal', bg='lightyellow')
            
            # Entry 위젯에 현재 값 설정
            self.icemaking_labels['target_rps'].delete(0, tk.END)
            self.icemaking_labels['target_rps'].insert(0, str(self.icemaking_temp_data['target_rps']))
            
            # 제빙 동작 라벨 UI 업데이트
            if self.icemaking_temp_data['operation'] == '동작':
                self.icemaking_labels['operation'].config(text="동작", bg="green")
            else:
                self.icemaking_labels['operation'].config(text="대기", bg="blue")
            
            # 버튼 텍스트 변경
            self.icemaking_send_btn.config(text="설정 완료 (CMD 0xB2 전송)")
            
            self.log_communication("제빙 설정 입력 모드 활성화", "purple")
        
        else:
            # ========== 입력 모드 비활성화 및 데이터 전송 ==========
            try:
                # 입력 값 가져오기
                target_rps_str = self.icemaking_labels['target_rps'].get()
                icemaking_time_str = self.icemaking_labels['icemaking_time'].get()
                water_capacity_str = self.icemaking_labels['water_capacity'].get()
                swing_on_str = self.icemaking_labels['swing_on_time'].get()
                swing_off_str = self.icemaking_labels['swing_off_time'].get()
                
                # 빈 값 체크
                if not target_rps_str or not icemaking_time_str or not water_capacity_str or not swing_on_str or not swing_off_str:
                    messagebox.showwarning("경고", "모든 값을 입력해주세요.")
                    return
                
                # 정수로 변환
                target_rps = int(float(target_rps_str))
                icemaking_time = int(float(icemaking_time_str))  # ms 단위
                water_capacity = int(float(water_capacity_str))  # Hz 단위
                swing_on = int(float(swing_on_str))              # ms 단위
                swing_off = int(float(swing_off_str))            # ms 단위
                
                # 범위 체크
                if not (constants.RPS_MIN <= target_rps <= constants.RPS_MAX):
                    messagebox.showwarning("경고", f"목표 RPS는 {constants.RPS_MIN}~{constants.RPS_MAX} 범위여야 합니다.")
                    return
                
                # 범위 체크
                if not (0 <= icemaking_time <= 65535):
                    messagebox.showwarning("경고", "제빙시간은 0~65535ms 범위여야 합니다.")
                    return
                
                if not (0 <= water_capacity <= 65535):
                    messagebox.showwarning("경고", "입수 용량은 0~65535Hz 범위여야 합니다.")
                    return
                
                if not (0 <= swing_on <= 255):
                    messagebox.showwarning("경고", "스윙바 ON 시간은 0~255ms 범위여야 합니다.")
                    return
                
                if not (0 <= swing_off <= 255):
                    messagebox.showwarning("경고", "스윙바 OFF 시간은 0~255ms 범위여야 합니다.")
                    return
                
                # TARGET TEMP 가져오기 (기본값 사용)
                target_temp = self.icemaking_data.get('target_temp', 0)
                icemaking_operation = 1 if self.icemaking_temp_data['operation'] == '동작' else 0
                
                # 범위 체크
                if not (-127 <= target_temp <= 127):
                    messagebox.showwarning("경고", "목표 온도는 -127~127℃ 범위여야 합니다.")
                    return
                
                # 임시 저장소의 값을 실제 데이터로 복사
                self.icemaking_data['operation'] = self.icemaking_temp_data['operation']
                self.icemaking_data['target_rps'] = target_rps
                self.icemaking_data['icemaking_time'] = icemaking_time
                self.icemaking_data['water_capacity'] = water_capacity
                self.icemaking_data['swing_on_time'] = swing_on
                self.icemaking_data['swing_off_time'] = swing_off
                
                # DATA FIELD 구성 (7바이트)
                data_field = bytearray(7)
                data_field[0] = target_rps  # TARGET RPS
                data_field[1] = self.comm.protocol.int_to_signed_byte(target_temp)  # TARGET TEMP (signed byte)
                data_field[2] = icemaking_operation  # 제빙 동작 (0: 대기, 1: 동작)
                data_field[3] = (icemaking_time >> 8) & 0xFF  # 제빙시간(ms) 상위 1B
                data_field[4] = icemaking_time & 0xFF  # 제빙시간(ms) 하위 1B
                data_field[5] = (water_capacity >> 8) & 0xFF  # 입수용량(Hz) 상위 1BYTE
                data_field[6] = water_capacity & 0xFF  # 입수용량(Hz) 하위 1BYTE
                
                # 로그 출력
                hex_data = " ".join([f"{b:02X}" for b in data_field])
                self.log_communication(f"[제빙 제어] CMD 0xB2 전송 (7바이트)", "blue")
                self.log_communication(f"  목표 RPS: {target_rps}", "gray")
                self.log_communication(f"  목표 온도: {target_temp}℃", "gray")
                self.log_communication(f"  제빙 동작: {self.icemaking_temp_data['operation']} (0x{data_field[2]:02X})", "gray")
                self.log_communication(f"  제빙시간: {icemaking_time}ms", "gray")
                self.log_communication(f"  입수 용량: {water_capacity}Hz", "gray")
                self.log_communication(f"  DATA FIELD (HEX): {hex_data}", "gray")
                
                # CMD 0xB2 패킷 전송
                success, message = self.comm.send_packet(0xB2, bytes(data_field))
                
                if success:
                    self.log_communication(f"  전송 성공 (CMD 0xB2, 7바이트)", "green")
                    
                    # 입력 모드 비활성화
                    self.icemaking_edit_mode = False
                    
                    # Entry 위젯들을 읽기 전용으로 설정
                    self.icemaking_labels['target_rps'].config(state='readonly', bg='white')
                    self.icemaking_labels['icemaking_time'].config(state='readonly', bg='white')
                    self.icemaking_labels['water_capacity'].config(state='readonly', bg='white')
                    self.icemaking_labels['swing_on_time'].config(state='readonly', bg='white')
                    self.icemaking_labels['swing_off_time'].config(state='readonly', bg='white')
                    
                    # 버튼 텍스트 변경
                    self.icemaking_send_btn.config(text="제빙 설정 입력 모드")
                    
                else:
                    self.log_communication(f"  전송 실패: {message}", "red")
                    
            except ValueError:
                messagebox.showerror("오류", "올바른 숫자를 입력해주세요.")
            except Exception as e:
                # self.log_communication(f"제빙 제어 오류: {str(e)}", "red")_communication(f"  스윙바 OFF: {swing_off}ms", "gray")
                self.log_communication(f"  DATA FIELD (HEX): {hex_data}", "gray")
                
                # CMD 0xB2 패킷 전송
                success, message = self.comm.send_packet(0xB2, bytes(data_field))
                
                if success:
                    self.log_communication(f"  전송 성공 (CMD 0xB2, 5바이트)", "green")
                    
                    # 입력 모드 비활성화
                    self.icemaking_edit_mode = False
                    
                    # Entry 위젯들을 읽기 전용으로 설정
                    self.icemaking_labels['icemaking_time'].config(state='readonly', bg='white')
                    self.icemaking_labels['water_capacity'].config(state='readonly', bg='white')
                    self.icemaking_labels['swing_on_time'].config(state='readonly', bg='white')
                    self.icemaking_labels['swing_off_time'].config(state='readonly', bg='white')
                    
                    # 버튼 텍스트 변경
                    self.icemaking_send_btn.config(text="제빙 설정 입력 모드")
                    
                else:
                    self.log_communication(f"  전송 실패: {message}", "red")
                    
            except ValueError:
                messagebox.showerror("오류", "올바른 숫자를 입력해주세요.")
            except Exception as e:
                self.log_communication(f"제빙 제어 오류: {str(e)}", "red")
    
    def toggle_refrigerant_valve_target(self, event):
        """냉매전환밸브 목표 토글 (냉각->제빙->핫가스->냉각)"""

        if not self.comm.is_connected:
            messagebox.showwarning("경고", "시리얼 포트가 연결되지 않았습니다.")
            return
        
        valve_sequence = ['냉각', '제빙', '핫가스']
        
        # 입력 모드 여부에 따라 다른 데이터 사용
        if self.hvac_edit_mode:
            current_value = self.hvac_temp_data['refrigerant_valve_target']
        else:
            current_value = self.hvac_data['refrigerant_valve_target']
        
        try:
            current_index = valve_sequence.index(current_value)
            next_index = (current_index + 1) % 3
            next_value = valve_sequence[next_index]
        except ValueError:
            next_value = '냉각'
        
        # UI 업데이트
        colors = {'냉각': 'green', '제빙': 'blue', '핫가스': 'red'}
        self.hvac_labels['refrigerant_valve_target'].config(
            text=next_value, 
            bg=colors.get(next_value, 'orange')
        )
        
        if self.hvac_edit_mode:
            # 입력 모드: 임시 저장소에만 저장 (전송은 입력 모드 해제 시)
            self.hvac_temp_data['refrigerant_valve_target'] = next_value
            self.log_communication(f"냉매전환밸브 목표 변경: {next_value} (입력 모드)", "purple")
            print(f"DEBUG: 임시 저장소에 저장됨")  # 디버그
        else:
            pass

    def toggle_compressor_state(self, event):
        """압축기 상태 토글 (동작중<->미동작)"""
        print(f"DEBUG: toggle_compressor_state 호출됨, hvac_edit_mode={self.hvac_edit_mode}")  # 디버그
        
        if not self.comm.is_connected:
            messagebox.showwarning("경고", "시리얼 포트가 연결되지 않았습니다.")
            return
        
        # 입력 모드 여부에 따라 다른 데이터 사용
        if self.hvac_edit_mode:
            current_value = self.hvac_temp_data['compressor_state']
        else:
            current_value = self.hvac_data['compressor_state']
        
        next_value = '미동작' if current_value == '동작중' else '동작중'
        print(f"DEBUG: {current_value} → {next_value}")  # 디버그
        
        # UI 업데이트
        if next_value == '동작중':
            self.hvac_labels['compressor_state'].config(text="동작중", bg="green")
        else:
            self.hvac_labels['compressor_state'].config(text="미동작", bg="gray")
        
        if self.hvac_edit_mode:
            # 입력 모드: 임시 저장소에만 저장 (전송은 입력 모드 해제 시)
            self.hvac_temp_data['compressor_state'] = next_value
            self.log_communication(f"압축기 상태 변경: {next_value} (입력 모드)", "purple")
        else:
            # 일반 모드: 즉시 데이터 업데이트 및 전송
            self.hvac_data['compressor_state'] = next_value
            self.send_hvac_immediate()

    def toggle_dc_fan1(self, event):
        """압축기 팬 토글 (ON<->OFF)"""
        print(f"DEBUG: toggle_dc_fan1 호출됨, hvac_edit_mode={self.hvac_edit_mode}")  # 디버그
        
        if not self.comm.is_connected:
            messagebox.showwarning("경고", "시리얼 포트가 연결되지 않았습니다.")
            return
        
        # 입력 모드 여부에 따라 다른 데이터 사용
        if self.hvac_edit_mode:
            current_value = self.hvac_temp_data['dc_fan1']
        else:
            current_value = self.hvac_data['dc_fan1']
        
        next_value = 'OFF' if current_value == 'ON' else 'ON'
        print(f"DEBUG: {current_value} → {next_value}")  # 디버그
        
        # UI 업데이트
        if next_value == 'ON':
            self.hvac_labels['dc_fan1'].config(text="ON", bg="green")
        else:
            self.hvac_labels['dc_fan1'].config(text="OFF", bg="gray")
        
        if self.hvac_edit_mode:
            # 입력 모드: 임시 저장소에만 저장 (전송은 입력 모드 해제 시)
            self.hvac_temp_data['dc_fan1'] = next_value
            self.log_communication(f"압축기 팬 변경: {next_value} (입력 모드)", "purple")
        else:
            # 일반 모드: 즉시 데이터 업데이트 및 전송
            self.hvac_data['dc_fan1'] = next_value
            self.send_hvac_immediate()

    def toggle_dc_fan2(self, event):
        """얼음탱크 팬 토글 (ON<->OFF)"""
        print(f"DEBUG: toggle_dc_fan2 호출됨, hvac_edit_mode={self.hvac_edit_mode}")  # 디버그
        
        if not self.comm.is_connected:
            messagebox.showwarning("경고", "시리얼 포트가 연결되지 않았습니다.")
            return
        
        # 입력 모드 여부에 따라 다른 데이터 사용
        if self.hvac_edit_mode:
            current_value = self.hvac_temp_data['dc_fan2']
        else:
            current_value = self.hvac_data['dc_fan2']
        
        next_value = 'OFF' if current_value == 'ON' else 'ON'
        print(f"DEBUG: {current_value} → {next_value}")  # 디버그
        
        # UI 업데이트
        if next_value == 'ON':
            self.hvac_labels['dc_fan2'].config(text="ON", bg="green")
        else:
            self.hvac_labels['dc_fan2'].config(text="OFF", bg="gray")
        
        if self.hvac_edit_mode:
            # 입력 모드: 임시 저장소에만 저장 (전송은 입력 모드 해제 시)
            self.hvac_temp_data['dc_fan2'] = next_value
            self.log_communication(f"얼음탱크 팬 변경: {next_value} (입력 모드)", "purple")
        else:
            # 일반 모드: 즉시 데이터 업데이트 및 전송
            self.hvac_data['dc_fan2'] = next_value
            self.send_hvac_immediate()

    def toggle_compressor_state(self, event):
        """압축기 상태 토글 (동작중<->미동작)"""
        if not self.comm.is_connected:
            messagebox.showwarning("경고", "시리얼 포트가 연결되지 않았습니다.")
            return
        
        # 입력 모드 여부에 따라 다른 데이터 사용
        if self.hvac_edit_mode:
            current_value = self.hvac_temp_data['compressor_state']
        else:
            current_value = self.hvac_data['compressor_state']
        
        next_value = '미동작' if current_value == '동작중' else '동작중'
        
        # UI 업데이트
        if next_value == '동작중':
            self.hvac_labels['compressor_state'].config(text="동작중", bg="green")
        else:
            self.hvac_labels['compressor_state'].config(text="미동작", bg="gray")
        
        if self.hvac_edit_mode:
            # 입력 모드: 임시 저장소에만 저장 (전송은 입력 모드 해제 시)
            self.hvac_temp_data['compressor_state'] = next_value
            self.log_communication(f"압축기 상태 변경: {next_value} (입력 모드)", "purple")
        else:
            # 일반 모드: 즉시 데이터 업데이트 및 전송
            self.hvac_data['compressor_state'] = next_value
            self.send_hvac_immediate()

    def toggle_dc_fan1(self, event):
        """압축기 팬 토글 (ON<->OFF)"""
        if not self.comm.is_connected:
            messagebox.showwarning("경고", "시리얼 포트가 연결되지 않았습니다.")
            return
        
        # 입력 모드 여부에 따라 다른 데이터 사용
        if self.hvac_edit_mode:
            current_value = self.hvac_temp_data['dc_fan1']
        else:
            current_value = self.hvac_data['dc_fan1']
        
        next_value = 'OFF' if current_value == 'ON' else 'ON'
        
        # UI 업데이트
        if next_value == 'ON':
            self.hvac_labels['dc_fan1'].config(text="ON", bg="green")
        else:
            self.hvac_labels['dc_fan1'].config(text="OFF", bg="gray")
        
        if self.hvac_edit_mode:
            # 입력 모드: 임시 저장소에만 저장 (전송은 입력 모드 해제 시)
            self.hvac_temp_data['dc_fan1'] = next_value
            self.log_communication(f"압축기 팬 변경: {next_value} (입력 모드)", "purple")
        else:
            # 일반 모드: 즉시 데이터 업데이트 및 전송
            self.hvac_data['dc_fan1'] = next_value
            self.send_hvac_immediate()

    def toggle_dc_fan2(self, event):
        """얼음탱크 팬 토글 (ON<->OFF)"""
        if not self.comm.is_connected:
            messagebox.showwarning("경고", "시리얼 포트가 연결되지 않았습니다.")
            return
        
        # 입력 모드 여부에 따라 다른 데이터 사용
        if self.hvac_edit_mode:
            current_value = self.hvac_temp_data['dc_fan2']
        else:
            current_value = self.hvac_data['dc_fan2']
        
        next_value = 'OFF' if current_value == 'ON' else 'ON'
        
        # UI 업데이트
        if next_value == 'ON':
            self.hvac_labels['dc_fan2'].config(text="ON", bg="green")
        else:
            self.hvac_labels['dc_fan2'].config(text="OFF", bg="gray")
        
        if self.hvac_edit_mode:
            # 입력 모드: 임시 저장소에만 저장 (전송은 입력 모드 해제 시)
            self.hvac_temp_data['dc_fan2'] = next_value
            self.log_communication(f"얼음탱크 팬 변경: {next_value} (입력 모드)", "purple")
        else:
            # 일반 모드: 즉시 데이터 업데이트 및 전송
            self.hvac_data['dc_fan2'] = next_value
            self.send_hvac_immediate()

    def toggle_compressor_state(self, event):
        """압축기 상태 토글 (동작중<->미동작)"""
        if not self.hvac_edit_mode:
            return
        
        current_value = self.hvac_temp_data['compressor_state']
        next_value = '미동작' if current_value == '동작중' else '동작중'
        self.hvac_temp_data['compressor_state'] = next_value
        
        # UI 업데이트
        if next_value == '동작중':
            self.hvac_labels['compressor_state'].config(text="동작중", bg="green")
        else:
            self.hvac_labels['compressor_state'].config(text="미동작", bg="gray")

    def toggle_dc_fan1(self, event):
        """압축기 팬 토글 (ON<->OFF)"""
        if not self.hvac_edit_mode:
            return
        
        current_value = self.hvac_temp_data['dc_fan1']
        next_value = 'OFF' if current_value == 'ON' else 'ON'
        self.hvac_temp_data['dc_fan1'] = next_value
        
        # UI 업데이트
        if next_value == 'ON':
            self.hvac_labels['dc_fan1'].config(text="ON", bg="green")
        else:
            self.hvac_labels['dc_fan1'].config(text="OFF", bg="gray")

    def toggle_dc_fan2(self, event):
        """얼음탱크 팬 토글 (ON<->OFF)"""
        if not self.hvac_edit_mode:
            return
        
        current_value = self.hvac_temp_data['dc_fan2']
        next_value = 'OFF' if current_value == 'ON' else 'ON'
        self.hvac_temp_data['dc_fan2'] = next_value
        
        # UI 업데이트
        if next_value == 'ON':
            self.hvac_labels['dc_fan2'].config(text="ON", bg="green")
        else:
            self.hvac_labels['dc_fan2'].config(text="OFF", bg="gray")

    # ============================================
    # 3. 제빙 동작 토글 함수 추가
    # ============================================
    def toggle_icemaking_operation(self, event):
        """제빙 동작 토글 (대기<->동작)"""
        if not self.icemaking_edit_mode:
            return
        
        current_value = self.icemaking_temp_data['operation']
        next_value = '동작' if current_value == '대기' else '대기'
        self.icemaking_temp_data['operation'] = next_value
        
        # UI 업데이트
        if next_value == '동작':
            self.icemaking_labels['operation'].config(text="동작", bg="green")
        else:
            self.icemaking_labels['operation'].config(text="대기", bg="blue")

    def send_hvac_control(self):
        """공조 제어 CMD 0xB0 전송 - 입력 모드 토글 방식"""
        if not self.comm.is_connected:
            messagebox.showwarning("경고", "시리얼 포트가 연결되지 않았습니다.")
            return
        
        if not self.hvac_edit_mode:
            # ========== 입력 모드 활성화 ==========
            self.hvac_edit_mode = True
            
            # 현재 값을 임시 저장소에 복사
            self.hvac_temp_data['refrigerant_valve_target'] = self.hvac_data['refrigerant_valve_target']
            self.hvac_temp_data['compressor_state'] = self.hvac_data['compressor_state']
            self.hvac_temp_data['dc_fan1'] = self.hvac_data['dc_fan1']
            self.hvac_temp_data['dc_fan2'] = self.hvac_data['dc_fan2']
            
            # UI를 임시 저장소 값으로 초기화
            colors = {'냉각': 'green', '제빙': 'blue', '핫가스': 'red'}
            self.hvac_labels['refrigerant_valve_target'].config(
                text=self.hvac_temp_data['refrigerant_valve_target'],
                bg=colors.get(self.hvac_temp_data['refrigerant_valve_target'], 'orange')
            )
            
            if self.hvac_temp_data['compressor_state'] == '동작중':
                self.hvac_labels['compressor_state'].config(text="동작중", bg="green")
            else:
                self.hvac_labels['compressor_state'].config(text="미동작", bg="gray")
            
            if self.hvac_temp_data['dc_fan1'] == 'ON':
                self.hvac_labels['dc_fan1'].config(text="ON", bg="green")
            else:
                self.hvac_labels['dc_fan1'].config(text="OFF", bg="gray")
            
            if self.hvac_temp_data['dc_fan2'] == 'ON':
                self.hvac_labels['dc_fan2'].config(text="ON", bg="green")
            else:
                self.hvac_labels['dc_fan2'].config(text="OFF", bg="gray")
            
            # 버튼 텍스트 변경
            self.hvac_send_btn.config(text="설정 완료 (CMD 0xB0 전송)")
            
            self.log_communication("공조 설정 입력 모드 활성화 (모든 설정 변경 가능)", "purple")
        
        else:
            # ========== 입력 모드 비활성화 및 데이터 전송 ==========
            try:
                # 임시 저장소의 모든 값을 실제 데이터로 복사
                self.hvac_data['refrigerant_valve_target'] = self.hvac_temp_data['refrigerant_valve_target']
                self.hvac_data['compressor_state'] = self.hvac_temp_data['compressor_state']
                self.hvac_data['dc_fan1'] = self.hvac_temp_data['dc_fan1']
                self.hvac_data['dc_fan2'] = self.hvac_temp_data['dc_fan2']
                
                # DATA FIELD 구성 (4바이트) - RPS 제거
                data_field = bytearray(4)
                
                # DATA 1: 냉매전환밸브 목표 (냉각=0, 제빙=1, 핫가스=2)
                valve_map = {'냉각': 0, '제빙': 1, '핫가스': 2}
                data_field[0] = valve_map[self.hvac_data['refrigerant_valve_target']]
                
                # DATA 2: 압축기 상태 (동작=1, 미동작=0)
                data_field[1] = 1 if self.hvac_data['compressor_state'] == '동작중' else 0
                
                # DATA 3: DC FAN 1 (압축기 팬, ON=1, OFF=0)
                data_field[2] = 1 if self.hvac_data['dc_fan1'] == 'ON' else 0
                
                # DATA 4: DC FAN 2 (얼음탱크 팬, ON=1, OFF=0)
                data_field[3] = 1 if self.hvac_data['dc_fan2'] == 'ON' else 0
                
                # 로그 출력
                hex_data = " ".join([f"{b:02X}" for b in data_field])
                self.log_communication(f"[공조 제어] CMD 0xB0 전송 (입력 모드 최종 설정)", "blue")
                self.log_communication(f"  냉매전환밸브 목표: {self.hvac_data['refrigerant_valve_target']} ({data_field[0]})", "gray")
                self.log_communication(f"  압축기 상태: {self.hvac_data['compressor_state']} ({data_field[1]})", "gray")
                self.log_communication(f"  압축기 팬: {self.hvac_data['dc_fan1']} ({data_field[2]})", "gray")
                self.log_communication(f"  얼음탱크 팬: {self.hvac_data['dc_fan2']} ({data_field[3]})", "gray")
                self.log_communication(f"  DATA FIELD (HEX): {hex_data}", "gray")
                
                # CMD 0xB0 패킷 전송
                success, message = self.comm.send_packet(0xB0, bytes(data_field))
                
                if success:
                    self.log_communication(f"  전송 성공 (CMD 0xB0, 4바이트)", "green")
                    
                    # 입력 모드 비활성화
                    self.hvac_edit_mode = False
                    
                    # 버튼 텍스트 변경
                    self.hvac_send_btn.config(text="입력모드")
                    
                else:
                    self.log_communication(f"  전송 실패: {message}", "red")
                    
            except ValueError:
                messagebox.showerror("오류", "올바른 숫자를 입력해주세요.")
            except Exception as e:
                self.log_communication(f"공조 제어 오류: {str(e)}", "red")
    
    def create_hvac_area(self, parent):
        """공조시스템 섹션 생성"""
        hvac_frame = ttk.LabelFrame(parent, text="공조시스템", padding="2")
        hvac_frame.grid(row=0, column=1, sticky=(tk.W, tk.E, tk.N, tk.S), padx=1)
        
        # 냉매전환밸브 서브프레임
        valve_subframe = ttk.LabelFrame(hvac_frame, text="냉매전환밸브", padding="3")
        valve_subframe.grid(row=0, column=0, columnspan=2, sticky=(tk.W, tk.E), pady=(0, 5))
        
        # 상태
        state_frame = ttk.Frame(valve_subframe)
        state_frame.grid(row=0, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(state_frame, text="상태:", font=("Arial", 8)).pack(side=tk.LEFT)
        self.hvac_labels['refrigerant_valve_state'] = tk.Label(state_frame, text="핫가스", 
                                                            fg="white", bg="red", font=("Arial", 8, "bold"),
                                                            width=8, relief="raised")
        self.hvac_labels['refrigerant_valve_state'].pack(side=tk.RIGHT)
        
        # 목표 (버튼으로 변경 - 클릭 가능하게 설정)
        target_frame = ttk.Frame(valve_subframe)
        target_frame.grid(row=1, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(target_frame, text="목표:", font=("Arial", 8)).pack(side=tk.LEFT)
        self.hvac_labels['refrigerant_valve_target'] = tk.Label(target_frame, text="핫가스", 
                                                            fg="white", bg="orange", font=("Arial", 8, "bold"),
                                                            width=8, relief="raised", cursor="hand2")
        self.hvac_labels['refrigerant_valve_target'].pack(side=tk.RIGHT)
        # 클릭 이벤트 바인딩 확인
        self.hvac_labels['refrigerant_valve_target'].bind("<Button-1>", self.toggle_refrigerant_valve_target)
        
        # 압축기 서브프레임
        comp_subframe = ttk.LabelFrame(hvac_frame, text="압축기", padding="3")
        comp_subframe.grid(row=1, column=0, columnspan=2, sticky=(tk.W, tk.E), pady=(0, 5))
        
        # 상태 (버튼으로 변경 - 클릭 가능하게 설정)
        comp_state_frame = ttk.Frame(comp_subframe)
        comp_state_frame.grid(row=0, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(comp_state_frame, text="상태:", font=("Arial", 8)).pack(side=tk.LEFT)
        self.hvac_labels['compressor_state'] = tk.Label(comp_state_frame, text="미동작", 
                                                    fg="white", bg="gray", font=("Arial", 8, "bold"),
                                                    width=8, relief="raised", cursor="hand2")
        self.hvac_labels['compressor_state'].pack(side=tk.RIGHT)
        # 클릭 이벤트 바인딩 확인
        self.hvac_labels['compressor_state'].bind("<Button-1>", self.toggle_compressor_state)
        
        # 현재 RPS
        curr_rps_frame = ttk.Frame(comp_subframe)
        curr_rps_frame.grid(row=1, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(curr_rps_frame, text="현재 RPS:", font=("Arial", 8)).pack(side=tk.LEFT)
        self.hvac_labels['current_rps'] = tk.Label(curr_rps_frame, text="0", 
                                                font=("Arial", 8), bg="white", relief="sunken")
        self.hvac_labels['current_rps'].pack(side=tk.RIGHT)
        
        # 에러코드
        error_frame = ttk.Frame(comp_subframe)
        error_frame.grid(row=2, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(error_frame, text="에러코드:", font=("Arial", 8)).pack(side=tk.LEFT)
        self.hvac_labels['error_code'] = tk.Label(error_frame, text="0", 
                                                font=("Arial", 8), bg="white", relief="sunken")
        self.hvac_labels['error_code'].pack(side=tk.RIGHT)
        
        # DC FAN 1 (압축기 팬, 버튼으로 변경 - 클릭 가능하게 설정)
        fan1_frame = ttk.Frame(comp_subframe)
        fan1_frame.grid(row=3, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(fan1_frame, text="압축기 팬:", font=("Arial", 8)).pack(side=tk.LEFT)
        self.hvac_labels['dc_fan1'] = tk.Label(fan1_frame, text="OFF", 
                                            fg="white", bg="gray", font=("Arial", 8, "bold"),
                                            width=5, relief="raised", cursor="hand2")
        self.hvac_labels['dc_fan1'].pack(side=tk.RIGHT)
        # 클릭 이벤트 바인딩 확인
        self.hvac_labels['dc_fan1'].bind("<Button-1>", self.toggle_dc_fan1)
        
        # DC FAN 2 (얼음탱크 팬, 버튼으로 변경 - 클릭 가능하게 설정)
        fan2_frame = ttk.Frame(comp_subframe)
        fan2_frame.grid(row=4, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(fan2_frame, text="얼음탱크 팬:", font=("Arial", 8)).pack(side=tk.LEFT)
        self.hvac_labels['dc_fan2'] = tk.Label(fan2_frame, text="OFF", 
                                            fg="white", bg="gray", font=("Arial", 8, "bold"),
                                            width=5, relief="raised", cursor="hand2")
        self.hvac_labels['dc_fan2'].pack(side=tk.RIGHT)
        # 클릭 이벤트 바인딩 확인
        self.hvac_labels['dc_fan2'].bind("<Button-1>", self.toggle_dc_fan2)
        
        # CMD 0xB0 전송 버튼
        send_btn_frame = ttk.Frame(hvac_frame)
        send_btn_frame.grid(row=2, column=0, columnspan=2, sticky=(tk.W, tk.E), pady=(5, 1))
        self.hvac_send_btn = ttk.Button(send_btn_frame, text="입력모드",
                                        command=self.send_hvac_control, state="disabled")
        self.hvac_send_btn.pack(fill=tk.X)
        
        valve_subframe.columnconfigure(0, weight=1)
        comp_subframe.columnconfigure(0, weight=1)
        hvac_frame.columnconfigure(0, weight=1)
    
    # ============================================
    # 2. create_icemaking_area 메서드 전체 교체
    # ============================================
    def create_icemaking_area(self, parent):
        """제빙 섹션 생성"""
        icemaking_frame = ttk.LabelFrame(parent, text="제빙", padding="2")
        icemaking_frame.grid(row=0, column=2, sticky=(tk.W, tk.E, tk.N, tk.S), padx=1)
        
        # 제빙 동작 (토글 버튼으로 변경)
        operation_frame = ttk.Frame(icemaking_frame)
        operation_frame.grid(row=0, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(operation_frame, text="제빙 동작:", font=("Arial", 9), width=9).pack(side=tk.LEFT)
        self.icemaking_labels['operation'] = tk.Label(operation_frame, text="대기", 
                                                    fg="white", bg="blue", font=("Arial", 8, "bold"),
                                                    width=10, relief="raised", cursor="hand2")
        self.icemaking_labels['operation'].pack(side=tk.LEFT, padx=(2, 0))
        self.icemaking_labels['operation'].bind("<Button-1>", self.toggle_icemaking_operation)
        
        # 목표 RPS (입력 가능)
        target_rps_frame = ttk.Frame(icemaking_frame)
        target_rps_frame.grid(row=1, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(target_rps_frame, text="목표 RPS:", font=("Arial", 9), width=9).pack(side=tk.LEFT)
        vcmd_rps = (self.root.register(self.validate_rps), '%P')
        self.icemaking_labels['target_rps'] = tk.Entry(target_rps_frame, font=("Arial", 9), 
                                             width=8, validate='key', validatecommand=vcmd_rps,
                                             state='readonly')
        self.icemaking_labels['target_rps'].insert(0, "0")
        self.icemaking_labels['target_rps'].pack(side=tk.LEFT, padx=(2, 0))
        
        # 제빙시간 (ms 단위, 입력 가능)
        time_frame = ttk.Frame(icemaking_frame)
        time_frame.grid(row=2, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(time_frame, text="제빙시간:", font=("Arial", 9), width=9).pack(side=tk.LEFT)
        
        vcmd_num = (self.root.register(self.validate_number), '%P')
        self.icemaking_labels['icemaking_time'] = tk.Entry(time_frame, font=("Arial", 9), 
                                                width=8, validate='key', validatecommand=vcmd_num,
                                                state='readonly')
        self.icemaking_labels['icemaking_time'].insert(0, "0")
        self.icemaking_labels['icemaking_time'].pack(side=tk.LEFT, padx=(2, 0))
        ttk.Label(time_frame, text="ms", font=("Arial", 9)).pack(side=tk.LEFT)
        
        # 입수 용량 (Hz 단위, 입력 가능)
        capacity_frame = ttk.Frame(icemaking_frame)
        capacity_frame.grid(row=3, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(capacity_frame, text="입수 용량:", font=("Arial", 9), width=9).pack(side=tk.LEFT)
        self.icemaking_labels['water_capacity'] = tk.Entry(capacity_frame, font=("Arial", 9), 
                                                    width=8, validate='key', validatecommand=vcmd_num,
                                                    state='readonly')
        self.icemaking_labels['water_capacity'].insert(0, "0")
        self.icemaking_labels['water_capacity'].pack(side=tk.LEFT, padx=(2, 0))
        ttk.Label(capacity_frame, text="Hz", font=("Arial", 9)).pack(side=tk.LEFT)
        
        # 스윙바 ON 시간 (ms 단위, 입력 가능)
        swing_on_frame = ttk.Frame(icemaking_frame)
        swing_on_frame.grid(row=4, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(swing_on_frame, text="스윙바 ON:", font=("Arial", 9), width=9).pack(side=tk.LEFT)
        self.icemaking_labels['swing_on_time'] = tk.Entry(swing_on_frame, font=("Arial", 9), 
                                                    width=8, validate='key', validatecommand=vcmd_num,
                                                    state='readonly')
        self.icemaking_labels['swing_on_time'].insert(0, "0")
        self.icemaking_labels['swing_on_time'].pack(side=tk.LEFT, padx=(2, 0))
        ttk.Label(swing_on_frame, text="ms", font=("Arial", 9)).pack(side=tk.LEFT)
        
        # 스윙바 OFF 시간 (ms 단위, 입력 가능)
        swing_off_frame = ttk.Frame(icemaking_frame)
        swing_off_frame.grid(row=5, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(swing_off_frame, text="스윙바 OFF:", font=("Arial", 9), width=9).pack(side=tk.LEFT)
        self.icemaking_labels['swing_off_time'] = tk.Entry(swing_off_frame, font=("Arial", 9), 
                                                    width=8, validate='key', validatecommand=vcmd_num,
                                                    state='readonly')
        self.icemaking_labels['swing_off_time'].insert(0, "0")
        self.icemaking_labels['swing_off_time'].pack(side=tk.LEFT, padx=(2, 0))
        ttk.Label(swing_off_frame, text="ms", font=("Arial", 9)).pack(side=tk.LEFT)
        
        # CMD 0xB2 전송 버튼
        send_btn_frame = ttk.Frame(icemaking_frame)
        send_btn_frame.grid(row=6, column=0, sticky=(tk.W, tk.E), pady=(5, 1))
        self.icemaking_send_btn = ttk.Button(send_btn_frame, text="제빙 설정 입력 모드",
                                        command=self.send_icemaking_control, state="disabled")
        self.icemaking_send_btn.pack(fill=tk.X)
        
        icemaking_frame.columnconfigure(0, weight=1)
    
    def create_graph_areas(self, parent):
        """그래프 영역 생성"""
        graph_container = ttk.Frame(parent)
        graph_container.grid(row=0, column=4, sticky=(tk.W, tk.E, tk.N, tk.S), padx=(2, 0))
        
        # 그래프 1
        graph1_frame = ttk.LabelFrame(graph_container, text="그래프 1", padding="3")
        graph1_frame.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S), pady=(0, 2))
        
        # 그래프 2
        graph2_frame = ttk.LabelFrame(graph_container, text="그래프 2", padding="3")
        graph2_frame.grid(row=1, column=0, sticky=(tk.W, tk.E, tk.N, tk.S), pady=(2, 0))
        
        # 그래프 생성 카운트
        if not hasattr(self, '_graph_creation_count'):
            self._graph_creation_count = 0
        self._graph_creation_count += 1
        is_freezing_tab = self._graph_creation_count == 1
        
        try:
            if is_freezing_tab:
                # 냉동검토용 탭 - 그래프 1
                self.fig1_freezing = Figure(figsize=(3.2, 2.0), dpi=80)
                self.temp_ax_freezing = self.fig1_freezing.add_subplot(1, 1, 1)
                self.temp_ax_freezing.set_title("Temperature Sensors (Freezing)", fontsize=8, fontfamily='DejaVu Sans')
                self.temp_ax_freezing.set_ylabel("Temperature (°C)", fontsize=7, fontfamily='DejaVu Sans')
                self.temp_ax_freezing.grid(True, alpha=0.3)
                self.fig1_freezing.tight_layout()
                
                self.canvas1_freezing = FigureCanvasTkAgg(self.fig1_freezing, graph1_frame)
                self.canvas1_freezing.draw()
                canvas1_widget = self.canvas1_freezing.get_tk_widget()
                canvas1_widget.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))
                
                # 냉동검토용 탭 - 그래프 2
                self.fig2_freezing = Figure(figsize=(3.2, 2.0), dpi=80)
                self.pressure_ax_freezing = self.fig2_freezing.add_subplot(1, 1, 1)
                self.pressure_ax_freezing.set_title("Other Sensors (Freezing)", fontsize=8, fontfamily='DejaVu Sans')
                self.pressure_ax_freezing.set_ylabel("Value", fontsize=7, fontfamily='DejaVu Sans')
                self.pressure_ax_freezing.set_xlabel("Time", fontsize=7, fontfamily='DejaVu Sans')
                self.pressure_ax_freezing.grid(True, alpha=0.3)
                self.fig2_freezing.tight_layout()
                
                self.canvas2_freezing = FigureCanvasTkAgg(self.fig2_freezing, graph2_frame)
                self.canvas2_freezing.draw()
                canvas2_widget = self.canvas2_freezing.get_tk_widget()
                canvas2_widget.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))
                
            else:
                # 제어검토용 탭 - 그래프 1
                self.fig1_control = Figure(figsize=(3.2, 2.0), dpi=80)
                self.temp_ax_control = self.fig1_control.add_subplot(1, 1, 1)
                self.temp_ax_control.set_title("Temperature Sensors (Control)", fontsize=8, fontfamily='DejaVu Sans')
                self.temp_ax_control.set_ylabel("Temperature (°C)", fontsize=7, fontfamily='DejaVu Sans')
                self.temp_ax_control.grid(True, alpha=0.3)
                self.fig1_control.tight_layout()
                
                self.canvas1_control = FigureCanvasTkAgg(self.fig1_control, graph1_frame)
                self.canvas1_control.draw()
                canvas1_widget = self.canvas1_control.get_tk_widget()
                canvas1_widget.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))
                
                # 제어검토용 탭 - 그래프 2
                self.fig2_control = Figure(figsize=(3.2, 2.0), dpi=80)
                self.pressure_ax_control = self.fig2_control.add_subplot(1, 1, 1)
                self.pressure_ax_control.set_title("Other Sensors (Control)", fontsize=8, fontfamily='DejaVu Sans')
                self.pressure_ax_control.set_ylabel("Value", fontsize=7, fontfamily='DejaVu Sans')
                self.pressure_ax_control.set_xlabel("Time", fontsize=7, fontfamily='DejaVu Sans')
                self.pressure_ax_control.grid(True, alpha=0.3)
                self.fig2_control.tight_layout()
                
                self.canvas2_control = FigureCanvasTkAgg(self.fig2_control, graph2_frame)
                self.canvas2_control.draw()
                canvas2_widget = self.canvas2_control.get_tk_widget()
                canvas2_widget.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))
            
        except Exception as e:
            error_label1 = tk.Label(graph1_frame, text=f"그래프1 오류: {str(e)}", fg="red", font=("Arial", 8))
            error_label1.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))
            
            error_label2 = tk.Label(graph2_frame, text=f"그래프2 오류: {str(e)}", fg="red", font=("Arial", 8))
            error_label2.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))
        
        graph1_frame.columnconfigure(0, weight=1)
        graph1_frame.rowconfigure(0, weight=1)
        graph2_frame.columnconfigure(0, weight=1)
        graph2_frame.rowconfigure(0, weight=1)
        graph_container.columnconfigure(0, weight=1)
        graph_container.rowconfigure(0, weight=1)
        graph_container.rowconfigure(1, weight=1)
    
    def create_valve_area(self, parent):
        """밸브류 섹션 생성"""
        valve_frame = ttk.LabelFrame(parent, text="밸브류", padding="3")
        valve_frame.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S), padx=(0, 2))
        
        # 현재 몇 번째 생성인지 확인 (냉동검토용=1, 제어검토용=2)
        if not hasattr(self, '_valve_creation_count'):
            self._valve_creation_count = 0
        self._valve_creation_count += 1
        is_freezing_tab = self._valve_creation_count == 1
        
        # NOS 밸브 서브프레임
        nos_frame = ttk.LabelFrame(valve_frame, text="NOS 밸브 (데이터 1=CLOSE, 0=OPEN)", padding="3")
        nos_frame.grid(row=1, column=0, columnspan=5, sticky=(tk.W, tk.E), pady=(0, 5))
        
        for i in range(1, 6):
            nos_container = ttk.Frame(nos_frame)
            nos_container.grid(row=0, column=i-1, padx=3, pady=2, sticky=tk.W)
            
            num_label = tk.Label(nos_container, text=f"NOS{i}:", font=("Arial", 7), width=6)
            num_label.pack(side=tk.TOP)
            
            status_label = tk.Label(nos_container, text="CLOSE", 
                                  fg="white", bg="red",
                                  font=("Arial", 7, "bold"),
                                  width=6, relief="raised", bd=1, cursor="hand2")
            status_label.pack(side=tk.TOP, pady=(2, 0))
            
            # 클릭 이벤트 바인딩 (밸브 제어 - 0xA0 전송)
            status_label.bind("<Button-1>", lambda e, valve_num=i: self.send_valve_control(valve_num, 'NOS'))
            
            # 탭별로 라벨 저장
            if is_freezing_tab:
                self.nos_valve_labels_freezing[i] = status_label
            else:
                self.nos_valve_labels_control[i] = status_label
        
        # FEED 밸브 서브프레임
        feed_frame = ttk.LabelFrame(valve_frame, text="FEED 밸브 (데이터 1=OPEN, 0=CLOSE)", padding="3")
        feed_frame.grid(row=2, column=0, columnspan=5, sticky=(tk.W, tk.E), pady=(0, 5))
        
        for i in range(1, 16):
            row = (i - 1) // 5
            col = (i - 1) % 5
            
            feed_container = ttk.Frame(feed_frame)
            feed_container.grid(row=row, column=col, padx=2, pady=2, sticky=tk.W)
            
            num_label = tk.Label(feed_container, text=f"F{i:2d}:", font=("Arial", 7), width=4)
            num_label.pack(side=tk.LEFT)
            
            status_label = tk.Label(feed_container, text="CLOSE", 
                                  fg="white", bg="red",
                                  font=("Arial", 6, "bold"),
                                  width=5, relief="raised", bd=1, cursor="hand2")
            status_label.pack(side=tk.LEFT, padx=(2, 0))
            
            # 클릭 이벤트 바인딩 (밸브 제어 - 0xA0 전송)
            status_label.bind("<Button-1>", lambda e, valve_num=i: self.send_valve_control(valve_num, 'FEED'))
            
            # 탭별로 라벨 저장
            if is_freezing_tab:
                self.feed_valve_labels_freezing[i] = status_label
            else:
                self.feed_valve_labels_control[i] = status_label
        
        for i in range(5):
            nos_frame.columnconfigure(i, weight=1)
            feed_frame.columnconfigure(i, weight=1)
    
    def create_sensor_area(self, parent):
        """센서류 섹션 생성"""
        sensor_frame = ttk.LabelFrame(parent, text="센서류", padding="2")
        sensor_frame.grid(row=0, column=1, sticky=(tk.W, tk.E, tk.N, tk.S), padx=2)
        
        sensors = [
            ('outdoor_temp1', '외기온도 1'),
            ('outdoor_temp2', '외기온도 2'),
            ('purified_temp', '정수온도'),
            ('cold_temp', '냉수온도'),
            ('hot_inlet_temp', '온수 입수온도'),
            ('hot_internal_temp', '온수 내부온도'),
            ('hot_outlet_temp', '온수 출수온도')
        ]
        
        for idx, (key, label_text) in enumerate(sensors):
            row = idx // 2
            col = idx % 2
            
            sensor_container = ttk.Frame(sensor_frame)
            sensor_container.grid(row=row, column=col, padx=3, pady=1, sticky=(tk.W, tk.E))
            
            ttk.Label(sensor_container, text=f"{label_text}:", font=("Arial", 7), width=12).pack(side=tk.LEFT)
            
            value_label = tk.Label(sensor_container, text="0.0", 
                                 fg="black", bg="white",
                                 font=("Arial", 7, "bold"),
                                 width=6, relief="sunken", bd=1, cursor="hand2")
            value_label.pack(side=tk.LEFT, padx=(2, 0))
            ttk.Label(sensor_container, text="℃", font=("Arial", 7)).pack(side=tk.LEFT)
            
            value_label.bind("<Button-1>", lambda e, sensor_key=key: self.toggle_graph2_item(sensor_key))
            
            self.sensor_labels[key] = value_label
        
        sensor_frame.columnconfigure(0, weight=1)
        sensor_frame.columnconfigure(1, weight=1)
    
    def create_drain_tank_area(self, parent):
        """드레인 탱크 섹션 생성"""
        drain_tank_frame = ttk.LabelFrame(parent, text="드레인 탱크", padding="2")
        drain_tank_frame.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S), padx=(0, 2))
        
        low_level_frame = ttk.Frame(drain_tank_frame)
        low_level_frame.grid(row=0, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(low_level_frame, text="저수위:", font=("Arial", 9), width=8).pack(side=tk.LEFT)
        self.drain_tank_labels['low_level'] = tk.Label(low_level_frame, text="미감지", 
                                                      fg="white", bg="gray", font=("Arial", 8, "bold"),
                                                      width=8, relief="raised")
        self.drain_tank_labels['low_level'].pack(side=tk.LEFT, padx=(2, 0))
        
        high_level_frame = ttk.Frame(drain_tank_frame)
        high_level_frame.grid(row=1, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(high_level_frame, text="만수위:", font=("Arial", 9), width=8).pack(side=tk.LEFT)
        self.drain_tank_labels['high_level'] = tk.Label(high_level_frame, text="미감지", 
                                                       fg="white", bg="gray", font=("Arial", 8, "bold"),
                                                       width=8, relief="raised")
        self.drain_tank_labels['high_level'].pack(side=tk.LEFT, padx=(2, 0))
        
        water_state_frame = ttk.Frame(drain_tank_frame)
        water_state_frame.grid(row=2, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(water_state_frame, text="수위상태:", font=("Arial", 9), width=8).pack(side=tk.LEFT)
        self.drain_tank_labels['water_level_state'] = tk.Label(water_state_frame, text="비어있음", 
                                                              fg="white", bg="blue", font=("Arial", 8, "bold"),
                                                              width=8, relief="raised")
        self.drain_tank_labels['water_level_state'].pack(side=tk.LEFT, padx=(2, 0))
        
        drain_tank_frame.columnconfigure(0, weight=1)
    
    def create_drain_pump_area(self, parent):
        """드레인 펌프 섹션 생성"""
        drain_pump_frame = ttk.LabelFrame(parent, text="드레인 펌프", padding="2")
        drain_pump_frame.grid(row=0, column=1, sticky=(tk.W, tk.E, tk.N, tk.S), padx=(1, 0))
        
        state_frame = ttk.Frame(drain_pump_frame)
        state_frame.grid(row=0, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(state_frame, text="운전 상태:", font=("Arial", 7), width=8).pack(side=tk.LEFT)
        self.drain_pump_labels['operation_state'] = tk.Label(state_frame, text="OFF", 
                                                           fg="white", bg="red", font=("Arial", 7, "bold"),
                                                           width=6, relief="raised")
        self.drain_pump_labels['operation_state'].pack(side=tk.LEFT, padx=(2, 0))
        
        drain_pump_frame.columnconfigure(0, weight=1)
    
    def create_control_sections(self, parent):
        """제어검토용 탭의 제어 관련 섹션들 생성"""
        # 제어 상태 섹션
        control_status_frame = ttk.LabelFrame(parent, text="제어 상태", padding="2")
        control_status_frame.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S), padx=(0, 1))
        
        self.control_labels = {}
        
        mode_frame = ttk.Frame(control_status_frame)
        mode_frame.grid(row=0, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(mode_frame, text="시스템 모드:", font=("Arial", 7), width=10).pack(side=tk.LEFT)
        self.control_labels['system_mode'] = tk.Label(mode_frame, text="자동", 
                                                     fg="white", bg="green", font=("Arial", 7, "bold"),
                                                     width=8, relief="raised")
        self.control_labels['system_mode'].pack(side=tk.LEFT, padx=(2, 0))
        
        # 제어 명령 섹션
        control_cmd_frame = ttk.LabelFrame(parent, text="제어 명령", padding="2")
        control_cmd_frame.grid(row=0, column=1, sticky=(tk.W, tk.E, tk.N, tk.S), padx=(1, 1))
        
        self.control_buttons = {}
        
        system_frame = ttk.Frame(control_cmd_frame)
        system_frame.grid(row=0, column=0, sticky=(tk.W, tk.E), pady=1)
        self.control_buttons['start'] = tk.Button(system_frame, text="시스템 시작", 
                                                 font=("Arial", 7), bg="lightgreen", width=10)
        self.control_buttons['start'].pack(side=tk.LEFT, padx=(0, 2))
        self.control_buttons['stop'] = tk.Button(system_frame, text="시스템 정지", 
                                                font=("Arial", 7), bg="lightcoral", width=10)
        self.control_buttons['stop'].pack(side=tk.LEFT)
        
        # 설정값 섹션
        setpoint_frame = ttk.LabelFrame(parent, text="설정값", padding="2")
        setpoint_frame.grid(row=0, column=2, sticky=(tk.W, tk.E, tk.N, tk.S), padx=(1, 1))
        
        self.setpoint_entries = {}
        
        temp_frame = ttk.Frame(setpoint_frame)
        temp_frame.grid(row=0, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(temp_frame, text="목표온도:", font=("Arial", 7), width=8).pack(side=tk.LEFT)
        self.setpoint_entries['target_temp'] = tk.Entry(temp_frame, font=("Arial", 7), width=6)
        self.setpoint_entries['target_temp'].pack(side=tk.LEFT, padx=(2, 0))
        ttk.Label(temp_frame, text="℃", font=("Arial", 7)).pack(side=tk.LEFT)
        
        # 알람 섹션
        alarm_frame = ttk.LabelFrame(parent, text="알람", padding="2")
        alarm_frame.grid(row=0, column=3, sticky=(tk.W, tk.E, tk.N, tk.S), padx=(1, 0))
        
        self.alarm_labels = {}
        
        temp_alarm_frame = ttk.Frame(alarm_frame)
        temp_alarm_frame.grid(row=0, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(temp_alarm_frame, text="온도알람:", font=("Arial", 7), width=8).pack(side=tk.LEFT)
        self.alarm_labels['temp_alarm'] = tk.Label(temp_alarm_frame, text="정상", 
                                                  fg="white", bg="green", font=("Arial", 7, "bold"),
                                                  width=6, relief="raised")
        self.alarm_labels['temp_alarm'].pack(side=tk.LEFT, padx=(2, 0))
        
        for i in range(4):
            parent.columnconfigure(i, weight=1)
        parent.rowconfigure(0, weight=1)
    
    def create_shared_communication_area(self, parent):
        """모든 탭에서 공용으로 사용하는 통신부 영역 생성"""
        comm_main_frame = ttk.LabelFrame(parent, text="통신 설정 (공용) - 프로토콜 적용", padding="3")
        comm_main_frame.pack(fill=tk.X, side=tk.BOTTOM)
        
        # 좌측: 통신 로그
        left_frame = ttk.Frame(comm_main_frame)
        left_frame.pack(side=tk.LEFT, fill=tk.BOTH, expand=True, padx=(0, 5))
        
        log_label = ttk.Label(left_frame, text="통신 로그:", font=("Arial", 8))
        log_label.pack(anchor=tk.W)
        
        self.comm_text = tk.Text(left_frame, height=4, width=40, font=("Arial", 7))
        self.comm_text.pack(fill=tk.BOTH, expand=True, pady=(2, 0))
        
        # 우측: 통신 설정
        right_frame = ttk.Frame(comm_main_frame)
        right_frame.pack(side=tk.RIGHT, fill=tk.Y)
        
        # 연결 상태
        status_frame = ttk.Frame(right_frame)
        status_frame.grid(row=0, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(status_frame, text="상태:", font=("Arial", 8), width=6).pack(side=tk.LEFT)
        self.status_label = tk.Label(status_frame, text="연결 안됨", 
                                   fg="red", font=("Arial", 8, "bold"))
        self.status_label.pack(side=tk.LEFT, padx=(2, 0))
        
        # 포트 선택
        port_frame = ttk.Frame(right_frame)
        port_frame.grid(row=1, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(port_frame, text="포트:", font=("Arial", 8), width=6).pack(side=tk.LEFT)
        self.port_var = tk.StringVar()
        self.port_combo = ttk.Combobox(port_frame, textvariable=self.port_var, 
                                      width=10, font=("Arial", 7), state="readonly")
        self.port_combo.pack(side=tk.LEFT, padx=(2, 2))
        
        self.refresh_ports_btn = ttk.Button(port_frame, text="⟳", 
                                           command=self.refresh_ports, width=3)
        self.refresh_ports_btn.pack(side=tk.LEFT)
        
        # 통신속도
        baud_frame = ttk.Frame(right_frame)
        baud_frame.grid(row=2, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(baud_frame, text="속도:", font=("Arial", 8), width=6).pack(side=tk.LEFT)
        self.baudrate_var = tk.StringVar(value="115200")
        self.baudrate_combo = ttk.Combobox(baud_frame, textvariable=self.baudrate_var,
                                 values=["9600", "19200", "38400", "57600", "115200"],
                                 width=8, font=("Arial", 7), state="readonly")
        self.baudrate_combo.pack(side=tk.LEFT, padx=(2, 0))
        
        # 연결 버튼
        self.connect_btn = ttk.Button(right_frame, text="연결", 
                                     command=self.toggle_connection)
        self.connect_btn.grid(row=3, column=0, sticky=(tk.W, tk.E), pady=(3, 0))
        
        # CMD 전송 버튼들 (0xA0 제외)
        cmd_frame = ttk.LabelFrame(right_frame, text="CMD 전송", padding="2")
        cmd_frame.grid(row=4, column=0, sticky=(tk.W, tk.E), pady=(3, 0))
        
        self.cmd_buttons = {}
        cmds = [0xA1, 0xB0, 0xB1, 0xB2, 0xB3, 0xC0]  # 0xA0 제거
        for cmd in cmds:
            btn = ttk.Button(cmd_frame, text=f"CMD 0x{cmd:02X}",
                           command=lambda c=cmd: self.send_cmd(c),
                           state="disabled")
            btn.pack(fill=tk.X, pady=1)
            self.cmd_buttons[cmd] = btn
        
        right_frame.columnconfigure(0, weight=1)
        
        # 포트 목록 초기화
        self.refresh_ports()
    
    def refresh_ports(self):
        """포트 목록 새로고침"""
        ports = self.comm.get_available_ports()
        self.port_combo['values'] = ports
        if ports:
            self.port_combo.set(ports[0])
        self.log_communication(f"포트 새로고침: {len(ports)}개 포트 발견", "blue")
    
    # ============================================
    # 5. toggle_connection 메서드에 제빙 버튼 활성화 추가
    # ============================================
    def toggle_connection(self):
        """연결/연결해제 토글"""
        if not self.comm.is_connected:
            port = self.port_var.get()
            baudrate = self.baudrate_var.get()
            
            if not port:
                messagebox.showerror("오류", "포트를 선택해주세요.")
                return
            
            success, message = self.comm.connect(port, baudrate)
            if success:
                self.connect_btn.config(text="연결해제")
                self.status_label.config(text="연결됨", fg="green")
                self.port_combo.config(state="disabled")
                self.baudrate_combo.config(state="disabled")
                self.refresh_ports_btn.config(state="disabled")
                
                # CMD 버튼 활성화
                for btn in self.cmd_buttons.values():
                    btn.config(state="normal")
                
                # 냉각 제어 버튼 활성화
                if hasattr(self, 'cooling_send_btn'):
                    self.cooling_send_btn.config(state="normal")

                # 공조 제어 버튼 활성화
                if hasattr(self, 'hvac_send_btn'):
                    self.hvac_send_btn.config(state="normal")

                # 제빙 제어 버튼 활성화 추가
                if hasattr(self, 'icemaking_send_btn'):
                    self.icemaking_send_btn.config(state="normal")
                
                # 보냉 제어 버튼 활성화
                self.refrigeration_system.set_connection_state(True)
                
                self.log_communication(f"포트 {port} 연결됨", "green")
            else:
                self.log_communication(f"연결 실패: {message}", "red")
                messagebox.showerror("연결 오류", message)
        else:
            success, message = self.comm.disconnect()
            if success:
                self.connect_btn.config(text="연결")
                self.status_label.config(text="연결 안됨", fg="red")
                self.port_combo.config(state="readonly")
                self.baudrate_combo.config(state="readonly")
                self.refresh_ports_btn.config(state="normal")
                
                # CMD 버튼 비활성화
                for btn in self.cmd_buttons.values():
                    btn.config(state="disabled")
                
                # 냉각 제어 버튼 비활성화
                if hasattr(self, 'cooling_send_btn'):
                    self.cooling_send_btn.config(state="disabled")
                
                # 공조 제어 버튼 비활성화
                if hasattr(self, 'hvac_send_btn'):
                    self.hvac_send_btn.config(state="disabled")

                # 제빙 제어 버튼 비활성화 추가
                if hasattr(self, 'icemaking_send_btn'):
                    self.icemaking_send_btn.config(state="disabled")
                
                # 보냉 제어 버튼 비활성화
                self.refrigeration_system.set_connection_state(False)
                
                self.log_communication("연결 해제됨", "orange")
    
    def send_valve_control(self, valve_num, valve_type):
        """밸브 제어 CMD 0xA0 전송 (개별 밸브만 제어)"""
        if not self.comm.is_connected:
            messagebox.showwarning("경고", "시리얼 포트가 연결되지 않았습니다.")
            return
        
        try:
            # 현재 모든 밸브 상태를 DATA FIELD로 구성
            # DATA 1~5: NOS 1~5 (1=CLOSE, 0=OPEN)
            # DATA 6~20: FEED 1~15 (1=OPEN, 0=CLOSE)
            data_field = bytearray(20)
            
            # 먼저 모든 밸브의 현재 상태를 data_field에 설정
            # NOS 밸브 상태 설정 (DATA 1~5): 1=CLOSE, 0=OPEN
            for i in range(1, 6):
                data_field[i-1] = 0x01 if self.nos_valve_states.get(i, False) else 0x00
            
            # FEED 밸브 상태 설정 (DATA 6~20): 1=OPEN, 0=CLOSE
            for i in range(1, 16):
                # FEED는 반대: True(OPEN)이면 0x01, False(CLOSE)이면 0x00
                data_field[5+i-1] = 0x01 if self.feed_valve_states.get(i, False) else 0x00
            
            # 클릭한 밸브만 토글
            if valve_type == 'NOS':
                # NOS: 1=CLOSE, 0=OPEN
                current_state = self.nos_valve_states.get(valve_num, False)
                new_state_value = 0x00 if current_state else 0x01  # 토글
                data_field[valve_num-1] = new_state_value
                new_state_text = "CLOSE" if new_state_value == 0x01 else "OPEN"
            else:  # FEED
                # FEED: 1=OPEN, 0=CLOSE
                current_state = self.feed_valve_states.get(valve_num, False)
                new_state_value = 0x00 if current_state else 0x01  # 토글
                data_field[5+valve_num-1] = new_state_value
                new_state_text = "OPEN" if new_state_value == 0x01 else "CLOSE"
            
            # 로그에 데이터 필드 상세 정보 출력
            valve_name = f"{valve_type}{valve_num}"
            
            # HEX 형식으로 데이터 표시
            hex_data = " ".join([f"{b:02X}" for b in data_field])
            
            # NOS 밸브 상태 문자열 생성 (1=CLOSE, 0=OPEN)
            nos_states = []
            for i in range(1, 6):
                state_char = 'C' if data_field[i-1] == 0x01 else 'O'
                # 변경된 밸브는 강조
                if valve_type == 'NOS' and i == valve_num:
                    nos_states.append(f"[NOS{i}:{state_char}]")
                else:
                    nos_states.append(f"NOS{i}:{state_char}")
            nos_str = " ".join(nos_states)
            
            # FEED 밸브 상태 문자열 생성 (1=OPEN, 0=CLOSE)
            feed_states = []
            for i in range(1, 16):
                state_char = 'O' if data_field[5+i-1] == 0x01 else 'C'
                # 변경된 밸브는 강조
                if valve_type == 'FEED' and i == valve_num:
                    feed_states.append(f"[F{i}:{state_char}]")
                else:
                    feed_states.append(f"F{i}:{state_char}")
            feed_str = " ".join(feed_states)
            
            self.log_communication(f"[밸브 제어] {valve_name} → {new_state_text}", "blue")
            self.log_communication(f"  DATA FIELD (HEX): {hex_data}", "gray")
            self.log_communication(f"  NOS (1=C,0=O): {nos_str}", "gray")
            self.log_communication(f"  FEED (1=O,0=C): {feed_str}", "gray")
            
            # CMD 0xA0 패킷 전송
            success, message = self.comm.send_packet(0xA0, bytes(data_field))
            
            if success:
                self.log_communication(f"  전송 성공 (CMD 0xA0, 20바이트)", "green")
                
                # 로컬 상태 업데이트 (전송 성공 시)
                if valve_type == 'NOS':
                    # NOS: 1=CLOSE(True), 0=OPEN(False)
                    self.nos_valve_states[valve_num] = (new_state_value == 0x01)
                else:  # FEED
                    # FEED: 1=OPEN(True), 0=CLOSE(False)
                    self.feed_valve_states[valve_num] = (new_state_value == 0x01)
            else:
                self.log_communication(f"  전송 실패: {message}", "red")
                
        except Exception as e:
            self.log_communication(f"밸브 제어 오류: {str(e)}", "red")
    
    def send_cmd(self, cmd):
        """CMD 패킷 전송"""
        if not self.comm.is_connected:
            messagebox.showwarning("경고", "시리얼 포트가 연결되지 않았습니다.")
            return
        
        try:
            data_length = self.comm.protocol.CMD_LENGTH_MAP.get(cmd, 0)
            data_field = bytes(data_length) if data_length > 0 else None
            
            success, message = self.comm.send_packet(cmd, data_field)
            if success:
                self.log_communication(f"CMD 0x{cmd:02X} 전송 ({data_length}바이트)", "blue")
            else:
                self.log_communication(f"CMD 0x{cmd:02X} 전송 실패: {message}", "red")
        except Exception as e:
            self.log_communication(f"CMD 전송 오류: {str(e)}", "red")
    
    def monitor_data(self):
        """데이터 모니터링 스레드"""
        while self.monitoring_active:
            received_data = self.comm.get_received_data()
            for msg_type, data in received_data:
                if msg_type == 'PACKET':
                    self.process_received_packet(data)
                elif msg_type == 'SENT':
                    self.log_sent_data(data)
                elif msg_type == 'ERROR':
                    self.log_communication(f"오류: {data}", "red")
            
            status_updates = self.comm.get_status_updates()
            for status_type, message in status_updates:
                color = "purple" if status_type == "SYSTEM" else "red"
                self.log_communication(f"상태: {message}", color)
            
            time.sleep(0.1)
    
    # ============================================
    # 7. process_received_packet에 CMD 0xB2 수신 처리 추가
    # ============================================
    def process_received_packet(self, packet_info):
        """수신된 프로토콜 패킷 처리"""
        try:
            tx_id = packet_info['tx_id']
            rx_id = packet_info['rx_id']
            cmd = packet_info['cmd']
            data_field = packet_info['data_field']
            
            device_names = {0x01: "PC", 0x02: "MAIN", 0x03: "FRONT"}
            tx_name = device_names.get(tx_id, f"0x{tx_id:02X}")
            rx_name = device_names.get(rx_id, f"0x{rx_id:02X}")
            
            hex_data = " ".join([f"{b:02X}" for b in data_field]) if data_field else "없음"
            
            # CMD 0x0F (상태응답) 처리
            if cmd == 0x0F:
                # POLLING [메인 → PC] 상태응답 처리
                if tx_id == 0x02 and rx_id == 0x01:  # 메인 → PC
                    self.log_communication(f"수신: {tx_name}→{rx_name}, CMD 0x{cmd:02X} (상태응답), 데이터: {hex_data}", "green")
                    self.process_status_response(data_field)
                else:
                    # Heartbeat는 로그에서 제외
                    pass
            else:
                log_msg = f"수신: {tx_name}→{rx_name}, CMD 0x{cmd:02X}, 데이터: {hex_data}"
                self.log_communication(log_msg, "green")
            
            # CMD별 데이터 처리
            if cmd in [0xA0, 0xA1, 0xB0, 0xB1, 0xB2, 0xB3, 0xC0]:
                # CMD 0xB1 수신 처리 (냉각 제어 응답)
                if cmd == 0xB1 and len(data_field) >= 4:
                    target_rps = data_field[0]  # TARGET RPS
                    target_temp = self.comm.protocol.signed_byte_to_int(data_field[1])  # TARGET TEMP (signed byte)
                    cooling_operation = data_field[2]  # 냉각 동작 (0: STOP, 1: GOING)
                    on_temp = self.comm.protocol.signed_byte_to_int(data_field[3])  # 냉각 ON 온도 (signed byte)
                    
                    self.cooling_data['target_rps'] = target_rps
                    self.cooling_data['operation_state'] = 'GOING' if cooling_operation == 1 else 'STOP'
                    self.cooling_data['on_temp'] = on_temp
                    
                    self.log_communication(f"  냉각 데이터 수신: 목표 RPS={target_rps}, "
                                        f"목표 온도={target_temp}℃, "
                                        f"동작={'GOING' if cooling_operation == 1 else 'STOP'}, "
                                        f"ON 온도={on_temp}℃", "gray")
                
                # CMD 0xB2 수신 처리 (제빙 제어 응답)
                if cmd == 0xB2 and len(data_field) >= 7:
                    operation = data_field[0]  # 0=대기, 1=동작
                    icemaking_time = (data_field[1] << 8) | data_field[2]  # 2바이트 (ms)
                    water_capacity = (data_field[3] << 8) | data_field[4]  # 2바이트 (Hz)
                    swing_on = data_field[5]   # 1바이트 (ms)
                    swing_off = data_field[6]  # 1바이트 (ms)
                    
                    self.icemaking_data['operation'] = '동작' if operation == 1 else '대기'
                    self.icemaking_data['icemaking_time'] = icemaking_time
                    self.icemaking_data['water_capacity'] = water_capacity
                    self.icemaking_data['swing_on_time'] = swing_on
                    self.icemaking_data['swing_off_time'] = swing_off
                    
                    self.log_communication(f"  제빙 데이터 수신: 동작={self.icemaking_data['operation']}, "
                                        f"시간={icemaking_time}ms, 용량={water_capacity}Hz, "
                                        f"스윙 ON={swing_on}ms, OFF={swing_off}ms", "gray")
                
                try:
                    data_string = data_field.decode('utf-8', errors='ignore')
                    self.parse_and_update_data(data_string)
                except:
                    pass
        
        except Exception as e:
            self.log_communication(f"패킷 처리 오류: {str(e)}", "red")
    
    def process_status_response(self, data_field):
        """CMD 0x0F (상태응답) 처리 - POLLING [메인 → PC]"""
        try:
            if not data_field or len(data_field) == 0:
                return
            
            # DATA1: 탱크커버 탈착상태 (0: OPEN)
            if len(data_field) >= 1:
                tank_cover = data_field[0]
                tank_cover_status = "OPEN" if tank_cover == 0 else "CLOSE"
                self.log_communication(f"  탱크커버 상태: {tank_cover_status} (0x{tank_cover:02X})", "gray")
            
            # DATA2~DATA5: NOS 밸브 상태 (1~5)
            if len(data_field) >= 5:
                for i in range(1, 6):
                    valve_state = data_field[i]  # 0=OPEN, 1=CLOSE
                    self.nos_valve_states[i] = (valve_state == 1)
                    state_str = "CLOSE" if valve_state == 1 else "OPEN"
                    self.log_communication(f"  NOS 밸브 {i}: {state_str} (0x{valve_state:02X})", "gray")
                
                # 밸브 시스템에 상태 업데이트
                if hasattr(self, 'valve_system'):
                    self.valve_system.update_data(
                        nos_states={i: self.nos_valve_states[i] for i in range(1, 6)},
                        feed_states=None
                    )
            
            # DATA6~DATA20: FEED 밸브 상태 (1~15)
            if len(data_field) >= 20:
                for i in range(1, 16):
                    valve_state = data_field[5 + i - 1]  # DATA6부터 시작
                    self.feed_valve_states[i] = (valve_state == 1)
                    state_str = "OPEN" if valve_state == 1 else "CLOSE"
                    self.log_communication(f"  FEED 밸브 {i}: {state_str} (0x{valve_state:02X})", "gray")
                
                # 밸브 시스템에 상태 업데이트
                if hasattr(self, 'valve_system'):
                    self.valve_system.update_data(
                        nos_states=None,
                        feed_states={i: self.feed_valve_states[i] for i in range(1, 16)}
                    )
            elif len(data_field) > 5:
                # FEED 밸브가 일부만 있는 경우
                feed_count = len(data_field) - 5
                for i in range(1, feed_count + 1):
                    valve_state = data_field[5 + i - 1]
                    self.feed_valve_states[i] = (valve_state == 1)
                    state_str = "OPEN" if valve_state == 1 else "CLOSE"
                    self.log_communication(f"  FEED 밸브 {i}: {state_str} (0x{valve_state:02X})", "gray")
                
                # 밸브 시스템에 상태 업데이트
                if hasattr(self, 'valve_system'):
                    self.valve_system.update_data(
                        nos_states=None,
                        feed_states={i: self.feed_valve_states[i] for i in range(1, feed_count + 1)}
                    )
            
            self.log_communication(f"  상태응답 처리 완료 (데이터 길이: {len(data_field)}바이트)", "gray")
            
        except Exception as e:
            self.log_communication(f"상태응답 처리 오류: {str(e)}", "red")
    
    def log_sent_data(self, data):
        """송신 데이터 로그"""
        try:
            if len(data) < 8:
                return
            
            # STX와 ETX 검증
            stx = data[0]
            etx = data[-1]
            if stx != 0x02:
                self.log_communication(f"경고: 전송 패킷의 STX가 올바르지 않습니다 (예상: 0x02, 실제: 0x{stx:02X})", "orange")
            if etx != 0x03:
                self.log_communication(f"경고: 전송 패킷의 ETX가 올바르지 않습니다 (예상: 0x03, 실제: 0x{etx:02X})", "orange")
            
            cmd = data[3]
            
            # Heartbeat는 생략
            if cmd != 0x0F:
                tx_id = data[1]
                rx_id = data[2]
                
                device_names = {0x01: "PC", 0x02: "MAIN", 0x03: "FRONT"}
                tx_name = device_names.get(tx_id, f"0x{tx_id:02X}")
                rx_name = device_names.get(rx_id, f"0x{rx_id:02X}")
                
                # 전체 패킷 HEX 출력 (STX와 ETX 포함)
                hex_packet = " ".join([f"{b:02X}" for b in data])
                log_msg = f"송신: {tx_name}→{rx_name}, CMD 0x{cmd:02X} [STX: 0x{stx:02X}, ETX: 0x{etx:02X}]"
                self.log_communication(log_msg, "blue")
                self.log_communication(f"  전체 패킷 (HEX): {hex_packet}", "gray")
        
        except Exception:
            pass
    
    def parse_and_update_data(self, data_string):
        """수신 데이터 파싱 및 업데이트"""
        try:
            # 밸브 상태 파싱
            valve_updates = self.data_parser.parse_valve_status(data_string)
            
            for valve_num, is_closed in valve_updates.get('nos_valves', {}).items():
                self.nos_valve_states[valve_num] = is_closed
            
            for valve_num, is_open in valve_updates.get('feed_valves', {}).items():
                self.feed_valve_states[valve_num] = is_open
            
            # 시스템 상태 파싱
            system_updates = self.data_parser.parse_system_status(data_string)
            for system_type, updates in system_updates.items():
                if system_type == 'hvac' and updates:
                    self.hvac_data.update(updates)
                elif system_type == 'cooling' and updates:
                    self.cooling_data.update(updates)
                elif system_type == 'icemaking' and updates:
                    self.icemaking_data.update(updates)
                elif system_type == 'drain_tank' and updates:
                    self.drain_tank_data.update(updates)
                elif system_type == 'drain_pump' and updates:
                    self.drain_pump_data.update(updates)
            
            # 센서 데이터 파싱
            sensor_data = self.data_parser.parse_sensor_data(data_string)
            if sensor_data:
                for key in self.sensor_data.keys():
                    if key in sensor_data:
                        self.sensor_data[key] = sensor_data[key]
                
                # 그래프 데이터 업데이트
                self.update_all_graph_data()
        
        except Exception as e:
            self.log_communication(f"데이터 파싱 오류: {str(e)}", "red")
    
    def log_communication(self, message, color="black"):
        """통신 로그 기록"""
        def _log():
            timestamp = datetime.now().strftime("%H:%M:%S")
            self.comm_text.insert(tk.END, f"[{timestamp}] {message}\n")
            
            line_start = self.comm_text.index("end-2l")
            line_end = self.comm_text.index("end-1l")
            tag_name = f"color_{color}"
            self.comm_text.tag_add(tag_name, line_start, line_end)
            self.comm_text.tag_config(tag_name, foreground=color)
            
            self.comm_text.see(tk.END)
            
            line_count = int(self.comm_text.index(tk.END).split('.')[0])
            if line_count > 100:
                self.comm_text.delete(1.0, "2.0")
        
        self.root.after(0, _log)
    
    def toggle_graph1_item(self, item_key):
        """그래프1 항목 토글"""
        if item_key in self.graph1_active_items:
            self.graph1_active_items.remove(item_key)
            self.update_item_visual(item_key, False, graph_num=1)
        else:
            self.graph1_active_items.add(item_key)
            self.update_item_visual(item_key, True, graph_num=1)
    
    def toggle_graph2_item(self, item_key):
        """그래프2 항목 토글"""
        if item_key in self.graph2_active_items:
            self.graph2_active_items.remove(item_key)
            self.update_item_visual(item_key, False, graph_num=2)
        else:
            self.graph2_active_items.add(item_key)
            self.update_item_visual(item_key, True, graph_num=2)
    
    def update_item_visual(self, item_key, selected, graph_num):
        """선택된 항목의 시각적 표시 업데이트"""
        try:
            if item_key.startswith('nos_valve_'):
                valve_num = int(item_key.split('_')[2])
                label = self.nos_valve_labels[valve_num]
            elif item_key.startswith('feed_valve_'):
                valve_num = int(item_key.split('_')[2])
                label = self.feed_valve_labels[valve_num]
            elif item_key in self.sensor_labels:
                label = self.sensor_labels[item_key]
            else:
                return
            
            if selected:
                label.config(relief="solid", bd=3)
            else:
                if item_key.startswith('valve_'):
                    label.config(relief="raised", bd=1)
                else:
                    label.config(relief="sunken", bd=1)
        except (KeyError, ValueError):
            pass
    
    def update_all_graph_data(self):
        """모든 그래프 데이터 업데이트"""
        current_time = datetime.now()
        self.all_graph_data['time'].append(current_time)
        
        # 센서 데이터
        for sensor_key in ['outdoor_temp1', 'outdoor_temp2', 'purified_temp', 
                          'hot_inlet_temp', 'hot_internal_temp', 'hot_outlet_temp']:
            value = self.sensor_data.get(sensor_key, 0)
            self.all_graph_data[sensor_key].append(float(value))
        
        # cold_temp는 cold_temp_sensor로 저장
        self.all_graph_data['cold_temp_sensor'].append(float(self.sensor_data.get('cold_temp', 0)))
        
        # 밸브 데이터
        for i in range(1, 6):
            valve_state = 1 if self.nos_valve_states.get(i, False) else 0
            self.all_graph_data[f'nos_valve_{i}'].append(valve_state)
        
        for i in range(1, 16):
            valve_state = 1 if self.feed_valve_states.get(i, False) else 0
            self.all_graph_data[f'feed_valve_{i}'].append(valve_state)
        
        # 냉각 시스템
        cooling_op = 1 if self.cooling_data.get('operation_state') == 'GOING' else 0
        self.all_graph_data['cooling_operation'].append(cooling_op)
        self.all_graph_data['cooling_on_temp'].append(float(self.cooling_data.get('on_temp', 0)))
        self.all_graph_data['cooling_off_temp'].append(float(self.cooling_data.get('off_temp', 0)))
        
        # 제빙 시스템
        self.all_graph_data['icemaking_time'].append(float(self.icemaking_data.get('icemaking_time', 0)))
        self.all_graph_data['icemaking_capacity'].append(float(self.icemaking_data.get('water_capacity', 0)))
        
        # 드레인
        tank_level = 1 if self.drain_tank_data.get('high_level') == '감지' else 0
        pump_state = 1 if self.drain_pump_data.get('operation_state') == 'ON' else 0
        self.all_graph_data['drain_tank_level'].append(tank_level)
        self.all_graph_data['drain_pump_state'].append(pump_state)
    
    def update_gui(self):
        """GUI 업데이트"""
        # NOS 밸브 상태 업데이트 (양쪽 탭 모두)
        for valve_num, is_closed in self.nos_valve_states.items():
            # 냉동검토용 탭
            if valve_num in self.nos_valve_labels_freezing:
                label = self.nos_valve_labels_freezing[valve_num]
                if is_closed:
                    label.config(text="CLOSE", bg="red")
                else:
                    label.config(text="OPEN", bg="blue")
            
            # 제어검토용 탭
            if valve_num in self.nos_valve_labels_control:
                label = self.nos_valve_labels_control[valve_num]
                if is_closed:
                    label.config(text="CLOSE", bg="red")
                else:
                    label.config(text="OPEN", bg="blue")
        
        # FEED 밸브 상태 업데이트 (양쪽 탭 모두)
        for valve_num, is_open in self.feed_valve_states.items():
            # 냉동검토용 탭
            if valve_num in self.feed_valve_labels_freezing:
                label = self.feed_valve_labels_freezing[valve_num]
                if is_open:
                    label.config(text="OPEN", bg="blue")
                else:
                    label.config(text="CLOSE", bg="red")
            
            # 제어검토용 탭
            if valve_num in self.feed_valve_labels_control:
                label = self.feed_valve_labels_control[valve_num]
                if is_open:
                    label.config(text="OPEN", bg="blue")
                else:
                    label.config(text="CLOSE", bg="red")
        
        # 센서 데이터 업데이트
        for sensor_key, value in self.sensor_data.items():
            if sensor_key in self.sensor_labels:
                label = self.sensor_labels[sensor_key]
                label.config(text=f"{value:.1f}")
        
        # # 공조시스템 상태 업데이트
        # for hvac_key, value in self.hvac_data.items():
        #     if hvac_key in self.hvac_labels:
        #         label = self.hvac_labels[hvac_key]
        #         if hvac_key in ['refrigerant_valve_state', 'refrigerant_valve_target']:
        #             colors = {'핫가스': 'red', '제빙': 'blue', '냉각': 'green'}
        #             color = colors.get(value, 'gray')
        #             label.config(text=value, bg=color)
        #         elif hvac_key == 'compressor_state':
        #             if value == '동작중':
        #                 label.config(text="동작중", bg="green")
        #             else:
        #                 label.config(text="미동작", bg="gray")
        #         elif hvac_key in ['dc_fan1', 'dc_fan2']:
        #             if value == 'ON':
        #                 label.config(text="ON", bg="green")
        #             else:
        #                 label.config(text="OFF", bg="gray")
        #         else:
        #             label.config(text=str(value))
        
        # 냉각 시스템 상태 업데이트
        for cooling_key, value in self.cooling_data.items():
            if cooling_key in self.cooling_labels:
                widget = self.cooling_labels[cooling_key]
                if cooling_key == 'operation_state':
                    # 운전 상태는 항상 업데이트
                    if value == 'GOING':
                        widget.config(text="GOING", bg="green")
                    else:
                        widget.config(text="STOP", bg="red")
                elif cooling_key == 'target_rps':
                    # 목표 RPS는 입력 모드가 아닐 때만 업데이트 (Entry)
                    if not self.cooling_edit_mode:
                        current_value = widget.get()
                        new_value = str(value)
                        if current_value != new_value:
                            widget.config(state='normal')
                            widget.delete(0, tk.END)
                            widget.insert(0, new_value)
                            widget.config(state='readonly')
                elif cooling_key in ['on_temp', 'off_temp', 'cooling_additional_time']:
                    # 입력 모드가 아닐 때만 Entry 위젯 업데이트
                    if not self.cooling_edit_mode:
                        current_value = widget.get()
                        new_value = str(value)
                        if current_value != new_value:
                            widget.config(state='normal')
                            widget.delete(0, tk.END)
                            widget.insert(0, new_value)
                            widget.config(state='readonly')

        # 공조시스템 상태 업데이트
        for hvac_key, value in self.hvac_data.items():
            if hvac_key in self.hvac_labels:
                label = self.hvac_labels[hvac_key]
                if hvac_key == 'refrigerant_valve_state':
                    # 상태는 항상 업데이트
                    colors = {'핫가스': 'red', '제빙': 'blue', '냉각': 'green'}
                    color = colors.get(value, 'gray')
                    label.config(text=value, bg=color)
                elif hvac_key == 'refrigerant_valve_target':
                    # 입력 모드가 아닐 때만 업데이트
                    if not self.hvac_edit_mode:
                        colors = {'핫가스': 'red', '제빙': 'blue', '냉각': 'green'}
                        color = colors.get(value, 'orange')
                        label.config(text=value, bg=color)
                elif hvac_key == 'compressor_state':
                    # 상태는 항상 업데이트 (하지만 입력 모드에서는 제어용 라벨 표시)
                    if not self.hvac_edit_mode:
                        if value == '동작중':
                            label.config(text="동작중", bg="green")
                        else:
                            label.config(text="미동작", bg="gray")

                elif hvac_key in ['dc_fan1', 'dc_fan2']:
                    # 입력 모드가 아닐 때만 업데이트
                    if not self.hvac_edit_mode:
                        if value == 'ON':
                            label.config(text="ON", bg="green")
                        else:
                            label.config(text="OFF", bg="gray")
                elif hvac_key == 'error_code':
                    # 에러코드는 항상 업데이트
                    label.config(text=str(value))
        
        # 제빙 시스템 상태 업데이트 (입력 모드가 아닐 때만)
        if not self.icemaking_edit_mode:
            for ice_key, value in self.icemaking_data.items():
                if ice_key in self.icemaking_labels:
                    widget = self.icemaking_labels[ice_key]
                    
                    if ice_key == 'operation':
                        # Label 위젯
                        if value == '동작':
                            widget.config(text="동작", bg="green")
                        else:
                            widget.config(text="대기", bg="blue")
                    
                    elif ice_key in ['target_rps', 'icemaking_time', 'water_capacity', 'swing_on_time', 'swing_off_time']:
                        # Entry 위젯 업데이트 (readonly 상태에서만)
                        current_value = widget.get()
                        new_value = str(value)
                        if current_value != new_value:
                            widget.config(state='normal')
                            widget.delete(0, tk.END)
                            widget.insert(0, new_value)
                            widget.config(state='readonly')
        
        # 드레인 탱크 상태 업데이트
        for tank_key, value in self.drain_tank_data.items():
            if tank_key in self.drain_tank_labels:
                label = self.drain_tank_labels[tank_key]
                if tank_key in ['low_level', 'high_level']:
                    if value == '감지':
                        label.config(text="감지", bg="orange")
                    else:
                        label.config(text="미감지", bg="gray")
                elif tank_key == 'water_level_state':
                    colors = {'만수위': 'red', '저수위': 'orange', '비어있음': 'blue'}
                    color = colors.get(value, 'gray')
                    label.config(text=value, bg=color)
        
        # 드레인 펌프 상태 업데이트
        for pump_key, value in self.drain_pump_data.items():
            if pump_key in self.drain_pump_labels:
                label = self.drain_pump_labels[pump_key]
                if value == 'ON':
                    label.config(text="ON", bg="green")
                else:
                    label.config(text="OFF", bg="gray")
        
        

        # 보냉시스템 데이터 업데이트
        refrigeration_data = self.refrigeration_system.get_data()
        self.refrigeration_system.update_data(refrigeration_data)
        
        # 그래프 업데이트
        self.update_graphs()
        
        # 다음 업데이트 예약
        self.root.after(200, self.update_gui)
    
    def update_graphs(self):
        """선택된 항목들만 그래프에 표시"""
        if len(self.all_graph_data['time']) < 2:
            return
        
        try:
            times = list(self.all_graph_data['time'])
            
            # 냉동검토용 탭의 그래프 1 업데이트
            if hasattr(self, 'temp_ax_freezing'):
                self.temp_ax_freezing.clear()
                self.temp_ax_freezing.set_title("Selected Items (Graph 1 - Freezing)", fontsize=8, fontfamily='DejaVu Sans')
                self.temp_ax_freezing.set_ylabel("Value", fontsize=7, fontfamily='DejaVu Sans')
                self.temp_ax_freezing.grid(True, alpha=0.3)
                
                colors = ['red', 'blue', 'green', 'orange', 'purple', 'brown', 'pink', 'gray', 'olive', 'cyan']
                color_idx = 0
                
                for item_key in self.graph1_active_items:
                    if item_key in self.all_graph_data and len(self.all_graph_data[item_key]) > 0:
                        values = list(self.all_graph_data[item_key])
                        color = colors[color_idx % len(colors)]
                        
                        if item_key.startswith('nos_valve_'):
                            label = f"NOS{item_key.split('_')[2]}"
                        elif item_key.startswith('feed_valve_'):
                            label = f"FEED{item_key.split('_')[2]}"
                        elif item_key == 'cooling_operation':
                            label = "Cooling Op"
                        elif item_key == 'cooling_on_temp':
                            label = "Cool ON Temp"
                        elif item_key == 'cooling_off_temp':
                            label = "Cool OFF Temp"
                        elif item_key == 'icemaking_time':
                            label = "Ice Time"
                        elif item_key == 'icemaking_capacity':
                            label = "Ice Capacity"
                        elif item_key == 'drain_tank_level':
                            label = "Tank Level"
                        elif item_key == 'drain_pump_state':
                            label = "Pump State"
                        else:
                            label = item_key
                        
                        self.temp_ax_freezing.plot(times, values, color=color, label=label, linewidth=1.5)
                        color_idx += 1
                
                if self.graph1_active_items:
                    self.temp_ax_freezing.legend(fontsize=6)
                
                self.fig1_freezing.tight_layout()
                try:
                    self.canvas1_freezing.draw_idle()
                except Exception:
                    pass

            # 냉각 시스템 상태 업데이트 (입력 모드가 아닐 때만)
            if not self.cooling_edit_mode:  # 입력 모드가 아닐 때만 업데이트
                for cooling_key, value in self.cooling_data.items():
                    if cooling_key in self.cooling_labels:
                        widget = self.cooling_labels[cooling_key]
                        if cooling_key == 'operation_state':
                            # 운전 상태는 Label
                            if value == 'GOING':
                                widget.config(text="GOING", bg="green")
                            else:
                                widget.config(text="STOP", bg="red")
                        elif cooling_key in ['on_temp', 'off_temp', 'cooling_additional_time']:
                            # Entry 위젯 업데이트 (readonly 상태에서만)
                            current_value = widget.get()
                            new_value = str(value)
                            if current_value != new_value:
                                widget.config(state='normal')
                                widget.delete(0, tk.END)
                                widget.insert(0, new_value)
                                widget.config(state='readonly')
            
            # 제어검토용 탭의 그래프 1 업데이트
            if hasattr(self, 'temp_ax_control'):
                self.temp_ax_control.clear()
                self.temp_ax_control.set_title("Selected Items (Graph 1 - Control)", fontsize=8, fontfamily='DejaVu Sans')
                self.temp_ax_control.set_ylabel("Value", fontsize=7, fontfamily='DejaVu Sans')
                self.temp_ax_control.grid(True, alpha=0.3)
                
                colors = ['red', 'blue', 'green', 'orange', 'purple', 'brown', 'pink', 'gray', 'olive', 'cyan']
                color_idx = 0
                
                for item_key in self.graph1_active_items:
                    if item_key in self.all_graph_data and len(self.all_graph_data[item_key]) > 0:
                        values = list(self.all_graph_data[item_key])
                        color = colors[color_idx % len(colors)]
                        
                        if item_key.startswith('nos_valve_'):
                            label = f"NOS{item_key.split('_')[2]}"
                        elif item_key.startswith('feed_valve_'):
                            label = f"FEED{item_key.split('_')[2]}"
                        else:
                            label = item_key
                        
                        self.temp_ax_control.plot(times, values, color=color, label=label, linewidth=1.5)
                        color_idx += 1
                
                if self.graph1_active_items:
                    self.temp_ax_control.legend(fontsize=6)
                
                self.fig1_control.tight_layout()
                try:
                    self.canvas1_control.draw_idle()
                except Exception:
                    pass
            
            # 냉동검토용 탭의 그래프 2 업데이트
            if hasattr(self, 'pressure_ax_freezing'):
                self.pressure_ax_freezing.clear()
                self.pressure_ax_freezing.set_title("Selected Sensors (Graph 2 - Freezing)", fontsize=8, fontfamily='DejaVu Sans')
                self.pressure_ax_freezing.set_ylabel("Temperature (°C)", fontsize=7, fontfamily='DejaVu Sans')
                self.pressure_ax_freezing.set_xlabel("Time", fontsize=7, fontfamily='DejaVu Sans')
                self.pressure_ax_freezing.grid(True, alpha=0.3)
                
                sensor_colors = ['red', 'blue', 'green', 'orange', 'purple', 'brown', 'pink']
                color_idx = 0
                
                for sensor_key in self.graph2_active_items:
                    data_key = 'cold_temp_sensor' if sensor_key == 'cold_temp' else sensor_key
                    
                    if data_key in self.all_graph_data and len(self.all_graph_data[data_key]) > 0:
                        values = list(self.all_graph_data[data_key])
                        color = sensor_colors[color_idx % len(sensor_colors)]
                        
                        sensor_labels = {
                            'outdoor_temp1': 'Outdoor Temp 1',
                            'outdoor_temp2': 'Outdoor Temp 2',
                            'purified_temp': 'Purified Temp',
                            'cold_temp': 'Cold Temp',
                            'hot_inlet_temp': 'Hot Inlet',
                            'hot_internal_temp': 'Hot Internal',
                            'hot_outlet_temp': 'Hot Outlet'
                        }
                        label = sensor_labels.get(sensor_key, sensor_key)
                        
                        self.pressure_ax_freezing.plot(times, values, color=color, label=label, linewidth=1.5)
                        color_idx += 1
                
                if self.graph2_active_items:
                    self.pressure_ax_freezing.legend(fontsize=6)
                
                self.fig2_freezing.tight_layout()
                try:
                    self.canvas2_freezing.draw_idle()
                except Exception:
                    pass
            
            # 제어검토용 탭의 그래프 2 업데이트
            if hasattr(self, 'pressure_ax_control'):
                self.pressure_ax_control.clear()
                self.pressure_ax_control.set_title("Selected Sensors (Graph 2 - Control)", fontsize=8, fontfamily='DejaVu Sans')
                self.pressure_ax_control.set_ylabel("Temperature (°C)", fontsize=7, fontfamily='DejaVu Sans')
                self.pressure_ax_control.set_xlabel("Time", fontsize=7, fontfamily='DejaVu Sans')
                self.pressure_ax_control.grid(True, alpha=0.3)
                
                sensor_colors = ['red', 'blue', 'green', 'orange', 'purple', 'brown', 'pink']
                color_idx = 0
                
                for sensor_key in self.graph2_active_items:
                    data_key = 'cold_temp_sensor' if sensor_key == 'cold_temp' else sensor_key
                    
                    if data_key in self.all_graph_data and len(self.all_graph_data[data_key]) > 0:
                        values = list(self.all_graph_data[data_key])
                        color = sensor_colors[color_idx % len(sensor_colors)]
                        
                        sensor_labels = {
                            'outdoor_temp1': 'Outdoor Temp 1',
                            'outdoor_temp2': 'Outdoor Temp 2',
                            'purified_temp': 'Purified Temp',
                            'cold_temp': 'Cold Temp',
                            'hot_inlet_temp': 'Hot Inlet',
                            'hot_internal_temp': 'Hot Internal',
                            'hot_outlet_temp': 'Hot Outlet'
                        }
                        label = sensor_labels.get(sensor_key, sensor_key)
                        
                        self.pressure_ax_control.plot(times, values, color=color, label=label, linewidth=1.5)
                        color_idx += 1
                
                if self.graph2_active_items:
                    self.pressure_ax_control.legend(fontsize=6)
                
                self.fig2_control.tight_layout()
                try:
                    self.canvas2_control.draw_idle()
                except Exception:
                    pass
            
        except Exception as e:
            print(f"그래프 업데이트 오류: {e}")
    
    def on_tab_changed(self, event):
        """탭 변경 시 호출"""
        current_tab = self.notebook.index(self.notebook.select())
        tab_names = ["냉동검토용", "제어검토용"]
        if current_tab < len(tab_names):
            self.log_communication(f"탭 전환: {tab_names[current_tab]} 탭으로 이동", "purple")
    
    def on_closing(self):
        """프로그램 종료 처리"""
        self.monitoring_active = False
        if self.comm.is_connected:
            self.comm.disconnect()
        self.root.destroy()


def main():
    root = tk.Tk()
    app = MainGUI(root)
    
    root.protocol("WM_DELETE_WINDOW", app.on_closing)
    
    root.mainloop()


if __name__ == "__main__":
    main()