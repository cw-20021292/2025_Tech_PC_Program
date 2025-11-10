
#ifndef _WIFI_RxProcess_H_
#define _WIFI_RxProcess_H_

/***** Variable ***************************************************************/
typedef struct _wifirx_
{
    U8  u8State;        // RxºÐ¼® (0:Rx°¡´É / 1:ºÐ¼®Áß)
    U8	u8Wait;         // ´ë±âÁßÀÎ µ¥ÀÌÅÍ (0:¾øÀ½ / 1: ÀÖÀ½)
    U8  u8CheckModem;   // °øÀ¯±â Ã¹ ¿¬°áÈ®ÀÎ
    U8  u8CheckServer;  // ¼­¹ö Ã¹ ¿¬°áÈ®ÀÎ
    U8  u8ConnectCheck; // Uart ¼ö½ÅÈ®ÀÎ
    U16 u16ConnectCheckCount;  // ¸ðµâ°úÀÇ ¿¬°áÈ®ÀÎ (5ºÐÄ«¿îÆ®)
    U8  u8DisplayConnectCount; // 0: Clear, 1 : SET(¿¬°á²÷±è 1ºÐ Ä«¿îÆ® Áß),  2: Done (BLEÀÚµ¿È°¼ºÈ­ ¿Ï·á- ¿¬°á²÷±è ÈÄ, ÃÊ±âÆä¾î¸µ ½Ã)
} WifiRx_T;

typedef U8 (*WifiRxAtFun_T)(void);
typedef struct _wifi_rxat_list_
{
    I8 *Id;
    U8  Disp;
    U8  Err;
    U8  ApWifi;
    U8  ApBle;
    U8  ApStep;
    U8  Connect;
    U16  Sound;
    WifiRxAtFun_T Func;
} WifiRxAtList_T;

/*******************************************************************************
FUNCTION NAME: InitializeWifiProcessRx

DESCRIPTION: Initialize Rx Value

PARAMETERS: X

RETURN VALUE: X

NOTES: $)CCJ1b 1H8 ( Control )
*******************************************************************************/
void InitializeWifiRxProcess ( void );

/*******************************************************************************
FUNCTION NAME: GetWifiUartRxData

DESCRIPTION: Get uart Rx Data

PARAMETERS: mai8UartData

RETURN VALUE: X

NOTES: Int $)CH#Cb ( UartInterrupt )
*******************************************************************************/
void GetWifiUartRxData ( I8* mai8UartData );

/*******************************************************************************
FUNCTION NAME: WifiRxDataProcess

DESCRIPTION: Rx Data Process

PARAMETERS: X

RETURN VALUE: X

NOTES: While ( Control )
*******************************************************************************/
void WifiRxDataProcess ( void );

/*******************************************************************************
FUNCTION NAME: WifiRxAckTimer

DESCRIPTION: Module Connect Check

PARAMETERS: X

RETURN VALUE: X

NOTES: 100ms ( DataCheck )
*******************************************************************************/
// void WifiRxAckTimer ( void );

void ClearRxStatus ( void );

#endif

