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
#include    "hot_triac.h"

void INTTM01_Triac_Timer(void);
void output_triac(void);
U8 preheating_temp_control(void);
U8 mainheating_temp_control(void);
U8 check_over_heat(void);
U8 triac_duty_oout(void);
void triac_control(void);

TYPE_WORD          U16WaterHeaterOnW;
#define            u16WaterHeaterOn                          U16WaterHeaterOnW.word
#define            u16WaterHeater_L                          U16WaterHeaterOnW.byte[0]
#define            u16WaterHeater_H                          U16WaterHeaterOnW.byte[1]
#define            Bit0_Pre_Heating_State                    U16WaterHeaterOnW.Bit.b0
#define            Bit1_Main_Heating_State                   U16WaterHeaterOnW.Bit.b1
#define            Bit2_First_Heating_Check_State            U16WaterHeaterOnW.Bit.b2
#define            Bit3_Tray_Ster_Heating_State              U16WaterHeaterOnW.Bit.b3



TYPE_WORD          U16WaterHeaterOffW;
#define            u16WaterHeaterOff                         U16WaterHeaterOffW.word
#define            u16WaterHeaterOff_L                       U16WaterHeaterOffW.byte[0]
#define            u16WaterHeaterOff_H                       U16WaterHeaterOffW.byte[1]
#define            Bit0_Over_Heat_State                      U16WaterHeaterOffW.Bit.b0
#define            Bit1_Water_Off_State                      U16WaterHeaterOffW.Bit.b1
#define            Bit2_Hot_Water_Select_Off_State           U16WaterHeaterOffW.Bit.b2
#define            Bit3_Hot_Temp_Much_Difference_Off_State   U16WaterHeaterOffW.Bit.b3
#define            Bit4_Low_Hot_Flow_Off_State               U16WaterHeaterOffW.Bit.b4
#define            Bit5_Damaged_Heater_Off_State             U16WaterHeaterOffW.Bit.b5
#define            Bit6_Heater_Make_Cool_State               U16WaterHeaterOffW.Bit.b6


TYPE_BYTE          U8HeaterMaxLimitB;
#define            u8HeaterMaxLimit                          U8HeaterMaxLimitB.byte
#define            Bit0_Over_1st_Limit                       U8HeaterMaxLimitB.Bit.b0
#define            Bit1_Over_2nd_Limit                       U8HeaterMaxLimitB.Bit.b1
#define            Bit2_Over_Flow_Increase_One               U8HeaterMaxLimitB.Bit.b2
#define            Bit3_Over_Flow_Increase_Two               U8HeaterMaxLimitB.Bit.b3
#define            Bit4_Over_Flow_Increase_Three             U8HeaterMaxLimitB.Bit.b4
#define            Bit5_Over_Flow_Increase_Four              U8HeaterMaxLimitB.Bit.b5
#define            Bit6_Over_Flow_Increase_Five              U8HeaterMaxLimitB.Bit.b6
#define            Bit7_Under_Flow_Decrease                  U8HeaterMaxLimitB.Bit.b7





bit Bit_Relay_Off_Holding;					// �� 2025-06-09 Phil 

U8 gu8CntTriacDuty;

U8  gu8TriacLevelMax;           // ���� ������ �°� ���� Ʈ���̾� ���� �ܰ� (���)
bit Bit_Triac_Output;
bit Bit_Relay_Output;
U8  u8Delta_Power_Level;
U8 gu8TriacLevel;
U8 gu8_real;

U8 gu8_realddddd;

bit F_Source_Change;
U8 gu8_source_change_time;

U16 gu16_overheat_timer;

/*F32 a,b,c,d,e,f;*/


U8 gu8_low_water_detect_timer;

//U8 gu8_adjust_heater_power_up;
//U8 gu8_adjust_heater_power_down;

//S8 gs8_adjust_heater_power;
//U8 gu8_adjust_heater_timer;

bit bit_increase_flow;

bit bit_triac_off;




bit bit_overheat;
U8 gu8_return_overheat;


