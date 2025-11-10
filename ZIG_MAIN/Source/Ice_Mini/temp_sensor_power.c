/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Main.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  : AT+CONNECT=74F07DB01010
***********************************************************************************************************************/
#include    "Macrodriver.h"
#include    "Global_Variable.h"
#include    "Port_Define.h"
#include    "temp_sensor_power.h"

void output_temp_power(void);
void output_room_temp_power(void);
void output_cold_temp_power(void);

/***********************************************************************************************************************/
TYPE_BYTE          U8RoomTempPowerONB;
#define            u8RoomTempPowerON                        U8RoomTempPowerONB.byte
#define            Bit0_Room_Temp_Ice_Making_Read_State     U8RoomTempPowerONB.Bit.b0
#define            Bit1_Room_Temp_Hot_Extarct_State         U8RoomTempPowerONB.Bit.b1
#define            Bit2_Room_Temp_Error_State               U8RoomTempPowerONB.Bit.b2
#define            Bit3_Room_Temp_Init_State                U8RoomTempPowerONB.Bit.b3
#define            Bit4_Room_Temp_Doctor_Test_State         U8RoomTempPowerONB.Bit.b4
#define            Bit5_Room_Temp_Periodic_Read_State       U8RoomTempPowerONB.Bit.b5
#define            Bit6_Room_Temp_Self_Test_State           U8RoomTempPowerONB.Bit.b6


TYPE_BYTE          U8ColdTempPowerONB;
#define            u8ColdTempPowerON                        U8ColdTempPowerONB.byte
#define            Bit0_Cold_Temp_Periodic_Read_State       U8ColdTempPowerONB.Bit.b0
#define            Bit1_Cold_Temp_Error_State               U8ColdTempPowerONB.Bit.b1
#define            Bit2_Cold_Temp_Init_State                U8ColdTempPowerONB.Bit.b2
#define            Bit3_Cold_Temp_Doctor_Test_State         U8ColdTempPowerONB.Bit.b3
#define            Bit4_Cold_Temp_Extract_State             U8ColdTempPowerONB.Bit.b4
#define            Bit5_Cold_Temp_Self_Test_State           U8ColdTempPowerONB.Bit.b5


/***********************************************************************************************************************/

U16 u16_cold_power_timer;
U16 u16_room_power_timer;

U16 u16_temp_power_init_timer;

U8 u8_cold_adc_on_delay_timer;
U8 u8_cold_adc_off_delay_timer;
bit bit_adc_cold_start;

