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
#include    "self_test_manual_load.h"
/***********************************************************************************************************************/
U8 manual_test_load(void);
U8 check_current( U8 mu8_load );
void output_load( U8 mu8_load, U8 mu8_out );
U8 manual_test_faucet(void);
U8 manual_test_flow(void);
/***********************************************************************************************************************/
U8 gu8_component_test_step;
U16 gu16_component_test_timer;
/***********************************************************************************************************************/
U8 gu8_current_check_step;
U16 gu16_current_check_timer;
/***********************************************************************************************************************/
U8 gu8_self_flow_test_step;
U16 gu16_self_flow_test_timer;
/***********************************************************************************************************************/
bit bit_self_faucet_reed_1;
bit bit_self_faucet_reed_2;
bit bit_self_faucet_reed_3;
bit bit_self_faucet_reed_4;
/***********************************************************************************************************************/
U16 gu16_self_tray_flow_save;
U16 gu16_self_ro_flow_save;
U16 gu16_gas_switch_current_max;
/***********************************************************************************************************************/
U8 gu8_heater_test_step;
U16 gu16_heater_test_timer;
/***********************************************************************************************************************/
U16 gu16_diagnos_flow_val;
/***********************************************************************************************************************/
/**
 * @brief 고장진단 부하 제어
 */
