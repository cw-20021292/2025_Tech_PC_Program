/// @file   Main.h
/// @date
/// @author Jaejin Ham
/// @brief  Program Main Header File

#include "Global_Header.h"

#ifndef __MAIN_H__
#define __MAIN_H__


void Delay(U16 mu8Time);
void Delay_ms(U16 mu8Time);
U8 HEX2DEC(U8 mu8HexData);
U8 DEC2HEX(U8 mu8DecData);
U16 Bit16HEX2DEC(U16 mu16HexData);
U16 DEC2Bit16HEX(U16 mu16DecData);
U8 HEX2ASCII(U8 mu8HexData);
U8 ASCII2HEX(U8 mu8AsciiData);

void Interrupt_Start(void);
void UART_Start(void);
void Main_Initailize(void);
void User_Main(void);

#endif

