/******************************************************************************
 ** File Name:      adm_dp.h                                                *
 ** Author:         Benjamin.Wang                                             *
 ** DATE:           12/18/2005                                                *
 ** Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of Audio Device Manager's 
 **                      Data Processor.*
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 12/17/2005     Benjamin.Wang     Create.                                  *
 ******************************************************************************/

 /*! \file adm_dp.h
  *  \author Benjamin Wang
  *  \date Dec 18, 2005
  *  \brief This file defines the basic operation interfaces of Audio Device Manager's Data 
  *           Processor..
  */

#ifndef _ADM_DP_H
#define _ADM_DP_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"
#include "audio_api.h"

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
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_DP_InitRingBuffer(
//!    uint32 uiTrackNum, 
//!    uint32* puiBufferStartAddr, 
//!    uint32 uiBufferSize, 
//!    BOOLEAN bStereoOn
//!    )
//! \param uiTrackNum Track number.
//! \param puiBufferStartAddr Track's buffer address.
//! \param uiBufferSize Track's buffer size.
//! \param bStereoOn Is this track a stereo track? SCI_TRUE: Stereo;SCI_FALSE:MONO
//! \return None.
//! \brief  Description: This function is used to initialize the track buffer.
//! \author Author: Benjamin.Wang
//! \note   Note: None.      
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_DP_InitRingBuffer(
    uint32 uiTrackNum, 
    uint32* puiBufferStartAddr, 
    uint32 uiBufferSize, 
    BOOLEAN bStereoOn
    );

/*****************************************************************************/
//! \fn PUBLIC void AUDIO_DP_WriteRingBuffer(
//!    uint32 uiTrackNum, 
//!    int16* pusLeftData, 
//!    int16* pusRightData, 
//!    uint32 uiCount
//!    )
//! \param uiTrackNum Track number.
//! \param pusLeftData Left channel data.
//! \param pusRightData Right channel data.
//! \param uiCount Number of data.
//! \return None.
//! \brief  Description: This function is used to write data into track buffer and decide 
//!                             whether this track can be turned on.
//! \author Author: Benjamin.Wang
//! \note   Note: If your data is mono data, you should send the same data to both left 
//!                    and right channel.      
/*****************************************************************************/
PUBLIC void AUDIO_DP_WriteRingBuffer(
    uint32 uiTrackNum, 
    int16* pusLeftData, 
    int16* pusRightData, 
    uint32 uiCount
    );

/*****************************************************************************/
//! \fn PUBLIC void AUDIO_DP_WaitBufferEmpty(
//!    uint32 uiTrackNum
//!    )
//! \param uiTrackNum Track number.
//! \return None.
//! \brief  Description: After calling this function, task will be suspend until buffer is empty.
//! \author Author: Benjamin.Wang
//! \note   Note: None.      
/*****************************************************************************/
PUBLIC void AUDIO_DP_WaitBufferEmpty(
    uint32 uiTrackNum
    );
/*****************************************************************************/
//! \fn PUBLIC uint32 AUDIO_DP_DecodeEnd(
//!    uint32 uiTrackNum 
//!    )
//! \param uiTrackNum Track number.
//! \return The operation result. \sa uint32
//! \brief  Description: This function is used to get the length of the data
//!           decode but not output.
//! \author Author: xin.xu1
//! \note   Note: None.      
/*****************************************************************************/
PUBLIC void AUDIO_DP_DecodeEnd(
    uint32 uiTrackNum
    );

/*****************************************************************************/
//! \fn PUBLIC uint32 AUDIO_DP_GetDecoderDataLen(
//!    uint32 uiTrackNum 
//!    )
//! \param uiTrackNum Track number.
//! \return The operation result. \sa uint32
//! \brief  Description: This function is used to get the length of the data
//!           decode but not output.
//! \author Author: Benjamin.Wang
//! \note   Note: None.      
/*****************************************************************************/
PUBLIC uint32 AUDIO_DP_GetDecoderDataLen(
    uint32 uiTrackNum
    );
    
/*****************************************************************************/
//! \fn PUBLIC void AUDIO_DP_ResetRingBuffer(
//!    uint32 uiTrackNum
//!    )
//! \param uiTrackNum Track number.
//! \return None.
//! \brief  Description: This function is used to reset the track buffer pointer.
//! \author Author: Benjamin.Wang
//! \note   Note: None.      
/*****************************************************************************/
PUBLIC void AUDIO_DP_ResetRingBuffer(
    uint32 uiTrackNum
    );
    
/*****************************************************************************/
//! \fn PUBLIC void AUDIO_DP_ResetRingBufferData(
//!    uint32 uiTrackNum
//!    )
//! \param uiTrackNum Track number.
//! \return None.
//! \brief  Description: This function is used to reset the track buffer data pointer.
//! \author Author: Benjamin.Wang
//! \note   Note: None.      
/*****************************************************************************/
PUBLIC void AUDIO_DP_ResetRingBufferData(
    uint32 uiTrackNum
    );    

/*****************************************************************************/
//! PUBLIC uint32  AUDIO_DP_GetTrackBufferSize(uint32 uiTrackNum)
//! \param uiTrackNum Track number.
//! \return TrackBufferSize.
//! \brief  Description: This function is used to get the buffer size of the track
//! \author Author: peter
//! \note   Note: None.      
/*****************************************************************************/

PUBLIC uint32  AUDIO_DP_GetTrackBufferSize(uint32 uiTrackNum);

/*****************************************************************************/
//! PUBLIC AUDIO_RESULT_E  AUDIO_DP_WritePcmForSPEC(
//!    uint32 uiTrackNum, 
//!   int16* pusLeftData, 
//!    int16* pusRightData, 
//!    uint32 uiCount)
//! \param uiTrackNum Track number.
//! \param pusLeftData Left channel data.
//! \param pusRightData Right channel data.
//! \param uiCount Number of data.
//! \return AUDIO_RESULT_E.
//! \brief  Description: This function is used to write pcm data to trackbuffer 
//     for SPEC expression processed. 
//! \author Author: TH
//! \note   Note: None.      
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E  AUDIO_DP_WritePcmForSPEC(
    uint32 uiTrackNum, 
    int16* pusLeftData, 
    int16* pusRightData, 
    uint32 uiCount);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/    
#ifdef __cplusplus
}
#endif

#endif  // _ADM_DP_H

// End of adm_dp.h


