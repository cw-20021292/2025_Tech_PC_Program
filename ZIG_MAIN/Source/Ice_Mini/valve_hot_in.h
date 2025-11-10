/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _VALVE_HHOT_IN_H_
#define _VALVE_HHOT_IN_H_

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
extern U8 gu8_Preheat_Step;
extern bit bit_hot_filling_start;
extern U8 gu8_Hot_Filling_Step;
extern bit F_First_Hot_Effluent;

extern U8 gu8_Hot_Preheat_Temp;											// ¢®U2025-06-02 Phil
extern U8 gu8_preheat_Closed_max_timer;							// ¢®U2025-06-02 Phil
extern U8 gu8_Hot_Heater_Temperature_One_Degree;		// ¢®U2025-06-02 Phil
extern U16 u16Heater_Power_Save;										// ¢®U2025-06-02 Phil
extern U8 gu8_hot_setting_temperature;							// ¢®U2025-06-02 Phil

extern U8 gu8_Hot_In_Temperature_One_Degree;        //2025-06-05 20:43:54 cbr

extern bit bit_ice_tank_ster_start;
extern ICE_STER_MODE gu8_ice_ster_mode;
extern U8 gu8_preheat_ster_step;
extern U8 gu8_hot_input_ster_step;
extern U8 gu8_hot_wait_step;
extern U16 gu16_hot_wait_timer;
//2025-07-15 cbr
extern U8 gu8_hot_filling_max_hz;
extern U8 u8Target_Hz_Hot;
extern U8 gu8_drain_preheat_ster_step;
extern U8 gu8_drain_input_ster_step;
#endif
