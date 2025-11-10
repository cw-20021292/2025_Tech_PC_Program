/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _AC_MOTOR_OUTPUT_H_
#define _AC_MOTOR_OUTPUT_H_

extern void GasSwitchHotGas(void);
extern U8 get_hotgas_mode_comp_rps(void);
extern void set_comp_rps( U8 mu8_target );
extern U16 get_hotgas_time(void);
extern U8 hot_gas_operation(void);

extern void control_ice_tray(void);
extern void run_down_ice_tray(void);
extern void run_up_ice_tray(void);
extern void run_stop_ice_tray(void);
extern void stop_tray_motor(void);
extern void up_tray_motor(void);
extern void down_tray_motor(void);

extern void ice_extraction_finish(void);

extern TYPE_LONG       U32ControlErrorsD;
#define         u32ControlErrors                                    U32ControlErrorsD.dward
#define         Bit0_Hot_Water_Flow_Block_Error__E08              	U32ControlErrorsD.Bit.b0    //E08
#define         Bit1_Main_Water_Flow_Leaked_Error__E02              U32ControlErrorsD.Bit.b1    //E02
#define         Bit2_Room_Temp_Open_Short_Error__E42                U32ControlErrorsD.Bit.b2    //E42
#define         Bit3_Leakage_Sensor_Error__E01                      U32ControlErrorsD.Bit.b3    //E01

#define         Bit4_Hot_In_Temp_Open_Short_Error__E46              U32ControlErrorsD.Bit.b4    //E46
#define         Bit5_Hot_Heater_OverHeat_Error__E40		            U32ControlErrorsD.Bit.b5    //E40
#define         Bit6_Main_Water_Flow_Block_Error__E09               U32ControlErrorsD.Bit.b6    //E09
#define         Bit7_BLDC_Communication_Error__E27                  U32ControlErrorsD.Bit.b7    //E27

#define         Bit8_BLDC_Operation_Error_Current_Sensing__E81      U32ControlErrorsD.Bit.b8    //E81
#define         Bit9_BLDC_Operation_Error_Starting_Fail__E82        U32ControlErrorsD.Bit.b9    //E82
#define         Bit10_BLDC_Operation_Error_Over_Current__E83        U32ControlErrorsD.Bit.b10   //E83
#define         Bit11_BLDC_Operation_Error_Overheat__E84            U32ControlErrorsD.Bit.b11   //E84

#define         Bit12_BLDC_Operation_Error_Disconnection_Comm__E85  U32ControlErrorsD.Bit.b12   //E85
#define         Bit13_BLDC_Operation_Error_Abnormal_Voltage__E86    U32ControlErrorsD.Bit.b13   //E86
#define         Bit14_Cold_Temp_Open_Short_Error__E44               U32ControlErrorsD.Bit.b14   //E44
#define         Bit15_Amb_Temp_Open_Short_Error__E43                U32ControlErrorsD.Bit.b15   //E43

#define         Bit16_Drain_Pump_Error__E60                         U32ControlErrorsD.Bit.b16   //E60
#define         Bit17_Tray_Micro_SW_Dual_Detect_Error__E61          U32ControlErrorsD.Bit.b17   //E61
#define         Bit18_Tray_Micro_SW_Up_Move_Error__E62              U32ControlErrorsD.Bit.b18   //E62
#define         Bit19_Tray_Micro_SW_Down_Move_Error__E63            U32ControlErrorsD.Bit.b19   //E63

#define         Bit20_Hot_Out_Temp_Open_Short_Error__E47	        U32ControlErrorsD.Bit.b20   //E52
#define         Bit21_Amb_Side_Temp_Open_Short_Error__E53           U32ControlErrorsD.Bit.b21   //E53
#define         Bit22_Hot_Heater_Temp_Open_Short_Error__E48		    U32ControlErrorsD.Bit.b22   //E48

#define         Bit23_faucet_UV_Error__E77                  		U32ControlErrorsD.Bit.b23   //E77
#define         Bit24_Ice_Tank_1_2_UV_Error__E75					    U32ControlErrorsD.Bit.b24   //E75
#define         Bit25_Ice_Faucet_UV_1_Error__E78                    U32ControlErrorsD.Bit.b25   //E78
#define         Bit26_Ice_Faucet_UV_2_Error__E78                    U32ControlErrorsD.Bit.b26   //E78
#define         Bit27_Ice_Tank_3_UV_Error__E75                	U32ControlErrorsD.Bit.b27   //E79
#define         Bit28_Ice_Tray_1_2_UV_Error__E76                	U32ControlErrorsD.Bit.b28   //E76





