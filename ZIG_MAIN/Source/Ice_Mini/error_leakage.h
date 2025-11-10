/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _ERR_LEAKAGE_H_
#define _ERR_LEAKAGE_H_


//
extern void play_voice_flow_block_error_11( void );
extern void play_voice_leakage_error_12( void );
extern void Play_Voice(U16 mu16MemoryAddress);

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



extern TYPE_BYTE          U8WaterOutStateB;
#define            u8WaterOutState                           U8WaterOutStateB.byte
#define            Bit0_Pure_Water_Select_State                U8WaterOutStateB.Bit.b0
#define            Bit1_Cold_Water_Select_State                 U8WaterOutStateB.Bit.b1
#define            Bit2_Hot_Water_Select_State                U8WaterOutStateB.Bit.b2
#define            Bit3_Ice_Water_Select_State                U8WaterOutStateB.Bit.b3












/*#define LEAKAGE_DETECT_AD                         475*/
/*#define LEAKAGE_DETECT_AD                         750*/
#define LEAKAGE_DETECT_AD                           615

#define LEAKAGE_ERROR_DETECT_TIME                   30
/*#define LEAKAGE_ERROR_CLEAR_TIME                  600*/
/*#define LEAKAGE_ERROR_CLEAR_TIME                  30*/
#define LEAKAGE_ERROR_CLEAR_TIME                    600




#define TRAY_WRONG_FLOW_RETRY_COUNT                  10
#define TRAY_NORMAL_FLOW_RETRY_COUNT                 5

/*#define BLOCKED_TRAIN_IN_CHECK_TIME                  100*/
#define BLOCKED_TRAY_IN_CHECK_TIME                  1500

/*#define BLOCKED_TRAY_ADD_TIME                        300*/
/*#define BLOCKED_TRAY_ADD_TIME                        1500*/
#define BLOCKED_TRAY_CLEAR_TIME                     6000

#define FILTER_FLOW_BLOCK_DETECT_HZ_0_1_LPM          15
#define FILTER_FLOW_BLOCK_DETECT_HZ_0_2_LPM          31

/*#define FLOW_BLOCK_ERROR_CHECK_TIME                  50*/

/*#define FLOW_BLOCK_ERROR_CHECK_TIME                  600*/
#define FLOW_BLOCK_ERROR_CHECK_TIME                  200
#define FLOW_BLOCK_ERROR_CLEAR_TIME                  600

#define FLOW_LEAKAGE_ERROR_CHECK_TIME    100
#define FLOW_LEAKAGE_ERROR_CLEAR_TIME    600

#define LEAKAGE_DETECT_HZ                5
/*#define BLOCK_DETECT_HZ                  5*/
#define BLOCK_DETECT_HZ                  10




extern U16 gu16ADLeakage;
extern bit bit_tray_pump_output;
extern U8 gu8_Flow_1sec;
extern ICE_STEP gu8IceStep;
extern bit F_WaterOut;

extern U16 gu16_Ice_Tray_Fill_Hz;
extern U8 gu8_Flow_1sec;
extern FLUSHING_STEP gu8_flushing_mode;
extern U8 gu8_flushing_step;
extern bit bit_install_flushing_state;
extern U16 gu16_flushing_timer;
extern U8 gu8_flushing_operation_timer_min;
extern U8 gu8_filter_flushing_state;
extern U8 gu8_flushing_tank_step;
extern U32 gu32_tank_flushing_operation_timer;

extern bit bit_fill_cold_start;
extern bit bit_hot_filling_start;
extern COLD_TANK_FILL_STEP gu8_cold_tank_fill_step;
extern bit bit_tray_valve_output;
extern U8 gu8_Hot_Filling_Step;
extern U8 gu8_filter_flushing_step;
extern COLDTANK_FLUSHING_STEP gu8_cold_tank_flushing_step;
extern special_func cody_water_line;

#endif
