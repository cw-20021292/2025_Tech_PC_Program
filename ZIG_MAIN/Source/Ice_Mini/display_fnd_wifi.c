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
#include    "display_fnd_wifi.h"

void fnd_wifi_set(void);
void display__wifi_pairing(void);
void show_pairing_normal(void);
void finish_pairing_screen(void);


TYPE_BYTE          U8PairingStepB;
#define            u8PairingStep                            U8PairingStepB.byte
#define            Bit0_Pairing_Step0_PrePare               U8PairingStepB.Bit.b0
#define            Bit1_Pairing_Step1_25_Percent            U8PairingStepB.Bit.b1
#define            Bit2_Pairing_Step2_50_Percent            U8PairingStepB.Bit.b2
#define            Bit3_Pairing_Step3_75_Percent            U8PairingStepB.Bit.b3
#define            Bit4_Pairing_Step_FINISH                 U8PairingStepB.Bit.b4
#define            Bit5_Pairing_Exit                        U8PairingStepB.Bit.b5
#define            Bit6_Pairing_Error                       U8PairingStepB.Bit.b6
#define            Bit7_Pairing_Forced_Exit                 U8PairingStepB.Bit.b7


extern TYPE_BYTE          U8ButtonSetDisplayB;
#define            u8ButtonSetDisplay                   U8ButtonSetDisplayB.byte
#define            Bit0_Cold_Button_Set_Display         U8ButtonSetDisplayB.Bit.b0
#define            Bit1_Mute_Button_Set_Display         U8ButtonSetDisplayB.Bit.b1
#define            Bit2_Ice_Button_Set_Display          U8ButtonSetDisplayB.Bit.b2
#define            Bit3_Recover_Fast_Ice_Set_Display    U8ButtonSetDisplayB.Bit.b3


U8 gu8_pairing_finish_timer;
U8 gu8_pairing_delay_timer;
U8 gu8_pairing_display_start;

U8 gu8_pairing_progress;

