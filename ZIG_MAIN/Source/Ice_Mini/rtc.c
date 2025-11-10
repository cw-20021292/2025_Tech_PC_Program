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
#include    "rtc.h"

void InitRtc(void);
static void EnableRtcCCR(void);
static void DisableRtcCCR(void);
U8 IsRtcBatBackUpMode(void);
void ReadRtcTime(void);
void WriteRtcTime(void);
void ProcessRtc(void);

U8 IsRTC_Error(void);
void InitRtcTime(void);


//U8 test_pcb_check_rtc_init(void);
U8 IsRTC_Reset(void);
U8 IsRTC_OSCF_Error(void);

void rtc_error_check(void);

U8 over_max_year(void);


void Delay_MS( U16 ms );
void Delay_US( U8 us );


U8 gu8_rtc_read_buf_1[8];
U8 gu8_rtc_read_buf_2[8];
U8 gu8_rtc_write_buf[8];

U8 gu8_rtc_time_Y2K;       // 19 or 20
U8 gu8_rtc_time_DayWeek;   // 0 ~ 6
U8 gu8_rtc_time_Year;      // 0 ~ 99
U8 gu8_rtc_time_Month;     // 1 ~ 12
U8 gu8_rtc_time_Date;      // 1 ~ 31
U8 gu8_rtc_time_Hour;      // 0 ~ 23
U8 gu8_rtc_time_Min;       // 0 ~ 59
U8 gu8_rtc_time_Sec;       // 0 ~ 59
U8 gu8_rtc_time_AmPm;



U8 gu8_rtc_init_success;


U16 gu16_rtc_error_cnt;
U16 gu16_rtc_error_timer;

bit bit_rtc_error_state;
bit bit_rtc_comm_error_state;


bit F_Rtc_Write;
U8 gu8_Rtc_Reg_Status;


U8 u8_test;
U8 gu8_uart_rtc_test_finish;
U8 gu8_uart_rtc_test_total_timer;

U8 gu8_rtc_garbage_timer;
U8 gu8_pba_rtc_test_finish;
bit bit_pba_rtc_error_state;
U8 u8_rtc_test_timer;


U8 gu8_rtc_time_Year_Before;
U8 gu8_rtc_time_Month_Before;
U8 gu8_rtc_time_Date_Before;
U8 gu8_rtc_time_Hour_Before;
U8 gu8_rtc_time_Min_Before;
U8 gu8_rtc_time_Sec_Before;

U8 gu8_rtc_stop_error_timer;
bit bit_rtc_stop_error_state;

U8 gu8_rtc_read_timer;

