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
#include    "error_leakage.h"

void check_flow_error(void);
void check_error_leakage_sensor(void);
void check_error_blocked_trayin(void);
void check_error_blocked_water_flow(void);
U8 flow_block_condition(void);

void check_error_leaked_water_flow(void);
U8 flow_leak_condition(void);

TYPE_BYTE          U8FlowBlockCheckStateB;
#define            u8FlowBlockCheckState                       U8FlowBlockCheckStateB.byte
#define            Bit0_Flow_Block_Check_State1                U8FlowBlockCheckStateB.Bit.b0
#define            Bit1_Flow_Block_Check_State2                U8FlowBlockCheckStateB.Bit.b1
#define            Bit2_Flow_Block_Check_State3                U8FlowBlockCheckStateB.Bit.b2
#define            Bit3_Flow_Block_Check_State4                U8FlowBlockCheckStateB.Bit.b3
#define            Bit4_Flow_Block_Check_State5                U8FlowBlockCheckStateB.Bit.b4
#define            Bit5_Flow_Block_Check_State6                U8FlowBlockCheckStateB.Bit.b5
#define            Bit6_Flow_Block_Check_State7                U8FlowBlockCheckStateB.Bit.b6
#define            Bit7_Flow_Block_Check_State8                U8FlowBlockCheckStateB.Bit.b7

TYPE_BYTE          U8FlowLeakCheckStateB;
#define            u8FlowLeakCheckState                        U8FlowLeakCheckStateB.byte
#define            Bit0_Flow_Leak_Check_State1                 U8FlowLeakCheckStateB.Bit.b0
#define            Bit1_Flow_Leak_Check_State2                 U8FlowLeakCheckStateB.Bit.b1
#define            Bit2_Flow_Leak_Check_State3                 U8FlowLeakCheckStateB.Bit.b2
#define            Bit3_Flow_Leak_Check_State4                 U8FlowLeakCheckStateB.Bit.b3
#define            Bit4_Flow_Leak_Check_State5                 U8FlowLeakCheckStateB.Bit.b4
#define            Bit5_Flow_Leak_Check_State6                 U8FlowLeakCheckStateB.Bit.b5
#define            Bit6_Flow_Leak_Check_State7                 U8FlowLeakCheckStateB.Bit.b6
#define            Bit7_Flow_Leak_Check_State8                 U8FlowLeakCheckStateB.Bit.b7

U16 gu16_LeakageCheckCount;
U16 gu16_LeakageCheckCount2;


U16 u16_tray_water_in_timer;
U16 u16_tray_flow_check_timer;

bit bit_tray_in_error_temporary;
U16 gu16_tray_error_clear_timer;

U8 gu8_tray_wrong_flow_retry_cnt;
U8 gu8_tray_normal_flow_retry_cnt;

U16 gu16_flow_block_check_timer;
U16 gu16_flow_block_clear_timer;

U8 gu8_clear_block_error;

