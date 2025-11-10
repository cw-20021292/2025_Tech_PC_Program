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
#include    "ster_ice_tray_op.h"


U8 prepare_ster(void);
U8 hot_preheat_ster(void);
U8 hot_input_ster(void);
U8 hot_wait_ster(void);
U8 wait_finish_clean(void);
U8 drain_preheat_ster(void);
U8 drain_input_ster(void);
U8 drain_wait_ster(void);


U8 gu8_prepare_ster_step;
U16 gu16_prepare_ster_timer;
U16 gu16_prepare_ster_max_timer;

U8 gu8_preheat_ster_step;
U16 gu16_preheat_ster_timer;
U32 gu32_preheat_ster_max_timer;

U8 gu8_hot_input_ster_step;
U16 gu16_hot_input_ster_timer;
U32 gu32_hot_input_ster_max_timer;

U8 gu8_hot_wait_step;
U16 gu16_hot_wait_timer;
U32 gu32_hot_wait_max_timer;

U8 gu8_wait_finish_step;
U16 gu16_wait_finish_timer;
U16 gu16_wait_finish_max_timer;


U8 gu8_drain_preheat_ster_step;
U16 gu16_drain_preheat_ster_timer;
U32 gu32_drain_preheat_ster_max_timer;

U8 gu8_drain_input_ster_step;
U16 gu16_drain_input_ster_timer;
U32 gu32_drain_input_ster_max_timer;

U8 gu8_drain_wait_step;
U16 gu16_drain_wait_timer;
U32 gu32_drain_wait_max_timer;




