/**
 * File : App_Comm_Protocol.h
 *
 * Application Programming Interface
 * Depend on API
*/

#ifndef __APP_COMM_PROTOCOL_H__
#define __APP_COMM_PROTOCOL_H__

#include "api_uart.h"

#define COMM_PROTOCOL_STX        0x02
#define COMM_PROTOCOL_ETX        0x03
#define COMM_PROTOCOL_ID_PC      0x01
#define COMM_PROTOCOL_ID_MAIN    0x02
#define COMM_PROTOCOL_ID_FRONT   0x03

#define COMM_PROTOCOL_PACKET_BASIC_LENGTH          7

#define GET_16_HIGH_BYTE(val)       (U8)(((val)&0xFF00) >> 8)
#define GET_16_LOW_BYTE(val)        (U8)((val)&0x00FF)

typedef enum
{
    PROTOCOL_IDX_STX = 0,
    PROTOCOL_IDX_ID = 1,
    PROTOCOL_IDX_CMD = 2,
    PROTOCOL_IDX_LENGTH = 3,
    PROTOCOL_IDX_DATA = 4,
} PROTOCOL_INDEX;

/* 공통 시스템 프로토콜 (응답) */
typedef enum
{
    PROTOCOL_F0_CMD = 0xF0,
    PROTOCOL_F0_LENGTH = 40,
} PROTOCOL_F0_COMMON_SYSTEM;

/* 냉각 시스템 프로토콜 (응답) */
typedef enum
{
    PROTOCOL_F1_CMD = 0xF1,
    PROTOCOL_F1_LENGTH = 76,
} PROTOCOL_F1_COLD_SYSTEM;

/* 히팅 시스템 프로토콜 (응답) */
typedef enum
{
    PROTOCOL_F2_CMD = 0xF2,
    PROTOCOL_F2_LENGTH = 0,
} PROTOCOL_F2_HEATING_SYSTEM;

typedef enum
{
    PROTOCOL_B3_CMD = 0xB3,
    PROTOCOL_B3_LENGTH = 0,
} PROTOCOL_B3_FREEZING_TABLE;

typedef enum
{
    PROTOCOL_B1_CMD = 0xB1,
    PROTOCOL_B1_LENGTH = 5,
} PROTOCOL_B1_COLD_TABLE;

typedef enum
{
    PROTOCOL_B2_CMD = 0xB2,
    PROTOCOL_B2_LENGTH = 0,
} PROTOCOL_B2_ICE_MAKE_TABLE;

typedef enum
{
    PROTOCOL_B4_CMD = 0xB4,
    PROTOCOL_B4_LENGTH = 0,
} PROTOCOL_B4_ICE_STORAGE_TABLE;

