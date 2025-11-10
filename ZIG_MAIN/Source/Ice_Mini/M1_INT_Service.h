/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _INT_SERVICE_H_
#define _INT_SERVICE_H_
/**********************************************************************************************************************/
extern void ADC_Start(void);
extern void ADC_Stop(void);
extern void INTP_Disable( void );
extern void INTP_Enable( void );
extern void Timer_250us_Service(void);
extern void motor_ice_select_output(void);
extern void motor_ice_door_output(void);
extern void motor_ice_tray_output(void);
extern void Icetray_Stepmotor_Handler(void);
extern void control_ice_tray_motor(void);
extern void UART1_Tx_INT(void);
extern void UART1_Rx_INT(void);
extern void UART1_Error_INT(void);
extern void iica0_master_handler_modify(void);
extern void Uart_ISR0_Front_Panel_Tx ( void );
extern void Uart_ISR0_Front_Panel_Rx(void);
extern void INTP11_Flow_Sensor_Input(void);
extern void StepMotorTimerInterrupt(void);
extern void UART3_TXD_DATA(void);
extern void int_UART3_AT_TX(void);
extern void int_UART3_WORK_RX(void);
extern void Uart_ISR3_PC_Tx( void );
extern void Uart_ISR2_Bldc_Comp_Rx(void);
extern void Uart_ISR2_Bldc_Comp_Tx(void);
/*extern void HAL_ControlMixMotor( void );*/
extern void UART2_Receive_Interrupt(U8 mu8RxData);
extern void UART2_Error_Interrupt(void);
extern void HAL_ControlRoDrainMotor( void );
/////extern void INTP7_Filter_Flow_Sensor_Input(void);
extern void InterruptUartWifiTx ( void );
extern void InterruptUartWifiRx ( U8 mu8RxData );
extern void InterruptUartWifiError ( void );
extern void INTP4_TDS_Input(void);
extern void INTTM01_Triac_Timer(void);
extern void HAL_ControlFlowMotor( void );
extern void OUTPUT_HEATER_ICE_DOOR(void);

extern void int_UART3_AT_TX(void);
extern void int_UART3_WORK_RX(void);
/**********************************************************************************************************************/

/**********************************************************************************************************************/
extern bit F_UART2ErrorInterrupt;
/**********************************************************************************************************************/
//extern void Uart1_Tx_ISR(void);
//extern void Uart1_Rx_ISR(void);
//extern void Uart1_Error_ISR(void);
#define FRAMING_ERROR    1
#define PARITY_ERROR     2
#define OVERRUN_ERROR    3
#define UNKNOWN_ERROR    4
/**********************************************************************************************************************/
extern U8  u8Buzzer_Delay;
extern U8  u8Buzzer_Mode;
extern U8  u8Buzzer_Time;
extern bit F_BUZZER_Power;
//extern U16  u16RemoteCount;
extern U8   u8IDCheckTimer;
extern U8   u8IDCheckCount;
extern U8   u8DECheckIntervalTimer;
extern U8  u8SeatLevelDetectTime;
extern bit F_WaterLevel;
extern bit F_SeatLevel;
extern U8 u8WaterPressureLevel;
extern U8 u8SeatTemperatureLevel;
extern U8 u8DryTempLevel;
extern U8 u8WaterTemperatureLevel;
extern U8 u8NozzleLevel;
extern U8  u8Mode;
extern U8 u8AgingEnableTimer;
extern U16 u16Motor3WayCount;
extern U16 u16Motor4WayCount;
extern U16 u16MotorBidetNozzleCount;
extern U16 u16MotorCleanNozzleCount;
extern bit  F_NewDataReceive;
//extern bit  F_RemoReceiveFinish;
//extern bit  F_ChildNozzleChange;
extern bit  F_ChildNozzleChange_Bidet;
extern F32 f32InHot_Temperature;      /*..�Լ����� ���� �µ� ��..*/
extern F32 f32OutHot_Temperature;      /*..�Լ����� ���� �µ� ��..*/
extern U16 u16Seat_Temperature;
extern U16 u16ADEWFeedbackResult;
extern U8 gu8_Error_Code;
//extern U8 u8IIC_Wait_Cycle_Delay;
extern U8  u8Count_1min;

extern U8 gu8_front_rx_receive_cnt;
/**********************************************************************************************************************/

/**********************************************************************************************************************/

/**********************************************************************************************************************/

/**********************************************************************************************************************/

/**********************************************************************************************************************/
/**********************************************************************************************************************/

#endif

