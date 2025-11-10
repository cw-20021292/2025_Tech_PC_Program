/// @file   Lib_TimeScheduler.c"
/// @date
/// @author Jaejin Ham
/// @brief  라이브러리 타임 스케쥴러 라이브러리

#include "Global_header.h"


#define MAX_FUNCTION_POINTER                    20          // 스케쥴러는 20개 한정
#define MAX_FUNCTION_INTP_POINTER               5           // Timer Interrput 안에서 직접 수행하는 스케쥴러는 5개 한정

// 스케쥴러 관리 자료형
typedef struct {
    void    (*FunctionPointer[MAX_FUNCTION_POINTER])(void);     // 함수 포인터 입력
    U16     mu16Counter[MAX_FUNCTION_POINTER];                  // 함수 실행 주기 카운터, 카운트 되면서 주기적으로 함수를 실행함. Counter > CallTimeCycle에서 리셋됨
    U16     mu16CallTimeCycle[MAX_FUNCTION_POINTER];            // 함수 실행 주기 저장소
    U8      mu8ActCount[MAX_FUNCTION_POINTER];                  // 수행 횟수 제한이 있는지 체크 변수
}   typeTimeSchedule;


// 스케쥴러 관리 자료형
typedef struct {
    void    (*FunctionPointer[MAX_FUNCTION_INTP_POINTER])(void);     // 함수 포인터 입력
    U16     mu16Counter[MAX_FUNCTION_INTP_POINTER];                  // 함수 실행 주기 카운터, 카운트 되면서 주기적으로 함수를 실행함. Counter > CallTimeCycle에서 리셋됨
    U16     mu16CallTimeCycle[MAX_FUNCTION_INTP_POINTER];            // 함수 실행 주기 저장소
    U8      mu8ActCount[MAX_FUNCTION_INTP_POINTER];                  // 수행 횟수 제한이 있는지 체크 변수
}   type1msTimeInterruptSchedule;


typeTimeSchedule tTimeSceduleHandler;                       // 타임 스케쥴러 변수(While문에서 수행)
type1msTimeInterruptSchedule t1msTimerInterruptHandler;     // 1ms Timer Interrput 안에서 직접 수행(문제가 생기지 않도록 필요시만 사용하되 처리 루틴이 길지 않은 함수 배치 요망)



/// @brief  1ms TimerInterrupt에서 실행되어 스케쥴러의 기준 시간이 되는 함수
/// @param  void
/// @return void
void CounterTimeScheduler(void)
{
    U8 mu8i = 0;

    for (mu8i = 0 ; mu8i < MAX_FUNCTION_POINTER ; mu8i++)
    {
        if (tTimeSceduleHandler.mu16CallTimeCycle[mu8i] != 0)
        {
            if (tTimeSceduleHandler.mu16Counter[mu8i] <= tTimeSceduleHandler.mu16CallTimeCycle[mu8i])
            {
                tTimeSceduleHandler.mu16Counter[mu8i]++;
            }
        }
    }

    Go1msTimeInterrputScheduler();      // 1ms Timer Interrput 안에서 직접 수행 처리 Scheduler
}


/// @brief  Time Scheduler Initialize
/// @param  void
/// @return void
void InitializeTimeScheduler(void)
{
    U8 mu8i = 0;
    U8 mu8j = 0;

// While 문 안에서 처리
    for (mu8i = 0 ; mu8i < MAX_FUNCTION_POINTER ; mu8i++)
    {
        tTimeSceduleHandler.FunctionPointer[mu8i] = NULL;
        tTimeSceduleHandler.mu16Counter[mu8i] = 1;             // CallTimeCycle과 비교를 위해 1로 세팅
        tTimeSceduleHandler.mu16CallTimeCycle[mu8i] = 0;
        tTimeSceduleHandler.mu8ActCount[mu8i] = 0;
    }

// 1ms Interrput 안에서 처리
    for (mu8j = 0 ; mu8j < MAX_FUNCTION_INTP_POINTER ; mu8j++)
    {
        t1msTimerInterruptHandler.FunctionPointer[mu8j] = NULL;
        t1msTimerInterruptHandler.mu16Counter[mu8j] = 1;             // CallTimeCycle과 비교를 위해 1로 세팅
        t1msTimerInterruptHandler.mu16CallTimeCycle[mu8j] = 0;
        t1msTimerInterruptHandler.mu8ActCount[mu8j] = 0;
    }
}


