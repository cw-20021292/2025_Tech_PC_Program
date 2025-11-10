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
#include    "display_led_flick.h"

void start_hot_off_flick(void);
U8 hot_off_flickering_timer(U8 flick_time, U8 interval);

void start_cold_off_flick(void);
U8 cold_off_flickering_timer(U8 flick_time, U8 interval);
void start_ice_off_flick(void);
U8 ice_off_flickering_timer(U8 flick_time, U8 interval);

void stop_ice_off_flick(void);
void stop_hot_off_flick(void);
void stop_cold_off_flick(void);
void stop_icewater_off_flick(void);

U8 all_lock_flickering_timer(U8 flick_time, U8 interval);
void start_all_lock_flick(void);



void start_ice_lock_flick(void);
U8 ice_lock_flickering_timer(U8 flick_time, U8 interval);
void stop_ice_lock_flick(void);

U8 gu8_indicator_flick_timer;
U8 gu8_indicator_flick_cnt;

U16 gu16_cold_off_flick_timer;
U8 gu8_cold_off_flick_cnt;

U16 gu16_hot_off_flick_timer;
U8 gu8_hot_off_flick_cnt;

U16 gu16_all_lock_flick_timer;
U8 gu8_all_lock_flick_cnt;

U16 gu16_ice_lock_flick_timer;
U8 gu8_ice_lock_flick_cnt;

U16 gu16_ice_off_flick_timer;
U8 gu8_ice_off_flick_cnt;

U16 gu16_icewater_off_flick_timer;
U8 gu8_icewater_off_flick_cnt;
		
U16 gu16_water_extract_flick_timer;
U8 gu8_water_extract_flick_cnt;

void init_flick_timer();
U8 get_remain_flick_timer();
extern void water_select_return_timer_control(U8 mu8_flag);
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void start_ice_lock_flick(void)
{
    /*..hui [24-4-3오후 5:34:52] 기존 깜빡거리던거 있으면 전부 종료하고 싲가하도??.*/
    gu8_bitton_indicator = 0;
    init_flick_timer();

    /*..hui [21-9-1오후 12:04:34] LED 점멸 추가..*/
    Bit0_Ice_Lock_Indicator = SET;
    gu8_ice_lock_flick_cnt = 0;
    gu16_ice_lock_flick_timer = 0;
}
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 ice_lock_flickering_timer(U8 flick_time, U8 interval)
{
    static U8 u8flick_icelock_state = 0;

    gu16_ice_lock_flick_timer++;

    if( (gu16_ice_lock_flick_timer % interval) == 0 )
    {
        u8flick_icelock_state ^= SET;
        gu8_ice_lock_flick_cnt++;
    }
    else{}

    if( gu8_ice_lock_flick_cnt >= flick_time && u8flick_icelock_state == SET )
    {
        gu8_ice_lock_flick_cnt = 0;
        gu16_ice_lock_flick_timer = 0;
        Bit0_Ice_Lock_Indicator = CLEAR;
        u8flick_icelock_state = CLEAR;
				//water_select_return_timer_control(1);
    }
    else{}

    return u8flick_icelock_state;
}

