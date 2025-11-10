/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _AD_CONVERSION_H_
#define _AD_CONVERSION_H_
/***********************************************************************************************************************/
#define AD_Count_1                                  1
#define AD_Count_2                                  2
#define AD_Count_3                                  3
#define AD_Count_4                                  4
#define AD_Count_5                                  5
#define AD_Count_6                                  6
#define AD_Count_7                                  7
#define AD_Count_8                                  8
#define AD_Count_9                                  9
#define AD_Count_10                                 10
#define AD_Count_11                                 11
#define AD_Count_12                                 12
#define AD_Count_13                                 13
#define AD_Count_14                                 14
#define AD_Count_15                                 15
#define AD_Count_16                                 16
#define AD_Count_17                                 17
#define AD_Count_18                                 18
#define AD_Count_19                                 19
#define	AD_Count_20						            20
#define	AD_Count_21						            21
#define	AD_Count_22						            22
#define	AD_Count_23						            23
#define	AD_Count_24						            24
#define	AD_Count_25						            25
#define	AD_Count_26						            26
/***********************************************************************************************************************/
/* ADC�� �޾Ƶ��̴� ä�� ���� */
#define AD_Channel_DC_Current_24V                   0       // O
#define AD_Channel_DC_Current_12V                   1       // O
#define AD_Channel_Water_Leakage                    2       // O
#define AD_Channel_Hot_Heater_Temp                  3       // O
#define AD_Channel_Hot_Out_Temp                     4       // O
#define AD_Channel_Hot_In_Temp                      5       // O
#define AD_Channel_Amb_Temp                         6       // O
#define AD_Channel_EVA_1_TEMP                       7       // O
#define AD_Channel_Room_Temp                        8       // O
#define AD_Channel_Cold_Temp                        9       // O
#define AD_Channel_Drain_Pump_Current               10      // O

#define AD_Channel_UV_Water_Faucet_Current          11      // O o
#define AD_Channel_UV_Ice_Faucet_1_Current          12      // O o
#define AD_Channel_UV_Ice_Faucet_2_Current          13      // O o
#define AD_Channel_UV_Ice_Tank_1_2_Current          14      // O o
#define AD_Channel_UV_Ice_Tray_1_2_Current          18      // O o
#define AD_Channel_UV_Ice_Tank_3_Current            20      // O x

#define AD_Channel_Ice_Full_Receive_1               19      // O
#define AD_Channel_Ice_Full_Receive_2               21      // 2KG ������ũ ���� ���� ä�� �߰�. 250207 @CH.PARK
#define AD_Channel_Ice_Door_Heater_IAD              22      // O
#define AD_Channel_Hot_Heater_Current               23      // O
#define AD_Channel_Amb_2_Temp                       25      // O
#define AD_Channel_FAN_Current                      26      // O
/***********************************************************************************************************************/
/////#define AD_CONV_10BIT                      6
/////#define AD_CONV_8BIT                       8
/***********************************************************************************************************************/
//extern void R_ADC_Start(void);
//extern void R_ADC_Stop(void);
extern void ADC_Start(void);
extern void ADC_Stop(void);
/***********************************************************************************************************************/
extern bit bit_adc_cold_start;
extern bit bit_adc_room_start;
/***********************************************************************************************************************/
extern U16 gu16TDS_OUT_Pulse_Time;
extern bit bit_ice_tank_ster_start;
/***********************************************************************************************************************/

/***********************************************************************************************************************/

/***********************************************************************************************************************/

/***********************************************************************************************************************/

/***********************************************************************************************************************/

/***********************************************************************************************************************/

#endif

