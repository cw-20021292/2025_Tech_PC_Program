/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _AUTO_DRAIN_H_
#define _AUTO_DRAIN_H_

//
/*#define COLD_DRAIN_TIME    3000*/




#define AUTO_DRAIN_OPERATION_WATER       500
#define AUTO_DRAIN_OPERATION_TIMER       1600

//extern bit F_HardError;
extern TYPE_BYTE          U8DrainWaterLevelB;
#define            u8DrainWaterLevel                       U8DrainWaterLevelB.byte
#define            Bit0_Drain_Water_Empty                U8DrainWaterLevelB.Bit.b0
#define            Bit1_Drain_Water_Low                 U8DrainWaterLevelB.Bit.b1
#define            Bit2_Drain_Water_High                U8DrainWaterLevelB.Bit.b2
#define            Bit3_Drain_Water_Error                U8DrainWaterLevelB.Bit.b3

extern TYPE_BYTE          U8WaterOutStateB;
#define            u8WaterOutState                           U8WaterOutStateB.byte
#define            Bit0_Pure_Water_Select_State                U8WaterOutStateB.Bit.b0
#define            Bit1_Cold_Water_Select_State                 U8WaterOutStateB.Bit.b1
#define            Bit2_Hot_Water_Select_State                U8WaterOutStateB.Bit.b2
#define            Bit3_Ice_Water_Select_State                U8WaterOutStateB.Bit.b3


extern TYPE_BYTE          U8DrainPumpONB;
#define            u8DrainPumpON                        U8DrainPumpONB.byte
#define            Bit0_Drain_Tank_Level_Full_State     U8DrainPumpONB.Bit.b0
#define            Bit1_Drain_Error_Check_On_State      U8DrainPumpONB.Bit.b1
#define            Bit2_Drain_Melt_Ice_State            U8DrainPumpONB.Bit.b2
#define            Bit3_Drain_Tank_Flushing_State       U8DrainPumpONB.Bit.b3
#define            Bit4_Auto_drain_State                U8DrainPumpONB.Bit.b4
#define            Bit5_Manual_drain_State              U8DrainPumpONB.Bit.b5
#define            Bit6_Acid_Clean_State                U8DrainPumpONB.Bit.b6


extern TYPE_BYTE       U8OperationB;
#define         u8Operation                                       U8OperationB.byte
#define         Bit0_Cold_Operation_Disable_State                 U8OperationB.Bit.b0
#define         Bit1_Hot_Operation_Disable_State                  U8OperationB.Bit.b1
#define         Bit2_Ice_Operation_Disable_State                  U8OperationB.Bit.b2
#define         Bit3_Ice_Tank_Ster_Operation_Disable_State        U8OperationB.Bit.b3


//extern U8 gu8_room_level;
extern U16 gu16ADCds;

extern bit F_WaterOut;
extern U16 u16Efluent_Time;

extern bit F_Circul_Drain;

extern bit F_auto_drain_mode_cold_water_valve_out;


extern U16 gu16_AD_Drain_Pump_Current;
extern U8 u8Extract_Continue;
extern CUP_LEVEL gu8Cup_level;
extern bit F_LineTest;
extern bit bit_filter_all;
extern bit bit_self_test_start;
extern U8 gu8_Room_Water_Level;


#endif
