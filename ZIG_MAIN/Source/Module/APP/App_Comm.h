/**
 * File : App_Comm.h
 *
 * Application Programming Interface
 * Depend on API
*/

#ifndef __APP_COMM_H__
#define __APP_COMM_H__

#include "api_uart.h"

typedef enum _teCommState
{
    COMM_STATE_IDLE = 0,
    COMM_STATE_RECEIVING,
    COMM_STATE_TRANSMIT,
    COMM_STATE_CLI,
}teCommState;

typedef struct _tsCommInfo
{
    teCommState comm_state;
    U8 comm_tx_index;
    U8 comm_rx_index;
    U8 comm_tx_buffer[UART3_TX_BUFFER_SIZE];
    U8 comm_rx_buffer[UART3_RX_BUFFER_SIZE];
}tsCommInfo;

void SetCommState(teCommState state);
teCommState GetCommState(void);
void SetCommRxIndex(U8 index);
U8 GetCommRxIndex(void);

void Comm_Packet_Handler(void);


#endif /*__APP_COMM_H__*/

