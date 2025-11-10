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
#include    "aging.h"

void water_durable_test(void);

U16 gu16_durable_test_init_timer;
U32 gu32_durable_test_timer;
U16 gu16_durable_test_step;
U8 gu8_durable_test_start;

U32 gu32_aging_count;

U16 gu16_durable_test_step_old;
U16 gu16_Ice_Extract_30min_Timer;
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void water_durable_test(void)
{
    if( gu8_durable_test_start == CLEAR )
    {
        gu16_durable_test_init_timer = 0;
        gu16_durable_test_step = 0;
        gu32_durable_test_timer = 0;
        gu32_aging_count = 0;
        gu16_durable_test_step_old = 0;
        return;
    }
    else{}

    if(gu8_Error_Code != 0)
    {
        gu16_durable_test_step = 0;
        gu32_durable_test_timer = 0;
        gu16_durable_test_step_old = 0;
        return;
    }
    else{}

    if( gu8_flushing_mode > FLUSHING_NONE_STATE )
    {
        gu16_durable_test_step = 0;
        gu32_durable_test_timer = 0;
        gu16_durable_test_step_old = 0;
        return;
    }
    else{}

    if( F_FW_Version_Display_Mode == CLEAR )
    {
        gu16_durable_test_step = 0;
        gu32_durable_test_timer = 0;
        gu16_durable_test_step_old = 0;
        return;
    }
    else{}

    /* 트레이살균 내구성 프로그램 - 양산 전 삭제 */
    #ifdef __TRAY_STER_ENDURANCE__
    switch( gu16_durable_test_step )
    {
        case 0:

            gu32_durable_test_timer++;
            if(gu32_durable_test_timer >= 10)
            {
                gu32_durable_test_timer = 0;
                gu16_durable_test_step++;

                gu32_aging_count++;
                if( gu32_aging_count >= 999999 )
                {
                    gu32_aging_count = 999999;
                }
                else{}
            }
            else{}

            break;

        case 1:                             /* 트레이 고온살균 내구성 시작 */
            /* 냉수 선택 */
            key_manual_ice_tank_ster();
            gu16_durable_test_step++;
            break;

        case 2:                             /* 내구성 동작완료 */
            if(bit_ice_tank_ster_start == CLEAR)
            {
                gu32_durable_test_timer++;
                if(gu32_durable_test_timer >= 50)
                {
                    gu32_durable_test_timer = 0;
                    gu16_durable_test_step++;
                }
                else{}
            }
            break;

        case 3:                             /* 해제 후 2시간 대기 */
            gu32_durable_test_timer++;
            if(gu32_durable_test_timer >= AGING_TEST_MODE_DELAY_TIME_2HOUR)
            {
                gu32_durable_test_timer = 0;
                gu16_durable_test_step = 0;
            }
            else{}
            break;

        default:

            gu16_durable_test_step = 0;
            gu32_durable_test_timer = 0;

            break;
    }
    #endif

    /* 얼음추출 내구성 프로그램 - 양산 전 삭제할 것 */
    #ifdef __ICE_OUT_ENDURANCE__
    switch( gu16_durable_test_step )
    {
        case 0:

            gu32_durable_test_timer++;
            if(gu32_durable_test_timer >= 20)
            {
                gu32_durable_test_timer = 0;
                gu16_durable_test_step++;

                gu32_aging_count++;
                if( gu32_aging_count >= 999999 )
                {
                    gu32_aging_count = 999999;
                }
                else{}
            }
            else{}

            break;

        case 1:                                 /* 일반얼음 선택 */
            gu32_durable_test_timer++;

            if(gu32_durable_test_timer >= 20)
            {
                key_ice_select_job();
                F_IceBreak = CLEAR;
                gu8_ice_amount_step = ICE_LEVEL_4_STEP;
                gu16_durable_test_step++;
                gu32_durable_test_timer = 0;
            }
            break;

        case 2:                                 /* 얼음 추출 3단계 */

            gu32_durable_test_timer++;

            if(gu32_durable_test_timer >= 20)
            {
                ice_extraction_start();
                gu16_durable_test_step++;
                gu32_durable_test_timer = 0;
            }
            else{}

            break;

        case 3:                                 /* 추출완료 후 2시간 대기 */

            gu32_durable_test_timer++;
            if(gu32_durable_test_timer >= AGING_TEST_MODE_DELAY_TIME_30MIN)
            {
                gu32_durable_test_timer = 0;
                gu16_durable_test_step++;
            }
            else{}

            break;

        case 4:                                 /* 분쇄얼음 선택 */

            gu32_durable_test_timer++;
            if(gu32_durable_test_timer >= 20)
            {
                key_ice_select_job();
                F_IceBreak = SET;
                gu8_ice_amount_step = ICE_LEVEL_2_STEP;
                gu16_durable_test_step++;
                gu32_durable_test_timer = 0;
            }
            else{}

            break;

        case 5:                                 /* 얼음 3단계 추출 */
            gu32_durable_test_timer++;
            if(gu32_durable_test_timer >= 20)
            {
                ice_extraction_start();
                gu16_durable_test_step++;
                gu32_durable_test_timer = 0;
            }
            else{}

            break;

        case 6:                                 /* 추출완료 시 2시간 대기 */
            gu32_durable_test_timer++;
            if(gu32_durable_test_timer >= AGING_TEST_MODE_DELAY_TIME_30MIN)
            {
                gu32_durable_test_timer = 0;
                gu16_durable_test_step++;
            }
            else{}
            break;

        case 7:                                 /* 얼음물 선택 */
            gu32_durable_test_timer++;
            if(gu32_durable_test_timer >= 20)
            {
                key_ice_water_select_job();
                F_IceBreak = CLEAR;
                gu16_durable_test_step++;
                gu32_durable_test_timer = 0;
            }
            else{}
            break;

        case 8:                                 /* 얼음물 추출 */

            gu32_durable_test_timer++;
            if(gu32_durable_test_timer >= 20)
            {
                ice_extraction_start();
                gu16_durable_test_step++;
                gu32_durable_test_timer = 0;
            }
            else{}

            break;

        case 9:                                 /* 추출완료 시 처음부터 */
                gu32_durable_test_timer++;
                if(gu32_durable_test_timer >= AGING_TEST_MODE_DELAY_TIME_30MIN)
                {
                    gu32_durable_test_timer = 0;
                    gu16_durable_test_step = 0;
                }
                else {  }
            break;

        default:

            gu16_durable_test_step = 0;
            gu32_durable_test_timer = 0;

            break;
    }
    #endif

    #ifdef __HOT_WATER_ENDURANCE__
    switch( gu16_durable_test_step )
    {
        case 0:

            gu32_durable_test_timer++;

            if(gu32_durable_test_timer >= 10)
            {
                gu32_durable_test_timer = 0;
                gu16_durable_test_step++;

                gu32_aging_count++;
                if( gu32_aging_count >= 999999 )
                {
                    gu32_aging_count = 999999;
                }
                else{}
            }
            else{}

            break;

        case 1:
            /* 온수 선택 */
            key_hot_water_select_job();
            gu16_durable_test_step++;

            break;

        case 2:
            gu32_durable_test_timer++;
            if(gu32_durable_test_timer >= 10)
            {
                gu32_durable_test_timer = 0;
                if(gu8Cup_level != CUP_LEVEL_3_500ML)
                {
                    gu8Cup_level = CUP_LEVEL_3_500ML;
                }
                else {  }
                gu16_durable_test_step++;
            }
            else{}

            break;

        case 3:

            gu32_durable_test_timer++;
            if(gu32_durable_test_timer >= 10)
            {
                gu32_durable_test_timer = 0;
                water_extract_key();
                gu16_durable_test_step++;
            }
            else{}

            break;

        case 4:
            if( F_WaterOut == CLEAR )
            {
                gu32_durable_test_timer++;
                if(gu32_durable_test_timer >= AGING_TEST_MODE_DELAY_TIME_10MIN)
                {
                    gu32_durable_test_timer = 0;
                    gu16_durable_test_step = 0;
                }
                else{}
            }
            else
            {
                gu32_durable_test_timer = 0;
            }

            break;

        default:

            gu16_durable_test_step = 0;
            gu32_durable_test_timer = 0;

            break;
    }
    #endif

    #ifdef __GYULRO_TEST_ENDURANCE__
    switch( gu16_durable_test_step )
    {
        case 0:

            gu32_durable_test_timer++;

            if(gu32_durable_test_timer >= 10)
            {
                gu32_durable_test_timer = 0;
                gu16_durable_test_step++;

                gu32_aging_count++;
                if( gu32_aging_count >= 999999 )
                {
                    gu32_aging_count = 999999;
                }
                else{}
            }
            else{}

            break;

        case 1:
            /* 온수 선택 */
            key_hot_water_select_job();
            gu16_durable_test_step++;

            break;

        case 2:

            gu32_durable_test_timer++;

            if(gu32_durable_test_timer >= 10)
            {
                gu32_durable_test_timer = 0;
                water_extract_key();
                gu16_durable_test_step++;
            }
            else{}

            break;

        case 3:

            if( F_WaterOut == CLEAR )
            {
                gu32_durable_test_timer++;

                if(gu32_durable_test_timer >= 50)       /* 5초 후 */
                {
                    gu32_durable_test_timer = 0;
                    /* 정수 선택 */
                    key_ambient_water_select_job();
                    gu16_durable_test_step++;
                }
                else{}
            }
            else
            {
                gu32_durable_test_timer = 0;
            }

            break;

        case 4:

            gu32_durable_test_timer++;

            if(gu32_durable_test_timer >= 10)
            {
                gu32_durable_test_timer = 0;
                water_extract_key();

                gu16_durable_test_step++;
            }
            else{}

            break;

        case 5:

            if( F_WaterOut == CLEAR )
            {
                gu32_durable_test_timer++;

                if(gu32_durable_test_timer >= 50)       /* 5초 후 */
                {
                    gu32_durable_test_timer = 0;
                    /* 냉수 선택 */
                    key_cold_water_select_job();
                    gu16_durable_test_step++;
                }
                else{}
            }
            else
            {
                gu32_durable_test_timer = 0;
            }

            break;

        case 6:

            gu32_durable_test_timer++;

            if(gu32_durable_test_timer >= 10)
            {
                gu32_durable_test_timer = 0;
                water_extract_key();

                gu16_durable_test_step++;
                gu16_Ice_Extract_30min_Timer = 0;
            }
            else{}

            break;

        case 7:

            if( F_WaterOut == CLEAR )
            {
                gu32_durable_test_timer++;

                if(gu32_durable_test_timer >= 36000)            /* 1시간 대기 */
                {
                    gu32_durable_test_timer = 0;
                    gu16_durable_test_step = 0;
                }
                else{}

                if(F_IceFull == SET)
                {
                    gu16_Ice_Extract_30min_Timer++;
                    if(gu16_Ice_Extract_30min_Timer >= 18000)   /* 만빙 감지 30분 후 */
                    {
                        if(gu16_durable_test_step == 7)
                        {
                            gu16_Ice_Extract_30min_Timer = 0;

                            key_ice_select_job();
                            gu8_ice_amount_step = ICE_LEVEL_4_STEP;
                            ice_extraction_start();
                        }
                    }
                }
                else
                {
                    gu16_Ice_Extract_30min_Timer = 0;
                }
            }
            else
            {
                gu32_durable_test_timer = 0;
            }

            break;

        default:

            gu16_durable_test_step = 0;
            gu32_durable_test_timer = 0;

            break;
    }

    #endif

    #ifdef __AGING_TEST__

    #else
    /* 제품 내구성 */
    switch( gu16_durable_test_step )
    {
        case 0:

            gu32_durable_test_timer++;

            if(gu32_durable_test_timer >= 10)
            {
                gu32_durable_test_timer = 0;
                gu16_durable_test_step++;

                gu32_aging_count++;
                if( gu32_aging_count >= 999999 )
                {
                    gu32_aging_count = 999999;
                }
                else{}
            }
            else{}

            break;

        case 1:
            /* 냉수 선택 */
            key_cold_water_select_job();
            gu16_durable_test_step++;

            break;

        case 2:

            gu32_durable_test_timer++;

            if(gu32_durable_test_timer >= 10)
            {
                gu32_durable_test_timer = 0;
                water_extract_key();
                gu16_durable_test_step++;
            }
            else{}

            break;

        case 3:

            if( F_WaterOut == CLEAR )
            {
                gu32_durable_test_timer++;

                if(gu32_durable_test_timer >= AGING_TEST_MODE_DELAY_TIME_10MIN)
                {
                    gu32_durable_test_timer = 0;
                    /* 정수 선택 */
                    key_ambient_water_select_job();
                    gu16_durable_test_step++;
                }
                else{}
            }
            else
            {
                gu32_durable_test_timer = 0;
            }

            break;

        case 4:

            gu32_durable_test_timer++;

            if(gu32_durable_test_timer >= 10)
            {
                gu32_durable_test_timer = 0;
                water_extract_key();

                gu16_durable_test_step++;
            }
            else{}

            break;

        case 5:

            if( F_WaterOut == CLEAR )
            {
                gu32_durable_test_timer++;

                if(gu32_durable_test_timer >= AGING_TEST_MODE_DELAY_TIME_10MIN)
                {
                    gu32_durable_test_timer = 0;
                    /* 온수 선택 */
                    key_hot_water_select_job();
                    gu16_durable_test_step++;
                }
                else{}
            }
            else
            {
                gu32_durable_test_timer = 0;
            }

            break;

        case 6:

            gu32_durable_test_timer++;

            if(gu32_durable_test_timer >= 10)
            {
                gu32_durable_test_timer = 0;
                water_extract_key();

                gu16_durable_test_step++;
            }
            else{}

            break;

        case 7:

            if( F_WaterOut == CLEAR )
            {
                gu32_durable_test_timer++;

                if(gu32_durable_test_timer >= AGING_TEST_MODE_DELAY_TIME_10MIN)
                {
                    gu32_durable_test_timer = 0;
                    /* 정수 선택 */
                    key_ambient_water_select_job();
                    gu16_durable_test_step++;
                }
                else{}
            }
            else
            {
                gu32_durable_test_timer = 0;
            }

            break;

        case 8:

            gu32_durable_test_timer++;

            if(gu32_durable_test_timer >= 10)
            {
                gu32_durable_test_timer = 0;
                water_extract_key();

                gu16_durable_test_step++;
            }
            else{}

            break;

        case 9:

            if( F_WaterOut == CLEAR )
            {
                gu32_durable_test_timer++;

                if(gu32_durable_test_timer >= AGING_TEST_MODE_DELAY_TIME_10MIN)
                {
                    gu32_durable_test_timer = 0;
                    key_hot_water_select_job();
                    gu16_durable_test_step++;
                }
                else{}
            }
            else
            {
                gu32_durable_test_timer = 0;
            }

            break;

        case 10:

            gu32_durable_test_timer++;

            if(gu32_durable_test_timer >= 10)
            {
                gu32_durable_test_timer = 0;
                water_extract_key();

                gu16_durable_test_step++;
            }
            else{}

            break;

        case 11:
            if( F_WaterOut == CLEAR )
            {
                gu32_durable_test_timer++;

                if(gu32_durable_test_timer >= AGING_TEST_MODE_DELAY_TIME_10MIN)
                {
                    gu32_durable_test_timer = 0;
                    /* 만빙이면 얼음 연속추출 */
                    if(F_IceFull == SET)
                    {
                        key_ice_select_job();
                        ice_extraction_start();
                        gu16_durable_test_step++;
                    }
                    else
                    {
                        gu16_durable_test_step = 13;              /* 만빙아니면 얼음물 추출로 이동 */
                    }

                }
                else{}
            }
            else
            {
                gu32_durable_test_timer = 0;
            }

        break;

        case 12:
            gu32_durable_test_timer++;
            if(gu32_durable_test_timer >= 10)
            {
                gu32_durable_test_timer = 0;
                ice_extract_long_key();             /* 얼음 연속추출 */

                gu16_durable_test_step++;
            }
            else{}

        break;

        case 13:
            if(( F_WaterOut == CLEAR )
            && (F_IceOut == CLEAR)
            )
            {
                gu32_durable_test_timer++;

                if(gu32_durable_test_timer >= 10)
                {
                    gu32_durable_test_timer = 0;
                    /* 2kg는 얼음물 추출 */
                    key_ice_water_select_job();
                    gu16_durable_test_step++;
                }
                else{}
            }
            else
            {
                gu32_durable_test_timer = 0;
            }

            break;

        case 14:
            gu32_durable_test_timer++;
            if(gu32_durable_test_timer >= 10)
            {
                gu32_durable_test_timer = 0;
                ice_extraction_start();

                gu16_durable_test_step++;
            }
            else{}

            break;

        case 15:
            if(( F_WaterOut == CLEAR )
            && (F_IceOut == CLEAR)
            )
            {
                gu32_durable_test_timer++;

                if(gu32_durable_test_timer >= 10)
                {
                    gu32_durable_test_timer = 0;
                    gu16_durable_test_step++;
                }
                else {  }
            }
            else
            {
                gu32_durable_test_timer = 0;
            }

            break;

        case 16:
            gu32_durable_test_timer++;

            /*..hui [25-3-26오전 9:45:43] 10분대기후 처음부터..*/
            if(gu32_durable_test_timer >= 6000)
            {
                gu32_durable_test_timer = 0;
                gu16_durable_test_step = 0;
            }
            else{}

            break;

        default:

            gu16_durable_test_step = 0;
            gu32_durable_test_timer = 0;

            break;
    }
    #endif
}





/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/




