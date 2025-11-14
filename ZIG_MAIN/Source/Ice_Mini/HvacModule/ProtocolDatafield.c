/*********************************************************************************/
/**
 * @file ProtocolDatafield.c
 * @brief 프로토콜 데이터 필드 파싱
 * @author
 * @date 2025-11-14
 * @version 1.0
*/
/*********************************************************************************/
#include "ProtocolDatafield.h"
#include "Protocol.h"
#include "Global_Variable.h"
#include "Port_Define.h"
/*********************************************************************************/
/* 변수 참조 및 헤더 참조 */
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
extern U8 gu8_GasSwitch_Status;
extern bit F_Comp_Output;
extern U8 u8DrainWaterLevel;
extern U8 gu8_Amb_Front_Temperature_One_Degree;
extern U8 gu8_Room_Temperature_One_Degree;
extern U8 gu8_Amb_Temperature_One_Degree;
extern U8 gu8_Cold_Temperature_One_Degree;
extern U8 gu8_Hot_Heater_Temperature_One_Degree;
extern U8 gu8_Hot_Out_Temperature_One_Degree;
extern TYPE_BYTE          U8MakeModeB;
extern U8 GetIceStep(void);
#define            gu8_Make_Mode                U8MakeModeB.byte
#define            Bit0_Cold_Make_Go                U8MakeModeB.Bit.b0
#define            Bit1_Ice_Make_Go                 U8MakeModeB.Bit.b1
extern TYPE_BYTE          U8DrainWaterLevelB;
#define            u8DrainWaterLevel                       U8DrainWaterLevelB.byte
#define            Bit0_Drain_Water_Empty                U8DrainWaterLevelB.Bit.b0
#define            Bit1_Drain_Water_Low                 U8DrainWaterLevelB.Bit.b1
#define            Bit2_Drain_Water_High                U8DrainWaterLevelB.Bit.b2
#define            Bit3_Drain_Water_Error                U8DrainWaterLevelB.Bit.b3

/*********************************************************************************/
/* 데이터필드 구조체 선언 */
F0_COMMON_SYSTEM_DATA_FIELD stF0Data;
F1_COLD_SYSTEM_DATA_FIELD stF1Data;
F2_HEATING_SYSTEM_DATA_FIELD stF2Data;
B1_COLD_TABLE_DATA_FIELD stB1Data;
/*********************************************************************************/

/**
 * @brief F0 프로토콜 데이터 필드 파싱
 * @param p_F0_DataField: F0 프로토콜 데이터 필드 구조체 포인터
 * @return void
*/
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
    p_F0_DataField->u8ValveFEED15 = 0;                                         // CMD 94: 밸브 FEED 15
    p_F0_DataField->u8FilterReed = bit_filter_reed;
    p_F0_DataField->u8FrontReed = bit_filter_cover;
}

/**
 * @brief F1 프로토콜 데이터 필드 파싱
 * @param p_F1_DataField: F1 프로토콜 데이터 필드 구조체 포인터
 * @return void
*/
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
    p_F1_DataField->u8DrainTankLowLevel = pLEVEL_ICE_DRAIN_LOW;
    p_F1_DataField->u8DrainTankFullLevel = pLEVEL_ICE_DRAIN_HIGH;
    if(u8DrainWaterLevel == 1)
    {
        p_F1_DataField->u8DrainWaterLevelStatus = 0;
    }
    else if(u8DrainWaterLevel == 2)
    {
        p_F1_DataField->u8DrainWaterLevelStatus = 1;
    }
    else if(u8DrainWaterLevel == 4)
    {
        p_F1_DataField->u8DrainWaterLevelStatus = 2;
    }
    else if(u8DrainWaterLevel == 8)
    {
        p_F1_DataField->u8DrainWaterLevelStatus = 3;
    }
    else
    {
        p_F1_DataField->u8DrainWaterLevelStatus = 4;
    }

    p_F1_DataField->u8DrainPumpOutput = F_Drain_Pump_Output;

    /* 기타 (DATAFIELD 72-74) */
    p_F1_DataField->u8IceTankCover = F_Tank_Cover_Input;
}

/**
 * @brief F2 프로토콜 데이터 필드 파싱
 * @param p_F2_DataField: F2 프로토콜 데이터 필드 구조체 포인터
 * @return void
*/
void Parse_F2_Protocol(F2_HEATING_SYSTEM_DATA_FIELD *pstDataField)
{

}

void SetColdOnTemp(U8 *u8ColdOnTemp)
{
    stB1Data.u8ColdOnTemp = *u8ColdOnTemp;
}

void SetColdOffTemp(U8 *u8ColdOffTemp)
{
    stB1Data.u8ColdOffTemp = *u8ColdOffTemp;
}

void SetColdDelayTime(U16 *u16ColdDelayTime)
{
    stB1Data.u16ColdDelayTime = *u16ColdDelayTime;
}

void SetColdTargetRPS(U8 *u8ColdTargetRPS)
{
    stB1Data.u8ColdTargetRPS = *u8ColdTargetRPS;
}

/**
 * @brief 프로토콜 데이터 필드 파싱
 * @param u16TxMode: 프로토콜 모드
 * @param TxData: 프로토콜 데이터 필드 배열
 * @return void
*/
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
