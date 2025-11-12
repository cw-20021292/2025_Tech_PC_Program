/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Main.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#include    "Macrodriver.h"
#include    "Global_Variable.h"
#include    "Port_Define.h"
#include    "test_uart_comm.h"
#include    "M8_Ice_Making.h"
#include    "M9_Front_Communication.h"

// 외부 변수 선언
extern U8 get_cold_mode_comp_rps(void);
extern U8 get_ice_mode_comp_rps(void);
extern U8 gu8BLDCErrorNum;
extern bit F_Cold_Operation_Init;
extern U8 gu8_uart_comp_rps;
extern U16 gu16_Ice_Tray_Fill_Hz;
extern U8 gu8_Cold_Temperature_One_Degree;
extern U8 gu8_Hot_Heater_Temperature_One_Degree;
extern U8 gu8_Hot_Out_Temperature_One_Degree;
extern bit bit_nos_output;
extern U8 u8RoomInValveON;
extern bit F_Drain_Pump_Output;
extern bit F_Tank_Cover_Input;
extern U16 gu16IceMakeTime;
extern U16 gu16CompOffDelay;
extern U8 gu8_bldc_target_hz;
extern U16 gu16_test_cold_on_temp;
extern U16 gu16_test_cold_off_temp;
extern U16 gu16_test_cold_delay_time;
extern U8 gu8_cristal_timer;
extern bit F_Safety_Routine;
extern U8 gu8IceLEV;
extern bit bit_cold_first_op;
extern bit F_IceInit;
void AT_UART_Communication(void);
void AT_UART_Rx_Process(void);
void AT_UART_Tx_Process(void);
void int_UART3_AT_TX(void);
void int_UART3_WORK_RX(void);

bit AT_F_TxStart;             //
bit AT_F_RxComplete;          //
bit AT_F_Rx_NG;               //

U8 AT_gu8TX_ERROR;
U8 AT_gu8TxData[255];
U8 AT_gu8RxData[255];
U8 AT_gu8TxdCounter;
U8 AT_gu8UARTStateMode;
U8 AT_gu8RxdCounter;

U16 AT_gu16_CMD_Mode;
U8 gu8UART_DataLength;
bit F_AT_TX_Finish;
bit F_Uart_Final;
U8 gu8RxdBufferData;

U8 gu8_uart_test_mode;

U8 gu8_uart_comp_start;
U8 gu8_uart_comp_rps;

/***********************************************************************************************************************
* Function Name: AT_UART_Communication
* Description  : UART 통신 처리
***********************************************************************************************************************/
void AT_UART_Communication(void)
{
    AT_UART_Rx_Process();         // rx 처리
    AT_UART_Tx_Process();         // tx 처리
}

