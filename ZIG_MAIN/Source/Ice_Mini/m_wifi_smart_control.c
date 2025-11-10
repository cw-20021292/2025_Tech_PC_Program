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
#include    "m_wifi_smart_control.h"
#include    "WIFI_Status.h"

void wifi_smart_control(void);
void get_wifi_info_data(void);
void get_wifi_connect(void);
void get_wifi_pairing(void);
void get_wifi_error(void);
void get_wifi_power(void);
U8 change_error_number( U8 mu8_error );
/////void get_wifi_filter_period(void);
/////void change_filter_period(void);
void get_wifi_cert(void);
void get_smart_image(void);
U8 decision_day_or_night(void);
void get_server_connect(void);





U8 gu8_wifi_weather;
U8 gu8_wifi_aqi;

S8 gs8_signed_wifi_temperature;
U8 gu8_unsigned_wifi_temperature;

U8 gu8_wifi_temperature;


U8 gu8_Wifi_Connect_State;

U8 gu8_Wifi_Pairing_State;
U8 gu8_Wifi_Pairing_Error;
U8 gu8_Wifi_Last_Error;


U8 gu8_smart_image;
U8 gu8_smart_text;

U8 gu8_ap_mode;

U8 gu8_smart_standby_mode;

U8 gu8_custom_cup_setting;


U8 gu8_wifi_power_temporary;
U8 gu8_wifi_ap_first_temporary;

U8 gu8_wifi_ap_mode;
U8 gu8_ble_ap_mode;

U8 gu8_wifi_disp_timer;

U16 gu16_wifi_pairing_30min_timer;
U8 gu8_wifi_pairing_5sec_timer;
bit bit_pairing_5s_display_start;

U8 gu8_fota_start;


U8 gu8_wifi_filter_cycle_percent;
bit bit_filter_cycle_change;

U16 gu16_define_filter_period__neo;
U16 gu16_define_filter_period__ro;
U16 gu16_define_filter_period__ino;

U16 gu16_test_cycle;

U8 gu8_Wifi_Cert_State;

bit bit_wifi_pairing_start;
bit bit_display_last_error;

