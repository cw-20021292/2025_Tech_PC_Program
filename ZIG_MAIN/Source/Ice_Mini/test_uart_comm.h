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

void AT_UART_Communication(void);
//
extern U16 HexToAsc(U8 mu8RxdData);
extern U8 AscToHex(U8 mu8RxdData1, U8 mu8RxdData2);
extern U16 DecToAsc(U8 mu8RxdData);


typedef enum
{
    UART_MODE_IDLE = 0,
    UART_MODE_RECEIVE = 1,
    UART_MODE_ERROR = 2,
} RX_MODE;


/* 상태조회 커맨드 */
#define TX_CMD_F0 0x01  // 공통 시스템 상태조회
#define TX_CMD_F1 0x02  // 냉각 시스템 상태조회
/* 냉동 관련 변경 커맨드 */
#define TX_CMD_B1 0x20
#define TX_CMD_B2 0x40
#define TX_CMD_B3 0x80
#define TX_CMD_B4 0x100

/* 패킷 구조 */
#define WORK_STX                        0x02  // 패킷 시작
#define WORK_ETX                        0x03  // 패킷 종료
#define WORK_ID_PC                      0x01  // PC 통신
#define WORK_ID_MAIN                    0x02  // 메인 통신
#define WORK_ID_FRONT                   0x03  // 프론트 통신
#define WORK_PACKET_BASIC_LENGTH        7     // 패킷 기본 길이

/* 공통 시스템 프로토콜 (응답) */
typedef enum
{
    PROTOCOL_F0_CMD = 0xF0,
    PROTOCOL_F0_LENGTH = 40,
} PROTOCOL_F0_COMMON_SYSTEM;

/* 냉각 시스템 프로토콜 (응답) */
typedef enum
{
    PROTOCOL_F1_CMD = 0xF1,
    PROTOCOL_F1_LENGTH = 76,
} PROTOCOL_F1_COLD_SYSTEM;

/* 히팅 시스템 프로토콜 (응답) */
typedef enum
{
    PROTOCOL_F2_CMD = 0xF2,
    PROTOCOL_F2_LENGTH = 0,
} PROTOCOL_F2_HEATING_SYSTEM;

typedef enum
{
    PROTOCOL_B3_CMD = 0xB3,
    PROTOCOL_B3_LENGTH = 0,
} PROTOCOL_B3_FREEZING_TABLE;

#endif
