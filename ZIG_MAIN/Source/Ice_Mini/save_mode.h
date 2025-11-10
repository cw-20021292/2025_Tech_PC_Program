/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _SAVE_MODE_H_
#define _SAVE_MODE_H_

//


extern void BuzStep(U8 mu8Step);
extern U8 get_final_large_amb_temp(void);
extern void play_melody_select_196( void );
extern void play_melody_warning_197( void );
extern void play_melody_setting_on_198( void );
extern void play_melody_setting_off_199( void );
extern void play_voice_test_mode_151( void );


extern U8 gu8_rtc_time_Y2K;       // 19 or 20
extern U8 gu8_rtc_time_DayWeek;   // 0 ~ 6
extern U8 gu8_rtc_time_Year;      // 0 ~ 99
extern U8 gu8_rtc_time_Month;     // 1 ~ 12
extern U8 gu8_rtc_time_Date;      // 1 ~ 31
extern U8 gu8_rtc_time_Hour;      // 0 ~ 23
extern U8 gu8_rtc_time_Min;       // 0 ~ 59
extern U8 gu8_rtc_time_Sec;       // 0 ~ 59


extern bit bit_sleep_mode_enable;
extern U8 gu8_sleep_mode_start_hour;
extern U8 gu8_sleep_mode_start_minute;
extern U8 gu8_sleep_mode_finish_hour;
extern U8 gu8_sleep_mode_finish_minute;
extern U8 gu8_Amb_Temperature_One_Degree;
extern U8 gu8_wifi_no_use_key;

extern bit F_FW_Version_Display_Mode;
extern LED_STEP gu8_Led_Display_Step;

#define NO_USE_SAVE_MODE_TIME            9
#define DISPLAY_SLEEP_TIME               6000



#endif
