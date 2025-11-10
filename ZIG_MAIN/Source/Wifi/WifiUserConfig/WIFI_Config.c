
#include "WIFI_Config.h"

/******************************************************************************/
/***** User Header ************************************************************/
/******************************************************************************/
/* Start user code for include (User heders). */
/******************************************************************************/
// #include "Voice_NXD1005.h"
#include "macrodriver.h"
#include "Global_Variable.h"
#include "Port_Define.h"
/******************************************************************************/
extern bit F_Wifi_Power;
extern bit F_Wifi_First_Paring;
extern bit F_Wifi_Fota_MODULE;
extern bit F_Wifi_Fota_MCU;

extern bit F_ExtractKeySet;
extern bit F_WaterOut;
extern bit bit_self_test_start;
extern bit F_Tank_Cover_Input;
extern bit bit_self_test_start;

extern bit bit_ice_tank_ster_start;
extern bit bit_filter_all;
extern FLUSHING_STEP gu8_flushing_mode;
extern bit F_First_Hot_Effluent;
extern void play_melody_setting_on_198(void);
extern void Play_Voice(U16 mu16MemoryAddress);
extern void get_wifi_time ( U16 mYear, U16 mMonth, U16 mDate, U16 mHour, U16 mMin, U16 mSec );
extern void start_fota(void);



/******************************************************************************/

/* End user code.*/


/******************************************************************************/
/***** EEPROM Setting *********************************************************/
/******************************************************************************/
/* User eeprom write settings */
void UserWriteEep ( U16 mu16Addr, U8 mu8Val )
{
    if( mu16Addr == EEPROM_ADDR_WIFI_POWER )
    {
        /* Start user code.*/

        // Ex)
        // WiFi.mUonOff = mu8Val;
        F_Wifi_Power = mu8Val;

        /* End user code. */
    }
    else if( mu16Addr == EEPROM_ADDR_FIRST_PARING )
    {
        /* Start user code.*/

        // WiFi.mUfirstParing = mu8Val;
        F_Wifi_First_Paring = mu8Val;

        /* End user code. */
    }
    else if( mu16Addr == EEPROM_ADDR_FOTA_MODULE )
    {
        /* Start user code.*/

        // WiFi.mUfotaModule = mu8Val;
        F_Wifi_Fota_MODULE = mu8Val;

        /* End user code. */
    }
    else if( mu16Addr == EEPROM_ADDR_FOTA_MCU )
    {
        /* Start user code.*/

        // WiFi.mUfotaMain = mu8Val;
        F_Wifi_Fota_MCU = mu8Val;

        /* End user code. */
    }
}

/* User eeprom read settings */
U8 UserReadEep ( U16 mu16Addr )
{
    U8 mu8Data = 0U;

    if( mu16Addr == EEPROM_ADDR_WIFI_POWER )
    {
        /* Start user code.*/

        // return WiFi.mUonOff;
        // mu8Data = WiFi.mUonOff;
        mu8Data = (U8)F_Wifi_Power;

        /* End user code. */
    }
    else if( mu16Addr == EEPROM_ADDR_FIRST_PARING )
    {
        /* Start user code.*/

        // return WiFi.mUfirstParing;
        mu8Data = (U8)F_Wifi_First_Paring;

        /* End user code. */
    }
    else if( mu16Addr == EEPROM_ADDR_FOTA_MODULE )
    {
        /* Start user code.*/

        // return WiFi.mUfotaModule;
        mu8Data = (U8)F_Wifi_Fota_MODULE;

        /* End user code. */
    }
    else if( mu16Addr == EEPROM_ADDR_FOTA_MCU )
    {
        /* Start user code.*/

        // return WiFi.mUfotaMain;
        mu8Data = (U8)F_Wifi_Fota_MCU;

        /* End user code. */
    }

    return mu8Data;
}
/* User buzzsound/voice settings */

