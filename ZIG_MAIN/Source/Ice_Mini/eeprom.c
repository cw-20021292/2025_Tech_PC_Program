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
#include    "Eeprom.h"

void EEPROM_PRIMARY(void);
void eeprom_data_load(void);
void load_spetial_setting(void);
void load_ice_setting(void);
void load_function_setting(void);
void load_etc_setting(void);
void load_memento(void);
void load_iot_function(void);
void load_wifi(void);
void load_my_setting(void);
void load_hot_temp_setting(void);
void load_water_usage(void);
void load_debug_data(void);

void eeprom_initial(void);
void initial_spetial_setting(void);
void initial_ice_setting(void);
void initial_function_setting(void);
void initial_etc_setting(void);
void initial_memento(void);
void initial_iot_function(void);
void initial_wifi(void);
void initial_my_setting(void);
void initial_hot_temp_setting(void);
void initial_water_usage(void);
void initial_debug_data(void);

void eeprom_test(void);
void eeprom_write_cycle_time(void);
void EepromByteWrite( U16 mu16Addr, U8 mu8Data );
void EepromPageWrite(U16 mu16Addr, U8 *pData, U8 mu8len );
void EepromPageWriteSub( U16 mu16Addr, U8 *pData, U8 mu8len );
void EepromSeqRead( U16 mu16Addr, U8 *pData, U8 mu16Length );





bit bit_eeprom_err;


U8 gu8_eeprom_wbuf[16];
U8 gu8_eeprom_rbuf[16];

U8 u811111;
U16 gu16_eep_cnt;

/***********************************************************************************************************************/
/**
 * @brief ���� �� EEPROM �ʱ�ȭ �Լ�
 *
 */
