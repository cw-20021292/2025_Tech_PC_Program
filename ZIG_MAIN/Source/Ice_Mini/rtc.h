/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _RTC_H_
#define _RTC_H_

//
extern U8 HAL_RTC_ByteRead( U8 _dev, U16 _addr , U8 *_data );
extern U8 Hex2Dec(U8 mu8Hex);

extern U8 HAL_RTC_ByteWrite( U8 _dev, U16 _addr , U8 _data );
extern U8 HAL_RTC_SeqRead( U8 _dev, U16 _addr , U8 * _data, U8 _len );
extern U8 HAL_RTC_PageWrite( U8 _dev, U16 _addr , U8 *_data, U8 _len );
extern U8 Dec2Hex(U8 mu8Dec);
extern U8 HAL_RTC_ByteRead( U8 _dev, U16 _addr , U8 *_data );
extern void change_24_hour(void);
extern void lcd_time_init(void);





//extern U8 gu8_system_depth;


extern U8 gu8_time_setting_Year;      // 0 ~ 99
extern U8 gu8_time_setting_Month;     // 1 ~ 12
extern U8 gu8_time_setting_Date;      // 1 ~ 31

extern U8 u811111;

extern U8 gu8_eeprom_initial_test_input;

extern bit F_FW_Version_Display_Mode;


/* MASK BIT OF STATUS REGISTER */
#define  MASK_BAT    0x80
#define  MASK_AL1    0x40
#define  MASK_AL0    0x20
#define  MASK_OSCF   0x10
#define  MASK_RWEL   0x04
#define  MASK_WEL    0x02
#define  MASK_RTCF   0x01

#define  MASK_MIL    0x80

/* RTC Memory Map  ----------------------------------------------------------*/
// RTC ADDRESS
#define ADDR_RTC_STATUS   0x003F
#define ADDR_RTC_Y2K      0x0037
#define ADDR_RTC_DAY      0x0036
#define ADDR_RTC_YEAR     0x0035
#define ADDR_RTC_MONTH    0x0034
#define ADDR_RTC_DATE     0x0033
#define ADDR_RTC_HOURS    0x0032
#define ADDR_RTC_MINUTES  0x0031
#define ADDR_RTC_SECONDS  0x0030

// CONTROL ADDRESS
#define ADDR_CTR_PWR      0x0014  // Power
#define ADDR_CTR_DTR      0x0013  // Digital Trimming
#define ADDR_CTR_ATR      0x0012  // Analog Trimming
#define ADDR_CTR_INT      0x0011  // Interrupt Control & Freq Output
#define ADDR_CTR_BL       0x0010  // Block Protect Bits


#define RTC_READ_TIME     20



#endif
