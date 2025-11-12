"""
공조 시스템 모듈
공조 시스템의 GUI 위젯 생성, 데이터 업데이트, 제어 기능을 담당합니다.
"""
import tkinter as tk
from tkinter import ttk, messagebox


class HVACSystem:
    """공조 시스템 클래스"""
    
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
            'refrigerant_valve_state_1': '핫가스',
            'refrigerant_valve_state_2': '핫가스',
            'compressor_state': '미동작',
            'error_code': 0,
            'dc_fan1': 'OFF',
            'dc_fan2': 'OFF'
        }
        
        # 입력 모드 상태
        self.edit_mode = False
        self.temp_data = {
            'refrigerant_valve_state_1': '핫가스',
            'refrigerant_valve_state_2': '핫가스',
            'compressor_state': '미동작',
            'dc_fan1': 'OFF',
            'dc_fan2': 'OFF'
        }
        
        # GUI 위젯 참조
        self.labels = {}
        self.send_btn = None
    
    def create_widgets(self, parent):
        """공조시스템 섹션 GUI 위젯 생성"""
        hvac_frame = ttk.LabelFrame(parent, text="공조시스템", padding="2")
        hvac_frame.grid(row=0, column=1, sticky=(tk.W, tk.E, tk.N, tk.S), padx=1)
        
        # 냉매전환밸브 서브프레임
        valve_subframe = ttk.LabelFrame(hvac_frame, text="냉매전환밸브", padding="3")
        valve_subframe.grid(row=0, column=0, columnspan=2, sticky=(tk.W, tk.E), pady=(0, 5))
        
        # 상태
        state_frame = ttk.Frame(valve_subframe)
        state_frame.grid(row=0, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(state_frame, text="1번 상태:", font=("Arial", 8)).pack(side=tk.LEFT)
        self.labels['refrigerant_valve_state_1'] = tk.Label(state_frame, text="핫가스", 
                                                          fg="white", bg="red", font=("Arial", 8, "bold"),
                                                          width=8, relief="raised")
        self.labels['refrigerant_valve_state_1'].pack(side=tk.RIGHT)
        
        # 목표 (클릭 가능)
        target_frame = ttk.Frame(valve_subframe)
        target_frame.grid(row=1, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(target_frame, text="2번 상태:", font=("Arial", 8)).pack(side=tk.LEFT)
        self.labels['refrigerant_valve_state_2'] = tk.Label(target_frame, text="핫가스", 
                                                          fg="white", bg="orange", font=("Arial", 8, "bold"),
                                                          width=8, relief="raised", cursor="hand2")
        self.labels['refrigerant_valve_state_2'].pack(side=tk.RIGHT)
        
        # 압축기 서브프레임
        comp_subframe = ttk.LabelFrame(hvac_frame, text="압축기", padding="3")
        comp_subframe.grid(row=1, column=0, columnspan=2, sticky=(tk.W, tk.E), pady=(0, 5))
        
        # 상태 (클릭 가능)
        comp_state_frame = ttk.Frame(comp_subframe)
        comp_state_frame.grid(row=0, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(comp_state_frame, text="상태:", font=("Arial", 8)).pack(side=tk.LEFT)
        self.labels['compressor_state'] = tk.Label(comp_state_frame, text="미동작", 
                                                   fg="white", bg="gray", font=("Arial", 8, "bold"),
                                                   width=8, relief="raised", cursor="hand2")
        self.labels['compressor_state'].pack(side=tk.RIGHT)
        self.labels['compressor_state'].bind("<Button-1>", self._toggle_compressor_state)
        
        # 에러코드
        error_frame = ttk.Frame(comp_subframe)
        error_frame.grid(row=1, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(error_frame, text="에러코드:", font=("Arial", 8)).pack(side=tk.LEFT)
        self.labels['error_code'] = tk.Label(error_frame, text="0", 
                                            font=("Arial", 8), bg="white", relief="sunken")
        self.labels['error_code'].pack(side=tk.RIGHT)
        
        # DC FAN 1 (클릭 가능)
        fan1_frame = ttk.Frame(comp_subframe)
        fan1_frame.grid(row=2, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(fan1_frame, text="압축기 팬:", font=("Arial", 8)).pack(side=tk.LEFT)
        self.labels['dc_fan1'] = tk.Label(fan1_frame, text="OFF", 
                                         fg="white", bg="gray", font=("Arial", 8, "bold"),
                                         width=5, relief="raised", cursor="hand2")
        self.labels['dc_fan1'].pack(side=tk.RIGHT)
        self.labels['dc_fan1'].bind("<Button-1>", self._toggle_dc_fan1)
        
        # DC FAN 2 (클릭 가능)
        fan2_frame = ttk.Frame(comp_subframe)
        fan2_frame.grid(row=3, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(fan2_frame, text="얼음탱크 팬:", font=("Arial", 8)).pack(side=tk.LEFT)
        self.labels['dc_fan2'] = tk.Label(fan2_frame, text="OFF", 
                                         fg="white", bg="gray", font=("Arial", 8, "bold"),
                                         width=5, relief="raised", cursor="hand2")
        self.labels['dc_fan2'].pack(side=tk.RIGHT)
        self.labels['dc_fan2'].bind("<Button-1>", self._toggle_dc_fan2)
        
        # CMD 0xB0 전송 버튼
        send_btn_frame = ttk.Frame(hvac_frame)
        send_btn_frame.grid(row=2, column=0, columnspan=2, sticky=(tk.W, tk.E), pady=(5, 1))
        self.send_btn = ttk.Button(send_btn_frame, text="입력모드",
                                   command=self.send_control, state="disabled")
        self.send_btn.pack(fill=tk.X)
        
        valve_subframe.columnconfigure(0, weight=1)
        comp_subframe.columnconfigure(0, weight=1)
        hvac_frame.columnconfigure(0, weight=1)
        
        return hvac_frame
    
    def _validate_number(self, value):
        """숫자 입력 검증"""
        if value == "":
            return True
        try:
            int(value)
            return True
        except ValueError:
            return False
    
    def _toggle_compressor_state(self, event):
        """압축기 상태 토글"""
        if not self.comm.is_connected:
            messagebox.showwarning("경고", "시리얼 포트가 연결되지 않았습니다!")
            return
        
        if self.edit_mode:
            current_value = self.temp_data['compressor_state']
        else:
            current_value = self.data['compressor_state']
        
        next_value = '미동작' if current_value == '동작중' else '동작중'
        
        if next_value == '동작중':
            self.labels['compressor_state'].config(text="동작중", bg="green")
        else:
            self.labels['compressor_state'].config(text="미동작", bg="gray")
        
        if self.edit_mode:
            self.temp_data['compressor_state'] = next_value
            self.log_communication(f"압축기 상태 변경: {next_value} (입력 모드)", "purple")
    
    def _toggle_dc_fan1(self, event):
        """압축기 팬 토글"""
        if not self.edit_mode:
            return
        
        current_value = self.temp_data['dc_fan1']
        next_value = 'OFF' if current_value == 'ON' else 'ON'
        self.temp_data['dc_fan1'] = next_value
        
        if next_value == 'ON':
            self.labels['dc_fan1'].config(text="ON", bg="green")
        else:
            self.labels['dc_fan1'].config(text="OFF", bg="gray")
    
    def _toggle_dc_fan2(self, event):
        """얼음탱크 팬 토글"""
        if not self.edit_mode:
            return
        
        current_value = self.temp_data['dc_fan2']
        next_value = 'OFF' if current_value == 'ON' else 'ON'
        self.temp_data['dc_fan2'] = next_value
        
        if next_value == 'ON':
            self.labels['dc_fan2'].config(text="ON", bg="green")
        else:
            self.labels['dc_fan2'].config(text="OFF", bg="gray")
    
    def send_control(self):
        """공조 제어 CMD 0xB0 전송"""
        if not self.comm.is_connected:
            messagebox.showwarning("경고", "시리얼 포트가 연결되지 않았습니다.")
            return
        
        if not self.edit_mode:
            # 입력 모드 활성화
            self.edit_mode = True
            
            # 현재 값을 임시 저장소에 복사
            self.temp_data['refrigerant_valve_state_1'] = self.data['refrigerant_valve_state_1']
            self.temp_data['refrigerant_valve_state_2'] = self.data['refrigerant_valve_state_2']
            self.temp_data['compressor_state'] = self.data['compressor_state']
            self.temp_data['dc_fan1'] = self.data['dc_fan1']
            self.temp_data['dc_fan2'] = self.data['dc_fan2']
            
            # UI 업데이트
            colors = {'냉각': 'green', '제빙': 'blue', '핫가스': 'red'}
            self.labels['refrigerant_valve_state_1'].config(
                text=self.temp_data['refrigerant_valve_state_1'],
                bg=colors.get(self.temp_data['refrigerant_valve_state_1'], 'orange')
            )
            
            self.labels['refrigerant_valve_state_2'].config(
                text=self.temp_data['refrigerant_valve_state_2'],
                bg=colors.get(self.temp_data['refrigerant_valve_state_2'], 'orange')
            )
            
            if self.temp_data['compressor_state'] == '동작중':
                self.labels['compressor_state'].config(text="동작중", bg="green")
            else:
                self.labels['compressor_state'].config(text="미동작", bg="gray")
            
            if self.temp_data['dc_fan1'] == 'ON':
                self.labels['dc_fan1'].config(text="ON", bg="green")
            else:
                self.labels['dc_fan1'].config(text="OFF", bg="gray")
            
            if self.temp_data['dc_fan2'] == 'ON':
                self.labels['dc_fan2'].config(text="ON", bg="green")
            else:
                self.labels['dc_fan2'].config(text="OFF", bg="gray")
            
            # 버튼 텍스트 변경
            self.send_btn.config(text="설정 완료 (CMD 0xB0 전송)")
            
            self.log_communication("공조 설정 입력 모드 활성화", "purple")
        
        else:
            # 입력 모드 비활성화 및 데이터 전송
            try:
                # 임시 저장소의 값을 실제 데이터로 복사
                self.data['refrigerant_valve_state_1'] = self.temp_data['refrigerant_valve_state_1']
                self.data['refrigerant_valve_state_2'] = self.temp_data['refrigerant_valve_state_2']
                self.data['compressor_state'] = self.temp_data['compressor_state']
                self.data['dc_fan1'] = self.temp_data['dc_fan1']
                self.data['dc_fan2'] = self.temp_data['dc_fan2']
                
                # DATA FIELD 구성 (4바이트) - RPS 제거
                data_field = bytearray(4)
                valve_map = {'냉각': 0, '제빙': 1, '핫가스': 2}
                data_field[0] = valve_map[self.data['refrigerant_valve_state_1']]
                data_field[1] = valve_map[self.data['refrigerant_valve_state_2']]
                data_field[1] = 1 if self.data['compressor_state'] == '동작중' else 0
                data_field[2] = 1 if self.data['dc_fan1'] == 'ON' else 0
                data_field[3] = 1 if self.data['dc_fan2'] == 'ON' else 0
                
                # 로그 출력
                hex_data = " ".join([f"{b:02X}" for b in data_field])
                self.log_communication(f"[공조 제어] CMD 0xB0 전송", "blue")
                self.log_communication(f"  냉매전환밸브 1번 상태: {self.data['refrigerant_valve_state_1']} ({data_field[0]})", "gray")
                self.log_communication(f"  냉매전환밸브 2번 상태: {self.data['refrigerant_valve_state_2']} ({data_field[1]})", "gray")
                self.log_communication(f"  압축기 상태: {self.data['compressor_state']} ({data_field[1]})", "gray")
                self.log_communication(f"  DATA FIELD (HEX): {hex_data}", "gray")
                
                # CMD 0xB0 패킷 전송
                success, message = self.comm.send_packet(0xB0, bytes(data_field))
                
                if success:
                    self.log_communication(f"  전송 성공 (CMD 0xB0, 4바이트)", "green")
                    
                    # 입력 모드 비활성화
                    self.edit_mode = False
                    self.send_btn.config(text="입력모드")
                    
                else:
                    self.log_communication(f"  전송 실패: {message}", "red")
                    
            except ValueError:
                messagebox.showerror("오류", "올바른 숫자를 입력해주세요.")
            except Exception as e:
                self.log_communication(f"공조 제어 오류: {str(e)}", "red")
    
    def update_data(self, new_data):
        """데이터 업데이트"""
        self.data.update(new_data)
        self._update_gui()
    
    def _update_gui(self):
        """GUI 업데이트"""
        # 냉매전환밸브 상태
        if 'refrigerant_valve_state_1' in self.labels:
            colors = {'핫가스': 'red', '제빙': 'blue', '냉각': 'green'}
            color = colors.get(self.data['refrigerant_valve_state_1'], 'gray')
            self.labels['refrigerant_valve_state_1'].config(text=self.data['refrigerant_valve_state_1'], bg=color)
        
        if 'refrigerant_valve_state_2' in self.labels:
            colors = {'핫가스': 'red', '제빙': 'blue', '냉각': 'green'}
            color = colors.get(self.data['refrigerant_valve_state_2'], 'gray')
            self.labels['refrigerant_valve_state_2'].config(text=self.data['refrigerant_valve_state_2'], bg=color)
        
        # 압축기 상태 (입력 모드가 아닐 때만)
        if 'compressor_state' in self.labels and not self.edit_mode:
            if self.data['compressor_state'] == '동작중':
                self.labels['compressor_state'].config(text="동작중", bg="green")
            else:
                self.labels['compressor_state'].config(text="미동작", bg="gray")
        
        # DC 팬 (입력 모드가 아닐 때만)
        if not self.edit_mode:
            for fan_key in ['dc_fan1', 'dc_fan2']:
                if fan_key in self.labels:
                    if self.data[fan_key] == 'ON':
                        self.labels[fan_key].config(text="ON", bg="green")
                    else:
                        self.labels[fan_key].config(text="OFF", bg="gray")
        
        # 현재 RPS, 에러코드 (항상 업데이트)
        if 'error_code' in self.labels:
            self.labels['error_code'].config(text=str(self.data.get('error_code', 0)))
    
    def set_connection_state(self, connected):
        """연결 상태에 따라 버튼 활성화/비활성화"""
        if self.send_btn:
            self.send_btn.config(state="normal" if connected else "disabled")
    
    def get_data(self):
        """현재 데이터 반환"""
        return self.data.copy()

