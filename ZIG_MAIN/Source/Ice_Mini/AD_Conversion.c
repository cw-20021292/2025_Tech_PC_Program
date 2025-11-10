/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : System_init.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#include    "Macrodriver.h"
#include    "Global_Variable.h"
#include    "Port_Define.h"
#include    "AD_Conversion.h"
#include    "Temp_Table.h"

/***********************************************************************************************************************/

void AD_Conversion(void);
U16 get_adc_value( U8 ad_channel, U8 resolution);
void filtering_dc_current_24V_adc( U16 mu16ad_result );
void filtering_dc_current_12V_adc( U16 mu16ad_result );
void filtering_drain_pump_current_adc( U16 mu16ad_result );
void filtering_hot_tank_temp_adc( U16 mu16ad_result );
U16 ConvAdc2Temp_HotTankWater(U16 mu16Adc);
void filtering_hot_pump_current_adc( U16 mu16ad_result );

void filtering_uv_water_faucet_current_feed_adc( U16 mu16ad_result );
void filtering_uv_ice_faucet_one_current_feed_adc( U16 mu16ad_result );
void filtering_uv_ice_tray_1_2_current_feed_adc( U16 mu16ad_result );
void filtering_uv_ice_tank_1_2_current_feed_adc( U16 mu16ad_result );
void FILTERING_UV_ICE_TANK3_CURRENT_FEED_ADC( U16 mu16ad_result );

void filtering_uv_water_tank_current_feed_adc( U16 mu16ad_result );         /* �̻�� */

void filtering_ice_full_adc( U16 mu16ad_result );
void filtering_ice_Low_adc( U16 mu16ad_result );
void filtering_leakage_adc( U16 mu16ad_result );
void filtering_eva_1_temp_adc( U16 mu16ad_result );
//void filtering_eva_2_temp_adc( U16 mu16ad_result );
U16 ConvAdc2Temp_Eva(U16 mu16Adc);
void filtering_cold_temp_adc( U16 mu16ad_result );
U16 ConvAdc2Temp_ColdWater(U16 mu16Adc);
void filtering_room_temp_adc( U16 mu16ad_result );
U16 ConvAdc2Temp_RoomWater(U16 mu16Adc);
///void filtering_mixing_temp_adc( U16 mu16ad_result );
///U16 ConvAdc2Temp_MixingOutWater(U16 mu16Adc);
void filtering_amb_temp_adc( U16 mu16ad_result );
void filtering_amb_side_temp_adc( U16 mu16ad_result );
U16 ConvAdc2Temp_Ambient(U16 mu16Adc);
void filtering_fan_current_adc( U16 mu16ad_result );
void filtering_heater_ice_door_current_adc( U16 mu16ad_result );
//void filtering_uv_ice_tray_1_2_current_feed_adc( U16 mu16ad_result );
//void filtering_tray_temp_adc( U16 mu16ad_result );
//U16 ConvAdc2Temp_HotOutWater(U16 mu16Adc);

//void filtering_tds_in_temp_adc( U16 mu16ad_result );
//U16 ConvAdc2Temp_TDS_In_Water(U16 mu16Adc);
void filtering_uv_ice_faucet_two_current_adc( U16 mu16ad_result );
void filtering_heater_current_feed_adc( U16 mu16ad_result );

void filtering_hot_heater_adc( U16 mu16ad_result );
void filtering_hot_temp_adc( U16 mu16ad_result );
U16 ConvAdc2Temp_HotOutWater(U16 mu16Adc);
void filtering_hot_in_temp_adc( U16 mu16ad_result );
U16 ConvAdc2Temp_HotInWater(U16 mu16Adc);

/***********************************************************************************************************************/
/*..hui [17-11-24���� 1:47:00] �巹������ �ǵ�� ����..*/
U16 gu16_AD_Result_Cold;
U16 gu16ADIceFull;
U16 gu16ADCds;
U16 gu16ADLeakage;
//U8 gu8_AD_Result_Eva_First;
U16 gu16_AD_Result_Amb;
U16 gu16_AD_Result_Room;


U8 u8AdCount;

U16 gu16_AD_Drain_Pump_Current;
U16 u16Max_Drain_Pump_Current_AD;
U16 u16Min_Drain_Pump_Current_AD;
U32 u32AD_Drain_Pump_Current_Sum;
U8 u8Cnt_Drain_Pump_Current;




U16 gu16_AD_Tray_Pump_Current;
U16 u16Max_Hot_Pump_Current_AD;
U16 u16Min_Hot_Pump_Current_AD;
U32 u32AD_Hot_Pump_Current_Sum;
U8 u8Cnt_Hot_Pump_Current;


U16 u16Max_Cold_Temp_AD;
U16 u16Min_Cold_Temp_AD;
U32 u32AD_Cold_Temp_Sum;
U8 u8Cnt_Cold_Temp;
U16 gu16_Cold_Temperature;
U8 gu8_Cold_Temperature_One_Degree;


U16 u16Max_Amb_Temp_AD;
U16 u16Min_Amb_Temp_AD;
U32 u32AD_Amb_Temp_Sum;
U8 u8Cnt_Amb_Temp;
U16 gu16_Amb_Front_Temperature;
U8 gu8_Amb_Front_Temperature_One_Degree;

U8 gu8_Amb_Temperature_One_Degree;


U16 gu16_AD_Result_Eva_First;
U16 u16Max_Eva_1_Temp_AD;
U16 u16Min_Eva_1_Temp_AD;
U32 u32AD_Eva_1_Temp_Sum;
U8 u8Cnt_Eva_1_Temp;
U16 gu16_Eva_First_Temperature;
U8 gu8_Eva_Cold_Temperature_One_Degree;

U16 u16Max_Room_Temp_AD;
U16 u16Min_Room_Temp_AD;
U32 u32AD_Room_Temp_Sum;
U8 u8Cnt_Room_Temp;
U16 gu16_Room_Temperature;
U8 gu8_Room_Temperature_One_Degree;


U16 gu16_AD_Result_Hot_Out;
U16 u16Max_Hot_Temp_AD;
U16 u16Min_Hot_Temp_AD;
U32 u32AD_Hot_Temp_Sum;
U8 u8Cnt_Hot_Temp;
U16 gu16_Hot_Out_Temperature;
U8 gu8_Hot_Out_Temperature_One_Degree;

U16 u16Max_Ice_Full_Temp_AD;
U16 u16Min_Ice_Full_Temp_AD;
U32 u32AD_Ice_Full_Temp_Sum;
U8 u8Cnt_Ice_Full_Temp;

U16 u16Max_Leakage_AD;
U16 u16Min_Leakage_AD;
U32 u32AD_Leakage_Sum;
U8 u8Cnt_Leakage;

U16 u16Max_CDS_AD;
U16 u16Min_CDS_AD;
U32 u32AD_CDS_Sum;
U8 u8Cnt_CDS;

U16 gu16_AD_Result_Fan_Current;
U16 gu16Max_Fan_Current_Data_AD;
U16 u16Min_Fan_Current_Data_AD;
U32 u32AD_Fan_Current_Sum;
U8 u8Cnt_Fan_Current_Data;

U16 gu16_AD_Result_UV_Ice_Tank_1_2_Current;
U16 gu16Max_UV_Ice_Tank_1_2_Current_Feed_AD;
U16 gu16Min_UV_Ice_Tank_1_2_Current_Feed_AD;
U32 u32AD_UV_Ice_Tank_1_2_Current_Feed_Sum;
U8 u8Cnt_UV_Ice_Tank_1_2_Current_Feed;

