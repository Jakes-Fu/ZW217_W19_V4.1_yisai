/*****************************************************************************
** File Name:      mmibrowser_wintable_snapshot.c                            *
** Author:          li.li                                                    *
** Date:           23/06/2009                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the browser window table    *
*****************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_browser_trc.h"
#include "window_parse.h"
#include "mmibrowser_id.h"
#include "browser_text.h"
#include "guirichtext.h"
#include "mmibrowser_menutable.h"
#include "brw_control_api.h"
#include "browser_image.h"
#include "mmipub.h"
#include "cfl_wchar.h"
#include "mmifmm_export.h"
#include "mmk_type.h"
#include "mmibrowser_export.h"
#include "mmibrowser_setting.h"
#include "cfl_char.h"
#include "guiedit.h"
#include "guiim_base.h"
#include "mmibrowser_wintable.h"
#include "mmicom_string.h"
#include "mmi_appmsg.h"
#include "mmi_default.h"
#include "guiform.h"
#include "guilabel.h"
#include "guidropdownlist.h"
#include "mmisd_export.h"
#include "mmibrowser_func.h"
#include "guisetlist.h"
#include "mmi_resource.h"
#ifdef MMI_PDA_SUPPORT
#include "mmisms_text.h"
#endif /* MMI_PDA_SUPPORT */

#include "mmi_text.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
#define MMIBROWSER_MAX_SNAPSHOT_ITEM_NUM        20
/*因为已存网页还需要建立文件夹保存相应的附件等,
    因此要小于MMIFILE_FULL_PATH_MAX_LEN,以确保已存网页保存的成功率*/
#define MMIBROWSER_SNAPSHOT_FULL_PATH_NAME_LEN       200
#define MMIBROWSER_SNAPSHOT_PATH_LEN            19//strlen("d:\") + strlen(g_brw_route_path)
#define MMIBROWSER_SNAPSHOT_TEMPBUF_LEN         30
#define MMIBROWSER_SNAPSHOT_EXT_NAME            ".html"
#define MMIBROWSER_SNAPSHOT_EXT_NAME_LEN        5

//"D(E):\\browser\\snapshot\\*.html"
#define MMIBROWSER_SNAPSHOT_FILE_NAME_LEN  (MMIBROWSER_SNAPSHOT_FULL_PATH_NAME_LEN - MMIBROWSER_SNAPSHOT_PATH_LEN - 1 - MMIBROWSER_SNAPSHOT_EXT_NAME_LEN)
//LOCAL BOOLEAN s_is_mark_all = TRUE;
const wchar g_brw_route_path[]  =  {'b','r','o','w','s','e','r','\\','s','n','a','p','s','h','o','t','\0'};

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : handle snapshot list window message
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleSnapshotListWinMsg(
                                           MMI_WIN_ID_T	win_id, 
                                           MMI_MESSAGE_ID_E	msg_id, 
                                           DPARAM param
                                           );

/*****************************************************************************/
//  Description : handle snapshot save file window message
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleSnapshotSaveFileWinMsg(
                                           MMI_WIN_ID_T	win_id, 
                                           MMI_MESSAGE_ID_E	msg_id, 
                                           DPARAM param
                                           );

#if 0
/*****************************************************************************/
//  Description : handle snapshot save media file window message
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleSnapshotSaveMediaFileWinMsg(
                                           MMI_WIN_ID_T	win_id, 
                                           MMI_MESSAGE_ID_E	msg_id, 
                                           DPARAM param
                                           );
#endif                                     

/*****************************************************************************/
//  Description : handle snapshot popmenu window message
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleSnapshotPopMenuWinMsg(MMI_WIN_ID_T win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param);

/*****************************************************************************/
//  Description : handle snapshot detail window message
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleSnapshotDetailWinMsg(
                                           MMI_WIN_ID_T	win_id, 
                                           MMI_MESSAGE_ID_E	msg_id, 
                                           DPARAM param
                                           );

/*****************************************************************************/
//  Description : set snapshot save file window
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void BrwSetSnapshotSaveFileWindow(void);

/*****************************************************************************/
//  Description : set the snapshot detail window
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void BrwSetSnapshotDetailWindow(MMI_CTRL_ID_T ctrl_id,BRW_SNAPSHOT_ITEM_INFO_T* item_info_ptr);

/*****************************************************************************/
//  Description : set snapshot save media file window
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void BrwSetSnapshotSaveMediaFileWindow(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : append picture and text item
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void BrwAppendPicTextListItem(
                            MMI_CTRL_ID_T       ctrl_id,
                            MMI_TEXT_ID_T       left_softkey_id,
                            MMI_STRING_T        item_string,
                            MMI_IMAGE_ID_T      image,
                            uint16              pos,
                            BOOLEAN             is_update
                            );

/*****************************************************************************/
//  Description : handle snapshot edit file name window message
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleSnapshotEditFileNameWinMsg(
                                   MMI_WIN_ID_T	win_id, 
                                   MMI_MESSAGE_ID_E	msg_id, 
                                   DPARAM param
                                   );

/*****************************************************************************/
//  Description : snapshot save file callback
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void BrwSnapshotSaveFileCallback(SFS_ERROR_E sfs_error);

/*****************************************************************************/
// 	Description : handle snapshot list mark option
//	Global resource dependence : none
//  Author: li.li
//	Note:
/*****************************************************************************/
LOCAL void BrwHandleSnapshotListMarked(
                               MMI_CTRL_ID_T  list_ctrl_id
                               );

/*****************************************************************************/
// 	Description : handle snapshot list mark cancel option
//	Global resource dependence : none
//  Author: li.li
//	Note:
/*****************************************************************************/
LOCAL void BrwHandleSnapshotListUnmarked(
                                 MMI_CTRL_ID_T    list_ctrl_id
                                 );

/*****************************************************************************/
// 	Description : handle snapshot list mark all option
//	Global resource dependence : none
//  Author: li.li
//	Note:
/*****************************************************************************/
LOCAL void BrwHandleSnapshotListMarkedAll(
                                  MMI_CTRL_ID_T   list_ctrl_id
                                  );

/*****************************************************************************/
// 	Description : handle snapshot list mark all cancel option
//	Global resource dependence : none
//  Author: li.li
//	Note:
/*****************************************************************************/
LOCAL void BrwHandleSnapshotListUnmarkedAll(
                                    MMI_CTRL_ID_T list_ctrl_id
                                    );

/*****************************************************************************/
//  Description : set snapshot option item
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void BrwSetSnapshotOptionItem(
                                MMI_WIN_ID_T win_id
                                );

/*****************************************************************************/
// 	Description : delete selected snapshot
//	Global resource dependence : none
//  Author: li.li
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BrwDeleteSelectedSnapshot(
                             MMI_CTRL_ID_T list_ctrl_id   //in
                             );

/*****************************************************************************/
// 	Description : query is delete snapshot
//	Global resource dependence : none
//  Author: li.li
//	Note:
/*****************************************************************************/
LOCAL void BrwQueryIsDeleteSnapshot(
                             MMI_CTRL_ID_T list_ctrl_id   //in
                             );

/*****************************************************************************/
//  Description : set snapshot detail message
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
//LOCAL void BrwSetFileDetailWindow(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : check if has "."
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN BrwSnapshopHasDot(wchar* file_name_ptr);

/*****************************************************************************/
//  Description : handle save wait window message
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleSaveWaitWinMsg(
                                       MMI_WIN_ID_T win_id, 
                                       MMI_MESSAGE_ID_E msg_id, 
                                       DPARAM param
                                       );

/*****************************************************************************/
//  Description : handle delete snapshot window message
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleDeleteSnapshotWinMsg(
                                       MMI_WIN_ID_T win_id, 
                                       MMI_MESSAGE_ID_E msg_id, 
                                       DPARAM param
                                       );

/*****************************************************************************/
//  Description : handle open wait window message
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleOpenWaitWinMsg(
                                       MMI_WIN_ID_T win_id, 
                                       MMI_MESSAGE_ID_E msg_id, 
                                       DPARAM param
                                       );

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : handle bookmark detail popmenu window message
//  Global resource dependence : 
//  Author: dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandlePageMemoPopUpWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  BrwCreatePageMemoPopUpMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void BrwOpenPageMemoPropertyWin(const BRW_SNAPSHOT_ITEM_INFO_T *item_info_ptr);

/*****************************************************************************/
// Description :
// Global resource dependence : none
// Author:Jiaoyou.wu
// Note:    
/*****************************************************************************/
LOCAL MMI_RESULT_E  BrwHandlePageMemoAlertWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

#endif /* MMI_z_SUPPORT */

/**--------------------------------------------------------------------------*
 **                         WINDOW DEFINITION                              *
 **--------------------------------------------------------------------------*/
WINDOW_TABLE(BRW_SNAPSHOT_LIST_WIN_TAB) = 
{
    WIN_TITLE(TXT_BROWSER_HAS_SAVE_FILE),
    WIN_FUNC((uint32)BrwHandleSnapshotListWinMsg),    
    WIN_ID(MMIBROWSER_SHAPSHOT_LIST_WIN_ID),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_DISABLE_SOFTKEY_TO_TITLE),

    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIBROWSER_PAGEMEMO_FORM_CTRL_ID),
    CHILD_SOFTKEY_CTRL(TXT_MARK_ALL, TXT_DELETE, STXT_CANCEL, MMICOMMON_SOFTKEY_CTRL_ID, MMIBROWSER_PAGEMEMO_FORM_CTRL_ID),
    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, MMIBROWSER_SNAPSHOT_LIST_CTRL_ID, MMIBROWSER_PAGEMEMO_FORM_CTRL_ID),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIBROWSER_SNAPSHOT_LIST_CTRL_ID),
#endif /* MMI_PDA_SUPPORT */
    END_WIN


};
WINDOW_TABLE(BRW_SNAPSHOT_SAVE_FILE_WIN_TAB) = 
{
    WIN_TITLE(TXT_BROWSER_SNAPSHOT_SAVE_FILE),
    WIN_FUNC((uint32)BrwHandleSnapshotSaveFileWinMsg),    
    WIN_ID(MMIBROWSER_SNAPSHOT_SAVE_FILE_WIN_ID),
#ifndef MMI_PDA_SUPPORT
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
#endif /* MMI_PDA_SUPPORT */
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIBROWSER_SNAPSHOT_SAVE_FILE_FORM_CTRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIBROWSER_SNAPSHOT_SAVE_F_FORM1_CTRL_ID,MMIBROWSER_SNAPSHOT_SAVE_FILE_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIBROWSER_SNAPSHOT_SAVE_F_LABEL1_CTRL_ID, MMIBROWSER_SNAPSHOT_SAVE_F_FORM1_CTRL_ID),
            CHILD_EDIT_TEXT_CTRL(TRUE, MMIBROWSER_SNAPSHOT_FILE_NAME_LEN, MMIBROWSER_SNAPSHOT_SAVE_F_EDIT_CTRL_ID, MMIBROWSER_SNAPSHOT_SAVE_F_FORM1_CTRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIBROWSER_SNAPSHOT_SAVE_F_FORM2_CTRL_ID,MMIBROWSER_SNAPSHOT_SAVE_FILE_FORM_CTRL_ID),
                CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIBROWSER_SNAPSHOT_SAVE_F_LABEL2_CTRL_ID, MMIBROWSER_SNAPSHOT_SAVE_F_FORM2_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
                CHILD_SETLIST_CTRL(TRUE,MMIBROWSER_SNAPSHOT_SAVE_FILE_SETLIST_CTRL_ID, MMIBROWSER_SNAPSHOT_SAVE_F_FORM2_CTRL_ID),
#else
                CHILD_DROPDOWNLIST_CTRL(TRUE, MMIBROWSER_SNAPSHOT_SAVE_F_DROPDOWN_CTRL_ID, MMIBROWSER_SNAPSHOT_SAVE_F_FORM2_CTRL_ID),
#endif /* MMI_PDA_SUPPORT */

#ifdef MMI_PDA_SUPPORT
        CHILD_SOFTKEY_CTRL(STXT_OK, TXT_NULL, STXT_RETURN,MMICOMMON_SOFTKEY_CTRL_ID,MMIBROWSER_SNAPSHOT_SAVE_FILE_FORM_CTRL_ID),
