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
#include    "Key_Input.h"


void Scan_Port_Select( U8 Scan_value );
void Led_Port_Clear(void);
void Key_Input_Check(U8 u8key_state, U8 u8key_num);
void Key_Matrix_Input( U8 Scan_value );
void Key_Input_Sampling( U32 u32key_input_value);
void Key_Management(U32 u32key_input_value);
void Key_Short_Input( U32 u32key_input_value);
void Key_Long_Input( U32 u32key_input_value);
void Key_Very_Long_Input( U32 u32key_input_value);
void Key_Normal_Long_Input( U32 u32key_input_value);
void Key_Middle_Input( U32 u32key_input_value);

U8 special_mode_check_U8(void);

//void Extract_KeyInput(void);

bit bit_cold_setting_double_disable;
bit bit_hot_setting_double_disable;
bit bit_ice_setting_double_disable;

U8  u8Scan_Value;
U32  u32Fix_Key_Input_Value;
U32  gu32_Key_Input;

//U16  u16Key_Input;
U32  u32Key_Buffer;

U16  u16Key_Sampling_Counter;
bit F_Key_Short_Push_State;
bit F_Key_Middle_Push_State;
bit F_Key_Long_Push_State;
bit F_Key_Very_Long_Push_State;
bit F_Key_Normal_Long_Push_State;

U8  u8Key_Input_Value;

U16  u16Key_Long_Counter;
U16  u16Key_Short_Counter;
U16 u16LedOutValue;

bit F_CoverOpenSet;
U16 gu16ExtrKeyDelay;

bit F_SterReady;
bit F_SterProhibit;
bit F_SterComplete;
bit F_SterWashCancel;

U16 gu16IceOutOff;
U8 gu8ICELockLEDFlick;
//U16 gu16KeyTime;

bit F_Cold_Water_VeryLongKey_Disable;
bit F_Hot_Water_VeryLongKey_Disable;

U8 gu8_debug_finish_count;
U16 gu16_develop_key_clear_time;

U8 gu8_hot_drain_cold;

// �÷��� ��ŵ�� ī��Ʈ ����
U8 gu8_flushing_finish_input_count;
U16 gu16_flushing_finish_key_clear_time;

// Ʈ���� ���»�� �Է¿� ī��Ʈ ����
U8 gu8_passive_hot_ster_key_input_count;
U16 gu16_passive_hot_ster_key_clear_timer;

U8 gu8_bright_test_mode;

TYPE_BYTE          U8LedFlickerStateB;
#define            u8LedFlickerState                           U8LedFlickerStateB.byte
#define            Bit0_Ice_Lock_LED_Flick                     U8LedFlickerStateB.Bit.b0
#define            Bit1_Ice_Lock_Extract_LED_Flick             U8LedFlickerStateB.Bit.b1
#define            Bit2_Hot_Water_Lock_LED_Flick               U8LedFlickerStateB.Bit.b2
#define            Bit3_All_Lock_Water_Extract_LED_Flick       U8LedFlickerStateB.Bit.b3




bit bit_setting_start;

extern bit bit_setting_mode_time_init;
extern FLUSHING_STEP gu8_flushing_mode;
extern bit bit_install_flushing_state;
extern ICE_STEP gu8IceStep;
extern U8 gu8_hot_setting_temperature;
extern U16 u16Efluent_Time;
extern bit F_Effluent_OK;
extern U8 u8Extract_Continue;
extern HOT_STER_STEP gu8_hot_ster_step;
extern bit bit_instruction_mode_start;

