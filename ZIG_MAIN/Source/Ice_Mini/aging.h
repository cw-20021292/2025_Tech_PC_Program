/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _AGING_H_
#define _AGING_H_

extern void key_cold_water_select_job(void);
extern void water_extract_key(void);
extern void key_ambient_water_select_job(void);
extern void key_hot_water_select_job(void);
extern void ice_extraction_finish(void);
extern void ice_extraction_start(void);
extern void key_ice_select_job(void);
extern void key_ice_water_select_job(void);
extern void ice_extract_long_key(void);
extern void hot_ster_start_job(void);
extern void key_manual_ice_tank_ster(void);

extern TYPE_BYTE          U8FeedValveONB;
#define            u8FeedValveON                         U8FeedValveONB.byte
#define            Bit0_Low_Water_Filling_State          U8FeedValveONB.Bit.b0
#define            Bit1_Feed_Valve_Flushing_State        U8FeedValveONB.Bit.b1
#define            Bit2_Feed_Valve_Water_Flushing_State  U8FeedValveONB.Bit.b2
#define            Bit3_Feed_Valve_Flushing_Tank_State   U8FeedValveONB.Bit.b3


extern CUP_LEVEL gu8Cup_level;
extern U8 gu8_Error_Code;
extern bit F_FW_Version_Display_Mode;
extern U8 gu8_Room_Water_Level;
extern U8 gu8_Cold_Temperature_One_Degree;
extern U8 gu8_Hot_Tank_Temperature_One_Degree;
extern bit F_WaterOut;
extern U8 gu8_ice_out_continue;
extern FLUSHING_STEP gu8_flushing_mode;
extern U16 gu16_ice_out_timer;
extern ICE_OUT_STEP gu8_Ice_Out_Step;
extern bit F_IceOut;
extern bit F_IceBreak;
extern bit F_IceFull;
extern HOT_STER_STEP gu8_hot_ster_step;
extern ICE_LEVEL  gu8_ice_amount_step;
extern bit bit_ice_tank_ster_start;
extern bit F_Effluent_OK;
extern U16 gu16Effluent_Hz;
#define AGING_TEST_MODE_DELAY_TIME_10MIN    6000
#define AGING_TEST_MODE_DELAY_TIME_2HOUR    72000
#define AGING_TEST_MODE_DELAY_TIME_30MIN    18000


#endif
