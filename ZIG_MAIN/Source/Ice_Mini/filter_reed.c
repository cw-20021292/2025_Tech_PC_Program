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
#include    "filter_reed.h"

void input_filter_reed_sw(void);



TYPE_BYTE          U8FilterResetStateB;
#define            u8FilterResetState                            U8FilterResetStateB.byte
#define            Bit0_Neo_Filter_1_Reset_State                 U8FilterResetStateB.Bit.b0
#define            Bit1_Ro_Filter_2_Reset_State                  U8FilterResetStateB.Bit.b1
#define            Bit2_Ino_Filter_3_Reset_State                 U8FilterResetStateB.Bit.b2

bit bit_filter_cover;
bit bit_filter_cover_open_to_close;
bit bit_filter_reed;		/* ���͸��� ����ġ bit �߰� */
bit bit_filter_reed_old;

U16 gu16_filter_reset_timer_sec;
U8 gu8_filter_reset_timer_min;
U16 gu16_reset_day_filter;

bit bit_neo_filter_1_reed;
bit bit_ro_filter_2_reed;
bit bit_ino_filter_3_reed;

bit bit_acid_reed;


U8 gu8_filter_cover_reed_on_decision_cnt;
U8 gu8_filter_cover_reed_off_decision_cnt;

U8 gu8_filter_reed_on_decision_cnt;
U8 gu8_filter_reed_off_decision_cnt;

U8 gu8_neo_reed_on_decision_cnt;
U8 gu8_neo_reed_off_decision_cnt;

U8 gu8_ro_reed_on_decision_cnt;
U8 gu8_ro_reed_off_decision_cnt;

U8 gu8_ino_reed_on_decision_cnt;
U8 gu8_ino_reed_off_decision_cnt;

U8 gu8_acid_reed_on_decision_cnt;
U8 gu8_acid_reed_off_decision_cnt;


bit bit_neo_filter_1_reed_old;
bit bit_ro_filter_2_reed_old;
bit bit_ino_filter_3_reed_old;

bit bit_acid_reed_old;





/*U16 gu16_filter_reset_day_neo;*/
/*U16 gu16_filter_reset_day_ro;*/


U16 gu16_reset_hour_neo_filter;
U16 gu16_reset_hour_ro_filter;
U16 gu16_reset_hour_ino_filter;


U16 gu16_neo_filter_reset_timer_sec;
U8 gu8_neo_filter_reset_timer_min;
U8 gu8_neo_filter_reset_timer_hour;


U16 gu16_ro_filter_reset_timer_sec;
U8 gu8_ro_filter_reset_timer_min;
U8 gu8_ro_filter_reset_timer_hour;

U16 gu16_ino_filter_reset_timer_sec;
U8 gu8_ino_filter_reset_timer_min;
U8 gu8_ino_filter_reset_timer_hour;


U16 gu16_display_filter_remain_day;
bit bit_filter_alarm_start;



bit bit_filter_alarm_1_3_voice;
bit bit_filter_alarm_1_2_3_voice;



bit bit_neo_filter_1_alarm;
bit bit_ro_filter_2_alarm;
bit bit_ino_filter_3_alarm;

U16 gu16_neo_filter_1_remain_day;
U16 gu16_ro_filter_2_remain_day;
U16 gu16_ino_filter_3_remain_day;


U8 gu8_filter_change_type;


U8 gu8_filter_alarm_popup_enable;


bit bit_yes_no_popup;			/* ���� ��ü �÷����� �������� ���� ������ �� SET */
bit bit_filter_all;

bit bit_filter_reset_yes;		/* ���� ��ü ���� �÷��� (����� ������ Ű) */
bit bit_filter_reset_no;		/* ���� ��ü ��� �÷��� (����� �ü� Ű) */

bit bit_wifi_neo_filter_1_reset;
bit bit_wifi_ro_filter_2_reset;
bit bit_wifi_ino_filter_3_reset;


U16 gu16_filter_change_reset_timer;

U16 gu16_1_3_remain_day_before;
U16 gu16_1_2_3_remain_day_before;

U8 gu8_filter_cover_reed_data;		/* ����Ŀ�� ���彺��ġ port �Է� ������ */

