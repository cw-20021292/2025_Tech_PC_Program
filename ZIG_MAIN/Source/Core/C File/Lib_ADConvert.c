/// @file     Lib_ADConvert.c
/// @date
/// @author   Jaejin Ham
/// @brief    AD Convert 동작 제어 file

#include "Global_Header.h"


typedef struct {
    U16 mu16Average[AD_CH_MAX];
    U16 mu16Sum[AD_CH_MAX];
    U8  mu8Count[AD_CH_MAX];
    U16 mu16Max[AD_CH_MAX];
    U16 mu16Min[AD_CH_MAX];
}   tAD_DataTable;

tAD_DataTable tAD_Data;          // AD Data Table

U8 gu8AD_ChannelCount = 0;


/// @brief    AD Convert Initialize
/// @param    void
/// @return   void
void AD_Convert_Initialize(void)
{
    U8 mu8i = 0;

    for (mu8i = 0 ; mu8i < AD_CH_MAX ; mu8i++)
    {
        tAD_Data.mu16Average[mu8i] = 0;
        tAD_Data.mu16Sum[mu8i] = 0;
        tAD_Data.mu8Count[mu8i] = 0;
        tAD_Data.mu16Max[mu8i] = 0;
        tAD_Data.mu16Min[mu8i] = 1024;
    }

    gu8AD_ChannelCount = 0;
}


/// @brief    AD Convert Start
/// @param    void
/// @return   void
void ADC_Start(void)
{
    ADIF = 0;  /* clear INTAD interrupt flag */
    //ADMK = 0;  /* enable INTAD interrupt */
    ADCS = 1;  /* enable AD conversion */
}


/// @brief    AD Convert Stop
/// @param    void
/// @return   void
void ADC_Stop(void)
{
    ADCS = 0;  /* disable AD conversion */
    //ADMK = 1;  /* disable INTAD interrupt */
    ADIF = 0;  /* clear INTAD interrupt flag */
}


/// @brief    AD 값 확인 함수
/// @param    mu8AD_Channel : AD Channel No
/// @return   tAD_Data.mu16Average[mu8AD_Channel] : 측정한 Channel의 AD 평균값
U16 Get_ADC_Value(U8 mu8AD_Channel)
{
    return  tAD_Data.mu16Average[mu8AD_Channel];
}


/// @brief    AD Convert 동작 함수
/// @param    mu8AD_Channel : AD Channel No
///           mu8Resolution : AD Register Shift할 Bit 수
/// @return   mu16AD_Result : AD 측정 값
U16 Get_ADC_Operation(U8 mu8AD_Channel, U8 mu8Resolution)
{
    U16 mu16AD_Result = 0;
    U16 mu16_final_ad_value = 0;

    ADC_Stop();

    ADS = mu8AD_Channel;
    ADCE = 1U;  /* enable AD comparator */

    ADC_Start();

    while (ADIF != SET) {}

    ADC_Stop();
    ADCE = 0U;  /* disable AD comparator */

    mu16AD_Result = (U16)(ADCR >> mu8Resolution);

    return  mu16AD_Result;
}


/// @brief    AD Convert 평균 계산 함수
/// @param    mu8AD_Channel : AD Channel No
///           mu16AD_Value : AD 값
/// @return   void
void Average_ADC(U8 mu8AD_Channel, U16 mu16AD_Value)
{
    if (mu16AD_Value > tAD_Data.mu16Max[mu8AD_Channel])
    {
        tAD_Data.mu16Max[mu8AD_Channel] = mu16AD_Value;      // 최대값 저장
    }

    if (mu16AD_Value < tAD_Data.mu16Min[mu8AD_Channel])
    {
        tAD_Data.mu16Min[mu8AD_Channel] = mu16AD_Value;      // 최소값 저장
    }

    tAD_Data.mu16Sum[mu8AD_Channel] += mu16AD_Value;

    if (++tAD_Data.mu8Count[mu8AD_Channel] >= 22)
    {     // 최대, 최소값을 제외한 20회 평균값 계산
        tAD_Data.mu16Average[mu8AD_Channel] = (U16)((tAD_Data.mu16Sum[mu8AD_Channel] - (tAD_Data.mu16Max[mu8AD_Channel] + tAD_Data.mu16Min[mu8AD_Channel])) / 20);

        tAD_Data.mu16Sum[mu8AD_Channel] = 0;
        tAD_Data.mu8Count[mu8AD_Channel] = 0;
        tAD_Data.mu16Max[mu8AD_Channel] = 0;
        tAD_Data.mu16Min[mu8AD_Channel] = 1024;
     }
}


