/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _KEY_HANDLING_LONG_H_
#define _KEY_HANDLING_LONG_H_

extern void water_select_return_timer_control( U8 mu8_flag );
extern void setting_buzzer( U8 mu8_setting );
extern void play_voice_setting_hot_lock_on_112( void );
extern void play_voice_setting_hot_lock_off_113( void );
extern void play_voice_setting_ice_on_149( void );
extern void play_voice_setting_ice_off_150( void );
extern void play_voice_setting_cold_on_124( void );
extern void play_voice_setting_cold_off_125( void );
extern void play_melody_select_196( void );
extern void play_melody_setting_on_198( void );
extern void play_melody_warning_197( void );
extern void start_time_setting_mode( U8 mu8_mode );
extern void play_voice_time_setting_start_144( void );
extern void play_voice_all_lock_finish_128();
extern void send_wifi_water_select_data_control( U8 mu8_data );
extern void send_wifi_each_data_control(U8 mu8_data);
extern void start_ice_tank_ster(void);
extern void stop_ice_tank_ster(void);
extern void play_melody_setting_off_199( void );
extern void play_voice_test_mode_151( void );
extern void play_voice_all_lock_select_103( void );
extern U8 special_mode_check_U8(void);
extern void cancel_filter_flushing(void);
extern void play_voice_time_setting_finish_145( void );
/* Memento Here! */
extern void memento_display_step_conv(DISPLAY_MEMENTO_STEP U8_p_step);
extern void start_button_set_display( U8 mu8_mode );
extern void cody_water_clean_mode_clear(void);
extern TYPE_BYTE          U8LedFlickerStateB;
#define            u8LedFlickerState                           U8LedFlickerStateB.byte
#define            Bit0_Ice_Lock_LED_Flick                U8LedFlickerStateB.Bit.b0
#define            Bit1_Ice_Lock_Extract_LED_Flick                U8LedFlickerStateB.Bit.b1
#define            Bit2_Hot_Water_Lock_LED_Flick                U8LedFlickerStateB.Bit.b2
#define            Bit3_All_Lock_Water_Extract_LED_Flick                U8LedFlickerStateB.Bit.b3

extern TYPE_BYTE          U8WaterOutStateB;
#define            u8WaterOutState                           U8WaterOutStateB.byte
#define            Bit0_Pure_Water_Select_State                U8WaterOutStateB.Bit.b0
#define            Bit1_Cold_Water_Select_State                 U8WaterOutStateB.Bit.b1
#define            Bit2_Hot_Water_Select_State                U8WaterOutStateB.Bit.b2
#define            Bit3_Ice_Water_Select_State                U8WaterOutStateB.Bit.b3



extern TYPE_BYTE       U8OperationB;
#define         u8Operation                                       U8OperationB.byte
#define         Bit0_Cold_Operation_Disable_State                 U8OperationB.Bit.b0
#define         Bit1_Hot_Operation_Disable_State                  U8OperationB.Bit.b1
#define         Bit2_Ice_Operation_Disable_State                  U8OperationB.Bit.b2
#define         Bit3_Ice_Tank_Ster_Operation_Disable_State        U8OperationB.Bit.b3


extern TYPE_WORD          U16ButtonIndicatorB;
#define            gu8_bitton_indicator                        U16ButtonIndicatorB.word
#define            gu8_button_indicator_L                      U16ButtonIndicatorB.byte[0]
#define            gu8_button_indicator_H                      U16ButtonIndicatorB.byte[1]
#define            Bit0_Ice_Lock_Indicator                     U16ButtonIndicatorB.Bit.b0
#define            Bit1_Hot_Lock_Indicator                     U16ButtonIndicatorB.Bit.b1
#define            Bit2_Cold_Off_Indicator                     U16ButtonIndicatorB.Bit.b2
#define            Bit3_Hot_Off_Indicator                      U16ButtonIndicatorB.Bit.b3
#define            Bit4_All_Lock_Indicator                     U16ButtonIndicatorB.Bit.b4
#define            Bit5_Ice_Off_Indicator                      U16ButtonIndicatorB.Bit.b6
#define            Bit6_Icewater_Off_Indicator                 U16ButtonIndicatorB.Bit.b7
#define            Bit7_water_extract_Indicator                U16ButtonIndicatorB.Bit.b8

extern TYPE_BYTE          U8FilterResetStateB;
#define            u8FilterResetState                            U8FilterResetStateB.byte
#define            Bit0_Neo_Filter_1_Reset_State                 U8FilterResetStateB.Bit.b0
#define            Bit1_Ro_Filter_2_Reset_State                  U8FilterResetStateB.Bit.b1
#define            Bit2_Ino_Filter_3_Reset_State                 U8FilterResetStateB.Bit.b2



extern TYPE_BYTE          U8TimeSettingLongKeyB;
#define            u8TimeSettingLongKey                        U8TimeSettingLongKeyB.byte
#define            Bit0_Ice_Water_Continue                           U8TimeSettingLongKeyB.Bit.b0
#define            Bit1_Ice_Continue                          U8TimeSettingLongKeyB.Bit.b1


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