/// @brief  Main 함수 While 문안에서 실행되며 실제 함수를 호출해 주는 함수
/// @param  void
/// @return void
void GoTimeScheduler(void)
{
    U8 mu8i = 0;

    for (mu8i = 0 ; mu8i < MAX_FUNCTION_POINTER ; mu8i++)
    {
        if (tTimeSceduleHandler.mu16CallTimeCycle[mu8i] != 0)
        {
            if (tTimeSceduleHandler.mu16Counter[mu8i] > tTimeSceduleHandler.mu16CallTimeCycle[mu8i])
            {
                tTimeSceduleHandler.mu16Counter[mu8i] = 1;
                tTimeSceduleHandler.FunctionPointer[mu8i]();

                if (tTimeSceduleHandler.mu8ActCount[mu8i])
                {   // 함수 수행 제한 횟수가 있는 경우
                    tTimeSceduleHandler.mu8ActCount[mu8i]--;     // 수행 횟수 차감

                    if (tTimeSceduleHandler.mu8ActCount[mu8i] == 0)
                    {       // 수행 횟수가 모두 차감된 경우 더이상 수행되지 않게 처리
                        tTimeSceduleHandler.mu16CallTimeCycle[mu8i] = 0;
                    }
                }
            }
        }
    }
}




/// @brief  스케쥴러에 함수를 등록 하는 함수
/// @param[in]  *tFuction 등록할 함수 포인터
///             mu16Timer 실행할 주기(시간, 1ms 단위)
///             mu8OneTimeCheck 1회만 수행하는 함수인지 체크 변수(0이면 계속 수행)
/// @return void
void SetupTimeScheduler(void (*tFuction)(void), U16 mu16Timer, U8 mu8ActionCount)
{
    U8 mu8i = 0;

    for (mu8i = 0 ; mu8i < MAX_FUNCTION_POINTER ; mu8i++ )
    {
        if (tTimeSceduleHandler.FunctionPointer[mu8i] == NULL)
        {
            tTimeSceduleHandler.FunctionPointer[mu8i] = tFuction;
            tTimeSceduleHandler.mu16CallTimeCycle[mu8i] = mu16Timer;
            tTimeSceduleHandler.mu8ActCount[mu8i] = mu8ActionCount;
            return;
        }
    }
}


/// @brief  정지된 함수의 호출을 다시 시작 하는 함수
/// @brief  함수의 호출 시간을 변경하고자 할 대도 사용됨
/// @param[in]  *tFuction 시작할 함수 포인터
///             mu16Timer 실행할 주기(시간, 1ms 단위)
///             mu8OneTimeCheck 1회만 수행하는 함수인지 체크 변수(0이면 계속 수행)
/// @return void
void StartTimeScheduler(void (*tFuction)(void), U16 mu16Timer, U8 mu8ActionCount)
{
    U8 mu8i = 0;

    for (mu8i = 0 ; mu8i < MAX_FUNCTION_POINTER ; mu8i++)
    {
        if ( (tTimeSceduleHandler.FunctionPointer[mu8i] == tFuction) ||
            (tTimeSceduleHandler.FunctionPointer[mu8i] == NULL) )
        {
            tTimeSceduleHandler.FunctionPointer[mu8i] = tFuction;
            tTimeSceduleHandler.mu16CallTimeCycle[mu8i] = mu16Timer;
            tTimeSceduleHandler.mu8ActCount[mu8i] = mu8ActionCount;
            return;
        }
    }
}


/// @brief  등록된 함수의 호출을 정지 하는 함수
/// @param[in]  *tFuction 정지할 함수 포인터
/// @return void
void StopTimeScheduler(void (*tFuction)(void))
{
    U8 mu8i = 0;

    for (mu8i = 0 ; mu8i < MAX_FUNCTION_POINTER ; mu8i++ )
    {
        if (tTimeSceduleHandler.FunctionPointer[mu8i] == tFuction)
        {
            tTimeSceduleHandler.mu16CallTimeCycle[mu8i] = 0;
            tTimeSceduleHandler.mu8ActCount[mu8i] = 0;
            return;
        }
    }
}


