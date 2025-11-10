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
#include    "error_feed.h"



TYPE_BYTE          U8FeedErrorCheckB;
#define            u8FeedErrorCheck                       U8FeedErrorCheckB.byte
#define            Bit0_Feed_Error_Ice_Full_State         U8FeedErrorCheckB.Bit.b0
#define            Bit1_Feed_Error_Ice_Step_State         U8FeedErrorCheckB.Bit.b1
#define            Bit2_Feed_Error_Room_Level_State       U8FeedErrorCheckB.Bit.b2
#define            Bit3_Feed_Error_Drain_Tank_State       U8FeedErrorCheckB.Bit.b3



/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/



