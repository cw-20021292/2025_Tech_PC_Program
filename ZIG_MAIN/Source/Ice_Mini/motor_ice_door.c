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
#include    "motor_ice_select.h"
/***********************************************************************************************************************/
bit F_DoorCW;                    // 살균 후 ICE Door Reset
bit F_IceDoor2Close;        /* 아이스 도어 닫힘 */
bit bit_ice_off_door_close;
/***********************************************************************************************************************/
U16 gu16_Ice_Door_StepMotor;
U16 gu16IceDoorCloseTimer_Min = 0; /* 60s x 60min x 24h = 86400 24Hour */
U16 gu16IceDoorCloseTimer_Hour = 0; /* 60s x 60min x 24h = 86400 24Hour */
U16 gu16IceDoorCloseResetTimer = 0;
bit F_IceDoorClose;        /* 아이스 도어 닫힘 */
/***********************************************************************************************************************/
/* 15분마다 열고 닫는 사양 - 1안 */
// bit F_close;                /* 얼음추출 이후 플래그 */
// U8  u8_count;               /* 얼음추출 이후 2회 카운트 */
// U16 u16_close_min;          /* 얼음추출 이후 카운트값 */
// U8  u8_close_step;          /* 얼음추출 full open / full close 스텝 */
// U8  u8_step_timer_100ms;    /* 얼음추출 동작 관련 스텝타이머 */
/***********************************************************************************************************************/
/* 20분 강제 닫기용 변수들 */
U16 gu16IceDoor10MinTimer = 0;      /* 10분(600초) 타이머 */
bit F_IceDoor10MinClose;            /* 10분 강제 닫기 플래그 */
U16 gu16ShortIceDoorCloseResetTimer = 0;

/* 추출 시에만 set되는 변수*/
bit bit_ice_door_open_after_close;
bit bit_ice_off_door_close;
/***********************************************************************************************************************/
void motor_ice_door_output(void);
/***********************************************************************************************************************/
void ice_door_close_24_hour(void);
U8 finish_ice_setting(void);
extern MODEL model;
extern ICE_STUCK_1 IceStuck;
/***********************************************************************************************************************/
/**
 * @brief 최종 DOOR STEP모터 제어
 *
 */
