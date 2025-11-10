/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _RXTRACT_CONTROL_H_
#define _RXTRACT_CONTROL_H_

//

extern void BuzStep(U8 mu8Step);
//extern void stop_hot_preheating(void);
//extern void hot_extract_return(void);
extern void play_melody_warning_197( void );
extern void play_melody_setting_on_198( void );
extern void play_melody_setting_off_199( void );
extern void play_melody_warning_197( void );
extern void play_melody_setting_on_198( void );
extern void play_melody_setting_off_199( void );
extern void play_melody_extract_complete_194( void );
extern void ice_extraction_finish(void);
extern void cold_effluent_hz(void);
extern void ice_extraction_finish(void);
extern U8 preheat_water(void);
extern void Play_Voice(U16 mu16MemoryAddress);
extern void ice_water_effluent_hz(void);
extern void effluent_hz_decision(void);


extern TYPE_BYTE          U8WaterOutStateB;
#define            u8WaterOutState                           U8WaterOutStateB.byte
#define            Bit0_Pure_Water_Select_State                U8WaterOutStateB.Bit.b0
#define            Bit1_Cold_Water_Select_State                 U8WaterOutStateB.Bit.b1
#define            Bit2_Hot_Water_Select_State                U8WaterOutStateB.Bit.b2
#define            Bit3_Ice_Water_Select_State                U8WaterOutStateB.Bit.b3



extern TYPE_LONG       U32ControlErrorsD;
#define         u32ControlErrors                                    U32ControlErrorsD.dward
#define         Bit0_Hot_Water_Flow_Block_Error__E08              	U32ControlErrorsD.Bit.b0    //E08
#define         Bit1_Main_Water_Flow_Leaked_Error__E02              U32ControlErrorsD.Bit.b1    //E02
#define         Bit2_Room_Temp_Open_Short_Error__E42                U32ControlErrorsD.Bit.b2    //E42
#define         Bit3_Leakage_Sensor_Error__E01                      U32ControlErrorsD.Bit.b3    //E01

#define         Bit4_Hot_In_Temp_Open_Short_Error__E46              U32ControlErrorsD.Bit.b4    //E46
#define         Bit5_Hot_Heater_OverHeat_Error__E40		            U32ControlErrorsD.Bit.b5    //E40
#define         Bit6_Main_Water_Flow_Block_Error__E09               U32ControlErrorsD.Bit.b6    //E09
#define         Bit7_BLDC_Communication_Error__E27                  U32ControlErrorsD.Bit.b7    //E27

#define         Bit8_BLDC_Operation_Error_Current_Sensing__E81      U32ControlErrorsD.Bit.b8    //E81
#define         Bit9_BLDC_Operation_Error_Starting_Fail__E82        U32ControlErrorsD.Bit.b9    //E82
#define         Bit10_BLDC_Operation_Error_Over_Current__E83        U32ControlErrorsD.Bit.b10   //E83
#define         Bit11_BLDC_Operation_Error_Overheat__E84            U32ControlErrorsD.Bit.b11   //E84

#define         Bit12_BLDC_Operation_Error_Disconnection_Comm__E85  U32ControlErrorsD.Bit.b12   //E85
#define         Bit13_BLDC_Operation_Error_Abnormal_Voltage__E86    U32ControlErrorsD.Bit.b13   //E86
#define         Bit14_Cold_Temp_Open_Short_Error__E44               U32ControlErrorsD.Bit.b14   //E44
#define         Bit15_Amb_Temp_Open_Short_Error__E43                U32ControlErrorsD.Bit.b15   //E43

#define         Bit16_Drain_Pump_Error__E60                         U32ControlErrorsD.Bit.b16   //E60
#define         Bit17_Tray_Micro_SW_Dual_Detect_Error__E61          U32ControlErrorsD.Bit.b17   //E61
#define         Bit18_Tray_Micro_SW_Up_Move_Error__E62              U32ControlErrorsD.Bit.b18   //E62
#define         Bit19_Tray_Micro_SW_Down_Move_Error__E63            U32ControlErrorsD.Bit.b19   //E63

#define         Bit20_Hot_Out_Temp_Open_Short_Error__E47	        U32ControlErrorsD.Bit.b20   //E52
#define         Bit21_Amb_Side_Temp_Open_Short_Error__E53           U32ControlErrorsD.Bit.b21   //E53
#define         Bit22_Hot_Heater_Temp_Open_Short_Error__E48		    U32ControlErrorsD.Bit.b22   //E48

