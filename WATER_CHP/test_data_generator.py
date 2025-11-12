"""
테스트용 데이터 생성기 - 시뮬레이션된 시리얼 데이터를 생성
"""
import random
import time
import threading
from communication import SerialCommunication


class TestDataGenerator:
    def __init__(self, output_callback=None):
        self.running = False
        self.output_callback = output_callback
        self.thread = None
        
        # 시뮬레이션 상태
        # NOS 밸브 상태 (1=CLOSE, 0=OPEN)
        self.nos_valve_states = {i: random.choice([0, 1]) for i in range(1, 6)}
        # FEED 밸브 상태 (1=OPEN, 0=CLOSE)
        self.feed_valve_states = {i: random.choice([0, 1]) for i in range(1, 16)}
        
        # 공조시스템 상태
        self.hvac_states = {
            'refrigerant_valve_state_1': random.choice(['핫가스', '냉각', '제빙']),
            'refrigerant_valve_state_2': random.choice(['핫가스', '냉각', '제빙']),
            'compressor_state': random.choice(['동작중', '미동작']),
            'current_rps': random.randint(0, 100),
            'target_rps': random.randint(0, 100),
            'error_code': 0,
            'dc_fan1': random.choice(['ON', 'OFF']),
            'dc_fan2': random.choice(['ON', 'OFF'])
        }
        
        # 냉각시스템 상태
        self.cooling_states = {
            'operation_state': random.choice(['GOING', 'STOP']),
            'on_temp': random.randint(5, 15),
            'off_temp': random.randint(3, 10),
            'cooling_additional_time': random.randint(0, 60)
        }
        
        # 제빙시스템 상태
        self.icemaking_states = {
            'operation': random.choice(['대기', '초기 핫가스', '예열', '트레이 상승', '입수 대기', '냉매 제빙 전환', '제빙중', '탈빙']),
            'icemaking_time': random.randint(0, 1800),
            'water_capacity': random.randint(0, 5000),
            'swing_on_time': random.randint(100, 300),
            'swing_off_time': random.randint(400, 800)
        }

        # 보냉시스템 상태
        self.refrigeration_states = {
            'operation': random.choice(['보냉대기', '보냉진행', '보냉완료', '만빙대기']),
            'refrigerant_valve_state': random.choice(['핫가스', '냉각', '제빙']),
            'target_rps': random.randint(0, 100),               ## 보냉진행 설정 RPS
            'target_temp': random.randint(-20, 100),            ## 보냉진행 설정온도
            'target_first_temp': random.randint(-20, 100),      ## 보냉진행 첫 온도
            'cur_tray_position': random.choice(['제빙', '중간', '탈빙']),
        }
        
        # 드레인탱크 상태
        self.drain_tank_states = {
            'low_level': random.choice(['감지', '미감지']),
            'high_level': random.choice(['감지', '미감지']),
            'water_level_state': random.choice(['만수위', '저수위', '비어있음'])
        }
        
        # 드레인펌프 상태
        self.drain_pump_states = {
            'operation_state': random.choice(['ON', 'OFF'])
        }
        
        # 센서 기준값
        self.base_outdoor_temp1 = 25.0
        self.base_outdoor_temp2 = 30.0
        self.base_purified_temp = 22.0
        self.base_cold_temp = 5.0
        self.base_hot_inlet_temp = 15.0
        self.base_hot_internal_temp = 80.0
        self.base_hot_outlet_temp = 85.0
        self.base_pressure1 = 150.0
        self.base_pressure2 = 200.0
    
    def start(self):
        """데이터 생성 시작"""
        if not self.running:
            self.running = True
            self.thread = threading.Thread(target=self._generate_data, daemon=True)
            self.thread.start()
    
    def stop(self):
        """데이터 생성 중지"""
        self.running = False
    
    def _generate_data(self):
        """데이터 생성 스레드"""
        while self.running:
            # 랜덤하게 NOS 밸브 상태 변경
            if random.random() < 0.50:  # 50% 확률로 NOS 밸브 상태 변경
                valve_num = random.randint(1, 5)
                self.nos_valve_states[valve_num] = 1 - self.nos_valve_states[valve_num]  # 0 <-> 1 토글
                
                nos_data = f"NOS{valve_num}:{self.nos_valve_states[valve_num]}"
                self._send_data(nos_data)
            
            # 랜덤하게 FEED 밸브 상태 변경
            if random.random() < 0.55:  # 55% 확률로 FEED 밸브 상태 변경
                valve_num = random.randint(1, 15)
                self.feed_valve_states[valve_num] = 1 - self.feed_valve_states[valve_num]  # 0 <-> 1 토글
                
                feed_data = f"FEED{valve_num}:{self.feed_valve_states[valve_num]}"
                self._send_data(feed_data)
            
            # 공조시스템 상태 변경
            if random.random() < 0.03:  # 3% 확률로 공조시스템 상태 변경
                component = random.choice(list(self.hvac_states.keys()))
                if component in ['refrigerant_valve_state_1', 'refrigerant_valve_state_2']:
                    self.hvac_states[component] = random.choice(['핫가스', '냉각', '제빙'])
                    self.hvac_states[component] = random.choice(['핫가스', '냉각', '제빙'])
                elif component == 'compressor_state':
                    self.hvac_states[component] = random.choice(['동작중', '미동작'])
                elif component in ['dc_fan1', 'dc_fan2']:
                    self.hvac_states[component] = random.choice(['ON', 'OFF'])
                elif component in ['current_rps', 'target_rps']:
                    self.hvac_states[component] = random.randint(0, 100)
                
                hvac_data = f"HVAC_{component.upper()}:{self.hvac_states[component]}"
                self._send_data(hvac_data)
            
            # 냉각시스템 상태 변경
            if random.random() < 0.02:  # 2% 확률로 냉각시스템 상태 변경
                component = random.choice(list(self.cooling_states.keys()))
                if component == 'operation_state':
                    self.cooling_states[component] = random.choice(['GOING', 'STOP'])
                elif component in ['on_temp', 'off_temp']:
                    self.cooling_states[component] = random.randint(3, 15)
                elif component == 'cooling_additional_time':
                    self.cooling_states[component] = random.randint(0, 60)
                
                cooling_data = f"COOLING_{component.upper()}:{self.cooling_states[component]}"
                self._send_data(cooling_data)
            
            # 제빙시스템 상태 변경
            if random.random() < 0.02:  # 2% 확률로 제빙시스템 상태 변경
                component = random.choice(list(self.icemaking_states.keys()))
                if component == 'operation':
                    self.icemaking_states[component] = random.choice(['대기', '초기 핫가스', '예열', '트레이 상승', '입수 대기', '냉매 제빙 전환', '제빙중', '탈빙'])
                elif component == 'icemaking_time':
                    self.icemaking_states[component] = random.randint(0, 1800)
                elif component == 'water_capacity':
                    self.icemaking_states[component] = random.randint(0, 5000)
                elif component in ['swing_on_time', 'swing_off_time']:
                    self.icemaking_states[component] = random.randint(100, 800)
                
                ice_data = f"ICE_{component.upper()}:{self.icemaking_states[component]}"
                self._send_data(ice_data)
            
            # 드레인 시스템 상태 변경
            if random.random() < 0.02:  # 2% 확률로 드레인 상태 변경
                if random.choice([True, False]):  # 탱크 또는 펌프
                    # 드레인 탱크
                    component = random.choice(list(self.drain_tank_states.keys()))
                    if component in ['low_level', 'high_level']:
                        self.drain_tank_states[component] = random.choice(['감지', '미감지'])
                    elif component == 'water_level_state':
                        self.drain_tank_states[component] = random.choice(['만수위', '저수위', '비어있음'])
                    
                    tank_data = f"DRAIN_TANK_{component.upper()}:{self.drain_tank_states[component]}"
                    self._send_data(tank_data)
                else:
                    # 드레인 펌프
                    self.drain_pump_states['operation_state'] = random.choice(['ON', 'OFF'])
                    pump_data = f"DRAIN_PUMP_OPERATION_STATE:{self.drain_pump_states['operation_state']}"
                    self._send_data(pump_data)
            
            # 센서 데이터 생성 (매번)
            outdoor_temp1 = self.base_outdoor_temp1 + random.uniform(-2, 2)
            outdoor_temp2 = self.base_outdoor_temp2 + random.uniform(-3, 3)
            purified_temp = self.base_purified_temp + random.uniform(-1, 1)
            cold_temp = self.base_cold_temp + random.uniform(-1, 1)
            hot_inlet_temp = self.base_hot_inlet_temp + random.uniform(-2, 2)
            hot_internal_temp = self.base_hot_internal_temp + random.uniform(-5, 5)
            hot_outlet_temp = self.base_hot_outlet_temp + random.uniform(-3, 3)
            pressure1 = self.base_pressure1 + random.uniform(-10, 10)
            pressure2 = self.base_pressure2 + random.uniform(-15, 15)
            
            sensor_data = (f"OUTDOOR_TEMP1:{outdoor_temp1:.1f} OUTDOOR_TEMP2:{outdoor_temp2:.1f} "
                          f"PURIFIED_TEMP:{purified_temp:.1f} COLD_TEMP:{cold_temp:.1f} "
                          f"HOT_INLET_TEMP:{hot_inlet_temp:.1f} HOT_INTERNAL_TEMP:{hot_internal_temp:.1f} "
                          f"HOT_OUTLET_TEMP:{hot_outlet_temp:.1f} "
                          f"PRESSURE1:{pressure1:.1f} PRESSURE2:{pressure2:.1f}")
            
            self._send_data(sensor_data)
            
            time.sleep(2.0)  # 2초마다 데이터 생성
    
    def _send_data(self, data):
        """데이터 출력"""
        if self.output_callback:
            self.output_callback(data)
        else:
            print(f"테스트 데이터: {data}")


# 독립 실행용
if __name__ == "__main__":
    def print_data(data):
        print(f"[{time.strftime('%H:%M:%S')}] {data}")
    
    generator = TestDataGenerator(print_data)
    
    print("테스트 데이터 생성기 시작... (Ctrl+C로 종료)")
    generator.start()
    
    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        print("\n테스트 데이터 생성기 종료")
        generator.stop()
