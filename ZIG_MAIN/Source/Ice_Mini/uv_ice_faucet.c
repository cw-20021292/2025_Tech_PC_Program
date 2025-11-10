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
#include    "uv_ice_faucet.h"
/***********************************************************************************************************************/

/***********************************************************************************************************************/
/* 얼음 파우셋 1 UV 출력 BIT 정의 */
TYPE_BYTE          u8IceFaucetUvONB;
#define            u8IceFaucetUvON                                        u8IceFaucetUvONB.byte
#define            Bit0_Ice_Faucet_UV_On_State                            u8IceFaucetUvONB.Bit.b0

TYPE_BYTE          u8IceFaucetUvOFFB;
#define            u8IceFaucetUvOFF                                     u8IceFaucetUvOFFB.byte
#define            Bit0_Faucet_Open_UV_Off_State                          u8IceFaucetUvOFFB.Bit.b0
#define            Bit1_Ice_Extract_UV_Off_State                        u8IceFaucetUvOFFB.Bit.b1
#define            Bit2_Ice_Faucet_UV_Retry_3S_Off_State                  u8IceFaucetUvOFFB.Bit.b2

/***********************************************************************************************************************/
/* 얼음 파우셋 2 UV 출력 BIT 정의 */
TYPE_BYTE          u8IceFaucetUv2ONB;
#define            u8IceFaucetUv2ON                                        u8IceFaucetUv2ONB.byte
#define            Bit0_Ice_Faucet_UV2_On_State                            u8IceFaucetUv2ONB.Bit.b0

TYPE_BYTE          u8IceFaucetUv2OFFB;
#define            u8IceFaucetUv2OFF                                     u8IceFaucetUv2OFFB.byte
#define            Bit0_Faucet_Open_UV2_Off_State                        u8IceFaucetUv2OFFB.Bit.b0
#define            Bit1_Ice_Extract_UV2_Off_State                        u8IceFaucetUv2OFFB.Bit.b1
#define            Bit2_Ice_Faucet_UV2_Retry_3S_Off_State                u8IceFaucetUv2OFFB.Bit.b2
/***********************************************************************************************************************/
void output_ice_faucet_uv(void);
U8 uv_ice_faucet_control(void);
void uv_ice_faucet_operation_timer(void);
U8 uv_ice_faucet_standby_timer(void);
void start_ice_faucet_uv(void);
void initial_ice_faucet_uv_timer(void);
/***********************************************************************************************************************/
bit bit_uv_ice_faucet_out;
bit bit_uv2_ice_faucet_out;
/***********************************************************************************************/
bit bit_ice_faucet_uv_start;

U16 gu16_ice_faucet_uv_operation_timer_sec;
U16 gu16_ice_faucet_uv_operation_timer_min;
U16 gu16_ice_faucet_uv_operation_timer_hour;

U16 gu16_ice_faucet_uv_standby_timer_sec;
U16 gu16_ice_faucet_uv_standby_timer_min;
U16 gu16_ice_faucet_uv_standby_timer_hour;

U8 gu8_uv_ice_faucet_step;
/***********************************************************************************************/
extern UV_Check uvIceFaucet_1;
extern UV_Check uvIceFaucet_2;
extern bit bit_self_test_start;
extern bit bit_uv_tank_input;
/***********************************************************************************************/
/**
 * @brief 얼음파우셋 UV LED 제어 동작 정의
 * 
 */
