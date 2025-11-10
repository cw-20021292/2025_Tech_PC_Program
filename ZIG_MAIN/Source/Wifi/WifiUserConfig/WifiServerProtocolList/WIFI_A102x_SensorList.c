
/*******************************************************************************
&Description :
    A1020 ~ A102x
    Sensor Table

&History : ssg@coway.com 2024.10.04 update

*******************************************************************************/


#include "WIFI_A102x_SensorList.h"

/******************************************************************************/
/***** User Header ************************************************************/
/******************************************************************************/
#include "m_wifi_global_data.h"
/******************************************************************************/
/***** User Function **********************************************************/
/******************************************************************************/

/******************************************************************************/
/***** Table ******************************************************************/
/******************************************************************************/
/* Protocol List */
static const WifiNormalList_T WifiSensorProtocolList[] =
{   /*  API ID                          Data Type     */
    {   WIFI_SENSOR_0001_COLD_TEMP,    TYPE_SIGNED      },
    // {   WIFI_SENSOR_0002_HOT_TEMP,     TYPE_SIGNED      },
    {   WIFI_SENSOR_0003_WATER_TEMP,   TYPE_SIGNED      },
    {   WIFI_SENSOR_0004_OUT_TEMP,     TYPE_SIGNED      },
    {   WIFI_SENSOR_000E_WATT_DATA,    TYPE_SENSOR_WATT },
    {   WIFI_SENSOR_000F_COMP_DATA,    TYPE_UNSIGNED    },
    {   WIFI_SENSOR_0015_OUT2_TEMP,    TYPE_SIGNED      },      // LPP 추가
    // {   WIFI_SENSOR_0014_HEATER_DATA,  TYPE_UNSIGNED    },
};

#define SZ_SENSOR_EVENT_LIST    (sizeof(WifiSensorProtocolList)/sizeof(WifiNormalList_T))

void* GetWifiSensorDataEventList ( void )
{
    if (IsUserModelType() == Model_0)
    {
        return WifiSensorProtocolList;
    }
    else
    {

    }
}

U16 GetWifiSensorEventListSize ( void )
{
    if (IsUserModelType() == Model_0)
    {
        return SZ_SENSOR_EVENT_LIST;
    }
    else
    {

    }
}

/* [User Sensor Data] Product -> Server */
I16 GetUserSystemSensor ( U16 mu16Sen )
{
    I16 mi16Data = 0;

    switch ( mu16Sen )
    {
        case WIFI_SENSOR_0001_COLD_TEMP:
        mi16Data = gu8_Cold_Temperature_One_Degree;

        if ( mi16Data >= 99 )
        {
            mi16Data = 99;
        }
        break;

        case WIFI_SENSOR_0002_HOT_TEMP:
            /* 2kg 미적용 */
        break;

        case WIFI_SENSOR_0003_WATER_TEMP:
		mi16Data = gu8_Hot_In_Temperature_One_Degree;

        if ( mi16Data >= 99 )
        {
            mi16Data = 99;
        }
        else{}
        break;
        
        case WIFI_SENSOR_0004_OUT_TEMP:
        mi16Data = gu8_Amb_Front_Temperature_One_Degree;

        if ( mi16Data >= 99 )
        {
            mi16Data = 99;
        }
        else{}
        break;

        case WIFI_SENSOR_000E_WATT_DATA:
        if( gu16_wifi_total_watt == 0 )
        {
            mi16Data = 1;
        }
        else
        {
            mi16Data = (gu16_wifi_total_watt / 10);
        }

        init_wifi_elec_watt();
        break;
				
        case WIFI_SENSOR_000F_COMP_DATA:
        if( F_Comp_Output == SET )
        {
           mi16Data = 1;
        }
        else
        {
           mi16Data = 0;
        }
        break;

        case WIFI_SENSOR_0015_OUT2_TEMP:        /* 외기온도센서 2 LPP 추가 250609 CH.PARK */
            mi16Data = gu8_Amb_Side_Temperature_One_Degree;

            if ( mi16Data >= 99 )
            {
                mi16Data = 99;
            }
            else{}
        break;

        default:
            mi16Data = 0;
        break;
    }
    return mi16Data;
}

/* User Data Clear (Called when wifi is not connected) */
void ClearUserSystemDataA1020 ( void )
{

}