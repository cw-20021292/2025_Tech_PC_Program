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
#include    "time_setting.h"


void time_setting(void);
void time_setting_up(void);
void time_setting_down(void);
void switch_time_setting(void);
void start_time_setting_mode( U8 mu8_mode );
void finish_time_setting_mode(void);
void change_rtc_time(void);
void change_reserve_time(void);
void check_continue_direction(void);
void time_setting_mode_timeout_check(void);
void break_time_setting_mode(void);
void reserve_time_setting_up(void);
void reserve_time_setting_down(void);
void time_setting_keep_up(void);
void time_setting_keep_down(void);

TYPE_BYTE          U8TimeSettingIndicatorLEDB;
#define            gu8_time_setting_indicator                  U8TimeSettingIndicatorLEDB.byte
#define            Bit0_Ice_Water_Minus_Indicator                    U8TimeSettingIndicatorLEDB.Bit.b0
#define            Bit1_Ice_Plus_Indicator                    U8TimeSettingIndicatorLEDB.Bit.b1
#define            Bit2_Settings_Switch_Indicator              U8TimeSettingIndicatorLEDB.Bit.b2
#define            Bit3_Settings_3S_Finish_Indicator           U8TimeSettingIndicatorLEDB.Bit.b3



TYPE_BYTE          U8TimeSettingLongKeyB;
#define            u8TimeSettingLongKey                       U8TimeSettingLongKeyB.byte
#define            Bit0_Ice_Water_Continue                    U8TimeSettingLongKeyB.Bit.b0
#define            Bit1_Ice_Continue                          U8TimeSettingLongKeyB.Bit.b1




U8 gu8_temporary_Year;      // 0 ~ 99
U8 gu8_temporary_Month;     // 1 ~ 12
U8 gu8_temporary_Date;      // 1 ~ 31
U8 gu8_temporary_Hour;      // 0 ~ 23
U8 gu8_temporary_Min;       // 0 ~ 59
U8 gu8_temporary_Sec;       // 0 ~ 59

U8 gu8_time_setting_step;
bit bit_time_setting_start;
bit bit_time_setting_finish;

bit bit_time_setting_fix;

U8 gu8_time_setting_finish_timer;


U8 gu8_time_setting_return_time;

U8 gu8_time_setting_mode;

