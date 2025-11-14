"""
냉각 시스템 모듈
냉각 시스템의 GUI 위젯 생성, 데이터 업데이트, 제어 기능을 담당합니다.
"""
import tkinter as tk
from tkinter import ttk, messagebox
import constants


class CoolingSystem:
    """냉각 시스템 클래스"""
    
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
            'operation_state': 'STOP',
            'initial_startup': False,      # 초기기동 여부 (1:초기기동, 0:일반기동)
            'target_rps': 0,               # 목표 RPS
            'on_temp': 0,
            'off_temp': 0,
            'cooling_additional_time': 0
        }
        
        # 입력 모드 상태
        self.edit_mode = False
        
        # GUI 위젯 참조
        self.labels = {}
        self.send_btn = None
    
    def create_widgets(self, parent):
        """냉각 섹션 GUI 위젯 생성"""
        cooling_frame = ttk.LabelFrame(parent, text="냉각", padding="2")
        cooling_frame.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S), padx=(0, 1))
        
        # 운전 상태
        state_frame = ttk.Frame(cooling_frame)
        state_frame.grid(row=0, column=0, sticky=(tk.W, tk.E), pady=1)
        state_frame.columnconfigure(0, weight=1)
        ttk.Label(state_frame, text="운전 상태:", font=("Arial", 8), width=8).pack(side=tk.LEFT)
        self.labels['operation_state'] = tk.Label(state_frame, text="대기", 
                                                    fg="white", bg="gray", font=("Arial", 7, "bold"),
                                                    width=8, relief="raised")
        self.labels['operation_state'].pack(side=tk.RIGHT)
        
        # 초기기동 여부
        initial_startup_frame = ttk.Frame(cooling_frame)
        initial_startup_frame.grid(row=1, column=0, sticky=(tk.W, tk.E), pady=1)
        initial_startup_frame.columnconfigure(0, weight=1)
        ttk.Label(initial_startup_frame, text="초기기동:", font=("Arial", 8), width=8).pack(side=tk.LEFT)
        self.labels['initial_startup'] = tk.Label(initial_startup_frame, text="일반기동", 
                                                  fg="white", bg="blue", font=("Arial", 7, "bold"),
                                                  width=8, relief="raised")
        self.labels['initial_startup'].pack(side=tk.RIGHT)
        
        # 목표 RPS (입력 가능)
        target_rps_frame = ttk.Frame(cooling_frame)
        target_rps_frame.grid(row=2, column=0, sticky=(tk.W, tk.E), pady=1)
        target_rps_frame.columnconfigure(0, weight=1)
        ttk.Label(target_rps_frame, text="목표 RPS:", font=("Arial", 8), width=8).pack(side=tk.LEFT)
        vcmd_rps = (self.root.register(self._validate_rps), '%P')
        self.labels['target_rps'] = tk.Entry(target_rps_frame, font=("Arial", 8), 
                                             width=6, validate='key', validatecommand=vcmd_rps,
                                             state='readonly')
        self.labels['target_rps'].insert(0, "0")
        self.labels['target_rps'].pack(side=tk.RIGHT)
        
        # ON 온도 (입력 가능)
        on_temp_frame = ttk.Frame(cooling_frame)
        on_temp_frame.grid(row=3, column=0, sticky=(tk.W, tk.E), pady=1)
        on_temp_frame.columnconfigure(0, weight=1)
        ttk.Label(on_temp_frame, text="ON 온도:", font=("Arial", 8), width=8).pack(side=tk.LEFT)
        
        vcmd_temp = (self.root.register(self._validate_number), '%P')
        temp_unit_frame = ttk.Frame(on_temp_frame)
        temp_unit_frame.pack(side=tk.RIGHT)
        self.labels['on_temp'] = tk.Entry(temp_unit_frame, font=("Arial", 8), 
                                            width=6, validate='key', validatecommand=vcmd_temp,
                                            state='readonly')
        self.labels['on_temp'].insert(0, "0")
        self.labels['on_temp'].pack(side=tk.LEFT)
        ttk.Label(temp_unit_frame, text="℃", font=("Arial", 8)).pack(side=tk.LEFT, padx=(2, 0))
        
        # OFF 온도 (입력 가능)
        off_temp_frame = ttk.Frame(cooling_frame)
        off_temp_frame.grid(row=4, column=0, sticky=(tk.W, tk.E), pady=1)
        off_temp_frame.columnconfigure(0, weight=1)
        ttk.Label(off_temp_frame, text="OFF 온도:", font=("Arial", 8), width=8).pack(side=tk.LEFT)
        temp_unit_frame2 = ttk.Frame(off_temp_frame)
        temp_unit_frame2.pack(side=tk.RIGHT)
        self.labels['off_temp'] = tk.Entry(temp_unit_frame2, font=("Arial", 8), 
                                            width=6, validate='key', validatecommand=vcmd_temp,
                                            state='readonly')
        self.labels['off_temp'].insert(0, "0")
        self.labels['off_temp'].pack(side=tk.LEFT)
        ttk.Label(temp_unit_frame2, text="℃", font=("Arial", 8)).pack(side=tk.LEFT, padx=(2, 0))
        
        # 냉각 추가시간 (입력 가능)
        add_time_frame = ttk.Frame(cooling_frame)
        add_time_frame.grid(row=5, column=0, sticky=(tk.W, tk.E), pady=1)
        add_time_frame.columnconfigure(0, weight=1)
        ttk.Label(add_time_frame, text="추가시간:", font=("Arial", 8), width=8).pack(side=tk.LEFT)
        time_unit_frame = ttk.Frame(add_time_frame)
        time_unit_frame.pack(side=tk.RIGHT)
        self.labels['cooling_additional_time'] = tk.Entry(time_unit_frame, font=("Arial", 8), 
                                                          width=6, validate='key', validatecommand=vcmd_temp,
                                                          state='readonly')
        self.labels['cooling_additional_time'].insert(0, "0")
        self.labels['cooling_additional_time'].pack(side=tk.LEFT)
        ttk.Label(time_unit_frame, text="초", font=("Arial", 8)).pack(side=tk.LEFT, padx=(2, 0))
        
        # CMD 0xB1 전송 버튼
        send_btn_frame = ttk.Frame(cooling_frame)
        send_btn_frame.grid(row=6, column=0, sticky=(tk.W, tk.E), pady=(5, 1))
        self.send_btn = ttk.Button(send_btn_frame, text="입력모드",
                                    command=self.send_control, state="disabled")
        self.send_btn.pack(fill=tk.X)
        
        cooling_frame.columnconfigure(0, weight=1)
        
        return cooling_frame
    
    def _validate_number(self, value):
        """숫자와 소숫점 입력 가능하도록 검증 (소숫점 첫째 자리만 허용)"""
        if value == "":
            return True
        try:
            # 소숫점이 2개 이상이면 안됨
            if value.count('.') > 1:
                return False
            # 소숫점이 있으면 소수 부분이 첫째 자리(1자리)까지만 허용
            if '.' in value:
                parts = value.split('.')
                if len(parts) != 2:
                    return False
                # 소수 부분(parts[1])이 1자리를 초과하면 안됨
                decimal_part = parts[1]
                if len(decimal_part) > 1:
                    return False
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
    
    def send_control(self):
        """냉각 제어 CMD 0xB1 전송 - 입력 모드 토글 방식"""
        if not self.comm.is_connected:
            messagebox.showwarning("경고", "시리얼 포트가 연결되지 않았습니다.")
            return
        
        if not self.edit_mode:
            # 입력 모드 활성화
            self.edit_mode = True
            
            # Entry 위젯들을 편집 가능하게 설정
            self.labels['target_rps'].config(state='normal', bg='lightyellow')
            self.labels['on_temp'].config(state='normal', bg='lightyellow')
            self.labels['off_temp'].config(state='normal', bg='lightyellow')
            self.labels['cooling_additional_time'].config(state='normal', bg='lightyellow')
            
            # 버튼 텍스트 변경
            self.send_btn.config(text="설정 완료 (CMD 0xB1 전송)")
            
            self.log_communication("냉각 설정 입력 모드 활성화", "purple")
        
        else:
            # 입력 모드 비활성화 및 데이터 전송
            try:
                # 입력 값 가져오기
                target_rps_str = self.labels['target_rps'].get()
                on_temp_str = self.labels['on_temp'].get()
                off_temp_str = self.labels['off_temp'].get()
                additional_time_str = self.labels['cooling_additional_time'].get()
                
                # 빈 값 체크
                if not target_rps_str or not on_temp_str or not off_temp_str or not additional_time_str:
                    messagebox.showwarning("경고", "모든 값을 입력해주세요.")
                    return
                
                # 온도 값 처리 (소숫점 첫째 자리만 허용, 10을 곱해서 정수로 변환)
                on_temp_float = float(on_temp_str)
                off_temp_float = float(off_temp_str)
                on_temp_int = int(on_temp_float * 10)  # 5.5 → 55
                off_temp_int = int(off_temp_float * 10)  # 25.5 → 255
                
                # 정수로 변환
                target_rps = int(float(target_rps_str))
                additional_time = int(float(additional_time_str))
                
                # 범위 체크
                if not (constants.RPS_MIN <= target_rps <= constants.RPS_MAX):
                    messagebox.showwarning("경고", f"목표 RPS는 {constants.RPS_MIN}~{constants.RPS_MAX} 범위여야 합니다.")
                    return
                
                # 온도 범위 체크 (unsigned char: 0~255, 10을 곱한 값이 0~255 범위여야 함)
                # 온도는 0.0~25.5℃ 범위 (10을 곱하면 0~255)
                if not (0.0 <= on_temp_float <= 25.5):
                    messagebox.showwarning("경고", "냉각 ON 온도는 0.0~25.5℃ 범위여야 합니다.")
                    return
                
                if not (0.0 <= off_temp_float <= 25.5):
                    messagebox.showwarning("경고", "냉각 OFF 온도는 0.0~25.5℃ 범위여야 합니다.")
                    return
                
                # unsigned char 범위 체크 (0~255)
                if not (0 <= on_temp_int <= 255):
                    messagebox.showwarning("경고", "냉각 ON 온도 값이 범위를 벗어났습니다 (0~255).")
                    return
                
                if not (0 <= off_temp_int <= 255):
                    messagebox.showwarning("경고", "냉각 OFF 온도 값이 범위를 벗어났습니다 (0~255).")
                    return
                
                # 추가시간 범위 체크 (0 ~ 65535)
                if not (0 <= additional_time <= 65535):
                    messagebox.showwarning("경고", "추가시간은 0~65535초 범위여야 합니다.")
                    return
                
                # DATA FIELD 구성 (5바이트)
                data_field = bytearray(5)
                data_field[0] = target_rps  # DATA 1: Target RPS
                data_field[1] = on_temp_int & 0xFF  # DATA 2: 냉각 ON 온도 (unsigned char, 10을 곱한 값)
                data_field[2] = off_temp_int & 0xFF  # DATA 3: 냉각 OFF 온도 (unsigned char, 10을 곱한 값)
                data_field[3] = (additional_time >> 8) & 0xFF  # DATA 4: 추가시간 High값
                data_field[4] = additional_time & 0xFF  # DATA 5: 추가시간 Low값
                
                # 로그 출력
                hex_data = " ".join([f"{b:02X}" for b in data_field])
                self.log_communication(f"[냉각 제어] CMD 0xB1 전송 (5바이트)", "blue")
                self.log_communication(f"  목표 RPS: {target_rps}", "gray")
                self.log_communication(f"  냉각 ON 온도: {on_temp_float}℃ (전송값: {on_temp_int})", "gray")
                self.log_communication(f"  냉각 OFF 온도: {off_temp_float}℃ (전송값: {off_temp_int})", "gray")
                self.log_communication(f"  추가시간: {additional_time}초", "gray")
                self.log_communication(f"  DATA FIELD (HEX): {hex_data}", "gray")
                
                # CMD 0xB1 패킷 전송 (우선순위, 응답을 받을 때까지 재전송)
                success, message = self.comm.send_packet(0xB1, bytes(data_field), priority=True, retry_until_response=True)
                
                if success:
                    self.log_communication(f"  전송 요청 성공 (CMD 0xB1, 5바이트, 우선순위 큐 추가됨)", "green")
                    
                    # 입력 모드 비활성화
                    self.edit_mode = False
                    
                    # Entry 위젯들을 읽기 전용으로 설정
                    self.labels['target_rps'].config(state='readonly', bg='white')
                    self.labels['on_temp'].config(state='readonly', bg='white')
                    self.labels['off_temp'].config(state='readonly', bg='white')
                    self.labels['cooling_additional_time'].config(state='readonly', bg='white')
                    
                    # 버튼 텍스트 변경
                    self.send_btn.config(text="입력모드")
                    
                else:
                    self.log_communication(f"  전송 실패: {message}", "red")
                    
            except ValueError:
                messagebox.showerror("오류", "올바른 숫자를 입력해주세요.")
            except Exception as e:
                self.log_communication(f"냉각 제어 오류: {str(e)}", "red")
    
    def update_data(self, new_data):
        """데이터 업데이트"""
        self.data.update(new_data)
        self._update_gui()
    
    def _update_gui(self):
        """GUI 업데이트"""
        # 운전 상태 업데이트
        if 'operation_state' in self.labels:
            if self.data['operation_state'] == 'GOING' or self.data['operation_state'] == '가동':
                self.labels['operation_state'].config(text="가동", bg="green")
            else:
                self.labels['operation_state'].config(text="대기", bg="gray")
        
        # 초기기동 여부 업데이트
        if 'initial_startup' in self.labels:
            if self.data.get('initial_startup', False):
                self.labels['initial_startup'].config(text="초기기동", bg="orange")
            else:
                self.labels['initial_startup'].config(text="일반기동", bg="blue")
        
        # 입력 모드가 아닐 때만 Entry 위젯 업데이트
        if not self.edit_mode:
            for key in ['target_rps', 'on_temp', 'off_temp', 'cooling_additional_time']:
                if key in self.labels:
                    widget = self.labels[key]
                    current_value = widget.get()
                    new_value = str(self.data.get(key, 0))
                    if current_value != new_value:
                        widget.config(state='normal')
                        widget.delete(0, tk.END)
                        widget.insert(0, new_value)
                        widget.config(state='readonly')
    
    def set_connection_state(self, connected):
        """연결 상태에 따라 버튼 활성화/비활성화"""
        if self.send_btn:
            self.send_btn.config(state="normal" if connected else "disabled")
    
    def get_data(self):
        """현재 데이터 반환"""
        return self.data.copy()