extern bit bit_filter_reed;
extern U8 gu8_front_rcv_filter_reed_data;

bit f_boot;

bit bit_filter_flushing_check;

void start_filter_flushing(void);
void reset_time_ino_filter(void);
void filter_reset(void);
void init_filter(void);
/******************************************************************************************************/
/**
 * @brief ���� ���� ���彺��ġ ����
 * 
 */
void input_filter_reed_sw(void)
{
    if( F_FW_Version_Display_Mode == CLEAR )
    {
        init_filter();
        return;
    }
    else{}

    if( bit_self_test_start == SET )
    {
        return;
    }
    else{}

	if(bit_filter_reed == SET)
	{
		return;
	}

	/* 2KG�� ���� ���彺��ġ�� ����Ʈ�� �ƴ� ���ο� ����Ǿ� ���� 250219 CH.PARK */
	gu8_front_rcv_filter_reed_data = pREED_FILTER;
	
	/* ���� ���彺��ġ [����] */
	if(gu8_front_rcv_filter_reed_data == SET)
	{
		gu8_filter_reed_off_decision_cnt = 0;
		gu8_filter_reed_on_decision_cnt++;

		if ( gu8_filter_reed_on_decision_cnt >= FILTER_REED_DETECT_TIME )
		{
			gu8_filter_reed_on_decision_cnt = FILTER_REED_DETECT_TIME;

			if( bit_filter_reed == SET )
			{			
				/*..hui [21-8-3���� 12:49:03] ����..*/
				bit_filter_reed = CLEAR;
				power_saving_init();
				play_voice_filter_reed_sw_open_4();

				/* �������̾��ٸ� ���� ��� ���� */
				if(F_WaterOut == SET)
				{
					F_WaterOut = CLEAR;
					u8Extract_Continue = CLEAR;
				}
				else {  }

				if(F_IceOut == SET)
				{
					ice_extraction_finish();

					// F_IceOut = CLEAR;
					// F_IceOutCCW = CLEAR;
					// F_IceBreak_Motor_Out = CLEAR;
				}
				else {  }

				/* ���͸��� �� ����Ŀ�� OPEN �� ���� ���� ���õ� ������ Ŭ���� 250730 CH.PARK */
				Extract_Stack.U8_iceSelect = CLEAR;
				Extract_Stack.U8_waterSelect = CLEAR;
			}
			else{}
		}
	}
	else
	{
		gu8_filter_reed_on_decision_cnt = 0;
		gu8_filter_reed_off_decision_cnt++;

		if( gu8_filter_reed_off_decision_cnt >= FILTER_REED_DETECT_TIME )
		{
			gu8_filter_reed_off_decision_cnt = FILTER_REED_DETECT_TIME;
		
			/* ���� ���彺��ġ [����] */
			if( bit_filter_reed == CLEAR )
			{
				bit_filter_reed = SET;

				play_melody_setting_on_198();
			}
			else{}
		}
	}
}

/******************************************************************************************************/
/**
 * @brief ����Ŀ�� ���� ����
 * 
 */
