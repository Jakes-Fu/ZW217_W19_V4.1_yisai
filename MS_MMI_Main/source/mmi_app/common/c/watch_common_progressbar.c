/*****************************************************************************
** File Name:        watch_common_progressbar.c                              *
** Author:           shuting.ma                                              *
** Date:             12/07/2021                                              *
** Copyright:                                                                *
** Description:    This file is used to set the percent of progressbar       *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                    NAME               DESCRIPTION                    *
** 12/07/2021           shuting.ma              Create                       *
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "watch_common_progressbar.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                               *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : 设置进度条当前播放值
//  Parameter: [In] progress_handle: 进度条控件ID
//             [In] percent: 进度条当前播放值
//             [Out] None
//             [Return] None
//  Author:shuting.ma
// Note:
/*****************************************************************************/
PUBLIC void WatchCOM_Progressbar_Set_Percent(uint32 percent, MMI_CTRL_ID_T ctrl_id)
{
    CTRLPROGRESSBAR_OBJ_T* progressbar_ptr = (CTRLPROGRESSBAR_OBJ_T*)MMK_GetCtrlPtr(ctrl_id);

    if(PNULL == progressbar_ptr)
    {
        TRACE_APP_COMMON("[WatchCOM_Progressbar] progressbar_ptr is PNULL");
        return;
    }
    if(TRUE == MMK_IsFocusWin(progressbar_ptr->win_id))
    {
        CTRLPROGRESSBAR_SetPos(TRUE, percent, 0, ctrl_id);
    }
}