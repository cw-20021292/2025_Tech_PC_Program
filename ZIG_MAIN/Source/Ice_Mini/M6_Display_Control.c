/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Main.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#include    "Macrodriver.h"
#include    "Global_Variable.h"
#include    "Port_Define.h"
#include    "M6_Display_Control.h"



void Display_Control(void);
void led_control_mode_decesion(void);
void display_version( void );
void led_blink_timer(void);
void on_all_control_led( void );
void off_all_control_led( void );
void all_duty_test_percent(void);
/* Memento Here! */
void    memento_display_step_conv(DISPLAY_MEMENTO_STEP U8_p_step);



extern FLUSHING_STEP gu8_flushing_mode;
extern bit bit_instruction_mode_start;

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////


TYPE_BYTE          U8FrontTempHundredB;
#define            gu8_front_temp_fnd_hundred                  U8FrontTempHundredB.byte
#define            Bit0_Front_Temp_Fnd_Hundred_1               U8FrontTempHundredB.Bit.b0
#define            Bit1_Front_Temp_Fnd_Hundred_2               U8FrontTempHundredB.Bit.b1
#define            Bit2_Front_Temp_Fnd_Hundred_3               U8FrontTempHundredB.Bit.b2
#define            Bit3_Front_Temp_Fnd_Hundred_4               U8FrontTempHundredB.Bit.b3
#define            Bit4_Front_Temp_Fnd_Hundred_5               U8FrontTempHundredB.Bit.b4
#define            Bit5_Front_Temp_Fnd_Hundred_6               U8FrontTempHundredB.Bit.b5
#define            Bit6_Front_Temp_Fnd_Hundred_7               U8FrontTempHundredB.Bit.b6

TYPE_BYTE          U8FrontTempTenB;
#define            gu8_front_temp_fnd_ten                      U8FrontTempTenB.byte
#define            Bit0_Front_Temp_Fnd_Ten_1                   U8FrontTempTenB.Bit.b0
#define            Bit1_Front_Temp_Fnd_Ten_2                   U8FrontTempTenB.Bit.b1
#define            Bit2_Front_Temp_Fnd_Ten_3                   U8FrontTempTenB.Bit.b2
#define            Bit3_Front_Temp_Fnd_Ten_4                   U8FrontTempTenB.Bit.b3
#define            Bit4_Front_Temp_Fnd_Ten_5                   U8FrontTempTenB.Bit.b4
#define            Bit5_Front_Temp_Fnd_Ten_6                   U8FrontTempTenB.Bit.b5
#define            Bit6_Front_Temp_Fnd_Ten_7                   U8FrontTempTenB.Bit.b6

TYPE_BYTE          U8FrontTempOneB;
#define            gu8_front_temp_fnd_one                      U8FrontTempOneB.byte
#define            Bit0_Front_Temp_Fnd_One_1                   U8FrontTempOneB.Bit.b0
#define            Bit1_Front_Temp_Fnd_One_2                   U8FrontTempOneB.Bit.b1
#define            Bit2_Front_Temp_Fnd_One_3                   U8FrontTempOneB.Bit.b2
#define            Bit3_Front_Temp_Fnd_One_4                   U8FrontTempOneB.Bit.b3
#define            Bit4_Front_Temp_Fnd_One_5                   U8FrontTempOneB.Bit.b4
#define            Bit5_Front_Temp_Fnd_One_6                   U8FrontTempOneB.Bit.b5
#define            Bit6_Front_Temp_Fnd_One_7                   U8FrontTempOneB.Bit.b6

/////////////////////////////////////////////////////////////////////////////////////////////

TYPE_BYTE          U8FrontAmountHundredB;
#define            gu8_front_amount_fnd_hundred                U8FrontAmountHundredB.byte
#define            Bit0_Front_Amount_Fnd_Hundred_1             U8FrontAmountHundredB.Bit.b0
#define            Bit1_Front_Amount_Fnd_Hundred_2             U8FrontAmountHundredB.Bit.b1
#define            Bit2_Front_Amount_Fnd_Hundred_3             U8FrontAmountHundredB.Bit.b2
#define            Bit3_Front_Amount_Fnd_Hundred_4             U8FrontAmountHundredB.Bit.b3
#define            Bit4_Front_Amount_Fnd_Hundred_5             U8FrontAmountHundredB.Bit.b4
#define            Bit5_Front_Amount_Fnd_Hundred_6             U8FrontAmountHundredB.Bit.b5
#define            Bit6_Front_Amount_Fnd_Hundred_7             U8FrontAmountHundredB.Bit.b6

TYPE_BYTE          U8FrontAmountTenB;
#define            gu8_front_amount_fnd_ten                    U8FrontAmountTenB.byte
#define            Bit0_Front_Amount_Fnd_Ten_1                 U8FrontAmountTenB.Bit.b0
#define            Bit1_Front_Amount_Fnd_Ten_2                 U8FrontAmountTenB.Bit.b1
#define            Bit2_Front_Amount_Fnd_Ten_3                 U8FrontAmountTenB.Bit.b2
#define            Bit3_Front_Amount_Fnd_Ten_4                 U8FrontAmountTenB.Bit.b3
#define            Bit4_Front_Amount_Fnd_Ten_5                 U8FrontAmountTenB.Bit.b4
#define            Bit5_Front_Amount_Fnd_Ten_6                 U8FrontAmountTenB.Bit.b5
#define            Bit6_Front_Amount_Fnd_Ten_7                 U8FrontAmountTenB.Bit.b6

TYPE_BYTE          U8FrontAmountOneB;
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

///////////
TYPE_BYTE          U8FrontLeftLEDB;
#define            gu8_front_left_led                          U8FrontLeftLEDB.byte
#define            Bit0_Front_Left_Led_Ice_Full_Text           U8FrontLeftLEDB.Bit.b0
#define            Bit1_Front_Left_Led_Ice_Shortae_Text        U8FrontLeftLEDB.Bit.b1
#define            Bit2_Front_Left_Led_UV_Ster_Text            U8FrontLeftLEDB.Bit.b2
#define            Bit3_Front_Left_Led_Hot_Ster_Text           U8FrontLeftLEDB.Bit.b3
#define            Bit4_Front_Left_Led_Sleep_Icon              U8FrontLeftLEDB.Bit.b4
#define            Bit5_Front_Left_Led_Step_Text               U8FrontLeftLEDB.Bit.b5
#define            Bit6_Front_Left_Led_Clock_Colon_Icon        U8FrontLeftLEDB.Bit.b6
#define            Bit7_Front_Left_Led_Celcius_oC_Icon         U8FrontLeftLEDB.Bit.b7

/* LED ���� BIT �߰� 250210 @CH.PARK */
TYPE_BYTE          U8FrontTextLEDB;
#define            gu8_front_left_led_2                         U8FrontTextLEDB.byte
#define            Bit0_Front_Left_Led_Eff_Faucet_Text          U8FrontTextLEDB.Bit.b0
#define            Bit1_Front_Left_Led_Ice_Faucet_Text          U8FrontTextLEDB.Bit.b1
#define            Bit2_Front_Left_Led_Ice_Tray_Text            U8FrontTextLEDB.Bit.b2
#define            Bit3_Front_Left_Led_Ice_Storage_Box_Text     U8FrontTextLEDB.Bit.b3
#define            Bit4_Front_Right_Led_Filter_Clean_Text       U8FrontTextLEDB.Bit.b4
#define            Bit5_Front_Right_Seg_Dot_Text                U8FrontTextLEDB.Bit.b5

TYPE_BYTE          U8FrontRightLEDB;
#define            gu8_front_right_led                         U8FrontRightLEDB.byte
#define            Bit0_Front_Right_Led_Wifi_Icon_White        U8FrontRightLEDB.Bit.b0
#define            Bit1_Front_Right_Led_Wifi_Icon_Blue         U8FrontRightLEDB.Bit.b1
#define            Bit2_Front_Right_Led_Sunny_Icon             U8FrontRightLEDB.Bit.b2
#define            Bit3_Front_Right_Led_Cloud_Icon             U8FrontRightLEDB.Bit.b3
#define            Bit4_Front_Right_Led_Rain_Icon              U8FrontRightLEDB.Bit.b4
#define            Bit5_Front_Right_Led_Percent_Icon           U8FrontRightLEDB.Bit.b5
#define            Bit6_Front_Right_Led_Liter_L_Icon           U8FrontRightLEDB.Bit.b6
#define            Bit7_Front_Right_Led_ml_Icon                U8FrontRightLEDB.Bit.b7


