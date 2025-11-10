/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _SYSTEM_INIT_H_
#define _SYSTEM_INIT_H_


extern void EEPROM_PRIMARY(void);
extern void TestUartStart(void);
extern void InitRtc(void);
extern void InitGasSwitch(void);
extern void init_before_save_data(void);
extern void BuzStep(U8 mu8Step);
extern void Delay_MS( U16 ms );
extern void R_TAU0_Channel0_Start(void);
extern void R_TAU0_Channel2_Start(void);
/////extern void R_TAU1_Channel0_Start(void);
extern void R_TAU1_Channel2_Start(void);

extern void R_UART0_Start(void);
extern void R_UART1_Start(void);
extern void R_UART2_Start(void);
extern void R_UART3_Start(void);
/*extern void R_INTC4_Start(void);*/
//////extern void R_INTC7_Start(void);
extern void R_INTC11_Start(void);
extern void off_all_control_led( void );
/*extern void run_init_flow(void);*/
extern void all_breath_stop(void);
extern void R_Systeminit(void);
extern void WifiControlProcess ( E_WIFI_TIME_T mu8Time );
extern void all_duty_100_percent(void);
extern void Voice_Initialize(void);
/*extern void R_INTC4_Start(void);*/
extern void initial_self_data(void);
extern void init_self_test_auto(void);
extern void down_tray_motor(void);
extern void init_ice_ster(void);
extern void diplay_test_start(void);
//////extern void init_acid_clean(void);

extern bit bit_filter_cover;
extern bit bit_filter_cover_old;
extern bit bit_filter_reed;

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


extern TYPE_WORD          U16HotTemplSelectW;
#define            U16HotTemplSelect							U16HotTemplSelectW.word
#define            U16HotTemplSelect_L							U16HotTemplSelectW.byte[0]
#define            U16HotTemplSelect_H							U16HotTemplSelectW.byte[1]
#define            Bit0_Temp_Select__45oC						U16HotTemplSelectW.Bit.b0
#define            Bit1_Temp_Select__50oC						U16HotTemplSelectW.Bit.b1
#define            Bit2_Temp_Select__55oC						U16HotTemplSelectW.Bit.b2
#define            Bit3_Temp_Select__60oC						U16HotTemplSelectW.Bit.b3
#define            Bit4_Temp_Select__65oC						U16HotTemplSelectW.Bit.b4
#define            Bit5_Temp_Select__70oC						U16HotTemplSelectW.Bit.b5
#define            Bit6_Temp_Select__75oC						U16HotTemplSelectW.Bit.b6
#define            Bit7_Temp_Select__80oC						U16HotTemplSelectW.Bit.b7
#define            Bit8_Temp_Select__85oC						U16HotTemplSelectW.Bit.b8
#define            Bit9_Temp_Select__90oC						U16HotTemplSelectW.Bit.b9
#define            Bit10_Temp_Select__95oC						U16HotTemplSelectW.Bit.b10
#define            Bit11_Temp_Select__100oC             		U16HotTemplSelectW.Bit.b11







//extern U16 gu16StepMotor1;
extern U16 gu16_IceSelect_StepMotor;
extern U16 gu16IceVVTime;
//extern U16 gu16IceWaterExt;
//extern U16 gu16IceWaterExtMax;
extern U16 gu16CompOffDelay;
extern U16 gu16IniStepDelay;
//extern U16 gu16NoWaterOut;
//extern U16 gu16IceWaterExtDelay;
//extern U8 gu8ErrRTHDelay;
//extern U8 gu8ErrCTHDelay;

//extern U8 gu8ErrHTHDelay;
//extern U8 gu8ErrETHDelay;
//extern U8 gu8ErrATHDelay;
//extern U8 gu8ErrOVCDelay;
//extern U8 gu8ErrOVHDelay;
//extern U8 gu8ErrBar;
extern U8 gu8ErrDoor;
//extern U16 gu16ErrPuriDelay;
//extern U16 gu16ErrOVHRoomTime;
//extern U16 gu16ErrColdLEVDelay;
//extern U16 ucErrOvice_Valve;
extern U16 ucErrOVice_Heater;
extern U16 ucTime_10min_cycle;
extern U16 ucErrOvice_Time;
extern bit F_Safety_Routine;
extern U8 gu8_IceHeaterControl;

