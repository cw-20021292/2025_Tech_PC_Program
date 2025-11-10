/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Main.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  : AT+CONNECT=74F07DB01010
***********************************************************************************************************************/
#include    "Macrodriver.h"
#include    "Global_Variable.h"
#include    "Port_Define.h"
#include    "display_fnd_left.h"
#include    "my_cup.h"

void fnd_left_normal_state(void);
void fnd_left_normal_ice_state(void);
void fnd_left_normal_water_state(void);

void normal_state_pure_temp(void);
void normal_state_cold_temp(void);
void normal_state_hot_temp(void);
//void normal_state_hot_preheat(void);
void fnd_left_front_version(void);
void fnd_left_pba_test(void);
void fnd_left_uart_test(void);
void fnd_left_my_recipe(void);

//void fnd_left_water_out(void);
//void fnd_left_water_out_normal(void);
//void fnd_left_water_out_continue(void);
//void fnd_left_ice_out(void);
void fnd_left_setting(void);
void fnd_left_all_off(void);
void fnd_left_error(void);
void fnd_left_uv(void);
void fnd_left_all_lock(void);
void fnd_left_display_test(void);
void fnd_left_flushing_state(void);
void fnd_left_time_setting(void);


TYPE_BYTE          U8SetDisplayB;
#define            u8SetDisplay                         U8SetDisplayB.byte
#define            Bit0_Ice_Set_Display                 U8SetDisplayB.Bit.b0
#define            Bit1_Cold_Temp_Set_Display           U8SetDisplayB.Bit.b1
#define            Bit2_Steam_Off_Set_Display           U8SetDisplayB.Bit.b2



extern U16 gu16_cold_off_flick_timer;
extern U8 gu8_indicator_flick_timer;
extern U16 gu16_ice_off_flick_timer;
extern U16 gu16_icewater_off_flick_timer;

U8 gu8_temp_hundred;
U8 gu8_temp_ten;
U8 gu8_temp_one;

bit bit_temp_old_update;

bit bit_uv_display_finish;
bit bit_error_display_finish;

U8 gu8_cold_temp_old;
U8 gu8_cold_temp_update_timer;

U8 gu8_current_room_temp_mode;

U8 gu8_Fahrenheit_temp;
U8 gu8_Celcius_temp;

U8 gu8_hot_last_current_temp;

U8 gu8_hour_setting_blink_timer;
bit bit_hour_blink_500ms;

U8 gu8_preheating_display_timer;        // LPP ���� �� �������� ǥ�� Ÿ�̸Ӻ��� �߰� 250611 CH.PARK

U8 gu8_cold_temp_old = CLEAR;
U16 gu16_cold_temp_update_timer;

U8 gu8_pure_temp_old = CLEAR;
U16 gu16_pure_temp_update_timer;
U8 gu8_pure_temp_one_degree_display;

extern bit F_All_Lock;
extern U8 gu8_rtc_time_Hour;
extern bit bit_first_time_setting;
extern U8 gu8_clock_hide_state;
extern U16 gu16_water_select_return_time;
extern bit bit_myWater_setting_start;
extern bit F_IceOut;
extern WATER_OUT_STEP gu8_Water_Out_Step;
extern U8 gu8_Preheat_Step;
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void fnd_left_normal_state(void)
{
    U8 mu8_temporary_hundred = 0;
    U8 mu8_temporary_ten = 0;
    U8 mu8_temporary_one = 0;

    if(bit_display_sleep_start == SET)
    {
        mu8_temporary_hundred = DISPLAY_OFF;
        mu8_temporary_ten = DISPLAY_OFF;
        mu8_temporary_one = DISPLAY_OFF;
        fnd_left_out( FIXED_DISPLAY, mu8_temporary_hundred, mu8_temporary_ten, mu8_temporary_one);
        return;
    }

    if(F_All_Lock == SET)
    {
        return;
    }

    if(bit_myWater_setting_start == SET)
    {
        fnd_left_my_recipe();
        return;
    }

    if(bit_setting_mode_start == SET)
    {
        fnd_left_setting();
        return;
    }

    if( gu8_durable_test_start == SET )
    {
        fnd_left_aging_count();
        return;
    }

    /* �ð� ������ ������ �ð�ǥ�� ���� */
    if(( gu8_cup_led_select == CLEAR ) 
    && ( gu16_water_select_return_time == 0 ) 
    && (bit_myWater_setting_start == 0)
    && (gu8_clock_hide_state == CLEAR) 
    && (F_WaterOut == CLEAR) 
    && (F_IceOut == CLEAR)
    )
    {
        if((GetWifiStatusValue(WIFI_STATUS_SERVER_CON) != CLEAR ) && (GetWifiStatusValue(WIFI_STATUS_PAIRING) == CLEAR ))
        {
            mu8_temporary_hundred = DISPLAY_OFF;
            mu8_temporary_ten = gu8_rtc_time_Hour / 10;
            mu8_temporary_one = gu8_rtc_time_Hour % 10;
            fnd_left_out( FIXED_DISPLAY, mu8_temporary_hundred, mu8_temporary_ten, mu8_temporary_one);
        }
        else
        {
            if(bit_first_time_setting == SET )
            {
                mu8_temporary_hundred = DISPLAY_OFF;
                mu8_temporary_ten = gu8_rtc_time_Hour / 10;
                mu8_temporary_one = gu8_rtc_time_Hour % 10;
                fnd_left_out( FIXED_DISPLAY, mu8_temporary_hundred, mu8_temporary_ten, mu8_temporary_one);
            }
            else
            {
                /* ���� ���� SEGMENT ǥ�� */
                fnd_left_normal_water_state();
            }
        }
    }
    else
    {
        /* ���� OR ������ ���� �� */
        if( u8IceOutState != ICE_SELECT__NONE )
        {
            /*..hui [25-1-9���� 6:44:10] �ӽ�..*/
            fnd_left_normal_ice_state();
        }
        else
        {
            fnd_left_normal_water_state();
        }
    }

    if( gu8_Led_Display_Step == LED_Display__SLEEP )
    {
        set_duty_percent( DIMMING__PERCENT_SEG_LEFT_HUNDRED, SLEEP_MODE_DIMMING_PERCENT );
        set_duty_percent( DIMMING__PERCENT_SEG_LEFT_TEN, SLEEP_MODE_DIMMING_PERCENT );
        set_duty_percent( DIMMING__PERCENT_SEG_LEFT_ONE, SLEEP_MODE_DIMMING_PERCENT );
    }
    else
    {
        set_duty_percent( DIMMING__PERCENT_SEG_LEFT_HUNDRED, left_normal_state_percent );
        set_duty_percent( DIMMING__PERCENT_SEG_LEFT_TEN, left_normal_state_percent );
        set_duty_percent( DIMMING__PERCENT_SEG_LEFT_ONE, left_normal_state_percent );
    }
}

