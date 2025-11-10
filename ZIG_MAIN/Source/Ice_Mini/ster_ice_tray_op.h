/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _STER_ICE_TRAY_OP_H_
#define _STER_ICE_TRAY_OP_H_

//
extern void down_tray_motor(void);
extern void up_tray_motor(void);
extern void finish_ice_ster(void);



extern TYPE_BYTE          U8DrainWaterLevelB;
#define            u8DrainWaterLevel                       U8DrainWaterLevelB.byte
#define            Bit0_Drain_Water_Empty                U8DrainWaterLevelB.Bit.b0
#define            Bit1_Drain_Water_Low                 U8DrainWaterLevelB.Bit.b1
#define            Bit2_Drain_Water_High                U8DrainWaterLevelB.Bit.b2
#define            Bit3_Drain_Water_Error                U8DrainWaterLevelB.Bit.b3



extern U8 gu8_cody_take_off_ice_start;
extern ICE_STEP gu8IceStep;
extern U8 gu8_ice_system_ok;
extern bit F_TrayMotorDOWN;
extern bit F_TrayMotorUP;
extern U8 gu8IceTrayLEV;
extern U8 gu8_Flow_1sec;
extern U8 gu8_Hot_Heater_Temperature_One_Degree;
extern U16 gu16_Tray_Ster_Hz;













#endif