U8 manual_test_load(void)
{
    U16 mu16_val = 0;
    U8 mu8_decision = 0;
    U8 mu8_ok = 0;
    U8 mu8_return = 0;

    switch( gu8_component_test_step )
    {
        case 0 :

            gu16_component_test_timer = 0;
            gu16_current_check_timer = 0;
            gu8_component_test_step=2;

        break;

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
        case 2 :

            mu8_ok = check_current( DC_LOAD__NOS_VALVE );

            if( mu8_ok == SET )
            {
                gu8_component_test_step++;
            }
            else{}

        break;

        case 3 :

            mu8_ok = check_current( DC_LOAD__HOT_OUT_VALVE );

            if( mu8_ok == SET )
            {
                gu8_component_test_step++;
            }
            else{}

        break;

        case 4 :

            mu8_ok = check_current( DC_LOAD__FLOWVALVE_MOTOR );

            if( mu8_ok == SET )
            {
                gu8_component_test_step++;
            }
            else{}

        break;

        case 5 :

            mu8_ok = check_current( DC_LOAD__ICE_TRAY_MOTOR);

            if( mu8_ok == SET )
            {
                gu8_component_test_step++;
            }
            else{}

        break;

        case 6 :

            mu8_ok = check_current( DC_LOAD__UV_ICE_FAUCET1 );

            if( mu8_ok == SET )
            {
                gu8_component_test_step++;
            }
            else{}

        break;

        case 7 :

            mu8_ok = check_current( DC_LOAD__UV_FAUCET );

            if( mu8_ok == SET )
            {
                gu8_component_test_step++;
            }
            else{}


        break;

        case 8 :

            mu8_ok = check_current( DC_LOAD__UV_ICE_FAUCET2 );

            if( mu8_ok == SET )
            {
                gu16_component_test_timer = 0;
                gu8_component_test_step++;
            }
            else{}

        break;

        case 9 :

           mu8_ok = check_current( DC_LOAD__UV_ICE_TANK_1_2 );

            if( mu8_ok == SET )
            {
                gu8_component_test_step++;
            }
            else{}

        break;


        case 10 :

            mu8_ok = check_current( DC_LOAD__SWING_BAR );

            if( mu8_ok == SET )
            {
                gu8_component_test_step++;
            }
            else{}

        break;

        case 11 :

            mu8_ok = check_current( DC_LOAD__ICE_DOOR_MOTOR );


            if( mu8_ok == SET )
            {
                gu8_component_test_step++;
            }
            else{}

        break;

        case 12 :

            mu8_ok = check_current( DC_LOAD__GAS_SWITCH_MOTOR );

            if( mu8_ok == SET )
            {
                gu8_component_test_step++;
            }
            else{}

        break;

        case 13 :

            mu8_ok = check_current( DC_LOAD__DRAIN_PUMP );

            if( mu8_ok == SET )
            {
                gu8_component_test_step++;
            }
            else{}

        break;

        case 14 :

            // mu8_ok = check_current( DC_LOAD__ICE_FULL_SENSOR );

            // if( mu8_ok == SET )
            // {
            //     gu8_component_test_step++;
            // }
            // else{}

            gu8_component_test_step++;
        break;

        case 15 :

            mu8_ok = check_current( DC_LOAD__COLD_FAN );

            if( mu8_ok == SET )
            {
                gu8_component_test_step++;
            }
            else{}

        break;

        case 16 :

            mu8_ok = check_current( DC_LOAD__UV_ICE_TANK_3 );

            if( mu8_ok == SET )
            {
                gu8_component_test_step++;
            }
            else{}

        break;

        case 17 :

            mu8_ok = check_current( DC_LOAD__UV_ICE_TRAY );

            if( mu8_ok == SET )
            {
                gu8_component_test_step++;
            }
            else{}

        break;

        case 18 :

            mu8_ok = check_current( DC_LOAD__PURE_IN_VALVE);

            if( mu8_ok == SET )
            {
                gu8_component_test_step++;
            }
            else{}

        break;

            case 19 :

            mu8_ok = check_current( DC_LOAD__OVERFLOW_VALVE );

            if( mu8_ok == SET )
            {
                gu8_component_test_step++;
            }
            else{}

        break;

        case 20 :
            mu8_ok = check_current( DC_LOAD__HOT_IN_VALVE );

            if( mu8_ok == SET )
            {
                gu8_component_test_step++;
            }
            else{}
        break;

        case 21 :
            mu8_ok = check_current( DC_LOAD__HOT_DRAIN_VALVE );

            if( mu8_ok == SET )
            {
                gu8_component_test_step++;
            }
            else{}
        break;

        case 22 :
            mu8_ok = check_current( DC_LOAD__COLD_IN_VALVE );

            if( mu8_ok == SET )
            {
                gu8_component_test_step++;
            }
            else{}
        break;

        case 23 :
            mu8_ok = check_current( DC_LOAD__COLD_DRAIN_VALVE );

            if( mu8_ok == SET )
            {
                gu8_component_test_step++;
            }
            else{}
        break;

        case 24 :
            mu8_ok = check_current( DC_LOAD__ICETRAY_IN_VALVE );

            if( mu8_ok == SET )
            {
                gu8_component_test_step++;
            }
            else{}
        break;

        case 25 :
            mu8_ok = check_current( DC_LOAD__COLD_PURE_OUT_VALVE );

            if( mu8_ok == SET )
            {
                gu8_component_test_step++;
            }
            else{}
        break;

        case 26:
            mu8_ok = check_current(DC_LOAD__ICE_WATER_EXTRACT_VALVE);       // 2KG MAX 얼음물 추출밸브

            if( mu8_ok == SET )
            {
                gu8_component_test_step++;
            }
            else{}
        break;

        case 27:
            mu8_ok = check_current(DC_LOAD__ICE_DOOR_HEATER);               // 2KG MAX 얼음도어 히터

            if( mu8_ok == SET )
            {
                gu8_component_test_step++;
            }
            else{}
        break;

        case 28:
            mu8_ok = check_current(DC_LOAD__ICE_SELECT_DOOR_MOTOR);            // LPP 얼음셀렉트 도어 추가 250611 CH.PARK

            if( mu8_ok == SET )
            {
                gu8_component_test_step++;
            }
            else{}
        break;

        case 29 :

            gu8_component_test_step = 0;
            gu16_component_test_timer = 0;
            mu8_return = SET;

        break;

        default :

            gu8_component_test_step = 0;
            gu16_component_test_timer = 0;

        break;
    }

    return mu8_return;
}

/***********************************************************************************************************************/
/**
 * @brief 전류피드백 확인
 */
