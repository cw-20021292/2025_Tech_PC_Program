/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _WATER_LEVEL_H_
#define _WATER_LEVEL_H_


extern void send_wifi_system_function(void);


extern bit F_ErrTrayMotor_DualInital;
//extern bit F_WaterInitSet;
//extern bit F_Overflow;
//extern bit F_ErrTrayMotor_Dual;
extern bit F_Feed;
//extern bit F_ColdVV;
//extern bit F_ColdWaterInit;




extern U8 gu8IceCount;
extern bit F_WaterInit;
//extern bit F_ErrPurify;
//extern bit F_ErrRoomLowLEV;
//extern bit F_ErrRoomLowLEV_D;
//extern U8 gu8DrainErrorRetrayDelay;
//extern bit F_IceWaterExt;
//extern bit F_ErrDrainSet;

extern bit F_Ext_cold;
extern bit F_Drain_Pump_Output;



/*#define DRAIN_WATER_LEVEL_DETECT_TIME    30*/
#define DRAIN_WATER_LEVEL_DETECT_TIME    20



#define ROOM_WATER_LEVEL_DETECT_TIME     20

#define ROOM_HIGH_LEVEL_ON_DETECT_TIME   300


#define DRAIN_LEVEL_POWER_ON_TIME           50
#define DRAIN_LEVEL_POWER_OFF_TIME          60


#endif