void UserBuzzSound ( U16 mu16Buzz )
{
    U16 Sound = 0;
    switch( mu16Buzz )
    {
        case WIFI_BUZZER_SELECT :       // �� (������ "��")
            /* Start user code.*/
            // ex) Sound = 1196;
            Play_Voice(VOICE_196_MELODY_SELECT);

            /* End user code. */
            break;

        case WIFI_BUZZER_AP_CONNECT :   // AP���� (������ "�츮��" / Wi-Fi �����尡 Ȱ��ȭ �Ǿ����ϴ�.)
            /* Start user code.*/
            // ex) Sound = 1001;
            Play_Voice(VOICE_23_INFORM_APP_PAIRING_SUCCESS_WITH_WIFI);

            /* End user code. */
            break;

        case WIFI_BUZZER_BLE_CONNECT :  // AP���� (������ "�츮��" / BLE�����尡 Ȱ��ȭ �Ǿ����ϴ�.)
            /* Start user code.*/
            // ex) Sound = 1001;
            Play_Voice(VOICE_19_START_BLE_PAIRNG);

            /* End user code. */
            break;

        case WIFI_BUZZER_SETUP :        // WIFI ON (������ "�򵿢�" / ��ǰ�� Wi-Fi�� �������ϴ�.)
            /* Start user code.*/
            // ex) Sound = 1198;
            Play_Voice(VOICE_27_INFORM_WIFI_ON);

            /* End user code. */
            break;

        case WIFI_BUZZER_SERVER_CONNECT :   // �������� (������ "�츮����" / ��ǰ�� ���� ���������� ����Ǿ����ϴ�.)
            /* Start user code.*/
            // ex) Sound = 0xFFFF;
            Play_Voice(VOICE_23_INFORM_APP_PAIRING_SUCCESS_WITH_WIFI);

            /* End user code. */
            break;

        case WIFI_BUZZER_CANCEL :           // WIFI OFF (������ "����" / ��ǰ�� Wi-Fi�� �������ϴ�.)
            /* Start user code.*/
            // ex) Sound = 1199;
            Play_Voice(VOICE_28_INFORM_WIFI_OFF);

            /* End user code. */
            break;

        case WIFI_BUZZER_AP_START :         // AP������ (������ "�� ��")
            /* Start user code.*/
            // ex) Sound = 1198;
            Play_Voice(VOICE_198_MELODY_SET_ON);

            /* End user code. */
            break;

        case WIFI_BUZZER_ERROR :            // �Ұ��� (������ "������")
            /* Start user code.*/
            // ex) Sound = 1197;
            Play_Voice(VOICE_197_MELODY_WARNING);

            /* End user code. */
            break;

        case WIFI_BUZZER_WiFi_OFF :         // Wi-Fi �� ������(����) / Wi-Fi ���� ��尡 ��Ȱ��ȭ �Ǿ����ϴ�.
            /* Start user code.*/
            // ex) Sound = 1199;
            Play_Voice(VOICE_25_INFORM_WIFI_MODE_DEACTIVATED);

            /* End user code. */
            break;

        case WIFI_BUZZER_BLE_OFF :          // �������� �� ������(����) / �������� ���� ��尡 ��Ȱ��ȭ �Ǿ����ϴ�.
            /* Start user code.*/
            // ex) Sound = 1199;
            Play_Voice(VOICE_21_INFORM_APP_PAIRING_FAIL_WITH_BLE);

            /* End user code. */
            break;

        case WIFI_BUZZER_BLE_CERT :         // (������ "��") / �������� �������� Ȱ��ȭ ���� / ��ǰ������ ���� ���Դϴ�. �ۿ��� ���̵��ϴ� ��ư�� �����ּ���.
            /* Start user code.*/
            // ex) Sound = 1196;
            Play_Voice(VOICE_29_APPLY_PRODUCT_CERT_OCCUPATION);

            /* End user code. */
            break;

        case WIFI_BUZZER_SERVER_FIRST :     // �� �Ϸ� �� �������� (������ "�츮����" / ��ǰ�� ���� ���������� ����Ǿ����ϴ�.
            /* Start user code.*/
            // ex) Sound = 0xFFFF;
            Play_Voice(VOICE_30_INFORM_APP_PAIRING_SUCCESS_WITH_WIFI);

            /* End user code. */
            break;

        case WIFI_BUZZER_SMART_CHECK_OK :       // (������ "�� ��") / ����Ʈ ������ �Ϸ� �Ǿ� ��ǰ�� ����Ͻ� �� �ֽ��ϴ�.
            /* Start user code.*/
            // ex) Sound = 1198;
            Play_Voice(VOICE_17_FINISH_SMART_TESTING_MODE_NO_ERROR);

            /* End user code. */
            break;

        case WIFI_BUZZER_SMART_CHECK_ERROR :    // �Ұ��� (������ "������") / ����Ʈ ������ �Ϸ� �Ǿ����ϴ�.
            /* Start user code.*/
            // ex) Sound = 1197;
            Play_Voice(VOICE_18_FINISH_SMART_TESTING_MODE_WITH_ERROR);

            /* End user code. */
            break;

        default :
            break;
    }

    if (Sound != 0xFFFFU)
	{
        /* Start user code.*/

		// Ex) Play_Voice(Sound);

        /* End user code. */
	}
}