#endif
    END_WIN
};

#if 0
WINDOW_TABLE(BRW_SNAPSHOT_SAVE_MEDIAFILE_WIN_TAB) = 
{
    WIN_TITLE(TXT_BROWSER_SNAPSHOT_SAVE_MEDIAFILE),
    WIN_FUNC((uint32)BrwHandleSnapshotSaveMediaFileWinMsg),    
    WIN_ID(MMIBROWSER_SNAPSHOT_SAVE_MEDIAFILE_WIN_ID),
    WIN_SOFTKEY(STXT_OK, TXT_BROWSER_CHANGE, STXT_RETURN),
    CREATE_RICHTEXT_CTRL(MMIBROWSER_SNAPSHOT_SAVE_MEDIAFILE_CTRL_ID),
    
    END_WIN
};
#endif

WINDOW_TABLE( MMIBROWSER_SNAPSHOT_POPMENU_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)BrwHandleSnapshotPopMenuWinMsg),
    WIN_ID(MMIBROWSER_SNAPSHOT_POPMENU_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_HIDE_STATUS,
    CREATE_POPMENU_CTRL(MENU_BRW_SNAPSHOT,MMIBROWSER_SNAPSHOT_CTRL_ID),
    WIN_SOFTKEY(STXT_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};

WINDOW_TABLE(MMIBROWSER_SNAPSHOT_EDIT_FILENAME_WIN_TAB) = 
{
    WIN_TITLE(TXT_BROWSER_SNAPSHOT_FILENAME),
    WIN_FUNC((uint32)BrwHandleSnapshotEditFileNameWinMsg),    
    WIN_ID(MMIBROWSER_SNAPSHOT_EDIT_FILENAME_WIN_ID),
#ifdef MMI_PDA_SUPPORT
        WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif /* MMI_PDA_SUPPORT */
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_TEXT_CTRL(MMIBROWSER_SNAPSHOT_FILE_NAME_LEN,MMIBROWSER_SNAPSHOT_EDIT_FILENAME_CTRL_ID),
    
    END_WIN
};

WINDOW_TABLE(BRW_SNAPSHOT_DETAIL_WIN_TAB) = 
{
    WIN_TITLE(TXT_BROWSER_FILE_DETAIL),
    WIN_FUNC((uint32)BrwHandleSnapshotDetailWinMsg),    
    WIN_ID(MMIBROWSER_SNAPSHOT_DETAIL_WIN_ID),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif /* MMI_PDA_SUPPORT */
    WIN_SOFTKEY(TXT_COMM_OPEN, TXT_NULL, STXT_RETURN),
    CREATE_RICHTEXT_CTRL(MMIBROWSER_SNAPSHOT_DETAIL_CTRL_ID),
    
    END_WIN
};

#ifdef MMI_PDA_SUPPORT

WINDOW_TABLE(MMIBROWSER_PAGEMEMO_POPUP_WIN_TAB) = 
{
    WIN_FUNC((uint32) BrwHandlePageMemoPopUpWinMsg), 
    WIN_ID(MMIBROWSER_PAGEMEMO_POPUP_WIN_ID),
    WIN_HIDE_STATUS,
    END_WIN
};

#endif /* MMI_PDA_SUPPORT */

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
 
/*****************************************************************************/
//  Description : set snapshot option item
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void BrwSetSnapshotOptionItem(
                                MMI_WIN_ID_T win_id
                                )
{
//     uint32                  selected_num = 0;
    MMI_CTRL_ID_T           ctrl_id = MMIBROWSER_SNAPSHOT_LIST_CTRL_ID;
    uint16                  cur_index   = 0;
    const GUILIST_ITEM_T*   item_ptr = PNULL;
    uint16                  mark_num = 0;
    uint16                  all_list_num = GUILIST_GetTotalItemNum(ctrl_id);

    cur_index = GUILIST_GetCurItemIndex(ctrl_id);

    //get item pointer
    item_ptr = GUILIST_GetItemPtrByIndex(ctrl_id,cur_index);
    if (PNULL == item_ptr)
    {
        //SCI_TRACE_LOW:"\n MMIBROWSER get item error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_SNAPSHOT_434_112_2_18_2_6_45_324,(uint8*)"");
        return;
    }

    //current item is mark
    if (GUILIST_GetItemState(item_ptr,GUIITEM_STATE_SELECTED))
    {
        GUIMENU_SetItemGrayed(MMIBROWSER_SNAPSHOT_CTRL_ID,MENU_BRW_SNAPSHOT_MARK,ID_POPUP_MENU_SNAPSHOT_MARKED,TRUE);
        GUIMENU_SetItemGrayed(MMIBROWSER_SNAPSHOT_CTRL_ID,MENU_BRW_SNAPSHOT_MARK,ID_POPUP_MENU_SNAPSHOT_UNMARKED,FALSE);
    }
    else
    {
        GUIMENU_SetItemGrayed(MMIBROWSER_SNAPSHOT_CTRL_ID,MENU_BRW_SNAPSHOT_MARK,ID_POPUP_MENU_SNAPSHOT_MARKED,FALSE);
        GUIMENU_SetItemGrayed(MMIBROWSER_SNAPSHOT_CTRL_ID,MENU_BRW_SNAPSHOT_MARK,ID_POPUP_MENU_SNAPSHOT_UNMARKED,TRUE);
    }

    //get mark num
    mark_num = GUILIST_GetSelectedItemIndex(ctrl_id,PNULL,0);
    if (0 < mark_num)
    {
        if (mark_num < all_list_num)
        {
            GUIMENU_SetItemGrayed(MMIBROWSER_SNAPSHOT_CTRL_ID,MENU_BRW_SNAPSHOT_MARK,ID_POPUP_MENU_SNAPSHOT_MARKED_ALL,FALSE);
            GUIMENU_SetItemGrayed(MMIBROWSER_SNAPSHOT_CTRL_ID,MENU_BRW_SNAPSHOT_MARK,ID_POPUP_MENU_SNAPSHOT_UNMARKED_ALL,FALSE);
        }
        else
        {
            GUIMENU_SetItemGrayed(MMIBROWSER_SNAPSHOT_CTRL_ID,MENU_BRW_SNAPSHOT_MARK,ID_POPUP_MENU_SNAPSHOT_MARKED_ALL,TRUE);
            GUIMENU_SetItemGrayed(MMIBROWSER_SNAPSHOT_CTRL_ID,MENU_BRW_SNAPSHOT_MARK,ID_POPUP_MENU_SNAPSHOT_UNMARKED_ALL,FALSE);
        }
        GUIMENU_SetItemGrayed(MMIBROWSER_SNAPSHOT_CTRL_ID,MENU_BRW_SNAPSHOT,ID_POPUP_MENU_SNAPSHOT_OPEN,TRUE);
        GUIMENU_SetItemGrayed(MMIBROWSER_SNAPSHOT_CTRL_ID,MENU_BRW_SNAPSHOT,ID_POPUP_MENU_SNAPSHOT_DETAIL,TRUE);
        GUIMENU_SetItemGrayed(MMIBROWSER_SNAPSHOT_CTRL_ID,MENU_BRW_SNAPSHOT,ID_POPUP_MENU_SNAPSHOT_RENAME,TRUE);
    }
    else
    {
        GUIMENU_SetItemGrayed(MMIBROWSER_SNAPSHOT_CTRL_ID,MENU_BRW_SNAPSHOT_MARK,ID_POPUP_MENU_SNAPSHOT_MARKED_ALL,FALSE);
        GUIMENU_SetItemGrayed(MMIBROWSER_SNAPSHOT_CTRL_ID,MENU_BRW_SNAPSHOT_MARK,ID_POPUP_MENU_SNAPSHOT_UNMARKED_ALL,TRUE);
    }
}
/*****************************************************************************/
//  Description : append snapshot list window items
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL void BrwAppendListboxitem(MMI_CTRL_ID_T ctrl_id, uint16 item_num)
{
    uint16 i = 0;
    GUILIST_ITEM_T item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};

    if( item_num > 0)
    {
        uint16 cur_index = 0;
        uint16 top_index = 0;

        cur_index = GUILIST_GetCurItemIndex(ctrl_id);
        top_index = GUILIST_GetTopItemIndex(ctrl_id);
        if (GUILIST_GetTotalItemNum(ctrl_id) > 0)
        {
            GUILIST_RemoveAllItems(ctrl_id);
        }
        GUILIST_SetMaxItem(ctrl_id, item_num, TRUE);//dynamic

        item_t.item_style = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
        item_t.item_data_ptr = &item_data;

        for( i = 0; i < item_num; i++ )
        {
            GUILIST_AppendItem( ctrl_id, &item_t );
        }
        GUILIST_SetTopItemIndex(ctrl_id, top_index);
        GUILIST_SetCurItemIndex(ctrl_id, cur_index);
    }
}
/*****************************************************************************/
//  Description : handle snapshot list window message
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleSnapshotListWinMsg(
                                           MMI_WIN_ID_T	win_id, 
                                           MMI_MESSAGE_ID_E	msg_id, 
                                           DPARAM param
                                           )
{
    MMI_RESULT_E                    result = MMI_RESULT_TRUE;
    uint32                          snapshot_num = 0;
    MMI_CTRL_ID_T                   list_ctrl_id = MMIBROWSER_SNAPSHOT_LIST_CTRL_ID;
    const BRW_SNAPSHOT_ITEM_INFO_T* item_info_ptr = PNULL;
    MMI_STRING_T                    name_str={0};
    uint16                          list_index = 0; 
    uint16                          mark_num = 0;
    uint16  total_num = 0;
#ifdef MMI_PDA_SUPPORT
    GUIFORM_CHILD_HEIGHT_T child_height = {0};
#endif /* MMI_PDA_SUPPORT */

    switch(msg_id) 
    {
    case MSG_OPEN_WINDOW:
        snapshot_num = BRW_SnapshotGetCount();
        BrwAppendListboxitem(list_ctrl_id, snapshot_num);

        GUILIST_SetTitleIndexType(list_ctrl_id, GUILIST_TITLE_INDEX_DEFAULT);

#ifdef MMI_PDA_SUPPORT
        GUILIST_PermitBorder(list_ctrl_id, FALSE);
        child_height.type = GUIFORM_CHILD_HEIGHT_LEFT;
        GUILIST_PermitBorder(list_ctrl_id, FALSE);
        GUIFORM_IsSlide(MMIBROWSER_PAGEMEMO_FORM_CTRL_ID, FALSE);
        GUIFORM_SetChildDisplay(MMIBROWSER_PAGEMEMO_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
        GUIFORM_SetChildHeight(MMIBROWSER_PAGEMEMO_FORM_CTRL_ID, list_ctrl_id, &child_height);
        GUIFORM_PermitChildBg(MMIBROWSER_PAGEMEMO_FORM_CTRL_ID, FALSE);
        GUILIST_SetUserBg(list_ctrl_id, FALSE);
        GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, STXT_MARK, FALSE);
        GUIWIN_SetTitleButtonHandleMsgInfo(win_id, GUITITLE_BUTTON_RIGHT, MSG_NOTIFY_TITLE_MENU_ICON, list_ctrl_id);
        if (snapshot_num > 0)
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
        }
#else
        if (0 == snapshot_num)
        {
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
        }
        else
        {
            GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_COMM_OPEN, STXT_RETURN, FALSE);
        }
#endif
        
        MMK_SetAtvCtrl(win_id,list_ctrl_id);
        break;

#ifdef MMI_PDA_SUPPORT
    case MSG_NOTIFY_TITLE_MENU_ICON:
        total_num = GUILIST_GetTotalItemNum(list_ctrl_id);
        if (total_num > 0)
        {
            GUIFORM_CHILD_DISPLAY_E display_type = GUIFORM_CHILD_DISP_NORMAL;

            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
            BrwHandleSnapshotListMarked(list_ctrl_id);
            GUISOFTKEY_SetTextId(win_id, MMICOMMON_SOFTKEY_CTRL_ID, TXT_MARK_ALL, TXT_DELETE, STXT_CANCEL, FALSE);
            GUIFORM_SetChildDisplay(MMIBROWSER_PAGEMEMO_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, display_type);
            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
        }
        break;
