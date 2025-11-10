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
#include    "uv_ice_tray.h"



void output_ice_tray_uv(void);
U8 uv_ice_tray_control(void);
void ice_tray_uv_operation_timer(void);
U8 ice_tray_uv_standby(void);
void start_ice_tray_uv(void);
void finish_ice_tray_uv(void);
void initial_ice_tray_uv_timer(void);
void uv_ice_tray_24_hour_timer(void);
U8 ice_tray_uv_making(void);

/**
 * @brief 얼음트레이 UV동작 함수
 */
void uv_ice_tray_decision(void);


/***********************************************************************************************************************/
TYPE_BYTE          U8IceTrayUvONB;
#define            u8IceTrayUvON                                          U8IceTrayUvONB.byte
#define            Bit0_Ice_Tray_1_2_UV_On_State                          U8IceTrayUvONB.Bit.b0
#define            Bit1_Ice_Tray_1_2_UV_Ice_Tray_Clean_On_State           U8IceTrayUvONB.Bit.b1

TYPE_BYTE          U8IceTrayUvOFFB;
#define            u8IceTrayUvOFF                                         U8IceTrayUvOFFB.byte
#define            Bit0_Ice_Tray_UV_Tank_Open_Off_State                   U8IceTrayUvOFFB.Bit.b0        /* 탱크커버가 열렸을 때 */
#define            Bit1_Ice_Tray_UV_Ice_Off_State                         U8IceTrayUvOFFB.Bit.b1        /* 얼음 OFF 시 */
#define            Bit2_Ice_Tray_UV_Ice_Extract_State                     U8IceTrayUvOFFB.Bit.b2        /* 얼음 추출 시 */
#define            Bit3_Ice_Tray_UV_Retry_3S_Off_State                    U8IceTrayUvOFFB.Bit.b3        /* 에러상태 RETRY */
/***********************************************************************************************************************/
bit bit_ice_tray_1_2_uv_start;

U16 gu16_ice_tray_1_2_uv_operation_timer_sec;
U8 gu8_ice_tray_1_2_uv_operation_timer_min;
U16 gu16_ice_tray_1_2_uv_operation_timer_hour;

U16 gu16_ice_tray_1_2_uv_standby_timer_sec;
U16 gu16_ice_tray_1_2_uv_standby_timer_min;
U16 gu16_ice_tray_1_2_uv_standby_timer_hour;

U16 gu16_ice_tray_uv_24_hour_timer_sec;
U16 gu16_ice_tray_uv_24_hour_timer_min;
U16 gu16_ice_tray_uv_24_hour_timer_hour;

U8 gu8_uv_ice_tray_step;

bit bit_uv_ice_tray_out;

bit bit_uv_ice_full_state;

bit bit_ice_tray_uv_op_1;
bit bit_ice_tray_uv_op_2;

U8 gu8_tray_uv_throw_count_one_day;
U8 gu8_uv_ice_tray_retry_timer;


U32 gu32_ice_tray_up_max_timer;
U8 gu8_tray_uv_making_count_one_day;
bit bit_ice_tray_before;
bit bit_ice_tray_making_enable;

U16 gu16_uv_ice_make_time;

U32 gu32_UV_Tray_Interval_Timer_100ms = CLEAR;
U16 gu16_UV_Tray_On_Timer_100ms = CLEAR;

