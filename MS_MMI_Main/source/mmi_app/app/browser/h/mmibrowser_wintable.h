/*****************************************************************************
** File Name:      mmibrowser_wintable.c                                     *
** Author:          li.li                                                    *
** Date:           23/06/2009                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the browser window table    *
*****************************************************************************/
#ifndef BROWSER_WINTABLE_H
#define BROWSER_WINTABLE_H

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h" 
#include "mmi_module.h"
#include "mmi_position.h"
#include "mmidisplay_data.h"
#include "guilistbox.h"
#include "brw_control_api.h"
#include "mmipub.h"
#include "mmi_signal_ext.h"
#include "mmifmm_export.h"
#include "mmibrowser_setting.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/    
/*这里的relative指的是anima和label相对在address bar图片上的坐标,不是绝对坐标。
    目的是减少横竖屏切换时的坐标数量*/
/*MMIBROWSER_LABEL_RELATIVE_LEFT是指相对animal的right的距离
    请不要小于2-->为IMAGE_BROWSER_LABEL_BG的显示留间隔*/    
#if (defined MAINLCD_SIZE_128X160) || (defined MAINLCD_SIZE_176X220)
#define MMIBROWSER_ANIMA_RELATIVE_LEFT		            0
#define MMIBROWSER_LABEL_RELATIVE_LEFT		            3
#define MMIBROWSER_LABEL_RELATIVE_RIGHT		         2
#elif defined MAINLCD_SIZE_240X320
#define MMIBROWSER_ANIMA_RELATIVE_LEFT		            0
#define MMIBROWSER_LABEL_RELATIVE_LEFT		            3
#define MMIBROWSER_LABEL_RELATIVE_RIGHT		         4
#elif defined MAINLCD_SIZE_320X480
#define MMIBROWSER_ANIMA_RELATIVE_LEFT		            0
#define MMIBROWSER_LABEL_RELATIVE_LEFT		            3
#define MMIBROWSER_LABEL_RELATIVE_RIGHT		         6
#elif defined MAINLCD_SIZE_240X400
#define MMIBROWSER_ANIMA_RELATIVE_LEFT		            0
#define MMIBROWSER_LABEL_RELATIVE_LEFT		            3
#define MMIBROWSER_LABEL_RELATIVE_RIGHT		         4
#else
#error
#endif

#ifdef MAINLCD_SIZE_320X480
#define MMIBROWSER_STARTPAGE_FORM1_HEIGHT       (40)
#else
#define MMIBROWSER_STARTPAGE_FORM1_HEIGHT       (36)
#endif

#define MMIBROWSER_DL_PROGRESS_MARGIN   6//下载进度条相对两边的边距


#define MMIBROWSER_NUM_TEMP_LEN                 10//@fen.xie MS00204059
#define MMIBROWSER_TEMP_LEN                     30

#define MMIBROWSER_ITEM_HIGH                    30
#define MMIBROWSER_ADDRESS_BAR_COLOR            0xFD49//0xFE64//0xF4C0
#define MMIBROWSER_ONE_K                        1024

#define WIN_ID_DEF(win_id)          win_id

#define MMIBRW_MODULE_ID MMI_MODULE_MMIBROWSER


//完整文件名的最大长度("*.*"不包括路径)
#define MMIBROWSER_DL_FILE_FULL_PATH_MAX_LEN            127

//文件名后缀的最大长度
#define MMIBROWSER_DL_SUFFIX_MAX_LEN  12

#define MMIBROWSER_DL_EXT_NAME_MAX_LEN             (1 + MMIBROWSER_DL_SUFFIX_MAX_LEN)//".*"

