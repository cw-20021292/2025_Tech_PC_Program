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
#include    "self_test_auto.h"

void periodic_self_test(void);
void send_periodic_self_test(void);
void get_peridoci_test_time(void);
void periodic_self_test_proc(void);
void init_self_test_auto(void);
/*void initial_no_test_data(void);*/
void check_hot_target_time(void);
void init_hot_target(void);
void check_cold_target_time(void);
void init_cold_target(void);



/*U32 gu32_info_update_timer;*/

U16 gu16_periodic_update_time;

U16 gu16_periodic_self_timer_min;
U16 gu16_periodic_self_timer_hour;


U8 gu8_fan_check_delay_timer;
U8 gu8_flow_check_delay_timer;
U8 gu8_cup_check_delay_timer;
U8 gu8_periodic_flow_save;


U16 gu16_hot_target_time_sec;
U16 gu16_hot_target_time_min;

U8 gu8_hot_first_temp;
bit bit_hot_first_on_check;
U16 gu16_wifi_hot_target_time_min;

U16 gu16_cold_target_time_sec;
U16 gu16_cold_target_time_min;

U8 gu8_cold_first_temp;
bit bit_cold_first_on_check;
U16 gu16_wifi_cold_target_time_min;


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void periodic_self_test(void)
{
    get_peridoci_test_time();
    send_periodic_self_test();

    periodic_self_test_proc();
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void send_periodic_self_test(void)
{
    if( gu8_Wifi_Connect_State != WIFI_CONNECT )
    {
        gu16_periodic_self_timer_min = 0;
        gu16_periodic_self_timer_hour = 0;
    }
    else{}

    /*..hui [21-9-10오전 9:33:28] 정밀진단 중인경우 일단 상시진단 시간 초기화..*/
    if( bit_self_test_start == SET )
    {
        gu16_periodic_self_timer_min = 0;
        gu16_periodic_self_timer_hour = 0;
        return;
    }
    else{}

    /*..hui [21-9-9오후 7:48:11] 0이면 상시진단 안함..*/
    if( gu16_periodic_update_time == 0 )
    {
        gu16_periodic_self_timer_min = 0;
        gu16_periodic_self_timer_hour = 0;
        return;
    }
    else{}

    gu16_periodic_self_timer_min++;

    if( gu16_periodic_self_timer_min >= SELF_TEST_TIME_1_HOUR )
    {
        gu16_periodic_self_timer_min = 0;
        gu16_periodic_self_timer_hour++;
    }
    else{}

    if( gu16_periodic_self_timer_hour >= gu16_periodic_update_time )
    {
        /*gu16_periodic_self_timer_min = 0;*/
        /*gu16_periodic_self_timer_hour = 0;*/
        /*..hui [21-9-10오전 9:22:29] 업데이트하고 변수 초기화..*/
        init_self_test_auto();
        send_wifi_self_test_function();
    }
    else{}
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void get_peridoci_test_time(void)
{
    if( gu8_Wifi_Connect_State == WIFI_CONNECT )
    {

        gu16_periodic_update_time = GetWifiRequestValue(WIFI_RQST_PART_PERIOD);
    }
    else
    {
        gu16_periodic_update_time = 0;
        /*gu32_info_update_timer = SELF_TEST_UPDATE_TIME_24HOUR;*/
    }
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void periodic_self_test_proc(void)
{
    if( bit_self_test_start == SET )
    {
        init_self_test_auto();
        return;
    }
    else{}
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    //check_hot_target_time();
    check_cold_target_time();
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void init_self_test_auto(void)
{
    gu16_periodic_self_timer_min = 0;
    gu16_periodic_self_timer_hour = 0;

    gu8_flow_check_delay_timer = 0;
    gu8_fan_check_delay_timer = 0;
    gu8_cup_check_delay_timer = 0;
    gu8_periodic_flow_save = 0;



}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void check_hot_target_time(void)
{

 
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void init_hot_target(void)
{
 
}



/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void check_cold_target_time(void)
{
    if( F_WaterOut == SET )
    {
        init_cold_target();
        return;
    }
    else{}

    /*..hui [24-1-17오후 2:07:43] 온수히터 미 가동 조건에서도 그만..*/
    if( Bit0_Cold_Operation_Disable_State == SET )
    {
        init_cold_target();
        return;
    }
    else{}

    /*..hui [24-2-21오후 1:52:58] 온수 OFF했을 경우에도..*/
    if( F_Cold_Enable == CLEAR )
    {
        init_cold_target();
        return;
    }
    else{}

    if( F_Comp_Output == SET )
    {
        gu16_cold_target_time_sec++;

        if( gu16_cold_target_time_sec >= 600 )
        {
            gu16_cold_target_time_sec = 0;
            gu16_cold_target_time_min++;
        }
        else{}

        if( gu16_cold_target_time_min >= 60 )
        {
            gu16_cold_target_time_min = 60;
        }
        else{}

        if( gu8_Cold_Temperature_One_Degree <= gu8_cold_first_temp - 2  )
        {
            if( bit_cold_first_on_check == CLEAR )
            {
                bit_cold_first_on_check = SET;
                gu16_wifi_cold_target_time_min = gu16_cold_target_time_min;
            }
            else{}
        }
        else if( gu8_Cold_Temperature_One_Degree <= ( mu16_cold_off_temp / 10 ) )
        {
            if( bit_cold_first_on_check == CLEAR )
            {
                bit_cold_first_on_check = SET;
                gu16_wifi_cold_target_time_min = gu16_cold_target_time_min;
            }
            else{}
        }
        else
        {
            if( gu16_cold_target_time_min >= 60 )
            {
                if( bit_cold_first_on_check == CLEAR )
                {
                    bit_cold_first_on_check = SET;
                    gu16_wifi_cold_target_time_min = gu16_cold_target_time_min;
                }
                else{}
            }
            else{}
        }

    }
    else
    {
        /*..hui [23-12-22오후 2:39:14] 모든게 정상인 상태에서 히터 OFF 상태에서 ON으로 변화될때만 측정..*/
        //gu8_hot_first_temp = gu8_Hot_Tank_Temperature_One_Degree;
        //bit_hot_first_on_check = CLEAR;
        init_cold_target();
    }
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void init_cold_target(void)
{
    gu16_cold_target_time_sec = 0;
    gu16_cold_target_time_min = 0;

    gu8_cold_first_temp = gu8_Cold_Temperature_One_Degree;
    /*bit_hot_first_on_check = SET;*/
    bit_cold_first_on_check = CLEAR;

    /*gu16_wifi_hot_target_time_min = SELF_TEST_INITIAL_DATA;*/

}



