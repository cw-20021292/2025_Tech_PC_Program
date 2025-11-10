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
#include    "display_fnd_right.h"

void fnd_right_normal_state(void);
void fnd_right_aging_count(void);
void fnd_right_display_ml(void);
void fnd_right_error(void);
void fnd_right_main_version(void);
void fnd_right_flushing_state(void);
void fnd_right_pba_test(void);
void fnd_right_display_test(void);
void fnd_right_my_recipe(void);
void fnd_right_volume_setting(void);

void fnd_right_onoff_text(U8 onoff);
void fnd_right_onoff_display_timer();

/* Cody Water Line Clean Service */
void cody_service_water_fnd_right(void);

void fnd_right_all_off(void);

U8 gu8_min_setting_blink_timer;
bit bit_min_blink_500ms;

/* �ð�������� ���� Blink ���� */
U8 gu8_volume_setting_blink_timer;
bit bit_volume_blink_500ms;

U8 gu8_fnd_right_on_display_flag;
U8 gu8_fnd_right_off_display_flag;
U16 gu16_fnd_right_onoff_display_timer;

extern void fnd_left_out( U8 mu8_direct, U8 mu8_hundred, U8 mu8_ten, U8 mu8_one  );
extern U16 gu16_cold_off_flick_timer;
extern U16 gu16_ice_off_flick_timer;
extern U8 gu8_indicator_flick_timer;
extern bit F_All_Lock;
extern FLUSHING_STEP gu8_flushing_mode;
extern bit bit_flushing_halt;
extern U8 gu8_display_flushing_total_percent;
extern U8 gu8_cup_led_select;
extern U16 gu16_water_select_return_time;
extern U8 gu8_rtc_time_Min;
extern bit bit_first_time_setting;
extern U8 gu8_clock_hide_state;
extern bit F_WaterOut;
extern bit bit_myWater_setting_start;
extern TYPE_BYTE          U8IceOutStateB;
extern bit F_IceOut;
extern U16 gu16_WaterQuantity_half;
extern U16 gu16_WaterQuantity_one;
extern U16 gu16_WaterQuantity_two;
extern U16 gu16_WaterQuantity_four;
extern U8 gu8_Icewater_level;

#define            u8IceOutState                             U8IceOutStateB.byte
#define            Bit0_Ice_Only_Select_State                U8IceOutStateB.Bit.b0
#define            Bit1_Ice_Plus_Water_Select_State          U8IceOutStateB.Bit.b1


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void fnd_right_normal_state(void)
{
    U8 mu8_temporary_hundred = 0;
    U8 mu8_temporary_ten = 0;
    U8 mu8_temporary_one = 0;

    if(bit_display_sleep_start == SET)
    {
        mu8_temporary_hundred = DISPLAY_OFF;
        mu8_temporary_ten = DISPLAY_OFF;
        mu8_temporary_one = DISPLAY_OFF;
        fnd_right_out( FIXED_DISPLAY, mu8_temporary_hundred, mu8_temporary_ten, mu8_temporary_one);
        return;
    }

    if(F_All_Lock == SET)
    {
        return;
    }

    if(gu8_fnd_right_on_display_flag == SET)
    {
        fnd_right_onoff_text(ON);
        fnd_right_onoff_display_timer();
        return;
    }
    else if(gu8_fnd_right_off_display_flag == SET)
    {
        fnd_right_onoff_text(OFF);
        fnd_right_onoff_display_timer();
        return;
    }

    if(bit_setting_mode_start == SET)
    {
        fnd_right_all_off();
        return;
    }

    if(bit_myWater_setting_start == SET)
    {
        fnd_right_my_recipe();
        return;
    }

    if( gu8_durable_test_start == SET )
    {
        fnd_right_aging_count();
        return;
    }

    if( (gu8_cup_led_select == CLEAR )
    && ( gu16_water_select_return_time == 0)
    && (bit_myWater_setting_start == 0)
    && (gu8_clock_hide_state == CLEAR)
    && (F_WaterOut == CLEAR)
    && (F_IceOut == CLEAR)
    )
    {
        if((GetWifiStatusValue(WIFI_STATUS_SERVER_CON) != CLEAR ) && (GetWifiStatusValue(WIFI_STATUS_PAIRING) == CLEAR ))
        {
            mu8_temporary_hundred = (gu8_rtc_time_Min / 10);
            mu8_temporary_ten = (gu8_rtc_time_Min % 10);
            mu8_temporary_one = DISPLAY_OFF;
            fnd_right_out( FIXED_DISPLAY, mu8_temporary_hundred, mu8_temporary_ten, mu8_temporary_one);
        }
        else
        {
            if(bit_first_time_setting == SET )
            {
                mu8_temporary_hundred = (gu8_rtc_time_Min / 10);
                mu8_temporary_ten = (gu8_rtc_time_Min % 10);
                mu8_temporary_one = DISPLAY_OFF;
                fnd_right_out( FIXED_DISPLAY, mu8_temporary_hundred, mu8_temporary_ten, mu8_temporary_one);
            }
            else
            {
                fnd_right_display_ml();
            }
        }
    }
    else
    {
        fnd_right_display_ml();
    }

    if( gu8_Led_Display_Step == LED_Display__SLEEP )
    {
        set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_HUNDRED, SLEEP_MODE_DIMMING_PERCENT );
        set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_TEN, SLEEP_MODE_DIMMING_PERCENT );
        set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_ONE, SLEEP_MODE_DIMMING_PERCENT );
    }
    else
    {
        set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_HUNDRED, right_normal_state_percent );
        set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_TEN, right_normal_state_percent );
        set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_ONE, right_normal_state_percent );
    }
}

