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
#include    "display_fnd_flick.h"

//void start_set_flick( U8 mu8_mode );
U8 on_off_flick_timer(U8 flick_time, U8 interval);
U8 error_flick_timer(U8 flick_time, U8 interval);
void start_button_set_display( U8 mu8_mode );
U8 button_set_display_timer( U8 flick_time );
void stop_button_set_display( void );
U8 uv_percent_display_timer(U16 flick_time );
U8 uv_time_display_timer(U16 flick_time );

TYPE_BYTE          U8ButtonSetDisplayB;
#define            u8ButtonSetDisplay                   U8ButtonSetDisplayB.byte
#define            Bit0_Cold_Button_Set_Display         U8ButtonSetDisplayB.Bit.b0
#define            Bit1_Mute_Button_Set_Display         U8ButtonSetDisplayB.Bit.b1
#define            Bit2_Ice_Button_Set_Display          U8ButtonSetDisplayB.Bit.b2
#define            Bit3_Recover_Fast_Ice_Set_Display    U8ButtonSetDisplayB.Bit.b3


U16 gu16_on_off_flick_timer;
U8 gu8_on_off_flick_cnt;

U8 gu8_error_flick_timer;
U8 gu8_error_flick_cnt;

U16 gu16_uv_percent_flick_timer;
U8 gu8_uv_flick_cnt;
U16 gu16_uv_time_flick_timer;


U16 gu16_button_set_display_timer;
U8 gu8_button_set_display_state;
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 on_off_flick_timer(U8 flick_time, U8 interval)
{
    static U8 u8flick_onoff_state = 0;

    if( gu16_on_off_flick_timer == 0 )
    {
        u8flick_onoff_state = CLEAR;
    }
    else{}

    /*..hui [23-3-15오후 5:46:39] 아이콘 아이스 = 점멸이 아니라 약 2초간 점등임.. 변경..*/
    gu16_on_off_flick_timer++;

    if( gu16_on_off_flick_timer >= flick_time )
    {
        gu8_on_off_flick_cnt = 0;
        gu16_on_off_flick_timer = 0;
        u8flick_onoff_state = CLEAR;

        u8SetDisplay = ON_OFF_DISPLAY_OFF;
        //////bit_periodic_ster_first_display = CLEAR;
    }
    else
    {
        u8flick_onoff_state = SET;
    }


    return u8flick_onoff_state;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 error_flick_timer(U8 flick_time, U8 interval)
{
    static U8 u8flick_error_state = 0;

    gu8_error_flick_timer++;

    if( gu8_error_flick_timer >= flick_time )
    {
        gu8_error_flick_cnt = 0;
        gu8_error_flick_timer = 0;
        u8flick_error_state = CLEAR;

        gu8_error_popup_enable = CLEAR;
    }
    else
    {
        u8flick_error_state = SET;
    }

    return u8flick_error_state;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 button_set_display_timer( U8 flick_time )
{
    gu16_button_set_display_timer++;

    if( gu16_button_set_display_timer >= flick_time )
    {
        gu16_button_set_display_timer = 0;
        gu8_button_set_display_state = CLEAR;

        u8ButtonSetDisplay = BUTTON_SET_DISPLAY_OFF;
    }
    else
    {
        gu8_button_set_display_state = SET;
    }

    return gu8_button_set_display_state;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void stop_button_set_display( void )
{
    u8ButtonSetDisplay = BUTTON_SET_DISPLAY_OFF;
    gu16_button_set_display_timer = 0;
    gu8_button_set_display_state = CLEAR;
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 uv_percent_display_timer(U16 flick_time )
{
    static U8 u8flick_uv_state = 0;

    gu16_uv_percent_flick_timer++;

    if( gu16_uv_percent_flick_timer >= flick_time )
    {
        gu8_uv_flick_cnt = 0;
        gu16_uv_percent_flick_timer = 0;
        u8flick_uv_state = CLEAR;

        gu8_uv_popup_enable = CLEAR;
    }
    else
    {
        u8flick_uv_state = SET;
    }

    return u8flick_uv_state;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 uv_time_display_timer(U16 flick_time )
{
    static U8 u8flick_uv_time_state = 0;

    gu16_uv_time_flick_timer++;

    if( gu16_uv_time_flick_timer >= flick_time )
    {
        gu16_uv_time_flick_timer = 0;
        u8flick_uv_time_state = CLEAR;

        gu8_uv_popup_enable = CLEAR;
    }
    else
    {
        u8flick_uv_time_state = SET;
    }

    return u8flick_uv_time_state;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void start_button_set_display( U8 mu8_mode )
{
    /*..hui [23-5-12오후 2:06:57] FND 표시 메인 화면에서만 표시..*/
    /*if( gu8_Led_Display_Step == LED_Display__MAIN )*/

    /*..hui [23-12-19오후 4:55:10] 온수 OFF -> 온수 선택 OFF표시중에 3초 누르고있으면 ON 표시안됨..*/
    if( gu8_Led_Display_Step == LED_Display__MAIN
    || gu8_Led_Display_Step == LED_Display__BUTTON_SET
    || gu8_Led_Display_Step == LED_Display__ERROR )
    {
        u8ButtonSetDisplay = mu8_mode;
        gu16_button_set_display_timer = 0;
        gu8_button_set_display_state = CLEAR;

        if( gu8_Led_Display_Step == LED_Display__ERROR )
        {
            gu8_error_popup_enable = CLEAR;
        }
        else{}
    }
    else{}
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/




