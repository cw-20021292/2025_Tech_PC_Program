/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _ICE_MAKING_H_
#define _ICE_MAKING_H_


//extern U8 CheckIceTray(void);
extern void up_tray_motor(void);
extern void down_tray_motor(void);
extern void stop_tray_motor(void);
extern void ice_init_operation(void);
extern void GasSwitchIce(void);
extern void GasSwitchHotGas(void);
extern void set_comp_rps( U8 mu8_target );


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







extern TYPE_BYTE          U8DrainWaterLevelB;
#define            u8DrainWaterLevel                       U8DrainWaterLevelB.byte
#define            Bit0_Drain_Water_Empty                U8DrainWaterLevelB.Bit.b0
#define            Bit1_Drain_Water_Low                 U8DrainWaterLevelB.Bit.b1
#define            Bit2_Drain_Water_High                U8DrainWaterLevelB.Bit.b2
#define            Bit3_Drain_Water_Error                U8DrainWaterLevelB.Bit.b3


extern TYPE_BYTE          U8WaterOutStateB;
#define            u8WaterOutState                           U8WaterOutStateB.byte
#define            Bit0_Pure_Water_Select_State                U8WaterOutStateB.Bit.b0
#define            Bit1_Cold_Water_Select_State                 U8WaterOutStateB.Bit.b1
#define            Bit2_Hot_Water_Select_State                U8WaterOutStateB.Bit.b2
#define            Bit3_Ice_Water_Select_State                U8WaterOutStateB.Bit.b3

extern TYPE_BYTE          U8MakeModeB;
#define            gu8_Make_Mode                U8MakeModeB.byte
#define            Bit0_Cold_Make_Go                U8MakeModeB.Bit.b0
#define            Bit1_Ice_Make_Go                 U8MakeModeB.Bit.b1

extern TYPE_BYTE       U8OperationB;
#define         u8Operation                                       U8OperationB.byte
#define         Bit0_Cold_Operation_Disable_State                 U8OperationB.Bit.b0
#define         Bit1_Hot_Operation_Disable_State                  U8OperationB.Bit.b1
#define         Bit2_Ice_Operation_Disable_State                  U8OperationB.Bit.b2
#define         Bit3_Ice_Tank_Ster_Operation_Disable_State        U8OperationB.Bit.b3

extern TYPE_BYTE          U8CodyInspectOperationB;
#define            u8CodyInspectOperation                      U8CodyInspectOperationB.byte
#define            Bit0_Cody_Service_Kit_Start                 U8CodyInspectOperationB.Bit.b0
#define            Bit1_Empty_Tank_Start                       U8CodyInspectOperationB.Bit.b1
#define            Bit2_Ice_Tray_Test_Start                    U8CodyInspectOperationB.Bit.b2

extern TYPE_WORD          U16CompOffW;
#define            u16CompOff                              U16CompOffW.word
#define            u8CompOff_L                             U16CompOffW.byte[0]
#define            u8CompOff_H                             U16CompOffW.byte[1]
#define            Bit0_Restart_5min_Delay_State           U16CompOffW.Bit.b0
#define            Bit1_Max_90min_CutOff_State             U16CompOffW.Bit.b1
#define            Bit2_Safty_Routine_State                U16CompOffW.Bit.b2
#define            Bit3_Ice_Making_Err_State               U16CompOffW.Bit.b3
#define            Bit4_Self_Test_COMP_Off_State           U16CompOffW.Bit.b4



extern bit F_ErrTrayMotor_DualInital;
extern U8 gu8_Room_Water_Level;
//extern bit F_HardError;
extern U16 gu16_AD_Result_Cold;
//extern bit F_Overflow;
extern bit F_Cold_Enable;
extern U16 gu16_AD_Result_Room;
extern U16 gu16_AD_Result_Eva_First;
extern U16 gu16_AD_Result_Amb;
extern bit F_SoftError;
//extern bit F_SelectBar;
//extern bit F_NoSelectBar;
extern bit F_CristalIce;
//extern bit F_IceOn;
extern bit F_Safety_Routine;
extern U8 gu8HeaterOffDelay;
//extern bit F_ErrRoomLowLEV;
extern bit F_IceOut;
//extern bit F_LeakageError;
extern bit F_FW_Version_Display_Mode;
//extern U16 gu16IceVVTime1;
//extern U8 gu8ColdWaterLEV;
extern U16 gu16IceOutOff;
//extern U16 gu16IceStopTime;
extern bit F_Ice_Tray_Down_Move_Reset;
extern U8 gu8_over_ice_melt_proc;
extern bit F_Safety_Routine;
//extern bit F_Fast_Ice_Make;
extern bit bit_first_ice_full;
extern U16 gu16CompOffDelay;
extern bit F_IceInit;
extern U16 gu16_Amb_Front_Temperature;
extern U16 gu16_Room_Temperature;
extern bit F_WaterOut;
extern U8 gu8_GasSwitch_Status;
extern bit F_Comp_Output;
extern U8 gu8_Amb_Temperature_One_Degree;
extern U8 gu8_Room_Temperature_One_Degree;
extern U8 gu8_Amb_Front_Temperature_One_Degree;
extern U8 gu8IceTrayLEV;
extern bit F_IR;
extern bit F_Low_IR;
extern bit bit_start_preheat;

extern U32 gu32_wifi_ice_make_time;
extern U32 gu32_wifi_hot_gas_time;
extern U16 gu16_wifi_tray_in_time;
extern U16 gu16_wifi_tray_in_flow;

extern bit bit_tray_pump_output;
extern U8 gu8_Tray_Temperature_One_Degree;
extern U16 gu16_uv_ice_make_time;
extern U16 gu16_tds_water_acc;

extern tsSTEPMOTOR sStepMotor;

//for test
extern bit F_IceFull;                        // �ˡ��ˡ�����u
extern bit F_IceLack;
extern bit bit_ice_size;
extern bit bit_tray_in_error_temporary;

extern bit bit_ice_out_back_state;
extern bit bit_ice_stuck_back_state;
extern bit bit_ice_mix_back_state;
extern U8 tray_error_flag_E63;
extern U16 tray_abnormal_E63_timer;
extern U8 tray_abnormal_E63_step;
extern U8 abnormal_1_repeat_cnt;

extern U8 tray_error_flag_E62;
extern U16 tray_abnormal_E62_timer;
extern U8 tray_abnormal_E62_step;
extern U8 abnormal_2_repeat_cnt;

extern U8 gu8_E62_dummy_iceheat_flag;
extern bit bit_ice_tray_1_2_uv_start;
extern bit bit_ice_tank_ster_start;
extern U32 gu32_wifi_ice_heater_timer;

extern special_func cody_water_line;

#endif
