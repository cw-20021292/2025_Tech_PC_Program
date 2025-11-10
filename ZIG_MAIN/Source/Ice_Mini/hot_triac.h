/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _HOT_TRIAC_H_
#define _HOT_TRIAC_H_

//
extern U16 conv_hot_hz( U8 mu8_flow_hz );
extern U8 tray_ster_temp_control(void);

extern TYPE_BYTE          U8WaterOutStateB;
#define            u8WaterOutState                           U8WaterOutStateB.byte
#define            Bit0_Pure_Water_Select_State                U8WaterOutStateB.Bit.b0
#define            Bit1_Cold_Water_Select_State                 U8WaterOutStateB.Bit.b1
#define            Bit2_Hot_Water_Select_State                U8WaterOutStateB.Bit.b2
#define            Bit3_Ice_Water_Select_State                U8WaterOutStateB.Bit.b3

extern TYPE_BYTE          U8OperationB;
#define            u8Operation                             U8OperationB.byte
#define            Bit0_Cold_Operation_Disable_State       U8OperationB.Bit.b0
#define            Bit1_Hot_Operation_Disable_State        U8OperationB.Bit.b1
#define            Bit2_Ice_Operation_Disable_State        U8OperationB.Bit.b2
extern bit F_WaterOut;
extern U8 gu8_hot_setting_temperature;
extern U8 gu8_Preheat_Step;
extern U8 gu8_Hot_Out_Temperature_One_Degree;
extern U8 gu8_Hot_Target_Temp;
extern U8 gu8_Flow_1sec;
extern U8 gu8_Hot_In_Temperature_One_Degree;
extern U8 u8Heater_1stepWatt;
extern U16 u16Heater_Power_Save;
extern U8 gu8_Hot_Filling_Step;
extern bit F_First_Hot_Effluent;

extern U8 gu8_Hot_Preheat_Temp;



extern U8 gu8_Hot_Heater_Temperature_One_Degree;

extern bit F_Hot_Temp_Much_Difference_State;


extern bit bit_hot_filling_start;

extern bit bit_damaged_heater_error_once;
extern bit bit_damaged_heater_error_fixed;


extern U8 gu8_min_power;


extern WATER_OUT_STEP gu8_Water_Out_Step;



extern bit bit_self_test_start;
//extern bit bit_test_mode_flushing_stop;
extern bit bit_gallery_mode_start;

extern U8 gu8_Test_dbg_drain_Mode;
extern bit bit_ice_tank_ster_start;
extern ICE_STER_MODE gu8_ice_ster_mode;
extern FLUSHING_STEP gu8_flushing_mode;
extern U8 gu8_hot_filling_max_hz;							// ��U 2025-06-10 Phil 
extern U8 u8Target_Hz_Hot; 										// ��U 2025-06-10 Phil 

extern bit F_hot_flow_error;    //cbr_test
extern U8 gu8_hot_drain_cold;
#endif
