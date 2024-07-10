/*****************************************************************************
** File Name:      mmibrowser_wintable_bookmark.c                            *
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
#include "browser_text.h"
#include "mmibrowser_id.h"
#include "mmibrowser_menutable.h"
#include "guirichtext.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmibrowser_bookmark.h"
#include "browser_image.h"
#include "mmibrowser_setting.h"
#include "mmipub.h"
#include "cfl_wchar.h"
#include "cfl_char.h"
#include "guiedit.h"
#include "guiim_base.h"
#include "mmi_appmsg.h"
#include "guiform.h"
#include "guilabel.h"
#include "mmisms_export.h"
#include "mmifmm_image.h"
#include "mmifmm_text.h"
#include "mmifmm_export.h"
#include "guisetlist.h"
#include "mmibrowser_wintable.h"
#include "mmibrowser_func.h"
#include "mmi_resource.h"
#include "guitab.h"
#ifdef SNS_SUPPORT
#include "mmisns_srv.h"
#endif

#ifdef BROWSER_START_PAGE_THEME1
#include "mmi_slide.h"
#endif
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                *
 **--------------------------------------------------------------------------*/
//#define MMIBRW_INLINE_IME_DEFAULT_INPUT (100)
#define MMIBRW_BOOKMARK_TITLE_INDEX   (1)
#define MMIBRW_BOOKMARK_URL_INDEX     (3)
//#define MMIBROWSER_BOOKMARK_PROPERTY_EXT_LEN     (30)

/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/
/*新建书签与之前书签的同名情况:*/
typedef enum _MMIBROWSER_BOOKMARK_TITLE_URL
{
    MMIBROWSER_BOOKMARK_NO_SAME_TITLE_URL  = 0,
    MMIBROWSER_BOOKMARK_SAME_TITLE_URL,
    MMIBROWSER_BOOKMARK_SAME_TITLE_DIFF_URL,
    MMIBROWSER_BOOKMARK_DIFF_TITLE_SAME_URL,
    MMIBROWSER_BOOKMARK_TITLE_URL_MAX
}MMIBROWSER_BOOKMARK_TITLE_URL_E;

typedef enum _MMIBROWSER_BK_MOVE_RESULT_E_
{
    MMIBROWSER_BK_MOVE_RESULT_NONE  = 0,
    MMIBROWSER_BK_MOVE_RESULT_SUCCESS = MMIBROWSER_BK_MOVE_RESULT_NONE,
    MMIBROWSER_BK_MOVE_RESULT_SAME_PATH,
    MMIBROWSER_BK_MOVE_RESULT_ERROR,
    MMIBROWSER_BK_MOVE_RESULT_MAX
}MMIBROWSER_BK_MOVE_RESULT_E;

typedef enum _MMIBROWSER_BK_REPLACE_RESULT_E_
{
    MMIBROWSER_BK_REPLACE_NONE  = 0,
    MMIBROWSER_BK_REPLACE_SUCCESS = MMIBROWSER_BK_REPLACE_NONE,
    MMIBROWSER_BK_REPLACE_DIR,
    MMIBROWSER_BK_REPLACE_FILE,
    MMIBROWSER_BK_REPLACE_DUPLICATE,
    MMIBROWSER_BK_REPLACE_ERROR,
    MMIBROWSER_BK_REPLACE_MAX
}MMIBROWSER_BK_REPLACE_RESULT_E;

typedef struct
{
    uint16 array_index;
    //FILEARRAY_TYPE_E type;
}BRW_BOOKMARK_OPT_INFO_T;


#ifdef SNS_SUPPORT
LOCAL IMMISns *pMe  = PNULL;

#if (defined(WRE_WEIBO_SUPPORT)) || (defined(WRE_FACEBOOK_SUPPORT)) || (defined(WRE_TWITTER_SUPPORT))

const uint32 s_menu_brwbm_share_sns_popupmenu_table[][2] = 
{
#ifdef WRE_WEIBO_SUPPORT
    {ID_POPUP_MENU_BRW_SHARE_TO_SINA,TXT_SHARE_TO_SINA},
#endif
#ifdef WRE_FACEBOOK_SUPPORT
    {ID_POPUP_MENU_BRW_SHARE_TO_FACEBOOK,TXT_SHARE_TO_FACEBOOK},
#endif
#ifdef WRE_TWITTER_SUPPORT
    {ID_POPUP_MENU_BRW_SHARE_TO_TWITTER,TXT_SHARE_TO_TWITTER},
#endif  	
};

#endif
#endif
    
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
#ifndef MMIDORADO_BOOKMARK_BY_NV
LOCAL BOOLEAN s_is_in_bookmark_moving = FALSE;//是否处于书签的moving状态
LOCAL BRW_BOOKMARK_OPT_INFO_T s_bookmark_cur_index_info = {0};//the info that is handled for bookmark option window
LOCAL FILEARRAY_DATA_T s_bookmark_move_array_data = {0};//当前被move的书签的file array data
LOCAL uint32    s_diplay_route_index = 0;   //当前bookmark所属的文件夹index
LOCAL uint32    s_former_route_index = 0;   //原来bookmark所属的文件夹index
LOCAL uint16    s_pre_path_index = 0;//用于记录之前的焦点项

LOCAL BOOLEAN       s_file_del_bookmark_is_cancelled = FALSE;     //delete file is canceled
LOCAL uint16        s_bookmark_del_total_num = 0;   //总共要被删除的书签总数
//LOCAL int32        s_bookmark_moveto_duplicate_index = 0;//移动书签时相同目录内的书签ID
#endif/*MMIDORADO_BOOKMARK_BY_NV*/

LOCAL BOOLEAN   s_is_append_when_paint = FALSE;

#ifdef BROWSER_START_PAGE_THEME1
LOCAL BOOLEAN   s_is_bk_need_reset = FALSE;//TRUE:ret to the open_window state(in root)
LOCAL BOOLEAN s_is_need_restore_list_slide = FALSE; //是否需要恢复list的slide状态
#endif

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : handle bookmark window message
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleBookmarkWinMsg(
                                   MMI_WIN_ID_T	win_id, 
                                   MMI_MESSAGE_ID_E	msg_id, 
                                   DPARAM param
                                   );

/*****************************************************************************/
//  Description : handle bookmark edit window message
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleBookMarkEditWinMsg(
                                   MMI_WIN_ID_T	win_id, 
                                   MMI_MESSAGE_ID_E	msg_id, 
                                   DPARAM param
                                   );

/*****************************************************************************/
//  Description : handle bookmark popmenu window message
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleBookmarkPopMenuWinMsg(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            );

/*****************************************************************************/
//  Description : handle bookmark detail window message
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleBookmarkDetailWinMsg(
                                           MMI_WIN_ID_T	win_id, 
                                           MMI_MESSAGE_ID_E	msg_id, 
                                           DPARAM param
                                           );

/*****************************************************************************/
// 	Description : handle bookmark list mark option
//	Global resource dependence : none
//  Author: li.li
//	Note:
/*****************************************************************************/
LOCAL void BrwHandleBookmarkListMarked(
                               MMI_CTRL_ID_T  list_ctrl_id
                               );

/*****************************************************************************/
// 	Description : handle bookmark list mark cancel option
//	Global resource dependence : none
//  Author: li.li
//	Note:
/*****************************************************************************/
LOCAL void BrwHandleBookmarkListUnmarked(
                                 MMI_CTRL_ID_T    list_ctrl_id
                                 );

/*****************************************************************************/
// 	Description : handle bookmark list mark all option
//	Global resource dependence : none
//  Author: li.li
//	Note:
/*****************************************************************************/
LOCAL void BrwHandleBookmarkListMarkedAll(
                                  MMI_CTRL_ID_T   list_ctrl_id
                                  );

/*****************************************************************************/
// 	Description : handle bookmark list mark all cancel option
//	Global resource dependence : none
//  Author: li.li
//	Note:
/*****************************************************************************/
LOCAL void BrwHandleBookmarkListUnmarkedAll(
                                    MMI_CTRL_ID_T list_ctrl_id
                                    );

/*****************************************************************************/
//  Description : set bookmark edit window message
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void BrwSetBookmarkEditWindow(MMIBRW_BM_CHANGE_TYPE_E change_type);

/*****************************************************************************/
//  Description : set the bookmark detail window
//  Global resource dependence : 
//  Author: dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL void BrwSetBookmarkDetailWindow(MMI_CTRL_ID_T ctrl_id,uint16 list_index);

/*****************************************************************************/
// 	Description : query is delete bookmark
//	Global resource dependence : none
//  Author: li.li
//	Note:
/*****************************************************************************/
LOCAL void BrwQueryIsDeleteBookmark(
                             MMI_CTRL_ID_T list_ctrl_id   //in
                             );

#ifndef MMIDORADO_BOOKMARK_BY_NV
/*****************************************************************************/
//  Description : move one bookmark
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN  BrwBookmarkMoveTo(
                           const wchar *full_path_ptr,     //in
                           uint16     full_path_len       //in, 双字节为单位
                           );

/*****************************************************************************/
//  Description : begin to move one bookmark
//  Global resource dependence : none
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL MMIBROWSER_BK_MOVE_RESULT_E BrwBookmarkBeginMoveTo(uint16 cur_index);

/*****************************************************************************/
// Description : get the bookmark filearray index that need to be handled
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
LOCAL uint16 BrwGetBookmarkCurIndex(void);

/*****************************************************************************/
// Description : get the bookmark filearray index that need to be handled
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
LOCAL void BrwSetBookmarkCurIndex(uint16 current_index);

/*****************************************************************************/
// Description : Handle replace the duplicated bookmark 
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIBRW_HandleReplaceDuplicatedBookmark(MMI_WIN_ID_T      win_id, 
                                          MMI_MESSAGE_ID_E  msg_id, 
                                          DPARAM            param);
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : handle bookmark detail popmenu window message
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleBookmarkPopUpWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  BrwCreateBookmarkPopUpMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void BrwOpenBookamrkPropertyWin(void);

/*****************************************************************************/
// Description :
// Global resource dependence : none
// Author:Jiaoyou.wu
// Note:    
/*****************************************************************************/
LOCAL MMI_RESULT_E  BrwHandleBookmarkAlertWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
// 	Description : enter bookmark list marked status
//	Global resource dependence : none
//  Author: fen.xie
//	Note:
/*****************************************************************************/
LOCAL void BrwEnterBookmarkListMarked(
                               MMI_CTRL_ID_T  list_ctrl_id
                               );
                               
/*****************************************************************************/
// 	Description : exit bookmark list marked status
//	Global resource dependence : none
//  Author: fen.xie
//	Note:
/*****************************************************************************/
LOCAL void BrwExitBookmarkListMarked(
                               MMI_CTRL_ID_T  list_ctrl_id
                               );
                
#endif /* MMI_PDA_SUPPORT */

#ifndef MMIDORADO_BOOKMARK_BY_NV
/*****************************************************************************/
//  Description : Wstricmp
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC int MMIBRW_Wstricmp(const wchar * string1, const wchar * string2);

/*****************************************************************************/
//  Description :enter the bookmark moving state and back to bookmark list window to choose
//  Global resource dependence : none
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL void BrwBookmarkEnterMoveState(MMI_WIN_ID_T win_id, uint16 array_index);

/*****************************************************************************/
//  Description : Delete bookmark(one bookmark, one bookmark dir or delete all)
//  Global resource dependence : none
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleBrowserDeleteBookmark(MMI_CTRL_ID_T ctrl_handle, BOOLEAN is_delete_all);
#endif

/*****************************************************************************/
//  Description : handle bookmark detail popmenu window message
//  Global resource dependence : 
//  Author: dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleBookmarkDetailPopMenuWinMsg(MMI_WIN_ID_T win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param);

#ifndef MMIDORADO_BOOKMARK_BY_NV
/*****************************************************************************/
//  Description: 修改路径文件夹的index
//  Global resource dependence : 
//  Author:dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL void BrwSetPathFolderIndex(uint32 index);

/*****************************************************************************/
//  Description:获取 修改路径文件夹的index
//  Global resource dependence : 
//  Author:dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL uint32 BrwGetPathFolderIndex(void);

/*****************************************************************************/
//  Description: 记下刚进入时的路径文件夹的index
//  Global resource dependence : 
//  Author:dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL void BrwSetPathOringalFolderIndex(uint32 index);

/*****************************************************************************/
//  Description:获取 刚进入时的原始路径文件夹的index
//  Global resource dependence : 
//  Author:dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL uint32 BrwGetOringalPathFolderIndex(void);
/*****************************************************************************/
//  Description:检查bookmark的合理性并进行修订
//  Global resource dependence : 
//  Author:liyan.zhu
//  Note: 修订bookmark显示：若显示为INDEX OF/修订为INDEX OF/.
/*****************************************************************************/
LOCAL BOOLEAN BrwChkAndMdBookMarkName(MMI_STRING_T* title_text_str);

/*****************************************************************************/
//  Description : delete Delete Marked Bookmark Item
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:
/*****************************************************************************/
LOCAL void BrwDeleteMarkedBookmarkItem(
                             MMI_CTRL_ID_T      list_ctrl_id,   //in
                             MMI_WIN_ID_T       wait_win_id,    //in
                             MMI_WIN_ID_T       win_id          //in
                             );

/*****************************************************************************/
//  Description : handle delete selected bookmark confirm message
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:
/*****************************************************************************/
LOCAL void HandleDeleteBookmarkCnf(
                              BOOLEAN           is_success,     //in:delete is success
                              MMI_CTRL_ID_T     list_ctrl_id,   //in:
                              MMI_WIN_ID_T      wait_win_id,    //in:
                              MMI_WIN_ID_T      win_id          //in:
                              );

/*****************************************************************************/
//  Description : to handle the message of progress window    
//  Global resource dependence : 
//  Author:dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleProcessWinMsg(
                                        MMI_WIN_ID_T     win_id, 
                                        MMI_MESSAGE_ID_E    msg_id, 
                                        DPARAM              param
                                        );
                                        
/*****************************************************************************/
//  Description:save bookmark or folder in edit bookmark window
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMIBROWSER_BK_REPLACE_RESULT_E BrowserHanldeSaveInEditWin(
                MMI_WIN_ID_T win_id, 
                MMIBRW_BM_CHANGE_TYPE_E change_type,
                uint16 *duplicated_index_ptr
                );

/*****************************************************************************/
//  Description : add dynamic item to listbox when item_num is not zero
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC void MMIBRW_AppendListboxitem( 
                                                        MMI_WIN_ID_T win_id, 
                                                        MMI_CTRL_ID_T ctrl_id, 
                                                        uint16 item_num
                                                        );
                                                        
/*****************************************************************************/
//  Description :  Handle open memo main waiting win
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBKOpenMainWaitingWin(
                         MMI_WIN_ID_T       win_id,     
                         MMI_MESSAGE_ID_E   msg_id, 
                         DPARAM             param
                         );
#endif
                         
/*****************************************************************************/
//  Description :  Handle open memo main waiting win
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
LOCAL void BrwUpdateBKListWindow(
                const MMI_STRING_T *title_str_ptr,
                uint16 *current_index_ptr   //current index
                );

#ifndef MMIDORADO_BOOKMARK_BY_NV
/*****************************************************************************/
//  Description : open the bookmark window
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL void BrwOpenBookmarkStepWin(                        
                        MMIBRW_BOOKMARK_STEP_TYPE_E step_type,//in
                        uint16 cur_array_index,//in:load 根目录时无效
                        MMIFMM_FUNC_TYPE_E   fmm_func_type//in
                        );
                        
/*****************************************************************************/
//  Description :  Handle open memo main waiting win
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBKOpenStepWaitingWin(
                         MMI_WIN_ID_T       win_id,     
                         MMI_MESSAGE_ID_E   msg_id, 
                         DPARAM             param
                         );

/*****************************************************************************/
//  Description :  Handle open memo main waiting win
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBKSaveWaitingWin(
                         MMI_WIN_ID_T       win_id,     
                         MMI_MESSAGE_ID_E   msg_id, 
                         DPARAM             param
                         );
/*****************************************************************************/
//  Description : prepare to go back to root path
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL void BrwPrepare2BackToRoot(void);

/*****************************************************************************/
//  Description : open the bookmark window
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL void BrwBegin2Save(MMIBRW_BM_CHANGE_TYPE_E change_type);

/*****************************************************************************/
//  Description : search the same name in album list
//  Global resource dependence : 
//  Author: liyan.zhu
//  Return: TRUE: contain invalid char. FALSE: do not contain invalid char
//  Note:copy from MMIAPICOM_IsIncludeInvalidChar
/*****************************************************************************/
LOCAL BOOLEAN BrwBookMark_IsIncludeInvalidChar(
											   const wchar* name_ptr, 
											   uint16 len//, 
											   //BOOLEAN is_ucs2
                                              );

/*****************************************************************************/
//  Description:分析编辑内容是否有效,并进行相应的提示
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN BrwParserEditInfoIsValid(
                MMIBRW_BM_CHANGE_TYPE_E change_type,
                MMI_STRING_T title_text_str,
                MMI_STRING_T url_text_str
                );
#endif

/*****************************************************************************/
//  Description:update the boomakr detail window by new infomation
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
LOCAL void BrwUpdateBKDetailWinInfo(
                MMI_HANDLE_T detail_win_handle,
                MMI_STRING_T title_text_str,
                MMI_STRING_T url_text_str
                );
                               
/*****************************************************************************/
//	Description : the handle function of bookmark list delete window
//	Global resource dependence : 
//	Author:fen.xie
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleBookmarkDelQueryWinMsg(
                MMI_WIN_ID_T win_id, 
                MMI_MESSAGE_ID_E msg_id, 
                DPARAM param
                );

#ifdef SNS_SUPPORT           
/*****************************************************************************/
//  Description:handle the SNS option menu item
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
LOCAL void BrwHandleSnsOptMenu(MMI_STRING_T *url_str_ptr, MMI_MENU_ID_T menu_id);
#endif

/*****************************************************************************/
//  Description : Tidy all bookmark windows
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
LOCAL void BookmarkTidyExistedBkWin(void);

/*****************************************************************************/
//  Description : get bookmark's file array index from bookmark list index
//  Global resource dependence : none
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL uint16 BrwGetBookmarkArrayIndex(uint16 cur_list_index);

/**--------------------------------------------------------------------------*
 **                         WINDOW DEFINITION                              *
 **--------------------------------------------------------------------------*/
WINDOW_TABLE(MMIBROWSER_BOOKMARK_WIN_TAB) = 
{
#ifndef BROWSER_BOOKMARK_HISTORY_TAB
    WIN_TITLE(TXT_BOOKMARK),
#endif	
    WIN_FUNC((uint32)BrwHandleBookmarkWinMsg),    
    WIN_ID(MMIBROWSER_BOOKMARK_WIN_ID),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_DISABLE_SOFTKEY_TO_TITLE),

    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIBROWSER_BOOKMARK_FORM_CTRL_ID),
//    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIBROWSER_BOOKMARK_SOFTKEY_CHILD_FORM_CTRL_ID,MMIBROWSER_BOOKMARK_FORM_CTRL_ID),
    CHILD_SOFTKEY_CTRL(TXT_MARK_ALL, TXT_DELETE, STXT_CANCEL, MMICOMMON_SOFTKEY_CTRL_ID, MMIBROWSER_BOOKMARK_FORM_CTRL_ID),
//    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIBROWSER_BOOKMARK_LIST_CHILD_FORM_CTRL_ID,MMIBROWSER_BOOKMARK_FORM_CTRL_ID),
    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, MMIBROWSER_LIST_BOOKMARK_CTRL_ID, MMIBROWSER_BOOKMARK_FORM_CTRL_ID),
#else
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIBROWSER_LIST_BOOKMARK_CTRL_ID),
#endif /* MMI_PDA_SUPPORT */
    END_WIN
};

WINDOW_TABLE(MMIBROWSER_BOOKMARK_EDIT_WIN_TAB) = 
{
    WIN_TITLE(TXT_EDIT),
    WIN_FUNC((uint32)BrwHandleBookMarkEditWinMsg),    
    WIN_ID(MMIBROWSER_BOOKMARK_EDIT_WIN_ID),
#ifndef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif /* MMI_PDA_SUPPORT */
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIBROWSER_BOOKMARK_EDIT_CTRL_ID),
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIBROWSER_BK_EDIT_FORM1_CTRL_ID,MMIBROWSER_BOOKMARK_EDIT_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIBROWSER_BK_EDIT_LABEL1_CTRL_ID, MMIBROWSER_BK_EDIT_FORM1_CTRL_ID),
            CHILD_EDIT_TEXT_CTRL(TRUE, MMIBRW_BOOKMARK_MAX_TITLE_LENGTH,MMIBROWSER_BOOKMARK_EDIT_TITLE_CTRL_ID, MMIBROWSER_BK_EDIT_FORM1_CTRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIBROWSER_BK_EDIT_FORM2_CTRL_ID,MMIBROWSER_BOOKMARK_EDIT_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIBROWSER_BK_EDIT_LABEL2_CTRL_ID, MMIBROWSER_BK_EDIT_FORM2_CTRL_ID),
            CHILD_EDIT_TEXT_CTRL(TRUE, MMIBROWSER_MAX_URL_LEN,MMIBROWSER_BOOKMARK_EDIT_URL_CTRL_ID, MMIBROWSER_BK_EDIT_FORM2_CTRL_ID),

#ifndef MMIDORADO_BOOKMARK_BY_NV
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIBROWSER_BK_EDIT_FORM3_CTRL_ID,MMIBROWSER_BOOKMARK_EDIT_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIBROWSER_BK_EDIT_LABEL3_CTRL_ID, MMIBROWSER_BK_EDIT_FORM3_CTRL_ID),
            CHILD_SETLIST_CTRL(TRUE,MMIBROWSER_BK_EDIT_FORM3_SETLIST_CTRL_ID, MMIBROWSER_BK_EDIT_FORM3_CTRL_ID),
#endif

#ifdef MMI_PDA_SUPPORT
    CHILD_SOFTKEY_CTRL(STXT_SAVE, TXT_NULL, STXT_RETURN,MMICOMMON_SOFTKEY_CTRL_ID,MMIBROWSER_BOOKMARK_EDIT_CTRL_ID),
#endif
 
    END_WIN
};

