/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Input.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#include    "Macrodriver.h"
#include    "Global_Variable.h"
#include    "Port_Define.h"
#include    "M9_Data_Save.h"

void Save_Eeprom_Data(void);
void save_special_setting(void);
void save_ice_setting(void);
void save_function_setting(void);
void save_etc_setting(void);
void water_select_state_save(void);
void save_iot_function(void);
void wifi_data_save(void);
void water_usage_save(void);
void debug_data_save(void);
void my_setting_data_save(void);
void hot_temp_data_save(void);

void apply_before_special_setting(void);
void apply_before_ice_setting(void);
void apply_before_function_setting(void);
void apply_before_etc_setting(void);
void apply_before_iot_function_setting(void);
void apply_wifi_data(void);
void apply_water_usage_data(void);
void apply_debug_data(void);

void init_before_save_data(void);



bit F_Cold_Enable_Old;
bit F_Hot_Lock_Old;
bit F_All_Lock_Old;
//bit F_Mute_Enable_Old;
bit F_Ice_Lock_Old;
bit F_IceOn_Old;
bit bit_fast_ice_make_old;
bit F_Save_Old;
bit F_Circul_Drain_Old;
bit F_Auto_UV_Control_Old;
U8 gu8_clock_theme_Old;
U8 gu8_screen_bright_Old;
//bit F_Service_1day_Save;

U8 gu8_cup_level_default_old;
U8 gu8_WaterOutState_Old;

U8 gu8_eeprom_data_save_xmit;


bit F_Wifi_Power;
bit F_Wifi_Power_Old;

bit F_Wifi_First_Paring;
bit F_Wifi_First_Paring_Old;

bit F_Wifi_Fota_MODULE;
bit F_Wifi_Fota_MODULE_Old;

bit F_Wifi_Fota_MCU;
bit F_Wifi_Fota_MCU_Old;

bit F_Welcome_Led_OnOff;
bit F_Welcome_Led_OnOff_Old;

U8 gu8_ice_setting;
U8 gu8_ice_setting_old;


bit F_Hot_Enable_Old;
bit bit_my_cup_enable_Old;
U16 gu16_my_cup_level_old;

bit bit_install_flushing_state_old;
U8 gu8_filter_flushing_state_old;

U16 gu16_cup_level_select_old;
U8 gu8_cup_level_order_old;


U8 gu8_hot_level_order_old;
U8 gu8_hot_default_temp_old;
U16 gu16_total_usage_water_gal_save_old;


U16 gu16_filter_reset_day_neo_old;
U16 gu16_filter_reset_day_ro_old;

bit bit_periodic_ster_enable_Old;
U8 gu8_wifi_period_ster_hour_old;
U8 gu8_wifi_period_ster_minute_old;


bit bit_sleep_mode_enable_old;
U8 gu8_sleep_mode_start_hour_old;
U8 gu8_sleep_mode_start_minute_old;
U8 gu8_sleep_mode_finish_hour_old;
U8 gu8_sleep_mode_finish_minute_old;

bit bit_waitmode_enable_old;

bit bit_child_lock_enable_old;

bit bit_first_time_setting_old;

U8 gu8_fota_start_old;
U32 gu32_total_usage_water_ml_save_old;

U8 gu8_wifi_smart_unused_setting_time_old;
U8 gu8Sound_Type_Old;
U8 gu8VoiceVolumeLevel_Old;

U8 gu8AltitudeStep_Old;


U16 gu16_water_usage_neo_filter_old;
U16 gu16_water_usage_ro_filter_old;
U16 gu16_water_usage_ino_filter_old;

U16 gu16_reset_day_neo_filter_old;
U16 gu16_reset_day_ro_filter_old;
U16 gu16_reset_day_ino_filter_old;

U8 gu8_voice_language_type_old;
U8 gu8_filter_change_type_old;


U8 gu8_ice_ster_drain_stop_count_old;
U8 gu8_ice_ster_low_water_stop_count_old;
U8 gu8_wifi_filter_change_cycle_old;
U8 gu8_ct_forced_flushing_start_old;
U8 gu8CTFilterReplace_old;


