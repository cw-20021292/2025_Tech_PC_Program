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
#include    "key_input_water_extraction.h"

void key_input_extract(void);
void Display_Test_Mode_Extract_Key_Short_Input(void);
void key_input_water_extract(void);
void input_water_extract_key(void);
void extract_key_management(void);
void Extract_Key_Short_Input(void);
void water_extract_key(void);
void effluent_hz_decision(void);
void room_effluent_hz(void);
void cold_effluent_hz(void);
void ice_water_effluent_hz(void);
void hot_effluent_hz(void);
void Extract_Key_Long_Input(void);
void ice_extract_long_key(void);
void water_extract_long_key(void);
void Extract_Key_Very_Long_Input(void);
void Extract_No_Key(void);
void Uart_Test_Mode_Extract_Key_Short_Input(void);
/*************************************************************************************************************************/
/* �ÿ��� ���� ���õ� Ű ���� */
U8  gu8_Key_Water_Extract;
U8  u8Extract_Key_Buffer;
U8  u8Extract_Key_Input_Value;
bit F_Extract_Key_Short_Push_State;
bit F_Extract_Key_Long_Push_State;
bit F_Extract_Key_Very_Long_Push_State;
U16  u16Extract_Key_Sampling_Counter;
U16  u16Extract_Key_Long_Counter;
U16  u16Extract_Key_Short_Counter;
/*************************************************************************************************************************/
U8  gu8_Water_Select_Backup = CLEAR;
/*************************************************************************************************************************/
U8  u8Efluent_Time;
U8  u8Extract_Continue;
/*************************************************************************************************************************/
U16 u16Efluent_Time;
bit F_ColdConty;
/*************************************************************************************************************************/
U16 gu16_WaterQuantity_half;
U16 gu16_WaterQuantity_one;
U16 gu16_WaterQuantity_two;
U16 gu16_WaterQuantity_four;
/*************************************************************************************************************************/
extern bit bit_setting_mode_start;
extern FLUSHING_STEP gu8_flushing_mode;
extern FLUSHING_STEP gu8_flushing_mode_saved;
extern U8 gu8_flushing_okay_key_indicator;
extern U8 gu8_clear_block_error;
extern DISPLAY_TEST_STEP Disp_test_step;
extern U16 gu16_extract_display_hz;
extern U8 gu8_cup_led_select;
extern U8 gu8_cup_led_off_time;
extern U8 gu8_dripcoffee_percent;
extern U8 gu8_Icewater_level;
/*************************************************************************************************************************/
// [25-02-18 19:30:33] yspark, ���� �� �����÷ο� ��� OPEN �ð� ���� ����
extern U16 gu16Extracted_Hz;
extern bit bit_instruction_mode_start;
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void key_input_extract(void)
{
    if( gu8_fota_start == SET )
    {
        //play_melody_warning_197();
        return;
    }
    else{}

    key_input_water_extract();
}