WINDOW_TABLE( MMIBROWSER_BOOKMARK_POPMENU_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)BrwHandleBookmarkPopMenuWinMsg),
    WIN_ID(MMIBROWSER_BOOKMARK_POPMENU_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_HIDE_STATUS,
    CREATE_POPMENU_CTRL(MENU_BRW_BOOKMARK,MMIBROWSER_BOOKMARK_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};

WINDOW_TABLE(BRW_BOOKMARK_DETAIL_WIN_TAB) = 
{
    WIN_TITLE(STXT_DETAIL),
    WIN_FUNC((uint32)BrwHandleBookmarkDetailWinMsg),    
    WIN_ID(MMIBROWSER_BOOKMARK_DETAIL_WIN_ID),
    WIN_SOFTKEY(STXT_OPTION, TXT_COMM_OPEN, STXT_RETURN),
    CREATE_RICHTEXT_CTRL(MMIBROWSER_BOOKMARK_DETAIL_CTRL_ID),
    
    END_WIN
};


WINDOW_TABLE( MMIBROWSER_BOOKMARK_DETAIL_POPMENU_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)BrwHandleBookmarkDetailPopMenuWinMsg),
    WIN_ID(MMIBROWSER_BOOKMARK_DETAIL_POPMENU_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_HIDE_STATUS,
    CREATE_POPMENU_CTRL(MENU_BRW_BOOKMARK_DETAIL_OPT,MMIBROWSER_BOOKMARK_DETAIL_OPTION_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};

#ifdef MMI_PDA_SUPPORT
WINDOW_TABLE(MMIBROWSER_BOOKMARK_POPUP_WIN_TAB) = 
{
    WIN_FUNC((uint32) BrwHandleBookmarkPopUpWinMsg), 
    WIN_ID(MMIBROWSER_BOOKMARK_POPUP_WIN_ID),
    WIN_HIDE_STATUS,
    END_WIN
};
#endif /* MMI_PDA_SUPPORT */

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
 /****begin:封装mmi pub alert window的API的原因,是为了browser的重入时能及时关闭browser模块的所有窗口;
因此browser模块内使用query pub接口,必须调用以下几个;未封装MMIPUB_Open的所有api,需要可以再扩展****/
/*****************************************************************************/
//  Description : 打开bookmark alert window by text ptr
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL void BrwOpenAlertWinByTextPtr(
                                         uint32                    *time_period_ptr,   //time period,default is 3s
                                         MMI_STRING_T              *text1_ptr,         //text 1
                                         MMI_STRING_T              *text2_ptr,         //text 2
                                         MMI_IMAGE_ID_T            image_id,           //alert image id
                                         MMI_WIN_PRIORITY_E        *win_priority_ptr,  //win priority,default is WIN_ONE_LEVEL
                                         MMIPUB_SOFTKEY_STYLE_E    softkey_style,      //softkey style
                                         MMIPUB_HANDLE_FUNC        handle_func         //handle function
                                         )
{
    MMI_WIN_ID_T    alert_win_id = MMIBROWSER_BOOKMARK_ALERT_WIN_ID;
    MMIPUB_OpenAlertWinByTextPtr(time_period_ptr, 
            text1_ptr, text2_ptr, 
            image_id, 
            &alert_win_id, 
            win_priority_ptr, 
            softkey_style,
            handle_func);
}

/*****************************************************************************/
//  Description : 打开bookmark alert window by text id
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL void BrwOpenAlertWinByTextId(uint32 * time_period_ptr, 
                    MMI_TEXT_ID_T text1_id, 
                    MMI_TEXT_ID_T text2_id, 
                    MMI_IMAGE_ID_T image_id, 
                    MMI_WIN_PRIORITY_E * win_priority_ptr, 
                    MMIPUB_SOFTKEY_STYLE_E softkey_style,
                    MMIPUB_HANDLE_FUNC         handle_func
                    )
{
    MMI_WIN_ID_T    alert_win_id = MMIBROWSER_BOOKMARK_ALERT_WIN_ID;
    MMIPUB_OpenAlertWinByTextId(time_period_ptr, 
            text1_id, text2_id, 
            image_id, 
            &alert_win_id, 
            win_priority_ptr, 
            softkey_style,
            handle_func);
}
/****end:封装mmi pub query window的API的原因,是为了browser的重入时能及时关闭browser模块的所有窗口;
因此browser模块内使用query pub接口,必须调用以上几个;未封装MMIPUB_OpenQuery的所有api,需要可以再扩展****/

/*****************************************************************************/
//  Description : open the bookmark window
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_OpenBookmarkWin(void)
{
#ifdef BROWSER_BOOKMARK_HISTORY_TAB
    MMIBROWSER_OpenBkHistoryWindow(MMIBROWSER_TAB1_BOOKMARK, PNULL);
#else//default
#ifndef MMIDORADO_BOOKMARK_BY_NV
    MMI_WIN_ID_T waiting_win_id = MMIBROWSER_BOOKMARK_WAITING_WIN_ID;
    uint32       waiting_time_out = 0;

    //if (MMIAPIFMM_GetFirstValidDevice() < MMI_DEVICE_NUM)
    if (MMIBROWSER_GetIsBookmarkLoaded())//normal
    {
        MMIPUB_OpenAlertWinByTextId(&waiting_time_out, TXT_COMMON_WAITING, TXT_NULL,
            IMAGE_PUBWIN_WAIT, &waiting_win_id,PNULL,MMIPUB_SOFTKEY_NONE,HandleBKOpenMainWaitingWin);

        MMK_PostMsg(waiting_win_id, MSG_BRW_BK_OPEN_MAIN, PNULL, NULL);
        //MMK_CreateWin((uint32*)MMIBROWSER_BOOKMARK_WIN_TAB,PNULL);
    }
    else
    {
        BrwOpenAlertWinByTextId(PNULL,TXT_SD_NO_EXIST, PNULL, IMAGE_PUBWIN_WARNING, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
        //MMIBROWSER_SetBookmarkLoaded(FALSE);//set bookmark is not loaded
    }
#else
    MMK_CreateWin((uint32*)MMIBROWSER_BOOKMARK_WIN_TAB,PNULL);
#endif
#endif    
}

/*****************************************************************************/
//  Description : close the bookmark window
//  Global resource dependence :
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_CloseBookmarkWin(void)
{
#ifdef BROWSER_BOOKMARK_HISTORY_TAB
    MMK_CloseWin(MMK_GetParentWinHandle(MMIBROWSER_BOOKMARK_WIN_ID));
#else
    MMK_CloseWin(MMIBROWSER_BOOKMARK_WIN_ID);
#endif     
}

#ifdef BROWSER_BOOKMARK_HISTORY_TAB
/*****************************************************************************/
//  Description : open the bookmark child window
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_CreateChildBookmarkWin(MMI_WIN_ID_T parent_win_id, ADD_DATA add_data_ptr)
{
	MMK_CreateChildWin(parent_win_id, (uint32*)MMIBROWSER_BOOKMARK_WIN_TAB, add_data_ptr);
}
#endif

/*****************************************************************************/
//  Description : get bookmark's file array index from bookmark list index
//  Global resource dependence : none
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL uint16 BrwGetBookmarkArrayIndex(uint16 cur_list_index)
{
#ifndef MMIDORADO_BOOKMARK_BY_NV
    uint16 array_index = 0;
#if 0
    MMIBRW_BOOKMARK_T *bookmark_info_ptr = MMIBRW_GetBookmark();

    if ((1 == bookmark_info_ptr->file_array_step) && 0 != cur_list_index)//二级目录有多余top 项"..."
    {
        array_index = cur_list_index - 1;
    }
    else
#endif
    if (s_is_in_bookmark_moving)//move时有多余top 项"/"
    {
        array_index = cur_list_index - 1;
    }
    else
    {
        array_index = cur_list_index;
    }
    return array_index;
#else
    return cur_list_index;
#endif
}

/*****************************************************************************/
//  Description : add history title item to listbox for www mdu 
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC void MMIBRW_AppendBookmarkItemData( 
                                                        MMI_CTRL_ID_T ctrl_id, 
                                                        uint16 index,  
                                                        MMI_STRING_T string_1, 
                                                        MMI_STRING_T string_2, 
                                                        MMI_IMAGE_ID_T image_id, 
                                                        MMI_TEXT_ID_T text1_id, 
                                                        MMI_TEXT_ID_T text2_id, 
                                                        MMI_TEXT_ID_T text3_id,
                                                        BOOLEAN  is_folder
                                                        )
{
    GUILIST_ITEM_T item_t = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    MMI_STRING_T        dst_temp = {0};

    item_t.item_style                        = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
    item_t.item_data_ptr                     = &item_data;
    item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;

    item_data.softkey_id[0] = text1_id;
    item_data.softkey_id[1] = text2_id;
    item_data.softkey_id[2] = text3_id;
    item_data.item_content[0].item_data.image_id = image_id;

#ifdef MMI_PDA_SUPPORT
#ifndef MMIDORADO_BOOKMARK_BY_NV
    if(is_folder)
    {
        item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[1].item_data.text_buffer.wstr_len = string_1.wstr_len;
        item_data.item_content[1].item_data.text_buffer.wstr_ptr = string_1.wstr_ptr;//SCI_ALLOCA((str_len + 1) * sizeof(wchar));
    }
    else
#endif        
    {
        item_t.item_style = GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_ICON;

        item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[3].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        if(string_1.wstr_len!=PNULL && string_1.wstr_len!=0)
        {
            item_data.item_content[1].item_data.text_buffer.wstr_len = string_1.wstr_len;
            item_data.item_content[1].item_data.text_buffer.wstr_ptr = string_1.wstr_ptr;
        }
        else
        {
            MMI_GetLabelTextByLang(TXT_NO_TITLE, &dst_temp);
            item_data.item_content[1].item_data.text_buffer.wstr_len = dst_temp.wstr_len;
            item_data.item_content[1].item_data.text_buffer.wstr_ptr = dst_temp.wstr_ptr;
        }   
 
        item_data.item_content[3].item_data.text_buffer.wstr_len = string_2.wstr_len;
        item_data.item_content[3].item_data.text_buffer.wstr_ptr = string_2.wstr_ptr;
    }

    GUILIST_SetItemData( ctrl_id, &item_data, index );

#else
#ifndef MMIDORADO_BOOKMARK_BY_NV
    if(is_folder)
    {
        item_data.item_content[1].item_data.text_buffer.wstr_len = string_1.wstr_len;
        item_data.item_content[1].item_data.text_buffer.wstr_ptr = string_1.wstr_ptr;//SCI_ALLOCA((str_len + 1) * sizeof(wchar));
    }
    else
#endif        
    {
        if(string_1.wstr_len!=PNULL && string_1.wstr_len!=0)
        {
            item_data.item_content[1].item_data.text_buffer.wstr_len = string_1.wstr_len;
            item_data.item_content[1].item_data.text_buffer.wstr_ptr = string_1.wstr_ptr;
        }
        else
        {
            MMI_GetLabelTextByLang(TXT_NO_TITLE, &dst_temp);
            item_data.item_content[1].item_data.text_buffer.wstr_len = dst_temp.wstr_len;
            item_data.item_content[1].item_data.text_buffer.wstr_ptr = dst_temp.wstr_ptr;
        }   
        
        item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_BUFFER;//url string
    
        item_data.item_content[2].item_data.text_buffer.wstr_len = string_2.wstr_len;
        item_data.item_content[2].item_data.text_buffer.wstr_ptr = string_2.wstr_ptr;

    }

    GUILIST_SetItemData( ctrl_id, &item_data, index );
#endif /* MMI_PDA_SUPPORT */
} 

/*****************************************************************************/
//  Description : add dynamic item to listbox when item_num is not zero
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC void MMIBRW_AppendListboxitem( 
                                                        MMI_WIN_ID_T win_id, 
                                                        MMI_CTRL_ID_T ctrl_id, 
                                                        uint16 item_num
                                                        )
{
    uint16 i=0;
    GUILIST_ITEM_T item_t = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    MMI_HANDLE_T ctrl_handle = MMK_GetCtrlHandleByWin(win_id, ctrl_id);
#ifndef MMIDORADO_BOOKMARK_BY_NV
    MMIBRW_BOOKMARK_T *bookmark_info_ptr = MMIBRW_GetBookmark();
#endif

    //remove all first
    if (GUILIST_GetTotalItemNum(ctrl_handle) > 0)
    {
        GUILIST_RemoveAllItems(ctrl_handle);
    }
    
    if( item_num > 0)
    {
        uint16 cur_index = 0;
        uint16 top_index = 0;

        GUILIST_SetTitleIndexType(ctrl_id, GUILIST_TITLE_INDEX_DEFAULT);

        cur_index = GUILIST_GetCurItemIndex(ctrl_handle);
        top_index = GUILIST_GetTopItemIndex(ctrl_handle);
        GUILIST_SetMaxItem( ctrl_handle, item_num, TRUE );
               
        item_t.item_data_ptr = &item_data;

        for( i = 0; i < item_num; i++ )
        {
#ifndef MMIDORADO_BOOKMARK_BY_NV
            FILEARRAY_DATA_T array_item = {0};
            uint16 array_index = 0;

            array_index = BrwGetBookmarkArrayIndex(i);
            if(MMIAPIFILEARRAY_Read(bookmark_info_ptr->file_array, (uint32)array_index, &array_item)
               /* && !((1 == bookmark_info_ptr->file_array_step) && (0 == i))*/)    //二级目录的top 项"..."设置为单行
            {
                if(FILEARRAY_TYPE_FILE== array_item.type)
                {
#ifdef MMI_PDA_SUPPORT
                    item_t.item_style = GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_ICON;

#else
                    item_t.item_style = GUIITEM_STYLE_ONE_TWO_LINE_ICON_TEXT_AND_TEXT;
#endif /* MMI_PDA_SUPPORT */
                }
                else
                {
                    item_t.item_style = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
                }
            }
            else
            {
                item_t.item_style = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
            }
            GUILIST_AppendItem( ctrl_handle, &item_t );
#else/*MMIDORADO_BOOKMARK_BY_NV*/

#ifdef MMI_PDA_SUPPORT
            item_t.item_style = GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_ICON;
#else
            item_t.item_style = GUIITEM_STYLE_ONE_TWO_LINE_ICON_TEXT_AND_TEXT;
#endif /* MMI_PDA_SUPPORT */

            GUILIST_AppendItem( ctrl_handle, &item_t );

#endif/*MMIDORADO_BOOKMARK_BY_NV*/
        }
        GUILIST_SetTopItemIndex(ctrl_handle, top_index);
        GUILIST_SetCurItemIndex(ctrl_handle, cur_index);
    }
    else
    {
#ifdef BROWSER_START_PAGE_THEME1
        if (!MMK_IsOpenWin(MMIBROWSER_MAIN_WIN_ID) 
#ifndef MMIDORADO_BOOKMARK_BY_NV
            && (bookmark_info_ptr != PNULL) && (0 == bookmark_info_ptr->file_array_step)
#endif            
            )
        {
            GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_EXIT, FALSE);
        }
        else
#endif            
        {
            GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
        }
    }
}
#ifndef MMIDORADO_BOOKMARK_BY_NV
/*****************************************************************************/
//  Description : handle bookmark window message
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleBookmarkWinMsg(
                                           MMI_WIN_ID_T	win_id, 
                                           MMI_MESSAGE_ID_E	msg_id, 
                                           DPARAM param
                                           )
{
    MMI_RESULT_E            result=MMI_RESULT_TRUE;
    MMI_CTRL_ID_T           list_ctrl_id = MMIBROWSER_LIST_BOOKMARK_CTRL_ID;
    uint16                  mark_num = 0;
    MMIBRW_BOOKMARK_T *bookmark_info_ptr = MMIBRW_GetBookmark();
#ifdef MMI_PDA_SUPPORT
    GUIFORM_CHILD_HEIGHT_T child_height = {0};
#endif /* MMI_PDA_SUPPORT */
    uint16 cur_index = 0;
    
    //SCI_TRACE_LOW:"BrwHandleBookmarkWinMsg msg_id= 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_BOOKMARK_931_112_2_18_2_6_27_258,(uint8*)"d", msg_id);
    
    switch(msg_id) 
    {
    case MSG_OPEN_WINDOW:        
#ifdef MMI_PDA_SUPPORT
        child_height.type = GUIFORM_CHILD_HEIGHT_LEFT;
        GUILIST_PermitBorder(list_ctrl_id, FALSE);
        GUIFORM_IsSlide(MMIBROWSER_BOOKMARK_FORM_CTRL_ID, FALSE);
        GUIFORM_SetChildDisplay(MMIBROWSER_BOOKMARK_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
        GUIFORM_SetChildHeight(MMIBROWSER_BOOKMARK_FORM_CTRL_ID, list_ctrl_id, &child_height);
        GUIFORM_PermitChildBg(MMIBROWSER_BOOKMARK_FORM_CTRL_ID, FALSE);
        GUILIST_SetUserBg(list_ctrl_id, FALSE);
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);

#ifdef BROWSER_START_PAGE_THEME1
        if (!MMK_IsOpenWin(MMIBROWSER_MAIN_WIN_ID))
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_LEFT, TITLE_BUTTON_NORMAL, FALSE);
            GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_LEFT, STXT_EXIT, FALSE);
        }
#endif        
#endif /* MMI_PDA_SUPPORT */
        GUILIST_SetMaxItem(list_ctrl_id,(MMIBRW_BOOKMARK_MAX_ITEM + MMIBRW_BOOKMARK_DIR_MAX_NUM),FALSE);
        GUILIST_RemoveAllItems(list_ctrl_id);
        s_pre_path_index = 0;//init
        s_is_in_bookmark_moving = FALSE;
        //MMIBRW_LoadBookmarkFileArray(BOOKMARK_STEP_TYPE_ROOT, 0, FUNC_FIND_NULL);
        
#ifndef BROWSER_BOOKMARK_HISTORY_TAB		
        MMIBRW_AppendListboxitem(win_id, 
            list_ctrl_id,
            (uint16)MMIBRW_GetBookmarkNumber()
            );
#else
        s_is_append_when_paint = TRUE;
#endif			
        
        MMK_SetAtvCtrl(win_id,list_ctrl_id);    
        break;
#ifdef BROWSER_START_PAGE_THEME1
    case MSG_SLIDE_MOVE_BEGIN:
        if (GUILIST_GetSlideState(list_ctrl_id))
        {
            GUILIST_SetSlideState(list_ctrl_id, FALSE);
            s_is_need_restore_list_slide = TRUE;
        }
        break;
    case MSG_SLIDE_MOVE_END:
        if (s_is_need_restore_list_slide)
        {
            GUILIST_SetSlideState(list_ctrl_id, TRUE);
            s_is_need_restore_list_slide = FALSE;
        }

        if (!MMK_IsFocusWin(win_id))
        {
            MMIBROWSER_BkExitMarked();
        }
        break;
#endif
        
    case MSG_FULL_PAINT:
        if (s_is_append_when_paint)
        {
            if (MMIBROWSER_GetIsBookmarkLoaded())//NEWMS00203394
            {
                MMIBRW_LoadBookmarkFileArray(BOOKMARK_STEP_TYPE_ROOT, 0, FUNC_FIND_NULL, 0, 0);
            }
            MMIBRW_AppendListboxitem(win_id, 
                list_ctrl_id,
                (uint16)MMIBRW_GetBookmarkNumber()
                );
            s_is_append_when_paint = FALSE;
        }
        break;
        
    case MSG_GET_FOCUS:
        {
            if(0 == MMIBRW_GetBookmarkNumber())//for none bookmakr after delete
            {
                MMIBRW_AppendListboxitem(win_id, list_ctrl_id, 0);
            }
            
#ifdef MMI_PDA_SUPPORT
            if (s_is_in_bookmark_moving || GUILIST_GetListStateById(list_ctrl_id,GUILIST_STATE_ENABLE_MARK))
            {
                GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
            }
            else
            {
                GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
            }

#ifdef BROWSER_START_PAGE_THEME1
            if (!MMK_IsOpenWin(MMIBROWSER_MAIN_WIN_ID) 
                && (0 == bookmark_info_ptr->file_array_step)
                && !s_is_in_bookmark_moving
                )
            {
                GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_LEFT, STXT_EXIT, FALSE);
            }
            else
            {
                GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_LEFT, STXT_RETURN, FALSE);
            }
#endif            
#endif
            
        }
        break;
        
    case MSG_CTL_LIST_NEED_ITEM_DATA:
    {
        MMI_STRING_T title_string_info = {0};
        MMI_STRING_T url_string_info = {0};
        GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;
        uint16 array_index = 0;
        FILEARRAY_TYPE_E type = FILEARRAY_TYPE_NULL;
        MMI_IMAGE_ID_T image_id = 0;
        MMI_TEXT_ID_T text1_id = STXT_OPTION;
        MMI_TEXT_ID_T text2_id = TXT_NULL;
        MMI_TEXT_ID_T text3_id = STXT_RETURN;

        if (PNULL == need_item_data_ptr)
        {
            break;
        }
        
#ifdef BROWSER_START_PAGE_THEME1
        if (!MMK_IsOpenWin(MMIBROWSER_MAIN_WIN_ID) 
            && (0 == bookmark_info_ptr->file_array_step)
            && !s_is_in_bookmark_moving
            )
        {
            text3_id = STXT_EXIT;
        }
#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_LEFT, text3_id, FALSE);
#endif
#endif

        if (0 == bookmark_info_ptr->cur_index_num)//当前目录的top为空项
        {
            MMI_GetLabelTextByLang(TXT_WWW_BOOKMARK_EMPTY, &title_string_info);
            image_id = IMAGE_NULL;
            text1_id = STXT_OPTION;
            text2_id = TXT_NULL;
            //text3_id = STXT_RETURN;
            
            MMIBRW_AppendBookmarkItemData(list_ctrl_id, 
                need_item_data_ptr->item_index, title_string_info, url_string_info, image_id,
                text1_id, text2_id, text3_id,TRUE );
        }
        else if (s_is_in_bookmark_moving && (0 == need_item_data_ptr->item_index))//增加moving  时root path 的显示
        {                
            MMI_GetLabelTextByLang(TXT_BROWSER_ROOT_FOLDER_ROUTE, &title_string_info);      
            image_id = IMAGE_FMM_FOLDER_ICON;
            text1_id = STXT_SELECT;
            text2_id = TXT_NULL;
            text3_id = STXT_CANCEL;
            
            MMIBRW_AppendBookmarkItemData(list_ctrl_id, 
                need_item_data_ptr->item_index, title_string_info, url_string_info,image_id,
                text1_id, text2_id, text3_id,TRUE );
        }
        else
        {
            array_index = BrwGetBookmarkArrayIndex(need_item_data_ptr->item_index);
        
            type =MMIBRW_GetBookmarkTitle(array_index, &title_string_info);
            MMIBRW_GetBookmarkUrl(array_index, &url_string_info);
            /*if(1 == bookmark_info_ptr->file_array_step)
            {
                text3_id = STXT_RETURN;
            }*/
            if (PNULL != title_string_info.wstr_ptr)
            {
                if (FILEARRAY_TYPE_FOLDER == type)
                {        
                    image_id = IMAGE_FMM_FOLDER_ICON;
                    text2_id = TXT_COMM_OPEN;
                    if (s_is_in_bookmark_moving)
                    {
                        text1_id = STXT_SELECT;
                        text2_id = TXT_NULL;
                        text3_id = STXT_CANCEL;
                    }
                    
                    MMIBRW_AppendBookmarkItemData(list_ctrl_id, 
                        need_item_data_ptr->item_index, title_string_info, url_string_info,image_id,
                        text1_id, text2_id, text3_id,TRUE );
                    
                }
                else  if (FILEARRAY_TYPE_FILE == type)
                {
                    
#ifdef MMI_PDA_SUPPORT
                    image_id = IMAGE_BROWSER_PAGE;
#else
#ifndef MMI_BROWSER_MINI_SUPPORT
                    image_id = IMAGE_BROWSER_BOOKMARK_LIST_ICON;
#else
                    image_id = IMAGE_COMMON_TXT_ICON;
#endif
#endif /* MMI_PDA_SUPPORT */
                    text2_id = TXT_COMM_OPEN;
                    
                    if (s_is_in_bookmark_moving)
                    {
                        text1_id = STXT_SELECT;
                        text2_id = TXT_NULL;
                        text3_id = STXT_CANCEL;
                    }
                    
                    MMIBRW_AppendBookmarkItemData(list_ctrl_id, 
                        need_item_data_ptr->item_index, title_string_info, url_string_info,image_id,
                        text1_id, text2_id, text3_id,FALSE );
                }
                
                SCI_FREE(title_string_info.wstr_ptr);
                SCI_FREE(url_string_info.wstr_ptr);
                
            }
            else
            {
                //SCI_TRACE_LOW:"BrwHandleBookmarkWinMsg() String is NULL"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_BOOKMARK_1106_112_2_18_2_6_27_259,(uint8*)"");
            }
        }
    }
        break;           
        
#ifdef MMI_PDA_SUPPORT
    case MSG_NOTIFY_TITLE_MENU_ICON:
    case MSG_APP_MENU:
        if (!s_is_in_bookmark_moving)
        {
            GUIFORM_CHILD_DISPLAY_E     display_type = GUIFORM_CHILD_DISP_NORMAL;
            
            GUIFORM_GetChildDisplay(MMIBROWSER_BOOKMARK_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, &display_type);
            if (GUIFORM_CHILD_DISP_NORMAL == display_type)
            {
                //do nothing
            }
            else//enter bookmark opt window
            {
                uint16 array_index = 0;
                FILEARRAY_DATA_T f_array_data = {0};
                
                SCI_MEMSET(&s_bookmark_cur_index_info, 0x00, sizeof(s_bookmark_cur_index_info));
                
                array_index = BrwGetBookmarkArrayIndex(GUILIST_GetCurItemIndex(list_ctrl_id));
                BrwSetBookmarkCurIndex(array_index);
                
                if(MMIAPIFILEARRAY_Read(bookmark_info_ptr->file_array, array_index, &f_array_data))
                {
//                    s_bookmark_cur_index_info.type = f_array_data.type;
                }
                else//error
                {
//                    s_bookmark_cur_index_info.type = FILEARRAY_TYPE_NULL;
                }
                
                MMK_CreateWin((uint32*)MMIBROWSER_BOOKMARK_POPMENU_WIN_TAB,(ADD_DATA)(f_array_data.type));    
            }
        }
        else
        {
            //do nothing
        }
        break;
