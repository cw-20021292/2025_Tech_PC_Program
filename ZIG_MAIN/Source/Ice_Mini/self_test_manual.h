/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _SELF_TEST_MANUAL_H_
#define _SELF_TEST_MANUAL_H_
/**********************************************************************************************************************/
extern TYPE_BYTE          U812VPowerControl;
#define            u8PowerControl12V                                    U812VPowerControl.byte
#define            Bit0_Gas_Switch_12V_Out_State                        U812VPowerControl.Bit.b0
#define            Bit1_Ice_Tank_UV_12V_Out_State                       U812VPowerControl.Bit.b1
#define            Bit2_Uart_Test_Mode_State                            U812VPowerControl.Bit.b2
#define            Bit3_Water_Tank_UV_12V_Out_State                     U812VPowerControl.Bit.b3
#define            Bit4_Ice_Tray_UV_12V_Out_State                       U812VPowerControl.Bit.b4
#define            Bit5_Ice_Tank_Front_UV_12V_Out_State                 U812VPowerControl.Bit.b5
#define            Bit6_Drain_Pump_FeedBack_12V_Out_State               U812VPowerControl.Bit.b6
/**********************************************************************************************************************/
extern void play_melody_setting_on_198(void);
extern void Play_Voice(U16 mu16MemoryAddress);
extern void stop_ice_cody_mode(void);
extern void periodic_self_test(void);
extern U8 manual_test_heater(void);
extern void play_voice_smart_test_mode_16(void);
/**********************************************************************************************************************/
extern U16 gu16_gas_switch_current_max;
extern U8 gu8_component_test_step;
/**********************************************************************************************************************/
extern U16 gu16_AD_Result_Heater_Current_Feed;
extern U8 gu8_Hot_Heater_Temperature_One_Degree;
/**********************************************************************************************************************/
extern bit F_First_Hot_Effluent;
/**********************************************************************************************************************/
extern U16 gu16_diagnos_flow_val;
/**********************************************************************************************************************/
extern U8 gu8IceLEV;
/**********************************************************************************************************************/


#endif
