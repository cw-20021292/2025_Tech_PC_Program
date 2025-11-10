/// @file   Lib_OperationMode.h
/// @date
/// @author Jaejin Ham
/// @brief  동작 모드 주관 헤더 파일

#ifndef __LIB_OPERATION_MODE_H__
#define __LIB_OPERATION_MODE_H__


#define OPERATION_MODE_NONE             0
#define OPERATION_MODE_NORMAL           1
#define OPERATION_MODE_PCB_TEST         2
#define OPERATION_MODE_FRONT_TEST       3
#define OPERATION_MODE_PTA_TEST_1       4
#define OPERATION_MODE_PTA_TEST_2       5
#define OPERATION_MODE_EXTRA_TEST_1     6
#define OPERATION_MODE_EXTRA_TEST_2     7


void InitializeOperationMode(void);
void ChangeOperationMode(U8 mu8OperationMode);
void OperationModeSetup(void);

#endif

