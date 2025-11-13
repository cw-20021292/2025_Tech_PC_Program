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

bit F_AT_TX_Finish;
U8 gu8RxdBufferData;

U8 gu8_uart_test_mode;

/* 통신 구조체 */
typedef struct
{
    U16 u16TxMode;
    U8 u8TxData[255];
    U8 u8TxCounter;

    RX_MODE u8RxStatus;
    U8 u8RxData[255];
    U8 u8RxCounter;
} COMM_T;
COMM_T Comm;

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

F0_COMMON_SYSTEM_DATA_FIELD stF0Data;
F1_COLD_SYSTEM_DATA_FIELD stF1Data;
F2_HEATING_SYSTEM_DATA_FIELD stF2Data;

void Parse_F0_Protocol(F0_COMMON_SYSTEM_DATA_FIELD *p_F0_DataField)
{
    /* 센서류 (DATAFIELD 1-13) */
    p_F0_DataField->u8AmbTemp1 = gu8_Amb_Front_Temperature_One_Degree;        // CMD 1: 외기온도 1
    p_F0_DataField->u8InletWaterTemp = gu8_Room_Temperature_One_Degree;      // CMD 2: 입수온도
    p_F0_DataField->u8PurifiedWaterTemp = 0;                                  // CMD 3: 정수온도
    p_F0_DataField->u8AmbTemp2 = gu8_Amb_Temperature_One_Degree;                                           // CMD 4: 외기온도 2
    p_F0_DataField->u8ColdWaterTemp = gu8_Cold_Temperature_One_Degree;       // CMD 5: 냉수온도
    p_F0_DataField->u8HeaterInternalTemp = gu8_Hot_Heater_Temperature_One_Degree; // CMD 6: 히터 내부온도
    p_F0_DataField->u8HotWaterOutletTemp = gu8_Hot_Out_Temperature_One_Degree; // CMD 7: 온수 출수온도

    /* 밸브 상태 (DATAFIELD 14-38) */
    // NOS 밸브: 1=CLOSE, 0=OPEN
    p_F0_DataField->u8ValveNOS1 = pVALVE_NOS;                                 // CMD 75: 밸브 NOS 1
    p_F0_DataField->u8ValveNOS2 = 1;                                           // CMD 76: 밸브 NOS 2
    p_F0_DataField->u8ValveNOS3 = 1;                                           // CMD 77: 밸브 NOS 3
    p_F0_DataField->u8ValveNOS4 = 1;                                           // CMD 78: 밸브 NOS 4
    p_F0_DataField->u8ValveNOS5 = 1;                                           // CMD 79: 밸브 NOS 5
    // FEED 밸브: 1=OPEN, 0=CLOSE
    p_F0_DataField->u8ValveFEED1 = pVALVE_ROOM_IN;                                          // CMD 80: 밸브 FEED 1
    p_F0_DataField->u8ValveFEED2 = pVALVE_HOT_IN;                                          // CMD 81: 밸브 FEED 2
    p_F0_DataField->u8ValveFEED3 = pVALVE_COLD_IN;                                          // CMD 82: 밸브 FEED 3
    p_F0_DataField->u8ValveFEED4 = pVALVE_ICE_TRAY_IN;                                          // CMD 83: 밸브 FEED 4
    p_F0_DataField->u8ValveFEED5 = pVALVE_HOT_DRAIN;                                          // CMD 84: 밸브 FEED 5
    p_F0_DataField->u8ValveFEED6 = pVALVE_COLD_DRAIN;                                          // CMD 85: 밸브 FEED 6
    p_F0_DataField->u8ValveFEED7 = pVALVE_HOT_COLD_OVERFLOW;                                          // CMD 86: 밸브 FEED 7
    p_F0_DataField->u8ValveFEED8 = pVALVE_ROOM_COLD_EXTRACT;                                          // CMD 87: 밸브 FEED 8
    p_F0_DataField->u8ValveFEED9 = pVALVE_HOT_OUT;                                          // CMD 88: 밸브 FEED 9
    p_F0_DataField->u8ValveFEED10 = pVALVE_ICE_WATER_EXTRACT;                                         // CMD 89: 밸브 FEED 10
    p_F0_DataField->u8ValveFEED11 = 0;                                         // CMD 90: 밸브 FEED 11
    p_F0_DataField->u8ValveFEED12 = 0;                                         // CMD 91: 밸브 FEED 12
    p_F0_DataField->u8ValveFEED13 = 0;                                         // CMD 92: 밸브 FEED 13
    p_F0_DataField->u8ValveFEED14 = 0;                                         // CMD 93: 밸브 FEED 14
    p_F0_DataField->u8ValveFEED15 = 0;                                         // CMD 94: 밸브 FEED 15
}

