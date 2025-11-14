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
// ÔøΩ‹∫ÔøΩ ÔøΩÔøΩÔøΩÔøΩ ÔøΩÔøΩÔøΩÔøΩ
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
#endif

F0_COMMON_SYSTEM_DATA_FIELD F0Data = {0, };
F1_COLD_SYSTEM_DATA_FIELD F1Data = {0, };
F2_HEATING_SYSTEM_DATA_FIELD F2Data = {0, };


static void Parse_F0_Protocol(F0_COMMON_SYSTEM_DATA_FIELD *p_F0_DataField)
{
    /* ºæº≠∑˘ (DATAFIELD 1-13) */
    p_F0_DataField->u8AmbTemp1 = gu8_Amb_Front_Temperature_One_Degree;        // CMD 1: ø‹±‚ø¬µµ 1
    p_F0_DataField->u8InletWaterTemp = gu8_Room_Temperature_One_Degree;      // CMD 2: ¿‘ºˆø¬µµ
    p_F0_DataField->u8PurifiedWaterTemp = 0;                                  // CMD 3: ¡§ºˆø¬µµ
    p_F0_DataField->u8AmbTemp2 = gu8_Amb_Temperature_One_Degree;                                           // CMD 4: ø‹±‚ø¬µµ 2
    p_F0_DataField->u8ColdWaterTemp = gu8_Cold_Temperature_One_Degree;       // CMD 5: ≥√ºˆø¬µµ
    p_F0_DataField->u8HeaterInternalTemp = gu8_Hot_Heater_Temperature_One_Degree; // CMD 6: »˜≈Õ ≥ª∫Œø¬µµ
    p_F0_DataField->u8HotWaterOutletTemp = gu8_Hot_Out_Temperature_One_Degree; // CMD 7: ø¬ºˆ √‚ºˆø¬µµ

    /* πÎ∫Í ªÛ≈¬ (DATAFIELD 14-38) */
    // NOS πÎ∫Í: 1=CLOSE, 0=OPEN
    p_F0_DataField->u8ValveNOS1 = pVALVE_NOS;                                 // CMD 75: πÎ∫Í NOS 1
    p_F0_DataField->u8ValveNOS2 = 1;                                           // CMD 76: πÎ∫Í NOS 2
    p_F0_DataField->u8ValveNOS3 = 1;                                           // CMD 77: πÎ∫Í NOS 3
    p_F0_DataField->u8ValveNOS4 = 1;                                           // CMD 78: πÎ∫Í NOS 4
    p_F0_DataField->u8ValveNOS5 = 1;                                           // CMD 79: πÎ∫Í NOS 5
    // FEED πÎ∫Í: 1=OPEN, 0=CLOSE
    p_F0_DataField->u8ValveFEED1 = pVALVE_ROOM_IN;                                          // CMD 80: πÎ∫Í FEED 1
    p_F0_DataField->u8ValveFEED2 = pVALVE_HOT_IN;                                          // CMD 81: πÎ∫Í FEED 2
    p_F0_DataField->u8ValveFEED3 = pVALVE_COLD_IN;                                          // CMD 82: πÎ∫Í FEED 3
    p_F0_DataField->u8ValveFEED4 = pVALVE_ICE_TRAY_IN;                                          // CMD 83: πÎ∫Í FEED 4
    p_F0_DataField->u8ValveFEED5 = pVALVE_HOT_DRAIN;                                          // CMD 84: πÎ∫Í FEED 5
    p_F0_DataField->u8ValveFEED6 = pVALVE_COLD_DRAIN;                                          // CMD 85: πÎ∫Í FEED 6
    p_F0_DataField->u8ValveFEED7 = pVALVE_HOT_COLD_OVERFLOW;                                          // CMD 86: πÎ∫Í FEED 7
    p_F0_DataField->u8ValveFEED8 = pVALVE_ROOM_COLD_EXTRACT;                                          // CMD 87: πÎ∫Í FEED 8
    p_F0_DataField->u8ValveFEED9 = pVALVE_HOT_OUT;                                          // CMD 88: πÎ∫Í FEED 9
    p_F0_DataField->u8ValveFEED10 = pVALVE_ICE_WATER_EXTRACT;                                         // CMD 89: πÎ∫Í FEED 10
    p_F0_DataField->u8ValveFEED11 = 0;                                         // CMD 90: πÎ∫Í FEED 11
    p_F0_DataField->u8ValveFEED12 = 0;                                         // CMD 91: πÎ∫Í FEED 12
    p_F0_DataField->u8ValveFEED13 = 0;                                         // CMD 92: πÎ∫Í FEED 13
    p_F0_DataField->u8ValveFEED14 = 0;                                         // CMD 93: πÎ∫Í FEED 14
    p_F0_DataField->u8ValveFEED15 = 0;                                         // CMD 94: πÎ∫Í FEED 15
}

