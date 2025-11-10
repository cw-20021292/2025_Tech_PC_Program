
/*******************************************************************************
&Description :
    A1010 ~ A101x
    Function Table

&History : ssg@coway.com 2024.10.04 update

*******************************************************************************/

#include "WIFI_A101x_FunctionList.h"

/******************************************************************************/
/***** User Header ************************************************************/
/******************************************************************************/
// #include "include/_type_define.h"
// #include "_fix_ram.h"
// #include "cold.h"
/******************************************************************************/
/***** User Function **********************************************************/
/******************************************************************************/
bit bit_wifi_first_hot_ster;
/******************************************************************************/
#include "Global_Variable.h"
#include "macrodriver.h"
#include "m_wifi_setting_data.h"
/******************************************************************************/
extern void wifi_hot_lock( U16 mu16_setting );
extern void init_water_quantity(void);
extern void BuzStep(U8 mu8Step);
extern void init_wifi_elec_watt(void);
extern void get_wifi_time ( U16 mYear, U16 mMonth, U16 mDate, U16 mHour, U16 mMin, U16 mSec );
extern void init_water_quantity(void);
extern void wifi_hot_lock( U16 mu16_setting );
extern void wifi_all_lock( U16 mu16_setting );
extern void wifi_child_lock( U16 mu16_setting );
extern void wifi_mute( U16 mu16_setting );
extern void wifi_ice_lock( U16 mu16_setting );
extern void wifi_hot_enable( U16 mu16_setting );
extern void wifi_altitude( U16 mu16_setting );
extern void wifi_ice_first( U16 mu16_setting );
extern void wifi_voice_select(U16 mu16_setting);
extern void wifi_cold_temp( U16 mu16_setting );
extern void wifi_ice_size( U16 mu16_setting );
extern void wifi_ice_enable( U16 mu16_setting );
extern void wifi_welcome_light( U16 mu16_setting );
extern void wifi_hot_default_temp( U16 mu16_setting );
extern void wifi_default_cup( U16 mu16_setting );
extern void wifi_4_oz_water_using( U16 mu16_setting );
extern void wifi_6_oz_water_using( U16 mu16_setting );
extern void wifi_8_oz_water_using( U16 mu16_setting );
extern void wifi_10_oz_water_using( U16 mu16_setting );
extern void wifi_12_oz_water_using( U16 mu16_setting );
extern void wifi_14_oz_water_using( U16 mu16_setting );
extern void wifi_16_oz_water_using( U16 mu16_setting );
extern void wifi_20_oz_water_using( U16 mu16_setting );
extern void wifi_32_oz_water_using( U16 mu16_setting );
extern void wifi_continue_water_using( U16 mu16_setting );
extern void wifi_my_default( U16 mu16_setting );
extern void wifi_cup_level_order( U16 mu16_setting );
extern void wifi_hot_temp_level_order( U16 mu16_setting );
extern void wifi_period_ster_hour( U16 mu16_setting );
extern void wifi_period_ster_minute( U16 mu16_setting );
extern void wifi_sleep_mode_enable( U16 mu16_setting );
extern void wifi_sleep_start_hour( U16 mu16_setting );
extern void wifi_sleep_start_minute( U16 mu16_setting );
extern void wifi_sleep_finish_hour( U16 mu16_setting );
extern void wifi_sleep_finish_minute( U16 mu16_setting );
extern void wifi_unused_setting_time( U16 mu16_setting );
extern void start_fota(void);
extern void init_wifi_no_use_time(void);
extern void init_wifi_ice_out_time(void);
extern void wifi_voice( U16 mu16_setting );
extern void wifi_volume( U16 mu16_setting );
extern void wifi_voice_language( U16 mu16_setting );
extern void Play_Voice(U16 mu16MemoryAddress);
extern void wifi_self_test( U16 mu16_setting );
extern void initial_each_self_data( U8 mu8_num );
extern void wifi_filter_change_type( U16 mu16_setting );
extern void play_melody_select_196( void );
extern void SetWifiBuzzSound ( U16 mu16Buzz );
extern void play_melody_setting_on_198( void );
extern void play_melody_setting_off_199( void );
extern void wifi_set_temp( U16 mu16_temp, U16 mu16_setting );
extern void wifi_my_use( U8 index, U16 mu16_setting );
extern void wifi_my_temp( U8 index, U16 mu16_setting );
extern void wifi_my_amount( U8 index, U16 mu16_setting );
extern void wifi_wait_mode_enable( U16 mu16_setting );
extern void wifi_set_waterquantity( U16 mu16_waterType, U16 mu16_quantity );
extern void wifi_set_ice_type(U16 mu16_setting);
extern void play_voice_setting_cold_on_124( void );
extern void play_voice_setting_cold_off_125( void );
extern void stop_cold_off_flick(void);
/******************************************************************************/
extern bit F_Cold_Enable;
extern bit F_Hot_Lock;
extern bit F_All_Lock;
extern bit bit_fast_ice_make;
extern U8 gu8_hot_setting_temperature;
extern bit F_Tank_Cover_Input;
extern U8 gu8_cody_care_timer_msec;
extern U16 gu16_cody_care_timer_sec;
extern bit F_Ice_Lock;
extern bit bit_filter_cover;
extern bit F_IceOn;
extern U8 gu8_Cold_Temperature_One_Degree;
extern U8 gu8_cold_setting_level;
extern U16 gu16_wifi_total_quant;
extern bit F_WaterOut;
extern U8 gu8_Hot_In_Temperature_One_Degree;
extern U8 gu8_Hot_Heater_Temperature_One_Degree;
extern U16 gu16_silver_timer_min;
extern U8 u8FirstSilverCare;
extern bit F_IceOut;
extern U32 gu32_wifi_ice_out_time;
extern U8 gu8_wifi_ice_send_count;
extern U8 gu8Sound_Type;
extern U8 gu8VoiceVolumeLevel;
extern U8 gu8_wifi_self_test_state;
extern U8 gu8_wifi_self_test_progress;
extern bit bit_filter_reed;
extern bit F_Welcome_Led_OnOff;
extern bit bit_uv_extract_faucet_out;
extern bit bit_wifi_first_faucet_uv;
extern bit bit_wifi_ino_filter_3_reset;
extern bit bit_ice_size;
extern bit bit_wifi_first_ice_tank_uv;
extern bit bit_uv_ice_tank_out;
extern bit bit_wifi_first_ice_tray_uv;
extern bit bit_uv_ice_tray_out;
extern bit bit_sleep_mode_enable;
extern U8 gu8_sleep_mode_start_hour;
extern U8 gu8_sleep_mode_start_minute;
extern U8 gu8_sleep_mode_finish_hour;
extern U8 gu8_sleep_mode_finish_minute;
extern U8 gu8Cup_level_Default;
extern U8 gu8_hot_level_order;
extern bit bit_uv_ice_faucet_out;
extern bit bit_wifi_hot_heater;
extern bit bit_wifi_heater_on_time_update;
extern U32 gu32_hot_target_reach_timer_ms;
extern U16 gu16_hot_target_reach_timer_sec;
extern bit bit_wifi_comp;
extern bit bit_wifi_comp_on_time_update;
extern U16 gu16_cold_target_reach_timer_sec;
extern U32 gu32_cold_target_reach_timer_ms;
extern bit F_Comp_Output;
extern bit bit_wifi_first_comp;
extern U8 gu8_wifi_ice_make_state;
extern ICE_STEP gu8IceStep;
extern U32 gu32_wifi_ice_make_time;
extern U32 gu32_wifi_ice_heater_timer;
extern bit F_drain_water_level_low;
extern bit bit_wifi_first_drain_low;
extern bit bit_wifi_first_drain_high;
extern bit F_drain_water_level_high;
extern U8 gu8_wifi_flushing_state;
extern LED_STEP gu8_Led_Display_Step;
extern bit F_IceLack;
extern bit F_IceFull;
extern bit bit_ice_tank_ster_start;
extern ICE_STER_MODE gu8_ice_ster_mode;
extern U8 gu8_wifi_period_ster_hour;
extern U8 gu8_wifi_period_ster_minute;
extern U16 WifiSmartCheckResultData;
extern bit bit_wifi_first_ice_full;
extern U16 gu16_water_usage_neo_filter;
extern U16 gu16_water_usage_ro_filter;
extern U16 gu16_water_usage_ino_filter;
extern ICE_STER_MODE gu8_ice_ster_mode;
extern bit bit_sleep_mode_start;
extern bit bit_wifi_first_sleep_mode;
extern MY_RECEIPE my_setting[MY_INDEX_MAX];
extern MY_INDEX my_recipe_select;
extern U8 gu8_periodic_ster_count;
extern bit bit_waitmode_enable;
extern MY_INDEX my_receipe_default;
extern U8 gu8_cooling_display_mode;
extern U16 gu16_WaterQuantity_half;
extern U16 gu16_WaterQuantity_one;
extern U16 gu16_WaterQuantity_two;
extern U16 gu16_WaterQuantity_four;
extern bit F_IceBreak;
extern U8 gu8_wait_finish_step;
/******************************************************************************/
// extern TYPE_BYTE          U8WaterOutStateB;
// #define            u8WaterOutState                           U8WaterOutStateB.byte
// #define            Bit0_Pure_Water_Select_State              U8WaterOutStateB.Bit.b0
// #define            Bit1_Cold_Water_Select_State              U8WaterOutStateB.Bit.b1
// #define            Bit2_Hot_Water_Select_State               U8WaterOutStateB.Bit.b2

