/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Main.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#include    "Macrodriver.h"
#include    "Global_Variable.h"
#include    "Port_Define.h"
#include    "valve_extract.h"



void output_valve_room_cold_extract_feed5(void);
void output_valve_hot_out_feed_8(void);



/***********************************************************************************************************************/
TYPE_BYTE          U8RoomColdOutFeed6ValveONB;
#define            u8RoomColdOutFeed6ValveON                       U8RoomColdOutFeed6ValveONB.byte
#define            Bit0_RCOF6_Water_Out_State                      U8RoomColdOutFeed6ValveONB.Bit.b0
#define            Bit1_Flushing_ColdTank_Fill_State               U8RoomColdOutFeed6ValveONB.Bit.b1
#define            Bit2_Icewater_extract_state                     U8RoomColdOutFeed6ValveONB.Bit.b2
#define            Bit3_Cody_Water_Line_Clean_State                U8RoomColdOutFeed6ValveONB.Bit.b3  /* Cody Water Line Clean Service */

TYPE_BYTE          U8RoomColdOutFeed6ValveOFFB;
#define            u8RoomColdOutFeed6ValveOFF                      U8RoomColdOutFeed6ValveOFFB.byte
#define            Bit0_RCOF6_Off_State                            U8RoomColdOutFeed6ValveOFFB.Bit.b0
#define            Bit1_Flushing_ColdTank_Fill_Off_State           U8RoomColdOutFeed6ValveOFFB.Bit.b1
#define            Bit2_CD_Circul_Drain_Off_State                  U8RoomColdOutFeed6ValveOFFB.Bit.b2



/***********************************************************************************************************************/
TYPE_BYTE          U8HotOutFeed7ValveONB;
#define            u8HotOutFeed7ValveON                            U8HotOutFeed7ValveONB.byte
#define            Bit0_HOF7_Water_Out_State                       U8HotOutFeed7ValveONB.Bit.b0
#define            Bit1_HOF7_Hot_Drain_Cold                        U8HotOutFeed7ValveONB.Bit.b1

TYPE_BYTE          U8HotOutFeed7ValveOFFB;
#define            u8HotOutFeed7ValveOFF                           U8HotOutFeed7ValveOFFB.byte
#define            Bit0_HOF7_Off_State                             U8HotOutFeed7ValveOFFB.Bit.b0



/***********************************************************************************************************************/


U8 gu8LockLEDFlick;

bit F_LineTest;                   // 라인 테스트
bit F_LineTest2;                  // 라인 테스트2
//bit F_PartTest;                   // 파트 테스트
//bit F_PBATest;                    // PBA TEST
//bit F_NfcTest;

//===================================== Part Test
bit F_PartHeater;
bit F_PartComp;
bit F_PartIceHeater;
bit F_PartTrayVV;
bit F_PartColdVV;
bit F_PartTrayCW;
bit F_PartDrainPump;
bit F_PartCristal;
bit F_PartIR;
bit F_PartIce;


U8 gu8_hot_close_delay_timer;

bit bit_temporary_hot_stop;
U16 gu16_temporary_hot_stop_timer;
extern bit F_Circul_Drain;
extern bit F_DrainStatus;

