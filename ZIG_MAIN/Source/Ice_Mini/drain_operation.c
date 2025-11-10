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
#include    "drain_operation.h"







void output_drain_pump(void);
void detect_no_water(void);
void drain_pump_output_decision(void);
void detect_drain_tank_high_level( void );

void keep_3_hour_check(void);
void ice_full_condition_check(void);
void detect_drain_pump_error(void);
U8 drain_pump_error_retry_proc( void );
void forced_drain_check(void);
void ice_off_check(void);
void ice_off_24hour_check(void);
void flushing_drain_check(void);

void first_drain_check(void);
void ice_make_drain_check(void);
void tray_ster_drain_check(void);
//////void acid_drain_check(void);

void detect_drain_tank_error_level( void );

void start_drain_pump( U16 u16_data );
void stop_drain_pump( void );
void drain_melt_water(void);






TYPE_WORD          U16DrainOperateB;
#define            u16DrainOperate                             U16DrainOperateB.word
#define            u8DrainOperate_L                            U16DrainOperateB.byte[0]
#define            Bit0_Drain_Level_Full                       U16DrainOperateB.Bit.b0      /* 드레인탱크 만수위 시 동작 */
#define            Bit1_Drain_Low_3_Hour                       U16DrainOperateB.Bit.b1      /* 중수위 3시간 이상 유지 시 동작 */
#define            Bit2_Drain_Ice_Full                         U16DrainOperateB.Bit.b2      /* 얼음가득 상태 시 동작 */
#define            Bit3_Drain_Forced                           U16DrainOperateB.Bit.b3      /* 중,만수위에서 탱크커버 열었을 시  */
#define            Bit4_Drain_Flushing                         U16DrainOperateB.Bit.b4      /* 냉수탱크 플러싱 시 */
#define            Bit5_Drain_Ice_Off                          U16DrainOperateB.Bit.b5      /* 얼음OFF 시 (중수위 이상일 때 )*/
#define            Bit6_Drain_First                            U16DrainOperateB.Bit.b6      /* 첫 전원 인가 시 (중수위일 때) */
#define            Bit7_Drain_Ice_Making                       U16DrainOperateB.Bit.b7      /* 제빙-핫가스 동작 시 (중수위일 때) */
#define            Bit8_Drain_Tray_ster                        U16DrainOperateB.Bit.b8      /* 트레이 고온살균 진행 중일 때 */
#define            Bit9_Drain_Ice_Off_24_Hour                  U16DrainOperateB.Bit.b9      /* 얼음OFF한 이후 24시간 이후 (중수위 이상일 때) */
// #define            Bit10_Drain_Level_Error                     U16DrainOperateB.Bit.b10

TYPE_BYTE          U8DrainPumpONB;
#define            u8DrainPumpON                        U8DrainPumpONB.byte
#define            Bit0_Drain_Tank_Level_Full_State     U8DrainPumpONB.Bit.b0
#define            Bit1_Drain_Error_Check_On_State      U8DrainPumpONB.Bit.b1
#define            Bit2_Drain_Melt_Ice_State            U8DrainPumpONB.Bit.b2
#define            Bit3_Drain_Tank_Flushing_State       U8DrainPumpONB.Bit.b3
#define            Bit4_Auto_drain_State                U8DrainPumpONB.Bit.b4
#define            Bit5_Manual_drain_State              U8DrainPumpONB.Bit.b5
#define            Bit6_Acid_Clean_State                U8DrainPumpONB.Bit.b6



TYPE_BYTE          U8DrainPumpOFFB;
#define            u8DrainPumpOFF                       U8DrainPumpOFFB.byte
#define            Bit0_Drain_Error_Check_Off_State     U8DrainPumpOFFB.Bit.b0
#define            Bit1_Drain_Error_Confirm_State       U8DrainPumpOFFB.Bit.b1
#define            Bit2_Bldc_Over_Current_Off_State     U8DrainPumpOFFB.Bit.b2
#define            Bit3_Hot_Filling_Drain_Off_State     U8DrainPumpOFFB.Bit.b3
#define            Bit4_Hot_Cooling_Drain_Off_State     U8DrainPumpOFFB.Bit.b4


U8 gu8drain_1s_time;


U16 gu16_drain_pump_first_op_timer;
U8 gu8_drain_pump_retry_step;

U16 gu16_drain_pump_1min_off_timer;
U16 gu16_drain_pump_2min_on_timer;

U16 gu16_drain_low_water_1min_tmr;
U16 gu16_drain_low_water_3hour_tmr;





bit F_Drain_Pump_Output;


U8 gu8_drain_err_10_count;

bit F_old_ice_full;
bit F_old_ice_on;

U16 gu16_ice_off_drain_timer;

//bit Bit2_Drain_Ice_Full;

bit bit_drain_err_check_on_state;
bit bit_drain_err_check_off_state;


bit bit_drain_error_check_enable;

U8 gu8_drain_pump_max_operation_count;



bit F_forced_drain_enable;

U8 gu8_forced_drain_add_timer;


bit F_Drain_Pump_Output;

U16 gu16_drain_pwm_out;


U16 gu16_drain_test_timer;
bit bit_first_drain;


U16 gu16_over_current_off_timer;
U16 gu16_over_current_detect_timer;

U16 gu16_drain_max_timer;
bit bit_drain_no_water_detect;
U8 gu8_drain_no_load_min_timer;

/*..hui [24-4-5오후 3:57:34] 만수위 감지, 저수위 미감지 재시도 횟수..*/
U8 gu8_drain_error_retry_cnt;

U32 gu32_ice_off_24h_teimer;
bit bit_ice_off_24h_start;


U16 gu16_drain_level_error_timer;
U8 gu8_drain_no_water_detect_timer;
U8 gu8_drain_no_water_clear_timer;

bit bit_drain_level_error_detect;

U32 gu32_drain_melt_standby_timer;
bit bit_ice_melt_2_hour;
bit bit_drain_melt_pump_output;
U16 gu16_drain_melt_op_timer;
U8 gu8_drain_melt_cnt;
bit bit_drain_melt_finish;

