"""
제빙 시스템 모듈
제빙 시스템의 GUI 위젯 생성, 데이터 업데이트, 제어 기능을 담당합니다.
"""
import tkinter as tk
from tkinter import ttk, messagebox
import constants


class IcemakingSystem:
    """제빙 시스템 클래스"""
    
    def __init__(self, root, comm, log_callback, apply_table_callback=None):
        """
        Args:
            root: Tkinter 루트 윈도우
            comm: SerialCommunication 객체
            log_callback: 로그 출력 콜백 함수
            apply_table_callback: 제빙테이블 적용 콜백 함수 (gui_main의 apply_icemaking_table)
        """
        self.root = root
        self.comm = comm
        self.log_communication = log_callback
        self.apply_table_callback = apply_table_callback
        
        # 데이터 저장소
        self.data = {
            'operation': '대기',
            'ice_step': 0,                # 제빙 STEP (ice_step 값에 따라 operation 상태 결정)
            'target_rps': 0,               # 목표 RPS
            'target_temp': 0,              # 목표 온도
            'icemaking_time': 0,
            'water_capacity': 0,
            'swing_on_time': 0,
            'swing_off_time': 0,
            'tray_position': 0,           # 트레이 위치 (0:제빙, 1:탈빙, 2:이동중, 3:에러)
            'ice_jam_state': 0            # 얼음걸림 상태 (0:없음, 1:걸림)
        }
        
        # 입력 모드 상태
        self.edit_mode = False
        self.temp_data = {
            'operation': '대기',
            'target_rps': 0,
            'target_temp': 0,
            'icemaking_time': 0,
            'water_capacity': 0,
            'swing_on_time': 0,
            'swing_off_time': 0
        }
        
        # GUI 위젯 참조
        self.labels = {}
        self.send_btn = None
    
    def create_widgets(self, parent):
        """제빙 섹션 GUI 위젯 생성"""
        icemaking_frame = ttk.LabelFrame(parent, text="제빙", padding="2")
        icemaking_frame.grid(row=0, column=2, sticky=(tk.W, tk.E, tk.N, tk.S), padx=1)
        
        # 제빙 동작 (토글 버튼)
        operation_frame = ttk.Frame(icemaking_frame)
        operation_frame.grid(row=0, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(operation_frame, text="제빙 동작:", font=("Arial", 9), width=9).pack(side=tk.LEFT)
        self.labels['operation'] = tk.Label(operation_frame, text="대기", 
                                            fg="white", bg="blue", font=("Arial", 8, "bold"),
                                            width=10, relief="raised", cursor="hand2")
        self.labels['operation'].pack(side=tk.LEFT, padx=(2, 0))
        self.labels['operation'].bind("<Button-1>", self._toggle_operation)
        
        # 목표 RPS (입력 가능)
        target_rps_frame = ttk.Frame(icemaking_frame)
        target_rps_frame.grid(row=2, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(target_rps_frame, text="목표 RPS:", font=("Arial", 9), width=9).pack(side=tk.LEFT)
        vcmd_rps = (self.root.register(self._validate_rps), '%P')
        self.labels['target_rps'] = tk.Entry(target_rps_frame, font=("Arial", 9), 
                                             width=8, validate='key', validatecommand=vcmd_rps,
                                             state='readonly')
        self.labels['target_rps'].insert(0, "0")
        self.labels['target_rps'].pack(side=tk.LEFT, padx=(2, 0))
        
        # 제빙시간
        time_frame = ttk.Frame(icemaking_frame)
        time_frame.grid(row=3, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(time_frame, text="제빙시간:", font=("Arial", 9), width=9).pack(side=tk.LEFT)
        vcmd_num = (self.root.register(self._validate_number), '%P')
        self.labels['icemaking_time'] = tk.Entry(time_frame, font=("Arial", 9), 
                                                 width=8, validate='key', validatecommand=vcmd_num,
                                                 state='readonly')
        self.labels['icemaking_time'].insert(0, "0")
        self.labels['icemaking_time'].pack(side=tk.LEFT, padx=(2, 0))
        ttk.Label(time_frame, text="ms", font=("Arial", 9)).pack(side=tk.LEFT)
        
        # 입수 용량
        capacity_frame = ttk.Frame(icemaking_frame)
        capacity_frame.grid(row=4, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(capacity_frame, text="입수 용량:", font=("Arial", 9), width=9).pack(side=tk.LEFT)
        self.labels['water_capacity'] = tk.Entry(capacity_frame, font=("Arial", 9), 
                                                 width=8, validate='key', validatecommand=vcmd_num,
                                                 state='readonly')
        self.labels['water_capacity'].insert(0, "0")
        self.labels['water_capacity'].pack(side=tk.LEFT, padx=(2, 0))
        ttk.Label(capacity_frame, text="Hz", font=("Arial", 9)).pack(side=tk.LEFT)
        
        # 스윙바 ON 시간
        swing_on_frame = ttk.Frame(icemaking_frame)
        swing_on_frame.grid(row=5, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(swing_on_frame, text="스윙바 ON:", font=("Arial", 9), width=9).pack(side=tk.LEFT)
        self.labels['swing_on_time'] = tk.Entry(swing_on_frame, font=("Arial", 9), 
                                                width=8, validate='key', validatecommand=vcmd_num,
                                                state='readonly')
        self.labels['swing_on_time'].insert(0, "0")
        self.labels['swing_on_time'].pack(side=tk.LEFT, padx=(2, 0))
        ttk.Label(swing_on_frame, text="ms", font=("Arial", 9)).pack(side=tk.LEFT)
        
        # 스윙바 OFF 시간
        swing_off_frame = ttk.Frame(icemaking_frame)
        swing_off_frame.grid(row=6, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(swing_off_frame, text="스윙바 OFF:", font=("Arial", 9), width=9).pack(side=tk.LEFT)
        self.labels['swing_off_time'] = tk.Entry(swing_off_frame, font=("Arial", 9), 
                                                 width=8, validate='key', validatecommand=vcmd_num,
                                                 state='readonly')
        self.labels['swing_off_time'].insert(0, "0")
        self.labels['swing_off_time'].pack(side=tk.LEFT, padx=(2, 0))
        ttk.Label(swing_off_frame, text="ms", font=("Arial", 9)).pack(side=tk.LEFT)
        
        # 트레이 위치
        tray_position_frame = ttk.Frame(icemaking_frame)
        tray_position_frame.grid(row=7, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(tray_position_frame, text="트레이위치:", font=("Arial", 9), width=9).pack(side=tk.LEFT)
        self.labels['tray_position'] = tk.Label(tray_position_frame, text="제빙", 
                                                fg="white", bg="gray", font=("Arial", 8, "bold"),
                                                width=10, relief="raised")
        self.labels['tray_position'].pack(side=tk.LEFT, padx=(2, 0))
        
        # 얼음걸림 상태
        ice_jam_frame = ttk.Frame(icemaking_frame)
        ice_jam_frame.grid(row=8, column=0, sticky=(tk.W, tk.E), pady=1)
        ttk.Label(ice_jam_frame, text="얼음걸림:", font=("Arial", 9), width=9).pack(side=tk.LEFT)
        self.labels['ice_jam_state'] = tk.Label(ice_jam_frame, text="없음", 
                                                fg="white", bg="green", font=("Arial", 8, "bold"),
                                                width=10, relief="raised")
        self.labels['ice_jam_state'].pack(side=tk.LEFT, padx=(2, 0))
        
        # CMD 0xB2 전송 버튼
        send_btn_frame = ttk.Frame(icemaking_frame)
        send_btn_frame.grid(row=9, column=0, sticky=(tk.W, tk.E), pady=(5, 1))
        self.send_btn = ttk.Button(send_btn_frame, text="제빙 설정 입력 모드",
                                   command=self.send_control, state="disabled")
        self.send_btn.pack(fill=tk.X)
        
        # 제빙테이블 적용 버튼
        table_btn_frame = ttk.Frame(icemaking_frame)
        table_btn_frame.grid(row=10, column=0, sticky=(tk.W, tk.E), pady=(5, 1))
        self.table_btn = ttk.Button(table_btn_frame, text="제빙테이블 적용",
                                   command=self._apply_freezing_table, state="disabled")
        self.table_btn.pack(fill=tk.X)
        
        icemaking_frame.columnconfigure(0, weight=1)
        
        return icemaking_frame
    
    def _validate_number(self, value):
        """숫자 입력 검증"""
        if value == "":
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
        """제빙 동작 토글"""
        if not self.edit_mode:
            return
        
        current_value = self.temp_data['operation']
        next_value = '동작' if current_value == '대기' else '대기'
        self.temp_data['operation'] = next_value
        
        if next_value == '동작':
            self.labels['operation'].config(text="동작", bg="green")
        else:
            self.labels['operation'].config(text="대기", bg="blue")
    
    def send_control(self):
        """제빙 제어 CMD 0xB2 전송"""
        if not self.comm.is_connected:
            messagebox.showwarning("경고", "시리얼 포트가 연결되지 않았습니다.")
            return
        
        if not self.edit_mode:
            # 입력 모드 활성화
            self.edit_mode = True
            
            # 현재 값을 임시 저장소에 복사
            self.temp_data['operation'] = self.data['operation']
            self.temp_data['target_rps'] = self.data['target_rps']
            self.temp_data['icemaking_time'] = self.data['icemaking_time']
            self.temp_data['water_capacity'] = self.data['water_capacity']
            self.temp_data['swing_on_time'] = self.data['swing_on_time']
            self.temp_data['swing_off_time'] = self.data['swing_off_time']
            
            # Entry 위젯들을 편집 가능하게 설정
            self.labels['target_rps'].config(state='normal', bg='lightyellow')
            self.labels['icemaking_time'].config(state='normal', bg='lightyellow')
            self.labels['water_capacity'].config(state='normal', bg='lightyellow')
            self.labels['swing_on_time'].config(state='normal', bg='lightyellow')
            self.labels['swing_off_time'].config(state='normal', bg='lightyellow')
            
            # 제빙 동작 라벨 UI 업데이트
            if self.temp_data['operation'] == '동작':
                self.labels['operation'].config(text="동작", bg="green")
            else:
                self.labels['operation'].config(text="대기", bg="blue")
            
            # 버튼 텍스트 변경
            self.send_btn.config(text="설정 완료 (CMD 0xB2 전송)")
            
            self.log_communication("제빙 설정 입력 모드 활성화", "purple")
        
        else:
            # 입력 모드 비활성화 및 데이터 전송
            try:
                # 입력 값 가져오기
                target_rps_str = self.labels['target_rps'].get()
                icemaking_time_str = self.labels['icemaking_time'].get()
                water_capacity_str = self.labels['water_capacity'].get()
                swing_on_str = self.labels['swing_on_time'].get()
                swing_off_str = self.labels['swing_off_time'].get()
                
                # 빈 값 체크
                if not target_rps_str or not icemaking_time_str or not water_capacity_str or not swing_on_str or not swing_off_str:
                    messagebox.showwarning("경고", "모든 값을 입력해주세요.")
                    return
                
                # 정수로 변환
                target_rps = int(float(target_rps_str))
                icemaking_time = int(float(icemaking_time_str))
                water_capacity = int(float(water_capacity_str))
                swing_on = int(float(swing_on_str))
                swing_off = int(float(swing_off_str))
                
                # 범위 체크
                if not (constants.RPS_MIN <= target_rps <= constants.RPS_MAX):
                    messagebox.showwarning("경고", f"목표 RPS는 {constants.RPS_MIN}~{constants.RPS_MAX} 범위여야 합니다.")
                    return
                
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
                target_temp = self.data.get('target_temp', 0)
                icemaking_operation = 1 if self.temp_data['operation'] == '동작' else 0
                
                # 범위 체크
                if not (-127 <= target_temp <= 127):
                    messagebox.showwarning("경고", "목표 온도는 -127~127℃ 범위여야 합니다.")
                    return
                
                # 임시 저장소의 값을 실제 데이터로 복사
                self.data['operation'] = self.temp_data['operation']
                self.data['target_rps'] = target_rps
                self.data['icemaking_time'] = icemaking_time
                self.data['water_capacity'] = water_capacity
                self.data['swing_on_time'] = swing_on
                self.data['swing_off_time'] = swing_off
                
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
                self.log_communication(f"  제빙 동작: {self.temp_data['operation']} (0x{data_field[2]:02X})", "gray")
                self.log_communication(f"  제빙시간: {icemaking_time}ms", "gray")
                self.log_communication(f"  입수 용량: {water_capacity}Hz", "gray")
                self.log_communication(f"  DATA FIELD (HEX): {hex_data}", "gray")
                
                # CMD 0xB2 패킷 전송
                success, message = self.comm.send_packet(0xB2, bytes(data_field))
                
                if success:
                    self.log_communication(f"  전송 성공 (CMD 0xB2, 7바이트)", "green")
                    
                    # 입력 모드 비활성화
                    self.edit_mode = False
                    
                    # Entry 위젯들을 읽기 전용으로 설정
                    self.labels['icemaking_time'].config(state='readonly', bg='white')
                    self.labels['water_capacity'].config(state='readonly', bg='white')
                    self.labels['swing_on_time'].config(state='readonly', bg='white')
                    self.labels['swing_off_time'].config(state='readonly', bg='white')
                    
                    # 버튼 텍스트 변경
                    self.send_btn.config(text="제빙 설정 입력 모드")
                    
                else:
                    self.log_communication(f"  전송 실패: {message}", "red")
                    
            except ValueError:
                messagebox.showerror("오류", "올바른 숫자를 입력해주세요.")
            except Exception as e:
                self.log_communication(f"제빙 제어 오류: {str(e)}", "red")
    
    def update_data(self, new_data):
        """데이터 업데이트"""
        self.data.update(new_data)
        self._update_gui()
    
    def _update_gui(self):
        """GUI 업데이트"""
        # 제빙 동작 (입력 모드가 아닐 때만)
        if 'operation' in self.labels and not self.edit_mode:
            operation_text = self.data.get('operation', '대기')
            # ice_step이 0이 아니면 동작 중으로 간주
            ice_step = self.data.get('ice_step', 0)
            if ice_step == 0 or operation_text == '대기':
                self.labels['operation'].config(text=operation_text, bg="blue")
            else:
                # 동작 중인 상태 (제빙중, 예열, 탈빙중 등)
                self.labels['operation'].config(text=operation_text, bg="green")
        
        # 입력 모드가 아닐 때만 Entry 위젯 업데이트
        if not self.edit_mode:
            for key in ['target_rps', 'icemaking_time', 'water_capacity', 'swing_on_time', 'swing_off_time']:
                if key in self.labels:
                    widget = self.labels[key]
                    current_value = widget.get()
                    new_value = str(self.data.get(key, 0))
                    if current_value != new_value:
                        widget.config(state='normal')
                        widget.delete(0, tk.END)
                        widget.insert(0, new_value)
                        widget.config(state='readonly')
        
        # 트레이 위치 업데이트
        if 'tray_position' in self.labels:
            tray_pos = self.data.get('tray_position', 0)
            tray_pos_map = {0: '제빙', 1: '탈빙', 2: '이동중', 3: '에러'}
            tray_pos_text = tray_pos_map.get(tray_pos, f'알 수 없음({tray_pos})')
            tray_pos_colors = {0: 'blue', 1: 'orange', 2: 'yellow', 3: 'red'}
            tray_pos_color = tray_pos_colors.get(tray_pos, 'gray')
            
            if self.labels['tray_position'].cget('text') != tray_pos_text:
                self.labels['tray_position'].config(text=tray_pos_text, bg=tray_pos_color)
        
        # 얼음걸림 상태 업데이트
        if 'ice_jam_state' in self.labels:
            ice_jam = self.data.get('ice_jam_state', 0)
            ice_jam_text = '걸림' if ice_jam == 1 else '없음'
            ice_jam_color = 'red' if ice_jam == 1 else 'green'
            
            if self.labels['ice_jam_state'].cget('text') != ice_jam_text:
                self.labels['ice_jam_state'].config(text=ice_jam_text, bg=ice_jam_color)
    
    def _apply_freezing_table(self):
        """제빙테이블 적용 버튼 클릭 시 호출"""
        if self.apply_table_callback:
            self.apply_table_callback()
        else:
            self.log_communication("제빙테이블 적용 기능이 연결되지 않았습니다.", "red")
    
    def set_connection_state(self, connected):
        """연결 상태에 따라 버튼 활성화/비활성화"""
        if self.send_btn:
            self.send_btn.config(state="normal" if connected else "disabled")
        if self.table_btn:
            self.table_btn.config(state="normal" if connected else "disabled")
    
    def get_data(self):
        """현재 데이터 반환"""
        return self.data.copy()

