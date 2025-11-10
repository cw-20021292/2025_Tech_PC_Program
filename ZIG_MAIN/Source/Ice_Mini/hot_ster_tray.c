
#include    "Macrodriver.h"
#include    "Global_Variable.h"
#include    "Port_Define.h"
#include    "hot_ster_tray.h"

void hot_ster_control(void);



HOT_STER_STEP gu8_hot_ster_step;
U16 gu16_hot_ster_timer;
bit F_Hot_Ster_ON;

// ¹Ì»ç¿ë
void hot_ster_control()
{
	switch(gu8_hot_ster_step)
	{
		case HOT_STER_STEP_STANDBY:
			if(gu8_tray_hot_ster_wait_flag == SET)
			{
				gu16_hot_ster_timer++;
				if(gu16_hot_ster_timer > 10)
				{
					gu16_hot_ster_timer = 0;
					gu8_hot_ster_step = HOT_STER_STEP_WAIT_TRAY_FULL;
				}
			}
			else
			{
			}
			break;

		case HOT_STER_STEP_VALVE_ON:
			gu16_hot_ster_timer++;
			if(gu16_hot_ster_timer > 10)
			{
				gu16_hot_ster_timer = 0;
				gu8_hot_ster_step++;
			}
			break;

		case HOT_STER_STEP_WAIT_TRAY_FULL:
			gu16_hot_ster_timer++;
			if(gu16_hot_ster_timer > 1800) /// 3minute
			{
				gu16_hot_ster_timer = 0;
				gu8_hot_ster_step = HOT_STER_STEP_TRAY_DOWN;
			}
			break;

		case HOT_STER_STEP_TRAY_DOWN:
			gu16_hot_ster_timer++;
			if(gu16_hot_ster_timer > 1)
			{
				down_tray_motor();
				gu16_hot_ster_timer = 0;
				gu8_hot_ster_step++;
			}
			break;

		case HOT_STER_STEP_FINISH:
			gu16_hot_ster_timer++;
			if(gu16_hot_ster_timer > 50) // 5seconds
			{
				gu16_hot_ster_timer = 0;
				gu8_tray_hot_ster_wait_flag = CLEAR;
				gu8_hot_ster_step = HOT_STER_STEP_STANDBY;
			}
			break;

		default:
			break;
	}
}