extern TYPE_BYTE          U8IceOutStateB;
#define            u8IceOutState                             U8IceOutStateB.byte
#define            Bit0_Ice_Only_Select_State                U8IceOutStateB.Bit.b0
#define            Bit1_Ice_Plus_Water_Select_State          U8IceOutStateB.Bit.b1



extern TYPE_WORD          U16HotTemplSelectW;
#define            U16HotTemplSelect				U16HotTemplSelectW.word
#define            U16HotTemplSelect_L				U16HotTemplSelectW.byte[0]
#define            U16HotTemplSelect_H				U16HotTemplSelectW.byte[1]
#define            Bit0_Temp_Select__45oC			U16HotTemplSelectW.Bit.b0
#define            Bit1_Temp_Select__50oC			U16HotTemplSelectW.Bit.b1
#define            Bit2_Temp_Select__55oC			U16HotTemplSelectW.Bit.b2
#define            Bit3_Temp_Select__60oC			U16HotTemplSelectW.Bit.b3
#define            Bit4_Temp_Select__65oC			U16HotTemplSelectW.Bit.b4
#define            Bit5_Temp_Select__70oC			U16HotTemplSelectW.Bit.b5
#define            Bit6_Temp_Select__75oC			U16HotTemplSelectW.Bit.b6
#define            Bit7_Temp_Select__80oC			U16HotTemplSelectW.Bit.b7
#define            Bit8_Temp_Select__85oC			U16HotTemplSelectW.Bit.b8
#define            Bit9_Temp_Select__90oC			U16HotTemplSelectW.Bit.b9
#define            Bit10_Temp_Select__95oC			U16HotTemplSelectW.Bit.b10
#define            Bit11_Temp_Select__100oC         U16HotTemplSelectW.Bit.b11

/******************************************************************************/

/******************************************************************************/
/***** Table ******************************************************************/
/******************************************************************************/
/* Event List */
U16 WifiFuncEventPreVal[USER_DATA_MAX_SIZE] = {0};