/* Time Setting Option (When using A2010 Time) - Called when time data is received*/
void SetUserSystemTime ( U16 mYear, U16 mMonth, U16 mDate, U16 mHour, U16 mMin, U16 mSec, U16 mDay )
{   /* Start user code.*/
    /* �������� �ð� ������ ������Ʈ */
    get_wifi_time( mYear, mMonth, mDate, mHour, mMin, mSec);


}   /* End user code. */


/* User FOTA Display Setting */
void SetUserSystemDisplay ( U8 mu8Display )
{
    if ( mu8Display == USER_DISPLAY_FOTA ) // FOTA
    {
        /* Start user code.*/
        start_fota();
        /* End user code. */
    }
}

/* Product Status Information Settings */
void SetUserSystemStatusData ( U8 mu8StatusType )
{
    switch ( mu8StatusType )
    {
        case USER_MODEL_TYPE:   // �� Ÿ�� ����
        /*
            SetUserSystemStatus(mu8StatusType, 0U);       // e.g. CHP
            SetUserSystemStatus(USER_MODEL_TYPE, 1U);    // e.g. CP
        */
        SetUserSystemStatus(mu8StatusType, 0U); // CHPI (Model 0)

        break;

        case USER_FACTORY_STATUS:       // Wi-Fi �˻��� ���� ����
        /*
            SetUserSystemStatus(mu8StatusType, 0U);       // e.g. �˻��� �ƴ�
            SetUserSystemStatus(mu8StatusType, 1U);       // e.g. �˻��� ����
        */

        break;

        case USER_RX_STATUS:    // �Ϲ����� �Ұ����� ����
        /*
            SetUserSystemStatus(mu8StatusType, 0U);       // e.g. �����
            SetUserSystemStatus(mu8StatusType, 1U);       // e.g. ����Ұ�
        */

        if((F_ExtractKeySet == SET)
        || (bit_ice_tank_ster_start == SET)
        || (F_WaterOut == SET)
        || (bit_self_test_start == SET)
        || (F_Tank_Cover_Input == CLEAR)
        || (bit_filter_all == CLEAR)
        )
        {
            SetUserSystemStatus(mu8StatusType, 1U);     // �Ұ�
        }
        else
        {
            SetUserSystemStatus(mu8StatusType, 0U);     // ����
        }
        break;

        case USER_SMART_POSSIBLE_STATUS:    // ����Ʈ���� �Ұ����� ����
        /*
            SetUserSystemStatus(mu8StatusType, 0U);       // e.g. ���ܰ���
            SetUserSystemStatus(mu8StatusType, 1U);       // e.g. ���ܺҰ�
        */

        if((F_ExtractKeySet == SET)
        || (bit_ice_tank_ster_start == SET)
        || (F_WaterOut == SET)
        || (F_Tank_Cover_Input == CLEAR)
        || (bit_filter_all == CLEAR)
        || (F_First_Hot_Effluent == SET)        // ���Ϳ� ���� ä���� ������ ������ ���� �������� �ȵ� (�÷��� ��ŵ�� �ȵ�)
        )
        {
            SetUserSystemStatus(mu8StatusType, 1U);
        }
        else
        {
            SetUserSystemStatus(mu8StatusType, 0U);
        }

        break;

        case USER_FOTA_POSSIBLE_STATUS:     // FOTA �Ұ� ���� ����
        /*
            SetUserSystemStatus(mu8StatusType, 0U);       // e.g. FOTA����
            SetUserSystemStatus(mu8StatusType, 1U);       // e.g. FOTA�Ұ�
        */

        if ((F_ExtractKeySet == SET)
        || (bit_ice_tank_ster_start == SET)
        || (F_WaterOut == SET)
        )
        {
            SetUserSystemStatus(mu8StatusType, 1U);
        }
        else
        {
            SetUserSystemStatus(mu8StatusType, 0U);
        }

        break;

        case USER_SMART_SENSING_STATUS:     // ����Ʈ���� �� ���� ����
        /*
            SetUserSystemStatus(mu8StatusType, 0U);       // e.g. �Ϲ� ���� (����Ʈ���� �� ���� �ƴ�)
            SetUserSystemStatus(mu8StatusType, 1U);       // e.g. ����Ʈ ���� ���� ��
        */

        if ((bit_self_test_start == SET))
        {
            SetUserSystemStatus(mu8StatusType, 1U);
        }
        else
        {
            SetUserSystemStatus(mu8StatusType, 0U);
        }
        break;

        case USER_FACTORY_DIRECTFOTA_STATUS:    // ���̷�Ʈ ��Ÿ ���� ����
        /*
            SetUserSystemStatus(mu8StatusType, 0U);       // e.g. ���̷�Ʈ FOTA ��� �ƴ�
            SetUserSystemStatus(mu8StatusType, 1U);       // e.g. ���̷�Ʈ FOTA ���
        */
        default:

        break;
    }

}