#endif

    case MSG_CTL_LIST_NEED_ITEM_DATA:
        {
            GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;

            if( PNULL == need_item_data_ptr )
            {
                //SCI_TRACE_LOW:"BrwHandleSnapshotListWinMsg ::  PNULL == need_item_data_ptr"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_SNAPSHOT_589_112_2_18_2_6_45_325,(uint8*)"");
                break;
            }
            
            item_info_ptr = BRW_SnapshotGetItemInfo(need_item_data_ptr->item_index);
            
            if (PNULL == item_info_ptr)
            {
                //SCI_TRACE_LOW:"\n MMIBROWSER BrwHandleSnapshotListWinMsg BRW_SnapshotGetItemInfo error"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_SNAPSHOT_597_112_2_18_2_6_45_326,(uint8*)"");
                break;
            }
            name_str.wstr_ptr=item_info_ptr->file_name + MMIBROWSER_SNAPSHOT_PATH_LEN + 1;
            name_str.wstr_len=MMIAPICOM_Wstrlen(item_info_ptr->file_name) - MMIBROWSER_SNAPSHOT_PATH_LEN - 1 - MMIBROWSER_SNAPSHOT_EXT_NAME_LEN;
            BrwAppendPicTextListItem(list_ctrl_id,
                STXT_OPTION,
                name_str,
#ifdef MMI_PDA_SUPPORT
                IMAGE_BROWSER_PAGE,
#else
                IMAGE_BROWSER_SNAPSHOT_ICON,
#endif                
                need_item_data_ptr->item_index,
                FALSE);
        }
        break;
        
    case MSG_GET_FOCUS:
        total_num = GUILIST_GetTotalItemNum(list_ctrl_id);
        if (0 == total_num)
        {
#ifdef MMI_PDA_SUPPORT
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
#else
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
#endif        
        }
#ifdef MMI_PDA_SUPPORT
        else
        {
            GUIFORM_CHILD_DISPLAY_E     display_type = GUIFORM_CHILD_DISP_NORMAL;

            GUIFORM_GetChildDisplay(MMIBROWSER_PAGEMEMO_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, &display_type);
            if (GUIFORM_CHILD_DISP_NORMAL == display_type)//in markded status
            {
                GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
            }
            else
            {
                GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
            }
        }
#endif
        break;


    case MSG_CTL_LIST_MARK_ITEM:
        //get mark num
        mark_num = GUILIST_GetSelectedItemIndex(list_ctrl_id,PNULL,0);
        total_num = GUILIST_GetTotalItemNum(list_ctrl_id);

#ifdef MMI_PDA_SUPPORT
        if ((0 < mark_num) && (total_num == mark_num))
        {
            GUISOFTKEY_SetTextId(win_id, MMICOMMON_SOFTKEY_CTRL_ID, TXT_CANCEL_SELECT_ALL, TXT_DELETE, STXT_CANCEL, TRUE);
        }
        else
        {
            GUISOFTKEY_SetTextId(win_id, MMICOMMON_SOFTKEY_CTRL_ID, TXT_MARK_ALL, TXT_DELETE, STXT_CANCEL, TRUE);
        }
#else
        if (0 == mark_num)
        {
            //set list enable mark
            GUILIST_SetTextListMarkable(list_ctrl_id,FALSE);
        }
#endif /* MMI_PDA_SUPPORT */
        break;
        
#ifdef MMI_PDA_SUPPORT
    case MSG_CTL_LIST_LONGOK:
    {
        GUIFORM_CHILD_DISPLAY_E     display_type = GUIFORM_CHILD_DISP_NORMAL;

        GUIFORM_GetChildDisplay(MMIBROWSER_PAGEMEMO_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, &display_type);
        if (GUIFORM_CHILD_DISP_NORMAL == display_type)
        {
            //do nothing
        }
        else//enter opt window
        {
            total_num = GUILIST_GetTotalItemNum(list_ctrl_id);
            if (total_num > 0)
            {
                MMK_CreateWin((uint32*)MMIBROWSER_PAGEMEMO_POPUP_WIN_TAB,PNULL);    
            }
        }
    }
        break;

    case MSG_PROMPTWIN_OK:
        {
            MMI_STRING_T    str = {0};
            MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &str);
            MMIPUB_OpenWaitWin(1, &str, PNULL, PNULL, MMIBROWSER_DELETE_WAIT_WIN_ID, IMAGE_NULL, ANIM_PUBWIN_WAIT, WIN_ONE_LEVEL, MMIPUB_SOFTKEY_NONE, BrwHandleDeleteSnapshotWinMsg);
        }
        
        MMIBROWSER_CloseQuerytWin(PNULL);
        break;

    case MSG_PROMPTWIN_CANCEL:
        MMIBROWSER_CloseQuerytWin(PNULL);
        break;

#endif /* MMI_PDA_SUPPORT */

    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_MENU:
    {
#ifdef MMI_PDA_SUPPORT
        GUIFORM_CHILD_DISPLAY_E     display_type = GUIFORM_CHILD_DISP_NORMAL;

        GUIFORM_GetChildDisplay(MMIBROWSER_PAGEMEMO_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, &display_type);
        if (GUIFORM_CHILD_DISP_NORMAL == display_type)
        {
            //do nothing
        }
        else//enter opt window
#endif        
        {
            total_num = GUILIST_GetTotalItemNum(list_ctrl_id);
            if (total_num > 0)
            {
                MMK_CreateWin((uint32*)MMIBROWSER_SNAPSHOT_POPMENU_WIN_TAB,PNULL);    
            }
        }
    }
        break;
        
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    {
        total_num = GUILIST_GetTotalItemNum(list_ctrl_id);
        
#ifdef MMI_PDA_SUPPORT
        if (PNULL != param)
        {
            MMI_CTRL_ID_T       src_ctrl_id = 0;
            src_ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;
            mark_num = GUILIST_GetSelectedItemIndex(list_ctrl_id,PNULL,0);

            if (src_ctrl_id == MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID)
            {
                if ((0 < mark_num) && (total_num == mark_num))
                {
                    GUISOFTKEY_SetTextId(win_id, MMICOMMON_SOFTKEY_CTRL_ID, TXT_MARK_ALL, TXT_DELETE, STXT_CANCEL, FALSE );
                    GUILIST_SetAllSelected(list_ctrl_id,FALSE);
                }
                else
                {
                    GUISOFTKEY_SetTextId(win_id, MMICOMMON_SOFTKEY_CTRL_ID, TXT_CANCEL_SELECT_ALL, TXT_DELETE, STXT_CANCEL, FALSE );
                    BrwHandleSnapshotListMarkedAll(list_ctrl_id);
                }

                MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
                break;

            }
            else if (src_ctrl_id == MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID)
            {
                //exit the marked status
                BrwHandleSnapshotListUnmarkedAll(list_ctrl_id);
                GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
                GUISOFTKEY_SetTextId(win_id, MMICOMMON_SOFTKEY_CTRL_ID, TXT_MARK_ALL, TXT_DELETE, STXT_CANCEL, FALSE);
                GUIFORM_SetChildDisplay(MMIBROWSER_PAGEMEMO_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
                MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
                break;
            }
            else if (src_ctrl_id == MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID)
            {
                if(0 == mark_num)
                {
                    MMIPUB_OpenAlertWarningWin(TXT_SMS_SELECT_ONT_PROMPT);
                }
                else
                {
                    BrwQueryIsDeleteSnapshot(list_ctrl_id);
                }
                break;
            }
        }
#endif /* MMI_PDA_SUPPORT */
       
        if (0 == total_num)
        {
            break;
        }
        list_index=GUILIST_GetCurItemIndex(MMIBROWSER_SNAPSHOT_LIST_CTRL_ID);
        item_info_ptr = BRW_SnapshotGetItemInfo(list_index);

        if (PNULL != item_info_ptr)
        {
            MMIBROWSER_StartWeb(MMIBROWSER_WEB_ENTRY_SNAPSHOT, PNULL, item_info_ptr->file_name);
        }

        MMK_CloseWin(win_id);
    }
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}

