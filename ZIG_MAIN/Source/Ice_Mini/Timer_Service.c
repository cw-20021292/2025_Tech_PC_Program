/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#include    "Macrodriver.h"
#include    "Global_Variable.h"
#include    "Port_Define.h"
#include    "Timer_Service.h"

void Timer_250us_Service(void);

bit F_250us;
bit F_1ms;
bit F_2ms;
bit F_2_5ms;
bit F_3ms;
bit F_10ms;
bit F_50ms;
bit F_100ms;
bit F_1sec;
bit F_1min;
bit F_Aging5s;
U8 u8Count_500us;
U8 u8Count_1ms;
U8 u8Count_2ms;
U8 u8Count_2_5ms;
U8 u8Count_3ms;
U8 u8Count_10ms;
U8 u8Count_50ms;
U8 u8Count_100ms;
U8 u8Count_1sec;
U8  u8Count_1min;
U8  u8CountSync;
U8  u8CountSync_50Ms;

U8 u8Count_ReceiveDelay;
U8 u8ChildNozzleChang_Cnt;
U8 u8ChildNozzleChang_Cnt2;

extern U16 u16Efluent_Time;
extern WATER_OUT_STEP gu8_Water_Out_Step;

bit F_ADC_Check;
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void Timer_250us_Service(void)
{

    F_250us = SET;

    u8Count_500us++;

    u8Count_2_5ms++;
    u8Count_3ms++;

    u8Count_1ms++;

    if(u8Count_500us >= 2)
    {
        u8Count_500us = 0;
        /*..hui [24-11-13���� 2:34:48] TDS �ٽ� ����..*/
        /*TDS_Out_Pulse_Control();*/
    }
    else{}

    if(u8Count_1ms >= 4)
    {
        /*1ms*/
        F_1ms = SET;

        flow_input_count();
        /////filter_flow_input_count();
        Key_Input();

        /*AD_Conversion();*/
        F_ADC_Check = SET;
        /*BuzControl();*/

        /*..hui [24-11-13���� 2:34:48] TDS �ٽ� ����..*/
        /*TDS_In_Control();*/

        u8CountSync++;
        if(u8CountSync >= 250)
        {
            u8CountSync = 250;
        }
        else{}

        u8CountSync_50Ms++;
        if(u8CountSync_50Ms >= 250)
        {
            u8CountSync_50Ms = 250;
        }
        else{}

        u8Count_2ms++;
        u8Count_1ms = CLEAR;
    }
    else{}

    if(u8Count_2ms >= 2)
    {
        /*2ms*/
        F_2ms = SET;
        u8Count_2ms = CLEAR;

        //lcd_rx_communication();

        u8Count_10ms++;
    }
    else{}

    if(u8Count_2_5ms >= 10)
    {
        /* 2.5ms*/
        F_2_5ms = SET;
        u8Count_2_5ms = CLEAR;
    }
    else{}

    if(u8Count_3ms >= 12)
    {
        /*3ms*/
        F_3ms = SET;
        u8Count_3ms = CLEAR;
    }
    else{}

    if(u8Count_10ms >= 5)
    {
        /*10ms*/
        F_10ms = SET;
        u8Count_10ms = CLEAR;
        u8Count_50ms++;
        u8Count_100ms++;

        //if(u8PcbTestStart != SET && F_UARTest != SET && F_Display_Test_Main_Line != SET)
        //{
        //    output_valve_extract();
        //}
        //else{}
/***********************************************************************************************************************/
        /*BuzTimeCounter();*/       // Buzzer �� ���� �ð� Counter
/***********************************************************************************************************************/
        //input_select_bar();
/***********************************************************************************************************************/
        //input_ice_tray_micro_sw();
        //Front_Communication();

        /*final_mode_decision();*/
        ////////////////////BLDC_RXD2_TimeOutCheck();


    }
    else{}

    if(u8Count_50ms >= 5)
    {
        F_50ms = SET;
        u8Count_50ms = 0;
    }
    else{}


    if(u8Count_100ms >= 10)
    {
        /*100ms*/
        F_100ms = SET;
        u8Count_100ms = CLEAR;

        if(gu8IceClose) { gu8IceClose--; }                      // ������ ���� ������
        if(gu8IceInnerClose) { gu8IceInnerClose--; }
        
        u8Count_1sec++;
    }
    else{}

    if(u8Count_1sec >= 10)
    {
        /*1s*/
        F_1sec = SET;
        u8Count_1sec = CLEAR;

        F_Powerin1sec = SET; /*..���� on�� 1��..*/
        //IceMakeOrder();

        //if(gu16MeltTime) gu16MeltTime--;                        // SB �����̺��ʹ��
 /***********************************************************************************************************************/


        if(gu8_altutude_setting_timeout > 0)
        {
            gu8_altutude_setting_timeout--;                    //nfc test key delay  20150410
        }
        else{}

        /*..hui [19-7-25���� 8:33:32] Ÿ�̸� 1�ʷ� �̵�..*/
        if(gu16IceStopTime > 0)
        {
            gu16IceStopTime--;                  // ������������ 6�ð�
        }
        else{}

 /***********************************************************************************************************************/

        u8Count_1min++;
    }
    else{}

    if(u8Count_1min >= 60)
    {
        /*1min*/
        F_1min = SET;
        u8Count_1min = CLEAR;
        //power_saving_timer();
    }
    else{}
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/