U8 gu8_drain_error_clear_timer;

extern COLDTANK_FLUSHING_STEP gu8_cold_tank_flushing_step;
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void output_drain_pump(void)
{



    /*..hui [17-12-18오후 5:49:56] 드레인펌프 에러 확인..*/
    detect_drain_pump_error();

    /*..hui [24-3-28오후 1:05:16] 드레인 펌프 에러 발생 후 탱크 내 얼음 녹은 물 배수.. 약 30시간 동안..*/
    drain_melt_water();

    /*..hui [17-12-18오후 5:50:02] 최종 출력 결정..*/
    drain_pump_output_decision();


    if( u8DrainPumpOFF > 0 )
    {
        stop_drain_pump();  /*off*/
        gu8_drain_pump_max_operation_count = 0;
        F_Drain_Pump_Output = CLEAR;
        Bit6_Drain_Pump_FeedBack_12V_Out_State = CLEAR;
    }
    else
    {
        if( u8DrainPumpON > 0 )
        {
            /*..hui [18-1-30오후 3:11:35] 가동 초반 3초동안 max로 가동하다가 약 66%로 변경.. ..*/
            /*..hui [18-1-30오후 3:11:53] 다른모델이 75%라고 돼있으나 실제로는 66%임..*/
            gu8_drain_pump_max_operation_count++;
            if(gu8_drain_pump_max_operation_count >= DRAIN_PUMP_MAX_OP_TIME)
            {
                gu8_drain_pump_max_operation_count = DRAIN_PUMP_MAX_OP_TIME;

                /*..hui [18-7-19오후 1:31:20] 재시도 10회 중 2회째부터는 Full duty로 가동한다...*/
                /*..hui [18-7-19오후 1:31:46] 배수 안되서 에러 발생하는 것 보다 소음나는게 더 낫지....*/
                if(gu8_drain_err_10_count >= DRAIN_PUMP_MAX_OUTPUT_ERROR_COUNT)
                {
                    start_drain_pump( PUMP_PWM_MAX ); /*on*/
                }
                else
                {
                    start_drain_pump( PUMP_PWM ); /*on*/
                }
            }
            else
            {
                start_drain_pump( PUMP_PWM_MAX ); /*on*/
            }

            F_Drain_Pump_Output = SET;
            /*..hui [24-4-4오후 5:30:42] 상진이가 회로 변경하면서 드레인펌프 켤때 12V 켜줘야함..*/
            /*..hui [24-4-4오후 5:30:58] 펌프 피드백 전류 증폭하면서 5V -> 12V로 바꿈..*/
            Bit6_Drain_Pump_FeedBack_12V_Out_State = SET;
        }
        else
        {
            stop_drain_pump(); /*off*/
            gu8_drain_pump_max_operation_count = 0;
            F_Drain_Pump_Output = CLEAR;
            Bit6_Drain_Pump_FeedBack_12V_Out_State = CLEAR;
        }
    }

    detect_no_water();
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void detect_no_water(void)
{
    /*..hui [24-4-1오후 5:17:02] 드레인 펌프 가동중일때만 확인..*/
    if( F_Drain_Pump_Output == SET )
    {
        gu8_drain_no_load_min_timer++;

        if( gu8_drain_no_load_min_timer >= 100 )
        {
            /*..hui [24-4-1오후 7:09:11] 가동하고 10초 후부터 체크.. AD 안정화 시간..*/
            gu8_drain_no_load_min_timer = 100;

            /*..hui [24-3-28오후 7:21:27] 최시 시간 가동 이후 펌프 AD가 물 없음 값 이하일 경우 가동 정지..*/
            if( gu16_AD_Drain_Pump_Current <= DRAIN_NO_WATER_FEEDBACK_AD )
            {
                gu8_drain_no_water_clear_timer = 0;
                gu8_drain_no_water_detect_timer++;

                /*if( gu8_drain_no_water_detect_timer >= 50 )*/
                if( gu8_drain_no_water_detect_timer >= 100 )
                {
                    gu8_drain_no_water_detect_timer = 100;
                    bit_drain_no_water_detect = SET;
                }
                else{}
            }
            else
            {
                gu8_drain_no_water_detect_timer = 0;
                gu8_drain_no_water_clear_timer++;

                if( gu8_drain_no_water_clear_timer >= 100 )
                {
                    gu8_drain_no_water_clear_timer = 100;
                    bit_drain_no_water_detect = CLEAR;
                }
                else{}
            }
        }
        else
        {
            gu8_drain_no_water_detect_timer = 0;
            gu8_drain_no_water_clear_timer = 0;
        }
    }
    else
    {
        gu8_drain_no_water_detect_timer = 0;
        gu8_drain_no_water_clear_timer = 0;
        gu8_drain_no_load_min_timer = 0;
    }
}
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void drain_pump_output_decision(void)
{
/***************************************************************************************************/

    /*..hui [18-1-14오후 8:57:59] 순환배수 기능 실행중일때는 에러체크 안함..*/
    /*Bit1_Drain_Error_Check_On_State = bit_drain_err_check_on_state & ~F_DrainStatus & ~bit_manual_drain_start;*/
    /*..hui [24-3-28오전 10:51:41] 순환배수, 메뉴얼 드레인은 에러체크하는 곳에서 한번 거르기때문에 ..*/
    Bit1_Drain_Error_Check_On_State = bit_drain_err_check_on_state;

    /*..hui [24-3-28오전 10:51:56] 순환배수 동작 시 드레인펌프 동작..*/
    Bit4_Auto_drain_State = bit_auto_drain_pump_out;

    /*..hui [23-8-14오후 1:16:49] 수동배수 기능 추가..*/
    if( bit_manual_drain_start == SET )
    {
        Bit5_Manual_drain_State = bit_manual_drain_pump_output;
    }
    else
    {
        Bit5_Manual_drain_State = CLEAR;
    }

    /*..hui [24-3-28오전 10:58:58] 드레인 에러 발생하고나서 얼음 녹은물 강제 배수..*/
    Bit2_Drain_Melt_Ice_State = bit_drain_melt_pump_output;


    /*..hui [24-5-24오후 5:57:19] 탱크 플러싱중에는 따로 제어..*/
		/*
    if( gu8_flushing_mode == FLUSHING_TANK_STATE && bit_flushing_tank_start == SET && gu8_flushing_tank_step == 10 )
    {
        Bit3_Drain_Tank_Flushing_State = bit_tank_drain_pump_output;
    }
    else
    {
        Bit3_Drain_Tank_Flushing_State = CLEAR;
    }
*/

    if(gu8_flushing_mode == FLUSHING_FILL_COLD_TANK_START_STATE)
    {
        if(gu8_cold_tank_fill_step == COLD_TANK_FILL_DRAIN)
        {
            Bit3_Drain_Tank_Flushing_State = SET;
        }
        else
        {
            Bit3_Drain_Tank_Flushing_State = CLEAR;
        }
    }
    else if(gu8_flushing_mode == FLUSHING_COLD_TANK)
    {
        if(gu8_cold_tank_flushing_step == COLDTANK_FLUSHING_DRAIN)
        {
            Bit3_Drain_Tank_Flushing_State = SET;
        }
        else
        {
            Bit3_Drain_Tank_Flushing_State = CLEAR;
        }
    }
    else
    {
        Bit3_Drain_Tank_Flushing_State = CLEAR;
    }

    Bit6_Acid_Clean_State = CLEAR;

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

    /*..hui [24-3-28오전 10:56:19] 이것도 필요없음.. 어차피 에러체크하는 부분에서 순환배수, 메뉴얼 드레인이면 멈추게 돼있음..*/
    /*Bit0_Drain_Error_Check_Off_State = bit_drain_err_check_off_state & ~F_DrainStatus & ~bit_manual_drain_start;*/

    /*Bit1_Drain_Error_Confirm_State = Bit16_Drain_Pump_Error__E60;*/

    if( Bit16_Drain_Pump_Error__E60 == SET )
    {
        /*..hui [24-3-28오전 11:09:02] 에러 걸린 상태에서도 녹은물 강제 배수할때는 동작하도록..*/
        if( bit_drain_melt_pump_output == SET )
        {
            Bit1_Drain_Error_Confirm_State = CLEAR;
        }
        else
        {
            Bit1_Drain_Error_Confirm_State = Bit16_Drain_Pump_Error__E60;
        }
    }
    else
    {
        Bit1_Drain_Error_Confirm_State = CLEAR;
    }


/***************************************************************************************************/

}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void ice_full_condition_check(void)
{
    /*..hui [18-1-8오후 3:50:20] 제빙 설정상태 추가..*/
    if( F_IceFull == SET )
    {
        if( F_old_ice_full != F_IceFull )
        {
            F_old_ice_full = F_IceFull;

            /*if(u8DrainWaterLevel >= DRAIN_LEVEL_LOW)*/
            /*..hui [24-3-28오후 7:11:46] 에러일때 제외.. 따로 처리해야함..*/
            if( u8DrainWaterLevel == DRAIN_LEVEL_LOW || u8DrainWaterLevel == DRAIN_LEVEL_HIGH )
            {
                Bit2_Drain_Ice_Full = SET;
            }
            else
            {
                Bit2_Drain_Ice_Full = CLEAR;
            }
        }
        else
        {
            if(Bit2_Drain_Ice_Full == SET)
            {
                /*if( u8DrainWaterLevel == DRAIN_LEVEL_EMPTY )*/
                /*..hui [24-4-1오후 5:03:29] 수위 에러일경우에도 정지.. 수위 에러 함수에서 따로 처리..*/
                if( u8DrainWaterLevel == DRAIN_LEVEL_EMPTY || u8DrainWaterLevel == DRAIN_LEVEL_ERROR )
                {
                    Bit2_Drain_Ice_Full = CLEAR;
                }
                else{}
            }
            else
            {

            }
        }
    }
    else
    {
        F_old_ice_full = CLEAR;

        /*..hui [23-7-7오후 1:29:17] 최소 가동시간 가동 이후 무부하 전류 감지 시 정지..*/
        /*if( u8DrainWaterLevel == DRAIN_LEVEL_EMPTY || bit_drain_no_water_detect == SET )*/
        /*if( u8DrainWaterLevel == DRAIN_LEVEL_EMPTY )*/
        /*..hui [24-4-1오후 5:03:29] 수위 에러일경우에도 정지.. 수위 에러 함수에서 따로 처리..*/
        if( u8DrainWaterLevel == DRAIN_LEVEL_EMPTY || u8DrainWaterLevel == DRAIN_LEVEL_ERROR )
        {
            Bit2_Drain_Ice_Full = CLEAR;
        }
        else{}
    }
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void keep_3_hour_check(void)
{
    /*..hui [17-12-18오후 1:05:30] 저수위 감지 3시간 이상 유지시 저수위 미감지까지 배수 ..*/
    if( u8DrainWaterLevel == DRAIN_LEVEL_LOW )
    {
        /*..hui [17-12-18오후 1:16:57] 드레인펌프 가동중에는 카운트 정지..*/
        if(Bit1_Drain_Low_3_Hour == SET)
        {

        }
        else
        {
            gu16_drain_low_water_1min_tmr++;
        }

        if(gu16_drain_low_water_1min_tmr >= 600)
        {
            gu16_drain_low_water_1min_tmr = 0;

            gu16_drain_low_water_3hour_tmr++;

            /*if(gu16_drain_low_water_3hour_tmr >= 180)*/
            if(gu16_drain_low_water_3hour_tmr >= 120)
            {
                Bit1_Drain_Low_3_Hour = SET;
                gu16_drain_low_water_3hour_tmr = 0;
            }
            else{}
        }
        else{}
    }
    else
    {
        if(Bit1_Drain_Low_3_Hour == SET)
        {
            /*..hui [23-7-7오후 1:29:17] 최소 가동시간 가동 이후 무부하 전류 감지 시 정지..*/
            /*if( u8DrainWaterLevel == DRAIN_LEVEL_EMPTY || bit_drain_no_water_detect == SET )*/
            /*if( u8DrainWaterLevel == DRAIN_LEVEL_EMPTY )*/
            /*..hui [24-4-1오후 5:03:29] 수위 에러일경우에도 정지.. 수위 에러 함수에서 따로 처리..*/
            if( u8DrainWaterLevel == DRAIN_LEVEL_EMPTY || u8DrainWaterLevel == DRAIN_LEVEL_ERROR )
            {
                Bit1_Drain_Low_3_Hour = CLEAR;
            }
            else{}
        }
        else
        {
            gu16_drain_low_water_1min_tmr = 0;
            gu16_drain_low_water_3hour_tmr = 0;
        }
    }
}




/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void detect_drain_tank_high_level( void )
{
    U8 mu8_return = 0;

    /*..hui [17-12-15오후 10:40:27] 만수위 감지시 저수위 미감지될때까지 드레인펌프 가동..*/
    /*..hui [17-12-18오전 10:27:56] 수위센서 에러시(저수위 미감지,고수위 감지)에도 배수 진행..*/
    /*if(u8DrainWaterLevel == DRAIN_LEVEL_HIGH || u8DrainWaterLevel == DRAIN_LEVEL_ERROR)*/

    /*..hui [23-7-7오후 2:26:32] 에러 조건은 분리..*/
    /*if( u8DrainWaterLevel == DRAIN_LEVEL_HIGH )*/
    /*if(u8DrainWaterLevel == DRAIN_LEVEL_HIGH || u8DrainWaterLevel == DRAIN_LEVEL_ERROR)*/
    /*..hui [24-3-28오후 7:07:52] 에러 조건 따로 분리..*/
    if( u8DrainWaterLevel == DRAIN_LEVEL_HIGH || u8DrainWaterLevel == DRAIN_LEVEL_ERROR)
    {
        Bit0_Drain_Level_Full = SET;
    }
    else
    {
        if( Bit0_Drain_Level_Full == SET )
        {
            /*..hui [23-7-7오후 1:29:17] 최소 가동시간 가동 이후 무부하 전류 감지 시 정지..*/
            /*if( u8DrainWaterLevel == DRAIN_LEVEL_EMPTY || bit_drain_no_water_detect == SET )*/
            /*if( u8DrainWaterLevel == DRAIN_LEVEL_EMPTY )*/
            /*..hui [24-4-1오후 5:03:29] 수위 에러일경우에도 정지.. 수위 에러 함수에서 따로 처리..*/
            if( u8DrainWaterLevel == DRAIN_LEVEL_EMPTY || u8DrainWaterLevel == DRAIN_LEVEL_ERROR )
            {
                Bit0_Drain_Level_Full = CLEAR;
            }
            else{}
        }
        else{}
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void ice_off_check(void)
{
    /*..hui [19-12-2오후 2:07:02] 얼음 기능 ON->OFF될 경우 저수위 이상이면 드레인 펌프 가동..*/
    /*if(F_IceOn == CLEAR)*/

    /*if( gu8_ice_setting == ICE_SETTING_OFF )*/
    if(F_IceOn == CLEAR)
    {
        if( F_old_ice_on == SET )
        {
            F_old_ice_on = CLEAR;

            /*if(u8DrainWaterLevel >= DRAIN_LEVEL_LOW)*/
            /*..hui [24-3-28오후 7:11:46] 에러일때 제외.. 따로 처리해야함..*/
            if( u8DrainWaterLevel == DRAIN_LEVEL_LOW || u8DrainWaterLevel == DRAIN_LEVEL_HIGH )
            {
                Bit5_Drain_Ice_Off = SET;
            }
            else
            {
                Bit5_Drain_Ice_Off = CLEAR;
            }

            bit_ice_off_24h_start = SET;
        }
        else
        {
            if(Bit5_Drain_Ice_Off == SET)
            {
                /*..hui [23-7-7오후 1:29:17] 최소 가동시간 가동 이후 무부하 전류 감지 시 정지..*/
                /*if( u8DrainWaterLevel == DRAIN_LEVEL_EMPTY || bit_drain_no_water_detect == SET )*/
                /*if( u8DrainWaterLevel == DRAIN_LEVEL_EMPTY )*/
                /*..hui [24-4-1오후 5:03:29] 수위 에러일경우에도 정지.. 수위 에러 함수에서 따로 처리..*/
                if( u8DrainWaterLevel == DRAIN_LEVEL_EMPTY || u8DrainWaterLevel == DRAIN_LEVEL_ERROR )
                {
                    Bit5_Drain_Ice_Off = CLEAR;
                }
                else{}
            }
            else{}
        }
    }
    else
    {
        F_old_ice_on = SET;
        bit_ice_off_24h_start = CLEAR;

        /*..hui [23-7-7오후 1:29:17] 최소 가동시간 가동 이후 무부하 전류 감지 시 정지..*/
        /*if( u8DrainWaterLevel == DRAIN_LEVEL_EMPTY || bit_drain_no_water_detect == SET )*/
        /*if( u8DrainWaterLevel == DRAIN_LEVEL_EMPTY )*/
        /*..hui [24-4-1오후 5:03:29] 수위 에러일경우에도 정지.. 수위 에러 함수에서 따로 처리..*/
        if( u8DrainWaterLevel == DRAIN_LEVEL_EMPTY || u8DrainWaterLevel == DRAIN_LEVEL_ERROR )
        {
            Bit5_Drain_Ice_Off = CLEAR;
        }
        else{}
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void ice_off_24hour_check(void)
{
    if( bit_ice_off_24h_start == SET )
    {
        gu32_ice_off_24h_teimer++;

        /*..hui [23-7-7오후 2:46:22] OFF하고 24시간 후에 저수위 이상이면 드레인..*/
        if( gu32_ice_off_24h_teimer >= 864000 )
        {
            gu32_ice_off_24h_teimer = 0;
            bit_ice_off_24h_start = CLEAR;

            /*if(u8DrainWaterLevel >= DRAIN_LEVEL_LOW)*/
            /*..hui [24-3-28오후 7:11:46] 에러일때 제외.. 따로 처리해야함..*/
            if( u8DrainWaterLevel == DRAIN_LEVEL_LOW || u8DrainWaterLevel == DRAIN_LEVEL_HIGH )
            {
                Bit9_Drain_Ice_Off_24_Hour = SET;
            }
            else
            {
                Bit9_Drain_Ice_Off_24_Hour = CLEAR;
            }
        }
        else{}
    }
    else
    {
        gu32_ice_off_24h_teimer = 0;

        if( Bit9_Drain_Ice_Off_24_Hour == SET )
        {
            /*..hui [23-7-7오후 1:29:17] 최소 가동시간 가동 이후 무부하 전류 감지 시 정지..*/
            /*if( u8DrainWaterLevel == DRAIN_LEVEL_EMPTY || bit_drain_no_water_detect == SET )*/
            /*if( u8DrainWaterLevel == DRAIN_LEVEL_EMPTY )*/
            /*..hui [24-4-1오후 5:03:29] 수위 에러일경우에도 정지.. 수위 에러 함수에서 따로 처리..*/
            if( u8DrainWaterLevel == DRAIN_LEVEL_EMPTY || u8DrainWaterLevel == DRAIN_LEVEL_ERROR )
            {
                Bit9_Drain_Ice_Off_24_Hour = CLEAR;
            }
            else{}
        }
        else{}
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void first_drain_check(void)
{
    /*..hui [23-4-4오후 2:35:55] 드레인 탱크 센서 읽는시간 대기..*/
    if(F_FW_Version_Display_Mode == CLEAR)
    {
        return;
    }
    else{}

    if( bit_first_drain == SET )
    {
        bit_first_drain = CLEAR;

        if( u8DrainWaterLevel == DRAIN_LEVEL_LOW )
        {
            Bit6_Drain_First = SET;
        }
        else
        {
            Bit6_Drain_First = CLEAR;
        }
    }
    else
    {
        /*if( u8DrainWaterLevel == DRAIN_LEVEL_EMPTY )*/
        /*..hui [24-4-1오후 5:03:29] 수위 에러일경우에도 정지.. 수위 에러 함수에서 따로 처리..*/
        if( u8DrainWaterLevel == DRAIN_LEVEL_EMPTY || u8DrainWaterLevel == DRAIN_LEVEL_ERROR )
        {
            Bit6_Drain_First = CLEAR;
        }
        else{}
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void detect_drain_pump_error(void)
{
    U8 mu8_error_chk_enable = 0;

    /*..hui [17-12-15오후 10:40:27] 만수위 감지시 저수위 미감지될때까지 드레인펌프 가동..*/
    /*..hui [17-12-18오후 1:19:04] 수위센서 에러시 가동 추가..*/
    detect_drain_tank_high_level();

    /*..hui [17-12-18오후 5:49:40] 저수위 3시간 유지 확인..*/
    keep_3_hour_check();

    /*..hui [17-12-18오후 5:49:47] 만빙 상태 확인..*/
    ice_full_condition_check();

    /*..hui [19-12-2오후 2:16:19] 얼음기능 OFF 체크..*/
    ice_off_check();


    /*..hui [23-7-7오후 2:38:42] 얼음 OFF 후 24시간 후에 드레인..*/
    ice_off_24hour_check();

    /*..hui [18-2-12오후 4:08:15] 탱크 강제 배수 기능 실행 확인..*/
    forced_drain_check();

    /*..hui [23-4-4오후 2:38:22] 최초 전원 ON시 드레인 저수위 감지 상태이면 가동.. 아이콘아이스 사양 추가..*/
    first_drain_check();

    /*..hui [23-5-15오후 4:55:48] 아이스트레이 입수 끝나고 트레이 내리고 버린물 드레인 해준다..*/
    /*..hui [23-5-15오후 4:56:02] 최대한 드레인탱크 물 없는 상태 유지하도록.. 임재한부장..*/
    ice_make_drain_check();
    
    /*..hui [25-4-17오후 3:44:27] 플러싱할때 드레인..*/
    flushing_drain_check();

    /*..hui [23-5-22오후 1:08:55] 얼음탱크 살균할때 드레인..*/
    tray_ster_drain_check();

    /*..hui [24-7-25오후 4:08:10] 구연산 세척 진행할때 드레인..*/
    /////acid_drain_check();

    /*..hui [23-7-7오후 2:27:48] 드레인탱크 수위센서 에러일때..*/
    /*Bit10_Drain_Level_Error = CLEAR;*/
    // detect_drain_tank_error_level();

    /*check_empty_icedetect_drain_pump_error_tray_in();*/

    /*..hui [18-1-22오후 4:34:56] 순환배수 기능 작동중일때 동작하지 않도록 변경..*/
    /*..hui [19-10-23오전 11:39:05] 온수 추출중일때는 정지했다 처음부터 다시 시작(펌프 압)..*/
    /*..hui [20-4-21오후 12:30:57] 첫잔드레인 및 히터 쿨링시 드레인 종료 후 다시 시작 추가..*/
    /*..hui [23-4-4오후 2:40:31] 최초 전원 ON시 저수위일경우 가동 추가 - 아이콘아이스 사양 - 신뢰성..*/



    /* 2KG는 펌프를 돌리는 모든 연관된 기능에 에러 체크를 진행해야 함 (수배관 특성) 250623 CH.PARK */
    if( u16DrainOperate > 0 )
    {
        if( F_DrainStatus == CLEAR && bit_manual_drain_start == CLEAR )
        {
            if( bit_ice_tank_ster_start == SET && gu8_ice_ster_mode == STER_MODE_HOT_WAIT_STATE
                /*&& gu8_hot_wait_step <= 3 )*/
                && gu8_hot_wait_step <= 5 )
            {
                /*..hui [25-7-7오후 3:48:47] 드레인 탱크에 온수 넣을때 드레인 안하고 대기..*/
                bit_drain_error_check_enable = CLEAR;
            }
            else if( bit_ice_tank_ster_start == SET && gu8_ice_ster_mode == STER_MODE_DRAIN_INPUT_STATE )
            {
                /*..hui [25-7-7오후 3:48:47] 드레인 탱크에 온수 넣을때 드레인 안하고 대기..*/
                bit_drain_error_check_enable = CLEAR;
            }
            else if( bit_ice_tank_ster_start == SET && gu8_ice_ster_mode == STER_MODE_DRAIN_WAIT_STATE )
            {
                /*..hui [25-7-7오후 3:48:57] 온수 넣고 기다리는 동안 드레인 안하고 대기..*/
                bit_drain_error_check_enable = CLEAR;
            }
            else
            {
                bit_drain_error_check_enable = SET;
            }
        }
        else
        {
            bit_drain_error_check_enable = SET;
        }
        // bit_drain_error_check_enable = SET;   
    }
    else
    {
        bit_drain_error_check_enable = CLEAR;
    }

    /*if(mu8_error_chk_enable != SET || Bit16_Drain_Pump_Error__E60 == SET)*/
    if( bit_drain_error_check_enable == CLEAR || Bit16_Drain_Pump_Error__E60 == SET)
    {
        gu16_drain_pump_first_op_timer = 0;
        /*gu8_drain_pump_retry_step = DRAIN_RETRY_PUMP_1MIN_OFF;*/
        gu8_drain_pump_retry_step = DRAIN_RETRY_INIT;
        gu16_drain_pump_2min_on_timer = 0;
        gu16_drain_pump_1min_off_timer = 0;
        bit_drain_err_check_on_state = CLEAR;
        /*bit_drain_err_check_off_state = CLEAR;*/
        gu8_drain_err_10_count = 0;
    }
    else
    {
        gu16_drain_pump_first_op_timer++;

        if( gu16_drain_pump_first_op_timer >= DRAIN_ERROR_PUMP_ON_10_MIN )   //10min
        {
            gu16_drain_pump_first_op_timer = DRAIN_ERROR_PUMP_ON_10_MIN;

            bit_drain_err_check_on_state = drain_pump_error_retry_proc();
            /*bit_drain_err_check_off_state = ~bit_drain_err_check_on_state;*/
        }
        else
        {
            bit_drain_err_check_on_state = SET;
            /*bit_drain_err_check_off_state = CLEAR;*/
        }
    }
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 drain_pump_error_retry_proc( void )
{
    U8 mu8_return = CLEAR;

    /*..hui [17-12-18오후 3:29:43] 10분 가동해도 저수위 해제가 안되면 2분 ON, 1분 OFF 10회 반복..*/
    switch( gu8_drain_pump_retry_step )
    {
        case DRAIN_RETRY_INIT :

            gu16_drain_pump_1min_off_timer++;

            /*..hui [24-3-28오후 2:30:21] 1분간 대기..*/
            if( gu16_drain_pump_1min_off_timer >= DRAIN_ERROR_PUMP_OFF_1_MIN )
            {
                gu16_drain_pump_1min_off_timer = 0;
                gu16_drain_pump_2min_on_timer = 0;

                /*..hui [24-3-28오후 2:30:53] 10분 가동후에 수위가 저수위 감지, 만수위 미감지일 경우..*/
                /*..hui [24-3-28오후 2:31:04] 저수위 센서 물방울에 의한 일시적 오감지로 가정..*/
                /*..hui [24-4-1오후 7:29:22] AD 입력도 확인... 배수 막힘 고려.. 물 가득차있을때 냉수 드레인 열면 안됨..*/
                /*..hui [24-4-5오후 5:47:09] 얼음탱크 세척중에는 냉수 드레인 금지..*/

                /*..hui [24-4-8오후 3:50:16] 얼음탱크 세척 중에도 동일하게....*/
                if( u8DrainWaterLevel == DRAIN_LEVEL_LOW
                && bit_drain_no_water_detect == SET )
                {
                    gu8_drain_pump_retry_step = DRAIN_RETRY_COLD_DRIN_ON;
                }
                else
                {
                    /*..hui [24-3-28오후 2:25:26] 그 외의 경우에는 바로 재시도 동작으로 진입..*/
                    gu8_drain_pump_retry_step = DRAIN_RETRY_PUMP_1MIN_OFF;
                }
            }
            else{}

            break;

        case DRAIN_RETRY_COLD_DRIN_ON :
            gu16_drain_pump_2min_on_timer++;

            /*..hui [24-3-28오후 2:45:34] 냉수 드레인 밸브 15초 동안 ON..1초당 유량 약 12ml = 180ml 정도..*/
            if( gu16_drain_pump_2min_on_timer >= 150 )
            {
                gu16_drain_pump_1min_off_timer = 0;
                gu16_drain_pump_2min_on_timer = 0;
                gu8_drain_pump_retry_step = DRAIN_RETRY_PUMP_1MIN_OFF;
            }
            else{}

            break;

        case DRAIN_RETRY_PUMP_1MIN_OFF :

            gu16_drain_pump_1min_off_timer++;

            if( gu16_drain_pump_1min_off_timer >= DRAIN_ERROR_PUMP_OFF_1_MIN )
            {
                gu16_drain_pump_1min_off_timer = 0;
                gu16_drain_pump_2min_on_timer = 0;
                gu8_drain_pump_retry_step = DRAIN_RETRY_PUMP_2MIN_ON;
                /*mu8_return = SET;*/

                /*if(gu8_drain_err_10_count >= 10)*/
                if(gu8_drain_err_10_count >= 12)
                {
                    /*..hui [17-12-18오후 5:33:37] 에러 set..*/
                    gu8_drain_err_10_count = 0;

                    Bit16_Drain_Pump_Error__E60 = SET;
                    mu8_return = CLEAR;
                }
                else
                {
                    mu8_return = SET;
                }
            }
            else
            {
                mu8_return = CLEAR;
            }

            break;

        case DRAIN_RETRY_PUMP_2MIN_ON :

            gu16_drain_pump_2min_on_timer++;

            if(gu16_drain_pump_2min_on_timer >= DRAIN_ERROR_PUMP_ON_2_MIN)
            {
                gu16_drain_pump_1min_off_timer = 0;
                gu16_drain_pump_2min_on_timer = 0;
                gu8_drain_pump_retry_step = DRAIN_RETRY_PUMP_1MIN_OFF;

                gu8_drain_err_10_count++;
                mu8_return = CLEAR;
            }
            else
            {
                mu8_return = SET;
            }

            break;

        default :
             //
             mu8_return = CLEAR;
            break;
    }

    return mu8_return;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void forced_drain_check(void)
{
    /*..hui [18-2-12오후 4:13:12] 정수탱크, 냉수탱크 비움상태일때 드레인탱크가 저수위 이상이면 강제배수 기능 실행..*/
    if( /*u8DrainWaterLevel >= DRAIN_LEVEL_LOW*/
        ( u8DrainWaterLevel == DRAIN_LEVEL_LOW || u8DrainWaterLevel == DRAIN_LEVEL_HIGH )
        && gu8_Room_Water_Level == ROOM_LEVEL_LOW
        && F_Tank_Cover_Input == CLEAR )
    {
        Bit3_Drain_Forced = SET;
        gu8_forced_drain_add_timer = 0;
    }
    else
    {
        if(Bit3_Drain_Forced == SET)
        {
            if(u8DrainWaterLevel == DRAIN_LEVEL_EMPTY)
            {
                gu8_forced_drain_add_timer++;

                /*..hui [18-2-12오후 4:14:14] 배수 완료 후 10초 추가 배수..*/
                if(gu8_forced_drain_add_timer >= FORCED_DRAIN_ADDITIONAL_TIME)
                {
                    gu8_forced_drain_add_timer = 0;
                    Bit3_Drain_Forced = CLEAR;
                }
                else
                {
                    Bit3_Drain_Forced = SET;
                }
            }
            else{}
        }
        else
        {
            gu8_forced_drain_add_timer = 0;
        }
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void ice_make_drain_check(void)
{
    /*..hui [23-5-15오후 5:20:05] 트레이 물 버리고 핫가스 시간 계산할때 저수위이면 드레인 진행..*/
    /*..hui [23-5-15오후 5:20:31] 1회에 한해.. 만수위일경우에는 기존대로 드레인됨..*/
    if( gu8IceStep == STATE_42_CALC_HOT_GAS_TIME )
    {
        if(u8DrainWaterLevel == DRAIN_LEVEL_LOW)
        {
            Bit7_Drain_Ice_Making = SET;
        }
        else
        {
            Bit7_Drain_Ice_Making = CLEAR;
        }
    }
    else
    {
        /*..hui [23-7-7오후 1:29:17] 최소 가동시간 가동 이후 무부하 전류 감지 시 정지..*/
        /*if( u8DrainWaterLevel == DRAIN_LEVEL_EMPTY || bit_drain_no_water_detect == SET )*/
        /*if( u8DrainWaterLevel == DRAIN_LEVEL_EMPTY )*/
        /*..hui [24-4-1오후 5:03:29] 수위 에러일경우에도 정지.. 수위 에러 함수에서 따로 처리..*/
        // if( u8DrainWaterLevel == DRAIN_LEVEL_EMPTY || u8DrainWaterLevel == DRAIN_LEVEL_ERROR )
        if( u8DrainWaterLevel == DRAIN_LEVEL_EMPTY )
        {
            Bit7_Drain_Ice_Making = CLEAR;
        }
        else{}
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void flushing_drain_check(void)
{
    if( gu8_flushing_mode > FLUSHING_NONE_STATE )
    {
        if( gu8_flushing_mode == FLUSHING_FILL_COLD_TANK_START_STATE )
        {
            if( gu8_cold_tank_fill_step == 1 )
            {
                if( u8DrainWaterLevel == DRAIN_LEVEL_LOW )
                {
                    Bit4_Drain_Flushing = SET;
                }
                else
                {
                    if( Bit4_Drain_Flushing == SET )
                    {
                        if( u8DrainWaterLevel == DRAIN_LEVEL_EMPTY )
                        {
                            Bit4_Drain_Flushing = CLEAR;
                        }
                        else{}
                    }
                    else{}
                }
            }
            else
            {
                Bit4_Drain_Flushing = CLEAR;
            }
        }
        else
        {
            if( u8DrainWaterLevel == DRAIN_LEVEL_EMPTY )
            {
                Bit4_Drain_Flushing = CLEAR;
            }
            else{}
        }
    }
    else
    {
        if( u8DrainWaterLevel == DRAIN_LEVEL_EMPTY )
        {
            Bit4_Drain_Flushing = CLEAR;
        }
        else{}
    }
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void tray_ster_drain_check(void)
{
	if( bit_ice_tank_ster_start == SET )
	{
		if( gu8_ice_ster_mode == STER_MODE_PREPARE )
		{
			if( gu8_prepare_ster_step <= 3 )
			{
				if( u8DrainWaterLevel == DRAIN_LEVEL_LOW )
				{
					Bit8_Drain_Tray_ster = SET;
				}
				else
				{
					if( Bit8_Drain_Tray_ster == SET )
					{
						if( u8DrainWaterLevel == DRAIN_LEVEL_EMPTY )
						{
							Bit8_Drain_Tray_ster = CLEAR;
						}
						else{}
					}
					else{}
				}
			}
			else
			{
				if( u8DrainWaterLevel == DRAIN_LEVEL_EMPTY )
				{
					Bit8_Drain_Tray_ster = CLEAR;
				}
				else{}
			}
		}
		else if( gu8_ice_ster_mode == STER_MODE_HOT_WAIT_STATE )
		{
			/*if( gu8_hot_wait_step == 4 )*/
			if( gu8_hot_wait_step == 6 )
			{
				if( u8DrainWaterLevel == DRAIN_LEVEL_LOW )
				{
					Bit8_Drain_Tray_ster = SET;
				}
				else
				{
					if( Bit8_Drain_Tray_ster == SET )
					{
						if( u8DrainWaterLevel == DRAIN_LEVEL_EMPTY )
						{
							Bit8_Drain_Tray_ster = CLEAR;
						}
						else{}
					}
					else{}
				}
			}
			else
			{
				if( u8DrainWaterLevel == DRAIN_LEVEL_EMPTY )
				{
					Bit8_Drain_Tray_ster = CLEAR;
				}
				else{}
			}
		}
		else if( gu8_ice_ster_mode == STER_MODE_FINISH )
		{
			if( u8DrainWaterLevel == DRAIN_LEVEL_LOW )
			{
				Bit8_Drain_Tray_ster = SET;
			}
			else
			{
				if( Bit8_Drain_Tray_ster == SET )
				{
					if( u8DrainWaterLevel == DRAIN_LEVEL_EMPTY )
					{
							Bit8_Drain_Tray_ster = CLEAR;
					}
					else{}
				}
				else{}
			}
		}
		else
		{
			if( u8DrainWaterLevel == DRAIN_LEVEL_EMPTY )
			{
				Bit8_Drain_Tray_ster = CLEAR;
			}
			else{}
		}
	}
	else
	{
		if( u8DrainWaterLevel == DRAIN_LEVEL_EMPTY )
		{
			Bit8_Drain_Tray_ster = CLEAR;
		}
		else{}
	}
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void start_drain_pump( U16 u16_data )
{
    gu16_drain_pwm_out = u16_data;
    TDR13 = gu16_drain_pwm_out;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void stop_drain_pump( void )
{
    gu16_drain_pwm_out = 0;
    TDR13 = gu16_drain_pwm_out;
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void drain_melt_water(void)
{
    U16 mu16_drain_time = 0;
    U32 mu32_standby_time = 0;

    if( Bit16_Drain_Pump_Error__E60 == CLEAR )
    {
        gu32_drain_melt_standby_timer = 0;
        gu16_drain_melt_op_timer = 0;
        bit_drain_melt_pump_output = CLEAR;
        bit_ice_melt_2_hour = CLEAR;
        gu8_drain_melt_cnt = 0;
        bit_drain_melt_finish = CLEAR;
        gu8_drain_error_clear_timer = 0;
        return;
    }
    else
    {
        /*..hui [24-3-28오전 9:47:39] 에러 걸린 후 탱크 녹은물 강제 배수 종료.. 이후부터는 전원 껐다 켜야 에러 해제됨..*/
        if( bit_drain_melt_finish == SET )
        {
            return;
        }
        else{}
    }


    if( gu8_drain_melt_cnt < 6 )
    {
        /*..hui [24-3-28오전 9:37:53] 6회까지는 2시간 간격..*/
        mu32_standby_time = 72000;
    }
    else
    {
        /*..hui [24-3-28오전 9:38:06] 7회부터는 3시간 간격..*/
        mu32_standby_time = 108000;
    }

    gu32_drain_melt_standby_timer++;

    /*..hui [24-3-27오후 4:50:29] 에러 발생하고 2시간 경과 확인..*/
    /*if( gu32_drain_melt_standby_timer >= 72000 )*/
    if( gu32_drain_melt_standby_timer >= mu32_standby_time )
    {
        /*gu8_drain_melt_cnt++;*/

        if( gu8_drain_melt_cnt >= 12 )
        {
            gu8_drain_melt_cnt = 12;
            /*gu32_drain_melt_standby_timer = 72000;*/
            gu32_drain_melt_standby_timer = 0;
            bit_ice_melt_2_hour = CLEAR;

            /*..hui [24-3-28오전 9:47:02] 탱크 녹은물 강제배수 종료..*/
            bit_drain_melt_finish = SET;
        }
        else
        {
            gu8_drain_melt_cnt++;
            gu32_drain_melt_standby_timer = 0;
            bit_ice_melt_2_hour = SET;
        }
    }
    else{}

    if( gu8_drain_melt_cnt <= 6 )
    {
        /*..hui [24-3-28오전 9:38:21] 6회까지는 1분동안 배수..*/
        mu16_drain_time = 600;
    }
    else
    {
        /*..hui [24-3-28오전 9:38:27] 7회부터는 30초간 배수..*/
        mu16_drain_time = 300;
    }

    /*..hui [24-3-27오후 4:50:35] 2시간 경과시..*/
    if( bit_ice_melt_2_hour == SET )
    {
        gu16_drain_melt_op_timer++;

        /*..hui [24-3-27오후 4:50:42] 1분동안 강제로 드레인 펌프 가동..*/
        /*if( gu16_drain_melt_op_timer >= 600 )*/
        if( gu16_drain_melt_op_timer >= mu16_drain_time )
        {
            gu16_drain_melt_op_timer = 0;
            bit_drain_melt_pump_output = CLEAR;
            bit_ice_melt_2_hour = CLEAR;
        }
        else
        {
            bit_drain_melt_pump_output = SET;

            if( u8DrainWaterLevel == DRAIN_LEVEL_EMPTY )
            {
                gu8_drain_error_clear_timer++;

                /*..hui [24-3-28오전 10:00:42] 드레인 수위센서가 5초 연속 물 없음 감지 시 에러 해제 후 정상 복귀..*/
                if( gu8_drain_error_clear_timer >= 50 )
                {
                    gu8_drain_error_clear_timer = 0;
                    Bit16_Drain_Pump_Error__E60 = CLEAR;
                }
                else{}
            }
            else
            {
                gu8_drain_error_clear_timer = 0;
            }
        }
    }
    else
    {
        gu16_drain_melt_op_timer = 0;
        bit_drain_melt_pump_output = CLEAR;
        gu8_drain_error_clear_timer = 0;
    }


}



/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/