/* 통신 데이터 F0 구조체 */
typedef struct
{
    /* 센서류 (CMD 1-13) */
    U8 u8AmbTemp1;              // CMD 1: 외기온도 1 (0.1℃ 단위)
    U8 u8InletWaterTemp;        // CMD 2: 입수온도 (0.1℃ 단위)
    U8 u8PurifiedWaterTemp;     // CMD 3: 정수온도 (0.1℃ 단위)
    U8 u8AmbTemp2;              // CMD 4: 외기온도 2 (0.1℃ 단위)
    U8 u8ColdWaterTemp;         // CMD 5: 냉수온도 (0.1℃ 단위)
    U8 u8HeaterInternalTemp;    // CMD 6: 히터 내부온도 (0.1℃ 단위)
    U8 u8HotWaterOutletTemp;    // CMD 7: 온수 출수온도 (0.1℃ 단위)
    U8 u8Reserved_8;             // CMD 8: Reserved
    U8 u8Reserved_9;             // CMD 9: Reserved
    U8 u8Reserved_10;            // CMD 10: Reserved
    U8 u8Reserved_11;            // CMD 11: Reserved
    U8 u8Reserved_12;            // CMD 12: Reserved
    U8 u8Reserved_13;            // CMD 13: Reserved

    /* 밸브 상태 (CMD 75-99) */
    U8 u8ValveNOS1;             // CMD 75: 밸브 NOS 1 상태 (1:CLOSE, 0:OPEN)
    U8 u8ValveNOS2;             // CMD 76: 밸브 NOS 2 상태 (1:CLOSE, 0:OPEN)
    U8 u8ValveNOS3;             // CMD 77: 밸브 NOS 3 상태 (1:CLOSE, 0:OPEN)
    U8 u8ValveNOS4;             // CMD 78: 밸브 NOS 4 상태 (1:CLOSE, 0:OPEN)
    U8 u8ValveNOS5;             // CMD 79: 밸브 NOS 5 상태 (1:CLOSE, 0:OPEN)
    U8 u8ValveFEED1;            // CMD 80: 밸브 FEED 1 상태 (1:OPEN, 0:CLOSE)
    U8 u8ValveFEED2;            // CMD 81: 밸브 FEED 2 상태 (1:OPEN, 0:CLOSE)
    U8 u8ValveFEED3;            // CMD 82: 밸브 FEED 3 상태 (1:OPEN, 0:CLOSE)
    U8 u8ValveFEED4;            // CMD 83: 밸브 FEED 4 상태 (1:OPEN, 0:CLOSE)
    U8 u8ValveFEED5;            // CMD 84: 밸브 FEED 5 상태 (1:OPEN, 0:CLOSE)
    U8 u8ValveFEED6;            // CMD 85: 밸브 FEED 6 상태 (1:OPEN, 0:CLOSE)
    U8 u8ValveFEED7;            // CMD 86: 밸브 FEED 7 상태 (1:OPEN, 0:CLOSE)
    U8 u8ValveFEED8;            // CMD 87: 밸브 FEED 8 상태 (1:OPEN, 0:CLOSE)
    U8 u8ValveFEED9;            // CMD 88: 밸브 FEED 9 상태 (1:OPEN, 0:CLOSE)
    U8 u8ValveFEED10;           // CMD 89: 밸브 FEED 10 상태 (1:OPEN, 0:CLOSE)
    U8 u8ValveFEED11;           // CMD 90: 밸브 FEED 11 상태 (1:OPEN, 0:CLOSE)
    U8 u8ValveFEED12;           // CMD 91: 밸브 FEED 12 상태 (1:OPEN, 0:CLOSE)
    U8 u8ValveFEED13;           // CMD 92: 밸브 FEED 13 상태 (1:OPEN, 0:CLOSE)
    U8 u8ValveFEED14;           // CMD 93: 밸브 FEED 14 상태 (1:OPEN, 0:CLOSE)
    U8 u8ValveFEED15;           // CMD 94: 밸브 FEED 15 상태 (1:OPEN, 0:CLOSE)
    U8 u8Reserved_95;           // CMD 95: Reserved
    U8 u8Reserved_96;           // CMD 96: Reserved
    U8 u8Reserved_97;           // CMD 97: Reserved
    U8 u8Reserved_98;           // CMD 98: Reserved
    U8 u8Reserved_99;           // CMD 99: Reserved

    U8 u8FilterReed;
    U8 u8FrontReed;
} F0_COMMON_SYSTEM_DATA_FIELD;

