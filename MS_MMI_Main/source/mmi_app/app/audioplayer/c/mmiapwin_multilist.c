/*****************************************************************************
** File Name:               mmiapwin_multilist.c                             *
** Author:                                                                   *
** Date:           2012/7/19                                                 *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe ap control                  *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME              DESCRIPTION                              *
** 2012/7/19      aoke.hu           Create                                   *
******************************************************************************/

#define _MMIAPWIN_MULTILIST_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_audioplayer_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmi_menutable.h"
#include "window_parse.h"
#include "mmipub.h"
#include "mmi_appmsg.h"
#include "mmi_pubmenuwin.h"

//other module
#include "mmiim.h"
#include "mmimms_export.h"

#if defined(DRM_SUPPORT)
#include "drm_api.h"
#include "mmi_drmfilemgr.h"
#include "mmidrm_text.h"
#include "mmidrm_export.h"
#endif

//local module
#include "mmiap_play.h"
#include "mmiap_list.h"
#include "mmiap_list_manager.h"
#include "mmiap_list_control.h"
#include "mmiap_applet.h"
#include "mmiapwin_multilist.h"
#include "mmimp3_menutable.h"
#include "mmimp3_id.h"
#include "mmimp3_text.h"
#include "mmimp3_image.h"
#include "mmiap_common.h"
#include "mmiap_list_control.h"
#include "mmiapwin_menu.h"
#include "mmimp3_export.h"
#include "mmiapwin_set.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef struct
{
    MMI_CTRL_ID_T list_ctrl_id;
    uint16        cur_list_selection;
    uint16        cur_file_selection;
    BOOLEAN       is_music_list;
    BOOLEAN       is_mark_all;
    BOOLEAN       is_need_add_music;
} LIST_WIN_USER_DATA_T;

typedef struct
{
    MMI_CTRL_ID_T list_ctrl_id;
    uint16        cur_list_selection;
    BOOLEAN       is_need_add_query_win;
} LISTS_WIN_USER_DATA_T;

typedef struct
{
    BOOLEAN is_music_list;
    uint16  list_selection;
    MMI_HANDLE_T win_handle;
} LIST_LOAD_MUSIC_T;
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Init the list item of the list window of all play lists.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void InitAllPlayListsItem(
    MMI_CTRL_ID_T   ctrl_id,
    BOOLEAN         is_adding_my_music,
    uint16          item_index,
    MMI_TEXT_ID_T   left_softkey_id,
    MMI_TEXT_ID_T   middle_softkey_id
);


/*****************************************************************************/
//  Description :  Handle list add to waiting win
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMp3ListAddToWaitingWin(
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
);

/*****************************************************************************/
//  Description : open opt menu
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void  OpenListListOptMenu(MMI_WIN_ID_T         dst_win_id);

/*****************************************************************************/
//  Description : handle the window message of the list of play lists.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMp3PlayListListWinMsg(
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
);

/*****************************************************************************/
//  Description : handle messages of the add to list window.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMp3AddToListWinMsg(
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
);

/*****************************************************************************/
//  Description : handle creating new mp3 play list window message
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMp3CreateNewListWinMsg(
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
);


/*****************************************************************************/
//  Description : handle renaming mp3 play list window message
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMp3RenameListWinMsg(
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
);


//PDA风格UI
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : open list list long ok pop context menu
//  Global resource dependence :
//  Author: xin.li
//  Note: 音乐列表窗口长按LIST弹出context menu
/*****************************************************************************/
LOCAL void  OpenListListLongOkOptMenu(MMI_WIN_ID_T         dst_win_id);

/*****************************************************************************/
//  Description : HandlePopMenuInitHandler
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandlePopMenuInitHandler(MMIPUB_MENU_NOTIFY_T *param);



#endif //MMI_PDA_SUPPORT

/*****************************************************************************/
//  Description : handle pub menu win init handler
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandlePubMenuInitHandler(MMIPUB_MENU_NOTIFY_T *param);

/*****************************************************************************/
//  Description : handle pub menu win ok handler
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandlePubMenuOKHandler(MMIPUB_MENU_NOTIFY_T *param);

/*****************************************************************************/
//  Description : CopyList
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void CopyList(MMI_HANDLE_T handle,
                    MMI_MENU_ID_T       menu_id,
                    uint16    cur_list_selection);

/*****************************************************************************/
//  Description : EntryRenameWin
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void EntryRenameWin(uint16 cur_list_selection);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void EnterPlayListWin(uint16 cur_selection,
                            BOOLEAN is_music_list);

/*****************************************************************************/
//  Description : handle general play list window message
//  Global resource dependence :
//  Author:jassmine.meng
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMp3PlayListWinMsg(
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void SetListWinTitle(
    MMI_WIN_ID_T    win_id,
    uint16          cur_list_selection
);

/*****************************************************************************/
//  Description :Init the control of general play list window.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void InitPlayListCtrl(LIST_WIN_USER_DATA_T      *user_data_ptr);

/*****************************************************************************/
//  Description :Init the control of general play list window.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void InitListCtrl(MMI_CTRL_ID_T ctrl_id,
                        uint32  total_num);

/*****************************************************************************/
//  Description :Init the list item of general play list window.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void InitPlayListItem(
    MMI_CTRL_ID_T   ctrl_id,
    uint16          item_index,
    MMI_TEXT_ID_T   left_softkey_id,
    MMI_TEXT_ID_T   mid_softkey_id,
    MMI_TEXT_ID_T   right_softkey_id,
    BOOLEAN         is_music_list,
    uint16          cur_list_selection
);



#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : open my music list Long Ok context menu
//  Global resource dependence :
//  Author: xin.li
//  Note: 其他音乐列表长按LIST弹出context menu
/*****************************************************************************/
LOCAL void  OpenPlaylistLongOkOptMenu(MMI_WIN_ID_T         dst_win_id);

/*****************************************************************************/
//  Description : handle pub menu win init handler
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandleListPopMenuInitHandler(MMIPUB_MENU_NOTIFY_T *param);
#endif
/*****************************************************************************/
//  Description : handle pub menu win init handler
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandleListPubMenuOKHandler(MMIPUB_MENU_NOTIFY_T *param);

/*****************************************************************************/
//  Description : CanbeSetRing
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CanbeSetRing(BOOLEAN is_music_list,
                           uint16 list_index,
                           uint16 file_index);

/*****************************************************************************/
//  Description : SetCallRing
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetRing(MMI_MENU_ID_T menu_id,
                      BOOLEAN is_music_list,
                      uint16 cur_list_selection,
                      uint16 cur_file_selection);

/*****************************************************************************/
//  Description : SendAudioFile
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SendAudioFile(MMI_MENU_ID_T menu_id,
                            BOOLEAN is_music_list,
                            uint16 cur_list_selection,
                            uint16 cur_file_selection);

/*****************************************************************************/
//  Description : OpenDetailWin
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN OpenDetailWin(BOOLEAN is_music_list,
                            uint16 cur_list_selection,
                            uint16 cur_file_selection);

/*****************************************************************************/
//  Description : OpenAddFileToListWaitingWin
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN OpenAddFileToListWaitingWin(MMIMP3_LIST_ADD_TO_INFO_T *user_data_ptr);




/*****************************************************************************/
//  Description : open opt menu
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void  OpenPlaylistOptMenu(MMI_WIN_ID_T         dst_win_id);

/*****************************************************************************/
//  Description : handle pub menu win init handler
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandleListPubMenuInitHandler(MMIPUB_MENU_NOTIFY_T *param);

#ifdef MMIAP_SORT_SUPPORT
/*****************************************************************************/
//  Description : set sort type selected
//  Global resource dependence :
//  Author:bin.ji
//  Note:
/*****************************************************************************/
LOCAL void SetSortTypeSelected(
    MMI_CTRL_ID_T       ctrl_id,
    uint16 list_selection,
    BOOLEAN is_music_list
);
#endif

/*****************************************************************************/
//  Description : Set Play List Opt Menu Grey Status
//  Global resource dependence :
//  Author: xingdong.li
//  Note:   //此版本的所有false分支皆可去除
/*****************************************************************************/
LOCAL void  SetPlaylistOptMenuGreyStatus(BOOLEAN is_music_list,
        uint16 cur_list_selection,
        uint16 cur_file_selection,
        MMI_CTRL_ID_T   list_ctrl_id,
        MMI_CTRL_ID_T   ctrl_id);

/*****************************************************************************/
//  Description : DeleteFileFromList
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DeleteFileFromList(LIST_WIN_USER_DATA_T user_data_info,
                                 MMI_WIN_ID_T list_win_id,
                                 BOOLEAN is_del_file);

/*****************************************************************************/
//  Description : CreateIndexList
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL uint16* CreateIndexList(MMI_CTRL_ID_T list_ctrl_id,
                              uint32 *select_num_ptr,
                              uint16 cur_file_selection);

/*****************************************************************************/
//  Description : CreateIndexList
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void DestoryIndexList(uint16 * index_list_ptr);

/*****************************************************************************/
//  Description :  Handle list delete waiting win
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMp3ListDeleteWaitingWin(
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
);

/*****************************************************************************/
//  Description :  UpdateList
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void UpdateList(MMI_HANDLE_T handle,
                      MMI_CTRL_ID_T ctrl_id,
                      uint32 total_num,
                      void *param);

/*****************************************************************************/
//  Description :  UpdateList
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void UpdatePlayList(MMI_HANDLE_T handle,
                          LIST_WIN_USER_DATA_T *user_data_ptr,
                          void *param);

/*****************************************************************************/
//  Description :CreateAddFilesWinData
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void CreateAddFilesWinData(MMI_HANDLE_T           win_handle,
                                 MMIMP3_LIST_ADD_TO_INFO_T * list_add_info_ptr);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void EnterAddToListWin(MMIMP3_LIST_ADD_TO_INFO_T * list_add_info_ptr);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void EnterCreateNewListWin(MMIMP3_LIST_ADD_TO_INFO_T * list_add_info_ptr);

/*****************************************************************************/
//  Description : GetListNameWithoutSuffix
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetListNameWithoutSuffix(uint16 cur_list_selection,
                                       MMI_STRING_T  *name_no_suffix_ptr);

/*****************************************************************************/
//  Description : MMIMP3_AddFileToList
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
LOCAL void AddFileFromFmmToList(BOOLEAN is_music_list,
                                uint16 cur_list_selection);

/*****************************************************************************/
//  Description : AddFilesToList
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AddFilesFromListToList(LIST_WIN_USER_DATA_T *user_data_ptr,
                                     MMI_MENU_ID_T menu_id,
                                     MMI_HANDLE_T win_handle);

/*****************************************************************************/
//  Description : OpenLoadAllMusicFilesWaitingWin
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN OpenLoadAllMusicFilesWaitingWin(BOOLEAN is_music_list,
        uint16  list_selection,
        MMI_HANDLE_T win_handle);
/**--------------------------------------------------------------------------*
 **                         CONSTANT VARIABLES                               *
 **--------------------------------------------------------------------------*/
LOCAL WINDOW_TABLE(MMIMP3_PLAY_LIST_LIST_WIN_TAB) =
{
#ifndef MMI_AUDIOPLAYER_MINI_SUPPORT
    WIN_STATUSBAR,
#endif
    WIN_TITLE(TXT_MP3_MUSIC_LIST),
    WIN_FUNC((uint32)HandleMp3PlayListListWinMsg),
    WIN_ID(MMIMP3_PLAY_LIST_LIST_WIN_ID),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIMP3_PLAY_LIST_LIST_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};



LOCAL WINDOW_TABLE(MMIMP3_ADD_TO_LIST_WIN_TAB) =
{
#ifndef MMI_AUDIOPLAYER_MINI_SUPPORT
    WIN_STATUSBAR,
#endif
    WIN_TITLE(TXT_MP3_CHOOSE_MUSIC_LIST),
    WIN_FUNC((uint32)HandleMp3AddToListWinMsg),
    WIN_ID(MMIMP3_ADD_TO_LIST_WIN_ID),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIMP3_ADD_TO_LIST_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

LOCAL WINDOW_TABLE(MMIMP3_CREATE_NEW_LIST_WIN_TAB) =
{
    WIN_TITLE(TXT_MP3_INPUT_LIST_NAME),
    WIN_FUNC((uint32)HandleMp3CreateNewListWinMsg),
    WIN_ID(MMIMP3_CREATE_NEW_LIST_WIN_ID),
#ifndef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#else
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
#endif
    CREATE_EDIT_TEXT_CTRL(0, MMIMP3_CREATE_NEW_LIST_EDITBOX_CTRL_ID),
    END_WIN
};

LOCAL WINDOW_TABLE(MMIMP3_RENAME_LIST_WIN_TAB) =
{
    WIN_TITLE(TXT_MP3_INPUT_LIST_NAME),
    WIN_FUNC((uint32)HandleMp3RenameListWinMsg),
    WIN_ID(MMIMP3_RENAME_LIST_WIN_ID),
#ifndef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#else
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
#endif
    CREATE_EDIT_TEXT_CTRL(0, MMIMP3_RENAME_LIST_EDITBOX_CTRL_ID),
    END_WIN
};

// the window for mp3 play list window
LOCAL WINDOW_TABLE(MMIMP3_PLAY_LIST_WIN_TAB) =
{
#ifndef MMI_AUDIOPLAYER_MINI_SUPPORT
    WIN_STATUSBAR,
#endif
    WIN_TITLE(TXT_NULL),
    WIN_FUNC((uint32)HandleMp3PlayListWinMsg),
    WIN_ID(MMIMP3_PLAY_LIST_WIN_ID),
#ifdef MMI_PDA_SUPPORT//NEWMS00169534
    WIN_STYLE(WS_DISABLE_SOFTKEY_TO_TITLE),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIMP3_PLAY_LIST_FORM_CTRL_ID),
    CHILD_SOFTKEY_CTRL(TXT_MARK_ALL, TXT_DELETE, STXT_CANCEL, MMICOMMON_SOFTKEY_CTRL_ID, MMIMP3_PLAY_LIST_FORM_CTRL_ID),
    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, MMIMP3_PLAY_LIST_CTRL_ID, MMIMP3_PLAY_LIST_FORM_CTRL_ID),
#else
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIMP3_PLAY_LIST_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    END_WIN
};



/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPMULTILIST_EnterMultiListWin(void)
{
    MMIAPAPPLET_CreateWinTable((uint32 *)MMIMP3_PLAY_LIST_LIST_WIN_TAB, PNULL);

}

/*****************************************************************************/
//  Description :CreateAddFilesWinData
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void CreateAddFilesWinData(MMI_HANDLE_T           win_handle,
                                 MMIMP3_LIST_ADD_TO_INFO_T * list_add_info_ptr)
{
    MMIMP3_LIST_ADD_TO_INFO_T * user_data_ptr = PNULL;

    if (PNULL != list_add_info_ptr
            && 0 != list_add_info_ptr->select_num)
    {
        user_data_ptr = SCI_ALLOCA(sizeof(MMIMP3_LIST_ADD_TO_INFO_T));
        if (PNULL != user_data_ptr)
        {
            SCI_MEMSET(user_data_ptr,0,sizeof(MMIMP3_LIST_ADD_TO_INFO_T));
            user_data_ptr->select_index_ptr = SCI_ALLOCA((sizeof(uint16)*list_add_info_ptr->select_num));
            if (PNULL != user_data_ptr->select_index_ptr)
            {
                SCI_MEMSET(user_data_ptr->select_index_ptr,0,((sizeof(uint16)*list_add_info_ptr->select_num)));
                SCI_MEMCPY(user_data_ptr->select_index_ptr,list_add_info_ptr->select_index_ptr,((sizeof(uint16)*list_add_info_ptr->select_num)));
                user_data_ptr->dst_list_index = list_add_info_ptr->dst_list_index;
                user_data_ptr->is_new_list = list_add_info_ptr->is_new_list;
                user_data_ptr->select_num = list_add_info_ptr->select_num;
                user_data_ptr->src_is_music_list = list_add_info_ptr->src_is_music_list;
                user_data_ptr->src_list_index = list_add_info_ptr->src_list_index;
                user_data_ptr->win_handle = list_add_info_ptr->win_handle;
                MMK_SetWinUserData(win_handle,user_data_ptr);
                SCI_TRACE_LOW("[AP MULTILIST] CreateAddFilesWinData dst_list_index = %d, is_new_list = %d, select_num=%d,src_list_index = %d", user_data_ptr->dst_list_index,user_data_ptr->is_new_list,user_data_ptr->select_num,user_data_ptr->src_list_index);
            }
            else
            {
                SCI_FREE(user_data_ptr);
            }
        }
    }

}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void EnterCreateNewListWin(MMIMP3_LIST_ADD_TO_INFO_T * list_add_info_ptr)
{

    MMI_HANDLE_T           win_handle = PNULL;

    win_handle = MMIAPAPPLET_CreateWinTable((uint32 *)MMIMP3_CREATE_NEW_LIST_WIN_TAB, PNULL);


    CreateAddFilesWinData(win_handle,list_add_info_ptr);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void EnterAddToListWin(MMIMP3_LIST_ADD_TO_INFO_T * list_add_info_ptr)
{

    MMI_HANDLE_T           win_handle = PNULL;

    win_handle = MMIAPAPPLET_CreateWinTable((uint32 *)MMIMP3_ADD_TO_LIST_WIN_TAB, PNULL);
    CreateAddFilesWinData(win_handle,list_add_info_ptr);
}

/*****************************************************************************/
//  Description : enter need add music win
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
LOCAL void EnterNeedAddMusicWin(
    LISTS_WIN_USER_DATA_T *user_data_ptr //in
)
{
    MMI_WIN_ID_T query_win_id = MMIMP3_NEED_ADD_MUSIC_WIN_ID;


    SCI_TRACE_LOW("[MMIAP] EnterNeedAddMusicWin");

    if (!MMK_IsOpenWin(query_win_id))
    {
        MMIPUB_OpenQueryWinByTextIdEx(SPRD_MP3_APPLET_ID,
                                      TXT_MP3_NEED_ADD_MUSIC,
                                      IMAGE_PUBWIN_QUERY,&query_win_id,
                                      PNULL,
                                      PNULL);
    }
}

/*****************************************************************************/
//  Description : close need add music win
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
LOCAL void CloseNeedAddMusicWin(
    LISTS_WIN_USER_DATA_T *user_data_ptr //in
)
{
    MMI_WIN_ID_T query_win_id = MMIMP3_NEED_ADD_MUSIC_WIN_ID;


    if (PNULL != user_data_ptr)
    {
        user_data_ptr->is_need_add_query_win = FALSE;
    }
    MMK_CloseWin(query_win_id);

    SCI_TRACE_LOW("[MMIAP] CloseNeedAddMusicWin");
}

/*****************************************************************************/
//  Description : handle the window message of the list of play lists.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMp3PlayListListWinMsg(
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
)
{
    MMI_RESULT_E    result  = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIMP3_PLAY_LIST_LIST_CTRL_ID;
    uint16          cur_selection = 0;
    GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr = PNULL;
    uint32          total_num    = MMIMP3_GetTotalListNum();
    LISTS_WIN_USER_DATA_T *user_data_ptr = (LISTS_WIN_USER_DATA_T *)MMK_GetWinUserData(win_id);

    SCI_TRACE_LOW("[AP MULTILIST] HandleMp3PlayListListWinMsg msg_id = %d", msg_id);


    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        user_data_ptr = SCI_ALLOCA(sizeof(LISTS_WIN_USER_DATA_T));
        if (PNULL != user_data_ptr)
        {
            SCI_MEMSET(user_data_ptr,0,sizeof(LISTS_WIN_USER_DATA_T));
            user_data_ptr->list_ctrl_id = ctrl_id;
            MMK_SetWinUserData(win_id,user_data_ptr);
        }
        else
        {
            MMK_CloseWin(win_id);
            break;
        }
#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
        GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, TXT_MP3_LIST_NEW, FALSE);
