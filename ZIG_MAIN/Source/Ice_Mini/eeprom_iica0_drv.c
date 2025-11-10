/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Main.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#include    "Macrodriver.h"
#include    "Global_Variable.h"
#include    "Port_Define.h"
#include    "eeprom_iica0_drv.h"
#include    <string.h>

U8 HAL_RTC_ByteWrite( U8 _dev, U16 _addr , U8 _data );
static U8 ByteWrite( U8 _dev, U16 _addr , U8 _data );
static U8 HAL_RTC_WriteProcComplete( void );
static U8 PageWrite( U8 _dev, U16 _addr , U8 *_data, U8 _len );
U8 HAL_RTC_PageWrite( U8 _dev, U16 _addr , U8 *_data, U8 _len );
static U8 ByteRead( U8 _dev, U16 _addr , U8 *_data );
U8 HAL_RTC_ByteRead( U8 _dev, U16 _addr , U8 *_data );
static U8 SeqRead( U8 _dev, U16 _addr , U8 * _data, U8 _len );
U8 HAL_RTC_SeqRead( U8 _dev, U16 _addr , U8 * _data, U8 _len );
void IICA0_StopCondition(void);
U8 IICA0_Master_Send(U8 adr, U8 * const tx_buf, U16 tx_num, U8 wait);
U8 IICA0_Master_Receive(U8 adr, U8 * const rx_buf, U16 rx_num, U8 wait);
U8 iica0_stop_condition_generate(void);
void iica0_master_handler_modify(void);
static void r_iica0_callback_master_error(U8 flag);
static void r_iica0_callback_master_receiveend(void);
static void r_iica0_callback_master_sendend(void);
void IICA0_Stop(void);
void IICA0_Start(void);


volatile U8 gu8_iica0_master_status_flag; /* iica0 master flag */
volatile U8 gu8_iica0_slave_status_flag;  /* iica0 slave flag */
volatile U8 *gu8_p_iica0_rx_address;        /* iica0 receive buffer address */
volatile U16 gu16_iica0_rx_len;             /* iica0 receive data length */
volatile U16 gu16_iica0_rx_cnt;             /* iica0 receive data count */
volatile U8 *gu8_p_iica0_tx_address;        /* iica0 send buffer address */
volatile U16 gu16_iica0_tx_cnt;             /* iica0 send data count */

U8 gu8_rtc_write_proc;
U8 gu8_rtc_write_fail_cnt;
U8 gu8_rtc_write_ok_cnt;
U8 gu8_rtc_read_proc;
U8 gu8_rtc_read_fail_cnt;
U8 gu8_rtc_read_ok_cnt;


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
/**
 * @brief   : Writing a single byte.
 * @param   : U8 memory address, U8 data.
 * @retval : True or False.
 */