U8 gu8_ice_tray_uv_step;
U32 gu32_ice_tray_uv_step_timer_100ms;
/***********************************************************************************************************************/
extern U16 gu16_Ice_Door_StepMotor;
extern UV_Check uvIceTray;
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void output_ice_tray_uv(void)
{
    if( bit_self_test_start == SET )
    {
        return;
    }
    else{}

    /* 얼음트레이 : 하루 최대 만빙 후 3회 / 제빙중 7회 → 1시간 ON */
    Bit1_Ice_Tray_1_2_UV_Ice_Tray_Clean_On_State = uv_ice_tray_control();

    /* 탱크커버가 하나라도 열렸으면 바로 트레이 UV는 OFF. 250224 CH.PARK */
    Bit0_Ice_Tray_UV_Tank_Open_Off_State = (~bit_uv_tank_input);
    
    /* 얼음 OFF상태일 때 UV LED를 끔. 250224 CH.PARK */
    if(F_IceOn == CLEAR)
    {
        Bit1_Ice_Tray_UV_Ice_Off_State = SET;
    }
    else
    {
        Bit1_Ice_Tray_UV_Ice_Off_State = CLEAR;
    }

    /* 얼음추출 시 UV LED OFF. 250224 CH.PARK */
    if(F_IceOut == SET)
    {
        Bit2_Ice_Tray_UV_Ice_Extract_State = SET;
    }
    else
    {
        if(gu16_Ice_Door_StepMotor == 0)
        {
            Bit2_Ice_Tray_UV_Ice_Extract_State = CLEAR;
        }
        else {  }
    }

    Bit3_Ice_Tray_UV_Retry_3S_Off_State = uvIceTray.gu8_uv_retry_stop_flag;

    if( u8IceTrayUvOFF > 0 )
    {
        pUV_LED_ICE_TRAY_1_2 = CLEAR;
        bit_uv_ice_tray_out = CLEAR;
    }
    else
    {
        if( u8IceTrayUvON > 0 )
        {
            pUV_LED_ICE_TRAY_1_2 = SET;
            bit_uv_ice_tray_out = SET;
        }
        else
        {
            pUV_LED_ICE_TRAY_1_2 = CLEAR;
            bit_uv_ice_tray_out = CLEAR;
        }
    }
/***********************************************************************************************/
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 uv_ice_tray_control(void)
{
    U8 mu8_return = 0;
    U8 mu8_finish = 0;
    U8 mu8_making_uv = 0;

    uv_ice_tray_24_hour_timer();

    /* 얼음 기능 OFF 상태이거나 설치플러싱 완료 안됐으면 중지 */
    if( F_IceOn == CLEAR || bit_install_flushing_state == SET )
    {
        initial_ice_tray_uv_timer();
        gu8_uv_ice_tray_step = 0;
        bit_ice_tray_1_2_uv_start = CLEAR;
        return CLEAR;
    }
    else{}

/********************************************************************************************/
    switch( gu8_uv_ice_tray_step )
    {
        case 0 :
        mu8_finish = ice_tray_uv_standby();     //만빙 후

        if(mu8_finish == SET)
        {
            gu8_uv_ice_tray_step = 1;
            start_ice_tray_uv();
        }
        else
        {
            mu8_return = ice_tray_uv_making();  // 제빙 중
        }
        break;

        case 1 :
        /*..hui [23-5-31오후 4:20:15] 아이스트레이 살균.. 1시간 동작..*/
        if(bit_ice_tray_1_2_uv_start == SET)
        {
            if(( gu8IceTrayLEV != ICE_TRAY_POSITION_ICE_THROW )
            && ( F_TrayMotorDOWN == CLEAR )                     // 이동중일 때는 끄면 안됨
            )
            {
                /*..hui [23-5-31오후 4:22:58] 아이스 트레이 제빙 위치로 이동하면 취소..*/
                finish_ice_tray_uv();
            }
            else if( Bit0_Ice_Tray_UV_Tank_Open_Off_State == CLEAR 
            && Bit2_Ice_Tray_UV_Ice_Extract_State == CLEAR )
            {
                ice_tray_uv_operation_timer();
                mu8_return = SET;
            }
            else{}
        }
        else
        {
            gu8_uv_ice_tray_step = 0;
        }
        break;

        default :

            gu8_uv_ice_tray_step = 0;

        break;
    }

    return mu8_return;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void ice_tray_uv_operation_timer(void)
{
    gu16_ice_tray_1_2_uv_operation_timer_sec++;
    if(gu16_ice_tray_1_2_uv_operation_timer_sec >= 600)
    {
        gu16_ice_tray_1_2_uv_operation_timer_sec = 0;
        gu8_ice_tray_1_2_uv_operation_timer_min++;
    }
    else{}

    /*..hui [23-5-31???? 4:08:05] 1분..*/
    if(gu8_ice_tray_1_2_uv_operation_timer_min >= ICE_TRAY_UV_THROW_MAX_TIME)
    {
        bit_ice_tray_1_2_uv_start = CLEAR;
        initial_ice_tray_uv_timer();
    }
    else{}
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 ice_tray_uv_standby(void)
{
    U8 mu8_return = 0;

    if( F_IceFull == SET )
    {
        /*..hui [23-5-31오후 4:04:19] 만빙 아님 -> 만빙으로 전환 시..*/
        if( bit_uv_ice_full_state == CLEAR )
        {
            bit_uv_ice_full_state = SET;
            bit_ice_tray_uv_op_1 = SET;
        }
        else{}
		
		/*..hui [23-5-31오후 4:04:34] 만빙되고나서 트레이가 탈빙 위치로 이동 완료하면..*/
        if( gu8IceTrayLEV == ICE_TRAY_POSITION_ICE_THROW )
        {
            bit_ice_tray_uv_op_2 = bit_ice_tray_uv_op_1;
        }
        else{}
    }
    else
    {
        bit_uv_ice_full_state = CLEAR;
        bit_ice_tray_uv_op_1 = CLEAR;
        bit_ice_tray_uv_op_2 = CLEAR;
    }

    if( bit_ice_tray_uv_op_1 == SET 
    && bit_ice_tray_uv_op_2 == SET 
    && gu8_tray_uv_throw_count_one_day < ICE_TRAY_UV_MAX_OPERATION_PER_ONE_DAY_THROW )        // 만빙 후 하루 UV를 쏘는 횟수 : 최대 2회
    {
        initial_ice_tray_uv_timer();
        bit_ice_tray_uv_op_1 = CLEAR;
        bit_ice_tray_uv_op_2 = CLEAR;
        mu8_return = SET;
    }
    else{}

    return mu8_return;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void start_ice_tray_uv(void)
{
    bit_ice_tray_1_2_uv_start = SET;
    initial_ice_tray_uv_timer();


    gu8_tray_uv_throw_count_one_day++;
    if( gu8_tray_uv_throw_count_one_day >= ICE_TRAY_UV_MAX_OPERATION_PER_ONE_DAY_THROW )
    {
        gu8_tray_uv_throw_count_one_day = ICE_TRAY_UV_MAX_OPERATION_PER_ONE_DAY_THROW;
    }
    else{}
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void finish_ice_tray_uv(void)
{
    bit_ice_tray_1_2_uv_start = CLEAR;
    initial_ice_tray_uv_timer();
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void initial_ice_tray_uv_timer(void)
{
    gu16_ice_tray_1_2_uv_operation_timer_sec = 0;
    gu8_ice_tray_1_2_uv_operation_timer_min = 0;
    gu16_ice_tray_1_2_uv_operation_timer_hour = 0;

    gu16_ice_tray_1_2_uv_standby_timer_sec = 0;
    gu16_ice_tray_1_2_uv_standby_timer_min = 0;
    gu16_ice_tray_1_2_uv_standby_timer_hour = 0;

    //gu16_ice_tray_uv_24_hour_timer_sec = 0;
    //gu16_ice_tray_uv_24_hour_timer_min = 0;
    //gu16_ice_tray_uv_24_hour_timer_hour = 0;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void uv_ice_tray_24_hour_timer(void)
{
    gu16_ice_tray_uv_24_hour_timer_sec++;

    if(gu16_ice_tray_uv_24_hour_timer_sec >= 600)
    {
        gu16_ice_tray_uv_24_hour_timer_sec = 0;
        gu16_ice_tray_uv_24_hour_timer_min++;
    }
    else{}

    if(gu16_ice_tray_uv_24_hour_timer_min >= 60)
    {
        gu16_ice_tray_uv_24_hour_timer_min = 0;
        gu16_ice_tray_uv_24_hour_timer_hour++;
    }
    else{}

    if(gu16_ice_tray_uv_24_hour_timer_hour >= 24)
    {
        gu16_ice_tray_uv_24_hour_timer_sec = 0;
        gu16_ice_tray_uv_24_hour_timer_min = 0;
        gu16_ice_tray_uv_24_hour_timer_hour = 0;
        gu8_tray_uv_throw_count_one_day = 0;

        /*..hui [24-2-5???? 10:07:13] ???? ???? ??? ???..*/
        gu8_tray_uv_making_count_one_day = 0;
    }
    else{}
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 ice_tray_uv_making(void)
{
    U8 mu8_out = 0;

    if( gu8IceStep == STATE_31_MAIN_ICE_MAKING )
    {
        if( bit_ice_tray_before == CLEAR )
        {
            bit_ice_tray_before = SET;

            gu8_tray_uv_making_count_one_day++;

            if( gu8_tray_uv_making_count_one_day >= ICE_TRAY_UV_MAX_OPERATION_PER_ONE_DAY_MAKING )     // 제빙 시 하루 uv를 쏘는 최대 횟수 7회
            {
                gu8_tray_uv_making_count_one_day = ICE_TRAY_UV_MAX_OPERATION_PER_ONE_DAY_MAKING;
                bit_ice_tray_making_enable = CLEAR;
            }
            else
            {
                /*..hui [24-2-5???? 10:09:37] ??? ???, ???? ?? 24?ð???? ??? ?????????? ???? ???????? ??????? ???????..*/
                bit_ice_tray_making_enable = SET;
            }
        }
        else{}

        /*..hui [24-2-5???? 9:59:38] ??? ??? 6?..*/
        /*if( u8IceTrayUvOFF == 0 && bit_ice_tray_making_enable == SET )*/
        if( Bit0_Ice_Tray_UV_Tank_Open_Off_State == CLEAR
        && Bit2_Ice_Tray_UV_Ice_Extract_State == CLEAR
        && bit_ice_tray_making_enable == SET )
        {
            gu32_ice_tray_up_max_timer++;
            /*..hui [24-2-5???? 9:59:32] 최대 30분 */
            if( gu32_ice_tray_up_max_timer >= ICE_TRAY_UV_MAKING_MAX_TIME )
            {
                gu32_ice_tray_up_max_timer = ICE_TRAY_UV_MAKING_MAX_TIME;
            }
            else
            {
                mu8_out = SET;
            }
        }
        else{}
    }
    else
    {
        gu32_ice_tray_up_max_timer = 0;
        bit_ice_tray_before = CLEAR;
        bit_ice_tray_making_enable = CLEAR;
    }

    return mu8_out;
}
