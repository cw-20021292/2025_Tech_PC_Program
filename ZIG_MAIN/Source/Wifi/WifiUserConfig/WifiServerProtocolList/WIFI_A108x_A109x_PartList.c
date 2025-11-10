    
/*******************************************************************************
&Description :
    A1080 ~ A108x
    A1090 ~ A109x
    Part Table

&History : ssg@coway.com 2024.10.04 update

*******************************************************************************/

/******************************************************************************/
/***** Header *****************************************************************/
/******************************************************************************/
#include "WIFI_A108x_A109x_PartList.h"

/******************************************************************************/
/***** User Header ************************************************************/
/******************************************************************************/
#include "Global_Variable.h"
#include "m_wifi_global_data.h"
#include "port_define.h"
/******************************************************************************/
/***** User Function **********************************************************/
/******************************************************************************/
extern U16 gu16_wifi_cold_target_time_min;
extern bit bit_filter_cover;
/******************************************************************************/
/***** Table ******************************************************************/
/******************************************************************************/
/* Protocol List */
static const WifiNormalList_T WifiPartProtocolList[] =
{   /*  API ID        */
    {   WIFI_PART_0001_VALVE_NOS,                TYPE_UNSIGNED   },
    {   WIFI_PART_0002_VALVE_HOT_OUT,            TYPE_UNSIGNED   },
    {   WIFI_PART_0005_VALVE_CP_OUT,             TYPE_UNSIGNED   },
    {   WIFI_PART_0007_VALVE_PURE_IN,            TYPE_UNSIGNED   },
    //{   WIFI_PART_0008_VALVE_HOT_AIR,            TYPE_UNSIGNED   },
    {   WIFI_PART_0009_VALVE_HOT_IN,             TYPE_UNSIGNED   },
    {   WIFI_PART_000A_VALVE_HOT_DRAIN,          TYPE_UNSIGNED   },
    {   WIFI_PART_000B_VALVE_COLD_AIR,           TYPE_UNSIGNED   },
    {   WIFI_PART_000C_VALVE_COLD_IN,            TYPE_UNSIGNED   },
    {   WIFI_PART_000D_VALVE_COLD_DRAIN,         TYPE_UNSIGNED   },
    {   WIFI_PART_0015_VALVE_ICE_TRAY_IN,        TYPE_UNSIGNED   },
    {   WIFI_PART_0023_VALVE_ICE_WATER,          TYPE_UNSIGNED   },
    //{   WIFI_PART_010B_LEVEL_DRAIN_HIGH,         TYPE_UNSIGNED   },
    ///{   WIFI_PART_010D_LEVEL_DRAIN_LOW,          TYPE_UNSIGNED   },
    {   WIFI_PART_0204_COLD_FAN,                 TYPE_UNSIGNED   },
    {   WIFI_PART_0206_COLD_TEMP_1,              TYPE_UNSIGNED   },
    {   WIFI_PART_0208_COLD_ROOM_TEMP,           TYPE_UNSIGNED   },
    {   WIFI_PART_020B_COLD_TARGET_TEMP_TIME,    TYPE_UNSIGNED   },
    {   WIFI_PART_020D_COLD_ROOM_TEMP2,          TYPE_UNSIGNED   },
    {   WIFI_PART_020E_COLD_STOP_ERROR,          TYPE_UNSIGNED   },     // 2kg LPP 이후 추가 250616 CH.PARK
    {   WIFI_PART_0210_COLD_BLDC_ERROR,          TYPE_UNSIGNED   },     // 2kg LPP 이후 추가 250616 CH.PARK
    {   WIFI_PART_0211_COLD_DRAIN_CLOSE,         TYPE_UNSIGNED   },     // 2kg LPP 이후 추가 250616 CH.PARK

    {   WIFI_PART_0214_COLD_BLDC1,              TYPE_UNSIGNED   },     // 2kg LPP 이후 추가 250626 CH.PARK
    {   WIFI_PART_0215_COLD_BLDC2,              TYPE_UNSIGNED   },     // 2kg LPP 이후 추가 250626 CH.PARK
    {   WIFI_PART_0216_COLD_BLDC3,              TYPE_UNSIGNED   },     // 2kg LPP 이후 추가 250626 CH.PARK
    {   WIFI_PART_0217_COLD_BLDC4,              TYPE_UNSIGNED   },     // 2kg LPP 이후 추가 250626 CH.PARK
    {   WIFI_PART_0218_COLD_BLDC5,              TYPE_UNSIGNED   },     // 2kg LPP 이후 추가 250626 CH.PARK
    {   WIFI_PART_0219_COLD_BLDC6,              TYPE_UNSIGNED   },     // 2kg LPP 이후 추가 250626 CH.PARK

    {   WIFI_PART_0302_HOT_INSTANT_HEATER_1,     TYPE_UNSIGNED   },
    {   WIFI_PART_0303_HOT_INSTANT_HEATER_2,     TYPE_UNSIGNED   },
    {   WIFI_PART_0304_HOT_FLOW_MOTOR,           TYPE_UNSIGNED   },
    {   WIFI_PART_0305_HOT_TEMP_IN,              TYPE_UNSIGNED   },
    {   WIFI_PART_0306_HOT_TEMP_OUT,             TYPE_UNSIGNED   },
    {   WIFI_PART_0307_HOT_HEATER_TEMP,          TYPE_UNSIGNED   },
    //{   WIFI_PART_030A_HOT_STOP_ERROR,           TYPE_UNSIGNED   },
    {   WIFI_PART_0402_SENSOR_LEAK,             TYPE_UNSIGNED   },
    {   WIFI_PART_0403_SENSOR_FLOW,             TYPE_UNSIGNED   },
    {   WIFI_PART_0405_SENSOR_FILTER_REED,      TYPE_UNSIGNED   },
    {   WIFI_PART_0406_SENSOR_FILTER_SW_1,      TYPE_UNSIGNED   },
    {   WIFI_PART_0409_SENSOR_TANK_REED,        TYPE_UNSIGNED   },
    {   WIFI_PART_0501_STER_UV_FAUCET,          TYPE_UNSIGNED   },
    {   WIFI_PART_0502_STER_UV_FAUCET_ICE,      TYPE_UNSIGNED   },
    {   WIFI_PART_0503_STER_UV_ICE_TANK1,       TYPE_UNSIGNED   },
    {   WIFI_PART_0504_STER_UV_ICE_TRAY,        TYPE_UNSIGNED   },
    {   WIFI_PART_0508_STER_UV_ICE_TANK2,       TYPE_UNSIGNED   },
    {   WIFI_PART_050F_STER_UV_ICE_FAUCET2,     TYPE_UNSIGNED   },
    
    {   WIFI_PART_0801_ICE_FULL_SENSOR,         TYPE_UNSIGNED   },
    {   WIFI_PART_0802_ICE_SWING_BAR,           TYPE_UNSIGNED   },
    {   WIFI_PART_0804_ICE_TRAY_STEPMOTOR,      TYPE_UNSIGNED   },
    {   WIFI_PART_0805_ICE_TRAY_SENSING_SW,     TYPE_UNSIGNED   },
    // {   WIFI_PART_0806_ICE_MOTOR_OUT,           TYPE_UNSIGNED   },
    {   WIFI_PART_0807_ICE_DOOR_STEPMOTOR,      TYPE_UNSIGNED   },
    {   WIFI_PART_0808_ICE_COURSE_CHANGE_VV,    TYPE_UNSIGNED   },
    {   WIFI_PART_080A_ICE_DRAIN_PUMP,          TYPE_UNSIGNED   },
    {   WIFI_PART_080E_ICE_DRAIN_ERROR_E60,     TYPE_UNSIGNED   },  // LPP 배수펌프 동작이상 추가 250611 CH.PARK
    {   WIFI_PART_0812_ICE_SELECT_DOOR,         TYPE_UNSIGNED   },  // LPP 얼음셀렉트 도어 추가 250611 CH.PARK 
    {   WIFI_PART_0813_ICE_DOOR_HEATER,         TYPE_UNSIGNED   },  // LPP 얼음 도어 히터 추가 250611 CH.PARK
    {   WIFI_PART_0814_ICE_LACK_SENSOR,         TYPE_UNSIGNED   },
};

