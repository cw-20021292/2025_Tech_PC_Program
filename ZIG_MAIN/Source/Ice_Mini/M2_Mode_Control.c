/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Mode_Control.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#include    "Macrodriver.h"
#include    "Global_Variable.h"
#include    "Port_Define.h"
#include    "M2_Mode_Control.h"

void Mode_Control(void);
void cold_temp_level_decision(void);
void hot_temp_level_decision(void);

void System(void);
void cody_test_mode(void);
void stop_ice_cody_mode(void);
void cold_level_setting_hi(void);
void get_final_small_amb_temp(void);
U8 get_final_large_amb_temp(void);


bit F_6HourNoUse;                 // �̻�� ����
//----------------------------------------------------// �ü�,�¼�
U8 gu8ColdTemp;
U8 gu8HotTemp;
//----------------------------------------------------// Heater
U8 gu8HotH2_On;
U8 gu8HotH3_On;
U8 gu8HotH2_Off;
U8 gu8HotH3_Off;





//----------------------------------------------------// Test
U8 gu8TestGo;
U8 gu8TestTemp;
U8 gu8VersionCount;
U8 gu8VersionTime;
//U16 gu16TestTime;

bit F_Reset;
U16 ucErrOvice_Valve;
U16 ucErrOVice_Heater;
U16 ucTime_10min_cycle;
U16 ucErrOvice_Time;
// 20130315 NFC TEST MODE���� ������ ���� 5�� �� 8�� ����




//U16 gu16MeltTime;

bit F_TrayCut;
bit F_NoSelectBar;
bit F_Melt;
bit F_Safety_Routine;
bit F_TrayStop;                       // ������ Ʈ�����̵� ����
bit F_Tray_up_moving_retry_state;
bit F_Trayretry1;
bit F_Trayretryfinal;



TYPE_BYTE          U8WaterOutStateB;
#define            u8WaterOutState                           U8WaterOutStateB.byte
#define            Bit0_Pure_Water_Select_State              U8WaterOutStateB.Bit.b0
#define            Bit1_Cold_Water_Select_State              U8WaterOutStateB.Bit.b1
#define            Bit2_Hot_Water_Select_State               U8WaterOutStateB.Bit.b2

TYPE_BYTE          U8IceOutStateB;
#define            u8IceOutState                             U8IceOutStateB.byte
#define            Bit0_Ice_Only_Select_State                U8IceOutStateB.Bit.b0
#define            Bit1_Ice_Plus_Water_Select_State          U8IceOutStateB.Bit.b1


//----------------------------------------------------// Altitude
U8 gu8AltitudeTime;
U8 gu8AltitudeStep;



U16 gu16Water_Extract_Timer;


bit bit_child_lock_enable;

U8 gu8_cooling_display_mode;
U8 gu8_heating_display_mode;

U16 gu16_display_cold_on_temp;
U16 gu16_display_cold_off_temp;
bit bit_cooling_complete_5degree;

