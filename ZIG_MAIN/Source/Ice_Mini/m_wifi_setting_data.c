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
#include    "m_wifi_setting_data.h"


void wifi_hot_lock( U16 mu16_setting );
void wifi_all_lock( U16 mu16_setting );
void wifi_child_lock( U16 mu16_setting );
void wifi_mute( U16 mu16_setting );
void wifi_ice_lock( U16 mu16_setting );
void wifi_hot_enable( U16 mu16_setting );
void wifi_altitude( U16 mu16_setting );
void wifi_ice_first( U16 mu16_setting );
void wifi_ice_enable( U16 mu16_setting );
void wifi_welcome_light( U16 mu16_setting );
void wifi_hot_default_temp( U16 mu16_setting );
void wifi_default_cup( U16 mu16_setting );
void wifi_4_oz_water_using( U16 mu16_setting );
void wifi_6_oz_water_using( U16 mu16_setting );
void wifi_8_oz_water_using( U16 mu16_setting );
void wifi_10_oz_water_using( U16 mu16_setting );
void wifi_12_oz_water_using( U16 mu16_setting );
void wifi_14_oz_water_using( U16 mu16_setting );
void wifi_16_oz_water_using( U16 mu16_setting );
void wifi_20_oz_water_using( U16 mu16_setting );
void wifi_32_oz_water_using( U16 mu16_setting );
void wifi_continue_water_using( U16 mu16_setting );
void wifi_cup_level_order( U16 mu16_setting );
void wifi_hot_temp_level_order( U16 mu16_setting );

void wifi_period_ster_hour( U16 mu16_setting );
void wifi_period_ster_minute( U16 mu16_setting );
void wifi_sleep_mode_enable( U16 mu16_setting );
void wifi_sleep_start_hour( U16 mu16_setting );
void wifi_sleep_start_minute( U16 mu16_setting );
void wifi_sleep_finish_hour( U16 mu16_setting );
void wifi_sleep_finish_minute( U16 mu16_setting );
void wifi_unused_setting_time( U16 mu16_setting );
void wifi_voice_language( U16 mu16_setting );
void wifi_filter_change_type( U16 mu16_setting );

void wifi_my_use( U8 index, U16 mu16_setting );
void wifi_my_temp( U8 index, U16 mu16_setting );
void wifi_my_amount( U8 index, U16 mu16_setting );
void wifi_my_default( U16 mu16_setting );

void start_fota(void);
void wifi_self_test( U16 mu16_setting );

void wifi_wait_mode_enable( U16 mu16_setting );

U8 gu8_hot_level_order;
U8 gu8_hot_default_temp;

U8 gu8_hot_water_count;


U8 gu8_temporary_sleep_mode_start_hour;
U8 gu8_temporary_sleep_mode_start_minute;
U8 gu8_temporary_sleep_mode_finish_hour;
U8 gu8_temporary_sleep_mode_finish_minute;


U8 gu8_temporary_period_ster_hour;
U8 gu8_temporary_period_ster_minute;

extern TYPE_WORD          U16HotTemplSelectW;
#define            U16HotTemplSelect							U16HotTemplSelectW.word
#define            U16HotTemplSelect_L							U16HotTemplSelectW.byte[0]
#define            U16HotTemplSelect_H							U16HotTemplSelectW.byte[1]
#define            Bit0_Temp_Select__45oC						U16HotTemplSelectW.Bit.b0
#define            Bit1_Temp_Select__50oC						U16HotTemplSelectW.Bit.b1
#define            Bit2_Temp_Select__55oC						U16HotTemplSelectW.Bit.b2
#define            Bit3_Temp_Select__60oC						U16HotTemplSelectW.Bit.b3
#define            Bit4_Temp_Select__65oC						U16HotTemplSelectW.Bit.b4
#define            Bit5_Temp_Select__70oC						U16HotTemplSelectW.Bit.b5
#define            Bit6_Temp_Select__75oC						U16HotTemplSelectW.Bit.b6
#define            Bit7_Temp_Select__80oC						U16HotTemplSelectW.Bit.b7
#define            Bit8_Temp_Select__85oC						U16HotTemplSelectW.Bit.b8
#define            Bit9_Temp_Select__90oC						U16HotTemplSelectW.Bit.b9
#define            Bit10_Temp_Select__95oC						U16HotTemplSelectW.Bit.b10
#define            Bit11_Temp_Select__100oC             		U16HotTemplSelectW.Bit.b11