U8 gu8_time_set_keeping_time;       // PCH JJANG
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void time_setting(void)
{
    time_setting_mode_timeout_check();

    if( bit_time_setting_start == CLEAR )
    {
        gu8_time_setting_indicator = 0;
        u8TimeSettingLongKey = 0;
        gu8_time_set_keeping_time = 0;  // PCH JJANG
        gu8_direction_continue_timer = 0;
        gu8_direction_continue_max = 0;
        return;
    }
    else{}

    /*..hui [23-8-1오후 3:44:49] 롱키 입력시 점점 빨라지게....*/
    // check_continue_direction();

    if( Bit0_Ice_Water_Minus_Indicator == SET )
    {
        Bit0_Ice_Water_Minus_Indicator = CLEAR;
        /*time_setting_up();*/
		time_setting_down();
    }
    else if(Bit0_Ice_Water_Continue == SET)
    {
        time_setting_keep_down();       // PCH JJANG
    }
    else{}

    if( Bit1_Ice_Plus_Indicator == SET )
    {
        Bit1_Ice_Plus_Indicator = CLEAR;
        time_setting_up();
    }
    else if(Bit1_Ice_Continue == SET)
    {
        time_setting_keep_up();     // PCH JJANG
    }
    else{}

    if( Bit2_Settings_Switch_Indicator == SET )
    {
        Bit2_Settings_Switch_Indicator = CLEAR;
        switch_time_setting();
    }
    else{}


    if( gu8_time_setting_step == SETTING_TIME_FINISH )
    {
        gu8_time_setting_finish_timer++;

        if( gu8_time_setting_finish_timer >= 10 )
        {
            gu8_time_setting_finish_timer = 0;
            finish_time_setting_mode();
        }
        else{}
    }
    else{}

	change_rtc_time();
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void time_setting_up(void)
{
    U8 mLastedDate = 0;

    switch( gu8_time_setting_step )
    {
        case SETTING_TIME_HOUR :

            if(gu8_temporary_Hour < 23)
            {
                gu8_temporary_Hour++;
            }
            else
            {
                gu8_temporary_Hour = 0;
            }

        break;

        case SETTING_TIME_MIN :

            if(gu8_temporary_Min < 59)
            {
                gu8_temporary_Min++;
            }
            else
            {
                /*..hui [19-10-16오후 5:30:44] 59분이면 0분으로..*/
                gu8_temporary_Min = 0;
            }

        break;


        default :

            gu8_time_setting_step = SETTING_TIME_HOUR;

        break;
    }

}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void time_setting_down(void)
{
    U8 mLastedDate = 0;

    switch( gu8_time_setting_step )
    {
        case SETTING_TIME_HOUR :

            if(gu8_temporary_Hour > 0)
            {
                gu8_temporary_Hour--;
            }
            else
            {
                gu8_temporary_Hour = 23;
            }

        break;

        case SETTING_TIME_MIN :

            if(gu8_temporary_Min > 0)
            {
                gu8_temporary_Min--;
            }
            else
            {
                /*..hui [19-10-16오후 5:21:51] 0분이면 59분으로..*/
                gu8_temporary_Min = 59;
            }

        break;


        default :

            gu8_time_setting_step = SETTING_TIME_HOUR;

        break;
    }

}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  : 장키입력 시 +6시 / +10분 // PCH JJANG
***********************************************************************************************************************/
void time_setting_keep_up(void)
{
    U8 mLastedDate = 0;
    gu8_time_set_keeping_time++;
    if(gu8_time_set_keeping_time >= 10)
    {
        gu8_time_set_keeping_time = 0;
        
        switch( gu8_time_setting_step )
        {
            case SETTING_TIME_HOUR :
                gu8_temporary_Hour = ((gu8_temporary_Hour + 6) % 24);

            break;

            case SETTING_TIME_MIN :
                gu8_temporary_Min += 10;
                if(gu8_temporary_Min >= 60) 
                {
                    gu8_temporary_Min = (gu8_temporary_Min % 60);
                }

            break;


            default :

                gu8_time_setting_step = SETTING_TIME_HOUR;

            break;
        }    
    }
    else {  }
    
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  : // PCH JJANG
***********************************************************************************************************************/
void time_setting_keep_down(void)
{
    U8 mLastedDate = 0;

    gu8_time_set_keeping_time++;
    if(gu8_time_set_keeping_time >= 10)
    {
        gu8_time_set_keeping_time = 0;
        switch( gu8_time_setting_step )
        {
            case SETTING_TIME_HOUR :
                
                if(gu8_temporary_Hour < 6)
                {
                    gu8_temporary_Hour = (24 - (6 - gu8_temporary_Hour));
                }
                else
                {
                    gu8_temporary_Hour -= 6;
                }

            break;

            case SETTING_TIME_MIN :

                if (gu8_temporary_Min < 10)
                {
                    gu8_temporary_Min = (60 - (10 - gu8_temporary_Min));
                }
                else
                {
                    gu8_temporary_Min -= 10;
                }
                
            break;


            default :

                gu8_time_setting_step = SETTING_TIME_HOUR;

            break;
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
void switch_time_setting(void)
{
    U8 mLastedDate = 0;

    if( gu8_time_setting_step == SETTING_TIME_HOUR )
    {
        gu8_time_setting_step = SETTING_TIME_MIN;
    }
    else if(gu8_time_setting_step == SETTING_TIME_MIN)
    {
        gu8_time_setting_step = SETTING_TIME_HOUR;
        // gu8_time_setting_step = SETTING_TIME_FINISH;
        ///finish_time_setting_mode();
        //bit_time_setting_fix = SET;
    }

    //play_melody_setting_on_198();
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void start_time_setting_mode( U8 mu8_mode )
{
    /* 와이파이 연결되어 있으면 시간설정 안됨 250630 CH.PARK */
    if( gu8_Wifi_Connect_State == WIFI_CONNECT )
    {
        play_melody_warning_197();
        return;
    }
    else{}

    gu8_time_setting_mode = mu8_mode;
    play_voice_time_setting_start_144();

    bit_time_setting_start = SET;
    gu8_time_setting_step = SETTING_TIME_HOUR;
	
    gu8_temporary_Year = gu8_rtc_time_Year;
    gu8_temporary_Month = gu8_rtc_time_Month;
    gu8_temporary_Date = gu8_rtc_time_Date;
    gu8_temporary_Hour = gu8_rtc_time_Hour;
    gu8_temporary_Min = gu8_rtc_time_Min;
    gu8_temporary_Sec = gu8_rtc_time_Sec;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void finish_time_setting_mode(void)
{
    /* 00:00일 때는 시계 기능 OFF */
    if(gu8_temporary_Hour == 0 && gu8_temporary_Min == 0)
    {
        bit_time_setting_finish = CLEAR;
        bit_first_time_setting = CLEAR;
    }
    else
    {
        bit_time_setting_finish = SET;
    }

    bit_time_setting_start = CLEAR;

    gu8_time_setting_step = SETTING_TIME_HOUR;
    gu8_hour_setting_blink_timer = 0;
    bit_hour_blink_500ms = CLEAR;
    gu8_min_setting_blink_timer = 0;
    bit_min_blink_500ms = CLEAR;

    play_voice_time_setting_finish_145();
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void change_rtc_time(void)
{
    U8 mu8_return = 0;

    if( bit_time_setting_finish == SET )
    {
        bit_time_setting_finish = CLEAR;

        /*..hui [23-6-29오후 2:42:14] 최초 시간 설정 완료..*/
        bit_first_time_setting = SET;

        /*..hui [20-3-12오후 2:10:12] 여기도 필터링 추가.. 안전사양..*/
        if( ( gu8_temporary_Year >= SETTING_MIN_YEAR && gu8_temporary_Year <= SETTING_MAX_YEAR )
            && ( gu8_temporary_Month >= 1 && gu8_temporary_Month <= 12 )
            && ( gu8_temporary_Date  >= 1 && gu8_temporary_Date  <= 31 )
            && ( gu8_temporary_Hour  <= 23 && gu8_temporary_Min   <= 59 && gu8_temporary_Sec   <= 59 ) )
        {
            gu8_rtc_time_Year = gu8_temporary_Year;
            gu8_rtc_time_Month = gu8_temporary_Month;
            gu8_rtc_time_Date = gu8_temporary_Date;
            gu8_rtc_time_Hour = gu8_temporary_Hour;
            gu8_rtc_time_Min = gu8_temporary_Min;
            gu8_rtc_time_Sec = gu8_temporary_Sec;

            gu8_rtc_time_Y2K = RTC_DEFAULT_Y2K;

            WriteRtcTime();
        }
        else{}
    }
    else{}
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void change_reserve_time(void)
{
    U8 mu8_return = 0;

    if( bit_time_setting_finish == SET )
    {
        bit_time_setting_finish = CLEAR;

        if( gu8_temporary_Hour  <= 23 && gu8_temporary_Min   <= 59 )
        {
    		/*..sean [24-10-04] wifi 아직 미적용으로 주석처리  .. */
            //gu8_wifi_period_ster_hour = gu8_temporary_Hour;
            //gu8_wifi_period_ster_minute = gu8_temporary_Min;
        }
        else{}
    }
    else{}
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void check_continue_direction(void)
{
    U8 mu8_speed = 10;

    if( gu8_direction_continue_max >= 60 )      // 1분
    {
        gu8_direction_continue_max = 0;
        u8TimeSettingLongKey = 0;
        gu8_time_set_keeping_time = 0;
    }
    else {  }

    switch( u8TimeSettingLongKey )
    {
        case ICE_WATER_KEY_CONTINUE:

            gu8_direction_continue_timer++;

            if(gu8_direction_continue_timer >= mu8_speed)
            {
                gu8_direction_continue_timer = 0;
                Bit0_Ice_Water_Continue = SET;

                gu8_direction_continue_max++;
            }
            else{}

            gu8_time_setting_return_time = 0;

        break;

        case ICE_KEY_CONTINUE:

            gu8_direction_continue_timer++;

            if(gu8_direction_continue_timer >= mu8_speed)
            {
                gu8_direction_continue_timer = 0;
                Bit1_Ice_Continue = SET;

                gu8_direction_continue_max++;
            }
            else{}

            gu8_time_setting_return_time = 0;

        break;

        default:

            u8TimeSettingLongKey = 0;
            gu8_direction_continue_timer = 0;
            gu8_direction_continue_max = 0;

        break;
    }
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void time_setting_mode_timeout_check(void)
{
    if( bit_time_setting_start == SET )
    {
        /*..hui [23-8-2오전 9:45:42] 필터 열림 감지시 강제 종료..*/
        /*..hui [25-3-19오후 6:49:36] 나중에다시..*/
        if( bit_filter_all == CLEAR )
        {
            break_time_setting_mode();
        }
        else{}
        
    	/*..sean [24-10-04] rtc 아직 미적용으로 주석처리  .. */
#if 1
        if( gu8_Wifi_Connect_State == WIFI_CONNECT )
        {
            break_time_setting_mode();
            play_melody_warning_197();
            return;
        }
        else{}
#endif
        gu8_time_setting_return_time++;		
        if( gu8_time_setting_return_time >= 200 )
        {
            // 20초 타임아웃 시 변경값 적용안함
            gu8_time_setting_return_time = 0;
            break_time_setting_mode();
            play_melody_warning_197();
        }
        else{}
    }
    else
    {
        gu8_time_setting_return_time = 0;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void break_time_setting_mode(void)
{
    bit_time_setting_start = CLEAR;
    /*..hui [23-8-2오전 9:34:48] 값 적용 안함..*/
    bit_time_setting_finish = CLEAR;

    gu8_time_setting_step = SETTING_TIME_HOUR;
    gu8_hour_setting_blink_timer = 0;
    bit_hour_blink_500ms = CLEAR;
    gu8_min_setting_blink_timer = 0;
    bit_min_blink_500ms = CLEAR;
    gu8_time_setting_return_time = 0;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void reserve_time_setting_up(void)
{
    U8 mLastedDate = 0;

    switch( gu8_time_setting_step )
    {
        case SETTING_TIME_HOUR :

            if(gu8_temporary_Hour < 12)
            {
                gu8_temporary_Hour++;
            }
            else
            {
                gu8_temporary_Hour = 1;
            }

        break;

        case SETTING_TIME_MIN :

            /*..hui [24-3-25오후 12:57:49] 세척 예약 시간도 1분단위로 설정 가능하도록..*/
            /*..hui [24-3-25오후 12:57:55] 상품기획팀, PM 요청..*/
            if(gu8_temporary_Min < 59)
            {
                gu8_temporary_Min++;
            }
            else
            {
                /*..hui [19-10-16오후 5:30:44] 59분이면 0분으로..*/
                gu8_temporary_Min = 0;
            }

        break;


        default :

            gu8_time_setting_step = SETTING_TIME_HOUR;

        break;
    }
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void reserve_time_setting_down(void)
{
    U8 mLastedDate = 0;

    switch( gu8_time_setting_step )
    {
        case SETTING_TIME_HOUR :

            if(gu8_temporary_Hour > 0)
            {
                gu8_temporary_Hour--;
            }
            else
            {
                gu8_temporary_Hour = 23;
            }

        break;

        case SETTING_TIME_MIN :


            /*..hui [24-3-25오후 12:57:49] 세척 예약 시간도 1분단위로 설정 가능하도록..*/
            /*..hui [24-3-25오후 12:57:55] 상품기획팀, PM 요청..*/
            if(gu8_temporary_Min > 0)
            {
                gu8_temporary_Min--;
            }
            else
            {
                /*..hui [19-10-16오후 5:21:51] 0분이면 59분으로..*/
                gu8_temporary_Min = 59;
            }

        break;


        default :

            gu8_time_setting_step = SETTING_TIME_HOUR;

        break;
    }

}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/