TYPE_BYTE          U8FrontTempSettingLEDB;
#define            gu8_front_temp_setting_bar_led              U8FrontTempSettingLEDB.byte
#define            Bit0_Front_Led_Temp_Setting_Bar_1_1         U8FrontTempSettingLEDB.Bit.b0
#define            Bit1_Front_Led_Temp_Setting_Bar_1_2         U8FrontTempSettingLEDB.Bit.b1
#define            Bit2_Front_Led_Temp_Setting_Bar_1_3         U8FrontTempSettingLEDB.Bit.b2
#define            Bit3_Front_Led_Temp_Setting_Bar_1_4         U8FrontTempSettingLEDB.Bit.b3
#define            Bit4_Front_Led_Temp_Setting_Bar_1_5         U8FrontTempSettingLEDB.Bit.b4        // [���� BAR 5 ������]

TYPE_BYTE          U8FrontAmountSettingLEDB;
#define            gu8_front_amount_setting_bar_led            U8FrontAmountSettingLEDB.byte
#define            Bit0_Front_Led_Amount_Setting_Bar_2_1       U8FrontAmountSettingLEDB.Bit.b0
#define            Bit1_Front_Led_Amount_Setting_Bar_2_2       U8FrontAmountSettingLEDB.Bit.b1
#define            Bit2_Front_Led_Amount_Setting_Bar_2_3       U8FrontAmountSettingLEDB.Bit.b2
#define            Bit3_Front_Led_Amount_Setting_Bar_2_4       U8FrontAmountSettingLEDB.Bit.b3        // [���� BAR 4 ������]
#define            Bit4_Front_Led_Amount_Setting_Bar_Continue  U8FrontAmountSettingLEDB.Bit.b4

TYPE_BYTE          U8FrontCenterRecipeLEDB;
#define            gu8_front_center_recipe_led                 U8FrontCenterRecipeLEDB.byte
#define            Bit0_Front_Center_Recipe_Ramen_Text         U8FrontCenterRecipeLEDB.Bit.b0
#define            Bit1_Front_Center_Tea_Text                  U8FrontCenterRecipeLEDB.Bit.b1
#define            Bit2_Front_Center_Recipe_Drip_Coffee_Text   U8FrontCenterRecipeLEDB.Bit.b2
#define            Bit3_Front_Center_MY_1_Text                 U8FrontCenterRecipeLEDB.Bit.b3
#define            Bit4_Front_Center_MY_2_Text                 U8FrontCenterRecipeLEDB.Bit.b4
#define            Bit5_Front_Center_MY_3_Text                 U8FrontCenterRecipeLEDB.Bit.b5

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

TYPE_BYTE          U8FrontSelectLEDB;
#define            gu8_front_select_led                        U8FrontSelectLEDB.byte
#define            Bit0_Front_Led_Ice_Select                   U8FrontSelectLEDB.Bit.b0
#define            Bit1_Front_Led_Ice_Water_Select             U8FrontSelectLEDB.Bit.b1
#define            Bit2_Front_Led_Hot_Select                   U8FrontSelectLEDB.Bit.b2
#define            Bit3_Front_Led_Ambient_Select               U8FrontSelectLEDB.Bit.b3
#define            Bit4_Front_Led_Cold_Select                  U8FrontSelectLEDB.Bit.b4
#define            Bit5_Front_Led_Amount_Select                U8FrontSelectLEDB.Bit.b5
#define            Bit6_Front_Led_My_Select                    U8FrontSelectLEDB.Bit.b6     // [MY] ��ġ ���� ������

TYPE_BYTE          U8FrontSettingLEDB;
#define            gu8_front_setting_led                       U8FrontSettingLEDB.byte
#define            Bit0_Front_Led_Ice_Size                     U8FrontSettingLEDB.Bit.b0
#define            Bit1_Front_Led_Ice_Off                      U8FrontSettingLEDB.Bit.b1
#define            Bit2_Front_Led_Hot_Lock_Text                U8FrontSettingLEDB.Bit.b2
#define            Bit3_Front_Led_Ice_First                    U8FrontSettingLEDB.Bit.b3
#define            Bit4_Front_Led_Cold_Temp                    U8FrontSettingLEDB.Bit.b4
#define            Bit5_Front_Led_Sleep_Mode_Text              U8FrontSettingLEDB.Bit.b5
#define            Bit6_Front_Led_Cold_Off                     U8FrontSettingLEDB.Bit.b6    // [�ü�OFF] ��ġ ���� ������


/* LED ���� BIT �߰� 250210 @CH.PARK */
TYPE_BYTE          U8FrontSettingLEDB_2;
#define            gu8_front_setting_led_2                     U8FrontSettingLEDB_2.byte
#define            Bit0_Front_Led_Ice_Lock                     U8FrontSettingLEDB_2.Bit.b0
#define            Bit1_Front_Led_All_Lock                     U8FrontSettingLEDB_2.Bit.b1
#define            Bit2_Front_Led_Icon_Led_Ice_Locker          U8FrontSettingLEDB_2.Bit.b2      // [���� �ڹ��� ������]
#define            Bit3_Front_Led_Icon_Led_Hot_Locker          U8FrontSettingLEDB_2.Bit.b3      // [�¼� �ڹ��� ������]
#define            Bit4_Front_Led_Icon_Fast_Ice                U8FrontSettingLEDB_2.Bit.b4      // LPP �߰� : �������� ���� ������
#define            Bit5_Front_Led_Icon_Comp_Blue               U8FrontSettingLEDB_2.Bit.b5      // LPP �߰� : COMP ���� LED
#define            Bit6_Front_Led_Icon_Comp_White              U8FrontSettingLEDB_2.Bit.b6      // COMP ȭ��Ʈ LED

TYPE_BYTE          U8FrontUnderTextLEDB;
#define            gu8_front_under_text_led                    U8FrontUnderTextLEDB.byte
#define            Bit0_Front_Under_Txt_Led_Ice_Large          U8FrontUnderTextLEDB.Bit.b0
#define            Bit1_Front_Under_Txt_Led_Ice_Center_bar     U8FrontUnderTextLEDB.Bit.b1
#define            Bit2_Front_Under_Txt_Led_Ice_Small          U8FrontUnderTextLEDB.Bit.b2
#define            Bit3_Front_Under_Txt_Led_Cold_High          U8FrontUnderTextLEDB.Bit.b3
#define            Bit4_Front_Under_Txt_Led_Cold_Center_Bar    U8FrontUnderTextLEDB.Bit.b4
#define            Bit5_Front_Under_Txt_Led_Cold_Low           U8FrontUnderTextLEDB.Bit.b5
#define            Bit6_Front_Under_Txt_Led_Ice_Normal         U8FrontUnderTextLEDB.Bit.b6
#define            Bit7_Front_Under_Txt_Led_Ice_Piece          U8FrontUnderTextLEDB.Bit.b7

TYPE_BYTE          U8FrontUnderIconLEDB;
#define            gu8_front_under_icon_led                         U8FrontUnderIconLEDB.byte
#define            Bit1_Front_Under_Ice_Inner_Blue_Extract          U8FrontUnderIconLEDB.Bit.b0     // ��������� ���� ���� LED
#define            Bit1_Front_Under_Ice_Inner_White_Extract         U8FrontUnderIconLEDB.Bit.b1     // ��������� ���� ȭ��Ʈ LED
#define            Bit2_Front_Under_Ice_Outer_Blue_Extract          U8FrontUnderIconLEDB.Bit.b2     // ��������� �ٱ��� ���� LED
#define            Bit3_Front_Under_Ice_Outer_White_Extract         U8FrontUnderIconLEDB.Bit.b3     // ��������� �ٱ��� ȭ��Ʈ LED
#define            Bit4_Front_Under_Water_White_Extract             U8FrontUnderIconLEDB.Bit.b4     // �ÿ�������� ȭ��Ʈ LED
#define            Bit5_Front_Under_Led_Welcome                     U8FrontUnderIconLEDB.Bit.b5     // WELCOME 1 LED
#define            Bit6_Front_Under_Led_Welcome_2                   U8FrontUnderIconLEDB.Bit.b6     // WELCOME 2 LED CH.PARK ADDED 250204

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
TYPE_DIMMING       U8DimmingPercentNo1;
#define            gu8_dimming_set_one                         U8DimmingPercentNo1.byte
#define            bit_0_3_ice_select                          U8DimmingPercentNo1.nibble.b0_3
#define            bit_4_7_ice_water_select                    U8DimmingPercentNo1.nibble.b4_7

