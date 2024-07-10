/******************************************************************************
 ** File Name:      aac_lc_adp.h                                             *
 ** Author:         Shujing.Dong                                              *
 ** DATE:           01/23/2007                                                *
 ** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
 ** Description:    This file defines aac dsp general operation interface.    *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 01/23/2007     Shujing.Dong     Create.                                   *
 *****************************************************************************/
#ifndef _AAC_LC_ADP_H
#define _AAC_LC_ADP_H
/**---------------------------------------------------------------------------**
**                         Dependencies                                      **
**---------------------------------------------------------------------------**/
#include "asm.h"

/**---------------------------------------------------------------------------**
**                        Debugging Flag                                     **
**---------------------------------------------------------------------------**/

/**---------------------------------------------------------------------------**
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------**/
#ifdef __cplusplus
extern   "C"
{
#endif
/**---------------------------------------------------------------------------**
 **                         MACRO Definations                                 **
 **---------------------------------------------------------------------------**/
	
	
/**---------------------------------------------------------------------------**
 **                         Data Structures                                   **
 **---------------------------------------------------------------------------**/
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
    uint8   profile;
    uint16  res0;
    AAC_FORMAT_TYPE_E aac_format_type_e;
}AAC_LC_HEADER_T;
/*****************************************************************************
**NOTES: 
**     For AAC stream buffer playback, APP passed the specified aac format info directly
**     The parameter in AUDIO_CreateStreamBufferHandle()
**     const uint8* pucData,   // the music data pointer
**                          //Here is specified that is start address of AAC_LC_HEADER_T 
**     uint32 uiDataLength,    // the music data length
**                         //Here is specified that is sizeof(AAC_LC_HEADER_T)                                  
******************************************************************************/

/**---------------------------------------------------------------------------**
 **                         Global Variables                                  **
 **---------------------------------------------------------------------------**/
	
	
/**---------------------------------------------------------------------------**
 **                         Constant Variables                                **
 **---------------------------------------------------------------------------**/
	
/**---------------------------------------------------------------------------**
 **                          Function Declare                                 **
 **---------------------------------------------------------------------------**/
/*****************************************************************************/
//! \fn PUBLIC HAUDIOCODEC AAC_LC_RegCodecPlugger(
//!    HCOLLISION hCollision,
//!    AUDIO_CODEC_EXT_CFGINFO_T *ptCodecExtCfgInfo
//!    )
//! \param hCollision  the collision handle which will be used.
//! \param ptCodecExtCfgInfo aac lc extend config info
//! \return SUCCESS - return the codec handle created
//!            FAILURE  - return SCI_NULL
//! \brief  Description:  This function is used to create an codec handle.
//! \author Author:  Shujing.Dong 
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC HAUDIOCODEC AAC_LC_RegCodecPlugger( //return hCodecHandle
    HCOLLISION hCollision, //collision handle
    AUDIO_CODEC_EXT_CFGINFO_T *ptCodecExtCfgInfo
    );

/**---------------------------------------------------------------------------**
 **                         Compiler Flag                                     **
 **---------------------------------------------------------------------------**/ 
#ifdef __cplusplus
}
#endif 

#endif //end of _TEST_H

//end of file
