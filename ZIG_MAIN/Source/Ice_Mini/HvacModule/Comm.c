/*********************************************************************************/
/**
 * @file Comm.c
 * @brief Hvac 검토용 통신 처리 모듈
 * @author
 * @date 2025-11-14
 * @version 1.0
*/
/*********************************************************************************/

#include "Comm.h"
#include "Protocol.h"
#include "ProtocolDatafield.h"
#include "Global_Variable.h"
#include "Port_Define.h"
#include "Parser.h"

#include "M8_Ice_Making.h"

bit AT_F_TxStart;             //
bit AT_F_RxComplete;          //

bit F_AT_TX_Finish;
U8 gu8RxdBufferData;

U8 gu8_uart_test_mode;

/* 통신 구조체 */
typedef struct
{
    U16 u16TxMode;
    U8 u8TxData[255];
    U8 u8TxCounter;

    RX_MODE u8RxStatus;
    U8 u8RxData[255];
    U8 u8RxCounter;
} COMM_T;
COMM_T Comm;

void SetTxMode(U16 u16TxMode)
{
    Comm.u16TxMode |= u16TxMode;
}

void ClearTxMode(U16 u16TxMode)
{
    Comm.u16TxMode &= ~u16TxMode;
}

U16 CheckTxMode_Queue(void)
{
    U8 i = 0;
    for(i = 0; i < 16; i++)
    {
        if(Comm.u16TxMode & (1 << i))
        {
           return (Comm.u16TxMode & (1 << i));
        }
    }

    return 0;
}

/*********************************************************************************/
/**
 * @brief Hvac 검토용 통신 처리
 * @param void
 * @return void
*/
void Hvac_Comm_Process(void)
{
    Hvac_Rx();         // rx 처리
    Hvac_Tx();         // tx 처리
}

/*********************************************************************************/
/**
 * @brief Hvac 검토용 PC프로그램 수신 처리
 * @param void
 * @return void
*/
void Hvac_Rx(void)
{
    U8 u8cmd = 0;

    if(AT_F_RxComplete == CLEAR)
    {
        return;
    }
    else
    {
        AT_F_RxComplete = 0;
    }

    u8cmd = Comm.u8RxData[2];

    switch( u8cmd )
    {
        case PROTOCOL_F0_CMD:                    // 0xF0: 공통 시스템 데이터
            // 공통 시스템 데이터 처리
            SetTxMode(TX_CMD_F0);
            break;

        case PROTOCOL_F1_CMD:                    // 0xF1: 냉각 시스템 데이터
            // 냉각 시스템 데이터 처리
            SetTxMode(TX_CMD_F1);
            break;

        case PROTOCOL_B1_CMD:
            // 냉각테이블 변경 데이터 처리
            SetColdOnTemp(&Comm.u8RxData[5]);
            SetColdOffTemp(&Comm.u8RxData[7]);
            SetColdDelayTime(&(Comm.u8RxData[9] << 8 | Comm.u8RxData[10]));
            SetColdTargetRPS(&Comm.u8RxData[11]);

            SetTxMode(TX_CMD_B1);
            break;

        case PROTOCOL_B3_CMD:        // 0xB3: 제빙 테이블 변경
            SetFreezingTable(&Comm.u8RxData[5]);
            SetUsedFreezingTable(SET);

            SetTxMode(TX_CMD_F1);
            break;

        default:
            // 잘못된 명령어
            SetTxMode(TX_CMD_F0);
            break;
    }

    AT_F_TxStart = SET;
}

/**
 * @brief Hvac 검토용 PC프로그램 송신 처리
 * @param void
 * @return void
*/
U16 u16DebugTxMode = 0;
void Hvac_Tx(void)
{
    U16 mu16_cal_crc = 0;
    U8 mu8_i = 0;
    U8 mu8_cmd = 0;
    U8 mu8_data_length = 0;
    U16 mu16_tx_mode = 0;

    if(AT_F_TxStart == CLEAR)
    {
        return;
    }
    else
    {
        AT_F_TxStart = 0;
    }

    if(CheckTxMode_Queue() == FALSE)
    {
        return;
    }
    else
    {
        mu16_tx_mode = CheckTxMode_Queue();
        u16DebugTxMode = mu16_tx_mode;
        ClearTxMode(mu16_tx_mode);
    }

    // 수신된 CMD 확인
    mu8_cmd = Comm.u8RxData[2];

    // 헤더 설정
    Comm.u8TxData[0] = WORK_STX;
    Comm.u8TxData[1] = WORK_ID_MAIN;
    Comm.u8TxData[2] = ConvertTxModeToCmd(mu16_tx_mode);             // CMD 생성
    Comm.u8TxData[3] = ConvertTxModeToLength(mu16_tx_mode);          // LEN 생성
    mu8_data_length = ConvertTxModeToLength(mu16_tx_mode);
    Parse_DataField(mu16_tx_mode, &Comm.u8TxData[4]);

    // CRC 계산 범위: STX부터 DATA 끝까지 (CRC와 ETX 제외)
    // 패킷 구조: STX(1) + ID(1) + CMD(1) + LEN(1) + DATA(N) + CRC_HIGH(1) + CRC_LOW(1) + ETX(1)
    // CRC 계산: STX부터 DATA 끝까지 = 4 + mu8_data_length
    mu16_cal_crc = CRC_Cal_V2(Comm.u8TxData, (U8)(4 + mu8_data_length));
    Comm.u8TxData[4 + mu8_data_length] = (U8)HighByte(mu16_cal_crc);
    Comm.u8TxData[4 + mu8_data_length + 1] = (U8)LowByte(mu16_cal_crc);
    Comm.u8TxData[4 + mu8_data_length + 2] = WORK_ETX;

    USER_TXD = Comm.u8TxData[Comm.u8TxCounter];               // 첫 번째 바이트 전송 시작
    Comm.u8TxCounter++;

    F_AT_TX_Finish = 1;
}