TYPE_DIMMING       U8DimmingPercentNo2;
#define            gu8_dimming_set_two                         U8DimmingPercentNo2.byte
#define            bit_0_3_hot_select                          U8DimmingPercentNo2.nibble.b0_3
#define            bit_4_7_ambient_select                      U8DimmingPercentNo2.nibble.b4_7

TYPE_DIMMING       U8DimmingPercentNo3;
#define            gu8_dimming_set_three                       U8DimmingPercentNo3.byte
#define            bit_0_3_cold_select                         U8DimmingPercentNo3.nibble.b0_3
#define            bit_4_7_amount_select                       U8DimmingPercentNo3.nibble.b4_7

TYPE_DIMMING       U8DimmingPercentNo4;
#define            gu8_dimming_set_four                        U8DimmingPercentNo4.byte
#define            bit_0_3_setting_ice_size                    U8DimmingPercentNo4.nibble.b0_3
#define            bit_4_7_setting_ice_off                     U8DimmingPercentNo4.nibble.b4_7


TYPE_DIMMING       U8DimmingPercentNo5;
#define            gu8_dimming_set_five                        U8DimmingPercentNo5.byte
#define            bit_0_3_setting_ice_first                   U8DimmingPercentNo5.nibble.b0_3
#define            bit_4_7_setting_cold_temp                   U8DimmingPercentNo5.nibble.b4_7


TYPE_DIMMING       U8DimmingPercentNo6;
#define            gu8_dimming_set_six                         U8DimmingPercentNo6.byte
#define            bit_0_3_setting_sleep_mode                  U8DimmingPercentNo6.nibble.b0_3
#define            bit_4_7_setting_ice_lock                    U8DimmingPercentNo6.nibble.b4_7


TYPE_DIMMING       U8DimmingPercentNo7;
#define            gu8_dimming_set_seven                       U8DimmingPercentNo7.byte
#define            bit_0_3_all_lock                            U8DimmingPercentNo7.nibble.b0_3
#define            bit_4_7_hot_lock                            U8DimmingPercentNo7.nibble.b4_7

//////////////////////////////////////////////////////////////////////////////////////////////////

TYPE_DIMMING       U8DimmingPercentNo8;
#define            gu8_dimming_set_eight                       U8DimmingPercentNo8.byte
#define            bit_0_3_ice_full_txt                        U8DimmingPercentNo8.nibble.b0_3
#define            bit_4_7_UV_ster_txt                         U8DimmingPercentNo8.nibble.b4_7


TYPE_DIMMING       U8DimmingPercentNo9;
#define            gu8_dimming_set_nine                        U8DimmingPercentNo9.byte
#define            bit_0_3_hot_ster_txt                        U8DimmingPercentNo9.nibble.b0_3
#define            bit_4_7_sleep_icon                          U8DimmingPercentNo9.nibble.b4_7


TYPE_DIMMING       U8DimmingPercentNo10;
#define            gu8_dimming_set_ten                         U8DimmingPercentNo10.byte
#define            bit_0_3_step_txt                            U8DimmingPercentNo10.nibble.b0_3
#define            bit_4_7_clock_colon_icon                    U8DimmingPercentNo10.nibble.b4_7


TYPE_DIMMING       U8DimmingPercentNo11;
#define            gu8_dimming_set_eleven                      U8DimmingPercentNo11.byte
#define            bit_0_3_oC_icon                             U8DimmingPercentNo11.nibble.b0_3
#define            bit_4_7_fast_ice_icon                       U8DimmingPercentNo11.nibble.b4_7
//////////////////////////////////////////////////////////////////////////////////////////////////

TYPE_DIMMING       U8DimmingPercentNo12;
#define            gu8_dimming_set_twelve                      U8DimmingPercentNo12.byte
#define            bit_0_3_wifi_white_icon                     U8DimmingPercentNo12.nibble.b0_3
#define            bit_4_7_my_select                      U8DimmingPercentNo12.nibble.b4_7

TYPE_DIMMING       U8DimmingPercentNo13;
#define            gu8_dimming_set_thirteen                    U8DimmingPercentNo13.byte
#define            bit_0_3_sunny_icon                          U8DimmingPercentNo13.nibble.b0_3
#define            bit_4_7_cloud_icon                          U8DimmingPercentNo13.nibble.b4_7

TYPE_DIMMING       U8DimmingPercentNo14;
#define            gu8_dimming_set_fourteen                    U8DimmingPercentNo14.byte
#define            bit_0_3_rain_icon                           U8DimmingPercentNo14.nibble.b0_3
#define            bit_4_7_percent_icon                        U8DimmingPercentNo14.nibble.b4_7


TYPE_DIMMING       U8DimmingPercentNo15;
#define            gu8_dimming_set_fifteen                     U8DimmingPercentNo15.byte
#define            bit_0_3_liter_txt                           U8DimmingPercentNo15.nibble.b0_3
#define            bit_4_7_ml_txt                              U8DimmingPercentNo15.nibble.b4_7
//////////////////////////////////////////////////////////////////////////////////////////////////

TYPE_DIMMING       U8DimmingPercentNo16;
#define            gu8_dimming_set_sixteen                     U8DimmingPercentNo16.byte
#define            bit_0_3_temp_bar_1_1                        U8DimmingPercentNo16.nibble.b0_3
#define            bit_4_7_temp_bar_1_2                        U8DimmingPercentNo16.nibble.b4_7


TYPE_DIMMING       U8DimmingPercentNo17;
#define            gu8_dimming_set_seventeen                   U8DimmingPercentNo17.byte
#define            bit_0_3_temp_bar_1_3                        U8DimmingPercentNo17.nibble.b0_3
#define            bit_4_7_temp_bar_1_4                        U8DimmingPercentNo17.nibble.b4_7

TYPE_DIMMING       U8DimmingPercentNo18;
#define            gu8_dimming_set_eighteen                    U8DimmingPercentNo18.byte
#define            bit_0_3_amount_bar_2_1                      U8DimmingPercentNo18.nibble.b0_3
#define            bit_4_7_amount_bar_2_2                      U8DimmingPercentNo18.nibble.b4_7

TYPE_DIMMING       U8DimmingPercentNo19;
#define            gu8_dimming_set_nineteen                    U8DimmingPercentNo19.byte
#define            bit_0_3_amount_bar_2_3                      U8DimmingPercentNo19.nibble.b0_3
#define            bit_4_7_amount_bar_continue                 U8DimmingPercentNo19.nibble.b4_7

//////////////////////////////////////////////////////////////////////////////////////////////////

TYPE_DIMMING       U8DimmingPercentNo20;
#define            gu8_dimming_set_twenty                      U8DimmingPercentNo20.byte
#define            bit_0_3_recipe_ramen_txt                    U8DimmingPercentNo20.nibble.b0_3
#define            bit_4_7_recipe_drip_coffee_txt              U8DimmingPercentNo20.nibble.b4_7

TYPE_DIMMING       U8DimmingPercentNo21;
#define            gu8_dimming_set_twenty_one                  U8DimmingPercentNo21.byte
#define            bit_0_3_MY_1_txt                            U8DimmingPercentNo21.nibble.b0_3
#define            bit_4_7_MY_2_txt                            U8DimmingPercentNo21.nibble.b4_7

//////////////////////////////////////////////////////////////////////////////////////////////////

TYPE_DIMMING       U8DimmingPercentNo22;
#define            gu8_dimming_set_twenty_two                  U8DimmingPercentNo22.byte
#define            bit_0_3_ice_large_txt                       U8DimmingPercentNo22.nibble.b0_3
#define            bit_4_7_ice_center_bar                      U8DimmingPercentNo22.nibble.b4_7


TYPE_DIMMING       U8DimmingPercentNo23;
#define            gu8_dimming_set_twenty_three                U8DimmingPercentNo23.byte
#define            bit_0_3_ice_small_txt                       U8DimmingPercentNo23.nibble.b0_3
#define            bit_4_7_ice_normal_txt                       U8DimmingPercentNo23.nibble.b4_7


TYPE_DIMMING       U8DimmingPercentNo24;
#define            gu8_dimming_set_twenty_four                 U8DimmingPercentNo24.byte
#define            bit_0_3_cold_high_txt                       U8DimmingPercentNo24.nibble.b0_3
#define            bit_4_7_cold_center_bar                     U8DimmingPercentNo24.nibble.b4_7


