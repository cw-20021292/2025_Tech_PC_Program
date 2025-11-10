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
#include    "error_temp_sensor.h"


void check_error_temp_sensor(void);
/*void check_error_hot_heater_temp_sensor(void);*/
void check_error_cold_temp_sensor_open_short(void);
void check_error_room_temp_sensor_open_short(void);
void check_error_amb_temp_sensor_open_short(void);
//void check_error_eva_first_temp_sensor_open_short(void);
//void check_error_eva_second_temp_sensor_open_short(void);

void check_error_amb_side_temp_sensor_open_short(void);
void check_error_hot_out_temp_sensor(void);
void check_error_hot_in_temp_sensor_open_short(void);
void check_error_hot_heater_temp_sensor(void);


U8 gu8_hot_out_temp_error_check_timer;
U8 gu8_hot_out_temp_error_clear_timer;

U8 gu8_hot_heater_temp_error_check_timer;
U8 gu8_hot_heater_temp_error_clear_timer;

U8 gu8_hot_tank_temp_error_check_timer;
U8 gu8_hot_tank_temp_error_clear_timer;

U8 gu8_hot_heater_temp_error_check_timer;
U8 gu8_hot_heater_temp_error_clear_timer;

U8 gu8_cold_temp_error_check_timer;
U8 gu8_cold_temp_error_clear_timer;

U8 gu8_room_temp_error_check_timer;
U8 gu8_room_temp_error_clear_timer;

U8 gu8_amb_temp_error_clear_timer;
U8 gu8_amb_temp_error_check_timer;

U8 gu8_eva_first_temp_error_check_timer;
U8 gu8_eva_first_temp_error_clear_timer;

U8 gu8_eva_second_temp_error_check_timer;
U8 gu8_eva_second_temp_error_clear_timer;

U8 gu8_amb_side_temp_error_clear_timer;
U8 gu8_amb_side_temp_error_check_timer;

U8 gu8_tray_temp_error_clear_timer;
U8 gu8_tray_temp_error_check_timer;

U8 gu8_mixing_temp_error_clear_timer;
U8 gu8_mixing_temp_error_check_timer;

U8 gu8_hot_in_temp_error_check_timer;
U8 gu8_hot_in_temp_error_clear_timer;