U8 gu8_Test_dbg_drain_Mode;

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void INTTM01_Triac_Timer(void)
{
    output_triac();
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void output_triac(void)
{

    triac_control();

    //cbr_test
    if(F_hot_flow_error == SET) 
    {
        Bit0_Pre_Heating_State = CLEAR;
        Bit1_Main_Heating_State = CLEAR;
    }
    else
    {
        Bit0_Pre_Heating_State = preheating_temp_control();
        Bit1_Main_Heating_State = mainheating_temp_control();
    }//cbr_test

    if(gu8_Hot_Filling_Step == 5)
    {
        Bit2_First_Heating_Check_State = bit_hot_filling_start & F_First_Hot_Effluent;
    }
    else
    {
        Bit2_First_Heating_Check_State = CLEAR;
    }

    /*..hui [25-4-9���� 7:29:25] ���̽�Ʈ���� ��ô��..*/
    Bit3_Tray_Ster_Heating_State = tray_ster_temp_control();

    /*****************************************************************************************************************/

    Bit0_Over_Heat_State = check_over_heat();

    if( F_First_Hot_Effluent == CLEAR )
    {
        if( bit_ice_tank_ster_start == CLEAR )
        {
            Bit1_Water_Off_State = ~F_WaterOut;
        }
        else
        {
            Bit1_Water_Off_State = CLEAR;
        }
    }
    else
    {
        Bit1_Water_Off_State = CLEAR;
    }

    if( F_First_Hot_Effluent == CLEAR )
    {
        if( bit_ice_tank_ster_start == CLEAR )
        {
            if(u8WaterOutState != HOT_WATER_SELECT)
            {
                Bit2_Hot_Water_Select_Off_State = SET;
            }
            else
            {
                Bit2_Hot_Water_Select_Off_State = CLEAR;
            }
        }
        else
        {
            Bit2_Hot_Water_Select_Off_State = CLEAR;
        }
    }
    else
    {
        Bit2_Hot_Water_Select_Off_State = CLEAR;
    }

    /*..hui [19-11-28���� 10:19:58] ����µ��� ���Ϳµ� 15�� ���̳���� ���� ���� ����..*/
    /*..hui [20-9-22���� 8:17:57] �������� ����Ǹ鼭 ��� ����..*/
    /*Bit3_Hot_Temp_Much_Difference_Off_State = F_Hot_Temp_Much_Difference_State;*/


    /*..hui [20-4-22���� 1:05:10] ���� �Ҽ� ����.. 1500W ���� ���� �� ��������.. 5ȸ ���� ���� �� ��������..*/
    /*Bit5_Damaged_Heater_Off_State = (bit_damaged_heater_error_once | bit_damaged_heater_error_fixed);*/

    Bit6_Heater_Make_Cool_State = (bit)gu8_hot_drain_cold;

    /*****************************************************************************************************************/
    if (u16WaterHeaterOff > 0)
    {
        pHEATER_HOT_H_RELAY = CLEAR;              /*off*/
        pHEATER_HOT_TRIAC_L = CLEAR;
        Bit_Relay_Output = CLEAR;
        gu8TriacLevelMax = 0;
        F_Source_Change = CLEAR;
        gu8CntTriacDuty = 0;
    }
    else
    {
        if (u16WaterHeaterOn > 0)      /*on*/
        {
            if(Bit2_First_Heating_Check_State == SET)
            {
                pHEATER_HOT_H_RELAY = SET;
                pHEATER_HOT_TRIAC_L = SET;
                gu8TriacLevelMax = STEP_TRIAC;
            }
            else if(Bit0_Pre_Heating_State == SET)
            {
                pHEATER_HOT_H_RELAY = Bit_Relay_Output;
                pHEATER_HOT_TRIAC_L = triac_duty_oout();
            }
            else if(Bit1_Main_Heating_State == SET)
            {
                pHEATER_HOT_H_RELAY = Bit_Relay_Output;
                pHEATER_HOT_TRIAC_L = triac_duty_oout();
            }
            else if( Bit3_Tray_Ster_Heating_State == SET )
            {
                pHEATER_HOT_H_RELAY = Bit_Relay_Output;
                pHEATER_HOT_TRIAC_L = triac_duty_oout();
            }
            else
            {
                pHEATER_HOT_H_RELAY = CLEAR;
                pHEATER_HOT_TRIAC_L = CLEAR;
                Bit_Relay_Output = CLEAR;
                gu8TriacLevelMax = 0;
                gu8CntTriacDuty = 0;
            }
        }
        else
        {
            pHEATER_HOT_H_RELAY = CLEAR;          /*off*/
            pHEATER_HOT_TRIAC_L = CLEAR;
            Bit_Relay_Output = CLEAR;
            gu8TriacLevelMax = 0;
            F_Source_Change = CLEAR;
            gu8CntTriacDuty = 0;
        }
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 preheating_temp_control(void)
{
    U8 mu8_return = CLEAR;

    if(F_WaterOut != SET)
    {
        return mu8_return;
    }
    else{}

    if(u8WaterOutState != HOT_WATER_SELECT)
    {
        return mu8_return;
    }
    else{}


    if( (  gu8_Water_Out_Step < STATE_20_HOT_PRE_HEATING_STATE ) 
	       || ( gu8_Water_Out_Step > STATE_22_HOT_PRE_HEATING_BUFFER_DRAIN_COMPLETE_STATE ) )
    {
        return mu8_return;
    }
// ��2025-06-08 Phil
    else if(gu8_Hot_Heater_Temperature_One_Degree > gu8_Hot_Preheat_Temp + 10)  
		{
        return mu8_return;
		}
// ��2025-06-08 Phil
    else{}

    //if(gu8_Preheat_Step < 3)STATE_2_PREHEAT_DRAIN_STATE
    // �ܼ�üũ ���� �����ϵ��� �ؾ���
//    //if( gu8_Preheat_Step < STATE_1_PREHEAT_CHECK_OUTAGE_STATE )
//    if( gu8_Preheat_Step < STATE_2_PREHEAT_DRAIN_STATE )
		//if( gu8_Preheat_Step < STATE_1_PREHEAT_CHECK_OUTAGE_STATE )		// �� 2025-06-11 Phil 
	if( gu8_Preheat_Step < STATE_2_PREHEAT_DRAIN_STATE ) 	// �� 2025-07-07 sean 
    {
        return mu8_return;
    }
    else{}
	// [25-5-12] sean, Noble Ro ������� �� ������� �����Ұ�� �����ð��� �����ɸ��� ���� �� ���� 
	// [25-5-14] sean, 70�� �̻� �������� ���� ����, preheat���� �µ� ���ø����� ��������

    /*..hui [19-11-7���� 9:25:34] �������� ���? ���� ���? ����..*/
    if( Bit1_Hot_Operation_Disable_State == SET )
    {
        mu8_return = CLEAR;
    }
    else
    {
        mu8_return = SET;
    }

    /*mu8_return = SET;*/

    return mu8_return;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 mainheating_temp_control(void)
{
    U8 mu8_return = CLEAR;
    U8 mu8_diff = 0;
    U8 mu8_ref_temp = 0;


    if(F_WaterOut != SET)
    {
        return mu8_return;
    }
    else{}

    if(u8WaterOutState != HOT_WATER_SELECT)
    {
        return mu8_return;
    }
    else{}

    // [25-05-23 16:54:54]  yspark ���� ��ũ ��ä��� �ð� Heating �� -> �������ÿ� �߰� 
    if( gu8_Water_Out_Step == STATE_21_HOT_PRE_HEATING_BUFFER_DRAIN_STATE
    		|| gu8_Water_Out_Step == STATE_22_HOT_PRE_HEATING_BUFFER_DRAIN_COMPLETE_STATE  // ��2025-06-08 Phil 
        || gu8_Water_Out_Step == STATE_30_EXTRACT_VALVE_ON_STATE
        || gu8_Water_Out_Step == STATE_31_WATER_EXTRACT_STATE
        || gu8_Water_Out_Step == STATE_32_WATER_AIR_VENT_STATE
        )
    {
        /*return mu8_return;*/
    }
    else
    {
        return mu8_return;
    }

	/*..sean [25-05-12] ���� �µ� ���� ����..*/
    switch( gu8_hot_setting_temperature )
    {
        case HOT_SET_TEMP____45oC:

            /*..hui [19-2-22���� 8:05:08] ���� 45��..*/
            //mu8_diff = 12;
            mu8_diff = 30;// ��2025-06-02 Phil   

            break;
        case HOT_SET_TEMP____50oC:

            /*..sean [25-05-12] �߰� �µ� 50��..*/
            //mu8_diff = 12;
            mu8_diff = 30;// ��2025-06-02 Phil  
            break;
		
        case HOT_SET_TEMP____55oC:

            /*..sean [25-05-12] �߰� �µ� 55��..*/
            //mu8_diff = 12;
            mu8_diff = 30;// ��2025-06-02 Phil  
            break;
		
        case HOT_SET_TEMP____60oC:

            /*..sean [25-05-12] �߰� �µ� 60��..*/
            //mu8_diff = 15;
            mu8_diff = 30;// ��2025-06-02 Phil  

            break;
		
        case HOT_SET_TEMP____65oC:

            /*..sean [25-05-12] �߰� �µ� 65��..*/
            //mu8_diff = 15;
            mu8_diff = 30;// ��2025-06-02 Phil  
            break;

        case HOT_SET_TEMP____70oC:

            /*..hui [19-2-22���� 8:05:13] �� 70��..*/
            //mu8_diff = 15;
            mu8_diff = 30;// ��2025-06-02 Phil  

            break;
		
        case HOT_SET_TEMP____75oC:

            /*..sean [25-05-12] �߰� �µ� 75��..*/

            //mu8_diff = 15;
            mu8_diff = 30;// ��2025-06-02 Phil  
            break;
		
        case HOT_SET_TEMP____80oC:

            /*..sean [25-05-12] �߰� �µ� 80��..*/
            mu8_diff = 12;

            break;


        case HOT_SET_TEMP____85oC:

            /*..hui [19-2-22���� 8:05:21] Ŀ�� 85��..*/
            mu8_diff = 12;

            break;

		
        case HOT_SET_TEMP____90oC:

            /*..sean [25-05-12] �߰� �µ� 90��..*/
            mu8_diff = 9;

            break;

		
        case HOT_SET_TEMP____95oC:

            /*..sean [25-05-12] �߰� �µ� 95��..*/
            mu8_diff = 9;

            break;


        case HOT_SET_TEMP____100oC:

            /*..hui [19-2-22���� 8:05:21] ���� �� 100��..*/
            mu8_diff = 9;

            break;

        default:

            mu8_diff = 5;

            break;
    }

	// ���� ������ 85�� / 100�� �߰���Ŵ 
    /*..sean [25-05-12] 90/95�� �ش� ���� �����ϰ� ����..*/
    if( (gu8_hot_setting_temperature == HOT_SET_TEMP____75oC) 
		|| ( gu8_hot_setting_temperature == HOT_SET_TEMP____80oC ) 
		|| ( gu8_hot_setting_temperature == HOT_SET_TEMP____85oC ) 
		|| ( gu8_hot_setting_temperature == HOT_SET_TEMP____90oC ) 
		|| ( gu8_hot_setting_temperature == HOT_SET_TEMP____95oC ) 
		|| ( gu8_hot_setting_temperature == HOT_SET_TEMP____100oC ) )
    {
        mu8_ref_temp = HOT_RELAY_TRIAC_OFF_TEMP + 1;
    }
    else
    {
        mu8_ref_temp = gu8_Hot_Target_Temp + mu8_diff;
    }

    /*if(gu8_Hot_Out_Temperature_One_Degree <= gu8_Hot_Target_Temp + mu8_diff)*/

    if( gu8_Hot_Out_Temperature_One_Degree <= mu8_ref_temp )
    {
        if(Bit1_Hot_Operation_Disable_State == SET)
        {
            mu8_return = CLEAR;
        }
        else
        {
            mu8_return = SET;
        }
    }
    else
    {
        mu8_return = CLEAR;
    }

    return mu8_return;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void triac_control(void)
{
    U8 mu8_target = 0;
    U8 mu8_max_limit = 0;
    U8 mu8_up_ref = 0;

    U8 mu8_overheat = 0;
    U8 mu8_overheat_clear = 0;

    U8 mu8_min_power = 0;

    if(F_WaterOut != SET)
    {
        u8HeaterMaxLimit = 0;

        return;
    }
    else{}

    if(u8WaterOutState != HOT_WATER_SELECT)
    {
        return;
    }
    else{}


    // if( gu8_Water_Out_Step == STATE_30_EXTRACT_VALVE_ON_STATE
    //     || gu8_Water_Out_Step == STATE_31_WATER_EXTRACT_STATE )
    // {
    //     mu8_target = gu8_Hot_Target_Temp;
    // }
    // else if(gu8_Water_Out_Step == STATE_20_HOT_PRE_HEATING_STATE
    //     && gu8_Preheat_Step >= STATE_1_PREHEAT_CHECK_OUTAGE_STATE)
    // {
    //     if( gu8_hot_setting_temperature == HOT_SET_TEMP_4__MAX__100_oC
    //         || gu8_hot_setting_temperature == HOT_SET_TEMP_3__COFFEE__85_oC
    //      || gu8_hot_setting_temperature == HOT_SET_TEMP_2__TEA__70_oC )
    //     {
    //         mu8_target = gu8_Hot_Preheat_Temp;
    //     }
    //     else
    //     {
    //         if( gu8_Preheat_Step >= STATE_3_PREHEAT_CHECK_TIME )
    //         {
    //             mu8_target = gu8_Hot_Preheat_Temp;
    //         }
    //         else
    //         {
    //             return;
    //         }

    //     }
    // }
    // else
    // {
    //     return;
    // }

////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
    /*..hui [20-9-23���� 1:19:12] Ʈ���̾� ���� OFF..*/
    mu8_overheat = HOT_TRIAC_OFF_TEMP;
    mu8_overheat_clear = HOT_TRIAC_OFF_RETURN_TEMP;

    if( gu8_Hot_Out_Temperature_One_Degree >= mu8_overheat
        || gu8_Hot_Heater_Temperature_One_Degree >= mu8_overheat )
    {
        //u8Delta_Power_Level = gu8_min_power;
		/*sean [25-05-08]*/
        //u8Delta_Power_Level = 60;
        u8Delta_Power_Level = gu8_min_power;

        bit_triac_off = SET;
    }
    else
    {
        if( bit_triac_off == SET )
        {
            if( gu8_Hot_Out_Temperature_One_Degree <= mu8_overheat_clear
                && gu8_Hot_Heater_Temperature_One_Degree <= mu8_overheat_clear )
            {
                bit_triac_off = CLEAR;
            }
            else
            {
                if(u8Delta_Power_Level >= gu8_min_power)
                {
                    u8Delta_Power_Level = gu8_min_power;
                }
                else{}
            }
        }
        else{}
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
    if(u16Heater_Power_Save >= HEATER_POWER_HIGH)
    {  // ������ ��
        // [25-05-23 18:13:08] yspark, ��� Ǯ������ �ϰ� �־� ���� ���� �����Ͽ� �µ� ���߱�
        // [25-05-26 11:21:14] yspark, Triac�� ���� 
        if (gu8_Hot_In_Temperature_One_Degree <= 15)
        {
            // [25-05-23 18:13:08] yspark, ��� Ǯ������ �ϰ� �־� ���� ���� �����Ͽ� �µ� ���߱� 
            if(((gu8_hot_setting_temperature == HOT_SET_TEMP____75oC)
                    || (gu8_hot_setting_temperature == HOT_SET_TEMP____70oC)
                    || (gu8_hot_setting_temperature == HOT_SET_TEMP____65oC)
                    || (gu8_hot_setting_temperature == HOT_SET_TEMP____60oC)
                    || (gu8_hot_setting_temperature == HOT_SET_TEMP____55oC)
                    //|| (gu8_hot_setting_temperature == HOT_SET_TEMP____50oC)// ��2025-06-02 Phil
                    //|| (gu8_hot_setting_temperature == HOT_SET_TEMP____45oC)// ��2025-06-02 Phil
                    )
                && Bit1_Main_Heating_State == SET )
            {
                if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 10))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 0;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 9))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 10;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 8))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 20;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 7))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 30;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 6))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 40;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 5))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 50;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 4))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 60;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 3))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 70;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 2))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 80;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 1))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 90;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 100;
                }
                else
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = STEP_TRIAC;
                }
            }
