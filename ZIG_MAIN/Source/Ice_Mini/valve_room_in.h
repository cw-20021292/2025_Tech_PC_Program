/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _VALVE_ROOM_IN_H_
#define _VALVE_ROOM_IN_H_

//



extern TYPE_BYTE          U8WaterOutStateB;
#define            u8WaterOutState                           U8WaterOutStateB.byte
#define            Bit0_Pure_Water_Select_State                U8WaterOutStateB.Bit.b0
#define            Bit1_Cold_Water_Select_State                 U8WaterOutStateB.Bit.b1
#define            Bit2_Hot_Water_Select_State                U8WaterOutStateB.Bit.b2
#define            Bit3_Ice_Water_Select_State                U8WaterOutStateB.Bit.b3

extern TYPE_BYTE          U8IceOutStateB;
#define            u8IceOutState                             U8IceOutStateB.byte
#define            Bit0_Ice_Only_Select_State                U8IceOutStateB.Bit.b0
#define            Bit1_Ice_Plus_Water_Select_State          U8IceOutStateB.Bit.b1

extern TYPE_BYTE          U8IceTrayValveONB;
#define            u8IceTrayValveON                         U8IceTrayValveONB.byte
#define            Bit0_Ice_Make_Input_State                U8IceTrayValveONB.Bit.b0
#define            Bit1_Over_Ice_Melt_Input_State           U8IceTrayValveONB.Bit.b1
#define            Bit2_Tray_Flushing_State                 U8IceTrayValveONB.Bit.b2
#define            Bit3_Tray_Clean_State                    U8IceTrayValveONB.Bit.b3



extern bit F_WaterOut;
extern WATER_OUT_STEP gu8_Water_Out_Step;
extern ICE_STEP gu8IceStep;

extern special_func cody_water_line;
extern bit bit_tray_valve_output;
extern bit F_Safety_Routine;
extern bit F_Ice_Tray_Down_Move_Reset;
extern U8 gu8_over_ice_melt_proc;
extern bit bit_filter_all;


#endif