/**
 * @brief Hvac 검토용 PC프로그램 송신 인터럽트 처리
 * @param void
 * @return void
*/
void HvacInterrupt_Tx(void)
{
//  TXD1=0x20;

    NOP();
    NOP();
    NOP();
    NOP();

    if(F_AT_TX_Finish == SET)
    {
        // 바이트 단위로 전송
        USER_TXD = Comm.u8TxData[Comm.u8TxCounter];

        // 패킷 길이 계산: WORK_PACKET_BASIC_LENGTH(7) + 데이터 길이(Comm.u8TxData[3])
        // 전체 패킷: STX(1) + ID(1) + CMD(1) + LEN(1) + DATA(N) + CRC_HIGH(1) + CRC_LOW(1) + ETX(1)
        // 마지막 인덱스 = 7 + N - 1 = 6 + N
        if(Comm.u8TxCounter < ((WORK_PACKET_BASIC_LENGTH + Comm.u8TxData[3] - 1)))
        {
            Comm.u8TxCounter++;
        }
        else
        {
            Comm.u8TxCounter = 0;
            F_AT_TX_Finish = 0;
        }
    }
}


/**
 * @brief Hvac 검토용 PC프로그램 수신 인터럽트 처리
 * @param void
 * @return void
*/
U16 u16RxDataDebug;
void HvacInterrupt_Rx(void)
{
    U8 err_type03;
    U16 mu16_cal_crc;
    mu16_cal_crc = 0;

    err_type03 = (U8)(SSR13 & 0x0007);
    SIR13 = (U16)err_type03;

    gu8RxdBufferData = USER_RXD;

    switch(Comm.u8RxStatus)
    {
        // 占쏙옙占?
        case UART_MODE_IDLE:

            if(gu8RxdBufferData == WORK_STX)
            {                 // STX check 0x01
                Comm.u8RxCounter = 0;
                Comm.u8RxStatus = UART_MODE_RECEIVE;            // 0x01占쏙옙 占쏙옙占쏙옙占쏙옙 '占쏙옙占쏙옙占쏙옙'占쏙옙占쏙옙
                Comm.u8RxData[Comm.u8RxCounter++] = gu8RxdBufferData;// Stx 카占쏙옙트 0
            }
            else
            {
                Comm.u8RxCounter = 0;
            }

             break;

         // 占쏙옙占쏙옙占쏙옙
        case UART_MODE_RECEIVE:
            Comm.u8RxData[Comm.u8RxCounter++] = gu8RxdBufferData;

            // ETX를 받았을 때 CRC 검증
            if(gu8RxdBufferData == WORK_ETX)
            {
                // 최소 패킷 길이 확인: STX(1) + TX_ID(1) + CMD(1) + LEN(1) + CRC_HIGH(1) + CRC_LOW(1) + ETX(1) = 7바이트
                if(Comm.u8RxCounter >= 7)
                {
                    // 패킷 길이 확인
                    u16RxDataDebug = (Comm.u8RxData[3] + WORK_PACKET_BASIC_LENGTH);
                    if(Comm.u8RxCounter == (Comm.u8RxData[3] + WORK_PACKET_BASIC_LENGTH))
                    {
                        mu16_cal_crc = CRC_Cal_V2(Comm.u8RxData, (Comm.u8RxCounter-3));

                        // CRC_HIGH와 CRC_LOW 비교
                        if(Comm.u8RxData[Comm.u8RxCounter-3] == (U8)HighByte(mu16_cal_crc)
                        && Comm.u8RxData[Comm.u8RxCounter-2] == (U8)LowByte(mu16_cal_crc))
                        {
                            if(Comm.u8RxData[2] == 0xB3)
                            {
                                Comm.u8RxCounter = 0;
                            }

                            AT_F_RxComplete = 1;
                            // Rx data initialize
                            Comm.u8RxCounter = 0;
                            Comm.u8RxStatus = UART_MODE_IDLE;
                        }
                        else
                        {
                            // CRC 불일치: 버퍼 초기화하고 IDLE 모드로 전환
                            AT_F_RxComplete = 0;
                            Comm.u8RxCounter = 0;
                            Comm.u8RxStatus = UART_MODE_IDLE;
                        }
                    }
                    else
                    {

                    }
                }
                else
                {
                    // 패킷 길이 부족: 버퍼 초기화하고 IDLE 모드로 전환
                    AT_F_RxComplete = 0;
                    Comm.u8RxCounter = 0;
                    Comm.u8RxStatus = UART_MODE_IDLE;
                }
            }
            break;

        // Error
        case UART_MODE_ERROR:

             Comm.u8RxCounter = 0;
             Comm.u8RxStatus = UART_MODE_IDLE;

             break;


        default:  // Rx data initialize //

             Comm.u8RxCounter = 0;
             Comm.u8RxStatus = UART_MODE_IDLE;

             break;
    }
}
