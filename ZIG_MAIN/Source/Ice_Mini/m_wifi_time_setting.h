/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _WIFI_TIME_SETTING_H_
#define _WIFI_TIME_SETTING_H_

//

extern void WriteRtcTime(void);
extern void WifiSendData ( E_WIFI_DATA_T mu8Data );

extern U8 gu8_rtc_time_Year;
extern U8 gu8_rtc_time_Month;
extern U8 gu8_rtc_time_Date;
extern U8 gu8_rtc_time_Hour;
extern U8 gu8_rtc_time_Min;
extern U8 gu8_rtc_time_Sec;
extern U8 gu8_rtc_time_Y2K;
extern U8 gu8_Wifi_Connect_State;
extern bit bit_first_time_setting;





#endif
