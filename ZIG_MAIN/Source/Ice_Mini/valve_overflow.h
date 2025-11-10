/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _VALVE_OVERFLOW_H_
#define _VALVE_OVERFLOW_H_

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



extern bit F_WaterOut;
extern WATER_OUT_STEP gu8_Water_Out_Step;
extern bit bit_filter_all;
extern ICE_STEP gu8IceStep;
extern bit bit_fill_cold_start;
extern COLD_TANK_FILL_STEP gu8_cold_tank_fill_step;
extern bit bit_install_flushing_state;
extern COLDTANK_FLUSHING_STEP gu8_cold_tank_flushing_step;
extern FLUSHING_STEP gu8_flushing_mode;
extern U8 gu8_filter_flushing_step;

extern bit bit_ice_tank_ster_start;
extern ICE_STER_MODE gu8_ice_ster_mode;
extern U8 gu8_preheat_ster_step;
extern U8 gu8_hot_input_ster_step;
extern U8 gu8_Hot_Filling_Step;
extern U8 gu8_drain_preheat_ster_step;
extern U8 gu8_drain_input_ster_step;
#endif
