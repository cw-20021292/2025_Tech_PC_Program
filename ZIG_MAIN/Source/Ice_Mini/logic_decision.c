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
#include    "logic_decision.h"
#include "WIFI_SetFunctionData.h"
#include "WIFI_A101x_FunctionList.h"

void logic_decision(void);
void water_select_return_decision(void);
void water_select_return_timer_control( U8 mu8_flag );
void cup_select_return_decision(void);
void water_out_disable_time_check(void);
void room_water_init_check(void);
void forced_key_clear_decision(void);
void setting_mode_return_decision(void);
void hot_water_setting_mode_timeout_check(void);
void sound_setting_mode_timeout_check(void);
void ice_select_return_decision(void);
void instruction_mode_timeout_check(void);

TYPE_WORD          U16CupLevelSelectW;
#define            u16CupLevelSelect                          U16CupLevelSelectW.word
#define            u8CupLevelSelect_L                         U16CupLevelSelectW.byte[0]
#define            u8CupLevelSelect_H                         U16CupLevelSelectW.byte[1]
#define            Bit0_4_Oz_Select__120_ml                   U16CupLevelSelectW.Bit.b0
#define            Bit1_6_Oz_Select__175_ml                   U16CupLevelSelectW.Bit.b1
#define            Bit2_8_Oz_Select__235_ml                   U16CupLevelSelectW.Bit.b2
#define            Bit3_10_Oz_Select__295_ml                  U16CupLevelSelectW.Bit.b3
#define            Bit4_12_Oz_Select__355_ml                  U16CupLevelSelectW.Bit.b4
#define            Bit5_14_Oz_Select__415_ml                  U16CupLevelSelectW.Bit.b5
#define            Bit6_16_Oz_Select__475_ml                  U16CupLevelSelectW.Bit.b6
#define            Bit7_20_Oz_Select__590_ml                  U16CupLevelSelectW.Bit.b7
#define            Bit8_32_Oz_Select__945_ml                  U16CupLevelSelectW.Bit.b8
#define            Bit9_Continue_Select__Continue             U16CupLevelSelectW.Bit.b9

U8 gu8_cup_level_order;


U16 gu16_water_select_return_time;

CUP_LEVEL gu8Cup_level;
U8 gu8Cup_level_Default;
U8 gu8Cup_level_Old;
U16 gu16_cup_level_default_return_time;



U8 gu8_pre_water_out_state;
U8 gu8_recovery_water_out_state;
bit F_WaterOut_Change_State;

U8 gu8_waterout_disable_time;
bit F_WaterOut_Disable_State;

U8 gu8_cup_level_time_init;
U8 gu8_hot_select_time_init;



U16 gu16_user_default_return_timer;

U8 gu8_water_state;
U8 gu8_cup_state;

U8 gu8_clock_hide_state;
U8 gu8_clock_hide_timer;

U8 gu8_direction_key_disable;
U8 gu8_direction_key_disable_time;

U16 gu16_setting_finish_timer;
bit bit_setting_mode_time_init;

U8 gu8_altitude_return_time;
U8 gu8_altutude_setting_timeout;


U8 gu8_low_level_delay_timer;

U8 gu8_sound_setting_return_time;

U16 gu16_ice_select_return_time;

U8 gu8_instruction_return_timer;
bit bit_instruction_mode_time_init;

ICE_LEVEL  gu8_ice_amount_step;        /* 얼음 추출량 (단계) : 1 ~ 4단계 */

