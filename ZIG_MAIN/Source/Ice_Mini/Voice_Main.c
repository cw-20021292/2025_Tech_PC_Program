/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Main.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  : AT+CONNECT=74F07DB01010
***********************************************************************************************************************/
#include    "Macrodriver.h"
#include    "Global_Variable.h"
#include    "Port_Define.h"
#include    "Voice_Main.h"

//void VoiceIC_Type_Check(void);
void Voice_Initialize(void);
void ProcessVoice_Main(void);
void Play_Voice(U16 mu16MemoryAddress);
U8 IsLevel_Mute(U16 addr);
U8 IsLevel_Melody(U16 addr);
/*U8 IsLevel_Voice(U16 addr);*/
U8 IsCriticalVoice(U16 addr);



U16 gu16_voice_address;
U16 gu16_voice_command;

U8 gu8VoiceVolumeLevel;
U8 gu8Sound_Type;                       // 음성안내 모드 (기존 gu8VoiceLevel)

U8 gu8_voice_language_type;


bit F_OneMoreSound;
bit F_Beep_Sound;
bit F_PlayVoice;     /*Sound Level에 따른 Address 받으면 SET 하여 음성 출력*/


bit bit_power_on_voice;

U8 gu8Voice_IC_Type;

U8 gu8_voice_land_select_timer;
U8 gu8_nyquest_select_timer;
U8 gu8_voice_sel_finish;

U8 gu8_power_on_timer;




