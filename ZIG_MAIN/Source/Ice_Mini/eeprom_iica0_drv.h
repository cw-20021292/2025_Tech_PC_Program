/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _IICA0_DRV_H_
#define _IICA0_DRV_H_

extern void eeprom_write_cycle_time(void);
extern void Delay_MS( U16 ms );

#if 0
/* Status list definition */
#define MD_STATUSBASE        (0x00U)
#define MD_OK                (MD_STATUSBASE + 0x00U) /* register setting OK */
#define MD_SPT               (MD_STATUSBASE + 0x01U) /* IIC stop */
#define MD_NACK              (MD_STATUSBASE + 0x02U) /* IIC no ACK */
#define MD_BUSY1             (MD_STATUSBASE + 0x03U) /* busy 1 */
#define MD_BUSY2             (MD_STATUSBASE + 0x04U) /* busy 2 */
#define MD_OVERRUN           (MD_STATUSBASE + 0x05U) /* IIC OVERRUN occur */

/* Error list definition */
#define MD_ERRORBASE         (0x80U)
#define MD_ERROR             (MD_ERRORBASE + 0x00U)  /* error */
#define MD_ARGERROR          (MD_ERRORBASE + 0x01U)  /* error agrument input error */
#define MD_ERROR1            (MD_ERRORBASE + 0x02U)  /* error 1 */
#define MD_ERROR2            (MD_ERRORBASE + 0x03U)  /* error 2 */
#define MD_ERROR3            (MD_ERRORBASE + 0x04U)  /* error 3 */
#define MD_ERROR4            (MD_ERRORBASE + 0x05U)  /* error 4 */
#endif

#define EEP_OP_WR              0x00
#define EEP_OP_RD              0x01


#define EEP_MAX_SIZE           512   /* bytes */
#define EEP_PAGE_SIZE          16    /* 16-byte page write */
#define EEP_PAGE_NUM           32    /* 512 bytes / 16 page per bytes = 32 pages */
//#define RTC_PAGE_SIZE          8     /* 8-byte page write for rtc */

#if 0
#define EEP_PAGE_SIZE          8    /* 8-byte page write */
#endif



#define EEP_PROC_START      0
#define EEP_PROC_DONE       1

#define  IIC_OK       1
#define  IIC_FAIL     0

#define IIC_RETRY_COUNT     5





#endif
