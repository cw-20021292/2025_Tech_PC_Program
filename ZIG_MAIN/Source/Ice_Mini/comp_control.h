/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _COMP_CONTROL_H_
#define _COMP_CONTROL_H_


//

extern void GasSwitchCold(void);
extern void Bldc_Comp_Communication(void);
extern void clear_bldc_buffer(void);

#define COMP_FIRST_TURN_ON_5MIN_TIME    3000
#define COMP_RESTART_DELAY_5MIN_TIME    3000

#define COMP_MID_TEMP_KEEP_TIME         900

#define COMP_STABLE_TIME                10

#define PREHEAT_SET_COMP_OFF_TIME       18000
#define PREHEAT_CLEAR_COMP_ON_TIME      1800


#define COMP_MAX_TURN_ON_150MIN_TIME    90000



extern TYPE_BYTE          U8MakeModeB;
#define            gu8_Make_Mode                U8MakeModeB.byte
#define            Bit0_Cold_Make_Go                U8MakeModeB.Bit.b0
#define            Bit1_Ice_Make_Go                 U8MakeModeB.Bit.b1

extern TYPE_BYTE       U8OperationB;
#define         u8Operation                                       U8OperationB.byte
#define         Bit0_Cold_Operation_Disable_State                 U8OperationB.Bit.b0
#define         Bit1_Hot_Operation_Disable_State                  U8OperationB.Bit.b1
#define         Bit2_Ice_Operation_Disable_State                  U8OperationB.Bit.b2
#define         Bit3_Ice_Tank_Ster_Operation_Disable_State        U8OperationB.Bit.b3



extern TYPE_BYTE          U8WaterOutStateB;
#define            u8WaterOutState                           U8WaterOutStateB.byte
#define            Bit0_Pure_Water_Select_State                U8WaterOutStateB.Bit.b0
#define            Bit1_Cold_Water_Select_State                 U8WaterOutStateB.Bit.b1
#define            Bit2_Hot_Water_Select_State                U8WaterOutStateB.Bit.b2
#define            Bit3_Ice_Water_Select_State                U8WaterOutStateB.Bit.b3





extern U16 gu16IceMakeTime;
extern bit F_Safety_Routine;
extern bit F_ErrTrayMotor_DualInital;
extern bit F_IceInit;
extern U8 gu8_GasSwitch_Status;
extern bit F_WaterOut;
extern U8 gu8InitStep;
extern ICE_STEP gu8IceStep;
extern U8 gu8_bldc_tx_timer;
extern bit bit_bldc_tx_request;
extern U8 gu8_bldc_target_hz;
extern U8 gu8_bldc_opration_hz;
extern bit bit_bldc_rx_complete;
extern U16 gu16_bldc_comm_error_timer;
extern U8 gu8_Amb_Temperature_One_Degree;
extern bit bit_temporary_comm_error;

extern bit F_WaterInit;
extern bit F_Ice_Tray_Down_Move_Reset;
extern U8 gu8_over_ice_melt_proc;
extern bit bit_self_test_start;
extern bit bit_cold_first_op;
extern U16 gu16_Cold_Temperature;


#define BLDC_40Hz               0b00101000
#define BLDC_41Hz               0b00101001
#define BLDC_42Hz               0b00101010
#define BLDC_43Hz               0b00101011
#define BLDC_44Hz               0b00101100
#define BLDC_45Hz               0b00101101
#define BLDC_46Hz               0b00101110
#define BLDC_47Hz               0b00101111
#define BLDC_48Hz               0b00110000
#define BLDC_49Hz               0b00110001
#define BLDC_50Hz               0b00110010
#define BLDC_51Hz               0b00110011
#define BLDC_52Hz               0b00110100
#define BLDC_53Hz               0b00110101
#define BLDC_54Hz               0b00110110
#define BLDC_55Hz               0b00110111
#define BLDC_56Hz               0b00111000
#define BLDC_57Hz               0b00111001
#define BLDC_58Hz               0b00111010
#define BLDC_59Hz               0b00111011
#define BLDC_60Hz               0b00111100
#define BLDC_61Hz               0b00111101
#define BLDC_62Hz               0b00111110
#define BLDC_63Hz               0b00111111
#define BLDC_64Hz               0b01000000
#define BLDC_65Hz               0b01000001
#define BLDC_66Hz               0b01000010
#define BLDC_67Hz               0b01000011
#define BLDC_68Hz               0b01000100
#define BLDC_69Hz               0b01000101
#define BLDC_70Hz               0b01000110
#define BLDC_71Hz               0b01000111
#define BLDC_72Hz               0b01001000
#define BLDC_73Hz               0b01001001
#define BLDC_74Hz               0b01001010
#define BLDC_75Hz               0b01001011

#define RXD_CODE_SEQ            0
#define RXD_HZ_SEQ              1
#define RXD_CHECKCODE_SEQ       2

#define BLDC_SENSING_ERROR      1
#define BLDC_LOW_VOLATE_ERROR   2
#define BLDC_SENSORLESS_ERROR   3
#define BLDC_COMP_CONNECT_ERROR 4
#define BLDC_OVER_CURRENT_ERROR 6
#define BLDC_IPM_FAULT_ERROR    7


#endif