/***********************************************************************************************************************
* Function Name: AT_UART_Rx_Process
* Description  : UART 수신 처리
***********************************************************************************************************************/
void AT_UART_Rx_Process(void)
{
    U8 u8cmd = 0;

    if(AT_F_RxComplete == CLEAR)
    {
        return;
    }
    else
    {
        AT_F_RxComplete = 0;
    }

    u8cmd = AT_gu8RxData[2];

    // WORK_CMD 泥섎━
    switch( u8cmd )
    {
        case WORK_CMD_HEARTBEAT:                    // 0x0F: Polling 데이터
            // Polling 데이터 처리
            break;

        case WORK_CMD_VALVE_CHANGE:                 // 0xA0: 밸브제어 변경
            // 밸브제어 처리
            break;

        case WORK_CMD_DRAIN_PUMP_CHANGE:            // 0xA1: 드레인 펌프 변경
            // 드레인 펌프 처리
            break;

        case WORK_CMD_COOLING_SYSTEM_CHANGE:        // 0xB0: 공조시스템 관련 데이터 변경
            // 공조시스템 관련 데이터 처리
            break;

        case WORK_CMD_COOLING_RUN_CHANGE:           // 0xB1: 냉각 관련 데이터 변경
            // 냉각 관련 데이터 처리

            break;

        case WORK_CMD_FREEZING_RUN_CHANGE:          // 0xB2: 제빙 관련 데이터 변경
            // ?젣鍮숈슫?쟾 蹂?寃? 泥섎━
            break;

        case WORK_CMD_FREEZING_TABLE_CHANGE:        // 0xB3: 제빙 테이블 변경
            SetFreezingTable(&AT_gu8RxData[5]);
            SetUsedFreezingTable(SET);
            break;

        case WORK_CMD_COOLING_TABLE_CHANGE:         // 0xB4: 냉각 테이블 변경
            // 냉각 테이블 처리
            break;

        case WORK_CMD_SENSOR_CHANGE:                // 0xC0: 센서 데이터 변경
            // 센서 데이터 처리
            break;

        default:
            // 잘못된 명령어
            break;
    }

    AT_F_TxStart = SET;
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
//------------------------
// 1-2 占쌜신븝옙 占쏙옙占쏙옙占쏙옙 占쏙옙환
//------------------------

// @TODO: 실제 변수 매핑 필요
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

    /* 공조시스템 (CMD 14-28) */
    U8 u8RefValve1Pos;          // CMD 14: 냉매전환밸브 1 현재위치 (0:핫가스, 1:냉각, 2:제빙, 3:보냉)
    U8 u8RefValve2Pos;          // CMD 15: 냉매전환밸브 2 현재위치 (병렬 구조)
    U8 u8CompOutputStatus;     // CMD 16: 압축기 출력상태 (1:가동, 0:정지)
    U8 u8CompStableTimeHigh;   // CMD 17: 압축기 안정시간 [HIGH] (초 단위)
    U8 u8CompStableTimeLow;    // CMD 18: 압축기 안정시간 [LOW] (초 단위)
    U8 u8CompCurrentRPS;       // CMD 19: 압축기 현재 RPS (37-75)
    U8 u8CompErrorCode;        // CMD 20: 압축기 에러코드 (E81~E88)
    U8 u8CompFanOutput;        // CMD 21: 압축기 팬 출력상태 (1:가동, 0:정지)
    U8 u8IceTankFanOutput;     // CMD 22: 얼음탱크 팬 출력상태 (1:가동, 0:정지)
    U8 u8Reserved_23;           // CMD 23: Reserved
    U8 u8Reserved_24;           // CMD 24: Reserved
    U8 u8Reserved_25;           // CMD 25: Reserved
    U8 u8Reserved_26;           // CMD 26: Reserved
    U8 u8Reserved_27;           // CMD 27: Reserved
    U8 u8Reserved_28;           // CMD 28: Reserved

    /* 냉각 데이터 (CMD 29-39) */
    U8 u8CoolingOpStatus;       // CMD 29: 운전상태 (1:운전, 0:정지)
    U8 u8CoolingInitStart;      // CMD 30: 초기 기동여부 (1:초기기동, 0:일반기동)
    U8 u8CoolingTargetRPS;      // CMD 31: 냉각용 목표 RPS (37-75)
    U8 u8CoolingOnTemp;         // CMD 32: ON 온도 (0.1℃ 단위)
    U8 u8CoolingOffTemp;        // CMD 33: OFF 온도 (0.1℃ 단위)
    U8 u8CoolingAddStartTime_H;   // CMD 34: 추가 기동시간 HIGH (ms)
    U8 u8CoolingAddStartTime_L;   // CMD 35: 추가 기동시간 LOW (ms)
    U8 u8Reserved_36;           // CMD 36: Reserved
    U8 u8Reserved_37;           // CMD 37: Reserved
    U8 u8Reserved_38;           // CMD 38: Reserved
    U8 u8Reserved_39;           // CMD 39: Reserved

    /* 제빙 데이터 (CMD 40-59) */
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
    U8 u8Reserved_50;           // CMD 50: Reserved
    U8 u8Reserved_51;           // CMD 51: Reserved
    U8 u8Reserved_52;           // CMD 52: Reserved
    U8 u8Reserved_53;           // CMD 53: Reserved
    U8 u8Reserved_54;           // CMD 54: Reserved
    U8 u8Reserved_55;           // CMD 55: Reserved
    U8 u8Reserved_56;           // CMD 56: Reserved
    U8 u8Reserved_57;           // CMD 57: Reserved
    U8 u8Reserved_58;           // CMD 58: Reserved
    U8 u8Reserved_59;           // CMD 59: Reserved

    /* 보냉 데이터 (CMD 60-74) */
    U8 u8KeepColdStep;          // CMD 60: 보냉 STEP
    U8 u8KeepColdTargetRPS;     // CMD 61: 보냉용 목표 RPS (37-75)
    U8 u8KeepColdTargetTemp;    // CMD 62: 보냉 목표온도 (0.1℃ 단위)
    U8 u8KeepColdFirstTargetTemp; // CMD 63: 보냉 첫 목표온도 (0.1℃ 단위)
    U8 u8KeepColdTrayPosition;  // CMD 64: 보냉 트레이 위치 (0:제빙, 1:중간, 2:탈빙)
    U8 u8Reserved_65;           // CMD 65: Reserved
    U8 u8Reserved_66;           // CMD 66: Reserved
    U8 u8Reserved_67;           // CMD 67: Reserved
    U8 u8Reserved_68;           // CMD 68: Reserved
    U8 u8Reserved_69;           // CMD 69: Reserved
    U8 u8Reserved_70;           // CMD 70: Reserved
    U8 u8Reserved_71;           // CMD 71: Reserved
    U8 u8Reserved_72;           // CMD 72: Reserved
    U8 u8Reserved_73;           // CMD 73: Reserved
    U8 u8Reserved_74;           // CMD 74: Reserved

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

    /* 드레인 탱크 (CMD 100-108) */
    U8 u8DrainTankLowLevel;     // CMD 100: 드레인탱크 저수위 (1:감지, 0:미감지)
    U8 u8DrainTankFullLevel;   // CMD 101: 드레인탱크 만수위 (1:감지, 0:미감지)
    U8 u8DrainWaterLevelStatus;  // CMD 102: 수위 상태 (0:없음, 1:저수위, 2:중수위, 3:만수위, 4:에러)
    U8 u8DrainPumpOutput;       // CMD 103: 드레인 펌프 출력상태 (1:가동, 0:정지)
    U8 u8Reserved_104;          // CMD 104: Reserved
    U8 u8Reserved_105;          // CMD 105: Reserved
    U8 u8Reserved_106;          // CMD 106: Reserved
    U8 u8Reserved_107;          // CMD 107: Reserved
    U8 u8Reserved_108;          // CMD 108: Reserved

    /* 기타 (CMD 109-114) */
    U8 u8IceTankCover;          // CMD 109: 얼음탱크 커버 (1:열림, 0:닫힘)
    U8 u8Reserved_110;          // CMD 110: Reserved
    U8 u8Reserved_111;          // CMD 111: Reserved
    U8 u8Reserved_112;          // CMD 112: Reserved
    U8 u8Reserved_113;          // CMD 113: Reserved
    U8 u8Reserved_114;          // CMD 114: Reserved
} AT_HEARTBEAT_DATA_FIELD;