void Parse_F1_Protocol(F1_COLD_SYSTEM_DATA_FIELD *p_F1_DataField)
{
    /* 공조시스템 (DATAFIELD 1-13) */
    p_F1_DataField->u8RefValve1Pos = gu8_GasSwitch_Status;
    p_F1_DataField->u8RefValve2Pos = 0;
    p_F1_DataField->u8CompOutputStatus = ((F_Comp_Output == SET) ? 1 : 0);
    p_F1_DataField->u8CompStableTimeHigh = (U8)(gu16CompOffDelay >> 8);
    p_F1_DataField->u8CompStableTimeLow = (U8)(gu16CompOffDelay);
    p_F1_DataField->u8CompCurrentRPS = gu8_bldc_target_hz;
    p_F1_DataField->u8CompErrorCode = gu8BLDCErrorNum;
    p_F1_DataField->u8CompFanOutput = pDC_FAN;
    p_F1_DataField->u8IceTankFanOutput = 0;

    /* 냉각 데이터 (DATAFIELD 14-26) */
    p_F1_DataField->u8CoolingOpStatus = (U8)(Bit0_Cold_Make_Go);
    p_F1_DataField->u8CoolingInitStart = (U8)bit_cold_first_op;
    p_F1_DataField->u8CoolingTargetRPS = get_cold_mode_comp_rps();
    p_F1_DataField->u8CoolingOnTemp = (U8)gu16_test_cold_on_temp;
    p_F1_DataField->u8CoolingOffTemp = (U8)gu16_test_cold_off_temp;
    p_F1_DataField->u8CoolingAddStartTime_H = (U8)(gu16_test_cold_delay_time >> 8);
    p_F1_DataField->u8CoolingAddStartTime_L = (U8)(gu16_test_cold_delay_time);

    /* 제빙 데이터 (DATAFIELD 27-47) */
    if(F_IceInit == SET)
    {
        p_F1_DataField->u8IceMakingStep = 255;
    }
    else
    {
        p_F1_DataField->u8IceMakingStep = GetIceStep();
    }

    p_F1_DataField->u8IceMakingTargetRPS = get_ice_mode_comp_rps();
    p_F1_DataField->u8IceMakingTimeHigh = (U8)((gu16IceMakeTime >> 8) & 0xFF); // CMD 42: 제빙시간 [HIGH]
    p_F1_DataField->u8IceMakingTimeLow = (U8)(gu16IceMakeTime & 0xFF);         // CMD 43: 제빙시간 [LOW]
    p_F1_DataField->u8InletWaterCapHigh = (U8)((gu16_Ice_Tray_Fill_Hz >> 8) & 0xFF); // CMD 44: 입수 용량 [HIGH]
    p_F1_DataField->u8InletWaterCapLow = (U8)(gu16_Ice_Tray_Fill_Hz & 0xFF);   // CMD 45: 입수 용량 [LOW]
    p_F1_DataField->u8SwingBarOnTime = 2;                                      // CMD 46: 스윙바 ON 시간
    p_F1_DataField->u8SwingBarOffTime = 6;                                     // CMD 47: 스윙바 OFF 시간
    p_F1_DataField->u8IceTrayPosition = gu8IceLEV;                                    // CMD 48: 제빙 트레이 위치
    p_F1_DataField->u8IceJamStatus = F_Safety_Routine;

    /* 보냉 데이터 (DATAFIELD 48-62) */
    p_F1_DataField->u8KeepColdStep = 0;
    p_F1_DataField->u8KeepColdTargetRPS = 0;
    p_F1_DataField->u8KeepColdTargetTemp = 0;
    p_F1_DataField->u8KeepColdFirstTargetTemp = 0;
    p_F1_DataField->u8KeepColdTrayPosition = gu8IceLEV;

    /* 드레인 탱크 (DATAFIELD 63-71) */
    p_F1_DataField->u8DrainTankLowLevel = (Bit0_Drain_Water_Empty == SET) ? 1 : 0;
    p_F1_DataField->u8DrainTankFullLevel = (Bit2_Drain_Water_High == SET) ? 1 : 0;
    p_F1_DataField->u8DrainWaterLevelStatus = u8DrainWaterLevel;
    p_F1_DataField->u8DrainPumpOutput = F_Drain_Pump_Output;

    /* 기타 (DATAFIELD 72-74) */
    p_F1_DataField->u8IceTankCover = F_Tank_Cover_Input;
}

