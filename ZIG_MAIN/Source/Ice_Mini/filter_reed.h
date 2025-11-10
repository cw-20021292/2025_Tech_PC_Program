/***********************************************************************************************************************
 * Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _FILTER_REED_H_
#define _FILTER_REED_H_

//
extern void BuzStep(U8 mu8Step);
extern void send_wifi_system_function(void);
extern void power_saving_init(void);
extern void play_melody_warning_197( void );
extern void play_melody_setting_on_198( void );
extern void play_melody_setting_off_199( void );
extern void play_voice_filter_cover_open_3( void );
extern void play_voice_filter_reed_sw_open_4( void );
extern void play_voice_filter_flushing_cancle_25( void );
extern void play_voice_1_3_filter_change_detect_18( void );
extern void play_voice_1_3_filter_change_finish_19( void );
extern void wifi_neo_ino_filter_alarm_start(void);
extern void wifi_neo_ino_filter_alarm_stop(void);
extern void wifi_neo_ro_ino_filter_alarm_start(void);
extern void wifi_neo_ro_ino_filter_alarm_stop(void);
extern void finish_ct_forced_flushing(void);
extern void service_reed_sw_input(void);
extern void uv_tank_reed_sw_input(void);
extern void Play_Voice(U16 mu16MemoryAddress);
extern void play_voice_filter_not_detected_14(void);
extern void ice_extraction_finish(void);

extern TYPE_BYTE          U8CTFilterReplaceB;
#define            gu8CTFilterReplace                           U8CTFilterReplaceB.byte
#define            Bit0_CT_Neo_Filter_1_Replace                 U8CTFilterReplaceB.Bit.b0
#define            Bit1_CT_Ro_Filter_2_Replace                  U8CTFilterReplaceB.Bit.b1
#define            Bit2_CT_Ino_Filter_3_Replace                 U8CTFilterReplaceB.Bit.b2




#define FILTER_COVER_REED_DETECT_TIME         20
#define FILTER_REED_DETECT_TIME               10




extern U8 gu8_filter_flushing_state;
extern bit F_FW_Version_Display_Mode;
extern FLUSHING_STEP gu8_flushing_mode;
//extern bit bit_filter_flushing_no_cancel;
extern U16 gu16_wifi_filter_remain_day;
extern bit bit_install_flushing_state;
extern U16 gu16_water_usage_neo_filter;
extern U16 gu16_water_usage_ro_filter;
extern U16 gu16_water_usage_ino_filter;
extern U16 gu16_temporary_save_usage;
extern bit bit_self_test_start;
extern U8 gu8_display_flushing_total_percent;


extern bit bit_wifi_neo_filter_1_alarm;
extern bit bit_wifi_ro_filter_2_alarm;
extern bit bit_wifi_ino_filter_3_alarm;

extern U16 gu16_define_filter_period__neo;
extern U16 gu16_define_filter_period__ro;
extern U16 gu16_define_filter_period__ino;
extern bit bit_filter_cycle_change;
extern bit bit_ct_flushing_standby_start;
extern U8 gu8_ct_forced_flushing_start;
extern bit F_WaterOut;
extern bit F_IceOut;
extern FLUSHING_STEP gu8_flushing_mode_saved;
extern bit bit_flushing_halt;
extern bit F_All_Lock;
extern bit F_IceOutCCW;
extern bit F_IceBreak_Motor_Out;
extern DISPLAY_STACK Extract_Stack;
extern U8  u8Extract_Continue;
#endif