U8 u8_room_adc_on_delay_timer;
U8 u8_room_adc_off_delay_timer;
bit bit_adc_room_start;


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void output_temp_power(void)
{
    output_room_temp_power();
    output_cold_temp_power();

    //pROOM_TH_POWER = SET;
    //pCOLD_TH_POWER = SET;
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void output_room_temp_power(void)
{
    ////////////////////////////////////////////////////////////////////////////////////////////
    /*..hui [19-12-19오후 1:30:08] 제빙할때 아이스트레이 입수 ~ 컴프ON 이전까지 전원 ON..*/
    if(  gu8IceStep >= STATE_20_WATER_IN_ICE_TRAY
      && gu8IceStep <= STATE_30_CALC_ICE_MAKING_TIME )
    {
        Bit0_Room_Temp_Ice_Making_Read_State = SET;
    }
    else
    {
        Bit0_Room_Temp_Ice_Making_Read_State = CLEAR;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////
    /*..hui [19-12-19오후 1:36:43] 정수센서 에러 발생 시 전원 지속 ON / 자동해제 빨리..*/
    /*Bit2_Room_Temp_Error_State = Bit2_Room_Temp_Open_Short_Error__E42;*/

    /*..hui [23-12-18오후 2:22:07] 정수 과열에러일때도 항시 ON..*/
    Bit2_Room_Temp_Error_State = Bit2_Room_Temp_Open_Short_Error__E42;

    ////////////////////////////////////////////////////////////////////////////////////////////
    /*..hui [19-12-19오후 5:51:06] 초기 전원 인가 후 1분 동안 감지..*/
    u16_temp_power_init_timer++;

    /*..hui [23-6-9오후 5:59:57] 10분으로 변경.. 제어사양서 통일..*/
    if(u16_temp_power_init_timer >= 6000)
    {
        u16_temp_power_init_timer = 6000;
        Bit3_Room_Temp_Init_State = CLEAR;
    }
    else
    {
        Bit3_Room_Temp_Init_State = SET;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////
    /*..hui [20-2-17오후 5:58:13] 코디 테스트 모드 및 개발자 모드에서는 ON..*/


    u16_room_power_timer++;

    if( u16_room_power_timer <= COLD_TEMP_POWER_ON_TIME)
    {
        Bit5_Room_Temp_Periodic_Read_State = SET;
    }
    else
    {
        Bit5_Room_Temp_Periodic_Read_State = CLEAR;

        if( u16_room_power_timer >= COLD_TEMP_POWER_OFF_TIME)
        {
            u16_room_power_timer = 0;
        }
        else{}
    }

    Bit6_Room_Temp_Self_Test_State = bit_self_test_start;

    ////////////////////////////////////////////////////////////////////////////////////////////
    if( u8RoomTempPowerON > 0 )
    {
        /*..hui [19-12-19오후 1:26:46] 정수 온도센서 전원 ON..*/
        pROOM_TH_POWER = SET;

        /*u8_room_adc_off_delay_timer = 0;*/

        u8_room_adc_on_delay_timer++;

        /*..hui [20-1-22오후 10:00:48] 전원 켜고 1s 후 온도센서 읽음..*/
        if( u8_room_adc_on_delay_timer >= ADC_DELAY_ON_DELAYTIME )
        {
            u8_room_adc_on_delay_timer = ADC_DELAY_ON_DELAYTIME;
            bit_adc_room_start = SET;
        }
        else{}
    }
    else
    {
        /*..hui [19-12-19오후 1:26:51] 정수 온도센서 전원 OFF..*/
        /*..hui [20-1-29오후 1:51:28] 끌때 지연시간은 삭제..*/
        pROOM_TH_POWER = CLEAR;
        bit_adc_room_start = CLEAR;

        u8_room_adc_on_delay_timer = 0;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void output_cold_temp_power(void)
{

    ////////////////////////////////////////////////////////////////////////////////////////////
    /*..hui [19-12-19오후 2:14:18] 10초 ON 50초 OFF 반복..*/
    /*..hui [19-12-19오후 4:53:10] 냉수 켜져있을 경우에만..*/
    if( F_Cold_Enable == SET )
    {
        u16_cold_power_timer++;

        if( u16_cold_power_timer <= COLD_TEMP_POWER_ON_TIME)
        {
            Bit0_Cold_Temp_Periodic_Read_State = SET;
        }
        else
        {
            Bit0_Cold_Temp_Periodic_Read_State = CLEAR;

            if( u16_cold_power_timer >= COLD_TEMP_POWER_OFF_TIME)
            {
                u16_cold_power_timer = 0;
            }
            else{}
        }
    }
    else
    {
        u16_cold_power_timer = 0;
        Bit0_Cold_Temp_Periodic_Read_State = CLEAR;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////
    /*..hui [19-12-19오후 1:36:43] 냉수센서 에러 발생 시 전원 지속 ON / 자동해제 빨리..*/
    Bit1_Cold_Temp_Error_State = Bit14_Cold_Temp_Open_Short_Error__E44;

    ////////////////////////////////////////////////////////////////////////////////////////////
    /*..hui [19-12-19오후 5:51:06] 초기 전원 인가 후 1분 동안 감지..*/
    Bit2_Cold_Temp_Init_State = Bit3_Room_Temp_Init_State;

    ////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////
    if( F_WaterOut == SET )
    {

        /*..hui [23-12-18오후 3:03:44] 냉수 추출할떄도 ON.. 냉수 추출할때 냉수 온도 변화가 생길수 있기때문..*/
        Bit4_Cold_Temp_Extract_State = SET;
    }
    else
    {
        /*..hui [23-12-18오후 2:27:27] 물추출 안하더라도 온수 선택 상태에서는 7초 동안 지속적으로 읽도록..*/
        if( u8WaterOutState == HOT_WATER_SELECT )
        {
            Bit4_Cold_Temp_Extract_State = SET;
        }
        else
        {
            Bit4_Cold_Temp_Extract_State = CLEAR;
        }
    }

    Bit5_Cold_Temp_Self_Test_State = bit_self_test_start;

    ////////////////////////////////////////////////////////////////////////////////////////////
    if( u8ColdTempPowerON > 0 )
    {
        /*..hui [19-12-19오후 1:27:21] 냉수 온도센서 전원 ON..*/
        pCOLD_TH_POWER = SET;

        /*u8_cold_adc_off_delay_timer = 0;*/

        u8_cold_adc_on_delay_timer++;

        /*..hui [20-1-22오후 10:00:48] 전원 켜고 500ms 후 온도센서 읽음..*/
        if( u8_cold_adc_on_delay_timer >= ADC_DELAY_ON_DELAYTIME )
        {
            u8_cold_adc_on_delay_timer = ADC_DELAY_ON_DELAYTIME;
            bit_adc_cold_start = SET;
        }
        else{}
    }
    else
    {
        /*..hui [19-12-19오후 1:27:27] 냉수 온도센서 전원 OFF..*/
        /*..hui [20-1-29오후 1:51:28] 끌때 지연시간은 삭제..*/
        pCOLD_TH_POWER = CLEAR;
        bit_adc_cold_start = CLEAR;

        u8_cold_adc_on_delay_timer = 0;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/




