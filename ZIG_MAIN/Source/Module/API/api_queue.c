/**
 * File : api_queue.c
 * 
 * 
 *
*/

#include "api_queue.h"

void Queue_Init(CircularQueue* queue, U8 *buffer_array, U16 buffer_size)
{
    queue->buffer = buffer_array;
    queue->size = buffer_size;
    queue->head = 0;
    queue->tail = 0;
}

U8 Queue_IsEmpty(const CircularQueue* queue)
{
    if(queue->head == queue->tail)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

U8 Queue_IsFull(const CircularQueue* queue)
{
    if( ((queue->tail + 1) % queue->size) == (queue->head) )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

U8 Queue_Enqueue(CircularQueue* queue, uint8_t data)
{
    if (Queue_IsFull(queue) == TRUE) 
    {
        return FALSE;
    }

    queue->tail = (queue->tail + 1) % queue->size;
    queue->buffer[queue->tail] = data;

    return TRUE;
}

U8 Queue_Dequeue(CircularQueue* queue, uint8_t* data)
{
    if (Queue_IsEmpty(queue) == TRUE)
    {
        return FALSE;
    }

    queue->head = (queue->head + 1) % queue->size;
    *data = queue->buffer[queue->head];

    return TRUE;
}


