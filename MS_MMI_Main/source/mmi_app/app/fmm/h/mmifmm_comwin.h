/*****************************************************************************
** File Name:      mmifmm_comwin.h                                           *
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

#ifndef _MMIFMM_COMWIN_H_
#define _MMIFMM_COMWIN_H_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmipub.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/
typedef struct
{
    MMI_WIN_ID_T win_id;
    MMI_WIN_ID_T win_id2;
}MMIFMM_QUERYWIN_DATA_T;

/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : HandleFmmQueryWin
//  Global resource dependence :                                
//      Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E HandleOverWriteQueryWin(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
/*****************************************************************************/
// Description : open ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIFMM_QueryAddCtrlFunc (
                                             MMI_WIN_ID_T win_id,
                                             MMIFMM_QUERYWIN_DATA_T *query_data
                                             );

/*****************************************************************************/
// Description : close ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIFMM_QueryCloseCtrlFunc (MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : open wait win
//  Global resource dependence : 
//  Author: jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC void MMIFMM_OpenComWaitWin(MMIPUB_HANDLE_FUNC    func);


#endif