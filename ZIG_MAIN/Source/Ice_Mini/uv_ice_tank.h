/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _UV_ICE_TANK_H_
#define _UV_ICE_TANK_H_

extern TYPE_BYTE       U8OperationB;
#define         u8Operation                                       U8OperationB.byte
#define         Bit0_Cold_Operation_Disable_State                 U8OperationB.Bit.b0
#define         Bit1_Hot_Operation_Disable_State                  U8OperationB.Bit.b1
#define         Bit2_Ice_Operation_Disable_State                  U8OperationB.Bit.b2
#define         Bit3_Ice_Tank_Ster_Operation_Disable_State        U8OperationB.Bit.b3


#define UV_MAX_OPERATION_UV_AUTO_OFF        1
#define UV_MAX_OPERATION_UV_AUTO_ON         3
#define UV_TANK_COVER_REED_DETECT_TIME      3

extern bit F_IceOn;
extern bit F_IceOut;

extern bit bit_ice_tank_ster_start;
extern ICE_STER_MODE gu8_ice_ster_mode;
extern U16 gu16IceMakeTime;
extern U16 gu16_uv_ice_make_time;
extern U8 gu8_ice_off_uv_operation_timer_min;
extern bit bit_ice_tray_making_enable;

#endif