bit bit_cold_read_finish;
bit bit_hot_read_finish;
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void check_error_temp_sensor(void)
{
    /*..hui [19-6-26오전 11:16:00] 온수 출수 온도센서..*/
    check_error_hot_out_temp_sensor();

    /*..hui [19-6-26오전 11:16:09] 온수 히터 내부 온도센서..*/
    check_error_hot_heater_temp_sensor();

    /*..hui [21-5-11오후 2:49:27] 온수 입수 온도센서..*/
    check_error_hot_in_temp_sensor_open_short();

    /*..hui [17-12-29오후 11:39:53] 냉수센서 오픈 쇼트 에러..*/
    check_error_cold_temp_sensor_open_short();

    /*..hui [19-6-26오전 11:16:15] 정수 온도센서..*/
    check_error_room_temp_sensor_open_short();

    /*..hui [19-6-26오전 11:16:19] 주변 온도센서..*/
    check_error_amb_temp_sensor_open_short();
    check_error_amb_side_temp_sensor_open_short();
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void check_error_hot_out_temp_sensor(void)
{

    if( gu16_AD_Result_Hot_Out >= TEMPERATURE_SENSOR_OPEN
        || gu16_AD_Result_Hot_Out <= TEMPERATURE_SENSOR_SHORT )
    {
        gu8_hot_out_temp_error_clear_timer = 0;
        gu8_hot_out_temp_error_check_timer++;

        if( gu8_hot_out_temp_error_check_timer >= TEMPERATURE_SENSOR_ERROR_CHECK_TIME )
        {
            gu8_hot_out_temp_error_check_timer = TEMPERATURE_SENSOR_ERROR_CHECK_TIME;
            Bit20_Hot_Out_Temp_Open_Short_Error__E47 = SET;
        }
        else{}
    }
    else
    {
        gu8_hot_out_temp_error_check_timer = 0;
        gu8_hot_out_temp_error_clear_timer++;

        if ( gu8_hot_out_temp_error_clear_timer >= TEMPERATURE_SENSOR_ERROR_CHECK_TIME )
        {
            gu8_hot_out_temp_error_clear_timer = TEMPERATURE_SENSOR_ERROR_CHECK_TIME;
            Bit20_Hot_Out_Temp_Open_Short_Error__E47 = CLEAR;
        }
        else{}

    }

}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void check_error_hot_heater_temp_sensor(void)
{

    if( gu16_AD_Result_Hot_Heater >= TEMPERATURE_SENSOR_OPEN
        || gu16_AD_Result_Hot_Heater <= TEMPERATURE_SENSOR_SHORT )
    {
        gu8_hot_heater_temp_error_clear_timer = 0;
        gu8_hot_heater_temp_error_check_timer++;

        if( gu8_hot_heater_temp_error_check_timer >= TEMPERATURE_SENSOR_ERROR_CHECK_TIME )
        {
            gu8_hot_heater_temp_error_check_timer = TEMPERATURE_SENSOR_ERROR_CHECK_TIME;
            //Bit1_Hot_Heater_Temp_Open_Short_Error = SET;
			Bit22_Hot_Heater_Temp_Open_Short_Error__E48 = SET;
        }
        else{}
    }
    else
    {
        gu8_hot_heater_temp_error_check_timer = 0;
        gu8_hot_heater_temp_error_clear_timer++;

        if ( gu8_hot_heater_temp_error_clear_timer >= TEMPERATURE_SENSOR_ERROR_CHECK_TIME )
        {
            gu8_hot_heater_temp_error_clear_timer = TEMPERATURE_SENSOR_ERROR_CHECK_TIME;
            //Bit1_Hot_Heater_Temp_Open_Short_Error = CLEAR;
			Bit22_Hot_Heater_Temp_Open_Short_Error__E48 = CLEAR;
        }
        else{}

    }

}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void check_error_cold_temp_sensor_open_short(void)
{
    /*..hui [23-3-3오후 3:36:03] 냉수 에러이면 에러해제..*/
    if( F_Cold_Enable != SET )
    {
        Bit14_Cold_Temp_Open_Short_Error__E44 = CLEAR;
        gu8_cold_temp_error_clear_timer = 0;
        gu8_cold_temp_error_check_timer = 0;
        bit_cold_read_finish = SET;
    }
    else
    {
        /*..hui [19-12-19오후 4:46:31] 파워 인가중에만 에러 감지..*/
        /*if( pCOLD_TH_POWER == SET )*/
        if( bit_adc_cold_start == SET )
        {
            if( gu16_AD_Result_Cold >= TEMPERATURE_SENSOR_OPEN
                || gu16_AD_Result_Cold <= TEMPERATURE_SENSOR_SHORT )
            {
                gu8_cold_temp_error_clear_timer = 0;
                gu8_cold_temp_error_check_timer++;

                if( gu8_cold_temp_error_check_timer >= TEMPERATURE_SENSOR_ERROR_CHECK_TIME )
                {
                    gu8_cold_temp_error_check_timer = TEMPERATURE_SENSOR_ERROR_CHECK_TIME;
                    Bit14_Cold_Temp_Open_Short_Error__E44 = SET;
                    bit_cold_read_finish = SET;
                }
                else{}
            }
            else
            {
                gu8_cold_temp_error_check_timer = 0;
                gu8_cold_temp_error_clear_timer++;

                if ( gu8_cold_temp_error_clear_timer >= TEMPERATURE_SENSOR_ERROR_CHECK_TIME )
                {
                    gu8_cold_temp_error_clear_timer = TEMPERATURE_SENSOR_ERROR_CHECK_TIME;
                    Bit14_Cold_Temp_Open_Short_Error__E44 = CLEAR;
                    bit_cold_read_finish = SET;
                }
                else{}

            }
        }
        else
        {
            gu8_cold_temp_error_clear_timer = 0;
            gu8_cold_temp_error_check_timer = 0;
        }

    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void check_error_room_temp_sensor_open_short(void)
{
    /*..hui [19-12-19오후 4:46:31] 파워 인가중에만 에러 감지..*/
    /*if( pROOM_TH_POWER == SET )*/
    if( bit_adc_room_start == SET )
    {
        if( gu16_AD_Result_Room >= TEMPERATURE_SENSOR_OPEN
            || gu16_AD_Result_Room <= TEMPERATURE_SENSOR_SHORT )
        {
            gu8_room_temp_error_clear_timer = 0;
            gu8_room_temp_error_check_timer++;

            if( gu8_room_temp_error_check_timer >= TEMPERATURE_SENSOR_ERROR_CHECK_TIME )
            {
                gu8_room_temp_error_check_timer = TEMPERATURE_SENSOR_ERROR_CHECK_TIME;
                Bit2_Room_Temp_Open_Short_Error__E42 = SET;
            }
            else{}
        }
        else
        {
            gu8_room_temp_error_check_timer = 0;
            gu8_room_temp_error_clear_timer++;

            if ( gu8_room_temp_error_clear_timer >= TEMPERATURE_SENSOR_ERROR_CHECK_TIME )
            {
                gu8_room_temp_error_clear_timer = TEMPERATURE_SENSOR_ERROR_CHECK_TIME;
                Bit2_Room_Temp_Open_Short_Error__E42 = CLEAR;
            }
            else{}
        }
    }
    else
    {
        gu8_room_temp_error_clear_timer = 0;
        gu8_room_temp_error_check_timer = 0;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void check_error_amb_temp_sensor_open_short(void)
{

    if( gu16_AD_Result_Amb >= TEMPERATURE_SENSOR_OPEN
        || gu16_AD_Result_Amb <= TEMPERATURE_SENSOR_SHORT )
    {
        gu8_amb_temp_error_clear_timer = 0;
        gu8_amb_temp_error_check_timer++;

        if( gu8_amb_temp_error_check_timer >= TEMPERATURE_SENSOR_ERROR_CHECK_TIME )
        {
            gu8_amb_temp_error_check_timer = TEMPERATURE_SENSOR_ERROR_CHECK_TIME;
            Bit15_Amb_Temp_Open_Short_Error__E43 = SET;
        }
        else{}
    }
    else
    {
        gu8_amb_temp_error_check_timer = 0;
        gu8_amb_temp_error_clear_timer++;

        if ( gu8_amb_temp_error_clear_timer >= TEMPERATURE_SENSOR_ERROR_CHECK_TIME )
        {
            gu8_amb_temp_error_clear_timer = TEMPERATURE_SENSOR_ERROR_CHECK_TIME;
            Bit15_Amb_Temp_Open_Short_Error__E43 = CLEAR;
        }
        else{}

    }

}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void check_error_hot_in_temp_sensor_open_short(void)
{
    if( gu16_AD_Result_Hot_In >= TEMPERATURE_SENSOR_OPEN
        || gu16_AD_Result_Hot_In <= TEMPERATURE_SENSOR_SHORT )
    {
        gu8_hot_in_temp_error_clear_timer = 0;
        gu8_hot_in_temp_error_check_timer++;

        if( gu8_hot_in_temp_error_check_timer >= TEMPERATURE_SENSOR_ERROR_CHECK_TIME )
        {
            gu8_hot_in_temp_error_check_timer = TEMPERATURE_SENSOR_ERROR_CHECK_TIME;
            Bit4_Hot_In_Temp_Open_Short_Error__E46 = SET;
        }
        else{}
    }
    else
    {
        gu8_hot_in_temp_error_check_timer = 0;
        gu8_hot_in_temp_error_clear_timer++;

        if ( gu8_hot_in_temp_error_clear_timer >= TEMPERATURE_SENSOR_ERROR_CHECK_TIME )
        {
            gu8_hot_in_temp_error_clear_timer = TEMPERATURE_SENSOR_ERROR_CHECK_TIME;
            Bit4_Hot_In_Temp_Open_Short_Error__E46 = CLEAR;
        }
        else{}
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void check_error_amb_side_temp_sensor_open_short(void)
{
    if( gu16_AD_Result_Amb_Side >= TEMPERATURE_SENSOR_OPEN
    || gu16_AD_Result_Amb_Side <= TEMPERATURE_SENSOR_SHORT )
    {
        gu8_amb_side_temp_error_clear_timer = 0;
        gu8_amb_side_temp_error_check_timer++;

        if( gu8_amb_side_temp_error_check_timer >= TEMPERATURE_SENSOR_ERROR_CHECK_TIME )
        {
            gu8_amb_side_temp_error_check_timer = TEMPERATURE_SENSOR_ERROR_CHECK_TIME;
            Bit21_Amb_Side_Temp_Open_Short_Error__E53 = SET;
        }
        else{}
    }
    else
    {
        gu8_amb_side_temp_error_check_timer = 0;
        gu8_amb_side_temp_error_clear_timer++;

        if ( gu8_amb_side_temp_error_clear_timer >= TEMPERATURE_SENSOR_ERROR_CHECK_TIME )
        {
            gu8_amb_side_temp_error_clear_timer = TEMPERATURE_SENSOR_ERROR_CHECK_TIME;
            Bit21_Amb_Side_Temp_Open_Short_Error__E53 = CLEAR;
        }
        else{}

    }

}



/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/










