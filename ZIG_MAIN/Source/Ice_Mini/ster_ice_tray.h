/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _STER_ICE_TRAY_H_
#define _STER_ICE_TRAY_H_

//
extern U8 prepare_ster(void);
extern U8 hot_preheat_ster(void);
extern U8 hot_input_ster(void);
extern U8 hot_wait_ster(void);
extern U8 wait_finish_clean(void);
extern void play_melody_warning_197( void );
extern void down_tray_motor(void);
extern void up_tray_motor(void);
extern void Play_Voice(U16 mu16MemoryAddress);
extern U8 drain_preheat_ster(void);
extern U8 drain_input_ster(void);
extern U8 drain_wait_ster(void);


extern TYPE_BYTE       U8OperationB;
#define         u8Operation                                       U8OperationB.byte
#define         Bit0_Cold_Operation_Disable_State                 U8OperationB.Bit.b0
#define         Bit1_Hot_Operation_Disable_State                  U8OperationB.Bit.b1
#define         Bit2_Ice_Operation_Disable_State                  U8OperationB.Bit.b2
#define         Bit3_Ice_Tank_Ster_Operation_Disable_State        U8OperationB.Bit.b3


extern U8 gu8_prepare_ster_step;
extern U16 gu16_prepare_ster_timer;
extern U16 gu16_prepare_ster_max_timer;

extern U8 gu8_preheat_ster_step;
extern U16 gu16_preheat_ster_timer;
extern U32 gu32_preheat_ster_max_timer;

extern U8 gu8_hot_input_ster_step;
extern U16 gu16_hot_input_ster_timer;
extern U32 gu32_hot_input_ster_max_timer;

extern U8 gu8_hot_wait_step;
extern U16 gu16_hot_wait_timer;
extern U32 gu32_hot_wait_max_timer;

extern U8 gu8_wait_finish_step;
extern U16 gu16_wait_finish_timer;
extern U16 gu16_wait_finish_max_timer;


extern bit bit_first_time_setting;
extern U8 gu8_rtc_time_Hour;
extern U8 gu8_rtc_time_Min;
extern U8 gu8_rtc_time_Sec;
extern bit F_Tank_Cover_Input;
extern bit bit_filter_all;
extern bit bit_self_test_start;
extern U8 gu8_ice_system_ok;
extern U8 gu8IceTrayLEV;
extern bit F_IR;
extern bit F_Low_IR;
extern bit F_IceDoorClose;
extern U8 gu8_tray_clean_percent;

extern ICE_STEP gu8IceStep;
extern bit F_First_Hot_Effluent;
extern bit bit_manual_drain_start;
extern special_func cody_water_line;

extern U8 gu8_drain_preheat_ster_step;
extern U16 gu16_drain_preheat_ster_timer;
extern U32 gu32_drain_preheat_ster_max_timer;

extern U8 gu8_drain_input_ster_step;
extern U16 gu16_drain_input_ster_timer;
extern U32 gu32_drain_input_ster_max_timer;

extern U8 gu8_drain_wait_step;
extern U16 gu16_drain_wait_timer;
extern U32 gu32_drain_wait_max_timer;

extern bit F_WaterOut;
extern bit F_IceOut;

extern FLUSHING_STEP gu8_flushing_mode;
extern bit bit_install_flushing_state;
extern bit F_IceSelectDoorClose;
#endif