void EEPROM_PRIMARY(void)
{
    U8 mu8_eep_checkA = 0;
    U8 mu8_eep_checkB = 0;
/***********************************************************************************************************************/
    EepromSeqRead(EEPROM_ADDR_CHECK_START, &mu8_eep_checkA, 1);
/***********************************************************************************************************************/
    EepromSeqRead(EEPROM_ADDR_CHECK_END, &mu8_eep_checkB, 1);
/***********************************************************************************************************************/
    if((mu8_eep_checkA != EEPROM_CHECK_A_VALUE)
    || (mu8_eep_checkB != EEPROM_CHECK_B_VALUE)
    )
    {
        eeprom_initial();
    }
    else
    {
        eeprom_data_load();
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void eeprom_data_load(void)
{
    U8 mu8_Read_Buffer[16];

    load_spetial_setting();
    load_ice_setting();
    load_function_setting();
    load_etc_setting();
    load_memento();
    load_iot_function();
    load_my_setting();      //LSH zzang
    load_hot_temp_setting();

    /*..hui [21-7-15���� 1:04:57] wifi ������ �ҷ����°͵� ��������..*/
    load_wifi();

    load_water_usage();

    load_debug_data();
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void load_spetial_setting(void)
{
    EepromSeqRead(SPECIAL_SETTING_START_ADDR, gu8_eeprom_rbuf, SPECIAL_SETTING_LENGTH);

    //=========================================================================================//
    /*.. �ü� ���? ����..*/
    #if 1
    if(gu8_eeprom_rbuf[0] <= 1)
    {
        F_Cold_Enable = gu8_eeprom_rbuf[0];
    }
    else
    {
        F_Cold_Enable = SET;
        EepromByteWrite(EEPROM_ADDR1_COLD_SETTING, (U8)F_Cold_Enable);
    }
    #endif
    //=========================================================================================//
    /*.. �¼� ���? ����..*/
    if(gu8_eeprom_rbuf[1] <= 1)
    {
        F_Hot_Lock = gu8_eeprom_rbuf[1];
    }
    else
    {
        F_Hot_Lock = CLEAR;
        EepromByteWrite(EEPROM_ADDR1_HOT_LOCK, (U8)F_Hot_Lock);
    }
    //=========================================================================================//
    /*.. ��ü ���? ����..*/
    if(gu8_eeprom_rbuf[2] <= 1)
    {
        F_All_Lock = gu8_eeprom_rbuf[2];
    }
    else
    {
        F_All_Lock = CLEAR;
        EepromByteWrite(EEPROM_ADDR1_ALL_LOCK, (U8)F_All_Lock);
    }
    //=========================================================================================//
    /*.. ���Ұ� ���?..*/
    if(gu8_eeprom_rbuf[3] <= 1)
    {
        F_Mute_Enable = gu8_eeprom_rbuf[3];
    }
    else
    {
        F_Mute_Enable = CLEAR;
        EepromByteWrite(EEPROM_ADDR1_MUTE_SETTING, (U8)F_Mute_Enable);
    }
    //=========================================================================================//
    /*..hui [20-1-6���� 7:54:35] �ü� �µ� ����..*/
    if(gu8_eeprom_rbuf[4] <= COLD_TEMP_LOW)
    {
        gu8_cold_setting_level = gu8_eeprom_rbuf[4];
    }
    else
    {
        gu8_cold_setting_level = COLD_TEMP_HIGH;
        EepromByteWrite(EEPROM_ADDR1_COLD_LEVEL, (U8)gu8_cold_setting_level);
    }
    //=========================================================================================//
    /*..hui [23-2-8���� 4:25:29] �¼� ON/OFF..*/
    if(gu8_eeprom_rbuf[5] <= 1)
    {
        F_Hot_Enable = gu8_eeprom_rbuf[5];
    }
    else
    {
        F_Hot_Enable = SET;
        EepromByteWrite(EEPROM_ADDR1_HOT_ENABLE, (U8)F_Hot_Enable);
    }
    //=========================================================================================//
    /*..hui [21-5-28���� 4:38:50] ��ġ �÷��� ����..*/
    if(gu8_eeprom_rbuf[6] <= 1)
    {
        bit_install_flushing_state = gu8_eeprom_rbuf[6];
    }
    else
    {
        bit_install_flushing_state = SET;
        EepromByteWrite(EEPROM_ADDR1_INSTALL_FLUSHING, (U8)bit_install_flushing_state);
    }

    //=========================================================================================//
    /*..hui [23-6-1���� 7:06:03] ���� �÷��� ����..*/
    if(gu8_eeprom_rbuf[7] <= FILTER_FLUSHING_RO)
    {
        gu8_filter_flushing_state = gu8_eeprom_rbuf[7];
    }
    else
    {
        gu8_filter_flushing_state = FILTER_FLUSHING_NONE;
        EepromByteWrite(EEPROM_ADDR1_FILTER_FLUSHING, (U8)gu8_filter_flushing_state);
    }

    //=========================================================================================//
    /*..hui [23-6-29���� 2:37:57] ���� ���� �ð� ����..*/
    if(gu8_eeprom_rbuf[8] <= 1)
    {
        bit_first_time_setting = gu8_eeprom_rbuf[8];
    }
    else
    {
        bit_first_time_setting = CLEAR;
        EepromByteWrite(EEPROM_ADDR1_FIRST_TIME_SETTING, (U8)bit_first_time_setting);
    }

    //=========================================================================================//
    /*.. ����IC..*/
    if(gu8_eeprom_rbuf[9] <= SOUND_TYPE_MUTE )
    {
        gu8Sound_Type = gu8_eeprom_rbuf[9];
    }
    else
    {
        gu8Sound_Type = SOUND_TYPE_VOICE;
        EepromByteWrite(EEPROM_ADDR1_SOUND_TYPE, gu8Sound_Type);
    }

    //=========================================================================================//
    /*..hui [21-5-12���� 1:52:30] ���� ����..*/
    if(gu8_eeprom_rbuf[10] <= SOUND_VOLUME_LEVEL_5)
    {
        gu8VoiceVolumeLevel = gu8_eeprom_rbuf[10];
    }
    else
    {
        /*gu8VoiceVolumeLevel = SOUND_VOLUME_LEVEL_2;*/
        gu8VoiceVolumeLevel = SOUND_VOLUME_LEVEL_3;
        EepromByteWrite(EEPROM_ADDR1_VOICE_VOLUME, gu8VoiceVolumeLevel);
    }
    //=========================================================================================//
    if(gu8_eeprom_rbuf[11] <= ALTITUDE_4_MODE_VERY_LOW)
    {
        gu8AltitudeStep = gu8_eeprom_rbuf[11];
    }
    else
    {
        gu8AltitudeStep = ALTITUDE_1_MODE_HIGH;
        EepromByteWrite(EEPROM_ADDR1_ALTITUDE, gu8AltitudeStep);
    }
    //=========================================================================================//
    if(gu8_eeprom_rbuf[12] <= LANGUAGE_MAX)
    {
        gu8_voice_language_type = gu8_eeprom_rbuf[12];
    }
    else
    {
        gu8_voice_language_type = LANGUAGE_TYPE_ENGLISH;
        EepromByteWrite(EEPROM_ADDR1_LANGUAGE_TYPE, gu8AltitudeStep);
    }
    //=========================================================================================//
    if(gu8_eeprom_rbuf[13] <= 1)
    {
        gu8_ct_forced_flushing_start = gu8_eeprom_rbuf[13];
    }
    else
    {
        gu8_ct_forced_flushing_start = CLEAR;
        EepromByteWrite(EEPROM_ADDR1_CT_FORCED_FLUSHING, gu8_ct_forced_flushing_start);
    }
    //=========================================================================================//
    if(gu8_eeprom_rbuf[14] <= ALL_NEO_INO_RO_FILTER_CHANGE)
    {
        gu8CTFilterReplace = gu8_eeprom_rbuf[14];
    }
    else
    {
        gu8CTFilterReplace = 0;
        EepromByteWrite(EEPROM_ADDR1_CT_REPLACE_FILTER, gu8CTFilterReplace);
    }
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void load_ice_setting(void)
{
    EepromSeqRead(ICE_SETTING_START_ADDR, gu8_eeprom_rbuf, ICE_SETTING_LENGTH);

    //=========================================================================================//
    /*.. ���� ���?..*/
    if(gu8_eeprom_rbuf[0] <= 1)
    {
        F_Ice_Lock = gu8_eeprom_rbuf[0];
    }
    else
    {
        F_Ice_Lock = CLEAR;
        EepromByteWrite(EEPROM_ADDR2_ICE_LOCK, (U8)F_Ice_Lock);
    }

    //=========================================================================================//
    /*.. ���� ����..*/
    if(gu8_eeprom_rbuf[1] <= 1)
    {
        F_IceOn = gu8_eeprom_rbuf[1];
    }
    else
    {
        F_IceOn = SET;
        EepromByteWrite(EEPROM_ADDR2_ICE_ON_OFF, (U8)F_IceOn);
    }
    //=========================================================================================//
    /*..hui [18-2-21���� 8:05:32] �ü�/���� �켱���� ���� ���? �߰�..*/
    if(gu8_eeprom_rbuf[2] <= ICE_FIRST_ICE_MAKE)
    {
        bit_fast_ice_make = gu8_eeprom_rbuf[2];
    }
    else
    {
        bit_fast_ice_make = COLD_FIRST_ICE_MAKE;
        EepromByteWrite(EEPROM_ADDR2_ICE_MAKE_PRIORITY, (U8)bit_fast_ice_make);
    }
    //=========================================================================================//
    /*..hui [24-4-11���� 7:11:04] �����켱 �������? �����°� �߰�..*/
    if(gu8_eeprom_rbuf[3] <= RECOVER_FAST_ICE)
    {
        gu8_recover_org_fast_ice = gu8_eeprom_rbuf[3];
    }
    else
    {
        gu8_recover_org_fast_ice = NEW_FAST_ICE;
        EepromByteWrite(EEPROM_ADDR2_RECOVER_FAST_ICE, gu8_recover_org_fast_ice);
    }
    //=========================================================================================//
    /*..hui [24-7-26���� 3:09:08] ������ ��������..*/
    if(gu8_eeprom_rbuf[4] <= 14)
    {
        gu8_return_acid_step = gu8_eeprom_rbuf[4];
    }
    else
    {
        gu8_return_acid_step = 0;
        EepromByteWrite(EEPROM_ADDR2_ACID_CLEAN, gu8_return_acid_step);
    }

    /* ���������� ��/�� ũ�� ����. 250224 CH.PARK */
    if(gu8_eeprom_rbuf[5] <= 1)
    {
        bit_ice_size = gu8_eeprom_rbuf[5];
    }
    else
    {
        bit_ice_size = 0;
        EepromByteWrite(EEPROM_ADD2_ICE_SIZE, (U8)bit_ice_size);
    }

    /* ����Ÿ�� �Ϲ�/���� ����. 250317 CH.PARK */
    if(gu8_eeprom_rbuf[6] <= 1)
    {
        F_IceBreak = gu8_eeprom_rbuf[6];
    }
    else
    {
        F_IceBreak = CLEAR;
        EepromByteWrite(EEPROM_ADDR2_ICE_TYPE, (U8)F_IceBreak);
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void load_function_setting(void)
{
    EepromSeqRead(FUNCTION_SETTING_START_ADDR, gu8_eeprom_rbuf, FUNCTION_SETTING_LENGTH);

    //=========================================================================================//
    /*.. �߰� ���� ����..*/
    if(gu8_eeprom_rbuf[0] <= 1)
    {
        F_Save = gu8_eeprom_rbuf[0];
    }
    else
    {
        F_Save = SET;
        EepromByteWrite(EEPROM_ADDR3_OWER_SAVE_SETTING, (U8)F_Save);
    }

    //=========================================================================================//
    /*.. ��ȯ ���? ����..*/
    /*..hui [20-3-26���� 8:22:15] ��ȯ���? ���� ����..*/
    /*..hui [20-3-26���� 8:22:20] Ȥ�ó� ���߿� �ʿ��Ҽ�������..*/
    if(gu8_eeprom_rbuf[1] <= 1)
    {
        F_Circul_Drain = gu8_eeprom_rbuf[1];
    }
    else
    {
        F_Circul_Drain = SET;
        EepromByteWrite(EEPROM_ADDR3_CIRCULATION_DRAIN, (U8)F_Circul_Drain);
    }
    //=========================================================================================//
    /*..hui [19-9-24���� 10:00:06] UV �ڵ� ���? ����..*/
    if(gu8_eeprom_rbuf[2] <= 1)
    {
        F_Auto_UV_Control = gu8_eeprom_rbuf[2];
    }
    else
    {
        F_Auto_UV_Control = SET;
        EepromByteWrite(EEPROM_ADDR3_AUTO_UV_CONTROL, (U8)F_Auto_UV_Control);
    }
    //=========================================================================================//
    /*..hui [19-9-24���� 10:01:12] ����,������ LED ONOFF����..*/
    if(gu8_eeprom_rbuf[3] <= 1)
    {
        F_Welcome_Led_OnOff = gu8_eeprom_rbuf[3];
    }
    else
    {
        F_Welcome_Led_OnOff = SET;
        EepromByteWrite(EEPROM_ADDR3_EXTRACT_LED_ONOFF, (U8)F_Welcome_Led_OnOff);
    }
    //=========================================================================================//
    /*..hui [23-9-4���� 11:46:29] �ּҽð��� 1�ð�����..*/
    if( gu8_eeprom_rbuf[4] >= USE_SAVE_LEARNEND_MODE_MIN_TIME
    && gu8_eeprom_rbuf[4] <= USE_SAVE_LEARNEND_MODE_MAX_TIME )
    {
        gu8_wifi_smart_unused_setting_time = gu8_eeprom_rbuf[4];
    }
    else
    {
        gu8_wifi_smart_unused_setting_time = USE_SAVE_LEARNEND_MODE_TIME;
        EepromByteWrite(EEPROM_ADDR3_SMART_UNUSED_TIME, gu8_wifi_smart_unused_setting_time);
    }
    //=========================================================================================//
    //=========================================================================================//
    if(gu8_eeprom_rbuf[7] <= 1)
    {
        bit_periodic_ster_enable = gu8_eeprom_rbuf[7];
    }
    else
    {
        bit_periodic_ster_enable = SET;
        EepromByteWrite(EEPROM_ADDR3_STEAM_OFF, (U8)bit_periodic_ster_enable);
    }
    //=========================================================================================//
    if(gu8_eeprom_rbuf[8] <= 23)
    {
        gu8_wifi_period_ster_hour = gu8_eeprom_rbuf[8];
    }
    else
    {
        gu8_wifi_period_ster_hour = WIFI_PERIOD_STER_HOUR_DEFAULT;
        EepromByteWrite(EEPROM_ADDR3_PERIOD_STER_HOUR, gu8_wifi_period_ster_hour);
    }
    //=========================================================================================//
    if(gu8_eeprom_rbuf[9] <= 59)
    {
        gu8_wifi_period_ster_minute = gu8_eeprom_rbuf[9];
    }
    else
    {
        gu8_wifi_period_ster_minute = WIFI_PERIOD_STER_MINUTE_DEFAULT;
        EepromByteWrite(EEPROM_ADDR3_PERIOD_STER_MINUTE, gu8_wifi_period_ster_minute);
    }
    //=========================================================================================//
    if(gu8_eeprom_rbuf[10] <= 1)
    {
        bit_sleep_mode_enable = gu8_eeprom_rbuf[10];
    }
    else
    {
        /*..hui [23-6-29���� 2:24:16] ��ħ���? ����Ʈ�� OFF..*/
        bit_sleep_mode_enable = CLEAR;
        EepromByteWrite(EEPROM_ADDR3_SLEEP_MODE_ENABLE, (U8)bit_sleep_mode_enable);
    }
    //=========================================================================================//
    if(gu8_eeprom_rbuf[11] <= 23)
    {
        gu8_sleep_mode_start_hour = gu8_eeprom_rbuf[11];
    }
    else
    {
        gu8_sleep_mode_start_hour = WIFI_SLEEP_MODE_START_HOUR_DEFAULT;
        EepromByteWrite(EEPROM_ADDR3_SLEEP_START_HOUR, gu8_sleep_mode_start_hour);
    }
    //=========================================================================================//
    if(gu8_eeprom_rbuf[12] <= 59)
    {
        gu8_sleep_mode_start_minute = gu8_eeprom_rbuf[12];
    }
    else
    {
        gu8_sleep_mode_start_minute = WIFI_SLEEP_MODE_START_MINUTE_DEFAULT;
        EepromByteWrite(EEPROM_ADDR3_SLEEP_START_MINUTE, gu8_sleep_mode_start_minute);
    }
    //=========================================================================================//
    if(gu8_eeprom_rbuf[13] <= 23)
    {
        gu8_sleep_mode_finish_hour = gu8_eeprom_rbuf[13];
    }
    else
    {
        gu8_sleep_mode_finish_hour = WIFI_SLEEP_MODE_FINISH_HOUR_DEFAULT;
        EepromByteWrite(EEPROM_ADDR3_SLEEP_FINISH_HOUR, gu8_sleep_mode_finish_hour);
    }
    //=========================================================================================//
    if(gu8_eeprom_rbuf[14] <= 59)
    {
        gu8_sleep_mode_finish_minute = gu8_eeprom_rbuf[14];
    }
    else
    {
        gu8_sleep_mode_finish_minute = WIFI_SLEEP_MODE_FINISH_MINUTE_DEFAULT;
        EepromByteWrite(EEPROM_ADDR3_SLEEP_FINISH_MINUTE, gu8_wifi_period_ster_minute);
    }

    //=========================================================================================//
    /* �����? �������� ���� */
    if(gu8_eeprom_rbuf[15] <= 1)
    {
        bit_waitmode_enable = gu8_eeprom_rbuf[15];
    }
    else
    {
        bit_waitmode_enable = WIFI_WAIT_MODE_ENABLE_DEFAULT;
        EepromByteWrite(EEPROM_ADDR3_WAIT_MODE_ENABLE, (U8)bit_waitmode_enable);
    }
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void load_etc_setting(void)
{
    EepromSeqRead(ETC_SAVE_START_ADDR, gu8_eeprom_rbuf, ETC_SAVE_LENGTH);

    //=========================================================================================//
    /*.. �ŷ��� ����Ʈ ����..*/
    /*if(gu8_eeprom_rbuf[0] <= CUP_LEVEL__20_OZ__590_ML)*/
    /*if(gu8_eeprom_rbuf[0] <= CUP_LEVEL__32_OZ__945_ML)*/
    if(gu8_eeprom_rbuf[0] <= CUP_LEVEL__MAX)
    {
        gu8Cup_level_Default = gu8_eeprom_rbuf[0];
        gu8Cup_level = gu8Cup_level_Default;
    }
    else
    {
        gu8Cup_level_Default = CUP_LEVEL_1_120ML;
        gu8Cup_level = gu8Cup_level_Default;
        EepromByteWrite(EEPROM_ADDR4_CUP_LEVEL, gu8Cup_level_Default);
    }
    //=========================================================================================//
    /*..hui [18-1-10���� 11:06:40] ����/�ü� ���� ����..*/
    if(gu8_eeprom_rbuf[1] <= HOT_WATER_SELECT)
    {
        u8WaterOutState = gu8_eeprom_rbuf[1];
    }
    else
    {
        u8WaterOutState = PURE_WATER_SELECT;
        EepromByteWrite(EEPROM_ADDR4_WATER_SELECT_DEFAULT, u8WaterOutState);
    }

    //=========================================================================================//
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void load_memento(void)
{
    EepromSeqRead(MEMENTO_START_ADDR, gu8_eeprom_rbuf, MEMENTO_LENGTH);

    //=========================================================================================//
    /*.. �޸��� 01..*/
    if(gu8_eeprom_rbuf[0] <= MAX_ERROR)
    {
        u8Memento_Buff[0] = gu8_eeprom_rbuf[0];
    }
    else
    {
        u8Memento_Buff[0] = 0;
        EepromByteWrite(EEPROM_ADDR5_MEMENTO_NUM0, u8Memento_Buff[0]);
    }
//=========================================================================================//
    /*.. �޸��� 02..*/
    if(gu8_eeprom_rbuf[1] <= MAX_ERROR)
    {
        u8Memento_Buff[1] = gu8_eeprom_rbuf[1];
    }
    else
    {
        u8Memento_Buff[1] = 0;
        EepromByteWrite(EEPROM_ADDR5_MEMENTO_NUM1, u8Memento_Buff[1]);
    }
//=========================================================================================//
    /*.. �޸��� 03..*/
    if(gu8_eeprom_rbuf[2] <= MAX_ERROR)
    {
        u8Memento_Buff[2] = gu8_eeprom_rbuf[2];
    }
    else
    {
        u8Memento_Buff[2] = 0;
        EepromByteWrite(EEPROM_ADDR5_MEMENTO_NUM2, u8Memento_Buff[2]);
    }

//=========================================================================================//
    /*.. �޸��� 04..*/
    if(gu8_eeprom_rbuf[3] <= MAX_ERROR)
    {
        u8Memento_Buff[3] = gu8_eeprom_rbuf[3];
    }
    else
    {
        u8Memento_Buff[3] = 0;
        EepromByteWrite(EEPROM_ADDR5_MEMENTO_NUM3, u8Memento_Buff[3]);
    }

//=========================================================================================//
    /*.. �޸��� 05..*/
    if(gu8_eeprom_rbuf[4] <= MAX_ERROR)
    {
        u8Memento_Buff[4] = gu8_eeprom_rbuf[4];
    }
    else
    {
        u8Memento_Buff[4] = 0;
        EepromByteWrite(EEPROM_ADDR5_MEMENTO_NUM4, u8Memento_Buff[4]);
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void load_iot_function(void)
{
    U16 u16_half_amount;
	U16 u16_one_amount;
	U16 u16_two_amount;
    U16 u16_four_amount;

    EepromSeqRead(IOT_FUNCTION_ADDR, gu8_eeprom_rbuf, IOT_FUNCTION_LENGTH);

    u16_half_amount = (U16)((gu8_eeprom_rbuf[0] * 256) + gu8_eeprom_rbuf[1]);

    if(u16_half_amount < 120 || u16_half_amount > 1000 || ((u16_half_amount % 10) > 0))
    {
        gu16_WaterQuantity_half = DEFAULT_HALF_CUP_AMOUNT;
        gu8_eeprom_wbuf[0] = 0;
        gu8_eeprom_wbuf[1] = 0;
        EepromPageWrite(EEPROM_ADDR6_HALF_CUP_AMOUNT, gu8_eeprom_wbuf, 2);
    }
    else
    {
        gu16_WaterQuantity_half = u16_half_amount;
    }

    u16_one_amount = (U16)((gu8_eeprom_rbuf[2] * 256) + gu8_eeprom_rbuf[3]);

    if(u16_one_amount < 120 || u16_one_amount > 1000 || ((u16_one_amount % 10) > 0))
    {
        gu16_WaterQuantity_one = DEFAULT_ONE_CUP_AMOUNT;
        gu8_eeprom_wbuf[2] = 0;
        gu8_eeprom_wbuf[3] = 0;
        EepromPageWrite(EEPROM_ADDR6_ONE_CUP_AMOUNT, gu8_eeprom_wbuf, 2);
    }
    else
    {
        gu16_WaterQuantity_one = u16_one_amount;
    }

    u16_two_amount = (U16)((gu8_eeprom_rbuf[4]* 256) + gu8_eeprom_rbuf[5]);

    if(u16_two_amount < 120 || u16_two_amount > 1000 || ((u16_two_amount % 10) > 0))
    {
        gu16_WaterQuantity_two = DEFAULT_TWO_CUP_AMOUNT;
        gu8_eeprom_wbuf[4] = 0;
        gu8_eeprom_wbuf[5] = 0;
        EepromPageWrite(EEPROM_ADDR6_TWO_CUP_AMOUNT, gu8_eeprom_wbuf, 2);
    }
    else
    {
        gu16_WaterQuantity_two = u16_two_amount;
    }

    u16_four_amount = (U16)((gu8_eeprom_rbuf[6]* 256) + gu8_eeprom_rbuf[7]);

    if(u16_four_amount < 120 || u16_four_amount > 1000 || ((u16_four_amount % 10) > 0))
    {
        gu16_WaterQuantity_four = DEFAULT_FOUR_CUP_AMOUNT;
        gu8_eeprom_wbuf[6] = 0;
        gu8_eeprom_wbuf[7] = 0;
        EepromPageWrite(EEPROM_ADDR6_FOUR_CUP_AMOUNT, gu8_eeprom_wbuf, 2);
    }
    else
    {
        gu16_WaterQuantity_four = u16_four_amount;
    }
    //=========================================================================================//
    if(gu8_eeprom_rbuf[8] <= 1)
    {
        gu8_fota_start = gu8_eeprom_rbuf[8];
    }
    else
    {
        gu8_fota_start = CLEAR;
        EepromByteWrite(EEPROM_ADDR6_FOTA_START, gu8_fota_start);
    }

    if(gu8_eeprom_rbuf[9] <= 10)
    {
        gu8_drain_tank_ster_count = gu8_eeprom_rbuf[9];
    }
    else
    {
        gu8_drain_tank_ster_count = 0;
        EepromByteWrite(EEPROM_ADDR6_DRAIN_TANK_STER_COUNT, gu8_drain_tank_ster_count);
    }

}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void load_wifi(void)
{
    EepromSeqRead(WIFI_DATA_ADDR, gu8_eeprom_rbuf, WIFI_DATA_LENGTH);

    //=========================================================================================//
    if(gu8_eeprom_rbuf[0] <= 1)
    {
        F_Wifi_Power = gu8_eeprom_rbuf[0];
    }
    else
    {
        F_Wifi_Power = CLEAR;
        EepromByteWrite(EEPROM_ADDR_WIFI_POWER, (U8)F_Wifi_Power);
    }
    //=========================================================================================//
    if(gu8_eeprom_rbuf[1] <= 1)
    {
        F_Wifi_First_Paring = gu8_eeprom_rbuf[1];
    }
    else
    {
        F_Wifi_First_Paring = CLEAR;
        EepromByteWrite(EEPROM_ADDR_FIRST_PARING, (U8)F_Wifi_First_Paring);
    }
    //=========================================================================================//
    if(gu8_eeprom_rbuf[2] <= 1)
    {
        F_Wifi_Fota_MODULE = gu8_eeprom_rbuf[2];
    }
    else
    {
        F_Wifi_Fota_MODULE = CLEAR;
        EepromByteWrite(EEPROM_ADDR_FOTA_MODULE, (U8)F_Wifi_Fota_MODULE);
    }
    //=========================================================================================//
    if(gu8_eeprom_rbuf[3] <= 1)
    {
        F_Wifi_Fota_MCU = gu8_eeprom_rbuf[3];
    }
    else
    {
        F_Wifi_Fota_MCU = CLEAR;
        EepromByteWrite(EEPROM_ADDR_FOTA_MCU, (U8)F_Wifi_Fota_MCU);
    }

    //=========================================================================================//
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void load_my_setting(void)
{
    //LSH zzang
    U16 u16_my1_amount;
    U16 u16_my2_amount;
    U16 u16_my3_amount;

    EepromSeqRead(MY_DATA_ADDR, gu8_eeprom_rbuf, MY_DATA_LENGTH);

    /* MY WATER 1 */
    //=========================================================================================//
    if(gu8_eeprom_rbuf[0] == 6
    || gu8_eeprom_rbuf[0] == 27
    || gu8_eeprom_rbuf[0] == 45
    || gu8_eeprom_rbuf[0] == 50
    || gu8_eeprom_rbuf[0] == 55
    || gu8_eeprom_rbuf[0] == 60
    || gu8_eeprom_rbuf[0] == 65
    || gu8_eeprom_rbuf[0] == 70
    || gu8_eeprom_rbuf[0] == 75
    || gu8_eeprom_rbuf[0] == 80
    || gu8_eeprom_rbuf[0] == 85
    || gu8_eeprom_rbuf[0] == 90
    || gu8_eeprom_rbuf[0] == 95
    || gu8_eeprom_rbuf[0] == 100)
    {
        my_setting[MY_INDEX_MY1].temp = gu8_eeprom_rbuf[0];
    }
    else
    {
        my_setting[MY_INDEX_MY1].temp = 0;
        EepromByteWrite(EEPROM_ADDR_MY1_TEMP, (U8)my_setting[MY_INDEX_MY1].temp);
    }
    //=========================================================================================//
    u16_my1_amount = (U16)((gu8_eeprom_rbuf[1] * 256) + gu8_eeprom_rbuf[2]);
    if(u16_my1_amount < 90 || u16_my1_amount > 1000)
    {
        my_setting[MY_INDEX_MY1].amount = 0;
        gu8_eeprom_wbuf[0] = 0;
        gu8_eeprom_wbuf[1] = 0;
        EepromPageWrite(EEPROM_ADDR_MY1_AMOUNT, gu8_eeprom_wbuf, 2);
    }
    else
    {
        my_setting[MY_INDEX_MY1].amount = u16_my1_amount;
    }
    //=========================================================================================//
    if(gu8_eeprom_rbuf[3] <= 1)
    {
        my_setting[MY_INDEX_MY1].use = gu8_eeprom_rbuf[3];
    }
    else
    {
        my_setting[MY_INDEX_MY1].use = 0;
        EepromByteWrite(EEPROM_ADDR_MY1_USE, (U8)my_setting[MY_INDEX_MY1].use);
    }

    /* MY WATER 2 */
    //=========================================================================================//
    if(gu8_eeprom_rbuf[4] == 6
    || gu8_eeprom_rbuf[4] == 27
    || gu8_eeprom_rbuf[4] == 45
    || gu8_eeprom_rbuf[4] == 50
    || gu8_eeprom_rbuf[4] == 55
    || gu8_eeprom_rbuf[4] == 60
    || gu8_eeprom_rbuf[4] == 65
    || gu8_eeprom_rbuf[4] == 70
    || gu8_eeprom_rbuf[4] == 75
    || gu8_eeprom_rbuf[4] == 80
    || gu8_eeprom_rbuf[4] == 85
    || gu8_eeprom_rbuf[4] == 90
    || gu8_eeprom_rbuf[4] == 95
    || gu8_eeprom_rbuf[4] == 100)
    {
        my_setting[MY_INDEX_MY2].temp = gu8_eeprom_rbuf[4];
    }
    else
    {
        my_setting[MY_INDEX_MY2].temp = 0;
        EepromByteWrite(EEPROM_ADDR_MY2_TEMP, (U8)my_setting[MY_INDEX_MY2].temp);
    }
    //=========================================================================================//
    u16_my2_amount = (U16)((gu8_eeprom_rbuf[5] * 256) + gu8_eeprom_rbuf[6]);
    if(u16_my2_amount < 90 || u16_my2_amount > 1000)
    {
        my_setting[MY_INDEX_MY2].amount = 0;
        gu8_eeprom_wbuf[0] = 0;
        gu8_eeprom_wbuf[1] = 0;
        EepromPageWrite(EEPROM_ADDR_MY2_AMOUNT, gu8_eeprom_wbuf, 2);
    }
		else
		{
			my_setting[MY_INDEX_MY2].amount = u16_my2_amount;
		}
    //=========================================================================================//
    if(gu8_eeprom_rbuf[7] <= 1)
    {
        my_setting[MY_INDEX_MY2].use = gu8_eeprom_rbuf[7];
    }
    else
    {
        my_setting[MY_INDEX_MY2].use = 0;
        EepromByteWrite(EEPROM_ADDR_MY2_USE, (U8)my_setting[MY_INDEX_MY2].use);
    }

    /* MY WATER 3 */
    //=========================================================================================//
    //=========================================================================================//
    if(gu8_eeprom_rbuf[8] == 6
    || gu8_eeprom_rbuf[8] == 27
    || gu8_eeprom_rbuf[8] == 45
    || gu8_eeprom_rbuf[8] == 50
    || gu8_eeprom_rbuf[8] == 55
    || gu8_eeprom_rbuf[8] == 60
    || gu8_eeprom_rbuf[8] == 65
    || gu8_eeprom_rbuf[8] == 70
    || gu8_eeprom_rbuf[8] == 75
    || gu8_eeprom_rbuf[8] == 80
    || gu8_eeprom_rbuf[8] == 85
    || gu8_eeprom_rbuf[8] == 90
    || gu8_eeprom_rbuf[8] == 95
    || gu8_eeprom_rbuf[8] == 100)
    {
        my_setting[MY_INDEX_MY3].temp = gu8_eeprom_rbuf[8];
    }
    else
    {
        my_setting[MY_INDEX_MY3].temp = 0;
        EepromByteWrite(EEPROM_ADDR_MY3_TEMP, (U8)my_setting[MY_INDEX_MY3].temp);
    }
    //=========================================================================================//
    u16_my3_amount = (U16)((gu8_eeprom_rbuf[9] * 256) + gu8_eeprom_rbuf[10]);
    if(u16_my3_amount < 90 || u16_my3_amount > 1000)                // �뷮
    {
        my_setting[MY_INDEX_MY3].amount = 0;
        gu8_eeprom_wbuf[0] = 0;
        gu8_eeprom_wbuf[1] = 0;
        EepromPageWrite(EEPROM_ADDR_MY3_AMOUNT, gu8_eeprom_wbuf, 2);
    }
		else
		{
			my_setting[MY_INDEX_MY3].amount = u16_my3_amount;
		}
    //=========================================================================================//
    if(gu8_eeprom_rbuf[11] <= 1)
    {
        my_setting[MY_INDEX_MY3].use = gu8_eeprom_rbuf[11];
    }
    else
    {
        my_setting[MY_INDEX_MY3].use = 0;
        EepromByteWrite(EEPROM_ADDR_MY3_USE, (U8)my_setting[MY_INDEX_MY3].use);
    }

    //=========================================================================================//
    if(gu8_eeprom_rbuf[12] <= 5)
    {
        my_receipe_default = gu8_eeprom_rbuf[12];
    }
    else
    {
        my_receipe_default = MY_INDEX_RAMEN;
        EepromByteWrite(EEPROM_ADDR_MY_DEFAULT, (U8)my_receipe_default);
    }

}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void load_hot_temp_setting(void)
{
    U16 u16_hot_temp;
    U16 temp1;
    U16 temp2;
    U16 temp3;
    U16 temp4;
    U8 i = 0;
    U8 j = 12;

    EepromSeqRead(HOT_TEMP_ADDR, gu8_eeprom_rbuf, HOT_TEMP_LENGTH);

    //=========================================================================================//
    if(gu8_eeprom_rbuf[0] >= HOT_SET_TEMP____45oC && gu8_eeprom_rbuf[0] <= HOT_SET_TEMP____100oC)
    {
        selected_hot_temp[0] = gu8_eeprom_rbuf[0];
    }
    else
    {
        selected_hot_temp[0] = 0;
        EepromByteWrite(EEPROM_ADDR_HOT_TEMP_0, (U8)selected_hot_temp[0]);
    }
    //=========================================================================================//

    if(gu8_eeprom_rbuf[1] >= HOT_SET_TEMP____45oC && gu8_eeprom_rbuf[1] <= HOT_SET_TEMP____100oC)
    {
        selected_hot_temp[1] = gu8_eeprom_rbuf[1];
    }
    else
    {
        selected_hot_temp[1] = 0;
        EepromByteWrite(EEPROM_ADDR_HOT_TEMP_1, (U8)selected_hot_temp[1]);
    }
    //=========================================================================================//

		if(gu8_eeprom_rbuf[2] >= HOT_SET_TEMP____45oC && gu8_eeprom_rbuf[2] <= HOT_SET_TEMP____100oC)
    {
        selected_hot_temp[2] = gu8_eeprom_rbuf[2];
    }
    else
    {
        selected_hot_temp[2] = 0;
        EepromByteWrite(EEPROM_ADDR_HOT_TEMP_2, (U8)selected_hot_temp[2]);
    }
    //=========================================================================================//

		if(gu8_eeprom_rbuf[3] >= HOT_SET_TEMP____45oC && gu8_eeprom_rbuf[3] <= HOT_SET_TEMP____100oC)
    {
        selected_hot_temp[3] = gu8_eeprom_rbuf[3];
    }
    else
    {
        selected_hot_temp[3] = 0;
        EepromByteWrite(EEPROM_ADDR_HOT_TEMP_3, (U8)selected_hot_temp[3]);
    }
    //=========================================================================================//

    temp1 = 1<<(selected_hot_temp[0]);
    temp2 = 1<<(selected_hot_temp[1]);
    temp3 = 1<<(selected_hot_temp[2]);
    temp4 = 1<<(selected_hot_temp[3]);

    U16HotTemplSelect = (U16)(temp1|temp2|temp3|temp4);
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void load_water_usage(void)
{
    U32 mu32_a = 0;
    U32 mu32_b = 0;

    EepromSeqRead(WATER_USAGE_ADDR, gu8_eeprom_rbuf, WATER_USAGE_LENGTH);

    //=========================================================================================//
    gu16_water_usage_neo_filter = (U16)((gu8_eeprom_rbuf[0] * 256) + gu8_eeprom_rbuf[1]);

    if( gu16_water_usage_neo_filter > NEO_TOTAL_USAGE_MAX_WATER )
    {
        gu16_water_usage_neo_filter = 0;
        gu8_eeprom_wbuf[0] = 0;
        gu8_eeprom_wbuf[1] = 0;
        EepromPageWrite(EEPROM_ADDR7_WATER_USAGE__NEO, gu8_eeprom_wbuf, 2);
    }
    else{}

    //=========================================================================================//
    gu16_water_usage_ro_filter = (U16)((gu8_eeprom_rbuf[2] * 256) + gu8_eeprom_rbuf[3]);

    if( gu16_water_usage_ro_filter > RO_TOTAL_USAGE_MAX_WATER )
    {
        gu16_water_usage_ro_filter = 0;
        gu8_eeprom_wbuf[0] = 0;
        gu8_eeprom_wbuf[1] = 0;
        EepromPageWrite(EEPROM_ADDR7_WATER_USAGE__RO, gu8_eeprom_wbuf, 2);
    }
    else{}

    //=========================================================================================//
    gu16_water_usage_ino_filter = (U16)((gu8_eeprom_rbuf[4] * 256) + gu8_eeprom_rbuf[5]);

    if( gu16_water_usage_ino_filter > INO_TOTAL_USAGE_MAX_WATER )
    {
        gu16_water_usage_ino_filter = 0;
        gu8_eeprom_wbuf[0] = 0;
        gu8_eeprom_wbuf[1] = 0;
        EepromPageWrite(EEPROM_ADDR7_WATER_USAGE__INO, gu8_eeprom_wbuf, 2);
    }
    else{}

    //=========================================================================================//
    gu16_reset_hour_neo_filter = (U16)((gu8_eeprom_rbuf[6] * 256) + gu8_eeprom_rbuf[7]);

    /*if( gu16_reset_hour_neo_filter > FILTER_RESET_DEFAULT_12_MONTH_456_DAY_HOUR__NEO )*/
    if( gu16_reset_hour_neo_filter > FILTER_RESET_MAX_HOUR__NEO )
    {
        gu16_reset_hour_neo_filter = 0;
        gu8_eeprom_wbuf[0] = 0;
        gu8_eeprom_wbuf[1] = 0;
        EepromPageWrite(EEPROM_ADDR7_FILTER_RESET_DAY__NEO, gu8_eeprom_wbuf, 2);
    }
    else{}

    //=========================================================================================//
    gu16_reset_hour_ro_filter = (U16)((gu8_eeprom_rbuf[8] * 256) + gu8_eeprom_rbuf[9]);

    /*if( gu16_reset_hour_ro_filter > FILTER_RESET_DEFAULT_24_MONTH_912_DAY_HOUR__RO )*/
    if( gu16_reset_hour_ro_filter > FILTER_RESET_MAX_HOUR__RO )
    {
        gu16_reset_hour_ro_filter = 0;
        gu8_eeprom_wbuf[0] = 0;
        gu8_eeprom_wbuf[1] = 0;
        EepromPageWrite(EEPROM_ADDR7_FILTER_RESET_DAY__RO, gu8_eeprom_wbuf, 2);
    }
    else{}

    //=========================================================================================//
    gu16_reset_hour_ino_filter = (U16)((gu8_eeprom_rbuf[10] * 256) + gu8_eeprom_rbuf[11]);

    /*if( gu16_reset_hour_ino_filter > FILTER_RESET_DEFAULT_12_MONTH_456_DAY_HOUR__INO )*/
    if( gu16_reset_hour_ino_filter > FILTER_RESET_MAX_HOUR__INO )
    {
        gu16_reset_hour_ino_filter = 0;
        gu8_eeprom_wbuf[0] = 0;
        gu8_eeprom_wbuf[1] = 0;
        EepromPageWrite(EEPROM_ADDR7_FILTER_RESET_DAY__INO, gu8_eeprom_wbuf, 2);
    }
    else{}

    //=========================================================================================//
    if( gu8_eeprom_rbuf[12] <= FILTER_CHANGE_TYPE__NEO_RO_INO_ALL )
    {
        gu8_filter_change_type = gu8_eeprom_rbuf[12];
    }
    else
    {
        gu8_filter_change_type = FILTER_CHANGE_TYPE__NEO_INO;
        EepromByteWrite(EEPROM_ADDR7_FILTER_CHANGE_TYPE, (U8)gu8_filter_change_type);
    }

    //=========================================================================================//
    //=========================================================================================//
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void load_debug_data(void)
{
    EepromSeqRead(DEBUG_DATA_ADDR, gu8_eeprom_rbuf, DEBUG_DATA_LENGTH);

    //=========================================================================================//
    if(gu8_eeprom_rbuf[0] <= 99)
    {
        gu8_ice_ster_drain_stop_count = gu8_eeprom_rbuf[0];
    }
    else
    {
        gu8_ice_ster_drain_stop_count = 0;
        EepromByteWrite(EEPROM_ADDR8_STER_DRAIN_STOP, gu8_ice_ster_drain_stop_count);
    }
    //=========================================================================================//
    if(gu8_eeprom_rbuf[1] <= 99)
    {
        gu8_ice_ster_low_water_stop_count = gu8_eeprom_rbuf[1];
    }
    else
    {
        gu8_ice_ster_low_water_stop_count = 0;
        EepromByteWrite(EEPROM_ADDR8_STER_LOW_WATER_STOP, gu8_ice_ster_low_water_stop_count);
    }

    //=========================================================================================//
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void eeprom_initial(void)
{
    //-----------------------
    /*..                �ʱ�ȭ      ..*/
    //-----------------------

    // Check A
    EepromByteWrite(EEPROM_ADDR_CHECK_START,EEPROM_CHECK_A_VALUE);

    initial_spetial_setting();
    initial_ice_setting();
    initial_function_setting();
    initial_etc_setting();
    initial_memento();
    initial_iot_function();
    initial_wifi();
    initial_my_setting();//LSH zzang
    initial_hot_temp_setting();
    initial_water_usage();

    initial_debug_data();

    EepromByteWrite(EEPROM_ADDR_CHECK_END,EEPROM_CHECK_B_VALUE);
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void initial_spetial_setting(void)
{
    F_Cold_Enable = SET;    /*.. �ü� ���? ����..*/
    F_Hot_Lock = CLEAR;         /*.. �¼� ���? ����..*/
    F_All_Lock = CLEAR;    /*.. ��ü ���? ����..*/
    /*..hui [24-1-5���� 9:15:43] ���Ұ� ���? �̻��?..*/
    F_Mute_Enable = CLEAR;  /*..hui [18-1-9���� 7:16:17] ���Ұ� ���?..*/
    /*..hui [20-1-6���� 7:56:30] �ü� �µ� ����..*/
    /*..hui [24-1-5���� 9:15:49] �ü��µ� ���� �̻��?..*/
    gu8_cold_setting_level = COLD_TEMP_HIGH;
    F_Hot_Enable = SET;

    /*..hui [21-5-28���� 4:41:00] ��ġ �÷��� ����..*/
    bit_install_flushing_state = SET;
    gu8_filter_flushing_state = FILTER_FLUSHING_NONE;

    bit_first_time_setting = CLEAR;

    gu8Sound_Type = SOUND_TYPE_VOICE;
    /*gu8VoiceVolumeLevel = SOUND_VOLUME_LEVEL_2;*/
    gu8VoiceVolumeLevel = SOUND_VOLUME_LEVEL_3;

    gu8AltitudeStep = ALTITUDE_1_MODE_HIGH;

    gu8_voice_language_type = LANGUAGE_TYPE_ENGLISH;
    gu8_ct_forced_flushing_start = CLEAR;
    gu8CTFilterReplace = 0;

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


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void initial_ice_setting(void)
{
    F_Ice_Lock = CLEAR;          /*.. ���� ���?..*/
    F_IceOn = SET;               /*.. ���� ����..*/
    bit_fast_ice_make = COLD_FIRST_ICE_MAKE;
    gu8_recover_org_fast_ice = NEW_FAST_ICE;
    gu8_return_acid_step = 0;
    bit_ice_size = 0;
    F_IceBreak = CLEAR;

    gu8_eeprom_wbuf[0] = (U8)F_Ice_Lock;
    gu8_eeprom_wbuf[1] = (U8)F_IceOn;
    gu8_eeprom_wbuf[2] = (U8)bit_fast_ice_make;
    gu8_eeprom_wbuf[3] = gu8_recover_org_fast_ice;
    gu8_eeprom_wbuf[4] = gu8_return_acid_step;
    gu8_eeprom_wbuf[5] = bit_ice_size;
    gu8_eeprom_wbuf[6] = F_IceBreak;

    EepromPageWrite(ICE_SETTING_START_ADDR, gu8_eeprom_wbuf, ICE_SETTING_LENGTH);
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void initial_function_setting(void)
{
    F_Save = SET;                /*..�߰� ���� ����..*/
    F_Circul_Drain = SET;        /*..��ȯ���? ����..*/
    F_Auto_UV_Control = SET;     /*..UV �ڵ����?..*/
    F_Welcome_Led_OnOff = SET;   /*..���� LED ONOFF..*/

    /*bit_my_cup_enable = CLEAR;*/
    /*gu16_my_cup_level = 100;*/
    gu8_wifi_smart_unused_setting_time = USE_SAVE_LEARNEND_MODE_TIME;

    bit_periodic_ster_enable = SET;
    gu8_wifi_period_ster_hour = WIFI_PERIOD_STER_HOUR_DEFAULT;
    gu8_wifi_period_ster_minute = WIFI_PERIOD_STER_MINUTE_DEFAULT;

    /*bit_sleep_mode_enable = SET;*/
    /*..hui [23-6-29���� 2:24:16] ��ħ���? ����Ʈ�� OFF..*/
    bit_sleep_mode_enable = CLEAR;
    gu8_sleep_mode_start_hour = WIFI_SLEEP_MODE_START_HOUR_DEFAULT;
    gu8_sleep_mode_start_minute = WIFI_SLEEP_MODE_START_MINUTE_DEFAULT;
    gu8_sleep_mode_finish_hour = WIFI_SLEEP_MODE_FINISH_HOUR_DEFAULT;
    gu8_sleep_mode_finish_minute = WIFI_SLEEP_MODE_FINISH_MINUTE_DEFAULT;

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

    EepromPageWrite(FUNCTION_SETTING_START_ADDR, gu8_eeprom_wbuf, FUNCTION_SETTING_LENGTH);
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void initial_etc_setting(void)
{
    /*.. cup level default ����..*/
    gu8Cup_level_Default = CUP_LEVEL_1_120ML;
    gu8Cup_level = gu8Cup_level_Default;

    /*..hui [18-1-10���� 11:06:40] ����/�ü� ���� ����..*/
    u8WaterOutState = PURE_WATER_SELECT;

    gu8_eeprom_wbuf[0] = gu8Cup_level;
    gu8_eeprom_wbuf[1] = u8WaterOutState;

    EepromPageWrite(ETC_SAVE_START_ADDR, gu8_eeprom_wbuf, ETC_SAVE_LENGTH);
}



/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void initial_memento(void)
{
    /*.. �޸���01..*/
    u8Memento_Buff[0] = 0;

    /*.. �޸���02..*/
    u8Memento_Buff[1] = 0;

    /*.. �޸���03..*/
    u8Memento_Buff[2] = 0;

    /*.. �޸���04..*/
    u8Memento_Buff[3] = 0;

     /*.. �޸���05..*/
    u8Memento_Buff[4] = 0;

    gu8_eeprom_wbuf[0] = u8Memento_Buff[0];
    gu8_eeprom_wbuf[1] = u8Memento_Buff[1];
    gu8_eeprom_wbuf[2] = u8Memento_Buff[2];
    gu8_eeprom_wbuf[3] = u8Memento_Buff[3];
    gu8_eeprom_wbuf[4] = u8Memento_Buff[4];

    EepromPageWrite(MEMENTO_START_ADDR, gu8_eeprom_wbuf, MEMENTO_LENGTH);
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void initial_iot_function(void)
{
    U16HotTemplSelect = HOT_TEMP_SELECT_DEFAULT_45_70_85_100;

    gu16_WaterQuantity_half = DEFAULT_HALF_CUP_AMOUNT;
    gu16_WaterQuantity_one = DEFAULT_ONE_CUP_AMOUNT;
    gu16_WaterQuantity_two = DEFAULT_TWO_CUP_AMOUNT;
    gu16_WaterQuantity_four = DEFAULT_FOUR_CUP_AMOUNT;

    gu8_fota_start = CLEAR;
    gu8_drain_tank_ster_count = 0;

    gu8_eeprom_wbuf[0] = (U8)(gu16_WaterQuantity_half / (U16)256);
    gu8_eeprom_wbuf[1] = (U8)(gu16_WaterQuantity_half % (U16)256);
    gu8_eeprom_wbuf[2] = (U8)(gu16_WaterQuantity_one / (U16)256);
    gu8_eeprom_wbuf[3] = (U8)(gu16_WaterQuantity_one / (U16)256);
    gu8_eeprom_wbuf[4] = (U8)(gu16_WaterQuantity_two / (U16)256);
    gu8_eeprom_wbuf[5] = (U8)(gu16_WaterQuantity_two % (U16)256);
    gu8_eeprom_wbuf[6] = (U8)(gu16_WaterQuantity_four / (U16)256);
    gu8_eeprom_wbuf[7] = (U8)(gu16_WaterQuantity_four % (U16)256);
    gu8_eeprom_wbuf[8] = gu8_fota_start;
    gu8_eeprom_wbuf[9] = gu8_drain_tank_ster_count;

    gu8_hot_default_temp = HOT_SET_TEMP____100oC;

    EepromPageWrite(IOT_FUNCTION_ADDR, gu8_eeprom_wbuf, IOT_FUNCTION_LENGTH);
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void initial_wifi(void)
{
    F_Wifi_Power = CLEAR;
    F_Wifi_First_Paring = CLEAR;
    F_Wifi_Fota_MODULE = CLEAR;
    F_Wifi_Fota_MCU = CLEAR;

    gu8_eeprom_wbuf[0] = CLEAR;
    gu8_eeprom_wbuf[1] = CLEAR;
    gu8_eeprom_wbuf[2] = CLEAR;
    gu8_eeprom_wbuf[3] = CLEAR;

    EepromPageWrite(WIFI_DATA_ADDR, gu8_eeprom_wbuf, WIFI_DATA_LENGTH);
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void initial_my_setting(void)
{
    //LSH zzang
    my_setting[MY_INDEX_MY1].temp = 0;
    my_setting[MY_INDEX_MY1].amount = 0;
    my_setting[MY_INDEX_MY1].use = 0;

    my_setting[MY_INDEX_MY2].temp = 0;
    my_setting[MY_INDEX_MY2].amount = 0;
    my_setting[MY_INDEX_MY2].use = 0;

    my_setting[MY_INDEX_MY3].temp = 0;
    my_setting[MY_INDEX_MY3].amount = 0;
    my_setting[MY_INDEX_MY3].use = 0;

    my_receipe_default = MY_INDEX_RAMEN;

    gu8_eeprom_wbuf[0] = CLEAR;
    gu8_eeprom_wbuf[1] = CLEAR;
    gu8_eeprom_wbuf[2] = CLEAR;
    gu8_eeprom_wbuf[3] = CLEAR;
    gu8_eeprom_wbuf[4] = CLEAR;
    gu8_eeprom_wbuf[5] = CLEAR;
    gu8_eeprom_wbuf[6] = CLEAR;
    gu8_eeprom_wbuf[7] = CLEAR;
    gu8_eeprom_wbuf[8] = CLEAR;
    gu8_eeprom_wbuf[9] = CLEAR;
    gu8_eeprom_wbuf[10] = CLEAR;
    gu8_eeprom_wbuf[11] = CLEAR;
    gu8_eeprom_wbuf[12] = CLEAR;

    EepromPageWrite(MY_DATA_ADDR, gu8_eeprom_wbuf, MY_DATA_LENGTH);
}

void initial_hot_temp_setting(void)
{
    selected_hot_temp[0] = HOT_SET_TEMP____100oC;
    selected_hot_temp[1] = HOT_SET_TEMP____85oC;
    selected_hot_temp[2] = HOT_SET_TEMP____70oC;
    selected_hot_temp[3] = HOT_SET_TEMP____45oC;

    gu8_eeprom_wbuf[0] = HOT_SET_TEMP____100oC;
    gu8_eeprom_wbuf[1] = HOT_SET_TEMP____85oC;
    gu8_eeprom_wbuf[2] = HOT_SET_TEMP____70oC;
    gu8_eeprom_wbuf[3] = HOT_SET_TEMP____45oC;

    U16HotTemplSelect = (U16)HOT_TEMP_SELECT_DEFAULT_45_70_85_100;
    EepromPageWrite(HOT_TEMP_ADDR, gu8_eeprom_wbuf, HOT_TEMP_LENGTH);
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void initial_water_usage(void)
{

    gu16_water_usage_neo_filter = 0;
    gu16_water_usage_ro_filter = 0;
    gu16_water_usage_ino_filter = 0;

    gu16_reset_hour_neo_filter = 0;
    gu16_reset_hour_ro_filter = 0;
    gu16_reset_hour_ino_filter = 0;

    gu16_temporary_save_usage = 0;

    gu8_filter_change_type = FILTER_CHANGE_TYPE__NEO_INO;

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

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void initial_debug_data(void)
{
    gu8_ice_ster_drain_stop_count = 0;
    gu8_ice_ster_low_water_stop_count = 0;

    gu8_eeprom_wbuf[0] = gu8_ice_ster_drain_stop_count;
    gu8_eeprom_wbuf[1] = gu8_ice_ster_low_water_stop_count;

    EepromPageWrite(DEBUG_DATA_ADDR, gu8_eeprom_wbuf, DEBUG_DATA_LENGTH);
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void eeprom_test(void)
{
    U8 mu8EEPROM_ReadDataBuffer = 0;

    if(u811111 == SET)
    {
        return;
    }
    else
    {
        u811111 = SET;
    }

    EepromSeqRead(EEPROM_ADDR_TEST, gu8_eeprom_rbuf, 1);

    if(gu8_eeprom_rbuf[0] == EEPROM_CHECK_A_VALUE)
    {
        EepromByteWrite(EEPROM_ADDR_TEST, EEPROM_CHECK_B_VALUE);

        EepromSeqRead(EEPROM_ADDR_TEST, gu8_eeprom_rbuf, 1);

        if(gu8_eeprom_rbuf[0] == EEPROM_CHECK_B_VALUE)
        {
            bit_eeprom_err = CLEAR;
        }
        else
        {
            bit_eeprom_err = 1;
        }
    }
    else
    {
        EepromByteWrite(EEPROM_ADDR_TEST, EEPROM_CHECK_A_VALUE);

        EepromSeqRead(EEPROM_ADDR_TEST, gu8_eeprom_rbuf, 1);

        if(gu8_eeprom_rbuf[0] == EEPROM_CHECK_A_VALUE)
        {
            bit_eeprom_err = CLEAR;
        }
        else
        {
            bit_eeprom_err = 1;
        }
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void eeprom_write_cycle_time(void)
{
    U16 mu16temp = 0;

    /*..hui [17-12-8���� 1:16:26] �� 5.3ms..*/
    /*..hui [19-10-21���� 1:25:35] �� 16ms, typical 12ms, max 20ms..*/
    for(mu16temp = 0 ; mu16temp < 13000 ; mu16temp++)
    {
       NOP(); NOP(); NOP(); NOP(); NOP();
       NOP(); NOP(); NOP(); NOP(); NOP();
       NOP(); NOP(); NOP(); NOP(); NOP();
       NOP(); NOP(); NOP(); NOP(); NOP();
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void EepromByteWrite( U16 mu16Addr, U8 mu8Data )
{
    U8 mu8_temp = 0;


    HAL_RTC_ByteWrite( DEV_ADDR_EEP, mu16Addr, mu8Data);

    gu16_eep_cnt++;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void EepromPageWrite(U16 mu16Addr, U8 *pData, U8 mu8len )
{
    U8 i = 0;
    U8 mu8PageNum = 0;

    EepromPageWriteSub( mu16Addr, pData, mu8len );

    gu16_eep_cnt++;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void EepromPageWriteSub( U16 mu16Addr, U8 *pData, U8 mu8len )
{
    U8 mu8_temp = 0;


    HAL_RTC_PageWrite( DEV_ADDR_EEP, mu16Addr, pData, mu8len);
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void EepromSeqRead( U16 mu16Addr, U8 *pData, U8 mu16Length )
{
    U8 mu8_temp = 0;


    HAL_RTC_SeqRead( DEV_ADDR_EEP, mu16Addr, pData, mu16Length );
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/