void fnd_right_flushing_state(void)
{
	U8 mu8_temporary_hundred = 0;
	U8 mu8_temporary_ten = 0;
	U8 mu8_temporary_one = 0;

	//Bit4_Front_Led_Temp_Fnd_Point = CLEAR;

	if( gu8_flushing_mode == FLUSHING_STANDBY_STATE || bit_flushing_halt == SET )
	{
        /*..hui [23-6-14���� 4:30:12] �÷��� �ȵǴ� ���� �߻��� ������ȣ ǥ��..*/
        if( Bit3_Leakage_Sensor_Error__E01 == SET )
        {
            /*mu8_temporary_hundred = DISPLAY_OFF;*/
            /*..hui [23-11-24���� 2:30:48] ���� ������ �����鼭 Exx�� ǥ��..*/
            mu8_temporary_hundred = DISPLAY_NUM_LARGE_E;
            mu8_temporary_ten = DISPLAY_NUM_0;
            mu8_temporary_one = DISPLAY_NUM_1;
        }
        else if( Bit6_Main_Water_Flow_Block_Error__E09 == SET )
        {
            /*mu8_temporary_hundred = DISPLAY_OFF;*/
            /*..hui [23-11-24���� 2:30:48] ���� ������ �����鼭 Exx�� ǥ��..*/
            mu8_temporary_hundred = DISPLAY_NUM_LARGE_E;
            mu8_temporary_ten = DISPLAY_NUM_0;
            mu8_temporary_one = DISPLAY_NUM_9;
        }
        else if( Bit16_Drain_Pump_Error__E60 == SET )
        {
            /*..hui [23-8-18���� 3:15:33] ��ũ �÷��� �߰�������.. �巹������ ������ ��� ����ȵ�..*/
            /*mu8_temporary_hundred = DISPLAY_OFF;*/
            /*..hui [23-11-24���� 2:30:48] ���� ������ �����鼭 Exx�� ǥ��..*/
            mu8_temporary_hundred = DISPLAY_NUM_LARGE_E;
            mu8_temporary_ten = DISPLAY_NUM_6;
            mu8_temporary_one = DISPLAY_NUM_0;
        }
        else
        {
            mu8_temporary_hundred = DISPLAY_OFF;
            mu8_temporary_ten = DISPLAY_OFF;
            mu8_temporary_one = DISPLAY_OFF;
        }
	}
	else
	{
        if( gu8_display_flushing_total_percent < 100 )
        {
            if( gu8_display_flushing_total_percent == 0 )
            {
                mu8_temporary_hundred = DISPLAY_OFF;
                mu8_temporary_ten = DISPLAY_OFF;
                mu8_temporary_one = DISPLAY_NUM_0;
            }
            else
            {
                mu8_temporary_hundred = DISPLAY_OFF;
                mu8_temporary_ten = (gu8_display_flushing_total_percent / 10);
                mu8_temporary_one = (gu8_display_flushing_total_percent % 10);
            }
        }
        else
        {
            mu8_temporary_hundred = DISPLAY_NUM_1;
            mu8_temporary_ten = DISPLAY_NUM_0;
            mu8_temporary_one = DISPLAY_NUM_0;
        }
	}

    if( gu8_Led_Display_Step == LED_Display__SLEEP )
    {
        set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_HUNDRED, SLEEP_MODE_DIMMING_PERCENT );
        set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_TEN, SLEEP_MODE_DIMMING_PERCENT );
        set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_ONE, SLEEP_MODE_DIMMING_PERCENT );
    }
    else
    {
        set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_HUNDRED, right_normal_state_percent );
        set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_TEN, right_normal_state_percent );
        set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_ONE, right_normal_state_percent );
    }

	fnd_right_out( FIXED_DISPLAY, mu8_temporary_hundred, mu8_temporary_ten, mu8_temporary_one);

}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void fnd_right_my_recipe(void)
{
	U8 mu8_temporary_hundred = 0;
	U8 mu8_temporary_ten = 0;
	U8 mu8_temporary_one = 0;

	if(my_recipe_select == RECIPE_RAMEN)
	{
		mu8_temporary_hundred = DISPLAY_NUM_5;
		mu8_temporary_ten = DISPLAY_NUM_5;
		mu8_temporary_one = DISPLAY_NUM_0;
	}
	else if(my_recipe_select == RECIPE_DRIPCOFFEE)
	{
		mu8_temporary_hundred = DISPLAY_NUM_1;
		mu8_temporary_ten = DISPLAY_NUM_6;
		mu8_temporary_one = DISPLAY_NUM_0;
	}
	else if(my_recipe_select == RECIPE_TEA)
	{
		mu8_temporary_hundred = DISPLAY_OFF;
		mu8_temporary_ten = DISPLAY_NUM_9;
		mu8_temporary_one = DISPLAY_NUM_0;
	}
	else if(my_recipe_select == MY_INDEX_MY1)
	{
        if(my_setting[MY_INDEX_MY1].amount >= 1000)             /* 1L �̻� */
        {
            mu8_temporary_hundred = (U8)(my_setting[MY_INDEX_MY1].amount / 1000);
            mu8_temporary_ten = (U8)((my_setting[MY_INDEX_MY1].amount % 1000) / 100);
            mu8_temporary_one = (U8)((my_setting[MY_INDEX_MY1].amount % 100) / 10);
        }
        else
        {
            if( my_setting[MY_INDEX_MY1].amount < 10 )
            {
                mu8_temporary_hundred = DISPLAY_OFF;
                mu8_temporary_ten = DISPLAY_OFF;
                mu8_temporary_one = (U8)my_setting[MY_INDEX_MY1].amount;
            }
            else if( my_setting[MY_INDEX_MY1].amount < 100 )
            {
                mu8_temporary_hundred = DISPLAY_OFF;
                mu8_temporary_ten = (U8)(my_setting[MY_INDEX_MY1].amount / 10);
                mu8_temporary_one = (U8)(my_setting[MY_INDEX_MY1].amount % 10);
            }
            else
            {
                mu8_temporary_hundred = (U8)((my_setting[MY_INDEX_MY1].amount / 100) % 10);
                mu8_temporary_ten = (U8)((my_setting[MY_INDEX_MY1].amount / 10) % 10);
                mu8_temporary_one = (U8)(my_setting[MY_INDEX_MY1].amount % 10);
            }
        }
	}
	else if(my_recipe_select == MY_INDEX_MY2)
	{
		if(my_setting[MY_INDEX_MY2].amount >= 1000)             /* 1L �̻� */
        {
            mu8_temporary_hundred = (U8)(my_setting[MY_INDEX_MY2].amount / 1000);
            mu8_temporary_ten = (U8)((my_setting[MY_INDEX_MY2].amount % 1000) / 100);
            mu8_temporary_one = (U8)((my_setting[MY_INDEX_MY2].amount % 100) / 10);
        }
        else
        {
            if( my_setting[MY_INDEX_MY2].amount < 10 )
            {
                mu8_temporary_hundred = DISPLAY_OFF;
                mu8_temporary_ten = DISPLAY_OFF;
                mu8_temporary_one = (U8)my_setting[MY_INDEX_MY2].amount;
            }
            else if( my_setting[MY_INDEX_MY2].amount < 100 )
            {
                mu8_temporary_hundred = DISPLAY_OFF;
                mu8_temporary_ten = (U8)(my_setting[MY_INDEX_MY2].amount / 10);
                mu8_temporary_one = (U8)(my_setting[MY_INDEX_MY2].amount % 10);
            }
            else
            {
                mu8_temporary_hundred = (U8)((my_setting[MY_INDEX_MY2].amount / 100) % 10);
                mu8_temporary_ten = (U8)((my_setting[MY_INDEX_MY2].amount / 10) % 10);
                mu8_temporary_one = (U8)(my_setting[MY_INDEX_MY2].amount % 10);
            }
        }
	}
    else if(my_recipe_select == MY_INDEX_MY3)
	{
        if(my_setting[MY_INDEX_MY3].amount >= 1000)             /* 1L �̻� */
        {
            mu8_temporary_hundred = (U8)(my_setting[MY_INDEX_MY3].amount / 1000);
            mu8_temporary_ten = (U8)((my_setting[MY_INDEX_MY3].amount % 1000) / 100);
            mu8_temporary_one = (U8)((my_setting[MY_INDEX_MY3].amount % 100) / 10);
        }
        else
        {
            if( my_setting[MY_INDEX_MY3].amount < 10 )
            {
                mu8_temporary_hundred = DISPLAY_OFF;
                mu8_temporary_ten = DISPLAY_OFF;
                mu8_temporary_one = (U8)my_setting[MY_INDEX_MY3].amount;
            }
            else if( my_setting[MY_INDEX_MY3].amount < 100 )
            {
                mu8_temporary_hundred = DISPLAY_OFF;
                mu8_temporary_ten = (U8)(my_setting[MY_INDEX_MY3].amount / 10);
                mu8_temporary_one = (U8)(my_setting[MY_INDEX_MY3].amount % 10);
            }
            else
            {
                mu8_temporary_hundred = (U8)((my_setting[MY_INDEX_MY3].amount / 100) % 10);
                mu8_temporary_ten = (U8)((my_setting[MY_INDEX_MY3].amount / 10) % 10);
                mu8_temporary_one = (U8)(my_setting[MY_INDEX_MY3].amount % 10);
            }
        }
	}

    if( gu8_Led_Display_Step == LED_Display__SLEEP )
    {
        set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_HUNDRED, SLEEP_MODE_DIMMING_PERCENT );
        set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_TEN, SLEEP_MODE_DIMMING_PERCENT );
        set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_ONE, SLEEP_MODE_DIMMING_PERCENT );
    }
    else
    {
        set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_HUNDRED, right_normal_state_percent );
        set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_TEN, right_normal_state_percent );
        set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_ONE, right_normal_state_percent );
    }

	fnd_right_out( FIXED_DISPLAY, mu8_temporary_hundred, mu8_temporary_ten, mu8_temporary_one);
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void fnd_right_aging_count(void)
{
    // U8 mu8_temporary_hundred = 0;
    // U8 mu8_temporary_ten = 0;
    // U8 mu8_temporary_one = 0;

    // if( gu32_aging_count < 10000 )
    // {
    //     mu8_temporary_hundred = (U8)((gu32_aging_count / 1000) % 10);
    //     mu8_temporary_ten = (U8)((gu32_aging_count / 100) % 10);
    //     mu8_temporary_one = (U8)((gu32_aging_count / 10) % 10);
    // }
    // else
    // {
    //     if( F_Wink_500ms == SET )
    //     {
    //         mu8_temporary_hundred = (U8)((gu32_aging_count / 1000) % 10);
    //     }
    //     else
    //     {
    //         mu8_temporary_hundred = DISPLAY_OFF;
    //     }

    //     mu8_temporary_ten = (U8)((gu32_aging_count / 100) % 10);
    //     mu8_temporary_one = (U8)((gu32_aging_count / 10) % 10);
    // }

    // fnd_right_out( FIXED_DISPLAY, mu8_temporary_hundred, mu8_temporary_ten, mu8_temporary_one);
    U8 mu8_temporary_hundred = 0;
    U8 mu8_temporary_ten = 0;
    U8 mu8_temporary_one = 0;

    mu8_temporary_hundred = (U8)((gu32_aging_count / 100) % 10);
    mu8_temporary_ten = (U8)((gu32_aging_count / 10) % 10);
    mu8_temporary_one = (U8)((gu32_aging_count / 1) % 10);

    fnd_right_out( FIXED_DISPLAY, mu8_temporary_hundred, mu8_temporary_ten, mu8_temporary_one);

}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void fnd_right_display_ml(void)
{
    U8 mu8_temporary_hundred = 0;
    U8 mu8_temporary_ten = 0;
    U8 mu8_temporary_one = 0;

    if(bit_time_setting_start == SET)
    {

    }
    else if((Bit2_Cold_Off_Indicator != 0)
    || (Bit0_Ice_Lock_Indicator != 0)
    || (Bit6_Icewater_Off_Indicator != 0)
    || (Bit5_Ice_Off_Indicator != 0)
    )
    {
        mu8_temporary_hundred = DISPLAY_NUM_LARGE_O;
        mu8_temporary_ten = DISPLAY_NUM_LARGE_F;
        mu8_temporary_one = DISPLAY_NUM_LARGE_F;
    }
    else
    {
        #ifdef __AC_MOTOR_TEST_MODE__
        if( u8IceOutState == ICE_SELECT__ICE )
        {
            mu8_temporary_hundred = DISPLAY_OFF;
            mu8_temporary_ten = DISPLAY_OFF;
            mu8_temporary_one = DISPLAY_OFF;
        }
        else
        {
            // Endurance Here!
            mu8_temporary_hundred = (U8)((gu32_feeder_screw_ccw_count % 1000) / 100);
            mu8_temporary_ten = (U8)((gu32_feeder_screw_ccw_count % 100) / 10);
            mu8_temporary_one = (U8)(gu32_feeder_screw_ccw_count % 10);
        }
        #else
        /* ���� ���� �� �뷮ǥ�� ���ߵ� */
        if( u8IceOutState == ICE_SELECT__ICE )
        {
            mu8_temporary_hundred = DISPLAY_OFF;
            mu8_temporary_ten = DISPLAY_OFF;
            mu8_temporary_one = DISPLAY_OFF;
        }
        else if(u8IceOutState == ICE_SELECT__ICE_WATER)
        {
            if(gu8_Icewater_level == ICEWATER_CUP_LEVEL_1_150ML)
            {
                mu8_temporary_hundred = DISPLAY_NUM_1;
                mu8_temporary_ten = DISPLAY_NUM_5;
                mu8_temporary_one = DISPLAY_NUM_0;
            }
            else if(gu8_Icewater_level == ICEWATER_CUP_LEVEL_2_250ML)
            {
                mu8_temporary_hundred = DISPLAY_NUM_2;
                mu8_temporary_ten = DISPLAY_NUM_5;
                mu8_temporary_one = DISPLAY_NUM_0;
            }
            else if(gu8_Icewater_level == ICEWATER_CUP_LEVEL_3_350ML)
            {
                mu8_temporary_hundred = DISPLAY_NUM_3;
                mu8_temporary_ten = DISPLAY_NUM_5;
                mu8_temporary_one = DISPLAY_NUM_0;
            }
            else if(gu8_Icewater_level == ICEWATER_CUP_LEVEL_4_500ML)
            {
                mu8_temporary_hundred = DISPLAY_NUM_5;
                mu8_temporary_ten = DISPLAY_NUM_0;
                mu8_temporary_one = DISPLAY_NUM_0;
            }
            else
            {
                mu8_temporary_hundred = DISPLAY_NUM_1;
                mu8_temporary_ten = DISPLAY_NUM_5;
                mu8_temporary_one = DISPLAY_NUM_0;
            }
        }
        else
        {
            if( gu8Cup_level == CUP_LEVEL_1_120ML )
            {
                if(gu16_WaterQuantity_half >= 1000)
                {
                    mu8_temporary_hundred = (U8)(gu16_WaterQuantity_half / 1000);
                    mu8_temporary_ten = (U8)((gu16_WaterQuantity_half / 100) % 10);
                    mu8_temporary_one = (U8)((gu16_WaterQuantity_half / 10)  % 10);
                }
                else
                {
                    mu8_temporary_hundred = (U8)(gu16_WaterQuantity_half / 100);
                    mu8_temporary_ten = (U8)((gu16_WaterQuantity_half / 10) % 10);
                    mu8_temporary_one = DISPLAY_NUM_0;
                }
            }
            else if(gu8Cup_level == CUP_LEVEL_2_250ML)
            {
                if(gu16_WaterQuantity_one >= 1000)
                {
                    mu8_temporary_hundred = (U8)(gu16_WaterQuantity_one / 1000);
                    mu8_temporary_ten = (U8)((gu16_WaterQuantity_one / 100) % 10);
                    mu8_temporary_one = (U8)((gu16_WaterQuantity_one / 10)  % 10);
                }
                else
                {
                    mu8_temporary_hundred = (U8)(gu16_WaterQuantity_one / 100);
                    mu8_temporary_ten = (U8)((gu16_WaterQuantity_one / 10) % 10);
                    mu8_temporary_one = DISPLAY_NUM_0;
                }
            }
            else if(gu8Cup_level == CUP_LEVEL_3_500ML)
            {
                if(gu16_WaterQuantity_two >= 1000)
                {
                    mu8_temporary_hundred = (U8)(gu16_WaterQuantity_two / 1000);
                    mu8_temporary_ten = (U8)((gu16_WaterQuantity_two / 100) % 10);
                    mu8_temporary_one = (U8)((gu16_WaterQuantity_two / 10)  % 10);
                }
                else
                {
                    mu8_temporary_hundred = (U8)(gu16_WaterQuantity_two / 100);
                    mu8_temporary_ten = (U8)((gu16_WaterQuantity_two / 10) % 10);
                    mu8_temporary_one = DISPLAY_NUM_0;
                }
            }
                else if(gu8Cup_level == CUP_LEVEL_4_1000ML)
                {
                    if(gu16_WaterQuantity_four >= 1000)
                    {
                        mu8_temporary_hundred = (U8)(gu16_WaterQuantity_four / 1000);
                        mu8_temporary_ten = (U8)((gu16_WaterQuantity_four / 100) % 10);
                        mu8_temporary_one = (U8)((gu16_WaterQuantity_four / 10)  % 10);
                    }
                    else
                    {
                        mu8_temporary_hundred = (U8)(gu16_WaterQuantity_four / 100);
                        mu8_temporary_ten = (U8)((gu16_WaterQuantity_four / 10) % 10);
                        mu8_temporary_one = DISPLAY_NUM_0;
                    }
                }
            else /*if(gu8Cup_level == CUP_LEVEL_CONTINUE)*/
            {
                mu8_temporary_hundred = DISPLAY_NUM_CONTINUE_100;
                mu8_temporary_ten = DISPLAY_NUM_CONTINUE_010;
                mu8_temporary_one = DISPLAY_NUM_CONTINUE_001;
            }
        }
        #endif
    }

    fnd_right_out( FIXED_DISPLAY, mu8_temporary_hundred, mu8_temporary_ten, mu8_temporary_one);
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void fnd_right_error(void)
{
    U8 mu8_temporary_hundred = 0;
    U8 mu8_temporary_ten = 0;
    U8 mu8_temporary_one = 0;

    mu8_temporary_hundred = DISPLAY_OFF;
    mu8_temporary_ten = DISPLAY_OFF;
    mu8_temporary_one = DISPLAY_OFF;

    fnd_right_out( FIXED_DISPLAY, mu8_temporary_hundred, mu8_temporary_ten, mu8_temporary_one);
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void fnd_right_time_setting(void)
{
    U8 mu8_temporary_hundred = 0;
    U8 mu8_temporary_ten = 0;
    U8 mu8_temporary_one = 0;

    if( gu8_time_setting_step == SETTING_TIME_MIN )
    {
        gu8_min_setting_blink_timer++;

        if( gu8_min_setting_blink_timer >= 5 )
        {
            if( u8TimeSettingLongKey > 0 )
            {
                gu8_min_setting_blink_timer = 0;
                bit_min_blink_500ms = CLEAR;
            }
            else
            {
                gu8_min_setting_blink_timer = 0;
                bit_min_blink_500ms ^= SET;
            }
        }
        else{}

        if( bit_min_blink_500ms == CLEAR )
        {
            mu8_temporary_hundred = gu8_temporary_Min/10;
            mu8_temporary_ten = gu8_temporary_Min%10;
            mu8_temporary_one = DISPLAY_OFF;
        }
        else
        {
            mu8_temporary_hundred = DISPLAY_OFF;
            mu8_temporary_ten = DISPLAY_OFF;
            mu8_temporary_one = DISPLAY_OFF;
        }
    }
    else
    {
        mu8_temporary_hundred = gu8_temporary_Min/10;
        mu8_temporary_ten = gu8_temporary_Min%10;
        mu8_temporary_one = DISPLAY_OFF;
    }

    if( gu8_Led_Display_Step == LED_Display__SLEEP )
    {
        set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_HUNDRED, SLEEP_MODE_DIMMING_PERCENT );
        set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_TEN, SLEEP_MODE_DIMMING_PERCENT );
        set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_ONE, SLEEP_MODE_DIMMING_PERCENT );
    }
    else
    {
        set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_HUNDRED, right_normal_state_percent );
        set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_TEN, right_normal_state_percent );
        set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_ONE, right_normal_state_percent );
    }


	fnd_right_out( FIXED_DISPLAY, mu8_temporary_hundred, mu8_temporary_ten, mu8_temporary_one);

}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void fnd_right_main_version(void)
{
    U8 mu8_temporary_hundred = 0;
    U8 mu8_temporary_ten = 0;
    U8 mu8_temporary_one = 0;

    mu8_temporary_hundred = (U8)((Main_FW_Version_Main / 10) % 10);
    mu8_temporary_ten = (U8)(Main_FW_Version_Main % 10);
    mu8_temporary_one = (U8)Main_FW_Version_Sub;

    fnd_right_out( FIXED_DISPLAY, mu8_temporary_hundred, mu8_temporary_ten, mu8_temporary_one);
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void fnd_right_onoff_text(U8 onoff)
{
	U8 mu8_temporary_hundred = 0;
	U8 mu8_temporary_ten = 0;
	U8 mu8_temporary_one = 0;

	if(onoff == OFF)
	{
		mu8_temporary_hundred = DISPLAY_NUM_LARGE_O;
		mu8_temporary_ten = DISPLAY_NUM_LARGE_F;
		mu8_temporary_one = DISPLAY_NUM_LARGE_F;
	}
	else if(onoff == ON)
	{
		mu8_temporary_hundred = DISPLAY_NUM_LARGE_O;
		mu8_temporary_ten = DISPLAY_NUM_SMALL_n;
		mu8_temporary_one = DISPLAY_OFF;
	}

    set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_HUNDRED, right_normal_state_percent );
    set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_TEN, right_normal_state_percent );
    set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_ONE, right_normal_state_percent );

	fnd_right_out( FIXED_DISPLAY, mu8_temporary_hundred, mu8_temporary_ten, mu8_temporary_one);

}
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/



