/***********************************************************************************************************************
* Version      : BAS25(STEP_UP)
* File Name    : Remote_Comm.c
* Device(s)    : R5F100MG
* Creation Date: 2015/07/31
* Copyright    : Coway_Electronics Engineering Team (DH,Kim)
* Description  :
***********************************************************************************************************************/
#ifndef _VOICE_H_
#define _VOICE_H_

/////////////////// Header File Control Program /////////////////////



extern U8 IsCriticalVoice(U16 addr);




//------
// ���
//------

#define VOICE_VOLUME_LEVEL_MAXIUM              4
#define VOICE_VOLUME_LEVEL_MINIUM              0
#define DEFAULT_VOICE_VOLUME_LEVEL             1





#define NEXT_LAB_FIRST_PLAY                    0
#define NEXT_LAB_SECOND_PLAY                   1






#define MK_COMMAND                             0x8000

/*NXD Command*/
#define VOICE_COMMAND_ADDRESS_0                0xE000


#if 0
// �����ȳ�
//#define VOICE_COMMAND_VOLUME_5  0x001E  /* -6 dB */
#define VOICE_COMMAND_VOLUME_5                 0x001F  /* -7 dB */
#define VOICE_COMMAND_VOLUME_4                 0x0021  /* -9 dB */
#define VOICE_COMMAND_VOLUME_3                 0x0024  /* -12 dB */
#define VOICE_COMMAND_VOLUME_2                 0x0027  /* -15 dB */
#define VOICE_COMMAND_VOLUME_1                 0x002A  /* -18 dB */

// ȿ����
//#define BEEP_COMMAND_VOLUME_5  0x001E  /* -6 dB */
#define BEEP_COMMAND_VOLUME_5                  0x001F  /* -7 dB */
#define BEEP_COMMAND_VOLUME_4                  0x0024  /* -12 dB */
#define BEEP_COMMAND_VOLUME_3                  0x002A  /* -18 dB */
#define BEEP_COMMAND_VOLUME_2                  0x0030  /* -24 dB */
#define BEEP_COMMAND_VOLUME_1                  0x0036  /* -30 dB */
#endif

#if 0
//31 - Volume 9
//33 - Volume 8
//36 - Volume 7
//39 - Volume 6
//42 - Volume 5
//48 - Volume 4
//54 - Volume 3
//57 - Volume 2
//60 - Volume 1
#endif

#if 0
// �����ȳ�
#define VOICE_COMMAND_VOLUME_5                 0x0024  /* 36 - volume 7 */
#define VOICE_COMMAND_VOLUME_4                 0x0027  /* 39 - volume 6 */
#define VOICE_COMMAND_VOLUME_3                 0x002A  /* 42 - volume 5 */
#define VOICE_COMMAND_VOLUME_2                 0x0030  /* 48 - volume 4 */
#define VOICE_COMMAND_VOLUME_1                 0x0036  /* 54 - volume 3 */

// ȿ����
#define BEEP_COMMAND_VOLUME_5                  0x0024  /* 36 - volume 7 */
#define BEEP_COMMAND_VOLUME_4                  0x0027  /* 39 - volume 6 */
#define BEEP_COMMAND_VOLUME_3                  0x002A  /* 42 - volume 5 */
#define BEEP_COMMAND_VOLUME_2                  0x0030  /* 48 - volume 4 */
#define BEEP_COMMAND_VOLUME_1                  0x0036  /* 54 - volume 3 */
#endif

#define VOICE_COMMAND_VOLUME_5                 0x0021  /* 33 - volume 8 */
#define VOICE_COMMAND_VOLUME_4                 0x0024  /* 36 - volume 7 */
#define VOICE_COMMAND_VOLUME_3                 0x0027  /* 39 - volume 6 */
#define VOICE_COMMAND_VOLUME_2                 0x002A  /* 42 - volume 5 */
#define VOICE_COMMAND_VOLUME_1                 0x0030  /* 48 - volume 4 */

// ȿ����
#define BEEP_COMMAND_VOLUME_5                  0x0021  /* 33 - volume 8 */
#define BEEP_COMMAND_VOLUME_4                  0x0024  /* 36 - volume 7 */
#define BEEP_COMMAND_VOLUME_3                  0x0027  /* 39 - volume 6 */
#define BEEP_COMMAND_VOLUME_2                  0x002A  /* 42 - volume 5 */
#define BEEP_COMMAND_VOLUME_1                  0x0030  /* 48 - volume 4 */





#if 0
// �����ȳ�
//#define VOICE_COMMAND_VOLUME_5  0x001E  /* -6 dB */
#define VOICE_COMMAND_VOLUME_5                 0x000F
#define VOICE_COMMAND_VOLUME_4                 0x0019
#define VOICE_COMMAND_VOLUME_3                 0x0023
#define VOICE_COMMAND_VOLUME_2                 0x002D
#define VOICE_COMMAND_VOLUME_1                 0x0037

// ȿ����
//#define BEEP_COMMAND_VOLUME_5  0x001E  /* -6 dB */
#define BEEP_COMMAND_VOLUME_5                  0x0019
#define BEEP_COMMAND_VOLUME_4                  0x0023
#define BEEP_COMMAND_VOLUME_3                  0x002D
#define BEEP_COMMAND_VOLUME_2                  0x0037
#define BEEP_COMMAND_VOLUME_1                  0x0041
#endif


#define VOICE_COMMAND_VOLUME_INITIAL           0xE121     /* -9 dB */
#define VOICE_COMMAND_VOLUME_MINIUM            0xE100
#define VOICE_COMMAND_VOLUME_MAXIUM            0xE157


#define VOICE_COMMAND_CONTROL2                 0xE260 /*operating mode: normal play*/
                                                       /*Repeat: 1 time play*/
#define VOICE_COMMAND_STOP                     0xE210

#define VOICE_COMMAND_CONTROL3                 0xE305 //..Disable, �񵿱�, 18bit, disable..//
#define VOICE_COMMAND_ADDRESS_256              0xE400
#define VOICE_COMMAND_ADDRESS_512              0xE401
//+++++++++++++++++++++++++++++++++++++++
// �߰� (Ȯ��)
#define VOICE_COMMAND_ADDRESS_768              0xE402
#define VOICE_COMMAND_ADDRESS_1024             0xE403
#define VOICE_COMMAND_ADDRESS_1280             0xE404
#define VOICE_COMMAND_ADDRESS_1536             0xE405
#define VOICE_COMMAND_ADDRESS_1792             0xE406
#define VOICE_COMMAND_ADDRESS_2048             0xE407
//+++++++++++++++++++++++++++++++++++++++







extern bit F_PlayVoice;
extern U16 gu16_voice_address;
extern bit F_Beep_Sound;
extern U16 gu16_voice_command;
extern U8 gu8VoiceVolumeLevel;
extern bit bit_sleep_mode_start;






#endif

