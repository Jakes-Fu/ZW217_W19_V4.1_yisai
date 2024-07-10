/*****************************************************************************
** File Name:      mmifmm_lsearch.h                                          *
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

#ifndef _MMIFMM_LSEARCH_H_
#define _MMIFMM_LSEARCH_H_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmifilearray_export.h"
#include "guilistbox.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
#ifdef SEARCH_SUPPORT
/*****************************************************************************/
//  Description : 文件搜索结果的打开函数
//  Global resource dependence :
//  Author: hermann liu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_OpenMatchedItem (uint32 user_data);

/*****************************************************************************/
// 	Description : 文件搜索结果的显示回调函数
//	Global resource dependence :
//    Author:hermann liu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_SetMatchItemData (
    GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr,
    uint32 user_data
);
/*****************************************************************************/
// 	Description : start search for local search
//	Global resource dependence :
//    Author:hermann liu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_StartLocalSearch (
    MMI_STRING_T  *keyword_str_ptr,
    MMI_WIN_ID_T        notice_win_id,         //如果win_id和msg_id都为0，则采用同步方式搜索文件
    MMI_MESSAGE_ID_E    notice_msg_id
);
/*****************************************************************************/
//  Description : free file search resource
//  Global resource dependence :
//  Author: hermann liu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_FreeLocalSearchResource (void);

/*****************************************************************************/
//  Description : get file search matched num
//  Global resource dependence :
//  Author: hermann liu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIFMM_GetFileSearchMatchedNum (void);
#endif

#endif