/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _TIME_SETTING_H_
#define _TIME_SETTING_H_

//

extern void WriteRtcTime(void);
extern void BuzStep(U8 mu8Step);
extern void play_melody_select_196( void );
extern void play_melody_warning_197( void );
extern void play_melody_setting_on_198( void );
extern void play_melody_setting_off_199( void );
extern void play_voice_time_setting_finish_145( void );
extern void play_voice_time_setting_start_144(void);




extern U8 gu8_rtc_time_Y2K;       // 19 or 20
extern U8 gu8_rtc_time_DayWeek;   // 0 ~ 6
extern U8 gu8_rtc_time_Year;      // 0 ~ 99
extern U8 gu8_rtc_time_Month;     // 1 ~ 12
extern U8 gu8_rtc_time_Date;      // 1 ~ 31
extern U8 gu8_rtc_time_Hour;      // 0 ~ 23
extern U8 gu8_rtc_time_Min;       // 0 ~ 59
extern U8 gu8_rtc_time_Sec;       // 0 ~ 59

extern bit bit_first_time_setting;
extern bit bit_filter_all;

extern U8 gu8_wifi_period_ster_hour;
extern U8 gu8_wifi_period_ster_minute;
extern U8 gu8_Wifi_Connect_State;

extern U16 gu16_Time_Setting_Mode_Timer_100ms;

extern U8 gu8_direction_continue_timer;
extern U8 gu8_direction_continue_max;
extern U8 gu8_hour_setting_blink_timer;
extern bit bit_hour_blink_500ms;
extern U8 gu8_min_setting_blink_timer;
extern bit bit_min_blink_500ms;

#endif