void output_ice_faucet_uv(void)
{
    if( bit_self_test_start == SET )
    {
        return;
    }
    else{}

    /* 얼음파우셋 : 3시간 30분 OFF ↔ 30분 ON */
    Bit0_Ice_Faucet_UV_On_State = uv_ice_faucet_control();
    Bit0_Ice_Faucet_UV2_On_State = Bit0_Ice_Faucet_UV_On_State;

    /* PCH ZZANG */
    Bit2_Ice_Faucet_UV_Retry_3S_Off_State = uvIceFaucet_1.gu8_uv_retry_stop_flag;
    Bit2_Ice_Faucet_UV2_Retry_3S_Off_State = uvIceFaucet_2.gu8_uv_retry_stop_flag;

    /* 탱크커버가 하나라도 열렸으면 바로 트레이 UV는 OFF. 250224 CH.PARK */
    Bit0_Faucet_Open_UV_Off_State = (~bit_uv_tank_input);
    Bit0_Faucet_Open_UV2_Off_State = (~bit_uv_tank_input);

    /* 얼음 추출중이면 UV LED OFF함 250225 CH.PARK */
    if(F_IceOut == SET)
    {
        Bit1_Ice_Extract_UV_Off_State = SET;
        Bit1_Ice_Extract_UV2_Off_State = SET;
    }
    else
    {
        Bit1_Ice_Extract_UV_Off_State = CLEAR;
        Bit1_Ice_Extract_UV2_Off_State = CLEAR;
    }

    /* 얼음 파우셋 UV 1 */
    if( u8IceFaucetUvOFF > 0 )
    {
        pUV_LED_ICE_FAUCET_1 = CLEAR;
        bit_uv_ice_faucet_out = CLEAR;
    }
    else
    {
        if( u8IceFaucetUvON > 0 )
        {
            pUV_LED_ICE_FAUCET_1 = SET;
            bit_uv_ice_faucet_out = SET;
        }
        else
        {
            pUV_LED_ICE_FAUCET_1 = CLEAR;
            bit_uv_ice_faucet_out = CLEAR;
        }
    }

    /* 얼음 파우셋 UV 2 */
    if( u8IceFaucetUv2OFF > 0 )
    {
        pUV_LED_ICE_FAUCET_2 = CLEAR;                  /*off*/
        bit_uv2_ice_faucet_out = CLEAR;
    }
    else
    {
        if( u8IceFaucetUv2ON > 0 )
        {
            pUV_LED_ICE_FAUCET_2 = SET;                 /*on*/
            bit_uv2_ice_faucet_out = SET;
        }
        else
        {
            pUV_LED_ICE_FAUCET_2 = CLEAR;              /*off*/
            bit_uv2_ice_faucet_out = CLEAR;
        }
    }
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 uv_ice_faucet_control(void)
{
    U8 mu8_return = 0;
    U8 mu8_finish = 0;

/********************************************************************************************/
    switch( gu8_uv_ice_faucet_step )
    {
        case 0 :

            /*..hui [19-10-24???? 8:34:44] 6?ð? ????.*/
			/*..sean [23-07-14???? 16:00:00] 5?ð? ????.*/
            mu8_finish = uv_ice_faucet_standby_timer();

            if(mu8_finish == SET)
            {
                gu8_uv_ice_faucet_step = 1;
                start_ice_faucet_uv();
            }
            else{}

        break;

        case 1 :

            /*..hui [23-2-10???? 10:55:28] ?Ŀ???UV?? 30?и? ????..*/
            if(bit_ice_faucet_uv_start == SET)
            {
                /*..hui [23-2-10???? 10:55:38] ?? ????????? ????..*/
                if( u8IceFaucetUvOFF == 0 )
                {
                    uv_ice_faucet_operation_timer();
                    mu8_return = SET;
                }
                else{}
            }
            else
            {
                gu8_uv_ice_faucet_step = 0;
            }

        break;

        default :

            gu8_uv_ice_faucet_step = 0;

        break;
    }

    return mu8_return;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void uv_ice_faucet_operation_timer(void)
{
    gu16_ice_faucet_uv_operation_timer_sec++;

    if(gu16_ice_faucet_uv_operation_timer_sec >= 600)
    {
        gu16_ice_faucet_uv_operation_timer_sec = 0;
        gu16_ice_faucet_uv_operation_timer_min++;
    }
    else{}

	/*.. sean [25-02-17] 3?ð? 30?? ????, 30?? ???????? ???? ????..*/
	if(gu16_ice_faucet_uv_operation_timer_min >= 30)
    {
        bit_ice_faucet_uv_start = CLEAR;
        initial_ice_faucet_uv_timer();
    }
    else{}
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 uv_ice_faucet_standby_timer(void)
{
    U8 mu8_return = 0;

    gu16_ice_faucet_uv_standby_timer_sec++;

    if(gu16_ice_faucet_uv_standby_timer_sec >= 600)
    {
        gu16_ice_faucet_uv_standby_timer_sec = 0;
        gu16_ice_faucet_uv_standby_timer_min++;
    }
    else{}

    if(gu16_ice_faucet_uv_standby_timer_min >= 60)
    {
        gu16_ice_faucet_uv_standby_timer_min = 0;
        gu16_ice_faucet_uv_standby_timer_hour++;
    }
    else{}

    if( gu16_ice_faucet_uv_standby_timer_hour >= 3 && gu16_ice_faucet_uv_standby_timer_min >= 30 )
    {
        initial_ice_faucet_uv_timer();
        mu8_return = SET;
    }
    else{}

    return mu8_return;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void start_ice_faucet_uv(void)
{
    bit_ice_faucet_uv_start = SET;
    initial_ice_faucet_uv_timer();
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void initial_ice_faucet_uv_timer(void)
{
    gu16_ice_faucet_uv_operation_timer_sec = 0;
    gu16_ice_faucet_uv_operation_timer_min = 0;
    gu16_ice_faucet_uv_operation_timer_hour = 0;

    gu16_ice_faucet_uv_standby_timer_sec = 0;
    gu16_ice_faucet_uv_standby_timer_min = 0;
    gu16_ice_faucet_uv_standby_timer_hour = 0;
}

/***********************************************************************************************************************/
