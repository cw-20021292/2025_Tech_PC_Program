/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _DISPLAY_CONTROL_H_
#define _DISPLAY_CONTROL_H_



//
extern void BuzStep(U8 mu8Step);

extern void led_display(void);
extern void set_duty_percent( U8 mu8_number, U8 mu8_percent );
extern void all_duty_100_percent(void);
extern void fnd_left_front_version(void);
extern void fnd_right_main_version(void);

extern void play_melody_select_196( void );

extern void make_ice_outer_rgb_color( U8 mu8_color );
extern void make_ice_inner_rgb_color( U8 mu8_color );
extern void make_rgb_color( U8 mu8_color );

/* Memento Here! */
extern void    memento_error_number_display(U8 U8_p_error_num);
extern void    memento_error_code_display(U8 U8_p_ErrorCode);

extern TYPE_BYTE          U8IceOutStateB;
#define            u8IceOutState                             U8IceOutStateB.byte
#define            Bit0_Ice_Only_Select_State                U8IceOutStateB.Bit.b0
#define            Bit1_Ice_Plus_Water_Select_State          U8IceOutStateB.Bit.b1

extern TYPE_BYTE          U8CodyInspectOperationB;
#define            u8CodyInspectOperation                      U8CodyInspectOperationB.byte
#define            Bit0_Cody_Service_Kit_Start                 U8CodyInspectOperationB.Bit.b0
#define            Bit1_Empty_Tank_Start                       U8CodyInspectOperationB.Bit.b1
#define            Bit2_Ice_Tray_Test_Start                    U8CodyInspectOperationB.Bit.b2





extern TYPE_BYTE          U8WaterOutStateB;
#define            u8WaterOutState                           U8WaterOutStateB.byte
#define            Bit0_Pure_Water_Select_State                U8WaterOutStateB.Bit.b0
#define            Bit1_Cold_Water_Select_State                 U8WaterOutStateB.Bit.b1
#define            Bit2_Hot_Water_Select_State                U8WaterOutStateB.Bit.b2
#define            Bit3_Ice_Water_Select_State                U8WaterOutStateB.Bit.b3



extern TYPE_BYTE          U8ButtonSetDisplayB;
#define            u8ButtonSetDisplay                   U8ButtonSetDisplayB.byte
#define            Bit0_Cold_Button_Set_Display         U8ButtonSetDisplayB.Bit.b0
#define            Bit1_Mute_Button_Set_Display         U8ButtonSetDisplayB.Bit.b1
#define            Bit2_Ice_Button_Set_Display          U8ButtonSetDisplayB.Bit.b2
#define            Bit3_Recover_Fast_Ice_Set_Display    U8ButtonSetDisplayB.Bit.b3






//extern U8 gu8_system_depth;
extern bit bit_ice_tank_uv_start;
extern U16 gu16CompOffDelay;
extern bit F_Comp_Output;
extern bit F_IceInit;
extern bit F_Tank_Cover_Input;
extern U8 gu8_Room_Water_Level;
extern U16 gu16ADCds;
extern bit F_Hot_Lock;
extern U8 gu8Cup_level_Default;

extern bit F_All_Lock;
extern U8 u8Memento_Buff[];
extern bit F_ErrTrayMotor_DualInital;

extern U8 gu8_Wifi_Pairing_State;
extern bit bit_filter_reed;
extern U8 gu8_fota_start;
extern bit bit_ice_disp_pause;

extern U8 gu8_Error_Code;
extern U8 gu8_error_popup_enable;

extern bit bit_setting_mode_start;

#define PROC_START                 0
#define PROC_1                     1
#define PROC_2                     2
#define PROC_END                   3



extern bit F_WaterOut;
extern bit F_IceOut;
extern U16 gu16_Time_Setting_Mode_Timer_100ms;
extern bit bit_time_setting_start;
extern bit bit_wifi_pairing_start;
extern bit bit_volume_setting_start;
extern bit bit_sleep_mode_enable;
extern bit bit_first_time_setting;
extern U8 gu8_rtc_time_Hour;

/* Memento Here! */
extern bit bit_memento_start;
extern U8 u8Memento_Buff[];
extern bit bit_ice_tank_ster_start;
extern bit bit_display_sleep_start;

extern bit bit_self_test_start;

extern special_func cody_water_line;
extern bit bit_filter_all;
extern bit bit_myWater_setting_start;
extern DISPLAY_STACK Extract_Stack;
extern bit bit_manual_drain_start;
extern bit bit_30_min_no_use_start;
extern U8 gu8_bright_test_mode;

extern U8 left_normal_state_percent;
extern U8 right_normal_state_percent;
extern U8 setting_mode_function_extra_percent;
extern U8 setting_mode_function_main_percent;
extern U8 funtion_led_percent;
extern U8 setting_led_percent;
extern U8 setting_led_side_percent;
extern U8 water_extract_led_percent;
extern U8 ice_extract_outer_led_percent;
extern U8 ice_extract_inner_led_percent;
extern U8 bar_led_percent;
extern U8 receipe_led_percent;
extern U8 receipe_led_dripcoffee_percent;
extern U8 big_ster_led_percent;
extern U8 small_ster_led_percent;
extern U8 top_small_led_percent;
extern U8 middle_small_led_percent;
extern U8 setting_led_side_percent;
extern U8 receipe_led_dripcoffee_percent;
extern U8 welcome_left_led_percent;
extern U8 welcome_right_led_percent;
extern U8 colon_dot_led_percent;
#endif
