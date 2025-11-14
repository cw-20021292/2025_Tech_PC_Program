/**
 * File : api_uart.h
 *
 * Application Programming Interface
 * Depend on HAL
*/

#ifndef __API_UART_H__
#define __API_UART_H__

#include "hal_serial.h"

#define UART0_RX_BUFFER_SIZE    128
#define UART0_TX_BUFFER_SIZE    128

#define UART3_RX_BUFFER_SIZE    256
#define UART3_TX_BUFFER_SIZE    256

void Uart_Init(void);

U8 Uart_Read_Data(teUART_CHANNEL channel, U8* data);
void Uart_Send_Data(teUART_CHANNEL channel, U8 data);
void Uart_Send_String(teUART_CHANNEL channel, const char *str_buf);
void Uart_Send_Buffer(teUART_CHANNEL channel, const char *str_buf, U16 length);

#endif /*__API_UART_H__*/

