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
#include    "valve_cold_in.h"

void output_valve_cold_in_feed2(void);

/***********************************************************************************************************************/
TYPE_BYTE          U8ColdInValveONB;
#define            u8ColdInValveON                         U8ColdInValveONB.byte
#define            Bit0_Cold_In_Water_Out_On_State         U8ColdInValveONB.Bit.b0
#define            Bit1_Tank_Fill_Cold_In                  U8ColdInValveONB.Bit.b1	/* ³Ã¼öÅÊÅ© Ã¤¿ì±â */
#define            Bit2_Cold_In_IceWater_Out_On_State      U8ColdInValveONB.Bit.b2
#define            Bit3_CIV_Ice_Tray_Ster_State            U8ColdInValveONB.Bit.b3
#define			   Bit4_Cody_Water_Line_Clean_State		   U8ColdInValveONB.Bit.b4	/* Cody Water Line Clean Service */
#define            Bit5_Circul_Drain_On_State              U8ColdInValveONB.Bit.b5
#define			   Bit6_Tank_Flushing_Cold_In			   U8ColdInValveONB.Bit.b6	/* ³Ã¼öÅÊÅ© ÇÃ·¯½Ì */

TYPE_BYTE          U8ColdInValveOFFB;
#define            u8ColdInValveOFF                        U8ColdInValveOFFB.byte
#define            Bit0_Cold_In___Off_State                U8ColdInValveOFFB.Bit.b0

