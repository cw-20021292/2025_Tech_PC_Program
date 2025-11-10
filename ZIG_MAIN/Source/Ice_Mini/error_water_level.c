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
#include    "error_water_level.h"


void check_error_water_level(void);
void check_error_room_low_level(void);



U16 u16_water_in_check_timer_sec;
U16 u16_water_in_check_timer_min;

bit F_Err_OVF_Check;
U8 gu8ErrOVFeedCnt;
U8 gu8ErrOVRoomLEVCnt;
U8 gu8ErrOVLEVCnt;

U8 gu8OVFOnDelay;
U8 gu8OVFOffDelay;

bit bit_ovf_clear;

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/



/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/