extern bit F_IceOutCCW;
extern U8 gu8IceClose;
extern bit F_IceOpen;
//extern U8 gu8IceOut;
//extern U8 gu8ErrIceMotor;
extern U8 gu8IceOutCCWInterval;


//extern U16 gu16NoWaterOut;
extern bit F_NightNoUseSave;
extern bit F_DayNoUseSave;
extern bit F_NoUseSave;
//extern U16 gu16IceOutTest;
//extern bit F_Sterilization;


//extern bit F_Service;

//extern U8 gu8ServiceStep;

//extern U16 gu16WashTime;
//extern U16 gu16Conty;
//extern U8 gu8LockLEDFlick;
//extern U8 gu8ContyCancel;
//extern bit F_ColdConty;
extern U16 gu16ExtrKeyDelay;
extern U8 gu8AltitudeStep;
//extern U16 gu16NeutralTime;
extern U8 gu8AltitudeTime;
extern U8 gu8_hot_water_lock_flick_tmr;

extern bit F_WaterOut_Change_State;

extern U16 gu16_water_select_return_time;

extern bit F_ErrorSave;
extern U8 u8Memento_Buff[];

extern bit F_IceVV;
extern U16 gu16CompOffDelay;
extern bit F_IceHeater;
extern U16 gu16IceVVTime;
extern bit F_IceDoorClose;        /* ���̽� ���� ���� */

extern bit F_FW_Version_Display_Mode;
extern U8 gu8_led_version_step;
extern U16 gu16_auto_drain_start_timer_sec;
extern U16 gu16_auto_drain_start_timer_min;
extern U16 gu16_auto_drain_start_timer_hour;

extern bit F_Cold_Water_VeryLongKey_Disable;
extern bit F_Hot_Water_VeryLongKey_Disable;
//extern bit F_HardError;
extern bit F_DrainStatus;
extern bit F_Key_Very_Long_Push_State;
extern bit F_Cold_Enable;
extern U8 gu8Cup_level_Default;






extern bit F_Cody_Inspection;

extern U8 gu8_cold_setting_level;
extern U8 gu8_cup_led_select;
//extern U8 gu8_cup_select_time_init;
extern U8 gu8_user_led_select;
extern bit bit_date_setting_confirm;
extern bit bit_time_setting_confirm;




extern CUP_LEVEL gu8Cup_level;

extern bit F_Welcome_Led_OnOff;

extern bit F_Wifi_Tx_Condition;


extern bit F_All_Lock;
extern bit F_WaterOut;
extern bit F_Mute_Enable;
extern bit F_Hot_Enable;
extern bit F_IceOut;
extern bit F_Hot_Lock;
extern bit bit_debug_mode_start;

extern LED_STEP gu8_Led_Display_Step;
extern bit F_IceOn;
extern bit F_IR;
extern bit F_Low_IR;
extern U8 gu8_cup_level_time_init;
extern bit F_Comp_Output;

extern bit bit_time_setting_start;
extern U8 gu8_Wifi_Connect_State;
extern U8 gu8_altutude_setting_timeout;

extern FLUSHING_STEP gu8_flushing_mode;
extern bit F_IceSelectDoorClose;
extern bit bit_child_lock_enable;
extern bit bit_temporary_release;
extern U8 gu8_child_release_timer;
extern bit F_Child_Lock;
extern U8 gu8Sound_Type;
extern bit bit_ice_ster_test_debug_start;
extern U8 gu8_ice_ster_test_debug_step;
extern bit bit_ct_filter_type_start;
extern bit bit_self_test_start;
extern bit bit_ct_flushing_standby_start;
extern U8 gu8_ct_forced_flushing_start;
extern U8 gu8_recover_org_fast_ice;

extern U8 gu8_Wifi_Cert_State;
extern U8 gu8_wifi_ap_mode;
extern U8 gu8_Wifi_Last_Error;
extern bit bit_wifi_pairing_start;
extern LED_STEP gu8_Led_Display_Step;
extern bit bit_display_last_error;
extern bit bit_memento_start;
extern U8 gu8_time_setting_step;
extern bit F_IceInit;
extern bit bit_ice_tank_ster_start;
extern bit F_Tank_Cover_Input;
extern bit bit_filter_all;
extern U8 gu8InitStep;
extern U16 gu16IceMakeTime;
extern U16 gu16IceHeaterTime;
extern bit F_First_Hot_Effluent;
extern bit bit_cold_setting_double_disable;
extern bit bit_hot_setting_double_disable;
extern bit bit_yes_no_popup;
extern U16 gu16_cold_off_flick_timer;
/* Memento Here! */
extern U8 gu8_memento_display_Timer;
extern U8 gu8_memento_display_count;

/* Cody Water Line Clean Service */
extern special_func cody_water_line;

extern bit bit_cold_setting_double_disable;
extern bit bit_hot_setting_double_disable;
extern bit bit_ice_setting_double_disable;
extern U8 gu8_flushing_finish_input_count;
extern U8 gu8_passive_hot_ster_key_input_count;
extern bit bit_manual_drain_start;

extern bit bit_filter_cover;
extern bit bit_volume_setting_start;
#endif

