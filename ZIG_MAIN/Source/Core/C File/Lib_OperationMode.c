/// @file   Lib_OperationMode.c
/// @date
/// @author Jaejin Ham
/// @brief  동작 모드 주관 헤더 파일

#include "Global_header.h"


U8 gu8OperaionMode = 0;                 // 현재 동작 중인 모드
U8 gu8ChangeOperaionMode = 0;           // 변경할 동작 모드


/// @brief  메인 동작 모드 초기화 함수
/// @param  void
/// @return void
void InitializeOperationMode(void)
{
    gu8OperaionMode = OPERATION_MODE_NONE;
}


/// @brief  동작 모드 변경 함수
/// @param  void
/// @return void
void ChangeOperationMode(U8 mu8OperationMode)
{
    gu8ChangeOperaionMode = mu8OperationMode;
}


/// @brief  메인 동작 제어 세팅 함수
/// @param  void
/// @return void
void OperationModeSetup(void)
{
    switch (gu8ChangeOperaionMode)
    {
        case OPERATION_MODE_NORMAL:
            if (gu8OperaionMode != OPERATION_MODE_NORMAL)
            {
                gu8OperaionMode = OPERATION_MODE_NORMAL;
                InitializeTimeScheduler();                  // 스케쥴러 초기화

                SetupTimeScheduler(AD_Convert_Control, 1, ACT_COUNT_INFINITE);      // AD Convert 1ms 루틴 동작
// 예)           SetupTimeScheduler(exFunction, 100, ACT_COUNT_INFINITE);            // 새로운 동작 함수 스케쥴 세팅
                SetupTimeScheduler(WifiControlProcess100ms, 100, ACT_COUNT_INFINITE);   // Wi-Fi 100ms Scheduler 동작
            }
            break;

        case OPERATION_MODE_PCB_TEST:
            if (gu8OperaionMode != OPERATION_MODE_PCB_TEST)
            {
                gu8OperaionMode = OPERATION_MODE_PCB_TEST;
                InitializeTimeScheduler();

                SetupTimeScheduler(AD_Convert_Control, 1, ACT_COUNT_INFINITE);      // AD Convert 1ms 루틴 동작
            }
            break;

        case OPERATION_MODE_FRONT_TEST:
            if (gu8OperaionMode != OPERATION_MODE_FRONT_TEST)
            {
                gu8OperaionMode = OPERATION_MODE_FRONT_TEST;
                InitializeTimeScheduler();

                SetupTimeScheduler(AD_Convert_Control, 1, ACT_COUNT_INFINITE);      // AD Convert 1ms 루틴 동작
            }
            break;

        case OPERATION_MODE_PTA_TEST_1:
            if (gu8OperaionMode != OPERATION_MODE_PTA_TEST_1)
            {
                gu8OperaionMode = OPERATION_MODE_PTA_TEST_1;
                InitializeTimeScheduler();

                SetupTimeScheduler(AD_Convert_Control, 1, ACT_COUNT_INFINITE);      // AD Convert 1ms 루틴 동작
            }
            break;

        case OPERATION_MODE_PTA_TEST_2:
            if (gu8OperaionMode != OPERATION_MODE_PTA_TEST_2)
            {
                gu8OperaionMode = OPERATION_MODE_PTA_TEST_2;
                InitializeTimeScheduler();

                SetupTimeScheduler(AD_Convert_Control, 1, ACT_COUNT_INFINITE);      // AD Convert 1ms 루틴 동작
            }
            break;

        default:
            break;
    }
}




