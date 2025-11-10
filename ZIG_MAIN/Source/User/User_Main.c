/// @encoding euc-kr
/// @file   User_Main.c
/// @date
/// @author Jaejin Ham
/// @brief  Program Main File

/// @mainpage 프로그램 메인 페이지
/// @section intro 소개
/// - 소개 :
/// @section Program 프로그램명
/// - 프로그램명  :
/// - 프로그램 내용 :
/// @section info 개발 목적
/// -
/// @section advenced 사용 CPU
/// -
/// @section CREATEINFO 작성정보
/// - 작성자      :   함재진
/// - 작성일      :
/// @section MODIFYINFO 수정정보
/// -


#include "Global_header.h"


/// @brief    delay 함수
/// @param    mu8Time : delay 시간(8us)
/// @return   void
void Delay(U16 mu8Time)
{
  U16 mu8i;

  for (mu8i = 0 ; mu8i <= mu8Time ; mu8i++)
  {
    R_WDT_Restart();
  }
}


/// @brief    delay ms 함수
/// @param    mu8Time : delay 시간(ms)
/// @return   void
void Delay_ms(U16 mu16Time)
{
  U16 mu16i = 0;
  U16 mu16j = 0;

  for (mu16i = 0 ; mu16i <= (mu16Time*2) ; mu16i++)
  {
    for (mu16j = 0 ; mu16j <= 830 ; mu16j++)
    {   // constant value for 1msec counter = 830
      R_WDT_Restart();
    }
  }
}


/// @brief    HEX to Decimal
/// @param    mu8HexData : HEX Data
/// @return   mu8ReturnData : DEC Data
U8 HEX2DEC(U8 mu8HexData)
{
  U8 mu8ReturnData = 0;

  mu8ReturnData = (((mu8HexData >> 4) & 0x0F) * 10) + (mu8HexData & 0x0F);

  return  mu8ReturnData;
}


/// @brief    Decimal to HEX
/// @param    mu8DecData : DEC Data
/// @return   mu8ReturnData : HEX Data
U8 DEC2HEX(U8 mu8DecData)
{
  U8 mu8ReturnData = 0;

  mu8ReturnData = (((mu8DecData / 10) << 4) & 0xF0) + (mu8DecData % 10);

  return  mu8ReturnData;
}


/// @brief    HEX to Decimal
/// @param    mu16HexData : HEX Data
/// @return   mu16ReturnData : DEC Data
U16 Bit16HEX2DEC(U16 mu16HexData)
{
  U16 mu16ReturnData = 0;

  mu16ReturnData = ((mu16HexData >> 12) & 0x000F) * 1000;
  mu16ReturnData += ((mu16HexData >> 8) & 0x000F) * 100;
  mu16ReturnData += ((mu16HexData >> 4) & 0x000F) * 10;
  mu16ReturnData += mu16HexData & 0x000F;

  return  mu16ReturnData;
}


/// @brief    Decimal to HEX
/// @param    mu16DecData : DEC Data
/// @return   mu16ReturnData : HEX Data
U16 DEC2Bit16HEX(U16 mu16DecData)
{
  U16 mu16ReturnData = 0;

  mu16ReturnData = ((mu16DecData / 1000) << 12) & 0xF000;
  mu16ReturnData += (((mu16DecData % 1000) / 100) << 8) & 0x0F00;
  mu16ReturnData += (((mu16DecData % 100) / 10) << 4) & 0x00F0;
  mu16ReturnData += (mu16DecData % 10) & 0x000F;

  return  mu16ReturnData;
}


/// @brief    Ascii to HEX
/// @param    mu8HexData : Hex Data
/// @return   mu8ReturnData : Ascii Data
U8 HEX2ASCII(U8 mu8HexData)
{
  U8 mu8ReturnData = 0;

  if (mu8HexData < 0x0A)
  {
    mu8ReturnData = mu8HexData | 0x30;
  }
  else
  {
    mu8ReturnData = (mu8HexData - 9) | 0x40;
  }

  return  mu8ReturnData;
}


/// @brief    Ascii to HEX
/// @param    mu8AsciiData : Ascii Data
/// @return   mu8ReturnData : Hex Data
U8 ASCII2HEX(U8 mu8AsciiData)
{
  U8 mu8ReturnData = 0;

  if (mu8AsciiData >= 0x41)
  {
    mu8ReturnData = (mu8AsciiData & ~0x40) + 9;
  }
  else
  {
    mu8ReturnData = mu8AsciiData & ~0x30;
  }

  return  mu8ReturnData;
}



/// @brief    Interrupt(Timer, ADC, External 등) Start 함수
/// @param    void
/// @return   void
void Interrupt_Start(void)
{
    R_TAU0_Channel0_Start();        // Timer Interrupt Start 수행
}


/// @brief    통신 UART들 모두  Start 함수
/// @param    void
/// @return   void
void UART_Start(void)
{
    R_UART1_Start();    // Wi-Fi Uart1
}


/// @brief    모든 변수 초기화 함수
/// @param    void
/// @return   void
void Main_Initailize(void)
{
    AD_Convert_Initialize();
    InitializeTimeScheduler();
    InitializeOperationMode();
}



// ******************************************************************************
// *                                                                            *
// *                        Program Main Fuction                                *
// *                                                                            *
// ******************************************************************************
/// @brief    Main function
/// @param    void
/// @return   void
void User_Main(void)
{
    DI();
    /* --- R_Systeminit START --- */
    PIOR = 0x00U;

    R_CGC_Get_ResetSource();
    R_CGC_Create();
    R_PORT_Create();
    R_SAU0_Create();
    R_ADC_Create();
    R_TAU0_Create();
    R_TAU1_Create();
    R_WDT_Create();
    R_INTC_Create();

    IAWCTL = 0x00U;
    /* --- R_Systeminit END --- */

    Interrupt_Start();      // 각종 Interrupt들 시작 수행

    Delay_ms(1000);         // 전압 안정화 시간 확보. While문 진입 이후에는 Delay 함수 사용 자제(Time Scheduler 밀릴 수 있음)

    Main_Initailize();      // 동작 전 모든 변수 초기화

//    ModelCheck();         // Model Type(CHP,CP,HP) Check

    EI();

    UART_Start();           // 각종 UART들 시작 수행

    WifiControlProcess(WIFI_TIME_INI);    // Wi-Fi Initialize. EEPROM Initialize 이전에 실행

    /* --- EEPROM Initialize START --- */
      // EEPROM_PRIMARY();     // EEPROM Initialize
    /* --- EEPROM Initialize END--- */

    WifiControlProcess(WIFI_TIME_SET);    // Wi-Fi WifiOnOffSetting. EEPROM Initialize 이후에 실행
    ChangeOperationMode(OPERATION_MODE_NORMAL);     // 일반 모드로 동작 시작 할 수 있도록 세팅
    

    while (1)
    {
        R_WDT_Restart();
        OperationModeSetup();       // 동작 모드 설정
        GoTimeScheduler();          // Time Scheduler 제어 동작
        WifiControlProcess(WIFI_TIME_WHILE);    // Wi-Fi While 동작
    }
}