/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void fnd_right_pba_test(void)
{
    U8 mu8_temporary_hundred = 0;
    U8 mu8_temporary_ten = 0;
    U8 mu8_temporary_one = 0;
    U8 mu8_flick_state = 0;

    mu8_temporary_hundred = DISPLAY_NUM_8;
    mu8_temporary_ten = DISPLAY_NUM_8;
    mu8_temporary_one = DISPLAY_NUM_8;

    fnd_right_out( FIXED_DISPLAY, mu8_temporary_hundred, mu8_temporary_ten, mu8_temporary_one);

}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void fnd_right_display_test(void)
{
}

/***********************************************************************************************************************/
/**
 * @brief �Ҹ����� �� ���� fnd ǥ��
 */
void fnd_right_volume_setting(void)
{
    U8 mu8_temporary_hundred = 0;
    U8 mu8_temporary_ten = 0;
    U8 mu8_temporary_one = 0;

    gu8_volume_setting_blink_timer++;
    if(gu8_volume_setting_blink_timer >= 5)
    {
        gu8_volume_setting_blink_timer = 0;
        bit_volume_blink_500ms ^= SET;
    }
    else {  }

    /* ��� �������� ���� (�ӽ�) */
    if(bit_volume_blink_500ms == CLEAR)
    {
        mu8_temporary_hundred = DISPLAY_BAR;

        if( gu8Sound_Type == SOUND_TYPE_VOICE )
        {
            mu8_temporary_ten = 3;
        }
        else if( gu8Sound_Type == SOUND_TYPE_MELODY )
        {
            mu8_temporary_ten = 2;
        }
        else
        {
            mu8_temporary_ten = 1;
        }

        mu8_temporary_one = DISPLAY_BAR;
    }
    else
    {
        mu8_temporary_hundred = DISPLAY_BAR;
        mu8_temporary_ten = DISPLAY_OFF;
        mu8_temporary_one = DISPLAY_BAR;
    }

    set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_HUNDRED, right_normal_state_percent );
    set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_TEN, right_normal_state_percent );
    set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_ONE, right_normal_state_percent );

    fnd_right_out( FIXED_DISPLAY, mu8_temporary_hundred, mu8_temporary_ten, mu8_temporary_one);
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  : Memento Here!
***********************************************************************************************************************/
void    memento_error_code_display(U8 U8_p_ErrorCode)
{
    U8 mu8_temporary_hundred = 0;
    U8 mu8_temporary_ten = 0;
    U8 mu8_temporary_one = 0;

    /* COMP ���� */
    if(U8_p_ErrorCode >= 101 && U8_p_ErrorCode <= 108)
    {
        mu8_temporary_hundred = DISPLAY_NUM_LARGE_E;
        mu8_temporary_ten = DISPLAY_NUM_LARGE_C;
        mu8_temporary_one = (U8_p_ErrorCode % 10);
    }
    else
    {
        mu8_temporary_hundred = DISPLAY_NUM_LARGE_E;
        mu8_temporary_ten = ((U8_p_ErrorCode / 10) % 10);
        mu8_temporary_one = (U8_p_ErrorCode % 10);
    }

    if(U8_p_ErrorCode == 0)
    {
        mu8_temporary_hundred = DISPLAY_BAR;
        mu8_temporary_ten = DISPLAY_BAR;
        mu8_temporary_one = DISPLAY_BAR;
    }
    else {  }

    set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_HUNDRED, right_normal_state_percent );
    set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_TEN, right_normal_state_percent );
    set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_ONE, right_normal_state_percent );

    fnd_right_out( FIXED_DISPLAY, mu8_temporary_hundred, mu8_temporary_ten, mu8_temporary_one);
}

