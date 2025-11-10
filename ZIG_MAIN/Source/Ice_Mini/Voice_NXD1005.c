/***********************************************************************************************************************/
/***********************************************************************************************************************/
/***********************************************************************************************************************/
/***********************************************************************************************************************/
/***********************************************************************************************************************/
#include    "Macrodriver.h"
#include    "Global_Variable.h"
#include    "Port_Define.h"
#include    "Voice_NXD1005.h"


/////void Initial_Voice_Next_Lab(void);
/////void Voice_Stop_Next_Lab(void);
void ProcessVoice_next_lab(void);
//void voice_stop_next_lab(void);
//void send_volume_next_lab(void);
//void send_data_next_lab(void);

//void SEND_SPI_COMMAND_NEXT_LAB(void);
//void Delay_Next_Lab(U16 time);




U8 gu8PlayVoice_Count;

U8 gu8_play_step_next_lab;

U8 u8test_nextlab;

U16 gu16_voice_delay_timer;

const U16 gu16VoiceVolumeRegList[5] =
{
    VOICE_COMMAND_VOLUME_1,
    VOICE_COMMAND_VOLUME_2,
    VOICE_COMMAND_VOLUME_3,
    VOICE_COMMAND_VOLUME_4,
    VOICE_COMMAND_VOLUME_5
};

const U16 gu16BeepVolumeRegList[5] =
{
    BEEP_COMMAND_VOLUME_1,
    BEEP_COMMAND_VOLUME_2,
    BEEP_COMMAND_VOLUME_3,
    BEEP_COMMAND_VOLUME_4,
    BEEP_COMMAND_VOLUME_5
};



/***********************************************************************************************************************/
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
void ProcessVoice_next_lab(void)
{
}






