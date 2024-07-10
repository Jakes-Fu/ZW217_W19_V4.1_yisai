/*****************************************************************************
** File Name:   watch_common_radiolist_win.c                                 *
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

/*---------------------------------------------------------------------------*/
/*                         Include Files                                     */
/*---------------------------------------------------------------------------*/
#include "window_parse.h"
#include "guilistbox.h"
#include "mmi_common.h"
#include "mmicom_trace.h"

#include "watch_common_list.h"
#include "watch_common_radiolist_win.h"

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
#ifdef ADULT_WATCH_SUPPORT
LOCAL void CommonRadioListWinCreateWinTitleCtrl( MMI_HANDLE_T win_handle ,MMI_TEXT_ID_T	title_id)
{
    BOOLEAN result =FALSE;

    if(title_id == 0)
    {
        return;
    }
    result = GUIWIN_CreateTitleDyna( win_handle, title_id);
    TRACE_APP_COMMON("result=%d",result);
}
#endif

LOCAL void CommonRadioListWinCreateSoftkeyCtrl( MMI_HANDLE_T win_handle )
{
#ifndef TOUCH_PANEL_SUPPORT
    BOOLEAN result = GUIWIN_CreateSoftkeyDyn( win_handle, STXT_OK, TXT_NULL, STXT_RETURN );
#endif
}

LOCAL void CommonRadioListWinCreateListBoxCtrl( MMI_HANDLE_T win_handle, MMI_CTRL_ID_T ctrl_id )
{
    GUILIST_INIT_DATA_T     initData = { 0 };
    MMI_CONTROL_CREATE_T    create = { 0 };

    initData.both_rect = MMITHEME_GetWinClientBothRect(win_handle);
    initData.type      = GUILIST_RADIOLIST_E;

    create.ctrl_id           = ctrl_id;
    create.guid              = SPRD_GUI_LIST_ID;
    create.parent_win_handle = win_handle;
    create.init_data_ptr     = &initData;

    MMK_CreateControl( &create );
    CTRLLIST_ChangeRadioListDefSoftkeyID( ctrl_id, STXT_OK, STXT_RETURN );
}

LOCAL void Common_RadioListWin_OPEN_WINDOW( WATCHCOM_RADIOLIST_WIN_INFO_ST* win_info_ptr )
{
    if( PNULL == win_info_ptr )
    {
        TRACE_APP_COMMON("win_info_ptr is PNULL!");
        return;
    }
    WatchCOM_RadioList_Create( win_info_ptr->pListData, win_info_ptr->listItemNum, win_info_ptr->listId );

    //set selected item
    GUILIST_SetSelectedItem( win_info_ptr->listId, win_info_ptr->listCurIdx, TRUE );

    //set current item
    GUILIST_SetCurItemIndex( win_info_ptr->listId, win_info_ptr->listCurIdx );

    MMK_SetAtvCtrl( win_info_ptr->winId, win_info_ptr->listId );
}

LOCAL MMI_RESULT_E Common_RadioListWin_User_Action( WATCHCOM_RADIOLIST_WIN_INFO_ST* win_info_ptr, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
    MMI_RESULT_E            retCode = MMI_RESULT_TRUE;

    if( PNULL == win_info_ptr )
    {
        TRACE_APP_COMMON("win_info_ptr is PNULL!");
        return retCode;
    }
    if( PNULL != win_info_ptr->pCallback )
    {
        retCode = win_info_ptr->pCallback( win_info_ptr->winId, msg_id, param );
    }
    //PNULL == win_info_ptr->pCallback user not need callback return TRUE
    return retCode;
}

LOCAL MMI_RESULT_E HandleMsgCommonRadioListWindow( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
    MMI_RESULT_E                    retCode = MMI_RESULT_TRUE;
    WATCHCOM_RADIOLIST_WIN_INFO_ST*      pWinInfo = PNULL;

    pWinInfo = (WATCHCOM_RADIOLIST_WIN_INFO_ST*)MMK_GetWinAddDataPtr( win_id );

    if( PNULL == pWinInfo )
    {
        TRACE_APP_SETTINGS("The pListInfo is PNULL!");
        return retCode;
    }
    // common win proccess msg or need user process firt msg
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            Common_RadioListWin_OPEN_WINDOW( pWinInfo );
            break;
        }
        default:
        {
            retCode = MMI_RESULT_FALSE;
            break;
        }
    }
    // common win not process then user process msg
    if( MMI_RESULT_FALSE == retCode )
    {
        retCode = Common_RadioListWin_User_Action( pWinInfo, msg_id, param );
    }
    return retCode;
}

