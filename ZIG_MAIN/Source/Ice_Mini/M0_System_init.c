/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : System_init.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#include    "Macrodriver.h"
#include    "Global_Variable.h"
#include    "Port_Define.h"
#include    "M0_System_init.h"
#include    "model_select.h"
/////#include "Wifi/WifiUser/Western_ICE/WIFI_MonitorFixRAM.h"


void System_ini(void);
void Ram_Init(void);
void Variable_Init(void);
void system_reset(void);


bit F_PowerOn;

bit F_System_Init_Finish;
extern ICETRAY_STATE icetray_state_current;
extern ICETRAY_STATE icetray_state_target;
extern bit F_First_Hot_Effluent;
extern void run_init_flow(void);
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void System_ini(void)
{
    DI();

    /*..hui [21-9-8���� 1:57:51] FOTA ����.. �������� �̵�..*/
    R_Systeminit();

    Ram_Init();

    //ADC_Start();

    R_TAU0_Channel0_Start();   /* timer 250us */

    R_TAU0_Channel1_Start();   /* triac timer 8333us */
    R_TAU0_Channel2_Start();   /* timer 500us */

    /*R_TAU1_Channel0_Start();*/   /* multi masetr pwm - drain / hot pump */
    R_TAU1_Channel2_Start(); /* master pwm - drain pump */

    R_UART0_Start();           /* Front */
    R_UART1_Start();           /* Wifi */
    R_UART2_Start();           /* BLDC Comp */
    R_UART3_Start();           /* Line Test, Pc */

    /*..hui [24-11-13���� 4:36:12] TDS ����..*/
    /*R_INTC4_Start();*/           /* TDS In */
    /*R_INTC7_Start();*/           /* Flow Sensor - Filter */
    R_INTC11_Start();          /* Flow Sensor */

    EI();

    Variable_Init();
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void Ram_Init(void)
{
    /* ram initialize */
    U32 ram_addr = 0;
    U8 *p_ram_addr = 0;



    for(ram_addr = 0xFAF00; ram_addr < 0xFFE00; ram_addr++)
    {
        p_ram_addr = (U8 *)ram_addr;
        *p_ram_addr = 0;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void Variable_Init(void)
{
    /*Delay_MS( 500 );*/
    /*EEPROM_PRIMARY();*/

    /*Delay_MS( 500 );*/

    Delay_MS( 1000 );

    InitRtc();

    Delay_MS( 50 );

    WifiControlProcess(WIFI_TIME_INI);      ////////////////////////////////

    EEPROM_PRIMARY();

    WifiControlProcess(WIFI_TIME_SET);      ////////////////////////////////

    gu16_IceSelect_StepMotor = STEP_ANGLE_SELECT;       // INNER DOOR CLOSE
    gu16_Ice_Door_StepMotor = STEP_ANGLE_DOOR;          // ICE DOOR CLOSE
    gu16CompOffDelay = COMP_START_TIME;                 // COMP STABLE TIME

    F_Safety_Routine = 0;
    gu8_IceHeaterControl = 0;
    //F_TrayMotorUP = 0;                                // Tray ����
    //F_TrayMotorDOWN = 0;                               //
    F_IceInit = 1;                                    // ���� �������� ������
    F_IR = SET;                                       // �������� ����
    F_Low_IR = SET;

    InitGasSwitch();

    /*..hui [19-8-1���� 8:49:18] ����Ʈ ���? ����..*/
    F_Front_Tx_Request = SET;

    off_all_control_led();

    init_before_save_data();

    /*..hui [23-2-15���� 11:10:25] ��ȯ�����? ����Ʈ ON..*/
    F_Circul_Drain = SET;

    bit_first_drain = SET;
    run_init_flow();


    gu8_animation_time = BREATH_ANIMATION_TIME;
    gu8_animation_duty = BREATH_ANIMATION_DUTY;

    u32ControlErrors = 0;

    gu8_altutude_setting_timeout = 30;

    gu8_uart_test_mode = NON_UART_TEST;

    bit_temporary_no_operation = CLEAR;

    gu8_wifi_water_select = u8WaterOutState;
    u8IceOutState = ICE_SELECT__NONE;

    gu8_durable_test_start = CLEAR;

    /*..hui [23-11-8���� 3:48:54] ���� ��ô�� OFF���? ����..*/
    bit_periodic_ster_enable = SET;


    bit_self_test_start = CLEAR;

    initial_self_data();
    /////init_self_test_auto();

    gu16_wifi_hot_target_time_min = 0;

    /* ������ ����Ʈ ���� */
    gu8_ice_amount_step = ICE_LEVEL_1_STEP;

    Voice_Initialize();

    /*..hui [24-11-28���� 9:47:30] UV ���� �׽�Ʈ ����..*/
    bit_uv_fault_test_start = SET;

    gu8_hk16_bright = DIMMING_SET_DEFAULT;

    F_First_Hot_Effluent = SET;
	icetray_state_target = ICETRAY_STATE_ICETHROW;

    init_ice_ster();

    // U16HotTemplSelect = HOT_TEMP_SELECT_DEFAULT_45_70_85_100;

	gu8_hot_default_temp = HOT_SET_TEMP____100oC ;

    my_setting[MY_INDEX_RAMEN].temp = 100;
    my_setting[MY_INDEX_RAMEN].amount = 550;
    my_setting[MY_INDEX_RAMEN].use = 1;

    my_setting[MY_INDEX_DRIPCOFFEE].temp = 100;
    my_setting[MY_INDEX_DRIPCOFFEE].amount = 160;
    my_setting[MY_INDEX_DRIPCOFFEE].use = 1;

    my_setting[MY_INDEX_TEA].temp = 100;
    my_setting[MY_INDEX_TEA].amount = 90;
    my_setting[MY_INDEX_TEA].use = 1;

    #ifdef __DUMMY_PROGRAM__
    my_setting[MY_INDEX_MY1].temp = 100;
    my_setting[MY_INDEX_MY1].amount = 380;
    my_setting[MY_INDEX_MY1].use = 1;

    my_setting[MY_INDEX_MY2].temp = 6;
    my_setting[MY_INDEX_MY2].amount = 620;
    my_setting[MY_INDEX_MY2].use = 1;

    my_setting[MY_INDEX_MY3].temp = 100;
    my_setting[MY_INDEX_MY3].amount = 90;
    my_setting[MY_INDEX_MY3].use = 1;
    #endif

    // �¿��� FND ���
    left_normal_state_percent = DIMMING_FND_LEFT_NORMAL_STATE;
    right_normal_state_percent = DIMMING_FND_RIGHT_NORMAL_STATE;

    // ������� �� ��� (��ɺ�, �ϴ� ������)
    setting_mode_function_main_percent = DIMMING_SETTING_MODE_FUNCTION_STATE_MAIN;      // �̻��
    setting_mode_function_extra_percent = DIMMING_SETTING_MODE_FUNCTION_STATE_EXTRA;

    // ��ɺ� (������ ~ MY) �Ϲݻ��� ���
    funtion_led_percent = DIMMING_FUNTION_LED_STATE;

    // ������� (�ڹ���, ����ũ�� ��)
    setting_led_percent = DIMMING_SETTING_LED_STATE;

    // ���� �� ����� ���
    water_extract_led_percent = DIMMING_EXTRACT_LED_STATE;

    // ���� ���� ����� ���
    ice_extract_outer_led_percent = DIMMING_ICE_EXTRACT_OUTER_LED_STATE;
    ice_extract_inner_led_percent = DIMMING_ICE_EXTRACT_INNER_LED_STATE;

    // ���� BAR ���
    bar_led_percent = DIMMING_BAR_LED_STATE;

    // ���̿��� ���� ���
    receipe_led_percent = DIMMING_RECEIPE_LED_STATE;

    // ���»��, UV��� (ū �۲�) ���� ���
    big_ster_led_percent = DIMMING_BIG_STER_LED_STATE;

    // UV (���� �۲�) ���� ���
    small_ster_led_percent = DIMMING_SMALL_STER_LED_STATE;

    // �帳Ŀ�� ���
    receipe_led_dripcoffee_percent = DIMMING_RECEIPE_LED_DRIPCOFFEE_STATE;

    // [�������], [��ü���] ǥ��
    setting_led_side_percent = DIMMING_SETTING_LED_SIDE_STATE;

    // [��ħ��� ������], [:], [��������,����], [WIFI] ��
    top_small_led_percent = DIMMING_TOP_SMALL_LED_STATE;

    // ['C] ������
    middle_small_led_percent = DIMMING_MIDDLE_SMALL_LED_STATE;

    colon_dot_led_percent = DIMMING_COLON_SEG_DOT_STATE;

    welcome_left_led_percent = DIMMING_WELCOME_LEFT_STATE;
    welcome_right_led_percent = DIMMING_WELCOME_RIGHT_STATE;

    ice_type_led_percent = DIMMING_ICE_TYPE_STATE;

    /* 2025-10-28 CH.PARK ���彺��ġ �߰� ���Ǵ� */
    ModelInit();
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void system_reset(void)
{
    DI();

    while(1)
    {
        ;
    }
}





