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
#include    "motor_flow.h"


void ControlFlow(void);
U8 Init_Flow(void);
U8 Increase_Flow( void );
U8 Decrease_flow( void );
void HAL_ControlFlowMotor( void );
void FLOW_Motor_MoveStep( void );
void Flow_Motor_Operation( U8 mode );
void run_init_flow(void);
void run_increase_flow( U16 mu16_move );
void run_decrease_flow( U16 mu16_move );


U16 gu16_flow_info_target;
U16 gu16_flow_info_current;

U8 gu8_flow_hold_start_step;
U8 gu8_flow_hold_stop_step;


U8 gu8_flow_info_current_step;
U8 gu8_flow_info_prev_step;

const U8 gu8FlowPhaseTable[ FLOW_STEP_NUM ] =
{ 0x04, 0x03, 0x02, 0x01 };


FLOW_VALVE_MODE  gu8_hot_flow_mode;
U8 gu8_hot_flow_mode_old;
U8 gu8_hot_flow_ctrl_delay_Timer_100ms = CLEAR;

U8 gu8_flow_init_step;
U8 gu8_flow_increase_step;
U8 gu8_flow_decrease_step;


U16 gu16_increase_pulse;
U16 gu16_decrease_pulse;

U16 gu16_moving_pulse;

U8 gu8_11111111;
U8 gu8_22222222;