extern bit F_auto_drain_mode_cold_water_valve_out;
extern FLUSHING_STEP gu8_flushing_mode;
extern COLD_TANK_FILL_STEP gu8_cold_tank_fill_step;
extern bit F_IceOut;
extern FLUSHING_STEP gu8_Ice_Out_Step;
extern U8 gu8_hot_drain_cold;
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void output_valve_room_cold_extract_feed5(void)
{
    if( F_WaterOut == SET )
    {
        if(u8IceOutState != ICE_SELECT__ICE_WATER)
        {
            if( u8WaterOutState == COLD_WATER_SELECT 
            || u8WaterOutState == PURE_WATER_SELECT )
            {
                if( gu8_Water_Out_Step >= STATE_30_EXTRACT_VALVE_ON_STATE
                && gu8_Water_Out_Step < STATE_40_EXTRACT_VALVE_OFF_STATE )
                {
                    Bit0_RCOF6_Water_Out_State = SET;
                }
                else
                {
                    Bit0_RCOF6_Water_Out_State = CLEAR;
                }
            }
            else
            {
                Bit0_RCOF6_Water_Out_State = CLEAR;
            }
        }
        else
        {
            Bit0_RCOF6_Water_Out_State = CLEAR;
        }
    }
    else
    {
        Bit0_RCOF6_Water_Out_State = CLEAR;
    }

    /* 코디유로살균 Cody Water Line Clean Service */
    if(cody_water_line.gu8_start == SET)
    {
        if((cody_water_line.gu8_step == 0)
        || (cody_water_line.gu8_step == 2)
        || (cody_water_line.gu8_step == 4)
        )
        {
            Bit3_Cody_Water_Line_Clean_State = SET;
        }
        else
        {
            Bit3_Cody_Water_Line_Clean_State = CLEAR;
        }
    }
    else
    {
        Bit3_Cody_Water_Line_Clean_State = CLEAR;
    }

    /* 순환배수 시 추출밸브 잠금 250620 CH.PARK */
    if( F_Circul_Drain == SET )
    {
        if( F_DrainStatus == SET )
        {
            /*..hui [23-8-25???? 1:19:18] ?????? ???? ??..*/
            if(F_auto_drain_mode_cold_water_valve_out == SET)
            {
                Bit2_CD_Circul_Drain_Off_State = SET;
            }
            else
            {
                Bit2_CD_Circul_Drain_Off_State  = CLEAR;
            }
        }
        else
        {
            Bit2_CD_Circul_Drain_Off_State  = CLEAR;
        }
    }
    else
    {
        Bit2_CD_Circul_Drain_Off_State  = CLEAR;
    }

/***********************************************************************************************/
    if (u8RoomColdOutFeed6ValveOFF > 0)
    {
        pVALVE_ROOM_COLD_EXTRACT = CLEAR;  /*off*/
    }
    else
    {
        if (u8RoomColdOutFeed6ValveON > 0)
        {
            pVALVE_ROOM_COLD_EXTRACT = SET; /*on*/
        }
        else
        {
            pVALVE_ROOM_COLD_EXTRACT = CLEAR;  /*off*/
        }
    }
/***********************************************************************************************/

}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void output_valve_hot_out_feed_8(void)
{
    #if 1
    if( F_WaterOut == SET )
    {
        if( u8WaterOutState == HOT_WATER_SELECT )
        {
            if( gu8_Water_Out_Step >= STATE_30_EXTRACT_VALVE_ON_STATE
                && gu8_Water_Out_Step < STATE_40_EXTRACT_VALVE_OFF_STATE )
            {
                Bit0_HOF7_Water_Out_State = F_WaterOut;
            }
            else
            {
                Bit0_HOF7_Water_Out_State = CLEAR;
            }
        }
        else
        {
            Bit0_HOF7_Water_Out_State = CLEAR;
        }
    }
    else
    {
        Bit0_HOF7_Water_Out_State = CLEAR;
    }
    #endif

    if(gu8_hot_drain_cold == SET)
    {
        Bit1_HOF7_Hot_Drain_Cold = SET;
    }
    else
    {
        Bit1_HOF7_Hot_Drain_Cold = CLEAR;
    }

/***********************************************************************************************/
    if (u8HotOutFeed7ValveOFF > 0)
    {
        pVALVE_HOT_OUT = CLEAR;  /*off*/
    }
    else
    {
        if (u8HotOutFeed7ValveON > 0)
        {
            pVALVE_HOT_OUT = SET; /*on*/
        }
        else
        {
            pVALVE_HOT_OUT = CLEAR;  /*off*/
        }
    }
/***********************************************************************************************/
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/







/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/