extern bit F_IceOutCCW;
extern U8 gu8IceOutCCWInterval;
extern ICE_LEVEL  gu8_ice_amount_step;
extern U8 gu8_ice_out_continue;

extern bit F_ErrTrayMotor_Move_D;
extern U16 gu16ErrIceOut;
//extern bit F_ErrTrayMotor_Move;
extern bit F_TrayMotorPreUP;
extern U8 gu8IceLEV;
extern bit F_IceInit;
//extern bit F_ErrTrayMotor_Dual;

//extern bit F_Tray_up_moving_retry_state;
extern bit F_TrayMotorPreDOWN;
extern U16 gu16IceHeaterTime;
extern U16 gu16IceMakeTime;
extern bit F_Trayretry1;

//extern bit F_Reset;
extern U16 gu16ErrTrayMotor_Test_Mode;


extern bit F_Safety_Routine;
//extern bit F_IceMakeGo;
//extern bit F_Hotgas_Time;
extern bit F_IceHeater;
extern bit F_IceVV;
//extern bit F_ColdIn;
extern U16 gu16CompOffDelay;
//extern bit F_over_ice_melt_mode_cold_water_valve_out;


//extern U8 gu8ColdWaterLEV;
//extern bit F_ColdDIR;
extern bit F_ColdIce;
extern bit F_BarCW;
extern U8 gu8IRCount;
extern U8 gu8NoSBStep;
//extern U16 gu16MeltTime;
extern U8 gu8BarCNT;
//extern bit F_NoSelectBar;
extern bit F_NoColdBar;
extern bit F_NoIceBar;

//extern U16 Select_Delay;
extern U8 gu8InitStep;
extern U8 gu8IceTrayLEV;
extern bit F_TrayMotorDOWN;
extern bit F_TrayMotorUP;
extern ICE_STEP gu8IceStep;

extern bit F_Low_IR;                        // 저빙 검사
extern bit F_IR;                            // 만빙 검사
extern bit F_IceOut;
extern bit F_IceFull;
extern bit F_IceBreak;
extern U8 gu8_GasSwitch_Status;

extern U8 gu8_ice_tray_mode;
extern ICETRAY_STATE icetray_state_target;
extern U16 gu16_ice_out_timer;
extern bit F_IceSelectClose;


/*#define ICETRAY_CW_TIME 500*/                     // Ice Tray CW 25초 -> 50초
/*#define ICETRAY_CCW_TIME 500*/                    // Ice Tray CCW 25초 -> 50초


#define ICETRAY_CW_TIME 400                     // Ice Tray CW 25초 -> 50초
#define ICETRAY_CCW_TIME 500                    // Ice Tray CCW 25초 -> 50초



// 20161101 아이스트레이 마이크로 스위치 감지 개선
#define ICETRAY_RE_DELAY       220                                              //20161012 마이크로스위치 해지 안될 시 Ice Tray 정/역 딜레이 5초
#define ICETRAY_RE_DELAY1       220                                              //20161012 마이크로스위치 해지 안될 시 Ice Tray 정/역 딜레이 5초


#define TRAY_MOTOR_STOP            0
#define TRAY_MOTOR_CW_DIRECTION    1
#define TRAY_MOTOR_CCW_DIRECTION   2



#define TRAY_ERROR_STANDBY_TIME    6000


/*#define TRAY_UP_RESET_DELAY_TIME    12000*/

/*..hui [23-9-21오후 3:53:57] 제빙 방향 대기시간 90분으로 변경.. 아이콘 아이스 사양..*/
#define TRAY_UP_RESET_DELAY_TIME    54000


/*#define TRAY_UP_RETRY_COUNT         10*/
/*..hui [23-9-22오전 9:38:15] 트레이 제빙 방향 이동 재시도 횟수 5회.. 김규태 과장과 협의. 10회는 너무 과함..*/
#define TRAY_UP_RETRY_COUNT         9 // 5

#define TRAY_UP_ICE_STUCK_COUNT         5
#define TRAY_UP_ICE_STUCK_DELAY_TIME    600

#endif