extern FLUSHING_STEP gu8_flushing_mode;
extern U8 gu8_flushing_step;
extern bit bit_fill_cold_start;
extern COLD_TANK_FILL_STEP gu8_cold_tank_fill_step;
extern COLDTANK_FLUSHING_STEP gu8_cold_tank_flushing_step;
extern bit F_IceOut;
extern FLUSHING_STEP gu8_Ice_Out_Step;
extern bit F_Circul_Drain;
extern bit F_DrainStatus;
extern bit F_auto_drain_mode_cold_water_valve_out;
/***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void output_valve_cold_in_feed2(void)
{

/***********************************************************************************************/
	/* ÇÃ·¯½Ì - ³Ã¼öÅÊÅ© Ã¤¿ì±â */
	if(gu8_flushing_mode == FLUSHING_FILL_COLD_TANK_START_STATE)
	{
		if(bit_fill_cold_start == SET)
		{
			if(gu8_cold_tank_fill_step == COLD_TANK_FILL_COLD_OPEN)	//step2¿¡¼­ ³Ã¼ö ¿À¹öÇÃ·Î¿ì ¹ëºê ¿¬ ÈÄ 3¿¡¼­ ³Ã¼öÀÔ¼ö¹ëºê ¿­±â
			{
				Bit1_Tank_Fill_Cold_In = SET;
			}
			else
			{
				Bit1_Tank_Fill_Cold_In = CLEAR;
			}
		}
		else
		{
			Bit1_Tank_Fill_Cold_In = CLEAR;
		}
	}
	else
	{
		Bit1_Tank_Fill_Cold_In = CLEAR;
	}

	if(gu8_flushing_mode == FLUSHING_COLD_TANK)	/* ÇÃ·¯½Ì - ³Ã¼öÅÊÅ© ÇÃ·¯½Ì */
	{
		if(gu8_cold_tank_flushing_step == COLDTANK_FLUSHING_5MINUTE)
		{
			Bit6_Tank_Flushing_Cold_In = SET;
		}
		else
		{
			Bit6_Tank_Flushing_Cold_In = CLEAR;
		}
	}
	else
	{
		Bit6_Tank_Flushing_Cold_In = CLEAR;
	}

	/* ÃßÃâ */
	if( F_WaterOut == SET )
    {
		if( u8IceOutState == ICE_SELECT__ICE_WATER )
		{
			if( gu8_Water_Out_Step >= STATE_10_WATER_IN_FEED_ON_STATE
			&& gu8_Water_Out_Step <= STATE_40_EXTRACT_VALVE_OFF_STATE )
			{
				Bit2_Cold_In_IceWater_Out_On_State = SET;
			}
			else
			{
				Bit2_Cold_In_IceWater_Out_On_State = CLEAR;
			}
		}
		else
		{
			/* ¹°ÃßÃâ ÇÏ·Á´Âµ¥ ³Ã¼ö ¼±ÅÃµÇ¾î ÀÖÀ¸¸é */
			if( u8WaterOutState == COLD_WATER_SELECT )
			{
				if( gu8_Water_Out_Step >= STATE_10_WATER_IN_FEED_ON_STATE
				&& gu8_Water_Out_Step <= STATE_40_EXTRACT_VALVE_OFF_STATE )
				{
					Bit0_Cold_In_Water_Out_On_State = SET;
				}
				else
				{
					Bit0_Cold_In_Water_Out_On_State = CLEAR;
				}
			}
			else
			{
				Bit0_Cold_In_Water_Out_On_State = CLEAR;
			}
		}
    }
    else
    {
        Bit0_Cold_In_Water_Out_On_State = CLEAR;
		Bit2_Cold_In_IceWater_Out_On_State = CLEAR;
    }

	/* °í¿Â»ì±Õ */
	if( bit_ice_tank_ster_start == SET )
	 {
			 if( gu8_ice_ster_mode == STER_MODE_HOT_WAIT_STATE )
			 {
                if( gu8_hot_wait_step == 4 )
                {
                    if( u8DrainWaterLevel == DRAIN_LEVEL_EMPTY
                    || u8DrainWaterLevel == DRAIN_LEVEL_LOW )
                    {
                        Bit3_CIV_Ice_Tray_Ster_State = SET;
                    }
                    else
                    {
                        Bit3_CIV_Ice_Tray_Ster_State = CLEAR;
                    }
                }
                else
                {
                    Bit3_CIV_Ice_Tray_Ster_State = CLEAR;
                }
			 }
			else if( gu8_ice_ster_mode == STER_MODE_DRAIN_WAIT_STATE )
			{
				if( gu8_drain_wait_step == 2 )
				{
					if( u8DrainWaterLevel == DRAIN_LEVEL_EMPTY
					|| u8DrainWaterLevel == DRAIN_LEVEL_LOW )
					{
						Bit3_CIV_Ice_Tray_Ster_State = SET;
					}
					else
					{
						Bit3_CIV_Ice_Tray_Ster_State = CLEAR;
					}
				}
				else
				{
					Bit3_CIV_Ice_Tray_Ster_State = CLEAR;
				}
			}
			 else
			 {
				Bit3_CIV_Ice_Tray_Ster_State = CLEAR;
			 }
	 }
	 else
	 {
		Bit3_CIV_Ice_Tray_Ster_State = CLEAR;
	 }

	/* ÄÚµðÀ¯·Î»ì±Õ Cody Water Line Clean Service */
    if(cody_water_line.gu8_start == SET)
    {
        if(cody_water_line.gu8_step == 2)
        {
            Bit4_Cody_Water_Line_Clean_State = SET;
        }
        else
        {
            Bit4_Cody_Water_Line_Clean_State = CLEAR;
        }
    }
    else
    {
        Bit4_Cody_Water_Line_Clean_State = CLEAR;
    }

	if( F_Circul_Drain == SET )
	{
		if( F_DrainStatus == SET )
		{
			/*..hui [23-8-25???? 1:19:18] ?????? ???? ??..*/
			if(F_auto_drain_mode_cold_water_valve_out == SET)
			{
				Bit5_Circul_Drain_On_State = SET;
			}
			else
			{
				Bit5_Circul_Drain_On_State = CLEAR;
			}
		}
		else
		{
			Bit5_Circul_Drain_On_State = CLEAR;
		}
	}
	else
	{
		Bit5_Circul_Drain_On_State = CLEAR;
	}

/***********************************************************************************************/
    if (u8ColdInValveOFF > 0)
    {
        pVALVE_COLD_IN = CLEAR;      /*off*/
    }
    else
    {
        if (u8ColdInValveON > 0)
        {
            pVALVE_COLD_IN = SET;    /*on*/
        }
        else
        {
            pVALVE_COLD_IN = CLEAR;  /*off*/
        }
    }
/***********************************************************************************************/

}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/






