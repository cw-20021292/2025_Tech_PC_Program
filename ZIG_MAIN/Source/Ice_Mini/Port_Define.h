/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/

#ifndef _Port_Define_H
#define _Port_Define_H

/***********************************************************************************************************************/

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
/******************************************************************************************************************************************/
#define UI2C_SCL                               P6.0    //(O)// EEPROM (SCL)
#define UI2C_SDA                               P6.1    //(IO)// EEPROM(SDA)
/******************************************************************************************************************************************/
#define pWifi_RESET                            P0.1
#define pWIFI_TX                               P0.2
#define pWIFI_RX                               P0.3
#define pWifi_ON                               P0.4
/******************************************************************************************************************************************/
/* 냉매전환 STEP MOTOR */
#define pSTEP_MOTOR_GAS_SW_1                   P7.1
#define pSTEP_MOTOR_GAS_SW_2                   P0.5
#define pSTEP_MOTOR_GAS_SW_3                   P7.0
#define pSTEP_MOTOR_GAS_SW_4                   P0.6
/******************************************************************************************************************************************/
#define pFRONT_RESET                           P1.0     // FRONT RESET
#define pFRONT_RX                              P1.1
#define pFRONT_TX                              P1.2
#define pBLDC_TX                               P1.3
#define pBLDC_RX                               P1.4
/******************************************************************************************************************************************/
/* ADC CHANNELS */
#define pAD_TEST_24V_CURRENT                   P2.0    //ANI 0
#define pAD_TEST_12V_CURRENT                   P2.1    //ANI 1
#define pAD_Leakage                            P2.2    //ANI 2
#define pAD_HOT_HEATER_TEMP_TH                 P2.3    //ANI 3
#define pAD_HOT_OUT_TEMP_TH                    P2.4    //ANI 4
#define pAD_HOT_IN_TEMP_TH                     P2.5    //ANI 5
#define pAD_AMB_TEMP_1                         P2.6    //ANI 6
#define pAD_EVA_TEMP                           P2.7    //ANI 7
#define pAD_HOT_HEATER_IAD                     P3.5    //ANI 23
#define P_ICE_DOOR_HEATER_IAD                  P3.6    //ANI 22
/******************************************************************************************************************************************/
/* 목업 : DOOR 반대로 되어있음 */
#define pSTEP_MOTOR_ICE_DOOR_1                 P3.3
#define pSTEP_MOTOR_ICE_DOOR_2                 P3.2
#define pSTEP_MOTOR_ICE_DOOR_3                 P10.6
#define pSTEP_MOTOR_ICE_DOOR_4                 P10.5
/******************************************************************************************************************************************/
/* 얼음 추출 최종도어 STEP MOTOR */
#define pSTEP_MOTOR_ICE_SELECT_4               P4.6
#define pSTEP_MOTOR_ICE_SELECT_3               P4.5
#define pSTEP_MOTOR_ICE_SELECT_2               P4.4
#define pSTEP_MOTOR_ICE_SELECT_1               P4.3
/******************************************************************************************************************************************/
/* 순간온수 히터 릴레이 */
#define pHEATER_HOT_H_RELAY                    P4.7
/******************************************************************************************************************************************/
/* ICE TRAY STEP MOTOR */
#define pSTEP_MOTOR_ICE_TRAY_4                 P5.7
#define pSTEP_MOTOR_ICE_TRAY_3                 P1.7
#define pSTEP_MOTOR_ICE_TRAY_2                 P1.6
#define pSTEP_MOTOR_ICE_TRAY_1                 P1.5
/******************************************************************************************************************************************/
/* EEPROM, RTC */
#define P_I2C_SCL                              P6.0
#define P_I2C_SDA                              P6.1
#define PIN_MODE_I2C_SDA                       PM6.1
/******************************************************************************************************************************************/
#define pROOM_TH_POWER                         P6.2
#define pCOLD_TH_POWER                         P6.3
/******************************************************************************************************************************************/
#ifndef FEEDER_SCREW_DIRECTION_BEFORE
/* 얼음 토출모터 (최종) */
#define pMOTOR_ICE_OUT_CW                      P7.4     // 얼음 추출 정방향 출력 (목업 : 커넥터 반대로)
#define pMOTOR_ICE_OUT_CCW                     P7.5     // 얼음 추출 역방향 출력 (목업 : 커넥터 반대로)

