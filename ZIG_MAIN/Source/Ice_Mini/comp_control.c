/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#include    "Macrodriver.h"
#include    "Global_Variable.h"
#include    "Port_Define.h"
#include    "Comp_Control.h"
#include    "App_Comm_Protocol.h"

void output_comp_control(void);
void comp_stable_check(void);
U8 cold_init_operation( U8 mu8_disable );
void comp_off(void);
void comp_on(void);
void bldc_comp_off(void);
void bldc_comp_on(void);
void set_comp_rps( U8 mu8_target );
void check_comp_off_time(void);
U8 get_cold_mode_comp_rps(void);

void UART2_Receive_Interrupt(U8 mu8RxData);
void UART2_Error_Interrupt(void);




TYPE_WORD          U16CompOnW;
#define            u16CompOn                               U16CompOnW.word
#define            u8CompOn_L                              U16CompOnW.byte[0]
#define            u8CompOn_H                              U16CompOnW.byte[1]
#define            Bit0_Cold_Mode_On_State                 U16CompOnW.Bit.b0
#define            Bit1_Ice_Make_On_State                  U16CompOnW.Bit.b1
#define            Bit2_Ice_Init_On_State                  U16CompOnW.Bit.b2
#define            Bit3_Ice_Tray_Melt_On_State             U16CompOnW.Bit.b3
#define            Bit4_test_State                         U16CompOnW.Bit.b4




TYPE_WORD          U16CompOffW;
#define            u16CompOff                              U16CompOffW.word
#define            u8CompOff_L                             U16CompOffW.byte[0]
#define            u8CompOff_H                             U16CompOffW.byte[1]
#define            Bit0_Restart_5min_Delay_State           U16CompOffW.Bit.b0
#define            Bit1_Max_90min_CutOff_State             U16CompOffW.Bit.b1
#define            Bit2_Safty_Routine_State                U16CompOffW.Bit.b2
#define            Bit3_Ice_Making_Err_State               U16CompOffW.Bit.b3
#define            Bit4_Self_Test_COMP_Off_State           U16CompOffW.Bit.b4




bit F_Comp_Output;
U16 gu16_Comp_fisrt_turn_on_timer;
U32 gu32_Comp_max_turn_on_timer;

U16 gu16_mid_temp_90s_keep_timer;

U16 gu16_air_temp_delay_sec_timer;
U16 gu16_air_temp_delay_min_timer;
U16 gu16_air_comp_restart_time;


U16 gu16_hot_water_comp_delay_tiemr;
bit F_Comp_On_Off_Decision;
U8 gu8_Comp_Stable_Time;
U8 gu8_Comp_Stable_State;

U8  gu8AirTempLevel;
U16 gu16CompOffDelay;

bit F_Cold_Operation_Init;
U8 gu8_cold_init_step;

U8 gu8_cold_setting_level;
U8 gu8_cold_setting_level_old;


U8 gu8_bldc_off_step;
U16 gu16_bldc_off_timer;
U8 gu8_bldc_on_step;
U16 gu16_bldc_on_timer;

U16 gu16_comp_on_timer;
U16 gu16_comp_off_timer;
bit bit_start_preheat;

U16 gu16ColdDelayTimer = 0;         // 냉각 동작 지연 시간 Timer
U16 gu16CoolerActTime = 0;          // 냉각 동작 시간 타이머
U16 gu16CoolUnderTempDelay = 0;     // 5.5도 이하 도달시 5분간 더 냉각 가동하도록 하는 Delay Timer