// ��2025-06-02 Phil
            else if(((gu8_hot_setting_temperature == HOT_SET_TEMP____50oC)
                    || (gu8_hot_setting_temperature == HOT_SET_TEMP____45oC)
                    )
                && Bit1_Main_Heating_State == SET )
            {
                if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 10))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 0;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 9))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 10;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 8))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 20;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 7))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 30;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 6))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 40;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 5))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 50;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 4))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 60;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 3))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 70;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 2))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 80;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 1))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 90;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 100;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree <= gu8_Hot_Target_Temp - 5))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 100;
                }
                else
                {
                    //Bit_Relay_Output = SET;
                    gu8TriacLevelMax = STEP_TRIAC;
                }
            }
// ��2025-06-02 Phil
            else
            {
                Bit_Relay_Output = SET;
                gu8TriacLevelMax = (U8)((u8Delta_Power_Level - 45)*2);
            }
        }
        else
        {
            if( ((gu8_hot_setting_temperature == HOT_SET_TEMP____65oC)
                        || (gu8_hot_setting_temperature == HOT_SET_TEMP____60oC)
                        || (gu8_hot_setting_temperature == HOT_SET_TEMP____55oC)
                        || (gu8_hot_setting_temperature == HOT_SET_TEMP____50oC)
                        || (gu8_hot_setting_temperature == HOT_SET_TEMP____45oC)
                        )
                && Bit1_Main_Heating_State == SET )
            {
                if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 10))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 0;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 9))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 10;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 8))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 20;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 7))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 30;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 6))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 40;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 5))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 50;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 4))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 60;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 3))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 70;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 2))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 80;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 1))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 90;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 100;
                }
                // ��2025-06-02 Phil  
                else if ((gu8_Hot_Out_Temperature_One_Degree <= gu8_Hot_Target_Temp - 5))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 100;
                }
                // ��2025-06-02 Phil  
                else
                {
                    //Bit_Relay_Output = CLEAR;// ��2025-06-02 Phil  (���� ��� ����)
                    gu8TriacLevelMax = STEP_TRIAC;
                }
            } 
