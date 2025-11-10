/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _SERVICE_CHECK_H_
#define _SERVICE_CHECK_H_

extern void Play_Voice(U16 mu16MemoryAddress);
extern void play_melody_warning_197( void );
extern void play_melody_setting_on_198( void );
extern void play_melody_setting_off_199( void );

extern bit F_FW_Version_Display_Mode;


#define SERVICE_REED_ON_DETECT_TIME    20
#define SERVICE_REED_OFF_DETECT_TIME    20

/*#define SERVICE_ELAPSE_TIME    181*/

#define SERVICE_ELAPSE_TIME    91
#define SERVICE_CLEAR_TIME     36000







#endif