void	input_filter_cover_sw(void)
{
	if( F_FW_Version_Display_Mode == CLEAR )
    {
        init_filter();
        return;
    }
    else{}

    if( bit_self_test_start == SET )
    {
        return;
    }
    else{}
	if(bit_filter_cover == SET)
	{
		return;
	}

	/* 2KG�� ���� ���彺��ġ�� ����Ʈ�� �ƴ� ���ο� ����Ǿ� ���� 250219 CH.PARK */
	gu8_filter_cover_reed_data = pREED_FILTER_COVER;
	
	/* ���� ���彺��ġ [����] */
	if(gu8_filter_cover_reed_data == SET)
	{
		gu8_filter_cover_reed_off_decision_cnt = 0;
		gu8_filter_cover_reed_on_decision_cnt++;

		if ( gu8_filter_cover_reed_on_decision_cnt >= FILTER_REED_DETECT_TIME )
		{
			gu8_filter_cover_reed_on_decision_cnt = FILTER_REED_DETECT_TIME;

			if( bit_filter_cover == SET )
			{
				/*..hui [21-8-3���� 12:49:03] ����..*/
				bit_filter_cover = CLEAR;

				/* �������̾��ٸ� ���� ��� ���� */
				if(F_WaterOut == SET)
				{
					F_WaterOut = CLEAR;
					u8Extract_Continue = CLEAR;
				}
				else {  }

				if(F_IceOut == SET)
				{
					ice_extraction_finish();
					// F_IceOut = CLEAR;
					// F_IceOutCCW = CLEAR;
					// F_IceBreak_Motor_Out = CLEAR;
				}
				else {  }

				/* ���͸��� �� ����Ŀ�� OPEN �� ���� ���� ���õ� ������ Ŭ���� 250730 CH.PARK */
				Extract_Stack.U8_iceSelect = CLEAR;
				Extract_Stack.U8_waterSelect = CLEAR;

				play_voice_filter_cover_open_3();
			}
			else{}
		}
	}
	else
	{
		gu8_filter_cover_reed_on_decision_cnt = 0;
		gu8_filter_cover_reed_off_decision_cnt++;

		if( gu8_filter_cover_reed_off_decision_cnt >= FILTER_REED_DETECT_TIME )
		{
			gu8_filter_cover_reed_off_decision_cnt = FILTER_REED_DETECT_TIME;
		
			/* ���� ���彺��ġ [����] */
			if( bit_filter_cover == CLEAR )
			{
				bit_filter_cover = SET;

				/* ���͸��� ����ġ�� ���������� "���Ͱ� �ùٸ��� ���յ��� ���� ������ �����Ǿ����ϴ�..." */
				if(bit_filter_reed == CLEAR)
				{
					play_voice_filter_not_detected_14();
				}
				else
				{
					play_melody_setting_on_198();

					/*..hui [24-1-17���� 5:01:07] ���� ���� �÷��� ���� ������.. ���� ���� ���� ���..*/
					/*..hui [24-1-17���� 5:01:25] 1,3������ ���� �÷��� �߿� Ŀ�� ��� �ߴ��ϰ� 2�� ��ü ������ �Ǹ�..*/
					/*..hui [24-1-17���� 5:01:41] �ݾ����� ���� ������ �ȳ������� �÷��� �ð��� ro �÷��� 30������ �����Ѵ�..*/
					/* ���Ͱ� ����� ���� ���� ������ ���ͱ�ü ���õ� �˾��� ���� �Ѵ� 250515 CH.PARK */
					if( gu8_filter_flushing_state == FILTER_FLUSHING_NONE )
					{
						bit_filter_cover_open_to_close = SET;
					}
					else
					{

					}
				}
			}
			else{}
		}
	}
}

/**
 * @brief ���� OPEN/CLOSE ���� Ȯ�� �Լ�
 */
void input_filter_all(void)
{
	if((bit_filter_cover == CLEAR)
	|| (bit_filter_reed == CLEAR)
	)
	{
		bit_filter_all = CLEAR;
	}
	else
	{
		bit_filter_all = SET;
	}

	/* ���� ���õ� �� �ϳ��� ������ ��� �÷��� �Ͻ����� */
	if(bit_filter_all == CLEAR)
	{
		if(gu8_flushing_mode > FLUSHING_STANDBY_STATE)
		{
			gu8_flushing_mode_saved = gu8_flushing_mode;
			gu8_flushing_mode = FLUSHING_STANDBY_STATE;

			bit_flushing_halt = SET;
		}
		else {  }
	}
	else {  }
}

void filter_reset_timer__ino(void)
{
	/*..hui [23-12-19���� 3:13:15] ��ġ �÷����϶��� Ȯ������ �ʴ´�..*/
    if( bit_install_flushing_state == SET )
    {
        return;
    }
    else{}

	if( bit_filter_reed == CLEAR )
    {
        bit_filter_reed_old = SET;
    }
    else
    {
        if( bit_filter_reed_old == SET )
        {
			bit_filter_reed_old = CLEAR;
			Bit2_Ino_Filter_3_Reset_State = SET;

			if( u8FilterResetState == NEO_INO_FILTER_CHANGE )
			{
				// ���͸� ��ü�ߴٸ� �� ���� ��ư�� �����ּ���
				play_voice_1_3_filter_change_detect_18();
			}
			else{}
        }
        else{}
    }

    gu16_filter_reset_timer_sec++;
    if(gu16_filter_reset_timer_sec >= 600)
    {
        gu16_filter_reset_timer_sec = 0;
        gu8_neo_filter_reset_timer_min++;
    }
    else{}

	/* 1�ð����� ���ͱ�ü���� ī��Ʈ */
    if(gu8_filter_reset_timer_min >= 60)
    {
        gu8_filter_reset_timer_min = 0;
        gu16_reset_day_filter++;
    }
    else{}

	/* ���� ��ü�ֱ� : 456�� */
    if( gu16_reset_day_filter >= FILTER_RESET_456_DAY )
    {
        gu16_reset_day_filter = FILTER_RESET_456_DAY;
    }
    else{}
}

