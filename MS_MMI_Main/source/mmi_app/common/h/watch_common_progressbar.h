/*****************************************************************************
** File Name:      watch_common_progressbar.h                                *
** Author:         shuting.ma                                                *
** Date:           12/07/2021                                                *
** Copyright:      All Rights Reserved.                                      *
** Description:    This file is used to set the percent of progressbar       *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2021      shuting.ma            Create
******************************************************************************/

#ifndef _MMICOM_PROGRESSBAR_H_
#define _MMICOM_PROGRESSBAR_H_

#include "ctrlprogressbar.h"
#include "mmk_type.h"
#include "mmicom_trace.h"

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
PUBLIC void WatchCOM_Progressbar_Set_Percent(uint32 percent, MMI_CTRL_ID_T ctrl_id);

#endif

