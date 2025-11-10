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
#include    "hot_water_filling.h"

void hot_water_filling_control(void);
void start_hot_water_filling(void);
void operation_hot_water_filling(void);


/***********************************************************************************************************************/
TYPE_BYTE          U8HotFillingStateB;
#define            u8HotFillingState                              U8HotFillingStateB.byte
#define            Bit0_Hot_Filling_Room_Full_State               U8HotFillingStateB.Bit.b0
#define            Bit1_Hot_Filling_Error_State                   U8HotFillingStateB.Bit.b1
#define            Bit2_Hot_Filling_Water_Out_State               U8HotFillingStateB.Bit.b2
#define            Bit3_Hot_Filling_First_Op_State                U8HotFillingStateB.Bit.b3
#define            Bit4_Hot_Filling_Delay_State                   U8HotFillingStateB.Bit.b4
#define            Bit5_Hot_Filling_Flushing_State                U8HotFillingStateB.Bit.b5
#define            Bit6_Hot_Filling_Filter_Open_State             U8HotFillingStateB.Bit.b6
#define            Bit7_Hot_Filling_Manual_State                  U8HotFillingStateB.Bit.b7

extern TYPE_WORD          U16HotInFeed9ValveONB;
#define            u16HotInFeed9ValveONs                        U16HotInFeed9ValveONB.word
#define            u8HotInFeed9ValveONs_L                       U16HotInFeed9ValveONB.byte[0]
#define            u8HotInFeed9ValveONs_H                       U16HotInFeed9ValveONB.byte[1]
#define            Bit0_HIF9_Water_Filling_State                U16HotInFeed9ValveONB.Bit.b0
#define            Bit1_HIF9_Hot_Out_State                      U16HotInFeed9ValveONB.Bit.b1
#define            Bit2_HIF9_Ice_Tray_Ster_State                U16HotInFeed9ValveONB.Bit.b2
/***********************************************************************************************************************/
bit F_First_Hot_Effluent;
U8 gu8_Hot_Filling_Step;
U16 gu16_hot_filling_max_timer;
U8 gu8_flow_motor_check_step;

bit bit_hot_filling_start;
bit bit_hot_filling_manual;

U16 gu16_hot_filling_control_timer;
bit F_Hot_Filling_Finish;

U16 gu16_Filling_Hz;

bit F_ColdTank_Filling_Finish;

U16 gu16_Flushing_ColdTank_Hz;
U16 gu16_first_delay_timer;


U8 gu8_hot_filling_max_hz;
U8 gu8_hot_filling_restart_timer;

