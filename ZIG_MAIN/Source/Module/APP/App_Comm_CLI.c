/**
 * File : App_Comm_CLI.c
 *
 * Application Programming Interface
 * Depend on API
*/

#include "App_Comm_CLI.h"

#include "App_Comm.h"
#include "App_Comm_Protocol.h"
#include "api_debug.h"

CLI_Info cli_info = {0};

static int CLI_Cmd_Exit(uint8_t, void**);
static int CLI_Cmd_Test(uint8_t, void**);

const sCLICommand CLI_COMMAND[] =
{
	{"exit", CLI_Cmd_Exit},
	{"test", CLI_Cmd_Test},
	{NULL, NULL}
};

static int CLI_Cmd_Exit(U8 xUArgc, void **xPtrArgv)
{
    SetCommState(COMM_STATE_IDLE);

    return 0;
}

static int CLI_Cmd_Test(U8 xUArgc, void **xPtrArgv)
{
    if(xUArgc == 2)
    {
        if(!strcmp("1", xPtrArgv[1]))
        {
            dlog(SYSMOD, FATAL, ("CLI Test - FATAL : %d \r\n", 1) );
            dlog(SYSMOD, INFO, ("CLI Test - INFO : %d \r\n", 2) );
            dlog(SYSMOD, DATA, ("CLI Test - DATA : %d \r\n", 3) );
            dlog(SYSMOD, DBUG, ("CLI Test - DBUG : %d \r\n", 4) );
        }
    }

    return 0;
}

static U16 CLI_CRC_Cal(U8 *puchMsg, U16 usDataLen)
{
    U8 i = 0;
    U16 wCRCin = 0x0000;
    U16 wCPoly = 0x1021;
    U8 wChar = 0;

    while(usDataLen--)
    {
        wChar = *(puchMsg++);
        wCRCin ^= ((U16)wChar << 8);
        for(i = 0; i < 8; i++)
        {
            if (wCRCin & 0x8000)
            {
                wCRCin = (wCRCin << 1) ^ wCPoly;
            }
            else
            {
                wCRCin = wCRCin << 1;
            }
        }
    }

    return (wCRCin);
}

U8 CLI_isValidPacket(U8 *buf)
{
    U16 calculated_crc = 0;
    U16 received_crc = 0;
    U16 packet_length = 0;

    if(buf[0] != COMM_PROTOCOL_STX)
    {
        return FALSE;
    }

    packet_length = (PROTOCOL_IDX_LENGTH+1) + buf[PROTOCOL_IDX_LENGTH] + 3;

    if(packet_length < COMM_PROTOCOL_PACKET_BASIC_LENGTH || packet_length > UART3_RX_BUFFER_SIZE)
    {
        return FALSE;
    }

    if(buf[packet_length - 1] != COMM_PROTOCOL_ETX)
    {
        return FALSE;
    }

    calculated_crc = CLI_CRC_Cal(buf, (U16)(packet_length - 3));

    if( buf[packet_length-3] != GET_16_HIGH_BYTE(calculated_crc) || buf[packet_length-2] != GET_16_LOW_BYTE(calculated_crc) )
    {
        return FALSE;
    }

    if(buf[PROTOCOL_IDX_CMD] == 0xFF)
    {
        return TRUE;
    }

    return FALSE;
}

static void CLI_Parse_Command(void)
{
    char *ptrArgv[128];
    U8 uArgc = 0;

    U8 i = 0;
    U8 in_word = FALSE;

    if (cli_info.idx == 0)
    {
        return;
    }
    for (i = 0; i < cli_info.idx; i++)
    {
        if (cli_info.buffer[i] == ' ')
        {
            cli_info.buffer[i] = '\0';
            in_word = FALSE;
        }
        else
        {
            if (in_word == FALSE)
            {
                in_word = TRUE;

                if (uArgc < 128)
                {
                    ptrArgv[uArgc] = (char*)&cli_info.buffer[i];
                    uArgc++;
                }
                else
                {
                    return;
                }
            }
        }
    }

    if (uArgc == 0)
    {
        return;
    }

    for (i = 0; CLI_COMMAND[i].mPtrFunction != NULL; i++)
    {
        if (strcmp(CLI_COMMAND[i].mPtrCCmdName, ptrArgv[0]) == 0)
        {
            CLI_COMMAND[i].mPtrFunction(uArgc, ptrArgv);
            return;
        }
    }
}

void CLI_Packet_Handler(U8 data)
{
    if(data == 13)
    {
        if(cli_info.idx > 0)
        {
        	CLI_Parse_Command();
        }

        _MEMSET_(cli_info.buffer, 0, BUFFER_MAX_LENGTH);
        cli_info.idx = 0;
        Uart_Send_String(UART_CHANNEL_3, "\n\r" PROMPT);
    }
    else if(data == 8 && cli_info.idx != 0)
    {
        cli_info.idx--;

        Uart_Send_String(UART_CHANNEL_3, "\b \b");
    }
    else if ((data >= ' ') && (data <= '~'))
    {
        if(cli_info.idx < BUFFER_MAX_LENGTH)
        {
            cli_info.buffer[cli_info.idx++] = data;
        }

        Uart_Send_Data(UART_CHANNEL_3, data);
    }
}