#endif /* MMI_PDA_SUPPORT */
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
        if (s_is_in_bookmark_moving)
        {
            MMIBROWSER_BK_MOVE_RESULT_E move_result = MMIBROWSER_BK_MOVE_RESULT_NONE;
            
            cur_index = GUILIST_GetCurItemIndex(list_ctrl_id);
            move_result = BrwBookmarkBeginMoveTo(cur_index);
            switch (move_result)
            {
            case MMIBROWSER_BK_MOVE_RESULT_SAME_PATH:
                s_is_in_bookmark_moving = FALSE;
                BrwPrepare2BackToRoot();//reset: go back to root path
                BrwOpenAlertWinByTextId(PNULL,TXT_WWW_BOOKMARK_SAME_PATH,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                break;
                
            case MMIBROWSER_BK_MOVE_RESULT_ERROR:
                s_is_in_bookmark_moving = FALSE;
                BrwPrepare2BackToRoot();//reset: go back to root path
                BrwOpenAlertWinByTextId(PNULL,TXT_ERROR,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);               
                break;
                
            default:
                break;
            }
        }
        else//enter bookmark opt window
        {
            uint16 array_index = 0;
            FILEARRAY_DATA_T f_array_data = {0};
            
            SCI_MEMSET(&s_bookmark_cur_index_info, 0x00, sizeof(s_bookmark_cur_index_info));
            
            array_index = BrwGetBookmarkArrayIndex(GUILIST_GetCurItemIndex(list_ctrl_id));
            
            BrwSetBookmarkCurIndex(array_index);
            
            if(MMIAPIFILEARRAY_Read(bookmark_info_ptr->file_array, array_index, &f_array_data))
            {
                //s_bookmark_cur_index_info.type = f_array_data.type;
            }
            else//error
            {
                //s_bookmark_cur_index_info.type = FILEARRAY_TYPE_NULL;
            }
            
            MMK_CreateWin((uint32*)MMIBROWSER_BOOKMARK_POPMENU_WIN_TAB,(ADD_DATA)(f_array_data.type));    
        }
        break;
        
#ifdef MMI_PDA_SUPPORT
    case MSG_CTL_LIST_LONGOK:
        if (!s_is_in_bookmark_moving)
        {
            GUIFORM_CHILD_DISPLAY_E     display_type = GUIFORM_CHILD_DISP_NORMAL;
            
            GUIFORM_GetChildDisplay(MMIBROWSER_BOOKMARK_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, &display_type);
            if (GUIFORM_CHILD_DISP_NORMAL == display_type)
            {
                //do nothing
            }
            else//enter one bookmark/folder's opt window
            {
                if (GUILIST_GetTotalItemNum(list_ctrl_id) > 0)
                {
                    uint16 array_index = 0;
                    
                    SCI_MEMSET(&s_bookmark_cur_index_info, 0x00, sizeof(s_bookmark_cur_index_info));
                    array_index = BrwGetBookmarkArrayIndex(GUILIST_GetCurItemIndex(list_ctrl_id));
                    BrwSetBookmarkCurIndex(array_index);
                    
                    MMK_CreateWin((uint32*)MMIBROWSER_BOOKMARK_POPUP_WIN_TAB, PNULL);
                }
            }
        }       
        break;
#endif /* MMI_PDA_SUPPORT */
        
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef MMI_PDA_SUPPORT
        {
            MMI_CTRL_ID_T       ctrl_id = 0;
            
            if (PNULL != param)
            {
                ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;
                mark_num = GUILIST_GetSelectedItemIndex(list_ctrl_id, PNULL, 0);

                if (ctrl_id == MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID)
                {
                    if (GUILIST_GetTotalItemNum(list_ctrl_id) != mark_num)
                    {
                        GUISOFTKEY_SetTextId(win_id, MMICOMMON_SOFTKEY_CTRL_ID, TXT_CANCEL_SELECT_ALL, TXT_DELETE, STXT_CANCEL, FALSE );
                        BrwHandleBookmarkListMarkedAll(list_ctrl_id);
                    }
                    else
                    {
                        GUISOFTKEY_SetTextId(win_id, MMICOMMON_SOFTKEY_CTRL_ID, TXT_MARK_ALL, TXT_DELETE, STXT_CANCEL, FALSE );
                        GUILIST_SetAllSelected(list_ctrl_id,FALSE);
                    }
                    
#ifdef BROWSER_BOOKMARK_HISTORY_TAB
                    MMK_PostMsg(MMK_GetParentWinHandle(win_id), MSG_FULL_PAINT, PNULL, 0);
#else
                    MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
#endif     
                    break;
                    
                }
                else if (ctrl_id == MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID)
                {
                    BrwExitBookmarkListMarked(list_ctrl_id);
#ifdef BROWSER_BOOKMARK_HISTORY_TAB
                    MMK_PostMsg(MMK_GetParentWinHandle(win_id), MSG_FULL_PAINT, PNULL, 0);
#else
                    MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
#endif     
                    break;
                }
                else if (ctrl_id == MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID)
                {
                    if(0 == mark_num)
                    {
                        BrwOpenAlertWinByTextId(PNULL, TXT_SMS_SELECT_ONT_PROMPT, TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
                    }
                    else
                    {
                        BrwQueryIsDeleteBookmark(list_ctrl_id);
                    }
                    break;
                }
            }
        }
#endif /* MMI_PDA_SUPPORT */
        
        if (s_is_in_bookmark_moving)//同MSG_APP_OK开始move操作
        {
            MMIBROWSER_BK_MOVE_RESULT_E move_result = MMIBROWSER_BK_MOVE_RESULT_NONE;
            
            cur_index = GUILIST_GetCurItemIndex(list_ctrl_id);
            move_result = BrwBookmarkBeginMoveTo(cur_index);
            switch (move_result)
            {
            case MMIBROWSER_BK_MOVE_RESULT_SAME_PATH:
                s_is_in_bookmark_moving = FALSE;
                BrwPrepare2BackToRoot();//reset: go back to root path
                BrwOpenAlertWinByTextId(PNULL,TXT_WWW_BOOKMARK_SAME_PATH,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                break;
                
            case MMIBROWSER_BK_MOVE_RESULT_ERROR:
                s_is_in_bookmark_moving = FALSE;
                BrwPrepare2BackToRoot();//reset: go back to root path
                BrwOpenAlertWinByTextId(PNULL,TXT_ERROR,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);               
                break;
                
            default:
                break;
            }
        }
        else
        {
            uint16 array_index = 0;
            FILEARRAY_DATA_T f_array_data = {0};

            cur_index = GUILIST_GetCurItemIndex(list_ctrl_id);
            array_index = BrwGetBookmarkArrayIndex(cur_index);
            MMIAPIFILEARRAY_Read(bookmark_info_ptr->file_array, array_index, &f_array_data);
            
            if (FILEARRAY_TYPE_FOLDER == f_array_data.type)//打开文件夹
            {
                s_pre_path_index = cur_index;
                BrwOpenBookmarkStepWin(BOOKMARK_STEP_TYPE_NEXT, array_index, FUNC_FIND_NULL);
            }
            else  if (FILEARRAY_TYPE_FILE == f_array_data.type)//选中当前的书签进行链接
            {
                if (MMIBRW_BookmarkGotoURL(array_index))
                {
                    BookmarkTidyExistedBkWin();
                }
            }
        }
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        {
            uint16 array_index = 0;
            //MMIFMM_FUNC_TYPE_E type = FUNC_FIND_NULL;//默认显示
            
            if (1 == bookmark_info_ptr->file_array_step) /*返回到上级目录(当前为根目录)*/
            {         
#ifdef MMI_PDA_SUPPORT
                {//exit the mark status first
                    GUIFORM_CHILD_DISPLAY_E     display_type = GUIFORM_CHILD_DISP_NORMAL;
                    
                    GUIFORM_GetChildDisplay(MMIBROWSER_BOOKMARK_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, &display_type);
                    if (GUIFORM_CHILD_DISP_NORMAL == display_type)
                    {
                        BrwExitBookmarkListMarked(list_ctrl_id);
                    }
                }
#endif /* MMI_PDA_SUPPORT */

                array_index = BrwGetBookmarkArrayIndex(GUILIST_GetCurItemIndex(list_ctrl_id));
                BrwOpenBookmarkStepWin(BOOKMARK_STEP_TYPE_PRE, array_index, FUNC_FIND_NULL);
            }
            else
            {
                if(s_is_in_bookmark_moving)//cancel the moving operation and back to root
                {
                    s_is_in_bookmark_moving = FALSE;
                    BrwOpenBookmarkStepWin(BOOKMARK_STEP_TYPE_ROOT, 0, FUNC_FIND_NULL);
                    break;
                }
#ifndef MMI_BROWSER_MINI_SUPPORT                
                MMIBROWSER_BkExitMarked();
#endif
                
#ifdef BROWSER_START_PAGE_THEME1
                if (MMK_IsOpenWin(MMIBROWSER_START_PAGE_WIN_ID))
                {
                    if (MMK_IsOpenWin(MMIBROWSER_MAIN_WIN_ID))
                    {
                        MMK_WinGrabFocus(MMIBROWSER_MAIN_WIN_ID);
                    }
                    else
                    {
                        MMIBROWSER_QueryExit();
                    }
                }
                else
#endif
                {
                    MMIBROWSER_CloseBookmarkWin();
                }
            }
        }
        break;
        
    case MSG_CTL_LIST_MARK_ITEM:
        //get mark num
        mark_num = GUILIST_GetSelectedItemIndex(list_ctrl_id,PNULL,0);
#ifdef MMI_PDA_SUPPORT
        if ((0 == mark_num) || (GUILIST_GetTotalItemNum(list_ctrl_id) > mark_num))
        {
            GUISOFTKEY_SetTextId(win_id, MMICOMMON_SOFTKEY_CTRL_ID, TXT_MARK_ALL, TXT_DELETE, STXT_CANCEL, TRUE);
        }
        else
        {
            GUISOFTKEY_SetTextId(win_id, MMICOMMON_SOFTKEY_CTRL_ID, TXT_CANCEL_SELECT_ALL, TXT_DELETE, STXT_CANCEL, TRUE);
        }
#else
        if (0 == mark_num)
        {
            //set list enable mark
            GUILIST_SetTextListMarkable(list_ctrl_id,FALSE);
        }
#endif /* MMI_PDA_SUPPORT */
        break;
        
    case MSG_BRW_DELETE_BOOKMARK:
        {
            uint16 select_num = 0;
            uint16 deleted_num = 0;
            MMI_WIN_ID_T     wait_win_id =  MMIBROWSER_BOOKMARK_MOVE_WAITING_WIN_ID;//use move wait win temp
            
            HandleDeleteBookmarkCnf(*((BOOLEAN*)param),list_ctrl_id,wait_win_id,win_id);
            select_num = GUILIST_GetSelectedItemIndex(list_ctrl_id,PNULL,0);
            
            deleted_num = ((s_bookmark_del_total_num > select_num) ? (s_bookmark_del_total_num - select_num) : 0);
            MMIPUB_UpdateProgressBar(&wait_win_id, deleted_num);
        }
        break;        
        
    case MSG_CLOSE_WINDOW:
        {
            if( PNULL != bookmark_info_ptr->file_array)
            {
                MMIAPIFILEARRAY_Destroy(&(bookmark_info_ptr->file_array));
            }
            bookmark_info_ptr->cur_index_num = 0;
            bookmark_info_ptr->file_array_step = 0;
            SCI_MEMSET(bookmark_info_ptr->full_path, 0x00, sizeof(bookmark_info_ptr->full_path));
            BrwGetDefaultRootPath(bookmark_info_ptr->full_path, &(bookmark_info_ptr->full_path_len));
        }
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}
#else
/*****************************************************************************/
//  Description : handle bookmark window message
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleBookmarkWinMsg(
                                   MMI_WIN_ID_T	win_id, 
                                   MMI_MESSAGE_ID_E	msg_id, 
                                   DPARAM param
                                   )
{
    MMI_RESULT_E            result=MMI_RESULT_TRUE;
    MMI_CTRL_ID_T           list_ctrl_id = MMIBROWSER_LIST_BOOKMARK_CTRL_ID;
    uint32                  list_num = 0;
    uint16                  mark_num = 0;
#ifdef MMI_PDA_SUPPORT
    GUIFORM_CHILD_HEIGHT_T child_height = {0};
#endif /* MMI_PDA_SUPPORT */

    switch(msg_id) 
    {
     case MSG_OPEN_WINDOW:        
#ifdef MMI_PDA_SUPPORT
        child_height.type = GUIFORM_CHILD_HEIGHT_LEFT;
        GUILIST_PermitBorder(list_ctrl_id, FALSE);
        GUIFORM_IsSlide(MMIBROWSER_BOOKMARK_FORM_CTRL_ID, FALSE);
        GUIFORM_SetChildDisplay(MMIBROWSER_BOOKMARK_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
        GUIFORM_SetChildHeight(MMIBROWSER_BOOKMARK_FORM_CTRL_ID, list_ctrl_id, &child_height);
        GUIFORM_PermitChildBg(MMIBROWSER_BOOKMARK_FORM_CTRL_ID, FALSE);
        GUILIST_SetUserBg(list_ctrl_id, FALSE);
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#ifdef BROWSER_START_PAGE_THEME1
        if (!MMK_IsOpenWin(MMIBROWSER_MAIN_WIN_ID))
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_LEFT, TITLE_BUTTON_NORMAL, FALSE);
            GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_LEFT, STXT_EXIT, FALSE);
        }
#endif        
#endif /* MMI_PDA_SUPPORT */

#ifndef BROWSER_BOOKMARK_HISTORY_TAB		
        MMIBRW_AppendListboxitem(win_id, 
            list_ctrl_id,
            MMIBRW_BookmarkGetCount()
            );
#else
        s_is_append_when_paint = TRUE;
#endif			
        
        MMK_SetAtvCtrl(win_id,list_ctrl_id);    
        break;

#ifdef BROWSER_START_PAGE_THEME1
    case MSG_SLIDE_MOVE_BEGIN:
        if (GUILIST_GetSlideState(list_ctrl_id))
        {
            GUILIST_SetSlideState(list_ctrl_id, FALSE);
            s_is_need_restore_list_slide = TRUE;
        }
        break;
    case MSG_SLIDE_MOVE_END:
        if (s_is_need_restore_list_slide)
        {
            GUILIST_SetSlideState(list_ctrl_id, TRUE);
            s_is_need_restore_list_slide = FALSE;
        }
        break;
#endif
        
    case MSG_FULL_PAINT:
        if (s_is_append_when_paint)
        {
            MMIBRW_AppendListboxitem(win_id, 
                list_ctrl_id,
                MMIBRW_BookmarkGetCount()
                );
           s_is_append_when_paint = FALSE;
        }
        break;
         
    case MSG_CTL_LIST_NEED_ITEM_DATA:
    {    
        MMI_STRING_T title_string_info = {0};
        MMI_STRING_T url_string_info = {0};
        GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;
        uint16 cur_index = 0;
        MMI_IMAGE_ID_T  image_id = 0;
        MMI_TEXT_ID_T   text3_id = STXT_RETURN;

#ifdef MMI_PDA_SUPPORT
        image_id = IMAGE_BROWSER_PAGE;
#else
#ifndef MMI_BROWSER_MINI_SUPPORT
        image_id = IMAGE_BROWSER_BOOKMARK_LIST_ICON;
#else
        image_id = IMAGE_COMMON_TXT_ICON;
#endif
#endif /* MMI_PDA_SUPPORT */

        if (PNULL == need_item_data_ptr)
        {
            break;
        }
        
#ifdef BROWSER_START_PAGE_THEME1
        if (!MMK_IsOpenWin(MMIBROWSER_MAIN_WIN_ID))
        {
            text3_id = STXT_EXIT;
        }
#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_LEFT, text3_id, FALSE);
#endif
#endif

        cur_index = need_item_data_ptr->item_index;
        
        MMIBRW_GetBookmarkTitle(cur_index, &title_string_info);
        MMIBRW_GetBookmarkUrl(cur_index, &url_string_info);         
            
        MMIBRW_AppendBookmarkItemData(list_ctrl_id, 
                need_item_data_ptr->item_index, title_string_info, url_string_info,image_id,
                STXT_OPTION, TXT_COMM_OPEN, text3_id,FALSE );

        if (url_string_info.wstr_ptr != PNULL)
        {
            SCI_FREE(url_string_info.wstr_ptr);
        }
    }
         break;
  
    case MSG_GET_FOCUS:
        list_num = MMIBRW_BookmarkGetCount();
        if (0 == list_num)
        {
            MMIBRW_AppendListboxitem(win_id, list_ctrl_id, 0);
            GUILIST_SetTextListMarkable(list_ctrl_id,FALSE);
        }
        
 #ifdef MMI_PDA_SUPPORT
        if (GUILIST_GetListStateById(list_ctrl_id,GUILIST_STATE_ENABLE_MARK))
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
        }
        else
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
        }
#ifdef BROWSER_START_PAGE_THEME1
		if (!MMK_IsOpenWin(MMIBROWSER_MAIN_WIN_ID))
		{
			GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_LEFT, STXT_EXIT, FALSE);
		}
		else
		{
			GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_LEFT, STXT_RETURN, FALSE);
		}
#endif            
#endif
       break;

#ifdef MMI_PDA_SUPPORT
    case MSG_NOTIFY_TITLE_MENU_ICON:
    case MSG_APP_MENU:
    {
        GUIFORM_CHILD_DISPLAY_E     display_type = GUIFORM_CHILD_DISP_NORMAL;
        
        GUIFORM_GetChildDisplay(MMIBROWSER_BOOKMARK_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, &display_type);
        if (GUIFORM_CHILD_DISP_NORMAL == display_type)
        {
            //do nothing
        }
        else//enter bookmark opt window
        {
            MMK_CreateWin((uint32*)MMIBROWSER_BOOKMARK_POPMENU_WIN_TAB, PNULL);    
        }
    }
        break;
#endif /* MMI_PDA_SUPPORT */

    case MSG_CTL_OK:
    case MSG_APP_OK:
        MMK_CreateWin((uint32*)MMIBROWSER_BOOKMARK_POPMENU_WIN_TAB,PNULL);    
        break;
        
#ifdef MMI_PDA_SUPPORT
    case MSG_CTL_LIST_LONGOK:
    {
        GUIFORM_CHILD_DISPLAY_E     display_type = GUIFORM_CHILD_DISP_NORMAL;
        
        GUIFORM_GetChildDisplay(MMIBROWSER_BOOKMARK_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, &display_type);
        if (GUIFORM_CHILD_DISP_NORMAL == display_type)
        {
            //do nothing
        }
        else if (MMIBRW_BookmarkGetCount() > 0)
        {
            MMK_CreateWin((uint32*)MMIBROWSER_BOOKMARK_POPUP_WIN_TAB, PNULL);
        }
    }       
        break;
#endif /* MMI_PDA_SUPPORT */

    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    {
        uint16  cur_index = 0;

#ifdef MMI_PDA_SUPPORT
        {
            MMI_CTRL_ID_T       ctrl_id = 0;
            
            if (PNULL != param)
            {
                ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;
                mark_num = GUILIST_GetSelectedItemIndex(list_ctrl_id, PNULL, 0);
                
                if (ctrl_id == MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID)
                {
                    if (GUILIST_GetTotalItemNum(list_ctrl_id) != mark_num)
                    {
                        GUISOFTKEY_SetTextId(win_id, MMICOMMON_SOFTKEY_CTRL_ID, TXT_CANCEL_SELECT_ALL, TXT_DELETE, STXT_CANCEL, FALSE );
                        BrwHandleBookmarkListMarkedAll(list_ctrl_id);
                    }
                    else
                    {
                        GUISOFTKEY_SetTextId(win_id, MMICOMMON_SOFTKEY_CTRL_ID, TXT_MARK_ALL, TXT_DELETE, STXT_CANCEL, FALSE );
                        GUILIST_SetAllSelected(list_ctrl_id,FALSE);
                    }
                    
#ifdef BROWSER_BOOKMARK_HISTORY_TAB
                    MMK_PostMsg(MMK_GetParentWinHandle(win_id), MSG_FULL_PAINT, PNULL, 0);
#else
                    MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
#endif     
                    break;
                    
                }
                else if (ctrl_id == MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID)
                {
                    BrwExitBookmarkListMarked(list_ctrl_id);
#ifdef BROWSER_BOOKMARK_HISTORY_TAB
                    MMK_PostMsg(MMK_GetParentWinHandle(win_id), MSG_FULL_PAINT, PNULL, 0);
#else
                    MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
#endif     
                    break;
                }
                else if (ctrl_id == MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID)
                {
                    if(0 == mark_num)
                    {
                        BrwOpenAlertWinByTextId(PNULL, TXT_SMS_SELECT_ONT_PROMPT, TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
                    }
                    else
                    {
                        BrwQueryIsDeleteBookmark(list_ctrl_id);
                    }
                    break;
                }
            }
        }
#endif /* MMI_PDA_SUPPORT */

        if ((GUILIST_GetSelectedItemIndex(list_ctrl_id,PNULL,0) > 0)//list in mark state
            || (0 == MMIBRW_BookmarkGetCount())
            )
        {
            break;
        }

        cur_index = GUILIST_GetCurItemIndex(list_ctrl_id);
        if (MMIBRW_BookmarkGotoURL(BrwGetBookmarkArrayIndex(cur_index)))
        {
            BookmarkTidyExistedBkWin();
        }
    }
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
#ifndef MMI_BROWSER_MINI_SUPPORT
        MMIBROWSER_BkExitMarked();
#endif

#ifdef BROWSER_START_PAGE_THEME1
        if (MMK_IsOpenWin(MMIBROWSER_START_PAGE_WIN_ID))
        {
            if (MMK_IsOpenWin(MMIBROWSER_MAIN_WIN_ID))
            {
                MMK_WinGrabFocus(MMIBROWSER_MAIN_WIN_ID);
            }
            else
            {
                MMIBROWSER_QueryExit();
            }
        }
        else
#endif
        {
            MMIBROWSER_CloseBookmarkWin();
        }
        break;
    
    case MSG_CTL_LIST_MARK_ITEM:
        //get mark num
        mark_num = GUILIST_GetSelectedItemIndex(list_ctrl_id,PNULL,0);
#ifdef MMI_PDA_SUPPORT
        if ((0 == mark_num) || (GUILIST_GetTotalItemNum(list_ctrl_id) > mark_num))
        {
            GUISOFTKEY_SetTextId(win_id, MMICOMMON_SOFTKEY_CTRL_ID, TXT_MARK_ALL, TXT_DELETE, STXT_CANCEL, TRUE);
        }
        else
        {
            GUISOFTKEY_SetTextId(win_id, MMICOMMON_SOFTKEY_CTRL_ID, TXT_CANCEL_SELECT_ALL, TXT_DELETE, STXT_CANCEL, TRUE);
        }
#else
        if (0 == mark_num)
        {
            //set list enable mark
            GUILIST_SetTextListMarkable(list_ctrl_id,FALSE);
        }
#endif /* MMI_PDA_SUPPORT */
        break;

    case MSG_CLOSE_WINDOW:
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}
#endif

/*****************************************************************************/
//  Description : set bookmark's mark list gray
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_BookmarkSetOptMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id,MMI_CTRL_ID_T list_ctrl_id)
{
#ifndef MMI_PDA_SUPPORT
#ifndef MMIDORADO_BOOKMARK_BY_NV
    FILEARRAY_TYPE_E add_array_data_ptr = (uint32)MMK_GetWinAddDataPtr(win_id);/*lint !e64*/
    MMIBRW_BOOKMARK_T *bookmark_info_ptr = MMIBRW_GetBookmark();
#endif
    MMI_MENU_GROUP_ID_T         group_id = 0;
    MMI_MENU_ID_T               menu_id = 0;
    uint16                  cur_index = 0;
    uint16                  mark_num = 0;
    MMI_MENU_GROUP_ID_T     mark_group_id = MENU_BRW_BOOKMARK_MARK;
#ifndef MMIDORADO_BOOKMARK_BY_NV
    MMI_MENU_GROUP_ID_T     add_group_id = MENU_BRW_BOOKMARK_ADD;
#endif
    const GUILIST_ITEM_T    *item_ptr = PNULL;
    uint16  cur_total_num = 0;

#ifndef MMIDORADO_BOOKMARK_BY_NV
    cur_total_num = bookmark_info_ptr->cur_index_num;
#else
    cur_total_num = MMIBRW_BookmarkGetCount();
#endif

    mark_num = GUILIST_GetSelectedItemIndex(list_ctrl_id,PNULL,0);
    GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
    cur_index = GUILIST_GetCurItemIndex(list_ctrl_id);

    if (0 != mark_num)//only:delete and mark
    {
        GUIMENU_SetItemVisible(ctrl_id, group_id, ID_POPUP_MENU_BOOKMARK_OPEN,   FALSE);
        GUIMENU_SetItemVisible(ctrl_id, group_id, ID_POPUP_MENU_BOOKMARK_EDIT,   FALSE);
        GUIMENU_SetItemVisible(ctrl_id, group_id, ID_POPUP_MENU_BOOKMARK_EDIT_FOLDER,   FALSE);
        GUIMENU_SetItemVisible(ctrl_id, group_id, ID_POPUP_MENU_BOOKMARK_DELETE_FOLDER,   FALSE);
        GUIMENU_SetItemVisible(ctrl_id, group_id, ID_POPUP_MENU_BOOKMARK_ADD,   FALSE);
        GUIMENU_SetItemVisible(ctrl_id, group_id, ID_POPUP_MENU_BOOKMARK_MOVE_TO,   FALSE);
        GUIMENU_SetItemVisible(ctrl_id, group_id, ID_POPUP_MENU_BOOKMARK_SEND_URL,   FALSE);
        GUIMENU_SetItemVisible(ctrl_id, group_id, ID_POPUP_MENU_BOOKMARK_DETAIL,   FALSE);
        GUIMENU_SetItemVisible(ctrl_id, group_id, ID_POPUP_MENU_BOOKMARK_AS_HOMEPAGE,   FALSE);
#ifdef MMIDORADO_MY_NAV_SUPPORT
        GUIMENU_SetItemVisible(ctrl_id, group_id, ID_POPUP_MENU_BOOKMARK_ADD2MYNAV,   FALSE);
#endif
#ifdef SNS_SUPPORT    
        GUIMENU_SetItemVisible(ctrl_id, group_id, ID_POPUP_MENU_BOOKMARK_SHARE2SNS, FALSE);
#endif
    }
#ifndef MMIDORADO_BOOKMARK_BY_NV
    else if (FILEARRAY_TYPE_FOLDER == add_array_data_ptr)
    {
        FILEARRAY_DATA_T f_array_data = {0};
        
        MMIAPIFILEARRAY_Read(bookmark_info_ptr->file_array, BrwGetBookmarkArrayIndex(cur_index), &f_array_data);

        GUIMENU_SetItemVisible(ctrl_id, group_id, ID_POPUP_MENU_BOOKMARK_OPEN,   FALSE);
        GUIMENU_SetItemVisible(ctrl_id, group_id, ID_POPUP_MENU_BOOKMARK_EDIT,   FALSE);
        GUIMENU_SetItemVisible(ctrl_id, group_id, ID_POPUP_MENU_BOOKMARK_DELETE,   FALSE);
        GUIMENU_SetItemVisible(ctrl_id, group_id, ID_POPUP_MENU_BOOKMARK_SEND_URL,   FALSE);
        GUIMENU_SetItemVisible(ctrl_id, group_id, ID_POPUP_MENU_BOOKMARK_DETAIL,   FALSE);
        GUIMENU_SetItemVisible(ctrl_id, group_id, ID_POPUP_MENU_BOOKMARK_MOVE_TO,   FALSE);
        GUIMENU_SetItemVisible(ctrl_id, group_id, ID_POPUP_MENU_BOOKMARK_AS_HOMEPAGE,   FALSE);
#ifdef MMIDORADO_MY_NAV_SUPPORT
        GUIMENU_SetItemVisible(ctrl_id, group_id, ID_POPUP_MENU_BOOKMARK_ADD2MYNAV,   FALSE);
#endif
#ifdef SNS_SUPPORT    
        GUIMENU_SetItemVisible(ctrl_id, group_id, ID_POPUP_MENU_BOOKMARK_SHARE2SNS, FALSE);
#endif
    }
    else
    {
        GUIMENU_SetItemVisible(ctrl_id, group_id, ID_POPUP_MENU_BOOKMARK_EDIT_FOLDER,   FALSE);
        GUIMENU_SetItemVisible(ctrl_id, group_id, ID_POPUP_MENU_BOOKMARK_DELETE_FOLDER,   FALSE);
        if(0 == MMIBRW_BookmarkGetFolderNum(FALSE))//无子文件夹
        {
            GUIMENU_SetItemGrayed(ctrl_id, group_id, ID_POPUP_MENU_BOOKMARK_MOVE_TO, TRUE);
        }
    }
#endif    

#ifndef MMIDORADO_BOOKMARK_BY_NV
    if (0 != bookmark_info_ptr->file_array_step) //非根目录情况下,不允许添加目录
    {
        //GUIMENU_SetItemGrayed(ctrl_id, add_group_id, ID_POPUP_MENU_BOOKMARK_ADD_FOLDER, TRUE);
        GUIMENU_SetItemVisible(ctrl_id, add_group_id, ID_POPUP_MENU_BOOKMARK_ADD_FOLDER, FALSE);
    }
#endif

    //no bookmark
    if (0 == cur_total_num)
    {
        GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_POPUP_MENU_BOOKMARK_OPEN,TRUE);
        GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_POPUP_MENU_BOOKMARK_EDIT,TRUE);
        GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_POPUP_MENU_BOOKMARK_DELETE,TRUE);
        GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_POPUP_MENU_BOOKMARK_MARK,TRUE);
        GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_POPUP_MENU_BOOKMARK_MOVE_TO,TRUE);
        GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_POPUP_MENU_BOOKMARK_SEND_URL,TRUE);
        GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_POPUP_MENU_BOOKMARK_DETAIL,TRUE);
        GUIMENU_SetItemGrayed(ctrl_id, group_id, ID_POPUP_MENU_BOOKMARK_AS_HOMEPAGE,TRUE);
#ifdef MMIDORADO_MY_NAV_SUPPORT
        GUIMENU_SetItemGrayed(ctrl_id, group_id, ID_POPUP_MENU_BOOKMARK_ADD2MYNAV,TRUE);
#endif
#ifdef SNS_SUPPORT    
        GUIMENU_SetItemGrayed(ctrl_id, group_id, ID_POPUP_MENU_BOOKMARK_SHARE2SNS, TRUE);
#endif
        return;
    }

    //mark sub menu
    item_ptr = GUILIST_GetItemPtrByIndex(list_ctrl_id,cur_index);
    if (PNULL == item_ptr)
    {
        //SCI_TRACE_LOW:"[BRW]W MMIBROWSER get item error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_BOOKMARK_1541_112_2_18_2_6_28_260,(uint8*)"");
        return;
    }
    if (GUILIST_GetItemState(item_ptr,GUIITEM_STATE_SELECTED))
    {
        GUIMENU_SetItemGrayed(ctrl_id,mark_group_id,ID_POPUP_MENU_BOOKMARK_MARKED,TRUE);
    }
    else
    {
        GUIMENU_SetItemGrayed(ctrl_id,mark_group_id,ID_POPUP_MENU_BOOKMARK_UNMARKED,TRUE);
    }

    if (mark_num > 0)
    {
        if (mark_num >= cur_total_num)
        {
            GUIMENU_SetItemGrayed(ctrl_id,mark_group_id,ID_POPUP_MENU_BOOKMARK_MARKED_ALL,TRUE);
        }
    }
    else
    {
        GUIMENU_SetItemGrayed(ctrl_id,mark_group_id,ID_POPUP_MENU_BOOKMARK_UNMARKED_ALL,TRUE);
    }
#else
    MMI_MENU_GROUP_ID_T         group_id = 0;
    MMI_MENU_ID_T               menu_id = 0;
    uint16  cur_total_num = 0;
#ifndef MMIDORADO_BOOKMARK_BY_NV
    MMIBRW_BOOKMARK_T *bookmark_info_ptr = MMIBRW_GetBookmark();
#endif

#ifndef MMIDORADO_BOOKMARK_BY_NV
    cur_total_num = bookmark_info_ptr->cur_index_num;
#else
    cur_total_num = MMIBRW_BookmarkGetCount();
#endif

    GUIMENU_GetId(ctrl_id, &group_id, &menu_id);

#ifndef MMIDORADO_BOOKMARK_BY_NV
    if (0 != bookmark_info_ptr->file_array_step) //非根目录情况下,不允许添加目录
    {
        //GUIMENU_SetItemGrayed(ctrl_id, group_id, ID_POPUP_MENU_BOOKMARK_ADD_FOLDER, TRUE);
        GUIMENU_SetItemVisible(ctrl_id, group_id, ID_POPUP_MENU_BOOKMARK_ADD_FOLDER, FALSE);
    }
#endif

    //no bookmark
    if (0 == cur_total_num)
    {
        GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_POPUP_MENU_BOOKMARK_MARK,TRUE);
    }
#endif
}