#else
        GUILIST_SetListState(ctrl_id, GUILIST_STATE_NEED_SOFTKEY, TRUE);
        GUILIST_SetOwnSofterKey(ctrl_id, TRUE);
#endif
        InitListCtrl(ctrl_id,total_num);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_CTL_LIST_NEED_ITEM_DATA:
        need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;
        if(PNULL == need_item_data_ptr)
        {
            MMIMP3_FatalError();
            break;
        }
        InitAllPlayListsItem(
            ctrl_id,
            TRUE,
            (uint16)need_item_data_ptr->item_index,
            STXT_OPTION,
            TXT_BROWSE
        );
        break;

    case MSG_MP3_UPDATE_LIST:
        UpdateList(win_id,ctrl_id,total_num,param);
        break;

    case MSG_PROMPTWIN_OK:
        if (PNULL == user_data_ptr)
        {
            break;
        }

        SCI_TRACE_LOW("[MMIAP] HandleMp3PlayListListMsg PROM OK is_need_add_query_win=%d",\
                      user_data_ptr->is_need_add_query_win);

        if (user_data_ptr->is_need_add_query_win)
        {

            EnterPlayListWin(user_data_ptr->cur_list_selection, TRUE);
            MMK_PostMsg(MMIMP3_PLAY_LIST_WIN_ID, MSG_MP3_NEW_LIST_ADD_MUSIC_MSG, PNULL,NULL);

            CloseNeedAddMusicWin(user_data_ptr);
        }
        else
        {
            MMK_CloseWin(MMIMP3_DEL_QUERY_WIN_ID);
            MMK_CloseWin(MMIMP3_PLAY_LIST_LIST_OPT_WIN_ID);
#ifdef MMI_PDA_SUPPORT
            MMK_CloseWin(MMIMP3_PLAY_LIST_LIST_LONGOK_OPT_WIN_ID);
#endif
            cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
            if (MMIMP3_IsCurPlayList(TRUE,cur_selection))
            {
                MMIAPIMP3_StopAudioPlayer();
            }
            MMIMP3_DeleteList(TRUE,&cur_selection);
            MMK_SendMsg(win_id, MSG_MP3_UPDATE_LIST, (DPARAM)&cur_selection);
        }
        break;


    case MSG_PROMPTWIN_CANCEL:
        if (PNULL == user_data_ptr)
        {
            break;
        }

        SCI_TRACE_LOW("[MMIAP] HandleMp3PlayListListMsg PROM CANCEL is_need_add_query_win=%d",\
                      user_data_ptr->is_need_add_query_win);

        if (user_data_ptr->is_need_add_query_win)
        {
            CloseNeedAddMusicWin(user_data_ptr);
        }
        else
        {
            MMK_CloseWin(MMIMP3_DEL_QUERY_WIN_ID);
        }
        break;

#ifdef CMCC_UI_STYLE
    case MSG_MP3_OPEN_MAIN_PLAY_WIN_SUCCESS:
    {
        MMI_WIN_ID_T    alert_win_id = MMIMP3_QUERY_WIN_ID;

        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
        MMIAPIMP3_StopAudioPlayer();
        if(MMIMP3_SetCurPlayInfo(TRUE,cur_selection,0,MMIMP3_GetMp3PlayMode()))
        {
            if(0 == MMIMP3_GetListTotalNum(TRUE, cur_selection))
            {
                MMIPUB_OpenAlertWinByTextId(PNULL, TXT_COMMON_COMMON_EMPTY_LIST, TXT_NULL,
                                            IMAGE_PUBWIN_WARNING, &alert_win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);

                MMIMP3_ClearCurPlayInfo();
                MMK_CloseWin(win_id);
            }
            else
            {
                MMK_CloseWin(win_id);
                MMK_CloseWin(MMIMP3_PLAYER_OPT_MENU_WIN_ID);
                MMIAPIMP3_PlayAudioPlayer(MMIMP3_ZERO_OFFSET);
            }
        }
    }
    break;
#endif

    case MSG_APP_MENU:
    case MSG_CTL_OK:
#if defined MMI_PDA_SUPPORT && defined MMI_ISTYLE_SUPPORT
        if(MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
        {
            EnterCreateNewListWin(PNULL);
        }
        else
#endif
            if (PNULL != user_data_ptr)
            {
                user_data_ptr->cur_list_selection = GUILIST_GetCurItemIndex(ctrl_id);
                OpenListListOptMenu(win_id);
            }

        break;

#ifdef MMI_PDA_SUPPORT
    case MSG_CTL_LIST_LONGOK:
        //长按List弹出context menu菜单
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
        if(cur_selection >= MMIMP3_SPECIAL_LIST_NUM)
        {
            user_data_ptr->cur_list_selection = cur_selection;
            OpenListListLongOkOptMenu(win_id);
        }
        break;
#endif //MMI_PDA_SUPPORT

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
        EnterPlayListWin(cur_selection,TRUE);
        break;

    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        if (PNULL != user_data_ptr)
        {
            SCI_FREE(user_data_ptr);

        }
        break;

    case MSG_KEYDOWN_RED:
        MMK_CloseWin(win_id);
        result = MMI_RESULT_FALSE;
        break;

    case MSG_MP3_NEW_LIST_ADD_MUSIC_MSG:
        if (PNULL == user_data_ptr)
        {
            break;
        }

        user_data_ptr->is_need_add_query_win = TRUE;
        if(MMK_IsFocusWin(win_id))
        {
            user_data_ptr->cur_list_selection = GUILIST_GetCurItemIndex(ctrl_id);
            EnterNeedAddMusicWin(user_data_ptr);
        }
        break;

    case MSG_GET_FOCUS:
        if (PNULL == user_data_ptr)
        {
            break;
        }

        SCI_TRACE_LOW("[MMIAP] HandleMp3PlayListListMsg GET FOCUS is_need_add_query_win=%d",\
                      user_data_ptr->is_need_add_query_win);

        if(user_data_ptr->is_need_add_query_win)
        {
            user_data_ptr->cur_list_selection = GUILIST_GetCurItemIndex(ctrl_id);
            EnterNeedAddMusicWin(user_data_ptr);
        }
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}


/*****************************************************************************/
//  Description : Init the list item of the list window of all play lists.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void InitAllPlayListsItem(
    MMI_CTRL_ID_T   ctrl_id,
    BOOLEAN         is_adding_my_music,
    uint16          item_index,
    MMI_TEXT_ID_T   left_softkey_id,
    MMI_TEXT_ID_T   middle_softkey_id
)
{
    GUILIST_ITEM_T       item_t    = {0};   /*lint !e64*/
    GUILIST_ITEM_DATA_T  item_data = {0};   /*lint !e64*/
    wchar                file_name[FMM_SEARCH_FILENAME_MAXLEN + 1] = {0};
    uint16               file_name_len = FMM_SEARCH_FILENAME_MAXLEN;

    MMIFILE_DEVICE_E     dev_type = MMI_DEVICE_NUM;
    wchar                file_name_no_suffix[FMM_SEARCH_FILENAME_MAXLEN + 1] = {0};
    MMI_STRING_T         name_no_suffix = {0};
    MMI_IMAGE_ID_T       icon_image = IMAGE_MP3_LIST_UDISK_ICON;
    wchar                space_char = 0x20;
    BOOLEAN              is_path_ok = FALSE;
    uint16               ac_index = item_index;

    //ucs2_str.str_ptr = (uint8*)str;
    item_t.item_data_ptr = &item_data;

    //soft key
    item_data.softkey_id[0] = left_softkey_id;
    item_data.softkey_id[1] = middle_softkey_id;
    item_data.softkey_id[2] = STXT_RETURN;

    //icon
    item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;

    if (!is_adding_my_music)
    {
        ac_index++;
    }

    if(ac_index < MMIMP3_SPECIAL_LIST_NUM)
    {
        item_data.item_content[0].item_data.image_id = IMAGE_MP3_ALL_MUSIC_ICON;
        item_data.item_content[1].item_data_type     = GUIITEM_DATA_TEXT_ID;

        switch(ac_index)
        {
        case MMIMP3_MY_MUSIC_MENU_INDEX:
            item_data.item_content[1].item_data.text_id = TXT_MP3_MY_MUSIC;
            break;

        case MMIMP3_PLAY_LIST_1_INDEX:
            item_data.item_content[1].item_data.text_id = TXT_MP3_LIST_1;
            break;

        case MMIMP3_PLAY_LIST_2_INDEX:
            item_data.item_content[1].item_data.text_id = TXT_MP3_LIST_2;
            break;

        case MMIMP3_PLAY_LIST_3_INDEX:
            item_data.item_content[1].item_data.text_id = TXT_MP3_LIST_3;
            break;

        default:
            break;
        }
    }
    else
    {

        if(MMIMP3_GetListFileNameInfo(TRUE, ac_index, &dev_type, file_name, &file_name_len))
        {
            name_no_suffix.wstr_ptr = file_name_no_suffix;
            MMIAPICOM_GetNameAndSuffix(file_name, file_name_len, &name_no_suffix, PNULL);

            if(MMI_DEVICE_UDISK == dev_type)
            {
                icon_image = IMAGE_MP3_LIST_UDISK_ICON;
                is_path_ok = TRUE;
            }
            else if(MMI_DEVICE_UDISK < dev_type && dev_type < MMI_DEVICE_NUM)
            {
                icon_image = IMAGE_MP3_LIST_SD_ICON;
                is_path_ok = TRUE;
            }
            else
            {
                is_path_ok = FALSE;
            }

        }

        if(!is_path_ok)
        {
            MMIMP3_FatalError();
            return;
        }

        if(0 == name_no_suffix.wstr_len || PNULL == name_no_suffix.wstr_ptr)
        {
            name_no_suffix.wstr_len = 1;
            name_no_suffix.wstr_ptr = &space_char;
        }

        item_data.item_content[0].item_data.image_id = icon_image;
        item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[1].item_data.text_buffer.wstr_len = name_no_suffix.wstr_len;
        item_data.item_content[1].item_data.text_buffer.wstr_ptr = name_no_suffix.wstr_ptr;
    }

    if(MMIMP3_IsCurPlayList(TRUE,ac_index))
    {
        item_data.item_content[0].item_data.image_id = IMAGE_MP3_MUSIC_PLAYING_ICON;
    }

    GUILIST_SetItemData(ctrl_id, &item_data, item_index);
}

/*****************************************************************************/
//  Description : open opt menu
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void  OpenListListOptMenu(MMI_WIN_ID_T         dst_win_id)
{
    MMIPUB_MENU_PARAM_T menuparam = {0};

    menuparam.group_id = MENU_DYNA;
    menuparam.win_id = (MMI_WIN_ID_T)MMIMP3_PLAY_LIST_LIST_OPT_WIN_ID;
    menuparam.ctrl_id = (MMI_CTRL_ID_T)MMIMP3_PLAY_LIST_LIST_OPT_MENU_CTRL_ID;
    menuparam.applet_handle = SPRD_MP3_APPLET_ID;

    menuparam.dst_handle = dst_win_id;
    menuparam.func.OnMenuOk = HandlePubMenuOKHandler;
    menuparam.func.OnMenuInit = HandlePubMenuInitHandler;
    menuparam.menu_style = GUIMENU_STYLE_POPUP;
    MMIPUB_OpenPubMenuWin(&menuparam);

}

/*****************************************************************************/
//  Description : handle pub menu win init handler
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandlePubMenuInitHandler(MMIPUB_MENU_NOTIFY_T *param)
{
#ifndef MMI_PDA_SUPPORT
    MMIFILE_DEVICE_E    dev = MMI_DEVICE_NUM;
    MMIFILE_DEVICE_E    device = MMI_DEVICE_UDISK;
#endif
#ifdef GUI_FULLSCREEN_POPMENU_SUPPORT
	MMI_STRING_T title_text = {0};
#endif
    LISTS_WIN_USER_DATA_T  *user_data_ptr = PNULL;

    MMI_SLISTHANDLE_T handle = PNULL;
    MMIAP_MENU_ITEM_T item = {0};
    BOOLEAN is_display_image = FALSE;


    if(PNULL != param)
    {
        user_data_ptr = (LISTS_WIN_USER_DATA_T *)MMK_GetWinUserData(param->dst_handle);

        //GUIMENU_CreatDynamic(PNULL, param->win_id, param->ctrl_id, GUIMENU_STYLE_POPUP);

        //opt menu

        handle = MMIAP_CreateDynamicMenu(sizeof(item));

        if (PNULL != handle)
        {
#ifndef MMI_PDA_SUPPORT
            item.menu_id = ID_MP3_LIST_OPT_RETURN;
            MMIAP_AddDynamicMenuItem(handle, &item);

            if(PNULL != user_data_ptr
                    && user_data_ptr->cur_list_selection >= MMIMP3_SPECIAL_LIST_NUM)
            {
                item.menu_id = ID_MP3_LIST_LIST_DEL;
                MMIAP_AddDynamicMenuItem(handle, &item);
            }


            item.menu_id = ID_MP3_LIST_LIST_NEW;
            MMIAP_AddDynamicMenuItem(handle, &item);


            if(PNULL != user_data_ptr
                    && user_data_ptr->cur_list_selection >= MMIMP3_SPECIAL_LIST_NUM)
            {
                item.menu_id = ID_MP3_LIST_LIST_RENAME;
                MMIAP_AddDynamicMenuItem(handle, &item);


                dev = MMIMP3_GetListFileDev(TRUE,user_data_ptr->cur_list_selection);
                if(MMI_DEVICE_NUM != dev)
                {

                    for(device = MMI_DEVICE_UDISK; device < MMI_DEVICE_NUM; device++)
                    {
                        if(dev != device
                                && MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(device), MMIAPIFMM_GetDevicePathLen(device)))
                        {
                            item.menu_id = ID_MP3_LIST_LIST_COPY + device - MMI_DEVICE_UDISK;
                            MMIAP_AddDynamicMenuItem(handle, &item);
                        }
                    }
                }
            }


#else   //PDA style menu

            is_display_image = TRUE;
            //新建列表
            item.menu_id = ID_MP3_LIST_LIST_NEW;
            MMIAP_AddDynamicMenuItem(handle, &item);

#endif

            MMIAP_BuildDynamicMenuToCtrl(handle, PNULL, is_display_image, param->ctrl_id);
            MMIAP_DestroyDynamicMenu(handle);
        }
#ifdef GUI_FULLSCREEN_POPMENU_SUPPORT
		MMI_GetLabelTextByLang(STXT_OPTION, &title_text);
		GUIMENU_SetMenuTitle( &title_text,param->ctrl_id );
#endif
    }
}

