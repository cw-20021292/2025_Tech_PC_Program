/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _MOTOR_ICE_SELECT_OUTPUT_H_
#define _MOTOR_ICE_SELECT_OUTPUT_H_


#define DEFAULT_ICE_DOOR_CLOSE_TIME     86400

void IceStuckInit(void);

extern U8 finish_ice_setting(void);

extern U8 gu8IceClose;
extern bit F_IceOpen;

extern U8 gu8ErrDoor;
extern bit F_IceOut;
extern bit F_LineTest;
extern bit F_IceBreak;

extern U8 gu8IceInnerClose;
#endif