extern void hot_ster_control(void);
extern void time_setting();
extern void wifi_time_setting();
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void Mode_Control(void)
{
    /* 2025-10-28 CH.PARK ���彺��ġ �߰����� Ȯ�� ���Ǵ� */
    model_select();


    cold_temp_level_decision();

    System();

    continued_extract_control();

    water_extract_control();

    /*..hui [25-1-10���� 1:46:00] ���� ���� ����..*/
    ice_extract_control();

    my_cup_return_decision();

    logic_decision();

    /*..hui [18-1-14���� 5:50:58] ��ȯ��� ��� �߰�..*/
    auto_drain_control();

    /*..hui [23-8-14���� 2:07:11] ������� ��� �߰�..*/
    manual_drain();

    /*..hui [18-1-23���� 2:33:06] 24�ð� ���� ���̽����� CLOSE..*/
    ice_door_close_24_hour();

    // 2025-08-27 CH.PARK [V1.0.0.5] �������� �� 20�� ���Ŀ� ���̽����� �ݴ� ��� �߰�
    ice_door_close_20_min();

    ice_select_door_close_24_hour();

    /*..hui [20-1-8���� 4:22:39] �ڵ� �׽�Ʈ ���..*/
    cody_test_mode();

    /*..hui [23-2-28���� 7:36:37] ���̽���ũ �¼� ���� ��� ��� �߰�..*/
    ice_tray_ster_control();

    calc_water_usage();

    /*..hui [23-6-12���� 4:39:12] �ð� ����..*/
    time_setting();

    /*..hui [21-7-27���� 12:43:14] WIFI ���� ����..*/
    wifi_operation_control();
    wifi_smart_control();
    wifi_time_setting();

    save_mode();

    child_lock();

    /*..hui [23-9-7���� 1:52:27] ǰ�� ����¡ �׽�Ʈ..*/
    water_durable_test();

	check_ice_system_ok();
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void cold_temp_level_decision(void)
{
    /*..hui [19-12-18���� 10:50:28] �ü����� ������ 3������ ����..*/
    if( Bit14_Cold_Temp_Open_Short_Error__E44 == SET )
    {
        /*gu8_cooling_display_mode = COOLING_DISPLAY_1_ON;*/
        gu8_cooling_display_mode = COOLING_DISPLAY_0_OFF;
        return;
    }
    else{}

    /*..hui [23-12-22���� 4:39:02] �ü� ������ �ʴ� ���� ���� ���� ����.. �ν�..*/
    if( Bit14_Cold_Temp_Open_Short_Error__E44 == SET
        || Bit3_Leakage_Sensor_Error__E01 == SET
        || Bit7_BLDC_Communication_Error__E27 == SET
        || bit_bldc_operation_error_total == SET )
    {
        gu8_cooling_display_mode = COOLING_DISPLAY_0_OFF;
        return;
    }
    else{}

    /*..hui [20-1-6���� 9:08:55] �ü� ���� ���� �� 1��..*/
    if( F_Cold_Enable == CLEAR )
    {
        /*gu8_cooling_display_mode = COOLING_DISPLAY_1_ON;*/
        gu8_cooling_display_mode = COOLING_DISPLAY_0_OFF;
        return;
    }
    else{}

    cold_level_setting_hi();
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void cold_level_setting_hi(void)
{
    switch( gu8_cooling_display_mode )
    {
        case COOLING_DISPLAY_0_OFF :
            bit_cooling_complete_5degree = CLEAR;

            /*if( gu8_Cold_Temperature_One_Degree <= 8 )*/
            if( gu16_Cold_Temperature <= gu16_display_cold_off_temp )
            {
                gu8_cooling_display_mode = COOLING_DISPLAY_2_COMPLETE;
            }
            /*else if( gu8_Cold_Temperature_One_Degree > 10 )*/
            else if( gu16_Cold_Temperature >= gu16_display_cold_on_temp )
            {
                gu8_cooling_display_mode = COOLING_DISPLAY_1_OPERATION;
            }
            else
            {
                if( Bit0_Cold_Mode_On_State == SET )
                {
                    gu8_cooling_display_mode = COOLING_DISPLAY_1_OPERATION;
                }
                else
                {
                    gu8_cooling_display_mode = COOLING_DISPLAY_2_COMPLETE;
                }
            }

            break;

        case COOLING_DISPLAY_1_OPERATION :

            /*..hui [23-11-21���� 10:14:17] �ü��µ� 8�� ����..*/
            /*if( gu8_Cold_Temperature_One_Degree <= 8 )*/
            if( gu16_Cold_Temperature <= gu16_display_cold_off_temp )
            {
                gu8_cooling_display_mode = COOLING_DISPLAY_2_COMPLETE;
				bit_cooling_complete_5degree = SET;
            }
            else{}

            break;

        case COOLING_DISPLAY_2_COMPLETE :

            /*if( gu8_Cold_Temperature_One_Degree > 10 )*/
            if( gu16_Cold_Temperature >= gu16_display_cold_on_temp )
            {
                gu8_cooling_display_mode = COOLING_DISPLAY_1_OPERATION;
				bit_cooling_complete_5degree = CLEAR;
            }
            else{}

            break;

        default :

            gu8_cooling_display_mode = COOLING_DISPLAY_0_OFF;

            break;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void hot_temp_level_decision(void)
{
}



/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
//""SUBR COMMENT""************************************************************
// ID         : System
// ����       :
//----------------------------------------------------------------------------
// ���       :
//----------------------------------------------------------------------------
//""SUBR COMMENT END""********************************************************

void System(void)
{
    if(gu16IRInterval > 0)
    {
        gu16IRInterval--;            // �����˻� �ֱ� 15��
    }
    else{}

    if(gu16IRInterval == 0)
    {
        F_IR = SET;
    }
    else{}

    if(gu16IR_l_Interval > 0)
    {
        gu16IR_l_Interval--;            // �����˻� �ֱ� 15��
    }
    else{}

    if(gu16IR_l_Interval == 0)
    {
        F_Low_IR = SET;
    }
    else{}

    //===================================================// ��ħ���� 6�ð� ��������
    //if(F_Sleep == SET && F_IceFull == SET && F_IceStop != SET)
    //{
    //    F_IceStop = SET;
    //    gu16IceStopTime = ICESTOP_TIME_SIX_HOURS;
    //}
    //else if(F_Sleep != SET || gu16IceStopTime == 0)
    //{
    //    F_IceStop = CLEAR;
    //}
    //else{}


    /*..hui [18-3-6���� 5:17:36] �׽�Ʈ��� �ڵ� ����..*/
    //if(gu16TestTime == 0 && F_LineTest == SET)
    //{
    //    F_LineTest = CLEAR;
    //    system_reset();
    //}
    //else{}

}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void cody_test_mode(void)
{
    cody_ice_tray_test();
    cody_service();
    cody_takeoff_ice();

    /* Cody Water Line Clean Service */
    cody_water_line_clean();
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void stop_ice_cody_mode(void)
{
    if( F_IceInit == SET )
    {
        /*..hui [20-2-19���� 3:26:02] ����Ż�� ��� �� �Ǵ� ���� �� ���� Ż���ϸ� ����Ż�� ���..*/
        /*..hui [20-2-19���� 3:26:14] ���νİ� ���� ���̽��Ҵ� ����..*/
        F_IceInit = CLEAR;
        gu8InitStep = 0;
        gu16IceMakeTime = 0;
        gu16IceHeaterTime = 0;
    }
    else
    {
        if( gu8IceStep != STATE_0_STANDBY )
        {
            /*if( gu8IceStep >= STATE_10_ICE_TRAY_MOVE_UP
                && gu8IceStep <= STATE_30_CALC_ICE_MAKING_TIME )*/
            if( gu8IceStep <= STATE_30_CALC_ICE_MAKING_TIME )
            {
                if( F_Comp_Output == CLEAR )
                {
                    /*..hui [20-1-29���� 3:48:29] ���� ���ܰ��̸� �ٷ� ����üũ �� ����..*/
                    /*..hui [20-2-19���� 7:46:55] ���� - ���� �ȵ����Ƿ� ���� üũ ���� ��� ����..*/
                    gu8IceStep = STATE_51_FINISH_ICE_MAKE;
                    /*..hui [20-1-29���� 3:53:01] Ʈ���̵� �ö󰡴� ���̾����� ������..*/
                    down_tray_motor();
                }
                else
                {
                    /*..hui [23-7-21���� 5:43:03] ��������.. ������ �������̸� �ְ��� Ż������..*/
                    gu8IceStep = STATE_40_ICE_TRAY_MOVE_DOWN;
                    /*..hui [20-1-29���� 3:53:01] Ʈ���̵� �ö󰡴� ���̾����� ������..*/
                    down_tray_motor();
                }
            }
            else if( gu8IceStep == STATE_31_MAIN_ICE_MAKING )
            {
                /*..hui [23-7-21���� 5:35:02] ���� �ð��� ������� ������ �ְ��� Ż�� ����.. ..*/
                /*..hui [23-7-21���� 5:35:20] �õ�.. ���¿��� �ְ��� Ż���ص� ������ ��������..*/
                gu16IceMakeTime = 0;
            }
            else if( gu8IceStep >= STATE_40_ICE_TRAY_MOVE_DOWN
                     && gu8IceStep <= STATE_43_ICE_TAKE_OFF )
            {
                /*..hui [20-1-29���� 3:47:24] Ż�� �̵����̰ų� Ż�����ϰ�� �ϴ��� ��� ����..*/

            }
            else{}
        }
        else{}
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void get_final_small_amb_temp(void)
{
    /*..hui [23-9-20���� 9:21:55] �� �� �����ϰ�� 25����..*/
    if( Bit15_Amb_Temp_Open_Short_Error__E43 == SET
        && Bit21_Amb_Side_Temp_Open_Short_Error__E53 == SET )
    {
        gu8_Amb_Temperature_One_Degree = 25;
        return;
    }
    else{}

    /*..hui [23-9-19���� 1:18:26] �ܱ⼾��2 ����� 1������..*/
    if( Bit21_Amb_Side_Temp_Open_Short_Error__E53 == SET )
    {
        gu8_Amb_Temperature_One_Degree = gu8_Amb_Front_Temperature_One_Degree;
        return;
    }
    else{}

    /*..hui [23-9-19���� 1:18:34] �ܱ⼾��1 ����� 2������..*/
    if( Bit15_Amb_Temp_Open_Short_Error__E43 == SET )
    {
        gu8_Amb_Temperature_One_Degree = gu8_Amb_Side_Temperature_One_Degree;
        return;
    }
    else{}

    /*..hui [23-9-19���� 11:19:28] û��. �ΰ� �� ���� �� ����, �̻�� ������ ���� �� ����..*/
    if( gu8_Amb_Side_Temperature_One_Degree > gu8_Amb_Front_Temperature_One_Degree )
    {
        gu8_Amb_Temperature_One_Degree = gu8_Amb_Front_Temperature_One_Degree;
    }
    else
    {
        gu8_Amb_Temperature_One_Degree = gu8_Amb_Side_Temperature_One_Degree;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 get_final_large_amb_temp(void)
{
    U8 mu8_return = 0;

    /*..hui [23-9-19���� 1:17:53] �Ѵ� �����ϰ�� �̻������ �����ϵ���.. �켱....*/
    if( Bit15_Amb_Temp_Open_Short_Error__E43 == SET
        && Bit21_Amb_Side_Temp_Open_Short_Error__E53 == SET )
    {
        mu8_return = 25;
        return mu8_return;
    }
    else{}

    /*..hui [23-9-19���� 1:18:26] �ܱ⼾��2 ����� 1������..*/
    if( Bit21_Amb_Side_Temp_Open_Short_Error__E53 == SET )
    {
        mu8_return = gu8_Amb_Front_Temperature_One_Degree;
        return mu8_return;
    }
    else{}

    /*..hui [23-9-19���� 1:18:34] �ܱ⼾��1 ����� 2������..*/
    if( Bit15_Amb_Temp_Open_Short_Error__E43 == SET )
    {
        mu8_return = gu8_Amb_Side_Temperature_One_Degree;
        return mu8_return;
    }
    else{}

    /*..hui [23-9-19���� 11:19:28] û��. �ΰ� �� ���� �� ����, �̻�� ������ ���� �� ����..*/
    /*..hui [23-9-19���� 1:18:45] �̻�� ���� ���ǿ����� ū ������..*/
    if( gu8_Amb_Side_Temperature_One_Degree > gu8_Amb_Front_Temperature_One_Degree )
    {
        mu8_return = gu8_Amb_Side_Temperature_One_Degree;
    }
    else
    {
        mu8_return = gu8_Amb_Front_Temperature_One_Degree;
    }

    return mu8_return;
}






/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/



