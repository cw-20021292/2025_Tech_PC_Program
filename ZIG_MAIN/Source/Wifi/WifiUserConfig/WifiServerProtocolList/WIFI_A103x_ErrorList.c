
/*******************************************************************************
&Description :
    A1030 ~ A103x
    Error Protocol Table

&History : ssg@coway.com 2024.10.04 update

*******************************************************************************/

#include "WIFI_A103x_ErrorList.h"
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
/* Event List */
U16 WifiErrorPreVal[USER_ERROR_DATA_MAX_SIZE] = {0};      // Error Protocol count

static const WifiTxFuncEventList_T WifiErrorProtocolList[] =
{   /*  API ID                                   DATA_TYPE                PreVal(Initial)                         EnventForm     */
    //{   WIFI_ERROR_0001_LOW_LEVEL,              TYPE_UNUSED,           (U16*)(&WifiErrorPreVal),              EVENT_STATUS_CHANGE },
    // {   WIFI_ERROR_0002_HIGH_LEVEL,             TYPE_UNUSED,           (U16*)(&WifiErrorPreVal),              EVENT_STATUS_CHANGE },
    // {   WIFI_ERROR_0003_COMPLEX_LEVEL,          TYPE_UNUSED,           (U16*)(&WifiErrorPreVal),              EVENT_STATUS_CHANGE },
    // {   WIFI_ERROR_0004_FEED_VALVE,             TYPE_UNUSED,           (U16*)(&WifiErrorPreVal),              EVENT_STATUS_CHANGE },
    {   WIFI_ERROR_0008_COLD_TEMP,              TYPE_UNUSED,           (U16*)(&WifiErrorPreVal),              EVENT_STATUS_CHANGE },
    {   WIFI_ERROR_0009_NATURAL_TEMP,           TYPE_UNUSED,           (U16*)(&WifiErrorPreVal),              EVENT_STATUS_CHANGE },
    {   WIFI_ERROR_000C_BLDC_COMP,              TYPE_UNUSED,           (U16*)(&WifiErrorPreVal),              EVENT_STATUS_CHANGE }, 
    {   WIFI_ERROR_000E_DRAIN_PUMP,             TYPE_UNUSED,           (U16*)(&WifiErrorPreVal),              EVENT_STATUS_CHANGE },
    {   WIFI_ERROR_0010_HOTDRAIN_CLOSE,         TYPE_UNUSED,           (U16*)(&WifiErrorPreVal),              EVENT_STATUS_CHANGE },     
    {   WIFI_ERROR_0019_STOP_WATER     ,        TYPE_UNUSED,           (U16*)(&WifiErrorPreVal),              EVENT_STATUS_CHANGE },     
    {   WIFI_ERROR_001B_STOP_HOTWATER    ,      TYPE_UNUSED,           (U16*)(&WifiErrorPreVal),              EVENT_STATUS_CHANGE },
    {   WIFI_ERROR_001D_LEAK_SENSOR,            TYPE_UNUSED,           (U16*)(&WifiErrorPreVal),              EVENT_STATUS_CHANGE },
    {   WIFI_ERROR_0020_ROOM_TEMP,              TYPE_UNUSED,           (U16*)(&WifiErrorPreVal),              EVENT_STATUS_CHANGE },
    {   WIFI_ERROR_0021_MICRO_SW_DETECT,        TYPE_UNUSED,           (U16*)(&WifiErrorPreVal),              EVENT_STATUS_CHANGE },
    {   WIFI_ERROR_0022_MICRO_SW_MOVE,          TYPE_UNUSED,           (U16*)(&WifiErrorPreVal),              EVENT_STATUS_CHANGE },
    {   WIFI_ERROR_0035_UV_ICE_TANK1,           TYPE_UNUSED,           (U16*)(&WifiErrorPreVal),              EVENT_STATUS_CHANGE },
    {   WIFI_ERROR_0037_UV_ICE_TRAY,            TYPE_UNUSED,           (U16*)(&WifiErrorPreVal),              EVENT_STATUS_CHANGE },
    {   WIFI_ERROR_0038_UV_EXTRACT,            TYPE_UNUSED,           (U16*)(&WifiErrorPreVal),              EVENT_STATUS_CHANGE },
    {   WIFI_ERROR_0039_UV_ICE_EXTRACT,        TYPE_UNUSED,           (U16*)(&WifiErrorPreVal),              EVENT_STATUS_CHANGE },

   // {   WIFI_ERROR_0026_HOT_TANK_TEMP,          TYPE_UNUSED,           (U16*)(&WifiErrorPreVal),              EVENT_STATUS_CHANGE },
    {   WIFI_ERROR_003B_MICRO_SW_MOVE2,         TYPE_UNUSED,           (U16*)(&WifiErrorPreVal),              EVENT_STATUS_CHANGE },
    {   WIFI_ERROR_003F_AMB_SIDE,               TYPE_UNUSED,           (U16*)(&WifiErrorPreVal),              EVENT_STATUS_CHANGE },
   // {   WIFI_ERROR_003C_ROOM_TEMP_OH,           TYPE_UNUSED,           (U16*)(&WifiErrorPreVal),              EVENT_STATUS_CHANGE },
   {   WIFI_ERROR_0046_DRAIN_BLOCK,             TYPE_UNUSED,           (U16*)(&WifiErrorPreVal),              EVENT_STATUS_CHANGE },
   {   WIFI_ERROR_0049_INVERTOR_ERROR1,         TYPE_UNUSED,           (U16*)(&WifiErrorPreVal),              EVENT_STATUS_CHANGE },
   {   WIFI_ERROR_004A_INVERTOR_ERROR2,         TYPE_UNUSED,           (U16*)(&WifiErrorPreVal),              EVENT_STATUS_CHANGE },
   {   WIFI_ERROR_004B_INVERTOR_ERROR3,         TYPE_UNUSED,           (U16*)(&WifiErrorPreVal),              EVENT_STATUS_CHANGE },
   {   WIFI_ERROR_004C_INVERTOR_ERROR4,         TYPE_UNUSED,           (U16*)(&WifiErrorPreVal),              EVENT_STATUS_CHANGE },
   {   WIFI_ERROR_004D_INVERTOR_ERROR5,         TYPE_UNUSED,           (U16*)(&WifiErrorPreVal),              EVENT_STATUS_CHANGE },
   {   WIFI_ERROR_004E_INVERTOR_ERROR6,         TYPE_UNUSED,           (U16*)(&WifiErrorPreVal),              EVENT_STATUS_CHANGE },
   
};
#define SZ_ERR_EVENT_LIST    (sizeof(WifiErrorProtocolList)/sizeof(WifiTxFuncEventList_T))