U16 gu16_aaaaaaaaa;
U16 gu16_bbbbbbbbb;
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void ControlFlow(void)
{
    U8 mu8Ret = FALSE;

    /* Init */
    if( gu8_hot_flow_mode == FLOW_MODE_INIT )
    {
        mu8Ret = Init_Flow();
        if( mu8Ret == TRUE )
        {
            gu8_hot_flow_mode = FLOW_MODE_NONE;
            gu8_flow_init_step = 0;
        }
        else{}

        return;
    }

    /* Open */
    if( gu8_hot_flow_mode == FLOW_MODE_INCREASE )
    {
        mu8Ret = Increase_Flow();
        if( mu8Ret == TRUE )
        {
            gu8_hot_flow_mode = FLOW_MODE_NONE;
            gu8_flow_increase_step = 0;
        }
        else{}

        return;
    }

    /* Close */
    if( gu8_hot_flow_mode == FLOW_MODE_DECREASE )
    {
        mu8Ret = Decrease_flow();
        if( mu8Ret == TRUE )
        {
            gu8_hot_flow_mode = FLOW_MODE_NONE;
            gu8_flow_decrease_step = 0;
        }
        else{}

        return;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 Init_Flow(void)
{
    switch( gu8_flow_init_step )
    {
        case 0:

            gu16_flow_info_target = 0;

            gu16_flow_info_current = FLOW_VALVE_MAX_OPEN;
            gu16_flow_info_target = 0;

            gu8_flow_init_step++;
            break;

        case 1:

            if( gu16_flow_info_target == gu16_flow_info_current )
            {
                gu16_flow_info_target = 0;
                gu16_flow_info_current = 0;
                gu8_flow_init_step++;
            }
            break;

        case 2:

            //gu16_flow_info_target = (U16)(FLOW_VALVE_MAX_OPEN - FLOW_VALVE_ADJUST);
            gu16_flow_info_target = FLOW_VALVE_ADJUST;

            gu8_flow_init_step++;
            break;

        case 3:

            if( gu16_flow_info_target == gu16_flow_info_current )
            {
                gu16_flow_info_target = 0;
                gu16_flow_info_current = 0;
                gu8_flow_init_step++;
            }
            break;

        case 4:
            return TRUE;

    }

    return FALSE;
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 Increase_Flow( void )
{
    switch( gu8_flow_increase_step )
    {
        case 0:

            gu8_flow_hold_start_step = FLOW_HOLD_STEP;
            gu8_flow_hold_stop_step = FLOW_HOLD_STEP;

            if( gu16_flow_info_target > gu16_moving_pulse )
            {
                gu16_flow_info_target = gu16_flow_info_target - gu16_moving_pulse;
            }
            else
            {
                gu16_flow_info_target = 0;
            }

            gu8_flow_increase_step++;

            break;

        case 1:

            if( gu16_flow_info_target == gu16_flow_info_current )
            {
                gu8_flow_increase_step++;
                gu16_moving_pulse = 0;
            }
            else{}

            break;

        case 2:
            return TRUE;

    }

    return FALSE;

}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 Decrease_flow( void )
{
    switch( gu8_flow_decrease_step )
    {
        case 0:

            gu8_flow_hold_start_step = FLOW_HOLD_STEP;
            gu8_flow_hold_stop_step = FLOW_HOLD_STEP;

            gu16_flow_info_target = gu16_flow_info_target + gu16_moving_pulse;

            gu8_flow_decrease_step++;
            break;

        case 1:

            if( gu16_flow_info_target == gu16_flow_info_current )
            {
                gu8_flow_decrease_step++;
                gu16_moving_pulse = 0;
            }
            break;

        case 2:
            return TRUE;

    }

    return FALSE;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void HAL_ControlFlowMotor( void )
{
    S16 val = 0;
    S16 target = 0;
    S16 current = 0;


    /* CALC STEP, TARGET - CURRENT */

    //if( gu16_flow_info_target != gu16_flow_info_current )
    if( gu16_flow_info_target != gu16_flow_info_current )
    {
        //gu16_flow_info_move_step = gu16_flow_info_target - gu16_flow_info_current;

        //gu8_flow_hold_start_step = FLOW_HOLD_STEP;
        //gu8_flow_hold_stop_step = FLOW_HOLD_STEP;
    }
    else{}

    FLOW_Motor_MoveStep();
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void FLOW_Motor_MoveStep( void )
{
    U8 val = 0;

    /* START HOLD */
    if( gu8_flow_hold_start_step > 0 )
    {
        gu8_flow_hold_start_step--;
        val = gu8FlowPhaseTable[ gu8_flow_info_prev_step ];
    }
    else if( gu16_flow_info_current == gu16_flow_info_target )
    {
        /* STOP HOLD */
        if( gu8_flow_hold_stop_step > 0 )
        {
            gu8_flow_hold_stop_step--;
            val = gu8FlowPhaseTable[ gu8_flow_info_current_step ];
        }
        else
        {
            /* STOP */
            val = 0x00;
        }
    }
    /* MOVE FORWARD */
    else if( gu16_flow_info_current < gu16_flow_info_target )
    {
        /* OPEN */
        /*..hui [21-6-24오후 2:24:48] 이쪽이 유량 줄이기로..*/
        gu16_flow_info_current++;

        if(gu8_flow_info_current_step == 0)
        {
            gu8_flow_info_current_step = FLOW_STEP_NUM - 1;
        }
        else
        {
            gu8_flow_info_current_step--;
        }

        gu8_flow_info_prev_step = gu8_flow_info_current_step;

        val = gu8FlowPhaseTable[ gu8_flow_info_current_step ];
    }
    /* MOVE BACKWARD */
    else if( gu16_flow_info_current > gu16_flow_info_target )
    {
        /* CLOSE */
        gu16_flow_info_current--;

        gu8_flow_info_current_step++;
        if( gu8_flow_info_current_step >= FLOW_STEP_NUM )
        {
            gu8_flow_info_current_step = 0;
        }
        else{}

        gu8_flow_info_prev_step = gu8_flow_info_current_step;

        val = gu8FlowPhaseTable[ gu8_flow_info_current_step ];
    }
    else{}

    Flow_Motor_Operation( val );
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void Flow_Motor_Operation( U8 mode )
{
    switch( mode )
    {
        case 0:
            pSTEP_MOTOR_FLOW_1  = 0;
            pSTEP_MOTOR_FLOW_2  = 0;
            pSTEP_MOTOR_FLOW_3  = 0;
            pSTEP_MOTOR_FLOW_4  = 0;

            break;

        case 1:
            pSTEP_MOTOR_FLOW_1 = 1;
            pSTEP_MOTOR_FLOW_2 = 0;
            pSTEP_MOTOR_FLOW_3 = 0;
            pSTEP_MOTOR_FLOW_4 = 1;

            break;

        case 2:
            pSTEP_MOTOR_FLOW_1 = 1;
            pSTEP_MOTOR_FLOW_2 = 1;
            pSTEP_MOTOR_FLOW_3 = 0;
            pSTEP_MOTOR_FLOW_4 = 0;

            break;

        case 3:
            pSTEP_MOTOR_FLOW_1 = 0;
            pSTEP_MOTOR_FLOW_2 = 1;
            pSTEP_MOTOR_FLOW_3 = 1;
            pSTEP_MOTOR_FLOW_4 = 0;

            break;
        case 4:
            pSTEP_MOTOR_FLOW_1 = 0;
            pSTEP_MOTOR_FLOW_2 = 0;
            pSTEP_MOTOR_FLOW_3 = 1;
            pSTEP_MOTOR_FLOW_4 = 1;

            break;
        default:
            pSTEP_MOTOR_FLOW_1  = 0;
            pSTEP_MOTOR_FLOW_2  = 0;
            pSTEP_MOTOR_FLOW_3  = 0;
            pSTEP_MOTOR_FLOW_4  = 0;
            break;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void run_init_flow(void)
{
    gu8_hot_flow_mode = FLOW_MODE_INIT;
    gu8_flow_init_step = 0;
    gu8_flow_increase_step = 0;
    gu8_flow_decrease_step = 0;
    gu16_moving_pulse = 0;

    gu8_flow_hold_start_step = FLOW_HOLD_STEP;
    gu8_flow_hold_stop_step = FLOW_HOLD_STEP;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void run_increase_flow( U16 mu16_move )
{
    /*..hui [21-8-4오전 11:30:58] 유량 늘림.. 펄스는 원점쪽으로 줄임..*/
    gu8_hot_flow_mode = FLOW_MODE_INCREASE;
    gu8_flow_decrease_step = 0;

    gu8_flow_hold_start_step = FLOW_HOLD_STEP;
    gu8_flow_hold_stop_step = FLOW_HOLD_STEP;

    if( mu16_move > gu16_flow_info_target )
    {
        gu16_moving_pulse = gu16_flow_info_target;
    }
    else
    {
        gu16_moving_pulse = mu16_move;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void run_decrease_flow( U16 mu16_move )
{
    /*..hui [21-8-4오전 9:34:59] 유량 줄임.. 펄스는 원점에서 늘림..*/
    gu8_hot_flow_mode = FLOW_MODE_DECREASE;
    gu8_flow_increase_step = 0;

    gu8_flow_hold_start_step = FLOW_HOLD_STEP;
    gu8_flow_hold_stop_step = FLOW_HOLD_STEP;

    if( (gu16_flow_info_target + mu16_move) >= MAX_DECREASE_FLOW )
    {
        gu16_moving_pulse = MAX_DECREASE_FLOW - gu16_flow_info_target;
    }
    else
    {
        gu16_moving_pulse = mu16_move;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/








