/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Input.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#include    "Macrodriver.h"
#include    "Global_Variable.h"
#include    "Port_Define.h"
#include    "M4_Input.h"


void Input(void);
//void input_select_bar(void);
//void input_cds(void);

//void input_overflow(void);
void check_ice_full(void);
void check_ice_low(void);       /* 저빙검사 추가 250224 CH.PARK */
void ice_tank_amount_check(void);   /* 저빙,만빙,중빙 판단 추가 250428 CH.PARK */

//void LeverInput(void);

//bit F_Bar;
//bit F_BeBar;
//bit F_BarSet;
//bit F_SelectBar;
//U8 gu8Bar;

Ice_Amount gu8_ice_tank_ice_amount;
Ice_Amount gu8_ice_tank_ice_amount_old;


bit F_NoUseSave;
bit F_DayNoUseSave;
bit F_NightNoUseSave;             // 낮 해제를 위해

//----------------------------------------------------// IR
/* 만빙 입력 변수 */
U8 gu8IRTime, gu8IRCount;
U16 gu16IRInterval;

/* 저빙 입력 변수 */
U8 gu8IR_l_Time, gu8IR_l_Count;
U16 gu16IR_l_Interval;
/****************************************************************************/
bit F_IceFull;                        // 만빙
bit F_IR;                             // 만빙 검사
/****************************************************************************/
bit F_IceLack;                    // 저빙
bit F_Low_IR;                         // 저빙 검사

/*..hui [18-3-8오후 5:25:49] 처음 전원 ON시 만빙 체크..*/
bit bit_first_ice_full;

U8 gu8ContyCancel;
U8 gu8Out;
U16 gu16Conty;

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void Input(void)
{
    /*..hui [19-6-26오후 1:54:08] 수위센서 입력..*/
    input_water_level();

    /*..hui [19-6-26오후 1:12:26] 트레이 마이크로스위치 입력..*/
    input_ice_tray_micro_sw();

    /*..hui [19-6-26오후 1:54:12] 만빙 입력..*/
    check_ice_full();

    /* 저빙 입력 250224 CH.PARK */
    check_ice_low();

    /* 만,중,저빙 판단 250428 CH.PARK */
    ice_tank_amount_check();

    /*..hui [19-7-22오후 7:36:53] 유량센서..*/
    calculate_flow_input();

    /*..hui [23-6-9오후 2:20:05] 필터 유량센서..*/
    /////calculate_filter_flow_input();

    /*..hui [19-7-25오후 8:25:00] 조도센서 다시 추가됨..*/
    /*..hui [23-2-28오후 3:34:11] 조도센서 없음..*/
    /*input_cds();*/

    /*detect_front_cds();*/
    
    /* 필터리드, 필터커버, 필터교체 등 관련 함수 */
    input_filter();

}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void check_ice_full(void)
{
    if(gu8IRTime > 0)
    {
        gu8IRTime--;                             // 만빙검사 10초
    }
    else{}

    //==================================================================
    if(F_IR != SET && F_LineTest != SET)
    {
        return;
    }
    else{}
//
    if(pIR_POWER != SET)                                         // IR 출력 없을경우
    {
        pIR_POWER = SET;

        if(F_LineTest == SET)
        {
            gu8IRTime = 30;
        }
        else
        {
            gu8IRTime = 100;
        }
    }
    else{}

    if(gu16ADIceFull >= ICEFULL_LEV)
    {
        gu8IRCount++;
    }
    else{}

    if(gu8IRTime == 0)
    {
        if(F_LineTest == SET)
        {
            if(gu8IRCount >= 10)
            {
                F_IceFull = CLEAR;
            }
            else
            {
                F_IceFull = SET;
            }
        }
        else
        {
            if(gu8IRCount >= 50)
            {
                F_IceFull = CLEAR;
            }
            else
            {
                F_IceFull = SET;
            }
        }

        gu8IRCount = 0;
        pIR_POWER = CLEAR;
        F_IR = CLEAR;

        if(F_LineTest == SET)
        {
            gu16IRInterval = 0;
        }
        else
        {
            gu16IRInterval = IR_INTERVAL;       /* 만빙검사 : 얼음 추출하지 않으면 15분마다 검사 진행 */
        }
    }
}

/****************************************************************************************************************/
/**
 * @brief 저빙 입력 체크
 * 
 */
void check_ice_low(void)
{
    if(gu8IR_l_Time > 0)
    {
        gu8IR_l_Time--;                             // 만빙검사 10초
    }
    else{}

    //==================================================================
    if(F_Low_IR != SET && F_LineTest != SET)
    {
        return;
    }
    else{}
//
    if(pIR_POWER2 != SET)                                         // IR 출력 없을경우
    {
        pIR_POWER2 = SET;

        if(F_LineTest == SET)
        {
            gu8IR_l_Time = 30;
        }
        else
        {
            gu8IR_l_Time = 100;
        }
    }
    else{}

    if(gu16ADIceLow <= ICEFULL_LEV)     // 1kg와 동일하게 적용
    {
        gu8IR_l_Count++;
    }
    else{}

    if(gu8IR_l_Time == 0)
    {
        if(F_LineTest == SET)
        {
            if(gu8IR_l_Count >= 10)
            {
                F_IceLack = CLEAR;      /* 미감지 */
            }
            else
            {
                F_IceLack = SET;        /* 감지 */
            }
        }
        else
        {
            if(gu8IR_l_Count >= 50)
            {
                F_IceLack = CLEAR;      /* 미감지 */
            }
            else
            {
                F_IceLack = SET;        /* 감지 */
            }
        }

        gu8IR_l_Count = 0;
        pIR_POWER2 = CLEAR;
        F_Low_IR = CLEAR;

        if(F_LineTest == SET)
        {
            gu16IR_l_Interval = 0;
        }
        else
        {
            gu16IR_l_Interval = IR_INTERVAL;       /* 저빙검사 : 얼음 추출하지 않으면 15분마다 검사 진행 */
        }
    }

    /////////F_IceFull = CLEAR;
    //F_Cold_Enable = CLEAR;

}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  : 얼음탱크 내 얼음잔량 판단
***********************************************************************************************************************/
void ice_tank_amount_check(void)
{
    if((F_IceFull == SET)			/* 만빙 */
    && (F_IceLack == SET)
    )
    {
        gu8_ice_tank_ice_amount = ICE_TANK_FULL;
    }
    else if((F_IceFull == CLEAR)	/* 저빙 */
    && (F_IceLack == CLEAR)
    )
    {
        gu8_ice_tank_ice_amount = ICE_TANK_LOW;
    }
    else if((F_IceFull == CLEAR)	/* 저빙 ~ 만빙 사이 */
    && (F_IceLack == SET)
    )
    {
        gu8_ice_tank_ice_amount = ICE_TANK_MID;
    }
    else 
    {
        gu8_ice_tank_ice_amount = ICE_TANK_MID;
    }

    if(gu8_ice_tank_ice_amount != gu8_ice_tank_ice_amount_old)
    {
        gu8_ice_tank_ice_amount_old = gu8_ice_tank_ice_amount;

        if(gu8_ice_tank_ice_amount == ICE_TANK_LOW)
        {
            /* 고객피로도 이슈로 삭제 */
            // Play_Voice(VOICE_156_ICE_30_PERCENT_UNDER);
        }
        else {  } 
    }
    else {  } 
}

