/*****************************************************************************
** File Name:      mmifmm_sd.h                                               *
** Author:                                                                   *
** Date:           07/2012                                                   *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe file manager module         *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/2012        jian.ma          Create                                    *
******************************************************************************/

#ifndef _MMIFMM_SD_H_
#define _MMIFMM_SD_H_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmifmm_export.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : memory format
// Global resource dependence :
// Author:  jian.ma
// RETRUN:
// Note:
/*****************************************************************************/
PUBLIC void MMIFMM_FmmMemoryFormat (uint32 device, MMI_TEXT_ID_T query_text_id);

/*****************************************************************************/
//  Description : stop current operation when sd plug
//  Global resource dependence :
//  Author:robert.lu
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_StopOperaSDPlug (void);

/*****************************************************************************/
// Description : enter rename win
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIFMM_EnterMemCardReNameWin (
    MMIFILE_DEVICE_E device_type// Êý¾Ý
);
#endif