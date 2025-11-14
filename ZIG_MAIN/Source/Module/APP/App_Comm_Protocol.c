/**
 * File : App_Comm_Protocol.c
 *
 * Application Programming Interface
 * Depend on API
*/

#include "App_Comm_Protocol.h"

#include    "Macrodriver.h"
#include    "Global_Variable.h"
#include    "Port_Define.h"
#include    "M8_Ice_Making.h"
#include    "M9_Front_Communication.h"

#if 1
/* @TODO : 유저용) 외부 변수 참조 및 헤더 참조 */
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
extern bit bit_filter_reed;
extern bit bit_filter_cover;
#endif

F0_COMMON_SYSTEM_DATA_FIELD F0Data = {0, };
F1_COLD_SYSTEM_DATA_FIELD F1Data = {0, };
F2_HEATING_SYSTEM_DATA_FIELD F2Data = {0, };
B1_COLD_TABLE_DATA_FIELD B1Data = {0, };
B2_ICE_MAKE_TABLE_DATA_FIELD B2Data = {0, };
B4_ICE_STORAGE_TABLE_DATA_FIELD B4Data = {0, };

/***************************************************************************************/
/**
 * @brief 커맨드 F0의 프로토콜 데이터필드
 * @details 커맨드 F0의 프로토콜 데이터필드를 사용자의 변수를 채우는 함수
 */
static void Parse_F0_Protocol(F0_COMMON_SYSTEM_DATA_FIELD *p_F0_DataField)
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
    p_F0_DataField->u8ValveNOS2 = pVALVE_HOT_DRAIN;                                           // CMD 76: 밸브 NOS 2
    p_F0_DataField->u8ValveNOS3 = pVALVE_COLD_DRAIN;                                           // CMD 77: 밸브 NOS 3
    p_F0_DataField->u8ValveNOS4 = pVALVE_HOT_COLD_OVERFLOW;                                           // CMD 78: 밸브 NOS 4
    p_F0_DataField->u8ValveNOS5 = 1;                                           // CMD 79: 밸브 NOS 5
    // FEED 밸브: 1=OPEN, 0=CLOSE
    p_F0_DataField->u8ValveFEED1 = pVALVE_ROOM_IN;                                          // CMD 80: 밸브 FEED 1
    p_F0_DataField->u8ValveFEED2 = pVALVE_HOT_IN;                                          // CMD 81: 밸브 FEED 2
    p_F0_DataField->u8ValveFEED3 = pVALVE_COLD_IN;                                          // CMD 82: 밸브 FEED 3
    p_F0_DataField->u8ValveFEED4 = pVALVE_ICE_TRAY_IN;                                          // CMD 83: 밸브 FEED 4
    p_F0_DataField->u8ValveFEED5 = pVALVE_ROOM_COLD_EXTRACT;                                          // CMD 84: 밸브 FEED 5
    p_F0_DataField->u8ValveFEED6 = pVALVE_HOT_OUT;                                          // CMD 85: 밸브 FEED 6
    p_F0_DataField->u8ValveFEED7 = pVALVE_ICE_WATER_EXTRACT;                                          // CMD 86: 밸브 FEED 7
    p_F0_DataField->u8ValveFEED8 = 0;                                          // CMD 87: 밸브 FEED 8
    p_F0_DataField->u8ValveFEED9 = 0;                                          // CMD 88: 밸브 FEED 9
    p_F0_DataField->u8ValveFEED10 = 0;                                         // CMD 89: 밸브 FEED 10
    p_F0_DataField->u8ValveFEED11 = 0;                                         // CMD 90: 밸브 FEED 11
    p_F0_DataField->u8ValveFEED12 = 0;                                         // CMD 91: 밸브 FEED 12
    p_F0_DataField->u8ValveFEED13 = 0;                                         // CMD 92: 밸브 FEED 13
    p_F0_DataField->u8ValveFEED14 = 0;                                         // CMD 93: 밸브 FEED 14
    p_F0_DataField->u8ValveFEED15 = 0;                                          // CMD 94: 밸브 FEED 15

    p_F0_DataField->u8FilterReed = bit_filter_reed;                                // CMD 95: 필터 리드
    p_F0_DataField->u8FrontReed = bit_filter_cover;                                  // CMD 96: 프론트 리드
}

/***************************************************************************************/
/**
 * @brief 커맨드 F1의 프로토콜 데이터필드
 * @details 커맨드 F1의 프로토콜 데이터필드를 사용자의 변수를 채우는 함수
 */