extern U8 gu8_display_flushing_total_percent;
extern U8 gu8_filter_flushing_state;
extern U8 gu8_flushing_operation_timer_min;
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void hot_water_filling_control(void)
{
    /*..hui [20-3-6오전 9:42:05] 모델 선택 완료되기 전까지 리턴..*/
    if( F_FW_Version_Display_Mode == CLEAR )
    {
        return;
    }
    else{}

	//flow 쪽에 추가 후 수정 예정 ****
	if(1)
	//if( bit_first_full_water == SET )
    {
        Bit0_Hot_Filling_Room_Full_State = SET;
    }
    else
    {
        Bit0_Hot_Filling_Room_Full_State = CLEAR;
    }

    Bit1_Hot_Filling_Error_State = ~Bit1_Hot_Operation_Disable_State;

    Bit2_Hot_Filling_Water_Out_State = SET;

    Bit3_Hot_Filling_First_Op_State = SET;

    gu16_first_delay_timer++;

    if( gu16_first_delay_timer >= HOT_FILLING_START_DELAY_TIME )
    {
        gu16_first_delay_timer = HOT_FILLING_START_DELAY_TIME;
        Bit4_Hot_Filling_Delay_State = SET;
    }
    else
    {
        Bit4_Hot_Filling_Delay_State = CLEAR;
    }

	// flushing 삽입 후 변경 예정  ****
    if( gu8_flushing_mode == FLUSHING_FILL_HOT_TANK_STATE )
    {
        Bit5_Hot_Filling_Flushing_State = SET;//~bit_install_flushing_state;
    }
    else
    {
        Bit5_Hot_Filling_Flushing_State = CLEAR;
    }

    /*Bit6_Hot_Filling_Filter_Open_State = bit_filter_reed;*/

    if( bit_filter_all == SET )
    {
        gu8_hot_filling_restart_timer++;

        if( gu8_hot_filling_restart_timer >= 10 )
        {
            gu8_hot_filling_restart_timer = 10;
            Bit6_Hot_Filling_Filter_Open_State = SET;
        }
        else{}
    }
    else
    {
        gu8_hot_filling_restart_timer = 0;
        Bit6_Hot_Filling_Filter_Open_State = CLEAR;
    }

    // if((bit_hot_filling_manual == CLEAR)
    // && (bit_self_test_start == CLEAR)
    // )
    // {
    //     Bit7_Hot_Filling_Manual_State = SET;
    // }
    // else
    // {
    //     Bit7_Hot_Filling_Manual_State = CLEAR;
    // }

    if( bit_flushing_halt == CLEAR )
    {
        Bit7_Hot_Filling_Manual_State = SET;
    }
    else
    {
        Bit7_Hot_Filling_Manual_State = CLEAR;
    }

    /***********************************************************************************************************************/
    /*..hui [21-6-28오후 3:56:56] 전부 다 SET 일 경우..*/
    if( u8HotFillingState == HOT_FILLING_START_STATE )
    {
        start_hot_water_filling();
    }
    else{}

    if( bit_hot_filling_start == SET )
    {
        operation_hot_water_filling();
    }
    else
    {
        gu8_Hot_Filling_Step = 0;
    }
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void start_hot_water_filling(void)
{
    if( Bit1_Hot_Filling_Error_State == SET
    && Bit2_Hot_Filling_Water_Out_State == SET
    && Bit6_Hot_Filling_Filter_Open_State == SET )
    {
        if( bit_hot_filling_start == CLEAR )
        {
            bit_hot_filling_start = SET;
            Bit0_HIF9_Water_Filling_State = SET;
            /*..hui [20-3-6오전 9:22:33] 온수 추출과 헷갈리지 않게 분리..*/
			// flushing 삽입 후 변경 예정  ****
            // gu16_Filling_Hz = C_CUP_120CC_ROOM; //100cc
            // gu16_Filling_Hz = C_CUP_120CC_ROOM; //100cc
            gu16_Filling_Hz = C_CUP_500CC_ROOM;      //2025-07-10 cbr 1kg 최종사양

            /*Bit1_Prepare_Hot_Water_Popup_State = SET;*/
			// flushing 삽입 후 변경 예정  ****
            //gu8_flushing_mode = FLUSHING_FILL_HOT_TANK_STATE;

        }
        else{}
    }
    else
    {

    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void operation_hot_water_filling(void)
{
    /*..hui [19-11-7오전 9:21:24] 초기 온수 드레인중에도 저수위 감지되면 즉시 정지..*/
    /*..hui [20-3-17오전 11:24:32] 온수 관련 에러 발생시에도 정지 추가..*/
    if( Bit1_Hot_Operation_Disable_State == SET )
        /*|| gu8_Tank_Water_Level == ROOM_LEVEL_ERROR )*/
    {
        bit_hot_filling_start = CLEAR;
        gu8_Hot_Filling_Step = 0;
        gu16_hot_filling_max_timer = 0;

        gu8_flushing_mode_saved = gu8_flushing_mode;
        gu8_flushing_mode = FLUSHING_STANDBY_STATE;
        bit_flushing_halt = SET;

        /*..hui [20-3-17오후 1:40:03] 드레인중 온수 추출 불가 에러 발생시에만 수동드레인으로 전환..*/
        /*..hui [20-3-17오후 1:40:13] 저수위 발생시에는 다시 만수위까지 채우고 자동으로 시작..*/
        //if( Bit1_Hot_Operation_Disable_State == SET )
        //{
            bit_hot_filling_manual = SET;
        //}
        //else{}

        return;
    }
    else{}

    /*if( bit_filter_reed == CLEAR )*/
    /*..hui [21-11-10오후 4:34:16] 고장진단시 정지 추가..*/
    if( bit_filter_all == CLEAR
    || bit_self_test_start == SET )
    {
        bit_hot_filling_start = CLEAR;
        gu8_Hot_Filling_Step = 0;
        gu16_hot_filling_max_timer = 0;

		gu8_flushing_mode_saved = gu8_flushing_mode;
        gu8_flushing_mode = FLUSHING_STANDBY_STATE;
        bit_flushing_halt = SET;
        return;
    }
    else{}

    if(gu8_Hot_Filling_Step >= 1 && gu8_Hot_Filling_Step <= 6)
    {
        gu16_hot_filling_max_timer++;

        if(gu16_hot_filling_max_timer >= 2400)      // 3분 -> 4분 (온수 최종사양)
        {
            gu16_hot_filling_max_timer = 2400;
            bit_hot_filling_start = CLEAR;
            gu8_Hot_Filling_Step = 0;

			gu8_flushing_mode_saved = gu8_flushing_mode;
            gu8_flushing_mode = FLUSHING_STANDBY_STATE;
            bit_flushing_halt = SET;

            /*..hui [20-3-17오후 1:03:04] 자동으로 드레인하다가 에러 걸렸을때도 정상 에러 표시하도록..*/
            bit_hot_filling_manual = SET;
            Bit0_Hot_Water_Flow_Block_Error__E08 = SET;
            return;
        }
        else{}
    }
    else{}


    if( gu8_Hot_Filling_Step >= 4 && gu8_Hot_Filling_Step <= 6 )
    {
        if( gu8_hot_filling_max_hz < gu8_Flow_1sec )
        {
           gu8_hot_filling_max_hz = gu8_Flow_1sec;
        }
        else{}
    }
    else{}

    switch( gu8_Hot_Filling_Step )
    {
        case 0:

            gu8_Hot_Filling_Step++;
            gu16_hot_filling_control_timer = 0;
            gu16_hot_filling_max_timer = 0;
            F_Hot_Filling_Finish = CLEAR;

			// flushing 삽입 후 변경 예정  ****
            //gu8_flushing_mode = FLUSHING_FILL_HOT_TANK_STATE;

            break;

        case 1:

            gu16_hot_filling_control_timer++;

            /*..hui [21-6-22오후 12:37:28] FEED9 ON..*/
            if(gu16_hot_filling_control_timer >= 3)
            {
                gu8_Hot_Filling_Step++;
                gu16_hot_filling_control_timer = 0;
                gu8_flow_motor_check_step = 0;          /* 유량조절밸브 에이징 적용 250730 CH.PARK */
            }
            else{}

            break;

        case 2:

            gu16_hot_filling_control_timer++;

            /* 설치플러싱일 때에만 1분 간 유량조절밸브 OPEN/CLOSE 에이징 제어 250730 CH.PARK */
            if(bit_install_flushing_state == SET)
            {
                if(gu16_hot_filling_control_timer >= 600) // 1분 동안 유량조절밸브 에이징
	            {
	                gu8_Hot_Filling_Step++;
	                gu16_hot_filling_control_timer = 0;
	            }
	            else
                {
                    switch(gu8_flow_motor_check_step)
                    {
                        case 0:
                            run_decrease_flow(1800);
                            gu8_flow_motor_check_step++;
                        break;

                        case 1:
                            if(gu8_hot_flow_mode == FLOW_MODE_NONE)
                            {
                                gu8_flow_motor_check_step++;
                            }
                        break;

                        case 2:
                            run_increase_flow(1800);
                            gu8_flow_motor_check_step++;
                        break;

                            case 3:
                            if(gu8_hot_flow_mode == FLOW_MODE_NONE)
                            {
                                gu8_flow_motor_check_step++;
                            }
                        break;

                        case 4:
                            run_decrease_flow(1800);
                            gu8_flow_motor_check_step++;
                        break;

                        case 5:
                            if(gu8_hot_flow_mode == FLOW_MODE_NONE)
                            {
                                gu8_flow_motor_check_step++;
                            }
                        break;

                        case 6:
                            run_increase_flow(1800);
                            gu8_flow_motor_check_step++;
                        break;

                        case 7:
                            if(gu8_hot_flow_mode == FLOW_MODE_NONE)
                            {
                                gu8_flow_motor_check_step++;
                            }
                        break;

                        case 8:
                            run_decrease_flow(1800);
                            gu8_flow_motor_check_step++;
                        break;

                        case 9:
                            if(gu8_hot_flow_mode == FLOW_MODE_NONE)
                            {
                                gu8_flow_motor_check_step++;
                            }
                        break;

                        case 10:
                            run_init_flow();
                            gu8_flow_motor_check_step++;
                        break;

                        case 11:
                            if(gu8_hot_flow_mode == FLOW_MODE_NONE)
                            {
                                gu8_flow_motor_check_step = 0;
                                gu8_Hot_Filling_Step++;
                                gu16_hot_filling_control_timer = 0;
                            }
                        break;
                    }
                }
            }
            else
            {
                /*..hui [21-2-8오후 9:33:33] FEED3 ON..*/
                /*..hui [21-6-22오후 5:01:46] 유량 안정화될때까지 1초 대기..*/
                if(gu16_hot_filling_control_timer >= 10)
                {
                    gu8_Hot_Filling_Step++;
                    gu16_hot_filling_control_timer = 0;
                }
                else {  }
            }
            break;

        case 3:

            /*..hui [19-7-22오후 1:54:25] 3초 경과 후 유량 센서 확인..*/
            /*if(gu8_Flow_1sec > HOT_BLOCKED_FLOW_HZ)*/
            /*..hui [20-2-13오후 1:46:12] 첫잔 드레인은 펌프 Full 가동이므로 15Hz로 변경..*/
            /*..hui [20-2-13오후 1:46:42] 드레인 막혀있는데 안전변으로 빠져서 배수막힘에러가 아닌 과열에러가 뜰수있음..*/
            /*..hui [20-2-13오후 1:50:51] 15Hz 약 0.2리터 이상..*/
            if(gu8_Flow_1sec > HOT_FIRST_BLOCKED_FLOW_HZ)
            {
                gu16_hot_filling_control_timer++;

                if(gu16_hot_filling_control_timer >= 10)
                {
                    gu8_Hot_Filling_Step++;
                    gu16_hot_filling_control_timer = 0;
                }
                else{}
            }
            else
            {
                gu16_hot_filling_control_timer = 0;
            }

            break;

        case 4:

            /*..hui [20-2-13오후 2:08:23] 250CC의 절반 125CC 들어가면 히터 전력체크시작..*/
            /*if( gu16Effluent_Hz <= (U16)(C_CUP_250CC_HOT / 2) )*/
			// flushing 삽입 후 변경 예정  ****

            // if( gu16_Filling_Hz <= (U16)(C_CUP_120CC_ROOM ) )
            if( gu16_Filling_Hz <= (U16)(C_CUP_250CC_ROOM ) )       //2025-07-10 cbr 1kg 최종사양
            {
                gu8_Hot_Filling_Step++;
                gu16_hot_filling_control_timer = 0;
                gu8_hot_setting_temperature = HOT_SET_TEMP____85oC;		// ★2025-06-20 phil    //2025-07-11 cbr
                u8Target_Hz_Hot = 30;									// ★2025-06-20 phil    //2025-07-11 cbr
            }
            else{}

            break;

        case 5:

            gu16_hot_filling_control_timer++;


            /*..hui [19-1-24오후 1:42:56] 1.5초 동안 전력 체크한다..*/
            if(gu16_hot_filling_control_timer >= 15)
            {
                gu16_hot_filling_control_timer = 0;
                gu8_Hot_Filling_Step++;
            }
            else
            {
                /*..hui [19-1-24오후 2:00:47] 풀 출력 완료된 후 계산..*/
                if( pHEATER_HOT_H_RELAY == SET
                   && pHEATER_HOT_TRIAC_L == SET
                   && gu8TriacLevelMax == STEP_TRIAC )
                {
                    Cal_HeaterWatt();

                }
                else{}
            }

            break;

        case 6:

            if(F_Hot_Filling_Finish == SET)
            {
                gu8_Hot_Filling_Step = 0;
                gu16_hot_filling_control_timer = 0;
                F_First_Hot_Effluent = CLEAR;
                bit_hot_filling_start = CLEAR;

                gu8_flushing_mode = FLUSHING_FINISH_STATE;
            }
            else{}

            break;

        default :

             gu8_Hot_Filling_Step = 0;
             gu16_hot_filling_control_timer = 0;

             break;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/


