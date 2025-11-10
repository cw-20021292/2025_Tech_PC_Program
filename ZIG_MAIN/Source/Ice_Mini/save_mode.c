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
#include    "save_mode.h"

void save_mode(void);
void no_use_save_control(void);
void powersaving_learned_mode_decision(void);
void smart_no_use_timer(void);
void smart_one_week_timer(void);
void key_no_use_no_operation_for_test(void);
void power_saving_init(void);
void no_use_save_init(void);
void sleep_mode_decesion(void);
void wait_mode_decision(void);

bit bit_9_hour_no_use_start;

U16 gu16_no_use_timer_min;
U8 gu8_no_use_timer_hour;

U16 gu16_smart_unused_timer_min;
U8 gu8_smart_one_week_timer_hour;
U8 gu8_smart_unused_timer_hour;


bit bit_sleep_mode_start;
bit bit_display_sleep_start;
U16 gu16_display_sleep_timer;


bit bit_first_time_setting;


U16 gu16_current_time_total;
U16 gu16_start_time_total;
U16 gu16_finish_time_total;


bit bit_temporary_no_operation;

//U8 mu8_no_use_timer;
U8 gu8_smart_unused_count;
U8 gu8_week;
U8 gu8_smart_total_no_use_timer_hour;

bit f_one_week_judgement;
bit f_direct_mode_conversion;
bit f_normal_use_start;



POWER_SAVING_STATE gu8_smart_operation_mode;

bit no_use_break;

U8 gu8_wifi_smart_unused_setting_time;

bit bit_waitmode_enable;
bit bit_30_min_no_use_start;
U32 gu32_no_use_30_min_timer = CLEAR;

bit bit_sleep_voice_sync;
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void save_mode(void)
{
    // powersaving_learned_mode_decision();

    /*..hui [23-6-29오후 2:56:55] 미사용 절전..*/
    // no_use_save_control();

    /*..hui [23-6-29오후 2:57:07] 취침모드 설정..*/
    sleep_mode_decesion();

    /* 대기모드 */
    wait_mode_decision();
}