bit bit_display_wifi_error;
extern bit bit_display_last_error;
extern bit bit_wifi_pairing_start;
extern void fnd_right_out( U8 mu8_direct, U8 mu8_hundred, U8 mu8_ten, U8 mu8_one  );

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void fnd_wifi_set(void)
{
    U8 mu8_temporary_thousand = 0;
    U8 mu8_temporary_hundred = 0;
    U8 mu8_temporary_ten = 0;
    U8 mu8_temporary_one = 0;

	mu8_temporary_thousand = DISPLAY_THOUSAND_OFF;

    display__wifi_pairing();

    if( bit_display_last_error == SET )
    {
        gu8_pairing_delay_timer = 0;
        if( gu8_Wifi_Last_Error == WIFI_FAIL_PASS__AP1 )
        {
            mu8_temporary_hundred = DISPLAY_NUM_LARGE_A;
            mu8_temporary_ten = DISPLAY_NUM_LARGE_P;
            mu8_temporary_one = DISPLAY_NUM_1;
        }
        else if( gu8_Wifi_Last_Error == WIFI_NO_ACK__AP2 )
        {
            mu8_temporary_hundred = DISPLAY_NUM_LARGE_A;
            mu8_temporary_ten = DISPLAY_NUM_LARGE_P;
            mu8_temporary_one = DISPLAY_NUM_2;
        }
        else if( gu8_Wifi_Last_Error == WIFI_ERROR_DENY__AP3 )
        {
            mu8_temporary_hundred = DISPLAY_NUM_LARGE_A;
            mu8_temporary_ten = DISPLAY_NUM_LARGE_P;
            mu8_temporary_one = DISPLAY_NUM_3;
        }
        else if( gu8_Wifi_Last_Error == WIFI_NO_AP__AP4 )
        {
            mu8_temporary_hundred = DISPLAY_NUM_LARGE_A;
            mu8_temporary_ten = DISPLAY_NUM_LARGE_P;
            mu8_temporary_one = DISPLAY_NUM_4;
        }
        else if( gu8_Wifi_Last_Error == WIFI_NO_SERVER__AP5 )
        {
            mu8_temporary_hundred = DISPLAY_NUM_LARGE_A;
            mu8_temporary_ten = DISPLAY_NUM_LARGE_P;
            mu8_temporary_one = DISPLAY_NUM_5;
        }
        else
        {
            mu8_temporary_hundred = DISPLAY_OFF;
            mu8_temporary_ten = DISPLAY_OFF;
            mu8_temporary_one = DISPLAY_OFF;
        }
    }
    else if( bit_display_wifi_error == SET )
    {
        gu8_pairing_delay_timer = 0;
        if( gu8_Wifi_Pairing_Error == WIFI_FAIL_PASS__AP1 )
        {
            mu8_temporary_hundred = DISPLAY_NUM_LARGE_A;
            mu8_temporary_ten = DISPLAY_NUM_LARGE_P;
            mu8_temporary_one = DISPLAY_NUM_1;
        }
        else if( gu8_Wifi_Pairing_Error == WIFI_NO_ACK__AP2 )
        {
            mu8_temporary_hundred = DISPLAY_NUM_LARGE_A;
            mu8_temporary_ten = DISPLAY_NUM_LARGE_P;
            mu8_temporary_one = DISPLAY_NUM_2;
        }
        else if( gu8_Wifi_Pairing_Error == WIFI_ERROR_DENY__AP3 )
        {
            mu8_temporary_hundred = DISPLAY_NUM_LARGE_A;
            mu8_temporary_ten = DISPLAY_NUM_LARGE_P;
            mu8_temporary_one = DISPLAY_NUM_3;
        }
        else if( gu8_Wifi_Pairing_Error == WIFI_NO_AP__AP4 )
        {
            mu8_temporary_hundred = DISPLAY_NUM_LARGE_A;
            mu8_temporary_ten = DISPLAY_NUM_LARGE_P;
            mu8_temporary_one = DISPLAY_NUM_4;
        }
        else if( gu8_Wifi_Pairing_Error == WIFI_NO_SERVER__AP5 )
        {
            mu8_temporary_hundred = DISPLAY_NUM_LARGE_A;
            mu8_temporary_ten = DISPLAY_NUM_LARGE_P;
            mu8_temporary_one = DISPLAY_NUM_5;
        }
        else
        {
            mu8_temporary_hundred = DISPLAY_OFF;
            mu8_temporary_ten = DISPLAY_OFF;
            mu8_temporary_one = DISPLAY_OFF;
        }
    }
    else
    {
        /* 0.5초 후 페어링 진행도 표시 */
        gu8_pairing_delay_timer++;
        if(gu8_pairing_delay_timer >= PAIRING_DISPLAY_DELAY_TIME_100MS)
        {
            gu8_pairing_delay_timer = PAIRING_DISPLAY_DELAY_TIME_100MS;
            
            if( gu8_pairing_progress < 100 )
            {
                if( gu8_pairing_progress == 0 )
                {
                    mu8_temporary_hundred = DISPLAY_OFF;
                    mu8_temporary_ten = DISPLAY_OFF;
                    mu8_temporary_one = DISPLAY_NUM_0;
                }
                else
                {
                    mu8_temporary_hundred = DISPLAY_OFF;
                    mu8_temporary_ten = (gu8_pairing_progress / 10);
                    mu8_temporary_one = (gu8_pairing_progress % 10);
                }
            }
            else
            {
                mu8_temporary_hundred = DISPLAY_NUM_1;
                mu8_temporary_ten = DISPLAY_NUM_0;
                mu8_temporary_one = DISPLAY_NUM_0;
            }
        }
        else
        {
            mu8_temporary_hundred = DISPLAY_OFF;
            mu8_temporary_ten = DISPLAY_OFF;
            mu8_temporary_one = DISPLAY_OFF;
        }
    }

    set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_HUNDRED, right_normal_state_percent );
    set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_TEN, right_normal_state_percent );
    set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_ONE, right_normal_state_percent );

    //fnd_out( FIXED_DISPLAY, mu8_temporary_thousand, mu8_temporary_hundred, mu8_temporary_ten, mu8_temporary_one);
    fnd_right_out( FIXED_DISPLAY, mu8_temporary_hundred, mu8_temporary_ten, mu8_temporary_one);
}