U16 gu16_AD_Result_UV_Ice_Faucet_One_Current;
U16 gu16Max_UV_Ice_Faucet_One_Current_Feed_AD;
U16 gu16Min_UV_Ice_Faucet_One_Current_Feed_AD;
U32 u32AD_UV_Ice_Faucet_One_Current_Feed_Sum;
U8 u8Cnt_UV_Ice_Faucet_One_Current_Feed;

U16 gu16_AD_Result_Hot_Tank_Temp;
U16 u16Max_Hot_Tank_Temp_AD;
U16 u16Min_Hot_Tank_Temp_AD;
U32 u32AD_Hot_Tank_Temp_Sum;
U8 u8Cnt_Hot_Tank_Temp;
U8 gu8_Hot_Tank_Temperature_One_Degree;
U16 gu16_Hot_Tank_Temperature;

U16 gu16_AD_Result_DC_Current_24V;
U16 gu16Max_DC_Current_24V_AD;
U16 gu16Min_DC_Current_24V_AD;
U32 gu32AD_DC_Current_24V_Sum;
U8 u8Cnt_DC_Current_24V;

U16 gu16_AD_Result_DC_Current_12V;
U16 gu16Max_DC_Current_12V_AD;
U16 u16Min_DC_Current_12V_AD;
U32 u32AD_DC_Current_12V_Sum;
U8 u8Cnt_DC_Current_12V;

U16 gu16_AD_Result_UV_Ice_Tray_1_2_Current_Feed;
U16 gu16Max_UV_Ice_Tray_1_2_Current_Feed_AD;
U16 gu16Min_UV_Ice_Tray_1_2_Current_Feed_AD;
U32 u32AD_UV_Ice_Tray_1_2_Current_Feed_Sum;
U8 u8Cnt_UV_Ice_Tray_1_2_Current_Feed;


U16 gu16_AD_Result_UV_Water_Tank_1_2_Current_Feed;
U16 gu16Max_UV_Water_Tank_Current_Feed_AD;
U16 gu16Min_UV_Water_Tank_Current_Feed_AD;
U32 u32AD_UV_Water_Tank_Current_Feed_Sum;
U8 u8Cnt_UV_Water_Tank_Current_Feed;

U16 gu16_AD_Result_Amb_Side;
U16 u16Max_Amb_Side_Temp_AD;
U16 u16Min_Amb_Side_Temp_AD;
U32 u32AD_Amb_Side_Temp_Sum;
U8 u8Cnt_Amb_Side_Temp;
U16 gu16_Amb_Side_Temperature;
U8 gu8_Amb_Side_Temperature_One_Degree;

U16 gu16_AD_Result_UV_Ice_Faucet_Two_Current;
U16 gu16Max_UV_Ice_Faucet_Two_Current_AD;
U16 gu16Min_UV_Ice_Faucet_Two_Current_AD;
U32 u32AD_UV_Ice_Faucet_Two_Current_Sum;
U8 u8Cnt_UV_Ice_Faucet_Two_Current;

U16 gu16_AD_Result_Heater_Current_Feed;
U16 gu16Max_Heater_Current_Feed_AD;
U16 gu16Min_Heater_Current_Feed_AD;
U32 u32AD_Heater_Current_Feed_Sum;
U8 u8Cnt_Heater_Current_Feed;

U16 gu16_AD_Result_Hot_Heater;
U16 u16Max_Hot_Heater_Temp_AD;
U16 u16Min_Hot_Heater_Temp_AD;
U32 u32AD_Hot_Heater_Temp_Sum;
U8 u8Cnt_Hot_Heater_Temp;
U16 gu16_Hot_Heater_Temperature;
U8 gu8_Hot_Heater_Temperature_One_Degree;

U16 gu16_AD_Result_Hot_In;
U16 gu16Max_Hot_In_Temp_AD;
U16 gu16Min_Hot_In_Temp_AD;
U32 gu32AD_Hot_In_Temp_Sum;
U8 gu8Cnt_Hot_In_Temp;
U16 gu16_Hot_In_Temperature;
U8 gu8_Hot_In_Temperature_One_Degree;

U16 gu16_AD_Result_UV_Water_Faucet_Current_Feed;
U16 gu16Max_UV_Water_Faucet_Current_Feed_AD;
U16 gu16Min_UV_Water_Faucet_Current_Feed_AD;
U32 u32AD_UV_Water_Faucet_Current_Feed_Sum;
U8 u8Cnt_UV_Water_Faucet_Current_Feed;

U16 u16Max_Ice_Low_Temp_AD;
U16 u16Min_Ice_Low_Temp_AD;
U32 u32AD_Ice_Low_Temp_Sum;
U8 u8Cnt_Ice_Low_Temp;
U16 gu16ADIceLow;

U16 gu16Max_UV_Ice_Tank_3_Current_Feed_AD;
U16 gu16Min_UV_Ice_Tank_3_Current_Feed_AD;
U32 u32AD_UV_Ice_Tank_3_Current_Feed_Sum;
U8 u8Cnt_UV_Ice_Tank_3_Current_Feed;
U16 gu16_AD_Result_UV_Ice_Tank_3_Current_Feed;

U16 gu16Max_UV_Ice_Tray_1_2_Current_Feed_AD;
U16 gu16Min_UV_Ice_Tray_1_2_Current_Feed_AD;
U32 u32AD_UV_Ice_Tray_1_2_Current_Feed_Sum;
U8 u8Cnt_UV_Ice_Tray_1_2_Current_Feed;
U16 gu16_AD_Result_UV_Ice_Tray_1_2_Current_Feed;

// [25-02-20 13:36:42] yspark
U16 Testgu16ADHeaterMax = 0;

