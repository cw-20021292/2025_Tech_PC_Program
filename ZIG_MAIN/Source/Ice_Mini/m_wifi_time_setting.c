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
#include    "m_wifi_time_setting.h"


void wifi_time_setting(void);
void get_wifi_time ( U16 mYear, U16 mMonth, U16 mDate, U16 mHour, U16 mMin, U16 mSec );
void write_rtc_wifi_time(void);




TYPE_BYTE          U8RequestWifiTimeB;
#define            u8RequestWifiTime                           U8RequestWifiTimeB.byte
#define            Bit0_Wifi_Init_State                        U8RequestWifiTimeB.Bit.b0
#define            Bit1_Wifi_Time_Periodic_Read_State          U8RequestWifiTimeB.Bit.b1


U16 gu16_info_request_timer_min;
U16 gu16_info_request_timer_hour;

U8 gu8_wifi_time_Year;      // 0 ~ 99
U8 gu8_wifi_time_Month;     // 1 ~ 12
U8 gu8_wifi_time_Date;      // 1 ~ 31

U8 gu8_wifi_time_Hour;
U8 gu8_wifi_time_Min;
U8 gu8_wifi_time_Sec;

bit F_Wifi_Time_Read_Finish;
bit bit_wifi_time_first_read;

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void wifi_time_setting(void)
{
    U8 mu8_connect_state = 0;
    U8 mu8_data = 0;
    S8 ms8_temp = 0;
    U16 mu16_req_time = 0;


    /*..hui [21-8-23오후 4:50:56] 현민이쪽에서 시간을 불러와도 써주는건 넣어야...........*/
    write_rtc_wifi_time();
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void get_wifi_time ( U16 mYear, U16 mMonth, U16 mDate, U16 mHour, U16 mMin, U16 mSec )
{
    U8 mu8_return = 0;

    if( ( mYear >= 2023 && mYear <= 2080 )
        && ( mMonth >= 1 && mMonth <= 12 )
        && ( mDate  >= 1 && mDate  <= 31 )
        && ( mHour  <= 23 )
        && ( mMin  <= 59 )
        && ( mSec  <= 59 ) )
    {
        gu8_wifi_time_Year = (U8)(mYear % 100);
        gu8_wifi_time_Month = (U8)mMonth;
        gu8_wifi_time_Date = (U8)mDate;

        gu8_wifi_time_Hour = (U8)mHour;
        gu8_wifi_time_Min = (U8)mMin;
        gu8_wifi_time_Sec = (U8)mSec;

        F_Wifi_Time_Read_Finish = SET;
        bit_wifi_time_first_read = SET;
    }
    else{}
}



/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void write_rtc_wifi_time(void)
{
    U8 mu8_return = 0;

    if(F_Wifi_Time_Read_Finish == SET)
    {
        F_Wifi_Time_Read_Finish = CLEAR;

        /*..hui [23-6-29오후 2:43:45] WIFI 현재시간 전송 완료..*/
        bit_first_time_setting = SET;

        /*..hui [20-3-12오후 2:10:12] 여기도 필터링 추가.. 안전사양..*/
        if( ( gu8_wifi_time_Year >= SETTING_MIN_YEAR && gu8_wifi_time_Year <= SETTING_MAX_YEAR )
            && ( gu8_wifi_time_Month >= 1 && gu8_wifi_time_Month <= 12 )
            && ( gu8_wifi_time_Date  >= 1 && gu8_wifi_time_Date  <= 31 )
            && ( gu8_wifi_time_Hour  <= 23 )
            && ( gu8_wifi_time_Min  <= 59 )
            && ( gu8_wifi_time_Sec  <= 59 ) )
        {
            gu8_rtc_time_Year = gu8_wifi_time_Year;
            gu8_rtc_time_Month = gu8_wifi_time_Month;
            gu8_rtc_time_Date = gu8_wifi_time_Date;
            gu8_rtc_time_Hour = gu8_wifi_time_Hour;
            gu8_rtc_time_Min = gu8_wifi_time_Min;

            if( gu8_wifi_time_Sec >= 2 )
            {
                /*..hui [21-3-9오후 12:56:28] 1초 보정..*/
                gu8_rtc_time_Sec = gu8_wifi_time_Sec - 2;
            }
            else
            {
                gu8_rtc_time_Sec = gu8_wifi_time_Sec;
            }

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





