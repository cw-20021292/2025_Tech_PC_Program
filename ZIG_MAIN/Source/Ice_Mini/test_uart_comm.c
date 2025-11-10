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
#include    "test_uart_comm.h"


void AT_UART_Communication(void);
void AT_UART_Rx_Process(void);
void AT_UART_Tx_Process(void);
void int_UART3_AT_TX(void);
void int_UART3_WORK_RX(void);




bit AT_F_TxStart;             //
bit AT_F_RxComplete;          //
bit AT_F_Rx_NG;               //

U8 AT_gu8TX_ERROR;
U8 AT_gu8TxData[70];
U8 AT_gu8RxData[70];
U8 AT_gu8TxdCounter;
U8 AT_gu8UARTStateMode;
U8 AT_gu8RxdCounter;

U16 AT_gu16_CMD_Mode;
U8 gu8UART_DataLength;
bit F_AT_TX_Finish;
bit F_Uart_Final;
U8 gu8RxdBufferData;

U8 gu8_uart_test_mode;

U8 gu8_uart_comp_start;
U8 gu8_uart_comp_rps;

extern U8 u8Freezing_Table_Used;
// U16 gu16Temp_MakeTime[46][46];
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//                    (1) UART ��� ������ ó��
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void AT_UART_Communication(void)
{
    AT_UART_Rx_Process();         // 1-1 ���ź� ������ ��ȯ
    AT_UART_Tx_Process();         // 1-2 �۽ź� ������ ��ȯ
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
///""SUBR COMMENT""************************************************************
// ID         : ATUO TEST_Rx_Process
// ����         : �ڵ�ȭ �׽�Ʈ ���ź�
//----------------------------------------------------------------------------
// ���       : �ڵ�ȭ �׽�Ʈ ���ź� ó��
//
//----------------------------------------------------------------------------
//""SUBR COMMENT END""********************************************************
//------------------------
// 1-1 ���ź� ������ ��ȯ
//------------------------
void AT_UART_Rx_Process(void)
{
    U8 u8cmd = 0;
    if(AT_F_RxComplete != SET)
    {
        return;
    }
    else
    {
        AT_F_RxComplete = 0;
    }

    u8cmd = AT_gu8RxData[2];

    // WORK_CMD 처리
    switch( u8cmd )
    {
        case WORK_CMD_HEARTBEAT:                    // 0x0F: 하트비트 요청
            // 하트비트 응답 처리
            break;

        case WORK_CMD_POLLING:                      // 0xF0: 상태조회 (POLLING) 요청
            // 폴링 데이터 응답 처리
            break;

        case WORK_CMD_VALVE_CHANGE:                 // 0xA0: 밸브 부하 변경 요청
            // 밸브 변경 처리
            break;

        case WORK_CMD_DRAIN_PUMP_CHANGE:            // 0xA1: 드레인 펌프 출력 변경 요청
            // 드레인 펌프 변경 처리
            break;

        case WORK_CMD_COOLING_SYSTEM_CHANGE:        // 0xB0: 공조시스템 변경 요청
            // 공조시스템 변경 처리
            break;

        case WORK_CMD_COOLING_RUN_CHANGE:           // 0xB1: 냉각운전 변경 요청
            // 냉각운전 변경 처리
            break;

        case WORK_CMD_FREEZING_RUN_CHANGE:          // 0xB2: 제빙운전 변경 요청
            // 제빙운전 변경 처리
            break;

        case WORK_CMD_FREEZING_TABLE_CHANGE:        // 0xB3: 제빙테이블 변경 요청
            // 제빙테이블 변경 처리
            
            break;

        case WORK_CMD_COOLING_TABLE_CHANGE:         // 0xB4: 보냉운전 변경 요청
            // 보냉운전 변경 처리
            break;

        case WORK_CMD_SENSOR_CHANGE:                // 0xC0: 센서값 변경 요청
            // 센서값 변경 처리
            break;

        default:
            // 알 수 없는 명령
            break;
    }
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
//------------------------
// 1-2 �۽ź� ������ ��ȯ
//------------------------
void AT_UART_Tx_Process(void)
{
    static U8  AT_mu8Temp_Data1, AT_mu8Temp_Data2, AT_mu8Temp_Data3, gu8UARTAddr;
    static U16 AT_mu16Temp_Data;

    if(AT_F_TxStart != SET)
    {
        return;
    }
    else
    {
        AT_F_TxStart = 0;
    }

    AT_gu8TxData[0] = AT_RS232_STX;
    TXD3 = AT_gu8TxData[AT_gu8TxdCounter];               // ù��° ����Ʈ ������

    AT_gu8TxdCounter++;

    F_AT_TX_Finish = 1;
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
//------------------------------------------------------------------------------
//                      UART RXD (INTST1) - AutoTest
//------------------------------------------------------------------------------
void int_UART3_AT_TX(void)
{
//  TXD1=0x20;

    NOP();
    NOP();
    NOP();
    NOP();

    if(F_AT_TX_Finish == SET)
    {
        // ���� ���ͷ� �Ŀ� �ٷ� ����
        TXD3 = AT_gu8TxData[AT_gu8TxdCounter];

        if(AT_gu8TxData[AT_gu8TxdCounter] == 0x04)        // �۽ſϷ�
        {
            AT_gu8TxdCounter = 0;                          // ETX ���� �� ���� ī��Ʈ �ʱ�ȭ
            F_AT_TX_Finish = 0;
        }
        else
        {
            AT_gu8TxdCounter++;
        }
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
//------------------------------------------------------------------------------
//                      UART RXD (INTSR1) - AutoTest
//------------------------------------------------------------------------------
void int_UART3_WORK_RX(void)
{
    U8 err_type03;

    err_type03 = (U8)(SSR13 & 0x0007);
    SIR13 = (U16)err_type03;

    gu8RxdBufferData = RXD3;

    switch(AT_gu8UARTStateMode)
    {
        // ���
        case UART_MODE_IDLE:

             if(gu8RxdBufferData == WORK_STX)
             {                 // STX check 0x01
                 AT_gu8RxdCounter = 0;
                 AT_gu8UARTStateMode = UART_MODE_RECEIVE;            // 0x01�� ������ '������'����
                 AT_gu8RxData[AT_gu8RxdCounter++] = gu8RxdBufferData;// Stx ī��Ʈ 0
             }
             else
             {
                 AT_gu8RxdCounter = 0;
             }

             break;

         // ������
        case UART_MODE_RECEIVE:

             if(gu8RxdBufferData == WORK_ETX)
             {                 // ETX check 0x04
                 AT_gu8RxData[AT_gu8RxdCounter] = gu8RxdBufferData;
                AT_F_RxComplete = 1;                                // ���ſϷ�

                // Rx data initialize //
                AT_gu8RxdCounter = 0;
                 AT_gu8UARTStateMode = UART_MODE_IDLE;
             }
             else
             {
                 AT_gu8RxData[AT_gu8RxdCounter++] = gu8RxdBufferData;// ���� ������ ����
             }

             break;

        // Error
        case UART_MODE_ERROR:

             AT_gu8RxdCounter = 0;
             AT_gu8UARTStateMode = UART_MODE_IDLE;

             break;


        default:  // Rx data initialize //

             AT_gu8RxdCounter = 0;
             AT_gu8UARTStateMode = UART_MODE_IDLE;

             break;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/