/*****************************************************************************/
//  Description : append picture and text item
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void BrwAppendPicTextListItem(
                            MMI_CTRL_ID_T       ctrl_id,
                            MMI_TEXT_ID_T       left_softkey_id,
                            MMI_STRING_T        item_string,
                            MMI_IMAGE_ID_T      image,
                            uint16              pos,
                            BOOLEAN             is_update
                            )
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
    item_t.item_data_ptr = &item_data;
    
    item_data.softkey_id[0] = left_softkey_id;
    item_data.softkey_id[1] = TXT_COMM_OPEN;
    item_data.softkey_id[2] = STXT_RETURN;
    
    item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[0].item_data.image_id = image;

    item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    
    if(item_string.wstr_len > 0 && item_string.wstr_ptr != PNULL)
    {
        item_data.item_content[1].item_data.text_buffer.wstr_len = item_string.wstr_len;
        item_data.item_content[1].item_data.text_buffer.wstr_ptr = item_string.wstr_ptr;
    }
        
    if( is_update )
    {
        GUILIST_ReplaceItem( ctrl_id, &item_t, pos );
    }
    else
    {
        GUILIST_SetItemData(ctrl_id, &item_data, pos );
    }

}
/*****************************************************************************/
//  Description : handle snapshot save file window message
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleSnapshotSaveFileWinMsg(
                                           MMI_WIN_ID_T	win_id, 
                                           MMI_MESSAGE_ID_E	msg_id, 
                                           DPARAM param
                                           )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch(msg_id) 
    {
    case MSG_OPEN_WINDOW:
        BrwSetSnapshotSaveFileWindow();
        MMK_SetAtvCtrl(win_id, MMIBROWSER_SNAPSHOT_SAVE_FILE_FORM_CTRL_ID);
        break;
        
#ifdef MMI_PDA_SUPPORT
    case MSG_CTL_SETLIST_SWITCH:
        {
            MMIFILE_DEVICE_E    dev = MMI_DEVICE_UDISK;
            uint16 cur_index = 0;
            uint16  i = 0;
            
            cur_index = GUISETLIST_GetCurIndex(MMIBROWSER_SNAPSHOT_SAVE_FILE_SETLIST_CTRL_ID);
            if(cur_index < MMI_DEVICE_NUM)
            {
                for(;dev<MMI_DEVICE_NUM; dev++)
                {
                    if(MMIAPIFMM_GetDeviceTypeStatus(dev))
                    {
                        if (i == cur_index)
                        {
                            MMIBROWSER_SetSaveDeviceType(dev);
                            break;
                        }
                        i++;
                    }
                }
            }
            else
            {
                //error do nothing
            }
        }        
        break;
#endif /* MMI_PDA_SUPPORT */

    case MSG_CTL_OK:
    case MSG_APP_OK:
    {
        wchar save_path[MMIBROWSER_SNAPSHOT_FULL_PATH_NAME_LEN + 1] = {0};
        uint16 save_path_len = MMIBROWSER_SNAPSHOT_FULL_PATH_NAME_LEN;
        MMI_STRING_T  dst_temp1 = {0};
        wchar file_name[MMIBROWSER_SNAPSHOT_FILE_NAME_LEN + 1] = {0};
        MMI_STRING_T wait_str = {0};
        uint32  result_for_check = 0;
        MMIFILE_DEVICE_E dev = MMI_DEVICE_UDISK;
        //get name
        GUIEDIT_GetString(MMIBROWSER_SNAPSHOT_SAVE_F_EDIT_CTRL_ID, &dst_temp1);
        if ((PNULL == dst_temp1.wstr_ptr) || (0 == dst_temp1.wstr_len)) 
        {
            MMIPUB_OpenAlertWarningWin(TXT_BROWSER_SNAPSHOT_FAIL_NO_TITLE);
            break;
        }
        else if (!MMIAPICOM_FileNameIsValid(&dst_temp1) || !MMIAPICOM_GetValidFileName(FALSE, dst_temp1.wstr_ptr,dst_temp1.wstr_len, file_name,MMIBROWSER_SNAPSHOT_FILE_NAME_LEN))//MS00192575:invalid filename
        {
            MMIPUB_OpenAlertWarningWin(TXT_DC_NAMEINVALID);
            break;
        }

        //update to valid file name
        dst_temp1.wstr_ptr = file_name;
        dst_temp1.wstr_len = MMIAPICOM_Wstrlen(file_name);
  
        MMIBROWSER_UpdateSaveFile(dst_temp1);//update the data only for save

#ifndef MMI_PDA_SUPPORT
        //get store place
        if (GUIDROPDOWNLIST_GetTotalItemNum(MMIBROWSER_SNAPSHOT_SAVE_F_DROPDOWN_CTRL_ID) > 1)
        {
            uint16 cur_index = 0;
            uint16  i = 0;
            
            cur_index = GUIDROPDOWNLIST_GetCurItemIndex(MMIBROWSER_SNAPSHOT_SAVE_F_DROPDOWN_CTRL_ID);
            if(cur_index < MMI_DEVICE_NUM)
            {
                for(;dev<MMI_DEVICE_NUM; dev++)
                {
                    if(MMIAPIFMM_GetDeviceTypeStatus(dev))
                    {
                        if (i == cur_index)
                        {
                            MMIBROWSER_SetSaveDeviceType(dev);
                            break;
                        }
                        i++;
                    }
                }
            }
            else
            {
                //error do nothing
            }
        }
#endif /* MMI_PDA_SUPPORT */
        //save
        dev = MMIBROWSER_GetSaveDeviceType();
        if(dev < MMI_DEVICE_NUM)
        {
            MMIAPIFMM_CombineFullPath(MMIAPIFMM_GetDevicePath(dev), 
                               MMIAPIFMM_GetDevicePathLen(dev),
                               g_brw_route_path,
                               MMIAPICOM_Wstrlen(g_brw_route_path),
                               PNULL,
                               0,
                               save_path,
                               &save_path_len
                               );
        }

        result_for_check = BRW_SnapshotCheckIfNameExist(save_path,file_name);
        if (1== result_for_check)/* parasoft-suppress BD-RES-INVFREE "有判断检查" */
        {
            MMIPUB_OpenAlertWarningWin(TXT_FILE_EXISTED);
        }
        else if (2 == result_for_check)/* parasoft-suppress BD-RES-INVFREE "有判断检查" */
        {
            MMIPUB_OpenAlertWarningWin(TXT_FOLDER_EXISTED);
        }
        else if (0 == result_for_check)/* parasoft-suppress BD-RES-INVFREE "有判断检查" */
        {
            uint32  dev_space = 0;
            MMI_TEXT_ID_T   alert_text_id = TXT_CMSBRW_FULL_NO_STORE;
            SFS_ERROR_E sfs_error = SFS_ERROR_NONE;
                
            MMIBROWSER_FreeSaveFile();//无论save成功或失败,都需关闭窗口.故save file的wstr无用处了

            dev_space = MMI_GetFreeSpace(dev);
            if (0 == dev_space)
            {
                sfs_error = SFS_ERROR_NO_SPACE;
            }
            else if (dev >= MMI_DEVICE_NUM)
            {
                sfs_error = SFS_ERROR_INVALID_PARAM;
            }
            else
            {
                sfs_error = BRW_SnapshotSaveCurWebPage(MMIBROWSER_GetInstance(),save_path,file_name,BrwSnapshotSaveFileCallback);
            }

            if (sfs_error != SFS_ERROR_NONE)
            {
                if (SFS_ERROR_NO_SPACE == sfs_error)
                {
                    alert_text_id = TXT_NO_SPACE;
                }

                MMIPUB_OpenAlertWarningWin(alert_text_id);
                MMK_CloseWin(win_id);
            }
            else
            {
                MMI_GetLabelTextByLang(TXT_BROWSER_SAVE_WAIT, &wait_str); 
                MMIPUB_OpenWaitWin(1,&wait_str,PNULL,PNULL,MMIBROWSER_SAVE_WAIT_WIN_ID,IMAGE_NULL,
                    ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,(MMIPUB_HANDLE_FUNC)BrwHandleSaveWaitWinMsg);
            }
        }
        else
        {
            //SCI_TRACE_LOW:"BrwHandleSnapshotSaveFileWinMsg  result_for_check = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_SNAPSHOT_995_112_2_18_2_6_46_327,(uint8*)"d", result_for_check);
        }
    }
        break;

#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
            MMI_CTRL_ID_T       ctrl_id = 0;

            if (PNULL != param)
            {
                ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;

                if (ctrl_id == MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID)
                {
                    MMK_PostMsg(win_id, MSG_CTL_OK, PNULL, 0);
                }
                else if (ctrl_id == MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID)
                {
                    MMK_PostMsg(win_id, MSG_CTL_CANCEL, PNULL, 0);
                }
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527        
#endif /* MMI_PDA_SUPPORT */

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        MMIBROWSER_FreeSaveFile();
        break;       

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}


/*****************************************************************************/
//  Description : handle save wait window message
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleSaveWaitWinMsg(
                                       MMI_WIN_ID_T win_id, 
                                       MMI_MESSAGE_ID_E msg_id, 
                                       DPARAM param
                                       )
{
    MMI_RESULT_E    result   = MMI_RESULT_TRUE;  
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIDEFAULT_AllowOpenKeylocWin(FALSE);
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, PNULL);
        break;
    
    case MSG_GET_FOCUS:
        MMIDEFAULT_AllowOpenKeylocWin(FALSE);//MS00189926
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, PNULL);
        break;

#ifndef MMI_PDA_SUPPORT
    case MSG_APP_RED:
#else
    case MSG_KEYUP_RED:
#endif
        break;      

    case MSG_CLOSE_WINDOW:
        MMIDEFAULT_AllowOpenKeylocWin(TRUE);//MS00189926
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, PNULL);
        break;

    default:
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, PNULL);
        break;
    }
    return result;
}


/*****************************************************************************/
//  Description : check if has '.'
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN BrwSnapshopHasDot(wchar* file_name_ptr)
{
    wchar* wch_ptr = file_name_ptr;
    BOOLEAN has_dot = FALSE;

    while (*wch_ptr != 0)
    {
        if (*wch_ptr == '.')
        {
            has_dot = TRUE;
            break;
        }
        wch_ptr ++;
    }
    return has_dot;
}
/*****************************************************************************/
//  Description : handle snapshot detail window message
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleSnapshotDetailWinMsg(
                                           MMI_WIN_ID_T	win_id, 
                                           MMI_MESSAGE_ID_E	msg_id, 
                                           DPARAM param
                                           )
{
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    BRW_SNAPSHOT_ITEM_INFO_T*   item_info_ptr = PNULL;
    
    switch(msg_id) 
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id,MMIBROWSER_SNAPSHOT_DETAIL_CTRL_ID);
        item_info_ptr = MMK_GetWinAddDataPtr(win_id);
        BrwSetSnapshotDetailWindow(MMIBROWSER_SNAPSHOT_DETAIL_CTRL_ID,item_info_ptr);
        break;
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
        item_info_ptr = MMK_GetWinAddDataPtr(win_id);
        MMIBROWSER_StartWeb(MMIBROWSER_WEB_ENTRY_SNAPSHOT, PNULL, item_info_ptr->file_name);
        MMK_CloseWin(win_id);
        MMK_CloseWin(MMIBROWSER_SHAPSHOT_LIST_WIN_ID);
        break;
        
#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
            MMI_CTRL_ID_T       src_ctrl_id = 0;

            if (PNULL != param)
            {
                src_ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;

                if (src_ctrl_id == MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID)
                {
                    MMK_PostMsg(win_id, MSG_CTL_OK, PNULL, 0);
                }
                else if (src_ctrl_id == MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID)
                {
                    MMK_PostMsg(win_id, MSG_CTL_CANCEL, PNULL, 0);
                }
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527        
#endif /* MMI_PDA_SUPPORT */
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}