TYPE_DIMMING       U8DimmingPercentNo25;
#define            gu8_dimming_set_twenty_five                 U8DimmingPercentNo25.byte
#define            bit_0_3_cold_low_txt                        U8DimmingPercentNo25.nibble.b0_3
#define            bit_4_7_ice_piece_txt                       U8DimmingPercentNo25.nibble.b4_7
//////////////////////////////////////////////////////////////////////////////////////////////////

TYPE_DIMMING       U8DimmingPercentNo26;
#define            gu8_dimming_set_twenty_six                  U8DimmingPercentNo26.byte
#define            bit_0_3_ice_lock_icon                       U8DimmingPercentNo26.nibble.b0_3
#define            bit_4_7_hot_lock_icon                       U8DimmingPercentNo26.nibble.b4_7

TYPE_DIMMING       U8DimmingPercentNo27;
#define            gu8_dimming_set_twenty_seven                U8DimmingPercentNo27.byte
#define            bit_0_3_comp_operation_dot                  U8DimmingPercentNo27.nibble.b0_3
#define            bit_4_7_comp_blue_dot                       U8DimmingPercentNo27.nibble.b4_7

TYPE_DIMMING       U8DimmingPercentNo28;
#define            gu8_dimming_set_twenty_eight                U8DimmingPercentNo28.byte
#define            bit_0_3_ice_extract                         U8DimmingPercentNo28.nibble.b0_3
#define            bit_4_7_water_extract                       U8DimmingPercentNo28.nibble.b4_7

TYPE_DIMMING       U8DimmingPercentNo29;
#define            gu8_dimming_set_twenty_nine                 U8DimmingPercentNo29.byte
#define            bit_0_3_welcome_led                         U8DimmingPercentNo29.nibble.b0_3
#define            bit_4_7_ice_blue_inner                      U8DimmingPercentNo29.nibble.b4_7
//////////////////////////////////////////////////////////////////////////////////////////////////

TYPE_DIMMING       U8DimmingPercentNo30;
#define            gu8_dimming_set_thirty                      U8DimmingPercentNo30.byte
#define            bit_0_3_amount_bar_2_4                      U8DimmingPercentNo30.nibble.b0_3
#define            bit_4_7_temp_bar_1_5                        U8DimmingPercentNo30.nibble.b4_7

TYPE_DIMMING       U8DimmingPercentNo31;
#define            gu8_dimming_set_thirty_one                 U8DimmingPercentNo31.byte
#define            bit_0_3_Ice_Shortage                       U8DimmingPercentNo31.nibble.b0_3
#define            bit_4_7_eff_Faucet_Text                    U8DimmingPercentNo31.nibble.b4_7

TYPE_DIMMING       U8DimmingPercentNo32;
#define            gu8_dimming_set_thirty_two                  U8DimmingPercentNo32.byte
#define            bit_0_3_Ice_Faucet                          U8DimmingPercentNo32.nibble.b0_3
#define            bit_4_7_Ice_Tray                            U8DimmingPercentNo32.nibble.b4_7

TYPE_DIMMING       U8DimmingPercentNo33;
#define            gu8_dimming_set_thirty_three                  U8DimmingPercentNo33.byte
#define            bit_0_3_Ice_Storage_Box                       U8DimmingPercentNo33.nibble.b0_3
#define            bit_4_7_Filter_Clean                       U8DimmingPercentNo33.nibble.b4_7

TYPE_DIMMING       U8DimmingPercentNo34;
#define            gu8_dimming_set_thirty_four                  U8DimmingPercentNo34.byte
#define            bit_0_3_Right_Seg_Dot                       U8DimmingPercentNo34.nibble.b0_3
#define            bit_4_7_Tea_Text                       U8DimmingPercentNo34.nibble.b4_7

TYPE_DIMMING       U8DimmingPercentNo35;
#define            gu8_dimming_set_thirty_five                  U8DimmingPercentNo35.byte
#define            bit_0_3_My_3_txt                       U8DimmingPercentNo35.nibble.b0_3
#define            bit_4_7_Cold_Off                       U8DimmingPercentNo35.nibble.b4_7

TYPE_DIMMING       U8DimmingPercentNo36;
#define            gu8_dimming_set_thirty_six                  U8DimmingPercentNo36.byte
#define            bit_0_3_Ice_Water_Extract                   U8DimmingPercentNo36.nibble.b0_3
#define            bit_4_7_Welcome_2                        U8DimmingPercentNo36.nibble.b4_7

TYPE_DIMMING       U8DimmingPercentNo37;
#define            gu8_dimming_set_thirty_seven                  U8DimmingPercentNo37.byte
#define            bit_0_3_ice_extract_inner                     U8DimmingPercentNo37.nibble.b0_3
#define            bit_4_7_Ice_Water_Inner_Extract                         U8DimmingPercentNo37.nibble.b4_7

TYPE_DIMMING       U8DimmingPercentNo38;
#define            gu8_dimming_set_thirty_eight                 U8DimmingPercentNo38.byte
#define            bit_0_3_Seg_Left_Hundred                     U8DimmingPercentNo38.nibble.b0_3
#define            bit_4_7_Seg_Left_Ten                         U8DimmingPercentNo38.nibble.b4_7

TYPE_DIMMING       U8DimmingPercentNo39;
#define            gu8_dimming_set_thirty_nine                  U8DimmingPercentNo39.byte
#define            bit_0_3_Seg_Left_One                       U8DimmingPercentNo39.nibble.b0_3
#define            bit_4_7_Seg_Right_Hundred                       U8DimmingPercentNo39.nibble.b4_7

TYPE_DIMMING       U8DimmingPercentNo40;
#define            gu8_dimming_set_fourty                       U8DimmingPercentNo40.byte
#define            bit_0_3_Seg_Right_Ten                       U8DimmingPercentNo40.nibble.b0_3
#define            bit_4_7_Seg_Right_One                       U8DimmingPercentNo40.nibble.b4_7


//////////////////////////////////////////////////////////////////////////////////////////////////







/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

TYPE_WORD          U16AnimationDimmingW;
#define            gu16_animation_dimming                                     U16AnimationDimmingW.word
#define            u8AnimationDimming_L                                       U16AnimationDimmingW.byte[0]
#define            u8AnimationDimming_H                                       U16AnimationDimmingW.byte[1]
#define            Bit0_Animation_Dimming_Ice_Select_State                    U16AnimationDimmingW.Bit.b0
#define            Bit1_Animation_Dimming_Ice_Water_Select_State              U16AnimationDimmingW.Bit.b1
#define            Bit2_Animation_Dimming_Hot_Select_State                    U16AnimationDimmingW.Bit.b2
#define            Bit3_Animation_Dimming_Ambient_Select_State                U16AnimationDimmingW.Bit.b3
#define            Bit4_Animation_Dimming_Cold_Select_State                   U16AnimationDimmingW.Bit.b4
#define            Bit5_Animation_Dimming_Amount_Select_State                 U16AnimationDimmingW.Bit.b5
#define            Bit6_Animation_Dimming_My_Select_State                     U16AnimationDimmingW.Bit.b6
#define            Bit7_Animation_Dimming_Cold_Off_State                      U16AnimationDimmingW.Bit.b7
#define            Bit8_Animation_Dimming_Setting_Ice_Size_State              U16AnimationDimmingW.Bit.b8
#define            Bit9_Animation_Dimming_Setting_Ice_Off_State               U16AnimationDimmingW.Bit.b9
#define            Bit10_Animation_Dimming_Setting_Hot_Lock_State              U16AnimationDimmingW.Bit.b10
#define            Bit11_Animation_Dimming_Setting_Ice_First_State            U16AnimationDimmingW.Bit.b11
#define            Bit12_Animation_Dimming_Setting_Cold_Temp_State            U16AnimationDimmingW.Bit.b12
#define            Bit13_Animation_Dimming_Setting_Sleep_Mode_Text_State      U16AnimationDimmingW.Bit.b13
#define            Bit14_Animation_Dimming_Ice_Lock_State                     U16AnimationDimmingW.Bit.b14
#define            Bit15_Animation_Dimming_Setting_All_Lock_State             U16AnimationDimmingW.Bit.b15