U8 check_current( U8 mu8_load )
{
    U16 mu16_val = 0;
    U8 mu8_decision = 0;
    U8 mu8_return = 0;
    U16 mu16_stable_time = 0;

    if( mu8_load == DC_LOAD__HOT_OUT_VALVE )
    {
        mu16_stable_time = 30;
    }
    else if( mu8_load == DC_LOAD__PURE_IN_VALVE )
    {
        mu16_stable_time = 30;
    }
    else if( mu8_load == DC_LOAD__NOS_VALVE
    ||mu8_load == DC_LOAD__HOT_OUT_VALVE
    ||mu8_load == DC_LOAD__PURE_IN_VALVE
    ||mu8_load == DC_LOAD__OVERFLOW_VALVE
    ||mu8_load == DC_LOAD__HOT_IN_VALVE
    ||mu8_load == DC_LOAD__HOT_DRAIN_VALVE
    ||mu8_load == DC_LOAD__COLD_IN_VALVE
    ||mu8_load == DC_LOAD__COLD_DRAIN_VALVE
    ||mu8_load == DC_LOAD__ICETRAY_IN_VALVE
    ||mu8_load == DC_LOAD__COLD_PURE_OUT_VALVE
    ||mu8_load == DC_LOAD__ICE_WATER_EXTRACT_VALVE
    )
    {
        mu16_stable_time = 30;
    }
    else if( mu8_load == DC_LOAD__UV_ICE_FAUCET1
    || mu8_load == DC_LOAD__UV_FAUCET
    || mu8_load == DC_LOAD__UV_ICE_FAUCET1
    || mu8_load == DC_LOAD__UV_ICE_FAUCET2
    || mu8_load == DC_LOAD__UV_ICE_TANK_1_2
    || mu8_load == DC_LOAD__UV_ICE_TRAY
    || mu8_load == DC_LOAD__UV_ICE_TANK_3
    )
    {
        mu16_stable_time = 20;
    }
    else if( mu8_load == DC_LOAD__ICE_DOOR_MOTOR )
    {
        mu16_stable_time = 20;
    }
    else if( mu8_load == DC_LOAD__ICE_SELECT_DOOR_MOTOR )
    {
        mu16_stable_time = 20;
    }
    else if( mu8_load == DC_LOAD__GAS_SWITCH_MOTOR )
    {
        mu16_stable_time = 30;
    }
    else if( mu8_load == DC_LOAD__ICE_TRAY_MOTOR )
    {
        mu16_stable_time = 30;
    }
    else if( mu8_load == DC_LOAD__FLOWVALVE_MOTOR )
    {
        mu16_stable_time = 30;
    }
    else
    {
        mu16_stable_time = DC_CURRENT_STABLE_TIME;
    }

    switch( gu8_current_check_step )
    {
        case 0 :

            gu16_current_check_timer = 0;
            gu8_current_check_step++;

        break;

        case 1 :

            gu16_current_check_timer++;

            /*..hui [20-10-28오후 3:08:07] 전류 떨어질때까지 1초 대기..*/
            if( gu16_current_check_timer >= CURRENT_DELAY_TIME )
            {
                gu16_current_check_timer = 0;
                gu8_current_check_step++;
                gu16_gas_switch_current_max = 0;
            }
            else{}

        break;

        case 2 :

            output_load( mu8_load, ON );
            gu8_current_check_step++;

        break;

        case 3 :

            gu16_current_check_timer++;

            /*..hui [20-10-28오후 1:30:44] 부하 켜고 3초 후 전류 체크..*/
            if( gu16_current_check_timer >= mu16_stable_time )
            {
                gu16_current_check_timer = 0;

                if( mu8_load == DC_LOAD__COLD_FAN )
                {
                    mu16_val = calculate_fan_current( gu16_AD_Result_Fan_Current );
                    /*..hui [20-10-28오후 9:26:06] 최종 값 업데이트..*/
                    SELF_Test_Result_Data.word[ DC_LOAD__COLD_FAN ] = mu16_val;
                }
                else if( mu8_load == DC_LOAD__UV_FAUCET )
                {
                    mu16_val = calculate_uv_current( gu16_AD_Result_UV_Water_Faucet_Current_Feed );
                    SELF_Test_Result_Data.word[ DC_LOAD__UV_FAUCET ] = mu16_val;
                }
                else if( mu8_load == DC_LOAD__UV_ICE_FAUCET1 )
                {
                    mu16_val = calculate_uv_current( gu16_AD_Result_UV_Ice_Faucet_One_Current );
                    SELF_Test_Result_Data.word[ DC_LOAD__UV_ICE_FAUCET1 ] = mu16_val;
                }
                else if( mu8_load == DC_LOAD__UV_ICE_FAUCET2 )
                {
                    mu16_val = calculate_uv_current( gu16_AD_Result_UV_Ice_Faucet_Two_Current );
                    SELF_Test_Result_Data.word[ DC_LOAD__UV_ICE_FAUCET2 ] = mu16_val;
                }
                else if( mu8_load == DC_LOAD__UV_ICE_TANK_1_2 )
                {
                    mu16_val = calculate_uv_current( gu16_AD_Result_UV_Ice_Tank_1_2_Current );
                    SELF_Test_Result_Data.word[ DC_LOAD__UV_ICE_TANK_1_2 ] = mu16_val;
                }
                else if( mu8_load == DC_LOAD__UV_ICE_TRAY )
                {
                    mu16_val = calculate_uv_current( gu16_AD_Result_UV_Ice_Tray_1_2_Current_Feed );
                    SELF_Test_Result_Data.word[ DC_LOAD__UV_ICE_TRAY ] = mu16_val;
                }
                else if( mu8_load == DC_LOAD__UV_ICE_TANK_3 )
                {
                    mu16_val = calculate_uv_current( gu16_AD_Result_UV_Ice_Tank_3_Current_Feed );
                    SELF_Test_Result_Data.word[ DC_LOAD__UV_ICE_TANK_3 ] = mu16_val;
                }
                else if( mu8_load == DC_LOAD__GAS_SWITCH_MOTOR )
                {
                    mu16_val = calculate_fan_current( gu16_AD_Result_DC_Current_12V );
                    //mu16_val = calculate_fan_current( gu16_gas_switch_current_max );

                    /*..hui [24-1-9오후 2:40:02] 냉매전환밸브는 12V..*/
                    SELF_Test_Result_Data.word[ DC_LOAD__GAS_SWITCH_MOTOR ] = mu16_val;
                }
                else if( mu8_load == DC_LOAD__DRAIN_PUMP )
                {
                    mu16_val = calculate_pump_current( gu16_AD_Drain_Pump_Current );
                    /*..hui [20-10-28오후 9:26:06] 최종 값 업데이트..*/
                    SELF_Test_Result_Data.word[ DC_LOAD__DRAIN_PUMP ] = mu16_val;
                }
                else if( mu8_load == DC_LOAD__ICE_DOOR_HEATER)
                {
                    mu16_val = gu16_AD_Result_IceDoor_Heater_Current; // calculate_pump_current( gu16_AD_Result_IceDoor_Heater_Current );
                    /*..hui [20-10-28오후 9:26:06] 최종 값 업데이트..*/
                    SELF_Test_Result_Data.word[ DC_LOAD__ICE_DOOR_HEATER ] = mu16_val;
                }
                else
                {
                    // 얼음 셀렉트 도어, 얼음 도어 등등
                    mu16_val = calculate_dc_current( gu16_AD_Result_DC_Current_24V );
                    /*..hui [20-10-28오후 9:26:06] 최종 값 업데이트..*/
                    SELF_Test_Result_Data.word[ mu8_load ] = mu16_val;
                }

                output_load( mu8_load, OFF );

                gu8_current_check_step++;
            }
            else
            {
                if( mu8_load == DC_LOAD__GAS_SWITCH_MOTOR )
                {
                    /*..hui [24-1-19오후 5:43:31] 냉매전환밸브 최대값으로 하기위해..*/
                    if( gu16_gas_switch_current_max < gu16_AD_Result_DC_Current_12V )
                    {
                        gu16_gas_switch_current_max  = gu16_AD_Result_DC_Current_12V;
                    }
                    else{}
                }
                else
                {
                    gu16_gas_switch_current_max = 0;
                }
            }

        break;

        case 4 :

            mu8_return = SET;
            gu8_current_check_step = 0;
            gu16_current_check_timer = 0;

        break;

        default :

            gu8_current_check_step = 0;
            gu16_current_check_timer = 0;

        break;
    }

    return mu8_return;
}

