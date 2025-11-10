/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _Memento_H_
#define _Memento_H_

//extern void EEPROM_ConfirmWrite(U8 mu8EEPROM_Address, U8 mu8EEPROM_Data);

extern void EepromPageWrite(U16 mu16Addr, U8 *pData, U8 mu8len );




extern bit F_AckFail;

extern U8 gu8_Error_Code;
//extern bit F_HardError;
extern U8 gu8_rtc_time_Year;
extern U8 gu8_rtc_time_Month;
extern U8 gu8_rtc_time_Date;








extern U8 gu8_eeprom_wbuf[16];














#endif
