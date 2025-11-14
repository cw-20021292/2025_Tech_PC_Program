/**
 * File : api_uart.c
 * 
 * Application Programming Interface
 * Depend on HAL
*/

#include "api_uart.h"
#include "api_queue.h"

#ifdef USE_UART0_MODULE
static uint8_t uart0_rx_buffer_array[UART0_RX_BUFFER_SIZE];
static uint8_t uart0_tx_buffer_array[UART0_TX_BUFFER_SIZE];
CircularQueue uart0_rx_queue = {0};
CircularQueue uart0_tx_queue = {0};

static volatile U8 uart0_tx_complete = TRUE;

static void Uart0RecvCallbackFunc(U8 rx_data);
static void Uart0SendCallbackFunc(void);
#endif /* USE_UART0_MODULE */

#ifdef USE_UART3_MODULE
static uint8_t uart3_rx_buffer_array[UART3_RX_BUFFER_SIZE];
static uint8_t uart3_tx_buffer_array[UART3_TX_BUFFER_SIZE];
CircularQueue uart3_rx_queue = {0};
CircularQueue uart3_tx_queue = {0};

static volatile U8 uart3_tx_complete = TRUE;

static void Uart3RecvCallbackFunc(U8 rx_data);
static void Uart3SendCallbackFunc(void);
#endif /* USE_UART3_MODULE */

void Uart_Init(void)
{
#ifdef USE_UART0_MODULE
    Queue_Init(&uart0_rx_queue, uart0_rx_buffer_array, sizeof(uart0_rx_buffer_array));
    Queue_Init(&uart0_tx_queue, uart0_tx_buffer_array, sizeof(uart0_tx_buffer_array));
    uart0_tx_complete = TRUE;
    HAL_Uart_Init(UART_CHANNEL_0, Uart0RecvCallbackFunc, Uart0SendCallbackFunc);
#endif /* USE_UART0_MODULE */

#ifdef USE_UART3_MODULE
    Queue_Init(&uart3_rx_queue, uart3_rx_buffer_array, sizeof(uart3_rx_buffer_array));
    Queue_Init(&uart3_tx_queue, uart3_tx_buffer_array, sizeof(uart3_tx_buffer_array));
    uart3_tx_complete = TRUE;
    HAL_Uart_Init(UART_CHANNEL_3, Uart3RecvCallbackFunc, Uart3SendCallbackFunc);
#endif /* USE_UART3_MODULE */
}

#ifdef USE_UART0_MODULE
static void Uart0RecvCallbackFunc(U8 rx_data)
{
    Queue_Enqueue(&uart0_rx_queue, rx_data);
}

static void Uart0SendCallbackFunc(void)
{
    if(Queue_IsEmpty(&uart0_tx_queue) == FALSE)
    {
        U8 data = 0;
        Queue_Dequeue(&uart0_tx_queue, &data);
        UART0_SET_TX_DATA(data);
    }
    else
    {
        uart0_tx_complete = TRUE;
    }
}
#endif /* USE_UART0_MODULE */

#ifdef USE_UART3_MODULE
static void Uart3RecvCallbackFunc(U8 rx_data)
{
    Queue_Enqueue(&uart3_rx_queue, rx_data);
}

static void Uart3SendCallbackFunc(void)
{
    if(Queue_IsEmpty(&uart3_tx_queue) == FALSE)
    {
        U8 data = 0;
        Queue_Dequeue(&uart3_tx_queue, &data);
        UART3_SET_TX_DATA(data);
    }
    else
    {
        uart3_tx_complete = TRUE;
    }
}
#endif /* USE_UART3_MODULE */

