/// @file     Lib_ADConvert.h
/// @date
/// @author   Jaejin Ham
/// @brief    각 센서류  동작 제어 관련 헤더 file

#ifndef __FUNC_AD_CONVERT_H__
#define __FUNC_AD_CONVERT_H__


#define AD_CH_0                                     0
#define AD_CH_1                                     1
#define AD_CH_2                                     2
#define AD_CH_3                                     3
#define AD_CH_4                                     4
#define AD_CH_5                                     5
#define AD_CH_6                                     6
#define AD_CH_7                                     7
#define AD_CH_8                                     8
#define AD_CH_9                                     9
#define AD_CH_10                                    10
#define AD_CH_11                                    11
#define AD_CH_12                                    12
#define AD_CH_13                                    13
#define AD_CH_14                                    14
#define AD_CH_15                                    15
#define AD_CH_16                                    16
#define AD_CH_17                                    17        // Wi-Fi Uart Port 사용금지
#define AD_CH_18                                    18
#define AD_CH_19                                    19
#define	AD_CH_20                                    20
#define	AD_CH_21                                    21
#define	AD_CH_22                                    22
#define	AD_CH_23                                    23
#define	AD_CH_24                                    24
#define	AD_CH_25                                    25

#define AD_CH_0_USE                                 NO
#define AD_CH_1_USE                                 NO
#define AD_CH_2_USE                                 NO
#define AD_CH_3_USE                                 NO
#define AD_CH_4_USE                                 NO
#define AD_CH_5_USE                                 NO
#define AD_CH_6_USE                                 NO
#define AD_CH_7_USE                                 NO
#define AD_CH_8_USE                                 NO
#define AD_CH_9_USE                                 NO
#define AD_CH_10_USE                                NO
#define AD_CH_11_USE                                NO
#define AD_CH_12_USE                                NO
#define AD_CH_13_USE                                NO
#define AD_CH_14_USE                                NO
#define AD_CH_15_USE                                NO
#define AD_CH_16_USE                                NO
#define AD_CH_17_USE                                NO      // Wi-Fi Uart Port 사용금지
#define AD_CH_18_USE                                NO
#define AD_CH_19_USE                                NO
#define	AD_CH_20_USE                                NO
#define	AD_CH_21_USE                                NO
#define	AD_CH_22_USE                                NO
#define	AD_CH_23_USE                                NO
#define	AD_CH_24_USE                                NO
#define	AD_CH_25_USE                                NO

#define AD_CH_MAX                                   26
#define AD_CONV_10BIT                               6


void AD_Convert_Initialize(void);
void ADC_Start(void);
void ADC_Stop(void);
U16 Get_ADC_Value(U8 mu8AD_Channel);
U16 Get_ADC_Operation(U8 mu8AD_Channel, U8 mu8Resolution);
void Average_ADC(U8 mu8AD_Channel, U16 mu16AD_Value);
void AD_Convert_Control(void);


#endif

