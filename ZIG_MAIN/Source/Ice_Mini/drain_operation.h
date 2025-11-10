/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _DRAIN_OP_H_
#define _DRAIN_OP_H_



//

extern void auto_drain_control(void);




extern TYPE_BYTE          U8DrainWaterLevelB;
#define            u8DrainWaterLevel                       U8DrainWaterLevelB.byte
#define            Bit0_Drain_Water_Empty                U8DrainWaterLevelB.Bit.b0
#define            Bit1_Drain_Water_Low                 U8DrainWaterLevelB.Bit.b1
#define            Bit2_Drain_Water_High                U8DrainWaterLevelB.Bit.b2
#define            Bit3_Drain_Water_Error                U8DrainWaterLevelB.Bit.b3


extern TYPE_LONG       U32ControlErrorsD;
#define         u32ControlErrors                                    U32ControlErrorsD.dward
#define         Bit0_Hot_Water_Flow_Block_Error__E08              	U32ControlErrorsD.Bit.b0    //E08
#define         Bit1_Main_Water_Flow_Leaked_Error__E02              U32ControlErrorsD.Bit.b1    //E02
#define         Bit2_Room_Temp_Open_Short_Error__E42                U32ControlErrorsD.Bit.b2    //E42
#define         Bit3_Leakage_Sensor_Error__E01                      U32ControlErrorsD.Bit.b3    //E01

#define         Bit4_Hot_In_Temp_Open_Short_Error__E46              U32ControlErrorsD.Bit.b4    //E46
#define         Bit5_Hot_Heater_OverHeat_Error__E40		            U32ControlErrorsD.Bit.b5    //E40
#define         Bit6_Main_Water_Flow_Block_Error__E09               U32ControlErrorsD.Bit.b6    //E09
#define         Bit7_BLDC_Communication_Error__E27                  U32ControlErrorsD.Bit.b7    //E27

#define         Bit8_BLDC_Operation_Error_Current_Sensing__E81      U32ControlErrorsD.Bit.b8    //E81
#define         Bit9_BLDC_Operation_Error_Starting_Fail__E82        U32ControlErrorsD.Bit.b9    //E82
#define         Bit10_BLDC_Operation_Error_Over_Current__E83        U32ControlErrorsD.Bit.b10   //E83
#define         Bit11_BLDC_Operation_Error_Overheat__E84            U32ControlErrorsD.Bit.b11   //E84

#define         Bit12_BLDC_Operation_Error_Disconnection_Comm__E85  U32ControlErrorsD.Bit.b12   //E85
#define         Bit13_BLDC_Operation_Error_Abnormal_Voltage__E86    U32ControlErrorsD.Bit.b13   //E86
#define         Bit14_Cold_Temp_Open_Short_Error__E44               U32ControlErrorsD.Bit.b14   //E44
#define         Bit15_Amb_Temp_Open_Short_Error__E43                U32ControlErrorsD.Bit.b15   //E43

#define         Bit16_Drain_Pump_Error__E60                         U32ControlErrorsD.Bit.b16   //E60
#define         Bit17_Tray_Micro_SW_Dual_Detect_Error__E61          U32ControlErrorsD.Bit.b17   //E61
#define         Bit18_Tray_Micro_SW_Up_Move_Error__E62              U32ControlErrorsD.Bit.b18   //E62
#define         Bit19_Tray_Micro_SW_Down_Move_Error__E63            U32ControlErrorsD.Bit.b19   //E63

#define         Bit20_Hot_Out_Temp_Open_Short_Error__E47	        U32ControlErrorsD.Bit.b20   //E52
#define         Bit21_Amb_Side_Temp_Open_Short_Error__E53           U32ControlErrorsD.Bit.b21   //E53
#define         Bit22_Hot_Heater_Temp_Open_Short_Error__E48		    U32ControlErrorsD.Bit.b22   //E48

#define         Bit23_faucet_UV_Error__E77                  		U32ControlErrorsD.Bit.b23   //E77
#define         Bit24_Ice_Tank_1_2_UV_Error__E75					    U32ControlErrorsD.Bit.b24   //E75
#define         Bit25_Ice_Faucet_UV_1_Error__E78                    U32ControlErrorsD.Bit.b25   //E78
#define         Bit26_Ice_Faucet_UV_2_Error__E78                    U32ControlErrorsD.Bit.b26   //E78
#define         Bit27_Ice_Tank_3_UV_Error__E75                	U32ControlErrorsD.Bit.b27   //E79
#define         Bit28_Ice_Tray_1_2_UV_Error__E76                	U32ControlErrorsD.Bit.b28   //E76
















