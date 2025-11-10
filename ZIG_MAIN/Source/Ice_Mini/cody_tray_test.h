/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _CODY_TRAY_TEST_H_
#define _CODY_TRAY_TEST_H_

//

extern void down_tray_motor(void);
extern void up_tray_motor(void);
extern void reset_micro_sw_move_err_check_state(void);
extern void play_melody_warning_197( void );
extern void play_melody_setting_on_198( void );
extern void play_melody_setting_off_199( void );





extern TYPE_BYTE          U8CodyInspectOperationB;
#define            u8CodyInspectOperation                      U8CodyInspectOperationB.byte
#define            Bit0_Cody_Service_Kit_Start                 U8CodyInspectOperationB.Bit.b0       // 코디 점검을 위한 제빙중지
#define            Bit1_Empty_Tank_Start                       U8CodyInspectOperationB.Bit.b1       // 코디 점검을 위한 탱크비움 (미사용)
#define            Bit2_Ice_Tray_Test_Start                    U8CodyInspectOperationB.Bit.b2       // 코디 점검을 위한 트레이 동작점검








extern bit F_IceHeater;
extern bit F_IceInit;
extern U8 u8_lcd_cody_popup_timer;



extern U16 gu16IceMakeTime;
extern U16 gu16IceHeaterTime;
extern U16 gu16_cody_ice_make_time;
extern U8 gu8InitStep;
extern U8 gu8IceTrayLEV;
extern bit F_TrayMotorDOWN;
extern bit F_TrayMotorUP;
extern ICE_STEP gu8IceStep;

extern bit F_Ice_Tray_Up_Move_Reset;
extern bit F_Ice_Tray_Down_Move_Reset;


extern U8 gu8_Err_Tray_Down_Count;
extern U8 gu8_err_tray_down_acc_count;
extern U8 gu8_Err_Tray_Up_Count;
extern bit F_ErrTrayMotor_DualInital;
extern bit F_All_Lock;
extern bit F_Comp_Output;
extern bit F_Child_Lock;

extern tsSTEPMOTOR sStepMotor;



#endif