void* GetWifiErrDataEventList ( void )
{
    if (IsUserModelType() == Model_0)
    {
        return WifiErrorProtocolList;
    }
    else
    {
        /* 2KG는 CPI 없음 */
    }
}

U16 GetWifiErrEventListSize ( void )
{
    if (IsUserModelType() == Model_0)
    {
        return SZ_ERR_EVENT_LIST;
    }
    else
    {
        /* 2KG는 CPI 없음 */
    }

}


/* [User Error Data] Product -> Server */
U16 GetUserSystemError ( U16 mu16Err )
{
    U16 mu16Data = 0U;

    switch ( mu16Err ) // 0:Normal / 1:Error
    {       
        case WIFI_ERROR_0008_COLD_TEMP:
            mu16Data = Bit14_Cold_Temp_Open_Short_Error__E44;
        break;
        case WIFI_ERROR_0009_NATURAL_TEMP:
            mu16Data = Bit15_Amb_Temp_Open_Short_Error__E43;
        break; 

        case WIFI_ERROR_000C_BLDC_COMP:
            mu16Data = Bit7_BLDC_Communication_Error__E27;
        break;
        case WIFI_ERROR_000E_DRAIN_PUMP:
            mu16Data = Bit16_Drain_Pump_Error__E60;
        break;
        case WIFI_ERROR_0010_HOTDRAIN_CLOSE:
            mu16Data = Bit0_Hot_Water_Flow_Block_Error__E08;
        break;
        case WIFI_ERROR_0019_STOP_WATER:
            mu16Data = Bit6_Main_Water_Flow_Block_Error__E09;
        break;
        case WIFI_ERROR_001B_STOP_HOTWATER:
            mu16Data = Bit0_Hot_Water_Flow_Block_Error__E08;
        break;
        case WIFI_ERROR_001D_LEAK_SENSOR:
            mu16Data = Bit3_Leakage_Sensor_Error__E01;
        break;
        case WIFI_ERROR_0020_ROOM_TEMP:
            mu16Data = Bit2_Room_Temp_Open_Short_Error__E42;
        break;
        case WIFI_ERROR_0021_MICRO_SW_DETECT:
            mu16Data = Bit17_Tray_Micro_SW_Dual_Detect_Error__E61;
        break;
        case WIFI_ERROR_0022_MICRO_SW_MOVE:
            mu16Data = Bit18_Tray_Micro_SW_Up_Move_Error__E62;
        break;
        case WIFI_ERROR_0035_UV_ICE_TANK1:
            mu16Data = (Bit27_Ice_Tank_3_UV_Error__E75 | Bit24_Ice_Tank_1_2_UV_Error__E75);
        break;
        case WIFI_ERROR_0037_UV_ICE_TRAY:
            mu16Data = Bit28_Ice_Tray_1_2_UV_Error__E76;
        break;
        case WIFI_ERROR_0038_UV_EXTRACT:        /* LPP 추가 250610 CH.PARK */
            mu16Data = Bit23_faucet_UV_Error__E77;
        break;
        case WIFI_ERROR_0039_UV_ICE_EXTRACT:    /* LPP 추가 250610 CH.PARK */
            mu16Data = (Bit25_Ice_Faucet_UV_1_Error__E78 | Bit26_Ice_Faucet_UV_2_Error__E78);
        break;
        case WIFI_ERROR_003B_MICRO_SW_MOVE2:
            mu16Data = Bit19_Tray_Micro_SW_Down_Move_Error__E63;
        break;
        case WIFI_ERROR_003F_AMB_SIDE:          /* LPP 추가 250610 CH.PARK */
            mu16Data = Bit21_Amb_Side_Temp_Open_Short_Error__E53;
        break;
        case WIFI_ERROR_0049_INVERTOR_ERROR1:
            mu16Data = Bit8_BLDC_Operation_Error_Current_Sensing__E81;
        break;
        case WIFI_ERROR_004A_INVERTOR_ERROR2:
            mu16Data = Bit9_BLDC_Operation_Error_Starting_Fail__E82;
        break;
        case WIFI_ERROR_004B_INVERTOR_ERROR3:
            mu16Data = Bit10_BLDC_Operation_Error_Over_Current__E83;
        break;
        case WIFI_ERROR_004C_INVERTOR_ERROR4:
            mu16Data = Bit11_BLDC_Operation_Error_Overheat__E84;
        break;
        case WIFI_ERROR_004D_INVERTOR_ERROR5:
            mu16Data = Bit12_BLDC_Operation_Error_Disconnection_Comm__E85;
        break;
        case WIFI_ERROR_004E_INVERTOR_ERROR6:
            mu16Data = Bit13_BLDC_Operation_Error_Abnormal_Voltage__E86;
        break;
        case WIFI_ERROR_0046_DRAIN_BLOCK:
            mu16Data = Bit0_Hot_Water_Flow_Block_Error__E08;
        break;

        default:
            mu16Data = 0U;
        break;
    }
    return mu16Data;
}