bit F_UART2Ok;                      // UART통신 OK Flag
bit F_UART2CommunicationErrorDetect;  // UART통신 이상 패킷 수신 Flag
bit F_UART2CommunicationError;
U8 gu8UART2CommunicationErrorCounter = 0;
U16 gu16UART2CommunicationNoiseTimer = 0;   // UART통신 노이즈 수신 타이머
U8 gu8BLDCCommunicationErrorNum = 0;        // BLDC Comp의 통신 에러 발생 카운터
U8 gu8BLDCErrorCounter = 0;         // BLDC Comp의 Error Code 수신 카운터(동작 에러 감지 Counter)
U8 gu8BLDCErrorClearCounter = 0;    // BLDC Comp의 Error Code 미수신 카운터(동작 에러 해제 Counter)
U8 gu8BLDCErrorNum = 0;             // BLDC Comp의 Error 발생 카운터
const U8 gu8BLDC_FrequencyData[36] = {BLDC_40Hz, BLDC_41Hz, BLDC_42Hz, BLDC_43Hz, BLDC_44Hz, BLDC_45Hz,
                                      BLDC_46Hz, BLDC_47Hz, BLDC_48Hz, BLDC_49Hz, BLDC_50Hz,
                                      BLDC_51Hz, BLDC_52Hz, BLDC_53Hz, BLDC_54Hz, BLDC_55Hz,
                                      BLDC_56Hz, BLDC_57Hz, BLDC_58Hz, BLDC_59Hz, BLDC_60Hz,
                                      BLDC_61Hz, BLDC_62Hz, BLDC_63Hz, BLDC_64Hz, BLDC_65Hz,
                                      BLDC_66Hz, BLDC_67Hz, BLDC_68Hz, BLDC_69Hz, BLDC_70Hz,
                                      BLDC_71Hz, BLDC_72Hz, BLDC_73Hz, BLDC_74Hz, BLDC_75Hz};

U8 gu8BLDC_ErrorCode = 0;
U8 gu8BLDC_TargetHz = 0;
U8 gu8BLDC_FeedbackHz = 0;
U8 gu8BLDCRxDataBuffer[3] = {0, 0, 0};
U8 gu8UART2_RXD_Sequence = 0;
U8 gu8RXD2_CheckTimer = 0;
U8 gu8BLDC_TXDPeroid = 0;
U8 gu8ColdPumpControlVolt = 0;
U16 gu16ColdPumpControlTimer = 0;     // 냉각 순환 펌프 제어 타이머
U16 gu16ColdEffluentEndPumpActTimer = 0;  // 냉수 추출 후 펌프 제어 타이머

bit F_UART2ErrorInterrupt;
bit F_CompActStart;               // 냉각 동작 시작 Flag
bit F_BLDCCompError;                  // BLDC Comp 동작 에러

