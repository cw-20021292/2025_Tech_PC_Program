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
#include    "cds.h"


bit F_Sleep;                      // 취침선택

U16 gu16IceStopTime;
//----------------------------------------------------// CDS
U16 gu16Lux;
U16 gu16LuxNoUse;
bit F_IceStop;                        // 취침모드시 제빙정지


bit bit_wakeup_button;
U16 gu16_wakeup_delay_timer;

bit bit_illumi_State;
U16 gu16_led_sleep_timer;


bit bit_wake_up;


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/




