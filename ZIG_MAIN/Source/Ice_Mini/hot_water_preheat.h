/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _HOT_WATER_PREHEAT_H_
#define _HOT_WATER_PREHEAT_H_

//
extern void Play_Voice(U16 mu16MemoryAddress);

//#define PRE_HEAT_MAX_TIME_SHORT    59
#define PRE_HEAT_MAX_TIME_SHORT    65

//#define PRE_HEAT_MAX_TIME_LONG     69
// #define PRE_HEAT_MAX_TIME_LONG     69

#define PRE_HEAT_MAX_TIME_LONG     80					// ¢®U2025-06-02 Phil
#define PRE_HEAT_MAX_TIME_FIRST_LOW     65					// ¡Ë¢çU2025-06-02 Phil

//#define PRE_HEAT_MAX_TIME_SHORT    49
//#define PRE_HEAT_MAX_TIME_LONG     59



//#define PRE_HEAT_MAX_TIME_SHORT    67
//#define PRE_HEAT_MAX_TIME_LONG     77

extern U16 u16Heater_Power_Save_Org;

extern U8 gu8_Flow_1sec;
extern U8 gu8_Hot_Out_Temperature_One_Degree;
extern U8 gu8_Hot_Preheat_Temp;
extern U8 gu8_Hot_In_Temperature_One_Degree;
extern U8 gu8_hot_setting_temperature;
extern U16 u16Heater_Power_Save;
extern U8 gu8_Hot_Heater_Temperature_One_Degree;
extern FLOW_VALVE_MODE  gu8_hot_flow_mode;
extern U8 gu8_hot_filling_max_hz;

extern U16 gu16_moving_pulse;
extern bit bit_Holding_Flow_Block_Error__E08;   // ¡Ú2025-06-02 Phil
extern bit F_firstEffluent_hotWaterHeat;        // ¢®U 2025-06-09 Phil  
extern U8 u8Target_Hz_Hot;
#endif
