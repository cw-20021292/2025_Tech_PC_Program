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
#include    "display_fnd_left_extract.h"

void fnd_left_ice_out(void);


U8 gu8_fnd_left_ice_step;
U8 gu8_fnd_left_ice_timer;

U8 gu8_fnd_left_ice_water_step;
U8 gu8_fnd_left_ice_water_timer;

extern U16 gu16_cold_off_flick_timer;
extern U8 gu8_indicator_flick_timer;
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void fnd_left_ice_out(void)
{
    U8 mu8_temporary_hundred = 0;
    U8 mu8_temporary_ten = 0;
    U8 mu8_temporary_one = 0;

    if( F_WaterOut == SET )
    {
        if( gu8_fnd_left_ice_step >= 5 )
        {
            gu8_fnd_left_ice_timer = 0;
            gu8_fnd_left_ice_step = 1;
        }
        else{}
    }
    else{}

    switch( gu8_fnd_left_ice_step )
    {
        case 0 :

            mu8_temporary_hundred = DISPLAY_OFF;
            mu8_temporary_ten = DISPLAY_OFF;
            mu8_temporary_one = DISPLAY_OFF;

            gu8_fnd_left_ice_timer++;

            if( gu8_fnd_left_ice_timer >= 5 )
            {
                gu8_fnd_left_ice_timer = 0;
                gu8_fnd_left_ice_step++;
            }
            else{}

        break;

        case 1 :

            mu8_temporary_hundred = DISPLAY_NUM_SQUARE_UP;
            mu8_temporary_ten = DISPLAY_OFF;
            mu8_temporary_one = DISPLAY_OFF;

            gu8_fnd_left_ice_timer++;

            if( gu8_fnd_left_ice_timer >= 5 )
            {
                gu8_fnd_left_ice_timer = 0;
                gu8_fnd_left_ice_step++;
            }
            else{}


        break;

        case 2 :

            mu8_temporary_hundred = DISPLAY_NUM_SQUARE_DOWN;
            mu8_temporary_ten = DISPLAY_NUM_SQUARE_UP;
            mu8_temporary_one = DISPLAY_OFF;

            gu8_fnd_left_ice_timer++;

            if( gu8_fnd_left_ice_timer >= 5 )
            {
                gu8_fnd_left_ice_timer = 0;
                gu8_fnd_left_ice_step++;
            }
            else{}

        break;

        case 3 :

            mu8_temporary_hundred = DISPLAY_NUM_SQUARE_UP;
            mu8_temporary_ten = DISPLAY_NUM_SQUARE_DOWN;
            mu8_temporary_one = DISPLAY_NUM_SQUARE_UP;

            gu8_fnd_left_ice_timer++;

            if( gu8_fnd_left_ice_timer >= 5 )
            {
                gu8_fnd_left_ice_timer = 0;
                gu8_fnd_left_ice_step++;
            }
            else{}


        break;

        case 4 :

            mu8_temporary_hundred = DISPLAY_NUM_SQUARE_DOWN;
            mu8_temporary_ten = DISPLAY_NUM_SQUARE_UP;
            mu8_temporary_one = DISPLAY_NUM_SQUARE_DOWN;

            gu8_fnd_left_ice_timer++;

            if( gu8_fnd_left_ice_timer >= 5 )
            {
                gu8_fnd_left_ice_timer = 0;

                if( F_WaterOut == SET )
                {
                    gu8_fnd_left_ice_step = 1;
                }
                else
                {
                    gu8_fnd_left_ice_step++;
                }
            }
            else{}

        break;

        case 5 :

            mu8_temporary_hundred = DISPLAY_NUM_SQUARE_UP;
            mu8_temporary_ten = DISPLAY_NUM_SQUARE_DOWN;
            mu8_temporary_one = DISPLAY_NUM_SQUARE_UP;

            gu8_fnd_left_ice_timer++;

            if( gu8_fnd_left_ice_timer >= 5 )
            {
                gu8_fnd_left_ice_timer = 0;
                gu8_fnd_left_ice_step++;
            }
            else{}

        break;

        case 6 :

            mu8_temporary_hundred = DISPLAY_NUM_SQUARE_DOWN;
            mu8_temporary_ten = DISPLAY_NUM_SQUARE_UP;
            mu8_temporary_one = DISPLAY_NUM_SQUARE_DOWN;

            gu8_fnd_left_ice_timer++;

            if( gu8_fnd_left_ice_timer >= 5 )
            {
                gu8_fnd_left_ice_timer = 0;
                gu8_fnd_left_ice_step++;
            }
            else{}

        break;

        case 7 :

            mu8_temporary_hundred = DISPLAY_OFF;
            mu8_temporary_ten = DISPLAY_NUM_SQUARE_DOWN;
            mu8_temporary_one = DISPLAY_NUM_SQUARE_UP;

            gu8_fnd_left_ice_timer++;

            if( gu8_fnd_left_ice_timer >= 5 )
            {
                gu8_fnd_left_ice_timer = 0;
                gu8_fnd_left_ice_step++;
            }
            else{}

        break;

        case 8 :

            mu8_temporary_hundred = DISPLAY_OFF;
            mu8_temporary_ten = DISPLAY_OFF;
            mu8_temporary_one = DISPLAY_NUM_SQUARE_DOWN;

            gu8_fnd_left_ice_timer++;

            if( gu8_fnd_left_ice_timer >= 5 )
            {
                gu8_fnd_left_ice_timer = 0;
                gu8_fnd_left_ice_step++;
            }
            else{}

        break;

        case 9 :

            mu8_temporary_hundred = DISPLAY_OFF;
            mu8_temporary_ten = DISPLAY_OFF;
            mu8_temporary_one = DISPLAY_OFF;

            gu8_fnd_left_ice_timer++;

            if( gu8_fnd_left_ice_timer >= 5 )
            {
                gu8_fnd_left_ice_timer = 0;
                gu8_fnd_left_ice_step++;
            }
            else{}

        break;

        case 10 :

            mu8_temporary_hundred = DISPLAY_OFF;
            mu8_temporary_ten = DISPLAY_OFF;
            mu8_temporary_one = DISPLAY_OFF;

            gu8_fnd_left_ice_timer++;

            if( gu8_fnd_left_ice_timer >= 5 )
            {
                gu8_fnd_left_ice_timer = 0;
                gu8_fnd_left_ice_step++;
            }
            else{}

        break;

        case 11 :

            mu8_temporary_hundred = DISPLAY_OFF;
            mu8_temporary_ten = DISPLAY_OFF;
            mu8_temporary_one = DISPLAY_OFF;

            gu8_fnd_left_ice_timer++;

            if( gu8_fnd_left_ice_timer >= 5 )
            {
                gu8_fnd_left_ice_timer = 0;
                gu8_fnd_left_ice_step = 1;
            }
            else{}

        break;



        default :

            gu8_fnd_left_ice_step = 0;
            gu8_fnd_left_ice_timer = 0;

        break;
    }

    if( gu8_Led_Display_Step == LED_Display__SLEEP )
    {
        set_duty_percent( DIMMING__PERCENT_SEG_LEFT_HUNDRED, SLEEP_MODE_DIMMING_PERCENT );
        set_duty_percent( DIMMING__PERCENT_SEG_LEFT_TEN, SLEEP_MODE_DIMMING_PERCENT );
        set_duty_percent( DIMMING__PERCENT_SEG_LEFT_ONE, SLEEP_MODE_DIMMING_PERCENT );
    }
    else
    {
        set_duty_percent( DIMMING__PERCENT_SEG_LEFT_HUNDRED, left_normal_state_percent );
        set_duty_percent( DIMMING__PERCENT_SEG_LEFT_TEN, left_normal_state_percent );
        set_duty_percent( DIMMING__PERCENT_SEG_LEFT_ONE, left_normal_state_percent );
    }

    fnd_left_out( FIXED_DISPLAY, mu8_temporary_hundred, mu8_temporary_ten, mu8_temporary_one);
}







