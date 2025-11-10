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
#include    "Key_Handling.h"
#include "WIFI_SetFunctionData.h"
#include "WIFI_A101x_FunctionList.h"
/**********************************************************************************************/
/* 일반상태 키입력 처리 */
void key_ice_select_job(void);
void key_ice_water_select_job(void);
void key_hot_water_select_job(void);
void hot_setting_change(void);
void key_ambient_water_select_job(void);
void key_cold_water_select_job(void);
void key_amount_select_job(void);
void key_my_select_job(void);
/**********************************************************************************************/
/* 설정모드 키입력 처리 */
void key_setting_select_job(void);
void key_setting_ice_size(void);
void key_setting_ice_off(void);
void key_setting_ice_lock(void);
void key_setting_hot_lock(void);
void key_setting_fast_icemake(void);
void key_setting_cold_power(void);
void key_setting_cold_off(U8 mu8_setting);
void key_setting_sleepmode(void);
void key_setting_all_lock_on(void);
/**********************************************************************************************/
/**********************************************************************************************/

//================================== Key Flag
bit F_KeySet;
bit F_ExtractKeySet;
bit F_TestKeySet;
bit F_LongKeySet;
bit F_IceKeySet;
bit F_Cold_Enable;
bit F_ColdOut;
bit F_ColdOn;
bit F_ColdOff;
bit F_Hot_Enable;
bit F_HotOut;
bit F_HotOn;
bit F_HotOff;
bit F_Room;
bit F_RoomOut;
bit F_Save;
bit F_Conty;
bit F_ContyGo;
bit F_Hot_Lock;
bit F_All_Lock;
bit F_BeIce;
bit F_IceOn;
bit F_IceBreak;     /* 얼음타입 조각 설정 플래그 추가 250217 CH.PARK */
bit F_IceWaterOn;
bit F_Ice_Lock;
///bit F_IceOut;
bit F_IceOutStop;
bit F_LEDOff;
bit F_ColdSet;
bit F_HotSet;
bit F_IcemakeSet;
bit F_SaveSet;
bit F_KeySetBuz;
bit F_SterAuto;
bit F_SterLED;
bit F_Altitude;
bit Be_COMP;
bit Be_EW_POW;
bit Be_F_EWDrainPump;
//================================== S/W 센서
bit F_Out;
bit F_BeOut;
bit F_OutSet;

bit F_WaterOut;         /* 냉온정 추출중 */
bit F_IceOut;           /* 얼음추출중 */
bit F_Ice_Water_Out;    /* 얼음물 추출중 */

U8 gu8LockFlick;
bit F_Mute_Enable;
bit F_Mute_Enable_Old;



bit F_Hot_Temp_Setting;
bit F_Circul_Drain;
U8 gu8_water_out_state_old;

bit F_Extraction_LED_Display_Mode;
bit F_Extraction_LED_Display_Mode_Old;

bit bit_fast_ice_make;
bit bit_ice_size;
bit F_Ice_Make_Priority_Setting_Mode;

U8 gu8_direction_continue_timer;
U8 gu8_direction_continue_max;

U8 gu8_ice_make_priority_settiong_time;

extern bit bit_setting_mode_time_init;
extern U16 gu16_setting_finish_timer;
extern U8 gu8_fnd_right_on_display_flag;
extern U8 gu8_fnd_right_off_display_flag;
extern U16 gu16_fnd_right_onoff_display_timer;
extern U8 gu8_Icewater_level;
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void key_ice_select_job(void)
{    
    // if( gu8_flushing_mode > FLUSHING_NONE_STATE )
    // {
    //     return;
    // }
    // else{}

    /* 필터커버 감지 시 선택불가 하지만 소리는 안냄 (추출버튼만 소리냄) */
    if(bit_filter_all == CLEAR)
    {
        return;
    }

    if(bit_sleep_mode_start == SET)
    {
        power_saving_init();
    }

    if( F_All_Lock == SET )
    {
        play_voice_all_lock_select_103();
        start_all_lock_flick();
        return;
    }

    if(bit_volume_setting_start == SET)
    {
        return;
    }
    else {  }

    if(bit_manual_drain_start == SET)
    {
        return;
    }

    if( F_IceOut == SET )
    {
        play_melody_warning_197();
        return;
    }
    else{}

    if( bit_ice_tank_ster_start == SET )
    {
        play_voice_ice_tray_hot_ster_is_going_86();
        return;
    }
    else{}

    if( F_Ice_Lock == SET )
    {
        start_ice_lock_flick();
        play_voice_ice_lock_ice_out_98();
        return;
    }
    else{}

    /* 유로살균중일 때 */
    if(cody_water_line.gu8_start == SET)
    {
        play_melody_warning_197();
        return;
    }

    /* 얼음물 추출중이면 얼음도 안눌림 250704 CH.PARK */
    if((u8IceOutState == ICE_SELECT__ICE_WATER)
    )
    {
        if((F_WaterOut == SET) 
        || (F_IceOut == SET)
        )
        {
            play_melody_warning_197();
            return;
        }
    }
    else {  }

    if(bit_myWater_setting_start == SET)
    {
        if( F_WaterOut == SET )
        {
            play_melody_warning_197();
            return;
        }
        else {  }
    }
    else {  }

    stop_hot_off_flick();

    /* 얼음OFF 상태면 선택 깜빡되지만 선택은 됨 */
    if(F_IceOn == CLEAR)
    {
        start_ice_off_flick();
        play_voice_ice_off_ice_out_99();
    }
    else
    {
        /*.. sean [25-06-30] 누수걸림시, 버튼 눌렀을 때에 누수 표시되도록 수정 ..*/
		if( Bit3_Leakage_Sensor_Error__E01 == SET )
		{
			play_voice_leakage_error_12();
		}
		else if( Bit17_Tray_Micro_SW_Dual_Detect_Error__E61 == SET
		|| Bit18_Tray_Micro_SW_Up_Move_Error__E62 == SET
		|| Bit19_Tray_Micro_SW_Down_Move_Error__E63 == SET
		|| Bit16_Drain_Pump_Error__E60 == SET
		|| Bit15_Amb_Temp_Open_Short_Error__E43 == SET
		|| Bit7_BLDC_Communication_Error__E27 == SET
		|| bit_bldc_operation_error_total == SET
		||Bit2_Room_Temp_Open_Short_Error__E42 == SET)
		{
			/*..hui [24-1-10오후 3:42:03] 얼음에러는.. 트레이에러, 드레인펌프에러, 외기센서에러, 컴프에러..*/
			/*..hui [24-2-1오후 4:30:53] 정수 온도센서 에러도 추가..*/
			play_voice_ice_error_10();
		}
		else
		{
        	play_melody_select_196();
		}

        gu16_ice_off_flick_timer = 0;
        gu16_icewater_off_flick_timer = 0;
        gu8_indicator_flick_timer = 0;
    }

    bit_myWater_setting_start = CLEAR;
		
    if( u8IceOutState != ICE_SELECT__ICE )
    {
        u8IceOutState = ICE_SELECT__ICE;

        /* 온수선택중 얼음, 얼음물 선택 시 즉시 온수선택 해제 250626 CH.PARK  */
        if((u8WaterOutState == HOT_WATER_SELECT)
        && (F_WaterOut == CLEAR)
        )
        {
            u8WaterOutState = gu8_recovery_water_out_state;
        }
        else {  }

        /* 얼음 용량은 별도로 무조건 1단계부터 선택하도록 수정 250717 CH.PARK */
        gu8_ice_amount_step = ICE_LEVEL_1_STEP;
        
        if( F_WaterOut == SET )
        {
            Extract_Stack.U8_iceSelect = SET;
        }
        else
        {
            Extract_Stack.U8_waterSelect = CLEAR;
        }
    }
    else
    {
        if( F_IceBreak == CLEAR )
        {
            F_IceBreak = SET;
            play_voice_ice_type_break_select_157();
        }
        else
        {
            F_IceBreak = CLEAR;
            play_melody_select_196();
        }
    }

    gu16_ice_select_return_time = 0;
    gu8_clock_hide_state = SET;
    gu8_clock_hide_timer = 0;
    cup_select_init();
}

