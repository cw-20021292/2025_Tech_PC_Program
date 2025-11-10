/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _SELF_TEST_AUTO_H_
#define _SELF_TEST_AUTO_H_

//
extern void send_wifi_self_test_function(void);
extern U16 GetWifiRequestValue ( E_WIFI_RQST_T mType );
extern void initial_each_self_data( U8 mu8_num );

extern TYPE_SELF_TEST_DATA             SELF_Test_Result_Data;
#define U16_VALVE_NOS_0001                        SELF_Test_Result_Data.word[DC_LOAD__NOS_VALVE]
#define U16_VALVE_HOT_OUT_0002                    SELF_Test_Result_Data.word[DC_LOAD__HOT_OUT_VALVE]
#define U16_VALVE_COLD_PURE_OUT_0005              SELF_Test_Result_Data.word[DC_LOAD__COLD_PURE_OUT_VALVE]
#define U16_VALVE_PURE_IN_0007                    SELF_Test_Result_Data.word[DC_LOAD__PURE_IN_VALVE]
#define U16_VALVE_HOT_IN_0009                     SELF_Test_Result_Data.word[DC_LOAD__HOT_IN_VALVE]
#define U16_VALVE_HOT_DRAIN_000A                  SELF_Test_Result_Data.word[DC_LOAD__HOT_DRAIN_VALVE]
#define U16_VALVE_HOT_AIR_0008                    SELF_Test_Result_Data.word[DC_LOAD__OVERFLOW_VALVE]
#define U16_VALVE_COLD_IN_000C                    SELF_Test_Result_Data.word[DC_LOAD__COLD_IN_VALVE]
#define U16_VALVE_COLD_DRAIN_000D                 SELF_Test_Result_Data.word[DC_LOAD__COLD_DRAIN_VALVE]
#define U16_VALVE_ICETRAY_IN_0015                 SELF_Test_Result_Data.word[DC_LOAD__ICETRAY_IN_VALVE]
#define U16_ICE_WATER_VALVE_0023                  SELF_Test_Result_Data.word[DC_LOAD__ICE_WATER_EXTRACT_VALVE]        // 2KG MAX

#define U16_COLD_BLDC_COMP_0201                   SELF_Test_Result_Data.word[DC_LOAD__BLDC_COMP]
#define U16_COLD_FAN_0204                         SELF_Test_Result_Data.word[DC_LOAD__COLD_FAN]

#define U16_HEATER_TRIAC_0302                     SELF_Test_Result_Data.word[AC_LOAD_HOT_TRIAC]
#define U16_HEATER_RELAY_0303                     SELF_Test_Result_Data.word[AC_LOAD_HOT_RELAY]

#define U16_HOT_FLOW_VALVE_0304                   SELF_Test_Result_Data.word[DC_LOAD__FLOWVALVE_MOTOR]
#define U16_FLOW_BLOCK_ERROR_E08_030A             SELF_Test_Result_Data.word[DC_LOAD__FLOW_SENSOR]

#define U16_UV_FAUCET_0501                        SELF_Test_Result_Data.word[DC_LOAD__UV_FAUCET]
#define U16_UV_ICE_FAUCET_0502                    SELF_Test_Result_Data.word[DC_LOAD__UV_ICE_FAUCET1]
#define U16_UV_ICE_FAUCET_2_                      SELF_Test_Result_Data.word[DC_LOAD__UV_ICE_FAUCET2]
#define U16_UV_ICE_TANK_0503                      SELF_Test_Result_Data.word[DC_LOAD__UV_ICE_TANK_1_2]
#define U16_UV_ICE_TRAY_0504                      SELF_Test_Result_Data.word[DC_LOAD__UV_ICE_TRAY]
#define U16_UV_ICE_TRAY_0508                      SELF_Test_Result_Data.word[DC_LOAD__UV_ICE_TANK_3]

#define U16_SENSOR_ICEFULL_0801                   SELF_Test_Result_Data.word[DC_LOAD__ICE_FULL_SENSOR]

#define U16_SWINGBAR_0802                         SELF_Test_Result_Data.word[DC_LOAD__SWING_BAR]
#define U16_ICETRAY_STEPMOTOR_0804                SELF_Test_Result_Data.word[DC_LOAD__ICE_TRAY_MOTOR]
#define U16_TRAY_MICROSWITCH_0805                 SELF_Test_Result_Data.word[DC_LOAD__MICRO_SWITCH]
#define U16_ICEDOOR_STEPMOTOR_0807                SELF_Test_Result_Data.word[DC_LOAD__ICE_DOOR_MOTOR]
#define U16_GAS_SWITCH_MOTOR                      SELF_Test_Result_Data.word[DC_LOAD__GAS_SWITCH_MOTOR]

#define U16_DRAIN_PUMP_080A                       SELF_Test_Result_Data.word[DC_LOAD__DRAIN_PUMP]
#define U16_DRAIN_PUMP_ERROR_E60_080E             SELF_Test_Result_Data.word[DC_LOAD__DRAIN_PUMP_ERROR_E60]

#define U16_ICE_DOOR_HEATER                       SELF_Test_Result_Data.word[DC_LOAD__ICE_DOOR_HEATER]              // 2KG MAX
#define U16_ICE_CRUSH_MOTOR                       SELF_Test_Result_Data.word[DC_LOAD__ICE_CRUSH_MOTOR]              // 2KG MAX
#define U16_ICE_SELECT_DOOR                       SELF_Test_Result_Data.word[DC_LOAD__ICE_SELECT_DOOR_MOTOR]        // 2KG MAX
#define U16_ICE_LACK_SENSOR_0814                  SELF_Test_Result_Data.word[DC_LOAD__ICE_LACK_SENSOR]              // 2KG MAX



extern TYPE_BYTE       U8OperationB;
#define         u8Operation                                       U8OperationB.byte
#define         Bit0_Cold_Operation_Disable_State                 U8OperationB.Bit.b0
#define         Bit1_Hot_Operation_Disable_State                  U8OperationB.Bit.b1
#define         Bit2_Ice_Operation_Disable_State                  U8OperationB.Bit.b2
#define         Bit3_Ice_Tank_Ster_Operation_Disable_State        U8OperationB.Bit.b3


extern U8 gu8_Wifi_Connect_State;
extern bit F_WaterOut;

extern bit bit_self_test_start;

extern U8 gu8_Room_Water_Level;
extern bit bit_hot_first_op;
extern bit F_WaterOut;
extern bit F_Heater_Output;
extern U8 gu8_Hot_Tank_Temperature_One_Degree;
extern U8 gu8_heater_off_temp;
extern bit F_Hot_Enable;


#define SELF_TEST_UPDATE_TIME_24HOUR    864000

#define SELF_TEST_TIME_1_HOUR           36000


extern bit F_Cold_Enable;
extern U8 gu8_Cold_Temperature_One_Degree;
extern bit F_Comp_Output;
extern U16 mu16_cold_off_temp;

#endif