/**********************************************************************************************************************/
extern bit bit_instruction_mode_start;
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void logic_decision(void)
{
    ice_select_return_decision();

    /*..hui [17-12-19오후 2:03:11] 추출 선택 복귀..*/
    water_select_return_decision();

    /*..hui [17-12-19오후 2:03:31] 정량추출 복귀..*/
    cup_select_return_decision();

    /*..hui [19-8-22오후 5:18:54] 사용자 선택 복귀..*/
    /*user_select_return_decision();*/

    /*..hui [18-3-14오후 3:54:53] 정량 추출 후 500ms 물 추출 disable..*/
    water_out_disable_time_check();

    /*..hui [19-6-26오후 1:07:42] 만수위까지 정수 후 제빙 시작..*/
    //room_water_init_check();

    /*..hui [19-10-24오후 5:25:23] 개발자모드 진입 키 카운트..*/
    forced_key_clear_decision();

    /*..hui [19-11-4오후 8:55:38] 설정 모드에서 왼쪽/오른쪽 버튼 연속 눌름 방지..*/
    /*..hui [19-11-4오후 8:55:50] LCD 성능이 못 따라옴. 화면 밀림현상때문..*/
    /*direction_key_disable_time_check();*/

    /*..hui [23-2-9오후 6:23:30] 20초 동안 버튼 입력 없을경우 자동종료..*/
    setting_mode_return_decision();

    /*..hui [23-2-22오후 12:34:34] 고도모드 10초간 버튼 입력 없으면 자동 종료..*/
    hot_water_setting_mode_timeout_check();

    /*..hui [23-11-23오후 5:52:41] 음성 설정 7초간 아무 입력 없으면 자동 종료..*/
    sound_setting_mode_timeout_check();

    instruction_mode_timeout_check();

#if 1
	if(gu8_clock_hide_state == SET)
	{
        if((F_WaterOut == CLEAR)
        && (F_IceOut == CLEAR)
        )
        {
            gu8_clock_hide_timer++;
            if(gu8_clock_hide_timer > 200)
            {
                gu8_clock_hide_timer = 0;
                gu8_clock_hide_state = CLEAR;
            }
        }
        else
        {
            gu8_clock_hide_timer = 0;
        }
	}
	else
	{
		gu8_clock_hide_timer = 0;
		gu8_clock_hide_state = CLEAR;
	}

#endif
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void ice_select_return_decision(void)
{
    if(u8IceOutState == ICE_SELECT__ICE)
    {
        if( F_IceOut == SET )
        {
            gu16_ice_select_return_time = 0;
        }
        else{}    
    }
    else if(u8IceOutState == ICE_SELECT__ICE_WATER)
    {
        /* 얼음물 추출은 물까지 추출이 완료되어야 선택이 돌아갈 수 있음 */
        if((F_IceOut == SET )
        || (F_WaterOut == SET)
        )
        {
            gu16_ice_select_return_time = 0;
        }
        else{  }
    }

    if( u8IceOutState != ICE_SELECT__NONE )
    {
        gu16_ice_select_return_time++;

        /* 얼음/얼음물 선택중이면 7초 후 얼음 선택으로 변경 */
        if( gu16_ice_select_return_time >= 70 )
        {
            gu16_ice_select_return_time = 0;
            u8IceOutState = ICE_SELECT__NONE;
            Extract_Stack.U8_iceSelect = CLEAR;
        }
        else{}
    }
    else
    {
        gu16_ice_select_return_time = 0;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void water_select_return_decision(void)
{
    /*..hui [19-8-22오후 2:45:46] 온수 선택 중 사용자/물량 버튼 눌릴 경우 복귀 시간 초기화..캐럿 사양..*/
    if(gu8_hot_select_time_init == SET)
    {
        gu8_hot_select_time_init = CLEAR;

        if(u8WaterOutState == HOT_WATER_SELECT)
        {
            water_select_return_timer_control(0);
        }
        else if(u8WaterOutState == COLD_WATER_SELECT && F_Cold_Enable != SET)
        {
            /*..hui [20-3-6오후 5:23:15] 냉수 OFF 상태에서 냉수 선택, 이후 물량/사용자 버튼 누르면 정수로 복귀 초기화..*/
            water_select_return_timer_control(0);
        }
        else{}
    }
    else{}

    /*..hui [18-1-10오전 10:05:31] 선택 버튼 변경이 있으면..*/
    if(gu8_pre_water_out_state != u8WaterOutState)
    {
        /*..hui [18-1-10오전 10:05:50] 냉수 선택 버튼이 선택됐으면..*/
        if(u8WaterOutState == COLD_WATER_SELECT)
        {
            if(F_Cold_Enable != SET)
            {
                /*..hui [18-1-10오전 10:09:06] 냉각모드 OFF시 7초 후 정수로 복귀..*/
                F_WaterOut_Change_State = SET;
            }
            else
            {
                /*..hui [18-1-10오전 10:09:16] 냉각모드 ON시 현재 상태 유지..*/
                F_WaterOut_Change_State = CLEAR;
            }
        }
        else
        {
            /*..hui [18-1-10오전 10:06:48] 온수 버튼이 선택됐으면..*/
            if(u8WaterOutState == HOT_WATER_SELECT)
            {
                F_WaterOut_Change_State = SET;
                /*..hui [18-1-10오전 10:07:23] 온수 사용 후 복귀 할 이전 상태 저장..*/
                gu8_recovery_water_out_state = gu8_pre_water_out_state;
            }
            else
            {
                /*..hui [18-1-10오전 10:07:50] 정수 버튼이 선택됐으면 그냥 현재 상태 유지..*/
                F_WaterOut_Change_State = CLEAR;
            }
        }

        gu8_pre_water_out_state = u8WaterOutState;
        gu16_water_select_return_time = 0;
    }
    else {  }

    /*..hui [18-1-10오전 10:09:40] 사용 후 복귀 조건이면..*/
    if(F_WaterOut_Change_State == SET)
    {
        /*..hui [17-12-14오후 4:53:30] 물 추출중일때는 추출 종료하고 7초..*/
        if(F_WaterOut == SET)
        {
            if(u8WaterOutState == COLD_WATER_SELECT)
            {
                /*..hui [18-1-10오전 10:17:58] 냉각모드 OFF 상태에서 냉수 선택하고 추출하면 추출 종료하고 바로 정수로 복귀..*/
                if(F_Cold_Enable != SET)
                {
                    gu16_water_select_return_time = 69;
                }
                else
                {
                    /*..hui [18-1-10오전 10:28:06] 냉각모드 ON 상태일 경우.. 들어올일 없음....*/
                    gu16_water_select_return_time = 0;
                }
            }
            else
            {
                /*..hui [19-8-22오후 2:14:09] 순간온수이므로 온수OFF일때 조건 삭제..*/
                gu16_water_select_return_time = 0;

            }
        }
        else
        {
            gu16_water_select_return_time++;
            // /* 냉온정 중에 온수로 선택한 후 MY워터 선택 시 
            // 뒤에서 냉온정 선택이 초기화돼서 라면레시피에서 정수나오거나 냉수 나오는 걸 방지
            // (온수 선택 초기화 관련 타이머 제한) 250703 CH.PARK */
            // if(bit_myWater_setting_start == SET)
            // {
            //     if(gu16_water_select_return_time >= 69)
            //     {
            //         gu16_water_select_return_time = 69;
            //     }
            //     else {  }
            // }
            // else {  }
        }

        /*..hui [17-12-14오후 4:31:12] 복귀 시간 7초..*/
        if(gu16_water_select_return_time >= 70)
        {
            gu16_water_select_return_time = 0;
            F_WaterOut_Change_State = CLEAR;

            /*..hui [18-1-10오전 10:10:24] 온수 선택 상태였으면..*/
            if(u8WaterOutState == HOT_WATER_SELECT)
            {
                /*..hui [18-1-10오전 10:10:35] 이전 상태가 냉수 선택 이었으면..*/
                if(gu8_recovery_water_out_state == COLD_WATER_SELECT)
                {
                    if(F_Cold_Enable == SET)
                    {
                        /*..hui [18-1-10오전 10:10:46] 냉각 ON이면 냉수로 변경..*/
                        u8WaterOutState = COLD_WATER_SELECT;
                        gu8_pre_water_out_state = u8WaterOutState;
                    }
                    else
                    {
                        /*..hui [18-1-10오전 10:10:53] 냉각 OFF이면 정수로 변경..*/
                        u8WaterOutState = PURE_WATER_SELECT;
                        gu8_pre_water_out_state = u8WaterOutState;
                    }
                }
                else
                {
                    /*..hui [18-1-10오전 10:11:08] 이전 상태가 정수 선택 이었으면 그냥 정수로 복귀..*/
                    u8WaterOutState = PURE_WATER_SELECT;
                    gu8_pre_water_out_state = u8WaterOutState;
                }
            }
            else
            {
                /*..hui [18-1-10오전 10:11:52] 냉각이 OFF이면 정수로 복귀..*/
                if(F_Cold_Enable != SET)
                {
                    u8WaterOutState = PURE_WATER_SELECT;
                    gu8_pre_water_out_state = u8WaterOutState;

                    /*..hui [20-3-6오후 5:58:36] 사용자도 초기화..*/
                    /*..hui [20-3-6오후 5:58:55] 냉수OFF / 사용자 선택 / 물 추출 완료시 즉시 정수로 변경되고..*/
                    /*..hui [20-3-6오후 5:59:01] 사용자는 그대로 남아있음..*/
                    /*..hui [20-3-6오후 5:59:12] 기본 컨셉은 물 선택이 바뀌면 사용자도 초기화되야..*/
                    //gu8_Select_user = USER_SELECT_NONE;
                }
                else{}
            }

            /*..hui [17-12-19오후 2:00:48] 온수 선택중 온수잠금 설정시 바로 복귀..*/
            if(u8WaterOutState == HOT_WATER_SELECT && F_Hot_Lock == SET)
            {
                gu16_water_select_return_time = 70;
            }
            else{}

        }
        else{}
    }
    else
    {
        /*..hui [18-1-10오전 10:12:41] 냉각이 OFF이면 정수로 복귀..*/
        /*..hui [18-1-10오후 10:08:41] 냉수 선택,온도로 표시하는 에러 발생시 정수로 복귀..*/
        if(F_Cold_Enable != SET)
        {
            u8WaterOutState = PURE_WATER_SELECT;
            gu8_pre_water_out_state = u8WaterOutState;
        }
        else{}
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void water_select_return_timer_control( U8 mu8_flag )
{
    if(mu8_flag == 0)
    {
        if(F_WaterOut_Change_State == SET)
        {
            gu16_water_select_return_time = 0;
        }
        else{}
    }
    else
    {
        if(F_WaterOut_Change_State == SET)
        {
            gu16_water_select_return_time = 70;
        }
        else{}
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void cup_select_return_decision(void)
{
    /*..hui [19-8-22오후 2:23:13] 물량은 냉/온/정/사용자/물량 눌렸을때만 초기화.. 캐럿 참조..*/
    if(gu8_cup_level_time_init == SET)
    {
        /*..hui [19-2-8오후 3:49:45] 같은 키 계속 눌렀을때도 시간 초기화 추가..*/
        gu8_cup_level_time_init = CLEAR;
        gu16_cup_level_default_return_time = 0;
        gu8_cup_led_off_time = 0;
    }
    else{}

    if(gu8Cup_level != gu8Cup_level_Default)
    {
        if( u8IceOutState == ICE_SELECT__ICE_WATER )
        {
            if(F_IceOut == SET)
            {
                gu16_cup_level_default_return_time = 0;
            }
            else
            {
                gu16_cup_level_default_return_time++;
            }
        }
        else
        {
            /*..hui [17-12-19오후 2:07:16] 추출중에는 시간 초기화..*/
            if(F_WaterOut == SET)
            {
                gu16_cup_level_default_return_time = 0;
            }
            else
            {
                gu16_cup_level_default_return_time++;
            }
        }

        if(gu16_cup_level_default_return_time >= CUP_LEVEL_RETURN_TIME)
        {
            gu8Cup_level = gu8Cup_level_Default;
        }
        else{}
        
    }
    else
    {
        gu16_cup_level_default_return_time = 0;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void water_out_disable_time_check(void)
{
    /*..hui [18-3-14오후 12:48:04] 물 추출 시 추출 종료되는 순간부터 500ms 동안 물 추출 입력 받지 않는다..*/
    /*..hui [18-3-14오후 12:48:47] 추출 종료 버튼 눌렀는데 종료되는 순간과 겹쳐서 한번 더 추출되는 현상때문..*/
    /*..hui [18-3-14오후 12:49:16] 품질팀 요청사항.. 별 효과는 없을 것 같다..*/
    if(F_WaterOut_Disable_State == SET)
    {
        gu8_waterout_disable_time++;

        if(gu8_waterout_disable_time >= 5)
        {
            gu8_waterout_disable_time = 5;
            F_WaterOut_Disable_State = CLEAR;
        }
        else
        {
            /*F_WaterOut_Disable_State = SET;*/
        }
    }
    else
    {
        gu8_waterout_disable_time = 0;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void room_water_init_check(void)
{
	
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void forced_key_clear_decision(void)
{
    /*..hui [20-3-13오후 2:48:40] 개발자 모드 키 카운트 초기화..*/
    if(gu8_debug_finish_count > 0)
    {
        gu16_develop_key_clear_time++;

        if(gu16_develop_key_clear_time >= 100)
        {
            gu16_develop_key_clear_time = 0;
            gu8_debug_finish_count = 0;
        }
        else{}
    }
    else
    {
        gu16_develop_key_clear_time = 0;
    }

    /* 고온살균을 위한 [냉수] 키 입력 카운트 초기화 250617 CH.PARK */
    if(gu8_passive_hot_ster_key_input_count > 0)
    {
        gu16_passive_hot_ster_key_clear_timer++;

        if(gu16_passive_hot_ster_key_clear_timer >= 100)
        {
            gu16_passive_hot_ster_key_clear_timer = 0;
            gu8_passive_hot_ster_key_input_count = 0;
        }
        else{}
    }
    else
    {
        gu16_passive_hot_ster_key_clear_timer = 0;
    }


    if( gu8_flushing_finish_input_count > 0 )
    {
        gu16_flushing_finish_key_clear_time++;

        if(gu16_flushing_finish_key_clear_time >= 100)
        {
            gu16_flushing_finish_key_clear_time = 0;
            gu8_flushing_finish_input_count = 0;
        }
        else{}
    }
    else
    {
        gu16_flushing_finish_key_clear_time = 0;
    }
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void setting_mode_return_decision(void)
{
    if(bit_setting_mode_time_init == SET)
    {
        bit_setting_mode_time_init = CLEAR;
        gu16_setting_finish_timer = 0;
    }
    else{}

    if( bit_setting_mode_start == SET )
    {
        /* 설정모드 20초 지속 시 TIMEOUT */
        gu16_setting_finish_timer++;
        if( gu16_setting_finish_timer >= SETTING_MODE_EXIT_TIME )
        {
            gu16_setting_finish_timer = 0;
            bit_setting_mode_start = CLEAR;
        }
        else{}
    }
    else
    {
        gu16_setting_finish_timer = 0;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void hot_water_setting_mode_timeout_check(void)
{
    if( bit_altitude_setting_start == SET )
    {
        /*..hui [23-8-2오전 9:45:42] 필터 열림 감지시 강제 종료..*/
        if( bit_filter_all == CLEAR )
        {
            gu8_altitude_return_time = 0;
            bit_altitude_setting_start = CLEAR;
        }
        else{}

        gu8_altitude_return_time++;

        if( gu8_altitude_return_time >= 100 )
        {
            gu8_altitude_return_time = 0;
            bit_altitude_setting_start = CLEAR;
            play_melody_setting_off_199();
        }
        else{}
    }
    else
    {
        gu8_altitude_return_time = 0;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void sound_setting_mode_timeout_check(void)
{
    if( bit_volume_setting_start == SET )
    {
        /*..hui [23-8-2오전 9:45:42] 필터 열림 감지시 강제 종료..*/
        if( bit_filter_all == CLEAR )
        {
            gu8_sound_setting_return_time = 0;
            bit_volume_setting_start = CLEAR;

            send_wifi_each_data_control( WIFI_FUNC_0031_VOICE_SEL );
        }
        else{}

        gu8_sound_setting_return_time++;

        /*..hui [23-11-23오후 5:54:13] 7초후종료..*/
        if( gu8_sound_setting_return_time >= 70 )
        {
            gu8_sound_setting_return_time = 0;
            bit_volume_setting_start = CLEAR;
            play_melody_setting_off_199();

            send_wifi_each_data_control( WIFI_FUNC_0031_VOICE_SEL );
        }
        else{}
    }
    else
    {
        gu8_sound_setting_return_time = 0;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void instruction_mode_timeout_check(void)
{
    if( bit_instruction_mode_time_init == SET )
    {
        bit_instruction_mode_time_init = CLEAR;
        gu8_instruction_return_timer = 0;
    }
    else{}

    if( bit_instruction_mode_start == SET )
    {
        gu8_instruction_return_timer++;

        if( gu8_instruction_return_timer >= 200 )
        {
            gu8_instruction_return_timer = 0;
            bit_instruction_mode_start = CLEAR;
            play_melody_setting_off_199();
        }
        else{}
    }
    else
    {
        gu8_instruction_return_timer = 0;
    }
}
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/