/*****************************************************************************/
//  Description : handle bookmark edit window message
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleBookMarkEditWinMsg(
                                   MMI_WIN_ID_T	win_id, 
                                   MMI_MESSAGE_ID_E	msg_id, 
                                   DPARAM		param
                                   )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMIBRW_BM_CHANGE_TYPE_E change_type = (uint32)MMK_GetWinAddDataPtr(win_id);

    switch(msg_id) 
    {
    case MSG_OPEN_WINDOW:
    {
        MMI_TEXT_ID_T   title_text_id = TXT_EDIT;

#ifdef MMI_PDA_SUPPORT
        GUIFORM_SetType(MMIBROWSER_BOOKMARK_EDIT_CTRL_ID, GUIFORM_TYPE_TP);
        GUIEDIT_SetAlign(MMIBROWSER_BOOKMARK_EDIT_TITLE_CTRL_ID, ALIGN_LVMIDDLE);
        GUIEDIT_SetAlign(MMIBROWSER_BOOKMARK_EDIT_URL_CTRL_ID, ALIGN_LVMIDDLE);
#endif /* MMI_PDA_SUPPORT */

        switch (change_type)
        {
#ifndef MMIDORADO_BOOKMARK_BY_NV
        case MMIBRW_BM_CHANGE_ADD_DIR:
            title_text_id = TXT_BROWSER_FOLDER_ROUTE_ADD;
            break;
            
        case MMIBRW_BM_CHANGE_EDIT_DIR:
            title_text_id = TXT_BROWSER_BOOKMARK_EDIT_FOLDER;
            break;
#endif

        case MMIBRW_BM_CHANGE_ADD_CUR_PAGE:
        case MMIBRW_BM_CHANGE_ADD_URL:
        case MMIBRW_BM_CHANGE_ADD_NULL:
            title_text_id = TXT_ADD_BOOKMARK;
            break;
            
        default:
            break;
        }
        
        GUIWIN_SetTitleTextId(win_id, title_text_id, FALSE);
        BrwSetBookmarkEditWindow(change_type);
        MMK_SetAtvCtrl(win_id,MMIBROWSER_BOOKMARK_EDIT_CTRL_ID);
    }
        break;
        
#ifndef MMIDORADO_BOOKMARK_BY_NV
    case MSG_CTL_SETLIST_SWITCH:
    {
        uint32  folder_index = 0;
        
        if(MMIBROWSER_BK_EDIT_FORM3_SETLIST_CTRL_ID == MMK_GetActiveCtrlId(win_id))
        {
            folder_index = GUISETLIST_GetCurIndex(MMIBROWSER_BK_EDIT_FORM3_SETLIST_CTRL_ID);
            BrwSetPathFolderIndex(folder_index);
        }
    }
        break;
#endif

#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
            MMI_CTRL_ID_T src_ctrl_id = 0;

            if (PNULL != param)
            {
                src_ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;

                if (src_ctrl_id == MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID)
                {
                    MMK_PostMsg(win_id, MSG_CTL_OK, PNULL, 0);
                }
                else if (src_ctrl_id == MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID)
                {
                    MMK_PostMsg(win_id, MSG_CTL_MIDSK, PNULL, 0);
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

    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
#ifndef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK: 
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527    
#endif /* MMI_PDA_SUPPORT */
    case MSG_CTL_OK:
    case MSG_APP_OK:
        //BrowserHanldeSaveInEditWin(win_id, change_type);
        {
            MMI_STRING_T url_text_str = {0};
            MMI_STRING_T title_text_str = {0}; 
            
            GUIEDIT_GetString(MMIBROWSER_BOOKMARK_EDIT_TITLE_CTRL_ID, &title_text_str);
            GUIEDIT_GetString(MMIBROWSER_BOOKMARK_EDIT_URL_CTRL_ID, &url_text_str);
        
#ifndef MMIDORADO_BOOKMARK_BY_NV
            if (BrwParserEditInfoIsValid(change_type, title_text_str, url_text_str))//alert fail info
            {
                BrwBegin2Save(change_type);

            }
#else
            {
                uint16  index = 0;
                MMIBRW_BOOKMARK_ITEM_T      item_info ={0};

                if ((PNULL == title_text_str.wstr_ptr) || (0 == title_text_str.wstr_len))
                {
                    //alert the name is empty
                    BrwOpenAlertWinByTextId(PNULL,TXT_COMM_NO_TITLE,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                    break;
                }
                
                if ((PNULL == url_text_str.wstr_ptr) || (0 == url_text_str.wstr_len))
                {
                    BrwOpenAlertWinByTextId(PNULL,TXT_COMM_URL_NULL,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                    break;
                }
                else
                {
                    //save url
                    MMI_WSTRNTOSTR((uint8*)item_info.url_ptr,MMIBROWSER_MAX_URL_LEN,
                                                    url_text_str.wstr_ptr, url_text_str.wstr_len, 
                                                    MIN(MMIBROWSER_MAX_URL_LEN, url_text_str.wstr_len));
                }
                
                if (Brw_IsIncludeInvalidUrl((uint8*)item_info.url_ptr))
                {
                    MMIBROWSER_AlertUrlInvalid();
                    break;
                }
                
                item_info.title_len = MIN(title_text_str.wstr_len,MMIBRW_BOOKMARK_MAX_TITLE_LENGTH);
                MMI_WSTRNCPY(item_info.title_arr, MMIBRW_BOOKMARK_MAX_TITLE_LENGTH,
                    title_text_str.wstr_ptr, title_text_str.wstr_len,
                    title_text_str.wstr_len);
                
                if (MMIBRW_BM_CHANGE_EDIT == change_type)
                {
                    MMI_HANDLE_T detail_win_handle = MMIBROWSER_BOOKMARK_DETAIL_WIN_ID;
                    
                    index = GUILIST_GetCurItemIndex(MMIBROWSER_LIST_BOOKMARK_CTRL_ID);
                    if (!MMIBRW_BookmarkUpdateItem(index, &item_info))
                    {
                        SCI_TRACE_LOW("BrwHandleBookMarkEditWinMsg MMIBRW_BookmarkUpdateItem error");
                        break;
                    }
#ifdef MMI_GUI_STYLE_TYPICAL
                    BrwOpenAlertWinByTextId(PNULL,TXT_COMPLETE,TXT_NULL,IMAGE_PUBWIN_SUCCESS,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
#endif /* MMI_GUI_STYLE_TYPICAL */

                    MMIBRW_AppendListboxitem(MMIBROWSER_BOOKMARK_WIN_ID, MMIBROWSER_LIST_BOOKMARK_CTRL_ID, 
                            MMIBRW_BookmarkGetCount ());
                    GUILIST_SetCurItemIndex(MMIBROWSER_LIST_BOOKMARK_CTRL_ID, index);

                    BrwUpdateBKDetailWinInfo(detail_win_handle, title_text_str, url_text_str);
                }
                else if (MMIBRW_BM_CHANGE_ADD_CUR_PAGE == change_type)
                {
                    if (!MMIBRW_BookmarkAddItem(&item_info))
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_COMON_BOOKMARK_FULL);
                        MMK_CloseWin(win_id);
                        break;
                    }
#ifdef MMI_GUI_STYLE_TYPICAL
                    BrwOpenAlertWinByTextId(PNULL,TXT_COMPLETE,TXT_NULL,IMAGE_PUBWIN_SUCCESS,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
#endif /* MMI_GUI_STYLE_TYPICAL */
                }
                else
                {
                    if (!MMIBRW_BookmarkAddItem(&item_info))
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_COMON_BOOKMARK_FULL);
                        MMK_CloseWin(win_id);
                        break;
                    }
                    
#ifdef MMI_GUI_STYLE_TYPICAL
                    BrwOpenAlertWinByTextId(PNULL,TXT_COMPLETE,TXT_NULL,IMAGE_PUBWIN_SUCCESS,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
#endif /* MMI_GUI_STYLE_TYPICAL */

                    if (MMK_IsOpenWin(MMIBROWSER_BOOKMARK_WIN_ID))
                    {
                        MMIBRW_AppendListboxitem(MMIBROWSER_BOOKMARK_WIN_ID, MMIBROWSER_LIST_BOOKMARK_CTRL_ID, 
                                MMIBRW_BookmarkGetCount ());
                        
                         GUILIST_SetCurItemIndex(MMIBROWSER_LIST_BOOKMARK_CTRL_ID, MMIBRW_BookmarkGetCount ());
                   }
                    
                }
                MMK_CloseWin(win_id);
            }
#endif
        }        
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        MMIBROWSER_FreeBookmarkEdit();
        break;
    
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}

/*****************************************************************************/
//  Description : set bookmark edit window message
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void BrwSetBookmarkEditWindow(MMIBRW_BM_CHANGE_TYPE_E change_type)
{
#ifndef MMIDORADO_BOOKMARK_BY_NV
    uint16  current_index = BrwGetBookmarkCurIndex();
    MMIBRW_BOOKMARK_T *bookmark_info_ptr = MMIBRW_GetBookmark();
    MMI_CTRL_ID_T      set_path_ctrl_id = MMIBROWSER_BK_EDIT_FORM3_SETLIST_CTRL_ID;
#else
    uint16  current_index = GUILIST_GetCurItemIndex(MMIBROWSER_LIST_BOOKMARK_CTRL_ID);
#endif
    MMI_CTRL_ID_T      edit_title_ctrl_id = MMIBROWSER_BOOKMARK_EDIT_TITLE_CTRL_ID;
    MMI_CTRL_ID_T      edit_url_ctrl_id = MMIBROWSER_BOOKMARK_EDIT_URL_CTRL_ID;

    //SCI_TRACE_LOW:"[BRW]BrwSetBookmarkEditWindow change_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_BOOKMARK_1728_112_2_18_2_6_29_261,(uint8*)"d", change_type);

    GUIFORM_SetStyle(MMIBROWSER_BK_EDIT_FORM1_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIBROWSER_BK_EDIT_FORM2_CTRL_ID,GUIFORM_STYLE_UNIT);
#ifndef MMIDORADO_BOOKMARK_BY_NV
    GUIFORM_SetStyle(MMIBROWSER_BK_EDIT_FORM3_CTRL_ID,GUIFORM_STYLE_UNIT);
#endif
    
#ifndef MMIDORADO_BOOKMARK_BY_NV
    if ((MMIBRW_BM_CHANGE_ADD_DIR == change_type) ||(MMIBRW_BM_CHANGE_EDIT_DIR == change_type))//folder 
    {
        MMI_STRING_T        bookmark_edit_folder = {0};
        MMI_STRING_T        add_dir_str = {0};
        uint32 i = 0;

        GUILABEL_SetTextById(MMIBROWSER_BK_EDIT_LABEL1_CTRL_ID, TXT_COMMON_NAME, FALSE);
        GUIFORM_SetChildDisplay(MMIBROWSER_BOOKMARK_EDIT_CTRL_ID,MMIBROWSER_BK_EDIT_FORM2_CTRL_ID,GUIFORM_CHILD_DISP_HIDE);
        GUIFORM_SetChildDisplay(MMIBROWSER_BOOKMARK_EDIT_CTRL_ID,MMIBROWSER_BK_EDIT_FORM3_CTRL_ID,GUIFORM_CHILD_DISP_HIDE);

        //NEWMS00197620
        GUIEDIT_SetTextMaxLen(MMIBROWSER_BOOKMARK_EDIT_TITLE_CTRL_ID, MMIBRW_BOOKMARK_MAX_FOLDER_NAME_LENGTH, MMIBRW_BOOKMARK_MAX_FOLDER_NAME_LENGTH);

        if(MMIBRW_BM_CHANGE_ADD_DIR == change_type)
        {
            uint8 slash_len = 1;
            uint8 default_folder_index_len = 1;

            MMI_GetLabelTextByLang(TXT_BROWSER_FOLDER_ROUTE_ADD, &add_dir_str);
            add_dir_str.wstr_len = MIN(add_dir_str.wstr_len, (MMIBRW_BOOKMARK_MAX_FOLDER_NAME_LENGTH - default_folder_index_len));
            
            for (i = 1; i <= MMIBRW_BOOKMARK_DIR_MAX_NUM; i++)
            {
                wchar full_path_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
                uint16 full_path_name_len = MMIFILE_FULL_PATH_MAX_LEN;

                //BrwCombineFullPath((char *)MMIBRW_BOOKMARK_FILE_DIR, full_path_name, &full_path_name_len);
                BrwGetDefaultRootPath(full_path_name, &full_path_name_len);

                if (full_path_name_len + slash_len + add_dir_str.wstr_len + default_folder_index_len <= MMIFILE_FULL_PATH_MAX_LEN)
                {
                    full_path_name[full_path_name_len] = MMIFILE_SLASH;
                    full_path_name_len += slash_len;

                    MMI_WSTRNCPY(full_path_name + full_path_name_len, MMIFILE_FULL_PATH_MAX_LEN, add_dir_str.wstr_ptr, add_dir_str.wstr_len, add_dir_str.wstr_len);
                    full_path_name_len += add_dir_str.wstr_len;

                    full_path_name[full_path_name_len] = 0x30 + i;
                    full_path_name_len += default_folder_index_len;

                    //is folder exist
                    if (!MMIAPIFMM_IsFolderExist(full_path_name, full_path_name_len))
                    {
                        SCI_MEMSET(full_path_name, 0x00, sizeof(full_path_name));

                        MMI_WSTRNCPY(full_path_name, MMIFILE_FULL_PATH_MAX_LEN, add_dir_str.wstr_ptr, add_dir_str.wstr_len, add_dir_str.wstr_len);
                        full_path_name_len = add_dir_str.wstr_len;

                        full_path_name[full_path_name_len] = 0x30 + i;
                        full_path_name_len += default_folder_index_len;

                        GUIEDIT_SetString(edit_title_ctrl_id,full_path_name,full_path_name_len);
                        break;
                    }
                }
            }

            if (i > MMIBRW_BOOKMARK_DIR_MAX_NUM)
            {
                GUIEDIT_SetString(edit_title_ctrl_id,add_dir_str.wstr_ptr,add_dir_str.wstr_len);
            }
        }
        else//edit dir
        {           
            MMIBRW_GetBookmarkTitle(current_index,&bookmark_edit_folder);
            GUIEDIT_SetString(edit_title_ctrl_id,bookmark_edit_folder.wstr_ptr, bookmark_edit_folder.wstr_len);
            if(PNULL != bookmark_edit_folder.wstr_ptr)
            {
                SCI_FREE(bookmark_edit_folder.wstr_ptr);
            }
        }
    }
    else
#endif        
    {
        MMI_STRING_T        bookmark_edit_title = {0};
        MMI_STRING_T        bookmark_edit_url = {0};
        //MMI_STRING_T bookmark_fodler_name ={0};
        
        GUILABEL_SetTextById(MMIBROWSER_BK_EDIT_LABEL1_CTRL_ID, TXT_BROWSER_FILE_TITLE, FALSE);
        GUILABEL_SetTextById(MMIBROWSER_BK_EDIT_LABEL2_CTRL_ID, TXT_URL_ADRESS, FALSE);
#ifndef MMIDORADO_BOOKMARK_BY_NV
        GUILABEL_SetTextById(MMIBROWSER_BK_EDIT_LABEL3_CTRL_ID, TXT_WWW_BOOKMARK_ADD_DIR, FALSE);//目录:
#endif
        
        GUIEDIT_SetIm(edit_url_ctrl_id,(uint32)GUIIM_TYPE_ENGLISH | (uint32)GUIIM_TYPE_DIGITAL | (uint32)GUIIM_TYPE_ABC,GUIIM_TYPE_ABC);//MS00220112
        GUIEDIT_SetImTag(edit_url_ctrl_id, GUIIM_TAG_URL);

        //title and url
        switch (change_type)
        {
        case MMIBRW_BM_CHANGE_ADD_NULL:
        {
            wchar   temp[MMIBROWSER_MAX_URL_LEN+1] ={0};

            MMI_STRNTOWSTR(temp, MMIBROWSER_MAX_URL_LEN, (uint8*)MMIBROWSER_HTTP_STRING, MMIBROWSER_MAX_URL_LEN,MMIBROWSER_MAX_URL_LEN);
            bookmark_edit_url.wstr_len = MMIAPICOM_Wstrlen(temp);
            bookmark_edit_url.wstr_ptr = temp;/*lint !e733*/
            GUIEDIT_SetString(edit_url_ctrl_id, bookmark_edit_url.wstr_ptr, bookmark_edit_url.wstr_len);
        }
            break;
            
        case MMIBRW_BM_CHANGE_ADD_CUR_PAGE:
        {            
            uint16  tile_len = 0;
            uint16  file_title[MMIBRW_BOOKMARK_MAX_TITLE_LENGTH + 1] = {0};

            if (BRW_GetPageTitle(MMIBROWSER_GetInstance(),file_title,&tile_len, MMIBRW_BOOKMARK_MAX_TITLE_LENGTH))
            {
                bookmark_edit_title.wstr_ptr = file_title;/*lint !e733*/
                bookmark_edit_title.wstr_len = tile_len;
                GUIEDIT_SetString(edit_title_ctrl_id,bookmark_edit_title.wstr_ptr,bookmark_edit_title.wstr_len);
            }
            else
            {
                //title is null
            }
            
            MMIBROWSER_GetCurPageUrlInfo(PNULL, &bookmark_edit_url.wstr_ptr);
            bookmark_edit_url.wstr_len = MMIAPICOM_Wstrlen(bookmark_edit_url.wstr_ptr);
            if (bookmark_edit_url.wstr_len > 0)
            {
                GUIEDIT_SetString(edit_url_ctrl_id, bookmark_edit_url.wstr_ptr, bookmark_edit_url.wstr_len);
            }
            MMIBROWSER_FreeCurPageUrlInfo(PNULL, &bookmark_edit_url.wstr_ptr);
        }
            break;

        case MMIBRW_BM_CHANGE_ADD_URL:/*show title from outside,such as SMS,MMS et.*/
        {
            MMIBROWSER_BOOKMARK_EDIT_T* bookmark_edit_ptr = PNULL;
            
            bookmark_edit_ptr = MMIBROWSER_GetBookmarkEdit();
            if(PNULL != bookmark_edit_ptr)
            {
                bookmark_edit_title.wstr_ptr = bookmark_edit_ptr->title_ptr;
                bookmark_edit_title.wstr_len = (uint16)bookmark_edit_ptr->title_len;
                GUIEDIT_SetString(edit_title_ctrl_id,bookmark_edit_title.wstr_ptr,bookmark_edit_title.wstr_len);

                bookmark_edit_url.wstr_ptr = bookmark_edit_ptr->url_ptr;
                bookmark_edit_url.wstr_len = (uint16)bookmark_edit_ptr->url_len;
                GUIEDIT_SetString(edit_url_ctrl_id, bookmark_edit_url.wstr_ptr, bookmark_edit_url.wstr_len);
            }
            else
            {
                //SCI_TRACE_LOW:"[BRW]W BrwSetBookmarkEditWindow MMIBRW_BM_CHANGE_ADD_URL err"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_BOOKMARK_1870_112_2_18_2_6_29_262,(uint8*)"");
            }
        }        
            break;
            
        case MMIBRW_BM_CHANGE_EDIT:
        {
#ifndef MMIDORADO_BOOKMARK_BY_NV
            if (FILEARRAY_TYPE_FILE == MMIBRW_GetBookmarkTitle(current_index,&bookmark_edit_title))
            {
                GUIEDIT_SetString(edit_title_ctrl_id,bookmark_edit_title.wstr_ptr,bookmark_edit_title.wstr_len);
            }
            if(PNULL != bookmark_edit_title.wstr_ptr)
            {
                SCI_FREE(bookmark_edit_title.wstr_ptr);
            }
#else
            if (MMIBRW_GetBookmarkTitle(current_index,&bookmark_edit_title))
            {
                GUIEDIT_SetString(edit_title_ctrl_id,bookmark_edit_title.wstr_ptr,bookmark_edit_title.wstr_len);
            }
#endif
            if (MMIBRW_GetBookmarkUrl(current_index,&bookmark_edit_url))
            {
                GUIEDIT_SetString(edit_url_ctrl_id, bookmark_edit_url.wstr_ptr, bookmark_edit_url.wstr_len);
            }
            if(PNULL != bookmark_edit_url.wstr_ptr)
            {
                SCI_FREE(bookmark_edit_url.wstr_ptr);
            }

        }
            break;
            
        default:
            break;
        }
          
#ifndef MMIDORADO_BOOKMARK_BY_NV
        //path
        if((MMIBRW_BM_CHANGE_ADD_CUR_PAGE == change_type)
            || (MMIBRW_BM_CHANGE_ADD_URL== change_type))
        {
            BrwSetPathFolderIndex(0);//add url set default as "root"
        }
        else
        {
            if(0 == bookmark_info_ptr->file_array_step)
            {
                BrwSetPathFolderIndex(0);//add url set default as "root"
            }
            else
            {
                BrwSetPathFolderIndex(s_pre_path_index + 1);//获取上一级路径,考虑root在内
            }
        }
        
        /*记住原始的目录index*/
        BrwSetPathOringalFolderIndex(BrwGetPathFolderIndex());

        if (MMIBRW_BM_CHANGE_EDIT == change_type)//edit, don't permit to change the dir
        {
            GUIFORM_SetChildDisplay(MMIBROWSER_BOOKMARK_EDIT_CTRL_ID,MMIBROWSER_BK_EDIT_FORM3_CTRL_ID,GUIFORM_CHILD_DISP_HIDE);
        }
        else
        {
            MMI_STRING_T    *folder_name_array_ptr = PNULL;
            uint16  folder_total_num = 0;
            BOOLEAN is_get_folder_info = FALSE;
            uint16  loop = 0;
            GUISETLIST_ITEM_INFO_T  item_info = {0};

            //the first item:root
            GUISETLIST_SetFontAlign(set_path_ctrl_id, ALIGN_LVMIDDLE);
            GUISETLIST_AddItemById(set_path_ctrl_id, TXT_BROWSER_ROOT_FOLDER_ROUTE);//set route

            //other folder items
            is_get_folder_info = MMIBRW_BookmarkGetFolderNameArray(PNULL, &folder_name_array_ptr, &folder_total_num);

            if (is_get_folder_info && (folder_name_array_ptr != PNULL))
            {
                for (loop = 0; loop < folder_total_num; loop++)
                {
                    MMI_STRING_T    *folder_name_ptr = folder_name_array_ptr + loop;
                    
                    if (folder_name_ptr != PNULL)/*lint !e774*/
                    {               
                        SCI_MEMSET(&item_info, 0x00, sizeof(GUISETLIST_ITEM_INFO_T));
                        item_info.item_state = GUISETLIST_ITEM_STATE_TEXT;
                        item_info.str_info.wstr_ptr = folder_name_ptr->wstr_ptr;
                        item_info.str_info.wstr_len = folder_name_ptr->wstr_len;
                        GUISETLIST_AddItem(set_path_ctrl_id,&item_info);
                        if(PNULL != folder_name_ptr->wstr_ptr)
                        {
                            SCI_FREE(folder_name_ptr->wstr_ptr);
                        }
                    }
                    else
                    {
                        break;//protect
                    }
                }
            }

            if (folder_name_array_ptr != PNULL)
            {
                SCI_FREE(folder_name_array_ptr);
            }

            //path setting:
            //GUISETLIST_SetCtrlState(set_path_ctrl_id, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);
            GUISETLIST_SetCurIndex(set_path_ctrl_id, BrwGetPathFolderIndex());
            GUISETLIST_SetTitleTextId(set_path_ctrl_id,TXT_SELECT_FOLDER);
        }
#endif
    }
}


/*****************************************************************************/
//  Description : handle bookmark popmenu window message
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleBookmarkPopMenuWinMsg(MMI_WIN_ID_T win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param)
{
    MMI_MENU_GROUP_ID_T         group_id = 0;
    MMI_MENU_ID_T               menu_id = 0;
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               ctrl_id = MMIBROWSER_BOOKMARK_CTRL_ID;
    MMI_CTRL_ID_T               list_ctrl_id = MMIBROWSER_LIST_BOOKMARK_CTRL_ID;
#ifndef MMIDORADO_BOOKMARK_BY_NV
    FILEARRAY_TYPE_E add_array_data_ptr = (uint32)MMK_GetWinAddDataPtr(win_id);/*lint !e64*/
#endif
    //MMIBRW_BOOKMARK_T *bookmark_info_ptr = MMIBRW_GetBookmark();
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIBROWSER_BookmarkSetOptMenu(win_id, ctrl_id, list_ctrl_id);
        MMK_SetAtvCtrl(win_id, ctrl_id);						
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
    {
        uint16          array_index = 0;     //bookmark index include folder

        array_index = BrwGetBookmarkArrayIndex(GUILIST_GetCurItemIndex(list_ctrl_id));
        
        GUIMENU_GetId(ctrl_id,&group_id,&menu_id);
        switch(menu_id) 
        {
        case ID_POPUP_MENU_BOOKMARK_OPEN:
#ifndef MMIDORADO_BOOKMARK_BY_NV
            if(FILEARRAY_TYPE_FOLDER == add_array_data_ptr)
            {
                BrwOpenBookmarkStepWin(BOOKMARK_STEP_TYPE_NEXT, array_index, FUNC_FIND_NULL);
            }
            else
#endif                
            {
                if (MMIBRW_BookmarkGotoURL(array_index))
                {
                    MMK_CloseWin(win_id);
                    BookmarkTidyExistedBkWin();      
                }
                else
                {
                    //do nothing
                }
            }
            break;
            
        case ID_POPUP_MENU_BOOKMARK_ADD_FILE:
        case ID_POPUP_MENU_BOOKMARK_ADD:
            MMIBROWSER_OpenBookmarkEditWin(MMIBRW_BM_CHANGE_ADD_NULL);
            MMK_CloseWin(win_id);
            break;
            
#ifndef MMIDORADO_BOOKMARK_BY_NV
        case ID_POPUP_MENU_BOOKMARK_ADD_FOLDER:
            MMIBROWSER_OpenBookmarkEditWin(MMIBRW_BM_CHANGE_ADD_DIR);
            MMK_CloseWin(win_id);
            break;
#endif

        case ID_POPUP_MENU_BOOKMARK_DELETE:
#ifndef MMIDORADO_BOOKMARK_BY_NV
        case ID_POPUP_MENU_BOOKMARK_DELETE_FOLDER:
#endif            
            BrwQueryIsDeleteBookmark(list_ctrl_id);
            MMK_CloseWin(win_id);
            break;
            
        case ID_POPUP_MENU_BOOKMARK_SEND_URL:
            {
                MMI_STRING_T    title_str = {0};
                MMI_STRING_T    url_str = {0};

#ifndef MMIDORADO_BOOKMARK_BY_NV
                if ((FILEARRAY_TYPE_FILE == MMIBRW_GetBookmarkTitle(array_index, &title_str))
                    && MMIBRW_GetBookmarkUrl(array_index, &url_str))
#else
                if (MMIBRW_GetBookmarkTitle(array_index, &title_str) && MMIBRW_GetBookmarkUrl(array_index, &url_str))
#endif
                {        
                    if(MMIBROWSER_SendUrlContentBySms(title_str.wstr_ptr, title_str.wstr_len, url_str.wstr_ptr, url_str.wstr_len))
                    {
                        //successful
                    }
                    else
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_UNKNOWN_ERROR);
                    }
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_UNKNOWN_ERROR);
                }
                
#ifndef MMIDORADO_BOOKMARK_BY_NV
                if(PNULL !=title_str.wstr_ptr)
                {
                    SCI_FREE(title_str.wstr_ptr);
                }
#endif

                if(PNULL !=url_str.wstr_ptr)
                {
                    SCI_FREE(url_str.wstr_ptr);
                }
                
                MMK_CloseWin(win_id);
            }
            break;

        case ID_POPUP_MENU_BOOKMARK_EDIT:
            MMIBROWSER_OpenBookmarkEditWin(MMIBRW_BM_CHANGE_EDIT);
            MMK_CloseWin(win_id);
            break;
            
#ifndef MMIDORADO_BOOKMARK_BY_NV
        case ID_POPUP_MENU_BOOKMARK_EDIT_FOLDER:
            MMIBROWSER_OpenBookmarkEditWin(MMIBRW_BM_CHANGE_EDIT_DIR);
            MMK_CloseWin(win_id);
            break;

        case ID_POPUP_MENU_BOOKMARK_MOVE_TO:
            BrwBookmarkEnterMoveState(win_id, array_index);
            MMK_CloseWin(win_id);
            break;
#endif

        case ID_POPUP_MENU_BOOKMARK_DETAIL:
            MMK_CreateWin((uint32*)BRW_BOOKMARK_DETAIL_WIN_TAB,(ADD_DATA)array_index);
            MMK_CloseWin(win_id);
            break;

#ifdef MMI_PDA_SUPPORT
        case ID_POPUP_MENU_BOOKMARK_MARK://enter to marked status
            BrwEnterBookmarkListMarked(list_ctrl_id);
            MMK_CloseWin(win_id);
            break;
#endif /* MMI_PDA_SUPPORT */

        case ID_POPUP_MENU_BOOKMARK_MARKED:
            BrwHandleBookmarkListMarked(list_ctrl_id);
            MMK_CloseWin(win_id);
            break;
            
        case ID_POPUP_MENU_BOOKMARK_UNMARKED:
            BrwHandleBookmarkListUnmarked(list_ctrl_id);
            MMK_CloseWin(win_id);
            break;

        case ID_POPUP_MENU_BOOKMARK_MARKED_ALL:
            BrwHandleBookmarkListMarkedAll(list_ctrl_id);
            MMK_CloseWin(win_id);
            break;

        case ID_POPUP_MENU_BOOKMARK_UNMARKED_ALL:
            BrwHandleBookmarkListUnmarkedAll(list_ctrl_id);
            MMK_CloseWin(win_id);
            break;

        case ID_POPUP_MENU_BOOKMARK_AS_HOMEPAGE:
        {
            MMI_STRING_T    url_str = {0};
            
            if (MMIBRW_GetBookmarkUrl(array_index, &url_str) && MMIBROWSER_SetHomepageUrl(&url_str))
            {
#ifdef MMI_GUI_STYLE_TYPICAL
                MMIPUB_OpenAlertSuccessWin(TXT_BROWSER_SET_HOMEPAGE_SUCC);
#endif /* MMI_GUI_STYLE_TYPICAL */
            }
//            else
//            {
//                MMIPUB_OpenAlertWarningWin(TXT_BROWSER_SET_HOMEPAGE_FAIL);
//            }
            if(PNULL != url_str.wstr_ptr)
            {
                SCI_FREE(url_str.wstr_ptr);
            }
            MMK_CloseWin(win_id);
        }
            break;

#ifdef MMIDORADO_MY_NAV_SUPPORT
        case ID_POPUP_MENU_BOOKMARK_ADD2MYNAV:
        {
            MMI_STRING_T           url_str = {0};
            MMI_STRING_T           title_str = {0};
            
#ifndef MMIDORADO_BOOKMARK_BY_NV
            if ((FILEARRAY_TYPE_FILE == MMIBRW_GetBookmarkTitle(array_index, &title_str))
                && MMIBRW_GetBookmarkUrl(array_index, &url_str))
#else
            if (MMIBRW_GetBookmarkTitle(array_index, &title_str)
                && MMIBRW_GetBookmarkUrl(array_index, &url_str))
#endif
            {
                MMIBROWSER_Add2MyNav(title_str, url_str);
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_UNKNOWN_ERROR);
            }

#ifndef MMIDORADO_BOOKMARK_BY_NV
            if(PNULL != title_str.wstr_ptr)
            {
                SCI_FREE(title_str.wstr_ptr);
            }
#endif

            if(PNULL != url_str.wstr_ptr)
            {
                SCI_FREE(url_str.wstr_ptr);
            }
            
            MMK_CloseWin(win_id);
        }
            break;
#endif

#ifdef SNS_SUPPORT           
        case ID_POPUP_MENU_BRW_SHARE_TO_SINA:
        case ID_POPUP_MENU_BRW_SHARE_TO_FACEBOOK:
        case ID_POPUP_MENU_BRW_SHARE_TO_TWITTER:
        {
            MMI_STRING_T           url_str = {0};

            if (MMIBRW_GetBookmarkUrl(array_index,&url_str))
            {
                BrwHandleSnsOptMenu(&url_str, menu_id);
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_UNKNOWN_ERROR);
            }
                                  
            if(PNULL != url_str.wstr_ptr)
            {
                SCI_FREE(url_str.wstr_ptr);
            }    
            MMK_CloseWin(win_id);
        }
        break;
#endif

        default:
            break;
        }
    }
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
         MMK_CloseWin(win_id);
         break;
         
