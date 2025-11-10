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
#include    "valve_hot_in.h"

void output_valve_hot_in_feed9(void);

/***********************************************************************************************************************/
/*..hui [21-5-21오후 3:53:52] 온수 입수 밸브 = FEED9..*/
TYPE_WORD          U16HotInFeed9ValveONB;
#define            u16HotInFeed9ValveONs                        U16HotInFeed9ValveONB.word
#define            u8HotInFeed9ValveONs_L                       U16HotInFeed9ValveONB.byte[0]
#define            u8HotInFeed9ValveONs_H                       U16HotInFeed9ValveONB.byte[1]
#define            Bit0_HIF9_Water_Filling_State                U16HotInFeed9ValveONB.Bit.b0
#define            Bit1_HIF9_Hot_Out_State                      U16HotInFeed9ValveONB.Bit.b1
#define            Bit2_HIF9_Ice_Tray_Ster_State                U16HotInFeed9ValveONB.Bit.b2
#define            Bit3_HIF9_Hot_Drain_Cold_State               U16HotInFeed9ValveONB.Bit.b3

TYPE_BYTE          U8HotInFeed9ValveOFFB;
#define            u8HotInFeed9ValveOFF                         U8HotInFeed9ValveOFFB.byte
#define            Bit0_HIF9_Off_State                          U8HotInFeed9ValveOFFB.Bit.b0

bit bit_hot_in_valve_output;

bit bit_Holding_Flow_Block_Error__E08;// ★2025-06-02 Phil

/***********************************************************************************************************************/
extern FLUSHING_STEP gu8_flushing_mode;
extern U8 gu8_filter_flushing_step;
extern U8 gu8_hot_drain_cold;