/**
 * @brief �����÷��� ������ ����
 */
void decesion_filter_flushing(void)
{
	if( bit_filter_cover == SET )
    {
        if( u8FilterResetState > 0 )
        {
            gu16_filter_change_reset_timer++;
            /*..hui [24-1-3���� 4:01:19] 15������ ����.. ���� �÷��� ��ɰ� �����ϰ� ������������..*/
            /*..hui [24-1-3���� 4:01:38] ������, ����ȣ�� ���� �� Ȯ��..*/
            if( gu16_filter_change_reset_timer >= FILTER_CHANGE_RESET_TIME )
            {
                gu16_filter_change_reset_timer = 0;
                u8FilterResetState = 0;
            }
            else{}
        }
        else
        {
            gu16_filter_change_reset_timer = 0;
        }
    }
    else
    {
        gu16_filter_change_reset_timer = 0;
    }

	if( bit_filter_cover_open_to_close == SET )
    {
        /*..hui [23-12-11���� 5:42:14] ����Ŀ�� OPEN->CLOSE������.. ���� ���彺��ġ�� ���� ������������..*/
		if( u8FilterResetState == NEO_INO_FILTER_CHANGE )
		{
			bit_filter_cover_open_to_close = CLEAR;
			start_filter_flushing();
			
			/* ���͸� ��ü�ϼ̳���? ��ü�ϼ̴ٸ� �� �����ư�� �����ּ���. .. 250515 CH.PARK */
			play_voice_1_3_filter_change_finish_19();
		}
		else
		{
			// /*..hui [23-12-6���� 2:22:33] 1,3�� ��ü�����ε� 1,2,3�� �������ǵ� �÷��� ����..*/
			// /*..hui [23-12-6���� 2:22:40] ��, ������ 1,3���� �Ѵ�..*/
			// start_filter_flushing();
		}
	}
}

/**
 * @brief �����÷��� ������ �Լ�
 */
void start_filter_flushing(void)
{
	gu8_flushing_mode = FLUSHING_STANDBY_STATE;
	
    /*..hui [23-6-14���� 6:44:36] ���� �÷��� �ѹ� �����ϸ� ��� �ȵǰ�..*/
    /*..hui [23-6-14���� 6:44:55] ���Ŀ� �ٽ� Ŀ�� ���ȴ� ������ yes no ǥ�� ����..*/
    /*if( gu8_filter_flushing_state == FILTER_FLUSHING_NONE )*/

    /*..hui [23-9-1���� 9:44:25] �����÷��� ��嵵 �ƴϰ�, ��ġ�÷��̵� �ƴҶ�..*/
    /*..hui [23-9-1���� 9:44:44] ��ġ�÷��� => ���̵�Ŀ�� ���� => ���̵�Ŀ�� ���� => ��� => ����ȭ������ ���ư�..*/
    if( (gu8_filter_flushing_state == FILTER_FLUSHING_NONE) 
	&& (bit_install_flushing_state == CLEAR)
	)
    {
        bit_yes_no_popup = SET;

        /*..hui [24-1-11���� 10:48:12] ���⼭�� �ʱ�ȭ.. ��ġ �÷��� ���� = 100% = �����÷��� �ٷ� �����ϸ� 100%�� ���۵�..*/
        gu8_display_flushing_total_percent = 0;
    }
    else {  }
}

/**
 * @brief ��ü�÷��� ���
 */
