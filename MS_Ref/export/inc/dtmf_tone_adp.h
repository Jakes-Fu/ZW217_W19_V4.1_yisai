/******************************************************************************
 ** File Name:      dtmf_tone_adp.h                                       *
 ** Author:         Shujing.Dong                                             *
 ** DATE:           12/13/2005                                                *
 ** Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE            NAME            DESCRIPTION                               *
 ** 12/13/2005      Shujing.Dong    Create.                                   *
 ** 04/30/2008      Binggo.Zhou   Modify for Audio Service 2.0.                                   *
 ******************************************************************************/
#ifndef _DTMF_TONE_ADP_H_
#define _DTMF_TONE_ADP_H_
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
typedef struct
{
	HAUDIO              hAudioHandle;
    AUD_DTMF_TONE_ID_E  tone_id;  	// Dtmf tone enumeration value.
    uint32  volume;   // volume of the tone to be generated. 0~4096, 0 is mute, and 4096 is max
    uint32  			duration;   // Number of milliseconds of playing this tone.
    BOOLEAN 			is_uplink;   // If uplink this tone.(SCI_TRUE: Uplink)
}DTMF_TONE_ADP_EXT_OPE_PLAY_PARA_T;
/**---------------------------------------------------------------------------*
 **                         Const variables                                   *
 **---------------------------------------------------------------------------*/
#define DTMF_TONE_ADP_EXT_OPE_PLAY_PARA_COUNT 4
/**---------------------------------------------------------------------------*
 **                         Global variables                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/

 /*****************************************************************************/
//  Description:    This Function is used to initialize the audio server.
//  Author:         Shujing.Dong
//  Note:
/*****************************************************************************/
PUBLIC HAUDIOCODEC DTMF_TONE_RegCodecPlugger( //return codec handle
    HCOLLISION hCollision,               //collision handle
    AUDIO_CODEC_EXT_CFGINFO_T* ptCodecExtCfgInfo
    );
BOOLEAN audevPlayVoiceTone(uint8 ToneType,uint16 Duration);
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
