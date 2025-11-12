"""
보냉 시스템 모듈
보냉 시스템의 GUI 위젯 생성, 데이터 업데이트, 제어 기능을 담당합니다.
"""
import tkinter as tk
from tkinter import ttk, messagebox
import struct
import constants


class RefrigerationSystem:
    """보냉 시스템 클래스"""
    
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
            'operation': '보냉대기',        # 보냉대기/보냉진행/보냉완료/만빙대기
            'target_rps': 0,               # 보냉진행 설정 RPS
            'target_temp': 0,              # 보냉진행 설정온도
            'target_first_temp': 0,        # 보냉진행 첫 온도
            'cur_tray_position': '제빙',    # 제빙/중간/탈빙
        }
        
        # 입력 모드 상태
        self.edit_mode = False
        self.temp_data = {
            'operation': '보냉대기',
            'target_rps': 0,
            'target_temp': 0,
            'target_first_temp': 0,
            'cur_tray_position': '제빙',
        }
        
        # GUI 위젯 참조
        self.labels = {}
        self.send_btn = None
    
    def create_widgets(self, parent):
        """보냉 섹션 GUI 위젯 생성"""
        refrigeration_frame = ttk.LabelFrame(parent, text="보냉", padding="2")
        refrigeration_frame.grid(row=0, column=3, sticky=(tk.W, tk.E, tk.N, tk.S), padx=1)
        
        # 보냉 동작 (토글 버튼)
        operation_frame = ttk.Frame(refrigeration_frame)
        operation_frame.grid(row=0, column=0, sticky=(tk.W, tk.E), pady=1)
        operation_frame.columnconfigure(0, weight=1)
        ttk.Label(operation_frame, text="보냉 동작:", font=("Arial", 9), width=9).pack(side=tk.LEFT)
        self.labels['operation'] = tk.Label(operation_frame, text="보냉대기", 
                                            fg="white", bg="blue", font=("Arial", 8, "bold"),
                                            width=10, relief="raised", cursor="hand2")
        self.labels['operation'].pack(side=tk.RIGHT)
        self.labels['operation'].bind("<Button-1>", self._toggle_operation)
        
        # 목표 RPS (입력 가능)
        target_rps_frame = ttk.Frame(refrigeration_frame)
        target_rps_frame.grid(row=1, column=0, sticky=(tk.W, tk.E), pady=1)
        target_rps_frame.columnconfigure(0, weight=1)
        ttk.Label(target_rps_frame, text="목표 RPS:", font=("Arial", 9), width=9).pack(side=tk.LEFT)
        vcmd_rps = (self.root.register(self._validate_rps), '%P')
        self.labels['target_rps'] = tk.Entry(target_rps_frame, font=("Arial", 9), 
                                             width=8, validate='key', validatecommand=vcmd_rps,
                                             state='readonly')
        self.labels['target_rps'].insert(0, "0")
        self.labels['target_rps'].pack(side=tk.RIGHT)
        
        # 검증 함수 등록 (온도 필드용)
        vcmd_num = (self.root.register(self._validate_number), '%P')
        
        # 목표 온도 (입력 가능)
        target_temp_frame = ttk.Frame(refrigeration_frame)
        target_temp_frame.grid(row=2, column=0, sticky=(tk.W, tk.E), pady=1)
        target_temp_frame.columnconfigure(0, weight=1)
        ttk.Label(target_temp_frame, text="목표온도:", font=("Arial", 9), width=9).pack(side=tk.LEFT)
        temp_unit_frame = ttk.Frame(target_temp_frame)
        temp_unit_frame.pack(side=tk.RIGHT)
        self.labels['target_temp'] = tk.Entry(temp_unit_frame, font=("Arial", 9), 
                                              width=8, validate='key', validatecommand=vcmd_num,
                                              state='readonly')
        self.labels['target_temp'].insert(0, "0")
        self.labels['target_temp'].pack(side=tk.LEFT)
        ttk.Label(temp_unit_frame, text="℃", font=("Arial", 9)).pack(side=tk.LEFT, padx=(2, 0))
        
        # 첫 온도 (입력 가능)
        first_temp_frame = ttk.Frame(refrigeration_frame)
        first_temp_frame.grid(row=3, column=0, sticky=(tk.W, tk.E), pady=1)
        first_temp_frame.columnconfigure(0, weight=1)
        ttk.Label(first_temp_frame, text="첫 온도:", font=("Arial", 9), width=9).pack(side=tk.LEFT)
        first_temp_unit_frame = ttk.Frame(first_temp_frame)
        first_temp_unit_frame.pack(side=tk.RIGHT)
        self.labels['target_first_temp'] = tk.Entry(first_temp_unit_frame, font=("Arial", 9), 
                                                    width=8, validate='key', validatecommand=vcmd_num,
                                                    state='readonly')
        self.labels['target_first_temp'].insert(0, "0")
        self.labels['target_first_temp'].pack(side=tk.LEFT)
        ttk.Label(first_temp_unit_frame, text="℃", font=("Arial", 9)).pack(side=tk.LEFT, padx=(2, 0))
        
        # 트레이 위치 (입력 모드에서 토글 가능)
        tray_frame = ttk.Frame(refrigeration_frame)
        tray_frame.grid(row=4, column=0, sticky=(tk.W, tk.E), pady=1)
        tray_frame.columnconfigure(0, weight=1)
        ttk.Label(tray_frame, text="트레이위치:", font=("Arial", 9), width=9).pack(side=tk.LEFT)
        self.labels['cur_tray_position'] = tk.Label(tray_frame, text="제빙", 
                                                    fg="white", bg="blue", font=("Arial", 8, "bold"),
                                                    width=10, relief="raised", cursor="hand2")
        self.labels['cur_tray_position'].pack(side=tk.RIGHT)
        self.labels['cur_tray_position'].bind("<Button-1>", self._toggle_tray_position)
        
        # CMD 전송 버튼
        send_btn_frame = ttk.Frame(refrigeration_frame)
        send_btn_frame.grid(row=5, column=0, sticky=(tk.W, tk.E), pady=(5, 1))
        self.send_btn = ttk.Button(send_btn_frame, text="보냉 설정 입력 모드",
                                   command=self.send_control, state="disabled")
        self.send_btn.pack(fill=tk.X)
        
        refrigeration_frame.columnconfigure(0, weight=1)
        
        return refrigeration_frame
    
    def _validate_number(self, value):
        """숫자 입력 검증 (음수 포함)"""
        if value == "" or value == "-":
            return True
        try:
            float(value)
            return True
        except ValueError:
            return False
    
    def _validate_rps(self, value):
        """RPS 범위 검증 (37~75) - 입력 중에는 숫자만 허용"""
        if value == "":
            return True
        try:
            # 숫자인지 확인 (입력 중에는 범위 체크하지 않음)
            int(value)
            return True
        except (ValueError, OverflowError):
            return False
    
    def _toggle_operation(self, event):
        """보냉 동작 토글"""
        if not self.edit_mode:
            return
        
        operation_sequence = ['보냉대기', '보냉진행', '보냉완료', '만빙대기']
        current_value = self.temp_data['operation']
        
        try:
            current_index = operation_sequence.index(current_value)
            next_index = (current_index + 1) % len(operation_sequence)
            next_value = operation_sequence[next_index]
        except ValueError:
            next_value = '보냉대기'
        
        self.temp_data['operation'] = next_value
        
        # UI 업데이트
        colors = {'보냉대기': 'blue', '보냉진행': 'green', '보냉완료': 'orange', '만빙대기': 'purple'}
        color = colors.get(next_value, 'gray')
        self.labels['operation'].config(text=next_value, bg=color)
    
    def _toggle_tray_position(self, event):
        """트레이 위치 토글 (입력 모드에서만)"""
        if not self.edit_mode:
            return
        
        # 제빙(0) / 탈빙(1)만 지원
        if self.temp_data['cur_tray_position'] == '제빙':
            self.temp_data['cur_tray_position'] = '탈빙'
            self.labels['cur_tray_position'].config(text='탈빙', bg='green')
        else:
            self.temp_data['cur_tray_position'] = '제빙'
            self.labels['cur_tray_position'].config(text='제빙', bg='blue')
    
    def send_control(self):
        """보냉 제어 CMD 0xB4 전송"""
        if not self.comm.is_connected:
            messagebox.showwarning("경고", "시리얼 포트가 연결되지 않았습니다.")
            return
        
        if not self.edit_mode:
            # 입력 모드 활성화
            self.edit_mode = True
            
            # 현재 값을 임시 저장소에 복사
            self.temp_data['target_rps'] = self.data['target_rps']
            self.temp_data['target_temp'] = self.data['target_temp']
            self.temp_data['target_first_temp'] = self.data['target_first_temp']
            self.temp_data['cur_tray_position'] = self.data['cur_tray_position']
            
            # Entry 위젯들을 편집 가능하게 설정
            self.labels['target_rps'].config(state='normal', bg='lightyellow')
            self.labels['target_temp'].config(state='normal', bg='lightyellow')
            self.labels['target_first_temp'].config(state='normal', bg='lightyellow')
            
            # Entry 위젯에 현재 값 설정
            self.labels['target_rps'].delete(0, tk.END)
            self.labels['target_rps'].insert(0, str(self.temp_data['target_rps']))
            self.labels['target_temp'].delete(0, tk.END)
            self.labels['target_temp'].insert(0, str(self.temp_data['target_temp']))
            self.labels['target_first_temp'].delete(0, tk.END)
            self.labels['target_first_temp'].insert(0, str(self.temp_data['target_first_temp']))
            
            # 트레이 위치 라벨을 클릭 가능하게 표시
            if self.temp_data['cur_tray_position'] == '제빙':
                self.labels['cur_tray_position'].config(text="제빙", bg="blue")
            else:
                self.labels['cur_tray_position'].config(text="탈빙", bg="green")
            
            # 버튼 텍스트 변경
            self.send_btn.config(text="설정 완료 (CMD 0xB4 전송)")
            
            self.log_communication("보냉 설정 입력 모드 활성화", "purple")
        
        else:
            # 입력 모드 비활성화 및 데이터 전송
            try:
                # 입력 값 가져오기
                target_rps_str = self.labels['target_rps'].get()
                target_temp_str = self.labels['target_temp'].get()
                target_first_temp_str = self.labels['target_first_temp'].get()
                
                # 빈 값 체크
                if not target_rps_str or not target_temp_str or not target_first_temp_str:
                    messagebox.showwarning("경고", "모든 값을 입력해주세요.")
                    return
                
                # 정수로 변환
                target_rps = int(float(target_rps_str))
                target_temp = int(float(target_temp_str))
                target_first_temp = int(float(target_first_temp_str))
                
                # 범위 체크
                if not (constants.RPS_MIN <= target_rps <= constants.RPS_MAX):
                    messagebox.showwarning("경고", f"목표 RPS는 {constants.RPS_MIN}~{constants.RPS_MAX} 범위여야 합니다.")
                    return
                
                if not (-40 <= target_temp <= 80):
                    messagebox.showwarning("경고", "목표 온도는 -40~80℃ 범위여야 합니다.")
                    return
                
                if not (-40 <= target_first_temp <= 80):
                    messagebox.showwarning("경고", "첫 온도는 -40~80℃ 범위여야 합니다.")
                    return
                
                # 트레이 위치: 제빙(0) / 탈빙(1)
                tray_position = 0 if self.temp_data['cur_tray_position'] == '제빙' else 1
                
                # DATA FIELD 구성 (4바이트)
                data_field = bytearray(4)
                data_field[0] = target_rps
                # 음수 처리: 1바이트에서 최상위 비트(MSB)가 1이면 음수, 0이면 양수
                # struct.pack('b', value)는 signed byte로 변환 (2의 보수 표현)
                data_field[1] = self.comm.protocol.int_to_signed_byte(target_temp)
                data_field[2] = self.comm.protocol.int_to_signed_byte(target_first_temp)
                data_field[3] = tray_position
                
                # 로그 출력
                hex_data = " ".join([f"{b:02X}" for b in data_field])
                self.log_communication(f"[보냉 제어] CMD 0xB4 전송 (4바이트)", "blue")
                self.log_communication(f"  목표 RPS: {target_rps}", "gray")
                self.log_communication(f"  목표 온도: {target_temp}℃", "gray")
                self.log_communication(f"  첫 온도: {target_first_temp}℃", "gray")
                self.log_communication(f"  트레이 위치: {self.temp_data['cur_tray_position']} ({tray_position})", "gray")
                self.log_communication(f"  DATA FIELD (HEX): {hex_data}", "gray")
                
                # CMD 0xB4 패킷 전송
                success, message = self.comm.send_packet(0xB4, bytes(data_field))
                
                if success:
                    self.log_communication(f"  전송 성공 (CMD 0xB4, 4바이트)", "green")
                    
                    # 입력 모드 비활성화
                    self.edit_mode = False
                    
                    # Entry 위젯들을 읽기 전용으로 설정
                    self.labels['target_rps'].config(state='readonly', bg='white')
                    self.labels['target_temp'].config(state='readonly', bg='white')
                    self.labels['target_first_temp'].config(state='readonly', bg='white')
                    
                    # 버튼 텍스트 변경
                    self.send_btn.config(text="보냉 설정 입력 모드")
                    
                    # 임시 데이터를 실제 데이터에 반영
                    self.data['target_rps'] = target_rps
                    self.data['target_temp'] = target_temp
                    self.data['target_first_temp'] = target_first_temp
                    self.data['cur_tray_position'] = self.temp_data['cur_tray_position']
                    
                    # GUI 업데이트
                    self._update_gui()
                else:
                    messagebox.showerror("오류", f"패킷 전송 실패: {message}")
                    self.log_communication(f"  전송 실패: {message}", "red")
                    
            except ValueError:
                messagebox.showerror("오류", "입력한 값이 올바르지 않습니다. 숫자를 입력해주세요.")
            except Exception as e:
                messagebox.showerror("오류", f"전송 중 오류가 발생했습니다:\n{str(e)}")
                self.log_communication(f"  전송 오류: {str(e)}", "red")
    
    def update_data(self, new_data):
        """데이터 업데이트"""
        self.data.update(new_data)
        self._update_gui()
    
    def _update_gui(self):
        """GUI 업데이트"""
        # 보냉 동작 (입력 모드가 아닐 때만)
        if 'operation' in self.labels and not self.edit_mode:
            colors = {'보냉대기': 'blue', '보냉진행': 'green', '보냉완료': 'orange', '만빙대기': 'purple'}
            color = colors.get(self.data['operation'], 'gray')
            self.labels['operation'].config(text=self.data['operation'], bg=color)
        
        # 목표 RPS (입력 모드가 아닐 때만)
        if 'target_rps' in self.labels and not self.edit_mode:
            widget = self.labels['target_rps']
            current_value = widget.get()
            new_value = str(self.data.get('target_rps', 0))
            if current_value != new_value:
                widget.config(state='normal')
                widget.delete(0, tk.END)
                widget.insert(0, new_value)
                widget.config(state='readonly')
        
        # 목표 온도 (입력 모드가 아닐 때만)
        if 'target_temp' in self.labels and not self.edit_mode:
            widget = self.labels['target_temp']
            current_value = widget.get()
            new_value = str(self.data.get('target_temp', 0))
            if current_value != new_value:
                widget.config(state='normal')
                widget.delete(0, tk.END)
                widget.insert(0, new_value)
                widget.config(state='readonly')
        
        # 첫 온도 (입력 모드가 아닐 때만)
        if 'target_first_temp' in self.labels and not self.edit_mode:
            widget = self.labels['target_first_temp']
            current_value = widget.get()
            new_value = str(self.data.get('target_first_temp', 0))
            if current_value != new_value:
                widget.config(state='normal')
                widget.delete(0, tk.END)
                widget.insert(0, new_value)
                widget.config(state='readonly')
        
        # 트레이 위치 (입력 모드가 아닐 때만 업데이트)
        if 'cur_tray_position' in self.labels and not self.edit_mode:
            colors = {'제빙': 'blue', '중간': 'orange', '탈빙': 'green'}
            color = colors.get(self.data['cur_tray_position'], 'gray')
            self.labels['cur_tray_position'].config(
                text=self.data['cur_tray_position'], bg=color
            )
    
    def set_connection_state(self, connected):
        """연결 상태에 따라 버튼 활성화/비활성화"""
        if self.send_btn:
            self.send_btn.config(state="normal" if connected else "disabled")
    
    def get_data(self):
        """현재 데이터 반환"""
        return self.data.copy()

