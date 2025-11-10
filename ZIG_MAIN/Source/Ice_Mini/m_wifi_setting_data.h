/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _WIFI_SETTING_DATA_H_
#define _WIFI_SETTING_DATA_H_

//
extern void water_select_return_timer_control( U8 mu8_flag );
//extern void stop_hot_preheating(void);
extern void play_melody_select_196( void );
extern void play_melody_warning_197( void );
extern void play_melody_setting_on_198( void );
extern void play_melody_setting_off_199( void );
extern void play_voice_setting_ice_lock_on_147( void );
extern void play_voice_setting_ice_lock_off_148( void );
extern void play_voice_setting_ice_on_149( void );
extern void play_voice_setting_ice_off_150( void );
extern void play_voice_sound_setting_voice_129( void );
extern void play_voice_sound_setting_melody_130( void );
extern void play_voice_sound_setting_mute_131( void );
extern void play_voice_all_lock_start_127( void );
extern void play_voice_all_lock_finish_128( void );

extern void play_voice_setting_hot_on_121( void );
extern void play_voice_setting_hot_off_122( void );

extern void play_voice_setting_hot_lock_on_112( void );
extern void play_voice_setting_hot_lock_off_113( void );

extern void play_voice_cold_temp_strong_140(void);
extern void play_voice_cold_temp_weak_139(void);
extern void play_voice_ice_size_large_144(void);
extern void play_voice_ice_size_small_143(void);
extern void play_voice_sleep_mode_enable_145(void);
extern void play_voice_sleep_mode_disable_146(void);
extern void play_voice_smart_test_mode_16(void);

extern void Play_Voice(U16 mu16MemoryAddress);

extern void start_drain_pump( U16 u16_data );
extern void stop_drain_pump( void );
extern void start_tray_pump( U16 u16_data );
extern void stop_tray_pump( void );

extern void start_self_test(void);
///extern void start_self_test(void);
extern void stop_cold_off_flick(void);
extern void stop_ice_off_flick(void);
extern void stop_icewater_off_flick(void);


extern TYPE_BYTE          U8WaterOutStateB;
#define            u8WaterOutState                           U8WaterOutStateB.byte
#define            Bit0_Pure_Water_Select_State                U8WaterOutStateB.Bit.b0
#define            Bit1_Cold_Water_Select_State                 U8WaterOutStateB.Bit.b1
#define            Bit2_Hot_Water_Select_State                U8WaterOutStateB.Bit.b2

extern TYPE_WORD          U16CupLevelSelectW;
#define            u16CupLevelSelect                          U16CupLevelSelectW.word
#define            u8CupLevelSelect_L                         U16CupLevelSelectW.byte[0]
#define            u8CupLevelSelect_H                         U16CupLevelSelectW.byte[1]
#define            Bit0_4_Oz_Select__120_ml                   U16CupLevelSelectW.Bit.b0
#define            Bit1_6_Oz_Select__175_ml                   U16CupLevelSelectW.Bit.b1
#define            Bit2_8_Oz_Select__235_ml                   U16CupLevelSelectW.Bit.b2
#define            Bit3_10_Oz_Select__295_ml                  U16CupLevelSelectW.Bit.b3
#define            Bit4_12_Oz_Select__355_ml                  U16CupLevelSelectW.Bit.b4
#define            Bit5_14_Oz_Select__415_ml                  U16CupLevelSelectW.Bit.b5
#define            Bit6_16_Oz_Select__475_ml                  U16CupLevelSelectW.Bit.b6
#define            Bit7_20_Oz_Select__590_ml                  U16CupLevelSelectW.Bit.b7
#define            Bit8_32_Oz_Select__945_ml                  U16CupLevelSelectW.Bit.b8
#define            Bit9_Continue_Select__Continue             U16CupLevelSelectW.Bit.b9



extern bit bit_ice_size;
extern U8 gu8_cold_setting_level;

extern bit F_Hot_Lock;
extern bit F_All_Lock;
extern bit bit_child_lock_enable;
extern bit F_Mute_Enable;
extern bit F_Ice_Lock;
extern bit F_Hot_Enable;
extern U8 gu8AltitudeStep;
extern bit bit_fast_ice_make;
extern bit F_IceOn;
extern bit F_Welcome_Led_OnOff;


extern U8 gu8_cup_level_order;
extern U8 gu8_wifi_period_ster_hour;
extern U8 gu8_wifi_period_ster_minute;
extern bit bit_sleep_mode_enable;
extern U8 gu8_sleep_mode_start_hour;
extern U8 gu8_sleep_mode_start_minute;
extern U8 gu8_sleep_mode_finish_hour;
extern U8 gu8_sleep_mode_finish_minute;
extern U8 gu8Cup_level_Default;
extern U8 gu8_fota_start;
extern U8 gu8_wifi_smart_unused_setting_time;

extern U8 gu8VoiceVolumeLevel;
extern U8 gu8Sound_Type;
extern U8 gu8_voice_language_type;

extern bit bit_self_test_start;
extern bit bit_self_test_stop;
extern U8 gu8_wifi_self_test_progress;
extern U8 gu8_wifi_self_test_state;
extern U8 gu8_filter_change_type;
extern CUP_LEVEL gu8Cup_level;
extern MY_RECEIPE my_setting[MY_INDEX_MAX];
extern MY_INDEX my_recipe_select;
extern bit bit_waitmode_enable;
extern MY_INDEX my_receipe_default;
extern bit bit_welcome_wifi_start;
extern bit bit_welcome_start;
extern U16 gu16_welcome_timer;
extern U16 gu16_water_select_return_time;
#endif
