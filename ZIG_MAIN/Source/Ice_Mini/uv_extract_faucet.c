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
#include    "uv_extract_faucet.h"
/***********************************************************************************************************************/
/* 출수파우셋 UV 출력 BIT 정의 */
TYPE_BYTE          u8ExtractFaucetUvONB;
#define            u8ExtractFaucetUvON                                  u8ExtractFaucetUvONB.byte
#define            Bit0_Water_Faucet_UV_On_State                        u8ExtractFaucetUvONB.Bit.b0
#define            Bit1_Water_Faucet_UV_Clean_On_State                  u8ExtractFaucetUvONB.Bit.b1
#define            Bit2_Water_Faucet_UV_Fault_Test_On_State             u8ExtractFaucetUvONB.Bit.b2

TYPE_BYTE          u8ExtractFaucetUvOFFB;
#define            u8ExtractFaucetUvOFF                                     u8ExtractFaucetUvOFFB.byte
#define            Bit0_Water_Faucet_Open_UV_Off_State                  u8ExtractFaucetUvOFFB.Bit.b0
#define            Bit1_Water_Extract_UV_Off_State                      u8ExtractFaucetUvOFFB.Bit.b1
#define            Bit2_Water_Faucet_UV_Retry_3S_Off_State              u8ExtractFaucetUvOFFB.Bit.b2
#define            Bit3_Water_Faucet_UV_Error_Off_State                 u8ExtractFaucetUvOFFB.Bit.b3
/***********************************************************************************************************************/
void output_extract_faucet_uv(void);
U8 uv_faucet_control(void);
void uv_faucet_operation_timer(void);
U8 uv_faucet_standby_timer(void);
void start_faucet_uv(void);
void initial_faucet_uv_timer(void);
/***********************************************************************************************************************/
bit bit_uv_extract_faucet_out;
/***********************************************************************************************/
bit bit_faucet_uv_start;

U16 gu16_faucet_uv_operation_timer_sec;
U16 gu16_faucet_uv_operation_timer_min;
U16 gu16_faucet_uv_operation_timer_hour;

U16 gu16_faucet_uv_standby_timer_sec;
U16 gu16_faucet_uv_standby_timer_min;
U16 gu16_faucet_uv_standby_timer_hour;

U8 gu8_uv_faucet_step;
/***********************************************************************************************/
extern UV_Check uvWaterFaucet;
extern bit bit_self_test_start;
/***********************************************************************************************/
/**
 * @brief 얼음파우셋 UV LED 제어 동작 정의
 * 
 */
void output_extract_faucet_uv(void)
{
    if( bit_self_test_start == SET )
    {
        return;
    }
    else{}

    /* 물 추출파우셋 : 3시간 30분 OFF ↔ 30분 ON */
    Bit0_Water_Faucet_UV_On_State = uv_faucet_control();

    /* 물 추출중이면 UV LED OFF함 250225 CH.PARK */
    if(F_WaterOut == SET)
    {
        Bit1_Water_Extract_UV_Off_State = SET;
    }
    else
    {
        Bit1_Water_Extract_UV_Off_State = CLEAR;
    }
    
    Bit2_Water_Faucet_UV_Retry_3S_Off_State = uvWaterFaucet.gu8_uv_retry_stop_flag;

    if( u8ExtractFaucetUvOFF > 0 )
    {
        pUV_LED_WATER_FAUCET = CLEAR;
        bit_uv_extract_faucet_out = CLEAR;
    }
    else
    {
        if( u8ExtractFaucetUvON > 0 )
        {
            pUV_LED_WATER_FAUCET = SET;
            bit_uv_extract_faucet_out = SET;
        }
        else
        {
            pUV_LED_WATER_FAUCET = CLEAR;
            bit_uv_extract_faucet_out = CLEAR;
        }
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 uv_faucet_control(void)
{
    U8 mu8_return = 0;
    U8 mu8_finish = 0;

/********************************************************************************************/
    switch( gu8_uv_faucet_step )
    {
        case 0 :

            /*..hui [19-10-24???? 8:34:44] 6?ð? ????.*/
			/*..sean [23-07-14???? 16:00:00] 5?ð? ????.*/
            mu8_finish = uv_faucet_standby_timer();

            if(mu8_finish == SET)
            {
                gu8_uv_faucet_step = 1;
                start_faucet_uv();
            }
            else{}

        break;

        case 1 :

            /*..hui [23-2-10???? 10:55:28] ?Ŀ???UV?? 30?и? ????..*/
            if(bit_faucet_uv_start == SET)
            {
                /*..hui [23-2-10???? 10:55:38] ?? ????????? ????..*/
                if( u8ExtractFaucetUvOFF == 0 )
                {
                    uv_faucet_operation_timer();
                    mu8_return = SET;
                }
                else{}
            }
            else
            {
                gu8_uv_faucet_step = 0;
            }

        break;

        default :

            gu8_uv_faucet_step = 0;

        break;
    }

    return mu8_return;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void uv_faucet_operation_timer(void)
{
    gu16_faucet_uv_operation_timer_sec++;

    if(gu16_faucet_uv_operation_timer_sec >= 600)
    {
        gu16_faucet_uv_operation_timer_sec = 0;
        gu16_faucet_uv_operation_timer_min++;
    }
    else{}

	/*.. sean [25-02-17] 3?ð? 30?? ????, 30?? ???????? ???? ????..*/
	if(gu16_faucet_uv_operation_timer_min >= 30)    
    {
        bit_faucet_uv_start = CLEAR;
        initial_faucet_uv_timer();
    }
    else{}
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 uv_faucet_standby_timer(void)
{
    U8 mu8_return = 0;

    gu16_faucet_uv_standby_timer_sec++;

    if(gu16_faucet_uv_standby_timer_sec >= 600)
    {
        gu16_faucet_uv_standby_timer_sec = 0;
        gu16_faucet_uv_standby_timer_min++;
    }
    else{}

    if(gu16_faucet_uv_standby_timer_min >= 60)
    {
        gu16_faucet_uv_standby_timer_min = 0;
        gu16_faucet_uv_standby_timer_hour++;
    }
    else{}

    if( gu16_faucet_uv_standby_timer_hour >= 3 && gu16_faucet_uv_standby_timer_min >= 30 )      // 5시간 30분 OFF
    {
        initial_faucet_uv_timer();
        mu8_return = SET;
    }
    else{}

    return mu8_return;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void start_faucet_uv(void)
{
    bit_faucet_uv_start = SET;
    initial_faucet_uv_timer();
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void initial_faucet_uv_timer(void)
{
    gu16_faucet_uv_operation_timer_sec = 0;
    gu16_faucet_uv_operation_timer_min = 0;
    gu16_faucet_uv_operation_timer_hour = 0;

    gu16_faucet_uv_standby_timer_sec = 0;
    gu16_faucet_uv_standby_timer_min = 0;
    gu16_faucet_uv_standby_timer_hour = 0;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/

