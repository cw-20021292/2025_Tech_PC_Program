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
#include    "auto_drain_manual.h"

void key_manual_drain(void);
void start_manual_drain(void);
void stop_manual_drain(void);

void manual_drain(void);
void manual_drain_pump_output(void);


bit bit_manual_drain_start;
U16 gu16_manual_drain_max_timer;


bit bit_manual_drain_pump_output;
bit bit_manual_drain_valve_output;
U8 gu8_before_manual_drain_level;
U16 gu16_manual_drain_op_timer;


U16 gu16_manual_drain_finish_timer;
U8 gu8_manual_drain_no_water_timer;


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void key_manual_drain(void)
{
	if( bit_filter_all == CLEAR )
	{
        play_melody_warning_197();
        return;
	}
	else{}

    if( F_All_Lock == SET )
    {
        play_melody_warning_197();
        return;
    }
    else{}

    if( bit_memento_start == SET )
    {
        return;
    }
    else{}
        

    if( bit_manual_drain_start == CLEAR )
    {
        start_manual_drain();
    }
    else
    {
        stop_manual_drain();
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void start_manual_drain(void)
{
    //
    if( bit_filter_all == CLEAR
    || Bit16_Drain_Pump_Error__E60 == SET
    || Bit3_Leakage_Sensor_Error__E01 == SET
    || F_DrainStatus == SET
    || F_WaterOut == SET )
    {
        play_melody_warning_197();
    }
    else
    {
        play_melody_setting_on_198();
        bit_manual_drain_start = SET;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void stop_manual_drain(void)
{
    //
    play_melody_setting_off_199();
    bit_manual_drain_start = CLEAR;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void manual_drain(void)
{
    //
    /*..hui [23-8-14오후 1:30:31] 수동배수 진행중 에러 또는 필터 열림 감지시 정지..*/
    if( bit_manual_drain_start == SET )
    {
        if( bit_filter_all == CLEAR
            || Bit16_Drain_Pump_Error__E60 == SET
            || Bit3_Leakage_Sensor_Error__E01 == SET
            || F_DrainStatus == SET )

        {
            play_melody_warning_197();
            bit_manual_drain_start = CLEAR;
        }
        else{}
    }
    else{}

    if( bit_manual_drain_start == CLEAR )
    {
        gu16_manual_drain_max_timer = 0;
        bit_manual_drain_pump_output = CLEAR;
        bit_manual_drain_valve_output = CLEAR;
        gu8_before_manual_drain_level = DRAIN_LEVEL_EMPTY;
        gu16_manual_drain_op_timer = 0;
        gu16_manual_drain_finish_timer = 0;
        gu8_manual_drain_no_water_timer = 0;
    }
    else
    {
        gu16_manual_drain_max_timer++;

        /*..hui [23-8-14오전 11:43:15] 수동배수 최대 동작시간은 최대 1시간..*/
        /*if(gu16_manual_drain_max_timer >= MANUAL_DRAIN_MAX_TIME)*/
        if(gu16_manual_drain_max_timer >= 3000)
        {
            gu16_manual_drain_max_timer = 0;
            bit_manual_drain_pump_output = CLEAR;
            bit_manual_drain_valve_output = CLEAR;
            gu8_before_manual_drain_level = DRAIN_LEVEL_EMPTY;
            gu16_manual_drain_op_timer = 0;
            gu16_manual_drain_finish_timer = 0;
            gu8_manual_drain_no_water_timer = 0;

        	bit_manual_drain_start = CLEAR;
        }
        else
        {
            manual_drain_pump_output();
    		// bit_manual_drain_pump_output = CLEAR;
    		bit_manual_drain_valve_output = SET;
        }


	
    }
}





/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void manual_drain_pump_output(void)
{

    /*..hui [23-8-14오후 4:17:30] 드레인 시간 - 만수위부터 시작..*/
    /* 600
       710
       757
       780
       775
       930
       930 */


    switch(u8DrainWaterLevel)
    {
        case DRAIN_LEVEL_EMPTY :

             bit_manual_drain_pump_output = CLEAR;
             bit_manual_drain_valve_output = SET;

             gu8_before_manual_drain_level = DRAIN_LEVEL_EMPTY;
             gu16_manual_drain_op_timer = 0;
             gu8_manual_drain_no_water_timer = 0;

			 gu16_manual_drain_finish_timer++;
			 
			 /*..hui [23-8-14오후 1:37:19] x분동안 드레인밸브 열었는데 저수위 이상 감지 안되면 종료..*/
			 if( gu16_manual_drain_finish_timer >= MANUAL_DRAIN_FINISH_CHECK_TIME )
			 {
				 gu16_manual_drain_finish_timer = 0;
				 gu16_manual_drain_max_timer = MANUAL_DRAIN_MAX_TIME;
			 }
			 else{}

			 

            break;

        case DRAIN_LEVEL_LOW :

             gu16_manual_drain_finish_timer = 0;

             /*..hui [18-1-14오후 6:06:23] 물없는 상태에서 냉수 배수밸브 ON 상태로 저수위 진입하면..*/
             if(gu8_before_manual_drain_level == DRAIN_LEVEL_EMPTY)
             {
                 gu16_manual_drain_op_timer++;

                 /*..hui [18-1-14오후 6:06:47] 40초 이상 냉수 배수했는데도 만수위에 도달하지 못하면..*/
                 /*..hui [18-1-14오후 6:07:00] 다시 드레인펌프 가동해서 드레인 시작..*/
                 /*..hui [18-1-14오후 6:11:06] 센서 오감지, 유로막힘등의 이유로.. 배수탱크 넘치지 않게 하기위해..???..*/
                 if(gu16_manual_drain_op_timer >= DRAIN_VALVE_OPEN_TIME)
                 {
                     bit_manual_drain_pump_output = SET;
                     bit_manual_drain_valve_output = CLEAR;

                     /*..hui [18-1-25오후 4:11:59] 드레인펌프 가동으로 전환하고 3초후에 피드백 확인..*/
                     if(gu16_manual_drain_op_timer >= DRAIN_VALVE_OPEN_TIME + 30)
                     {
                         /*..hui [18-1-25오후 4:12:27] 피드백 이하이면 다시 냉수 밸브 ON..*/
                         if(gu16_AD_Drain_Pump_Current <= DRAIN_COMPLETE_FEEDBACK)
                         {
                             gu16_manual_drain_op_timer = 0;
                         }
                         else
                         {
                             /*..hui [18-1-25오후 4:12:45] 피드백 초과이면 드레인펌프 ON 상태 유지..*/
                             gu16_manual_drain_op_timer = DRAIN_VALVE_OPEN_TIME + 30;
                         }
                     }
                     else{}
                 }
                 else
                 {
                     bit_manual_drain_pump_output = CLEAR;
                     bit_manual_drain_valve_output = SET;
                 }
             }
             else
             {
                 gu16_manual_drain_op_timer = 0;
                 /*..hui [18-1-14오후 6:15:33] 만수위 감지되고 드레인펌프 가동하여 저수위까지 내려왔을때..*/
                 /*..hui [18-1-14오후 6:21:04] 이것도 센서 오감지나 유로 에어등의 이유로 물없음까지 잘 안내려갈때..*/
                 /*..hui [18-1-14오후 6:21:55] 다시 드레인펌프 OFF하고 냉수 밸브 ON하여 만수위로 가도록....*/
                 if(gu16_AD_Drain_Pump_Current <= DRAIN_COMPLETE_FEEDBACK)
                 {
                     gu8_manual_drain_no_water_timer++;

                     if( gu8_manual_drain_no_water_timer >= 100 )
                     {
                         gu8_manual_drain_no_water_timer = 100;
                         bit_manual_drain_pump_output = CLEAR;
                         bit_manual_drain_valve_output = SET;
                     }
                     else
                     {
                         bit_manual_drain_pump_output = SET;
                         bit_manual_drain_valve_output = CLEAR;
                     }
                 }
                 else
                 {
                     bit_manual_drain_pump_output = SET;
                     bit_manual_drain_valve_output = CLEAR;
                     gu8_manual_drain_no_water_timer = 0;
                 }
             }

            break;

        case DRAIN_LEVEL_ERROR :
        case DRAIN_LEVEL_HIGH :

             bit_manual_drain_pump_output = SET;
             bit_manual_drain_valve_output = CLEAR;
             gu8_before_manual_drain_level = DRAIN_LEVEL_HIGH;
             gu8_manual_drain_no_water_timer = 0;

            break;

        //=====================================================
        default :

             bit_manual_drain_pump_output = CLEAR;
             bit_manual_drain_valve_output = CLEAR;
             gu8_before_manual_drain_level = DRAIN_LEVEL_EMPTY;

            break;
      }
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/