//文件名字的最大长度
#define MMIBROWSER_DL_FILE_NAME_MAX_LEN    (MMIBROWSER_DL_FILE_FULL_PATH_MAX_LEN - MMIBROWSER_DL_EXT_NAME_MAX_LEN)

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef enum
{
    MMI_BRW_BEGIN_WIN_ID = (MMI_MODULE_MMIBROWSER<< 16),                   // begin

#include "mmibrowser_id.def"

	MMIBROWSER_WINDOW_ID_NUMBER          // 窗口ID的总数目
}MMIBROWSER_WINDOW_ID_E;

#ifdef BROWSER_BOOKMARK_HISTORY_TAB
//bookmark and history Table包含的应用类型
typedef enum
{
    MMIBROWSER_TAB1_FIRST = 0,
    MMIBROWSER_TAB1_BOOKMARK = MMIBROWSER_TAB1_FIRST,
    MMIBROWSER_TAB1_HISTORY,
    MMIBROWSER_TAB1_MAX,
}MMIBROWSER_TAB1_TYPE_E;
#endif
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : entry the input url window
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_InputUrl(uint8* original_url_ptr);

/*****************************************************************************/
//  Description : open the history list window
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_OpenHisListWin(void);

/*****************************************************************************/
//  Description : open the bookmark window
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_OpenBookmarkWin(void);

/*****************************************************************************/
//  Description : set bookmark's mark list gray
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_BookmarkSetGrayListOptMenu(MMI_CTRL_ID_T list_ctrl_id);

/*****************************************************************************/
//  Description : open the snapshot list window
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_OpenSnapshotListWin(void);

/*****************************************************************************/
//  Description : open the snapshot save file window
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_OpenSnapshotSaveFileWin(void);

#if 0 /* Reduce Code size */
/*****************************************************************************/
//  Description : open the snapshot save media file window
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_OpenSnapshotSaveMediaFileWin(void);
#endif /* Reduce Code size */

/*****************************************************************************/
//  Description : open file detail window
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_OpenFileDetailWin(void);

/*****************************************************************************/
//  Description : clear the window of browser
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_ClearClientLcd(GUI_COLOR_T back_color);

/*****************************************************************************/
//  Description : entry the edit file name window
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
//PUBLIC void MMIBROWSER_InputFileName(void);

/*****************************************************************************/
//  Description : update the display url in address bar
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_UpdateAddrLable(wchar* url_ptr, BOOLEAN is_update);

/*****************************************************************************/
//  Description : add two line list item
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_AppendTwoLineTextListItem(
                                                     MMI_CTRL_ID_T       ctrl_id,
                                                     MMI_TEXT_ID_T       left_softkey_id,
                                                     MMI_STRING_T        string_1,
                                                     MMI_STRING_T        string_2,
                                                     MMI_IMAGE_ID_T      image,
                                                     uint16              pos,
                                                     BOOLEAN             is_update
                                                     );

/*****************************************************************************/
//  Description : open the setting window
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_OpenSettingWin(void);

/*****************************************************************************/
//  Description : set profile
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC BRW_PROFILE_T MMIBROWSER_SetProfile(void);

/*****************************************************************************/
//  Description : set profile for BRW_SCREEN_PROFILE
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_GetScreenProfile(BRW_SCREEN_OPTION_T *screen_opt_ptr);

/*****************************************************************************/
//  Description : display address bar(address bar bg, url label bg, process, label ctrl, animal ctrl)
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_DisplayAdressBar(uint32 pos, uint32 totalsize);

