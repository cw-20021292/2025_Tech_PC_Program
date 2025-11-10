/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#include    "Macrodriver.h"
#include    "Global_Variable.h"
#include    "Port_Define.h"
#include    "Key_Handling_Long.h"
#include "WIFI_SetFunctionData.h"
#include "WIFI_A101x_FunctionList.h"

void calcel_comp_delay(void);
void ice_on_off_setting_job( U8 mu8_mode );
void cold_water_enable_setting_job( U8 mu8_mode );

void setting_mode_start(void);
void key_time_setting_job(void);
void key_time_setting_ice_water_long_select_job(void);
void key_time_setting_ice_long_select_job(void);
void key_setting_all_lock_off(void);
void key_manual_ice_tank_ster(void);

/* Cody Water Line Clean Service */
void key_cody_water_line_clean_job(void);
void stop_flushing_test_mode_start(void);

bit bit_setting_mode_start;

U16 gu16_Time_Setting_Mode_Timer_100ms;

bit bit_altitude_setting_start;
bit bit_memento_start;
U8 gu8_memento_display_num;
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void calcel_comp_delay(void)
{
    /*..hui [19-10-25오전 9:21:25] 여기서 한번 더 필터링..*/
    if(F_Comp_Output == CLEAR && gu16CompOffDelay > 0)
    {
        /*..hui [19-10-25오전 9:22:56] 냉매전환밸브 초기화시간 고려..*/
        gu16CompOffDelay = DEVELOPE_COMP_DELAY_CANCEL_TIME;
        F_IceInit = CLEAR;
        play_melody_setting_on_198();
    }
    else
    {
        play_melody_warning_197();
    }
}



