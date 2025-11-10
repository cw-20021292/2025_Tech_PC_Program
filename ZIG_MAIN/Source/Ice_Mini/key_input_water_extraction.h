/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _KEY_INPUT_WATER_EXT_H_
#define _KEY_INPUT_WATER_EXT_H_


extern void PCB_Test_Mode_Extract_Key_Short_Input(void);
extern void auto_drain_key_off_check(void);
//extern void power_saving_init(void);
extern void power_saving_init(void);
extern void BuzStep(U8 mu8Step);
extern void start_all_lock_flick(void);
extern void stop_button_set_display( void );
extern void start_breath_steam_clean(void);
//extern void start_hot_preheating(void);
//extern void stop_hot_preheating(void);
extern void finish_filter_alarm(void);
extern void calc_mix_water(void);
extern void Pure_Temp_Compensation_time_Cold_Big( void );
extern void water_out_compensation( void );
//extern void hot_extract_return(void);
extern void start_low_water_flick(void);
extern void stop_cooling_breath(void);
extern void play_melody_warning_197( void );
extern void play_melody_select_196( void );
extern void play_melody_setting_off_199( void );
extern void play_melody_continue_extract_start_192( void );
extern void play_melody_normal_extract_start( void );
extern void play_melody_extract_complete_194( void );
extern void play_voice_all_lock_select_103( void );
extern void finish_pairing_screen(void);
extern void off_all_flick(void);
extern void key_input_ice_extract(void);
extern void ice_extraction_start(void);
extern void ice_extraction_finish(void);
extern void play_voice_test_mode_151( void );
extern void play_voice_filter_cover_open_3( void );
extern void Play_Voice(U16 mu16MemoryAddress);
extern void cup_select_init(void);
extern void play_voice_filter_not_detected_14(void);
extern void stop_cold_off_flick(void);
extern void init_water_extract_display(void);
extern void cody_water_clean_mode_clear(void);
extern void start_all_lock_flick(void);
extern void play_voice_ice_tray_hot_ster_is_going_86(void);
extern void play_voice_filter_cover_opened_44(void);
extern void play_voice_hot_lcok_hot_select_93( void );
extern TYPE_BYTE          U8TrayPumpONB;
#define            u8TrayPumpON                              U8TrayPumpONB.byte
#define            Bit0_Ice_Tray_Water_Input_State          U8TrayPumpONB.Bit.b0
#define            Bit1_Safty_Ice_Melt_State                U8TrayPumpONB.Bit.b1

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


/*******************************************************************************************************************/
extern TYPE_BYTE          U8IceOutStateB;
#define            u8IceOutState                             U8IceOutStateB.byte
#define            Bit0_Ice_Only_Select_State                U8IceOutStateB.Bit.b0
#define            Bit1_Ice_Plus_Water_Select_State          U8IceOutStateB.Bit.b1







/*..hui [20-2-3���� 10:57:20] �ۼ�Ʈ ����� ���� 300ms ���� ���������..*/
#define EXTRACT_TIME_CONTINUE                        1800 // 903
#define EXTRACT_TIME_CONTINUE_LINE_TEST              6000



/*..hui [20-5-11���� 4:00:55] 5������ ����..*/
#define EXTRACT_TIME_COLD_DRAIN                      3003


extern TYPE_BYTE          U8WaterOutStateB;
#define            u8WaterOutState                           U8WaterOutStateB.byte
#define            Bit0_Pure_Water_Select_State                U8WaterOutStateB.Bit.b0
#define            Bit1_Cold_Water_Select_State                 U8WaterOutStateB.Bit.b1
#define            Bit2_Hot_Water_Select_State                U8WaterOutStateB.Bit.b2
#define            Bit3_Ice_Water_Select_State                U8WaterOutStateB.Bit.b3

extern TYPE_BYTE          U8LedFlickerStateB;
#define            u8LedFlickerState                           U8LedFlickerStateB.byte
#define            Bit0_Ice_Lock_LED_Flick                U8LedFlickerStateB.Bit.b0
#define            Bit1_Ice_Lock_Extract_LED_Flick                U8LedFlickerStateB.Bit.b1
#define            Bit2_Hot_Water_Lock_LED_Flick                U8LedFlickerStateB.Bit.b2
#define            Bit3_All_Lock_Water_Extract_LED_Flick                U8LedFlickerStateB.Bit.b3






