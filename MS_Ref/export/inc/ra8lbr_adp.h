/******************************************************************************
 ** File Name:      ra8lbr_adp.h                                              *
 ** Author:         TH                                                  *
 ** DATE:           2011.02.14                                                *
 ** Copyright:      2011 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:   RealAudio 8 Low Bitrate codec decoder adapter layer header file.    *
 **                                                                           *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                               Edit History                                *
 ** ------------------------------------------------------------------------- *
 **     DATE          NAME             DESCRIPTION                            *
 **  2011.02.14      TH               Create.                               *
 ******************************************************************************/
/******************************************************************************/
#ifndef _RA8LBR_ADP_H_
#define _RA8LBR_ADP_H_

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
#define RA8LBR_DEBUG

#ifdef RA8LBR_DEBUG
#define RA8LBR_TRACE SCI_TRACE_LOW
#else
#define RA8LBR_TRACE
#endif /* RA8LBR_DEBUG */
/**---------------------------------------------------------------------------*
 **                           Data Structures                                 *
 **---------------------------------------------------------------------------*/
/*****************************************************************************
**NOTES: 
**     For RA8LBR stream buffer playback, APP passed the specified ra8lbr format info directly
**     The parameter in AUDIO_CreateStreamBufferHandle()
**     const uint8* pucData,   // the music data pointer
**                          //Here is specified that is start address of RA8LBR_CFG_INFO_T 
**     uint32 uiDataLength,    // the music data length
**                         //Here is specified that is sizeof(RA8LBR_CFG_INFO_T)                                  
******************************************************************************/
typedef struct RA8LBR_CFG_INFO
{
    uint32 sample_num; //number of samples per frame
    uint32 frame_size; 
    uint32  sample_rate;
    uint32 region_num; //Number of consecutive Modulated Lapped Transform coefficients that share the same quantizer value.
    uint32  channel_num; 
    uint32 cplStart; //Starting point for the band of quantization bits.
    uint32 cplQBits; //Number of quantization bits for each band in the joint stereo coupling information.
}RA8LBR_CFG_INFO_T;

/**---------------------------------------------------------------------------*
 **                             Variables                                     *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//! \fn PUBLIC HAUDIOCODEC RA8LBR_RegCodecPlugger(
//!    HCOLLISION hCollision,
//!    AUDIO_CODEC_EXT_CFGINFO_T *ptCodecExtCfgInfo
//!    )
//! \param hCollision  the collision handle which will be used.
//! \param ptCodecExtCfgInfo RA8LBR extend config info
//! \return SUCCESS - return the codec handle created
//!            FAILURE  - return INVALID_HANDLE
//! \brief  Description:  This function is used to create RA8LBR audio codec handle.
//! \author Author:  TH 
//! \note   Note:  Nothing.  
/*****************************************************************************/
PUBLIC HAUDIOCODEC RA8LBR_RegCodecPlugger( //return hCodecHandle
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
#endif //#ifndef _RA8LBR_ADP_H_