U8 gu8_Server_Connect_State;

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void wifi_smart_control(void)
{
    get_wifi_connect();
    get_wifi_pairing();
    get_wifi_error();
    get_wifi_power();

    /////get_wifi_filter_period();
    get_wifi_info_data();
    get_smart_image();

    get_server_connect();
    get_wifi_cert();
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void get_wifi_info_data(void)
{
    U8 mu8_connect_state = 0;
    U8 mu8_data = 0;
    S8 ms8_temp = 0;
    U8 mu8_temp = 0;
    U8 mu8_aqi = 0;

    mu8_data = (U8)GetWifiRequestValue( WIFI_RQST_WEATHER );

    /*if( mu8_data >= WEATHER_SUNNY && mu8_data <= WEATHER_FOG )*/
    if( mu8_data >= SMART_WEATHER_MIN && mu8_data <= SMART_WEATHER_MAX )
    {
        gu8_wifi_weather = mu8_data;
    }
    else{}

    mu8_aqi  = (U8)GetWifiRequestValue( WIFI_RQST_AQI );

    if( mu8_aqi >= AQI_GOOD && mu8_aqi <= AQI_VERY_BAD )
    {
        gu8_wifi_aqi = mu8_data;
    }
    else{}

    /*..hui [21-3-12오후 2:49:14] 케이웨더 서버 이상 시 날씨, 미세먼지, 시간 값 0xFF로 들어옴 - 현민..*/
    /*..hui [21-3-12오후 2:49:45] 온도는 0xff가 범위에 포함되므로 aqi 값으로 서버 이상 확인 후 적용..*/
    /*..hui [21-3-12오후 2:51:29] 케이웨더 서버 고장시에는 이전 값 표시 유지..*/
    /*if( mu8_data != 0xff )*/

}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void get_wifi_connect(void)
{
    U8 mu8_connect_state = 0;

    mu8_connect_state = GetWifiStatusValue( WIFI_STATUS_DISP );
    // mu8_connect_state = GetWifiStatusValue( WIFI_STATUS_SERVER_CON );

    if( mu8_connect_state == WIFI_DISP_OFF )
    {
        gu8_Wifi_Connect_State = WIFI_OFF;
    }
    else if( mu8_connect_state == WIFI_DISP_CONNECT )
    {
        gu8_Wifi_Connect_State = WIFI_CONNECT;
    }
    else
    {
        gu8_Wifi_Connect_State = WIFI_DISCONNECT;
    }

}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void get_wifi_pairing(void)
{
    U8 mu8_pairing_state = 0;

    mu8_pairing_state = GetWifiStatusValue( WIFI_STATUS_DISP );

    gu8_wifi_ap_mode  = GetWifiStatusValue( WIFI_STATUS_AP_MODE );
    gu8_ble_ap_mode  = GetWifiStatusValue( WIFI_STATUS_AP_BLE );

    // if( gu8_Wifi_Connect_State == WIFI_CONNECT )
    // {
    //     gu8_Wifi_Pairing_State = WIFI_PAIRING_STEP_FINISH;
    //     return;
    // }
    // else{}

    if( gu8_wifi_ap_mode == SET )
    {
        if( mu8_pairing_state == WIFI_DISP_AP_START )
        {
            gu8_Wifi_Pairing_State = WIFI_PAIRING_STANDBY;
        }
        else if( mu8_pairing_state == WIFI_DISP_AP_STEP0 )
        {
            gu8_Wifi_Pairing_State = WIFI_PAIRING_STEP0_PREPARE;
        }
        else if( mu8_pairing_state == WIFI_DISP_AP_STEP1 )
        {
            /*..hui [23-12-20오후 1:22:03] 점유인증되면 STEP1이 날라옴.. 그때부터 25%로 화면 표시 시작..*/
            if( gu8_Wifi_Pairing_State != WIFI_PAIRING_STEP1_25_PERCENT )
            {
                gu8_Wifi_Pairing_State = WIFI_PAIRING_STEP1_25_PERCENT;

                if( gu8_Wifi_Pairing_Error == 0 )
                {
                    bit_wifi_pairing_start = SET;
                    bit_display_last_error = CLEAR;
                }
                else{}
            }
            else{}

        }
        else if( mu8_pairing_state == WIFI_DISP_AP_STEP2 )
        {
            gu8_Wifi_Pairing_State = WIFI_PAIRING_STEP2_50_PERCENT;
        }
        else if( mu8_pairing_state == WIFI_DISP_AP_STEP3 )
        {
            gu8_Wifi_Pairing_State = WIFI_PAIRING_STEP3_75_PERCENT;
        }
        else
        {
            gu8_Wifi_Pairing_State = WIFI_PAIRING_STANDBY;
        }
    }
    else if( gu8_ble_ap_mode == SET )
    {
        if( mu8_pairing_state == WIFI_DISP_BLE_START )
        {
            gu8_Wifi_Pairing_State = WIFI_PAIRING_STANDBY;
        }
        else if( mu8_pairing_state == WIFI_DISP_BLE_STEP0 )
        {
            gu8_Wifi_Pairing_State = WIFI_PAIRING_STEP0_PREPARE;
        }
        else if( mu8_pairing_state == WIFI_DISP_BLE_STEP1 )
        {
            /*..hui [23-12-20오후 1:22:03] 점유인증되면 STEP1이 날라옴.. 그때부터 25%로 화면 표시 시작..*/
            if( gu8_Wifi_Pairing_State != WIFI_PAIRING_STEP1_25_PERCENT )
            {
                gu8_Wifi_Pairing_State = WIFI_PAIRING_STEP1_25_PERCENT;

                if( gu8_Wifi_Pairing_Error == 0 )
                {
                    bit_wifi_pairing_start = SET;
                    bit_display_last_error = CLEAR;
                }
                else{}
            }
            else{}
        }
        else if( mu8_pairing_state == WIFI_DISP_BLE_STEP2 )
        {
            gu8_Wifi_Pairing_State = WIFI_PAIRING_STEP2_50_PERCENT;
        }
        else if( mu8_pairing_state == WIFI_DISP_BLE_STEP3 )
        {
            gu8_Wifi_Pairing_State = WIFI_PAIRING_STEP3_75_PERCENT;
        }
        else
        {
            gu8_Wifi_Pairing_State = WIFI_PAIRING_STANDBY;
        }
    }
    else
    {
        gu8_Wifi_Pairing_State = WIFI_PAIRING_STANDBY;
        bit_wifi_pairing_start = CLEAR;
    }

}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void get_wifi_error(void)
{
    U8 mu8_error = 0;

    mu8_error = GetWifiStatusValue(WIFI_STATUS_AP_ERROR);

    gu8_Wifi_Pairing_Error = change_error_number( mu8_error );


    mu8_error = GetWifiStatusValue(WIFI_STATUS_ERROR);

    gu8_Wifi_Last_Error = change_error_number( mu8_error );
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void get_wifi_power(void)
{
    /*..hui [21-8-2오후 8:23:11] 전원 상태 및 첫 페어링 상태..*/
    /*..hui [21-8-2오후 8:23:24] 설정에서 현민이 쪽에서 가져다 쓰기위해..*/
    gu8_wifi_power_temporary = GetWifiStatusValue(WIFI_STATUS_POWER);

    gu8_wifi_ap_first_temporary = GetWifiApStatus(STATUS_AP_FIRST);
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 change_error_number( U8 mu8_error )
{
    U8 mu8_return = 0;

    if( mu8_error == WIFI_DISP_AP1 )
    {
        /*..hui [21-10-5오후 2:10:03] AP1 - 무선공유기 암호불일치..*/
        mu8_return = WIFI_FAIL_PASS__AP1;
    }
    else if( mu8_error == WIFI_DISP_AP2 )
    {
        /*..hui [21-10-5오후 2:10:13] AP2 - 무선공유기 미검색 1분..*/
        mu8_return = WIFI_NO_ACK__AP2;
    }
    else if( mu8_error == WIFI_DISP_AP3 )
    {
        /*..hui [21-10-5오후 2:10:25] AP3 - 무선공유기 접속거부..*/
        mu8_return = WIFI_ERROR_DENY__AP3;
    }
    else if( mu8_error == WIFI_DISP_AP4 )
    {
        /*..hui [21-10-5오후 2:10:33] AP4 - 무선공유기 응답없음..*/
        mu8_return = WIFI_NO_AP__AP4;
    }
    else if( mu8_error == WIFI_DISP_AP5 )
    {
        /*..hui [21-10-5오후 2:10:41] AP5 - 서버 응답없음..*/
        mu8_return = WIFI_NO_SERVER__AP5;
    }
    else
    {
        mu8_return = 0;
    }

    return mu8_return;
}



/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void get_wifi_cert(void)
{
    U8 mu8_connect_state = 0;

    gu8_Wifi_Cert_State = GetWifiStatusValue(WIFI_STATUS_BLE_CERT);
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void get_smart_image(void)
{
    U8 mu8_day = 0;

    /*..hui [21-3-8오후 6:59:16] 와이파이 연결 안됐으면 와이파이 연결 권고 이미지..*/
    if( gu8_Wifi_Connect_State == WIFI_OFF )
    {
        gu8_smart_image = SMART_ICON_NONE;
        return;
    }
    else if( gu8_Wifi_Connect_State == WIFI_DISCONNECT )
    {
        gu8_smart_image = SMART_ICON_NONE;
        return;
    }
    else{}

    /*mu8_day = decision_day_or_night();*/

    switch ( gu8_wifi_weather )
    {
        case WEATHER_SUNNY:
        case WEATHER_BIT_CLOUD:

            gu8_smart_image = SMART_ICON_SUNNY;

        break;

        case WEATHER_LOTS_CLOUD:
        case WEATHER_CLOUDY:
        case WEATHER_THUNDER:
        case WEATHER_FOG:
            gu8_smart_image = SMART_ICON_CLOUDY;
        break;

        case WEATHER_CLOUDY_AND_RAINY:
        case WEATHER_CLOUDY_AND_SNOWY:
        case WEATHER_SUNNY_AFTER_RAINY:
        case WEATHER_SHOWER:
        case WEATHER_RAINY_OR_SNOWY:
        case WEATHER_SNOWY_OR_RAINY:
            gu8_smart_image = SMART_ICON_RAINY;
        break;


        default:
            gu8_wifi_weather = SMART_ICON_NONE;
        break;
    }


}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 decision_day_or_night(void)
{
    U8 mu8_return = 0;

    /*..hui [21-3-8오후 6:57:55] 아침 5:00 부터 ~ 오후 17시 59분 까지 낮..*/
    if( gu8_rtc_time_Hour >= 5 && gu8_rtc_time_Hour < 18  )
    {
        mu8_return = SMART_DAY;
    }
    else
    {
        mu8_return = SMART_NIGHT;
    }

    return mu8_return;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void get_server_connect(void)
{
    U8 mu8_connect_state = 0;

    mu8_connect_state = GetWifiStatusValue( WIFI_STATUS_SERVER_CON );

    if( mu8_connect_state == SERVER_CONNECT )
    {
        gu8_Server_Connect_State = SERVER_CONNECT;
    }
    else /*if( mu8_connect_state == SERVER_DISCONNECT )*/
    {
        gu8_Server_Connect_State = SERVER_DISCONNECT;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/




