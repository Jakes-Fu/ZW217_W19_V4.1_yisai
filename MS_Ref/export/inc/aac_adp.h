/******************************************************************************
 ** File Name:      aac_adp.h                                                *
 ** Author:         Shujing.Dong                                             *
 ** DATE:           11/28/2005                                                *
 ** Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces and data structure of 
 **                      AAC Codec adapter layer.*
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 **  11/28/2005    Shujing.Dong     Create.                                  *
 ******************************************************************************/
 /*! \file aac_adp.h
  *  \author Shujing.Dong 
  *  \date Nov 28, 2005
  *  \brief This file defines the basic operation interfaces and data structure of AAC Codec adapter layer. 
  */
  
#ifndef _AAC_ADP_H
#define _AAC_ADP_H


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"
#include "apm_codec.h"
#include "asm.h"

#ifndef AAC_ASSERT
#define AAC_ASSERT SCI_ASSERT  /*assert verified*/
#endif
#ifndef AAC_PASSERT
#define AAC_PASSERT SCI_PASSERT  /*assert verified*/
#endif

#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         MACRO Definations                                     *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
typedef enum
{
    AAC_FORMAT_TYPE_ADTS,
    AAC_FORMAT_TYPE_ADIF,
    AAC_FORMAT_TYPE_LATM,
    AAC_FORMAT_TYPE_UNKNOW
}AAC_FORMAT_TYPE_E;

typedef struct
{
    uint32  sample_rate;    
    uint8   channel_count;
    AAC_FORMAT_TYPE_E aac_format_type_e;
}MPEG4_DATA_HEAD_T;

/*****************************************************************************
**NOTES: 
**     For AAC stream buffer playback, APP passed the specified aac format info directly
**     The parameter in AUDIO_CreateStreamBufferHandle()
**     const uint8* pucData,   // the music data pointer
**                          //Here is specified that is start address of MPEG4_DATA_HEAD_T 
**     uint32 uiDataLength,    // the music data length
**                         //Here is specified that is sizeof(MPEG4_DATA_HEAD_T)                                  
******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//! \fn PUBLIC HAUDIOCODEC AAC_RegCodecPlugger(
//!    HCOLLISION hCollision,
//!    AUDIO_CODEC_EXT_CFGINFO_T *ptCodecExtCfgInfo
//!    )
//! \param hCollision  the collision handle which will be used.
//! \param ptCodecExtCfgInfo aac extend config info
//! \return SUCCESS - return the codec handle created
//!            FAILURE  - return SCI_NULL
//! \brief  Description:  This function is used to create an codec handle.
//! \author Author:  Shujing.Dong 
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC HAUDIOCODEC AAC_RegCodecPlugger( //return hCodecHandle
    HCOLLISION hCollision, //collision handle
    AUDIO_CODEC_EXT_CFGINFO_T * ptCodecExtCfgInfo
    );

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/    
#ifdef __cplusplus
}
#endif

#endif  // _AAC_ADP_H

// End of aac_adp.h

