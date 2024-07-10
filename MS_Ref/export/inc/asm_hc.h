/******************************************************************************
 ** File Name:      asm_hc.h                                                *
 ** Author:         Benjamin.Wang                                             *
 ** DATE:           12/11/2005                                                *
 ** Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    Audio Service Manager -- Handle Controller*
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 12/11/2005     Benjamin.Wang     Create.                                  *
 ******************************************************************************/

 /*! \file asm_hc.h
  *  \author Benjamin Wang
  *  \date Nov 12, 2005
  *  \brief This file is used to provide the operation interface of audio handle controller.
  */

#ifndef _ASM_HC_H
#define _ASM_HC_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
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
 **                         Macro Definition                                   *
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
//! \fn PUBLIC uint32 AUDIO_SMHC_GenerateHandle(
//!    uint32 uiHandleType, 
//!    uint32 uiSubHandleType
//!    )
//! \param uiHandleType The main handle type. It occupys the 23th bit to the 20th bit of a 
//!                                 32 bits handle.\sa AUDIO_HANDLE_TYPE_T
//! \param uiSubHandleType The main handle type. It occupys the 19th bit to the 16th bit of a 
//!                                       32 bits handle. \sa AUDIO_HANDLE_TYPE_T
//! \return SUCCESS - return a 32 bits handle. \par
//!            FAILURE - return INVALID_SEQ_NUM
//! \brief  Description: This function is used to generate a unique 16 bits sequence number.
//! \author Author: Benjamin.Wang
//! \note   Note: None. 
/*****************************************************************************/
PUBLIC uint32 AUDIO_SMHC_GenerateHandle(
    uint32 uiHandleType, 
    uint32 uiSubHandleType
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_SMHC_VerifyHandle(
//!    uint32 uiHandle, 
//!    uint32 uiHandleType, 
//!    uint32 uiSubHandleType
//!    )
//! \param uiHandle The handle which will be verified.
//! \param uiHandleType The main handle type. It occupys the 23th bit to the 20th bit of a 
//!                                 32 bits handle.\sa AUDIO_HANDLE_TYPE_T
//! \param uiSubHandleType The main handle type. It occupys the 19th bit to the 16th bit of a 
//!                                       32 bits handle. \sa AUDIO_HANDLE_TYPE_T
//! \return the operation result. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to verify the validity of an audio handle.
//! \author Author: Benjamin.Wang
//! \note   Note: None. 
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_SMHC_VerifyHandle(
    uint32 uiHandle, 
    uint32 uiHandleType, 
    uint32 uiSubHandleType
    );

/*****************************************************************************/
//! \fn PUBLIC uint32 AUDIO_SMHC_GetModuleType(
//!    uint32 uiHandle
//!    )
//! \param uiHandle The handle which will be operated.
//! \return SUCCESS - return the module type of the handle.
//!            FAILURE - return INVALID_HANDLE.
//! \brief  Description: This function is used to get the module type of a handle.
//! \author Author: Benjamin.Wang
//! \note   Note: None. 
/*****************************************************************************/
PUBLIC uint32 AUDIO_SMHC_GetModuleType(
    uint32 uiHandle
    );

/*****************************************************************************/
//! \fn PUBLIC uint32 AUDIO_SMHC_GetHandleType(
//!    uint32 uiHandle
//!    )
//! \param uiHandle The handle which will be operated.
//! \return SUCCESS - return the handle type of the handle.
//!            FAILURE - return INVALID_HANDLE.
//! \brief  Description: This function is used to get the handle type of a handle.
//! \author Author: Benjamin.Wang
//! \note   Note: None. 
/*****************************************************************************/
PUBLIC uint32 AUDIO_SMHC_GetHandleType(
    uint32 uiHandle
    );

/*****************************************************************************/
//! \fn PUBLIC uint32 AUDIO_SMHC_GetSubHandleType(
//!    uint32 uiHandle
//!    )
//! \param uiHandle The handle which will be operated.
//! \return SUCCESS - return the subhandle type of the handle. \par
//!            FAILURE - return INVALID_HANDLE.
//! \brief  Description: This function is used to get the subhandle type of a handle.
//! \author Author: Benjamin.Wang
//! \note   Note: None. 
/*****************************************************************************/
PUBLIC uint32 AUDIO_SMHC_GetSubHandleType(
    uint32 uiHandle
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_SMHC_ReleaseHandle(
//!    uint32 uiHandle
//!    )
//! \param uiHandle The handle which will be operated.
//! \return operation results. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to get the subhandle type of a handle.
//! \author Author: Benjamin.Wang
//! \note   Note: None. 
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_SMHC_ReleaseHandle(
    uint32 uiHandle
    );

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/    
#ifdef __cplusplus
}
#endif

#endif  // _ASM_HC_H

// End of asm_hc.h