static void Parse_F1_Protocol(F1_COLD_SYSTEM_DATA_FIELD *p_F1_DataField)
{
    /* ∞¯¡∂Ω√Ω∫≈€ (DATAFIELD 1-13) */
    p_F1_DataField->u8RefValve1Pos = gu8_GasSwitch_Status;
    p_F1_DataField->u8RefValve2Pos = 0;
    p_F1_DataField->u8CompOutputStatus = ((F_Comp_Output == SET) ? 1 : 0);
    p_F1_DataField->u8CompStableTimeHigh = (U8)(gu16CompOffDelay >> 8);
    p_F1_DataField->u8CompStableTimeLow = (U8)(gu16CompOffDelay);
    p_F1_DataField->u8CompCurrentRPS = gu8_bldc_target_hz;
    p_F1_DataField->u8CompErrorCode = gu8BLDCErrorNum;
    p_F1_DataField->u8CompFanOutput = pDC_FAN;
    p_F1_DataField->u8IceTankFanOutput = 0;

    /* ≥√∞¢ µ•¿Ã≈Õ (DATAFIELD 14-26) */
    p_F1_DataField->u8CoolingOpStatus = (U8)(Bit0_Cold_Make_Go);
    p_F1_DataField->u8CoolingInitStart = (U8)bit_cold_first_op;
    p_F1_DataField->u8CoolingTargetRPS = get_cold_mode_comp_rps();
    p_F1_DataField->u8CoolingOnTemp = (U8)gu16_test_cold_on_temp;
    p_F1_DataField->u8CoolingOffTemp = (U8)gu16_test_cold_off_temp;
    p_F1_DataField->u8CoolingAddStartTime_H = (U8)(gu16_test_cold_delay_time >> 8);
    p_F1_DataField->u8CoolingAddStartTime_L = (U8)(gu16_test_cold_delay_time);

    /* ¡¶∫˘ µ•¿Ã≈Õ (DATAFIELD 27-47) */
    if(F_IceInit == SET)
    {
        p_F1_DataField->u8IceMakingStep = 255;
    }
    else
    {
        p_F1_DataField->u8IceMakingStep = GetIceStep();
    }

    p_F1_DataField->u8IceMakingTargetRPS = get_ice_mode_comp_rps();
    p_F1_DataField->u8IceMakingTimeHigh = (U8)((gu16IceMakeTime >> 8) & 0xFF); // CMD 42: ¡¶∫˘Ω√∞£ [HIGH]
    p_F1_DataField->u8IceMakingTimeLow = (U8)(gu16IceMakeTime & 0xFF);         // CMD 43: ¡¶∫˘Ω√∞£ [LOW]
    p_F1_DataField->u8InletWaterCapHigh = (U8)((gu16_Ice_Tray_Fill_Hz >> 8) & 0xFF); // CMD 44: ¿‘ºˆ øÎ∑Æ [HIGH]
    p_F1_DataField->u8InletWaterCapLow = (U8)(gu16_Ice_Tray_Fill_Hz & 0xFF);   // CMD 45: ¿‘ºˆ øÎ∑Æ [LOW]
    p_F1_DataField->u8SwingBarOnTime = 2;                                      // CMD 46: Ω∫¿ÆπŸ ON Ω√∞£
    p_F1_DataField->u8SwingBarOffTime = 6;                                     // CMD 47: Ω∫¿ÆπŸ OFF Ω√∞£
    p_F1_DataField->u8IceTrayPosition = gu8IceLEV;                                    // CMD 48: ¡¶∫˘ ∆Æ∑π¿Ã ¿ßƒ°
    p_F1_DataField->u8IceJamStatus = F_Safety_Routine;

    /* ∫∏≥√ µ•¿Ã≈Õ (DATAFIELD 48-62) */
    p_F1_DataField->u8KeepColdStep = 0;
    p_F1_DataField->u8KeepColdTargetRPS = 0;
    p_F1_DataField->u8KeepColdTargetTemp = 0;
    p_F1_DataField->u8KeepColdFirstTargetTemp = 0;
    p_F1_DataField->u8KeepColdTrayPosition = gu8IceLEV;

    /* µÂ∑π¿Œ ≈ ≈© (DATAFIELD 63-71) */
    p_F1_DataField->u8DrainTankLowLevel = (Bit0_Drain_Water_Empty == SET) ? 1 : 0;
    p_F1_DataField->u8DrainTankFullLevel = (Bit2_Drain_Water_High == SET) ? 1 : 0;
    p_F1_DataField->u8DrainWaterLevelStatus = u8DrainWaterLevel;
    p_F1_DataField->u8DrainPumpOutput = F_Drain_Pump_Output;

    /* ±‚≈∏ (DATAFIELD 72-74) */
    p_F1_DataField->u8IceTankCover = F_Tank_Cover_Input;
}

static void Parse_F2_Protocol(F2_HEATING_SYSTEM_DATA_FIELD *pstDataField)
{

}


U8 Protocol_Make_Ack_Packet(U8* buf, U8* Txbuf)
{
    U8 u8cmd = buf[PROTOCOL_IDX_CMD];
    U8 u8DataIndex = 0;
    U8 data_length = 0;

    switch( u8cmd )
    {
        case PROTOCOL_F0_CMD:                    // 0xF0
            // F0 ±∏¡∂√ºø° µ•¿Ã≈Õ √§øÏ±‚
            Parse_F0_Protocol(&F0Data);
            data_length = PROTOCOL_F0_LENGTH;

            // F0 ±∏¡∂√º∏¶ πŸ¿Ã∆Æ πËø≠∑Œ ∫Ø»Ø«œø© TxDataø° ∫πªÁ
            for(u8DataIndex = 0; u8DataIndex < data_length; u8DataIndex++)
            {
                Txbuf[u8DataIndex] = ((U8*)&F0Data)[u8DataIndex];
            }
            break;

        case PROTOCOL_F1_CMD:                    // 0xF1
        case PROTOCOL_B3_CMD:                    // 0xB3
            // F1 ±∏¡∂√ºø° µ•¿Ã≈Õ √§øÏ±‚
            Parse_F1_Protocol(&F1Data);
            data_length = PROTOCOL_F1_LENGTH;

            // F1 ±∏¡∂√º∏¶ πŸ¿Ã∆Æ πËø≠∑Œ ∫Ø»Ø«œø© TxDataø° ∫πªÁ
            for(u8DataIndex = 0; u8DataIndex < data_length; u8DataIndex++)
            {
                Txbuf[u8DataIndex] = ((U8*)&F1Data)[u8DataIndex];
            }
            break;

        default:
            break;
    }

    return data_length;

}