U8 HAL_RTC_ByteWrite( U8 _dev, U16 _addr , U8 _data )
{
    U8 mu8_cnt = 0;
    U8 mu8_com_status = 0;

    for(mu8_cnt = 0; mu8_cnt < IIC_RETRY_COUNT; mu8_cnt++)
    {
        mu8_com_status = ByteWrite( _dev, _addr, _data );

        if(mu8_com_status == IIC_OK)
        {
            return IIC_OK;
        }
        else
        {
            /*..hui [25-5-23오전 9:09:53] 방어코드 추가..*/
            R_IICA0_Stop();
            R_IICA0_Create();
        }
    }

    return IIC_FAIL;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
static U8 ByteWrite( U8 _dev, U16 _addr , U8 _data )
{
    U8 buf[ 3 ];
    U16 mu16_wait_cycle = 10000;
    U8 mu8_return = 0;

    buf[ 0 ] = (U8)((_addr >> 8) & 0x00FF);
    buf[ 1 ] = (U8)(_addr & 0x00FF);
    buf[ 2 ] = _data;


    gu8_rtc_write_proc = EEP_PROC_START;

    mu8_return = IICA0_Master_Send( _dev | EEP_OP_WR , buf , 3 , 200 );

    if( mu8_return != MD_OK )
    {
        gu8_rtc_write_fail_cnt++;

        iica0_stop_condition_generate();
        return IIC_FAIL;
    }
    else{}


    while(gu8_rtc_write_proc != EEP_PROC_DONE)
    {
        mu16_wait_cycle--;

        if(mu16_wait_cycle <= 0)
        {
            gu8_rtc_write_fail_cnt++;
            iica0_stop_condition_generate();
            return IIC_FAIL;
        }
        else{}
    }

    iica0_stop_condition_generate();
    eeprom_write_cycle_time();
    //Delay_MS( 25 );

    gu8_rtc_write_ok_cnt++;
    return IIC_OK;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
/**
 * @brief   : EEPROM processing status check.
 * @param   : None.
 * @retval : True or False.
 */
static U8 HAL_RTC_WriteProcComplete( void )
{
    U16 mu16_wait_cycle = 10000;

    while(gu8_rtc_write_proc != EEP_PROC_DONE)
    {
        mu16_wait_cycle--;

        if(mu16_wait_cycle <= 0)
        {
            return IIC_FAIL;
        }
        else{}
    }

    return IIC_OK;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
/**
 * @brief   : Writing as the page size.
 * @param   : U8 memory address, U8 data.
 * @retval : True or False.
 */
static U8 PageWrite( U8 _dev, U16 _addr , U8 *_data, U8 _len )
{
    U8 buf [EEP_PAGE_SIZE + 2];
    U8 rbuf[EEP_PAGE_SIZE];
    U16 mu16_wait_cycle = 10000;
    U8 mu8_return = 0;

    buf[ 0 ] = (U8)((_addr >> 8) & 0x00FF);
    buf[ 1 ] = (U8)(_addr & 0x00FF);

    /*memcpy( &buf[ 2 ], &_data[ 0 ], _len );*/    //kdh
    memcpy_f( &buf[ 2 ], &_data[ 0 ], _len );      //kdh

    gu8_rtc_write_proc = EEP_PROC_START;

    // Write Page..
    mu8_return = IICA0_Master_Send( _dev | EEP_OP_WR , &buf[ 0 ] , _len + 2 , 200 );

    if( mu8_return != MD_OK )
    {
        gu8_rtc_write_fail_cnt++;
        iica0_stop_condition_generate();
        return IIC_FAIL;
    }
    else{}

    while(gu8_rtc_write_proc != EEP_PROC_DONE)
    {
        mu16_wait_cycle--;

        if(mu16_wait_cycle <= 0)
        {
            gu8_rtc_write_fail_cnt++;
            iica0_stop_condition_generate();
            return IIC_FAIL;
        }
        else{}
    }

    //IICA0_StopCondition();
    /*Delay_MS( DEFAULT_DELAY );*/


    iica0_stop_condition_generate();
    //Delay_MS( DEFAULT_DELAY );
    eeprom_write_cycle_time();
    //Delay_MS( 25 );
    gu8_rtc_write_ok_cnt++;
    return IIC_OK;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 HAL_RTC_PageWrite( U8 _dev, U16 _addr , U8 *_data, U8 _len )
{
    U8 mu8_cnt = 0;
    U8 mu8_com_status = 0;

    for(mu8_cnt = 0; mu8_cnt < IIC_RETRY_COUNT; mu8_cnt++)
    {
        mu8_com_status = PageWrite( _dev, _addr, _data, _len );

        if(mu8_com_status == IIC_OK)
        {
            return IIC_OK;
        }
        else
        {
            /*..hui [25-5-23오전 9:09:53] 방어코드 추가..*/
            R_IICA0_Stop();
            R_IICA0_Create();
        }
    }

    return IIC_FAIL;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
/**
 * @brief   : Read one byte.
 * @param   : U8 memory address, U8 * read data.
 * @retval : True or False.
 */
static U8 ByteRead( U8 _dev, U16 _addr , U8 *_data )
{
    U8 buf[2] = {0};
    U16 mu16_wait_cycle = 10000;
    U8 mu8_return = 0;

    buf[0] = (U8)((_addr >> 8) & 0x00FF);
    buf[1] = (U8)(_addr & 0x00FF);

    gu8_rtc_write_proc = EEP_PROC_START;

    // Write address..
    mu8_return = IICA0_Master_Send( _dev | EEP_OP_WR , buf , 2 , 200 );

    if( mu8_return != MD_OK )
    {
        gu8_rtc_write_fail_cnt++;
        iica0_stop_condition_generate();
        return IIC_FAIL;
    }
    else{}

    while(gu8_rtc_write_proc != EEP_PROC_DONE)
    {
        mu16_wait_cycle--;

        if(mu16_wait_cycle <= 0)
        {
            gu8_rtc_write_fail_cnt++;
            iica0_stop_condition_generate();
            return IIC_FAIL;
        }
        else{}
    }

    gu8_rtc_read_proc = EEP_PROC_START;

    // Read data..
    if( IICA0_Master_Receive( _dev | EEP_OP_RD , _data , 1 , 200 ) != MD_OK )
    {
        gu8_rtc_write_fail_cnt++;
        iica0_stop_condition_generate();
        return IIC_FAIL;
    }
    else{}

    while(gu8_rtc_read_proc != EEP_PROC_DONE)
    {
        mu16_wait_cycle--;

        if(mu16_wait_cycle <= 0)
        {
            gu8_rtc_read_fail_cnt++;
            iica0_stop_condition_generate();
            return IIC_FAIL;
        }
        else{}
    }

    iica0_stop_condition_generate();
    //Delay_MS( DEFAULT_DELAY );
    gu8_rtc_read_ok_cnt++;
    return IIC_OK;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 HAL_RTC_ByteRead( U8 _dev, U16 _addr , U8 *_data )
{
    U8 mu8_cnt = 0;
    U8 mu8_com_status = 0;

    for(mu8_cnt = 0; mu8_cnt < IIC_RETRY_COUNT; mu8_cnt++)
    {
        mu8_com_status = ByteRead( _dev, _addr , _data );

        if(mu8_com_status == IIC_OK)
        {
            return IIC_OK;
        }
        else
        {
            /*..hui [25-5-23오전 9:09:53] 방어코드 추가..*/
            R_IICA0_Stop();
            R_IICA0_Create();
        }
    }

    return IIC_FAIL;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
/**
 * @brief   : Read multiple bytes.
 * @param   : U8 memory address, U8 * read data, U8 read size.
 * @retval : True or False.
 */
static U8 SeqRead( U8 _dev, U16 _addr , U8 * _data, U8 _len )
{
    U8 buf[2] = {0};
    U16 mu16_wait_cycle = 10000;

    buf[0] = (U8)((_addr >> 8) & 0x00FF);
    buf[1] = (U8)(_addr & 0x00FF);

    gu8_rtc_write_proc = EEP_PROC_START;

    /* Send  Address */
    if( IICA0_Master_Send( _dev | EEP_OP_WR , buf , 2 , 200 ) != MD_OK )
    {
        gu8_rtc_write_fail_cnt++;
        iica0_stop_condition_generate();
        return IIC_FAIL;
    }

    /* Wait */
    while(gu8_rtc_write_proc != EEP_PROC_DONE)
    {
        mu16_wait_cycle--;

        if(mu16_wait_cycle <= 0)
        {
            gu8_rtc_write_fail_cnt++;
            iica0_stop_condition_generate();
            return IIC_FAIL;
        }
        else{}
    }

    gu8_rtc_read_proc = EEP_PROC_START;

    /* Recv Data */
    if( IICA0_Master_Receive( _dev | EEP_OP_RD , _data , _len , 200 ) != MD_OK )
    {
        gu8_rtc_write_fail_cnt++;
        iica0_stop_condition_generate();
        return IIC_FAIL;
    }
    else{}

    while(gu8_rtc_read_proc != EEP_PROC_DONE)
    {
        mu16_wait_cycle--;

        if(mu16_wait_cycle <= 0)
        {
            gu8_rtc_read_fail_cnt++;
            iica0_stop_condition_generate();
            return IIC_FAIL;
        }
        else{}
    }

    iica0_stop_condition_generate();
    //Delay_MS( DEFAULT_DELAY );
    gu8_rtc_read_ok_cnt++;
    return IIC_OK;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 HAL_RTC_SeqRead( U8 _dev, U16 _addr , U8 * _data, U8 _len )
{
    U8 mu8_cnt = 0;
    U8 mu8_com_status = 0;

    for(mu8_cnt = 0; mu8_cnt < IIC_RETRY_COUNT; mu8_cnt++)
    {
        mu8_com_status = SeqRead( _dev, _addr , _data, _len );

        if(mu8_com_status == IIC_OK)
        {
            return IIC_OK;
        }
        else
        {
            /*..hui [25-5-23오전 9:09:53] 방어코드 추가..*/
            R_IICA0_Stop();
            R_IICA0_Create();
        }
    }

    return IIC_FAIL;
}

/***********************************************************************************************************************
* Function Name: R_IICA0_Master_Send
* Description  : This function starts to send data as master mode.
* Arguments    : adr -
*                    transfer address
*                tx_buf -
*                    transfer buffer pointer
*                tx_num -
*                    buffer size
*                wait -
*                    wait for start condition
* Return Value : status -
*                    MD_OK or MD_ERROR1 or MD_ERROR2
***********************************************************************************************************************/
U8 IICA0_Master_Send(U8 adr, U8 * const tx_buf, U16 tx_num, U8 wait)
{
    U8 status = MD_OK;

    IICAMK0 = 1;  /* disable INTIICA0 interrupt */

    if (1 == IICBSY0)
    {
        /* Check bus busy */
        IICAMK0 = 0;  /* enable INTIICA0 interrupt */
        status = MD_ERROR1;
    }
    else if( ( 1 == SPT0 ) || ( 1 == STT0 ) )
    {
        IICAMK0 = 0;                                /* enable INTIICA0 interrupt */
        status  = MD_ERROR2;
    }
    else
    {
        STT0 = 1; /* send IICA0 start condition */
        IICAMK0 = 0;  /* enable INTIICA0 interrupt */

        /* Wait */
        while (STD0 == CLEAR)
        {
            wait--;

            if(wait <= 0)
            {
                status = MD_ERROR2;
                /*..hui [25-5-23오전 9:09:53] 방어코드 추가..*/
                return (status);
            }
            else{}
        }

        /* Set parameter */
        gu16_iica0_tx_cnt = tx_num;
        gu8_p_iica0_tx_address = tx_buf;
        gu8_iica0_master_status_flag = 0;
        adr &= (U8)~0x01U; /* set send mode */
        IICA0 = adr; /* send address */
    }

    return (status);
}


/***********************************************************************************************************************
* Function Name: R_IICA0_Master_Receive
* Description  : This function starts to receive IICA0 data as master mode.
* Arguments    : adr -
*                    receive address
*                rx_buf -
*                    receive buffer pointer
*                rx_num -
*                    buffer size
*                wait -
*                    wait for start condition
* Return Value : status -
*                    MD_OK or MD_ERROR1 or MD_ERROR2
***********************************************************************************************************************/
U8 IICA0_Master_Receive(U8 adr, U8 * const rx_buf, U16 rx_num, U8 wait)
{
    U8 status = MD_OK;


    {
        STT0 = 1; /* set IICA0 start condition */
        IICAMK0 = 0;  /* enable INTIIA0 interrupt */

        /* Wait */
        while (wait--)
        {
            ;
        }

        if (0 == STD0)
        {
            status = MD_ERROR2;
        }

        /* Set parameter */
        gu16_iica0_rx_len = rx_num;
        gu16_iica0_rx_cnt = 0;
        gu8_p_iica0_rx_address = rx_buf;
        gu8_iica0_master_status_flag  = 0;
        adr |= 0x01U; /* set receive mode */
        IICA0 = adr; /* receive address */
    }

    return (status);
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 iica0_stop_condition_generate(void)
{
    U8 gu8status = 0;
    U8 gu8wait_cycle = 50;

    SPT0 = SET;     /* set stop condition flag */

    /* Wait */
    while (SPD0 == CLEAR)
    {
        gu8wait_cycle--;

        if(gu8wait_cycle <= 0)
        {
            gu8status = IIC_FAIL;
            return gu8status;
        }
        else{}
    }

    gu8status = IIC_OK;

    return gu8status;
}

/***********************************************************************************************************************
* Function Name: R_IICA0_StopCondition
* Description  : This function sets IICA0 stop condition flag.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void IICA0_StopCondition(void)
{
    SPT0 = 1;     /* set stop condition flag */
}

/***********************************************************************************************************************
* Function Name: iica0_master_handler_modify
* Description  : This function is IICA0 master handler.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void iica0_master_handler_modify(void)
{
    /* Control for communication */
    if ((0 == IICBSY0) && (gu16_iica0_tx_cnt != 0))
    {
        r_iica0_callback_master_error(MD_SPT);
    }
    /* Control for sended address */
    else
    {
        if ((gu8_iica0_master_status_flag & 0x80) == 0)
        {
            if (1 == ACKD0)
            {
                gu8_iica0_master_status_flag |= 0x80;

                if (1 == TRC0)
                {
                    WTIM0 = 1;

                    if (gu16_iica0_tx_cnt > 0)
                    {
                        IICA0 = *gu8_p_iica0_tx_address;
                        gu8_p_iica0_tx_address++;
                        gu16_iica0_tx_cnt--;
                    }
                    else
                    {
                        r_iica0_callback_master_sendend();
                    }
                }
                else
                {
                    ACKE0 = 1;
                    WTIM0 = 0;
                    WREL0 = 1;
                }
            }
            else
            {
                r_iica0_callback_master_error(MD_NACK);
            }
        }
        else
        {
            /* Master send control */
            if (1 == TRC0)
            {
                if ((0 == ACKD0) && (gu16_iica0_tx_cnt != 0))
                {
                    r_iica0_callback_master_error(MD_NACK);
                }
                else
                {
                    if (gu16_iica0_tx_cnt > 0)
                    {
                        IICA0 = *gu8_p_iica0_tx_address;
                        gu8_p_iica0_tx_address++;
                        gu16_iica0_tx_cnt--;
                    }
                    else
                    {
                        r_iica0_callback_master_sendend();
                    }
                }
            }
            /* Master receive control */
            else
            {
                if (gu16_iica0_rx_cnt < gu16_iica0_rx_len)
                {
                    *gu8_p_iica0_rx_address = IICA0;
                    gu8_p_iica0_rx_address++;
                    gu16_iica0_rx_cnt++;

                    if (gu16_iica0_rx_cnt == gu16_iica0_rx_len)
                    {
                        ACKE0 = 0;
                        WREL0 = 1;
                        WTIM0 = 1;
                    }
                    else
                    {
                        WREL0 = 1;
                    }
                }
                else
                {
                    r_iica0_callback_master_receiveend();
                }
            }
        }
    }
}

/***********************************************************************************************************************
* Function Name: r_iica0_callback_master_error
* Description  : This function is a callback function when IICA0 master error occurs.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_iica0_callback_master_error(U8 flag)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_iica0_callback_master_receiveend
* Description  : This function is a callback function when IICA0 finishes master reception.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_iica0_callback_master_receiveend(void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */

    gu8_rtc_read_proc = EEP_PROC_DONE;
}

/***********************************************************************************************************************
* Function Name: r_iica0_callback_master_sendend
* Description  : This function is a callback function when IICA0 finishes master transmission.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_iica0_callback_master_sendend(void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */

    gu8_rtc_write_proc = EEP_PROC_DONE;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void IICA0_Stop(void)
{
    IICE0 = CLEAR;    /* disable IICA0 operation */
}
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void IICA0_Start(void)
{
    IICE0 = SET;    /* Enable IICA0 operation */
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/




