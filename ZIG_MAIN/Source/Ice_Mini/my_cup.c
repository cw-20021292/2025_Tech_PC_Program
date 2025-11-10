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
#include    "my_cup.h"

void my_cup_return_decision(void);

U8 gu8_my_cup_type = CLEAR;
U8 gu8_my_led_off_time = CLEAR;
bit bit_myWater_setting_start;
U16 gu16_my_recipe_menu_on_timer;
extern U8 gu8_water_out_state_old;

extern U16 gu16_water_select_return_time;
extern TYPE_BYTE          U8WaterOutStateB;
extern bit F_WaterOut_Change_State;
#define            u8WaterOutState                           U8WaterOutStateB.byte
#define            Bit0_Pure_Water_Select_State                U8WaterOutStateB.Bit.b0
#define            Bit1_Cold_Water_Select_State                 U8WaterOutStateB.Bit.b1
#define            Bit2_Hot_Water_Select_State                U8WaterOutStateB.Bit.b2
#define            Bit3_Ice_Water_Select_State                U8WaterOutStateB.Bit.b3


MY_RECEIPE my_setting[MY_INDEX_MAX];
MY_INDEX my_recipe_select;
MY_INDEX my_receipe_default;
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
#if 1
void my_cup_return_decision(void)
{
	if( bit_myWater_setting_start == CLEAR )
	{    
		gu16_my_recipe_menu_on_timer = 0;
		return;
	}
	else if(bit_myWater_setting_start == SET)
	{
		if(F_WaterOut == SET)
		{
			gu16_my_recipe_menu_on_timer = 0;
		}
		else
		{
			gu16_my_recipe_menu_on_timer ++;
		}

		if(gu16_my_recipe_menu_on_timer > 70)		// 7√ 
		{
			gu16_my_recipe_menu_on_timer = 0;
			bit_myWater_setting_start = CLEAR;
			//F_WaterOut_Change_State = SET;
			//gu16_water_select_return_time = 70;
			//u8WaterOutState = gu8_water_out_state_old;
		}
	}
}
#endif

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/