//----------------
            //if( ((gu8_hot_setting_temperature == HOT_SET_TEMP____100oC)
            else if( ((gu8_hot_setting_temperature == HOT_SET_TEMP____100oC)
                        || (gu8_hot_setting_temperature == HOT_SET_TEMP____95oC)
                        || (gu8_hot_setting_temperature == HOT_SET_TEMP____90oC)
                        || (gu8_hot_setting_temperature == HOT_SET_TEMP____85oC)
                        || (gu8_hot_setting_temperature == HOT_SET_TEMP____80oC)
                        )
                && Bit1_Main_Heating_State == SET && u8Target_Hz_Hot > gu8_hot_filling_max_hz)
            {
                if ((gu8_Hot_Out_Temperature_One_Degree >= 98))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 0;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp - 3))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 0;
                }
                // ��2025-06-02 Phil  
                else if ((gu8_Hot_Out_Temperature_One_Degree <= gu8_Hot_Target_Temp - 5))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 100;
                }
                // ��2025-06-02 Phil  
                else
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = STEP_TRIAC;
                }
            } 

//----------------
            // [25-05-23 18:13:08] yspark, ��� Ǯ������ �ϰ� �־� ���� ���� �����Ͽ� �µ� ���߱� 
            else if( ((gu8_hot_setting_temperature == HOT_SET_TEMP____75oC)
                        || (gu8_hot_setting_temperature == HOT_SET_TEMP____70oC)
                        )
                && Bit1_Main_Heating_State == SET )
            {
                if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 10))
                {
                    //Bit_Relay_Output = SET;
                    // Bit_Relay_Output = CLEAR;// ��2025-06-08 Phil  (���� ��� ����)
                    Bit_Relay_Output = SET;     //2025-07-16 cbr 70�� ù�� �̴�
                    gu8TriacLevelMax = 0;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 9))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 10;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 8))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 20;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 7))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 30;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 6))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 40;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 5))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 50;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 4))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 60;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 3))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 70;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 2))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 80;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 1))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 90;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 100;
                }
                else
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = STEP_TRIAC;
                }
            }
            else
            {
                if(u8Delta_Power_Level >= 45)
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax =  (U8)((u8Delta_Power_Level - 45)*2);
                }
                else
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax =  0;
                }
            }
        }
    }
    else if (u16Heater_Power_Save <= HEATER_POWER_LOW)
    {  // ������ ��
        if (gu8_Hot_In_Temperature_One_Degree <= 15)
        {
            // [25-05-23 18:13:08] yspark, ��� Ǯ������ �ϰ� �־� ���� ���� �����Ͽ� �µ� ���߱� 
            if(((gu8_hot_setting_temperature == HOT_SET_TEMP____75oC)
                    || (gu8_hot_setting_temperature == HOT_SET_TEMP____70oC)
                    || (gu8_hot_setting_temperature == HOT_SET_TEMP____65oC)
                    || (gu8_hot_setting_temperature == HOT_SET_TEMP____60oC)
                    || (gu8_hot_setting_temperature == HOT_SET_TEMP____55oC)
                    //|| (gu8_hot_setting_temperature == HOT_SET_TEMP____50oC)// ��2025-06-02 Phil
                    //|| (gu8_hot_setting_temperature == HOT_SET_TEMP____45oC)// ��2025-06-02 Phil
                    )
                && Bit1_Main_Heating_State == SET )
            {
                if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 10))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 0;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 9))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 10;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 8))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 20;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 7))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 30;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 6))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 40;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 5))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 50;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 4))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 60;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 3))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 70;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 2))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 80;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 1))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 90;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 100;
                }
                else
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = STEP_TRIAC;
                }
            }
// ��2025-06-02 Phil
            else if( ((gu8_hot_setting_temperature == HOT_SET_TEMP____50oC)
                || (gu8_hot_setting_temperature == HOT_SET_TEMP____45oC)
                )
                && Bit1_Main_Heating_State == SET )
            {
                if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 10))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 0;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 9))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 10;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 8))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 20;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 7))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 30;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 6))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 40;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 5))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 50;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 4))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 60;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 3))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 70;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 2))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 80;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 1))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 90;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 100;
                }
                // ��2025-06-02 Phil  
                else if ((gu8_Hot_Out_Temperature_One_Degree <= gu8_Hot_Target_Temp - 5))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 100;
                }
                // ��2025-06-02 Phil  
                else
                {
                    //Bit_Relay_Output = CLEAR;// ��2025-06-02 Phil  (���� ��� ����)
                    gu8TriacLevelMax = STEP_TRIAC;
                }
            }