static CircularQueue* Get_Uart_Queue(teUART_CHANNEL channel, teUART_DIRECTION direction)
{
#ifdef USE_UART0_MODULE
    if(channel == UART_CHANNEL_0)
    {
        if(direction == UART_DIR_RX)
        {
            return &uart0_rx_queue;
        }
        else if(direction == UART_DIR_TX)
        {
            return &uart0_tx_queue;
        }
    }
#endif /* USE_UART0_MODULE */
#ifdef USE_UART3_MODULE
    if(channel == UART_CHANNEL_3)
    {
        if(direction == UART_DIR_RX)
        {
            return &uart3_rx_queue;
        }
        else if(direction == UART_DIR_TX)
        {
            return &uart3_tx_queue;
        }
    }
#endif /* USE_UART3_MODULE */

    return NULL;
}

U8 Uart_Read_Data(teUART_CHANNEL channel, U8* data)
{
    U8 result = FALSE;
    CircularQueue *queue = Get_Uart_Queue(channel, UART_DIR_RX);

    if (queue == NULL)
    {
        return;
    }

    DI();

    result = Queue_Dequeue(queue, data);

    EI();

    return result;
}

static void Uart_Send_Trigger(teUART_CHANNEL channel)
{
    U8 data = 0;
    
    DI();

#ifdef USE_UART0_MODULE
    if(channel == UART_CHANNEL_0)
    {
        if( (Queue_IsEmpty(&uart0_tx_queue) == FALSE) && (uart0_tx_complete == TRUE) )
        {
            uart0_tx_complete = FALSE;

            Queue_Dequeue(&uart0_tx_queue, &data);
            UART0_SET_TX_DATA(data);
        }
    }
#endif /* USE_UART0_MODULE */
#ifdef USE_UART3_MODULE
    if(channel == UART_CHANNEL_3)
    {
        if( (Queue_IsEmpty(&uart3_tx_queue) == FALSE) && (uart3_tx_complete == TRUE) )
        {
            uart3_tx_complete = FALSE;

            Queue_Dequeue(&uart3_tx_queue, &data);
            UART3_SET_TX_DATA(data);
        }
    }
#endif /* USE_UART3_MODULE */

    EI();

}

void Uart_Send_Data(teUART_CHANNEL channel, U8 data)
{
    U16 timeout = 1000;
    CircularQueue *queue = Get_Uart_Queue(channel, UART_DIR_TX);

    if (queue == NULL)
    {
        return;
    }

    while(Queue_IsFull(queue) == TRUE)
    {
        timeout--;
        if(timeout == 0)
        {
            break;
        }
    }

    if(Queue_IsFull(queue) == FALSE)
    {
        DI();
        Queue_Enqueue(queue, data);
        EI();
    }

    Uart_Send_Trigger(channel);
}

void Uart_Send_String(teUART_CHANNEL channel, const char *str_buf)
{
    U16 len = 0;
    U16 index = 0;
    U16 timeout = 1000;
    CircularQueue *queue = Get_Uart_Queue(channel, UART_DIR_TX);

    len = strlen(str_buf); 

    for(index = 0; index < len; index++)
    {
        timeout = 1000;
        while(Queue_IsFull(queue) == TRUE)
        {
            timeout--;
            if(timeout == 0)
            {
                break;
            }
        }

        if(Queue_IsFull(queue) == FALSE)
        {
            DI();
            Queue_Enqueue(queue, str_buf[index]);
            EI();
        }
        else
        {
            break;
        }
    }

    Uart_Send_Trigger(channel);
}

void Uart_Send_Buffer(teUART_CHANNEL channel, const char *str_buf, U16 length)
{
    U16 index = 0;
    U16 timeout = 1000;
    CircularQueue *queue = Get_Uart_Queue(channel, UART_DIR_TX);

    for(index = 0; index < length; index++)
    {
        timeout = 1000;
        while(Queue_IsFull(queue) == TRUE)
        {
            timeout--;
            if(timeout == 0)
            {
                break;
            }
        }

        if(Queue_IsFull(queue) == FALSE)
        {
            DI();
            Queue_Enqueue(queue, str_buf[index]);
            EI();
        }
        else
        {
            break;
        }
    }

    Uart_Send_Trigger(channel);
}

