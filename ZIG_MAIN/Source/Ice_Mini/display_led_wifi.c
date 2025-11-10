/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Main.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  : AT+CONNECT=74F07DB01010
***********************************************************************************************************************/
#include    "Macrodriver.h"
#include    "Global_Variable.h"
#include    "Port_Define.h"
#include    "display_led_wifi.h"

void led_wifi_pairing(void);
void wifi_icon_output(void);

extern void set_duty_percent( U8 mu8_number, U8 mu8_percent );
extern void percent_icon_out(void);
extern void wifi_pairing_display(void);

extern TYPE_BYTE          U8FrontRightLEDB;
#define            gu8_front_right_led                         U8FrontRightLEDB.byte
#define            Bit0_Front_Right_Led_Wifi_Icon_White        U8FrontRightLEDB.Bit.b0
#define            Bit1_Front_Right_Led_Wifi_Icon_Blue         U8FrontRightLEDB.Bit.b1
#define            Bit2_Front_Right_Led_Sunny_Icon             U8FrontRightLEDB.Bit.b2
#define            Bit3_Front_Right_Led_Cloud_Icon             U8FrontRightLEDB.Bit.b3
#define            Bit4_Front_Right_Led_Rain_Icon              U8FrontRightLEDB.Bit.b4
#define            Bit5_Front_Right_Led_Percent_Icon           U8FrontRightLEDB.Bit.b5
#define            Bit6_Front_Right_Led_Liter_L_Icon           U8FrontRightLEDB.Bit.b6
#define            Bit7_Front_Right_Led_ml_Icon                U8FrontRightLEDB.Bit.b7

extern bit bit_install_flushing_state;
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void led_wifi_pairing(void)
{
    /*..hui [23-12-4오후 2:14:16] 페어링 화면에서는 wifi 아이콘이랑 퍼센트만 표시..*/
    wifi_icon_output();
	percent_icon_out();
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void wifi_icon_output(void)
{
    if( gu8_Led_Display_Step == LED_Display__SLEEP )
    {
        set_duty_percent( DIMMING__PERCENT_WIFI_WHITE_ICON, SLEEP_MODE_DIMMING_PERCENT );
    }
    else
    {
        set_duty_percent( DIMMING__PERCENT_WIFI_WHITE_ICON, top_small_led_percent );
    }
		
    if( gu8_Wifi_Connect_State == WIFI_OFF )
    {
        Bit0_Front_Right_Led_Wifi_Icon_White = CLEAR;
        //Bit4_Led_Wifi_Icon_Blue = CLEAR;
        gu8_wifi_disp_timer = 0;
        gu16_wifi_pairing_30min_timer = 0;
        gu8_wifi_pairing_5sec_timer = 0;
        bit_pairing_5s_display_start = CLEAR;
    }
    else if( gu8_Wifi_Connect_State == WIFI_CONNECT )
    {
        Bit0_Front_Right_Led_Wifi_Icon_White = SET;
        //Bit4_Led_Wifi_Icon_Blue = CLEAR;
        gu8_wifi_disp_timer = 0;
        gu16_wifi_pairing_30min_timer = 0;
        gu8_wifi_pairing_5sec_timer = 0;
        bit_pairing_5s_display_start = CLEAR;
    }
    else /*if( gu8_Wifi_Connect_State == WIFI_DISCONNECT )*/
    {
        gu8_wifi_disp_timer++;

        if( gu8_wifi_disp_timer >= 10 )
        {
            gu8_wifi_disp_timer = 0;
        }
        else{}

        if( gu8_wifi_ap_mode == SET || gu8_ble_ap_mode == SET )
        {
            if( bit_install_flushing_state == SET )
            {
                /*..hui [24-8-6???? 9:49:17] ??? ?÷???????? ?????????? ???? ???..*/
                gu16_wifi_pairing_30min_timer = 0;
                gu8_wifi_pairing_5sec_timer = 0;
                bit_pairing_5s_display_start = CLEAR;
                wifi_pairing_display();
            }
            else
            {
                gu16_wifi_pairing_30min_timer++;

                if( gu16_wifi_pairing_30min_timer >= WIFI_PAIRING_START_30_MIN )
                {
                    /*..hui [24-8-6???? 9:50:14] ???????? 30?е??? ??? ?? OFF ???..*/
                    gu16_wifi_pairing_30min_timer  = WIFI_PAIRING_START_30_MIN;

                    if( bit_pairing_5s_display_start == SET
                    && gu8_Led_Display_Step != LED_Display__ICE_WATER_EXTRACT 
                    && gu8_Led_Display_Step != LED_Display__MY_WATER_EXTRACT
                    && gu8_Led_Display_Step != LED_Display__WATER_EXTRACT
                    && gu8_Led_Display_Step != LED_Display__ICE_EXTRACT
                    )
                    {
                        gu8_wifi_pairing_5sec_timer++;

                        /*..hui [24-8-6???? 9:58:42] ?? ???? ?????? 5?? ???? ???..*/
                        if( gu8_wifi_pairing_5sec_timer >= 50 )
                        {
                            gu8_wifi_pairing_5sec_timer = 0;
                            bit_pairing_5s_display_start = CLEAR;
                        }
                        else{}

                        wifi_pairing_display();
                    }
                    else
                    {
                        Bit0_Front_Right_Led_Wifi_Icon_White = CLEAR;
                        Bit1_Front_Right_Led_Wifi_Icon_Blue = CLEAR;
                        gu8_wifi_disp_timer = 0;
                        gu8_wifi_pairing_5sec_timer = 0;
                    }
                }
                else
                {
                    gu8_wifi_pairing_5sec_timer = 0;
                    bit_pairing_5s_display_start = CLEAR;
                    wifi_pairing_display();
                }
            }
        }
        else
        {
            gu16_wifi_pairing_30min_timer = 0;
            gu8_wifi_pairing_5sec_timer = 0;
            bit_pairing_5s_display_start = CLEAR;

            if( gu8_wifi_disp_timer >= 5 )
            {
                Bit0_Front_Right_Led_Wifi_Icon_White = SET;
                //Bit4_Led_Wifi_Icon_Blue = CLEAR;
            }
            else
            {
                Bit0_Front_Right_Led_Wifi_Icon_White = CLEAR;
                //Bit4_Led_Wifi_Icon_Blue = CLEAR;
            }
        }
    }

}