#define SZ_PART_EVENT_LIST    (sizeof(WifiPartProtocolList)/sizeof(WifiNormalList_T))

void* GetWifiPartDataEventList ( void )
{
    if (IsUserModelType() == Model_0)
    {
        return WifiPartProtocolList;
    }
    else
    {

    }
}

U16 GetWifiPartEventListSize ( void )
{
    if (IsUserModelType() == Model_0)
    {
        return SZ_PART_EVENT_LIST;
    }
    else
    {

    }
}

/* [User Part Data] Product -> Server */
U16 GetUserSystemPart ( U16 mu16Part )
{
    U16 mu16Data = 9999U;
    switch ( mu16Part )
    {
        case WIFI_PART_0001_VALVE_NOS :         // NOS 원수밸브
            mu16Data = SELF_Test_Result_Data.word[DC_LOAD__NOS_VALVE];
            initial_each_self_data( DC_LOAD__NOS_VALVE );
        break;

        case WIFI_PART_0002_VALVE_HOT_OUT :     // 온수 출수 V/V
            mu16Data = SELF_Test_Result_Data.word[DC_LOAD__HOT_OUT_VALVE];
            initial_each_self_data( DC_LOAD__HOT_OUT_VALVE );
        break;

        case WIFI_PART_0005_VALVE_CP_OUT :      // 냉정 추출 V/V
            mu16Data = SELF_Test_Result_Data.word[DC_LOAD__COLD_PURE_OUT_VALVE];
            initial_each_self_data( DC_LOAD__COLD_PURE_OUT_VALVE );
        break;

        case WIFI_PART_0007_VALVE_PURE_IN :     // 정수 입수 V/V
            mu16Data = SELF_Test_Result_Data.word[DC_LOAD__PURE_IN_VALVE];
            initial_each_self_data( DC_LOAD__PURE_IN_VALVE );
        break;

        case WIFI_PART_0009_VALVE_HOT_IN :      // 온수 입수 V/V
            mu16Data = SELF_Test_Result_Data.word[DC_LOAD__HOT_IN_VALVE];
            initial_each_self_data( DC_LOAD__HOT_IN_VALVE );
        break;

        case WIFI_PART_000A_VALVE_HOT_DRAIN :   // 온수 드레인 NOS V/V
            mu16Data = SELF_Test_Result_Data.word[DC_LOAD__HOT_DRAIN_VALVE];
            initial_each_self_data( DC_LOAD__HOT_DRAIN_VALVE );
        break;

        case WIFI_PART_000B_VALVE_COLD_AIR :    // 오버플로우 NOS V/V
            mu16Data = SELF_Test_Result_Data.word[DC_LOAD__OVERFLOW_VALVE];
            initial_each_self_data( DC_LOAD__OVERFLOW_VALVE);
        break;

        case WIFI_PART_000C_VALVE_COLD_IN :     // 냉수 입수 V/V
            mu16Data = SELF_Test_Result_Data.word[DC_LOAD__COLD_IN_VALVE];
            initial_each_self_data( DC_LOAD__COLD_IN_VALVE );
        break;

        case WIFI_PART_000D_VALVE_COLD_DRAIN :  // 냉수 드레인 V/V
            mu16Data = SELF_Test_Result_Data.word[DC_LOAD__COLD_DRAIN_VALVE];
            initial_each_self_data( DC_LOAD__COLD_DRAIN_VALVE );
        break;

        case WIFI_PART_0015_VALVE_ICE_TRAY_IN : // 아이스 트레이 입수 V/V
            mu16Data = SELF_Test_Result_Data.word[DC_LOAD__ICETRAY_IN_VALVE];
            initial_each_self_data( DC_LOAD__ICETRAY_IN_VALVE );
        break;

        case WIFI_PART_0023_VALVE_ICE_WATER:
            mu16Data = SELF_Test_Result_Data.word[DC_LOAD__ICE_WATER_EXTRACT_VALVE];
            initial_each_self_data( DC_LOAD__ICE_WATER_EXTRACT_VALVE );
        break;

        #if 0
        case WIFI_PART_010B_LEVEL_DRAIN_HIGH : 
            if( pLEVEL_ICE_DRAIN_HIGH == SET )
            {
                /*..hui [23-12-21오후 8:12:16] 감지..*/
                mu16Data = 1;
            }
            else
            {
                /*..hui [23-12-21오후 8:12:22] 미감지..*/
                mu16Data = 0;
            }
        break;
        case WIFI_PART_010D_LEVEL_DRAIN_LOW : 
            if( pLEVEL_ICE_DRAIN_LOW == SET )
            {
                /*..hui [23-12-21오후 8:12:16] 감지..*/
                mu16Data = 1;
            }
            else
            {
                /*..hui [23-12-21오후 8:12:22] 미감지..*/
                mu16Data = 0;
            }
        break;
        #endif

        case WIFI_PART_0204_COLD_FAN : 
            mu16Data = SELF_Test_Result_Data.word[DC_LOAD__COLD_FAN];
            initial_each_self_data( DC_LOAD__COLD_FAN );

        break;
        case WIFI_PART_0206_COLD_TEMP_1 : 
            if( Bit14_Cold_Temp_Open_Short_Error__E44 == SET )
            {
                mu16Data = SELF_TEST_SENSOR_ERROR_DATA;
            }
            else
            {
                mu16Data = gu8_Cold_Temperature_One_Degree;
            }
        break;

        case WIFI_PART_0208_COLD_ROOM_TEMP : 
            if( Bit15_Amb_Temp_Open_Short_Error__E43 == SET )
            {
                mu16Data = SELF_TEST_SENSOR_ERROR_DATA;
            }
            else
            {
                mu16Data = gu8_Amb_Temperature_One_Degree;
            }
        break;

        case WIFI_PART_020B_COLD_TARGET_TEMP_TIME : 
            mu16Data = gu16_wifi_cold_target_time_min;
        break;

        case WIFI_PART_020D_COLD_ROOM_TEMP2:
            if( Bit21_Amb_Side_Temp_Open_Short_Error__E53 == SET )
            {
                mu16Data = SELF_TEST_SENSOR_ERROR_DATA;
            }
            else
            {
                mu16Data = gu8_Amb_Temperature_One_Degree;
            }
        break;

        case WIFI_PART_020E_COLD_STOP_ERROR:
            if( Bit6_Main_Water_Flow_Block_Error__E09 == SET )
            {
                mu16Data = 1;
            }
            else
            {
                mu16Data = 0;
            }
        break;

        case WIFI_PART_0210_COLD_BLDC_ERROR:
            if( Bit7_BLDC_Communication_Error__E27 == SET )
            {
                mu16Data = 1;
            }
            else
            {
                mu16Data = 0;
            }
        break;

        case WIFI_PART_0211_COLD_DRAIN_CLOSE:
            if( Bit0_Hot_Water_Flow_Block_Error__E08 == SET )
            {
                mu16Data = 1;
            }
            else
            {
                mu16Data = 0;
            }
        break;

        case WIFI_PART_0214_COLD_BLDC1:
            if(Bit8_BLDC_Operation_Error_Current_Sensing__E81 == SET)
            {
                mu16Data = 1;
            }
            else
            {
                mu16Data = 0;
            }
        break;

        case WIFI_PART_0215_COLD_BLDC2:
            if(Bit9_BLDC_Operation_Error_Starting_Fail__E82 == SET)
            {
                mu16Data = 1;
            }
            else
            {
                mu16Data = 0;
            }
        break;

        case WIFI_PART_0216_COLD_BLDC3:
            if(Bit10_BLDC_Operation_Error_Over_Current__E83 == SET)
            {
                mu16Data = 1;
            }
            else
            {
                mu16Data = 0;
            }
        break;

        case WIFI_PART_0217_COLD_BLDC4:
            if(Bit11_BLDC_Operation_Error_Overheat__E84 == SET)
            {
                mu16Data = 1;
            }
            else
            {
                mu16Data = 0;
            }
        break;

        case WIFI_PART_0218_COLD_BLDC5:
            if(Bit12_BLDC_Operation_Error_Disconnection_Comm__E85 == SET)
            {
                mu16Data = 1;
            }
            else
            {
                mu16Data = 0;
            }
        break;

        case WIFI_PART_0219_COLD_BLDC6:
            if(Bit13_BLDC_Operation_Error_Abnormal_Voltage__E86 == SET)
            {
                mu16Data = 1;
            }
            else
            {
                mu16Data = 0;
            }
        break;

        case WIFI_PART_0301_HOT_TEMP : 
				mu16Data = gu8_Hot_Heater_Temperature_One_Degree;
        break;
        case WIFI_PART_0302_HOT_INSTANT_HEATER_1 : 				

				//mu16Data = U16_HEATER_TRIAC_0302;
				mu16Data = SELF_Test_Result_Data.word[AC_LOAD_HOT_TRIAC];
				initial_each_self_data( AC_LOAD_HOT_TRIAC );

        break;
        case WIFI_PART_0303_HOT_INSTANT_HEATER_2 : 
                //mu16Data = U16_HEATER_RELAY_0303;
				mu16Data = SELF_Test_Result_Data.word[AC_LOAD_HOT_RELAY];
                initial_each_self_data( AC_LOAD_HOT_RELAY );
        break;
        case WIFI_PART_0304_HOT_FLOW_MOTOR : 
                //mu16Data = U16_HOT_FLOW_VALVE_0304;
				mu16Data = SELF_Test_Result_Data.word[DC_LOAD__FLOWVALVE_MOTOR];
                initial_each_self_data( DC_LOAD__FLOWVALVE_MOTOR );
        break;

        case WIFI_PART_0305_HOT_TEMP_IN : 
             mu16Data = gu8_Hot_In_Temperature_One_Degree;
        break;
        case WIFI_PART_0306_HOT_TEMP_OUT : 
             mu16Data = gu8_Hot_Out_Temperature_One_Degree;
        break;
        case WIFI_PART_0307_HOT_HEATER_TEMP : 
			mu16Data = gu8_Hot_Heater_Temperature_One_Degree;
        break;

#if 0
        case WIFI_PART_030A_HOT_STOP_ERROR : 
            if( Bit0_Hot_Water_Flow_Block_Error__E08 == SET )
            {
                mu16Data = 1;
            }
            else
            {
                mu16Data = 0;
            }
        break;
#endif
        case WIFI_PART_0402_SENSOR_LEAK : 
            if( Bit3_Leakage_Sensor_Error__E01 == SET )
            {
                mu16Data = 1;
            }
            else
            {
                mu16Data = 0;
            }
        break;

        case WIFI_PART_0403_SENSOR_FLOW :
           mu16Data = SELF_Test_Result_Data.word[DC_LOAD__FLOW_SENSOR];
            initial_each_self_data( DC_LOAD__FLOW_SENSOR );
        break;

        case WIFI_PART_0405_SENSOR_FILTER_REED :
           if(bit_filter_cover == SET)
           {
              mu16Data = 0;
           }
           else
           {
              mu16Data = 1;
           }
           
        break;

        case WIFI_PART_0406_SENSOR_FILTER_SW_1 :
           if(bit_filter_reed == SET)
           {
              mu16Data = 0;
           }
           else
           {
              mu16Data = 1;
           }
        break;

        case WIFI_PART_0409_SENSOR_TANK_REED :
           if(F_Tank_Cover_Input == SET)
           {
              mu16Data = 0;
           }
           else
           {
              mu16Data = 1;
           }
        break;

        case WIFI_PART_0501_STER_UV_FAUCET :
            mu16Data = SELF_Test_Result_Data.word[DC_LOAD__UV_FAUCET];
            initial_each_self_data( DC_LOAD__UV_FAUCET );
        break;

        case WIFI_PART_0502_STER_UV_FAUCET_ICE : 
            mu16Data = SELF_Test_Result_Data.word[DC_LOAD__UV_ICE_FAUCET1];
            initial_each_self_data( DC_LOAD__UV_ICE_FAUCET1 );
        break;

        case WIFI_PART_0503_STER_UV_ICE_TANK1 : 
            mu16Data = SELF_Test_Result_Data.word[DC_LOAD__UV_ICE_TANK_1_2];
            initial_each_self_data( DC_LOAD__UV_ICE_TANK_1_2 );
        break;
        
        case WIFI_PART_0504_STER_UV_ICE_TRAY : 
            mu16Data = SELF_Test_Result_Data.word[DC_LOAD__UV_ICE_TRAY];
            initial_each_self_data( DC_LOAD__UV_ICE_TRAY);
        break;

        case WIFI_PART_0508_STER_UV_ICE_TANK2 : 
            mu16Data = SELF_Test_Result_Data.word[DC_LOAD__UV_ICE_TANK_3];
            initial_each_self_data( DC_LOAD__UV_ICE_TANK_3);
        break;

        case WIFI_PART_050F_STER_UV_ICE_FAUCET2:
            mu16Data = SELF_Test_Result_Data.word[DC_LOAD__UV_ICE_FAUCET2];
            initial_each_self_data( DC_LOAD__UV_ICE_FAUCET2);
        break;
        
        case WIFI_PART_0801_ICE_FULL_SENSOR : 
            mu16Data = SELF_Test_Result_Data.word[DC_LOAD__ICE_FULL_SENSOR];
            initial_each_self_data( DC_LOAD__ICE_FULL_SENSOR);
        break;
        
        case WIFI_PART_0802_ICE_SWING_BAR : 
            mu16Data = SELF_Test_Result_Data.word[DC_LOAD__SWING_BAR];
            initial_each_self_data( DC_LOAD__SWING_BAR);

        break;
        
        case WIFI_PART_0804_ICE_TRAY_STEPMOTOR : 
            mu16Data = SELF_Test_Result_Data.word[DC_LOAD__ICE_TRAY_MOTOR];
            initial_each_self_data( DC_LOAD__ICE_TRAY_MOTOR);
        break;

        case WIFI_PART_0805_ICE_TRAY_SENSING_SW : 
            if( Bit17_Tray_Micro_SW_Dual_Detect_Error__E61 == SET )
            {
                mu16Data = 3;
            }
            else if( Bit18_Tray_Micro_SW_Up_Move_Error__E62 == SET )
            {
                mu16Data = 1;
            }
            else if( Bit19_Tray_Micro_SW_Down_Move_Error__E63 == SET )
            {
                mu16Data = 2;
            }
            else
            {
                mu16Data = 0;
            }

        break;

        case WIFI_PART_0807_ICE_DOOR_STEPMOTOR : 
            mu16Data = SELF_Test_Result_Data.word[DC_LOAD__ICE_DOOR_MOTOR];
            initial_each_self_data( DC_LOAD__ICE_DOOR_MOTOR);
        break;

        case WIFI_PART_0808_ICE_COURSE_CHANGE_VV : 
            mu16Data = SELF_Test_Result_Data.word[DC_LOAD__GAS_SWITCH_MOTOR];
            initial_each_self_data( DC_LOAD__GAS_SWITCH_MOTOR);
        break;
        
        case WIFI_PART_080A_ICE_DRAIN_PUMP : 
            mu16Data = SELF_Test_Result_Data.word[DC_LOAD__DRAIN_PUMP];
            initial_each_self_data( DC_LOAD__DRAIN_PUMP);
        break;

        case WIFI_PART_080E_ICE_DRAIN_ERROR_E60 : // LPP 배수펌프 동작이상 추가 250611 CH.PARK
            if(Bit16_Drain_Pump_Error__E60 == SET)
            {
              mu16Data = 1;
            }
            else
            {
              mu16Data = 0;
            }
        break;

        case WIFI_PART_0812_ICE_SELECT_DOOR:    // LPP 얼음셀렉트 도어 추가 250611 CH.PARK 
            mu16Data = SELF_Test_Result_Data.word[DC_LOAD__ICE_SELECT_DOOR_MOTOR];
            initial_each_self_data( DC_LOAD__ICE_SELECT_DOOR_MOTOR);
        break;

        case WIFI_PART_0813_ICE_DOOR_HEATER:    // LPP 얼음 도어 히터 추가 250611 CH.PARK
            mu16Data = SELF_Test_Result_Data.word[DC_LOAD__ICE_DOOR_HEATER];
            initial_each_self_data( DC_LOAD__ICE_DOOR_HEATER);
        break;

        case WIFI_PART_0814_ICE_LACK_SENSOR : 
            mu16Data = SELF_Test_Result_Data.word[DC_LOAD__ICE_LACK_SENSOR];
            initial_each_self_data( DC_LOAD__ICE_LACK_SENSOR);
        break;

        default:
		mu16Data = 0xFFFF;
        break;
    }

    return mu16Data;
}

/* [A1091] [Diagnosis Result]  Server -> Product */
/* SetWifiSendStatus( TX_DATA, WIFI_TX_RQST_INFO_DRR );  A2010.0029 DiagnosisResultRequest  */ 
void SetUserSystemDiagnosis ( U16 mu16DR, I16 mData ) // [1]: Error 있음, [-100] : 이상1
{
	#define DiagnosisError 	 (1)
	#define DiagnosisAbnormal1 (-100)
	#define DiagnosisAbnormal2 (-200)

	switch ( mu16DR )
    {
		// case 0x0001:
		// 	if (mData == DiagnosisError)
		// 	{

		// 	}
        // 	else if (mData == DiagnosisAbnormal1)
		// 	{

		// 	}
		// 	break;


		default:

        break;
	}
}