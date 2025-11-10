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
#include    "display_led_error.h"

void led_error_state(void);

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void led_error_state(void)
{
    funcition_led_output();
    /*temp_led_out();*/
    /*level_led_out();*/
    setting_led_output();
    extract_led_output();
    /*uv_led_out();*/

    /*icon_led_out();*/
    /*current_text_output();*/
    under_icon_led_output();
}



/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/







