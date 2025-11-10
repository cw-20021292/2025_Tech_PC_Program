/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _CALC_WATER_ACC_H_
#define _CALC_WATER_ACC_H_

//

extern TYPE_BYTE          U8WaterOutStateB;
#define            u8WaterOutState                           U8WaterOutStateB.byte
#define            Bit0_Pure_Water_Select_State                U8WaterOutStateB.Bit.b0
#define            Bit1_Cold_Water_Select_State                 U8WaterOutStateB.Bit.b1
#define            Bit2_Hot_Water_Select_State                U8WaterOutStateB.Bit.b2
#define            Bit3_Ice_Water_Select_State                U8WaterOutStateB.Bit.b3



extern U8 gu8_Room_Water_Level;
extern bit F_WaterOut;
extern U16 u16Efluent_Time;
extern U8 u8Extract_Continue;
extern CUP_LEVEL gu8Cup_level;
extern bit F_FW_Version_Display_Mode;
extern U16 gu16_wifi_water_acc;
extern U16 gu16_wifi_total_usage_water;
extern bit F_DrainStatus;
extern ICE_STEP gu8IceStep;
extern bit bit_install_flushing_state;
extern U8 gu8_filter_change_type;
extern U16 gu16_tds_water_acc;
extern U16 gu16_extract_display_cnt;



/*#define ICE_TRAY_INPUT_USING_WATER                300*/
#define ICE_TRAY_INPUT_USING_WATER                260

#define AUTO_DRAIN_USING_WATER                    500

/*..hui [23-12-6오전 11:35:06] 준비.. 170ml..*/
#define PREPARE_STER_USING_WATER                  170
/*..hui [23-12-6오전 11:35:12] 온수세척.. 225ml..*/
#define HOT_SPRAY_USING_WATER                     225
/*..hui [23-12-6오전 11:35:27] 중수세척.. 340ml..*/
#define LUKEWARM_SPRAY_USING_WATER                340
/*..hui [23-12-6오전 11:35:34] 정수세척.. 530ml..*/
#define FINAL_CLEAN_USING_WATER                   530

#define ICE_TRAY_CLEAN_USING_WATER                1050
/*..hui [23-12-6오전 11:35:40] 해빙..1050ml..*/
#define MELT_ICE_USING_WATER                      1050

#if 0
#define INSTALL_FLUSHING_USING_WATER              14400
#define NEO_INO_FILTER_FLUSHING_USING_WATER       750
#define NEO_RO_INO_FILTER_FLUSHING_USING_WATER    11000
#endif

/*..hui [23-12-18오후 7:02:48] 설치플러싱 = RO 플러싱 1500 + 냉수탱크 3400 = 4900..*/
#define INSTALL_FLUSHING_USING_WATER              4900
/*..hui [23-12-18오후 7:01:44] 0.05LPM 기준 2분..*/
#define NEO_INO_FILTER_FLUSHING_USING_WATER       100
/*..hui [23-12-18오후 7:02:12] 0.05LPM 기준 30분..*/
#define NEO_RO_INO_FILTER_FLUSHING_USING_WATER    1500



#endif
