/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _DISPLAY_LED_WIFI_H_
#define _DISPLAY_LED_WIFI_H_

//

extern void funcition_led_output(void);
extern void temp_led_out(void);
extern void level_led_out(void);
extern void setting_led_out(void);
extern void extract_led_out(void);

extern void set_led_duty( U8 mu8_number, U8 mu8_duty );


extern TYPE_BYTE          U8FrontTempHundredB;
#define            gu8_front_temp_fnd_hundred                  U8FrontTempHundredB.byte
#define            Bit0_Front_Temp_Fnd_Hundred_1               U8FrontTempHundredB.Bit.b0
#define            Bit1_Front_Temp_Fnd_Hundred_2               U8FrontTempHundredB.Bit.b1
#define            Bit2_Front_Temp_Fnd_Hundred_3               U8FrontTempHundredB.Bit.b2
#define            Bit3_Front_Temp_Fnd_Hundred_4               U8FrontTempHundredB.Bit.b3
#define            Bit4_Front_Temp_Fnd_Hundred_5               U8FrontTempHundredB.Bit.b4
#define            Bit5_Front_Temp_Fnd_Hundred_6               U8FrontTempHundredB.Bit.b5
#define            Bit6_Front_Temp_Fnd_Hundred_7               U8FrontTempHundredB.Bit.b6

extern TYPE_BYTE          U8FrontTempTenB;
#define            gu8_front_temp_fnd_ten                      U8FrontTempTenB.byte
#define            Bit0_Front_Temp_Fnd_Ten_1                   U8FrontTempTenB.Bit.b0
#define            Bit1_Front_Temp_Fnd_Ten_2                   U8FrontTempTenB.Bit.b1
#define            Bit2_Front_Temp_Fnd_Ten_3                   U8FrontTempTenB.Bit.b2
#define            Bit3_Front_Temp_Fnd_Ten_4                   U8FrontTempTenB.Bit.b3
#define            Bit4_Front_Temp_Fnd_Ten_5                   U8FrontTempTenB.Bit.b4
#define            Bit5_Front_Temp_Fnd_Ten_6                   U8FrontTempTenB.Bit.b5
#define            Bit6_Front_Temp_Fnd_Ten_7                   U8FrontTempTenB.Bit.b6

extern TYPE_BYTE          U8FrontTempOneB;
#define            gu8_front_temp_fnd_one                      U8FrontTempOneB.byte
#define            Bit0_Front_Temp_Fnd_One_1                   U8FrontTempOneB.Bit.b0
#define            Bit1_Front_Temp_Fnd_One_2                   U8FrontTempOneB.Bit.b1
#define            Bit2_Front_Temp_Fnd_One_3                   U8FrontTempOneB.Bit.b2
#define            Bit3_Front_Temp_Fnd_One_4                   U8FrontTempOneB.Bit.b3
#define            Bit4_Front_Temp_Fnd_One_5                   U8FrontTempOneB.Bit.b4
#define            Bit5_Front_Temp_Fnd_One_6                   U8FrontTempOneB.Bit.b5
#define            Bit6_Front_Temp_Fnd_One_7                   U8FrontTempOneB.Bit.b6

/////////////////////////////////////////////////////////////////////////////////////////////

extern TYPE_BYTE          U8FrontAmountHundredB;
#define            gu8_front_amount_fnd_hundred                U8FrontAmountHundredB.byte
#define            Bit0_Front_Amount_Fnd_Hundred_1             U8FrontAmountHundredB.Bit.b0
#define            Bit1_Front_Amount_Fnd_Hundred_2             U8FrontAmountHundredB.Bit.b1
#define            Bit2_Front_Amount_Fnd_Hundred_3             U8FrontAmountHundredB.Bit.b2
#define            Bit3_Front_Amount_Fnd_Hundred_4             U8FrontAmountHundredB.Bit.b3
#define            Bit4_Front_Amount_Fnd_Hundred_5             U8FrontAmountHundredB.Bit.b4
#define            Bit5_Front_Amount_Fnd_Hundred_6             U8FrontAmountHundredB.Bit.b5
#define            Bit6_Front_Amount_Fnd_Hundred_7             U8FrontAmountHundredB.Bit.b6

extern TYPE_BYTE          U8FrontAmountTenB;
#define            gu8_front_amount_fnd_ten                    U8FrontAmountTenB.byte
#define            Bit0_Front_Amount_Fnd_Ten_1                 U8FrontAmountTenB.Bit.b0
#define            Bit1_Front_Amount_Fnd_Ten_2                 U8FrontAmountTenB.Bit.b1
#define            Bit2_Front_Amount_Fnd_Ten_3                 U8FrontAmountTenB.Bit.b2
#define            Bit3_Front_Amount_Fnd_Ten_4                 U8FrontAmountTenB.Bit.b3
#define            Bit4_Front_Amount_Fnd_Ten_5                 U8FrontAmountTenB.Bit.b4
#define            Bit5_Front_Amount_Fnd_Ten_6                 U8FrontAmountTenB.Bit.b5
#define            Bit6_Front_Amount_Fnd_Ten_7                 U8FrontAmountTenB.Bit.b6

