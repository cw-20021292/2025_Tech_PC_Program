/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _HOT_WATER_FILLING_H_
#define _HOT_WATER_FILLING_H_

extern void Cal_HeaterWatt(void);
extern void run_init_flow(void);
extern void run_decrease_flow( U16 mu16_move );
extern void run_increase_flow( U16 mu16_move );

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






extern TYPE_BYTE          U8OperationB;
#define            u8Operation                             U8OperationB.byte
#define            Bit0_Cold_Operation_Disable_State       U8OperationB.Bit.b0
#define            Bit1_Hot_Operation_Disable_State        U8OperationB.Bit.b1
#define            Bit2_Ice_Operation_Disable_State        U8OperationB.Bit.b2


extern TYPE_LONG          U32ShowPopupFunctionD;
#define            u32ShowPopupFunctions                          U32ShowPopupFunctionD.dward
#define            Bit0_Prepare_Cold_Water_Popup_State            U32ShowPopupFunctionD.Bit.b0
#define            Bit1_Prepare_Hot_Water_Popup_State             U32ShowPopupFunctionD.Bit.b1
#define            Bit2_Low_Water_Popup_State                     U32ShowPopupFunctionD.Bit.b2
#define            Bit3_Hot_Lock_Push_Popup_State                 U32ShowPopupFunctionD.Bit.b3
#define            Bit4_Hot_Lock_Setting_ON_Popup_State           U32ShowPopupFunctionD.Bit.b4
#define            Bit5_Hot_Lock_Setting_OFF_Popup_State          U32ShowPopupFunctionD.Bit.b5
#define            Bit6_COLD_OFF_Push_Popup_State                 U32ShowPopupFunctionD.Bit.b6
#define            Bit7_COLD_Setting_ON_Popup_State               U32ShowPopupFunctionD.Bit.b7
#define            Bit8_COLD_Setting_OFF_Popup_State              U32ShowPopupFunctionD.Bit.b8
#define            Bit9_ALL_LOCK_Push_Popup_State                 U32ShowPopupFunctionD.Bit.b9
#define            Bit10_ALL_LOCK_Setting_ON_Popup_State          U32ShowPopupFunctionD.Bit.b10
#define            Bit11_ALL_LOCK_Setting_OFF_Popup_State         U32ShowPopupFunctionD.Bit.b11
#define            Bit12_Filter_Cover_Open_Popup_State            U32ShowPopupFunctionD.Bit.b12
#define            Bit13_Filter_Fix_ReCheck_Popup_State           U32ShowPopupFunctionD.Bit.b13
#define            Bit14_Filter_Change_Yes_No_Popup_State         U32ShowPopupFunctionD.Bit.b14
#define            Bit15_Hurry_Up_Filter_Chenge_Popup_State       U32ShowPopupFunctionD.Bit.b15
#define            Bit16_Smart_Self_Test_Popup_State              U32ShowPopupFunctionD.Bit.b16
#define            Bit17_Auto_Drain_Popup_State                   U32ShowPopupFunctionD.Bit.b17
#define            Bit18_Memento_Popup_State                      U32ShowPopupFunctionD.Bit.b18
#define            Bit19_Faucet_Mode_5_Min_Popup_State            U32ShowPopupFunctionD.Bit.b19
#define            Bit20_Faucet_Mode_60_Min_Popup_State           U32ShowPopupFunctionD.Bit.b20










extern bit F_WaterOut;



extern U16 gu16Effluent_Hz;
extern U8 gu8_effluent_control_timer;
extern bit F_Effluent_OK;
extern U8 u8Extract_Continue;
extern U8 gu8_Flow_1sec;
extern U8 gu8TriacLevelMax;


extern bit F_FW_Version_Display_Mode;


extern bit bit_first_full_water;
extern bit bit_fill_cold_start;
extern bit bit_install_flushing_state;
extern FLUSHING_STEP gu8_flushing_mode;
extern bit bit_filter_reed;

extern bit bit_filter_flushing_start;
extern bit bit_filter_all;
extern bit bit_self_test_start;
extern U16 gu16_flushing_finish_delay_timer;
extern bit bit_flushing_halt;
extern FLUSHING_STEP gu8_flushing_mode_saved;

extern FLOW_VALVE_MODE  gu8_hot_flow_mode;
//2025-07-11 cbr
extern U8 u8Target_Hz_Hot;
extern U8 gu8_hot_setting_temperature;

//#define HOT_FILLING_START_DELAY_TIME          150
//#define HOT_FILLING_START_DELAY_TIME          100
#define HOT_FILLING_START_DELAY_TIME          50

//#define HOT_FILLING_START_STATE               0x3f
//#define HOT_FILLING_START_STATE               0x7f
#define HOT_FILLING_START_STATE               0xff


#define AUTO_FILLING                          1
#define MANUAL_FILLING                        2

#endif