/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void ice_on_off_setting_job( U8 mu8_mode )
{
    if( F_All_Lock == SET )
    {
        return;
    }
    else{}

    if( F_WaterOut == SET )
    {
        return;
    }
    else{}

    /*..hui [17-12-28오후 1:03:15] 얼음 추출중에는 변경못하도록 변경..*/
    if( F_IceOut == SET )
    {
        return;
    }
    else{}

    /* 시간 설정모드 중에는 처리 안되도록 막음 250529 CH.PARK */
    if(bit_time_setting_start == SET)
    {
        return;
    }
    else {  }

    if( bit_memento_start == SET )
    {
        return;
    }
    else{}

    if(bit_manual_drain_start == SET)
    {
        return;
    }

    // 2025-09-04 CH.PARK [V1.0.0.5] 소리설정모드에서 얼음 장키 입력으로 얼음기능 ON/OFF 안되도록 개선
    if( bit_volume_setting_start == SET )
    {
        return;
    }
    else{}

    if( mu8_mode == SETTING_ENABLE )
    {
        if( F_IceOn == CLEAR )
        {
            F_IceOn = SET;

            F_IR = SET;               // 만빙감지
            F_Low_IR = SET;           // 저빙감지

            play_voice_setting_ice_on_149();
            start_button_set_display( BUTTON_SET_ICE_OFF_DISPLAY );
            bit_ice_setting_double_disable = SET;
        }
        else
        {
            bit_ice_setting_double_disable = CLEAR;
        }
    }
    else
    {
        if( F_IceOn == SET && bit_ice_setting_double_disable == CLEAR )
        {
            F_IceOn = CLEAR;
            F_IceDoorClose = SET;
            F_IceSelectDoorClose = SET;

            play_voice_setting_ice_off_150();
            start_button_set_display( BUTTON_SET_ICE_OFF_DISPLAY );
        }
        else
        {
            bit_ice_setting_double_disable = CLEAR;
        }
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void cold_water_enable_setting_job( U8 mu8_mode )
{
    if( F_All_Lock == SET )
    {
        return;
    }
    else{}

    if( F_WaterOut == SET )
    {
        return;
    }
    else{}

    if( F_IceOut == SET )
    {
        return;
    }
    else{}

    /* 시간 설정모드 중에는 처리 안되도록 막음 250529 CH.PARK */
    if(bit_time_setting_start == SET)
    {
        return;
    }
    else {  }

    if(bit_manual_drain_start == SET)
    {
        return;
    }

    // 2025-09-04 CH.PARK [V1.0.0.5] 소리설정모드에서 냉수장키 입력으로 냉수기능 ON/OFF 안되도록 개선
    if( bit_volume_setting_start == SET )
    {
        return;
    }
    else{}

    if( bit_memento_start == SET )
    {
        return;
    }
    else{}

    if( mu8_mode == SETTING_ENABLE )
    {
        if( F_Cold_Enable == CLEAR )
        {
            F_Cold_Enable = SET;
            water_select_return_timer_control(0);

            /*..hui [25-2-27오후 5:44:09] 임시..*/
            play_voice_setting_cold_on_124();

            /*..hui [24-4-24오후 5:48:35] 냉수 ON, OFF 시 표시..*/
            start_button_set_display( BUTTON_SET_COLD_DISPLAY );

            /*..hui [18-1-31오전 9:43:54] 냉수 미설정 상태에서 설정으로 변경시 5초 very long키 막음....*/
            /*..hui [18-1-31오전 9:44:00] 키 누르고있으면 연속해서 설정됐다 해제되는 문제..*/
            bit_cold_setting_double_disable = SET;
        }
        else
        {
            bit_cold_setting_double_disable = CLEAR;
        }
    }
    else
    {
        if(F_Cold_Enable == SET && bit_cold_setting_double_disable == CLEAR)
        {
            F_Cold_Enable = CLEAR;
            water_select_return_timer_control(1);

            /*..hui [20-1-6오후 8:06:19] 냉수 버튼으로 냉수 끄면 냉수 설정도 OFF로 변경..*/
            /*..hui [20-2-24오후 7:44:02] 냉수 ONOFF랑 냉수온도 설정 메뉴 분리..*/
            /*gu8_cold_setting_level = COLD_TEMP_OFF;*/

            /*..hui [25-2-27오후 5:44:05] 임시..*/
            play_voice_setting_cold_off_125();

            /*..hui [24-4-24오후 5:48:35] 냉수 ON, OFF 시 표시..*/
            start_button_set_display( BUTTON_SET_COLD_DISPLAY );
        }
        else
        {
            bit_cold_setting_double_disable = CLEAR;
        }
    }
}



/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void setting_mode_start(void)
{
    /*..hui [17-12-28오후 1:03:15] 추출중에는 변경못하도록 변경..*/
    if( F_WaterOut == SET || F_IceOut == SET )
    {
        return;
    }
    else{}

    /*if(F_All_Lock == SET)*/
    if( F_All_Lock == SET )
    {
        return;
    }
    else{}

    if( bit_memento_start == SET )
    {
        return;
    }
    else{}

    if( bit_volume_setting_start == SET )
    {
        return;
    }
    else{}

    if( bit_self_test_start == SET )
    {
        return;
    }
    else{}

    if(bit_manual_drain_start == SET)
    {
        return;
    }

    if( bit_setting_mode_start == CLEAR )
    {
        bit_setting_mode_start = SET;
        play_melody_select_196();
    }
    else{}
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void key_time_setting_job(void)
{
    /*..hui [23-6-14오전 10:37:57] 와이파이 연결되면 시간설정 금지..*/
    if( gu8_Wifi_Connect_State == WIFI_CONNECT )
    {
        play_melody_warning_197();
        return;
    }
    else{}

    if( F_All_Lock == SET )
    {
        //start_all_lock_flick();
        return;
    }
    else{}

    if( F_WaterOut == SET || F_IceOut == SET )
    {
        return;
    }
    else{}

    if(bit_setting_mode_start == SET)
    {
        return;
    }
    else{}

    if( bit_memento_start == SET )
    {
        return;
    }
    else{}

    if( bit_volume_setting_start == SET )
    {
        return;
    }
    else{}

    //if( bit_ice_tank_ster_start == SET )
    //{
        /*play_melody_warning_197();*/
    //    return;
    ///}
    //else{}

    /*..hui [23-9-14오후 3:38:14] 현재시간, 설정시간 설정시에는 진입못하게..*/
    //if( bit_time_setting_start == SET )
    //{
    //    /*play_melody_warning_197();*/
    //    return;
    //}
    //else{}

    if( bit_self_test_start == SET )
    {
        return;
    }
    else{}

    if( bit_time_setting_start == CLEAR )
    {
        play_voice_time_setting_start_144();
        start_time_setting_mode( CURRENT_TIME_SETTING_MODE );
    }
    else
    {
        gu8_time_setting_step = SETTING_TIME_FINISH;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void key_time_setting_ice_water_long_select_job(void)
{
    if( bit_time_setting_start == CLEAR )
    {
        return;
    }
    else{}

    if( gu8_Led_Display_Step == LED_Display__TIME_SETTING )
    {
        Bit0_Ice_Water_Continue = SET;
    }
    else{}
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void key_time_setting_ice_long_select_job(void)
{
    if( bit_time_setting_start == CLEAR )
    {
        return;
    }
    else{}

    if( gu8_Led_Display_Step == LED_Display__TIME_SETTING )
    {
        Bit1_Ice_Continue = SET;
    }
    else{}
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void key_setting_all_lock_off(void)
{
    if( bit_setting_mode_start == SET )
    {
        return;
    }
    else{}

    if( F_All_Lock == CLEAR )
    {
        return;
    }
    else{}

    if(bit_time_setting_start == SET)
    {
        return;
    }

    if( bit_memento_start == SET )
    {
        return;
    }
    else{}

    F_All_Lock = CLEAR;
    play_voice_all_lock_finish_128();
    Bit4_All_Lock_Indicator = CLEAR;

    send_wifi_each_data_control( WIFI_FUNC_0005_LOCK_SEL );
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void key_manual_ice_tank_ster(void)
{
    if( gu8_flushing_mode > FLUSHING_NONE_STATE )
    {
        return;
    }
    else{}

    if( F_All_Lock == SET )
    {
        play_voice_all_lock_select_103();
        return;
    }
    else {  }

    if( bit_setting_mode_start == SET )
    {
        play_melody_warning_197();
        return;
    }
    else{}

    /*..hui [23-5-22오후 7:17:44] 우선 물추출, 얼음추출중에는 막는걸로..*/
    if( F_WaterOut == SET || F_IceOut == SET )
    {
        play_melody_warning_197();
        return;
    }
    else{}

    if( bit_time_setting_start == SET )
    {
        play_melody_warning_197();
        return;
    }
    else{}

    if(bit_time_setting_start == SET)
    {
        return;
    }

    if( bit_ice_tank_ster_start != SET )
    {
        if( Bit3_Ice_Tank_Ster_Operation_Disable_State == SET )
        {
            play_melody_warning_197();
            return;
        }
        else{}

        if( F_First_Hot_Effluent == SET )
        {
            play_melody_warning_197();
            return;
        }
        else{}

        if( F_Tank_Cover_Input == CLEAR )
        {
            play_melody_warning_197();
            return;
        }
        else{}

        /*..hui [23-7-28오후 3:42:30] 중간에 사이드커버, RO 필터 열림 감지되면 정지..*/
        if( bit_filter_all == CLEAR )
        {
            play_melody_warning_197();
            return;
        }
        else{}

        /* 유로살균 진행중에는 수동 고온살균 불가능 */
        if(cody_water_line.gu8_start == SET)
        {
            play_melody_warning_197();
            return;
        }
        else {  }

        /* 수동드레인 진행중에는 수동 고온살균 불가능 */
        if(bit_manual_drain_start == SET)
        {
            play_melody_warning_197();
            return;
        }
        else {  }

        // /* 고온살균 냉수 키 입력 3회 미만 시 시작안됨 (취소는 가능) */
        // if(gu8_passive_hot_ster_key_input_count < 3)
        // {
        //     play_melody_warning_197();
        //     return;
        // }
        // else {  }

        /*..hui [23-10-27오전 11:56:29] 더미탈빙중이었으면 더미탈빙 취소..*/
        if( F_IceInit == SET )
        {
            F_IceInit = CLEAR;
            gu8InitStep = 0;
            gu16IceMakeTime = 0;
            gu16IceHeaterTime = 0;
        }
        else {  }

        play_melody_setting_on_198();

        start_ice_tank_ster();      // 수동 고온살균
    }
    else
    {
        // bit_ice_tank_ster_start = CLEAR;
        // play_melody_setting_off_199();

        // stop_ice_tank_ster();
    }
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  : Memento Here!
***********************************************************************************************************************/
void    key_memento_mode_job(void)
{
    if( gu8_flushing_mode > FLUSHING_NONE_STATE )
    {
        return;
    }
    else{}

    if( F_All_Lock == SET )
    {
        play_voice_all_lock_select_103();
        return;
    }
    else {  }

    bit_memento_start = SET;

    play_melody_select_196();
    memento_display_step_conv(MEMENTO_DISPLAY_START);
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  : Cody Water Line Clean Service
***********************************************************************************************************************/
void key_cody_water_line_clean_job(void)
{
    if( gu8_flushing_mode > FLUSHING_NONE_STATE )
    {
        return;
    }
    else{}

    if( F_All_Lock == SET )
    {
        play_voice_all_lock_select_103();
        return;
    }
    else {  }

    if(bit_time_setting_start == SET)
    {
        return;
    }

    if(cody_water_line.gu8_start == CLEAR)
    {
        /* 코디살균모드 E01,E09 발생 상태에서는 진입안되고 경고음 발생 250716 CH.PARK */
        if((Bit3_Leakage_Sensor_Error__E01 == SET)
        || (Bit6_Main_Water_Flow_Block_Error__E09 == SET)
        )
        {
            play_melody_warning_197();
        }
        else
        {
            cody_water_line.gu8_start = SET;
            play_melody_setting_on_198();
        }

    }
    else
    {
        cody_water_clean_mode_clear();
    }

    cody_water_line.gu8_step = 0;
    cody_water_line.gu16_timer = 0;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void cold_func_onoff_long_key_job(void)
{
    if(gu8_flushing_mode > FLUSHING_NONE_STATE)
    {
        return;
    }

    if(special_mode_check_U8() == CLEAR)
    {
        if(F_Cold_Enable == CLEAR)
        {
            F_Cold_Enable = SET;
            gu16_cold_off_flick_timer = 0;          /* flick 타이머 변수 초기화해야지 표시 사양이 변경됨 250318 CH.PARK */
            Bit2_Cold_Off_Indicator = CLEAR;
            play_voice_setting_cold_on_124();
        }
        else
        {
            F_Cold_Enable = CLEAR;
            play_voice_setting_cold_off_125();
        }
    }
    else
    {

    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void cold_long_flushing_cancel_job(void)
{
    /* 플러싱 취소 */
    if( gu8_flushing_mode > FLUSHING_NONE_STATE )
    {
        if( gu8_flushing_mode == FLUSHING_STANDBY_STATE )
    {
            if((u8FilterResetState == NEO_INO_FILTER_CHANGE)
            && (bit_yes_no_popup == SET)
            )
            {
                cancel_filter_flushing();
            }
            else {  }
        }
        else {  }
    }
    else {  }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void stop_flushing_test_mode_start(void)
{
    /* 2KG는 정수키 3회 이상으로 적용 */
    if( gu8_flushing_finish_input_count >= 3 )
    {
        play_voice_test_mode_151();
        gu8_flushing_mode = FLUSHING_NONE_STATE;
    }
    else
    {
        play_melody_warning_197();
    }
}


