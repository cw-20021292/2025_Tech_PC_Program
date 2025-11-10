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
#include    "display_main.h"


void led_display(void);
void init_water_extract_display(void);
void init_ice_extract_display(void);
void ice_disp_key_check(void);
void get_popup(void);

bit bit_ice_disp_pause;
U8  gu8_ice_disp_pause_timer;
DISPLAY_STACK Extract_Stack;
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void led_display(void)
{
    U8 mu8_return = 0;
    static LED_STEP before_display;

    gu8_animation_time = BREATH_ANIMATION_TIME;
    gu8_animation_duty = BREATH_ANIMATION_DUTY;

    /*..hui [23-5-25오후 3:18:14] 디밍 애니메이션..*/
    breath_control();

    /*welcome_led_output();*/
    update_cold_temp();

    switch( gu8_Led_Display_Step )
    {
        case LED_Display__VERSION:

            if( before_display == LED_Display__VERSION
            || before_display > LED_Display__VERSION )
            {
                before_display = LED_Display__VERSION;
                //all_dimming_full_on();
            }
            else{}

            display_version();

            break;

        case LED_Display__ICE_WATER_EXTRACT:                    /* 얼음물 추출 */

            if( before_display != LED_Display__ICE_WATER_EXTRACT )
            {
                before_display = LED_Display__ICE_WATER_EXTRACT;
                off_all_control_led();
                init_water_extract_display();
                gu8_cold_temp_old = CLEAR;
                gu8_pure_temp_old = CLEAR;
            }
            else{}
            led_normal_state();
            fnd_left_ice_out();
            fnd_right_water_out_normal();

            break;

        case LED_Display__MY_WATER_EXTRACT:                      /* MY워터 추출 */

            if( before_display != LED_Display__MY_WATER_EXTRACT )
            {
                before_display = LED_Display__MY_WATER_EXTRACT;
                off_all_control_led();

                init_ice_extract_display();
                bit_return_ice = CLEAR;
                init_water_extract_display();
            }
            else{}
            led_normal_state();
            fnd_left_normal_state();
            // fnd_left_my_recipe();
            fnd_right_water_out();
            
            break;

        case LED_Display__WATER_EXTRACT:
            if( before_display != LED_Display__WATER_EXTRACT )
            {
                before_display = LED_Display__WATER_EXTRACT;
                off_all_control_led();

                // init_ice_extract_display();
                // init_water_extract_display();
            }
            else{}

            water_extract_led_output();
            fnd_left_normal_water_state();
            // fnd_left_normal_state();
            fnd_right_water_out();

            break;

        case LED_Display__ICE_EXTRACT:
            if( before_display != LED_Display__ICE_EXTRACT )
            {
                before_display = LED_Display__ICE_EXTRACT;
                off_all_control_led();

                init_ice_extract_display();
                // init_water_extract_display();
            }
            else{}

            ice_extract_led_output();
            fnd_left_ice_out();
            fnd_right_ice_out();

            break;

        case LED_Display__MAIN:
            if( before_display != LED_Display__MAIN )
            {
                if( before_display != LED_Display__ICE_WATER_EXTRACT )
				{
                    gu8_cold_temp_old = CLEAR;
                    gu8_pure_temp_old = CLEAR;
					gu16_cold_temp_update_timer = 0;
					gu16_pure_temp_update_timer = 0;

                    gu8_fnd_right_on_display_flag = CLEAR;
                    gu8_fnd_right_off_display_flag = CLEAR;
				}
				else{}

                before_display = LED_Display__MAIN;
                off_all_control_led();
            }
            else{}

            led_normal_state();
            fnd_left_normal_state();
            fnd_right_normal_state();
            break;

        case LED_Display__ALL_LOCK:

            if( before_display != LED_Display__ALL_LOCK )
            {
                before_display = LED_Display__ALL_LOCK;
                off_all_control_led();
            }
            else{}

            led_all_lock();
            fnd_left_all_lock();
            fnd_right_all_off();

            break;

        case LED_Display__FLUSHING:
            if( before_display != LED_Display__FLUSHING )
            {
                before_display = LED_Display__FLUSHING;
                off_all_control_led();
            }
            else{}

            led_flushing_state();
            fnd_left_flushing_state();
            fnd_right_flushing_state();
            break;

        case LED_Display__ERROR:
            if( before_display != LED_Display__ERROR )
            {
                before_display = LED_Display__ERROR;
                off_all_control_led();
                gu8_error_flick_timer = 0;
                gu8_error_flick_cnt = 0;
                bit_error_display_finish = CLEAR;
            }
            else{}

            led_error_state();
            fnd_left_error();
            fnd_right_error();
            break;

        case LED_Display__FILTER_COVER_OPEN:
            if( before_display != LED_Display__FILTER_COVER_OPEN )
            {
                before_display = LED_Display__FILTER_COVER_OPEN;
                off_all_control_led();
            }
            else{}

            led_filter_open_state();
            fnd_left_all_off();
            fnd_right_all_off();
        break;

        case LED_Display__MANUAL_DRAIN:

            if( before_display != LED_Display__MANUAL_DRAIN )
            {
                before_display = LED_Display__MANUAL_DRAIN;
                off_all_control_led();
            }
            else{}

            led_manual_drain_state();
            fnd_left_all_off();
            fnd_right_all_off();

            break;

        case LED_Display__TRAY_CLEAN:
            if( before_display != LED_Display__TRAY_CLEAN )
            {
                before_display = LED_Display__TRAY_CLEAN;
                off_all_control_led();
            }
            else{}

            led_tray_clean();
            fnd_left_all_off();
            fnd_right_tray_clean();
        break;

        case LED_Display__SETTING:
            if( before_display != LED_Display__SETTING )
            {
                before_display = LED_Display__SETTING;
                off_all_control_led();
                gu8_fnd_right_on_display_flag = CLEAR;
                gu8_fnd_right_off_display_flag = CLEAR;
            }
            else{}

            led_setting_state();
            fnd_left_setting();
            fnd_right_normal_state();
        break;

        case LED_Display__TIME_SETTING:
            if( before_display != LED_Display__TIME_SETTING )
            {
                before_display = LED_Display__TIME_SETTING;
                off_all_control_led();
            }
            else {  }

            led_time_setting_state();
            fnd_left_time_setting();
            fnd_right_time_setting();
        break;
                    
        case LED_Display__WIFI_PAIRING:
            if( before_display != LED_Display__WIFI_PAIRING )
            {
                before_display = LED_Display__WIFI_PAIRING;
                off_all_control_led();

                u8PairingStep = 0;
                gu8_pairing_finish_timer = 0;
                bit_display_wifi_error = CLEAR;
            }
            else{}
            
            led_wifi_pairing();
            fnd_wifi_set();
        break;

        case LED_Display__BUTTON_SET:
            if( before_display != LED_Display__BUTTON_SET )
            {
                before_display = LED_Display__BUTTON_SET;
                off_all_control_led();
            }
            else{}

            led_button_set();
            fnd_left_all_off();
            fnd_right_button_set();
        break;

        case LED_Display__SLEEP :
            if( before_display != LED_Display__SLEEP )
            {
                before_display = LED_Display__SLEEP;
                off_all_control_led();
            }
            else{}
            
            led_normal_state();
            fnd_left_normal_state();
            fnd_right_normal_state();
        break;

        case LED_Display__VOLUME_SETTING:
            if( before_display != LED_Display__VOLUME_SETTING )
            {
                before_display = LED_Display__VOLUME_SETTING;
                off_all_control_led();
            }
            else{}
            
            led_volume_setting_state();
            fnd_left_all_off();
            fnd_right_volume_setting();

        break;

        case LED_Display__MEMENTO:      /* Memento Here! */
            if( before_display != LED_Display__MEMENTO )
            {
                before_display = LED_Display__MEMENTO;
                off_all_control_led();
            }
            else{}
            
            display_memento();

        break;

        case LED_Display__FOTA_START:
            if( before_display != LED_Display__FOTA_START )
            {
                before_display = LED_Display__FOTA_START;
                off_all_control_led();
                //all_dimming_full_on();
            }
            else{}

            off_all_control_led();
            Bit0_Front_Right_Led_Wifi_Icon_White = SET;
            Bit5_Front_Under_Led_Welcome = CLEAR;
            Bit6_Front_Under_Led_Welcome_2 = CLEAR;
        break;

        case LED_Display__SELF_TEST:
            if( before_display != LED_Display__SELF_TEST )
            {
                before_display = LED_Display__SELF_TEST;
                off_all_control_led();
            }
            else{}

            led_self_test_state();
            fnd_left_all_off();
            fnd_right_self_test_state();
        break;

        /* Cody Water Line Clean Service */
        case LED_Display__CODY_WATER_LINE_CLEAN:
            if( before_display != LED_Display__CODY_WATER_LINE_CLEAN )
            {
                before_display = LED_Display__CODY_WATER_LINE_CLEAN;
                off_all_control_led();
            }
            else{}

            cody_service_water_icon();
            fnd_left_all_off();
            cody_service_water_fnd_right();
        break;

        case LED_Display__INSTRUCTION:
            if( before_display != LED_Display__INSTRUCTION )
            {
                before_display = LED_Display__INSTRUCTION;
                off_all_control_led();
                gu8_cold_temp_old = CLEAR;
                gu8_pure_temp_old = CLEAR;
            }
            else{}

            led_normal_state();
            fnd_left_normal_state();
            fnd_right_normal_state();

            break;

            case LED_Display__BRIGHT_TEST:
            if( before_display != LED_Display__INSTRUCTION )
            {
                before_display = LED_Display__INSTRUCTION;
                off_all_control_led();
                gu8_cold_temp_old = CLEAR;
                gu8_pure_temp_old = CLEAR;
            }
            else{}

            on_all_control_led();
            break;

                    
            default:
            gu8_Led_Display_Step = LED_Display__VERSION;
            break;
        }


    welcome_led_output();
    get_popup();
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void get_popup(void)
{
    if(( gu8_Led_Display_Step == LED_Display__ICE_WATER_EXTRACT )
    || ( gu8_Led_Display_Step == LED_Display__MY_WATER_EXTRACT )
    || (gu8_Led_Display_Step == LED_Display__WATER_EXTRACT)
    || (gu8_Led_Display_Step == LED_Display__ICE_EXTRACT)
    )
    {
        if( gu8_Error_Code > 0 )
        {
            if( gu8_Error_Code == 75
            || gu8_Error_Code == 76
            || gu8_Error_Code == 77
            || gu8_Error_Code == 78
            || gu8_Error_Code == 79)
            {
                gu8_error_popup_enable = CLEAR;
            }
            else
            {
                // if(gu8_Error_Code != 8)
                // {
                    gu8_error_popup_enable = SET;
                // }
                // else { }
            }
        }
        else{}


        /*..hui [24-4-5오전 9:11:09] 얼음탱크 앞UV는 아이스트레이랑 연동되기 떄문에 표시 안함..*/

        #if 1
        if( gu16_wifi_pairing_30min_timer >= WIFI_PAIRING_START_30_MIN )
        {
            /*..hui [24-8-6오전 10:52:13] 30분 경과후부터 추출 종료후 5초 동안 표시..*/
            bit_pairing_5s_display_start = SET;
        }
        else{}
        #endif
    }
    else
    {
        if( gu8_Led_Display_Step == LED_Display__MAIN )
        {
            gu8_error_popup_enable = CLEAR;
        }
        else{}
    }


}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void init_water_extract_display(void)
{
    gu8_fnd_water_step = 0;
    gu8_fnd_continue_timer = 0;
    //gu16_extract_display_timer = 0;
    gu16_extract_display_cnt = 0;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void init_ice_extract_display(void)
{
    gu8_fnd_left_ice_step = 0;
    gu8_fnd_left_ice_timer = 0;

    gu8_fnd_left_ice_water_step = 0;
    gu8_fnd_left_ice_water_timer = 0;

    gu8_fnd_right_ice_water_step = 0;
    gu8_fnd_right_ice_water_timer = 0;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void ice_disp_key_check(void)
{
    /*..hui [23-8-2오후 3:35:25] 얼음 추출 -> 버튼 누름 -> 일반화면 전환 -> 7초후 얼음추출화면 전환..*/
    //if( F_IceOut == SET && gu8_Led_Display_Step == LED_Display__MY_WATER_EXTRACT )
    //{
    //    bit_ice_disp_pause = SET;
    //}
    //else{}

    gu8_ice_disp_pause_timer = 0;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/