/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void output_valve_hot_in_feed9(void)
{
	if(gu8_flushing_mode == FLUSHING_FILTER)
	{
		if(gu8_filter_flushing_step >= 3 && gu8_filter_flushing_step <= 4)
		{
			Bit0_HIF9_Water_Filling_State = SET;
		}
		else
		{
			Bit0_HIF9_Water_Filling_State = CLEAR;
		}
		
	}
	else if(gu8_flushing_mode == FLUSHING_FILL_HOT_TANK_STATE)
	{
		if( gu8_Hot_Filling_Step >= 1 && gu8_Hot_Filling_Step <= 6 )
		{
			Bit0_HIF9_Water_Filling_State = F_First_Hot_Effluent;
		}
		else
		{
			Bit0_HIF9_Water_Filling_State = CLEAR;
		}
	}
	else
	{
		Bit0_HIF9_Water_Filling_State = CLEAR;
	}

    if(F_WaterOut == SET )
    {
        if( u8WaterOutState == HOT_WATER_SELECT )
        {
            if( gu8_Water_Out_Step == STATE_20_HOT_PRE_HEATING_STATE
                /*&& gu8_Preheat_Step <= 2 )*/
                && gu8_Preheat_Step >= STATE_1_PREHEAT_CHECK_OUTAGE_STATE
                && gu8_Preheat_Step <= STATE_2_PREHEAT_DRAIN_STATE )
            {
                /*..hui [21-9-1오후 4:35:31] 초기 유량조절밸브 원점 이동시에는 밸브 OFF상태 유지..*/
                /*..hui [21-5-21오후 8:11:32] 초기 유량 확인용..*/
                Bit1_HIF9_Hot_Out_State = F_WaterOut;

    // ★2025-06-02 Phil
                if(gu8_Preheat_Step == STATE_2_PREHEAT_DRAIN_STATE)
                {
                    // 2025-06-05 20:23:04  cbr 30도 조건 가둬서 예열 시 스팀
                    if(gu8_Hot_In_Temperature_One_Degree <= 25)
                    {
                        if(u16Heater_Power_Save <= HEATER_POWER_LOW)
                        {
                            if ( ( gu8_hot_setting_temperature == HOT_SET_TEMP____100oC )
                                            || ( gu8_hot_setting_temperature == HOT_SET_TEMP____95oC )
                                            || ( gu8_hot_setting_temperature == HOT_SET_TEMP____90oC ) )
                            {
                                if(gu8_preheat_Closed_max_timer > 100 || gu8_Hot_Heater_Temperature_One_Degree >= gu8_Hot_Preheat_Temp - 5)
                                {
                                    Bit1_HIF9_Hot_Out_State = F_WaterOut;
                                    bit_Holding_Flow_Block_Error__E08 = 0;
                                }
                                else
                                {
                                    Bit1_HIF9_Hot_Out_State = CLEAR;            // 잠깐 가둬예열해보자
                                    bit_Holding_Flow_Block_Error__E08 = 1;
                                }
                            }
                            //2025-07-22 cbr 10도 저전압 첫잔 full로해도 미달
                            else if ( (( gu8_hot_setting_temperature == HOT_SET_TEMP____85oC )
                                    || ( gu8_hot_setting_temperature == HOT_SET_TEMP____80oC ))
                                    && (gu8_Hot_In_Temperature_One_Degree <= 15) )
                            {
                                if(gu8_preheat_Closed_max_timer > 100 || gu8_Hot_Heater_Temperature_One_Degree >= gu8_Hot_Preheat_Temp - 5)
                                {
                                    Bit1_HIF9_Hot_Out_State = F_WaterOut;
                                    bit_Holding_Flow_Block_Error__E08 = 0;
                                }
                                else
                                {
                                    Bit1_HIF9_Hot_Out_State = CLEAR;            // 잠깐 가둬예열해보자
                                    bit_Holding_Flow_Block_Error__E08 = 1;
                                }
                            }
                            else
                            {
                                if(gu8_preheat_Closed_max_timer > 60 || gu8_Hot_Heater_Temperature_One_Degree >= gu8_Hot_Preheat_Temp - 5)
                                {
                                    Bit1_HIF9_Hot_Out_State = F_WaterOut;
                                    bit_Holding_Flow_Block_Error__E08 = 0;
                                }
                                else
                                {
                                    Bit1_HIF9_Hot_Out_State = CLEAR;            // 잠깐 가둬예열해보자
                                    bit_Holding_Flow_Block_Error__E08 = 1;
                                }
                            }
                        }
                        else
                        {
                            if ( ( gu8_hot_setting_temperature == HOT_SET_TEMP____100oC )
                                            || ( gu8_hot_setting_temperature == HOT_SET_TEMP____95oC )
                                            || ( gu8_hot_setting_temperature == HOT_SET_TEMP____90oC ) )
                            {
                                if(gu8_preheat_Closed_max_timer > 60 || gu8_Hot_Heater_Temperature_One_Degree >= gu8_Hot_Preheat_Temp - 5)
                                {       //2025-07-28 cbr
                                    Bit1_HIF9_Hot_Out_State = F_WaterOut;
                                    bit_Holding_Flow_Block_Error__E08 = 0;
                                }
                                else
                                {
                                    Bit1_HIF9_Hot_Out_State = CLEAR;            // 잠깐 가둬예열해보자
                                    bit_Holding_Flow_Block_Error__E08 = 1;
                                }
                            }
                            else if ( ( gu8_hot_setting_temperature == HOT_SET_TEMP____85oC ) 
                                        || ( gu8_hot_setting_temperature == HOT_SET_TEMP____80oC ) )
                            {           //2025-07-15 cbr  1kg 동일  20도 고전압 저유량 끊김
                                if((u8Target_Hz_Hot > gu8_hot_filling_max_hz)&&(u16Heater_Power_Save >= HEATER_POWER_HIGH))
                                {
                                    if(gu8_preheat_Closed_max_timer > 30 || gu8_Hot_Heater_Temperature_One_Degree >= gu8_Hot_Preheat_Temp - 5)
                                    {
                                        Bit1_HIF9_Hot_Out_State = F_WaterOut;            
                                        bit_Holding_Flow_Block_Error__E08 = 0; 
                                    }
                                    else
                                    {
                                        Bit1_HIF9_Hot_Out_State = CLEAR;            // 잠깐 가둬예열해보자              
                                        bit_Holding_Flow_Block_Error__E08 = 1;      
                                    }      
                                }
                                else
                                {
                                    if(gu8_preheat_Closed_max_timer > 60 || gu8_Hot_Heater_Temperature_One_Degree >= gu8_Hot_Preheat_Temp - 5)
                                    {
                                        Bit1_HIF9_Hot_Out_State = F_WaterOut;            
                                        bit_Holding_Flow_Block_Error__E08 = 0; 
                                    }
                                    else
                                    {
                                        Bit1_HIF9_Hot_Out_State = CLEAR;            // 잠깐 가둬예열해보자              
                                        bit_Holding_Flow_Block_Error__E08 = 1;      
                                    }     
                                }           	                
                            }
                            //2025-07-16 cbr   //입수20 정격,고전압 조건 가둬서 예열 시 과열
                            else if ( ( gu8_hot_setting_temperature == HOT_SET_TEMP____45oC )
                                    && (gu8_Hot_In_Temperature_One_Degree > 15))
                            {
                                if(gu8_preheat_Closed_max_timer > 30 || gu8_Hot_Heater_Temperature_One_Degree >= gu8_Hot_Preheat_Temp - 5)
                                {
                                    Bit1_HIF9_Hot_Out_State = F_WaterOut;
                                    bit_Holding_Flow_Block_Error__E08 = 0;
                                }
                                else
                                {
                                    Bit1_HIF9_Hot_Out_State = CLEAR;            // 잠깐 가둬예열해보자
                                    bit_Holding_Flow_Block_Error__E08 = 1;
                                }
                            }
                            else
                            {
                                if(gu8_preheat_Closed_max_timer > 60 || gu8_Hot_Heater_Temperature_One_Degree >= gu8_Hot_Preheat_Temp - 5)
                                {
                                    Bit1_HIF9_Hot_Out_State = F_WaterOut;
                                    bit_Holding_Flow_Block_Error__E08 = 0;
                                }
                                else
                                {
                                    Bit1_HIF9_Hot_Out_State = CLEAR;            // 잠깐 가둬예열해보자
                                    bit_Holding_Flow_Block_Error__E08 = 1;
                                }
                            }
                        }
                    }
                    else       // 2025-06-05 20:23:04  cbr 30도 조건 가둬서 예열 시 스팀
                    {
                        if(u16Heater_Power_Save <= HEATER_POWER_LOW)
                        {
                            if ( ( gu8_hot_setting_temperature == HOT_SET_TEMP____100oC )
                                            || ( gu8_hot_setting_temperature == HOT_SET_TEMP____95oC )
                                            || ( gu8_hot_setting_temperature == HOT_SET_TEMP____90oC ) )
                            {
                                if(gu8_preheat_Closed_max_timer > 100 || gu8_Hot_Heater_Temperature_One_Degree >= gu8_Hot_Preheat_Temp - 5)
                                {
                                    Bit1_HIF9_Hot_Out_State = F_WaterOut;
                                    bit_Holding_Flow_Block_Error__E08 = 0;
                                }
                                else
                                {
                                    Bit1_HIF9_Hot_Out_State = CLEAR;            // 잠깐 가둬예열해보자
                                    bit_Holding_Flow_Block_Error__E08 = 1;
                                }
                            }   //2025-06-05 17:58:36 cbr   //입수30 조건 가둬서 예열 시 과열
                            else if ( ( gu8_hot_setting_temperature == HOT_SET_TEMP____45oC ) )
                            {
                                if(gu8_preheat_Closed_max_timer > 10 || gu8_Hot_Heater_Temperature_One_Degree >= gu8_Hot_Preheat_Temp - 5)
                                {
                                    Bit1_HIF9_Hot_Out_State = F_WaterOut;
                                    bit_Holding_Flow_Block_Error__E08 = 0;
                                }
                                else
                                {
                                    Bit1_HIF9_Hot_Out_State = CLEAR;            // 잠깐 가둬예열해보자
                                    bit_Holding_Flow_Block_Error__E08 = 1;
                                }
                            }
                            else
                            {
                                if(gu8_preheat_Closed_max_timer > 60 || gu8_Hot_Heater_Temperature_One_Degree >= gu8_Hot_Preheat_Temp - 5)
                                {
                                    Bit1_HIF9_Hot_Out_State = F_WaterOut;
                                    bit_Holding_Flow_Block_Error__E08 = 0;
                                }
                                else
                                {
                                    Bit1_HIF9_Hot_Out_State = CLEAR;            // 잠깐 가둬예열해보자
                                    bit_Holding_Flow_Block_Error__E08 = 1;
                                }
                            }
                        }
                        else
                        {
                            if ( ( gu8_hot_setting_temperature == HOT_SET_TEMP____100oC )
                                            || ( gu8_hot_setting_temperature == HOT_SET_TEMP____95oC )
                                            || ( gu8_hot_setting_temperature == HOT_SET_TEMP____90oC ) )
                                            // || ( gu8_hot_setting_temperature == HOT_SET_TEMP____85oC ) )
                            {
                                if(gu8_preheat_Closed_max_timer > 10 || gu8_Hot_Heater_Temperature_One_Degree >= gu8_Hot_Preheat_Temp - 5)
                                {       //2025-07-23 cbr_test 30도 고전압 과열 에어 물끊김(정격때 재확인 후 전압 구분필요)
                                    Bit1_HIF9_Hot_Out_State = F_WaterOut;
                                    bit_Holding_Flow_Block_Error__E08 = 0;
                                }
                                else
                                {
                                    Bit1_HIF9_Hot_Out_State = CLEAR;            // 잠깐 가둬예열해보자
                                    bit_Holding_Flow_Block_Error__E08 = 1;
                                }
                            }
                            else if( gu8_hot_setting_temperature == HOT_SET_TEMP____85oC )
                            {
                                if(gu8_preheat_Closed_max_timer > 100 || gu8_Hot_Heater_Temperature_One_Degree >= gu8_Hot_Preheat_Temp - 5)
                                {
                                    Bit1_HIF9_Hot_Out_State = F_WaterOut;
                                    bit_Holding_Flow_Block_Error__E08 = 0;
                                }
                                else
                                {
                                    Bit1_HIF9_Hot_Out_State = CLEAR;            // 잠깐 가둬예열해보자
                                    bit_Holding_Flow_Block_Error__E08 = 1;
                                }
                            }
                            //2025-06-05 17:58:36 cbr   //입수30 조건 가둬서 예열 시 과열
                            // else if ( ( gu8_hot_setting_temperature == HOT_SET_TEMP____45oC ) )
                            else if ( ( gu8_hot_setting_temperature == HOT_SET_TEMP____45oC )
                                    || ( gu8_hot_setting_temperature == HOT_SET_TEMP____50oC )
                                    || ( gu8_hot_setting_temperature == HOT_SET_TEMP____55oC ) )     //2025-07-24 cbr
                            {
                                if(gu8_preheat_Closed_max_timer > 10 || gu8_Hot_Heater_Temperature_One_Degree >= gu8_Hot_Preheat_Temp - 5)
                                {
                                    Bit1_HIF9_Hot_Out_State = F_WaterOut;
                                    bit_Holding_Flow_Block_Error__E08 = 0;
                                }
                                else
                                {
                                    Bit1_HIF9_Hot_Out_State = CLEAR;            // 잠깐 가둬예열해보자
                                    bit_Holding_Flow_Block_Error__E08 = 1;
                                }
                            }
                            else
                            {
                                if(gu8_preheat_Closed_max_timer > 60 || gu8_Hot_Heater_Temperature_One_Degree >= gu8_Hot_Preheat_Temp - 5)
                                {
                                    Bit1_HIF9_Hot_Out_State = F_WaterOut;
                                    bit_Holding_Flow_Block_Error__E08 = 0;
                                }
                                else
                                {
                                    Bit1_HIF9_Hot_Out_State = CLEAR;            // 잠깐 가둬예열해보자
                                    bit_Holding_Flow_Block_Error__E08 = 1;
                                }
                            }
                        }
                    }
                }
                else
                {
                        Bit1_HIF9_Hot_Out_State = F_WaterOut;
                }
    // ★2025-06-02 Phil
            }
            else if( gu8_Water_Out_Step == STATE_20_HOT_PRE_HEATING_STATE
                    && gu8_Preheat_Step == STATE_4_PREHEAT_OPERATE_STATE )
            {
                /*..hui [21-5-21오후 8:11:43] 예열 끝나고 스팀 빼주기할때..*/
                Bit1_HIF9_Hot_Out_State = F_WaterOut;
            }
            else if ( gu8_Water_Out_Step == STATE_21_HOT_PRE_HEATING_BUFFER_DRAIN_STATE )
            {
                Bit1_HIF9_Hot_Out_State = SET;
            }
            else if( gu8_Water_Out_Step >= STATE_30_EXTRACT_VALVE_ON_STATE
                    && gu8_Water_Out_Step <= STATE_40_EXTRACT_VALVE_OFF_STATE )
            {
                /*..hui [21-5-21오후 8:11:49] 실제 추출할때..*/
                Bit1_HIF9_Hot_Out_State = SET;
            }
            else
            {
                Bit1_HIF9_Hot_Out_State = CLEAR;
            }
        }
        else
        {
            Bit1_HIF9_Hot_Out_State = CLEAR;
        }
    }
    else
    {
        Bit1_HIF9_Hot_Out_State = CLEAR;
    }
   
    if( bit_ice_tank_ster_start == SET )
    {
        if( gu8_ice_ster_mode == STER_MODE_HOT_PREHEAT )
        {
            /*..hui [25-4-8오전 9:54:35] 예열중..*/
            if( gu8_preheat_ster_step >= 2 )
            {
                Bit2_HIF9_Ice_Tray_Ster_State = SET;
            }
            else
            {
                Bit2_HIF9_Ice_Tray_Ster_State = CLEAR;
            }
        }
        else if( gu8_ice_ster_mode == STER_MODE_HOT_INPUT_STATE )
        {
            /*..hui [25-4-8오전 9:54:41] 실제 트레이 입수중..*/
            if( gu8_hot_input_ster_step >= 0 && gu8_hot_input_ster_step <= 4 )
            {
                Bit2_HIF9_Ice_Tray_Ster_State = SET;
            }
            else
            {
                Bit2_HIF9_Ice_Tray_Ster_State = CLEAR;
            }
        }
        else if( gu8_ice_ster_mode == STER_MODE_DRAIN_PREHEAT )
        {
            /*..hui [25-4-8오전 9:54:35] 예열중..*/
            if( gu8_drain_preheat_ster_step >= 2 )
            {
                Bit2_HIF9_Ice_Tray_Ster_State = SET;
            }
            else
            {
                Bit2_HIF9_Ice_Tray_Ster_State = CLEAR;
            }
        }
        else if( gu8_ice_ster_mode == STER_MODE_DRAIN_INPUT_STATE )
        {
            /*..hui [25-7-7오후 2:28:44] 실제 드레인탱크 입수중..*/
            if( gu8_drain_input_ster_step >= 0 && gu8_drain_input_ster_step <= 4 )
            {
                Bit2_HIF9_Ice_Tray_Ster_State = SET;
            }
            else
            {
                Bit2_HIF9_Ice_Tray_Ster_State = CLEAR;
            }
        }
        else
        {
            Bit2_HIF9_Ice_Tray_Ster_State = CLEAR;
        }
    }
    else
    {
        Bit2_HIF9_Ice_Tray_Ster_State = CLEAR;
    }

    if(gu8_hot_drain_cold == SET)
    {
        Bit3_HIF9_Hot_Drain_Cold_State = SET;
    }
    else
    {
        Bit3_HIF9_Hot_Drain_Cold_State = CLEAR;
    }

/***********************************************************************************************/
    if (u8HotInFeed9ValveOFF > 0)
    {
        pVALVE_HOT_IN = CLEAR;  /*off*/
        bit_hot_in_valve_output = CLEAR;
    }
    else
    {
        if (u16HotInFeed9ValveONs > 0)
        {
            pVALVE_HOT_IN = SET; /*on*/
            bit_hot_in_valve_output = SET;
        }
        else
        {
            pVALVE_HOT_IN = CLEAR;  /*off*/
            bit_hot_in_valve_output = CLEAR;
        }
    }
/***********************************************************************************************/


}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/




