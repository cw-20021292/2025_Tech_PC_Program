"""
메인 GUI 모듈 - 프로토콜 적용 (전체 기능 유지)
"""
import tkinter as tk
from tkinter import ttk, messagebox, filedialog
import threading
import time
import os
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

from communication import SerialCommunication, DataParser, StatusResponseHandler
from systems import (
    RefrigerationSystem, CoolingSystem, HVACSystem, IcemakingSystem,
    DrainTankSystem, DrainPumpSystem, ValveSystem
)
import constants
from excel_sheet_selector import ExcelSheetSelector


class MainGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("코웨이 정수기 시스템 검토 프로그램 - 프로토콜 적용")
        self.root.geometry("1000x880")  # Log 삭제 버튼까지 보이는 높이로 조정
        self.root.resizable(True, False)  # 가로만 조정 가능, 세로 크기 고정
        
        # 통신 모듈 초기화
        self.comm = SerialCommunication()
        self.data_parser = DataParser()
        self.status_handler = StatusResponseHandler(self.comm.protocol)
        
        # 시스템 클래스 인스턴스화
        self.cooling_system = CoolingSystem(self.root, self.comm, self.log_communication)
        self.hvac_system = HVACSystem(self.root, self.comm, self.log_communication)
        self.icemaking_system = IcemakingSystem(self.root, self.comm, self.log_communication, 
                                                apply_table_callback=self.apply_icemaking_table)
        self.refrigeration_system = RefrigerationSystem(self.root, self.comm, self.log_communication)
        self.drain_tank_system = DrainTankSystem(self.root, self.comm, self.log_communication)
        self.drain_pump_system = DrainPumpSystem(self.root, self.comm, self.log_communication)
        self.valve_system = ValveSystem(self.root, self.comm, self.log_communication)
        
        # 센서 데이터 (시스템 클래스에 없는 경우를 위한 임시 저장소)
        self.sensor_data = {
            'outdoor_temp1': 0, 'outdoor_temp2': 0,
            'purified_temp': 0, 'cold_temp': 0,
            'hot_inlet_temp': 0, 'hot_internal_temp': 0,
            'hot_outlet_temp': 0
        }
        
        # Excel Sheet 선택 모듈 초기화
        self.excel_sheet_selector = ExcelSheetSelector(self.root)
        
        # 제빙테이블 데이터 저장 (Excel에서 읽은 데이터를 내부에 저장)
        self.freezing_table_data = None  # {'outdoor_temps': [], 'water_temps': [], 'table_data': [[]]}
        self.freezing_table_loaded = False  # 제빙테이블 로드 여부
        
        # 통신 디버그 모드 (True: 상세 로그 표시, False: 간단한 로그만 표시)
        self.debug_comm = True  # 통신 문제 디버깅용
        
        # Heartbeat 재개 타이머 (ice_step == 22일 때 12초 후 재개)
        self.heartbeat_resume_timer = None
        
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
        
        # 상단 영역 (그래프)
        top_frame = ttk.Frame(main_frame)
        top_frame.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S), pady=(0, 2))
        
        # 중단 영역 (시스템 제어)
        middle_frame = ttk.Frame(main_frame)
        middle_frame.grid(row=1, column=0, sticky=(tk.W, tk.E, tk.N, tk.S), pady=(0, 2))
        
        # 하단 영역 (밸브/센서)
        bottom_frame = ttk.Frame(main_frame)
        bottom_frame.grid(row=2, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))
        
        # 상단 영역 레이아웃 (그래프 2개 가로 배치)
        self.create_graph_areas(top_frame)
        
        # 중단 영역 레이아웃 (시스템 제어)
        # 시스템 클래스의 create_widgets 메서드 사용
        self.cooling_system.create_widgets(middle_frame)
        self.hvac_system.create_widgets(middle_frame)
        self.icemaking_system.create_widgets(middle_frame)
        self.refrigeration_system.create_widgets(middle_frame)
        
        # 하단 영역 레이아웃 (밸브/센서)
        # 시스템 클래스의 create_widgets 메서드 사용
        self.valve_system.create_widgets(bottom_frame, tab_type='freezing', row=0, column=0)
        self.drain_tank_system.create_widgets(bottom_frame, row=0, column=1)
        self.create_sensor_area(bottom_frame)  # 센서는 별도 처리 필요 (column=2)
        
        # 시스템 클래스의 레이블을 gui_main의 레이블 딕셔너리에 매핑
        self.cooling_labels = self.cooling_system.labels
        self.hvac_labels = self.hvac_system.labels
        self.icemaking_labels = self.icemaking_system.labels
        self.drain_tank_labels = self.drain_tank_system.labels
        self.drain_pump_labels = self.drain_pump_system.labels
        
        # 프레임 확장 설정
        # 상단 프레임 (그래프 2개 가로 배치)
        top_frame.columnconfigure(0, weight=1)
        top_frame.columnconfigure(1, weight=1)
        top_frame.rowconfigure(0, weight=1)
        
        # 중단 프레임 (시스템 제어 4개)
        for i in range(4):
            middle_frame.columnconfigure(i, weight=1)
        middle_frame.rowconfigure(0, weight=1)
        
        # 하단 프레임 (밸브/센서 3개)
        bottom_frame.columnconfigure(0, weight=1)
        bottom_frame.columnconfigure(1, weight=1)
        bottom_frame.columnconfigure(2, weight=1)
        bottom_frame.rowconfigure(0, weight=1)
        
        main_frame.columnconfigure(0, weight=1)
        main_frame.rowconfigure(0, weight=2)  # 그래프 영역
        main_frame.rowconfigure(1, weight=3)  # 시스템 제어 영역
        main_frame.rowconfigure(2, weight=4)  # 밸브/센서 영역
    
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
        
        # 상단 영역 레이아웃 (그래프 2개 가로 배치)
        self.create_graph_areas(top_frame)
        
        # 중단 영역 레이아웃 (밸브 시스템 클래스 사용)
        self.valve_system.create_widgets(middle_frame, tab_type='control')
        
        # 하단 영역 레이아웃
        self.create_control_sections(bottom_frame)
        
        # 프레임 확장 설정
        # 상단 프레임 (그래프 2개 가로 배치)
        top_frame.columnconfigure(0, weight=1)
        top_frame.columnconfigure(1, weight=1)
        top_frame.rowconfigure(0, weight=1)
        
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
        state_frame.columnconfigure(0, weight=1)
        ttk.Label(state_frame, text="운전 상태:", font=("Arial", 8), width=8).pack(side=tk.LEFT)
        self.cooling_labels['operation_state'] = tk.Label(state_frame, text="STOP", 
                                                        fg="white", bg="red", font=("Arial", 7, "bold"),
                                                        width=8, relief="raised")
        self.cooling_labels['operation_state'].pack(side=tk.RIGHT)
        
        # 초기기동 여부
        initial_startup_frame = ttk.Frame(cooling_frame)
        initial_startup_frame.grid(row=1, column=0, sticky=(tk.W, tk.E), pady=1)
        initial_startup_frame.columnconfigure(0, weight=1)
        ttk.Label(initial_startup_frame, text="초기기동:", font=("Arial", 8), width=8).pack(side=tk.LEFT)
        self.cooling_labels['initial_startup'] = tk.Label(initial_startup_frame, text="일반기동", 
                                                          fg="white", bg="blue", font=("Arial", 7, "bold"),
                                                          width=8, relief="raised")
        self.cooling_labels['initial_startup'].pack(side=tk.RIGHT)
        
        # 목표 RPS (입력 가능)
        target_rps_frame = ttk.Frame(cooling_frame)
        target_rps_frame.grid(row=2, column=0, sticky=(tk.W, tk.E), pady=1)
        target_rps_frame.columnconfigure(0, weight=1)
        ttk.Label(target_rps_frame, text="목표 RPS:", font=("Arial", 8), width=8).pack(side=tk.LEFT)
        vcmd_rps = (self.root.register(self.validate_rps), '%P')
        self.cooling_labels['target_rps'] = tk.Entry(target_rps_frame, font=("Arial", 8), 
                                             width=6, validate='key', validatecommand=vcmd_rps,
                                             state='readonly')
        self.cooling_labels['target_rps'].insert(0, "0")
        self.cooling_labels['target_rps'].pack(side=tk.RIGHT)
        
        # ON 온도 (입력 가능)
        on_temp_frame = ttk.Frame(cooling_frame)
        on_temp_frame.grid(row=3, column=0, sticky=(tk.W, tk.E), pady=1)
        on_temp_frame.columnconfigure(0, weight=1)
        ttk.Label(on_temp_frame, text="ON 온도:", font=("Arial", 8), width=8).pack(side=tk.LEFT)
        
        vcmd_temp = (self.root.register(self.validate_number), '%P')
        temp_unit_frame = ttk.Frame(on_temp_frame)
        temp_unit_frame.pack(side=tk.RIGHT)
        self.cooling_labels['on_temp'] = tk.Entry(temp_unit_frame, font=("Arial", 8), 
                                                width=6, validate='key', validatecommand=vcmd_temp,
                                                state='readonly')  # 기본 읽기 전용
        self.cooling_labels['on_temp'].insert(0, "0")
        self.cooling_labels['on_temp'].pack(side=tk.LEFT)
        ttk.Label(temp_unit_frame, text="℃", font=("Arial", 8)).pack(side=tk.LEFT, padx=(2, 0))
        
        # OFF 온도 (입력 가능)
        off_temp_frame = ttk.Frame(cooling_frame)
        off_temp_frame.grid(row=4, column=0, sticky=(tk.W, tk.E), pady=1)
        off_temp_frame.columnconfigure(0, weight=1)
        ttk.Label(off_temp_frame, text="OFF 온도:", font=("Arial", 8), width=8).pack(side=tk.LEFT)
        temp_unit_frame2 = ttk.Frame(off_temp_frame)
        temp_unit_frame2.pack(side=tk.RIGHT)
        self.cooling_labels['off_temp'] = tk.Entry(temp_unit_frame2, font=("Arial", 8), 
                                                width=6, validate='key', validatecommand=vcmd_temp,
                                                state='readonly')  # 기본 읽기 전용
        self.cooling_labels['off_temp'].insert(0, "0")
        self.cooling_labels['off_temp'].pack(side=tk.LEFT)
        ttk.Label(temp_unit_frame2, text="℃", font=("Arial", 8)).pack(side=tk.LEFT, padx=(2, 0))
        
        # 냉각 추가시간 (입력 가능)
        add_time_frame = ttk.Frame(cooling_frame)
        add_time_frame.grid(row=5, column=0, sticky=(tk.W, tk.E), pady=1)
        add_time_frame.columnconfigure(0, weight=1)
        ttk.Label(add_time_frame, text="추가시간:", font=("Arial", 8), width=8).pack(side=tk.LEFT)
        time_unit_frame = ttk.Frame(add_time_frame)
        time_unit_frame.pack(side=tk.RIGHT)
        self.cooling_labels['cooling_additional_time'] = tk.Entry(time_unit_frame, font=("Arial", 8), 
                                                                width=6, validate='key', validatecommand=vcmd_temp,
                                                                state='readonly')  # 기본 읽기 전용
        self.cooling_labels['cooling_additional_time'].insert(0, "0")
        self.cooling_labels['cooling_additional_time'].pack(side=tk.LEFT)
        ttk.Label(time_unit_frame, text="초", font=("Arial", 8)).pack(side=tk.LEFT, padx=(2, 0))
        
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
    
    def _get_icemaking_operation_from_step(self, ice_step):
        """ice_step 값에 따라 제빙 동작 상태 텍스트 반환"""
        if ice_step == 255:
            return '초기화'
        elif ice_step == 0:
            return '대기'
        elif 5 <= ice_step <= 7:
            return '예열'
        elif ice_step == 10:
            return '트레이 상승'
        elif 11 <= ice_step <= 14:
            return '입수 전 준비'
        elif ice_step == 20:
            return '트레이 입수'
        elif ice_step == 22 or ice_step == 30:
            return '시간 적용'
        elif ice_step == 31:
            return '제빙중'
        elif ice_step == 40:
            return '트레이 하강'
        elif 41 <= ice_step <= 44:
            return '탈빙중'
        elif ice_step == 50:
            return '얼음양 체크'
        elif ice_step == 51:
            return '제빙 완료'
        else:
            # 알 수 없는 값은 ice_step 값을 그대로 표시
            return f'STEP {ice_step}'
    
    def _get_icemaking_operation_color(self, ice_step):
        """ice_step 값에 따라 제빙 동작 상태 배경색 반환"""
        if ice_step == 255:
            return 'purple'  # 초기화
        elif ice_step == 0:
            return 'blue'  # 대기
        elif 5 <= ice_step <= 7:
            return 'red'  # 예열
        elif ice_step == 10:
            return 'black'  # 트레이 상승
        elif 11 <= ice_step <= 14:
            return 'lightblue'  # 입수 전 준비
        elif ice_step == 20:
            return 'lightblue'  # 트레이 입수
        elif ice_step == 22 or ice_step == 30:
            return 'gray'  # 시간 확정
        elif ice_step == 31:
            return 'green'  # 제빙중
        elif ice_step == 40:
            return 'black'  # 트레이 하강
        elif 41 <= ice_step <= 44:
            return 'pink'  # 탈빙중
        elif ice_step == 50:
            return 'lightgray'  # 얼음양 체크
        elif ice_step == 51:
            return 'darkgreen'  # 제빙 완료
        else:
            return 'gray'  # 알 수 없는 값
    
    def _resume_heartbeat_after_delay(self):
        """12초 지연 후 heartbeat 재개 (ice_step == 22일 때 호출)"""
        if self.comm.heartbeat_paused:
            self.comm.resume_heartbeat()
            if self.debug_comm:
                self.log_communication(
                    f"  [제빙 STEP 22] 12초 경과 후 Heartbeat 자동 재개",
                    "green"
                )
        self.heartbeat_resume_timer = None
    
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
                self.log_communication(f"제빙 제어 오류: {str(e)}", "red")
    
    def apply_icemaking_table(self):
        """제빙테이블 적용 - Excel 파일 선택 및 Sheet 선택"""
        if not self.comm.is_connected:
            messagebox.showwarning("경고", "시리얼 포트가 연결되지 않았습니다.")
            return
        
        def on_sheet_selected(sheet_name):
            """Sheet 선택 시 호출되는 콜백 함수"""
            if sheet_name:
                self.log_communication(
                    f"제빙테이블 선택: Excel 파일={self.excel_sheet_selector.selected_file_path}, "
                    f"Sheet={sheet_name}",
                    "blue"
                )
                
                # 제빙테이블 데이터 읽기
                table_data = self.excel_sheet_selector.read_icemaking_table_data(sheet_name)
                
                if table_data is None:
                    self.log_communication("제빙테이블 데이터 읽기 실패", "red")
                    self.freezing_table_loaded = False
                    return
                
                # 데이터를 내부에 저장 (바로 전송하지 않음)
                self.freezing_table_data = table_data
                self.freezing_table_loaded = True
                
                file_name = os.path.basename(self.excel_sheet_selector.selected_file_path)
                self.log_communication(
                    f"제빙테이블 데이터 로드 완료: {file_name} - {sheet_name}",
                    "green"
                )
                self.log_communication(
                    f"  입수온도 범위: {table_data['water_temps'][0]}~{table_data['water_temps'][-1]}℃",
                    "gray"
                )
                self.log_communication(
                    f"  외기온도 범위: {table_data['outdoor_temps'][0]}~{table_data['outdoor_temps'][-1]}℃",
                    "gray"
                )
                self.log_communication(
                    f"제빙테이블이 메모리에 저장되었습니다. CMD 0x0F 응답을 대기 중...",
                    "purple"
                )
                
                # 제빙테이블 뷰어 표시
                self.show_icemaking_table_viewer(table_data, file_name, sheet_name)
        
        # Excel 파일 선택 및 Sheet 선택 다이얼로그 표시
        selected_sheet = self.excel_sheet_selector.show_sheet_selection_dialog(
            callback=on_sheet_selected
        )
        
        if selected_sheet:
            self.log_communication(
                f"제빙테이블 적용 완료: Sheet={selected_sheet}",
                "green"
            )
    
    def show_icemaking_table_viewer(self, table_data, file_name, sheet_name):
        """제빙테이블 데이터를 시각적으로 표시하는 팝업 창
        
        Args:
            table_data: 제빙테이블 데이터 (water_temps, outdoor_temps, table_data)
            file_name: Excel 파일명
            sheet_name: Sheet 이름
        """
        try:
            water_temps = table_data['water_temps']
            outdoor_temps = table_data['outdoor_temps']
            table_values = table_data['table_data']
            
            # 팝업 창 생성
            viewer = tk.Toplevel(self.root)
            viewer.title(f"제빙테이블 뷰어 - {file_name} ({sheet_name})")
            viewer.geometry("1200x700")
            viewer.transient(self.root)
            
            # 중앙 정렬
            viewer.update_idletasks()
            x = (viewer.winfo_screenwidth() // 2) - (viewer.winfo_width() // 2)
            y = (viewer.winfo_screenheight() // 2) - (viewer.winfo_height() // 2)
            viewer.geometry(f"+{x}+{y}")
            
            # 메인 프레임
            main_frame = ttk.Frame(viewer, padding="10")
            main_frame.pack(fill=tk.BOTH, expand=True)
            
            # 제목 라벨
            title_label = ttk.Label(
                main_frame,
                text=f"제빙테이블: {file_name} - {sheet_name}",
                font=("Arial", 12, "bold")
            )
            title_label.pack(pady=(0, 10))
            
            # 정보 라벨
            info_label = ttk.Label(
                main_frame,
                text=f"입수온도: {water_temps[0]}~{water_temps[-1]}℃ (46개) | "
                     f"외기온도: {outdoor_temps[0]}~{outdoor_temps[-1]}℃ (46개)",
                font=("Arial", 9)
            )
            info_label.pack(pady=(0, 10))
            
            # 스크롤 가능한 프레임
            scroll_frame = ttk.Frame(main_frame)
            scroll_frame.pack(fill=tk.BOTH, expand=True)
            
            # Canvas와 Scrollbar 생성
            canvas = tk.Canvas(scroll_frame, highlightthickness=0)
            v_scrollbar = ttk.Scrollbar(scroll_frame, orient="vertical", command=canvas.yview)
            h_scrollbar = ttk.Scrollbar(scroll_frame, orient="horizontal", command=canvas.xview)
            
            # 테이블 프레임
            table_frame = ttk.Frame(canvas)
            
            # Canvas 설정
            canvas.configure(yscrollcommand=v_scrollbar.set, xscrollcommand=h_scrollbar.set)
            
            # 스크롤바 배치
            v_scrollbar.pack(side=tk.RIGHT, fill=tk.Y)
            h_scrollbar.pack(side=tk.BOTTOM, fill=tk.X)
            canvas.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
            
            # 테이블 프레임을 Canvas에 배치
            canvas_window = canvas.create_window((0, 0), window=table_frame, anchor="nw")
            
            # 테이블 생성
            # 좌상단 빈 셀
            corner_label = tk.Label(
                table_frame,
                text="입수↓ / 외기→",
                font=("Arial", 8, "bold"),
                bg="lightgray",
                relief="solid",
                borderwidth=1,
                width=12,
                height=2
            )
            corner_label.grid(row=0, column=0, sticky="nsew")
            
            # 외기온도 헤더 (가로, 첫 번째 행)
            for col_idx, outdoor_temp in enumerate(outdoor_temps):
                temp_label = tk.Label(
                    table_frame,
                    text=f"{outdoor_temp:.1f}℃",
                    font=("Arial", 7, "bold"),
                    bg="lightblue",
                    relief="solid",
                    borderwidth=1,
                    width=8,
                    height=2
                )
                temp_label.grid(row=0, column=col_idx+1, sticky="nsew")
            
            # 입수온도 헤더 (세로, 첫 번째 열) 및 테이블 데이터
            for row_idx, water_temp in enumerate(water_temps):
                # 입수온도 헤더
                water_label = tk.Label(
                    table_frame,
                    text=f"{water_temp:.1f}℃",
                    font=("Arial", 7, "bold"),
                    bg="lightyellow",
                    relief="solid",
                    borderwidth=1,
                    width=12,
                    height=2
                )
                water_label.grid(row=row_idx+1, column=0, sticky="nsew")
                
                # 테이블 데이터
                for col_idx, value in enumerate(table_values[row_idx]):
                    value_label = tk.Label(
                        table_frame,
                        text=f"{int(value)}",
                        font=("Arial", 7),
                        bg="white",
                        relief="solid",
                        borderwidth=1,
                        width=8,
                        height=2
                    )
                    value_label.grid(row=row_idx+1, column=col_idx+1, sticky="nsew")
            
            # Canvas 크기 조정 이벤트
            def on_frame_configure(event):
                canvas.configure(scrollregion=canvas.bbox("all"))
            
            table_frame.bind("<Configure>", on_frame_configure)
            
            # 마우스 휠 스크롤 이벤트
            def on_mousewheel(event):
                canvas.yview_scroll(int(-1 * (event.delta / 120)), "units")
            
            def on_shift_mousewheel(event):
                canvas.xview_scroll(int(-1 * (event.delta / 120)), "units")
            
            def bind_to_mousewheel(event):
                canvas.bind_all("<MouseWheel>", on_mousewheel)
                canvas.bind_all("<Shift-MouseWheel>", on_shift_mousewheel)
            
            def unbind_from_mousewheel(event):
                canvas.unbind_all("<MouseWheel>")
                canvas.unbind_all("<Shift-MouseWheel>")
            
            canvas.bind("<Enter>", bind_to_mousewheel)
            canvas.bind("<Leave>", unbind_from_mousewheel)
            
            # 하단 버튼 프레임
            button_frame = ttk.Frame(main_frame)
            button_frame.pack(pady=(10, 0))
            
            # 확인 버튼
            ok_button = ttk.Button(
                button_frame,
                text="확인",
                command=viewer.destroy,
                width=15
            )
            ok_button.pack()
            
            self.log_communication("제빙테이블 뷰어 표시", "blue")
            
        except Exception as e:
            messagebox.showerror("오류", f"제빙테이블 표시 중 오류 발생:\n{str(e)}")
            self.log_communication(f"제빙테이블 뷰어 오류: {str(e)}", "red")
    
    def send_freezing_table_row(self, water_temp_idx):
        """제빙테이블 특정 행을 CMD 0xB3으로 전송
        
        Args:
            water_temp_idx: 입수온도 인덱스 (0~45)
        
        Returns:
            bool: 전송 성공 여부
        """
        if not self.freezing_table_loaded or self.freezing_table_data is None:
            self.log_communication("제빙테이블이 로드되지 않았습니다.", "red")
            return False
        
        try:
            table_data = self.freezing_table_data
            water_temps = table_data['water_temps']
            outdoor_temps = table_data['outdoor_temps']
            table_rows = table_data['table_data']
            
            if water_temp_idx < 0 or water_temp_idx >= len(table_rows):
                self.log_communication(f"잘못된 입수온도 인덱스: {water_temp_idx}", "red")
                return False
            
            # DATA FIELD 생성 (93바이트)
            # DATA1: 행 인덱스 (1바이트)
            # DATA2~93: 테이블 데이터 46개 x 2바이트 = 92바이트
            data_field = bytearray(93)
            
            # DATA1: 행 인덱스 (0~45)
            data_field[0] = water_temp_idx
            
            # DATA2~DATA93: 테이블 데이터 46개 (B~AU열), 각 2바이트
            for col_idx in range(46):
                table_value = int(table_rows[water_temp_idx][col_idx])
                # 범위 체크 (0~65535)
                if table_value < 0:
                    table_value = 0
                elif table_value > 65535:
                    table_value = 65535
                
                # 2바이트로 변환 (상위 바이트, 하위 바이트)
                high_byte = (table_value >> 8) & 0xFF  # 상위 바이트
                low_byte = table_value & 0xFF           # 하위 바이트
                
                # DATA2부터 시작, 각 값마다 2바이트 할당
                data_field[1 + (col_idx * 2)] = high_byte      # 상위 바이트
                data_field[1 + (col_idx * 2) + 1] = low_byte   # 하위 바이트
            
            # 패킷 구조 검증 로그
            if self.debug_comm:
                self.log_communication(
                    f"[CMD 0xB3 패킷 검증]",
                    "blue"
                )
                self.log_communication(
                    f"  DATA FIELD 길이: {len(data_field)} 바이트 (예상: 93바이트)",
                    "gray"
                )
                self.log_communication(
                    f"  DATA1 (행 인덱스): {data_field[0]} (입수온도: {water_temps[water_temp_idx]}℃)",
                    "gray"
                )
                
                # 처음 3개 테이블 값 예시 로그
                for i in range(min(3, 46)):
                    idx = 1 + (i * 2)
                    value = (data_field[idx] << 8) | data_field[idx + 1]
                    self.log_communication(
                        f"  DATA{idx+1}~{idx+2} (외기온도{outdoor_temps[i]}℃): 0x{data_field[idx]:02X} 0x{data_field[idx+1]:02X} = {value}ms",
                        "gray"
                    )
                
                if len(outdoor_temps) > 3:
                    self.log_communication(f"  ... (중간 {len(outdoor_temps)-6}개 생략)", "gray")
                
                # 마지막 3개 테이블 값 예시 로그
                for i in range(max(3, 46-3), 46):
                    idx = 1 + (i * 2)
                    value = (data_field[idx] << 8) | data_field[idx + 1]
                    self.log_communication(
                        f"  DATA{idx+1}~{idx+2} (외기온도{outdoor_temps[i]}℃): 0x{data_field[idx]:02X} 0x{data_field[idx+1]:02X} = {value}ms",
                        "gray"
                    )
            
            # CMD 0xB3 패킷 생성 (내부적으로 STX, TX_ID, CMD, DATA_LEN, CRC, ETX 추가)
            # 최종 패킷 구조: STX(1) + TX_ID(1) + CMD(1) + DATA_LEN(1) + DATA_FIELD(93) + CRC_HIGH(1) + CRC_LOW(1) + ETX(1) = 100바이트
            # Heartbeat는 제빙 STEP 22 감지 시 이미 일시 중지된 상태
            success, message = self.comm.send_packet(0xB3, bytes(data_field))
            
            # 패킷 전송 후 대기 (전송 완료 대기)
            time.sleep(0.2)  # 200ms
            
            water_temp = int(water_temps[water_temp_idx])
            if success:
                if self.debug_comm:
                    self.log_communication(
                        f"[자동] 제빙테이블 전송 성공: 입수온도 {water_temp}℃ (행 {water_temp_idx})",
                        "green"
                    )
                    self.log_communication(
                        f"  전송 패킷 총 길이: 100바이트 (STX(1) + TX_ID(1) + CMD(1) + LEN(1) + DATA(93) + CRC(2) + ETX(1))",
                        "gray"
                    )
                return True
            else:
                if self.debug_comm:
                    self.log_communication(
                        f"[자동] 제빙테이블 전송 실패: 입수온도 {water_temp}℃ (행 {water_temp_idx}), {message}",
                        "red"
                    )
                return False
                
        except Exception as e:
            self.log_communication(f"제빙테이블 전송 오류: {str(e)}", "red")
            return False

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
            self.hvac_temp_data['refrigerant_valve_state_1'] = self.hvac_data['refrigerant_valve_state_1']
            self.hvac_temp_data['refrigerant_valve_state_2'] = self.hvac_data['refrigerant_valve_state_2']
            self.hvac_temp_data['compressor_state'] = self.hvac_data['compressor_state']
            self.hvac_temp_data['dc_fan1'] = self.hvac_data['dc_fan1']
            self.hvac_temp_data['dc_fan2'] = self.hvac_data['dc_fan2']
            
            # UI를 임시 저장소 값으로 초기화
            colors = {'냉각': 'green', '제빙': 'blue', '핫가스': 'red', '보냉': 'orange'}
            self.hvac_labels['refrigerant_valve_state_1'].config(
                text=self.hvac_temp_data['refrigerant_valve_state_1'],
                bg=colors.get(self.hvac_temp_data['refrigerant_valve_state_1'], 'gray')
            )
            
            self.hvac_labels['refrigerant_valve_state_2'].config(
                text=self.hvac_temp_data['refrigerant_valve_state_2'],
                bg=colors.get(self.hvac_temp_data['refrigerant_valve_state_2'], 'gray')
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
                self.hvac_data['refrigerant_valve_state_1'] = self.hvac_temp_data['refrigerant_valve_state_1']
                self.hvac_data['refrigerant_valve_state_2'] = self.hvac_temp_data['refrigerant_valve_state_2']
                self.hvac_data['compressor_state'] = self.hvac_temp_data['compressor_state']
                self.hvac_data['dc_fan1'] = self.hvac_temp_data['dc_fan1']
                self.hvac_data['dc_fan2'] = self.hvac_temp_data['dc_fan2']
                
                # DATA FIELD 구성 (4바이트) - RPS 제거
                data_field = bytearray(4)
                
                # DATA 0: 냉매전환밸브 1 목표 (냉각=0, 제빙=1, 핫가스=2, 보냉=3)
                valve_map = {'냉각': 0, '제빙': 1, '핫가스': 2, '보냉': 3}
                data_field[0] = valve_map.get(self.hvac_data['refrigerant_valve_state_1'], 2)  # 기본값: 핫가스(2)
                
                # DATA 1: 냉매전환밸브 2 목표 (냉각=0, 제빙=1, 핫가스=2, 보냉=3)
                data_field[1] = valve_map.get(self.hvac_data['refrigerant_valve_state_2'], 2)  # 기본값: 핫가스(2)
                
                # DATA 2: 압축기 상태 (동작=1, 미동작=0)
                data_field[2] = 1 if self.hvac_data['compressor_state'] == '동작중' else 0
                
                # DATA 3: DC FAN 1 (압축기 팬, ON=1, OFF=0)
                data_field[3] = 1 if self.hvac_data['dc_fan1'] == 'ON' else 0
                
                # DATA 4: DC FAN 2 (얼음탱크 팬, ON=1, OFF=0) - 주의: 4바이트이므로 인덱스 3까지만 사용
                # 실제로는 4바이트이므로 data_field[3]이 마지막
                
                # 로그 출력
                hex_data = " ".join([f"{b:02X}" for b in data_field])
                self.log_communication(f"[공조 제어] CMD 0xB0 전송 (입력 모드 최종 설정)", "blue")
                self.log_communication(f"  냉매전환밸브 1번 상태: {self.hvac_data['refrigerant_valve_state_1']} ({data_field[0]})", "gray")
                self.log_communication(f"  냉매전환밸브 2번 상태: {self.hvac_data['refrigerant_valve_state_2']} ({data_field[1]})", "gray")
                self.log_communication(f"  압축기 상태: {self.hvac_data['compressor_state']} ({data_field[2]})", "gray")
                self.log_communication(f"  압축기 팬: {self.hvac_data['dc_fan1']} ({data_field[3]})", "gray")
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
        ttk.Label(state_frame, text="1번 상태:", font=("Arial", 8)).pack(side=tk.LEFT)
        self.hvac_labels['refrigerant_valve_state_1'] = tk.Label(state_frame, text="핫가스", 
                                                            fg="white", bg="red", font=("Arial", 8, "bold"),
                                                            width=8, relief="raised")
        self.hvac_labels['refrigerant_valve_state_1'].pack(side=tk.RIGHT)
        
        # 목표 (버튼으로 변경 - 클릭 가능하게 설정)
        target_frame = ttk.Frame(valve_subframe)
        target_frame.grid(row=1, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(target_frame, text="2번 상태:", font=("Arial", 8)).pack(side=tk.LEFT)
        self.hvac_labels['refrigerant_valve_state_2'] = tk.Label(target_frame, text="핫가스", 
                                                            fg="white", bg="orange", font=("Arial", 8, "bold"),
                                                            width=8, relief="raised", cursor="hand2")
        self.hvac_labels['refrigerant_valve_state_2'].pack(side=tk.RIGHT)
        
        # 압축기 서브프레임
        comp_subframe = ttk.LabelFrame(hvac_frame, text="압축기", padding="3")
        comp_subframe.grid(row=1, column=0, columnspan=2, sticky=(tk.W, tk.E), pady=(0, 5))
        
        # 상태 (버튼으로 변경 - 클릭 가능하게 설정)
        comp_state_frame = ttk.Frame(comp_subframe)
        comp_state_frame.grid(row=0, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(comp_state_frame, text="가동 상태:", font=("Arial", 8)).pack(side=tk.LEFT)
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
        
        # 안정화 시간
        stabilization_frame = ttk.Frame(comp_subframe)
        stabilization_frame.grid(row=2, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(stabilization_frame, text="안정화 시간:", font=("Arial", 8)).pack(side=tk.LEFT)
        self.hvac_labels['stabilization_time'] = tk.Label(stabilization_frame, text="0", 
                                                       font=("Arial", 8), bg="white", relief="sunken")
        self.hvac_labels['stabilization_time'].pack(side=tk.RIGHT)
        
        # 에러코드
        error_frame = ttk.Frame(comp_subframe)
        error_frame.grid(row=3, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(error_frame, text="에러코드:", font=("Arial", 8)).pack(side=tk.LEFT)
        self.hvac_labels['error_code'] = tk.Label(error_frame, text="0", 
                                                font=("Arial", 8), bg="white", relief="sunken")
        self.hvac_labels['error_code'].pack(side=tk.RIGHT)
        
        # DC FAN 1 (압축기 팬, 버튼으로 변경 - 클릭 가능하게 설정)
        fan1_frame = ttk.Frame(comp_subframe)
        fan1_frame.grid(row=4, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(fan1_frame, text="압축기 팬:", font=("Arial", 8)).pack(side=tk.LEFT)
        self.hvac_labels['dc_fan1'] = tk.Label(fan1_frame, text="OFF", 
                                            fg="white", bg="gray", font=("Arial", 8, "bold"),
                                            width=5, relief="raised", cursor="hand2")
        self.hvac_labels['dc_fan1'].pack(side=tk.RIGHT)
        # 클릭 이벤트 바인딩 확인
        self.hvac_labels['dc_fan1'].bind("<Button-1>", self.toggle_dc_fan1)
        
        # DC FAN 2 (얼음탱크 팬, 버튼으로 변경 - 클릭 가능하게 설정)
        fan2_frame = ttk.Frame(comp_subframe)
        fan2_frame.grid(row=5, column=0, sticky=(tk.W, tk.E), pady=1)
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
        operation_frame.columnconfigure(0, weight=1)
        ttk.Label(operation_frame, text="제빙 동작:", font=("Arial", 9), width=9).pack(side=tk.LEFT)
        self.icemaking_labels['operation'] = tk.Label(operation_frame, text="대기", 
                                                    fg="white", bg="blue", font=("Arial", 8, "bold"),
                                                    width=10, relief="raised", cursor="hand2")
        self.icemaking_labels['operation'].pack(side=tk.RIGHT)
        self.icemaking_labels['operation'].bind("<Button-1>", self.toggle_icemaking_operation)
        
        # 목표 RPS (입력 가능)
        target_rps_frame = ttk.Frame(icemaking_frame)
        target_rps_frame.grid(row=1, column=0, sticky=(tk.W, tk.E), pady=1)
        target_rps_frame.columnconfigure(0, weight=1)
        ttk.Label(target_rps_frame, text="목표 RPS:", font=("Arial", 9), width=9).pack(side=tk.LEFT)
        vcmd_rps = (self.root.register(self.validate_rps), '%P')
        self.icemaking_labels['target_rps'] = tk.Entry(target_rps_frame, font=("Arial", 9), 
                                             width=8, validate='key', validatecommand=vcmd_rps,
                                             state='readonly')
        self.icemaking_labels['target_rps'].insert(0, "0")
        self.icemaking_labels['target_rps'].pack(side=tk.RIGHT)
        
        # 제빙시간 (ms 단위, 입력 가능)
        time_frame = ttk.Frame(icemaking_frame)
        time_frame.grid(row=2, column=0, sticky=(tk.W, tk.E), pady=1)
        time_frame.columnconfigure(0, weight=1)
        ttk.Label(time_frame, text="제빙시간:", font=("Arial", 9), width=9).pack(side=tk.LEFT)
        
        vcmd_num = (self.root.register(self.validate_number), '%P')
        time_unit_frame = ttk.Frame(time_frame)
        time_unit_frame.pack(side=tk.RIGHT)
        self.icemaking_labels['icemaking_time'] = tk.Entry(time_unit_frame, font=("Arial", 9), 
                                                width=8, validate='key', validatecommand=vcmd_num,
                                                state='readonly')
        self.icemaking_labels['icemaking_time'].insert(0, "0")
        self.icemaking_labels['icemaking_time'].pack(side=tk.LEFT)
        ttk.Label(time_unit_frame, text="ms", font=("Arial", 9)).pack(side=tk.LEFT, padx=(2, 0))
        
        # 입수 용량 (Hz 단위, 입력 가능)
        capacity_frame = ttk.Frame(icemaking_frame)
        capacity_frame.grid(row=3, column=0, sticky=(tk.W, tk.E), pady=1)
        capacity_frame.columnconfigure(0, weight=1)
        ttk.Label(capacity_frame, text="입수 용량:", font=("Arial", 9), width=9).pack(side=tk.LEFT)
        capacity_unit_frame = ttk.Frame(capacity_frame)
        capacity_unit_frame.pack(side=tk.RIGHT)
        self.icemaking_labels['water_capacity'] = tk.Entry(capacity_unit_frame, font=("Arial", 9), 
                                                    width=8, validate='key', validatecommand=vcmd_num,
                                                    state='readonly')
        self.icemaking_labels['water_capacity'].insert(0, "0")
        self.icemaking_labels['water_capacity'].pack(side=tk.LEFT)
        ttk.Label(capacity_unit_frame, text="Hz", font=("Arial", 9)).pack(side=tk.LEFT, padx=(2, 0))
        
        # 스윙바 ON 시간 (ms 단위, 입력 가능)
        swing_on_frame = ttk.Frame(icemaking_frame)
        swing_on_frame.grid(row=4, column=0, sticky=(tk.W, tk.E), pady=1)
        swing_on_frame.columnconfigure(0, weight=1)
        ttk.Label(swing_on_frame, text="스윙바 ON:", font=("Arial", 9), width=9).pack(side=tk.LEFT)
        swing_on_unit_frame = ttk.Frame(swing_on_frame)
        swing_on_unit_frame.pack(side=tk.RIGHT)
        self.icemaking_labels['swing_on_time'] = tk.Entry(swing_on_unit_frame, font=("Arial", 9), 
                                                    width=8, validate='key', validatecommand=vcmd_num,
                                                    state='readonly')
        self.icemaking_labels['swing_on_time'].insert(0, "0")
        self.icemaking_labels['swing_on_time'].pack(side=tk.LEFT)
        ttk.Label(swing_on_unit_frame, text="ms", font=("Arial", 9)).pack(side=tk.LEFT, padx=(2, 0))
        
        # 스윙바 OFF 시간 (ms 단위, 입력 가능)
        swing_off_frame = ttk.Frame(icemaking_frame)
        swing_off_frame.grid(row=5, column=0, sticky=(tk.W, tk.E), pady=1)
        swing_off_frame.columnconfigure(0, weight=1)
        ttk.Label(swing_off_frame, text="스윙바 OFF:", font=("Arial", 9), width=9).pack(side=tk.LEFT)
        swing_off_unit_frame = ttk.Frame(swing_off_frame)
        swing_off_unit_frame.pack(side=tk.RIGHT)
        self.icemaking_labels['swing_off_time'] = tk.Entry(swing_off_unit_frame, font=("Arial", 9), 
                                                    width=8, validate='key', validatecommand=vcmd_num,
                                                    state='readonly')
        self.icemaking_labels['swing_off_time'].insert(0, "0")
        self.icemaking_labels['swing_off_time'].pack(side=tk.LEFT)
        ttk.Label(swing_off_unit_frame, text="ms", font=("Arial", 9)).pack(side=tk.LEFT, padx=(2, 0))
        
        # 트레이 위치
        tray_position_frame = ttk.Frame(icemaking_frame)
        tray_position_frame.grid(row=6, column=0, sticky=(tk.W, tk.E), pady=1)
        tray_position_frame.columnconfigure(0, weight=1)
        ttk.Label(tray_position_frame, text="트레이 위치:", font=("Arial", 9), width=9).pack(side=tk.LEFT)
        self.icemaking_labels['tray_position'] = tk.Label(tray_position_frame, text="제빙", 
                                                          fg="white", bg="blue", font=("Arial", 8, "bold"),
                                                          width=8, relief="raised")
        self.icemaking_labels['tray_position'].pack(side=tk.RIGHT)
        
        # 얼음걸림 상태
        ice_jam_frame = ttk.Frame(icemaking_frame)
        ice_jam_frame.grid(row=7, column=0, sticky=(tk.W, tk.E), pady=1)
        ice_jam_frame.columnconfigure(0, weight=1)
        ttk.Label(ice_jam_frame, text="얼음걸림:", font=("Arial", 9), width=9).pack(side=tk.LEFT)
        self.icemaking_labels['ice_jam_state'] = tk.Label(ice_jam_frame, text="없음", 
                                                         fg="white", bg="blue", font=("Arial", 8, "bold"),
                                                         width=8, relief="raised")
        self.icemaking_labels['ice_jam_state'].pack(side=tk.RIGHT)
        
        # CMD 0xB2 전송 버튼
        send_btn_frame = ttk.Frame(icemaking_frame)
        send_btn_frame.grid(row=8, column=0, sticky=(tk.W, tk.E), pady=(5, 1))
        self.icemaking_send_btn = ttk.Button(send_btn_frame, text="제빙 설정 입력 모드",
                                        command=self.send_icemaking_control, state="disabled")
        self.icemaking_send_btn.pack(fill=tk.X)
        
        # 제빙테이블 적용 버튼
        table_btn_frame = ttk.Frame(icemaking_frame)
        table_btn_frame.grid(row=9, column=0, sticky=(tk.W, tk.E), pady=(5, 1))
        self.icemaking_table_btn = ttk.Button(table_btn_frame, text="제빙테이블 적용",
                                        command=self.apply_icemaking_table, state="disabled")
        self.icemaking_table_btn.pack(fill=tk.X)
        
        icemaking_frame.columnconfigure(0, weight=1)
    
    def create_graph_areas(self, parent):
        """그래프 영역 생성"""
        # 그래프 1
        graph1_frame = ttk.LabelFrame(parent, text="그래프 1", padding="3")
        graph1_frame.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S), padx=(0, 2))
        
        # 그래프 2
        graph2_frame = ttk.LabelFrame(parent, text="그래프 2", padding="3")
        graph2_frame.grid(row=0, column=1, sticky=(tk.W, tk.E, tk.N, tk.S))
        
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
        sensor_frame.grid(row=0, column=2, sticky=(tk.W, tk.E, tk.N, tk.S), padx=(2, 0))
        
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
        drain_tank_frame.grid(row=0, column=1, sticky=(tk.W, tk.E, tk.N, tk.S), padx=2)
        
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
        
        # 드레인펌프 운전 상태 추가
        pump_state_frame = ttk.Frame(drain_tank_frame)
        pump_state_frame.grid(row=3, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(pump_state_frame, text="펌프 상태:", font=("Arial", 9), width=8).pack(side=tk.LEFT)
        self.drain_pump_labels['operation_state'] = tk.Label(pump_state_frame, text="OFF", 
                                                           fg="white", bg="red", font=("Arial", 8, "bold"),
                                                           width=8, relief="raised")
        self.drain_pump_labels['operation_state'].pack(side=tk.LEFT, padx=(2, 0))
        
        drain_tank_frame.columnconfigure(0, weight=1)
    
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
        
        # 디버그 모드 체크박스
        debug_frame = ttk.Frame(right_frame)
        debug_frame.grid(row=4, column=0, sticky=(tk.W, tk.E), pady=(3, 0))
        self.debug_var = tk.BooleanVar(value=True)
        debug_check = ttk.Checkbutton(
            debug_frame, 
            text="통신 디버그", 
            variable=self.debug_var,
            command=self.toggle_debug_mode
        )
        debug_check.pack(side=tk.LEFT)
        
        # Log 추출 버튼
        self.log_export_btn = ttk.Button(right_frame, text="Log 추출",
                                        command=self.export_log,
                                        state="disabled")
        self.log_export_btn.grid(row=5, column=0, sticky=(tk.W, tk.E), pady=(3, 0))
        
        # Log 삭제 버튼
        self.log_clear_btn = ttk.Button(right_frame, text="Log 삭제",
                                       command=self.clear_log,
                                       state="disabled")
        self.log_clear_btn.grid(row=6, column=0, sticky=(tk.W, tk.E), pady=(3, 0))
        
        # CMD 전송 버튼들 (0xA0 제외)
        cmd_frame = ttk.LabelFrame(right_frame, text="CMD 전송", padding="2")
        cmd_frame.grid(row=7, column=0, sticky=(tk.W, tk.E), pady=(3, 0))
        
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
    
    def toggle_debug_mode(self):
        """통신 디버그 모드 토글"""
        self.debug_comm = self.debug_var.get()
        if self.debug_comm:
            self.log_communication("🔍 통신 디버그 모드 활성화 (RAW 데이터 표시)", "blue")
        else:
            self.log_communication("통신 디버그 모드 비활성화", "gray")
    
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
                
                # 각 시스템의 버튼 활성화 (set_connection_state 사용)
                self.cooling_system.set_connection_state(True)
                self.hvac_system.set_connection_state(True)
                self.icemaking_system.set_connection_state(True)
                self.refrigeration_system.set_connection_state(True)
                
                # Log 추출/삭제 버튼 활성화
                if hasattr(self, 'log_export_btn'):
                    self.log_export_btn.config(state="normal")
                if hasattr(self, 'log_clear_btn'):
                    self.log_clear_btn.config(state="normal")
                
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
                
                # 각 시스템의 버튼 비활성화 (set_connection_state 사용)
                self.cooling_system.set_connection_state(False)
                self.hvac_system.set_connection_state(False)
                self.icemaking_system.set_connection_state(False)
                self.refrigeration_system.set_connection_state(False)
                
                # Log 추출/삭제 버튼 비활성화
                if hasattr(self, 'log_export_btn'):
                    self.log_export_btn.config(state="disabled")
                if hasattr(self, 'log_clear_btn'):
                    self.log_clear_btn.config(state="disabled")
                
                self.log_communication("연결 해제됨", "orange")
    
    def send_valve_control(self, valve_num, valve_type):
        """밸브 제어 CMD 0xA0 전송 (개별 밸브만 제어) - valve_system 위임"""
        # valve_system의 send_valve_control 메서드 사용
        self.valve_system.send_valve_control(valve_num, valve_type)
    
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
        """데이터 모니터링 스레드 - 디버깅 로그 포함"""
        while self.monitoring_active:
            received_data = self.comm.get_received_data()
            for msg_type, data in received_data:
                if msg_type == 'PACKET':
                    # 패킷 파싱 전 RAW 데이터 로깅
                    if hasattr(self, 'debug_comm') and self.debug_comm:
                        if 'tx_id' in data and 'cmd' in data:
                            # 정상 패킷
                            pass  # process_received_packet에서 처리
                        else:
                            # 파싱 오류 정보
                            self.log_communication(
                                f"[디버그] 패킷 파싱 정보: {data}",
                                "orange"
                            )
                    self.process_received_packet(data)
                elif msg_type == 'SENT':
                    self.log_sent_data(data)
                elif msg_type == 'ERROR':
                    self.log_communication(f"❌ 통신 오류: {data}", "red")
                elif msg_type == 'RAW_DATA':
                    # RAW 데이터 수신 로그 (Heartbeat 제외)
                    if hasattr(self, 'debug_comm') and self.debug_comm:
                        # Heartbeat 패킷인지 확인 (0x02 0xXX 0x0F ... 형태)
                        raw_bytes = data.get('bytes', '')
                        # "02 01 0F" 또는 "02 02 0F" 패턴이 있으면 heartbeat으로 간주
                        is_heartbeat = ('02 01 0F' in raw_bytes or '02 02 0F' in raw_bytes)
                        
                        if not is_heartbeat:
                            pass
                            # self.log_communication(
                            #     f"[수신 RAW] {data['bytes']} ({data['length']}바이트)",
                            #     "purple"
                            # )
            
            status_updates = self.comm.get_status_updates()
            for status_type, message in status_updates:
                color = "purple" if status_type == "SYSTEM" else "red"
                self.log_communication(f"상태: {message}", color)
            
            time.sleep(0.1)
    
    # ============================================
    # 7. process_received_packet에 CMD 0xB2 수신 처리 추가
    # ============================================
    def process_received_packet(self, packet_info):
        """수신된 프로토콜 패킷 처리 - 성공/실패 로그 추가"""
        try:
            # 패킷 파싱 에러 체크
            if 'error' in packet_info:
                # 파싱 실패
                error_type = packet_info.get('error', 'UNKNOWN')
                error_detail = packet_info.get('detail', '상세 정보 없음')
                raw_data = packet_info.get('raw_data', '')
                
                # 에러 타입별 로그 색상 및 메시지
                error_messages = {
                    'INVALID_START': '❌ [1단계 실패] STX 확인 실패',
                    'UNDEFINED_CMD': '❌ [2단계 실패] CMD 확인 실패',
                    'ETX_POSITION_MISMATCH': '❌ [4단계 실패] ETX 위치 확인 실패',
                    'CRC_MISMATCH': '❌ [4단계 실패] CRC 검증 실패',
                    'PACKET_TOO_SHORT': '❌ 패킷 길이 부족',
                    'LENGTH_MISMATCH': '❌ 패킷 길이 불일치',
                    'INVALID_STX': '❌ STX 오류',
                    'INVALID_ETX': '❌ ETX 오류',
                    'PARSE_EXCEPTION': '❌ 파싱 예외'
                }
                
                if self.debug_comm:
                    error_msg = error_messages.get(error_type, f"❌ 패킷 수신 실패: {error_type}")
                    self.log_communication(error_msg, "red")
                    self.log_communication(f"   사유: {error_detail}", "orange")
                    
                    if raw_data:
                        self.log_communication(f"   RAW: {raw_data}", "gray")
                    
                    # 버퍼 초기화 안내
                    if error_type in ['INVALID_START', 'UNDEFINED_CMD', 'ETX_POSITION_MISMATCH', 'CRC_MISMATCH']:
                        self.log_communication(f"   ⚠️  수신 버퍼 초기화됨", "orange")
                
                return
            
            # 정상 패킷
            if 'tx_id' not in packet_info or 'cmd' not in packet_info:
                self.log_communication("❌ 패킷 수신 실패: 필수 필드 누락", "red")
                return
            
            tx_id = packet_info['tx_id']
            cmd = packet_info['cmd']
            data_field = packet_info['data_field']
            
            device_names = {0x01: "PC", 0x02: "MAIN", 0x03: "FRONT"}
            tx_name = device_names.get(tx_id, f"0x{tx_id:02X}")
            
            hex_data = " ".join([f"{b:02X}" for b in data_field]) if data_field else "없음"
            
            # CMD 0xF0 (공통 상태조회) 처리
            if cmd == 0xF0:
                # POLLING [메인 → PC] 공통 상태응답 처리
                if tx_id == 0x02:  # 메인 → PC
                    if self.debug_comm:
                        pass
                        # self.log_communication(f"✅ 패킷 수신 성공: {tx_name}, CMD 0x{cmd:02X} (공통 상태조회)", "green")
                        # self.log_communication(f"   데이터: {hex_data}", "gray")
                    self.process_common_status_response(data_field, tx_id)
                else:
                    pass
            # CMD 0xF1 (냉동상태조회) 처리
            elif cmd == 0xF1:
                # POLLING [메인 → PC] 냉동 상태응답 처리
                if tx_id == 0x02:  # 메인 → PC
                    if self.debug_comm:
                        pass
                        # self.log_communication(f"✅ 패킷 수신 성공: {tx_name}, CMD 0x{cmd:02X} (냉동 상태조회)", "green")
                        # self.log_communication(f"   데이터: {hex_data}", "gray")
                    self.process_freezing_status_response(data_field, tx_id)
                else:
                    pass
            # CMD 0x0F (구버전 호환용) 처리
            elif cmd == 0x0F:
                # POLLING [메인 → PC] 상태응답 처리
                if tx_id == 0x02:  # 메인 → PC
                    if self.debug_comm:
                        pass
                        # self.log_communication(f"✅ 패킷 수신 성공: {tx_name}, CMD 0x{cmd:02X} (상태응답)", "green")
                        # self.log_communication(f"   데이터: {hex_data}", "gray")
                    self.process_status_response(data_field, tx_id)
                else:
                    pass
            else:
                if self.debug_comm:
                    log_msg = f"✅ 패킷 수신 성공: {tx_name}, CMD 0x{cmd:02X}, 데이터: {hex_data}"
                    self.log_communication(log_msg, "green")
            
            # CMD별 데이터 처리
            if cmd in [0xA0, 0xA1, 0xB0, 0xB1, 0xB2, 0xB3, 0xC0]:
                # CMD 0xB1 수신 처리 (냉각 제어 응답)
                if cmd == 0xB1 and len(data_field) >= 4:
                    target_rps = data_field[0]  # TARGET RPS
                    target_temp = self.comm.protocol.signed_byte_to_int(data_field[1])  # TARGET TEMP (signed byte)
                    cooling_operation = data_field[2]  # 냉각 동작 (0: STOP, 1: GOING)
                    on_temp = self.comm.protocol.signed_byte_to_int(data_field[3])  # 냉각 ON 온도 (signed byte)
                    
                    cooling_data = {
                        'target_rps': target_rps,
                        'operation_state': 'GOING' if cooling_operation == 1 else 'STOP',
                        'on_temp': on_temp
                    }
                    self.cooling_system.update_data(cooling_data)
                    
                    if self.debug_comm:
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
                    
                    icemaking_data = {
                        'operation': '동작' if operation == 1 else '대기',
                        'icemaking_time': icemaking_time,
                        'water_capacity': water_capacity,
                        'swing_on_time': swing_on,
                        'swing_off_time': swing_off
                    }
                    self.icemaking_system.update_data(icemaking_data)
                    
                    if self.debug_comm:
                        self.log_communication(f"  제빙 데이터 수신: 동작={icemaking_data['operation']}, "
                                            f"시간={icemaking_time}ms, 용량={water_capacity}Hz, "
                                            f"스윙 ON={swing_on}ms, OFF={swing_off}ms", "gray")
                
                try:
                    data_string = data_field.decode('utf-8', errors='ignore')
                    self.parse_and_update_data(data_string)
                except:
                    pass
        
        except Exception as e:
            self.log_communication(f"패킷 처리 오류: {str(e)}", "red")
    
    def process_common_status_response(self, data_field, tx_id=None):
        """CMD 0xF0 (공통 상태조회) 처리 - 40바이트"""
        try:
            if not data_field or len(data_field) == 0:
                return
            
            if tx_id != 0x02:  # MAIN_ID
                return
            
            if len(data_field) < 40:
                if self.debug_comm:
                    self.log_communication(
                        f"⚠ CMD 0xF0 데이터 필드 길이 부족: {len(data_field)}바이트 (예상: 40바이트)",
                        "orange"
                    )
                return
            
            # communication.py의 StatusResponseHandler를 사용하여 데이터 파싱
            parsed_data = self.status_handler.parse_common_status(data_field, tx_id)
            
            # 센서 데이터 업데이트
            if parsed_data.get('sensor_data'):
                self.sensor_data.update(parsed_data['sensor_data'])
                # 센서 데이터가 업데이트되면 그래프 데이터도 업데이트
                self.update_all_graph_data()
            
            # 밸브 상태 업데이트
            if parsed_data.get('valve_states'):
                valve_states = parsed_data['valve_states']
                if valve_states.get('nos'):
                    self.valve_system.update_data(nos_states=valve_states['nos'], feed_states=None)
                if valve_states.get('feed'):
                    self.valve_system.update_data(nos_states=None, feed_states=valve_states['feed'])
            
        except Exception as e:
            self.log_communication(f"공통 상태조회 처리 오류: {str(e)}", "red")
    
    def process_freezing_status_response(self, data_field, tx_id=None):
        """CMD 0xF1 (냉동상태조회) 처리 - 76바이트"""
        try:
            if not data_field or len(data_field) == 0:
                return
            
            if tx_id != 0x02:  # MAIN_ID
                return
            
            if len(data_field) < 76:
                if self.debug_comm:
                    self.log_communication(
                        f"⚠ CMD 0xF1 데이터 필드 길이 부족: {len(data_field)}바이트 (예상: 76바이트)",
                        "orange"
                    )
                return
            
            # communication.py의 StatusResponseHandler를 사용하여 데이터 파싱
            parsed_data = self.status_handler.parse_freezing_status(data_field, tx_id)
            
            # 각 시스템 클래스에 데이터 전달
            if parsed_data.get('hvac_data'):
                self.hvac_system.update_data(parsed_data['hvac_data'])
            
            if parsed_data.get('cooling_data'):
                cooling_data = parsed_data['cooling_data'].copy()
                # operation_state를 'GOING'/'STOP'에서 '가동'/'대기'로 변환 (기존 코드 호환성)
                if cooling_data.get('operation_state') == '가동':
                    cooling_data['operation_state'] = 'GOING'
                elif cooling_data.get('operation_state') == '대기':
                    cooling_data['operation_state'] = 'STOP'
                self.cooling_system.update_data(cooling_data)
            
            if parsed_data.get('icemaking_data'):
                icemaking_data = parsed_data['icemaking_data'].copy()
                # ice_step에 따라 operation 상태 결정
                ice_step = icemaking_data.get('ice_step', 0)
                icemaking_data['operation'] = self._get_icemaking_operation_from_step(ice_step)
                
                # 제빙 STEP에 따른 Heartbeat 제어
                if ice_step == 22:
                    if not self.comm.heartbeat_paused:
                        self.comm.pause_heartbeat()
                        if self.debug_comm:
                            self.log_communication(
                                f"  [제빙 STEP 22] 상태조회 일시 중지 (12초 후 자동 재개 예정)",
                                "orange"
                            )
                        
                        if self.heartbeat_resume_timer is not None:
                            self.heartbeat_resume_timer.cancel()
                        
                        self.heartbeat_resume_timer = threading.Timer(12.0, self._resume_heartbeat_after_delay)
                        self.heartbeat_resume_timer.start()
                else:
                    if self.heartbeat_resume_timer is not None:
                        self.heartbeat_resume_timer.cancel()
                        self.heartbeat_resume_timer = None
                    
                    if self.comm.heartbeat_paused:
                        self.comm.resume_heartbeat()
                        if self.debug_comm:
                            self.log_communication(
                                f"  [제빙 STEP {ice_step}] 상태조회 재개",
                                "orange"
                            )
                
                # 제빙테이블 자동 전송 처리
                if ice_step == 22 and self.freezing_table_loaded and self.freezing_table_data is not None:
                    if self.debug_comm:
                        self.log_communication(
                            f"  제빙 STEP이 22입니다. 제빙테이블 자동 전송을 시작합니다...",
                            "purple"
                        )
                    
                    water_temps = self.freezing_table_data['water_temps']
                    hot_inlet_temp = self.sensor_data.get('hot_inlet_temp', 0)
                    
                    water_temp_idx = None
                    min_diff = float('inf')
                    for idx, temp in enumerate(water_temps):
                        diff = abs(temp - hot_inlet_temp)
                        if diff < min_diff:
                            min_diff = diff
                            water_temp_idx = idx
                    
                    if water_temp_idx is not None:
                        if self.debug_comm:
                            self.log_communication(
                                f"  온수입수온도 {hot_inlet_temp}℃에 해당하는 테이블 행 {water_temp_idx} (테이블 입수온도: {water_temps[water_temp_idx]}℃) 선택",
                                "cyan"
                            )
                        
                        success = self.send_freezing_table_row(water_temp_idx)
                        
                        if self.debug_comm:
                            if success:
                                self.log_communication(f"  제빙테이블 자동 전송 완료", "green")
                            else:
                                self.log_communication(f"  제빙테이블 자동 전송 실패", "red")
                    else:
                        if self.debug_comm:
                            self.log_communication(
                                f"  온수입수온도 {hot_inlet_temp}℃에 해당하는 테이블 행을 찾을 수 없습니다.",
                                "orange"
                            )
                elif ice_step == 22 and not self.freezing_table_loaded:
                    if self.debug_comm:
                        self.log_communication(
                            f"  제빙 STEP이 22이지만 제빙테이블이 로드되지 않았습니다.",
                            "orange"
                        )
                
                self.icemaking_system.update_data(icemaking_data)
            
            if parsed_data.get('refrigeration_data'):
                self.refrigeration_system.update_data(parsed_data['refrigeration_data'])
            
            if parsed_data.get('drain_tank_data'):
                self.drain_tank_system.update_data(parsed_data['drain_tank_data'])
            
            if parsed_data.get('drain_pump_data'):
                self.drain_pump_system.update_data(parsed_data['drain_pump_data'])
            
        except Exception as e:
            self.log_communication(f"냉동 상태조회 처리 오류: {str(e)}", "red")
    
    def process_status_response(self, data_field, tx_id=None):
        """CMD 0x0F (상태응답) 처리 - POLLING [메인 → PC] - 새로운 114바이트 구조 (구버전 호환용)"""
        try:
            if not data_field or len(data_field) == 0:
                return
            
            # TX_ID가 MAIN_ID(0x02)일 때만 정상 데이터로 처리
            if tx_id != 0x02:  # MAIN → PC
                device_names = {0x01: "PC", 0x02: "MAIN", 0x03: "FRONT"}
                tx_name = device_names.get(tx_id, f"0x{tx_id:02X}") if tx_id else "알 수 없음"
                self.log_communication(
                    f"⚠ 비정상 TX_ID: {tx_name} (정상: MAIN만 가능)",
                    "orange"
                )
                return
            
            # 최소 114바이트 필요
            if len(data_field) < 114:
                if self.debug_comm:
                    self.log_communication(
                        f"⚠ CMD 0x0F 데이터 필드 길이 부족: {len(data_field)}바이트 (예상: 114바이트)",
                        "orange"
                    )
                return
            
            # communication.py의 StatusResponseHandler를 사용하여 데이터 파싱
            parsed_data = self.status_handler.parse_status_response(data_field, tx_id)
            
            # 센서 데이터 업데이트
            if parsed_data.get('sensor_data'):
                self.sensor_data.update(parsed_data['sensor_data'])
                # 센서 데이터가 업데이트되면 그래프 데이터도 업데이트
                self.update_all_graph_data()
            
            # 각 시스템 클래스에 데이터 전달
            if parsed_data.get('hvac_data'):
                self.hvac_system.update_data(parsed_data['hvac_data'])
            
            if parsed_data.get('cooling_data'):
                cooling_data = parsed_data['cooling_data'].copy()
                # operation_state를 'GOING'/'STOP'에서 '가동'/'대기'로 변환 (기존 코드 호환성)
                if cooling_data.get('operation_state') == '가동':
                    cooling_data['operation_state'] = 'GOING'
                elif cooling_data.get('operation_state') == '대기':
                    cooling_data['operation_state'] = 'STOP'
                self.cooling_system.update_data(cooling_data)
            
            if parsed_data.get('icemaking_data'):
                icemaking_data = parsed_data['icemaking_data'].copy()
                # ice_step에 따라 operation 상태 결정
                ice_step = icemaking_data.get('ice_step', 0)
                icemaking_data['operation'] = self._get_icemaking_operation_from_step(ice_step)
                
                # 제빙 STEP에 따른 Heartbeat 제어
                if ice_step == 22:
                    if not self.comm.heartbeat_paused:
                        self.comm.pause_heartbeat()
                        if self.debug_comm:
                            self.log_communication(
                                f"  [제빙 STEP 22] Heartbeat 일시 중지 (12초 후 자동 재개 예정)",
                                "orange"
                            )
                        
                        if self.heartbeat_resume_timer is not None:
                            self.heartbeat_resume_timer.cancel()
                        
                        self.heartbeat_resume_timer = threading.Timer(12.0, self._resume_heartbeat_after_delay)
                        self.heartbeat_resume_timer.start()
                else:
                    if self.heartbeat_resume_timer is not None:
                        self.heartbeat_resume_timer.cancel()
                        self.heartbeat_resume_timer = None
                    
                    if self.comm.heartbeat_paused:
                        self.comm.resume_heartbeat()
                        if self.debug_comm:
                            self.log_communication(
                                f"  [제빙 STEP {ice_step}] Heartbeat 재개",
                                "orange"
                            )
                
                # 제빙테이블 자동 전송 처리
                if ice_step == 22 and self.freezing_table_loaded and self.freezing_table_data is not None:
                    if self.debug_comm:
                        self.log_communication(
                            f"  제빙 STEP이 22입니다. 제빙테이블 자동 전송을 시작합니다...",
                            "purple"
                        )
                    
                    water_temps = self.freezing_table_data['water_temps']
                    hot_inlet_temp = self.sensor_data.get('hot_inlet_temp', 0)
                    
                    water_temp_idx = None
                    min_diff = float('inf')
                    for idx, temp in enumerate(water_temps):
                        diff = abs(temp - hot_inlet_temp)
                        if diff < min_diff:
                            min_diff = diff
                            water_temp_idx = idx
                    
                    if water_temp_idx is not None:
                        if self.debug_comm:
                            self.log_communication(
                                f"  온수입수온도 {hot_inlet_temp}℃에 해당하는 테이블 행 {water_temp_idx} (테이블 입수온도: {water_temps[water_temp_idx]}℃) 선택",
                                "cyan"
                            )
                        
                        success = self.send_freezing_table_row(water_temp_idx)
                        
                        if self.debug_comm:
                            if success:
                                self.log_communication(f"  제빙테이블 자동 전송 완료", "green")
                            else:
                                self.log_communication(f"  제빙테이블 자동 전송 실패", "red")
                    else:
                        if self.debug_comm:
                            self.log_communication(
                                f"  온수입수온도 {hot_inlet_temp}℃에 해당하는 테이블 행을 찾을 수 없습니다.",
                                "orange"
                            )
                elif ice_step == 22 and not self.freezing_table_loaded:
                    if self.debug_comm:
                        self.log_communication(
                            f"  제빙 STEP이 22이지만 제빙테이블이 로드되지 않았습니다.",
                            "orange"
                        )
                
                self.icemaking_system.update_data(icemaking_data)
            
            if parsed_data.get('refrigeration_data'):
                self.refrigeration_system.update_data(parsed_data['refrigeration_data'])
            
            if parsed_data.get('drain_tank_data'):
                self.drain_tank_system.update_data(parsed_data['drain_tank_data'])
            
            if parsed_data.get('drain_pump_data'):
                self.drain_pump_system.update_data(parsed_data['drain_pump_data'])
            
            if parsed_data.get('valve_states'):
                valve_states = parsed_data['valve_states']
                if valve_states.get('nos'):
                    self.valve_system.update_data(nos_states=valve_states['nos'], feed_states=None)
                if valve_states.get('feed'):
                    self.valve_system.update_data(nos_states=None, feed_states=valve_states['feed'])
            
            # 기존 코드와의 호환성을 위해 하드코딩된 변수도 업데이트 (점진적 마이그레이션)
            # TODO: 나중에 완전히 제거
            if parsed_data.get('hvac_data'):
                # self.hvac_data는 더 이상 사용하지 않지만, 기존 코드 호환성을 위해 유지
                pass
            
            if parsed_data.get('cooling_data'):
                # self.cooling_data는 더 이상 사용하지 않지만, 기존 코드 호환성을 위해 유지
                pass
            
            if parsed_data.get('icemaking_data'):
                # self.icemaking_data는 더 이상 사용하지 않지만, 기존 코드 호환성을 위해 유지
                pass
            
            # 기존 하드코딩된 로직 제거 (위에서 StatusResponseHandler로 처리)
            
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
                if self.debug_comm:
                    self.log_communication(f"경고: 전송 패킷의 STX가 올바르지 않습니다 (예상: 0x02, 실제: 0x{stx:02X})", "orange")
            if etx != 0x03:
                if self.debug_comm:
                    self.log_communication(f"경고: 전송 패킷의 ETX가 올바르지 않습니다 (예상: 0x03, 실제: 0x{etx:02X})", "orange")
            
            cmd = data[2]  # RX ID 제거로 인덱스 변경
            data_len = data[3]  # DATA LENGTH
            
            # Heartbeat는 생략
            if cmd != 0x0F:
                if self.debug_comm:
                    tx_id = data[1]
                    
                    device_names = {0x01: "PC", 0x02: "MAIN", 0x03: "FRONT"}
                    tx_name = device_names.get(tx_id, f"0x{tx_id:02X}")
                    
                    # 전체 패킷 길이 검증
                    total_length = len(data)
                    expected_length = 7 + data_len  # STX(1) + TX_ID(1) + CMD(1) + LEN(1) + DATA(N) + CRC(2) + ETX(1)
                    
                    log_msg = f"송신: {tx_name}, CMD 0x{cmd:02X}, 패킷 길이: {total_length}바이트"
                    self.log_communication(log_msg, "blue")
                    
                    # CMD 0xB3의 경우 상세 검증
                    if cmd == 0xB3:
                        self.log_communication(
                            f"  [CMD 0xB3 검증] DATA LENGTH: {data_len}바이트 (예상: 93바이트)",
                            "cyan"
                        )
                        self.log_communication(
                            f"  [CMD 0xB3 검증] 전체 패킷: {total_length}바이트 (예상: 100바이트)",
                            "cyan"
                        )
                        
                        if total_length != 100:
                            self.log_communication(
                                f"  ⚠️  경고: CMD 0xB3 패킷 길이 불일치! (예상: 100, 실제: {total_length})",
                                "red"
                            )
                        
                        if data_len != 93:
                            self.log_communication(
                                f"  ⚠️  경고: CMD 0xB3 DATA FIELD 길이 불일치! (예상: 93, 실제: {data_len})",
                                "red"
                            )
                        
                        # DATA FIELD 구조 확인
                        if total_length >= 8:
                            row_idx = data[4]  # DATA1: 행 인덱스
                            self.log_communication(
                                f"  DATA1 (행 인덱스): {row_idx}",
                                "gray"
                            )
                            
                            # 전체 DATA FIELD 출력 (STX(1) + TX_ID(1) + CMD(1) + LEN(1) 다음부터 CRC(2) + ETX(1) 전까지)
                            if total_length > 7:
                                data_field_bytes = data[4:total_length-3]  # DATA FIELD만 추출
                                self.log_communication(
                                    f"  [DATA FIELD 전체 ({len(data_field_bytes)}바이트)]",
                                    "cyan"
                                )
                                
                                # 10바이트씩 끊어서 출력
                                for i in range(0, len(data_field_bytes), 10):
                                    chunk = data_field_bytes[i:i+10]
                                    hex_chunk = " ".join([f"{b:02X}" for b in chunk])
                                    self.log_communication(
                                        f"    [{i:3d}~{i+len(chunk)-1:3d}] {hex_chunk}",
                                        "gray"
                                    )
                            
                            # 처음 2개 테이블 값 확인
                            if total_length >= 12:
                                for i in range(2):
                                    idx = 5 + (i * 2)
                                    if idx + 1 < total_length - 3:  # ETX, CRC 제외
                                        high = data[idx]
                                        low = data[idx + 1]
                                        value = (high << 8) | low
                                        self.log_communication(
                                            f"  DATA{idx-3}~{idx-2} (테이블값 {i+1}): 0x{high:02X} 0x{low:02X} = {value}",
                                            "gray"
                                        )
                    else:
                        self.log_communication(
                            f"  DATA LENGTH: {data_len}바이트, 패킷 구조: STX(1) + TX_ID(1) + CMD(1) + LEN(1) + DATA({data_len}) + CRC(2) + ETX(1)",
                            "gray"
                        )
                    
                    # 전체 패킷 HEX 출력 (CMD 0xB3가 아닌 경우만, CMD 0xB3는 너무 길어서 생략)
                    if cmd != 0xB3:
                        hex_packet = " ".join([f"{b:02X}" for b in data])
                        self.log_communication(f"  전체 패킷 (HEX): {hex_packet}", "gray")
                    else:
                        # CMD 0xB3는 첫 10바이트와 마지막 10바이트만 표시
                        hex_start = " ".join([f"{b:02X}" for b in data[:10]])
                        hex_end = " ".join([f"{b:02X}" for b in data[-10:]])
                        self.log_communication(f"  패킷 시작 (10B): {hex_start}", "gray")
                        self.log_communication(f"  ... (중간 {total_length-20}바이트 생략)", "gray")
                        self.log_communication(f"  패킷 끝 (10B): {hex_end}", "gray")
        
        except Exception as e:
            if self.debug_comm:
                self.log_communication(f"송신 로그 오류: {str(e)}", "red")
    
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
    
    def export_log(self):
        """통신 로그를 파일로 저장"""
        # 현재 로그 내용 가져오기
        log_content = self.comm_text.get("1.0", tk.END)
        
        if not log_content.strip():
            messagebox.showwarning("경고", "저장할 로그가 없습니다.")
            return
        
        # 파일 저장 다이얼로그
        file_path = filedialog.asksaveasfilename(
            title="통신 로그 저장",
            defaultextension=".txt",
            filetypes=[
                ("텍스트 파일", "*.txt"),
                ("모든 파일", "*.*")
            ],
            initialfile=f"comm_log_{datetime.now().strftime('%Y%m%d_%H%M%S')}.txt"
        )
        
        if file_path:
            try:
                with open(file_path, 'w', encoding='utf-8') as f:
                    f.write(log_content)
                self.log_communication(f"로그 저장 완료: {os.path.basename(file_path)}", "green")
                messagebox.showinfo("성공", f"로그가 저장되었습니다.\n\n{file_path}")
            except Exception as e:
                self.log_communication(f"로그 저장 실패: {str(e)}", "red")
                messagebox.showerror("오류", f"로그 저장 중 오류가 발생했습니다.\n{str(e)}")
    
    def clear_log(self):
        """통신 로그 삭제"""
        result = messagebox.askyesno(
            "확인",
            "통신 로그를 모두 삭제하시겠습니까?"
        )
        
        if result:
            self.comm_text.delete("1.0", tk.END)
            self.log_communication("로그가 삭제되었습니다.", "blue")
    
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
            # 그래프에서 제거되었음을 로그로 표시
            sensor_names = {
                'outdoor_temp1': '외기온도 1',
                'outdoor_temp2': '외기온도 2',
                'purified_temp': '정수온도',
                'cold_temp': '냉수온도',
                'hot_inlet_temp': '온수 입수온도',
                'hot_internal_temp': '온수 내부온도',
                'hot_outlet_temp': '온수 출수온도'
            }
            sensor_name = sensor_names.get(item_key, item_key)
            self.log_communication(f"그래프 2에서 제거: {sensor_name}", "gray")
        else:
            self.graph2_active_items.add(item_key)
            self.update_item_visual(item_key, True, graph_num=2)
            # 그래프에 추가되었음을 로그로 표시
            sensor_names = {
                'outdoor_temp1': '외기온도 1',
                'outdoor_temp2': '외기온도 2',
                'purified_temp': '정수온도',
                'cold_temp': '냉수온도',
                'hot_inlet_temp': '온수 입수온도',
                'hot_internal_temp': '온수 내부온도',
                'hot_outlet_temp': '온수 출수온도'
            }
            sensor_name = sensor_names.get(item_key, item_key)
            self.log_communication(f"그래프 2에 추가: {sensor_name}", "blue")
    
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
        valve_data = self.valve_system.get_data()
        for i in range(1, 6):
            valve_state = 1 if valve_data['nos_valve_states'].get(i, False) else 0
            self.all_graph_data[f'nos_valve_{i}'].append(valve_state)
        
        for i in range(1, 16):
            valve_state = 1 if valve_data['feed_valve_states'].get(i, False) else 0
            self.all_graph_data[f'feed_valve_{i}'].append(valve_state)
        
        # 냉각 시스템
        cooling_data = self.cooling_system.get_data()
        cooling_op = 1 if cooling_data.get('operation_state') == 'GOING' or cooling_data.get('operation_state') == '가동' else 0
        self.all_graph_data['cooling_operation'].append(cooling_op)
        self.all_graph_data['cooling_on_temp'].append(float(cooling_data.get('on_temp', 0)))
        self.all_graph_data['cooling_off_temp'].append(float(cooling_data.get('off_temp', 0)))
        
        # 제빙 시스템
        icemaking_data = self.icemaking_system.get_data()
        self.all_graph_data['icemaking_time'].append(float(icemaking_data.get('icemaking_time', 0)))
        self.all_graph_data['icemaking_capacity'].append(float(icemaking_data.get('water_capacity', 0)))
        
        # 드레인
        drain_tank_data = self.drain_tank_system.get_data()
        drain_pump_data = self.drain_pump_system.get_data()
        tank_level = 1 if drain_tank_data.get('high_level') == '감지' else 0
        pump_state = 1 if drain_pump_data.get('operation_state') == 'ON' else 0
        self.all_graph_data['drain_tank_level'].append(tank_level)
        self.all_graph_data['drain_pump_state'].append(pump_state)
    
    def update_gui(self):
        """GUI 업데이트"""
        # 밸브 시스템은 자체적으로 GUI를 업데이트하므로 여기서는 처리하지 않음
        # 대신 valve_system의 update_gui 메서드를 호출하거나, 직접 레이블을 업데이트
        # NOS 밸브 상태 업데이트 (양쪽 탭 모두)
        if hasattr(self.valve_system, 'nos_valve_states'):
            for valve_num, is_closed in self.valve_system.nos_valve_states.items():
                # 냉동검토용 탭
                if hasattr(self.valve_system, 'nos_valve_labels') and valve_num in self.valve_system.nos_valve_labels:
                    label = self.valve_system.nos_valve_labels[valve_num]
                    if is_closed:
                        label.config(text="CLOSE", bg="red")
                    else:
                        label.config(text="OPEN", bg="blue")
                
                # 제어검토용 탭
                if hasattr(self.valve_system, 'nos_valve_labels_control') and valve_num in self.valve_system.nos_valve_labels_control:
                    label = self.valve_system.nos_valve_labels_control[valve_num]
                    if is_closed:
                        label.config(text="CLOSE", bg="red")
                    else:
                        label.config(text="OPEN", bg="blue")
        
        # FEED 밸브 상태 업데이트 (양쪽 탭 모두)
        if hasattr(self.valve_system, 'feed_valve_states'):
            for valve_num, is_open in self.valve_system.feed_valve_states.items():
                # 냉동검토용 탭
                if hasattr(self.valve_system, 'feed_valve_labels') and valve_num in self.valve_system.feed_valve_labels:
                    label = self.valve_system.feed_valve_labels[valve_num]
                    if is_open:
                        label.config(text="OPEN", bg="blue")
                    else:
                        label.config(text="CLOSE", bg="red")
                
                # 제어검토용 탭
                if hasattr(self.valve_system, 'feed_valve_labels_control') and valve_num in self.valve_system.feed_valve_labels_control:
                    label = self.valve_system.feed_valve_labels_control[valve_num]
                    if is_open:
                        label.config(text="OPEN", bg="blue")
                    else:
                        label.config(text="CLOSE", bg="red")
        
        # 센서 데이터 업데이트
        for sensor_key, value in self.sensor_data.items():
            if sensor_key in self.sensor_labels:
                label = self.sensor_labels[sensor_key]
                try:
                    # 숫자 타입으로 변환하여 표시
                    numeric_value = float(value) if value is not None else 0.0
                    label.config(text=f"{numeric_value:.1f}")
                except (ValueError, TypeError):
                    label.config(text="0.0")
        
        # 냉각 시스템 상태 업데이트 (시스템 클래스의 update_gui 메서드 사용)
        self.cooling_system._update_gui()

        # 공조시스템 상태 업데이트 (시스템 클래스의 update_gui 메서드 사용)
        self.hvac_system._update_gui()
        
        # 제빙 시스템 상태 업데이트 (시스템 클래스의 update_gui 메서드 사용)
        self.icemaking_system._update_gui()
        
        # 드레인 탱크 상태 업데이트 (시스템 클래스의 update_gui 메서드 사용)
        self.drain_tank_system._update_gui()
        
        # 드레인 펌프 상태 업데이트 (시스템 클래스의 update_gui 메서드 사용)
        self.drain_pump_system._update_gui()
        
        

        # 보냉시스템 데이터 업데이트
        refrigeration_data = self.refrigeration_system.get_data()
        self.refrigeration_system.update_data(refrigeration_data)
        
        # 그래프 업데이트
        self.update_graphs()
        
        # 다음 업데이트 예약
        self.root.after(200, self.update_gui)
    
    def update_graphs(self):
        """선택된 항목들만 그래프에 표시"""
        # 그래프 데이터가 없으면 업데이트하지 않음
        if len(self.all_graph_data['time']) < 2:
            return
        
        # 그래프 2에 표시할 센서가 없으면 그래프를 그리지 않음
        if not self.graph2_active_items:
            # 그래프 2를 비워둠
            if hasattr(self, 'pressure_ax_freezing'):
                self.pressure_ax_freezing.clear()
                self.pressure_ax_freezing.set_title("Selected Sensors (Graph 2 - Freezing)", fontsize=8, fontfamily='DejaVu Sans')
                self.pressure_ax_freezing.set_ylabel("Temperature (°C)", fontsize=7, fontfamily='DejaVu Sans')
                self.pressure_ax_freezing.set_xlabel("Time", fontsize=7, fontfamily='DejaVu Sans')
                self.pressure_ax_freezing.grid(True, alpha=0.3)
                self.fig2_freezing.tight_layout()
                try:
                    self.canvas2_freezing.draw_idle()
                except Exception:
                    pass
            
            if hasattr(self, 'pressure_ax_control'):
                self.pressure_ax_control.clear()
                self.pressure_ax_control.set_title("Selected Sensors (Graph 2 - Control)", fontsize=8, fontfamily='DejaVu Sans')
                self.pressure_ax_control.set_ylabel("Temperature (°C)", fontsize=7, fontfamily='DejaVu Sans')
                self.pressure_ax_control.set_xlabel("Time", fontsize=7, fontfamily='DejaVu Sans')
                self.pressure_ax_control.grid(True, alpha=0.3)
                self.fig2_control.tight_layout()
                try:
                    self.canvas2_control.draw_idle()
                except Exception:
                    pass
        
        try:
            times = list(self.all_graph_data['time'])
            
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
                        # times와 values의 길이가 같아야 함
                        values = list(self.all_graph_data[data_key])
                        if len(values) != len(times):
                            # 길이가 다르면 짧은 쪽에 맞춤
                            min_len = min(len(times), len(values))
                            times_plot = times[-min_len:] if len(times) > min_len else times
                            values_plot = values[-min_len:] if len(values) > min_len else values
                        else:
                            times_plot = times
                            values_plot = values
                        
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
                        
                        self.pressure_ax_freezing.plot(times_plot, values_plot, color=color, label=label, linewidth=1.5)
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
                        # times와 values의 길이가 같아야 함
                        values = list(self.all_graph_data[data_key])
                        if len(values) != len(times):
                            # 길이가 다르면 짧은 쪽에 맞춤
                            min_len = min(len(times), len(values))
                            times_plot = times[-min_len:] if len(times) > min_len else times
                            values_plot = values[-min_len:] if len(values) > min_len else values
                        else:
                            times_plot = times
                            values_plot = values
                        
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
                        
                        self.pressure_ax_control.plot(times_plot, values_plot, color=color, label=label, linewidth=1.5)
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