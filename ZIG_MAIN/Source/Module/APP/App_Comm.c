/**
 * File : App_Comm.c
 * 
 * User Application
 * Depend on API
*/

#include "App_Comm.h"

#include "App_Comm_Protocol.h"

tsCommInfo CommInfo = {0, };

static U16 Rx_CRC_CCITT(U8 *puchMsg, U16 usDataLen)
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

static void Comm_Send_Packet_Handler(void)
{
    if(CommInfo.comm_state == COMM_STATE_TRANSMIT)
    {
        Uart_Send_Buffer(UART_CHANNEL_3, (const char *)CommInfo.comm_tx_buffer, CommInfo.comm_tx_index);
        CommInfo.comm_state = COMM_STATE_IDLE;
    }
}

static U8 Comm_isValidPacket(U8 *buf)
{
    U16 calculated_crc = 0;
    U16 received_crc = 0;
    U16 packet_length = 0;

    if(buf[0] != COMM_STX)
    {
        return FALSE;
    }

    packet_length = (PROTOCOL_IDX_LENGTH+1) + buf[PROTOCOL_IDX_LENGTH] + 3;

    if(packet_length < COMM_PACKET_BASIC_LENGTH || packet_length > UART3_RX_BUFFER_SIZE)
    {
        return FALSE;
    }

    if(buf[packet_length - 1] != COMM_ETX)
    {
        return FALSE;
    }

    calculated_crc = Rx_CRC_CCITT(buf, (U16)(packet_length - 3));

    if( buf[packet_length-3] != GET_16_HIGH_BYTE(calculated_crc) || buf[packet_length-2] != GET_16_LOW_BYTE(calculated_crc) )
    {
        return FALSE;
    }

    return TRUE;
}

static U8 Comm_Make_Ack_Packet(U8 *buf)
{
    U8 data_length = 0;
    U16 calculated_crc = 0;
    
    CommInfo.comm_tx_buffer[PROTOCOL_IDX_STX] = COMM_STX;
    CommInfo.comm_tx_buffer[PROTOCOL_IDX_ID] = COMM_ID_MAIN;
    CommInfo.comm_tx_buffer[PROTOCOL_IDX_CMD] = buf[PROTOCOL_IDX_CMD];
    
    data_length = Protocol_Make_Ack_Packet(buf, &CommInfo.comm_tx_buffer[PROTOCOL_IDX_DATA]);
    CommInfo.comm_tx_buffer[PROTOCOL_IDX_LENGTH] = data_length;
    
    calculated_crc = Rx_CRC_CCITT(CommInfo.comm_tx_buffer, (U8)(4 + data_length));
    CommInfo.comm_tx_buffer[4 + data_length] = GET_16_HIGH_BYTE(calculated_crc);
    CommInfo.comm_tx_buffer[4 + data_length + 1] = GET_16_LOW_BYTE(calculated_crc);
    CommInfo.comm_tx_buffer[4 + data_length + 2] = COMM_ETX;

    CommInfo.comm_tx_index = 4 + data_length + 2 + 1;
    
    return TRUE;
}

static void Comm_Rcv_Packet_Handler(void)
{
    U8 data = 0;

    while(Uart_Read_Data(UART_CHANNEL_3, &data) == TRUE)
    {
        switch(CommInfo.comm_state)
        {
            case COMM_STATE_IDLE :
                if( data == COMM_STX )
                {
                    //_MEMSET_((void __FAR*)&CommInfo, 0, sizeof(tsCommInfo));
                    CommInfo.comm_state = COMM_STATE_RECEIVING;
                    CommInfo.comm_rx_index = 0;
                    CommInfo.comm_rx_buffer[CommInfo.comm_rx_index++] = data;
                }
                break;

            case COMM_STATE_RECEIVING :
                CommInfo.comm_rx_buffer[CommInfo.comm_rx_index++] = data;

                if(data == COMM_ETX)
                {
                    if(Comm_isValidPacket(CommInfo.comm_rx_buffer) == TRUE)
                    {
                        Comm_Make_Ack_Packet(CommInfo.comm_rx_buffer);
                        CommInfo.comm_state = COMM_STATE_TRANSMIT;
                        CommInfo.comm_rx_index = 0;
                    }
                    else
                    {
                        CommInfo.comm_state = COMM_STATE_IDLE;
                        CommInfo.comm_rx_index = 0;
                    }
                }
                else if(CommInfo.comm_rx_index > UART3_RX_BUFFER_SIZE)
                {
                    CommInfo.comm_state = COMM_STATE_IDLE;
                    CommInfo.comm_rx_index = 0;
                }
                break;
        }

    }
}

void Comm_Packet_Handler(void)
{
    Comm_Rcv_Packet_Handler();
    Comm_Send_Packet_Handler();
}

