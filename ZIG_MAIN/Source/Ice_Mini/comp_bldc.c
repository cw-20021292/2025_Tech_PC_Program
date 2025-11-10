/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Main.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  : AT+CONNECT=74F07DB01010
***********************************************************************************************************************/
#include    "Macrodriver.h"
#include    "Global_Variable.h"
#include    "Port_Define.h"
#include    "comp_bldc.h"

void Bldc_Comp_Communication(void);
void bldc_rx_communication(void);
void receive_bldc_data(U8 *mu8_rx_data);
void Uart_ISR2_Bldc_Comp_Rx(void);
void bldc_tx_communication(void);
void bldc_transmit_error_check(void);
void bldc_tx_start(void);
void make_bldc_tx_packet(void);
void Uart_ISR2_Bldc_Comp_Tx(void);
U8 bldc_tx_crc( U8 *puchMsg );
U8 bldc_rx_crc( U8 *puchMsg );
void clear_bldc_buffer(void);
void bldc_comm_error_check(void);

U8 gu8_bldc_rx_buffer[BLDC_COMP_RX_LENGTH];
U8 gu8_bldc_tx_buffer[BLDC_COMP_TX_LENGTH];

U8 gu8_bldc_tx_count;
bit bit_bldc_tx_request;
bit bit_bldc_rx_complete;
U8 gu8_bldc_rx_receive_cnt;
U8 gu8_bldc_cs_error_cnt;
U16 gu16_bldc_comm_error_timer;
U8 gu8_bldc_retry_count;
U8 gu8_bldc_comm_error_count;
U8 gu8_bldc_target_hz;
U8 gu8_bldc_error_code;
U8 gu8_bldc_opration_hz;
//U8 gu8_bldc_temperature;

U8 gu8_bldc_tx_timer;

bit bit_temporary_comm_error;
bit bit_temporary_operation_error;

U8 gu8_comm_error_retry_count;

U8 gu8_bldc_self_error_e81_samsung_e2_retry_count;
U8 gu8_bldc_self_error_e82_samsung_e1_retry_count;
U8 gu8_bldc_self_error_e83_samsung_e3_retry_count;
U8 gu8_bldc_self_error_e84_samsung_e5_retry_count;
U8 gu8_bldc_self_error_e85_samsung_e6_retry_count;
U8 gu8_bldc_self_error_e86_samsung_e4_retry_count;


U8 gu8_bldc_self_count;
U16 gu16_bldc_self_release_count;