void Parse_F2_Protocol(F2_HEATING_SYSTEM_DATA_FIELD *pstDataField)
{

}

/***********************************************************************************************************************
* Function Name: AT_UART_Communication
* Description  : UART 통신 처리
***********************************************************************************************************************/
void AT_UART_Communication(void)
{
    AT_UART_Rx_Process();         // rx 처리
    AT_UART_Tx_Process();         // tx 처리
}

void SetTxMode(U16 u16TxMode)
{
    Comm.u16TxMode |= u16TxMode;
}

void ClearTxMode(U16 u16TxMode)
{
    Comm.u16TxMode &= ~u16TxMode;
}

U16 CheckTxMode_Queue(void)
{
    U8 i = 0;
    for(i = 0; i < 16; i++)
    {
        if(Comm.u16TxMode & (1 << i))
        {
           return (Comm.u16TxMode & (1 << i));
        }
    }

    return 0;
}

U8 ConvertTxModeToCmd(U16 u16TxMode)
{
    U8 u8Cmd = 0;

    switch(u16TxMode)
    {
        case TX_CMD_F0:
            u8Cmd = PROTOCOL_F0_CMD;
            break;

        case TX_CMD_F1:
        case TX_CMD_B3:
            u8Cmd = PROTOCOL_F1_CMD;
            break;

        default:
            u8Cmd = 0;
            break;
    }
    return u8Cmd;
}

U8 ConvertTxModeToLength(U16 u16TxMode)
{
    U8 u8Length = 0;
    switch(u16TxMode)
    {
        case TX_CMD_F0:
            u8Length = PROTOCOL_F0_LENGTH;
            break;
        case TX_CMD_F1:
        case TX_CMD_B3:
            u8Length = PROTOCOL_F1_LENGTH;
            break;

        default:
            u8Length = 0;
            break;
    }
    return u8Length;
}

