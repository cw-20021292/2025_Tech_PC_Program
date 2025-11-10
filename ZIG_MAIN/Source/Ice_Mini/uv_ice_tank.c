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
#include    "uv_ice_tank.h"



void output_ice_tank_uv(void);
U8 uv_ice_tank_control(void);
void ice_tank_uv_operation_timer(void);
U8 ice_tank_uv_standby_timer(void);
void start_ice_tank_uv(void);
void initial_ice_tank_uv_timer(void);
void uv_tank_reed_sw_input(void);

/***********************************************************************************************************************/
/* TANK UV 1,2 제어 */
TYPE_BYTE          U8IceTankUvONB;
#define            u8IceTankUvON                                        U8IceTankUvONB.byte
#define            Bit0_Ice_Tank_UV_On_State                            U8IceTankUvONB.Bit.b0

TYPE_BYTE          U8IceTankUvOFFB;
#define            u8IceTankUvOFF                                       U8IceTankUvOFFB.byte
#define            Bit0_Tank_Open_UV_Off_State                          U8IceTankUvOFFB.Bit.b0
#define            Bit1_Ice_Extract_UV_Off_State                        U8IceTankUvOFFB.Bit.b1
#define            Bit2_Ice_Tank_UV_Retry_3S_Off_State                  U8IceTankUvOFFB.Bit.b2
#define            Bit4_Ice_Tank_YV_Ice_Off_State                       U8IceTankUvOFFB.Bit.b4

/***********************************************************************************************************************/
/* TANK UV 3 제어 */
TYPE_BYTE          U8IceTankUv3ONB;
#define            u8IceTankUv3ON                                        U8IceTankUv3ONB.byte
#define            Bit0_Ice_Tank_UV3_On_State                            U8IceTankUv3ONB.Bit.b0

TYPE_BYTE          U8IceTankUv3OFFB;
#define            u8IceTankUv3OFF                                       U8IceTankUv3OFFB.byte
#define            Bit0_Tank_Open_UV3_Off_State                          U8IceTankUv3OFFB.Bit.b0
#define            Bit1_Ice_Extract_UV3_Off_State                        U8IceTankUv3OFFB.Bit.b1
#define            Bit2_Ice_Tank_UV3_Retry_3S_Off_State                  U8IceTankUv3OFFB.Bit.b2
#define            Bit4_Ice_Tank3_YV_Ice_Off_State                       U8IceTankUv3OFFB.Bit.b4

/***********************************************************************************************************************/
TYPE_BYTE          U812VPowerControl;
#define            u8PowerControl12V                                    U812VPowerControl.byte
#define            Bit0_Gas_Switch_12V_Out_State                        U812VPowerControl.Bit.b0
#define            Bit1_Ice_Tank_UV_12V_Out_State                       U812VPowerControl.Bit.b1
#define            Bit2_Uart_Test_Mode_State                            U812VPowerControl.Bit.b2
#define            Bit3_Water_Tank_UV_12V_Out_State                     U812VPowerControl.Bit.b3
#define            Bit4_Ice_Tray_UV_12V_Out_State                       U812VPowerControl.Bit.b4
#define            Bit5_Ice_Tank_Front_UV_12V_Out_State                 U812VPowerControl.Bit.b5
#define            Bit6_Drain_Pump_FeedBack_12V_Out_State               U812VPowerControl.Bit.b6
/***********************************************************************************************************************/

/***********************************************************************************************************************/
extern UV_Check uvIceTank_1_2;
extern UV_Check uvIceTank_3;
/***********************************************************************************************************************/


bit F_Auto_UV_Control;
bit bit_ice_tank_uv_start;

U8 gu8_uv_ster_timer_per_min;

U8 gu8_UV_Ster_Count_One_Day;

U8 gu8_uv_ster_max_operation_one_day;


U16 gu16_ice_tank_uv_operation_timer_sec;
U8 gu8_ice_tank_uv_operation_timer_min;
U16 gu16_ice_tank_uv_operation_timer_hour;

U16 gu16_ice_tank_uv_standby_timer_sec;
U16 gu16_ice_tank_uv_standby_timer_min;
U16 gu16_ice_tank_uv_standby_timer_hour;


U8 gu8_uv_ice_tank_step;
bit F_Immediately_Uv_Ster;

bit bit_uv_power_on;

U8 gu8_uv_service_reed_on_cnt;
U8 gu8_uv_service_reed_off_cnt;

bit bit_uv_tank_input;

bit bit_uv_ice_tank_out;
bit bit_uv_ice_tank3_out;