/*****************************************************************************/
//  Description : enter settings OneLabel_Small select list window
//  Parameter: [In] win_info_ptr: win info data
//             [Out] None
//             [Return] None
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
PUBLIC void WatchCOM_RadioListWin_Create( WATCHCOM_RADIOLIST_WIN_INFO_ST* win_info_ptr )
{
    if( PNULL == win_info_ptr )
    {
        TRACE_APP_COMMON("win_info_ptr is PNULL!");
        return;
    }
    if( !MMK_IsOpenWin(win_info_ptr->winId) )
    {
        MMI_WINDOW_CREATE_T win_create = { 0 };

        win_create.applet_handle    = MMK_GetFirstAppletHandle();
        win_create.win_id           = win_info_ptr->winId;
        win_create.func             = HandleMsgCommonRadioListWindow;
        win_create.win_priority     = WIN_ONE_LEVEL;
        win_create.window_style     = WS_NO_DEFAULT_STYLE;
        win_create.add_data_ptr     = win_info_ptr;
        MMK_CreateWindow( &win_create );


        //UI layout
        //soft key
        CommonRadioListWinCreateSoftkeyCtrl( win_info_ptr->winId );
        //radio list
        CommonRadioListWinCreateListBoxCtrl( win_info_ptr->winId, win_info_ptr->listId );
    }
}

#ifdef ADULT_WATCH_SUPPORT
LOCAL void Common_RadioListWin_OPEN_WINDOW_AdultWatch( WATCHCOM_RADIOLIST_WIN_INFO_ST* win_info_ptr )
{
    uint16          index = 0;
    MMI_STRING_T    itemStr = { 0 };

    if( PNULL == win_info_ptr )
    {
        TRACE_APP_COMMON("win_info_ptr is PNULL!");
        return;
    }
    AdultWatchCOM_RadioList_Create( win_info_ptr->pListData, win_info_ptr->listItemNum, win_info_ptr->listId );

    //set selected item
    GUILIST_SetSelectedItem( win_info_ptr->listId, win_info_ptr->listCurIdx, TRUE );

    //set current item
    GUILIST_SetCurItemIndex( win_info_ptr->listId, win_info_ptr->listCurIdx );

    MMK_SetAtvCtrl( win_info_ptr->winId, win_info_ptr->listId );
}

LOCAL void Common_RadioListWin_Right_OPEN_WINDOW_AdultWatch( WATCHCOM_RADIOLIST_WIN_INFO_ST* win_info_ptr )
{
	uint16          index = 0;
	MMI_STRING_T    itemStr = { 0 };

	if( PNULL == win_info_ptr )
	{
		TRACE_APP_COMMON("win_info_ptr is PNULL!");
		return;
	}
	AdultWatchCOM_RadioList_Right_Create( win_info_ptr->pListData, win_info_ptr->listItemNum, win_info_ptr->listId );

	//set selected item
	GUILIST_SetSelectedItem( win_info_ptr->listId, win_info_ptr->listCurIdx, TRUE );

	//set current item
	GUILIST_SetCurItemIndex( win_info_ptr->listId, win_info_ptr->listCurIdx );

	MMK_SetAtvCtrl( win_info_ptr->winId, win_info_ptr->listId );
}
LOCAL MMI_RESULT_E HandleMsgCommonRadioListWindow_AdultWatch( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
    MMI_RESULT_E                    retCode = MMI_RESULT_TRUE;
    WATCHCOM_RADIOLIST_WIN_INFO_ST*      pWinInfo = PNULL;

    pWinInfo = (WATCHCOM_RADIOLIST_WIN_INFO_ST*)MMK_GetWinAddDataPtr( win_id );

    if( PNULL == pWinInfo )
    {
        TRACE_APP_SETTINGS("The pListInfo is PNULL!");
        return retCode;
    }
    // common win proccess msg or need user process firt msg
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            Common_RadioListWin_OPEN_WINDOW_AdultWatch( pWinInfo );
            break;
        }
        default:
        {
            retCode = MMI_RESULT_FALSE;
            break;
        }
    }
    // common win not process then user process msg
    if( MMI_RESULT_FALSE == retCode )
    {
        retCode = Common_RadioListWin_User_Action( pWinInfo, msg_id, param );
    }
    return retCode;
}