/***********************************************************************************************************************
* Function Name: AT_FillHeartbeatDataField
* Description  : Heartbeat 데이터 필드 구조체에 실제 변수 값 채우기
* Return       : void
***********************************************************************************************************************/
void AT_FillHeartbeatDataField(AT_HEARTBEAT_DATA_FIELD *pstDataField)
{
    U8 mu8_i;

    /* 센서류 (CMD 1-13) */
    pstDataField->u8AmbTemp1 = gu8_Amb_Front_Temperature_One_Degree;        // CMD 1: 외기온도 1
    pstDataField->u8InletWaterTemp = gu8_Room_Temperature_One_Degree;      // CMD 2: 입수온도
    // @TODO: 실제 변수 매핑 필요
    pstDataField->u8PurifiedWaterTemp = 0;                                  // CMD 3: 정수온도
    pstDataField->u8AmbTemp2 = gu8_Amb_Temperature_One_Degree;                                           // CMD 4: 외기온도 2
    pstDataField->u8ColdWaterTemp = gu8_Cold_Temperature_One_Degree;       // CMD 5: 냉수온도
    pstDataField->u8HeaterInternalTemp = gu8_Hot_Heater_Temperature_One_Degree; // CMD 6: 히터 내부온도
    pstDataField->u8HotWaterOutletTemp = gu8_Hot_Out_Temperature_One_Degree; // CMD 7: 온수 출수온도
    pstDataField->u8Reserved_8 = 0;
    pstDataField->u8Reserved_9 = 0;
    pstDataField->u8Reserved_10 = 0;
    pstDataField->u8Reserved_11 = 0;
    pstDataField->u8Reserved_12 = 0;
    pstDataField->u8Reserved_13 = 0;

    /* 공조시스템 (CMD 14-28) */
    // TODO: 실제 변수 매핑 필요
    pstDataField->u8RefValve1Pos = gu8_GasSwitch_Status;                    // CMD 14: 냉매전환밸브 1 (0:핫가스, 1:냉각, 2:제빙, 3:보냉)
    pstDataField->u8RefValve2Pos = 0;                                        // CMD 15: 냉매전환밸브 2
    pstDataField->u8CompOutputStatus = ((F_Comp_Output == SET) ? 1 : 0);      // CMD 16: 압축기 출력상태
    pstDataField->u8CompStableTimeHigh = (U8)(gu16CompOffDelay >> 8);                                 // CMD 17: 압축기 안정시간 [HIGH]
    pstDataField->u8CompStableTimeLow = (U8)(gu16CompOffDelay);                                  // CMD 18: 압축기 안정시간 [LOW]
    pstDataField->u8CompCurrentRPS = gu8_bldc_target_hz;                      // CMD 19: 압축기 현재 RPS
    pstDataField->u8CompErrorCode = gu8BLDCErrorNum;                        // CMD 20: 압축기 에러코드
    pstDataField->u8CompFanOutput = pDC_FAN;                                      // CMD 21: 압축기 팬 출력상태
    pstDataField->u8IceTankFanOutput = 0;                                   // CMD 22: 얼음탱크 팬 출력상태
    pstDataField->u8Reserved_23 = 0;
    pstDataField->u8Reserved_24 = 0;
    pstDataField->u8Reserved_25 = 0;
    pstDataField->u8Reserved_26 = 0;
    pstDataField->u8Reserved_27 = 0;
    pstDataField->u8Reserved_28 = 0;

    /* 냉각 데이터 (CMD 29-39) */
    pstDataField->u8CoolingOpStatus = (U8)(Bit0_Cold_Make_Go);        // CMD 29: 운전상태
    pstDataField->u8CoolingInitStart = (U8)bit_cold_first_op; // CMD 30: 초기 기동여부
    pstDataField->u8CoolingTargetRPS = get_cold_mode_comp_rps();            // CMD 31: 냉각용 목표 RPS
    // TODO: 실제 변수 매핑 필요
    pstDataField->u8CoolingOnTemp = (U8)gu16_test_cold_on_temp;                                      // CMD 32: ON 온도
    pstDataField->u8CoolingOffTemp = (U8)gu16_test_cold_off_temp;                                     // CMD 33: OFF 온도
    pstDataField->u8CoolingAddStartTime_H = (U8)(gu16_test_cold_delay_time >> 8);                                // CMD 34: 추가 기동시간
    pstDataField->u8CoolingAddStartTime_L = (U8)(gu16_test_cold_delay_time);
    pstDataField->u8Reserved_36 = 0;
    pstDataField->u8Reserved_37 = 0;
    pstDataField->u8Reserved_38 = 0;
    pstDataField->u8Reserved_39 = 0;

    /* 제빙 데이터 (CMD 40-59) */
    if(F_IceInit == SET)
    {
        pstDataField->u8IceMakingStep = 255;
    }
    else
    {
        pstDataField->u8IceMakingStep = GetIceStep();                            // CMD 40: 제빙 STEP
    }

    pstDataField->u8IceMakingTargetRPS = get_ice_mode_comp_rps();           // CMD 41: 제빙용 목표 RPS
    // TODO: 실제 변수 매핑 필요
    pstDataField->u8IceMakingTimeHigh = (U8)((gu16IceMakeTime >> 8) & 0xFF); // CMD 42: 제빙시간 [HIGH]
    pstDataField->u8IceMakingTimeLow = (U8)(gu16IceMakeTime & 0xFF);         // CMD 43: 제빙시간 [LOW]
    pstDataField->u8InletWaterCapHigh = (U8)((gu16_Ice_Tray_Fill_Hz >> 8) & 0xFF); // CMD 44: 입수 용량 [HIGH]
    pstDataField->u8InletWaterCapLow = (U8)(gu16_Ice_Tray_Fill_Hz & 0xFF);   // CMD 45: 입수 용량 [LOW]
    pstDataField->u8SwingBarOnTime = gu8_cristal_timer;                                      // CMD 46: 스윙바 ON 시간
    pstDataField->u8SwingBarOffTime = gu8_cristal_timer;                                     // CMD 47: 스윙바 OFF 시간
    pstDataField->u8IceTrayPosition = gu8IceLEV;                                    // CMD 48: 제빙 트레이 위치
    pstDataField->u8IceJamStatus = F_Safety_Routine;                                       // CMD 49: 얼음걸림 상태
    for(mu8_i = 50; mu8_i <= 59; mu8_i++)
    {
        ((U8*)pstDataField)[mu8_i - 1] = 0;  // Reserved 50-59
    }

    /* 보냉 데이터 (CMD 60-74) */
    pstDataField->u8KeepColdStep = 0;                                       // CMD 60: 보냉 STEP
    pstDataField->u8KeepColdTargetRPS = 0;                                  // CMD 61: 보냉용 목표 RPS
    pstDataField->u8KeepColdTargetTemp = 0;                                // CMD 62: 보냉 목표온도
    pstDataField->u8KeepColdFirstTargetTemp = 0;                            // CMD 63: 보냉 첫 목표온도
    pstDataField->u8KeepColdTrayPosition = gu8IceLEV;                                // CMD 64: 보냉 트레이 위치
    for(mu8_i = 65; mu8_i <= 74; mu8_i++)
    {
        ((U8*)pstDataField)[mu8_i - 1] = 0;  // Reserved 65-74
    }

    /* 밸브 상태 (CMD 75-99) */
    // NOS 밸브: 1=CLOSE, 0=OPEN
    pstDataField->u8ValveNOS1 = pVALVE_NOS;                                 // CMD 75: 밸브 NOS 1
    pstDataField->u8ValveNOS2 = 1;                                           // CMD 76: 밸브 NOS 2
    pstDataField->u8ValveNOS3 = 1;                                           // CMD 77: 밸브 NOS 3
    pstDataField->u8ValveNOS4 = 1;                                           // CMD 78: 밸브 NOS 4
    pstDataField->u8ValveNOS5 = 1;                                           // CMD 79: 밸브 NOS 5
    // FEED 밸브: 1=OPEN, 0=CLOSE
    pstDataField->u8ValveFEED1 = pVALVE_ROOM_IN;                                          // CMD 80: 밸브 FEED 1
    pstDataField->u8ValveFEED2 = pVALVE_HOT_IN;                                          // CMD 81: 밸브 FEED 2
    pstDataField->u8ValveFEED3 = pVALVE_COLD_IN;                                          // CMD 82: 밸브 FEED 3
    pstDataField->u8ValveFEED4 = pVALVE_ICE_TRAY_IN;                                          // CMD 83: 밸브 FEED 4
    pstDataField->u8ValveFEED5 = pVALVE_HOT_DRAIN;                                          // CMD 84: 밸브 FEED 5
    pstDataField->u8ValveFEED6 = pVALVE_COLD_DRAIN;                                          // CMD 85: 밸브 FEED 6
    pstDataField->u8ValveFEED7 = pVALVE_HOT_COLD_OVERFLOW;                                          // CMD 86: 밸브 FEED 7
    pstDataField->u8ValveFEED8 = pVALVE_ROOM_COLD_EXTRACT;                                          // CMD 87: 밸브 FEED 8
    pstDataField->u8ValveFEED9 = pVALVE_HOT_OUT;                                          // CMD 88: 밸브 FEED 9
    pstDataField->u8ValveFEED10 = pVALVE_ICE_WATER_EXTRACT;                                         // CMD 89: 밸브 FEED 10
    pstDataField->u8ValveFEED11 = 0;                                         // CMD 90: 밸브 FEED 11
    pstDataField->u8ValveFEED12 = 0;                                         // CMD 91: 밸브 FEED 12
    pstDataField->u8ValveFEED13 = 0;                                         // CMD 92: 밸브 FEED 13
    pstDataField->u8ValveFEED14 = 0;                                         // CMD 93: 밸브 FEED 14
    pstDataField->u8ValveFEED15 = 0;                                         // CMD 94: 밸브 FEED 15
    for(mu8_i = 95; mu8_i <= 99; mu8_i++)
    {
        ((U8*)pstDataField)[mu8_i - 1] = 0;  // Reserved 95-99
    }

    /* 드레인 탱크 (CMD 100-108) */
    pstDataField->u8DrainTankLowLevel = (Bit0_Drain_Water_Empty == SET) ? 1 : 0;      // CMD 100: 드레인탱크 저수위
    pstDataField->u8DrainTankFullLevel = (Bit2_Drain_Water_High == SET) ? 1 : 0;     // CMD 101: 드레인탱크 만수위

    // 수위 상태: 0:없음, 1:저수위, 2:중수위, 3:만수위, 4:에러
    if(Bit3_Drain_Water_Error == SET)
    {
        pstDataField->u8DrainWaterLevelStatus = 4;                          // 에러
    }
    else if(Bit2_Drain_Water_High == SET)
    {
        pstDataField->u8DrainWaterLevelStatus = 3;                          // 만수위
    }
    else if(Bit1_Drain_Water_Low == SET)
    {
        pstDataField->u8DrainWaterLevelStatus = 2;                          // 중수위
    }
    else if(Bit0_Drain_Water_Empty == SET)
    {
        pstDataField->u8DrainWaterLevelStatus = 1;                          // 저수위
    }
    else
    {
        pstDataField->u8DrainWaterLevelStatus = 0;                           // 없음
    }
    pstDataField->u8DrainPumpOutput = (F_Drain_Pump_Output == SET) ? 1 : 0; // CMD 103: 드레인 펌프 출력상태
    for(mu8_i = 104; mu8_i <= 108; mu8_i++)
    {
        ((U8*)pstDataField)[mu8_i - 1] = 0;  // Reserved 104-108
    }

    /* 기타 (CMD 109-114) */
    pstDataField->u8IceTankCover = (F_Tank_Cover_Input == SET) ? 0 : 1;      // CMD 109: 얼음탱크 커버
    for(mu8_i = 110; mu8_i <= 114; mu8_i++)
    {
        ((U8*)pstDataField)[mu8_i - 1] = 0;  // Reserved 110-114
    }
}

