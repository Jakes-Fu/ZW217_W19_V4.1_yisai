/*****************************************************************************
** File Name:               mmiapwin_musicmark.c                             *
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
#ifdef MMIAP_MUSICMARK_SUPPORT
#define _MMIAPWIN_MUSICMARK_C_

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
#include "guirichtext.h"
#include "guires.h"
#include "guilistbox.h"
#include "mmi_common.h"
#include "mmi_appmsg.h"


#include "mmiap_play.h"
#include "mmiap_list_manager.h"
#include "mmiap_list_control.h"
#include "mmiap_musicmark.h"
#include "mmiapwin_musicmark.h"
#include "mmimp3_id.h"
#include "mmimp3_text.h"
#include "mmimp3_image.h"
#include "mmimp3_menutable.h"
#include "mmiap_applet.h"
#include "mmi_pubmenuwin.h"
#include "mmimp3_export.h"
#include "mmiapwin_menu.h"
#include "mmi_appmsg.h"

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
    uint16  cur_list_selection;
    MMI_CTRL_ID_T ctrl_id;
    BOOLEAN       is_markall;
} MARKLIST_USERDATA_INFO_T;

typedef struct
{
    uint16  cur_list_selection;
} MARKLIST_DETAIL_USER_DATA_T;

typedef MARKLIST_DETAIL_USER_DATA_T MARKLIST_RENAME_USER_DATA_T;

typedef struct
{
    uint16  *select_item_ptr;
    uint16  select_num;
    MMI_HANDLE_T list_win_id;
    uint16   list_selection;
} MARKLIST_DEL_INFO;
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
//  Description : Set Mark List Opt Menu Grey Status
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void  SetMarkListOptMenuGreyStatus(MMI_CTRL_ID_T   list_ctrl_id,
        MMI_CTRL_ID_T   ctrl_id,
        uint16          total_num,
        BOOLEAN is_exist_play_file);

/*****************************************************************************/
//  Description : open opt menu
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void  OpenMarklistOptMenu(MMI_WIN_ID_T         dst_win_id);

/*****************************************************************************/
//  Description : handle pub menu win init handler
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandleMarkListPubMenuInitHandler(MMIPUB_MENU_NOTIFY_T *param);

/*****************************************************************************/
//  Description : handle pub menu win init handler
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandleMarkListPubMenuOKHandler(MMIPUB_MENU_NOTIFY_T *param);

/*****************************************************************************/
//  Description : handle music mark list window message
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMp3MusicMarkListWinMsg(
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
);


/*****************************************************************************/
//  Description : handle renaming music mark window message
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMp3RenameMarkWinMsg(
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
);

/*****************************************************************************/
//  Description : handle music mark detail window message
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMp3MusicMarkDetailWinMsg(
    MMI_WIN_ID_T      win_id,
    MMI_MESSAGE_ID_E  msg_id,
    DPARAM            param
);


/*****************************************************************************/
//  Description :Set Rich Text Item Character
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void SetRichTextItemCharacter(MMI_CTRL_ID_T       ctrl_id,
                                    MMI_STRING_T        detail_str,
                                    uint16   *nIndex_ptr);

/*****************************************************************************/
//  Description : Set music mark detail window param.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void SetMp3MarkDetail(MMI_CTRL_ID_T                   ctrl_id,
                            uint16  cur_selection);

/*****************************************************************************/
//  Description : Init the control of the mark list window.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void InitMusicMarkListCtrl(
    MMI_CTRL_ID_T ctrl_id,
    uint32 total_num
);

/*****************************************************************************/
//  Description :  Init the list item of the mark list window.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void InitMusicMarkListItem(
    MMI_CTRL_ID_T   ctrl_id,
    uint32          item_index,
    MMI_TEXT_ID_T   left_softkey_id,
    MMI_TEXT_ID_T   mid_softkey_id,
    MMI_TEXT_ID_T   right_softkey_id
);

/*****************************************************************************/
//  Description :  Handle list delete waiting win
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMarkDeleteWaitingWin(
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
);

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : open mark list Long Ok context menu
//  Global resource dependence :
//  Author: xin.li
//  Note: 音乐书签列表长按LIST弹出context menu
/*****************************************************************************/
LOCAL void  OpenMarklistLongOkOptMenu(MMI_WIN_ID_T         dst_win_id);

/*****************************************************************************/
//  Description : set Context Menu Grey Status
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void  SetMarkListLongOkOptMenuGreyStatus(MMI_CTRL_ID_T   ctrl_id,
        MMI_CTRL_ID_T   list_ctrl_id,
        uint16          cur_selection,
        uint16          total_num);

#endif //MMI_PDA_SUPPORT

/*****************************************************************************/
//  Description :GetMarkListName
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetMarkListName(MMIMP3_MUSIC_MARK_DETAIL_INFO_T *mark_info_ptr,
                              MMI_STRING_T        *detail_str_ptr);

/*****************************************************************************/
//  Description :OpenMarkListDetailWin
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void OpenMarkListDetailWin(uint16 cur_list_selection);

/*****************************************************************************/
//  Description :OpenMarkListRenameWin
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void OpenMarkListRenameWin(uint16 cur_list_selection);

/*****************************************************************************/
//  Description :  Handle list delete waiting win
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN OpenDelMarkListWin(MMI_HANDLE_T win_id,
                                 MMI_CTRL_ID_T list_ctrl_id,
                                 uint16 list_selection);

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : handle pub menu win init handler
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandleMarkListLONGPubMenuInitHandler(MMIPUB_MENU_NOTIFY_T *param);
#endif

/*****************************************************************************/
//  Description : PlayMarkFile
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN PlayMarkFile(uint16  cur_list_selection);

/**--------------------------------------------------------------------------*
 **                         CONSTANT VARIABLES                               *
 **--------------------------------------------------------------------------*/
LOCAL WINDOW_TABLE(MMIMP3_MUSIC_MARK_WIN_TAB) =
{
#ifndef MMI_AUDIOPLAYER_MINI_SUPPORT
    WIN_STATUSBAR,
#endif
    WIN_TITLE(TXT_MP3_MUSIC_MARK_MENU),
    WIN_FUNC((uint32)HandleMp3MusicMarkListWinMsg),
    WIN_ID(MMIMP3_MUSIC_MARK_WIN_ID),
#ifdef MMI_PDA_SUPPORT//NEWMS00169534
    WIN_STYLE(WS_DISABLE_SOFTKEY_TO_TITLE),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIMP3_MUSIC_MARK_LIST_FORM_CTRL_ID),
    CHILD_SOFTKEY_CTRL(TXT_MARK_ALL, TXT_DELETE, STXT_CANCEL, MMICOMMON_SOFTKEY_CTRL_ID, MMIMP3_MUSIC_MARK_LIST_FORM_CTRL_ID),
    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, MMIMP3_MUSIC_MARK_LIST_CTRL_ID, MMIMP3_MUSIC_MARK_LIST_FORM_CTRL_ID),
#else
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIMP3_MUSIC_MARK_LIST_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    END_WIN
};


LOCAL WINDOW_TABLE(MMIMP3_RENAME_MARK_WIN_TAB) =
{
    WIN_TITLE(TXT_MP3_INPUT_MARK_NAME),
    WIN_FUNC((uint32)HandleMp3RenameMarkWinMsg),
    WIN_ID(MMIMP3_RENAME_MARK_WIN_ID),
    //WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_TEXT_CTRL(MMIMP3_MARK_NAME_MAX_LEN, MMIMP3_RENAME_MARK_EDITBOX_CTRL_ID),
#ifndef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#else
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
#endif
    END_WIN
};