extern TYPE_BYTE          U8FrontAmountOneB;
#define            gu8_front_amount_fnd_one                    U8FrontAmountOneB.byte
#define            Bit0_Front_Amount_Fnd_One_1                 U8FrontAmountOneB.Bit.b0
#define            Bit1_Front_Amount_Fnd_One_2                 U8FrontAmountOneB.Bit.b1
#define            Bit2_Front_Amount_Fnd_One_3                 U8FrontAmountOneB.Bit.b2
#define            Bit3_Front_Amount_Fnd_One_4                 U8FrontAmountOneB.Bit.b3
#define            Bit4_Front_Amount_Fnd_One_5                 U8FrontAmountOneB.Bit.b4
#define            Bit5_Front_Amount_Fnd_One_6                 U8FrontAmountOneB.Bit.b5
#define            Bit6_Front_Amount_Fnd_One_7                 U8FrontAmountOneB.Bit.b6

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

extern TYPE_BYTE          U8FrontSelectLEDB;
#define            gu8_front_select_led                        U8FrontSelectLEDB.byte
#define            Bit0_Front_Led_Set_Select                   U8FrontSelectLEDB.Bit.b0
#define            Bit1_Front_Led_Ice_Select                   U8FrontSelectLEDB.Bit.b1
#define            Bit2_Front_Led_Hot_Select                   U8FrontSelectLEDB.Bit.b2
#define            Bit3_Front_Led_Ambient_Select               U8FrontSelectLEDB.Bit.b3
#define            Bit4_Front_Led_Cold_Select                  U8FrontSelectLEDB.Bit.b4
#define            Bit5_Front_Led_Amount_Select                U8FrontSelectLEDB.Bit.b5

extern TYPE_BYTE          U8FrontSettingLEDB;
#define            gu8_front_setting_led                       U8FrontSettingLEDB.byte
#define            Bit0_Front_Led_Ice_First                    U8FrontSettingLEDB.Bit.b0
#define            Bit1_Front_Led_Ice_Lock                     U8FrontSettingLEDB.Bit.b1
#define            Bit2_Front_Led_Hot_Lock                     U8FrontSettingLEDB.Bit.b2
#define            Bit3_Front_Led_Touch_Lock                   U8FrontSettingLEDB.Bit.b3
#define            Bit4_Front_Led_Eco_Mode                     U8FrontSettingLEDB.Bit.b4    //

//revised
extern TYPE_BYTE          U8TimeNWifiSettingLEDB;
#define            gu8_time_wifi_setting_led				   U8TimeNWifiSettingLEDB.byte
#define            Bit0_Time_Led_AM							   U8TimeNWifiSettingLEDB.Bit.b0
#define            Bit1_Time_Led_PM							   U8TimeNWifiSettingLEDB.Bit.b1
#define            Bit2_Time_Led_Colon						   U8TimeNWifiSettingLEDB.Bit.b2
#define            Bit3_Led_Wifi_Icon_White					   U8TimeNWifiSettingLEDB.Bit.b3
#define            Bit4_Led_Wifi_Icon_Blue					   U8TimeNWifiSettingLEDB.Bit.b4


extern TYPE_BYTE          U8FrontUVCareLEDB;
#define            gu8_front_uv_care_txt_led                   U8FrontUVCareLEDB.byte
#define            Bit0_Front_Led_Ice_Tank_Clean_Text          U8FrontUVCareLEDB.Bit.b0
#define            Bit1_Front_Led_UV_Care_Txt_Under_bar        U8FrontUVCareLEDB.Bit.b1
#define            Bit2_Front_Led_Water_Tank_Text              U8FrontUVCareLEDB.Bit.b2
#define            Bit3_Front_Led_Ice_Tray_Text                U8FrontUVCareLEDB.Bit.b3
#define            Bit4_Front_Led_Ice_Tank_Text                U8FrontUVCareLEDB.Bit.b4

//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////


extern TYPE_WORD          U16ButtonIndicatorB;
#define            gu8_bitton_indicator                        U16ButtonIndicatorB.word
#define            gu8_button_indicator_L                      U16ButtonIndicatorB.byte[0]
#define            gu8_button_indicator_H                      U16ButtonIndicatorB.byte[1]
#define            Bit0_Ice_Lock_Indicator                     U16ButtonIndicatorB.Bit.b0
#define            Bit1_Hot_Lock_Indicator                     U16ButtonIndicatorB.Bit.b1
#define            Bit2_Cold_Off_Indicator                     U16ButtonIndicatorB.Bit.b2
#define            Bit3_Hot_Off_Indicator                      U16ButtonIndicatorB.Bit.b3
#define            Bit4_All_Lock_Indicator                     U16ButtonIndicatorB.Bit.b4
#define            Bit5_Ice_Off_Indicator                      U16ButtonIndicatorB.Bit.b6
#define            Bit6_Icewater_Off_Indicator                 U16ButtonIndicatorB.Bit.b7
#define            Bit7_water_extract_Indicator                U16ButtonIndicatorB.Bit.b8

//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

extern LED_STEP gu8_Led_Display_Step;

extern U8 gu8_Wifi_Pairing_State;
extern U8 gu8_Wifi_Connect_State;

extern U8 gu8_wifi_disp_timer;

extern U16 gu16_wifi_pairing_30min_timer;
extern U8 gu8_wifi_pairing_5sec_timer;
extern bit bit_pairing_5s_display_start;
extern U8 top_small_led_percent;

extern U8 gu8_wifi_ap_mode;
extern U8 gu8_ble_ap_mode;

#endif