/***********************************************************************************************************************/
/**
 * @brief 고장진단 부하 출력
 */
void output_load( U8 mu8_load, U8 mu8_out )
{
    if( mu8_load == DC_LOAD__NOS_VALVE )
    {
        pVALVE_NOS = mu8_out;
    }
    else if( mu8_load == DC_LOAD__HOT_OUT_VALVE )
    {
        pVALVE_HOT_OUT = mu8_out;
    }
        else if( mu8_load == DC_LOAD__PURE_IN_VALVE )
    {
        pVALVE_ROOM_IN = mu8_out;
    }
    else if( mu8_load == DC_LOAD__OVERFLOW_VALVE )
    {
        pVALVE_HOT_COLD_OVERFLOW = mu8_out;
    }
    else if( mu8_load == DC_LOAD__HOT_IN_VALVE )
    {
        pVALVE_HOT_IN = mu8_out;
    }
    else if( mu8_load == DC_LOAD__HOT_DRAIN_VALVE )
    {
        pVALVE_HOT_DRAIN = mu8_out;
    }
    else if( mu8_load == DC_LOAD__COLD_IN_VALVE )
    {
        pVALVE_COLD_IN = mu8_out;
    }
    else if( mu8_load == DC_LOAD__COLD_DRAIN_VALVE )
    {
        pVALVE_COLD_DRAIN = mu8_out;
    }
    else if( mu8_load == DC_LOAD__ICETRAY_IN_VALVE )
    {
        pVALVE_ICE_TRAY_IN = mu8_out;
    }
    else if( mu8_load == DC_LOAD__COLD_PURE_OUT_VALVE )
    {
        pVALVE_ROOM_COLD_EXTRACT = mu8_out;
    }
    else if( mu8_load == DC_LOAD__ICE_WATER_EXTRACT_VALVE )
    {
        pVALVE_ICE_WATER_EXTRACT = mu8_out;
    }
    else if( mu8_load == DC_LOAD__UV_FAUCET )
    {
        pUV_LED_WATER_FAUCET = mu8_out;
    }
    else if( mu8_load == DC_LOAD__UV_ICE_FAUCET1 )
    {
        pUV_LED_ICE_FAUCET_1 = mu8_out;
    }
    else if( mu8_load == DC_LOAD__UV_ICE_FAUCET2 )
    {
        pUV_LED_ICE_FAUCET_2 = mu8_out;
    }
    else if( mu8_load == DC_LOAD__UV_ICE_TRAY )
    {
        pUV_LED_ICE_TRAY_1_2 = mu8_out;
    }
    else if( mu8_load == DC_LOAD__UV_ICE_TANK_1_2 )
    {
        pUV_LED_ICE_TANK_1_2 = mu8_out;
    }
    else if ( mu8_load == DC_LOAD__UV_ICE_TANK_3)
    {
        pUV_LED_ICE_TANK_3 = mu8_out;
    }
    else if( mu8_load == DC_LOAD__SWING_BAR )
    {
        pSWING_BAR = mu8_out;
    }
    else if( mu8_load == DC_LOAD__ICE_DOOR_MOTOR )
    {
        F_IceDoorClose = SET;
    }
    else if( mu8_load == DC_LOAD__ICE_SELECT_DOOR_MOTOR )   // 2KG MAX
    {
        F_IceSelectDoorClose = SET;
    }
    else if( mu8_load == DC_LOAD__ICE_DOOR_HEATER)          // 2KG MAX
    {
        pHEATER_ICE_DOOR = mu8_out;
    }
    // else if( mu8_load == DC_LOAD__ICE_CRUSH_MOTOR)          // 2KG MAX
    // {
    //     P_OUT_ICE_CRUSH = mu8_out;
    // }
    else if( mu8_load == DC_LOAD__GAS_SWITCH_MOTOR )
    {
        GasSwitchInit();
    }
    else if( mu8_load == DC_LOAD__FLOWVALVE_MOTOR )
    {
        if( mu8_out == ON )
        {
            /*run_increase_flow( 500 );*/
            run_init_flow();
        }
        else
        {
            gu16_flow_info_target = gu16_flow_info_current;
        }
    }
    else if( mu8_load == DC_LOAD__ICE_TRAY_MOTOR )
    {
        if( mu8_out == ON )
        {
            /*run_increase_flow( 500 );*/
            // Bit2_Ice_Tray_Test_Start = SET;
            cody_ice_tray_test();
        }
        else
        {
            gu16_flow_info_target = gu16_flow_info_current;
        }
    }
    else if( mu8_load == DC_LOAD__DRAIN_PUMP )
    {
        if( mu8_out == ON )
        {
            start_drain_pump( PUMP_PWM_MAX );
        }
        else
        {
            stop_drain_pump();
        }
    }
    else if( mu8_load == DC_LOAD__ICE_FULL_SENSOR )
    {
        pIR_POWER = mu8_out;
    }
    else if( mu8_load == DC_LOAD__ICE_LACK_SENSOR )
    {
        pIR_POWER2 = mu8_out;
    }
    else if( mu8_load == DC_LOAD__COLD_FAN )
    {
        pDC_FAN = mu8_out;
    }
    else
    {

    }
}

