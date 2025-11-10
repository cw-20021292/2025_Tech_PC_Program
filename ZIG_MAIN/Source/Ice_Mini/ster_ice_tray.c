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
#include    "ster_ice_tray.h"


void ice_tray_ster_control(void);
U8 check_ice_ster_enable(void);
void periodic_ster(void);
void start_ice_tank_ster(void);
void init_ice_ster(void);
void stop_ice_tank_ster(void);
void halt_ice_tank_ster(void);
void finish_ice_ster(void);



bit bit_ice_tank_ster_start;
ICE_STER_MODE gu8_ice_ster_mode;
U16 gu16_periodic_ster_time_total;
U16 gu16_periodic_ster_current_time_total;
bit bit_periodic_ster_ready;
U8 gu8_periodic_ster_count;
bit bit_ster_count_check;
U16 gu16_ster_ready_max_timer;

U8 gu8_Tray_Ster_Target_Temp;
U16 gu16_Tray_Ster_Hz;
U16 gu16_drain_fault_timer;

bit bit_periodic_ster_enable;
U8 gu8_wifi_period_ster_hour;
U8 gu8_wifi_period_ster_minute;
U8 gu8_ice_ster_drain_stop_count;
U8 gu8_ice_ster_low_water_stop_count;

U8 gu8_wifi_ster_operation_hour;
U8 gu8_wifi_ster_operation_min;
U8 gu8_wifi_ster_operation_sec;
U32 gu32_wifi_ster_operation_ms;

U8 gu8_drain_tank_ster_count;

