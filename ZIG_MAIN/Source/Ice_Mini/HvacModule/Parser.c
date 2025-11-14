/*********************************************************************************/
/**
 * @file Parser.c
 * @brief 프로토콜 모드를 명령어로 변환
 * @author
 * @date 2025-11-14
 * @version 1.0
*/
/*********************************************************************************/
#include "Parser.h"
#include "Protocol.h"
#include "ProtocolDatafield.h"
#include "Comm.h"
/*********************************************************************************/


/*********************************************************************************/
/**
 * @brief 프로토콜 모드를 명령어로 변환
 * @param u16TxMode: 프로토콜 모드
 * @return U8: 명령어
*/
U8 ConvertTxModeToCmd(U16 u16TxMode)
{
    U8 u8Cmd = 0;

    switch(u16TxMode)
    {
        case TX_CMD_F0:
            u8Cmd = PROTOCOL_F0_CMD;
            break;

        case TX_CMD_F1:
        case TX_CMD_B3:
            u8Cmd = PROTOCOL_F1_CMD;
            break;

        default:
            u8Cmd = 0;
            break;
    }
    return u8Cmd;
}

/*********************************************************************************/
/**
 * @brief 프로토콜 모드를 길이로 변환
 * @param u16TxMode: 프로토콜 모드
 * @return U8: 길이
*/
U8 ConvertTxModeToLength(U16 u16TxMode)
{
    U8 u8Length = 0;
    switch(u16TxMode)
    {
        case TX_CMD_F0:
            u8Length = PROTOCOL_F0_LENGTH;
            break;
        case TX_CMD_F1:
        case TX_CMD_B3:
            u8Length = PROTOCOL_F1_LENGTH;
            break;

        default:
            u8Length = 0;
            break;
    }
    return u8Length;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U16 CRC_Cal_V2(U8 *puchMsg, U8 usDataLen)
{
    U8 i = 0;
    unsigned short  wCRCin = 0x0000;
    unsigned short  wCPoly = 0x1021;
    U16 wChar = 0;

    while (usDataLen--)
    {
        wChar = *(puchMsg++);
        wCRCin ^= (wChar << 8);

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