//extern bit F_Room;
extern bit F_TrayMotorUP;
extern bit F_TrayMotorDOWN;
extern bit F_WaterOut;
extern bit F_IceInit;
extern bit F_IR;
extern bit F_Low_IR;
//extern bit F_Hotgas_Time;
//extern U16 Select_Delay;

extern U16 gu16_water_select_return_time;




//extern bit F_ColdIn;
extern bit F_ColdIce;
//extern U16 gu16TestTime;
extern bit F_FW_Version_Display_Mode;

extern bit F_Front_Tx_Request;
extern bit F_Welcome_Led_OnOff;
extern bit F_Cold_Enable;

extern U8 gu8_rtc_time_Y2K;       // 19 or 20
extern U8 gu8_rtc_time_DayWeek;   // 0 ~ 6
extern U8 gu8_rtc_time_Year;      // 0 ~ 99
extern U8 gu8_rtc_time_Month;     // 1 ~ 12
extern U8 gu8_rtc_time_Date;      // 1 ~ 31
extern U8 gu8_rtc_time_Hour;      // 0 ~ 23
extern U8 gu8_rtc_time_Min;       // 0 ~ 59
extern U8 gu8_rtc_time_Sec;       // 0 ~ 59

extern bit F_Circul_Drain;

extern U8 gu8_selected_led_dimming;
extern U8 gu8_unselected_led_dimming;


extern U8 gu8_wifi_weather;
extern U8 gu8_wifi_aqi;
extern U8 gu8_wifi_temperature;

extern U8 gu8_smart_image;
extern U8 gu8_smart_text;
extern bit F_Circul_Drain;
extern bit F_Cold_Enable;
extern bit bit_first_drain;

extern U8 gu8_animation_time;
extern U8 gu8_animation_duty;
extern bit bit_hot_first_op;
extern U8 gu8_altutude_setting_timeout;

extern U8 gu8_uart_test_mode;
extern U8 gu8_fota_start;
extern bit bit_temporary_no_operation;
extern U8 gu8_wifi_water_select;
extern U8 gu8_smart_total_no_use_timer_hour;
extern U8 gu8_durable_test_start;
extern bit bit_periodic_ster_enable;
extern bit bit_self_test_start;
extern U16 gu16_wifi_hot_target_time_min;
extern U16 gu16_define_filter_period__neo;
extern U16 gu16_define_filter_period__ro;
extern U16 gu16_define_filter_period__ino;
extern bit bit_acid_power_off_check;
/////extern U16 u16Min_TDS_Out_Data_AD;
extern bit bit_uv_fault_test_start;
extern U8 gu8_hk16_bright;

extern U16 gu16_Ice_Door_StepMotor;
extern U16 gu16_IceSelect_StepMotor;
extern U8 gu8_ice_tray_mode;

extern U16 gu16_WaterQuantity_half;
extern U16 gu16_WaterQuantity_one;
extern U16 gu16_WaterQuantity_two;
extern U8 gu8_hot_default_temp;
extern MY_RECEIPE my_setting[MY_INDEX_MAX];
extern ICE_LEVEL  gu8_ice_amount_step;

extern U8 left_normal_state_percent;
extern U8 right_normal_state_percent;
extern U8 setting_mode_function_extra_percent;
extern U8 setting_mode_function_main_percent;
extern U8 funtion_led_percent;
extern U8 setting_led_percent;
extern U8 water_extract_led_percent;
extern U8 bar_led_percent;
extern U8 receipe_led_percent;
extern U8 big_ster_led_percent;
extern U8 small_ster_led_percent;
extern U8 top_small_led_percent;
extern U8 middle_small_led_percent;
extern U8 setting_led_side_percent;
extern U8 receipe_led_dripcoffee_percent;
extern U8 ice_extract_outer_led_percent;
extern U8 ice_extract_inner_led_percent;
extern U8 welcome_left_led_percent;
extern U8 welcome_right_led_percent;
extern U8 colon_dot_led_percent;
extern U8 ice_type_led_percent;
#endif