bit bit_comp_comm_temp_error;
U8 gu8_comp_1s_timer;
U8 gu8_cold_init_timer;
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void output_comp_control(void)
{
/***********************************************************************************************************/
/***********************************************************************************************************/
    /* ON */
    if(Bit0_Cold_Make_Go == SET)
    {
        Bit0_Cold_Mode_On_State = cold_init_operation( Bit0_Restart_5min_Delay_State );
    }
    else
    {
        Bit0_Cold_Mode_On_State = CLEAR;
        gu8_cold_init_step = 0;
        gu8_cold_init_timer = 0;
    }


    if( Bit1_Ice_Make_Go == SET )
    {
        /*..hui [23-7-21오후 2:13:29] 저수위 상태 후 제빙 대기시 만수위까지 채운 후 진행..*/
        /*..hui [23-7-21오후 2:13:49] 연속 제빙중 COMP 끄고 대기하도록..*/
        if( gu8IceStep == STATE_0_STANDBY )
        {
        	/*
            if( F_WaterInit != SET )
            {
                Bit1_Ice_Make_On_State = CLEAR;
            }
            else{}
			*/
        }
        else{}

        if( gu8IceStep == STATE_7_PREHEAT_OPERATION )
        {
            Bit1_Ice_Make_On_State = SET;
        }
        else{}

        if( gu8IceStep >= STATE_31_MAIN_ICE_MAKING && gu8IceStep <= STATE_51_FINISH_ICE_MAKE )
        {
            Bit1_Ice_Make_On_State = SET;
        }
        else{}
    }
    else
    {
        Bit1_Ice_Make_On_State = CLEAR;
    }


    /*..hui [23-4-13오전 11:08:46] 더미제빙, 핫가스까지..*/
    if( gu8InitStep >= 5 && gu8InitStep <= 7 )
    {
        Bit2_Ice_Init_On_State = F_IceInit;

    }
    else
    {
        Bit2_Ice_Init_On_State = CLEAR;
    }



    /*..hui [23-9-22오전 9:44:29] 해빙 동작 중 핫가스 동작 삭제..*/
    Bit3_Ice_Tray_Melt_On_State = CLEAR;

/***********************************************************************************************************/
/***********************************************************************************************************/
    /* OFF */
    //===========================================================================//
    /*..hui [18-10-23오후 3:24:08] COMP 150분 연속 가동시 강제 정지..*/
    if( F_Comp_Output == CLEAR )
    {
        gu32_Comp_max_turn_on_timer = 0;
        Bit1_Max_90min_CutOff_State = CLEAR;
    }
    else
    {
        gu32_Comp_max_turn_on_timer++;
        if( gu32_Comp_max_turn_on_timer >= COMP_MAX_TURN_ON_150MIN_TIME )
        {
            gu32_Comp_max_turn_on_timer = COMP_MAX_TURN_ON_150MIN_TIME;

            /*..hui [19-7-26오후 8:53:31] 압축기 보호시간 초과시 진행중인 제빙까지 마무리하고 COMP OFF..*/
            if( gu8IceStep == STATE_0_STANDBY )
            {
                Bit1_Max_90min_CutOff_State = SET;
            }
            else{}
        }
        else
        {
            Bit1_Max_90min_CutOff_State = CLEAR;
        }
    }

    //===========================================================================//
    /*..hui [18-10-23오후 3:28:54] 컴프 가동 후 정지 시 휴지시간 5분 이후에 재 냉각 조건 만족시 재 냉각 시작..*/
    if(F_Comp_Output == SET)
    {
        gu16CompOffDelay = COMP_START_TIME;                       // 컴프 재기동대기 5분
        Bit0_Restart_5min_Delay_State = CLEAR;
    }
    else
    {
        if(gu16CompOffDelay > 0)
        {
            gu16CompOffDelay--;                                   // 콤프 5분 재기동대기
            Bit0_Restart_5min_Delay_State = SET;
        }
        else
        {
            Bit0_Restart_5min_Delay_State = CLEAR;
        }
    }


    //===========================================================================//
    /*..hui [19-12-18오후 7:26:16] 냉각/얼음 둘 다 고장일떄..*/
    Bit3_Ice_Making_Err_State = (Bit2_Ice_Operation_Disable_State & Bit0_Cold_Operation_Disable_State);


    /*..hui [23-12-20오후 5:12:03] 고장진단 진행중일때..*/
    Bit4_Self_Test_COMP_Off_State = bit_self_test_start;

    //===========================================================================//

/***********************************************************************************************************/
    if( u16CompOff > 0 )            /*off*/
    {
        comp_off();
        F_Comp_Output = CLEAR;
    }
    else
    {
        if( u16CompOn > 0 )          /*on*/
        {
            comp_on();
            F_Comp_Output = SET;
        }
        else                       /*off*/
        {
            comp_off();
            F_Comp_Output = CLEAR;
        }
    }

    /*if( F_Comp_Output == SET )*/
    if( pCOMP == SET )
    {
        Bldc_Comp_Communication();
    }
    else
    {
        clear_bldc_buffer();
    }

    comp_stable_check();
    check_comp_off_time();
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void comp_stable_check(void)
{
    /*if(F_Comp_Output == CLEAR)*/
    /*..hui [24-2-5오후 5:33:14] 실제 출력으로 변경..*/
    if( pCOMP == CLEAR )
    {
        gu8_Comp_Stable_Time = 0;
        gu8_Comp_Stable_State = SET;
    }
    else
    {
        gu8_Comp_Stable_Time++;

        if(gu8_Comp_Stable_Time >= COMP_STABLE_TIME)
        {
            gu8_Comp_Stable_Time = COMP_STABLE_TIME;
            gu8_Comp_Stable_State = SET;
        }
        else
        {
            gu8_Comp_Stable_State = CLEAR;
        }
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  : 냉각 초기동작
***********************************************************************************************************************/
U8 cold_init_operation( U8 mu8_disable )
{
    U8 mu8_return = 0;
    U8 mu8_comp_rps = 0;

    if(mu8_disable == SET)
    {
        gu8_cold_init_step = 0;
        gu8_cold_init_timer = 0;
        return mu8_return;
    }
    else{}

    switch(gu8_cold_init_step)
    {
        case 0:

            gu8_cold_init_timer++;

            if( gu8_cold_init_timer >= 10 )
            {
                gu8_cold_init_timer = 0;
                gu8_cold_init_step++;
            }
            else{}

        break;

        case 1:

            GasSwitchCold();
            gu8_cold_init_step++;

        break;

        case 2:

            if(gu8_GasSwitch_Status == GAS_SWITCH_COLD)
            {
                gu8_cold_init_step++;
            }
            else{}

        break;

        case 3:

            mu8_comp_rps = get_cold_mode_comp_rps();
            set_comp_rps( mu8_comp_rps );

            mu8_return = SET;

        break;

        default:

            gu8_cold_init_step = 0;

        break;
    }


    return mu8_return;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void comp_off(void)
{
    /*if( F_Comp_Output == SET )*/
    if( pCOMP == SET )
    {
        bldc_comp_off();
    }
    else
    {
        pCOMP = CLEAR;
        //////F_Comp_Output = CLEAR;

        gu8_bldc_on_step = 0;
        gu16_bldc_on_timer = 0;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void comp_on(void)
{
    /*if( F_Comp_Output == CLEAR )*/
    if( pCOMP == CLEAR )
    {
        bldc_comp_on();
    }
    else
    {
        pCOMP = SET;
        /////F_Comp_Output = SET;

        gu8_bldc_off_step = 0;
        gu16_bldc_off_timer = 0;
    }
}



/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void bldc_comp_off(void)
{
    gu8_bldc_target_hz = BLDC_COMP_OFF_0_HZ;

    switch( gu8_bldc_off_step )
    {
        case 0:

            if( gu8_bldc_opration_hz == BLDC_COMP_OFF_0_HZ )
            {
                gu8_bldc_off_step++;
                gu16_bldc_off_timer = 0;
            }
            else if( bit_temporary_comm_error == SET )
            {
                /*..hui [23-7-4오전 10:57:55] 통신에러일때.. 통신이 안되므로 바로 OFF..*/
                /*..hui [23-7-4오전 11:13:31] 에러코드로 OFF될때는 제외.. 통신은 되므로 기다린다..*/
                gu8_bldc_off_step++;
                gu16_bldc_off_timer = 0;
            }
            else
            {
                gu16_bldc_off_timer++;

                if( gu16_bldc_off_timer >= 300 )
                {
                    /*gu16_bldc_off_timer = 300;*/
                    /*..hui [23-5-8오후 2:24:33] 에러 추가 ..*/
                    gu8_bldc_off_step++;
                    gu16_bldc_off_timer = 0;
                }
                else{}
            }

        break;

        case 1:

            gu16_bldc_off_timer++;

            if( gu16_bldc_off_timer >= 30 )
            {
                gu16_bldc_off_timer = 0;
                gu8_bldc_off_step++;
            }
            else{}

        break;

        case 2:

            pCOMP = CLEAR;
            F_Comp_Output = CLEAR;

        break;

        default:
            gu16_bldc_off_timer = 0;
            gu8_bldc_off_step = 0;
        break;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void bldc_comp_on(void)
{
    /*gu8_bldc_target_hz = BLDC_COMP_40Hz;*/

    switch( gu8_bldc_on_step )
    {
        case 0:

            gu16_bldc_on_timer++;

            if( gu16_bldc_on_timer >= 10 )
            {
                gu16_bldc_on_timer = 0;
                gu8_bldc_on_step++;
            }
            else{}

        break;

        case 1:

            pCOMP = SET;

        break;

        default:
            gu16_bldc_on_timer = 0;
            gu8_bldc_on_step = 0;
        break;
    }


}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void set_comp_rps( U8 mu8_target )
{
    gu8_bldc_target_hz = mu8_target;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void check_comp_off_time(void)
{
    if( F_Comp_Output == SET )
    {
        gu16_comp_off_timer = 0;
        gu16_comp_on_timer++;

        /*..hui [23-4-12오후 2:21:16] 3분 이상 가동했을 경우에만 클리어..*/
        if( gu16_comp_on_timer >= PREHEAT_CLEAR_COMP_ON_TIME )
        {
            gu16_comp_on_timer = PREHEAT_CLEAR_COMP_ON_TIME;
            bit_start_preheat = CLEAR;
        }
        else{}
    }
    else
    {
        /*..hui [23-4-7오후 4:48:23] 외기 온도 25도 미만인 상태에서 압축기 휴지 시간이 30분 초과한 경우..*/
        /*..hui [23-4-7오후 4:48:30] 제빙 시작 전에 예열 동작 진행..*/
        gu16_comp_on_timer = 0;
        gu16_comp_off_timer++;

        if( gu16_comp_off_timer >= PREHEAT_SET_COMP_OFF_TIME )
        {
            gu16_comp_off_timer = PREHEAT_SET_COMP_OFF_TIME;
            bit_start_preheat = SET;
        }
        else{}
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  : 냉각 초기기동 (250429) (냉각테이블)
***********************************************************************************************************************/
U8 get_cold_mode_comp_rps(void)
{
    U8 mu8_return = 0;

    if( gu8_Amb_Temperature_One_Degree <= 10 )
    {
        /*..hui [23-4-7오전 11:15:58] 10도 이하..*/
        if( bit_cold_first_op == SET )                  /* 초기 */
        {
            mu8_return = BLDC_COMP_50Hz;                /* V18 냉각테이블 */
        }
        else
        {
            mu8_return = BLDC_COMP_40Hz;                /* V18 냉각테이블 */
        }
    }
    else if( gu8_Amb_Temperature_One_Degree <= 20 )
    {
        /*..hui [23-4-7오전 11:16:02] 20도 이하..*/
        if( bit_cold_first_op == SET )
        {
            mu8_return = BLDC_COMP_50Hz;                /* V18 냉각테이블 */
        }
        else
        {
            /* V11 냉각테이블 */
            if( gu16_Cold_Temperature >= 65 )
            {
                mu8_return = BLDC_COMP_40Hz;
            }
            else
            {
                mu8_return = BLDC_COMP_45Hz;
            }
        }
    }
    else if( gu8_Amb_Temperature_One_Degree <= 30 )
    {
        /*..hui [23-4-7오전 11:16:10] 30도 이하..*/
        mu8_return = BLDC_COMP_45Hz;                    /* V18 냉각테이블 */
    }
    else
    {
        /*..hui [23-4-7오전 11:16:14] 30도 초과..*/
        mu8_return = BLDC_COMP_45Hz;                    /* V18 냉각테이블 */
    }

    /* CH.PARK 냉각 검토 고정값 데이터 반영 */
    if(GetB1ColdTargetRPS() > 0)
    {
        mu8_return = GetB1ColdTargetRPS();
    }

    return mu8_return;
}



/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/