static const WifiTxFuncEventList_T WifiFuncEventList[] =
{   /*  API ID                                      DATA_TYPE                                 PreVal(Initial)                         EnventForm     */
    {   WIFI_FUNC_0002_COLD_SEL,                TYPE_A1014,                          (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_0003_HOT_LOCK_SEL,            TYPE_A1014,                          (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_0005_LOCK_SEL,                TYPE_A1014,                          (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_0008_ICE_SEL,                 TYPE_A1014,                          (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_000A_COLD_TEMP,               TYPE_UNUSED,                          (U16*)(&WifiFuncEventPreVal),         EVENT_UNUSED },
    {   WIFI_FUNC_000B_WATER_SEL,               TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_UNUSED        },
    {   WIFI_FUNC_000D_WATER_OUTQUANTITY,       TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_ZERO_TO_OTHER },   // 0 -> ??

    {   WIFI_FUNC_001A_HOT_INTEMP,              TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_UNUSED },
    {   WIFI_FUNC_001B_HOT_MEANTEMP,            TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_UNUSED },
    {   WIFI_FUNC_001C_HOT_SELTEMP,             TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_UNUSED },

    {   WIFI_FUNC_0024_HEART_TIME4,             TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_ZERO_TO_OTHER },   // 0 -> ??
    {   WIFI_FUNC_002C_SILVER_CARE,             TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_ZERO_TO_ONE   },

    {   WIFI_FUNC_0030_ICE_MAKETIME,            TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_ZERO_TO_OTHER },   // 0 -> ??
    {   WIFI_FUNC_0031_VOICE_SEL,               TYPE_A1014,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },   // 0 -> ??
    {   WIFI_FUNC_0032_ICELOCK_SEL,             TYPE_A1014,                          (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_0033_VOICE_VOL_SEL,           TYPE_UNUSED,                          (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_0035_COVER1_OPEN,             TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_0036_COVER2_OPEN,             TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_003C_COLD_STRENGTH_SEL,       TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },

    {   WIFI_FUNC_0040_SMART_CHECK_RQST,        TYPE_SMART_DIAGNOSIS_CONTROL,        (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_0041_SMART_CHECK_STATUS,      TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_0042_SMART_CHECK_PROGRESS,    TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_0043_FILTER_STATUS1,          TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_004A_WELCOME_LIGHT,           TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },

    {   WIFI_FUNC_0050_UV_STERILIZE_STATUS,     TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_ZERO_TO_ONE   },
    {   WIFI_FUNC_0053_FILTER_RESET1,           TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_0058_ICE_SIZE_SEL,            TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_0059_ICE_ONOFF_SEL,           TYPE_A1014,                          (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_005A_UV_ICE_TANK_STATUS,      TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_ZERO_TO_ONE   },
    {   WIFI_FUNC_005B_UV_ICE_TRAY_STATUS,      TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_ZERO_TO_ONE   },
    {   WIFI_FUNC_005C_SMART_CHECK_RESULT,      TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_UNUSED        },
    {   WIFI_FUNC_005D_UV_ICE_FAUCET_STATUS,    TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_ZERO_TO_ONE   },
    {   WIFI_FUNC_005E_SLEEP_MODE_SEL,          TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_005F_SLEEP_START_HOUR,        TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },

    {   WIFI_FUNC_0060_SLEEP_START_MIN,         TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_0061_SLEEP_END_HOUR,          TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_0062_SLEEP_END_MIN,           TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_0066_DEFAULT_QUANTITY_SEL,    TYPE_A1014,                          (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_0078_COLD_TARGET_TEMP_TIME,   TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_ZERO_TO_OTHER },   //0 -> ??
    {   WIFI_FUNC_0083_POWER_SAVING_STATUS,     TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_ZERO_TO_ONE   },
    {   WIFI_FUNC_008C_COMP_STATUS,             TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_ZERO_TO_ONE   },
    {   WIFI_FUNC_008F_ICE_FULL_STATUS,         TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_ZERO_TO_ONE   },

    {   WIFI_FUNC_0093_ICEMAKING_STAUTS,        TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_SPECIFIC_CONDITION   },
    {   WIFI_FUNC_0095_ICEMAKING_COMPLET_TIME,  TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_ZERO_TO_OTHER },   //0 -> ??
    {   WIFI_FUNC_0096_DEICING_COMPLET_TIME,    TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_ZERO_TO_OTHER },   //0 -> ??
    {   WIFI_FUNC_009B_DRAINTANK_LOW_WLEVEL,    TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_ZERO_TO_ONE   },
    {   WIFI_FUNC_009C_DRAINTANK_HIGH_WLEVEL,   TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_ZERO_TO_ONE   },

    {   WIFI_FUNC_00A0_COLD_TANK_TEMP,          TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_UNUSED        },
    // {   WIFI_FUNC_00AC_FILTER1_WATER_USAGE,     TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_UNUSED        },
    // {   WIFI_FUNC_00AD_FILTER1_UASGE_DAY,       TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },

    {   WIFI_FUNC_00B5_FILTER_FLUSHING,         TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_ZERO_TO_ONE   },
    {   WIFI_FUNC_00BE_QUANTITY_HALFCUP,        TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_00BF_QUANTITY_ONECUP,         TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_00C0_QUANTITY_TWOCUPS,        TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },

    {   WIFI_FUNC_00C1_HOTTEMP_USE_SEL0,        TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_00C2_HOTTEMP_USE_SEL1,        TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_00C3_HOTTEMP_USE_SEL2,        TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_00C4_HOTTEMP_USE_SEL3,        TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_00C5_HOTTEMP_USE_SEL4,        TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_00C6_HOTTEMP_USE_SEL5,        TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_00C7_HOTTEMP_USE_SEL6,        TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_00C8_HOTTEMP_USE_SEL7,        TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_00C9_HOTTEMP_USE_SEL8,        TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_00CA_HOTTEMP_USE_SEL9,        TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_00CB_WATER_EXTRACT_STATUS,    TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_00CC_ICE_EXTRACT_STATUS,      TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_00CD_SETUP_MODE_STATUS,       TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_00CE_HOTTEMP_USE_SEL10,       TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_00CF_HOTTEMP_USE_SEL11,       TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },

    {   WIFI_FUNC_00E1_MYWATER_TEMP_1,          TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_00E2_MYWATER_QUANTITY_1,      TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_00E3_MYWATER_RECIPE_1,        TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_00E4_MYWATER_TEMP_2,          TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_00E5_MYWATER_QUANTITY_2,      TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_00E6_MYWATER_RECIPE_2,        TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_00E7_MYWATER_TEMP_3,          TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_00E8_MYWATER_QUANTITY_3,      TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_00E9_MYWATER_RECIPE_3,        TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_00EA_MYWATER_TEMP_4,          TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_00EB_MYWATER_QUANTITY_4,      TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_00EC_MYWATER_RECIPE_4,        TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_00ED_MYWATER_TEMP_5,          TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_00EE_MYWATER_QUANTITY_5,      TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_00EF_MYWATER_RECIPE_5,        TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },

    /* 2KG */
    {   WIFI_FUNC_00F0_MYWATER_TEMP_6,          TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_00F1_MYWATER_QUANTITY_6,      TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_00F2_MYWATER_RECIPE_6,        TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },

    /* MINI,1KG,2KG */
    {   WIFI_FUNC_0101_STERILIZE_REMAINING_DAY, TYPE_UNUSED,                    (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },

    {   WIFI_FUNC_0107_MYWATER_ENABLE_1,        TYPE_UNUSED,                    (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_0108_MYWATER_ENABLE_2,        TYPE_UNUSED,                    (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_0109_MYWATER_ENABLE_3,        TYPE_UNUSED,                    (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_010A_MYWATER_ENABLE_4,        TYPE_UNUSED,                    (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_010B_MYWATER_ENABLE_5,        TYPE_UNUSED,                    (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },

    /* 2KG */
    {   WIFI_FUNC_010C_MYWATER_ENABLE_6,        TYPE_UNUSED,                    (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },

    {   WIFI_FUNC_0116_AMOUNT_OF_ICE,           TYPE_UNUSED,                    (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },

    { 	WIFI_FUNC_011A_TRAY_HOT_STER_STATE, 	TYPE_UNUSED,					(U16*)(&WifiFuncEventPreVal),		EVENT_STATUS_CHANGE },
    { 	WIFI_FUNC_011B_TRAY_HOT_STER_HOUR,		TYPE_UNUSED,					(U16*)(&WifiFuncEventPreVal),		EVENT_STATUS_CHANGE },
    { 	WIFI_FUNC_011C_TRAY_HOT_STER_MINUTE,	TYPE_UNUSED,					(U16*)(&WifiFuncEventPreVal),		EVENT_STATUS_CHANGE },
    { 	WIFI_FUNC_011D_WAITMODE,				TYPE_UNUSED,					(U16*)(&WifiFuncEventPreVal),		EVENT_STATUS_CHANGE },
    { 	WIFI_FUNC_011E_ONSU_SET_NUM_MIN,		TYPE_UNUSED,					(U16*)(&WifiFuncEventPreVal),		EVENT_STATUS_CHANGE },
    { 	WIFI_FUNC_011F_DEFAULT_MY_WATER,		TYPE_UNUSED,					(U16*)(&WifiFuncEventPreVal),		EVENT_STATUS_CHANGE },
    { 	WIFI_FUNC_0120_HOT_SETTING_MAX_NUM,		TYPE_UNUSED,					 (U16*)(&WifiFuncEventPreVal),		EVENT_STATUS_CHANGE },
    { 	WIFI_FUNC_0122_QUANTITY_FOURCUPS,		TYPE_UNUSED,					 (U16*)(&WifiFuncEventPreVal),		EVENT_STATUS_CHANGE },      // ���� �뷮 �߰�
    { 	WIFI_FUNC_0123_ICE_TYPE,				TYPE_UNUSED,					 (U16*)(&WifiFuncEventPreVal),		EVENT_STATUS_CHANGE },      // ���� �뷮 �߰�
};

#define SZ_FUNC_EVENT_LIST    (sizeof(WifiFuncEventList)/sizeof(WifiTxFuncEventList_T))

void* GetWifiFuncDataEventList ( void )
{
    if (IsUserModelType() == Model_0)
    {
        return WifiFuncEventList;
    }
    else
    {

    }

}
U16 GetWifiFuncEventListSize ( void )
{
    if (IsUserModelType() == Model_0)
    {
        return SZ_FUNC_EVENT_LIST;
    }
    else
    {

    }
}


/* [User Function Data] Product -> Server */
U16 GetUserSystemFunction ( U16 mu16Func )
{
    U16 mu16Data = 0U;
    U16 buf = 0;

    switch ( mu16Func )
    {
        case WIFI_FUNC_0002_COLD_SEL :       /* ID */   /* �ü� ON/OFF ? */
            // mu16Data = 1;                /* Value */
            if(F_Cold_Enable == SET)
            {
                mu16Data = SET;
            }
            else
            {
                mu16Data = CLEAR;
            }
        break;

        case WIFI_FUNC_0003_HOT_LOCK_SEL :      /* �¼���� */
            if( F_Hot_Lock == SET )
            {
                mu16Data =  WIFI_HOT_LOCK_ON;
            }
            else
            {
                mu16Data =  WIFI_HOT_LOCK_OFF;
            }
        break;

        case WIFI_FUNC_0005_LOCK_SEL :          /* ��ü���? */
            if( F_All_Lock == SET )
            {
                mu16Data = SET;
            }
            else
            {
                mu16Data = CLEAR;
            }
        break;

        case WIFI_FUNC_0008_ICE_SEL :           /* ��������  */
            if( bit_fast_ice_make == COLD_FIRST_ICE_MAKE )
            {
                /*..hui [23-6-23���� 4:07:01] �ü��켱 - 1..*/
                mu16Data = 1;
            }
            else
            {
                /*..hui [23-6-23���� 4:07:05] �����켱 - 3..*/
                mu16Data = 3;
            }
        break;

        case WIFI_FUNC_000A_COLD_TEMP :

        if(F_Cold_Enable == CLEAR)
        {
            mu16Data = 0;
        }
        else
        {
            if( gu8_cooling_display_mode == COOLING_DISPLAY_1_OPERATION )
            {
                mu16Data = 1;
            }
            else if( gu8_cooling_display_mode == COOLING_DISPLAY_2_COMPLETE )
            {
                mu16Data = 2;
            }
        }

        break;

        case WIFI_FUNC_000B_WATER_SEL :         /* ������?? */
            if( u8WaterOutState == PURE_WATER_SELECT )
            {
                mu16Data = 1;
            }
            else if( u8WaterOutState == COLD_WATER_SELECT )
            {
                mu16Data = 2;
            }
            else if(( u8WaterOutState == HOT_WATER_SELECT )
            && ( u8IceOutState == ICE_SELECT__NONE )
            )
            {
                mu16Data = 3;
            }
            else
            {

            }

        break;

        case WIFI_FUNC_000D_WATER_OUTQUANTITY : /* ���� ����?? */
            if( F_WaterOut == SET )
            {
                mu16Data = 0;
            }
            else
            {
                mu16Data = gu16_wifi_total_quant;
                init_water_quantity();
            }
        break;

        case WIFI_FUNC_001A_HOT_INTEMP :
           if(u8WaterOutState == HOT_WATER_SELECT)
            {
                mu16Data = gu8_Hot_In_Temperature_One_Degree;
            }
            else
            {
                mu16Data = 0;
            }
        break;

        case WIFI_FUNC_001B_HOT_MEANTEMP :                  /* �����¼� ��տµ� : ��� ��ճ�����? */
            if(u8WaterOutState == HOT_WATER_SELECT)
            {
                mu16Data = gu8_Hot_Heater_Temperature_One_Degree;
            }
            else
            {
                mu16Data = 0;
            }
        break;

        case WIFI_FUNC_001C_HOT_SELTEMP:
            if(u8WaterOutState == HOT_WATER_SELECT)
            {
                if(gu8_hot_setting_temperature == HOT_SET_TEMP____100oC)
                {
                    mu16Data = 100;
                }
                else if(gu8_hot_setting_temperature == HOT_SET_TEMP____95oC)
                {
                    mu16Data = 95;
                }
                else if(gu8_hot_setting_temperature == HOT_SET_TEMP____90oC)
                {
                    mu16Data = 90;
                }
                else if(gu8_hot_setting_temperature == HOT_SET_TEMP____85oC)
                {
                    mu16Data = 85;
                }
                else if(gu8_hot_setting_temperature == HOT_SET_TEMP____80oC)
                {
                    mu16Data = 80;
                }
                else if(gu8_hot_setting_temperature == HOT_SET_TEMP____75oC)
                {
                    mu16Data = 75;
                }
                else if(gu8_hot_setting_temperature == HOT_SET_TEMP____70oC)
                {
                    mu16Data = 70;
                }
                else if(gu8_hot_setting_temperature == HOT_SET_TEMP____65oC)
                {
                    mu16Data = 65;
                }
                else if(gu8_hot_setting_temperature == HOT_SET_TEMP____60oC)
                {
                    mu16Data = 60;
                }
                else if(gu8_hot_setting_temperature == HOT_SET_TEMP____55oC)
                {
                    mu16Data = 55;
                }
                else if(gu8_hot_setting_temperature == HOT_SET_TEMP____50oC)
                {
                    mu16Data = 50;
                }
                else if(gu8_hot_setting_temperature == HOT_SET_TEMP____45oC)
                {
                    mu16Data = 45;
                }
            }
            else
            {
                mu16Data = 0;
            }
        break;

        case WIFI_FUNC_0024_HEART_TIME4 :
            /*..hui [21-3-17���� 7:56:45] ���������� �ð� ����.. ���������� 0����.. ����..*/
            if( F_Tank_Cover_Input == SET )
            {
                mu16Data = gu16_cody_care_timer_sec;
            }
            else
            {
                mu16Data = 0;
            }

            /*..hui [21-3-5���� 3:08:57] Ŀ�� ������ ����Ÿ ������ ���⼭ �ʱ�ȭ..*/
            if( F_Tank_Cover_Input == SET )
            {
                gu8_cody_care_timer_msec = 0;
                gu16_cody_care_timer_sec = 0;
            }
            else{}
        break;
        case WIFI_FUNC_002C_SILVER_CARE :
            // 48�ð� �̻�� (0�̰�� 1:�����)
            if( gu16_silver_timer_min >= SILVER_CARE_TIME_MIN )
            {
                if ( u8FirstSilverCare == CLEAR )
                {
                    u8FirstSilverCare = SET;
                    mu16Data = 1;
                }
                else
                {
                    mu16Data = 2;
                }
            }
            else
            {
                u8FirstSilverCare = CLEAR;
                mu16Data = 0;
            }
        break;

        case WIFI_FUNC_0030_ICE_MAKETIME :
            if( F_IceOut == CLEAR )
            {
                mu16Data = (U16)(gu32_wifi_ice_out_time / 10);
                gu8_wifi_ice_send_count++;

                /*..hui [24-1-9���� 1:29:37] 1010������ 1014���� ������ ���� �ʱ�ȭ�ϱ����� ī��Ʈ..*/
                if( gu8_wifi_ice_send_count >= 2 )
                {
                    gu8_wifi_ice_send_count = 2;
                    gu32_wifi_ice_out_time = 0;
                }
                else{}
            }
            else
            {
                mu16Data = 0;
            }
        break;

        case WIFI_FUNC_0031_VOICE_SEL :
            if( gu8Sound_Type == SOUND_TYPE_MUTE )
            {
                mu16Data = 1;
            }
            else if(gu8Sound_Type == SOUND_TYPE_MELODY )
            {
                mu16Data = 2;
            }
            else
            {
                mu16Data = 3;
            }
        break;

        case WIFI_FUNC_0032_ICELOCK_SEL :
            if( F_Ice_Lock == CLEAR )
            {
                mu16Data = 0;
            }
            else
            {
                mu16Data = 1;
            }
        break;

        case WIFI_FUNC_0033_VOICE_VOL_SEL :
            if(gu8VoiceVolumeLevel == SOUND_VOLUME_LEVEL_1)
            {
                mu16Data = 1;
            }
            else if(gu8VoiceVolumeLevel == SOUND_VOLUME_LEVEL_2)
            {
                mu16Data = 2;
            }
            else if(gu8VoiceVolumeLevel == SOUND_VOLUME_LEVEL_3)
            {
                mu16Data = 3;
            }
            else if(gu8VoiceVolumeLevel == SOUND_VOLUME_LEVEL_4)
            {
                mu16Data = 4;
            }
            else if(gu8VoiceVolumeLevel == SOUND_VOLUME_LEVEL_5)
            {
                mu16Data = 5;
            }
        break;

        case WIFI_FUNC_0035_COVER1_OPEN :
            if ( bit_filter_cover == SET )
            {
                /*..hui [21-8-3���� 12:48:30] ����..*/
                mu16Data = 0;
            }
            else
            {
                /*..hui [21-8-3���� 12:48:23] ����..*/
                mu16Data = 1;
            }
        break;

        case WIFI_FUNC_0036_COVER2_OPEN :
            if (F_Tank_Cover_Input == SET)
            {
                mu16Data = 0;
            }
            else
            {
                mu16Data = 1;
            }
        break;

        case WIFI_FUNC_003C_COLD_STRENGTH_SEL :
            if( gu8_cold_setting_level == COLD_TEMP_HIGH )
            {
                mu16Data = 0;
            }
            else if( gu8_cold_setting_level == COLD_TEMP_LOW )
            {
                mu16Data = 1;
            }
        break;

        case WIFI_FUNC_0040_SMART_CHECK_RQST :
            if( bit_self_test_start == 0 )
            {
                mu16Data = 0;
            }
        else
            {
                mu16Data = 1;
            }
        break;

        case WIFI_FUNC_0041_SMART_CHECK_STATUS :
            #if 0
            if((F_WaterOut == SET)
            || (Bit1_Uart_Test_Mode == SET)
            || (Bit0_Pcb_Test_Mode == SET)
            || (F_IceOut == SET )
            || (bit_ice_tank_ster_start == SET )
            )
			{
                mu16Data = WIFI_SELF_TEST_PROHIBIT;
                gu8_wifi_self_test_state = WIFI_SELF_TEST_PROHIBIT;
            }
            else
            {
                mu16Data = gu8_wifi_self_test_state;
            }
            #endif
            mu16Data = gu8_wifi_self_test_state;
        break;

        case WIFI_FUNC_0042_SMART_CHECK_PROGRESS :
            mu16Data = gu8_wifi_self_test_progress;
        break;

        case WIFI_FUNC_0043_FILTER_STATUS1 :
            if(bit_filter_reed == SET)
            {
                mu16Data = 0;       /* ���� */
            }
            else
            {
                mu16Data = 1;       /* �и� */
            }
        break;

        case WIFI_FUNC_004A_WELCOME_LIGHT :
            if( F_Welcome_Led_OnOff == CLEAR )
            {
                /*..hui [23-6-23���� 2:32:59] OFF..*/
                mu16Data = 0;
            }
            else
            {
                /*..hui [23-6-23���� 2:33:03] ON..*/
                mu16Data = 1;
            }
        break;

        case WIFI_FUNC_0050_UV_STERILIZE_STATUS :
            if(bit_uv_extract_faucet_out == SET)
            {
                if( bit_wifi_first_faucet_uv == CLEAR )
                {
                    bit_wifi_first_faucet_uv = SET;
                    mu16Data = 1;
                }
                else
                {
                    // mu16Data = 2;
                    mu16Data = 1;
                }
            }
            else
            {
                bit_wifi_first_faucet_uv = CLEAR;
                mu16Data = 0;
            }
        break;

        case WIFI_FUNC_0053_FILTER_RESET1 :
            if( bit_wifi_ino_filter_3_reset  == SET )
            {
                mu16Data = 1;
                bit_wifi_ino_filter_3_reset = CLEAR;
            }
            else
            {
                mu16Data = 0;
            }
        break;

        case WIFI_FUNC_0058_ICE_SIZE_SEL :
            if(bit_ice_size == ICE_SIZE_LARGE)
         	{
                mu16Data = 0;
         	}
            else
            {
                mu16Data = 2;
            }
       break;

       case WIFI_FUNC_0059_ICE_ONOFF_SEL :
            if( F_IceOn == CLEAR )
            {
                mu16Data = 0;
            }
            else
            {
                mu16Data = 1;
            }
        break;

       case WIFI_FUNC_005A_UV_ICE_TANK_STATUS:
            if(bit_uv_ice_tank_out == SET)
            {
                if( bit_wifi_first_ice_tank_uv == CLEAR )
                {
                    bit_wifi_first_ice_tank_uv = SET;
                    mu16Data = 1;
                }
                else
                {
                    // mu16Data = 2;
                    mu16Data = 1;
                }
            }
            else
            {
                bit_wifi_first_ice_tank_uv = CLEAR;
                mu16Data = 0;
            }
        break;

        case WIFI_FUNC_005B_UV_ICE_TRAY_STATUS :
            if(bit_uv_ice_tray_out == SET)
            {
                if( bit_wifi_first_ice_tray_uv == CLEAR )
                {
                    bit_wifi_first_ice_tray_uv = SET;
                    mu16Data = 1;
                }
                else
                {
                    // mu16Data = 2;
                    mu16Data = 1;
                }
            }
            else
            {
                bit_wifi_first_ice_tray_uv = CLEAR;
                mu16Data = 0;
            }
        break;

        case WIFI_FUNC_005C_SMART_CHECK_RESULT :
            mu16Data = WifiSmartCheckResultData;
            WifiSmartCheckResultData = 0U;
        break;

        case WIFI_FUNC_005D_UV_ICE_FAUCET_STATUS :
            if(bit_uv_ice_faucet_out == SET)
            {
                if( bit_wifi_first_faucet_uv == CLEAR )
                {
                    bit_wifi_first_faucet_uv = SET;
                    mu16Data = 1;
                }
                else
                {
                    // mu16Data = 2;
                    mu16Data = 1;
                }
            }
            else
            {
                bit_wifi_first_faucet_uv = CLEAR;
                mu16Data = 0;
            }

        break;

        case WIFI_FUNC_005E_SLEEP_MODE_SEL :
            if( bit_sleep_mode_enable == CLEAR )
            {
                mu16Data = 0;
            }
            else
            {
                mu16Data = 1;
            }
        break;

        case WIFI_FUNC_005F_SLEEP_START_HOUR :
            mu16Data = (U16)gu8_sleep_mode_start_hour;
        break;

        case WIFI_FUNC_0060_SLEEP_START_MIN :
            mu16Data = (U16)gu8_sleep_mode_start_minute;
        break;

        case WIFI_FUNC_0061_SLEEP_END_HOUR :
            mu16Data = (U16)gu8_sleep_mode_finish_hour;
        break;

        case WIFI_FUNC_0062_SLEEP_END_MIN :
            mu16Data = (U16)gu8_sleep_mode_finish_minute;
        break;

        case WIFI_FUNC_0066_DEFAULT_QUANTITY_SEL :
            if(gu8Cup_level_Default == CUP_LEVEL_1_120ML)
            {
                mu16Data = 0;
            }
            else if(gu8Cup_level_Default == CUP_LEVEL_2_250ML)
            {
                mu16Data = 1;
            }
            else if(gu8Cup_level_Default == CUP_LEVEL_3_500ML)
            {
                mu16Data = 2;
            }
            else if(gu8Cup_level_Default == CUP_LEVEL_4_1000ML)
            {
                mu16Data = 3;
            }
        break;

        case WIFI_FUNC_0078_COLD_TARGET_TEMP_TIME :
            /*..hui [23-6-23���� 4:16:22] �ü� ��ǥ �µ� ���� �ð�..???..*/
            if( bit_wifi_comp == CLEAR )
            {
                if( bit_wifi_comp_on_time_update == SET )
                {
                    bit_wifi_comp_on_time_update = CLEAR;
                    mu16Data = gu16_cold_target_reach_timer_sec;
                    gu32_cold_target_reach_timer_ms = 0;
                    gu16_cold_target_reach_timer_sec = 0;
                }
                else
                {
                    mu16Data = 0;
                }
            }
            else
            {
                bit_wifi_comp_on_time_update = CLEAR;
                mu16Data = 0;
            }
        break;

        case WIFI_FUNC_0083_POWER_SAVING_STATUS :       /* ���� ���ۻ��� */
            if( bit_sleep_mode_start == SET)
          	{
          		if(bit_wifi_first_sleep_mode == CLEAR)
                {
                    bit_wifi_first_sleep_mode = SET;
                    mu16Data = 1;
                }
                else
                {
                    // mu16Data = 2;
                    mu16Data = 1;
                }
            }
            else
            {
                bit_wifi_first_sleep_mode = CLEAR;
                mu16Data = 0;
            }

        break;

        case WIFI_FUNC_008C_COMP_STATUS :
            if( F_Comp_Output == SET )
            {
                if( bit_wifi_first_comp == CLEAR )
                {
                    bit_wifi_first_comp = SET;
                    mu16Data = 1;
                }
                else
                {
                    // mu16Data = 2;
                    mu16Data = 1;
                }
            }
            else
            {
                bit_wifi_first_comp = CLEAR;
                mu16Data = 0;
            }
        break;

        case WIFI_FUNC_008F_ICE_FULL_STATUS:        /* �������� ���� */
            if( F_IceFull == SET )
            {
                if( bit_wifi_first_ice_full == CLEAR )
                {
                    bit_wifi_first_ice_full = SET;
                    mu16Data = 1;
                }
                else
                {
                    mu16Data = 2;
                }
            }
            else
            {
                bit_wifi_first_ice_full = CLEAR;
                mu16Data = 0;
            }

        break;

        case WIFI_FUNC_0093_ICEMAKING_STAUTS :
            mu16Data = gu8_wifi_ice_make_state;
        break;

        case WIFI_FUNC_0095_ICEMAKING_COMPLET_TIME :            /* �����Ϸ� �ð� */
            if( gu8IceStep == STATE_50_ICE_FULL_IR_CHECK )
            {
                mu16Data = (U16)(gu32_wifi_ice_make_time / 10);
                gu32_wifi_ice_make_time = 0;
            }
            else
            {
                mu16Data = 0;
            }
        break;

        case WIFI_FUNC_0096_DEICING_COMPLET_TIME :              /* Ż���Ϸ� �ð� */

            if( gu8IceStep == STATE_50_ICE_FULL_IR_CHECK )
            {
                mu16Data = (U16)(gu32_wifi_ice_heater_timer / 10);      // ����Ÿ�̸� �� ������ ��ȯ
                gu32_wifi_ice_heater_timer = 0;
            }
            else
            {
                mu16Data = 0;
            }
        break;

        case WIFI_FUNC_009B_DRAINTANK_LOW_WLEVEL :              /* �巹����ũ ���������� ���� */

            if( F_drain_water_level_low == SET )
            {
                if( bit_wifi_first_drain_low == CLEAR )
                {
                    bit_wifi_first_drain_low = SET;
                    mu16Data = 1;
                }
                else
                {
                    mu16Data = 2;
                }
            }
            else
            {
                bit_wifi_first_drain_low = CLEAR;
                mu16Data = 0;
            }

        break;
        case WIFI_FUNC_009C_DRAINTANK_HIGH_WLEVEL :             /* �巹����ũ ���������� ���� */

            if( F_drain_water_level_high == SET )
            {
                if( bit_wifi_first_drain_high == CLEAR )
                {
                    bit_wifi_first_drain_high = SET;
                    mu16Data = 1;
                }
                else
                {
                    mu16Data = 2;
                }
            }
            else
            {
                bit_wifi_first_drain_high = CLEAR;
                mu16Data = 0;
            }

        break;

        case WIFI_FUNC_00A0_COLD_TANK_TEMP :                    /* �ü���ũ �µ� */
            mu16Data = gu8_Cold_Temperature_One_Degree;
        break;

        // case WIFI_FUNC_00AC_FILTER1_WATER_USAGE:                /* ����1 ���� ��뷮 */
        //     mu16Data = gu16_water_usage_ino_filter;
        // break;

        // case WIFI_FUNC_00AD_FILTER1_UASGE_DAY:                  /* ����1 �ܿ� ��� �ð� */
        //     mu16Data = 99;
        // break;

        case WIFI_FUNC_00B5_FILTER_FLUSHING :
            mu16Data = gu8_wifi_flushing_state;
        break;

        case WIFI_FUNC_00C1_HOTTEMP_USE_SEL0:
            if( Bit0_Temp_Select__45oC == SET )
            {
                mu16Data = 1;
            }
            else
            {
                mu16Data = 0;
            }
			break;

		case WIFI_FUNC_00C2_HOTTEMP_USE_SEL1 :

            if( Bit1_Temp_Select__50oC == SET )
            {
                mu16Data = 1;
            }
            else
            {
                mu16Data = 0;
            }
			break;

        break;

		case WIFI_FUNC_00C3_HOTTEMP_USE_SEL2 :

            if( Bit2_Temp_Select__55oC == SET )
            {
                mu16Data = 1;
            }
            else
            {
                mu16Data = 0;
            }
			break;

        break;

		case WIFI_FUNC_00C4_HOTTEMP_USE_SEL3:

            if( Bit3_Temp_Select__60oC == SET )
            {
                mu16Data = 1;
            }
            else
            {
                mu16Data = 0;
            }
			break;

        break;

		case WIFI_FUNC_00C5_HOTTEMP_USE_SEL4 :

            if( Bit4_Temp_Select__65oC == SET )
            {
                mu16Data = 1;
            }
            else
            {
                mu16Data = 0;
            }
			break;

        break;

		case WIFI_FUNC_00C6_HOTTEMP_USE_SEL5 :

            if( Bit5_Temp_Select__70oC == SET )
            {
                mu16Data = 1;
            }
            else
            {
                mu16Data = 0;
            }
			break;

        break;

		case WIFI_FUNC_00C7_HOTTEMP_USE_SEL6 :

            if( Bit6_Temp_Select__75oC == SET )
            {
                mu16Data = 1;
            }
            else
            {
                mu16Data = 0;
            }
			break;

        break;

		case WIFI_FUNC_00C8_HOTTEMP_USE_SEL7 :

            if( Bit7_Temp_Select__80oC == SET )
            {
                mu16Data = 1;
            }
            else
            {
                mu16Data = 0;
            }
			break;

        break;

		case WIFI_FUNC_00C9_HOTTEMP_USE_SEL8 :

            if( Bit8_Temp_Select__85oC == SET )
            {
                mu16Data = 1;
            }
            else
            {
                mu16Data = 0;
            }
			break;

        break;

		case WIFI_FUNC_00CA_HOTTEMP_USE_SEL9 :

            if( Bit9_Temp_Select__90oC == SET )
            {
                mu16Data = 1;
            }
            else
            {
                mu16Data = 0;
            }
			break;

        break;
        case WIFI_FUNC_00CB_WATER_EXTRACT_STATUS :
            if( (F_WaterOut == SET) )
            {
                mu16Data = 1U;
            }
            else
            {
                mu16Data = 0U;
            }
        break;

        case WIFI_FUNC_00CC_ICE_EXTRACT_STATUS :
            if( (F_IceOut == SET) )
            {
                mu16Data = 1U;
            }
            else
            {
                mu16Data = 0U;
            }
        break;

        case WIFI_FUNC_00CD_SETUP_MODE_STATUS :
            if( (gu8_Led_Display_Step == LED_Display__SETTING))
            {
                mu16Data = 1U;
            }
            else
            {
                mu16Data = 0U;
            }
        break;

		case WIFI_FUNC_00CE_HOTTEMP_USE_SEL10 :

            if( Bit10_Temp_Select__95oC == SET )
            {
                mu16Data = 1;
            }
            else
            {
                mu16Data = 0;
            }
			break;

				break;

		case WIFI_FUNC_00CF_HOTTEMP_USE_SEL11 :

            if( Bit11_Temp_Select__100oC == SET )
            {
                mu16Data = 1;
            }
            else
            {
                mu16Data = 0;
            }
			break;

        break;

        //LSH zzang
        case WIFI_FUNC_00E1_MYWATER_TEMP_1 :                /* MY WATER 1 �µ� : ��� */
            mu16Data = my_setting[MY_INDEX_RAMEN].temp;
        break;

        case WIFI_FUNC_00E2_MYWATER_QUANTITY_1 :            /* MY WATER 1 �뷮 : ��� */
            mu16Data = my_setting[MY_INDEX_RAMEN].amount;
        break;

        case WIFI_FUNC_00E3_MYWATER_RECIPE_1 :              /* MY WATER 1 ������ : ��� */
            mu16Data = 4; // ramen
        break;

        case WIFI_FUNC_00E4_MYWATER_TEMP_2 :                /* MY WATER 2 �µ� : �帳Ŀ�� */
            mu16Data = my_setting[MY_INDEX_DRIPCOFFEE].temp;
        break;

        case WIFI_FUNC_00E5_MYWATER_QUANTITY_2 :            /* MY WATER 2 �뷮 : �帳Ŀ�� */
            mu16Data = my_setting[MY_INDEX_DRIPCOFFEE].amount;
        break;

        case WIFI_FUNC_00E6_MYWATER_RECIPE_2 :              /* MY WATER 2 ������ : �帳Ŀ�� */
            mu16Data = 2; // drip coffee
        break;

        case WIFI_FUNC_00E7_MYWATER_TEMP_3 :                /* MY WATER 3 �µ� : �� */
            mu16Data = my_setting[MY_INDEX_TEA].temp;
        break;

        case WIFI_FUNC_00E8_MYWATER_QUANTITY_3 :            /* MY WATER 3 �뷮 : �� */
            mu16Data = my_setting[MY_INDEX_TEA].amount;
        break;

        case WIFI_FUNC_00E9_MYWATER_RECIPE_3 :              /* MY WATER 3 ������ : �� */
            mu16Data = 5;
        break;

        case WIFI_FUNC_00EA_MYWATER_TEMP_4 :                /* MY WATER 4 �µ� : MY1�� �µ� */
            mu16Data = my_setting[MY_INDEX_MY1].temp;
        break;

        case WIFI_FUNC_00EB_MYWATER_QUANTITY_4 :            /* MY WATER 4 �뷮 : MY1�� �뷮 */
            mu16Data = my_setting[MY_INDEX_MY1].amount;
        break;

        case WIFI_FUNC_00EC_MYWATER_RECIPE_4 :              /* MY WATER 4 ������ : MY1�� ������ */
            mu16Data = 7;
        break;

        case WIFI_FUNC_00ED_MYWATER_TEMP_5 :                /* MY WATER 5 �µ� : MY2�� �µ� */
            mu16Data = my_setting[MY_INDEX_MY2].temp;
        break;

        case WIFI_FUNC_00EE_MYWATER_QUANTITY_5 :            /* MY WATER 5 �뷮 : MY2�� �뷮 */
            mu16Data = my_setting[MY_INDEX_MY2].amount;
        break;

        case WIFI_FUNC_00EF_MYWATER_RECIPE_5 :              /* MY WATER 5 ������ : MY2�� ������ */
            mu16Data = 8;
        break;

        case WIFI_FUNC_00F0_MYWATER_TEMP_6:                 /* MY WATER 6 �µ� : 6 ~ 100�� (MY3�� �µ�) */
            mu16Data = my_setting[MY_INDEX_MY3].temp;
        break;

        case WIFI_FUNC_00F1_MYWATER_QUANTITY_6:             /* MY WATER 6 �뷮 : 90 ~ 1000ml (MY3�� �뷮) */
            mu16Data = my_setting[MY_INDEX_MY3].amount;
        break;

        case WIFI_FUNC_00F2_MYWATER_RECIPE_6:               /* MY WATER 6 ������ : 0 ~ 9 (MY3�� ������) */
            mu16Data = 9;
        break;

		case WIFI_FUNC_0101_STERILIZE_REMAINING_DAY :
            if(gu8_periodic_ster_count <= 3)
            {
                mu16Data = (3 - gu8_periodic_ster_count);
            }
            else
            {
                mu16Data = 0;
            }
		break;

        //LSH zzang
        case WIFI_FUNC_0107_MYWATER_ENABLE_1 :              /* MY WATER 1 ������� (0 : ������, 1 : �����) */
            mu16Data = my_setting[MY_INDEX_RAMEN].use;
        break;

        case WIFI_FUNC_0108_MYWATER_ENABLE_2 :              /* MY WATER 2 ������� (0 : ������, 1 : �����) */
            mu16Data = my_setting[MY_INDEX_DRIPCOFFEE].use;
        break;

        case WIFI_FUNC_0109_MYWATER_ENABLE_3 :              /* MY WATER 3 ������� (0 : ������, 1 : �����) */
            mu16Data = my_setting[MY_INDEX_TEA].use;
        break;

        case WIFI_FUNC_010A_MYWATER_ENABLE_4 :              /* MY WATER 4 ������� (0 : ������, 1 : �����) */
            mu16Data = my_setting[MY_INDEX_MY1].use;
        break;

        case WIFI_FUNC_010B_MYWATER_ENABLE_5 :              /* MY WATER 5 ������� (0 : ������, 1 : �����) */
            mu16Data = my_setting[MY_INDEX_MY2].use;
        break;

        case WIFI_FUNC_010C_MYWATER_ENABLE_6:               /* MY WATER 6 ������� (0 : ������, 1 : �����) */
            mu16Data = my_setting[MY_INDEX_MY3].use;
        break;

        case WIFI_FUNC_0116_AMOUNT_OF_ICE :                 /* ������ */
        /* 2025-11-04 CH.PARK ������ ǥ�� ���� */
        if(F_IceFull == SET)
        {
            mu16Data = 2;   // ���� (2)
        }
        else if( F_IceLack == CLEAR)
        {
            mu16Data = 1;   // ���� (1)
        }
        else
        {
            mu16Data = 0;   // ���� (0)
        }
        break;

        case WIFI_FUNC_011A_TRAY_HOT_STER_STATE:            /* ����Ʈ���� ���»�� ���ۻ��� */
            #if 0
            if(bit_ice_tank_ster_start == SET)
            {
                if(gu8_ice_ster_mode <= STER_MODE_PREPARE)
                {
                    mu16Data = 1;                               // ON
                }
                else if(gu8_ice_ster_mode <= STER_MODE_HOT_WAIT_STATE)
                {
                    mu16Data = 2;                               // Going
                }
                else
                {
                    mu16Data = 3;                               // Done
                }
            }
            else
            {
                mu16Data = 0;                                   // OFF
            }
            #endif
            if(bit_ice_tank_ster_start == SET && gu8_ice_ster_mode > STER_MODE_NONE_STATE)
            {
                if( bit_wifi_first_hot_ster == CLEAR )
                {
                    bit_wifi_first_hot_ster = SET;
                    mu16Data = 1;
                }
                else
                {
                    if(gu8_ice_ster_mode == STER_MODE_FINISH && gu8_wait_finish_step == 4)
                    {
                        mu16Data = 3;
                    }
                    else
                    {
                        mu16Data = 2;
                    }
                }
            }
            else
            {
                bit_wifi_first_hot_ster = CLEAR;
                mu16Data = 0;
            }
        break;

        case WIFI_FUNC_011B_TRAY_HOT_STER_HOUR:             /* ����Ʈ���� ���»�� ���� �ð� (��) */
            mu16Data = gu8_wifi_period_ster_hour;
        break;

        case WIFI_FUNC_011C_TRAY_HOT_STER_MINUTE:           /* ����Ʈ���� ���»�� ���� �ð� (��) */
            mu16Data = gu8_wifi_period_ster_minute;
        break;

        case WIFI_FUNC_011D_WAITMODE:                       /* ����� ���� (0 : �̻��, 1 : ���) */
        if(bit_waitmode_enable == SET)
		{
			mu16Data = 1;
		}
		else
		{
			mu16Data = 0;
		}
        break;

        case WIFI_FUNC_011E_ONSU_SET_NUM_MIN:               /* �ּ� �¼� ���� ���� (0 ~ 20��) */
            mu16Data = 4;   /* 4�� */
        break;

        case WIFI_FUNC_011F_DEFAULT_MY_WATER:                 /* ���� My���� ���� 1 ~ 10 */
            mu16Data = my_receipe_default + 1;
        break;

        case WIFI_FUNC_0120_HOT_SETTING_MAX_NUM:               /* �ִ� �¼� ���� ���� */
            mu16Data = 4;   /* 4�� */
        break;

        case WIFI_FUNC_00BE_QUANTITY_HALFCUP:                   /* ���� �뷮 */
            mu16Data = gu16_WaterQuantity_half;
        break;

        case WIFI_FUNC_00BF_QUANTITY_ONECUP:                    /* ���� �뷮 */
            mu16Data = gu16_WaterQuantity_one;
        break;

        case WIFI_FUNC_00C0_QUANTITY_TWOCUPS:                   /* ���� �뷮 */
            mu16Data = gu16_WaterQuantity_two;
        break;

        case WIFI_FUNC_0122_QUANTITY_FOURCUPS:                  /* ���� �뷮 */
            mu16Data = gu16_WaterQuantity_four;
        break;

        case WIFI_FUNC_0123_ICE_TYPE:                           /* ����Ÿ�� : �Ϲ�(0)/����(1) */
            if(F_IceBreak == CLEAR)
            {
                mu16Data = 0;
            }
            else
            {
                mu16Data = 1;
            }
        break;

        default:
            mu16Data = 0U;
        break;
    }
    return mu16Data;
}


/* [Set User Function Data] App Control(Server) -> Product */
void SetUserSystemFunction ( U16 mu16Func, U16 mData )
{
    switch ( mu16Func )
    {
      case WIFI_FUNC_0002_COLD_SEL :
        if( mData == SET )
        {
            F_Cold_Enable = SET;
            play_voice_setting_cold_on_124();
            stop_cold_off_flick();
        }
        else
        {
            F_Cold_Enable = CLEAR;
            play_voice_setting_cold_off_125();
        }
        break;

        case WIFI_FUNC_0003_HOT_LOCK_SEL :
            wifi_hot_lock( mData );
        break;

        case WIFI_FUNC_0005_LOCK_SEL :
     	 	wifi_all_lock( mData );
        break;

        case WIFI_FUNC_0008_ICE_SEL :
            wifi_ice_first( mData );
        break;

        case WIFI_FUNC_0031_VOICE_SEL :
            wifi_voice_select( mData );
        break;

        case WIFI_FUNC_0032_ICELOCK_SEL :
            wifi_ice_lock( mData );
        break;

        case WIFI_FUNC_0033_VOICE_VOL_SEL :
            wifi_volume( mData );
        break;

        case WIFI_FUNC_003C_COLD_STRENGTH_SEL :
            wifi_cold_temp( mData );
        break;

        case WIFI_FUNC_0040_SMART_CHECK_RQST :
            wifi_self_test( mData );
        break;

        case WIFI_FUNC_004A_WELCOME_LIGHT :
            wifi_welcome_light( mData );
        break;

        case WIFI_FUNC_0058_ICE_SIZE_SEL :
            wifi_ice_size( mData );
        break;

        case WIFI_FUNC_0059_ICE_ONOFF_SEL:
            wifi_ice_enable( mData );
        break;

        case WIFI_FUNC_005C_SMART_CHECK_RESULT :

            WifiSmartCheckResultData = (U8)mData;

            if (WifiSmartCheckResultData == CLEAR)
            {
                SetWifiBuzzSound(WIFI_BUZZER_SMART_CHECK_OK); // OK
            }
            else
            {
                SetWifiBuzzSound(WIFI_BUZZER_SMART_CHECK_ERROR); // NG
            }

        break;

        case WIFI_FUNC_005E_SLEEP_MODE_SEL :
            wifi_sleep_mode_enable( mData );
        break;

        case WIFI_FUNC_005F_SLEEP_START_HOUR :
            wifi_sleep_start_hour( mData );
            break;

        case WIFI_FUNC_0060_SLEEP_START_MIN :
            wifi_sleep_start_minute( mData );
            break;

        case WIFI_FUNC_0061_SLEEP_END_HOUR :
            wifi_sleep_finish_hour( mData );
        break;

        case WIFI_FUNC_0062_SLEEP_END_MIN :
            wifi_sleep_finish_minute( mData );
        break;

		case WIFI_FUNC_0066_DEFAULT_QUANTITY_SEL :
            wifi_default_cup( mData );
        break;

		case WIFI_FUNC_00C1_HOTTEMP_USE_SEL0 :
			wifi_set_temp( HOT_SET_TEMP____45oC, mData );
        break;

		case WIFI_FUNC_00C2_HOTTEMP_USE_SEL1 :
			wifi_set_temp( HOT_SET_TEMP____50oC, mData );
        break;

		case WIFI_FUNC_00C3_HOTTEMP_USE_SEL2 :
			wifi_set_temp( HOT_SET_TEMP____55oC, mData );
        break;

		case WIFI_FUNC_00C4_HOTTEMP_USE_SEL3 :
			wifi_set_temp( HOT_SET_TEMP____60oC, mData );
        break;

		case WIFI_FUNC_00C5_HOTTEMP_USE_SEL4 :
			wifi_set_temp( HOT_SET_TEMP____65oC, mData );
        break;

		case WIFI_FUNC_00C6_HOTTEMP_USE_SEL5 :
			wifi_set_temp( HOT_SET_TEMP____70oC, mData );
        break;

		case WIFI_FUNC_00C7_HOTTEMP_USE_SEL6 :
			wifi_set_temp( HOT_SET_TEMP____75oC, mData );
        break;

		case WIFI_FUNC_00C8_HOTTEMP_USE_SEL7 :
			wifi_set_temp( HOT_SET_TEMP____80oC, mData );
        break;

		case WIFI_FUNC_00C9_HOTTEMP_USE_SEL8 :
			wifi_set_temp( HOT_SET_TEMP____85oC, mData );
        break;

		case WIFI_FUNC_00CA_HOTTEMP_USE_SEL9 :
			wifi_set_temp( HOT_SET_TEMP____90oC, mData );
        break;

		case WIFI_FUNC_00CE_HOTTEMP_USE_SEL10 :
			wifi_set_temp( HOT_SET_TEMP____95oC, mData );
        break;

		case WIFI_FUNC_00CF_HOTTEMP_USE_SEL11 :
			wifi_set_temp( HOT_SET_TEMP____100oC, mData );
        break;

		//LSH zzang
		case WIFI_FUNC_00E1_MYWATER_TEMP_1 :
            wifi_my_temp(MY_INDEX_RAMEN, mData);
        break;

		case WIFI_FUNC_00E2_MYWATER_QUANTITY_1 :
            wifi_my_amount(MY_INDEX_RAMEN, mData);
        break;

		case WIFI_FUNC_00E4_MYWATER_TEMP_2 :
            wifi_my_temp(MY_INDEX_DRIPCOFFEE, mData);
        break;

		case WIFI_FUNC_00E5_MYWATER_QUANTITY_2 :
            wifi_my_amount(MY_INDEX_DRIPCOFFEE, mData);
        break;

		case WIFI_FUNC_00E7_MYWATER_TEMP_3 :
            wifi_my_temp(MY_INDEX_TEA, mData);
        break;

		case WIFI_FUNC_00E8_MYWATER_QUANTITY_3 :
            wifi_my_amount(MY_INDEX_TEA, mData);
        break;

		case WIFI_FUNC_00EA_MYWATER_TEMP_4 :
            wifi_my_temp(MY_INDEX_MY1, mData);
        break;

		case WIFI_FUNC_00EB_MYWATER_QUANTITY_4 :
            wifi_my_amount(MY_INDEX_MY1, mData);
        break;

		case WIFI_FUNC_00ED_MYWATER_TEMP_5 :
            wifi_my_temp(MY_INDEX_MY2, mData);
        break;

		case WIFI_FUNC_00EE_MYWATER_QUANTITY_5 :
            wifi_my_amount(MY_INDEX_MY2, mData);
        break;

		case WIFI_FUNC_00F0_MYWATER_TEMP_6 :
            wifi_my_temp(MY_INDEX_MY3, mData);
        break;

		case WIFI_FUNC_00F1_MYWATER_QUANTITY_6 :
            wifi_my_amount(MY_INDEX_MY3, mData);
        break;

		case WIFI_FUNC_0107_MYWATER_ENABLE_1 :
            wifi_my_use(MY_INDEX_RAMEN, mData);
        break;

		case WIFI_FUNC_0108_MYWATER_ENABLE_2 :
            wifi_my_use(MY_INDEX_DRIPCOFFEE, mData);
        break;

		case WIFI_FUNC_0109_MYWATER_ENABLE_3 :
            wifi_my_use(MY_INDEX_TEA, mData);
        break;

		case WIFI_FUNC_010A_MYWATER_ENABLE_4 :
            wifi_my_use(MY_INDEX_MY1, mData);
        break;

		case WIFI_FUNC_010B_MYWATER_ENABLE_5 :
            wifi_my_use(MY_INDEX_MY2, mData);
        break;

        case WIFI_FUNC_010C_MYWATER_ENABLE_6:
            wifi_my_use(MY_INDEX_MY3, mData);
        break;

		case WIFI_FUNC_011B_TRAY_HOT_STER_HOUR:
            wifi_period_ster_hour(mData);
			break;

		case WIFI_FUNC_011C_TRAY_HOT_STER_MINUTE:
            wifi_period_ster_minute(mData);
			break;

		case WIFI_FUNC_011D_WAITMODE:
            wifi_wait_mode_enable(mData);
			break;

        case WIFI_FUNC_011F_DEFAULT_MY_WATER:                 /* ���� My���� ���� 1 ~ 10 */
            wifi_my_default(mData);
        break;

        case WIFI_FUNC_00BE_QUANTITY_HALFCUP:                   /* ���� �뷮 */
            wifi_set_waterquantity( CUP_LEVEL_HALF, mData );
        break;

        case WIFI_FUNC_00BF_QUANTITY_ONECUP:                    /* ���� �뷮 */
            wifi_set_waterquantity( CUP_LEVEL_ONE, mData );
        break;

        case WIFI_FUNC_00C0_QUANTITY_TWOCUPS:                   /* ���� �뷮 */
            wifi_set_waterquantity( CUP_LEVEL_TWO, mData );
        break;

        case WIFI_FUNC_0122_QUANTITY_FOURCUPS:                  /* ���� �뷮 */
            wifi_set_waterquantity( CUP_LEVEL_FOUR, mData );
        break;

        // case WIFI_FUNC_0123_ICE_TYPE:                           /* ����Ÿ�� ���� �߰� : �Ϲ�(0)/����(1) */
        //     wifi_set_ice_type(mData);
        // break;

			default:
				break;
    }
}

/* User Data Clear (Called when wifi is not connected) */
void ClearUserSystemDataA1010 ( void )
{

}
