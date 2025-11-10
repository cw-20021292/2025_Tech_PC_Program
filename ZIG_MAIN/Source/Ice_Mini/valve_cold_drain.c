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
#include    "valve_cold_drain.h"

void output_cold_drain_valve_feed6(void);


/***********************************************************************************************************************/
TYPE_BYTE          U8ColdDrainValveONB;
#define            u8ColdDrainValveON                      U8ColdDrainValveONB.byte
#define            Bit0_CD_Flushing_Tank_Drain_On_State    U8ColdDrainValveONB.Bit.b0
#define            Bit1_CD_Circul_Drain_On_State           U8ColdDrainValveONB.Bit.b1
#define            Bit2_CD_Manual_Drain_On_State           U8ColdDrainValveONB.Bit.b2
#define            Bit3_CD_Ice_Ster_On_State               U8ColdDrainValveONB.Bit.b3
#define            Bit4_CD_Drain_Retry_On_State            U8ColdDrainValveONB.Bit.b4
#define            Bit5_CD_Drain_Acid_On_State             U8ColdDrainValveONB.Bit.b5




TYPE_BYTE          U8ColdDrainValveOFFB;
#define            u8ColdDrainValveOFF                     U8ColdDrainValveOFFB.byte
#define            Bit0_CD_Error_Off_State                 U8ColdDrainValveOFFB.Bit.b0
#define            Bit1_Flushing_ColdTank_Flushing_State   U8ColdDrainValveOFFB.Bit.b1

/***********************************************************************************************************************/
bit F_auto_drain_mode_cold_water_valve_out;

bit bit_cold_drain_output;

extern FLUSHING_STEP gu8_flushing_mode;
extern COLDTANK_FLUSHING_STEP gu8_cold_tank_flushing_step;
extern bit F_Circul_Drain;
extern bit F_DrainStatus;
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void output_cold_drain_valve_feed6(void)
{
    /***********************************************************************************************/

    if( F_Circul_Drain == SET )
    {
        if( F_DrainStatus == SET )
        {
            /*..hui [23-8-25???? 1:19:18] ?????? ???? ??..*/
            if(F_auto_drain_mode_cold_water_valve_out == SET)
            {
                Bit1_CD_Circul_Drain_On_State = SET;
            }
            else
            {
                Bit1_CD_Circul_Drain_On_State = CLEAR;
            }
        }
        else
        {
            Bit1_CD_Circul_Drain_On_State = CLEAR;
        }
    }
    else
    {
        Bit1_CD_Circul_Drain_On_State = CLEAR;
    }

    /*..hui [23-8-25오후 1:21:54] 수동 배수 동작 중..*/
    if( bit_manual_drain_start == SET )
    {
        if(bit_manual_drain_valve_output == SET)
        {
            Bit2_CD_Manual_Drain_On_State = SET;
        }
        else
        {
            Bit2_CD_Manual_Drain_On_State = CLEAR;
        }
    }
    else
    {
        Bit2_CD_Manual_Drain_On_State = CLEAR;
    }


/***********************************************************************************************/
    if (u8ColdDrainValveOFF > 0)
    {
        pVALVE_COLD_DRAIN = CLEAR;      /*off*/
        bit_cold_drain_output = CLEAR;
    }
    else
    {
        if (u8ColdDrainValveON > 0)
        {
            pVALVE_COLD_DRAIN = SET;    /*on*/
            bit_cold_drain_output = SET;
        }
        else
        {
            pVALVE_COLD_DRAIN = CLEAR;  /*off*/
            bit_cold_drain_output = CLEAR;
        }
    }
/***********************************************************************************************/

}



/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/