U16 gu16_flow_leak_error_clear_timer;
U16 gu16_flow_leak_error_check_timer;

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void check_flow_error(void)
{
    /*..hui [18-11-22오후 3:36:39] 누수 센서 에러..*/
    check_error_leakage_sensor();

    /* 트레이 입수 단수에러 */
    check_error_blocked_trayin();

    /*..hui [23-6-9오후 2:43:47] 단수에러..*/
    check_error_blocked_water_flow();

    /*..hui [18-11-22오후 3:36:46] 유량센서 누수 에러..*/
    // check_error_leaked_water_flow();

}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void check_error_leakage_sensor(void)
{
    if(gu16ADLeakage >= LEAKAGE_DETECT_AD)
    {
        gu16_LeakageCheckCount2 = 0;

        if (gu16_LeakageCheckCount < LEAKAGE_ERROR_DETECT_TIME)
        {
            gu16_LeakageCheckCount++;
        }
        else
        {
            if(Bit3_Leakage_Sensor_Error__E01 != SET)
            {
                Bit3_Leakage_Sensor_Error__E01 = SET;
                play_voice_leakage_error_12();
            }
            else{}
        }
    }
    else
    {
        gu16_LeakageCheckCount = 0;

        if(gu16_LeakageCheckCount2 < LEAKAGE_ERROR_CLEAR_TIME)
        {
            gu16_LeakageCheckCount2++;
        }
        else
        {
            Bit3_Leakage_Sensor_Error__E01 = CLEAR;
        }
    }

}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void check_error_leaked_water_flow(void)
{
    U8 u8_check_enable = CLEAR;
    U8 u8_block_error_fix = CLEAR;

    u8_check_enable = flow_leak_condition();

    /*if( (u16ValveOutputState & 0x7FD) == (U16)0 )*/
    /*..hui [19-2-14오후 9:22:33] 감지될수있는 조건으로 변경..*/
    if(u8_check_enable == 0)
    {
        if( gu8_Flow_1sec >= LEAKAGE_DETECT_HZ )
        {
            gu16_flow_leak_error_clear_timer = 0;
            gu16_flow_leak_error_check_timer++;

            if( gu16_flow_leak_error_check_timer >= FLOW_LEAKAGE_ERROR_CHECK_TIME )
            {
                gu16_flow_leak_error_check_timer = FLOW_LEAKAGE_ERROR_CHECK_TIME;

                if( Bit1_Main_Water_Flow_Leaked_Error__E02 != SET )
                {
                    Bit1_Main_Water_Flow_Leaked_Error__E02 = SET;

                    play_voice_leakage_error_12();
                }
                else{}
            }
            else{}
        }
        else
        {
            gu16_flow_leak_error_check_timer = 0;
            gu16_flow_leak_error_clear_timer++;

            if( gu16_flow_leak_error_clear_timer >= FLOW_LEAKAGE_ERROR_CLEAR_TIME )
            {
                gu16_flow_leak_error_clear_timer = FLOW_LEAKAGE_ERROR_CLEAR_TIME;
                Bit1_Main_Water_Flow_Leaked_Error__E02 = CLEAR;
            }
            else{}
        }
    }
    else
    {
        gu16_flow_leak_error_check_timer = 0;
        gu16_flow_leak_error_clear_timer = 0;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 flow_leak_condition(void)
{
    U8 mu8_return = 0;

    /*..hui [19-2-14오후 4:18:50] 물 추출시..*/
    if( F_WaterOut == SET)
    {
        Bit0_Flow_Leak_Check_State1 = SET;
    }
    else
    {
        Bit0_Flow_Leak_Check_State1 = CLEAR;
    }

    if( (bit_hot_filling_start == SET)
    || (bit_fill_cold_start == SET)
    )
    {
        Bit1_Flow_Leak_Check_State2 = SET;
    }
    else
    {
        Bit1_Flow_Leak_Check_State2 = CLEAR;
    }

    if( pVALVE_COLD_IN == SET
    || pVALVE_HOT_IN == SET
    || pVALVE_ROOM_COLD_EXTRACT == SET
    || pVALVE_HOT_OUT  == SET )
    {
        Bit2_Flow_Leak_Check_State3 = SET;
    }
    else
    {
        Bit2_Flow_Leak_Check_State3 = CLEAR;
    }

    Bit3_Flow_Leak_Check_State4 = CLEAR;
    Bit4_Flow_Leak_Check_State5 = CLEAR;
    Bit5_Flow_Leak_Check_State6 = CLEAR;
    Bit6_Flow_Leak_Check_State7 = CLEAR;
    Bit7_Flow_Leak_Check_State8 = CLEAR;

    if(u8FlowLeakCheckState > 0)
    {
        mu8_return = SET;
    }
    else
    {
        mu8_return = CLEAR;
    }

    return mu8_return;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void check_error_blocked_trayin(void)
{
    U16 mu16_clear_time = 0;

    if( gu8IceStep == STATE_20_WATER_IN_ICE_TRAY )
    {
        if( bit_tray_valve_output == SET )
        {
            if( gu8_Flow_1sec <= HOT_BLOCKED_FLOW_HZ )
            {
                u16_tray_flow_check_timer++;
                if(u16_tray_flow_check_timer >= BLOCKED_TRAY_IN_CHECK_TIME)
                {
                    u16_tray_flow_check_timer = 0;
                    bit_tray_in_error_temporary = SET;
                }
                else{}
            }
            else
            {
                u16_tray_flow_check_timer = 0;
            }
        }
        else
        {
            if( F_WaterOut == SET )
            {
                /*..hui [20-6-22오전 10:21:02] 추출할때 카운트 초기화하도록 수정.. 반복적인 추출일때 해제동작이 제대로 안이루어질수있음..*/
                /*..hui [20-6-22오전 10:21:06] 이승헌과장..*/
                u16_tray_flow_check_timer = 0;
            }
            else{  }
        }
    }
    else
    {
        u16_tray_flow_check_timer = 0;
    }

    if( bit_tray_in_error_temporary == SET )
    {
        if( gu8IceStep == STATE_0_STANDBY )
        {
            gu16_tray_error_clear_timer++;

            if( gu16_tray_error_clear_timer >= BLOCKED_TRAY_CLEAR_TIME )
            {
                gu16_tray_error_clear_timer = 0;
                bit_tray_in_error_temporary = CLEAR;
            }
            else{}
        }
        else
        {
            gu16_tray_error_clear_timer = 0;
        }
    }
    else
    {
        gu16_tray_error_clear_timer = 0;
    }
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void check_error_blocked_water_flow(void)
{
    U8 u8_check_enable = CLEAR;
    U8 u8_block_error_fix = CLEAR;
    U16 mu16_time = 0;
    U8 mu8_hz = 0;

    u8_check_enable = flow_block_condition();

    if( u8_check_enable == SET )
    {
        if( Bit6_Main_Water_Flow_Block_Error__E09 != SET )
        {
            if( gu8_Flow_1sec < FILTER_FLOW_BLOCK_DETECT_HZ_0_1_LPM )
            {
                gu16_flow_block_check_timer++;

                if(gu16_flow_block_check_timer >= FLOW_BLOCK_ERROR_CHECK_TIME)      /* 단수 20초 감지 */
                {
                    gu16_flow_block_check_timer = 0;
                    Bit6_Main_Water_Flow_Block_Error__E09 = SET;
                    play_voice_flow_block_error_11();
                }
                else {  }
            }
            else
            {
                gu16_flow_block_check_timer = 0;
            }
        }
        else{}
    }
    else
    {
        // gu16_flow_block_check_timer = 0;
    }
    if(gu8_clear_block_error == SET)
    {
        gu8_clear_block_error = CLEAR;
        gu16_flow_block_clear_timer = FLOW_BLOCK_ERROR_CLEAR_TIME;
    }
    else{}

    if( Bit6_Main_Water_Flow_Block_Error__E09 == SET )
    {
        gu16_flow_block_clear_timer++;

        if(gu16_flow_block_clear_timer >= FLOW_BLOCK_ERROR_CLEAR_TIME)
        {
            gu16_flow_block_clear_timer = 0;
            Bit6_Main_Water_Flow_Block_Error__E09 = CLEAR;
        }
        else{}
    }
    else
    {
        gu16_flow_block_clear_timer = 0;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 flow_block_condition(void)
{
    U8 mu8_return = 0;

    /*..hui [19-2-14오후 4:18:50] 물 추출시..*/
    if( F_WaterOut == SET
    && u8WaterOutState != HOT_WATER_SELECT)
    {
        Bit0_Flow_Block_Check_State1 = SET;
    }
    else
    {
        Bit0_Flow_Block_Check_State1 = CLEAR;
    }

    if( gu8_flushing_mode == FLUSHING_FILTER
    && gu8_filter_flushing_step == 3 )
    {
        Bit1_Flow_Block_Check_State2 = SET;
    }
    else
    {
        Bit1_Flow_Block_Check_State2 = CLEAR;
    }

    if( gu8_flushing_mode == FLUSHING_COLD_TANK
    && gu8_cold_tank_flushing_step == COLDTANK_FLUSHING_5MINUTE )
    {
        Bit2_Flow_Block_Check_State3 = SET;
    }
    else
    {
        Bit2_Flow_Block_Check_State3 = CLEAR;
    }

    if( gu8_flushing_mode == FLUSHING_FILL_COLD_TANK_START_STATE
    && gu8_cold_tank_fill_step == COLD_TANK_FILL_COLD_OPEN )
    {
        Bit3_Flow_Block_Check_State4 = SET;
    }
    else
    {
        Bit3_Flow_Block_Check_State4 = CLEAR;
    }

    // /* 얼음 제빙을 위한 트레이 입수단계에서 원수 단수 체크 250423 CH.PARK */
    // if(gu8IceStep == STATE_20_WATER_IN_ICE_TRAY)
    // {
    //     Bit4_Flow_Block_Check_State5 = SET;
    // }
    // else
    // {
    //     Bit4_Flow_Block_Check_State5 = CLEAR;
    // }

    if(cody_water_line.gu8_start == SET)
    {
        if((cody_water_line.gu8_step == 0)
        || (cody_water_line.gu8_step == 2)
        || (cody_water_line.gu8_step == 4)
        )
        {
            Bit5_Flow_Block_Check_State6 = SET;
        }
        else
        {
            Bit5_Flow_Block_Check_State6 = CLEAR;
        }
    }
    else
    {
        /* 유로살균모드 해제 후 계속 E09 에러 발생되는 현상 개선 250724 CH.PARK */
        Bit5_Flow_Block_Check_State6 = CLEAR;
    }



    /*..hui [24-12-17오전 10:55:19] 나중에 다시..*/
    if( u8FlowBlockCheckState > 0 )
    {
        /*..hui [19-2-14오후 6:26:57] 메인 NOS 밸브가 잠궈져있지 않을때만.. 혹시나해서..*/
        /*..hui [23-8-23오후 2:10:37] FEED 열려있을때 추가..*/
        if( pVALVE_NOS != SET )
        {
            mu8_return = SET;
        }
        else
        {
            mu8_return = CLEAR;
        }
    }
    else
    {
        mu8_return = CLEAR;
    }

    return mu8_return;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/