static void Parse_F1_Protocol(F1_COLD_SYSTEM_DATA_FIELD *p_F1_DataField)
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
    p_F1_DataField->u8CoolingTargetRPS = (B1Data.u8ColdTargetRPS > 0) ? B1Data.u8ColdTargetRPS : get_cold_mode_comp_rps();
    p_F1_DataField->u8CoolingOnTemp = (B1Data.u8ColdOnTemp > 0) ? B1Data.u8ColdOnTemp : (U8)gu16_test_cold_on_temp;
    p_F1_DataField->u8CoolingOffTemp = (B1Data.u8ColdOffTemp > 0) ? B1Data.u8ColdOffTemp : (U8)gu16_test_cold_off_temp;
    p_F1_DataField->u8CoolingAddStartTime_H = (B1Data.u16ColdDelayTime > 0) ? (U8)(B1Data.u16ColdDelayTime >> 8) : (U8)(gu16_test_cold_delay_time >> 8);
    p_F1_DataField->u8CoolingAddStartTime_L = (B1Data.u16ColdDelayTime > 0) ? (U8)(B1Data.u16ColdDelayTime) : (U8)(gu16_test_cold_delay_time);

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
    p_F1_DataField->u8InletWaterCapHigh = GetB2TrayIn_Hz() > 0 ? (U8)((GetB2TrayIn_Hz() >> 8) & 0xFF) : (U8)((gu16_Ice_Tray_Fill_Hz >> 8) & 0xFF); // CMD 44: 입수 용량 [HIGH]
    p_F1_DataField->u8InletWaterCapLow = GetB2TrayIn_Hz() > 0 ? (U8)(GetB2TrayIn_Hz() & 0xFF) : (U8)(gu16_Ice_Tray_Fill_Hz & 0xFF);   // CMD 45: 입수 용량 [LOW]
    p_F1_DataField->u8SwingBarOnTime = GetB2SwingbarOn() > 0 ? GetB2SwingbarOn() : 2;                                      // CMD 46: 스윙바 ON 시간
    p_F1_DataField->u8SwingBarOffTime = GetB2SwingbarOff() > 0 ? GetB2SwingbarOff() : 6;                                     // CMD 47: 스윙바 OFF 시간
    p_F1_DataField->u8IceTrayPosition = gu8IceLEV;                                    // CMD 48: 제빙 트레이 위치
    p_F1_DataField->u8IceJamStatus = F_Safety_Routine;

    /* 보냉 데이터 (DATAFIELD 48-62) */
    p_F1_DataField->u8KeepColdStep = GetB4IceStorageTargetRPS() > 0 ? GetB4IceStorageTargetRPS() : 0;
    p_F1_DataField->u8KeepColdTargetRPS = GetB4IceStorageTargetRPS() > 0 ? GetB4IceStorageTargetRPS() : 0;
    p_F1_DataField->u8KeepColdTargetTemp = GetB4TargetTemp() > 0 ? GetB4TargetTemp() : 0;
    p_F1_DataField->u8KeepColdFirstTargetTemp = GetB4TargetTempFirst() > 0 ? GetB4TargetTempFirst() : 0;
    p_F1_DataField->u8KeepColdTrayPosition = GetB4TrayPosition() > 0 ? GetB4TrayPosition() : gu8IceLEV;

    /* 드레인 탱크 (DATAFIELD 63-71) */
    p_F1_DataField->u8DrainTankLowLevel = (Bit0_Drain_Water_Empty == SET) ? 1 : 0;
    p_F1_DataField->u8DrainTankFullLevel = (Bit2_Drain_Water_High == SET) ? 1 : 0;
    p_F1_DataField->u8DrainWaterLevelStatus = u8DrainWaterLevel;
    p_F1_DataField->u8DrainPumpOutput = F_Drain_Pump_Output;

    /* 기타 (DATAFIELD 72-74) */
    p_F1_DataField->u8IceTankCover = F_Tank_Cover_Input;
}

static void Parse_F2_Protocol(F2_HEATING_SYSTEM_DATA_FIELD *pstDataField)
{

}

/***************************************************************************************/
/**
 * @brief 커맨드 B1 (냉각 디버깅 데이터)의 프로토콜 데이터필드
 * @details PC → 제품 방향으로 고정값을 받아서 쓰는 함수
 */
static void Parse_B1_Protocol(U8 *buf, B1_COLD_TABLE_DATA_FIELD *pstDataField)
{
    pstDataField->u8ColdTargetRPS = buf[4];
    pstDataField->u8ColdOnTemp = buf[5];
    pstDataField->u8ColdOffTemp = buf[6];
    pstDataField->u16ColdDelayTime = (U16)((U16)buf[7] << 8 | buf[8]);
}