#define         Bit23_faucet_UV_Error__E77                  		U32ControlErrorsD.Bit.b23   //E77
#define         Bit24_Ice_Tank_1_2_UV_Error__E75					    U32ControlErrorsD.Bit.b24   //E75
#define         Bit25_Ice_Faucet_UV_1_Error__E78                    U32ControlErrorsD.Bit.b25   //E78
#define         Bit26_Ice_Faucet_UV_2_Error__E78                    U32ControlErrorsD.Bit.b26   //E78
#define         Bit27_Ice_Tank_3_UV_Error__E75                	U32ControlErrorsD.Bit.b27   //E79
#define         Bit28_Ice_Tray_1_2_UV_Error__E76                	U32ControlErrorsD.Bit.b28   //E76




extern TYPE_BYTE          U8IceOutStateB;
#define            u8IceOutState                             U8IceOutStateB.byte
#define            Bit0_Ice_Only_Select_State                U8IceOutStateB.Bit.b0
#define            Bit1_Ice_Plus_Water_Select_State          U8IceOutStateB.Bit.b1













extern U8 gu8_Test_dbg_drain_Mode;


extern bit F_WaterOut;
extern U16 gu16Water_Extract_Timer;
extern U16 u16Efluent_Time;
extern bit F_ColdConty;


extern U8 u8Extract_Continue;
extern bit F_WaterOut_Disable_State;
extern U8 gu8_Flow_1sec;


extern U8 gu8_Room_Water_Level;
extern bit F_Tank_Cover_Input;

extern bit bit_first_adjust;
extern U8 gu8_hot_setting_temperature;

extern U8 gu8_hot_45_moving_finish;
extern bit bit_nos_output;
extern bit bit_feed_output;
extern CUP_LEVEL gu8Cup_level;
extern bit F_FW_Version_Display_Mode;
extern bit F_IceOut;
extern bit F_IceOpen;
extern U16 gu16_IceSelect_StepMotor;
extern U8 gu8_ice_out_continue;
extern U8 gu8_Preheat_Step;
extern U16 gu16Extracted_Hz;
extern bit F_IceBreak_Motor_Out;
extern bit F_IceBreak;
extern bit F_IceSelect;
extern U8 gu8_Water_Select_Backup;
extern U16 gu16_Ice_Door_StepMotor;

extern ICE_LEVEL gu8_ice_amount_step;
extern U8 gu8_ice_out_continue;

extern bit F_Low_IR;
extern bit bit_tray_in_error_temporary;
extern U8 gu8_clock_hide_timer;
extern U8 gu8_clock_hide_state;
extern U8 gu8_dripcoffee_percent;
extern U8 gu8_cup_led_select;
extern U8 gu8_cup_led_off_time;
extern DISPLAY_STACK Extract_Stack;

#define HOT_FIRST_MIN_HZ    20


#define USER_WATER_MAX_COUNT           20
/*#define FEED_WATER_TIME_1_MIN          600*/
/*#define FEED_WATER_TIME_1_MIN          800*/
/*#define FEED_WATER_TIME_1_MIN          700*/
#define FEED_WATER_TIME_1_MIN          500




#define USER_LOW_WATER_MAX_COUNT       15

//heater ¨ùO¨ùO ¢¬¡¤¡¾a A¡×C¨ª ©öe¨¬e ¨öA¡Æ¡Ì ¢¥A¢¬©÷
#define VALVE_CONTROL_TIME_FOR_DEBUG            1
#define VALVE_PRESSURE_PREVENT_TIME             3   // 300ms
#define VALVE_ICE_WATER_OUT_TIME                30  // 3S
//#define VALVE_CONTROL_TIME_FOR_DEBUG             10


#define ICE_EXTRACT_TIME__10_EA        70 // 25 // ³Ê¹« ÂªÀ½

// #define BUFFER_DRAIN_TIME			30
#define BUFFER_AIR_VENT_TIME		5
#define DRAIN_REMAIN_WATER_TIME     22

#define BUFFER_REMOVE_AIR_TIME  10

#define BUFFER_DRAIN_TIME					10			// ¡Ú2025-06-02 Phil
#define BUFFER_DRAIN_TIME_FOR_HIGH_TEMP 	30			// ¡ÚU2025-06-11 Phil
extern U8 gu8_hot_filling_max_hz;							// ¢®U 2025-06-10 Phil
extern U8 u8Target_Hz_Hot; 									// ¢®U 2025-06-10 Phil
extern bit bit_Hot_InLowTemp_SetHighTemp;					// ¢®U2025-06-11 Phil
extern U8 gu8Pre_hot_setting_temperature;
extern U16 gu16Effluent_Hz;
extern bit bit_Hot_InLowTemp_SetHighTemp;
extern U16 gu16_water_select_return_time;
extern bit F_hot_flow_error;           //cbr_test
extern bit bit_cooling_complete_5degree;
extern bit bit_normal_ice_out_start;
extern bit bit_ice_out_back_1s_state;
extern U16 gu16_ice_extract_timer_100ms;
extern U8 gu8_ice_feeder_shake;
extern U8 gu8IceOutCCWInterval;
extern bit F_IceSelectClose;
#endif