#else
/* 개발용 (노란색) */
#define pMOTOR_ICE_OUT_CW                      P7.5     // 얼음 추출 정방향 출력 (목업 : 커넥터 반대로)
#define pMOTOR_ICE_OUT_CCW                     P7.4     // 얼음 추출 역방향 출력 (목업 : 커넥터 반대로)
#endif
/******************************************************************************************************************************************/
#define pCOMP                                  P7.2
#define P_OUT_ICE_CRUSH                        P7.3     // 얼음 분쇄모터 (DC)
/******************************************************************************************************************************************/
#define pOPTION                                P7.6     // CPI(냉정아이스) / CHPI(냉온정아이스) 모델 판단 (2KG는 CHPI만 있음)
/******************************************************************************************************************************************/
#define P_IN_FLOW_FEEDBACK_1                   P7.7     //INTP 11 (유량 감지)
/******************************************************************************************************************************************/
/* 온수 유량조절밸브 STEP MOTOR */
#define pSTEP_MOTOR_FLOW_1                     P8.2
#define pSTEP_MOTOR_FLOW_2                     P8.3
#define pSTEP_MOTOR_FLOW_3                     P8.4
#define pSTEP_MOTOR_FLOW_4                     P8.5
/******************************************************************************************************************************************/
/* 드레인탱크 및 펌프 */
#define pLEVEL_ICE_DRAIN_LOW                   P9.3
#define pLEVEL_ICE_DRAIN_HIGH                  P9.4
#define pPUMP_DRAIN_PWM                        P6.7    //TO13 (Timer PWM으로 제어)
/******************************************************************************************************************************************/
#define pHEATER_HOT_TRIAC_L                   P10.3
/******************************************************************************************************************************************/
#define P_OUT_PUMP_ON                          P11.1
#define pDC_FAN                                P11.4
#define P_AD_COOLING_FAN_IAD                  P11.5   // ANI 26
#define P_AD_OUTDOOR_TEMP_2                   P11.6   // ANI 25
/******************************************************************************************************************************************/
/* 리드스위치 */
#define pREED_FILTER_COVER                    P12.3
#define pREED_FILTER                          P12.4
#define pREED_TANK_COVER_1                    P13.7
#define pREED_TANK_COVER_2                    P12.2
// #define P_IN_SW_REED_5                        P12.1
/******************************************************************************************************************************************/
#define pHEATER_ICE_DOOR                      P14.0         /* 얼음 추출도어 쪽 결로 발생 방지용 히터 */
/******************************************************************************************************************************************/
/* 만빙센서 */
#define pIR_POWER                              P14.2
#define P_IN_ICE_FULL_RX_1                     P12.0   // ANI 19

/* 저빙센서 */
#define pIR_POWER2                          P14.1
#define P_IN_ICE_FULL_RX_2                     P3.7
/******************************************************************************************************************************************/
#define pFACTORY_TEST_RX                       P14.3
#define pFACTORY_TEST_TX                       P14.4
/******************************************************************************************************************************************/
/* 트레이 리미트 스위치 */
#define pICE_MICRO_SW_HIGH                     P0.0
#define pICE_MICRO_SW_LOW                      P14.5
/******************************************************************************************************************************************/
#define pAD_ROOM_Temp                          P15.0    //ANI 8
#define pAD_COLD_Temp                          P15.1    //ANI 9
#define pAD_Drain_Pump_IAD                     P15.2    // ANI 10
/******************************************************************************************************************************************/
/* UV 관련 PORT */
#define pUV_LED_WATER_FAUCET                   P12.7
#define pUV_LED_ICE_FAUCET_1                   P12.6
#define pUV_LED_ICE_FAUCET_2                   P12.5
#define pUV_LED_ICE_TANK_1_2                   P0.7
#define pUV_LED_ICE_TANK_3                     P10.2
#define pUV_LED_ICE_TRAY_1_2                   P13.0

#define pAD_UV_WATER_FAUCET_IAD                P15.3    // ANI 11
#define pAD_UV_ICE_FAUCET_1_IAD                P15.4    // ANI 12
#define pAD_UV_ICE_FAUCET_2_IAD                P15.5    // ANI 13
#define pAD_UV_ICE_TANK_1_2_IAD                P15.6    // ANI 14
#define pAD_UV_ICE_TANK_3_IAD                  P10.0    // ANI 20
#define pAD_UV_ICE_TRAY_1_2_IAD                P14.7    // ANI 18
/******************************************************************************************************************************************/
#define pVALVE_HOT_COLD_OVERFLOW               P5.6       // 온수버퍼/냉수탱크 [NOS] 오버플로우 밸브 V1
#define pVALVE_ICE_WATER_EXTRACT               P5.5       // 얼음물 [FEED] 추출밸브 (2kg 추가) V2
#define pVALVE_ROOM_IN                         P5.4       // 정수 [FEED] 입수밸브 V3
#define pVALVE_COLD_DRAIN                      P5.3       // 냉수 [FEED] 드레인밸브 V4
#define pVALVE_COLD_IN                         P8.7       // 냉수 [FEED] 입수밸브 V5
#define pVALVE_ROOM_COLD_EXTRACT               P8.6       // 냉/정수 [FEED] 출수밸브 V6TJD
#define pVALVE_HOT_OUT                         P3.0       // 온수 [FEED] 출수밸브 V7
#define pVALVE_HOT_DRAIN                       P5.0       // 온수 [NOS] 드레인밸브 V8
#define pVALVE_ICE_TRAY_IN                     P5.1       // 트레이 [FEED] 입수밸브 V9
#define pSWING_BAR                             P5.2       // 전자석스윙바 V10

#define pVALVE_NOS                             P8.1       // 입수 [NOS] 밸브
#define pVALVE_HOT_IN                          P8.0       // 온수입수 [FEED] 밸브 (O)
/******************************************************************************************************************************************/
/* Ice InnerDoor Reed Added 2025-10-28 */
#define P_INNER_DOOR_REED_SW                    P12.1
#define GET_INNER_DOOR_REED_SW()                (P_INNER_DOOR_REED_SW)
/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
#endif
