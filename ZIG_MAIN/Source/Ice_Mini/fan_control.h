/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _FAN_CONTROL_H_
#define _FAN_CONTROL_H_


//
extern TYPE_WORD          U16CompOnW;
#define            u16CompOn                               U16CompOnW.word
#define            u8CompOn_L                              U16CompOnW.byte[0]
#define            u8CompOn_H                              U16CompOnW.byte[1]
#define            Bit0_Cold_Mode_On_State                 U16CompOnW.Bit.b0
#define            Bit1_Ice_Make_On_State                  U16CompOnW.Bit.b1
#define            Bit2_Ice_Init_On_State                  U16CompOnW.Bit.b2
#define            Bit3_Ice_Tray_Melt_On_State             U16CompOnW.Bit.b3
#define            Bit4_test_State                         U16CompOnW.Bit.b4

extern bit F_Comp_Output;
extern ICE_STEP gu8IceStep;
extern U8 gu8_Amb_Temperature_One_Degree;
extern bit F_Ice_Tray_Down_Move_Reset;
extern U8 gu8_over_ice_melt_proc;
extern bit F_Safety_Routine;
extern bit bit_self_test_start;
extern bit F_FW_Version_Display_Mode;



#define FAN_DELAY_15_MIN         9000
#define FAN_DELAY_5_MIN          3000


#endif

