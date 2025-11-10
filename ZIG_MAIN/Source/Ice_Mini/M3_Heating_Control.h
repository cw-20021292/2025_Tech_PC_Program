/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _HEATING_CONTROL_H_
#define _HEATING_CONTROL_H_


//extern void mix_motor_control(void);
//extern void stop_hot_preheating(void);
extern void ControlFlow(void);
extern void run_init_flow(void);
extern void run_decrease_flow( U16 mu16_move );
extern void run_increase_flow( U16 mu16_move );

extern U16 conv_hot_hz( U8 mu8_flow_hz );
extern void control_tray_ster_flow_valve(void);
extern void heating_control_tray_ster(void);
extern TYPE_BYTE          U8IceOutStateB;
#define            u8IceOutState                             U8IceOutStateB.byte
#define            Bit0_Ice_Only_Select_State                U8IceOutStateB.Bit.b0
#define            Bit1_Ice_Plus_Water_Select_State          U8IceOutStateB.Bit.b1

extern TYPE_WORD          U16WaterHeaterOnW;
#define            u16WaterHeaterOn                          U16WaterHeaterOnW.word
#define            u16WaterHeater_L                          U16WaterHeaterOnW.byte[0]
#define            u16WaterHeater_H                          U16WaterHeaterOnW.byte[1]
#define            Bit0_Pre_Heating_State                    U16WaterHeaterOnW.Bit.b0
#define            Bit1_Main_Heating_State                   U16WaterHeaterOnW.Bit.b1
#define            Bit2_First_Heating_Check_State            U16WaterHeaterOnW.Bit.b2

extern TYPE_BYTE          U8WaterOutStateB;
#define            u8WaterOutState                           U8WaterOutStateB.byte
#define            Bit0_Pure_Water_Select_State                U8WaterOutStateB.Bit.b0
#define            Bit1_Cold_Water_Select_State                 U8WaterOutStateB.Bit.b1
#define            Bit2_Hot_Water_Select_State                U8WaterOutStateB.Bit.b2
#define            Bit3_Ice_Water_Select_State                U8WaterOutStateB.Bit.b3

extern TYPE_BYTE          U8HeaterMaxLimitB;
#define            u8HeaterMaxLimit                          U8HeaterMaxLimitB.byte
#define            Bit0_Over_1st_Limit                       U8HeaterMaxLimitB.Bit.b0
#define            Bit1_Over_2nd_Limit                       U8HeaterMaxLimitB.Bit.b1
#define            Bit2_Over_Flow_Increase_One               U8HeaterMaxLimitB.Bit.b2
#define            Bit3_Over_Flow_Increase_Two               U8HeaterMaxLimitB.Bit.b3
#define            Bit4_Over_Flow_Increase_Three             U8HeaterMaxLimitB.Bit.b4
#define            Bit5_Over_Flow_Increase_Four              U8HeaterMaxLimitB.Bit.b5
#define            Bit6_Over_Flow_Increase_Five              U8HeaterMaxLimitB.Bit.b6
#define            Bit7_Under_Flow_Decrease                  U8HeaterMaxLimitB.Bit.b7

extern bit F_First_Hot_Effluent;
extern U8 gu8_drain_max_flow;
	
extern bit F_WaterOut;
extern U8 gu8_Hot_In_Temperature_One_Degree;
extern U8 gu8_Flow_1sec;
extern WATER_OUT_STEP gu8_Water_Out_Step;
extern U8 gu8_Preheat_Step;

extern U8 gu8_hot_filling_max_hz;
extern U8 gu8_hot_setting_temperature;
extern U8  u8Delta_Power_Level;
extern U8 gu8_Hot_Out_Temperature_One_Degree;
extern U8 gu8_Hot_Heater_Temperature_One_Degree;
extern U8  gu8TriacLevelMax; 
extern U8 gu8_pre_heater_temp;

extern U16 gu16_AD_Result_Heater_Current_Feed;
extern FLOW_VALVE_MODE  gu8_hot_flow_mode;

extern U16 gu16_AD_Result_Heater_Current_Feed;
extern FLOW_VALVE_MODE  gu8_hot_flow_mode;
extern U16 gu16_AD_Result_Heater_Current_Feed_Max;
extern bit bit_ice_tank_ster_start;
extern ICE_STER_MODE gu8_ice_ster_mode;
extern U8 gu8_Test_dbg_drain_Mode;
extern U16 gu16_firstEffluent_hotwater_timer;
extern bit F_firstEffluent_hotWater;

extern MY_RECEIPE my_setting[MY_INDEX_MAX];
extern MY_INDEX my_recipe_select;

extern bit F_firstEffluent_hotWater;
extern bit bit_Hot_InLowTemp_SetHighTemp;											// ¢®U2025-06-11 Phil after PM

extern U8 gu8_hot_drain_cold;

extern U8 u8Extract_Continue;   //2025-07-31 cbr
extern U16 gu16_effluent_hot;   //2025-07-31 cbr
extern U8 gu8_Hot_Filling_Step;				//2025-06-20 phil   //2025-07-11 cbr
extern FLUSHING_STEP gu8_flushing_mode;		//2025-06-20 phil   //2025-07-11 cbr

//for test
extern U8 gu8_Test_dbg_drain_Mode;
#endif