#if 1
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void ice_tray_ster_control(void)
{
    U8 mu8_return = 0;
    U8 mu8_finish = 0;
    U8 mu8_enable = 0;

		halt_ice_tank_ster();

		if(gu8_ice_ster_mode > 0)
		{
			gu32_wifi_ster_operation_ms++;
		}
		else
		{
			gu32_wifi_ster_operation_ms = 0;
		}

    switch( gu8_ice_ster_mode )
    {
        case STER_MODE_NONE_STATE:

            mu8_enable = check_ice_ster_enable();

            if( mu8_enable == SET )
            {
                gu8_ice_ster_mode = STER_MODE_PREPARE;
                gu16_prepare_ster_timer = 0;
                gu16_prepare_ster_max_timer = 0;
                gu8_prepare_ster_step = 0;
            }
            else{}

        break;

        case STER_MODE_PREPARE:

            mu8_enable = prepare_ster();

            if( mu8_enable == SET )
            {
                gu8_ice_ster_mode = STER_MODE_HOT_PREHEAT;
                gu16_preheat_ster_timer = 0;
                gu32_preheat_ster_max_timer = 0;
                gu8_preheat_ster_step = 0;

                gu8_Tray_Ster_Target_Temp = ICE_TANK_STER_PREHEAT_TEMP;
            }
            else{}

        break;

        case STER_MODE_HOT_PREHEAT:

            mu8_enable = hot_preheat_ster();

            if( mu8_enable == SET )
            {
                gu8_ice_ster_mode = STER_MODE_HOT_INPUT_STATE;
                gu16_hot_input_ster_timer = 0;
                gu32_hot_input_ster_max_timer = 0;
                gu8_hot_input_ster_step = 0;
            }
            else{}

        break;

        case STER_MODE_HOT_INPUT_STATE:

            mu8_enable = hot_input_ster();

            if( mu8_enable == SET )
            {
                gu8_ice_ster_mode = STER_MODE_HOT_WAIT_STATE;
                gu16_hot_wait_timer = 0;
                gu32_hot_wait_max_timer = 0;
                gu8_hot_wait_step = 0;
            }
            else{}

        break;

        case STER_MODE_HOT_WAIT_STATE:

            mu8_enable = hot_wait_ster();

            if( mu8_enable == SET )
            {
                /* 드레인탱크 살균 기능 추가 [V1.0.0.5] 250903 CH.PARK */
                /*..hui [25-7-7오후 1:08:11] 트레이살균 10회마다 드레인탱크 살균 1회 진행..*/
                if( gu8_drain_tank_ster_count == DRAIN_TANK__STER_CYCLE )
                {
                    gu8_ice_ster_mode = STER_MODE_DRAIN_PREHEAT;
                    gu16_drain_preheat_ster_timer = 0;
                    gu32_drain_preheat_ster_max_timer = 0;
                    gu8_drain_preheat_ster_step = 0;

                    gu8_Tray_Ster_Target_Temp = DRAIN_TANK_STER_PREHEAT_TEMP;
                }
                else
                {
                    gu8_ice_ster_mode = STER_MODE_FINISH;
                    gu16_wait_finish_timer = 0;
                    gu16_wait_finish_max_timer = 0;
                    gu8_wait_finish_step = 0;
                }
            }
            else{}

        break;

        case STER_MODE_DRAIN_PREHEAT:

            mu8_enable = drain_preheat_ster();

            if( mu8_enable == SET )
            {
                gu8_ice_ster_mode = STER_MODE_DRAIN_INPUT_STATE;
                gu16_drain_input_ster_timer = 0;
                gu32_drain_input_ster_max_timer = 0;
                gu8_drain_input_ster_step = 0;
            }
            else{}

        break;

        case STER_MODE_DRAIN_INPUT_STATE:

            mu8_enable = drain_input_ster();

            if( mu8_enable == SET )
            {
                gu8_ice_ster_mode = STER_MODE_DRAIN_WAIT_STATE;
                gu16_drain_wait_timer = 0;
                gu32_drain_wait_max_timer = 0;
                gu8_drain_wait_step = 0;
            }
            else{}

        break;

        case STER_MODE_DRAIN_WAIT_STATE:

            mu8_enable = drain_wait_ster();

            if( mu8_enable == SET )
            {
                gu8_ice_ster_mode = STER_MODE_FINISH;
                gu16_wait_finish_timer = 0;
                gu16_wait_finish_max_timer = 0;
                gu8_wait_finish_step = 0;
            }
            else{}

        break;

        case STER_MODE_FINISH:

            mu8_finish = wait_finish_clean();

            if( mu8_finish == SET )
            {
                finish_ice_ster();

                if( gu8_periodic_ster_count >= PERIODIC__STER_CYCLE )
                {
                    gu8_periodic_ster_count = 0;
                }
                else{}

                gu8_drain_tank_ster_count++;

                /*..hui [25-7-7오후 12:19:51] 10회째에 드레인 탱크 살균하고 초기화..*/
                /*..hui [25-7-7오후 12:24:04] 이건 수동살균 횟수 포함..*/
                if( gu8_drain_tank_ster_count > DRAIN_TANK__STER_CYCLE )
                {
                    gu8_drain_tank_ster_count = 0;
                }
                else{}
            }
            else{}

        break;

        default:

            gu8_ice_ster_mode = STER_MODE_NONE_STATE;

        break;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 check_ice_ster_enable(void)
{
    U8 mu8_return = 0;

    periodic_ster();

    if( bit_ice_tank_ster_start == SET)// && gu8IceStep == STATE_0_STANDBY)
    {
        mu8_return = SET;
    }
    else{}

    if( F_First_Hot_Effluent == SET )
    {
        bit_ice_tank_ster_start = CLEAR;
        bit_periodic_ster_ready = CLEAR;
        gu8_ice_ster_mode = STER_MODE_NONE_STATE;
        mu8_return = CLEAR;
    }
    else{}

    return mu8_return;
}



/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void periodic_ster(void)
{
    U8 mu8_a = 0;
    U8 mu8_b = 0;

    if( bit_periodic_ster_enable == CLEAR || bit_first_time_setting == CLEAR )
    {
        bit_periodic_ster_ready = CLEAR;
        gu16_ster_ready_max_timer = 0;
        bit_ster_count_check = CLEAR;
        gu8_periodic_ster_count = FIRST_POWER_ON_PERIODIC__STER_CYCLE;
        gu8_drain_tank_ster_count = 0;

        return;
    }
    else{}

    /*..hui [23-6-29???? 4:01:43] ?ð?, ?? ?и???? ??~~~~~~~~~~~~ ?????? ???????....*/
    gu16_periodic_ster_current_time_total = (U16)((U16)gu8_rtc_time_Hour * 100);
    gu16_periodic_ster_current_time_total = gu16_periodic_ster_current_time_total + gu8_rtc_time_Min;

    gu16_periodic_ster_time_total = (U16)((U16)gu8_wifi_period_ster_hour * 100);
    gu16_periodic_ster_time_total = gu16_periodic_ster_time_total + gu8_wifi_period_ster_minute;


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
    if( gu16_periodic_ster_current_time_total == gu16_periodic_ster_time_total )
    {
        if( bit_ster_count_check == CLEAR )
        {
            bit_ster_count_check = SET;

            gu8_periodic_ster_count++;

            if( gu8_periodic_ster_count >= PERIODIC__STER_CYCLE )
            {
                gu8_periodic_ster_count = PERIODIC__STER_CYCLE;
            }
            else{}
        }
        else{}

        /*..hui [23-7-7???? 4:27:56] ????????.. ??? ??????.. ?¼? ????????????..*/
        /*..hui [23-10-4???? 2:39:55] ???? / ?u? ?μ? ?? ?? 15?? ????????? ??o ????..*/
        if( Bit3_Ice_Tank_Ster_Operation_Disable_State == CLEAR
        && F_Tank_Cover_Input == SET
        && bit_filter_all == SET
        /*&& gu8_Cold_Temperature_One_Degree <= STER_NO_OPERATION_COLD_TEMP*/
        && gu8_periodic_ster_count >= PERIODIC__STER_CYCLE )
        {
            bit_periodic_ster_ready = SET;
        }
        else{}
    }
    else
    {
        bit_ster_count_check = CLEAR;
    }


    if( bit_periodic_ster_ready == SET )
    {
        /*..hui [24-7-29???? 2:51:38] ?????? ???? ?? ???? ????..*/
        if( Bit3_Ice_Tank_Ster_Operation_Disable_State == CLEAR
            && F_Tank_Cover_Input == SET
            && bit_filter_all == SET
            /*&& gu8_Cold_Temperature_One_Degree <= STER_NO_OPERATION_COLD_TEMP*/
            && bit_self_test_start == CLEAR
            //&& gu8IceStep == STATE_0_ICE_STANDBY_STATE
            && F_WaterOut == CLEAR
            && F_IceOut == CLEAR
            && cody_water_line.gu8_start == CLEAR
            && gu8_ice_system_ok == SET
            && F_First_Hot_Effluent == CLEAR
            && gu8_flushing_mode == FLUSHING_NONE_STATE
            && bit_install_flushing_state == CLEAR )
        {
            start_ice_tank_ster();

            /*..hui [23-7-13???? 9:42:19] ???? ??? ???????? ????? ??? ????..*/
            /*..hui [23-7-13???? 9:42:52] ?ð???μ? ??? ????? ???????.. ??????.. ?? ??????.. ?? ??? ?и?..*/

        }
        else
        {
            gu16_ster_ready_max_timer++;

            /*..hui [23-7-7???? 4:26:42] 1?ð????? ??????, ?¼? ?μ? ?? ????? ?? ??..*/
            /*..hui [23-8-29???? 2:38:34] 5?????? ????..*/
            if( gu16_ster_ready_max_timer >= 3000 ) // 5 minute
            {
                gu16_ster_ready_max_timer = 0;
                stop_ice_tank_ster();
            }
            else{}
        }
    }
    else
    {
        gu16_ster_ready_max_timer = 0;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void start_ice_tank_ster(void)
{
    bit_ice_tank_ster_start = SET;
    bit_periodic_ster_ready = CLEAR;

    gu8_tray_clean_percent = 0;
    gu16_drain_fault_timer = 0;

    init_ice_ster();

    /*..hui [24-5-28???? 3:23:42] ????? ???? ?????? ??´?....*/
    /*..hui [24-5-28???? 3:23:54] ????????μ? ????? ??............. ???????? ???..*/
    F_IceDoorClose = SET;
    F_IceSelectDoorClose = SET;     /* 2KG는 셀렉트도어 닫기도 추가 250729 CH.PARK */
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void init_ice_ster(void)
{
    gu16_prepare_ster_timer = 0;
    gu16_prepare_ster_max_timer = 0;
    gu8_prepare_ster_step = 0;

    gu8_prepare_ster_step = 0;
    gu16_prepare_ster_timer = 0;
    gu16_prepare_ster_max_timer = 0;

    gu8_preheat_ster_step = 0;
    gu16_preheat_ster_timer = 0;
    gu32_preheat_ster_max_timer = 0;

    gu8_hot_input_ster_step = 0;
    gu16_hot_input_ster_timer = 0;
    gu32_hot_input_ster_max_timer = 0;

    gu8_hot_wait_step = 0;
    gu16_hot_wait_timer = 0;
    gu32_hot_wait_max_timer = 0;

    gu8_wait_finish_step = 0;
    gu16_wait_finish_timer = 0;
    gu16_wait_finish_max_timer = 0;

    gu8_tray_clean_percent = 0;

    gu8_Tray_Ster_Target_Temp = ICE_TANK_STER_PREHEAT_TEMP;
    gu16_Tray_Ster_Hz = HOT_STER_INPUT_HZ;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void stop_ice_tank_ster(void)
{
    bit_ice_tank_ster_start = CLEAR;
    bit_periodic_ster_ready = CLEAR;

    gu8_tray_clean_percent  = 0;
    gu16_drain_fault_timer = 0;

    finish_ice_ster();
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void halt_ice_tank_ster(void)
{
    if( gu8_ice_ster_mode >= STER_MODE_PREPARE )
    {
        if( Bit3_Ice_Tank_Ster_Operation_Disable_State == SET )
        {
            finish_ice_ster();
        }
        else{}

        if( F_Tank_Cover_Input == CLEAR )
        {
            play_melody_warning_197();
            finish_ice_ster();
        }
        else{}

        /*..hui [23-7-28???? 3:42:30] ????? ?????Ŀ??, RO ???? ???? ??????? ????..*/
        if( bit_filter_all == CLEAR )
        {
            play_melody_warning_197();
            finish_ice_ster();
        }
        else{}

    }
    else{}
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void finish_ice_ster(void)
{
    bit_ice_tank_ster_start = CLEAR;
    gu8_ice_ster_mode = STER_MODE_NONE_STATE;

    init_ice_ster();

    /*..hui [23-5-22???? 7:08:41] ????? ?o???? ???¸? ????? ????..*/
    /*if( gu8_ice_system_ok == SET && F_TrayMotorUP == CLEAR && gu8IceTrayLEV != ICE_TRAY_POSITION_ICE_THROW )*/
    if( gu8IceTrayLEV != ICE_TRAY_POSITION_ICE_THROW )
    {
        down_tray_motor();
    }
    else{}

    /*..hui [23-11-6???? 9:11:27] ??o ????? ?????? ?????? ????????..*/
    F_IR = SET;
}
#endif


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/