/*****************************************************************************/
//  Description : display progress(loading webpage)
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_DisplayProcess(uint32 pos, uint32 totalsize, MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : set full lcd color
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_BrwSetFullLcd(GUI_COLOR_T back_color);

/*****************************************************************************/
//  Description : for stop loading
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_StopLoading(void);

/*****************************************************************************/
//  Description : Tidy all windows except Mainmenu window
//  Global resource dependence : 
//  Author:fen.xie
//  Note: MS00192555
/*****************************************************************************/
PUBLIC void MMIBROWSER_TidyExistedWin(void);

/*****************************************************************************/
//  Description : 当stop web 的UI显示
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetToStop(void);

/*****************************************************************************/
//  Description : 当stop时,reset some params
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetParamToStop(void);

/*****************************************************************************/
//  Description : 设置access web的相关参数,UI显示
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetToAccess(
                    const char *url_ptr, //[IN]:access url
                    BOOLEAN is_update//[IN]:是否需要刷新当前的address bar
                    );

/****begin:封装mmi pub query window的API的原因,是为了browser的重入时能及时关闭browser模块的所有窗口;
因此browser模块内使用query pub接口,必须调用以下几个;未封装MMIPUB_OpenQuery的所有api,需要可以再扩展****/
/*****************************************************************************/
//  Description : 打开browser的query window by text ptr
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_OpenQueryWinByTextPtr(
                                         MMI_STRING_T               *text1_ptr,          //text 1,no default TXT_NULL
                                         MMI_STRING_T               *text2_ptr,          //text 1,no default TXT_NULL
                                         MMI_IMAGE_ID_T             image_id,           //alert image id
                                         MMI_WIN_ID_T               *win_id_ptr,  //win id must be in browser module,default is MMIBROWSER_QUERY_WIN_ID
                                         MMIPUB_HANDLE_FUNC         handle_func         //handle function
                                         );
/*****************************************************************************/
//  Description :  打开browser的query window by text id
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_OpenQueryWinByTextId(
                                        MMI_TEXT_ID_T              text_id,           //text 1,no default TXT_NULL
                                        MMI_IMAGE_ID_T             image_id,           //alert image id
                                        MMI_WIN_ID_T               *win_id_ptr,  //win id must be in browser module,default is MMIBROWSER_QUERY_WIN_ID
                                        MMIPUB_HANDLE_FUNC         handle_func         //handle function
                                        );
/*****************************************************************************/
//  Description : close browser query windows
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_CloseQuerytWin(
                                         MMI_WIN_ID_T               *win_id_ptr  //win id must be in browser module,default is MMIBROWSER_QUERY_WIN_ID
                                         );
/****end:封装mmi pub query window的API的原因,是为了browser的重入时能及时关闭browser模块的所有窗口;
因此browser模块内使用query pub接口,必须调用以上几个;未封装MMIPUB_OpenQuery的所有api,需要可以再扩展****/

/*****************************************************************************/
//  Description : stop loading and enter a new valid url to browser;if url is invalid,alert ......
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_EnterNewUrl(uint8* url_ptr, //[IN]new url buf
                                BOOLEAN is_need_check_url//[IN]TRUE:需判断url有效性
                                );

/*****************************************************************************/
//  Description : 停止下载后设置相应的参数
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetParaForStopDL(void);

/*****************************************************************************/
//  Description : 停止下载文件
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetToStopDL(void);

/*****************************************************************************/
//  Description : check if main browser is the focused win
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_IsMainActive(void);

/*****************************************************************************/
//  Description : get default UA profile(first according to different network)
//  Global resource dependence : 
//  Author: Sally.He
//  Note:  modify by fen.xie--Add return vaule
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_GetDefaultUserAgent(uint8** user_agent_pptr);

/*****************************************************************************/
//  Description : 提示URL无效
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIBROWSER_AlertUrlInvalid(void);

/*****************************************************************************/
//  Description : 删除未下载完毕(下载失败或用户取消)的目标文件
//  Global resource dependence : 
//  Author: fen.xie
//  Note:MS00233946
/*****************************************************************************/
PUBLIC void MMIBROWSER_DelInvalidDlFile(void);

/*****************************************************************************/
//  Description : stop download
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIBROWSER_StopDownload(void);

#ifdef BROWSER_SEARCH_SUPPORT
/*****************************************************************************/
//  Description : open the search window
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_OpenSearchWin(void);
#endif

/*****************************************************************************/
//  Description : init the reference infomation of brower position
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_OpenStartPageWin(void);

 /*****************************************************************************/
//  Description : open the web window
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_OpenWebWin(void);

/*****************************************************************************/
//  Description : open wait window for open one snapshot file
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_OpenSnapshotWaitingWin(void);

/*****************************************************************************/
//  Description : open the sim select window of browser
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_OpenSimSelectWin(void);

/*****************************************************************************/
//  Description : open the save media file window for enter file name and path to load
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_OpenSaveMediaFileWin(BRW_MIME_TYPE_E mime_type,ADD_DATA add_data_ptr);

/*****************************************************************************/
//  Description : close the bookmark window
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_CloseBookmarkWin(void);

/*****************************************************************************/
//  Description : close history list window
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_CloseHisListWin(void);

#ifdef BROWSER_BOOKMARK_HISTORY_TAB
/*****************************************************************************/
// 	Description : MMIBROWSER_OpenBkHistoryWindow
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_OpenBkHistoryWindow(MMIBROWSER_TAB1_TYPE_E tab_child_type, ADD_DATA add_data_ptr);

/*****************************************************************************/
//  Description : open the bookmark child window
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_CreateChildBookmarkWin(MMI_WIN_ID_T parent_win_id, ADD_DATA add_data_ptr);

/*****************************************************************************/
//  Description : open the history child window
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_CreateChildHistoryWin(MMI_WIN_ID_T parent_win_id, ADD_DATA add_data_ptr);

#endif

/*****************************************************************************/
//  Description : set public profile
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BRW_PUBLIC_PROFILE_T MMIBROWSER_SetPublicProfile(void);

/*****************************************************************************/
//  Description : free the alloc data of public profile
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 与MMIBROWSER_SetPublicProfile配套使用
/*****************************************************************************/
PUBLIC void MMIBROWSER_FreePublicProfile(BRW_PUBLIC_PROFILE_T profile);

/*****************************************************************************/
//  Description : set profile
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BRW_PROFILE_T MMIBROWSER_SetFixHtmlProfile(MMI_WIN_ID_T	win_id);

#ifdef MMIDORADO_MY_NAV_SUPPORT
/*****************************************************************************/
//  Description : open the edit window to add one my navigation item
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_Add2MyNav(
                MMI_STRING_T    title_text_str,
                MMI_STRING_T    url_text_str
                );
#endif

/*****************************************************************************/
//  Description : query for exit the browser
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_QueryExit(void);

/*****************************************************************************/
//  Description :update the history list(remove all and re-append)
//  Global resource dependence : 
//  Author:fen.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIBROWSER_UpdateHistoryList(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description:update the bookmark list to root and normal status
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_UpdateBkListToRoot(void);

#ifdef BROWSER_START_PAGE_THEME1
/*****************************************************************************/
//  Description : get s_is_history_need_update
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_GetIsHistoryUpdate(void);

/*****************************************************************************/
//  Description : set s_is_history_need_update
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetIsHistoryUpdate(BOOLEAN is_need_update);
#endif

/*****************************************************************************/
//  Description : Delete History
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_DeleteAllHistory(void);

#ifdef BROWSER_START_PAGE_THEME1
/*****************************************************************************/
//  Description : Set Side
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetSlide(BOOLEAN is_slide);

/*****************************************************************************/
//  Description : release the Side in browser
//  Global resource dependence : s_startpage_win_slide_handle
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_ReleaseSlide(void);
#endif

#ifdef MMIDORADO_FIX_HMTL_SUPPORT
/*****************************************************************************/
//  Description : MMIBROWSER_InitFixInfo
//  Global resource dependence : s_fix_display_info
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_InitFixInfo(void);
#endif

/*****************************************************************************/
//  Description:exit marked status for bookmark list
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_BkExitMarked(void);

#ifdef SNS_SUPPORT
PUBLIC void MMIBROWSER_Query_To_Share(uint32 share_id);
#endif
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
