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
#include    "ster_ice_tray_hot.h"


U8 tray_ster_temp_control(void);
U8 check_tray_ster_low_flow(void);
void control_tray_ster_flow_valve(void);
void tray_ster_flow_control(void);
void heating_control_tray_ster(void);
void tray_ster_heater_watt_check(void);
void Cal_Tray_Ster_HotFlowHz(void);
void tray_ster_flow_state(void);
void tray_ster_flow_adjust( U8 mu8_time );
void tray_ster_heater_control(void);
void check_ster_triac_off(void);



U8 gu8_ster_low_flow_timer;
bit bit_ster_triac_off;
U8 gu8_ster_flow_control_timer;
U8 gu8Tray_HeaterWatt_CheckTime;

U8 gu8_flow_time;
U8 gu8_before_hot_out_temp;
U8 gu8_hot_inc_time;
U8 gu8_hot_dec_time;
U8 gu8_hot_keep_time;

HOT_STATE gu8_hot_state;
FLOW_STATE gu8_flow_state;
U8 gu8_first_over;
bit bit_first_over_target;


U16 gu16_tray_ster_flow_inc_timer;
U16 gu16_tray_ster_flow_dec_timer;
S8 gs8_tray_ster_flow_inc;


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 tray_ster_temp_control(void)
{
    U8 mu8_return = CLEAR;
    U8 mu8_ref_temp = 0;
    U8 mu8_low_flow = 0;

    if( F_WaterOut == SET )
    {
        return mu8_return;
    }
    else{}

    if( bit_ice_tank_ster_start == CLEAR )
    {
        return mu8_return;
    }
    else{}

    if( gu8_ice_ster_mode == STER_MODE_NONE_STATE
    || gu8_ice_ster_mode == STER_MODE_PREPARE
    || gu8_ice_ster_mode == STER_MODE_HOT_WAIT_STATE
    || gu8_ice_ster_mode == STER_MODE_DRAIN_WAIT_STATE
    || gu8_ice_ster_mode == STER_MODE_FINISH )
    {
        return mu8_return;
    }
    else{}

    if( gu8_ice_ster_mode == STER_MODE_HOT_PREHEAT )
    {
        /*if( gu8_preheat_ster_step >= 5 )*/
        if( gu8_preheat_ster_step <= 4 )
        {
            return mu8_return;
        }
        else{}
    }
    else{}

    if( gu8_ice_ster_mode == STER_MODE_HOT_INPUT_STATE )
    {
        if( gu8_hot_input_ster_step >= 3 )
        {
            return mu8_return;
        }
        else{}
    }
    else{}

    if( gu8_ice_ster_mode == STER_MODE_DRAIN_PREHEAT )
    {
        if( gu8_drain_preheat_ster_step <= 4 )
        {
            return mu8_return;
        }
        else{}
    }
    else{}

    if( gu8_ice_ster_mode == STER_MODE_DRAIN_INPUT_STATE )
    {
        if( gu8_drain_input_ster_step >= 3 )
        {
            return mu8_return;
        }
        else{}
    }
    else{}

    if( Bit3_Ice_Tank_Ster_Operation_Disable_State == SET )
    {
        return mu8_return;
    }
    else{}

    /* mu8_ref_temp = gu8_Tray_Ster_Target_Temp + 19; */
    /* mu8_ref_temp = gu8_Tray_Ster_Target_Temp + 20; */
    mu8_ref_temp = 97;

    if( gu8_Hot_Out_Temperature_One_Degree > mu8_ref_temp )
    {
        return mu8_return;
    }
    else{}

    mu8_low_flow = check_tray_ster_low_flow();

    if( mu8_low_flow == SET)
    {
        mu8_return = CLEAR;
    }
    else
    {
        mu8_return = SET;
    }

    return mu8_return;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 check_tray_ster_low_flow(void)
{
    U8 mu8_return = CLEAR;

    if( gu8_Flow_1sec < C_03_LPM )
    {
        gu8_ster_low_flow_timer++;

        if( gu8_ster_low_flow_timer >= 10 )
        {
            gu8_ster_low_flow_timer = 10;
            mu8_return = SET;
        }
        else{}
    }
    else
    {
        gu8_ster_low_flow_timer = 0;
    }

    return mu8_return;
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void control_tray_ster_flow_valve(void)
{
    U16 mu16_adjust = 0;
    U8 mu8_target = 0;

    if( bit_ice_tank_ster_start == SET )
    {
        if( gu8_ice_ster_mode == STER_MODE_HOT_PREHEAT )
        {
            if( gu8_preheat_ster_step >= 5 )
            {
                tray_ster_flow_control();
            }
            else{}
        }
        else if( gu8_ice_ster_mode == STER_MODE_HOT_INPUT_STATE )
        {
            if( gu8_hot_input_ster_step <= 3 )
            {
                tray_ster_flow_control();
            }
            else{}
        }
        else if( gu8_ice_ster_mode == STER_MODE_DRAIN_PREHEAT )
        {
            if( gu8_drain_preheat_ster_step >= 5 )
            {
                tray_ster_flow_control();
            }
            else{}
        }
        else if( gu8_ice_ster_mode == STER_MODE_DRAIN_INPUT_STATE )
        {
            if( gu8_drain_input_ster_step <= 3 )
            {
                tray_ster_flow_control();
            }
            else{}
        }
        else{}
    }
    else
    {
        gu8_ster_flow_control_timer = 0;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void tray_ster_flow_control(void)
{
    U16 mu16_adjust = 0;
    U8 mu8_target = 0;
    U8 mu8_stable_timr = 0;

    mu8_target = u8Target_Hz_Hot;

    if(gu8_Flow_1sec >= u8Target_Hz_Hot + 10
       || gu8_Flow_1sec <= u8Target_Hz_Hot - 10)
    {
        mu16_adjust = 100;
    }
    else if(gu8_Flow_1sec >= u8Target_Hz_Hot + 8
       || gu8_Flow_1sec <= u8Target_Hz_Hot - 8)
    {
        mu16_adjust = 80;
    }
    else if(gu8_Flow_1sec >= u8Target_Hz_Hot + 6
       || gu8_Flow_1sec <= u8Target_Hz_Hot - 6)
    {
        mu16_adjust = 60;
    }
    else if(gu8_Flow_1sec >= u8Target_Hz_Hot + 4
       || gu8_Flow_1sec <= u8Target_Hz_Hot - 4)
    {
        mu16_adjust = 40;
    }
    else if(gu8_Flow_1sec >= u8Target_Hz_Hot + 2
       || gu8_Flow_1sec <= u8Target_Hz_Hot - 2)
    {
        mu16_adjust = 20;
    }
    else
    {
        mu16_adjust = 5;
    }

    mu8_stable_timr = 2;

    if( gu8_hot_flow_mode == FLOW_MODE_NONE  )
    {
        gu8_ster_flow_control_timer++;

        if( gu8_ster_flow_control_timer >= mu8_stable_timr )
        {
            gu8_ster_flow_control_timer = mu8_stable_timr;
        }
        else
        {
            return;
        }
    }
    else
    {
        gu8_ster_flow_control_timer = 0;
        return;
    }

    if( gu8_Flow_1sec >= mu8_target + 1 )
    {
        run_decrease_flow( mu16_adjust );
    }
    else if( gu8_Flow_1sec <= mu8_target - 1 )
    {
        run_increase_flow( mu16_adjust );
    }
    else{}
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void heating_control_tray_ster(void)
{
    tray_ster_heater_watt_check();
    Cal_Tray_Ster_HotFlowHz();
    tray_ster_heater_control();
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void tray_ster_heater_watt_check(void)
{
    U8 mu8_disable = CLEAR;

    if(bit_ice_tank_ster_start != SET)
    {
        mu8_disable = SET;
    }
    else{}

    if(gu8_ice_ster_mode <= STER_MODE_HOT_PREHEAT)
    {
        mu8_disable = SET;
    }
    else{}

    if( mu8_disable == CLEAR )
    {
        if(pHEATER_HOT_H_RELAY == SET && pHEATER_HOT_TRIAC_L == SET && gu8TriacLevelMax == STEP_TRIAC)
        {
            gu8Tray_HeaterWatt_CheckTime++;

            if(gu8Tray_HeaterWatt_CheckTime >= 10)
            {
                gu8Tray_HeaterWatt_CheckTime = 10;
                Cal_HeaterWatt();
            }
            else{}
        }
        else
        {
            gu8Tray_HeaterWatt_CheckTime = 0;
        }
    }
    else
    {
        gu8Tray_HeaterWatt_CheckTime = 0;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
/*------------------------------------------------------------------------------
                            Cal_HotFlowHz()
//온수 추출에 적정한 유량을 계산하세요
//목표: 0.29LPM -> 290ml/min
--------------------------------------------------------------------------------*/
void Cal_Tray_Ster_HotFlowHz(void)
{
    U32 a = 0;
    U32 b = 0;
    U32 c = 0;
    U32 d = 0;
    U32 e = 0;
    U8 mu8_target = 0;
    U8 mu8_flow_inc_time = 0;
    U8 mu8_flow_dec_time = 0;

    if(bit_ice_tank_ster_start != SET)
    {
        gu16_tray_ster_flow_dec_timer = 0;
        gu16_tray_ster_flow_inc_timer = 0;
        gs8_tray_ster_flow_inc = 0;



        gu8_flow_state = FLOW_KEEP_STATE;
        gu8_flow_time = 0;
        bit_first_over_target = CLEAR;

        gu8_hot_state = HOT_KEEP_STATE;
        gu8_hot_inc_time = 0;
        gu8_hot_dec_time = 0;
        gu8_hot_keep_time = 0;

        gu8_first_over = CLEAR;

        return;
    }
    else{}

    mu8_target = gu8_Tray_Ster_Target_Temp;

    if( mu8_target > gu8_Hot_In_Temperature_One_Degree )
    {
        a = (U32)(Heater_Efficiency * 860 * u16Heater_Power_Save)/(U32)1000;
        b = (U32)(mu8_target - gu8_Hot_In_Temperature_One_Degree);
        c = (U32)(a/b);
        d = (U32)(c*1000/60);
    }
    else{}

    Target_Flow_Q = (U16)d;

    // 최소 제한치
    if(Target_Flow_Q <= C_Flow_Min_Q)
    {
        Target_Flow_Q = C_Flow_Min_Q;  //최소 유량은 260으로 고정합니다.
    }
    else{}

    /*..hui [19-12-11오후 3:20:02] 1.0lpm으로 최대 제한.. 오버플로 방지..*/
    if(Target_Flow_Q >= 1044)
    {
        Target_Flow_Q = 1044;
    }
    else{}

    u8Target_Hz_Hot = (U8)(Target_Flow_Q / 18); //20170417

    if(u8Target_Hz_Hot <= C_03_LPM)
    {
        u8Target_Hz_Hot = C_03_LPM;
    }
    else if( u8Target_Hz_Hot >= C_08_LPM)
    {
        u8Target_Hz_Hot = C_08_LPM;
    }
    else{}

    u8Target_Hz_Hot = u8Target_Hz_Hot - 1;

    if(u8Target_Hz_Hot <= C_03_LPM)
    {
        u8Target_Hz_Hot = C_03_LPM;
    }
    else if( u8Target_Hz_Hot >= C_08_LPM)
    {
        u8Target_Hz_Hot = C_08_LPM;
    }
    else{}

    if( (gu8_ice_ster_mode == STER_MODE_HOT_PREHEAT && gu8_preheat_ster_step >= 5)
    || (gu8_ice_ster_mode == STER_MODE_HOT_INPUT_STATE && gu8_hot_input_ster_step <= 3)
    || (gu8_ice_ster_mode == STER_MODE_DRAIN_PREHEAT && gu8_drain_preheat_ster_step >= 5)
    || (gu8_ice_ster_mode == STER_MODE_DRAIN_INPUT_STATE && gu8_drain_input_ster_step <= 3) )
    {
        tray_ster_flow_state();
        tray_ster_flow_adjust( gu8_flow_time );

        if( gu8_Hot_Out_Temperature_One_Degree >= HOT_TRIAC_OFF_TEMP
            || gu8_Hot_Heater_Temperature_One_Degree >= HOT_TRIAC_OFF_TEMP )
        {
            u8Target_Hz_Hot = C_08_LPM;
        }
        else if( gu8_Hot_Out_Temperature_One_Degree >= 97
            || gu8_Hot_Heater_Temperature_One_Degree >= 97 )
        {
            u8Target_Hz_Hot = C_06_LPM;
        }
        else
        {
            u8Target_Hz_Hot = u8Target_Hz_Hot + gs8_tray_ster_flow_inc;
        }
    }
    else
    {
        gu8_before_hot_out_temp = gu8_Hot_Out_Temperature_One_Degree;
    }


    if(u8Target_Hz_Hot <= C_03_LPM)
    {
        u8Target_Hz_Hot = C_03_LPM;
    }
    else if( u8Target_Hz_Hot >= C_08_LPM)
    {
        u8Target_Hz_Hot = C_08_LPM;
    }
    else{}
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void tray_ster_flow_state(void)
{
    U8 mu8_flow_time = 0;
    U8 mu8_flow_inc_time = 0;
    U8 mu8_flow_dec_time = 0;

    if( gu8_Hot_Out_Temperature_One_Degree > gu8_before_hot_out_temp )
    {
        gu8_hot_dec_time = 0;
        gu8_hot_keep_time = 0;

        gu8_hot_inc_time++;

        if( gu8_hot_inc_time >= 5 )
        {
            gu8_hot_inc_time = 0;
            gu8_before_hot_out_temp = gu8_Hot_Out_Temperature_One_Degree;

            gu8_hot_state = HOT_INCREASE_STATE;
        }
        else{}
    }
    else if( gu8_Hot_Out_Temperature_One_Degree < gu8_before_hot_out_temp )
    {
        gu8_hot_inc_time = 0;
        gu8_hot_keep_time = 0;

        gu8_hot_dec_time++;

        if( gu8_hot_dec_time >= 5 )
        {
            gu8_hot_dec_time = 0;
            gu8_before_hot_out_temp = gu8_Hot_Out_Temperature_One_Degree;

            gu8_hot_state = HOT_DECREASE_STATE;
        }
        else{}
    }
    else
    {
        gu8_hot_inc_time = 0;
        gu8_hot_dec_time = 0;
        gu8_hot_keep_time++;

        if( gu8_hot_keep_time >= 10 )
        {
            gu8_hot_keep_time = 10;
            gu8_hot_state = HOT_KEEP_STATE;
        }
        else{}
    }


    if( bit_first_over_target == CLEAR )
    {
        if( gu8_Hot_Heater_Temperature_One_Degree >= gu8_Tray_Ster_Target_Temp )
        {
            bit_first_over_target = SET;
            gu8_flow_state = FLOW_INCREASE_STATE;
            gu8_flow_time = 15;
        }
        else
        {
            if( gu8_Hot_Heater_Temperature_One_Degree <= gu8_Tray_Ster_Target_Temp - 6 )
            {
                gu8_flow_state = FLOW_DECREASE_STATE;
                gu8_flow_time = 18;
            }
            else
            {
                gu8_flow_state = FLOW_INCREASE_STATE;
                gu8_flow_time = 13;
            }
        }
    }
    else
    {
        if( gu8_Hot_Out_Temperature_One_Degree >= 97 )
        {
            gu8_flow_state = FLOW_INCREASE_STATE;
            gu8_flow_time = 8;
        }
        else if( gu8_Hot_Out_Temperature_One_Degree >= gu8_Tray_Ster_Target_Temp + 3 )
        {
            if( gu8_hot_state == HOT_INCREASE_STATE )
            {
                if( gu8_first_over == CLEAR )
                {
                    gu8_first_over = SET;
                    gu8_flow_state = FLOW_INCREASE_STATE;
                    gu8_flow_time = 1;
                }
                else
                {
                    gu8_flow_state = FLOW_INCREASE_STATE;
                    gu8_flow_time = 5;
                }
            }
            else if( gu8_hot_state == HOT_DECREASE_STATE )
            {
                gu8_flow_state = FLOW_DECREASE_STATE;
                gu8_flow_time = 11;
            }
            else
            {
                //gu8_flow_state = FLOW_KEEP_STATE;
                //gu8_flow_time = 5;
            }
        }
        else if( gu8_Hot_Out_Temperature_One_Degree <= gu8_Tray_Ster_Target_Temp )
        {
            if( gu8_hot_state == HOT_INCREASE_STATE )
            {
                gu8_flow_state = FLOW_INCREASE_STATE;
                gu8_flow_time = 18;
            }
            else if( gu8_hot_state == HOT_DECREASE_STATE )
            {
                gu8_flow_state = FLOW_DECREASE_STATE;
                gu8_flow_time = 12;
            }
            else
            {
                //gu8_flow_state = FLOW_KEEP_STATE;
                //gu8_flow_time = 5;
            }
        }
        else
        {
            if( gu8_hot_state == HOT_INCREASE_STATE )
            {
                gu8_flow_state = FLOW_INCREASE_STATE;
                gu8_flow_time = 13;
            }
            else if( gu8_hot_state == HOT_DECREASE_STATE )
            {
                gu8_flow_state = FLOW_DECREASE_STATE;
                gu8_flow_time = 10;
            }
            else
            {
                gu8_flow_state = FLOW_KEEP_STATE;
                gu8_flow_time = 5;
            }
        }

    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void tray_ster_flow_adjust( U8 mu8_time )
{
    U8 mu8_flow_inc_time = 0;
    U8 mu8_flow_dec_time = 0;

    if( gu8_flow_state == FLOW_INCREASE_STATE )
    {
        gu16_tray_ster_flow_inc_timer++;
        gu16_tray_ster_flow_dec_timer = 0;

        if( gu16_tray_ster_flow_inc_timer >= mu8_time )
        {
            gu16_tray_ster_flow_inc_timer = 0;

            gs8_tray_ster_flow_inc = gs8_tray_ster_flow_inc + 1;

            if( gs8_tray_ster_flow_inc >= 7 )
            {
                gs8_tray_ster_flow_inc = 7;
            }
            else{}
        }
        else{}
    }
    else if( gu8_flow_state == FLOW_DECREASE_STATE )
    {
        gu16_tray_ster_flow_dec_timer++;
        gu16_tray_ster_flow_inc_timer = 0;

        if( gu16_tray_ster_flow_dec_timer >= mu8_time )
        {
            gu16_tray_ster_flow_dec_timer = 0;

            gs8_tray_ster_flow_inc = gs8_tray_ster_flow_inc - 1;

            if( gs8_tray_ster_flow_inc <= -4 )
            {
                gs8_tray_ster_flow_inc = -4;
            }
            else{}
        }
        else{}
    }
    else
    {
        gu16_tray_ster_flow_dec_timer = 0;
        gu16_tray_ster_flow_inc_timer = 0;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void tray_ster_heater_control(void)
{
    if( bit_ice_tank_ster_start == CLEAR )
    {
        bit_ster_triac_off = CLEAR;
        return;
    }
    else{}

    if( Bit3_Tray_Ster_Heating_State == CLEAR )
    {
        bit_ster_triac_off = CLEAR;
        return;
    }
    else{}

    check_ster_triac_off();

    if( bit_ster_triac_off == SET )
    {
        Bit_Relay_Output = SET;
        gu8TriacLevelMax =  0;
    }
    else
    {
        Bit_Relay_Output = SET;
        gu8TriacLevelMax =  STEP_TRIAC;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void check_ster_triac_off(void)
{
    U8 mu8_return = CLEAR;

    /*..hui [20-9-23오후 1:19:12] 트라이악 한판 OFF..*/
    if( gu8_Hot_Out_Temperature_One_Degree >= 87
        || gu8_Hot_Heater_Temperature_One_Degree >= 87 )
    {
        bit_ster_triac_off = SET;
    }
    else
    {
        if( bit_ster_triac_off == SET )
        {
            if( gu8_Hot_Out_Temperature_One_Degree <= 82
                && gu8_Hot_Heater_Temperature_One_Degree <= 82 )
            {
                bit_ster_triac_off = CLEAR;
            }
            else{}
        }
        else{}
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/

