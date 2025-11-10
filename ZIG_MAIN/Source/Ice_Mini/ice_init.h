/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _ICE_INIT_H_
#define _ICE_INIT_H_

//
//extern U16 calc_ice_heater_time(void);


extern void down_tray_motor(void);
extern void up_tray_motor(void);
extern void GasSwitchIce(void);
extern U8 hot_gas_operation(void);
extern U8 get_ice_mode_comp_rps(void);
extern U8 get_hotgas_mode_comp_rps(void);
extern void set_comp_rps( U8 mu8_target );
extern U16 get_hotgas_time(void);
extern void GasSwitchHotGas(void);




extern TYPE_WORD          U16CompOffW;
#define            u16CompOff                              U16CompOffW.word
#define            u8CompOff_L                             U16CompOffW.byte[0]
#define            u8CompOff_H                             U16CompOffW.byte[1]
#define            Bit0_Restart_5min_Delay_State           U16CompOffW.Bit.b0
#define            Bit1_Max_90min_CutOff_State             U16CompOffW.Bit.b1
#define            Bit2_Safty_Routine_State                U16CompOffW.Bit.b2
#define            Bit3_Ice_Making_Err_State               U16CompOffW.Bit.b3
#define            Bit4_Self_Test_COMP_Off_State           U16CompOffW.Bit.b4

extern TYPE_BYTE          U8WaterOutStateB;
#define            u8WaterOutState                           U8WaterOutStateB.byte
#define            Bit0_Pure_Water_Select_State                U8WaterOutStateB.Bit.b0
#define            Bit1_Cold_Water_Select_State                 U8WaterOutStateB.Bit.b1
#define            Bit2_Hot_Water_Select_State                U8WaterOutStateB.Bit.b2
#define            Bit3_Ice_Water_Select_State                U8WaterOutStateB.Bit.b3


extern U16 gu16CompOffDelay;
extern U16 gu16IceMakeTime;
extern U16 gu16IceHeaterTime;

/*extern U8 gu8_IceHeaterControl;*/


extern bit F_IceHeater;
//extern bit F_IceMakeGo;
extern U8 gu8_GasSwitch_Status;
extern bit F_WaterOut;
extern U8 gu8InitStep;
extern U8 gu8IceTrayLEV;
extern bit F_TrayMotorDOWN;
extern bit F_TrayMotorUP;
extern ICE_STEP gu8IceStep;
extern tsSTEPMOTOR sStepMotor;
extern U8 gu8_E62_dummy_iceheat_flag;

#define DUMMY_ICE_MELT_TIME          1800






#endif
