/*****************************************************************************
** File Name:      mmifmm_findfilewin.h                                      *
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

#ifndef _MMIFMM_FINDFILEWIN_H_
#define _MMIFMM_FINDFILEWIN_H_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "sci_types.h"
#include "mmifmm_export.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/
typedef struct
{
    uint16              *find_path_ptr;  //查找全路径(目录)
    MMIFMM_DATA_INFO_T  *result_data_ptr;  //查找结果保存位置
    uint16              max_result_file_num; //查找结果保存最大文件数
    uint32              win_id;  //通知窗口
    uint32              msg_id;  //通知消息
} MMIFMM_FINDFILE_WIN_PARAM_T;


/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : enter find file win
// Global resource dependence :
// Author: jian.ma
// RETRUN:
// Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIFMM_EnterFindFileWin (MMIFMM_FINDFILE_WIN_PARAM_T *find_file_p);


#endif