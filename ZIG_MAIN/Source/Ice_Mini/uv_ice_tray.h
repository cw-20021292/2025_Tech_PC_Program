/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _UV_ICE_TRAY_H_
#define _UV_ICE_TRAY_H_

extern TYPE_BYTE       U8OperationB;
#define         u8Operation                                       U8OperationB.byte
#define         Bit0_Cold_Operation_Disable_State                 U8OperationB.Bit.b0
#define         Bit1_Hot_Operation_Disable_State                  U8OperationB.Bit.b1
#define         Bit2_Ice_Operation_Disable_State                  U8OperationB.Bit.b2
#define         Bit3_Ice_Tank_Ster_Operation_Disable_State        U8OperationB.Bit.b3

extern void down_tray_motor(void);
extern void up_tray_motor(void);

extern bit F_IceOn;
extern bit bit_uv_tank_input;
extern bit F_IceOut;
extern bit F_IceFull;
extern bit F_IceLack;
extern bit F_IceInit;
extern bit bit_ice_tank_ster_start;
extern U8 gu8IceTrayLEV;
extern U8 gu8IceLEV;
extern Ice_Amount gu8_ice_tank_ice_amount;
extern ICE_STEP gu8IceStep;
extern bit bit_install_flushing_state;
extern bit bit_self_test_start;
extern bit F_TrayMotorDOWN;


#endif