TYPE_WORD          U16AnimationDimmingTwoW;
#define            gu16_animation_dimming_two                                 U16AnimationDimmingTwoW.word
#define            u8AnimationDimmingTwo_L                                    U16AnimationDimmingTwoW.byte[0]
#define            u8AnimationDimmingTwo_H                                    U16AnimationDimmingTwoW.byte[1]
#define            Bit0_Animation_Dimming_Ice_Full_Txt_State                  U16AnimationDimmingTwoW.Bit.b0
#define            Bit1_Animation_Dimming_UV_Ster_Txt_State                   U16AnimationDimmingTwoW.Bit.b1
#define            Bit2_Animation_Dimming_Hot_Ster_Txt_State                  U16AnimationDimmingTwoW.Bit.b2
#define            Bit3_Animation_Dimming_Sleep_Icon_State                    U16AnimationDimmingTwoW.Bit.b3
#define            Bit4_Animation_Dimming_Step_Txt_State                      U16AnimationDimmingTwoW.Bit.b4
#define            Bit5_Animation_Dimming_Clock_Colon_Icon_State              U16AnimationDimmingTwoW.Bit.b5
#define            Bit6_Animation_Dimming_oC_Icon_State                       U16AnimationDimmingTwoW.Bit.b6
#define            Bit7_Animation_Dimming_Wifi_Icon_State                     U16AnimationDimmingTwoW.Bit.b7
#define            Bit8_Animation_Dimming_Sunny_Icon_State                    U16AnimationDimmingTwoW.Bit.b8
#define            Bit9_Animation_Dimming_Cloud_Icon_State                    U16AnimationDimmingTwoW.Bit.b9
#define            Bit10_Animation_Dimming_Rain_Icon_State                    U16AnimationDimmingTwoW.Bit.b10
#define            Bit11_Animation_Dimming_Percent_Icon_State                 U16AnimationDimmingTwoW.Bit.b11
#define            Bit12_Animation_Dimming_Liter_Txt_State                    U16AnimationDimmingTwoW.Bit.b12
#define            Bit13_Animation_Dimming_ml_txt_State                       U16AnimationDimmingTwoW.Bit.b13
#define            Bit14_Animation_Dimming_right_dot_txt_State                U16AnimationDimmingTwoW.Bit.b14
#define            Bit15_Animation_Dimming_Hot_Locker_State                   U16AnimationDimmingTwoW.Bit.b15

TYPE_BYTE          U8AnimationDimmingThreeB;
#define            gu8_animation_dimming_three                                U8AnimationDimmingThreeB.byte
#define            Bit0_Animation_Dimming_Temp_bar_1_1_State                  U8AnimationDimmingThreeB.Bit.b0
#define            Bit1_Animation_Dimming_Temp_bar_1_2_State                  U8AnimationDimmingThreeB.Bit.b1
#define            Bit2_Animation_Dimming_Temp_bar_1_3_State                  U8AnimationDimmingThreeB.Bit.b2
#define            Bit3_Animation_Dimming_Temp_bar_1_4_State                  U8AnimationDimmingThreeB.Bit.b3
#define            Bit4_Animation_Dimming_Amount_bar_2_1_State                U8AnimationDimmingThreeB.Bit.b4
#define            Bit5_Animation_Dimming_Amount_bar_2_2_State                U8AnimationDimmingThreeB.Bit.b5
#define            Bit6_Animation_Dimming_Amount_bar_2_3_State                U8AnimationDimmingThreeB.Bit.b6
#define            Bit7_Animation_Dimming_Amount_bar_Continue_State           U8AnimationDimmingThreeB.Bit.b7

TYPE_BYTE          U8AnimationDimmingFourB;
#define            gu8_animation_dimming_four                                 U8AnimationDimmingFourB.byte
#define            Bit0_Animation_Dimming_Recipe_Ramen_txt_State              U8AnimationDimmingFourB.Bit.b0
#define            Bit1_Animation_Dimming_Recipe_Drip_Coffee_txt_State        U8AnimationDimmingFourB.Bit.b1
#define            Bit2_Animation_Dimming_Tea_txt_State                       U8AnimationDimmingFourB.Bit.b2
#define            Bit3_Animation_Dimming_MY_1_txt_State                      U8AnimationDimmingFourB.Bit.b3
#define            Bit4_Animation_Dimming_MY_2_txt_State                      U8AnimationDimmingFourB.Bit.b4
#define            Bit5_Animation_Dimming_Ice_Lock_Icon_State                 U8AnimationDimmingFourB.Bit.b5
#define            Bit6_Animation_Dimming_Hot_Lock_Icon_State                 U8AnimationDimmingFourB.Bit.b6
#define            Bit7_Animation_Dimming_Comp_Operation_Dot_State            U8AnimationDimmingFourB.Bit.b7



TYPE_BYTE          U8AnimationDimmingFiveB;
#define            gu8_animation_dimming_five                                 U8AnimationDimmingFiveB.byte
#define            Bit0_Animation_Dimming_Ice_Large_txt_State                 U8AnimationDimmingFiveB.Bit.b0
#define            Bit1_Animation_Dimming_Ice_Center_Bar_State                U8AnimationDimmingFiveB.Bit.b1
#define            Bit2_Animation_Dimming_Ice_Small_txt_State                 U8AnimationDimmingFiveB.Bit.b2
#define            Bit3_Animation_Dimming_Cold_High_txt_State                 U8AnimationDimmingFiveB.Bit.b3
#define            Bit4_Animation_Dimming_Cold_Center_Bar_State               U8AnimationDimmingFiveB.Bit.b4
#define            Bit5_Animation_Dimming_Cold_Low_txt_State                  U8AnimationDimmingFiveB.Bit.b5
#define            Bit6_Animation_Dimming_Welcome_2_Led_State                 U8AnimationDimmingFiveB.Bit.b6    // CH.PARK ADDED 250204
#define            Bit7_Animation_Dimming_My_3_txt_State                      U8AnimationDimmingFiveB.Bit.b7


TYPE_BYTE          U8AnimationDimmingSixB;
#define            gu8_animation_dimming_six                                  U8AnimationDimmingSixB.byte
#define            Bit0_Animation_Dimming_Ice_Extract_Outer_State             U8AnimationDimmingSixB.Bit.b0
#define            Bit1_Animation_Dimming_Water_Extract_State                 U8AnimationDimmingSixB.Bit.b1
#define            Bit2_Animation_Dimming_Welcome_Led_State                   U8AnimationDimmingSixB.Bit.b2
#define            Bit3_Animation_Dimming_Eff_Faucet_Text_State               U8AnimationDimmingSixB.Bit.b3
#define            Bit4_Animation_Dimming_Ice_Faucet_Text_State               U8AnimationDimmingSixB.Bit.b4
#define            Bit5_Animation_Dimming_Ice_Tray_Text_State                 U8AnimationDimmingSixB.Bit.b5
#define            Bit6_Animation_Dimming_Ice_Box_Text_State                  U8AnimationDimmingSixB.Bit.b6
#define            Bit7_Animation_Dimming_Filter_Clean_Text_State             U8AnimationDimmingSixB.Bit.b7

TYPE_BYTE          U8AnimationDimmingSevenB;
#define            gu8_animation_dimming_seven                                U8AnimationDimmingSevenB.byte
#define            Bit0_Animation_Dimming_Temp_bar_1_5_State                  U8AnimationDimmingSevenB.Bit.b0
#define            Bit1_Animation_Dimming_Amount_bar_2_4_State                U8AnimationDimmingSevenB.Bit.b1
#define            Bit2_Animation_Dimming_Ice_Shortage_Text_State             U8AnimationDimmingSevenB.Bit.b2
#define            Bit3_Animation_Dimming_Ice_Water_Text_State                U8AnimationDimmingSevenB.Bit.b3
#define            Bit4_Animation_Dimming_Ice_Water_Extract_State             U8AnimationDimmingSevenB.Bit.b4
#define            Bit5_Animation_Dimming_Ice_Normal_State                    U8AnimationDimmingSevenB.Bit.b5
#define            Bit6_Animation_Dimming_Ice_Piece_State                     U8AnimationDimmingSevenB.Bit.b6
#define            Bit7_Animation_Dimming_Ice_Extract_Inner_State             U8AnimationDimmingSevenB.Bit.b7

