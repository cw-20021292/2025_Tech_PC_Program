"""
그래프 시스템 모듈
그래프 시스템의 GUI 위젯 생성, 데이터 업데이트, 그래프 그리기를 담당합니다.
"""
import tkinter as tk
from tkinter import ttk
from collections import deque
import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure

plt.rcParams['font.family'] = ['Malgun Gothic', 'DejaVu Sans', 'sans-serif']
plt.rcParams['axes.unicode_minus'] = False


class GraphSystem:
    """그래프 시스템 클래스"""
    
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
        
        # 그래프 데이터
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
        
        # 그래프 토글 상태
        self.graph1_active_items = set()
        self.graph2_active_items = set()
        
        # 그래프 생성 카운트
        self._graph_creation_count = 0
        
        # 그래프 위젯 참조
        self.fig1_freezing = None
        self.fig2_freezing = None
        self.fig1_control = None
        self.fig2_control = None
        self.temp_ax_freezing = None
        self.pressure_ax_freezing = None
        self.temp_ax_control = None
        self.pressure_ax_control = None
        self.canvas1_freezing = None
        self.canvas2_freezing = None
        self.canvas1_control = None
        self.canvas2_control = None
    
    def create_widgets(self, parent):
        """그래프 영역 GUI 위젯 생성"""
        graph_container = ttk.Frame(parent)
        graph_container.grid(row=0, column=4, sticky=(tk.W, tk.E, tk.N, tk.S), padx=(2, 0))
        
        # 그래프 1
        graph1_frame = ttk.LabelFrame(graph_container, text="그래프 1", padding="3")
        graph1_frame.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S), pady=(0, 2))
        
        # 그래프 2
        graph2_frame = ttk.LabelFrame(graph_container, text="그래프 2", padding="3")
        graph2_frame.grid(row=1, column=0, sticky=(tk.W, tk.E, tk.N, tk.S), pady=(2, 0))
        
        # 그래프 생성 카운트
        self._graph_creation_count += 1
        is_freezing_tab = self._graph_creation_count == 1
        
        try:
            if is_freezing_tab:
                # 냉동검토용 탭 - 그래프 1
                self.fig1_freezing = Figure(figsize=(3.2, 2.0), dpi=80)
                self.temp_ax_freezing = self.fig1_freezing.add_subplot(1, 1, 1)
                self.temp_ax_freezing.set_title("센서 데이터", fontsize=8)
                self.temp_ax_freezing.set_ylabel("ON/OFF", fontsize=7)
                self.temp_ax_freezing.grid(True, alpha=0.3)
                self.fig1_freezing.tight_layout()
                
                self.canvas1_freezing = FigureCanvasTkAgg(self.fig1_freezing, graph1_frame)
                self.canvas1_freezing.draw()
                canvas1_widget = self.canvas1_freezing.get_tk_widget()
                canvas1_widget.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))
                
                # 냉동검토용 탭 - 그래프 2
                self.fig2_freezing = Figure(figsize=(3.2, 2.0), dpi=80)
                self.pressure_ax_freezing = self.fig2_freezing.add_subplot(1, 1, 1)
                self.pressure_ax_freezing.set_title("Sensors", fontsize=8)
                self.pressure_ax_freezing.set_ylabel("℃", fontsize=7)
                self.pressure_ax_freezing.set_xlabel("Time", fontsize=7)
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
                self.temp_ax_control.set_title("부하 출력 그래프", fontsize=8,)
                self.temp_ax_control.set_ylabel("Temperature (°C)", fontsize=7,)
                self.temp_ax_control.grid(True, alpha=0.3)
                self.fig1_control.tight_layout()
                
                self.canvas1_control = FigureCanvasTkAgg(self.fig1_control, graph1_frame)
                self.canvas1_control.draw()
                canvas1_widget = self.canvas1_control.get_tk_widget()
                canvas1_widget.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))
                
                # 제어검토용 탭 - 그래프 2
                self.fig2_control = Figure(figsize=(3.2, 2.0), dpi=80)
                self.pressure_ax_control = self.fig2_control.add_subplot(1, 1, 1)
                self.pressure_ax_control.set_title("Sensors", fontsize=8)
                self.pressure_ax_control.set_ylabel("Value", fontsize=7)
                self.pressure_ax_control.set_xlabel("Time", fontsize=7)
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
        
        return graph_container
    
    def update_all_graph_data(self, sensor_data, valve_data, cooling_data, 
                             icemaking_data, drain_tank_data, drain_pump_data):
        """그래프 데이터 업데이트"""
        from datetime import datetime
        current_time = datetime.now().strftime("%H:%M:%S")
        self.all_graph_data['time'].append(current_time)
        
        # 센서 데이터 추가
        for sensor_key, value in sensor_data.items():
            if sensor_key in self.all_graph_data:
                self.all_graph_data[sensor_key].append(float(value))
        
        # 냉수온도 센서 데이터 추가
        if 'cold_temp' in sensor_data:
            self.all_graph_data['cold_temp_sensor'].append(float(sensor_data.get('cold_temp', 0)))
        
        # 밸브 데이터 추가
        if valve_data:
            nos_states = valve_data.get('nos_valve_states', {})
            feed_states = valve_data.get('feed_valve_states', {})
            
            for i in range(1, 6):
                valve_state = 1 if nos_states.get(i, False) else 0
                self.all_graph_data[f'nos_valve_{i}'].append(valve_state)
            
            for i in range(1, 16):
                valve_state = 1 if feed_states.get(i, False) else 0
                self.all_graph_data[f'feed_valve_{i}'].append(valve_state)
        
        # 냉각 데이터 추가
        if cooling_data:
            cooling_op = 1 if cooling_data.get('operation_state') == 'GOING' else 0
            self.all_graph_data['cooling_operation'].append(cooling_op)
            self.all_graph_data['cooling_on_temp'].append(float(cooling_data.get('on_temp', 0)))
            self.all_graph_data['cooling_off_temp'].append(float(cooling_data.get('off_temp', 0)))
        
        # 제빙 데이터 추가
        if icemaking_data:
            self.all_graph_data['icemaking_time'].append(float(icemaking_data.get('icemaking_time', 0)))
            self.all_graph_data['icemaking_capacity'].append(float(icemaking_data.get('water_capacity', 0)))
        
        # 드레인탱크 데이터 추가
        if drain_tank_data:
            tank_level = 0 if drain_tank_data.get('water_level_state') == '비어있음' else \
                        1 if drain_tank_data.get('water_level_state') == '저수위' else 2
            self.all_graph_data['drain_tank_level'].append(tank_level)
        
        # 드레인펌프 데이터 추가
        if drain_pump_data:
            pump_state = 1 if drain_pump_data.get('operation_state') == 'ON' else 0
            self.all_graph_data['drain_pump_state'].append(pump_state)
    
    def update_graphs(self):
        """선택된 항목들만 그래프에 표시"""
        if len(self.all_graph_data['time']) < 2:
            return
        
        try:
            times = list(self.all_graph_data['time'])
            
            # 냉동검토용 탭의 그래프 1 업데이트
            if self.temp_ax_freezing:
                self.temp_ax_freezing.clear()
                self.temp_ax_freezing.set_title("Selected Items (Graph 1 - Output)", fontsize=8, fontfamily='DejaVu Sans')
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
            
            # 제어검토용 탭의 그래프 1 업데이트
            if self.temp_ax_control:
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
            if self.pressure_ax_freezing:
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
            if self.pressure_ax_control:
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
    
    def get_data(self):
        """현재 그래프 데이터 반환"""
        return {
            'all_graph_data': {k: list(v) for k, v in self.all_graph_data.items()},
            'graph1_active_items': self.graph1_active_items.copy(),
            'graph2_active_items': self.graph2_active_items.copy()
        }
    
    def set_active_items(self, graph1_items=None, graph2_items=None):
        """그래프에 표시할 항목 설정"""
        if graph1_items is not None:
            self.graph1_active_items = graph1_items
        if graph2_items is not None:
            self.graph2_active_items = graph2_items