#ifdef SNS_SUPPORT           
    case MSG_SNS_OPERA_RESULT:
        {
            if(pMe)
            {
                IMMISNS_Release(pMe);
                pMe = PNULL;
            }
        }
        break;
#endif            

    default:
         result = MMI_RESULT_FALSE;
         break;
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : query is delete bookmark
//	Global resource dependence : none
//  Author: li.li
//	Note:
/*****************************************************************************/
LOCAL void BrwQueryIsDeleteBookmark(
                             MMI_CTRL_ID_T list_ctrl_id   //in
                             )
{
    uint16                      mark_num = 0;
    uint16                      item_index = 0;
    MMI_STRING_T                out_title = {0};
    MMI_STRING_T                dst_temp1 = {0};
    MMI_STRING_T                dst_temp2 = {0};
    MMI_WIN_ID_T query_win_id = MMIBROWSER_BOOKMARK_QUERY_WIN_ID;
#ifndef MMIDORADO_BOOKMARK_BY_NV
    MMIBRW_BOOKMARK_T *bookmark_info_ptr = MMIBRW_GetBookmark();
    uint16 array_index = 0;
    FILEARRAY_DATA_T f_array_data = {0};
#endif

    //get mark num
    mark_num = GUILIST_GetSelectedItemIndex(list_ctrl_id,PNULL,0);
#ifndef MMIDORADO_BOOKMARK_BY_NV
    s_bookmark_del_total_num = mark_num;
#endif

    if (0 == mark_num)
    {
        //ger current index
        item_index = GUILIST_GetCurItemIndex(list_ctrl_id);

        MMIBRW_GetBookmarkTitle(item_index,&out_title);
        if ((PNULL == out_title.wstr_ptr) || (0 == out_title.wstr_len))//error
        {
            MMIPUB_OpenAlertWarningWin(TXT_UNKNOWN_ERROR);
        }
        else
        {
#ifndef MMIDORADO_BOOKMARK_BY_NV
            array_index = BrwGetBookmarkArrayIndex(item_index);

            if (PNULL != bookmark_info_ptr)
            {
                MMIAPIFILEARRAY_Read(bookmark_info_ptr->file_array, array_index, &f_array_data);
            }

            if (FILEARRAY_TYPE_FOLDER == f_array_data.type)//文件夹
            {
                MMI_GetLabelTextByLang(TXT_BROWSER_BOOKMARK_DELETE_FOLDER, &dst_temp1);
            }
            else
#endif            
            {
                MMI_GetLabelTextByLang(TXT_BROWSER_DELETE_BOOKMARK, &dst_temp1);
            }

            dst_temp2.wstr_len = out_title.wstr_len;
            dst_temp2.wstr_ptr = out_title.wstr_ptr;
            MMIBROWSER_OpenQueryWinByTextPtr(&dst_temp1,&dst_temp2,IMAGE_PUBWIN_QUERY,&query_win_id,BrwHandleBookmarkDelQueryWinMsg);
        }

#ifndef MMIDORADO_BOOKMARK_BY_NV
        if(out_title.wstr_ptr != PNULL)
        {
            SCI_FREE(out_title.wstr_ptr);
        }
#endif        
    }
    else if (1 == mark_num)
    {
        GUILIST_GetSelectedItemIndex(list_ctrl_id,&item_index,1);
        MMIBRW_GetBookmarkTitle(item_index,&out_title);
        if ((PNULL == out_title.wstr_ptr) || (0 == out_title.wstr_len))//error
        {
            MMIPUB_OpenAlertWarningWin(TXT_UNKNOWN_ERROR);
        }
        else
        {
            MMI_GetLabelTextByLang(TXT_BROWSER_DELETE_BOOKMARK, &dst_temp1);
            dst_temp2.wstr_len = out_title.wstr_len;
            dst_temp2.wstr_ptr = out_title.wstr_ptr;
            MMIBROWSER_OpenQueryWinByTextPtr(&dst_temp1,&dst_temp2,IMAGE_PUBWIN_QUERY,&query_win_id,BrwHandleBookmarkDelQueryWinMsg);
        }
        
#ifndef MMIDORADO_BOOKMARK_BY_NV
        if(out_title.wstr_ptr != PNULL)
        {
            SCI_FREE(out_title.wstr_ptr);
        }
#endif
    }
    else
    {
#ifdef MMIDORADO_BOOKMARK_BY_NV
        char    num_temp[MMIBROWSER_NUM_TEMP_LEN + 1] = {0};
        wchar   *temp_wptr = PNULL;
        uint16  temp_wstr_len = 0;
        
        MMI_GetLabelTextByLang(TXT_BROWSER_DELETE_BY_NUM, &dst_temp1);
        CFL_Snprintf(num_temp,sizeof(num_temp),"%d ",mark_num);
        MMI_GetLabelTextByLang(TXT_BROWSER_DELETE_BOOKMARK_BY_NUM, &dst_temp2);

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
            //SCI_TRACE_LOW:"BrwQueryIsDeleteBookmark temp_wptr ALLOC ERROR!!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_BOOKMARK_2327_112_2_18_2_6_30_266,(uint8*)"");
        }
        
        MMIBROWSER_OpenQueryWinByTextPtr(&dst_temp1,&dst_temp2,IMAGE_PUBWIN_QUERY,&query_win_id,BrwHandleBookmarkDelQueryWinMsg);

        if (temp_wptr != PNULL)
        {
            SCI_FREE(temp_wptr);
        }
#else
        MMIBROWSER_OpenQueryWinByTextId(TXT_DELETE, IMAGE_PUBWIN_QUERY, &query_win_id,BrwHandleBookmarkDelQueryWinMsg);
#endif
    }
}

/*****************************************************************************/
//  Description : Delete bookmark(one bookmark, one bookmark dir or delete all)
//  Global resource dependence : none
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleBrowserDeleteBookmark(MMI_CTRL_ID_T ctrl_handle, BOOLEAN is_delete_all)
{
    uint16 cur_index = 0;
    BOOLEAN result = 0;

#ifndef MMIDORADO_BOOKMARK_BY_NV
    cur_index = BrwGetBookmarkCurIndex();//GetBookmarkCurIndex
#else
    cur_index = GUILIST_GetCurItemIndex(ctrl_handle);
#endif

    if (is_delete_all)
    {
        //需考虑删除中出现失败情况下,list的刷新
#ifndef MMIDORADO_BOOKMARK_BY_NV
        if (MMIBRW_BookmarkIsInRoot())
        {
            if(MMIBRW_RemoveAllBookmark())
            {
                GUILIST_RemoveAllItems(ctrl_handle);
                result = TRUE;
            }
        }
        else
        {
            if(MMIBRW_RemoveAllBookmarkOfSubFolder())
            {
                GUILIST_RemoveAllItems(ctrl_handle);
                result =TRUE;
            }
        }
#else
        if(MMIBRW_RemoveAllBookmark())
        {
            GUILIST_RemoveAllItems(ctrl_handle);
            result = TRUE;
        }
#endif
    }
    else
    {
        if(MMIBRW_RemoveBookmark(cur_index, FALSE))
        {
    		GUILIST_RemoveItem(ctrl_handle, cur_index);
        	result =TRUE;
        }
    }

    return result;
}