extern TYPE_BYTE          U8PairingStepB;
#define            u8PairingStep                            U8PairingStepB.byte
#define            Bit0_Pairing_Step0_PrePare               U8PairingStepB.Bit.b0
#define            Bit1_Pairing_Step1_25_Percent            U8PairingStepB.Bit.b1
#define            Bit2_Pairing_Step2_50_Percent            U8PairingStepB.Bit.b2
#define            Bit3_Pairing_Step3_75_Percent            U8PairingStepB.Bit.b3
#define            Bit4_Pairing_Step_FINISH                 U8PairingStepB.Bit.b4
#define            Bit5_Pairing_Exit                        U8PairingStepB.Bit.b5
#define            Bit6_Pairing_Error                       U8PairingStepB.Bit.b6
#define            Bit7_Pairing_Forced_Exit                 U8PairingStepB.Bit.b7



extern bit F_WaterOut;
//extern bit F_Sterilization;
//extern bit F_SterProhibit;
extern bit F_LineTest;
//extern bit F_NfcTest;

extern U8 u8PcbTestEnableFlag;
extern bit F_FW_Version_Display_Mode;

extern bit F_All_Lock;
extern U8 gu8_all_lock_flick_tmr;
extern bit F_auto_drain_off_check;
//extern bit F_ServiceSet;

extern bit F_WaterOut_Disable_State;

extern bit F_Effluent_OK;

extern U8 gu8_Room_Water_Level;
extern U16 gu16_display_effnt_hz;


extern bit F_Tank_Cover_Input;
extern CUP_LEVEL gu8Cup_level;
extern U16 gu16Water_Extract_Timer;
extern U16 gu16_water_select_return_time;
/*extern bit F_HotWaterInit;*/

extern U16 gu16IceMakeTime;
extern bit F_IceInit;
extern LED_STEP gu8_Led_Display_Step;
extern bit bit_welcome_start;
extern U16 gu16Effluent_Hz;
extern U16 gu16_total_extract_amount;
extern bit F_IceOut;
extern U8 gu8_ice_out_continue;
extern U16 gu16_ice_select_return_time;
extern U8 gu8_water_ext_test_input;

extern U8 gu8_error_popup_enable;
extern U8 gu8_error_code;
extern bit bit_manual_drain_start;
extern U8 gu8_clock_hide_timer;
extern U8 gu8_clock_hide_state;
extern bit bit_myWater_setting_start;
extern U8 gu8_hot_setting_temperature;
extern U8 gu8_hot_default_temp;
extern MY_RECIPE_INDEX my_recipe_select;
extern bit bit_filter_cover;
extern U8 gu8_clear_hot_block_error;
extern bit bit_volume_setting_start;
extern bit bit_install_flushing_state;
extern bit bit_ice_tank_ster_start;
extern bit bit_flushing_halt;
extern bit bit_yes_no_popup;

extern bit bit_sleep_mode_enable;
extern bit bit_first_time_setting;
extern U8 gu8_rtc_time_Hour;
extern bit bit_filter_all;
extern U8 gu8_filter_flushing_state;

extern MY_RECEIPE my_setting[MY_INDEX_MAX];
extern MY_INDEX my_recipe_select;

extern bit bit_30_min_no_use_start;
extern U32 gu32_no_use_30_min_timer;
extern bit bit_display_sleep_start;
extern bit bit_sleep_mode_start;
extern bit bit_self_test_start;
extern U8 display_test_flick_timer_100ms;
/* Cody Water Line Clean Service */
extern special_func cody_water_line;
extern bit bit_time_setting_start;
extern U16 drip_timer;
extern bit F_Hot_Lock;
extern U8 gu8_fota_start;
extern bit F_First_Hot_Effluent;
extern DISPLAY_STACK Extract_Stack;
extern bit bit_memento_start;
extern U16 gu16_effluent_hot;   //2025-07-31 cbr
extern U16 gu16_filter_flushing_timer;
extern U16 gu16_filter_flushing_timer_saved;
extern bit bit_wifi_pairing_start;
extern LED_STEP gu8_Led_Display_Step;
extern U8 gu8_ice_feeder_shake;
extern U8 gu8_ice_feeder_shake_time;
#endif