/// @brief  1ms Interrput 안에서 직접 실행되며 함수를 호출해 주는 함수
/// @param  void
/// @return void
void Go1msTimeInterrputScheduler(void)
{
    U8 mu8j = 0;

    for (mu8j = 0 ; mu8j < MAX_FUNCTION_INTP_POINTER ; mu8j++)
    {
        if (t1msTimerInterruptHandler.mu16CallTimeCycle[mu8j] != 0)
        {
            if (t1msTimerInterruptHandler.mu16Counter[mu8j] <= t1msTimerInterruptHandler.mu16CallTimeCycle[mu8j])
            {
                t1msTimerInterruptHandler.mu16Counter[mu8j]++;
            }

            if (t1msTimerInterruptHandler.mu16Counter[mu8j] > t1msTimerInterruptHandler.mu16CallTimeCycle[mu8j])
            {
                t1msTimerInterruptHandler.mu16Counter[mu8j] = 1;
                t1msTimerInterruptHandler.FunctionPointer[mu8j]();

                if (t1msTimerInterruptHandler.mu8ActCount[mu8j])
                {   // 함수 수행 제한 횟수가 있는 경우
                    t1msTimerInterruptHandler.mu8ActCount[mu8j]--;     // 수행 횟수 차감

                    if (t1msTimerInterruptHandler.mu8ActCount[mu8j] == 0)
                    {       // 수행 횟수가 모두 차감된 경우 더이상 수행되지 않게 처리
                        t1msTimerInterruptHandler.mu16CallTimeCycle[mu8j] = 0;
                    }
                }
            }
        }
    }
}


/// @brief  스케쥴러에 함수를 등록 하는 함수
/// @param[in]  *tFuction 등록할 함수 포인터
///             mu16Timer 실행할 주기(시간, 1ms 단위)
///             mu8OneTimeCheck 1회만 수행하는 함수인지 체크 변수(0이면 계속 수행)
/// @return void
void Setup1msTimeInterruptScheduler(void (*tFuction)(void), U16 mu16Timer, U8 mu8ActionCount)
{
    U8 mu8i = 0;

    for (mu8i = 0 ; mu8i < MAX_FUNCTION_POINTER ; mu8i++ )
    {
        if (t1msTimerInterruptHandler.FunctionPointer[mu8i] == NULL)
        {
            t1msTimerInterruptHandler.FunctionPointer[mu8i] = tFuction;
            t1msTimerInterruptHandler.mu16CallTimeCycle[mu8i] = mu16Timer;
            t1msTimerInterruptHandler.mu8ActCount[mu8i] = mu8ActionCount;
            return;
        }
    }
}


/// @brief  정지된 함수의 호출을 다시 시작 하는 함수
/// @brief  함수의 호출 시간을 변경하고자 할 대도 사용됨
/// @param[in]  *tFuction 시작할 함수 포인터
///             mu16Timer 실행할 주기(시간, 1ms 단위)
///             mu8OneTimeCheck 1회만 수행하는 함수인지 체크 변수(0이면 계속 수행)
/// @return void
void Start1msTimeInterruptScheduler(void (*tFuction)(void), U16 mu16Timer, U8 mu8ActionCount)
{
    U8 mu8i = 0;

    for (mu8i = 0 ; mu8i < MAX_FUNCTION_POINTER ; mu8i++)
    {
        if ( (t1msTimerInterruptHandler.FunctionPointer[mu8i] == tFuction) ||
            (t1msTimerInterruptHandler.FunctionPointer[mu8i] == NULL) )
        {
            t1msTimerInterruptHandler.FunctionPointer[mu8i] = tFuction;
            t1msTimerInterruptHandler.mu16CallTimeCycle[mu8i] = mu16Timer;
            t1msTimerInterruptHandler.mu8ActCount[mu8i] = mu8ActionCount;
            return;
        }
    }
}


/// @brief  등록된 함수의 호출을 정지 하는 함수
/// @param[in]  *tFuction 정지할 함수 포인터
/// @return void
void Stop1msTimeInterruptScheduler(void (*tFuction)(void))
{
    U8 mu8i = 0;

    for (mu8i = 0 ; mu8i < MAX_FUNCTION_POINTER ; mu8i++ )
    {
        if (t1msTimerInterruptHandler.FunctionPointer[mu8i] == tFuction)
        {
            t1msTimerInterruptHandler.mu16CallTimeCycle[mu8i] = 0;
            t1msTimerInterruptHandler.mu8ActCount[mu8i] = 0;
            return;
        }
    }
}