// ��2025-06-02 Phil
            else
            {
                Bit_Relay_Output = SET;
                gu8TriacLevelMax = (U8)((u8Delta_Power_Level - 45)*2);
            }
        }
        else if (gu8_Hot_In_Temperature_One_Degree <= 25)
        {
            if (gu8_hot_setting_temperature == HOT_SET_TEMP____45oC
                && Bit1_Main_Heating_State == SET)
            {
                if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 10))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 0;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 9))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 10;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 8))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 20;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 7))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 30;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 6))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 40;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 5))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 50;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 4))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 60;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 3))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 70;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 2))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 80;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 1))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 90;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 100;
                }
                // ��2025-06-02 Phil  
                else if ((gu8_Hot_Out_Temperature_One_Degree <= gu8_Hot_Target_Temp - 5))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 100;
                }
                // ��2025-06-02 Phil  
                else
                {
                    //Bit_Relay_Output = CLEAR;// ��2025-06-02 Phil  (���� ��� ����)
                    gu8TriacLevelMax = STEP_TRIAC;
                }
            } 
            // [25-05-23 18:13:08] yspark, ��� Ǯ������ �ϰ� �־� ���� ���� �����Ͽ� �µ� ���߱� 
            else if(((gu8_hot_setting_temperature == HOT_SET_TEMP____75oC)
                    || (gu8_hot_setting_temperature == HOT_SET_TEMP____70oC)
                    || (gu8_hot_setting_temperature == HOT_SET_TEMP____65oC)
                    || (gu8_hot_setting_temperature == HOT_SET_TEMP____60oC)
                    || (gu8_hot_setting_temperature == HOT_SET_TEMP____55oC)
                    || (gu8_hot_setting_temperature == HOT_SET_TEMP____50oC)
                    )
                && Bit1_Main_Heating_State == SET )
            {
                if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 10))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 0;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 9))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 10;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 8))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 20;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 7))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 30;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 6))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 40;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 5))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 50;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 4))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 60;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 3))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 70;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 2))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 80;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 1))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 90;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 100;
                }
                else
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = STEP_TRIAC;
                }
            }
            else
            {
                Bit_Relay_Output = SET;
                gu8TriacLevelMax = (U8)((u8Delta_Power_Level - 45)*2);
            }
        }
        else
        {
            if( ((gu8_hot_setting_temperature == HOT_SET_TEMP____50oC)
                || (gu8_hot_setting_temperature == HOT_SET_TEMP____45oC)
                )
                && Bit1_Main_Heating_State == SET )
            {
                if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 10))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 0;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 9))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 10;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 8))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 20;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 7))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 30;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 6))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 40;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 5))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 50;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 4))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 60;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 3))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 70;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 2))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 80;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 1))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 90;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 100;
                }
                // ��2025-06-02 Phil  
                else if ((gu8_Hot_Out_Temperature_One_Degree <= gu8_Hot_Target_Temp - 5))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 100;
                }
                // ��2025-06-02 Phil  
                else
                {
                    //Bit_Relay_Output = CLEAR;// ��2025-06-02 Phil  (���� ��� ����)
                    gu8TriacLevelMax = STEP_TRIAC;
                }
            } 
            // [25-05-23 18:13:08] yspark, ��� Ǯ������ �ϰ� �־� ���� ���� �����Ͽ� �µ� ���߱� 
            else if(((gu8_hot_setting_temperature == HOT_SET_TEMP____75oC)
                    || (gu8_hot_setting_temperature == HOT_SET_TEMP____70oC)
                    || (gu8_hot_setting_temperature == HOT_SET_TEMP____65oC)
                    || (gu8_hot_setting_temperature == HOT_SET_TEMP____60oC)
                    || (gu8_hot_setting_temperature == HOT_SET_TEMP____55oC)
                    )
                && Bit1_Main_Heating_State == SET )
            {
                if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 10))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 0;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 9))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 10;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 8))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 20;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 7))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 30;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 6))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 40;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 5))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 50;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 4))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 60;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 3))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 70;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 2))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 80;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 1))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 90;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 100;
                }
                else
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = STEP_TRIAC;
                }
            }
            else
            {
                Bit_Relay_Output = SET;
                gu8TriacLevelMax = (U8)((u8Delta_Power_Level - 45)*2);
            }
        }
    }
    else
    {  // ��� ����
        if (gu8_Hot_In_Temperature_One_Degree <= 15)
        {
            // [25-05-23 18:13:08] yspark, ��� Ǯ������ �ϰ� �־� ���� ���� �����Ͽ� �µ� ���߱� 
            if(((gu8_hot_setting_temperature == HOT_SET_TEMP____75oC)
                    || (gu8_hot_setting_temperature == HOT_SET_TEMP____70oC)
                    || (gu8_hot_setting_temperature == HOT_SET_TEMP____65oC)
                    || (gu8_hot_setting_temperature == HOT_SET_TEMP____60oC)
                    || (gu8_hot_setting_temperature == HOT_SET_TEMP____55oC)
                    //|| (gu8_hot_setting_temperature == HOT_SET_TEMP____50oC)// ��2025-06-02 Phil
                    //|| (gu8_hot_setting_temperature == HOT_SET_TEMP____45oC)// ��2025-06-02 Phil
                    )
                && Bit1_Main_Heating_State == SET )
            {
                if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 10))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 0;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 9))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 10;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 8))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 20;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 7))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 30;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 6))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 40;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 5))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 50;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 4))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 60;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 3))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 70;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 2))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 80;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 1))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 90;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 100;
                }
                else
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = STEP_TRIAC;
                }
            }
// ��2025-06-02 Phil
            else if( ((gu8_hot_setting_temperature == HOT_SET_TEMP____50oC)
                || (gu8_hot_setting_temperature == HOT_SET_TEMP____45oC)
                )
                && Bit1_Main_Heating_State == SET )
            {
                if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 10))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 0;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 9))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 10;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 8))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 20;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 7))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 30;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 6))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 40;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 5))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 50;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 4))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 60;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 3))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 70;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 2))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 80;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 1))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 90;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 100;
                }
                // ��2025-06-02 Phil  
                else if ((gu8_Hot_Out_Temperature_One_Degree <= gu8_Hot_Target_Temp - 5))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 100;
                }
                // ��2025-06-02 Phil  
                else
                {
                    //Bit_Relay_Output = CLEAR;// ��2025-06-02 Phil  (���� ��� ����)
                    gu8TriacLevelMax = STEP_TRIAC;
                }
            }