void motor_ice_door_output(void)
{
    if(F_IceOpen == SET)                                       // 열림
    {
        if(F_DoorCW != SET)
        {
            F_DoorCW = SET;
        }
        else{}

        if(gu16_Ice_Door_StepMotor < STEP_ANGLE_DOOR)
        {
            gu16_Ice_Door_StepMotor++;
            //gu8ErrDoor = 30;
        }
        else
        {
            gu16_Ice_Door_StepMotor = STEP_ANGLE_DOOR;
            pSTEP_MOTOR_ICE_DOOR_1 = 0;
            pSTEP_MOTOR_ICE_DOOR_2 = 0;
            pSTEP_MOTOR_ICE_DOOR_3 = 0;
            pSTEP_MOTOR_ICE_DOOR_4 = 0;

            /* 얼음 추출중이 아니고 얼음 걸림 제어도 안하고 있을 때에만 클리어 */
            if((F_IceOut == CLEAR)
            && (IceStuck.u8IceJamResolveStep == PROCESS_ICE_JAM_INIT)
            )
            {
                F_IceOpen = CLEAR;              // Door 열림 완료 후 Off
            }
            else{}
        }
    }
    else                                                  // 닫힘
    {
        if(F_DoorCW == SET)
        {
            gu16_Ice_Door_StepMotor = STEP_ANGLE_DOOR;
            F_DoorCW = CLEAR;
        }
        else{}

        if(gu16_Ice_Door_StepMotor > 0)
        {
            /* 최종 도어는 3초 지연 후 Close */
            if(gu8IceClose == 0)
            {
                gu16_Ice_Door_StepMotor--;
            }
            else{}
        }
        else
        {
            gu16_Ice_Door_StepMotor = 0;
            pSTEP_MOTOR_ICE_DOOR_1 = 0;
            pSTEP_MOTOR_ICE_DOOR_2 = 0;
            pSTEP_MOTOR_ICE_DOOR_3 = 0;
            pSTEP_MOTOR_ICE_DOOR_4 = 0;
        }
    }

    if(gu16_Ice_Door_StepMotor == 0 || gu16_Ice_Door_StepMotor == STEP_ANGLE_DOOR)
    {
        pSTEP_MOTOR_ICE_DOOR_1 = 0;
        pSTEP_MOTOR_ICE_DOOR_2 = 0;
        pSTEP_MOTOR_ICE_DOOR_3 = 0;
        pSTEP_MOTOR_ICE_DOOR_4 = 0;
    }
    else
    {
        switch(gu16_Ice_Door_StepMotor % 8)
        {
            case 0 :

                    pSTEP_MOTOR_ICE_DOOR_1 = 1;
                    pSTEP_MOTOR_ICE_DOOR_2 = 0;
                    pSTEP_MOTOR_ICE_DOOR_3 = 0;
                    pSTEP_MOTOR_ICE_DOOR_4 = 0;

                    break;

            case 1 :

                    pSTEP_MOTOR_ICE_DOOR_1 = 1;
                    pSTEP_MOTOR_ICE_DOOR_2 = 0;
                    pSTEP_MOTOR_ICE_DOOR_3 = 0;
                    pSTEP_MOTOR_ICE_DOOR_4 = 1;

                    break;

            case 2 :

                    pSTEP_MOTOR_ICE_DOOR_1 = 0;
                    pSTEP_MOTOR_ICE_DOOR_2 = 0;
                    pSTEP_MOTOR_ICE_DOOR_3 = 0;
                    pSTEP_MOTOR_ICE_DOOR_4 = 1;

                    break;

            case 3 :

                    pSTEP_MOTOR_ICE_DOOR_1 = 0;
                    pSTEP_MOTOR_ICE_DOOR_2 = 0;
                    pSTEP_MOTOR_ICE_DOOR_3 = 1;
                    pSTEP_MOTOR_ICE_DOOR_4 = 1;

                    break;

            case 4 :

                    pSTEP_MOTOR_ICE_DOOR_1 = 0;
                    pSTEP_MOTOR_ICE_DOOR_2 = 0;
                    pSTEP_MOTOR_ICE_DOOR_3 = 1;
                    pSTEP_MOTOR_ICE_DOOR_4 = 0;

                    break;

            case 5 :

                    pSTEP_MOTOR_ICE_DOOR_1 = 0;
                    pSTEP_MOTOR_ICE_DOOR_2 = 1;
                    pSTEP_MOTOR_ICE_DOOR_3 = 1;
                    pSTEP_MOTOR_ICE_DOOR_4 = 0;

                    break;

            case 6 :

                    pSTEP_MOTOR_ICE_DOOR_1 = 0;
                    pSTEP_MOTOR_ICE_DOOR_2 = 1;
                    pSTEP_MOTOR_ICE_DOOR_3 = 0;
                    pSTEP_MOTOR_ICE_DOOR_4 = 0;

                    break;

            case 7 :

                    pSTEP_MOTOR_ICE_DOOR_1 = 1;
                    pSTEP_MOTOR_ICE_DOOR_2 = 1;
                    pSTEP_MOTOR_ICE_DOOR_3 = 0;
                    pSTEP_MOTOR_ICE_DOOR_4 = 0;

                    break;

            default:

                break;
        }
    }
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void ice_door_close_24_hour(void)
{
    U16 mu16_forced_close_time_min = 0;
    U16 mu16_forced_close_time_hour = 0;
    U8 mu8_return = 0;

    //====================================================
    /* 아이스 도어 주기적으로 닫는 로직( 24시간 기준으로 반복 )
     * 아이스 도어가 강제로 열린 경우를 가정해서 24시간 기준으로 닫아 준다.*/

    if( F_LineTest == SET )
    {
        /*..hui [18-1-23오후 2:29:46] 테스트 모드시 60초..*/
        mu16_forced_close_time_min = 600;
        /*..hui [18-1-23오후 2:29:52] 테스트 모드시 1분..*/
        mu16_forced_close_time_hour = 5;
    }
    else
    {
        /*..hui [18-1-23오후 2:29:03] 일반 모드시 60분..*/
        mu16_forced_close_time_min = 36000;
        /*..hui [18-1-23오후 2:29:09] 일반 모드시 24시간..*/
        mu16_forced_close_time_hour = 24;
    }

    mu8_return = finish_ice_setting();

    if( mu8_return == SET )
    {
        F_IceDoorClose = SET;
    }
    else{}

    if( F_IceOut == SET )
    {
        gu16IceDoorCloseTimer_Min = 0;
        gu16IceDoorCloseTimer_Hour = 0;
    }
    else{}

    /*..hui [18-1-23오후 2:12:10] 60분 타이머..*/
    gu16IceDoorCloseTimer_Min++;

    if(gu16IceDoorCloseTimer_Min >= mu16_forced_close_time_min)
    {
        gu16IceDoorCloseTimer_Min = 0;
        gu16IceDoorCloseTimer_Hour++;
    }
    else{}

    /*..hui [18-1-23오후 2:12:15] 24시간 타이머..*/
    if(gu16IceDoorCloseTimer_Hour >= mu16_forced_close_time_hour)
    {
        F_IceDoorClose = SET;
        gu16IceDoorCloseTimer_Hour = 0;
    }
    else{}

    if( F_IceDoorClose == SET )
    {
        F_IceDoorClose = CLEAR;

        gu16IceDoorCloseTimer_Min = 0;
        gu16IceDoorCloseTimer_Hour = 0;

        gu16IceDoorCloseResetTimer = 70;
        gu16_Ice_Door_StepMotor = STEP_ANGLE_DOOR;
    }
    else{}

    /*..hui [18-1-23오후 2:44:04] 아이스도어 강제 CLOSE중에 얼음 추출할경우 FULL OPEN..*/
    if(gu16IceDoorCloseResetTimer > 0)
    {
        gu16IceDoorCloseResetTimer--;
    }
    else{}
}

/**
 * @brief 추출 이후 2회 강제 도어닫는 사양 추가
 *
 */
// void ice_door_extract_close(void)
// {
    // /* 추출 후 CLOSE가 활성화 안되어 있으면 관련 데이터 초기화 */
    // if(F_close == CLEAR)
    // {
    //     u8_count = 0;
    //     u16_close_min = 0;
    //     u8_close_step = 0;
    //     u8_step_timer_100ms = 0;
    //     return;
    // }
    // else {  }

    // if(F_IceOut == SET)
    // {
    //     F_close = CLEAR;
    //     u8_count = 0;
    //     u16_close_min = 0;
    //     u8_close_step = 0;
    //     u8_step_timer_100ms = 0;
    //     return;
    // }
    // else {  }

    // if(u8_count < 2)
    // {
    //     u16_close_min++;
    //     if(u16_close_min >= ICE_DOOR_CLOSE_INTERVAL)
    //     {
    //         switch (u8_close_step)
    //         {
    //             case 0:             // 준비 (200ms 대기)
    //                 u8_step_timer_100ms++;
    //                 if(u8_step_timer_100ms >= 2)
    //                 {
    //                     u8_step_timer_100ms = 0;
    //                     u8_close_step++;
    //                 }
    //                 else {  }
    //                 break;

    //             case 1:             // full open
    //                 F_IceOpen = SET;
    //                 /* 도어열고 완전히 다 열렸으면 다음으로 */
    //                 if(gu16_Ice_Door_StepMotor >= STEP_ANGLE_DOOR)
    //                 {
    //                     u8_close_step++;
    //                 }
    //                 else {  }
    //                 break;

    //             case 2:             // 1초 대기
    //                 u8_step_timer_100ms++;
    //                 if(u8_step_timer_100ms >= 10)
    //                 {
    //                     u8_step_timer_100ms = 0;
    //                     u8_close_step++;
    //                 }
    //                 else {  }
    //                 break;

    //             case 3:             // full close
    //                 F_IceOpen = CLEAR;
    //                 /* 도어닫고 완전히 다 닫혔으면 다음으로 */
    //                 if(gu16_Ice_Door_StepMotor == 0)
    //                 {
    //                     u8_close_step++;
    //                 }
    //                 else {  }
    //                 break;

    //             case 4:             // 완료 (카운트 증가)
    //                 u8_close_step = 0;
    //                 u16_close_min = 0;
    //                 u8_count++;
    //                 break;

    //             default:
    //                 F_close = CLEAR;
    //                 u8_count = 0;
    //                 u16_close_min = 0;
    //                 u8_close_step = 0;
    //                 u8_step_timer_100ms = 0;
    //                 break;
    //         }
    //     }
    //     else {  }
    // }
    // else
    // {
    //     F_close = CLEAR;
    // }
// }

/***********************************************************************************************************************
* Function Name: ice_door_close_20_min
* Description  : 2025-08-27 CH.PARK [V1.0.0.5] 아이스 추출 완료 후 20분 뒤에 아이스 도어를 강제로 닫는 함수
***********************************************************************************************************************/
void ice_door_close_20_min(void)
{
    U16 mu16_forced_close_time_min = 0;
    U16 mu16_forced_close_time_hour = 0;
    U8 mu8_return = 0;

    /*..hui [18-1-23오후 2:29:03] 일반 모드시 20분..*/
    mu16_forced_close_time_min = 12000;

	if(bit_ice_door_open_after_close == CLEAR)
	{
		return;
	}
	else {}

    if( F_IceOut == SET )
    {
        gu16IceDoor10MinTimer = 0;
    }
    else{}

    /*..hui [18-1-23오후 2:12:10] 10분 타이머..*/
    gu16IceDoor10MinTimer++;

    if(gu16IceDoor10MinTimer >= mu16_forced_close_time_min)
    {
        F_IceDoor10MinClose = SET;
        gu16IceDoor10MinTimer = 0;
    }
    else{}

    if( F_IceDoor10MinClose == SET )
    {
        F_IceDoor10MinClose = CLEAR;

        gu16IceDoor10MinTimer = 0;

        gu16ShortIceDoorCloseResetTimer = 70;
        gu16_Ice_Door_StepMotor = STEP_ANGLE_TEMP_DOOR;

		bit_ice_door_open_after_close = CLEAR;
    }
    else{}

    /*..hui [18-1-23오후 2:44:04] 아이스도어 강제 CLOSE중에 얼음 추출할경우 FULL OPEN..*/
    if(gu16ShortIceDoorCloseResetTimer > 0)
    {
        gu16ShortIceDoorCloseResetTimer--;
    }
    else{}
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 finish_ice_setting(void)
{
    U8 mu8_return = 0;

    if(bit_ice_off_door_close == SET)
    {
        //if(gu8_function_depth != LCD_MENU_DEPTH_TWO)
        //{
        //    bit_ice_off_door_close = CLEAR;
        //   mu8_return = SET;
        //}
        //else{}
    }
    else{}

    return mu8_return;
}
