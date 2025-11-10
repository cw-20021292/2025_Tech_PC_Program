/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Main.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright  d x  : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#include    "Macrodriver.h"
#include    "Global_Variable.h"
#include    "Port_Define.h"
#include    "auto_drain.h"

void auto_drain_control(void);
void auto_drain_output(void);
void auto_drain_operate_check(void);
void auto_drain_check_timer(void);
void integrate_water_quantity(void);
void UseWater24HoursTotalCount(U8 mu8Type, U16 mu8Data);
void UseWater48HoursTotalCount(U8 mu8Type, U8 mu8Data);

void auto_mode_drain_pump_out(void);
void auto_drain_key_off_check(void);
void normal_mode_auto_drain_check_timer(void);
/*void test_mode_auto_drain_check_timer(void);*/


bit F_night;                      // ??ħ????
U16 gu16_night_timer;

bit F_first_night_check;

U16 gu16_auto_drain_start_timer_sec;
U16 gu16_auto_drain_start_timer_min;
U16 gu16_auto_drain_start_timer_hour;
bit F_Auto_drain_time_expired;


bit F_drain_operate;

U16 gu16NormalEffluentTotal = 0;      // 24?ð? ????(?¼?) ????
//U8 gu8ColdEffluentTotal = 0;        // 24?ð? ?u? ????
U16 gu16ColdEffluentTotal = 0;        // 24?ð? ?u? ????
U16 gu16EffluentTimeCNT = 0;        // ???? ???? ?ð? ?????
bit F_AutoDrainCheckOK;             // ??? ??? u? ??? Flag
bit F_DrainStatus;                  // ??? ??? ???? ???? Flag


U16 gu16_auto_drain_check_period;

U8 gu8AutoDrainStep;

U16 gu16ColdDrainTime;

bit F_Cold_Drain;


bit bit_auto_drain_pump_out;


bit F_auto_drain_drinking_check_before_1hour;



U8 gu8_before_drain_water_level;
U16 gu16_cold_drain_op_timer;



bit F_auto_drain_off_check;


U8 gu8_before_2hour_drinking_count;
bit F_save_count;
bit F_auto_drain_drinking_check_before_2hour;

U8 gu8_auto_drain_no_water_timer;

bit bit_WaterOut_extracted_state;
extern bit bit_ice_tank_ster_start;
extern U16 gu16_extract_display_cnt;
extern bit F_IceOut;

