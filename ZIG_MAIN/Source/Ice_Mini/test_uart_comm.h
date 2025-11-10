/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _TEST_UART_COMM_H_
#define _TEST_UART_COMM_H_

//
extern U16 HexToAsc(U8 mu8RxdData);
extern U8 AscToHex(U8 mu8RxdData1, U8 mu8RxdData2);
extern U16 DecToAsc(U8 mu8RxdData);



#define UART_MODE_IDLE             0            //
#define UART_MODE_RECEIVE          1
#define UART_MODE_ERROR            2









extern U8 gu8UARTData[35];



#endif
