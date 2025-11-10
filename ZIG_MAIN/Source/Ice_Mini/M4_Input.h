/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _INPUT_H_
#define _INPUT_H_

extern void input_water_level(void);
extern void input_drain_tank_level(void);
extern void input_ice_tray_micro_sw(void);
extern void calculate_flow_input(void);
extern void input_cds(void);
extern void service_reed_sw_input(void);
extern void uv_tank_reed_sw_input(void);
extern void detect_front_cds(void);
extern void input_filter_reed_sw(void);
extern void	input_filter_cover_sw(void);
extern void input_filter_all(void);
extern void filter_reset_timer__ino(void);
extern void decesion_filter_flushing(void);
extern void input_filter(void);
extern void Play_Voice(U16 mu16MemoryAddress);
extern void up_tray_motor(void);

extern TYPE_BYTE          U8MakeModeB;
#define            gu8_Make_Mode                U8MakeModeB.byte
#define            Bit0_Cold_Make_Go                U8MakeModeB.Bit.b0
#define            Bit1_Ice_Make_Go                 U8MakeModeB.Bit.b1

extern TYPE_BYTE          U8CodyInspectOperationB;
#define            u8CodyInspectOperation                      U8CodyInspectOperationB.byte
#define            Bit0_Cody_Service_Kit_Start                 U8CodyInspectOperationB.Bit.b0
#define            Bit1_Empty_Tank_Start                       U8CodyInspectOperationB.Bit.b1
#define            Bit2_Ice_Tray_Test_Start                    U8CodyInspectOperationB.Bit.b2

extern ICE_STEP gu8IceStep;

extern bit bit_ice_one_more;


extern bit F_Cold_Enable;
//extern bit F_Conty;
extern bit F_WaterOut;
extern bit F_ContyGo;
//extern U8 gu8ContyCancel;
extern bit F_6HourNoUse;


extern U16 gu16ADCds;
extern bit F_Save;
//extern U8 gu8OVF;
extern U16 gu16ADIceFull;
extern bit F_Out;
extern bit F_BeOut;
extern bit F_OutSet;
extern bit F_LineTest;
extern U16 gu16ADIceLow;
extern U8 gu8IceTrayLEV;
extern bit F_IceInit;
extern bit bit_ice_tray_1_2_uv_start;


/*#define ICEFULL_LEV 125*/                         // 만빙 인식 레벨 1V(204)->0.6V(125)
/*#define ICEFULL_LEV 250*/                         // 만빙 인식 레벨 1V(204)->0.6V(125)
/*#define ICEFULL_LEV 700*/                         // 만빙 인식 레벨 1V(204)->0.6V(125)

//#define ICEFULL_LEV 500                           // 만빙 인식 레벨 1V(204)->0.6V(125)

//#define ICEFULL_LEV 150                             // 만빙 인식 레벨 1V(204)->0.6V(125)

#define ICEFULL_LEV 250                             // 만빙 인식 레벨 1V(204)->0.6V(125)
#define IR_INTERVAL 9000                            // 만빙 인식 주기 15분








#endif

