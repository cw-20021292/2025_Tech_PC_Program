"""
센서 시스템 모듈
센서 시스템의 GUI 위젯 생성, 데이터 업데이트를 담당합니다.
"""
import tkinter as tk
from tkinter import ttk


class SensorSystem:
    """센서 시스템 클래스"""
    
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
            'outdoor_temp1': 0,
            'outdoor_temp2': 0,
            'purified_temp': 0,
            'cold_temp': 0,
            'hot_inlet_temp': 0,
            'hot_internal_temp': 0,
            'hot_outlet_temp': 0
        }
        
        # GUI 위젯 참조
        self.labels = {}
    
    def create_widgets(self, parent):
        """센서 섹션 GUI 위젯 생성"""
        sensor_frame = ttk.LabelFrame(parent, text="센서 데이터", padding="2")
        sensor_frame.grid(row=0, column=1, sticky=(tk.W, tk.E, tk.N, tk.S), padx=(1, 0))
        
        sensors = [
            ('outdoor_temp1', '외부온도1', '℃'),
            ('outdoor_temp2', '외부온도2', '℃'),
            ('purified_temp', '정수온도', '℃'),
            ('cold_temp', '냉수온도', '℃'),
            ('hot_inlet_temp', '온수입구', '℃'),
            ('hot_internal_temp', '온수내부', '℃'),
            ('hot_outlet_temp', '온수출구', '℃'),
        ]
        
        for idx, (key, label_text, unit) in enumerate(sensors):
            sensor_item_frame = ttk.Frame(sensor_frame)
            sensor_item_frame.grid(row=idx, column=0, sticky=(tk.W, tk.E), pady=1)
            
            ttk.Label(sensor_item_frame, text=f"{label_text}:", font=("Arial", 8), width=10).pack(side=tk.LEFT)
            label = tk.Label(sensor_item_frame, text="0.0", 
                            font=("Arial", 8), bg="white", relief="sunken", width=8)
            label.pack(side=tk.LEFT, padx=(2, 0))
            ttk.Label(sensor_item_frame, text=unit, font=("Arial", 8)).pack(side=tk.LEFT)
            
            self.labels[key] = label
        
        sensor_frame.columnconfigure(0, weight=1)
        
        return sensor_frame
    
    def update_data(self, new_data):
        """데이터 업데이트"""
        self.data.update(new_data)
        self._update_gui()
    
    def _update_gui(self):
        """GUI 업데이트"""
        for sensor_key, value in self.data.items():
            if sensor_key in self.labels:
                self.labels[sensor_key].config(text=f"{value:.1f}")
    
    def get_data(self):
        """현재 데이터 반환"""
        return self.data.copy()