/* Cody Water Line Clean Service */
void cody_service_water_fnd_right(void)
{
    U8 mu8_temporary_hundred = DISPLAY_NUM_5;
    U8 mu8_temporary_ten = DISPLAY_NUM_SMALL_t;
    U8 mu8_temporary_one = DISPLAY_NUM_LARGE_E;

    if(cody_water_line.gu8_start == SET)
	{

    }
    else {  }

    set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_HUNDRED, right_normal_state_percent );
    set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_TEN, right_normal_state_percent );
    set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_ONE, right_normal_state_percent );

    fnd_right_out( FIXED_DISPLAY, mu8_temporary_hundred, mu8_temporary_ten, mu8_temporary_one);
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void fnd_right_self_test_state(void)
{
    U8 mu8_temporary_hundred = 0;
    U8 mu8_temporary_ten = 0;
    U8 mu8_temporary_one = 0;

    if( gu8_wifi_self_test_progress < 100 )
    {
        if( gu8_wifi_self_test_progress == 0 )
        {
            mu8_temporary_hundred = DISPLAY_OFF;
            mu8_temporary_ten = DISPLAY_OFF;
            mu8_temporary_one = DISPLAY_NUM_0;
        }
        else
        {
            mu8_temporary_hundred = DISPLAY_OFF;
            mu8_temporary_ten = (gu8_wifi_self_test_progress / 10);
            mu8_temporary_one = (gu8_wifi_self_test_progress % 10);
        }
    }
    else
    {
        mu8_temporary_hundred = DISPLAY_NUM_1;
        mu8_temporary_ten = DISPLAY_NUM_0;
        mu8_temporary_one = DISPLAY_NUM_0;
    }

    set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_HUNDRED, right_normal_state_percent );
    set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_TEN, right_normal_state_percent );
    set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_ONE, right_normal_state_percent );

    fnd_right_out( FIXED_DISPLAY, mu8_temporary_hundred, mu8_temporary_ten, mu8_temporary_one);
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void fnd_right_button_set(void)
{
    U8 mu8_temporary_thousand = 0;
    U8 mu8_temporary_hundred = 0;
    U8 mu8_temporary_ten = 0;
    U8 mu8_temporary_one = 0;
    U8 mu8_flick_state = 0;

    switch( u8ButtonSetDisplay )
    {
        case BUTTON_SET_DISPLAY_OFF:

            mu8_temporary_hundred = DISPLAY_OFF;
            mu8_temporary_ten = DISPLAY_OFF;
            mu8_temporary_one = DISPLAY_OFF;

        break;

        case BUTTON_SET_COLD_DISPLAY:

            mu8_flick_state = button_set_display_timer( BUTTON_SET_DISPLAY_TIME );

            if( mu8_flick_state == SET )
            {
                if( F_Cold_Enable == SET )
                {
                    mu8_temporary_hundred = DISPLAY_NUM_LARGE_O;
                    mu8_temporary_ten = DISPLAY_NUM_SMALL_n;
                    mu8_temporary_one = DISPLAY_OFF;
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
                mu8_temporary_hundred = DISPLAY_OFF;
                mu8_temporary_ten = DISPLAY_OFF;
                mu8_temporary_one = DISPLAY_OFF;
            }

        break;

        case BUTTON_SET_MUTE_DISPLAY:

            mu8_flick_state = button_set_display_timer( BUTTON_SET_DISPLAY_TIME );

            if( mu8_flick_state == SET )
            {
                /*if( F_Mute_Enable == SET )*/

                /*..hui [23-7-26���� 7:15:45] �Ҹ��� ���°� ON��.. UI Ȯ�ι޾���.. �����Ѱ� �¾ҳ�..*/
                if( F_Mute_Enable == CLEAR )
                {
                    /*..hui [23-5-12���� 2:01:32] �Ҹ� OFF�̹Ƿ� ONǥ��..*/
                    mu8_temporary_hundred = DISPLAY_NUM_LARGE_O;
                    mu8_temporary_ten = DISPLAY_NUM_SMALL_n;
                    mu8_temporary_one = DISPLAY_OFF;
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
                mu8_temporary_hundred = DISPLAY_OFF;
                mu8_temporary_ten = DISPLAY_OFF;
                mu8_temporary_one = DISPLAY_OFF;
            }


        break;

        case BUTTON_SET_ICE_OFF_DISPLAY:

            mu8_flick_state = button_set_display_timer( BUTTON_SET_DISPLAY_TIME );

            if( mu8_flick_state == SET )
            {
                if( F_IceOn == SET )
                {
                    mu8_temporary_hundred = DISPLAY_NUM_LARGE_O;
                    mu8_temporary_ten = DISPLAY_NUM_SMALL_n;
                    mu8_temporary_one = DISPLAY_OFF;
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
                mu8_temporary_hundred = DISPLAY_OFF;
                mu8_temporary_ten = DISPLAY_OFF;
                mu8_temporary_one = DISPLAY_OFF;
            }

        break;

        case BUTTON_SET_RECOVER_FAST_ICE_DISPLAY:

            mu8_flick_state = button_set_display_timer( BUTTON_SET_DISPLAY_TIME );

            if( mu8_flick_state == SET )
            {
                /*..hui [24-4-11���� 7:27:17] �����켱 ���� �������..*/
                if( gu8_recover_org_fast_ice == RECOVER_FAST_ICE )
                {
                    mu8_temporary_hundred = DISPLAY_NUM_LARGE_O;
                    mu8_temporary_ten = DISPLAY_NUM_SMALL_n;
                    mu8_temporary_one = DISPLAY_OFF;
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
                mu8_temporary_hundred = DISPLAY_OFF;
                mu8_temporary_ten = DISPLAY_OFF;
                mu8_temporary_one = DISPLAY_OFF;
            }

        break;

        default:

            u8ButtonSetDisplay = BUTTON_SET_DISPLAY_OFF;
            gu16_button_set_display_timer = 0;

        break;
    }

    // 2025-09-04 CH.PARK [V1.0.0.5] �Ҹ����� �� ���� fnd ��� ���� ���� ����
    set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_HUNDRED, right_normal_state_percent );
    set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_TEN, right_normal_state_percent );
    set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_ONE, right_normal_state_percent );

    fnd_right_out( FIXED_DISPLAY, mu8_temporary_hundred, mu8_temporary_ten, mu8_temporary_one);
}

void fnd_right_onoff_display_timer()
{
	gu16_fnd_right_onoff_display_timer ++;
	if(gu16_fnd_right_onoff_display_timer > 30)
	{
		gu16_fnd_right_onoff_display_timer = 0;
		gu8_fnd_right_on_display_flag = CLEAR;
		gu8_fnd_right_off_display_flag = CLEAR;
	}
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void fnd_right_all_off(void)
{
    U8 mu8_temporary_hundred = 0;
    U8 mu8_temporary_ten = 0;
    U8 mu8_temporary_one = 0;

    mu8_temporary_hundred = DISPLAY_OFF;
    mu8_temporary_ten = DISPLAY_OFF;
    mu8_temporary_one = DISPLAY_OFF;

    fnd_right_out( FIXED_DISPLAY, mu8_temporary_hundred, mu8_temporary_ten, mu8_temporary_one);
}
