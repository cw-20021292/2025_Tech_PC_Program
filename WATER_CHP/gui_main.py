"""
메인 GUI 모듈 - 태그된 이미지 레이아웃
"""
import tkinter as tk
from tkinter import ttk, messagebox
import threading
import time
from datetime import datetime, timedelta
from collections import deque
import queue

# matplotlib 백엔드 설정 (GUI 에러 방지)
import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure
import matplotlib.font_manager as fm

# 한글 폰트 설정
plt.rcParams['font.family'] = ['Malgun Gothic', 'DejaVu Sans', 'sans-serif']
plt.rcParams['axes.unicode_minus'] = False  # 마이너스 부호 깨짐 방지

from communication import SerialCommunication, DataParser


class MainGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("코웨이 정수기 시스템 검토 프로그램")
        self.root.geometry("800x850")
        self.root.resizable(False, False)
        
        # 통신 모듈 초기화
        self.comm = SerialCommunication()
        self.data_parser = DataParser()
        
        # 디버깅 정보: 통신 클래스 메서드 확인
        self.debug_communication_class()
        
        # 데이터 저장소
        # 밸브류
        self.nos_valve_states = {i: False for i in range(1, 6)}  # NOS 밸브 5개
        self.feed_valve_states = {i: False for i in range(1, 16)}  # FEED 밸브 15개
        
        # 센서류
        self.sensor_data = {
            'outdoor_temp1': 0,
            'outdoor_temp2': 0,
            'purified_temp': 0,
            'cold_temp': 0,
            'hot_inlet_temp': 0,
            'hot_internal_temp': 0,
            'hot_outlet_temp': 0
        }
        
        # 공조시스템
        self.hvac_data = {
            'refrigerant_valve_state': '핫가스',
            'refrigerant_valve_target': '핫가스',
            'compressor_state': '미동작',
            'current_rps': 0,
            'target_rps': 0,
            'error_code': 0,
            'dc_fan1': 'OFF',
            'dc_fan2': 'OFF'
        }
        
        # 냉각시스템
        self.cooling_data = {
            'operation_state': 'STOP',
            'on_temp': 0,
            'off_temp': 0,
            'cooling_additional_time': 0
        }
        
        # 제빙시스템
        self.icemaking_data = {
            'operation': '대기',
            'icemaking_time': 0,
            'water_capacity': 0,
            'swing_on_time': 0,
            'swing_off_time': 0
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
        
        # 그래프 데이터 (최근 100개 데이터포인트)
        self.graph_data = {
            'time': deque(maxlen=100),
            'temp1': deque(maxlen=100),
            'temp2': deque(maxlen=100),
            'cold_temp': deque(maxlen=100),
            'hot_temp': deque(maxlen=100),
            'pressure1': deque(maxlen=100),
            'pressure2': deque(maxlen=100)
        }
        
        # 그래프 토글 상태 추적
        self.graph1_active_items = set()  # 그래프1에 표시할 항목들 (밸브, 냉각, 제빙, 탱크, 펌프)
        self.graph2_active_items = set()  # 그래프2에 표시할 항목들 (센서류)
        
        # 모든 데이터 항목의 그래프 데이터 저장소
        self.all_graph_data = {
            'time': deque(maxlen=100),
            # 센서류 (그래프2용)
            'outdoor_temp1': deque(maxlen=100),
            'outdoor_temp2': deque(maxlen=100),
            'purified_temp': deque(maxlen=100),
            'cold_temp_sensor': deque(maxlen=100),
            'hot_inlet_temp': deque(maxlen=100),
            'hot_internal_temp': deque(maxlen=100),
            'hot_outlet_temp': deque(maxlen=100),
            # 밸브류 (그래프1용)
            **{f'nos_valve_{i}': deque(maxlen=100) for i in range(1, 6)},
            **{f'feed_valve_{i}': deque(maxlen=100) for i in range(1, 16)},
            # 냉각 시스템 (그래프1용)
            'cooling_operation': deque(maxlen=100),
            'cooling_on_temp': deque(maxlen=100),
            'cooling_off_temp': deque(maxlen=100),
            # 제빙 시스템 (그래프1용)
            'icemaking_time': deque(maxlen=100),
            'icemaking_capacity': deque(maxlen=100),
            # 드레인 (그래프1용)
            'drain_tank_level': deque(maxlen=100),
            'drain_pump_state': deque(maxlen=100)
        }
        
        # GUI 위젯 참조
        self.nos_valve_labels = {}
        self.feed_valve_labels = {}
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
        # 메인 컨테이너 프레임
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
        """냉동검토용 탭 생성 (기존 화면)"""
        # 냉동검토용 탭 프레임
        freezing_frame = ttk.Frame(self.notebook)
        self.notebook.add(freezing_frame, text="냉동검토용")
        
        # 메인 프레임 (패딩 최소화)
        main_frame = ttk.Frame(freezing_frame, padding="2")
        main_frame.pack(fill=tk.BOTH, expand=True)
        
        # 상단 영역 (4개 영역: 냉각, 공조시스템, 제빙, 그래프 1&2)
        top_frame = ttk.Frame(main_frame)
        top_frame.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S), pady=(0, 2))
        
        # 중단 영역 (밸브류, 센서류) - 더 많은 공간 할당
        middle_frame = ttk.Frame(main_frame)
        middle_frame.grid(row=1, column=0, sticky=(tk.W, tk.E, tk.N, tk.S), pady=(0, 2))
        
        # 하단 영역 (드레인 탱크, 드레인 펌프)
        bottom_frame = ttk.Frame(main_frame)
        bottom_frame.grid(row=2, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))
        
        # 상단 영역 레이아웃
        self.create_cooling_area(top_frame)      # 냉각
        self.create_hvac_area(top_frame)         # 공조시스템  
        self.create_icemaking_area(top_frame)    # 제빙
        self.create_graph_areas(top_frame)       # 그래프 1&2 (공통)
        
        # 중단 영역 레이아웃
        self.create_valve_area(middle_frame)     # 밸브류 (공통)
        self.create_sensor_area(middle_frame)    # 센서류
        
        # 하단 영역 레이아웃
        self.create_drain_tank_area(bottom_frame)   # 드레인 탱크
        self.create_drain_pump_area(bottom_frame)   # 드레인 펌프 (통신부 제외)
        
        # 프레임 확장 설정
        # 상단 프레임 (4개 영역)
        for i in range(4):
            top_frame.columnconfigure(i, weight=1)
        top_frame.rowconfigure(0, weight=1)
        
        # 중단 프레임 (2개 영역)
        middle_frame.columnconfigure(0, weight=1)
        middle_frame.columnconfigure(1, weight=1)
        middle_frame.rowconfigure(0, weight=1)
        
        # 하단 프레임 (2개 영역)
        bottom_frame.columnconfigure(0, weight=1)
        bottom_frame.columnconfigure(1, weight=1)
        bottom_frame.rowconfigure(0, weight=1)
        
        # 메인 프레임 (800x800 정사각형 비율 최적화)
        main_frame.columnconfigure(0, weight=1)
        main_frame.rowconfigure(0, weight=3)  # 상단 영역 (냉각, 공조, 제빙, 그래프)
        main_frame.rowconfigure(1, weight=4)  # 중단 영역 (밸브류, 센서류)
        main_frame.rowconfigure(2, weight=1)  # 하단 영역 (드레인)
    
    def create_control_tab(self):
        """제어검토용 탭 생성 (새로운 화면)"""
        # 제어검토용 탭 프레임
        control_frame = ttk.Frame(self.notebook)
        self.notebook.add(control_frame, text="제어검토용")
        
        # 메인 프레임 (패딩 최소화)
        main_frame = ttk.Frame(control_frame, padding="2")
        main_frame.pack(fill=tk.BOTH, expand=True)
        
        # 상단 영역 (그래프 1&2)
        top_frame = ttk.Frame(main_frame)
        top_frame.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S), pady=(0, 2))
        
        # 중단 영역 (밸브류)
        middle_frame = ttk.Frame(main_frame)
        middle_frame.grid(row=1, column=0, sticky=(tk.W, tk.E, tk.N, tk.S), pady=(0, 2))
        
        # 하단 영역 (제어 관련 섹션들)
        bottom_frame = ttk.Frame(main_frame)
        bottom_frame.grid(row=2, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))
        
        # 상단 영역 레이아웃 (그래프만)
        self.create_graph_areas_control(top_frame)  # 그래프 1&2 (공통)
        
        # 중단 영역 레이아웃 (밸브류만)
        self.create_valve_area_control(middle_frame)  # 밸브류 (공통)
        
        # 하단 영역 레이아웃 (제어 관련)
        self.create_control_sections(bottom_frame)  # 제어 관련 섹션들
        
        # 프레임 확장 설정
        main_frame.columnconfigure(0, weight=1)
        main_frame.rowconfigure(0, weight=2)  # 그래프 영역
        main_frame.rowconfigure(1, weight=4)  # 밸브류 영역
        main_frame.rowconfigure(2, weight=3)  # 제어 섹션 영역 (통신부 제거로 확장)
    
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
        
        # ON 온도
        on_temp_frame = ttk.Frame(cooling_frame)
        on_temp_frame.grid(row=1, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(on_temp_frame, text="ON 온도:", font=("Arial", 8), width=8).pack(side=tk.LEFT)
        self.cooling_labels['on_temp'] = tk.Label(on_temp_frame, text="0", 
                                                 font=("Arial", 8), bg="white", relief="sunken", width=6)
        self.cooling_labels['on_temp'].pack(side=tk.LEFT, padx=(2, 0))
        ttk.Label(on_temp_frame, text="℃", font=("Arial", 8)).pack(side=tk.LEFT)
        
        # OFF 온도
        off_temp_frame = ttk.Frame(cooling_frame)
        off_temp_frame.grid(row=2, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(off_temp_frame, text="OFF 온도:", font=("Arial", 8), width=8).pack(side=tk.LEFT)
        self.cooling_labels['off_temp'] = tk.Label(off_temp_frame, text="0", 
                                                  font=("Arial", 8), bg="white", relief="sunken", width=6)
        self.cooling_labels['off_temp'].pack(side=tk.LEFT, padx=(2, 0))
        ttk.Label(off_temp_frame, text="℃", font=("Arial", 8)).pack(side=tk.LEFT)
        
        # 냉각 추가시간
        add_time_frame = ttk.Frame(cooling_frame)
        add_time_frame.grid(row=3, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(add_time_frame, text="추가시간:", font=("Arial", 8), width=8).pack(side=tk.LEFT)
        self.cooling_labels['cooling_additional_time'] = tk.Label(add_time_frame, text="0", 
                                                                 font=("Arial", 8), bg="white", relief="sunken", width=6)
        self.cooling_labels['cooling_additional_time'].pack(side=tk.LEFT, padx=(2, 0))
        ttk.Label(add_time_frame, text="초", font=("Arial", 8)).pack(side=tk.LEFT)
        
        cooling_frame.columnconfigure(0, weight=1)
    
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
        
        # 목표
        target_frame = ttk.Frame(valve_subframe)
        target_frame.grid(row=1, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(target_frame, text="목표:", font=("Arial", 8)).pack(side=tk.LEFT)
        self.hvac_labels['refrigerant_valve_target'] = tk.Label(target_frame, text="핫가스", 
                                                               fg="white", bg="orange", font=("Arial", 8, "bold"),
                                                               width=8, relief="raised")
        self.hvac_labels['refrigerant_valve_target'].pack(side=tk.RIGHT)
        
        # 압축기 서브프레임
        comp_subframe = ttk.LabelFrame(hvac_frame, text="압축기", padding="3")
        comp_subframe.grid(row=1, column=0, columnspan=2, sticky=(tk.W, tk.E), pady=(0, 5))
        
        # 상태
        comp_state_frame = ttk.Frame(comp_subframe)
        comp_state_frame.grid(row=0, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(comp_state_frame, text="상태:", font=("Arial", 8)).pack(side=tk.LEFT)
        self.hvac_labels['compressor_state'] = tk.Label(comp_state_frame, text="미동작", 
                                                       fg="white", bg="gray", font=("Arial", 8, "bold"),
                                                       width=8, relief="raised")
        self.hvac_labels['compressor_state'].pack(side=tk.RIGHT)
        
        # 현재 RPS
        curr_rps_frame = ttk.Frame(comp_subframe)
        curr_rps_frame.grid(row=1, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(curr_rps_frame, text="현재 RPS:", font=("Arial", 8)).pack(side=tk.LEFT)
        self.hvac_labels['current_rps'] = tk.Label(curr_rps_frame, text="0", 
                                                  font=("Arial", 8), bg="white", relief="sunken")
        self.hvac_labels['current_rps'].pack(side=tk.RIGHT)
        
        # 목표 RPS
        target_rps_frame = ttk.Frame(comp_subframe)
        target_rps_frame.grid(row=2, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(target_rps_frame, text="목표 RPS:", font=("Arial", 8)).pack(side=tk.LEFT)
        self.hvac_labels['target_rps'] = tk.Label(target_rps_frame, text="0", 
                                                 font=("Arial", 8), bg="white", relief="sunken")
        self.hvac_labels['target_rps'].pack(side=tk.RIGHT)
        
        # 에러코드
        error_frame = ttk.Frame(comp_subframe)
        error_frame.grid(row=3, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(error_frame, text="에러코드:", font=("Arial", 8)).pack(side=tk.LEFT)
        self.hvac_labels['error_code'] = tk.Label(error_frame, text="0", 
                                                 font=("Arial", 8), bg="white", relief="sunken")
        self.hvac_labels['error_code'].pack(side=tk.RIGHT)
        
        # DC FAN 1
        fan1_frame = ttk.Frame(comp_subframe)
        fan1_frame.grid(row=4, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(fan1_frame, text="DC FAN 1:", font=("Arial", 8)).pack(side=tk.LEFT)
        self.hvac_labels['dc_fan1'] = tk.Label(fan1_frame, text="OFF", 
                                              fg="white", bg="gray", font=("Arial", 8, "bold"),
                                              width=5, relief="raised")
        self.hvac_labels['dc_fan1'].pack(side=tk.RIGHT)
        
        # DC FAN 2
        fan2_frame = ttk.Frame(comp_subframe)
        fan2_frame.grid(row=5, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(fan2_frame, text="DC FAN 2:", font=("Arial", 8)).pack(side=tk.LEFT)
        self.hvac_labels['dc_fan2'] = tk.Label(fan2_frame, text="OFF", 
                                              fg="white", bg="gray", font=("Arial", 8, "bold"),
                                              width=5, relief="raised")
        self.hvac_labels['dc_fan2'].pack(side=tk.RIGHT)
        
        # 컬럼 확장
        valve_subframe.columnconfigure(0, weight=1)
        comp_subframe.columnconfigure(0, weight=1)
        hvac_frame.columnconfigure(0, weight=1)
    
    def create_icemaking_area(self, parent):
        """제빙 섹션 생성"""
        icemaking_frame = ttk.LabelFrame(parent, text="제빙", padding="2")
        icemaking_frame.grid(row=0, column=2, sticky=(tk.W, tk.E, tk.N, tk.S), padx=1)
        
        # 제빙 동작
        operation_frame = ttk.Frame(icemaking_frame)
        operation_frame.grid(row=0, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(operation_frame, text="제빙 동작:", font=("Arial", 9), width=9).pack(side=tk.LEFT)
        self.icemaking_labels['operation'] = tk.Label(operation_frame, text="대기", 
                                                     fg="white", bg="blue", font=("Arial", 8, "bold"),
                                                     width=10, relief="raised")
        self.icemaking_labels['operation'].pack(side=tk.LEFT, padx=(2, 0))
        
        # 제빙시간
        time_frame = ttk.Frame(icemaking_frame)
        time_frame.grid(row=1, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(time_frame, text="제빙시간:", font=("Arial", 9), width=9).pack(side=tk.LEFT)
        self.icemaking_labels['icemaking_time'] = tk.Label(time_frame, text="0", 
                                                          font=("Arial", 9), bg="white", relief="sunken", width=8)
        self.icemaking_labels['icemaking_time'].pack(side=tk.LEFT, padx=(2, 0))
        ttk.Label(time_frame, text="초", font=("Arial", 9)).pack(side=tk.LEFT)
        
        # 입수 용량
        capacity_frame = ttk.Frame(icemaking_frame)
        capacity_frame.grid(row=2, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(capacity_frame, text="입수 용량:", font=("Arial", 9), width=9).pack(side=tk.LEFT)
        self.icemaking_labels['water_capacity'] = tk.Label(capacity_frame, text="0", 
                                                           font=("Arial", 9), bg="white", relief="sunken", width=8)
        self.icemaking_labels['water_capacity'].pack(side=tk.LEFT, padx=(2, 0))
        ttk.Label(capacity_frame, text="Hz", font=("Arial", 9)).pack(side=tk.LEFT)
        
        # 스윙바 ON 시간
        swing_on_frame = ttk.Frame(icemaking_frame)
        swing_on_frame.grid(row=3, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(swing_on_frame, text="스윙바 ON:", font=("Arial", 9), width=9).pack(side=tk.LEFT)
        self.icemaking_labels['swing_on_time'] = tk.Label(swing_on_frame, text="0", 
                                                         font=("Arial", 9), bg="white", relief="sunken", width=8)
        self.icemaking_labels['swing_on_time'].pack(side=tk.LEFT, padx=(2, 0))
        ttk.Label(swing_on_frame, text="ms", font=("Arial", 9)).pack(side=tk.LEFT)
        
        # 스윙바 OFF 시간
        swing_off_frame = ttk.Frame(icemaking_frame)
        swing_off_frame.grid(row=4, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(swing_off_frame, text="스윙바 OFF:", font=("Arial", 9), width=9).pack(side=tk.LEFT)
        self.icemaking_labels['swing_off_time'] = tk.Label(swing_off_frame, text="0", 
                                                          font=("Arial", 9), bg="white", relief="sunken", width=8)
        self.icemaking_labels['swing_off_time'].pack(side=tk.LEFT, padx=(2, 0))
        ttk.Label(swing_off_frame, text="ms", font=("Arial", 9)).pack(side=tk.LEFT)
        
        icemaking_frame.columnconfigure(0, weight=1)
    
    def create_graph_areas(self, parent):
        """그래프 1, 2 영역 생성"""
        graph_container = ttk.Frame(parent)
        graph_container.grid(row=0, column=3, sticky=(tk.W, tk.E, tk.N, tk.S), padx=(2, 0))
        
        # 그래프 1
        graph1_frame = ttk.LabelFrame(graph_container, text="그래프 1", padding="3")
        graph1_frame.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S), pady=(0, 2))
        
        # 그래프 2
        graph2_frame = ttk.LabelFrame(graph_container, text="그래프 2", padding="3")
        graph2_frame.grid(row=1, column=0, sticky=(tk.W, tk.E, tk.N, tk.S), pady=(2, 0))
        
        # 냉동검토용 탭의 그래프인지 확인 (간단한 방법)
        # 첫 번째 호출은 냉동검토용, 두 번째 호출은 제어검토용으로 간주
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
                canvas1_widget.configure(takefocus=0)
                
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
                canvas2_widget.configure(takefocus=0)
                
                # 기본 참조 설정 (냉동검토용이 기본)
                self.fig1 = self.fig1_freezing
                self.temp_ax = self.temp_ax_freezing
                self.canvas1 = self.canvas1_freezing
                self.fig2 = self.fig2_freezing
                self.pressure_ax = self.pressure_ax_freezing
                self.canvas2 = self.canvas2_freezing
                
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
                canvas1_widget.configure(takefocus=0)
                
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
                canvas2_widget.configure(takefocus=0)
            
        except Exception as e:
            # 그래프 생성 실패 시 대체 라벨 표시
            error_label1 = tk.Label(graph1_frame, text=f"그래프1 오류: {str(e)}", 
                                   fg="red", font=("Arial", 8))
            error_label1.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))
            
            error_label2 = tk.Label(graph2_frame, text=f"그래프2 오류: {str(e)}", 
                                   fg="red", font=("Arial", 8))
            error_label2.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))
        
        # 프레임 확장 설정
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
        
        # NOS 밸브 서브프레임
        nos_frame = ttk.LabelFrame(valve_frame, text="NOS 밸브 (데이터 1=CLOSE, 0=OPEN)", padding="3")
        nos_frame.grid(row=1, column=0, columnspan=5, sticky=(tk.W, tk.E), pady=(0, 5))
        
        # NOS 밸브 1~5 상태 표시
        for i in range(1, 6):
            nos_container = ttk.Frame(nos_frame)
            nos_container.grid(row=0, column=i-1, padx=3, pady=2, sticky=tk.W)
            
            # 밸브 번호
            num_label = tk.Label(nos_container, text=f"NOS{i}:", font=("Arial", 7), width=6)
            num_label.pack(side=tk.TOP)
            
            # 상태 표시 (클릭 가능)
            status_label = tk.Label(nos_container, text="CLOSE", 
                                  fg="white", bg="red",
                                  font=("Arial", 7, "bold"),
                                  width=6, relief="raised", bd=1, cursor="hand2")
            status_label.pack(side=tk.TOP, pady=(2, 0))
            
            # 클릭 이벤트 바인딩 (그래프1용)
            status_label.bind("<Button-1>", lambda e, valve_key=f"nos_valve_{i}": self.toggle_graph1_item(valve_key))
            
            self.nos_valve_labels[i] = status_label
        
        # FEED 밸브 서브프레임
        feed_frame = ttk.LabelFrame(valve_frame, text="FEED 밸브 (데이터 1=OPEN, 0=CLOSE)", padding="3")
        feed_frame.grid(row=2, column=0, columnspan=5, sticky=(tk.W, tk.E), pady=(0, 5))
        
        # FEED 밸브 1~15 상태 표시 (3행 5열)
        for i in range(1, 16):
            row = (i - 1) // 5
            col = (i - 1) % 5
            
            feed_container = ttk.Frame(feed_frame)
            feed_container.grid(row=row, column=col, padx=2, pady=2, sticky=tk.W)
            
            # 밸브 번호
            num_label = tk.Label(feed_container, text=f"F{i:2d}:", font=("Arial", 7), width=4)
            num_label.pack(side=tk.LEFT)
            
            # 상태 표시 (클릭 가능)
            status_label = tk.Label(feed_container, text="CLOSE", 
                                  fg="white", bg="red",
                                  font=("Arial", 6, "bold"),
                                  width=5, relief="raised", bd=1, cursor="hand2")
            status_label.pack(side=tk.LEFT, padx=(2, 0))
            
            # 클릭 이벤트 바인딩 (그래프1용)
            status_label.bind("<Button-1>", lambda e, valve_key=f"feed_valve_{i}": self.toggle_graph1_item(valve_key))
            
            self.feed_valve_labels[i] = status_label
        
        # 컬럼 확장 설정
        for i in range(5):
            nos_frame.columnconfigure(i, weight=1)
            feed_frame.columnconfigure(i, weight=1)
    
    def create_sensor_area(self, parent):
        """센서류 섹션 생성"""
        sensor_frame = ttk.LabelFrame(parent, text="센서류", padding="2")
        sensor_frame.grid(row=0, column=1, sticky=(tk.W, tk.E, tk.N, tk.S), padx=2)
        
        # 온도 센서들
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
            
            # 센서 라벨
            ttk.Label(sensor_container, text=f"{label_text}:", font=("Arial", 7), width=12).pack(side=tk.LEFT)
            
            # 값 표시 (클릭 가능)
            value_label = tk.Label(sensor_container, text="0.0", 
                                 fg="black", bg="white",
                                 font=("Arial", 7, "bold"),
                                 width=6, relief="sunken", bd=1, cursor="hand2")
            value_label.pack(side=tk.LEFT, padx=(2, 0))
            ttk.Label(sensor_container, text="℃", font=("Arial", 7)).pack(side=tk.LEFT)
            
            # 클릭 이벤트 바인딩 (그래프2용)
            value_label.bind("<Button-1>", lambda e, sensor_key=key: self.toggle_graph2_item(sensor_key))
            
            self.sensor_labels[key] = value_label
        
        # 컬럼 균등 배치
        sensor_frame.columnconfigure(0, weight=1)
        sensor_frame.columnconfigure(1, weight=1)
    
    def create_drain_tank_area(self, parent):
        """드레인 탱크 섹션 생성"""
        drain_tank_frame = ttk.LabelFrame(parent, text="드레인 탱크", padding="2")
        drain_tank_frame.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S), padx=(0, 2))
        
        # 저수위 센서
        low_level_frame = ttk.Frame(drain_tank_frame)
        low_level_frame.grid(row=0, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(low_level_frame, text="저수위:", font=("Arial", 9), width=8).pack(side=tk.LEFT)
        self.drain_tank_labels['low_level'] = tk.Label(low_level_frame, text="미감지", 
                                                      fg="white", bg="gray", font=("Arial", 8, "bold"),
                                                      width=8, relief="raised")
        self.drain_tank_labels['low_level'].pack(side=tk.LEFT, padx=(2, 0))
        
        # 만수위 센서
        high_level_frame = ttk.Frame(drain_tank_frame)
        high_level_frame.grid(row=1, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(high_level_frame, text="만수위:", font=("Arial", 9), width=8).pack(side=tk.LEFT)
        self.drain_tank_labels['high_level'] = tk.Label(high_level_frame, text="미감지", 
                                                       fg="white", bg="gray", font=("Arial", 8, "bold"),
                                                       width=8, relief="raised")
        self.drain_tank_labels['high_level'].pack(side=tk.LEFT, padx=(2, 0))
        
        # 수위 상태
        water_state_frame = ttk.Frame(drain_tank_frame)
        water_state_frame.grid(row=2, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(water_state_frame, text="수위상태:", font=("Arial", 9), width=8).pack(side=tk.LEFT)
        self.drain_tank_labels['water_level_state'] = tk.Label(water_state_frame, text="비어있음", 
                                                              fg="white", bg="blue", font=("Arial", 8, "bold"),
                                                              width=8, relief="raised")
        self.drain_tank_labels['water_level_state'].pack(side=tk.LEFT, padx=(2, 0))
        
        drain_tank_frame.columnconfigure(0, weight=1)
    
    def create_drain_pump_area(self, parent):
        """드레인 펌프 섹션 생성 (통신부 제외)"""
        drain_pump_frame = ttk.LabelFrame(parent, text="드레인 펌프", padding="2")
        drain_pump_frame.grid(row=0, column=1, sticky=(tk.W, tk.E, tk.N, tk.S), padx=(1, 0))
        
        # 드레인 펌프 상태 라벨들
        self.drain_pump_labels = {}
        
        # 운전 상태
        state_frame = ttk.Frame(drain_pump_frame)
        state_frame.grid(row=0, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(state_frame, text="운전 상태:", font=("Arial", 7), width=8).pack(side=tk.LEFT)
        self.drain_pump_labels['operation_state'] = tk.Label(state_frame, text="OFF", 
                                                           fg="white", bg="red", font=("Arial", 7, "bold"),
                                                           width=6, relief="raised")
        self.drain_pump_labels['operation_state'].pack(side=tk.LEFT, padx=(2, 0))
        
        # 컬럼 확장 설정
        drain_pump_frame.columnconfigure(0, weight=1)
    
    def refresh_ports(self):
        """포트 목록 새로고침 (초기화용)"""
        ports = self.comm.get_available_ports()
        self.port_combo['values'] = ports
        if ports:
            self.port_combo.set(ports[0])
    
    def refresh_ports_with_log(self):
        """포트 목록 새로고침 (사용자 액션용, 로그 포함)"""
        # 현재 선택된 포트 저장
        current_port = self.port_var.get()
        
        # 포트 목록 새로고침
        ports = self.comm.get_available_ports()
        self.port_combo['values'] = ports
        
        # 포트 개수 로그 출력
        port_count = len(ports)
        self.log_communication(f"포트 새로고침: {port_count}개 포트 발견", "blue")
        
        # 이전에 선택된 포트가 여전히 존재하는지 확인
        if current_port and current_port in ports:
            # 기존 포트 유지
            self.port_combo.set(current_port)
            self.log_communication(f"기존 포트 {current_port} 유지", "green")
        elif ports:
            # 기존 포트가 없으면 첫 번째 포트 선택
            self.port_combo.set(ports[0])
            selected_port = ports[0].split(" - ")[0] if " - " in ports[0] else ports[0]
            self.log_communication(f"새 포트 {selected_port} 자동 선택", "orange")
        else:
            # 포트가 없는 경우
            self.port_combo.set("")
            self.log_communication("사용 가능한 포트가 없습니다", "red")
        
        # 발견된 포트 목록 및 사용 가능성 출력
        if ports:
            for i, port in enumerate(ports, 1):
                port_name = port.split(" - ")[0] if " - " in port else port
                
                # 각 포트의 사용 가능성 확인 (빠른 체크)
                try:
                    available, _ = self.comm.check_port_availability(port)
                    status = "✓ 사용가능" if available else "✗ 사용중"
                    color = "green" if available else "orange"
                except:
                    status = "? 확인중"
                    color = "gray"
                
                self.log_communication(f"  {i}. {port_name} ({status})", color)
    
    def toggle_connection(self):
        """연결/연결해제 토글"""
        if not self.comm.is_connected:
            port = self.port_var.get()
            baudrate = self.baudrate_var.get()
            
            if not port:
                messagebox.showerror("오류", "포트를 선택해주세요.")
                return
            
            # 포트 사용 가능성 미리 확인
            self.log_communication(f"포트 {port.split(' - ')[0]} 사용 가능성 확인 중...", "blue")
            port_available, port_message = self.comm.check_port_availability(port)
            
            if not port_available:
                error_title = "포트 사용 불가"
                full_message = f"{port_message}\n\n해결 방법:\n"
                
                if "사용 중" in port_message or "거부" in port_message:
                    full_message += "• 다른 시리얼 통신 프로그램을 종료하세요\n"
                    full_message += "• 장치 관리자에서 포트를 비활성화/활성화하세요\n"
                    full_message += "• USB 케이블을 재연결하세요"
                elif "찾을 수 없습니다" in port_message:
                    full_message += "• USB 장치가 올바르게 연결되었는지 확인하세요\n"
                    full_message += "• 장치 드라이버가 설치되었는지 확인하세요\n"
                    full_message += "• 포트 새로고침 버튼(⟳)을 클릭하세요"
                else:
                    full_message += "• 다른 포트를 선택하세요\n"
                    full_message += "• 시스템을 재시작하세요"
                
                messagebox.showerror(error_title, full_message)
                self.log_communication(f"포트 연결 실패: {port_message}", "red")
                return
            
            self.log_communication(f"포트 {port.split(' - ')[0]} 사용 가능 확인됨", "green")
            success, message = self.comm.connect(port, baudrate)
            if success:
                self.connect_btn.config(text="연결해제")
                self.status_label.config(text="연결됨", fg="green")
                self.comm_test_btn.config(state="normal")  # 통신테스트 버튼 활성화
                self.refresh_ports_btn.config(state="disabled")  # 연결 중에는 비활성화
                self.port_combo.config(state="disabled")  # 연결 중에는 포트 변경 방지
                self.log_communication(f"포트 {port} 연결됨", "green")
            else:
                # 상세한 오류 메시지와 해결 방법 표시
                self.show_connection_error_dialog(message)
                self.log_communication(f"연결 실패: {message}", "red")
        else:
            success, message = self.comm.disconnect()
            if success:
                self.connect_btn.config(text="연결")
                self.status_label.config(text="연결 안됨", fg="red")
                self.comm_test_btn.config(state="disabled")  # 통신테스트 버튼 비활성화
                self.refresh_ports_btn.config(state="normal")  # 연결 해제 시 활성화
                self.port_combo.config(state="readonly")  # 연결 해제 시 포트 선택 가능
                self.log_communication("연결 해제됨", "orange")
    
    def start_test_data(self):
        """테스트 데이터 생성 시작"""
        from test_data_generator import TestDataGenerator
        
        def test_data_callback(data):
            """테스트 데이터를 GUI로 전달"""
            self.process_received_data(data.encode('utf-8'))
        
        if not hasattr(self, 'test_generator'):
            self.test_generator = TestDataGenerator(test_data_callback)
        
        if not self.test_generator.running:
            self.test_generator.start()
            self.log_communication("테스트 데이터 생성 시작", "blue")
        else:
            self.test_generator.stop()
            self.log_communication("테스트 데이터 생성 중지", "red")
    
    def monitor_data(self):
        """데이터 모니터링 스레드"""
        while self.monitoring_active:
            # 수신된 데이터 처리
            received_data = self.comm.get_received_data()
            for msg_type, data in received_data:
                if msg_type == 'DATA':
                    self.process_received_data(data)
                elif msg_type == 'SENT':
                    self.log_communication(f"송신: {data.decode('utf-8', errors='replace')}", "blue")
                elif msg_type == 'ERROR':
                    self.log_communication(f"오류: {data}", "red")
            
            # 상태 업데이트 처리
            status_updates = self.comm.get_status_updates()
            for status_type, message in status_updates:
                self.log_communication(f"상태: {message}", "purple")
            
            time.sleep(0.1)
    
    def process_received_data(self, data):
        """수신된 데이터 처리"""
        try:
            data_string = data.decode('utf-8', errors='replace')
            
            # 통신 로그 기록
            self.log_communication(f"수신: {data_string}", "green")
            
            # 밸브 상태 파싱
            valve_updates = self.data_parser.parse_valve_status(data_string)
            # NOS 밸브 업데이트
            for valve_num, is_closed in valve_updates.get('nos_valves', {}).items():
                self.nos_valve_states[valve_num] = is_closed
            # FEED 밸브 업데이트
            for valve_num, is_open in valve_updates.get('feed_valves', {}).items():
                self.feed_valve_states[valve_num] = is_open
            
            # 시스템 상태 파싱
            system_updates = self.data_parser.parse_system_status(data_string)
            # 각 시스템별 상태 업데이트
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
            
            # 센서 데이터 파싱 및 업데이트
            sensor_data = self.data_parser.parse_sensor_data(data_string)
            if sensor_data:
                # 센서 라벨 업데이트용 데이터
                for sensor_key in self.sensor_data.keys():
                    if sensor_key in sensor_data:
                        self.sensor_data[sensor_key] = sensor_data[sensor_key]
                
                # 모든 그래프 데이터 업데이트 (실제 데이터 수신 시에만)
                self.update_all_graph_data()
        
        except Exception as e:
            self.log_communication(f"데이터 처리 오류: {str(e)}", "red")
    
    def log_communication(self, message, color="black"):
        """통신 로그 기록 (스레드 안전)"""
        def _log():
            timestamp = datetime.now().strftime("%H:%M:%S")
            self.comm_text.insert(tk.END, f"[{timestamp}] {message}\n")
            
            # 색상 적용
            line_start = self.comm_text.index("end-2l")
            line_end = self.comm_text.index("end-1l")
            tag_name = f"color_{color}"
            self.comm_text.tag_add(tag_name, line_start, line_end)
            self.comm_text.tag_config(tag_name, foreground=color)
            
            # 자동 스크롤
            self.comm_text.see(tk.END)
            
            # 로그 크기 제한
            line_count = int(self.comm_text.index(tk.END).split('.')[0])
            if line_count > 100:
                self.comm_text.delete(1.0, "2.0")
        
        # 메인 스레드에서 실행
        self.root.after(0, _log)
    
    def update_gui(self):
        """GUI 업데이트 (메인 스레드에서 실행)"""
        # NOS 밸브 상태 업데이트 (데이터 1=CLOSE, 0=OPEN)
        for valve_num, is_closed in self.nos_valve_states.items():
            if valve_num in self.nos_valve_labels:
                label = self.nos_valve_labels[valve_num]
                if is_closed:  # 데이터가 1이면 CLOSE
                    label.config(text="CLOSE", bg="red")
                else:  # 데이터가 0이면 OPEN
                    label.config(text="OPEN", bg="blue")
        
        # FEED 밸브 상태 업데이트 (데이터 1=OPEN, 0=CLOSE)
        for valve_num, is_open in self.feed_valve_states.items():
            if valve_num in self.feed_valve_labels:
                label = self.feed_valve_labels[valve_num]
                if is_open:  # 데이터가 1이면 OPEN
                    label.config(text="OPEN", bg="blue")
                else:  # 데이터가 0이면 CLOSE
                    label.config(text="CLOSE", bg="red")
        
        # 센서 데이터 업데이트
        for sensor_key, value in self.sensor_data.items():
            if sensor_key in self.sensor_labels:
                label = self.sensor_labels[sensor_key]
                label.config(text=f"{value:.1f}")
        
        # 공조시스템 상태 업데이트
        for hvac_key, value in self.hvac_data.items():
            if hvac_key in self.hvac_labels:
                label = self.hvac_labels[hvac_key]
                if hvac_key in ['refrigerant_valve_state', 'refrigerant_valve_target']:
                    # 냉매전환밸브 색상
                    colors = {'핫가스': 'red', '제빙': 'blue', '냉각': 'green'}
                    color = colors.get(value, 'gray')
                    label.config(text=value, bg=color)
                elif hvac_key == 'compressor_state':
                    # 압축기 상태
                    if value == '동작중':
                        label.config(text="동작중", bg="green")
                    else:   # 들여쓰기 수정하지 말 것
                        label.config(text="미동작", bg="gray")
                elif hvac_key in ['dc_fan1', 'dc_fan2']:
                    # DC FAN 상태
                    if value == 'ON':
                        label.config(text="ON", bg="green")
                    else:
                        label.config(text="OFF", bg="gray")
                else:
                    # 숫자 값들
                    label.config(text=str(value))
        
        # 냉각 시스템 상태 업데이트
        for cooling_key, value in self.cooling_data.items():
            if cooling_key in self.cooling_labels:
                label = self.cooling_labels[cooling_key]
                if cooling_key == 'operation_state':
                    if value == 'GOING':
                        label.config(text="GOING", bg="green")
                    else:
                        label.config(text="STOP", bg="red")
                elif cooling_key in ['on_temp', 'off_temp']:
                    label.config(text=str(value))
                elif cooling_key == 'cooling_additional_time':
                    label.config(text=str(value))
        
        # 제빙 시스템 상태 업데이트
        for ice_key, value in self.icemaking_data.items():
            if ice_key in self.icemaking_labels:
                label = self.icemaking_labels[ice_key]
                if ice_key == 'operation':
                    label.config(text=value)
                elif ice_key == 'icemaking_time':
                    label.config(text=str(value))
                elif ice_key == 'water_capacity':
                    label.config(text=str(value))
                elif ice_key in ['swing_on_time', 'swing_off_time']:
                    label.config(text=str(value))
        
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
            
            # 현재 활성 탭 확인
            current_tab = self.notebook.index(self.notebook.select())
            
            # 냉동검토용 탭 (인덱스 0)의 그래프 업데이트
            if hasattr(self, 'temp_ax_freezing'):
                self.temp_ax_freezing.clear()
                self.temp_ax_freezing.set_title("Selected Items (Graph 1 - Freezing)", fontsize=8, fontfamily='DejaVu Sans')
                self.temp_ax_freezing.set_ylabel("Value", fontsize=7, fontfamily='DejaVu Sans')
                self.temp_ax_freezing.grid(True, alpha=0.3)
                
                # 선택된 항목들만 그래프에 표시
                colors = ['red', 'blue', 'green', 'orange', 'purple', 'brown', 'pink', 'gray', 'olive', 'cyan']
                color_idx = 0
                
                for item_key in self.graph1_active_items:
                    if item_key in self.all_graph_data and len(self.all_graph_data[item_key]) > 0:
                        values = list(self.all_graph_data[item_key])
                        color = colors[color_idx % len(colors)]
                        
                        # 라벨 생성
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
            
            # 제어검토용 탭 (인덱스 1)의 그래프 업데이트
            if hasattr(self, 'temp_ax_control'):
                self.temp_ax_control.clear()
                self.temp_ax_control.set_title("Selected Items (Graph 1 - Control)", fontsize=8, fontfamily='DejaVu Sans')
                self.temp_ax_control.set_ylabel("Value", fontsize=7, fontfamily='DejaVu Sans')
                self.temp_ax_control.grid(True, alpha=0.3)
                
                # 선택된 항목들만 그래프에 표시
                colors = ['red', 'blue', 'green', 'orange', 'purple', 'brown', 'pink', 'gray', 'olive', 'cyan']
                color_idx = 0
                
                for item_key in self.graph1_active_items:
                    if item_key in self.all_graph_data and len(self.all_graph_data[item_key]) > 0:
                        values = list(self.all_graph_data[item_key])
                        color = colors[color_idx % len(colors)]
                        
                        # 라벨 생성
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
                        
                        self.temp_ax_control.plot(times, values, color=color, label=label, linewidth=1.5)
                        color_idx += 1
                
                if self.graph1_active_items:
                    self.temp_ax_control.legend(fontsize=6)
                
                self.fig1_control.tight_layout()
                try:
                    self.canvas1_control.draw_idle()
                except Exception:
                    pass
            
            # 냉동검토용 탭의 그래프 2 업데이트 (선택된 센서류 항목들)
            if hasattr(self, 'pressure_ax_freezing'):
                self.pressure_ax_freezing.clear()
                self.pressure_ax_freezing.set_title("Selected Sensors (Graph 2 - Freezing)", fontsize=8, fontfamily='DejaVu Sans')
                self.pressure_ax_freezing.set_ylabel("Temperature (°C)", fontsize=7, fontfamily='DejaVu Sans')
                self.pressure_ax_freezing.set_xlabel("Time", fontsize=7, fontfamily='DejaVu Sans')
                self.pressure_ax_freezing.grid(True, alpha=0.3)
                
                # 선택된 센서들만 그래프에 표시
                sensor_colors = ['red', 'blue', 'green', 'orange', 'purple', 'brown', 'pink']
                color_idx = 0
                
                for sensor_key in self.graph2_active_items:
                    data_key = sensor_key
                    if sensor_key == 'cold_temp':
                        data_key = 'cold_temp_sensor'
                    
                    if data_key in self.all_graph_data and len(self.all_graph_data[data_key]) > 0:
                        values = list(self.all_graph_data[data_key])
                        color = sensor_colors[color_idx % len(sensor_colors)]
                        
                        # 센서 라벨 생성
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
            
            # 제어검토용 탭의 그래프 2 업데이트 (선택된 센서류 항목들)
            if hasattr(self, 'pressure_ax_control'):
                self.pressure_ax_control.clear()
                self.pressure_ax_control.set_title("Selected Sensors (Graph 2 - Control)", fontsize=8, fontfamily='DejaVu Sans')
                self.pressure_ax_control.set_ylabel("Temperature (°C)", fontsize=7, fontfamily='DejaVu Sans')
                self.pressure_ax_control.set_xlabel("Time", fontsize=7, fontfamily='DejaVu Sans')
                self.pressure_ax_control.grid(True, alpha=0.3)
                
                # 선택된 센서들만 그래프에 표시
                sensor_colors = ['red', 'blue', 'green', 'orange', 'purple', 'brown', 'pink']
                color_idx = 0
                
                for sensor_key in self.graph2_active_items:
                    data_key = sensor_key
                    if sensor_key == 'cold_temp':
                        data_key = 'cold_temp_sensor'
                    
                    if data_key in self.all_graph_data and len(self.all_graph_data[data_key]) > 0:
                        values = list(self.all_graph_data[data_key])
                        color = sensor_colors[color_idx % len(sensor_colors)]
                        
                        # 센서 라벨 생성
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
    
    def on_closing(self):
        """프로그램 종료 처리"""
        self.monitoring_active = False
        if hasattr(self, 'test_generator') and self.test_generator.running:
            self.test_generator.stop()
        if self.comm.is_connected:
            self.comm.disconnect()
        self.root.destroy()
    
    # ========== 제어검토용 탭 전용 메서드들 ==========
    
    def create_graph_areas_control(self, parent):
        """제어검토용 탭의 그래프 영역 생성 (공통 컴포넌트)"""
        # 기존 그래프 영역과 동일하게 생성
        self.create_graph_areas(parent)
    
    def create_valve_area_control(self, parent):
        """제어검토용 탭의 밸브류 영역 생성 (공통 컴포넌트)"""
        # 기존 밸브 영역과 동일하게 생성
        self.create_valve_area(parent)
    
    def create_control_sections(self, parent):
        """제어검토용 탭의 제어 관련 섹션들 생성"""
        # 제어 관련 섹션들을 4개 영역으로 나누어 배치
        
        # 제어 상태 섹션
        control_status_frame = ttk.LabelFrame(parent, text="제어 상태", padding="2")
        control_status_frame.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S), padx=(0, 1))
        
        # 제어 상태 라벨들
        self.control_labels = {}
        
        # 시스템 모드
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
        
        # 제어 버튼들
        self.control_buttons = {}
        
        # 시스템 시작/정지
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
        
        # 설정값 입력들
        self.setpoint_entries = {}
        
        # 목표 온도
        temp_frame = ttk.Frame(setpoint_frame)
        temp_frame.grid(row=0, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(temp_frame, text="목표온도:", font=("Arial", 7), width=8).pack(side=tk.LEFT)
        self.setpoint_entries['target_temp'] = tk.Entry(temp_frame, font=("Arial", 7), width=6)
        self.setpoint_entries['target_temp'].pack(side=tk.LEFT, padx=(2, 0))
        ttk.Label(temp_frame, text="℃", font=("Arial", 7)).pack(side=tk.LEFT)
        
        # 알람 섹션
        alarm_frame = ttk.LabelFrame(parent, text="알람", padding="2")
        alarm_frame.grid(row=0, column=3, sticky=(tk.W, tk.E, tk.N, tk.S), padx=(1, 0))
        
        # 알람 상태들
        self.alarm_labels = {}
        
        # 온도 알람
        temp_alarm_frame = ttk.Frame(alarm_frame)
        temp_alarm_frame.grid(row=0, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(temp_alarm_frame, text="온도알람:", font=("Arial", 7), width=8).pack(side=tk.LEFT)
        self.alarm_labels['temp_alarm'] = tk.Label(temp_alarm_frame, text="정상", 
                                                  fg="white", bg="green", font=("Arial", 7, "bold"),
                                                  width=6, relief="raised")
        self.alarm_labels['temp_alarm'].pack(side=tk.LEFT, padx=(2, 0))
        
        # 컬럼 확장 설정
        for i in range(4):
            parent.columnconfigure(i, weight=1)
        parent.rowconfigure(0, weight=1)
    
    def create_shared_communication_area(self, parent):
        """모든 탭에서 공용으로 사용하는 통신부 영역 생성"""
        # 통신 설정 프레임 (하단 고정)
        comm_main_frame = ttk.LabelFrame(parent, text="통신 설정 (공용)", padding="3")
        comm_main_frame.pack(fill=tk.X, side=tk.BOTTOM)
        
        # 좌측: 통신 로그
        left_frame = ttk.Frame(comm_main_frame)
        left_frame.pack(side=tk.LEFT, fill=tk.BOTH, expand=True, padx=(0, 5))
        
        # 통신 로그 영역
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
        
        # 포트 새로고침 버튼
        self.refresh_ports_btn = ttk.Button(port_frame, text="⟳", 
                                           command=self.refresh_ports_with_log, width=3)
        self.refresh_ports_btn.pack(side=tk.LEFT)
        
        # 툴팁 추가 (간단한 방식)
        def show_tooltip(event):
            self.log_communication("포트 새로고침: 사용 가능한 시리얼 포트 목록을 업데이트합니다", "gray")
        
        self.refresh_ports_btn.bind("<Enter>", show_tooltip)
        
        # 통신속도
        baud_frame = ttk.Frame(right_frame)
        baud_frame.grid(row=2, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(baud_frame, text="속도:", font=("Arial", 8), width=6).pack(side=tk.LEFT)
        self.baudrate_var = tk.StringVar(value="9600")
        baud_combo = ttk.Combobox(baud_frame, textvariable=self.baudrate_var,
                                 values=["9600", "19200", "38400", "57600", "115200"],
                                 width=8, font=("Arial", 7))
        baud_combo.pack(side=tk.LEFT, padx=(2, 0))
        
        # 연결 버튼
        self.connect_btn = ttk.Button(right_frame, text="연결", 
                                     command=self.toggle_connection)
        self.connect_btn.grid(row=3, column=0, sticky=(tk.W, tk.E), pady=(3, 0))
        
        # 테스트 데이터 생성 버튼
        test_btn = ttk.Button(right_frame, text="테스트 데이터 생성", 
                             command=self.start_test_data)
        test_btn.grid(row=4, column=0, sticky=(tk.W, tk.E), pady=(3, 0))
        
        # 통신테스트 버튼
        self.comm_test_btn = ttk.Button(right_frame, text="통신테스트", 
                                       command=self.send_communication_test, state="disabled")
        self.comm_test_btn.grid(row=5, column=0, sticky=(tk.W, tk.E), pady=(3, 0))
        
        # 통신테스트 버튼 툴팁
        def show_comm_test_tooltip(event):
            self.log_communication("통신테스트: STX~ETX 프로토콜 패킷으로 센서 데이터를 한 번 전송합니다", "gray")
        
        self.comm_test_btn.bind("<Enter>", show_comm_test_tooltip)
        
        # 컬럼 확장
        right_frame.columnconfigure(0, weight=1)
        
        # 포트 목록 초기화
        self.refresh_ports()
    
    def toggle_graph1_item(self, item_key):
        """그래프1 항목 토글 (밸브, 냉각, 제빙, 드레인)"""
        if item_key in self.graph1_active_items:
            self.graph1_active_items.remove(item_key)
            # 선택 해제 시각적 표시
            self.update_item_visual(item_key, False, graph_num=1)
            self.log_communication(f"그래프1에서 {item_key} 제거됨", "orange")
        else:
            self.graph1_active_items.add(item_key)
            # 선택 시각적 표시
            self.update_item_visual(item_key, True, graph_num=1)
            self.log_communication(f"그래프1에 {item_key} 추가됨", "blue")
    
    def toggle_graph2_item(self, item_key):
        """그래프2 항목 토글 (센서류)"""
        if item_key in self.graph2_active_items:
            self.graph2_active_items.remove(item_key)
            # 선택 해제 시각적 표시
            self.update_item_visual(item_key, False, graph_num=2)
            self.log_communication(f"그래프2에서 {item_key} 제거됨", "orange")
        else:
            self.graph2_active_items.add(item_key)
            # 선택 시각적 표시
            self.update_item_visual(item_key, True, graph_num=2)
            self.log_communication(f"그래프2에 {item_key} 추가됨", "blue")
    
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
                # 선택됨: 테두리 강조
                label.config(relief="solid", bd=3)
            else:
                # 선택 해제: 원래 상태
                if item_key.startswith('valve_'):
                    label.config(relief="raised", bd=1)
                else:
                    label.config(relief="sunken", bd=1)
        except (KeyError, ValueError):
            pass
    
    def update_all_graph_data(self):
        """모든 그래프 데이터 업데이트 (실제 데이터 수신 시에만 호출)"""
        current_time = datetime.now()
        self.all_graph_data['time'].append(current_time)
        
        # 센서 데이터 업데이트
        for sensor_key in ['outdoor_temp1', 'outdoor_temp2', 'purified_temp', 
                          'cold_temp_sensor', 'hot_inlet_temp', 'hot_internal_temp', 'hot_outlet_temp']:
            if sensor_key == 'cold_temp_sensor':
                value = self.sensor_data.get('cold_temp', 0)
            else:
                value = self.sensor_data.get(sensor_key, 0)
            self.all_graph_data[sensor_key].append(float(value))
        
        # 밸브 데이터 업데이트 (0 또는 1)
        for i in range(1, 6):
            valve_state = 1 if self.nos_valve_states.get(i, False) else 0
            self.all_graph_data[f'nos_valve_{i}'].append(valve_state)
        
        for i in range(1, 16):
            valve_state = 1 if self.feed_valve_states.get(i, False) else 0
            self.all_graph_data[f'feed_valve_{i}'].append(valve_state)
        
        # 냉각 시스템 데이터
        cooling_op = 1 if self.cooling_data.get('operation_state') == 'GOING' else 0
        self.all_graph_data['cooling_operation'].append(cooling_op)
        self.all_graph_data['cooling_on_temp'].append(float(self.cooling_data.get('on_temp', 0)))
        self.all_graph_data['cooling_off_temp'].append(float(self.cooling_data.get('off_temp', 0)))
        
        # 제빙 시스템 데이터
        self.all_graph_data['icemaking_time'].append(float(self.icemaking_data.get('icemaking_time', 0)))
        self.all_graph_data['icemaking_capacity'].append(float(self.icemaking_data.get('water_capacity', 0)))
        
        # 드레인 데이터
        tank_level = 1 if self.drain_tank_data.get('high_level') == '감지' else 0
        pump_state = 1 if self.drain_pump_data.get('operation_state') == 'ON' else 0
        self.all_graph_data['drain_tank_level'].append(tank_level)
        self.all_graph_data['drain_pump_state'].append(pump_state)
    
    def show_connection_error_dialog(self, error_message):
        """연결 오류 시 상세한 해결 방법을 보여주는 대화상자"""
        title = "시리얼 포트 연결 오류"
        
        # 오류 유형별 맞춤형 메시지 생성
        if "권한" in error_message or "Permission" in error_message:
            detailed_message = f"{error_message}\n\n💡 권한 문제 해결 방법:\n"
            detailed_message += "1. 관리자 권한으로 프로그램을 실행하세요\n"
            detailed_message += "2. 다른 시리얼 통신 프로그램을 모두 종료하세요\n"
            detailed_message += "3. Arduino IDE, PuTTY, Tera Term 등을 확인하세요\n"
            detailed_message += "4. 장치 관리자에서 포트를 새로고침하세요"
        elif "사용 중" in error_message or "Access is denied" in error_message:
            detailed_message = f"{error_message}\n\n💡 포트 사용 중 해결 방법:\n"
            detailed_message += "1. 작업 관리자에서 다른 시리얼 프로그램 종료\n"
            detailed_message += "2. USB 케이블 재연결\n"
            detailed_message += "3. 장치 관리자에서 포트 비활성화 후 재활성화\n"
            detailed_message += "4. 시스템 재시작"
        elif "찾을 수 없음" in error_message or "not found" in error_message:
            detailed_message = f"{error_message}\n\n💡 포트 없음 해결 방법:\n"
            detailed_message += "1. USB 장치가 올바르게 연결되었는지 확인\n"
            detailed_message += "2. USB 드라이버가 설치되었는지 확인\n"
            detailed_message += "3. 장치 관리자에서 포트 확인\n"
            detailed_message += "4. 포트 새로고침 버튼(⟳) 클릭"
        else:
            detailed_message = f"{error_message}\n\n💡 일반적인 해결 방법:\n"
            detailed_message += "1. USB 케이블과 연결 상태 확인\n"
            detailed_message += "2. 다른 USB 포트에 연결 시도\n"
            detailed_message += "3. 장치 드라이버 재설치\n"
            detailed_message += "4. 포트 새로고침 후 다시 시도"
        
        messagebox.showerror(title, detailed_message)
    
    def send_communication_test(self):
        """통신테스트 데이터 전송"""
        if not self.comm.is_connected:
            messagebox.showwarning("경고", "시리얼 포트가 연결되지 않았습니다.")
            return
        
        # 메서드 존재 여부 확인 및 대안 실행
        if not hasattr(self.comm, 'send_test_data'):
            # 대안: 직접 테스트 데이터 생성 및 전송
            self.log_communication("send_test_data 메서드가 없어 대안 방법으로 실행합니다", "orange")
            success, message = self.send_test_data_alternative()
            if success:
                self.log_communication(f"통신테스트 성공 (대안): {message}", "green")
                self.comm_test_btn.config(state="disabled")
                self.root.after(500, lambda: self.comm_test_btn.config(state="normal"))
            else:
                self.log_communication(f"통신테스트 실패: {message}", "red")
                messagebox.showerror("통신테스트 오류", message)
            return
        
        # 테스트 데이터 전송
        try:
            success, message = self.comm.send_test_data()
            
            if success:
                self.log_communication(f"통신테스트 성공: {message}", "green")
                # 버튼을 잠시 비활성화하여 중복 클릭 방지
                self.comm_test_btn.config(state="disabled")
                self.root.after(500, lambda: self.comm_test_btn.config(state="normal"))  # 0.5초 후 재활성화
            else:
                self.log_communication(f"통신테스트 실패: {message}", "red")
                messagebox.showerror("통신테스트 오류", message)
                
        except Exception as e:
            error_msg = f"통신테스트 실행 중 오류 발생: {str(e)}"
            self.log_communication(error_msg, "red")
            messagebox.showerror("통신테스트 오류", error_msg)
    
    def send_test_data_alternative(self):
        """대안적인 테스트 데이터 전송 방법"""
        try:
            import random
            
            # 테스트 데이터 생성 (communication.py의 generate_test_data와 동일한 로직)
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
            
            data_bytes = data_str.encode('utf-8')
            
            # 프로토콜 패킷 생성 (STX + 데이터 + 패딩 + ETX)
            STX = 0x02
            ETX = 0x03
            PACKET_SIZE = 20
            
            if len(data_bytes) > PACKET_SIZE - 2:
                data_bytes = data_bytes[:PACKET_SIZE - 2]  # 크기 제한
            
            packet = bytearray()
            packet.append(STX)
            packet.extend(data_bytes)
            
            # 패딩 추가
            padding_size = PACKET_SIZE - len(packet) - 1
            packet.extend([0x00] * padding_size)
            packet.append(ETX)
            
            # 전송 (바이트 데이터 처리 개선)
            packet_bytes = bytes(packet)
            
            if hasattr(self.comm, 'send_data'):
                # send_data 메서드 사용 (이제 바이트 데이터 지원)
                success, msg = self.comm.send_data(packet_bytes)
            elif hasattr(self.comm, 'send_queue'):
                # 송신 큐에 직접 추가
                self.comm.send_queue.put(packet_bytes)
                success, msg = True, f"데이터 전송됨 ({len(packet_bytes)}바이트)"
            elif hasattr(self.comm, 'serial_connection') and self.comm.serial_connection:
                # 시리얼 연결에 직접 쓰기
                self.comm.serial_connection.write(packet_bytes)
                success, msg = True, f"데이터 직접 전송됨 ({len(packet_bytes)}바이트)"
            else:
                success, msg = False, "전송 방법을 찾을 수 없습니다"
            
            # 데이터 내용 로그
            data_preview = data_str[:50] + "..." if len(data_str) > 50 else data_str
            self.log_communication(f"전송 데이터: {data_preview}", "blue")
            
            # HEX 패킷 로그
            hex_packet = " ".join([f"{b:02X}" for b in packet_bytes])
            self.log_communication(f"HEX 패킷: {hex_packet}", "gray")
            
            return success, msg
            
        except Exception as e:
            return False, f"대안 전송 오류: {str(e)}"
    
    def debug_communication_class(self):
        """통신 클래스 디버깅 정보 출력"""
        try:
            class_name = self.comm.__class__.__name__
            module_name = self.comm.__class__.__module__
            
            # 사용 가능한 메서드 확인
            methods = [method for method in dir(self.comm) if not method.startswith('_')]
            
            print(f"[DEBUG] 통신 클래스: {class_name}")
            print(f"[DEBUG] 모듈: {module_name}")
            print(f"[DEBUG] 사용 가능한 메서드들: {', '.join(methods)}")
            
            # send_test_data 메서드 존재 여부 확인
            has_send_test_data = hasattr(self.comm, 'send_test_data')
            print(f"[DEBUG] send_test_data 메서드 존재: {has_send_test_data}")
            
            # GUI가 준비되면 로그에도 출력
            if hasattr(self, 'log_communication'):
                self.root.after(1000, lambda: self.log_communication(
                    f"통신클래스: {class_name}, send_test_data 메서드: {'있음' if has_send_test_data else '없음'}", "purple"))
                    
        except Exception as e:
            print(f"[DEBUG] 디버깅 정보 출력 오류: {e}")
    
    def on_tab_changed(self, event):
        """탭 변경 시 호출되는 이벤트 핸들러"""
        current_tab = self.notebook.index(self.notebook.select())
        tab_names = ["냉동검토용", "제어검토용"]
        if current_tab < len(tab_names):
            self.log_communication(f"탭 전환: {tab_names[current_tab]} 탭으로 이동", "purple")


def main():
    root = tk.Tk()
    app = MainGUI(root)
    
    # 프로그램 종료 시 정리 작업
    root.protocol("WM_DELETE_WINDOW", app.on_closing)
    
    root.mainloop()


if __name__ == "__main__":
    main()
