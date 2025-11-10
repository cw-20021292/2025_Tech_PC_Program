/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _SELF_TEST_MANUAL_COMPONENT_H_
#define _SELF_TEST_MANUAL_COMPONENT_H_

//
extern U16 calculate_dc_current( U16 mu16_ad );
extern U16 calculate_fan_current( U16 mu16_ad );
extern U16 calculate_uv_current( U16 mu16_ad );
extern U16 calculate_pump_current( U16 mu16_ad );

extern void initial_each_self_data( U8 mu8_num );

extern void start_drain_pump( U16 u16_data );
extern void stop_drain_pump( void );
extern void start_tray_pump( U16 u16_data );
extern void stop_tray_pump( void );

extern void run_open_ro_drain(void);
extern void run_close_ro_drain(void);
extern void GasSwitchInit(void);
extern void run_init_flow(void);
extern void cody_ice_tray_test(void);

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


extern TYPE_BYTE          U8FrontExtractUVLEDB;
#define            gu8_front_extract_uv_led                    U8FrontExtractUVLEDB.byte
#define            Bit0_Front_Led_UV_Sterilization             U8FrontExtractUVLEDB.Bit.b0
#define            Bit1_Front_Led_Faucet                       U8FrontExtractUVLEDB.Bit.b1
#define            Bit2_Front_Led_Ice_Tank                     U8FrontExtractUVLEDB.Bit.b2
#define            Bit3_Front_Led_Ice_Extract                  U8FrontExtractUVLEDB.Bit.b3
#define            Bit4_Front_Led_Water_Extract                U8FrontExtractUVLEDB.Bit.b4
//#define            Bit5_Front_Led_Icon_All_Lock                U8FrontExtractUVLEDB.Bit.b5
#define            Bit5_Front_Led_Icon_ice_faucet			   U8FrontExtractUVLEDB.Bit.b5
#define            Bit6_Front_Led_Icon_Save                    U8FrontExtractUVLEDB.Bit.b6
#define            Bit7_Front_Led_Icon_Ice_First               U8FrontExtractUVLEDB.Bit.b7

extern TYPE_BYTE          U8CodyInspectOperationB;
#define            u8CodyInspectOperation                      U8CodyInspectOperationB.byte
#define            Bit0_Cody_Service_Kit_Start                 U8CodyInspectOperationB.Bit.b0
#define            Bit1_Empty_Tank_Start                       U8CodyInspectOperationB.Bit.b1
#define            Bit2_Ice_Tray_Test_Start                    U8CodyInspectOperationB.Bit.b2

extern U16 gu16_AD_Result_Fan_Current;

extern bit F_IceDoorClose;
extern bit F_IceSelectDoorClose;

extern U16 gu16_AD_Drain_Pump_Current;
extern U16 gu16_AD_Tray_Pump_Current;
extern U16 gu16_AD_Result_DC_Current_12V;
extern U16 gu16_AD_Result_DC_Current_24V;
extern U16 gu16_AD_Drain_Pump_Current;
extern U16 gu16_AD_Tray_Pump_Current;

extern U16 gu16ADIceFull;
extern U16 gu16ADIceLow;

extern U16 gu16_AD_Result_UV_Faucet_Current;
extern U8 gu8_front_uv_ice_faucet_current;
extern U16 gu16_AD_Result_UV_Ice_Tank_Current;
extern U16 gu16_AD_Result_DC_Current_24V;
extern U8 gu8_Flow_1sec;

extern U16 gu16_AD_Result_UV_Water_Faucet_Current_Feed;
extern U16 gu16_AD_Result_UV_Ice_Faucet_One_Current;
extern U16 gu16_AD_Result_UV_Ice_Faucet_Two_Current;
extern U16 gu16_AD_Result_UV_Ice_Tank_1_2_Current;
extern U16 gu16_AD_Result_UV_Ice_Tray_1_2_Current_Feed;

extern U16 gu16_AD_Result_Heater_Current_Feed;
extern U8 gu8_Hot_Heater_Temperature_One_Degree;

extern U16 gu16_AD_Result_IceDoor_Heater_Current;
extern U16 gu16_AD_Result_UV_Ice_Tank_3_Current_Feed;
/**********************************************************************************************************************/
extern bit F_First_Hot_Effluent;
extern U16 gu16_flow_info_target;
extern U16 gu16_flow_info_current;

/*#define DC_CURRENT_STABLE_TIME              10*/
#define DC_CURRENT_STABLE_TIME              45
#define CURRENT_DELAY_TIME                  5
#define SELF_MANUAL_FLOW_TEST_TIME          40
#define HEATER_TEST_PROHIBIT_TEMP           85

#endif