U16 gu16_comp_error_check_timer;
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void Bldc_Comp_Communication(void)
{
    bldc_rx_communication();
    bldc_tx_communication();

    bldc_comm_error_check();
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void bldc_rx_communication(void)
{
    U8 mu8_cal_crc = 0;

    if( bit_bldc_rx_complete == SET )
    {
        mu8_cal_crc = bldc_rx_crc( gu8_bldc_rx_buffer );

        if( gu8_bldc_rx_buffer[ BLDC_RX_DATA_CHECK_SUM ] == mu8_cal_crc )
        {
            bit_bldc_tx_request = SET;
            receive_bldc_data( gu8_bldc_rx_buffer );

            /*..hui [23-7-4���� 10:42:52] ������ �ѹ��̶� �°� ������ ī��Ʈ �ʱ�ȭ..*/
            gu8_comm_error_retry_count = 0;

            /*..hui [24-11-25���� 10:03:01] üũ������ �������� ������ ���������� �ʱ�ȭ..*/
            /*gu16_bldc_comm_error_timer = 0;*/
        }
        else
        {
            gu8_bldc_cs_error_cnt++;
        }

        bit_bldc_rx_complete = CLEAR;
        gu16_bldc_comm_error_timer = 0;
    }
    else
    {
        gu16_bldc_comm_error_timer++;

        if(gu16_bldc_comm_error_timer >= 300)
        {
            gu16_bldc_comm_error_timer = 300;

            if( bit_temporary_comm_error == CLEAR )
            {
                bit_temporary_comm_error = SET;
                gu8_comm_error_retry_count++;

                /*if( gu8_comm_error_retry_count >= 3 )*/
                /*..hui [24-11-18���� 1:27:11] ��ſ��� ��õ� Ƚ�� 10ȸ�� ����..*/
                if( gu8_comm_error_retry_count >= 10 )
                {
                    Bit7_BLDC_Communication_Error__E27 = SET;
                }
                else{}
            }
            else{}
        }
        else{}
    }
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void receive_bldc_data(U8 *mu8_rx_data)
{
    gu8_bldc_error_code = mu8_rx_data[BLDC_RX_DATA_ERROR_CODE];
    gu8_bldc_opration_hz = mu8_rx_data[BLDC_RX_DATA_OPERATION_FREQUENCY];
    //gu8_bldc_temperature = mu8_rx_data[BLDC_RX_DATA_TEMPERATURE];

    /*..hui [24-1-11���� 2:09:24] COMP �Ѱ� 1�� �ĺ��� ���� �˻�.. ���ν� �����缭..*/
    if( gu16_comp_error_check_timer >= 600 )
    {
        if( gu8_bldc_error_code > 0 )
        {
            gu8_bldc_self_count++;
            gu16_bldc_self_release_count = 0;

            if( gu8_bldc_self_count >= 10 )
            {
                gu8_bldc_self_count = 10;

                if( bit_temporary_operation_error == CLEAR )
                {
                    bit_temporary_operation_error = SET;

                    if( gu8_bldc_error_code == SAMSUNG_BLDC_COMP_E81_CURRENT_SENSING_ERROR )
                    {
                        gu8_bldc_self_error_e81_samsung_e2_retry_count++;

                        if( gu8_bldc_self_error_e81_samsung_e2_retry_count >= BLDC_COMP_ERROR_RETRY_COUNT )
                        {
                            gu8_bldc_self_error_e81_samsung_e2_retry_count = BLDC_COMP_ERROR_RETRY_COUNT;
                            Bit8_BLDC_Operation_Error_Current_Sensing__E81 = SET;
                            gu8_memento_e29_detail_code = gu8_bldc_error_code;
                        }
                        else{}
                    }
                    else if( gu8_bldc_error_code == SAMSUNG_BLDC_COMP_E82_STARTING_FAIL_ERROR )
                    {
                        gu8_bldc_self_error_e82_samsung_e1_retry_count++;

                        if( gu8_bldc_self_error_e82_samsung_e1_retry_count >= BLDC_COMP_ERROR_RETRY_COUNT )
                        {
                            gu8_bldc_self_error_e82_samsung_e1_retry_count = BLDC_COMP_ERROR_RETRY_COUNT;
                            Bit9_BLDC_Operation_Error_Starting_Fail__E82 = SET;
                            gu8_memento_e29_detail_code = gu8_bldc_error_code;
                        }
                        else{}
                    }
                    else if( gu8_bldc_error_code == SAMSUNG_BLDC_COMP_E83_OVER_CURRENT_ERROR )
                    {
                        gu8_bldc_self_error_e83_samsung_e3_retry_count++;

                        if( gu8_bldc_self_error_e83_samsung_e3_retry_count >= BLDC_COMP_ERROR_RETRY_COUNT )
                        {
                            gu8_bldc_self_error_e83_samsung_e3_retry_count = BLDC_COMP_ERROR_RETRY_COUNT;
                            Bit10_BLDC_Operation_Error_Over_Current__E83 = SET;
                            gu8_memento_e29_detail_code = gu8_bldc_error_code;
                        }
                        else{}
                    }
                    else if( gu8_bldc_error_code == SAMSUNG_BLDC_COMP_E84_OVERHEAT_ERROR )
                    {
                        gu8_bldc_self_error_e84_samsung_e5_retry_count++;

                        if( gu8_bldc_self_error_e84_samsung_e5_retry_count >= BLDC_COMP_ERROR_RETRY_COUNT )
                        {
                            gu8_bldc_self_error_e84_samsung_e5_retry_count = BLDC_COMP_ERROR_RETRY_COUNT;
                            Bit11_BLDC_Operation_Error_Overheat__E84 = SET;
                            gu8_memento_e29_detail_code = gu8_bldc_error_code;
                        }
                        else{}
                    }
                    else if( gu8_bldc_error_code == SAMSUNG_BLDC_COMP_E85_DISCONNECTION_COMM_ERROR )
                    {
                        gu8_bldc_self_error_e85_samsung_e6_retry_count++;

                        if( gu8_bldc_self_error_e85_samsung_e6_retry_count >= BLDC_COMP_ERROR_RETRY_COUNT )
                        {
                            gu8_bldc_self_error_e85_samsung_e6_retry_count = BLDC_COMP_ERROR_RETRY_COUNT;
                            Bit12_BLDC_Operation_Error_Disconnection_Comm__E85 = SET;
                            gu8_memento_e29_detail_code = gu8_bldc_error_code;
                        }
                        else{}
                    }
                    else if( gu8_bldc_error_code == SAMSUNG_BLDC_COMP_E86_ABNORMAL_VOLTAGE_ERROR )
                    {
                        gu8_bldc_self_error_e86_samsung_e4_retry_count++;

                        if( gu8_bldc_self_error_e86_samsung_e4_retry_count >= BLDC_COMP_ERROR_RETRY_COUNT )
                        {
                            gu8_bldc_self_error_e86_samsung_e4_retry_count = BLDC_COMP_ERROR_RETRY_COUNT;
                            Bit13_BLDC_Operation_Error_Abnormal_Voltage__E86 = SET;
                            gu8_memento_e29_detail_code = gu8_bldc_error_code;
                        }
                        else{}
                    }
                    else
                    {
                        /*..hui [24-11-18���� 5:01:26] ���� �� ���� ������ ������ �׳� E85 ��ſ��� ���°ɷ�.....*/
                        gu8_bldc_self_error_e85_samsung_e6_retry_count++;

                        if( gu8_bldc_self_error_e85_samsung_e6_retry_count >= BLDC_COMP_ERROR_RETRY_COUNT )
                        {
                            gu8_bldc_self_error_e85_samsung_e6_retry_count = BLDC_COMP_ERROR_RETRY_COUNT;
                            Bit12_BLDC_Operation_Error_Disconnection_Comm__E85 = SET;
                            gu8_memento_e29_detail_code = gu8_bldc_error_code;
                        }
                        else{}
                    }


                }
                else{}
            }
            else{}
        }
        else
        {
            gu8_bldc_self_count = 0;

            gu16_bldc_self_release_count++;

            /*..hui [23-7-4���� 11:11:02] ���� 10�� �̻� �����ڵ� ���� �޾������� �ʱ�ȭ..*/
            /*if( gu16_bldc_self_release_count >= 10 )*/
            /*..hui [24-1-11���� 2:55:16] �ι��� ����̹� ��ü ���� ���� �ð� ����..*/
            /*..hui [24-1-11���� 2:55:32] 1�� + 2�� �� 3������ ����.. �ִ��� �˳��ϰ�..*/
            /*if( gu16_bldc_self_release_count >= 60 )*/
            if( gu16_bldc_self_release_count >= 300 )
            {
                gu16_bldc_self_release_count = 0;
                /*..hui [23-7-4���� 11:04:40] ���� ���� ���� ���Ž� ��õ� Ƚ�� �ʱ�ȭ..*/
                /*..hui [24-11-18���� 5:03:28] �и��� ������ Ƚ�� ���� �ʱ�ȭ..*/
                gu8_bldc_self_error_e81_samsung_e2_retry_count = 0;
                gu8_bldc_self_error_e82_samsung_e1_retry_count = 0;
                gu8_bldc_self_error_e83_samsung_e3_retry_count = 0;
                gu8_bldc_self_error_e84_samsung_e5_retry_count = 0;
                gu8_bldc_self_error_e85_samsung_e6_retry_count = 0;
                gu8_bldc_self_error_e86_samsung_e4_retry_count = 0;
            }
            else{}
        }
    }
    else{}
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void Uart_ISR2_Bldc_Comp_Rx(void)
{
    U8 mu8_rx_buffer = RXD2;
    U8 mu8_packet_length = 0;

    if( (bit_bldc_rx_complete == CLEAR) && (bit_bldc_tx_request == CLEAR) )
    {
        gu8_bldc_rx_buffer[gu8_bldc_rx_receive_cnt] = mu8_rx_buffer;

        if( gu8_bldc_rx_receive_cnt >= (U8)(BLDC_COMP_RX_LENGTH - 1) )
        {
            gu8_bldc_rx_receive_cnt = 0;
            bit_bldc_rx_complete = SET;
        }
        else
        {
            gu8_bldc_rx_receive_cnt++;
        }
    }
    else
    {
        gu8_bldc_rx_receive_cnt = 0;
    }

}





/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void bldc_tx_communication(void)
{
    gu8_bldc_tx_timer++;

    if( gu8_bldc_tx_timer >= BLDC_COMP_COMM_PERIOD )
    {
        gu8_bldc_tx_timer = 0;
        bldc_transmit_error_check();
        bldc_tx_start();
    }
    else{}
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void bldc_transmit_error_check(void)
{
    if( bit_bldc_tx_request == CLEAR )
    {
        gu8_bldc_retry_count++;

        if( gu8_bldc_retry_count >= 2 )
        {
            gu8_bldc_comm_error_count++;
            gu8_bldc_retry_count = 0;
            bit_bldc_tx_request = SET;
            R_UART2_Start();

            /*..hui [24-11-25���� 11:23:22] RX ���� �ʱ�ȭ �߰�..*/
            gu8_bldc_rx_receive_cnt = 0;
        }
        else{}
    }
    else
    {
        gu8_bldc_retry_count = 0;
    }
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void bldc_tx_start(void)
{
    if( bit_bldc_tx_request == SET )
    {
        make_bldc_tx_packet();
        TXD2 = gu8_bldc_tx_buffer[0];
        gu8_bldc_tx_count = 1;
    }
    else{}
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void make_bldc_tx_packet(void)
{
    U8 mu8_crc = 0;

    gu8_bldc_tx_buffer[0] = gu8_bldc_target_hz;

    mu8_crc = bldc_tx_crc( gu8_bldc_tx_buffer );

    gu8_bldc_tx_buffer[1] = mu8_crc;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void Uart_ISR2_Bldc_Comp_Tx(void)
{
    if( gu8_bldc_tx_count < BLDC_COMP_TX_LENGTH )
    {
        TXD2 = gu8_bldc_tx_buffer[gu8_bldc_tx_count];
        gu8_bldc_tx_count++;
    }
    else
    {
        bit_bldc_tx_request = CLEAR;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 bldc_tx_crc( U8 *puchMsg )
{
    U8 mu8_crc = 0;

    mu8_crc = puchMsg[0] ^ BLDC_CHECK_CODE;

    return mu8_crc;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 bldc_rx_crc( U8 *puchMsg )
{
    U8 mu8_crc = 0;

    /*mu8_crc = (puchMsg[0] + puchMsg[1]) ^ BLDC_CHECK_CODE;*/
    /*mu8_crc = (puchMsg[0] + puchMsg[1] + puchMsg[2]) ^ BLDC_CHECK_CODE;*/
    mu8_crc = (puchMsg[0] + puchMsg[1]) ^ BLDC_CHECK_CODE;

    return mu8_crc;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void clear_bldc_buffer(void)
{
    gu8_bldc_rx_buffer[0] = 0;
    gu8_bldc_rx_buffer[1] = 0;
    gu8_bldc_rx_buffer[2] = 0;

    gu8_bldc_tx_buffer[0] = 0;
    gu8_bldc_tx_buffer[1] = 0;

    bit_bldc_tx_request = SET;
    gu8_bldc_tx_timer = 0;
    bit_bldc_rx_complete = CLEAR;
    gu16_bldc_comm_error_timer = 0;
    gu8_bldc_rx_receive_cnt = 0;

    /*gu8_bldc_target_hz = BLDC_COMP_OFF_0_HZ;*/
    gu8_bldc_opration_hz = BLDC_COMP_OFF_0_HZ;
    gu8_bldc_error_code = 0;

    bit_temporary_comm_error = CLEAR;
    bit_temporary_operation_error = CLEAR;

    gu8_bldc_self_count = 0;
    gu16_bldc_self_release_count = 0;

    gu16_comp_error_check_timer = 0;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void bldc_comm_error_check(void)
{
    gu16_comp_error_check_timer++;

    /*..hui [24-1-11���� 2:09:24] COMP �Ѱ� 1�� �ĺ��� ���� �˻�.. ���ν� �����缭..*/
    if( gu16_comp_error_check_timer >= 600 )
    {
        gu16_comp_error_check_timer = 600;
    }
    else{}
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/




