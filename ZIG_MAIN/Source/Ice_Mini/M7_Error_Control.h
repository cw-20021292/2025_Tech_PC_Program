/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _ERROR_CONTROL_H_
#define _ERROR_CONTROL_H_


extern void ErrorSaveStart( U8 mu8_err_code );
extern void ErrorSave(void);
extern void check_error_temp_sensor(void);
extern void check_flow_error(void);
extern void check_error_water_level(void);
extern void hot_water_error_check(void);
extern void down_tray_motor(void);
extern void BuzStep(U8 mu8Step);
extern void check_error_uv(void);
//extern void stop_hot_preheating(void);
extern void play_melody_warning_197( void );



extern TYPE_BYTE          U8WaterOutStateB;
#define            u8WaterOutState                           U8WaterOutStateB.byte
#define            Bit0_Pure_Water_Select_State                U8WaterOutStateB.Bit.b0
#define            Bit1_Cold_Water_Select_State                 U8WaterOutStateB.Bit.b1
#define            Bit2_Hot_Water_Select_State                U8WaterOutStateB.Bit.b2
#define            Bit3_Ice_Water_Select_State                U8WaterOutStateB.Bit.b3

extern TYPE_BYTE          U8ErrorPopupEnableB;
#define            u8ErrorPopupEnable              U8ErrorPopupEnableB.byte
#define            Bit0_Now_Error_Occur_State      U8ErrorPopupEnableB.Bit.b0
#define            Bit1_Hot_Error_Repeat_State     U8ErrorPopupEnableB.Bit.b1





#define MAX_ERROR_NUM          32


extern bit F_Hot_Enable;
extern U8 gu8_Room_Water_Level;
extern U16 gu16_AD_Result_Cold;
extern bit F_Cold_Enable;

//extern bit F_ColdWaterInit;
//extern U8 gu8ColdWaterLEV;
extern U16 gu16_AD_Result_Room;
extern U16 gu16_AD_Result_Eva_First;
extern U16 gu16_AD_Result_Amb;
extern U16 gu16ADLeakage;

extern bit F_WaterOut;
extern U16 gu16_water_select_return_time;
extern U8 u8Extract_Continue;
extern bit bit_tray_in_error_temporary;



extern bit F_IceInit;
extern U8 gu8IceTrayLEV;
//extern bit F_Memento;
extern bit F_IR;
extern ICE_STEP gu8IceStep;

extern U8 gu8_rtc_time_Year;
extern U8 gu8_rtc_time_Month;
extern U8 gu8_rtc_time_Date;

extern bit F_FW_Version_Display_Mode;
extern bit bit_temporary_comm_error;
extern bit bit_temporary_operation_error;
extern bit bit_filter_all;
extern bit bit_self_test_start;
extern U8 gu8InitStep;
extern bit F_Hot_Enable;
extern U16 gu16_AD_Result_Hot_Tank_Temp;

extern U8 gu8_error_popup_enable;

extern bit F_IceOut;
extern bit F_IceOutCCW;
extern bit F_IceBreak_Motor_Out;
extern ICE_OUT_STEP gu8_Ice_Out_Step;
extern U8 gu8_ice_out_continue;
extern DISPLAY_STACK Extract_Stack;
#endif
