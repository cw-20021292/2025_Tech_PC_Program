/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _LOGIC_DECISION_H_
#define _LOGIC_DECISION_H_

//
extern void BuzStep(U8 mu8Step);
extern void send_wifi_each_data_control( U8 mu8_data );
extern void play_melody_setting_off_199( void );




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

extern DISPLAY_STACK Extract_Stack;
extern bit F_Hot_Lock;
extern bit F_Cold_Enable;
extern bit F_WaterOut;
extern U8 gu8_Room_Water_Level;
extern bit F_WaterInit;
extern U8 gu8_debug_finish_count;
extern U16 gu16_develop_key_clear_time;
/*extern bit F_HotWaterInit;*/
extern U8 gu8_user_led_off_time;
extern U8 gu8_cup_led_off_time;
extern U8 gu8_passive_hot_ster_key_input_count;
extern U16 gu16_passive_hot_ster_key_clear_timer;
extern bit bit_cold_first_op;
extern U8 u8Extract_Continue;
extern bit bit_hot_first_op;
extern bit bit_setting_mode_start;

extern bit bit_altitude_setting_start;
extern bit bit_filter_all;
extern bit bit_volume_setting_start;
extern bit bit_install_flushing_state;
extern U8 gu8_flushing_finish_input_count;
extern U16 gu16_flushing_finish_key_clear_time;
extern bit bit_ct_filter_type_start;
extern FLUSHING_STEP gu8_flushing_mode;
extern bit F_IceOut;

extern U8 gu8_passive_hot_ster_key_input_count;    // 트레이 고온살균 입력용 카운트 변수
extern bit bit_myWater_setting_start;
extern bit bit_volume_setting_start;

#define PRE_WATER_DRAIN_RETURN_TIME    70
#define CUP_LEVEL_RETURN_TIME          70
#define USER_SELECT_RETURN_TIME        70


#define SETTING_MODE_EXIT_TIME         200


#define LOW_LEVEL_DELAY_TIME           70



#endif
