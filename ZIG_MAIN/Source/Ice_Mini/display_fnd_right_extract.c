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
#include    "display_fnd_right_extract.h"


void fnd_right_water_out(void);
void fnd_right_water_out_continue(void);
void fnd_right_water_out_normal(void);
void clac_extract_amount(void);
void fnd_right_ice_out(void);
void fnd_right_dripcoffee_out(void);

U8 gu8_fnd_water_step;
U16 gu16_extract_display_timer;


U16 gu16_extract_display_hz;
U16 gu16_extract_display_cnt;
U8 gu16_extract_display_cnt_old;
U16 gu16_total_extract_amount;
U16 gu16_display_effnt_hz;

U8 gu8_fnd_continue_timer;

U8 gu8_fnd_right_ice_step;
U8 gu8_fnd_right_ice_timer;

U8 gu8_fnd_right_ice_water_step;
U8 gu8_fnd_right_ice_water_timer;
bit bit_return_ice;

U8 gu8_dripcoffee_percent;

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void fnd_right_water_out(void)
{
    if(bit_myWater_setting_start == SET 
    && my_recipe_select == MY_INDEX_DRIPCOFFEE)
    {
        fnd_right_dripcoffee_out();
    }
    else
    {
        fnd_right_water_out_normal();
    }
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void fnd_right_water_out_continue(void)
{
    U8 mu8_temporary_hundred = 0;
    U8 mu8_temporary_ten = 0;
    U8 mu8_temporary_one = 0;

    gu8_fnd_continue_timer++;

    if( gu8_fnd_continue_timer <= 3 )
    {
        mu8_temporary_hundred = 16;
        mu8_temporary_ten = 0;
        mu8_temporary_one = 0;
    }
    else if( gu8_fnd_continue_timer <= 6 )
    {
        mu8_temporary_hundred = 48;
        mu8_temporary_ten = 0;
        mu8_temporary_one = 0;
    }
    else if( gu8_fnd_continue_timer <= 9 )
    {
        mu8_temporary_hundred = 49;
        mu8_temporary_ten = 0;
        mu8_temporary_one = 0;
    }
    else if( gu8_fnd_continue_timer <= 12 )
    {
        mu8_temporary_hundred = 49;
        mu8_temporary_ten = 1;
        mu8_temporary_one = 0;
    }
    else if( gu8_fnd_continue_timer <= 15 )
    {
        mu8_temporary_hundred = 49;
        mu8_temporary_ten = 1;
        mu8_temporary_one = 1;
    }
    else if( gu8_fnd_continue_timer <= 18 )
    {
        mu8_temporary_hundred = 49;
        mu8_temporary_ten = 1;
        mu8_temporary_one = 3;
    }
    else if( gu8_fnd_continue_timer <= 21 )
    {
        mu8_temporary_hundred = 49;
        mu8_temporary_ten = 1;
        mu8_temporary_one = 7;
    }
    else if( gu8_fnd_continue_timer <= 24 )
    {
        mu8_temporary_hundred = 49;
        mu8_temporary_ten = 1;
        mu8_temporary_one = 15;
    }
    else if( gu8_fnd_continue_timer <= 27 )
    {
        mu8_temporary_hundred = 49;
        mu8_temporary_ten = 9;
        mu8_temporary_one = 15;
    }
    else if( gu8_fnd_continue_timer <= 30 )
    {
        mu8_temporary_hundred = 57;
        mu8_temporary_ten = 9;
        mu8_temporary_one = 15;
    }
    else if( gu8_fnd_continue_timer <= 33 )
    {
        mu8_temporary_hundred = 41;
        mu8_temporary_ten = 9;
        mu8_temporary_one = 15;
    }
    else if( gu8_fnd_continue_timer <= 36 )
    {
        mu8_temporary_hundred = 9;
        mu8_temporary_ten = 9;
        mu8_temporary_one = 15;
    }
    else if( gu8_fnd_continue_timer <= 39 )
    {
        mu8_temporary_hundred = 8;
        mu8_temporary_ten = 9;
        mu8_temporary_one = 15;
    }
    else if( gu8_fnd_continue_timer <= 42 )
    {
        mu8_temporary_hundred = 8;
        mu8_temporary_ten = 8;
        mu8_temporary_one = 15;
    }
    else if( gu8_fnd_continue_timer <= 45 )
    {
        mu8_temporary_hundred = 8;
        mu8_temporary_ten = 8;
        mu8_temporary_one = 14;
    }
    else if( gu8_fnd_continue_timer <= 48 )
    {
        mu8_temporary_hundred = 8;
        mu8_temporary_ten = 8;
        mu8_temporary_one = 12;
    }
    else if( gu8_fnd_continue_timer <= 51 )
    {
        mu8_temporary_hundred = 8;
        mu8_temporary_ten = 8;
        mu8_temporary_one = 8;
    }
    else if( gu8_fnd_continue_timer <= 54 )
    {
        mu8_temporary_hundred = 8;
        mu8_temporary_ten = 8;
        mu8_temporary_one = 0;
    }
    else if( gu8_fnd_continue_timer <= 57 )
    {
        mu8_temporary_hundred = 8;
        mu8_temporary_ten = 0;
        mu8_temporary_one = 0;
    }
    else if( gu8_fnd_continue_timer <= 60 )
    {
        mu8_temporary_hundred = 0;
        mu8_temporary_ten = 0;
        mu8_temporary_one = 0;
        gu8_fnd_continue_timer = 0;
    }
    else
    {
        mu8_temporary_hundred = 0;
        mu8_temporary_ten = 0;
        mu8_temporary_one = 0;
        gu8_fnd_continue_timer = 0;
    }

    fnd_right_out( DIRECT_DISPLAY, mu8_temporary_hundred, mu8_temporary_ten, mu8_temporary_one);
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void fnd_right_water_out_normal(void)
{
    U8 mu8_temporary_hundred = 0;
    U8 mu8_temporary_ten = 0;
    U8 mu8_temporary_one = 0;
    U16 mu16_out_time = 0;
    U16 mu16_amount = 0;

    switch( gu8_fnd_water_step )
    {
        case 0:

            mu8_temporary_hundred = DISPLAY_OFF;
            mu8_temporary_ten = DISPLAY_OFF;
            mu8_temporary_one = DISPLAY_NUM_0;

            gu8_fnd_water_step++;
            gu16_extract_display_cnt = 0;
        break;

        case 1:

            mu16_amount = (U16)(gu16_extract_display_cnt * 10);

            if( mu16_amount < 10 )
            {
                mu8_temporary_hundred = DISPLAY_OFF;
                mu8_temporary_ten = DISPLAY_OFF;
                mu8_temporary_one = (U8)mu16_amount;
            }
            else if( mu16_amount < 100 )
            {
                mu8_temporary_hundred = DISPLAY_OFF;
                mu8_temporary_ten = (U8)(mu16_amount / 10);
                mu8_temporary_one = (U8)(mu16_amount % 10);
            }
            else if( mu16_amount < 999)
            {
                mu8_temporary_hundred = (U8)((mu16_amount / 100) % 10);
                mu8_temporary_ten = (U8)((mu16_amount / 10) % 10);
                mu8_temporary_one = (U8)(mu16_amount % 10);
            }
            else if( mu16_amount < 9999)
            {
                mu8_temporary_hundred = (U8)(mu16_amount / 1000);
                mu8_temporary_ten = (U8)((mu16_amount / 100) % 10);
                mu8_temporary_one = (U8)((mu16_amount / 10) % 10);
            }
            else
            {
                fnd_right_water_out_continue();
                return;
            }
        break;


        default :

            gu8_fnd_water_step = 0;

        break;
    }

    if( gu8_Led_Display_Step == LED_Display__SLEEP )
    {
        set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_HUNDRED, SLEEP_MODE_DIMMING_PERCENT );
        set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_TEN, SLEEP_MODE_DIMMING_PERCENT );
        set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_ONE, SLEEP_MODE_DIMMING_PERCENT );
    }
    else
    {
        set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_HUNDRED, right_normal_state_percent );
        set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_TEN, right_normal_state_percent );
        set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_ONE, right_normal_state_percent );
    }

    fnd_right_out( FIXED_DISPLAY, mu8_temporary_hundred, mu8_temporary_ten, mu8_temporary_one);
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void fnd_right_dripcoffee_out(void)
{
	U8 mu8_temporary_hundred = 0;
	U8 mu8_temporary_ten = 0;
	U8 mu8_temporary_one = 0;

	if( gu8_dripcoffee_percent < 100 )
	{
		if( gu8_dripcoffee_percent == 0 )
		{
			mu8_temporary_hundred = DISPLAY_OFF;
			mu8_temporary_ten = DISPLAY_OFF;
			mu8_temporary_one = DISPLAY_NUM_0;
		}
		else if( gu8_dripcoffee_percent < 10)
		{
			mu8_temporary_hundred = DISPLAY_OFF;
			mu8_temporary_ten = DISPLAY_OFF;
			mu8_temporary_one = gu8_dripcoffee_percent;
		}
		else
		{
			mu8_temporary_hundred = DISPLAY_OFF;
			mu8_temporary_ten = (gu8_dripcoffee_percent / 10);
			mu8_temporary_one = (gu8_dripcoffee_percent % 10);
		}
	}
	else
	{
		mu8_temporary_hundred = DISPLAY_NUM_1;
		mu8_temporary_ten = DISPLAY_NUM_0;
		mu8_temporary_one = DISPLAY_NUM_0;
	}

    if( gu8_Led_Display_Step == LED_Display__SLEEP )
    {
        set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_HUNDRED, SLEEP_MODE_DIMMING_PERCENT );
        set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_TEN, SLEEP_MODE_DIMMING_PERCENT );
        set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_ONE, SLEEP_MODE_DIMMING_PERCENT );
    }
    else
    {
        set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_HUNDRED, right_normal_state_percent );
        set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_TEN, right_normal_state_percent );
        set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_ONE, right_normal_state_percent );
    }
	
	fnd_right_out( FIXED_DISPLAY, mu8_temporary_hundred, mu8_temporary_ten, mu8_temporary_one);
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void clac_extract_amount(void)
{
    U16 mu16_extract_disp_hz = 0;

    gu16_extract_display_hz++;

    mu16_extract_disp_hz = (U16)( (gu16_display_effnt_hz - 15) / ( gu16_total_extract_amount / 10 ) );

    if(0 >= gu16_extract_display_hz % mu16_extract_disp_hz)
    {
        gu16_extract_display_cnt++;
    }
    else{}

    if( u8Extract_Continue == SET )
    {
        
    }
    else
    {
        if( gu16_extract_display_cnt >= (U8)(gu16_total_extract_amount / 10 ) )
        {
            gu16_extract_display_cnt = (U8)(gu16_total_extract_amount / 10 );
        }
        else{}    
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void fnd_right_ice_out(void)
{
    U8 mu8_temporary_hundred = 0;
    U8 mu8_temporary_ten = 0;
    U8 mu8_temporary_one = 0;

    if( bit_return_ice == SET )
    {
        /*..hui [23-5-11오후 2:52:00] 물 추출 후 얼음추출 화면으로 돌아갈때 자연스럽게 돌아가기위해..*/
        if( gu8_fnd_left_ice_timer == 0 )
        {
            bit_return_ice = CLEAR;
        }
        else{}

        /*..hui [23-5-12오후 12:31:31] FND 끈 상태로 대기..*/
        gu8_fnd_right_ice_step = 0;
    }
    else
    {
        gu8_fnd_right_ice_step = gu8_fnd_left_ice_step;
    }

    switch( gu8_fnd_right_ice_step )
    {
        case 0 :

            mu8_temporary_hundred = DISPLAY_OFF;
            mu8_temporary_ten = DISPLAY_OFF;
            mu8_temporary_one = DISPLAY_OFF;

        break;

        case 1 :

            mu8_temporary_hundred = DISPLAY_OFF;
            mu8_temporary_ten = DISPLAY_OFF;
            mu8_temporary_one = DISPLAY_OFF;

        break;

        case 2 :

            mu8_temporary_hundred = DISPLAY_OFF;
            mu8_temporary_ten = DISPLAY_OFF;
            mu8_temporary_one = DISPLAY_OFF;

        break;

        case 3 :

            mu8_temporary_hundred = DISPLAY_OFF;
            mu8_temporary_ten = DISPLAY_OFF;
            mu8_temporary_one = DISPLAY_OFF;

        break;

        case 4 :

            mu8_temporary_hundred = DISPLAY_NUM_SQUARE_UP;
            mu8_temporary_ten = DISPLAY_OFF;
            mu8_temporary_one = DISPLAY_OFF;

        break;

        case 5 :

            mu8_temporary_hundred = DISPLAY_NUM_SQUARE_DOWN;
            mu8_temporary_ten = DISPLAY_NUM_SQUARE_UP;
            mu8_temporary_one = DISPLAY_OFF;

        break;

        case 6 :

            mu8_temporary_hundred = DISPLAY_NUM_SQUARE_UP;
            mu8_temporary_ten = DISPLAY_NUM_SQUARE_DOWN;
            mu8_temporary_one = DISPLAY_NUM_SQUARE_UP;

        break;

        case 7 :

            mu8_temporary_hundred = DISPLAY_NUM_SQUARE_DOWN;
            mu8_temporary_ten = DISPLAY_NUM_SQUARE_UP;
            mu8_temporary_one = DISPLAY_NUM_SQUARE_DOWN;

        break;

        case 8 :

            mu8_temporary_hundred = DISPLAY_NUM_SQUARE_UP;
            mu8_temporary_ten = DISPLAY_NUM_SQUARE_DOWN;
            mu8_temporary_one = DISPLAY_NUM_SQUARE_UP;

        break;

        case 9 :

            mu8_temporary_hundred = DISPLAY_NUM_SQUARE_DOWN;
            mu8_temporary_ten = DISPLAY_NUM_SQUARE_UP;
            mu8_temporary_one = DISPLAY_NUM_SQUARE_DOWN;

        break;

        case 10 :

            mu8_temporary_hundred = DISPLAY_OFF;
            mu8_temporary_ten = DISPLAY_NUM_SQUARE_DOWN;
            mu8_temporary_one = DISPLAY_NUM_SQUARE_UP;

        break;

        case 11 :

            mu8_temporary_hundred = DISPLAY_OFF;
            mu8_temporary_ten = DISPLAY_OFF;
            mu8_temporary_one = DISPLAY_NUM_SQUARE_DOWN;

        break;



        default :

            gu8_fnd_left_ice_step = 0;
            gu8_fnd_left_ice_timer = 0;

        break;
    }

    if( gu8_Led_Display_Step == LED_Display__SLEEP )
    {
        set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_HUNDRED, SLEEP_MODE_DIMMING_PERCENT );
        set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_TEN, SLEEP_MODE_DIMMING_PERCENT );
        set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_ONE, SLEEP_MODE_DIMMING_PERCENT );
    }
    else
    {
        set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_HUNDRED, right_normal_state_percent );
        set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_TEN, right_normal_state_percent );
        set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_ONE, right_normal_state_percent );
    }

    fnd_right_out( FIXED_DISPLAY, mu8_temporary_hundred, mu8_temporary_ten, mu8_temporary_one);
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/