U8 gu8_display_uv_reamin_minute;
U8 gu8_display_uv_elapsed_percent;
U8 gu8_uv_popup_enable;

U8 gu8_uv_ice_tank_retry_timer;

U16 gu16_tank_clean_uv_up_max_timer;
U16 gu16_tank_clean_uv_down_max_timer;

bit bit_uv_fault_test_start;
U16 gu16_fault_test_timer;

U32 gu32_UV_Boot_Wait_Timer_100ms = CLEAR;
U32 gu32_UV_Tank_Interval_Timer_100ms = CLEAR;
U16 gu16_UV_Tank_On_Timer_100ms = CLEAR;

extern bit bit_self_test_start;
extern bit bit_install_flushing_state;
extern U16 gu16_Ice_Door_StepMotor;
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void output_ice_tank_uv(void)
{
    if( bit_self_test_start == SET )
    {
        return;
    }
    else{}

    /* 얼음탱크 : 6시간 OFF ↔ 2시간 ON (8시간마다) */
    Bit0_Ice_Tank_UV_On_State = uv_ice_tank_control();
    Bit0_Ice_Tank_UV3_On_State = Bit0_Ice_Tank_UV_On_State;

    /* 탱크커버가 하나라도 열렸으면 바로 트레이 UV는 OFF. 250224 CH.PARK */
    Bit0_Tank_Open_UV_Off_State = (~bit_uv_tank_input);
    Bit0_Tank_Open_UV3_Off_State = (~bit_uv_tank_input);

    /* 얼음추출 시 UV LED OFF. 250224 CH.PARK */
    if(F_IceOut == SET)
    {
        Bit1_Ice_Extract_UV_Off_State = F_IceOut;
        Bit1_Ice_Extract_UV3_Off_State = F_IceOut;
    }
    else
    {
        if(gu16_Ice_Door_StepMotor == 0)
        {
            Bit1_Ice_Extract_UV_Off_State = CLEAR;
            Bit1_Ice_Extract_UV3_Off_State = CLEAR;
        }
        else {  }
    }

    /* 얼음 OFF상태일 때 UV LED를 끔. 250224 CH.PARK */
    if(F_IceOn == CLEAR)
    {
        Bit4_Ice_Tank_YV_Ice_Off_State = SET;
        Bit4_Ice_Tank3_YV_Ice_Off_State = SET;
    }
    else
    {
        Bit4_Ice_Tank_YV_Ice_Off_State = CLEAR;
        Bit4_Ice_Tank3_YV_Ice_Off_State = CLEAR;
    }  
    
    Bit2_Ice_Tank_UV_Retry_3S_Off_State = uvIceTank_1_2.gu8_uv_retry_stop_flag;
    Bit2_Ice_Tank_UV3_Retry_3S_Off_State =  uvIceTank_3.gu8_uv_retry_stop_flag;

/***********************************************************************************************/

    if( u8IceTankUvOFF > 0 )
    {
        pUV_LED_ICE_TANK_1_2 = CLEAR;
        bit_uv_ice_tank_out = CLEAR;
    }
    else
    {
        if( u8IceTankUvON > 0 )
        {
            pUV_LED_ICE_TANK_1_2 = SET;
            bit_uv_ice_tank_out = SET;
        }
        else
        {
            pUV_LED_ICE_TANK_1_2 = CLEAR;
            bit_uv_ice_tank_out = CLEAR;
        }
    }

    if( u8IceTankUv3OFF > 0 )
    {
        pUV_LED_ICE_TANK_3 = CLEAR;
        bit_uv_ice_tank3_out = CLEAR;
    }
    else
    {
        if( u8IceTankUv3ON > 0 )
        {
            pUV_LED_ICE_TANK_3 = SET;
            bit_uv_ice_tank3_out = SET;
        }
        else
        {
            pUV_LED_ICE_TANK_3 = CLEAR;
            bit_uv_ice_tank3_out = CLEAR;
        }
    }

/***********************************************************************************************/
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void uv_tank_reed_sw_input(void)
{
    if(pREED_TANK_COVER_2 == CLEAR)
    {
        gu8_uv_service_reed_off_cnt = 0;
        gu8_uv_service_reed_on_cnt++;

        if( gu8_uv_service_reed_on_cnt >= UV_TANK_COVER_REED_DETECT_TIME )
        {
            /*..hui [19-1-29오후 3:51:34] 탱크 커버 닫힘..*/
            gu8_uv_service_reed_on_cnt = UV_TANK_COVER_REED_DETECT_TIME;
            bit_uv_tank_input = SET;
        }
        else{}
    }
    else
    {
        gu8_uv_service_reed_on_cnt = 0;
        gu8_uv_service_reed_off_cnt++;

        if( gu8_uv_service_reed_off_cnt >= UV_TANK_COVER_REED_DETECT_TIME )
        {
            /*..hui [19-1-29오후 3:51:38] 탱크 커버 열림..*/
            gu8_uv_service_reed_on_cnt = UV_TANK_COVER_REED_DETECT_TIME;
            bit_uv_tank_input = CLEAR;
        }
        else{}
    }
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 uv_ice_tank_control(void)
{
    U8 mu8_return = 0;
    U8 mu8_finish = 0;

    /*..hui [23-6-30???? 10:20:26] ??? ?÷???????? ????..*/
    /*if( F_IceOn == CLEAR || bit_install_flushing_state == SET )*/
    /*..hui [24-1-12???? 2:43:29] ???? OFF????? UV ?????????..*/
    /*if( bit_install_flushing_state == SET )*/
    /*..hui [24-2-2???? 3:27:57] ???? OFF????? ???? ?????? ???..*/
    if( F_IceOn == CLEAR || bit_install_flushing_state == SET )
    {
        initial_ice_tank_uv_timer();
        gu8_uv_ice_tank_step = 0;
        bit_ice_tank_uv_start = CLEAR;
        return CLEAR;
    }
    else{}

/********************************************************************************************/
    switch( gu8_uv_ice_tank_step )
    {
        case 0 :

            /*..hui [19-10-24???? 8:34:44] 6?ð? ???..*/
            mu8_finish = ice_tank_uv_standby_timer();

            if(mu8_finish == SET)
            {
                gu8_uv_ice_tank_step = 1;
                start_ice_tank_uv();
            }
            else{}

        break;

        case 1 :

            /*..hui [19-10-24???? 8:34:49] 2?ð? ????..*/
            if(bit_ice_tank_uv_start == SET)
            {
                /*..hui [19-10-25???? 9:41:56] ???? ??? ????????? ??쿡?? ????..*/
                if( u8IceTankUvOFF == 0 )
                {
                    ice_tank_uv_operation_timer();
                    mu8_return = SET;
                }
                else{}
            }
            else
            {
                gu8_uv_ice_tank_step = 0;
            }

        break;

        default :

            gu8_uv_ice_tank_step = 0;

        break;
    }

    return mu8_return;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void ice_tank_uv_operation_timer(void)
{
    gu16_ice_tank_uv_operation_timer_sec++;

    if(gu16_ice_tank_uv_operation_timer_sec >= 600)
    {
        gu16_ice_tank_uv_operation_timer_sec = 0;
        gu8_ice_tank_uv_operation_timer_min++;
    }
    else{}

    /*..hui [19-10-24???? 8:23:29] ?ð? ?????? ??? ??? ?д?????..LCD ????..*/
    if(gu8_ice_tank_uv_operation_timer_min >= ICE_TANK_UV_OEPRATION_MAX_TIME)
    {
        bit_ice_tank_uv_start = CLEAR;
        initial_ice_tank_uv_timer();
    }
    else{}
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 ice_tank_uv_standby_timer(void)
{
    U8 mu8_return = 0;

    gu16_ice_tank_uv_standby_timer_sec++;

    if(gu16_ice_tank_uv_standby_timer_sec >= 600)
    {
        gu16_ice_tank_uv_standby_timer_sec = 0;
        gu16_ice_tank_uv_standby_timer_min++;
    }
    else{}

    if(gu16_ice_tank_uv_standby_timer_min >= 60)
    {
        gu16_ice_tank_uv_standby_timer_min = 0;
        gu16_ice_tank_uv_standby_timer_hour++;
    }
    else{}

    if( gu16_ice_tank_uv_standby_timer_hour >= ICE_TANK_UV_STANDBY_MAX_TIME )
    {
        initial_ice_tank_uv_timer();
        mu8_return = SET;
    }
    else{}

    return mu8_return;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void start_ice_tank_uv(void)
{
    bit_ice_tank_uv_start = SET;
    initial_ice_tank_uv_timer();
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void initial_ice_tank_uv_timer(void)
{
    gu16_ice_tank_uv_operation_timer_sec = 0;
    gu8_ice_tank_uv_operation_timer_min = 0;
    gu16_ice_tank_uv_operation_timer_hour = 0;

    gu16_ice_tank_uv_standby_timer_sec = 0;
    gu16_ice_tank_uv_standby_timer_min = 0;
    gu16_ice_tank_uv_standby_timer_hour = 0;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/


