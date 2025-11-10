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
#include    "cody_service.h"



void cody_service(void);
void start_cody_takeoff_ice(void);
void cody_takeoff_ice(void);
void cody_water_line_clean(void);
void cody_water_clean_mode_clear(void);
extern void play_melody_setting_on_198( void );
extern void play_melody_setting_off_199( void );

TYPE_BYTE          U8CodyInspectOperationB;
#define            u8CodyInspectOperation                      U8CodyInspectOperationB.byte
#define            Bit0_Cody_Service_Kit_Start                 U8CodyInspectOperationB.Bit.b0
#define            Bit1_Empty_Tank_Start                       U8CodyInspectOperationB.Bit.b1
#define            Bit2_Ice_Tray_Test_Start                    U8CodyInspectOperationB.Bit.b2

U8 gu8_cody_service_step;
U16 gu16_cody_service_timer;

U8 gu8_cody_ok;


U8 gu8_take_off_ice_step;
U8 gu8_take_off_ice_timer;

U8 gu8_cody_take_off_ice_start;
U8 gu8_service_ice_stop_timer;

special_func cody_water_line;
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void cody_service(void)
{
    if( F_FW_Version_Display_Mode == CLEAR )
    {
        gu8_service_ice_stop_timer = 0;
        return;
    }
    else{}

    if( bit_tank_reed_read_finish == CLEAR )
    {
        gu8_service_ice_stop_timer = 0;
        return;
    }
    else{}

    /*..hui [23-3-24오후 3:14:49] 탱크 커버 열리면 코디 청소.. 제빙 정지 및 이후 제빙 안함..*/
    if( F_Tank_Cover_Input == CLEAR )
    {
        if( Bit0_Cody_Service_Kit_Start == CLEAR )
        {
            gu8_service_ice_stop_timer++;
            if( gu8_service_ice_stop_timer >= 30 )
            {
                gu8_service_ice_stop_timer = 30;
                Bit0_Cody_Service_Kit_Start = SET;
                stop_ice_cody_mode();

                play_voice_top_cover_open_2();
            }
            else {  }
        }
        else {  }
    }
    else
    {
        gu8_service_ice_stop_timer = 0;

        if( Bit0_Cody_Service_Kit_Start == SET )
        {
            play_voice_top_cover_close_6();
            Bit0_Cody_Service_Kit_Start = CLEAR;
        }
        else{}
    }
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void start_cody_takeoff_ice(void)
{
    /*if(F_All_Lock == SET)*/
    if( F_All_Lock == SET && F_Child_Lock == SET )
    {
        return;
    }
    else{}

    /*..hui [20-2-20오후 5:55:32] 더미 탈빙 대기 조건 및 제빙 중일경우에만 강제 탈빙 동작..*/
    /*if(F_IceInit == SET || gu8IceStep == STATE_31_MAIN_ICE_MAKING)*/
    if(F_IceInit == SET || gu8IceStep > STATE_0_STANDBY)
    {
        gu8_cody_take_off_ice_start = SET;
        play_melody_setting_on_198();
    }
    else
    {
        play_melody_warning_197();
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void cody_takeoff_ice(void)
{
    if( gu8_cody_take_off_ice_start == CLEAR )
    {
        gu8_take_off_ice_step = 0;
        gu8_take_off_ice_timer = 0;
        return;
    }
    else{}

    switch( gu8_take_off_ice_step )
    {
        case 0:

            /*force_take_off_ice();*/
            stop_ice_cody_mode();
            gu8_take_off_ice_timer = 0;
            gu8_take_off_ice_step++;

            /* 고온살균 시에는 소리 안나오도록 함 (고온살균 하면 두번 소리남) 250729 CH.PARK */
            if(bit_ice_tank_ster_start == CLEAR)
            {
                play_melody_setting_on_198();
            }
            else {  }

        break;

        case 1:

            gu8_take_off_ice_timer++;

            if( gu8_take_off_ice_timer >= 20 )
            {
                gu8_take_off_ice_timer = 0;
                gu8_take_off_ice_step++;
            }
            else{}

        break;

        case 2:

            gu8_take_off_ice_step = 0;
            gu8_cody_take_off_ice_start = CLEAR;

        break;


        default:

            gu8_take_off_ice_step = 0;

        break;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  : 코디 유로살균모드 (온수+정수+용량) Cody Water Line Clean Service
***********************************************************************************************************************/
void cody_water_line_clean(void)
{
    if(cody_water_line.gu8_start == CLEAR)
    {
        cody_water_line.gu8_step = 0;
        cody_water_line.gu16_timer = 0;
        return;
    }
    else {  }

    /* 누수이거나 단수면 즉시 중지 */
    if((Bit3_Leakage_Sensor_Error__E01 == SET)
    || (Bit6_Main_Water_Flow_Block_Error__E09 == SET)
    )
    {
        cody_water_line.gu8_start = CLEAR;
        cody_water_line.gu8_step = 0;
        cody_water_line.gu16_timer = 0;

        Bit3_Animation_Dimming_Ambient_Select_State = CLEAR;
        Bit4_Animation_Dimming_Cold_Select_State = CLEAR;
    }
    else {  }

    switch (cody_water_line.gu8_step)
    {
        case 0:
            // 정수 입출수 밸브 30초 간 OPEN (정수살균)
            cody_water_line.gu16_timer++;
            if(cody_water_line.gu16_timer >= 300)
            {
                cody_water_line.gu16_timer = 0;
                cody_water_line.gu8_step++;
            }
            else {  }
        break;
        
        case 1:
            // 전체 밸브 CLOSE
            cody_water_line.gu16_timer++;
            if(cody_water_line.gu16_timer >= 10)            // 3초 대기
            {
                cody_water_line.gu16_timer = 0;
                cody_water_line.gu8_step++;
            }
            else {  }
        break;
        
        case 2:
            // 냉수 입출수 밸브 4분 간 OPEN (냉수살균)
            cody_water_line.gu16_timer++;
            if(cody_water_line.gu16_timer >= 2400)
            {
                cody_water_line.gu16_timer = 0;
                cody_water_line.gu8_step++;
            }
            else {  }
        break;
        
        case 3:
            // 전체 밸브 CLOSE
            cody_water_line.gu16_timer++;
            if(cody_water_line.gu16_timer >= 10)            // 3초 대기
            {
                cody_water_line.gu16_timer = 0;
                cody_water_line.gu8_step++;
            }
            else {  }
        break;
        
        case 4:
            // 정수 입출수 밸브 30초 간 OPEN (정수살균)
            cody_water_line.gu16_timer++;
            if(cody_water_line.gu16_timer >= 300)
            {
                cody_water_line.gu16_timer = 0;
                cody_water_line.gu8_step++;
            }
            else {  }
        break;
        
        case 5:
            // 전체 밸브 CLOSE
            cody_water_line.gu16_timer++;
            if(cody_water_line.gu16_timer >= 10)            // 3초 대기
            {
                cody_water_line.gu16_timer = 0;
                cody_water_line.gu8_step++;
                play_melody_setting_off_199();
            }
            else {  }
        break;

        case 6:
            // FINISHD
            cody_water_line.gu8_start = CLEAR;
            cody_water_line.gu8_step = CLEAR;
            cody_water_line.gu16_timer = CLEAR;
            
            Bit3_Animation_Dimming_Ambient_Select_State = CLEAR;
            Bit4_Animation_Dimming_Cold_Select_State = CLEAR;
        break;

        default:
        
        break;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  : 코디 유로살균모드 해제 Cody Water Line Clean Service
***********************************************************************************************************************/
void cody_water_clean_mode_clear(void)
{
    play_melody_setting_off_199();
    cody_water_line.gu8_start = CLEAR;

    Bit3_Animation_Dimming_Ambient_Select_State = CLEAR;
    Bit4_Animation_Dimming_Cold_Select_State = CLEAR;
}