// ��2025-06-02 Phil
            else
            {
                Bit_Relay_Output = SET;
                gu8TriacLevelMax = (U8)((u8Delta_Power_Level - 45)*2);
            }
        }
        else if(gu8_Hot_In_Temperature_One_Degree <= 25)        //2025-07-29 cbr 20��,30�� ����
        {
            if( (  (gu8_hot_setting_temperature == HOT_SET_TEMP____55oC)
                || (gu8_hot_setting_temperature == HOT_SET_TEMP____50oC)
                || (gu8_hot_setting_temperature == HOT_SET_TEMP____45oC)
                )
                && Bit1_Main_Heating_State == SET )
            {
                if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 10))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 0;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 9))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 10;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 8))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 20;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 7))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 30;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 6))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 40;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 5))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 50;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 4))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 60;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 3))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 70;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 2))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 80;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 1))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 90;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 100;
                }
                // ��2025-06-02 Phil  
                else if ((gu8_Hot_Out_Temperature_One_Degree <= gu8_Hot_Target_Temp - 5))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 100;
                }
                // ��2025-06-02 Phil  
                else
                {
                    //Bit_Relay_Output = CLEAR;// ��2025-06-02 Phil  (���� ��� ����)
                    gu8TriacLevelMax = STEP_TRIAC;
                }
            }
            else if( (  (gu8_hot_setting_temperature == HOT_SET_TEMP____65oC)
                || (gu8_hot_setting_temperature == HOT_SET_TEMP____60oC)
                )
                && Bit1_Main_Heating_State == SET )
            {
                if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 10))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 0;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 9))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 10;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 8))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 20;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 7))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 30;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 6))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 40;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 5))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 50;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 4))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 60;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 3))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 70;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 2))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 80;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 1))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 90;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 100;
                }
                else
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = STEP_TRIAC;
                }
            }
            else if(((gu8_hot_setting_temperature == HOT_SET_TEMP____75oC)
                    || (gu8_hot_setting_temperature == HOT_SET_TEMP____70oC)
                    )
                && Bit1_Main_Heating_State == SET )
            {
                if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 10))
                {
                    //Bit_Relay_Output = SET;		// ��2025-06-08 Phil
                    if(u16Heater_Power_Save <= HEATER_POWER_LOW)    
                    {                
                    	Bit_Relay_Output = SET;		// ��2025-06-08 Phil
                    	gu8TriacLevelMax = 0;// ��2025-06-09 Phil 
                    }
                    else
                    {
                      Bit_Relay_Output = CLEAR;		// ��2025-06-08 Phil     
                      gu8TriacLevelMax = STEP_TRIAC;// ��2025-06-09 Phil  
                      Bit_Relay_Off_Holding = SET;  // ��2025-06-09 Phil              
                    }
                    //gu8TriacLevelMax = 0;// ��2025-06-09 Phil 
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 9))
                {
//                    Bit_Relay_Output = SET;
//                    //Bit_Relay_Output = CLEAR;		// ��2025-06-09 Phil  
//                    gu8TriacLevelMax = 10;
// ��2025-06-09 Phil                     
                    if(Bit_Relay_Off_Holding)
                    {
                      Bit_Relay_Output = CLEAR; 
                      gu8TriacLevelMax = STEP_TRIAC;                       
                    }
                    else
                    {
                      Bit_Relay_Output = SET;
                      gu8TriacLevelMax = 10;                    
                    }
// ��2025-06-09 Phil 
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 8))
                {
//                    Bit_Relay_Output = SET;
//                    //Bit_Relay_Output = CLEAR;		// ��2025-06-09 Phil  
//                    gu8TriacLevelMax = 20;
// ��2025-06-09 Phil                     
                    if(Bit_Relay_Off_Holding)
                    {
                      Bit_Relay_Output = CLEAR; 
                      gu8TriacLevelMax = STEP_TRIAC;                       
                    }
                    else
                    {
                      Bit_Relay_Output = SET;
                      gu8TriacLevelMax = 20;                    
                    }
// ��2025-06-09 Phil 
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 7))
                {
//                    Bit_Relay_Output = SET;
//                    //Bit_Relay_Output = CLEAR;		// ��2025-06-09 Phil  
//                    gu8TriacLevelMax = 30;
// ��2025-06-09 Phil                     
                    if(Bit_Relay_Off_Holding)
                    {
                      Bit_Relay_Output = CLEAR; 
                      gu8TriacLevelMax = STEP_TRIAC;                       
                    }
                    else
                    {
                      Bit_Relay_Output = SET;
                      gu8TriacLevelMax = 30;                    
                    }
// ��2025-06-09 Phil 
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 6))
                {
//                    Bit_Relay_Output = SET;
//                    //Bit_Relay_Output = CLEAR;		// ��2025-06-09 Phil  
//                    gu8TriacLevelMax = 40;
// ��2025-06-09 Phil                     
                    if(Bit_Relay_Off_Holding)
                    {
                      Bit_Relay_Output = CLEAR; 
                      gu8TriacLevelMax = STEP_TRIAC;                       
                    }
                    else
                    {
                      Bit_Relay_Output = SET;
                      gu8TriacLevelMax = 40;                    
                    }
// ��2025-06-09 Phil 
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 5))
                {
//                    //Bit_Relay_Output = SET;
//                    Bit_Relay_Output = CLEAR;		// ��2025-06-09 Phil  
//                    gu8TriacLevelMax = 50;
// ��2025-06-09 Phil                     
                    if(Bit_Relay_Off_Holding)
                    {
                      Bit_Relay_Output = CLEAR; 
                      gu8TriacLevelMax = STEP_TRIAC;                       
                    }
                    else
                    {
                      Bit_Relay_Output = SET;
                      gu8TriacLevelMax = 50;                    
                    }
// ��2025-06-09 Phil 
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 4))
                {
//                    Bit_Relay_Output = SET;
//                    //Bit_Relay_Output = CLEAR;		// ��2025-06-09 Phil  
//                    gu8TriacLevelMax = 60;
// ��2025-06-09 Phil                     
                    if(Bit_Relay_Off_Holding)
                    {
                      Bit_Relay_Output = CLEAR; 
                      gu8TriacLevelMax = STEP_TRIAC;                       
                    }
                    else
                    {
                      Bit_Relay_Output = SET;
                      gu8TriacLevelMax = 60;                    
                    }
// ��2025-06-09 Phil 
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 3))
                {
//                    Bit_Relay_Output = SET;
//                    //Bit_Relay_Output = CLEAR;		// ��2025-06-09 Phil  
//                    gu8TriacLevelMax = 70;
// ��2025-06-09 Phil                     
                    if(Bit_Relay_Off_Holding)
                    {
                      Bit_Relay_Output = CLEAR; 
                      gu8TriacLevelMax = STEP_TRIAC;                       
                    }
                    else
                    {
                      Bit_Relay_Output = SET;
                      gu8TriacLevelMax = 70;                    
                    }
// ��2025-06-09 Phil 
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 2))
                {
//                    Bit_Relay_Output = SET;
//                    //Bit_Relay_Output = CLEAR;		// ��2025-06-09 Phil  
//                    gu8TriacLevelMax = 80;
// ��2025-06-09 Phil                     
                    if(Bit_Relay_Off_Holding)
                    {
                      Bit_Relay_Output = CLEAR; 
                      gu8TriacLevelMax = STEP_TRIAC;                       
                    }
                    else
                    {
                      Bit_Relay_Output = SET;
                      gu8TriacLevelMax = 80;                    
                    }
// ��2025-06-09 Phil 
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 1))
                {
//                    Bit_Relay_Output = SET;
//                    //Bit_Relay_Output = CLEAR;		// ��2025-06-09 Phil  
//                    gu8TriacLevelMax = 90;
// ��2025-06-09 Phil                     
                    if(Bit_Relay_Off_Holding)
                    {
                      Bit_Relay_Output = CLEAR; 
                      gu8TriacLevelMax = STEP_TRIAC;                       
                    }
                    else
                    {
                      Bit_Relay_Output = SET;
                      gu8TriacLevelMax = 90;                    
                    }
// ��2025-06-09 Phil 
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp))
                {
//                    Bit_Relay_Output = SET;
//                    //Bit_Relay_Output = CLEAR;		// ��2025-06-09 Phil  
//                    gu8TriacLevelMax = 100;
// ��2025-06-09 Phil                     
                    if(Bit_Relay_Off_Holding)
                    {
                      Bit_Relay_Output = CLEAR; 
                      gu8TriacLevelMax = STEP_TRIAC;                       
                    }
                    else
                    {
                      Bit_Relay_Output = SET;
                      gu8TriacLevelMax = 100;                    
                    }
// ��2025-06-09 Phil 
                }