/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void display__wifi_pairing(void)
{
    /*..hui [21-3-15오후 7:56:08] 페어링 진행 중 전원ON/OFF로 전원 OFF시 종료..*/
    if( gu8_Wifi_Connect_State == WIFI_OFF )
    {
        finish_pairing_screen();

        return;
    }
    else{}

    if( Bit7_Pairing_Forced_Exit == SET )
    {
        finish_pairing_screen();

        return;
    }
    else{}

    if( Bit5_Pairing_Exit == SET )
    {
        gu8_pairing_finish_timer++;

        if( gu8_pairing_finish_timer >= 20 )
        {
            finish_pairing_screen();
        }
        else{}
    }
    else
    {
        /*if( gu8_Wifi_Last_Error != 0 && bit_display_last_error == CLEAR )*/
        if( bit_display_last_error == SET )
        {
            gu8_pairing_finish_timer++;
            if( gu8_pairing_finish_timer >= 50 )        // 5초
            {
                /*..hui [23-12-20오후 1:27:13] 최초 3초 누르고 에러있을경우 에러만 5초 표시후 화면 종료..*/
                /*..hui [23-12-20오후 1:27:21] 이후 점유인증하면 다시 %화면으로 표시해야함..*/
                finish_pairing_screen();

                bit_display_last_error = CLEAR;
            }
            else{}
        }
        else if( gu8_Wifi_Pairing_Error != 0
        && gu8_Wifi_Pairing_State >= WIFI_PAIRING_STEP1_25_PERCENT )
        {
            gu8_pairing_finish_timer++;
            if( gu8_pairing_finish_timer >= 30 )
            {
                finish_pairing_screen();
            }
            else
            {
                bit_display_wifi_error = SET;
            }
        }
        else
        {
            show_pairing_normal();
        }
    }
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void show_pairing_normal(void)
{
    gu8_pairing_display_start = SET;

    if( gu8_pairing_display_start == SET )
    {
        switch( gu8_Wifi_Pairing_State  )
        {
            case WIFI_PAIRING_STEP0_PREPARE:

                if( Bit0_Pairing_Step0_PrePare != SET )
                {
                    Bit0_Pairing_Step0_PrePare = SET;

                    gu8_pairing_progress = PAIRING_PROGRESS_25_PERCENT;
                }
                else{}

            break;


            case WIFI_PAIRING_STEP1_25_PERCENT:

                if( Bit1_Pairing_Step1_25_Percent != SET )
                {
                    Bit1_Pairing_Step1_25_Percent = SET;
                    gu8_pairing_progress = PAIRING_PROGRESS_25_PERCENT;
                }
                else{}

            break;

            case WIFI_PAIRING_STEP2_50_PERCENT:

                if( Bit2_Pairing_Step2_50_Percent != SET )
                {
                    Bit2_Pairing_Step2_50_Percent = SET;
                    gu8_pairing_progress = PAIRING_PROGRESS_50_PERCENT;
                }
                else{}

            break;

            case WIFI_PAIRING_STEP3_75_PERCENT:

                if( Bit3_Pairing_Step3_75_Percent != SET )
                {
                    Bit3_Pairing_Step3_75_Percent = SET;
                    gu8_pairing_progress = PAIRING_PROGRESS_75_PERCENT;
                }
                else{}

            break;

            case WIFI_PAIRING_STEP_FINISH:

                if( Bit4_Pairing_Step_FINISH != SET )
                {
                    Bit4_Pairing_Step_FINISH = SET;
                    gu8_pairing_progress = PAIRING_PROGRESS_100_PERCENT;
                    Bit5_Pairing_Exit = SET;
                }
                else{}

            break;


            default :

            break;

        }
    }
    else{}
}


void finish_pairing_screen(void)
{
    bit_wifi_pairing_start = CLEAR;
    gu8_pairing_finish_timer = 0;
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/