/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void Voice_Initialize(void)
{
    /*..hui [21-9-8오후 3:18:10] 포타 진행중 부팅음 안나게..*/
    if( gu8_fota_start == SET )
    {
        bit_power_on_voice = SET;
        gu8_fota_start = CLEAR;

        /*..hui [24-1-18오후 5:30:03] FOTA 완료 후 BLE 활성화..*/
        WifiKey(WIFI_KEY_BLE);
    }
    else
    {
        bit_power_on_voice = CLEAR;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void ProcessVoice_Main(void)
{  
    if( bit_power_on_voice == CLEAR )
    {
        gu8_power_on_timer++;

        if( gu8_power_on_timer >= 10 )
        {
            gu8_power_on_timer = 0;
            bit_power_on_voice = SET;

    		Play_Voice(VOICE_191_MELODY_PLUG_IN_COWAY);
        }
        else{}
    }
    else{}

    gu16_voice_command = 0;   /*..모두 전송하고 나면 Command 초기화..*/
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void Play_Voice(U16 mu16MemoryAddress)
{
    if( (mu16MemoryAddress == VOICE_151_SPECIAL_FUNCTION) 
    // || (mu16MemoryAddress == VOICE_191_MELODY_PLUG_IN_COWAY)
    // || (mu16MemoryAddress == VOICE_196_MELODY_SELECT)
    // || (mu16MemoryAddress == VOICE_197_MELODY_WARNING)
    // || (mu16MemoryAddress == VOICE_198_MELODY_SET_ON)
    // || (mu16MemoryAddress == VOICE_199_MELODY_SET_OFF)
    || (mu16MemoryAddress == VOICE_196_WIFI_SELECT)
    || (mu16MemoryAddress == VOICE_197_WIFI_WARNING)
    || (mu16MemoryAddress == VOICE_198_WIFI_SET_ON)
    || (mu16MemoryAddress == VOICE_199_WIFI_SET_OFF)
    || (mu16MemoryAddress == VOICE_151_INFORM_MY2_RECIPE)
    // || (mu16MemoryAddress == VOICE_192_MELODY_DISPENSE_CONTINUOUS)
    // || (mu16MemoryAddress == VOICE_193_MELODY_DISPENSE_START)
    // || (mu16MemoryAddress == VOICE_194_MELODY_DISPENSE_FINISH)
    )
    {
        gu16_voice_address = mu16MemoryAddress;
    }
    else
    {
        gu16_voice_address = mu16MemoryAddress + KR_25;
    }

    if( gu8Sound_Type == SOUND_TYPE_MUTE )
    {
        if( IsLevel_Mute( mu16MemoryAddress ) == TRUE )
        {
            F_PlayVoice = SET;
        }
        else{}
    }
    else if( gu8Sound_Type == SOUND_TYPE_MELODY )
    {
        if( IsLevel_Mute( mu16MemoryAddress ) == TRUE
        || IsLevel_Melody( mu16MemoryAddress ) == TRUE )
        {
            F_PlayVoice = SET;
        }
        else{}
    }
    else if( gu8Sound_Type == SOUND_TYPE_VOICE )
    {
        /*..hui [24-5-30오후 2:19:32] 롬사이즈 다이어트.. 음성출력은 전부 다 출력..*/
        F_PlayVoice = SET;
    }
    else{}

    /////Voice_Stop_Next_Lab();
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  : 음성안내를 [음소거]로 설정한 상태에서도 나와야 하는 음성들 정의
***********************************************************************************************************************/
U8 IsLevel_Mute(U16 addr)
{
    switch( addr )
    {
        /* Errors */
        case VOICE_4_CALL_AS_COLD_WATER_ERROR:
        case VOICE_5_CALL_AS_HOT_WATER_ERROR:
        case VOICE_6_CALL_AS_ICE_ERROR:
        case VOICE_7_CALL_AS_WATER_SUPPLY_ERROR:
        case VOICE_8_CALL_AS_WATER_LEAKAGE_ERROR:

        /* Wifi,BLE,IoCare */
        case VOICE_16_START_SMART_TESTING_MODE:
        case VOICE_17_FINISH_SMART_TESTING_MODE_NO_ERROR:
        case VOICE_18_FINISH_SMART_TESTING_MODE_WITH_ERROR:
        case VOICE_19_START_BLE_PAIRNG:
        case VOICE_20_INFORM_APP_PAIRING_SUCCESS_WITH_BLE:
        case VOICE_21_INFORM_APP_PAIRING_FAIL_WITH_BLE:
        case VOICE_22_START_WIFI_PAIRNG:
        case VOICE_23_INFORM_APP_PAIRING_SUCCESS_WITH_WIFI:
        case VOICE_24_INFORM_APP_PAIRING_FAIL_WITH_WIFI:
        case VOICE_25_INFORM_WIFI_MODE_DEACTIVATED:
        case VOICE_26_APPLY_WIFI_AFTER_DEACTIVATING:
        case VOICE_27_INFORM_WIFI_ON:
        case VOICE_28_INFORM_WIFI_OFF:
        case VOICE_29_APPLY_PRODUCT_CERT_OCCUPATION:
        case VOICE_30_INFORM_APP_PAIRING_SUCCESS_WITH_WIFI:
        case VOICE_31_INFORM_APP_PAIRING_FAIL_WITH_WIFI:
        case VOICE_32_INFORM_APP_PAIRING_FAIL_WITH_WIFI:
        case VOICE_34_INFORM_WIFI_BEFORE_FLUSHING:
        case VOICE_196_WIFI_SELECT:
        case VOICE_197_WIFI_WARNING:
        case VOICE_198_WIFI_SET_ON:
        case VOICE_199_WIFI_SET_OFF:

        /* 설치 및 필터교체 */
        case VOICE_35_INFORM_FLUSHING_START:
        case VOICE_36_INFORM_FLUSHING_FINISH:
        case VOICE_33_INFORM_PROEDUCT_BEFORE_FLUSHING:

        /* 음성, 효과음 */
        case VOICE_65_VOICE_MODE:
        case VOICE_66_MELODY_MODE:
        case VOICE_67_MUTE_MODE:
        case VOICE_68_MODE_SET_VOICE:
        case VOICE_69_MODE_SET_MELODY:
        case VOICE_70_MODE_SET_MUTE:

        /* 부팅음 */
        case VOICE_191_MELODY_PLUG_IN_COWAY:
            return TRUE;

        default :
            return FALSE;
    }

    return FALSE;
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  : 음성안내를 [멜로디]로 설정한 상태에서도 나와야 하는 음성들 정의
***********************************************************************************************************************/
U8 IsLevel_Melody(U16 addr)
{
    switch( addr )
    {
        case VOICE_4_CALL_AS_COLD_WATER_ERROR:
        case VOICE_5_CALL_AS_HOT_WATER_ERROR:
        case VOICE_6_CALL_AS_ICE_ERROR:
        case VOICE_7_CALL_AS_WATER_SUPPLY_ERROR:
        case VOICE_8_CALL_AS_WATER_LEAKAGE_ERROR:

        case VOICE_16_START_SMART_TESTING_MODE:
        case VOICE_17_FINISH_SMART_TESTING_MODE_NO_ERROR:
        case VOICE_18_FINISH_SMART_TESTING_MODE_WITH_ERROR:
        case VOICE_19_START_BLE_PAIRNG:
        case VOICE_20_INFORM_APP_PAIRING_SUCCESS_WITH_BLE:
        case VOICE_21_INFORM_APP_PAIRING_FAIL_WITH_BLE:
        case VOICE_22_START_WIFI_PAIRNG:
        case VOICE_23_INFORM_APP_PAIRING_SUCCESS_WITH_WIFI:
        case VOICE_24_INFORM_APP_PAIRING_FAIL_WITH_WIFI:
        case VOICE_25_INFORM_WIFI_MODE_DEACTIVATED:
        case VOICE_26_APPLY_WIFI_AFTER_DEACTIVATING:
        case VOICE_27_INFORM_WIFI_ON:
        case VOICE_28_INFORM_WIFI_OFF:
        case VOICE_29_APPLY_PRODUCT_CERT_OCCUPATION:
        case VOICE_30_INFORM_APP_PAIRING_SUCCESS_WITH_WIFI:
        case VOICE_31_INFORM_APP_PAIRING_FAIL_WITH_WIFI:
        case VOICE_32_INFORM_APP_PAIRING_FAIL_WITH_WIFI:
        case VOICE_34_INFORM_WIFI_BEFORE_FLUSHING:
        case VOICE_196_WIFI_SELECT:
        case VOICE_197_WIFI_WARNING:
        case VOICE_198_WIFI_SET_ON:
        case VOICE_199_WIFI_SET_OFF:
        
        case VOICE_33_INFORM_PROEDUCT_BEFORE_FLUSHING:
        case VOICE_35_INFORM_FLUSHING_START:
        case VOICE_36_INFORM_FLUSHING_FINISH:

        case VOICE_65_VOICE_MODE:
        case VOICE_66_MELODY_MODE:
        case VOICE_67_MUTE_MODE:
        case VOICE_68_MODE_SET_VOICE:
        case VOICE_69_MODE_SET_MELODY:
        case VOICE_70_MODE_SET_MUTE:

        case VOICE_191_MELODY_PLUG_IN_COWAY:

        case VOICE_192_MELODY_DISPENSE_CONTINUOUS:
        case VOICE_193_MELODY_DISPENSE_START:
        case VOICE_194_MELODY_DISPENSE_FINISH:
        case VOICE_196_MELODY_SELECT:
        case VOICE_197_MELODY_WARNING:
        case VOICE_198_MELODY_SET_ON:
        case VOICE_199_MELODY_SET_OFF:

            return TRUE;

        default :
            return FALSE;
    }

    return FALSE;
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 IsCriticalVoice(U16 addr)
{
    switch( addr )
    {
        return TRUE;

        default :
            return FALSE;
    }

    return FALSE;
}



/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/