// the window for mp3 detail window
LOCAL WINDOW_TABLE(MMIMP3_MUSIC_MARK_DETAIL_WIN_TAB) =
{
#ifndef MMI_AUDIOPLAYER_MINI_SUPPORT
    WIN_STATUSBAR,
#endif
    WIN_TITLE(STXT_DETAIL),
    WIN_FUNC((uint32)HandleMp3MusicMarkDetailWinMsg),
    WIN_ID(MMIMP3_MUSIC_MARK_DETAIL_WIN_ID),
    CREATE_RICHTEXT_CTRL(MMIMP3_MUSIC_MARK_DETAIL_TEXT_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
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
PUBLIC void MMIAPMUSICMARK_Enter(void)
{
    MARKLIST_USERDATA_INFO_T *user_data_ptr = PNULL;
    MMI_HANDLE_T  win_handle = PNULL;

    user_data_ptr = SCI_ALLOCA(sizeof(MARKLIST_USERDATA_INFO_T));
    if (PNULL != user_data_ptr)
    {
        win_handle = MMIAPAPPLET_CreateWinTable((uint32 *)MMIMP3_MUSIC_MARK_WIN_TAB, PNULL);
        SCI_MEMSET(user_data_ptr,0,sizeof(MARKLIST_USERDATA_INFO_T));
        MMK_SetWinUserData(win_handle,user_data_ptr);
    }
}

/*****************************************************************************/
//  Description : handle music mark list window message
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMp3MusicMarkListWinMsg(
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
)
{
    MMI_RESULT_E    result  = MMI_RESULT_TRUE;
    uint16          cur_top_pos   = 0;
    uint16          cur_total_num = 0;
    uint16          new_selection = 0;
    MMI_CTRL_ID_T   list_ctrl_id = MMIMP3_MUSIC_MARK_LIST_CTRL_ID;
    GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr = PNULL;

#ifdef MMI_PDA_SUPPORT      //NEWMS00169534
    GUIFORM_CHILD_HEIGHT_T child_height = {0};
    uint16                  mark_num = 0;
    GUIFORM_CHILD_DISPLAY_E   display_type = GUIFORM_CHILD_DISP_MAX;
#endif
    uint32       total_num = MMIMP3_GetMusicMarkNum();
    MARKLIST_USERDATA_INFO_T *user_data_ptr = (MARKLIST_USERDATA_INFO_T *)MMK_GetWinUserData(win_id);


    MMI_WIN_ID_T    query_win_id = MMIMP3_QUERY_WIN_ID;




    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        if (PNULL == user_data_ptr)
        {
            MMK_CloseWin(win_id);
            break;
        }
        user_data_ptr->ctrl_id = list_ctrl_id;
#ifdef MMI_PDA_SUPPORT
        user_data_ptr->is_markall = FALSE;
        child_height.type = GUIFORM_CHILD_HEIGHT_LEFT;
        GUILIST_PermitBorder(list_ctrl_id, FALSE);
        GUIFORM_IsSlide(MMIMP3_MUSIC_MARK_LIST_FORM_CTRL_ID, FALSE);
        GUIFORM_SetChildDisplay(MMIMP3_MUSIC_MARK_LIST_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
        GUIFORM_SetChildHeight(MMIMP3_MUSIC_MARK_LIST_FORM_CTRL_ID, list_ctrl_id, &child_height);
        GUIFORM_PermitChildBg(MMIMP3_MUSIC_MARK_LIST_FORM_CTRL_ID, FALSE);
        GUILIST_SetUserBg(list_ctrl_id, FALSE);
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#else
        GUILIST_SetListState(list_ctrl_id, GUILIST_STATE_NEED_SOFTKEY, TRUE);
        GUILIST_SetOwnSofterKey(list_ctrl_id, TRUE);
#endif

        InitMusicMarkListCtrl(list_ctrl_id,total_num);
        MMK_SetAtvCtrl(win_id, list_ctrl_id);
        break;

    case MSG_GET_FOCUS:
#ifdef MMI_PDA_SUPPORT      //NEWMS00169534
        if(0 == total_num)
        {
            GUIFORM_SetChildDisplay(MMIMP3_MUSIC_MARK_LIST_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
            GUILIST_SetTextListMarkable(list_ctrl_id, FALSE);
        }

#endif
        break;

    case MSG_CTL_LIST_NEED_ITEM_DATA:
        if(0 != total_num)
        {
            need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;

            if(PNULL == need_item_data_ptr)
            {
                MMIMP3_FatalError();
                break;
            }
            InitMusicMarkListItem(
                list_ctrl_id,
                (uint32)need_item_data_ptr->item_index,
                STXT_OPTION,
                TXT_COMMON_PLAY,
                STXT_RETURN
            );
        }

        break;


    case MSG_MP3_UPDATE_MARK_LIST:
        if (PNULL == user_data_ptr)
        {
            break;
        }
        user_data_ptr->cur_list_selection = GUILIST_GetCurItemIndex(list_ctrl_id);
        cur_top_pos   = GUILIST_GetTopItemIndex(list_ctrl_id);
        cur_total_num = GUILIST_GetTotalItemNum(list_ctrl_id);
        InitMusicMarkListCtrl(list_ctrl_id,total_num);

        if(PNULL != (uint16*)param)
        {
            new_selection = *(uint16*)param;
        }

        if(user_data_ptr->cur_list_selection == new_selection &&
                cur_total_num == total_num)
        {
            GUILIST_SetTopItemIndex(list_ctrl_id, cur_top_pos);
        }

        if(new_selection < total_num)
        {
            GUILIST_SetCurItemIndex(list_ctrl_id, new_selection);
        }

        if(MMK_IsFocusWin(win_id))
        {
            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, NULL);
        }

        break;

    case MSG_CTL_LIST_MARK_ITEM:
#ifdef MMI_PDA_SUPPORT//NEWMS00169534
        mark_num = GUILIST_GetSelectedItemIndex(list_ctrl_id, PNULL, 0);
        if(0 == mark_num)
        {
            GUISOFTKEY_SetTextId(win_id, MMICOMMON_SOFTKEY_CTRL_ID, TXT_MARK_ALL, TXT_DELETE, STXT_CANCEL, TRUE);
            if (PNULL != user_data_ptr)
            {
                user_data_ptr->is_markall = FALSE;
            }
        }
        if(GUILIST_GetTotalItemNum(list_ctrl_id) == mark_num)
        {
            GUISOFTKEY_SetTextId(win_id, MMICOMMON_SOFTKEY_CTRL_ID, TXT_CANCEL_MARK_ALL, TXT_DELETE, STXT_CANCEL, TRUE);
            if (PNULL != user_data_ptr)
            {
                user_data_ptr->is_markall = TRUE;
            }
        }

#else
        if(0 == GUILIST_GetSelectedItemIndex(list_ctrl_id, PNULL, 0))
        {
            GUILIST_SetTextListMarkable(list_ctrl_id, FALSE);
        }

#endif
        break;

    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:
        if (PNULL == user_data_ptr)
        {
            break;
        }
        user_data_ptr->cur_list_selection = GUILIST_GetCurItemIndex(MMIMP3_MUSIC_MARK_LIST_CTRL_ID);
#ifdef MMI_PDA_SUPPORT      //NEWMS00169534
        GUIFORM_GetChildDisplay(MMIMP3_MUSIC_MARK_LIST_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, &display_type);

        if(GUIFORM_CHILD_DISP_NORMAL == display_type)
        {
            MMI_CTRL_ID_T       ctrl_id_local = 0;

            if(PNULL != param)
            {
                ctrl_id_local = ((MMI_NOTIFY_T*) param)->src_id;

                if(ctrl_id_local == MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID) //删除菜单
                {
                    if (0 == GUILIST_GetSelectedItemIndex(list_ctrl_id, PNULL, 0))
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_SMS_SELECT_ONT_PROMPT);
                    }
                    else
                    {
                        //打开询问是否删除窗口;
                        MMIPUB_OpenQueryWinByTextIdEx(SPRD_MP3_APPLET_ID,
                                                      TXT_DELETE,
                                                      IMAGE_PUBWIN_QUERY,
                                                      &query_win_id,
                                                      PNULL,
                                                      PNULL
                                                     );
                        //OpenDelMarkListWin(win_id,list_ctrl_id,user_data_ptr->cur_list_selection);
                    }
                }
                else if(ctrl_id_local == MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID)//mark all 菜单
                {
                    user_data_ptr->is_markall =  !user_data_ptr->is_markall;
                    if(user_data_ptr->is_markall)
                    {
                        GUISOFTKEY_SetTextId(win_id, MMICOMMON_SOFTKEY_CTRL_ID, TXT_CANCEL_MARK_ALL, TXT_DELETE, STXT_CANCEL, FALSE);

                        if(0 == GUILIST_GetSelectedItemIndex(list_ctrl_id, PNULL, 0))
                        {
                            GUILIST_SetTextListMarkable(list_ctrl_id, TRUE);
                            GUILIST_SetMaxSelectedItem(list_ctrl_id, (uint16)total_num);
                        }

                        GUILIST_SetAllSelected(list_ctrl_id, TRUE);
                    }
                    else
                    {
                        GUISOFTKEY_SetTextId(win_id, MMICOMMON_SOFTKEY_CTRL_ID, TXT_MARK_ALL, TXT_DELETE, STXT_CANCEL, FALSE);
                        GUILIST_SetAllSelected(list_ctrl_id, FALSE);
                    }

                    MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
                }
                else if(ctrl_id_local == MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID)
                {
                    GUIFORM_SetChildDisplay(MMIMP3_MUSIC_MARK_LIST_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);

                    GUILIST_SetTextListMarkable(list_ctrl_id, FALSE);
                    GUILIST_SetAllSelected(list_ctrl_id, FALSE);

                    GUISOFTKEY_SetTextId(win_id, MMICOMMON_SOFTKEY_CTRL_ID, TXT_MARK_ALL, TXT_DELETE, STXT_CANCEL, TRUE);

                    user_data_ptr->is_markall = FALSE;
                    MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
                }
            }
        }
        else
#endif

            if(PlayMarkFile(user_data_ptr->cur_list_selection))
            {
                MMK_CloseWin(win_id);
                MMK_CloseWin(MMIMP3_PLAYER_OPT_MENU_WIN_ID);
            }
        break;

    case MSG_CTL_OK:
    case MSG_APP_MENU:
#ifdef MMI_PDA_SUPPORT  //NEWMS00169534
        GUIFORM_GetChildDisplay(MMIMP3_MUSIC_MARK_LIST_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, &display_type);
        if(GUIFORM_CHILD_DISP_NORMAL != display_type)
#endif
        {
            if (PNULL != user_data_ptr)
            {
                user_data_ptr->cur_list_selection = GUILIST_GetCurItemIndex(MMIMP3_MUSIC_MARK_LIST_CTRL_ID);
                OpenMarklistOptMenu(win_id);
            }
        }

        break;

#ifdef MMI_PDA_SUPPORT
    case MSG_CTL_LIST_LONGOK:
        //长按List弹出context menu菜单
        if(0 < total_num
                && !GUILIST_GetListStateById(list_ctrl_id, GUILIST_STATE_ENABLE_MARK))   //NEWMS00169057
        {
            if (PNULL != user_data_ptr)
            {
                user_data_ptr->cur_list_selection = GUILIST_GetCurItemIndex(MMIMP3_MUSIC_MARK_LIST_CTRL_ID);
                OpenMarklistLongOkOptMenu(win_id);
            }
        }

        break;
#endif

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

    case MSG_PROMPTWIN_CANCEL:
        MMK_CloseWin(query_win_id);
        break;

    case MSG_PROMPTWIN_OK:
        MMK_CloseWin(query_win_id);
        if (PNULL != user_data_ptr)
        {
            user_data_ptr->cur_list_selection = GUILIST_GetCurItemIndex(list_ctrl_id);

            OpenDelMarkListWin(win_id,list_ctrl_id,user_data_ptr->cur_list_selection);


            //change mark status
#ifdef MMI_PDA_SUPPORT
            if (GUILIST_GetListState(list_ctrl_id, GUILIST_STATE_ENABLE_MARK))
            {
                mark_num = GUILIST_GetSelectedItemIndex(list_ctrl_id, PNULL, 0);
                if(GUILIST_GetTotalItemNum(list_ctrl_id) == mark_num)
                {
                    GUILIST_SetTextListMarkable(list_ctrl_id, FALSE);
                    GUIFORM_SetChildDisplay(MMIMP3_MUSIC_MARK_LIST_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
                }

                GUISOFTKEY_SetTextId(win_id, MMICOMMON_SOFTKEY_CTRL_ID, TXT_MARK_ALL, TXT_DELETE, STXT_CANCEL, TRUE);

                user_data_ptr->is_markall = FALSE;
            }
#else
            GUILIST_SetTextListMarkable(list_ctrl_id, FALSE);

#endif

        }
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : PlayMarkFile
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN PlayMarkFile(uint16  cur_list_selection)
{
    APLAYER_LIST_ERROR_E  list_error = APLAYER_LIST_ERROR;
    MMI_WIN_ID_T    alert_win_id = MMIMP3_ALERT_WIN_ID;
    MMIMP3_MUSIC_MARK_DETAIL_INFO_T mark_info = {0};
    BOOLEAN   result = FALSE;

    if(0 < MMIMP3_GetMusicMarkNum()
            && MMIMP3_GetMusicMarkInfo(cur_list_selection, &mark_info))
    {
        MMIAPIMP3_StopAudioPlayer();
        list_error = MMIMP3_SetCurPlayInfoByName(
                         mark_info.list_name, mark_info.list_name_len,
                         mark_info.music_file_name, mark_info.music_file_name_len,
                         MMIMP3_GetMp3PlayMode());

        if(APLAYER_LIST_SUCCESS == list_error)
        {
            MMIAPIMP3_PlayAudioPlayer(mark_info.mark_offset_in_byte);
            result = TRUE;
        }
        else if(APLAYER_LIST_FILE_NOT_IN_LIST == list_error)
        {
            MMIPUB_OpenAlertWinByTextId(PNULL, TXT_MP3_MUSIC_FILE_NOT_IN_LIST, TXT_NULL,
                                        IMAGE_PUBWIN_WARNING, &alert_win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
        }
        else if(APLAYER_LIST_NO_EXIST == list_error)
        {
            MMIPUB_OpenAlertWinByTextId(PNULL, TXT_MP3_MARK_MUSIC_LIST_NOT_EXIST, TXT_NULL,
                                        IMAGE_PUBWIN_WARNING, &alert_win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
        }
        else if (APLAYER_LIST_FILE_NO_EXIST == list_error)
        {
            MMIPUB_OpenAlertWinByTextId(PNULL, TXT_MP3_MARK_MUSIC_LIST_NOT_EXIST, TXT_NULL,
                                        IMAGE_PUBWIN_WARNING, &alert_win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
        }
    }


    return result;
}

/*****************************************************************************/
//  Description : Init the control of the mark list window.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void InitMusicMarkListCtrl(
    MMI_CTRL_ID_T ctrl_id,
    uint32 total_num
)
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
        GUILIST_SetMaxItem(ctrl_id, total_num + 1, FALSE);
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

        //icon
        //item_data.item_content[0].item_data_type     = GUIITEM_DATA_NONE;
        //item_data.item_content[0].item_data.image_id = IMAGE_NULL;//空

        // text 1
        item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[0].item_data.text_id = TXT_MP3_NO_MARK;

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
//  Description :  Init the list item of the mark list window.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void InitMusicMarkListItem(
    MMI_CTRL_ID_T   ctrl_id,
    uint32          item_index,
    MMI_TEXT_ID_T   left_softkey_id,
    MMI_TEXT_ID_T   mid_softkey_id,
    MMI_TEXT_ID_T   right_softkey_id
)
{
    GUILIST_ITEM_T       item_t    = {0};   /*lint !e64*/
    GUILIST_ITEM_DATA_T  item_data = {0};   /*lint !e64*/
    MMIMP3_MUSIC_MARK_DETAIL_INFO_T mark_info = {0};

    //ucs2_str.str_ptr = (uint8*)str;
    item_t.item_data_ptr = &item_data;

    //soft key
    item_data.softkey_id[0] = left_softkey_id;
    item_data.softkey_id[1] = mid_softkey_id;
    item_data.softkey_id[2] = right_softkey_id;

    //icon
    item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[0].item_data.image_id = IMAGE_MP3_MUSIC_MARK_ICON; //mark icon

    // text 1
    if(MMIMP3_GetMusicMarkInfo(item_index, &mark_info))
    {
        item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[1].item_data.text_buffer.wstr_len = mark_info.mark_name_len;
        item_data.item_content[1].item_data.text_buffer.wstr_ptr = mark_info.mark_name;
    }

    GUILIST_SetItemData(ctrl_id, &item_data, item_index);
}

#ifndef MMI_AUDIOPLAYER_OPT_MINI_SUPPORT
/*****************************************************************************/
//  Description : open opt menu
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void  OpenMarklistOptMenu(MMI_WIN_ID_T         dst_win_id)
{
    MMIPUB_MENU_PARAM_T menuparam = {0};

    menuparam.group_id = MENU_DYNA;
    menuparam.win_id = (MMI_WIN_ID_T)MMIMP3_MUSIC_MARK_OPT_WIN_ID;
    menuparam.ctrl_id = (MMI_CTRL_ID_T)MMIMP3_MUSIC_MARK_OPT_MENU_CTRL_ID;
    menuparam.applet_handle = SPRD_MP3_APPLET_ID;

    menuparam.dst_handle = dst_win_id;
    menuparam.func.OnMenuOk = HandleMarkListPubMenuOKHandler;
    menuparam.func.OnMenuInit = HandleMarkListPubMenuInitHandler;
    menuparam.menu_style = GUIMENU_STYLE_POPUP;
    MMIPUB_OpenPubMenuWin(&menuparam);
}

/*****************************************************************************/
//  Description : handle pub menu win init handler
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandleMarkListPubMenuInitHandler(MMIPUB_MENU_NOTIFY_T *param)
{
    MARKLIST_USERDATA_INFO_T *user_data_ptr = PNULL;

#ifdef MMI_PDA_SUPPORT
    BOOLEAN is_display_image = TRUE;
//PDA style menu
    MMIAP_MENU_ITEM_T menu_list[]=
    {
        //标记
        {ID_MP3_LIST_MARK, 0},
        //添加书签
        {ID_MP3_MARK_ADD, 0}
    };
#else
    BOOLEAN is_display_image = FALSE;
    MMIAP_MENU_ITEM_T menu_list[]=
    {
        {ID_MP3_LIST_OPT_RETURN, 0},
        {ID_MP3_MARK_ADD, 0},
        {ID_MP3_LIST_DEL, 0},
        {ID_MP3_LIST_MARK, 0},
        {ID_MP3_LIST_LIST_RENAME, 0},
        {ID_MP3_LIST_DETAIL, 0},
        {ID_MP3_SUB_MARK, ID_MP3_LIST_MARK},
        {ID_MP3_CANCEL_MARK, ID_MP3_LIST_MARK},
        {ID_MP3_MARK_ALL, ID_MP3_LIST_MARK},
        {ID_MP3_CANCEL_ALL_MARK, ID_MP3_LIST_MARK}
    };
#endif

    if (PNULL != param)
    {
        user_data_ptr = (MARKLIST_USERDATA_INFO_T *)MMK_GetWinUserData(param->dst_handle);
        //GUIMENU_CreatDynamic(PNULL, param->win_id, param->ctrl_id, GUIMENU_STYLE_POPUP);


        MMIAP_BuildMenuToCtrl(menu_list, ARR_SIZE(menu_list),
                              PNULL,
                              is_display_image,
                              param->ctrl_id
                             );



        if (PNULL != user_data_ptr)
        {
            SetMarkListOptMenuGreyStatus(user_data_ptr->ctrl_id,param->ctrl_id,
                                         MMIMP3_GetMusicMarkNum(),
                                         MMIAPIMP3_IsLoadCurPlayInfo());
        }

    }
}

/*****************************************************************************/
//  Description : Set Mark List Opt Menu Grey Status
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void  SetMarkListOptMenuGreyStatus(MMI_CTRL_ID_T   list_ctrl_id,
        MMI_CTRL_ID_T   ctrl_id,
        uint16          total_num,
        BOOLEAN is_exist_play_file)
{
    uint16          cur_selection   = GUILIST_GetCurItemIndex(list_ctrl_id);
    uint16          selected_num    = 0;
    GUILIST_ITEM_T  *list_item_ptr   = PNULL;


    MMI_SLISTHANDLE_T handle = PNULL;
    MMIAP_DYN_MENU_GRAY_T item = {0};
    BOOLEAN is_gray = FALSE;



    handle = MMIAP_CreateDynamicMenu(sizeof(item));
    if (PNULL != handle)
    {
        is_gray = TRUE;

        item.is_gray = is_gray;

#ifndef MMI_PDA_SUPPORT

        if(0 < total_num)
        {
            selected_num = GUILIST_GetSelectedItemIndex(list_ctrl_id, PNULL, 0);
            list_item_ptr = GUILIST_GetItemPtrByIndex(list_ctrl_id, cur_selection); /*lint !e605*/

            if(GUILIST_GetListStateById(list_ctrl_id, GUILIST_STATE_ENABLE_MARK))
            {
                item.menu_item.parent_id = 0;
                item.menu_item.menu_id = ID_MP3_MARK_ADD;
                MMIAP_AddDynMenuGrayItem(handle, &item);

                item.menu_item.menu_id = ID_MP3_LIST_LIST_RENAME;
                MMIAP_AddDynMenuGrayItem(handle, &item);

                item.menu_item.menu_id = ID_MP3_LIST_DETAIL;
                MMIAP_AddDynMenuGrayItem(handle, &item);


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
        }
        else
        {
            item.menu_item.parent_id = 0;
            item.menu_item.menu_id = ID_MP3_LIST_DEL;
            MMIAP_AddDynMenuGrayItem(handle, &item);

            item.menu_item.menu_id = ID_MP3_LIST_MARK;
            MMIAP_AddDynMenuGrayItem(handle, &item);

            item.menu_item.menu_id = ID_MP3_LIST_LIST_RENAME;
            MMIAP_AddDynMenuGrayItem(handle, &item);

            item.menu_item.menu_id = ID_MP3_LIST_DETAIL;
            MMIAP_AddDynMenuGrayItem(handle, &item);
        }

        if(!is_exist_play_file)
        {
            item.menu_item.parent_id = 0;
            item.menu_item.menu_id = ID_MP3_MARK_ADD;
            MMIAP_AddDynMenuGrayItem(handle, &item);
        }

#else

        if(0 < total_num)
        {
            selected_num = GUILIST_GetSelectedItemIndex(list_ctrl_id, PNULL, 0);
            list_item_ptr = GUILIST_GetItemPtrByIndex(list_ctrl_id, cur_selection); /*lint !e605*/

            if(GUILIST_GetListStateById(list_ctrl_id, GUILIST_STATE_ENABLE_MARK))
            {
                //添加书签
                item.menu_item.parent_id = 0;
                item.menu_item.menu_id = ID_MP3_MARK_ADD;
                MMIAP_AddDynMenuGrayItem(handle, &item);


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
        }
        else
        {
            item.menu_item.parent_id = 0;
            item.menu_item.menu_id = ID_MP3_LIST_MARK;
            MMIAP_AddDynMenuGrayItem(handle, &item);
        }

        if(!is_exist_play_file)
        {
            item.menu_item.parent_id = 0;
            item.menu_item.menu_id = ID_MP3_MARK_ADD;
            MMIAP_AddDynMenuGrayItem(handle, &item);
        }

#endif //MMI_PDA_SUPPORT

        MMIAP_BuildDynMenuGrayByCtrl(handle, ctrl_id);

        MMIAP_DestroyDynamicMenu(handle);

    }
}
#endif

/*****************************************************************************/
//  Description : handle pub menu win init handler
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandleMarkListPubMenuOKHandler(MMIPUB_MENU_NOTIFY_T *param)
{
    MMI_MENU_ID_T       menu_id = ID_MP3_EXIT;
    MARKLIST_USERDATA_INFO_T *user_data_ptr = PNULL;


    MMI_WIN_ID_T        alert_win_id = MMIMP3_ALERT_WIN_ID;
    MMIFILE_ERROR_E     add_result = SFS_NO_ERROR;
    uint16              cur_selection = 0;


    MMI_WIN_ID_T    query_win_id = MMIMP3_QUERY_WIN_ID;


    if (PNULL != param)
    {
        user_data_ptr = (MARKLIST_USERDATA_INFO_T *)MMK_GetWinUserData(param->dst_handle);
        menu_id = GUIMENU_GetCurNodeId(param->ctrl_id);

        switch(menu_id)
        {
        case ID_MP3_LIST_DEL:
            //打开询问是否删除窗口;
            MMIPUB_OpenQueryWinByTextIdEx(SPRD_MP3_APPLET_ID,
                                          TXT_DELETE,
                                          IMAGE_PUBWIN_QUERY,
                                          &query_win_id,
                                          PNULL,
                                          PNULL
                                         );
            break;
#ifndef MMI_PDA_SUPPORT     //NEWMS00169534

        case ID_MP3_SUB_MARK:
            if (PNULL != user_data_ptr)
            {
                GUILIST_SetTextListMarkable(user_data_ptr->ctrl_id, TRUE);
                GUILIST_SetMaxSelectedItem(user_data_ptr->ctrl_id, (uint16)MMIMP3_GetMusicMarkNum());
                GUILIST_SetSelectedItem(user_data_ptr->ctrl_id, user_data_ptr->cur_list_selection, TRUE);
            }
            break;

        case ID_MP3_CANCEL_MARK:
            if (PNULL != user_data_ptr)
            {
                GUILIST_SetSelectedItem(user_data_ptr->ctrl_id, user_data_ptr->cur_list_selection, FALSE);

                if(0 == GUILIST_GetSelectedItemIndex(user_data_ptr->ctrl_id, PNULL, 0))
                {
                    GUILIST_SetTextListMarkable(user_data_ptr->ctrl_id, FALSE);
                }
            }

            break;

        case ID_MP3_MARK_ALL:
            if (PNULL != user_data_ptr)
            {
                GUILIST_SetTextListMarkable(user_data_ptr->ctrl_id, TRUE);
                GUILIST_SetMaxSelectedItem(user_data_ptr->ctrl_id, (uint16)MMIMP3_GetMusicMarkNum());
                GUILIST_SetAllSelected(user_data_ptr->ctrl_id, TRUE);
            }
            break;

        case ID_MP3_CANCEL_ALL_MARK:
            if (PNULL != user_data_ptr)
            {

                GUILIST_SetTextListMarkable(user_data_ptr->ctrl_id, FALSE);
            }
            break;
#else

        case ID_MP3_LIST_MARK:
            if (PNULL != user_data_ptr)
            {
                GUIFORM_SetChildDisplay(MMIMP3_MUSIC_MARK_LIST_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_NORMAL);
                GUILIST_SetTextListMarkable(user_data_ptr->ctrl_id, TRUE);
                GUILIST_SetMaxSelectedItem(user_data_ptr->ctrl_id, (uint16)MMIMP3_GetMusicMarkNum());
                user_data_ptr->is_markall = FALSE;
            }
            GUISOFTKEY_SetTextId(param->win_id, MMICOMMON_SOFTKEY_CTRL_ID, TXT_MARK_ALL, TXT_DELETE, STXT_CANCEL, TRUE);
            break;
#endif

        case ID_MP3_LIST_LIST_RENAME:
            if (PNULL != user_data_ptr)
            {
                OpenMarkListRenameWin(user_data_ptr->cur_list_selection);
            }
            break;

        case ID_MP3_LIST_DETAIL:
            if (PNULL != user_data_ptr)
            {
                OpenMarkListDetailWin(user_data_ptr->cur_list_selection);
            }
            break;

        case ID_MP3_MARK_ADD:
            if (PNULL != user_data_ptr)
            {
                GUILIST_SetTextListMarkable(user_data_ptr->ctrl_id, FALSE);
                add_result = MMIMP3_AddMusicMark(&cur_selection);

                if (SFS_NO_ERROR == add_result)
                {
                    MMK_SendMsg(param->dst_handle, MSG_MP3_UPDATE_MARK_LIST, (DPARAM)&cur_selection);
                }
                else if (SFS_ERROR_NO_SPACE == add_result)
                {
                    MMIPUB_OpenAlertWinByTextId(PNULL, TXT_NO_SPACE,
                                                TXT_NULL, IMAGE_PUBWIN_WARNING, &alert_win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
                }
                else if (SFS_ERROR_FOLD_FULL == add_result)
                {
                    MMIPUB_OpenAlertWinByTextId(PNULL, TXT_COMON_BOOKMARK_FULL, TXT_NULL,
                                                IMAGE_PUBWIN_WARNING, &alert_win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
                }
                else
                {
                    MMIPUB_OpenAlertWinByTextId(PNULL, TXT_ADD_FAIL, TXT_NULL,
                                                IMAGE_PUBWIN_FAIL, &alert_win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
                }
            }
            break;

        case ID_MP3_LIST_OPT_RETURN:
            MMK_ReturnAppointedWin(MMIMP3_MAIN_PLAY_WIN_ID);
            break;

        default:
            break;
        }
    }

}

/*****************************************************************************/
//  Description : handle music mark detail window message
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMp3MusicMarkDetailWinMsg(
    MMI_WIN_ID_T      win_id,
    MMI_MESSAGE_ID_E  msg_id,
    DPARAM            param
)
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    MMI_CTRL_ID_T       ctrl_id = MMIMP3_MUSIC_MARK_DETAIL_TEXT_CTRL_ID;
    GUI_POINT_T         dis_point = {0};
    MARKLIST_DETAIL_USER_DATA_T  *user_data_ptr = (MARKLIST_DETAIL_USER_DATA_T *)MMK_GetWinUserData(win_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        if (PNULL == user_data_ptr)
        {
            MMK_CloseWin(win_id);
            break;
        }
        SetMp3MarkDetail(ctrl_id, user_data_ptr->cur_list_selection);
        GUIRICHTEXT_SetFocusMode(ctrl_id, GUIRICHTEXT_FOCUS_SHIFT_NONE);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
        break;

    case MSG_FULL_PAINT:
        //display bg
        GUIRES_DisplayImg(&dis_point,
                          PNULL,
                          PNULL,
                          win_id,
                          IMAGE_COMMON_BG,
                          &lcd_dev_info);
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        if (PNULL != user_data_ptr)
        {
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
//  Description : Set detail of music mark.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void SetMp3MarkDetail(MMI_CTRL_ID_T                   ctrl_id,
                            uint16  cur_selection)
{
    MMI_STRING_T        detail_str = {0};
    uint16              nIndex = 0;
    uint8               uint8_str[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    wchar item_text[GUIRICHTEXT_TEXT_MAX_LEN + 1] = {0};
    MMIMP3_MUSIC_MARK_DETAIL_INFO_T mark_info = {0};

    MMIMP3_GetMusicMarkInfo(cur_selection, &mark_info);

    //mark name title
    MMI_GetLabelTextByLang(TXT_MP3_MARK_DETAIL_NAME, &detail_str);
    SetRichTextItemCharacter(ctrl_id, detail_str,&nIndex);

    //mark name
    detail_str.wstr_len = mark_info.mark_name_len;
    detail_str.wstr_ptr = mark_info.mark_name;
    SetRichTextItemCharacter(ctrl_id, detail_str,&nIndex);

    //song name title
    MMI_GetLabelTextByLang(TXT_MP3_MARK_DETAIL_SONG_NAME, &detail_str);
    SetRichTextItemCharacter(ctrl_id, detail_str,&nIndex);

    //song name
    MMIAPIFMM_SplitFullPath(
        mark_info.music_file_name, mark_info.music_file_name_len,
        PNULL, PNULL, PNULL, PNULL,
        item_text, &(detail_str.wstr_len)
    );
    detail_str.wstr_ptr = item_text;
    SetRichTextItemCharacter(ctrl_id, detail_str,&nIndex);

    //list name title
    MMI_GetLabelTextByLang(TXT_MP3_MARK_DETAIL_LIST_NAME, &detail_str);
    SetRichTextItemCharacter(ctrl_id, detail_str,&nIndex);


    //list name

    detail_str.wstr_ptr = item_text;
    if(GetMarkListName(&mark_info,&detail_str))
    {
        SetRichTextItemCharacter(ctrl_id, detail_str,&nIndex);
    }

    //offset time title
    MMI_GetLabelTextByLang(TXT_MP3_MARK_DETAIL_TIME, &detail_str);
    SetRichTextItemCharacter(ctrl_id, detail_str,&nIndex);

    //offset time
    SCI_MEMSET(uint8_str, 0x00, sizeof(uint8_str));
    sprintf((char*)uint8_str, "%02ld:%02ld", (uint32)(mark_info.mark_offset_in_time / 60), (mark_info.mark_offset_in_time % 60));
    MMIAPICOM_StrToWstr((const uint8 *)uint8_str, item_text);
    detail_str.wstr_ptr = item_text;
    detail_str.wstr_len = MMIAPICOM_Wstrlen(item_text);
    SetRichTextItemCharacter(ctrl_id, detail_str,&nIndex);
}


/*****************************************************************************/
//  Description :Set Rich Text Item Character
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void SetRichTextItemCharacter(MMI_CTRL_ID_T       ctrl_id,
                                    MMI_STRING_T        detail_str,
                                    uint16   *nIndex_ptr)
{
    GUIRICHTEXT_ITEM_T item_data = {0};

    if (PNULL == nIndex_ptr)
    {
        return;
    }

    item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;
    item_data.text_data.buf.len = detail_str.wstr_len;
    item_data.text_data.buf.str_ptr = detail_str.wstr_ptr;

    GUIRICHTEXT_AddItem(ctrl_id, &item_data, nIndex_ptr);
}

/*****************************************************************************/
//  Description :GetMarkListName
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetMarkListName(MMIMP3_MUSIC_MARK_DETAIL_INFO_T *mark_info_ptr,
                              MMI_STRING_T        *detail_str_ptr)
{
    BOOLEAN  result = FALSE;
    MMI_STRING_T list_name = {0};

    if(PNULL != detail_str_ptr
            && PNULL != mark_info_ptr
            && 0 < mark_info_ptr->list_name_len
            && PNULL != detail_str_ptr->wstr_ptr)
    {
        MMIAPIFMM_SplitFullPath(
            mark_info_ptr->list_name, mark_info_ptr->list_name_len,
            PNULL, PNULL, PNULL, PNULL,
            detail_str_ptr->wstr_ptr, &(detail_str_ptr->wstr_len)
        );

        if(0 == MMIAPICOM_CompareTwoWstrExt(
                    APLAYER_MY_MUSIC_NAME, APLAYER_MY_MUSIC_NAME_LEN,
                    detail_str_ptr->wstr_ptr, detail_str_ptr->wstr_len,
                    FALSE
                ))
        {
            MMI_GetLabelTextByLang(TXT_MP3_MY_MUSIC, &list_name);
            SCI_MEMSET(detail_str_ptr->wstr_ptr,0,(detail_str_ptr->wstr_len*sizeof(wchar)));
            detail_str_ptr->wstr_len = list_name.wstr_len;
            MMIAPICOM_Wstrncpy(detail_str_ptr->wstr_ptr,list_name.wstr_ptr,list_name.wstr_len);
            result = TRUE;
        }

#ifdef MMIAP_MUSIC_LIST_SUPPORT
        else if(0 == MMIAPICOM_CompareTwoWstrExt(
                    APLAYER_RES_LIST_1_NAME, APLAYER_RES_LIST_1_NAME_LEN,
                    detail_str_ptr->wstr_ptr, detail_str_ptr->wstr_len,
                    FALSE
                ))
        {
            MMI_GetLabelTextByLang(TXT_MP3_LIST_1, &list_name);
            SCI_MEMSET(detail_str_ptr->wstr_ptr,0,(detail_str_ptr->wstr_len*sizeof(wchar)));
            detail_str_ptr->wstr_len = list_name.wstr_len;
            MMIAPICOM_Wstrncpy(detail_str_ptr->wstr_ptr,list_name.wstr_ptr,list_name.wstr_len);
            result = TRUE;
        }
        else if(0 == MMIAPICOM_CompareTwoWstrExt(
                    APLAYER_RES_LIST_2_NAME, APLAYER_RES_LIST_2_NAME_LEN,
                    detail_str_ptr->wstr_ptr, detail_str_ptr->wstr_len,
                    FALSE
                ))
        {
            MMI_GetLabelTextByLang(TXT_MP3_LIST_2, &list_name);
            SCI_MEMSET(detail_str_ptr->wstr_ptr,0,(detail_str_ptr->wstr_len*sizeof(wchar)));
            detail_str_ptr->wstr_len = list_name.wstr_len;
            MMIAPICOM_Wstrncpy(detail_str_ptr->wstr_ptr,list_name.wstr_ptr,list_name.wstr_len);
            result = TRUE;
        }
        else if(0 == MMIAPICOM_CompareTwoWstrExt(
                    APLAYER_RES_LIST_3_NAME, APLAYER_RES_LIST_2_NAME_LEN,
                    detail_str_ptr->wstr_ptr, detail_str_ptr->wstr_len,
                    FALSE
                ))
        {
            MMI_GetLabelTextByLang(TXT_MP3_LIST_3, &list_name);
            SCI_MEMSET(detail_str_ptr->wstr_ptr,0,(detail_str_ptr->wstr_len*sizeof(wchar)));
            detail_str_ptr->wstr_len = list_name.wstr_len;
            MMIAPICOM_Wstrncpy(detail_str_ptr->wstr_ptr,list_name.wstr_ptr,list_name.wstr_len);
            result = TRUE;
        }
#endif
        else if(0 == MMIAPICOM_CompareTwoWstrExt(
                    MMIMP3_PLAY_LIST_SUFFIX_UCS, MMIMP3_PLAY_LIST_SUFFIX_UCS_LEN,
                    detail_str_ptr->wstr_ptr + detail_str_ptr->wstr_len - MMIMP3_PLAY_LIST_SUFFIX_UCS_LEN, MMIMP3_PLAY_LIST_SUFFIX_UCS_LEN,
                    FALSE
                ))
        {
            SCI_MEMSET(detail_str_ptr->wstr_ptr + detail_str_ptr->wstr_len - MMIMP3_PLAY_LIST_SUFFIX_UCS_LEN,0,(MMIMP3_PLAY_LIST_SUFFIX_UCS_LEN*sizeof(wchar)));
            result = TRUE;
        }

    }
    return result;
}

/*****************************************************************************/
//  Description :OpenMarkListDetailWin
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void OpenMarkListDetailWin(uint16 cur_list_selection)
{
    MARKLIST_DETAIL_USER_DATA_T  *user_data_ptr = PNULL;
    MMI_HANDLE_T                  win_handle = PNULL;

    user_data_ptr = SCI_ALLOCA(sizeof(MARKLIST_DETAIL_USER_DATA_T));
    if (PNULL != user_data_ptr)
    {
        win_handle = MMIAPAPPLET_CreateWinTable((uint32 *)MMIMP3_MUSIC_MARK_DETAIL_WIN_TAB, PNULL);
        user_data_ptr->cur_list_selection = cur_list_selection;

        MMK_SetWinUserData(win_handle,user_data_ptr);
    }
}

/*****************************************************************************/
//  Description :IsMarkNameValid
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsMarkNameValid(MMI_STRING_T    *file_name_ptr)
{
    uint16      i = 0;
    
    if (PNULL == file_name_ptr)
    {
        SCI_TRACE_LOW("IsMarkNameValid PNULL == file_name_ptr");
        return FALSE;
    }

    if(' ' == *(file_name_ptr->wstr_ptr))
    {
        return FALSE;
    }
	if(1 == file_name_ptr->wstr_len && ('.' == *file_name_ptr->wstr_ptr))
	{
		//name: ".", is invalid
		return FALSE;
	}
    for(; i < file_name_ptr->wstr_len; i++)
    {
        if('\n' == *(file_name_ptr->wstr_ptr + i))
        {
            return FALSE;
        } 
    }
    
    return TRUE;
}

/*****************************************************************************/
//  Description : handle renaming music mark window message
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMp3RenameMarkWinMsg(
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
)
{
    MMI_RESULT_E              result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T             ctrl_id = MMIMP3_RENAME_MARK_EDITBOX_CTRL_ID;
    MMI_STRING_T    name_str = {0};
    MMI_STRING_T              old_name_str = {0};
    uint16                    max_ucs2_len = 0;
    MMIMP3_MUSIC_MARK_DETAIL_INFO_T mark_info = {0};
    MMI_WIN_ID_T              alert_win_id = MMIMP3_ALERT_WIN_ID;
    MARKLIST_RENAME_USER_DATA_T  *user_data_ptr = (MARKLIST_RENAME_USER_DATA_T *)MMK_GetWinUserData(win_id);
    uint16 msg_param = 0;
    wchar                     new_name_str[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    uint16                    new_name_str_len = {0};

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        if (PNULL == user_data_ptr)
        {
            MMK_CloseWin(win_id);
            break;
        }
        max_ucs2_len = MMIMP3_MARK_NAME_MAX_LEN;
        GUIEDIT_SetTextMaxLen(ctrl_id, max_ucs2_len, max_ucs2_len);
        MMIMP3_GetMusicMarkInfo(user_data_ptr->cur_list_selection , &mark_info);
        old_name_str.wstr_ptr = mark_info.mark_name;
        old_name_str.wstr_len = mark_info.mark_name_len;
        GUIEDIT_SetString(ctrl_id, old_name_str.wstr_ptr, old_name_str.wstr_len);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        if (PNULL == user_data_ptr)
        {
            MMK_CloseWin(win_id);
            break;
        }
        GUIEDIT_GetString(ctrl_id, &name_str);
        
        if(IsMarkNameValid(&name_str)
			&& MMIAPICOM_GetValidFileName(FALSE, name_str.wstr_ptr, name_str.wstr_len, new_name_str, MMIFILE_FILE_NAME_MAX_LEN))
        {
            new_name_str_len = MMIAPICOM_Wstrlen(new_name_str);
            name_str.wstr_ptr = new_name_str;
            name_str.wstr_len = new_name_str_len;
            if(MMIMP3_UpdateMusicMarkName(user_data_ptr->cur_list_selection, name_str))
            {
                msg_param = user_data_ptr->cur_list_selection;
                MMK_SendMsg(MMIMP3_MUSIC_MARK_WIN_ID, MSG_MP3_UPDATE_MARK_LIST, &msg_param);
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
//  Description :OpenMarkListRenameWin
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void OpenMarkListRenameWin(uint16 cur_list_selection)
{
    MARKLIST_RENAME_USER_DATA_T  *user_data_ptr = PNULL;
    MMI_HANDLE_T                  win_handle = PNULL;

    user_data_ptr = SCI_ALLOCA(sizeof(MARKLIST_RENAME_USER_DATA_T));
    if (PNULL != user_data_ptr)
    {
        win_handle = MMIAPAPPLET_CreateWinTable((uint32 *)MMIMP3_RENAME_MARK_WIN_TAB, PNULL);
        user_data_ptr->cur_list_selection = cur_list_selection;

        MMK_SetWinUserData(win_handle,user_data_ptr);
    }
}

/*****************************************************************************/
//  Description :  Handle list delete waiting win
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMarkDeleteWaitingWin(
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
)
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_WIN_ID_T        alert_win_id    = MMIMP3_ALERT_WIN_ID;
    uint16              new_selection = 0;
    MARKLIST_DEL_INFO   **del_info_ptr = (MARKLIST_DEL_INFO   **)param;
    switch(msg_id)
    {
    case MSG_APP_CANCEL:
    case MSG_APP_RED:
        break;

    case MSG_MP3_DEL_MARK_INFO:
        if (PNULL != del_info_ptr
                && PNULL != *del_info_ptr)
        {
            new_selection = (*del_info_ptr)->list_selection;
            if(!MMIMP3_DelMusicMark((*del_info_ptr)->select_item_ptr, (*del_info_ptr)->select_num, &new_selection))
            {
                MMIPUB_OpenAlertWinByTextId(PNULL, TXT_COMMON_DEL_FAIL, TXT_NULL,
                                            IMAGE_PUBWIN_FAIL, &alert_win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
            }
            MMK_SendMsg((*del_info_ptr)->list_win_id, MSG_MP3_UPDATE_MARK_LIST, (DPARAM)&new_selection);
            if (PNULL != (*del_info_ptr)->select_item_ptr)
            {
                SCI_FREE((*del_info_ptr)->select_item_ptr);
            }
            SCI_FREE(*del_info_ptr);
        }
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
//  Description :  Handle list delete waiting win
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN OpenDelMarkListWin(MMI_HANDLE_T win_id,
                                 MMI_CTRL_ID_T list_ctrl_id,
                                 uint16 list_selection)
{
    MARKLIST_DEL_INFO   *del_info_ptr = PNULL;
    MMI_WIN_ID_T        waiting_win_id = MMIMP3_DEL_FILE_WAIT_WIN_ID;
    uint32              waiting_time_out = 0;
    uint16              mark_num = 0;
    BOOLEAN             result = FALSE;

    del_info_ptr = SCI_ALLOCA(sizeof(MARKLIST_DEL_INFO));
    if (PNULL != del_info_ptr)
    {
        SCI_MEMSET(del_info_ptr,0,sizeof(MARKLIST_DEL_INFO));
        mark_num = GUILIST_GetSelectedItemIndex(list_ctrl_id, PNULL, 0);
        if (0 != mark_num)
        {
            del_info_ptr->select_num = mark_num;
        }
        else
        {
            del_info_ptr->select_num = 1;
        }

        del_info_ptr->list_win_id = win_id;
        del_info_ptr->list_selection = list_selection;
        del_info_ptr->select_item_ptr = (uint16*)(SCI_ALLOCA(sizeof(uint16) * del_info_ptr->select_num));
        if (PNULL != del_info_ptr->select_item_ptr)
        {
            if (0 != mark_num)
            {
                GUILIST_GetSelectedItemIndex(list_ctrl_id, del_info_ptr->select_item_ptr, mark_num);
            }
            else
            {
                del_info_ptr->select_item_ptr[0] = list_selection;
            }

            MMIPUB_OpenAlertWinByTextIdEx(SPRD_MP3_APPLET_ID,
                                          &waiting_time_out, TXT_COMMON_WAITING, TXT_NULL,
                                          IMAGE_PUBWIN_WAIT, &waiting_win_id, PNULL, MMIPUB_SOFTKEY_NONE, HandleMarkDeleteWaitingWin,
                                          PNULL);

            MMK_PostMsg(waiting_win_id, MSG_MP3_DEL_MARK_INFO, &del_info_ptr, sizeof(del_info_ptr));
            result = TRUE;
        }
        else
        {
            SCI_FREE(del_info_ptr);
        }

    }

    return result;
}

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : open mark list Long Ok context menu
//  Global resource dependence :
//  Author: xin.li
//  Note: 音乐书签列表长按LIST弹出context menu
/*****************************************************************************/
LOCAL void  OpenMarklistLongOkOptMenu(MMI_WIN_ID_T         dst_win_id)
{
    MMIPUB_MENU_PARAM_T menuparam = {0};

    menuparam.group_id = MENU_DYNA;
    menuparam.win_id = (MMI_WIN_ID_T)MMIMP3_MUSIC_MARK_LONGOK_OPT_WIN_ID;
    menuparam.ctrl_id = (MMI_CTRL_ID_T)MMIMP3_MUSIC_MARK_LONGOK_OPT_MENU_CTRL_ID;
    menuparam.applet_handle = SPRD_MP3_APPLET_ID;

    menuparam.dst_handle = dst_win_id;
    menuparam.func.OnMenuOk = HandleMarkListPubMenuOKHandler;
    menuparam.func.OnMenuInit = HandleMarkListLONGPubMenuInitHandler;
    menuparam.menu_style = GUIMENU_STYLE_POPUP_AUTO;
    MMIPUB_OpenPubMenuWin(&menuparam);
}

/*****************************************************************************/
//  Description : handle pub menu win init handler
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandleMarkListLONGPubMenuInitHandler(MMIPUB_MENU_NOTIFY_T *param)
{
    MARKLIST_USERDATA_INFO_T *user_data_ptr = PNULL;
    GUIMENU_TITLE_INFO_T    title_info = {0};
    MMI_STRING_T            main_title = {0};
    MMI_STRING_T            sub_title = {0};

    MMIAP_MENU_ITEM_T menu_list[]=
    {
        //删除
        {ID_MP3_LIST_DEL, 0},
        //重命名
        {ID_MP3_LIST_LIST_RENAME, 0},
        //详情
        {ID_MP3_LIST_DETAIL, 0}
    };

    if (PNULL != param)
    {
        user_data_ptr = (MARKLIST_USERDATA_INFO_T *)MMK_GetWinUserData(param->dst_handle);
        //GUIMENU_CreatDynamic(PNULL, param->win_id, param->ctrl_id, GUIMENU_STYLE_POPUP_AUTO);


        MMIAP_BuildMenuToCtrl(menu_list, ARR_SIZE(menu_list),
                              PNULL,
                              FALSE,
                              param->ctrl_id
                             );


        if (PNULL != user_data_ptr)
        {
            //set title
            MMIAP_GetListItemTextInfo(user_data_ptr->ctrl_id, user_data_ptr->cur_list_selection, &main_title, &sub_title);
            title_info.is_static    = FALSE;
            title_info.title_ptr    = &main_title;
            title_info.sub_title_ptr = &sub_title;
            GUIMENU_SetMenuTitleEx(param->ctrl_id, &title_info);
            SetMarkListLongOkOptMenuGreyStatus(param->ctrl_id,
                                               user_data_ptr->ctrl_id,
                                               user_data_ptr->cur_list_selection,
                                               MMIMP3_GetMusicMarkNum());
        }

    }
}

/*****************************************************************************/
//  Description : set Context Menu Grey Status
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void  SetMarkListLongOkOptMenuGreyStatus(MMI_CTRL_ID_T   ctrl_id,
        MMI_CTRL_ID_T   list_ctrl_id,
        uint16          cur_selection,
        uint16          total_num)
{
    //GUILIST_ITEM_T  *list_item_ptr   = PNULL;
    MMIAP_STATIC_MENU_GRAY_PARAM_T menu_gray_param = {0};

    MMIAP_MENU_ITEM_T menu_group1[]=
    {
        {ID_MP3_LIST_LIST_RENAME, 0},
        {ID_MP3_LIST_DETAIL, 0} //详情
    };

    MMIAP_MENU_ITEM_T menu_group2[]=
    {
        {ID_MP3_LIST_DEL, 0},//删除
        {ID_MP3_LIST_LIST_RENAME, 0}, //重命名
        {ID_MP3_LIST_DETAIL ,0}//详情
    };


    if(0 < total_num)
    {
       // list_item_ptr = GUILIST_GetItemPtrByIndex(list_ctrl_id, cur_selection); /*lint !e605*/
        if(GUILIST_GetListStateById(list_ctrl_id, GUILIST_STATE_ENABLE_MARK))
        {
            menu_gray_param.menu_group_ptr = menu_group1;
            menu_gray_param.total_num = ARR_SIZE(menu_group1);
            menu_gray_param.is_gray = TRUE;
            menu_gray_param.ctrl_id = ctrl_id;
            menu_gray_param.gray_func = PNULL;
            MMIAP_BuildMenuGrayByCtrl(&menu_gray_param);

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
}

#endif //MMI_PDA_SUPPORT
#endif //MMIAP_MUSICMARK_SUPPORT