/* 통신 데이터 F1 구조체 */
typedef struct
{
    /* 공조시스템 (DATAFIELD 14-15) */
    U8 u8RefValve1Pos;          // CMD 14: 냉매전환밸브 1 현재위치 (0:핫가스, 1:냉각, 2:제빙, 3:보냉)
    U8 u8RefValve2Pos;          // CMD 15: 냉매전환밸브 2 현재위치 (병렬 구조)
    U8 u8CompOutputStatus;     // CMD 16: 압축기 출력상태 (1:가동, 0:정지)
    U8 u8CompStableTimeHigh;   // CMD 17: 압축기 안정시간 [HIGH] (초 단위)
    U8 u8CompStableTimeLow;    // CMD 18: 압축기 안정시간 [LOW] (초 단위)
    U8 u8CompCurrentRPS;       // CMD 19: 압축기 현재 RPS (37-75)
    U8 u8CompErrorCode;        // CMD 20: 압축기 에러코드 (E81~E88)
    U8 u8CompFanOutput;        // CMD 21: 압축기 팬 출력상태 (1:가동, 0:정지)
    U8 u8IceTankFanOutput;     // CMD 22: 얼음탱크 팬 출력상태 (1:가동, 0:정지)
    U8 u8Reserved_1;
    U8 u8Reserved_2;
    U8 u8Reserved_3;
    U8 u8Reserved_4;
    U8 u8Reserved_5;
    U8 u8Reserved_6;

    /* 냉각 데이터 (DATAFIELD 16-26) */
    U8 u8CoolingOpStatus;       // CMD 29: 운전상태 (1:운전, 0:정지)
    U8 u8CoolingInitStart;      // CMD 30: 초기 기동여부 (1:초기기동, 0:일반기동)
    U8 u8CoolingTargetRPS;      // CMD 31: 냉각용 목표 RPS (37-75)
    U8 u8CoolingOnTemp;         // CMD 32: ON 온도 (0.1℃ 단위)
    U8 u8CoolingOffTemp;        // CMD 33: OFF 온도 (0.1℃ 단위)
    U8 u8CoolingAddStartTime_H;   // CMD 34: 추가 기동시간 HIGH (ms)
    U8 u8CoolingAddStartTime_L;   // CMD 35: 추가 기동시간 LOW (ms)
    U8 u8Reserved_7;
    U8 u8Reserved_8;
    U8 u8Reserved_9;
    U8 u8Reserved_10;

    /* 제빙 데이터 (DATAFIELD 27-47) */
    U8 u8IceMakingStep;         // CMD 40: 제빙 STEP (0:더미탈빙, 1~:제빙STEP)
    U8 u8IceMakingTargetRPS;   // CMD 41: 제빙용 목표 RPS (37-75)
    U8 u8IceMakingTimeHigh;    // CMD 42: 제빙시간 [HIGH] (초 단위)
    U8 u8IceMakingTimeLow;     // CMD 43: 제빙시간 [LOW] (초 단위)
    U8 u8InletWaterCapHigh;    // CMD 44: 입수 용량 [HIGH] (Hz)
    U8 u8InletWaterCapLow;      // CMD 45: 입수 용량 [LOW] (Hz)
    U8 u8SwingBarOnTime;        // CMD 46: 스윙바 ON 시간 (0.1초)
    U8 u8SwingBarOffTime;       // CMD 47: 스윙바 OFF 시간 (0.1초)
    U8 u8IceTrayPosition;       // CMD 48: 제빙 트레이 위치 (0:제빙, 1:중간, 2:탈빙)
    U8 u8IceJamStatus;          // CMD 49: 얼음걸림 상태 (0:없음, 1:걸림)
    U8 u8Reserved_11;
    U8 u8Reserved_12;
    U8 u8Reserved_13;
    U8 u8Reserved_14;
    U8 u8Reserved_15;
    U8 u8Reserved_16;
    U8 u8Reserved_17;
    U8 u8Reserved_18;
    U8 u8Reserved_19;
    U8 u8Reserved_20;

    /* 보냉 데이터 (DATAFIELD 47-61) */
    U8 u8KeepColdStep;          // CMD 60: 보냉 STEP
    U8 u8KeepColdTargetRPS;     // CMD 61: 보냉용 목표 RPS (37-75)
    U8 u8KeepColdTargetTemp;    // CMD 62: 보냉 목표온도 (0.1℃ 단위)
    U8 u8KeepColdFirstTargetTemp; // CMD 63: 보냉 첫 목표온도 (0.1℃ 단위)
    U8 u8KeepColdTrayPosition;  // CMD 64: 보냉 트레이 위치 (0:제빙, 1:중간, 2:탈빙)
    U8 u8Reserved_21;
    U8 u8Reserved_22;
    U8 u8Reserved_23;
    U8 u8Reserved_24;
    U8 u8Reserved_25;
    U8 u8Reserved_26;
    U8 u8Reserved_27;
    U8 u8Reserved_28;
    U8 u8Reserved_29;
    U8 u8Reserved_30;

    /* 드레인 탱크 (DATAFIELD 62-70) */
    U8 u8DrainTankLowLevel;     // CMD 100: 드레인탱크 저수위 (1:감지, 0:미감지)
    U8 u8DrainTankFullLevel;   // CMD 101: 드레인탱크 만수위 (1:감지, 0:미감지)
    U8 u8DrainWaterLevelStatus;  // CMD 102: 수위 상태 (0:없음, 1:저수위, 2:중수위, 3:만수위, 4:에러)
    U8 u8DrainPumpOutput;       // CMD 103: 드레인 펌프 출력상태 (1:가동, 0:정지)
    U8 u8Reserved_31;
    U8 u8Reserved_32;
    U8 u8Reserved_33;
    U8 u8Reserved_34;
    U8 u8Reserved_35;

    /* 기타 (DATAFIELD 71-74) */
    U8 u8IceTankCover;          // DATA 71: 얼음탱크 커버 (1:열림, 0:닫힘)
    U8 u8Reserved_36;
    U8 u8Reserved_37;
    U8 u8Reserved_38;
    U8 u8Reserved_39;
    U8 u8Reserved_40;
} F1_COLD_SYSTEM_DATA_FIELD;