U8 gu8_recover_org_fast_ice_old;
U8 gu8_return_acid_step_old;

U8 gu8_my1_setting_use_old;
U8 gu8_my1_setting_temp_old;
U16 gu16_my1_setting_amount_old;
U8 gu8_my2_setting_use_old;
U8 gu8_my2_setting_temp_old;
U16 gu16_my2_setting_amount_old;
U8 gu8_my3_setting_use_old;
U8 gu8_my3_setting_temp_old;
U16 gu16_my3_setting_amount_old;
MY_INDEX my_receipe_default_old;

U8 selected_hot_temp_0_old;
U8 selected_hot_temp_1_old;
U8 selected_hot_temp_2_old;
U8 selected_hot_temp_3_old;
U16 U16HotTemplSelect_old;

U16 gu16_WaterQuantity_half_old;
U16 gu16_WaterQuantity_one_old;
U16 gu16_WaterQuantity_two_old;
U16 gu16_WaterQuantity_four_old;

U8 gu8_drain_tank_ster_count_old;

bit bit_ice_size_old;       /* 얼음사이즈 대/소 EEPROM 데이터 추가 250224 CH.PARK */
bit bit_IceBreak_old;
/*..hui [24-12-17오후 3:33:50] 임시 컴파일 에러 해제용 eeprom 관련 나중에 정리..*/
U8 gu8_ct_forced_flushing_start;
bit bit_tray_pump_output;
U8 gu8_return_acid_step;

TYPE_BYTE          U8CTFilterReplaceB;
#define            gu8CTFilterReplace                           U8CTFilterReplaceB.byte
#define            Bit0_CT_Neo_Filter_1_Replace                 U8CTFilterReplaceB.Bit.b0
#define            Bit1_CT_Ro_Filter_2_Replace                  U8CTFilterReplaceB.Bit.b1
#define            Bit2_CT_Ino_Filter_3_Replace                 U8CTFilterReplaceB.Bit.b2

