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
#include    "hot_water_error.h"



void hot_water_error_check(void);
void check_error_room_overheat(void);

void check_error_hot_flow_block(void);
void check_error_hot_overheat(void);
void check_error_hot_low_water(void);

U8 gu8_hot_overheat_on_timer;
U8 gu8_hot_overheat_off_timer;


U16 gu16_hot_flow_block_err_chk_timer;
U16 gu16_hot_flow_block_err_clr_timer;

U16 gu16_hot_overheat_err_chk_timer;
U16 gu16_hot_overheat_err_clr_timer;


bit F_Hot_Temp_Much_Difference_State;

U8 gu8_room_overheat_on_timer;
U8 gu8_room_overheat_off_timer;

U16 gu16_hot_low_water_check_timer;
U16 gu16_hot_low_water_clear_timer;

bit bit_hot_overheat;
bit bit_hot_low_water;

U8 gu8_clear_hot_block_error;

bit F_hot_flow_error;           //cbr_test
U16 gu16_hot_low_flow_clear_timer;
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void hot_water_error_check(void)
{
    /*..hui [23-5-17ï¿½ï¿½ï¿½ï¿½ 7:48:26] ï¿½Â¼ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½..*/
    /*check_error_hot_overheat();*/
    /*..sean [25-02-03] ï¿½ï¿½ï¿½ï¿½ Å¸ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½..*/
    /*check_error_room_overheat();*/
	/*.. sean [25-02-03] ï¿½ï¿½ï¿½ï¿½Å¸ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½Ï¿ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½..*/
	check_error_hot_flow_block();       /* ÇÃ·¯½Ì ¿Â¼öÅÊÅ© Ã¤¿ì±â ½Ã FLOW BLOCK °¨Áö */

    // check_error_hot_overheat();      /* ÃÊ°í¿Â¼ö ¶§¹®¿¡ ÀÓ½Ã »èÁ¦ 250227 CH.PARK */

    check_error_hot_low_water();        /* ¿Â¼ö ÃßÃâ ½Ã FLOW BLOCK °¨Áö */
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void check_error_hot_flow_block(void)
{
    U8 u8_check_enable = CLEAR;
    U8 u8_block_error_fix = CLEAR;
    U8 mu8_err_check_time = 0;
    U8 mu8_err_check_hz = 0;

    return;     /* ¹Ì»ç¿ë [2025-07-08 ¿ÀÈÄ 2½Ã38ºÐ] ¿öÅÍ_ÀüÀå_SW Åå¹æ ¸Þ¼¼Áö È®ÀÎ */

    /*..hui [21-8-31ï¿½ï¿½ï¿½ï¿½ 9:36:51] ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½Èµï¿½....*/
    /*..hui [21-8-31ï¿½ï¿½ï¿½ï¿½ 9:37:02] ï¿½ï¿½ï¿½ï¿½ ï¿½Â¼ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿?ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½Èµï¿½..*/
    /*..hui [21-8-31ï¿½ï¿½ï¿½ï¿½ 9:37:14] ï¿½Â¼ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿?ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ Ã¹ï¿½ï¿½ ï¿½å·¹ï¿½Î½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½Ï´Ï±ï¿½ ï¿½×¶ï¿½ Ã¼Å©..*/
    /*..hui [21-8-31ï¿½ï¿½ï¿½ï¿½ 9:37:21] ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½Ç´ï¿½.. ï¿½ï¿½Â¿ ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½..*/
    if( F_First_Hot_Effluent == SET && bit_hot_filling_start == SET
        && (gu8_Hot_Filling_Step >= 3 && gu8_Hot_Filling_Step <= 6) )
    {
        /*..hui [19-8-28ï¿½ï¿½ï¿½ï¿½ 7:20:26] ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½Ã¤ï¿½ï¿½â¶§ï¿½ï¿?1ï¿½ï¿½..*/
        /*..hui [20-2-13ï¿½ï¿½ï¿½ï¿½ 2:44:23] ï¿½Â¼ï¿½ Ã¹ï¿½ï¿½ï¿½ï¿½ 14Hz ï¿½ï¿½ï¿½ï¿½ 1ï¿½ï¿½ ï¿½Ì»ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿?ï¿½Â¼ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿?ï¿½ß»ï¿½..*/
        /*..hui [20-2-13ï¿½ï¿½ï¿½ï¿½ 2:44:44] 5hzï¿½ï¿½ ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½.. ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ 12Hzï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½..*/
        /*..hui [20-4-3ï¿½ï¿½ï¿½ï¿½ 11:12:36] Ã¹ ï¿½ï¿½ ï¿½å·¹ï¿½Î¶ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ 2ï¿½Ê·ï¿½ ï¿½ï¿½ï¿½ï¿½..*/
        u8_check_enable = SET;
        //mu8_err_check_time = 30;
        mu8_err_check_time = 50;
        //mu8_err_check_hz = HOT_FIRST_BLOCKED_FLOW_HZ;
        mu8_err_check_hz = HOT_FIRST_BLOCKED_FLOW_HZ;
    }
    else
    {
        u8_check_enable = CLEAR;
    }


    if( u8_check_enable == SET )
    {
        if( Bit0_Hot_Water_Flow_Block_Error__E08 != SET )
        {
            /*..hui [19-8-28ï¿½ï¿½ï¿½ï¿½ 7:15:10] 5Hz ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿?.*/
            /*if( gu8_Flow_1sec <= HOT_BLOCKED_FLOW_HZ )*/
            if( gu8_Flow_1sec <= mu8_err_check_hz )
            {
                gu16_hot_flow_block_err_chk_timer++;

                /*if(gu16_hot_flow_block_err_chk_timer >= HOT_FLOW_BLOCK_ERROR_CHECK_TIME)*/
                if(gu16_hot_flow_block_err_chk_timer >= mu8_err_check_time)
                {
                    Bit0_Hot_Water_Flow_Block_Error__E08 = SET;
					//¿Â¼ö ´Ü¼ö ¹× ¹è¼ö¿¡·¯µµ À½¼º Ãß°¡ 25-06-12
                    Play_Voice(VOICE_7_CALL_AS_WATER_SUPPLY_ERROR);
                    gu16_hot_flow_block_err_chk_timer = 0;

	                gu16_firstEffluent_hotwater_timer = 0;	// ¡Ú 2025-06-10 Phil
	                F_firstEffluent_hotWater = CLEAR;				// ¡Ú 2025-06-10 Phil
	                F_firstEffluent_hotWaterHeat = CLEAR;
                }
                else{}
            }
            else
            {
                gu16_hot_flow_block_err_chk_timer = 0;
            }
        }
        else
        {
            gu16_hot_flow_block_err_chk_timer = 0;
        }
    }
    else
    {
        gu16_hot_flow_block_err_chk_timer = 0;
    }

    if(Bit0_Hot_Water_Flow_Block_Error__E08 == SET)
    {
        gu16_hot_flow_block_err_clr_timer++;

        if(gu16_hot_flow_block_err_clr_timer >= HOT_FLOW_BLOCK_ERROR_CLEAR_TIME)
        {
            Bit0_Hot_Water_Flow_Block_Error__E08 = CLEAR;
            gu16_hot_flow_block_err_clr_timer = 0;
        }
        else{}
    }
    else
    {
        gu16_hot_flow_block_err_clr_timer = 0;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void check_error_hot_overheat(void)
{
    U8 u8_check_enable = CLEAR;

    if(F_WaterOut == SET && u8WaterOutState == HOT_WATER_SELECT)
    {
        /*..hui [20-12-24ï¿½ï¿½ï¿½ï¿½ 9:57:00] ï¿½ï¿½ï¿½ï¿½ ï¿½ß¿ï¿½ï¿½ï¿½ ï¿½å·¹ï¿½Î¶ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½Ï°ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½Ï´ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ È®ï¿½ï¿½ï¿½Ïµï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½..*/
        if( (gu8_Water_Out_Step == STATE_20_HOT_PRE_HEATING_STATE  && gu8_Preheat_Step >= 3)
            || gu8_Water_Out_Step == STATE_30_EXTRACT_VALVE_ON_STATE
            || gu8_Water_Out_Step == STATE_31_WATER_EXTRACT_STATE )
        {
            u8_check_enable = SET;
        }
        else
        {
            u8_check_enable = CLEAR;
        }
    }
    else
    {
        u8_check_enable = CLEAR;
    }

    if( u8_check_enable == SET )
    {
        if( Bit5_Hot_Heater_OverHeat_Error__E40 != SET )
        {
            if(gu8_Hot_Out_Temperature_One_Degree >= 102
               || gu8_Hot_Heater_Temperature_One_Degree >= 102)
            {
                gu16_hot_overheat_err_chk_timer++;

                if(gu16_hot_overheat_err_chk_timer >= HOT_OVERHEAT_ERROR_CHECK_TIME)
                {
                    Bit5_Hot_Heater_OverHeat_Error__E40 = SET;
                    gu16_hot_overheat_err_chk_timer = 0;

                    /*..hui [20-4-8ï¿½ï¿½ï¿½ï¿½ 7:34:47] ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ß»ï¿½ ï¿½ï¿½ 10ï¿½Êµï¿½ï¿½ï¿½ ï¿½å·¹ï¿½ï¿½ ï¿½ï¿½ï¿? ï¿½ß°ï¿½..*/
                    bit_hot_overheat = SET;
                }
                else{}
            }
            else
            {
                gu16_hot_overheat_err_chk_timer = 0;
            }
        }
        else{}
    }
    else
    {
        gu16_hot_overheat_err_chk_timer = 0;
    }

    if(Bit5_Hot_Heater_OverHeat_Error__E40 == SET)
    {
        gu16_hot_overheat_err_clr_timer++;

        if(gu16_hot_overheat_err_clr_timer >= HOT_OVERHEAT_ERROR_CLEAR_TIME)
        {
            Bit5_Hot_Heater_OverHeat_Error__E40 = CLEAR;
            gu16_hot_overheat_err_clr_timer = 0;
        }
        else{}
    }
    else
    {
        gu16_hot_overheat_err_clr_timer = 0;
    }
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void check_error_hot_low_water(void)
{
    U8 u8_check_enable = CLEAR;
    U8 u8_block_error_fix = CLEAR;
    U8 mu8_err_check_time = 0;
    U8 mu8_err_check_hz = 0;

    if( gu8_flushing_mode == FLUSHING_FILL_HOT_TANK_STATE
        && F_First_Hot_Effluent == SET && bit_hot_filling_start == SET
        && (gu8_Hot_Filling_Step >= 3 && gu8_Hot_Filling_Step <= 6) )
    {
        /*..hui [19-8-28¿ÀÈÄ 7:20:26] È÷ÅÍ ¹°Ã¤¿ì±â¶§´Â 1ÃÊ..*/
        /*..hui [20-2-13¿ÀÈÄ 2:44:23] ¿Â¼ö Ã¹ÀÜÀº 14Hz ÀÌÇÏ 1ÃÊ ÀÌ»ó °¨ÁöÀÏ °æ¿ì ¿Â¼ö¹è¼ö¿¡·¯ ¹ß»ý..*/
        /*..hui [20-2-13¿ÀÈÄ 2:44:44] 5hz´Â ³Ñ ³·´Ù.. ¾ÈÀüº¯À¸·Î ¹°ÀÌ »õµµ 12HzÁ¤µµ ³ª¿È..*/
        /*..hui [20-4-3¿ÀÀü 11:12:36] Ã¹ ÀÜ µå·¹ÀÎ¶§´Â È÷ÅÍ ¾ÈÄÑÁø±î 2ÃÊ·Î º¯°æ..*/
        u8_check_enable = SET;
        mu8_err_check_time = 50;
        mu8_err_check_hz = HOT_FIRST_BLOCKED_FLOW_HZ;
    }
    else if(F_WaterOut == SET && u8WaterOutState == HOT_WATER_SELECT)
    {
        /******************************************************************************************************/
        if( gu8_Water_Out_Step == STATE_20_HOT_PRE_HEATING_STATE
            && (gu8_Preheat_Step >= 1  && gu8_Preheat_Step <= 2)
            && gu8_preheat_max_timer >= 10)
        {
            /*..hui [19-8-29¿ÀÀü 11:12:58] ¿¹¿­ Áß ½ÃÀÛ 1ÃÊ ÀÌÈÄºÎÅÍ °¨Áö..*/
            /*..hui [19-8-28¿ÀÈÄ 7:17:20] ÃÊ±â ¿¹¿­Áß¿¡´Â 1.0ÃÊ..*/
            u8_check_enable = SET;
            mu8_err_check_time = 20;

            if(bit_Holding_Flow_Block_Error__E08) {u8_check_enable = CLEAR;} // ¡Ú2025-06-02 Phil
            /*..hui [20-4-3¿ÀÀü 11:17:19] ¿¹¿­ È÷ÅÍ °¡µ¿ Àü 5hz ÀÌ»ó È®ÀÎ..*/
            mu8_err_check_hz = HOT_BLOCKED_FLOW_HZ;
        }
        else if( gu8_Water_Out_Step == STATE_30_EXTRACT_VALVE_ON_STATE
                 || gu8_Water_Out_Step == STATE_31_WATER_EXTRACT_STATE
                 || gu8_Water_Out_Step == STATE_32_WATER_AIR_VENT_STATE )
        {
            /*..hui [19-8-28¿ÀÈÄ 7:17:29] Á¤»ó ÃßÃâÁß¿¡´Â 1.5ÃÊ..*/
            u8_check_enable = SET;
            mu8_err_check_time = 15;

            /*mu8_err_check_hz = HOT_BLOCKED_FLOW_HZ;*/
            /*..hui [21-11-9¿ÀÀü 9:46:53] Àú¼öÀ§½Ã °ú¿­ ¹æÁö¸¦ À§ÇØ Á¤»ó ÃßÃâÁßÀÏ¶§´Â 0.2lpmÀ¸·Î º¯°æ..*/
            mu8_err_check_hz = HOT_FIRST_BLOCKED_FLOW_HZ;
        }
        else{}
    }
    else if( bit_ice_tank_ster_start == SET )
    {
        if( gu8_ice_ster_mode == STER_MODE_HOT_PREHEAT )
        {
            if( gu8_preheat_ster_step >= 3 )
            {
                u8_check_enable = SET;
                mu8_err_check_time = 20;
                mu8_err_check_hz = HOT_BLOCKED_FLOW_HZ;
            }
            else{}
        }
        else if( gu8_ice_ster_mode == STER_MODE_HOT_INPUT_STATE )
        {
            if( gu8_hot_input_ster_step <= 2 )
            {
                u8_check_enable = SET;
                mu8_err_check_time = 20;
                mu8_err_check_hz = HOT_BLOCKED_FLOW_HZ;
            }
            else{}
        }
        else if( gu8_ice_ster_mode == STER_MODE_DRAIN_PREHEAT )
        {
            if( gu8_drain_preheat_ster_step >= 3 )
            {
                u8_check_enable = SET;
                mu8_err_check_time = 20;
                mu8_err_check_hz = HOT_BLOCKED_FLOW_HZ;
            }
            else{}
        }
        else if( gu8_ice_ster_mode == STER_MODE_DRAIN_INPUT_STATE )
        {
            if( gu8_drain_input_ster_step <= 2 )
            {
                u8_check_enable = SET;
                mu8_err_check_time = 20;
                mu8_err_check_hz = HOT_BLOCKED_FLOW_HZ;
            }
            else{}
        }
        else{}
    }
    else
    {
        u8_check_enable = CLEAR;
    }

    if( u8_check_enable == SET )
    {
        if( Bit0_Hot_Water_Flow_Block_Error__E08 != SET )
        {
            if( gu8_Flow_1sec <= mu8_err_check_hz )
            {
                gu16_hot_low_water_check_timer++;

                if( gu16_hot_low_water_check_timer >= mu8_err_check_time )
                {
                    Bit0_Hot_Water_Flow_Block_Error__E08 = SET;
					//¿Â¼ö ´Ü¼ö ¹× ¹è¼ö¿¡·¯µµ À½¼º Ãß°¡ 25-06-12
					F_hot_flow_error = SET; //cbr_test
					//¿¡·¯°¡ ¸ÕÀú°É·Á¼­ ÃßÃâ Á¤Áö ¾ÈµÇ´Â ¹®Á¦ °³¼±
	            	F_WaterOut = CLEAR;
                    Play_Voice(VOICE_7_CALL_AS_WATER_SUPPLY_ERROR);
                    gu16_hot_low_water_check_timer = 0;
                }
                else{}
            }
            else
            {
                gu16_hot_low_water_check_timer = 0;
				/*..sean [25-07-08] È÷½ºÅ×¸®½Ã½º Àû¿ëÇÏ±â À§ÇØ¼­ ÇØÁ¦ Å¸ÀÌ¸Ó µÒ..*/
            	gu16_hot_low_flow_clear_timer++;
            	if(gu16_hot_low_flow_clear_timer >= mu8_err_check_time)
            	{
                	F_hot_flow_error = CLEAR;   //cbr_test
                	gu16_hot_low_flow_clear_timer = 0;
            	}
				else {}
            }
        }
        else
        {
            gu16_hot_low_water_check_timer = 0;
            //cbr_test
            if( (gu8_Flow_1sec > mu8_err_check_hz) && ( u8WaterOutState == HOT_WATER_SELECT ) )
            {
            	gu16_hot_low_flow_clear_timer++;
            	if(gu16_hot_low_flow_clear_timer >= mu8_err_check_time)
            	{
                	F_hot_flow_error = CLEAR;   //cbr_test
                	gu16_hot_low_flow_clear_timer = 0;
            	}
				else {}
            }//cbr_test
            else {}
        }
    }
    else
    {
        gu16_hot_low_water_check_timer = 0;
    }

    if(gu8_clear_hot_block_error == SET)
    {
        gu8_clear_hot_block_error = CLEAR;
        gu16_hot_low_water_clear_timer = HOT_LOW_WATER0_CLEAR_TIME;
    }
    else{}

    if(Bit0_Hot_Water_Flow_Block_Error__E08 == SET)
    {
        gu16_hot_low_water_clear_timer++;

        if(gu16_hot_low_water_clear_timer >= HOT_LOW_WATER0_CLEAR_TIME)
        {
            Bit0_Hot_Water_Flow_Block_Error__E08 = CLEAR;
            gu16_hot_low_water_clear_timer = 0;
        }
        else{}

        //cbr_test
        if( (gu8_Flow_1sec > mu8_err_check_hz) && ( u8WaterOutState == HOT_WATER_SELECT ) )
        {
        	gu16_hot_low_flow_clear_timer++;
        	if(gu16_hot_low_flow_clear_timer >= mu8_err_check_time)
        	{
            	F_hot_flow_error = CLEAR;   //cbr_test
            	gu16_hot_low_flow_clear_timer = 0;
        	}
			else {}
        }//cbr_test
        else {}
    }
    else
    {
        gu16_hot_low_water_clear_timer = 0;
    }
}


/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void check_error_room_overheat(void)
{
}