bit bit_rtc_time_read;

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void InitRtc(void)
{
    U16 mu16Delay = 30;
    U8 mu8_rtc_err = 0;

    // 만약 배터리 모드로 동작하는 중이면, 재시도...
    do
    {
        Delay_MS( 100 );
        mu16Delay--;
        if( mu16Delay == 0 )
        {
            break;
        }
        else{}

    } while( IsRtcBatBackUpMode() == TRUE );


    EnableRtcCCR();
    HAL_RTC_ByteWrite( DEV_ADDR_RTC, ADDR_CTR_PWR, 0x00 );
    DisableRtcCCR();

    EnableRtcCCR();
    HAL_RTC_ByteWrite( DEV_ADDR_RTC, ADDR_CTR_DTR, 0x03 );  // Digital Trimming
    DisableRtcCCR();

    EnableRtcCCR();
    HAL_RTC_ByteWrite( DEV_ADDR_RTC, ADDR_CTR_ATR, (0x20|0x10) );
    DisableRtcCCR();

    //EnableRtcCCR();
    //HAL_RTC_ByteWrite( DEV_ADDR_RTC, ADDR_CTR_INT, 0x18 );  // Output freq 32.768khz
    //DisableRtcCCR();

    EnableRtcCCR();
    HAL_RTC_ByteWrite( DEV_ADDR_RTC, ADDR_CTR_BL,  0x00 );
    DisableRtcCCR();

    // Read RTC Status
    HAL_RTC_ByteRead( DEV_ADDR_RTC, ADDR_RTC_STATUS, &gu8_Rtc_Reg_Status);

    mu8_rtc_err = IsRTC_Reset();

    if( mu8_rtc_err == SET )
    {
        /*..hui [20-1-10오후 7:37:44] 방전된상태..*/
        InitRtcTime();
    }
    else
    {
        ReadRtcTime();

        if(gu8_rtc_init_success == SET)
        {
            /*..hui [20-6-10오후 7:29:49] 시간이 설정 범위 이외일 경우 무조건 초기화하도록 개선..*/
            /*..hui [20-6-10오후 7:30:03] 라인에서 쓰레기값 들어감.. 가아끔..*/
            if( (gu8_rtc_time_Year >= SETTING_MIN_YEAR && gu8_rtc_time_Year <= SETTING_MAX_YEAR)
                && (gu8_rtc_time_Month >= 1 && gu8_rtc_time_Month <= 12)
                && (gu8_rtc_time_Date  >= 1 && gu8_rtc_time_Date  <= 31)
                && gu8_rtc_time_Hour  <= 23
                && gu8_rtc_time_Min   <= 59
                && gu8_rtc_time_Sec   <= 59 )
            {

            }
            else
            {
                InitRtcTime();
            }
        }
        else
        {
            /*..hui [20-6-16오후 10:38:53] 통신 실패할경우 값만 초기화..*/
            gu8_rtc_time_Year = RTC_DEFAULT_YEAR;
            gu8_rtc_time_Month = RTC_DEFAULT_MONTH;
            gu8_rtc_time_Date = RTC_DEFAULT_DATE;
            gu8_rtc_time_Hour = RTC_DEFAULT_HUOR;
            gu8_rtc_time_Min = RTC_DEFAULT_MIN;
            gu8_rtc_time_Sec = RTC_DEFAULT_SEC;
            gu8_rtc_time_DayWeek = RTC_DEFAULT_DAYWEEK;
            gu8_rtc_time_Y2K = RTC_DEFAULT_Y2K;
        }
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
static void EnableRtcCCR(void)
{
    HAL_RTC_ByteWrite( DEV_ADDR_RTC, ADDR_RTC_STATUS, 0x02 );
    HAL_RTC_ByteWrite( DEV_ADDR_RTC, ADDR_RTC_STATUS, 0x06 );
    Delay_MS(200);
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
static void DisableRtcCCR(void)
{
    HAL_RTC_ByteWrite( DEV_ADDR_RTC, ADDR_RTC_STATUS, 0x00 );
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 IsRTC_Reset(void)
{
    if( (gu8_Rtc_Reg_Status & (MASK_RTCF)) == 0 )
    {
        return CLEAR; // 정상
    }
    else{}

    return SET; //ERROR
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void ProcessRtc(void)
{
    U8 mu8_rtc_err = 0;
    U8 mu8_return = 0;
    U8 mu8_reset = 0;

    /*..hui [20-4-1오후 5:50:14] 시간 설정이랑 날짜 설정이 합쳐짐..*/
    /*..hui [20-4-1오후 5:50:18] 메뉴 하나에서 설정..*/
    /*..hui [20-4-27오전 11:20:11] LCD 쪽으로 이동..*/
    mu8_return = over_max_year();

    if(mu8_return == SET)
    {
        InitRtcTime();
        /*lcd_time_init();*/
        return;
    }
    else{}

    // Read Time from RTC
    gu8_rtc_read_timer++;

    if( gu8_rtc_read_timer >= RTC_READ_TIME || F_FW_Version_Display_Mode == CLEAR )
    {
        /*..hui [20-6-19오후 1:33:38] 20초마다 한번씩 읽도록 변경..*/
        gu8_rtc_read_timer = 0;

        // Read RTC Status
        HAL_RTC_ByteRead( DEV_ADDR_RTC, ADDR_RTC_STATUS, &gu8_Rtc_Reg_Status);

        /*..hui [20-6-11오전 9:38:33] 방전되거나 크리스탈 불량인 상태..*/
        mu8_rtc_err = IsRTC_Error();

        // RTC 상태
        if( mu8_rtc_err == SET )
        {
            /*..hui [20-1-10오후 7:37:44] 방전된상태..*/
            gu16_rtc_error_cnt++;
            /*..hui [20-2-18오전 10:00:22] 방전됐을때도 시간 초기화는 안하는걸로..*/
            /*..hui [20-2-18오전 10:00:36] 그냥 이전시간에서 이어서 시작되도록.. 그래야 설정이 편할테니..*/
            /*..hui [20-2-18오후 1:38:42] 리셋되면 RTC 자체에서 시간 초기화됨..*/
            /*..hui [20-4-17오후 8:41:41] 최대 100번까지만 제한..*/

            bit_rtc_error_state = SET;
        }
        else
        {
            /*..hui [20-4-17오후 8:42:42] rtc 에러 상태 클리어..*/
            gu16_rtc_error_timer = 0;
            bit_rtc_error_state = CLEAR;
        }

        ReadRtcTime();
        bit_rtc_time_read = SET;

        /*..hui [20-6-10오후 7:29:49] 시간이 설정 범위 이외일 경우 무조건 초기화하도록 개선..*/
        /*..hui [20-6-10오후 7:30:03] 라인에서 쓰레기값 들어감.. 가아끔..*/
        if( (gu8_rtc_time_Year >= SETTING_MIN_YEAR && gu8_rtc_time_Year <= SETTING_MAX_YEAR)
            && (gu8_rtc_time_Month >= 1 && gu8_rtc_time_Month <= 12)
            && (gu8_rtc_time_Date  >= 1 && gu8_rtc_time_Date  <= 31)
            && gu8_rtc_time_Hour  <= 23
            && gu8_rtc_time_Min   <= 59
            && gu8_rtc_time_Sec   <= 59 )
        {
            gu8_rtc_garbage_timer = 0;

            rtc_error_check();
        }
        else
        {
            if( bit_rtc_comm_error_state == CLEAR && bit_rtc_error_state == CLEAR )
            {
                gu8_rtc_garbage_timer++;
            }
            else
            {
                gu8_rtc_garbage_timer = 0;
            }

            /*..hui [20-6-11오전 9:46:21] 5회 연속 쓰레기값이 읽힐경우 초기화 추가..*/
            /*..hui [20-6-19오후 1:47:26] 3분동안 연속으로..*/
            if( gu8_rtc_garbage_timer >= 9 )
            {
                gu8_rtc_garbage_timer = 0;
                InitRtcTime();
            }
            else{}
        }
    }
    else{}
}



/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 over_max_year(void)
{
    U8 mu8_return = 0;

    if( gu8_rtc_time_Year > SETTING_MAX_YEAR
        && gu8_rtc_time_Month == 1
        && gu8_rtc_time_Date == 1 )
    {
        mu8_return = SET;
    }
    else{}

    return mu8_return;
}




/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 IsRtcBatBackUpMode(void)
{
    U8 mu8Ret = 0;
    U8 mu8Reg = 0;

    mu8Ret = HAL_RTC_ByteRead( DEV_ADDR_RTC, ADDR_RTC_STATUS, &mu8Reg );

    if( mu8Ret != TRUE )
    {
        return TRUE;    // Erro
    }
    else{}

    if( (mu8Reg & MASK_BAT) != 0 )
    {
        return TRUE;   // Battery BackUp Mode
    }
    else{}

    return FALSE;  // Vcc Mode
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void ReadRtcTime(void)
{
    U8 mu8Ret_1 = 0;
    U8 mu8Ret_2 = 0;
    U16 mu16_wait_cycle = 1000;

    // Read time buffer
    //mu8Ret_1 = HAL_RTC_SeqRead( DEV_ADDR_RTC, ADDR_RTC_SECONDS, &gu8_rtc_read_buf_1[0], 8 );

    mu8Ret_1 = HAL_RTC_SeqRead( DEV_ADDR_RTC, ADDR_RTC_SECONDS, gu8_rtc_read_buf_1, 8 );

    if( mu8Ret_1 == FALSE )
    {
        /*..hui [20-4-20오전 9:52:22] RTC 통신 에러..*/
        bit_rtc_comm_error_state = SET;
        return;
    }
    else{}

    ///////////////////////////wait
    /*..hui [20-3-13오후 1:36:39] 연속으로 읽기전에 일정시간 대기(약 60us).. 김종민 차장님 조언..*/
    while (mu16_wait_cycle--)
    {
        ;
    }

    // Read aging time buffer
    //mu8Ret_2 = HAL_RTC_SeqRead( DEV_ADDR_RTC, ADDR_RTC_SECONDS, &gu8_rtc_read_buf_2[0], 8 );

    mu8Ret_2 = HAL_RTC_SeqRead( DEV_ADDR_RTC, ADDR_RTC_SECONDS, gu8_rtc_read_buf_2, 8 );

    if( mu8Ret_2 == FALSE )
    {
        bit_rtc_comm_error_state = SET;
        return;
    }
    else{}

    // [0] 번지의 sec 값을 제외하고 나머지 시간이 동일하면 시간을 저장함.
    // read 과정에서 시간 값이 noise에 의해서 손상되는 것을 막기 위한 동작 사양임
    if( gu8_rtc_read_buf_1[1] == gu8_rtc_read_buf_2[1]
        && gu8_rtc_read_buf_1[2] == gu8_rtc_read_buf_2[2]
        && gu8_rtc_read_buf_1[3] == gu8_rtc_read_buf_2[3]
        && gu8_rtc_read_buf_1[4] == gu8_rtc_read_buf_2[4]
        && gu8_rtc_read_buf_1[5] == gu8_rtc_read_buf_2[5]
        && gu8_rtc_read_buf_1[6] == gu8_rtc_read_buf_2[6]
        && gu8_rtc_read_buf_1[7] == gu8_rtc_read_buf_2[7] )
    {
        gu8_rtc_time_Sec = Hex2Dec( gu8_rtc_read_buf_2[0] );
        gu8_rtc_time_Min = Hex2Dec( gu8_rtc_read_buf_2[1] );
        gu8_rtc_time_Hour = Hex2Dec( gu8_rtc_read_buf_2[2] & 0x3F );
        gu8_rtc_time_Date = Hex2Dec( gu8_rtc_read_buf_2[3] );
        gu8_rtc_time_Month = Hex2Dec( gu8_rtc_read_buf_2[4] );
        gu8_rtc_time_Year = Hex2Dec( gu8_rtc_read_buf_2[5] );
        gu8_rtc_time_DayWeek = Hex2Dec( gu8_rtc_read_buf_2[6] );
        gu8_rtc_time_Y2K = Hex2Dec( gu8_rtc_read_buf_2[7] );

        gu8_rtc_init_success = SET;
        bit_rtc_comm_error_state = CLEAR;
    }
    else{}
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void WriteRtcTime(void)
{
    I8 mLastedDate;
    U8 mu8Ret = 0;

    /*..hui [20-3-12오후 2:06:18] 0일때 필터링 추가..*/
    if( (gu8_rtc_time_Year >= SETTING_MIN_YEAR && gu8_rtc_time_Year <= SETTING_MAX_YEAR)
        && (gu8_rtc_time_Month >= 1 && gu8_rtc_time_Month <= 12)
        && (gu8_rtc_time_Date  >= 1 && gu8_rtc_time_Date  <= 31)
        && gu8_rtc_time_Hour  <= 23
        && gu8_rtc_time_Min   <= 59
        && gu8_rtc_time_Sec   <= 59 )
    {
        //
    }
    else
    {
        return;
    }


    gu8_rtc_write_buf[0] =  Dec2Hex(gu8_rtc_time_Sec);
    gu8_rtc_write_buf[1] =  Dec2Hex(gu8_rtc_time_Min);
    gu8_rtc_write_buf[2] =  Dec2Hex(gu8_rtc_time_Hour) | MASK_MIL;
    gu8_rtc_write_buf[3] =  Dec2Hex(gu8_rtc_time_Date);
    gu8_rtc_write_buf[4] =  Dec2Hex(gu8_rtc_time_Month);
    gu8_rtc_write_buf[5] =  Dec2Hex(gu8_rtc_time_Year);
    gu8_rtc_write_buf[6] =  Dec2Hex(gu8_rtc_time_DayWeek);
    gu8_rtc_write_buf[7] =  Dec2Hex(gu8_rtc_time_Y2K);

    EnableRtcCCR();
    HAL_RTC_PageWrite( DEV_ADDR_RTC, ADDR_RTC_SECONDS, gu8_rtc_write_buf, 8 );
    DisableRtcCCR();
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void InitRtcTime(void)
{
    I8 mLastedDate;
    U8 mu8Ret = 0;

    /*..hui [19-10-16오후 7:16:13] RTC 디폴트값 2019년 1월 1일 화요일 오전 1시 00분..*/
    /*..hui [19-11-5오후 5:56:07] RTC 디폴트값 2020년 1월 1일 수요일 오전 1시 00분..*/
    gu8_rtc_time_Year = RTC_DEFAULT_YEAR;
    gu8_rtc_time_Month = RTC_DEFAULT_MONTH;
    gu8_rtc_time_Date = RTC_DEFAULT_DATE;
    gu8_rtc_time_Hour = RTC_DEFAULT_HUOR;
    gu8_rtc_time_Min = RTC_DEFAULT_MIN;
    gu8_rtc_time_Sec = RTC_DEFAULT_SEC;
    gu8_rtc_time_DayWeek = RTC_DEFAULT_DAYWEEK;
    gu8_rtc_time_Y2K = RTC_DEFAULT_Y2K;

    gu8_rtc_write_buf[0] =  Dec2Hex(gu8_rtc_time_Sec);
    gu8_rtc_write_buf[1] =  Dec2Hex(gu8_rtc_time_Min);
    gu8_rtc_write_buf[2] =  Dec2Hex(gu8_rtc_time_Hour) | MASK_MIL;
    gu8_rtc_write_buf[3] =  Dec2Hex(gu8_rtc_time_Date);
    gu8_rtc_write_buf[4] =  Dec2Hex(gu8_rtc_time_Month);
    gu8_rtc_write_buf[5] =  Dec2Hex(gu8_rtc_time_Year);
    gu8_rtc_write_buf[6] =  Dec2Hex(gu8_rtc_time_DayWeek);
    gu8_rtc_write_buf[7] =  Dec2Hex(gu8_rtc_time_Y2K);

    EnableRtcCCR();
    HAL_RTC_PageWrite( DEV_ADDR_RTC, ADDR_RTC_SECONDS, gu8_rtc_write_buf, 8 );
    DisableRtcCCR();
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
/* RTC IC 오류 확인
 *
 * Return
 *  TRUE : RTC 오류!!
 *  FALSE : RTC 정상!!
 *
 */
U8 IsRTC_Error(void)
{
    if( (gu8_Rtc_Reg_Status & (MASK_OSCF|MASK_RTCF)) == 0 )
    {
        return CLEAR; // 정상
    }
    else{}

    return SET; //ERROR
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 IsRTC_OSCF_Error(void)
{
    if( (gu8_Rtc_Reg_Status & (MASK_OSCF)) == 0 )
    {
        return CLEAR; // 정상
    }
    else{}

    return SET; //ERROR
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void rtc_error_check(void)
{
    /*..hui [20-6-18오후 3:59:08] RTC 시간이 변경되지 않을 경우 LCD 시간으로 전환하기 위해 추가..*/
    if( gu8_rtc_time_Year == gu8_rtc_time_Year_Before
        && gu8_rtc_time_Month == gu8_rtc_time_Month_Before
        && gu8_rtc_time_Date == gu8_rtc_time_Date_Before
        && gu8_rtc_time_Hour == gu8_rtc_time_Hour_Before
        && gu8_rtc_time_Min == gu8_rtc_time_Min_Before
        && gu8_rtc_time_Sec == gu8_rtc_time_Sec_Before )
    {
        gu8_rtc_stop_error_timer++;

        /*..hui [20-6-19오후 1:38:00] 1분연속..*/
        /*..hui [20-6-19오후 1:46:23] 어차피 RTC 다시 읽었을때 정상값이 읽히면 그때부터 다시 LCD에 업뎃해주면 되므로..*/
        /*..hui [20-6-19오후 1:46:35] 그냥 한번만 동일하게 읽혀도 LCD 시간 업뎃은 안하는걸로..*/
        if( gu8_rtc_stop_error_timer >= 1 )
        {
            gu8_rtc_stop_error_timer = 1;
            bit_rtc_stop_error_state = SET;
        }
        else{}
    }
    else
    {
        gu8_rtc_time_Year_Before = gu8_rtc_time_Year;
        gu8_rtc_time_Month_Before = gu8_rtc_time_Month;
        gu8_rtc_time_Date_Before = gu8_rtc_time_Date;
        gu8_rtc_time_Hour_Before = gu8_rtc_time_Hour;
        gu8_rtc_time_Min_Before = gu8_rtc_time_Min;
        gu8_rtc_time_Sec_Before = gu8_rtc_time_Sec;

        gu8_rtc_stop_error_timer = 0;
        bit_rtc_stop_error_state = CLEAR;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void Delay_MS( U16 ms )
{
    U16 i;
    U8      j;

    /*..hui [19-10-21오후 1:32:34] 실측 약 86.8ms..*/
    for( i = 0 ; i < ms ; i++ )
    {
        for( j = 0; j < 4 ; j++ )   // 1ms loop delay
        {
            Delay_US( 250 );
            WDTE = (U8)0xAC;
        }
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void Delay_US( U8 us )
{
    while( us-- )
    {
        NOP();
        NOP();
        NOP();
        NOP();
    }

}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/



