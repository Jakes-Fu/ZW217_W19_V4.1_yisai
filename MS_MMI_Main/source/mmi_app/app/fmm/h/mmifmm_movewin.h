/*****************************************************************************
** File Name:      mmifmm_movewin.h                                          *
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

#ifndef _MMIFMM_MOVEWIN_H_
#define _MMIFMM_MOVEWIN_H_


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
//  Description : set target path name
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_SetTargetPathName(uint16 current_index,MMIFMM_PATH_INFO_T *path_info);

/*****************************************************************************/
//  Description : copy is from move
//  Global resource dependence : none
//  Author: juan.wu
//  Note:如果从move呼叫copy,则为TRUE
/*****************************************************************************/
PUBLIC BOOLEAN MMIFMM_IsCopyFromMove(void);

/*****************************************************************************/
//  Description : set move index
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
//PUBLIC void MMIFMM_SetMoveIndex(uint16 move_index);

/*****************************************************************************/
// Description : preview current video file
// Global resource dependence : 
// Author:  murphy.xie
// RETRUN: 
// Note:   
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIFMM_EnterMoveWin(
                                        MMIFMM_DATA_INFO_T* list_data_ptr,
                                        MMIFMM_PATH_INFO_T* current_path_ptr
                                        );

#endif