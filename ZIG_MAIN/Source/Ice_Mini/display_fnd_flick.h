/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _DISPLAY_FND_FLICK_H_
#define _DISPLAY_FND_FLICK_H_

//

extern TYPE_BYTE          U8SetDisplayB;
#define            u8SetDisplay                         U8SetDisplayB.byte
#define            Bit0_Ice_Set_Display                 U8SetDisplayB.Bit.b0
#define            Bit1_Cold_Temp_Set_Display           U8SetDisplayB.Bit.b1
#define            Bit2_Steam_Off_Set_Display           U8SetDisplayB.Bit.b2


extern U8 gu8_error_popup_enable;
extern LED_STEP gu8_Led_Display_Step;
extern U8 gu8_uv_popup_enable;



#endif