/*****************************************************************************/
//  Description : set the snapshot detail window
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void BrwSetSnapshotDetailWindow(MMI_CTRL_ID_T ctrl_id,BRW_SNAPSHOT_ITEM_INFO_T *item_info_ptr)
{
    GUIRICHTEXT_ITEM_T  item_data = {0};
    uint16 index = 0;
    uint16 len = 0;
    char temp_buf[MMIBROWSER_SNAPSHOT_TEMPBUF_LEN +1] = {0};
    uint16 temp_uint16_buf[MMIBROWSER_SNAPSHOT_TEMPBUF_LEN +1] = {0};

    item_data.text_type = GUIRICHTEXT_TEXT_RES;
    item_data.text_data.res.id = TXT_COMM_TITLE;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

    SCI_MEMSET(&item_data, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;
    len = MMIAPICOM_Wstrlen(item_info_ptr->file_name) - MMIBROWSER_SNAPSHOT_PATH_LEN - 1 - MMIBROWSER_SNAPSHOT_EXT_NAME_LEN;
    item_data.text_data.buf.len = MIN(len, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = item_info_ptr->file_name + MMIBROWSER_SNAPSHOT_PATH_LEN+ 1;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
    
    SCI_MEMSET(&item_data, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
    item_data.text_type = GUIRICHTEXT_TEXT_RES;
    item_data.text_data.res.id = TXT_BROWSER_SNAPSHOT_FILE_SIZE;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
    
    SCI_MEMSET(&item_data, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;
    if (item_info_ptr->file_size / MMIBROWSER_ONE_K > 0)
    {
        CFL_Snprintf(temp_buf,sizeof(temp_buf),"%.1f%s",(float)(item_info_ptr->file_size)/MMIBROWSER_ONE_K,"KB");
    }
    else
    {
        CFL_Snprintf(temp_buf,sizeof(temp_buf),"%d%s",item_info_ptr->file_size,"B");
    }
    
    CFLWCHAR_StrNCpyWithAsciiStr(temp_uint16_buf,temp_buf,MMIBROWSER_SNAPSHOT_TEMPBUF_LEN +1);
    item_data.text_data.buf.len = MMIAPICOM_Wstrlen(temp_uint16_buf);
    item_data.text_data.buf.str_ptr = temp_uint16_buf;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

    SCI_MEMSET(&item_data, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
    item_data.text_type = GUIRICHTEXT_TEXT_RES;
    item_data.text_data.res.id = TXT_BROWSER_SNAPSHOT_FILE_SAVE_TIME;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
    
    SCI_MEMSET(&item_data, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;
    //year
    SCI_MEMSET(temp_buf, 0x00, sizeof(temp_buf));
    CFL_Snprintf(temp_buf,sizeof(temp_buf),"%d",item_info_ptr->creat_date.year);
    CFLWCHAR_StrNCpyWithAsciiStr(temp_uint16_buf,temp_buf,MMIBROWSER_SNAPSHOT_TEMPBUF_LEN +1);
    CFLWCHAR_StrNCatWithAsciiStr(temp_uint16_buf,"-",MMIBROWSER_SNAPSHOT_TEMPBUF_LEN +1);
    //month
    SCI_MEMSET(temp_buf, 0x00, sizeof(temp_buf));
    CFL_Snprintf(temp_buf,sizeof(temp_buf),"%d",item_info_ptr->creat_date.mon);
    CFLWCHAR_StrNCatWithAsciiStr(temp_uint16_buf,temp_buf,MMIBROWSER_SNAPSHOT_TEMPBUF_LEN +1);
    CFLWCHAR_StrNCatWithAsciiStr(temp_uint16_buf,"-",MMIBROWSER_SNAPSHOT_TEMPBUF_LEN +1);
    //day
    SCI_MEMSET(temp_buf, 0x00, sizeof(temp_buf));
    CFL_Snprintf(temp_buf,sizeof(temp_buf),"%d",item_info_ptr->creat_date.day);
    CFLWCHAR_StrNCatWithAsciiStr(temp_uint16_buf,temp_buf,MMIBROWSER_SNAPSHOT_TEMPBUF_LEN +1);

    item_data.text_data.buf.len = MMIAPICOM_Wstrlen(temp_uint16_buf);
    item_data.text_data.buf.str_ptr = temp_uint16_buf;
    
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
}

#if 0
/*****************************************************************************/
//  Description : handle snapshot save media file window message
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleSnapshotSaveMediaFileWinMsg(
                                           MMI_WIN_ID_T	win_id, 
                                           MMI_MESSAGE_ID_E	msg_id, 
                                           DPARAM param
                                           )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    uint16 index = 0;
    GUIRICHTEXT_ITEM_T  item_data = {0};
    MMI_STRING_T  dst_temp1 = {0};
    BOOLEAN is_save_to_phone = 0;
    MMI_CTRL_ID_T   ctrl_id = MMIBROWSER_SNAPSHOT_SAVE_MEDIAFILE_CTRL_ID;
    switch(msg_id) 
    {
        case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id,ctrl_id);
        BrwSetSnapshotSaveMediaFileWindow(ctrl_id);
        break;
        
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        //获得聚焦的item索引
        GUIRICHTEXT_GetFocusItemIndex( ctrl_id, &index );
        if (1==index)
        {
            //SCI_TRACE_LOW:"\n BROWSER LILI TEST richtext 1"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_SNAPSHOT_1265_112_2_18_2_6_46_328,(uint8*)"");
        }
        else if (2==index)
        {
            //SCI_TRACE_LOW:"\n BROWSER LILI TEST richtext 2"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_SNAPSHOT_1269_112_2_18_2_6_46_329,(uint8*)"");
        }
        else if (3==index)
        {
            //SCI_TRACE_LOW:"\n BROWSER LILI TEST richtext 3"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_SNAPSHOT_1273_112_2_18_2_6_46_330,(uint8*)"");
        }
        else
        {
            
        }
        break;
    case MSG_CTL_RICHTEXT_SWITCHFOCUS_BY_KEY:
    case MSG_CTL_RICHTEXT_SWITCHFOCUS_BY_TP:
        GUIRICHTEXT_GetFocusItemIndex( ctrl_id, &index);
        if ((3 == index) || (4 == index))
        {
            GUIRICHTEXT_ITEM_T  item_data = {0};
            uint16 index_unselect = 0;

            index_unselect = (3 == index)? 4: 3;
            is_save_to_phone = (3 == index)? TRUE: FALSE;

            GUIRICHTEXT_GetItem(ctrl_id, index, &item_data);
            item_data.img_data.res.id = IMAGE_CONTROL_LIST_RADIO_SELECTED_ICON;
            GUIRICHTEXT_ModifyItem(ctrl_id, index, &item_data);

            SCI_MEMSET(&item_data, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
            GUIRICHTEXT_GetItem(ctrl_id, index_unselect, &item_data);
            item_data.img_data.res.id = IMAGE_CONTROL_LIST_RADIO_UNSELECTED_ICON;
            GUIRICHTEXT_ModifyItem(ctrl_id, index_unselect, &item_data);

            GUIRICHTEXT_Update(ctrl_id);
            //to do :need to set is_save_to_phone for save
        }
        break;
    case MSG_CTL_OK:
    case MSG_APP_OK:
        MMIBROWSER_OpenSnapshotListWin();
        MMK_CloseWin(win_id);
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}
#endif

/*****************************************************************************/
//  Description : open snapshot list window
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_OpenSnapshotListWin(void)
{
    MMIFILE_DEVICE_E fmm_dev = MMIAPIFMM_GetValidateDevice(MMI_DEVICE_SDCARD);

    if (fmm_dev < MMI_DEVICE_NUM)
    {
        MMK_CreateWin((uint32*)BRW_SNAPSHOT_LIST_WIN_TAB,PNULL);
    }
    else
    {
        MMIPUB_OpenAlertWarningWin(TXT_SD_NO_EXIST);
    }
}

/*****************************************************************************/
//  Description : open snapshot save file window
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_OpenSnapshotSaveFileWin(void)
{
    MMK_CreateWin((uint32*)BRW_SNAPSHOT_SAVE_FILE_WIN_TAB,PNULL);
}

#if 0 /* Reduce Code size */
/*****************************************************************************/
//  Description : open snapshot save media file window
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_OpenSnapshotSaveMediaFileWin(void)
{
#if 0
    MMK_CreateWin((uint32*)BRW_SNAPSHOT_SAVE_MEDIAFILE_WIN_TAB,PNULL);
#endif
}
#endif /* Reduce Code size */

/*****************************************************************************/
//  Description : set snapshot save file window
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void BrwSetSnapshotSaveFileWindow(void)
{
    MMI_STRING_T  dst_temp1 = {0};
    uint16 file_title[GUIRICHTEXT_TEXT_MAX_LEN + 1] = {0};
    uint16 title_len = 0;
    uint16 max_title_len = 0;
    MMI_STRING_T  dropdownlist[MMI_DEVICE_NUM]  = {0};
    uint16  index = 0;
    uint16  select_index = 0;
    MMIFILE_DEVICE_E    dev = MMI_DEVICE_UDISK;
    MMIFILE_DEVICE_E    default_dev = MMI_DEVICE_UDISK;
#ifdef MMI_PDA_SUPPORT
    GUISETLIST_ITEM_INFO_T  item_info = {0};
#endif /* MMI_PDA_SUPPORT */

#ifdef MMI_PDA_SUPPORT
    GUIFORM_SetType(MMIBROWSER_SNAPSHOT_SAVE_FILE_FORM_CTRL_ID, GUIFORM_TYPE_TP);
    GUIEDIT_SetAlign(MMIBROWSER_SNAPSHOT_SAVE_F_EDIT_CTRL_ID, ALIGN_LVMIDDLE);
    GUISETLIST_SetFontAlign(MMIBROWSER_SNAPSHOT_SAVE_FILE_SETLIST_CTRL_ID, ALIGN_LEFT);//左对齐
    GUISETLIST_SetTitleTextId(MMIBROWSER_SNAPSHOT_SAVE_FILE_SETLIST_CTRL_ID,STXT_STORE_MEDIUM);
#endif /* MMI_PDA_SUPPORT */

    GUIFORM_SetStyle(MMIBROWSER_SNAPSHOT_SAVE_F_FORM1_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIBROWSER_SNAPSHOT_SAVE_F_FORM2_CTRL_ID,GUIFORM_STYLE_UNIT);

    GUILABEL_SetTextById(MMIBROWSER_SNAPSHOT_SAVE_F_LABEL1_CTRL_ID, TXT_BROWSER_SNAPSHOT_FILENAME, FALSE);
    GUILABEL_SetTextById(MMIBROWSER_SNAPSHOT_SAVE_F_LABEL2_CTRL_ID, STXT_STORE_MEDIUM, FALSE);

    //title
    max_title_len = MIN(MMIBROWSER_SNAPSHOT_FILE_NAME_LEN, GUIRICHTEXT_TEXT_MAX_LEN);/*lint !e506*/
    if (BRW_GetPageTitle(MMIBROWSER_GetInstance(), file_title, &title_len, max_title_len))
    {
        GUIEDIT_SetString(MMIBROWSER_SNAPSHOT_SAVE_F_EDIT_CTRL_ID,file_title,title_len);
    }
    dst_temp1.wstr_len = title_len;
    dst_temp1.wstr_ptr = file_title;
    MMIBROWSER_UpdateSaveFile(dst_temp1);
    default_dev  = MMIAPIFMM_GetFirstValidDevice();
   MMIBROWSER_SetSaveDeviceType(default_dev);//init
    
    if (default_dev < MMI_DEVICE_NUM)//valid
    {
        for(;dev<MMI_DEVICE_NUM; dev++)
        {
            if(MMIAPIFMM_GetDeviceTypeStatus(dev))
            {
                MMI_GetLabelTextByLang(MMIAPIFMM_GetDeviceName(dev), &dropdownlist[index]);
#ifdef MMI_PDA_SUPPORT

                if (dropdownlist[index].wstr_len != 0)
                {
                    item_info.item_state = GUISETLIST_ITEM_STATE_TEXT;
                    item_info.str_info.wstr_ptr = dropdownlist[index].wstr_ptr;
                    item_info.str_info.wstr_len = dropdownlist[index].wstr_len;
                    GUISETLIST_AddItem(MMIBROWSER_SNAPSHOT_SAVE_FILE_SETLIST_CTRL_ID, &item_info);
                }
#endif

                if(dev == default_dev)
                {
                    select_index = index;
                }
                index++;
           }
        }
        
#ifdef MMI_PDA_SUPPORT
        GUISETLIST_SetCurIndex(MMIBROWSER_SNAPSHOT_SAVE_FILE_SETLIST_CTRL_ID, select_index);
#else
        GUIDROPDOWNLIST_AppendItemArray(MMIBROWSER_SNAPSHOT_SAVE_F_DROPDOWN_CTRL_ID, dropdownlist, index);
        GUIDROPDOWNLIST_SetCurItemIndex(MMIBROWSER_SNAPSHOT_SAVE_F_DROPDOWN_CTRL_ID, select_index);
#endif /* MMI_PDA_SUPPORT */
    }
    else//don't find storage,hide it temp
    {
        GUIFORM_SetChildDisplay(MMIBROWSER_SNAPSHOT_SAVE_FILE_FORM_CTRL_ID, MMIBROWSER_SNAPSHOT_SAVE_F_FORM2_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
    }

}

/*****************************************************************************/
//  Description : set snapshot save media file window
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void BrwSetSnapshotSaveMediaFileWindow(MMI_CTRL_ID_T ctrl_id)
{
#if 0
    GUIRICHTEXT_ITEM_T  item_data = {0};
    uint16 index = 0;
    BRW_MSG_ENTER_FILENM_REQ_T* file_name_req = PNULL;

    item_data.text_type = GUIRICHTEXT_TEXT_RES;
    item_data.text_data.res.id = TXT_FILENAME;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

    SCI_MEMSET(&item_data, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;
    item_data.text_frame_type = GUIRICHTEXT_FRAME_NORMAL;
    item_data.tag_type = GUIRICHTEXT_TAG_NORMAL;
    file_name_req = MMIBROWSER_GetFileNameReq();
    if (0 != MMIAPICOM_Wstrlen(file_name_req->res_name_arr))
    {
        item_data.text_data.buf.len = MMIAPICOM_Wstrlen(file_name_req->res_name_arr);
        item_data.text_data.buf.str_ptr = file_name_req->res_name_arr;
    }
    else
    {
        item_data.text_data.buf.len = 1;
        item_data.text_data.buf.str_ptr = L" ";
    }
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
    
    SCI_MEMSET(&item_data, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
    item_data.text_type = GUIRICHTEXT_TEXT_RES;
    item_data.text_data.res.id = STXT_STORE_MEDIUM;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

    SCI_MEMSET(&item_data, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
    item_data.img_type = GUIRICHTEXT_IMAGE_RES;
    item_data.img_data.res.id = IMAGE_CONTROL_LIST_RADIO_UNSELECTED_ICON;
    item_data.text_type = GUIRICHTEXT_TEXT_RES;
    item_data.text_data.res.id = TXT_BROWSER_SAVE_TO_PHONE;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
    
    item_data.img_data.res.id = IMAGE_CONTROL_LIST_RADIO_UNSELECTED_ICON;
    item_data.text_data.res.id = TXT_COMMON_SD_CARD;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
#endif
}

/*****************************************************************************/
//  Description : handle snapshot popmenu window message
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleSnapshotPopMenuWinMsg(MMI_WIN_ID_T win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param)
{
    MMI_MENU_GROUP_ID_T         group_id = 0;
    MMI_MENU_ID_T               menu_id = 0;
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               ctrl_id = MMIBROWSER_SNAPSHOT_CTRL_ID;
    uint16                      list_index = 0; 
    const BRW_SNAPSHOT_ITEM_INFO_T*   item_info_ptr = PNULL;
    MMI_CTRL_ID_T   snapshot_list_ctrl_id = MMIBROWSER_SNAPSHOT_LIST_CTRL_ID;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        BrwSetSnapshotOptionItem(win_id);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
        GUIMENU_GetId(ctrl_id,&group_id,&menu_id);
        list_index=GUILIST_GetCurItemIndex(snapshot_list_ctrl_id);
        switch(menu_id) 
        {
        case ID_POPUP_MENU_SNAPSHOT_OPEN:
            item_info_ptr = BRW_SnapshotGetItemInfo(list_index);

            if (PNULL != item_info_ptr)
            {
                MMIBROWSER_StartWeb(MMIBROWSER_WEB_ENTRY_SNAPSHOT, PNULL, item_info_ptr->file_name);
            }
            MMK_CloseWin(win_id);
            MMK_CloseWin(MMIBROWSER_SHAPSHOT_LIST_WIN_ID);
            break;
        case ID_POPUP_MENU_SNAPSHOT_DELETE:
            BrwQueryIsDeleteSnapshot(snapshot_list_ctrl_id);
            break;
        case ID_POPUP_MENU_SNAPSHOT_DETAIL:
            item_info_ptr = BRW_SnapshotGetItemInfo(list_index);
            if (PNULL == item_info_ptr)
            {
                //SCI_TRACE_LOW:"\n mmibrowser BrwHandleSnapshotPopMenuWinMsg BRW_SnapshotGetItemInfo error 1"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_SNAPSHOT_1543_112_2_18_2_6_47_331,(uint8*)"");
                break;
            }
            MMK_CreateWin((uint32*)BRW_SNAPSHOT_DETAIL_WIN_TAB,(ADD_DATA)item_info_ptr);
            MMK_CloseWin(win_id);
            break;
        case ID_POPUP_MENU_SNAPSHOT_RENAME:
            item_info_ptr = BRW_SnapshotGetItemInfo(list_index);
            if (PNULL == item_info_ptr)
            {
                //SCI_TRACE_LOW:"\n mmibrowser BrwHandleSnapshotPopMenuWinMsg BRW_SnapshotGetItemInfo error 2"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_SNAPSHOT_1553_112_2_18_2_6_47_332,(uint8*)"");
                break;
            }
            MMK_CreateWin((uint32*)MMIBROWSER_SNAPSHOT_EDIT_FILENAME_WIN_TAB,(ADD_DATA)item_info_ptr);
            MMK_CloseWin(win_id);
            break;
            
#ifdef MMI_PDA_SUPPORT
        case ID_POPUP_MENU_SNAPSHOT_MARK:
            MMK_CloseWin(win_id);
            GUIWIN_SetTitleButtonState(MMIBROWSER_SHAPSHOT_LIST_WIN_ID, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
            BrwHandleSnapshotListMarked(snapshot_list_ctrl_id);
            GUIFORM_SetChildDisplay(MMIBROWSER_PAGEMEMO_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_NORMAL);
            GUILIST_SetAllSelected(snapshot_list_ctrl_id,FALSE);
            break;
#endif /* MMI_PDA_SUPPORT */

        case ID_POPUP_MENU_SNAPSHOT_MARKED:
            BrwHandleSnapshotListMarked(snapshot_list_ctrl_id);
            MMK_CloseWin(win_id);
            break;
        case ID_POPUP_MENU_SNAPSHOT_UNMARKED:
            BrwHandleSnapshotListUnmarked(snapshot_list_ctrl_id);
            MMK_CloseWin(win_id);
            break;
        case ID_POPUP_MENU_SNAPSHOT_MARKED_ALL:
            BrwHandleSnapshotListMarkedAll(snapshot_list_ctrl_id);
            MMK_CloseWin(win_id);
            break;
        case ID_POPUP_MENU_SNAPSHOT_UNMARKED_ALL:
            BrwHandleSnapshotListUnmarkedAll(snapshot_list_ctrl_id);
            MMK_CloseWin(win_id);
            break;
        default:
            break;
        }
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
         MMK_CloseWin(win_id);
         break;
            
    case MSG_PROMPTWIN_OK:
        {
            MMI_STRING_T    str = {0};
            MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &str);
            MMIPUB_OpenWaitWin(1, &str, PNULL, PNULL, MMIBROWSER_DELETE_WAIT_WIN_ID, IMAGE_NULL, ANIM_PUBWIN_WAIT, WIN_ONE_LEVEL, MMIPUB_SOFTKEY_NONE, BrwHandleDeleteSnapshotWinMsg);
        }
        
        MMIBROWSER_CloseQuerytWin(PNULL);
        MMK_CloseWin(win_id);
        break;

    case MSG_PROMPTWIN_CANCEL:
        MMIBROWSER_CloseQuerytWin(PNULL);
        MMK_CloseWin(win_id);
        break;
    
    default:
         result = MMI_RESULT_FALSE;
         break;
    }
    
    return result;
}

/*****************************************************************************/
//  Description : handle delete snapshot window message
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleDeleteSnapshotWinMsg(
                                       MMI_WIN_ID_T win_id, 
                                       MMI_MESSAGE_ID_E msg_id, 
                                       DPARAM param
                                       )
{
    MMI_RESULT_E    result   = MMI_RESULT_TRUE;  
    
    switch(msg_id)
    {
    
    case MSG_OPEN_WINDOW:
        MMIDEFAULT_AllowOpenKeylocWin(FALSE);
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, PNULL);
        break;

    case MSG_GET_FOCUS:
        MMIDEFAULT_AllowOpenKeylocWin(FALSE);//MS00189926
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, PNULL);
        break;

    case MSG_FULL_PAINT:
        MMK_PostMsg(MMIBROWSER_DELETE_WAIT_WIN_ID,MSG_BRW_DELETE_SNAPSHOT,PNULL,0);
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, PNULL);        
        break;     
        
    case MSG_BRW_DELETE_SNAPSHOT:
        if(!BrwDeleteSelectedSnapshot(MMIBROWSER_SNAPSHOT_LIST_CTRL_ID))
        {
            MMIPUB_OpenAlertWarningWin(TXT_COMMON_DEL_FAIL);
        }
        else
        {
#ifdef MMI_GUI_STYLE_TYPICAL
            MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
#endif /* MMI_GUI_STYLE_TYPICAL */
        }
        
        //删除完全后,取消列表的标记状态
#ifndef MMI_PDA_SUPPORT
        GUILIST_SetTextListMarkable(MMIBROWSER_SNAPSHOT_LIST_CTRL_ID,FALSE);
#else
        BrwHandleSnapshotListUnmarkedAll(MMIBROWSER_SNAPSHOT_LIST_CTRL_ID);
        GUISOFTKEY_SetTextId(MMIBROWSER_SNAPSHOT_LIST_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, TXT_MARK_ALL, TXT_DELETE, STXT_CANCEL, FALSE);
        GUIFORM_SetChildDisplay(MMIBROWSER_PAGEMEMO_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
#endif

        MMK_CloseWin(win_id);
        break;

#ifndef MMI_PDA_SUPPORT
    case MSG_APP_RED:
#else
    case MSG_KEYUP_RED:
#endif
        break;
        
    case MSG_CLOSE_WINDOW:
        MMIDEFAULT_AllowOpenKeylocWin(TRUE);//MS00189926
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, PNULL);
        break;

    default:
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, PNULL);
        break;
    }
    return result;
}

/*****************************************************************************/
// 	Description : delete selected snapshot
//	Global resource dependence : none
//  Author: li.li
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BrwDeleteSelectedSnapshot(
                             MMI_CTRL_ID_T list_ctrl_id   //in
                             )
{
    uint16  mark_num = 0;
    uint16  item_index = 0;
    uint16* select_item_ptr = PNULL;
//     uint16  i = 0;
    BOOLEAN result = FALSE;

    //get mark num
    mark_num = GUILIST_GetSelectedItemIndex(list_ctrl_id,PNULL,0);
    if (0 == mark_num)
    {
        //ger current index
        item_index = GUILIST_GetCurItemIndex(list_ctrl_id);
        if (BRW_SnapshotDeleteItem(item_index))
        {
            GUILIST_RemoveItem(list_ctrl_id,item_index);
            result = TRUE;
        }
        else
        {
            //SCI_TRACE_LOW:"[BRW] BrwDeleteSelectedSnapshot FAIL 1"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_SNAPSHOT_1713_112_2_18_2_6_47_333,(uint8*)"");
        }
    }
    else
    {
	    select_item_ptr = (uint16*)SCI_ALLOCA(sizeof(uint16)*GUILIST_GetTotalItemNum(list_ctrl_id));
        if (PNULL != select_item_ptr)
        {
            while(mark_num>0)
            {
                GUILIST_GetSelectedItemIndex(list_ctrl_id,select_item_ptr,mark_num);
                if (BRW_SnapshotDeleteItem(select_item_ptr[0]))
                {
                    GUILIST_RemoveItem(list_ctrl_id,select_item_ptr[0]);
                }
                else
                {
                    //SCI_TRACE_LOW:"[BRW]BrwDeleteSelectedSnapshot FAIL 2"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_SNAPSHOT_1730_112_2_18_2_6_47_334,(uint8*)"");
                    break;
                }
                mark_num = GUILIST_GetSelectedItemIndex(list_ctrl_id,PNULL,0);
            }
            SCI_FREE(select_item_ptr);
            
            result = (mark_num > 0)? FALSE: TRUE;
        }
        else
        {
            //SCI_TRACE_LOW:"[BRW] BrwDeleteSelectedSnapshot ALLOC FAIL!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_SNAPSHOT_1741_112_2_18_2_6_47_335,(uint8*)"");
        }
    }

    return result;
}

/*****************************************************************************/
// 	Description : query is delete snapshot
//	Global resource dependence : none
//  Author: li.li
//	Note:
/*****************************************************************************/
LOCAL void BrwQueryIsDeleteSnapshot(
                             MMI_CTRL_ID_T list_ctrl_id   //in
                             )
{
    uint16                      mark_num = 0;
    uint16                      item_index = 0;
//     uint16                      i = 0;
    const BRW_SNAPSHOT_ITEM_INFO_T*   item_info_ptr = PNULL;
    MMI_STRING_T                dst_temp1 = {0};
    MMI_STRING_T                dst_temp2 = {0};
    char                        num_temp[MMIBROWSER_NUM_TEMP_LEN + 1] = {0};
    //wchar                       temp_wchar[MMIBROWSER_TEMP_LEN + 1] = {0};
    wchar                       *temp_wptr = PNULL;
    uint16  temp_wstr_len = 0;

    //get mark num
    mark_num = GUILIST_GetSelectedItemIndex(list_ctrl_id,PNULL,0);
    if (0 == mark_num)
    {
        //ger current index
        item_index = GUILIST_GetCurItemIndex(list_ctrl_id);
        item_info_ptr = BRW_SnapshotGetItemInfo(item_index);
        if (PNULL != item_info_ptr)
        {
#ifdef MMI_PDA_SUPPORT
            MMI_GetLabelTextByLang(TXT_BROWSER_PDA_DELETE_SNAPSHOT, &dst_temp1);
#else
            MMI_GetLabelTextByLang(TXT_BROWSER_DELETE_SNAPSHOT, &dst_temp1);
#endif
            dst_temp2.wstr_len = MMIAPICOM_Wstrlen(item_info_ptr->file_name) - MMIBROWSER_SNAPSHOT_PATH_LEN - 1 - MMIBROWSER_SNAPSHOT_EXT_NAME_LEN;
            dst_temp2.wstr_ptr = item_info_ptr->file_name + MMIBROWSER_SNAPSHOT_PATH_LEN + 1;
            MMIBROWSER_OpenQueryWinByTextPtr(&dst_temp1,&dst_temp2,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
        }
        
    }
    else if (1 == mark_num)
    {
        GUILIST_GetSelectedItemIndex(list_ctrl_id,&item_index,GUILIST_GetTotalItemNum(list_ctrl_id));
        item_info_ptr = BRW_SnapshotGetItemInfo(item_index);
        if (PNULL != item_info_ptr)
        {
#ifdef MMI_PDA_SUPPORT
            MMI_GetLabelTextByLang(TXT_BROWSER_PDA_DELETE_SNAPSHOT, &dst_temp1);
#else
            MMI_GetLabelTextByLang(TXT_BROWSER_DELETE_SNAPSHOT, &dst_temp1);
#endif
            dst_temp2.wstr_len = MMIAPICOM_Wstrlen(item_info_ptr->file_name) - MMIBROWSER_SNAPSHOT_PATH_LEN - 1 - MMIBROWSER_SNAPSHOT_EXT_NAME_LEN;
            dst_temp2.wstr_ptr = item_info_ptr->file_name + MMIBROWSER_SNAPSHOT_PATH_LEN + 1;
            MMIBROWSER_OpenQueryWinByTextPtr(&dst_temp1,&dst_temp2,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
        }
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_BROWSER_DELETE_BY_NUM, &dst_temp1);
        CFL_Snprintf(num_temp,sizeof(num_temp),"%d",mark_num);
        MMI_GetLabelTextByLang(TXT_BROWSER_DELETE_SNAPSHOT_BY_NUM, &dst_temp2);

        temp_wstr_len = strlen(num_temp) + dst_temp2.wstr_len;
        
        temp_wptr = SCI_ALLOCA(sizeof(wchar) * (temp_wstr_len + 1));
        
        if (temp_wptr != PNULL)
        {
            SCI_MEMSET(temp_wptr, 0x00, sizeof(wchar) * (temp_wstr_len + 1));
            MMIAPICOM_StrToWstr((const uint8*)num_temp, temp_wptr);
            MMIAPICOM_Wstrncat(temp_wptr, dst_temp2.wstr_ptr, dst_temp2.wstr_len);
            dst_temp2.wstr_len = MMIAPICOM_Wstrlen(temp_wptr);
            dst_temp2.wstr_ptr = temp_wptr;
        }
        else//error,无法显示数量
        {
            dst_temp2.wstr_len = 0;
            dst_temp2.wstr_ptr = PNULL;
            //SCI_TRACE_LOW:"BrwQueryIsDeleteSnapshot temp_wptr ALLOC ERROR!!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_SNAPSHOT_1825_112_2_18_2_6_48_336,(uint8*)"");
        }
        
        MMIBROWSER_OpenQueryWinByTextPtr(&dst_temp1,&dst_temp2,IMAGE_PUBWIN_QUERY,PNULL,PNULL);

        if (temp_wptr != PNULL)
        {
            SCI_FREE(temp_wptr);
        }
    }
}
/*****************************************************************************/
// 	Description : handle snapshot list mark option
//	Global resource dependence : none
//  Author: li.li
//	Note:
/*****************************************************************************/
LOCAL void BrwHandleSnapshotListMarked(
                               MMI_CTRL_ID_T  list_ctrl_id
                               )
{
    uint32      list_num = GUILIST_GetTotalItemNum(list_ctrl_id);
    
    //set list enable mark
    GUILIST_SetTextListMarkable(list_ctrl_id,TRUE);
    
    //set mark max number
    GUILIST_SetMaxSelectedItem(list_ctrl_id,(uint16)list_num);
    
#ifndef MMI_PDA_SUPPORT//NEWMS00133500
    //mark item
    GUILIST_SetSelectedItem(list_ctrl_id,GUILIST_GetCurItemIndex(list_ctrl_id),TRUE);
#endif    
}

/*****************************************************************************/
// 	Description : handle snapshot list mark cancel option
//	Global resource dependence : none
//  Author: li.li
//	Note:
/*****************************************************************************/
LOCAL void BrwHandleSnapshotListUnmarked(
                                 MMI_CTRL_ID_T    list_ctrl_id
                                 )
{
    uint16      cur_index = 0;
    uint16      mark_num = 0;

    //get item index
    cur_index = GUILIST_GetCurItemIndex(list_ctrl_id);

    //cancel mark item
    GUILIST_SetSelectedItem(list_ctrl_id,cur_index,FALSE);

    //get mark num
    mark_num = GUILIST_GetSelectedItemIndex(list_ctrl_id,PNULL,0);
    if (0 == mark_num)
    {
        //set list enable mark
        GUILIST_SetTextListMarkable(list_ctrl_id,FALSE);
    }
}

/*****************************************************************************/
// 	Description : handle snapshot list mark all option
//	Global resource dependence : none
//  Author: li.li
//	Note:
/*****************************************************************************/
LOCAL void BrwHandleSnapshotListMarkedAll(
                                  MMI_CTRL_ID_T   list_ctrl_id
                                  )
{
    uint16      mark_num = 0;
    uint32      list_num = GUILIST_GetTotalItemNum(list_ctrl_id);

    //get mark num
    mark_num = GUILIST_GetSelectedItemIndex(list_ctrl_id,PNULL,0);
    if (0 == mark_num)
    {
        //set list enable mark
        GUILIST_SetTextListMarkable(list_ctrl_id,TRUE);

        //set mark max number
        GUILIST_SetMaxSelectedItem(list_ctrl_id,(uint16)list_num);
    }

    //mark all item
    GUILIST_SetAllSelected(list_ctrl_id,TRUE);
}

/*****************************************************************************/
// 	Description : handle snapshot list mark all cancel option
//	Global resource dependence : none
//  Author: li.li
//	Note:
/*****************************************************************************/
LOCAL void BrwHandleSnapshotListUnmarkedAll(
                                    MMI_CTRL_ID_T list_ctrl_id
                                    )
{
    GUILIST_SetAllSelected(list_ctrl_id,FALSE);

    //set list enable mark
    GUILIST_SetTextListMarkable(list_ctrl_id,FALSE);
}

/*****************************************************************************/
// 	Description : handle snapshot rename edit window
//	Global resource dependence : none
//  Author: li.li
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleSnapshotEditFileNameWinMsg(
                                   MMI_WIN_ID_T	win_id, 
                                   MMI_MESSAGE_ID_E	msg_id, 
                                   DPARAM		param
                                   )
{
    MMI_RESULT_E    ret=MMI_RESULT_TRUE;    
    MMI_CTRL_ID_T   ctrl_id=MMIBROWSER_SNAPSHOT_EDIT_FILENAME_CTRL_ID;
    BRW_SNAPSHOT_ITEM_INFO_T    *item_info_ptr = MMK_GetWinAddDataPtr(win_id);
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        if (PNULL != item_info_ptr)//rename
        {
            GUIEDIT_SetString(ctrl_id,
                item_info_ptr->file_name+MMIBROWSER_SNAPSHOT_PATH_LEN+1,
                MMIAPICOM_Wstrlen(item_info_ptr->file_name)-MMIBROWSER_SNAPSHOT_PATH_LEN -1 - MMIBROWSER_SNAPSHOT_EXT_NAME_LEN);
        }
        
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    {
        wchar   full_path_file_name[MMIBROWSER_SNAPSHOT_FULL_PATH_NAME_LEN+1]={0};
        uint16  file_name_len = 0;    
        MMI_STRING_T 	            text_str = {0};
        wchar                       temp_file_name[MMIBROWSER_SNAPSHOT_FILE_NAME_LEN+1] = {0};
        uint16  list_index = 0; 

        GUIEDIT_GetString(ctrl_id,&text_str);
        
        //MS00192558:重命名时增加文件名有效性(此时不可为空)的判断
        if ((PNULL == text_str.wstr_ptr) || (0 == text_str.wstr_len))
        {
            MMIPUB_OpenAlertWarningWin(TXT_BROWSER_SNAPSHOT_FAIL_NO_TITLE);
            break;
        }
        if (!MMIAPICOM_FileNameIsValid(&text_str)
           || !MMIAPICOM_GetValidFileName(FALSE, text_str.wstr_ptr,text_str.wstr_len, temp_file_name, MMIBROWSER_SNAPSHOT_FILE_NAME_LEN))
        {
            MMIPUB_OpenAlertWarningWin(TXT_DC_NAMEINVALID);
            break;
        }
        
        list_index=GUILIST_GetCurItemIndex(MMIBROWSER_SNAPSHOT_LIST_CTRL_ID);
        SCI_MEMSET(full_path_file_name, 0, sizeof(full_path_file_name));
        //path
        MMIAPICOM_Wstrncpy(full_path_file_name, item_info_ptr->file_name, MMIBROWSER_SNAPSHOT_PATH_LEN + 1);
        //filename
        file_name_len = MMIAPICOM_Wstrlen(temp_file_name);
        MMIAPICOM_Wstrncpy(full_path_file_name + MMIAPICOM_Wstrlen(full_path_file_name),
                temp_file_name, 
                MIN(file_name_len, MMIBROWSER_SNAPSHOT_FILE_NAME_LEN));
        //.suffix
        CFLWCHAR_StrNCatWithAsciiStr(full_path_file_name, MMIBROWSER_SNAPSHOT_EXT_NAME, (MMIBROWSER_SNAPSHOT_FULL_PATH_NAME_LEN + 1));
        
        file_name_len = MMIAPICOM_Wstrlen(full_path_file_name);
        if (0 == MMIAPICOM_CompareTwoWstr(full_path_file_name, file_name_len,item_info_ptr->file_name,MMIAPICOM_Wstrlen(item_info_ptr->file_name)))
        {
            //SCI_TRACE_LOW:"\n MMIBROWSER not change snapshot name"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_SNAPSHOT_1989_112_2_18_2_6_48_337,(uint8*)"");
#ifdef MMI_GUI_STYLE_TYPICAL
            MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
#endif /* MMI_GUI_STYLE_TYPICAL */
        }
        else if (MMIAPIFMM_IsFileExist(full_path_file_name, file_name_len))
        {
            MMIPUB_OpenAlertWarningWin(TXT_DC_NAMEREPEAT);
            break;
        }
        else
        {
            if (BRW_SnapshotRenameFile(list_index,full_path_file_name))
            {
                text_str.wstr_len = MMIAPICOM_Wstrlen(temp_file_name);
                text_str.wstr_ptr = temp_file_name;
                BrwAppendPicTextListItem(MMIBROWSER_SNAPSHOT_LIST_CTRL_ID,
                    STXT_OPTION,
                    text_str,
#ifdef MMI_PDA_SUPPORT
                    IMAGE_BROWSER_PAGE,
#else
                    IMAGE_BROWSER_SNAPSHOT_ICON,
#endif                
                    list_index,
                    TRUE);
#ifdef MMI_GUI_STYLE_TYPICAL
                MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
#endif /* MMI_GUI_STYLE_TYPICAL */
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_BROWSER_SNAPSHOT_RENAME_FAIL);
            }
        }
        MMK_CloseWin(win_id);
    }
        break;
        
#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
            MMI_CTRL_ID_T       src_ctrl_id = 0;
            if (PNULL != param)
            {
                src_ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;

                if (src_ctrl_id == MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID)
                {
                    MMK_PostMsg(win_id, MSG_CTL_OK, PNULL, 0);
                }
                else if (src_ctrl_id == MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID)
                {
                    MMK_PostMsg(win_id, MSG_CTL_CANCEL, PNULL, 0);
                }
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527        
#endif /* MMI_PDA_SUPPORT */

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL: 
        MMK_CloseWin(win_id);
        break;

    default:
        ret = MMI_RESULT_FALSE;
        break;
    }

    return ret;
}

/*****************************************************************************/
//  Description : snapshot save file callback
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void BrwSnapshotSaveFileCallback(SFS_ERROR_E sfs_error)
{
    MMI_TEXT_ID_T   alert_text_id = TXT_CMSBRW_FULL_NO_STORE;

    if (SFS_ERROR_NONE == sfs_error)
    {
#ifdef MMI_GUI_STYLE_TYPICAL
        MMIPUB_OpenAlertSuccessWin(TXT_BROWSER_SNAPSHOT_SAVE_FILE_SUCC);
#endif /* MMI_GUI_STYLE_TYPICAL */
    }
    else
    {
        if ((SFS_ERROR_NO_SPACE == sfs_error) || (SFS_ERROR_FOLD_FULL == sfs_error))
        {
            alert_text_id = TXT_NO_SPACE;
        }
        MMIPUB_OpenAlertWarningWin(alert_text_id);
    }
    MMIPUB_CloseWaitWin(MMIBROWSER_SAVE_WAIT_WIN_ID);
    MMK_CloseWin(MMIBROWSER_SNAPSHOT_SAVE_FILE_WIN_ID);
}

/*****************************************************************************/
//  Description : handle open wait window message
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleOpenWaitWinMsg(
                                       MMI_WIN_ID_T win_id, 
                                       MMI_MESSAGE_ID_E msg_id, 
                                       DPARAM param
                                       )
{
    MMI_RESULT_E    result   = MMI_RESULT_TRUE;  
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIDEFAULT_AllowOpenKeylocWin(FALSE);
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, PNULL);
        break;
    
    case MSG_GET_FOCUS:
        MMIDEFAULT_AllowOpenKeylocWin(FALSE);//MS00189926
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, PNULL);
        break;

#ifndef MMI_PDA_SUPPORT
    case MSG_APP_RED:
#else
    case MSG_KEYUP_RED:
#endif
        break;      

    case MSG_CLOSE_WINDOW:
        MMIDEFAULT_AllowOpenKeylocWin(TRUE);//MS00189926
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, PNULL);
        break;

    default:
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, PNULL);
        break;
    }
    return result;
}