/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void no_use_save_control(void)
{
    U8 mu8_amb = 0;
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
    /*..hui [23-7-31오후 3:21:56] 냉동 요청.. 강제 미사용 절전 해제 기능..*/
    /*..hui [23-7-31오후 3:22:04] 전원 껐다 켜면 매번 설정해줘야 함..*/
    if( bit_temporary_no_operation == SET || F_FW_Version_Display_Mode == CLEAR )
    {
        gu16_no_use_timer_min = 0;
        gu8_no_use_timer_hour = 0;
        bit_9_hour_no_use_start = CLEAR;

        f_one_week_judgement = CLEAR;
        f_direct_mode_conversion = CLEAR;

        gu8_smart_unused_count = 0;
        gu8_smart_one_week_timer_hour = 0;
        gu8_smart_total_no_use_timer_hour = 0;

        gu16_smart_unused_timer_min = 0;
        gu8_smart_unused_timer_hour = 0;

        gu8_smart_operation_mode = GREEN_WEEK;
        return;
    }
    else{}

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
    /*..hui [23-8-31오후 3:35:38] 미사용 절전 9시간 타이머..*/
    gu16_no_use_timer_min++;

    /*..hui [23-8-30오후 3:27:06] 1시간 체크..*/
    if( gu16_no_use_timer_min >= 36000 )
    {
        gu16_no_use_timer_min = 0;
        gu8_no_use_timer_hour++;
    }
    else{}

    /*..hui [23-8-31오후 3:36:05] 9시간 경과 확인..*/
    if( gu8_no_use_timer_hour >= NO_USE_SAVE_MODE_TIME )
    {
        if( f_normal_use_start == SET )
        {
            bit_9_hour_no_use_start = CLEAR;
        }
        else
        {
            bit_9_hour_no_use_start = SET;
        }
    }
    else{}
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
    /*..hui [23-8-31오후 3:36:36] 스마트절전 관련 타이머..*/
    gu16_smart_unused_timer_min++;

    /*..hui [23-8-31오후 3:31:51] 스마트 절전 미사용 1시간 체크..*/
    if( gu16_smart_unused_timer_min >= 36000 )
    {
        gu16_smart_unused_timer_min = 0;

        smart_no_use_timer();
        gu8_smart_one_week_timer_hour++;
    }
    else{}

    smart_one_week_timer();
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
    mu8_amb = get_final_large_amb_temp();

    /*.. sean [23-8-24 오후 1:27:11 ] 외기에 의한 Sleep 모드 해제 조건 추가 ..*/
    if( (mu8_amb < 24) || (mu8_amb > 32 ) )
    {
        no_use_save_init();
    }
    else{}

}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void  powersaving_learned_mode_decision(void)
{
    // 1주일 지났는지 확인하여 모드 설정
    if( f_one_week_judgement == SET )
    {
        //GreenWeek 여부 확인
        /*..hui [23-9-4오전 11:14:46] 84시간 미사용, 144시간 누적 미사용 삭제.. 필차장님.. 상품기획팀 요청..*/
        if( gu8_smart_operation_mode == GREEN_WEEK )
        {
            if( gu8_smart_unused_count <= 2 )
            {
                gu8_smart_operation_mode = INTENSIVE_WEEK;
            }
            else
            {
                gu8_smart_operation_mode = GREEN_WEEK;
            }
        }
        else //if(gu8_smart_operation_mode[gu8_week] == INTENSIVE_WEEK)
        {
            if( gu8_smart_unused_count >= 4 )
            {
                gu8_smart_operation_mode = GREEN_WEEK;
            }
            else
            {
                gu8_smart_operation_mode = INTENSIVE_WEEK;
            }
        }

        f_one_week_judgement = CLEAR;
        f_direct_mode_conversion = CLEAR;

        /*..hui [23-8-31오전 10:51:14] 16시간 이상 미사용 횟수 카운트 초기화..*/
        gu8_smart_unused_count = 0;
        /*..hui [23-8-31오전 10:51:54] 16시간 이상 미사용 체크 타이머 초기화..*/
        gu16_smart_unused_timer_min = 0;
        gu8_smart_unused_timer_hour = 0;

        /*..hui [23-8-31오전 10:50:10] 일주일 경과 타이머 초기화..*/
        gu8_smart_one_week_timer_hour = 0;
        /*..hui [23-8-31오전 10:51:00] 16시간 이상 총 누적시간(144시간) 타이머 초기화..*/
        gu8_smart_total_no_use_timer_hour = 0;

        /*..hui [23-8-31오후 4:13:33] 일주일 지나면 무조건 초기화.. 양대리 - 팀장님 지시..*/
        no_use_save_init();
    }
    else
    {
        //Green Week에서 터치 들어올 시,
        if( no_use_break == SET )
        {
            if( gu8_smart_unused_timer_hour >= USE_SAVE_LEARNEND_MODE_TIME )
            {
                /*..hui [23-8-30오후 3:33:26] 16시간 이상 미사용 카운트 횟수 증가..*/
                gu8_smart_unused_count++;

                /*..hui [23-9-4오전 11:58:29] 한시간으로 설정했을때 최대 시간은 일주일..*/
                if( gu8_smart_unused_count >= ONE_WEEK_TIME_HOUR )
                {
                    gu8_smart_unused_count = ONE_WEEK_TIME_HOUR;
                }
                else{}
            }
            else{}

            gu16_smart_unused_timer_min = 0;
            gu8_smart_unused_timer_hour = 0;
            no_use_break = CLEAR;
        }
        else{}
    }

    if( gu8_smart_operation_mode == GREEN_WEEK )
    {
        f_normal_use_start = CLEAR;
    }
    else
    {
        f_normal_use_start = SET;
        /*..hui [23-8-31오후 3:18:27] 인텐시브위크면 9시간 초기화 및 미사용절전 해제..*/
        /*..hui [23-8-31오후 3:18:38] 이후 그린위크로 들어가면 그때부터 9시간 다시 카운팅함..*/
        no_use_save_init();
    }

}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void smart_no_use_timer(void)
{
    gu8_smart_unused_timer_hour++;

    /*..hui [23-8-30오후 3:46:44] 미사용 144시간 체크..*/
    if( gu8_smart_unused_timer_hour == USE_SAVE_LEARNEND_MODE_TIME )
    {
        /*..hui [23-8-30오후 3:28:49] 16시간되면 16더해줌..*/
        gu8_smart_total_no_use_timer_hour = gu8_smart_total_no_use_timer_hour + gu8_smart_unused_timer_hour;
    }
    else if( gu8_smart_unused_timer_hour > USE_SAVE_LEARNEND_MODE_TIME )
    {
        /*..hui [23-8-30오후 3:28:58] 17시간부터는 1씩 더해줌..*/
        gu8_smart_total_no_use_timer_hour = gu8_smart_total_no_use_timer_hour + 1;
    }
    else{}
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void smart_one_week_timer(void)
{
    /*..hui [23-8-31오전 10:56:04] 일주일 경과 체크..*/
    if( gu8_smart_one_week_timer_hour >= ONE_WEEK_TIME_HOUR )
    {
        gu8_smart_one_week_timer_hour = 0;
        f_one_week_judgement = SET;
    }
    else{}
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void key_no_use_no_operation_for_test(void)
{
    if( bit_temporary_no_operation == CLEAR )
    {
        bit_temporary_no_operation = SET;
        /*play_melody_setting_on_198();*/
        play_voice_test_mode_151();

        f_normal_use_start = SET;
    }
    else
    {
        bit_temporary_no_operation = CLEAR;
        play_melody_setting_off_199();

        f_normal_use_start = CLEAR;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void power_saving_init(void)
{
    /*..hui [23-6-29오후 2:54:25] 취침모드 디스플레이만 해제..*/
    bit_display_sleep_start = CLEAR;
    gu16_display_sleep_timer = 0;

    no_use_break = SET;

    gu8_wifi_no_use_key = SET;

    no_use_save_init();
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void no_use_save_init(void)
{
    gu16_no_use_timer_min = 0;
    gu8_no_use_timer_hour = 0;
    bit_9_hour_no_use_start = CLEAR;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void sleep_mode_decesion(void)
{
    if( bit_sleep_mode_enable == CLEAR )
    {
        bit_sleep_mode_start = CLEAR;
        bit_display_sleep_start = CLEAR;
        gu16_display_sleep_timer = 0;

        return;
    }
    else{}

    /*..hui [23-6-29오후 4:01:43] 시간, 분 분리되면 넘~~~~~~~~~~~~ 헷갈려서 통합해서....*/
    gu16_current_time_total = (U16)((U16)gu8_rtc_time_Hour * 100);
    gu16_current_time_total = gu16_current_time_total + gu8_rtc_time_Min;

    gu16_start_time_total = (U16)((U16)gu8_sleep_mode_start_hour * 100);
    gu16_start_time_total = gu16_start_time_total + gu8_sleep_mode_start_minute;

		//gu16_start_time_total = 1500;

    gu16_finish_time_total = (U16)((U16)gu8_sleep_mode_finish_hour * 100);
    gu16_finish_time_total = gu16_finish_time_total + gu8_sleep_mode_finish_minute;

    if( bit_sleep_mode_start == CLEAR )
    {
        if( gu16_start_time_total > gu16_finish_time_total )
        {
            if( gu16_current_time_total >= gu16_start_time_total || gu16_current_time_total < gu16_finish_time_total )
            {
                bit_sleep_mode_start = SET;
                /*..hui [23-6-29오후 2:51:39] 디스플레이 슬립 바로 시작..*/
                /*..hui [23-12-18오전 10:58:17] 메인 화면에서만 진입하도록 변경..*/
                /*..hui [23-12-18오전 10:58:39] 설정모드 진입 -> 에코모드 해제 -> 설정모드에서 바로 진입됨....*/
                /* 대기모드랑 취침모드랑 겹쳤을 떄는 취침모드가 우선순위로 동작하게 수정 250827 CH.PARK */
                if((bit_display_sleep_start != SET)
                && (gu8_Led_Display_Step == LED_Display__MAIN || bit_30_min_no_use_start == SET)
                )
                {
                    bit_display_sleep_start = SET;
                }
                else{}
            }
            else{}
        }
        else if( gu16_start_time_total < gu16_finish_time_total )
        {
            if( gu16_current_time_total >= gu16_start_time_total && gu16_current_time_total < gu16_finish_time_total )
            {
                bit_sleep_mode_start = SET;
                /*..hui [23-6-29오후 2:51:39] 디스플레이 슬립 바로 시작..*/
                /*..hui [23-12-18오전 10:58:17] 메인 화면에서만 진입하도록 변경..*/
                /*..hui [23-12-18오전 10:58:39] 설정모드 진입 -> 에코모드 해제 -> 설정모드에서 바로 진입됨....*/
                /* 대기모드랑 취침모드랑 겹쳤을 떄는 취침모드가 우선순위로 동작하게 수정 250827 CH.PARK */
                if((bit_display_sleep_start != SET)
                && (gu8_Led_Display_Step == LED_Display__MAIN || bit_30_min_no_use_start == SET)
                )
                {
                    bit_display_sleep_start = SET;
                }
                else{}
            }
            else{}
        }
        else{}
    }
    else
    {
        if( gu16_start_time_total > gu16_finish_time_total )
        {
            if( gu16_current_time_total < gu16_start_time_total && gu16_current_time_total >= gu16_finish_time_total )
            {
                if( bit_sleep_mode_start == SET )
                {
                    bit_sleep_mode_start = CLEAR;
                    /*..hui [23-6-29오후 2:55:49] 취침모드 들어갔다가 해제될때는 미사용 절전도 해제해야함..*/
                    //power_saving_init();
                    no_use_save_init();
                }
                else{}
            }
            else{}
        }
        else if( gu16_start_time_total < gu16_finish_time_total )
        {
            if( gu16_current_time_total < gu16_start_time_total || gu16_current_time_total >= gu16_finish_time_total )
            {
                if( bit_sleep_mode_start == SET )
                {
                    bit_sleep_mode_start = CLEAR;
                    /*..hui [23-6-29오후 2:55:49] 취침모드 들어갔다가 해제될때는 미사용 절전도 해제해야함..*/
                    //power_saving_init();
                    no_use_save_init();
                }
                else{}
            }
            else{}
        }
        else
        {
            /*..hui [23-10-5오후 1:58:25] 시작시간 종료시간 동일.. 하면 그냥 종료..*/
            if( bit_sleep_mode_start == SET )
            {
                bit_sleep_mode_start = CLEAR;
                /*..hui [23-6-29오후 2:55:49] 취침모드 들어갔다가 해제될때는 미사용 절전도 해제해야함..*/
                //no_use_save_init();
            }
            else{}
        }
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

    if( bit_sleep_mode_start == SET )
    {
        gu16_display_sleep_timer++;
        if( gu16_display_sleep_timer >= DISPLAY_SLEEP_TIME )
        {
            gu16_display_sleep_timer = DISPLAY_SLEEP_TIME;

            /*if( bit_display_sleep_start != SET )*/
            /*..hui [23-12-15오전 10:59:18] 메인화면에서만 진입..*/
            /* 대기모드랑 취침모드랑 겹쳤을 떄는 취침모드가 우선순위로 동작하게 수정 250827 CH.PARK */
            if((bit_display_sleep_start != SET)
            && (gu8_Led_Display_Step == LED_Display__MAIN || bit_30_min_no_use_start == SET)
            )
            {
                bit_display_sleep_start = SET;
            }
            else{}
        }
        else{}
    }
    else
    {
        if( bit_display_sleep_start == SET )
        {
            bit_display_sleep_start = CLEAR;
            /*..hui [23-6-29오후 2:55:49] 취침모드 들어갔다가 해제될때는 미사용 절전도 해제해야함..*/
            /*power_saving_init();*/
        }
        else{}

        gu16_display_sleep_timer = 0;
    }

}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void    wait_mode_decision(void)
{
    if( F_FW_Version_Display_Mode == CLEAR )
    {
        gu32_no_use_30_min_timer = CLEAR;
        bit_30_min_no_use_start = CLEAR;
        return;
    }
    else{}

    /* 대기모드 enable 안되어 있으면 안씀 */
    if(bit_waitmode_enable == CLEAR)
    {
        gu32_no_use_30_min_timer = CLEAR;
        bit_30_min_no_use_start = CLEAR;
    }
    else {  }

    if(bit_30_min_no_use_start == CLEAR)
    {
        gu32_no_use_30_min_timer++;
        if(gu32_no_use_30_min_timer >= 18000)      /* 30분 간 키 입력없으면 대기모드 진입 */
        {
            gu32_no_use_30_min_timer = 18000;
            bit_30_min_no_use_start = SET;
        }
        else {  }
    }
    else
    {
        gu32_no_use_30_min_timer = CLEAR;
    }
}