//                // ��2025-06-09 Phil  
//                else if ((gu8_Hot_Out_Temperature_One_Degree <= gu8_Hot_Target_Temp - 5))
//                {
//                    Bit_Relay_Output = SET;
//                    gu8TriacLevelMax = 100;
//                }
//                // ��2025-06-09 Phil 
                else
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = STEP_TRIAC;
                    Bit_Relay_Off_Holding = CLEAR;  // ��2025-06-09 Phil      
                }
            }
            else
            {
                if(u8Delta_Power_Level >= 45)
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax =  (U8)((u8Delta_Power_Level - 45)*2);
                }
                else
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax =  0;
                }
            }
        }       //2025-07-29 cbr
        else     //gu8_Hot_In_Temperature_One_Degree > 25
        {
            if( (  (gu8_hot_setting_temperature == HOT_SET_TEMP____65oC)      //2025-07-25 cbr_test
                || (gu8_hot_setting_temperature == HOT_SET_TEMP____60oC)    //2025-07-25 cbr_test
                || (gu8_hot_setting_temperature == HOT_SET_TEMP____55oC)    //2025-07-24 cbr
                || (gu8_hot_setting_temperature == HOT_SET_TEMP____50oC)
                || (gu8_hot_setting_temperature == HOT_SET_TEMP____45oC)
                )
                && Bit1_Main_Heating_State == SET )
            {
                if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 10))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 0;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 9))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 10;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 8))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 20;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 7))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 30;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 6))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 40;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 5))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 50;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 4))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 60;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 3))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 70;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 2))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 80;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 1))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 90;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 100;
                }
                // ��2025-06-02 Phil  
                else if ((gu8_Hot_Out_Temperature_One_Degree <= gu8_Hot_Target_Temp - 5))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 100;
                }
                // ��2025-06-02 Phil  
                else
                {
                    //Bit_Relay_Output = CLEAR;// ��2025-06-02 Phil  (���� ��� ����)
                    gu8TriacLevelMax = STEP_TRIAC;
                }
            } 
            // [25-05-23 18:13:08] yspark, ��� Ǯ������ �ϰ� �־� ���� ���� �����Ͽ� �µ� ���߱� 
            else if(((gu8_hot_setting_temperature == HOT_SET_TEMP____75oC)
                    || (gu8_hot_setting_temperature == HOT_SET_TEMP____70oC)
                    )
                && Bit1_Main_Heating_State == SET )
            {
                if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 10))
                {
                    //Bit_Relay_Output = SET;		// ��2025-06-08 Phil
                    if(u16Heater_Power_Save <= HEATER_POWER_LOW)    
                    {                
                    	Bit_Relay_Output = SET;		// ��2025-06-08 Phil
                    	gu8TriacLevelMax = 0;// ��2025-06-09 Phil 
                    }
                    else
                    {
                      Bit_Relay_Output = CLEAR;		// ��2025-06-08 Phil     
                      gu8TriacLevelMax = STEP_TRIAC;// ��2025-06-09 Phil  
                      Bit_Relay_Off_Holding = SET;  // ��2025-06-09 Phil              
                    }
                    //gu8TriacLevelMax = 0;// ��2025-06-09 Phil 
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 9))
                {
//                    Bit_Relay_Output = SET;
//                    //Bit_Relay_Output = CLEAR;		// ��2025-06-09 Phil  
//                    gu8TriacLevelMax = 10;
// ��2025-06-09 Phil                     
                    if(Bit_Relay_Off_Holding)
                    {
                      Bit_Relay_Output = CLEAR; 
                      gu8TriacLevelMax = STEP_TRIAC;                       
                    }
                    else
                    {
                      Bit_Relay_Output = SET;
                      gu8TriacLevelMax = 10;                    
                    }
// ��2025-06-09 Phil 
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 8))
                {
//                    Bit_Relay_Output = SET;
//                    //Bit_Relay_Output = CLEAR;		// ��2025-06-09 Phil  
//                    gu8TriacLevelMax = 20;
// ��2025-06-09 Phil                     
                    if(Bit_Relay_Off_Holding)
                    {
                      Bit_Relay_Output = CLEAR; 
                      gu8TriacLevelMax = STEP_TRIAC;                       
                    }
                    else
                    {
                      Bit_Relay_Output = SET;
                      gu8TriacLevelMax = 20;                    
                    }
// ��2025-06-09 Phil 
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 7))
                {
//                    Bit_Relay_Output = SET;
//                    //Bit_Relay_Output = CLEAR;		// ��2025-06-09 Phil  
//                    gu8TriacLevelMax = 30;
// ��2025-06-09 Phil                     
                    if(Bit_Relay_Off_Holding)
                    {
                      Bit_Relay_Output = CLEAR; 
                      gu8TriacLevelMax = STEP_TRIAC;                       
                    }
                    else
                    {
                      Bit_Relay_Output = SET;
                      gu8TriacLevelMax = 30;                    
                    }
// ��2025-06-09 Phil 
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 6))
                {
//                    Bit_Relay_Output = SET;
//                    //Bit_Relay_Output = CLEAR;		// ��2025-06-09 Phil  
//                    gu8TriacLevelMax = 40;
// ��2025-06-09 Phil                     
                    if(Bit_Relay_Off_Holding)
                    {
                      Bit_Relay_Output = CLEAR; 
                      gu8TriacLevelMax = STEP_TRIAC;                       
                    }
                    else
                    {
                      Bit_Relay_Output = SET;
                      gu8TriacLevelMax = 40;                    
                    }
// ��2025-06-09 Phil 
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 5))
                {
//                    //Bit_Relay_Output = SET;
//                    Bit_Relay_Output = CLEAR;		// ��2025-06-09 Phil  
//                    gu8TriacLevelMax = 50;
// ��2025-06-09 Phil                     
                    if(Bit_Relay_Off_Holding)
                    {
                      Bit_Relay_Output = CLEAR; 
                      gu8TriacLevelMax = STEP_TRIAC;                       
                    }
                    else
                    {
                      Bit_Relay_Output = SET;
                      gu8TriacLevelMax = 50;                    
                    }
// ��2025-06-09 Phil 
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 4))
                {
//                    Bit_Relay_Output = SET;
//                    //Bit_Relay_Output = CLEAR;		// ��2025-06-09 Phil  
//                    gu8TriacLevelMax = 60;
// ��2025-06-09 Phil                     
                    if(Bit_Relay_Off_Holding)
                    {
                      Bit_Relay_Output = CLEAR; 
                      gu8TriacLevelMax = STEP_TRIAC;                       
                    }
                    else
                    {
                      Bit_Relay_Output = SET;
                      gu8TriacLevelMax = 60;                    
                    }
// ��2025-06-09 Phil 
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 3))
                {
//                    Bit_Relay_Output = SET;
//                    //Bit_Relay_Output = CLEAR;		// ��2025-06-09 Phil  
//                    gu8TriacLevelMax = 70;
// ��2025-06-09 Phil                     
                    if(Bit_Relay_Off_Holding)
                    {
                      Bit_Relay_Output = CLEAR; 
                      gu8TriacLevelMax = STEP_TRIAC;                       
                    }
                    else
                    {
                      Bit_Relay_Output = SET;
                      gu8TriacLevelMax = 70;                    
                    }
// ��2025-06-09 Phil 
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 2))
                {
//                    Bit_Relay_Output = SET;
//                    //Bit_Relay_Output = CLEAR;		// ��2025-06-09 Phil  
//                    gu8TriacLevelMax = 80;
// ��2025-06-09 Phil                     
                    if(Bit_Relay_Off_Holding)
                    {
                      Bit_Relay_Output = CLEAR; 
                      gu8TriacLevelMax = STEP_TRIAC;                       
                    }
                    else
                    {
                      Bit_Relay_Output = SET;
                      gu8TriacLevelMax = 80;                    
                    }
// ��2025-06-09 Phil 
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp + 1))
                {
//                    Bit_Relay_Output = SET;
//                    //Bit_Relay_Output = CLEAR;		// ��2025-06-09 Phil  
//                    gu8TriacLevelMax = 90;
// ��2025-06-09 Phil                     
                    if(Bit_Relay_Off_Holding)
                    {
                      Bit_Relay_Output = CLEAR; 
                      gu8TriacLevelMax = STEP_TRIAC;                       
                    }
                    else
                    {
                      Bit_Relay_Output = SET;
                      gu8TriacLevelMax = 90;                    
                    }
// ��2025-06-09 Phil 
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp))
                {
//                    Bit_Relay_Output = SET;
//                    //Bit_Relay_Output = CLEAR;		// ��2025-06-09 Phil  
//                    gu8TriacLevelMax = 100;
// ��2025-06-09 Phil                     
                    if(Bit_Relay_Off_Holding)
                    {
                      Bit_Relay_Output = CLEAR; 
                      gu8TriacLevelMax = STEP_TRIAC;                       
                    }
                    else
                    {
                      Bit_Relay_Output = SET;
                      gu8TriacLevelMax = 100;                    
                    }
// ��2025-06-09 Phil 
                }