/*****************************************************************************/
//  Description : open wait window for open one snapshot file
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_OpenSnapshotWaitingWin(void)
{
    MMI_STRING_T    wait_str = {0};
    MMI_GetLabelTextByLang(TXT_BROWSER_OPEN_WAIT, &wait_str); 
    MMIPUB_OpenWaitWin(1,&wait_str,PNULL,PNULL,MMIBROWSER_OPEN_SNAPSHOT_WAIT_WIN_ID,IMAGE_NULL,
                ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,(MMIPUB_HANDLE_FUNC)BrwHandleOpenWaitWinMsg);
}

#ifdef MMI_PDA_SUPPORT

/*****************************************************************************/
//  Description : handle bookmark detail popmenu window message
//  Global resource dependence : 
//  Author: dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandlePageMemoPopUpWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"BrwHandlePageMemoPopUpWinMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_SNAPSHOT_2130_112_2_18_2_6_48_338,(uint8*)"d", msg_id);

    switch (msg_id) {
    case MSG_OPEN_WINDOW:
        BrwCreatePageMemoPopUpMenu(win_id, MMIBROWSER_PAGEMEMO_POPUP_MENU_CTRL_ID);
        MMK_SetAtvCtrl(win_id, MMIBROWSER_PAGEMEMO_POPUP_MENU_CTRL_ID);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        {
            MMI_MENU_ID_T                   menu_id = 0;
            uint16                          list_index = 0; 
            const BRW_SNAPSHOT_ITEM_INFO_T  *item_info_ptr = PNULL;
            MMI_CTRL_ID_T                   snapshot_list_ctrl_id = MMIBROWSER_SNAPSHOT_LIST_CTRL_ID;
    
            list_index = GUILIST_GetCurItemIndex(snapshot_list_ctrl_id);

            menu_id = GUIMENU_GetCurNodeId(MMIBROWSER_PAGEMEMO_POPUP_MENU_CTRL_ID);

            item_info_ptr = BRW_SnapshotGetItemInfo(list_index);

            if (PNULL == item_info_ptr)
            {
                //SCI_TRACE_LOW:"\n mmibrowser BrwHandleSnapshotPopMenuWinMsg item_info_ptr = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_SNAPSHOT_2157_112_2_18_2_6_48_339,(uint8*)"d", list_index);
                MMK_CloseWin(win_id);
                break;
            }

            switch (menu_id)
            {
            case ID_POPUP_MENU_SNAPSHOT_DELETE:
                BrwQueryIsDeleteSnapshot(snapshot_list_ctrl_id);
                break;

            case ID_POPUP_MENU_SNAPSHOT_DETAIL:
                //BrwOpenPageMemoPropertyWin((BRW_SNAPSHOT_ITEM_INFO_T *)item_info_ptr);
                MMK_CreateWin((uint32*)BRW_SNAPSHOT_DETAIL_WIN_TAB,(ADD_DATA)item_info_ptr);
                MMK_CloseWin(win_id);
                break;

            case ID_POPUP_MENU_SNAPSHOT_RENAME:
                MMK_CreatePubEditWin((uint32*)MMIBROWSER_SNAPSHOT_EDIT_FILENAME_WIN_TAB,(ADD_DATA)item_info_ptr);
                MMK_CloseWin(win_id);
                break;

            default:
                break;
            }

            break;
        }

    case MSG_PROMPTWIN_OK:
        {
            MMI_STRING_T    str = {0};
            MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &str);
            MMIPUB_OpenWaitWin(1, &str, PNULL, PNULL, MMIBROWSER_DELETE_WAIT_WIN_ID, IMAGE_NULL, ANIM_PUBWIN_WAIT, WIN_ONE_LEVEL, MMIPUB_SOFTKEY_NONE, BrwHandleDeleteSnapshotWinMsg);
        }
        
        MMIBROWSER_CloseQuerytWin(PNULL);
        MMK_CloseWin(win_id);
        break;

    case MSG_PROMPTWIN_CANCEL:
        MMIBROWSER_CloseQuerytWin(PNULL);
        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  BrwCreatePageMemoPopUpMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    uint32                              num = 0;
    uint16                              index = 0;
    MMI_STRING_T                        title_str = {0};
    const BRW_SNAPSHOT_ITEM_INFO_T      *item_info_ptr = PNULL;

    MMI_TEXT_ID_T tex_index[] = {TXT_DELETE, TXT_COMM_DETAIL, TXT_COMMON_RENAME};
    uint32 node_index[] = {ID_POPUP_MENU_SNAPSHOT_DELETE, ID_POPUP_MENU_SNAPSHOT_DETAIL, ID_POPUP_MENU_SNAPSHOT_RENAME};

    index = GUILIST_GetCurItemIndex(MMIBROWSER_SNAPSHOT_LIST_CTRL_ID);

    item_info_ptr = BRW_SnapshotGetItemInfo(index);

    if (PNULL != item_info_ptr)
    {
        title_str.wstr_ptr = item_info_ptr->file_name + MMIBROWSER_SNAPSHOT_PATH_LEN + 1;
        title_str.wstr_len = MMIAPICOM_Wstrlen(item_info_ptr->file_name) - MMIBROWSER_SNAPSHOT_PATH_LEN - 1 - MMIBROWSER_SNAPSHOT_EXT_NAME_LEN;
    }

    num = sizeof(node_index) / sizeof(node_index[0]);

    MMIBROWSER_CreatePopupAutoMenu(win_id, ctrl_id, &title_str, num, tex_index, node_index);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void BrwOpenPageMemoPropertyWin(const BRW_SNAPSHOT_ITEM_INFO_T *item_info_ptr)
{
    uint16              len = 0;
    char                temp_buf[MMIBROWSER_SNAPSHOT_TEMPBUF_LEN +1] = {0};
    uint16              temp_uint16_buf[MMIBROWSER_SNAPSHOT_TEMPBUF_LEN +1] = {0};
    uint32              alert_time_period = 0;
    wchar               property[MMIBROWSER_MAX_URL_LEN + GUIRICHTEXT_TEXT_MAX_LEN + 2 * MMIBROWSER_SNAPSHOT_TEMPBUF_LEN + MMIBROWSER_PROPERTY_EXT_LEN] = {0};
    MMI_STRING_T        str = {0};
    wchar               *property_ptr = PNULL;

    if (PNULL == item_info_ptr)
    {
        return;
    }

    property_ptr = property;

    /* Title: */
    MMIRES_GetText(TXT_COMM_TITLE, PNULL, &str);
    MMIAPICOM_Wstrncpy(property_ptr, str.wstr_ptr, str.wstr_len);
    property_ptr += str.wstr_len;

    property_ptr[0] = '\n';
    property_ptr += 1;

    /* Title */
    len = MMIAPICOM_Wstrlen(item_info_ptr->file_name) - MMIBROWSER_SNAPSHOT_PATH_LEN - 1 - MMIBROWSER_SNAPSHOT_EXT_NAME_LEN;
    len = MIN(len, GUIRICHTEXT_TEXT_MAX_LEN);
    MMIAPICOM_Wstrncpy(property_ptr, item_info_ptr->file_name + MMIBROWSER_SNAPSHOT_PATH_LEN+ 1, len);
    property_ptr += len;

    property_ptr[0] = '\n';
    property_ptr += 1;

    /* Size: */
    MMIRES_GetText(TXT_BROWSER_SNAPSHOT_FILE_SIZE, PNULL, &str);
    MMIAPICOM_Wstrncpy(property_ptr, str.wstr_ptr, str.wstr_len);
    property_ptr += str.wstr_len;

    property_ptr[0] = '\n';
    property_ptr += 1;

    /* Size */
    if (item_info_ptr->file_size / MMIBROWSER_ONE_K > 0)
    {
        CFL_Snprintf(temp_buf,sizeof(temp_buf),"%.1f%s",(float)(item_info_ptr->file_size)/MMIBROWSER_ONE_K,"KB");
    }
    else
    {
        CFL_Snprintf(temp_buf,sizeof(temp_buf),"%d%s",item_info_ptr->file_size,"B");
    }

    CFLWCHAR_StrNCpyWithAsciiStr(temp_uint16_buf,temp_buf,MMIBROWSER_SNAPSHOT_TEMPBUF_LEN +1);
    len = MMIAPICOM_Wstrlen(temp_uint16_buf);

    MMIAPICOM_Wstrncpy(property_ptr, temp_uint16_buf, len);
    property_ptr += len;

    property_ptr[0] = '\n';
    property_ptr += 1;

    /* Date: */
    MMIRES_GetText(TXT_BROWSER_SNAPSHOT_FILE_SAVE_TIME, PNULL, &str);
    MMIAPICOM_Wstrncpy(property_ptr, str.wstr_ptr, str.wstr_len);
    property_ptr += str.wstr_len;

    property_ptr[0] = '\n';
    property_ptr += 1;

    /* Date */

    SCI_MEMSET(temp_buf, 0x00, sizeof(temp_buf));
    CFL_Snprintf(temp_buf,sizeof(temp_buf),"%d",item_info_ptr->creat_date.year);
    CFLWCHAR_StrNCpyWithAsciiStr(temp_uint16_buf,temp_buf,MMIBROWSER_SNAPSHOT_TEMPBUF_LEN +1);
    CFLWCHAR_StrNCatWithAsciiStr(temp_uint16_buf,"-",MMIBROWSER_SNAPSHOT_TEMPBUF_LEN +1);

    SCI_MEMSET(temp_buf, 0x00, sizeof(temp_buf));
    CFL_Snprintf(temp_buf,sizeof(temp_buf),"%d",item_info_ptr->creat_date.mon);
    CFLWCHAR_StrNCatWithAsciiStr(temp_uint16_buf,temp_buf,MMIBROWSER_SNAPSHOT_TEMPBUF_LEN +1);
    CFLWCHAR_StrNCatWithAsciiStr(temp_uint16_buf,"-",MMIBROWSER_SNAPSHOT_TEMPBUF_LEN +1);

    SCI_MEMSET(temp_buf, 0x00, sizeof(temp_buf));
    CFL_Snprintf(temp_buf,sizeof(temp_buf),"%d",item_info_ptr->creat_date.day);
    CFLWCHAR_StrNCatWithAsciiStr(temp_uint16_buf,temp_buf,MMIBROWSER_SNAPSHOT_TEMPBUF_LEN +1);

    len = MMIAPICOM_Wstrlen(temp_uint16_buf);

    MMIAPICOM_Wstrncpy(property_ptr, temp_uint16_buf, len);
    property_ptr += len;

    str.wstr_ptr = property;
    str.wstr_len = MMIAPICOM_Wstrlen(property);

    MMIPUB_OpenAlertWinByTextPtr(&alert_time_period, &str, PNULL, IMAGE_PUBWIN_SUCCESS, PNULL, PNULL, MMIPUB_SOFTKEY_CUSTOMER, BrwHandlePageMemoAlertWinMsg);
}

/*****************************************************************************/
// Description :
// Global resource dependence : none
// Author:Jiaoyou.wu
// Note:    
/*****************************************************************************/
LOCAL MMI_RESULT_E  BrwHandlePageMemoAlertWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"BrwHandleHistoryAlertWinMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_SNAPSHOT_2352_112_2_18_2_6_49_340,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            MMI_STRING_T    title_str = {0};

            MMIPUB_SetWinSoftkey(win_id, TXT_NULL, STXT_OK, FALSE);

            MMIRES_GetText(TXT_BROWSER_FILE_DETAIL, PNULL, &title_str);
            MMIPUB_SetWinTitleText(win_id, &title_str, FALSE);
        }
        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
    default:
        recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    }

    return (recode);
}
#endif /* MMI_PDA_SUPPORT */
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif


/*Edit by script, ignore 6 case. Fri Apr 27 09:38:48 2012*/ //IGNORE9527