/// @brief    AD Convert 동작 함수(1ms 루틴)
/// @param    void
/// @return   void
void AD_Convert_Control(void)
{
    U16 mu16AD_result = 0;

    switch (gu8AD_ChannelCount)
    {
        case AD_CH_0:
#if (AD_CH_0_USE == YES)
            mu16AD_result = Get_ADC_Operation(gu8AD_ChannelCount, AD_CONV_10BIT);
            Average_ADC(gu8AD_ChannelCount, mu16AD_result);
#endif
            break;

        case AD_CH_1:
#if (AD_CH_1_USE == YES)
            mu16AD_result = Get_ADC_Operation(gu8AD_ChannelCount, AD_CONV_10BIT);
            Average_ADC(gu8AD_ChannelCount, mu16AD_result);
#endif
            break;

        case AD_CH_2:
#if (AD_CH_2_USE == YES)
            mu16AD_result = Get_ADC_Operation(gu8AD_ChannelCount, AD_CONV_10BIT);
            Average_ADC(gu8AD_ChannelCount, mu16AD_result);
#endif
            break;

        case AD_CH_3:
#if (AD_CH_3_USE == YES)
            mu16AD_result = Get_ADC_Operation(gu8AD_ChannelCount, AD_CONV_10BIT);
            Average_ADC(gu8AD_ChannelCount, mu16AD_result);
#endif
            break;

        case AD_CH_4:
#if (AD_CH_4_USE == YES)
            mu16AD_result = Get_ADC_Operation(gu8AD_ChannelCount, AD_CONV_10BIT);
            Average_ADC(gu8AD_ChannelCount, mu16AD_result);
#endif
            break;

        case AD_CH_5:
#if (AD_CH_5_USE == YES)
            mu16AD_result = Get_ADC_Operation(gu8AD_ChannelCount, AD_CONV_10BIT);
            Average_ADC(gu8AD_ChannelCount, mu16AD_result);
#endif
            break;

        case AD_CH_6:
#if (AD_CH_6_USE == YES)
            mu16AD_result = Get_ADC_Operation(gu8AD_ChannelCount, AD_CONV_10BIT);
            Average_ADC(gu8AD_ChannelCount, mu16AD_result);
#endif
            break;

        case AD_CH_7:
#if (AD_CH_7_USE == YES)
            mu16AD_result = Get_ADC_Operation(gu8AD_ChannelCount, AD_CONV_10BIT);
            Average_ADC(gu8AD_ChannelCount, mu16AD_result);
#endif
            break;

        case AD_CH_8:
#if (AD_CH_8_USE == YES)
            mu16AD_result = Get_ADC_Operation(gu8AD_ChannelCount, AD_CONV_10BIT);
            Average_ADC(gu8AD_ChannelCount, mu16AD_result);
#endif
            break;

        case AD_CH_9:
#if (AD_CH_9_USE == YES)
            mu16AD_result = Get_ADC_Operation(gu8AD_ChannelCount, AD_CONV_10BIT);
            Average_ADC(gu8AD_ChannelCount, mu16AD_result);
#endif
            break;

        case AD_CH_10:
#if (AD_CH_10_USE == YES)
            mu16AD_result = Get_ADC_Operation(gu8AD_ChannelCount, AD_CONV_10BIT);
            Average_ADC(gu8AD_ChannelCount, mu16AD_result);
#endif
            break;

        case AD_CH_11:
#if (AD_CH_11_USE == YES)
            mu16AD_result = Get_ADC_Operation(gu8AD_ChannelCount, AD_CONV_10BIT);
            Average_ADC(gu8AD_ChannelCount, mu16AD_result);
#endif
            break;

        case AD_CH_12:
#if (AD_CH_12_USE == YES)
            mu16AD_result = Get_ADC_Operation(gu8AD_ChannelCount, AD_CONV_10BIT);
            Average_ADC(gu8AD_ChannelCount, mu16AD_result);
#endif
            break;

        case AD_CH_13:
#if (AD_CH_13_USE == YES)
            mu16AD_result = Get_ADC_Operation(gu8AD_ChannelCount, AD_CONV_10BIT);
            Average_ADC(gu8AD_ChannelCount, mu16AD_result);
#endif
            break;

        case AD_CH_14:
#if (AD_CH_14_USE == YES)
            mu16AD_result = Get_ADC_Operation(gu8AD_ChannelCount, AD_CONV_10BIT);
            Average_ADC(gu8AD_ChannelCount, mu16AD_result);
#endif
            break;

        case AD_CH_15:
#if (AD_CH_15_USE == YES)
            mu16AD_result = Get_ADC_Operation(gu8AD_ChannelCount, AD_CONV_10BIT);
            Average_ADC(gu8AD_ChannelCount, mu16AD_result);
#endif
            break;

        case AD_CH_16:
#if (AD_CH_16_USE == YES)
            mu16AD_result = Get_ADC_Operation(gu8AD_ChannelCount, AD_CONV_10BIT);
            Average_ADC(gu8AD_ChannelCount, mu16AD_result);
#endif
            break;

        case AD_CH_17:
#if (AD_CH_17_USE == YES)
            mu16AD_result = Get_ADC_Operation(gu8AD_ChannelCount, AD_CONV_10BIT);
            Average_ADC(gu8AD_ChannelCount, mu16AD_result);
#endif
            break;

        case AD_CH_18:
#if (AD_CH_18_USE == YES)
            mu16AD_result = Get_ADC_Operation(gu8AD_ChannelCount, AD_CONV_10BIT);
            Average_ADC(gu8AD_ChannelCount, mu16AD_result);
#endif
            break;

        case AD_CH_19:
#if (AD_CH_19_USE == YES)
            mu16AD_result = Get_ADC_Operation(gu8AD_ChannelCount, AD_CONV_10BIT);
            Average_ADC(gu8AD_ChannelCount, mu16AD_result);
#endif
            break;

        case AD_CH_20:
#if (AD_CH_20_USE == YES)
            mu16AD_result = Get_ADC_Operation(gu8AD_ChannelCount, AD_CONV_10BIT);
            Average_ADC(gu8AD_ChannelCount, mu16AD_result);
#endif
            break;

        case AD_CH_21:
#if (AD_CH_21_USE == YES)
            mu16AD_result = Get_ADC_Operation(gu8AD_ChannelCount, AD_CONV_10BIT);
            Average_ADC(gu8AD_ChannelCount, mu16AD_result);
#endif
            break;

        case AD_CH_22:
#if (AD_CH_22_USE == YES)
            mu16AD_result = Get_ADC_Operation(gu8AD_ChannelCount, AD_CONV_10BIT);
            Average_ADC(gu8AD_ChannelCount, mu16AD_result);
#endif
            break;

        case AD_CH_23:
#if (AD_CH_23_USE == YES)
            mu16AD_result = Get_ADC_Operation(gu8AD_ChannelCount, AD_CONV_10BIT);
            Average_ADC(gu8AD_ChannelCount, mu16AD_result);
#endif
            break;

        case AD_CH_24:
#if (AD_CH_24_USE == YES)
            mu16AD_result = Get_ADC_Operation(gu8AD_ChannelCount, AD_CONV_10BIT);
            Average_ADC(gu8AD_ChannelCount, mu16AD_result);
#endif
            break;

        case AD_CH_25:
#if (AD_CH_25_USE == YES)
            mu16AD_result = Get_ADC_Operation(gu8AD_ChannelCount, AD_CONV_10BIT);
            Average_ADC(gu8AD_ChannelCount, mu16AD_result);
#endif
            break;

        default:
            gu8AD_ChannelCount = 0;
            mu16AD_result = 0;
            break;
    }

    if (++gu8AD_ChannelCount >= AD_CH_MAX)
    {
        gu8AD_ChannelCount = 0;
    }
}