/***************************************************************************************/
/**
 * @brief 커맨드 B2 (제빙 디버깅 데이터)의 프로토콜 데이터필드
 * @details PC → 제품 방향으로 고정값을 받아서 쓰는 함수
 */
static void Parse_B2_Protocol(U8 *buf, B2_ICE_MAKE_TABLE_DATA_FIELD *pstDataField)
{
    pstDataField->u8IceMakeTargetRPS = buf[4];
    pstDataField->u16TrayIn_Hz = (U16)((U16)buf[5] << 8 | buf[6]);
    pstDataField->u8SwingbarOn = buf[7];
    pstDataField->u8SwingbarOff = buf[8];
}

/***************************************************************************************/
/**
 * @brief 커맨드 B4 (보냉 디버깅 데이터)의 프로토콜 데이터필드
 * @details PC → 제품 방향으로 고정값을 받아서 쓰는 함수
 */
static void Parse_B4_Protocol(U8 *buf, B4_ICE_STORAGE_TABLE_DATA_FIELD *pstDataField)
{
    pstDataField->u8IceStorageTargetRPS = buf[4];
    pstDataField->u8TargetTemp = buf[5];
    pstDataField->u8TargetTempFirst = buf[6];
    pstDataField->u8TrayPosition = buf[7];
}

U8 Protocol_Make_Ack_Packet(U8* buf, U8* Txbuf)
{
    U8 u8cmd = buf[PROTOCOL_IDX_CMD];
    U8 u8DataIndex = 0;
    U8 data_length = 0;

    switch( u8cmd )
    {
        case PROTOCOL_F0_CMD:                    // 0xF0
            // F0 구조체에 데이터 채우기
            Parse_F0_Protocol(&F0Data);
            data_length = PROTOCOL_F0_LENGTH;

            // F0 구조체를 바이트 배열로 변환하여 TxData에 복사
            for(u8DataIndex = 0; u8DataIndex < data_length; u8DataIndex++)
            {
                Txbuf[u8DataIndex] = ((U8*)&F0Data)[u8DataIndex];
            }
            break;

        case PROTOCOL_F1_CMD:                    // 0xF1
            // F1 구조체에 데이터 채우기
            Parse_F1_Protocol(&F1Data);
            data_length = PROTOCOL_F1_LENGTH;

            // F1 구조체를 바이트 배열로 변환하여 TxData에 복사
            for(u8DataIndex = 0; u8DataIndex < data_length; u8DataIndex++)
            {
                Txbuf[u8DataIndex] = ((U8*)&F1Data)[u8DataIndex];
            }
        break;

        case PROTOCOL_B3_CMD:                    // 0xB3
            SetFreezingTable(&buf[5]);
            SetUsedFreezingTable(SET);

            // F1 구조체에 데이터 채우기
            Parse_F1_Protocol(&F1Data);
            data_length = PROTOCOL_F1_LENGTH;

            // F1 구조체를 바이트 배열로 변환하여 TxData에 복사
            for(u8DataIndex = 0; u8DataIndex < data_length; u8DataIndex++)
            {
                Txbuf[u8DataIndex] = ((U8*)&F1Data)[u8DataIndex];
            }

            break;

        case PROTOCOL_B1_CMD:                    // 0xB1
            /* ECO */
            Parse_B1_Protocol(buf, &B1Data);

            data_length = buf[PROTOCOL_IDX_LENGTH];
            for(u8DataIndex = 0; u8DataIndex < data_length; u8DataIndex++)
            {
                Txbuf[u8DataIndex] = ((U8*)&buf)[u8DataIndex + 4];
            }
            break;

        case PROTOCOL_B2_CMD:                    // 0xB2
            /* ECO */
            Parse_B2_Protocol(buf, &B2Data);
            data_length = buf[PROTOCOL_IDX_LENGTH];
            for(u8DataIndex = 0; u8DataIndex < data_length; u8DataIndex++)
            {
                Txbuf[u8DataIndex] = ((U8*)&buf)[u8DataIndex + 4];
            }
            break;

        case PROTOCOL_B4_CMD:                    // 0xB4
            /* ECO */
            Parse_B4_Protocol(buf, &B4Data);
            data_length = buf[PROTOCOL_IDX_LENGTH];
            for(u8DataIndex = 0; u8DataIndex < data_length; u8DataIndex++)
            {
                Txbuf[u8DataIndex] = ((U8*)&buf)[u8DataIndex + 4];
            }
            break;

        default:
            break;
    }

    return data_length;

}

