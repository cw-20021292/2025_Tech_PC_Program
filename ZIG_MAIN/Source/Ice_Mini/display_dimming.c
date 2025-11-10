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
#include    "display_dimming.h"


//void set_led_duty( U8 mu8_number, U8 mu8_duty );
//void dimming_on_off_select( U8 mu8_number, U8 mu8_on_off );
//void all_dimming_full_on(void);
void set_duty_percent( U8 mu8_number, U8 mu8_percent );
void all_duty_100_percent(void);


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void set_duty_percent( U8 mu8_number, U8 mu8_percent )
{
    if(  mu8_number >= DIMMING__PERCENT_MAX )
    {
        return;
    }
    else{}

    if( mu8_percent > DIIMMING__100_PERCENT )
    {
        mu8_percent = DIIMMING__100_PERCENT;
    }
    else{}

    switch( mu8_number )
    {
        case DIMMING__PERCENT_ICE_SELECT:

            bit_0_3_ice_select = mu8_percent;

        break;

        case DIMMING__PERCENT_ICE_WATER_SELECT:

            bit_4_7_ice_water_select = mu8_percent;

        break;

        case DIMMING__PERCENT_HOT_SELECT:

            bit_0_3_hot_select = mu8_percent;

        break;

        case DIMMING__PERCENT_AMBIENT_SELECT:

            bit_4_7_ambient_select = mu8_percent;

        break;

        case DIMMING__PERCENT_COLD_SELECT:

            bit_0_3_cold_select = mu8_percent;

        break;

        case DIMMING__PERCENT_AMOUNT_SELECT:

            bit_4_7_amount_select = mu8_percent;

        break;

        case DIMMING__PERCENT_SETTING_ICE_SIZE:

            bit_0_3_setting_ice_size = mu8_percent;

        break;

        case DIMMING__PERCENT_SETTING_ICE_OFF:

            bit_4_7_setting_ice_off = mu8_percent;

        break;

        case DIMMING__PERCENT_SETTING_ICE_FIRST:

            bit_0_3_setting_ice_first = mu8_percent;

        break;

        case DIMMING__PERCENT_SETTING_COLD_TEMP:

            bit_4_7_setting_cold_temp = mu8_percent;

        break;

        case DIMMING__PERCENT_SETTING_SLEEP_MODE:

            bit_0_3_setting_sleep_mode = mu8_percent;

        break;

        case DIMMING__PERCENT_SETTING_ICE_LOCK:

            bit_4_7_setting_ice_lock = mu8_percent;

        break;

        case DIMMING__PERCENT_SETTING_ALL_LOCK:

            bit_0_3_all_lock = mu8_percent;

        break;

        case DIMMING__PERCENT_SETTING_HOT_LOCK:

            bit_4_7_hot_lock = mu8_percent;

        break;

        case DIMMING__PERCENT_ICE_FULL_TEXT:

            bit_0_3_ice_full_txt = mu8_percent;

        break;

        case DIMMING__PERCENT_UV_STER_TEXT:

            bit_4_7_UV_ster_txt = mu8_percent;

        break;

        case DIMMING__PERCENT_HOT_STER_TEXT:

            bit_0_3_hot_ster_txt = mu8_percent;

        break;

        case DIMMING__PERCENT_SLEEP_ICON:

            bit_4_7_sleep_icon = mu8_percent;

        break;

        case DIMMING__PERCENT_STEP_TEXT:

            bit_0_3_step_txt = mu8_percent;

        break;

        case DIMMING__PERCENT_CLOCK_COLON_ICON:

            bit_4_7_clock_colon_icon = mu8_percent;

        break;

        case DIMMING__PERCENT_oC_ICON:

            bit_0_3_oC_icon = mu8_percent;

        break;

        case DIMMING__PERCENT_WIFI_WHITE_ICON:

            bit_0_3_wifi_white_icon = mu8_percent;

        break;

        case DIMMING__PERCENT_MY_SELECT:

            bit_4_7_my_select = mu8_percent;

        break;

        case DIMMING__PERCENT_SUNNY_ICON:

            bit_0_3_sunny_icon = mu8_percent;

        break;

        case DIMMING__PERCENT_CLOUD_ICON:

            bit_4_7_cloud_icon = mu8_percent;

        break;

        case DIMMING__PERCENT_RAIN_ICON:

            bit_0_3_rain_icon = mu8_percent;

        break;

        case DIMMING__PERCENT_PERCENT_ICON:

            bit_4_7_percent_icon = mu8_percent;

        break;

        case DIMMING__PERCENT_LITER_TEXT:

            bit_0_3_liter_txt = mu8_percent;

        break;

        case DIMMING__PERCENT_ml_TEXT:

            bit_4_7_ml_txt = mu8_percent;

        break;

        case DIMMING__PERCENT_TEMP_BAR_1_1:

            bit_0_3_temp_bar_1_1 = mu8_percent;

        break;

        case DIMMING__PERCENT_TEMP_BAR_1_2:

            bit_4_7_temp_bar_1_2 = mu8_percent;

        break;

        case DIMMING__PERCENT_TEMP_BAR_1_3:

            bit_0_3_temp_bar_1_3 = mu8_percent;

        break;

        case DIMMING__PERCENT_TEMP_BAR_1_4:

            bit_4_7_temp_bar_1_4 = mu8_percent;

        break;

        case DIMMING__PERCENT_TEMP_BAR_1_5:

            bit_4_7_temp_bar_1_5 = mu8_percent;

        break;

        case DIMMING__PERCENT_AMOUNT_BAR_2_1:

            bit_0_3_amount_bar_2_1 = mu8_percent;

        break;

        case DIMMING__PERCENT_AMOUNT_BAR_2_2:

            bit_4_7_amount_bar_2_2 = mu8_percent;

        break;

        case DIMMING__PERCENT_AMOUNT_BAR_2_3:

            bit_0_3_amount_bar_2_3 = mu8_percent;

        break;

        case DIMMING__PERCENT_AMOUNT_BAR_2_4:

            bit_0_3_amount_bar_2_4 = mu8_percent;

        break;

        case DIMMING__PERCENT_AMOUNT_BAR_CONTINUE:

            bit_4_7_amount_bar_continue = mu8_percent;

        break;

        case DIMMING__PERCENT_RAMEN_TEXT:

            bit_0_3_recipe_ramen_txt = mu8_percent;

        break;

        case DIMMING__PERCENT_DRIP_COFFEE_TEXT:

            bit_4_7_recipe_drip_coffee_txt = mu8_percent;

        break;

        case DIMMING__PERCENT_MY_1_TEXT:

            bit_0_3_MY_1_txt = mu8_percent;

        break;

        case DIMMING__PERCENT_MY_2_TEXT:

            bit_4_7_MY_2_txt = mu8_percent;

        break;


        case DIMMING__PERCENT_ICE_LARGE_TEXT:

            bit_0_3_ice_large_txt = mu8_percent;

        break;

        case DIMMING__PERCENT_ICE_CENTER_BAR:

            bit_4_7_ice_center_bar = mu8_percent;

        break;

        case DIMMING__PERCENT_ICE_SMALL_TEXT:

            bit_0_3_ice_small_txt = mu8_percent;

        break;

        case DIMMING__PERCENT_COLD_HIGH_TEXT:

            bit_0_3_cold_high_txt = mu8_percent;

        break;

        case DIMMING__PERCENT_COLD_CENTER_BAR:

            bit_4_7_cold_center_bar = mu8_percent;

        break;

        case DIMMING__PERCENT_COLD_LOW_TEXT:

            bit_0_3_cold_low_txt = mu8_percent;

        break;

        case DIMMING__PERCENT_ICE_LOCK_ICON:

            bit_0_3_ice_lock_icon = mu8_percent;

        break;

        case DIMMING__PERCENT_HOT_LOCK_ICON:

            bit_4_7_hot_lock_icon = mu8_percent;

        break;

        case DIMMING__PERCENT_COMP_WHITE_DOT:

            bit_0_3_comp_operation_dot = mu8_percent;

        break;

        case DIMMING__PERCENT_ICE_EXTRACT:

            bit_0_3_ice_extract = mu8_percent;

        break;

        case DIMMING__PERCENT_WATER_EXTRACT:

            bit_4_7_water_extract = mu8_percent;

        break;

        case DIMMING__PERCENT_WELCOME_LED:
            bit_0_3_welcome_led = mu8_percent;
        break;

        /* 아래로 LED ON Percent 출력 정의 추가 250211 CH.PARK */
        case DIMMING__PERCENT_COLD_OFF: 
        bit_4_7_Cold_Off = mu8_percent; 
        break;
        
        case DIMMING__PERCENT_ICE_SHORTAGE_TEXT:
        bit_0_3_Ice_Shortage = mu8_percent;
        break;
        
        case DIMMING__PERCENT_EXT_FAUCET_TEXT:
        bit_4_7_eff_Faucet_Text = mu8_percent; 
        break;

        case DIMMING__PERCENT_ICE_FAUCET_TEXT:
        bit_0_3_Ice_Faucet = mu8_percent; 
        break;
        
        case DIMMING__PERCENT_ICE_TRAY_TEXT:
        bit_4_7_Ice_Tray = mu8_percent; 
        break;
        
        case DIMMING__PERCENT_ICE_STORAGE_BOX_TEXT:
        bit_0_3_Ice_Storage_Box = mu8_percent; 
        break;

        case DIMMING__PERCENT_TEA_TEXT:
        bit_4_7_Tea_Text = mu8_percent; 
        break;

        case DIMMING__PERCENT_MY_3_TEXT:
        bit_0_3_My_3_txt = mu8_percent; 
        break;

        case DIMMING__PERCENT_ICE_NORMAL:
        bit_4_7_ice_normal_txt = mu8_percent; 
        break;

        case DIMMING__PERCENT_ICE_PIECE:
        bit_4_7_ice_piece_txt = mu8_percent; 
        break;

        case DIMMING__PERCENT_WELCOME_LED_2:
        bit_4_7_Welcome_2 = mu8_percent;
        break;

        case DIMMING__PERCENT_FILTER_CLEAN:
        bit_4_7_Filter_Clean = mu8_percent;
        break;

        case DIMMING__PERCENT_RIGHT_SEG_DOT:
        bit_0_3_Right_Seg_Dot = mu8_percent;
        break;
        
        case DIMMING__PERCENT_ICE_WATER_EXTRACT_OUTER:
        bit_0_3_Ice_Water_Extract = mu8_percent;
        break;

        case DIMMING__PERCENT_ICE_EXTRACT_INNER:
        bit_0_3_ice_extract_inner = mu8_percent;
        break;

        case DIMMING__PERCENT_SEG_LEFT_HUNDRED:
        bit_0_3_Seg_Left_Hundred = mu8_percent;
        break;

        case DIMMING__PERCENT_SEG_LEFT_TEN:
        bit_4_7_Seg_Left_Ten = mu8_percent;
        break;

        case DIMMING__PERCENT_SEG_LEFT_ONE:
        bit_0_3_Seg_Left_One = mu8_percent;
        break;

        case DIMMING__PERCENT_SEG_RIGHT_HUNDRED:
        bit_4_7_Seg_Right_Hundred = mu8_percent;
        break;

        case DIMMING__PERCENT_SEG_RIGHT_TEN:
        bit_0_3_Seg_Right_Ten = mu8_percent;
        break;

        case DIMMING__PERCENT_SEG_RIGHT_ONE:
        bit_4_7_Seg_Right_One = mu8_percent;
        break;

        case DIMMING__PERCENT_COMP_BLUE_DOT:
        bit_4_7_comp_blue_dot = mu8_percent;
        break;

        case DIMMING__PERCENT_FAST_ICE_ICON:
        bit_4_7_fast_ice_icon = mu8_percent;
        break;

        case DIMMING__PERCENT_ICE_WATER_EXTRACT_INNER:
        bit_4_7_Ice_Water_Inner_Extract = mu8_percent;
        break;

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////


        default:

        break;
    }



}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void all_duty_100_percent(void)
{
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
    set_duty_percent( DIMMING__PERCENT_SEG_LEFT_HUNDRED, SEGMENT__DIMMING__90_PERCENT );
    set_duty_percent( DIMMING__PERCENT_SEG_LEFT_TEN, SEGMENT__DIMMING__90_PERCENT );
    set_duty_percent( DIMMING__PERCENT_SEG_LEFT_ONE, SEGMENT__DIMMING__90_PERCENT );
    
    set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_HUNDRED, SEGMENT__DIMMING__90_PERCENT );
    set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_TEN, SEGMENT__DIMMING__90_PERCENT );
    set_duty_percent( DIMMING__PERCENT_SEG_RIGHT_ONE, SEGMENT__DIMMING__90_PERCENT );
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
    /*..hui [24-2-22오후 5:26:02] 기능부..*/////////////////////////////////
    set_duty_percent( DIMMING__PERCENT_ICE_SELECT, FUNCTION_ICON__DIMMING__70_PERCENT );
    set_duty_percent( DIMMING__PERCENT_ICE_WATER_SELECT, FUNCTION_ICON__DIMMING__70_PERCENT );
    set_duty_percent( DIMMING__PERCENT_HOT_SELECT, FUNCTION_ICON__DIMMING__70_PERCENT );
    set_duty_percent( DIMMING__PERCENT_AMBIENT_SELECT, FUNCTION_ICON__DIMMING__70_PERCENT );
    set_duty_percent( DIMMING__PERCENT_COLD_SELECT, FUNCTION_ICON__DIMMING__70_PERCENT );
    set_duty_percent( DIMMING__PERCENT_AMOUNT_SELECT, FUNCTION_ICON__DIMMING__70_PERCENT );
    set_duty_percent( DIMMING__PERCENT_MY_SELECT, FUNCTION_ICON__DIMMING__70_PERCENT );
    
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
    /*..hui [24-2-22오후 5:26:07] 설정부..*/
    set_duty_percent( DIMMING__PERCENT_SETTING_ICE_SIZE, DIIMMING__100_PERCENT );
    set_duty_percent( DIMMING__PERCENT_SETTING_ICE_OFF, DIIMMING__100_PERCENT );

    set_duty_percent( DIMMING__PERCENT_SETTING_ICE_FIRST, DIIMMING__100_PERCENT );
    set_duty_percent( DIMMING__PERCENT_SETTING_COLD_TEMP, DIIMMING__100_PERCENT );

    set_duty_percent( DIMMING__PERCENT_SETTING_SLEEP_MODE, DIIMMING__100_PERCENT );
    set_duty_percent( DIMMING__PERCENT_SETTING_ICE_LOCK, DIIMMING__100_PERCENT );

    set_duty_percent( DIMMING__PERCENT_SETTING_ALL_LOCK, DIIMMING__100_PERCENT );
    set_duty_percent( DIMMING__PERCENT_SETTING_HOT_LOCK, DIIMMING__100_PERCENT );
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
    /*..hui [24-2-22오후 5:32:07] 상단아이콘부..*/
    set_duty_percent( DIMMING__PERCENT_ICE_FULL_TEXT, DIIMMING__100_PERCENT );
    set_duty_percent( DIMMING__PERCENT_UV_STER_TEXT, DIIMMING__100_PERCENT );

    set_duty_percent( DIMMING__PERCENT_HOT_STER_TEXT, DIIMMING__100_PERCENT );
    set_duty_percent( DIMMING__PERCENT_SLEEP_ICON, DIIMMING__100_PERCENT );    /////

    set_duty_percent( DIMMING__PERCENT_STEP_TEXT, DIIMMING__100_PERCENT );
    set_duty_percent( DIMMING__PERCENT_CLOCK_COLON_ICON, DIIMMING__100_PERCENT );

    set_duty_percent( DIMMING__PERCENT_oC_ICON, DIIMMING__100_PERCENT );

    set_duty_percent( DIMMING__PERCENT_WIFI_WHITE_ICON, DIIMMING__100_PERCENT );

    set_duty_percent( DIMMING__PERCENT_SUNNY_ICON, DIIMMING__100_PERCENT );    /////
    set_duty_percent( DIMMING__PERCENT_CLOUD_ICON, DIIMMING__100_PERCENT );

    set_duty_percent( DIMMING__PERCENT_RAIN_ICON, DIIMMING__100_PERCENT );
    set_duty_percent( DIMMING__PERCENT_PERCENT_ICON, DIIMMING__100_PERCENT );

    set_duty_percent( DIMMING__PERCENT_LITER_TEXT, DIIMMING__100_PERCENT );    /////
    set_duty_percent( DIMMING__PERCENT_ml_TEXT, DIIMMING__100_PERCENT );

    set_duty_percent( DIMMING__PERCENT_TEMP_BAR_1_1, DIIMMING__100_PERCENT );
    set_duty_percent( DIMMING__PERCENT_TEMP_BAR_1_2, DIIMMING__100_PERCENT );

    set_duty_percent( DIMMING__PERCENT_TEMP_BAR_1_3, DIIMMING__100_PERCENT );
    set_duty_percent( DIMMING__PERCENT_TEMP_BAR_1_4, DIIMMING__100_PERCENT );

    set_duty_percent( DIMMING__PERCENT_AMOUNT_BAR_2_1, DIIMMING__100_PERCENT );
    set_duty_percent( DIMMING__PERCENT_AMOUNT_BAR_2_2, DIIMMING__100_PERCENT );

    set_duty_percent( DIMMING__PERCENT_AMOUNT_BAR_2_3, DIIMMING__100_PERCENT );
    set_duty_percent( DIMMING__PERCENT_AMOUNT_BAR_CONTINUE, DIIMMING__100_PERCENT );

    set_duty_percent( DIMMING__PERCENT_RAMEN_TEXT, DIIMMING__100_PERCENT );
    set_duty_percent( DIMMING__PERCENT_DRIP_COFFEE_TEXT, DIIMMING__100_PERCENT );

    set_duty_percent( DIMMING__PERCENT_MY_1_TEXT, DIIMMING__100_PERCENT );    /////
    set_duty_percent( DIMMING__PERCENT_MY_2_TEXT, DIIMMING__100_PERCENT );    /////

    set_duty_percent( DIMMING__PERCENT_ICE_LARGE_TEXT, DIIMMING__100_PERCENT );    /////
    set_duty_percent( DIMMING__PERCENT_ICE_CENTER_BAR, DIIMMING__100_PERCENT );    /////

    set_duty_percent( DIMMING__PERCENT_ICE_SMALL_TEXT, DIIMMING__100_PERCENT );    /////

    set_duty_percent( DIMMING__PERCENT_COLD_HIGH_TEXT, DIIMMING__100_PERCENT );    /////
    set_duty_percent( DIMMING__PERCENT_COLD_CENTER_BAR, DIIMMING__100_PERCENT );    /////

    set_duty_percent( DIMMING__PERCENT_COLD_LOW_TEXT, DIIMMING__100_PERCENT );    //////

    set_duty_percent( DIMMING__PERCENT_ICE_LOCK_ICON, DIIMMING__100_PERCENT );    /////
    set_duty_percent( DIMMING__PERCENT_HOT_LOCK_ICON, DIIMMING__100_PERCENT );    /////

    set_duty_percent( DIMMING__PERCENT_COMP_WHITE_DOT, DIIMMING__100_PERCENT );    /////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
    /*..hui [24-2-22오후 5:29:05] 중간 텍스트 아이콘부..*/
    set_duty_percent( DIMMING__PERCENT_WELCOME_LED, DIIMMING__100_PERCENT );

    /* LED 100% 출력 추가 250211 CH.PARK */
    set_duty_percent( DIMMING__PERCENT_COLD_OFF, DIIMMING__100_PERCENT );
    set_duty_percent( DIMMING__PERCENT_ICE_SHORTAGE_TEXT, DIIMMING__100_PERCENT );
    set_duty_percent( DIMMING__PERCENT_EXT_FAUCET_TEXT, DIIMMING__100_PERCENT );
    set_duty_percent( DIMMING__PERCENT_ICE_FAUCET_TEXT, DIIMMING__100_PERCENT );
    set_duty_percent( DIMMING__PERCENT_ICE_TRAY_TEXT, DIIMMING__100_PERCENT );
    set_duty_percent( DIMMING__PERCENT_ICE_STORAGE_BOX_TEXT, DIIMMING__100_PERCENT );
    set_duty_percent( DIMMING__PERCENT_TEMP_BAR_1_5, DIIMMING__100_PERCENT );
    set_duty_percent( DIMMING__PERCENT_AMOUNT_BAR_2_4, DIIMMING__100_PERCENT );
    set_duty_percent( DIMMING__PERCENT_TEA_TEXT, DIIMMING__100_PERCENT );
    set_duty_percent( DIMMING__PERCENT_MY_3_TEXT, DIIMMING__100_PERCENT );
    set_duty_percent( DIMMING__PERCENT_ICE_NORMAL, DIIMMING__100_PERCENT );
    set_duty_percent( DIMMING__PERCENT_ICE_PIECE, DIIMMING__100_PERCENT );
    set_duty_percent( DIMMING__PERCENT_WELCOME_LED_2, DIIMMING__100_PERCENT );
    set_duty_percent( DIMMING__PERCENT_FILTER_CLEAN, DIIMMING__100_PERCENT );
    set_duty_percent( DIMMING__PERCENT_RIGHT_SEG_DOT, DIIMMING__100_PERCENT );
    set_duty_percent( DIMMING__PERCENT_COMP_BLUE_DOT, DIIMMING__100_PERCENT );
    set_duty_percent( DIMMING__PERCENT_FAST_ICE_ICON, DIIMMING__100_PERCENT );
    
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
    /*..hui [24-2-22오후 5:26:40] 추출..*/
    set_duty_percent( DIMMING__PERCENT_ICE_EXTRACT, EXTRACT__DIMMING__40_PERCETN );
    set_duty_percent( DIMMING__PERCENT_WATER_EXTRACT, EXTRACT__DIMMING__40_PERCETN );
    set_duty_percent( DIMMING__PERCENT_ICE_WATER_EXTRACT_OUTER, EXTRACT__DIMMING__40_PERCETN );
    set_duty_percent( DIMMING__PERCENT_ICE_EXTRACT_INNER, EXTRACT__DIMMING__40_PERCETN );
    set_duty_percent( DIMMING__PERCENT_ICE_WATER_EXTRACT_INNER, EXTRACT__DIMMING__40_PERCETN );
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/




