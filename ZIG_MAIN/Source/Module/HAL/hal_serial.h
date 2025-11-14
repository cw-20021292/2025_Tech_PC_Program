/**
 * File : hal_serial.h
 *
 * Hardware Abstraction Layer
 * Depend on Renesas MCU Chip
*/
#ifndef __HAL_SERIAL_H__
#define __HAL_SERIAL_H__

#include "macrodriver.h"

//#define USE_UART0_MODULE
#define USE_UART3_MODULE

#define UART0_GET_SERIAL_STATUS()           (U8)(SSR01 & 0x0007U)
#define UART0_CLEAR_ERROR_FLAG(errType)      do{ SIR01 = (U16)errType; }while(0);
#define UART0_GET_RX_DATA()                  (RXD0)
#define UART0_SET_TX_DATA(data)              (TXD0 = data)

#define UART0_START()                        R_UART0_Start();
#define UART0_STOP()                         R_UART0_Stop();

#define UART3_GET_SERIAL_STATUS()           (U8)(SSR13 & 0x0007U)
#define UART3_CLEAR_ERROR_FLAG(errType)      do{ SIR13 = (U16)errType; }while(0);
#define UART3_GET_RX_DATA()                  (RXD3)
#define UART3_SET_TX_DATA(data)              (TXD3 = data)

#define UART3_START()                        R_UART3_Start();
#define UART3_STOP()                         R_UART3_Stop();

typedef enum _teUART_CHANNEL
{
    UART_CHANNEL_0 = 0,
    UART_CHANNEL_1,
    UART_CHANNEL_2,
    UART_CHANNEL_3,
}teUART_CHANNEL;

typedef enum _teUART_DIRECTION
{
    UART_DIR_RX = 0,
    UART_DIR_TX,
}teUART_DIRECTION;

void HAL_Uart_Init(teUART_CHANNEL channel, void (*recv_callback_func)(U8), void (*send_callback_func)(void));
void HAL_Uart_Deinit(U8 channel);

#endif  /* __HAL_SERIAL_H__ */

