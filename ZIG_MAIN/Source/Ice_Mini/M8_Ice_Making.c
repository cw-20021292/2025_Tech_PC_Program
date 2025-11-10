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
#include    "M8_Ice_making.h"
#include    "Temp_Table.h"
#include    "Ice_Make_Time_Table.h"


void Ice_Make_Process(void);
void normal_mode_ice_init_operation(void);
U16 calc_ice_make_time(U8 mu8AmbTemp, U8 mu8RoomTemp);
//U16 calc_ice_heater_time(void);
U8 Ice_Tray_Sensing(void);
void ice_make_operation(void);
void ice_init_select_bar_test(void);
U8 hot_gas_operation(void);
void recovery_ice_tray(void);
void reduce_hot_gas_noise(void);

U8 get_ice_mode_comp_rps(void);
U8 get_hotgas_mode_comp_rps(void);
U8 get_preheat_mode_comp_rps(void);
U16 get_hotgas_time(void);
U16 get_preheat_time(void);
void get_average_tray_temp(void);


bit F_IceHeater;
bit F_Hotgas_Time;
U8 gu8InitStep;


ICE_STEP gu8IceStep;

bit F_IceVV;

//----------------------------------------------------// ICE Heater
U16 gu16IceHeaterTime;                                // Ż�� Heater ���� �ð�
bit F_ColdWaterInit;
bit F_WaterInit;                      // ���� �����Ϸ�
/*bit F_HotWaterInit;*/                   // ���� �����Ϸ� - �¼�
bit F_WaterInitSet;
bit F_ColdVV;

bit F_TrayMotorUP;
bit F_TrayMotorPreUP;
bit F_TrayMotorDOWN;
bit F_TrayMotorPreDOWN;

//bit F_ColdIn;
//bit F_ColdDIR;               // �ü��� ��������
bit F_IceOpen;
bit F_IceTray;
U8 gu8IceOut;
bit F_IceOutCCW;
U8 gu8IceOutCCWInterval;
U16 gu16IceOutTest;
//U16 gu16StepMotor1;
U8 gu8IceClose;
U8 gu8IceInnerClose;
U8 gu8SBTest;                // SB Error ����
bit F_NoColdBar;
bit F_NoIceBar;
//U16 gu16CompOffDelay;
U16 gu16IceMakeTime;
U8 gu8_IceHeaterControl;
U16 gu16IceVVTimeSetDelay;
U16 gu16ErrPuriDelay;



U16 gu16IceVVTime;
//U8 gu8IceCount;
U8 gu8ICETrayRoomDelay;
//U8 Tray_Comp_Delay;

/*..hui [18-3-8���� 11:23:33] ���� �� 50�� ���� ���� �� �ü� Ż�� ����..*/
bit F_Ext_cold;

U8 gu8AmbTemp;
U8 gu8RoomTemp;

bit F_CompOn;                     // �ü� ���� ����
bit F_IceCompOn;                  // ���� ���� ����
bit F_CompHeater;                 // ����,���� ���ñ⵿����


U8 gu8_ice_make_1sec_timer;
bit F_ice_make_one_more_time;

U8 gu8_ice_tray_reovery_time;

U16 gu16_Ice_Tray_Fill_Hz;
U16 gu16_cody_ice_make_time;


U16 gu16_preheat_time;


U16 gu16_averge_count;
U32 gu32_averge_temp_sum;
U8 gu8_averge_temp_min;
U8 gu8_average_temp_max;

U8 gu8_average_tray_temp;
U8 gu8_average_timer;

U8 gu8_ice_mix_timer;

U8 gu8_ice_take_off_tray_delay = CLEAR;
U8 gu8_ice_take_off_count = CLEAR;
U8 gu8_ice_take_off_stop_time = CLEAR;
U8 gu8_ice_take_off_op_time = CLEAR;

/*******************************************************/
// 2025-11-10 @CH.PARK ?�빙?�이�??�스?�용 �???추�? 