/* Ice Door Heater ADC Values 250228 CH.PARK */
U16 gu16_AD_Result_IceDoor_Heater_Current;
U16 gu16Max_IceDoor_Heater_Current_Data_AD;
U16 u16Min_IceDoor_Heater_Current_Data_AD;
U32 u32AD_IceDoor_Heater_Current_Sum;
U8 u8Cnt_IceDoor_Heater_Current_Data;
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void AD_Conversion(void)
{
    U16 ad_result = 0;
    U8 mu8_ad_channel;

    u8AdCount++;

    switch(u8AdCount)
    {
        case AD_Count_1:
                /* 24V ���� �Է� �ǵ�� */
               ad_result = get_adc_value( (U8)AD_Channel_DC_Current_24V,
                                           AD_CONV_10BIT);

               filtering_dc_current_24V_adc(ad_result);
               break;

        case AD_Count_2:
                /* 12V ���� �Է� �ǵ�� */
               ad_result = get_adc_value( (U8)AD_Channel_DC_Current_12V,
                                           AD_CONV_10BIT);

               filtering_dc_current_12V_adc(ad_result);

               break;

        case AD_Count_3:
               /*..hui [17-11-24���� 2:38:16] ����..*/
               ad_result = get_adc_value( (U8)AD_Channel_Water_Leakage,
                                           AD_CONV_10BIT);

               filtering_leakage_adc(ad_result);
               break;

        case AD_Count_4:
                /*..hui [24-12-16���� 1:10:52] �¼� ���� �µ�����..*/
                ad_result = get_adc_value( (U8)AD_Channel_Hot_Heater_Temp,
                                            AD_CONV_10BIT);

                filtering_hot_heater_adc(ad_result);

                gu16_Hot_Heater_Temperature = ConvAdc2Temp_HotOutWater(gu16_AD_Result_Hot_Heater);
                
                /* �ŷڼ��� ������ */
                gu8_Hot_Heater_Temperature_One_Degree = (U8)((gu16_Hot_Heater_Temperature + 5) / 10);
               
               break;

        case AD_Count_5:
               /*..hui [24-12-16���� 1:10:59] �¼� ���� �µ�����..*/
                ad_result = get_adc_value( (U8)AD_Channel_Hot_Out_Temp,
                                            AD_CONV_10BIT );

                filtering_hot_temp_adc(ad_result);

                gu16_Hot_Out_Temperature = ConvAdc2Temp_HotOutWater(gu16_AD_Result_Hot_Out);
                if(bit_ice_tank_ster_start == CLEAR)
                {
                    gu8_Hot_Out_Temperature_One_Degree = (U8)((gu16_Hot_Out_Temperature + 5) / 10);
                }
                else {  }
            break;

        case AD_Count_6:
               /*..hui [24-12-16���� 1:11:06] �¼� �Լ� �µ�����..*/
               ad_result = get_adc_value( (U8)AD_Channel_Hot_In_Temp,
                                           AD_CONV_10BIT );

               filtering_hot_in_temp_adc(ad_result);

               gu16_Hot_In_Temperature = ConvAdc2Temp_HotInWater(gu16_AD_Result_Hot_In);

               gu8_Hot_In_Temperature_One_Degree = (U8)((gu16_Hot_In_Temperature + 5) / 10);
               break;

        case AD_Count_7:
               /*..hui [17-11-24���� 2:37:48] �ܱ� �µ����� 1..*/
               ad_result = get_adc_value( (U8)AD_Channel_Amb_Temp,
                                           AD_CONV_10BIT );


               filtering_amb_temp_adc(ad_result);

               gu16_Amb_Front_Temperature = ConvAdc2Temp_Ambient(gu16_AD_Result_Amb);
               gu8_Amb_Front_Temperature_One_Degree = (U8)((gu16_Amb_Front_Temperature + 5) / 10);
               break;

        case AD_Count_8:
               /*..hui [17-11-24���� 2:37:56] ���� �µ�����..*/
               ad_result = get_adc_value( (U8)AD_Channel_EVA_1_TEMP,
                                           AD_CONV_10BIT );

               filtering_eva_1_temp_adc(ad_result);

               /*..hui [23-6-30���� 5:45:47] �������� EVA �µ� 1��.. ������..*/
               gu16_Eva_First_Temperature = ConvAdc2Temp_Eva(gu16_AD_Result_Eva_First);
               
               gu8_Eva_Cold_Temperature_One_Degree = (U8)((gu16_Eva_First_Temperature + 5) / 10);
               break;

        case AD_Count_9:
               /*..hui [17-11-24���� 2:38:01] ���� �µ�����..*/
               ad_result = get_adc_value( (U8)AD_Channel_Room_Temp,
                                           AD_CONV_10BIT );

               if( bit_adc_room_start == SET && pROOM_TH_POWER == SET)
               {
                   filtering_room_temp_adc(ad_result);

                   gu16_Room_Temperature = ConvAdc2Temp_RoomWater(gu16_AD_Result_Room);
                   
                   gu8_Room_Temperature_One_Degree = (U8)((gu16_Room_Temperature + 5) / 10);

                    if(bit_ice_tank_ster_start == SET)
                    {
                        gu8_Hot_Out_Temperature_One_Degree = gu8_Room_Temperature_One_Degree;                        
                    }
               }
               else{}

               break;

        case AD_Count_10:
               /*..hui [17-11-24���� 2:37:41] �ü� �µ�����..*/
               ad_result = get_adc_value( (U8)AD_Channel_Cold_Temp,
                                           AD_CONV_10BIT );

               if( bit_adc_cold_start == SET && pCOLD_TH_POWER == SET )
               {
                   filtering_cold_temp_adc(ad_result);

                   gu16_Cold_Temperature = ConvAdc2Temp_ColdWater(gu16_AD_Result_Cold);
                   gu8_Cold_Temperature_One_Degree = (U8)((gu16_Cold_Temperature + 5) / 10);
               }
               else{}

               break;

        case AD_Count_11:
               /*..hui [24-12-16���� 1:11:28] �巹�� ���� ����..*/
               ad_result = get_adc_value( (U8)AD_Channel_Drain_Pump_Current,
                                           AD_CONV_10BIT );

               filtering_drain_pump_current_adc(ad_result);

               break;

        case AD_Count_12:
               /*..hui [25-1-24���� 5:29:03] ��� �Ŀ�� UV..*/
               ad_result = get_adc_value( (U8)AD_Channel_UV_Water_Faucet_Current,
                                           AD_CONV_10BIT);

               filtering_uv_water_faucet_current_feed_adc(ad_result);

               break;

        case AD_Count_13:
               /*..hui [25-1-24���� 5:29:12] ���� �Ŀ�� UV 1..*/
               ad_result = get_adc_value( (U8)AD_Channel_UV_Ice_Faucet_1_Current,
                                           AD_CONV_10BIT);

               filtering_uv_ice_faucet_one_current_feed_adc(ad_result);

               break;

        case AD_Count_14:
               /*..hui [25-1-24���� 5:29:20] ���� �Ŀ�� UV2..*/
               ad_result = get_adc_value( (U8)AD_Channel_UV_Ice_Faucet_2_Current,
                                           AD_CONV_10BIT);

               filtering_uv_ice_faucet_two_current_adc(ad_result);

               break;

        case AD_Count_15:
               /*..hui [25-1-24���� 5:29:30] ������ũ UV 1,2..*/
               ad_result = get_adc_value( (U8)AD_Channel_UV_Ice_Tank_1_2_Current,
                                           AD_CONV_10BIT);

               filtering_uv_ice_tank_1_2_current_feed_adc(ad_result);

               break;

        case AD_Count_16:
               /*..hui [17-11-24���� 2:38:10] ���� ����..*/
               ad_result = get_adc_value( (U8)AD_Channel_Ice_Full_Receive_1,
                                           AD_CONV_10BIT);

               filtering_ice_full_adc(ad_result);

               break;

        case AD_Count_17:
               /*..hui [25-1-24���� 5:30:27] ���̽�Ʈ���� UV1,2..*/
               ad_result = get_adc_value( (U8)AD_Channel_UV_Ice_Tray_1_2_Current,
                                           AD_CONV_10BIT);

               filtering_uv_ice_tray_1_2_current_feed_adc(ad_result);

               break;


        case AD_Count_18:
               /*..hui [24-12-16���� 1:12:15] ���� ���� �ǵ��..*/
               ad_result = get_adc_value( (U8)AD_Channel_Hot_Heater_Current,
                                           AD_CONV_10BIT);

               filtering_heater_current_feed_adc(ad_result);
               break;

        case AD_Count_19:
               /*..hui [24-12-16���� 1:12:25] �ܱ� �µ����� 2 ..*/
               ad_result = get_adc_value( (U8)AD_Channel_Amb_2_Temp,
                                           AD_CONV_10BIT );

               filtering_amb_side_temp_adc(ad_result);

               gu16_Amb_Side_Temperature = ConvAdc2Temp_Ambient(gu16_AD_Result_Amb_Side);
               gu8_Amb_Side_Temperature_One_Degree = (U8)((gu16_Amb_Side_Temperature + 5) / 10);

               break;

        case AD_Count_20:
               /*..hui [24-12-16���� 1:12:31] �� �ǵ�� ����..*/
               ad_result = get_adc_value( (U8)AD_Channel_FAN_Current,
                                           AD_CONV_10BIT );

               filtering_fan_current_adc(ad_result);
               
               break;

        case AD_Count_21:
                /* ������ũ �������� �߰� 250210 @CH.PARK */
                ad_result = get_adc_value( (U8)AD_Channel_Ice_Full_Receive_2,
                                           AD_CONV_10BIT );

                filtering_ice_Low_adc(ad_result);

                break;

        case AD_Count_22:
                /* ������ũ ���� ���� ���� �ǵ�� �߰� 250210 @CH.PARK */
                ad_result = get_adc_value( (U8)AD_Channel_Ice_Door_Heater_IAD,
                                           AD_CONV_10BIT );
                
                filtering_heater_ice_door_current_adc(ad_result);
                break;

        case AD_Count_23:
                /* ��������� UV LED �����ǵ�� 250225 CH.PARK */
                ad_result = get_adc_value( (U8)AD_Channel_UV_Ice_Tank_3_Current,
                                            AD_CONV_10BIT);

                FILTERING_UV_ICE_TANK3_CURRENT_FEED_ADC(ad_result);
                u8AdCount = 0;
            break;

        default:

               ADS = AD_Channel_DC_Current_24V;
               u8AdCount = 0;

               break;
    }
}
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U16 get_adc_value( U8 ad_channel, U8 resolution)
{
    U16 mu16_ad_result = 0;
    U16 mu16_final_ad_value = 0;

    ADC_Stop();

    ADS = ad_channel;
    ADCE = 1U;  /* enable AD comparator */

    ADC_Start();

    while(ADIF != SET){}

    ADC_Stop();
    ADCE = 0U;  /* disable AD comparator */

    mu16_ad_result = (U16)(ADCR >> resolution);

    /*..hui [17-11-24���� 2:11:20] ACD�� ���͸�....*/
    /*mu16_final_ad_value = filtering_adc_value( ad_old_value,
                                          mu16_ad_result );  */

    /*return mu16_final_ad_value;*/

    return mu16_ad_result;
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void filtering_dc_current_24V_adc( U16 mu16ad_result )
{
    if(mu16ad_result > gu16Max_DC_Current_24V_AD)    //Max Save
    {
        gu16Max_DC_Current_24V_AD = mu16ad_result;
    }

    if(mu16ad_result < gu16Min_DC_Current_24V_AD)  //Min   Save
    {
        gu16Min_DC_Current_24V_AD = mu16ad_result;
    }

    gu32AD_DC_Current_24V_Sum += mu16ad_result;

    if(++u8Cnt_DC_Current_24V >= 12)                   // Sum = count +   Max +   Min
    {
        u8Cnt_DC_Current_24V = 0;
        gu16_AD_Result_DC_Current_24V
            = (U16)(gu32AD_DC_Current_24V_Sum - gu16Max_DC_Current_24V_AD - gu16Min_DC_Current_24V_AD) / 10;

        gu32AD_DC_Current_24V_Sum  = 0;
        gu16Max_DC_Current_24V_AD = 0;
        gu16Min_DC_Current_24V_AD = 1024;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void filtering_dc_current_12V_adc( U16 mu16ad_result )
{
    if(mu16ad_result > gu16Max_DC_Current_12V_AD)    //Max Save
    {
        gu16Max_DC_Current_12V_AD = mu16ad_result;
    }

    if(mu16ad_result < u16Min_DC_Current_12V_AD)  //Min   Save
    {
        u16Min_DC_Current_12V_AD = mu16ad_result;
    }

    u32AD_DC_Current_12V_Sum += mu16ad_result;

    if(++u8Cnt_DC_Current_12V >= 12)                   // Sum = count +   Max +   Min
    {
        u8Cnt_DC_Current_12V = 0;
        gu16_AD_Result_DC_Current_12V
            = (U16)(u32AD_DC_Current_12V_Sum - gu16Max_DC_Current_12V_AD - u16Min_DC_Current_12V_AD) / 10;

        /*gu16_AD_Result_DC_Current_12V = 1024 - gu16_AD_Result_DC_Current_12V;*/

        u32AD_DC_Current_12V_Sum  = 0;
        gu16Max_DC_Current_12V_AD = 0;
        u16Min_DC_Current_12V_AD = 1024;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void filtering_drain_pump_current_adc( U16 mu16ad_result )
{
    if(mu16ad_result > u16Max_Drain_Pump_Current_AD)    //Max Save
    {
        u16Max_Drain_Pump_Current_AD = mu16ad_result;
    }

    if(mu16ad_result < u16Min_Drain_Pump_Current_AD)  //Min   Save
    {
        u16Min_Drain_Pump_Current_AD = mu16ad_result;
    }

    u32AD_Drain_Pump_Current_Sum += mu16ad_result;

    if(++u8Cnt_Drain_Pump_Current >= 52)                   // Sum = count +   Max +   Min
    {
        u8Cnt_Drain_Pump_Current = 0;

        gu16_AD_Drain_Pump_Current = (U16)(u32AD_Drain_Pump_Current_Sum - u16Max_Drain_Pump_Current_AD - u16Min_Drain_Pump_Current_AD) / 50;

        u32AD_Drain_Pump_Current_Sum  = 0;
        u16Max_Drain_Pump_Current_AD = 0;
        u16Min_Drain_Pump_Current_AD = 1024;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void filtering_hot_tank_temp_adc( U16 mu16ad_result )
{
    if(mu16ad_result > u16Max_Hot_Tank_Temp_AD)    //Max Save
    {
        u16Max_Hot_Tank_Temp_AD = (U16)mu16ad_result;
    }

    if(mu16ad_result < u16Min_Hot_Tank_Temp_AD)  //Min   Save
    {
        u16Min_Hot_Tank_Temp_AD = (U16)mu16ad_result;
    }

    u32AD_Hot_Tank_Temp_Sum += (U16)mu16ad_result;

    if(++u8Cnt_Hot_Tank_Temp >= 52)                   // Sum = count +   Max +   Min
    {
        u8Cnt_Hot_Tank_Temp = 0;
        gu16_AD_Result_Hot_Tank_Temp = (U16)(u32AD_Hot_Tank_Temp_Sum - u16Max_Hot_Tank_Temp_AD - u16Min_Hot_Tank_Temp_AD) / 50;
        /*..hui [23-2-8���� 4:07:44] Ǯ�ٿ��� Ǯ������ ����...*/
        gu16_AD_Result_Hot_Tank_Temp = 1024 - gu16_AD_Result_Hot_Tank_Temp;

        u32AD_Hot_Tank_Temp_Sum  = 0;
        u16Max_Hot_Tank_Temp_AD = 0;
        u16Min_Hot_Tank_Temp_AD = 1024;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U16 ConvAdc2Temp_HotTankWater(U16 mu16Adc)
{
    U16 mu16Index;

    if(  mu16Adc < MIN_ADC_HOT_WATER )
    {
        return 0;  // min temperture..
    }

    if( mu16Adc > MAX_ADC_HOT_WATER )
    {
        return 1000;  // max temperture..
    }

    mu16Index = mu16Adc - MIN_ADC_HOT_WATER;
    return temp_hot_tank_water_table[ mu16Index ];
}




/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void filtering_hot_pump_current_adc( U16 mu16ad_result )
{

    if(mu16ad_result > u16Max_Hot_Pump_Current_AD)    //Max Save
    {
        u16Max_Hot_Pump_Current_AD = mu16ad_result;
    }

    if(mu16ad_result < u16Min_Hot_Pump_Current_AD)  //Min   Save
    {
        u16Min_Hot_Pump_Current_AD = mu16ad_result;
    }

    u32AD_Hot_Pump_Current_Sum += mu16ad_result;

    if(++u8Cnt_Hot_Pump_Current >= 52)                   // Sum = count +   Max +   Min
    {
        u8Cnt_Hot_Pump_Current = 0;
        gu16_AD_Tray_Pump_Current = (U16)(u32AD_Hot_Pump_Current_Sum - u16Max_Hot_Pump_Current_AD - u16Min_Hot_Pump_Current_AD) / 50;

        u32AD_Hot_Pump_Current_Sum  = 0;
        u16Max_Hot_Pump_Current_AD = 0;
        u16Min_Hot_Pump_Current_AD = 1024;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void  filtering_uv_water_tank_current_feed_adc( U16 mu16ad_result )
{
    if(mu16ad_result > gu16Max_UV_Water_Tank_Current_Feed_AD)    //Max Save
    {
        gu16Max_UV_Water_Tank_Current_Feed_AD = mu16ad_result;
    }

    if(mu16ad_result < gu16Min_UV_Water_Tank_Current_Feed_AD)  //Min   Save
    {
        gu16Min_UV_Water_Tank_Current_Feed_AD = mu16ad_result;
    }

    u32AD_UV_Water_Tank_Current_Feed_Sum += mu16ad_result;

    if(++u8Cnt_UV_Water_Tank_Current_Feed >= 52)                   // Sum = count +   Max +   Min
    {
        u8Cnt_UV_Water_Tank_Current_Feed = 0;
        gu16_AD_Result_UV_Water_Tank_1_2_Current_Feed
            = (U16)(u32AD_UV_Water_Tank_Current_Feed_Sum - gu16Max_UV_Water_Tank_Current_Feed_AD - gu16Min_UV_Water_Tank_Current_Feed_AD) / 50;

        u32AD_UV_Water_Tank_Current_Feed_Sum  = 0;
        gu16Max_UV_Water_Tank_Current_Feed_AD = 0;
        gu16Min_UV_Water_Tank_Current_Feed_AD = 1024;
    }

}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void filtering_uv_water_faucet_current_feed_adc( U16 mu16ad_result )
{
    if(mu16ad_result > gu16Max_UV_Water_Faucet_Current_Feed_AD)    //Max Save
    {
        gu16Max_UV_Water_Faucet_Current_Feed_AD = mu16ad_result;
    }

    if(mu16ad_result < gu16Min_UV_Water_Faucet_Current_Feed_AD)  //Min   Save
    {
        gu16Min_UV_Water_Faucet_Current_Feed_AD = mu16ad_result;
    }

    u32AD_UV_Water_Faucet_Current_Feed_Sum += mu16ad_result;

    if(++u8Cnt_UV_Water_Faucet_Current_Feed >= 52)                   // Sum = count +   Max +   Min
    {
        u8Cnt_UV_Water_Faucet_Current_Feed = 0;
        
        gu16_AD_Result_UV_Water_Faucet_Current_Feed
        = (U16)(u32AD_UV_Water_Faucet_Current_Feed_Sum - gu16Max_UV_Water_Faucet_Current_Feed_AD - gu16Min_UV_Water_Faucet_Current_Feed_AD) / 50;

        u32AD_UV_Water_Faucet_Current_Feed_Sum  = 0;
        gu16Max_UV_Water_Faucet_Current_Feed_AD = 0;
        gu16Min_UV_Water_Faucet_Current_Feed_AD = 1024;
    }

}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void filtering_uv_ice_faucet_one_current_feed_adc( U16 mu16ad_result )
{
    if(mu16ad_result > gu16Max_UV_Ice_Faucet_One_Current_Feed_AD)    //Max Save
    {
        gu16Max_UV_Ice_Faucet_One_Current_Feed_AD = mu16ad_result;
    }

    if(mu16ad_result < gu16Min_UV_Ice_Faucet_One_Current_Feed_AD)  //Min   Save
    {
        gu16Min_UV_Ice_Faucet_One_Current_Feed_AD = mu16ad_result;
    }

    u32AD_UV_Ice_Faucet_One_Current_Feed_Sum += mu16ad_result;

    if(++u8Cnt_UV_Ice_Faucet_One_Current_Feed >= 52)                   // Sum = count +   Max +   Min
    {
        u8Cnt_UV_Ice_Faucet_One_Current_Feed = 0;

        gu16_AD_Result_UV_Ice_Faucet_One_Current
            = (U16)(u32AD_UV_Ice_Faucet_One_Current_Feed_Sum - gu16Max_UV_Ice_Faucet_One_Current_Feed_AD - gu16Min_UV_Ice_Faucet_One_Current_Feed_AD) / 50;

        u32AD_UV_Ice_Faucet_One_Current_Feed_Sum  = 0;
        gu16Max_UV_Ice_Faucet_One_Current_Feed_AD = 0;
        gu16Min_UV_Ice_Faucet_One_Current_Feed_AD = 1024;
    }

}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void filtering_uv_ice_tank_1_2_current_feed_adc( U16 mu16ad_result )
{
    if(mu16ad_result > gu16Max_UV_Ice_Tank_1_2_Current_Feed_AD)    //Max Save
    {
        gu16Max_UV_Ice_Tank_1_2_Current_Feed_AD = mu16ad_result;
    }

    if(mu16ad_result < gu16Min_UV_Ice_Tank_1_2_Current_Feed_AD)  //Min   Save
    {
        gu16Min_UV_Ice_Tank_1_2_Current_Feed_AD = mu16ad_result;
    }

    u32AD_UV_Ice_Tank_1_2_Current_Feed_Sum += mu16ad_result;

    if(++u8Cnt_UV_Ice_Tank_1_2_Current_Feed >= 52)                   // Sum = count +   Max +   Min
    {
        u8Cnt_UV_Ice_Tank_1_2_Current_Feed = 0;

        gu16_AD_Result_UV_Ice_Tank_1_2_Current
            = (U16)(u32AD_UV_Ice_Tank_1_2_Current_Feed_Sum - gu16Max_UV_Ice_Tank_1_2_Current_Feed_AD - gu16Min_UV_Ice_Tank_1_2_Current_Feed_AD) / 50;

        u32AD_UV_Ice_Tank_1_2_Current_Feed_Sum  = 0;
        gu16Max_UV_Ice_Tank_1_2_Current_Feed_AD = 0;
        gu16Min_UV_Ice_Tank_1_2_Current_Feed_AD = 1024;
    }

}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void filtering_uv_ice_tray_1_2_current_feed_adc( U16 mu16ad_result )
{
    if(mu16ad_result > gu16Max_UV_Ice_Tray_1_2_Current_Feed_AD)    //Max Save
    {
        gu16Max_UV_Ice_Tray_1_2_Current_Feed_AD = mu16ad_result;
    }

    if(mu16ad_result < gu16Min_UV_Ice_Tray_1_2_Current_Feed_AD)  //Min   Save
    {
        gu16Min_UV_Ice_Tray_1_2_Current_Feed_AD = mu16ad_result;
    }

    u32AD_UV_Ice_Tray_1_2_Current_Feed_Sum += mu16ad_result;

    if(++u8Cnt_UV_Ice_Tray_1_2_Current_Feed >= 52)                   // Sum = count +   Max +   Min
    {
        u8Cnt_UV_Ice_Tray_1_2_Current_Feed = 0;

        gu16_AD_Result_UV_Ice_Tray_1_2_Current_Feed
            = (U16)(u32AD_UV_Ice_Tray_1_2_Current_Feed_Sum - gu16Max_UV_Ice_Tray_1_2_Current_Feed_AD - gu16Min_UV_Ice_Tray_1_2_Current_Feed_AD) / 50;

        u32AD_UV_Ice_Tray_1_2_Current_Feed_Sum  = 0;
        gu16Max_UV_Ice_Tray_1_2_Current_Feed_AD = 0;
        gu16Min_UV_Ice_Tray_1_2_Current_Feed_AD = 1024;
    }

}

/**
 * @brief UV TANK3 ���� �ǵ��
 * 
 * @param mu16ad_result 
 */
void FILTERING_UV_ICE_TANK3_CURRENT_FEED_ADC( U16 mu16ad_result )
{
    if(mu16ad_result > gu16Max_UV_Ice_Tank_3_Current_Feed_AD)    //Max Save
    {
        gu16Max_UV_Ice_Tank_3_Current_Feed_AD = mu16ad_result;
    }

    if(mu16ad_result < gu16Min_UV_Ice_Tank_3_Current_Feed_AD)  //Min   Save
    {
        gu16Min_UV_Ice_Tank_3_Current_Feed_AD = mu16ad_result;
    }

    u32AD_UV_Ice_Tank_3_Current_Feed_Sum += mu16ad_result;

    if(++u8Cnt_UV_Ice_Tank_3_Current_Feed >= 52)                   // Sum = count +   Max +   Min
    {
        u8Cnt_UV_Ice_Tank_3_Current_Feed = 0;

        gu16_AD_Result_UV_Ice_Tank_3_Current_Feed
            = (U16)(u32AD_UV_Ice_Tank_3_Current_Feed_Sum - gu16Max_UV_Ice_Tank_3_Current_Feed_AD - gu16Min_UV_Ice_Tank_3_Current_Feed_AD) / 50;

        u32AD_UV_Ice_Tank_3_Current_Feed_Sum  = 0;
        gu16Max_UV_Ice_Tank_3_Current_Feed_AD = 0;
        gu16Min_UV_Ice_Tank_3_Current_Feed_AD = 1024;
    }

}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void filtering_ice_full_adc( U16 mu16ad_result )
{
    if(mu16ad_result > u16Max_Ice_Full_Temp_AD)    //Max Save
    {
        u16Max_Ice_Full_Temp_AD = mu16ad_result;
    }

    if(mu16ad_result < u16Min_Ice_Full_Temp_AD)  //Min   Save
    {
        u16Min_Ice_Full_Temp_AD = mu16ad_result;
    }

    u32AD_Ice_Full_Temp_Sum += mu16ad_result;

    if(++u8Cnt_Ice_Full_Temp >= 52)                   // Sum = count +   Max +   Min
    {
        u8Cnt_Ice_Full_Temp = 0;
        gu16ADIceFull = (U16)(u32AD_Ice_Full_Temp_Sum - u16Max_Ice_Full_Temp_AD - u16Min_Ice_Full_Temp_AD) / 50;

        u32AD_Ice_Full_Temp_Sum  = 0;
        u16Max_Ice_Full_Temp_AD = 0;
        u16Min_Ice_Full_Temp_AD = 1024;
    }

}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void filtering_ice_Low_adc( U16 mu16ad_result )
{
    if(mu16ad_result > u16Max_Ice_Low_Temp_AD)    //Max Save
    {
        u16Max_Ice_Low_Temp_AD = mu16ad_result;
    }

    if(mu16ad_result < u16Min_Ice_Low_Temp_AD)  //Min   Save
    {
        u16Min_Ice_Low_Temp_AD = mu16ad_result;
    }

    u32AD_Ice_Low_Temp_Sum += mu16ad_result;

    if(++u8Cnt_Ice_Low_Temp >= 52)                   // Sum = count +   Max +   Min
    {
        u8Cnt_Ice_Low_Temp = 0;
        gu16ADIceLow = (U16)(u32AD_Ice_Low_Temp_Sum - u16Max_Ice_Low_Temp_AD - u16Min_Ice_Low_Temp_AD) / 50;

        u32AD_Ice_Low_Temp_Sum  = 0;
        u16Max_Ice_Low_Temp_AD = 0;
        u16Min_Ice_Low_Temp_AD = 1024;
    }

}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void filtering_leakage_adc( U16 mu16ad_result )
{
    if(mu16ad_result > u16Max_Leakage_AD)    //Max Save
    {
        u16Max_Leakage_AD = mu16ad_result;
    }

    if(mu16ad_result < u16Min_Leakage_AD)  //Min   Save
    {
        u16Min_Leakage_AD = mu16ad_result;
    }

    u32AD_Leakage_Sum += mu16ad_result;

    if(++u8Cnt_Leakage >= 52)                   // Sum = count +   Max +   Min
    {
        u8Cnt_Leakage = 0;

        gu16ADLeakage = (U16)((u32AD_Leakage_Sum - u16Max_Leakage_AD - u16Min_Leakage_AD) / 50);

        u32AD_Leakage_Sum  = 0;
        u16Max_Leakage_AD = 0;
        u16Min_Leakage_AD = 1024;
    }

}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void filtering_eva_1_temp_adc( U16 mu16ad_result )
{
    if(mu16ad_result > u16Max_Eva_1_Temp_AD)    //Max Save
    {
        u16Max_Eva_1_Temp_AD = mu16ad_result;
    }

    if(mu16ad_result < u16Min_Eva_1_Temp_AD)  //Min   Save
    {
        u16Min_Eva_1_Temp_AD = mu16ad_result;
    }

    u32AD_Eva_1_Temp_Sum += mu16ad_result;

    if(++u8Cnt_Eva_1_Temp >= 52)                   // Sum = count +   Max +   Min
    {
        u8Cnt_Eva_1_Temp = 0;
        gu16_AD_Result_Eva_First = (U16)((u32AD_Eva_1_Temp_Sum - u16Max_Eva_1_Temp_AD - u16Min_Eva_1_Temp_AD) / 50);
        /*..hui [23-2-8���� 4:07:44] Ǯ�ٿ��� Ǯ������ ����...*/
        gu16_AD_Result_Eva_First = 1024 - gu16_AD_Result_Eva_First;

        u32AD_Eva_1_Temp_Sum  = 0;
        u16Max_Eva_1_Temp_AD = 0;
        u16Min_Eva_1_Temp_AD = 1024;
    }

}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U16 ConvAdc2Temp_Eva(U16 mu16Adc)
{
    U16 mu16Index;

    if(  mu16Adc < MIN_ADC_EVA_TEMP )
    {
        return 0;  // min temperture..
    }

    if( mu16Adc > MAX_ADC_EVA_TEMP )
    {
        return 1000;  // max temperture..
    }

    mu16Index = mu16Adc - MIN_ADC_EVA_TEMP;
    return temp_eva_table[ mu16Index ];
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void filtering_cold_temp_adc( U16 mu16ad_result )
{
    if(mu16ad_result > u16Max_Cold_Temp_AD)    //Max Save
    {
        u16Max_Cold_Temp_AD = mu16ad_result;
    }

    if(mu16ad_result < u16Min_Cold_Temp_AD)  //Min   Save
    {
        u16Min_Cold_Temp_AD = mu16ad_result;
    }

    u32AD_Cold_Temp_Sum += mu16ad_result;

    if(++u8Cnt_Cold_Temp >= 52)                   // Sum = count +   Max +   Min
    {
        u8Cnt_Cold_Temp = 0;
        gu16_AD_Result_Cold = (U16)((u32AD_Cold_Temp_Sum - u16Max_Cold_Temp_AD - u16Min_Cold_Temp_AD) / 50);

        u32AD_Cold_Temp_Sum  = 0;
        u16Max_Cold_Temp_AD = 0;
        u16Min_Cold_Temp_AD = 1024;
    }

}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U16 ConvAdc2Temp_ColdWater(U16 mu16Adc)
{
    U16 mu16Index;

    if(  mu16Adc < MIN_ADC_COLD_WATER )
    {
        return 900;  // max temperture..
    }

    if( mu16Adc > MAX_ADC_COLD_WATER )
    {
        return 0;  // min temperture..
    }

    mu16Index = mu16Adc - MIN_ADC_COLD_WATER;
    return temp_cold_water_table[ mu16Index ];
}




/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void filtering_room_temp_adc( U16 mu16ad_result )
{
    if(mu16ad_result > u16Max_Room_Temp_AD)    //Max Save
    {
        u16Max_Room_Temp_AD = mu16ad_result;
    }

    if(mu16ad_result < u16Min_Room_Temp_AD)  //Min   Save
    {
        u16Min_Room_Temp_AD = mu16ad_result;
    }

    u32AD_Room_Temp_Sum += mu16ad_result;

    if(++u8Cnt_Room_Temp >= 52)                   // Sum = count +   Max +   Min
    {
        u8Cnt_Room_Temp = 0;
        gu16_AD_Result_Room = (U16)((u32AD_Room_Temp_Sum - u16Max_Room_Temp_AD - u16Min_Room_Temp_AD) / 50);

        u32AD_Room_Temp_Sum  = 0;
        u16Max_Room_Temp_AD = 0;
        u16Min_Room_Temp_AD = 1024;
    }

}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U16 ConvAdc2Temp_RoomWater(U16 mu16Adc)
{
    /*return ConvAdc2Temp_ColdWater( mu16Adc );*/
    return ConvAdc2Temp_HotInWater( mu16Adc );
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void filtering_amb_temp_adc( U16 mu16ad_result )
{
    if(mu16ad_result > u16Max_Amb_Temp_AD)    //Max Save
    {
        u16Max_Amb_Temp_AD = mu16ad_result;
    }

    if(mu16ad_result < u16Min_Amb_Temp_AD)  //Min   Save
    {
        u16Min_Amb_Temp_AD = mu16ad_result;
    }

    u32AD_Amb_Temp_Sum += mu16ad_result;

    if(++u8Cnt_Amb_Temp >= 52)                   // Sum = count +   Max +   Min
    {
        u8Cnt_Amb_Temp = 0;
        gu16_AD_Result_Amb = (U16)((u32AD_Amb_Temp_Sum - u16Max_Amb_Temp_AD - u16Min_Amb_Temp_AD) / 50);
        /*..hui [23-2-8���� 4:07:44] Ǯ�ٿ��� Ǯ������ ����...*/
        gu16_AD_Result_Amb = 1024 - gu16_AD_Result_Amb;

        u32AD_Amb_Temp_Sum  = 0;
        u16Max_Amb_Temp_AD = 0;
        u16Min_Amb_Temp_AD = 1024;
    }

}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void filtering_amb_side_temp_adc( U16 mu16ad_result )
{
    if(mu16ad_result > u16Max_Amb_Side_Temp_AD)    //Max Save
    {
        u16Max_Amb_Side_Temp_AD = mu16ad_result;
    }

    if(mu16ad_result < u16Min_Amb_Side_Temp_AD)  //Min   Save
    {
        u16Min_Amb_Side_Temp_AD = mu16ad_result;
    }

    u32AD_Amb_Side_Temp_Sum += mu16ad_result;

    if(++u8Cnt_Amb_Side_Temp >= 52)                   // Sum = count +   Max +   Min
    {
        u8Cnt_Amb_Side_Temp = 0;
        gu16_AD_Result_Amb_Side = (U16)((u32AD_Amb_Side_Temp_Sum - u16Max_Amb_Side_Temp_AD - u16Min_Amb_Side_Temp_AD) / 50);
        /*..hui [23-2-8���� 4:07:44] Ǯ�ٿ��� Ǯ������ ����...*/
        gu16_AD_Result_Amb_Side = 1024 - gu16_AD_Result_Amb_Side;

        u32AD_Amb_Side_Temp_Sum  = 0;
        u16Max_Amb_Side_Temp_AD = 0;
        u16Min_Amb_Side_Temp_AD = 1024;
    }

}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U16 ConvAdc2Temp_Ambient(U16 mu16Adc)
{
    U16 mu16Index;

    if(  mu16Adc < MIN_ADC_AMBIENT_TEMP )
    {
        return 0;  // min temperture..
    }

    if( mu16Adc > MAX_ADC_AMBIENT_TEMP )
    {
        return 1000;  // max temperture..
    }

    mu16Index = mu16Adc - MIN_ADC_AMBIENT_TEMP;
    return temp_ambient_table[ mu16Index ];
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void filtering_fan_current_adc( U16 mu16ad_result )
{
    if(mu16ad_result > gu16Max_Fan_Current_Data_AD)    //Max Save
    {
        gu16Max_Fan_Current_Data_AD = mu16ad_result;
    }

    if(mu16ad_result < u16Min_Fan_Current_Data_AD)  //Min   Save
    {
        u16Min_Fan_Current_Data_AD = mu16ad_result;
    }

    u32AD_Fan_Current_Sum += mu16ad_result;

    if(++u8Cnt_Fan_Current_Data >= 22)                   // Sum = count +   Max +   Min
    {
        u8Cnt_Fan_Current_Data = 0;
        gu16_AD_Result_Fan_Current = (U16)(u32AD_Fan_Current_Sum - gu16Max_Fan_Current_Data_AD - u16Min_Fan_Current_Data_AD) / 20;

        u32AD_Fan_Current_Sum  = 0;
        gu16Max_Fan_Current_Data_AD = 0;
        u16Min_Fan_Current_Data_AD = 1024;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  : Ice Door Heater current feedback
***********************************************************************************************************************/
void filtering_heater_ice_door_current_adc( U16 mu16ad_result )
{
    if(mu16ad_result > gu16Max_IceDoor_Heater_Current_Data_AD)    //Max Save
    {
        gu16Max_IceDoor_Heater_Current_Data_AD = mu16ad_result;
    }

    if(mu16ad_result < u16Min_IceDoor_Heater_Current_Data_AD)  //Min   Save
    {
        u16Min_IceDoor_Heater_Current_Data_AD = mu16ad_result;
    }

    u32AD_IceDoor_Heater_Current_Sum += mu16ad_result;

    if(++u8Cnt_IceDoor_Heater_Current_Data >= 22)                   // Sum = count +   Max +   Min
    {
        u8Cnt_IceDoor_Heater_Current_Data = 0;
        gu16_AD_Result_IceDoor_Heater_Current = (U16)(u32AD_IceDoor_Heater_Current_Sum - gu16Max_IceDoor_Heater_Current_Data_AD - u16Min_IceDoor_Heater_Current_Data_AD) / 20;

        u32AD_IceDoor_Heater_Current_Sum  = 0;
        gu16Max_IceDoor_Heater_Current_Data_AD = 0;
        u16Min_IceDoor_Heater_Current_Data_AD = 1024;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void filtering_uv_ice_faucet_two_current_adc( U16 mu16ad_result )
{
    if(mu16ad_result > gu16Max_UV_Ice_Faucet_Two_Current_AD)    //Max Save
    {
        gu16Max_UV_Ice_Faucet_Two_Current_AD = mu16ad_result;
    }

    if(mu16ad_result < gu16Min_UV_Ice_Faucet_Two_Current_AD)  //Min   Save
    {
        gu16Min_UV_Ice_Faucet_Two_Current_AD = mu16ad_result;
    }

    u32AD_UV_Ice_Faucet_Two_Current_Sum += mu16ad_result;

    if(++u8Cnt_UV_Ice_Faucet_Two_Current >= 52)                   // Sum = count +   Max +   Min
    {
        u8Cnt_UV_Ice_Faucet_Two_Current = 0;

        gu16_AD_Result_UV_Ice_Faucet_Two_Current
            = (U16)(u32AD_UV_Ice_Faucet_Two_Current_Sum - gu16Max_UV_Ice_Faucet_Two_Current_AD - gu16Min_UV_Ice_Faucet_Two_Current_AD) / 50;

        u32AD_UV_Ice_Faucet_Two_Current_Sum  = 0;
        gu16Max_UV_Ice_Faucet_Two_Current_AD = 0;
        gu16Min_UV_Ice_Faucet_Two_Current_AD = 1024;
    }

}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void filtering_heater_current_feed_adc( U16 mu16ad_result )
{
    if(mu16ad_result > gu16Max_Heater_Current_Feed_AD)    //Max Save
    {
        gu16Max_Heater_Current_Feed_AD = mu16ad_result;
    }

    if(mu16ad_result < gu16Min_Heater_Current_Feed_AD)  //Min   Save
    {
        gu16Min_Heater_Current_Feed_AD = mu16ad_result;
    }

    u32AD_Heater_Current_Feed_Sum += mu16ad_result;

    if(++u8Cnt_Heater_Current_Feed >= 12)                   // Sum = count +   Max +   Min
    {
        u8Cnt_Heater_Current_Feed = 0;
        gu16_AD_Result_Heater_Current_Feed
            = (U16)(u32AD_Heater_Current_Feed_Sum - gu16Max_Heater_Current_Feed_AD - gu16Min_Heater_Current_Feed_AD) / 10;

        u32AD_Heater_Current_Feed_Sum  = 0;
        gu16Max_Heater_Current_Feed_AD = 0;
        gu16Min_Heater_Current_Feed_AD = 1024;
    }

}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void filtering_hot_heater_adc( U16 mu16ad_result )
{
    if(mu16ad_result > u16Max_Hot_Heater_Temp_AD)    //Max Save
    {
        u16Max_Hot_Heater_Temp_AD = (U16)mu16ad_result;
    }

    if(mu16ad_result < u16Min_Hot_Heater_Temp_AD)  //Min   Save
    {
        u16Min_Hot_Heater_Temp_AD = (U16)mu16ad_result;
    }

    u32AD_Hot_Heater_Temp_Sum += (U16)mu16ad_result;

    if(++u8Cnt_Hot_Heater_Temp >= 22)                   // Sum = count +   Max +   Min
    {
        u8Cnt_Hot_Heater_Temp = 0;
        gu16_AD_Result_Hot_Heater
            = (U16)(u32AD_Hot_Heater_Temp_Sum - u16Max_Hot_Heater_Temp_AD - u16Min_Hot_Heater_Temp_AD) / 20;

        u32AD_Hot_Heater_Temp_Sum  = 0;
        u16Max_Hot_Heater_Temp_AD = 0;
        u16Min_Hot_Heater_Temp_AD = 1024;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void filtering_hot_temp_adc( U16 mu16ad_result )
{
    if(mu16ad_result > u16Max_Hot_Temp_AD)    //Max Save
    {
        u16Max_Hot_Temp_AD = mu16ad_result;
    }

    if(mu16ad_result < u16Min_Hot_Temp_AD)  //Min   Save
    {
        u16Min_Hot_Temp_AD = mu16ad_result;
    }

    u32AD_Hot_Temp_Sum += mu16ad_result;

    if(++u8Cnt_Hot_Temp >= 22)                   // Sum = count +   Max +   Min
    {
        u8Cnt_Hot_Temp = 0;
        gu16_AD_Result_Hot_Out = (U16)((u32AD_Hot_Temp_Sum - u16Max_Hot_Temp_AD - u16Min_Hot_Temp_AD) / 20);

        u32AD_Hot_Temp_Sum  = 0;
        u16Max_Hot_Temp_AD = 0;
        u16Min_Hot_Temp_AD = 1024;
    }

}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U16 ConvAdc2Temp_HotOutWater(U16 mu16Adc)
{
    U16 mu16Index;

    if(  mu16Adc < MIN_ADC_HOT_OUT_WATER )
    {
        return 1200;  // max temperture..
    }

    if( mu16Adc > MAX_ADC_HOT_OUT_WATER )
    {
        return 0;  // min temperture..
    }

    mu16Index = mu16Adc - MIN_ADC_HOT_OUT_WATER;
    return temp_hot_out_water_table[ mu16Index ];
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void filtering_hot_in_temp_adc( U16 mu16ad_result )
{
    if(mu16ad_result > gu16Max_Hot_In_Temp_AD)    //Max Save
    {
        gu16Max_Hot_In_Temp_AD = mu16ad_result;
    }

    if(mu16ad_result < gu16Min_Hot_In_Temp_AD)  //Min   Save
    {
        gu16Min_Hot_In_Temp_AD = mu16ad_result;
    }

    gu32AD_Hot_In_Temp_Sum += mu16ad_result;

    if(++gu8Cnt_Hot_In_Temp >= 22)                   // Sum = count +   Max +   Min
    {
        gu8Cnt_Hot_In_Temp = 0;
        gu16_AD_Result_Hot_In = (U16)((gu32AD_Hot_In_Temp_Sum - gu16Max_Hot_In_Temp_AD - gu16Min_Hot_In_Temp_AD) / 20);

        gu32AD_Hot_In_Temp_Sum  = 0;
        gu16Max_Hot_In_Temp_AD = 0;
        gu16Min_Hot_In_Temp_AD = 1024;
    }

}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U16 ConvAdc2Temp_HotInWater(U16 mu16Adc)
{
    U16 mu16Index;

    if(  mu16Adc < MIN_ADC_HOT_IN_WATER )
    {
        return 900;  // max temperture..
    }

    if( mu16Adc > MAX_ADC_HOT_IN_WATER )
    {
        return 0;  // min temperture..
    }

    mu16Index = mu16Adc - MIN_ADC_HOT_IN_WATER;
    return temp_hot_in_water_table[ mu16Index ];
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/