extern TYPE_BYTE          U8IceOutStateB;
#define            u8IceOutState                             U8IceOutStateB.byte
#define            Bit0_Ice_Only_Select_State                U8IceOutStateB.Bit.b0
#define            Bit1_Ice_Plus_Water_Select_State          U8IceOutStateB.Bit.b1

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void auto_drain_control(void)
{
    //
    auto_drain_operate_check();

    auto_drain_output();
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void auto_drain_output(void)
{
    U8 mu8_auto_drain_enable_check;

    if( bit_self_test_start == SET )
    {
        F_DrainStatus = CLEAR;
        F_auto_drain_mode_cold_water_valve_out = CLEAR;
		
        return;
    }
    else{}

    if( ( F_IceOut == SET ) 
    && ( u8IceOutState == ICE_SELECT__ICE_WATER ) )
    {
        F_DrainStatus = CLEAR;
        F_auto_drain_mode_cold_water_valve_out = CLEAR;

        return;
    }
    
    if( bit_ice_tank_ster_start == SET )
    {
        F_DrainStatus = CLEAR;
        F_auto_drain_mode_cold_water_valve_out = CLEAR;

        return;
    }

    mu8_auto_drain_enable_check = (U8)(F_DrainStatus & !Bit2_Ice_Operation_Disable_State);

    if(mu8_auto_drain_enable_check == CLEAR)
    {
        gu16ColdDrainTime = 0;
        bit_auto_drain_pump_out = CLEAR;
        F_auto_drain_mode_cold_water_valve_out = CLEAR;
        gu8_before_drain_water_level = DRAIN_LEVEL_EMPTY;
        gu16_cold_drain_op_timer = 0;
        gu8_auto_drain_no_water_timer = 0;
    }
    else
    {
        /* 2KG MAX는 순환배수 시 수배관 특성상 1KG, MINI와 다르게 
        배수탱크를 반드시 지나가야 하기 때문에 펌프제어 필요 250623 CH.PARK */
        auto_mode_drain_pump_out();
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void auto_mode_drain_pump_out(void)
{
    switch(u8DrainWaterLevel)
    {
        case DRAIN_LEVEL_EMPTY :
             bit_auto_drain_pump_out = CLEAR;
             F_auto_drain_mode_cold_water_valve_out = SET;          // 냉수탱크 드레인

             gu8_before_drain_water_level = DRAIN_LEVEL_EMPTY;
             gu16_cold_drain_op_timer = 0;
             gu8_auto_drain_no_water_timer = 0;
            break;

        case DRAIN_LEVEL_LOW :
            /*..hui [18-1-14오후 6:06:23] 물없는 상태에서 냉수 배수밸브 ON 상태로 저수위 진입하면..*/
            if(gu8_before_drain_water_level == DRAIN_LEVEL_EMPTY)
            {
                gu16_cold_drain_op_timer++;

                /*..hui [18-1-14오후 6:06:47] 40초 이상 냉수 배수했는데도 만수위에 도달하지 못하면..*/
                    /*..hui [18-1-14오후 6:07:00] 다시 드레인펌프 가동해서 드레인 시작..*/
                    /*..hui [18-1-14오후 6:11:06] 센서 오감지, 유로막힘등의 이유로.. 배수탱크 넘치지 않게 하기위해..???..*/
                if(gu16_cold_drain_op_timer >= DRAIN_VALVE_OPEN_TIME)
                {
                    /*gu16_cold_drain_op_timer = DRAIN_VALVE_OPEN_TIME;*/
                    bit_auto_drain_pump_out = SET;
                    F_auto_drain_mode_cold_water_valve_out = CLEAR;

                    /*..hui [18-1-25오후 4:11:59] 드레인펌프 가동으로 전환하고 3초후에 피드백 확인..*/
                    if(gu16_cold_drain_op_timer >= (DRAIN_VALVE_OPEN_TIME + 30))
                    {
                        /*..hui [18-1-25오후 4:12:27] 피드백 이하이면 다시 냉수 밸브 ON..*/
                        if(gu16_AD_Drain_Pump_Current <= DRAIN_COMPLETE_FEEDBACK)
                        {
                            gu16_cold_drain_op_timer = 0;
                        }
                        else
                        {
                            /*..hui [18-1-25오후 4:12:45] 피드백 초과이면 드레인펌프 ON 상태 유지..*/
                            gu16_cold_drain_op_timer = DRAIN_VALVE_OPEN_TIME + 30;
                        }
                    }
                    else{}
                }
                else
                {
                    bit_auto_drain_pump_out = CLEAR;
                    F_auto_drain_mode_cold_water_valve_out = SET;
                }
                }
                else
                {
                gu16_cold_drain_op_timer = 0;
                /*..hui [18-1-14오후 6:15:33] 만수위 감지되고 드레인펌프 가동하여 저수위까지 내려왔을때..*/
                    /*..hui [18-1-14오후 6:21:04] 이것도 센서 오감지나 유로 에어등의 이유로 물없음까지 잘 안내려갈때..*/
                    /*..hui [18-1-14오후 6:21:55] 다시 드레인펌프 OFF하고 냉수 밸브 ON하여 만수위로 가도록....*/
                if(gu16_AD_Drain_Pump_Current <= DRAIN_COMPLETE_FEEDBACK)
                {
                    gu8_auto_drain_no_water_timer++;

                    if( gu8_auto_drain_no_water_timer >= 100 )
                    {
                        gu8_auto_drain_no_water_timer = 100;
                        bit_auto_drain_pump_out = CLEAR;
                        F_auto_drain_mode_cold_water_valve_out = SET;
                    }
                    else
                    {
                        bit_auto_drain_pump_out = SET;
                        F_auto_drain_mode_cold_water_valve_out = CLEAR;
                    }
                }
                else
                {
                    gu8_auto_drain_no_water_timer = 0;
                    bit_auto_drain_pump_out = SET;
                    F_auto_drain_mode_cold_water_valve_out = CLEAR;
                }
            }
            break;

        case DRAIN_LEVEL_ERROR :
        case DRAIN_LEVEL_HIGH :
            bit_auto_drain_pump_out = SET;
            F_auto_drain_mode_cold_water_valve_out = CLEAR;
            gu8_before_drain_water_level = DRAIN_LEVEL_HIGH;
            gu8_auto_drain_no_water_timer = 0;
            break;

        //=====================================================
        default :
            bit_auto_drain_pump_out = CLEAR;
            F_auto_drain_mode_cold_water_valve_out = CLEAR;
            gu8_before_drain_water_level = DRAIN_LEVEL_EMPTY;
            break;
      }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void auto_drain_operate_check(void)
{
    auto_drain_check_timer();
    integrate_water_quantity();

    F_drain_operate = F_Auto_drain_time_expired;
    if(F_drain_operate == SET)
    {
        gu16_auto_drain_check_period++;
        
        /* 순환배수 동작 시간 : 2분 40초 */
        if(gu16_auto_drain_check_period >= AUTO_DRAIN_OPERATION_TIMER)
        {
            gu16_auto_drain_check_period = 0;
       		
            /*..sean [25-06-13] 10분.. 아이콘 아이스 기준 정수 사용량 체크 안함..*/
            gu16NormalEffluentTotal = 0;
            gu16ColdEffluentTotal = 0;
            F_Auto_drain_time_expired = CLEAR;
            F_DrainStatus = CLEAR;
        }
        else
        {
            /* 용량 500ml 기준 (확인 필요) 250703 CH.PARK */
            if (gu16ColdEffluentTotal < AUTO_DRAIN_OPERATION_WATER)
            {
                F_DrainStatus = SET;
            }
            else
            {
                gu16NormalEffluentTotal = 0;
                gu16ColdEffluentTotal = 0;
                F_Auto_drain_time_expired = CLEAR;
                F_DrainStatus = CLEAR;
            }
        }
    }
    else
    {
        F_AutoDrainCheckOK = CLEAR;
        gu16_auto_drain_check_period = 0;
    }

    /*..hui [18-1-14???? 7:35:24] ???? ????? ?????? ????..*/
    if( (Bit2_Ice_Operation_Disable_State == SET)           // 제빙 관련 에러
    || (F_Circul_Drain != SET)                              // 순환배수 사용
    || (F_auto_drain_off_check == SET)                      // 순환배수 동작 취소 플래그
    || (F_WaterOut == SET)                                  // 물 추출 시
    || (bit_filter_all == CLEAR)                            // 필터 커버/리드 열림
    )
    {
        F_Auto_drain_time_expired = CLEAR;
        F_auto_drain_off_check = CLEAR;
        F_DrainStatus = CLEAR;
    }
    else{}
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void auto_drain_key_off_check(void)
{
    if(F_DrainStatus == SET)
    {
        F_auto_drain_off_check = SET;
    }
    else{}
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void auto_drain_check_timer(void)
{

    normal_mode_auto_drain_check_timer();
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void normal_mode_auto_drain_check_timer(void)
{
    if(F_Circul_Drain != SET)
    {
        /*..hui [18-1-14???? 8:43:52] ?????? ???? ?? ??? OFF ?? ????..*/
        F_Auto_drain_time_expired = CLEAR;
    }
    else{}

    /*..hui [18-1-14???? 8:53:23] ?????? ????? OFF???? ?ð? ???? ?? ???? ????? ??? ????? ????....*/
    /*..hui [18-1-14???? 8:53:41] ????? ????? ??? ON??? ????ð????? ????? ???????..*/
    gu16_auto_drain_start_timer_sec++;

    if(gu16_auto_drain_start_timer_sec >= 600)
    {
        gu16_auto_drain_start_timer_sec = 0;
        gu16_auto_drain_start_timer_min++;

    }
    else{}

    if(gu16_auto_drain_start_timer_min >= 60)
    {
        gu16_auto_drain_start_timer_min = 0;
        gu16_auto_drain_start_timer_hour++;
    }
    else{}

    /*..hui [18-1-29???? 9:23:04] 24?ð? ??? 2?ð? ???? ?? ??????? ???..*/
    /*if(gu16_auto_drain_start_timer_hour >= 22)*/
    /*..hui [23-7-31???? 2:58:50] 72?ð???? 2?ð? ??..*/
    /*if(gu16_auto_drain_start_timer_hour >= 70)*/
    if(gu16_auto_drain_start_timer_hour >= 22)
    {
        if(F_WaterOut == SET)
        {
            F_auto_drain_drinking_check_before_2hour = SET;
        }
        else{}
    }
    else{}

    /*..hui [18-1-14???? 7:02:24] 24?ð???? 1?ð? ???? ?? ??????? ???..*/
    /*if(gu16_auto_drain_start_timer_hour >= 23)*/
    /*..hui [23-7-31???? 2:59:10] 72?ð???? 1?ð? ??..*/
    /*if(gu16_auto_drain_start_timer_hour >= 71)*/
    if(gu16_auto_drain_start_timer_hour >= 23)
    {
        if(F_WaterOut == SET)
        {
            F_auto_drain_drinking_check_before_1hour = SET;
        }
        else{}
    }
    else{}

    /*if(gu16_auto_drain_start_timer_hour >= 24)*/
    /*..hui [23-7-31???? 2:59:28] 72?ð? ??? ???..*/
    /*if(gu16_auto_drain_start_timer_hour >= 72)*/
    if(gu16_auto_drain_start_timer_hour >= 24)
    {
        /*..hui [18-1-14???? 7:19:19] 24?ð???? ??ð? ???? ???? ???????..*/
        /*..hui [18-1-14???? 7:19:30] ??ð? ?? ?Ŀ? ???? u?..*/
        if(F_auto_drain_drinking_check_before_1hour == SET)
        {
            gu8_before_2hour_drinking_count = 0;                // 순환배수 동작시점 1시간 증가
        }
        else
        {
            gu16_auto_drain_start_timer_hour = 0;
            F_Auto_drain_time_expired = SET;

            gu16_auto_drain_start_timer_sec = 0;
            gu16_auto_drain_start_timer_min = 0;
            gu16_auto_drain_start_timer_hour = 0;
            F_auto_drain_drinking_check_before_1hour = CLEAR;

            /*..hui [18-1-29???? 9:47:42] 2?ð? ???? ?? ?????? ??????..*/
            if(F_auto_drain_drinking_check_before_2hour == CLEAR)
            {
                gu8_before_2hour_drinking_count++;

                /*..hui [18-1-29???? 9:53:21] 3?? ???? 2?ð? ?????? ?? ?????? ?????? ??u ?ð?  1?ð? ????..*/
                if(gu8_before_2hour_drinking_count >= 7)
                {
                    gu8_before_2hour_drinking_count = 0;
                    gu16_auto_drain_start_timer_hour = 1;
                }
                else{}
            }
            else
            {
                gu8_before_2hour_drinking_count = 0;
                F_auto_drain_drinking_check_before_2hour = CLEAR;
            }
        }
    }
    else{}

    /*if(gu16_auto_drain_start_timer_hour >= 25)*/
    /*..hui [23-7-31???? 3:00:57] 73?ð? 1?ð? ???? ???..*/
    /*if(gu16_auto_drain_start_timer_hour >= 73)*/
    if(gu16_auto_drain_start_timer_hour >= 25)
    {
        gu16_auto_drain_start_timer_hour = 0;
        F_Auto_drain_time_expired = SET;

        gu16_auto_drain_start_timer_sec = 0;
        gu16_auto_drain_start_timer_min = 0;
        gu16_auto_drain_start_timer_hour = 0;
        F_auto_drain_drinking_check_before_1hour = CLEAR;
        F_auto_drain_drinking_check_before_2hour = CLEAR;

    }
    else{}
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void integrate_water_quantity(void)
{
    U8 mu8_minimum_time;
    U16 mu16_amount = 0;

	
    mu16_amount = (U16)((U16)gu16_extract_display_cnt * 10);



    if(F_WaterOut == SET)
    {
		bit_WaterOut_extracted_state = SET;
    }
    else
    {
    	if(bit_WaterOut_extracted_state == SET)
        // 24?ð? ?? ??? ????
    	{
	        /*UseWater48HoursTotalCount(u8WaterOutState, gu8Cup_level);*/
	        UseWater24HoursTotalCount(u8WaterOutState, mu16_amount);
			bit_WaterOut_extracted_state = CLEAR;
        }
    }

    /*..hui [18-1-14???? 8:47:29] ??? ??? ??? OFF?? ???? ????..*/
    /*..hui [18-1-14???? 8:56:16] ??? ??? ??? OFF???? ?ð? ???? ????? ????????? ????..*/
    if(F_Circul_Drain != SET)
    {
        gu16EffluentTimeCNT = 0;
        gu16NormalEffluentTotal = 0;
        gu16ColdEffluentTotal = 0;
    }
    else{}
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/

/// @brief    ?????? ????? ???? 24?ð? ?? ??? ???? ???(????? ????)
/// @param    mu8Type     : ???? ??????
///           mu8Data     : ????
/// @return   void
void UseWater24HoursTotalCount(U8 mu8Type, U16 mu8Data)
{
    U16 mu16_w = 0;
    /*..sean [25-06-13] 10??.. ?????? ????? ???? ???? ??? u? ????..*/

    if (gu16ColdEffluentTotal >= AUTO_DRAIN_OPERATION_WATER)
    {
        gu16ColdEffluentTotal = AUTO_DRAIN_OPERATION_WATER;
    }
    else
    {
    	if ((mu8Type == COLD_WATER_SELECT))
    	{
        	gu16ColdEffluentTotal = gu16ColdEffluentTotal + mu8Data;
    	}
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/

void UseWater48HoursTotalCount(U8 mu8Type, U8 mu8Data)
{
    U16 mu16_w = 0;

    /*..hui [23-7-31???? 3:11:01] ?y??? ???????? ???.. ???????? ?u? 1.5L?? ????????..*/
    /*..hui [23-7-31???? 3:11:21] 120cc ???? 12??..*/
    if (gu16NormalEffluentTotal >= AUTO_DRAIN_OPERATION_WATER)
    {
        gu16NormalEffluentTotal = AUTO_DRAIN_OPERATION_WATER;
    }
    else
    {
        /*..hui [23-7-31???? 5:18:07] ?????? 120??? ???????? ?ð????? ??????..*/
        /*if(u8Extract_Continue == SET)*/
        if(mu8Data != 255)
        {
            mu16_w = mu8Data * 120;
        }
        else
        {
        }

        gu16NormalEffluentTotal = gu16NormalEffluentTotal + mu16_w;
    }



}





