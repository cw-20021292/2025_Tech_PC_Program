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
#include    "swing_bar.h"
#include    "App_Comm_Protocol.h"

void output_swing_bar(void);


//===================================== 투명얼음
bit F_CristalIce;
bit F_Cristal;
U8 gu8_cristal_timer;


U8 onon;
U8 offoff;

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void output_swing_bar(void)
{
    if(gu8_cristal_timer > 0)
    {
        gu8_cristal_timer--;
    }
    else{}

    if(Bit2_Ice_Operation_Disable_State == SET
    || F_ErrTrayMotor_DualInital == SET
    || F_Safety_Routine == SET)
    {
        F_CristalIce = 0;
        pSWING_BAR = 0;
        return;
    }
    else{}


    if(F_CristalIce != SET)
    {
        gu8_cristal_timer = 0;
        pSWING_BAR = 0;                              // 스윙바정지
        return;
    }
    else{}


    if(gu16IceMakeTime <= 5)
    {
        gu8_cristal_timer = 0;
        pSWING_BAR = 0;                              // 스윙바정지
        return;
    }
    else{}

    if(gu8_cristal_timer == 0 && gu8IceStep == STATE_31_MAIN_ICE_MAKING)          // 제빙구간 스윙바 동작
    {
        F_Cristal = !F_Cristal;

        if(F_Cristal == SET)
        {
            if(GetB2SwingbarOn() > 0)
            {
                gu8_cristal_timer = GetB2SwingbarOn();
            }
            else
            {
                gu8_cristal_timer = CRISTAL_ON;
            }

            pSWING_BAR = 1;
        }
        else
        {
            if(GetB2SwingbarOff() > 0)
            {
                gu8_cristal_timer = GetB2SwingbarOff();
            }
            else
            {
                gu8_cristal_timer = CRISTAL_OFF;
            }

            pSWING_BAR = 0;
        }
    }
    else{}
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/




