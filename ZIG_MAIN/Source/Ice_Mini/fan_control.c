/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#include    "Macrodriver.h"
#include    "Global_Variable.h"
#include    "Port_Define.h"
#include    "Fan_Control.h"




void output_fan(void);
U8 get_hotgas_fan_operation(void);



TYPE_BYTE          U8CoolingFanONB;
#define            u8CoolingFanON                              U8CoolingFanONB.byte
#define            Bit0_CF_Cold_Make_State                     U8CoolingFanONB.Bit.b0
#define            Bit1_CF_Ice_Make_State                      U8CoolingFanONB.Bit.b1
#define            Bit2_CF_Hot_Gase_State                      U8CoolingFanONB.Bit.b2
#define            Bit3_CF_Fan_Delay_Operation_State           U8CoolingFanONB.Bit.b3
#define            Bit4_CF_Fan_First_Operation_State           U8CoolingFanONB.Bit.b4





TYPE_BYTE          U8CoolingFanOFFB;
#define            u8CoolingFanOFF                             U8CoolingFanOFFB.byte
//#define            Bit0_Filling_Valve_Error_State            U8CoolingFanOFFB.Bit.b0

bit bit_comp_off_check;
U16 gu16_fan_delay_timer;

bit bit_fan_first;
U16 gu16_fan_first_timer;


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void output_fan(void)
{
    U8 mu8_hot_gas_fan = 0;

    if( bit_self_test_start == SET )
    {
        return;
    }
    else{}

    if( F_FW_Version_Display_Mode == CLEAR )
    {

        return;
    }
    else{}

    //Bit0_CF_Cold_Make_State = F_Comp_Output;

    if( Bit0_Cold_Mode_On_State == SET )
    {
        Bit0_CF_Cold_Make_State = SET;
    }
    else
    {
        Bit0_CF_Cold_Make_State = CLEAR;
    }

    /*..hui [23-4-13오전 10:11:47] 연속 제빙 - 트레이 올리고 COMP 가동 시 바로 가동..*/
    /*..hui [23-4-13오전 10:12:09] 첫 제빙 - 트레이 올리고 물 넣고 COMP 가동 시 가동..*/
    if( gu8IceStep >= STATE_14_CHECK_ICE_TRAY_HZ && gu8IceStep <= STATE_41_GAS_SWITCH_HOT_GAS )
    {
        /*Bit1_CF_Ice_Make_State = F_Comp_Output;*/
        Bit1_CF_Ice_Make_State = SET;
    }
    else
    {
        Bit1_CF_Ice_Make_State = CLEAR;
    }


    if( gu8IceStep >= STATE_42_CALC_HOT_GAS_TIME && gu8IceStep <= STATE_43_ICE_TAKE_OFF )
    {
        mu8_hot_gas_fan = get_hotgas_fan_operation();

        if( mu8_hot_gas_fan == SET )
        {
            /*Bit2_CF_Hot_Gase_State = F_Comp_Output;*/
            Bit2_CF_Hot_Gase_State = SET;
        }
        else
        {
            Bit2_CF_Hot_Gase_State = CLEAR;
        }
    }
    else
    {
        Bit2_CF_Hot_Gase_State = CLEAR;
    }


    /*..hui [24-3-25오후 2:13:27] 컴프 OFF 후 15분간 지연 후 OFF..*/
    if( F_Comp_Output == SET )
    {
        /*..hui [24-3-25오후 4:39:49] 외기온도 34도 이상에서만..*/
        /*if( gu8_Amb_Temperature_One_Degree >= 34 )*/
        /*..hui [25-3-19오후 5:44:26] 14도 이상에서만 5분 지연 가동후 OFF 이경민님..*/
        if( gu8_Amb_Temperature_One_Degree > HOT_GAS_AMB_TEMP_13_DIGREE )
        {
            bit_comp_off_check = SET;
        }
        else{}

        gu16_fan_delay_timer = 0;
        Bit3_CF_Fan_Delay_Operation_State = CLEAR;
    }
    else
    {
        /*..hui [24-3-25오후 4:39:41] 제빙 진입후에는 정지..*/
        if( gu8IceStep > STATE_0_STANDBY )
        {
            bit_comp_off_check = CLEAR;
        }
        else{}

        if( bit_comp_off_check == SET )
        {
            gu16_fan_delay_timer++;

            if( gu16_fan_delay_timer >= FAN_DELAY_5_MIN )
            {
                bit_comp_off_check = CLEAR;
                gu16_fan_delay_timer = 0;
                Bit3_CF_Fan_Delay_Operation_State = CLEAR;
            }
            else
            {
                Bit3_CF_Fan_Delay_Operation_State = SET;
            }
        }
        else
        {
            gu16_fan_delay_timer = 0;
            Bit3_CF_Fan_Delay_Operation_State = CLEAR;
        }
    }





    /*..hui [23-9-22오전 9:54:36] 해빙 동작때 핫가스 삭제..*/
    /*Bit3_CF_Tray_Melt_Operation_State = CLEAR;*/

/***********************************************************************************************/
    if (u8CoolingFanOFF > 0)
    {
        pDC_FAN = CLEAR;  /*off*/
    }
    else
    {
        if (u8CoolingFanON > 0)
        {
            pDC_FAN = SET; /*on*/
        }
        else
        {
            pDC_FAN = CLEAR;  /*off*/
        }
    }
/***********************************************************************************************/
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 get_hotgas_fan_operation(void)
{
    U8 mu8_return = 0;

    if( gu8_Amb_Temperature_One_Degree < HOT_GAS_AMB_TEMP_9_DIGREE )
    {
        mu8_return = CLEAR;
    }
    else if( gu8_Amb_Temperature_One_Degree <= HOT_GAS_AMB_TEMP_13_DIGREE )
    {
        /*..hui [23-4-7오후 1:23:42] 9도 ~ 13도..*/
        mu8_return = CLEAR;
    }
    else if( gu8_Amb_Temperature_One_Degree <= HOT_GAS_AMB_TEMP_19_DIGREE )
    {
        /*..hui [23-4-7오후 1:23:55] 14도 ~ 19도..*/
        mu8_return = CLEAR;
    }
    else if( gu8_Amb_Temperature_One_Degree <= HOT_GAS_AMB_TEMP_24_DIGREE )
    {
        /*..hui [23-4-7오후 1:24:08] 20도 ~ 24도..*/
        mu8_return = SET;
    }
    else if( gu8_Amb_Temperature_One_Degree <= HOT_GAS_AMB_TEMP_29_DIGREE )
    {
        /*..hui [23-4-7오후 1:24:18] 25도 ~ 29도..*/
        mu8_return = SET;
    }
    else
    {
        /*..hui [23-4-7오후 1:24:23] 30도 이상..*/
        mu8_return = SET;
    }

    return mu8_return;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/