/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 prepare_ster(void)
{
    U8 mu8_return = 0;

    switch( gu8_prepare_ster_step )
    {
        case 0:

            /*..hui [23-5-15오후 7:39:52] 대기..*/
            gu16_prepare_ster_timer++;

            if( gu16_prepare_ster_timer >= ICE_TANK_STER_DELAY_TIME )
            {
                gu16_prepare_ster_timer = 0;
                gu16_prepare_ster_max_timer = 0;
                gu8_prepare_ster_step++;
            }
            else{}

            break;

        case 1:

            gu8_cody_take_off_ice_start = SET;
            gu16_prepare_ster_timer = 0;
            gu16_prepare_ster_max_timer = 0;
            gu8_prepare_ster_step++;

            break;

        case 2:

            if( gu8IceStep == STATE_0_STANDBY )
            {
                gu16_prepare_ster_timer = 0;
                gu16_prepare_ster_max_timer = 0;
                gu8_prepare_ster_step++;
            }
            else
            {
                /*..hui [23-5-22오전 11:15:23] 최대 제한시간은 핫가스 최대 시간 + 2분정도..*/
                gu16_prepare_ster_timer++;

                if( gu16_prepare_ster_timer >= HOT_GAS_TIME_9_UNDER_765S + 1200 )
                {
                    gu16_prepare_ster_timer = 0;
                    gu16_prepare_ster_max_timer = 0;
                    gu8_prepare_ster_step++;
                }
                else{}
            }

            break;

        case 3:

            if( u8DrainWaterLevel == DRAIN_LEVEL_EMPTY )
            {
                gu16_prepare_ster_timer = 0;
                gu16_prepare_ster_max_timer = 0;
                gu8_prepare_ster_step++;
            }
            else
            {
                /*..hui [23-5-22오전 11:38:20] 드레인 대기 최대 시간은 우선 5분으로..*/
                gu16_prepare_ster_timer++;

                if( gu16_prepare_ster_timer >= 6000 )
                {
                    gu16_prepare_ster_timer = 0;
                    gu16_prepare_ster_max_timer = 0;
                    gu8_prepare_ster_step++;
                }
                else{}
            }

            break;

        case 4:

            /*..hui [23-8-24오후 2:21:23] 트레이 재시도 중이거나 에러일 경우..*/
            if( gu8_ice_system_ok == SET )
            {
                if( F_TrayMotorDOWN == CLEAR )
                {
                    /*..hui [23-7-5오후 7:37:03] 트레이 제빙 방향 이동..*/
                    up_tray_motor();    // 고온살균 시 상승
                    gu16_prepare_ster_timer = 0;
                    gu16_prepare_ster_max_timer = 0;
                    gu8_prepare_ster_step++;
                }
                else
                {
                    gu16_prepare_ster_max_timer++;

                    if( gu16_prepare_ster_max_timer >= 600 )
                    {
                        /*..hui [25-4-4오전 10:05:36] 하던거 중단 취소..*/
                        //gu16_prepare_ster_timer = 0;
                        //gu16_prepare_ster_max_timer = 0;
                        //gu8_prepare_ster_step++;
                        finish_ice_ster();
                    }
                    else{}
                }
            }
            else
            {
                /*..hui [25-4-4오전 10:05:44] 하던거 중단 취소..*/
                //gu16_prepare_ster_timer = 0;
                //gu16_prepare_ster_max_timer = 0;
                //gu8_prepare_ster_step++;
                finish_ice_ster();
            }

            break;

        case 5:

            /*..hui [23-7-5오후 7:37:03] 트레이 제빙 방향 이동..*/
            if( gu8IceTrayLEV == ICE_TRAY_POSITION_ICE_MAKING )
            {
                gu16_prepare_ster_timer = 0;
                gu16_prepare_ster_max_timer = 0;
                gu8_prepare_ster_step++;
            }
            else
            {
                gu16_prepare_ster_max_timer++;

                /*..hui [23-8-16오후 1:12:24] 트레이 이동 최대 시간 1분..*/
                if( gu16_prepare_ster_max_timer >= 600 )
                {
                    /*..hui [25-4-4오전 10:05:44] 하던거 중단 취소..*/
                    //gu16_prepare_ster_timer = 0;
                    //gu16_prepare_ster_max_timer = 0;
                    //gu8_prepare_ster_step++;
                    finish_ice_ster();
                }
                else{}
            }

            break;


        case 6:

            gu16_prepare_ster_timer = 0;
            gu16_prepare_ster_max_timer = 0;
            gu8_prepare_ster_step = 0;

            /*..hui [23-5-22오전 11:39:04] 살균 준비작업 종료..*/
            mu8_return = SET;

            break;

        default:

            gu16_prepare_ster_timer = 0;
            gu16_prepare_ster_max_timer = 0;
            gu8_prepare_ster_step = 0;

        break;

    }

    return mu8_return;
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 hot_preheat_ster(void)
{
    U8 mu8_return = 0;
    U8 mu8_max_time = 0;

    switch( gu8_preheat_ster_step )
    {
        case 0:

            gu16_preheat_ster_timer++;

            if( gu16_preheat_ster_timer >= ICE_TANK_STER_DELAY_TIME )
            {
                gu16_preheat_ster_timer = 0;
                gu32_preheat_ster_max_timer = 0;
                gu8_preheat_ster_step++;
            }
            else{}

            break;

        case 1:

            gu16_preheat_ster_timer++;

            if( gu16_preheat_ster_timer >= ICE_TANK_STER_DELAY_TIME )
            {
                gu16_preheat_ster_timer = 0;
                gu32_preheat_ster_max_timer = 0;
                gu8_preheat_ster_step++;
            }
            else{}

            break;

        case 2:

            gu16_preheat_ster_timer++;

            /*..hui [25-4-4오후 1:45:17] 온수 입수밸브 ON..*/
            if( gu16_preheat_ster_timer >= ICE_TANK_STER_DELAY_TIME )
            {
                gu16_preheat_ster_timer = 0;
                gu32_preheat_ster_max_timer = 0;
                gu8_preheat_ster_step++;
            }
            else{}

            break;

        case 3:

            if( gu8_Flow_1sec > HOT_BLOCKED_FLOW_HZ )
            {
                gu16_preheat_ster_timer++;

                if( gu16_preheat_ster_timer >= 10 )
                {
                    gu16_preheat_ster_timer = 0;
                    gu32_preheat_ster_max_timer = 0;
                    gu8_preheat_ster_step++;
                }
                else{}
            }
            else
            {
                gu32_preheat_ster_max_timer++;

                if( gu32_preheat_ster_max_timer >= 300 )
                {
                    /*..hui [25-4-4오전 10:05:44] 하던거 중단 취소..*/
                    finish_ice_ster();
                }
                else{}
            }

            break;

        case 4:

            gu32_preheat_ster_max_timer++;

            /*if( (gu8_Hot_Heater_Temperature_One_Degree < ICE_TRAY_STER_PREHEAT_TEMP - 10)
                || (gu32_preheat_ster_max_timer >= ICE_TRAY_PRE_DRAIN_MAX_TIME) )*/
            if( (gu8_Hot_Heater_Temperature_One_Degree < ICE_TRAY_STER_PREHEAT_TEMP - 40)
                || (gu32_preheat_ster_max_timer >= ICE_TRAY_PRE_DRAIN_MAX_TIME) )
            {
                gu16_preheat_ster_timer = 0;
                gu32_preheat_ster_max_timer = 0;
                gu8_preheat_ster_step++;
            }
            else{}

            break;

        case 5:

            /*..hui [25-4-4오후 2:01:08] 예열..*/
            /*if( gu8_Hot_Heater_Temperature_One_Degree >= ICE_TRAY_STER_PREHEAT_TEMP + 5 )*/
            if( gu8_Hot_Heater_Temperature_One_Degree >= ICE_TRAY_STER_PREHEAT_TEMP - 18 )
            {
                gu16_preheat_ster_timer = 0;
                gu32_preheat_ster_max_timer = 0;
                gu8_preheat_ster_step++;
            }
            else
            {
                gu32_preheat_ster_max_timer++;

                if( gu32_preheat_ster_max_timer >= 100 )
                {
                    gu16_preheat_ster_timer = 0;
                    gu32_preheat_ster_max_timer = 0;
                    gu8_preheat_ster_step++;
                }
                else{}
            }

            break;

        case 6:

            gu16_preheat_ster_timer++;

            if( gu16_preheat_ster_timer >= 1 )
            {
                gu16_preheat_ster_timer = 0;
                gu32_preheat_ster_max_timer = 0;
                gu8_preheat_ster_step++;
            }
            else{}

            break;

        case 7:

            /*..hui [25-4-4오후 2:09:26] 예열 완료..*/
            gu16_preheat_ster_timer = 0;
            gu32_preheat_ster_max_timer = 0;
            gu8_preheat_ster_step = 0;

            mu8_return = SET;

            break;

        default:

            gu16_preheat_ster_timer = 0;
            gu32_preheat_ster_max_timer = 0;
            gu8_preheat_ster_step = 0;

        break;

    }

    return mu8_return;
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 hot_input_ster(void)
{
    U8 mu8_return = 0;

    switch( gu8_hot_input_ster_step )
    {
        case 0:

            gu16_hot_input_ster_timer++;

            /*..hui [25-4-4오후 2:14:36] 트레이 입수 밸브 ON..*/
            if( gu16_hot_input_ster_timer >= ICE_TANK_STER_DELAY_TIME )
            {
                gu16_hot_input_ster_timer = 0;
                gu32_hot_input_ster_max_timer = 0;
                gu8_hot_input_ster_step++;
            }
            else{}

            break;

        case 1:

            gu16_hot_input_ster_timer++;

            if( gu16_hot_input_ster_timer >= ICE_TANK_STER_DELAY_TIME )
            {
                gu16_hot_input_ster_timer = 0;
                gu32_hot_input_ster_max_timer = 0;
                gu8_hot_input_ster_step++;

                gu16_Tray_Ster_Hz = HOT_STER_INPUT_HZ;
            }
            else{}

            break;

        case 2:

            if( gu16_Tray_Ster_Hz == 0 )
            {
                gu16_hot_input_ster_timer = 0;
                gu32_hot_input_ster_max_timer = 0;
                gu8_hot_input_ster_step++;
            }
            else{}

            break;

        case 3:

            gu16_hot_input_ster_timer++;

            /*..hui [25-4-4오후 2:41:54] 온수히터 OFF..*/
            if( gu16_hot_input_ster_timer >= ICE_TANK_STER_DELAY_TIME )
            {
                gu16_hot_input_ster_timer = 0;
                gu32_hot_input_ster_max_timer = 0;
                gu8_hot_input_ster_step++;
            }
            else{}

            break;

        case 4:

            gu16_hot_input_ster_timer++;

            /*..hui [25-4-4오후 2:41:24] 온수 입수 밸브 OFF..*/
            if( gu16_hot_input_ster_timer >= ICE_TANK_STER_DELAY_TIME )
            {
                gu16_hot_input_ster_timer = 0;
                gu32_hot_input_ster_max_timer = 0;
                gu8_hot_input_ster_step++;
            }
            else{}

            break;

        case 5:

            gu16_hot_input_ster_timer++;

            /*..hui [25-4-4오후 2:42:40] 트레이 입수 밸브 OFF..*/
            if( gu16_hot_input_ster_timer >= ICE_TANK_STER_DELAY_TIME )
            {
                gu16_hot_input_ster_timer = 0;
                gu32_hot_input_ster_max_timer = 0;
                gu8_hot_input_ster_step++;
            }
            else{}

            break;

        case 6:

            gu16_hot_input_ster_timer = 0;
            gu32_hot_input_ster_max_timer = 0;
            gu8_hot_input_ster_step = 0;

            /*..hui [25-4-4오후 2:43:08] 트레이 온수 입수 완료..*/
            mu8_return = SET;

            break;

        default:

            gu16_hot_input_ster_timer = 0;
            gu32_hot_input_ster_max_timer = 0;
            gu8_hot_input_ster_step = 0;

        break;

    }

    return mu8_return;
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 hot_wait_ster(void)
{
    U8 mu8_return = 0;

    switch( gu8_hot_wait_step )
    {
        case 0:

            gu16_hot_wait_timer++;

            if( gu16_hot_wait_timer >= ICE_TANK_STER_DELAY_TIME )
            {
                gu16_hot_wait_timer = 0;
                gu32_hot_wait_max_timer = 0;
                gu8_hot_wait_step++;
            }
            else{}

            break;

        case 1:

            gu16_hot_wait_timer++;

            /*..hui [25-4-4오후 2:49:28] 5분동안 정체 대기..*/
            /*..hui [25-4-4오후 2:49:47] 여기서 냉수 입수 밸브 약 3초 ON..*/
            if( gu16_hot_wait_timer >= HOT_STER_WAIT_TIME )
            {
                gu16_hot_wait_timer = 0;
                gu32_hot_wait_max_timer = 0;
                gu8_hot_wait_step++;
            }
            else{}

            break;

        case 2:

            /*..hui [23-8-24오후 2:21:23] 트레이 재시도 중이거나 에러일 경우..*/
            if( gu8_ice_system_ok == SET )
            {
                if( F_TrayMotorUP == CLEAR )
                {
                    /*..hui [23-7-5오후 7:41:30] 트레이 탈빙 방향 이동..*/
                    down_tray_motor();
                    gu16_hot_wait_timer = 0;
                    gu32_hot_wait_max_timer = 0;
                    gu8_hot_wait_step++;
                }
                else
                {
                    gu32_hot_wait_max_timer++;

                    if( gu32_hot_wait_max_timer >= 600 )
                    {
                        gu16_hot_wait_timer = 0;
                        gu32_hot_wait_max_timer = 0;
                        gu8_hot_wait_step = 4;
                    }
                    else{}
                }
            }
            else
            {
                gu16_hot_wait_timer = 0;
                gu32_hot_wait_max_timer = 0;
                gu8_hot_wait_step = 4;
            }

            break;

        case 3:

            /*..hui [23-7-5오후 7:41:30] 트레이 탈빙 방향 이동..*/
            if( gu8IceTrayLEV == ICE_TRAY_POSITION_ICE_THROW )
            {
                gu16_hot_wait_timer = 0;
                gu32_hot_wait_max_timer = 0;
                gu8_hot_wait_step++;
            }
            else
            {
                gu32_hot_wait_max_timer++;

                /*..hui [23-8-16오후 1:12:24] 트레이 이동 최대 시간 1분..*/
                if( gu32_hot_wait_max_timer >= 600 )
                {
                    gu16_hot_wait_timer = 0;
                    gu32_hot_wait_max_timer = 0;
                    gu8_hot_wait_step++;
                }
                else{}
            }

            break;

        case 4:
            /*..hui [25-7-7오후 3:04:16] 여기서 냉수입수밸브 ON.. 드레인 만수위센서 감지될때까지..*/
            if( u8DrainWaterLevel == DRAIN_LEVEL_HIGH )
            {
                gu16_hot_wait_timer = 0;
                gu32_hot_wait_max_timer = 0;
                gu8_hot_wait_step++;
            }
            else
            {
                gu32_hot_wait_max_timer++;

                if( gu32_hot_wait_max_timer >= 600 )
                {
                    gu16_hot_wait_timer = 0;
                    gu32_hot_wait_max_timer = 0;
                    gu8_hot_wait_step++;
                }
                else{}
            }
            break;

        case 5:
            gu16_hot_wait_timer++;

            /*..hui [25-7-9오후 4:10:38] 7분간 방치..*/
            if( gu16_hot_wait_timer >= 4200 )
            {
                gu16_hot_wait_timer = 0;
                gu32_hot_wait_max_timer = 0;
                gu8_hot_wait_step++;
            }
            else{}

            break;

        case 6:

            if( u8DrainWaterLevel == DRAIN_LEVEL_EMPTY )
            {
                gu16_hot_wait_timer = 0;
                gu32_hot_wait_max_timer = 0;
                gu8_hot_wait_step++;
            }
            else
            {
                /*..hui [23-5-22오전 11:38:20] 드레인 대기 최대 시간은 우선 5분으로..*/
                gu32_hot_wait_max_timer++;

                if( gu32_hot_wait_max_timer >= 6000 )
                {
                    gu16_hot_wait_timer = 0;
                    gu32_hot_wait_max_timer = 0;
                    gu8_hot_wait_step++;
                }
                else{}
            }

            break;

        case 7:

            gu16_hot_wait_timer = 0;
            gu32_hot_wait_max_timer = 0;
            gu8_hot_wait_step = 0;

            /*..hui [25-4-4오후 2:51:00] 트레이 대기 완료..*/
            mu8_return = SET;

            break;

        default:

            gu16_hot_wait_timer = 0;
            gu32_hot_wait_max_timer = 0;
            gu8_hot_wait_step = 0;

        break;

    }

    return mu8_return;
}



/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 wait_finish_clean(void)
{
    U8 mu8_return = 0;

    switch( gu8_wait_finish_step )
    {
        case 0:

            gu16_wait_finish_timer++;

            if( gu16_wait_finish_timer >= ICE_TANK_STER_DELAY_TIME )
            {
                gu16_wait_finish_timer = 0;
                gu16_wait_finish_max_timer = 0;
                gu8_wait_finish_step++;
            }
            else{}

            break;

        case 1:

            gu16_wait_finish_timer++;

            /*if( gu16_wait_finish_timer >= 10 )*/
            /*..hui [25-4-11오전 10:36:50] 증발기 식을시간 2분..*/
            // if( gu16_wait_finish_timer >= 600 )
            /*..hui [25-7-9오후 4:03:01] 방치 시간 증대 - 증발기 식는시간 필요없음..*/
            if( gu16_wait_finish_timer >= 10 )
            {
                gu16_wait_finish_timer = 0;
                gu16_wait_finish_max_timer = 0;
                gu8_wait_finish_step++;
            }
            else{}

            break;

        case 2:

            gu16_wait_finish_timer++;

            if( gu16_wait_finish_timer >= 10 )
            {
                gu16_wait_finish_timer = 0;
                gu16_wait_finish_max_timer = 0;
                gu8_wait_finish_step++;
            }
            else{}

            break;

        case 3:

            gu16_wait_finish_timer = 0;
            gu16_wait_finish_max_timer = 0;
            gu8_wait_finish_step = 0;

            /*..hui [25-4-4오후 2:55:45] 완전히 끝..*/
            mu8_return = SET;

            break;

        default:

            gu16_wait_finish_timer = 0;
            gu16_wait_finish_max_timer = 0;
            gu8_wait_finish_step = 0;

        break;

    }

    return mu8_return;
}










/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 drain_preheat_ster(void)
{
    U8 mu8_return = 0;
    U8 mu8_max_time = 0;

    switch( gu8_drain_preheat_ster_step )
    {
        case 0:

            gu16_drain_preheat_ster_timer++;

            if( gu16_drain_preheat_ster_timer >= ICE_TANK_STER_DELAY_TIME )
            {
                gu16_drain_preheat_ster_timer = 0;
                gu32_drain_preheat_ster_max_timer = 0;
                gu8_drain_preheat_ster_step++;
            }
            else{}

            break;

        case 1:

            gu16_drain_preheat_ster_timer++;

            if( gu16_drain_preheat_ster_timer >= ICE_TANK_STER_DELAY_TIME )
            {
                gu16_drain_preheat_ster_timer = 0;
                gu32_drain_preheat_ster_max_timer = 0;
                gu8_drain_preheat_ster_step++;
            }
            else{}

            break;

        case 2:

            gu16_drain_preheat_ster_timer++;

            /*..hui [25-4-4오후 1:45:17] 온수 입수밸브 ON..*/
            if( gu16_drain_preheat_ster_timer >= ICE_TANK_STER_DELAY_TIME )
            {
                gu16_drain_preheat_ster_timer = 0;
                gu32_drain_preheat_ster_max_timer = 0;
                gu8_drain_preheat_ster_step++;
            }
            else{}

            break;

        case 3:

            if( gu8_Flow_1sec > HOT_BLOCKED_FLOW_HZ )
            {
                gu16_drain_preheat_ster_timer++;

                if( gu16_drain_preheat_ster_timer >= 10 )
                {
                    gu16_drain_preheat_ster_timer = 0;
                    gu32_drain_preheat_ster_max_timer = 0;
                    gu8_drain_preheat_ster_step++;
                }
                else{}
            }
            else
            {
                gu32_drain_preheat_ster_max_timer++;

                if( gu32_drain_preheat_ster_max_timer >= 300 )
                {
                    /*..hui [25-4-4오전 10:05:44] 하던거 중단 취소..*/
                    finish_ice_ster();
                }
                else{}
            }

            break;

        case 4:

            gu32_drain_preheat_ster_max_timer++;

            if( (gu8_Hot_Heater_Temperature_One_Degree < DRAIN_TANK_STER_PREHEAT_TEMP - 40)
                || (gu32_drain_preheat_ster_max_timer >= DRAIN_TANK_PRE_DRAIN_MAX_TIME) )
            {
                gu16_drain_preheat_ster_timer = 0;
                gu32_drain_preheat_ster_max_timer = 0;
                gu8_drain_preheat_ster_step++;
            }
            else{}

            break;

        case 5:

            /*..hui [25-4-4오후 2:01:08] 예열..*/
            /*if( gu8_Hot_Heater_Temperature_One_Degree >= ICE_TRAY_STER_PREHEAT_TEMP + 5 )*/
            if( gu8_Hot_Heater_Temperature_One_Degree >= DRAIN_TANK_STER_PREHEAT_TEMP - 10 )
            {
                gu16_drain_preheat_ster_timer = 0;
                gu32_drain_preheat_ster_max_timer = 0;
                gu8_drain_preheat_ster_step++;
            }
            else
            {
                gu32_drain_preheat_ster_max_timer++;

                if( gu32_drain_preheat_ster_max_timer >= 150 )
                {
                    gu16_drain_preheat_ster_timer = 0;
                    gu32_drain_preheat_ster_max_timer = 0;
                    gu8_drain_preheat_ster_step++;
                }
                else{}
            }

            break;

        case 6:

            gu16_drain_preheat_ster_timer++;

            if( gu16_drain_preheat_ster_timer >= 1 )
            {
                gu16_drain_preheat_ster_timer = 0;
                gu32_drain_preheat_ster_max_timer = 0;
                gu8_drain_preheat_ster_step++;
            }
            else{}

            break;

        case 7:

            /*..hui [25-4-4오후 2:09:26] 예열 완료..*/
            gu16_drain_preheat_ster_timer = 0;
            gu32_drain_preheat_ster_max_timer = 0;
            gu8_drain_preheat_ster_step = 0;

            mu8_return = SET;

            break;

        default:

            gu16_drain_preheat_ster_timer = 0;
            gu32_drain_preheat_ster_max_timer = 0;
            gu8_drain_preheat_ster_step = 0;

        break;

    }

    return mu8_return;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 drain_input_ster(void)
{
    U8 mu8_return = 0;

    switch( gu8_drain_input_ster_step )
    {
        case 0:

            gu16_drain_input_ster_timer++;

            /*..hui [25-4-4오후 2:14:36] 트레이 입수 밸브 ON..*/
            if( gu16_drain_input_ster_timer >= ICE_TANK_STER_DELAY_TIME )
            {
                gu16_drain_input_ster_timer = 0;
                gu32_drain_input_ster_max_timer = 0;
                gu8_drain_input_ster_step++;
            }
            else{}

            break;

        case 1:

            gu16_drain_input_ster_timer++;

            if( gu16_drain_input_ster_timer >= ICE_TANK_STER_DELAY_TIME )
            {
                gu16_drain_input_ster_timer = 0;
                gu32_drain_input_ster_max_timer = 0;
                gu8_drain_input_ster_step++;

                gu16_Tray_Ster_Hz = DRAIN_STER_INPUT_HZ;
            }
            else{}

            break;

        case 2:

            if( gu16_Tray_Ster_Hz == 0 )
            {
                gu16_drain_input_ster_timer = 0;
                gu32_drain_input_ster_max_timer = 0;
                gu8_drain_input_ster_step++;
            }
            else{}

            break;

        case 3:

            gu16_drain_input_ster_timer++;

            /*..hui [25-4-4오후 2:41:54] 온수히터 OFF..*/
            if( gu16_drain_input_ster_timer >= ICE_TANK_STER_DELAY_TIME )
            {
                gu16_drain_input_ster_timer = 0;
                gu32_drain_input_ster_max_timer = 0;
                gu8_drain_input_ster_step++;
            }
            else{}

            break;

        case 4:

            gu16_drain_input_ster_timer++;

            /*..hui [25-4-4오후 2:41:24] 온수 입수 밸브 OFF..*/
            if( gu16_drain_input_ster_timer >= ICE_TANK_STER_DELAY_TIME )
            {
                gu16_drain_input_ster_timer = 0;
                gu32_drain_input_ster_max_timer = 0;
                gu8_drain_input_ster_step++;
            }
            else{}

            break;

        case 5:

            gu16_drain_input_ster_timer++;

            /*..hui [25-4-4오후 2:42:40] 트레이 입수 밸브 OFF..*/
            if( gu16_drain_input_ster_timer >= ICE_TANK_STER_DELAY_TIME )
            {
                gu16_drain_input_ster_timer = 0;
                gu32_drain_input_ster_max_timer = 0;
                gu8_drain_input_ster_step++;
            }
            else{}

            break;

        case 6:

            gu16_drain_input_ster_timer = 0;
            gu32_drain_input_ster_max_timer = 0;
            gu8_drain_input_ster_step = 0;

            /*..hui [25-4-4오후 2:43:08] 트레이 온수 입수 완료..*/
            mu8_return = SET;

            break;

        default:

            gu16_drain_input_ster_timer = 0;
            gu32_drain_input_ster_max_timer = 0;
            gu8_drain_input_ster_step = 0;

        break;

    }

    return mu8_return;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 drain_wait_ster(void)
{
    U8 mu8_return = 0;

    switch( gu8_drain_wait_step )
    {
        case 0:

            gu16_drain_wait_timer++;

            if( gu16_drain_wait_timer >= ICE_TANK_STER_DELAY_TIME )
            {
                gu16_drain_wait_timer = 0;
                gu32_drain_wait_max_timer = 0;
                gu8_drain_wait_step++;
            }
            else{}

            break;

        case 1:

            gu16_drain_wait_timer++;

            /*..hui [25-4-4오후 2:49:28] 2분동안 정체 대기..*/
            if( gu16_drain_wait_timer >= DRAIN_STER_WAIT_TIME )
            {
                gu16_drain_wait_timer = 0;
                gu32_drain_wait_max_timer = 0;
                gu8_drain_wait_step++;
            }
            else{}

            break;

        case 2:

            /*..hui [25-7-7오후 3:04:16] 여기서 냉수입수밸브 ON.. 드레인 만수위센서 감지될때까지..*/
            if( u8DrainWaterLevel == DRAIN_LEVEL_HIGH )
            {
                gu16_drain_wait_timer = 0;
                gu32_drain_wait_max_timer = 0;
                gu8_drain_wait_step++;
            }
            else
            {
                gu32_drain_wait_max_timer++;

                if( gu32_drain_wait_max_timer >= 600 )
                {
                    gu16_drain_wait_timer = 0;
                    gu32_drain_wait_max_timer = 0;
                    gu8_drain_wait_step++;
                }
                else{}
            }

            break;

        case 3:

            gu16_drain_wait_timer++;

            /*..hui [25-7-7오후 3:04:48] 냉수랑 온수 섞이는 시간 10초 대기..*/
            /*..hui [25-7-9오후 4:01:59] 18분 대기로 변경..*/
            // if( gu16_drain_wait_timer >= 100 )
            if( gu16_drain_wait_timer >= 10800 )
            {
                gu16_drain_wait_timer = 0;
                gu32_drain_wait_max_timer = 0;
                gu8_drain_wait_step++;
            }
            else{}

            break;

        case 4:

            gu16_drain_wait_timer = 0;
            gu32_drain_wait_max_timer = 0;
            gu8_drain_wait_step = 0;

            /*..hui [25-4-4오후 2:51:00] 트레이 대기 완료..*/
            mu8_return = SET;

            break;

        default:

            gu16_drain_wait_timer = 0;
            gu32_drain_wait_max_timer = 0;
            gu8_drain_wait_step = 0;

        break;

    }

    return mu8_return;
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/



