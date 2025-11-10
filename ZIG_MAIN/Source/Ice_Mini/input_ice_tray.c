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
#include    "input_ice_tray.h"



void input_ice_tray_micro_sw(void);

U16 gu16_ice_level_delay;
//----------------------------------------------------// 얼음받이센서
U8 gu8IceLEV;
U8 gu8BeIceLEV;
U8 gu8IceTrayLEV;
U8 u8TestIceTrayDelay;
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
//""SUBR COMMENT""************************************************************
// ID         : Ice Tray Level
// 개요         : 얼음받이 센서
//----------------------------------------------------------------------------
// 기능       :  1.gu8IceTrayLEV = 0 : 제빙위치
//               2.gu8IceTrayLEV = 1 : 탈빙위치
//
//----------------------------------------------------------------------------
//""SUBR COMMENT END""********************************************************
void input_ice_tray_micro_sw(void)
{
    if(gu16_ice_level_delay > 0)
    {
        gu16_ice_level_delay--;            // Ice Tray 딜레이
    }
    else{}
	
    //============================================================ 트레이 위치 감지
    if(pICE_MICRO_SW_HIGH != SET && pICE_MICRO_SW_LOW == SET)
    {
        gu8IceLEV = ICE_TRAY_POSITION_ICE_THROW;            // 탈빙위치 (2KG는 반대로 되어있음)
    }
    else if(pICE_MICRO_SW_HIGH == SET && pICE_MICRO_SW_LOW != SET)
    {
        gu8IceLEV = ICE_TRAY_POSITION_ICE_MAKING;           // 제빙위치 (2KG는 반대로 되어있음)
    }
    else if(pICE_MICRO_SW_HIGH == SET && pICE_MICRO_SW_LOW == SET)
    {
        gu8IceLEV = ICE_TRAY_POSITION_MOVING;        // 이동중
    }
    else if(pICE_MICRO_SW_HIGH != SET && pICE_MICRO_SW_LOW != SET)
    {
        gu8IceLEV = ICE_TRAY_POSITION_ERROR;        // 에러
    }
    else{}

    if(gu8IceLEV != gu8BeIceLEV)
    {
        gu8BeIceLEV = gu8IceLEV;
        /*gu16_ice_level_delay = 30;*/           // 트레이시간딜레이
        /*gu16_ice_level_delay = 3;*/           // 트레이시간딜레이
        gu16_ice_level_delay = 7;           // 트레이시간딜레이

        return;
    }
    else{}

    if(gu16_ice_level_delay > 0)
    {
        return;
    }
    else{}

    gu8IceTrayLEV = gu8IceLEV;

    if(gu8IceTrayLEV == ICE_TRAY_POSITION_ERROR && Bit17_Tray_Micro_SW_Dual_Detect_Error__E61 != SET)
    {
        F_ErrTrayMotor_DualInital = 1;     // 동시 감지면 Tray M/S Error 초기발생
    }
    else{}
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/