void fnd_left_flushing_state(void)
{
	fnd_left_out( FIXED_DISPLAY, DISPLAY_OFF, DISPLAY_OFF, DISPLAY_OFF);
}
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void fnd_left_normal_ice_state(void)
{
    U8 mu8_temporary_hundred = 0;
    U8 mu8_temporary_ten = 0;
    U8 mu8_temporary_one = 0;
    
    if((Bit2_Cold_Off_Indicator != 0) 
    || (Bit0_Ice_Lock_Indicator != 0)
    || (Bit6_Icewater_Off_Indicator != 0)
    || (Bit5_Ice_Off_Indicator != 0)
    )
    {
        mu8_temporary_hundred = DISPLAY_OFF;
        mu8_temporary_ten = DISPLAY_OFF;
        mu8_temporary_one = DISPLAY_OFF;
    }
    else
    {
        /* ���� �ܰ� ǥ�� */
        mu8_temporary_hundred = DISPLAY_OFF;
        mu8_temporary_ten =  DISPLAY_OFF;
        mu8_temporary_one = gu8_ice_amount_step;
    }

    if( gu8_Led_Display_Step == LED_Display__SLEEP )
    {
        set_duty_percent( DIMMING__PERCENT_SEG_LEFT_HUNDRED, SLEEP_MODE_DIMMING_PERCENT );
        set_duty_percent( DIMMING__PERCENT_SEG_LEFT_TEN, SLEEP_MODE_DIMMING_PERCENT );
        set_duty_percent( DIMMING__PERCENT_SEG_LEFT_ONE, SLEEP_MODE_DIMMING_PERCENT );
    }
    else
    {
        set_duty_percent( DIMMING__PERCENT_SEG_LEFT_HUNDRED, left_normal_state_percent );
        set_duty_percent( DIMMING__PERCENT_SEG_LEFT_TEN, left_normal_state_percent );
        set_duty_percent( DIMMING__PERCENT_SEG_LEFT_ONE, left_normal_state_percent );
    }

    fnd_left_out( FIXED_DISPLAY, mu8_temporary_hundred, mu8_temporary_ten, mu8_temporary_one);
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void fnd_left_normal_water_state(void)
{
    U8 mu8_temporary_hundred = 0;
    U8 mu8_temporary_ten = 0;
    U8 mu8_temporary_one = 0;

    if((Bit2_Cold_Off_Indicator != 0) 
    || (Bit0_Ice_Lock_Indicator != 0)
    || (Bit6_Icewater_Off_Indicator != 0)
    )
    {
        gu8_temp_hundred = DISPLAY_OFF;
        gu8_temp_ten = DISPLAY_OFF;
        gu8_temp_one = DISPLAY_OFF;
    }
    else
    {
        #ifdef __AC_MOTOR_TEST_MODE__
        gu8_temp_hundred = (U8)(gu32_feeder_screw_ccw_count / 100000);
        gu8_temp_ten = (U8)((gu32_feeder_screw_ccw_count % 100000) / 10000);
        gu8_temp_one = (U8)((gu32_feeder_screw_ccw_count % 10000) / 1000);
        #else
        if( u8IceOutState != ICE_SELECT__NONE )
        {
            gu8_temp_hundred = DISPLAY_OFF;
            gu8_temp_ten =  DISPLAY_OFF;
            gu8_temp_one = gu8_ice_amount_step;
        }
        else if( u8WaterOutState == PURE_WATER_SELECT )
        {
            normal_state_pure_temp();
        }
        else if( u8WaterOutState == COLD_WATER_SELECT )
        {
            normal_state_cold_temp();
        }
		else /*if( u8WaterOutState == HOT_WATER_SELECT )*/
		{
		    if( F_Hot_Enable == CLEAR )
	    	{
    			if(gu16_hot_off_flick_timer != 0 || gu8_indicator_flick_timer != 0)
    			{
    				gu8_temp_hundred = DISPLAY_NUM_LARGE_O;
    				gu8_temp_ten = DISPLAY_NUM_LARGE_F;
    				gu8_temp_one = DISPLAY_NUM_LARGE_F;
    			}
				else
				{
					normal_state_hot_temp();
				}
	    	}
			else
			{
				normal_state_hot_temp();
			}
		}
        #endif
    }

    mu8_temporary_hundred = gu8_temp_hundred;
    mu8_temporary_ten = gu8_temp_ten;
    mu8_temporary_one = gu8_temp_one;

    if( gu8_Led_Display_Step == LED_Display__SLEEP )
    {
        set_duty_percent( DIMMING__PERCENT_SEG_LEFT_HUNDRED, SLEEP_MODE_DIMMING_PERCENT );
        set_duty_percent( DIMMING__PERCENT_SEG_LEFT_TEN, SLEEP_MODE_DIMMING_PERCENT );
        set_duty_percent( DIMMING__PERCENT_SEG_LEFT_ONE, SLEEP_MODE_DIMMING_PERCENT );
    }
    else
    {
        set_duty_percent( DIMMING__PERCENT_SEG_LEFT_HUNDRED, left_normal_state_percent );
        set_duty_percent( DIMMING__PERCENT_SEG_LEFT_TEN, left_normal_state_percent );
        set_duty_percent( DIMMING__PERCENT_SEG_LEFT_ONE, left_normal_state_percent );
    }

    fnd_left_out( FIXED_DISPLAY, mu8_temporary_hundred, mu8_temporary_ten, mu8_temporary_one);
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void normal_state_pure_temp(void)
{
    U8 mu8_pure_temp = 0;

    mu8_pure_temp = gu8_Hot_In_Temperature_One_Degree;

    if( gu8_pure_temp_old == 0 || gu8_pure_temp_one_degree_display == 0 )
    {
        gu8_pure_temp_one_degree_display = mu8_pure_temp;
    }
    else{}

    //  �µ� ����� �ĺ��� 1��...
    if(gu8_pure_temp_old != mu8_pure_temp)
    {
        gu8_pure_temp_old = mu8_pure_temp;
        gu16_pure_temp_update_timer = 0;
    }
    else
    {
        gu16_pure_temp_update_timer++;

		/*..sean [25-06-10] ���� ������Ʈ ���� 60���� 30�ʷ� ����..*/
        //if( gu16_cold_temp_update_timer >= 600 )
        if(gu16_pure_temp_update_timer >= 300)
        {
            gu16_pure_temp_update_timer = 300;
            gu8_pure_temp_one_degree_display = mu8_pure_temp;
        }
        else{}
    }


	/*.. sean [25-06-08] 30�̻��� ��쿡�� 30���� �����Ͽ� ǥ�� ..*/
	if(gu8_pure_temp_one_degree_display <= 2)
	{
		gu8_pure_temp_one_degree_display = 2;
	}
    else if(gu8_pure_temp_one_degree_display >= 30)
	{
		gu8_pure_temp_one_degree_display = 30;
	}
	else {  }

    if( Bit4_Hot_In_Temp_Open_Short_Error__E46 == SET )
    {
        /*..hui [25-6-10���� 3:04:51] �¼��Լ����� ������ 25�� ���� ǥ��..*/
        gu8_pure_temp_one_degree_display = 25;
    }
    else{}

    #ifdef __DUMMY_PROGRAM__
    gu8_pure_temp_one_degree_display = 25;
    #endif

    /*..hui [24-1-31���� 9:14:53] 10�̸��϶��� ���� 2�� OFF�� ǥ��..*/
    if( gu8_pure_temp_one_degree_display < 10 )
    {
        gu8_temp_hundred = DISPLAY_OFF;
        gu8_temp_ten = DISPLAY_OFF;
        gu8_temp_one = gu8_pure_temp_one_degree_display;
    }
    else if( gu8_pure_temp_one_degree_display < 100 )
    {
        gu8_temp_hundred = DISPLAY_OFF;
        gu8_temp_ten = (gu8_pure_temp_one_degree_display / 10);
        gu8_temp_one = (gu8_pure_temp_one_degree_display % 10);
    }
    else
    {
        gu8_temp_hundred = (U8)((gu8_pure_temp_one_degree_display / 100) % 10);
        gu8_temp_ten = (U8)((gu8_pure_temp_one_degree_display / 10) % 10);
        gu8_temp_one = (U8)(gu8_pure_temp_one_degree_display % 10);
    }

//     #if 1
// 		if( gu8_Room_Temperature_One_Degree > 30 )
// 		{
// 				gu8_temp_hundred = DISPLAY_OFF;
// 				gu8_temp_ten = DISPLAY_NUM_3;
// 				gu8_temp_one = DISPLAY_NUM_0;
// 				//gu8_temp_ten = DISPLAY_NUM_2;
// 				//gu8_temp_one = DISPLAY_NUM_7;
// 				return;
// 		}
// #endif
//     /*..hui [24-1-31���� 9:14:53] 10�̸��϶��� ���� 2�� OFF�� ǥ��..*/
//     if( gu8_Room_Temperature_One_Degree < 10 )
//     {
//         gu8_temp_hundred = DISPLAY_OFF;
//         gu8_temp_ten = DISPLAY_OFF;
//         gu8_temp_one = gu8_Room_Temperature_One_Degree;
//     }
//     else if( gu8_Room_Temperature_One_Degree < 100 )
//     {
//         gu8_temp_hundred = DISPLAY_OFF;
//         gu8_temp_ten = (gu8_Room_Temperature_One_Degree / 10);
//         gu8_temp_one = (gu8_Room_Temperature_One_Degree % 10);
// 				//gu8_temp_ten = DISPLAY_NUM_2;
//         //gu8_temp_one = DISPLAY_NUM_7;
//     }
//     else
//     {
//         gu8_temp_hundred = (U8)((gu8_Room_Temperature_One_Degree / 100) % 10);
//         gu8_temp_ten = (U8)((gu8_Room_Temperature_One_Degree / 10) % 10);
//         gu8_temp_one = (U8)(gu8_Room_Temperature_One_Degree % 10);
//     }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void normal_state_cold_temp(void)
{
    U8 mu8_Fahrenheit_temp = 0;
    U8 mu8_Celcius_temp = 0;

    #ifdef __DUMMY_PROGRAM__
    gu8_Celcius_temp = 6;
    #endif

    if( Bit14_Cold_Temp_Open_Short_Error__E44 == SET )
    {
        /*..hui [25-6-10���� 2:50:25] �ü����� �����϶��� --- ǥ��..*/
        gu8_temp_hundred = DISPLAY_BAR;
        gu8_temp_ten = DISPLAY_BAR;
        gu8_temp_one = DISPLAY_BAR;
    }
    else if( F_Cold_Enable == CLEAR )
    {
        /*..hui [25-6-10���� 2:50:32] �ü� OFF�϶��� OFF ǥ�� ����..*/
        gu8_temp_hundred = DISPLAY_NUM_LARGE_O;
        gu8_temp_ten = DISPLAY_NUM_LARGE_F;
        gu8_temp_one = DISPLAY_NUM_LARGE_F;
    }
    else
    {
        /*..hui [24-1-31���� 9:14:53] 10�̸��϶��� ���� 2�� OFF�� ǥ��..*/
        if( gu8_Celcius_temp < 10 )
        {
            gu8_temp_hundred = DISPLAY_OFF;
            gu8_temp_ten = DISPLAY_OFF;
            gu8_temp_one = gu8_Celcius_temp;
        }
        else if( gu8_Celcius_temp < 100 )
        {
            gu8_temp_hundred = DISPLAY_OFF;
            gu8_temp_ten = (gu8_Celcius_temp / 10);
            gu8_temp_one = (gu8_Celcius_temp % 10);
        }
        else
        {
            gu8_temp_hundred = (U8)((gu8_Celcius_temp / 100) % 10);
            gu8_temp_ten = (U8)((gu8_Celcius_temp / 10) % 10);
            gu8_temp_one = (U8)(gu8_Celcius_temp % 10);
        }
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void normal_state_hot_temp(void)
{
    if(F_WaterOut == SET)
    {
        if((gu8_Water_Out_Step >= STATE_20_HOT_PRE_HEATING_STATE)
        && (gu8_Water_Out_Step <= STATE_23_HOT_PRE_HEATING_PRESS_DOWN_STATE)
        )
        {
            // 0.7�ʸ��� ���� �������� ǥ��
            gu8_preheating_display_timer++;
            if(gu8_preheating_display_timer <= 7)
            {
                gu8_temp_hundred = DISPLAY_NUM_PREHEAT_UNDER_BAR;
                gu8_temp_ten = DISPLAY_NUM_PREHEAT_UNDER_BAR;
                gu8_temp_one = DISPLAY_NUM_PREHEAT_UNDER_BAR;
            }
            else if(gu8_preheating_display_timer <= 14)
            {
                gu8_temp_hundred = DISPLAY_NUM_PREHEAT_MIDDLE_BAR;
                gu8_temp_ten = DISPLAY_NUM_PREHEAT_MIDDLE_BAR;
                gu8_temp_one = DISPLAY_NUM_PREHEAT_MIDDLE_BAR;
            }
            else if(gu8_preheating_display_timer <= 21)
            {
                gu8_temp_hundred = DISPLAY_NUM_PREHEAT_UPPER_BAR;
                gu8_temp_ten = DISPLAY_NUM_PREHEAT_UPPER_BAR;
                gu8_temp_one = DISPLAY_NUM_PREHEAT_UPPER_BAR;
            }
            else
            {
                gu8_preheating_display_timer = 0;
            }   
        }
        else
        {
            gu8_preheating_display_timer = 0;

            /* �����߿� ���� �Ϸ�Ǹ� �����µ� ǥ�� */
            if( gu8_hot_setting_temperature == HOT_SET_TEMP____100oC )
            {
                gu8_temp_hundred = DISPLAY_NUM_1;
                gu8_temp_ten = DISPLAY_NUM_0;
                gu8_temp_one = DISPLAY_NUM_0;
            }
            else if( gu8_hot_setting_temperature == HOT_SET_TEMP____95oC )
            {
                gu8_temp_hundred = DISPLAY_OFF;
                gu8_temp_ten = DISPLAY_NUM_9;
                gu8_temp_one = DISPLAY_NUM_5;
            }
            else if( gu8_hot_setting_temperature == HOT_SET_TEMP____90oC )
            {
                gu8_temp_hundred = DISPLAY_OFF;
                gu8_temp_ten = DISPLAY_NUM_9;
                gu8_temp_one = DISPLAY_NUM_0;
            }
            else if( gu8_hot_setting_temperature == HOT_SET_TEMP____85oC )
            {
                gu8_temp_hundred = DISPLAY_OFF;
                gu8_temp_ten = DISPLAY_NUM_8;
                gu8_temp_one = DISPLAY_NUM_5;
            }
            else if( gu8_hot_setting_temperature == HOT_SET_TEMP____80oC )
            {
                gu8_temp_hundred = DISPLAY_OFF;
                gu8_temp_ten = DISPLAY_NUM_8;
                gu8_temp_one = DISPLAY_NUM_0;
            }
            else if( gu8_hot_setting_temperature == HOT_SET_TEMP____75oC )
            {
                gu8_temp_hundred = DISPLAY_OFF;
                gu8_temp_ten = DISPLAY_NUM_7;
                gu8_temp_one = DISPLAY_NUM_5;
            }
            else if( gu8_hot_setting_temperature == HOT_SET_TEMP____70oC )
            {
                gu8_temp_hundred = DISPLAY_OFF;
                gu8_temp_ten = DISPLAY_NUM_7;
                gu8_temp_one = DISPLAY_NUM_0;
            }
            else if( gu8_hot_setting_temperature == HOT_SET_TEMP____65oC )
            {
                gu8_temp_hundred = DISPLAY_OFF;
                gu8_temp_ten = DISPLAY_NUM_6;
                gu8_temp_one = DISPLAY_NUM_5;
            }
            else if( gu8_hot_setting_temperature == HOT_SET_TEMP____60oC )
            {
                gu8_temp_hundred = DISPLAY_OFF;
                gu8_temp_ten = DISPLAY_NUM_6;
                gu8_temp_one = DISPLAY_NUM_0;
            }
            else if( gu8_hot_setting_temperature == HOT_SET_TEMP____55oC )
            {
                gu8_temp_hundred = DISPLAY_OFF;
                gu8_temp_ten = DISPLAY_NUM_5;
                gu8_temp_one = DISPLAY_NUM_5;
            }
            else if( gu8_hot_setting_temperature == HOT_SET_TEMP____50oC )
            {
                gu8_temp_hundred = DISPLAY_OFF;
                gu8_temp_ten = DISPLAY_NUM_5;
                gu8_temp_one = DISPLAY_NUM_0;
            }
            else /*if( gu8_hot_setting_temperature == HOT_SET_TEMP_1__MILK__43_oC )*/
            {
                gu8_temp_hundred = DISPLAY_OFF;
                gu8_temp_ten = DISPLAY_NUM_4;
                gu8_temp_one = DISPLAY_NUM_5;
            }
        }
    }
    else
    {
        gu8_preheating_display_timer = 0;

        if( gu8_hot_setting_temperature == HOT_SET_TEMP____100oC )
        {
            gu8_temp_hundred = DISPLAY_NUM_1;
            gu8_temp_ten = DISPLAY_NUM_0;
            gu8_temp_one = DISPLAY_NUM_0;
        }
        else if( gu8_hot_setting_temperature == HOT_SET_TEMP____95oC )
        {
            gu8_temp_hundred = DISPLAY_OFF;
            gu8_temp_ten = DISPLAY_NUM_9;
            gu8_temp_one = DISPLAY_NUM_5;
        }
        else if( gu8_hot_setting_temperature == HOT_SET_TEMP____90oC )
        {
            gu8_temp_hundred = DISPLAY_OFF;
            gu8_temp_ten = DISPLAY_NUM_9;
            gu8_temp_one = DISPLAY_NUM_0;
        }
        else if( gu8_hot_setting_temperature == HOT_SET_TEMP____85oC )
        {
            gu8_temp_hundred = DISPLAY_OFF;
            gu8_temp_ten = DISPLAY_NUM_8;
            gu8_temp_one = DISPLAY_NUM_5;
        }
        else if( gu8_hot_setting_temperature == HOT_SET_TEMP____80oC )
        {
            gu8_temp_hundred = DISPLAY_OFF;
            gu8_temp_ten = DISPLAY_NUM_8;
            gu8_temp_one = DISPLAY_NUM_0;
        }
        else if( gu8_hot_setting_temperature == HOT_SET_TEMP____75oC )
        {
            gu8_temp_hundred = DISPLAY_OFF;
            gu8_temp_ten = DISPLAY_NUM_7;
            gu8_temp_one = DISPLAY_NUM_5;
        }
        else if( gu8_hot_setting_temperature == HOT_SET_TEMP____70oC )
        {
            gu8_temp_hundred = DISPLAY_OFF;
            gu8_temp_ten = DISPLAY_NUM_7;
            gu8_temp_one = DISPLAY_NUM_0;
        }
        else if( gu8_hot_setting_temperature == HOT_SET_TEMP____65oC )
        {
            gu8_temp_hundred = DISPLAY_OFF;
            gu8_temp_ten = DISPLAY_NUM_6;
            gu8_temp_one = DISPLAY_NUM_5;
        }
        else if( gu8_hot_setting_temperature == HOT_SET_TEMP____60oC )
        {
            gu8_temp_hundred = DISPLAY_OFF;
            gu8_temp_ten = DISPLAY_NUM_6;
            gu8_temp_one = DISPLAY_NUM_0;
        }
        else if( gu8_hot_setting_temperature == HOT_SET_TEMP____55oC )
        {
            gu8_temp_hundred = DISPLAY_OFF;
            gu8_temp_ten = DISPLAY_NUM_5;
            gu8_temp_one = DISPLAY_NUM_5;
        }
        else if( gu8_hot_setting_temperature == HOT_SET_TEMP____50oC )
        {
            gu8_temp_hundred = DISPLAY_OFF;
            gu8_temp_ten = DISPLAY_NUM_5;
            gu8_temp_one = DISPLAY_NUM_0;
        }
        else /*if( gu8_hot_setting_temperature == HOT_SET_TEMP_1__MILK__43_oC )*/
        {
            gu8_temp_hundred = DISPLAY_OFF;
            gu8_temp_ten = DISPLAY_NUM_4;
            gu8_temp_one = DISPLAY_NUM_5;
        }
    }
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void fnd_left_setting(void)
{
    U8 mu8_temporary_hundred = 0;
    U8 mu8_temporary_ten = 0;
    U8 mu8_temporary_one = 0;

    mu8_temporary_hundred = DISPLAY_OFF;
    mu8_temporary_ten = DISPLAY_OFF;
    mu8_temporary_one = DISPLAY_OFF;

    fnd_left_out( FIXED_DISPLAY, mu8_temporary_hundred, mu8_temporary_ten, mu8_temporary_one);

    // U8 mu8_temporary\_hundred = 0;
    // U8 mu8_temporary_ten = 0;
    // U8 mu8_temporary_one = 0;
    // U8 mu8_flick_state = 0;


    // /*..hui [23-2-7���� 7:55:29] ICE OFF, Cold Temp, Eco Mode..*/
    // switch( u8SetDisplay )
    // {
    //     case ON_OFF_DISPLAY_OFF:

    //         mu8_temporary_hundred = DISPLAY_OFF;
    //         mu8_temporary_ten = DISPLAY_OFF;
    //         mu8_temporary_one = DISPLAY_OFF;
    //         gu8_on_off_flick_cnt = 0;
    //         gu16_on_off_flick_timer = 0;

    //     break;

    //     case ON_OFF_DISPLAY_STEAM_OFF_SET:

    //         /*mu8_flick_state = on_off_flick_timer(5, 5);*/
    //         mu8_flick_state = on_off_flick_timer(30, 0);

    //         if( mu8_flick_state == SET )
    //         {
    //             if( bit_periodic_ster_enable == SET )
    //             {
    //                 mu8_temporary_hundred = DISPLAY_NUM_LARGE_O;
    //                 mu8_temporary_ten = DISPLAY_NUM_SMALL_n;
    //                 mu8_temporary_one = DISPLAY_OFF;
    //             }
    //             else
    //             {
    //                 mu8_temporary_hundred = DISPLAY_NUM_LARGE_O;
    //                 mu8_temporary_ten = DISPLAY_NUM_LARGE_F;
    //                 mu8_temporary_one = DISPLAY_NUM_LARGE_F;
    //             }
    //         }
    //         else
    //         {
    //             mu8_temporary_hundred = DISPLAY_OFF;
    //             mu8_temporary_ten = DISPLAY_OFF;
    //             mu8_temporary_one = DISPLAY_OFF;
    //         }

    //     break;

    //     default:

    //         u8SetDisplay = ON_OFF_DISPLAY_OFF;
    //         gu8_on_off_flick_cnt = 0;
    //         gu16_on_off_flick_timer = 0;

    //     break;
    // }

    // fnd_left_out( FIXED_DISPLAY, mu8_temporary_hundred, mu8_temporary_ten, mu8_temporary_one);
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/

void fnd_left_time_setting(void)
{
    U8 mu8_temporary_hundred = 0;
    U8 mu8_temporary_ten = 0;
    U8 mu8_temporary_one = 0;

    if( gu8_time_setting_step == SETTING_TIME_HOUR )
    {
        gu8_hour_setting_blink_timer++;

        if( gu8_hour_setting_blink_timer >= 5 )
        {
                if( u8TimeSettingLongKey > 0 )
                {
                    gu8_hour_setting_blink_timer = 0;
                    bit_hour_blink_500ms = CLEAR;
                }
                else
                {
                    gu8_hour_setting_blink_timer = 0;
                    bit_hour_blink_500ms ^= SET;
                }
        }
        else{}

        if( bit_hour_blink_500ms == CLEAR )
        {
            mu8_temporary_hundred = DISPLAY_OFF;
            mu8_temporary_ten = gu8_temporary_Hour / 10;
            mu8_temporary_one = gu8_temporary_Hour % 10;

        }
        else // ( bit_hour_blink_500ms == SET )
        {
            mu8_temporary_hundred = DISPLAY_OFF;
            mu8_temporary_ten = DISPLAY_OFF;
            mu8_temporary_one = DISPLAY_OFF;
        }
    }
    else
    {
        mu8_temporary_hundred = DISPLAY_OFF;
        mu8_temporary_ten = gu8_temporary_Hour / 10;
        mu8_temporary_one = gu8_temporary_Hour % 10;
    }

    if( gu8_Led_Display_Step == LED_Display__SLEEP )
    {
        set_duty_percent( DIMMING__PERCENT_SEG_LEFT_HUNDRED, SLEEP_MODE_DIMMING_PERCENT );
        set_duty_percent( DIMMING__PERCENT_SEG_LEFT_TEN, SLEEP_MODE_DIMMING_PERCENT );
        set_duty_percent( DIMMING__PERCENT_SEG_LEFT_ONE, SLEEP_MODE_DIMMING_PERCENT );
    }
    else
    {
        set_duty_percent( DIMMING__PERCENT_SEG_LEFT_HUNDRED, left_normal_state_percent );
        set_duty_percent( DIMMING__PERCENT_SEG_LEFT_TEN, left_normal_state_percent );
        set_duty_percent( DIMMING__PERCENT_SEG_LEFT_ONE, left_normal_state_percent );
    }

	fnd_left_out( FIXED_DISPLAY, mu8_temporary_hundred, mu8_temporary_ten, mu8_temporary_one);

}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void fnd_left_error(void)
{
    U8 mu8_temporary_hundred = 0;
    U8 mu8_temporary_ten = 0;
    U8 mu8_temporary_one = 0;
    U8 mu8_flick_state = 0;

    if( bit_error_display_finish == SET )
    {
        bit_error_display_finish = CLEAR;
        gu8_error_flick_timer = EERROR_DISPLAY_TIME;
    }
    else{}

    /*mu8_flick_state = error_flick_timer(5, 5);*/
    mu8_flick_state = error_flick_timer(EERROR_DISPLAY_TIME, 5);

    if( mu8_flick_state == SET )
    {
        /*mu8_temporary_hundred = DISPLAY_OFF;*/
        /*..hui [23-11-24���� 2:30:48] ���� ������ �����鼭 Exx�� ǥ��..*/
        mu8_temporary_hundred = DISPLAY_NUM_LARGE_E;
        mu8_temporary_ten = (gu8_Error_Code / 10);
        mu8_temporary_one = (gu8_Error_Code % 10);
    }
    else
    {
        mu8_temporary_hundred = DISPLAY_OFF;
        mu8_temporary_ten = DISPLAY_OFF;
        mu8_temporary_one = DISPLAY_OFF;
    }

    fnd_left_out( FIXED_DISPLAY, mu8_temporary_hundred, mu8_temporary_ten, mu8_temporary_one);
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void fnd_left_uv(void)
{
    U8 mu8_temporary_hundred = 0;
    U8 mu8_temporary_ten = 0;
    U8 mu8_temporary_one = 0;
    U8 mu8_flick_state = 0;

    if( bit_uv_display_finish == SET )
    {
        /*bit_uv_display_finish = CLEAR;*/
        gu16_uv_percent_flick_timer = UV_PROGRESS_DISPLAY_TIME;
    }
    else{}

    /*mu8_flick_state = error_flick_timer(5, 5);*/
    mu8_flick_state = uv_percent_display_timer(UV_PROGRESS_DISPLAY_TIME);

    if( mu8_flick_state == SET )
    {
        if( gu8_display_uv_elapsed_percent < 100 )
        {
            if( gu8_display_uv_elapsed_percent == 0 )
            {
                mu8_temporary_hundred = DISPLAY_OFF;
                mu8_temporary_ten = DISPLAY_OFF;
                mu8_temporary_one = DISPLAY_NUM_0;
            }
            else
            {
                mu8_temporary_hundred = DISPLAY_OFF;
                mu8_temporary_ten = (gu8_display_uv_elapsed_percent / 10);
                mu8_temporary_one = (gu8_display_uv_elapsed_percent % 10);
            }
        }
        else
        {
            mu8_temporary_hundred = DISPLAY_NUM_1;
            mu8_temporary_ten = DISPLAY_NUM_0;
            mu8_temporary_one = DISPLAY_NUM_0;
        }
    }
    else
    {
        mu8_temporary_hundred = DISPLAY_OFF;
        mu8_temporary_ten = DISPLAY_OFF;
        mu8_temporary_one = DISPLAY_OFF;
    }

    fnd_left_out( FIXED_DISPLAY, mu8_temporary_hundred, mu8_temporary_ten, mu8_temporary_one);
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void fnd_left_all_lock(void)
{
    U8 mu8_temporary_hundred = 0;
    U8 mu8_temporary_ten = 0;
    U8 mu8_temporary_one = 0;
    U8 mu8_flick_state = 0;

    mu8_temporary_hundred = DISPLAY_OFF;
    mu8_temporary_ten = DISPLAY_OFF;
    mu8_temporary_one = DISPLAY_OFF;

    fnd_left_out( FIXED_DISPLAY, mu8_temporary_hundred, mu8_temporary_ten, mu8_temporary_one);
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void fnd_left_my_recipe(void)
{
	U8 mu8_temporary_hundred = 0;
	U8 mu8_temporary_ten = 0;
	U8 mu8_temporary_one = 0;

    if((gu8_Water_Out_Step >= STATE_20_HOT_PRE_HEATING_STATE)
    && (gu8_Water_Out_Step <= STATE_23_HOT_PRE_HEATING_PRESS_DOWN_STATE)
    )
    {
        // 0.7�ʸ��� ���� �������� ǥ��
        gu8_preheating_display_timer++;
        if(gu8_preheating_display_timer <= 7)
        {
            mu8_temporary_hundred = DISPLAY_NUM_PREHEAT_UNDER_BAR;
            mu8_temporary_ten = DISPLAY_NUM_PREHEAT_UNDER_BAR;
            mu8_temporary_one = DISPLAY_NUM_PREHEAT_UNDER_BAR;
        }
        else if(gu8_preheating_display_timer <= 14)
        {
            mu8_temporary_hundred = DISPLAY_NUM_PREHEAT_MIDDLE_BAR;
            mu8_temporary_ten = DISPLAY_NUM_PREHEAT_MIDDLE_BAR;
            mu8_temporary_one = DISPLAY_NUM_PREHEAT_MIDDLE_BAR;
        }
        else if(gu8_preheating_display_timer <= 21)
        {
            mu8_temporary_hundred = DISPLAY_NUM_PREHEAT_UPPER_BAR;
            mu8_temporary_ten = DISPLAY_NUM_PREHEAT_UPPER_BAR;
            mu8_temporary_one = DISPLAY_NUM_PREHEAT_UPPER_BAR;
        }
        else
        {
            gu8_preheating_display_timer = 0;

            // ���� �ȳ����� 000���� ����
            mu8_temporary_hundred = DISPLAY_NUM_PREHEAT_UPPER_BAR;
            mu8_temporary_ten = DISPLAY_NUM_PREHEAT_UPPER_BAR;
            mu8_temporary_one = DISPLAY_NUM_PREHEAT_UPPER_BAR;
        }   
    }
    else
    {
        if(my_recipe_select == RECIPE_RAMEN)
        {
            mu8_temporary_hundred = DISPLAY_NUM_1;
            mu8_temporary_ten = DISPLAY_NUM_0;
            mu8_temporary_one = DISPLAY_NUM_0;
        }
        else if(my_recipe_select == RECIPE_DRIPCOFFEE)
        {
            mu8_temporary_hundred = DISPLAY_NUM_1;
            mu8_temporary_ten = DISPLAY_NUM_0;
            mu8_temporary_one = DISPLAY_NUM_0;
        }
        else if(my_recipe_select == RECIPE_TEA)
        {
            mu8_temporary_hundred = DISPLAY_NUM_1;
            mu8_temporary_ten = DISPLAY_NUM_0;
            mu8_temporary_one = DISPLAY_NUM_0;
        }
        else if(my_recipe_select == MY_INDEX_MY1)
        {
            if(my_setting[RECIPE_MY1].temp == 6)
            {
                if(F_Cold_Enable == SET)
                {
                    /*..hui [24-1-31���� 9:14:53] 10�̸��϶��� ���� 2�� OFF�� ǥ��..*/
                    if( gu8_Celcius_temp < 10 )
                    {
                        mu8_temporary_hundred = DISPLAY_OFF;
                        mu8_temporary_ten = DISPLAY_OFF;
                        mu8_temporary_one = gu8_Celcius_temp;
                    }
                    else if( gu8_Celcius_temp < 100 )
                    {
                        mu8_temporary_hundred = DISPLAY_OFF;
                        mu8_temporary_ten = (gu8_Celcius_temp / 10);
                        mu8_temporary_one = (gu8_Celcius_temp % 10);
                    }
                    else
                    {
                        mu8_temporary_hundred = (U8)((gu8_Celcius_temp / 100) % 10);
                        mu8_temporary_ten = (U8)((gu8_Celcius_temp / 10) % 10);
                        mu8_temporary_one = (U8)(gu8_Celcius_temp % 10);
                    }
                }
                else
                {
                    mu8_temporary_hundred = DISPLAY_NUM_LARGE_O;
                    mu8_temporary_ten = DISPLAY_NUM_LARGE_F;
                    mu8_temporary_one = DISPLAY_NUM_LARGE_F;
                }
            }
            else
            {			
                mu8_temporary_hundred = (U8)((my_setting[RECIPE_MY1].temp / 100));
                if(mu8_temporary_hundred == 0)
                {
                    mu8_temporary_hundred = DISPLAY_OFF;
                }

                mu8_temporary_ten = (U8)((my_setting[RECIPE_MY1].temp / 10) % 10);
                if(mu8_temporary_hundred == DISPLAY_OFF && mu8_temporary_ten == 0)
                {
                    mu8_temporary_ten = DISPLAY_OFF;
                }

                mu8_temporary_one = (U8)((my_setting[RECIPE_MY1].temp ) % 10);
            }
        }
        else if(my_recipe_select == MY_INDEX_MY2)
        {
            if(my_setting[RECIPE_MY2].temp == 6)
            {
                if(F_Cold_Enable == SET)
                {
                    /*..hui [24-1-31���� 9:14:53] 10�̸��϶��� ���� 2�� OFF�� ǥ��..*/
                    if( gu8_Celcius_temp < 10 )
                    {
                        mu8_temporary_hundred = DISPLAY_OFF;
                        mu8_temporary_ten = DISPLAY_OFF;
                        mu8_temporary_one = gu8_Celcius_temp;
                    }
                    else if( gu8_Celcius_temp < 100 )
                    {
                        mu8_temporary_hundred = DISPLAY_OFF;
                        mu8_temporary_ten = (gu8_Celcius_temp / 10);
                        mu8_temporary_one = (gu8_Celcius_temp % 10);
                    }
                    else
                    {
                        mu8_temporary_hundred = (U8)((gu8_Celcius_temp / 100) % 10);
                        mu8_temporary_ten = (U8)((gu8_Celcius_temp / 10) % 10);
                        mu8_temporary_one = (U8)(gu8_Celcius_temp % 10);
                    }
                }
                else
                {
                    mu8_temporary_hundred = DISPLAY_NUM_LARGE_O;
                    mu8_temporary_ten = DISPLAY_NUM_LARGE_F;
                    mu8_temporary_one = DISPLAY_NUM_LARGE_F;
                }
            }
            else
            {			
                mu8_temporary_hundred = (U8)((my_setting[RECIPE_MY2].temp / 100));
                if(mu8_temporary_hundred == 0)
                {
                    mu8_temporary_hundred = DISPLAY_OFF;
                }

                mu8_temporary_ten = (U8)((my_setting[RECIPE_MY2].temp / 10) % 10);
                if(mu8_temporary_hundred == DISPLAY_OFF && mu8_temporary_ten == 0)
                {
                    mu8_temporary_ten = DISPLAY_OFF;
                }

                mu8_temporary_one = (U8)((my_setting[RECIPE_MY2].temp ) % 10);
            }
        }
        else if(my_recipe_select == MY_INDEX_MY3)
        {
            if(my_setting[RECIPE_MY3].temp == 6)
            {
                if(F_Cold_Enable == SET)
                {
                    /*..hui [24-1-31���� 9:14:53] 10�̸��϶��� ���� 2�� OFF�� ǥ��..*/
                    if( gu8_Celcius_temp < 10 )
                    {
                        mu8_temporary_hundred = DISPLAY_OFF;
                        mu8_temporary_ten = DISPLAY_OFF;
                        mu8_temporary_one = gu8_Celcius_temp;
                    }
                    else if( gu8_Celcius_temp < 100 )
                    {
                        mu8_temporary_hundred = DISPLAY_OFF;
                        mu8_temporary_ten = (gu8_Celcius_temp / 10);
                        mu8_temporary_one = (gu8_Celcius_temp % 10);
                    }
                    else
                    {
                        mu8_temporary_hundred = (U8)((gu8_Celcius_temp / 100) % 10);
                        mu8_temporary_ten = (U8)((gu8_Celcius_temp / 10) % 10);
                        mu8_temporary_one = (U8)(gu8_Celcius_temp % 10);
                    }
                }
                else
                {
                    mu8_temporary_hundred = DISPLAY_NUM_LARGE_O;
                    mu8_temporary_ten = DISPLAY_NUM_LARGE_F;
                    mu8_temporary_one = DISPLAY_NUM_LARGE_F;
                }
            }
            else
            {			
                mu8_temporary_hundred = (U8)((my_setting[RECIPE_MY3].temp / 100));
                if(mu8_temporary_hundred == 0)
                {
                    mu8_temporary_hundred = DISPLAY_OFF;
                }

                mu8_temporary_ten = (U8)((my_setting[RECIPE_MY3].temp / 10) % 10);
                if(mu8_temporary_hundred == DISPLAY_OFF && mu8_temporary_ten == 0)
                {
                    mu8_temporary_ten = DISPLAY_OFF;
                }

                mu8_temporary_one = (U8)((my_setting[RECIPE_MY3].temp ) % 10);
            }
        }
    }
        
    if(F_WaterOut == CLEAR)
    {
        gu8_preheating_display_timer = 0;
    }
    else {  }

    if( gu8_Led_Display_Step == LED_Display__SLEEP )
    {
        set_duty_percent( DIMMING__PERCENT_SEG_LEFT_HUNDRED, SLEEP_MODE_DIMMING_PERCENT );
        set_duty_percent( DIMMING__PERCENT_SEG_LEFT_TEN, SLEEP_MODE_DIMMING_PERCENT );
        set_duty_percent( DIMMING__PERCENT_SEG_LEFT_ONE, SLEEP_MODE_DIMMING_PERCENT );
    }
    else
    {
        set_duty_percent( DIMMING__PERCENT_SEG_LEFT_HUNDRED, left_normal_state_percent );
        set_duty_percent( DIMMING__PERCENT_SEG_LEFT_TEN, left_normal_state_percent );
        set_duty_percent( DIMMING__PERCENT_SEG_LEFT_ONE, left_normal_state_percent );
    }

	fnd_left_out( FIXED_DISPLAY, mu8_temporary_hundred, mu8_temporary_ten, mu8_temporary_one);
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void fnd_left_front_version(void)
{
    U8 mu8_temporary_hundred = 0;
    U8 mu8_temporary_ten = 0;
    U8 mu8_temporary_one = 0;
    U8 mu8DataBuffer_1 = 0;
    U8 mu8DataBuffer_2 = 0;

    mu8DataBuffer_1 = gu8_front_version >> 4;

    if( mu8DataBuffer_1 == 0x01 )
    {
        mu8DataBuffer_1 = 1;
    }
    else if( mu8DataBuffer_1 == 0x02 )
    {
        mu8DataBuffer_1 = 2;
    }
    else if( mu8DataBuffer_1 == 0x03 )
    {
        mu8DataBuffer_1 = 10;
    }
    else if( mu8DataBuffer_1 == 0x04 )
    {
        mu8DataBuffer_1 = 20;
    }

    mu8DataBuffer_2 = (U8)(gu8_front_version & 0x0F);


    mu8_temporary_hundred = (U8)((mu8DataBuffer_1 / 10) % 10);
    mu8_temporary_ten = (U8)(mu8DataBuffer_1 % 10);
    mu8_temporary_one = mu8DataBuffer_2;


    fnd_left_out( FIXED_DISPLAY, mu8_temporary_hundred, mu8_temporary_ten, mu8_temporary_one);
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void fnd_left_aging_count(void)
{
    U8 mu8_temporary_hundred = 0;
    U8 mu8_temporary_ten = 0;
    U8 mu8_temporary_one = 0;

    mu8_temporary_hundred = (U8)((gu32_aging_count / 100000) % 10);
    mu8_temporary_ten = (U8)((gu32_aging_count / 10000) % 10);
    mu8_temporary_one = (U8)((gu32_aging_count / 1000) % 10);

    fnd_left_out( FIXED_DISPLAY, mu8_temporary_hundred, mu8_temporary_ten, mu8_temporary_one);
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void fnd_left_display_test(void)
{
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void fnd_left_pba_test(void)
{
    U8 mu8_temporary_hundred = 0;
    U8 mu8_temporary_ten = 0;
    U8 mu8_temporary_one = 0;
    U8 mu8_flick_state = 0;

    mu8_temporary_hundred = DISPLAY_NUM_8;
    mu8_temporary_ten = DISPLAY_NUM_8;
    mu8_temporary_one = DISPLAY_NUM_8;

    fnd_left_out( FIXED_DISPLAY, mu8_temporary_hundred, mu8_temporary_ten, mu8_temporary_one);
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void fnd_left_uart_test(void)
{
    
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  : Memento Here!
***********************************************************************************************************************/
void    memento_error_number_display(U8 U8_p_error_num)
{
    U8 mu8_temporary_hundred = DISPLAY_OFF;
    U8 mu8_temporary_ten = DISPLAY_OFF;
    U8 mu8_temporary_one = (U8_p_error_num % 10);

    set_duty_percent( DIMMING__PERCENT_SEG_LEFT_HUNDRED, left_normal_state_percent );
    set_duty_percent( DIMMING__PERCENT_SEG_LEFT_TEN, left_normal_state_percent );
    set_duty_percent( DIMMING__PERCENT_SEG_LEFT_ONE, left_normal_state_percent );

    fnd_left_out( FIXED_DISPLAY, mu8_temporary_hundred, mu8_temporary_ten, mu8_temporary_one);
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void fnd_left_all_off(void)
{
    U8 mu8_temporary_hundred = 0;
    U8 mu8_temporary_ten = 0;
    U8 mu8_temporary_one = 0;
    U8 mu8_flick_state = 0;

    mu8_temporary_hundred = DISPLAY_OFF;
    mu8_temporary_ten = DISPLAY_OFF;
    mu8_temporary_one = DISPLAY_OFF;

    fnd_left_out( FIXED_DISPLAY, mu8_temporary_hundred, mu8_temporary_ten, mu8_temporary_one);
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void update_cold_temp(void)
{
    U16 mu16_time = 0;

    if( F_FW_Version_Display_Mode == CLEAR )
    {
        gu8_Celcius_temp = 0;
        gu16_cold_temp_update_timer = 0;
        return;
    }
    else{}

    if( gu8_Celcius_temp == 0 )
    {
        gu8_Celcius_temp = gu8_Cold_Temperature_One_Degree;
    }
    else{}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
    mu16_time = 100;        // �ü��µ� ǥ�� ���� �ð� : 10��

    gu16_cold_temp_update_timer++;

    if( gu16_cold_temp_update_timer >= mu16_time )
    {
        gu16_cold_temp_update_timer = 0;

        if( gu8_cooling_display_mode == COOLING_DISPLAY_2_COMPLETE
        && bit_cooling_complete_5degree == SET )
        {
            if( gu8_Celcius_temp > COLD_COMPLETE_DISPLAY_TEMPERATURE )
            {
                gu8_Celcius_temp = (gu8_Celcius_temp - 1);        // �ð� �Ϸ� �� ǥ�ÿµ��� 5������ ������ 10�ʸ��� 1���� ǥ�ø� ����
            }
            else
            {
                if( gu8_Cold_Temperature_One_Degree > gu8_Celcius_temp )    // �ü��µ� �������� ǥ�ð����� ���� �� (�µ��� �ö� ��, 10�ʸ��� 1���� �ø�)
                {
                    gu8_Celcius_temp = (gu8_Celcius_temp + 1);
                    
                    if( gu8_Celcius_temp >= COLD_COMPLETE_DISPLAY_TEMPERATURE )
                    {
                        gu8_Celcius_temp = COLD_COMPLETE_DISPLAY_TEMPERATURE;
                    }
                    else{}
                }
                else if( gu8_Cold_Temperature_One_Degree < gu8_Celcius_temp )   // ǥ�ð��� �ü��µ� ���������� ���� �� (�µ��� �������� ��, 10�ʸ��� 1���� ����)
                {
                    gu8_Celcius_temp = (gu8_Celcius_temp - 1);

                    if( gu8_Celcius_temp <= gu8_Cold_Temperature_One_Degree )
                    {
                        gu8_Celcius_temp = gu8_Cold_Temperature_One_Degree;
                    }
                    else{}
                }
            }
        }
        else
        {
            if( gu8_Cold_Temperature_One_Degree > gu8_Celcius_temp )        // �ü��µ� �������� ǥ�ð����� ���� �� (�µ��� �ö� ��, 10�ʸ��� 1���� �ø�)
            {
                gu8_Celcius_temp = (gu8_Celcius_temp + 1);
                
                if( gu8_Celcius_temp >= gu8_Cold_Temperature_One_Degree )
                {
                    gu8_Celcius_temp = gu8_Cold_Temperature_One_Degree;
                }
                else{}
            }
            else if(gu8_Cold_Temperature_One_Degree < gu8_Celcius_temp )    // ǥ�ð��� �ü��µ� ���������� ���� �� (�µ��� �������� ��, 10�ʸ��� 1���� ����)
            {
                gu8_Celcius_temp = (gu8_Celcius_temp - 1);

                if( gu8_Celcius_temp <= gu8_Cold_Temperature_One_Degree )
                {
                    gu8_Celcius_temp = gu8_Cold_Temperature_One_Degree;
                }
                else{}
            }
            else {}
        }       
    }
    else{}

    /*..hui [25-6-10���� 2:59:35] ���� �ü��µ� 2�� ~ 30�� ����..*/
    if( gu8_Celcius_temp <= 2 )
    {
        gu8_Celcius_temp = 2;
    }
    else if( gu8_Celcius_temp >= 30 )
    {
        gu8_Celcius_temp = 30;
    }
    else{}
}