void cancel_filter_flushing(void)
{
    play_voice_filter_flushing_cancle_25();
    gu8_filter_flushing_state = FILTER_FLUSHING_NONE;
    gu8_flushing_mode = FLUSHING_NONE_STATE;

    if( bit_yes_no_popup == SET )
    {
        bit_yes_no_popup = CLEAR;
        bit_filter_reset_yes = CLEAR;
        bit_filter_reset_no = SET;
    }
    else{}
}

/**
 * @brief ���� ���� ������ �ʱ�ȭ �Լ�
 */
void init_filter(void)
{
	gu8_filter_cover_reed_on_decision_cnt = 0;
	gu8_filter_cover_reed_off_decision_cnt = 0;
	bit_filter_cover = SET;

	gu8_filter_reed_on_decision_cnt = 0;
	gu8_filter_reed_off_decision_cnt = 0;
	bit_filter_reed = SET;
}

/**
 * @brief ���� ��ü �� ��뷮 ���õ� ������ �ʱ�ȭ
 */
void filter_reset(void)
{
	if(bit_filter_reset_yes == SET)
	{
		bit_filter_reset_yes = CLEAR;
	}
	else if( bit_filter_reset_no == SET )
    {
        bit_filter_reset_no = CLEAR;
        u8FilterResetState = FILTER_NO_CHANGE;
        return;
    }
	else
	{
		return;
	}
	
	if( u8FilterResetState == NEO_INO_FILTER_CHANGE )
	{
		reset_time_ino_filter();		/* ���ͻ�뷮 �ʱ�ȭ */
		// send_wifi_system_function();
	}
	else
	{

	}
}

/**
 * @brief ���ͱ�ü �߻� �� ���õ� ������ �ʱ�ȭ
 */
void reset_time_ino_filter(void)
{
	if(Bit2_Ino_Filter_3_Reset_State == SET)
	{
		Bit2_Ino_Filter_3_Reset_State = CLEAR;

		gu16_filter_reset_timer_sec = 0;
		gu8_filter_reset_timer_min = 0;
		gu16_reset_day_filter = 0;

		gu16_water_usage_ino_filter = 0;

        gu16_temporary_save_usage = 0;

		bit_wifi_ino_filter_3_reset = SET;
	}
	else
	{

	}
}

/**
 * @brief �����Է� ���õ� �Լ� ����
 */
void input_filter(void)
{
	if( F_FW_Version_Display_Mode == CLEAR )
    {
        init_filter();
        return;
    }
    else{}

    if( bit_self_test_start == SET )
    {
        return;
    }
    else{}

	/*..hui [19-10-23���� 7:56:31] ��ũ Ŀ�� ���彺��ġ ..*/
    service_reed_sw_input();

    /*..hui [20-2-19���� 5:57:55] UV ���ܿ� ��ũ Ŀ�� ���彺��ġ ���� �и�..*/
    /*..hui [20-2-19���� 5:58:13] ��ũ Ŀ�� �������� ��� UV �����ϱ� ���� �и���..*/
    uv_tank_reed_sw_input();

    /*..hui [21-8-25���� 5:29:54] ���� ���� ���彺��ġ..*/
    input_filter_reed_sw();
    
    /* ����Ŀ�� ���彺��ġ ���� 250219 CH.PARK */
    input_filter_cover_sw();     // ����� �� ���� ���� �׶� �ּ� ����

    /* ���� ���� Ȯ�� 250421 CH.PARK */
    input_filter_all();

    /* ���ͱ�ü�ֱ� ��� */
    filter_reset_timer__ino();

    /* ���ͱ�ü Ȯ�� */
    decesion_filter_flushing();

	/* ���ͱ�ü �� ��뷮 ������ �ʱ�ȭ */
	filter_reset();

	// if(bit_filter_cover == CLEAR && bit_filter_reed == CLEAR)
	// {
	// 	bit_filter_flushing_check = SET;
	// }

	// if(bit_filter_flushing_check == SET)
	// {
	// 	if(bit_filter_cover == SET && bit_filter_reed == SET)
	// 	{
	// 		bit_filter_flushing_check = CLEAR;
	// 		gu8_filter_flushing_state = SET;
	// 		gu8_flushing_mode = FLUSHING_STANDBY_STATE;
	// 	}
	// }
}



