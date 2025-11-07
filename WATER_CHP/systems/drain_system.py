"""
드레인 시스템 모듈
드레인탱크와 드레인펌프 시스템의 GUI 위젯 생성, 데이터 업데이트를 담당합니다.
"""
import tkinter as tk
from tkinter import ttk


class DrainTankSystem:
    """드레인탱크 시스템 클래스"""
    
    def __init__(self, root, comm, log_callback):
        """
        Args:
            root: Tkinter 루트 윈도우
            comm: SerialCommunication 객체
            log_callback: 로그 출력 콜백 함수
        """
        self.root = root
        self.comm = comm
        self.log_communication = log_callback
        
        # 데이터 저장소
        self.data = {
            'low_level': '미감지',
            'high_level': '미감지',
            'water_level_state': '비어있음'
        }
        
        # GUI 위젯 참조
        self.labels = {}
    
    def create_widgets(self, parent):
        """드레인탱크 섹션 GUI 위젯 생성"""
        drain_tank_frame = ttk.LabelFrame(parent, text="드레인탱크", padding="2")
        drain_tank_frame.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S), padx=(0, 2))
        
        # 저수위
        low_level_frame = ttk.Frame(drain_tank_frame)
        low_level_frame.grid(row=0, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(low_level_frame, text="저수위:", font=("Arial", 9), width=8).pack(side=tk.LEFT)
        self.labels['low_level'] = tk.Label(low_level_frame, text="미감지", 
                                            fg="white", bg="gray", font=("Arial", 8, "bold"),
                                            width=8, relief="raised")
        self.labels['low_level'].pack(side=tk.LEFT, padx=(2, 0))
        
        # 만수위
        high_level_frame = ttk.Frame(drain_tank_frame)
        high_level_frame.grid(row=1, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(high_level_frame, text="만수위:", font=("Arial", 9), width=8).pack(side=tk.LEFT)
        self.labels['high_level'] = tk.Label(high_level_frame, text="미감지", 
                                           fg="white", bg="gray", font=("Arial", 8, "bold"),
                                           width=8, relief="raised")
        self.labels['high_level'].pack(side=tk.LEFT, padx=(2, 0))
        
        # 수위상태
        water_state_frame = ttk.Frame(drain_tank_frame)
        water_state_frame.grid(row=2, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(water_state_frame, text="수위상태:", font=("Arial", 9), width=8).pack(side=tk.LEFT)
        self.labels['water_level_state'] = tk.Label(water_state_frame, text="비어있음", 
                                                   fg="white", bg="blue", font=("Arial", 8, "bold"),
                                                   width=8, relief="raised")
        self.labels['water_level_state'].pack(side=tk.LEFT, padx=(2, 0))
        
        drain_tank_frame.columnconfigure(0, weight=1)
        
        return drain_tank_frame
    
    def update_data(self, new_data):
        """데이터 업데이트"""
        self.data.update(new_data)
        self._update_gui()
    
    def _update_gui(self):
        """GUI 업데이트"""
        for key, value in self.data.items():
            if key in self.labels:
                label = self.labels[key]
                if key in ['low_level', 'high_level']:
                    if value == '감지':
                        label.config(text="감지", bg="orange")
                    else:
                        label.config(text="미감지", bg="gray")
                elif key == 'water_level_state':
                    colors = {'만수위': 'red', '저수위': 'orange', '비어있음': 'blue'}
                    color = colors.get(value, 'gray')
                    label.config(text=value, bg=color)
    
    def get_data(self):
        """현재 데이터 반환"""
        return self.data.copy()


class DrainPumpSystem:
    """드레인펌프 시스템 클래스"""
    
    def __init__(self, root, comm, log_callback):
        """
        Args:
            root: Tkinter 루트 윈도우
            comm: SerialCommunication 객체
            log_callback: 로그 출력 콜백 함수
        """
        self.root = root
        self.comm = comm
        self.log_communication = log_callback
        
        # 데이터 저장소
        self.data = {
            'operation_state': 'OFF'
        }
        
        # GUI 위젯 참조
        self.labels = {}
    
    def create_widgets(self, parent):
        """드레인펌프 섹션 GUI 위젯 생성"""
        drain_pump_frame = ttk.LabelFrame(parent, text="드레인 펌프", padding="2")
        drain_pump_frame.grid(row=0, column=1, sticky=(tk.W, tk.E, tk.N, tk.S), padx=(1, 0))
        
        state_frame = ttk.Frame(drain_pump_frame)
        state_frame.grid(row=0, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(state_frame, text="운전 상태:", font=("Arial", 7), width=8).pack(side=tk.LEFT)
        self.labels['operation_state'] = tk.Label(state_frame, text="OFF", 
                                                  fg="white", bg="red", font=("Arial", 7, "bold"),
                                                  width=6, relief="raised")
        self.labels['operation_state'].pack(side=tk.LEFT, padx=(2, 0))
        
        drain_pump_frame.columnconfigure(0, weight=1)
        
        return drain_pump_frame
    
    def update_data(self, new_data):
        """데이터 업데이트"""
        self.data.update(new_data)
        self._update_gui()
    
    def _update_gui(self):
        """GUI 업데이트"""
        if 'operation_state' in self.labels:
            if self.data['operation_state'] == 'ON':
                self.labels['operation_state'].config(text="ON", bg="green")
            else:
                self.labels['operation_state'].config(text="OFF", bg="gray")
    
    def get_data(self):
        """현재 데이터 반환"""
        return self.data.copy()

