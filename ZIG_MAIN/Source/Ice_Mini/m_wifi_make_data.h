/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _WIFI_MAKE_DATA_H_
#define _WIFI_MAKE_DATA_H_

//
extern void WifiSendData ( E_WIFI_DATA_T mu8Data );
extern void WifiSendDataControl ( E_WIFI_DATA_T mu8Data, U16 mu16IdData );
extern U16 GetUserSystemFunction ( U16 mu16Func );

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








extern TYPE_WORD          U16CompOnW;
#define            u16CompOn                               U16CompOnW.word
#define            u8CompOn_L                              U16CompOnW.byte[0]
#define            u8CompOn_H                              U16CompOnW.byte[1]
#define            Bit0_Cold_Mode_On_State                 U16CompOnW.Bit.b0
#define            Bit1_Ice_Make_On_State                  U16CompOnW.Bit.b1
#define            Bit2_Ice_Init_On_State                  U16CompOnW.Bit.b2
#define            Bit3_Ice_Tray_Melt_On_State             U16CompOnW.Bit.b3
#define            Bit4_test_State                         U16CompOnW.Bit.b4

extern TYPE_BYTE          U8WaterOutStateB;
#define            u8WaterOutState                           U8WaterOutStateB.byte
#define            Bit0_Pure_Water_Select_State                U8WaterOutStateB.Bit.b0
#define            Bit1_Cold_Water_Select_State                 U8WaterOutStateB.Bit.b1
#define            Bit2_Hot_Water_Select_State                U8WaterOutStateB.Bit.b2
#define            Bit3_Ice_Water_Select_State                U8WaterOutStateB.Bit.b3


extern TYPE_BYTE          U8TDS_ErrorB;
#define            gu8_TDS_Error                                             U8TDS_ErrorB.byte
#define            Bit0_No_Display_Tds_In_Temp_Open_Short_Error__E90         U8TDS_ErrorB.Bit.b0
#define            Bit1_No_Display_Tds_Out_Temp_Open_Short_Error__E91        U8TDS_ErrorB.Bit.b1
#define            Bit2_No_Display_Tds_In_Data_Error__E92                    U8TDS_ErrorB.Bit.b2
#define            Bit3_No_Display_Tds_Out_Data_Error__E93                   U8TDS_ErrorB.Bit.b3
#define            Bit4_No_Display_Tds_In_Pollution_Error__E98               U8TDS_ErrorB.Bit.b4
#define            Bit5_No_Display_Tds_Remove_Error__E99                     U8TDS_ErrorB.Bit.b5




//#define TEM_FAST_MODE_WATT                      30
///#define TEM_KEEP_MODE_WATT                      5


#define COMP_45HZ_UNDER_WATT                    8
#define COMP_60HZ_UNDER_WATT                    11
#define COMP_60HZ_OOVER_WATT                    12

#if 0
#define COMP_AMB_20_UNDER_ICE_55_WATT           15
#define COMP_AMB_20_UNDER_COLD_50_WATT          14
#define COMP_AMB_20__30_UNDER_ICE_70_WATT       19
#define COMP_AMB_20__30_UNDER_COLD_85_WATT      23
#define COMP_AMB_30_OVER_ICE_80_WATT            22
#define COMP_AMB_30_OVER_COLD_90_WATT           25
#endif

#if 0
#define COMP_AMB_20_UNDER_ICE_55_WATT           13
#define COMP_AMB_20_UNDER_COLD_50_WATT          12
#define COMP_AMB_20__30_UNDER_ICE_70_WATT       17
#define COMP_AMB_20__30_UNDER_COLD_85_WATT      21
#define COMP_AMB_30_OVER_ICE_80_WATT            20
#define COMP_AMB_30_OVER_COLD_90_WATT           23
#endif

#if 0
#define COMP_AMB_20_UNDER_ICE_55_WATT           10
#define COMP_AMB_20_UNDER_COLD_50_WATT          9
#define COMP_AMB_20__30_UNDER_ICE_70_WATT       12
#define COMP_AMB_20__30_UNDER_COLD_85_WATT      15
#define COMP_AMB_30_OVER_ICE_80_WATT            14
#define COMP_AMB_30_OVER_COLD_90_WATT           17
#endif

#if 0
#define COMP_ICE_80_WATT                        15
#define COMP_COLD_90_WATT                       17
#endif

#define COMP_ICE_80_WATT                        18
#define COMP_COLD_90_WATT                       18




/*#define HOT_HEATER_WATT_FULL                    138*/
#define HOT_HEATER_WATT_FULL                    119

/*#define MAX_COMP_WATT                           4000*/
/*#define MAX_COMP_WATT                           8000*/
#define MAX_COMP_WATT                           6000


/*#define MAX_HOT_HEATER_WATT                     42000*/
#define MAX_HOT_HEATER_WATT                     36000
/*#define MAX_TOTAL_WATT                          46000*/
/*#define MAX_TOTAL_WATT                          40000*/
/*#define MAX_TOTAL_WATT                          44000*/
#define MAX_TOTAL_WATT                          42000



#define MAX_ICE_OUT_TIME                        90000




extern bit F_WaterOut;
extern U8 gu8_Error_Code;
extern bit bit_filter_reed;
extern bit F_IceOut;
extern bit F_Comp_Output;
extern bit F_IceFull;
extern ICE_STEP gu8IceStep;
extern bit bit_9_hour_no_use_start;

extern U8 gu8_bldc_target_hz;
extern bit bit_uv_ice_tank_out;
extern bit bit_uv_ice_tray_out;
extern U8 gu8_hot_setting_temperature;
extern U8 gu8_Mixing_Out_Temperature_One_Degree;
extern bit F_Tank_Cover_Input;
//extern bit F_Heater_Output;
extern FLUSHING_STEP gu8_flushing_mode;
extern POWER_SAVING_STATE gu8_smart_operation_mode;
extern bit bit_filter_all;
extern bit bit_neo_filter_1_reed;
extern bit bit_ro_filter_2_reed;
extern bit bit_ino_filter_3_reed;

extern U8 gu8_Amb_Temperature_One_Degree;
extern U8 gu8_GasSwitch_Status;
extern bit bit_sleep_mode_start;


extern bit bit_neo_filter_1_alarm;
extern bit bit_ro_filter_2_alarm;
extern bit bit_ino_filter_3_alarm;

extern bit bit_self_test_start;
extern U8 gu8_self_test_manual_step;
extern U8 gu8_component_test_step;
extern U16 gu16_current_check_timer;
extern bit bit_flushing_water_start;
extern LED_STEP gu8_Led_Display_Step;
extern bit bit_ice_tray_making_enable;

extern bit bit_bldc_operation_error_total;
extern bit bit_uv_fault_test_start;

extern bit bit_ice_tank_ster_start;






#endif
