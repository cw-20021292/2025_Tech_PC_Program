/**
 * File : hal_serial.c
 * 
 * Hardware Abstraction Layer
 * Depend on Renesas MCU Chip
*/

#if 0
#pragma interrupt INTST0 HAL_Uart0_interrupt_send
#pragma interrupt INTSR0 HAL_Uart0_interrupt_receive
#endif

#if 0
#pragma interrupt INTST3 HAL_Uart3_interrupt_send
#pragma interrupt INTSR3 HAL_Uart3_interrupt_receive
#endif

#include "hal_serial.h"

#ifdef USE_UART0_MODULE
static void (*uart0_rx_callback_func)(U8) = NULL;
static void (*uart0_tx_callback_func)(void) = NULL;
#endif /* USE_UART0_MODULE */

#ifdef USE_UART3_MODULE
static void (*uart3_rx_callback_func)(U8) = NULL;
static void (*uart3_tx_callback_func)(void) = NULL;
#endif /* USE_UART3_MODULE */

void HAL_Uart_Init(teUART_CHANNEL channel, void (*recv_callback_func)(U8), void (*send_callback_func)(void))
{
#ifdef USE_UART0_MODULE
    if(channel == UART_CHANNEL_0)
    {
        uart0_rx_callback_func = recv_callback_func;
        uart0_tx_callback_func = send_callback_func;
        UART0_START();
    }
#endif /* USE_UART0_MODULE */
#ifdef USE_UART3_MODULE
    if(channel == UART_CHANNEL_3)
    {
        uart3_rx_callback_func = recv_callback_func;
        uart3_tx_callback_func = send_callback_func;
        UART3_START();
    }
#endif /* USE_UART3_MODULE */
}

void HAL_Uart_Deinit(U8 channel)
{
#ifdef USE_UART0_MODULE
    if(channel == UART_CHANNEL_0)
    {
        UART0_STOP();
        uart0_rx_callback_func = NULL;
        uart0_tx_callback_func = NULL;
    }
#endif /* USE_UART0_MODULE */
#ifdef USE_UART3_MODULE
    if(channel == UART_CHANNEL_3)
    {
        UART3_STOP();
        uart3_rx_callback_func = NULL;
        uart3_tx_callback_func = NULL;
    }
#endif /* USE_UART3_MODULE */
}

#ifdef USE_UART0_MODULE
__interrupt static void HAL_Uart0_interrupt_send(void)
{
    volatile U8 rx_data = 0;
    volatile U8 errType = 0;

    errType = UART0_GET_SERIAL_STATUS();
    UART0_CLEAR_ERROR_FLAG(errType);

    if (errType != 0U)
    {
        //ERROR
    }
    
    rx_data = UART0_GET_RX_DATA();

    if (uart0_rx_callback_func != NULL)
    {
        uart0_rx_callback_func(rx_data);
    }
}

__interrupt static void HAL_Uart0_interrupt_receive(void)
{
    if (uart0_tx_callback_func != NULL)
    {
        uart0_tx_callback_func();
    }
}
#endif /* USE_UART0_MODULE */

#ifdef USE_UART3_MODULE
__interrupt static void HAL_Uart3_interrupt_send(void)
{
    if (uart3_tx_callback_func != NULL)
    {
        uart3_tx_callback_func();
    }
}

__interrupt static void HAL_Uart3_interrupt_receive(void)
{
    volatile U8 rx_data = 0;
    volatile U8 errType = 0;

    errType = UART3_GET_SERIAL_STATUS();
    UART3_CLEAR_ERROR_FLAG(errType);

    if (errType != 0U)
    {
        //ERROR
    }
    
    rx_data = UART3_GET_RX_DATA();

    if (uart3_rx_callback_func != NULL)
    {
        uart3_rx_callback_func(rx_data);
    }
}
#endif /* USE_UART3_MODULE */