/*******************************************************/
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void Ice_Make_Process(void)
{
    /*..hui [17-12-26���� 9:51:20] ���� ǥ�� �Ϸ� �� ����..*/
    if(F_FW_Version_Display_Mode != SET)
    {
        return;
    }
    else{}

    /*..hui [17-12-19���� 7:07:33] �����϶� ���� �� ���� ���� �߰�....*/
    /*..hui [17-12-19���� 7:07:54] ���߿� �ü� �Լ����?OFF��Ű�°͵� �߰��ؾ���....*/
    /* ���λ��?���� �� ���� ���� �� ���� �Ұ� 250721 CH.PARK */
    if( Bit2_Ice_Operation_Disable_State == SET
    || F_Safety_Routine == SET
    || F_ErrTrayMotor_DualInital == SET
    || cody_water_line.gu8_start == SET)
    {
        F_IceHeater = CLEAR;
        gu8InitStep = 0;
        gu8IceStep = STATE_0_STANDBY;
        return;
    }
    else{}

    //=======================================================//�������� �ʱ�ȭ
    if(F_IceInit == SET)
    {
        ice_init_operation();
        return;
    }
    else
    {
        gu8InitStep = 0;
    }

    //======================================================// ���������?    if(Bit1_Ice_Make_Go != SET)
    if(Bit1_Ice_Make_Go != SET)
    {
        gu8IceStep = STATE_0_STANDBY;
        F_IceHeater = 0;
        return;
    }
    else{}

    //=======================================================// ���� ������
    ice_make_operation();

    /*..hui [23-9-19���� 3:37:57] Ʈ���� �Լ��µ� ��հ�?Ȯ��..*/
    /*get_average_tray_temp();*/

}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void ice_make_operation(void)
{
    U8 mu8_return_value = 0;
    U8 mu8_comp_rps = 0;

    //=======================================================// ���� ������
    switch(gu8IceStep)
    {
        //----------------------------------------------// �������� ����ȸ��
        case STATE_0_STANDBY :
            if( (gu16CompOffDelay == 0)
            || (F_Comp_Output == SET)
            )
            {
                /*..hui [19-10-23���� 12:47:20] ���� �����߿��� Ʈ���� �ø��� �ʵ���..*/
                /*..hui [19-10-23���� 12:47:34] �����߿� ������ �з��� Ʈ���̰� �ɸ���..*/
                /*..hui [19-10-23���� 12:47:46] �ⱸ������ ������ �ʟG��..*/
                if( F_IceOut == CLEAR && bit_tray_in_error_temporary == CLEAR )
                {
                    /*if( bit_start_preheat == SET )*/

                    /* �ܱ� �µ� 25�� �̸��� ���¿��� �����?���� �ð��� 30�� �ʰ��� ���?*/
                    if( bit_start_preheat == SET && gu8_Amb_Temperature_One_Degree < PREHEAT_AMB_TEMP )
                    {
                        gu8IceStep = STATE_5_PREHEAT_HOTGAS_MOVE;
                    }
                    else
                    {
                        gu8IceStep = STATE_10_ICE_TRAY_MOVE_UP;
                    }
                }
                else{}
            }
            else{}

            break;

        case STATE_5_PREHEAT_HOTGAS_MOVE :

            /*..hui [23-4-7���� 5:10:11] �ø���ȯ���?�ְ��� �̵�..*/
            GasSwitchHotGas();
            gu8IceStep = STATE_6_CALC_PREHEAT_TIME;

            break;

        case STATE_6_CALC_PREHEAT_TIME :

            if( gu8_GasSwitch_Status == GAS_SWITCH_HOTGAS )
            {
                /*..hui [23-4-7���� 5:10:03] �ø���ȯ���?�ְ��� �̵� �Ϸ� ��, COMP ����, 60HZ..*/
                mu8_comp_rps = get_preheat_mode_comp_rps();
                set_comp_rps( mu8_comp_rps );

                gu16_preheat_time = get_preheat_time();

                gu8IceStep = STATE_7_PREHEAT_OPERATION;
            }
            else
            {
                GasSwitchHotGas();
            }

            break;

        case STATE_7_PREHEAT_OPERATION :

            if(gu16_preheat_time > 0 && pCOMP == SET)
            {
                gu16_preheat_time--;
            }
            else{}

            if(gu16_preheat_time == 0)
            {
                GasSwitchIce();
                gu8IceStep = STATE_10_ICE_TRAY_MOVE_UP;
            }
            else{}

            break;

        //----------------------------------------------// �������� ����ȸ��
        case STATE_10_ICE_TRAY_MOVE_UP :
            if(F_TrayMotorDOWN != SET)
            {
                up_tray_motor();    // �������� �� ���?                gu8IceStep = STATE_11_WAIT_ROOM_WATER_FULL;

                F_CristalIce = SET;
            }
            else{}

            break;

        //----------------------------------------------// ����ȸ���� ������VV ON
        case STATE_11_WAIT_ROOM_WATER_FULL :

            //if( /*F_TrayMotorUP != SET && */gu8IceTrayLEV == ICE_TRAY_POSITION_ICE_MAKING )
            //if( (sStepMotor.state == STEP_STATE_DEACTIVE) && ( gu8IceTrayLEV == ICE_TRAY_POSITION_ICE_MAKING ) )
            /*if ( gu8IceTrayLEV == ICE_TRAY_POSITION_ICE_MAKING )*/
            if( F_TrayMotorUP != SET && gu8IceTrayLEV == ICE_TRAY_POSITION_ICE_MAKING )
            {
                F_TrayMotorUP = CLEAR;

                gu8_E62_dummy_iceheat_flag = CLEAR;
                tray_error_flag_E62 = CLEAR;
                tray_abnormal_E62_timer = 0;
                tray_abnormal_E62_step = 0;
                abnormal_2_repeat_cnt = 0;

                /*..hui [23-4-7���� 5:52:57] ���� ���� �����ϰ��?.*/
                /*..hui [23-8-14���� 1:56:47] ���� Ȯ���� Ʈ���� �Լ��ʿ���.. ..*/
                /*..hui [23-8-14���� 1:57:10] �������?���� ���̿� �� �� �������� COMP �������·� ���ʿ��� ����ϰԵ�?.*/
                if( F_Comp_Output == SET )
                {
                    gu8IceStep = STATE_12_CONT_ICE_SWITCH_MOVE;
                }
                else
                {
                    gu8IceStep = STATE_14_CHECK_ICE_TRAY_HZ;
                }
            }
            else
            {
                /*..hui [18-3-20���� 7:21:54] ������ũ ������ ä����߿�?Ʈ���� ������ ���� ������..*/
                if(F_TrayMotorUP != SET)
                {
                    gu8IceStep = STATE_10_ICE_TRAY_MOVE_UP;
                }
                else{}

            }

            break;

        case STATE_12_CONT_ICE_SWITCH_MOVE :

            GasSwitchIce();
            gu8IceStep = STATE_13_CONT_RPS_SETTING;

            break;

        case STATE_13_CONT_RPS_SETTING :

            /*..hui [19-7-24���� 4:35:37] �ø���ȯ���?�̵� �Ϸ� ��..*/
            if(gu8_GasSwitch_Status == GAS_SWITCH_ICE)
            {
                mu8_comp_rps = get_ice_mode_comp_rps();
                set_comp_rps( mu8_comp_rps );

                gu8IceStep = STATE_14_CHECK_ICE_TRAY_HZ;
            }
            else
            {
                GasSwitchIce();
            }

            break;

        //----------------------------------------------// ����ȸ���� ������VV ON
        case STATE_14_CHECK_ICE_TRAY_HZ :

            /*..hui [19-7-23���� 2:08:21] Ʈ�����Լ� �����?���濡 ���� ���?����(�������� �Ĵ�)..*/
            /*..hui [19-7-23���� 2:08:26] �¼� ����߿���?Ʈ���� �Լ� ����..*/
            /*if(u8WaterOutState == HOT_WATER_SELECT && F_WaterOut == SET)*/
            /*..hui [19-8-28���� 9:54:48] �¼� �Ӹ��ƴ϶� �� �����߿��� Ʈ���� �Լ� ���?.*/
            #if 1
            if(F_WaterOut == SET)
            {
                /* Ʈ���� �Լ� �Ϸ��µ� �ÿ��� �������?�����ϸ� ����ؾ�?�� */
            }
            else
            {
                gu16_Ice_Tray_Fill_Hz = C_ICE_TRAY_FILL_200CC;
                gu8IceStep = STATE_20_WATER_IN_ICE_TRAY;
            }
            #endif

            break;

        //-----------------------------------------------// ������VV Off
        case STATE_20_WATER_IN_ICE_TRAY :

            if( gu16_Ice_Tray_Fill_Hz <= 0 )
            {
                gu8IceStep = STATE_21_ICE_SWITCH_MOVE;

                gu16_wifi_tray_in_flow = 260;
                /*..hui [24-11-13���� 2:57:55] TDS ����..*/
                /*gu16_tds_water_acc = gu16_tds_water_acc + 200;*/
            }
            else
            {
                gu16_wifi_tray_in_time++;
                /* Ʈ���� �Լ����ε� Ʈ���� �Լ��� ������ �����?                ���?Ʈ���� ������ Ż�� �� ���߿� �ٽ� �õ� 250423 CH.PARK */
                if( bit_tray_in_error_temporary == SET )
                {
                    down_tray_motor();      // Ʈ���� �Լ��� ����
                    if( F_Comp_Output == SET )
                    {
                        gu8IceStep = STATE_40_ICE_TRAY_MOVE_DOWN;
                    }
                    else
                    {
                        gu8IceStep = STATE_51_FINISH_ICE_MAKE;
                    }
                }
                else {  }
            }

            break;

        //-----------------------------------------------// ������VV Off
        case STATE_21_ICE_SWITCH_MOVE :

            GasSwitchIce();

            gu8IceStep = STATE_30_CALC_ICE_MAKING_TIME;

            break;

        //-----------------------------------------------// ���� ����
        case STATE_30_CALC_ICE_MAKING_TIME :

            /*..hui [19-7-24���� 4:35:37] �ø���ȯ���?�̵� �Ϸ� ��..*/
            if(gu8_GasSwitch_Status == GAS_SWITCH_ICE)
            {
                if(gu16CompOffDelay == 0 ||  F_Comp_Output == SET)
                {
                    /* ���������� [��/��]�� �õ����� ���̺� �ָ� �׿� �°� �����ϸ� �� 250224 CH.PARK */
                    gu16IceMakeTime
                        = (U16)calc_ice_make_time( gu8_Amb_Front_Temperature_One_Degree, gu8_Room_Temperature_One_Degree);

                    /*gu16IceMakeTime
                        = (U16)calc_ice_make_time( gu8_Amb_Temperature_One_Degree, gu8_average_tray_temp);*/

                    /*..hui [19-7-5���� 2:08:13] 100ms ī��Ʈ ���� ����..*/
                    gu16IceMakeTime = (U16)(gu16IceMakeTime  * 10);

                    /*..hui [25-3-27���� 1:34:22] ���� ���?���?�߰�..*/
                    if( bit_ice_size == ICE_SIZE_SMALL )
                    {
                        gu16IceMakeTime = (U16)((F32)gu16IceMakeTime * 0.9f);   // �õ���û 80% -> 90%
                    }
                    else{}

                    gu16_cody_ice_make_time  = gu16IceMakeTime;

                    /*..hui [24-2-14���� 4:28:53] UV �����ð� ����..*/
                    gu16_uv_ice_make_time = gu16IceMakeTime;

                    mu8_comp_rps = get_ice_mode_comp_rps();
                    set_comp_rps( mu8_comp_rps );

                    gu8IceStep = STATE_31_MAIN_ICE_MAKING;
                    gu8_ice_tray_reovery_time = 0;
                }
                else{}
            }
            else
            {
                GasSwitchIce();
            }

            break;
        //-----------------------------------------------// �����Ϸ��� ����ȸ��
        case STATE_31_MAIN_ICE_MAKING :

            if(gu16IceMakeTime > 0 && pCOMP == SET)
            {
                gu16IceMakeTime--;    // ���� 13��

                gu32_wifi_ice_make_time++;
            }
            else{}

            if(gu16IceMakeTime == 0)
            {
                /*gu16IceHeaterTime = calc_ice_heater_time();*/
                down_tray_motor();      // �����Ϸ� �� Ż��

                gu8IceStep = STATE_40_ICE_TRAY_MOVE_DOWN;
                gu8_ice_take_off_tray_delay = CLEAR;
                gu8_ice_take_off_count = CLEAR;
                F_CristalIce = CLEAR;
            }
            else
            {
                /* �������ε� Ʈ���̰� �Ʒ��� ���� �� ���� */
                recovery_ice_tray();

                if( gu16IceMakeTime <= HOT_GAS_NOISE_REDUCE_TIME )
                {
                    reduce_hot_gas_noise();
                }
                else{}
            }

            break;

        //-----------------------------------------------// ����ȸ���Ϸ�
        case STATE_40_ICE_TRAY_MOVE_DOWN :
			if(F_TrayMotorDOWN != SET && gu8IceTrayLEV == ICE_TRAY_POSITION_ICE_THROW)
            {
				F_TrayMotorDOWN = 0;
                gu8IceStep = STATE_41_GAS_SWITCH_HOT_GAS;
            }
            else {}
            break;

        case STATE_41_GAS_SWITCH_HOT_GAS :

            mu8_comp_rps = get_hotgas_mode_comp_rps();
            set_comp_rps( mu8_comp_rps );

            GasSwitchHotGas();

            gu8IceStep = STATE_42_CALC_HOT_GAS_TIME;
            break;

        case STATE_42_CALC_HOT_GAS_TIME :

            if(gu8_GasSwitch_Status == GAS_SWITCH_HOTGAS)
            {
                gu16IceHeaterTime = get_hotgas_time();
                gu8IceStep = STATE_43_ICE_TAKE_OFF;
            }
            else
            {
                GasSwitchHotGas();
            }

            break;


            case STATE_43_ICE_TAKE_OFF :
            gu32_wifi_ice_heater_timer++;

            mu8_return_value = hot_gas_operation();

            if(mu8_return_value == SET)
            {
                F_IR = SET;              // �������� ����
                F_Low_IR = SET;
                F_IceHeater = CLEAR;
                gu8_ice_mix_timer = 0;
                gu8IceStep = STATE_44_FEEDER_OPERATION;
                bit_ice_mix_back_state = SET;       // �����Ϸ� �� 2�� ����
            }
            else{}

            break;

            case STATE_44_FEEDER_OPERATION:

            gu8_ice_mix_timer++;
            if( gu8_ice_mix_timer >= ICE_FEDDER_MIX_MAX_TIME )
            {
                gu8_ice_mix_timer = 0;
                F_IR = SET;              // ������u�������� �좯AU
                F_Low_IR = SET;
                gu8IceStep = STATE_50_ICE_FULL_IR_CHECK;
            }
            else{}

            break;
        //-------------------------------------------------// SB �ü� ȸ���Ϸ�
        case STATE_50_ICE_FULL_IR_CHECK :

            if(F_IR != SET)
            {
                gu8IceStep = STATE_51_FINISH_ICE_MAKE;
            }
            else{}

            break;

        case STATE_51_FINISH_ICE_MAKE :
            // gu8IceStep = STATE_52_TRAY_UP_POSITION;
            gu8IceStep = STATE_0_STANDBY;
            /*..hui [23-7-21���� 4:52:04] �߰�..*/
            F_CristalIce = CLEAR;
            break;

        default :

            gu8IceStep = STATE_0_STANDBY;
            gu8InitStep = 0;
            F_IceHeater = CLEAR;
            F_IceInit = SET;

            break;
      }

}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U16 calc_ice_make_time(U8 mu8AmbTemp, U8 mu8RoomTemp)
{
    U8 mu8_amb_temp = 0;
    U8 mu8_room_temp = 0;

    if(mu8AmbTemp >= 45)
    {
        mu8_amb_temp = 45;
    }
    else
    {
        mu8_amb_temp = mu8AmbTemp;
    }

    if(mu8RoomTemp >= 45)
    {
        mu8_room_temp = 45;
    }
    else
    {
        mu8_room_temp = mu8RoomTemp;
    }

    return Temp_MakeTime[ mu8_room_temp ][ mu8_amb_temp ];
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 hot_gas_operation(void)
{
    U8 mu8_return_value = 0;

    if(gu16IceHeaterTime > 0)
    {
        gu16IceHeaterTime--;

        gu32_wifi_hot_gas_time++;
    }
    else{}

    if(gu16IceHeaterTime == 0)
    {
        F_IceHeater = CLEAR;
        mu8_return_value = SET;
    }
    else{}

    return mu8_return_value;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void recovery_ice_tray(void)
{
    /*..hui [18-3-22���� 9:39:58] �ⱸ���� ������ ������ �ƿ���Ʈ���̰� ������ ���� �߻��Ҽ�����..*/
    /*..hui [18-3-22���� 9:40:16] ���̽�Ʈ���̰� ������ 10�ʿ� �ѹ��� �ٽ� �÷��ش�..*/
    if(gu8IceTrayLEV != ICE_TRAY_POSITION_ICE_MAKING)
    {
        /*..hui [18-3-22���� 10:00:09] Ʈ���� ��õ���� �߿��� ���۾���..*/
        if(F_Safety_Routine != SET)
        {
            gu8_ice_tray_reovery_time++;
        }
        else
        {
            gu8_ice_tray_reovery_time = 0;
        }

        if(gu8_ice_tray_reovery_time >= 200)
        {
            gu8_ice_tray_reovery_time = 0;
            up_tray_motor();    // ������ Ʈ���� ó�� �� ���?        }
        }
        else{}
    }
    else
    {
        gu8_ice_tray_reovery_time = 0;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void reduce_hot_gas_noise(void)
{
    U8 mu8_comp_rps = 0;

    mu8_comp_rps = get_hotgas_mode_comp_rps();
    set_comp_rps( mu8_comp_rps );
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 get_ice_mode_comp_rps(void)
{
    U8 mu8_return = 0;

	/*.. sean [25-02-04] gu8_Amb_Temperature_One_Degree���� ó���Ǿ� front�� ����..*/
    if( gu8_Amb_Front_Temperature_One_Degree <= 10 )
    {
        /*..hui [23-4-7���� 11:15:58] 10�� ����..*/
        mu8_return = BLDC_COMP_65Hz;
    }
    else if( gu8_Amb_Front_Temperature_One_Degree <= 20 )
    {
        /*..hui [23-4-7���� 11:16:02] 20�� ����..*/
        mu8_return = BLDC_COMP_66Hz;
    }
    else if( gu8_Amb_Front_Temperature_One_Degree <= 25 )
    {
        /*..hui [23-4-7���� 11:16:06] 25�� ����..*/
        mu8_return = BLDC_COMP_66Hz;
    }
    else if( gu8_Amb_Front_Temperature_One_Degree <= 30 )
    {
        /*..hui [23-4-7���� 11:16:10] 30�� ����..*/
        mu8_return = BLDC_COMP_66Hz;
    }
    else
    {
        /*..hui [23-4-7���� 11:16:14] 30�� �ʰ�..*/
        mu8_return = BLDC_COMP_65Hz;
    }

    return mu8_return;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 get_hotgas_mode_comp_rps(void)
{
    U8 mu8_return = 0;

    if( gu8_Amb_Temperature_One_Degree < HOT_GAS_AMB_TEMP_9_DIGREE )
    {
        /*..hui [23-4-7���� 1:17:04] 9�� �̸�..*/
        mu8_return = BLDC_COMP_50Hz;
    }
    else if( gu8_Amb_Temperature_One_Degree <= HOT_GAS_AMB_TEMP_13_DIGREE )
    {
        /*..hui [23-4-7���� 1:23:42] 9�� ~ 13��..*/
        mu8_return = BLDC_COMP_50Hz;
    }
    else if( gu8_Amb_Temperature_One_Degree <= HOT_GAS_AMB_TEMP_19_DIGREE )
    {
        /*..hui [23-4-7���� 1:23:55] 14�� ~ 19��..*/
        mu8_return = BLDC_COMP_50Hz;
    }
    else if( gu8_Amb_Temperature_One_Degree <= HOT_GAS_AMB_TEMP_24_DIGREE )
    {
        /*..hui [23-4-7���� 1:24:08] 20�� ~ 24��..*/
        mu8_return = BLDC_COMP_47Hz;
    }
    else if( gu8_Amb_Temperature_One_Degree <= HOT_GAS_AMB_TEMP_29_DIGREE )
    {
        /*..hui [23-4-7���� 1:24:18] 25�� ~ 29��..*/
        mu8_return = BLDC_COMP_47Hz;
    }
    else
    {
        /*..hui [23-4-7���� 1:24:23] 30�� �̻�..*/
        /*mu8_return = BLDC_COMP_43Hz;*/
        /*..hui [23-5-24���� 10:59:56] û�� ����..*/
        /*mu8_return = BLDC_COMP_51Hz;*/
        mu8_return = BLDC_COMP_43Hz;
    }

    return mu8_return;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 get_preheat_mode_comp_rps(void)
{
    /*..hui [23-4-7���� 5:00:06] ���� ���?RPS 60HZ..*/
    return BLDC_COMP_60Hz;
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U16 get_hotgas_time(void)
{
    U16 mu16_return = 0;

	/*.. sean [25-02-04] gu8_Amb_Temperature_One_Degree���� ó���Ǿ� front�� ����..*/
    if( gu8_Amb_Front_Temperature_One_Degree < HOT_GAS_AMB_TEMP_9_DIGREE )
    {
        /*..hui [23-4-7���� 1:17:04] 9�� �̸�..*/
        mu16_return = HOT_GAS_TIME_9_UNDER_765S;
    }
    else if( gu8_Amb_Front_Temperature_One_Degree <= HOT_GAS_AMB_TEMP_13_DIGREE )
    {
        /*..hui [23-4-7���� 1:23:42] 9�� ~ 13��..*/
        mu16_return = HOT_GAS_TIME_13_UNDER_600S;
    }
    else if( gu8_Amb_Front_Temperature_One_Degree <= HOT_GAS_AMB_TEMP_19_DIGREE )
    {
        /*..hui [23-4-7���� 1:23:55] 14�� ~ 19��..*/
        mu16_return = HOT_GAS_TIME_19_UNDER_180S;
    }
    else if( gu8_Amb_Front_Temperature_One_Degree <= HOT_GAS_AMB_TEMP_24_DIGREE )
    {
        /*..hui [23-4-7���� 1:24:08] 20�� ~ 24��..*/
        mu16_return = HOT_GAS_TIME_24_UNDER_30S;
    }
    else if( gu8_Amb_Front_Temperature_One_Degree <= HOT_GAS_AMB_TEMP_29_DIGREE )
    {
        /*..hui [23-4-7���� 1:24:18] 25�� ~ 29��..*/
        mu16_return = HOT_GAS_TIME_29_UNDER_20S;
    }
    else
    {
        /*..hui [23-4-7���� 1:24:23] 30�� �̻�..*/
        mu16_return = HOT_GAS_TIME_30_OVER_15S;
    }

    return mu16_return;
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U16 get_preheat_time(void)
{
    U16 mu16_return = 0;

    if( gu8_Amb_Temperature_One_Degree < 14 )
    {
        /*..hui [23-4-7���� 5:05:22] 14�� �̸�..*/
        mu16_return = PREHEAT_TIME_14_UNDER_600S;
    }
    else if( gu8_Amb_Temperature_One_Degree < 20 )
    {
        /*..hui [23-4-7���� 5:05:35] 14�� ~ 19��..*/
        mu16_return = PREHEAT_TIME_20_UNDER_360S;
    }
    else
    {
        /*..hui [23-4-7���� 5:05:45] 20�� ~ 24��..*/
        mu16_return = PREHEAT_TIME_25_UNDER_360S;
    }

    return mu16_return;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void get_average_tray_temp(void)
{
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/



