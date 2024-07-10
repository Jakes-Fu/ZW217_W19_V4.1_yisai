/******************************************************************************
 ** File Name:      arm_tone_adp.h                                            *
 ** Author:         rui.huang                                                 *
 ** DATE:           09/23/2022                                                *
 ** Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE            NAME            DESCRIPTION                               *
 ** 09/23/2022      rui.huang    Create.                                      *
 ******************************************************************************/
#ifndef _ARM_TONE_ADP_H_
#define _ARM_TONE_ADP_H_
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "apm_codec.h"
#include "asm.h"
#include "audio_config.h"
/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                            Data structure                                 *
 **---------------------------------------------------------------------------*/
typedef enum
{
    ARM_TONE_GENERIC,
    ARM_TONE_CUSTOM,
    ARM_TONE_DTMF
}ARM_TONE_TYPE_E;

typedef struct
{
    AUD_GENERIC_TONE_ID_E tone_id;      // Generic tone enumeration value.
    uint32  volume;                     // volume of the tone to be generated. -32768~32767
    uint32  duration;                   // Number of milliseconds of playing this tone.
}ARM_TONE_GENERIC_PARA_T;
typedef struct
{
    uint32  freq1;          // Freq to be played. 0 or 250 <= freq1 <= 3400Hz
    uint32  freq2;          // Freq to be played. 0 or 250 <= freq2 <= 3400Hz
    uint32  volume;         // volume of the tone to be generated. 0~4096, 0 is mute, and 4096 is max
    uint32  duration;       // Number of milliseconds of playing this tone.
}ARM_TONE_CUSTOM_PARA_T;
typedef struct
{
    AUD_DTMF_TONE_ID_E  tone_id;    // Dtmf tone enumeration value.
    uint32  volume;                 // volume of the tone to be generated. 0~4096, 0 is mute, and 4096 is max
    uint32  duration;               // Number of milliseconds of playing this tone.
}ARM_TONE_DTMF_PARA_T;

union Arm_Tone_Para_u
{
    ARM_TONE_GENERIC_PARA_T generic_para;
    ARM_TONE_CUSTOM_PARA_T custom_para;
    ARM_TONE_DTMF_PARA_T dtmf_para;
};

typedef struct
{
    HAUDIO hAudioHandle;
    ARM_TONE_TYPE_E tone_type_e;
    union Arm_Tone_Para_u tone_para_u;
}ARM_TONE_PARA_T;

/**---------------------------------------------------------------------------*
 **                         Const variables                                   *
 **---------------------------------------------------------------------------*/
#define ARM_TONE_PARA_CONFIG_COUNT 2
/**---------------------------------------------------------------------------*
 **                         Global variables                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/

 /*****************************************************************************/
//  Description:    This Function is used to initialize the audio server.
//  Author:         Rui.huang
//  Note:
/*****************************************************************************/
PUBLIC HAUDIOCODEC ARM_TONE_RegCodecPlugger( //return codec handle
    HCOLLISION hCollision,               //collision handle
    AUDIO_CODEC_EXT_CFGINFO_T* ptCodecExtCfgInfo
    );
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
