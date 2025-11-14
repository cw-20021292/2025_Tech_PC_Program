#ifndef _PARSER_H_
#define _PARSER_H_

#include "Macrodriver.h"


U8 ConvertTxModeToCmd(U16 u16TxMode);
U8 ConvertTxModeToLength(U16 u16TxMode);
U16 CRC_Cal_V2(U8 *puchMsg, U8 usDataLen);

#endif
