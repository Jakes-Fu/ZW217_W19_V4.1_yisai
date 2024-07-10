/******************************************************************************
 ** File Name:      pcm_adp.h                                                 *
 ** Author:         Michael chen                                              *
 ** Date:           12/07/2005                                                *
 ** Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    adaptation layer for PCM playback.                        *
*******************************************************************************
*******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                               *
 ** 12/07/2005     Michael.Chen     Create.                                   *
*******************************************************************************/
#ifndef _PCM_ADP_H_
#define _PCM_ADP_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "asm.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

typedef struct
{
    uint32 sampleRate;
    uint16 channel;  
    uint16 is_big_endian;   
}PCM_STREAM_HEAD_T;
/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    Register PCM decoder to the audio server.
//  Author:         Michael.Chen
//  Note:           
/*****************************************************************************/
PUBLIC HAUDIOCODEC PCM_RegCodecPlugger(
    HCOLLISION hCollision, 
    AUDIO_CODEC_EXT_CFGINFO_T* tCodecExtCfgInfo
);
#ifdef __cplusplus
    }
#endif

#endif /* _PCM_ADP_H_ */

/*~~~~~~~~~~~~~~~~~~~~END~~~~~~~~~~~~~~~~~~~~*/