/* LPP �߰� */
TYPE_BYTE          U8AnimationDimmingEightB;
#define            gu8_animation_dimming_eight                                U8AnimationDimmingEightB.byte
#define            Bit0_Animation_Dimming_Ice_Inner_Blue_state                U8AnimationDimmingEightB.Bit.b0
#define            Bit1_Animation_Dimming_Fast_Ice_State                      U8AnimationDimmingEightB.Bit.b1
#define            Bit2_Animation_Dimming_Comp_Blue_State                     U8AnimationDimmingEightB.Bit.b2
#define            Bit3_Animation_Dimming_Ice_Locker_State                    U8AnimationDimmingEightB.Bit.b3
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
U8 gu8_etc_1;
U8 gu8_etc_2;
U8 gu8_etc_3;
U8 gu8_etc_4;
U8 gu8_animation_time;
U8 gu8_animation_duty;
U8 gu8_rgb_pwm_timer;
U8 gu8_rgb_pwm_period;
U8 gu8_rgb_red_on_time;
U8 gu8_rgb_green_on_time;
U8 gu8_rgb_blue_on_time;

U8 gu8_rgb_ice_outer_red_on_time;
U8 gu8_rgb_ice_outer_green_on_time;
U8 gu8_rgb_ice_outer_blue_on_time;

U8 gu8_rgb_ice_inner_red_on_time;
U8 gu8_rgb_ice_inner_green_on_time;
U8 gu8_rgb_ice_inner_blue_on_time;

U8 gu8T300ms;
bit F_Wink_300ms;
U8 gu8T500ms;
bit F_Wink_500ms;
U8 gu8T1000ms;
bit F_Wink_1000ms;
U8 gu8T2000ms;
bit F_Wink_2000ms;
U16 gu16T3000ms;
bit F_Wink_3000ms;
LED_STEP gu8_Led_Display_Step;
U8 gu8_led_version_step;
U8 gu8_version_display_Timer;
U8 gu8_version_display_count;
bit F_FW_Version_Display_Mode;
U8 gu8_selected_led_dimming;
U8 gu8_unselected_led_dimming;
U8 gu8_user_led_select;
U8 gu8_user_led_off_time;
U8 gu8_main_move_delay_timer;