void Parse_DataField(U16 u16TxMode, U8 *TxData)
{
    U8 u8DataIndex = 0;

    switch(u16TxMode)
    {
        case TX_CMD_F0:
            // F0 구조체에 데이터 채우기
            Parse_F0_Protocol(&stF0Data);

            // F0 구조체를 바이트 배열로 변환하여 TxData에 복사
            for(u8DataIndex = 0; u8DataIndex < PROTOCOL_F0_LENGTH; u8DataIndex++)
            {
                TxData[u8DataIndex] = ((U8*)&stF0Data)[u8DataIndex];
            }
            break;

        case TX_CMD_F1:
        case TX_CMD_B3:
            // F1 구조체에 데이터 채우기
            Parse_F1_Protocol(&stF1Data);

            // F1 구조체를 바이트 배열로 변환하여 TxData에 복사
            for(u8DataIndex = 0; u8DataIndex < PROTOCOL_F1_LENGTH; u8DataIndex++)
            {
                TxData[u8DataIndex] = ((U8*)&stF1Data)[u8DataIndex];
            }
            break;

        default:
            break;
    }
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

    u8cmd = Comm.u8RxData[2];

    // WORK_CMD 泥섎━
    switch( u8cmd )
    {
        case PROTOCOL_F0_CMD:                    // 0xF0: 공통 시스템 데이터
            // 공통 시스템 데이터 처리
            SetTxMode(TX_CMD_F0);
            break;

        case PROTOCOL_F1_CMD:                    // 0xF1: 냉각 시스템 데이터
            // 냉각 시스템 데이터 처리
            SetTxMode(TX_CMD_F1);
            break;

            case PROTOCOL_B3_CMD:        // 0xB3: 제빙 테이블 변경
            SetFreezingTable(&Comm.u8RxData[5]);
            SetUsedFreezingTable(SET);
            SetTxMode(TX_CMD_F1);
            break;

        default:
            // 잘못된 명령어
            SetTxMode(TX_CMD_F0);
            break;
    }

    AT_F_TxStart = SET;
}