/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void key_input_water_extract(void)
{
    input_water_extract_key();
    extract_key_management();
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void input_water_extract_key(void)
{
    if(gu8_Key_Water_Extract == u8Extract_Key_Buffer)
    {
        if(u8Extract_Key_Buffer == UKey_Mask)
        {
            u16Extract_Key_Sampling_Counter = 0;
            F_Extract_Key_Short_Push_State = CLEAR;
            F_Extract_Key_Long_Push_State = CLEAR;
            F_Extract_Key_Very_Long_Push_State = CLEAR;
        }
        else
        {
            u16Extract_Key_Sampling_Counter++;
        }

        if(u16Extract_Key_Sampling_Counter >= 1)
        {
            u16Extract_Key_Short_Counter = u16Extract_Key_Sampling_Counter;
            u16Extract_Key_Long_Counter++;

            /*..hui [19-11-19���� 4:26:26] Ű �����ִµ��� �¼� ���� ī���� �ʱ�ȭ..*/
            /*gu16_water_select_return_time = 0;*/
        }
        else{}
    }
    else
    {
        u8Extract_Key_Buffer = gu8_Key_Water_Extract;

        u16Extract_Key_Sampling_Counter = 0;
        u16Extract_Key_Long_Counter = 0;
        u16Extract_Key_Short_Counter = 0;
    }
}
/***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void extract_key_management(void)
{
    if(F_Extract_Key_Short_Push_State == CLEAR)
    {
        if((u16Extract_Key_Short_Counter >= 1)    /* �ּ� 100ms �̻� ������ �� */
        && (u16Extract_Key_Short_Counter < 2000)
        )
        {
            Extract_Key_Short_Input();
            F_Extract_Key_Short_Push_State = SET;
            u16Extract_Key_Short_Counter = 0;
        }
        else
        {
            /* ���⶧�� �������� �ȵ� */
            // if( F_IceOut == SET
            // && gu8_ice_out_continue == SET )
            // {
            //     ice_extraction_finish();
            // }
            // else{}
        }
    }
    else
    {
        if(F_Extract_Key_Long_Push_State == CLEAR)
        {
            if( u16Extract_Key_Long_Counter >= 2000 )
            {
                F_Extract_Key_Long_Push_State = SET;

                Extract_Key_Long_Input();
            }
            else{}
        }
        else
        {
            if(F_Extract_Key_Very_Long_Push_State == CLEAR)
            {
                if( u16Extract_Key_Long_Counter >= 8000 )
                {
                    F_Extract_Key_Very_Long_Push_State = SET;
                    u16Extract_Key_Long_Counter = 0;

                    Extract_Key_Very_Long_Input();
                }
                else{}
            }
            else{}
        }
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void Extract_Key_Short_Input(void)
{
    // if(gu8_filter_flushing_state == SET && gu8_flushing_mode == FLUSHING_STANDBY_STATE)
    // {
    //     gu8_flushing_okay_key_indicator = SET;
    //     return;
    // }

    gu16_water_select_return_time = 0;

    gu8_clear_block_error = SET;
    // gu8_clear_hot_block_error = SET;
    // bit_clock_start_time_init = SET;

    auto_drain_key_off_check();

    if(bit_sleep_mode_start == SET)
    {
        power_saving_init();
    }
    else {  }

    /*..hui [23-5-12���� 3:27:25] ��ư ���� ON, OFF ǥ���� ��ư ������ ǥ�� �ٷ� ����..*/
    stop_button_set_display();
    /*..hui [24-4-25���� 7:12:26] LED �������̴��� ����..*/
    /////off_all_flick();

    if(F_FW_Version_Display_Mode != SET)
    {
        return;
    }
    else{}

    if( bit_setting_mode_start == SET )
    {
        return;
    }
    else{}

    if(bit_time_setting_start == SET)
    {
        return;
    }

    /* ���»�� �߿��� ������� */
    if(bit_ice_tank_ster_start == SET)
    {
        play_voice_ice_tray_hot_ster_is_going_86();
        return;
    }
    else {  }

    if( bit_manual_drain_start == SET )
	{
		return;
	}
	else{}

    if(bit_volume_setting_start == SET)
    {
        return;
    }
    else {  }

    if( bit_self_test_start == SET )
{
        play_melody_warning_197();
        return;
    }
    else{}

    if(bit_memento_start == SET)
    {
        return;
    }

    /* ���λ�� �� ���� ��ư �Է� �� ��� �ߴ�
    Cody Water Line Clean Service */
    if(cody_water_line.gu8_start == SET)
    {
        cody_water_clean_mode_clear();
        return;
    }
    else {  }

    /* ����Ŀ�� ���� �� ����Ұ� */
    if(bit_filter_cover == CLEAR)
    {
        play_voice_filter_cover_opened_44();
        return;
    }

    if(bit_filter_all == CLEAR)
    {
        play_voice_filter_not_detected_14();
        return;
    }

    /*..hui [25-6-2���� 1:16:55] ���� ��ǰ ���� ���� �����ư ����..*/
    if( gu8_flushing_mode > FLUSHING_NONE_STATE && bit_instruction_mode_start == SET )
    {
        play_melody_warning_197();
        return;
    }
    else{}

	if( gu8_Led_Display_Step == LED_Display__FLUSHING )
	{
        if( gu8_flushing_mode == FLUSHING_STANDBY_STATE )
        {
            gu8_flushing_okay_key_indicator = SET;
        }
        else
        {
            if(gu8_filter_flushing_state == SET)
            {
                gu16_filter_flushing_timer_saved = gu16_filter_flushing_timer;
            }
            else {  }

            gu8_flushing_mode_saved = gu8_flushing_mode;
            gu8_flushing_mode = FLUSHING_STANDBY_STATE;
            bit_flushing_halt = SET;
            play_melody_setting_off_199();
        }

        return;
	}
	else {  }

    if( F_All_Lock == SET )
    {
        play_voice_all_lock_select_103();
        start_all_lock_flick();
        return;
    }

    water_extract_key();
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void water_extract_key(void)
{
    if(( F_WaterOut == CLEAR )
    && (F_IceOut == CLEAR)
    )
    {
        bit_welcome_start = SET;
    }
    else {  }

    bit_30_min_no_use_start = CLEAR;
    gu32_no_use_30_min_timer = CLEAR;

	/*..sean [25-02-03] E08 ���� �ȵǾ��־ �߰�..*/
	if( Bit3_Leakage_Sensor_Error__E01 == SET
    // || Bit1_Main_Water_Flow_Leaked_Error__E02 == SET
    // || Bit6_Main_Water_Flow_Block_Error__E09 == SET
    || (Bit0_Hot_Water_Flow_Block_Error__E08 == SET && u8WaterOutState == HOT_WATER_SELECT)
    )
    {
        /*..hui [21-7-23���� 7:12:37] ���� ���� �߻������� �����ϰ� ������ �층�층..*/
        play_melody_warning_197();
        gu8_error_popup_enable = SET;
        return;
    }
    else{}

    /*..sean [25-06-11] �ܼ� ���� �ɷ������� Clear�� ���ֵ��� ..*/
	if( Bit6_Main_Water_Flow_Block_Error__E09 == SET )
	{
		gu8_clear_block_error = SET;
		/*..sean [25-07-08] �¼� �ܼ� ���� �ɷ������� 30�� ���� �����ǵ��� ����..*/
    	//gu8_clear_hot_block_error = SET;
    	// �ü�,���� / �¼� �и�
		play_melody_select_196();
		return;
	}
	else {}

    /*..sean [25-07-08] E08 �и� ..*/
	if( Bit0_Hot_Water_Flow_Block_Error__E08 == SET )
	{
		gu8_clear_block_error = SET;
		/*..sean [25-07-08] �¼� �ܼ� ���� �ɷ������� 30�� ���� �����ǵ��� ����..*/
    	//gu8_clear_hot_block_error = SET;
		//play_melody_select_196();
		//return;
        if( u8WaterOutState == HOT_WATER_SELECT )
       	{
			play_melody_warning_197();
			return;
        }
		else
		{
			//Ȥ�� ������ �ɼ� �ֱ� ������ �߰�
			play_melody_select_196();
			//return;
		}
	}
	else {}

    if(F_Hot_Lock == SET)
    {
        if(bit_myWater_setting_start == SET
        && (my_setting[my_recipe_select].temp != 6 && my_setting[my_recipe_select].temp != 27))
        {
            // play_melody_warning_197();
            play_voice_hot_lcok_hot_select_93();
            return;
        }
    }

    /* ������ �������̸� ������Ű �ȴ��� 250625 CH.PARK */
    if((u8IceOutState == ICE_SELECT__ICE_WATER)
    && ((F_WaterOut == SET) || (F_IceOut == SET))
    )
    {
        play_melody_warning_197();
        return;
    }
    else {  }

    /* �� �� ����ڰ� � ��ư�� ������ �ٷ� ��ǥ�ô� Ǯ����. 250714 CH.PARK */
    if(( gu8_Led_Display_Step == LED_Display__WIFI_PAIRING )
    // && (gu8_Wifi_Pairing_State == )
    )
    {
        //bit_wifi_pairing_start = CLEAR;
        finish_pairing_screen();
    }
    else {  }

    /* �÷��� ��ŵ �� �¼� ����Ұ� 250611 CH.PARK */
    if(F_WaterOut == CLEAR)
    {
        if( F_First_Hot_Effluent == SET )
        {
            if(u8WaterOutState == HOT_WATER_SELECT)
            {
                play_melody_warning_197();
                return;
            }
            else {  }

            if(bit_myWater_setting_start == SET)
            {
                if((my_recipe_select == RECIPE_RAMEN)
                || (my_recipe_select == RECIPE_DRIPCOFFEE)
                || (my_recipe_select == RECIPE_TEA)
                )
                {
                    play_melody_warning_197();
                    return;
                }
                else {  }
            }
        }
    }
    else {  }

    if( F_WaterOut == CLEAR )
    {
        stop_cold_off_flick();
        init_water_extract_display();

        /* ���� �������� ���·� �� ����Ű�� ������ ��, ��/���� ������ ������ �� ������ ���� 250625 CH.PARK */
        if(u8IceOutState != ICE_SELECT__NONE)
        {
            // �¼� �������̸� �˾Ƽ� ���ƿ��� ��. �ǵ����� �׶� �� ��
            u8IceOutState = ICE_SELECT__NONE;
            play_melody_select_196();

            if(F_IceOut == SET)
            {
                Extract_Stack.U8_waterSelect = SET;
            }
            else
            {
                Extract_Stack.U8_iceSelect = CLEAR;
            }
            return;
        }
        else {  }

        F_WaterOut = SET;
        F_Effluent_OK = CLEAR;

        /*..hui [19-9-5���� 4:49:47] ���������� �޴�������..*/
        if( (gu8Cup_level == CUP_LEVEL_CONTINUE)
        && (bit_myWater_setting_start == CLEAR)
        )
        {
            u8Extract_Continue = SET;
            play_melody_continue_extract_start_192();
        }
        else
        {
            play_melody_normal_extract_start();
        }
    }
    else
    {
        F_WaterOut = CLEAR;
        u8Extract_Continue = CLEAR;
        gu16_extract_display_hz = 0;
        play_melody_extract_complete_194();
        Extract_Stack.U8_waterSelect = CLEAR;

        gu8_cup_led_select = SET;
        gu8_cup_led_off_time = 0;
    }

    if(F_WaterOut == SET)
    {
        if(bit_myWater_setting_start == SET)
        {
            if(my_recipe_select == MY_INDEX_DRIPCOFFEE)
            {
                /* �帳Ŀ�� �ٽ� ���� ������ �� ó������ �Ǵ��ϰ� (���� ����) 250722 CH.PARK */
                gu16Extracted_Hz = 0;
                drip_timer = 0;
                gu8_dripcoffee_percent = 0;
            }
            effluent_hz_decision();
        }
        else if(u8Extract_Continue != SET )
        {
            effluent_hz_decision();
        }
        else
        {
            /* �������� �ÿ��� ����ǥ�÷��� ���� ���� �ȸ´� ���� ���� 250718 CH.PARK */
            u16Efluent_Time = EXTRACT_TIME_CONTINUE;    // ���� ���� �� MAX �ð� ����
            effluent_hz_decision();
            gu16_total_extract_amount = 1990;
        }
    }
    else
    {
        u16Efluent_Time = 0;
        gu16Effluent_Hz = 0;
        gu16Extracted_Hz = 0;
    }

    gu8_clock_hide_state = SET;
    gu8_clock_hide_timer = 0;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void effluent_hz_decision(void)
{
    //LSH zzang
    if(bit_myWater_setting_start == SET)
    {
        if(my_recipe_select == RECIPE_RAMEN)
        {
            gu16Effluent_Hz = C_CUP_MY_RAMEN;
        }
        else if(my_recipe_select == RECIPE_DRIPCOFFEE)
        {
            gu16Effluent_Hz = C_CUP_MY_DRIPCOFFEE;
        }
        else if(my_recipe_select == RECIPE_TEA)
        {
            gu16Effluent_Hz = C_CUP_MY_TEA;
        }
        else if(my_recipe_select == MY_INDEX_MY1)
        {
            if(my_setting[MY_INDEX_MY1].temp == 6)
            {
                gu16Effluent_Hz = (U16)((C_CUP_MY_COLD) * (my_setting[MY_INDEX_MY1].amount));
            }
            else if(my_setting[MY_INDEX_MY1].temp == 27)
            {
                gu16Effluent_Hz = (U16)((C_CUP_MY_PURE) * (my_setting[MY_INDEX_MY1].amount));
            }
            else
            {
                gu16Effluent_Hz = (U16)((C_CUP_MY_HOT) * (my_setting[MY_INDEX_MY1].amount));
            }
        }
        else if(my_recipe_select == MY_INDEX_MY2)
        {
            if(my_setting[MY_INDEX_MY2].temp == 6)
            {
                gu16Effluent_Hz = (U16)((C_CUP_MY_COLD) * (my_setting[MY_INDEX_MY2].amount));
            }
            else if(my_setting[MY_INDEX_MY2].temp == 27)
            {
                gu16Effluent_Hz = (U16)((C_CUP_MY_PURE) * (my_setting[MY_INDEX_MY2].amount));
            }
            else
            {
                gu16Effluent_Hz = (U16)((C_CUP_MY_HOT) * (my_setting[MY_INDEX_MY2].amount));
            }
        }
        else if(my_recipe_select == MY_INDEX_MY3)
        {
            if(my_setting[MY_INDEX_MY3].temp == 6)
            {
                gu16Effluent_Hz = (U16)((C_CUP_MY_COLD) * (my_setting[MY_INDEX_MY3].amount));
            }
            else if(my_setting[MY_INDEX_MY3].temp == 27)
            {
                gu16Effluent_Hz = (U16)((C_CUP_MY_PURE) * (my_setting[MY_INDEX_MY3].amount));
            }
            else
            {
                gu16Effluent_Hz = (U16)((C_CUP_MY_HOT) * (my_setting[MY_INDEX_MY3].amount));
            }
        }
    }
    else
    {
        /* ������ */
        if(u8IceOutState == ICE_SELECT__ICE_WATER)
        {
            ice_water_effluent_hz();
            return;
        }
        else if(u8WaterOutState == PURE_WATER_SELECT)
        {
            room_effluent_hz();
        }
        else if(u8WaterOutState == COLD_WATER_SELECT)
        {
            cold_effluent_hz();
        }
        else if(u8WaterOutState == HOT_WATER_SELECT)
        {
            hot_effluent_hz();
        }
        else
        {
            gu16Effluent_Hz = 0;
        }
    }

    gu16_display_effnt_hz = gu16Effluent_Hz;

    if(bit_myWater_setting_start == SET)
    {
        gu16_total_extract_amount = my_setting[my_recipe_select].amount;
    }
    else
    {
        if( gu8Cup_level == CUP_LEVEL_1_120ML )
	    {
	        //gu16_total_extract_amount = 120;
            gu16_total_extract_amount = gu16_WaterQuantity_half;
	    }
	    else if( gu8Cup_level == CUP_LEVEL_2_250ML )
	    {
	        //gu16_total_extract_amount = 250;
            gu16_total_extract_amount = gu16_WaterQuantity_one;
	    }
	    else if( gu8Cup_level == CUP_LEVEL_3_500ML )
	    {
	        //gu16_total_extract_amount = 500;
            gu16_total_extract_amount = gu16_WaterQuantity_two;
	    }
			else if( gu8Cup_level == CUP_LEVEL_4_1000ML )
	    {
	        gu16_total_extract_amount = gu16_WaterQuantity_four;
	    }
			else if( gu8Cup_level == CUP_LEVEL_CONTINUE )
	    {
	        gu16_total_extract_amount = 1000;
	    }
	    else
	    {
	        gu16_total_extract_amount = 120;
            gu16_total_extract_amount = gu16_WaterQuantity_half;
	    }
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void room_effluent_hz(void)
{
    switch(gu8Cup_level)
    {

        case CUP_LEVEL_1_120ML:
            //gu16Effluent_Hz = C_CUP_120CC_ROOM;
            gu16Effluent_Hz = (U16)(gu16_WaterQuantity_half  * C_CUP_MY_PURE);
            break;

        case CUP_LEVEL_2_250ML:
            //gu16Effluent_Hz = C_CUP_250CC_ROOM;
            gu16Effluent_Hz = (U16)(gu16_WaterQuantity_one * C_CUP_MY_PURE);
            break;

        case CUP_LEVEL_3_500ML:
            //gu16Effluent_Hz = C_CUP_500CC_ROOM;
            gu16Effluent_Hz = (U16)(gu16_WaterQuantity_two * C_CUP_MY_PURE);
            break;

        case CUP_LEVEL_4_1000ML:
            //gu16Effluent_Hz = C_CUP_1000CC_ROOM;
            gu16Effluent_Hz = (U16)(gu16_WaterQuantity_four * C_CUP_MY_PURE);
        break;

        case CUP_LEVEL_CONTINUE:
            gu16Effluent_Hz = C_CUP_2000CC_ROOM;
        break;

        default:
            //gu16Effluent_Hz = C_CUP_120CC_ROOM;
            gu16Effluent_Hz = (U16)(gu16_WaterQuantity_half  * C_CUP_MY_PURE);
        break;
    }
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void cold_effluent_hz(void)
{
    switch(gu8Cup_level)
    {
        case CUP_LEVEL_1_120ML:
        gu16Effluent_Hz = (U16)(gu16_WaterQuantity_half * C_CUP_MY_COLD_120ML);
        break;

        case CUP_LEVEL_2_250ML:
        gu16Effluent_Hz = (U16)(gu16_WaterQuantity_one * C_CUP_MY_COLD);

        break;

        case CUP_LEVEL_3_500ML:
        gu16Effluent_Hz = (U16)(gu16_WaterQuantity_two * C_CUP_MY_COLD);
        break;

        case CUP_LEVEL_4_1000ML:
        gu16Effluent_Hz = (U16)(gu16_WaterQuantity_four * C_CUP_MY_COLD);
        break;

        case CUP_LEVEL_CONTINUE:
            gu16Effluent_Hz = C_CUP_2000CC_COLD;
        break;

        default:
        gu16Effluent_Hz = (U16)(gu16_WaterQuantity_half * C_CUP_MY_COLD);
        break;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void ice_water_effluent_hz(void)
{
    switch(gu8_Icewater_level)
    {
        case ICEWATER_CUP_LEVEL_1_150ML:
            gu16Effluent_Hz = (U16)((C_CUP_MY_COLD) * 150);
            break;

        case ICEWATER_CUP_LEVEL_2_250ML:
            gu16Effluent_Hz = (U16)((C_CUP_MY_COLD) * 250);
            break;

        case ICEWATER_CUP_LEVEL_3_350ML:
            gu16Effluent_Hz = (U16)((C_CUP_MY_COLD) * 350);
            break;

        case ICEWATER_CUP_LEVEL_4_500ML:
            gu16Effluent_Hz = (U16)((C_CUP_MY_COLD) * 500);
            break;

        default:
            gu16Effluent_Hz = (U16)((C_CUP_MY_COLD) * 150);
            break;
    }

    gu16_display_effnt_hz = gu16Effluent_Hz;

    if( gu8_Icewater_level == ICEWATER_CUP_LEVEL_1_150ML )
    {
        gu16_total_extract_amount = 150;
    }
    else if( gu8_Icewater_level == ICEWATER_CUP_LEVEL_2_250ML )
    {
        gu16_total_extract_amount = 250;
    }
    else if( gu8_Icewater_level == ICEWATER_CUP_LEVEL_3_350ML )
    {
        gu16_total_extract_amount = 350;
    }
		else if( gu8_Icewater_level == ICEWATER_CUP_LEVEL_4_500ML )
    {
        gu16_total_extract_amount = 500;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void hot_effluent_hz(void)
{
	// �׽�Ʈ �� ���� ����****
    #if 1
    switch(gu8Cup_level)
    {
        case CUP_LEVEL_1_120ML:
            gu16Effluent_Hz = (U16)(gu16_WaterQuantity_half  * C_CUP_MY_HOT);
        break;

        case CUP_LEVEL_2_250ML:
            gu16Effluent_Hz = (U16)(gu16_WaterQuantity_one  * C_CUP_MY_HOT);
        break;

        case CUP_LEVEL_3_500ML:
            gu16Effluent_Hz = (U16)(gu16_WaterQuantity_two  * C_CUP_MY_HOT);
        break;

        case CUP_LEVEL_4_1000ML:
            gu16Effluent_Hz = (U16)(gu16_WaterQuantity_four  * C_CUP_MY_HOT);
        break;

        case CUP_LEVEL_CONTINUE:
            gu16Effluent_Hz = C_CUP_2000CC_HOT;
        break;

        default:
            gu16Effluent_Hz = (U16)(gu16_WaterQuantity_half  * C_CUP_MY_HOT);
        break;
    }
    gu16_effluent_hot = gu16Effluent_Hz;	//2025-07-31 cbr
    #endif



}




/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void Extract_Key_Long_Input(void)
{
    if( F_All_Lock == SET )
    {
        play_voice_all_lock_select_103();
        start_all_lock_flick();
        return;
    }

    if(bit_myWater_setting_start == SET)
    {
        return;
    }

    if( u8IceOutState != ICE_SELECT__NONE )
    {
        ice_extract_long_key();
    }
    else
    {
        water_extract_long_key();
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void ice_extract_long_key(void)
{
    /* ����Ŀ�� ���� �� ����Ұ� */
    if(bit_filter_cover == CLEAR)
    {
        return;
    }

    if(bit_volume_setting_start == SET)
    {
        return;
    }
    else {  }

    if(bit_time_setting_start == SET)
    {
        return;
    }

    /* ���»�� �߿��� ������� */
    if(bit_ice_tank_ster_start == SET)
    {
        // play_voice_ice_tray_hot_ster_is_going_86();
        return;
    }
    else {  }

    if( bit_setting_mode_start == SET )
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

    if( bit_manual_drain_start == SET )
    {
        return;
    }

    if(bit_memento_start == SET)
    {
        return;
    }

    /*..hui [25-1-9���� 7:56:24] ���������϶��� �������� ����?..*/
    if( u8IceOutState == ICE_SELECT__ICE )
    {
        if( F_IceOut == SET )
        {
            gu8_ice_out_continue = SET;
            gu8_ice_feeder_shake = CLEAR;
            gu8_ice_feeder_shake_time = 0;
        }
        else
        {

        }
    }
    else{  }

    gu8_clock_hide_state = SET;
    gu8_clock_hide_timer = 0;
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void water_extract_long_key(void)
{
    gu16_water_select_return_time = 0;

    /*..hui [19-11-18���� 2:48:30] ���� �޴��� ���� ��..*/
    if(u8Extract_Continue == SET || bit_myWater_setting_start == SET)
    {
        return;
    }
    else{}

    /* ����Ŀ�� ���� �� ����Ұ� */
    if(bit_filter_cover == CLEAR)
    {
        return;
    }

    if(bit_volume_setting_start == SET)
    {
        return;
    }
    else {  }

    if( bit_self_test_start == SET )
    {
        play_melody_warning_197();
        return;
    }
    else {  }

    if(bit_memento_start == SET)
    {
        return;
    }

    /* ������ �������̸� ������ �����ư �Է� �ȵ� 250625 CH.PARK */
    if(u8IceOutState == ICE_SELECT__ICE_WATER)
    {
        return;
    }
    else {  }

    if( F_WaterOut == SET )
    {
        u16Efluent_Time = EXTRACT_TIME_CONTINUE;
        u8Extract_Continue = SET;

        /*..hui [19-12-5���� 4:48:26] ���� ��ư���� ���� ���������� ������ ���� ���ϴ°ɷ�..*/
        gu8Cup_level = CUP_LEVEL_CONTINUE;

        /* �������� �ÿ��� ����ǥ�÷��� ���� ���� �ȸ´� ���� ���� 250718 CH.PARK */
        effluent_hz_decision();
        gu16_total_extract_amount = 1990;
        // play_melody_continue_extract_start_192();
        /* ���� ���ش� �ȵ����� mini, 1kg�� ���� 250718 CH.PARK */
        Play_Voice(VOICE_192_MELODY_DISPENSE_CONTINUOUS);
    }
    else
    {
        u16Efluent_Time = 0;
    }

}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void Extract_Key_Very_Long_Input(void)
{
    if( F_All_Lock == SET )
    {
        play_voice_all_lock_select_103();
        start_all_lock_flick();
        return;
    }

    gu16_water_select_return_time = 0;

    /* ����Ű 8�� �̻� ������ �� ���� ���� */
    if( F_WaterOut == SET )
    {
        F_WaterOut = CLEAR;

        gu16Effluent_Hz = 0;
        u8Extract_Continue = CLEAR;
        gu16_extract_display_hz = 0;
        play_melody_extract_complete_194();
    }
    else{}

    gu8_clock_hide_state = SET;
	gu8_clock_hide_timer = 0;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void Extract_No_Key(void)
{
    F_Extract_Key_Short_Push_State = SET;
    F_Extract_Key_Long_Push_State = SET;
    F_Extract_Key_Very_Long_Push_State = SET;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/


