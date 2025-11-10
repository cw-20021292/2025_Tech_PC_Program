/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _MODE_CONTROL_H_
#define _MODE_CONTROL_H_


extern void Ice_Make_Process(void);
extern void auto_drain_control(void);
extern void ice_door_close_24_hour(void);
extern void ice_select_door_close_24_hour(void);
extern void ice_make_system_up_move_reset(void);
extern void ice_make_system_down_move_reset(void);
extern void execute_ice_tray_test(void);
extern void system_reset(void);
extern void logic_decision(void);
extern void Make_Mode_Decision(void);
extern void water_extract_control(void);
extern void check_ice_stop(void);
extern void service_check(void);
extern void cody_ice_tray_test(void);
/* Cody Water Line Clean Service */
extern void cody_water_line_clean(void);
extern void model_select(void);
extern void calculate_electric_rate(void);
extern void cody_empty_tank(void);
extern void cody_service(void);
extern void cody_takeoff_ice(void);

extern void down_tray_motor(void);
extern void my_cup_return_decision(void);
extern void calc_water_usage(void);
extern void time_setting(void);

extern void wifi_smart_control(void);
extern void wifi_operation_control(void);

extern void wifi_time_setting(void);
extern void save_mode(void);
extern void child_lock(void);
extern void manual_drain(void);
extern void water_durable_test(void);
extern void ice_extract_control(void);
extern void Set_Step_Direction(U8 direction, U16 Pulse);

extern void check_ice_system_ok();
extern void icetray_control(void);
extern void continued_extract_control(void);
extern void ice_tray_ster_control(void);

extern void uv_ice_tray_decision(void);
// extern void ice_door_extract_close(void);
extern void ice_door_close_20_min(void);
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



extern U16 gu16_Cold_Temperature;



extern U8 gu8_Room_Water_Level;
extern bit F_IceHeater;


extern U16 gu16_AD_Result_Cold;
//extern bit F_Overflow;
//extern U8 gu8ErrOVFeedCnt;
//extern U8 gu8ErrOVRoomLEVCnt;
//extern bit F_ErrOVFeed;
//extern bit F_ErrOVFeed_D;
//extern bit F_ErrOVRoomLEV;
//extern bit F_ErrOVRoomLEV_D;
//extern bit F_ErrOVLEV;
//extern bit F_ErrOVLEV_D;
//extern bit F_Conty;
extern bit F_WaterOut;
extern bit F_Cold_Enable;
extern bit F_ContyGo;

extern U16 gu16IRInterval;
extern bit F_IceFull;
//extern bit F_ColdConty;
extern bit F_IceVV;
//extern bit F_ColdDIR;
//extern bit F_ColdIn;
//extern U8 gu8ContyCancel;
extern bit F_ColdIce;
extern bit F_BarCW;
extern bit F_TrayMotorUP;
extern U16 gu16CompOffDelay;
//extern bit F_IceMakeGo;
//extern U16 Select_Delay;
//extern bit F_Hotgas_Time;
extern bit F_IceInit;
//extern bit F_ReIceInit;
extern U8 gu8IRCount;
extern U8 gu8BarCNT;
extern U8 gu8NoSBStep;

extern bit F_NoColdBar;
extern bit F_NoIceBar;
//extern U16 gu32ServiceTime;

//extern bit F_Service;
//extern bit F_ErrOVF;
extern U16 u16Efluent_Time;
extern bit F_Altitude;

extern bit F_Hot_Temp_Setting;
extern bit F_Hot_Lock;

extern U8 gu8Cup_level_Default;


//extern U16 gu16NoWaterOut;
extern bit F_Save;
//extern bit F_IceOn;
extern U16 gu16IceMakeTime;

//extern U8 Tray_Comp_Delay;
extern U16 gu16IceOutOff;
extern U16 gu16ErrIceOut;
//extern U8 gu8ErrIceMotor;


extern U16 gu16IceHeaterTime;
//extern U16 gu16ErrPuriDelay;
//extern U16 gu16IceSupplyTime;
//extern U16 gu16LimitSterTime;
extern bit Be_COMP;
extern U16 gu16ColdVVOnTime;
//extern U8 gu8DrainErrorRetrayDelay;
//extern U8 gu8ICETrayRoomDelay;
//extern U16 gu16ErrOVHRoomTime;
//extern U16 gu16MaxSterTime;
//extern U8 gu8ICELockLEDFlick;
extern U8 gu8IRTime;
//extern U8 gu8ErrBar;

extern U8 gu8IceClose;


//extern U16 gu16ErrColdLEVDelay;
//extern U16 gu16IceOutTest;

extern U8 gu8ErrDoor;
extern U8 gu8IceOutCCWInterval;

extern U8 u8_Tray_up_moving_retry_timer;
//extern U16 gu16WashTime;
//extern U16 gu16NeutralTime;
//extern U8 gu8_room_level_delay;
//extern U8 gu8ColdLEVDelay;
//extern U8 gu8IceDrainLEVDelay;


//extern U8 gu8LockLEDFlick;

//extern U8 gu8ErrEW;
//extern U8 gu8LockFlick;
extern U8 gu8HeaterOffDelay;
extern U16 gu16IceVVTime;
//extern U16 gu16IceVVTime1;
//extern U8 gu8ColdWaterLEV;
//extern bit F_CyclePumpTimeFix;
//extern bit F_CyclePumpSet;
//extern bit F_CyclePump;
//extern U16 gu16IceVVTimeSetDelay;
//extern U8 gu8CyclePumpCNT;


extern bit F_Hot_Enable;
extern U8 gu8_Error_Code;

extern U8 u8Extract_Continue;


extern bit F_ColdConty;
extern U8 gu8Cup_level_Old;


extern bit F_Ice_Make_Priority_Setting_Mode;
extern U8 gu8_ice_make_priority_settiong_time;
extern bit F_WaterOut_Disable_State;

extern U16 gu16_cody_ice_make_time;
extern U8 gu8InitStep;
extern bit F_IR;
extern bit F_Low_IR;
extern U16 gu16IR_l_Interval;
extern ICE_STEP gu8IceStep;


extern U8 gu8_Cold_Temperature_One_Degree;
extern U8 gu8_Hot_Tank_Temperature_One_Degree;

extern U8 gu8_cold_setting_level;
extern bit bit_cold_read_finish;
extern bit bit_hot_read_finish;

extern bit bit_9_hour_no_use_start;
extern bit bit_sleep_mode_start;
extern bit F_Comp_Output;

extern U8 gu8_Amb_Temperature_One_Degree;
extern U8 gu8_Amb_Side_Temperature_One_Degree;
extern U8 gu8_Amb_Front_Temperature_One_Degree;

extern U8 gu8_cooling_display_mode;

extern bit bit_bldc_operation_error_total;
extern U8 gu8IceTrayLEV;


#endif

