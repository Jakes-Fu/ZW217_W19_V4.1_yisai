/******************************************************************************
 ** File Name:      audio_hal.h                                               *
 ** Author:         rui.huang                                                 *
 ** DATE:           06/28/2022                                                *
 ** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define the basic hw interfaces of audio device. *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/28/2022     rui.huang        Create.                                   *
 ******************************************************************************/

#ifndef _AUDIO_HAL_H_
#define _AUDIO_HAL_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **------------------------------------------------------------------------- */

#include "sci_types.h"

/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**---------------------------------------------------------------------------*/
#define AUDIO_HAL_DEBUG
#ifdef AUDIO_HAL_DEBUG
#define AUDHAL_DEBUG(A, ...)    SCI_TRACE_LOW("[AUDHAL][%s] " A "\n", __FUNCTION__, ##__VA_ARGS__)
#define AUDHAL_PASSERT          SCI_PASSERT
#else
#define AUDHAL_DEBUG(...)
#define AUDHAL_PASSERT(...)
#endif
#define AUDHAL_TRACE(A, ...)    SCI_TRACE_LOW("[AUDHAL][%s] " A "\n", __FUNCTION__, ##__VA_ARGS__)
#define AUDHAL_ERROR(A, ...)    SCI_TRACE_LOW("[AUDHAL][%s][%d]{E} " A "\n",__FUNCTION__, __LINE__, ##__VA_ARGS__)

#define AUDIO_HAL_NONE      AUDIO_FUN_NONE
#define AUDIO_HAL_PLAYBACK  AUDIO_FUN_PLAYBACK
#define AUDIO_HAL_CAPTURE   AUDIO_FUN_CAPTRUE
#define AUDIO_HAL_TONE      0x80000000 // deal by audio_hal, invalid for codec/dai

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
enum AUDIO_HAL_OC_E{
    AUDIO_CLOSE = 0,
    AUDIO_OPEN
};

/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif //_AUDIO_HAL_H_
// End