extern void Main_PBA_Test_Mode_Start();
extern void start_instruction_mode(void);
extern void stop_flushing_test_mode_start(void);
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void Key_Input(void)        // it operates per 1ms
{
    /*..hui [19-11-7���� 9:55:18] �ʱ� Ű ����..*/
    if(F_FW_Version_Display_Mode != SET)
    {
        return;
    }
    else{}

    Key_Input_Sampling(gu32_Key_Input);
    Key_Management(gu32_Key_Input);

    key_input_ice_extract();        /* ���� �����ư */
    key_input_water_extract();      /* �ÿ��� �����ư */

    /*..hui [25-1-9���� 5:15:48] ���� ��ư ����..*/
    // key_input_extract();
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void Key_Input_Sampling( U32 u32key_input_value)
{
    U8 mu8_filter = 0;

    mu8_filter = 1;

    if(u32key_input_value == u32Key_Buffer)
    {
        if(u32Key_Buffer == UControl_Key_Mask)
        {
            u16Key_Sampling_Counter = 0;
            F_Key_Short_Push_State = CLEAR;
            F_Key_Middle_Push_State = CLEAR;
            F_Key_Long_Push_State = CLEAR;
            F_Key_Very_Long_Push_State = CLEAR;
            F_Key_Normal_Long_Push_State = CLEAR;

            u8TimeSettingLongKey = 0;
            gu8_time_set_keeping_time = 0;  // PCH JJANG
        }
        else
        {
            u16Key_Sampling_Counter++;
        }

        if(u16Key_Sampling_Counter >= mu8_filter)
        {
            u16Key_Short_Counter = u16Key_Sampling_Counter;
            u16Key_Long_Counter++;
        }
        else{}
    }
    else
    {
        u32Key_Buffer = u32key_input_value;
        u16Key_Sampling_Counter = 0;
        u16Key_Long_Counter = 0;
        u16Key_Short_Counter = 0;

        //F_Key_Short_Push_State = CLEAR;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void Key_Management(U32 u32key_input_value)
{
    if(F_Key_Short_Push_State == CLEAR)
    {
        if( (u16Key_Short_Counter >= 1)
        && (u16Key_Short_Counter < 1000)
        )
        {
            Key_Short_Input( u32key_input_value );
            F_Key_Short_Push_State = SET;
            u16Key_Short_Counter = 0;
        }
        else{}
    }
    else
    {
        /* �߰� ��Ű�Է� ó�� (1�� �̻� ~ 2�� �̸�) */
        if(F_Key_Middle_Push_State == CLEAR)
        {
            if( (u16Key_Long_Counter >= 1000 && u16Key_Long_Counter < 2000) )
            {
                F_Key_Middle_Push_State = SET;
                
                Key_Middle_Input( u32key_input_value );
            }
            else{}
        }
        else
        {
            /* 1�ܰ� ��Ű�Է� ó�� (2�� �̻� ~ 3�� �̸�) */
            if(F_Key_Long_Push_State == CLEAR)
            {
                if( (u16Key_Long_Counter >= 2000 && u16Key_Long_Counter < 3000) )
                {
                    F_Key_Long_Push_State = SET;

                    Key_Long_Input( u32key_input_value );
                }
                else{}
            }
            else
            {
                /* 2�ܰ� ��Ű�Է� ó�� (3�� �̻� ~ 5�� �̸�) */
                if(F_Key_Normal_Long_Push_State == CLEAR)
                {
                    if( (u16Key_Long_Counter >= 3000 && u16Key_Long_Counter < 5000) )
                    {
                        F_Key_Normal_Long_Push_State = SET;

                        Key_Normal_Long_Input( u32key_input_value );
                    }
                    else{}
                }
                else
                {
                    /* 3�ܰ� ��Ű�Է� ó�� (7�� �̻�) */
                    if(F_Key_Very_Long_Push_State == CLEAR)
                    {
                        if( u16Key_Long_Counter >= 7000 )
                        {
                            F_Key_Very_Long_Push_State = SET;
                            u16Key_Long_Counter = 0;

                            Key_Very_Long_Input( u32key_input_value );
                        }
                        else{}
                    }
                    else{}
                }
            }
        }
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void Key_Short_Input( U32 u32key_input_value )
{
    U8 mu8_return = 0;

    if( gu8_fota_start == SET )
    {
        return;
    }
    else{}

    if( bit_self_test_start == SET )
    {
        play_melody_warning_197();
        return;
    }
    else{}

    // gu8_clear_hot_block_error = SET;
    gu8_clear_block_error = SET;

    /* ������ �� ���·� ������ � ��ư�� ������ ���������� ȭ���� �̻��ϰ� �ٲ�� ���� ������ 250704 CH.PARK */
    if((F_WaterOut == CLEAR)
    && (F_IceOut == CLEAR)
    )
    {
        bit_error_display_finish = SET;
    }
    else {  }

    gu8_time_setting_return_time = 0;
    // bit_clock_start_time_init = SET;

	bit_setting_mode_time_init = SET;
    bit_instruction_mode_time_init = SET;

    power_saving_init();
    stop_button_set_display();

	/*..sean [25-05-29] �Ѵ� ��Ű�� �����ϵ��� ����..*/
    if( gu8_flushing_mode > FLUSHING_NONE_STATE && bit_instruction_mode_start == CLEAR )
    {
    	if( gu8_flushing_mode == FLUSHING_STANDBY_STATE )
        {
            /*..hui [23-9-15���� 2:05:21] �÷��� ȭ�鿡�� �� ��ư(����) ��Ű ������ ��Ҹ�~..*/
            if( u32key_input_value == KEY_AMBIENT_SELECT )
            {
                gu8_flushing_finish_input_count++;

                /* 3ȸ �̻� */
                if(gu8_flushing_finish_input_count >= 3)
                {
                    gu8_flushing_finish_input_count = 3;
                }
                else {  }
            }
            else{}
        }
        else {  }
    }
    else {  }

    if(( F_WaterOut == CLEAR )
    && (F_IceOut == CLEAR)
    )
    {
        bit_welcome_start = SET;
    }
    else {  }

    bit_30_min_no_use_start = CLEAR;
    gu32_no_use_30_min_timer = CLEAR;

    /* �� �� ����ڰ� � ��ư�� ������ �ٷ� ��ǥ�ô� Ǯ����. 250714 CH.PARK */
    if(( gu8_Led_Display_Step == LED_Display__WIFI_PAIRING )
    // && (gu8_Wifi_Pairing_State == )
    )
    {
        //bit_wifi_pairing_start = CLEAR;
        finish_pairing_screen();
    }
    else {  }

    switch(u32key_input_value)
    {
        case KEY_ICE_SELECT:                /* ���� ���� */
            /*..hui [23-2-9���� 6:28:05] ���ø�忡���� �ȴ���..*/
            if(special_mode_check_U8() == CLEAR)
            {
                if( gu8_flushing_mode > FLUSHING_NONE_STATE
                && bit_instruction_mode_start == CLEAR
                && u8FilterResetState == FILTER_NO_CHANGE
                )
                {
                    return;
                }
                else {  }

                key_ice_select_job();
            }
            else if(bit_time_setting_start == SET)
            {
                Bit1_Ice_Plus_Indicator = SET;
                play_melody_select_196();
            }
            else{}
        break;

        case KEY_ICE_WATER_SELECT:          /* ������ ���� */
            /*..hui [23-2-9���� 6:28:05] ���ø�忡���� �ȴ���..*/
            if(special_mode_check_U8() == CLEAR)
            {
                if( gu8_flushing_mode > FLUSHING_NONE_STATE
                && bit_instruction_mode_start == CLEAR
                && u8FilterResetState == FILTER_NO_CHANGE
                )
                {
                    return;
                }
                else {  }

                key_ice_water_select_job();
            }
            else if(bit_time_setting_start == SET)
            {
                Bit0_Ice_Water_Minus_Indicator = SET;
                play_melody_select_196();
            }
            else {  }
        break;

        case KEY_HOT_SELECT:
            /*..hui [23-2-9���� 6:28:05] ���ø�忡���� �ȴ���..*/
            if(special_mode_check_U8() == CLEAR)
            {
                if( gu8_flushing_mode > FLUSHING_NONE_STATE
                && bit_instruction_mode_start == CLEAR
                && u8FilterResetState == FILTER_NO_CHANGE
                )
                {
                    return;
                }
                else {  }

                key_hot_water_select_job();
            }

        break;

        case KEY_AMBIENT_SELECT:                /* ���� */
            /*..hui [23-2-9���� 6:28:05] ���ø�忡���� �ȴ���..*/
            if(special_mode_check_U8() == CLEAR)
            {
                if( gu8_flushing_mode > FLUSHING_NONE_STATE
                && bit_instruction_mode_start == CLEAR
                && u8FilterResetState == FILTER_NO_CHANGE
                )
                {
                    return;
                }
                else {  }

                key_ambient_water_select_job();
            }
            else if(bit_volume_setting_start == SET)
            {
                play_melody_select_196();
                gu8_sound_setting_return_time = 0;

                if(gu8Sound_Type != SOUND_TYPE_MUTE)
                {
                    gu16_volume_setting_mode_timer = 0;
                    if(gu8VoiceVolumeLevel  > SOUND_VOLUME_LEVEL_1)
                    {
                        gu8VoiceVolumeLevel--;
                    }
                    else {  }
                }
                else {  }
            }
            else {  }
        break;

        case KEY_COLD_SELECT:                   /* �ü� ��Ű */
            /*..hui [23-2-9���� 6:28:05] ���ø�忡���� �ȴ���..*/
            if(special_mode_check_U8() == CLEAR)
            {
                if( gu8_flushing_mode > FLUSHING_NONE_STATE
                && bit_instruction_mode_start == CLEAR
                && u8FilterResetState == FILTER_NO_CHANGE
                )
                {
                    return;
                }
                else {  }

                key_cold_water_select_job();
            }
            else if(bit_volume_setting_start == SET)
            {
                gu16_volume_setting_mode_timer = 0;       /* 7�� */
                gu8_sound_setting_return_time = 0;

                gu8Sound_Type++;
                if(gu8Sound_Type > SOUND_TYPE_MUTE)
                {
                    gu8Sound_Type = SOUND_TYPE_VOICE;
                }
                else {  }

                /* ���� ���¿� ������� �Ʒ� ������ �������� �� 250617 CH.PARK */
                if(gu8Sound_Type == SOUND_TYPE_MUTE)
                {
                    Play_Voice(VOICE_70_MODE_SET_MUTE);
                }
                else if(gu8Sound_Type == SOUND_TYPE_MELODY)
                {
                    Play_Voice(VOICE_69_MODE_SET_MELODY);
                }
                else if(gu8Sound_Type == SOUND_TYPE_VOICE)
                {
                    Play_Voice(VOICE_68_MODE_SET_VOICE);
                }
                else {  }
            }
            else {  }

            /* ���� ���»���� ���� �ü�Ű 3ȸ �Է� */
            gu8_passive_hot_ster_key_input_count++;
            if(gu8_passive_hot_ster_key_input_count >= 3)
            {
                gu8_passive_hot_ster_key_input_count = 3;
            }
        break;

        case KEY_AMOUNT_SELECT:                 /* �뷮 */

            /*..hui [23-2-9���� 6:28:05] ���ø�忡���� �ȴ���..*/
            if(bit_time_setting_start == SET)
            {
                Bit2_Settings_Switch_Indicator = SET;
                play_melody_select_196();
            }
            else if(bit_volume_setting_start == SET)
            {
                play_melody_select_196();
                gu8_sound_setting_return_time = 0;

                if(gu8Sound_Type != SOUND_TYPE_MUTE)
                {
                    gu16_volume_setting_mode_timer = 0;
                    if(gu8VoiceVolumeLevel < SOUND_VOLUME_LEVEL_5)
                    {
                        gu8VoiceVolumeLevel++;
                    }
                    else {  }
                }
                else {  }
            }
            else
            {
                if( gu8_flushing_mode > FLUSHING_NONE_STATE
                && bit_instruction_mode_start == CLEAR
                && u8FilterResetState == FILTER_NO_CHANGE
                )
                {
                    return;
                }
                else {  }

                key_amount_select_job();
            }
        break;

        case KEY_MY_SELECT:                            /* MY ��ư */
            if(special_mode_check_U8() == CLEAR)
            {
                if( gu8_flushing_mode > FLUSHING_NONE_STATE
                && bit_instruction_mode_start == CLEAR
                && u8FilterResetState == FILTER_NO_CHANGE
                )
                {
                    return;
                }
                else {  }

                key_my_select_job();
            }
        break;

        case KEY_SETTING_ICE_SIZE:              /* ������� */
            if(Bit0_Front_Led_Ice_Size == SET)
            {
                key_setting_ice_size();
            }
            break;

        case KEY_SETTING_ICE_OFF:               /* ����OFF */
            if(Bit1_Front_Led_Ice_Off == SET)
            {
                key_setting_ice_off();
            }
            break;

        case KEY_SETTING_HOT_LOCK:              /* �¼���� */
            if(Bit2_Front_Led_Hot_Lock_Text == SET)
            {
                key_setting_hot_lock();
            }
            break;

        case KEY_SETTING_ICE_FIRST:             /* �������� */
            if(Bit3_Front_Led_Ice_First == SET)
            {
                key_setting_fast_icemake();
            }
            break;

        case KEY_SETTING_COLD_TEMP:             /* �ü����� */
            if(Bit4_Front_Led_Cold_Temp == SET)
            {
                key_setting_cold_power();
            }
            break;

        case KEY_SETTING_SLEEP_MODE:            /* ��ħ��� */
            if(Bit5_Front_Led_Sleep_Mode_Text == SET)
            {
                key_setting_sleepmode();
            }
            break;

        case KEY_SETTING_ICE_LOCK:              /* ������� */
            if(Bit0_Front_Led_Ice_Lock == SET)
            {
                key_setting_ice_lock();
            }
            break;

        case KEY_SETTING_ALL_LOCK:              /* ��ü��� */
            if(Bit1_Front_Led_All_Lock == SET)
            {
                key_setting_all_lock_on();
            }
            break;

        case KEY_COLD_WATER_OFF:                /* �ü�OFF */
            if(F_Cold_Enable == CLEAR)
            {
                key_setting_cold_off(SET);
            }
            else
            {
                key_setting_cold_off(CLEAR);
            }
            break;

        default:
            NOP();
        break;
    }

}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void Key_Middle_Input( U32 u32key_input_value)
{
    #if 1
    // if( gu8_flushing_mode > FLUSHING_NONE_STATE )
    // {
    //     return;
    // }
    // else{}

    if( gu8_fota_start == SET )
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
    #endif

    /* ����Ŀ�� ���� �� ����Ұ� */
    if(bit_filter_cover == CLEAR)
    {
        return;
    }

    switch(u32key_input_value)
    {
        case KEY_ICE_WATER_SELECT:
            //
            key_time_setting_ice_water_long_select_job();

        break;

        case KEY_ICE_SELECT:
            //
            key_time_setting_ice_long_select_job();

        break;

        default:

            NOP();
        break;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void Key_Long_Input( U32 u32key_input_value)
{

    /* ����Ŀ�� ���� �� ����Ұ� */
    // if(bit_filter_cover == CLEAR)
    // {
    //     play_voice_filter_cover_open_3();
    //     return;
    // }

    /* ���»�� �߿��� ��Ű�Է� ���� 250721 (1kg�� ����) 250721 CH.PARK */
    if( gu8_fota_start == SET )
    {
        return;
    }
    else{}

    if( bit_self_test_start == SET )
    {
        return;
    }
    else{}

    if(bit_ice_tank_ster_start == SET)
    {
        return;
    }

    switch(u32key_input_value)
    {

        case LONG_KEY_VOLUME_2S:
            if( bit_memento_start == SET )
            {
                return;
            }
            else{}

            if(cody_water_line.gu8_start == SET)
            {
                return;
            }
            else {}

            // if( gu8_flushing_mode > FLUSHING_NONE_STATE
            // && bit_instruction_mode_start == CLEAR )
            // {
            //     return;
            // }
            // else{}

            if(bit_volume_setting_start == CLEAR)
            {
                play_melody_setting_on_198();
                bit_volume_setting_start = SET;
                gu16_volume_setting_mode_timer = 0;       /* 7�� */
            }
            else
            {
                play_melody_setting_off_199();
                bit_volume_setting_start = CLEAR;
            }
            break;

        case LONG_KEY_MANUAL_FACTORY_TEST:

        break;

        case LONG_KEY_FLUSHING_SKIP:
            if( gu8_Led_Display_Step == LED_Display__FLUSHING )
            {
                stop_flushing_test_mode_start();
            }
            else{}
        break;

        /* �������׽�Ʈ��� 250403 CH.PARK */
        case LONG_KEY_3S_TESTPROGRAM:
            if( gu8_flushing_mode > FLUSHING_NONE_STATE )
            {
                return;
            }
            else{}

            if( F_All_Lock == SET )
            {
                play_voice_all_lock_select_103();
                start_all_lock_flick();
                return;
            }
            else {  }

            if(gu8_durable_test_start == CLEAR)
            {
                gu8_durable_test_start = SET;
                play_melody_setting_on_198();
            }
            else
            {
                gu8_durable_test_start = CLEAR;
                play_melody_setting_off_199();
            }
            break;

        case LONG_KEY_3S_HOT_STER:

            if( bit_memento_start == SET )
            {
                return;
            }
            else{}

            if(cody_water_line.gu8_start == SET)
            {
                return;
            }
            else {}

            if( gu8_flushing_mode > FLUSHING_NONE_STATE
            && bit_instruction_mode_start == CLEAR )
            {
                return;
            }
            else{}

            if(special_mode_check_U8() == CLEAR)
            {
                key_manual_ice_tank_ster();
            }
            else {  }
            break;

        case LONG_KEY_3S_MEMENTO:

            if(cody_water_line.gu8_start == SET)
            {
                return;
            }
            else {}

            if( gu8_flushing_mode > FLUSHING_NONE_STATE
            && bit_instruction_mode_start == CLEAR )
            {
                return;
            }
            else{}

            if(special_mode_check_U8() == CLEAR)
            {
                key_memento_mode_job();
            }
            else {  }
            break;

        /* Cody Water Line Clean Service */
        case LONG_KEY_3S_CODY_WATER_LINE:
            if( bit_memento_start == SET )
            {
                return;
            }
            else{}

            if( gu8_flushing_mode > FLUSHING_NONE_STATE
            && bit_instruction_mode_start == CLEAR )
            {
                return;
            }
            else{}

            if(special_mode_check_U8() == CLEAR)
            {
                key_cody_water_line_clean_job();
            }
            else {  }
            break;

        case LONG_KEY_3S_ICE_TRAY_TEST:

            if( bit_memento_start == SET )
            {
                return;
            }
            else{}

            if(cody_water_line.gu8_start == SET)
            {
                return;
            }
            else {}

            if( gu8_flushing_mode > FLUSHING_NONE_STATE
            && bit_instruction_mode_start == CLEAR )
            {
                return;
            }
            else{}

            start_ice_tray_test();
            break;

        case LONG_KEY_3S_TAKEOFF_ICE:
            if( bit_memento_start == SET )
            {
                return;
            }
            else{}

            if(cody_water_line.gu8_start == SET)
            {
                return;
            }
            else {}

            if( gu8_flushing_mode > FLUSHING_NONE_STATE
            && bit_instruction_mode_start == CLEAR )
            {
                return;
            }
            else{}

            start_cody_takeoff_ice();
            break;

        case KEY_AMBIENT_SELECT:
            if(bit_memento_start == SET )
            {
                /*..hui [23-9-26���� 6:25:52] �޸��� ��忡���� �޸��� ���� ������..*/
                memento_mode_error_clear();     /* Memento Here! */
            }
            else {  }
            break;

        case LONG_KEY_3S_HOT_DRAIN_COLD:
            if( bit_memento_start == SET )
            {
                return;
            }
            else{}

            if(cody_water_line.gu8_start == SET)
            {
                return;
            }
            else {}

            if( gu8_flushing_mode > FLUSHING_NONE_STATE
            && bit_instruction_mode_start == CLEAR )
            {
                return;
            }
            else{}

            if((bit_setting_mode_start == CLEAR )
            &&(bit_time_setting_start == CLEAR)
            &&(bit_memento_start == CLEAR )
            &&(bit_volume_setting_start == CLEAR))
            {
                if(gu8_hot_drain_cold == SET)
                {
                    gu8_hot_drain_cold = CLEAR;
                    play_melody_setting_off_199();
                }
                else
                {
                    gu8_hot_drain_cold = SET;
                    play_melody_setting_on_198();
                }
            }

        break;

        // case LONG_KEY_3S_BRIGHT_TEST:
        //     if((bit_setting_mode_start == CLEAR )
        //     &&(bit_time_setting_start == CLEAR)
        //     &&(bit_memento_start == CLEAR )
        //     &&(bit_volume_setting_start == CLEAR))
        //     {
        //         if(gu8_bright_test_mode == SET)
        //         {
        //             gu8_bright_test_mode = CLEAR;
        //             play_melody_setting_off_199();
        //         }
        //         else
        //         {
        //             gu8_bright_test_mode = SET;
        //             play_melody_setting_on_198();
        //         }
        //     }

        // break;

        case LONG_KEY_3S_CANCEL_COMP_DELAY:
            calcel_comp_delay();
            break;

        case KEY_AMOUNT_SELECT:         /* Memento Here! */
            memento_mode_cancel();
            break;

        default:
            NOP();
        break;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void Key_Normal_Long_Input( U32 u32key_input_value)
{
    if( gu8_fota_start == SET )
    {
        return;
    }
    else{}

    if(bit_filter_all == CLEAR)
	{
		if( (u32key_input_value == LONG_KEY_3S_BLE_FAIRING)
        || (u32key_input_value == LONG_KEY_3S_WIFI_FAIRING)
        )
		{
            /* do nothing */
		}
		else
		{
			return;
		}
	}

    if( bit_self_test_start == SET )
    {
        return;
    }
    else{}

    if(bit_ice_tank_ster_start == SET)
    {
        return;
    }

    if( bit_memento_start == SET )
    {
        return;
    }
    else{}

    if(cody_water_line.gu8_start == SET)
    {
        return;
    }
    else {}

    if( gu8_flushing_mode > FLUSHING_NONE_STATE )
	{
		if((u32key_input_value == LONG_KEY_3S_BLE_FAIRING)
        || (u32key_input_value == LONG_KEY_3S_MANAGER_INSTRUCTION_MODE)
		|| (u32key_input_value == KEY_AMOUNT_SELECT)
        || (u32key_input_value == KEY_SETTING_ALL_LOCK)
		)
        {

		}
		else
		{
			return;
		}
	}

	if( F_WaterOut == SET || F_IceOut == SET )
	{
		return;

	}
	else{}

    switch(u32key_input_value)
    {
        /* ������� ���� (����) 250211 CH.PARK */
        case KEY_AMOUNT_SELECT:

            if( gu8_flushing_mode > FLUSHING_NONE_STATE
            && bit_instruction_mode_start == CLEAR )
            {
                return;
            }
            else{}

            key_setting_select_job();
        break;

        case KEY_HOT_SELECT:
        if( gu8_flushing_mode > FLUSHING_NONE_STATE
        && bit_instruction_mode_start == CLEAR )
        {
            return;
        }
        else{}

        if(bit_manual_drain_start == SET)
        {
            return;
        }

        if(special_mode_check_U8() == CLEAR)
        {
            if( F_Hot_Lock == CLEAR )
            {
                play_voice_setting_hot_lock_on_112();
                F_Hot_Lock = SET;
                water_select_return_timer_control(1);
            }
            else
            {
                play_voice_setting_hot_lock_off_113();
                F_Hot_Lock = CLEAR;

            }
        }
        else {  }

        break;

        case LONG_KEY_3S_MANUAL_DRAIN:
        if(special_mode_check_U8() == CLEAR)
        {
            key_manual_drain();
        }
        else {  }
        break;

        case KEY_ICE_SELECT:

            /*..hui [23-2-9���� 6:28:05] ���ø�忡���� �ȴ���..*/
            if( bit_setting_mode_start == CLEAR )
            {
                ice_on_off_setting_job(SETTING_ENABLE);
            }
            else{}

        break;

        case KEY_COLD_SELECT:       /* �ü� �߰� ��Ű */
            /*..hui [23-2-9���� 6:28:05] ���ø�忡���� �ȴ���..*/
            if( bit_setting_mode_start == CLEAR )
            {
                cold_water_enable_setting_job(SETTING_ENABLE);
            }
            else{}
        break;


        case LONG_KEY_3S_WIFI_ONOFF:
        // 2025-09-04 CH.PARK [V1.0.0.5] �������� ���õ� ��� ����Ű�� �Ϲݻ��¿����� �����ϵ��� ���� (�÷��� ����)
        if(special_mode_check_U8() == SET)
        {
            return;
        }
        else {}

        if( F_All_Lock == SET )
        {

        }
        else
        {
            WifiKey(WIFI_KEY_ONOFF);
        }
        break;

    case LONG_KEY_3S_BLE_FAIRING:
        // 2025-09-04 CH.PARK [V1.0.0.5] �������� ���õ� ��� ����Ű�� �Ϲݻ��¿����� �����ϵ��� ���� (�÷��� ����)
        if(special_mode_check_U8() == SET)
        {
            return;
        }
        else {}

        /*..hui [21-7-15���� 12:47:23] BLE �� - ���� 3��..*/
        if( F_All_Lock == SET )
        {
            /*..hui [24-7-18���� 2:57:17] ��ü ����̾ �÷����߿��� �� ����..*/
            if( gu8_flushing_mode > FLUSHING_NONE_STATE )
            {
                WifiKey(WIFI_KEY_BLE);
            }
            else
            {
                /*..hui [24-7-18���� 2:20:10] ��ü����� ���������� �Է� �����ϵ���..*/
                /*..hui [24-7-18���� 2:57:34] ��ü��� - �÷��� �̿� �����ϋ�..*/
                if( gu8_Wifi_Cert_State == WIFI_CERTIFICATE_ON )
                {
                    WifiKey(WIFI_KEY_BLE);
                }
                else{}
            }
        }
        else
        {
            WifiKey(WIFI_KEY_BLE);

            if( gu8_Led_Display_Step == LED_Display__MAIN )
            {
                if( gu8_wifi_ap_mode == CLEAR && gu8_ble_ap_mode == CLEAR )
                {
                    if( GetWifiStatusValue( WIFI_STATUS_DISP ) == WIFI_DISP_OFF )
                    {
                        /*gu8_Lcd_Display_Step = LCD_Display__WIFI_PAIRING;*/
                    }
                    else
                    {
                        // bit_wifi_pairing_start = SET;
                        // /* �� �ٽ� �õ��� ���� [0%] ���� �����ϵ��� �� 250714 CH.PARK */
                        // gu8_pairing_progress = PAIRING_PROGRESS_0_PERCENT;

                        if( gu8_Wifi_Last_Error != 0 )
                        {
                            bit_display_last_error = SET;
                            bit_wifi_pairing_start = SET;
                        }
                        else
                        {
                            bit_display_last_error = CLEAR;
                        }
                    }
                }
                else
                {
                    bit_wifi_pairing_start = CLEAR;
                }
            }
            else
            {
                // bit_wifi_pairing_start = CLEAR;
                finish_pairing_screen();
            }
        }
    break;

    case LONG_KEY_3S_WIFI_FAIRING:
        // 2025-09-04 CH.PARK [V1.0.0.5] �������� ���õ� ��� ����Ű�� �Ϲݻ��¿����� �����ϵ��� ���� (�÷��� ����)
        if(special_mode_check_U8() == SET)
        {
            return;
        }
        else {}

        /*..hui [21-7-15���� 12:51:01] WIFI �� - ����+�ü� 3��..*/
        /*if(F_All_Lock == CLEAR)*/
        if( F_All_Lock == SET )
        {

        }
        else
        {
            WifiKey(WIFI_KEY_AP);

            if( gu8_Led_Display_Step == LED_Display__MAIN )
            {
                if( gu8_wifi_ap_mode == CLEAR && gu8_ble_ap_mode == CLEAR )
                {
                        if( GetWifiStatusValue( WIFI_STATUS_DISP ) == WIFI_DISP_OFF )
                        {
                            /*gu8_Lcd_Display_Step = LCD_Display__WIFI_PAIRING;*/
                        }
                        else
                        {
                            // bit_wifi_pairing_start = SET;

                            // /* �� �ٽ� �õ��� ���� [0%] ���� �����ϵ��� �� 250714 CH.PARK */
                            // gu8_pairing_progress = PAIRING_PROGRESS_0_PERCENT;

                            if( gu8_Wifi_Last_Error != 0 )
                            {
                                bit_display_last_error = SET;
                                bit_wifi_pairing_start = SET;
                            }
                            else
                            {
                                bit_display_last_error = CLEAR;
                            }
                        }
                }
                else
                {
                    bit_wifi_pairing_start = CLEAR;
                }
            }
            else
            {
                bit_wifi_pairing_start = CLEAR;
            }
        }
        break;

        case KEY_SETTING_ALL_LOCK:
            key_setting_all_lock_off();
        break;

        case LONG_KEY_3S_MANAGER_INSTRUCTION_MODE:  /* ���� ������� */
            start_instruction_mode();
        break;

        /*..hui [24-12-3���� 1:51:11] �ð� ���� �̽���ġ ����..*/
        case LONG_KEY_TIME_SETTING:
            key_time_setting_job();
        break;

        default:

            NOP();

        break;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void Key_Very_Long_Input( U32 u32key_input_value)
{
    if( gu8_flushing_mode > FLUSHING_NONE_STATE )
    {
        return;
    }
    else{}

    if( gu8_fota_start == SET )
    {
        return;
    }
    else{}

    if( F_All_Lock == SET )
    {
        return;
    }
    else{}

    if(bit_filter_cover == CLEAR)
    {
       return;
    }

    if( F_WaterOut == SET || F_IceOut == SET )
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

    if(bit_ice_tank_ster_start == SET)
    {
        return;
    }

    if( bit_self_test_start == SET )
    {
        return;
    }
    else{}

    switch(u32key_input_value)
    {
        case KEY_COLD_SELECT:

            /*..hui [23-2-9���� 6:28:05] ���ø�忡���� �ȴ���..*/
            if( bit_setting_mode_start == CLEAR )
            {
                cold_water_enable_setting_job(SETTING_DISABLE);
            }
            else{}

        break;

        case KEY_ICE_SELECT:

            /*..hui [23-2-9���� 6:28:05] ���ø�忡���� �ȴ���..*/
            if( bit_setting_mode_start == CLEAR )
            {
                ice_on_off_setting_job(SETTING_DISABLE);
            }
            else{}

        break;

        case KEY_HOT_SELECT:
            //
            /*..hui [23-2-9���� 6:28:05] ���ø�忡���� �ȴ���..*/
            /*..hui [24-4-2���� 5:40:53] �¼� OFF�� �¼�+����+���� ����Ű�� ����..*/

        break;


        default:
            NOP();
        break;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 special_mode_check_U8(void)
{
    U8 gu8_return = 0;
    if(( bit_setting_mode_start == CLEAR )
    && ( bit_time_setting_start == CLEAR)
    && ( bit_memento_start == CLEAR )
    && ( bit_volume_setting_start == CLEAR )
    )
    {
        gu8_return = CLEAR;
    }
    else
    {
        gu8_return = SET;
    }

    return gu8_return;
}