/*****************************************************************************/
//  Description : enter settings OneLabel_Small select list window
//  Parameter: [In] win_info_ptr: win info data
//             [Out] None
//             [Return] None
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
PUBLIC void AdultWatchCOM_RadioListWin_Create( WATCHCOM_RADIOLIST_WIN_INFO_ST* win_info_ptr )
{
    if( PNULL == win_info_ptr )
    {
        TRACE_APP_COMMON("win_info_ptr is PNULL!");
        return;
    }
    if( !MMK_IsOpenWin(win_info_ptr->winId) )
    {
        MMI_WINDOW_CREATE_T win_create = { 0 };

        win_create.applet_handle    = MMK_GetFirstAppletHandle();
        win_create.win_id           = win_info_ptr->winId;
        win_create.func             = HandleMsgCommonRadioListWindow_AdultWatch;
        win_create.win_priority     = WIN_ONE_LEVEL;
        win_create.window_style     = WS_NO_DEFAULT_STYLE;
        win_create.add_data_ptr     = win_info_ptr;
        MMK_CreateWindow( &win_create );

        //Win Title
        CommonRadioListWinCreateWinTitleCtrl( win_info_ptr->winId, win_info_ptr->title_id);
        //radio list
        CommonRadioListWinCreateListBoxCtrl( win_info_ptr->winId, win_info_ptr->listId );
    }
}

LOCAL MMI_RESULT_E HandleMsgCommonRadioListWindow_Right_AdultWatch( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
	MMI_RESULT_E                    retCode = MMI_RESULT_TRUE;
	WATCHCOM_RADIOLIST_WIN_INFO_ST*      pWinInfo = PNULL;

	pWinInfo = (WATCHCOM_RADIOLIST_WIN_INFO_ST*)MMK_GetWinAddDataPtr( win_id );

	if( PNULL == pWinInfo )
	{
		TRACE_APP_SETTINGS("The pListInfo is PNULL!");
		return retCode;
	}
	// common win proccess msg or need user process firt msg
	switch (msg_id)
	{
	case MSG_OPEN_WINDOW:
		{
			Common_RadioListWin_Right_OPEN_WINDOW_AdultWatch( pWinInfo );
			break;
		}
	default:
		{
			retCode = MMI_RESULT_FALSE;
			break;
		}
	}
	// common win not process then user process msg
	if( MMI_RESULT_FALSE == retCode )
	{
		retCode = Common_RadioListWin_User_Action( pWinInfo, msg_id, param );
	}
	return retCode;
}
/*****************************************************************************/
//  Description : creat radiolist right
//  Parameter: [In] win_info_ptr: win info data
//             [Out] None
//             [Return] None
//  Author: miao.xi
//  Note:
/*****************************************************************************/
PUBLIC void AdultWatchCOM_RadioListWin_Right_Create( WATCHCOM_RADIOLIST_WIN_INFO_ST* win_info_ptr )
{
	if( PNULL == win_info_ptr )
	{
		TRACE_APP_COMMON("win_info_ptr is PNULL!");
		return;
	}
	if( !MMK_IsOpenWin(win_info_ptr->winId) )
	{
		MMI_WINDOW_CREATE_T win_create = { 0 };

		win_create.applet_handle    = MMK_GetFirstAppletHandle();
		win_create.win_id           = win_info_ptr->winId;
		win_create.func             = HandleMsgCommonRadioListWindow_Right_AdultWatch;
		win_create.win_priority     = WIN_ONE_LEVEL;
		win_create.window_style     = WS_NO_DEFAULT_STYLE;
		win_create.add_data_ptr     = win_info_ptr;
		MMK_CreateWindow( &win_create );

		//Win Title
		CommonRadioListWinCreateWinTitleCtrl( win_info_ptr->winId, win_info_ptr->title_id);
		//radio list
		CommonRadioListWinCreateListBoxCtrl( win_info_ptr->winId, win_info_ptr->listId );
	}

}
#endif
