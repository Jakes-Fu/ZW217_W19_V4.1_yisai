/******************************************************************************
 ** File Name:      dra_adp.h                                                 *
 ** Author:          TH                                                   *
 ** DATE:           2010.10.13                                                *
 ** Copyright:      2010 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    Support DRA(digital rise audio) codec for streaming playback.          *
 **                      This file declared the prototype of  the codec entry operation interface  *
 **                      and data structure.   *
 ******************************************************************************

 ******************************************************************************
 **                               Edit History                                *
 ** ------------------------------------------------------------------------- *
 **     DATE          NAME             DESCRIPTION                            *
 **  2010.10.13     TH               Create.                               *
 ******************************************************************************/
/******************************************************************************/
#ifndef _DRA_ADP_H_
#define _DRA_ADP_H_

/**---------------------------------------------------------------------------*
 **                             Dependencies                                  *
 **---------------------------------------------------------------------------*/
#include "os_api.h"
#include "apm_codec.h"
#include "asm.h"
/**---------------------------------------------------------------------------*
 **                            Compiler Flag                                  *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C" 
{
#endif
/**---------------------------------------------------------------------------*
 **                           Macros & Enums                                  *
 **---------------------------------------------------------------------------*/
 #define DRA_DEBUG
#ifdef DRA_DEBUG
#define DRA_TRACE SCI_TRACE_LOW
#else
#define DRA_TRACE
#endif /* DRA_DEBUG */

/**---------------------------------------------------------------------------*
 **                           Data Structures                                 *
 **---------------------------------------------------------------------------*/
/*****************************************************************************
**NOTES: 
**     For DRA stream buffer playback, APP passed the specified dra format info directly
**     The parameter in AUDIO_CreateStreamBufferHandle()
**     const uint8* pucData,   // the music data pointer
**                          //Here is specified that is start address of DRA_CFG_INFO_T 
**     uint32 uiDataLength,    // the music data length
**                         //Here is specified that is sizeof(DRA_CFG_INFO_T)                                  
******************************************************************************/
typedef struct DRA_CFG_INFO
{
    uint32  sample_rate;    
    uint32  channel_num;
}DRA_CFG_INFO_T;

/**---------------------------------------------------------------------------*
 **                             Variables                                     *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//! \fn PUBLIC HAUDIOCODEC DRA_RegCodecPlugger(
//!    HCOLLISION hCollision,
//!    AUDIO_CODEC_EXT_CFGINFO_T *ptCodecExtCfgInfo
//!    )
//! \param hCollision  the collision handle which will be used.
//! \param ptCodecExtCfgInfo DRA extend config info
//! \return SUCCESS - return the codec handle created
//!            FAILURE  - return INVALID_HANDLE
//! \brief  Description:  This function is used to create DRA audio codec handle.
//! \author Author:  TH 
//! \note   Note:  Nothing.  
/*****************************************************************************/
PUBLIC HAUDIOCODEC DRA_RegCodecPlugger( //return hCodecHandle
    HCOLLISION hCollision, //collision handle
    AUDIO_CODEC_EXT_CFGINFO_T * ptCodecExtCfgInfo
    );

/**---------------------------------------------------------------------------*
 **                            Compiler Flag                                  *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

/******************************************************************************/
#endif //end of #ifndef _DRA_ADP_H_