extern U8 gu8_wifi_period_ster_hour;
extern U8 gu8_wifi_period_ster_minute;
extern U8 gu8_ice_ster_drain_stop_count;
extern U8 gu8_ice_ster_low_water_stop_count;
extern U16 gu16_WaterQuantity_half;
extern U16 gu16_WaterQuantity_one;
extern U16 gu16_WaterQuantity_two;
extern U16 gu16_WaterQuantity_four;
extern bit bit_periodic_ster_enable;
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void Save_Eeprom_Data(void)
{
    switch(gu8_eeprom_data_save_xmit)
    {
        case 0:

            save_special_setting();
            gu8_eeprom_data_save_xmit++;

            break;

        case 1:

            save_ice_setting();
            gu8_eeprom_data_save_xmit++;

            break;

        case 2:

            save_function_setting();
            gu8_eeprom_data_save_xmit++;

            break;

        case 3:

            save_etc_setting();
            gu8_eeprom_data_save_xmit++;

            break;

        case 4:

            ErrorSave();
            gu8_eeprom_data_save_xmit++;

            break;

        case 5:

            debug_data_save();
            gu8_eeprom_data_save_xmit++;

            break;

        case 6:

            save_iot_function();
            gu8_eeprom_data_save_xmit++;

            break;

        case 7:

            wifi_data_save();
            gu8_eeprom_data_save_xmit++;

            break;

        case 8:

            water_usage_save();
            gu8_eeprom_data_save_xmit++;

            break;

        case 9:
            my_setting_data_save(); //LSH zzang
            gu8_eeprom_data_save_xmit++;

            break;

        case 10:
            hot_temp_data_save();
            gu8_eeprom_data_save_xmit++;

            break;

        case 11:

            gu8_eeprom_data_save_xmit++;

            break;

        case 12:

            ProcessRtc();
            gu8_eeprom_data_save_xmit = 0;

            break;

        default:

            gu8_eeprom_data_save_xmit = 0;

            break;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void save_special_setting(void)
{
    U8 mu8_enable = 0;

    /*if( F_Cold_Enable_Old != F_Cold_Enable
       || F_Hot_Lock_Old != F_Hot_Lock
       || F_All_Lock_Old != F_All_Lock
       || F_Mute_Enable_Old != F_Mute_Enable
       || gu8_cold_setting_level_old != gu8_cold_setting_level
       || F_Hot_Enable_Old != F_Hot_Enable
       || bit_install_flushing_state_old != bit_install_flushing_state
       || gu8_filter_flushing_state_old != gu8_filter_flushing_state
       || bit_first_time_setting_old != bit_first_time_setting )*/
    
    if((F_Cold_Enable_Old != F_Cold_Enable)
    || (F_Hot_Lock_Old != F_Hot_Lock)
    || (F_All_Lock_Old != F_All_Lock)
    || (F_Mute_Enable_Old != F_Mute_Enable)
    || (gu8_cold_setting_level_old != gu8_cold_setting_level)
    || (F_Hot_Enable_Old != F_Hot_Enable)
    || (bit_install_flushing_state_old != bit_install_flushing_state)
    || (gu8_filter_flushing_state_old != gu8_filter_flushing_state)
    || (bit_first_time_setting_old != bit_first_time_setting)
    || (gu8Sound_Type_Old != gu8Sound_Type)
    || (gu8VoiceVolumeLevel_Old != gu8VoiceVolumeLevel)
    || (gu8AltitudeStep_Old != gu8AltitudeStep)
    || (gu8_voice_language_type_old != gu8_voice_language_type)
    || (gu8_ct_forced_flushing_start_old != gu8_ct_forced_flushing_start)
    || (gu8CTFilterReplace_old != gu8CTFilterReplace)
    )
    {
        mu8_enable = SET;
    }
    else{}

    if(mu8_enable == SET)
    {
        apply_before_special_setting();

        gu8_eeprom_wbuf[0] = (U8)F_Cold_Enable;
        gu8_eeprom_wbuf[1] = (U8)F_Hot_Lock;
        gu8_eeprom_wbuf[2] = (U8)F_All_Lock;
        gu8_eeprom_wbuf[3] = (U8)F_Mute_Enable;
        gu8_eeprom_wbuf[4] = gu8_cold_setting_level;
        gu8_eeprom_wbuf[5] = (U8)F_Hot_Enable;
        gu8_eeprom_wbuf[6] = (U8)bit_install_flushing_state;
        gu8_eeprom_wbuf[7] = gu8_filter_flushing_state;
        gu8_eeprom_wbuf[8] = (U8)bit_first_time_setting;
        gu8_eeprom_wbuf[9] = gu8Sound_Type;
        gu8_eeprom_wbuf[10] = gu8VoiceVolumeLevel;
        gu8_eeprom_wbuf[11] = gu8AltitudeStep;
        gu8_eeprom_wbuf[12] = gu8_voice_language_type;
        gu8_eeprom_wbuf[13] = gu8_ct_forced_flushing_start;
        gu8_eeprom_wbuf[14] = gu8CTFilterReplace;

        EepromPageWrite(SPECIAL_SETTING_START_ADDR, gu8_eeprom_wbuf, SPECIAL_SETTING_LENGTH);
    }
    else{}
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void save_ice_setting(void)
{
    U8 mu8_enable = 0;

    /*..hui [20-3-31오후 4:32:27] 얼음생성이랑 얼음우선이 얼음설정 하나로 통합됨.. 꺼짐,얼음,냉수 순....*/
    if((F_Ice_Lock_Old != F_Ice_Lock)
    || (F_IceOn_Old != F_IceOn)
    || (bit_fast_ice_make_old != bit_fast_ice_make)
    || (gu8_recover_org_fast_ice_old != gu8_recover_org_fast_ice)
    || (gu8_return_acid_step_old != gu8_return_acid_step )
    || (bit_ice_size_old != bit_ice_size)
    || (bit_IceBreak_old != F_IceBreak)
    )
    {
        mu8_enable = SET;
    }
    else{}

    if(mu8_enable == SET)
    {
        apply_before_ice_setting();

        gu8_eeprom_wbuf[0] = (U8)F_Ice_Lock;
        gu8_eeprom_wbuf[1] = (U8)F_IceOn;
        gu8_eeprom_wbuf[2] = (U8)bit_fast_ice_make;
        gu8_eeprom_wbuf[3] = gu8_recover_org_fast_ice;
        gu8_eeprom_wbuf[4] = gu8_return_acid_step;
        gu8_eeprom_wbuf[5] = (U8)bit_ice_size;
        gu8_eeprom_wbuf[6] = (U8)F_IceBreak;

        EepromPageWrite(ICE_SETTING_START_ADDR, gu8_eeprom_wbuf, ICE_SETTING_LENGTH);
    }
    else{}
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void save_function_setting(void)
{
    U8 mu8_enable = 0;

    if( F_Save_Old != F_Save
       || F_Circul_Drain_Old != F_Circul_Drain
       || F_Auto_UV_Control_Old != F_Auto_UV_Control
       || F_Welcome_Led_OnOff_Old != F_Welcome_Led_OnOff

       /*|| bit_my_cup_enable_Old != bit_my_cup_enable*/
       /*|| gu16_my_cup_level_old != gu16_my_cup_level*/
       || gu8_wifi_smart_unused_setting_time_old != gu8_wifi_smart_unused_setting_time

       || bit_periodic_ster_enable_Old != bit_periodic_ster_enable
       || gu8_wifi_period_ster_hour_old != gu8_wifi_period_ster_hour
       || gu8_wifi_period_ster_minute_old != gu8_wifi_period_ster_minute

       || bit_sleep_mode_enable_old != bit_sleep_mode_enable
       || gu8_sleep_mode_start_hour_old != gu8_sleep_mode_start_hour
       || gu8_sleep_mode_start_minute_old != gu8_sleep_mode_start_minute
       || gu8_sleep_mode_finish_hour_old != gu8_sleep_mode_finish_hour
       || gu8_sleep_mode_finish_minute_old != gu8_sleep_mode_finish_minute 
       || bit_waitmode_enable_old != bit_waitmode_enable   
    )
    {
        mu8_enable = SET;
    }
    else{}

    if(mu8_enable == SET)
    {
        apply_before_function_setting();

        gu8_eeprom_wbuf[0] = (U8)F_Save;
        gu8_eeprom_wbuf[1] = (U8)F_Circul_Drain;
        gu8_eeprom_wbuf[2] = (U8)F_Auto_UV_Control;
        gu8_eeprom_wbuf[3] = (U8)F_Welcome_Led_OnOff;

        gu8_eeprom_wbuf[4] = gu8_wifi_smart_unused_setting_time;
        gu8_eeprom_wbuf[5] = (U8)0;
        gu8_eeprom_wbuf[6] = (U8)0;

        gu8_eeprom_wbuf[7] = (U8)bit_periodic_ster_enable;
        gu8_eeprom_wbuf[8] = gu8_wifi_period_ster_hour;
        gu8_eeprom_wbuf[9] = gu8_wifi_period_ster_minute;

        gu8_eeprom_wbuf[10] = (U8)bit_sleep_mode_enable;
        gu8_eeprom_wbuf[11] = gu8_sleep_mode_start_hour;
        gu8_eeprom_wbuf[12] = gu8_sleep_mode_start_minute;
        gu8_eeprom_wbuf[13] = gu8_sleep_mode_finish_hour;
        gu8_eeprom_wbuf[14] = gu8_sleep_mode_finish_minute;
        gu8_eeprom_wbuf[15] = (U8)bit_waitmode_enable;

        EepromPageWrite(FUNCTION_SETTING_START_ADDR, gu8_eeprom_wbuf, FUNCTION_SETTING_LENGTH);
    }
    else{}
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void save_etc_setting(void)
{

    if( gu8_cup_level_default_old != gu8Cup_level_Default )
    {
        gu8_cup_level_default_old = gu8Cup_level_Default;

        gu8_eeprom_wbuf[0] = (U8)gu8Cup_level_Default;

        EepromPageWrite(ETC_SAVE_START_ADDR, gu8_eeprom_wbuf, 1);
    }
    else{}

    water_select_state_save();
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void water_select_state_save(void)
{
    /*..hui [18-1-10오전 11:15:29] 온수 선택상태이거나 선택 후 복귀 이전이면 저장안함..*/
    if(u8WaterOutState == HOT_WATER_SELECT || F_WaterOut_Change_State == SET)
    {
        return;
    }
    else{}

    if(gu8_WaterOutState_Old != u8WaterOutState)
    {
        gu8_WaterOutState_Old = u8WaterOutState;

        EepromByteWrite(EEPROM_ADDR4_WATER_SELECT_DEFAULT, u8WaterOutState);
    }
    else{}
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void save_iot_function(void)
{
    U8 mu8_enable = 0;

    if( gu16_WaterQuantity_half != gu16_WaterQuantity_half_old
    || gu16_WaterQuantity_one != gu16_WaterQuantity_one_old
    || gu16_WaterQuantity_two != gu16_WaterQuantity_two_old
    || gu16_WaterQuantity_four != gu16_WaterQuantity_four_old
    || gu8_fota_start_old != gu8_fota_start 
    || gu8_drain_tank_ster_count_old != gu8_drain_tank_ster_count
    )
    {
        mu8_enable = SET;
    }
    else{}

    if(mu8_enable == SET)
    {
        apply_before_iot_function_setting();
        
        gu8_eeprom_wbuf[0] = (U8)(gu16_WaterQuantity_half/(U16)256);
        gu8_eeprom_wbuf[1] = (U8)(gu16_WaterQuantity_half%(U16)256);
        gu8_eeprom_wbuf[2] = (U8)(gu16_WaterQuantity_one/(U16)256);
        gu8_eeprom_wbuf[3] = (U8)(gu16_WaterQuantity_one%(U16)256);
        gu8_eeprom_wbuf[4] = (U8)(gu16_WaterQuantity_two/(U16)256);
        gu8_eeprom_wbuf[5] = (U8)(gu16_WaterQuantity_two%(U16)256);
        gu8_eeprom_wbuf[6] = (U8)(gu16_WaterQuantity_four/(U16)256);
        gu8_eeprom_wbuf[7] = (U8)(gu16_WaterQuantity_four%(U16)256);
        gu8_eeprom_wbuf[8] = gu8_fota_start;
        gu8_eeprom_wbuf[9] = gu8_drain_tank_ster_count;

        EepromPageWrite(IOT_FUNCTION_ADDR, gu8_eeprom_wbuf, IOT_FUNCTION_LENGTH);
    }
    else{}

}

void my_setting_data_save(void)
{//LSH zzang
    U8 mu8_enable = 0;

    if( my_setting[MY_INDEX_MY1].temp != gu8_my1_setting_temp_old
    || my_setting[MY_INDEX_MY1].amount != gu16_my1_setting_amount_old
    || my_setting[MY_INDEX_MY1].use != gu8_my1_setting_use_old
    || my_setting[MY_INDEX_MY2].temp != gu8_my2_setting_temp_old
    || my_setting[MY_INDEX_MY2].amount != gu16_my2_setting_amount_old
    || my_setting[MY_INDEX_MY2].use != gu8_my2_setting_use_old 
    || my_setting[MY_INDEX_MY3].temp != gu8_my3_setting_temp_old
    || my_setting[MY_INDEX_MY3].amount != gu16_my3_setting_amount_old
    || my_setting[MY_INDEX_MY3].use != gu8_my3_setting_use_old
    || my_receipe_default != my_receipe_default_old)
    {
        mu8_enable = SET;
    }
    else
    {
        mu8_enable = CLEAR;
    }

    if(mu8_enable == SET)
    {
        gu8_my1_setting_temp_old = my_setting[MY_INDEX_MY1].temp;
        gu16_my1_setting_amount_old = my_setting[MY_INDEX_MY1].amount;
        gu8_my1_setting_use_old = my_setting[MY_INDEX_MY1].use;
        
        gu8_my2_setting_temp_old = my_setting[MY_INDEX_MY2].temp;
        gu16_my2_setting_amount_old = my_setting[MY_INDEX_MY2].amount;
        gu8_my2_setting_use_old = my_setting[MY_INDEX_MY2].use;

        gu8_my3_setting_temp_old = my_setting[MY_INDEX_MY3].temp;
        gu16_my3_setting_amount_old = my_setting[MY_INDEX_MY3].amount;
        gu8_my3_setting_use_old = my_setting[MY_INDEX_MY3].use;

        my_receipe_default_old = my_receipe_default;
        
        gu8_eeprom_wbuf[0] = (U8)gu8_my1_setting_temp_old;
        gu8_eeprom_wbuf[1] = (U8)(gu16_my1_setting_amount_old/(U16)256);
        gu8_eeprom_wbuf[2] = (U8)(gu16_my1_setting_amount_old%(U16)256);
        gu8_eeprom_wbuf[3] = (U8)gu8_my1_setting_use_old;

        gu8_eeprom_wbuf[4] = (U8)gu8_my2_setting_temp_old;
        gu8_eeprom_wbuf[5] = (U8)(gu16_my2_setting_amount_old/(U16)256);
        gu8_eeprom_wbuf[6] = (U8)(gu16_my2_setting_amount_old%(U16)256);
        gu8_eeprom_wbuf[7] = (U8)gu8_my2_setting_use_old;

        gu8_eeprom_wbuf[8] = (U8)gu8_my3_setting_temp_old;
        gu8_eeprom_wbuf[9] = (U8)(gu16_my3_setting_amount_old/(U16)256);
        gu8_eeprom_wbuf[10] = (U8)(gu16_my3_setting_amount_old%(U16)256);
        gu8_eeprom_wbuf[11] = (U8)gu8_my3_setting_use_old;
        gu8_eeprom_wbuf[12] = (U8)my_receipe_default_old;

        EepromPageWrite(MY_DATA_ADDR, gu8_eeprom_wbuf, MY_DATA_LENGTH);
    }
    else{}
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void hot_temp_data_save(void)
{
    U8 mu8_enable = 0;

    if(U16HotTemplSelect != U16HotTemplSelect_old)
    {
        mu8_enable = SET;
    }
    else
    {
        mu8_enable = CLEAR;
    }

    if(mu8_enable == SET)
    {        
        U16HotTemplSelect_old = U16HotTemplSelect;
        sort_hot_temp();
        selected_hot_temp_0_old = selected_hot_temp[0];
        selected_hot_temp_1_old = selected_hot_temp[1];
        selected_hot_temp_2_old = selected_hot_temp[2];
        selected_hot_temp_3_old = selected_hot_temp[3];
       
        gu8_eeprom_wbuf[0] = selected_hot_temp_0_old;
        gu8_eeprom_wbuf[1] = selected_hot_temp_1_old;
        gu8_eeprom_wbuf[2] = selected_hot_temp_2_old;
        gu8_eeprom_wbuf[3] = selected_hot_temp_3_old;

        EepromPageWrite(HOT_TEMP_ADDR, gu8_eeprom_wbuf, HOT_TEMP_LENGTH);

        gu8_hot_setting_temperature = selected_hot_temp[gu8_hot_water_select_index];
    }
    else{}
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void wifi_data_save(void)
{
    U8 mu8_enable = 0;

    if( F_Wifi_Power != F_Wifi_Power_Old
    || F_Wifi_First_Paring != F_Wifi_First_Paring_Old
    || F_Wifi_Fota_MODULE != F_Wifi_Fota_MODULE_Old
    || F_Wifi_Fota_MCU != F_Wifi_Fota_MCU_Old )
    {
        mu8_enable = SET;
    }
    else
    {
        mu8_enable = CLEAR;
    }

    if(mu8_enable == SET)
    {
        apply_wifi_data();

        gu8_eeprom_wbuf[0] = (U8)F_Wifi_Power;
        gu8_eeprom_wbuf[1] = (U8)F_Wifi_First_Paring;
        gu8_eeprom_wbuf[2] = (U8)F_Wifi_Fota_MODULE;
        gu8_eeprom_wbuf[3] = (U8)F_Wifi_Fota_MCU;

        EepromPageWrite(WIFI_DATA_ADDR, gu8_eeprom_wbuf, WIFI_DATA_LENGTH);
    }
    else{}
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void water_usage_save(void)
{
    U8 mu8_enable = 0;
    U16 mu16_a = 0;
    U16 mu16_b = 0;

    if( gu16_water_usage_neo_filter != gu16_water_usage_neo_filter_old
       || gu16_water_usage_ro_filter != gu16_water_usage_ro_filter_old
       || gu16_water_usage_ino_filter != gu16_water_usage_ino_filter_old

       || gu16_reset_hour_neo_filter != gu16_reset_day_neo_filter_old
       || gu16_reset_hour_ro_filter != gu16_reset_day_ro_filter_old
       || gu16_reset_hour_ino_filter != gu16_reset_day_ino_filter_old

       || gu8_filter_change_type != gu8_filter_change_type_old )
    {
        mu8_enable = SET;
    }
    else
    {
        mu8_enable = CLEAR;
    }

    if( mu8_enable == SET )
    {
        apply_water_usage_data();

        gu8_eeprom_wbuf[0] = (U8)(gu16_water_usage_neo_filter/(U16)256);
        gu8_eeprom_wbuf[1] = (U8)(gu16_water_usage_neo_filter%(U16)256);

        gu8_eeprom_wbuf[2] = (U8)(gu16_water_usage_ro_filter/(U16)256);
        gu8_eeprom_wbuf[3] = (U8)(gu16_water_usage_ro_filter%(U16)256);

        gu8_eeprom_wbuf[4] = (U8)(gu16_water_usage_ino_filter/(U16)256);
        gu8_eeprom_wbuf[5] = (U8)(gu16_water_usage_ino_filter%(U16)256);

        gu8_eeprom_wbuf[6] = (U8)(gu16_reset_hour_neo_filter/(U16)256);
        gu8_eeprom_wbuf[7] = (U8)(gu16_reset_hour_neo_filter%(U16)256);

        gu8_eeprom_wbuf[8] = (U8)(gu16_reset_hour_ro_filter/(U16)256);
        gu8_eeprom_wbuf[9] = (U8)(gu16_reset_hour_ro_filter%(U16)256);

        gu8_eeprom_wbuf[10] = (U8)(gu16_reset_hour_ino_filter/(U16)256);
        gu8_eeprom_wbuf[11] = (U8)(gu16_reset_hour_ino_filter%(U16)256);

        gu8_eeprom_wbuf[12] = gu8_filter_change_type;

        EepromPageWrite(WATER_USAGE_ADDR, gu8_eeprom_wbuf, WATER_USAGE_LENGTH);
    }
    else{}

}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void debug_data_save(void)
{
    U8 mu8_enable = 0;

    if( gu8_ice_ster_drain_stop_count != gu8_ice_ster_drain_stop_count_old
       || gu8_ice_ster_low_water_stop_count != gu8_ice_ster_low_water_stop_count_old )
    {
        mu8_enable = SET;
    }
    else
    {
        mu8_enable = CLEAR;
    }

    if(mu8_enable == SET)
    {
        apply_debug_data();

        gu8_eeprom_wbuf[0] = gu8_ice_ster_drain_stop_count;
        gu8_eeprom_wbuf[1] = gu8_ice_ster_low_water_stop_count;

        EepromPageWrite(DEBUG_DATA_ADDR, gu8_eeprom_wbuf, DEBUG_DATA_LENGTH);
    }
    else{}
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void apply_before_special_setting(void)
{
    F_Cold_Enable_Old = F_Cold_Enable;
    F_Hot_Lock_Old = F_Hot_Lock;
    F_All_Lock_Old = F_All_Lock;
    F_Mute_Enable_Old = F_Mute_Enable;
    gu8_cold_setting_level_old = gu8_cold_setting_level;
    F_Hot_Enable_Old = F_Hot_Enable;
    bit_install_flushing_state_old = bit_install_flushing_state;
    gu8_filter_flushing_state_old = gu8_filter_flushing_state;
    bit_first_time_setting_old = bit_first_time_setting;

    gu8Sound_Type_Old = gu8Sound_Type;
    gu8VoiceVolumeLevel_Old = gu8VoiceVolumeLevel;

    gu8AltitudeStep_Old = gu8AltitudeStep;
    gu8_voice_language_type_old = gu8_voice_language_type;

    gu8_ct_forced_flushing_start_old = gu8_ct_forced_flushing_start;
    gu8CTFilterReplace_old = gu8CTFilterReplace;

}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void apply_before_ice_setting(void)
{
    F_Ice_Lock_Old = F_Ice_Lock;
    F_IceOn_Old = F_IceOn;
    bit_fast_ice_make_old = bit_fast_ice_make;
    gu8_recover_org_fast_ice_old = gu8_recover_org_fast_ice;
    gu8_return_acid_step_old = gu8_return_acid_step;
    /* EEPROM 데이터 추가하면 여기도 추가 해야됨 250228 CH.PARK */
    bit_ice_size_old = bit_ice_size;
    bit_IceBreak_old = F_IceBreak;
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void apply_before_function_setting(void)
{
    F_Save_Old = F_Save;
    F_Circul_Drain_Old = F_Circul_Drain;
    F_Auto_UV_Control_Old = F_Auto_UV_Control;
    F_Welcome_Led_OnOff_Old = F_Welcome_Led_OnOff;

    /*bit_my_cup_enable_Old = bit_my_cup_enable;*/
    /*gu16_my_cup_level_old = gu16_my_cup_level;*/
    gu8_wifi_smart_unused_setting_time_old = gu8_wifi_smart_unused_setting_time;

    bit_periodic_ster_enable_Old = bit_periodic_ster_enable;
    gu8_wifi_period_ster_hour_old = gu8_wifi_period_ster_hour;
    gu8_wifi_period_ster_minute_old = gu8_wifi_period_ster_minute;

    bit_sleep_mode_enable_old = bit_sleep_mode_enable;
    gu8_sleep_mode_start_hour_old = gu8_sleep_mode_start_hour;
    gu8_sleep_mode_start_minute_old = gu8_sleep_mode_start_minute;
    gu8_sleep_mode_finish_hour_old = gu8_sleep_mode_finish_hour;
    gu8_sleep_mode_finish_minute_old = gu8_sleep_mode_finish_minute;
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void apply_before_etc_setting(void)
{
    gu8_cup_level_default_old = gu8Cup_level_Default;
    gu8_WaterOutState_Old = u8WaterOutState;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void apply_before_iot_function_setting(void)
{
    gu16_WaterQuantity_half_old = gu16_WaterQuantity_half;
    gu16_WaterQuantity_one_old = gu16_WaterQuantity_one;
    gu16_WaterQuantity_two_old = gu16_WaterQuantity_two;
    gu16_WaterQuantity_four_old = gu16_WaterQuantity_four;
    
    gu8_fota_start_old = gu8_fota_start;
    
    gu8_drain_tank_ster_count_old = gu8_drain_tank_ster_count;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void apply_wifi_data(void)
{
    F_Wifi_First_Paring_Old = F_Wifi_First_Paring;
    F_Wifi_Power_Old = F_Wifi_Power;
    F_Wifi_Fota_MODULE_Old = F_Wifi_Fota_MODULE;
    F_Wifi_Fota_MCU_Old = F_Wifi_Fota_MCU;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void apply_water_usage_data(void)
{
    gu16_water_usage_neo_filter_old = gu16_water_usage_neo_filter;
    gu16_water_usage_ro_filter_old = gu16_water_usage_ro_filter;
    gu16_water_usage_ino_filter_old = gu16_water_usage_ino_filter;

    gu16_reset_day_neo_filter_old = gu16_reset_hour_neo_filter;
    gu16_reset_day_ro_filter_old = gu16_reset_hour_ro_filter;
    gu16_reset_day_ino_filter_old = gu16_reset_hour_ino_filter;

    gu8_filter_change_type_old = gu8_filter_change_type;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void apply_debug_data(void)
{
    gu8_ice_ster_drain_stop_count_old = gu8_ice_ster_drain_stop_count;
    gu8_ice_ster_low_water_stop_count_old = gu8_ice_ster_low_water_stop_count;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void init_before_save_data(void)
{
    apply_before_special_setting();
    apply_before_ice_setting();
    apply_before_function_setting();
    apply_before_etc_setting();
    apply_before_iot_function_setting();

    apply_wifi_data();
    apply_water_usage_data();

    apply_debug_data();
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/