U8 Protocol_Make_Cmd(U8 *buf)
{
    U8 u8cmd = buf[PROTOCOL_IDX_CMD];
    U8 u8Txcmd = 0;

    /* 제품은 F0, F1으로만 응답 */
    switch( u8cmd )
    {
        case PROTOCOL_F0_CMD:                    // 0xF0
            u8Txcmd = PROTOCOL_F0_CMD;
            break;

        case PROTOCOL_F1_CMD:                    // 0xF1
        case PROTOCOL_B3_CMD:                    // 0xB3
            u8Txcmd = PROTOCOL_F1_CMD;
        break;

        case PROTOCOL_B1_CMD:                    // 0xB1
            u8Txcmd = PROTOCOL_B1_CMD;
        break;

        case PROTOCOL_B2_CMD:                    // 0xB2
            u8Txcmd = PROTOCOL_B2_CMD;
        break;

        case PROTOCOL_B4_CMD:                    // 0xB4
            u8Txcmd = PROTOCOL_B4_CMD;
        break;

        default:
            u8Txcmd = 0;
            break;
    }

    return u8Txcmd;
}


U8 GetB1ColdTargetRPS(void)
{
    return B1Data.u8ColdTargetRPS;
}

void SetB1ColdTargetRPS(U8 *u8ColdTargetRPS)
{
    *u8ColdTargetRPS = B1Data.u8ColdTargetRPS;
}

U8 GetB1ColdOnTemp(void)
{
    return B1Data.u8ColdOnTemp;
}

void SetB1ColdOnTemp(U8 *u8ColdOnTemp)
{
    *u8ColdOnTemp = B1Data.u8ColdOnTemp;
}

U8 GetB1ColdOffTemp(void)
{
    return B1Data.u8ColdOffTemp;
}

void SetB1ColdOffTemp(U8 *u8ColdOffTemp)
{
    *u8ColdOffTemp = B1Data.u8ColdOffTemp;
}

U16 GetB1ColdDelayTime(void)
{
    return B1Data.u16ColdDelayTime;
}

void SetB1ColdDelayTime(U16 *u16ColdDelayTime)
{
    *u16ColdDelayTime = B1Data.u16ColdDelayTime;
}

void SetB2IceMakeTargetRPS(U8 *u8IceMakeTargetRPS)
{
    *u8IceMakeTargetRPS = B2Data.u8IceMakeTargetRPS;
}

U8 GetB2IceMakeTargetRPS(void)
{
    return B2Data.u8IceMakeTargetRPS;
}

void SetB2TrayIn_Hz(U16 *u16TrayIn_Hz)
{
    *u16TrayIn_Hz = B2Data.u16TrayIn_Hz;
}

U16 GetB2TrayIn_Hz(void)
{
    return B2Data.u16TrayIn_Hz;
}

void SetB2SwingbarOn(U8 *u8SwingbarOn)
{
    *u8SwingbarOn = B2Data.u8SwingbarOn;
}

U8 GetB2SwingbarOn(void)
{
    return B2Data.u8SwingbarOn;
}

void SetB2SwingbarOff(U8 *u8SwingbarOff)
{
    *u8SwingbarOff = B2Data.u8SwingbarOff;
}

U8 GetB2SwingbarOff(void)
{
    return B2Data.u8SwingbarOff;
}

void SetB4IceStorageTargetRPS(U8 *u8IceStorageTargetRPS)
{
    *u8IceStorageTargetRPS = B4Data.u8IceStorageTargetRPS;
}

U8 GetB4IceStorageTargetRPS(void)
{
    return B4Data.u8IceStorageTargetRPS;
}

void SetB4TargetTemp(U8 *u8TargetTemp)
{
    *u8TargetTemp = B4Data.u8TargetTemp;
}

U8 GetB4TargetTemp(void)
{
    return B4Data.u8TargetTemp;
}

void SetB4TargetTempFirst(U8 *u8TargetTempFirst)
{
    *u8TargetTempFirst = B4Data.u8TargetTempFirst;
}

U8 GetB4TargetTempFirst(void)
{
    return B4Data.u8TargetTempFirst;
}

void SetB4TrayPosition(U8 *u8TrayPosition)
{
    *u8TrayPosition = B4Data.u8TrayPosition;
}

U8 GetB4TrayPosition(void)
{
    return B4Data.u8TrayPosition;
}