//                // ��2025-06-09 Phil  
//                else if ((gu8_Hot_Out_Temperature_One_Degree <= gu8_Hot_Target_Temp - 5))
//                {
//                    Bit_Relay_Output = SET;
//                    gu8TriacLevelMax = 100;
//                }
//                // ��2025-06-09 Phil 
                else
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = STEP_TRIAC;
                    Bit_Relay_Off_Holding = CLEAR;  // ��2025-06-09 Phil      
                }
            }
            //2025-07-24 cbr 30�� ���� 85�� ������ ����
            else if( ( (gu8_hot_setting_temperature == HOT_SET_TEMP____85oC)
                    || (gu8_hot_setting_temperature == HOT_SET_TEMP____80oC) )
                // && (gu8_Hot_In_Temperature_One_Degree > 25)
                && Bit1_Main_Heating_State == SET && u8Target_Hz_Hot > gu8_hot_filling_max_hz)
            {
                if ((gu8_Hot_Out_Temperature_One_Degree >= 98))
                {
                    Bit_Relay_Output = CLEAR;
                    gu8TriacLevelMax = 0;
                }
                else if ((gu8_Hot_Out_Temperature_One_Degree >= gu8_Hot_Target_Temp - 2))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 50;
                }
                // ��2025-06-02 Phil  
                else if ((gu8_Hot_Out_Temperature_One_Degree <= gu8_Hot_Target_Temp - 5))
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = 100;
                }
                // ��2025-06-02 Phil  
                else
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax = STEP_TRIAC;
                }
            }
            else
            {
                if(u8Delta_Power_Level >= 45)
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax =  (U8)((u8Delta_Power_Level - 45)*2);
                }
                else
                {
                    Bit_Relay_Output = SET;
                    gu8TriacLevelMax =  0;
                }
            }
        } 
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 check_over_heat(void)
{
    U8 mu8_return = CLEAR;

    if( gu8_Hot_Out_Temperature_One_Degree >= HOT_RELAY_TRIAC_OFF_TEMP
        || gu8_Hot_Heater_Temperature_One_Degree >= HOT_RELAY_TRIAC_OFF_TEMP )
    {
        gu16_overheat_timer++;

        if(gu16_overheat_timer >= 3)
        {
            gu16_overheat_timer = 3;
            bit_overheat = SET;
        }
        else{}
    }
    else
    {
        gu16_overheat_timer = 0;

        if( bit_overheat == SET )
        {
            /*..hui [20-9-23���� 1:17:39] ������ ���?..*/
            if( gu8_Hot_Out_Temperature_One_Degree <= HOT_RELAY_TRIAC_OFF_RETURN_TEMP
                && gu8_Hot_Heater_Temperature_One_Degree <= HOT_RELAY_TRIAC_OFF_RETURN_TEMP )
            {
                bit_overheat = CLEAR;
            }
            else{}
        }
        else{}
    }

    mu8_return = (U8)bit_overheat;

    return mu8_return;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U8 triac_duty_oout(void)
{
    U8 mu8_return = CLEAR;

    gu8CntTriacDuty++;

    if(gu8CntTriacDuty <= gu8TriacLevelMax)
    {
        mu8_return = SET;
    }
    else
    {
        mu8_return = CLEAR;
    }

    if(gu8CntTriacDuty >= STEP_TRIAC)
    {
        gu8CntTriacDuty = 0;
    }
    else{}

    return mu8_return;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/





