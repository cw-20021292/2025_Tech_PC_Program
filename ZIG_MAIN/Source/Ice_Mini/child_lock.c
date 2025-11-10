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
#include    "child_lock.h"


void child_lock(void);


bit F_Child_Lock;


bit bit_temporary_release;
U8 gu8_child_release_timer;

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void child_lock(void)
{
    if( F_WaterOut == SET || F_IceOut == SET )
    {
        gu8_child_release_timer = 0;
    }
    else{}

    if( bit_setting_mode_start == SET )
    {
        gu8_child_release_timer = 0;
    }
    else{}

    if( bit_altitude_setting_start == SET )
    {
        gu8_child_release_timer = 0;
    }
    else{}

    if( bit_time_setting_start == SET )
    {
        gu8_child_release_timer = 0;
    }
    else{}

    if( F_All_Lock == SET )
    {
        if( bit_child_lock_enable == SET )
        {
            if( bit_temporary_release == SET )
            {
                gu8_child_release_timer++;

                if( gu8_child_release_timer >= 70 )
                {
                    bit_temporary_release = CLEAR;
                    F_Child_Lock = SET;
                }
                else
                {
                    F_Child_Lock = CLEAR;
                }
            }
            else
            {
                F_Child_Lock = SET;
                gu8_child_release_timer = 0;
            }
        }
        else
        {
            F_Child_Lock = SET;
            bit_temporary_release = CLEAR;
            gu8_child_release_timer = 0;
        }
    }
    else
    {
        F_Child_Lock = CLEAR;
        bit_temporary_release = CLEAR;
        gu8_child_release_timer = 0;
    }


}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/