/* 통신 데이터 F2 구조체 */
typedef struct
{
    U8 u8HeatingOpStatus;
} F2_HEATING_SYSTEM_DATA_FIELD;

typedef struct
{
    U8 u8ColdTargetRPS;    // 냉각 목표 RPS
    U8 u8ColdOnTemp;       // 냉각 ON 온도
    U8 u8ColdOffTemp;      // 냉각 OFF 온도
    U16 u16ColdDelayTime;   // 냉각 추가기동 시간
} B1_COLD_TABLE_DATA_FIELD;

typedef struct
{
    U8 u8IceMakeTargetRPS;  // 제빙 목표 RPS
    U16 u16TrayIn_Hz;       // 입수용량(Hz)
    U8 u8SwingbarOn;        // 스윙바 ON 시간
    U8 u8SwingbarOff;       // 스윙바 OFF 시간
} B2_ICE_MAKE_TABLE_DATA_FIELD;

typedef struct
{
    U8 u8IceStorageTargetRPS;  // 보냉 목표 RPS
    U8 u8TargetTemp;           // 보냉 목표온도
    U8 u8TargetTempFirst;      // 보냉 첫 목표온도
    U8 u8TrayPosition;         // 보냉 트레이 위치
} B4_ICE_STORAGE_TABLE_DATA_FIELD;

U8 Protocol_isValidPacket(U8 *buf);
U8 Protocol_Make_Ack_Packet(U8 *buf, U8 *tx_buf);

U8 GetB1ColdTargetRPS(void);
void SetB1ColdTargetRPS(U8 *u8ColdTargetRPS);
U8 GetB1ColdOnTemp(void);
void SetB1ColdOnTemp(U8 *u8ColdOnTemp);
U8 GetB1ColdOffTemp(void);
void SetB1ColdOffTemp(U8 *u8ColdOffTemp);
U16 GetB1ColdDelayTime(void);
void SetB1ColdDelayTime(U16 *u16ColdDelayTime);

U8 GetB2IceMakeTargetRPS(void);
void SetB2IceMakeTargetRPS(U8 *u8IceMakeTargetRPS);
U16 GetB2TrayIn_Hz(void);
void SetB2TrayIn_Hz(U16 *u16TrayIn_Hz);
U8 GetB2SwingbarOn(void);
void SetB2SwingbarOn(U8 *u8SwingbarOn);
U8 GetB2SwingbarOff(void);
void SetB2SwingbarOff(U8 *u8SwingbarOff);

U8 GetB4IceStorageTargetRPS(void);
void SetB4IceStorageTargetRPS(U8 *u8IceStorageTargetRPS);
U8 GetB4TargetTemp(void);
void SetB4TargetTemp(U8 *u8TargetTemp);
U8 GetB4TargetTempFirst(void);
void SetB4TargetTempFirst(U8 *u8TargetTempFirst);
U8 GetB4TrayPosition(void);
void SetB4TrayPosition(U8 *u8TrayPosition);


#endif /*__APP_COMM_PROTOCOL_H__*/

