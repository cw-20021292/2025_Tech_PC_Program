/**
 * File : App_Comm.h
 *
 * Application Programming Interface
 * Depend on API
*/

#ifndef __APP_COMM_H__
#define __APP_COMM_H__

#include "api_uart.h"

#define COMM_STX        0x02
#define COMM_ETX        0x03
#define COMM_ID_PC      0x01
#define COMM_ID_MAIN    0x02
#define COMM_ID_FRONT   0x03

#define COMM_PACKET_BASIC_LENGTH          7

#define GET_16_HIGH_BYTE(val)       (U8)(((val)&0xFF00) >> 8)
#define GET_16_LOW_BYTE(val)        (U8)((val)&0x00FF)

typedef enum _teCommState
{
    COMM_STATE_IDLE = 0,
    COMM_STATE_RECEIVING,
    COMM_STATE_TRANSMIT,
    COMM_STATE_ERROR,
}teCommState;

typedef struct _tsCommInfo
{
    teCommState comm_state;
    U8 comm_tx_index;
    U8 comm_rx_index;
    U8 comm_tx_buffer[UART3_TX_BUFFER_SIZE];
    U8 comm_rx_buffer[UART3_RX_BUFFER_SIZE];
}tsCommInfo;

void Comm_Packet_Handler(void);


#endif /*__APP_COMM_H__*/