/*****************************************************************************
* Function Name: AT_UART_Tx_Process
* Description  : UART 통신 처리
* Parameter    : void
* Return       : void
*****************************************************************************/
U16 u16DebugTxMode = 0;
void AT_UART_Tx_Process(void)
{
    U16 mu16_cal_crc = 0;
    U8 mu8_i = 0;
    U8 mu8_cmd = 0;
    U8 mu8_data_length = 0;
    U16 mu16_tx_mode = 0;

    if(AT_F_TxStart == CLEAR)
    {
        return;
    }
    else
    {
        AT_F_TxStart = 0;
    }

    if(CheckTxMode_Queue() == FALSE)
    {
        return;
    }
    else
    {
        mu16_tx_mode = CheckTxMode_Queue();
        u16DebugTxMode = mu16_tx_mode;
        ClearTxMode(mu16_tx_mode);
    }

    // 수신된 CMD 확인
    mu8_cmd = Comm.u8RxData[2];

    // 헤더 설정
    Comm.u8TxData[0] = WORK_STX;
    Comm.u8TxData[1] = WORK_ID_MAIN;
    Comm.u8TxData[2] = ConvertTxModeToCmd(mu16_tx_mode);             // CMD 생성
    Comm.u8TxData[3] = ConvertTxModeToLength(mu16_tx_mode);          // LEN 생성
    mu8_data_length = ConvertTxModeToLength(mu16_tx_mode);
    Parse_DataField(mu16_tx_mode, &Comm.u8TxData[4]);

    // CRC 계산 범위: STX부터 DATA 끝까지 (CRC와 ETX 제외)
    // 패킷 구조: STX(1) + ID(1) + CMD(1) + LEN(1) + DATA(N) + CRC_HIGH(1) + CRC_LOW(1) + ETX(1)
    // CRC 계산: STX부터 DATA 끝까지 = 4 + mu8_data_length
    mu16_cal_crc = Rx_CRC_CCITT(Comm.u8TxData, (U8)(4 + mu8_data_length));
    Comm.u8TxData[4 + mu8_data_length] = (U8)HighByte(mu16_cal_crc);
    Comm.u8TxData[4 + mu8_data_length + 1] = (U8)LowByte(mu16_cal_crc);
    Comm.u8TxData[4 + mu8_data_length + 2] = WORK_ETX;

    TXD3 = Comm.u8TxData[Comm.u8TxCounter];               // 첫 번째 바이트 전송 시작
    Comm.u8TxCounter++;

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
        // 바이트 단위로 전송
        TXD3 = Comm.u8TxData[Comm.u8TxCounter];

        // 패킷 길이 계산: WORK_PACKET_BASIC_LENGTH(7) + 데이터 길이(Comm.u8TxData[3])
        // 전체 패킷: STX(1) + ID(1) + CMD(1) + LEN(1) + DATA(N) + CRC_HIGH(1) + CRC_LOW(1) + ETX(1)
        // 마지막 인덱스 = 7 + N - 1 = 6 + N
        if(Comm.u8TxCounter < ((WORK_PACKET_BASIC_LENGTH + Comm.u8TxData[3] - 1)))
        {
            Comm.u8TxCounter++;
        }
        else
        {
            Comm.u8TxCounter = 0;
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

    switch(Comm.u8RxStatus)
    {
        // 占쏙옙占?
        case UART_MODE_IDLE:

            if(gu8RxdBufferData == WORK_STX)
            {                 // STX check 0x01
                Comm.u8RxCounter = 0;
                Comm.u8RxStatus = UART_MODE_RECEIVE;            // 0x01占쏙옙 占쏙옙占쏙옙占쏙옙 '占쏙옙占쏙옙占쏙옙'占쏙옙占쏙옙
                Comm.u8RxData[Comm.u8RxCounter++] = gu8RxdBufferData;// Stx 카占쏙옙트 0
            }
            else
            {
                Comm.u8RxCounter = 0;
            }

             break;

         // 占쏙옙占쏙옙占쏙옙
        case UART_MODE_RECEIVE:
            Comm.u8RxData[Comm.u8RxCounter++] = gu8RxdBufferData;

            // ETX를 받았을 때 CRC 검증
            if(gu8RxdBufferData == WORK_ETX)
            {
                // 최소 패킷 길이 확인: STX(1) + TX_ID(1) + CMD(1) + LEN(1) + CRC_HIGH(1) + CRC_LOW(1) + ETX(1) = 7바이트
                if(Comm.u8RxCounter >= 7)
                {
                    // 패킷 길이 확인
                    u16RxDataDebug = (Comm.u8RxData[3] + WORK_PACKET_BASIC_LENGTH);
                    if(Comm.u8RxCounter == (Comm.u8RxData[3] + WORK_PACKET_BASIC_LENGTH))
                    {
                        mu16_cal_crc = Rx_CRC_CCITT(Comm.u8RxData, (Comm.u8RxCounter-3));

                        // CRC_HIGH와 CRC_LOW 비교
                        if(Comm.u8RxData[Comm.u8RxCounter-3] == (U8)HighByte(mu16_cal_crc)
                        && Comm.u8RxData[Comm.u8RxCounter-2] == (U8)LowByte(mu16_cal_crc))
                        {
                            if(Comm.u8RxData[2] == 0xB3)
                            {
                                Comm.u8RxCounter = 0;
                            }

                            AT_F_RxComplete = 1;
                            // Rx data initialize
                            Comm.u8RxCounter = 0;
                            Comm.u8RxStatus = UART_MODE_IDLE;
                        }
                        else
                        {
                            // CRC 불일치: 버퍼 초기화하고 IDLE 모드로 전환
                            AT_F_RxComplete = 0;
                            Comm.u8RxCounter = 0;
                            Comm.u8RxStatus = UART_MODE_IDLE;
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
                    Comm.u8RxCounter = 0;
                    Comm.u8RxStatus = UART_MODE_IDLE;
                }
            }
            break;

        // Error
        case UART_MODE_ERROR:

             Comm.u8RxCounter = 0;
             Comm.u8RxStatus = UART_MODE_IDLE;

             break;


        default:  // Rx data initialize //

             Comm.u8RxCounter = 0;
             Comm.u8RxStatus = UART_MODE_IDLE;

             break;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/


