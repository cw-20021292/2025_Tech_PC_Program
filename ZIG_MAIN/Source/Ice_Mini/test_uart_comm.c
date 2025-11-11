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
#include    "M8_Ice_Making.h"
#include    "M9_Front_Communication.h"

void AT_UART_Communication(void);
void AT_UART_Rx_Process(void);
void AT_UART_Tx_Process(void);
void int_UART3_AT_TX(void);
void int_UART3_WORK_RX(void);

bit AT_F_TxStart;             //
bit AT_F_RxComplete;          //
bit AT_F_Rx_NG;               //

U8 AT_gu8TX_ERROR;
U8 AT_gu8TxData[255];
U8 AT_gu8RxData[255];
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


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//                    (1) UART ÔøΩÔøΩÔø? ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ √≥ÔøΩÔøΩ
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void AT_UART_Communication(void)
{
    AT_UART_Rx_Process();         // 1-1 ÔøΩÔøΩÔøΩ≈∫ÔøΩ ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ ÔøΩÔøΩ»Ø
    AT_UART_Tx_Process();         // 1-2 ÔøΩ€Ω≈∫ÔøΩ ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ ÔøΩÔøΩ»Ø
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
///""SUBR COMMENT""************************************************************
// ID         : ATUO TEST_Rx_Process
// ÔøΩÔøΩÔøΩÔøΩ         : ÔøΩ⁄µÔøΩ»≠ ÔøΩ◊ΩÔøΩ∆Æ ÔøΩÔøΩÔøΩ≈∫ÔøΩ
//----------------------------------------------------------------------------
// ÔøΩÔøΩÔø?       : ÔøΩ⁄µÔøΩ»≠ ÔøΩ◊ΩÔøΩ∆Æ ÔøΩÔøΩÔøΩ≈∫ÔøΩ √≥ÔøΩÔøΩ
//
//----------------------------------------------------------------------------
//""SUBR COMMENT END""********************************************************
//------------------------
// 1-1 ÔøΩÔøΩÔøΩ≈∫ÔøΩ ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ ÔøΩÔøΩ»Ø
//------------------------
void AT_UART_Rx_Process(void)
{
    U8 u8cmd = 0;

    if(AT_F_RxComplete == CLEAR)
    {
        return;
    }
    else
    {
        AT_F_RxComplete = 0;
    }

    u8cmd = AT_gu8RxData[2];

    // WORK_CMD Ï≤òÎ¶¨
    switch( u8cmd )
    {
        case WORK_CMD_HEARTBEAT:                    // 0x0F: ?ïò?ä∏ÎπÑÌä∏ ?öîÏ≤?
            // ?ïò?ä∏ÎπÑÌä∏ ?ùë?ãµ Ï≤òÎ¶¨
            break;

        case WORK_CMD_VALVE_CHANGE:                 // 0xA0: Î∞∏Î∏å Î∂??ïò Î≥?Í≤? ?öîÏ≤?
            // Î∞∏Î∏å Î≥?Í≤? Ï≤òÎ¶¨
            break;

        case WORK_CMD_DRAIN_PUMP_CHANGE:            // 0xA1: ?ìú?†à?ù∏ ?éå?îÑ Ï∂úÎ†• Î≥?Í≤? ?öîÏ≤?
            // ?ìú?†à?ù∏ ?éå?îÑ Î≥?Í≤? Ï≤òÎ¶¨
            break;

        case WORK_CMD_COOLING_SYSTEM_CHANGE:        // 0xB0: Í≥µÏ°∞?ãú?ä§?Öú Î≥?Í≤? ?öîÏ≤?
            // Í≥µÏ°∞?ãú?ä§?Öú Î≥?Í≤? Ï≤òÎ¶¨
            break;

        case WORK_CMD_COOLING_RUN_CHANGE:           // 0xB1: ?ÉâÍ∞ÅÏö¥?†Ñ Î≥?Í≤? ?öîÏ≤?
            // ?ÉâÍ∞ÅÏö¥?†Ñ Î≥?Í≤? Ï≤òÎ¶¨
            break;

        case WORK_CMD_FREEZING_RUN_CHANGE:          // 0xB2: ?†úÎπôÏö¥?†Ñ Î≥?Í≤? ?öîÏ≤?
            // ?†úÎπôÏö¥?†Ñ Î≥?Í≤? Ï≤òÎ¶¨
            break;

        case WORK_CMD_FREEZING_TABLE_CHANGE:        // 0xB3: ?†úÎπôÌÖå?ù¥Î∏? Î≥?Í≤? ?öîÏ≤?
            SetFreezingTable(&AT_gu8RxData[5]);
            SetUsedFreezingTable(SET);
            break;

        case WORK_CMD_COOLING_TABLE_CHANGE:         // 0xB4: Î≥¥ÎÉâ?ö¥?†Ñ Î≥?Í≤? ?öîÏ≤?
            // Î≥¥ÎÉâ?ö¥?†Ñ Î≥?Í≤? Ï≤òÎ¶¨
            break;

        case WORK_CMD_SENSOR_CHANGE:                // 0xC0: ?Ñº?ÑúÍ∞? Î≥?Í≤? ?öîÏ≤?
            // ?Ñº?ÑúÍ∞? Î≥?Í≤? Ï≤òÎ¶¨
            break;

        default:
            // ?ïå ?àò ?óÜ?äî Î™ÖÎ†π
            break;
    }

    AT_F_TxStart = SET;
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
//------------------------
// 1-2 ÔøΩ€Ω≈∫ÔøΩ ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ ÔøΩÔøΩ»Ø
//------------------------
void AT_UART_Tx_Process(void)
{
    U16 mu16_cal_crc = 0;

    if(AT_F_TxStart == CLEAR)
    {
        return;
    }
    else
    {
        AT_F_TxStart = 0;
    }

    AT_gu8TxData[0] = WORK_STX;
    AT_gu8TxData[1] = WORK_ID_MAIN;
    AT_gu8TxData[2] = WORK_CMD_HEARTBEAT;
    AT_gu8TxData[3] = WORK_CMD_HEARTBEAT_LENGTH;

    /* Datafield */
    AT_gu8TxData[4] = gu8_Amb_Front_Temperature_One_Degree;     // ?ô∏Í∏∞Ïò®?èÑ
    AT_gu8TxData[5] = gu8_Room_Temperature_One_Degree;          // ?ûÖ?àò?ò®?èÑ
    AT_gu8TxData[6] = GetIceStep();                               // ?†úÎπ? Step

    mu16_cal_crc = Rx_CRC_CCITT(AT_gu8TxData, 7);
    AT_gu8TxData[7] = (U8)HighByte(mu16_cal_crc);
    AT_gu8TxData[8] = (U8)LowByte(mu16_cal_crc);
    AT_gu8TxData[9] = WORK_ETX;

    TXD3 = AT_gu8TxData[AT_gu8TxdCounter];               // √πÔøΩÔøΩ¬∞ ÔøΩÔøΩÔøΩÔøΩ∆Æ ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ
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
        // ÔøΩÔøΩÔøΩÔøΩ ÔøΩÔøΩÔøΩÕ∑ÔøΩ ÔøΩƒøÔøΩ ÔøΩŸ∑ÔøΩ ÔøΩÔøΩÔøΩÔøΩ
        TXD3 = AT_gu8TxData[AT_gu8TxdCounter];

        if(AT_gu8TxdCounter < ((WORK_PACKET_BASIC_LENGTH + WORK_CMD_HEARTBEAT_LENGTH - 1)))
        {
            AT_gu8TxdCounter++;
        }
        else
        {
            AT_gu8TxdCounter = 0;
            F_AT_TX_Finish = 0;
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
U16 u16RxDataDebug;

void int_UART3_WORK_RX(void)
{
    U8 err_type03;
    U16 mu16_cal_crc;
    mu16_cal_crc = 0;

    err_type03 = (U8)(SSR13 & 0x0007);
    SIR13 = (U16)err_type03;

    gu8RxdBufferData = RXD3;

    switch(AT_gu8UARTStateMode)
    {
        // ÔøΩÔøΩÔø?
        case UART_MODE_IDLE:

             if(gu8RxdBufferData == WORK_STX)
             {                 // STX check 0x01
                 AT_gu8RxdCounter = 0;
                 AT_gu8UARTStateMode = UART_MODE_RECEIVE;            // 0x01ÔøΩÔøΩ ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ 'ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ'ÔøΩÔøΩÔøΩÔøΩ
                 AT_gu8RxData[AT_gu8RxdCounter++] = gu8RxdBufferData;// Stx ƒ´ÔøΩÔøΩ∆Æ 0
             }
             else
             {
                 AT_gu8RxdCounter = 0;
             }

             break;

         // ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ
        case UART_MODE_RECEIVE:
            AT_gu8RxData[AT_gu8RxdCounter++] = gu8RxdBufferData;

            // ETX∏¶ πﬁæ“¿ª ∂ß CRC ∞À¡ı
            if(gu8RxdBufferData == WORK_ETX)
            {
                // √÷º“ ∆–≈∂ ±Ê¿Ã »Æ¿Œ: STX(1) + TX_ID(1) + CMD(1) + LEN(1) + CRC_HIGH(1) + CRC_LOW(1) + ETX(1) = 7πŸ¿Ã∆Æ
                if(AT_gu8RxdCounter >= 7)
                {
                    // ∆–≈∂ ±Ê¿Ã »Æ¿Œ
                    u16RxDataDebug = (AT_gu8RxData[3] + WORK_PACKET_BASIC_LENGTH);
                    if(AT_gu8RxdCounter == (AT_gu8RxData[3] + WORK_PACKET_BASIC_LENGTH))
                    {
                        mu16_cal_crc = Rx_CRC_CCITT(AT_gu8RxData, (AT_gu8RxdCounter-3));

                        // CRC_HIGHøÕ CRC_LOW ∫Ò±≥
                        if(AT_gu8RxData[AT_gu8RxdCounter-3] == (U8)HighByte(mu16_cal_crc)
                        && AT_gu8RxData[AT_gu8RxdCounter-2] == (U8)LowByte(mu16_cal_crc))
                        {
                            if(AT_gu8RxData[2] == 0xB3)
                            {
                                AT_gu8RxdCounter = 0;
                            }

                            AT_F_RxComplete = 1;
                            // Rx data initialize
                            AT_gu8RxdCounter = 0;
                            AT_gu8UARTStateMode = UART_MODE_IDLE;
                        }
                        else
                        {
                            // CRC ∫“¿œƒ°: πˆ∆€ √ ±‚»≠«œ∞Ì IDLE ∏µÂ∑Œ ¿¸»Ø
                            AT_F_RxComplete = 0;
                            AT_gu8RxdCounter = 0;
                            AT_gu8UARTStateMode = UART_MODE_IDLE;
                        }
                    }
                    else
                    {

                    }
                }
                else
                {
                    // ∆–≈∂ ±Ê¿Ã ∫Œ¡∑: πˆ∆€ √ ±‚»≠«œ∞Ì IDLE ∏µÂ∑Œ ¿¸»Ø
                    AT_F_RxComplete = 0;
                    AT_gu8RxdCounter = 0;
                    AT_gu8UARTStateMode = UART_MODE_IDLE;
                }
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


