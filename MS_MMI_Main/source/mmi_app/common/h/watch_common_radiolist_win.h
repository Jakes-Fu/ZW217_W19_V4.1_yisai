/*****************************************************************************
** File Name:  watch_common_radiolist_win.h                                  *
** Author:                                                                   *
** Date:       03/05/2020                                                    *
** Copyright:                                                                *
** Description: Display view content of radia select list window             *
**                                                                           *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/2020        bin.wang1        Create                                    *
******************************************************************************/

#ifndef _WATCH_COMMON_RADIOLIST_WIN_H_
#define _WATCH_COMMON_RADIOLIST_WIN_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/

#include "window_parse.h"
#include "mmk_type.h"
#include "guilistbox.h"
#include "mmi_common.h"


typedef struct
{
    MMI_WIN_ID_T                winId;
    MMI_CTRL_ID_T               listId;
    WATCHCOM_LIST_ITEM__ST*     pListData;
    PROCESSMSG_FUNC             pCallback;
    uint16                      listCurIdx;
    uint16                      listItemNum;
#ifdef ADULT_WATCH_SUPPORT
    MMI_TEXT_ID_T               title_id;
#endif
}WATCHCOM_RADIOLIST_WIN_INFO_ST;

/*****************************************************************************/
//  Description : enter settings OneLabel_Small select list window
//  Parameter: [In] win_info_ptr: win info data
//             [Out] None
//             [Return] None
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
PUBLIC void WatchCOM_RadioListWin_Create( WATCHCOM_RADIOLIST_WIN_INFO_ST* win_info_ptr );
#ifdef ADULT_WATCH_SUPPORT
/*****************************************************************************/
//  Description : enter settings OneLabel_Small select list window
//  Parameter: [In] win_info_ptr: win info data
//             [Out] None
//             [Return] None
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
PUBLIC void AdultWatchCOM_RadioListWin_Create( WATCHCOM_RADIOLIST_WIN_INFO_ST* win_info_ptr );

/*****************************************************************************/
//  Description : creat radiolist right
//  Parameter: [In] win_info_ptr: win info data
//             [Out] None
//             [Return] None
//  Author: miao.xi
//  Note:
/*****************************************************************************/
PUBLIC void AdultWatchCOM_RadioListWin_Right_Create( WATCHCOM_RADIOLIST_WIN_INFO_ST* win_info_ptr );
#endif
#endif

