"""
밸브 시스템 모듈
밸브 시스템의 GUI 위젯 생성, 데이터 업데이트, 제어 기능을 담당합니다.
"""
import tkinter as tk
from tkinter import ttk


class ValveSystem:
    """밸브 시스템 클래스"""
    
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
        self.nos_valve_states = {i: False for i in range(1, 6)}  # False=CLOSE, True=OPEN
        self.feed_valve_states = {i: False for i in range(1, 16)}  # False=CLOSE, True=OPEN
        
        # GUI 위젯 참조
        self.nos_valve_labels = {}
        self.feed_valve_labels = {}
    
    def create_widgets(self, parent, tab_type='freezing'):
        """밸브 섹션 GUI 위젯 생성"""
        valve_frame = ttk.LabelFrame(parent, text="밸브 상태", padding="2")
        valve_frame.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S), padx=(0, 2))
        
        # NOS 밸브 (1~5)
        nos_frame = ttk.LabelFrame(valve_frame, text="NOS 밸브", padding="2")
        nos_frame.grid(row=0, column=0, sticky=(tk.W, tk.E), pady=(0, 2))
        
        for i in range(1, 6):
            row = (i - 1) // 3
            col = (i - 1) % 3
            
            valve_item_frame = ttk.Frame(nos_frame)
            valve_item_frame.grid(row=row, column=col, padx=2, pady=1, sticky=tk.W)
            
            ttk.Label(valve_item_frame, text=f"NOS{i}:", font=("Arial", 8), width=6).pack(side=tk.LEFT)
            label = tk.Label(valve_item_frame, text="CLOSE", 
                            fg="white", bg="red", font=("Arial", 7, "bold"),
                            width=6, relief="raised", cursor="hand2")
            label.pack(side=tk.LEFT, padx=(2, 0))
            label.bind("<Button-1>", lambda e, num=i: self._on_valve_click(num, 'NOS'))
            
            if tab_type == 'freezing':
                self.nos_valve_labels[i] = label
            elif tab_type == 'control':
                if not hasattr(self, 'nos_valve_labels_control'):
                    self.nos_valve_labels_control = {}
                self.nos_valve_labels_control[i] = label
        
        # FEED 밸브 (1~15)
        feed_frame = ttk.LabelFrame(valve_frame, text="FEED 밸브", padding="2")
        feed_frame.grid(row=1, column=0, sticky=(tk.W, tk.E))
        
        for i in range(1, 16):
            row = (i - 1) // 5
            col = (i - 1) % 5
            
            valve_item_frame = ttk.Frame(feed_frame)
            valve_item_frame.grid(row=row, column=col, padx=1, pady=1, sticky=tk.W)
            
            ttk.Label(valve_item_frame, text=f"F{i:2d}:", font=("Arial", 7), width=4).pack(side=tk.LEFT)
            label = tk.Label(valve_item_frame, text="CLOSE", 
                            fg="white", bg="red", font=("Arial", 6, "bold"),
                            width=5, relief="raised", cursor="hand2")
            label.pack(side=tk.LEFT, padx=(1, 0))
            label.bind("<Button-1>", lambda e, num=i: self._on_valve_click(num, 'FEED'))
            
            if tab_type == 'freezing':
                self.feed_valve_labels[i] = label
            elif tab_type == 'control':
                if not hasattr(self, 'feed_valve_labels_control'):
                    self.feed_valve_labels_control = {}
                self.feed_valve_labels_control[i] = label
        
        valve_frame.columnconfigure(0, weight=1)
        
        return valve_frame
    
    def _on_valve_click(self, valve_num, valve_type):
        """밸브 클릭 이벤트 처리"""
        if not self.comm.is_connected:
            return
        
        # 밸브 제어 CMD 0xA0 전송
        self.send_valve_control(valve_num, valve_type)
    
    def send_valve_control(self, valve_num, valve_type):
        """밸브 제어 CMD 0xA0 전송"""
        if not self.comm.is_connected:
            return
        
        try:
            # 모든 밸브 상태를 DATA FIELD로 구성 (20바이트)
            data_field = bytearray(20)
            
            # NOS 밸브 1~5 (DATA 1~5): 1=OPEN, 0=CLOSE
            for i in range(1, 6):
                data_field[i-1] = 0x01 if self.nos_valve_states.get(i, False) else 0x00
            
            # FEED 밸브 1~15 (DATA 6~20): 1=OPEN, 0=CLOSE
            for i in range(1, 16):
                data_field[5+i-1] = 0x01 if self.feed_valve_states.get(i, False) else 0x00
            
            # 클릭한 밸브만 토글
            if valve_type == 'NOS':
                current_state = self.nos_valve_states.get(valve_num, False)
                new_state_value = 0x00 if current_state else 0x01
                data_field[valve_num-1] = new_state_value
                self.nos_valve_states[valve_num] = not current_state
            else:  # FEED
                current_state = self.feed_valve_states.get(valve_num, False)
                new_state_value = 0x00 if current_state else 0x01
                data_field[5+valve_num-1] = new_state_value
                self.feed_valve_states[valve_num] = not current_state
            
            # CMD 0xA0 패킷 전송
            success, message = self.comm.send_packet(0xA0, bytes(data_field))
            
            if success:
                valve_name = f"{valve_type}{valve_num}"
                self.log_communication(f"[밸브 제어] {valve_name} 토글 전송 성공", "green")
            else:
                self.log_communication(f"[밸브 제어] 전송 실패: {message}", "red")
                
        except Exception as e:
            self.log_communication(f"밸브 제어 오류: {str(e)}", "red")
    
    def update_data(self, nos_states=None, feed_states=None):
        """데이터 업데이트"""
        if nos_states:
            self.nos_valve_states.update(nos_states)
        if feed_states:
            self.feed_valve_states.update(feed_states)
        self._update_gui()
    
    def _update_gui(self):
        """GUI 업데이트"""
        # NOS 밸브 상태 업데이트
        for valve_num, is_closed in self.nos_valve_states.items():
            if valve_num in self.nos_valve_labels:
                label = self.nos_valve_labels[valve_num]
                if is_closed:
                    label.config(text="CLOSE", bg="red")
                else:
                    label.config(text="OPEN", bg="blue")
            
            if hasattr(self, 'nos_valve_labels_control') and valve_num in self.nos_valve_labels_control:
                label = self.nos_valve_labels_control[valve_num]
                if is_closed:
                    label.config(text="CLOSE", bg="red")
                else:
                    label.config(text="OPEN", bg="blue")
        
        # FEED 밸브 상태 업데이트
        for valve_num, is_open in self.feed_valve_states.items():
            if valve_num in self.feed_valve_labels:
                label = self.feed_valve_labels[valve_num]
                if is_open:
                    label.config(text="OPEN", bg="blue")
                else:
                    label.config(text="CLOSE", bg="red")
            
            if hasattr(self, 'feed_valve_labels_control') and valve_num in self.feed_valve_labels_control:
                label = self.feed_valve_labels_control[valve_num]
                if is_open:
                    label.config(text="OPEN", bg="blue")
                else:
                    label.config(text="CLOSE", bg="red")
    
    def get_data(self):
        """현재 데이터 반환"""
        return {
            'nos_valve_states': self.nos_valve_states.copy(),
            'feed_valve_states': self.feed_valve_states.copy()
        }