extern TYPE_BYTE          U8WaterOutStateB;
#define            u8WaterOutState                           U8WaterOutStateB.byte
#define            Bit0_Pure_Water_Select_State                U8WaterOutStateB.Bit.b0
#define            Bit1_Cold_Water_Select_State                 U8WaterOutStateB.Bit.b1
#define            Bit2_Hot_Water_Select_State                U8WaterOutStateB.Bit.b2
#define            Bit3_Ice_Water_Select_State                U8WaterOutStateB.Bit.b3


extern TYPE_BYTE          U812VPowerControl;
#define            u8PowerControl12V                                    U812VPowerControl.byte
#define            Bit0_Gas_Switch_12V_Out_State                        U812VPowerControl.Bit.b0
#define            Bit1_Ice_Tank_UV_12V_Out_State                       U812VPowerControl.Bit.b1
#define            Bit2_Uart_Test_Mode_State                            U812VPowerControl.Bit.b2
#define            Bit3_Water_Tank_UV_12V_Out_State                     U812VPowerControl.Bit.b3
#define            Bit4_Ice_Tray_UV_12V_Out_State                       U812VPowerControl.Bit.b4
#define            Bit5_Ice_Tank_Front_UV_12V_Out_State                 U812VPowerControl.Bit.b5
#define            Bit6_Drain_Pump_FeedBack_12V_Out_State               U812VPowerControl.Bit.b6


extern U16 gu16_AD_Drain_Pump_Current;
//extern bit F_IceOn;
//extern bit F_ErrDrain;

extern bit bit_auto_drain_pump_out;
extern bit F_DrainStatus;
//extern U8 gu8_room_level;
//extern U8 gu8ColdLEV;
//extern bit F_Overflow;
extern bit F_WaterOut;
extern U8 gu8_Room_Water_Level;
extern bit F_Tank_Cover_Input;
extern bit F_IceFull;
extern ICE_STEP gu8IceStep;
extern bit F_IceOn;

extern bit F_FW_Version_Display_Mode;
extern bit bit_manual_drain_start;
extern bit bit_manual_drain_pump_output;
extern bit bit_tank_drain_pump_output;
extern FLUSHING_STEP gu8_flushing_mode;
extern bit bit_flushing_tank_start;
extern U8 gu8_flushing_tank_step;
extern U8 gu8_prepare_ster_step;
extern ICE_STER_MODE gu8_ice_ster_mode;
extern U8 gu8_wait_finish_step;
extern COLD_TANK_FILL_STEP gu8_cold_tank_fill_step;
extern bit bit_ice_tank_ster_start;
extern U8 gu8_hot_wait_step;

//#define DRAIN_WATER_LEVEL_DETECT_TIME    30
#define DRAIN_PUMP_MAX_OP_TIME                 30
/////#define DRAIN_RETRY_PUMP_1MIN_OFF              0
/////#define DRAIN_RETRY_PUMP_2MIN_ON               1
#define DRAIN_ERROR_PUMP_ON_10_MIN             6000
#define DRAIN_ERROR_PUMP_OFF_1_MIN             600
#define DRAIN_ERROR_PUMP_ON_2_MIN              1200
#define FORCED_DRAIN_ADDITIONAL_TIME           100

/*..hui [18-7-19오후 1:28:38] 드레인펌프 에러 감지 2회째부터 Full Duty 가동..*/
#define DRAIN_PUMP_MAX_OUTPUT_ERROR_COUNT      1


/*#define DRAIN_LEVEL_ERROR_OPERATION_MIN_TIME   1800*/
#define DRAIN_LEVEL_ERROR_OPERATION_MIN_TIME   3000

#define DRAIN_LEVEL_ERROR_RETYR_COUNT          20

/*#define PUMP_OVER_CURRENT_ADC                        40*/

#if 0
/*..hui [23-8-21오후 1:57:03] 실제 프로텍트 걸리면 ad 50정도임..*/
#define PUMP_OVER_CURRENT_ADC                        70
#define PUMP_OVER_CURRENT_DETECT_TIME                50
#define PUMP_OVER_CURRENT_OFF_TIME                   10

#define PUMP_NO_LOAD_MINIMUM_OPERATION_TIME          450
#define DRAIN_NO_WATER_FEEDBACK_AD                              190
#define PUMP_NO_LOAD_CHECK_TIME                      30
#endif





#endif