extern TYPE_BYTE          U8IceOutStateB;
#define            u8IceOutState                             U8IceOutStateB.byte
#define            Bit0_Ice_Only_Select_State                U8IceOutStateB.Bit.b0
#define            Bit1_Ice_Plus_Water_Select_State          U8IceOutStateB.Bit.b1


extern U16 gu16_WaterQuantity_half;
extern U16 gu16_WaterQuantity_one;
extern U16 gu16_WaterQuantity_two;
extern U16 gu16_WaterQuantity_four;
extern bit F_IceBreak;
extern bit bit_myWater_setting_start;

extern U16 gu16_my_recipe_menu_on_timer;
extern bit bit_welcome_start_wifi_set;
extern U16 drip_timer;
extern U8 gu8_dripcoffee_percent;
extern U8 gu8_hot_setting_temperature;
extern bit bit_sleep_voice_sync;
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void wifi_hot_lock( U16 mu16_setting )
{
    U8 mu8_on_off = 0;
    U8 mu8_cold_level = 0;

    if( mu16_setting == WIFI_HOT_LOCK_ON )
    {
        F_Hot_Lock = SET;
        play_voice_setting_hot_lock_on_112();

        if( u8WaterOutState == HOT_WATER_SELECT )
        {
            water_select_return_timer_control(1);
        }
        else{}
    }
    else if( mu16_setting == WIFI_HOT_LOCK_OFF )
    {
        F_Hot_Lock = CLEAR;
        play_voice_setting_hot_lock_off_113();
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
void wifi_all_lock( U16 mu16_setting )
{
    if( mu16_setting <= 1 )
    {
        if( mu16_setting == 0 )
        {
            F_All_Lock = CLEAR;
            bit_child_lock_enable = CLEAR;      // 자동잠금 강제 OFF
            play_voice_all_lock_finish_128();
        }
        else /*if( mu16_setting == 1 )*/
        {
            F_All_Lock = SET;
            play_voice_all_lock_start_127();
        }
    }
    else
    {
        play_melody_warning_197();
    }
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  : 미사용!
***********************************************************************************************************************/
void wifi_child_lock( U16 mu16_setting )
{
    if( mu16_setting <= 1 )
    {
        if( mu16_setting == 0 )
        {
            bit_child_lock_enable = CLEAR;
            play_melody_setting_off_199();
        }
        else /*if( mu16_setting == 1 )*/
        {
            bit_child_lock_enable = SET;
            play_melody_setting_on_198();
        }
    }
    else
    {
        play_melody_warning_197();
    }
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  : 미사용! 
***********************************************************************************************************************/
void wifi_mute( U16 mu16_setting )
{
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void wifi_ice_lock( U16 mu16_setting )
{
    if( mu16_setting <= 1 )
    {
        if( mu16_setting == 0 )
        {
            F_Ice_Lock = CLEAR;
            play_voice_setting_ice_lock_off_148();
        }
        else /*if( mu16_setting == 1 )*/
        {
            F_Ice_Lock = SET;
            play_voice_setting_ice_lock_on_147();

            /* 얼으 및 얼음물 선택 상태로 app을 통해 얼음잠금 시,
            선택상태 유지되는 현상 개선 250714 CH.PARK */
            if(u8IceOutState != ICE_SELECT__NONE)
            {
                u8IceOutState = ICE_SELECT__NONE;
            }
            else {  }
        }
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

void wifi_cold_temp( U16 mu16_setting )
{
	if(mu16_setting == 0)
	{
		gu8_cold_setting_level = COLD_TEMP_HIGH;
        play_voice_cold_temp_strong_140();
	}
	else
	{
		gu8_cold_setting_level = COLD_TEMP_LOW;
        play_voice_cold_temp_weak_139();
	}
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  : 미사용!
***********************************************************************************************************************/
void wifi_hot_enable( U16 mu16_setting )
{
    if( mu16_setting <= 1 )
    {
        if( mu16_setting == 0 )
        {
            F_Hot_Enable = CLEAR;
            play_voice_setting_hot_off_122();

            /*..hui [23-7-24오후 1:31:41] 프리히팅 중이었으면 프리히팅 종료..*/
            /////stop_hot_preheating();///////

            /*..hui [24-1-19오후 2:27:45] 온수 선택 상태에서 온수 OFF시 이전 선택으로..*/
            if( u8WaterOutState == HOT_WATER_SELECT )
            {
                water_select_return_timer_control(1);
            }
            else{}
        }
        else /*if( mu16_setting == 1 )*/
        {
            F_Hot_Enable = SET;
            play_voice_setting_hot_on_121();
        }
    }
    else
    {
        play_melody_warning_197();
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  : 미사용!
***********************************************************************************************************************/
void wifi_altitude( U16 mu16_setting )
{
    /*if( mu16_setting <= 2 )*/
    if( mu16_setting <= 3 )
    {
        if( mu16_setting == 0 )
        {
            gu8AltitudeStep = ALTITUDE_1_MODE_HIGH;
            play_melody_setting_on_198();
        }
        else if( mu16_setting == 1 )
        {
            gu8AltitudeStep = ALTITUDE_2_MODE_MID;
            play_melody_setting_on_198();
        }
        else if( mu16_setting == 2 )
        {
            gu8AltitudeStep = ALTITUDE_3_MODE_LOW;
            play_melody_setting_on_198();
        }
        else /*if( mu16_setting == 3 )*/
        {
            gu8AltitudeStep = ALTITUDE_4_MODE_VERY_LOW;
            play_melody_setting_on_198();
        }

        /*..hui [24-11-26오후 4:34:18] 고도모드 변경에따른 온수 디폴트 온도 변경..*/
        ///change_hot_default_altitude();
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
void wifi_ice_first( U16 mu16_setting )
{
    if( mu16_setting == 1 )
    {
        bit_fast_ice_make = COLD_FIRST_ICE_MAKE;
        play_melody_setting_off_199();
    }
    else if( mu16_setting == 3 )
    {
        bit_fast_ice_make = ICE_FIRST_ICE_MAKE;
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
void wifi_ice_enable( U16 mu16_setting )
{
    if( mu16_setting <= 1 )
    {
        if( mu16_setting == 0 )
        {
            F_IceOn = CLEAR;
            play_voice_setting_ice_off_150();
        }
        else /*if( mu16_setting == 1 )*/
        {
            F_IceOn = SET;
            play_voice_setting_ice_on_149();
            stop_icewater_off_flick();
            stop_ice_off_flick();
        }
    }
    else
    {
        play_melody_warning_197();
    }
}

void wifi_voice_select(U16 mu16_setting)
{
	if(mu16_setting == 1)
	{
		gu8Sound_Type = SOUND_TYPE_MUTE;

        Play_Voice(VOICE_70_MODE_SET_MUTE);
	}
	else if(mu16_setting == 2)
	{
		gu8Sound_Type = SOUND_TYPE_MELODY;

        Play_Voice(VOICE_69_MODE_SET_MELODY);
	}
	else 
	{
		gu8Sound_Type = SOUND_TYPE_VOICE;

        Play_Voice(VOICE_68_MODE_SET_VOICE);
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/

void wifi_ice_size( U16 mu16_setting )
{
    if( mu16_setting == 0 )
    {
        bit_ice_size = ICE_SIZE_LARGE;
        play_voice_ice_size_large_144();
    }
    else
    {
        bit_ice_size = ICE_SIZE_SMALL;
        play_voice_ice_size_small_143();
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void wifi_welcome_light( U16 mu16_setting )
{
    if( mu16_setting <= 1 )
    {
        if( mu16_setting == 0 )
        {
            F_Welcome_Led_OnOff = CLEAR;
            play_melody_setting_off_199();
        }
        else /*if( mu16_setting == 1 )*/
        {
            F_Welcome_Led_OnOff = SET;
            bit_welcome_start = CLEAR;
            bit_welcome_wifi_start = SET;
            play_melody_setting_on_198();
        }
    }
    else
    {
        play_melody_warning_197();
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  : 미사용!
***********************************************************************************************************************/
void wifi_hot_default_temp( U16 mu16_setting )
{
    if( mu16_setting <= 3 )
    {
        if( mu16_setting == 0 )
        {
            gu8_hot_default_temp = HOT_SET_TEMP_1__MILK__43_oC;
        }
        else if( mu16_setting == 1 )
        {
            gu8_hot_default_temp = HOT_SET_TEMP_2__TEA__70_oC;
        }
        else if( mu16_setting == 2 )
        {
            gu8_hot_default_temp = HOT_SET_TEMP_3__COFFEE__85_oC;
        }
        else /*if( mu16_setting == 3 )*/
        {
            gu8_hot_default_temp = HOT_SET_TEMP_4__MAX__100_oC;
        }

        /*play_melody_setting_on_198();*/

        if( gu8AltitudeStep == ALTITUDE_3_MODE_LOW )
        {
            /*..hui [24-11-26오후 4:47:48] 데이터 잘못들어온경우..3단계인데 온수설정 4단계 설정으로 온경우..*/
            if( gu8_hot_default_temp == HOT_SET_TEMP_4__MAX__100_oC )
            {
                gu8_hot_default_temp = HOT_SET_TEMP_3__COFFEE__85_oC;
                play_melody_warning_197();
            }
            else
            {
                play_melody_setting_on_198();
            }
        }
        else if( gu8AltitudeStep == ALTITUDE_4_MODE_VERY_LOW )
        {
            /*..hui [24-11-26오후 4:48:27] 4단계인데 온수설정 3단계 or 4단계 설정으로 온경우..*/
            if( gu8_hot_default_temp == HOT_SET_TEMP_4__MAX__100_oC || gu8_hot_default_temp == HOT_SET_TEMP_3__COFFEE__85_oC )
            {
                gu8_hot_default_temp = HOT_SET_TEMP_2__TEA__70_oC;
                play_melody_warning_197();
            }
            else
            {
                play_melody_setting_on_198();
            }
        }
        else
        {
            play_melody_setting_on_198();
        }
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
void wifi_default_cup( U16 mu16_setting )
{
    if(mu16_setting == 0)
    {
        gu8Cup_level_Default = CUP_LEVEL_1_120ML;
    }
    else if(mu16_setting == 1)
    {
        gu8Cup_level_Default = CUP_LEVEL_2_250ML;
    }
    else if(mu16_setting == 2)
    {
        gu8Cup_level_Default = CUP_LEVEL_3_500ML;
    }
    else if(mu16_setting == 3)
    {
        gu8Cup_level_Default = CUP_LEVEL_4_1000ML;
    }
    else {  }

    play_melody_setting_on_198();
    gu8Cup_level = gu8Cup_level_Default;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void wifi_period_ster_hour( U16 mu16_setting )
{

    gu8_temporary_period_ster_hour = (U8)mu16_setting;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void wifi_period_ster_minute( U16 mu16_setting )
{

    gu8_temporary_period_ster_minute = (U8)mu16_setting;

    if( gu8_temporary_period_ster_hour <= 23 && gu8_temporary_period_ster_minute <= 59 )
    {
        gu8_wifi_period_ster_hour = gu8_temporary_period_ster_hour;
        gu8_wifi_period_ster_minute = gu8_temporary_period_ster_minute;
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
void wifi_sleep_mode_enable( U16 mu16_setting )
{
    if( mu16_setting <= 1 )
    {
        if( mu16_setting == 0 )
        {
            bit_sleep_mode_enable = CLEAR;
            bit_sleep_voice_sync = SET;
            play_voice_sleep_mode_disable_146();
        }
        else /*if( mu16_setting == 1 )*/
        {
            bit_sleep_mode_enable = SET;
            bit_sleep_voice_sync = SET;
            play_voice_sleep_mode_enable_145();
        }
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
void wifi_sleep_start_hour( U16 mu16_setting )
{

    gu8_temporary_sleep_mode_start_hour = (U8)mu16_setting;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void wifi_sleep_start_minute( U16 mu16_setting )
{

    gu8_temporary_sleep_mode_start_minute = (U8)mu16_setting;

}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void wifi_sleep_finish_hour( U16 mu16_setting )
{

    gu8_temporary_sleep_mode_finish_hour = (U8)mu16_setting;

}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void wifi_sleep_finish_minute( U16 mu16_setting )
{

    gu8_temporary_sleep_mode_finish_minute = (U8)mu16_setting;


    if( gu8_temporary_sleep_mode_start_hour <= 23 && gu8_temporary_sleep_mode_start_minute <= 59
    && gu8_temporary_sleep_mode_finish_hour <= 23 && gu8_temporary_sleep_mode_finish_minute <= 59 )
    {
        gu8_sleep_mode_start_hour = gu8_temporary_sleep_mode_start_hour;
        gu8_sleep_mode_start_minute = gu8_temporary_sleep_mode_start_minute;
        gu8_sleep_mode_finish_hour = gu8_temporary_sleep_mode_finish_hour;
        gu8_sleep_mode_finish_minute = gu8_temporary_sleep_mode_finish_minute;
        play_melody_setting_on_198();
    }
    else
    {
        play_melody_warning_197();
    }

}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  : 미사용!
***********************************************************************************************************************/
void wifi_unused_setting_time( U16 mu16_setting )
{
    if( mu16_setting >= USE_SAVE_LEARNEND_MODE_MIN_TIME && mu16_setting <= USE_SAVE_LEARNEND_MODE_MAX_TIME)
    {
        gu8_wifi_smart_unused_setting_time = (U8)mu16_setting;
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
void wifi_volume( U16 mu16_setting )
{
    if( mu16_setting == 1 )
    {
        gu8VoiceVolumeLevel = SOUND_VOLUME_LEVEL_1;
    }
    else if( mu16_setting == 2 )
    {
        gu8VoiceVolumeLevel = SOUND_VOLUME_LEVEL_2;
    }
    else if( mu16_setting == 3 )
    {
        gu8VoiceVolumeLevel = SOUND_VOLUME_LEVEL_3;
    }
    else if( mu16_setting == 4 )
    {
        gu8VoiceVolumeLevel = SOUND_VOLUME_LEVEL_4;
    }
    else /*if( mu16_setting == 5 )*/
    {
        gu8VoiceVolumeLevel = SOUND_VOLUME_LEVEL_5;
    }

    play_melody_setting_on_198();
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  : 미사용!
***********************************************************************************************************************/
void wifi_voice_language( U16 mu16_setting )
{
    if( mu16_setting <= LANGUAGE_MAX )
    {
        if( mu16_setting == 0 )
        {
            gu8_voice_language_type = LANGUAGE_TYPE_KOREA;
        }
        else if( mu16_setting == 1 )
        {
            gu8_voice_language_type = LANGUAGE_TYPE_ENGLISH;
        }
        else if( mu16_setting == 2 )
        {
            gu8_voice_language_type = LANGUAGE_TYPE_SPANISH;
        }
        else if( mu16_setting == 3 )
        {
            gu8_voice_language_type = LANGUAGE_TYPE_CHINESE;
        }
        else /*if( mu16_setting == 4 )*/
        {
            gu8_voice_language_type = LANGUAGE_TYPE_FRANCE;
        }

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
void start_fota(void)
{
    gu8_fota_start = SET;

    /*off_all_test_load();*/
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void wifi_self_test( U16 mu16_setting )
{
    /*..hui [21-8-25오후 5:47:58] 0이면 중지, 일반동작..*/
    if( mu16_setting == 0 )
    {
        if( bit_self_test_start == SET )
        {
            if( bit_self_test_stop == CLEAR )
            {
                bit_self_test_stop = SET;

                /*..hui [21-9-8오전 10:26:04] 시작하면 wifi쪽에서 자동으로 보내기때문에 이쪽에서..*/
                gu8_wifi_self_test_state = WIFI_SELF_TEST_STOP;
                gu8_wifi_self_test_progress = WIFI_SELF_TEST_0_PERCENT;
                play_melody_setting_off_199();
            }
            else{}
        }
        else{}
    }
    else if( mu16_setting == 1 )
    {
        /*..hui [21-8-25오후 5:48:08] 1이면 스마트 정밀진단 시작..*/
        if( bit_self_test_start == CLEAR )
        {
            bit_self_test_start = SET;

            /*..hui [21-9-8오전 11:02:26] 이쪽에서 한번 더 막아줌..*/
            if( gu8_wifi_self_test_state == WIFI_SELF_TEST_NO_OPERATION )
            {
                start_self_test();
            }
            else{}
        }
        else{}
    }
    else{}
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/

void wifi_set_temp( U16 mu16_temp, U16 mu16_setting )
{
    if( mu16_setting == 1 )
    {
        if(mu16_temp == HOT_SET_TEMP____45oC)
        {
        	Bit0_Temp_Select__45oC = SET;
        }
        else if(mu16_temp == HOT_SET_TEMP____50oC)
        {
        	Bit1_Temp_Select__50oC = SET;
        }
        else if(mu16_temp == HOT_SET_TEMP____55oC)
        {
        	Bit2_Temp_Select__55oC = SET;
        }
        else if(mu16_temp == HOT_SET_TEMP____60oC)
        {
        	Bit3_Temp_Select__60oC = SET;
        }
        else if(mu16_temp == HOT_SET_TEMP____65oC)
        {
        	Bit4_Temp_Select__65oC = SET;
        }
        else if(mu16_temp == HOT_SET_TEMP____70oC)
        {
        	Bit5_Temp_Select__70oC = SET;
        }
        else if(mu16_temp == HOT_SET_TEMP____75oC)
        {
        	Bit6_Temp_Select__75oC = SET;
        }
        else if(mu16_temp == HOT_SET_TEMP____80oC)
        {
        	Bit7_Temp_Select__80oC = SET;
        }
        else if(mu16_temp == HOT_SET_TEMP____85oC)
        {
        	Bit8_Temp_Select__85oC = SET;
        }
        else if(mu16_temp == HOT_SET_TEMP____90oC)
        {
        	Bit9_Temp_Select__90oC = SET;
        }
        else if(mu16_temp == HOT_SET_TEMP____95oC)
        {
        	Bit10_Temp_Select__95oC = SET;
        }
        else if(mu16_temp == HOT_SET_TEMP____100oC)
        {
        	Bit11_Temp_Select__100oC = SET;
        }
		else
		{
			play_melody_warning_197();
			return;
		}
        play_melody_setting_on_198();
    }
    else
    {
        if(mu16_temp == HOT_SET_TEMP____45oC)
        {
        	Bit0_Temp_Select__45oC = CLEAR;
        }
        else if(mu16_temp == HOT_SET_TEMP____50oC)
        {
        	Bit1_Temp_Select__50oC = CLEAR;
        }
        else if(mu16_temp == HOT_SET_TEMP____55oC)
        {
        	Bit2_Temp_Select__55oC = CLEAR;
        }
        else if(mu16_temp == HOT_SET_TEMP____60oC)
        {
        	Bit3_Temp_Select__60oC = CLEAR;
        }
        else if(mu16_temp == HOT_SET_TEMP____65oC)
        {
        	Bit4_Temp_Select__65oC = CLEAR;
        }
        else if(mu16_temp == HOT_SET_TEMP____70oC)
        {
        	Bit5_Temp_Select__70oC = CLEAR;
        }
        else if(mu16_temp == HOT_SET_TEMP____75oC)
        {
        	Bit6_Temp_Select__75oC = CLEAR;
        }
        else if(mu16_temp == HOT_SET_TEMP____80oC)
        {
        	Bit7_Temp_Select__80oC = CLEAR;
        }
        else if(mu16_temp == HOT_SET_TEMP____85oC)
        {
        	Bit8_Temp_Select__85oC = CLEAR;
        }
        else if(mu16_temp == HOT_SET_TEMP____90oC)
        {
        	Bit9_Temp_Select__90oC = CLEAR;
        }
        else if(mu16_temp == HOT_SET_TEMP____95oC)
        {
        	Bit10_Temp_Select__95oC = CLEAR;
        }
        else if(mu16_temp == HOT_SET_TEMP____100oC)
        {
        	Bit11_Temp_Select__100oC = CLEAR;
        }
		else
		{
			play_melody_warning_197();
			return;
		}
        play_melody_setting_off_199();
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void wifi_my_use( U8 index, U16 mu16_setting )
{
    //LSH zzang
    if( mu16_setting == 0 )
    {
        my_setting[index].use = CLEAR;
        play_melody_setting_off_199();
    }
    else if( mu16_setting == 1 )
    {
        my_setting[index].use = SET;
        play_melody_setting_on_198();
    }
    else
    {
        // play_melody_warning_197();
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void wifi_set_waterquantity( U16 mu16_waterType, U16 mu16_quantity )
{
    if(mu16_waterType == CUP_LEVEL_HALF)
    {
        gu16_WaterQuantity_half = mu16_quantity;
        //quantity conversion해서 들어가도록 변경 해야함
    }
    else if(mu16_waterType == CUP_LEVEL_ONE)
    {
        gu16_WaterQuantity_one = mu16_quantity;
    }
    else if(mu16_waterType == CUP_LEVEL_TWO)
    {
        gu16_WaterQuantity_two = mu16_quantity;
    }
    else if(mu16_waterType == CUP_LEVEL_FOUR)
    {
        gu16_WaterQuantity_four = mu16_quantity;
        play_melody_setting_on_198();
    }
    else
    {
        play_melody_warning_197();
        return;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void wifi_my_temp( U8 index, U16 mu16_setting )
{
    //LSH zzang
    my_setting[index].temp = (U8)mu16_setting;

    if(index == MY_INDEX_MY1)
    {
        Play_Voice(VOICE_153_INFORM_SET_MY1_RECIPE);
    }
    else if(index == MY_INDEX_MY2)
    {
        Play_Voice(VOICE_154_INFORM_SET_MY2_RECIPE);
    }
    else if(index == MY_INDEX_MY3)
    {
        Play_Voice(VOICE_155_INFORM_SET_MY3_RECIPE);
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void wifi_my_amount( U8 index, U16 mu16_setting )
{
    //LSH zzang
    my_setting[index].amount = mu16_setting;

    if(index == MY_INDEX_MY1)
    {
        Play_Voice(VOICE_153_INFORM_SET_MY1_RECIPE);
    }
    else if(index == MY_INDEX_MY2)
    {
        Play_Voice(VOICE_154_INFORM_SET_MY2_RECIPE);
    }
    else if(index == MY_INDEX_MY3)
    {
        Play_Voice(VOICE_155_INFORM_SET_MY3_RECIPE);
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void wifi_wait_mode_enable( U16 mu16_setting )
{
    if( mu16_setting <= 1 )
    {
        if( mu16_setting == 1 )
        {
            bit_waitmode_enable = SET;
            play_melody_setting_on_198();
        }
        else
        {
            bit_waitmode_enable = CLEAR;
            play_melody_setting_off_199();
        }
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
void wifi_my_default( U16 mu16_setting )
{
    if(mu16_setting >= 1 && mu16_setting <= 6)
    {
        /* My1 ~ My3 */
        if(mu16_setting >= 4 && mu16_setting <= 6)
        {
            if(my_setting[mu16_setting-1].use == SET)
            {
                my_receipe_default = (U8)(mu16_setting - 1);
                play_melody_setting_on_198();
            }
            else
            {
                // 미사용하는 MY1,2,3 중 하나를 디폴트로 설정하는 경우 라면으로 다시 초기화 
                my_receipe_default = 0;
            }
        }
        else
        {
            my_receipe_default = (U8)(mu16_setting - 1);
            play_melody_setting_on_198();
            // // 라면,드립커피,차는 원래 쓰는 거라서 셀렉트 음성을 낸다.
            // play_melody_select_196();
        }
    }
    else
    {
        play_melody_warning_197();
    }

    bit_myWater_setting_start = SET;
    my_recipe_select = my_receipe_default;

    if(my_recipe_select != MY_INDEX_DRIPCOFFEE )
    {
        drip_timer = 0;
        gu8_dripcoffee_percent = 0;
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
    
    gu16_water_select_return_time = 0;
    gu16_my_recipe_menu_on_timer = 0;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  : 미사용
***********************************************************************************************************************/
void wifi_set_ice_type(U16 mu16_setting)
{
    /* 범위체크 */
    if(mu16_setting > SET)
    {
        play_melody_warning_197();
        return;
    }
    
    F_IceBreak = mu16_setting;

    if(F_IceBreak == CLEAR)
    {
        play_melody_setting_off_199();
    }
    else
    {
        Play_Voice(VOICE_157_BREAK_ICE_SELECTED);
    }
}