#ifndef MMIDORADO_BOOKMARK_BY_NV
/*****************************************************************************/
//  Description : handle delete selected bookmark confirm message
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:
/*****************************************************************************/
LOCAL void HandleDeleteBookmarkCnf(
                              BOOLEAN           is_success,     //in:delete is success
                              MMI_CTRL_ID_T     list_ctrl_id,   //in:
                              MMI_WIN_ID_T      wait_win_id,    //in:
                              MMI_WIN_ID_T      win_id          //in:bookmark main win id
                              )
{
    MMI_WIN_ID_T bookmark_pop_win_id = MMIBROWSER_BOOKMARK_POPMENU_WIN_ID;
    uint16 mark_num = 0;

    if (is_success)
    {
        if(s_file_del_bookmark_is_cancelled)
        {
            //stop delete
            BrwHandleBookmarkListUnmarkedAll(list_ctrl_id);
            MMK_CloseWin(bookmark_pop_win_id);
            MMK_CloseWin(wait_win_id);
        }
        else
        {
            //get mark num
            mark_num = GUILIST_GetSelectedItemIndex(list_ctrl_id,PNULL,0);
            if (0 == mark_num)
            {
                //set list not enable mark
#ifndef MMI_PDA_SUPPORT
                GUILIST_SetTextListMarkable(list_ctrl_id,FALSE);
#else                
                BrwExitBookmarkListMarked(list_ctrl_id);
#endif

#ifdef MMI_GUI_STYLE_TYPICAL
                BrwOpenAlertWinByTextId(PNULL, TXT_DELETED, TXT_NULL, IMAGE_PUBWIN_SUCCESS, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
#endif /* MMI_GUI_STYLE_TYPICAL */

                //close wait win
                MMK_CloseWin(wait_win_id);
                //close list option menu win
                MMK_CloseWin(bookmark_pop_win_id);
            }
            else
            {
                //delete the next bookmark
                BrwDeleteMarkedBookmarkItem(list_ctrl_id,wait_win_id,win_id);
            }
        }
    }
    else
    {
        //prompt fail
        BrwOpenAlertWinByTextId(PNULL,TXT_ERROR,TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);

        //close wait win
        MMK_CloseWin(wait_win_id);

        //close list option menu win
        MMK_CloseWin(bookmark_pop_win_id);
    }
}

/*****************************************************************************/
//  Description : delete selected pictures
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL void BrwDeleteMarkedBookmarkItem(
                             MMI_CTRL_ID_T      list_ctrl_id,   //in
                             MMI_WIN_ID_T       wait_win_id,    //in
                             MMI_WIN_ID_T       win_id          //in bookmark main win id
                             )
{
    uint16              mark_num = 0;
    uint16              item_index = 0;
    MMIBRW_BOOKMARK_T *bookmark_info_ptr = MMIBRW_GetBookmark();
    MMI_WIN_ID_T bookmark_pop_win_id = MMIBROWSER_BOOKMARK_POPMENU_WIN_ID;
    FILEARRAY_DATA_T f_array_data = {0};
    BOOLEAN             result    = FALSE;
    
    //get mark num
    mark_num = GUILIST_GetSelectedItemIndex(list_ctrl_id,PNULL,0);
    if (0 == mark_num)
    {
        //ger current index
        item_index = GUILIST_GetCurItemIndex(list_ctrl_id);
        if(MMIBRW_RemoveBookmark(item_index, FALSE))
        {
            GUILIST_RemoveItem(list_ctrl_id, item_index);
        }
        result = TRUE;
    }
    else
    {
        //get first mark items
        GUILIST_GetSelectedItemIndex(list_ctrl_id,&item_index,1);
    }
        
    if (MMIAPIFILEARRAY_Read(bookmark_info_ptr->file_array, item_index, &f_array_data))
    {
        /*文件和文件夹使用同一个删除函数，在里面区分*/
        if(MMIBRW_RemoveBookmark(item_index, FALSE))
        {
            GUILIST_RemoveItem(list_ctrl_id, item_index);
            result = TRUE;
            MMK_PostMsg(win_id, MSG_BRW_DELETE_BOOKMARK, &result, sizeof(result));
        }
        else
        {
            //SCI_TRACE_LOW:"BrwDeleteMarkedBookmarkItem : MMIBRW_RemoveBookmark error;item_index = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_BOOKMARK_2503_112_2_18_2_6_30_267,(uint8*)"d",item_index);
            result = FALSE;
            return;
        }
    }

    if ( !result )
    {
        //prompt fail
        BrwOpenAlertWinByTextId(PNULL,TXT_ERROR,TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        
        //close wait win
        MMK_CloseWin(wait_win_id);

        //close list option menu win
        MMK_CloseWin(bookmark_pop_win_id);
    }

    return;
}
/*****************************************************************************/
//  Description : to handle the message of progress window    
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleProcessWinMsg(
                                        MMI_WIN_ID_T     win_id, 
                                        MMI_MESSAGE_ID_E    msg_id, 
                                        DPARAM              param)
{
    MMI_RESULT_E result  = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIPUB_SetWinSoftkey(win_id, TXT_NULL, STXT_CANCEL, FALSE); 
        result = MMIPUB_HandleProgressWinMsg(win_id,msg_id,param);
        break;

    //case MSG_APP_RED:   
    case MSG_APP_CANCEL: 
        s_file_del_bookmark_is_cancelled = TRUE;
        //MMK_CloseWin(win_id);
        break;

    default:
        result = MMIPUB_HandleProgressWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}
#endif

/*****************************************************************************/
// 	Description : handle bookmark list mark option
//	Global resource dependence : none
//  Author: li.li
//	Note:
/*****************************************************************************/
LOCAL void BrwHandleBookmarkListMarked(
                               MMI_CTRL_ID_T  list_ctrl_id
                               )
{
    uint16      cur_index = 0;
    uint32      list_num = GUILIST_GetTotalItemNum(list_ctrl_id);
    uint16      mark_num =0;
    
    mark_num = GUILIST_GetSelectedItemIndex(list_ctrl_id,PNULL,0);//get mark num
    if (0 == mark_num)
    {
        GUILIST_SetTextListMarkable(list_ctrl_id,TRUE);//set list enable mark
        GUILIST_SetMaxSelectedItem(list_ctrl_id,(uint16)list_num);//set mark max number
    }
    
    //mark current item index
    cur_index = GUILIST_GetCurItemIndex(list_ctrl_id);
    GUILIST_SetSelectedItem(list_ctrl_id,cur_index,TRUE);
}

/*****************************************************************************/
// 	Description : handle bookmark list mark cancel option
//	Global resource dependence : none
//  Author: li.li
//	Note:
/*****************************************************************************/
LOCAL void BrwHandleBookmarkListUnmarked(
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
// 	Description : handle bookmark list mark all option
//	Global resource dependence : none
//  Author: li.li
//	Note:
/*****************************************************************************/
LOCAL void BrwHandleBookmarkListMarkedAll(
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
// 	Description : handle bookmark list mark all cancel option
//	Global resource dependence : none
//  Author: li.li
//	Note:
/*****************************************************************************/
LOCAL void BrwHandleBookmarkListUnmarkedAll(
                                    MMI_CTRL_ID_T list_ctrl_id
                                    )
{
    GUILIST_SetAllSelected(list_ctrl_id,FALSE);

    //set list enable mark
    GUILIST_SetTextListMarkable(list_ctrl_id,FALSE);
}
/*****************************************************************************/
//  Description : handle bookmark detail window message
//  Global resource dependence : 
//  Author:dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleBookmarkDetailWinMsg(
                                           MMI_WIN_ID_T	win_id, 
                                           MMI_MESSAGE_ID_E	msg_id, 
                                           DPARAM param
                                           )
{
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    //MMIBRW_BOOKMARK_T *bookmark_info_ptr = MMIBRW_GetBookmark();
    uint32 array_index = (uint32)MMK_GetWinAddDataPtr(win_id);
   
    switch(msg_id) 
    {
        case MSG_OPEN_WINDOW:
#ifdef MMI_PDA_SUPPORT
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
            MMK_SetAtvCtrl(win_id,MMIBROWSER_BOOKMARK_DETAIL_CTRL_ID);
            BrwSetBookmarkDetailWindow(MMIBROWSER_BOOKMARK_DETAIL_CTRL_ID,(uint16)array_index);
            GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION,TXT_COMM_OPEN, STXT_RETURN, FALSE );
            break;
        
        case MSG_CTL_OK:
        case MSG_APP_OK:
    	case MSG_APP_MENU:
#ifndef MMIDORADO_BOOKMARK_BY_NV
       {
            //FILEARRAY_DATA_T f_array_data = {0};
            
            SCI_MEMSET(&s_bookmark_cur_index_info, 0x00, sizeof(s_bookmark_cur_index_info));
            
            BrwSetBookmarkCurIndex(array_index);
#if 0//useless
            if ((1 == bookmark_info_ptr->file_array_step))
            {
                //do nothing
            }
            else
            {
                if(MMIAPIFILEARRAY_Read(bookmark_info_ptr->file_array, array_index, &f_array_data))
                {
                    s_bookmark_cur_index_info.type = f_array_data.type;
                }
                else//error
                {
                    s_bookmark_cur_index_info.type = FILEARRAY_TYPE_NULL;
                }
            }
#endif                

            MMK_CreateWin((uint32*)MMIBROWSER_BOOKMARK_DETAIL_POPMENU_WIN_TAB,(ADD_DATA)array_index);    
        }
#else
        {
            MMK_CreateWin((uint32*)MMIBROWSER_BOOKMARK_DETAIL_POPMENU_WIN_TAB,(ADD_DATA)array_index);    
        }
#endif
            break;
            
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
            if (MMIBRW_BookmarkGotoURL((uint16)array_index))
            {
                MMK_CloseWin(win_id);
                BookmarkTidyExistedBkWin();
            }
            else
            {
                //do nothing
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
//  Description : set the bookmark detail window
//  Global resource dependence : 
//  Author: dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL void BrwSetBookmarkDetailWindow(MMI_CTRL_ID_T ctrl_id,uint16 list_index)
{
    GUIRICHTEXT_ITEM_T  item_data = {0};
    uint16              index = 0;
    MMI_STRING_T url_text_str ={0};
    MMI_STRING_T title_text_str = {0};

    MMIBRW_GetBookmarkTitle(list_index, &title_text_str);
    MMIBRW_GetBookmarkUrl(list_index, &url_text_str);
    
    item_data.text_type = GUIRICHTEXT_TEXT_RES;
    item_data.text_data.res.id = TXT_COMM_TITLE;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

    SCI_MEMSET(&item_data, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
    if ((PNULL != title_text_str.wstr_ptr) && (title_text_str.wstr_len > 0))/*lint !e774*/
    {
        item_data.text_type = GUIRICHTEXT_TEXT_BUF;
        item_data.text_data.buf.len = MIN(title_text_str.wstr_len, GUIRICHTEXT_TEXT_MAX_LEN);
        item_data.text_data.buf.str_ptr = title_text_str.wstr_ptr;
    }
    else
    {
        item_data.text_type = GUIRICHTEXT_TEXT_RES;
        item_data.text_data.res.id = TXT_NO_TITLE;
    }
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
    
    SCI_MEMSET(&item_data, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
    item_data.text_type = GUIRICHTEXT_TEXT_RES;
    item_data.text_data.res.id = TXT_INPUT_URL_ADRESS;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
    
    SCI_MEMSET(&item_data, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;
    if ((PNULL != url_text_str.wstr_ptr) && (url_text_str.wstr_len > 0))/*lint !e774*/
    {
        item_data.text_data.buf.len = url_text_str.wstr_len;
        item_data.text_data.buf.str_ptr =  url_text_str.wstr_ptr;
    }
    else
    {
        item_data.text_data.buf.len = 1;
        item_data.text_data.buf.str_ptr = L" ";
    }
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
    
#ifndef MMIDORADO_BOOKMARK_BY_NV
    if(PNULL !=title_text_str.wstr_ptr)
    {
        SCI_FREE(title_text_str.wstr_ptr);
    }
#endif

    if(PNULL !=url_text_str.wstr_ptr)
    {
        SCI_FREE(url_text_str.wstr_ptr);
    }

}

/*****************************************************************************/
//  Description : handle bookmark detail popmenu window message
//  Global resource dependence : 
//  Author: dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleBookmarkDetailPopMenuWinMsg(MMI_WIN_ID_T win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param)
{
    MMI_MENU_GROUP_ID_T         group_id = 0;
    MMI_MENU_ID_T               menu_id = 0;
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               ctrl_id = MMIBROWSER_BOOKMARK_DETAIL_OPTION_CTRL_ID;
    uint16  array_index = (uint32)MMK_GetWinAddDataPtr(win_id);/*lint !e64*/
    //MMI_CTRL_ID_T               list_ctrl_id = MMIBROWSER_LIST_BOOKMARK_CTRL_ID;
    //uint16                      list_index = 0;
     
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIMENU_GetId(ctrl_id,&group_id,&menu_id);
#ifndef MMIDORADO_BOOKMARK_BY_NV
        if(0 == MMIBRW_BookmarkGetFolderNum((FALSE)))//MS00248666
        {
            GUIMENU_SetItemGrayed(ctrl_id, group_id, ID_POPUP_MENU_BOOKMARK_MOVE_TO, TRUE);
        }
#endif       

        MMK_SetAtvCtrl(win_id, ctrl_id);						
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
        GUIMENU_GetId(ctrl_id,&group_id,&menu_id);
        switch(menu_id) 
        {            
        case ID_POPUP_MENU_BOOKMARK_EDIT:
            MMIBROWSER_OpenBookmarkEditWin(MMIBRW_BM_CHANGE_EDIT);
            MMK_CloseWin(win_id);
            break;
         
#ifndef MMIDORADO_BOOKMARK_BY_NV
        case ID_POPUP_MENU_BOOKMARK_MOVE_TO:
            BrwBookmarkEnterMoveState(win_id, array_index);  //add_array_data_ptr = array_index
            if(MMK_IsOpenWin(MMIBROWSER_BOOKMARK_DETAIL_WIN_ID))
            {
                MMK_CloseWin(MMIBROWSER_BOOKMARK_DETAIL_WIN_ID);
            }
            MMK_CloseWin(win_id);
            break;
#endif
            
        case ID_POPUP_MENU_BOOKMARK_SEND_URL:
            {
                MMI_STRING_T    title_str = {0};
                MMI_STRING_T    url_str = {0};
                
#ifndef MMIDORADO_BOOKMARK_BY_NV
               if ((FILEARRAY_TYPE_FILE == MMIBRW_GetBookmarkTitle(array_index, &title_str))
                    && MMIBRW_GetBookmarkUrl(array_index, &url_str))
#else
               if ( MMIBRW_GetBookmarkTitle(array_index, &title_str)
                    && MMIBRW_GetBookmarkUrl(array_index, &url_str))
#endif
                {
                    if(MMIBROWSER_SendUrlContentBySms(title_str.wstr_ptr, title_str.wstr_len, url_str.wstr_ptr, url_str.wstr_len))
                    {
                        //successful
                    }
                    else
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_UNKNOWN_ERROR);
                    }
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_UNKNOWN_ERROR);
                }
                
#ifndef MMIDORADO_BOOKMARK_BY_NV
                if(PNULL !=title_str.wstr_ptr)
                {
                    SCI_FREE(title_str.wstr_ptr);
                }
#endif          
                if(PNULL !=url_str.wstr_ptr)
                {
                    SCI_FREE(url_str.wstr_ptr);
                }
                
                MMK_CloseWin(win_id);
            }
            break;

        case ID_POPUP_MENU_BOOKMARK_OPEN:
#ifndef MMIDORADO_BOOKMARK_BY_NV
        {
            FILEARRAY_DATA_T f_array_data = {0};
            MMIBRW_BOOKMARK_T *bookmark_info_ptr = MMIBRW_GetBookmark();
      
            MMIAPIFILEARRAY_Read(bookmark_info_ptr->file_array, array_index, &f_array_data);
            
            if (FILEARRAY_TYPE_FOLDER == f_array_data.type)/*打开文件夹*/
            {
                BrwOpenBookmarkStepWin(BOOKMARK_STEP_TYPE_NEXT, array_index, FUNC_FIND_NULL);
            }
            else  if (FILEARRAY_TYPE_FILE == f_array_data.type)/*选中当前的书签进行链接*/
            {
                if (MMIBRW_BookmarkGotoURL(array_index))
                {
                    MMK_CloseWin(win_id);
                    BookmarkTidyExistedBkWin();
                }
            }
        }           
#else
            if (MMIBRW_BookmarkGotoURL(array_index))
            {
                MMK_CloseWin(win_id);
                BookmarkTidyExistedBkWin();
            }
#endif
             break;
            
        default:
            break;
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

#ifndef MMIDORADO_BOOKMARK_BY_NV
/*****************************************************************************/
//  Description : handle move bookmark waiting win msg
//  Global resource dependence : none
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBookmarkMoveWaitWinMsg(
                                       MMI_WIN_ID_T        win_id,   //in
                                       MMI_MESSAGE_ID_E    msg_id,  //in  
                                       DPARAM              param    //in
                                       )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;

    switch( msg_id)
    {
    case MSG_PIC_COPY_FILE:
        {
            uint16 folder_index = BrwGetBookmarkArrayIndex(BrwGetPathFolderIndex());
            MMI_STRING_T   bookmark_string_moved_to ={0};
            MMI_STRING_T   bookmark_string_folder ={0};
            BOOLEAN  copy_result = FALSE;
            BOOLEAN is_need_free =  FALSE;

            copy_result=*(BOOLEAN *)param;

            //SCI_TRACE_LOW:"HandleBookmarkMoveWaitWinMsg  copy_result=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_BOOKMARK_2929_112_2_18_2_6_31_268,(uint8*)"d",copy_result);
        
            s_is_in_bookmark_moving = FALSE;
            
            //get the path folder info
            MMI_GetLabelTextByLang(TXT_BROWSER_BOOKMARK_BE_MOVEDTO, &bookmark_string_moved_to);
            if(0 == BrwGetPathFolderIndex())
            {
                MMI_GetLabelTextByLang(TXT_BROWSER_ROOT_FOLDER_ROUTE, &bookmark_string_folder);
            }
            else
            {   
                is_need_free = TRUE;
                MMIBRW_BookmarkGetFolderName(PNULL, folder_index, &bookmark_string_folder);//string free outside
            }
            
            //second:delete the old bookmark file
            if (copy_result && MMIAPIFMM_DeleteFileSyn(s_bookmark_move_array_data.filename, s_bookmark_move_array_data.name_len))
            {
#ifdef MMI_GUI_STYLE_TYPICAL
                wchar           *wchar_ptr = PNULL;
                MMI_STRING_T    info_str = {0};

                info_str.wstr_len = bookmark_string_moved_to.wstr_len + bookmark_string_folder.wstr_len + 1; //Add " "

                if (PNULL != (wchar_ptr = SCI_ALLOCA((info_str.wstr_len + 1) * sizeof(wchar))))
                {
                    info_str.wstr_ptr = wchar_ptr;

                    SCI_MEMSET(wchar_ptr, 0, info_str.wstr_len + 1);

                    MMIAPICOM_Wstrncpy(wchar_ptr, bookmark_string_moved_to.wstr_ptr, bookmark_string_moved_to.wstr_len);

                    wchar_ptr += bookmark_string_moved_to.wstr_len;

                    *wchar_ptr = 0x20;

                    wchar_ptr++;

                    MMIAPICOM_Wstrncpy(wchar_ptr, bookmark_string_folder.wstr_ptr, bookmark_string_folder.wstr_len);

                    BrwOpenAlertWinByTextPtr(PNULL, &info_str, PNULL, IMAGE_PUBWIN_SUCCESS, PNULL, MMIPUB_SOFTKEY_ONE, PNULL );

                    SCI_FREE(info_str.wstr_ptr);
                }
#endif /* MMI_GUI_STYLE_TYPICAL */
            }
            else
            {
                BrwOpenAlertWinByTextId(PNULL,TXT_ERROR,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);               
            }    
            
            if (is_need_free && (PNULL != bookmark_string_folder.wstr_ptr))
            {
                SCI_FREE(bookmark_string_folder.wstr_ptr);
            }
            
            //reset(success or fail): go back to root path
            BrwPrepare2BackToRoot();

            MMK_CloseWin(win_id);//close waiting window(success or fail)
        }
        break;
            
    case MSG_APP_CANCEL:
#ifndef MMI_PDA_SUPPORT
    case MSG_APP_RED:
#else
    case MSG_KEYUP_RED:
#endif
        break;
        
    default:
       result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : move and replace specified bookmark
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIBRW_HandleMoveBookmark(MMI_WIN_ID_T      win_id, 
                                          MMI_MESSAGE_ID_E  msg_id, 
                                          DPARAM            param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"MMIBRW_HandleMoveBookmark() msg_id=0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_BOOKMARK_2997_112_2_18_2_6_31_269,(uint8*)"d", msg_id);

    switch(msg_id)
    {
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
        {
            MMIPUB_INFO_T *win_info_ptr = PNULL;
            wchar *full_file_name_ptr = PNULL;
            MMIBRW_BOOKMARK_T *bookmark_info_ptr = MMIBRW_GetBookmark();
            BOOLEAN moving_result = FALSE;
          
            win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
            if (PNULL != win_info_ptr)
            {
                full_file_name_ptr = (wchar *)win_info_ptr->user_data;
            }
            
            //先删除存在的书签
            if (SFS_NO_ERROR == MMIAPIFMM_DeleteFile((const wchar *)full_file_name_ptr,  PNULL))
            {
                if (bookmark_info_ptr->total > 0)
                {
                    bookmark_info_ptr->total--;
                }

                if (BrwBookmarkMoveTo(full_file_name_ptr, MMIAPICOM_Wstrlen(full_file_name_ptr)))
                {
                    moving_result = TRUE;
                    //do nothing, waiting for asynchronous
                }
                else//error
                {
                    moving_result = FALSE;
                }
            }
            else
            {
               //SCI_TRACE_LOW:"[BRW]W MMIBRW_HandleMoveBookmark() MMIAPIFMM_DeleteFile FAIL"
               SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_BOOKMARK_3038_112_2_18_2_6_31_270,(uint8*)"");
            }

            if (!moving_result)
            {
                s_is_in_bookmark_moving = FALSE;
                
                BrwPrepare2BackToRoot();//reset(success or fail): go back to root path

                BrwOpenAlertWinByTextId(PNULL,TXT_ERROR,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);               
            }
            
            MMK_CloseWin(win_id);
        }
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        s_is_in_bookmark_moving = FALSE;
        BrwPrepare2BackToRoot();//reset(success or fail): go back to root path
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        {
            MMIPUB_INFO_T *win_info_ptr = PNULL;
            wchar *full_file_name_ptr = PNULL;

            win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
            if (PNULL != win_info_ptr)
            {
                full_file_name_ptr = (wchar *)win_info_ptr->user_data;
                SCI_FREE(full_file_name_ptr);
            }
        }
        break;

    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }
    
    return recode;
}


/*****************************************************************************/
//  Description : move one bookmark
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN  BrwBookmarkMoveTo(
                           const wchar *full_path_ptr,     //in
                           uint16     full_path_len       //in, 双字节为单位
                           )
{
    BOOLEAN result = TRUE; 
    uint32 time_out = 0;
    MMI_WIN_ID_T    wait_win_id = MMIBROWSER_BOOKMARK_MOVE_WAITING_WIN_ID;

    if ((PNULL == full_path_ptr) || (0 == full_path_len))
    {
        //SCI_TRACE_LOW:"[BRW]W BrwBookmarkMoveTo() PARAM ERROR"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_BOOKMARK_3099_112_2_18_2_6_31_271,(uint8*)"");
        return FALSE;
    }
    
    MMIPUB_OpenAlertWinByTextId(&time_out,
                    TXT_COMMON_WAITING,
                    TXT_NULL,
                    IMAGE_PUBWIN_WAIT,
                    &wait_win_id,
                    PNULL,
                    MMIPUB_SOFTKEY_NONE,
                    HandleBookmarkMoveWaitWinMsg
                    );
     
    result = MMIAPIFMM_CopyOneFile(s_bookmark_move_array_data.filename, s_bookmark_move_array_data.name_len, 
                        full_path_ptr, full_path_len, wait_win_id, MSG_PIC_COPY_FILE);

    //如果失败，直接关闭等待窗口
    if(!result)        
    {
        MMIAPIFMM_DeleteFile(full_path_ptr, PNULL);//protect
        MMK_CloseWin(wait_win_id);
        s_is_in_bookmark_moving = FALSE;
    } 
    
    //SCI_TRACE_LOW:"[BRW]W BrwBookmarkMoveTo() result=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_BOOKMARK_3124_112_2_18_2_6_31_272,(uint8*)"d", result);
        
    return result; 
}

/*****************************************************************************/
//  Description : begin to move one bookmark
//  Global resource dependence : none
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL MMIBROWSER_BK_MOVE_RESULT_E BrwBookmarkBeginMoveTo(uint16 cur_index)
{
    MMIBRW_BOOKMARK_T *bookmark_info_ptr = MMIBRW_GetBookmark();
    uint16 file_array_index = 0;
    FILEARRAY_DATA_T f_array_data = {0};
    wchar   file_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16  file_name_len = MMIFILE_FULL_PATH_MAX_LEN;
    wchar   path_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16  path_name_len = MMIFILE_FULL_PATH_MAX_LEN;
    wchar   full_file_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16  full_file_name_len = MMIFILE_FULL_PATH_MAX_LEN;
    MMI_STRING_T string_1 ={0};
    int32     duplicate_index = 0;
    MMI_WIN_ID_T query_win_id = MMIBROWSER_BOOKMARK_QUERY_WIN_ID;

    /*remember current folder index for menu entry moveto state*/
    BrwSetPathFolderIndex(cur_index);
    //get new path
    if (0 == cur_index)//means:move to root
    {
        //BrwCombineFullPath((char *)MMIBRW_BOOKMARK_FILE_DIR, path_name, &path_name_len);//set path name as the "root"
        BrwGetDefaultRootPath(path_name, &path_name_len);
        string_1.wstr_ptr = path_name;
        string_1.wstr_len = path_name_len;
    }
    else
    {
        file_array_index = BrwGetBookmarkArrayIndex(cur_index);
        MMIAPIFILEARRAY_Read(bookmark_info_ptr->file_array, file_array_index, &f_array_data);
        string_1.wstr_ptr = f_array_data.filename;
        string_1.wstr_len = f_array_data.name_len;
    }        

    //get the file name(without path and suffix)    
    MMIAPIFMM_SplitFullPathExt(s_bookmark_move_array_data.filename, s_bookmark_move_array_data.name_len, 
                        PNULL,PNULL,
                        file_name, &file_name_len,
                        PNULL,PNULL);

    //combin new file full path name
    MMIBRW_BookmarkCombinePath(string_1.wstr_ptr, string_1.wstr_len, 
                    file_name, file_name_len, 
                    MMIBRW_GetBkSuffixName(), MMIAPICOM_Wstrlen(MMIBRW_GetBkSuffixName()), 
                    full_file_name, &full_file_name_len);
    
    duplicate_index =MMIBRW_FindDuplicatedBookmarkIndex(string_1.wstr_ptr, string_1.wstr_len, bookmark_info_ptr->file_array_step, file_name);

    if (0 == full_file_name_len)//error
    {
        //SCI_TRACE_LOW:"[BRW]W BrwBookmarkBeginMoveTo 1"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_BOOKMARK_3182_112_2_18_2_6_31_273,(uint8*)"");
        return MMIBROWSER_BK_MOVE_RESULT_ERROR;
    }
    else if (0 == MMIAPICOM_Wstrcmp(s_bookmark_move_array_data.filename, full_file_name))//same path
    {
        //SCI_TRACE_LOW:"[BRW]W BrwBookmarkBeginMoveTo 2"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_BOOKMARK_3187_112_2_18_2_6_31_274,(uint8*)"");
        return MMIBROWSER_BK_MOVE_RESULT_SAME_PATH;
    }
    else if (0 <= duplicate_index)//暂不支持替换
    {
        wchar * move_file_name = PNULL;

        move_file_name = SCI_ALLOCA((full_file_name_len + 1) * sizeof(wchar));
        if (PNULL == move_file_name)
        {
            //SCI_TRACE_LOW:"[BRW]W BrwBookmarkBeginMoveTo 3"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_BOOKMARK_3197_112_2_18_2_6_32_275,(uint8*)"");
            return MMIBROWSER_BK_MOVE_RESULT_ERROR;
        }

        SCI_MEMSET(move_file_name, 0, (full_file_name_len + 1) * sizeof(wchar));
        SCI_MEMCPY(move_file_name, full_file_name, full_file_name_len * sizeof(wchar));
        MMIPUB_OpenQueryWinByTextIdEx(VIRTUAL_WIN_ID, 
                TXT_WWW_BOOKMARK_QUERY_REPLACE, 
                IMAGE_PUBWIN_QUERY, 
                &query_win_id, 
                MMIBRW_HandleMoveBookmark, 
                (uint32)move_file_name);
        //SCI_TRACE_LOW:"[BRW]W BrwBookmarkBeginMoveTo 4"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_BOOKMARK_3209_112_2_18_2_6_32_276,(uint8*)"");
    }
    else//normal,begin to move
    {     
        if (BrwBookmarkMoveTo(full_file_name, full_file_name_len))
        {
            //do nothing, waiting for asynchronous
        }
        else//error
        {
            //SCI_TRACE_LOW:"[BRW]W BrwBookmarkBeginMoveTo 5"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_BOOKMARK_3219_112_2_18_2_6_32_277,(uint8*)"");
            return MMIBROWSER_BK_MOVE_RESULT_ERROR;
        }
    }

    return MMIBROWSER_BK_MOVE_RESULT_SUCCESS;
}

/*****************************************************************************/
//  Description :enter the bookmark moving state and back to bookmark list window to choose
//  Global resource dependence : none
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL void BrwBookmarkEnterMoveState(MMI_WIN_ID_T win_id, uint16 array_index)
{
    MMIBRW_BOOKMARK_T *bookmark_info_ptr = MMIBRW_GetBookmark();
    MMI_HANDLE_T bk_list_ctrl_handle = MMIBROWSER_LIST_BOOKMARK_CTRL_ID;
    MMI_HANDLE_T bk_list_win_handle = MMIBROWSER_BOOKMARK_WIN_ID;

    SCI_MEMSET(&s_bookmark_move_array_data, 0x00, sizeof(s_bookmark_move_array_data));
    if(MMIAPIFILEARRAY_Read(bookmark_info_ptr->file_array, array_index, &s_bookmark_move_array_data))
    {
        s_is_in_bookmark_moving = TRUE;
        //MMK_CloseWin(win_id);

        //back to bookmark list window to choose,update the list for choose
        if (MMIBRW_LoadBookmarkFileArray(BOOKMARK_STEP_TYPE_ROOT, 0, FUNC_FIND_FOLDER, 0, 0))
        {
            bookmark_info_ptr->cur_index_num++;//增加了"/" 根目录的显示
            GUILIST_SetCurItemIndex(bk_list_ctrl_handle, 0);
            MMIBRW_AppendListboxitem(bk_list_win_handle, bk_list_ctrl_handle, (uint16)bookmark_info_ptr->cur_index_num);
        }
        else//error,do nothing
        {
            s_is_in_bookmark_moving = FALSE;
            SCI_MEMSET(&s_bookmark_move_array_data, 0x00, sizeof(s_bookmark_move_array_data));
        }
    }
}

/*****************************************************************************/
// Description : get the bookmark filearray index that need to be handled
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
LOCAL uint16 BrwGetBookmarkCurIndex(void)
{
    SCI_TRACE_LOW("BrwGetBookmarkCurIndex array_index = %d", s_bookmark_cur_index_info.array_index);
    return s_bookmark_cur_index_info.array_index;
}

/*****************************************************************************/
// Description : get the bookmark filearray index that need to be handled
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
LOCAL void BrwSetBookmarkCurIndex(uint16 current_index)
{
    SCI_TRACE_LOW("BrwSetBookmarkCurIndex array_index = %d", current_index);

    s_bookmark_cur_index_info.array_index = current_index;
}

/*****************************************************************************/
// Description : Handle replace the duplicated bookmark 
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIBRW_HandleReplaceDuplicatedBookmark(MMI_WIN_ID_T      win_id, 
                                          MMI_MESSAGE_ID_E  msg_id, 
                                          DPARAM            param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_APP_WEB:
    case MSG_APP_OK:
        {
            MMI_STRING_T url_text_str = {0};
            uint16 current_index = 0;
            MMI_HANDLE_T input_win_handle = MMIBROWSER_BOOKMARK_EDIT_WIN_ID;  
            MMI_HANDLE_T bm_win_handle = MMIBROWSER_BOOKMARK_WIN_ID; 
            MMI_HANDLE_T bm_ctrl_handle = MMIBROWSER_LIST_BOOKMARK_CTRL_ID;
            uint32 folder_index = 0;
            wchar full_path_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
            uint16 full_path_name_len = MMIFILE_FULL_PATH_MAX_LEN;
            uint32 file_array_step = 0;
            MMI_STRING_T bookmark_fodler_name ={0};
            MMIPUB_INFO_T *win_info_ptr = PNULL;
            uint16 new_index = 0;
            BOOLEAN is_save_success = FALSE;
            BOOLEAN is_save_step_success = FALSE;

            win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
            if (PNULL != win_info_ptr)
            {
                current_index = (uint16)win_info_ptr->user_data;//get the index that need to be replaced
            }

            //get the replaced path
            folder_index = BrwGetPathFolderIndex();
            if (0 == folder_index)
            {
                BrwGetDefaultRootPath(full_path_name, &full_path_name_len);
                file_array_step = 0;
            }
            else
            {
                MMIBRW_BookmarkGetFolder(folder_index - 1, full_path_name, &full_path_name_len, PNULL);
                file_array_step = 1;
            }

            //load the replaced path's file array
            if (0 != folder_index)
            {
                is_save_step_success = MMIBRW_LoadBookmarkFileArray(BOOKMARK_STEP_TYPE_ROOT, 0, FUNC_FIND_NULL, 0, 0);
                if (is_save_step_success)
                {
                    is_save_step_success = MMIBRW_LoadBookmarkFileArray(BOOKMARK_STEP_TYPE_NEXT, folder_index - 1, FUNC_FIND_NULL, 0, 0);
                }
            }
            else
            {
                is_save_step_success = MMIBRW_LoadBookmarkFileArray(BOOKMARK_STEP_TYPE_ROOT, 0, FUNC_FIND_NULL, 0, 0);
            }

#if 0//title is same, don't need to change
            {
                FILEARRAY_DATA_T array_item = {0};
                MMIBRW_BOOKMARK_T *bookmark_info_ptr = MMIBRW_GetBookmark();
                MMI_STRING_T title_text_str = {0};

                GUIEDIT_GetString(MMIBROWSER_BOOKMARK_EDIT_TITLE_CTRL_ID, &title_text_str);
                if (MMIAPIFILEARRAY_Read(bookmark_info_ptr->file_array, (uint32)current_index, &array_item ))
                {
                    if ((0 != title_text_str.wstr_ptr) && (0 != MMIAPICOM_Wstrcmp(title_text_str.wstr_ptr,array_item.filename)))
                    {
                        MMIBRW_ChangeBookmarkTitle(current_index, title_text_str, PNULL);
                    }
                }
            }
#endif            
            if (is_save_step_success)
            {
                //update the replaced bookmark url info
                GUIEDIT_GetString(MMIBROWSER_BOOKMARK_EDIT_URL_CTRL_ID, &url_text_str);
                if (MMK_IsOpenWin(bm_win_handle))
                {
                    is_save_success = MMIBRW_ChangeBookmarkUrl(current_index, url_text_str, &new_index); /* Use the new index */
                }
                else
                {
                    is_save_success = MMIBRW_ChangeBookmarkUrl(current_index, url_text_str, PNULL); 
                }
            }
            else
            {
                is_save_success = FALSE;
            }

            if (is_save_success)
            {
                if (MMK_IsOpenWin(bm_win_handle))
                {
                    MMI_STRING_T    str_text = {0};
                    if (0 != folder_index)
                    {
                        MMIBRW_BookmarkGetFolderName(PNULL, folder_index - 1, &bookmark_fodler_name);
                        //GUIWIN_SetTitleText(bm_win_handle, bookmark_fodler_name.wstr_ptr, bookmark_fodler_name.wstr_len, FALSE);
                        str_text = bookmark_fodler_name;
                    }
                    else
                    {
                        //GUIWIN_SetTitleTextId(bm_win_handle, TXT_BOOKMARK, FALSE);
                        MMI_GetLabelTextByLang(TXT_BOOKMARK, &str_text);
                    }
#ifndef BROWSER_BOOKMARK_HISTORY_TAB					
			GUIWIN_SetTitleText(bm_win_handle, str_text.wstr_ptr, str_text.wstr_len, FALSE);
#else					
			GUITAB_SetSimpleItem(
                            MMIBROWSER_BK_AND_HISTORY_TAB_CTRL_ID,
                            &str_text,
                            IMAGE_BROWSER_BOOKMARK_LIST,
                            IMAGE_BROWSER_BOOKMARK_LIST,
                            MMIBROWSER_TAB1_BOOKMARK
                            );
#endif
                    MMIBRW_AppendListboxitem(bm_win_handle, MMIBROWSER_LIST_BOOKMARK_CTRL_ID, (uint16)MMIBRW_GetBookmarkNumber());
                    GUILIST_SetCurItemIndex(bm_ctrl_handle, new_index);
                }
                
#ifdef MMI_GUI_STYLE_TYPICAL
                BrwOpenAlertWinByTextId(PNULL,TXT_COMPLETE,TXT_NULL,IMAGE_PUBWIN_SUCCESS,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
#endif /* MMI_GUI_STYLE_TYPICAL */
                MMK_CloseWin(win_id);
                MMK_CloseWin(MMIBROWSER_BOOKMARK_SAVE_WAITING_WIN_ID);
                MMK_CloseWin(input_win_handle);
            }
            else//fail
            {
                MMIPUB_OpenAlertFailWin(TXT_MMI_SAVEFAILED);
                MMK_CloseMiddleWin(MMIBROWSER_BOOKMARK_START_WIN_ID, MMIBROWSER_BOOKMARK_END_WIN_ID);//exit the bookmark 
            }
        }
        break;

    case MSG_APP_CANCEL:
        //cancel the replace and return to the edit window
        MMK_CloseWin(win_id);
        MMK_CloseWin(MMIBROWSER_BOOKMARK_SAVE_WAITING_WIN_ID);
        break;

    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : Wstricmp
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC int MMIBRW_Wstricmp(const wchar * string1, const wchar * string2)
{
#define MMIBRW_ERR_BAD_PARAM    (-2)
    wchar * str1_temp = PNULL;
    wchar * str2_temp = PNULL;
    uint16 str1_len = 0;
    uint16 str2_len = 0;
    int ret = 0;


    if (PNULL == string1 || PNULL == string2)
    {
        return MMIBRW_ERR_BAD_PARAM;
    }

    str1_len = MMIAPICOM_Wstrlen(string1) + 1;
    str2_len = MMIAPICOM_Wstrlen(string2) + 1;

    str1_temp = SCI_ALLOCA(str1_len * sizeof(wchar));
    if (PNULL == str1_temp)
    {
        return MMIBRW_ERR_BAD_PARAM;
    }

    str2_temp = SCI_ALLOCA(str2_len * sizeof(wchar));
    if (PNULL == str2_temp)
    {
        SCI_FREE(str1_temp);
        return MMIBRW_ERR_BAD_PARAM;
    }

    SCI_MEMSET(str1_temp, 0, sizeof(wchar) * str1_len);
    SCI_MEMSET(str2_temp, 0, sizeof(wchar) * str2_len);
    MMIAPICOM_Wstrncpy(str1_temp, string1, str1_len); //Including '\0'
    MMIAPICOM_Wstrncpy(str2_temp, string2, str2_len);
    MMIAPICOM_Wstrupper(str1_temp);
    MMIAPICOM_Wstrupper(str2_temp);
    ret = MMIAPICOM_Wstrcmp(str1_temp, str2_temp);

    SCI_FREE(str1_temp);
    SCI_FREE(str2_temp);
    return ret;
}

/*****************************************************************************/
//  Description : Add or Edit bookmark
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIBRW_ChangeBookmarkWin(MMIBRW_BM_CHANGE_TYPE_E change_type)
{
    wchar* device_name_ptr = PNULL;
    uint16 device_name_len = 0;
    
    MMIBROWSER_GetBrwSysDevName(&device_name_ptr, &device_name_len);
    if(PNULL == device_name_ptr ||0 == device_name_len)
    {
        BrwOpenAlertWinByTextId(PNULL, TXT_SD_NO_EXIST, TXT_NULL, IMAGE_PUBWIN_WARNING,  PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
        MMIBROWSER_SetBookmarkLoaded(FALSE);//set bookmark is not loaded
        return;
    }
    //限制目录数量
    if ((MMIBRW_BM_CHANGE_ADD_DIR == change_type) && (MMIBRW_BookmarkGetFolderNum((FALSE)) >= MMIBRW_BOOKMARK_DIR_MAX_NUM))
    {
        BrwOpenAlertWinByTextId(PNULL,TXT_COMMON_FOLDER_FULL,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        return;
    }

    //限制书签数量
    if(((MMIBRW_BM_CHANGE_ADD_CUR_PAGE == change_type)
        || (MMIBRW_BM_CHANGE_ADD_URL == change_type)
        || (MMIBRW_BM_CHANGE_ADD_NULL == change_type))
        && (MMIBRW_BookmarkGetCount() >= MMIBRW_BOOKMARK_MAX_ITEM))
    {
        BrwOpenAlertWinByTextId(PNULL,TXT_COMON_BOOKMARK_FULL,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
    }
    else
    {
#ifdef BROWSER_START_PAGE_THEME1
        if((MMIBRW_BM_CHANGE_ADD_CUR_PAGE == change_type) || (MMIBRW_BM_CHANGE_ADD_URL == change_type))
        {
            MMIBROWSER_UpdateBkListToRoot();
        }
#endif
    
        MMK_CreateWin((uint32*)MMIBROWSER_BOOKMARK_EDIT_WIN_TAB, (ADD_DATA)change_type);
    }
}

/*****************************************************************************/
//  Description: 修改路径文件夹的index
//  Global resource dependence : 
//  Author:dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL void BrwSetPathFolderIndex(uint32 index)
{
    s_diplay_route_index = index;
}

/*****************************************************************************/
//  Description:获取 修改路径文件夹的index
//  Global resource dependence : 
//  Author:dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL uint32 BrwGetPathFolderIndex(void)
{
    return s_diplay_route_index;
}

/*****************************************************************************/
//  Description: 记下刚进入时的路径文件夹的index
//  Global resource dependence : 
//  Author:dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL void BrwSetPathOringalFolderIndex(uint32 index)
{
    s_former_route_index = index;
}

/*****************************************************************************/
//  Description:获取 刚进入时的原始路径文件夹的index
//  Global resource dependence : 
//  Author:dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL uint32 BrwGetOringalPathFolderIndex(void)
{
    return s_former_route_index;
} 
/*****************************************************************************/
//  Description:检查bookmark的合理性并进行修订
//  Global resource dependence : 
//  Author:liyan.zhu
//  Note: 修订bookmark显示：若显示为INDEX OF/修订为INDEX OF/.
/*****************************************************************************/
LOCAL BOOLEAN BrwChkAndMdBookMarkName(MMI_STRING_T* title_text_ptr)
{
	uint16     i = 0;
	uint16 count = 0 ;
	if(PNULL == title_text_ptr)
    {
		return FALSE;
    }
	if (0 == title_text_ptr->wstr_len)
	{
		return FALSE;
	}
	for (i = 0; i < title_text_ptr->wstr_len; ++i)
	{
		if ('/' == title_text_ptr->wstr_ptr[i])
		{
			count++;
		}
	}
	if (count>1)
	{
		BrwOpenAlertWinByTextId(PNULL,TXT_COMM_INVALID_TITLE,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        return FALSE;
	}
    else
	{
		if ('/' == title_text_ptr->wstr_ptr[title_text_ptr->wstr_len-1])
		{
			title_text_ptr->wstr_ptr[title_text_ptr->wstr_len] = '.';
			title_text_ptr->wstr_len++;
		}
	    return TRUE;
	}

}
/*****************************************************************************/
//  Description:save bookmark or folder in edit bookmark window
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMIBROWSER_BK_REPLACE_RESULT_E BrowserHanldeSaveInEditWin(
                MMI_WIN_ID_T win_id, 
                MMIBRW_BM_CHANGE_TYPE_E change_type,
                uint16 *duplicated_index_ptr
                )
{
    uint16 current_index = BrwGetBookmarkCurIndex();
    int32    duplicated_index = 0;
    uint16 new_index = 0;
    MMI_STRING_T url_text_str = {0};
    MMI_STRING_T title_text_str = {0};
    MMI_HANDLE_T detail_win_handle = MMIBROWSER_BOOKMARK_DETAIL_WIN_ID;
    MMI_HANDLE_T bm_win_handle = MMIBROWSER_BOOKMARK_WIN_ID;
    MMI_HANDLE_T bm_ctrl_handle = MMIBROWSER_LIST_BOOKMARK_CTRL_ID;
    MMIBRW_BOOKMARK_T *bookmark_info_ptr = MMIBRW_GetBookmark();
    uint32 folder_index = 0;
    wchar full_path_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16 full_path_name_len = MMIFILE_FULL_PATH_MAX_LEN;
    uint32 file_array_step = 0;
    uint16 *new_index_ptr = PNULL;
    BOOLEAN is_need_update_cur_index = TRUE;
    BOOLEAN is_save_success = FALSE;

    if (MMK_IsOpenWin(bm_win_handle))
    {
        new_index_ptr = &new_index;
    }
    
    folder_index = BrwGetPathFolderIndex();
    if (0 == folder_index)
    {
        BrwGetDefaultRootPath(full_path_name, &full_path_name_len);
        file_array_step = 0;
    }
    else
    {
        if (0 ==bookmark_info_ptr->file_array_step)//root
        {
            MMIBRW_BookmarkGetFolder(folder_index - 1, full_path_name, &full_path_name_len, bookmark_info_ptr->file_array);
        }
        else
        {
            MMIBRW_BookmarkGetFolder(folder_index - 1, full_path_name, &full_path_name_len, PNULL);
        }
        file_array_step = 1;
    }
    
    GUIEDIT_GetString(MMIBROWSER_BOOKMARK_EDIT_TITLE_CTRL_ID, &title_text_str);
    GUIEDIT_GetString(MMIBROWSER_BOOKMARK_EDIT_URL_CTRL_ID, &url_text_str);

	if(!BrwChkAndMdBookMarkName(&title_text_str))
	{
		return MMIBROWSER_BK_REPLACE_MAX;
	}

    if ((MMIBRW_BM_CHANGE_EDIT_DIR == change_type) || (MMIBRW_BM_CHANGE_ADD_DIR == change_type))//dir
    {
        duplicated_index = MMIBRW_FindDuplicatedBookmarkDirIndex(bookmark_info_ptr->full_path, bookmark_info_ptr->full_path_len, 0, title_text_str.wstr_ptr);
        if (MMIBRW_BM_CHANGE_EDIT_DIR == change_type)
        {
            if ((duplicated_index >= 0) && (duplicated_index != current_index))/*重复了,提示"已存在"*/
            {
                return MMIBROWSER_BK_REPLACE_DIR;
            }
            else
            {
                is_save_success = MMIBRW_ChangeBookmarkTitle(current_index, title_text_str, PNULL);
            }
        }
        else
        {
            if (duplicated_index >= 0)/*重复了,提示"已存在"*/
            {
                return MMIBROWSER_BK_REPLACE_DIR; 
            }
            else
            {
                is_save_success = MMIBRW_AddBookmarkFolder(title_text_str, FALSE,  new_index_ptr);
            }
        }
    }
    else//bookmark
    {
        duplicated_index = MMIBRW_FindDuplicatedBookmarkIndex(full_path_name, full_path_name_len, file_array_step, title_text_str.wstr_ptr);

        if (MMIBRW_BM_CHANGE_EDIT == change_type)
        {
            if ((duplicated_index >=0) && (duplicated_index != current_index))/*重复了,提示"已存在"*/
            {
                return MMIBROWSER_BK_REPLACE_FILE;                    
            }

            is_save_success = MMIBRW_ChangeBookmarkTitleAndUrl(current_index, title_text_str ,url_text_str, new_index_ptr);/* Use the new index */ 
        }
        else  //add bookmark
        {
            if (duplicated_index >= 0)//提示"是否替换"
            {
                MMI_STRING_T url_string = {0};

                MMIBRW_GetBookmarkUrl(duplicated_index, &url_string);

                if (url_string.wstr_len == url_text_str.wstr_len && (0 == MMIBRW_Wstricmp(url_string.wstr_ptr, url_text_str.wstr_ptr)))
                {
                    return MMIBROWSER_BK_REPLACE_FILE;
                }
                else
                {
                    if (duplicated_index_ptr != PNULL)
                    {
                        *duplicated_index_ptr = (uint16)duplicated_index;
                    }
                    //query replace?
                }

                if (PNULL != url_string.wstr_ptr)
                {
                   SCI_FREE(url_string.wstr_ptr);
                }
                return MMIBROWSER_BK_REPLACE_DUPLICATE;
            }
            else
            {
                if (BrwGetOringalPathFolderIndex() == BrwGetPathFolderIndex())//same path step
                {
                    is_save_success = MMIBRW_AddBookmark(url_text_str, title_text_str, FALSE, new_index_ptr, full_path_name, TRUE);
                }
                else
                {
                    is_need_update_cur_index = FALSE;
                    is_save_success = MMIBRW_AddBookmark(url_text_str, title_text_str, FALSE, PNULL, full_path_name, FALSE);
                }
            }
        }
    }


    //SCI_TRACE_LOW:"[BRW]BrowserHanldeSaveInEditWin:is_save_success = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_BOOKMARK_3679_112_2_18_2_6_32_278,(uint8*)"d", is_save_success);
    if (!is_save_success)
    {
        return MMIBROWSER_BK_REPLACE_ERROR;
    }
    
    if (MMK_IsOpenWin(bm_win_handle))
    {
        MMIBRW_AppendListboxitem(bm_win_handle, MMIBROWSER_LIST_BOOKMARK_CTRL_ID, 
                (uint16)MMIBRW_GetBookmarkNumber());

        if (is_need_update_cur_index)
        {
            if (new_index_ptr != PNULL)
            {
                GUILIST_SetCurItemIndex(bm_ctrl_handle, *new_index_ptr);
            }
            else
            {
                GUILIST_SetCurItemIndex(bm_ctrl_handle, 0);
            }
        }
    }
    else
    {
        //do nothing
    }

    BrwUpdateBKDetailWinInfo(detail_win_handle, title_text_str, url_text_str);
    
    return MMIBROWSER_BK_REPLACE_SUCCESS;
} 

/*****************************************************************************/
//  Description :  Handle open memo main waiting win
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBKOpenMainWaitingWin(
                         MMI_WIN_ID_T       win_id,     
                         MMI_MESSAGE_ID_E   msg_id, 
                         DPARAM             param
                         )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    static BOOLEAN  s_is_lose_focus = FALSE;
    
    //SCI_TRACE_LOW:"HandleBKOpenMainWaitingWin, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_BOOKMARK_3725_112_2_18_2_6_33_279,(uint8*)"d", msg_id);
    
    switch (msg_id)
    {    
    case MSG_BRW_BK_OPEN_MAIN:
        MMIBRW_LoadBookmarkFileArray(BOOKMARK_STEP_TYPE_ROOT, 0, FUNC_FIND_NULL, win_id, MSG_BRW_BK_OPEN_MAIN_FINISH);
        break;
        
    case MSG_BRW_BK_OPEN_MAIN_FINISH:
        MMIBROWSER_LoadFinish();
        if(MMK_IsFocusWin(win_id))
        {
            MMK_CreateWin((uint32*)MMIBROWSER_BOOKMARK_WIN_TAB,PNULL);
            MMK_CloseWin(win_id);
        }
        else
        {
            s_is_lose_focus = TRUE;
        }
        break;

    case MSG_GET_FOCUS:
        if(s_is_lose_focus)
        {
            s_is_lose_focus = FALSE;
            MMK_CreateWin((uint32*)MMIBROWSER_BOOKMARK_WIN_TAB,PNULL);
            MMK_CloseWin(win_id);
        }
        else
        {
            result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        }
        break;

#ifndef MMI_PDA_SUPPORT
    case MSG_APP_RED:
#else
    case MSG_KEYUP_RED:
#endif
        break;
        
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }

    //SCI_TRACE_LOW:"HandleBKOpenMainWaitingWin, msg_id = 0x%x end"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_BOOKMARK_3767_112_2_18_2_6_33_280,(uint8*)"d", msg_id);

    return result;
}
#endif

/*****************************************************************************/
//  Description :  update the bk list window
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
LOCAL void BrwUpdateBKListWindow(
                const MMI_STRING_T *title_str_ptr,
                uint16 *current_index_ptr   //current index
                )
{
    MMI_HANDLE_T bk_list_ctrl_handle = MMIBROWSER_LIST_BOOKMARK_CTRL_ID;
    MMI_HANDLE_T bk_list_win_handle = MMIBROWSER_BOOKMARK_WIN_ID;

#ifndef MMIDORADO_BOOKMARK_BY_NV
    MMI_STRING_T    str_text = {0};
    MMIBRW_BOOKMARK_T *bookmark_info_ptr = MMIBRW_GetBookmark();

   MMIBRW_AppendListboxitem(bk_list_win_handle,bk_list_ctrl_handle, 
                                            (uint16)bookmark_info_ptr->cur_index_num);
                                            
    if (0 == bookmark_info_ptr->file_array_step)
    {
        //set title as "my bookmark"
        //GUIWIN_SetTitleTextId(bk_list_win_handle, TXT_BOOKMARK, FALSE);
        MMI_GetLabelTextByLang(TXT_BOOKMARK, &str_text);
        GUILIST_SetCurItemIndex(bk_list_ctrl_handle, s_pre_path_index);
    }
    else
    {
        if ((title_str_ptr != PNULL) && (title_str_ptr->wstr_ptr != PNULL) && (title_str_ptr->wstr_len > 0))
        {
            //GUIWIN_SetTitleText(bk_list_win_handle, title_str_ptr->wstr_ptr, title_str_ptr->wstr_len, FALSE);
            SCI_MEMCPY(&str_text, title_str_ptr, sizeof(MMI_STRING_T));
        }
        else
        {
            //set title as "my bookmark"
            //GUIWIN_SetTitleTextId(bk_list_win_handle, TXT_BOOKMARK, FALSE);
            MMI_GetLabelTextByLang(TXT_BOOKMARK, &str_text);
        }
        
        GUILIST_SetCurItemIndex(bk_list_ctrl_handle, 0);
    }
	
#ifdef BROWSER_BOOKMARK_HISTORY_TAB
    GUITAB_SetSimpleItem(
        MMIBROWSER_BK_AND_HISTORY_TAB_CTRL_ID,
        &str_text,
        IMAGE_BROWSER_BOOKMARK_LIST,
        IMAGE_BROWSER_BOOKMARK_LIST,
        MMIBROWSER_TAB1_BOOKMARK
        );
#else
	GUIWIN_SetTitleText(bk_list_win_handle, str_text.wstr_ptr, str_text.wstr_len, FALSE);
#endif	

#else/*MMIDORADO_BOOKMARK_BY_NV*/
   MMIBRW_AppendListboxitem(bk_list_win_handle,bk_list_ctrl_handle, MMIBRW_BookmarkGetCount());
#endif/*MMIDORADO_BOOKMARK_BY_NV*/

    if (current_index_ptr != PNULL)
    {
        GUILIST_SetCurItemIndex(bk_list_ctrl_handle, *current_index_ptr);
    }
}

#ifndef MMIDORADO_BOOKMARK_BY_NV
/*****************************************************************************/
//  Description :  Handle open memo main waiting win
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBKOpenStepWaitingWin(
                         MMI_WIN_ID_T       win_id,     
                         MMI_MESSAGE_ID_E   msg_id, 
                         DPARAM             param
                         )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    LOCAL BOOLEAN  s_is_lose_focus = FALSE;
    LOCAL MMI_STRING_T s_step_title_str= {0};
    MMIBROWSER_BK_STEP_INFO *step_info_ptr = PNULL;

    //SCI_TRACE_LOW:"HandleBKOpenStepWaitingWin, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_BOOKMARK_3846_112_2_18_2_6_33_281,(uint8*)"d", msg_id);
    
    switch (msg_id)
    {
    case MSG_BRW_BK_OPEN_STEP:
        if (PNULL != param) //MMIAPIFMM_SearchFileInPath is successful 
        {
            step_info_ptr = (MMIBROWSER_BK_STEP_INFO*)param;
            
            if (BOOKMARK_STEP_TYPE_ROOT != step_info_ptr->step_type)
            {
                MMIBRW_GetBookmarkTitle(step_info_ptr->cur_array_index, &s_step_title_str);
            }
            MMIBRW_LoadBookmarkFileArray(step_info_ptr->step_type, step_info_ptr->cur_array_index, 
                step_info_ptr->fmm_func_type, win_id, MSG_BRW_BK_OPEN_STEP_FINISH);
        }
        else
        {
            MMK_CloseWin(win_id);
        }
        break;
        
    case MSG_BRW_BK_OPEN_STEP_FINISH:
        MMIBROWSER_LoadFinish();
        BrwUpdateBKListWindow(&s_step_title_str, PNULL);
        
        if(MMK_IsFocusWin(win_id))
        {
            MMK_CloseWin(win_id);
        }
        else
        {
            s_is_lose_focus = TRUE;
        }
        break;

    case MSG_GET_FOCUS:
        if(s_is_lose_focus)
        {
            MMK_CloseWin(win_id);
        }
        break;

#ifndef MMI_PDA_SUPPORT
    case MSG_APP_RED:
#else
    case MSG_KEYUP_RED:
#endif
        break;

    case MSG_CLOSE_WINDOW:
        if (PNULL != s_step_title_str.wstr_ptr)
        {
            SCI_FREE(s_step_title_str.wstr_ptr);
        }
        SCI_MEMSET(&s_step_title_str, 0x00, sizeof(s_step_title_str));
        s_is_lose_focus= FALSE;
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
        
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }

    //SCI_TRACE_LOW:"HandleBKOpenStepWaitingWin, msg_id = 0x%x end"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_BOOKMARK_3907_112_2_18_2_6_33_282,(uint8*)"d", msg_id);

    return result;
}


/*****************************************************************************/
//  Description : open the bookmark window
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL void BrwOpenBookmarkStepWin(                        
                        MMIBRW_BOOKMARK_STEP_TYPE_E step_type,//in
                        uint16 cur_array_index,//in:load 根目录时无效
                        MMIFMM_FUNC_TYPE_E   fmm_func_type//in
                        )
{
    MMI_WIN_ID_T waiting_win_id = MMIBROWSER_BOOKMARK_WAITING_WIN_ID;
    uint32       waiting_time_out = 0;
    MMIBROWSER_BK_STEP_INFO step_info = {0};
    
    step_info.step_type = step_type;
    step_info.cur_array_index = cur_array_index;
    step_info.fmm_func_type =fmm_func_type;
    
    MMIPUB_OpenAlertWinByTextId(&waiting_time_out, TXT_COMMON_WAITING, TXT_NULL,
        IMAGE_PUBWIN_WAIT, &waiting_win_id,PNULL,MMIPUB_SOFTKEY_NONE,HandleBKOpenStepWaitingWin);

    MMK_PostMsg(waiting_win_id, MSG_BRW_BK_OPEN_STEP, (DPARAM)&step_info, sizeof(MMIBROWSER_BK_STEP_INFO));
}

/*****************************************************************************/
//  Description : prepare to go back to root path
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL void BrwPrepare2BackToRoot(void)
{
    uint16  current_index = 0;

    MMIBRW_LoadBookmarkFileArray(BOOKMARK_STEP_TYPE_ROOT, 0, FUNC_FIND_NULL, 0, 0);
    BrwUpdateBKListWindow(PNULL, &current_index);
}

/*****************************************************************************/
//  Description : open the bookmark window
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL void BrwBegin2Save(MMIBRW_BM_CHANGE_TYPE_E change_type)
{
    MMI_WIN_ID_T waiting_win_id = MMIBROWSER_BOOKMARK_SAVE_WAITING_WIN_ID;
    uint32       waiting_time_out = 0;
    MMIBRW_BM_CHANGE_TYPE_E change_type_param = change_type;
    
    MMIPUB_OpenAlertWinByTextId(&waiting_time_out, TXT_COMMON_WAITING, TXT_NULL,
        IMAGE_PUBWIN_WAIT, &waiting_win_id,PNULL,MMIPUB_SOFTKEY_NONE,HandleBKSaveWaitingWin);

    MMK_PostMsg(waiting_win_id, MSG_BRW_BK_SAVE, (DPARAM)&change_type_param, sizeof(change_type_param));
}
#endif

/*****************************************************************************/
//  Description:update the boomakr detail window by new infomation
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
LOCAL void BrwUpdateBKDetailWinInfo(
                MMI_HANDLE_T detail_win_handle,
                MMI_STRING_T title_text_str,
                MMI_STRING_T url_text_str
                )
{
    if (MMK_IsOpenWin(detail_win_handle))
    {
        GUIRICHTEXT_ITEM_T item_data1 = {0};
        GUIRICHTEXT_ITEM_T org_item_data = {0};
        MMI_HANDLE_T rich_ctrl_handle = MMK_GetCtrlHandleByWin(detail_win_handle, MMIBROWSER_BOOKMARK_DETAIL_CTRL_ID); 

        GUIRICHTEXT_GetItem(rich_ctrl_handle, MMIBRW_BOOKMARK_TITLE_INDEX, &org_item_data);
        SCI_MEMCPY(&item_data1, &org_item_data, sizeof(GUIRICHTEXT_ITEM_T));
        item_data1.text_data.buf.len = title_text_str.wstr_len;
        item_data1.text_data.buf.str_ptr = title_text_str.wstr_ptr;
        item_data1.text_type = GUIRICHTEXT_TEXT_BUF;
        GUIRICHTEXT_ModifyItem(rich_ctrl_handle, MMIBRW_BOOKMARK_TITLE_INDEX, &item_data1);

        SCI_MEMSET(&item_data1, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
        SCI_MEMSET(&org_item_data, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
        GUIRICHTEXT_GetItem(rich_ctrl_handle, MMIBRW_BOOKMARK_URL_INDEX, &org_item_data);
        SCI_MEMCPY(&item_data1, &org_item_data, sizeof(GUIRICHTEXT_ITEM_T));
        item_data1.text_data.buf.len = url_text_str.wstr_len;
        item_data1.text_data.buf.str_ptr = url_text_str.wstr_ptr;
        item_data1.text_type = GUIRICHTEXT_TEXT_BUF;
        GUIRICHTEXT_ModifyItem(rich_ctrl_handle, MMIBRW_BOOKMARK_URL_INDEX, &item_data1);
    }

}

#ifndef MMIDORADO_BOOKMARK_BY_NV
/*****************************************************************************/
//  Description : search the same name in album list
//  Global resource dependence : 
//  Author: liyan.zhu
//  Return: TRUE: contain invalid char. FALSE: do not contain invalid char
//  Note:copy from MMIAPICOM_IsIncludeInvalidChar
/*****************************************************************************/
LOCAL BOOLEAN BrwBookMark_IsIncludeInvalidChar(
                                              const wchar* name_ptr, 
                                              uint16 len//, 
                                              //BOOLEAN is_ucs2
                                              )
{
    const uint8 invalid_char[] = {'\\', ':', '*', '?', '"', '<', '>', '|',0x0D, 0x0A};
    int32 i = 0;
    int32 j = 0;
    uint8 invalid_char_num  = sizeof(invalid_char) ;
	
    if (PNULL == name_ptr || 0 == len)
    {
        return TRUE;
    }
	
    for(i = 0; i < len; i++)
    {
        for (j = 0; j < invalid_char_num; j++)
        {
            if (*(name_ptr+i) == invalid_char[j])
            {
                return TRUE;
            }
        }
    }
	
    return FALSE;
}
/*****************************************************************************/
//  Description:分析编辑内容是否有效,并进行相应的提示
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN BrwParserEditInfoIsValid(
                MMIBRW_BM_CHANGE_TYPE_E change_type,
                MMI_STRING_T title_text_str,
                MMI_STRING_T url_text_str
                )
{
    BOOLEAN is_with_url = TRUE;

    if ((MMIBRW_BM_CHANGE_ADD_DIR == change_type) || (MMIBRW_BM_CHANGE_EDIT_DIR == change_type))
    {
        is_with_url = FALSE;
    }

    //begin:alert fail
    if ((PNULL == title_text_str.wstr_ptr) || (0 == title_text_str.wstr_len))
    {
        //alert the name is empty
        BrwOpenAlertWinByTextId(PNULL,TXT_COMM_NO_TITLE,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        return FALSE;
    }
    else if (BrwBookMark_IsIncludeInvalidChar(title_text_str.wstr_ptr, title_text_str.wstr_len))//MS00209419
    {
        //alert the nameis empty
        BrwOpenAlertWinByTextId(PNULL,TXT_COMM_INVALID_TITLE,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        return FALSE;
    }      
    if (!BrwChkAndMdBookMarkName(&title_text_str))
    {
		return FALSE;
    }
    
    if (is_with_url)
    {
        uint8    url_ptr[MMIBRW_BOOKMARK_MAX_URL_LENGTH+1] ={0};   
        
        if ((PNULL == url_text_str.wstr_ptr) || (0 == url_text_str.wstr_len))
        {
            BrwOpenAlertWinByTextId(PNULL,TXT_COMM_URL_NULL,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            return FALSE;//stop save
        }
        else
        {
            //save url
            MMI_WSTRNTOSTR(url_ptr,MMIBROWSER_MAX_URL_LEN,
                                            url_text_str.wstr_ptr, url_text_str.wstr_len, 
                                            MIN(MMIBROWSER_MAX_URL_LEN, url_text_str.wstr_len));
        }
        
        if (Brw_IsIncludeInvalidUrl(url_ptr))
        {
            MMIBROWSER_AlertUrlInvalid();
            return FALSE;//stop save
        }
    }
    //end:alert fail
#ifndef WIN32
    if ((MMIBRW_BM_CHANGE_ADD_NULL == change_type) || (MMIBRW_BM_CHANGE_ADD_CUR_PAGE == change_type) 
        || (MMIBRW_BM_CHANGE_ADD_URL == change_type) || (MMIBRW_BM_CHANGE_ADD_DIR == change_type))
    {
        MMIFILE_DEVICE_E    dev = MMI_DEVICE_NUM;
        wchar               path[MMIFMM_FILE_FULL_PATH_MAX_LEN + 1] = {0};
        uint16              full_path_len = MMIFMM_FILE_FULL_PATH_MAX_LEN;

        MMIBROWSER_BookmarkGetRootPath(path, &full_path_len, &dev);

        if (MMI_GetFreeSpace(dev) < MMIBRW_ONE_BK_MIN_SPACE)
        {
            BrwOpenAlertWinByTextId(PNULL, TXT_NO_SPACE, TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
            MMK_CloseWin(MMIBROWSER_BOOKMARK_EDIT_WIN_ID);
            return FALSE;//stop save
        }
    }
#endif

    return TRUE;
}

/*****************************************************************************/
//  Description :  Handle open memo main waiting win
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBKSaveWaitingWin(
                         MMI_WIN_ID_T       win_id,     
                         MMI_MESSAGE_ID_E   msg_id, 
                         DPARAM             param
                         )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    LOCAL BOOLEAN  s_is_lose_focus = FALSE;
    LOCAL uint16 s_duplicated_index = 0;
    LOCAL MMIBROWSER_BK_REPLACE_RESULT_E  s_save_result = MMIBROWSER_BK_REPLACE_NONE;

    //SCI_TRACE_LOW:"HandleBKSaveWaitingWin, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_BOOKMARK_4092_112_2_18_2_6_33_283,(uint8*)"d", msg_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        s_is_lose_focus = FALSE;
        s_duplicated_index = 0;
        s_save_result = MMIBROWSER_BK_REPLACE_NONE;
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    
    case MSG_BRW_BK_SAVE:
        if (PNULL != param) //MMIAPIFMM_SearchFileInPath is successful 
        {            
            s_save_result = BrowserHanldeSaveInEditWin(MMIBROWSER_BOOKMARK_EDIT_WIN_ID, *((MMIBRW_BM_CHANGE_TYPE_E*)param), &s_duplicated_index);
            //SCI_TRACE_LOW:"[BRW]HandleBKSaveWaitingWin:s_save_result = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_BOOKMARK_4107_112_2_18_2_6_33_284,(uint8*)"d", s_save_result);

            if(MMK_IsFocusWin(win_id))//handle save result
            {
                switch (s_save_result)
                {
                case MMIBROWSER_BK_REPLACE_DIR:
                    BrwOpenAlertWinByTextId(PNULL, TXT_WWW_BOOKMARK_DIR_EXIST, TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
                    MMK_CloseWin(win_id);
                    break;
                    
                case MMIBROWSER_BK_REPLACE_FILE:
                    BrwOpenAlertWinByTextId(PNULL, TXT_WWW_BOOKMARK_EXIST, TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
                    MMK_CloseWin(win_id);
                  break;
                    
                case MMIBROWSER_BK_REPLACE_DUPLICATE:
                    {
                        MMI_WIN_ID_T query_win_id = MMIBROWSER_BOOKMARK_QUERY_WIN_ID;
                        MMIPUB_OpenQueryWinByTextIdEx(VIRTUAL_WIN_ID, 
                                    TXT_WWW_BOOKMARK_QUERY_REPLACE, 
                                    IMAGE_PUBWIN_QUERY, 
                                    &query_win_id, 
                                    MMIBRW_HandleReplaceDuplicatedBookmark, 
                                    s_duplicated_index);
                    }
                    break;
                    
                case MMIBROWSER_BK_REPLACE_ERROR:
                    MMIPUB_OpenAlertFailWin(TXT_MMI_SAVEFAILED);
                    MMK_CloseMiddleWin(MMIBROWSER_BOOKMARK_START_WIN_ID, MMIBROWSER_BOOKMARK_END_WIN_ID);//exit the bookmark 
                    break;
                                    
                default:
#ifdef MMI_GUI_STYLE_TYPICAL
                    BrwOpenAlertWinByTextId(PNULL,TXT_COMPLETE,TXT_NULL,IMAGE_PUBWIN_SUCCESS,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
#endif /* MMI_GUI_STYLE_TYPICAL */
                    MMK_CloseWin(win_id);
                    MMK_CloseWin(MMIBROWSER_BOOKMARK_EDIT_WIN_ID);
                    break;
                }
            }
            else
            {
                s_is_lose_focus = TRUE;
            }

        }
        else
        {
            MMK_CloseWin(win_id);
        }
        break;
        
    case MSG_GET_FOCUS:
        if(s_is_lose_focus)
        {
            switch (s_save_result)
            {
            case MMIBROWSER_BK_REPLACE_DIR:
                BrwOpenAlertWinByTextId(PNULL, TXT_WWW_BOOKMARK_DIR_EXIST, TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
                break;
                
            case MMIBROWSER_BK_REPLACE_FILE:
                BrwOpenAlertWinByTextId(PNULL, TXT_WWW_BOOKMARK_EXIST, TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
                break;
                
            case MMIBROWSER_BK_REPLACE_DUPLICATE:
                {
                    MMI_WIN_ID_T query_win_id = MMIBROWSER_BOOKMARK_QUERY_WIN_ID;
                    MMIPUB_OpenQueryWinByTextIdEx(VIRTUAL_WIN_ID, 
                                TXT_WWW_BOOKMARK_QUERY_REPLACE, 
                                IMAGE_PUBWIN_QUERY, 
                                &query_win_id, 
                                MMIBRW_HandleReplaceDuplicatedBookmark, 
                                s_duplicated_index);
                }
                break;
                
            case MMIBROWSER_BK_REPLACE_ERROR:
                MMIPUB_OpenAlertFailWin(TXT_MMI_SAVEFAILED);
                MMK_CloseMiddleWin(MMIBROWSER_BOOKMARK_START_WIN_ID, MMIBROWSER_BOOKMARK_END_WIN_ID);//exit the bookmark 
                break;

            default:
#ifdef MMI_GUI_STYLE_TYPICAL
                BrwOpenAlertWinByTextId(PNULL,TXT_COMPLETE,TXT_NULL,IMAGE_PUBWIN_SUCCESS,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
#endif /* MMI_GUI_STYLE_TYPICAL */
                MMK_CloseWin(win_id);
                MMK_CloseWin(MMIBROWSER_BOOKMARK_EDIT_WIN_ID);
                break;
            }
        }
        break;

#ifndef MMI_PDA_SUPPORT
    case MSG_APP_RED:
#else
    case MSG_KEYUP_RED:
#endif
        break;
        
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }

    //SCI_TRACE_LOW:"HandleBKOpenStepWaitingWin, msg_id = 0x%x end"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_BOOKMARK_4206_112_2_18_2_6_34_285,(uint8*)"d", msg_id);

    return result;
}
#endif

#ifdef MMI_PDA_SUPPORT
/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  BrwCreateBookmarkPopUpMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    uint32   num = 0;
    uint16  index = 0;
    MMI_STRING_T    title_str = {0};
    uint16  array_index = 0;
#ifndef MMIDORADO_BOOKMARK_BY_NV
    MMIBRW_BOOKMARK_T   *bookmark_info_ptr = MMIBRW_GetBookmark();
    FILEARRAY_DATA_T    array_item = {0};
    uint32  folder_node_index[] = {ID_POPUP_MENU_BOOKMARK_EDIT_FOLDER, ID_POPUP_MENU_BOOKMARK_DELETE,};
#endif

    MMI_TEXT_ID_T   tex_index[] = {
                TXT_EDIT, 
                TXT_DELETE, 
#ifndef MMIDORADO_BOOKMARK_BY_NV
                TXT_WWW_BOOKMARK_MANAGE_MOVE_TO, 
#endif                
                TXT_WWW_SEND_LINK, 
#ifdef SNS_SUPPORT
                TXT_SHARE_TO_SNS,
#endif
#ifndef CMCC_UI_STYLE
                TXT_BROWSER_PDA_SAVE_AS_HOMEPAGE,
#endif
#ifdef MMIDORADO_MY_NAV_SUPPORT
                TXT_BROWSER_ADD_TO_NAV,
#endif
                TXT_COMM_DETAIL
                };
    uint32  node_index[] = {
                ID_POPUP_MENU_BOOKMARK_EDIT, 
                ID_POPUP_MENU_BOOKMARK_DELETE, 
#ifndef MMIDORADO_BOOKMARK_BY_NV
                ID_POPUP_MENU_BOOKMARK_MOVE_TO, 
#endif                
                ID_POPUP_MENU_BOOKMARK_SEND_URL, 
#ifdef SNS_SUPPORT
                ID_POPUP_MENU_BOOKMARK_SHARE2SNS,
#endif
#ifndef CMCC_UI_STYLE
                ID_POPUP_MENU_BOOKMARK_AS_HOMEPAGE,
#endif
#ifdef MMIDORADO_MY_NAV_SUPPORT
                ID_POPUP_MENU_BOOKMARK_ADD2MYNAV,
#endif
                ID_POPUP_MENU_BOOKMARK_DETAIL
                };
    
#ifdef SNS_SUPPORT    
    uint32 i = 0;
    uint16              nodex_index =0;
    uint32  node_id = 0;
    MMI_STRING_T        kstring = {0};
    GUIMENU_DYNA_ITEM_T node_item = {0};
#endif    
    
    index = GUILIST_GetCurItemIndex(MMIBROWSER_LIST_BOOKMARK_CTRL_ID);
    array_index  = BrwGetBookmarkArrayIndex(index);

#ifndef MMIDORADO_BOOKMARK_BY_NV
    MMIAPIFILEARRAY_Read(bookmark_info_ptr->file_array, array_index, &array_item);
    MMIBRW_GetBookmarkTitle(array_index, &title_str);

    if (FILEARRAY_TYPE_FILE == array_item.type)
    {
        num = sizeof(node_index) / sizeof(node_index[0]);
        MMIBROWSER_CreatePopupAutoMenu(win_id, ctrl_id, &title_str, num, tex_index, node_index);
    }
    else
    {
        num = sizeof(folder_node_index) / sizeof(folder_node_index[0]);
        MMIBROWSER_CreatePopupAutoMenu(win_id, ctrl_id, &title_str, num, tex_index, folder_node_index);
    }
    
    if(PNULL !=title_str.wstr_ptr)
    {
        SCI_FREE(title_str.wstr_ptr);
    }

#else
    MMIBRW_GetBookmarkTitle(array_index, &title_str);

    num = sizeof(node_index) / sizeof(node_index[0]);
    MMIBROWSER_CreatePopupAutoMenu(win_id, ctrl_id, &title_str, num, tex_index, node_index);
#endif    

#ifdef SNS_SUPPORT
#if (defined(WRE_WEIBO_SUPPORT)) || (defined(WRE_FACEBOOK_SUPPORT)) || (defined(WRE_TWITTER_SUPPORT))

    num = sizeof(s_menu_brwbm_share_sns_popupmenu_table)/sizeof(*s_menu_brwbm_share_sns_popupmenu_table);
    nodex_index = 0;
    for(i = 0; i < num; i++)
    {
        node_id = s_menu_brwbm_share_sns_popupmenu_table[i][0];
        MMI_GetLabelTextByLang(s_menu_brwbm_share_sns_popupmenu_table[i][1], &kstring);
        node_item.item_text_ptr = &kstring;
        GUIMENU_InsertNode(nodex_index, node_id, ID_POPUP_MENU_BOOKMARK_SHARE2SNS, &node_item, ctrl_id);
        nodex_index ++;
    }
#endif
#endif/*SNS_SUPPORT*/

}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleBookmarkPopUpWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"BrwHandleBookmarkPopUpWinMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_BOOKMARK_4306_112_2_18_2_6_34_286,(uint8*)"d", msg_id);

    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        BrwCreateBookmarkPopUpMenu(win_id, MMIBROWSER_BOOKMARK_POPUP_MENU_CTRL_ID);
        MMK_SetAtvCtrl(win_id, MMIBROWSER_BOOKMARK_POPUP_MENU_CTRL_ID);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    {
        MMI_MENU_ID_T               menu_id = 0;
        MMI_CTRL_ID_T               list_ctrl_id = MMIBROWSER_LIST_BOOKMARK_CTRL_ID;
        uint16  array_index = 0;//bookmark index include folder

        array_index = BrwGetBookmarkArrayIndex(GUILIST_GetCurItemIndex(list_ctrl_id));

        menu_id = GUIMENU_GetCurNodeId(MMIBROWSER_BOOKMARK_POPUP_MENU_CTRL_ID);
        switch (menu_id)
        {
        case ID_POPUP_MENU_BOOKMARK_EDIT:
            MMIBROWSER_OpenBookmarkEditWin(MMIBRW_BM_CHANGE_EDIT);
            break;

        case ID_POPUP_MENU_BOOKMARK_DELETE:
            BrwQueryIsDeleteBookmark(list_ctrl_id);
            break;

#ifndef MMIDORADO_BOOKMARK_BY_NV
        case ID_POPUP_MENU_BOOKMARK_EDIT_FOLDER:
            MMIBROWSER_OpenBookmarkEditWin(MMIBRW_BM_CHANGE_EDIT_DIR);
            break;

        case ID_POPUP_MENU_BOOKMARK_MOVE_TO:
            BrwBookmarkEnterMoveState(MMIBROWSER_BOOKMARK_WIN_ID, array_index);
            break;
#endif

        case ID_POPUP_MENU_BOOKMARK_SEND_URL:
            {
                MMI_STRING_T    title_str = {0};
                MMI_STRING_T    url_str = {0};

#ifndef MMIDORADO_BOOKMARK_BY_NV
                if ((FILEARRAY_TYPE_FILE == MMIBRW_GetBookmarkTitle(array_index, &title_str))
                    && MMIBRW_GetBookmarkUrl(array_index, &url_str))
#else
               if (MMIBRW_GetBookmarkTitle(array_index, &title_str)
                    && MMIBRW_GetBookmarkUrl(array_index, &url_str))
#endif
                {
                    if(MMIBROWSER_SendUrlContentBySms(title_str.wstr_ptr, title_str.wstr_len, url_str.wstr_ptr, url_str.wstr_len))
                    {
                        //successful
                    }
                    else
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_UNKNOWN_ERROR);
                    }
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_UNKNOWN_ERROR);
                }

#ifndef MMIDORADO_BOOKMARK_BY_NV
                if(PNULL !=title_str.wstr_ptr)
                {
                    SCI_FREE(title_str.wstr_ptr);
                }
#endif                
                if(PNULL !=url_str.wstr_ptr)
                {
                    SCI_FREE(url_str.wstr_ptr);
                }
            }
            break;

#ifdef SNS_SUPPORT           
            case ID_POPUP_MENU_BRW_SHARE_TO_SINA:
            case ID_POPUP_MENU_BRW_SHARE_TO_FACEBOOK:
            case ID_POPUP_MENU_BRW_SHARE_TO_TWITTER:
            {
                MMI_STRING_T           url_str = {0};
            
                if (MMIBRW_GetBookmarkUrl(array_index,&url_str))
                {
                    BrwHandleSnsOptMenu(&url_str, menu_id);
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_UNKNOWN_ERROR);
                }
                                      
                if(PNULL != url_str.wstr_ptr)
                {
                    SCI_FREE(url_str.wstr_ptr);
                }    
            }
            break;
#endif

        case ID_POPUP_MENU_BOOKMARK_DETAIL:
            //BrwOpenBookamrkPropertyWin();
            MMK_CreateWin((uint32*)BRW_BOOKMARK_DETAIL_WIN_TAB,(ADD_DATA)array_index);
            break;
            
        case ID_POPUP_MENU_BOOKMARK_AS_HOMEPAGE:
        {
            MMI_STRING_T    url_str = {0};
            
            if (MMIBRW_GetBookmarkUrl(array_index, &url_str) && MMIBROWSER_SetHomepageUrl(&url_str))
            {
#ifdef MMI_GUI_STYLE_TYPICAL
                MMIPUB_OpenAlertSuccessWin(TXT_BROWSER_SET_HOMEPAGE_SUCC);
#endif /* MMI_GUI_STYLE_TYPICAL */
            }
//            else
//            {
//                MMIPUB_OpenAlertWarningWin(TXT_BROWSER_SET_HOMEPAGE_FAIL);
//            }
            if(PNULL != url_str.wstr_ptr)
            {
                SCI_FREE(url_str.wstr_ptr);
            }
        }
            break;

#ifdef MMIDORADO_MY_NAV_SUPPORT
        case ID_POPUP_MENU_BOOKMARK_ADD2MYNAV:
        {
            MMI_STRING_T           url_str = {0};
            MMI_STRING_T           title_str = {0};
            
#ifndef MMIDORADO_BOOKMARK_BY_NV
            if ((FILEARRAY_TYPE_FILE == MMIBRW_GetBookmarkTitle(array_index, &title_str))
                && MMIBRW_GetBookmarkUrl(array_index, &url_str))
#else
            if (MMIBRW_GetBookmarkTitle(array_index, &title_str)
                && MMIBRW_GetBookmarkUrl(array_index, &url_str))
#endif
            {
                MMIBROWSER_Add2MyNav(title_str, url_str);
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_UNKNOWN_ERROR);
            }

#ifndef MMIDORADO_BOOKMARK_BY_NV
            if(PNULL != title_str.wstr_ptr)
            {
                SCI_FREE(title_str.wstr_ptr);
            }
#endif            
            if(PNULL != url_str.wstr_ptr)
            {
                SCI_FREE(url_str.wstr_ptr);
            }
        }
            break;
#endif

        default:
            break;
        }
        MMK_CloseWin(win_id);
        break;
    }
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
#ifdef SNS_SUPPORT           
    case MSG_SNS_OPERA_RESULT:
    {
        if(pMe)
        {
            IMMISNS_Release(pMe);
            pMe = PNULL;
        }
    }
    break;
#endif

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void BrwOpenBookamrkPropertyWin(void)
{
    uint32              alert_time_period = 0;
    wchar               property[MMIBROWSER_MAX_URL_LEN + GUIRICHTEXT_TEXT_MAX_LEN + MMIBROWSER_PROPERTY_EXT_LEN] = {0};
    MMI_STRING_T        str = {0};
    wchar               *property_ptr = PNULL;
    uint16              array_index = 0;
    MMI_CTRL_ID_T       list_ctrl_id = MMIBROWSER_LIST_BOOKMARK_CTRL_ID;
    uint16              cur_index = 0;

    cur_index = GUILIST_GetCurItemIndex(list_ctrl_id);
    array_index = BrwGetBookmarkArrayIndex(cur_index);

    property_ptr = property;

    /* Title: */
    MMIRES_GetText(TXT_COMM_TITLE, PNULL, &str);
    MMIAPICOM_Wstrncpy(property_ptr, str.wstr_ptr, str.wstr_len);
    property_ptr += str.wstr_len;

    property_ptr[0] = '\n';
    property_ptr += 1;

    /* Title */
    MMIBRW_GetBookmarkTitle(array_index, &str);

    MMIAPICOM_Wstrncpy(property_ptr, str.wstr_ptr, str.wstr_len);
    property_ptr += str.wstr_len;

    property_ptr[0] = '\n';
    property_ptr += 1;

#ifndef MMIDORADO_BOOKMARK_BY_NV
    if(PNULL != str.wstr_ptr)
    {
        SCI_FREE(str.wstr_ptr);
    }
#endif    

    /* URL: */
    MMIRES_GetText(TXT_INPUT_URL_ADRESS, PNULL, &str);
    MMIAPICOM_Wstrncpy(property_ptr, str.wstr_ptr, str.wstr_len);
    property_ptr += str.wstr_len;

    property_ptr[0] = '\n';
    property_ptr += 1;

    /* URL */
    MMIBRW_GetBookmarkUrl(array_index,&str);

    MMIAPICOM_Wstrncpy(property_ptr, str.wstr_ptr, str.wstr_len);
    property_ptr += str.wstr_len;

    if(PNULL != str.wstr_ptr)
    {
        SCI_FREE(str.wstr_ptr);
    }

    str.wstr_ptr = property;
    str.wstr_len = MMIAPICOM_Wstrlen(property);

    MMIPUB_OpenAlertWinByTextPtr(&alert_time_period, &str, PNULL, IMAGE_PUBWIN_SUCCESS, PNULL, PNULL, MMIPUB_SOFTKEY_CUSTOMER, BrwHandleBookmarkAlertWinMsg);
}

/*****************************************************************************/
// Description :
// Global resource dependence : none
// Author:Jiaoyou.wu
// Note:    
/*****************************************************************************/
LOCAL MMI_RESULT_E  BrwHandleBookmarkAlertWinMsg(
                                        MMI_WIN_ID_T     win_id, 
                                        MMI_MESSAGE_ID_E msg_id, 
                                        DPARAM           param 
                                        )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"BrwHandleBookmarkAlertWinMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_BOOKMARK_4547_112_2_18_2_6_34_287,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            MMI_STRING_T    title_str = {0};

            MMIPUB_SetWinSoftkey(win_id, TXT_NULL, TXT_COMMON_OK, FALSE);

            MMIRES_GetText(STXT_DETAIL, PNULL, &title_str);
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

/*****************************************************************************/
// 	Description : enter bookmark list marked status
//	Global resource dependence : none
//  Author: fen.xie
//	Note:
/*****************************************************************************/
LOCAL void BrwEnterBookmarkListMarked(
                               MMI_CTRL_ID_T  list_ctrl_id
                               )
{
    if (0 == GUILIST_GetSelectedItemIndex(list_ctrl_id,PNULL,0))
    {
        GUILIST_SetTextListMarkable(list_ctrl_id,TRUE);//set list enable mark
        GUILIST_SetMaxSelectedItem(list_ctrl_id, GUILIST_GetTotalItemNum(list_ctrl_id));//set mark max number
    }
    
    GUISOFTKEY_SetTextId(MMIBROWSER_BOOKMARK_WIN_ID, MMICOMMON_SOFTKEY_CTRL_ID, TXT_MARK_ALL, TXT_DELETE, STXT_CANCEL, FALSE );
    GUIFORM_SetChildDisplay(MMIBROWSER_BOOKMARK_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_NORMAL);
    GUIWIN_SetTitleButtonState(MMIBROWSER_BOOKMARK_WIN_ID, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
    GUILIST_SetAllSelected(list_ctrl_id,FALSE);
}

/*****************************************************************************/
// 	Description : exit bookmark list marked status
//	Global resource dependence : none
//  Author: fen.xie
//	Note:
/*****************************************************************************/
LOCAL void BrwExitBookmarkListMarked(
                               MMI_CTRL_ID_T  list_ctrl_id
                               )
{
    GUISOFTKEY_SetTextId(MMIBROWSER_BOOKMARK_WIN_ID, MMICOMMON_SOFTKEY_CTRL_ID, TXT_MARK_ALL, TXT_DELETE, STXT_CANCEL, FALSE );//reset
    GUIFORM_SetChildDisplay(MMIBROWSER_BOOKMARK_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
    GUIWIN_SetTitleButtonState(MMIBROWSER_BOOKMARK_WIN_ID, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
    GUILIST_SetAllSelected(list_ctrl_id,FALSE);
    GUILIST_SetTextListMarkable(list_ctrl_id,FALSE);
}
#endif /* MMI_PDA_SUPPORT */

/*****************************************************************************/
//	Description : the handle function of bookmark list delete window
//	Global resource dependence : 
//	Author:fen.xie
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleBookmarkDelQueryWinMsg(
                                                   MMI_WIN_ID_T win_id, 
                                                   MMI_MESSAGE_ID_E msg_id, 
                                                   DPARAM param
                                                   )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
#ifndef MMI_PDA_SUPPORT
    case MSG_APP_RED:
#else
    case MSG_KEYUP_RED:
#endif
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_WEB:
    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        {
            BOOLEAN delete_result =FALSE;
            uint16  total_bookmark_item = 0;
            uint16  mark_num = 0;
            MMI_CTRL_ID_T   list_ctrl_id = MMIBROWSER_LIST_BOOKMARK_CTRL_ID;
            
            mark_num = GUILIST_GetSelectedItemIndex(list_ctrl_id,PNULL,0);
            total_bookmark_item = GUILIST_GetTotalItemNum(list_ctrl_id);
            
            if(0 == mark_num)
            {
                if (HandleBrowserDeleteBookmark(list_ctrl_id,FALSE))//delete one 
                {
                    delete_result = TRUE;
                }
            }
            else  if(total_bookmark_item == mark_num)
            {
                if (HandleBrowserDeleteBookmark(list_ctrl_id,TRUE))//delete all
                {
                    delete_result = TRUE;
                }
            }
            else
            {
#ifndef MMIDORADO_BOOKMARK_BY_NV
				MMI_WIN_ID_T    wait_win_id =  MMIBROWSER_BOOKMARK_MOVE_WAITING_WIN_ID;//use move wait win temp
                MMIPUB_OpenProgressWinByTextId(TXT_PUB_DELETING, &wait_win_id, PNULL, MMIPUB_SOFTKEY_CUSTOMER, HandleProcessWinMsg);
                MMIPUB_SetProgressTotalSize(&wait_win_id, mark_num);
                
                BrwDeleteMarkedBookmarkItem(list_ctrl_id,wait_win_id, MMIBROWSER_BOOKMARK_WIN_ID);
                MMK_CloseWin(win_id);
                break;
#else
                uint16* select_item_ptr = PNULL;
                uint16  old_item_index = 0;

                select_item_ptr = (uint16*)(SCI_ALLOCA(sizeof(uint16) * mark_num));
                //ger current index
                old_item_index = GUILIST_GetCurItemIndex(list_ctrl_id);

                if (PNULL != select_item_ptr)
                {
                    while(mark_num > 0)
                    {
                        GUILIST_GetSelectedItemIndex(list_ctrl_id,select_item_ptr,mark_num);
                        if (MMIBRW_RemoveBookmark(select_item_ptr[0], FALSE))
                        {
                            GUILIST_RemoveItem(list_ctrl_id,select_item_ptr[0]);
                            mark_num--;
                        }
                        else
                        {
                            SCI_TRACE_LOW("[BRW] BrwDeleteSelectedBookmark FAIL 2");
                            break;
                        }
                    }      
                    
                    SCI_FREE(select_item_ptr);
                    GUILIST_SetCurItemIndex(list_ctrl_id, old_item_index); 
                    delete_result = (mark_num > 0)? FALSE: TRUE;
                }
                else
                {
                    SCI_TRACE_LOW("[BRW] BrwDeleteSelectedBookmark ALLOC FAIL!");
                }
#endif
            }
            
            //exit the marked statuc
#ifdef MMI_PDA_SUPPORT
            BrwExitBookmarkListMarked(list_ctrl_id);
#else        
            GUILIST_SetTextListMarkable(list_ctrl_id,FALSE);//MS00186085
#endif
            
            if(delete_result)
            {
#ifdef MMI_GUI_STYLE_TYPICAL
                BrwOpenAlertWinByTextId(PNULL,TXT_COMPLETE,TXT_NULL,IMAGE_PUBWIN_SUCCESS,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
#endif /* MMI_GUI_STYLE_TYPICAL */
            }
            else
            {
                uint16  current_index = 0;//the first
                BrwUpdateBKListWindow(PNULL, &current_index);
                BrwOpenAlertWinByTextId(PNULL,TXT_COMMON_DEL_FAIL,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            }

            MMK_CloseWin(win_id);
        }        
        break;
        
    default:
        result = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }
    return result;
}

#ifdef SNS_SUPPORT           
/*****************************************************************************/
//  Description : handle wheterh install app
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E  HandleBROWSERBookMarkWhetherInstallWinMsg(
                                                               MMI_WIN_ID_T		win_id, 	
                                                               MMI_MESSAGE_ID_E	msg_id,
                                                               DPARAM				param
                                                               )
{
    int            widget_id       = 0;
    MMI_RESULT_E   recode          = MMI_RESULT_TRUE;
    MMIPUB_INFO_T *pubwin_info_ptr = PNULL;
    
    pubwin_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
    
    if(PNULL == pubwin_info_ptr)
    {
        return MMI_RESULT_FALSE;
    }
    
    widget_id = pubwin_info_ptr->user_data;
    
    switch(msg_id)
    {
    case MSG_APP_WEB:
    case MSG_APP_OK:
        {
            IMMISNS_CreateObject(&pMe);
            
            if(pMe)
            {
                IMMISNS_AppInstall(pMe, widget_id);
                
                IMMISNS_Release(pMe);
                pMe = PNULL;
            }
            break;
        }
    case MSG_APP_CANCEL:
        break;
    default:
        recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description:handle the SNS option menu item
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
LOCAL void BrwHandleSnsOptMenu(MMI_STRING_T *url_str_ptr, MMI_MENU_ID_T menu_id)
{
    int share_id = 0;
    
    if(ID_POPUP_MENU_BRW_SHARE_TO_SINA == menu_id)
    {
        share_id = MMI_SNS_SINA;
    }
    else if(ID_POPUP_MENU_BRW_SHARE_TO_FACEBOOK == menu_id)
    {
        share_id = MMI_SNS_FACEBOOK;
    }
    else
    {
        share_id = MMI_SNS_TIWTTER;
    }
    
    IMMISNS_CreateObject(&pMe);
    
    if(pMe)
    {
        if(IMMISNS_IsFixAppInstalled(pMe, share_id) < 0)
        {
            IMMISNS_Release(pMe);
            pMe = PNULL;
            
            MMIPUB_OpenAlertWinByTextIdEx(MMK_GetFirstAppletHandle(),PNULL,TXT_WHETHER_INSTALL,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_OKCANCEL,HandleBROWSERBookMarkWhetherInstallWinMsg, share_id);
        }
        else
        {
            if (PNULL != url_str_ptr->wstr_ptr)
            {
                uint32        time_num    = 1000;
                IMMISNS_ShareToSns(pMe, share_id, SNS_MODULE_BROWSER, (unsigned long int)url_str_ptr);
                MMIPUB_OpenAlertWinByTextId(&time_num, TXT_COMMON_WAITING,TXT_NULL,IMAGE_PUBWIN_WAIT,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            }
        }
    }
}
#endif

/*****************************************************************************/
//  Description:update the bookmark list to root and normal status
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_UpdateBkListToRoot(void)
{
    if (!MMK_IsOpenWin(MMIBROWSER_BOOKMARK_WIN_ID))
    {
        return;
    }

    s_is_append_when_paint = TRUE;//append when fullpaint in bookmark list window

#ifndef MMIDORADO_BOOKMARK_BY_NV
    //reset to root and normal status
    s_pre_path_index = 0;
    s_is_in_bookmark_moving = FALSE;
#endif

#ifdef MMI_PDA_SUPPORT
    BrwExitBookmarkListMarked(MMIBROWSER_LIST_BOOKMARK_CTRL_ID);
#else
    GUILIST_SetTextListMarkable(MMIBROWSER_LIST_BOOKMARK_CTRL_ID,FALSE);
#endif
    GUIWIN_SetTitleButtonTextId(MMIBROWSER_BOOKMARK_WIN_ID, GUITITLE_BUTTON_RIGHT, STXT_OPTION, FALSE);
}

/*****************************************************************************/
//  Description : Tidy all bookmark windows
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
LOCAL void BookmarkTidyExistedBkWin(void)
{
    MMI_WIN_ID_T win_id = MMI_BRW_BEGIN_WIN_ID;

    for (win_id = MMIBROWSER_BOOKMARK_START_WIN_ID + 1; win_id <  MMIBROWSER_BOOKMARK_END_WIN_ID; win_id++)
    {
#ifdef BROWSER_START_PAGE_THEME1
        if (MMK_IsOpenWin(MMIBROWSER_START_PAGE_WIN_ID)
            && ((MMIBROWSER_BK_AND_HISTORY_WIN_ID == win_id) || (MMIBROWSER_BOOKMARK_WIN_ID == win_id)))
        {
            //don't close the window
        }
        else
#endif
        {
            MMK_CloseWin(win_id);
        }           
    }
}

/*****************************************************************************/
//  Description : open the bookmark edit window of browser
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_OpenBookmarkEditWin(MMIBRW_BM_CHANGE_TYPE_E change_type)
{
#ifndef MMIDORADO_BOOKMARK_BY_NV
    MMIBRW_ChangeBookmarkWin(change_type);
#else    
    MMK_CreateWin((uint32*)MMIBROWSER_BOOKMARK_EDIT_WIN_TAB, (ADD_DATA)change_type);

#ifdef BROWSER_START_PAGE_THEME1
    if((MMIBRW_BM_CHANGE_ADD_CUR_PAGE == change_type) || (MMIBRW_BM_CHANGE_ADD_URL == change_type))
    {
        MMIBROWSER_UpdateBkListToRoot();
    }
#endif

#endif
}

#ifndef MMI_BROWSER_MINI_SUPPORT
/*****************************************************************************/
//  Description:exit marked status for bookmark list
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_BkExitMarked(void)
{

    if (!MMK_IsOpenWin(MMIBROWSER_BOOKMARK_WIN_ID))
    {
        return;
    }
    
    if (GUILIST_GetListState(MMIBROWSER_LIST_BOOKMARK_CTRL_ID, GUILIST_STATE_ENABLE_MARK))
    {
#ifdef MMI_PDA_SUPPORT
        BrwExitBookmarkListMarked(MMIBROWSER_LIST_BOOKMARK_CTRL_ID);
#else
        GUILIST_SetTextListMarkable(MMIBROWSER_LIST_BOOKMARK_CTRL_ID,FALSE);
#endif
        GUIWIN_SetTitleButtonTextId(MMIBROWSER_BOOKMARK_WIN_ID, GUITITLE_BUTTON_RIGHT, STXT_OPTION, FALSE);
    }
}
#endif
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif


/*Edit by script, ignore 9 case. Fri Apr 27 09:38:48 2012*/ //IGNORE9527
