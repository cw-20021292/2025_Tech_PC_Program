/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _OUTPUT_H_
#define _OUTPUT_H_


extern void output_cold_drain_valve_feed6(void);
extern void output_valve_cold_in_feed2(void);
extern void output_valve_room_cold_extract_feed5(void);
extern void output_valve_hot_out_feed_8(void);
extern void output_valve_hot_drain_nos7(void);
extern void output_valve_hot_in_feed9(void);
extern void output_valve_ice_tray_feed3(void);
extern void output_valve_nos(void);
extern void output_valve_overflow_nos4(void);
extern void output_valve_room_in_feed1(void);
extern void OUTPUT_VALVE_ICE_WATER_OUT_FEED6(void);

extern void output_swing_bar(void);
extern void output_ac_motor(void);
extern void output_drain_pump(void);
extern void output_comp_control(void);
/////extern void output_tray_pump(void);
extern void ControlGasSwitch(void);
extern void output_fan(void);
extern void WriteRtcTime(void);
extern void output_temp_power(void);
extern void output_ice_tank_uv(void);
/////extern void output_water_tank_uv(void);
extern void output_ice_tray_uv(void);
extern void output_extract_faucet_uv(void);
extern void output_ice_faucet_uv(void);
/////extern void calc_uv_remain_time(void);
extern void output_boost_pump(void);
/////extern void output_ice_tank_front_uv(void);
extern void MOTOR_ICE_BREAK_OUTPUT(void);
extern void OUTPUT_HEATER_ICE_DOOR(void);

extern bit F_ErrTrayMotor_DualInital;
//extern bit F_HardError;
//extern bit F_ErrOVFeed;
//extern bit F_ErrOVFeed_D;
//extern bit F_ErrOVRoomLEV;
//extern bit F_ErrOVRoomLEV_D;
//extern bit F_ErrOVLEV;
//extern bit F_ErrOVLEV_D;
extern bit F_IceVV;
//extern bit F_ColdDIR;
extern U16 gu16IceMakeTime;
extern bit F_Safety_Routine;
extern bit F_Ice_Tray_Down_Move_Reset;


//extern bit F_ErrDrainSetFix;
extern bit F_ErrIceVV;
//extern bit F_ErrOVF;
extern bit F_Feed;
//extern bit F_ErrColdLEV;
//extern bit F_ErrDrain;
extern bit F_ErrEWDrain;
extern bit F_COLD_VV_In;

//extern bit F_ColdVV;
//extern U8 gu8IceOverCold;

//extern bit F_CyclePump;
//extern bit F_LeakageError;
extern bit F_ColdConty;
extern bit F_over_ice_check_enable;
extern U8 gu8_over_ice_melt_proc;



extern bit bit_self_test_start;





#if 0
#define CRISTAL_ON          60                      // 전자석 ON
#define CRISTAL_OFF         40                      // 전자석 OFF
#endif

//#define CRISTAL_ON          6                      // 전자석 ON
//#define CRISTAL_OFF         4                      // 전자석 OFF






#endif

