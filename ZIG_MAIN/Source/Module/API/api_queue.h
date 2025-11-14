/**
 * File : api_queue.h
 * 
 * Application Programming Interface
 * 
*/

#ifndef __API_QUEUE_H__
#define __API_QUEUE_H__

#include "macrodriver.h"

typedef struct {
    volatile U8 *buffer;
    U16 size;
    volatile U16 head;
    volatile U16 tail;
} CircularQueue;

void Queue_Init(CircularQueue* queue, U8 *buffer_array, U16 buffer_size);
U8 Queue_Enqueue(CircularQueue* queue, uint8_t data);
U8 Queue_Dequeue(CircularQueue* queue, uint8_t* data);
U8 Queue_IsEmpty(const CircularQueue* queue);
U8 Queue_IsFull(const CircularQueue* queue);


#endif /*__API_QUEUE_H__*/