/***********************************************************************************************************************
* Function Name: AT_ParseHeartbeatDataToBuffer
* Description  : Heartbeat 데이터 필드 구조체를 바이트 배열로 파싱
* Parameter    : pstDataField - 데이터 필드 구조체 포인터
*                pu8Buffer - 출력 버퍼 (최소 114바이트)
* Return       : void
***********************************************************************************************************************/
void AT_ParseHeartbeatDataToBuffer(AT_HEARTBEAT_DATA_FIELD *pstDataField, U8 *pu8Buffer)
{
    U8 mu8_i;

    // 구조체를 바이트 배열로 복사 (114바이트)
    for(mu8_i = 0; mu8_i < WORK_CMD_HEARTBEAT_LENGTH; mu8_i++)
    {
        pu8Buffer[mu8_i] = ((U8*)pstDataField)[mu8_i];
    }
}

void AT_UART_Tx_Process(void)
{
    U16 mu16_cal_crc = 0;
    AT_HEARTBEAT_DATA_FIELD stHeartbeatData;

    if(AT_F_TxStart == CLEAR)
    {
        return;
    }
    else
    {
        AT_F_TxStart = 0;
    }

    // 헤더 설정
    AT_gu8TxData[0] = WORK_STX;
    AT_gu8TxData[1] = WORK_ID_MAIN;
    AT_gu8TxData[2] = WORK_CMD_HEARTBEAT;
    AT_gu8TxData[3] = WORK_CMD_HEARTBEAT_LENGTH;

    // 데이터 필드 구조체에 값 채우기
    AT_FillHeartbeatDataField(&stHeartbeatData);

    // 구조체를 바이트 배열로 파싱하여 AT_gu8TxData에 복사
    AT_ParseHeartbeatDataToBuffer(&stHeartbeatData, &AT_gu8TxData[4]);

    // CRC 계산 범위: STX부터 DATA 끝까지 (CRC와 ETX 제외)
    // 패킷 구조: STX(1) + ID(1) + CMD(1) + LEN(1) + DATA(114) + CRC_HIGH(1) + CRC_LOW(1) + ETX(1)
    // CRC 계산: STX부터 DATA 끝까지 = 4 + WORK_CMD_HEARTBEAT_LENGTH = 4 + 114 = 118바이트
    // 수신 측과 동일: (전체 패킷 길이 - 3) = (LEN + WORK_PACKET_BASIC_LENGTH - 3) = (114 + 7 - 3) = 118
    // 수신 측 코드: Rx_CRC_CCITT(AT_gu8RxData, (AT_gu8RxdCounter-3))와 동일한 범위
    mu16_cal_crc = Rx_CRC_CCITT(AT_gu8TxData, (U8)(4 + WORK_CMD_HEARTBEAT_LENGTH));
    AT_gu8TxData[4 + WORK_CMD_HEARTBEAT_LENGTH] = (U8)HighByte(mu16_cal_crc);
    AT_gu8TxData[4 + WORK_CMD_HEARTBEAT_LENGTH + 1] = (U8)LowByte(mu16_cal_crc);
    AT_gu8TxData[4 + WORK_CMD_HEARTBEAT_LENGTH + 2] = WORK_ETX;

    TXD3 = AT_gu8TxData[AT_gu8TxdCounter];               // 첫 번째 바이트 전송 시작
    AT_gu8TxdCounter++;

    F_AT_TX_Finish = 1;
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
//------------------------------------------------------------------------------
//                      UART RXD (INTST1) - AutoTest
//------------------------------------------------------------------------------
void int_UART3_AT_TX(void)
{
//  TXD1=0x20;

    NOP();
    NOP();
    NOP();
    NOP();

    if(F_AT_TX_Finish == SET)
    {
        // 占쏙옙占쏙옙 占쏙옙占싶뤄옙 占식울옙 占쌕뤄옙 占쏙옙占쏙옙
        TXD3 = AT_gu8TxData[AT_gu8TxdCounter];

        if(AT_gu8TxdCounter < ((WORK_PACKET_BASIC_LENGTH + WORK_CMD_HEARTBEAT_LENGTH - 1)))
        {
            AT_gu8TxdCounter++;
        }
        else
        {
            AT_gu8TxdCounter = 0;
            F_AT_TX_Finish = 0;
        }
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
//------------------------------------------------------------------------------
//                      UART RXD (INTSR1) - AutoTest
//------------------------------------------------------------------------------
U16 u16RxDataDebug;

void int_UART3_WORK_RX(void)
{
    U8 err_type03;
    U16 mu16_cal_crc;
    mu16_cal_crc = 0;

    err_type03 = (U8)(SSR13 & 0x0007);
    SIR13 = (U16)err_type03;

    gu8RxdBufferData = RXD3;

    switch(AT_gu8UARTStateMode)
    {
        // 占쏙옙占?
        case UART_MODE_IDLE:

             if(gu8RxdBufferData == WORK_STX)
             {                 // STX check 0x01
                 AT_gu8RxdCounter = 0;
                 AT_gu8UARTStateMode = UART_MODE_RECEIVE;            // 0x01占쏙옙 占쏙옙占쏙옙占쏙옙 '占쏙옙占쏙옙占쏙옙'占쏙옙占쏙옙
                 AT_gu8RxData[AT_gu8RxdCounter++] = gu8RxdBufferData;// Stx 카占쏙옙트 0
             }
             else
             {
                 AT_gu8RxdCounter = 0;
             }

             break;

         // 占쏙옙占쏙옙占쏙옙
        case UART_MODE_RECEIVE:
            AT_gu8RxData[AT_gu8RxdCounter++] = gu8RxdBufferData;

            // ETX를 받았을 때 CRC 검증
            if(gu8RxdBufferData == WORK_ETX)
            {
                // 최소 패킷 길이 확인: STX(1) + TX_ID(1) + CMD(1) + LEN(1) + CRC_HIGH(1) + CRC_LOW(1) + ETX(1) = 7바이트
                if(AT_gu8RxdCounter >= 7)
                {
                    // 패킷 길이 확인
                    u16RxDataDebug = (AT_gu8RxData[3] + WORK_PACKET_BASIC_LENGTH);
                    if(AT_gu8RxdCounter == (AT_gu8RxData[3] + WORK_PACKET_BASIC_LENGTH))
                    {
                        mu16_cal_crc = Rx_CRC_CCITT(AT_gu8RxData, (AT_gu8RxdCounter-3));

                        // CRC_HIGH와 CRC_LOW 비교
                        if(AT_gu8RxData[AT_gu8RxdCounter-3] == (U8)HighByte(mu16_cal_crc)
                        && AT_gu8RxData[AT_gu8RxdCounter-2] == (U8)LowByte(mu16_cal_crc))
                        {
                            if(AT_gu8RxData[2] == 0xB3)
                            {
                                AT_gu8RxdCounter = 0;
                            }

                            AT_F_RxComplete = 1;
                            // Rx data initialize
                            AT_gu8RxdCounter = 0;
                            AT_gu8UARTStateMode = UART_MODE_IDLE;
                        }
                        else
                        {
                            // CRC 불일치: 버퍼 초기화하고 IDLE 모드로 전환
                            AT_F_RxComplete = 0;
                            AT_gu8RxdCounter = 0;
                            AT_gu8UARTStateMode = UART_MODE_IDLE;
                        }
                    }
                    else
                    {

                    }
                }
                else
                {
                    // 패킷 길이 부족: 버퍼 초기화하고 IDLE 모드로 전환
                    AT_F_RxComplete = 0;
                    AT_gu8RxdCounter = 0;
                    AT_gu8UARTStateMode = UART_MODE_IDLE;
                }
            }
            break;

        // Error
        case UART_MODE_ERROR:

             AT_gu8RxdCounter = 0;
             AT_gu8UARTStateMode = UART_MODE_IDLE;

             break;


        default:  // Rx data initialize //

             AT_gu8RxdCounter = 0;
             AT_gu8UARTStateMode = UART_MODE_IDLE;

             break;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/


