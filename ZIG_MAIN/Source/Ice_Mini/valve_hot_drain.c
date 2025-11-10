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
#include    "valve_hot_drain.h"

void output_valve_hot_drain_nos7(void);

/***********************************************************************************************************************/
/*..sean [25-01-16] �¼� �巹�� NOS 7 ���..*/
TYPE_BYTE          U8HoTDrainNOS7ValveONB;
#define            u8HoTDrainNOS7ValveON                    U8HoTDrainNOS7ValveONB.byte
#define            Bit0_HDN7_Room_Cold_Water_Out_State      U8HoTDrainNOS7ValveONB.Bit.b0
#define            Bit1_HDN7_Hot_Water_Out_State            U8HoTDrainNOS7ValveONB.Bit.b1
#define            Bit2_HDN7_Hot_Out_State                  U8HoTDrainNOS7ValveONB.Bit.b2
#define            Bit3_HDN7_Water_In_State                 U8HoTDrainNOS7ValveONB.Bit.b3
#define            Bit4_HDN7_Ice_Tray_Ster_State            U8HoTDrainNOS7ValveONB.Bit.b4
#define            Bit5_HDN7_Buffer_Remain_Water_State      U8HoTDrainNOS7ValveONB.Bit.b5


TYPE_BYTE          U8HotDrainNOS7ValveOFFB;
#define            u8HotDrainNOS7ValveOFF                   U8HotDrainNOS7ValveOFFB.byte
#define            Bit0_HDN7_Off_State                      U8HotDrainNOS7ValveOFFB.Bit.b0

/***********************************************************************************************************************/




U16 gu16_feed_drain_full_time;


bit F_filling_add_op;
U16 gu16_filling_add_op_timer;


bit bit_feed_output;

U32 gu32_feed_overheat_on_timer;
U16 gu16_feed_overheat_off_timer;

extern FLUSHING_STEP gu8_flushing_mode;
extern U8 gu8_filter_flushing_step;
extern U8 gu8_Preheat_Step;
/***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void output_valve_hot_drain_nos7(void)
{
    U8 mu8_cold_drain_start_state = 0;

	if( F_WaterOut == SET )
	{
		if( u8WaterOutState == HOT_WATER_SELECT )
		{
// �� 2025-06-10 Phil
			// if( gu8_Water_Out_Step >= STATE_31_WATER_EXTRACT_STATE
			// && gu8_Water_Out_Step < STATE_40_EXTRACT_VALVE_OFF_STATE )
			// {
			// 	Bit1_HDN7_Hot_Water_Out_State = F_WaterOut;
			// }
			// else
			// {
			// 	Bit1_HDN7_Hot_Water_Out_State = CLEAR;
			// }
// �� 2025-06-10 Phil
			//buffer tank ��� ���ؼ� 21 Preheat Step���� ���� �߰�
			/*..hui [25-3-5���� 2:19:44] ���߿� �ٽ�..*/
			if( gu8_Water_Out_Step == STATE_21_HOT_PRE_HEATING_BUFFER_DRAIN_STATE
			|| gu8_Water_Out_Step ==STATE_20_HOT_PRE_HEATING_STATE)// �� 2025-06-10 Phil
//				|| gu8_Preheat_Step == STATE_2_PREHEAT_DRAIN_STATE)// �� 2025-06-10 Phil
			{
				Bit1_HDN7_Hot_Water_Out_State = CLEAR;
			}
			else
			{
				Bit1_HDN7_Hot_Water_Out_State = F_WaterOut;
			}
		}
		else
		{
			Bit1_HDN7_Hot_Water_Out_State = CLEAR;
		}
	}
	else
	{
		Bit1_HDN7_Hot_Water_Out_State = CLEAR;
	}

	if(gu8_flushing_mode == FLUSHING_FILTER)
	{
		if(gu8_filter_flushing_step >= 1 && gu8_filter_flushing_step <= 4)
		{
			Bit2_HDN7_Hot_Out_State = CLEAR;
		}
		else
		{
			Bit2_HDN7_Hot_Out_State = SET;
		}
	}
	else if(gu8_flushing_mode == FLUSHING_FILL_HOT_TANK_STATE)
	{
		Bit2_HDN7_Hot_Out_State = CLEAR;
	}
	else
	{
		Bit2_HDN7_Hot_Out_State = CLEAR;
	}

	
    if( bit_ice_tank_ster_start == SET )
    {
        if( gu8_ice_ster_mode == STER_MODE_HOT_INPUT_STATE )
        {
            /*..hui [25-4-8���� 9:54:41] ���� Ʈ���� �Լ���..*/
            if( gu8_hot_input_ster_step >= 1 && gu8_hot_input_ster_step <= 5 )
            {
                Bit4_HDN7_Ice_Tray_Ster_State = SET;
            }
            else
            {
                Bit4_HDN7_Ice_Tray_Ster_State = CLEAR;
            }
        }
        else if( gu8_ice_ster_mode == STER_MODE_DRAIN_INPUT_STATE )
        {
            /*..hui [25-4-8���� 9:54:41] ���� Ʈ���� �Լ���..*/
            if( gu8_drain_input_ster_step >= 1 && gu8_drain_input_ster_step <= 5 )
            {
                Bit4_HDN7_Ice_Tray_Ster_State = SET;
            }
            else
            {
                Bit4_HDN7_Ice_Tray_Ster_State = CLEAR;
            }
        }
        else
        {
            Bit4_HDN7_Ice_Tray_Ster_State = CLEAR;
        }
    }
    else
    {
        Bit4_HDN7_Ice_Tray_Ster_State = CLEAR;
    }

    if (u8HotDrainNOS7ValveOFF > 0)
    {
        pVALVE_HOT_DRAIN = CLEAR;      /*off*/
    }
    else
    {
        if (u8HoTDrainNOS7ValveON > 0)
        {
            pVALVE_HOT_DRAIN = SET;    /*on*/
        }
        else
        {
            pVALVE_HOT_DRAIN = CLEAR;  /*off*/
        }
    }
/***********************************************************************************************/

}



/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/