/* Memento Here! */
DISPLAY_MEMENTO_STEP gu8_memento_mode_step;
U8 gu8_memento_display_Timer;
U8 gu8_memento_display_count;

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void Display_Control(void)
{
    led_blink_timer();
    led_control_mode_decesion();
    led_display();
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void led_control_mode_decesion(void)
{
    if( F_FW_Version_Display_Mode != SET )
    {
        gu8_Led_Display_Step = LED_Display__VERSION;
    }
    else if( gu8_fota_start == SET )
    {
        gu8_Led_Display_Step = LED_Display__FOTA_START;
    }
    else if( bit_self_test_start == SET )
    {
        gu8_Led_Display_Step = LED_Display__SELF_TEST;
    }
    else if( bit_filter_all == CLEAR )
    {
        gu8_Led_Display_Step = LED_Display__FILTER_COVER_OPEN;
    }
    else if(bit_setting_mode_start == SET)
    {
        gu8_Led_Display_Step = LED_Display__SETTING;
    }
    else if( bit_time_setting_start == SET)
    {
        gu8_Led_Display_Step = LED_Display__TIME_SETTING;
    }
    else if(bit_volume_setting_start == SET)
    {
        gu8_Led_Display_Step = LED_Display__VOLUME_SETTING;
    }
    else if( gu8_flushing_mode > FLUSHING_NONE_STATE && bit_instruction_mode_start == CLEAR )
    {
        gu8_Led_Display_Step = LED_Display__FLUSHING;
    }
    else if( u8ButtonSetDisplay > BUTTON_SET_DISPLAY_OFF )
    {
        gu8_Led_Display_Step = LED_Display__BUTTON_SET;
    }
    else if( bit_wifi_pairing_start == SET )
    {
        gu8_Led_Display_Step = LED_Display__WIFI_PAIRING;
    }
    else if(bit_memento_start == SET)                           /* Memento Here! */
    {
        gu8_Led_Display_Step = LED_Display__MEMENTO;
    }
    else if( bit_ice_tank_ster_start == SET )
    {
        gu8_Led_Display_Step = LED_Display__TRAY_CLEAN;
    }
    else if( F_All_Lock == SET )
    {
        gu8_Led_Display_Step = LED_Display__ALL_LOCK;
    }
    else if( bit_manual_drain_start == SET )
	{
    	gu8_Led_Display_Step = LED_Display__MANUAL_DRAIN;
	}
    else if(gu8_bright_test_mode == SET)
    {
        gu8_Led_Display_Step = LED_Display__BRIGHT_TEST;
    }
    else if((u8IceOutState == ICE_SELECT__ICE_WATER)
    && ((F_IceOut == SET) || (F_WaterOut == SET))
    )
    {
        gu8_Led_Display_Step = LED_Display__ICE_WATER_EXTRACT;          // ������ ���� ǥ��
    }
    else if((bit_myWater_setting_start == SET)
    && (F_WaterOut == SET)
    )
    {
        gu8_Led_Display_Step = LED_Display__MY_WATER_EXTRACT;           // MY���� ���� ǥ��
    }
    else if((F_WaterOut == SET) && (Extract_Stack.U8_iceSelect == CLEAR))
    {
        gu8_Led_Display_Step = LED_Display__WATER_EXTRACT;
    }
    else if((F_IceOut == SET) && (Extract_Stack.U8_waterSelect == CLEAR))
    {
        gu8_Led_Display_Step = LED_Display__ICE_EXTRACT;
    }
    else if(cody_water_line.gu8_start == SET)
    {
        gu8_Led_Display_Step = LED_Display__CODY_WATER_LINE_CLEAN;      /* Cody Water Line Clean Service */
    }
    else if( bit_display_sleep_start == SET || bit_30_min_no_use_start == SET )
    {
        gu8_Led_Display_Step = LED_Display__SLEEP;
    }
    else if( gu8_Error_Code > 0 && gu8_error_popup_enable == SET )
    {
        gu8_Led_Display_Step = LED_Display__ERROR;
    }
    else if( gu8_flushing_mode > FLUSHING_NONE_STATE && bit_instruction_mode_start == SET )
    {
        gu8_Led_Display_Step = LED_Display__INSTRUCTION;
    }
    else
    {
        gu8_Led_Display_Step = LED_Display__MAIN;
    }
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void display_version( void )
{
    switch(gu8_led_version_step)
    {
        case PROC_START:

            gu8_version_display_Timer++;

            if(gu8_version_display_Timer >= 5)
            {
                gu8_version_display_Timer = 0;

                gu8_version_display_count++;

                if((BIT)(gu8_version_display_count & 1) == (BIT)0)
                {
                    on_all_control_led();
                }
                else
                {
                    off_all_control_led();
                }

                /*if(gu8_version_display_count > 6)*/
                /*..hui [23-3-22���� 12:50:58] �˻�� �ʹ� ����.. 2�� ����� ����..*/
                if(gu8_version_display_count > 4)
                {
                    /*..hui [21-11-4���� 9:13:03] �Ϲݸ�忡���� ���� ǥ������ �ʵ���..*/

                        gu8_led_version_step++;
                        gu8_version_display_count = 0;
                        gu8_version_display_Timer = 0;
                }
                else{}
            }
            else{}

            break;

        case PROC_1:

            gu8_version_display_Timer++;

            if(gu8_version_display_Timer >= 5)
            {
                gu8_version_display_Timer = 0;
                gu8_led_version_step++;
            }
            else{}

            break;

        case PROC_2:

            //Bit0_Front_Led_Set_Select = SET;
            fnd_left_front_version();
            fnd_right_main_version();

            gu8_version_display_Timer++;

            if(gu8_version_display_Timer >= 10)
            {
                gu8_led_version_step++;
                gu8_version_display_Timer = 0;
            }
            else{}

            break;

        case PROC_END:

            /*..hui [17-4-4���� 2:20:26] ���������� 500ms OFF�ϰ� ����..*/
            off_all_control_led();

            gu8_version_display_Timer++;

            if(gu8_version_display_Timer >= 5)
            {
                gu8_led_version_step = 0;
                gu8_version_display_Timer = 0;
                F_FW_Version_Display_Mode = SET;
            }
            else{}

            break;

        default:
            break;
    }
}



/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void led_blink_timer(void)
{
    gu8T300ms++;

    if(gu8T300ms >= 3)
    {
        gu8T300ms = 0;
        F_Wink_300ms ^= SET;          // 300msec Wink
    }
    else{}

    gu8T500ms++;

    if(gu8T500ms >= 5)
    {
        gu8T500ms = 0;
        F_Wink_500ms ^= SET;          // 300msec Wink
    }
    else{}

    gu8T1000ms++;

    if(gu8T1000ms >= 10)
    {
        gu8T1000ms = 0;
        F_Wink_1000ms ^= SET;          // 300msec Wink
    }
    else{}

    gu8T2000ms++;

    if(gu8T2000ms >= 20)
    {
        gu8T2000ms = 0;
        F_Wink_2000ms ^= SET;          // 300msec Wink
    }
    else{}

    gu16T3000ms++;

    if(gu16T3000ms >= 30)
    {
        gu16T3000ms = 0;
        F_Wink_3000ms ^= SET;          // 300msec Wink
    }
    else{}
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void on_all_control_led( void )
{
    gu8_front_temp_fnd_hundred = 0xff;
    gu8_front_temp_fnd_ten = 0xff;
    gu8_front_temp_fnd_one = 0xff;

    gu8_front_amount_fnd_hundred = 0xff;
    gu8_front_amount_fnd_ten = 0xff;
    gu8_front_amount_fnd_one = 0xff;

    gu8_front_left_led = 0xff;
    gu8_front_right_led = 0xff;
    gu8_front_temp_setting_bar_led = 0xff;
    gu8_front_amount_setting_bar_led = 0xff;
    gu8_front_center_recipe_led = 0xff;

    gu8_front_select_led = 0xff;
    gu8_front_setting_led = 0xff;
    gu8_front_under_text_led = 0xff;
    gu8_front_under_icon_led = 0xff;

    /* LED �߰� 250210 @CH.PARK */
    gu8_front_left_led_2 = 0xFF;
    gu8_front_setting_led_2 = 0xFF;

    //////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////
    all_duty_100_percent();
    // all_duty_test_percent();
}

void all_duty_test_percent(void)
{
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
    set_duty_percent( DIMMING__PERCENT_SEG_LEFT_HUNDRED, left_normal_state_percent );
    set_duty_percent( DIMMING__PERCENT_SEG_LEFT_TEN, left_normal_state_percent );
    set_duty_percent( DIMMING__PERCENT_SEG_LEFT_ONE, left_normal_state_percent );
    
    set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_HUNDRED, right_normal_state_percent );
    set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_TEN, right_normal_state_percent );
    set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_ONE, right_normal_state_percent );
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
    /*..hui [24-2-22���� 5:26:02] ��ɺ�..*/////////////////////////////////
    set_duty_percent( DIMMING__PERCENT_ICE_SELECT, funtion_led_percent );
    set_duty_percent( DIMMING__PERCENT_ICE_WATER_SELECT, funtion_led_percent );
    set_duty_percent( DIMMING__PERCENT_HOT_SELECT, funtion_led_percent );
    set_duty_percent( DIMMING__PERCENT_AMBIENT_SELECT, funtion_led_percent );
    set_duty_percent( DIMMING__PERCENT_COLD_SELECT, funtion_led_percent );
    set_duty_percent( DIMMING__PERCENT_AMOUNT_SELECT, funtion_led_percent );
    set_duty_percent( DIMMING__PERCENT_MY_SELECT, funtion_led_percent );
    
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
    /*..hui [24-2-22���� 5:26:07] ������..*/
    set_duty_percent( DIMMING__PERCENT_SETTING_ICE_SIZE, setting_led_percent );
    set_duty_percent( DIMMING__PERCENT_SETTING_ICE_OFF, setting_led_percent );

    set_duty_percent( DIMMING__PERCENT_SETTING_ICE_FIRST, setting_led_percent );
    set_duty_percent( DIMMING__PERCENT_SETTING_COLD_TEMP, setting_led_percent );

    set_duty_percent( DIMMING__PERCENT_SETTING_SLEEP_MODE, setting_led_percent );
    set_duty_percent( DIMMING__PERCENT_SETTING_ICE_LOCK, setting_led_percent );

    set_duty_percent( DIMMING__PERCENT_SETTING_ALL_LOCK, setting_led_percent );
    set_duty_percent( DIMMING__PERCENT_SETTING_HOT_LOCK, setting_led_percent );
    set_duty_percent( DIMMING__PERCENT_COLD_OFF, setting_led_percent );
    
    set_duty_percent( DIMMING__PERCENT_ICE_LARGE_TEXT, setting_led_percent );    /////
    set_duty_percent( DIMMING__PERCENT_ICE_CENTER_BAR, setting_led_percent );    /////
    set_duty_percent( DIMMING__PERCENT_ICE_SMALL_TEXT, setting_led_percent );    /////

    set_duty_percent( DIMMING__PERCENT_COLD_HIGH_TEXT, setting_led_percent );    /////
    set_duty_percent( DIMMING__PERCENT_COLD_CENTER_BAR, setting_led_percent );    /////
    set_duty_percent( DIMMING__PERCENT_COLD_LOW_TEXT, setting_led_percent );    //////

    set_duty_percent( DIMMING__PERCENT_ICE_LOCK_ICON, setting_led_percent );    /////
    set_duty_percent( DIMMING__PERCENT_HOT_LOCK_ICON, setting_led_percent );    /////

    set_duty_percent( DIMMING__PERCENT_COMP_BLUE_DOT, setting_led_percent );
    set_duty_percent( DIMMING__PERCENT_COMP_WHITE_DOT, setting_led_percent );    /////

    set_duty_percent( DIMMING__PERCENT_ICE_NORMAL, setting_led_percent );
    set_duty_percent( DIMMING__PERCENT_ICE_PIECE, setting_led_percent );

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
    /*..hui [24-2-22���� 5:32:07] ��ܾ����ܺ�..*/
    set_duty_percent( DIMMING__PERCENT_ICE_FULL_TEXT, top_small_led_percent );
    set_duty_percent( DIMMING__PERCENT_ICE_SHORTAGE_TEXT, top_small_led_percent );
    set_duty_percent( DIMMING__PERCENT_SLEEP_ICON, top_small_led_percent );    /////
    set_duty_percent( DIMMING__PERCENT_WIFI_WHITE_ICON, top_small_led_percent );
    set_duty_percent( DIMMING__PERCENT_SUNNY_ICON, top_small_led_percent );    /////
    set_duty_percent( DIMMING__PERCENT_CLOUD_ICON, top_small_led_percent );
    set_duty_percent( DIMMING__PERCENT_RAIN_ICON, top_small_led_percent );
    set_duty_percent( DIMMING__PERCENT_FILTER_CLEAN, top_small_led_percent );
    set_duty_percent( DIMMING__PERCENT_FAST_ICE_ICON, top_small_led_percent );

    set_duty_percent( DIMMING__PERCENT_HOT_STER_TEXT, big_ster_led_percent );
    set_duty_percent( DIMMING__PERCENT_UV_STER_TEXT, big_ster_led_percent );

    set_duty_percent( DIMMING__PERCENT_EXT_FAUCET_TEXT, small_ster_led_percent );
    set_duty_percent( DIMMING__PERCENT_ICE_FAUCET_TEXT, small_ster_led_percent );
    set_duty_percent( DIMMING__PERCENT_ICE_TRAY_TEXT, small_ster_led_percent );
    set_duty_percent( DIMMING__PERCENT_ICE_STORAGE_BOX_TEXT, small_ster_led_percent );

    set_duty_percent( DIMMING__PERCENT_STEP_TEXT, middle_small_led_percent );
    set_duty_percent( DIMMING__PERCENT_oC_ICON, middle_small_led_percent );
    set_duty_percent( DIMMING__PERCENT_PERCENT_ICON, middle_small_led_percent );
    set_duty_percent( DIMMING__PERCENT_LITER_TEXT, middle_small_led_percent );    /////
    set_duty_percent( DIMMING__PERCENT_ml_TEXT, middle_small_led_percent );

    set_duty_percent( DIMMING__PERCENT_RIGHT_SEG_DOT, colon_dot_led_percent );
    set_duty_percent( DIMMING__PERCENT_CLOCK_COLON_ICON, colon_dot_led_percent );

    /* LED 100% ��� �߰� 250211 CH.PARK */
    set_duty_percent( DIMMING__PERCENT_TEMP_BAR_1_5, bar_led_percent );
    set_duty_percent( DIMMING__PERCENT_AMOUNT_BAR_2_4, bar_led_percent );
    
    set_duty_percent( DIMMING__PERCENT_TEMP_BAR_1_1, bar_led_percent );
    set_duty_percent( DIMMING__PERCENT_TEMP_BAR_1_2, bar_led_percent );

    set_duty_percent( DIMMING__PERCENT_TEMP_BAR_1_3, bar_led_percent );
    set_duty_percent( DIMMING__PERCENT_TEMP_BAR_1_4, bar_led_percent );

    set_duty_percent( DIMMING__PERCENT_AMOUNT_BAR_2_1, bar_led_percent );
    set_duty_percent( DIMMING__PERCENT_AMOUNT_BAR_2_2, bar_led_percent );

    set_duty_percent( DIMMING__PERCENT_AMOUNT_BAR_2_3, bar_led_percent );
    set_duty_percent( DIMMING__PERCENT_AMOUNT_BAR_CONTINUE, bar_led_percent );

    set_duty_percent( DIMMING__PERCENT_DRIP_COFFEE_TEXT, receipe_led_dripcoffee_percent );
    set_duty_percent( DIMMING__PERCENT_RAMEN_TEXT, receipe_led_percent );
    set_duty_percent( DIMMING__PERCENT_TEA_TEXT, receipe_led_percent );
    set_duty_percent( DIMMING__PERCENT_MY_1_TEXT, receipe_led_percent );    /////
    set_duty_percent( DIMMING__PERCENT_MY_2_TEXT, receipe_led_percent );    /////
    set_duty_percent( DIMMING__PERCENT_MY_3_TEXT, receipe_led_percent );


////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
    /*..hui [24-2-22���� 5:29:05] �߰� �ؽ�Ʈ �����ܺ�..*/
    set_duty_percent( DIMMING__PERCENT_WELCOME_LED, welcome_right_led_percent );
    set_duty_percent( DIMMING__PERCENT_WELCOME_LED_2, welcome_left_led_percent );

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
    /*..hui [24-2-22���� 5:26:40] ����..*/
    set_duty_percent( DIMMING__PERCENT_WATER_EXTRACT, water_extract_led_percent );
    set_duty_percent( DIMMING__PERCENT_ICE_EXTRACT, ice_extract_outer_led_percent );
    set_duty_percent( DIMMING__PERCENT_ICE_EXTRACT_INNER, ice_extract_inner_led_percent );
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void off_all_control_led( void )
{
    gu8_front_temp_fnd_hundred = 0;
    gu8_front_temp_fnd_ten = 0;
    gu8_front_temp_fnd_one = 0;

    gu8_front_amount_fnd_hundred = 0;
    gu8_front_amount_fnd_ten = 0;
    gu8_front_amount_fnd_one = 0;

    gu8_front_left_led = 0;
    gu8_front_right_led = 0;
    gu8_front_temp_setting_bar_led = 0;
    gu8_front_amount_setting_bar_led = 0;
    gu8_front_center_recipe_led = 0;

    gu8_front_select_led = 0;
    gu8_front_setting_led = 0;
    gu8_front_under_text_led = 0;
    gu8_front_under_icon_led = 0;

    /* 2KG LED �߰� 250211 CH.PARK */
    gu8_front_left_led_2 = CLEAR;
    gu8_front_setting_led_2 = CLEAR;

    make_rgb_color( RGB_OFF );
    make_ice_outer_rgb_color(RGB_OFF);
    make_ice_inner_rgb_color(RGB_OFF);

    //////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////
    all_duty_100_percent();
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  : Memento Here!
***********************************************************************************************************************/
void    display_memento(void)
{
    switch(gu8_memento_mode_step)
    {
        case MEMENTO_DISPLAY_START:
        gu8_memento_display_Timer++;
        if(gu8_memento_display_Timer >= 5)
        {
            gu8_memento_display_Timer = 0;
            gu8_memento_display_count++;

            if((BIT)(gu8_memento_display_count & 1) == (BIT)0)
            {
                on_all_control_led();
            }
            else
            {
                off_all_control_led();
            }

            if(gu8_memento_display_count > 4)
            {
                memento_display_step_conv(MEMENTO_DISPLAY_ERROR_1);
            }
            else {  }
        }
        break;

        case MEMENTO_DISPLAY_ERROR_1:
            Bit3_Front_Led_Ambient_Select = SET;
            Bit5_Front_Led_Amount_Select = SET;
            gu8_memento_display_Timer++;
            if(gu8_memento_display_Timer <= MEMENTO_DISPLAY_OFF_TIME)
            {
                off_all_control_led();
            }
            else if(gu8_memento_display_Timer <= MEMENTO_DISPLAY_ON_TIME)
            {
                memento_error_number_display(MEMENTO_DISPLAY_ERROR_1);
                memento_error_code_display(u8Memento_Buff[0]);
            }
            else
            {
                memento_display_step_conv(MEMENTO_DISPLAY_ERROR_2);
            }    
        break;

        case MEMENTO_DISPLAY_ERROR_2:
            Bit3_Front_Led_Ambient_Select = SET;
            Bit5_Front_Led_Amount_Select = SET;
            gu8_memento_display_Timer++;
            if(gu8_memento_display_Timer <= MEMENTO_DISPLAY_OFF_TIME)
            {
                off_all_control_led();
            }
            else if(gu8_memento_display_Timer <= MEMENTO_DISPLAY_ON_TIME)
            {
                memento_error_number_display(MEMENTO_DISPLAY_ERROR_2);
                memento_error_code_display(u8Memento_Buff[1]);
            }
            else
            {
                memento_display_step_conv(MEMENTO_DISPLAY_ERROR_3);
            }
        break;

        case MEMENTO_DISPLAY_ERROR_3:
            Bit3_Front_Led_Ambient_Select = SET;
            Bit5_Front_Led_Amount_Select = SET;
            gu8_memento_display_Timer++;
            if(gu8_memento_display_Timer <= MEMENTO_DISPLAY_OFF_TIME)
            {
                off_all_control_led();
            }
            else if(gu8_memento_display_Timer <= MEMENTO_DISPLAY_ON_TIME)
            {
                memento_error_number_display(MEMENTO_DISPLAY_ERROR_3);
                memento_error_code_display(u8Memento_Buff[2]);
            }
            else
            {
                memento_display_step_conv(MEMENTO_DISPLAY_ERROR_4);
            }
        break;

        case MEMENTO_DISPLAY_ERROR_4:
            Bit3_Front_Led_Ambient_Select = SET;
            Bit5_Front_Led_Amount_Select = SET;
            gu8_memento_display_Timer++;
            if(gu8_memento_display_Timer <= MEMENTO_DISPLAY_OFF_TIME)
            {
                off_all_control_led();
            }
            else if(gu8_memento_display_Timer <= MEMENTO_DISPLAY_ON_TIME)
            {
                memento_error_number_display(MEMENTO_DISPLAY_ERROR_4);
                memento_error_code_display(u8Memento_Buff[3]);
            }
            else
            {
                memento_display_step_conv(MEMENTO_DISPLAY_ERROR_5);
            }
        break;

        case MEMENTO_DISPLAY_ERROR_5:
            Bit3_Front_Led_Ambient_Select = SET;
            Bit5_Front_Led_Amount_Select = SET;
            gu8_memento_display_Timer++;
            if(gu8_memento_display_Timer <= MEMENTO_DISPLAY_OFF_TIME)
            {
                off_all_control_led();
            }
            else if(gu8_memento_display_Timer <= MEMENTO_DISPLAY_ON_TIME)
            {
                memento_error_number_display(MEMENTO_DISPLAY_ERROR_5);
                memento_error_code_display(u8Memento_Buff[4]);
            }
            else
            {
                memento_display_step_conv(MEMENTO_DISPLAY_END);
            }
        break;

        case MEMENTO_DISPLAY_END:
        gu8_memento_display_Timer++;
        if(gu8_memento_display_Timer <= 10)
        {
            off_all_control_led();
        }
        else
        {
            memento_display_step_conv(MEMENTO_DISPLAY_START);
        }
        break;
    }

    if(gu8_memento_mode_step > MEMENTO_DISPLAY_START)
    {
        set_duty_percent( DIMMING__PERCENT_AMBIENT_SELECT, setting_mode_function_extra_percent );
        set_duty_percent( DIMMING__PERCENT_AMOUNT_SELECT, setting_mode_function_extra_percent );
    }
    else {  }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  : Memento Here!
***********************************************************************************************************************/
void    memento_display_step_conv(DISPLAY_MEMENTO_STEP U8_p_step)
{
    if(gu8_memento_mode_step != U8_p_step)
    {
        gu8_memento_mode_step = U8_p_step;

        gu8_memento_display_Timer = 0;
        gu8_memento_display_count = 0;

        play_melody_select_196();
    }
    else {  }
}