/*****************************************************************************/
//  Description : handle pub menu win ok handler
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandlePubMenuOKHandler(MMIPUB_MENU_NOTIFY_T *param)
{
    MMI_MENU_ID_T       menu_id = ID_MP3_EXIT;
    MMI_WIN_ID_T        query_win_id = MMIMP3_DEL_QUERY_WIN_ID;
    LISTS_WIN_USER_DATA_T  *user_data_ptr = PNULL;
	uint32          total_num = 0;
	MMI_WIN_ID_T        alert_win_id = MMIMP3_ALERT_WIN_ID;
	
    if(PNULL != param)
    {
        user_data_ptr = (LISTS_WIN_USER_DATA_T *)MMK_GetWinUserData(param->dst_handle);

        menu_id = GUIMENU_GetCurNodeId(param->ctrl_id);
        switch(menu_id)
        {
        case ID_MP3_LIST_LIST_DEL:
            //打开询问是否删除窗口;
            MMIPUB_OpenQueryWinByTextIdEx(SPRD_MP3_APPLET_ID,
                                          TXT_DELETE, IMAGE_PUBWIN_QUERY, &query_win_id, PNULL,
                                          PNULL);

            break;

        case ID_MP3_LIST_LIST_NEW:
            //创建list新建窗口;
			total_num = MMIMP3_GetTotalListNum();
			if(total_num >= AP_MUSIC_LIST_NUM_MAX)
			{
				MMIPUB_OpenAlertWinByTextId(PNULL, TXT_NO_SPACE,//sd 空间不足
                                        TXT_NULL, IMAGE_PUBWIN_WARNING, &alert_win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
			}
			else
			{
				EnterCreateNewListWin(PNULL);
			}

            break;

        case ID_MP3_LIST_LIST_RENAME:
            //创建List重命名窗口;
            if (PNULL != user_data_ptr)
            {
                EntryRenameWin(user_data_ptr->cur_list_selection);
            }
            break;

        case ID_MP3_LIST_OPT_RETURN:
            MMK_ReturnAppointedWin(MMIMP3_MAIN_PLAY_WIN_ID);
            break;

        default:
            if ( PNULL != user_data_ptr)
            {
                CopyList(param->dst_handle,menu_id,user_data_ptr->cur_list_selection);
            }
            break;
        }
    }
}

/*****************************************************************************/
//  Description : CopyList
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void CopyList(MMI_HANDLE_T handle,
                    MMI_MENU_ID_T       menu_id,
                    uint16    cur_list_selection)
{
    MMIFILE_DEVICE_E tem_dev = MMI_DEVICE_UDISK;
    APLAYER_LIST_ERROR_E copy_result = APLAYER_LIST_SUCCESS;
    MMI_WIN_ID_T        alert_win_id = MMIMP3_ALERT_WIN_ID;


    if(menu_id >= ID_MP3_LIST_LIST_COPY && (menu_id <= ID_MP3_LIST_LIST_OPT_END))
    {
        tem_dev = menu_id - ID_MP3_LIST_LIST_COPY + MMI_DEVICE_UDISK;

        copy_result = MMIMP3_CopyList(TRUE,&cur_list_selection,tem_dev);

        SCI_TRACE_LOW("[AP MULTILIST] CopyList cur_list_selection = %d, copy_result = %d, tem_dev = %d",cur_list_selection,copy_result,tem_dev);

        if(APLAYER_LIST_NO_SAPCE == copy_result)
        {
            MMIPUB_OpenAlertWinByTextId(PNULL, TXT_NO_SPACE,//sd 空间不足
                                        TXT_NULL, IMAGE_PUBWIN_WARNING, &alert_win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);

        }
        else if(APLAYER_LIST_HAS_EXIST == copy_result)
        {
            MMIPUB_OpenAlertWinByTextId(PNULL, TXT_MP3_FILE_EXIST,
                                        TXT_NULL, IMAGE_PUBWIN_WARNING, &alert_win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);

        }
        else if (APLAYER_LIST_SUCCESS == copy_result)
        {
            MMK_SendMsg(handle, MSG_MP3_UPDATE_LIST, (DPARAM)&cur_list_selection);
        }
        else
        {
            MMIPUB_OpenAlertWinByTextId(PNULL, TXT_ERROR, TXT_NULL,
                                        IMAGE_PUBWIN_FAIL, &alert_win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
        }


    }

}

/*****************************************************************************/
//  Description : EntryRenameWin
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void EntryRenameWin(uint16 cur_list_selection)
{
    MMI_HANDLE_T      win_handle = PNULL;
    uint16        *cur_list_selection_ptr = PNULL;

    win_handle = MMIAPAPPLET_CreateWinTable((uint32 *)MMIMP3_RENAME_LIST_WIN_TAB, PNULL);

    cur_list_selection_ptr = SCI_ALLOCA(sizeof(uint16));
    if (PNULL != cur_list_selection_ptr)
    {
        SCI_MEMSET(cur_list_selection_ptr,0,sizeof(uint16));
        *cur_list_selection_ptr = cur_list_selection;
        MMK_SetWinUserData(win_handle,cur_list_selection_ptr);
    }
}

/*****************************************************************************/
//  Description : GetListNameWithoutSuffix
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetListNameWithoutSuffix(uint16 cur_list_selection,
                                       MMI_STRING_T  *name_no_suffix_ptr)
{
    wchar   cur_list_file_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16   cur_list_file_name_len = MMIFILE_FULL_PATH_MAX_LEN;
    BOOLEAN   result = FALSE;

    if(PNULL != name_no_suffix_ptr
            && MMIMP3_GetListFileNameInfo(TRUE,cur_list_selection, PNULL,
                                          cur_list_file_name, &cur_list_file_name_len))
    {
        name_no_suffix_ptr->wstr_ptr = SCI_ALLOCA(sizeof(wchar)*(MMIFILE_FILE_NAME_MAX_LEN+1));
        if (PNULL != name_no_suffix_ptr->wstr_ptr)
        {
            SCI_MEMSET(name_no_suffix_ptr->wstr_ptr,0,(sizeof(wchar)*(MMIFILE_FILE_NAME_MAX_LEN+1)));
            MMIAPICOM_GetNameAndSuffix(cur_list_file_name, cur_list_file_name_len, name_no_suffix_ptr, PNULL);
            result = TRUE;
        }
    }

    return result;
}
/*****************************************************************************/
//  Description : handle renaming mp3 play list window message
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMp3RenameListWinMsg(
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
)
{
    MMI_RESULT_E              result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T             ctrl_id = MMIMP3_RENAME_LIST_EDITBOX_CTRL_ID;
    MMI_STRING_T              name_str = {0};
    uint16                    max_ucs2_len = 0;
    uint16                    max_default_len = 0;
    MMI_WIN_ID_T              alert_win_id = MMIMP3_ALERT_WIN_ID;
    APLAYER_LIST_ERROR_E       rename_result = APLAYER_LIST_SUCCESS;
    wchar   new_list_file_name[MMIFILE_FILE_NAME_MAX_LEN +1] = {0};
    uint16  new_list_file_name_len = MMIFILE_FILE_NAME_MAX_LEN;
    uint16                    *cur_selection_ptr = (uint16 *)MMK_GetWinUserData(win_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        if (PNULL == cur_selection_ptr)
        {
            MMK_CloseWin(win_id);
            break;
        }
        max_ucs2_len = (MMIFILE_FULL_PATH_MAX_LEN) - (MMIMP3_RESERVED_DIR_LEN) - (MMIMULTIM_DIR_SYSTEM_LEN) - 20;
        max_ucs2_len = MIN(MMIFILE_FILE_NAME_MAX_LEN, max_ucs2_len);
        max_default_len = max_ucs2_len;
        GUIEDIT_SetTextMaxLen(ctrl_id, max_ucs2_len, max_default_len);

        if(GetListNameWithoutSuffix(*cur_selection_ptr,&name_str))
        {
            GUIEDIT_SetString(ctrl_id, name_str.wstr_ptr, name_str.wstr_len);

            if (PNULL != name_str.wstr_ptr)
            {
                SCI_FREE(name_str.wstr_ptr);
            }
        }
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        if (PNULL != cur_selection_ptr)
        {
            GUIEDIT_GetString(ctrl_id, &name_str);

            SCI_MEMSET(new_list_file_name,0,(sizeof(new_list_file_name)));
            //文件名是否有效,判断接口不统一.......
            if(MMIAPICOM_FileNameIsValid(&name_str)
                    && MMIAPICOM_GetValidFileName(FALSE, name_str.wstr_ptr, name_str.wstr_len, new_list_file_name, MMIFILE_FILE_NAME_MAX_LEN))
            {
                new_list_file_name_len = MMIAPICOM_Wstrlen(new_list_file_name);
                MMI_WSTRNCPY((new_list_file_name + new_list_file_name_len), MMIFILE_FILE_NAME_MAX_LEN - new_list_file_name_len,
                             MMIMP3_PLAY_LIST_SUFFIX_UCS, MMIMP3_PLAY_LIST_SUFFIX_UCS_LEN, MMIMP3_PLAY_LIST_SUFFIX_UCS_LEN);

                new_list_file_name_len += MMIMP3_PLAY_LIST_SUFFIX_UCS_LEN;

                rename_result = MMIMP3_RenameListFile(TRUE,*cur_selection_ptr, new_list_file_name, new_list_file_name_len);

                if(APLAYER_LIST_SUCCESS == rename_result)
                {
                    MMK_SendMsg(MMIMP3_PLAY_LIST_LIST_WIN_ID, MSG_MP3_UPDATE_LIST,PNULL);
                    MMK_CloseWin(win_id);
                }
                else if(APLAYER_LIST_HAS_EXIST == rename_result)
                {
                    MMIPUB_OpenAlertWinByTextId(PNULL, TXT_DC_NAMEREPEAT, TXT_NULL,
                                                IMAGE_PUBWIN_WARNING, &alert_win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
                }
                else
                {
                    MMIPUB_OpenAlertWinByTextId(PNULL, TXT_ERROR, TXT_NULL,
                                                IMAGE_PUBWIN_FAIL, &alert_win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
                }
            }
            else
            {
                MMIPUB_OpenAlertWinByTextId(PNULL, TXT_DC_NAMEINVALID, TXT_NULL,
                                            IMAGE_PUBWIN_WARNING, &alert_win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
            }
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_CTL_PENOK:
#ifdef MMI_PDA_SUPPORT
        if((PNULL != param) && ((MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)
                                || (MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)
                                || (MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)))
        {
            if((MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id))
            {
                //left button
                MMK_PostMsg(win_id, MSG_CTL_OK, PNULL, 0);
            }
            else if((MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id))
            {
                //middle button
            }
            else if((MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id))
            {
                //right button
                MMK_PostMsg(win_id, MSG_CTL_CANCEL, PNULL, 0);
            }

            break;
        }

#endif //MMI_PDA_SUPPORT
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;


    case MSG_CLOSE_WINDOW:
        if (PNULL != cur_selection_ptr)
        {
            SCI_FREE(cur_selection_ptr);
        }
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}


#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : open list list long ok pop context menu
//  Global resource dependence :
//  Author: xin.li
//  Note: 音乐列表窗口长按LIST弹出context menu
/*****************************************************************************/
LOCAL void  OpenListListLongOkOptMenu(MMI_WIN_ID_T         dst_win_id)
{
    MMIPUB_MENU_PARAM_T menuparam = {0};

    menuparam.group_id = MENU_DYNA;
    menuparam.win_id = (MMI_WIN_ID_T)MMIMP3_PLAY_LIST_LIST_LONGOK_OPT_WIN_ID;
    menuparam.ctrl_id = (MMI_CTRL_ID_T)MMIMP3_PLAY_LIST_LIST_LONGOK_OPT_MENU_CTRL_ID;
    menuparam.applet_handle = SPRD_MP3_APPLET_ID;

    menuparam.dst_handle = dst_win_id;
    menuparam.func.OnMenuOk = HandlePubMenuOKHandler;
    menuparam.func.OnMenuInit = HandlePopMenuInitHandler;
    menuparam.menu_style = GUIMENU_STYLE_POPUP_AUTO;
    MMIPUB_OpenPubMenuWin(&menuparam);
}


/*****************************************************************************/
//  Description : HandlePopMenuInitHandler
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandlePopMenuInitHandler(MMIPUB_MENU_NOTIFY_T *param)
{
    //  FILEARRAY_DATA_T    f_array_data = {0};
    MMIFILE_DEVICE_E    dev = MMI_DEVICE_NUM;
    MMIFILE_DEVICE_E    device = MMI_DEVICE_UDISK;
    GUIMENU_TITLE_INFO_T    title_info = {0};
    MMI_STRING_T            main_title = {0};
    MMI_STRING_T            sub_title = {0};
    LISTS_WIN_USER_DATA_T   *user_data_ptr = PNULL;


    MMI_SLISTHANDLE_T handle = PNULL;
    MMIAP_MENU_ITEM_T item = {0};


    if (PNULL != param)
    {
        user_data_ptr = (LISTS_WIN_USER_DATA_T *)MMK_GetWinUserData(param->dst_handle);

        //删除列表
        if(PNULL != user_data_ptr
                && user_data_ptr->cur_list_selection >= MMIMP3_SPECIAL_LIST_NUM)
        {

            //GUIMENU_CreatDynamic(PNULL, param->win_id, param->ctrl_id, GUIMENU_STYLE_POPUP_AUTO);

            //opt menu

            handle = MMIAP_CreateDynamicMenu(sizeof(item));
            if (PNULL != handle)
            {
                item.menu_id = ID_MP3_LIST_LIST_DEL;
                MMIAP_AddDynamicMenuItem(handle, &item);


                item.menu_id = ID_MP3_LIST_LIST_RENAME;
                MMIAP_AddDynamicMenuItem(handle, &item);


                dev = MMIMP3_GetListFileDev(TRUE,user_data_ptr->cur_list_selection);
                if(MMI_DEVICE_NUM != dev)
                {
                    for(device = MMI_DEVICE_UDISK; device < MMI_DEVICE_NUM; device++)
                    {
                        if(dev != device)
                        {
                            if(MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(device),  MMIAPIFMM_GetDevicePathLen(device)))    //copy菜单要求非nor
                            {
                                item.menu_id = ID_MP3_LIST_LIST_COPY + device - MMI_DEVICE_UDISK;
                                MMIAP_AddDynamicMenuItem(handle, &item);
                            }

                        }
                    }
                }

                MMIAP_BuildDynamicMenuToCtrl(handle, PNULL, FALSE, param->ctrl_id);
                MMIAP_DestroyDynamicMenu(handle);
            }


            //set title
            MMIAP_GetListItemTextInfo(user_data_ptr->list_ctrl_id, user_data_ptr->cur_list_selection, &main_title, &sub_title);
            title_info.is_static    = FALSE;
            title_info.title_ptr    = &main_title;
            title_info.sub_title_ptr = &sub_title;
            GUIMENU_SetMenuTitleEx(param->ctrl_id, &title_info);
        }
    }
}

#endif

/*****************************************************************************/
//  Description : handle creating new mp3 play list window message
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMp3CreateNewListWinMsg(
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
)
{
    MMI_RESULT_E              result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T             ctrl_id = MMIMP3_CREATE_NEW_LIST_EDITBOX_CTRL_ID;
    MMI_STRING_T              name_str = {0};
    uint16                    max_ucs2_len = 0;
    uint16                    max_default_len = 0;
    uint16                    list_new_selection = 0;
    MMI_WIN_ID_T              alert_win_id = MMIMP3_ALERT_WIN_ID;
    MMI_STRING_T              default_name = {0};
    APLAYER_LIST_ERROR_E      create_result = APLAYER_LIST_SUCCESS;
    wchar                     list_file_name[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    uint16                    list_file_name_len = MMIFILE_FILE_NAME_MAX_LEN;
    MMIMP3_LIST_ADD_TO_INFO_T * user_data_ptr = (MMIMP3_LIST_ADD_TO_INFO_T *)MMK_GetWinUserData(win_id);


    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        max_ucs2_len = (MMIFILE_FILE_NAME_MAX_LEN) - (MMIMP3_RESERVED_DIR_LEN) - (MMIMULTIM_DIR_SYSTEM_LEN) - 20;
        max_ucs2_len = MIN(MMIFILE_FILE_NAME_MAX_LEN, max_ucs2_len);
        max_default_len = max_ucs2_len;

        GUIEDIT_SetTextMaxLen(ctrl_id, max_ucs2_len, max_default_len);

        MMITHEME_GetResText(TXT_MP3_LIST_NEW, win_id, &default_name);

        if(MMIMP3_GetNewListFileName(default_name, list_file_name,&list_file_name_len))
        {
            GUIEDIT_SetString(ctrl_id, list_file_name, list_file_name_len);
        }

        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        GUIEDIT_GetString(ctrl_id, &name_str);

        SCI_MEMSET(list_file_name,0,(sizeof(list_file_name)));
        //文件名是否有效,判断接口不统一.......
        if(MMIAPICOM_FileNameIsValid(&name_str)
                && MMIAPICOM_GetValidFileName(FALSE, name_str.wstr_ptr, name_str.wstr_len, list_file_name, MMIFILE_FILE_NAME_MAX_LEN))
        {
            list_file_name_len = MMIAPICOM_Wstrlen(list_file_name);

            MMI_WSTRNCPY((list_file_name + list_file_name_len), MMIFILE_FILE_NAME_MAX_LEN - list_file_name_len,
                         MMIMP3_PLAY_LIST_SUFFIX_UCS, MMIMP3_PLAY_LIST_SUFFIX_UCS_LEN, MMIMP3_PLAY_LIST_SUFFIX_UCS_LEN);

            list_file_name_len += MMIMP3_PLAY_LIST_SUFFIX_UCS_LEN;

            create_result = MMIMP3_CreateList(&list_new_selection,list_file_name,list_file_name_len);
            if (APLAYER_LIST_NO_SAPCE == create_result)
            {
                MMIPUB_OpenAlertWinByTextId(PNULL, TXT_NO_SPACE, TXT_NULL,
                                            IMAGE_PUBWIN_WARNING, &alert_win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
            }
            else if(APLAYER_LIST_HAS_EXIST == create_result)
            {
                MMIPUB_OpenAlertWinByTextId(PNULL, TXT_MP3_FILE_EXIST, TXT_NULL,
                                            IMAGE_PUBWIN_WARNING, &alert_win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
            }
            else if(APLAYER_LIST_SUCCESS == create_result)
            {
                if (PNULL != user_data_ptr)
                {
                    user_data_ptr->dst_list_index = list_new_selection;
                    OpenAddFileToListWaitingWin(user_data_ptr);
                }
                else
                {
                    MMK_SendMsg(MMIMP3_PLAY_LIST_LIST_WIN_ID, MSG_MP3_UPDATE_LIST, (DPARAM)&list_new_selection);
                    MMK_PostMsg(MMIMP3_PLAY_LIST_LIST_WIN_ID, MSG_MP3_NEW_LIST_ADD_MUSIC_MSG,PNULL,PNULL);
                }
				GUILIST_SetTextListMarkable(MMIMP3_PLAY_LIST_CTRL_ID, FALSE);
                MMK_CloseWin(win_id);
            }
            else
            {
                MMIPUB_OpenAlertWinByTextId(PNULL, TXT_ERROR, TXT_NULL,
                                            IMAGE_PUBWIN_FAIL, &alert_win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
            }
        }
        else
        {
            MMIPUB_OpenAlertWinByTextId(PNULL, TXT_DC_NAMEINVALID, TXT_NULL,
                                        IMAGE_PUBWIN_WARNING, &alert_win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_CTL_PENOK:
#ifdef MMI_PDA_SUPPORT
        if((PNULL != param) && ((MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)
                                || (MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)
                                || (MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)))
        {
            if((MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id))
            {
                //left button
                MMK_PostMsg(win_id, MSG_CTL_OK, PNULL, 0);
            }
            else if((MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id))
            {
                //middle button
            }
            else if((MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id))
            {
                //right button
                MMK_PostMsg(win_id, MSG_CTL_CANCEL, PNULL, 0);
            }

            break;
        }

#endif //MMI_PDA_SUPPORT
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        if (PNULL != user_data_ptr)
        {
            if ( PNULL != user_data_ptr->select_index_ptr)
            {
                SCI_FREE(user_data_ptr->select_index_ptr);
            }
            SCI_FREE(user_data_ptr);
        }
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}


/*****************************************************************************/
//  Description : OpenAddFileToListWaitingWin
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN OpenAddFileToListWaitingWin(MMIMP3_LIST_ADD_TO_INFO_T *user_data_ptr)
{
    MMIMP3_LIST_ADD_TO_INFO_T *list_info_ptr = PNULL;
    MMI_WIN_ID_T    waiting_win_id = MMIMP3_ADD_TO_WAIT_WIN_ID;
    uint32          waiting_time_out = 0;

    if (PNULL != user_data_ptr)
    {
        list_info_ptr = SCI_ALLOCA(sizeof(MMIMP3_LIST_ADD_TO_INFO_T));
        if (PNULL != list_info_ptr)
        {
            SCI_MEMSET(list_info_ptr,0,sizeof(MMIMP3_LIST_ADD_TO_INFO_T));
            list_info_ptr->select_index_ptr = SCI_ALLOCA((sizeof(uint16)*(user_data_ptr->select_num)));
            if (PNULL != list_info_ptr->select_index_ptr)
            {
                SCI_MEMSET(list_info_ptr->select_index_ptr,0,((sizeof(uint16)*user_data_ptr->select_num)));
                SCI_MEMCPY(list_info_ptr->select_index_ptr,user_data_ptr->select_index_ptr,((sizeof(uint16)*user_data_ptr->select_num)));
                list_info_ptr->dst_list_index = user_data_ptr->dst_list_index;
                list_info_ptr->is_new_list = user_data_ptr->is_new_list;
                list_info_ptr->select_num = user_data_ptr->select_num;
                list_info_ptr->src_is_music_list = user_data_ptr->src_is_music_list;
                list_info_ptr->src_list_index = user_data_ptr->src_list_index;
                list_info_ptr->win_handle = user_data_ptr->win_handle;
                MMIPUB_OpenAlertWinByTextIdEx(SPRD_MP3_APPLET_ID,
                                              &waiting_time_out, TXT_COMMON_WAITING, TXT_NULL,
                                              IMAGE_PUBWIN_WAIT, &waiting_win_id, PNULL, MMIPUB_SOFTKEY_NONE, HandleMp3ListAddToWaitingWin,
                                              PNULL);

                MMK_PostMsg(waiting_win_id, MSG_MP3_BEGIN_ADD_TO_LIST, &list_info_ptr, sizeof(list_info_ptr));
                return TRUE;
            }
            else
            {
                SCI_FREE(list_info_ptr);
            }
        }
    }
    return FALSE;

}


/*****************************************************************************/
//  Description :  Handle list add to waiting win
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMp3ListAddToWaitingWin(
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
)
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMIMP3_LIST_ADD_TO_INFO_T **opr_info_ptr = PNULL;
    MMI_WIN_ID_T        alert_win_id = MMIMP3_ALERT_WIN_ID;
    uint32              added_num = 0;
    APLAYER_LIST_ERROR_E  add_result = APLAYER_LIST_SUCCESS;

    SCI_TRACE_LOW("[AP MULTILIST] HandleMp3ListAddToWaitingWin msg_id = %d", msg_id);

    switch(msg_id)
    {
    case MSG_APP_CANCEL:
    case MSG_APP_RED:
        break;

    case MSG_MP3_BEGIN_ADD_TO_LIST:
        opr_info_ptr = (MMIMP3_LIST_ADD_TO_INFO_T **)param;

        if(PNULL == opr_info_ptr
                || PNULL == *opr_info_ptr
                ||PNULL == (*opr_info_ptr)->select_index_ptr
                ||0  == (*opr_info_ptr)->select_num)

        {
            MMK_CloseWin(win_id);
            break;
        }

        add_result = MMIMP3_AddFileFromListToList(TRUE,
                     (*opr_info_ptr)->dst_list_index,
                     (*opr_info_ptr)->src_is_music_list,
                     (*opr_info_ptr)->src_list_index,
                     (*opr_info_ptr)->select_index_ptr,
                     (*opr_info_ptr)->select_num ,
                     &added_num);

        if (APLAYER_LIST_NO_SAPCE == add_result)
        {
            MMIPUB_OpenAlertWinByTextId(PNULL, TXT_NO_SPACE,
                                        TXT_NULL, IMAGE_PUBWIN_WARNING, &alert_win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
        }
        else if(APLAYER_LIST_SUCCESS == add_result)
        {

            if (PNULL != (*opr_info_ptr)->win_handle)
            {
                MMK_SendMsg((*opr_info_ptr)->win_handle, MSG_MP3_UPDATE_LIST, PNULL);
            }
            MMK_SendMsg(MMIMP3_PLAY_LIST_LIST_WIN_ID, MSG_MP3_UPDATE_LIST, PNULL);

            if((*opr_info_ptr)->select_num != added_num)
            {
                MMIPUB_OpenAlertWinByTextId(PNULL, TXT_NO_SPACE,
                                            TXT_NULL, IMAGE_PUBWIN_WARNING, &alert_win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
            }
            else
            {
                if((*opr_info_ptr)->is_new_list)
                {
                    MMIPUB_OpenAlertWinByTextId(PNULL, TXT_MP3_SUCCESS_ADD_TO_NEW_LIST, TXT_NULL,
                                                IMAGE_PUBWIN_SUCCESS, &alert_win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
                }
                else
                {
                    MMIPUB_OpenAlertWinByTextId(PNULL, TXT_MP3_SUCCESS_ADD_TO_CHOSEN_LIST, TXT_NULL,
                                                IMAGE_PUBWIN_SUCCESS, &alert_win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
                }
            }

        }
        else
        {
            MMIPUB_OpenAlertWinByTextId(PNULL, TXT_ERROR, TXT_NULL,
                                        IMAGE_PUBWIN_FAIL, &alert_win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
        }


        if (PNULL != (*opr_info_ptr)->select_index_ptr)
        {
            SCI_FREE((*opr_info_ptr)->select_index_ptr);
        }
        SCI_FREE((*opr_info_ptr));
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;

    default:
        result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    }

    return result;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void EnterPlayListWin(uint16 cur_selection,BOOLEAN is_music_list)
{
    MMI_HANDLE_T      win_handle = PNULL;
    LIST_WIN_USER_DATA_T      *play_list_user_data_ptr = PNULL;

    play_list_user_data_ptr = SCI_ALLOCA(sizeof(LIST_WIN_USER_DATA_T));
    if (PNULL != play_list_user_data_ptr)
    {
        win_handle = MMIAPAPPLET_CreateWinTable((uint32 *)MMIMP3_PLAY_LIST_WIN_TAB, PNULL);
        SCI_MEMSET(play_list_user_data_ptr,0,sizeof(LIST_WIN_USER_DATA_T));
        play_list_user_data_ptr->cur_list_selection = cur_selection;
        play_list_user_data_ptr->list_ctrl_id = MMIMP3_PLAY_LIST_CTRL_ID;
        play_list_user_data_ptr->is_music_list = is_music_list;
        MMK_SetWinUserData(win_handle,play_list_user_data_ptr);
    }
}

/*****************************************************************************/
//  Description : get first latter by name
//  Global resource dependence :
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL uint16 GetFirstLatterByName (const wchar *filename)
{
    wchar upper_buf [2] = {0};
    upper_buf[0] = filename[0];
    upper_buf[1] = 0;
    MMIAPICOM_Wstrupper (&upper_buf[0]);
    return upper_buf[0];
}


/*****************************************************************************/
//  Description : handle general play list window message
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMp3PlayListWinMsg(
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
)
{
    MMI_RESULT_E    result  = MMI_RESULT_TRUE;
    GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr = PNULL;
#ifdef MMI_PDA_SUPPORT      //NEWMS00169534
    GUIFORM_CHILD_HEIGHT_T child_height = {0};
    uint16                  mark_num = 0;
    GUIFORM_CHILD_DISPLAY_E   display_type = GUIFORM_CHILD_DISP_MAX;
#endif

    LIST_WIN_USER_DATA_T      *play_list_user_data_ptr = (LIST_WIN_USER_DATA_T *)MMK_GetWinUserData(win_id);
    MMI_WIN_ID_T        query_win_id = MMIMP3_DEL_QUERY_WIN_ID;
    uint32              total_num  = 0;


    //SCI_TRACE_LOW:"[MMIMP3] HandleMp3PlayList, msg = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_9769_112_2_18_2_4_58_334, (uint8*)"d", msg_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        if (PNULL == play_list_user_data_ptr)
        {
            MMK_CloseWin(win_id);
            break;
        }
        MMIMP3_OpenList(play_list_user_data_ptr->is_music_list,play_list_user_data_ptr->cur_list_selection);

        if (play_list_user_data_ptr->is_music_list)
        {
            SetListWinTitle(win_id, play_list_user_data_ptr->cur_list_selection);
        }
        InitPlayListCtrl(play_list_user_data_ptr);
        if(MMIMP3_IsCurPlayList(play_list_user_data_ptr->is_music_list,play_list_user_data_ptr->cur_list_selection))
        {
            GUILIST_SetCurItemIndex(play_list_user_data_ptr->list_ctrl_id, MMIMP3_GetCurPlayFileIndex());
        }
        MMK_SetAtvCtrl(win_id, play_list_user_data_ptr->list_ctrl_id);
        play_list_user_data_ptr->is_mark_all = FALSE;

#ifdef MMI_PDA_SUPPORT
        child_height.type = GUIFORM_CHILD_HEIGHT_LEFT;
        GUILIST_PermitBorder(play_list_user_data_ptr->list_ctrl_id, FALSE);
        GUIFORM_IsSlide(MMIMP3_PLAY_LIST_FORM_CTRL_ID, FALSE);
        GUIFORM_SetChildDisplay(MMIMP3_PLAY_LIST_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
        GUIFORM_SetChildHeight(MMIMP3_PLAY_LIST_FORM_CTRL_ID, play_list_user_data_ptr->list_ctrl_id, &child_height);
        GUIFORM_PermitChildBg(MMIMP3_PLAY_LIST_FORM_CTRL_ID, FALSE);
        GUILIST_SetUserBg(play_list_user_data_ptr->list_ctrl_id, FALSE);
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#else
        GUILIST_SetListState(play_list_user_data_ptr->list_ctrl_id, GUILIST_STATE_NEED_SOFTKEY, TRUE);
        GUILIST_SetOwnSofterKey(play_list_user_data_ptr->list_ctrl_id, TRUE);
#endif
        break;

    case MSG_CTL_LIST_NEED_ITEM_DATA:
        if(PNULL != play_list_user_data_ptr
                && 0 != MMIMP3_GetListTotalNum(play_list_user_data_ptr->is_music_list,
                                               play_list_user_data_ptr->cur_list_selection
                                              )
          )
        {
            need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;

            if(PNULL == need_item_data_ptr)
            {
                //SCI_TRACE_LOW:" PNULL != need_item_data_ptr "
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_9884_112_2_18_2_4_58_335, (uint8*)"");
                break;
            }

            InitPlayListItem(
                play_list_user_data_ptr->list_ctrl_id,
                (uint16)need_item_data_ptr->item_index,
                STXT_OPTION,
                TXT_COMMON_PLAY,
                STXT_RETURN,
                play_list_user_data_ptr->is_music_list,
                play_list_user_data_ptr->cur_list_selection
            );
        }

        break;

    case MSG_CTL_LIST_NEED_ITEM_CONTENT:
        break;
    case MSG_MP3_SWITCH_AUTO:
	case MSG_MP3_SWITCH_BY_BT:
        if(MMIMP3_IsCurPlayList(play_list_user_data_ptr->is_music_list,play_list_user_data_ptr->cur_list_selection))
        {
			GUILIST_SetCurItemIndex(play_list_user_data_ptr->list_ctrl_id, MMIMP3_GetCurPlayFileIndex());
            InitPlayListItem(
                play_list_user_data_ptr->list_ctrl_id,
                MMIMP3_GetCurPlayFileIndex(),
                STXT_OPTION,
                TXT_COMMON_PLAY,
                STXT_RETURN,
                play_list_user_data_ptr->is_music_list,
                play_list_user_data_ptr->cur_list_selection
				);
        }
		MMK_UpdateScreen();
		break;

    case MSG_MP3_UPDATE_LIST:
        if (PNULL != play_list_user_data_ptr)
        {
            UpdatePlayList(win_id,play_list_user_data_ptr,param);
        }
        break;

    case MSG_GET_FOCUS:
#ifdef MMI_PDA_SUPPORT
        if (PNULL != play_list_user_data_ptr)
        {
            if( 0 == MMIMP3_GetListTotalNum(play_list_user_data_ptr->is_music_list,play_list_user_data_ptr->cur_list_selection))
            {
                GUIFORM_SetChildDisplay(MMIMP3_PLAY_LIST_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
                GUILIST_SetTextListMarkable(play_list_user_data_ptr->list_ctrl_id,FALSE);
            }

            if(play_list_user_data_ptr->is_mark_all)//NEWMS00178103
            {
                GUISOFTKEY_SetTextId(win_id, MMICOMMON_SOFTKEY_CTRL_ID, TXT_CANCEL_MARK_ALL, TXT_DELETE, STXT_CANCEL, TRUE);
            }
            else
            {
                GUISOFTKEY_SetTextId(win_id, MMICOMMON_SOFTKEY_CTRL_ID, TXT_MARK_ALL, TXT_DELETE, STXT_CANCEL, TRUE);
            }
        }
#else
        MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, NULL);
#endif

        if (PNULL != play_list_user_data_ptr)
        {
            if (play_list_user_data_ptr->is_need_add_music)
            {
                play_list_user_data_ptr->is_need_add_music = FALSE;
                AddFileFromFmmToList(play_list_user_data_ptr->is_music_list,play_list_user_data_ptr->cur_list_selection);
            }
        }

		MMIMP3_UpdatePlayListInfo(); 
 		if(MMIMP3_IsCurPlayList(play_list_user_data_ptr->is_music_list,play_list_user_data_ptr->cur_list_selection))
        {
 			GUILIST_SetCurItemIndex(play_list_user_data_ptr->list_ctrl_id, MMIMP3_GetCurPlayFileIndex());
        }

        break;

    case MSG_CTL_LIST_MARK_ITEM:
        if (PNULL != play_list_user_data_ptr)
        {

#ifdef MMI_PDA_SUPPORT//NEWMS00169534
            mark_num = GUILIST_GetSelectedItemIndex(play_list_user_data_ptr->list_ctrl_id, PNULL, 0);

            if(0 == mark_num)
            {
                GUISOFTKEY_SetTextId(win_id, MMICOMMON_SOFTKEY_CTRL_ID, TXT_MARK_ALL, TXT_DELETE, STXT_CANCEL, TRUE);
                play_list_user_data_ptr->is_mark_all = FALSE;
            }

            if(GUILIST_GetTotalItemNum(play_list_user_data_ptr->list_ctrl_id) == mark_num)
            {
                GUISOFTKEY_SetTextId(win_id, MMICOMMON_SOFTKEY_CTRL_ID, TXT_CANCEL_MARK_ALL, TXT_DELETE, STXT_CANCEL, TRUE);
                play_list_user_data_ptr->is_mark_all = TRUE;
            }
#else

            if(0 == GUILIST_GetSelectedItemIndex(play_list_user_data_ptr->list_ctrl_id, PNULL, 0))
            {
                GUILIST_SetTextListMarkable(play_list_user_data_ptr->list_ctrl_id, FALSE);
            }
#endif
        }
        break;

    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:
        if (PNULL != play_list_user_data_ptr)
        {
            play_list_user_data_ptr->cur_file_selection = GUILIST_GetCurItemIndex(play_list_user_data_ptr->list_ctrl_id);
            total_num = MMIMP3_GetListTotalNum(play_list_user_data_ptr->is_music_list,play_list_user_data_ptr->cur_list_selection);

#ifdef MMI_PDA_SUPPORT      //NEWMS00169534
            GUIFORM_GetChildDisplay(MMIMP3_PLAY_LIST_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, &display_type);

            if(GUIFORM_CHILD_DISP_NORMAL == display_type)
            {
                MMI_CTRL_ID_T       ctrl_id_local = 0;

                if(PNULL != param)
                {
                    ctrl_id_local = ((MMI_NOTIFY_T*) param)->src_id;

                    if(ctrl_id_local == MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID) //删除菜单
                    {
                        mark_num = GUILIST_GetSelectedItemIndex(play_list_user_data_ptr->list_ctrl_id, PNULL, 0);

                        if(0 == mark_num)
                        {
                            MMIPUB_OpenAlertWarningWin(TXT_SMS_SELECT_ONT_PROMPT);
                        }
                        else
                        {
                            DeleteFileFromList(*play_list_user_data_ptr,win_id,FALSE);
                        }

                        break;
                    }
                    else if(ctrl_id_local == MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID)//mark all 菜单
                    {
                        play_list_user_data_ptr->is_mark_all = !play_list_user_data_ptr->is_mark_all;
                        if (play_list_user_data_ptr->is_mark_all)
                        {
                            GUISOFTKEY_SetTextId(win_id, MMICOMMON_SOFTKEY_CTRL_ID, TXT_CANCEL_MARK_ALL, TXT_DELETE, STXT_CANCEL, FALSE);
                            if(0 == GUILIST_GetSelectedItemIndex(play_list_user_data_ptr->list_ctrl_id, PNULL, 0))
                            {
                                GUILIST_SetTextListMarkable(play_list_user_data_ptr->list_ctrl_id, TRUE);
                                GUILIST_SetMaxSelectedItem(play_list_user_data_ptr->list_ctrl_id, (uint16)total_num);

                            }
                            GUILIST_SetAllSelected(play_list_user_data_ptr->list_ctrl_id, TRUE);
                        }
                        else
                        {
                            GUISOFTKEY_SetTextId(win_id, MMICOMMON_SOFTKEY_CTRL_ID, TXT_MARK_ALL, TXT_DELETE, STXT_CANCEL, TRUE);
                            GUILIST_SetAllSelected(play_list_user_data_ptr->list_ctrl_id,FALSE);

                        }
                        MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
                        break;
                    }
                    else if(ctrl_id_local == MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID)
                    {
                        GUIFORM_SetChildDisplay(MMIMP3_PLAY_LIST_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
                        GUILIST_SetTextListMarkable(play_list_user_data_ptr->list_ctrl_id, FALSE);
                        GUILIST_SetAllSelected(play_list_user_data_ptr->list_ctrl_id, FALSE);
                        play_list_user_data_ptr->is_mark_all = FALSE;
                        GUISOFTKEY_SetTextId(win_id, MMICOMMON_SOFTKEY_CTRL_ID, TXT_MARK_ALL, TXT_DELETE, STXT_CANCEL, TRUE);
                        MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
                        break;
                    }
                }
            }
            else
#endif
            {
                if(total_num > 0)
                {
                    MMIAPIMP3_StopAudioPlayer();
                    MMIMP3_SetCurPlayInfo(play_list_user_data_ptr->is_music_list,
                                          play_list_user_data_ptr->cur_list_selection,
                                          play_list_user_data_ptr->cur_file_selection,
                                          MMIMP3_GetMp3PlayMode());
                    MMK_CloseWin(win_id);
                    MMK_CloseWin(MMIMP3_PLAY_LIST_LIST_OPT_WIN_ID);
                    MMK_CloseWin(MMIMP3_PLAY_LIST_LIST_WIN_ID);
                    MMK_CloseWin(MMIMP3_PLAYER_OPT_MENU_WIN_ID);
                    MMIAPIMP3_PlayAudioPlayer(MMIMP3_ZERO_OFFSET);
                }
            }
        }
        break;

    case MSG_PROMPTWIN_OK:
        MMK_CloseWin(query_win_id);
        if (PNULL != play_list_user_data_ptr)
        {
            DeleteFileFromList(*play_list_user_data_ptr ,win_id,TRUE);
        }
        MMK_CloseWin(MMIMP3_PLAY_LIST_OPT_WIN_ID);
#ifdef MMI_PDA_SUPPORT
        MMK_CloseWin(MMIMP3_PLAY_LIST_LONGOK_OPT_WIN_ID);
#endif
        break;

    case MSG_PROMPTWIN_CANCEL:
        MMK_CloseWin(query_win_id);
        break;

    case MSG_APP_MENU:
    case MSG_CTL_OK:
#ifdef MMI_PDA_SUPPORT  //NEWMS00169534
        GUIFORM_GetChildDisplay(MMIMP3_PLAY_LIST_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, &display_type);

        if(GUIFORM_CHILD_DISP_NORMAL != display_type)
#endif
        {
            play_list_user_data_ptr->cur_file_selection = GUILIST_GetCurItemIndex(play_list_user_data_ptr->list_ctrl_id);
            OpenPlaylistOptMenu(win_id);
        }

        break;

#ifdef MMI_PDA_SUPPORT

    case MSG_CTL_LIST_LONGOK:
        if(PNULL != play_list_user_data_ptr
                && 0 < MMIMP3_GetListTotalNum(play_list_user_data_ptr->is_music_list,play_list_user_data_ptr->cur_list_selection)
                && !GUILIST_GetListStateById(play_list_user_data_ptr->list_ctrl_id, GUILIST_STATE_ENABLE_MARK)) ////同 NEWMS00169057 保持一致
        {
            play_list_user_data_ptr->cur_file_selection = GUILIST_GetCurItemIndex(play_list_user_data_ptr->list_ctrl_id);
            //长按List弹出context menu菜单
            OpenPlaylistLongOkOptMenu(win_id);
        }

        break;
#endif //MMI_PDA_SUPPORT

    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        if (PNULL != play_list_user_data_ptr)
        {
            MMIMP3_CloseList(play_list_user_data_ptr->is_music_list, play_list_user_data_ptr->cur_list_selection,TRUE);
        }

        if (PNULL != play_list_user_data_ptr)
        {
            SCI_FREE(play_list_user_data_ptr);
        }
        break;

    case MSG_KEYDOWN_RED:
        MMK_CloseWin(win_id);
        result = MMI_RESULT_FALSE;
        break;

    case MSG_MP3_NEW_LIST_ADD_MUSIC_MSG:
        if (PNULL == play_list_user_data_ptr)
        {
            break;
        }

        play_list_user_data_ptr->is_need_add_music = TRUE;
        if(MMK_IsFocusWin(win_id))
        {
            play_list_user_data_ptr->is_need_add_music = FALSE;

            AddFileFromFmmToList(play_list_user_data_ptr->is_music_list,play_list_user_data_ptr->cur_list_selection);
        }
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void SetListWinTitle(MMI_WIN_ID_T    win_id,
                           uint16 cur_list_selection)
{
    MMI_STRING_T        name_no_suffix = {0};

    switch(cur_list_selection)
    {
    case MMIMP3_MY_MUSIC_MENU_INDEX:
        GUIWIN_SetTitleTextId(win_id, TXT_MP3_MY_MUSIC, FALSE);
        break;

#ifdef MMIAP_MUSIC_LIST_SUPPORT
    case MMIMP3_PLAY_LIST_1_INDEX:
        GUIWIN_SetTitleTextId(win_id, TXT_MP3_LIST_1, FALSE);
        break;

    case MMIMP3_PLAY_LIST_2_INDEX:
        GUIWIN_SetTitleTextId(win_id, TXT_MP3_LIST_2, FALSE);
        break;

    case MMIMP3_PLAY_LIST_3_INDEX:
        GUIWIN_SetTitleTextId(win_id, TXT_MP3_LIST_3, FALSE);
        break;
#endif

    default:
#ifdef MMIAP_MUSIC_LIST_SUPPORT
        if(GetListNameWithoutSuffix(cur_list_selection, &name_no_suffix))
        {
            GUIWIN_SetTitleText(win_id, name_no_suffix.wstr_ptr, name_no_suffix.wstr_len, FALSE);

            if (PNULL != name_no_suffix.wstr_ptr)
            {
                SCI_FREE(name_no_suffix.wstr_ptr);
            }
        }
#endif
        break;
    }

}

/*****************************************************************************/
//  Description :Init the control of general play list window.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void InitPlayListCtrl(LIST_WIN_USER_DATA_T      *user_data_ptr)
{
    GUILIST_ITEM_T       item_t    = {0};   /*lint !e64*/
    GUILIST_ITEM_DATA_T  item_data = {0};   /*lint !e64*/
    uint32               i         = 0;
    MMI_HANDLE_T         list_handle = 0;
    uint32               total_num = 0;
    wchar                full_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16               full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    wchar                file_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16               file_name_len = MMIFILE_FULL_PATH_MAX_LEN;

    if (PNULL != user_data_ptr)
    {
        if(APLAYER_LSIT_SORT_NAME ==  MMIMP3_GetListSortType(user_data_ptr->is_music_list,user_data_ptr->cur_list_selection))
        {
#ifdef GUIF_PRGBOX_LETTERSCROLL
            GUILIST_SetListScrollBarStyle(user_data_ptr->list_ctrl_id,GUIPRGBOX_STYLE_LETTERS_SCROLL);
#else
            GUILIST_SetListScrollBarStyle(user_data_ptr->list_ctrl_id,GUIPRGBOX_STYLE_VERTICAL_SCROLL);
#endif
        }
        else
        {
            GUILIST_SetListScrollBarStyle(user_data_ptr->list_ctrl_id,GUIPRGBOX_STYLE_VERTICAL_SCROLL);
        }

        total_num = MMIMP3_GetListTotalNum(user_data_ptr->is_music_list,user_data_ptr->cur_list_selection);

        list_handle = MMK_ConvertIdToHandle(user_data_ptr->list_ctrl_id);

        if(GUILIST_GetTotalItemNum(user_data_ptr->list_ctrl_id))
        {
            GUILIST_RemoveAllItems(user_data_ptr->list_ctrl_id);
        }

        if(0 == total_num)
        {
            GUILIST_SetMaxItem(user_data_ptr->list_ctrl_id, 1, FALSE);
        }
        else
        {
            if(total_num > MMIMP3_LIST_GUI_LIMIT)
            {
                total_num = MMIMP3_LIST_GUI_LIMIT;
            }

            GUILIST_SetMaxItem(user_data_ptr->list_ctrl_id, total_num, TRUE);
        }

        GUILIST_SetTitleIndexType(user_data_ptr->list_ctrl_id, GUILIST_TITLE_INDEX_DEFAULT);
        GUILIST_SetTitleStyle(user_data_ptr->list_ctrl_id, GUILIST_TITLE_ONLY);

        if(0 == total_num)  //将不会再次发送添补list的消息
        {
            GUILIST_SetTitleIndexType(user_data_ptr->list_ctrl_id, GUILIST_TITLE_INDEX_NONE);
            GUILIST_SetTitleStyle(user_data_ptr->list_ctrl_id, GUILIST_TITLE_ONLY);

            item_t.item_data_ptr = &item_data;
            item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT;

            //soft key
            item_data.softkey_id[0] = STXT_OPTION;
            item_data.softkey_id[1] = TXT_NULL;
            item_data.softkey_id[2] = STXT_RETURN;

            // text 1
            item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
            item_data.item_content[0].item_data.text_id = TXT_COMMON_COMMON_EMPTY_LIST;

            GUILIST_AppendItem(list_handle, &item_t);
        }
        else
        {
            item_t.item_style = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;

            for(i = 0; i < total_num; i++)
            {
                MMIMP3_GetAudioFileFullPath(user_data_ptr->is_music_list,user_data_ptr->cur_list_selection,i,full_path,&full_path_len);
                MMIAPIFMM_SplitFullPath(full_path,full_path_len,PNULL,PNULL,PNULL,PNULL,file_name,&file_name_len);
                item_t.first_letter = GetFirstLatterByName (file_name);
                GUILIST_AppendItem(list_handle, &item_t);
            }
        }
    }

}


/*****************************************************************************/
//  Description :Init the control of general play list window.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void InitListCtrl(MMI_CTRL_ID_T ctrl_id,
                        uint32  total_num)
{
    GUILIST_ITEM_T       item_t    = {0};   /*lint !e64*/
    GUILIST_ITEM_DATA_T  item_data = {0};   /*lint !e64*/
    uint32               i         = 0;
    MMI_HANDLE_T         list_handle = 0;


    list_handle = MMK_ConvertIdToHandle(ctrl_id);

    if(GUILIST_GetTotalItemNum(ctrl_id))
    {
        GUILIST_RemoveAllItems(ctrl_id);
    }

    if(0 == total_num)
    {
        GUILIST_SetMaxItem(ctrl_id, 1, FALSE);
    }
    else
    {
        if(total_num > MMIMP3_LIST_GUI_LIMIT)
        {
            total_num = MMIMP3_LIST_GUI_LIMIT;
        }

        GUILIST_SetMaxItem(ctrl_id, total_num, TRUE);
    }

    GUILIST_SetTitleIndexType(ctrl_id, GUILIST_TITLE_INDEX_DEFAULT);
    GUILIST_SetTitleStyle(ctrl_id, GUILIST_TITLE_ONLY);

    if(0 == total_num)  //将不会再次发送添补list的消息
    {
        GUILIST_SetTitleIndexType(ctrl_id, GUILIST_TITLE_INDEX_NONE);
        GUILIST_SetTitleStyle(ctrl_id, GUILIST_TITLE_ONLY);

        item_t.item_data_ptr = &item_data;
        item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT;

        //soft key
        item_data.softkey_id[0] = STXT_OPTION;
        item_data.softkey_id[1] = TXT_NULL;
        item_data.softkey_id[2] = STXT_RETURN;

        // text 1
        item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[0].item_data.text_id = TXT_COMMON_COMMON_EMPTY_LIST;

        GUILIST_AppendItem(list_handle, &item_t);
    }
    else
    {
        item_t.item_style = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;

        for(i = 0; i < total_num; i++)
        {
            GUILIST_AppendItem(list_handle, &item_t);
        }
    }
}



/*****************************************************************************/
//  Description :Init the list item of general play list window.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void InitPlayListItem(
    MMI_CTRL_ID_T   ctrl_id,
    uint16          item_index,
    MMI_TEXT_ID_T   left_softkey_id,
    MMI_TEXT_ID_T   mid_softkey_id,
    MMI_TEXT_ID_T   right_softkey_id,
    BOOLEAN         is_music_list,
    uint16          cur_list_selection
)
{
    GUILIST_ITEM_T       item_t    = {0};       /*lint !e64*/
    GUILIST_ITEM_DATA_T  item_data = {0};       /*lint !e64*/
    wchar                file_name[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    uint16               file_name_len = 0;
    wchar                full_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16               full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    MMIFILE_DEVICE_E     dev_type = MMI_DEVICE_NUM;
#ifdef DRM_SUPPORT
    MMIFMM_FILE_TYPE_E   file_type = MMIFMM_FILE_TYPE_NORMAL;
#endif



    if(!MMIMP3_GetAudioFileFullPath(is_music_list,cur_list_selection,item_index,full_path,&full_path_len))
    {
        return;
    }


    item_t.item_data_ptr = &item_data;

    //soft key
    item_data.softkey_id[0] = left_softkey_id;
    item_data.softkey_id[1] = mid_softkey_id;
    item_data.softkey_id[2] = right_softkey_id;

    //icon
    item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;

    if (MMIMP3_IsCurPlayFile(is_music_list, cur_list_selection, item_index))
    {
        item_data.item_content[0].item_data.image_id = IMAGE_MP3_MUSIC_PLAYING_ICON;
    }
    else
    {

        dev_type = MMIAPIFMM_GetDeviceTypeByPath(full_path, MMIFILE_DEVICE_LEN);
#ifdef DRM_SUPPORT
        file_type = MMIAPIFMM_GetFileTypeByFileName(full_path, full_path_len);

        if(MMIFMM_FILE_TYPE_DRM == file_type)
        {
            item_data.item_content[0].item_data.image_id = MMIAPIFMM_GetDrmIconId(full_path);
        }
        else
#endif
            if(MMI_DEVICE_UDISK == dev_type)
            {
                item_data.item_content[0].item_data.image_id = IMAGE_COMMON_MUSIC_UDISK_ICON;//IMAGE_MP3_FILE_UDISK_ICON;
            }
            else
            {
                item_data.item_content[0].item_data.image_id = IMAGE_COMMON_MUSIC_SD_ICON;//IMAGE_MP3_FILE_SD_ICON;
            }

    }

    MMIAPIFMM_SplitFullPath(full_path,full_path_len,PNULL,PNULL,PNULL,PNULL,file_name,&file_name_len);
    item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[1].item_data.text_buffer.wstr_len = file_name_len;
    item_data.item_content[1].item_data.text_buffer.wstr_ptr = file_name;


    {
        wchar diff_char = 0;
        wchar ucs2_str1[10] = {0};

        //Compare pinyin index if the double ucs2 characters.
        diff_char = *file_name;
        MMIAPIIM_GetTextLetters(diff_char, ucs2_str1);
        item_t.first_letter = ucs2_str1[0];


        GUILIST_SetItemData(ctrl_id, &item_data, item_index);
    }
}



#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : open my music list Long Ok context menu
//  Global resource dependence :
//  Author: xin.li
//  Note: 其他音乐列表长按LIST弹出context menu
/*****************************************************************************/
LOCAL void  OpenPlaylistLongOkOptMenu(MMI_WIN_ID_T         dst_win_id)
{
    MMIPUB_MENU_PARAM_T menuparam = {0};

    menuparam.group_id = MENU_DYNA;
    menuparam.win_id = (MMI_WIN_ID_T)MMIMP3_PLAY_LIST_LONGOK_OPT_WIN_ID;
    menuparam.ctrl_id = (MMI_CTRL_ID_T)MMIMP3_PLAY_LIST_LONGOK_OPT_MENU_CTRL_ID;
    menuparam.applet_handle = SPRD_MP3_APPLET_ID;

    menuparam.dst_handle = dst_win_id;
    menuparam.func.OnMenuOk = HandleListPubMenuOKHandler;
    menuparam.func.OnMenuInit = HandleListPopMenuInitHandler;
    menuparam.menu_style = GUIMENU_STYLE_POPUP_AUTO;
    MMIPUB_OpenPubMenuWin(&menuparam);

}

/*****************************************************************************/
//  Description : handle pub menu win init handler
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandleListPopMenuInitHandler(MMIPUB_MENU_NOTIFY_T *param)
{
    GUIMENU_TITLE_INFO_T    title_info = {0};
    MMI_STRING_T            main_title = {0};
    MMI_STRING_T            sub_title = {0};
    LIST_WIN_USER_DATA_T *list_user_data_ptr = PNULL;

    MMI_SLISTHANDLE_T handle = PNULL;
    MMIAP_MENU_ITEM_T item = {0};


    if (PNULL != param)
    {
        list_user_data_ptr = (LIST_WIN_USER_DATA_T *)MMK_GetWinUserData(param->dst_handle);

        //GUIMENU_CreatDynamic(PNULL, param->win_id, param->ctrl_id, GUIMENU_STYLE_POPUP_AUTO);



        handle = MMIAP_CreateDynamicMenu(sizeof(item));
        if (PNULL != handle)
        {

            if (PNULL != list_user_data_ptr
                    && list_user_data_ptr->is_music_list
                    && MMIMP3_MY_MUSIC_MENU_INDEX != list_user_data_ptr->cur_list_selection
               )
            {
                item.menu_id = ID_MP3_LIST_DEL_FROM_LIST;

                //从播放列表删除
                MMIAP_AddDynamicMenuItem(handle, &item);

            }

            //永久删除
            item.menu_id = ID_MP3_LIST_DEL_REAL;
            MMIAP_AddDynamicMenuItem(handle, &item);

            //设为来电铃音
            item.menu_id = ID_MP3_SET_CALL_RING;
            MMIAP_AddDynamicMenuItem(handle, &item);


            //设为闹钟铃音
            item.menu_id = ID_MP3_SET_ALARM_RING;
            MMIAP_AddDynamicMenuItem(handle, &item);


            //彩信发送
            item.menu_id = ID_MP3_SEND_BY_MMS;
            MMIAP_AddDynamicMenuItem(handle, &item);


            //蓝牙发送
#ifdef BLUETOOTH_SUPPORT
            item.menu_id = ID_MP3_SEND_BY_BT;
            MMIAP_AddDynamicMenuItem(handle, &item);

#endif

            //详情
            item.menu_id = ID_MP3_LIST_DETAIL;
            MMIAP_AddDynamicMenuItem(handle, &item);


            //添加至已存播放列表
            item.menu_id = ID_MP3_LIST_ADD_TO_LIST;
            MMIAP_AddDynamicMenuItem(handle, &item);


            //添加至新建播放列表
            item.menu_id = ID_MP3_ADD_TO_NEW_LIST;
            MMIAP_AddDynamicMenuItem(handle, &item);


            MMIAP_BuildDynamicMenuToCtrl(handle, PNULL, FALSE, param->ctrl_id);

            MMIAP_DestroyDynamicMenu(handle);
        }

        //set title
        if (PNULL != list_user_data_ptr)
        {
            MMIAP_GetListItemTextInfo(list_user_data_ptr->list_ctrl_id, list_user_data_ptr->cur_file_selection, &main_title, &sub_title);
            title_info.is_static    = FALSE;
            title_info.title_ptr    = &main_title;
            title_info.sub_title_ptr = &sub_title;
            GUIMENU_SetMenuTitleEx(param->ctrl_id, &title_info);
        }

        MMK_SetAtvCtrl(param->win_id, param->ctrl_id);
        GUIWIN_SetSoftkeyTextId(param->win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
    }
}

#endif
/*****************************************************************************/
//  Description :  Handle list add to waiting win
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLoadMusicFilesWaitingWin(
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
)
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    LIST_LOAD_MUSIC_T *list_info_ptr = (LIST_LOAD_MUSIC_T *)MMK_GetWinUserData(win_id);
    MMI_WIN_ID_T        alert_win_id = MMIMP3_ALERT_WIN_ID;
    APLAYER_LIST_ERROR_E  save_result = APLAYER_LIST_ERROR;

    SCI_TRACE_LOW("[AP MULTILIST] HandleLoadMusicFilesWaitingWin msg_id = %d", msg_id);

    switch(msg_id)
    {
    case MSG_APP_CANCEL:
        MMIAPIFMM_SearchFileStop();
        break;

    case MSG_APP_RED:
        MMIAPIFMM_SearchFileStop();
        result = MMI_RESULT_FALSE;
        break;


    case MSG_MP3_BEGIN_ADD_TO_LIST:
        if (PNULL == list_info_ptr
                || !MMIMP3_SearchAllAudioFiles(list_info_ptr->is_music_list,list_info_ptr->list_selection,win_id,MSG_MP3_LOAD_ALL_MUSIC_FINISH))

        {
            MMIPUB_OpenAlertWinByTextId(PNULL, TXT_ERROR, TXT_NULL,
                                        IMAGE_PUBWIN_FAIL, &alert_win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
            MMK_CloseWin(win_id);
        }
        break;

    case MSG_MP3_LOAD_ALL_MUSIC_FINISH:
        if (PNULL != list_info_ptr)
        {
            save_result = MMIMP3_SaveAllAudioFilesToList(list_info_ptr->is_music_list,list_info_ptr->list_selection);
            if (APLAYER_LIST_ERROR == save_result)
            {
                MMIPUB_OpenAlertWinByTextId(PNULL, TXT_ERROR, TXT_NULL,
                                            IMAGE_PUBWIN_FAIL, &alert_win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
            }
            else if (APLAYER_LIST_NO_SAPCE == save_result)
            {
                MMIPUB_OpenAlertWinByTextId(PNULL, TXT_NO_SPACE,
                                            TXT_NULL, IMAGE_PUBWIN_WARNING, &alert_win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
            }
            MMK_SendMsg(MMIMP3_PLAY_LIST_WIN_ID, MSG_MP3_UPDATE_LIST, PNULL);
        }

        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        if (PNULL != list_info_ptr)
        {
            SCI_FREE(list_info_ptr);
        }
        result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;

    default:
        result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    }

    return result;
}
/*****************************************************************************/
//  Description : OpenLoadAllMusicFilesWaitingWin
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN OpenLoadAllMusicFilesWaitingWin(BOOLEAN is_music_list,
        uint16  list_selection,
        MMI_HANDLE_T win_handle)
{
    LIST_LOAD_MUSIC_T *list_info_ptr = PNULL;
    MMI_WIN_ID_T    waiting_win_id = MMIMP3_ADD_TO_WAIT_WIN_ID;
    uint32          waiting_time_out = 0;

    list_info_ptr = SCI_ALLOCA(sizeof(LIST_LOAD_MUSIC_T));

    if (PNULL != list_info_ptr)
    {
        SCI_MEMSET(list_info_ptr,0,sizeof(LIST_LOAD_MUSIC_T));

        list_info_ptr->is_music_list = is_music_list;
        list_info_ptr->list_selection = list_selection;
        list_info_ptr->win_handle = win_handle;

        MMIPUB_OpenAlertWinByTextIdEx(SPRD_MP3_APPLET_ID,
                                      &waiting_time_out, TXT_COMMON_WAITING, TXT_NULL,
                                      IMAGE_PUBWIN_WAIT, &waiting_win_id, PNULL, MMIPUB_SOFTKEY_NONE, HandleLoadMusicFilesWaitingWin,
                                      PNULL);

        MMK_SetWinUserData(waiting_win_id,list_info_ptr);
        MMK_PostMsg(waiting_win_id, MSG_MP3_BEGIN_ADD_TO_LIST, PNULL,PNULL);
        return TRUE;

    }

    return FALSE;

}

/*****************************************************************************/
//  Description : handle pub menu win init handler
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandleListPubMenuOKHandler(MMIPUB_MENU_NOTIFY_T *param)
{
    MMI_MENU_ID_T       menu_id = ID_MP3_EXIT;
    MMI_WIN_ID_T        query_win_id = MMIMP3_DEL_QUERY_WIN_ID;
    LIST_WIN_USER_DATA_T *user_data_ptr = PNULL;
    MMI_WIN_ID_T        alert_win_id = MMIMP3_ALERT_WIN_ID;


    if (PNULL != param)
    {
        user_data_ptr = (LIST_WIN_USER_DATA_T *)MMK_GetWinUserData(param->dst_handle);
        menu_id = GUIMENU_GetCurNodeId(param->ctrl_id);

        switch(menu_id)
        {
        case ID_MP3_LIST_OPT_RETURN:
            MMK_ReturnAppointedWin(MMIMP3_MAIN_PLAY_WIN_ID);
            break;

        case ID_MP3_LIST_DEL_FROM_LIST:
            if (PNULL != user_data_ptr)
            {
                DeleteFileFromList(*user_data_ptr,param->dst_handle,FALSE);
            }
            break;


        case ID_MP3_LIST_DEL_REAL:
            MMIPUB_OpenQueryWinByTextIdEx(SPRD_MP3_APPLET_ID,
                                          TXT_MP3_DEL_REAL_MUSIC_QUERY, IMAGE_PUBWIN_QUERY, &query_win_id, PNULL,
                                          PNULL);
            break;

#ifdef MMI_PDA_SUPPORT
        case ID_MP3_LIST_MARK:
            GUIFORM_SetChildDisplay(MMIMP3_PLAY_LIST_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_NORMAL);
            GUILIST_SetTextListMarkable(user_data_ptr->list_ctrl_id, TRUE);
            GUILIST_SetMaxSelectedItem(user_data_ptr->list_ctrl_id, MMIMP3_GetListTotalNum(user_data_ptr->is_music_list,user_data_ptr->cur_list_selection ));
            user_data_ptr->is_mark_all = FALSE;
            GUISOFTKEY_SetTextId(param->dst_handle, MMICOMMON_SOFTKEY_CTRL_ID, TXT_MARK_ALL, TXT_DELETE, STXT_CANCEL, TRUE);
            break;
#endif

        case ID_MP3_SUB_MARK:
            if (PNULL != user_data_ptr)
            {
                GUILIST_SetTextListMarkable(user_data_ptr->list_ctrl_id, TRUE);
                GUILIST_SetMaxSelectedItem(user_data_ptr->list_ctrl_id, MMIMP3_GetListTotalNum(user_data_ptr->is_music_list,user_data_ptr->cur_list_selection ));
                GUILIST_SetSelectedItem(user_data_ptr->list_ctrl_id, user_data_ptr->cur_file_selection, TRUE);
            }
            break;

        case ID_MP3_CANCEL_MARK:
            if (PNULL != user_data_ptr)
            {
                GUILIST_SetSelectedItem(user_data_ptr->list_ctrl_id, user_data_ptr->cur_file_selection, FALSE);

                if(0 == GUILIST_GetSelectedItemIndex(user_data_ptr->list_ctrl_id, PNULL, 0))
                {
                    GUILIST_SetTextListMarkable(user_data_ptr->list_ctrl_id, FALSE);
                }

            }
            break;

        case ID_MP3_MARK_ALL:
            if (PNULL != user_data_ptr)
            {
                GUILIST_SetTextListMarkable(user_data_ptr->list_ctrl_id, TRUE);
                GUILIST_SetMaxSelectedItem(user_data_ptr->list_ctrl_id, (uint16)MMIMP3_GetListTotalNum(user_data_ptr->is_music_list,user_data_ptr->cur_list_selection ));
                GUILIST_SetAllSelected(user_data_ptr->list_ctrl_id, TRUE);
            }
            break;

        case ID_MP3_CANCEL_ALL_MARK:
            if (PNULL != user_data_ptr)
            {
                GUILIST_SetTextListMarkable(user_data_ptr->list_ctrl_id, FALSE);
            }
            break;


        case ID_MP3_SET_CALL_RING:
        case ID_MP3_SET_ALARM_RING:
            if (PNULL != user_data_ptr)
            {
                SetRing(menu_id, user_data_ptr->is_music_list,user_data_ptr->cur_list_selection,user_data_ptr->cur_file_selection);
            }
            break;


#ifdef BLUETOOTH_SUPPORT
        case ID_MP3_SEND_BY_BT:
#endif
        case ID_MP3_SEND_BY_MMS:
            if (PNULL != user_data_ptr)
            {
                if(!SendAudioFile(menu_id,user_data_ptr->is_music_list,user_data_ptr->cur_list_selection, user_data_ptr->cur_file_selection))
                {
                    MMIPUB_OpenAlertWinByTextId(PNULL, TXT_COM_FILE_NO_EXIST, TXT_NULL,
                                                IMAGE_PUBWIN_WARNING, &alert_win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
                }
            }
            break;


        case ID_MP3_LIST_DETAIL:
            if (PNULL != user_data_ptr)
            {
                if (!OpenDetailWin(user_data_ptr->is_music_list,user_data_ptr->cur_list_selection, user_data_ptr->cur_file_selection))
                {
                    MMIPUB_OpenAlertWinByTextId(PNULL, TXT_COM_FILE_NO_EXIST, TXT_NULL,
                                                IMAGE_PUBWIN_WARNING, &alert_win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
                }
            }
            break;

#ifdef MMIAP_MUSIC_LIST_SUPPORT
        case ID_MP3_LIST_ADD:
            if (PNULL != user_data_ptr)
            {
                AddFileFromFmmToList(user_data_ptr->is_music_list,user_data_ptr->cur_list_selection);
            }
            break;

        case ID_MP3_LIST_SEARCH_ALL:
            if (PNULL != user_data_ptr)
            {
                OpenLoadAllMusicFilesWaitingWin(user_data_ptr->is_music_list,user_data_ptr->cur_list_selection,param->dst_handle);
            }
            break;

        case ID_MP3_LIST_ADD_TO_LIST:
        case ID_MP3_ADD_TO_NEW_LIST:
            if(PNULL != user_data_ptr
                    && !AddFilesFromListToList(user_data_ptr,menu_id,param->dst_handle)
              )
            {
                MMIPUB_OpenAlertWinByTextId(PNULL, TXT_MP3_CUR_LIST_NOT_SUPPORT_OPERATION, TXT_NULL,
                                            IMAGE_PUBWIN_WARNING, &alert_win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
#ifdef MMI_PDA_SUPPORT
                GUIFORM_SetChildDisplay(MMIMP3_PLAY_LIST_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
#endif
                GUILIST_SetTextListMarkable(user_data_ptr->list_ctrl_id, FALSE);
            }

            break;
#endif

#ifdef MMIAP_SORT_SUPPORT

        case ID_MP3_LIST_SORT_BY_NAME:
            if (PNULL != user_data_ptr)
            {
                MMIMP3_SortList(user_data_ptr->is_music_list,user_data_ptr->cur_list_selection,APLAYER_LSIT_SORT_NAME);
                MMK_SendMsg(param->dst_handle,MSG_MP3_UPDATE_LIST,PNULL);
            }
            break;

        case ID_MP3_LIST_SORT_BY_TIME:
            if (PNULL != user_data_ptr)
            {
                MMIMP3_SortList(user_data_ptr->is_music_list, user_data_ptr->cur_list_selection,APLAYER_LSIT_SORT_TIME);
                MMK_SendMsg(param->dst_handle,MSG_MP3_UPDATE_LIST,PNULL);
            }
            break;

        case ID_MP3_LIST_SORT_BY_SIZE:
            if (PNULL != user_data_ptr)
            {
                MMIMP3_SortList(user_data_ptr->is_music_list, user_data_ptr->cur_list_selection,APLAYER_LSIT_SORT_SIZE);
                MMK_SendMsg(param->dst_handle,MSG_MP3_UPDATE_LIST,PNULL);
            }
            break;

        case ID_MP3_LIST_SORT_BY_TYPE:
            if (PNULL != user_data_ptr)
            {
                MMIMP3_SortList(user_data_ptr->is_music_list, user_data_ptr->cur_list_selection,APLAYER_LSIT_SORT_TYPE);
                MMK_SendMsg(param->dst_handle,MSG_MP3_UPDATE_LIST,PNULL);
            }
            break;

        case ID_MP3_LIST_SORT_BY_NONE:
            if (PNULL != user_data_ptr)
            {
                MMIMP3_SortList(user_data_ptr->is_music_list, user_data_ptr->cur_list_selection,APLAYER_LSIT_SORT_NULL);
                MMK_SendMsg(param->dst_handle,MSG_MP3_UPDATE_LIST,PNULL);
            }
            break;
#endif

        default:
            break;
        }
    }
}

/*****************************************************************************/
//  Description : CanbeSetRing
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CanbeSetRing(BOOLEAN is_music_list,
                           uint16 list_index,
                           uint16 file_index)
{
    BOOLEAN result = TRUE;
#ifdef DRM_SUPPORT
    wchar full_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16 full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    MMI_WIN_ID_T        alert_win_id = MMIMP3_ALERT_WIN_ID;

    if(MMIMP3_GetAudioFileFullPath(is_music_list,list_index,file_index, full_path,&full_path_len))
    {
        if(!MMIAPIDRM_CanDrmFileSetToWSPC(full_path, full_path_len, DRMFILE_SETTING_CALLRING))
        {
            MMIPUB_OpenAlertWinByTextId(PNULL, TXT_DRM_NOT_SET_RINGTONE, TXT_NULL, IMAGE_PUBWIN_WARNING, &alert_win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
            result = FALSE;
        }
    }
    else
    {
        MMIMP3_FatalError();
        result = FALSE;
    }
#endif

    return result;
}

/*****************************************************************************/
//  Description : SetCallRing
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetRing(MMI_MENU_ID_T menu_id,
                      BOOLEAN is_music_list,
                      uint16 cur_list_selection,
                      uint16 cur_file_selection)
{
    BOOLEAN result = TRUE;
    wchar full_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16 full_path_len = MMIFILE_FULL_PATH_MAX_LEN;


    if(CanbeSetRing(is_music_list,cur_list_selection,cur_file_selection)
            && MMIMP3_GetAudioFileFullPath(is_music_list,cur_list_selection,cur_file_selection, full_path,&full_path_len))
    {
        if (ID_MP3_SET_ALARM_RING == menu_id)
        {
            MMIAPSETWIN_OpenSetAlarmWin(full_path,full_path_len);
        }
        else
        {

#ifndef MMI_MULTI_SIM_SYS_SINGLE
            MMIAPISET_OpenUssdSelectWin(MMISET_MP3_PLAYER_CALL_RING);
#else
            MMIAPIMP3_SetMp3AsCallRing(MN_DUAL_SYS_1);
#endif
        }


    }
    else
    {
        result = FALSE;
    }
    return result;

}

/*****************************************************************************/
//  Description : SendAudioFile
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SendAudioFile(MMI_MENU_ID_T menu_id,
                            BOOLEAN is_music_list,
                            uint16 cur_list_selection,
                            uint16 cur_file_selection)
{
    BOOLEAN result = TRUE;
    wchar full_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16 full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    MMIFILE_FILE_INFO_T fmm_file_info = {0};

#ifdef BLUETOOTH_SUPPORT
    MMIFMM_BT_SENDFILE_INFO_T send_file_info = {0};
    uint32      file_size  = 0;
#endif


    if(MMIMP3_GetAudioFileFullPath(is_music_list,cur_list_selection,cur_file_selection, full_path,&full_path_len))
    {
        if (ID_MP3_SEND_BY_MMS == menu_id)
        {
            if(MMIAPIFMM_GetFileInfoFormFullPath(full_path, full_path_len, &fmm_file_info))
            {
                MMIAPIMMS_EditMMSFromFile(&fmm_file_info, MMIFMM_FILE_TYPE_MUSIC);
            }
            else
            {
                result = FALSE;
            }
        }
#ifdef BLUETOOTH_SUPPORT
        else
        {
            if(MMIAPIFMM_GetFileInfo(full_path, full_path_len, &file_size, NULL, NULL))
            {
                MMIAPICOM_Wstrncpy((wchar*)send_file_info.filepath_name, full_path, full_path_len);
                send_file_info.filepath_len = full_path_len;
                send_file_info.file_size = file_size;
                send_file_info.is_temp_file = FALSE;
                MMIAPIBT_SendMultiFile(&send_file_info, 1);
            }
            else
            {
                result = FALSE;
            }

        }
#endif

    }
    else
    {
        MMIMP3_FatalError();
        result = FALSE;
    }

    return result;
}

/*****************************************************************************/
//  Description : OpenDetailWin
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN OpenDetailWin(BOOLEAN is_music_list,
                            uint16 cur_list_selection,
                            uint16 cur_file_selection)
{
    BOOLEAN result = FALSE;
    wchar  full_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16 full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    MMIFMM_DETAIL_DATA_T detail_data = {0};
    SFS_DATE_T modify_date = {0};
    SFS_TIME_T modify_time = {0};


    if(MMIMP3_GetAudioFileFullPath(is_music_list,cur_list_selection,cur_file_selection,full_path,&full_path_len)
            && MMIAPIFMM_IsFileExist(full_path, full_path_len))
    {
        MMIAPIFMM_GetFileInfo(full_path,full_path_len,&(detail_data.file_size),&modify_date,&modify_time);

        detail_data.time = MMIAPICOM_Tm2Second(modify_time.sec, modify_time.min, modify_time.hour, modify_date.mday, modify_date.mon, modify_date.year);
        detail_data.filename_len = MIN(MMIFMM_FILENAME_LEN, full_path_len);
        MMIAPICOM_Wstrncpy(detail_data.filename, full_path, detail_data.filename_len);

#ifdef DRM_SUPPORT
        if(MMIAPIDRM_IsDRMFile(PNULL, detail_data.filename))
        {
            detail_data.type = MMIFMM_FILE_TYPE_DRM;
        }
        else
#endif
        {
            detail_data.type = MMIFMM_FILE_TYPE_MUSIC;
        }

        MMIAPIFMM_DetailFileData(&detail_data);
        result = TRUE;
    }
    return result;

}

/*****************************************************************************/
//  Description : MMIAPMULTILIST_AddCurFileToList
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPMULTILIST_AddCurFileToList(BOOLEAN is_create_new_list)
{
    MMIMP3_LIST_ADD_TO_INFO_T list_add_info = {0};
    BOOLEAN  result = FALSE;
    MMIMP3_PLAY_LIST_INFO_T cur_list_info = MMIMP3_GetCurPlayListInfo();
	uint32          total_num = 0;
	MMI_WIN_ID_T        alert_win_id = MMIMP3_ALERT_WIN_ID;


    list_add_info.src_is_music_list = cur_list_info.is_music_list;
    list_add_info.src_list_index = cur_list_info.cur_list_index;
    list_add_info.select_index_ptr = CreateIndexList(PNULL,&(list_add_info.select_num),MMIMP3_GetCurPlayFileIndex());
    list_add_info.win_handle = PNULL;
    if (PNULL != list_add_info.select_index_ptr)
    {
        if (!is_create_new_list)
        {
            list_add_info.is_new_list = FALSE;
            EnterAddToListWin(&list_add_info);
        }
        else
        {
            list_add_info.is_new_list = TRUE;
			total_num    = MMIMP3_GetTotalListNum();
			if(total_num >= AP_MUSIC_LIST_NUM_MAX)
			{
				MMIPUB_OpenAlertWinByTextId(PNULL, TXT_NO_SPACE,//sd 空间不足
					TXT_NULL, IMAGE_PUBWIN_WARNING, &alert_win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
			}
			else
			{	
	            EnterCreateNewListWin(&list_add_info);
			}

        }
        DestoryIndexList(list_add_info.select_index_ptr);
        result = TRUE;
    }

    return result;

}

/*****************************************************************************/
//  Description : AddFilesToList
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AddFilesFromListToList(LIST_WIN_USER_DATA_T *user_data_ptr,
                                     MMI_MENU_ID_T menu_id,
                                     MMI_HANDLE_T win_handle)
{
    MMIMP3_LIST_ADD_TO_INFO_T list_add_info = {0};
    BOOLEAN  result = FALSE;

    if (PNULL != user_data_ptr)
    {
        list_add_info.src_is_music_list = user_data_ptr->is_music_list;
        list_add_info.src_list_index = user_data_ptr->cur_list_selection;
        list_add_info.select_index_ptr = CreateIndexList(user_data_ptr->list_ctrl_id,&(list_add_info.select_num),user_data_ptr->cur_file_selection);
        list_add_info.win_handle = win_handle;
        if (PNULL != list_add_info.select_index_ptr)
        {
            if (ID_MP3_ADD_TO_NEW_LIST != menu_id)
            {
                list_add_info.is_new_list = FALSE;
                EnterAddToListWin(&list_add_info);
            }
            else
            {
                list_add_info.is_new_list = TRUE;
                EnterCreateNewListWin(&list_add_info);
            }
            DestoryIndexList(list_add_info.select_index_ptr);
            result = TRUE;
        }
    }

    return result;
}




/*****************************************************************************/
//  Description : handle messages of the add to list window.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMp3AddToListWinMsg(
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
)
{
    MMI_RESULT_E    result  = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIMP3_ADD_TO_LIST_CTRL_ID;
    GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr = PNULL;
    MMIMP3_LIST_ADD_TO_INFO_T * user_data_ptr = (MMIMP3_LIST_ADD_TO_INFO_T *)MMK_GetWinUserData(win_id);
    uint32          total_list_num = MMIMP3_GetTotalListNum();

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        if (total_list_num > 0)
        {
            total_list_num--;
        }
        else
        {
            MMK_CloseWin(win_id);
            break;
        }
        InitListCtrl(ctrl_id, total_list_num); //no my music
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_CTL_LIST_NEED_ITEM_DATA:
        need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;
        if(PNULL == need_item_data_ptr)
        {
            MMIMP3_FatalError();
            break;
        }
        InitAllPlayListsItem(ctrl_id,FALSE,need_item_data_ptr->item_index,TXT_COMMON_OK,TXT_NULL);
        break;

    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        if (PNULL != user_data_ptr)
        {
            user_data_ptr->dst_list_index = GUILIST_GetCurItemIndex(ctrl_id)+1;
            OpenAddFileToListWaitingWin(user_data_ptr);
        }

#ifdef MMI_PDA_SUPPORT
        GUIFORM_SetChildDisplay(MMIMP3_PLAY_LIST_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
#endif
        GUILIST_SetTextListMarkable(MMIMP3_PLAY_LIST_CTRL_ID, FALSE);
        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        if (PNULL != user_data_ptr)
        {
            if (PNULL != user_data_ptr->select_index_ptr)
            {
                SCI_FREE(user_data_ptr->select_index_ptr);
            }
            SCI_FREE(user_data_ptr);
        }
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}



/*****************************************************************************/
//  Description : open opt menu
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void  OpenPlaylistOptMenu(MMI_WIN_ID_T         dst_win_id)
{
    MMIPUB_MENU_PARAM_T menuparam = {0};

    menuparam.group_id = MENU_DYNA;
    menuparam.win_id = (MMI_WIN_ID_T)MMIMP3_PLAY_LIST_OPT_WIN_ID;
    menuparam.ctrl_id = (MMI_CTRL_ID_T)MMIMP3_PLAY_LIST_OPT_MENU_CTRL_ID;
    menuparam.applet_handle = SPRD_MP3_APPLET_ID;

    menuparam.dst_handle = dst_win_id;
    menuparam.func.OnMenuOk = HandleListPubMenuOKHandler;
    menuparam.func.OnMenuInit = HandleListPubMenuInitHandler;
    menuparam.menu_style = GUIMENU_STYLE_POPUP;
    MMIPUB_OpenPubMenuWin(&menuparam);
}

/*****************************************************************************/
//  Description : handle pub menu win init handler
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandleListPubMenuInitHandler(MMIPUB_MENU_NOTIFY_T *param)
{

    LIST_WIN_USER_DATA_T *list_user_data_ptr = PNULL;

    MMI_SLISTHANDLE_T handle = PNULL;
    MMIAP_MENU_ITEM_T item = {0};
    BOOLEAN is_display_image = FALSE;
#ifdef GUI_FULLSCREEN_POPMENU_SUPPORT
	MMI_STRING_T title_text = {0};
#endif

    if (PNULL != param)
    {
        list_user_data_ptr = (LIST_WIN_USER_DATA_T *)MMK_GetWinUserData(param->dst_handle);
        GUIMENU_CreatDynamic(PNULL, param->win_id, param->ctrl_id, GUIMENU_STYLE_POPUP);

        //opt menu
        handle = MMIAP_CreateDynamicMenu(sizeof(item));
        if (PNULL != handle)
        {

#ifndef MMI_PDA_SUPPORT
#ifndef MMI_AUDIOPLAYER_OPT_MINI_SUPPORT

            SCI_MEMSET(&item, 0, sizeof(item));
            item.menu_id = ID_MP3_LIST_OPT_RETURN;
            MMIAP_AddDynamicMenuItem(handle, &item);


            if (PNULL != list_user_data_ptr
                    && list_user_data_ptr->is_music_list
                    && MMIMP3_MY_MUSIC_MENU_INDEX != list_user_data_ptr->cur_list_selection)
            {
                SCI_MEMSET(&item, 0, sizeof(item));
                item.menu_id = ID_MP3_LIST_ADD;
                MMIAP_AddDynamicMenuItem(handle, &item);


#ifdef MMIAP_F_FIRST_SEARCH
                SCI_MEMSET(&item, 0, sizeof(item));
                item.menu_id = ID_MP3_LIST_SEARCH_ALL;
                MMIAP_AddDynamicMenuItem(handle, &item);
#endif

                SCI_MEMSET(&item, 0, sizeof(item));
                item.menu_id = ID_MP3_LIST_DEL;
                MMIAP_AddDynamicMenuItem(handle, &item);
            }
            else
            {
                SCI_MEMSET(&item, 0, sizeof(item));
                item.menu_id = ID_MP3_LIST_DEL_REAL;
                MMIAP_AddDynamicMenuItem(handle, &item);
            }

            SCI_MEMSET(&item, 0, sizeof(item));
            item.menu_id = ID_MP3_LIST_MARK;
            MMIAP_AddDynamicMenuItem(handle, &item);
#endif

            SCI_MEMSET(&item, 0, sizeof(item));
            item.menu_id = ID_MP3_SET_RING;
            MMIAP_AddDynamicMenuItem(handle, &item);

            SCI_MEMSET(&item, 0, sizeof(item));
            item.menu_id = ID_MP3_LIST_SEND;
            MMIAP_AddDynamicMenuItem(handle, &item);

            SCI_MEMSET(&item, 0, sizeof(item));
            item.menu_id = ID_MP3_LIST_DETAIL;
            MMIAP_AddDynamicMenuItem(handle, &item);


#ifdef MMIAP_MUSIC_LIST_SUPPORT
            SCI_MEMSET(&item, 0, sizeof(item));
            item.menu_id = ID_MP3_ADD;
            MMIAP_AddDynamicMenuItem(handle, &item);
#endif

#ifdef MMIAP_SORT_SUPPORT
            SCI_MEMSET(&item, 0, sizeof(item));
            item.menu_id = ID_MP3_LIST_SORT;
            MMIAP_AddDynamicMenuItem(handle, &item);
#endif

            //sub menu
            if (PNULL != list_user_data_ptr
                    && (MMIMP3_MY_MUSIC_MENU_INDEX != list_user_data_ptr->cur_list_selection
                        || !list_user_data_ptr->is_music_list))
            {

                SCI_MEMSET(&item, 0, sizeof(item));
                item.menu_id = ID_MP3_LIST_DEL_FROM_LIST;
                item.parent_id = ID_MP3_LIST_DEL;
                MMIAP_AddDynamicMenuItem(handle, &item);


                SCI_MEMSET(&item, 0, sizeof(item));
                item.menu_id = ID_MP3_LIST_DEL_REAL;
                item.parent_id = ID_MP3_LIST_DEL;
                MMIAP_AddDynamicMenuItem(handle, &item);
            }

            //sub menu
            SCI_MEMSET(&item, 0, sizeof(item));
            item.menu_id = ID_MP3_SET_CALL_RING;
            item.parent_id = ID_MP3_SET_RING;
            MMIAP_AddDynamicMenuItem(handle, &item);


            SCI_MEMSET(&item, 0, sizeof(item));
            item.menu_id = ID_MP3_SET_ALARM_RING;
            item.parent_id = ID_MP3_SET_RING;
            MMIAP_AddDynamicMenuItem(handle, &item);

#ifdef MMS_SUPPORT
            SCI_MEMSET(&item, 0, sizeof(item));
            item.menu_id = ID_MP3_SEND_BY_MMS;
            item.parent_id = ID_MP3_LIST_SEND;
            MMIAP_AddDynamicMenuItem(handle, &item);
#endif


#ifdef BLUETOOTH_SUPPORT
            SCI_MEMSET(&item, 0, sizeof(item));
            item.menu_id = ID_MP3_SEND_BY_BT;
            item.parent_id = ID_MP3_LIST_SEND;
            MMIAP_AddDynamicMenuItem(handle, &item);

#endif



#ifdef MMIAP_MUSIC_LIST_SUPPORT
            //sub menu
            SCI_MEMSET(&item, 0, sizeof(item));
            item.menu_id = ID_MP3_LIST_ADD_TO_LIST;
            item.parent_id = ID_MP3_ADD;
            MMIAP_AddDynamicMenuItem(handle, &item);

            SCI_MEMSET(&item, 0, sizeof(item));
            item.menu_id = ID_MP3_ADD_TO_NEW_LIST;
            item.parent_id = ID_MP3_ADD;
            MMIAP_AddDynamicMenuItem(handle, &item);
#endif
#ifdef MMIAP_SORT_SUPPORT
            //sub menu
            SCI_MEMSET(&item, 0, sizeof(item));
            item.menu_id = ID_MP3_LIST_SORT_BY_NONE;
            item.parent_id = ID_MP3_LIST_SORT;
            MMIAP_AddDynamicMenuItem(handle, &item);


            SCI_MEMSET(&item, 0, sizeof(item));
            item.menu_id = ID_MP3_LIST_SORT_BY_TIME;
            item.parent_id = ID_MP3_LIST_SORT;
            MMIAP_AddDynamicMenuItem(handle, &item);


            SCI_MEMSET(&item, 0, sizeof(item));
            item.menu_id = ID_MP3_LIST_SORT_BY_NAME;
            item.parent_id = ID_MP3_LIST_SORT;
            MMIAP_AddDynamicMenuItem(handle, &item);
#endif
            //sub menu
            SCI_MEMSET(&item, 0, sizeof(item));
            item.menu_id = ID_MP3_SUB_MARK;
            item.parent_id = ID_MP3_LIST_MARK;
            MMIAP_AddDynamicMenuItem(handle, &item);


            SCI_MEMSET(&item, 0, sizeof(item));
            item.menu_id = ID_MP3_CANCEL_MARK;
            item.parent_id = ID_MP3_LIST_MARK;
            MMIAP_AddDynamicMenuItem(handle, &item);

            SCI_MEMSET(&item, 0, sizeof(item));
            item.menu_id = ID_MP3_MARK_ALL;
            item.parent_id = ID_MP3_LIST_MARK;
            MMIAP_AddDynamicMenuItem(handle, &item);


            SCI_MEMSET(&item, 0, sizeof(item));
            item.menu_id = ID_MP3_CANCEL_ALL_MARK;
            item.parent_id = ID_MP3_LIST_MARK;
            MMIAP_AddDynamicMenuItem(handle, &item);

            is_display_image = FALSE;

#else   //PDA style menu

            if (PNULL != list_user_data_ptr
                    && list_user_data_ptr->is_music_list
                    && MMIMP3_MY_MUSIC_MENU_INDEX != list_user_data_ptr->cur_list_selection)
            {
                //添加曲目
                SCI_MEMSET(&item, 0, sizeof(item));
                item.menu_id = ID_MP3_LIST_ADD;
                MMIAP_AddDynamicMenuItem(handle, &item);



                //搜索全部曲目
                SCI_MEMSET(&item, 0, sizeof(item));
                item.menu_id = ID_MP3_LIST_SEARCH_ALL;
                MMIAP_AddDynamicMenuItem(handle, &item);
            }


#ifdef MMIAP_F_FIRST_SEARCH
            if (PNULL != list_user_data_ptr
                    && list_user_data_ptr->is_music_list
                    && MMIMP3_MY_MUSIC_MENU_INDEX == list_user_data_ptr->cur_list_selection)
            {
                //搜索全部曲目
                SCI_MEMSET(&item, 0, sizeof(item));
                item.menu_id = ID_MP3_LIST_SEARCH_ALL;
                MMIAP_AddDynamicMenuItem(handle, &item);
            }
#endif


            //标记
            SCI_MEMSET(&item, 0, sizeof(item));
            item.menu_id = ID_MP3_LIST_MARK;
            MMIAP_AddDynamicMenuItem(handle, &item);

#ifdef MMIAP_SORT_SUPPORT
            //排序
            SCI_MEMSET(&item, 0, sizeof(item));
            item.menu_id = ID_MP3_LIST_SORT;
            MMIAP_AddDynamicMenuItem(handle, &item);


            //排序 sub menu
            SCI_MEMSET(&item, 0, sizeof(item));
            item.menu_id = ID_MP3_LIST_SORT_BY_TIME;
            item.parent_id = ID_MP3_LIST_SORT;
            MMIAP_AddDynamicMenuItem(handle, &item);

            SCI_MEMSET(&item, 0, sizeof(item));
            item.menu_id = ID_MP3_LIST_SORT_BY_SIZE;
            item.parent_id = ID_MP3_LIST_SORT;
            MMIAP_AddDynamicMenuItem(handle, &item);


            SCI_MEMSET(&item, 0, sizeof(item));
            item.menu_id = ID_MP3_LIST_SORT_BY_NAME;
            item.parent_id = ID_MP3_LIST_SORT;
            MMIAP_AddDynamicMenuItem(handle, &item);
#endif

            is_display_image = TRUE;
#endif //MMI_PDA_SUPPORT


            MMIAP_BuildDynamicMenuToCtrl(handle, PNULL, is_display_image, param->ctrl_id);
            MMIAP_DestroyDynamicMenu(handle);
        }


#ifdef MMIAP_SORT_SUPPORT
#ifdef MMI_PDA_SUPPORT//NEWMS00160343
        GUIMENU_SetSubMenuStyle(param->ctrl_id, ID_MP3_LIST_SORT,  GUIMENU_STYLE_POPUP);
#else
        GUIMENU_SetSubMenuStyle(param->ctrl_id, ID_MP3_LIST_SORT,  GUIMENU_STYLE_POPUP_RADIO);
#endif

        if (PNULL != list_user_data_ptr)
        {
            SetSortTypeSelected(param->ctrl_id, list_user_data_ptr->cur_list_selection, list_user_data_ptr->is_music_list);
        }
#endif
        if (PNULL != list_user_data_ptr)
        {
            SetPlaylistOptMenuGreyStatus(list_user_data_ptr->is_music_list,
                                         list_user_data_ptr->cur_list_selection,
                                         list_user_data_ptr->cur_file_selection,
                                         list_user_data_ptr->list_ctrl_id,
                                         param->ctrl_id);
        }
#ifdef GUI_FULLSCREEN_POPMENU_SUPPORT
		MMI_GetLabelTextByLang(STXT_OPTION, &title_text);
		GUIMENU_SetMenuTitle( &title_text,param->ctrl_id );
#endif

    }
}


#ifdef MMIAP_SORT_SUPPORT
/*****************************************************************************/
//  Description : set sort type selected
//  Global resource dependence :
//  Author:bin.ji
//  Note:
/*****************************************************************************/
LOCAL void SetSortTypeSelected(MMI_CTRL_ID_T       ctrl_id,
                               uint16 list_selection,
                               BOOLEAN is_music_list)
{
    MMI_MENU_ID_T       menu_id = 0;
    GUIMENU_POP_SEL_INFO_T pop_item_info = {0};

#ifdef MMIAP_SORT_SUPPORT
    {
        APLAYER_LIST_SORT_TYPE_E sort_type = APLAYER_LSIT_SORT_NULL;

        sort_type = MMIMP3_GetListSortType(is_music_list,list_selection);

        switch(sort_type)
        {
        case APLAYER_LSIT_SORT_SIZE:
            menu_id = ID_MP3_LIST_SORT_BY_SIZE;
            break;

        case APLAYER_LSIT_SORT_TYPE:
            menu_id = ID_MP3_LIST_SORT_BY_TYPE;
            break;

        case APLAYER_LSIT_SORT_NULL :
            menu_id = ID_MP3_LIST_SORT_BY_NONE;
            break;

        case APLAYER_LSIT_SORT_TIME:
            menu_id = ID_MP3_LIST_SORT_BY_TIME;
            break;

        default:
            menu_id = ID_MP3_LIST_SORT_BY_NAME;
            break;
        }
    }
#else
    menu_id = ID_MP3_LIST_SORT_BY_NAME;
#endif
    pop_item_info.is_static = FALSE;
    pop_item_info.ctrl_id = ctrl_id;
    pop_item_info.node_id = menu_id;

    GUIMENU_SetPopItemStatus(TRUE, &pop_item_info);

}
#endif

/*****************************************************************************/
//  Description : Set Play List Opt Menu Grey Status
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void  SetPlaylistOptMenuGreyStatus(BOOLEAN is_music_list,
        uint16 cur_list_selection,
        uint16 cur_file_selection,
        MMI_CTRL_ID_T   list_ctrl_id,
        MMI_CTRL_ID_T   ctrl_id)
{
    uint16          selected_num    = 0;
    GUILIST_ITEM_T  *list_item_ptr   = PNULL;    /*lint -e529*/
    uint32   total_num = MMIMP3_GetListTotalNum(is_music_list,cur_list_selection);

    MMIAP_STATIC_MENU_GRAY_PARAM_T menu_gray_param = {0};


#ifdef MMI_PDA_SUPPORT
    MMIAP_MENU_ITEM_T menu_group2[]=
    {
        {ID_MP3_LIST_MARK, 0}//灰化标记
#ifdef MMIAP_SORT_SUPPORT
        ,{ID_MP3_LIST_SORT, 0}, //灰化排序
        {ID_MP3_LIST_SORT_BY_TIME ,ID_MP3_LIST_SORT},
        {ID_MP3_LIST_SORT_BY_SIZE, ID_MP3_LIST_SORT},
        {ID_MP3_LIST_SORT_BY_NAME, ID_MP3_LIST_SORT}
#endif
    };
#else
    MMIAP_MENU_ITEM_T menu_group2[]=
    {
        {ID_MP3_LIST_DEL, 0},//灰化标记
        {ID_MP3_LIST_DEL_FROM_LIST, ID_MP3_LIST_DEL}, //灰化排序
        {ID_MP3_LIST_DEL_REAL ,ID_MP3_LIST_DEL},

        {ID_MP3_SET_RING, 0},
        {ID_MP3_LIST_SEND, 0},
        {ID_MP3_LIST_DETAIL, 0},

#ifdef MMIAP_SORT_SUPPORT
        {ID_MP3_LIST_SORT, 0},
        {ID_MP3_LIST_SORT_BY_NONE, ID_MP3_LIST_SORT},
        {ID_MP3_LIST_SORT_BY_TIME, ID_MP3_LIST_SORT},
        {ID_MP3_LIST_SORT_BY_SIZE, ID_MP3_LIST_SORT},
        {ID_MP3_LIST_SORT_BY_NAME, ID_MP3_LIST_SORT},
#endif

        {ID_MP3_SEND_BY_MMS, ID_MP3_LIST_SEND},
#ifdef BLUETOOTH_SUPPORT
        {ID_MP3_SEND_BY_BT, ID_MP3_LIST_SEND},
#endif

        {ID_MP3_ADD, 0},
        {ID_MP3_LIST_ADD_TO_LIST, ID_MP3_ADD},
        {ID_MP3_ADD_TO_NEW_LIST, ID_MP3_ADD},

        {ID_MP3_LIST_MARK, 0},

        {ID_MP3_SUB_MARK, ID_MP3_LIST_MARK},
        {ID_MP3_CANCEL_MARK, ID_MP3_LIST_MARK},
        {ID_MP3_MARK_ALL, ID_MP3_LIST_MARK},
        {ID_MP3_CANCEL_ALL_MARK ,ID_MP3_LIST_MARK}

    };
#endif

#ifndef MMI_PDA_SUPPORT

    if(0 < total_num)
    {
        MMI_SLISTHANDLE_T handle = PNULL;
        MMIAP_DYN_MENU_GRAY_T item = {0};
        BOOLEAN is_gray = FALSE;



        handle = MMIAP_CreateDynamicMenu(sizeof(item));
        if (PNULL != handle)
        {
            is_gray = TRUE;

            item.is_gray = is_gray;


            selected_num = GUILIST_GetSelectedItemIndex(list_ctrl_id, PNULL, 0);
            list_item_ptr = GUILIST_GetItemPtrByIndex(list_ctrl_id, cur_file_selection); /*lint !e605*/

            if(GUILIST_GetListStateById(list_ctrl_id, GUILIST_STATE_ENABLE_MARK))
            {
                //other
                item.menu_item.parent_id = 0;
                item.menu_item.menu_id = ID_MP3_LIST_ADD;
                MMIAP_AddDynMenuGrayItem(handle, &item);

                if (is_music_list)
                {
                    item.menu_item.menu_id = ID_MP3_LIST_SEARCH_ALL;
                    MMIAP_AddDynMenuGrayItem(handle, &item);
                }
                item.menu_item.menu_id = ID_MP3_SET_RING;
                MMIAP_AddDynMenuGrayItem(handle, &item);


                item.menu_item.menu_id = ID_MP3_LIST_SEND;
                MMIAP_AddDynMenuGrayItem(handle, &item);


                item.menu_item.menu_id = ID_MP3_LIST_DETAIL;
                MMIAP_AddDynMenuGrayItem(handle, &item);
#ifdef MMIAP_SORT_SUPPORT
                item.menu_item.menu_id = ID_MP3_LIST_SORT;
                MMIAP_AddDynMenuGrayItem(handle, &item);
#endif
                item.menu_item.menu_id = ID_MP3_SET_CALL_RING;
                item.menu_item.parent_id = ID_MP3_SET_RING;
                MMIAP_AddDynMenuGrayItem(handle, &item);

                item.menu_item.menu_id = ID_MP3_SET_ALARM_RING;
                item.menu_item.parent_id = ID_MP3_SET_RING;
                MMIAP_AddDynMenuGrayItem(handle, &item);

                item.menu_item.menu_id = ID_MP3_SEND_BY_MMS;
                item.menu_item.parent_id = ID_MP3_LIST_SEND;
                MMIAP_AddDynMenuGrayItem(handle, &item);
#ifdef BLUETOOTH_SUPPORT
                item.menu_item.menu_id = ID_MP3_SEND_BY_BT;
                item.menu_item.parent_id = ID_MP3_LIST_SEND;
                MMIAP_AddDynMenuGrayItem(handle, &item);
#endif
#ifdef MMIAP_SORT_SUPPORT
                item.menu_item.menu_id = ID_MP3_LIST_SORT_BY_NONE;
                item.menu_item.parent_id = ID_MP3_LIST_SORT;
                MMIAP_AddDynMenuGrayItem(handle, &item);

                item.menu_item.menu_id = ID_MP3_LIST_SORT_BY_TIME;
                item.menu_item.parent_id = ID_MP3_LIST_SORT;
                MMIAP_AddDynMenuGrayItem(handle, &item);

                item.menu_item.menu_id = ID_MP3_LIST_SORT_BY_SIZE;
                item.menu_item.parent_id = ID_MP3_LIST_SORT;
                MMIAP_AddDynMenuGrayItem(handle, &item);

                item.menu_item.menu_id = ID_MP3_LIST_SORT_BY_NAME;
                item.menu_item.parent_id = ID_MP3_LIST_SORT;
                MMIAP_AddDynMenuGrayItem(handle, &item);
#endif

                if(PNULL != list_item_ptr
                        && GUILIST_GetItemState(list_item_ptr, GUIITEM_STATE_SELECTED)
                  )
                {
                    //mark
                    item.menu_item.menu_id = ID_MP3_SUB_MARK;
                    item.menu_item.parent_id = ID_MP3_LIST_MARK;
                    MMIAP_AddDynMenuGrayItem(handle, &item);

                    if(total_num == selected_num)
                    {
                        item.menu_item.menu_id = ID_MP3_MARK_ALL;
                        item.menu_item.parent_id = ID_MP3_LIST_MARK;
                        MMIAP_AddDynMenuGrayItem(handle, &item);
                    }

                    if(0 == selected_num)
                    {
                        item.menu_item.menu_id = ID_MP3_CANCEL_ALL_MARK;
                        item.menu_item.parent_id = ID_MP3_LIST_MARK;
                        MMIAP_AddDynMenuGrayItem(handle, &item);
                    }

                }
                else
                {
                    //mark
                    item.menu_item.menu_id = ID_MP3_CANCEL_MARK;
                    item.menu_item.parent_id = ID_MP3_LIST_MARK;
                    MMIAP_AddDynMenuGrayItem(handle, &item);

                    if(0 == selected_num)
                    {
                        item.menu_item.menu_id = ID_MP3_CANCEL_ALL_MARK;
                        item.menu_item.parent_id = ID_MP3_LIST_MARK;
                        MMIAP_AddDynMenuGrayItem(handle, &item);
                    }
                }
            }
            else
            {
                //mark
                item.menu_item.menu_id = ID_MP3_CANCEL_MARK;
                item.menu_item.parent_id = ID_MP3_LIST_MARK;
                MMIAP_AddDynMenuGrayItem(handle, &item);


                item.menu_item.menu_id = ID_MP3_CANCEL_ALL_MARK;
                item.menu_item.parent_id = ID_MP3_LIST_MARK;
                MMIAP_AddDynMenuGrayItem(handle, &item);
            }


            MMIAP_BuildDynMenuGrayByCtrl(handle, ctrl_id);

            MMIAP_DestroyDynamicMenu(handle);
        }
    }
    else
    {

        menu_gray_param.menu_group_ptr = menu_group2;
        menu_gray_param.total_num = ARR_SIZE(menu_group2);
        menu_gray_param.is_gray = TRUE;
        menu_gray_param.ctrl_id = ctrl_id;
        menu_gray_param.gray_func = PNULL;
        MMIAP_BuildMenuGrayByCtrl(&menu_gray_param);
    }

#else
    if(0 == total_num)
    {
        menu_gray_param.menu_group_ptr = menu_group2;
        menu_gray_param.total_num = ARR_SIZE(menu_group2);
        menu_gray_param.is_gray = TRUE;
        menu_gray_param.ctrl_id = ctrl_id;
        menu_gray_param.gray_func = PNULL;
        MMIAP_BuildMenuGrayByCtrl(&menu_gray_param);
    }

    /*lint -e529*/
#endif //MMI_PDA_SUPPORT
}

/*****************************************************************************/
//  Description : CreateIndexList
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL uint16* CreateIndexList(MMI_CTRL_ID_T list_ctrl_id,
                              uint32 *select_num_ptr,
                              uint16 cur_file_selection)
{
    uint16 *select_index_ptr = PNULL;


    if (PNULL != select_num_ptr)
    {
        if (PNULL != list_ctrl_id)
        {
            *select_num_ptr = GUILIST_GetSelectedItemIndex(list_ctrl_id, PNULL, 0);
        }
        else
        {
            *select_num_ptr = 0;
        }

        if (0 == *select_num_ptr)
        {
            *select_num_ptr = 1;
            select_index_ptr = (uint16*)SCI_ALLOCA(sizeof(uint16));
            if (PNULL != select_index_ptr)
            {
                SCI_MEMSET(select_index_ptr,0,sizeof(uint16));
                *select_index_ptr = cur_file_selection;
            }
        }
        else
        {
            select_index_ptr = (uint16*)SCI_ALLOCA(sizeof(uint16) * *select_num_ptr);
            if(PNULL != select_index_ptr)
            {
                SCI_MEMSET(select_index_ptr,0,(sizeof(uint16) * *select_num_ptr));
                GUILIST_GetSelectedItemIndex(list_ctrl_id, (uint16*)select_index_ptr, (uint16)*select_num_ptr);
            }
        }
    }

    return select_index_ptr;
}

/*****************************************************************************/
//  Description : CreateIndexList
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void DestoryIndexList(uint16 * index_list_ptr)
{
    if (PNULL != index_list_ptr)
    {
        SCI_FREE(index_list_ptr);
    }

}
/*****************************************************************************/
//  Description : DeleteFileFromList
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DeleteFileFromList(LIST_WIN_USER_DATA_T user_data_info,
                                 MMI_WIN_ID_T list_win_id,
                                 BOOLEAN is_del_file)
{
    MMI_WIN_ID_T        waiting_win_id = MMIMP3_DEL_FILE_WAIT_WIN_ID;
    uint32              waiting_time_out = 0;
    MMIMP3_LIST_DEL_INFO_T *list_delete_info_ptr = PNULL;
    BOOLEAN  result = FALSE;

    list_delete_info_ptr = SCI_ALLOCA(sizeof(MMIMP3_LIST_DEL_INFO_T));
    if (PNULL != list_delete_info_ptr)
    {
        SCI_MEMSET(list_delete_info_ptr,0,sizeof(MMIMP3_LIST_DEL_INFO_T));

        list_delete_info_ptr->is_del_file = is_del_file;
        list_delete_info_ptr->select_index_ptr = CreateIndexList(user_data_info.list_ctrl_id,&(list_delete_info_ptr->select_num),user_data_info.cur_file_selection);
        list_delete_info_ptr->list_selection = user_data_info.cur_list_selection;
        list_delete_info_ptr->list_win_id = list_win_id;
        list_delete_info_ptr->is_music_list = user_data_info.is_music_list;
        if (PNULL != list_delete_info_ptr->select_index_ptr)
        {
            MMIPUB_OpenAlertWinByTextIdEx(SPRD_MP3_APPLET_ID,
                                          &waiting_time_out, TXT_COMMON_WAITING, TXT_NULL,
                                          IMAGE_PUBWIN_WAIT, &waiting_win_id, PNULL, MMIPUB_SOFTKEY_NONE, HandleMp3ListDeleteWaitingWin,
                                          PNULL);
            MMK_PostMsg(waiting_win_id, MSG_MP3_DEL_FILE_INFO, &list_delete_info_ptr, sizeof(list_delete_info_ptr));
#ifdef MMI_PDA_SUPPORT
            GUIFORM_SetChildDisplay(MMIMP3_PLAY_LIST_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
#endif
            GUILIST_SetTextListMarkable(user_data_info.list_ctrl_id, FALSE);
            result = TRUE;
        }
        else
        {
            SCI_FREE(list_delete_info_ptr);
        }
    }
    return result;
}



/*****************************************************************************/
//  Description :  Handle list delete waiting win
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMp3ListDeleteWaitingWin(
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
)
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_WIN_ID_T        alert_win_id = MMIMP3_ALERT_WIN_ID;
    MMIMP3_LIST_DEL_INFO_T **list_delete_info_ptr = PNULL;
    uint32   i = 0;
    uint16   list_index = 0;


    //SCI_TRACE_LOW:"[MMIMP3]: HandleMp3ListDeleteWaitingWin, msg=%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_12273_112_2_18_2_5_4_346, (uint8*)"d", msg_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;


    case MSG_MP3_DEL_FILE_INFO:
        list_delete_info_ptr = (MMIMP3_LIST_DEL_INFO_T **)param;

        if(PNULL == list_delete_info_ptr                   ||
                PNULL == *list_delete_info_ptr              ||
                PNULL == (*list_delete_info_ptr)->select_index_ptr ||
                0     == (*list_delete_info_ptr)->select_num)
        {
            MMK_CloseWin(win_id);
            break;
        }

        if(MMIAPIMP3_IsPlayerActive()
                && MMIMP3_IsCurPlayList((*list_delete_info_ptr)->is_music_list,(*list_delete_info_ptr)->list_selection))
        {
/*            MMIAPIMP3_StopAudioPlayer();*/
            for(i = 0; i < (*list_delete_info_ptr)->select_num; i++)
            {
                if(((*list_delete_info_ptr)->select_index_ptr)[i] == MMIMP3_GetCurPlayFileIndex())
                {
                    MMIAPIMP3_StopAudioPlayer();
					MMISRVAUD_Close(MMIAPIMP3_GetBGPlayerHandle());
                    break;
                }
            }
        }
		else
		{
			MMIMP3_ResetPlayingDynInfo();
		}

        if(!MMIMP3_StartDelFile(*list_delete_info_ptr))
        {
            DestoryIndexList((*list_delete_info_ptr)->select_index_ptr);
            SCI_FREE(*list_delete_info_ptr);
            MMIPUB_OpenAlertWinByTextId(PNULL, TXT_ERROR, TXT_NULL,
                                        IMAGE_PUBWIN_FAIL, &alert_win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);

            MMK_CloseWin(win_id);
        }
        break;

    case MSG_MP3_DEL_CNF:
        list_delete_info_ptr = (MMIMP3_LIST_DEL_INFO_T **)param;
        if (PNULL != list_delete_info_ptr
                && PNULL != *list_delete_info_ptr)
        {
            if((*list_delete_info_ptr)->del_num == 0)
            {
                MMIPUB_OpenAlertWinByTextId(PNULL, TXT_ERROR, TXT_NULL,
                                            IMAGE_PUBWIN_FAIL, &alert_win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
            }
            else
            {
                MMK_SendMsg((*list_delete_info_ptr)->list_win_id, MSG_MP3_UPDATE_LIST, (DPARAM)&((*list_delete_info_ptr)->cur_file_selection));
            }
            MMIMP3_StopDelFile();
            DestoryIndexList((*list_delete_info_ptr)->select_index_ptr);
            SCI_FREE(*list_delete_info_ptr);
        }
        break;

    case MSG_MP3_DEL_DONE:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;

    default:
        result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    }

    return result;
}

/*****************************************************************************/
//  Description :  UpdateList
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void UpdateList(MMI_HANDLE_T handle,
                      MMI_CTRL_ID_T ctrl_id,
                      uint32 total_num,
                      void *param)
{
    uint16  cur_file_selection = 0;

    InitListCtrl(ctrl_id,total_num);
    if(PNULL != param)
    {
        cur_file_selection = *(uint16*)param;
        if(cur_file_selection >= GUILIST_GetTotalItemNum(ctrl_id))
        {
            cur_file_selection = 0;
        }
        GUILIST_SetCurItemIndex(ctrl_id, cur_file_selection);
    }

    if(MMK_IsFocusWin(handle))
    {
        MMK_PostMsg(handle, MSG_FULL_PAINT, PNULL, NULL);
    }

}

/*****************************************************************************/
//  Description :  UpdateList
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void UpdatePlayList(MMI_HANDLE_T handle,
                          LIST_WIN_USER_DATA_T *user_data_ptr,
                          void *param)
{
    uint16  cur_file_selection = 0;
    uint32  total_num = 0;

    if (PNULL != user_data_ptr)
    {
        total_num = MMIMP3_GetListTotalNum(user_data_ptr->is_music_list,user_data_ptr->cur_list_selection);

        InitPlayListCtrl(user_data_ptr);
        if(PNULL != param)
        {
            cur_file_selection = *(uint16*)param;
            if(cur_file_selection >= GUILIST_GetTotalItemNum(user_data_ptr->list_ctrl_id))
            {
                cur_file_selection = 0;
            }
            GUILIST_SetCurItemIndex(user_data_ptr->list_ctrl_id, cur_file_selection);
        }

        if(MMK_IsFocusWin(handle))
        {
            MMK_PostMsg(handle, MSG_FULL_PAINT, PNULL, NULL);
        }
    }

}

/*****************************************************************************/
//  Description : open current play list
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPLISTWIN_OpenCurPlayList(void)
{
    MMIMP3_PLAY_LIST_INFO_T list_info = MMIMP3_GetCurPlayListInfo();

    EnterPlayListWin(list_info.cur_list_index,list_info.is_music_list);
}

/*****************************************************************************/
//  Description : MMIAPMULTILIST_GetListIndex
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPMULTILIST_GetListSelection(MMI_WIN_ID_T win_id)
{
    uint16 list_index = 0;
    LIST_WIN_USER_DATA_T *user_data_ptr = PNULL;

    user_data_ptr = (LIST_WIN_USER_DATA_T *)MMK_GetWinUserData(win_id);

    if (PNULL != user_data_ptr)
    {
        list_index = user_data_ptr->cur_list_selection;
    }
    return list_index;
}

/*****************************************************************************/
//  Description : MMIAPMULTILIST_GetFileIndex
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPMULTILIST_GetFileIndex(MMI_WIN_ID_T win_id)
{
    uint16 file_index = 0;
    LIST_WIN_USER_DATA_T *user_data_ptr = PNULL;

    user_data_ptr = (LIST_WIN_USER_DATA_T *)MMK_GetWinUserData(win_id);

    if (PNULL != user_data_ptr)
    {
        file_index = user_data_ptr->cur_file_selection;
    }

    return file_index;


}

/*****************************************************************************/
//  Description : MMIAPMULTILIST_GetFileIndex
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPMULTILIST_IsMusicList(MMI_WIN_ID_T win_id)
{
    LIST_WIN_USER_DATA_T *user_data_ptr = PNULL;

    user_data_ptr = (LIST_WIN_USER_DATA_T *)MMK_GetWinUserData(win_id);

    if (PNULL != user_data_ptr)
    {
        return user_data_ptr->is_music_list;
    }
    return FALSE;


}


/*****************************************************************************/
//  Description : Call back function for adding file to general play list.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void AddFileToListCallBack(BOOLEAN     is_success,
                                 FILEARRAY   file_array)
{
    uint32          added_num = 0;
    uint32          total_num = 0;
    uint16          new_file_selection = 0;
    MMI_WIN_ID_T    alert_win_id = MMIMP3_ALERT_WIN_ID;
    uint32          file_num = 0;
    LIST_WIN_USER_DATA_T      *user_data_ptr = (LIST_WIN_USER_DATA_T *)MMK_GetWinUserData(MMIMP3_PLAY_LIST_WIN_ID);
    
    file_num = MMIAPIFILEARRAY_GetArraySize(file_array);
    
    if(is_success && 0 < file_num && PNULL != user_data_ptr)
    {
        total_num = MMIMP3_GetListTotalNum(user_data_ptr->is_music_list,user_data_ptr->cur_list_selection);
        if(total_num < MMIAP_PLAY_LIST_MAX_NUM)
        {
        added_num = MMIAP_PLAY_LIST_MAX_NUM - total_num;;
        }
        else
        {
        added_num = 0;
        }
    
        if(MMIMP3_AppendFilesToList(TRUE,user_data_ptr->cur_list_selection,file_array,&added_num))
        {
            if(file_num != added_num)
            {
                MMIPUB_OpenAlertWinByTextId(PNULL, TXT_NO_SPACE,//"空间不足，未能全部加载",
                                            TXT_NULL, IMAGE_PUBWIN_WARNING, &alert_win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
            }


            MMK_SendMsg(MMIMP3_PLAY_LIST_WIN_ID, MSG_MP3_UPDATE_LIST, (DPARAM)&new_file_selection);
        }
        else
        {
            MMIPUB_OpenAlertWinByTextId(PNULL, TXT_ERROR, TXT_NULL,
                                        IMAGE_PUBWIN_FAIL, &alert_win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
        }
    }

    if(PNULL != file_array)
    {
        MMIAPIFILEARRAY_Destroy(&file_array);
        file_array = PNULL;
    }
}

/*****************************************************************************/
//  Description : MMIMP3_AddFileToList
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
LOCAL void AddFileFromFmmToList(BOOLEAN is_music_list,
                                uint16 cur_list_selection)
{
    MMIFMM_FILTER_T filter = {0};
    FILEARRAY   ret_array = NULL;
    wchar       full_path_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16      full_path_len  = MMIFILE_FULL_PATH_MAX_LEN;
    wchar       *path_ptr = PNULL;

    MMIMP3_GetMusicFilter(&filter);

    ret_array = MMIAPIFILEARRAY_Create();

    if(MMIMP3_GetDefaultAudioFolder(full_path_name, &full_path_len))
    {
        path_ptr = full_path_name;
    }
    if(FMM_OPENWIN_SUCCESS != MMIAPIFMM_OpenFileWinByLayer(path_ptr,full_path_len,&filter,FUNC_FIND_FILE_AND_ALLFOLER,TRUE,NULL,ret_array,AddFileToListCallBack,FALSE))
    {
        MMIAPIFILEARRAY_Destroy(&ret_array);
    }

}