void stop_ice_lock_flick(void)
{
	gu8_ice_lock_flick_cnt = 0;
	gu16_ice_lock_flick_timer = 0;
	Bit0_Ice_Lock_Indicator = CLEAR;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void start_all_lock_flick(void)
{
    /*..hui [24-4-3오후 5:34:52] 기존 깜빡거리던거 있으면 전부 종료하고 싲가하도??.*/
    gu8_bitton_indicator = 0;
		init_flick_timer();

    /*..hui [21-9-1오후 12:04:34] LED 점멸 추가..*/
    Bit4_All_Lock_Indicator = SET;
    gu8_all_lock_flick_cnt = 0;
		gu16_all_lock_flick_timer = 0;
}
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 all_lock_flickering_timer(U8 flick_time, U8 interval)
{
    static U8 u8flick_alllock_state = 0;

    gu16_all_lock_flick_timer++;

    if( (gu16_all_lock_flick_timer % interval) == 0 )
    {
        u8flick_alllock_state ^= SET;
        gu8_all_lock_flick_cnt++;
    }
    else{}

    if( gu8_all_lock_flick_cnt >= flick_time && u8flick_alllock_state == SET )
    {
        gu8_all_lock_flick_cnt = 0;
        gu16_all_lock_flick_timer = 0;
        Bit4_All_Lock_Indicator = CLEAR;
        u8flick_alllock_state = CLEAR;
				//water_select_return_timer_control(1);
    }
    else{}

    return u8flick_alllock_state;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void start_hot_off_flick(void)
{
    /*..hui [24-4-3오후 5:34:52] 기존 깜빡거리던거 있으면 전부 종료하고 싲가하도??.*/
    gu8_bitton_indicator = 0;
    init_flick_timer();

    /*..hui [21-9-1오후 12:04:34] LED 점멸 추가..*/
    Bit3_Hot_Off_Indicator = SET;
    gu8_hot_off_flick_cnt = 0;
    gu16_hot_off_flick_timer = 0;
}
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 hot_off_flickering_timer(U8 flick_time, U8 interval)
{
    static U8 u8flick_hot_state = 0;

    gu16_hot_off_flick_timer++;

    if( (gu16_hot_off_flick_timer % interval) == 0 )
    {
        u8flick_hot_state ^= SET;
        gu8_hot_off_flick_cnt++;
    }
    else{}

    if( gu8_hot_off_flick_cnt >= flick_time && u8flick_hot_state == SET )
    {
        gu8_hot_off_flick_cnt = 0;
        gu16_hot_off_flick_timer = 0;
        Bit3_Hot_Off_Indicator = CLEAR;
        u8flick_hot_state = CLEAR;
				//water_select_return_timer_control(1);
    }
    else{}

    return u8flick_hot_state;
}

void stop_hot_off_flick(void)
{
	gu8_hot_off_flick_cnt = 0;
	gu16_hot_off_flick_timer = 0;
	Bit3_Hot_Off_Indicator = CLEAR;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void start_cold_off_flick(void)
{
    /*..hui [24-4-3오후 5:34:52] 기존 깜빡거리던거 있으면 전부 종료하고 싲가하도??.*/
    gu8_bitton_indicator = 0;
    init_flick_timer();

    /*..hui [21-9-1오후 12:04:34] LED 점멸 추가..*/
    Bit2_Cold_Off_Indicator = SET;
    gu8_cold_off_flick_cnt = 0;
		gu16_cold_off_flick_timer = 0;
}
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 cold_off_flickering_timer(U8 flick_time, U8 interval)
{
    static U8 u8flick_cold_state = 0;

    gu16_cold_off_flick_timer++;

    if( (gu16_cold_off_flick_timer % interval) == 0 )
    {
        u8flick_cold_state ^= SET;
        gu8_cold_off_flick_cnt++;
    }
    else{}

    if( gu8_cold_off_flick_cnt >= flick_time && u8flick_cold_state == SET )
    {
        gu8_cold_off_flick_cnt = 0;
        gu16_cold_off_flick_timer = 0;
        Bit2_Cold_Off_Indicator = CLEAR;
        u8flick_cold_state = CLEAR;
				//water_select_return_timer_control(1);
    }
    else{}

    return u8flick_cold_state;
}

void stop_cold_off_flick(void)
{
	gu8_cold_off_flick_cnt = 0;
	gu16_cold_off_flick_timer = 0;
	Bit2_Cold_Off_Indicator = CLEAR;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void start_ice_off_flick(void)
{
    /*..hui [24-4-3오후 5:34:52] 기존 깜빡거리던거 있으면 전부 종료하고 싲가하도??.*/
    gu8_bitton_indicator = 0;
    init_flick_timer();

    /*..hui [21-9-1오후 12:04:34] LED 점멸 추가..*/
    Bit5_Ice_Off_Indicator = SET;
    gu8_ice_off_flick_cnt = 0;
    gu16_ice_off_flick_timer = 0;
    
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 ice_off_flickering_timer(U8 flick_time, U8 interval)
{
    static U8 u8flick_ice_state = 0;

    gu16_ice_off_flick_timer++;

    if( (gu16_ice_off_flick_timer % interval) == 0 )
    {
        u8flick_ice_state ^= SET;
        gu8_ice_off_flick_cnt++;
    }
    else{}

    if( gu8_ice_off_flick_cnt >= flick_time && u8flick_ice_state == SET )
    {
        u8flick_ice_state = CLEAR;
				stop_ice_off_flick();
    }
    else{}

    return u8flick_ice_state;
}

void stop_ice_off_flick(void)
{
	gu8_ice_off_flick_cnt = 0;
	gu16_ice_off_flick_timer = 0;
	Bit5_Ice_Off_Indicator = CLEAR;
}
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void start_icewater_off_flick(void)
{
    /*..hui [24-4-3오후 5:34:52] 기존 깜빡거리던거 있으면 전부 종료하고 싲가하도??.*/
    gu8_bitton_indicator = 0;
    init_flick_timer();

    /*..hui [21-9-1오후 12:04:34] LED 점멸 추가..*/
    Bit6_Icewater_Off_Indicator = SET;
    gu8_icewater_off_flick_cnt = 0;
		gu16_icewater_off_flick_timer = 0;
    
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 icewater_off_flickering_timer(U8 flick_time, U8 interval)
{
    static U8 u8flick_icewater_state = 0;

    gu16_icewater_off_flick_timer++;

    if( (gu16_icewater_off_flick_timer % interval) == 0 )
    {
        u8flick_icewater_state ^= SET;
        gu8_icewater_off_flick_cnt++;
    }
    else{}

    if( gu8_icewater_off_flick_cnt >= flick_time && u8flick_icewater_state == SET )
    {
        gu8_icewater_off_flick_cnt = 0;
        gu16_icewater_off_flick_timer = 0;
        Bit6_Icewater_Off_Indicator = CLEAR;
        u8flick_icewater_state = CLEAR;
    }
    else{}

    return u8flick_icewater_state;
}

void stop_icewater_off_flick(void)
{
	gu8_icewater_off_flick_cnt = 0;
	gu16_icewater_off_flick_timer = 0;
	Bit6_Icewater_Off_Indicator = CLEAR;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/

void start_water_extract_flick(void)
{
    /*..hui [24-4-3오후 5:34:52] 기존 깜빡거리던거 있으면 전부 종료하고 싲가하도??.*/
    gu8_bitton_indicator = 0;

    /*..hui [21-9-1오후 12:04:34] LED 점멸 추가..*/
    Bit7_water_extract_Indicator = SET;
    gu8_water_extract_flick_cnt = 0;
		gu16_water_extract_flick_timer = 0;
    
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 water_extract_flickering_timer(U8 flick_time, U8 interval)
{
    static U8 u8flick_water_extract_state = 0;

    gu16_water_extract_flick_timer++;

    if( (gu16_water_extract_flick_timer % interval) == 0 )
    {
        u8flick_water_extract_state ^= SET;
        gu8_water_extract_flick_cnt++;
    }
    else{}

    if( gu8_water_extract_flick_cnt >= flick_time && u8flick_water_extract_state == SET )
    {
        gu8_water_extract_flick_cnt = 0;
        gu16_water_extract_flick_timer = 0;
        Bit7_water_extract_Indicator = CLEAR;
        u8flick_water_extract_state = CLEAR;
    }
    else{}

    return u8flick_water_extract_state;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 get_remain_flick_timer()
{
	if(gu8_indicator_flick_timer != 0
	||gu16_cold_off_flick_timer != 0
	||gu16_hot_off_flick_timer != 0
	||gu16_all_lock_flick_timer != 0
	||gu16_ice_off_flick_timer != 0
	||gu16_icewater_off_flick_timer != 0
	||gu16_water_extract_flick_timer != 0)
	{
		return 0;
	}
	else 
		return 1;
}

void init_flick_timer()
{
	gu8_indicator_flick_timer = 0;
	
	gu16_cold_off_flick_timer = 0;
	
	gu16_hot_off_flick_timer = 0;
	
	gu16_all_lock_flick_timer = 0;
	
	gu16_ice_off_flick_timer = 0;
	
	gu16_icewater_off_flick_timer = 0;
	
	gu16_water_extract_flick_timer = 0;

    gu16_ice_lock_flick_timer = 0;
}


