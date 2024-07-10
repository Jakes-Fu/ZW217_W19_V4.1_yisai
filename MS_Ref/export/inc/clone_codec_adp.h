/******************************************************************************
** File Name:      null_codec_adp.h                                            
** Author:         shujing.dong                                              
** DATE:           11/13/2010                                                
** Copyright:      2010 Spreatrum, Incoporated. All Rights Reserved.         
** Description:    This file defines the basic operation interfaces 
**                 of null codec                     
******************************************************************************

******************************************************************************
**                        Edit History                                       
**  -----------------------------------------------------------------------  
** DATE           NAME             DESCRIPTION                               
** 12/13/2010     shujing.dong     Create.  
******************************************************************************/

 /*! \file null_codec_adp.h
  *  \author shujing.dong    
  *  \date Nov 13, 2010
  *  \brief This file defines the basic operation interfaces of null codec.
  */
  
#ifndef _CLONE_CODEC_ADP_H
#define _CLONE_CODEC_ADP_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

#include "audio_api.h"
#include "asm.h"
/**---------------------------------------------------------------------------*
 **                         MACRO Definations                                 *
 **---------------------------------------------------------------------------*/
//notes:
/*
    codec name: ANSI2UINT16("CLONE_CODEC")
    bVerifyAudioFormat: SCI_FALSE
    uiTrackOnPoint: 160

*/

#define CLONE_CODEC_CLONE_HANDLE_PARA_COUNT 2
typedef struct
{
    HAUDIO    hAudioHandle;
    HAUDIO    hAudioToBeCloned;
} CLONE_CODEC_CLONE_HANDLE_PARA_T;

#define CLONE_CODEC_OUTPUT_DATA_PARA_COUNT 4
typedef struct
{
    HAUDIO    hAudioHandle;
    int16* pLeftChannel;
    int16* pRightChannel;
    uint32 uiSampleCount;
} CLONE_CODEC_OUTPUT_DATA_PARA_T;

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
extern HAUDIOCODEC hCloneCodec;
 
/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This Function is used to reg null codec pluger. 
//  Author:         Shujing.Dong
//  Note:           
/*****************************************************************************/
PUBLIC HAUDIOCODEC CLONE_CODEC_RegCodecPlugger( //return codec handle
    HCOLLISION hCollision,               //collision handle
    AUDIO_CODEC_EXT_CFGINFO_T* ptCodecExtCfgInfo
    );
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/    
#ifdef __cplusplus
}
#endif

#endif  // _AUDIO_API_H

// End of audio_api.h

