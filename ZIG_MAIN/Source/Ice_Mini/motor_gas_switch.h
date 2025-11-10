/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _MOTOR_GAS_SWITCH_H_
#define _MOTOR_GAS_SWITCH_H_

//
enum
{
    INIT_POWER_ON,
    INIT_WAIT_POWER_ON,

    INIT_MOVE_ZERO,
    INIT_WAIT_ZERO,

    INIT_MOVE_COLD,
    INIT_WAIT_COLD,

    INIT_DONE
};

enum
{
    COLD_POWER_ON,
    COLD_WAIT_POWER_ON,

    COLD_CHECK_POSITION,

    COLD_MOVE,
    COLD_WAIT,

    COLD_DONE
};


enum
{
    ICE_POWER_ON,
    ICE_WAIT_POWER_ON,

    ICE_CHECK_POSITION,

    ICE_MOVE,
    ICE_WAIT,

    ICE_DONE
};

enum
{
    HOTGAS_POWER_ON,
    HOTGAS_WAIT_POWER_ON,

    HOTGAS_CHECK_POSITION,

    HOTGAS_MOVE,
    HOTGAS_WAIT,

    HOTGAS_DONE
};


#define	PHASE_STEP_NUM		8

//#define GAS_SWITCH_COLD  0
//#define GAS_SWITCH_ICE   1
#if 0
#define GAS_SWITCH_CLOSE_STEP_VAL      (34)
#define GAS_SWITCH_COLD_STEP_VAL       (100)
#define GAS_SWITCH_ICE_STEP_VAL        (195)
#define GAS_SWITCH_INIT_STEP_VAL       (-30)
#endif

#define  GAS_SWITCH_COLD_STEP_VAL    (24)
#define  GAS_SWITCH_ICE_STEP_VAL     (64)
#define  GAS_SWITCH_HOTGAS_STEP_VAL  (104)
#define  GAS_SWITCH_END_STEP_VAL     (200)
#define  GAS_SWITCH_INIT_STEP_VAL    (-15)


/* MODE */
#if 0
#define GAS_SWITCH_MODE_NONE           0x00
#define GAS_SWITCH_MODE_INIT           0x01
#define GAS_SWITCH_MODE_COLD           0x02
#define GAS_SWITCH_MODE_ICE            0x04
#define GAS_SWITCH_MODE_POWER_INIT     0x08
#endif

#define  GAS_SWITCH_MODE_NONE           0x00
#define  GAS_SWITCH_MODE_INIT           0x01
#define  GAS_SWITCH_MODE_COLD           0x02
#define  GAS_SWITCH_MODE_ICE            0x04
#define  GAS_SWITCH_MODE_HOTGAS         0x08
//#define  GAS_SWITCH_MODE_POWER_INIT     0x10


#define POWER_ON_TIME                  10
#define COMP_OFF_INIT_TIME             60U

/*#define	DEFAULT_HOLD_STEP	           20*/		/* about 0.5 sec */
#define	DEFAULT_HOLD_STEP	           5		/* about 0.5 sec */



extern U16 gu16CompOffDelay;
extern bit F_Comp_Output;



extern TYPE_WORD          U16CompOffW;
#define            u16CompOff                              U16CompOffW.word
#define            u8CompOff_L                             U16CompOffW.byte[0]
#define            u8CompOff_H                             U16CompOffW.byte[1]
#define            Bit0_Restart_5min_Delay_State           U16CompOffW.Bit.b0
#define            Bit1_Max_90min_CutOff_State             U16CompOffW.Bit.b1
#define            Bit2_Safty_Routine_State                U16CompOffW.Bit.b2
#define            Bit3_Ice_Making_Err_State               U16CompOffW.Bit.b3
#define            Bit4_Self_Test_COMP_Off_State           U16CompOffW.Bit.b4






extern TYPE_BYTE          U812VPowerControl;
#define            u8PowerControl12V                                    U812VPowerControl.byte
#define            Bit0_Gas_Switch_12V_Out_State                        U812VPowerControl.Bit.b0
#define            Bit1_Ice_Tank_UV_12V_Out_State                       U812VPowerControl.Bit.b1
#define            Bit2_Uart_Test_Mode_State                            U812VPowerControl.Bit.b2
#define            Bit3_Water_Tank_UV_12V_Out_State                     U812VPowerControl.Bit.b3
#define            Bit4_Ice_Tray_UV_12V_Out_State                       U812VPowerControl.Bit.b4
#define            Bit5_Ice_Tank_Front_UV_12V_Out_State                 U812VPowerControl.Bit.b5
#define            Bit6_Drain_Pump_FeedBack_12V_Out_State               U812VPowerControl.Bit.b6



#endif
