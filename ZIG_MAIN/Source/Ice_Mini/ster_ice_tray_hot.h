/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _STER_ICE_TRAY_HOT_H_
#define _STER_ICE_TRAY_HOT_H_



//

extern void run_decrease_flow( U16 mu16_move );
extern void run_increase_flow( U16 mu16_move );
extern void Cal_HeaterWatt(void);



typedef enum
{
    FLOW_KEEP_STATE,
    FLOW_INCREASE_STATE,
    FLOW_DECREASE_STATE
}FLOW_STATE;

typedef enum
{
    HOT_KEEP_STATE,
    HOT_INCREASE_STATE,
    HOT_DECREASE_STATE
}HOT_STATE;




/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
extern TYPE_BYTE       U8OperationB;
#define         u8Operation                                       U8OperationB.byte
#define         Bit0_Cold_Operation_Disable_State                 U8OperationB.Bit.b0
#define         Bit1_Hot_Operation_Disable_State                  U8OperationB.Bit.b1
#define         Bit2_Ice_Operation_Disable_State                  U8OperationB.Bit.b2
#define         Bit3_Ice_Tank_Ster_Operation_Disable_State        U8OperationB.Bit.b3

extern TYPE_WORD          U16WaterHeaterOnW;
#define            u16WaterHeaterOn                          U16WaterHeaterOnW.word
#define            u16WaterHeater_L                          U16WaterHeaterOnW.byte[0]
#define            u16WaterHeater_H                          U16WaterHeaterOnW.byte[1]
#define            Bit0_Pre_Heating_State                    U16WaterHeaterOnW.Bit.b0
#define            Bit1_Main_Heating_State                   U16WaterHeaterOnW.Bit.b1
#define            Bit2_First_Heating_Check_State            U16WaterHeaterOnW.Bit.b2
#define            Bit3_Tray_Ster_Heating_State              U16WaterHeaterOnW.Bit.b3



extern bit F_WaterOut;
extern bit bit_ice_tank_ster_start;
extern ICE_STER_MODE gu8_ice_ster_mode;
extern U8 gu8_preheat_ster_step;
extern U8 gu8_hot_input_ster_step;
extern U8 gu8_Tray_Ster_Target_Temp;
extern U8 gu8_Hot_Out_Temperature_One_Degree;
extern U8 gu8_Flow_1sec;

extern bit Bit_Relay_Output;
extern U8 gu8TriacLevelMax;
extern U8 gu8_Hot_Heater_Temperature_One_Degree;

extern bit F_First_Hot_Effluent;
extern U8 gu8_drain_max_flow;

extern U8 gu8_Hot_In_Temperature_One_Degree;
extern U8 gu8_Flow_1sec;
extern U16 gu16_AD_Result_Heater_Current_Feed;
extern FLOW_VALVE_MODE  gu8_hot_flow_mode;
extern U16 gu16_AD_Result_Heater_Current_Feed_Max;
extern U8 u8Target_Hz_Hot;
extern U16 Target_Flow_Q;
extern U16 u16Heater_Power_Save;

extern U8 gu8_drain_preheat_ster_step;
extern U8 gu8_drain_input_ster_step;







/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////



#endif