/***********************************************************************************************************************/
/**
 * @brief 유량 검사
 */
U8 manual_test_flow(void)
{
    U8 mu8_return = 0;
    U16 mu16_val = 0;

    switch( gu8_self_flow_test_step )
    {
        case 0:

            SELF_Test_Result_Data.word[DC_LOAD__FLOW_SENSOR] = 0;
            gu16_self_tray_flow_save = 0;
            gu16_self_ro_flow_save = 0;

            gu16_self_flow_test_timer = 0;
            gu8_self_flow_test_step++;

            break;

        case 1:

            gu16_self_flow_test_timer++;

            if( gu16_self_flow_test_timer >= SELF_MANUAL_FLOW_TEST_TIME )
            {
 //               pVALVE_ICE_TRAY_IN_FEED = CLEAR;
                //stop_tray_pump();

                gu16_self_flow_test_timer = 0;
                gu8_self_flow_test_step++;

                /*..hui [23-12-21오전 11:01:02] 47hz === 300cc..*/
                /*..hui [23-12-21오전 11:01:08] 79hz == 500cc..*/
                /*..hui [23-12-21오전 11:01:14] 161hz ===1000cc..*/
                /*..hui [23-12-21오전 11:01:29] hz에 6 곱하고 20 더해주면 얼추 비슷하게 나옴....*/
                // SELF_Test_Result_Data.word[DC_LOAD__FLOW_SENSOR] = gu16_self_tray_flow_save * 6;
                // SELF_Test_Result_Data.word[DC_LOAD__FLOW_SENSOR] = SELF_Test_Result_Data.word[DC_LOAD__FLOW_SENSOR] + 20;

                SELF_Test_Result_Data.word[DC_LOAD__ICE_FULL_SENSOR] = gu16ADIceFull;
                SELF_Test_Result_Data.word[DC_LOAD__ICE_LACK_SENSOR] = gu16ADIceLow;
                pIR_POWER = CLEAR;
                pIR_POWER2 = CLEAR;
            }
            else
            {
//                pVALVE_ICE_TRAY_IN_FEED = SET;
                //start_tray_pump( PUMP_PWM_MAX );

                if( gu8_Flow_1sec > gu16_self_tray_flow_save )
                {
                    gu16_self_tray_flow_save = gu8_Flow_1sec;
                }
                else{}

                /*..hui [23-12-22오전 9:20:26] 만빙센서 검사도 여기서..*/
                pIR_POWER = SET;
                pIR_POWER2 = SET;
            }

            break;

        case 2:

            //run_open_ro_drain();
            gu16_self_flow_test_timer = 0;
            gu8_self_flow_test_step++;

            break;

        case 3:


            //gu16_self_flow_test_timer = 0;
            //gu8_self_flow_test_step = 0;

            //mu8_return = SET;

            //break;
#if 1
            gu16_self_flow_test_timer++;

            if( gu16_self_flow_test_timer >= SELF_MANUAL_FLOW_TEST_TIME )
            {
                pVALVE_ROOM_IN = CLEAR;
                pVALVE_ROOM_COLD_EXTRACT = CLEAR;

                gu16_self_flow_test_timer = 0;
                gu8_self_flow_test_step++;

                /*..hui [23-12-21오전 11:01:02] 47hz === 300cc..*/
                /*..hui [23-12-21오전 11:01:08] 79hz == 500cc..*/
                /*..hui [23-12-21오전 11:01:14] 161hz ===1000cc..*/
                /*..hui [23-12-21오전 11:01:29] hz에 6 곱하고 20 더해주면 얼추 비슷하게 나옴....*/
                SELF_Test_Result_Data.word[DC_LOAD__FLOW_SENSOR] = (gu16_diagnos_flow_val * 6);
                if(SELF_Test_Result_Data.word[DC_LOAD__FLOW_SENSOR] > 0)
				{
                	SELF_Test_Result_Data.word[DC_LOAD__FLOW_SENSOR] = SELF_Test_Result_Data.word[DC_LOAD__FLOW_SENSOR] + 20;
				}
				else
				{
					SELF_Test_Result_Data.word[DC_LOAD__FLOW_SENSOR] = 0;
				}
            }
            else
            {
                pVALVE_ROOM_IN = SET;
                pVALVE_ROOM_COLD_EXTRACT = SET;
                if( gu8_Flow_1sec > gu16_diagnos_flow_val )
                {
                    gu16_diagnos_flow_val = gu8_Flow_1sec;
                }
                else{}
            }

            break;

        case 4:

            gu16_self_flow_test_timer = 0;
            gu8_self_flow_test_step = 0;
            pVALVE_ROOM_IN = CLEAR;
            pVALVE_ROOM_COLD_EXTRACT = CLEAR;

            mu8_return = SET;

            break;
#endif
default:

            gu16_self_flow_test_timer = 0;
            gu8_self_flow_test_step = 0;

            break;
    }

    return mu8_return;
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 manual_test_heater(void)
{
    U8 mu8_return = 0;

    switch( gu8_heater_test_step )
    {
        case 0 :

            /*..hui [21-9-8오전 11:32:13] 히터 온도 90도 이상에서는 테스트 하지 않는다..*/
            /*..hui [21-9-9오후 3:38:22] CP 모델일때도 안한다..*/
            /*..hui [21-12-9오후 3:12:18] 온수 첫잔 완료 전에 진행시에도 히터 검사 안함..*/
            if(  gu8_Hot_Heater_Temperature_One_Degree >= HEATER_TEST_PROHIBIT_TEMP
            || F_First_Hot_Effluent == SET )
            {
                initial_each_self_data( AC_LOAD_HOT_TRIAC );
                initial_each_self_data( AC_LOAD_HOT_RELAY );
                gu16_heater_test_timer = 0;
                gu8_heater_test_step = 3;
            }
            else
            {
                //U16_HEATER_TRIAC_0302 = 0;
                //U16_HEATER_RELAY_0303 = 0;
                SELF_Test_Result_Data.word[ AC_LOAD_HOT_TRIAC ] = 0;
                SELF_Test_Result_Data.word[ AC_LOAD_HOT_RELAY ] = 0;
                gu16_heater_test_timer = 0;
                gu8_heater_test_step++;
            }

        break;


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
        case 1 :

            gu16_heater_test_timer++;

            if( gu16_heater_test_timer >= 5 )
            {
                pHEATER_HOT_TRIAC_L = CLEAR;

                /*if( gu16_heater_test_timer >= 10 )*/

                /*..hui [21-12-2오후 4:12:21] 50 미만일 겨우 에러..*/
                /*..hui [21-12-2오후 4:12:39] AD 전압 완전 떨어진 후 다음거 체크해야..*/
                if( gu16_heater_test_timer >= 30 || gu16_AD_Result_Heater_Current_Feed <= 20  )
                {
                    if( gu16_heater_test_timer >= 10 )
                    {
                        gu16_heater_test_timer = 0;
                        gu8_heater_test_step++;
                    }
                    else{}
                }
                else{}
            }
            else
            {
                pHEATER_HOT_TRIAC_L = SET;
            }


            //if(gu16_AD_Result_Heater_Current_Feed > U16_HEATER_TRIAC_0302)
            if(gu16_AD_Result_Heater_Current_Feed > SELF_Test_Result_Data.word[ AC_LOAD_HOT_TRIAC ])
            {
                SELF_Test_Result_Data.word[ AC_LOAD_HOT_TRIAC ] = gu16_AD_Result_Heater_Current_Feed;
            }
            else{}

        break;

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
        case 2 :

            gu16_heater_test_timer++;

            if( gu16_heater_test_timer >= 5 )
            {
                pHEATER_HOT_H_RELAY = CLEAR;

                if( gu16_heater_test_timer >= 10 )
                {
                    gu16_heater_test_timer = 0;
                    gu8_heater_test_step++;
                }
                else{}
            }
            else
            {
                pHEATER_HOT_H_RELAY = SET;
            }

            if(gu16_AD_Result_Heater_Current_Feed > SELF_Test_Result_Data.word[ AC_LOAD_HOT_RELAY ])
            {
                SELF_Test_Result_Data.word[ AC_LOAD_HOT_RELAY ] = gu16_AD_Result_Heater_Current_Feed;
            }
            else{}

        break;

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
        case 3 :

            pHEATER_HOT_TRIAC_L = CLEAR;
            pHEATER_HOT_H_RELAY = CLEAR;

            gu16_heater_test_timer = 0;
            gu8_heater_test_step = 0;
            mu8_return = SET;

        break;

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

        default :

            gu16_heater_test_timer = 0;
            gu8_heater_test_step = 0;

        break;

    }

    return mu8_return;
}

/***********************************************************************************************************************/

/***********************************************************************************************************************/

/***********************************************************************************************************************/

/***********************************************************************************************************************/

/***********************************************************************************************************************/

/***********************************************************************************************************************/



