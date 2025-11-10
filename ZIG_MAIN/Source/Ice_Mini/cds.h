/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _CDS_H_
#define _CDS_H_

//

extern U16 gu16ADCds;
/////extern bit F_Save;
extern U8 gu8_Amb_Temperature_One_Degree;
extern bit F_IceFull;


/*#define ICESTOP_TIME_SIX_HOURS               60*60*6*/                 // 취침시 제빙정지 6시간(21600초)

#define ICESTOP_TIME_SIX_HOURS                 21600                 // 취침시 제빙정지 6시간(21600초)
#define ICESTOP_TIME_FIVE_HOURS                18000                 // 취침시 제빙정지 5시간(21600초)

#define POWER_SAVING_WAKEUP_DELAY_TIME         3000



#endif
