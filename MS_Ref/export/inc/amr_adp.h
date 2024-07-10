/******************************************************************************
 ** File Name:      amr_adp.h                                                *
 ** Author:         Benjamin.Wang                                             *
 ** DATE:           09/05/2005                                                *
 ** Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces prototype of AMR parser    *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 09/05/2005     Benjamin.Wang     Create.     
 ** 04/20/2008     Binggo.Zhou         Modify For Audio Service 2.0  .    *
 ******************************************************************************/

#ifndef _AMR_ADP_H
#define _AMR_ADP_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "apm_codec.h"
#include "asm.h"

#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         MACRO Definations                                     *
 **---------------------------------------------------------------------------*/
#define AMR_DEFAULT_FRAME_LENGTH 32

#define AMR_FT_MASK 0x78
/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
extern const uint8 AMR_HEADER[6];
extern const uint8 g_amr_frame_length_table[16];

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
//notes:
/*
in stream buffer play:
the para in AUDIO_CreateStreamBufferHandle:
    const uint8* pucData,   // the music data pointer
          here set data of amr header. currently only support amr, namely, "#!AMR\n"
    uint32 uiDataLength,    // the music data length
           here set the length of amr header.
*/
/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This Function is used to reg AMR codec pluger. 
//  Author:         Benjamin.Wang
//  Note:           
/*****************************************************************************/
PUBLIC HAUDIOCODEC AMR_RegCodecPlugger(HCOLLISION hCollision, AUDIO_CODEC_EXT_CFGINFO_T* ptCodecExtCfgInfo);
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/    
#ifdef __cplusplus
}
#endif

#endif  // _AMR_H

// End of amr.h

