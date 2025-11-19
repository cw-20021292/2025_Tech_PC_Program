/**
 * File : App_Comm_CLI.h
 *
 * Application Programming Interface
 * Depend on API
*/

#ifndef __APP_COMM_CLI_H__
#define __APP_COMM_CLI_H__

#include "api_uart.h"

//#define USE_COMM_CLI

#define PROMPT	"COWAY> "
#define BUFFER_MAX_LENGTH    128

typedef struct {
    U8 idx;
    char buffer[BUFFER_MAX_LENGTH];
} CLI_Info;

typedef struct _tsCLICommand{
	char *mPtrCCmdName;
	int (*mPtrFunction)(uint8_t xArgc, void **xPtrArgv);
} sCLICommand;

U8 CLI_isValidPacket(U8 *buf);
void CLI_Packet_Handler(U8 data);


#endif /*__APP_COMM_CLI_H__*/

