/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _EEPROM_H_
#define _EEPROM_H_

//extern U8 IsRtcBatBackUpMode(void);
extern U8 HAL_RTC_ByteWrite( U8 _dev, U16 _addr , U8 _data );
extern U8 HAL_RTC_PageWrite( U8 _dev, U16 _addr , U8 *_data, U8 _len );
extern U8 HAL_RTC_SeqRead( U8 _dev, U16 _addr , U8 * _data, U8 _len );






#define EEP_ADDR_OFFSET   0x0040




/*#define DEFAULT_SERVICE_MODE_DAY    91*/

#define DEFAULT_SERVICE_MODE_DAY    0



#define MAX_ERROR    110



#define EEPROM_CHECK_A_VALUE              0xAA
#define EEPROM_CHECK_B_VALUE              0x55


extern TYPE_BYTE          U8WaterOutStateB;
#define            u8WaterOutState                           U8WaterOutStateB.byte
#define            Bit0_Pure_Water_Select_State                U8WaterOutStateB.Bit.b0
#define            Bit1_Cold_Water_Select_State                 U8WaterOutStateB.Bit.b1
#define            Bit2_Hot_Water_Select_State                U8WaterOutStateB.Bit.b2
#define            Bit3_Ice_Water_Select_State                U8WaterOutStateB.Bit.b3

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

extern TYPE_BYTE          U8CTFilterReplaceB;
#define            gu8CTFilterReplace                           U8CTFilterReplaceB.byte
#define            Bit0_CT_Neo_Filter_1_Replace                 U8CTFilterReplaceB.Bit.b0
#define            Bit1_CT_Ro_Filter_2_Replace                  U8CTFilterReplaceB.Bit.b1
#define            Bit2_CT_Ino_Filter_3_Replace                 U8CTFilterReplaceB.Bit.b2

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



extern U8 gu8_cup_level_order;


extern bit F_SpecialSettingModeSeatDetect;
extern bit F_SpecialSettingModeAutoDrain;


extern U8 u8Memento_Buff[];
extern bit F_SpecialSettingModeEW;        /*..자동배수 설정..*/

extern bit F_FirstTwistMoveCheck;  /*..전원인가 후 원점제어를 할 것인지를 확인하는 플래그..*/
extern bit F_First_Turn_On_Check;
extern bit F_First_Turn_On_Load_Value;

extern bit F_Cold_Enable;
extern bit F_Hot_Enable;
extern bit F_Hot_Lock;

extern bit F_Save;
extern bit F_Ice_Lock;
//extern bit F_IceOn;


extern U8 gu8AltitudeStep;
extern bit F_Altitude;
extern bit F_Circul_Drain;
extern U8 gu8Cup_level_Default;
//extern bit bit_eeprom_err;
extern bit F_All_Lock;

//extern U32	gu32ServiceTime;

extern U8 gu8_service_timer_1day;

extern bit F_Mute_Enable;

//extern bit F_Fast_Ice_Make;


extern bit F_Auto_UV_Control;
extern bit F_Welcome_Led_OnOff;
extern U8 gu8_clock_theme;
extern U8 gu8_screen_bright;
extern U8 gu8_ice_setting;

extern U8 gu8_cold_setting_level;
extern CUP_LEVEL gu8Cup_level;

extern bit bit_fast_ice_make;
extern bit F_IceOn;


extern U16 gu16_my_cup_level;
extern bit bit_my_cup_enable;
extern bit bit_install_flushing_state;
extern U8 gu8_filter_flushing_state;

extern U8 gu8_hot_level_order;
extern U8 gu8_hot_default_temp;
//extern U16 gu16_total_usage_water_gal_save;
//extern U16 u16_display_usage_water_gal;
//extern U32 gu32_total_usage_water_ml;

//extern U16 gu16_filter_reset_day_neo;
//extern U16 gu16_filter_reset_day_ro;

extern bit F_Wifi_Power;
extern bit F_Wifi_First_Paring;
extern bit F_Wifi_Fota_MODULE;
extern bit F_Wifi_Fota_MCU;
extern bit bit_periodic_ster_enable;
extern U8 gu8_wifi_period_ster_hour;
extern U8 gu8_wifi_period_ster_minute;

extern bit bit_sleep_mode_enable;
extern U8 gu8_sleep_mode_start_hour;
extern U8 gu8_sleep_mode_start_minute;
extern U8 gu8_sleep_mode_finish_hour;
extern U8 gu8_sleep_mode_finish_minute;

extern bit bit_child_lock_enable;
extern bit bit_first_time_setting;

extern U8 gu8_fota_start;
//extern U32 gu32_total_usage_water_ml_save;
extern U16 gu16_wifi_total_usage_water;
extern U8 gu8_wifi_smart_unused_setting_time;

extern U8 gu8VoiceVolumeLevel;
extern U8 gu8Sound_Type;
extern U8 gu8_voice_language_type;

extern U16 gu16_water_usage_neo_filter;
extern U16 gu16_water_usage_ro_filter;
extern U16 gu16_water_usage_ino_filter;
extern U16 gu16_reset_hour_neo_filter;
extern U16 gu16_reset_hour_ro_filter;
extern U16 gu16_reset_hour_ino_filter;
extern U8 gu8_filter_change_type;
extern U16 gu16_temporary_save_usage;

extern U8 gu8_ice_ster_drain_stop_count;
extern U8 gu8_ice_ster_low_water_stop_count;
extern U8 gu8_wifi_filter_cycle_percent;
extern U8 gu8_ct_forced_flushing_start;
extern U8 gu8_recover_org_fast_ice;
extern U8 gu8_return_acid_step;

extern bit bit_ice_size;
extern bit F_IceBreak;

extern MY_RECEIPE my_setting[MY_INDEX_MAX];
extern MY_INDEX my_recipe_select;
extern bit bit_waitmode_enable;
extern HOT_TEMP_LEVEL selected_hot_temp[4];
extern MY_INDEX my_receipe_default;

extern U16 gu16_WaterQuantity_half;
extern U16 gu16_WaterQuantity_one;
extern U16 gu16_WaterQuantity_two;
extern U16 gu16_WaterQuantity_four;

extern U8 gu8_drain_tank_ster_count;
#endif