/**
 * @brief 얼음물 선택 시 호출되는 함수 (얼음물 관련된 변수 초기화)
 * 
 */
void key_ice_water_select_job(void)
{
    // if( gu8_flushing_mode > FLUSHING_NONE_STATE )
    // {
    //     return;
    // }
    // else{}

    /* 필터커버 감지 시 선택불가 하지만 소리는 안냄 (추출버튼만 소리냄) */
    if(bit_filter_all == CLEAR)
    {
        return;
    }

    if( F_All_Lock == SET )
    {
        play_voice_all_lock_select_103();
        start_all_lock_flick();
        return;
    }

    /* 전체 잠금, 어린이 잠금 설정 시 무시처리 (다른 키 공통 사양) */
    if( F_Child_Lock == SET )
    {
        return;
    }
    else{}
    
    if(bit_sleep_mode_start == SET)
    {
        power_saving_init();
    }

   /* 냉온정 출수 중이면 눌리지 않음 (다른 키 공통 사양) */
    if( F_WaterOut == SET )
    {
        play_melody_warning_197();
        return;
    }
    else{}

    /* 얼음 추출중이면 눌리지 않음 (다른 키 공통 사양) */
    if( F_IceOut == SET )
    {
        play_melody_warning_197();
        return;
    }
    else{}

    if(bit_volume_setting_start == SET)
    {
        return;
    }
    else {  }

    if(bit_manual_drain_start == SET)
    {
        return;
    }

    gu8_cup_level_time_init = SET; //LSH zzang 확인필요

    if( bit_ice_tank_ster_start == SET )
    {
        play_voice_ice_tray_hot_ster_is_going_86();
        return;
    }
    else{}

    /* 유로살균중일 때 */
    if(cody_water_line.gu8_start == SET)
    {
        play_melody_warning_197();
        return;
    }

    /* 얼음잠금 상태이면 경고음 + 무시처리  */
    if( F_Ice_Lock == SET )
    {
        start_ice_lock_flick();
        play_voice_ice_lock_ice_out_98();
        return;
    }
    else{}

    stop_hot_off_flick();

    /* 얼음OFF 상태면 선택 깜빡되지만 선택은 됨 */
    if((F_IceOn == CLEAR)
    || (F_Cold_Enable == CLEAR)
    )
    {
        if(F_IceOn == CLEAR)
        {
            start_icewater_off_flick();
            play_voice_ice_off_ice_out_99();
        }
        else if(F_Cold_Enable == CLEAR)
        {
            start_cold_off_flick();
            play_voice_cold_off_cold_select_96();
        }
    }
    else
    {
        /*.. sean [25-06-30] 누수걸림시, 버튼 눌렀을 때에 누수 표시되도록 수정 ..*/
		if( Bit3_Leakage_Sensor_Error__E01 == SET )
		{
			play_voice_leakage_error_12();
		}
		else if( Bit17_Tray_Micro_SW_Dual_Detect_Error__E61 == SET
		|| Bit18_Tray_Micro_SW_Up_Move_Error__E62 == SET
		|| Bit19_Tray_Micro_SW_Down_Move_Error__E63 == SET
		|| Bit16_Drain_Pump_Error__E60 == SET
		|| Bit15_Amb_Temp_Open_Short_Error__E43 == SET
		|| Bit7_BLDC_Communication_Error__E27 == SET
		|| bit_bldc_operation_error_total == SET
		||Bit2_Room_Temp_Open_Short_Error__E42 == SET)
		{
			/*..hui [24-1-10오후 3:42:03] 얼음에러는.. 트레이에러, 드레인펌프에러, 외기센서에러, 컴프에러..*/
			/*..hui [24-2-1오후 4:30:53] 정수 온도센서 에러도 추가..*/
			play_voice_ice_error_10();
		}
		else
		{
        	play_melody_select_196();
		}
        
        gu16_ice_off_flick_timer = 0;
        gu16_icewater_off_flick_timer = 0;
        gu8_indicator_flick_timer = 0;
    }

    // /* 냉수 OFF 상태로 얼음물 선택 시 OFF 표시해야됨 250722 CH.PARK */
    // if( F_Cold_Enable == CLEAR )
    // {
    //     start_cold_off_flick();
    //     play_voice_cold_off_cold_select_96();
    // }
    // else
    // {
    //    play_melody_select_196();    
    // }

    bit_myWater_setting_start = CLEAR;

    if( u8IceOutState != ICE_SELECT__ICE_WATER )
    {
        u8IceOutState = ICE_SELECT__ICE_WATER;

        /* 온수선택중 얼음, 얼음물 선택 시 즉시 온수선택 해제 250626 CH.PARK  */
        if(u8WaterOutState == HOT_WATER_SELECT)
        {
            u8WaterOutState = gu8_recovery_water_out_state;
        }
        else {  }
        
        if(gu8Cup_level >= CUP_LEVEL_CONTINUE)
        {
            gu8_ice_amount_step = ICE_LEVEL_4_STEP;
            gu8_Icewater_level = ICEWATER_CUP_LEVEL_4_500ML;
            gu8Cup_level = CUP_LEVEL_4_1000ML;
        }
        else
        {
            gu8_Icewater_level = gu8Cup_level;
            gu8_ice_amount_step = gu8Cup_level;
        }
    }
    else {  }

    gu16_ice_select_return_time = 0;
    gu8_clock_hide_state = SET;
    gu8_clock_hide_timer = 0;
    cup_select_init();
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void key_hot_water_select_job(void)
{
    // if( gu8_flushing_mode > FLUSHING_NONE_STATE )
    // {
    //     return;
    // }
    // else{}

    /* 필터커버 감지 시 선택불가 하지만 소리는 안냄 (추출버튼만 소리냄) */
    if(bit_filter_all == CLEAR)
    {
        return;
    }

    if( F_All_Lock == SET )
    {
        play_voice_all_lock_select_103();
        start_all_lock_flick();
        return;
    }

    if(bit_sleep_mode_start == SET)
    {
        power_saving_init();
    }

    gu8_cup_level_time_init = SET;

    if( F_WaterOut == SET )
    {
        play_melody_warning_197();
        return;
    }
    else{}

    /* 유로살균중일 때 */
    if(cody_water_line.gu8_start == SET)
    {
        play_melody_warning_197();
        return;
    }

    if( bit_ice_tank_ster_start == SET )
    {
        play_voice_ice_tray_hot_ster_is_going_86();
        return;
    }
    else{}

    if(bit_manual_drain_start == SET)
    {
        return;
    }

    /*..hui [19-10-7오후 3:24:23] 팝업 노출 순위 뒤로..*/
    if(F_Hot_Lock == SET)
    {
        start_hot_off_flick();
    	play_voice_hot_lcok_hot_select_93();
        return;
    }
    else {  }

    /* 얼음물 추출중이면 냉온정 안눌림 250704 CH.PARK */
    if((u8IceOutState == ICE_SELECT__ICE_WATER)
    )
    {
        if((F_WaterOut == SET) 
        || (F_IceOut == SET)
        )
        {
            play_melody_warning_197();
            return;
        }
    }
    else {  }

    gu16_water_select_return_time = 0;

    /* 온수 단수 발생 후, 온수 선택 시 E08 표시 및 선택 불가 250730 CH.PARK */
    if(Bit0_Hot_Water_Flow_Block_Error__E08 == SET)
    {
        play_melody_warning_197();
        gu8_error_popup_enable = SET;
        return;

    }
    else
    {
        play_melody_select_196();

    }
    
    if(F_IceOut == SET)
    {
        Extract_Stack.U8_waterSelect = SET;
    }
    else
    {
        Extract_Stack.U8_iceSelect = CLEAR;
    }
    
    if((u8WaterOutState != HOT_WATER_SELECT)
    || (u8IceOutState != ICE_SELECT__NONE)
    || (bit_myWater_setting_start == SET)
    )
    {
        u8WaterOutState = HOT_WATER_SELECT;

        /*..hui [18-8-14오후 1:31:34] 온수온도 디폴트는 무조건 커피..*/
        /*..hui [23-5-11오후 4:29:49] 디폴트 온수온도 85도로 변경.. 85가 사용 빈도가 더 높음?..*/
        gu8_hot_water_select_index = 0;
        gu8_hot_setting_temperature = selected_hot_temp[gu8_hot_water_select_index];

        if( Bit3_Leakage_Sensor_Error__E01 == SET )
        {
            play_voice_leakage_error_12();
        }
        else if( Bit4_Hot_In_Temp_Open_Short_Error__E46 == SET
        || Bit5_Hot_Heater_OverHeat_Error__E40 == SET
        || Bit20_Hot_Out_Temp_Open_Short_Error__E47 == SET )
        {
            /*..sean [25-05-31] 온수에러 음성은 온수센서 에러만..*/
            play_voice_hot_error_9();
        }
		else
		{
			play_melody_select_196();
		}

        /*..hui [23-9-25오후 3:37:28] 키 조작으로 선택 변경시에만 A1014만 보냄..*/
        send_wifi_water_select_data_control( WIFI_FUNC_000B_WATER_SEL );
    }
    else
    {
        hot_setting_change();
    }

    gu8_clock_hide_state = SET;
    gu8_clock_hide_timer = 0;

    /* 냉온정 선택 시 얼음선택 상태 미선택으로 설정 250625 CH.PARK */
    u8IceOutState = ICE_SELECT__NONE;

    /* MY워터 선택 후 온수 누른 경우 미선택으로 설정 250630 CH.PARK */
    bit_myWater_setting_start = CLEAR;

    bit_ice_water_extract = CLEAR;
    gu8_ice_water_ext_start_timer_100ms = CLEAR;
    cup_select_init();
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void hot_setting_change(void)
{
    gu8_hot_water_select_index++;
    if(gu8_hot_water_select_index > 3)
    {
        gu8_hot_water_select_index = 0;
    }
    else {  }

    gu8_hot_setting_temperature = selected_hot_temp[gu8_hot_water_select_index];
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void key_ambient_water_select_job(void)
{
    // if( gu8_flushing_mode > FLUSHING_NONE_STATE )
    // {
    //     return;
    // }
    // else{}

    /* 필터커버 감지 시 선택불가 하지만 소리는 안냄 (추출버튼만 소리냄) */
    if(bit_filter_all == CLEAR)
    {
        return;
    }

    if( F_All_Lock == SET )
    {
        play_voice_all_lock_select_103();
        start_all_lock_flick();
        return;
    }

    if(bit_sleep_mode_start == SET)
    {
        power_saving_init();
    }

    gu8_cup_level_time_init = SET;

    if(F_WaterOut == SET)
    {
        play_melody_warning_197();
        return;
    }
    else{}

    if(bit_volume_setting_start == SET)
    {
        return;
    }
    else {  }

    if(bit_manual_drain_start == SET)
    {
        return;
    }

    /* 유로살균중일 때 */
    if(cody_water_line.gu8_start == SET)
    {
        play_melody_warning_197();
        return;
    }

    if( bit_ice_tank_ster_start == SET )
    {
        play_voice_ice_tray_hot_ster_is_going_86();
        return;
    }
    else{}

    /* 얼음물 추출중이면 냉온정 안눌림 250704 CH.PARK */
    if((u8IceOutState == ICE_SELECT__ICE_WATER)
    )
    {
        if((F_WaterOut == SET) 
        || (F_IceOut == SET)
        )
        {
            play_melody_warning_197();
            return;
        }
    }
    else {  }

    bit_myWater_setting_start = CLEAR;
    
    if(F_IceOut == SET)
    {
        Extract_Stack.U8_waterSelect = SET;
    }
    else
    {
        Extract_Stack.U8_iceSelect = CLEAR;
    }

	/*sean [25-05-31] error에 대한 음성 추가*/
    if( Bit3_Leakage_Sensor_Error__E01 == SET )
    {
        play_voice_leakage_error_12();
    }
	else
	{
		play_melody_select_196();
	}

    if( u8WaterOutState != PURE_WATER_SELECT )
    {
        u8WaterOutState = PURE_WATER_SELECT;
        /*..hui [23-9-25오후 3:37:28] 키 조작으로 선택 변경시에만 A1014만 보냄..*/
        send_wifi_water_select_data_control( WIFI_FUNC_000B_WATER_SEL );
    }
    else {  }

    gu16_water_select_return_time = 0;
    
    play_melody_select_196();

    stop_ice_off_flick();
    stop_ice_lock_flick();
    stop_icewater_off_flick();
    stop_cold_off_flick();
    stop_hot_off_flick();

    gu8_clock_hide_state = SET;
    gu8_clock_hide_timer = 0;
    
    /* 냉온정 선택 시 얼음선택 상태 미선택으로 설정 250625 CH.PARK */
    u8IceOutState = ICE_SELECT__NONE;

    bit_ice_water_extract = CLEAR;
    gu8_ice_water_ext_start_timer_100ms = CLEAR;
    cup_select_init();
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void key_cold_water_select_job(void)
{
    /* 교체플러싱 취소 ->  [냉수] 단키 입력 */
    if(gu8_Led_Display_Step == LED_Display__FLUSHING)
    {
        if( gu8_flushing_mode == FLUSHING_STANDBY_STATE )
        {
            cold_long_flushing_cancel_job();

            play_voice_filter_flushing_cancle_25();

            return;
        }
        else {  }
    } else {  }

    // if( gu8_flushing_mode > FLUSHING_NONE_STATE )
    // {
    //     return;
    // }
    // else{}

    /* 필터커버 감지 시 선택불가 하지만 소리는 안냄 (추출버튼만 소리냄) */
    if(bit_filter_all == CLEAR)
    {
        return;
    }
    
    if( F_All_Lock == SET )
    {
        play_voice_all_lock_select_103();
        start_all_lock_flick();
        return;
    }

    if(bit_sleep_mode_start == SET)
    {
        power_saving_init();
    }

    gu8_cup_level_time_init = SET;

    if( F_WaterOut == SET )
    {
        play_melody_warning_197();
        return;
    }
    else{}

    /* 유로살균중일 때 */
    if(cody_water_line.gu8_start == SET)
    {
        play_melody_warning_197();
        return;
    }

    if(bit_manual_drain_start == SET)
    {
        return;
    }

    if( bit_ice_tank_ster_start == SET )
    {
        play_voice_ice_tray_hot_ster_is_going_86();
        return;
    }
    else{}

    if( F_Cold_Enable == CLEAR )
    {
        start_cold_off_flick();
        //play_melody_warning_197();
        play_voice_cold_off_cold_select_96();
    }
    else
    {
        play_melody_select_196();
    }

    /* 얼음물 추출중이면 냉온정 안눌림 250704 CH.PARK */
    if((u8IceOutState == ICE_SELECT__ICE_WATER)
    )
    {
        if((F_WaterOut == SET) 
        || (F_IceOut == SET)
        )
        {
            play_melody_warning_197();
            return;
        }
    }
    else {  }

    bit_myWater_setting_start = CLEAR;
    if(F_IceOut == SET)
    {
        Extract_Stack.U8_waterSelect = SET;
    }
    else
    {
        Extract_Stack.U8_iceSelect = CLEAR;
    }

    if( u8WaterOutState != COLD_WATER_SELECT )
    {
        u8WaterOutState = COLD_WATER_SELECT;

        if( Bit3_Leakage_Sensor_Error__E01 == SET )
        {
            play_voice_leakage_error_12();
        }
        else if( Bit14_Cold_Temp_Open_Short_Error__E44 == SET
        || Bit7_BLDC_Communication_Error__E27 == SET
        || bit_bldc_operation_error_total == SET )
        {
            /*..hui [23-12-15오후 4:14:02] 냉수에러 음성 추가..*/
            /*..hui [24-1-10오후 3:38:37] 냉수 에러 음성은.. 냉수센서 에러, 컴프 에러일때만 출력하기로.. 함재진..*/
            play_voice_cold_error_7();
        }
        else {  }
        
        /*..hui [23-9-25오후 3:37:28] 키 조작으로 선택 변경시에만 A1014만 보냄..*/
        send_wifi_water_select_data_control( WIFI_FUNC_000B_WATER_SEL );
    }
    else{}

    gu16_water_select_return_time = 0;

    gu8_clock_hide_state = SET;
    gu8_clock_hide_timer = 0;

    /* 냉온정 선택 시 얼음선택 상태 미선택으로 설정 250625 CH.PARK */
    u8IceOutState = ICE_SELECT__NONE;

    bit_ice_water_extract = CLEAR;
    gu8_ice_water_ext_start_timer_100ms = CLEAR;
    cup_select_init();
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void key_amount_select_job(void)
{
    // if( gu8_flushing_mode > FLUSHING_NONE_STATE )
    // {
    //     return;
    // }
    // else{}

    /* 필터커버 감지 시 선택불가 하지만 소리는 안냄 (추출버튼만 소리냄) */
    if(bit_filter_all == CLEAR)
    {
        return;
    }

    if( F_All_Lock == SET )
    {
        play_voice_all_lock_select_103();
        start_all_lock_flick();
        return;
    }

    if(bit_sleep_mode_start == SET)
    {
        power_saving_init();
    }

    if(bit_volume_setting_start == SET)
    {
        return;
    }
    else {  }

    /* 유로살균중일 때 */
    if(cody_water_line.gu8_start == SET)
    {
        play_melody_warning_197();
        return;
    }

    if(bit_memento_start == SET)
    {
        return;
    }

    if(bit_volume_setting_start == SET)
    {
        return;
    }

    if(bit_manual_drain_start == SET)
    {
        return;
    }

    if( bit_setting_mode_start == SET )
    {
        /* 설정모드 해제 (단키입력) */
        bit_setting_mode_start = CLEAR;

        gu16_fnd_right_onoff_display_timer = 0;
		gu8_fnd_right_on_display_flag = CLEAR;
		gu8_fnd_right_off_display_flag = CLEAR;
        
        /* 설정모드 해제 */
        play_voice_setting_mode_out_77();
        return;
    }
    else{}

    if( bit_ice_tank_ster_start == SET )
    {
        play_voice_ice_tray_hot_ster_is_going_86();
        return;
    }
    else{}

    /* 얼음물 추출중이면 냉온정 안눌림 250704 CH.PARK */
    if((u8IceOutState == ICE_SELECT__ICE_WATER)
    )
    {
        if((F_WaterOut == SET) 
        || (F_IceOut == SET)
        )
        {
            play_melody_warning_197();
            return;
        }
    }
    else {  }

    if(bit_myWater_setting_start == SET)
    {
        play_melody_warning_197();
        return;
    }
    else {  }

    stop_ice_off_flick();
    stop_ice_lock_flick();
    stop_icewater_off_flick();
    stop_cold_off_flick();
    stop_hot_off_flick();

    gu8_cup_level_time_init = SET;
    /*..hui [19-8-22오후 2:45:46] 온수 선택 중 사용자/물량 버튼 눌릴 경우 복귀 시간 초기화..캐럿 사양..*/
    gu8_hot_select_time_init = SET;
    bit_myWater_setting_start = CLEAR;

    /* 소개모드 및 일반상태에서만 용량설정 가능 250725 CH.PARK */
    /* 슬립모드 상태에서 용량 버튼만 소리가 안나는 현상 개선 250807 CH.PARK */
    if((gu8_Led_Display_Step == LED_Display__MAIN)
    || (gu8_Led_Display_Step == LED_Display__INSTRUCTION)
    || (gu8_Led_Display_Step == LED_Display__SLEEP)
    )
    {
        if( u8IceOutState == ICE_SELECT__ICE )
        {
            if(gu8_ice_amount_step == ICE_LEVEL_1_STEP)
            {
                gu8_ice_amount_step = ICE_LEVEL_2_STEP;
                // gu8Cup_level = CUP_LEVEL_2_250ML;
            }
            else if(gu8_ice_amount_step == ICE_LEVEL_2_STEP)
            {
                gu8_ice_amount_step = ICE_LEVEL_3_STEP;
                // gu8Cup_level = CUP_LEVEL_3_500ML;
            }
            else if(gu8_ice_amount_step == ICE_LEVEL_3_STEP)
            {
                gu8_ice_amount_step = ICE_LEVEL_4_STEP;
                // gu8Cup_level = CUP_LEVEL_4_1000ML;
            }
            else if(gu8_ice_amount_step == ICE_LEVEL_4_STEP)
            {
                gu8_ice_amount_step = ICE_LEVEL_1_STEP;
                // gu8Cup_level = CUP_LEVEL_1_120ML;
            }
        }
        else if(u8IceOutState == ICE_SELECT__ICE_WATER)
        {
            if(gu8_Icewater_level == ICEWATER_CUP_LEVEL_1_150ML)
            {
                gu8_ice_amount_step = ICE_LEVEL_2_STEP;
                gu8_Icewater_level = ICEWATER_CUP_LEVEL_2_250ML;
                gu8Cup_level = CUP_LEVEL_2_250ML;
            }
            else if(gu8_Icewater_level == ICEWATER_CUP_LEVEL_2_250ML)
            {
                gu8_ice_amount_step = ICE_LEVEL_3_STEP;
                gu8_Icewater_level = ICEWATER_CUP_LEVEL_3_350ML;
                gu8Cup_level = CUP_LEVEL_3_500ML;
            }
            else if(gu8_Icewater_level == ICEWATER_CUP_LEVEL_3_350ML)
            {
                gu8_ice_amount_step = ICE_LEVEL_4_STEP;
                gu8_Icewater_level = ICEWATER_CUP_LEVEL_4_500ML;
                gu8Cup_level = CUP_LEVEL_4_1000ML;
            }
            else if(gu8_Icewater_level == ICEWATER_CUP_LEVEL_4_500ML)
            {
                gu8_ice_amount_step = ICE_LEVEL_1_STEP;
                gu8_Icewater_level = ICEWATER_CUP_LEVEL_1_150ML;
                gu8Cup_level = CUP_LEVEL_1_120ML;
            }
        }
        else
        {
            if(gu8Cup_level == CUP_LEVEL_1_120ML)
            {
                gu8Cup_level = CUP_LEVEL_2_250ML;
            }
            else if(gu8Cup_level == CUP_LEVEL_2_250ML)
            {
                gu8Cup_level = CUP_LEVEL_3_500ML;
            }
            else if(gu8Cup_level == CUP_LEVEL_3_500ML)
            {
                gu8Cup_level = CUP_LEVEL_4_1000ML;
            }
            else if(gu8Cup_level == CUP_LEVEL_4_1000ML)
            {
                gu8Cup_level = CUP_LEVEL_CONTINUE;
            }
            else
            {
                gu8Cup_level = CUP_LEVEL_1_120ML;
            }
        }
        play_melody_select_196();
    }
    else {  }


    /*..hui [20-1-7오전 9:56:14] 물량 선택 LED 표시용..*/
    gu8_cup_led_select = SET;

    gu16_ice_select_return_time = 0;
    gu8_clock_hide_state = SET;
    gu8_clock_hide_timer = 0;
    cup_select_init();
}

/**
 * @brief MY 버튼 눌렀을 시 처리함수
 * 
 */
void    key_my_select_job(void)
{
    // if( gu8_flushing_mode > FLUSHING_NONE_STATE )
    // {
    //     return;
    // }
    // else{}

    /* 필터커버 감지 시 선택불가 하지만 소리는 안냄 (추출버튼만 소리냄) */
    if(bit_filter_all == CLEAR)
    {
        return;
    }

    if( F_All_Lock == SET )
    {
        play_voice_all_lock_select_103();
        start_all_lock_flick();
        return;
    }

    if(bit_sleep_mode_start == SET)
    {
        power_saving_init();
    }

    if( F_WaterOut == SET )
    {
        play_melody_warning_197();
        return;
    }
    else{}

    if( F_IceOut == SET )
    {
        play_melody_warning_197();
        return;
    }
    else{}

    gu8_cup_level_time_init = SET;

    if(( F_WaterOut == SET )
    || (F_IceOut == SET)
    )
    {
        play_melody_warning_197();
        return;
    }
    else{}

    if(bit_volume_setting_start == SET)
    {
        return;
    }
    else {  }

    if(bit_manual_drain_start == SET)
    {
        return;
    }

    /* 유로살균중일 때 */
    if(cody_water_line.gu8_start == SET)
    {
        play_melody_warning_197();
        return;
    }

    if( bit_ice_tank_ster_start == SET )
    {
        play_voice_ice_tray_hot_ster_is_going_86();
        return;
    }
    else{}
    
    if( F_WaterOut != SET )                /* 냉정수 추출중이 아닐 때 평상 시 설정 */
    {
        //LSH zzang
        if(bit_myWater_setting_start == CLEAR)
        {
            /* MY워터 선택 시에도 flick 표시 즉시 없앰 250624 CH.PARK */
            stop_ice_lock_flick();
            stop_icewater_off_flick();
            stop_cold_off_flick();
            stop_hot_off_flick();

            gu8_cup_led_select = CLEAR;
            bit_myWater_setting_start = SET;
            gu16_my_recipe_menu_on_timer = 0;
            my_recipe_select = my_receipe_default;

            play_melody_select_196();
            gu8_water_out_state_old = u8WaterOutState;
        }
        else
        {
            play_melody_select_196();
            if(my_recipe_select == RECIPE_RAMEN)
            {
                my_recipe_select = RECIPE_DRIPCOFFEE;
                // play_voice_drip_coffee_recipe_select_148();
            }
            else if(my_recipe_select == RECIPE_DRIPCOFFEE)
            {
                my_recipe_select = RECIPE_TEA;
                // play_voice_tea_recipe_select_149();
            }
            else if(my_recipe_select == RECIPE_TEA)
            {
                if(my_setting[MY_INDEX_MY1].use == SET)
                {
                    my_recipe_select = MY_INDEX_MY1;
                    // play_voice_my1_recipe_select_150();
                }
                else if(my_setting[MY_INDEX_MY2].use == SET)
                {
                    my_recipe_select = MY_INDEX_MY2;
                    // play_voice_my2_recipe_select_151();
                }
                else if(my_setting[MY_INDEX_MY3].use == SET)
                {
                    my_recipe_select = MY_INDEX_MY3;
                    // play_voice_my3_recipe_select_152();
                }
                else
                {
                    my_recipe_select = MY_INDEX_RAMEN;
                    // play_voice_ramyeon_recipe_select_147();
                }
            }
            else if(my_recipe_select == MY_INDEX_MY1)
            {
                if(my_setting[MY_INDEX_MY2].use == SET)
                {
                    my_recipe_select = MY_INDEX_MY2;
                    // play_voice_my2_recipe_select_151();
                }
                else if(my_setting[MY_INDEX_MY3].use == SET)
                {
                    my_recipe_select = MY_INDEX_MY3;
                    // play_voice_my3_recipe_select_152();
                }
                else
                {
                    my_recipe_select = MY_INDEX_RAMEN;
                    // play_voice_ramyeon_recipe_select_147();
                }
            }
            else if(my_recipe_select == MY_INDEX_MY2)
            {
                if(my_setting[MY_INDEX_MY3].use == SET)
                {
                    my_recipe_select = MY_INDEX_MY3;
                    // play_voice_my3_recipe_select_152();
                }
                else
                {
                    my_recipe_select = MY_INDEX_RAMEN;
                    // play_voice_ramyeon_recipe_select_147();
                }
            }
            else if(my_recipe_select == MY_INDEX_MY3)
            {
                my_recipe_select = MY_INDEX_RAMEN;
                // play_voice_ramyeon_recipe_select_147();
            }
            else
            {
                my_recipe_select = MY_INDEX_RAMEN;
                // play_voice_ramyeon_recipe_select_147();
            }

            if(my_recipe_select != MY_INDEX_DRIPCOFFEE )
            {
                drip_timer = 0;
                gu8_dripcoffee_percent = 0;
            }

            gu16_my_recipe_menu_on_timer = 0;
        }

        if(my_setting[my_recipe_select].temp == 6)
        {
            u8WaterOutState = COLD_WATER_SELECT;
        }
        else if(my_setting[my_recipe_select].temp == 27)
        {
            u8WaterOutState = PURE_WATER_SELECT;
        }
        else
        {
            u8WaterOutState = HOT_WATER_SELECT;
            
            gu8_hot_setting_temperature = ((my_setting[my_recipe_select].temp)-45)/5;
        }

        gu16_cup_level_default_return_time = 0;
        
        gu16_water_select_return_time = 0;
        
        gu8_clock_hide_state = SET;
        gu8_clock_hide_timer = 0;
        u8Extract_Continue = CLEAR;
        
        gu16_cold_off_flick_timer = CLEAR;
        gu8_indicator_flick_timer = CLEAR;

        /* 냉온정 선택 시 얼음선택 상태 미선택으로 설정 250625 CH.PARK */
        u8IceOutState = ICE_SELECT__NONE;

        bit_ice_water_extract = CLEAR;
        gu8_ice_water_ext_start_timer_100ms = CLEAR;
    }
}

void key_setting_select_job()
{
    // if( gu8_flushing_mode > FLUSHING_NONE_STATE )
    // {
    //     return;
    // }
    // else{}

    /* 소리설정 때에는 무시 */
    if(bit_volume_setting_start == SET)
    {
        return;
    }
    else {  }
    
    /* 시간설정 때에는 무시 */
    if(bit_time_setting_start == SET)
    {
        return;
    }
    else {  }

    /* 메멘토모드일 때 무시 */
    if( bit_memento_start == SET )
    {
        return;
    }
    else {  }

    if( F_WaterOut == SET || F_IceOut == SET )
    {
        return;
    }
    else{}

    if(cody_water_line.gu8_start == SET)
	{
        return;
    }

    if( F_All_Lock == SET )
    {
        play_voice_all_lock_select_103();
        start_all_lock_flick();
        return;
    }

    if(bit_manual_drain_start == SET)
    {
        return;
    }

	if(bit_setting_mode_start == SET)
	{

	}
	else
	{
		bit_setting_mode_start = SET;

		if( F_WaterOut == SET || F_Out == SET )
		{
			play_melody_warning_197();
		}
		else
		{
            /* 설정모드 진입 */
            play_voice_setting_mode_in_43();
        }
	}
}

void key_setting_ice_size(void)
{
    if( bit_setting_mode_start == CLEAR )
	{
		return;
	}
	else{}

	bit_setting_mode_time_init = SET;

	if(bit_ice_size == ICE_SIZE_LARGE)
	{
		bit_ice_size = ICE_SIZE_SMALL;
		play_voice_ice_size_small_143();
	}
	else
	{
		bit_ice_size = ICE_SIZE_LARGE;
		play_voice_ice_size_large_144();
	}
}
void key_setting_cold_off(U8 mu8_setting)
{
	bit_setting_mode_time_init = SET;

    if( bit_setting_mode_start == CLEAR )
	{
		return;
	}
	else{}
    	
    if(mu8_setting == SET)
	{
        gu16_fnd_right_onoff_display_timer = 0;
		gu8_fnd_right_on_display_flag = SET;
		gu8_fnd_right_off_display_flag = CLEAR;

		water_select_return_timer_control(1);

		/*..hui [20-1-6오후 8:06:19] 냉수 버튼으로 냉수 끄면 냉수 설정도 OFF로 변경..*/
		/*..hui [20-2-24오후 7:44:02] 냉수 ONOFF랑 냉수온도 설정 메뉴 분리..*/
		/*gu8_cold_setting_level = COLD_TEMP_OFF;*/

		play_voice_setting_cold_on_124();
	}
	else
	{
        gu16_fnd_right_onoff_display_timer = 0;
		gu8_fnd_right_on_display_flag = CLEAR;
		gu8_fnd_right_off_display_flag = SET;

        Bit2_Cold_Off_Indicator = CLEAR;
        gu16_cold_off_flick_timer = 0;          /* flick 타이머 변수 초기화해야지 표시 사양이 변경됨 250318 CH.PARK */
		water_select_return_timer_control(0);

		play_voice_setting_cold_off_125();
	}
    
    F_Cold_Enable = mu8_setting;
}
void key_setting_ice_lock(void)
{
	bit_setting_mode_time_init = SET;

	if( bit_setting_mode_start == CLEAR )
	{
		return;
	}
	else{}
	
	F_Ice_Lock = ~F_Ice_Lock;
	
	if( F_Ice_Lock == SET )
	{
		play_voice_setting_ice_lock_on_147();
	}
	else
	{
		play_voice_setting_ice_lock_off_148();
	}
	
	send_wifi_each_data_control( WIFI_FUNC_0032_ICELOCK_SEL );
}

void key_setting_hot_lock(void)
{
    if( bit_setting_mode_start == CLEAR )
	{
		return;
	}
	else{}

	bit_setting_mode_time_init = SET;

	// if( bit_setting_mode_start == CLEAR )
	// {
	// 	return;
	// }
	// else{}

	F_Hot_Lock = ~F_Hot_Lock;

	if( F_Hot_Lock == SET )
	{
		play_voice_setting_hot_lock_on_112();

		/*..hui [24-1-19오후 2:23:12] 온수 선택 상태 -> 온수잠금 시 온수 선택상태 해제되도록..*/
		if( u8WaterOutState == HOT_WATER_SELECT )
		{
			water_select_return_timer_control(1);
		}
		else{}
	}
	else
	{
        play_voice_setting_hot_lock_off_113();
	}
}
void key_setting_fast_icemake(void)
{
	bit_setting_mode_time_init = SET;

	if( bit_setting_mode_start == CLEAR )
	{
		return;
	}
	else{}

	if( bit_fast_ice_make == COLD_FIRST_ICE_MAKE )
    {
        bit_fast_ice_make = ICE_FIRST_ICE_MAKE;
        play_melody_setting_on_198();
    }
    else
    {
        bit_fast_ice_make = COLD_FIRST_ICE_MAKE;
        play_melody_setting_off_199();
    }

  send_wifi_each_data_control( WIFI_FUNC_0008_ICE_SEL );
}

void key_setting_cold_power(void)
{
	bit_setting_mode_time_init = SET;

	if( bit_setting_mode_start == CLEAR )
	{
		return;
	}
	else{}

	if(gu8_cold_setting_level == COLD_POWER_HIGH)
	{
		gu8_cold_setting_level = COLD_POWER_LOW;
        play_voice_cold_temp_weak_139();
	}
	else
	{
		gu8_cold_setting_level = COLD_POWER_HIGH;
        play_voice_cold_temp_strong_140();
	}

}
void key_setting_ice_off(void)
{
	bit_setting_mode_time_init = SET;

	if( bit_setting_mode_start == CLEAR )
	{
		return;
	}
	else{}

	if(F_IceOn == SET)
	{
		F_IceOn = CLEAR;
        
        /* 얼음선택 상태 [얼음] 선택으로 Default 설정 250625 CH.PARK */
        u8IceOutState = ICE_SELECT__NONE;

        gu16_fnd_right_onoff_display_timer = 0;
		gu8_fnd_right_on_display_flag = CLEAR;
		gu8_fnd_right_off_display_flag = SET;

        F_IceDoorClose = SET;
        F_IceSelectDoorClose = SET;
		play_voice_setting_ice_off_150();
	}
	else
	{
		F_IceOn = SET;

        Bit0_Ice_Lock_Indicator = CLEAR;
        gu16_ice_off_flick_timer = 0;
        gu16_icewater_off_flick_timer = 0;
        gu8_indicator_flick_timer = 0;
        
        gu16_fnd_right_onoff_display_timer = 0;
		gu8_fnd_right_on_display_flag = SET;
		gu8_fnd_right_off_display_flag = CLEAR;

		play_voice_setting_ice_on_149();
	}
}
void key_setting_sleepmode(void)
{
	bit_setting_mode_time_init = SET;

	if( bit_setting_mode_start == CLEAR )
	{
		return;
	}
	else{}
    
    if(bit_first_time_setting == CLEAR)
	{
		play_voice_eco_mode_not_available_51();
		return;
	}

    if(bit_sleep_mode_enable == CLEAR)
    {
        bit_sleep_mode_enable = SET;
        play_voice_sleep_mode_enable_145();
    }
    else
    {
        bit_sleep_mode_enable = CLEAR;
        play_voice_sleep_mode_disable_146();
    }
}

void key_setting_all_lock_on(void)
{  
	bit_setting_mode_time_init = SET;

	if( bit_setting_mode_start == CLEAR )
	{
		return;
	}
	else{}

    if(F_All_Lock == CLEAR)
	{
		F_All_Lock = SET;
		play_voice_all_lock_start_127();
        
        /* 즉시 설정모드 해제 */
        bit_setting_mode_start = CLEAR;
		send_wifi_each_data_control( WIFI_FUNC_0005_LOCK_SEL );
	}
	else
	{
        
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  : Memento Here! 
***********************************************************************************************************************/
void memento_mode_cancel(void)
{
    if( bit_memento_start == CLEAR )
    {
        return;
    }
    else{}

    play_melody_setting_off_199();      // 메멘토모드 해제니까 해제음
    bit_memento_start = CLEAR;
    memento_display_step_conv(MEMENTO_DISPLAY_START);
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  : Memento Here! 
***********************************************************************************************************************/
void memento_mode_error_clear(void)
{
    if( bit_memento_start == CLEAR )
    {
        return;
    }
    else{}

    play_melody_setting_on_198();       // 제어사양서와 동일하게

    // 1번 에러 초기화 안돼서 개선 250717 CH.PARK
    u8Memento_Buff[0] = CLEAR;
    u8Memento_Buff[1] = CLEAR;
    u8Memento_Buff[2] = CLEAR;
    u8Memento_Buff[3] = CLEAR;
    u8Memento_Buff[4] = CLEAR;
    u8Memento_Buff[5] = CLEAR;
    F_ErrorSave = SET;      /* 저장 */
}





