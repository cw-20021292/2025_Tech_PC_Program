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
#include    "boost_pump.h"


void output_boost_pump(void);
U8 pump_retry_proc( void );
void start_boost_pump( U16 u16_data );
void stop_boost_pump( void );
void boost_pump_input(void);


/***********************************************************************************************************************/
TYPE_BYTE          U8PumpONB;
#define            u8PumpON                               U8PumpONB.byte
#define            Bit0_Pump_Feed_Operation_State         U8PumpONB.Bit.b0




TYPE_BYTE          U8PumpOFFB;
#define            u8PumpOFF                              U8PumpOFFB.byte
#define            Bit0_Pump_Not_Connect_Off_State        U8PumpOFFB.Bit.b0



bit bit_pump_output;
U8 gu8_pump_delay_timer;

PUMP_RETRY_STEP gu8_pump_retry_step;
U32 gu32_pump_retry_timer;

U16 gu16_current_pump_pwm;

U8 gu8_boost_on_decision_timer;
U8 gu8_boost_off_decision_timer;

bit bit_boost_pump_connect;
bit bit_boost_init_check;
/***********************************************************************************************************************/



/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void output_boost_pump(void)
{
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 pump_retry_proc( void )
{
    U8 mu8_return = 0;


    return mu8_return;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void start_boost_pump( U16 u16_data )
{
    //R_TAU0_Channel0_Start();
    //TDR01 = u16_data;
    //gu16_current_pump_pwm = u16_data;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void stop_boost_pump( void )
{
    //R_TAU0_Channel0_Stop();
    //TDR01 = 0;
    //gu16_current_pump_pwm = 0;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void boost_pump_input(void)
{
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/




