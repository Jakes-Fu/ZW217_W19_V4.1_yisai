/*****************************************************************************
** File Name:      mmifmm_deletewin.h                                        *
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

#ifndef _MMIFMM_DELETEWIN_H_
#define _MMIFMM_DELETEWIN_H_


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
//  Description : del file menu function
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_DeleteSelectFile(
                            MMIFMM_DATA_INFO_T  *list_data_ptr,
                            MMI_CTRL_ID_T       ctrl_id,
                            MMIFMM_PATH_INFO_T  *current_path_ptr
                            );

/*****************************************************************************/
// Description : enter delete file win
// Global resource dependence : 
// Author:  jian.ma
// RETRUN: 
// Note:   
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIFMM_EnterDeleteWin(
                                          MMIFMM_DATA_INFO_T  *list_data_ptr,
                                          MMIFMM_PATH_INFO_T  *current_path_ptr,
                                          wchar* cur_pathname
                                          );

/*****************************************************************************/
//  Description : get del state
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIFMM_GetDelState(void);

#endif