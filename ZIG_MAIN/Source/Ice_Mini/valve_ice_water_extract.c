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
#include    "valve_ice_water_extract.h"
/***********************************************************************************************************************/
/***********************************************************************************************************************/
TYPE_BYTE          U8IceWaterOutValveON;
#define            u8IceWaterOutON                      U8IceWaterOutValveON.byte
#define            Bit0_Ice_Water_Extract_State         U8IceWaterOutValveON.Bit.b0

TYPE_BYTE          U8IceWaterOutValveOFF;
#define            u8IceWaterOutOFF                     U8IceWaterOutValveOFF.byte
#define            Bit0_Ice_Water_Out_Off_State         U8IceWaterOutValveOFF.Bit.b0

/***********************************************************************************************************************/
extern bit F_IceOut;
extern bit F_WaterOut;
extern FLUSHING_STEP gu8_Ice_Out_Step;
/***********************************************************************************************************************/

/***********************************************************************************************************************/
/**
 * @brief 얼음물 추출밸브 제어 함수
 * 
 */
void    OUTPUT_VALVE_ICE_WATER_OUT_FEED6(void)
{
    /* 얼음 추출 관련된 플래그 SET 시 */
    // if(F_IceOut == SET)
	if(F_WaterOut == SET)
	{
        if( u8IceOutState == ICE_SELECT__ICE_WATER )
        {
            /* 얼음물 추출 STATE 시작 */
            if((gu8_Water_Out_Step >= STATE_30_EXTRACT_VALVE_ON_STATE)
            && (gu8_Water_Out_Step < STATE_40_EXTRACT_VALVE_OFF_STATE)
            )
            {
                Bit0_Ice_Water_Extract_State = SET;
            }
            else
            {
                Bit0_Ice_Water_Extract_State = CLEAR;
            }
        }
        else
        {
            Bit0_Ice_Water_Extract_State = CLEAR;
        }
	}
	else
	{
		Bit0_Ice_Water_Extract_State = CLEAR;
	}

    /* 최종 밸브 제어 */
    if (u8IceWaterOutOFF > 0)
    {
        pVALVE_ICE_WATER_EXTRACT = CLEAR;      /*off*/
    }
    else
    {
        if (u8IceWaterOutON > 0)
        {
            pVALVE_ICE_WATER_EXTRACT = SET;    /*on*/
        }
        else
        {
            pVALVE_ICE_WATER_EXTRACT = CLEAR;  /*off*/
        }
    }
}

/***********************************************************************************************************************/

/***********************************************************************************************************************/

/***********************************************************************************************************************/

/***********************************************************************************************************************/

/***********************************************************************************************************************/

/***********************************************************************************************************************/

/***********************************************************************************************************************/
