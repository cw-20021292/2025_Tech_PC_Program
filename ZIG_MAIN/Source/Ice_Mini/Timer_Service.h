/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _Timer_Service_H_
#define _Timer_Service_H_



//extern void input_select_bar(void);
//extern void input_overflow(void);
extern void BuzTimeCounter(void);
extern void BuzControl(void);
//extern void LeverInput(void);
//extern void Extract_KeyInput(void);
//extern void IceMakeOrder(void);
extern void input_ice_tray_micro_sw(void);
extern void output_valve_extract(void);
extern void AD_Conversion(void);
extern void start_drain_pump( U16 u16_data );
extern void stop_drain_pump( void );

//extern void TAU0_Channel3_Start(void);
//extern void input_extract_key(void);
//extern void extract_key_management(void);
extern void output_valve_ice_tray_feed3(void);
extern void key_input_ice_extract(void);
extern void key_input_water_extract(void);
//extern void power_saving_timer(void);
extern void flow_input_count(void);
extern void Key_Input(void);

extern void Front_Communication(void);
extern void final_mode_decision(void);
/////extern void filter_flow_input_count(void);
/////extern void TDS_In_Control(void);
/////extern void TDS_Out_Pulse_Control(void);

///////////extern void BLDC_RXD2_TimeOutCheck(void);
extern TYPE_BYTE          U812VPowerControl;
#define            u8PowerControl12V                                    U812VPowerControl.byte
#define            Bit0_Gas_Switch_12V_Out_State                        U812VPowerControl.Bit.b0
#define            Bit1_Ice_Tank_UV_12V_Out_State                       U812VPowerControl.Bit.b1
#define            Bit2_Uart_Test_Mode_State                            U812VPowerControl.Bit.b2
#define            Bit3_Water_Tank_UV_12V_Out_State                     U812VPowerControl.Bit.b3
#define            Bit4_Ice_Tray_UV_12V_Out_State                       U812VPowerControl.Bit.b4
#define            Bit5_Ice_Tank_Front_UV_12V_Out_State                 U812VPowerControl.Bit.b5
#define            Bit6_Drain_Pump_FeedBack_12V_Out_State               U812VPowerControl.Bit.b6







extern bit F_System_Init_Finish;
//extern U8 gu8OVFOnDelay;
extern bit F_ContyGo;
extern U16 gu16IRInterval;
extern bit F_IceFull;
//extern U8 gu8Bar;
//extern U8 gu8OVF;
extern U8 gu8Out;
extern U16 gu16ExtrKeyDelay;
//extern U16 gu16_ice_level_delay;
extern U8 gu8Reverse;
//extern U8 gu8ErrRTHDelay;
//extern U8 gu8ErrCTHDelay;
//extern U8 gu8ErrHTHDelay;
//extern U8 gu8ErrETHDelay;
//extern U8 gu8ErrATHDelay;
//extern U8 gu8ErrOVCDelay;
//extern U8 gu8ErrOVHDelay;
extern U8 gu8_cristal_timer;
extern U16 gu16ColdVVOnTime;
extern U8 gu8IRTime;
extern U8 gu8ErrDoor;
//extern U8 gu8ErrBar;
extern U8 gu8IceClose;
extern U8 gu8IceOutCCWInterval;
extern U8 u8_Tray_up_moving_retry_timer;
//extern U8 gu8_room_level_delay;
//extern U8 gu8ColdLEVDelay;
//extern U8 gu8IceOverCold;
extern U8 gu8HeaterOffDelay;
extern U16 gu16IceVVTime;
//extern U16 gu16IceVVTime1;
extern U16 gu16CompOffDelay;
//extern U16 gu16NoWaterOut;
extern bit F_NoUseSave;
extern bit F_IceInit;
extern U16 gu16IceMakeTime;
extern bit F_Save;
//extern U8 gu8ColdWaterLEV;
//extern bit F_IceOn;
//extern U16 gu16MeltTime;
//extern bit F_ErrDrainSet;
extern U16 gu16_AD_Drain_Pump_Current;
extern bit F_Powerin1sec;
//extern bit F_Reset;
extern U16 ucErrOvice_Valve;
extern U16 ucTime_10min_cycle;
//extern U16 gu16IniStepDelay;
//extern U8 gu8ICETrayRoomDelay;
extern U8 Tray_Comp_Delay;
//extern U8 gu8OVFOffDelay;
extern U16 ucErrOVice_Heater;
extern U16 ucErrOvice_Time;
extern U16 gu16IceOutOff;
//extern U16 gu16ErrColdLEVDelay;
//extern U16 gu16IceStopTime;
extern bit Be_COMP;
extern U16 gu16IceHeaterTime;
//extern bit F_Hotgas_Time;
//extern U16 usNFCTestTime;
//extern U16 gu16TestTime;
extern U16 gu16IceStopTime;
extern U8 gu8_altutude_setting_timeout;

extern U16 u16Efluent_Time;
extern WATER_OUT_STEP gu8_Water_Out_Step;
extern U8 display_test_flick_timer_100ms;
extern U8 gu8IceInnerClose;
#endif
