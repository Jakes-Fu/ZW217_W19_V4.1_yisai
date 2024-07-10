/****************************************************************************
** File Name:      mmiwww_wintab.c                                            *
** Author:                                                                    *
** Date:           06/15/2009                                                *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:    This file is used to describe the window for WWW Browser            *
*****************************************************************************
**                         Important Edit History                            *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                *
** 06/2009        wancan.you        Create
** 
****************************************************************************/
#define _MMIWWW_WINTAB_C_  

/**-------------------------------------------------------------------------*
**                         Dependencies                                     *
**--------------------------------------------------------------------------*/
#include "mmi_app_www_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
#include "mmk_app.h" 
#include "mmi_text.h"
#include "guiedit.h"
#include "guisoftkey.h"
#include "mmiwww_text.h"
#include "mmiwww_image.h"
#include "mmiwww_id.h"
#include "mmiwww_internal.h"
#include "mmiwww_export.h"
#include "mmiwww_netfront.h"
#include "mmidisplay_data.h"
#include "mmiwww_menutab.h"
#include "guilabel.h"
#include "guibutton.h"
#include "mmiwww_position.h"
#include "guilistbox.h"
#include "mmipub.h"
#include "mmi_resource.h"
#include "mmi_nv.h"
#include "mmipub.h"
#include "guimenu.h"
#include "guirichtext.h"
#include "gui_ucs2b_converter.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmiwww_func.h"
#include "guirichtext.h"
#include "guitab.h"
#include "mmisms_export.h"
#include "mmi_appmsg.h"
#include "guires.h"
#include "mmidl_export.h"
#include "mmiphone_export.h"
#include "guitext.h"
#include "mmipdp_export.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "guidropdownlist.h"
#include "mmimms_export.h"
#ifdef DL_SUPPORT
#include "mmidl_export.h"
#include "mmidl_text.h"
#endif
#ifdef JAVA_SUPPORT
#include "mmijava_export.h"
#endif
//#include "javacall_lifecycle.h"
#include "caf.h"
#include "mmidm_export.h"
#include "mmiidle_export.h"
#include "mmifmm_image.h"
#include "mmifmm_text.h"
#ifdef MMI_AUTOTEST_SUPPORT
#include "mmi_autotest.h"
#endif
#include "mmicc_export.h"
#include "mmiwww_bookmark.h"
#include "guiform.h"
#include "guictrl_api.h"
#include "mmistk_export.h"
#include "mmistk_text.h"
#ifdef MMI_WIFI_SUPPORT
#include "mmiwifi_export.h"
#endif
#if defined(MMI_UPDATE_TIME_SUPPORT)
#include "mmiaut_export.h"
#endif
//#include "mmiset.h"
#include "mmiset_export.h"
#ifdef BROWSER_SUPPORT_NETFRONT
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMIWWW_INLINE_IME_DEFAULT_INPUT (100)
#define MMIWWW_BOOKMARK_TITLE_INDEX   (1)
#define MMIWWW_BOOKMARK_URL_INDEX     (3)
//另存为窗口中可编辑条目的索引定义
#define MMIWWW_SAVEAS_FILENAME_INDEX    (1)  
#define MMIWWW_SAVEAS_FILEPATH_INDEX    (3)

#define MMIWWW_GPRS_FLOW_RATE_LEFT          5
#define MMIWWW_ITEM_HIGH                    30
#define MMIWWW_ONE_K_LEN                    1024
#define MMIWWW_DSP_STR_LEN                  16

#define MMIWWW_AUTH_USER_LEN  20
#define MMIWWW_AUTH_PASSWORD_LEN  20

#define MMIWWW_AUTH_TITLE_LEFT  0
#define MMIWWW_AUTH_TITLE_TOP  0
#define MMIWWW_AUTH_TITLE_RIGHT  (MMI_MAINSCREEN_RIGHT_MAX_PIXEL - MMIWWW_AUTH_TITLE_LEFT)
#define MMIWWW_AUTH_TITLE_BOTTOM  (MMIWWW_AUTH_TITLE_TOP + MMI_TITLE_HEIGHT - 1)

#define MMIWWW_AUTH_ACCOUNT_LEFT  (MMIWWW_AUTH_TITLE_LEFT + 8)
#define MMIWWW_AUTH_ACCOUNT_TOP  (MMIWWW_AUTH_TITLE_BOTTOM + 10)
#define MMIWWW_AUTH_ACCOUNT_RIGHT  (MMIWWW_AUTH_TITLE_RIGHT - 8)
#define MMIWWW_AUTH_ACCOUNT_BOTTOM  (MMIWWW_AUTH_ACCOUNT_TOP + MMIWWW_LABEL_HEIGHT - 1)

#define MMIWWW_AUTH_ACCOUNT_VALUE_LEFT  MMIWWW_AUTH_ACCOUNT_LEFT
#define MMIWWW_AUTH_ACCOUNT_VALUE_TOP  (MMIWWW_AUTH_ACCOUNT_BOTTOM + 10)
#define MMIWWW_AUTH_ACCOUNT_VALUE_RIGHT  MMIWWW_AUTH_ACCOUNT_RIGHT
#define MMIWWW_AUTH_ACCOUNT_VALUE_BOTTOM  (MMIWWW_AUTH_ACCOUNT_VALUE_TOP + MMIWWW_LABEL_HEIGHT - 1)

#define MMIWWW_AUTH_PASSWORD_LEFT  MMIWWW_AUTH_ACCOUNT_LEFT
#define MMIWWW_AUTH_PASSWORD_TOP  (MMIWWW_AUTH_ACCOUNT_VALUE_BOTTOM + 10)
#define MMIWWW_AUTH_PASSWORD_RIGHT  MMIWWW_AUTH_ACCOUNT_RIGHT
#define MMIWWW_AUTH_PASSWORD_BOTTOM  (MMIWWW_AUTH_PASSWORD_TOP + MMIWWW_LABEL_HEIGHT - 1)

#define MMIWWW_AUTH_PASSWORD_VALUE_LEFT  MMIWWW_AUTH_ACCOUNT_LEFT
#define MMIWWW_AUTH_PASSWORD_VALUE_TOP  (MMIWWW_AUTH_PASSWORD_BOTTOM + 10)
#define MMIWWW_AUTH_PASSWORD_VALUE_RIGHT  MMIWWW_AUTH_ACCOUNT_RIGHT
#define MMIWWW_AUTH_PASSWORD_VALUE_BOTTOM  (MMIWWW_AUTH_PASSWORD_VALUE_TOP + MMIWWW_LABEL_HEIGHT - 1)
#define MMIWWW_SHOTCUTMENU_NAME_MAX (50)
#define MMIWWW_TOOLBOX_VERTICAL_MARGIN (10)
#define MMIWWW_TOOLBOX_HORIZONTAL_MARGIN (19)

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef enum
{
    MMIWWW_SHOTCUT_HORIZONTAL_DISPLAY,
    MMIWWW_SHOTCUT_PAGEUP,
    MMIWWW_SHOTCUT_REFRESH,
    MMIWWW_SHOTCUT_PAGE_BACK,
    MMIWWW_SHOTCUT_FULL_DISPLAY,
    MMIWWW_SHOTCUT_PAGE_FORWARD,
    MMIWWW_SHOTCUT_ZOOMOUT,
    MMIWWW_SHOTCUT_PAGEDOWN,
    MMIWWW_SHOTCUT_ZOOMIN,
    MMIWWW_SHOTCUT_SWITCH_PAGE,
    MMIWWW_SHOTCUT_SWITCH_WINDOW,

    MMIWWW_SHOTCUT_MAX
}MMIWWW_SHOTCUT_E;

typedef struct
{
    uint16 array_index;
    FILEARRAY_TYPE_E type;
}WWW_BOOKMARK_OPT_INFO_T;

/**--------------------------------------------------------------------------*
 **                          EXTERN DECLARE                                *
 **--------------------------------------------------------------------------*/
extern const wchar g_bookmark_file_suffix[];

/**-------------------------------------------------------------------------*
**                          GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL WWW_BOOKMARK_OPT_INFO_T s_bookmark_cur_index_info = {0};//the info that is handled for bookmark option window
LOCAL BOOLEAN s_is_in_bookmark_moving = FALSE;//是否处于书签的moving状态
LOCAL FILEARRAY_DATA_T s_bookmark_move_array_data = {0};//当前被move的书签的file array data

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : handle message of  main menu of browser
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWMainMenuWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
// Description : Handle query allow access network win
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E mmiwww_HandleNetAccess(MMI_WIN_ID_T      win_id, 
                                          MMI_MESSAGE_ID_E  msg_id, 
                                          DPARAM            param);

/*****************************************************************************/
// Description : Handle query allow access pagememo
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E mmiwww_HandleNetAccessPagememo(MMI_WIN_ID_T      win_id, 
                                          MMI_MESSAGE_ID_E  msg_id, 
                                          DPARAM            param);

/*****************************************************************************/
// Description : Handle query delet history
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWWW_HandleDelHistory(MMI_WIN_ID_T      win_id, 
                                          MMI_MESSAGE_ID_E  msg_id, 
                                          DPARAM            param);

/*****************************************************************************/
//  Description : handle message of browser
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWBrowserWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
// Description : mmiwww_DrawPageTitleBG
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL void mmiwww_DrawPageTitleBG(MMI_WIN_ID_T  win_id);

/*****************************************************************************/
// Description : Draw secure icon
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
LOCAL void mmiwww_DrawSecureIcon(MMI_WIN_ID_T  win_id);

/*****************************************************************************/
// Description : Draw Button BG
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
LOCAL void mmiwww_DrawToolButtonBG(MMI_WIN_ID_T  win_id);

/*****************************************************************************/
// Description : Handle open win msg for browser win
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E mmiwww_HandleBrowserOpenWin(MMI_WIN_ID_T  win_id);

/*****************************************************************************/
// Description : Handle full paint msg for browser win
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E mmiwww_HandleBrowserFullPaint(MMI_WIN_ID_T win_id, DPARAM param);

/*****************************************************************************/
// Description : Handle page load msg for browser win
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E mmiwww_HandleBrowserLoadmsg(MMI_WIN_ID_T      win_id,
                                               MMI_MESSAGE_ID_E  msg_id, DPARAM param);

/*****************************************************************************/
// Description : Handle App Cancel msg for browser win
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E mmiwww_HandleBrowserCancel(MMI_WIN_ID_T      win_id,
                                              MMI_MESSAGE_ID_E  msg_id);

/*****************************************************************************/
// Description : Handle App OK msg for browser win
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E mmiwww_HandleBrowserOK(MMI_WIN_ID_T  win_id);

/*****************************************************************************/
// Description : Handle MSG_TP_PRESS_DOWN msg for browser win
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E mmiwww_HandleBrowserTPDown(MMI_WIN_ID_T      win_id,
                                              MMI_MESSAGE_ID_E  msg_id,
                                              DPARAM            param);

/*****************************************************************************/
//  Description : handle messages of page option menu win
//  Global resource dependence : none
//  Author: Bill.Ji
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWPageOptMenuWinMsg(MMI_WIN_ID_T     win_id,
                                              MMI_MESSAGE_ID_E msg_id,
                                              DPARAM           param);

/*****************************************************************************/
//  Description : 设置网页选项菜单需要灰化的ID
//  Global resource dependence : 
//  Author: Bill.Ji
//  Note:
/*****************************************************************************/
LOCAL void mmiwww_SetPageOptMenuGrayed(MMI_WIN_ID_T  win_id);

/*****************************************************************************/
//  Description : Check gray
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN mmiwww_CheckMenuGrayed(MMIWWW_SHOTCUT_E shotcut_id);

/*****************************************************************************/
//  Description : Create shotcut Menu
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void mmiwww_CreateShotcutMenu(MMI_WIN_ID_T  win_id);

/*****************************************************************************/
// Description : Check zoom in/out can be set
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN mmiwww_CheckPageZoom(MMIWWW_ZOOM_MODE_E  mode);

/*****************************************************************************/
//  Description : Open new browser tab & switch to www main menu
//  Global resource dependence : 
//  Author: Bill.Ji
//  Note:
/*****************************************************************************/
LOCAL void mmiwww_OpenNewBrowserWin(MMI_WIN_ID_T  win_id);

/*****************************************************************************/
//  Description : Switch browser tab
//  Global resource dependence : 
//  Author: Bill.Ji
//  Note:
/*****************************************************************************/
LOCAL void mmiwww_SwitchBrowserTab(MMI_WIN_ID_T  win_id);

/*****************************************************************************/
//  Description : handle messages of page search win
//  Global resource dependence : none
//  Author: Bill.Ji
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWPageSearchEditWin(MMI_WIN_ID_T     win_id,
                                              MMI_MESSAGE_ID_E msg_id,
                                              DPARAM           param);

/*****************************************************************************/
//  Description : handle messages of page property win
//  Global resource dependence : 
//  Author: Bill.Ji
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWPagePropertyWin(MMI_WIN_ID_T     win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM           param);

/*****************************************************************************/
//  Description : Set page property info to text ctrl
//  Global resource dependence : 
//  Author: Bill.Ji
//  Note:
/*****************************************************************************/
LOCAL void mmiwww_SetPagePropertyInfo(MMI_WIN_ID_T  win_id);

/*****************************************************************************/
//  Description : Create page save as win
//  Global resource dependence : 
//  Author: Bill.Ji
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E mmiwww_OpenPageSaveAsWin(void);

/*****************************************************************************/
//  Description : Handle MSG_APP_OK msg for save as win
//  Global resource dependence : 
//  Author: Bill.Ji
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN mmiwww_HandleSaveAsOKWin(MMI_WIN_ID_T  win_id);

/*****************************************************************************/
//  Description : handle messages of page search win
//  Global resource dependence : none
//  Author: Bill.Ji
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWSaveAsEditWin(MMI_WIN_ID_T     win_id,
                                          MMI_MESSAGE_ID_E msg_id,
                                          DPARAM           param);

/*****************************************************************************/
//  Description : Handle JavaScript confirmation msg win
//  Global resource dependence : none
//  Author: Bill.Ji
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWJSConfirmWin(MMI_WIN_ID_T     win_id,
                                         MMI_MESSAGE_ID_E msg_id,
                                         DPARAM           param);

/*****************************************************************************/
//  Description : Handle JavaScript prompt msg win
//  Global resource dependence : none
//  Author: Bill.Ji
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWJSPromptWin(MMI_WIN_ID_T     win_id,
                                        MMI_MESSAGE_ID_E msg_id,
                                        DPARAM           param);

/*****************************************************************************/
//  Description : Handle MSG_APP_OK msg for JavaScript prompt win
//  Global resource dependence : 
//  Author: Bill.Ji
//  Note:需调用者对返回的数据进行释放
/*****************************************************************************/
LOCAL uint8 *mmiwww_HandleJSPromptOKWin(MMI_WIN_ID_T  win_id);

/*****************************************************************************/
//  Description : Init ctrl data for JavaScript prompt win
//  Global resource dependence : 
//  Author: Bill.Ji
//  Note:
/*****************************************************************************/
LOCAL void mmiwww_HandleWWWJSPromptOpenWin(MMI_HANDLE_T  win_handle,
                                           slim_char    *in_message,
                                           slim_char    *in_value);

/*****************************************************************************/
//  Description : Create JavaScript prompt edit win
//  Global resource dependence : 
//  Author: Bill.Ji
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E mmiwww_OpenJSPromptEditWin(MMI_WIN_ID_T  win_id, uint16 index);

/*****************************************************************************/
//  Description : handle messages of JSPrompt edit win
//  Global resource dependence : none
//  Author: Bill.Ji
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWJSPromptEditWin(MMI_WIN_ID_T     win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM           param);

/*****************************************************************************/
//  Description : handle messages of WMLS confire
//  Global resource dependence : none
//  Author: Kun.yu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWWMLSConfirmWin(MMI_WIN_ID_T     win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM           param);

/*****************************************************************************/
//  Description : Handle WMLS prompt msg win
//  Global resource dependence : none
//  Author: Kun.yu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWWMLSPromptWin(MMI_WIN_ID_T     win_id,
                                        MMI_MESSAGE_ID_E msg_id,
                                        DPARAM           param);

/*****************************************************************************/
//  Description : handle messages of WMLS edit win
//  Global resource dependence : none
//  Author: Kun.yu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWWMLSPromptEditWin(MMI_WIN_ID_T     win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM           param);

/*****************************************************************************/
//  Description : Init ctrl data for WMLS prompt win
//  Global resource dependence : 
//  Author: Kun.yu
//  Note:
/*****************************************************************************/
LOCAL void mmiwww_HandleWWWWMLSPromptOpenWin(MMI_HANDLE_T  win_handle,
                                           slim_char    *in_message,
                                           slim_char    *in_value);

/*****************************************************************************/
//  Description : Handle MSG_APP_OK msg for WMLS prompt win
//  Global resource dependence : 
//  Author: Kun.yu
//  Note:需调用者对返回的数据进行释放
/*****************************************************************************/
LOCAL uint8 *mmiwww_HandleWMLSPromptOKWin(MMI_WIN_ID_T  win_id);

/*****************************************************************************/
//  Description : Create WMLS prompt edit win
//  Global resource dependence : 
//  Author: Kun.yu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E mmiwww_OpenWMLSPromptEditWin(MMI_WIN_ID_T  win_id, uint16 index);

/*****************************************************************************/
//  Description : Create page option menu
//  Global resource dependence : 
//  Author: Bill.Ji
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWWW_OpenWWWPageOptMenuWin(void);

/*****************************************************************************/
//  Description : Create charset win
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWWW_OpenCharsetWin(void);

/*****************************************************************************/
//  Description : Create homepage setting win
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWWW_OpenHomepageWin(void);

/*****************************************************************************/
//  Description : Create Pointer Mode win
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWWW_OpenPointerModeWin(void);

/*****************************************************************************/
//  Description : Create page search win
//  Global resource dependence : 
//  Author: Bill.Ji
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E mmiwww_OpenPageSearchWin(void);

/*****************************************************************************/
//  Description : Change display win
//  Global resource dependence : 
//  Author: JIaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E mmiwww_HandleFullDisplay(MMI_HANDLE_T win_handle, BOOLEAN is_full);

/*****************************************************************************/
//  Description : Change display win
//  Global resource dependence : 
//  Author: JIaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E mmiwww_HandleHorizontalDisplay(MMI_HANDLE_T win_handle, BOOLEAN is_horizontal);

#if 0
/*****************************************************************************/
//  Description : Create Full display win
//  Global resource dependence : 
//  Author: Bill.Ji
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E mmiwww_OpenFullDisplayWin(void);

/*****************************************************************************/
//  Description : handle messages of page full display win
//  Global resource dependence : none
//  Author: Bill.Ji
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWFullDisplayWin(MMI_WIN_ID_T     win_id,
                                           MMI_MESSAGE_ID_E msg_id,
                                           DPARAM           param);
#endif

/*****************************************************************************/
//  Description : Create page property win
//  Global resource dependence : 
//  Author: Bill.Ji
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E mmiwww_OpenPagePropertyWin(void);

/*****************************************************************************/
//  Description : handle message of sub menu of browser
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWMainmenuOptWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : handle message of  Inline IME
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWInlineIMEWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : creat softkey control
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL void CreatSoftkeyCtrl(
                           MMI_HANDLE_T win_handle,     //win id
                           MMI_TEXT_ID_T leftsoft_id,
                           MMI_TEXT_ID_T midsoft_id,
                           MMI_TEXT_ID_T rightsoft_id
                           );


/*****************************************************************************/
//  Description : handle message of set menu of browser
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWSetMenuWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : handle message of history of browser
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWHistoryWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : handle message of pagememo
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWPageMemoWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : handle message of certificate
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWCertificateWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : handle message of match key win
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWMatchKeyWinMsg (MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : handle message of match URL win
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWMatchURLWinMsg (MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : handle message of history option of browser
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWHistoryOptWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : handle message of pagememo option of browser
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWWWPageMemoOptWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : handle message of set font of browser
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWSetWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//    Description : handle message of history item attribute
//    Global resource dependence : none
//    Author: kun.yu
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHisAttributeDetailWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//    Description : handle message of certificate item attribute
//    Global resource dependence : none
//    Author: kun.yu
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCerAttributeDetailWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : handle message of sub menu of bookmark
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWBookmarkOptWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : handle message of sub menu of bookmark detail
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWBookmarkDetailSubMenuWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : handle message of bookmark list
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWBookmarkWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : bookmark url input
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWWW_OpenInputBookmarkWin(MMIWWW_BM_CHANGE_TYPE_E change_type);

/*****************************************************************************/
//  Description : show bookmark
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWWW_ShowBookmarkWin(void);

/*****************************************************************************/
//  Description : handle message of showing bookmark
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleShowBookmarkWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : Delete bookmark
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void HandleWWWDeleteBookmark(MMI_WIN_ID_T win_id, BOOLEAN is_delete_all);

/****************************************************************************/    
//  Description :add item to richtext
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void MMIWWW_RichText_AddItem(MMI_CTRL_ID_T ctrl_id, MMI_STRING_T * item_string, GUIRICHTEXT_FRAME_TYPE_E frame_type, GUIRICHTEXT_TAG_TYPE_E tag_type);

/****************************************************************************/    
//  Description : Set tab property
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void MMIWWW_SetTabProperty(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

/****************************************************************************/    
//  Description :
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void MMIWWW_CreateToolboxFulldisplayButton(MMI_WIN_ID_T win_id);

/****************************************************************************/    
//  Description :
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void MMIWWW_SetToolboxFulldisplayButtonProperty(MMI_WIN_ID_T win_id);

/****************************************************************************/  
//  Description : Set tab Title
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void MMIWWW_SetTabTitle(MMI_CTRL_ID_T ctrl_id, DPARAM param);

/****************************************************************************/  
//  Description : Set bookmark and history list
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void MMIWWW_SetListProperty(MMI_WIN_ID_T win_id);

/****************************************************************************/    
//  Description : Set richtext
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void SetInputBookmarkDetailEntry(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, MMIWWW_BM_CHANGE_TYPE_E change_type);

/*****************************************************************************/
//  Description : handle message of bookmark input
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleInputBookmarkWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
// Description : Page back button call back func
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E www_PagePrevCallBack(void);

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
// Description : Page option button call back func
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E www_PageOptionCallBack(void);

/*****************************************************************************/
// Description : Page reload button call back func
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E www_PageReloadCallBack(void);

#if 0
/*****************************************************************************/
// Description : Page forward button call back func
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E www_PageNextCallBack(void);
#endif

/*****************************************************************************/
// Description : Switch full screen button call back func
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E www_PageFullCallBack(void);
#endif

/*****************************************************************************/
//  Description : Open Common Edit window
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void MMIWWW_OpenCommonEditWin(MMI_WINDOW_CREATE_T * win_create, GUIEDIT_INIT_DATA_T * init_data);

/*****************************************************************************/
//  Description : Handle common Edit message
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWWW_HandleCommonEditWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : Handle Center Key
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWWW_MainMenuHandleWebKey(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : Create opt of main menu
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWWW_OpenWWWMainmenuOptWin(BOOLEAN is_web_open);

/*****************************************************************************/
//  Description : Handle Open Window
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWWW_MainMenuHandleOpenWindow(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : Handle Paint
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWWW_MainMenuHandleFullPaint(MMI_WIN_ID_T win_id, DPARAM param);

/*****************************************************************************/
//  Description : Handle Tp pressed
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWWW_MainMenuHandleTPPressUp(MMI_WIN_ID_T win_id, DPARAM param);

/*****************************************************************************/
//  Description : Handle Tp Released
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWWW_MainMenuHandleTPPressDown(MMI_WIN_ID_T win_id, DPARAM param);

/*****************************************************************************/
//  Description : Change the focus of control
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWWW_MainMenuHandleDirectionKey(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id);

/*****************************************************************************/
//  Description : Create bookmark sub menu
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWWW_OpenWWWBookmarkOptWin(BOOLEAN is_web_open);

/*****************************************************************************/
//  Description : Create sub menu of bookmark detail
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWWW_OpenWWWBookmarkDetailSubMenuWin(void);

/*****************************************************************************/
//  Description : Draw Background of mainmenu
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void MMIWWW_MainMenuSetBackground(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : Set address and search label of mainmenu
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void MMIWWW_MainMenuSetLabel(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : Set link and search button of mainmenu
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void MMIWWW_MainMenuSetButton(MMI_WIN_ID_T win_id, DPARAM param);

/*****************************************************************************/
//  Description : Set softkey of mainmenu when it is focused
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void MMIWWW_MainMenuSetSoftKey(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : Display Link Button
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void MMIWWW_MainMenuDisplayLinkButton(MMI_WIN_ID_T win_id, MMIWWW_BUTTON_MODE_E button_mode);

/*****************************************************************************/
//  Description : Display Search Button
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void MMIWWW_MainMenuDisplaySearchButton(MMI_WIN_ID_T win_id, MMIWWW_BUTTON_MODE_E button_mode);

/*****************************************************************************/
//  Description : Handle link press
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWWW_MainMenuHandleSearchButtonPress(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : Handle link press
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWWW_MainMenuHandleLinkButtonPress(MMI_WIN_ID_T win_id);

/*****************************************************************************/
// Description : 显示网页加载完成动画的初始/完成状态
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL void mmiwww_LoadPageAnimPic(MMI_WIN_ID_T  win_id);

/*****************************************************************************/
// Description : 显示或停止网页加载动画
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL void mmiwww_LoadPageAnimation(MMI_WIN_ID_T  win_id, BOOLEAN  is_play);

/*****************************************************************************/
// Description : 设置网页加载进度
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL void mmiwww_SetLoadPercent(MMI_HANDLE_T  label_handle,
                                 uint8         percent);

/*****************************************************************************/
// Description : 设置网页大小信息
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL void mmiwww_SetPageSize(MMI_HANDLE_T label_handle);

/*****************************************************************************/
// Description : 切换网页模式
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL void mmiwww_SwitchImageSelectMode(void);

/*****************************************************************************/
// Description : Get browser win handle
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL MMI_HANDLE_T mmiwww_GetBrowserWinhandle(void);

/*****************************************************************************/
// Description : 设置网页标题栏
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL void mmiwww_SetPageTitle(MMI_HANDLE_T   label_handle,
                               MMI_TEXT_ID_T  text_id);

/*****************************************************************************/
// Description : Handle MSG_WWW_NOTIFY_BG_SOUND msg 
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
LOCAL void MMIWWW_Handle_BG_Sound(DPARAM param);

/*****************************************************************************/
//  Description : update WWW net setting when the index list item is selected  
//  Global resource dependence : none
//  Author: jixin.xu
//  Note:
/*****************************************************************************/
LOCAL void mmiwww_ConnectionCallback(MN_DUAL_SYS_E dual_sys, uint32 selected_index);

/*****************************************************************************/
//  Description :setting link list
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLinkListWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description :charset list
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCharsetListWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description :Pointer mode list
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePointerModeListWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description :Shotcut key menu
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWShotcutKeyWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : append certificate attribute to richtext
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
LOCAL void MMIWWW_Append_Certificate_Attribute( MMI_WIN_ID_T win_id );

/*****************************************************************************/
//  Description : add history attribute to richtext
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
LOCAL void MMIWWW_Append_History_Attribute( MMI_WIN_ID_T win_id, MMIWWW_PAGEMEMO_ATTRIBUTE_TYPE_E type );

/*****************************************************************************/
//    Description : set WWW display settings param
//    Global resource dependence : 
//    Author: kun.yu
//    Note: 
/*****************************************************************************/
LOCAL void SetWWWDisplaySetting(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//    Description : set WWW cookie settings param
//    Global resource dependence : 
//    Author: kun.yu
//    Note: 
/*****************************************************************************/
LOCAL void SetWWWCookieSetting(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//    Description : set WWW cache settings param
//    Global resource dependence : 
//    Author: kun.yu
//    Note: 
/*****************************************************************************/
LOCAL void SetWWWCacheSetting(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//    Description : set WWW webdata settings param
//    Global resource dependence : 
//    Author: kun.yu
//    Note: 
/*****************************************************************************/
LOCAL void SetWWWWebdataSetting(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : handle message of WWW Auth win
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWAuthWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

#if 0//@fen.xie
/*****************************************************************************/
//  Description : handle message of CC select
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWCCSelectWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#endif

/*****************************************************************************/
//  Description : handle message of CC select
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWHomepageSetWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
// Description : Handle replace the duplicated bookmark 
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWWW_HandleReplaceDuplicatedBookmark(MMI_WIN_ID_T      win_id, 
                                          MMI_MESSAGE_ID_E  msg_id, 
                                          DPARAM            param);

/*****************************************************************************/
//  Description :enter the bookmark moving state and back to bookmark list window to choose
//  Global resource dependence : none
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL void BookmarkEnterMoveState(MMI_WIN_ID_T win_id, uint16 array_index);

/*****************************************************************************/
//  Description : some message of  main menu of browser can't handled within bookmark moving state
//  Global resource dependence : none
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleWWWMainMenuInBMMove(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
// Description : get the bookmark filearray index that need to be handled
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
LOCAL uint16 GetBookmarkCurIndex(void);

 /*****************************************************************************/
//  Description : get bookmark's file array index from bookmark list index
//  Global resource dependence : none
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL uint16 GetBookmarkArrayIndex(uint16 cur_item_index);

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
                                       );

/*****************************************************************************/
//  Description : move and replace specified bookmark
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWWW_HandleMoveBookmark(MMI_WIN_ID_T      win_id, 
                                          MMI_MESSAGE_ID_E  msg_id, 
                                          DPARAM            param);

/*****************************************************************************/
//  Description : move one bookmark
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN  BookmarkMoveTo(
                           const wchar *full_path_ptr,     //in
                           uint16     full_path_len       //in, 双字节为单位
                           );

/*****************************************************************************/
//  Description : begin to move one bookmark
//  Global resource dependence : none
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL void BookmarkBeginMoveTo(uint16 cur_index);

/****************************************************************************/    
//  Description : show the bookmark
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void MMIWWW_SetShowBookmarkDetailEntry(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
// Description : Access homepage
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
LOCAL void mmiwww_AccessHomepage(void);

#if defined(MMI_WIFI_SUPPORT) || !defined(MMI_MULTI_SIM_SYS_SINGLE) 
/*****************************************************************************/
//  Description : handle sim select window message
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E WWWHandleSIMSelectWinMsg(
                                           MMI_WIN_ID_T	win_id, 
                                           MMI_MESSAGE_ID_E	msg_id, 
                                           DPARAM param
                                         );
#endif

/*****************************************************************************/
//  Description : handle message of Window mangaer
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWWindowManagerWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWWindowManagerOptWinMsg(MMI_WIN_ID_T	win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E mmiwww_OpenWindowManagerWin(void);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E mmiwww_OpenWindowManagerOptWin(void);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void mmiwww_DeleteSubWindow(int32 index);

#ifdef MMI_WIFI_SUPPORT
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleQueryGPRSWinMsg(
                                           MMI_WIN_ID_T	win_id, 
                                           MMI_MESSAGE_ID_E	msg_id, 
                                           DPARAM param
                                         );
#endif

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL void mmiwww_SetQueryGPRSWindowProperty(MMI_WIN_ID_T win_id);
       
/**---------------------------------------------------------------------------*
**                         Constant Variables                                *
**---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL DATA DECLARE                               */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         WINDOW  DEFINITION                               *
**--------------------------------------------------------------------------*/
//the window for www browser main menu
WINDOW_TABLE(MMIWWW_MAINMENU_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleWWWMainMenuWinMsg),
    WIN_ID(MMIWWW_MAINMENU_WIN_ID),
    WIN_STATUSBAR,
    WIN_TITLE(TXT_COMMUNICATION_BROWSER),
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_EXIT),
    //Address bar
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMIWWW_MAINMENU_LABEL_ADDRESS_CTRL_ID),
     //Search Bar
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMIWWW_MAINMENU_LABEL_SEARCH_CTRL_ID),
    //Bookmark and History Tab
    CREATE_TAB_CTRL(GUI_TAB_ITEM_LEFT,GUITAB_STATE_NONEED_SCROLL, MMIWWW_MAINMENU_TAB_CTRL_ID),
    END_WIN
};

//the window for www browser
#ifdef TOUCH_PANEL_SUPPORT
WINDOW_TABLE(MMIWWW_BROWSER_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleWWWBrowserWinMsg),
    WIN_ID(MMIWWW_BROWSER_WIN_ID),
    WIN_STATUSBAR,
    //WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_STOP),
    CREATE_ANIM_CTRL(MMIWWW_PAGE_ANIMATE_LOADING_CTRL_ID, MMIWWW_BROWSER_WIN_ID),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMIWWW_PAGE_LOAD_LABEL_CTRL_ID),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_RIGHT, MMIWWW_PAGE_PROG_LABEL_CTRL_ID),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_RIGHT, MMIWWW_PAGE_SECURE_LABEL_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAGE_WWW_PAGE_OPTION_DISABLED,MMIWWW_PAGE_OPTION_BUTTON_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAGE_WWW_PAGE_RELOAD_DISABLED,MMIWWW_PAGE_RELOAD_BUTTON_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAGE_WWW_PAGE_FULL_DISABLED, MMIWWW_PAGE_FULL_BUTTON_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAGE_WWW_PAGE_PREV_DISABLED,MMIWWW_PAGE_PREV_BUTTON_CTRL_ID),
//    CREATE_BUTTON_CTRL(IMAGE_WWW_PAGE_NEXT_DEFAULT, MMIWWW_PAGE_NEXT_BUTTON_CTRL_ID),
    END_WIN
};
#else
WINDOW_TABLE(MMIWWW_BROWSER_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleWWWBrowserWinMsg),
    WIN_ID(MMIWWW_BROWSER_WIN_ID),
    WIN_STATUSBAR,
    WIN_SOFTKEY(TXT_REFRESH, TXT_NULL, STXT_STOP),
    CREATE_ANIM_CTRL(MMIWWW_PAGE_ANIMATE_LOADING_CTRL_ID, MMIWWW_BROWSER_WIN_ID),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMIWWW_PAGE_LOAD_LABEL_CTRL_ID),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_RIGHT, MMIWWW_PAGE_PROG_LABEL_CTRL_ID),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_RIGHT, MMIWWW_PAGE_SECURE_LABEL_CTRL_ID),
    END_WIN
};
#endif

//the window for www page option menu
WINDOW_TABLE(MMIWWW_PAGE_OPTMENU_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleWWWPageOptMenuWinMsg),
    WIN_ID(MMIWWW_PAGE_OPT_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_POPMENU_CTRL(MENU_WWW_PAGE_OPT, MMIWWW_PAGE_OPTION_MENU_CTRL_ID),
    END_WIN
};

//the window for www page option menu
WINDOW_TABLE(MMIWWW_PAGE_FULL_OPTMENU_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleWWWPageOptMenuWinMsg),
    WIN_ID(MMIWWW_PAGE_OPT_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_POPMENU_CTRL(MENU_WWW_PAGE_FULL_OPT, MMIWWW_PAGE_OPTION_MENU_CTRL_ID),
    END_WIN
};

//the window for www page search
WINDOW_TABLE( MMIWWW_PAGE_SEARCH_WIN_TAB ) = 
{
    WIN_FUNC((uint32)HandleWWWPageSearchEditWin),    
    WIN_ID( MMIWWW_PAGE_SEARCH_WIN_ID ),
    WIN_TITLE(TXT_COMM_SEARCH),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_TEXT_CTRL(WWW_MAX_SEARCH_STRING_LEN,MMIWWW_PAGE_SEARCH_EDIT_CTRL_ID),
    END_WIN
};

#if 0
//the window for www page full display
WINDOW_TABLE( MMIWWW_FULL_DISPLAY_WIN_TAB ) = 
{
    WIN_FUNC((uint32)HandleWWWFullDisplayWin),    
    WIN_ID( MMIWWW_FULL_DISPLAY_WIN_ID ),
    END_WIN
};
#endif

WINDOW_TABLE( MMIWWW_PAGE_PROPERTY_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleWWWPagePropertyWin ),    
    WIN_ID( MMIWWW_PAGE_PROPERTY_WIN_ID ),
    WIN_TITLE(TXT_WWW_PAGE_PROPERTY),
    CREATE_RICHTEXT_CTRL(MMIWWW_PAGE_PROPERTY_RICHTEXT_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    END_WIN
}; 

//the window for www page save as edit
WINDOW_TABLE( MMIWWW_PAGE_SAVEAS_EDIT_WIN_TAB ) = 
{
    WIN_FUNC((uint32)HandleWWWSaveAsEditWin),    
    WIN_ID( MMIWWW_PAGE_SAVE_AS_EDIT_WIN_ID ),
    WIN_TITLE(TXT_EDIT),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

//the window for JavaScript confirmation
WINDOW_TABLE( MMIWWW_JSCONFIRM_WIN_TAB ) = 
{
    WIN_FUNC((uint32)HandleWWWJSConfirmWin),    
    WIN_ID( MMIWWW_JSCONFIRM_WIN_ID ),
    WIN_TITLE(TXT_WWW_JAVA_SCRIPT),
    CREATE_TEXT_CTRL(MMIWWW_JSCONFIRM_TEXT_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
    END_WIN
};

//the window for JavaScript prompt
WINDOW_TABLE( MMIWWW_JSPROMPT_WIN_TAB ) = 
{
    WIN_FUNC((uint32)HandleWWWJSPromptWin),    
    WIN_ID( MMIWWW_JSPROMPT_WIN_ID ),
    WIN_TITLE(TXT_WWW_JAVA_SCRIPT),
    CREATE_RICHTEXT_CTRL(MMIWWW_JSPROMPT_RICHTEXT_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_EDIT, STXT_CANCEL),
    END_WIN
};

//the window for www JavaScript prompt edit
WINDOW_TABLE( MMIWWW_JSPROMPT_EDIT_WIN_TAB ) = 
{
    WIN_FUNC((uint32)HandleWWWJSPromptEditWin),  
    WIN_ID( MMIWWW_JSPROMPT_EDIT_WIN_ID ),
    WIN_TITLE(TXT_EDIT),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

//the window for JavaScript confirmation
WINDOW_TABLE( MMIWWW_WMLSCONFIRM_WIN_TAB ) = 
{
    WIN_FUNC((uint32)HandleWWWWMLSConfirmWin),   
    WIN_ID( MMIWWW_WMLSCONFIRM_WIN_ID ),
    WIN_TITLE(TXT_WWW_WML_SCRIPT),
    CREATE_TEXT_CTRL(MMIWWW_WMLSCONFIRM_TEXT_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
    END_WIN
};

WINDOW_TABLE( MMIWWW_WMLSPROMPT_WIN_TAB ) = 
{
    WIN_FUNC((uint32)HandleWWWWMLSPromptWin),    
    WIN_ID( MMIWWW_WMLSPROMPT_WIN_ID ),
    WIN_TITLE(TXT_WWW_WML_SCRIPT),
    CREATE_RICHTEXT_CTRL(MMIWWW_WMLSPROMPT_RICHTEXT_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_EDIT, STXT_CANCEL),
    END_WIN
};

//the window for www WMLS prompt edit
WINDOW_TABLE( MMIWWW_WMLSPROMPT_EDIT_WIN_TAB ) = 
{
    WIN_FUNC((uint32)HandleWWWWMLSPromptEditWin),    
    WIN_ID( MMIWWW_WMLSPROMPT_EDIT_WIN_ID ),
    WIN_TITLE(TXT_EDIT),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

//the window for www browser sub menu1
WINDOW_TABLE(MMIWWW_MAINMENU_OPT_WIN_TAB1) = 
{
    WIN_FUNC((uint32)HandleWWWMainmenuOptWinMsg),
    WIN_ID(MMIWWW_MAINMENU_OPT_WIN_ID),
    WIN_STYLE(WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_POPMENU_CTRL(MENU_WWW_MAINMENU_OPT1, MMIWWW_POPMENU_MAINMENU_OPT_CTRL_ID),
    END_WIN
};

//the window for www browser sub menu2
WINDOW_TABLE(MMIWWW_MAINMENU_OPT_WIN_TAB2) = 
{
    WIN_FUNC((uint32)HandleWWWMainmenuOptWinMsg),
    WIN_ID(MMIWWW_MAINMENU_OPT_WIN_ID),
    WIN_STYLE(WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_POPMENU_CTRL(MENU_WWW_MAINMENU_OPT2, MMIWWW_POPMENU_MAINMENU_OPT_CTRL_ID),
    END_WIN
};

//the window for www browser set menu
WINDOW_TABLE( MMIWWW_SETMENU_WIN_TAB ) = 
{
    WIN_FUNC((uint32)HandleWWWSetMenuWinMsg),
    WIN_ID(MMIWWW_SETMENU_WIN_ID),
    WIN_TITLE(STXT_MAIN_SETTING),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_MENU_CTRL(MENU_WWW_SET_OPT, MMIWWW_MENU_SET_OPT_CTRL_ID),
    END_WIN
};

//the window for www history
WINDOW_TABLE( MMIWWW_HISTORY_WIN_TAB ) =
{
    WIN_FUNC((uint32)HandleWWWHistoryWinMsg),
    WIN_ID(MMIWWW_HISTORY_WIN_ID),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, TXT_NULL),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIWWW_LISTBOX_HISTORY_CTRL_ID),
    END_WIN
};

//the window for pagememo win
WINDOW_TABLE( MMIWWW_PAGEMEMO_WIN_TAB ) =
{
    WIN_FUNC((uint32)HandleWWWPageMemoWinMsg),
    WIN_ID(MMIWWW_PAGEMEMO_WIN_ID),
    WIN_TITLE(TXT_WWW_PAGE_MEMO),
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIWWW_LISTBOX_PAGEMEMO_CTRL_ID),
    END_WIN
};

//the window for certificate win
WINDOW_TABLE( MMIWWW_CERTIFICATE_WIN_TAB ) =
{
    WIN_FUNC((uint32)HandleWWWCertificateWinMsg),
    WIN_ID(MMIWWW_CERTIFICATE_WIN_ID),
    WIN_TITLE(TXT_WWW_CERTIFICATE),
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIWWW_LISTBOX_CERTIFICATE_CTRL_ID),
    END_WIN
};

//the window for search match key win
WINDOW_TABLE( MMIWWW_MATCHKEY_WIN_TAB ) =
{
    WIN_FUNC((uint32)HandleWWWMatchKeyWinMsg),
    WIN_ID(MMIWWW_MATCHKEY_WIN_ID),
    WIN_STATUSBAR,
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN), //禁止3D旋转
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIWWW_LISTBOX_MATCHKEY_CTRL_ID),
    END_WIN
};

//the window for search match url win
WINDOW_TABLE( MMIWWW_MATCHURL_WIN_TAB ) =
{
    WIN_FUNC((uint32)HandleWWWMatchURLWinMsg),
    WIN_ID(MMIWWW_MATCHURL_WIN_ID),
    WIN_STATUSBAR,
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN), //禁止3D旋转
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIWWW_LISTBOX_MATCHURL_CTRL_ID),
    END_WIN
};

//the window for www history option win
WINDOW_TABLE( MMIWWW_HISTORY_OPT_WIN_TAB1 ) =
{
    WIN_FUNC((uint32)HandleWWWHistoryOptWinMsg),
    WIN_ID(MMIWWW_HISTORY_OPT_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_POPMENU_CTRL(MENU_WWW_HISTORY_OPT1,MMIWWW_POPMENU_HISTORY_OPT_CTRL_ID),
    END_WIN  
};

//the window for www history option win
WINDOW_TABLE( MMIWWW_HISTORY_OPT_WIN_TAB2 ) =
{
    WIN_FUNC((uint32)HandleWWWHistoryOptWinMsg),
    WIN_ID(MMIWWW_HISTORY_OPT_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_POPMENU_CTRL(MENU_WWW_HISTORY_OPT2,MMIWWW_POPMENU_HISTORY_OPT_CTRL_ID),
    END_WIN  
};

//the window for www pagememo option win
WINDOW_TABLE( MMIWWW_PAGEMEMO_OPT_WIN_TAB ) =
{
    WIN_FUNC((uint32)HandleWWWPageMemoOptWinMsg),
    WIN_ID(MMIWWW_PAGEMEMO_OPT_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_POPMENU_CTRL(MENU_WWW_PAGEMEMO_OPT,MMIWWW_POPMENU_PAGEMEMO_OPT_CTRL_ID),
    END_WIN  
};

//the window for www history item attribute win
WINDOW_TABLE( MMIWWW_HISTORY_ATTRIBUTE_WIN_TAB ) = 
{
    WIN_TITLE(TXT_WWW_SHOW_ADDRESS),
    WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
    WIN_FUNC((uint32)HandleHisAttributeDetailWinMsg),    
    WIN_ID(MMIWWW_HISTORY_ATTRIBUTE_WIN_ID),
    CREATE_RICHTEXT_CTRL(MMIWWW_RICHTEXT_HISTORY_ATTRIBUTE_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_CONNECT_COMN, TXT_NULL, STXT_RETURN),
    END_WIN
};

//the window for www certificate attribute win
WINDOW_TABLE( MMIWWW_CERTIFICATE_ATTRIBUTE_WIN_TAB ) =
{
    WIN_TITLE(TXT_WWW_CERT_ATTRIBUTE),
    WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
    WIN_FUNC((uint32)HandleCerAttributeDetailWinMsg),    
    WIN_ID(MMIWWW_CERTIFICATE_ATTRIBUTE_WIN_ID),
    CREATE_RICHTEXT_CTRL(MMIWWW_RICHTEXT_CERTIFICATE_ATTRIBUTE_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    END_WIN
};

//the window for www browser set
WINDOW_TABLE( MMIWWW_SET_WIN_TAB ) =
{
    WIN_FUNC((uint32)HandleWWWSetWinMsg),
    WIN_ID(MMIWWW_SET_WIN_ID),
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIWWW_LISTBOX_SET_CTRL_ID),
    END_WIN    
};

WINDOW_TABLE( MMIWWW_SET_WIN_TAB1 ) =
{
    WIN_FUNC((uint32)HandleWWWSetWinMsg),
    WIN_ID(MMIWWW_SET_WIN_ID),
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(STXT_SAVE, TXT_NULL, STXT_RETURN),
	CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIWWW_SETTINGS_DISPLAY_FORM_CTRL_ID),
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWWW_SETTINGS_SCRIPT_FORM_CTRL_ID,MMIWWW_SETTINGS_DISPLAY_FORM_CTRL_ID),
	        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIWWW_SETTINGS_SCRIPT_LABLE_CTRL_ID,MMIWWW_SETTINGS_SCRIPT_FORM_CTRL_ID),
            CHILD_DROPDOWNLIST_CTRL(TRUE,MMIWWW_SETTINGS_SCRIPT_DROPLIST_CTRL_ID,MMIWWW_SETTINGS_SCRIPT_FORM_CTRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWWW_SETTINGS_ANIMAL_FORM_CTRL_ID,MMIWWW_SETTINGS_DISPLAY_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIWWW_SETTINGS_ANIMAL_LABLE_CTRL_ID,MMIWWW_SETTINGS_ANIMAL_FORM_CTRL_ID),
            CHILD_DROPDOWNLIST_CTRL(TRUE,MMIWWW_SETTINGS_ANIMAL_DROPLIST_CTRL_ID,MMIWWW_SETTINGS_ANIMAL_FORM_CTRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWWW_SETTINGS_IMAGE_FORM_CTRL_ID,MMIWWW_SETTINGS_DISPLAY_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIWWW_SETTINGS_IMAGE_LABLE_CTRL_ID,MMIWWW_SETTINGS_IMAGE_FORM_CTRL_ID),
            CHILD_DROPDOWNLIST_CTRL(TRUE,MMIWWW_SETTINGS_IMAGE_DROPLIST_CTRL_ID,MMIWWW_SETTINGS_IMAGE_FORM_CTRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWWW_SETTINGS_TABLE_FORM_CTRL_ID,MMIWWW_SETTINGS_DISPLAY_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIWWW_SETTINGS_TABLE_LABLE_CTRL_ID,MMIWWW_SETTINGS_TABLE_FORM_CTRL_ID),
            CHILD_DROPDOWNLIST_CTRL(TRUE,MMIWWW_SETTINGS_TABLE_DROPLIST_CTRL_ID,MMIWWW_SETTINGS_TABLE_FORM_CTRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWWW_SETTINGS_SOUND_FORM_CTRL_ID,MMIWWW_SETTINGS_DISPLAY_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIWWW_SETTINGS_SOUND_LABLE_CTRL_ID,MMIWWW_SETTINGS_SOUND_FORM_CTRL_ID),
            CHILD_DROPDOWNLIST_CTRL(TRUE,MMIWWW_SETTINGS_SOUND_DROPLIST_CTRL_ID,MMIWWW_SETTINGS_SOUND_FORM_CTRL_ID),
/*    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIWWW_SETTINGS_POPUP_LABLE_CTRL_ID,MMIWWW_SETTINGS_DISPLAY_FORM_CTRL_ID),
    CHILD_DROPDOWNLIST_CTRL(TRUE,MMIWWW_SETTINGS_POPUP_TYPE_NUM, GUIDNLIST_SHOW_DOWNWARD, MMIWWW_SETTINGS_POPUP_DROPLIST_CTRL_ID),*/

    END_WIN    
};

WINDOW_TABLE( MMIWWW_SET_WIN_TAB2 ) =
{
    WIN_FUNC((uint32)HandleWWWSetWinMsg),
    WIN_ID(MMIWWW_SET_WIN_ID),
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(STXT_SAVE, TXT_NULL, STXT_RETURN),
	CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIWWW_SETTINGS_CACHE_FORM_CTRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWWW_SETTINGS_CACHE_MODE_FORM_CTRL_ID,MMIWWW_SETTINGS_CACHE_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIWWW_SETTINGS_CACHE_MODE_LABLE_CTRL_ID,MMIWWW_SETTINGS_CACHE_MODE_FORM_CTRL_ID),
            CHILD_DROPDOWNLIST_CTRL(TRUE,MMIWWW_SETTINGS_CACHE_MODE_DROPLIST_CTRL_ID,MMIWWW_SETTINGS_CACHE_MODE_FORM_CTRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWWW_SETTINGS_QUERY_DISK_FORM_CTRL_ID,MMIWWW_SETTINGS_CACHE_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIWWW_SETTINGS_QUERY_DISK_LABLE_CTRL_ID,MMIWWW_SETTINGS_QUERY_DISK_FORM_CTRL_ID),
            CHILD_DROPDOWNLIST_CTRL(TRUE,MMIWWW_SETTINGS_QUERY_DISK_DROPLIST_CTRL_ID,MMIWWW_SETTINGS_QUERY_DISK_FORM_CTRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWWW_SETTINGS_CLEAR_CACHE_FORM_CTRL_ID,MMIWWW_SETTINGS_CACHE_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMIWWW_SETTINGS_CLEAR_CACHE_LABLE_CTRL_ID,MMIWWW_SETTINGS_CLEAR_CACHE_FORM_CTRL_ID),
    END_WIN
};

WINDOW_TABLE( MMIWWW_SET_WIN_TAB3 ) =
{
    WIN_FUNC((uint32)HandleWWWSetWinMsg),
    WIN_ID(MMIWWW_SET_WIN_ID),
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(STXT_SAVE, TXT_NULL, STXT_RETURN),
	CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIWWW_SETTINGS_COOKIE_FORM_CTRL_ID),
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWWW_SETTINGS_COOKIE_MODE_FORM_CTRL_ID,MMIWWW_SETTINGS_COOKIE_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIWWW_SETTINGS_COOKIE_MODE_LABLE_CTRL_ID,MMIWWW_SETTINGS_COOKIE_MODE_FORM_CTRL_ID),
            CHILD_DROPDOWNLIST_CTRL(TRUE,MMIWWW_SETTINGS_COOKIE_MODE_DROPLIST_CTRL_ID,MMIWWW_SETTINGS_COOKIE_MODE_FORM_CTRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWWW_SETTINGS_CLEAR_COOKIE_FORM_CTRL_ID,MMIWWW_SETTINGS_COOKIE_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMIWWW_SETTINGS_CLEAR_COOKIE_LABLE_CTRL_ID,MMIWWW_SETTINGS_CLEAR_COOKIE_FORM_CTRL_ID),

    END_WIN
};

WINDOW_TABLE( MMIWWW_SET_WIN_TAB4 ) =
{
    WIN_FUNC((uint32)HandleWWWSetWinMsg),
    WIN_ID(MMIWWW_SET_WIN_ID),
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(STXT_SAVE, TXT_NULL, STXT_RETURN),
	CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIWWW_SETTINGS_WEBDATA_FORM_CTRL_ID),
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWWW_SETTINGS_WEBDATA_FORM1_CTRL_ID,MMIWWW_SETTINGS_WEBDATA_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIWWW_SETTINGS_WEBDATA_LABLE_CTRL_ID,MMIWWW_SETTINGS_WEBDATA_FORM1_CTRL_ID),
            CHILD_DROPDOWNLIST_CTRL(TRUE,MMIWWW_SETTINGS_WEBDATA_DROPLIST_CTRL_ID,MMIWWW_SETTINGS_WEBDATA_FORM1_CTRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWWW_SETTINGS_CLEAR_WEBDATA_FORM_CTRL_ID,MMIWWW_SETTINGS_COOKIE_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMIWWW_SETTINGS_CLEAR_WEBDATA_LABLE_CTRL_ID,MMIWWW_SETTINGS_CLEAR_WEBDATA_FORM_CTRL_ID),
    END_WIN
};

//the window for www bookmark sub menu
WINDOW_TABLE(MMIWWW_BOOKMARK_OPT_WIN_TAB1) = 
{
    WIN_FUNC((uint32)HandleWWWBookmarkOptWinMsg),
    WIN_ID(MMIWWW_BOOKMARK_OPT_WIN_ID),
    WIN_STYLE(WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_POPMENU_CTRL(MENU_WWW_BOOKMARK_OPT1,MMIWWW_POPMENU_BOOKMARK_OPT_CTRL_ID),
    END_WIN
};

//the window for www bookmark sub menu
WINDOW_TABLE(MMIWWW_BOOKMARK_OPT_WIN_TAB2) = 
{
    WIN_FUNC((uint32)HandleWWWBookmarkOptWinMsg),
    WIN_ID(MMIWWW_BOOKMARK_OPT_WIN_ID),
    WIN_STYLE(WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_POPMENU_CTRL(MENU_WWW_BOOKMARK_OPT2,MMIWWW_POPMENU_BOOKMARK_OPT_CTRL_ID),
    END_WIN
};

//the window for sub menu of bookmark detail
WINDOW_TABLE(MMIWWW_BOOKMARKDETAILSUBMENU_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleWWWBookmarkDetailSubMenuWinMsg),
    WIN_ID(MMIWWW_BOOKMARK_DETAIL_SUBMENU_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_POPMENU_CTRL(MENU_WWW_BOOKMARK_DETAIL_SUBMENU,MMIWWW_BOOKMARK_DETAIL_SUBMENU_CTRL_ID),
    END_WIN
};

//the window for www bookmark list 
WINDOW_TABLE(MMIWWW_BOOKMARK_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleWWWBookmarkWinMsg),    
    WIN_ID(MMIWWW_BOOKMARK_WIN_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIWWW_BOOKMARK_LIST_CTRL_ID),
    END_WIN
};

//the window for www window mangaer
WINDOW_TABLE(MMIWWW_WINDOW_MANAGER_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleWWWWindowManagerWinMsg),    
    WIN_ID(MMIWWW_WINDOW_MANAGER_WIN_ID),
    WIN_TITLE(TXT_WWW_WINDOW_MANAGER),
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIWWW_WINDOWN_MANAER_LIST_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIWWW_WINDOW_MANAGER_OPT_WIN_TAB) =
{
    WIN_FUNC((uint32)HandleWWWWindowManagerOptWinMsg),
    WIN_ID(MMIWWW_WINDOW_MANAGER_OPTION_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_POPMENU_CTRL(MENU_WWW_WINDOW_MANAGER, MMIWWW_POPMENU_WINDOW_MANAGER_OPT_CTRL_ID),
    END_WIN  
};

//the window for inputing bookmark
WINDOW_TABLE(MMIWWW_INPUTBOOKMARK_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleInputBookmarkWinMsg),    
    WIN_ID(MMIWWW_INPUT_BOOKMARK_WIN_ID),
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(TXT_NULL, TXT_EDIT, STXT_RETURN),
    CREATE_RICHTEXT_CTRL(MMIWWW_BOOKMARK_INPUT_RICHTEXT_CTRL_ID),
    END_WIN
};

//the window for showing bookmark 
WINDOW_TABLE(MMIWWW_SHOWBOOKMARK_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleShowBookmarkWinMsg),    
    WIN_ID(MMIWWW_SHOW_BOOKMARK_WIN_ID),
    WIN_TITLE(TXT_WWW_SHOW_ADDRESS),
    WIN_SOFTKEY(STXT_OPTION, TXT_COMMON_CONNECT_COMN, STXT_RETURN),
    CREATE_RICHTEXT_CTRL(MMIWWW_BOOKMARK_SHOW_RICHTEXT_CTRL_ID),
    END_WIN
};

//the window for Char set
WINDOW_TABLE(MMIWWW_CHARSETLIST_WIN_TAB ) = 
{
    WIN_FUNC((uint32)HandleCharsetListWinMsg),    
    WIN_ID(MMIWWW_CHARSETLIST_WIN_ID),
    WIN_TITLE(TXT_WWW_CHARSET),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIWWW_LISTBOX_CHARSET_ID),
    END_WIN
};

//the window for point mode
WINDOW_TABLE(MMIWWW_POINTERMODE_WIN_TAB ) = 
{
    WIN_FUNC((uint32)HandlePointerModeListWinMsg),    
    WIN_ID(MMIWWW_POINTERMODE_WIN_ID),
    WIN_TITLE(TXT_WWW_POINTER_MODE),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIWWW_LISTBOX_POINTERMODE_ID),
    END_WIN
};

//the window for shotcut key
WINDOW_TABLE(MMIWWW_SHOTCUT_KEY_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleWWWShotcutKeyWinMsg),
    WIN_ID(MMIWWW_MAINMENU_OPT_WIN_ID),
    WIN_STYLE(WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

//the window for download auth
WINDOW_TABLE(MMIWWW_AUTH_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleWWWAuthWinMsg),    
    WIN_ID(MMIWWW_AUTH_WIN_ID),
//    WIN_STATUSBAR,
    WIN_TITLE(TXT_DL_DOWNLOAD_AUTH),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),

    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIWWW_AUTH_FORM_CTRL_ID),
    
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWWW_AUTH_ACCOUNT_FORM_CTRL_ID,MMIWWW_AUTH_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIWWW_AUTH_ACCOUNT_CTRL_ID,MMIWWW_AUTH_ACCOUNT_FORM_CTRL_ID),
    CHILD_EDIT_TEXT_CTRL(TRUE,MMIWWW_AUTH_USER_LEN,MMIWWW_AUTH_ACCOUNT_VALUE_CTRL_ID,MMIWWW_AUTH_ACCOUNT_FORM_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWWW_AUTH_PASSWORD_FORM_CTRL_ID,MMIWWW_AUTH_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIWWW_AUTH_PASSWORD_CTRL_ID,MMIWWW_AUTH_PASSWORD_FORM_CTRL_ID),
    CHILD_EDIT_PASSWORD_CTRL(TRUE,MMIWWW_AUTH_PASSWORD_LEN,MMIWWW_AUTH_PASSWORD_VALUE_CTRL_ID,MMIWWW_AUTH_PASSWORD_FORM_CTRL_ID),

    END_WIN
};

#if 0//@fen.xie
//the window for CC select menu
WINDOW_TABLE(MMIWWW_CC_SELECT_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleWWWCCSelectWinMsg),
    WIN_TITLE(TXT_NULL),
    WIN_ID(MMIWWW_CC_SELECT_WIN_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
#endif

//the window for CC select menu
WINDOW_TABLE(MMIWWW_HOMEPAGE_SET_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleWWWHomepageSetWinMsg),
    WIN_TITLE(TXT_COMMON_HOMEPAGE_SETTING),
    WIN_ID(MMIWWW_HOMEPAGE_SET_WIN_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),   
	CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIWWW_SETTINGS_HOMEPAGE_FORM_CTRL_ID),
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER, MMIWWW_SETTINGS_HOMEPAGE_FORM1_CTRL_ID,MMIWWW_SETTINGS_HOMEPAGE_FORM_CTRL_ID),
	        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWWW_SETTINGS_HOMEPAGE_LABLE_CTRL_ID, MMIWWW_SETTINGS_HOMEPAGE_FORM1_CTRL_ID),
	        CHILD_EDIT_TEXT_CTRL(TRUE, MAX_WEB_URL_LEN, MMIWWW_SETTINGS_HOMEPAGE_TEXT_CTRL_ID, MMIWWW_SETTINGS_HOMEPAGE_FORM1_CTRL_ID),

    END_WIN
};

#if defined(MMI_WIFI_SUPPORT) || !defined (MMI_MULTI_SIM_SYS_SINGLE)
//the select sim window
WINDOW_TABLE( MMIWWW_SELECTSIM_WIN_TAB ) = 
{
    WIN_TITLE(TXT_NULL),
    WIN_FUNC((uint32)WWWHandleSIMSelectWinMsg),    
    WIN_ID(MMIWWW_SELECTSIM_WIN_ID),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMIWWW_SELECT_SIM_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
#endif

#ifdef MMI_WIFI_SUPPORT
WINDOW_TABLE(MMIWLAN_QUERY_GPRS_WIN_TAB) = 
{
    WIN_FUNC((uint32) HandleQueryGPRSWinMsg), 
    WIN_ID(MMIWLAN_QUERY_GPRS_WIN_ID),
    WIN_STATUSBAR,
    WIN_TITLE(TXT_WWW_SELECT_NETWORK),
    WIN_SOFTKEY(TXT_STK_YES, TXT_NULL, TXT_COMM_NO),

    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIWWW_QUERY_GPRS_FORM_CTRL_ID),
	CHILD_EDIT_TEXT_CTRL(FALSE, MMIWWW_QUERY_GPRS_NOTIFICATION_MAX ,MMIWWW_QUERY_GPRS_NOTIFICATION_LABEL_CTRL_ID, MMIWWW_QUERY_GPRS_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, MMIWWW_QUERY_NEVER_NOTIFY_LABEL_CTRL_ID, MMIWWW_QUERY_GPRS_FORM_CTRL_ID),

    END_WIN
};
#endif

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Create WWW main menu
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIWWW_OpenWWWMainMenuWin(MMIWWW_CTRL_E focus_ctrl)
{
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};
    WWW_APPLET_T *applet = MMIWWW_APPLETINSTANCE();
    MMI_HANDLE_T applet_handle = PNULL;
    MMI_HANDLE_T mainmenu_handle = PNULL;
    MMI_HANDLE_T histroy_handle = PNULL;
    MMI_HANDLE_T bookmark_handle = PNULL;
    GUI_RECT_T   win_rect = {0};
    
    win_rect.left = MMIWWW_TAB_LIST_LEFT,
    win_rect.top = MMIWWW_TAB_LIST_TOP;
    win_rect.right = MMIWWW_TAB_LIST_RIGHT;
    win_rect.bottom = MMIWWW_TAB_LIST_BOTTOM;

    applet_handle = MMIWWW_HANDLE(applet);
    mainmenu_handle = MMK_GetWinHandle(applet_handle, MMIWWW_MAINMENU_WIN_ID);

    if (MMITHEME_IsMainScreenLandscape())
    {
        win_rect.top = MMIWWW_TAB_LIST_TOP_H;
        win_rect.right = MMIWWW_TAB_LIST_RIGHT_H;
        win_rect.right = MMIWWW_TAB_LIST_BOTTOM_H;
    }

    if (MMK_IsOpenWin(mainmenu_handle))
    {
        histroy_handle = MMK_GetWinHandle(applet_handle, MMIWWW_HISTORY_WIN_ID);
        bookmark_handle = MMK_GetWinHandle(applet_handle, MMIWWW_BOOKMARK_WIN_ID);
        MMIWWW_NetfrontSetMainMenuFocusCtrl(focus_ctrl);
        /* Grab to parent before grab to child.It's not needed if MMK improved */
        MMK_WinGrabFocus(mainmenu_handle);
    }
    else
    {
        MMIWWW_NetfrontSetMainMenuFocusCtrl(focus_ctrl);

        win_table_create.applet_handle =applet_handle;
        win_table_create.parent_win_handle = PNULL;
        win_table_create.win_table_ptr = MMIWWW_MAINMENU_WIN_TAB;
        mainmenu_handle = MMK_CreateWinTable(&win_table_create );

        win_table_create.parent_win_handle = mainmenu_handle;
        win_table_create.win_table_ptr = MMIWWW_HISTORY_WIN_TAB;
        histroy_handle = MMK_CreateWinTable(&win_table_create );
        MMK_SetWinRect(histroy_handle, &win_rect);

        win_table_create.win_table_ptr = MMIWWW_BOOKMARK_WIN_TAB;
        bookmark_handle = MMK_CreateWinTable(&win_table_create );
        MMK_SetWinRect(bookmark_handle, &win_rect);
    }

    if (MMIWWW_CTRL_HISTORY_LIST == focus_ctrl)
    {
        MMK_WinGrabFocus(histroy_handle);
    }
    if (MMIWWW_CTRL_BOOKMARK_LIST == focus_ctrl)
    {
        MMK_WinGrabFocus(bookmark_handle);
    }

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : Create page option menu
//  Global resource dependence : 
//  Author: Bill.Ji
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWWW_OpenWWWPageOptMenuWin(void)
{
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};
    
    win_table_create.applet_handle = MMIWWW_HANDLE(MMIWWW_APPLETINSTANCE());
    win_table_create.parent_win_handle = 0;
    if (MMIWWW_IsFullDisplay())
    {
        win_table_create.win_table_ptr = MMIWWW_PAGE_FULL_OPTMENU_WIN_TAB;
    }
    else
    {
        win_table_create.win_table_ptr = MMIWWW_PAGE_OPTMENU_WIN_TAB;
    }

    MMK_CreateWinTable( &win_table_create );

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : Create charset win
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWWW_OpenCharsetWin(void)
{
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};

    win_table_create.applet_handle = MMIWWW_HANDLE(MMIWWW_APPLETINSTANCE());
    win_table_create.parent_win_handle = 0;
    win_table_create.win_table_ptr = MMIWWW_CHARSETLIST_WIN_TAB;

    MMK_CreateWinTable(&win_table_create);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : Create homepage setting win
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWWW_OpenHomepageWin(void)
{
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};

    win_table_create.applet_handle = MMIWWW_HANDLE(MMIWWW_APPLETINSTANCE());
    win_table_create.parent_win_handle = 0;
    win_table_create.win_table_ptr = MMIWWW_HOMEPAGE_SET_WIN_TAB;

    MMK_CreateWinTable(&win_table_create);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : Create Pointer Mode win
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWWW_OpenPointerModeWin(void)
{
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};

    win_table_create.applet_handle = MMIWWW_HANDLE(MMIWWW_APPLETINSTANCE());
    win_table_create.parent_win_handle = 0;
    win_table_create.win_table_ptr = MMIWWW_POINTERMODE_WIN_TAB;

    MMK_CreateWinTable(&win_table_create);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : Create shotcut option menu
//  Global resource dependence : 
//  Author: Bill.Ji
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWWW_OpenShotcutKeyOptMenuWin(BOOLEAN is_full_disp)
{
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};

    win_table_create.applet_handle = MMIWWW_HANDLE(MMIWWW_APPLETINSTANCE());
    win_table_create.parent_win_handle = PNULL;
    win_table_create.win_table_ptr = MMIWWW_SHOTCUT_KEY_WIN_TAB;

    MMK_CreateWinTable( &win_table_create );

    return MMI_RESULT_TRUE;
}

#if 0//@fen.xie
/*****************************************************************************/
//  Description : Create CC select win
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIWWW_OpenCCSelectWin(MMIWWW_CC_INFO_T *in_cc_info)
{
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};
    MMI_WIN_ID_T win_handle = PNULL;
    MMIWWW_CC_INFO_T *cc_info;
    uint16 num_len = 0;

    if (PNULL == in_cc_info || PNULL == in_cc_info->phone_num_ptr)
    {
        return MMI_RESULT_TRUE;
    }

    cc_info = SCI_ALLOCA(sizeof(MMIWWW_CC_INFO_T));

    if (PNULL == cc_info)
    {
        return MMI_RESULT_TRUE;
    }
    SCI_MEMSET(cc_info, 0x00, sizeof(MMIWWW_CC_INFO_T));
    cc_info->cc_option = in_cc_info->cc_option;

    num_len = strlen((char *)in_cc_info->phone_num_ptr);
    cc_info->phone_num_ptr = SCI_ALLOCA(num_len + 1);
    if (PNULL == cc_info->phone_num_ptr)
    {
        SCI_FREE(cc_info);
        return MMI_RESULT_TRUE;
    }

    SCI_MEMSET(cc_info->phone_num_ptr, 0x00, num_len + 1);
    SCI_MEMCPY(cc_info->phone_num_ptr, in_cc_info->phone_num_ptr, num_len);

    win_table_create.applet_handle = MMIWWW_HANDLE(MMIWWW_APPLETINSTANCE());
    win_table_create.parent_win_handle = PNULL;
    win_table_create.win_table_ptr = MMIWWW_CC_SELECT_WIN_TAB;
    win_table_create.add_data_ptr = (ADD_DATA)cc_info;

    win_handle = MMK_CreateWinTable(&win_table_create);

    return MMI_RESULT_TRUE;
}
#endif

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIWWW_OpenQueryGPRSWin(void)
{
#ifdef MMI_WIFI_SUPPORT
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};

    win_table_create.applet_handle = MMIWWW_HANDLE(MMIWWW_APPLETINSTANCE());
    win_table_create.parent_win_handle = PNULL;
    win_table_create.win_table_ptr = MMIWLAN_QUERY_GPRS_WIN_TAB;

    MMK_CreateWinTable(&win_table_create);
#endif
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : Set the focus of ctrl of mainmenu
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWWW_MainMenuHandleDirectionKey(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMIWWW_CTRL_E focus_ctrl = MMIWWW_NetfrontGetMainMenuFocusCtrl();
    MMI_HANDLE_T applet_handle = MMIWWW_HANDLE((WWW_APPLET_T *)MMK_GetAppletPtrByWin(win_id));

    switch(msg_id)
    {
    case MSG_KEYDOWN_UP:
        if (MMIWWW_CTRL_NULL == focus_ctrl || MMIWWW_CTRL_ADDRESS_LABEL == focus_ctrl || MMIWWW_CTRL_LINK_BUTTON == focus_ctrl)
        {
           recode = MMI_RESULT_TRUE;
        }
        else if (MMIWWW_CTRL_SEARCH_LABEL == focus_ctrl)
        {
            focus_ctrl = MMIWWW_CTRL_ADDRESS_LABEL;
        }
        else if (MMIWWW_CTRL_SEARCH_BUTTON == focus_ctrl)
        {
            if (!MMIWWW_NetfrontGetLinkButtonDisabled())
            {
                focus_ctrl = MMIWWW_CTRL_LINK_BUTTON;
            }
        }
        else if (MMIWWW_CTRL_TAB == focus_ctrl)
        {
            focus_ctrl = MMIWWW_CTRL_SEARCH_LABEL;
        }
        else /* MMIWWW_CTRL_BOOKMARK_LIST MMIWWW_CTRL_HISTORY_LIST */
        {
           return MMI_RESULT_FALSE;
        }
        break;

    case MSG_KEYDOWN_DOWN:
        if (MMIWWW_CTRL_NULL == focus_ctrl || MMIWWW_CTRL_TAB == focus_ctrl)
        {
            focus_ctrl = MMIWWW_CTRL_ADDRESS_LABEL;
        }
        else if (MMIWWW_CTRL_ADDRESS_LABEL == focus_ctrl)
        {
            focus_ctrl = MMIWWW_CTRL_SEARCH_LABEL;
        }
        else if (MMIWWW_CTRL_LINK_BUTTON == focus_ctrl)
        {
            if (MMIWWW_NetfrontGetSearchButtonDisabled())
            {
                focus_ctrl = MMIWWW_CTRL_SEARCH_LABEL;
            }
            else
            {
                focus_ctrl = MMIWWW_CTRL_SEARCH_BUTTON;
            }
        }
        else if (MMIWWW_CTRL_SEARCH_LABEL == focus_ctrl || MMIWWW_CTRL_SEARCH_BUTTON == focus_ctrl)
        {
            MMI_HANDLE_T win_handle = PNULL;
            MMI_HANDLE_T ctrl_handle = PNULL;
            MMI_CTRL_ID_T ctrl_id = 0;
            MMI_WIN_ID_T list_win_id = 0;
            uint16 top_index = 0;

            if (0 == GUITAB_GetCurSel(MMK_GetCtrlHandleByWin(win_id, MMIWWW_MAINMENU_TAB_CTRL_ID)))
            {
                ctrl_id = MMIWWW_BOOKMARK_LIST_CTRL_ID;
                list_win_id = MMIWWW_BOOKMARK_WIN_ID;
                focus_ctrl = MMIWWW_CTRL_BOOKMARK_LIST;
            }
            else
            {
                ctrl_id = MMIWWW_LISTBOX_HISTORY_CTRL_ID;
                list_win_id = MMIWWW_HISTORY_WIN_ID;
                focus_ctrl = MMIWWW_CTRL_HISTORY_LIST;
            }
            win_handle = MMK_GetWinHandle(applet_handle, list_win_id);
            ctrl_handle = MMK_GetCtrlHandleByWin(win_handle, ctrl_id);
            MMIWWW_NetfrontSetMainMenuFocusCtrl(focus_ctrl);
            top_index = GUILIST_GetTopItemIndex(ctrl_handle);
            GUILIST_SetCurItemIndex(ctrl_handle, top_index);
        }
        else /* MMIWWW_CTRL_BOOKMARK_LIST MMIWWW_CTRL_HISTORY_LIST */
        {
           return MMI_RESULT_FALSE;
        }
        break;

    case MSG_KEYDOWN_LEFT:
        if (MMIWWW_CTRL_LINK_BUTTON == focus_ctrl)
        {
            focus_ctrl = MMIWWW_CTRL_ADDRESS_LABEL;
        }
        else if (MMIWWW_CTRL_SEARCH_BUTTON == focus_ctrl)
        {
            focus_ctrl = MMIWWW_CTRL_SEARCH_LABEL;
        }
        else if (MMIWWW_CTRL_HISTORY_LIST == focus_ctrl)
        {
            focus_ctrl = MMIWWW_CTRL_BOOKMARK_LIST;
            MMIWWW_NetfrontSetMainMenuFocusCtrl(focus_ctrl);
            MMK_WinGrabFocus(MMK_GetWinHandle(applet_handle, MMIWWW_BOOKMARK_WIN_ID));
            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
            return recode;
        }
        else
        {
            return MMI_RESULT_FALSE;
        }
        break;
        
    case MSG_KEYDOWN_RIGHT:
        if (MMIWWW_CTRL_ADDRESS_LABEL == focus_ctrl)
        {
            if (!MMIWWW_NetfrontGetLinkButtonDisabled())
            {
                focus_ctrl = MMIWWW_CTRL_LINK_BUTTON;
            }
        }
        else if (MMIWWW_CTRL_SEARCH_LABEL == focus_ctrl)
        {
            if (!MMIWWW_NetfrontGetSearchButtonDisabled())
            {
                focus_ctrl = MMIWWW_CTRL_SEARCH_BUTTON;
            }
        }
        else if (MMIWWW_CTRL_BOOKMARK_LIST == focus_ctrl)
        {
            focus_ctrl = MMIWWW_CTRL_HISTORY_LIST;
            MMIWWW_NetfrontSetMainMenuFocusCtrl(focus_ctrl);
            MMK_WinGrabFocus(MMK_GetWinHandle(applet_handle, MMIWWW_HISTORY_WIN_ID));
            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
            return recode;
        }
        else
        {
            return MMI_RESULT_FALSE;
        }
        break;

    default:
        break;
    }

    MMIWWW_NetfrontSetMainMenuFocusCtrl(focus_ctrl);
    MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
    return recode;
}

/*****************************************************************************/
//  Description : Open Address and Key edit window
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void MMIWWW_OpenEditWin(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    wchar               star_value[]= {'.'};
    MMI_HANDLE_T ctrl_handle = PNULL;
    MMI_STRING_T text_str = {0};
    MMI_WINDOW_CREATE_T win_create = {0};
    GUIEDIT_INIT_DATA_T init_data = {0};
    WWW_APPLET_T *applet = (WWW_APPLET_T *)MMK_GetAppletPtrByWin(win_id);
    GUI_RECT_T edit_rect = MMITHEME_GetClientRect();
    wchar http_head[] = {'h', 't', 't', 'p',':', '/', '/', '\0'};
    GUIIM_TYPE_T allow_im = GUIIM_TYPE_NONE;
    GUIIM_TYPE_T init_im = GUIIM_TYPE_NONE;
    uint16 max_input_len = 0;

    win_create.applet_handle = MMIWWW_HANDLE(applet);
    win_create.win_id = MMIWWW_COMMON_EDIT_WIN_ID;
    win_create.func = MMIWWW_HandleCommonEditWinMsg;
    win_create.win_priority = WIN_ONE_LEVEL;
    win_create.add_data_ptr = (ADD_DATA)ctrl_id;

    if (MMIWWW_BOOKMARK_LABEL_URL_INPUT_CTRL_ID == ctrl_id || MMIWWW_BOOKMARK_LABEL_TITLE_INPUT_CTRL_ID == ctrl_id)
    {
        GUIRICHTEXT_ITEM_T item_data = {0};/*lint !e64*/
        MMI_HANDLE_T rich_ctrl_handle = MMK_GetCtrlHandleByWin(MMK_GetWinHandle(MMIWWW_HANDLE(applet), MMIWWW_INPUT_BOOKMARK_WIN_ID), MMIWWW_BOOKMARK_INPUT_RICHTEXT_CTRL_ID); 

        /* Get focus item text (The index is 1 or 3) */
        GUIRICHTEXT_GetItem(rich_ctrl_handle, (uint16)(MMIWWW_BOOKMARK_LABEL_TITLE_INPUT_CTRL_ID == ctrl_id ? 1 : 3), &item_data);
        text_str.wstr_ptr = item_data.text_data.buf.str_ptr;
        text_str.wstr_len = item_data.text_data.buf.len;
    }
    else if ((MMIWWW_MAINMENU_LABEL_ADDRESS_CTRL_ID == ctrl_id && MMIWWW_NetfrontGetUrlChange()) || (MMIWWW_MAINMENU_LABEL_SEARCH_CTRL_ID == ctrl_id && MMIWWW_NetfrontGetKeywordChange()))
    {
        GUILABEL_GetText(MMK_GetCtrlHandleByWin(win_id, ctrl_id), &text_str);
    } 
    else if ((MMIWWW_MAINMENU_LABEL_ADDRESS_CTRL_ID == ctrl_id && !MMIWWW_NetfrontGetUrlChange()))
    {
        text_str.wstr_ptr = http_head;
        text_str.wstr_len = (uint16)MMIAPICOM_Wstrlen(http_head);
    }

    init_data.type = GUIEDIT_TYPE_TEXT;
    init_data.both_rect.h_rect = init_data.both_rect.v_rect = edit_rect;

    if (MMIWWW_MAINMENU_LABEL_ADDRESS_CTRL_ID == ctrl_id || MMIWWW_BOOKMARK_LABEL_URL_INPUT_CTRL_ID == ctrl_id)
    {
        max_input_len = MAX_WEB_URL_LEN;
        allow_im = GUIIM_TYPE_ENGLISH | GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL; //lint !e655
        init_im = GUIIM_TYPE_ABC;
    }
    else if (MMIWWW_MAINMENU_LABEL_SEARCH_CTRL_ID == ctrl_id || MMIWWW_BOOKMARK_LABEL_TITLE_INPUT_CTRL_ID == ctrl_id)
    {
        max_input_len = (uint16)((ctrl_id == MMIWWW_MAINMENU_LABEL_SEARCH_CTRL_ID) ? MMIWWW_MAX_KEYNAME_LEN : MAX_WEB_TITLE_LEN);
        allow_im = GUIIM_TYPE_SET_ALL;
        //init_im = GUIIM_TYPE_DEFAULT;
        init_im = GUIIM_TYPE_NONE;
    }
    else
    {
        max_input_len = MMIWWW_MAX_KEYNAME_LEN;
        allow_im = GUIIM_TYPE_SET_ALL;
        //init_im = GUIIM_TYPE_DEFAULT;
        init_im = GUIIM_TYPE_NONE;
    }
    MMIWWW_OpenCommonEditWin(&win_create, &init_data);

    ctrl_handle = MMK_GetCtrlHandleByWin(MMK_GetWinHandle(MMIWWW_HANDLE(applet), MMIWWW_COMMON_EDIT_WIN_ID), MMIWWW_COMMON_EDIT_CTRL_ID); 

    //set max len
    GUIEDIT_SetTextMaxLen(ctrl_handle, max_input_len, max_input_len);

    //set im
    GUIEDIT_SetIm(ctrl_handle, allow_im, init_im);

    if (MMIWWW_MAINMENU_LABEL_ADDRESS_CTRL_ID == ctrl_id || MMIWWW_BOOKMARK_LABEL_URL_INPUT_CTRL_ID == ctrl_id)
    {
        GUIEDIT_ConfigImCustomKey(ctrl_handle,GUIIM_TYPE_DIGITAL,GUIIM_CUSTOM_KEY_STAR,star_value,1);
        GUIEDIT_SetImTag(ctrl_handle, GUIIM_TAG_DOT_NUM);
    }

    //set string
    if (PNULL != text_str.wstr_ptr && 0 != text_str.wstr_len)
    {
        GUIEDIT_SetString(ctrl_handle, text_str.wstr_ptr, text_str.wstr_len);
    }
}

/*****************************************************************************/
//  Description : Open Common Edit window
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void MMIWWW_OpenCommonEditWin(MMI_WINDOW_CREATE_T * win_create, GUIEDIT_INIT_DATA_T * init_data)
{
    MMI_HANDLE_T win_handle = PNULL;
    MMI_CONTROL_CREATE_T create = {0};

    if (PNULL == win_create || PNULL == init_data)
    {
        return;
    }

    win_handle = MMK_CreateWindow( win_create );

#ifdef MMI_PDA_SUPPORT
    GUIWIN_CreateStbDyn(win_handle);
#endif

    CreatSoftkeyCtrl(win_handle, TXT_COMMON_OK, TXT_NULL, STXT_RETURN);
    GUIWIN_CreateTitleDyna(win_handle, TXT_NULL);

    create.ctrl_id = MMIWWW_COMMON_EDIT_CTRL_ID;
    create.guid = SPRD_GUI_EDITBOX_ID;
    create.parent_win_handle = win_handle;
    create.init_data_ptr = init_data;

    MMK_CreateControl(&create);
}

/*****************************************************************************/
//  Description : Handle common Edit message
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWWW_HandleCommonEditWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    WWW_APPLET_T *applet = (WWW_APPLET_T *)MMK_GetAppletPtrByWin(win_id);

    //SCI_TRACE_LOW:"mmiwww_wintab.c:MMIWWW_HandleCommonEditWinMsg() enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_2243_112_2_18_3_12_5_177,(uint8*)"");

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            MMI_TEXT_ID_T title_id = TXT_NULL;
            MMI_HANDLE_T ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_COMMON_EDIT_CTRL_ID);
            MMI_CTRL_ID_T label_ctrl_id = (MMI_HANDLE_T)MMK_GetWinAddDataPtr(win_id);

            if (MMIWWW_MAINMENU_LABEL_ADDRESS_CTRL_ID == label_ctrl_id)
            {
                title_id = TXT_WWW_INPUT_URL;
            }
            else if (MMIWWW_MAINMENU_LABEL_SEARCH_CTRL_ID == label_ctrl_id)
            {
                title_id = TXT_WWW_INPUT_KEYWORD;
            }
            else if (MMIWWW_BOOKMARK_LABEL_URL_INPUT_CTRL_ID == label_ctrl_id)
            {
                title_id = TXT_WWW_INPUT_URL;
            }
            else if (MMIWWW_BOOKMARK_LABEL_TITLE_INPUT_CTRL_ID == label_ctrl_id)
            {
                title_id = TXT_WWW_INPUT_TITLE;
            }
            else
            {
                title_id = TXT_NULL;
            }

            GUIWIN_SetTitleTextId(win_id, title_id, FALSE);
            MMK_SetActiveCtrl(ctrl_handle, FALSE);
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
        {
            MMI_STRING_T string_info = {0};
            MMI_CTRL_ID_T label_ctrl_id = 0;
            MMI_WIN_ID_T label_win_id = 0;
            label_ctrl_id = (MMI_HANDLE_T)MMK_GetWinAddDataPtr(win_id);
            GUIEDIT_GetString(MMK_GetCtrlHandleByWin(win_id, MMIWWW_COMMON_EDIT_CTRL_ID), &string_info);
 
            switch(label_ctrl_id)
            {
            case MMIWWW_MAINMENU_LABEL_ADDRESS_CTRL_ID:
                if (PNULL != string_info.wstr_ptr && string_info.wstr_len > 0)
                {
                    MMIWWW_NetfrontSetUrlChange(TRUE);
                    MMIWWW_NetfrontSetMainMenuFocusCtrl(MMIWWW_CTRL_LINK_BUTTON);
                }
                else
                {
                    MMIWWW_NetfrontSetUrlChange(FALSE);
                }
                label_win_id = MMIWWW_MAINMENU_WIN_ID;
                break;

            case MMIWWW_MAINMENU_LABEL_SEARCH_CTRL_ID:
                if (PNULL != string_info.wstr_ptr && string_info.wstr_len > 0)
                {
                    MMIWWW_NetfrontSetKeywordChange(TRUE);
                    MMIWWW_NetfrontSetMainMenuFocusCtrl(MMIWWW_CTRL_SEARCH_BUTTON);
                }
                else
                {
                    MMIWWW_NetfrontSetKeywordChange(FALSE);
                }
                label_win_id = MMIWWW_MAINMENU_WIN_ID;
                break;

            case MMIWWW_BOOKMARK_LABEL_URL_INPUT_CTRL_ID:
            case MMIWWW_BOOKMARK_LABEL_TITLE_INPUT_CTRL_ID:
                {
                    GUIRICHTEXT_ITEM_T item_data = {0};/*lint !e64*/
                    GUIRICHTEXT_ITEM_T org_item_data = {0};/*lint !e64*/
                    MMI_HANDLE_T input_win_handle = MMK_GetWinHandle(MMIWWW_HANDLE(applet), MMIWWW_INPUT_BOOKMARK_WIN_ID);
                    MMI_HANDLE_T rich_ctrl_handle = MMK_GetCtrlHandleByWin(input_win_handle, MMIWWW_BOOKMARK_INPUT_RICHTEXT_CTRL_ID); 
                    uint16 focus_index = (uint16)(MMIWWW_BOOKMARK_LABEL_TITLE_INPUT_CTRL_ID == label_ctrl_id ? MMIWWW_BOOKMARK_TITLE_INDEX : MMIWWW_BOOKMARK_URL_INDEX);

                    GUIRICHTEXT_GetItem(rich_ctrl_handle, focus_index, &org_item_data);
                    if (string_info.wstr_len != org_item_data.text_data.buf.len || 0 != MMIAPICOM_Wstrncmp(org_item_data.text_data.buf.str_ptr, string_info.wstr_ptr, org_item_data.text_data.buf.len)) 
                    {
                        SCI_MEMCPY(&item_data, &org_item_data, sizeof(GUIRICHTEXT_ITEM_T));
                        item_data.text_data.buf.len = string_info.wstr_len;
                        item_data.text_data.buf.str_ptr = string_info.wstr_ptr;
                        item_data.text_type = GUIRICHTEXT_TEXT_BUF;
                        GUIRICHTEXT_ModifyItem(rich_ctrl_handle, focus_index, &item_data);
                        MMIWWW_NetfrontSetBookmarkChange(TRUE);
                        GUIWIN_SetSoftkeyTextId(input_win_handle, STXT_SAVE, TXT_EDIT, STXT_RETURN, FALSE);
                    }
                    MMK_CloseWin(win_id);
                }
                return MMI_RESULT_TRUE;

            default:
                break;

            }
            GUILABEL_SetText(MMK_GetCtrlHandleByWin(MMK_GetWinHandle(MMIWWW_HANDLE(applet), label_win_id), label_ctrl_id), &string_info, FALSE);
            MMK_CloseWin(win_id);
            GUILABEL_GetText(MMK_GetCtrlHandleByWin(MMK_GetWinHandle(MMIWWW_HANDLE(applet), label_win_id), label_ctrl_id), &string_info);

            if(label_ctrl_id == MMIWWW_MAINMENU_LABEL_ADDRESS_CTRL_ID && PNULL != string_info.wstr_ptr && string_info.wstr_len > 0)
            {                
                if(MMIWWW_GetMatchURLNum(string_info.wstr_ptr))
                {
                    MMIWWW_NetfrontSetMainMenuFocusState(FALSE);
                    MMK_PostMsg(MMK_GetWinHandle(MMIWWW_HANDLE(applet), MMIWWW_MAINMENU_WIN_ID), MSG_WWW_LIST_MATCHURL, (DPARAM)&string_info, sizeof(string_info));
                }
            }
            else if(label_ctrl_id == MMIWWW_MAINMENU_LABEL_SEARCH_CTRL_ID && PNULL != string_info.wstr_ptr && string_info.wstr_len > 0)
            {
                if(MMIWWW_GetMatchKeyNum(string_info.wstr_ptr))
                {
                    MMIWWW_NetfrontSetMainMenuFocusState(FALSE);
                    MMK_PostMsg(MMK_GetWinHandle(MMIWWW_HANDLE(applet), MMIWWW_MAINMENU_WIN_ID), MSG_WWW_LIST_MATCHKEY, (DPARAM)&string_info, sizeof(string_info));
                }
            }
        }
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

/*****************************************************************************/
//  Description : Handle Center Key
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWWW_MainMenuHandleWebKey(MMI_WIN_ID_T win_id)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMIWWW_CTRL_E focus_ctrl = MMIWWW_NetfrontGetMainMenuFocusCtrl();

    if (MMIWWW_CTRL_ADDRESS_LABEL == focus_ctrl)
    {
        MMIWWW_OpenEditWin(win_id, MMIWWW_MAINMENU_LABEL_ADDRESS_CTRL_ID);
    }
    else if (MMIWWW_CTRL_LINK_BUTTON == focus_ctrl)
    {
        MMIWWW_MainMenuHandleLinkButtonPress(win_id);
    }
    else if (MMIWWW_CTRL_SEARCH_LABEL == focus_ctrl)
    {
        MMIWWW_OpenEditWin(win_id, MMIWWW_MAINMENU_LABEL_SEARCH_CTRL_ID);
    }
    else if (MMIWWW_CTRL_SEARCH_BUTTON == focus_ctrl)
    {
        MMIWWW_MainMenuHandleSearchButtonPress(win_id);
    }
/*
    else if (MMIWWW_CTRL_TAB == focus_ctrl)
    {
        MMI_CTRL_ID_T  ctrl_id = 0;
        MMI_WIN_ID_T list_win_id = 0;

        if (0 == GUITAB_GetCurSel(MMK_GetCtrlHandleByWin(win_id, MMIWWW_MAINMENU_TAB_CTRL_ID)))
        {
            ctrl_id = MMIWWW_BOOKMARK_LIST_CTRL_ID;
            list_win_id = MMIWWW_BOOKMARK_WIN_ID;
            focus_ctrl = MMIWWW_CTRL_BOOKMARK_LIST;
        }
        else
        {
            ctrl_id = MMIWWW_LISTBOX_HISTORY_CTRL_ID;
            list_win_id = MMIWWW_HISTORY_WIN_ID;
            focus_ctrl = MMIWWW_CTRL_HISTORY_LIST;
        }

        MMIWWW_NetfrontSetMainMenuFocusCtrl(focus_ctrl);
        MMK_SetActiveCtrl(MMK_GetCtrlHandleByWin(MMK_GetWinHandle(MMIWWW_HANDLE((WWW_APPLET_T *)MMK_GetAppletPtrByWin(win_id)), list_win_id), ctrl_id), TRUE);
    }
*/
    else if (MMIWWW_CTRL_NULL == focus_ctrl)
    {
        /* Do nothing */
        recode = MMI_RESULT_TRUE; 
    }
    else
    {
        /* Leave it to child wind*/
        recode = MMI_RESULT_FALSE;
    }

    return recode;
}

/*****************************************************************************/
//  Description : Handle link press
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWWW_MainMenuHandleLinkButtonPress(MMI_WIN_ID_T win_id)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_STRING_T text_str = {0};

    GUILABEL_GetText(MMK_GetCtrlHandleByWin(win_id, MMIWWW_MAINMENU_LABEL_ADDRESS_CTRL_ID), &text_str);
    MMIWWW_ActionGoToURL(&text_str);
    MMIWWW_Netfront_AddInputURLHistory(&text_str);
    return recode;
}

/*****************************************************************************/
//  Description : Handle link press
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWWW_MainMenuHandleSearchButtonPress(MMI_WIN_ID_T win_id)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_STRING_T text_str = {0};
    uint8 * url = PNULL;
    uint8 search_engine = 0;

    GUILABEL_GetText(MMK_GetCtrlHandleByWin(win_id, MMIWWW_MAINMENU_LABEL_SEARCH_CTRL_ID), &text_str);
    search_engine = MMIWWW_GetSearchEngine();
    url = MMIWWW_GenerateSearchWeb((MMIWWW_SEARCH_ENGINE_MODE_E)search_engine, &text_str);
    if (PNULL != url)
    {
        MMIWWW_OpenWWWBrowserWin(BROWSER_START_BY_URL, (char *)url);
        MMIWWW_NV_WriteKeyItem(&text_str);
        SCI_FREE(url);
    }

    return recode;
}

/*****************************************************************************/
//  Description : Handle Open Window
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWWW_MainMenuHandleOpenWindow(MMI_WIN_ID_T win_id)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_HANDLE_T address_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_MAINMENU_LABEL_ADDRESS_CTRL_ID);
    MMI_HANDLE_T search_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_MAINMENU_LABEL_SEARCH_CTRL_ID);
    MMIWWW_SEARCH_ENGINE_MODE_E search_engine = (MMIWWW_SEARCH_ENGINE_MODE_E)MMIWWW_GetSearchEngine();
    MMI_IMAGE_ID_T icon_id;
    GUISOFTKEY_IMG_T img_info = {0};
    GUI_COLOR_T font_color = 0;

    font_color = MMITHEME_GetCurThemeFontColor(MMI_THEME_LIST_ITEM0); /* label & softkey's theme is Not OK */

    GUIWIN_SetTitleBackground(win_id, MMITHEME_GetWinTitleBar(), 0);
    GUIWIN_SetTitleFontColor(win_id,MMITHEME_GetCurThemeFontColor(MMI_THEME_MENU));

    /* address Label */
    GUILABEL_SetIcon(address_handle, IMAGE_WWW_ADDRESS);
    GUILABEL_SetFont(address_handle,MMI_DEFAULT_TEXT_FONT, font_color);

    /* and search */
    if (MMIWWW_SEARCH_ENGINE_GOOGLE == search_engine)
    {
        icon_id = IMAGE_WWW_GOOGLE;
    }
    else if (MMIWWW_SEARCH_ENGINE_CMCC == search_engine)
    {
        icon_id = IMAGE_WWW_CMCC;
    }
    else if (MMIWWW_SEARCH_ENGINE_BAIDU == search_engine)
    {
        icon_id = IMAGE_WWW_BAIDU;
    }
    else
    {
        icon_id = IMAGE_WWW_YAHOO;
    }

    GUILABEL_SetIcon(search_handle, icon_id);
    GUILABEL_SetFont(search_handle,MMI_DEFAULT_TEXT_FONT, font_color);

    /* Tab */
    MMIWWW_SetTabProperty(win_id, MMK_GetCtrlHandleByWin(win_id, MMIWWW_MAINMENU_TAB_CTRL_ID));
    
    /* softkey */
    MMITHEME_GetDefaultSoftkeyImg(&img_info);
    GUIWIN_SetSoftkeyStyle(win_id, font_color, GUI_SOFTKEY_BG_IMAGE_ONLY, 0, img_info.bg_img );

    return recode;
}

/*****************************************************************************/
//  Description : Draw Background of mainmenu
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void MMIWWW_MainMenuSetBackground(MMI_WIN_ID_T win_id)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T bg_image_rect = {0};
    uint16 screen_width = 0;
    uint16 screen_height = 0;

    bg_image_rect.right = MMI_MAINSCREEN_RIGHT_MAX_PIXEL;
    bg_image_rect.bottom = MMIWWW_TAB_BOTTOM;

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &screen_width, &screen_height);
    bg_image_rect.right = screen_width - 1;

    GUIRES_DisplayImg(PNULL, &bg_image_rect, &bg_image_rect, win_id, IMAGE_COMMON_BG, &lcd_dev_info);
}

/*****************************************************************************/
//  Description : Set address label of mainmenu
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void MMIWWW_MainMenuSetLabel(MMI_WIN_ID_T win_id)
{
    MMI_STRING_T text_str = {0};
    MMI_HANDLE_T address_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_MAINMENU_LABEL_ADDRESS_CTRL_ID);
    MMI_HANDLE_T search_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_MAINMENU_LABEL_SEARCH_CTRL_ID);
    GUI_RECT_T address_rect = {0};
    GUI_RECT_T search_rect ={0};
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    MMIWWW_CTRL_E focus_ctrl = MMIWWW_NetfrontGetMainMenuFocusCtrl();
    
    GUILABEL_GetRect(address_handle, &address_rect);
    GUILABEL_GetRect(search_handle, &search_rect);
    address_rect.right += 1; /* Draw rect */
    search_rect.right += 1; /* Draw rect */

    /* Address */
    GUILABEL_GetText(address_handle, &text_str);
    if (PNULL == text_str.wstr_ptr || text_str.wstr_len == 0)
    {
        GUILABEL_SetTextById(address_handle, TXT_WWW_INPUT_URL, FALSE);
    }

    if (MMIWWW_CTRL_ADDRESS_LABEL == focus_ctrl)
    {
        GUILABEL_SetBackgroundColor(address_handle, MMITHEME_GetCurThemeFontColor(MMI_THEME_SELECT));
        MMK_SetActiveCtrl(address_handle, FALSE);
    }
    else
    {
        GUILABEL_SetBackgroundColor(address_handle, 0);
    }

    LCD_DrawRect(&lcd_dev_info, address_rect, MMI_DARK_GRAY_COLOR);

    /* Search */
    GUILABEL_GetText(search_handle, &text_str);
    if (PNULL == text_str.wstr_ptr || text_str.wstr_len == 0)
    {
        GUILABEL_SetTextById(search_handle, TXT_WWW_SEARCH_KEYWORD, FALSE);
    }

    if (MMIWWW_CTRL_SEARCH_LABEL == focus_ctrl)
    {
        GUILABEL_SetBackgroundColor(search_handle, MMITHEME_GetCurThemeFontColor(MMI_THEME_SELECT));
        MMK_SetActiveCtrl(search_handle, FALSE);
    }
    else
    {
        GUILABEL_SetBackgroundColor(search_handle, 0);
    }

    LCD_DrawRect(&lcd_dev_info, search_rect, MMI_DARK_GRAY_COLOR);
}

/*****************************************************************************/
//  Description : Set Link button of mainmenu
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void MMIWWW_MainMenuSetButton(MMI_WIN_ID_T win_id, DPARAM param)
{
    MMIWWW_CTRL_E focus_ctrl = MMIWWW_NetfrontGetMainMenuFocusCtrl();

    if ((param != NULL) && ((MMIWWW_CTRL_E)(*(uint32*)param) == MMIWWW_CTRL_LINK_BUTTON))
    {
        /* Link button is pressed */
        MMIWWW_MainMenuDisplayLinkButton(win_id, MMIWWW_BUTTON_MODE_PRESSED);
        MMIWWW_MainMenuDisplaySearchButton(win_id, MMIWWW_NetfrontGetSearchButtonDisabled() ? MMIWWW_BUTTON_MODE_DISABLED : MMIWWW_BUTTON_MODE_DEFAULT);    
    }
    else if((param != NULL) && ((MMIWWW_CTRL_E)(*(uint32*)param) == MMIWWW_CTRL_SEARCH_BUTTON))
    {
        /* Search button is pressed */
        MMIWWW_MainMenuDisplaySearchButton(win_id, MMIWWW_BUTTON_MODE_PRESSED);
        MMIWWW_MainMenuDisplayLinkButton(win_id, MMIWWW_NetfrontGetLinkButtonDisabled() ? MMIWWW_BUTTON_MODE_DISABLED : MMIWWW_BUTTON_MODE_DEFAULT);
    }
    else
    {
        if (MMIWWW_CTRL_LINK_BUTTON == focus_ctrl && MMIWWW_NetfrontGetMainMenuFocusState() )
        {
            /* Link button is focused */
            MMIWWW_MainMenuDisplayLinkButton(win_id, MMIWWW_BUTTON_MODE_FOCUSED);
            MMIWWW_MainMenuDisplaySearchButton(win_id, MMIWWW_NetfrontGetSearchButtonDisabled() ? MMIWWW_BUTTON_MODE_DISABLED : MMIWWW_BUTTON_MODE_DEFAULT);    
        }
        else if (MMIWWW_CTRL_SEARCH_BUTTON == focus_ctrl && MMIWWW_NetfrontGetMainMenuFocusState() )
        {
            /* Search button is focused */
            MMIWWW_MainMenuDisplaySearchButton(win_id, MMIWWW_BUTTON_MODE_FOCUSED);
            MMIWWW_MainMenuDisplayLinkButton(win_id, MMIWWW_NetfrontGetLinkButtonDisabled() ? MMIWWW_BUTTON_MODE_DISABLED : MMIWWW_BUTTON_MODE_DEFAULT);
        }
        else
        {
            MMIWWW_MainMenuDisplayLinkButton(win_id, MMIWWW_NetfrontGetLinkButtonDisabled() ? MMIWWW_BUTTON_MODE_DISABLED : MMIWWW_BUTTON_MODE_DEFAULT);
            MMIWWW_MainMenuDisplaySearchButton(win_id, MMIWWW_NetfrontGetSearchButtonDisabled() ? MMIWWW_BUTTON_MODE_DISABLED : MMIWWW_BUTTON_MODE_DEFAULT);
        }
    }
}

/*****************************************************************************/
//  Description : Set softkey of mainmenu when it is focused
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void MMIWWW_MainMenuSetSoftKey(MMI_WIN_ID_T win_id)
{
    MMIWWW_CTRL_E focus_ctrl = MMIWWW_NetfrontGetMainMenuFocusCtrl();
    MMI_HANDLE_T applet_handle = MMIWWW_HANDLE((WWW_APPLET_T *)MMK_GetAppletPtrByWin(win_id));

    if (MMIWWW_CTRL_BOOKMARK_LIST == focus_ctrl || MMIWWW_CTRL_HISTORY_LIST == focus_ctrl)
    {
        /*Use the Softkey of list*/
        GUI_RECT_T   win_rect = {0};

        win_rect.left = MMIWWW_TAB_LIST_LEFT;
        win_rect.top = MMIWWW_TAB_LIST_TOP;
        win_rect.right = MMIWWW_TAB_LIST_RIGHT;
        win_rect.bottom = MMI_MAINSCREEN_HEIGHT - 1;

        if (MMITHEME_IsMainScreenLandscape())
        {
            win_rect.top = MMIWWW_TAB_LIST_TOP_H;
            win_rect.right = MMIWWW_TAB_LIST_RIGHT_H;
            win_rect.right = MMI_MAINSCREEN_WIDTH - 1;
        }

        GUIWIN_SetSoftkeyVisible(MMK_GetWinHandle(applet_handle, MMIWWW_HISTORY_WIN_ID), TRUE);
        GUIWIN_SetSoftkeyVisible(MMK_GetWinHandle(applet_handle, MMIWWW_BOOKMARK_WIN_ID), TRUE);
        MMK_SetWinRect(MMK_GetWinHandle(applet_handle, MMIWWW_HISTORY_WIN_ID), &win_rect);
        MMK_SetWinRect(MMK_GetWinHandle(applet_handle, MMIWWW_BOOKMARK_WIN_ID), &win_rect);
        return;
    }
    else
    {
        GUI_RECT_T   win_rect = {0};

        win_rect.left = MMIWWW_TAB_LIST_LEFT;
        win_rect.top = MMIWWW_TAB_LIST_TOP;
        win_rect.right = MMIWWW_TAB_LIST_RIGHT;
        win_rect.bottom = MMIWWW_TAB_LIST_BOTTOM;

        if (MMITHEME_IsMainScreenLandscape())
        {
            win_rect.top = MMIWWW_TAB_LIST_TOP_H;
            win_rect.right = MMIWWW_TAB_LIST_RIGHT_H;
            win_rect.right = MMIWWW_TAB_LIST_BOTTOM_H;
        }

        GUITAB_SetFocusItemBgColor(MMK_GetCtrlHandleByWin(win_id, MMIWWW_MAINMENU_TAB_CTRL_ID), 0x397);
        GUIWIN_SetSoftkeyVisible(MMK_GetWinHandle(applet_handle, MMIWWW_HISTORY_WIN_ID), FALSE);
        GUIWIN_SetSoftkeyVisible(MMK_GetWinHandle(applet_handle, MMIWWW_BOOKMARK_WIN_ID), FALSE);
        MMK_SetWinRect(MMK_GetWinHandle(applet_handle, MMIWWW_HISTORY_WIN_ID), &win_rect);
        MMK_SetWinRect(MMK_GetWinHandle(applet_handle, MMIWWW_BOOKMARK_WIN_ID), &win_rect);
    }

    if (MMIWWW_CTRL_ADDRESS_LABEL == focus_ctrl || MMIWWW_CTRL_SEARCH_LABEL == focus_ctrl)
    {
        GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_EDIT, TXT_WWW_SUBMENU_EXIT, FALSE);
    }
    else if (MMIWWW_CTRL_LINK_BUTTON == focus_ctrl)
    {
        GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_COMMON_CONNECT_COMN, TXT_WWW_SUBMENU_EXIT, FALSE);
    }
    else if (MMIWWW_CTRL_SEARCH_BUTTON == focus_ctrl)
    {
        GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, STXT_SEARCH, TXT_WWW_SUBMENU_EXIT, FALSE);
    }
    else if (MMIWWW_CTRL_TAB == focus_ctrl)
    {
        GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_WWW_ENTER, STXT_EXIT, FALSE);
    }
    else if (MMIWWW_CTRL_TAB == focus_ctrl)
    {
        GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_WWW_ENTER, STXT_EXIT, FALSE);
    }
    else 
    {
        GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_EXIT, FALSE);
    }               
}

/*****************************************************************************/
//  Description : Display Link Button
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void MMIWWW_MainMenuDisplayLinkButton(MMI_WIN_ID_T win_id, MMIWWW_BUTTON_MODE_E button_mode)
{
    GUI_RECT_T button_rect = {0};
    MMI_IMAGE_ID_T image_id = IMAGE_NULL;
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

    button_rect.left = MMIWWW_LINK_IMAGE_LEFT;
    button_rect.top = MMIWWW_LINK_IMAGE_TOP;
    button_rect.right = MMIWWW_LINK_IMAGE_RIGHT;
    button_rect.bottom = MMIWWW_LINK_IMAGE_BOTTOM;

    if (MMITHEME_IsMainScreenLandscape())
    {
        button_rect.top = MMIWWW_LINK_IMAGE_TOP_H;
        button_rect.left = MMIWWW_LINK_IMAGE_LEFT_H;
        button_rect.right = MMIWWW_LINK_IMAGE_RIGHT_H;
    }

    if (MMIWWW_BUTTON_MODE_PRESSED == button_mode)
    {
        image_id = IMAGE_WWW_LINK_PRESSED;
    }
    else if (MMIWWW_BUTTON_MODE_DISABLED == button_mode)
    {
        image_id = IMAGE_WWW_LINK_DISABLED;
    }
    else if (MMIWWW_BUTTON_MODE_FOCUSED == button_mode)
    {
        image_id = IMAGE_WWW_LINK_FOCUSED;
    }
    else
    {
        image_id = IMAGE_WWW_LINK_DEFAULT;
    }
    GUIRES_DisplayImg(PNULL, &button_rect, PNULL, win_id, image_id, &lcd_dev_info);
}

/*****************************************************************************/
//  Description : Display Search Button
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void MMIWWW_MainMenuDisplaySearchButton(MMI_WIN_ID_T win_id, MMIWWW_BUTTON_MODE_E button_mode)
{
    GUI_RECT_T button_rect = {0};
    MMI_IMAGE_ID_T image_id = IMAGE_NULL;
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

    button_rect.left = MMIWWW_SEARCH_IMAGE_LEFT;
    button_rect.top = MMIWWW_SEARCH_IMAGE_TOP;
    button_rect.right = MMIWWW_SEARCH_IMAGE_RIGHT;
    button_rect.bottom = MMIWWW_SEARCH_IMAGE_BOTTOM;

    if (MMITHEME_IsMainScreenLandscape())
    {
        button_rect.top = MMIWWW_SEARCH_IMAGE_TOP_H;
        button_rect.left = MMIWWW_SEARCH_IMAGE_LEFT_H;
        button_rect.right = MMIWWW_SEARCH_IMAGE_RIGHT_H;
    }

    if (MMIWWW_BUTTON_MODE_PRESSED == button_mode)
    {
        image_id = IMAGE_WWW_SEARCH_PRESSED;
    }
    else if (MMIWWW_BUTTON_MODE_DISABLED == button_mode)
    {
        image_id = IMAGE_WWW_SEARCH_DISABLED;
    }
    else if (MMIWWW_BUTTON_MODE_FOCUSED == button_mode)
    {
        image_id = IMAGE_WWW_SEARCH_FOCUSED;
    }
    else
    {
        image_id = IMAGE_WWW_SEARCH_DEFAULT;
    }
    GUIRES_DisplayImg(PNULL, &button_rect, PNULL, win_id, image_id, &lcd_dev_info);
}

/*****************************************************************************/
//  Description : Handle Paint
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWWW_MainMenuHandleFullPaint(MMI_WIN_ID_T win_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    /* Background */
    MMIWWW_MainMenuSetBackground(win_id);

    /* address and search Label */
    MMIWWW_MainMenuSetLabel(win_id);

    /* address and search button */
    MMIWWW_MainMenuSetButton(win_id, param);

    if (MMK_IsFocusWin(win_id))
    {
        /* Softkey */
        MMIWWW_MainMenuSetSoftKey(win_id);
    }

    /* BG and Title of Tab */
    MMIWWW_SetTabTitle(MMK_GetCtrlHandleByWin(win_id, MMIWWW_MAINMENU_TAB_CTRL_ID), param);

    /* Bookmark and History List */
    MMIWWW_SetListProperty(win_id);

    return recode; 
}

/*****************************************************************************/
//  Description : Handle Tp pressed
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWWW_MainMenuHandleTPPressUp(MMI_WIN_ID_T win_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    GUI_POINT_T point = {0};
    GUI_RECT_T link_button_rect = {0};
    GUI_RECT_T search_button_rect = {0};

    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);

    link_button_rect.left = MMIWWW_LINK_IMAGE_LEFT;
    link_button_rect.top = MMIWWW_LINK_IMAGE_TOP;
    link_button_rect.right = MMIWWW_LINK_IMAGE_RIGHT;
    link_button_rect.bottom = MMIWWW_LINK_IMAGE_BOTTOM;

    search_button_rect.left = MMIWWW_SEARCH_IMAGE_LEFT;
    search_button_rect.top = MMIWWW_SEARCH_IMAGE_TOP;
    search_button_rect.right = MMIWWW_SEARCH_IMAGE_RIGHT;
    search_button_rect.bottom = MMIWWW_SEARCH_IMAGE_BOTTOM;


    if (MMITHEME_IsMainScreenLandscape())
    {
        link_button_rect.top = MMIWWW_LINK_IMAGE_TOP_H;
        link_button_rect.left = MMIWWW_LINK_IMAGE_LEFT_H;
        link_button_rect.right = MMIWWW_LINK_IMAGE_RIGHT_H;
        search_button_rect.top = MMIWWW_SEARCH_IMAGE_TOP_H;
        search_button_rect.left = MMIWWW_SEARCH_IMAGE_LEFT_H;
        search_button_rect.right = MMIWWW_SEARCH_IMAGE_RIGHT_H;
    }

    if(GUI_PointIsInRect(point, link_button_rect))
    {
        if (!MMIWWW_NetfrontGetLinkButtonDisabled())
        {
            MMIWWW_NetfrontSetMainMenuFocusCtrl(MMIWWW_CTRL_LINK_BUTTON);
            MMIWWW_MainMenuHandleLinkButtonPress(win_id);
        }
    }
    else if (GUI_PointIsInRect(point, search_button_rect))
    {
        if (!MMIWWW_NetfrontGetSearchButtonDisabled())
        {
            MMIWWW_NetfrontSetMainMenuFocusCtrl(MMIWWW_CTRL_SEARCH_BUTTON);
            MMIWWW_MainMenuHandleSearchButtonPress(win_id);
        }
    }
    else
    {
        recode = MMI_RESULT_FALSE;
    }

    return recode;
}

/*****************************************************************************/
//  Description : Handle Tp released
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWWW_MainMenuHandleTPPressDown(MMI_WIN_ID_T win_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    GUI_POINT_T point = {0};
    GUI_RECT_T address_rect = {0};
    GUI_RECT_T search_rect = {0};
    MMI_HANDLE_T address_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_MAINMENU_LABEL_ADDRESS_CTRL_ID);
    MMI_HANDLE_T search_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_MAINMENU_LABEL_SEARCH_CTRL_ID);
    GUI_RECT_T link_button_rect = {0};
    GUI_RECT_T search_button_rect = {0};

    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);

    link_button_rect.left = MMIWWW_LINK_IMAGE_LEFT;
    link_button_rect.top = MMIWWW_LINK_IMAGE_TOP;
    link_button_rect.right = MMIWWW_LINK_IMAGE_RIGHT;
    link_button_rect.bottom = MMIWWW_LINK_IMAGE_BOTTOM;

    search_button_rect.left = MMIWWW_SEARCH_IMAGE_LEFT;
    search_button_rect.top = MMIWWW_SEARCH_IMAGE_TOP;
    search_button_rect.right = MMIWWW_SEARCH_IMAGE_RIGHT;
    search_button_rect.bottom = MMIWWW_SEARCH_IMAGE_BOTTOM;

    GUILABEL_GetRect(address_handle, &address_rect);
    GUILABEL_GetRect(search_handle, &search_rect);

    if (MMITHEME_IsMainScreenLandscape())
    {
        link_button_rect.top = MMIWWW_LINK_IMAGE_TOP_H;
        link_button_rect.left = MMIWWW_LINK_IMAGE_LEFT_H;
        link_button_rect.right = MMIWWW_LINK_IMAGE_RIGHT_H;
        search_button_rect.top = MMIWWW_SEARCH_IMAGE_TOP_H;
        search_button_rect.left = MMIWWW_SEARCH_IMAGE_LEFT_H;
        search_button_rect.right = MMIWWW_SEARCH_IMAGE_RIGHT_H;
    }

    if(GUI_PointIsInRect(point, address_rect))
    {
        MMIWWW_NetfrontSetMainMenuFocusCtrl(MMIWWW_CTRL_ADDRESS_LABEL);
        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
    }
    else if(GUI_PointIsInRect(point, search_rect))
    {
        MMIWWW_NetfrontSetMainMenuFocusCtrl(MMIWWW_CTRL_SEARCH_LABEL);
        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
    }
    else if(GUI_PointIsInRect(point, link_button_rect))
    {
        if (!MMIWWW_NetfrontGetLinkButtonDisabled())
        {
            uint32 temp_ctrl_id = MMIWWW_CTRL_LINK_BUTTON;
            MMIWWW_NetfrontSetMainMenuFocusCtrl(MMIWWW_CTRL_LINK_BUTTON);
            MMK_SendMsg(win_id, MSG_FULL_PAINT, (DPARAM)&temp_ctrl_id);
        }
        else
        {
            return MMI_RESULT_FALSE;
        }
    }
    else if (GUI_PointIsInRect(point, search_button_rect))
    {
        if (!MMIWWW_NetfrontGetSearchButtonDisabled())
        {
            uint32 temp_ctrl_id = MMIWWW_CTRL_SEARCH_BUTTON;
            MMIWWW_NetfrontSetMainMenuFocusCtrl(MMIWWW_CTRL_SEARCH_BUTTON);
            MMK_SendMsg(win_id, MSG_FULL_PAINT, (DPARAM)&temp_ctrl_id);
        }
        else
        {
            return MMI_RESULT_FALSE;
        }
    }
    else
    {
        return MMI_RESULT_FALSE;
    }

    return recode;
}

/*****************************************************************************/
//  Description : Create Sub main menu
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWWW_OpenWWWMainmenuOptWin(BOOLEAN is_web_open)
{
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};

    win_table_create.applet_handle = MMIWWW_HANDLE(MMIWWW_APPLETINSTANCE());
    win_table_create.parent_win_handle = 0;
    win_table_create.win_table_ptr = is_web_open ? MMIWWW_MAINMENU_OPT_WIN_TAB2 : MMIWWW_MAINMENU_OPT_WIN_TAB1;
    win_table_create.add_data_ptr = (ADD_DATA)(is_web_open ? MENU_WWW_MAINMENU_OPT2 : MENU_WWW_MAINMENU_OPT1);

    MMK_CreateWinTable( &win_table_create );

    return MMI_RESULT_TRUE;
}


/*****************************************************************************/
//  Description : Create InlineIME
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIWWW_InlineIME_EnterTextControl(NFBrowserControlInfo *in_info)
{
    MMI_HANDLE_T win_handle = 0;
    MMI_CONTROL_CREATE_T create = {0};
    MMI_HANDLE_T ctrl_handle = PNULL;
    GUIEDIT_INIT_DATA_T init_data = {0};
    MMI_WINDOW_CREATE_T win_create = {0};
    GUI_RECT_T editbox_rect = {0, 0, 0, 0};
    uint16 max_input_len = 0;
    GUIEDIT_TYPE_E type = GUIEDIT_TYPE_NULL;
    void* edit_box_data = PNULL;
    MMIWWW_INSTANCE_T *self = MMIWWW_INSTANCE();
    wchar *wstr_ptr = PNULL; 
    uint16 wstr_len = 0; 
    GUIIM_TYPE_T allow_im = GUIIM_TYPE_NONE;
    GUIIM_TYPE_T init_im = GUIIM_TYPE_NONE;

    if (PNULL == in_info || PNULL == self)
    {
        return MMI_RESULT_FALSE;
    }

    PEER_DP(("MMIWWW_InlineIME_EnterTextControl Type=%d,Font=%d, Max=%d,Restriction=%d", in_info->fInputType, in_info->fFont, in_info->fMaxLength,in_info->fInputRestriction));

    switch (in_info->fInputType)
    {
    case SLIM_PAGEINPUT_TEXT:
    case SLIM_PAGEINPUT_TEXTAREA:
        type = GUIEDIT_TYPE_TEXT;
        allow_im = GUIIM_TYPE_SET_ALL;
        //init_im = GUIIM_TYPE_DEFAULT;
        init_im = GUIIM_TYPE_NONE;
        break;

    case SLIM_PAGEINPUT_PASSWORD:
        type = GUIEDIT_TYPE_PASSWORD;
        allow_im = GUIIM_TYPE_ENGLISH | GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL;//lint !e655
        init_im = GUIIM_TYPE_ABC;
        break;

    default:
        return MMI_RESULT_FALSE;
    }

    if (PNULL != in_info->fInputRestrictionArgs)
    {
        PEER_DP(("MMIWWW_InlineIME_EnterTextControl fInputRestrictionArgs=%s", in_info->fInputRestrictionArgs));
    }

    switch (in_info->fInputRestriction)
    {
    case SLIM_AWS_INPUT_REST_ZEN_KANA:
        allow_im = GUIIM_TYPE_ABC;
        init_im = GUIIM_TYPE_ABC;
        break;

    case SLIM_AWS_INPUT_REST_HAN_KANA:
        allow_im = GUIIM_TYPE_DEFAULT;
        init_im = GUIIM_TYPE_DEFAULT;
        break;

    case SLIM_AWS_INPUT_REST_ALPHA:
        allow_im = GUIIM_TYPE_ABC;
        init_im = GUIIM_TYPE_ABC;
        break;

    case SLIM_AWS_INPUT_REST_NUM:
        allow_im = GUIIM_TYPE_DIGITAL;
        init_im = GUIIM_TYPE_DIGITAL;
        break;

    default:
        break;
    }
    
    win_create.applet_handle = MMIWWW_HANDLE(MMIWWW_APPLETINSTANCE());
    win_create.win_id = MMIWWW_INLINE_IME_WIN_ID;
    win_create.func = HandleWWWInlineIMEWinMsg;
    win_create.win_priority = WIN_ONE_LEVEL;
    win_create.add_data_ptr = (ADD_DATA)type ;
    
    win_handle = MMK_CreateWindow( &win_create );

    if (PNULL == win_handle)
    {
        return MMI_RESULT_FALSE;
    }

#ifdef MMI_PDA_SUPPORT
    GUIWIN_CreateStbDyn(win_handle);
#endif

    self->ime_win_id = win_handle;

    //set softkey
    CreatSoftkeyCtrl(win_handle, TXT_COMMON_OK, TXT_NULL, STXT_RETURN);

    //set title
    GUIWIN_CreateTitleDyna(win_handle, TXT_NULL);

    GUIWIN_SetTitleTextId(win_handle, TXT_EDIT, FALSE);

    editbox_rect = MMITHEME_GetClientRect();

    if (0 >= in_info->fMaxLength)
    {
        max_input_len = MMIWWW_INLINE_IME_DEFAULT_INPUT;
    }
    else
    {
        max_input_len = (uint16)in_info->fMaxLength;
    }

    if (PNULL != self->ime_endparam.ime_result_text)
    {
        SCI_FREE(self->ime_endparam.ime_result_text);
        self->ime_endparam.ime_result_text = PNULL;
    }
    self->ime_endparam.ime_result_bytes = 0;

    if (PNULL != in_info->fText)
    {
        uint16 len = 0;

        len = (uint16)strlen((char *)in_info->fText);

        PEER_DP(("MMIWWW_InlineIME_EnterTextControl len=%d", len));

        self->ime_endparam.ime_result_bytes = len;
        self->ime_endparam.ime_result_text = SCI_ALLOCA(self->ime_endparam.ime_result_bytes + 1);

        if (PNULL == self->ime_endparam.ime_result_text)
        {
            return MMI_RESULT_FALSE;
        }
        
        SCI_MEMSET(self->ime_endparam.ime_result_text, 0x00, (self->ime_endparam.ime_result_bytes + 1));

        SCI_MEMCPY(self->ime_endparam.ime_result_text, in_info->fText, len);

        wstr_len = (uint16)((len + 1) * sizeof(wchar));
        wstr_ptr = SCI_ALLOCA(wstr_len);

        if (PNULL == wstr_ptr)
        {
            return MMI_RESULT_FALSE;
        }

        SCI_MEMSET(wstr_ptr, 0x00, wstr_len);
        wstr_len = GUI_UTF8ToWstr(wstr_ptr, wstr_len, in_info->fText, len);
    }

    init_data.type = type;
    init_data.both_rect.h_rect = init_data.both_rect.v_rect = editbox_rect;

    create.ctrl_id = MMIWWW_INLINE_IME_CTRL_ID;
    create.guid = SPRD_GUI_EDITBOX_ID;
    create.parent_win_handle = win_handle;
    create.init_data_ptr = &init_data;

    edit_box_data = MMK_CreateControl(&create);

    ctrl_handle = MMK_GetCtrlHandleByWin(MMK_GetWinHandle(MMIWWW_HANDLE(MMIWWW_APPLETINSTANCE()), MMIWWW_INLINE_IME_WIN_ID), MMIWWW_INLINE_IME_CTRL_ID); 
    
    //set max len
    if( SLIM_PAGEINPUT_PASSWORD == in_info->fInputType )
    {
        BOOLEAN is_disp_im_icon = TRUE;
        BOOLEAN is_disp_num_info = TRUE;

        GUIEDIT_SetMaxLen(ctrl_handle, max_input_len);
        GUIEDIT_SetStyle(ctrl_handle, GUIEDIT_STYLE_SINGLE);
        GUIEDIT_SetDispImIcon(ctrl_handle, &is_disp_im_icon, &is_disp_num_info);
    }
    else
    {
        GUIEDIT_SetTextMaxLen(ctrl_handle, max_input_len, max_input_len);
    }    

    //set im
    GUIEDIT_SetIm(ctrl_handle, allow_im, init_im);

    //set string
    if (PNULL != wstr_ptr && 0 != wstr_len)
    {
        GUIEDIT_SetString(ctrl_handle, wstr_ptr, wstr_len);
    }

    if (PNULL != wstr_ptr)
    {
        SCI_FREE(wstr_ptr);
        wstr_ptr = PNULL;
    }

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : Get InlineIME Value
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC slim_bool MMIWWW_InlineIME_GetTextControlValue(slim_char *out_string)
{
    MMIWWW_INSTANCE_T *self = MMIWWW_INSTANCE();

    if (PNULL != self && PNULL != self->ime_endparam.ime_result_text && PNULL != out_string)
    {
        SCI_MEMCPY(out_string, self->ime_endparam.ime_result_text, self->ime_endparam.ime_result_bytes);

        return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : Get InlineIME Value Length
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC slim_int MMIWWW_InlineIME_GetTextControlValueLength(void)
{
    MMIWWW_INSTANCE_T *self = MMIWWW_INSTANCE();

    if (PNULL != self)
    {
        return self->ime_endparam.ime_result_bytes;
    }

    return 0;
}

/*****************************************************************************/
//  Description : Create WWW set menu win
//  Global resource dependence : 
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIWWW_OpenWWWSetMenuWin(void)
{
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};

    win_table_create.applet_handle = MMIWWW_HANDLE(MMIWWW_APPLETINSTANCE());
    win_table_create.parent_win_handle = 0;
    win_table_create.win_table_ptr = MMIWWW_SETMENU_WIN_TAB;

    MMK_CreateWinTable( &win_table_create );
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : Create WWW history win
//  Global resource dependence : 
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIWWW_OpenWWWPageMemoWin(void)
{
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};

    win_table_create.applet_handle = MMIWWW_HANDLE(MMIWWW_APPLETINSTANCE());
    win_table_create.parent_win_handle = 0;
    win_table_create.win_table_ptr = MMIWWW_PAGEMEMO_WIN_TAB;

    MMK_CreateWinTable( &win_table_create );
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : Create WWW Certificate win
//  Global resource dependence : 
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIWWW_OpenWWWCertificateWin(MMIWWW_CERT_MODE_E mode)
{
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};

    win_table_create.applet_handle = MMIWWW_HANDLE(MMIWWW_APPLETINSTANCE());
    win_table_create.parent_win_handle = 0;
    win_table_create.win_table_ptr = MMIWWW_CERTIFICATE_WIN_TAB;
    win_table_create.add_data_ptr = (ADD_DATA)mode;

    MMK_CreateWinTable( &win_table_create );
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : Create WWW match key win
//  Global resource dependence : 
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIWWW_OpenWWWMatchKeyWin(MMI_STRING_T *info_ptr, uint16 num)
{
    wchar *wstr_ptr = PNULL;
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};
    GUI_RECT_T          list_rect = {0};
    MMI_HANDLE_T search_handle = PNULL;

    search_handle = MMK_GetCtrlHandleByWin(MMK_GetWinHandle(MMIWWW_HANDLE(MMIWWW_APPLETINSTANCE()), MMIWWW_MAINMENU_WIN_ID), MMIWWW_MAINMENU_LABEL_SEARCH_CTRL_ID);

    wstr_ptr = SCI_ALLOCA((info_ptr->wstr_len + 1) * sizeof(wchar));
    SCI_MEMSET(wstr_ptr, 0x00, ((info_ptr->wstr_len + 1) * sizeof(wchar)));
    MMI_WSTRNCPY( wstr_ptr,info_ptr->wstr_len,info_ptr->wstr_ptr,info_ptr->wstr_len,info_ptr->wstr_len);    

    win_table_create.applet_handle = MMIWWW_HANDLE(MMIWWW_APPLETINSTANCE());
    win_table_create.parent_win_handle = 0;
    win_table_create.win_table_ptr = MMIWWW_MATCHKEY_WIN_TAB;
    win_table_create.add_data_ptr = (ADD_DATA)wstr_ptr;
    MMK_CreateWinTable( &win_table_create );

    GUILABEL_GetRect(search_handle, &list_rect);
    list_rect.top = list_rect.bottom;
    num = (num > 3) ? 3 : num;
    list_rect.bottom += num * MMI_LIST_ITEM_HEIGHT;
    list_rect.bottom += MMI_LIST_EXPAND_ITEM_HEIGHT - MMI_LIST_ITEM_HEIGHT; /* Adjust expend height */

    GUILIST_SetRect(MMIWWW_LISTBOX_MATCHKEY_CTRL_ID,&list_rect);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : Create WWW match key win
//  Global resource dependence : 
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIWWW_OpenWWWMatchURLWin(MMI_STRING_T *info_ptr, uint16 num)
{
    wchar *wstr_ptr = PNULL;
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};
    GUI_RECT_T  list_rect = {0};
    MMI_HANDLE_T address_handle = PNULL;

    address_handle = MMK_GetCtrlHandleByWin(MMK_GetWinHandle(MMIWWW_HANDLE(MMIWWW_APPLETINSTANCE()), MMIWWW_MAINMENU_WIN_ID), MMIWWW_MAINMENU_LABEL_ADDRESS_CTRL_ID);

    wstr_ptr = SCI_ALLOCA((info_ptr->wstr_len + 1) * sizeof(wchar));
    SCI_MEMSET(wstr_ptr, 0x00, ((info_ptr->wstr_len + 1) * sizeof(wchar)));
    MMI_WSTRNCPY( wstr_ptr,info_ptr->wstr_len,info_ptr->wstr_ptr,info_ptr->wstr_len,info_ptr->wstr_len);

    win_table_create.applet_handle = MMIWWW_HANDLE(MMIWWW_APPLETINSTANCE());
    win_table_create.parent_win_handle = 0;
    win_table_create.win_table_ptr = MMIWWW_MATCHURL_WIN_TAB;
    win_table_create.add_data_ptr = (ADD_DATA)wstr_ptr;
    MMK_CreateWinTable( &win_table_create );

    GUILABEL_GetRect(address_handle, &list_rect);
    list_rect.top = list_rect.bottom;
    num = (num > 3) ? 3 : num;
    list_rect.bottom += num * MMI_LIST_ITEM_HEIGHT;
    list_rect.bottom += MMI_LIST_EXPAND_ITEM_HEIGHT - MMI_LIST_ITEM_HEIGHT; /* Adjust expend height */
    GUILIST_SetRect(MMIWWW_LISTBOX_MATCHURL_CTRL_ID,&list_rect);

    return MMI_RESULT_TRUE;
}


/*****************************************************************************/
//  Description : Create WWW history option win
//  Global resource dependence : 
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIWWW_OpenWWWHistoryOptWin(BOOLEAN is_web_open)
{
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};

    win_table_create.applet_handle = MMIWWW_HANDLE(MMIWWW_APPLETINSTANCE());
    win_table_create.parent_win_handle = 0;
    win_table_create.win_table_ptr = is_web_open ? MMIWWW_HISTORY_OPT_WIN_TAB2 : MMIWWW_HISTORY_OPT_WIN_TAB1;
    win_table_create.add_data_ptr = (ADD_DATA)(is_web_open ? MENU_WWW_HISTORY_OPT2 : MENU_WWW_HISTORY_OPT1);

    MMK_CreateWinTable( &win_table_create );
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : Create WWW pagememo option win
//  Global resource dependence : 
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIWWW_OpenWWWPageMemoOptWin(void)
{
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};

    win_table_create.applet_handle = MMIWWW_HANDLE(MMIWWW_APPLETINSTANCE());
    win_table_create.parent_win_handle = 0;
    win_table_create.win_table_ptr = MMIWWW_PAGEMEMO_OPT_WIN_TAB;

    MMK_CreateWinTable( &win_table_create );
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : Create WWW history attribute win
//  Global resource dependence : 
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIWWW_OpenWWWHistoryAttributeWin(MMIWWW_PAGEMEMO_ATTRIBUTE_TYPE_E type)
{
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};

    win_table_create.applet_handle = MMIWWW_HANDLE(MMIWWW_APPLETINSTANCE());
    win_table_create.parent_win_handle = 0;
    win_table_create.win_table_ptr = MMIWWW_HISTORY_ATTRIBUTE_WIN_TAB;
    win_table_create.add_data_ptr = (ADD_DATA)type;

    MMK_CreateWinTable( &win_table_create );
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : Create WWW certificate attribute win
//  Global resource dependence : 
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIWWW_OpenWWWCertificateAttributeWin(void)
{
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};

    win_table_create.applet_handle = MMIWWW_HANDLE(MMIWWW_APPLETINSTANCE());
    win_table_create.parent_win_handle = 0;
    win_table_create.win_table_ptr = MMIWWW_CERTIFICATE_ATTRIBUTE_WIN_TAB;
    
    MMK_CreateWinTable( &win_table_create );
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : Create WWW set win
//  Global resource dependence : 
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIWWW_OpenWWWSetWin(MMIWWW_ID_MENU_E menu_id)
{
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};

    win_table_create.applet_handle = MMIWWW_HANDLE(MMIWWW_APPLETINSTANCE());
    win_table_create.parent_win_handle = 0;
    if(ID_WWW_SETMENU_DISPALAYOPT == menu_id)
    {
        win_table_create.win_table_ptr = MMIWWW_SET_WIN_TAB1;
    }
    else if(ID_WWW_SETMENU_SETCACHE == menu_id)
    {
        win_table_create.win_table_ptr = MMIWWW_SET_WIN_TAB2;
    }
    else if(ID_WWW_SETMENU_SETCOOKIE == menu_id)
    {
        win_table_create.win_table_ptr = MMIWWW_SET_WIN_TAB3;
    }
    else if(ID_WWW_SETMENU_WEBDATA == menu_id)
    {
        win_table_create.win_table_ptr = MMIWWW_SET_WIN_TAB4;
    }
    else
    {
        win_table_create.win_table_ptr = MMIWWW_SET_WIN_TAB;
    }    
    win_table_create.add_data_ptr = (ADD_DATA)menu_id;

    MMK_CreateWinTable( &win_table_create );
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : handle message of  main menu of browser
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWMainMenuWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    uint16          num = 0;
    MMI_STRING_T    *edit_info_ptr = PNULL;
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"mmiwww_wintab.c:HandleWWWMainMenuWinMsg() msg_id=0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_3478_112_2_18_3_12_7_178,(uint8*)"d", msg_id);

    //bookmark window need handle move to
    if (HandleWWWMainMenuInBMMove(win_id, msg_id, param))
    {
        return recode;
    }

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUI_BOTH_RECT_T address_lab_rect = {0};
            GUI_BOTH_RECT_T serch_lab_rect = {0};
            GUI_BOTH_RECT_T tab_rect = {0};

            address_lab_rect.v_rect.left = MMIWWW_ADDRESSBAR_LEFT;
            address_lab_rect.v_rect.top = MMIWWW_ADDRESSBAR_TOP;
            address_lab_rect.v_rect.right = MMIWWW_ADDRESSBAR_RIGHT;
            address_lab_rect.v_rect.bottom = MMIWWW_ADDRESSBAR_BOTTOM;
            address_lab_rect.h_rect.left = MMIWWW_ADDRESSBAR_LEFT;
            address_lab_rect.h_rect.top = MMIWWW_ADDRESSBAR_TOP_H;
            address_lab_rect.h_rect.right = MMIWWW_ADDRESSBAR_RIGHT_H;
            address_lab_rect.h_rect.bottom = MMIWWW_ADDRESSBAR_BOTTOM_H;

            serch_lab_rect.v_rect.left = MMIWWW_SEARCHBAR_LEFT;
            serch_lab_rect.v_rect.top = MMIWWW_SEARCHSBAR_TOP;
            serch_lab_rect.v_rect.right = MMIWWW_SEARCHSBAR_RIGHT;
            serch_lab_rect.v_rect.bottom = MMIWWW_SEARCHSBAR_BOTTOM;
            serch_lab_rect.h_rect.left = MMIWWW_SEARCHBAR_LEFT;
            serch_lab_rect.h_rect.top = MMIWWW_SEARCHSBAR_TOP_H;
            serch_lab_rect.h_rect.right = MMIWWW_SEARCHSBAR_RIGHT_H;
            serch_lab_rect.h_rect.bottom = MMIWWW_SEARCHSBAR_BOTTOM_H;

            tab_rect.v_rect.left = MMIWWW_TAB_LEFT;
            tab_rect.v_rect.top = MMIWWW_TAB_TOP;
            tab_rect.v_rect.right = MMIWWW_TAB_RIGHT;
            tab_rect.v_rect.bottom = MMIWWW_TAB_BOTTOM;
            tab_rect.h_rect.left = MMIWWW_TAB_LEFT;
            tab_rect.h_rect.top = MMIWWW_TAB_TOP_H;
            tab_rect.h_rect.right = MMIWWW_TAB_RIGHT_H;
            tab_rect.h_rect.bottom = MMIWWW_TAB_BOTTOM_H;

            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIWWW_MAINMENU_LABEL_ADDRESS_CTRL_ID), &address_lab_rect);
            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIWWW_MAINMENU_LABEL_SEARCH_CTRL_ID), &serch_lab_rect);
            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIWWW_MAINMENU_TAB_CTRL_ID), &tab_rect);
            MMIWWW_NetfrontSetMainMenuFocusState(TRUE);
#ifdef MMI_PDA_SUPPORT
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
            recode = MMIWWW_MainMenuHandleOpenWindow(win_id);
        }
        break;

    case MSG_FULL_PAINT:
        recode = MMIWWW_MainMenuHandleFullPaint(win_id, param);
        break;

    case MSG_GET_FOCUS:
        break;

    case MSG_WWW_LIST_MATCHKEY:
        edit_info_ptr = (MMI_STRING_T *)param;
        num = MMIWWW_GetMatchKeyNum(edit_info_ptr->wstr_ptr);
        MMIWWW_OpenWWWMatchKeyWin(edit_info_ptr, num);
/*        num = MMIWWW_GetMatchKeyNum(edit_info_ptr->wstr_ptr);
        if(0!= num)
        {
            MMIWWW_OpenWWWMatchKeyWin(edit_info_ptr, num);
        }*/
        break;

    case MSG_WWW_LIST_MATCHURL:
        edit_info_ptr = (MMI_STRING_T *)param;
        num = MMIWWW_GetMatchURLNum(edit_info_ptr->wstr_ptr);
        MMIWWW_OpenWWWMatchURLWin(edit_info_ptr, num);
/*        num = MMIWWW_GetMatchURLNum(edit_info_ptr->wstr_ptr);
        if(0!= num)
        {
            MMIWWW_OpenWWWMatchURLWin(edit_info_ptr, num);
        }*/
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        recode = MMIWWW_MainMenuHandleTPPressUp(win_id, param);            
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        recode = MMIWWW_MainMenuHandleTPPressDown(win_id, param);            
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        /*Need to be modified*/
        {
            MMIWWW_CTRL_E focus_ctrl = MMIWWW_NetfrontGetMainMenuFocusCtrl();

            if(MMIWWW_CTRL_BOOKMARK_LIST == focus_ctrl || MMIWWW_CTRL_HISTORY_LIST == focus_ctrl)
            {
                return MMI_RESULT_FALSE;
            }
            else
            {
                if (MMK_IsOpenWin(mmiwww_GetBrowserWinhandle()))
                {
                    MMIPUB_OpenQueryWinByTextId(TXT_EXIT_BROWSER, IMAGE_PUBWIN_QUERY,PNULL,PNULL);
                }
                else
                {
                    MMIAPIWWW_StopWWW();
                }
            }
        }
        break;

    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
        break;

    case MSG_PROMPTWIN_OK:
        MMIPUB_CloseQuerytWin(PNULL);
        MMIAPIWWW_StopWWW();
        break;

#ifdef WIN32
    case MSG_APP_1:
#ifdef BJ_DEV
        MMIWWW_OpenWWWBrowserWin(BROWSER_START_BY_URL, "http://172.16.74.20:8082/");
#else
        MMIWWW_OpenWWWBrowserWin(BROWSER_START_BY_URL, "http://172.16.14.173:8082/");
#endif
        break;

    case MSG_APP_2:
#ifdef BJ_DEV
        MMIWWW_OpenWWWBrowserWin(BROWSER_START_BY_URL, "http://172.16.74.20:8082/html/google.cn.html");
#else
        MMIWWW_OpenWWWBrowserWin(BROWSER_START_BY_URL, "http://172.16.14.173:8082/image/");
#endif
        break;

    case MSG_APP_3:
#ifdef BJ_DEV
        MMIWWW_OpenWWWBrowserWin(BROWSER_START_BY_URL, "http://172.16.74.20:8082/html/wap Google.htm");
#else
        MMIWWW_OpenWWWBrowserWin(BROWSER_START_BY_URL, "http://www.google.cn/");
#endif
        break;

    case MSG_APP_4:
#ifdef BJ_DEV
        MMIWWW_OpenWWWBrowserWin(BROWSER_START_BY_URL, "http://172.16.74.20:8082/html/baidu_new_lite.htm");
#else
        MMIWWW_OpenWWWBrowserWin(BROWSER_START_BY_URL, "http://www.sina.com.cn/");
#endif
        break;

    case MSG_APP_5:
        MMIWWW_OpenWWWBrowserWin(BROWSER_START_NORMAL, PNULL);
        break;

#endif

/*
    case MSG_APP_STAR:
        {
            MMI_HANDLE_T applet_handle = MMIWWW_HANDLE((WWW_APPLET_T *)MMK_GetAppletPtrByWin(win_id));

            if (MMK_IsOpenWin(mmiwww_GetBrowserWinhandle()))    
            {
                MMK_PostMsg(MMK_GetWinHandle(applet_handle, MMIWWW_MAINMENU_WIN_ID), MSG_WWW_NOTIFY_CHANGE_WIN, PNULL, 0);
            }
        }
        break;
*/
    case MSG_APP_HASH:
        return MMI_RESULT_TRUE;

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        recode = MMIWWW_MainMenuHandleWebKey(win_id);
        break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_MENU:
        {
            MMIWWW_CTRL_E focus_ctrl = MMIWWW_NetfrontGetMainMenuFocusCtrl();

            if(MMIWWW_CTRL_TAB == focus_ctrl || MMIWWW_CTRL_BOOKMARK_LIST == focus_ctrl || MMIWWW_CTRL_HISTORY_LIST == focus_ctrl)
            {
                return MMI_RESULT_FALSE;
            }

            MMIWWW_OpenWWWMainmenuOptWin(MMK_IsOpenWin(mmiwww_GetBrowserWinhandle()));
        }    
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
            MMI_HANDLE_T address_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_MAINMENU_LABEL_ADDRESS_CTRL_ID);
            MMI_HANDLE_T search_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_MAINMENU_LABEL_SEARCH_CTRL_ID);
            MMI_NOTIFY_T *notify = param;

            if (PNULL != notify)
            {
                if(address_handle == notify->src_handle)
                {
                    MMIWWW_OpenEditWin(win_id, MMIWWW_MAINMENU_LABEL_ADDRESS_CTRL_ID);
                }
                else if(search_handle == notify->src_handle)
                {
                    MMIWWW_OpenEditWin(win_id, MMIWWW_MAINMENU_LABEL_SEARCH_CTRL_ID);
                }
            }
            recode = MMI_RESULT_FALSE;
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_KEYDOWN_UP:
    case MSG_KEYDOWN_DOWN:
    case MSG_KEYDOWN_LEFT:
    case MSG_KEYDOWN_RIGHT:
        recode = MMIWWW_MainMenuHandleDirectionKey(win_id, msg_id);
        break;

    case MSG_CLOSE_WINDOW:
        break;

    case MSG_NOTIFY_TAB_SWITCH:
        {
            MMI_HANDLE_T applet_handle = MMIWWW_HANDLE((WWW_APPLET_T *)MMK_GetAppletPtrByWin(win_id));

            if (0 == GUITAB_GetCurSel(MMK_GetCtrlHandleByWin(win_id, MMIWWW_MAINMENU_TAB_CTRL_ID)))
            {
                MMIWWW_NetfrontSetMainMenuFocusCtrl(MMIWWW_CTRL_BOOKMARK_LIST);
            }
            else
            {
                MMIWWW_NetfrontSetMainMenuFocusCtrl(MMIWWW_CTRL_HISTORY_LIST);
            }

            MMK_PostMsg(MMK_GetWinHandle(applet_handle, MMIWWW_MAINMENU_WIN_ID), MSG_FULL_PAINT, PNULL, 0);        
        }
        break;

    case MSG_WWW_NOTIFY_CHANGE_WIN:
        MMK_WinGrabFocus(mmiwww_GetBrowserWinhandle());
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
// Description : Get browser win handle
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL MMI_HANDLE_T mmiwww_GetBrowserWinhandle(void)
{
    MMI_HANDLE_T    applet_handle = PNULL;

    applet_handle = MMIWWW_HANDLE(MMIWWW_APPLETINSTANCE());
    return MMK_GetWinHandle(applet_handle, MMIWWW_BROWSER_WIN_ID);
}

/*****************************************************************************/
//  Description : Create Browser Win
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIWWW_OpenWWWBrowserWin(NFBrowserStartType in_type, char *in_url)
{
    MMIWWW_PAGEINFO_T        *pageinfo_ptr = PNULL;
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};
    MMI_HANDLE_T              win_handle = PNULL;
    slim_int                  ret = 0;
    MMIPUB_HANDLE_FUNC handle_end_proc = PNULL;
    uint8 * url = PNULL;
    MMI_RESULT_E is_created = MMI_RESULT_FALSE;
    MMIWWW_INSTANCE_T *self = MMIWWW_INSTANCE();

    //SCI_TRACE_LOW:"MMIWWW_OpenWWWBrowserWin in_type=%d, in_url=%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_3768_112_2_18_3_12_8_179,(uint8*)"ds", in_type, in_url);

    //提示SIM卡状态(打开空白页不加提示)
    if (PNULL != self && (MMIPDP_INTERFACE_WIFI != self->customized_connection.bearer_type))
    {
        //提示SIM卡状态(打开空白页不加提示)
        if ((BROWSER_START_NORMAL != in_type) 
            && ((self->customized_connection.dual_sys >= MMI_DUAL_SYS_MAX)
            || (!MMIAPIPHONE_IsSimAvailable(self->customized_connection.dual_sys)))
            )
        {
            MMIAPIPHONE_AlertSimNorOKStatus();
            return MMI_RESULT_FALSE;
        }
    }

    if (PNULL != in_url)
    {
        while (' ' == *in_url)
        {
            in_url++;
        }

        if (MMIWWW_HandleExScheme((uint8 *)in_url))
        {
           return MMI_RESULT_TRUE; /* Do not stop applet */
        }

        url = SCI_ALLOCAZ(MAX_WEB_URL_LEN + 1);

        if (PNULL == url)
        {
            return MMI_RESULT_FALSE;
        }

        MMIAPICOM_NormalizeUrl(in_url, url, MAX_WEB_URL_LEN, TRUE);
    }

    if (MMIWWW_IsFirstOpen())
    {
        MMIWWW_SaveUrl(in_type, (char *)url);

        if (BROWSER_START_NORMAL == in_type)
        {
            handle_end_proc = mmiwww_HandleNetAccessPagememo;
        }
        else
        {
            handle_end_proc = mmiwww_HandleNetAccess;
        }
        MMIPUB_OpenQueryWinByTextId(TXT_WWW_ALLOW_NET_ACCESS, IMAGE_PUBWIN_QUERY,
                                    PNULL, handle_end_proc);
        if (PNULL != url)
        {
            SCI_FREE(url);
            url = PNULL;
        }
        return MMI_RESULT_TRUE;
    }

    //浏览网页窗口是否已打开
    win_handle = mmiwww_GetBrowserWinhandle();
    if (!MMK_IsOpenWin(win_handle))
    {
        ret = MMIWWW_Netfront_StartBrowser(MMIWWW_INSTANCE(), in_type, (char *)url);
        if (SLIM_E_OK == ret)
        {
            //打开网页窗口
            pageinfo_ptr = SCI_ALLOCA(sizeof(MMIWWW_PAGEINFO_T));
            SCI_MEMSET(pageinfo_ptr,0,sizeof(MMIWWW_PAGEINFO_T));
            win_table_create.applet_handle = MMIWWW_HANDLE(MMIWWW_APPLETINSTANCE());
            win_table_create.parent_win_handle = 0;
            win_table_create.win_table_ptr = MMIWWW_BROWSER_WIN_TAB;
            win_table_create.add_data_ptr = pageinfo_ptr;
            MMK_CreateWinTable( &win_table_create );
            //初始化页面大小
            ret = MMIWWW_Netfront_Resize();
            //设置浏览器状态
            if (PNULL != url)
            {
                MMIWWW_SetWWWState(MMIWWW_Netfront_GetActWinIndex(), MMIWWW_STATE_PAGE_LOADING);
            }
            else
            {
                MMIWWW_SetWWWState(MMIWWW_Netfront_GetActWinIndex(), MMIWWW_STATE_PAGE_VIEW);
            }

            MMIWWW_Netfront_SetPointerMode(MMIWWW_GetPointerMode());
            is_created = MMI_RESULT_TRUE;
        }
        else
        {
            MMIPUB_OpenAlertWarningWin( TXT_UNKNOWN_ERROR);
            is_created = MMI_RESULT_FALSE;
        }
    }
    else
    {
        MMK_WinGrabFocus(win_handle);
        //此处不做判断，无论创成与否，均在当前激活页面中加载
        if (MMIWWW_GetIsCreateNewWin())
        {
            MMIWWW_CreateBrowserTab();
        }

        MMIWWW_SetIsCreateNewWin(TRUE);

        ret = MMIWWW_Netfront_SetLocation((slim_char *)url);
        if (SLIM_E_OK == ret)
        {
            //设置浏览器状态
            if (PNULL != url)
            {
                MMIWWW_SetWWWState(MMIWWW_Netfront_GetActWinIndex(), MMIWWW_STATE_PAGE_LOADING);
            }
            else
            {
                MMIWWW_SetWWWState(MMIWWW_Netfront_GetActWinIndex(), MMIWWW_STATE_PAGE_VIEW);
            }
        }
        else
        {
            MMIPUB_OpenAlertWarningWin( TXT_UNKNOWN_ERROR);
        }
        is_created = MMI_RESULT_TRUE;
    }
    if (PNULL != url)
    {
        SCI_FREE(url);
        url = PNULL;
    }
    return is_created;
}

/*****************************************************************************/
// Description : Handle query allow access network win
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E mmiwww_HandleNetAccess(MMI_WIN_ID_T      win_id, 
                                          MMI_MESSAGE_ID_E  msg_id, 
                                          DPARAM            param)
{
    MMI_RESULT_E          result = MMI_RESULT_TRUE;
    NFBrowserStartType    type = BROWSER_START_NORMAL;
    uint8                 *url = PNULL;

    //SCI_TRACE_LOW:"mmiwww_HandleNetAccess() msg_id=0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_3956_112_2_18_3_12_8_180,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        MMK_CloseWin(win_id);
        MMIWWW_SetIsFirstOpen(FALSE);
        url = MMIWWW_GetSavedUrl(&type);
        MMIWWW_OpenWWWBrowserWin(type, (char *)url);
        if (PNULL != url)
        {
            SCI_FREE(url);
        }
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        {
            MMI_HANDLE_T applet_handle = PNULL;
            applet_handle = MMIWWW_HANDLE(MMIWWW_APPLETINSTANCE());

            MMK_CloseWin(win_id);
            if (!MMK_IsOpenWin(MMK_GetWinHandle(applet_handle, MMIWWW_MAINMENU_WIN_ID)))
            {
                MMIAPIWWW_StopWWW();
            }
        }
#ifdef JAVA_SUPPORT_SUN
        MMIAPIJAVA_JavaCallClose(TRUE);
#endif
        break;
     case MSG_APP_RED:
        MMK_CloseWin(win_id);
#ifdef JAVA_SUPPORT_SUN
        MMIAPIJAVA_JavaCallClose(FALSE);
#endif
        break;

    default:
        result = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }
    return (result);
}

/*****************************************************************************/
// Description : Handle query allow access pagememo
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E mmiwww_HandleNetAccessPagememo(MMI_WIN_ID_T      win_id, 
                                          MMI_MESSAGE_ID_E  msg_id, 
                                          DPARAM            param)
{
    MMI_RESULT_E          result = MMI_RESULT_TRUE;
    NFBrowserStartType    type = BROWSER_START_NORMAL;
    uint8                 *url = PNULL;
    MMI_HANDLE_T page_memo_handle = PNULL;
    MMI_HANDLE_T attribute_handle = PNULL;
    MMI_HANDLE_T list_ctrl_handle = PNULL;
    uint16 selected_item = 0;
    MMI_HANDLE_T applet_handle = MMIWWW_HANDLE(MMIWWW_APPLETINSTANCE()); /* Do Not use win_id */

    //SCI_TRACE_LOW:"mmiwww_HandleNetAccessPagememo() msg_id=0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_4019_112_2_18_3_12_8_181,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        MMK_CloseWin(win_id);
        MMIWWW_SetIsFirstOpen(FALSE);
        url = MMIWWW_GetSavedUrl(&type);
        MMIWWW_OpenWWWBrowserWin(type, (char *)url);

        page_memo_handle = MMK_GetWinHandle(applet_handle, MMIWWW_PAGEMEMO_WIN_ID);
        attribute_handle = MMK_GetWinHandle(applet_handle, MMIWWW_HISTORY_ATTRIBUTE_WIN_ID);
        list_ctrl_handle = MMK_GetCtrlHandleByWin(page_memo_handle, MMIWWW_LISTBOX_PAGEMEMO_CTRL_ID);
        selected_item = GUILIST_GetCurItemIndex(list_ctrl_handle);

        MMIWWW_Netfront_LoadPageMemo(selected_item);
        MMIWWW_SetWWWState(MMIWWW_Netfront_GetActWinIndex(), MMIWWW_STATE_PAGE_LOADING);
        if (MMK_IsOpenWin(page_memo_handle))
        {
            MMK_CloseWin(page_memo_handle);
        }
        if (MMK_IsOpenWin(attribute_handle))
        {
            MMK_CloseWin(attribute_handle);
        }

        if (PNULL != url)
        {
            SCI_FREE(url);
        }
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        result = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }
    return (result);
}

/*****************************************************************************/
// Description : Handle query delet history
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWWW_HandleDelHistory(MMI_WIN_ID_T      win_id, 
                                          MMI_MESSAGE_ID_E  msg_id, 
                                          DPARAM            param)
{
    MMI_RESULT_E          result = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"MMIWWW_HandleDelHistory() msg_id=0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_4074_112_2_18_3_12_8_182,(uint8*)"d", msg_id);

    switch(msg_id)
    {
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        {
            MMIWWW_CTRL_E focus_ctrl = MMIWWW_CTRL_NULL;

            focus_ctrl = MMIWWW_NetfrontGetMainMenuFocusCtrl();
            MMK_CloseWin(win_id);
            if (MMIWWW_CTRL_ADDRESS_LABEL == focus_ctrl)
            {
                MMIWWW_Netfront_RemoveAllInputURLHistory();
            }
            else if (MMIWWW_CTRL_SEARCH_LABEL == focus_ctrl)
            {
                MMIWWW_KEY_T key_data = {0};

                MMINV_WRITE(MMINV_WWW_KEY, &key_data );
            }
            else
            {
                //SCI_TRACE_LOW:"MMIWWW_HandleDelHistory Error"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_4098_112_2_18_3_12_8_183,(uint8*)"");
            }
            MMIPUB_OpenAlertSuccessWin(TXT_DELETED);
        }
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        result = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }
    return (result);
}
/*****************************************************************************/
//  Description : handle message of browser
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWBrowserWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    //SCI_TRACE_LOW:"HandleWWWBrowserWinMsg() msg_id=0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_4122_112_2_18_3_12_9_184,(uint8*)"d", msg_id);

    if (msg_id != MSG_APP_5 && /* refresh when loading*/
        MMIWWW_STATE_PAGE_VIEW != MMIWWW_GetWWWState(MMIWWW_Netfront_GetActWinIndex()) && (MSG_APP_1 <= msg_id && MSG_APP_HASH >= msg_id))
    {
        //SCI_TRACE_LOW:"HandleWWWBrowserWinMsg() No action"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_4127_112_2_18_3_12_9_185,(uint8*)"");
        return MMI_RESULT_TRUE;
    }

    switch(msg_id)
    {
#if 0
    case MSG_APP_7:
        {
            slim_char    message[] = "java script prompt";
            slim_char    value[] = "tester ";
            MMIWWW_OpenJSPromptWin(message, value);
        }
        break;
#endif
    case MSG_OPEN_WINDOW:
        {
            GUI_BG_T img_bg = {0};
            GUI_BOTH_RECT_T anim_rect = {{MMIWWW_LOAD_ANIM_LEFT, MMIWWW_LOAD_ANIM_TOP, MMIWWW_LOAD_ANIM_RIGHT, MMIWWW_LOAD_ANIM_BOTTOM}, 
                                         {MMIWWW_LOAD_ANIM_LEFT, MMIWWW_LOAD_ANIM_TOP, MMIWWW_LOAD_ANIM_RIGHT, MMIWWW_LOAD_ANIM_BOTTOM}};
            GUI_BOTH_RECT_T load_lab_rect = {{MMIWWW_LOAD_LABEL_LEFT, MMIWWW_LOAD_LABEL_TOP, MMIWWW_LOAD_LABEL_RIGHT, MMIWWW_LOAD_LABEL_BOTTOM}, 
                                             {MMIWWW_LOAD_LABEL_LEFT, MMIWWW_LOAD_LABEL_TOP, MMIWWW_LOAD_LABEL_RIGHT_H, MMIWWW_LOAD_LABEL_BOTTOM}};
            GUI_BOTH_RECT_T pro_lab_rect = {{MMIWWW_LOAD_PROG_LEFT, MMIWWW_LOAD_PROG_TOP, MMIWWW_LOAD_PROG_RIGHT, MMIWWW_LOAD_PROG_BOTTOM}, 
                                            {MMIWWW_LOAD_PROG_LEFT_H, MMIWWW_LOAD_PROG_TOP, MMIWWW_LOAD_PROG_RIGHT_H, MMIWWW_LOAD_PROG_BOTTOM}};
            GUI_BOTH_RECT_T sec_lab_rect = {{MMIWWW_PAGE_SECURE_LEFT, MMIWWW_PAGE_SECURE_TOP, MMIWWW_PAGE_SECURE_RIGHT, MMIWWW_PAGE_SECURE_BOTTOM}, 
                                            {MMIWWW_PAGE_SECURE_LEFT_H, MMIWWW_PAGE_SECURE_TOP, MMIWWW_PAGE_SECURE_RIGHT_H, MMIWWW_PAGE_SECURE_BOTTOM}};
            GUI_BOTH_RECT_T opt_btn_rect = {{MMIWWW_PAGE_OPTION_LEFT, MMIWWW_PAGE_OPTION_TOP, MMIWWW_PAGE_OPTION_RIGHT, MMIWWW_PAGE_OPTION_BOTTOM},
                                            {MMIWWW_PAGE_OPTION_LEFT_H, MMIWWW_PAGE_OPTION_TOP_H, MMIWWW_PAGE_OPTION_RIGHT_H, MMIWWW_PAGE_OPTION_BOTTOM_H}};
            GUI_BOTH_RECT_T reload_btn_rect = {{MMIWWW_PAGE_RELOAD_LEFT, MMIWWW_PAGE_RELOAD_TOP, MMIWWW_PAGE_RELOAD_RIGHT, MMIWWW_PAGE_RELOAD_BOTTOM},
                                            {MMIWWW_PAGE_RELOAD_LEFT_H, MMIWWW_PAGE_OPTION_TOP_H, MMIWWW_PAGE_RELOAD_RIGHT_H, MMIWWW_PAGE_OPTION_BOTTOM_H}};
            GUI_BOTH_RECT_T full_btn_rect = {{MMIWWW_PAGE_FULL_LEFT, MMIWWW_PAGE_FULL_TOP, MMIWWW_PAGE_FULL_RIGHT, MMIWWW_PAGE_FULL_BOTTOM},
                                            {MMIWWW_PAGE_FULL_LEFT_H, MMIWWW_PAGE_OPTION_TOP_H, MMIWWW_PAGE_FULL_RIGHT_H, MMIWWW_PAGE_OPTION_BOTTOM_H}};
            GUI_BOTH_RECT_T pre_btn_rect = {{MMIWWW_PAGE_PREV_LEFT, MMIWWW_PAGE_PREV_TOP, MMIWWW_PAGE_PREV_RIGHT, MMIWWW_PAGE_PREV_BOTTOM},
                                            {MMIWWW_PAGE_PREV_LEFT_H, MMIWWW_PAGE_OPTION_TOP_H, MMIWWW_PAGE_PREV_RIGHT_H, MMIWWW_PAGE_OPTION_BOTTOM_H}};

            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIWWW_PAGE_ANIMATE_LOADING_CTRL_ID), &anim_rect);
            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIWWW_PAGE_LOAD_LABEL_CTRL_ID), &load_lab_rect);
            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIWWW_PAGE_PROG_LABEL_CTRL_ID), &pro_lab_rect);
            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIWWW_PAGE_SECURE_LABEL_CTRL_ID), &sec_lab_rect);
            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIWWW_PAGE_OPTION_BUTTON_CTRL_ID), &opt_btn_rect);
            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIWWW_PAGE_RELOAD_BUTTON_CTRL_ID), &reload_btn_rect);
            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIWWW_PAGE_FULL_BUTTON_CTRL_ID), &full_btn_rect);
            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIWWW_PAGE_PREV_BUTTON_CTRL_ID), &pre_btn_rect);

            img_bg.bg_type = GUI_BG_IMG;
            GUILABEL_SetBg(MMK_GetCtrlHandleByWin(win_id, MMIWWW_PAGE_SECURE_LABEL_CTRL_ID), &img_bg);

            MMIWWW_SetFreq();
            MMIWWW_SetBrowserFocusFlag(TRUE);
            recode = mmiwww_HandleBrowserOpenWin(win_id);
        }
        break;

    case MSG_FULL_PAINT:
        recode = mmiwww_HandleBrowserFullPaint(win_id, param);
        break;

    case MSG_LOSE_FOCUS:
	    MMIWWW_SetBrowserFocusFlag(FALSE);
	    MMIWWW_StopZoomTimer(win_id);
        MMIWWW_PostMsgToBrowser(MSG_WWW_ASYN_LOSE_FOCUS, PNULL, 0);
        break;

    case MSG_GET_FOCUS:
#ifdef TOUCH_PANEL_SUPPORT
        if (UILAYER_IsMultiLayerEnable())
        {
            MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();
            UILAYER_APPEND_BLT_T append_layer = {0};

            append_layer.lcd_dev_info = www_instance->btn_layer_info;
            append_layer.layer_level = UILAYER_LEVEL_NORMAL;
            UILAYER_AppendBltLayer(&append_layer);
        }
#endif
        MMIWWW_SetBrowserFocusFlag(TRUE);
        break;

    case MSG_LCD_SWITCH:
#ifdef MOTION_SENSOR_TYPE
        mmiwww_HandleHorizontalDisplay(mmiwww_GetBrowserWinhandle(), (BOOLEAN)(!MMITHEME_IsMainScreenLandscape()));
#endif /* MOTION_SENSOR_TYPE */
        break;

    case MSG_WWW_UPDATE_LOAD_PERCENT:
        if ((MMIWWW_STATE_PAGE_LOADING == MMIWWW_GetWWWState(MMIWWW_Netfront_GetActWinIndex())) && (PNULL != param))
        {
            MMI_HANDLE_T ctrl_handle = PNULL;
            static uint8 pre_percent = 0xFF;
            uint8 cur_percent = (uint8)(*(uint32*)param);

            if (pre_percent != cur_percent)
            {
                pre_percent = cur_percent;
            
                if (MMK_IsFocusWin(win_id) && !MMIWWW_IsFullDisplay())
                {
                    mmiwww_DrawPageTitleBG(win_id);

                    /* Set value and refresh */
                    ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_PAGE_PROG_LABEL_CTRL_ID);
                    mmiwww_SetLoadPercent(ctrl_handle, cur_percent);
                    MMK_SendMsg(ctrl_handle, MSG_CTL_PAINT, PNULL);

                    ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_PAGE_LOAD_LABEL_CTRL_ID);
                    MMK_SendMsg(ctrl_handle, MSG_CTL_PAINT, PNULL);

                    ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_PAGE_ANIMATE_LOADING_CTRL_ID);
                    MMK_SendMsg(ctrl_handle, MSG_CTL_PAINT, PNULL);

                    mmiwww_DrawSecureIcon(win_id);
                    ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_PAGE_SECURE_LABEL_CTRL_ID);
                    MMK_SendMsg(ctrl_handle, MSG_CTL_PAINT, PNULL);
                }
                else
                {
                    /* Just set value */
                    ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_PAGE_PROG_LABEL_CTRL_ID);
                    mmiwww_SetLoadPercent(ctrl_handle, cur_percent);
                }
            }
        }
        break;

    case MSG_WWW_PAGE_LOAD_START:
        recode = mmiwww_HandleBrowserLoadmsg(win_id, msg_id, param);
        break;

    case MSG_WWW_PAGE_LOAD_FINISH:
        recode = mmiwww_HandleBrowserLoadmsg(win_id, msg_id, param);
        break;

    case MSG_WWW_PAGE_SECURE_INFO:
        MMIWWW_SetPageSecureFlag(MMIWWW_Netfront_GetActWinIndex(), (BOOLEAN)(*(uint32*)param));

        if (MMK_IsFocusWin(win_id) && !MMIWWW_IsFullDisplay())
        {
            MMI_HANDLE_T ctrl_handle = PNULL;

            mmiwww_DrawPageTitleBG(win_id);

            /* Set value and refresh */
            ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_PAGE_PROG_LABEL_CTRL_ID);
            MMK_SendMsg(ctrl_handle, MSG_CTL_PAINT, PNULL);

            ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_PAGE_LOAD_LABEL_CTRL_ID);
            MMK_SendMsg(ctrl_handle, MSG_CTL_PAINT, PNULL);

            ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_PAGE_ANIMATE_LOADING_CTRL_ID);
            MMK_SendMsg(ctrl_handle, MSG_CTL_PAINT, PNULL);

            mmiwww_DrawSecureIcon(win_id);
            ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_PAGE_SECURE_LABEL_CTRL_ID);
            MMK_SendMsg(ctrl_handle, MSG_CTL_PAINT, PNULL);
        }

        break;

    case MSG_WWW_PAGE_CERTIFICATE:
        MMIWWW_OpenWWWCertificateWin(MMIWWW_CERT_IMPORT_MODE);
        break;

    case MSG_WWW_NOTIFY_BG_SOUND:
        MMIWWW_Handle_BG_Sound(param);
        break;

    case MSG_APP_1:
#ifdef MOTION_SENSOR_TYPE
        MMIAPIDL_StartDownload(MMIDL_OPEN_DOWNLOAD_MANAGER, PNULL);
#else
        mmiwww_HandleHorizontalDisplay(mmiwww_GetBrowserWinhandle(), (BOOLEAN)(!MMITHEME_IsMainScreenLandscape()));
#endif /* MOTION_SENSOR_TYPE */
        break;

    case MSG_APP_2:
        MMIWWW_Netfront_PageUp();
        break;

    case MSG_APP_3:
        {
            MMI_STRING_T string = {0};

            if (MMIWWW_Netfront_GetPageAddress(&string) && PNULL != string.wstr_ptr)
            {
                MMIWWW_FREE_STRING(string);
                MMIWWW_Netfront_ReloadPage();
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_WWW_REFRESH_FAILED);
            }
        }
        break;

    case MSG_APP_4:
        if (MMIWWW_BrowserCanBackForward(TRUE))
        {
            if (!MMIWWW_HandleBackForward(TRUE))
            {
                recode = MMI_RESULT_FALSE;
            }
        }
        break;

    case MSG_APP_5:
        mmiwww_HandleFullDisplay(win_id, (BOOLEAN)(!MMIWWW_IsFullDisplay()));
        break;

    case MSG_APP_6:
        if (MMIWWW_BrowserCanBackForward(FALSE))
        {
            if (!MMIWWW_HandleBackForward(FALSE))
            {
                recode = MMI_RESULT_FALSE;
            }
        }
        break;

    case MSG_APP_7:
        if (mmiwww_CheckPageZoom(MMIWWW_ZOOMOUT))
        {
            if (MMIWWW_Netfront_PageZoom(MMIWWW_ZOOMOUT))
            {
                MMIWWW_StartZoomTimer(win_id, SCI_FALSE);
            }
            else
            {
                recode = MMI_RESULT_FALSE;
            }
        }
        break;
    case MSG_KEYUP_7:
    case MSG_KEYPRESSUP_7:
        MMIWWW_StopZoomTimer(win_id);
        break;

    case MSG_APP_8:
        MMIWWW_Netfront_PageDown();
        break;

    case MSG_APP_9:
        if (mmiwww_CheckPageZoom(MMIWWW_ZOOMIN))
        {
            if (MMIWWW_Netfront_PageZoom(MMIWWW_ZOOMIN))
            {
                MMIWWW_StartZoomTimer(win_id, SCI_TRUE);
            }
            else
            {
                recode = MMI_RESULT_FALSE;
            }
        }
        break;

    case MSG_KEYUP_9:
    case MSG_KEYPRESSUP_9:
        MMIWWW_StopZoomTimer(win_id);
        break;

    case MSG_APP_0:
        mmiwww_OpenWindowManagerWin();
        break;

    case MSG_APP_STAR:
        {
            MMI_HANDLE_T applet_handle = PNULL;
            MMI_HANDLE_T mainmenu_handle = PNULL;

            applet_handle = MMIWWW_HANDLE((WWW_APPLET_T *)MMK_GetAppletPtrByWin(win_id));
            mainmenu_handle = MMK_GetWinHandle(applet_handle, MMIWWW_MAINMENU_WIN_ID);
            if (MMK_IsOpenWin(mainmenu_handle))
            {
                MMIWWW_OpenWWWMainMenuWin(MMIWWW_NetfrontGetMainMenuFocusCtrl());
            }
        }
        break;

    case MSG_APP_HASH:
        {
            BOOLEAN is_disp_full_menu = FALSE;

            if (MMIWWW_IsFullDisplay())
            {
                is_disp_full_menu = TRUE;
            }
            else
            {
                is_disp_full_menu = FALSE;
            }
            MMIWWW_OpenShotcutKeyOptMenuWin(is_disp_full_menu);
        }
        break;

    case MSG_TIMER:
        {    
            MMIWWW_PAGEINFO_T    *pageinfo_ptr = PNULL;

            pageinfo_ptr = (MMIWWW_PAGEINFO_T *)MMK_GetWinAddDataPtr(win_id);

            if (PNULL == pageinfo_ptr)
            {
                break;
            }

            if (*(uint8*)param == pageinfo_ptr->timer_id)
            {
                MMIWWW_StopZoomTimer(win_id);
                if (MMIWWW_ZOOMIN == pageinfo_ptr->zoom_mode)
                {
                     MMK_PostMsg(win_id, MSG_APP_4, PNULL, 0);
                }
                else
                {
                     MMK_PostMsg(win_id, MSG_APP_6, PNULL, 0);
                }
            }
            else
            {
                //SCI_TRACE_LOW:"HandleWWW'Msg wrong timer id"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_4445_112_2_18_3_12_9_186,(uint8*)"");
            }
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:

        {
            MMI_USER_INPUT_MESSAGE_T input_msg = {0};
            
            input_msg.msg_type = msg_id;
            SCI_MEMCPY(&input_msg.tp_msg_param, param, sizeof(MMI_MULTI_KEY_TP_MSG_PARAM_T));
            MMIWWW_SetUserInputMSG(&input_msg); /* save last user input */

            recode = mmiwww_HandleBrowserTPDown(win_id, msg_id, param);
            if (MMI_RESULT_FALSE == recode)
            {
                if (!MMIWWW_HandleMsgEvent(msg_id, param))
                {
                    recode = MMI_RESULT_FALSE;
                }
                else
                {
                    recode = MMI_RESULT_TRUE;
                }
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_KEYDOWN_WEB:
        if (MMIWWW_Netfront_IsImageSelectMode())
        {
            mmiwww_OpenPageSaveAsWin();
            MMIWWW_Netfront_SetImageSelectMode(FALSE);
            recode = MMI_RESULT_TRUE;
            break;
        }
        if (MMIWWW_COPY_MODE_CHOSE_START == MMIWWW_GetCopyMode())
        {
            MMIWWW_SetCopyMode(MMIWWW_COPY_MODE_CHOSE_END);
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        else if (MMIWWW_COPY_MODE_CHOSE_END == MMIWWW_GetCopyMode())
        {
            MMIWWW_Netfront_LeaveCopyMode();
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
    //lint -fallthrough
    case MSG_KEYDOWN_UP:
    case MSG_KEYUP_UP:
    case MSG_KEYDOWN_DOWN:
    case MSG_KEYUP_DOWN:
    case MSG_KEYDOWN_LEFT:
    case MSG_KEYUP_LEFT:
    case MSG_KEYDOWN_RIGHT:
    case MSG_KEYUP_RIGHT:
    case MSG_KEYUP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_MOVE:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_KEYPRESSUP_UP:
    case MSG_KEYPRESSUP_DOWN:
    case MSG_KEYPRESSUP_LEFT:
    case MSG_KEYPRESSUP_RIGHT:
    case MSG_KEYREPEAT_UP:
    case MSG_KEYREPEAT_DOWN:
    case MSG_KEYREPEAT_LEFT:
    case MSG_KEYREPEAT_RIGHT:
        {
            MMI_USER_INPUT_MESSAGE_T input_msg = {0};
            
            input_msg.msg_type = msg_id;
            if (MSG_TP_PRESS_UP == msg_id || MSG_TP_PRESS_MOVE == msg_id)
            {
                SCI_MEMCPY(&input_msg.tp_msg_param, param, sizeof(MMI_MULTI_KEY_TP_MSG_PARAM_T));
            }
            MMIWWW_SetUserInputMSG(&input_msg); /* save last user input */

            if (!MMIWWW_HandleMsgEvent(msg_id, param))
            {
                recode = MMI_RESULT_FALSE;
            }
        }
        break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_MENU:
        recode = mmiwww_HandleBrowserOK(win_id);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        if (MMIWWW_Netfront_IsImageSelectMode())
        {
            MMIWWW_Netfront_SetImageSelectMode(FALSE);
            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
        }
        else if (MMIWWW_COPY_MODE_NONE != MMIWWW_GetCopyMode())
        {
            MMIWWW_Netfront_LeaveCopyMode();
        }
        else
        {
            if (MMIWWW_BrowserCanBackForward(TRUE))
            {
                recode = www_PagePrevCallBack();
            }
            else
            {
                recode = mmiwww_HandleBrowserCancel(win_id, MSG_CTL_CANCEL);
            }
        }
        break;

    case MSG_PROMPTWIN_OK:
    case MSG_PROMPTWIN_CANCEL:
        recode = mmiwww_HandleBrowserCancel(win_id, msg_id);
        break;

    case MSG_WWW_ASYN_LOSE_FOCUS:
        {
            MMI_USER_INPUT_MESSAGE_T last_input_msg = {0};

            MMIWWW_GetUserInputMSG(&last_input_msg); /* Get last input */

            //SCI_TRACE_LOW:"HandleWWWBrowserWinMsg() Before msg_type =0x%04x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_4570_112_2_18_3_12_10_187,(uint8*)"d", last_input_msg.msg_type);

            if (KEY_WEB != (last_input_msg.msg_type & 0x00FF)) /* Do Not send functional key */
            {
                /* Send missed event to Netfront according to user input */
                if (KEY_PRESSED == (last_input_msg.msg_type & 0xFF00))
                {
                    last_input_msg.msg_type = KEY_RELEASED | (last_input_msg.msg_type & 0x00FF);
                }
                else if (KEY_LONG_PRESSED == (last_input_msg.msg_type & 0xFF00))
                {
                    last_input_msg.msg_type = KEY_LONG_RELEASED | (last_input_msg.msg_type & 0x00FF);
                }
                else if (KEY_REPEATED == (last_input_msg.msg_type & 0xFF00))
                {
                    last_input_msg.msg_type = KEY_LONG_RELEASED | (last_input_msg.msg_type & 0x00FF);
                }
                else if (MSG_TP_PRESS_DOWN == last_input_msg.msg_type || MSG_TP_PRESS_MOVE == last_input_msg.msg_type)
                {
                    last_input_msg.msg_type = MSG_TP_PRESS_UP;
                }
                else
                {
                    last_input_msg.msg_type = 0;
                }
            }
            else
            {
                last_input_msg.msg_type = 0;
            }

            //SCI_TRACE_LOW:"HandleWWWBrowserWinMsg() After msg_type =0x%04x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_4601_112_2_18_3_12_10_188,(uint8*)"d", last_input_msg.msg_type);

            if (0 != last_input_msg.msg_type)
            {
                MMIWWW_HandleMsgEvent(last_input_msg.msg_type, &last_input_msg.tp_msg_param);
            }

            MMIWWW_SetBrowserFocusFlag(FALSE);
            MMIWWW_StopZoomTimer(win_id);
        }
        break;

    case MSG_CLOSE_WINDOW:
        MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
        MMIWWW_StopZoomTimer(win_id);
        mmiwww_LoadPageAnimation(win_id, FALSE);
        MMK_FreeWinAddData(win_id);
        if (SLIM_E_OK != MMIWWW_Netfront_StopBrowser())
        {
            MMIWWW_PostMsgToWWWApplet(MSG_WWW_STOP_APPLET, PNULL, 0);
        }
#ifndef MMIWWW_PDP_APPLET_CONNECT
        MMIWWW_NETDISCONNECT();
#endif
        MMIWWW_SetFullDisplayFlag(FALSE);
        MMIWWW_RestoreFreq();
#ifdef JAVA_SUPPORT_SUN    
        MMIAPIJAVA_JavaCallClose(TRUE);
#endif
        MMIAPIDM_HandWapClose();
        {
            MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
            MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();
            dual_sys = www_instance->customized_connection.dual_sys;
            MMIAPISTK_BrowserTermination(dual_sys);
        }
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}

/*****************************************************************************/
// Description : mmiwww_DrawPageTitleBG
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL void mmiwww_DrawPageTitleBG(MMI_WIN_ID_T  win_id)
{
    MMI_IMAGE_ID_T image_id = 0;
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    uint16 screen_width = 0;
    uint16 screen_height = 0;
    GUI_RECT_T img_rect = {0};

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &screen_width, &screen_height);

    img_rect.left = 0;
    img_rect.top = MMI_STATUSBAR_HEIGHT;
    img_rect.right = screen_width;
    img_rect.bottom = img_rect.top + MMIWWW_PAGE_TITLE_HEIGHT;

    if (MMITHEME_IsMainScreenLandscape())
    {
        image_id = IMAGE_WWW_PAGE_DEFAULT_TITLE_H;
    }
    else
    {
        image_id = IMAGE_WWW_PAGE_DEFAULT_TITLE;
    }

    GUIRES_DisplayImg(PNULL, &img_rect, PNULL, win_id, image_id, &lcd_dev_info);
}

/*****************************************************************************/
// Description : Draw Button BG
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
LOCAL void mmiwww_DrawToolButtonBG(MMI_WIN_ID_T  win_id)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    MMI_IMAGE_ID_T image_id = 0;
    uint16 screen_width = 0;
    uint16 screen_height = 0;
    GUI_RECT_T img_rect = {0};

    if (MMITHEME_IsMainScreenLandscape())
    {
        image_id = IMAGE_WWW_PAGE_BUTTON_BG_H;
    }
    else
    {
        image_id = IMAGE_WWW_PAGE_BUTTON_BG;
    }

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &screen_width, &screen_height);

    img_rect.left = 0;
    img_rect.right = screen_width;
    img_rect.bottom = screen_height;
#ifdef MAINLCD_SIZE_320X480
    img_rect.top = screen_height - MMIWWW_PAGE_BUTTON_BG_HEIGHT;
#else
    img_rect.top = screen_height - MMIWWW_PAGE_BUTTON_HEIGHT;
#endif

    GUIRES_DisplayImg(PNULL, &img_rect, PNULL, win_id, image_id, &lcd_dev_info);
}

/*****************************************************************************/
// Description : Draw secure icon
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
LOCAL void mmiwww_DrawSecureIcon(MMI_WIN_ID_T win_id)
{
    MMI_IMAGE_ID_T  icon_id = 0;
    MMI_HANDLE_T label_handle = PNULL;

    if (MMIWWW_GetPageSecureFlag(MMIWWW_Netfront_GetActWinIndex()))
    {
        icon_id = IMAGE_WWW_SECURE;
    }
    label_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_PAGE_SECURE_LABEL_CTRL_ID);
    if (PNULL != label_handle)
    {
        GUILABEL_SetIcon(label_handle, icon_id);
    }
}

/*****************************************************************************/
// Description : Handle open win msg for browser win
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E mmiwww_HandleBrowserOpenWin(MMI_WIN_ID_T  win_id)
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
#ifdef TOUCH_PANEL_SUPPORT
    MMI_HANDLE_T        button_handle = NULL;
#endif

#ifdef TOUCH_PANEL_SUPPORT
    //设置button callback
    button_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_PAGE_OPTION_BUTTON_CTRL_ID);
    GUIBUTTON_SetCallBackFunc(button_handle, www_PageOptionCallBack);
    button_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_PAGE_RELOAD_BUTTON_CTRL_ID);
    GUIBUTTON_SetCallBackFunc(button_handle, www_PageReloadCallBack);
    button_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_PAGE_PREV_BUTTON_CTRL_ID);
    GUIBUTTON_SetCallBackFunc(button_handle, www_PagePrevCallBack);
    //button_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_PAGE_NEXT_BUTTON_CTRL_ID);
    //GUIBUTTON_SetCallBackFunc(button_handle, www_PageNextCallBack);
    button_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_PAGE_FULL_BUTTON_CTRL_ID);
    GUIBUTTON_SetCallBackFunc(button_handle, www_PageFullCallBack);
    MMIWWW_CreateToolboxFulldisplayButton(win_id);
#endif
    if (!MMIWWW_IsFullDisplay())
    {
        //初始化加载动画
        mmiwww_LoadPageAnimation(win_id, FALSE);
    }
    mmiwww_SetLoadPercent(MMK_GetCtrlHandleByWin(win_id, MMIWWW_PAGE_PROG_LABEL_CTRL_ID), 0);
    //初始化退出标志
    MMIWWW_SetExitFlag(FALSE);

    MMIWWW_Netfront_PageZoom(MMIWWW_ZOOMINIT);

    return recode;
}

/*****************************************************************************/
// Description : Handle full paint msg for browser win
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E mmiwww_HandleBrowserFullPaint(MMI_WIN_ID_T win_id, DPARAM param)
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    MMIWWW_STATE_T    state = MMIWWW_STATE_PAGE_LOADING;
    int32 active_index = 0;
    MMI_HANDLE_T ctrl_handle = PNULL;
#ifdef TOUCH_PANEL_SUPPORT
    MMI_HANDLE_T        button_handle = NULL;
    GUI_BG_T     button_bg = {0};/*lint !e64*/
    MMI_STRING_T string = {0};
#endif
    MMIWWW_COPY_MODE_E copy_mode = MMIWWW_COPY_MODE_NONE;

    active_index = MMIWWW_Netfront_GetActWinIndex();
    copy_mode = MMIWWW_GetCopyMode();

    if (PNULL != param && (*(uint32*)param) != (uint32)active_index)
    {
        return recode;
    }

    state = MMIWWW_GetWWWState(active_index);

    if (!MMIWWW_IsFullDisplay())
    {
        mmiwww_DrawPageTitleBG(win_id);
        mmiwww_DrawSecureIcon(win_id);
        mmiwww_LoadPageAnimPic(win_id);

#ifdef TOUCH_PANEL_SUPPORT
        mmiwww_DrawToolButtonBG(win_id);
        if (MMIWWW_Netfront_GetPageAddress(&string) && PNULL != string.wstr_ptr)
        {
            MMIWWW_FREE_STRING(string);

            button_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_PAGE_RELOAD_BUTTON_CTRL_ID);
            button_bg.bg_type = GUI_BG_IMG;
            button_bg.img_id = IMAGE_WWW_PAGE_RELOAD_DEFAULT;
            GUIBUTTON_SetBg(button_handle, &button_bg);
            GUIBUTTON_SetGrayed(button_handle, FALSE, &button_bg, FALSE);
        }
        else
        {
            //设置Reload button image为Disbale
            button_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_PAGE_RELOAD_BUTTON_CTRL_ID);
            button_bg.bg_type = GUI_BG_IMG;
            button_bg.img_id = IMAGE_WWW_PAGE_RELOAD_DISABLED;
            GUIBUTTON_SetBg(button_handle, &button_bg);
            GUIBUTTON_SetGrayed(button_handle, TRUE, &button_bg, FALSE);
        }
#endif /* TOUCH_PANEL_SUPPORT */

        if (MMIWWW_STATE_PAGE_VIEW == state)
        {
            ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_PAGE_LOAD_LABEL_CTRL_ID);
            mmiwww_SetPageTitle(ctrl_handle, TXT_NULL);
            ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_PAGE_PROG_LABEL_CTRL_ID);
            mmiwww_SetPageSize(ctrl_handle);
#ifdef TOUCH_PANEL_SUPPORT
            //设置Option button image为Default
            button_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_PAGE_OPTION_BUTTON_CTRL_ID);
            button_bg.bg_type = GUI_BG_IMG;
            button_bg.img_id = IMAGE_WWW_PAGE_OPTION_DEFAULT;
            GUIBUTTON_SetBg(button_handle, &button_bg);
            GUIBUTTON_SetGrayed(button_handle, FALSE, &button_bg, FALSE);

            //设置Full button image为Default
            button_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_PAGE_FULL_BUTTON_CTRL_ID);
            button_bg.bg_type = GUI_BG_IMG;
            button_bg.img_id = IMAGE_WWW_PAGE_FULL_DEFAULT;
            GUIBUTTON_SetBg(button_handle, &button_bg);
            GUIBUTTON_SetGrayed(button_handle, FALSE, &button_bg, FALSE);

            //设置button image为PREV
            button_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_PAGE_PREV_BUTTON_CTRL_ID);
            button_bg.bg_type = GUI_BG_IMG;
            if (MMIWWW_BrowserCanBackForward(TRUE))
            {
                button_bg.img_id = IMAGE_WWW_PAGE_PREV_DEFAULT;
                GUIBUTTON_SetGrayed(button_handle, FALSE, &button_bg, FALSE);
            }
            else
            {
                button_bg.img_id = IMAGE_WWW_PAGE_PREV_DISABLED;
                GUIBUTTON_SetGrayed(button_handle, TRUE, &button_bg, FALSE);
            }
            GUIBUTTON_SetBg(button_handle, &button_bg);
#else
            if (MMIWWW_Netfront_IsImageSelectMode())
            {
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, STXT_SAVE,
                                        STXT_RETURN, FALSE);

#ifdef MMI_AUTOTEST_SUPPORT
                MMIWWW_TraceAutoTestNotifySoftkey(STXT_OPTION, STXT_SAVE, STXT_RETURN);
#endif /* MMI_AUTOTEST_SUPPORT */
            }
            else if (MMIWWW_COPY_MODE_CHOSE_START == copy_mode)
            {
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_COMMON_START,
                                        TXT_COMMON_COMMON_BACK, FALSE);
            }
            else if (MMIWWW_COPY_MODE_CHOSE_END == copy_mode)
            {
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_COMMON_COMMON_END,
                                        TXT_COMMON_COMMON_BACK, FALSE);
            }
            else
            {
                if (MMIWWW_BrowserCanBackForward(TRUE))
                {
                    GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL,
                                            TXT_COMMON_COMMON_BACK, FALSE);

#ifdef MMI_AUTOTEST_SUPPORT
                    MMIWWW_TraceAutoTestNotifySoftkey(STXT_OPTION, TXT_NULL, TXT_COMMON_COMMON_BACK);
#endif /* MMI_AUTOTEST_SUPPORT */
                }
                else
                {
                    GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL,
                                            TXT_COMMON_CLOSE, FALSE);

#ifdef MMI_AUTOTEST_SUPPORT
                    MMIWWW_TraceAutoTestNotifySoftkey(STXT_OPTION, TXT_NULL, TXT_COMMON_CLOSE);
#endif /* MMI_AUTOTEST_SUPPORT */
                }
            }
#endif
        }
        else if (MMIWWW_STATE_PAGE_LOADING == state)
        {
            ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_PAGE_LOAD_LABEL_CTRL_ID);
            mmiwww_SetPageTitle(ctrl_handle, TXT_WWW_PAGE_LOADING);
            mmiwww_LoadPageAnimation(win_id, TRUE);
#ifdef TOUCH_PANEL_SUPPORT
            //设置Option button image为Disbale
            button_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_PAGE_OPTION_BUTTON_CTRL_ID);
            button_bg.bg_type = GUI_BG_IMG;
            button_bg.img_id = IMAGE_WWW_PAGE_OPTION_DISABLED;
            GUIBUTTON_SetBg(button_handle, &button_bg);
            GUIBUTTON_SetGrayed(button_handle, TRUE, &button_bg, FALSE);

            //设置Full button image为Disbale
            button_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_PAGE_FULL_BUTTON_CTRL_ID);
            button_bg.bg_type = GUI_BG_IMG;
            button_bg.img_id = IMAGE_WWW_PAGE_FULL_DISABLED;
            GUIBUTTON_SetBg(button_handle, &button_bg);
            GUIBUTTON_SetGrayed(button_handle, TRUE, &button_bg, FALSE);

            //设置Stop button image为STOP
            button_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_PAGE_PREV_BUTTON_CTRL_ID);
            button_bg.bg_type = GUI_BG_IMG;
            button_bg.img_id = IMAGE_WWW_PAGE_STOP_DEFAULT;
            GUIBUTTON_SetBg(button_handle, &button_bg);
            GUIBUTTON_SetGrayed(button_handle, FALSE, &button_bg, FALSE);
#else
            GUIWIN_SetSoftkeyTextId(win_id, TXT_REFRESH, TXT_NULL, STXT_STOP, FALSE);
#ifdef MMI_AUTOTEST_SUPPORT
            MMIWWW_TraceAutoTestNotifySoftkey(TXT_REFRESH, TXT_NULL, STXT_STOP);
#endif /* MMI_AUTOTEST_SUPPORT */
#endif
        }
    }

#ifdef TOUCH_PANEL_SUPPORT
    MMIWWW_SetToolboxFulldisplayButtonProperty(win_id);
#endif /* TOUCH_PANEL_SUPPORT */

    /* Notify NF to refresh */
    if (!MMK_IsFocusWin(win_id))
    {
        MMIWWW_SetBrowserFocusFlag(TRUE);
    }
    MMIWWW_Netfront_SuspendBrowser();
    MMIWWW_Netfront_ResumeBrowser();
    if (!MMK_IsFocusWin(win_id))
    {
        MMIWWW_SetBrowserFocusFlag(FALSE); /* Restore the flag*/
    }

    return recode;

}

/*****************************************************************************/
// Description : Handle page load msg for browser win
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E mmiwww_HandleBrowserLoadmsg(MMI_WIN_ID_T      win_id,
                                               MMI_MESSAGE_ID_E  msg_id, DPARAM param)
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();
    slim_int tab = 0;

    tab = (*(uint32*)param);

    switch(msg_id)
    {
      case MSG_WWW_PAGE_LOAD_START:
        MMIWWW_SetWWWState(tab, MMIWWW_STATE_PAGE_LOADING);
        //SCI_TRACE_LOW:"mmiwww_HandleBrowserLoadmsg() param =%d, active = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_4977_112_2_18_3_12_10_189,(uint8*)"dd", tab, MMIWWW_Netfront_GetActWinIndex());

        MMIWWW_Netfront_SetImageSelectMode(FALSE);
        MMIWWW_Netfront_LeaveCopyMode();

        if (PNULL == www_instance)
        {
            break;
        }

        if (www_instance->is_mem_alerted)
        {
            www_instance->is_mem_alerted = FALSE;
//            MMIWWW_Netfront_SetImageDisplayMode((slim_int)MMIWWW_GetImageMode());
//            MMIWWW_Netfront_SetScriptMode((slim_int)MMIWWW_GetScriptMode());
        }

        if (tab < MMIWWW_BROWSER_MAX_TAB)
        {
            if (MMIWWW_GetCharsetChange(tab))
            {
                MMIWWW_SetCharsetChange(tab, FALSE);
            }
            else
            {
                MMIWWW_SetCharsetIndex(tab, 0);
            }
            if (www_instance->is_bg_sound[tab])
            {
                www_instance->is_bg_sound[tab] = slim_false;
                MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
            }
            MMIWWW_SetBrowserReloadFlag((uint8)tab, FALSE);
        }

        break;

      case MSG_WWW_PAGE_LOAD_FINISH:
        MMIWWW_SetWWWState(tab, MMIWWW_STATE_PAGE_VIEW);
        MMIAPIWWW_CheckTimerDisable();
        //SCI_TRACE_LOW:"mmiwww_HandleBrowserLoadmsg() param =%d, active = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_5017_112_2_18_3_12_10_190,(uint8*)"dd", tab, MMIWWW_Netfront_GetActWinIndex());
#if defined(MMI_UPDATE_TIME_SUPPORT) && defined(MMI_NET_TIME_SUPPORT)
        {
            slim_bool  is_get_header_succ = 0;
            NFBrowserMem out_content_header ={0};
            if (PNULL == www_instance)
            {
                break;
            }
            is_get_header_succ = NFBrowser_GetPageHeader(www_instance->browser_instance,&out_content_header);
            //SCI_TRACE_LOW:"MMIWWW_Netfront_StartBrowser out_content_header->fData=%s"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_5027_112_2_18_3_12_11_191,(uint8*)"s", out_content_header.fData);
            if(is_get_header_succ)
            {
                /*get time infor from receive data, if get time,set time to system*/
                MMIAPIAUT_GetTimeFromRecvData((slim_tiny*)out_content_header.fData, (slim_word)out_content_header.fDataSize);
            }
            
            NFBrowser_ReleaseSlimMem(&out_content_header);
        }
#endif
        break;

      default:
        break;
    }

    if (MMK_IsFocusWin(win_id) && tab == MMIWWW_Netfront_GetActWinIndex() && !MMIWWW_IsFullDisplay())
    {
        MMK_SendMsg(win_id, MSG_FULL_PAINT, param);
    }
    return recode;
}

/*****************************************************************************/
// Description : Handle App Cancel msg for browser win
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E mmiwww_HandleBrowserCancel(MMI_WIN_ID_T      win_id,
                                              MMI_MESSAGE_ID_E  msg_id)
{
    MMI_RESULT_E      recode = MMI_RESULT_TRUE;
    MMIWWW_STATE_T    state = MMIWWW_STATE_PAGE_LOADING;
    slim_int          active = 0;
    
    state = MMIWWW_GetWWWState(MMIWWW_Netfront_GetActWinIndex());
    //SCI_TRACE_LOW:"mmiwww_HandleBrowserCancel() state=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_5062_112_2_18_3_12_11_192,(uint8*)"d", state);
    switch(msg_id)
    {
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        switch(state)
        {
          case MMIWWW_STATE_PAGE_LOADING:
            if (!MMIWWW_Netfront_Abort())
            {
                recode = MMI_RESULT_FALSE;
            }
            break;
          case MMIWWW_STATE_PAGE_VIEW:
            MMIPUB_OpenQueryWinByTextId(TXT_WWW_CLOSE_WEBPAGE, IMAGE_PUBWIN_QUERY,PNULL,PNULL);
            break;
          default:
            break;
        }
        break;
    case MSG_CLOSE_WINDOW:
        if (MMIWWW_STATE_PAGE_VIEW == state )
        {
            MMIWWW_SetExitFlag(TRUE);
            MMIPUB_OpenQueryWinByTextId(TXT_EXIT_BROWSER, IMAGE_PUBWIN_QUERY,PNULL,PNULL);
        }
        break;
    case MSG_PROMPTWIN_OK:
        MMIWWW_Netfront_ResumeBrowser();
        MMIPUB_CloseQuerytWin(PNULL);
        if (MMIWWW_IsExit())
        {
            MMIWWW_SetExitFlag(FALSE);
            MMK_CloseWin(win_id);
            MMIAPIWWW_StopWWW();
        }
        else
        {
            active = MMIWWW_Netfront_GetActWinIndex();
            mmiwww_DeleteSubWindow(active);
        }
        break;

    case MSG_PROMPTWIN_CANCEL:
        if (MMIWWW_IsExit())
        {
            MMIWWW_SetExitFlag(FALSE);
        }
        MMIPUB_CloseQuerytWin(PNULL);
        break;
    default:
        break;
    }
    return recode;
}

/*****************************************************************************/
// Description : Handle App OK msg for browser win
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E mmiwww_HandleBrowserOK(MMI_WIN_ID_T  win_id)
{
    MMI_RESULT_E      recode = MMI_RESULT_TRUE;
    MMIWWW_STATE_T    state = MMIWWW_STATE_PAGE_LOADING;
    
    state = MMIWWW_GetWWWState(MMIWWW_Netfront_GetActWinIndex());
    //SCI_TRACE_LOW:"mmiwww_HandleBrowserOK() state=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_5128_112_2_18_3_12_11_193,(uint8*)"d", state);
    switch(state)
    {
      case MMIWWW_STATE_PAGE_LOADING:
        {
            MMI_STRING_T string = {0};

            if (MMIWWW_Netfront_GetPageAddress(&string) && PNULL != string.wstr_ptr)
            {
                MMIWWW_FREE_STRING(string);
                MMIWWW_Netfront_ReloadPage();
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_WWW_REFRESH_FAILED);
            }
        }
        break;

      case MMIWWW_STATE_PAGE_VIEW:
        MMIWWW_OpenWWWPageOptMenuWin();
        break;

      default:
        break;
    }
    return recode;
}

/*****************************************************************************/
// Description : Handle MSG_TP_PRESS_DOWN msg for browser win
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E mmiwww_HandleBrowserTPDown(MMI_WIN_ID_T      win_id,
                                              MMI_MESSAGE_ID_E  msg_id,
                                              DPARAM            param)
{
    MMI_RESULT_E    recode = MMI_RESULT_FALSE;
    GUI_POINT_T     point = {0};
    slim_int        num = 0;

    GUI_RECT_T      switch_rect = WWW_PAGE_SWITCH_RECT;

    //SCI_TRACE_LOW:"mmiwww_HandleBrowserTPDown()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_5171_112_2_18_3_12_11_194,(uint8*)"");
    MMIWWW_Netfront_SetImageSelectMode(FALSE);

    //loading状态时不可切换
    if (!MMIWWW_IsFullDisplay() && MMIWWW_STATE_PAGE_LOADING != MMIWWW_GetWWWState(MMIWWW_Netfront_GetActWinIndex()))
    {
        //是否多标签浏览
        num = MMIWWW_Netfront_GetWinNum();
        if (num > 1)
        {
            //act_index = MMIWWW_Netfront_GetActWinIndex();
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            if (GUI_PointIsInRect(point, switch_rect))
            {
                mmiwww_SwitchBrowserTab(win_id);
                MMIWWW_SendMsgToBrowser(MSG_FULL_PAINT, MMIWWW_Netfront_GetActWinIndex());
                recode = MMI_RESULT_TRUE;
            }
        }
    }
    return recode;
}

/*****************************************************************************/
// Description : 显示网页加载完成动画的初始/完成状态
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL void mmiwww_LoadPageAnimPic(MMI_WIN_ID_T  win_id)
{
    GUIANIM_CTRL_INFO_T     ctrl_info = {0};
    GUIANIM_DATA_INFO_T     data_info = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};
    MMI_HANDLE_T            anim_handle = NULL;

    anim_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_PAGE_ANIMATE_LOADING_CTRL_ID);
    ctrl_info.is_ctrl_id     = TRUE;
    ctrl_info.ctrl_id        = anim_handle;
    data_info.img_id         = IMAGE_WWW_PAGELOAD_PIC;
    display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
    GUIANIM_SetParam(&ctrl_info,&data_info,PNULL,&display_info);
    return;
}

/*****************************************************************************/
// Description : 显示或停止网页加载动画
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL void mmiwww_LoadPageAnimation(MMI_WIN_ID_T  win_id, BOOLEAN  is_play)
{
    GUIANIM_CTRL_INFO_T     ctrl_info = {0};
    GUIANIM_DATA_INFO_T     data_info = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};
    MMI_HANDLE_T            anim_handle = NULL;

    anim_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_PAGE_ANIMATE_LOADING_CTRL_ID);
    ctrl_info.is_ctrl_id        = TRUE;
    ctrl_info.ctrl_id           = anim_handle;
    data_info.img_id            = IMAGE_WWW_PAGELOADING;
    display_info.is_bg_buf      = TRUE;
    display_info.align_style    = GUIANIM_ALIGN_HVMIDDLE;
    display_info.is_update      = is_play;
    if(is_play)
    {
        GUIANIM_SetParam(&ctrl_info,&data_info,PNULL,&display_info);
    }
    else
    {
        display_info.is_disp_one_frame = TRUE;
        GUIANIM_SetParam(&ctrl_info,&data_info,PNULL,&display_info);
        GUIANIM_PauseAnim(anim_handle);
    }
    return;
}
/*****************************************************************************/
// Description : 设置网页标题栏
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL void mmiwww_SetPageTitle(MMI_HANDLE_T   label_handle,
                               MMI_TEXT_ID_T  text_id)
{
    MMI_STRING_T    title = {0};
    slim_bool       bret = slim_false;

    if (TXT_NULL != text_id)
    {
        GUILABEL_SetTextById(label_handle, text_id, FALSE);
#ifdef MMI_AUTOTEST_SUPPORT
        MMIRES_GetText(text_id, label_handle, &title);
        MMIBROWSER_TraceAutoTestNotifyTiTle(&title);
#endif /* MMI_AUTOTEST_SUPPORT */
    }
    else
    {
        bret = MMIWWW_Netfront_GetPageTitle(&title);
        if (bret)
        {
            GUILABEL_SetText(label_handle, &title, FALSE);

#ifdef MMI_AUTOTEST_SUPPORT
            MMIBROWSER_TraceAutoTestNotifyTiTle(&title);
#endif /* MMI_AUTOTEST_SUPPORT */

            MMIWWW_FREE_STRING(title);
        }
        else
        {
            GUILABEL_SetTextById(label_handle, TXT_NO_TITLE, FALSE);

#ifdef MMI_AUTOTEST_SUPPORT
            MMIRES_GetText(TXT_NO_TITLE, label_handle, &title);
            MMIBROWSER_TraceAutoTestNotifyTiTle(&title);
#endif /* MMI_AUTOTEST_SUPPORT */
        }
    }
    return;
}

/*****************************************************************************/
// Description : 设置网页加载进度
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL void mmiwww_SetLoadPercent(MMI_HANDLE_T  label_handle,
                                 uint8         percent)
{
    uint8 str_buf[10] = {0};
    wchar w_text[4] = {0};
    MMI_STRING_T string = {0};

    //设置进度栏
    if (percent > 100)
    {
        percent = 100;
    }

    sprintf((char*)str_buf, "%d%%", percent);
    string.wstr_len = (uint16)strlen((char *)str_buf);
    string.wstr_ptr = w_text;
    MMI_STRNTOWSTR(w_text, 4, (uint8 *)str_buf, string.wstr_len, string.wstr_len);

    GUILABEL_SetText(label_handle, &string, FALSE);

    //SCI_TRACE_LOW:"mmiwww_SetLoadPercent() percent = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_5313_112_2_18_3_12_11_195,(uint8*)"d", percent);
    return;
}

/*****************************************************************************/
// Description : 设置网页大小信息
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL void mmiwww_SetPageSize(MMI_HANDLE_T label_handle)
{
    uint8           str_buf[10] = {0};
    wchar           w_text[4] = {0};
    MMI_STRING_T    string = {0};
    slim_int        size = 0;

    //获取网页大小
    size = MMIWWW_Netfront_GetPageSize();
    if (size > 0)
    {
        size /= 1024;
        if (0 == size)
        {
            size = 1;
        }
        else if (size > 999)
        {
            size = 999;
        }
    }
    sprintf((char*)str_buf, "%dK", size);
    string.wstr_len = (uint16)strlen((char *)str_buf);
    string.wstr_ptr = w_text;
    MMI_STRNTOWSTR(w_text, 4, (uint8 *)str_buf, string.wstr_len, string.wstr_len);
    GUILABEL_SetText(label_handle, &string, FALSE);
    return;
}

/*****************************************************************************/
// Description : 切换网页模式
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL void mmiwww_SwitchImageSelectMode(void)
{
    if (MMIWWW_STATE_PAGE_VIEW == MMIWWW_GetWWWState(MMIWWW_Netfront_GetActWinIndex()) )
    {
        if (MMIWWW_Netfront_IsImageSelectMode())
        {
            MMIWWW_Netfront_SetImageSelectMode(FALSE);
        }
        else
        {
            MMIWWW_Netfront_SetImageSelectMode(TRUE);
        }
    }

    return;
}

/*****************************************************************************/
//  Description : handle messages of page option menu win
//  Global resource dependence : none
//  Author: Bill.Ji
//  Note:
/*****************************************************************************/

LOCAL MMI_RESULT_E HandleWWWPageOptMenuWinMsg(MMI_WIN_ID_T     win_id,
                                              MMI_MESSAGE_ID_E msg_id,
                                              DPARAM           param)
{
    MMI_RESULT_E           recode = MMI_RESULT_TRUE;
    MMI_MENU_ID_T          menu_id = 0;
    MMI_MENU_GROUP_ID_T    group_id = 0;
    MMI_STRING_T           title_string_info = {0};
    MMI_STRING_T           page_string_url = {0};

    //SCI_TRACE_LOW:"mmiwww_wintab.c:HandleWWWPageOptMenuWinMsg() msg_id=0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_5386_112_2_18_3_12_11_196,(uint8*)"d", msg_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            MMI_HANDLE_T ctrl_handle = PNULL;
            GUIMENU_POP_SEL_INFO_T  pop_item_info = {0};/*lint !e64*/
            int32 focus_item = 0;

            focus_item = (int32)((uint32)MMIWWW_GetCurrentPageZoomPercent() - (uint32)MMIWWW_ZOOM_50);
            ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_PAGE_OPTION_MENU_CTRL_ID);
            pop_item_info.is_static = TRUE;
            pop_item_info.ctrl_id = ctrl_handle;
            pop_item_info.menu_id = ID_WWW_PAGE_ZOOM_PERCENT_50 + focus_item;
            pop_item_info.group_id = MENU_WWW_PAGE_ZOOM;
            GUIMENU_SetPopItemStatus(TRUE, &pop_item_info);
            mmiwww_SetPageOptMenuGrayed(win_id);
            MMK_SetAtvCtrl(win_id, ctrl_handle);
        }
        break;

    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:  
        GUIMENU_GetId(MMK_GetCtrlHandleByWin(win_id, MMIWWW_PAGE_OPTION_MENU_CTRL_ID),&group_id,&menu_id);
        switch ( menu_id )
        {
        case ID_WWW_OPEN_URL:
        case ID_WWW_SEARCH:
        case ID_WWW_BOOKMARK:
        case ID_WWW_HISTORY:
            {
                MMIWWW_CTRL_E focus_ctrl = MMIWWW_CTRL_NULL;

                if (ID_WWW_OPEN_URL == menu_id)
                {
                    focus_ctrl = MMIWWW_CTRL_ADDRESS_LABEL;
                }
                else if (ID_WWW_SEARCH == menu_id)
                {
                    focus_ctrl = MMIWWW_CTRL_SEARCH_LABEL;
                }
                else if (ID_WWW_BOOKMARK == menu_id)
                {
                    focus_ctrl = MMIWWW_CTRL_BOOKMARK_LIST;
                }
                else
                {
                    focus_ctrl = MMIWWW_CTRL_HISTORY_LIST;
                }
                MMK_CloseWin(win_id);
                MMIWWW_SetIsCreateNewWin(FALSE);
                MMIWWW_OpenWWWMainMenuWin(focus_ctrl);
            }
            break;

        case ID_WWW_HOMPAGE:
            mmiwww_AccessHomepage();
            MMK_CloseWin(win_id);
            break;

        case ID_WWW_PAGE_REFRESH:
            {
                MMI_STRING_T string = {0};

                if (MMIWWW_Netfront_GetPageAddress(&string) && PNULL != string.wstr_ptr)
                {
                    MMIWWW_FREE_STRING(string);
                    MMIWWW_Netfront_ReloadPage();
                    MMK_CloseWin(win_id);
                }
                else
                {
                    MMK_CloseWin(win_id);
                    MMIPUB_OpenAlertWarningWin(TXT_WWW_REFRESH_FAILED);
                }
            }

            break;

        case ID_WWW_PAGE_FORWARD:
            MMIWWW_HandleBackForward(FALSE);
            MMK_CloseWin(win_id);
            break;

        case ID_WWW_PAGE_BACK:
            MMIWWW_HandleBackForward(TRUE);
            MMK_CloseWin(win_id);
            break;

        case ID_WWW_FULL_DISPLAY:
            MMK_CloseWin(win_id);
            mmiwww_HandleFullDisplay(mmiwww_GetBrowserWinhandle(), (BOOLEAN)(!MMIWWW_IsFullDisplay()));
            break;

        case ID_WWW_HORIZONTAL_DISPLAY:
            MMK_CloseWin(win_id);
            mmiwww_HandleHorizontalDisplay(mmiwww_GetBrowserWinhandle(), (BOOLEAN)(!MMITHEME_IsMainScreenLandscape()));
            break;

        case ID_WWW_PAGE_NAVIGATION:
            break;

        case ID_WWW_PAGE_SEARCH:
            mmiwww_OpenPageSearchWin();
            MMK_CloseWin(win_id);
            break;

        case ID_WWW_PAGE_COPY:
            MMIWWW_Netfront_EnterCopyMode();
            MMIPUB_OpenAlertSuccessWin( TXT_WWW_COPY_NOTIFICATION);
            MMK_CloseWin(win_id);
            break;

        case ID_WWW_PAGE_PROPERTY:
            mmiwww_OpenPagePropertyWin();
            MMK_CloseWin(win_id);
            break;

        case ID_WWW_DOWNLOAD_MANAGE:
            MMIAPIDL_StartDownload(MMIDL_OPEN_DOWNLOAD_MANAGER, PNULL);
            MMK_CloseWin(win_id);
            break;

        case ID_WWW_MESSAGE:
            MMIAPISMS_OpenMessageMainMenuWin();
            MMK_CloseWin(win_id);
            break;

        case ID_WWW_CALL:
            MMIAPICC_SwitchFromBrowser();
            MMK_CloseWin(win_id);
            break;

        case ID_WWW_SAVE_AS:
            if (0 < MMIWWW_Netfront_GetImageNumber())
            {
                if (!MMIWWW_Netfront_IsImageSelectMode())
                {
                    MMIWWW_Netfront_SetImageSelectMode(TRUE);
                    MMIPUB_OpenAlertWarningWin( TXT_WWW_SAVE_IMAGE_QUERY);
                }
            }
            MMK_CloseWin(win_id);
            break;

        case ID_WWW_NEW_WINDOW:
            MMIWWW_OpenWWWMainMenuWin(MMIWWW_CTRL_ADDRESS_LABEL);
            MMK_CloseWin(win_id);
            break;

        case ID_WWW_WINDOW_MANAGER:
            mmiwww_OpenWindowManagerWin();
            MMK_CloseWin(win_id);
            break;

        case ID_WWW_CLOSE_WINDOW:
            MMK_CloseWin(win_id);
            mmiwww_HandleBrowserCancel(win_id, MSG_CTL_CANCEL);    
            break;

        case ID_WWW_PAGE_ZOOM_PERCENT_50:
        case ID_WWW_PAGE_ZOOM_PERCENT_75:
        case ID_WWW_PAGE_ZOOM_PERCENT_100:
        case ID_WWW_PAGE_ZOOM_PERCENT_125:
        case ID_WWW_PAGE_ZOOM_PERCENT_150:
            {
                uint32 percent = 0;

                MMK_CloseWin(win_id);
                MMIWWW_SetCurrentPageZoomPercent((MMIWWW_ZOOM_E)(menu_id - ID_WWW_PAGE_ZOOM_PERCENT_50 + MMIWWW_ZOOM_50));
                percent = (uint32)(MMIWWW_PAGE_ZOOM_PERCENT_START + (menu_id - ID_WWW_PAGE_ZOOM_PERCENT_50) * MMIWWW_PAGE_ZOOM_PERCENT_STEP);
                MMIWWW_Netfront_PageZoomPercent(percent);
            }
            break;

        case ID_WWW_AS_HOMEPAGE:
            {
                slim_bool        bret = slim_true;
                NFBrowserMem     address = {0};/*lint !e64*/

                MMK_CloseWin(win_id);
                bret = NFBrowser_GetPageAddress(MMIWWW_GETBROWSER, &address);
                if (bret && address.fDataSize > 1)
                {
                    MMIWWW_SetHomepageURL((char *)address.fData);
                    MMIPUB_OpenAlertSuccessWin(TXT_WWW_BOOKMARK_SAVE_SUCCESS);

                }
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_WWW_SAVE_FAILED);
                }
                NFBrowser_ReleaseSlimMem(&address);
            }
            break;

        case ID_WWW_WEBPAGE:
        case ID_WWW_SAVE_BOOKMARK:
            MMIWWW_Netfront_GetPageAddress(&page_string_url);
            MMIWWW_Netfront_GetPageTitle(&title_string_info);
            if (PNULL != page_string_url.wstr_ptr)
            {
                if (ID_WWW_WEBPAGE == menu_id)
                {
#ifndef WIN32
                    BOOLEAN is_space_enough = FALSE;
#else 
                    BOOLEAN is_space_enough = TRUE;
#endif
                    uint32 free_high_word = 0;
                    uint32 free_low_word  = 0;
                    if (MMIAPIFMM_GetDeviceFreeSpace(
                            MMIAPIFMM_GetDevicePath(MMI_DEVICE_SYSTEM), 
                            MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SYSTEM),
                            &free_high_word, 
                            &free_low_word
                            ))
                    {
                        if (!(0 == free_high_word && MMIWWW_FILE_FREE_SIZE > free_low_word))
                        {
                            is_space_enough = TRUE;
                        }
                    }
                    if(is_space_enough) /*lint !e774*/
                    {
                        if (MMIWWW_MAX_PAGEMEMO_NUM <= MMIWWW_Netfront_GetPageMemoNumber())
                        {
                            MMIPUB_OpenAlertWarningWin(TXT_WWW_PAGEMEMO_FULL);
                        }
                        else
                        {
                            MMIWWW_Netfront_AddPageMemo(&title_string_info);
                            MMIPUB_OpenAlertSuccessWin(TXT_WWW_BOOKMARK_SAVE_SUCCESS);
                        }
                    }
                    else
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_NO_SPACE);
                    }
                }
                else
                {
                    MMIWWW_ChangeBookmarkWin(MMIWWW_BM_CHANGE_ADD_CUR_PAGE);
                }
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_WWW_SAVE_FAILED);
            }
            if (PNULL != title_string_info.wstr_ptr)
            {
                SCI_FREE(title_string_info.wstr_ptr);
            }
            if (PNULL != page_string_url.wstr_ptr)
            {
                SCI_FREE(page_string_url.wstr_ptr);
            }
            MMK_CloseWin(win_id);
            break;

        case ID_WWW_SUBMENU_SETTING:
            MMIWWW_OpenWWWSetMenuWin();
            MMK_CloseWin(win_id);
            break;

        case ID_WWW_PAGE_MEMO:
            {
                MMI_HANDLE_T applet_handle = MMIWWW_HANDLE((WWW_APPLET_T *)MMK_GetAppletPtrByWin(win_id));
                MMI_HANDLE_T win_handle = MMK_GetWinHandle(applet_handle, MMIWWW_PAGEMEMO_WIN_ID);
                if( !MMK_IsOpenWin(win_handle) )
                {
                    MMIWWW_OpenWWWPageMemoWin();
                }
                else
                {
                    if (!MMK_IsFocusWin(win_handle))
                    {
                        MMK_WinGrabFocus(win_handle);
                    }
                }
                MMK_CloseWin(win_id);
            }
            break;

        case ID_WWW_MINIMIZE:
            MMK_CloseWin(win_id);
            MMIAPIWWW_MinimizeWWW();
            break;

        case ID_WWW_EXIT:
            //MMK_CloseWin(win_id);
            //mmiwww_HandleBrowserCancel(win_id, MSG_CLOSE_WINDOW);
            MMIAPIWWW_StopWWW();
            break;

        default:
            //SCI_TRACE_LOW:"HandleWWWPageOptMenuWinMsg() not handled menu"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_5686_112_2_18_3_12_12_197,(uint8*)"");
            break;
        }
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        //待menu控件修改后，此处代码可删除
        GUIMENU_SetItemGrayed(MMIWWW_PAGE_OPTION_MENU_CTRL_ID, MMIWWW_IsFullDisplay() ? MENU_WWW_PAGE_FULL_PILOT : MENU_WWW_PAGE_PILOT, ID_WWW_PAGE_REFRESH, FALSE);
        GUIMENU_SetItemGrayed(MMIWWW_PAGE_OPTION_MENU_CTRL_ID, MMIWWW_IsFullDisplay() ? MENU_WWW_PAGE_FULL_PILOT : MENU_WWW_PAGE_PILOT, ID_WWW_PAGE_BACK, FALSE);
        GUIMENU_SetItemGrayed(MMIWWW_PAGE_OPTION_MENU_CTRL_ID, MMIWWW_IsFullDisplay() ? MENU_WWW_PAGE_FULL_PILOT : MENU_WWW_PAGE_PILOT, ID_WWW_PAGE_FORWARD, FALSE);
        GUIMENU_SetItemGrayed(MMIWWW_PAGE_OPTION_MENU_CTRL_ID, MENU_WWW_PAGE_SAVE, ID_WWW_SAVE_AS, FALSE);
        MMIAPICOM_EnableGrayed(win_id, MENU_WWW_PAGE_TOOL, ID_WWW_PAGE_COPY, FALSE);
        MMIAPICOM_EnableGrayed(win_id, MENU_WWW_PAGE_TOOL, ID_WWW_PAGE_SEARCH, FALSE);

        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}

/*****************************************************************************/
//  Description : 设置网页选项菜单需要灰化的ID
//  Global resource dependence : 
//  Author: Bill.Ji
//  Note:
/*****************************************************************************/
LOCAL void mmiwww_SetPageOptMenuGrayed(MMI_WIN_ID_T  win_id)
{
    slim_int    num = 0;
    MMI_STRING_T string = {0};
    MMIWWW_COPY_MODE_E copy_mode = MMIWWW_COPY_MODE_NONE;
    BOOLEAN is_image_select = FALSE;
    MMIWWW_POINTER_MODE_E pointer_mode = MMIWWW_POINTER_MODE_NORMAL;

    copy_mode = MMIWWW_GetCopyMode();
    is_image_select = MMIWWW_Netfront_IsImageSelectMode();
    pointer_mode = MMIWWW_GetPointerMode();
    num = MMIWWW_Netfront_GetWinNum();

    if (MMIWWW_Netfront_GetPageAddress(&string) && PNULL != string.wstr_ptr)
    {
        MMIWWW_FREE_STRING(string);
    }
    else
    {
        GUIMENU_SetItemGrayed(MMIWWW_PAGE_OPTION_MENU_CTRL_ID, MMIWWW_IsFullDisplay() ? MENU_WWW_PAGE_FULL_PILOT : MENU_WWW_PAGE_PILOT, ID_WWW_PAGE_REFRESH, TRUE);
    }

    if (!MMIWWW_BrowserCanBackForward(TRUE))
    {
        GUIMENU_SetItemGrayed(MMIWWW_PAGE_OPTION_MENU_CTRL_ID, MMIWWW_IsFullDisplay() ? MENU_WWW_PAGE_FULL_PILOT : MENU_WWW_PAGE_PILOT, ID_WWW_PAGE_BACK, TRUE);
    }
    if (!MMIWWW_BrowserCanBackForward(FALSE))
    {
        GUIMENU_SetItemGrayed(MMIWWW_PAGE_OPTION_MENU_CTRL_ID, MMIWWW_IsFullDisplay() ? MENU_WWW_PAGE_FULL_PILOT : MENU_WWW_PAGE_PILOT, ID_WWW_PAGE_FORWARD, TRUE);
    }

    if (MMIWWW_Netfront_GetImageNumber() <= 0 || is_image_select || MMIWWW_COPY_MODE_NONE != copy_mode)
    {
        MMIAPICOM_EnableGrayed(win_id, MENU_WWW_PAGE_SAVE, ID_WWW_SAVE_AS, TRUE);
    }

    if (is_image_select || MMIWWW_COPY_MODE_NONE != copy_mode)
    {
        MMIAPICOM_EnableGrayed(win_id, MENU_WWW_PAGE_TOOL, ID_WWW_PAGE_COPY, TRUE);
    }

    if (is_image_select || MMIWWW_COPY_MODE_NONE != copy_mode)
    {
        MMIAPICOM_EnableGrayed(win_id, MENU_WWW_PAGE_TOOL, ID_WWW_PAGE_SEARCH, TRUE);
    }

    if (MMIWWW_BROWSER_MAX_TAB <= num)
    {
        MMIAPICOM_EnableGrayed(win_id, MENU_WWW_PAGE_WINDOW, ID_WWW_NEW_WINDOW, TRUE);
    }
    return;
}

/*****************************************************************************/
//  Description : Check gray
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN mmiwww_CheckMenuGrayed(MMIWWW_SHOTCUT_E shotcut_id)
{
    BOOLEAN is_need_gray = FALSE;

    switch(shotcut_id) {
    case MMIWWW_SHOTCUT_HORIZONTAL_DISPLAY:
        break;

    case MMIWWW_SHOTCUT_PAGEUP:
        break;

    case MMIWWW_SHOTCUT_REFRESH:
        {
            MMI_STRING_T string;

            if (MMIWWW_Netfront_GetPageAddress(&string) && PNULL != string.wstr_ptr)
            {
                MMIWWW_FREE_STRING(string);
            }
            else
            {
                is_need_gray = TRUE;
            }
        }
        break;

    case MMIWWW_SHOTCUT_PAGE_BACK:
        if (!MMIWWW_BrowserCanBackForward(TRUE))
        {
            is_need_gray = TRUE;
        }
        break;

    case MMIWWW_SHOTCUT_FULL_DISPLAY:
        break;

    case MMIWWW_SHOTCUT_PAGE_FORWARD:
        if (!MMIWWW_BrowserCanBackForward(FALSE))
        {
            is_need_gray = TRUE;
        }
        break;

    case MMIWWW_SHOTCUT_ZOOMIN:
        if (!mmiwww_CheckPageZoom(MMIWWW_ZOOMIN))
        {
            is_need_gray = TRUE;
        }
        break;

    case MMIWWW_SHOTCUT_PAGEDOWN:
    break;

    case MMIWWW_SHOTCUT_ZOOMOUT:
        if (!mmiwww_CheckPageZoom(MMIWWW_ZOOMOUT))
        {
            is_need_gray = TRUE;
        }
        break;

/*
    case MMIWWW_SHOTCUT_SWITCH_PAGE:
        {
            if (MMIWWW_Netfront_GetWinNum() <= 1)
            {
                is_need_gray = TRUE;
            }
        }
        break;
*/
    case MMIWWW_SHOTCUT_SWITCH_WINDOW:
        if (!MMK_IsOpenWin(MMK_GetWinHandle(MMIWWW_HANDLE(MMIWWW_APPLETINSTANCE()), MMIWWW_MAINMENU_WIN_ID)))
        {
            is_need_gray = TRUE;
        }
        break;

    default:
        break;
    }

    //SCI_TRACE_LOW:"mmiwww_CheckMenuGrayed shotcut_id = %d, is_need_gray = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_5855_112_2_18_3_12_12_198,(uint8*)"dd", shotcut_id, is_need_gray);
    return is_need_gray;
}

/*****************************************************************************/
// Description : Check zoom in/out can be set
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN mmiwww_CheckPageZoom(MMIWWW_ZOOM_MODE_E mode)
{
    BOOLEAN bret = FALSE;
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();
    slim_int percent = MMIWWW_ZOOM_ORG;
    slim_int act_index = 0;

    act_index = MMIWWW_Netfront_GetActWinIndex();

    //SCI_TRACE_LOW:"mmiwww_CheckPageZoom mode=%d, percent=%d, act_index=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_5872_112_2_18_3_12_12_199,(uint8*)"ddd", mode, percent, act_index);

    if (act_index < MMIWWW_BROWSER_MAX_TAB && PNULL != www_instance)
    {
        switch(mode)
        {
        case MMIWWW_ZOOMIN:
            if (www_instance->peer_data.fZoom_percent[act_index] < MMIWWW_ZOOM_MAX)
            {
                bret = TRUE;
            }
            break;

        case MMIWWW_ZOOMOUT:
            if (www_instance->peer_data.fZoom_percent[act_index] > MMIWWW_ZOOM_MIN)
            {
                bret = TRUE;
            }
            break;

        case MMIWWW_ZOOMORG:
            bret = TRUE;
            break;

        default:
            break;
        }
    }

    return bret;
}

/*****************************************************************************/
//  Description : Open new browser tab & switch to www main menu
//  Global resource dependence : 
//  Author: Bill.Ji
//  Note:
/*****************************************************************************/
LOCAL void mmiwww_OpenNewBrowserWin(MMI_WIN_ID_T  win_id)
{
    slim_int    ret = SLIM_E_OK;
    slim_int    out_index = 0xFF;

    ret = MMIWWW_Netfront_NewWin(&out_index);

    if (SLIM_E_OK == ret)
    {
        MMIWWW_NetfrontSetMainMenuFocusCtrl(MMIWWW_CTRL_ADDRESS_LABEL);
        MMIWWW_Netfront_SetPointerMode(MMIWWW_GetPointerMode());
        MMK_WinGrabFocus(MMK_GetWinHandle(MMIWWW_HANDLE((WWW_APPLET_T *)MMK_GetAppletPtrByWin(win_id)),
                         MMIWWW_MAINMENU_WIN_ID));
    }
    else
    {
        MMIPUB_OpenAlertWarningWin( TXT_UNKNOWN_ERROR);
    }
    return;
}

/*****************************************************************************/
//  Description : Create a new browser tab
//  Global resource dependence : 
//  Author: Bill.Ji
//  Note:
/*****************************************************************************/
PUBLIC slim_int MMIWWW_CreateBrowserTab(void)
{
    slim_int    num = 0;
    slim_int    ret = SLIM_E_GENERIC;
    slim_int    out_index = 0xFF;
    
    num = MMIWWW_Netfront_GetWinNum();
    if (num < MMIWWW_BROWSER_MAX_TAB)
    {
        ret = MMIWWW_Netfront_NewWin(&out_index);
    }
    MMIWWW_Netfront_SetPointerMode(MMIWWW_GetPointerMode());
    return ret;
}

/*****************************************************************************/
//  Description : Switch browser tab
//  Global resource dependence : 
//  Author: Bill.Ji
//  Note:
/*****************************************************************************/
LOCAL void mmiwww_SwitchBrowserTab(MMI_WIN_ID_T  win_id)
{
    slim_int active = MMIWWW_BROWSER_TAB_1;
    
    active = MMIWWW_Netfront_GetActWinIndex() + 1;
    MMIWWW_Netfront_SetImageSelectMode(FALSE);
    MMIWWW_Netfront_LeaveCopyMode();

    if (active >= MMIWWW_BROWSER_MAX_TAB)
    {
        active = MMIWWW_BROWSER_TAB_1;
    }

    if (MMIWWW_Netfront_ActWinByIndex(active))
    {
        MMIWWW_Netfront_SetPointerMode(MMIWWW_GetPointerMode());
        if (MMIWWW_GetBrowserReloadFlag(active))
        {
            MMIWWW_SetBrowserReloadFlag(active, FALSE);
            MMIWWW_Netfront_ReloadPage();
        }
    }
    else
    {
        MMIPUB_OpenAlertWarningWin( TXT_UNKNOWN_ERROR);
    }

    return;
}

/*****************************************************************************/
//  Description : Create page search win
//  Global resource dependence : 
//  Author: Bill.Ji
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E mmiwww_OpenPageSearchWin(void)
{
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};

    win_table_create.applet_handle = MMIWWW_HANDLE(MMIWWW_APPLETINSTANCE());
    win_table_create.parent_win_handle = 0;
    win_table_create.win_table_ptr = MMIWWW_PAGE_SEARCH_WIN_TAB;
    MMK_CreateWinTable( &win_table_create );
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : handle messages of page search win
//  Global resource dependence : none
//  Author: Bill.Ji
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWPageSearchEditWin(MMI_WIN_ID_T     win_id,
                                              MMI_MESSAGE_ID_E msg_id,
                                              DPARAM           param)
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_STRING_T    edit_string = {0};
    MMI_STRING_T    *init_string_ptr = {0};
    MMI_HANDLE_T    ctrl_handle = NULL;
    slim_int        flag = (SLIM_BROWSER_TEXTSEARCH_ROUND | SLIM_BROWSER_TEXTSEARCH_IGNORECASE);
    slim_bool       bret = slim_true;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        init_string_ptr = (MMI_STRING_T *)MMIWWW_GetSearchString();

        ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_PAGE_SEARCH_EDIT_CTRL_ID);

        if (PNULL != init_string_ptr)
        {
            GUIEDIT_SetString(ctrl_handle, init_string_ptr->wstr_ptr, init_string_ptr->wstr_len);
        }

        GUIEDIT_SetSoftkey(ctrl_handle, 0, 1, TXT_NULL, TXT_COMMON_OK, PNULL);

        MMK_SetActiveCtrl(ctrl_handle, FALSE);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_PAGE_SEARCH_EDIT_CTRL_ID);
        GUIEDIT_GetString(ctrl_handle, &edit_string);

        if (0 != edit_string.wstr_len)
        {
            MMIWWW_SetSearchString(&edit_string);
            
            MMK_CloseWin(win_id);
            bret = MMIWWW_Netfront_SearchText(&edit_string, flag);
            if (!bret)
            {
                MMIPUB_OpenAlertWarningWin( TXT_WWW_NO_MATCH_TEXT);
            }
        }
        else
        {
            MMIPUB_OpenAlertWarningWin( TXT_WWW_INPUT_EMPTY);
        }
        break;

    case MSG_CTL_CANCEL:        
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return (recode);
}

/*****************************************************************************/
//  Description : Change display win
//  Global resource dependence : 
//  Author: JIaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E mmiwww_HandleFullDisplay(MMI_HANDLE_T win_handle, BOOLEAN is_full)
{
    MMI_HANDLE_T        ctrl_handle = NULL;
    GUICTRL_STATE_T dispay_state = 0;

    //loading状态不可切换
    if (MMIWWW_STATE_PAGE_LOADING == MMIWWW_GetWWWState(MMIWWW_Netfront_GetActWinIndex()))
    {
        return MMI_RESULT_TRUE;
    }

    dispay_state = GUICTRL_STATE_INVISIBLE | GUICTRL_STATE_DISABLE_TP;
  
    ctrl_handle = MMK_GetCtrlHandleByWin(win_handle, MMIWWW_PAGE_ANIMATE_LOADING_CTRL_ID);
    IGUICTRL_SetState(MMK_GetCtrlPtr(ctrl_handle), dispay_state, is_full);
    ctrl_handle = MMK_GetCtrlHandleByWin(win_handle, MMIWWW_PAGE_LOAD_LABEL_CTRL_ID);
    IGUICTRL_SetState(MMK_GetCtrlPtr(ctrl_handle), dispay_state, is_full);
    ctrl_handle = MMK_GetCtrlHandleByWin(win_handle, MMIWWW_PAGE_PROG_LABEL_CTRL_ID);
    IGUICTRL_SetState(MMK_GetCtrlPtr(ctrl_handle), dispay_state, is_full);
    ctrl_handle = MMK_GetCtrlHandleByWin(win_handle, MMIWWW_PAGE_SECURE_LABEL_CTRL_ID);
    IGUICTRL_SetState(MMK_GetCtrlPtr(ctrl_handle), dispay_state, is_full);
    if (is_full)
    {
        ctrl_handle = MMK_GetCtrlHandleByWin(win_handle, MMITHEME_GetStatusBarCtrlId());
        MMK_DestroyControl(ctrl_handle);
    }
    else
    {
        GUIWIN_CreateStbDyn(win_handle);
    }
#ifdef TOUCH_PANEL_SUPPORT
    ctrl_handle = MMK_GetCtrlHandleByWin(win_handle, MMIWWW_PAGE_OPTION_BUTTON_CTRL_ID);
    IGUICTRL_SetState(MMK_GetCtrlPtr(ctrl_handle), dispay_state, is_full);
    ctrl_handle = MMK_GetCtrlHandleByWin(win_handle, MMIWWW_PAGE_RELOAD_BUTTON_CTRL_ID);
    IGUICTRL_SetState(MMK_GetCtrlPtr(ctrl_handle), dispay_state, is_full);
    ctrl_handle = MMK_GetCtrlHandleByWin(win_handle, MMIWWW_PAGE_PREV_BUTTON_CTRL_ID);
    IGUICTRL_SetState(MMK_GetCtrlPtr(ctrl_handle), dispay_state, is_full);
    ctrl_handle = MMK_GetCtrlHandleByWin(win_handle, MMIWWW_PAGE_FULL_BUTTON_CTRL_ID);
    IGUICTRL_SetState(MMK_GetCtrlPtr(ctrl_handle), dispay_state, is_full);
#else
    ctrl_handle = MMK_GetCtrlHandleByWin(win_handle, MMK_GetWinSoftkeyCtrlId(win_handle));
    IGUICTRL_SetState(MMK_GetCtrlPtr(ctrl_handle),dispay_state, is_full);
#endif

    MMIWWW_SetFullDisplayFlag(is_full);
    MMIWWW_Netfront_SuspendBrowser();
    MMIWWW_Netfront_Resize();
    MMIWWW_Netfront_ResumeBrowser();
    MMIWWW_SendMsgToBrowser(MSG_FULL_PAINT, MMIWWW_Netfront_GetActWinIndex());
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : Change display win
//  Global resource dependence : 
//  Author: JIaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E mmiwww_HandleHorizontalDisplay(MMI_HANDLE_T win_handle, BOOLEAN is_horizontal)
{
#ifndef MOTION_SENSOR_TYPE
    MMK_SetWinSupportAngle(win_handle, WIN_SUPPORT_ANGLE_ALL);
    if (is_horizontal)
    {
        MMK_SetWinAngle( win_handle, LCD_ANGLE_90, FALSE );
    }
    else
    {
        MMK_SetWinAngle( win_handle, LCD_ANGLE_0, FALSE );
    }
#endif /* MOTION_SENSOR_TYPE */
    MMIWWW_Netfront_SuspendBrowser();
    MMIWWW_Netfront_Resize();
    MMIWWW_Netfront_ResumeBrowser();
#ifndef MOTION_SENSOR_TYPE
    MMK_SetWinSupportAngle(win_handle, WIN_SUPPORT_ANGLE_CUR);
#endif /* MOTION_SENSOR_TYPE */
    MMIWWW_SendMsgToBrowser(MSG_FULL_PAINT, MMIWWW_Netfront_GetActWinIndex());
    return MMI_RESULT_TRUE;
}

#if 0
/*****************************************************************************/
//  Description : Create Full display win
//  Global resource dependence : 
//  Author: Bill.Ji
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E mmiwww_OpenFullDisplayWin(void)
{
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};

    win_table_create.applet_handle = MMIWWW_HANDLE(MMIWWW_APPLETINSTANCE());
    win_table_create.parent_win_handle = 0;
    win_table_create.win_table_ptr = MMIWWW_FULL_DISPLAY_WIN_TAB;
    MMK_CreateWinTable( &win_table_create );
    MMIWWW_SetWWWState(MMIWWW_Netfront_GetActWinIndex(), MMIWWW_STATE_FULL_PAGE_VIEW);
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : handle messages of page full display win
//  Global resource dependence : none
//  Author: Bill.Ji
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWFullDisplayWin(MMI_WIN_ID_T     win_id,
                                           MMI_MESSAGE_ID_E msg_id,
                                           DPARAM           param)
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    slim_int            ret = 0;
    GUI_RECT_T          rect = MMITHEME_GetFullScreenRect();
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        LCD_FillRect(&lcd_dev_info, rect, MMI_WHITE_COLOR);
        MMIWWW_Netfront_ResumeBrowser();
        ret = MMIWWW_Netfront_Resize();
        break;
    case MSG_LOSE_FOCUS:
        MMIWWW_Netfront_SuspendBrowser();
        break;
    case MSG_GET_FOCUS:
        MMIWWW_Netfront_ResumeBrowser();
        break;
    case MSG_KEYDOWN_UP:
    case MSG_KEYUP_UP:
    case MSG_KEYDOWN_DOWN:
    case MSG_KEYUP_DOWN:
    case MSG_KEYDOWN_LEFT:
    case MSG_KEYUP_LEFT:
    case MSG_KEYDOWN_RIGHT:
    case MSG_KEYUP_RIGHT:
    case MSG_KEYDOWN_WEB:
    case MSG_KEYUP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_MOVE:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        if (!MMIWWW_HandleMsgEvent(msg_id, param))
        {
            recode = MMI_RESULT_FALSE;
        }
        break;
    case MSG_APP_4:
        if (!MMIWWW_HandleBackForward(TRUE))
        {
            recode = MMI_RESULT_FALSE;
        }
        break;
    case MSG_APP_6:
        if (!MMIWWW_HandleBackForward(FALSE))
        {
            recode = MMI_RESULT_FALSE;
        }
        break;
    case MSG_APP_OK:
        recode = mmiwww_HandleBrowserOK(win_id);
        break;
    case MSG_APP_2:
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        MMIWWW_SetWWWState(MMIWWW_Netfront_GetActWinIndex(), MMIWWW_STATE_PAGE_VIEW);
        ret = MMIWWW_Netfront_Resize();
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return (recode);
}
#endif

/*****************************************************************************/
//  Description : Create page property win
//  Global resource dependence : 
//  Author: Bill.Ji
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E mmiwww_OpenPagePropertyWin(void)
{
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};

    win_table_create.applet_handle = MMIWWW_HANDLE(MMIWWW_APPLETINSTANCE());
    win_table_create.parent_win_handle = 0;
    win_table_create.win_table_ptr = MMIWWW_PAGE_PROPERTY_WIN_TAB;
    MMK_CreateWinTable( &win_table_create );
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : handle messages of page property win
//  Global resource dependence : 
//  Author: Bill.Ji
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWPagePropertyWin(MMI_WIN_ID_T     win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM           param)
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        mmiwww_SetPagePropertyInfo(win_id);
        break;
    case MSG_FULL_PAINT:
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CLOSE_WINDOW:
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}

/*****************************************************************************/
//  Description : Set page property info to text ctrl
//  Global resource dependence : 
//  Author: Bill.Ji
//  Note:
/*****************************************************************************/
LOCAL void mmiwww_SetPagePropertyInfo(MMI_WIN_ID_T  win_id)
{
    MMI_STRING_T    string_id = {0};
    MMI_STRING_T    string = {0};
    MMI_HANDLE_T    ctrl_handle = NULL;
    slim_int        size = 0;
    uint8           str_buf[20] = {0};
    wchar           wstr_buf[20] = {0};
    slim_bool       bret = slim_true;
    //初始化
    ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_PAGE_PROPERTY_RICHTEXT_CTRL_ID);
    //标题
    MMI_GetLabelTextByLang(TXT_COMM_TITLE, &string_id);
    MMIWWW_RichText_AddItem(ctrl_handle, &string_id, GUIRICHTEXT_FRAME_NONE, GUIRICHTEXT_TAG_NONE);
    //实际网页的Title
    bret = MMIWWW_Netfront_GetPageTitle(&string);
    if (bret)
    {
        MMIWWW_RichText_AddItem(ctrl_handle, &string, GUIRICHTEXT_FRAME_NONE, GUIRICHTEXT_TAG_NONE);
        MMIWWW_FREE_STRING(string);
    }
    //URL
    MMI_GetLabelTextByLang(TXT_COMMON_URL, &string_id);
    MMIWWW_RichText_AddItem(ctrl_handle, &string_id, GUIRICHTEXT_FRAME_NONE, GUIRICHTEXT_TAG_NONE);
    //实际网页的URL
    bret = MMIWWW_Netfront_GetPageAddress(&string);
    if (bret)
    {
        MMIWWW_RichText_AddItem(ctrl_handle, &string, GUIRICHTEXT_FRAME_NONE, GUIRICHTEXT_TAG_NONE);
        MMIWWW_FREE_STRING(string);
    }
    //MIME格式
    MMI_GetLabelTextByLang(TXT_WWW_PAGE_MIME_TYPE, &string_id);
    MMIWWW_RichText_AddItem(ctrl_handle, &string_id, GUIRICHTEXT_FRAME_NONE, GUIRICHTEXT_TAG_NONE);
    //实际网页的MIME格式
    bret = MMIWWW_Netfront_GetPageMIMEType(&string);
    if (bret)
    {
        MMIWWW_RichText_AddItem(ctrl_handle, &string, GUIRICHTEXT_FRAME_NONE, GUIRICHTEXT_TAG_NONE);
        MMIWWW_FREE_STRING(string);
    }
    //大小
    MMI_GetLabelTextByLang(TXT_DETAIL_SIZE, &string_id);
    MMIWWW_RichText_AddItem(ctrl_handle, &string_id, GUIRICHTEXT_FRAME_NONE, GUIRICHTEXT_TAG_NONE);
    //实际网页大小
    size = MMIWWW_Netfront_GetPageSize();
    if (size < 1024)
    {
        if (size < 0)
        {
            size = 0;
        }
        sprintf((char*)str_buf, "%dByte(s)", size);
    }
    else
    {
        size += 512; /* 四舍五入 */
        size /= 1024;
        sprintf((char*)str_buf, "%dKB", size);
    }

    MMIAPICOM_StrToWstr((uint8 *)str_buf, wstr_buf);
    string.wstr_len = (uint16)MMIAPICOM_Wstrlen(wstr_buf);
    string.wstr_ptr = wstr_buf;
    MMIWWW_RichText_AddItem(ctrl_handle, &string, GUIRICHTEXT_FRAME_NONE, GUIRICHTEXT_TAG_NONE);
    //设置active控件
    MMK_SetActiveCtrl(ctrl_handle, FALSE);
    return;
}

/*****************************************************************************/
//  Description : Create page save as win
//  Global resource dependence : 
//  Author: Bill.Ji
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E mmiwww_OpenPageSaveAsWin(void)
{
    MMIDL_DOWNLOAD_INFO_T        download_info = {0};
    uint8                       *url = PNULL;

    if (0 < MMIWWW_Netfront_GetImageNumber())
    {
        if (MMIWWW_Netfront_IsImageSelectMode())
        {
            url = MMIWWW_Netfront_GetImageAddress();
            if (PNULL != url)
            {
                download_info.mime_type = MMIWWW_GetSelectedImageMIMEIndex();
                download_info.url = url;
                download_info.buf_len = MMIWWW_Netfront_GetImageData(&download_info.buf);
                if (PNULL != download_info.buf)
                {
                    MMIAPIDL_StartDownload(MMIDL_CREATE_SAVE_AS, &download_info);
                    SCI_FREE(download_info.buf);
                }
                SCI_FREE(url);
            }
            else
            {
                MMIPUB_OpenAlertWarningWin( TXT_WWW_GET_IMAGE_FAIL);
            }
        }
        else
        {
            MMIPUB_OpenAlertWarningWin( TXT_WWW_GET_DATA_FAIL);
        }
    }
    else
    {
        MMIPUB_OpenAlertWarningWin( TXT_WWW_NO_IMAGE);
    }
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : Handle MSG_APP_OK msg for save as win
//  Global resource dependence : 
//  Author: Bill.Ji
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN mmiwww_HandleSaveAsOKWin(MMI_WIN_ID_T  win_id)
{
    BOOLEAN             bret = FALSE;
    wchar               fullname[MMIFILE_FULL_PATH_MAX_LEN] = {0};  
    uint16              fullname_len = MMIFILE_FULL_PATH_MAX_LEN;
    wchar               splash[] = {MMIFILE_SLASH, '\0'};
    MMI_HANDLE_T        ctrl_handle = NULL;
    MMIFILE_HANDLE      handle = 0;
    uint32              access_mode = 0;
    MMIFILE_ERROR_E     error = SFS_NO_ERROR;
    GUIRICHTEXT_ITEM_T  item = {0};/*lint !e64*/
    uint32              written = 0;
    MMI_TEXT_ID_T       text_id = TXT_WWW_BOOKMARK_SAVE_SUCCESS;
    MMI_IMAGE_ID_T      image_id = IMAGE_PUBWIN_SUCCESS;
    uint8              *image_ptr = PNULL;
    uint32              image_len = 0;
    //初始化
    ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_PAGE_SAVE_AS_RICHTEXT_CTRL_ID);
    //获取路径
    GUIRICHTEXT_GetItem(ctrl_handle, MMIWWW_SAVEAS_FILEPATH_INDEX, &item);
    if (PNULL == item.text_data.buf.str_ptr || 0 == item.text_data.buf.len)
    {
        MMIPUB_OpenAlertWarningWin( TXT_WWW_EMPTY_PATH_NAME);
        return bret;
    }
    MMIAPICOM_Wstrcat(fullname, item.text_data.buf.str_ptr);
    //获取文件名
    GUIRICHTEXT_GetItem(ctrl_handle, MMIWWW_SAVEAS_FILENAME_INDEX, &item);
    if (PNULL == item.text_data.buf.str_ptr || 0 == item.text_data.buf.len)
    {
        MMIPUB_OpenAlertWarningWin( TXT_WWW_EMPTY_FILE_NAME);
        return bret;
    }
    MMIAPICOM_Wstrcat(fullname, (const wchar*)splash);
    MMIAPICOM_Wstrcat(fullname, item.text_data.buf.str_ptr);
    fullname_len = (uint16)MMIAPICOM_Wstrlen(fullname);
    //获取图像数据
    image_len = MMIWWW_Netfront_GetImageData(&image_ptr);
    if (PNULL != image_ptr && image_len > 0)
    {
        //保存
        if (!MMIAPIFMM_IsFileExist(fullname, fullname_len))
        {
            access_mode = SFS_MODE_WRITE | SFS_MODE_OPEN_ALWAYS;/*lint !e655*/
            handle = MMIAPIFMM_CreateFile(fullname, access_mode, NULL, NULL);
            if (0 != handle)
            {
                error = MMIAPIFMM_WriteFile(handle, image_ptr,
                                          image_len, &written, PNULL);
                if (SFS_NO_ERROR == error)
                {
                    bret = TRUE;
                    MMIAPIFMM_CloseFile(handle);
                }
                else
                {
                    //写文件失败
                    text_id  = TXT_ERROR;
                    image_id = IMAGE_PUBWIN_FAIL;
                    MMIAPIFMM_CloseFile(handle);
                    MMIAPIFMM_DeleteFile(fullname, PNULL);
                }
            }
            else
            {
                //打开文件失败
                text_id  = TXT_ERROR;
                image_id = IMAGE_PUBWIN_FAIL;
            }
        }
        else
        {
            //文件名已存在
            text_id  = TXT_FILE_EXISTED;
            image_id = IMAGE_PUBWIN_WARNING;
        }
        SCI_FREE(image_ptr);
    }
    else
    {
        //获取图像数据失败
        text_id  = TXT_WWW_GET_DATA_FAIL;
        image_id = IMAGE_PUBWIN_WARNING;
    }
    //显示结果
    MMIPUB_OpenAlertWinByTextId(PNULL, text_id, TXT_NULL, image_id, PNULL,
                                PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
    return bret;
}

/*****************************************************************************/
//  Description : handle messages of page search win
//  Global resource dependence : none
//  Author: Bill.Ji
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWSaveAsEditWin(MMI_WIN_ID_T     win_id,
                                          MMI_MESSAGE_ID_E msg_id,
                                          DPARAM           param)
{
    MMI_RESULT_E          recode = MMI_RESULT_TRUE;
    MMI_STRING_T          edit_string = {0};
    MMI_HANDLE_T          ctrl_handle = NULL;
    GUIRICHTEXT_ITEM_T    new_item = {0};/*lint !e64*/
    GUIRICHTEXT_ITEM_T    org_item = {0};/*lint !e64*/
    uint32                index = 0;
    WWW_APPLET_T         *applet = PNULL;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_PAGE_SAVE_AS_EDIT_CTRL_ID);
        MMK_SetActiveCtrl(ctrl_handle, FALSE);
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        //get data from edit ctrl
        ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_PAGE_SAVE_AS_EDIT_CTRL_ID);
        GUIEDIT_GetString(ctrl_handle, &edit_string);
        // set to richtext ctrl
        applet = (WWW_APPLET_T *)MMK_GetAppletPtrByWin(win_id);
        ctrl_handle = MMK_GetCtrlHandleByWin(MMK_GetWinHandle(MMIWWW_HANDLE(applet), MMIWWW_PAGE_SAVE_AS_WIN_ID),
                                             MMIWWW_PAGE_SAVE_AS_RICHTEXT_CTRL_ID);
        index = (uint32)MMK_GetWinAddDataPtr(win_id);
        GUIRICHTEXT_GetItem(ctrl_handle, (uint16)index, &org_item);
        SCI_MEMCPY(&new_item, &org_item, sizeof(GUIRICHTEXT_ITEM_T));
        new_item.text_data.buf.len = edit_string.wstr_len;
        new_item.text_data.buf.str_ptr = edit_string.wstr_ptr;
        new_item.text_type = GUIRICHTEXT_TEXT_BUF;
        GUIRICHTEXT_ModifyItem(ctrl_handle, (uint16)index, &new_item);
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
    return (recode);
}

/*****************************************************************************/
//  Description : handle message of sub menu of browser
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWMainmenuOptWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"mmiwww_wintab.c:HandleWWWMainmenuOptWinMsg() msg_id=0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_6558_112_2_18_3_12_14_200,(uint8*)"d", msg_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            MMI_HANDLE_T ctrl_handle = PNULL;
            GUIMENU_POP_SEL_INFO_T  pop_item_info = {0};/*lint !e64*/
            uint8 search_engine = 0;
            MMIWWW_CTRL_E focus_ctrl = MMIWWW_CTRL_NULL;

            focus_ctrl = MMIWWW_NetfrontGetMainMenuFocusCtrl();
            search_engine = MMIWWW_GetSearchEngine();
            ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_POPMENU_MAINMENU_OPT_CTRL_ID);
            pop_item_info.is_static = TRUE;
            pop_item_info.ctrl_id = ctrl_handle;
            pop_item_info.menu_id = ID_WWW_ENGINE_CMCC + search_engine;
            pop_item_info.group_id = MENU_WWW_SEARCH_ENGINE_SEL;
            GUIMENU_SetPopItemStatus(TRUE,&pop_item_info);
            if (MMIWWW_CTRL_ADDRESS_LABEL != focus_ctrl && MMIWWW_CTRL_SEARCH_LABEL != focus_ctrl)
            {
                GUIMENU_SetItemGrayed(MMIWWW_POPMENU_MAINMENU_OPT_CTRL_ID, (MMI_MENU_ID_T)MMK_GetWinAddDataPtr(win_id), ID_WWW_SUBMENU_CLEAN_INPUT_HISTORY, TRUE);
            }
            MMK_SetAtvCtrl(win_id, ctrl_handle);
        }
        break;

    case MSG_CLOSE_WINDOW:
        GUIMENU_SetItemGrayed(MMIWWW_POPMENU_MAINMENU_OPT_CTRL_ID, (MMI_MENU_ID_T)MMK_GetWinAddDataPtr(win_id), ID_WWW_SUBMENU_CLEAN_INPUT_HISTORY, FALSE);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        {
            MMI_MENU_ID_T menu_id = 0;
            MMI_MENU_GROUP_ID_T group_id = 0;
            uint8 search_engine = 0;
            MMI_HANDLE_T applet_handle = MMIWWW_HANDLE((WWW_APPLET_T *)MMK_GetAppletPtrByWin(win_id));

            GUIMENU_GetId(MMK_GetCtrlHandleByWin(win_id, MMIWWW_POPMENU_MAINMENU_OPT_CTRL_ID), &group_id,&menu_id);
            switch (menu_id)
            {
            case ID_WWW_WINDOW_MANAGER:
                mmiwww_OpenWindowManagerWin();
                MMK_CloseWin(win_id);
                break;

            case ID_WWW_SUBMENU_SEARCH_ENGINE:
                break;

            case ID_WWW_HOMPAGE:
                mmiwww_AccessHomepage();
                MMK_CloseWin(win_id);
                break;

            case ID_WWW_SUBMENU_CLEAN_INPUT_HISTORY:
                MMK_CloseWin(win_id);
                MMIPUB_OpenQueryWinByTextId(TXT_COMMON_CONFIRM_DELETE, IMAGE_PUBWIN_QUERY, PNULL, MMIWWW_HandleDelHistory);
                break;

            case ID_WWW_SUBMENU_SAVED_PAGE:
                {
                    MMI_HANDLE_T win_handle = MMK_GetWinHandle(applet_handle, MMIWWW_PAGEMEMO_WIN_ID);
                    if( !MMK_IsOpenWin(win_handle) )
                    {
                        MMIWWW_OpenWWWPageMemoWin();
                    }
                    else
                    {
                        if (!MMK_IsFocusWin(win_handle))
                        {
                            MMK_WinGrabFocus(win_handle);
                        }
                    }
                    MMK_CloseWin(win_id);
                }
                break;

            case ID_WWW_DOWNLOAD_MANAGE:
                MMIAPIDL_StartDownload(MMIDL_OPEN_DOWNLOAD_MANAGER, PNULL);
                MMK_CloseWin(win_id);
                break;

            case ID_WWW_MINIMIZE:
                MMK_CloseWin(win_id);
                MMIAPIWWW_MinimizeWWW();
                break;

            case ID_WWW_EXIT:
                if (MMK_IsOpenWin(mmiwww_GetBrowserWinhandle()))
                {
                    MMK_CloseWin(win_id);
                    MMIPUB_OpenQueryWinByTextId(TXT_EXIT_BROWSER, IMAGE_PUBWIN_QUERY,PNULL,PNULL);
                }
                else
                {
                    MMIAPIWWW_StopWWW();
                }
                break;

            case ID_WWW_SUBMENU_SETTING:
                MMIWWW_OpenWWWSetMenuWin();
                MMK_CloseWin(win_id);
                break;

            case ID_WWW_ENGINE_CMCC:
            case ID_WWW_ENGINE_GOOGLE:
            case ID_WWW_ENGINE_BAIDU:
            case ID_WWW_ENGINE_YAHOO:
                {
                    MMI_IMAGE_ID_T icon_id;
                    MMI_HANDLE_T search_handle = PNULL;
                  
                    applet_handle = MMIWWW_HANDLE((WWW_APPLET_T *)MMK_GetAppletPtrByWin(win_id));
                    search_handle = MMK_GetCtrlHandleByWin(MMK_GetWinHandle(applet_handle, MMIWWW_MAINMENU_WIN_ID), MMIWWW_MAINMENU_LABEL_SEARCH_CTRL_ID);
                    search_engine = (uint8)(menu_id - ID_WWW_ENGINE_CMCC);
                    /* and search */
                    if (MMIWWW_SEARCH_ENGINE_GOOGLE == search_engine)
                    {
                        icon_id = IMAGE_WWW_GOOGLE;
                    }
                    else if (MMIWWW_SEARCH_ENGINE_CMCC == search_engine)
                    {
                        icon_id = IMAGE_WWW_CMCC;
                    }
                    else if (MMIWWW_SEARCH_ENGINE_BAIDU == search_engine)
                    {
                        icon_id = IMAGE_WWW_BAIDU;
                    }
                    else
                    {
                        icon_id = IMAGE_WWW_YAHOO;
                    }
                    GUILABEL_SetIcon(search_handle, icon_id);
                    MMIWWW_SetSearchEngine(&search_engine);
                    MMK_CloseWin(win_id);
                }
                break;

            default:
                break;
            }
        }
        break;

    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
        break;

    case MSG_PROMPTWIN_OK:
        MMIPUB_CloseQuerytWin(PNULL);
        MMIAPIWWW_StopWWW();
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : handle message of www set menu win
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWSetMenuWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_MENU_ID_T         menu_id = 0;
    MMI_MENU_GROUP_ID_T  group_id = 0;
    uint16             list_index = 0;
    MMI_RESULT_E           recode = MMI_RESULT_TRUE;
    
    //SCI_TRACE_LOW:"mmiwww_wintab.c:HandleWWWSetMenuWinMsg() msg_id=0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_6738_112_2_18_3_12_14_201,(uint8*)"d", msg_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIMENU_GetId(MMK_GetCtrlHandleByWin(win_id, MMIWWW_MENU_SET_OPT_CTRL_ID),&group_id,&menu_id);
        if (!MMK_IsOpenWin(mmiwww_GetBrowserWinhandle()))
        {
            GUIMENU_SetItemGrayed(MMIWWW_MENU_SET_OPT_CTRL_ID, group_id, ID_WWW_SETMENU_CHARSET, TRUE);
        }
        else
        {
            GUIMENU_SetItemGrayed(MMIWWW_MENU_SET_OPT_CTRL_ID, group_id, ID_WWW_SETMENU_CHARSET, FALSE);
        }
        
        MMK_SetAtvCtrl(win_id, MMK_GetCtrlHandleByWin(win_id, MMIWWW_MENU_SET_OPT_CTRL_ID));
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        GUIMENU_GetId(MMK_GetCtrlHandleByWin(win_id, MMIWWW_MENU_SET_OPT_CTRL_ID),&group_id,&menu_id);
        switch( menu_id )
        {
        case ID_WWW_SETMENU_VIEWMODE:
            MMIWWW_OpenWWWSetWin(ID_WWW_SETMENU_VIEWMODE);
            break;

        case ID_WWW_SETMENU_DISPALAYOPT:
            MMIWWW_OpenWWWSetWin(ID_WWW_SETMENU_DISPALAYOPT);
            break;

 /*       case ID_WWW_SETMENU_SCRIPT:
            MMIWWW_OpenWWWSetWin(ID_WWW_SETMENU_SCRIPT);
            break;

        case ID_WWW_SETMENU_ANIMATION:
            MMIWWW_OpenWWWSetWin(ID_WWW_SETMENU_ANIMATION);
            break;

        case ID_WWW_SETMENU_PHOTO:
            MMIWWW_OpenWWWSetWin(ID_WWW_SETMENU_PHOTO);
            break;

        case ID_WWW_SETMENU_TABLE:
            MMIWWW_OpenWWWSetWin(ID_WWW_SETMENU_TABLE);
            break;

        case ID_WWW_SETMENU_BGMUSIC:
            MMIWWW_OpenWWWSetWin(ID_WWW_SETMENU_BGMUSIC);
            break;

        case ID_WWW_SETMENU_POPUPWIN:
            MMIWWW_OpenWWWSetWin(ID_WWW_SETMENU_POPUPWIN);
            break;*/

        case ID_WWW_SETMENU_SETFONT:
            MMIWWW_OpenWWWSetWin(ID_WWW_SETMENU_SETFONT);
            break;

        case ID_WWW_SETMENU_SETCOOKIE:
            MMIWWW_OpenWWWSetWin(ID_WWW_SETMENU_SETCOOKIE);
            break;
            
/*        case ID_WWW_SETMENU_COOKIEMODE:
            MMIWWW_OpenWWWSetWin(ID_WWW_SETMENU_COOKIEMODE);
            break;*/

        case ID_WWW_SETMENU_ACCOUNT:
        case ID_WWW_SETMENU_ACCOUNT1:
        case ID_WWW_SETMENU_ACCOUNT2:
#if defined(MMI_MULTI_SIM_SYS_SINGLE)
            list_index = MMIAPICONNECTION_GetListIndexByAPNIndex(MN_DUAL_SYS_1,
                                                                 MMIAPIWWW_GetNetSettingIndex((MN_DUAL_SYS_E)MN_DUAL_SYS_1));
            MMIAPICONNECTION_CreateLinklistWin(MMICONNECTION_ACCOUNT_BROWSER, list_index, MN_DUAL_SYS_1, mmiwww_ConnectionCallback);
#elif defined(MMI_MULTI_SIM_SYS_DUAL)
            list_index = MMIAPICONNECTION_GetListIndexByAPNIndex((ID_WWW_SETMENU_ACCOUNT2 != menu_id ? MN_DUAL_SYS_1 : MN_DUAL_SYS_2),
                         MMIAPIWWW_GetNetSettingIndex((MN_DUAL_SYS_E)(ID_WWW_SETMENU_ACCOUNT2 != menu_id ? MN_DUAL_SYS_1 : MN_DUAL_SYS_2)));
            MMIAPICONNECTION_CreateLinklistWin(MMICONNECTION_ACCOUNT_BROWSER, list_index,
                (ID_WWW_SETMENU_ACCOUNT2 != menu_id ? MN_DUAL_SYS_1 : MN_DUAL_SYS_2), mmiwww_ConnectionCallback);
#else
            //SCI_TRACE_LOW:"HandleWWWSetMenuWinMsg account err"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_6823_112_2_18_3_12_14_202,(uint8*)"");
#endif
            break;

        case ID_WWW_SETMENU_CHARSET:
            MMIWWW_OpenCharsetWin();
            break;

        case ID_WWW_SETMENU_HOMEPAGE:
            MMIWWW_OpenHomepageWin();
            break;

        case ID_WWW_SETMENU_POINTER_MODE:
            MMIWWW_OpenPointerModeWin();
            break;

/*        case ID_WWW_SETMENU_CLEARCOOKIE:
            MMIPUB_OpenQueryWinByTextId(TXT_WWW_IS_CLEAR_COOKIES,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
            break;*/

        case ID_WWW_SETMENU_SETCACHE:
            MMIWWW_OpenWWWSetWin(ID_WWW_SETMENU_SETCACHE);
            break;

/*        case ID_WWW_SETMENU_IF_USE_DISK_CACHE:
            MMIWWW_OpenWWWSetWin(ID_WWW_SETMENU_IF_USE_DISK_CACHE);
            break;*/

/*        case ID_WWW_SETMENU_CLEAR_CACHE:
            MMIPUB_OpenQueryWinByTextId(TXT_WWW_IS_CLEAR_CACHE,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
            break;*/

        case ID_WWW_SETMENU_CERTIFICATE:
            MMIWWW_OpenWWWCertificateWin(MMIWWW_CERT_VIEW_MODE);
            break;

 /*       case ID_WWW_SETMENU_AUTOWEBDATA:
            MMIWWW_OpenWWWSetWin(ID_WWW_SETMENU_AUTOWEBDATA);
            break;

        case ID_WWW_SETMENU_CLEARWEBDATA:
            MMIPUB_OpenQueryWinByTextId(TXT_WWW_CLEAR_WEBDATA,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
            break;*/

      case ID_WWW_SETMENU_WEBDATA:
            MMIWWW_OpenWWWSetWin(ID_WWW_SETMENU_WEBDATA);
            break;

        case ID_WWW_SETMENU_DEFAULTSET:
            MMIPUB_OpenQueryWinByTextId(TXT_RESET_QUERY,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
            break;

        default:
            break;
        }
        break;

    case MSG_PROMPTWIN_OK:
        MMIPUB_CloseQuerytWin(PNULL);
        GUIMENU_GetId( MMK_GetCtrlHandleByWin(win_id, MMIWWW_MENU_SET_OPT_CTRL_ID), &group_id, &menu_id );
        if( ID_WWW_SETMENU_CLEARCOOKIE == menu_id )
        {
            MMIWWW_Netfront_ClearCookie();
            MMIPUB_OpenAlertSuccessWin(TXT_WWW_CLEAR_COOKIES_SUCCESS);
        }
        else if( ID_WWW_SETMENU_CLEAR_CACHE == menu_id)
        {
            MMIWWW_Netfront_ClearCache();
            MMIPUB_OpenAlertSuccessWin(TXT_WWW_CLEAR_CACHE_SUCCESS);
        }
        else if( ID_WWW_SETMENU_CLEARWEBDATA == menu_id )
        {
            MMIWWW_Netfront_ClearWebData();
            MMIPUB_OpenAlertSuccessWin(TXT_COMMON_DELETESUCESS);
        }
        else if( ID_WWW_SETMENU_DEFAULTSET == menu_id )
        {
            MMIWWW_ResetDefaultSettings();

            MMIPUB_OpenAlertSuccessWin(TXT_WWW_RESET_SUCCESS);
        }
        break;

    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
        break;

    case MSG_CLOSE_WINDOW:
        GUIMENU_GetId(MMK_GetCtrlHandleByWin(win_id, MMIWWW_MENU_SET_OPT_CTRL_ID),&group_id,&menu_id);
        GUIMENU_SetItemGrayed(MMIWWW_MENU_SET_OPT_CTRL_ID, group_id, ID_WWW_SETMENU_CHARSET, FALSE);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : handle message of www history win
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWHistoryWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_HANDLE_T applet_handle = MMIWWW_HANDLE((WWW_APPLET_T *)MMK_GetAppletPtrByWin(win_id));
    //SCI_TRACE_LOW:"mmiwww_wintab.c:HandleWWWHistoryWinMsg() msg_id=0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_6928_112_2_18_3_12_14_203,(uint8*)"d", msg_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUI_BOTH_RECT_T bm_list_rect = {0};

            bm_list_rect.v_rect.left = MMIWWW_TAB_LIST_LEFT;
            bm_list_rect.v_rect.top = MMIWWW_TAB_LIST_TOP;
            bm_list_rect.v_rect.right = MMIWWW_TAB_LIST_RIGHT;
            bm_list_rect.v_rect.bottom = MMIWWW_TAB_LIST_BOTTOM;
            bm_list_rect.h_rect.left = MMIWWW_TAB_LIST_LEFT;
            bm_list_rect.h_rect.top = MMIWWW_TAB_LIST_TOP_H;
            bm_list_rect.h_rect.right = MMIWWW_TAB_LIST_RIGHT_H;
            bm_list_rect.h_rect.bottom = MMIWWW_TAB_LIST_BOTTOM_H;

            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIWWW_LISTBOX_HISTORY_CTRL_ID), &bm_list_rect);

            MMIWWW_AppendListboxitem( win_id, MMIWWW_LISTBOX_HISTORY_CTRL_ID, (uint16)MMIWWW_Netfront_GetVisitHistoryNumber(), TXT_WWW_HISTORY_EMPTY, STXT_OPTION, TXT_WWW_EXIT);

#ifdef MMI_PDA_SUPPORT
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif

            GUILIST_SetCycScroll(MMK_GetCtrlHandleByWin(win_id, MMIWWW_LISTBOX_HISTORY_CTRL_ID), FALSE);
        }
        break;

    case MSG_GET_FOCUS:
        MMIWWW_AppendListboxitem( win_id, MMIWWW_LISTBOX_HISTORY_CTRL_ID, (uint16)MMIWWW_Netfront_GetVisitHistoryNumber(), TXT_WWW_HISTORY_EMPTY, STXT_OPTION, TXT_WWW_EXIT);

        if (MMIWWW_CTRL_HISTORY_LIST == MMIWWW_NetfrontGetMainMenuFocusCtrl())
        {
            MMK_SetActiveCtrl(MMK_GetCtrlHandleByWin(win_id, MMIWWW_LISTBOX_HISTORY_CTRL_ID), TRUE);
        }
        break;

    case MSG_LOSE_FOCUS:
        break;

    case MSG_CTL_LIST_NEED_ITEM_DATA:
        {
            MMI_STRING_T title_string_info = {0};
            GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;

            if (PNULL == need_item_data_ptr)
            {
                break;
            }
            
            MMIWWW_Netfront_GetVisitHistoryTitle( need_item_data_ptr->item_index, &title_string_info );

            if (PNULL == title_string_info.wstr_ptr || 0 == title_string_info.wstr_len)
            {
                MMIWWW_Netfront_GetVisitHistoryUrl( need_item_data_ptr->item_index, &title_string_info );
            }

            MMIWWW_AppendTitleItemData( MMK_GetCtrlHandleByWin(win_id, MMIWWW_LISTBOX_HISTORY_CTRL_ID), need_item_data_ptr->item_index, title_string_info, IMAGE_WWW_PAGE, STXT_OPTION, TXT_COMMON_CONNECT_COMN, TXT_WWW_EXIT );

            if (title_string_info.wstr_ptr)
            {
                SCI_FREE(title_string_info.wstr_ptr);
            }
        }
        break;

    case MSG_NOTIFY_GET_ACTIVE:
        {
        uint32 tmp_win_id = MMIWWW_HISTORY_WIN_ID;  
        MMIWWW_NetfrontSetMainMenuFocusCtrl(MMIWWW_CTRL_HISTORY_LIST);
        MMK_PostMsg(MMK_GetWinHandle(applet_handle, MMIWWW_MAINMENU_WIN_ID), MSG_FULL_PAINT, (DPARAM)&tmp_win_id, sizeof(uint32));
        }
        return MMI_RESULT_FALSE;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_SetActiveCtrl(MMK_GetCtrlHandleByWin(MMK_GetWinHandle(applet_handle, MMIWWW_MAINMENU_WIN_ID), MMIWWW_MAINMENU_TAB_CTRL_ID), FALSE);
        MMIWWW_NetfrontSetMainMenuFocusCtrl(MMIWWW_CTRL_SEARCH_LABEL);
        MMK_PostMsg(MMK_GetWinHandle(applet_handle, MMIWWW_MAINMENU_WIN_ID), MSG_FULL_PAINT, PNULL, 0);
        break;

    case MSG_NOTIFY_LIST_HEAD:
        MMK_SetActiveCtrl(MMK_GetCtrlHandleByWin(MMK_GetWinHandle(applet_handle, MMIWWW_MAINMENU_WIN_ID), MMIWWW_MAINMENU_TAB_CTRL_ID), FALSE);
        MMIWWW_NetfrontSetMainMenuFocusCtrl(MMIWWW_CTRL_SEARCH_LABEL);
        MMK_PostMsg(MMK_GetWinHandle(applet_handle, MMIWWW_MAINMENU_WIN_ID), MSG_FULL_PAINT, PNULL, 0);
        break;

    case MSG_NOTIFY_LIST_TAIL:
        {
            MMI_HANDLE_T ctrl_handle = PNULL;

            ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_LISTBOX_HISTORY_CTRL_ID);
            if (GUILIST_GetTotalItemNum(ctrl_handle) > 1)
            {
                GUILIST_SetCurItemIndex(ctrl_handle, 0);
                MMK_PostMsg(MMK_GetWinHandle(applet_handle, MMIWWW_MAINMENU_WIN_ID), MSG_FULL_PAINT, PNULL, 0);
            }
        }
        break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_MENU:
        MMIWWW_OpenWWWHistoryOptWin(MMK_IsOpenWin(mmiwww_GetBrowserWinhandle()));
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        if(MMIWWW_Netfront_GetVisitHistoryNumber() > 0)
        {
            MMIWWW_Netfront_History_GotoURL(GUILIST_GetCurItemIndex(MMK_GetCtrlHandleByWin(win_id, MMIWWW_LISTBOX_HISTORY_CTRL_ID)));
        }
        break;

    case MSG_CLOSE_WINDOW:
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : handle message of www pagememo win
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWPageMemoWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    uint16 item_selected = 0;
    MMI_HANDLE_T ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_LISTBOX_PAGEMEMO_CTRL_ID);
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint16 memo_num = 0;

    //SCI_TRACE_LOW:"mmiwww_wintab.c:HandleWWWPageMemoWinMsg() msg_id=0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_7066_112_2_18_3_12_15_204,(uint8*)"d", msg_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:    
#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
        MMK_SetAtvCtrl(win_id, ctrl_handle);
        break;

    case MSG_FULL_PAINT:
        //Set title
        memo_num = (uint16)MMIWWW_Netfront_GetPageMemoNumber();

        if(memo_num > 0)
        {
            GUILIST_SetTitleIndexType( ctrl_handle, GUILIST_TITLE_INDEX_DEFAULT );
        }
        else
        {
            GUILIST_SetTitleIndexType( ctrl_handle, GUILIST_TITLE_INDEX_EXCLUDE_FIRST );
        }

        MMIWWW_AppendListboxitem( win_id, MMIWWW_LISTBOX_PAGEMEMO_CTRL_ID, memo_num, TXT_WWW_PAGEMEMO_EMPTY, 0, STXT_RETURN);
        break;

    case MSG_WWW_RESET_LIST:
        MMIWWW_AppendListboxitem( win_id, MMIWWW_LISTBOX_PAGEMEMO_CTRL_ID, (uint16)MMIWWW_Netfront_GetPageMemoNumber(), TXT_WWW_PAGEMEMO_EMPTY, 0, STXT_RETURN);
        break;

    case MSG_CTL_LIST_NEED_ITEM_DATA:
        {
            MMI_STRING_T title_string_info = {0};
            GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;
            BOOLEAN is_free = TRUE;

            if (PNULL == need_item_data_ptr)
            {
                break;
            }

            MMIWWW_Netfront_GetPageMemoTitle( need_item_data_ptr->item_index, &title_string_info );

            //SCI_TRACE_LOW:"mmiwww_wintab.c:HandleWWWPageMemoWinMsg() strlen  =%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_7110_112_2_18_3_12_15_205,(uint8*)"d", title_string_info.wstr_len);
            if (PNULL == title_string_info.wstr_ptr || 0 == title_string_info.wstr_len)
            {
                MMIRES_GetText(TXT_NO_TITLE, win_id, &title_string_info);
                is_free = FALSE;
            }
            
            MMIWWW_AppendTitleItemData( ctrl_handle, need_item_data_ptr->item_index, title_string_info, IMAGE_WWW_PAGE, STXT_OPTION, TXT_COMM_OPEN, STXT_RETURN );

            if (is_free && title_string_info.wstr_ptr)
            {
                SCI_FREE(title_string_info.wstr_ptr);
            }
        }
        break;

    case MSG_GET_FOCUS:
        /* TODO: refresh Softkey */
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_MENU:
        if( MMIWWW_Netfront_GetPageMemoNumber() > 0 )
        {
            MMIWWW_OpenWWWPageMemoOptWin();
        }
        break;

    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        if( MMIWWW_Netfront_GetPageMemoNumber() > 0 )
        {
            MMI_HANDLE_T win_handle = PNULL;

            win_handle = mmiwww_GetBrowserWinhandle();
            item_selected = GUILIST_GetCurItemIndex(ctrl_handle);
            if( !MMK_IsOpenWin(win_handle) )
            {
                MMIWWW_OpenWWWBrowserWin(BROWSER_START_NORMAL, PNULL);
                if (!MMIWWW_IsFirstOpen()) /* Open pagememo here else open pagememo in mmiwww_HandleNetAccessPagememo*/
                {
                    MMK_CloseWin(win_id);
                    MMIWWW_Netfront_LoadPageMemo(item_selected);
                    MMIWWW_SetWWWState(MMIWWW_Netfront_GetActWinIndex(), MMIWWW_STATE_PAGE_LOADING);
                }
            }
            else
            {
                MMK_CloseWin(win_id);
                if (!MMK_IsFocusWin(win_handle))
                {
                    MMK_WinGrabFocus(win_handle);
                }
                MMIWWW_Netfront_LoadPageMemo(item_selected);
                MMIWWW_SetWWWState(MMIWWW_Netfront_GetActWinIndex(), MMIWWW_STATE_PAGE_LOADING);
            }
        }
        break;

    case MSG_CLOSE_WINDOW:
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : handle message of www certificate win
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWCertificateWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    uint16 cer_num = 0;
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMIWWW_CERT_MODE_E mode = (uint32)MMK_GetWinAddDataPtr(win_id);/*lint !e64*/

    //SCI_TRACE_LOW:"mmiwww_wintab.c:HandleWWWCertificateWinMsg() msg_id=0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_7197_112_2_18_3_12_15_206,(uint8*)"d", msg_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        cer_num = (uint16)(MMIWWW_Netfront_GetX509CertNumber(NFBROWSER_CERTLIST_TYPE_ROOT) + MMIWWW_Netfront_GetX509CertNumber(NFBROWSER_CERTLIST_TYPE_CA) + MMIWWW_Netfront_GetX509CertNumber(NFBROWSER_CERTLIST_TYPE_SERVER));
        MMIWWW_AppendListboxitem( win_id, MMIWWW_LISTBOX_CERTIFICATE_CTRL_ID, cer_num, TXT_WWW_NO_HISTORY, 0, STXT_RETURN);
        MMK_SetAtvCtrl(win_id, MMK_GetCtrlHandleByWin(win_id, MMIWWW_LISTBOX_CERTIFICATE_CTRL_ID));
        break;

    case MSG_FULL_PAINT:
        break;

    case MSG_CTL_LIST_NEED_ITEM_DATA:
        {
            MMI_STRING_T title_string_info = {0};
            GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;

            if (PNULL == need_item_data_ptr)
            {
                break;
            }

            if( need_item_data_ptr->item_index < MMIWWW_Netfront_GetX509CertNumber(NFBROWSER_CERTLIST_TYPE_ROOT))
            {
                MMIWWW_Netfront_GetX509CertName( NFBROWSER_CERTLIST_TYPE_ROOT, need_item_data_ptr->item_index, &title_string_info );
            }
            else if( (need_item_data_ptr->item_index >= MMIWWW_Netfront_GetX509CertNumber(NFBROWSER_CERTLIST_TYPE_ROOT))
                &&(need_item_data_ptr->item_index < (MMIWWW_Netfront_GetX509CertNumber(NFBROWSER_CERTLIST_TYPE_ROOT) + MMIWWW_Netfront_GetX509CertNumber(NFBROWSER_CERTLIST_TYPE_CA))))
            {
                MMIWWW_Netfront_GetX509CertName( NFBROWSER_CERTLIST_TYPE_CA, (need_item_data_ptr->item_index - MMIWWW_Netfront_GetX509CertNumber(NFBROWSER_CERTLIST_TYPE_ROOT)), &title_string_info );
            }
            else
            {
                MMIWWW_Netfront_GetX509CertName( NFBROWSER_CERTLIST_TYPE_SERVER, need_item_data_ptr->item_index - MMIWWW_Netfront_GetX509CertNumber(NFBROWSER_CERTLIST_TYPE_ROOT)
                    - MMIWWW_Netfront_GetX509CertNumber(NFBROWSER_CERTLIST_TYPE_CA), &title_string_info );
            }          

            if( MMIWWW_CERT_VIEW_MODE == mode ) 
            {
                MMIWWW_AppendTitleItemData( MMK_GetCtrlHandleByWin(win_id, MMIWWW_LISTBOX_CERTIFICATE_CTRL_ID), need_item_data_ptr->item_index, title_string_info, IMAGE_WWW_CERT_ITEM, TXT_VIEW, 0, STXT_RETURN );
            }
            else
            {
                MMIWWW_AppendTitleItemData( MMK_GetCtrlHandleByWin(win_id, MMIWWW_LISTBOX_CERTIFICATE_CTRL_ID), need_item_data_ptr->item_index, title_string_info, IMAGE_WWW_CERT_ITEM, TXT_VIEW, TXT_COMMON_IMPORT, STXT_RETURN );
            }
            
            if (title_string_info.wstr_ptr)
            {
                SCI_FREE(title_string_info.wstr_ptr);
            }
        }
        break;
 
    case MSG_GET_FOCUS:
        /* TODO: refresh Softkey */
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_OK:
    case MSG_CTL_OK:
        MMIWWW_OpenWWWCertificateAttributeWin();
        break;

    case MSG_CLOSE_WINDOW:
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : handle message of www history option win
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWHistoryOptWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    uint16 selected_item = 0;
    MMI_MENU_ID_T menu_id = 0;
    MMI_STRING_T url_string_info = {0};
    MMI_HANDLE_T applet_handle = MMIWWW_HANDLE((WWW_APPLET_T *)MMK_GetAppletPtrByWin(win_id));
    MMI_HANDLE_T history_list_ctrl_handle = PNULL;
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"mmiwww_wintab.c:HandleWWWHistoryOptWinMsg() msg_id=0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_7293_112_2_18_3_12_15_207,(uint8*)"d", msg_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            uint16 history_num = (uint16)MMIWWW_Netfront_GetVisitHistoryNumber();
            group_id = (MMI_MENU_ID_T)MMK_GetWinAddDataPtr(win_id);

            if (MMIWWW_CTRL_TAB == MMIWWW_NetfrontGetMainMenuFocusCtrl())
            {
                GUIMENU_SetItemGrayed(MMIWWW_POPMENU_HISTORY_OPT_CTRL_ID, group_id, ID_WWW_DELETE_OPT_DELETE_CUR, TRUE);
                GUIMENU_SetItemGrayed(MMIWWW_POPMENU_HISTORY_OPT_CTRL_ID, group_id, ID_WWW_DELETE_OPT_DELETE_CURDAY, TRUE);
                GUIMENU_SetItemGrayed(MMIWWW_POPMENU_HISTORY_OPT_CTRL_ID, group_id, ID_WWW_OPT_SHOW_ADDRESS, TRUE);
                GUIMENU_SetItemGrayed(MMIWWW_POPMENU_HISTORY_OPT_CTRL_ID, group_id, ID_WWW_OPT_SEND_LINK, TRUE);
                if (history_num < 1)
                {
                    GUIMENU_SetItemGrayed(MMIWWW_POPMENU_HISTORY_OPT_CTRL_ID, group_id, ID_WWW_DELETE_OPT_DELETE_ALL, TRUE);
                }
            }
            else
            {
                if (history_num < 1)
                {
                    group_id = (MMI_MENU_ID_T)MMK_GetWinAddDataPtr(win_id);

                    GUIMENU_SetItemGrayed(MMIWWW_POPMENU_HISTORY_OPT_CTRL_ID, group_id, ID_WWW_DELETE_OPT_DELETE_CUR, TRUE);
                    GUIMENU_SetItemGrayed(MMIWWW_POPMENU_HISTORY_OPT_CTRL_ID, group_id, ID_WWW_DELETE_OPT_DELETE_CURDAY, TRUE);
                    GUIMENU_SetItemGrayed(MMIWWW_POPMENU_HISTORY_OPT_CTRL_ID, group_id, ID_WWW_DELETE_OPT_DELETE_ALL, TRUE);
                    GUIMENU_SetItemGrayed(MMIWWW_POPMENU_HISTORY_OPT_CTRL_ID, group_id, ID_WWW_OPT_SHOW_ADDRESS, TRUE);
                    GUIMENU_SetItemGrayed(MMIWWW_POPMENU_HISTORY_OPT_CTRL_ID, group_id, ID_WWW_OPT_SEND_LINK, TRUE);
                }
            }

            MMK_SetActiveCtrl(MMK_GetCtrlHandleByWin(win_id, MMIWWW_POPMENU_HISTORY_OPT_CTRL_ID), FALSE);
        }
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        {
            /* reset the global variable of menu */
            group_id = (MMI_MENU_ID_T)MMK_GetWinAddDataPtr(win_id);

            GUIMENU_SetItemGrayed(MMIWWW_POPMENU_HISTORY_OPT_CTRL_ID, group_id, ID_WWW_DELETE_OPT_DELETE_CUR, FALSE);
            GUIMENU_SetItemGrayed(MMIWWW_POPMENU_HISTORY_OPT_CTRL_ID, group_id, ID_WWW_DELETE_OPT_DELETE_CURDAY, TRUE);
            GUIMENU_SetItemGrayed(MMIWWW_POPMENU_HISTORY_OPT_CTRL_ID, group_id, ID_WWW_DELETE_OPT_DELETE_ALL, FALSE);
            GUIMENU_SetItemGrayed(MMIWWW_POPMENU_HISTORY_OPT_CTRL_ID, group_id, ID_WWW_OPT_SHOW_ADDRESS, FALSE);
            GUIMENU_SetItemGrayed(MMIWWW_POPMENU_HISTORY_OPT_CTRL_ID, group_id, ID_WWW_OPT_SEND_LINK, FALSE);
        }
        break;

    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK: 
        GUIMENU_GetId(MMK_GetCtrlHandleByWin(win_id, MMIWWW_POPMENU_HISTORY_OPT_CTRL_ID),&group_id,&menu_id);
        switch( menu_id )
        {
        case ID_WWW_HOMPAGE:
            mmiwww_AccessHomepage();
            MMK_CloseWin(win_id);
            break;

        case ID_WWW_DELETE_OPT_DELETE_CUR:
            MMIPUB_OpenQueryWinByTextId(TXT_WWW_IS_DELETE_CUR_HISTORY,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
            break;
        case ID_WWW_DELETE_OPT_DELETE_ALL:
            MMIPUB_OpenQueryWinByTextId(TXT_WWW_IS_DELETE_ALL_HISTORY,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
            break;
        case ID_WWW_DELETE_OPT_DELETE_CURDAY:
            MMIPUB_OpenQueryWinByTextId(TXT_COMMON_CONFIRM_DELETE,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
            break;
        case ID_WWW_SEND_OPT_VIA_MMS:
        case ID_WWW_SEND_OPT_VIA_SMS:
            history_list_ctrl_handle = MMK_GetCtrlHandleByWin(MMK_GetWinHandle( applet_handle,MMIWWW_HISTORY_WIN_ID), MMIWWW_LISTBOX_HISTORY_CTRL_ID);
            selected_item = GUILIST_GetCurItemIndex(history_list_ctrl_handle);
            MMIWWW_Netfront_GetVisitHistoryUrl( selected_item, &url_string_info );
            if(ID_WWW_SEND_OPT_VIA_SMS == menu_id)
            {
                MMIAPISMS_WriteNewMessage( MN_DUAL_SYS_MAX, &url_string_info, PNULL, 0 );
            }
            else
            {
                MMIAPIMMS_BrwSendUrlByMMS(&url_string_info);
            }           
            SCI_FREE(url_string_info.wstr_ptr);
            MMK_CloseWin(win_id);
            break;
            
        case ID_WWW_OPT_SHOW_ADDRESS:
            MMIWWW_OpenWWWHistoryAttributeWin(MMIWWW_PAGE_ATTRIBUTE_HISTORY);
            MMK_CloseWin(win_id);
            break;

        case ID_WWW_SUBMENU_SAVED_PAGE:
            {
                MMI_HANDLE_T win_handle = MMK_GetWinHandle(applet_handle, MMIWWW_PAGEMEMO_WIN_ID);
                if( !MMK_IsOpenWin(win_handle) )
                {
                    MMIWWW_OpenWWWPageMemoWin();
                }
                else
                {
                    if (!MMK_IsFocusWin(win_handle))
                    {
                        MMK_WinGrabFocus(win_handle);
                    }
                }
                MMK_CloseWin(win_id);
            }
            break;

        case ID_WWW_DOWNLOAD_MANAGE:
            MMIAPIDL_StartDownload(MMIDL_OPEN_DOWNLOAD_MANAGER, PNULL);
            MMK_CloseWin(win_id);
            break;

        case ID_WWW_SUBMENU_SETTING:
            MMIWWW_OpenWWWSetMenuWin();
            MMK_CloseWin(win_id);
            break;

        case ID_WWW_WINDOW_MANAGER:
            mmiwww_OpenWindowManagerWin();
            MMK_CloseWin(win_id);
            break;

        case ID_WWW_MINIMIZE:
            MMK_CloseWin(win_id);
            MMIAPIWWW_MinimizeWWW();
            break;

        case ID_WWW_EXIT:
            if (MMK_IsOpenWin(mmiwww_GetBrowserWinhandle()))
            {
                MMK_CloseWin(win_id);
                MMIPUB_OpenQueryWinByTextId(TXT_EXIT_BROWSER, IMAGE_PUBWIN_QUERY,PNULL,PNULL);
            }
            else
            {
                MMIAPIWWW_StopWWW();
            }
            break;

        default:
            break;
        }
        break;

    case MSG_PROMPTWIN_OK:
        MMIPUB_CloseQuerytWin(PNULL);
        history_list_ctrl_handle = MMK_GetCtrlHandleByWin(MMK_GetWinHandle( applet_handle,MMIWWW_HISTORY_WIN_ID), MMIWWW_LISTBOX_HISTORY_CTRL_ID);
        selected_item = GUILIST_GetCurItemIndex(history_list_ctrl_handle);
        GUIMENU_GetId( MMK_GetCtrlHandleByWin(win_id, MMIWWW_POPMENU_HISTORY_OPT_CTRL_ID), &group_id, &menu_id );
        if( ID_WWW_DELETE_OPT_DELETE_CUR == menu_id )
        {
            MMIWWW_Netfront_RemoveVisitHistory(selected_item);
            GUILIST_RemoveItem( history_list_ctrl_handle, selected_item );
        }
        else if( ID_WWW_DELETE_OPT_DELETE_ALL == menu_id)
        {
            MMIWWW_Netfront_RemoveAllVisitHistory();
            GUILIST_RemoveAllItems( history_list_ctrl_handle );
        }
        else if( ID_WWW_DELETE_OPT_DELETE_CURDAY == menu_id )
        {
            int32 i = 0;
            int32 his_num = 0;
            SCI_DATE_T sys_date = {0};
            uint32 day_cnt = 0;
            slim_time time = {0};

            TM_GetSysDate(&sys_date);
            day_cnt = MMIWWW_CountDate_From1970(sys_date);
            his_num = MMIWWW_Netfront_GetVisitHistoryNumber();
            for (i = his_num - 1; i >= 0; i--)
            {
                MMIWWW_Netfront_GetHistoryTime(i, &time);

                if(day_cnt == (uint32)(time.day))
                {
                    MMIWWW_Netfront_RemoveVisitHistory(i);
                    GUILIST_RemoveItem(history_list_ctrl_handle, i);
                }
            }
        }

        MMIPUB_OpenAlertSuccessWin(TXT_DELETED);
        MMK_CloseWin(win_id);
        break;

    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : handle message of www pagememo option win
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWWWPageMemoOptWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    uint16 selected_item = 0;
    MMI_STRING_T url_string_info = {0};
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_MENU_ID_T menu_id = 0;
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_HANDLE_T applet_handle = MMIWWW_HANDLE((WWW_APPLET_T *)MMK_GetAppletPtrByWin(win_id));
    MMI_HANDLE_T pagememo_list_ctrl_handle = PNULL;
    uint16 memo_num = 0;
    uint8 pagememo_sort_type = 0;

    //SCI_TRACE_LOW:"mmiwww_wintab.c:HandleWWWPageMemoOptWinMsg() msg_id=0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_7519_112_2_18_3_12_16_208,(uint8*)"d", msg_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MMK_GetCtrlHandleByWin(win_id, MMIWWW_POPMENU_PAGEMEMO_OPT_CTRL_ID));
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK: 
        GUIMENU_GetId(MMK_GetCtrlHandleByWin(win_id, MMIWWW_POPMENU_PAGEMEMO_OPT_CTRL_ID),&group_id,&menu_id);
        switch( menu_id )
        {
        case ID_WWW_OPT_SORT_BY_DATE:
            pagememo_sort_type = (uint8)MMIWWW_PAGEMEMO_SORT_BY_DATE;
            MMINV_WRITE( MMINV_WWW_PAGEMEMO_SORT_TYPE, &pagememo_sort_type );
            MMK_PostMsg(MMK_GetWinHandle(applet_handle, MMIWWW_PAGEMEMO_WIN_ID), MSG_WWW_RESET_LIST, PNULL, 0);
            MMIWWW_Netfront_SetPageMemoSortType(MMIWWW_PAGEMEMO_SORT_BY_DATE);
            MMK_CloseWin(win_id);
            break;
        case ID_WWW_OPT_SORT_BY_NAME:
            pagememo_sort_type = (uint8)MMIWWW_PAGEMEMO_SORT_BY_NAME;
            MMINV_WRITE( MMINV_WWW_PAGEMEMO_SORT_TYPE, &pagememo_sort_type );
            MMIWWW_Netfront_SetPageMemoSortType(MMIWWW_PAGEMEMO_SORT_BY_NAME);
            MMK_PostMsg(MMK_GetWinHandle(applet_handle, MMIWWW_PAGEMEMO_WIN_ID), MSG_WWW_RESET_LIST, PNULL, 0);
            MMK_CloseWin(win_id);
            break;

        case ID_WWW_DELETE_OPT_DELETE_CUR:
            MMIPUB_OpenQueryWinByTextId(TXT_COMMON_CONFIRM_DELETE,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
            break;

        case ID_WWW_DELETE_OPT_DELETE_ALL:
            MMIPUB_OpenQueryWinByTextId(TXT_COMMON_CONFIRM_DELETE,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
            break;

        case ID_WWW_SEND_OPT_VIA_MMS:
        case ID_WWW_SEND_OPT_VIA_SMS:
            pagememo_list_ctrl_handle = MMK_GetCtrlHandleByWin(MMK_GetWinHandle( applet_handle,MMIWWW_PAGEMEMO_WIN_ID), MMIWWW_LISTBOX_PAGEMEMO_CTRL_ID);
            selected_item = GUILIST_GetCurItemIndex(pagememo_list_ctrl_handle);
            MMIWWW_Netfront_GetPageMemoUrl( selected_item, &url_string_info );
            if(ID_WWW_SEND_OPT_VIA_SMS == menu_id)
            {
                MMIAPISMS_WriteNewMessage( MN_DUAL_SYS_MAX, &url_string_info, PNULL, 0 );
            }
            else
            {
                MMIAPIMMS_BrwSendUrlByMMS(&url_string_info);
            }
            
            SCI_FREE(url_string_info.wstr_ptr);
            MMK_CloseWin(win_id);
            break;

        case ID_WWW_OPT_SHOW_ADDRESS:
            MMIWWW_OpenWWWHistoryAttributeWin(MMIWWW_PAGE_ATTRIBUTE_PAGEMEMO);
            MMK_CloseWin(win_id);
            break;

        default:
            break;
        }
        break;

    case MSG_PROMPTWIN_OK:
        MMIPUB_CloseQuerytWin(PNULL);

        pagememo_list_ctrl_handle = MMK_GetCtrlHandleByWin(MMK_GetWinHandle( applet_handle,MMIWWW_PAGEMEMO_WIN_ID), MMIWWW_LISTBOX_PAGEMEMO_CTRL_ID);
        selected_item = GUILIST_GetCurItemIndex(pagememo_list_ctrl_handle);
        GUIMENU_GetId(MMK_GetCtrlHandleByWin(win_id, MMIWWW_POPMENU_PAGEMEMO_OPT_CTRL_ID),&group_id,&menu_id);
        if( ID_WWW_DELETE_OPT_DELETE_CUR == menu_id )
        {
            MMIWWW_Netfront_RemovePageMemo(selected_item);
            GUILIST_RemoveItem( pagememo_list_ctrl_handle, selected_item );
            memo_num = (uint16)MMIWWW_Netfront_GetPageMemoNumber();
            if(memo_num)
            {
                GUILIST_SetTitleIndexType( pagememo_list_ctrl_handle, GUILIST_TITLE_INDEX_DEFAULT );
            }
            else
            {
                GUILIST_SetTitleIndexType( pagememo_list_ctrl_handle, GUILIST_TITLE_INDEX_NONE );
            }
        }
        else if( ID_WWW_DELETE_OPT_DELETE_ALL == menu_id)
        {
            MMIWWW_Netfront_RemoveAllPageMemo();
            GUILIST_RemoveAllItems( pagememo_list_ctrl_handle );
            GUILIST_SetTitleIndexType( pagememo_list_ctrl_handle, GUILIST_TITLE_INDEX_NONE );
        }
        MMIPUB_OpenAlertSuccessWin(TXT_DELETED);
        MMK_CloseWin(win_id);
        break;

    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : add history attribute to richtext
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
LOCAL void MMIWWW_Append_History_Attribute( MMI_WIN_ID_T win_id, MMIWWW_PAGEMEMO_ATTRIBUTE_TYPE_E type )
{
    uint16 index = 0;
    uint16 selected_item = 0;
    MMI_HANDLE_T applet_handle = PNULL;
    MMI_HANDLE_T list_ctrl_handle = PNULL;
    GUIRICHTEXT_ITEM_T item_data = {0};/*lint !e64*/
    MMI_STRING_T string_show = {0};
    MMI_STRING_T title_string_info = {0};
    MMI_STRING_T url_string_info = {0};

    item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
    item_data.text_type = GUIRICHTEXT_TEXT_BUF; 

    applet_handle = MMIWWW_HANDLE((WWW_APPLET_T *)MMK_GetAppletPtrByWin(win_id));

    if( type == MMIWWW_PAGE_ATTRIBUTE_HISTORY )
    {
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_CONNECT_COMN, TXT_NULL, STXT_RETURN, TRUE);
    }
    else if( type == MMIWWW_PAGE_ATTRIBUTE_PAGEMEMO )
    {
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMM_OPEN, TXT_NULL, STXT_RETURN, TRUE);
    }
    
    if( type == MMIWWW_PAGE_ATTRIBUTE_HISTORY )
    {
        list_ctrl_handle = MMK_GetCtrlHandleByWin( MMK_GetWinHandle( applet_handle, MMIWWW_HISTORY_WIN_ID ), MMIWWW_LISTBOX_HISTORY_CTRL_ID );
        selected_item = GUILIST_GetCurItemIndex(list_ctrl_handle);
    }
    else if( type == MMIWWW_PAGE_ATTRIBUTE_PAGEMEMO )
    {
        list_ctrl_handle = MMK_GetCtrlHandleByWin(MMK_GetWinHandle( applet_handle,MMIWWW_PAGEMEMO_WIN_ID), MMIWWW_LISTBOX_PAGEMEMO_CTRL_ID);
        selected_item = GUILIST_GetCurItemIndex(list_ctrl_handle);
    }     

    MMI_GetLabelTextByLang(TXT_COMM_TITLE, &string_show);
    item_data.text_data.buf.len = string_show.wstr_len;
    item_data.text_data.buf.str_ptr = string_show.wstr_ptr;
    GUIRICHTEXT_AddItem(MMK_GetCtrlHandleByWin(win_id, MMIWWW_RICHTEXT_HISTORY_ATTRIBUTE_CTRL_ID), &item_data, &index); 

    item_data.text_frame_type = GUIRICHTEXT_FRAME_NONE;
    if( type == MMIWWW_PAGE_ATTRIBUTE_HISTORY )
    {
        MMIWWW_Netfront_GetVisitHistoryTitle( selected_item, &title_string_info );
    }
    else if( type == MMIWWW_PAGE_ATTRIBUTE_PAGEMEMO )
    {
        MMIWWW_Netfront_GetPageMemoTitle( selected_item, &title_string_info );
    }  
    item_data.text_data.buf.len = title_string_info.wstr_len;
    item_data.text_data.buf.str_ptr = title_string_info.wstr_ptr;
    GUIRICHTEXT_AddItem(MMK_GetCtrlHandleByWin(win_id, MMIWWW_RICHTEXT_HISTORY_ATTRIBUTE_CTRL_ID), &item_data, &index); 
    SCI_FREE(title_string_info.wstr_ptr);

    item_data.text_frame_type = GUIRICHTEXT_FRAME_NONE;
    MMI_GetLabelTextByLang(TXT_COMMON_URL, &string_show);
    item_data.text_data.buf.len = string_show.wstr_len;
    item_data.text_data.buf.str_ptr = string_show.wstr_ptr;
    GUIRICHTEXT_AddItem(MMK_GetCtrlHandleByWin(win_id, MMIWWW_RICHTEXT_HISTORY_ATTRIBUTE_CTRL_ID), &item_data, &index); 

    item_data.text_frame_type = GUIRICHTEXT_FRAME_NONE;
    if( type == MMIWWW_PAGE_ATTRIBUTE_HISTORY )
    {
        MMIWWW_Netfront_GetVisitHistoryUrl( selected_item, &url_string_info );
    }
    else if( type == MMIWWW_PAGE_ATTRIBUTE_PAGEMEMO )
    {
        MMIWWW_Netfront_GetPageMemoUrl( selected_item, &url_string_info );
    }
    item_data.text_data.buf.len = url_string_info.wstr_len;
    item_data.text_data.buf.str_ptr = url_string_info.wstr_ptr;
    GUIRICHTEXT_AddItem(MMK_GetCtrlHandleByWin(win_id, MMIWWW_RICHTEXT_HISTORY_ATTRIBUTE_CTRL_ID), &item_data, &index); 
    SCI_FREE(url_string_info.wstr_ptr);
}

/*****************************************************************************/
//  Description : append certificate attribute to richtext
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
LOCAL void MMIWWW_Append_Certificate_Attribute( MMI_WIN_ID_T win_id )
{
    uint16 index = 0;
    uint16 str_len = 0;
    uint16 selected_item = 0;
    MMI_HANDLE_T applet_handle = PNULL;
    MMI_HANDLE_T list_ctrl_handle = PNULL;
    GUIRICHTEXT_ITEM_T item_data = {0};/*lint !e64*/
    MMI_STRING_T string_lab = {0};
    NFBrowserCertInfo *CertInfo_ptr = PNULL;
    char data_str[GUIRICHTEXT_TEXT_MAX_LEN + 1] = {0};
    char date[MMISET_DATE_STR_LEN] = {0};
    char time[MMISET_TIME_STR_12HOURS_LEN + 1] = {0};
    wchar item_text[GUIRICHTEXT_TEXT_MAX_LEN + 1] = {0};
    
    item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
    item_data.text_type = GUIRICHTEXT_TEXT_BUF; 

    applet_handle = MMIWWW_HANDLE((WWW_APPLET_T *)MMK_GetAppletPtrByWin(win_id));
    list_ctrl_handle = MMK_GetCtrlHandleByWin( MMK_GetWinHandle( applet_handle, MMIWWW_CERTIFICATE_WIN_ID ), MMIWWW_LISTBOX_CERTIFICATE_CTRL_ID );
    selected_item = GUILIST_GetCurItemIndex(list_ctrl_handle);

    if( selected_item < MMIWWW_Netfront_GetX509CertNumber(NFBROWSER_CERTLIST_TYPE_ROOT))
    {
        CertInfo_ptr = MMIWWW_Netfront_GetX509CertData(NFBROWSER_CERTLIST_TYPE_ROOT, selected_item);
    }
    else if( (selected_item >= MMIWWW_Netfront_GetX509CertNumber(NFBROWSER_CERTLIST_TYPE_ROOT))
                &&(selected_item < MMIWWW_Netfront_GetX509CertNumber(NFBROWSER_CERTLIST_TYPE_CA) + MMIWWW_Netfront_GetX509CertNumber(NFBROWSER_CERTLIST_TYPE_ROOT)))
    {
        CertInfo_ptr = MMIWWW_Netfront_GetX509CertData(NFBROWSER_CERTLIST_TYPE_CA, selected_item - MMIWWW_Netfront_GetX509CertNumber(NFBROWSER_CERTLIST_TYPE_ROOT));
    }
    else
    {
        CertInfo_ptr = MMIWWW_Netfront_GetX509CertData(NFBROWSER_CERTLIST_TYPE_SERVER, selected_item - MMIWWW_Netfront_GetX509CertNumber(NFBROWSER_CERTLIST_TYPE_ROOT)
            - MMIWWW_Netfront_GetX509CertNumber(NFBROWSER_CERTLIST_TYPE_CA));
    }

    MMI_GetLabelTextByLang(TXT_WWW_OWNER_COUNTRY, &string_lab);

    SCI_MEMSET(item_text, 0, sizeof(item_text));
    item_data.text_data.buf.len = MMIAPICOM_CopyStr2Buf(item_text, &string_lab, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = item_text;

    GUIRICHTEXT_AddItem(MMK_GetCtrlHandleByWin(win_id, MMIWWW_RICHTEXT_CERTIFICATE_ATTRIBUTE_CTRL_ID), &item_data, &index); 

    SCI_MEMSET(item_text, 0, sizeof(item_text));
    if(PNULL != CertInfo_ptr->fSubject.fName_C)
    {
        str_len = (uint16)strlen((char *)CertInfo_ptr->fSubject.fName_C);
        GUI_UTF8ToWstr(item_text, str_len, CertInfo_ptr->fSubject.fName_C, str_len);
        item_data.text_data.buf.len = str_len;
        item_data.text_data.buf.str_ptr = item_text;

        GUIRICHTEXT_AddItem(MMK_GetCtrlHandleByWin(win_id, MMIWWW_RICHTEXT_CERTIFICATE_ATTRIBUTE_CTRL_ID), &item_data, &index); 
    }

    MMI_GetLabelTextByLang(TXT_WWW_OWNER_ORGANIZE, &string_lab);

    SCI_MEMSET(item_text, 0, sizeof(item_text));
    item_data.text_data.buf.len = MMIAPICOM_CopyStr2Buf(item_text, &string_lab, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = item_text;

    GUIRICHTEXT_AddItem(MMK_GetCtrlHandleByWin(win_id, MMIWWW_RICHTEXT_CERTIFICATE_ATTRIBUTE_CTRL_ID), &item_data, &index); 

    SCI_MEMSET(item_text, 0, sizeof(item_text));
    if(PNULL != CertInfo_ptr->fSubject.fName_O)
    {
        str_len = (uint16)strlen((char *)CertInfo_ptr->fSubject.fName_O);
        GUI_UTF8ToWstr(item_text, str_len, CertInfo_ptr->fSubject.fName_O, str_len);
        item_data.text_data.buf.len = str_len;
        item_data.text_data.buf.str_ptr = item_text;

        GUIRICHTEXT_AddItem(MMK_GetCtrlHandleByWin(win_id, MMIWWW_RICHTEXT_CERTIFICATE_ATTRIBUTE_CTRL_ID), &item_data, &index); 
    }

    MMI_GetLabelTextByLang(TXT_WWW_OWNER_UNIT, &string_lab);

    SCI_MEMSET(item_text, 0, sizeof(item_text));
    item_data.text_data.buf.len = MMIAPICOM_CopyStr2Buf(item_text, &string_lab, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = item_text;

    GUIRICHTEXT_AddItem(MMK_GetCtrlHandleByWin(win_id, MMIWWW_RICHTEXT_CERTIFICATE_ATTRIBUTE_CTRL_ID), &item_data, &index); 

    SCI_MEMSET(item_text, 0, sizeof(item_text));
    if(PNULL != CertInfo_ptr->fSubject.fName_OU)
    {
        str_len = (uint16)strlen((char *)CertInfo_ptr->fSubject.fName_OU);
        GUI_UTF8ToWstr(item_text, str_len, CertInfo_ptr->fSubject.fName_OU, str_len);
        item_data.text_data.buf.len = str_len;
        item_data.text_data.buf.str_ptr = item_text;

        GUIRICHTEXT_AddItem(MMK_GetCtrlHandleByWin(win_id, MMIWWW_RICHTEXT_CERTIFICATE_ATTRIBUTE_CTRL_ID), &item_data, &index); 
    }

    MMI_GetLabelTextByLang(TXT_WWW_OWNER_NAME, &string_lab);

    SCI_MEMSET(item_text, 0, sizeof(item_text));
    item_data.text_data.buf.len = MMIAPICOM_CopyStr2Buf(item_text, &string_lab, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = item_text;

    GUIRICHTEXT_AddItem(MMK_GetCtrlHandleByWin(win_id, MMIWWW_RICHTEXT_CERTIFICATE_ATTRIBUTE_CTRL_ID), &item_data, &index); 

    SCI_MEMSET(item_text, 0, sizeof(item_text));
    if(PNULL != CertInfo_ptr->fSubject.fName_CN)
    {
        str_len = (uint16)strlen((char *)CertInfo_ptr->fSubject.fName_CN);
        GUI_UTF8ToWstr(item_text, str_len, CertInfo_ptr->fSubject.fName_CN, str_len);
        item_data.text_data.buf.len = str_len;
        item_data.text_data.buf.str_ptr = item_text;

        GUIRICHTEXT_AddItem(MMK_GetCtrlHandleByWin(win_id, MMIWWW_RICHTEXT_CERTIFICATE_ATTRIBUTE_CTRL_ID), &item_data, &index); 
    }

    MMI_GetLabelTextByLang(TXT_WWW_ISSUER_COUNTRY, &string_lab);

    SCI_MEMSET(item_text, 0, sizeof(item_text));
    item_data.text_data.buf.len = MMIAPICOM_CopyStr2Buf(item_text, &string_lab, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = item_text;

    GUIRICHTEXT_AddItem(MMK_GetCtrlHandleByWin(win_id, MMIWWW_RICHTEXT_CERTIFICATE_ATTRIBUTE_CTRL_ID), &item_data, &index); 

    SCI_MEMSET(item_text, 0, sizeof(item_text));
    if(PNULL != CertInfo_ptr->fIssuer.fName_C)
    {
        str_len = (uint16)strlen((char *)CertInfo_ptr->fIssuer.fName_C);
        GUI_UTF8ToWstr(item_text, str_len, CertInfo_ptr->fIssuer.fName_C, str_len);
        item_data.text_data.buf.len = str_len;
        item_data.text_data.buf.str_ptr = item_text;

        GUIRICHTEXT_AddItem(MMK_GetCtrlHandleByWin(win_id, MMIWWW_RICHTEXT_CERTIFICATE_ATTRIBUTE_CTRL_ID), &item_data, &index); 
    }

    MMI_GetLabelTextByLang(TXT_WWW_ISSUER_ORGANIZE, &string_lab);

    SCI_MEMSET(item_text, 0, sizeof(item_text));
    item_data.text_data.buf.len = MMIAPICOM_CopyStr2Buf(item_text, &string_lab, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = item_text;

    GUIRICHTEXT_AddItem(MMK_GetCtrlHandleByWin(win_id, MMIWWW_RICHTEXT_CERTIFICATE_ATTRIBUTE_CTRL_ID), &item_data, &index); 

    SCI_MEMSET(item_text, 0, sizeof(item_text));
    if(PNULL != CertInfo_ptr->fIssuer.fName_O)
    {
        str_len = (uint16)strlen((char *)CertInfo_ptr->fIssuer.fName_O);
        GUI_UTF8ToWstr(item_text, str_len, CertInfo_ptr->fIssuer.fName_O, str_len);
        item_data.text_data.buf.len = str_len;
        item_data.text_data.buf.str_ptr = item_text;

        GUIRICHTEXT_AddItem(MMK_GetCtrlHandleByWin(win_id, MMIWWW_RICHTEXT_CERTIFICATE_ATTRIBUTE_CTRL_ID), &item_data, &index); 
    }

    MMI_GetLabelTextByLang(TXT_WWW_ISSUER_UNIT, &string_lab);

    SCI_MEMSET(item_text, 0, sizeof(item_text));
    item_data.text_data.buf.len = MMIAPICOM_CopyStr2Buf(item_text, &string_lab, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = item_text;

    GUIRICHTEXT_AddItem(MMK_GetCtrlHandleByWin(win_id, MMIWWW_RICHTEXT_CERTIFICATE_ATTRIBUTE_CTRL_ID), &item_data, &index); 

    SCI_MEMSET(item_text, 0, sizeof(item_text));
    if(PNULL != CertInfo_ptr->fIssuer.fName_OU)
    {
        str_len = (uint16)strlen((char *)CertInfo_ptr->fIssuer.fName_OU);
        GUI_UTF8ToWstr(item_text, str_len, CertInfo_ptr->fIssuer.fName_OU, str_len);
        item_data.text_data.buf.len = str_len;
        item_data.text_data.buf.str_ptr = item_text;

        GUIRICHTEXT_AddItem(MMK_GetCtrlHandleByWin(win_id, MMIWWW_RICHTEXT_CERTIFICATE_ATTRIBUTE_CTRL_ID), &item_data, &index); 
    }

    MMI_GetLabelTextByLang(TXT_WWW_ISSUER_NAME, &string_lab);

    SCI_MEMSET(item_text, 0, sizeof(item_text));
    item_data.text_data.buf.len = MMIAPICOM_CopyStr2Buf(item_text, &string_lab, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = item_text;

    GUIRICHTEXT_AddItem(MMK_GetCtrlHandleByWin(win_id, MMIWWW_RICHTEXT_CERTIFICATE_ATTRIBUTE_CTRL_ID), &item_data, &index); 

    SCI_MEMSET(item_text, 0, sizeof(item_text));
    if(PNULL != CertInfo_ptr->fIssuer.fName_CN)
    {
        str_len = (uint16)strlen((char *)CertInfo_ptr->fIssuer.fName_CN);
        GUI_UTF8ToWstr(item_text, str_len, CertInfo_ptr->fIssuer.fName_CN, str_len);
        item_data.text_data.buf.len = str_len;
        item_data.text_data.buf.str_ptr = item_text;

        GUIRICHTEXT_AddItem(MMK_GetCtrlHandleByWin(win_id, MMIWWW_RICHTEXT_CERTIFICATE_ATTRIBUTE_CTRL_ID), &item_data, &index); 
    }

    MMI_GetLabelTextByLang(TXT_WWW_VALID_BEGIN, &string_lab);

    SCI_MEMSET(item_text, 0, sizeof(item_text));
    item_data.text_data.buf.len = MMIAPICOM_CopyStr2Buf(item_text, &string_lab, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = item_text;

    GUIRICHTEXT_AddItem(MMK_GetCtrlHandleByWin(win_id, MMIWWW_RICHTEXT_CERTIFICATE_ATTRIBUTE_CTRL_ID), &item_data, &index); 

    SCI_MEMSET(data_str, 0, sizeof(data_str));
    SCI_MEMSET(item_text, 0, sizeof(item_text));
    MMIAPISET_FormatDateStrByDateStyle(CertInfo_ptr->fValidity_start.year, 
        CertInfo_ptr->fValidity_start.mon + 1, 
        CertInfo_ptr->fValidity_start.mday,
        '-',(uint8*)date,MMISET_DATE_STR_LEN);
    MMIAPISET_FormatTimeStrByTime(CertInfo_ptr->fValidity_start.hour, CertInfo_ptr->fValidity_start.min,(uint8*)time,MMISET_TIME_STR_12HOURS_LEN + 1);
    sprintf(data_str, "%s %s", date,time);
    MMI_STRNTOWSTR(item_text, strlen(data_str), (uint8 *)data_str, strlen(data_str), strlen(data_str));
    item_data.text_data.buf.len = (uint16)strlen(data_str);
    item_data.text_data.buf.str_ptr = item_text;

    GUIRICHTEXT_AddItem(MMK_GetCtrlHandleByWin(win_id, MMIWWW_RICHTEXT_CERTIFICATE_ATTRIBUTE_CTRL_ID), &item_data, &index); 

    MMI_GetLabelTextByLang(TXT_WWW_VALID_END, &string_lab);

    SCI_MEMSET(item_text, 0, sizeof(item_text));
    item_data.text_data.buf.len = MMIAPICOM_CopyStr2Buf(item_text, &string_lab, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = item_text;

    GUIRICHTEXT_AddItem(MMK_GetCtrlHandleByWin(win_id, MMIWWW_RICHTEXT_CERTIFICATE_ATTRIBUTE_CTRL_ID), &item_data, &index); 

    SCI_MEMSET(data_str, 0, sizeof(data_str));
    SCI_MEMSET(item_text, 0, sizeof(item_text));
    MMIAPISET_FormatDateStrByDateStyle(CertInfo_ptr->fValidity_end.year, 
        CertInfo_ptr->fValidity_end.mon + 1, 
        CertInfo_ptr->fValidity_end.mday,
        '-',(uint8*)date,MMISET_DATE_STR_LEN);
    MMIAPISET_FormatTimeStrByTime(CertInfo_ptr->fValidity_end.hour, CertInfo_ptr->fValidity_end.min,(uint8*)time,MMISET_TIME_STR_12HOURS_LEN + 1);
    sprintf(data_str, "%s %s", date,time);

    MMI_STRNTOWSTR(item_text, strlen(data_str), (uint8 *)data_str, strlen(data_str), strlen(data_str));
    item_data.text_data.buf.len = (uint16)strlen(data_str);
    item_data.text_data.buf.str_ptr = item_text;

    GUIRICHTEXT_AddItem(MMK_GetCtrlHandleByWin(win_id, MMIWWW_RICHTEXT_CERTIFICATE_ATTRIBUTE_CTRL_ID), &item_data, &index); 

    MMI_GetLabelTextByLang(TXT_WWW_SERIAL_NUM, &string_lab);

    SCI_MEMSET(item_text, 0, sizeof(item_text));
    item_data.text_data.buf.len = MMIAPICOM_CopyStr2Buf(item_text, &string_lab, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = item_text;

    GUIRICHTEXT_AddItem(MMK_GetCtrlHandleByWin(win_id, MMIWWW_RICHTEXT_CERTIFICATE_ATTRIBUTE_CTRL_ID), &item_data, &index); 

    SCI_MEMSET(item_text, 0, sizeof(item_text));
    if(PNULL != CertInfo_ptr->fSerialnumber)
    {
        str_len = (uint16)strlen((char *)CertInfo_ptr->fSerialnumber);
        GUI_UTF8ToWstr(item_text, str_len, CertInfo_ptr->fSerialnumber, str_len);
        item_data.text_data.buf.len = str_len;
        item_data.text_data.buf.str_ptr = item_text;

        GUIRICHTEXT_AddItem(MMK_GetCtrlHandleByWin(win_id, MMIWWW_RICHTEXT_CERTIFICATE_ATTRIBUTE_CTRL_ID), &item_data, &index); 
    }

    MMIWWW_Netfront_DeleteX509CertData(CertInfo_ptr);
}

/*****************************************************************************/
//  Description : handle message of www match key win
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWMatchKeyWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    uint16 cur_index = 0; 
    uint16 match_num = 0;
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    wchar *key_name_ptr = PNULL;
    GUILIST_ITEM_T *cur_list_ptr = PNULL;
    MMI_STRING_T list_info_str = {0};    
    MMI_HANDLE_T applet_handle = PNULL;
    MMI_HANDLE_T ctrl_handle = PNULL;

    //SCI_TRACE_LOW:"mmiwww_wintab.c:HandleWWWMatchKeyWinMsg() msg_id=0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_7988_112_2_18_3_12_17_209,(uint8*)"d", msg_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_LISTBOX_MATCHKEY_CTRL_ID);
        key_name_ptr = (wchar *)MMK_GetWinAddDataPtr(win_id);
        match_num = MMIWWW_ListMatchKeyName(key_name_ptr,  ctrl_handle);
        GUILIST_SetBgImage(ctrl_handle, 0, FALSE);
        GUILIST_SetBgColor(ctrl_handle, MMI_DARK_GRAY_COLOR);
        MMK_SetAtvCtrl(win_id, ctrl_handle);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        applet_handle = MMIWWW_HANDLE((WWW_APPLET_T *)MMK_GetAppletPtrByWin(win_id));
        ctrl_handle = MMK_GetCtrlHandleByWin(MMK_GetWinHandle( applet_handle,MMIWWW_MAINMENU_WIN_ID), MMIWWW_MAINMENU_LABEL_SEARCH_CTRL_ID);
        cur_index = GUILIST_GetCurItemIndex(MMK_GetCtrlHandleByWin(win_id, MMIWWW_LISTBOX_MATCHKEY_CTRL_ID));
        cur_list_ptr = (GUILIST_ITEM_T *)GUILIST_GetItemPtrByIndex(MMK_GetCtrlHandleByWin(win_id, MMIWWW_LISTBOX_MATCHKEY_CTRL_ID), cur_index);

        if (PNULL == cur_list_ptr)
        {
            MMK_CloseWin(win_id);

            break;
        }

        list_info_str.wstr_len = cur_list_ptr->item_data_ptr->item_content[0].item_data.text_buffer.wstr_len;
        list_info_str.wstr_ptr = cur_list_ptr->item_data_ptr->item_content[0].item_data.text_buffer.wstr_ptr;

        GUILABEL_SetText(ctrl_handle, &list_info_str, FALSE);
        MMIWWW_NetfrontSetKeywordChange(TRUE);
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        key_name_ptr = (wchar *)MMK_GetWinAddDataPtr(win_id);
        if (PNULL != key_name_ptr)
        {
            SCI_FREE(key_name_ptr);
        }
        MMIWWW_NetfrontSetMainMenuFocusState(TRUE);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : handle message of www match URL win
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWMatchURLWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    uint16 cur_index = 0;
    uint16 match_num = 0;
    wchar *url_ptr = PNULL;
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    GUILIST_ITEM_T *cur_list_ptr = PNULL;
    MMI_STRING_T list_info_str = {0};    
    MMI_HANDLE_T applet_handle = PNULL;
    MMI_HANDLE_T ctrl_handle = PNULL;

    //SCI_TRACE_LOW:"mmiwww_wintab.c:HandleWWWMatchURLWinMsg() msg_id=0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_8064_112_2_18_3_12_17_210,(uint8*)"d", msg_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        url_ptr = (wchar *)MMK_GetWinAddDataPtr(win_id);
        ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_LISTBOX_MATCHURL_CTRL_ID);
        match_num = MMIWWW_ListMatchURLName(url_ptr, ctrl_handle);
        GUILIST_SetBgImage(ctrl_handle, 0, FALSE);
        GUILIST_SetBgColor(ctrl_handle, MMI_DARK_GRAY_COLOR);
        MMK_SetAtvCtrl(win_id, ctrl_handle);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        applet_handle = MMIWWW_HANDLE((WWW_APPLET_T *)MMK_GetAppletPtrByWin(win_id));
        ctrl_handle = MMK_GetCtrlHandleByWin(MMK_GetWinHandle( applet_handle,MMIWWW_MAINMENU_WIN_ID), MMIWWW_MAINMENU_LABEL_ADDRESS_CTRL_ID);
        cur_index = GUILIST_GetCurItemIndex(MMK_GetCtrlHandleByWin(win_id, MMIWWW_LISTBOX_MATCHURL_CTRL_ID));
        cur_list_ptr = (GUILIST_ITEM_T *)GUILIST_GetItemPtrByIndex(MMK_GetCtrlHandleByWin(win_id, MMIWWW_LISTBOX_MATCHURL_CTRL_ID), cur_index);

        if (PNULL == cur_list_ptr)
        {
            MMK_CloseWin(win_id);

            break;
        }

        list_info_str.wstr_len = cur_list_ptr->item_data_ptr->item_content[0].item_data.text_buffer.wstr_len;
        list_info_str.wstr_ptr = cur_list_ptr->item_data_ptr->item_content[0].item_data.text_buffer.wstr_ptr;
        GUILABEL_SetText(ctrl_handle, &list_info_str, FALSE);
        MMIWWW_NetfrontSetUrlChange(TRUE);
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        url_ptr = (wchar *)MMK_GetWinAddDataPtr(win_id);
        if (PNULL != url_ptr)
        {
            SCI_FREE(url_ptr);
        }
        MMIWWW_NetfrontSetMainMenuFocusState(TRUE);
//        MMIWWW_Netfront_DeleteInputURLHistory();
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : handle message of www history item attribute win
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHisAttributeDetailWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    uint16 selected_item = 0;
    MMI_HANDLE_T list_ctrl_handle = PNULL;
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_HANDLE_T applet_handle = MMIWWW_HANDLE((WWW_APPLET_T *)MMK_GetAppletPtrByWin(win_id));
    MMIWWW_PAGEMEMO_ATTRIBUTE_TYPE_E type = (uint32)MMK_GetWinAddDataPtr(win_id);/*lint !e64*/

    //SCI_TRACE_LOW:"mmiwww_wintab.c:HandleWWWMatchURLWinMsg() msg_id=0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_8137_112_2_18_3_12_17_211,(uint8*)"d", msg_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            MMIWWW_Append_History_Attribute(win_id, type);
            MMK_SetAtvCtrl(win_id, MMK_GetCtrlHandleByWin(win_id, MMIWWW_RICHTEXT_HISTORY_ATTRIBUTE_CTRL_ID));
        }
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        if( type == MMIWWW_PAGE_ATTRIBUTE_HISTORY )
        {
            list_ctrl_handle = MMK_GetCtrlHandleByWin( MMK_GetWinHandle( applet_handle, MMIWWW_HISTORY_WIN_ID ), MMIWWW_LISTBOX_HISTORY_CTRL_ID );
            selected_item = GUILIST_GetCurItemIndex(list_ctrl_handle);
            MMIWWW_Netfront_History_GotoURL(selected_item);
            MMK_CloseWin(win_id);
        }
        else if( type == MMIWWW_PAGE_ATTRIBUTE_PAGEMEMO )
        {
            MMI_HANDLE_T win_handle = PNULL;
            MMI_HANDLE_T page_memo_handle = PNULL;

            win_handle = mmiwww_GetBrowserWinhandle();
            page_memo_handle = MMK_GetWinHandle( applet_handle,MMIWWW_PAGEMEMO_WIN_ID);
            list_ctrl_handle = MMK_GetCtrlHandleByWin(page_memo_handle, MMIWWW_LISTBOX_PAGEMEMO_CTRL_ID);
            selected_item = GUILIST_GetCurItemIndex(list_ctrl_handle);
            
            if( !MMK_IsOpenWin(mmiwww_GetBrowserWinhandle()) )
            {
                MMIWWW_OpenWWWBrowserWin(BROWSER_START_NORMAL, PNULL);
                if (!MMIWWW_IsFirstOpen()) /* Open pagememo here else open pagememo in mmiwww_HandleNetAccessPagememo*/
                {
                    MMK_CloseWin(win_id);
                    if (MMK_IsOpenWin(page_memo_handle))
                    {
                        MMK_CloseWin(page_memo_handle);
                    }
                    if (!MMK_IsFocusWin(win_handle))
                    {
                        MMK_WinGrabFocus(win_handle);
                    }
                    MMIWWW_Netfront_LoadPageMemo(selected_item);
                    MMIWWW_SetWWWState(MMIWWW_Netfront_GetActWinIndex(), MMIWWW_STATE_PAGE_LOADING);
                }
            }  
            else
            {
                MMK_CloseWin(win_id);
                if (MMK_IsOpenWin(page_memo_handle))
                {
                    MMK_CloseWin(page_memo_handle);
                }
                if (!MMK_IsFocusWin(win_handle))
                {
                    MMK_WinGrabFocus(win_handle);
                }
                MMIWWW_Netfront_LoadPageMemo(selected_item);
                MMIWWW_SetWWWState(MMIWWW_Netfront_GetActWinIndex(), MMIWWW_STATE_PAGE_LOADING);
            }
        }    

        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : handle message of www history item attribute win
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCerAttributeDetailWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"mmiwww_wintab.c:HandleCerAttributeDetailWinMsg() msg_id=0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_8227_112_2_18_3_12_17_212,(uint8*)"d", msg_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIWWW_Append_Certificate_Attribute(win_id);
        MMK_SetAtvCtrl(win_id, MMK_GetCtrlHandleByWin(win_id, MMIWWW_RICHTEXT_CERTIFICATE_ATTRIBUTE_CTRL_ID));
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : handle message of www set  win
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWSetWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    uint16 item_set = 0;
    uint16 selected_item = 0;
    uint16 org_item = 0;
    MMI_RESULT_E recode = MMI_RESULT_TRUE;   
    WWW_NV_ITEM_E item_nv = MMINV_WWW_FONT_STYLE;
    MMIWWW_ID_MENU_E menu_id = ID_WWW_MENU_START;

    //SCI_TRACE_LOW:"mmiwww_wintab.c:HandleWWWSetWinMsg() msg_id=0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_8269_112_2_18_3_12_17_213,(uint8*)"d", msg_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        menu_id = (uint32)MMK_GetWinAddDataPtr(win_id);/*lint !e64*/

        switch(menu_id)
        {
        case ID_WWW_SETMENU_VIEWMODE:
            {
                MMI_TEXT_ID_T ViewMode_arr[] = {TXT_WWW_FULL_VIEW,TXT_WWW_TEXT_VIEW,TXT_WWW_SMART_VIEW/*TXT_WWW_FAST_VIEW*/};

                GUIWIN_SetTitleTextId(win_id, (MMI_TEXT_ID_T)TXT_WWW_SET_VIEW, FALSE);
                GUILIST_SetMaxItem( MMK_GetCtrlHandleByWin(win_id, MMIWWW_LISTBOX_SET_CTRL_ID), 3, FALSE );
                item_set = MMIWWW_GetViewMode();
                if (SETTINGS_VIEW_ORIGINAL == item_set)
                {
                    selected_item = 0;
                }
                else if (SETTINGS_VIEW_TEXT == item_set)
                {
                    selected_item = 1;
                }
                else
                {
                    selected_item = 2;
                }
                MMIWWW_AppendListItem( win_id, MMK_GetCtrlHandleByWin(win_id, MMIWWW_LISTBOX_SET_CTRL_ID), &ViewMode_arr[0], 3, selected_item );
                MMK_SetAtvCtrl( win_id, MMK_GetCtrlHandleByWin(win_id, MMIWWW_LISTBOX_SET_CTRL_ID) );
            }
            break;

        case ID_WWW_SETMENU_DISPALAYOPT:
            {
                MMI_HANDLE_T script_list_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_SCRIPT_DROPLIST_CTRL_ID);

                GUIWIN_SetTitleTextId(win_id, (MMI_TEXT_ID_T)TXT_DISPLAY_OPTION, FALSE);
                SetWWWDisplaySetting(win_id);
                MMK_SetAtvCtrl(win_id, script_list_ctrl);
            }
            break;

        case ID_WWW_SETMENU_SETCOOKIE:
            {
                MMI_HANDLE_T cookie_mode_list_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_COOKIE_MODE_DROPLIST_CTRL_ID);

                GUIWIN_SetTitleTextId(win_id, (MMI_TEXT_ID_T)TXT_COOKIE, FALSE);
                SetWWWCookieSetting(win_id);
                MMK_SetAtvCtrl(win_id, cookie_mode_list_ctrl);
            }
            break;

        case ID_WWW_SETMENU_SETCACHE:
            {
                MMI_HANDLE_T cache_mode_list_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_CACHE_MODE_DROPLIST_CTRL_ID);

                GUIWIN_SetTitleTextId(win_id, (MMI_TEXT_ID_T)TXT_WWW_SET_CACHE, FALSE);
                SetWWWCacheSetting(win_id);
                MMK_SetAtvCtrl(win_id, cache_mode_list_ctrl);
            }
            break;

        case ID_WWW_SETMENU_WEBDATA:
            {
                MMI_HANDLE_T webdata_mode_list_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_WEBDATA_DROPLIST_CTRL_ID);

                GUIWIN_SetTitleTextId(win_id, (MMI_TEXT_ID_T)TXT_WWW_WEBDATA, FALSE);
                SetWWWWebdataSetting(win_id);
                MMK_SetAtvCtrl(win_id, webdata_mode_list_ctrl);
            }
            break;
            
        case ID_WWW_SETMENU_SETFONT:     
            {
                MMI_TEXT_ID_T FontType_arr[] = {TXT_COMM_FONT_BIG, TXT_COMMON_NORMAL, TXT_WWW_FONT_SMALL};

                GUIWIN_SetTitleTextId(win_id, (MMI_TEXT_ID_T)TXT_COMMON_FONT_SIZE, FALSE);
                GUILIST_SetMaxItem( MMK_GetCtrlHandleByWin(win_id, MMIWWW_LISTBOX_SET_CTRL_ID), MMIWWW_FONT_TYPE_MAX, FALSE );
                item_set = MMIWWW_GetFontStyle();
                MMIWWW_AppendListItem( win_id, MMK_GetCtrlHandleByWin(win_id, MMIWWW_LISTBOX_SET_CTRL_ID), &FontType_arr[0], (uint16)MMIWWW_FONT_TYPE_MAX, item_set );
                MMK_SetAtvCtrl( win_id, MMK_GetCtrlHandleByWin(win_id, MMIWWW_LISTBOX_SET_CTRL_ID) );
            }
            break;

        default:
            break;
        }     
        break;

    case MSG_FULL_PAINT:
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
         {
            MMI_HANDLE_T clear_cookie_lable_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_CLEAR_COOKIE_LABLE_CTRL_ID);
            MMI_HANDLE_T clear_cache_lable_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_CLEAR_CACHE_LABLE_CTRL_ID);
            MMI_HANDLE_T clear_webdata_lable_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_CLEAR_WEBDATA_LABLE_CTRL_ID);
            MMI_HANDLE_T active_ctrl = MMK_GetActiveCtrl(win_id);
            MMI_TEXT_ID_T text_id = TXT_NULL;
            
            menu_id = (uint32)MMK_GetWinAddDataPtr(win_id);/*lint !e64*/

            if (ID_WWW_SETMENU_VIEWMODE == menu_id)
            {

                    slim_int view_mode[] = {SETTINGS_VIEW_ORIGINAL,SETTINGS_VIEW_TEXT,SETTINGS_VIEW_SMARTFIT};
                    selected_item = GUILIST_GetCurItemIndex(MMK_GetCtrlHandleByWin(win_id, MMIWWW_LISTBOX_SET_CTRL_ID));
                    
                    item_nv = MMINV_WWW_VIEW_MODE;
                    org_item = MMIWWW_GetViewMode();
                    if (org_item != selected_item)
                    {
                        MMIWWW_Netfront_ViewMode(view_mode[selected_item]);
                        MMIWWW_SetBrowserReloadFlag(MMIWWW_BROWSER_MAX_TAB, TRUE);
                        if ( !GUILIST_IsCurrItemInvalid( MMK_GetCtrlHandleByWin(win_id, MMIWWW_LISTBOX_SET_CTRL_ID) ) )
                        {
                            MMINV_WRITE( item_nv, &selected_item );
                        }
                    }
                    MMIPUB_OpenAlertSuccessWin(TXT_SET_SUCCESS);
                    MMK_CloseWin( win_id );
               }
            else
            {
                if (clear_cookie_lable_ctrl == active_ctrl)
                {
                    text_id = TXT_WWW_IS_CLEAR_COOKIES;
                }
                else if (clear_cache_lable_ctrl == active_ctrl)
                {
                    text_id = TXT_WWW_IS_CLEAR_CACHE;
                }
                else if (clear_webdata_lable_ctrl == active_ctrl)
                {
                    text_id = TXT_WWW_CLEAR_WEBDATA;
                }
                else
                {
                    text_id = TXT_NULL;
                }

                if (TXT_NULL != text_id)
                {
                    MMIPUB_OpenQueryWinByTextId(text_id, IMAGE_PUBWIN_QUERY, PNULL, PNULL);
                }
            }
         }
         break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
        {
            menu_id = (uint32)MMK_GetWinAddDataPtr(win_id);/*lint !e64*/

            switch(menu_id)
            {
            case ID_WWW_SETMENU_VIEWMODE:
                {
                    slim_int view_mode[] = {SETTINGS_VIEW_ORIGINAL,SETTINGS_VIEW_TEXT,SETTINGS_VIEW_SMARTFIT};
                    selected_item = GUILIST_GetCurItemIndex(MMK_GetCtrlHandleByWin(win_id, MMIWWW_LISTBOX_SET_CTRL_ID));
                    
                    item_nv = MMINV_WWW_VIEW_MODE;
                    org_item = MMIWWW_GetViewMode();
                    if (org_item != selected_item)
                    {
                        MMIWWW_Netfront_ViewMode(view_mode[selected_item]);
                        MMIWWW_SetBrowserReloadFlag(MMIWWW_BROWSER_MAX_TAB, TRUE);
                        if ( !GUILIST_IsCurrItemInvalid( MMK_GetCtrlHandleByWin(win_id, MMIWWW_LISTBOX_SET_CTRL_ID) ) )
                        {
                            MMINV_WRITE( item_nv, &selected_item );
                        }
                    }
                    MMIPUB_OpenAlertSuccessWin(TXT_SET_SUCCESS);
                    MMK_CloseWin( win_id );
                }
                break;

            case ID_WWW_SETMENU_DISPALAYOPT:
                {
                    uint16 cur_index = 0;
                    slim_int image_mode[] = {SETTINGS_IMAGES_DISPLAY, SETTINGS_IMAGES_NOT_DISPLAY};
                    slim_int script_mode[] = {SETTINGS_JAVASCRIPT, SETTINGS_ECMASCRIPT_MP, SETTINGS_SCRIPT_DISABLED};
                    MMI_HANDLE_T script_list_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_SCRIPT_DROPLIST_CTRL_ID);
                    MMI_HANDLE_T animal_list_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_ANIMAL_DROPLIST_CTRL_ID);
                    MMI_HANDLE_T image_list_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_IMAGE_DROPLIST_CTRL_ID);
                    MMI_HANDLE_T table_list_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_TABLE_DROPLIST_CTRL_ID);
                    MMI_HANDLE_T sound_list_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_SOUND_DROPLIST_CTRL_ID);
//                    MMI_HANDLE_T popup_list_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_POPUP_DROPLIST_CTRL_ID);

                    cur_index = GUIDROPDOWNLIST_GetCurItemIndex(script_list_ctrl);
                    MMIWWW_Netfront_SetScriptMode(script_mode[cur_index]);
                    MMINV_WRITE( MMINV_WWW_SCRIPT_MODE, &cur_index );

                    cur_index = GUIDROPDOWNLIST_GetCurItemIndex(animal_list_ctrl);
                    MMIWWW_Netfront_SetDisplayAnimateImage((slim_bool)(0 == cur_index));
                    MMINV_WRITE( MMINV_WWW_ANIMATION_MODE, &cur_index );

                    cur_index = GUIDROPDOWNLIST_GetCurItemIndex(image_list_ctrl);
                    MMIWWW_Netfront_SetImageDisplayMode(image_mode[cur_index]);
                    MMINV_WRITE( MMINV_WWW_PHOTO_MODE, &cur_index );

                    cur_index = GUIDROPDOWNLIST_GetCurItemIndex(table_list_ctrl);
                    MMIWWW_Netfront_SetDisplayTable((slim_bool)(0 == cur_index));
                    MMINV_WRITE( MMINV_WWW_TABLE_MODE, &cur_index );

                    cur_index = GUIDROPDOWNLIST_GetCurItemIndex(sound_list_ctrl);
                    MMIWWW_Netfront_SetEnableBGSound((slim_bool)(0 == cur_index));
                    MMINV_WRITE( MMINV_WWW_BGMUSIC_MODE, &cur_index );

//                    cur_index = GUIDROPDOWNLIST_GetCurItemIndex(popup_list_ctrl);
//                    MMINV_WRITE( MMINV_WWW_POPUPWIN_MODE, &cur_index );

                    MMIPUB_OpenAlertSuccessWin(TXT_SET_SUCCESS);
                    MMK_CloseWin( win_id );
                }
                break;

            case ID_WWW_SETMENU_SETCOOKIE:
                {
                    uint16 cur_index = 0;
#ifdef MMIWWW_SETTINGS_COOKIE_USE_QUERY
                    slim_int cookie_mode[] = {SETTINGS_COOKIE_MODE_ACCEPT_ALL, SETTINGS_COOKIE_MODE_REJECT_ALL, SETTINGS_COOKIE_MODE_PROMPT};
#else
                    slim_int cookie_mode[] = {SETTINGS_COOKIE_MODE_ACCEPT_ALL, SETTINGS_COOKIE_MODE_REJECT_ALL};
#endif
                    MMI_HANDLE_T cookie_mode_list_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_COOKIE_MODE_DROPLIST_CTRL_ID);

                    cur_index = GUIDROPDOWNLIST_GetCurItemIndex(cookie_mode_list_ctrl);
                    MMIWWW_Netfront_SetCookieMode(cookie_mode[cur_index]);
                    MMINV_WRITE( MMINV_WWW_COOKIE_MODE, &cur_index );

                    MMIPUB_OpenAlertSuccessWin(TXT_SET_SUCCESS);
                    MMK_CloseWin( win_id );
                }
                break;
                
            case ID_WWW_SETMENU_SETCACHE:
                {
                    uint16 cur_index = 0;
                    slim_int cache_mode[] = {SETTINGS_CACHE_MODE_NO_CACHE, SETTINGS_CACHE_MODE_NET_FIRST, SETTINGS_CACHE_MODE_CACHE_FIRST,
                        SETTINGS_CACHE_MODE_CACHE_ONLY};
                    MMI_HANDLE_T cache_mode_list_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_CACHE_MODE_DROPLIST_CTRL_ID);
                    MMI_HANDLE_T query_disk_list_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_QUERY_DISK_DROPLIST_CTRL_ID);
                    cur_index = GUIDROPDOWNLIST_GetCurItemIndex(cache_mode_list_ctrl);
                    MMIWWW_Netfront_SetCacheMode(cache_mode[cur_index]);
                    MMINV_WRITE( MMINV_WWW_CACHE_MODE, &cur_index );
                   
                    cur_index = GUIDROPDOWNLIST_GetCurItemIndex(query_disk_list_ctrl);
                    MMIWWW_Netfront_SetUseDiskCacheMode(cur_index);
                    MMINV_WRITE( MMINV_WWW_DISKCACHE_MODE, &cur_index );

                    MMIPUB_OpenAlertSuccessWin(TXT_SET_SUCCESS);
                    MMK_CloseWin( win_id );
                }
                break;

            case ID_WWW_SETMENU_WEBDATA:
                {
                    uint16 cur_index = 0;
                    MMI_HANDLE_T webdata_mode_list_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_WEBDATA_DROPLIST_CTRL_ID);
                    cur_index = GUIDROPDOWNLIST_GetCurItemIndex(webdata_mode_list_ctrl);
                    MMIWWW_Netfront_SetWebData((slim_bool)(0 == cur_index));
                    MMINV_WRITE( MMINV_WWW_WEBDATA_MODE, &cur_index );

                    MMIPUB_OpenAlertSuccessWin(TXT_SET_SUCCESS);
                    MMK_CloseWin( win_id );
                }
                break;         
                
            case ID_WWW_SETMENU_SETFONT:  
                selected_item = GUILIST_GetCurItemIndex(MMK_GetCtrlHandleByWin(win_id, MMIWWW_LISTBOX_SET_CTRL_ID));
                item_nv = MMINV_WWW_FONT_STYLE;
                org_item = MMIWWW_GetFontStyle();
                if (org_item != selected_item)
                {
                    MMIWWW_SetFontSetting(selected_item);
                    MMIWWW_SetBrowserReloadFlag(MMIWWW_BROWSER_MAX_TAB, TRUE);
                    if ( !GUILIST_IsCurrItemInvalid( MMK_GetCtrlHandleByWin(win_id, MMIWWW_LISTBOX_SET_CTRL_ID) ) )
                    {
                        MMINV_WRITE(item_nv, &selected_item);
                    }
                }
                MMIPUB_OpenAlertSuccessWin(TXT_SET_SUCCESS);
                MMK_CloseWin( win_id );
                break;

            default:
                break;
            }
        }
        break;

    case MSG_WWW_DELETE_SYNC_MSG:
        {
            MMI_HANDLE_T active_ctrl = 0;
            MMI_HANDLE_T clear_cookie_lable_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_CLEAR_COOKIE_LABLE_CTRL_ID);
            MMI_HANDLE_T clear_cache_lable_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_CLEAR_CACHE_LABLE_CTRL_ID);
            MMI_HANDLE_T clear_webdata_lable_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_CLEAR_WEBDATA_LABLE_CTRL_ID); 

            MMIPUB_CloseAlertWin();

            active_ctrl = MMK_GetActiveCtrl(win_id);
            if( clear_cookie_lable_ctrl == active_ctrl )
            {
                MMIWWW_Netfront_ClearCookie();
                MMIPUB_OpenAlertSuccessWin(TXT_WWW_CLEAR_COOKIES_SUCCESS);
            }
            else if( clear_cache_lable_ctrl == active_ctrl)
            {
                MMIWWW_Netfront_ClearCache();
                MMIPUB_OpenAlertSuccessWin(TXT_WWW_CLEAR_CACHE_SUCCESS);
            }
            else if( clear_webdata_lable_ctrl == active_ctrl )
            {
                MMIWWW_Netfront_ClearWebData();
                MMIPUB_OpenAlertSuccessWin(TXT_COMMON_DELETESUCESS);
            }
        }
        break;

    case MSG_PROMPTWIN_OK:
        {
            MMIPUB_CloseQuerytWin(PNULL);
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMMON_WAITING,TXT_NULL,IMAGE_PUBWIN_WAIT,PNULL,PNULL,MMIPUB_SOFTKEY_NONE,PNULL);
            MMK_PostMsg(win_id, MSG_WWW_DELETE_SYNC_MSG, PNULL, 0);
        }
        break;

    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
        break;
        
    case MSG_CLOSE_WINDOW:
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : handle message of  Inline IME
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWInlineIMEWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
   
    uint32   data_info = (uint32 )MMK_GetWinAddDataPtr(win_id);

    //SCI_TRACE_LOW:"mmiwww_wintab.c:HandleWWWInlineIMEWinMsg() msg_id=0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_8630_112_2_18_3_12_18_214,(uint8*)"d", msg_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        if(data_info == (uint32)GUIEDIT_TYPE_PASSWORD)
        {
            GUI_BOTH_RECT_T edit_both_rect = {0};
            edit_both_rect = MMIAPISET_GetPwEditRect(win_id);
            GUIAPICTRL_SetBothRect(MMIWWW_INLINE_IME_CTRL_ID, &edit_both_rect);
            //set left softkey
            GUIEDIT_SetSoftkey(MMIWWW_INLINE_IME_CTRL_ID,0,1,TXT_NULL,TXT_COMMON_OK,PNULL);
        }
        MMK_SetActiveCtrl(MMK_GetCtrlHandleByWin(win_id, MMIWWW_INLINE_IME_CTRL_ID), FALSE);
        break;

    case MSG_FULL_PAINT:
        if(data_info == (uint32)GUIEDIT_TYPE_PASSWORD)
        {
            MMIAPISET_SetPwBg(win_id, IMAGE_NUMBERINPUT_PIN);
        }
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        {
            MMIWWW_INSTANCE_T *self = MMIWWW_INSTANCE();

            if (PNULL != self)
            {
                MMIWWW_Netfront_LeaveTextControlEditMode(self->ime_endparam.ime_result_text);
            }

            MMK_CloseWin(win_id);
        }
        break;

    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
        {
            MMIWWW_INSTANCE_T *self = MMIWWW_INSTANCE();
            MMI_STRING_T string_info = {0};

            GUIEDIT_GetString(MMK_GetCtrlHandleByWin(win_id, MMIWWW_INLINE_IME_CTRL_ID), &string_info);

            if (PNULL == self)
            {
                break;
            }

            if (PNULL != self->ime_endparam.ime_result_text)
            {
                SCI_FREE(self->ime_endparam.ime_result_text);
                self->ime_endparam.ime_result_text = PNULL;
            }

            if (0 != string_info.wstr_len)
            {
                self->ime_endparam.ime_result_bytes = string_info.wstr_len * 3 + 1;
                self->ime_endparam.ime_result_text = SCI_ALLOCA(self->ime_endparam.ime_result_bytes);

                if (PNULL == self->ime_endparam.ime_result_text)
                {
                    MMIWWW_Netfront_LeaveTextControlEditMode(self->ime_endparam.ime_result_text);
                    break;
                }
                
                SCI_MEMSET(self->ime_endparam.ime_result_text, 0x00, self->ime_endparam.ime_result_bytes);
                
                self->ime_endparam.ime_result_bytes = (slim_int)GUI_WstrToUTF8(self->ime_endparam.ime_result_text,
                    (uint16)self->ime_endparam.ime_result_bytes,
                                                                                                    string_info.wstr_ptr,
                                                                                                    string_info.wstr_len);

                MMIWWW_Netfront_LeaveTextControlEditMode(self->ime_endparam.ime_result_text);
            }
            else
            {
                MMIWWW_Netfront_LeaveTextControlEditMode(self->ime_endparam.ime_result_text);
            }

            MMK_CloseWin(win_id);
        }
        break;

    case MSG_CLOSE_WINDOW:
        {
            MMIWWW_INSTANCE_T *self = MMIWWW_INSTANCE();

            if (PNULL != self)
            {
                self->ime_win_id = PNULL;
            }
        }
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : creat softkey control
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL void CreatSoftkeyCtrl(
                           MMI_HANDLE_T win_handle,     //win id
                           MMI_TEXT_ID_T leftsoft_id,
                           MMI_TEXT_ID_T midsoft_id,
                           MMI_TEXT_ID_T rightsoft_id
                           )
{
#ifndef MMI_PDA_SUPPORT	//pda大多数场景不显示softkey
    GUISOFTKEY_INIT_DATA_T    softkey_data = {0};
    MMI_CONTROL_CREATE_T softkey_create = {0};
    MMI_CTRL_ID_T    softkey_ctrl_id = 0;

    softkey_data.leftsoft_id = TXT_COMMON_OK;
    softkey_data.midsoft_id    = TXT_NULL;
    softkey_data.rightsoft_id = STXT_RETURN;

    softkey_ctrl_id = MMITHEME_GetSoftkeyCtrlId();
    softkey_create.ctrl_id = softkey_ctrl_id;
    softkey_create.guid    = SPRD_GUI_SOFTKEY_ID;
    softkey_create.parent_win_handle = win_handle;
    softkey_create.init_data_ptr = &softkey_data;

    MMK_CreateControl( &softkey_create );
    MMK_SetWinSoftkeyCtrlId(win_handle, softkey_ctrl_id);
#endif
}

/****************************************************************************/    
//  Description :add item to richtext
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void MMIWWW_RichText_AddItem(MMI_CTRL_ID_T ctrl_id, MMI_STRING_T * item_string, GUIRICHTEXT_FRAME_TYPE_E frame_type, GUIRICHTEXT_TAG_TYPE_E tag_type)
{
    GUIRICHTEXT_ITEM_T  item_data = {0};/*lint !e64*/
    uint16 index = 0;

    if (PNULL == item_string)
    {
        return;
    }

    item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;
    item_data.text_data.buf.len = item_string->wstr_len;
    item_data.text_data.buf.str_ptr = item_string->wstr_ptr;
    item_data.frame_type = frame_type;
    item_data.tag_type = tag_type;
    
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
}

/****************************************************************************/    
//  Description : Set tab Title
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void MMIWWW_SetTabTitle(MMI_CTRL_ID_T ctrl_id, DPARAM param)
{
    GUITAB_TEXT_T item_text = {0};
    MMI_STRING_T string = {0};
    wchar str_temp[4 + 1] = {0};
    uint8 num_str[4 + 1] = {0};
    uint16 num_len = 0;

    /* Bookmark(XX) */
    MMIRES_GetText(TXT_BOOKMARK, ctrl_id, &string);
    if (string.wstr_len > GUITAB_MAX_ITEM_NAME_LENGTH - 4)
    {
        string.wstr_len = GUITAB_MAX_ITEM_NAME_LENGTH - 4;
    }
    SCI_MEMSET(num_str, 0x0, (4 + 1));
    SCI_MEMCPY(item_text.wstr, string.wstr_ptr, string.wstr_len * sizeof(wchar));
    num_len = (uint16)sprintf((char *)num_str, "(%d)", MMIWWW_GetBookmarkTotalNumber());
    MMI_STRNTOWSTR(str_temp, 4, (uint8 *)num_str, 4, num_len);

    SCI_MEMCPY(item_text.wstr + string.wstr_len, str_temp, num_len * sizeof(wchar));
    item_text.wstr_len = (uint16)(string.wstr_len + num_len);
    item_text.wstr[item_text.wstr_len + 1] = '\0';
    GUITAB_SetItemText(ctrl_id, &item_text, 0);

    /* History(XX) */
    MMIRES_GetText(TXT_COMMON_HISTORY, ctrl_id, &string);
    if (string.wstr_len > GUITAB_MAX_ITEM_NAME_LENGTH - 4)
    {
        string.wstr_len = GUITAB_MAX_ITEM_NAME_LENGTH - 4;
    }
    SCI_MEMSET(num_str, 0x0, (4 + 1));
    SCI_MEMCPY(item_text.wstr, string.wstr_ptr, string.wstr_len * sizeof(wchar));
    num_len = (uint16)sprintf((char *)num_str, "(%d)",MMIWWW_Netfront_GetVisitHistoryNumber());
    MMI_STRNTOWSTR(str_temp, 4, (uint8 *)num_str, 4, num_len);
    SCI_MEMCPY(item_text.wstr + string.wstr_len, str_temp, num_len * sizeof(wchar));
    item_text.wstr_len = (uint16)(string.wstr_len + num_len);
    item_text.wstr[item_text.wstr_len + 1] = '\0';
    GUITAB_SetItemText(ctrl_id, &item_text, 1);
    if (PNULL != param)
    {
        /* Full paint from sub win */
        if(MMIWWW_BOOKMARK_WIN_ID == *(uint32*)param)
        {
            GUITAB_SetCurSel(ctrl_id, 0);
        }
        else if (MMIWWW_HISTORY_WIN_ID == *(uint32*)param)
        {
            GUITAB_SetCurSel(ctrl_id, 1);
        }
    }
}

/****************************************************************************/  
//  Description : Set bookmark and history list
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void MMIWWW_SetListProperty(MMI_WIN_ID_T win_id)
{
    MMI_HANDLE_T bm_win_handle = PNULL;
    MMI_HANDLE_T his_win_handle = PNULL;
    MMI_HANDLE_T main_win_handle = PNULL;
    MMI_HANDLE_T his_ctrl_handle = PNULL;
    MMI_HANDLE_T bm_ctrl_handle = PNULL;
    MMI_HANDLE_T applet_handle = PNULL;
    MMIWWW_CTRL_E focus_ctrl = MMIWWW_CTRL_NULL;


    focus_ctrl = MMIWWW_NetfrontGetMainMenuFocusCtrl();
    applet_handle = MMIWWW_HANDLE((WWW_APPLET_T *)MMK_GetAppletPtrByWin(win_id));
    main_win_handle = MMK_GetWinHandle(applet_handle, MMIWWW_MAINMENU_WIN_ID);
    bm_win_handle = MMK_GetWinHandle(applet_handle, MMIWWW_BOOKMARK_WIN_ID);
    his_win_handle = MMK_GetWinHandle(applet_handle, MMIWWW_HISTORY_WIN_ID);
    bm_ctrl_handle = MMK_GetCtrlHandleByWin(bm_win_handle, MMIWWW_BOOKMARK_LIST_CTRL_ID);
    his_ctrl_handle = MMK_GetCtrlHandleByWin(his_win_handle, MMIWWW_LISTBOX_HISTORY_CTRL_ID);

    if (MMIWWW_CTRL_BOOKMARK_LIST == focus_ctrl)
    {
        GUILIST_SetListState(bm_ctrl_handle, GUILIST_STATE_NEED_HIGHTBAR, TRUE);
        GUILIST_SetListState(his_ctrl_handle, GUILIST_STATE_NEED_HIGHTBAR, FALSE);
        MMK_WinInactiveCtrl(main_win_handle, FALSE);
        MMK_WinInactiveCtrl(his_win_handle, FALSE);
        MMK_SetActiveCtrl(MMK_GetCtrlHandleByWin(bm_win_handle, MMIWWW_BOOKMARK_LIST_CTRL_ID), TRUE);
    }
    else if (MMIWWW_CTRL_HISTORY_LIST == focus_ctrl)
    {
        GUILIST_SetListState(his_ctrl_handle, GUILIST_STATE_NEED_HIGHTBAR, TRUE);
        GUILIST_SetListState(bm_ctrl_handle, GUILIST_STATE_NEED_HIGHTBAR, FALSE);
        MMK_WinInactiveCtrl(main_win_handle, FALSE);
        MMK_WinInactiveCtrl(bm_win_handle, FALSE);
        MMK_SetActiveCtrl(MMK_GetCtrlHandleByWin(his_win_handle, MMIWWW_LISTBOX_HISTORY_CTRL_ID), TRUE);
    }
    else
    {
        GUILIST_SetListState(bm_ctrl_handle, GUILIST_STATE_NEED_HIGHTBAR, FALSE);
        GUILIST_SetListState(his_ctrl_handle, GUILIST_STATE_NEED_HIGHTBAR, FALSE);
        if (MMIWWW_CTRL_NULL == focus_ctrl || MMIWWW_CTRL_LINK_BUTTON == focus_ctrl || MMIWWW_CTRL_SEARCH_BUTTON == focus_ctrl)
        {
            MMK_WinInactiveCtrl(main_win_handle, FALSE);
        }
        MMK_WinInactiveCtrl(bm_win_handle, FALSE);
        MMK_WinInactiveCtrl(his_win_handle, FALSE);
    }
}
/****************************************************************************/  
//  Description : Set tab property
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void MMIWWW_SetTabProperty(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    uint16        img_width = 0; 
    GUITAB_ITEM_T item_ptr = {0};
    GUITAB_ITEM_CONTENT_T item_data_ptr = {0};
    MMI_HANDLE_T applet_handle = MMIWWW_HANDLE((WWW_APPLET_T *)MMIWWW_APPLETINSTANCE());
 
    GUITAB_SetMaxItemNum(ctrl_id, 2);
    GUIRES_GetImgWidthHeight(&img_width,PNULL,IMAGE_TAB_SELECT2,win_id);
//    GUITAB_SetItemWidth(ctrl_id, img_width);
    GUITAB_SetState(ctrl_id, GUITAB_STATE_SINGLE_LINE, TRUE);

    SCI_MEMSET(&item_ptr, 0, sizeof(item_ptr));
    SCI_MEMSET(&item_data_ptr, 0, sizeof(item_data_ptr));
    item_data_ptr.text_data.pressed_image_id = IMAGE_TAB_SELECT2;
    item_data_ptr.text_data.released_image_id = IMAGE_NULL;
    item_data_ptr.text_data.is_disp_font_bg = TRUE;
    item_ptr.child_win_handle = MMK_GetWinHandle(applet_handle, MMIWWW_BOOKMARK_WIN_ID);
    item_ptr.item_state = GUITAB_ITEMSTATE_SHOW_TEXT;
    item_ptr.item_data_ptr = &item_data_ptr;
    GUITAB_AppendItem(ctrl_id, &item_ptr);

    SCI_MEMSET(&item_ptr, 0, sizeof(item_ptr));
    SCI_MEMSET(&item_data_ptr, 0, sizeof(item_data_ptr));
    item_data_ptr.text_data.pressed_image_id = IMAGE_TAB_SELECT2;
    item_data_ptr.text_data.released_image_id = IMAGE_NULL;
    item_data_ptr.text_data.is_disp_font_bg = TRUE;
    item_ptr.child_win_handle = MMK_GetWinHandle(applet_handle, MMIWWW_HISTORY_WIN_ID);
    item_ptr.item_state = GUITAB_ITEMSTATE_SHOW_TEXT;
    item_ptr.item_data_ptr = &item_data_ptr;
    GUITAB_AppendItem(ctrl_id, &item_ptr);
    GUITAB_SetBackGroundImage(ctrl_id, IMAGE_WWW_TAB_BG);
    GUITAB_SetFontParam(ctrl_id, MMIWWW_MAINMENU_TAB_TEXT_FONT, MMITHEME_GetCurThemeFontColor(MMI_THEME_LIST_ITEM0), 0); /* tab's theme is Not OK*/
}

/****************************************************************************/    
//  Description :
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void MMIWWW_CreateToolboxFulldisplayButton(MMI_WIN_ID_T win_id)
{
    if (UILAYER_IsMultiLayerEnable())
    {
        UILAYER_CREATE_T create_info = {0};
        UILAYER_APPEND_BLT_T append_layer = {0};
        GUIBUTTON_INIT_DATA_T init_data = {0};
        uint16 img_width = 0; 
        uint16 img_height = 0; 
        MMI_HANDLE_T btn_handle = PNULL;
        MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

        if(PNULL == www_instance)
        {
        	return;
        }

        /* Create Button */
        init_data.bg.bg_type = GUI_BG_IMG;
        init_data.bg.img_id  = IMAGE_WWW_TOOLBOX_FULL_DISPLAY;
        btn_handle = GUIBUTTON_CreateDynamic(win_id, MMIWWW_TOOLBOX_FULL_DISPLAY_BUTTON_CTRL_ID, &init_data);
        IGUICTRL_SetState(MMK_GetCtrlPtr(btn_handle), GUICTRL_STATE_INVISIBLE | GUICTRL_STATE_DISABLE_TP, TRUE); /* Set Invisible */
#ifdef TOUCH_PANEL_SUPPORT
        GUIBUTTON_SetCallBackFunc(btn_handle, www_PageFullCallBack);
#endif
        /* Create Layer for Button*/
        GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_WWW_TOOLBOX_NORMAL_DISPLAY, win_id);
        create_info.lcd_id = GUI_MAIN_LCD_ID;
        create_info.owner_handle = win_id;
        create_info.offset_x = 0;
        create_info.offset_y = 0;
        create_info.width = img_width;
        create_info.height = img_height;
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE; 
        UILAYER_CreateLayer(&create_info, &www_instance->btn_layer_info);

        append_layer.lcd_dev_info = www_instance->btn_layer_info;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer);

        UILAYER_SetLayerColorKey(&www_instance->btn_layer_info, TRUE, UILAYER_TRANSPARENT_COLOR);
        GUICTRL_SetLcdDevInfo(MMK_GetCtrlPtr(btn_handle), &www_instance->btn_layer_info);
    }
}

/****************************************************************************/    
//  Description :
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void MMIWWW_SetToolboxFulldisplayButtonProperty(MMI_WIN_ID_T win_id)
{
    if (UILAYER_IsMultiLayerEnable())
    {
        uint16 img_width = 0;
        uint16 img_height = 0;
        BOOLEAN is_ful_dsp = TRUE;
        GUI_BOTH_RECT_T button_rect = {0};
        MMI_HANDLE_T button_handle = NULL;
        GUI_BG_T button_bg = {0};/*lint !e64*/
        MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();
        GUI_BOTH_RECT_T full_rect = MMITHEME_GetFullScreenBothRect();

        if(PNULL == www_instance)
        {
        	return;
        }
        UILAYER_Clear(&www_instance->btn_layer_info);

        is_ful_dsp = MMIWWW_IsFullDisplay();

        button_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_TOOLBOX_FULL_DISPLAY_BUTTON_CTRL_ID);
        IGUICTRL_SetState(MMK_GetCtrlPtr(button_handle), GUICTRL_STATE_INVISIBLE | GUICTRL_STATE_DISABLE_TP, (BOOLEAN)(!is_ful_dsp));

        GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_WWW_TOOLBOX_NORMAL_DISPLAY, win_id);
        button_rect.v_rect.right = full_rect.v_rect.right - MMIWWW_TOOLBOX_HORIZONTAL_MARGIN;
        button_rect.v_rect.left = button_rect.v_rect.right - img_width;
        button_rect.v_rect.bottom = full_rect.v_rect.bottom - MMIWWW_TOOLBOX_VERTICAL_MARGIN;
        button_rect.v_rect.top = button_rect.v_rect.bottom - img_height;
        button_rect.h_rect.right = full_rect.h_rect.right - MMIWWW_TOOLBOX_HORIZONTAL_MARGIN;
        button_rect.h_rect.left = button_rect.h_rect.right - img_width;
        button_rect.h_rect.bottom = full_rect.h_rect.bottom - MMIWWW_TOOLBOX_VERTICAL_MARGIN;
        button_rect.h_rect.top = button_rect.h_rect.bottom - img_height;
        GUIAPICTRL_SetBothRect(button_handle, &button_rect);

        button_bg.bg_type = GUI_BG_IMG;
        button_bg.img_id = is_ful_dsp ? IMAGE_WWW_TOOLBOX_NORMAL_DISPLAY : IMAGE_WWW_TOOLBOX_FULL_DISPLAY;
        GUIBUTTON_SetBg(button_handle, &button_bg);

        if (MMITHEME_IsMainScreenLandscape())
        {
            UILAYER_SetLayerPosition(&www_instance->btn_layer_info, button_rect.h_rect.left, button_rect.h_rect.top);
        }
        else
        {
            UILAYER_SetLayerPosition(&www_instance->btn_layer_info, button_rect.v_rect.left, button_rect.v_rect.top);
        }
    }
}
 
/*****************************************************************************/
// Description : Page back button call back func
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E www_PagePrevCallBack(void)
{
    slim_int     ret = SLIM_E_OK;
    slim_bool    bret = slim_true;

    if (MMIWWW_STATE_PAGE_LOADING == MMIWWW_GetWWWState(MMIWWW_Netfront_GetActWinIndex()))
    {
        ret = MMIWWW_Netfront_Abort();
    }
    else
    {
        bret = MMIWWW_HandleBackForward(TRUE);
    }
    return MMI_RESULT_TRUE;
}

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
// Description : Page option button call back func
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E www_PageOptionCallBack(void)
{
    if (MMIWWW_STATE_PAGE_VIEW == MMIWWW_GetWWWState(MMIWWW_Netfront_GetActWinIndex()))
    {
        MMIWWW_OpenWWWPageOptMenuWin();
    }
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
// Description : Page reload button call back func
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E www_PageReloadCallBack(void)
{
    slim_int    ret = SLIM_E_OK;
    
    ret = MMIWWW_Netfront_ReloadPage();

    return ((SLIM_E_OK == ret) ? MMI_RESULT_TRUE : MMI_RESULT_FALSE);
}

/*****************************************************************************/
// Description : Page forward button call back func
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
#if 0
LOCAL MMI_RESULT_E www_PageNextCallBack(void)
{
    slim_bool    bret = slim_true;

    bret = MMIWWW_HandleBackForward(FALSE);
    return ((bret) ? MMI_RESULT_TRUE : MMI_RESULT_FALSE);
}
#endif

/*****************************************************************************/
// Description : Switch full screen button call back func
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E www_PageFullCallBack(void)
{
    mmiwww_HandleFullDisplay(mmiwww_GetBrowserWinhandle(), (BOOLEAN)(!MMIWWW_IsFullDisplay()));
    return MMI_RESULT_TRUE;
}
#endif

/*****************************************************************************/
//  Description : Open WWW Alert Win
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_OpenWWWAlertWin(uint32 *time_period_ptr,
                                                MMI_TEXT_ID_T alert_txt1_id,
                                                MMI_TEXT_ID_T alert_txt2_id,
                                                MMI_IMAGE_ID_T image_id
                                                )
{
    MMI_WIN_ID_T alert_win_id = MMIWWW_ALERT_WIN_ID;

    if (MMK_IsOpenWin(alert_win_id))
    {
        MMK_CloseWin(alert_win_id);
    }

    MMIPUB_OpenAlertWinByTextId(time_period_ptr, alert_txt1_id, alert_txt2_id, image_id, &alert_win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
}

/*****************************************************************************/
// Description : Send message to browser page handle
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_SendMsgToBrowser(MMI_MESSAGE_ID_E  msg_id, uint32 data)
{
    MMI_HANDLE_T    win_handle = PNULL;

    win_handle = mmiwww_GetBrowserWinhandle();
    MMK_SendMsg(win_handle, msg_id, (DPARAM)&data);
    return;
}

/*****************************************************************************/
// Description : Post message to browser page handle
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
 PUBLIC void MMIWWW_PostMsgToBrowser(MMI_MESSAGE_ID_E msg_id, DPARAM param_ptr, uint32 size_of_param)
{
    MMI_HANDLE_T    win_handle = PNULL;

    win_handle = mmiwww_GetBrowserWinhandle();
    if (PNULL != win_handle)
    {
        MMK_PostMsg(win_handle, msg_id, param_ptr, size_of_param);
    }
    return;
}

/*****************************************************************************/
// Description : Handle MSG_WWW_NOTIFY_BG_SOUND msg 
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
LOCAL void MMIWWW_Handle_BG_Sound(DPARAM param)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();
    slim_bool is_exist = (slim_bool)(*((uint32*)param) & 0x000000ff);
    uint32 index = ((*(uint32*)param & 0x0000ff00) >> 8);

    //SCI_TRACE_LOW:"mmiwww_wintab.c:MMIWWW_Handle_BG_Sound is_exist=%d, index=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_9172_112_2_18_3_12_20_215,(uint8*)"dd", is_exist, index);

    if (index >= MMIWWW_BROWSER_MAX_TAB || PNULL == www_instance)
    {
        return;
    }

    www_instance->is_bg_sound[index] = is_exist;
    
    if (is_exist)
    {
        MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);

        MMIWWW_Play_BG_sound(index);
    }
}

/*****************************************************************************/
//  Description : Open JavaScript warning win
//  Global resource dependence : 
// Author: Bill.Ji
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_OpenJSAlertWin(slim_int  in_index, slim_char  *in_message)
{
    MMI_STRING_T    text = {0};

    //SCI_TRACE_LOW:"MMIWWW_OpenJSAlertWin() enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_9197_112_2_18_3_12_20_216,(uint8*)"");
    if (PNULL != in_message)
    {
        MMIWWW_ConvertToMMIString(&text, in_message);
        MMIPUB_OpenAlertWinByTextPtr(PNULL, &text, PNULL, IMAGE_PUBWIN_WARNING,
                                     PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
        MMIWWW_FREE_STRING(text);
    }
    return;
}

/*****************************************************************************/
//  Description : Convert UTF8 string to WString
//  Global resource dependence : 
// Author: Bill.Ji
//  Note:需调用者对out_string进行释放
/*****************************************************************************/
PUBLIC void MMIWWW_ConvertToMMIString(MMI_STRING_T  *out_string,
                                      slim_char     *in_string)
{
    wchar          *wstr_ptr = PNULL;
    uint16          len = 0;

    if (PNULL == out_string || PNULL == in_string)
    {
        return;
    }
    
    //SCI_TRACE_LOW:"MMIWWW_ConvertToMMIString() enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_9223_112_2_18_3_12_20_217,(uint8*)"");
    //初始化
    out_string->wstr_len = 0;
    out_string->wstr_ptr = PNULL;
    //开始转换
    len = (uint16)SCI_STRLEN((char *)in_string);
    if (len > 0)
    {
        wstr_ptr = SCI_ALLOCA((len + 1)* sizeof(wchar));
        //SCI_TRACE_LOW:"MMIWWW_ConvertToMMIString() len=%d wstr_ptr=0X%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_9233_112_2_18_3_12_20_218,(uint8*)"dd",len, wstr_ptr);
        if (PNULL != wstr_ptr)
        {
            SCI_MEMSET(wstr_ptr, 0x00, ((len + 1) * sizeof(wchar)));
            out_string->wstr_len = GUI_UTF8ToWstr(wstr_ptr, len, in_string, len);
            out_string->wstr_ptr = wstr_ptr;
        }
    }
    return;
}

/*****************************************************************************/
//  Description : Create JavaScript confirmation msg win
//  Global resource dependence : 
//  Author: Bill.Ji
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIWWW_OpenJSConfirmWin(slim_char  *in_message)
{
    MMI_WINDOW_TABLE_CREATE_T    win_table_create = {0};
    MMI_HANDLE_T                 win_handle = 0;
    MMI_HANDLE_T                 ctrl_handle = NULL;
    MMI_STRING_T                 text = {0};

    //SCI_TRACE_LOW:"MMIWWW_OpenJSConfirmWin enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_9255_112_2_18_3_12_20_219,(uint8*)"");

    //创建win
    win_table_create.applet_handle = MMIWWW_HANDLE(MMIWWW_APPLETINSTANCE());
    win_table_create.parent_win_handle = 0;
    win_table_create.win_table_ptr = MMIWWW_JSCONFIRM_WIN_TAB;
    win_handle = MMK_CreateWinTable( &win_table_create );
    //设置text数据
    if (PNULL != in_message)
    {
        MMIWWW_ConvertToMMIString(&text, in_message);
        ctrl_handle = MMK_GetCtrlHandleByWin(win_handle, MMIWWW_JSCONFIRM_TEXT_CTRL_ID);
        GUITEXT_SetString(ctrl_handle, text.wstr_ptr,text.wstr_len, FALSE);
        MMIWWW_FREE_STRING(text);
    }
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : Handle JavaScript confirmation msg win
//  Global resource dependence : none
//  Author: Bill.Ji
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWJSConfirmWin(MMI_WIN_ID_T     win_id,
                                        MMI_MESSAGE_ID_E msg_id,
                                        DPARAM           param)
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_HANDLE_T    ctrl_handle = NULL;
    slim_int        ret = SLIM_E_OK;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_JSCONFIRM_TEXT_CTRL_ID);
        MMK_SetActiveCtrl(ctrl_handle, FALSE);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        //SCI_TRACE_LOW:"HandleWWWJSConfirmWin enter OK ret=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_9297_112_2_18_3_12_20_220,(uint8*)"d", ret);
        MMK_CloseWin(win_id);
        ret = NFBrowser_ReplyRequestJSConfirm(MMIWWW_GETBROWSER, slim_true);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        //SCI_TRACE_LOW:"HandleWWWJSConfirmWin enter CANCEL ret=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_9304_112_2_18_3_12_20_221,(uint8*)"d", ret);
        MMK_CloseWin(win_id);
        ret = NFBrowser_ReplyRequestJSConfirm(MMIWWW_GETBROWSER, slim_false);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return (recode);
}

/*****************************************************************************/
//  Description : Create JavaScript prompt msg win
//  Global resource dependence : 
//  Author: Bill.Ji
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIWWW_OpenJSPromptWin(void)
{
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};
    MMI_HANDLE_T win_handle = 0;
    slim_char *in_message = PNULL;
    slim_char *in_value = PNULL;

    //SCI_TRACE_LOW:"MMIWWW_OpenJSPromptWin enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_9327_112_2_18_3_12_20_222,(uint8*)"");
    //初始化
    MMIWWW_GetJSPromtString(&in_message, &in_value);

    //创建win
    win_table_create.applet_handle = MMIWWW_HANDLE(MMIWWW_APPLETINSTANCE());
    win_table_create.parent_win_handle = 0;
    win_table_create.win_table_ptr = MMIWWW_JSPROMPT_WIN_TAB;
    win_handle = MMK_CreateWinTable( &win_table_create );

    mmiwww_HandleWWWJSPromptOpenWin(win_handle, in_message, in_value);
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E mmiwww_OpenWindowManagerWin(void)
{
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};
    MMI_HANDLE_T win_handle = 0;
    MMI_HANDLE_T applet_handle = PNULL;
    WWW_APPLET_T *applet = NULL;

    applet = MMIWWW_APPLETINSTANCE();
    applet_handle = MMIWWW_HANDLE(applet);
    win_handle = MMK_GetWinHandle(applet_handle, MMIWWW_WINDOW_MANAGER_WIN_ID);

    if (MMK_IsOpenWin(win_handle))
    {
        MMK_WinGrabFocus(win_handle);
        return MMI_RESULT_TRUE;
    }

    win_table_create.applet_handle = applet_handle;
    win_table_create.parent_win_handle = 0;
    win_table_create.win_table_ptr = MMIWWW_WINDOW_MANAGER_WIN_TAB;
    win_handle = MMK_CreateWinTable(&win_table_create);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E mmiwww_OpenWindowManagerOptWin(void)
{
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};
    MMI_HANDLE_T win_handle = 0;

    win_table_create.applet_handle = MMIWWW_HANDLE(MMIWWW_APPLETINSTANCE());
    win_table_create.parent_win_handle = 0;
    win_table_create.win_table_ptr = MMIWWW_WINDOW_MANAGER_OPT_WIN_TAB;
    win_handle = MMK_CreateWinTable(&win_table_create);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : Init ctrl data for JavaScript prompt win
//  Global resource dependence : 
//  Author: Bill.Ji
//  Note:
/*****************************************************************************/
LOCAL void mmiwww_HandleWWWJSPromptOpenWin(MMI_HANDLE_T  win_handle,
                                           slim_char    *in_message,
                                           slim_char    *in_value)
{
    MMI_HANDLE_T    ctrl_handle = NULL;
    MMI_STRING_T    text = {0};
    MMI_STRING_T    value = {0};
    //初始化
    ctrl_handle = MMK_GetCtrlHandleByWin(win_handle, MMIWWW_JSPROMPT_RICHTEXT_CTRL_ID);
    //in message
    if (PNULL != in_message)
    {
        MMIWWW_ConvertToMMIString(&text, in_message);
        MMIWWW_RichText_AddItem(ctrl_handle, &text, GUIRICHTEXT_FRAME_NONE, GUIRICHTEXT_TAG_NONE);
        MMIWWW_FREE_STRING(text);
    }
    //in value
    if (PNULL != in_value)
    {
        MMIWWW_ConvertToMMIString(&value, in_value);
        MMIWWW_RichText_AddItem(ctrl_handle, &value, GUIRICHTEXT_FRAME_NORMAL,
                                GUIRICHTEXT_TAG_NORMAL);
        MMIWWW_FREE_STRING(value);
    }
    //设置active控件
    MMK_SetActiveCtrl(ctrl_handle, FALSE);
    GUIRICHTEXT_SetFocusMode(ctrl_handle, GUIRICHTEXT_FOCUS_SHIFT_ADVANCED);
    MMK_PostMsg(ctrl_handle, MSG_APP_DOWN, PNULL, 0);
    return;
}

/*****************************************************************************/
//  Description : Handle JavaScript prompt msg win
//  Global resource dependence : none
//  Author: Bill.Ji
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWJSPromptWin(MMI_WIN_ID_T     win_id,
                                        MMI_MESSAGE_ID_E msg_id,
                                        DPARAM           param)
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    uint8          *value_ptr = PNULL;
    slim_int        ret = SLIM_E_OK;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
        //SCI_TRACE_LOW:"HandleWWWJSPromptWin enter OK"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_9441_112_2_18_3_12_20_223,(uint8*)"");
        value_ptr = mmiwww_HandleJSPromptOKWin(win_id);
        if (PNULL != value_ptr)
        {
            NFBrowser_ReplyRequestJSPrompt(MMIWWW_GETBROWSER, slim_true, value_ptr);
            SCI_FREE(value_ptr);
        }
        else
        {
            NFBrowser_ReplyRequestJSPrompt(MMIWWW_GETBROWSER, slim_true, PNULL);
        }
        //SCI_TRACE_LOW:"HandleWWWJSPromptWin enter OK ret=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_9452_112_2_18_3_12_20_224,(uint8*)"d", ret);
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        mmiwww_OpenJSPromptEditWin(win_id, 1);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        //SCI_TRACE_LOW:"HandleWWWJSPromptWin enter CANCEL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_9464_112_2_18_3_12_20_225,(uint8*)"");
        NFBrowser_ReplyRequestJSPrompt(MMIWWW_GETBROWSER, slim_false, PNULL);
        //SCI_TRACE_LOW:"HandleWWWJSPromptWin enter CANCEL ret=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_9466_112_2_18_3_12_20_226,(uint8*)"d", ret);
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return (recode);
}
/*****************************************************************************/
//  Description : Handle MSG_APP_OK msg for JavaScript prompt win
//  Global resource dependence : 
//  Author: Bill.Ji
//  Note:需调用者对返回的数据进行释放
/*****************************************************************************/
LOCAL uint8 *mmiwww_HandleJSPromptOKWin(MMI_WIN_ID_T  win_id)
{
    GUIRICHTEXT_ITEM_T    item = {0};/*lint !e64*/
    uint8                *ret_ptr = PNULL;
    uint16                buf_len = 0;
    MMI_HANDLE_T          ctrl_handle = NULL;
    //初始化
    ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_JSPROMPT_RICHTEXT_CTRL_ID);
    //获取输入的内容
    GUIRICHTEXT_GetItem(ctrl_handle, 1, &item);
    if (PNULL != item.text_data.buf.str_ptr && 0 != item.text_data.buf.len)
    {
        buf_len = (uint16)(item.text_data.buf.len * 3 + 1);
        ret_ptr = SCI_ALLOCA(buf_len+1);
        if (PNULL != ret_ptr)
        {
            //转换为utf8格式
            SCI_MEMSET(ret_ptr, 0x00, (buf_len+1));
            GUI_WstrToUTF8(ret_ptr, buf_len, item.text_data.buf.str_ptr,
                           item.text_data.buf.len);
        }
    }
    return ret_ptr;
}

/*****************************************************************************/
//  Description : Create JavaScript prompt edit win
//  Global resource dependence : 
//  Author: Bill.Ji
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E mmiwww_OpenJSPromptEditWin(MMI_WIN_ID_T  win_id, uint16 index)
{
    WWW_APPLET_T *applet = (WWW_APPLET_T *)MMK_GetAppletPtrByWin(win_id);
    uint32 focus_index = index;
    MMI_WINDOW_TABLE_CREATE_T    win_table_create = {0};
    MMI_HANDLE_T                 win_handle = 0;
    MMI_HANDLE_T                 ctrl_handle = NULL;
    MMI_CONTROL_CREATE_T         create = {0};
    GUIEDIT_INIT_DATA_T      init_data = {0};
    GUIRICHTEXT_ITEM_T           item = {0};/*lint !e64*/
    GUI_RECT_T                   rect = MMITHEME_GetClientRect();

    //初始化edit控件数据
    //SCI_TRACE_LOW:"mmiwww_OpenJSPromptEditWin enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_9522_112_2_18_3_12_20_227,(uint8*)"");

    ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_JSPROMPT_RICHTEXT_CTRL_ID);
    GUIRICHTEXT_GetItem(ctrl_handle, index, &item);
    init_data.type = GUIEDIT_TYPE_TEXT;
    init_data.both_rect.h_rect = init_data.both_rect.v_rect = rect;
    init_data.str_max_len = WWW_MAX_JSPROMPT_STRING_LEN;

    //创建win
    win_table_create.applet_handle = MMIWWW_HANDLE(MMIWWW_APPLETINSTANCE());
    win_table_create.parent_win_handle = 0;
    win_table_create.win_table_ptr = MMIWWW_JSPROMPT_EDIT_WIN_TAB;
    win_table_create.add_data_ptr = (ADD_DATA)focus_index;
    win_handle = MMK_CreateWinTable( &win_table_create );

    //创建edit控件
    create.ctrl_id = MMIWWW_JSPROMPT_EDIT_CTRL_ID;
    create.guid = SPRD_GUI_EDITBOX_ID;
    create.parent_win_handle = win_handle;
    create.init_data_ptr = &init_data;
    MMK_CreateControl(&create);

    ctrl_handle = MMK_GetCtrlHandleByWin(MMK_GetWinHandle(MMIWWW_HANDLE(applet), MMIWWW_JSPROMPT_EDIT_WIN_ID), MMIWWW_JSPROMPT_EDIT_CTRL_ID); 

    //set string
    if (PNULL != item.text_data.buf.str_ptr && 0 != item.text_data.buf.len)
    {
        GUIEDIT_SetString(ctrl_handle, item.text_data.buf.str_ptr, item.text_data.buf.len);
    }
        
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : handle messages of JSPrompt edit win
//  Global resource dependence : none
//  Author: Bill.Ji
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWJSPromptEditWin(MMI_WIN_ID_T     win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM           param)
{
    MMI_RESULT_E          recode = MMI_RESULT_TRUE;
    MMI_STRING_T          edit_string = {0};
    MMI_HANDLE_T          ctrl_handle = NULL;
    GUIRICHTEXT_ITEM_T    new_item = {0};/*lint !e64*/
    GUIRICHTEXT_ITEM_T    org_item = {0};/*lint !e64*/
    uint32                index = 0;
    WWW_APPLET_T         *applet = PNULL;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_JSPROMPT_EDIT_CTRL_ID);
        MMK_SetActiveCtrl(ctrl_handle, FALSE);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        //get data from edit ctrl
        ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_JSPROMPT_EDIT_CTRL_ID);
        GUIEDIT_GetString(ctrl_handle, &edit_string);
        // set to richtext ctrl
        applet = (WWW_APPLET_T *)MMK_GetAppletPtrByWin(win_id);
        ctrl_handle = MMK_GetCtrlHandleByWin(MMK_GetWinHandle(MMIWWW_HANDLE(applet), MMIWWW_JSPROMPT_WIN_ID), MMIWWW_JSPROMPT_RICHTEXT_CTRL_ID);
        index = (uint32)MMK_GetWinAddDataPtr(win_id);
        GUIRICHTEXT_GetItem(ctrl_handle, (uint16)index, &org_item);
        SCI_MEMCPY(&new_item, &org_item, sizeof(GUIRICHTEXT_ITEM_T));
        new_item.text_data.buf.len = edit_string.wstr_len;
        new_item.text_data.buf.str_ptr = edit_string.wstr_ptr;
        new_item.text_type = GUIRICHTEXT_TEXT_BUF;
        GUIRICHTEXT_ModifyItem(ctrl_handle, (uint16)index, &new_item);
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
    return (recode);
}

/*****************************************************************************/
//  Description : update WWW net setting when the index list item is selected  
//  Global resource dependence : none
//  Author: jixin.xu
//  Note:
/*****************************************************************************/
LOCAL void mmiwww_ConnectionCallback(MN_DUAL_SYS_E dual_sys, uint32 selected_index)
{
    MMIWWW_SetNetSettingIndex(dual_sys, selected_index);
    MMIPUB_OpenAlertSuccessWin( TXT_WWW_SET_LINK_SUCCESS);
}

/*****************************************************************************/
//  Description :setting link list
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLinkListWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E err_code = MMI_RESULT_TRUE;
    uint8  link_index = 0;
    MN_DUAL_SYS_E  dual_sim = MN_DUAL_SYS_MAX;
    uint8 index = 0;
    uint16 default_index = 0;
    MMICONNECTION_LINKSETTING_DETAIL_T* link_setting_ptr = PNULL;
    MMI_STRING_T name_str = {0};
    MMI_HANDLE_T ctrl_hanlde = PNULL;

    dual_sim = (uint32)MMK_GetWinAddDataPtr(win_id);/*lint !e64*/
    ctrl_hanlde = MMK_GetCtrlHandleByWin(win_id, MMIWWW_LISTBOX_LINK_SETTINGCTRL_ID);
    //SCI_TRACE_LOW:"mmiwww_wintab.c:HandleLinkListWinMsg() msg_id=0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_9638_112_2_18_3_12_21_228,(uint8*)"d", msg_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:

        GUILIST_SetMaxItem(ctrl_hanlde, MMIWWW_MAX_SETTING_NUM, FALSE );
        for(index = 0; index < MMIAPICONNECTION_GetLinkSettingNum(dual_sim); index++)
        {
            link_setting_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(dual_sim, index);
            name_str.wstr_ptr = link_setting_ptr->name;
            name_str.wstr_len = link_setting_ptr->name_len;

            MMIWWW_AppendOneListItem(ctrl_hanlde, &name_str, TXT_COMMON_OK, TXT_NULL, STXT_RETURN);
        }

        default_index = MMIAPIWWW_GetNetSettingIndex(dual_sim);
        GUILIST_SetSelectedItem(ctrl_hanlde,default_index,TRUE);
        GUILIST_SetCurItemIndex(ctrl_hanlde,default_index); 
        MMK_SetAtvCtrl(win_id, ctrl_hanlde);
        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
        link_index = (uint8)GUILIST_GetCurItemIndex(ctrl_hanlde);
        GUILIST_SetSelectedItem(ctrl_hanlde, link_index, TRUE);
        MMIWWW_SetNetSettingIndex(dual_sim,link_index);
        MMIPUB_OpenAlertSuccessWin( TXT_WWW_SET_LINK_SUCCESS);
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default :
        err_code = MMI_RESULT_FALSE;
        break;
    }    
    
    return err_code;
}

/*****************************************************************************/
//  Description :charset list
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCharsetListWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E err_code = MMI_RESULT_TRUE;
    MMI_HANDLE_T ctrl_hanlde = PNULL;

    ctrl_hanlde = MMK_GetCtrlHandleByWin(win_id, MMIWWW_LISTBOX_CHARSET_ID);
    //SCI_TRACE_LOW:"mmiwww_wintab.c:HandleCharsetListWinMsg() msg_id=0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_9695_112_2_18_3_12_21_229,(uint8*)"d", msg_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            uint32 i = 0;
            MMI_STRING_T text = {0};
            uint32 max_item = 0;
            int32 default_item = 0;

            const MMI_TEXT_ID_T text_id[] = {
                                                TXT_COMMON_AUTO_SELECT, TXT_WWW_CHARSET_US_ASCII, TXT_WWW_CHARSET_GB2312, TXT_WWW_CHARSET_GBK, TXT_WWW_CHARSET_GB18030,
                                                TXT_WWW_CHARSET_BIG5, TXT_WWW_CHARSET_UTF_8, TXT_WWW_CHARSET_UTF_16BE
                                            };

            max_item = sizeof(text_id) / sizeof(text_id[0]);
            GUILIST_SetMaxItem(ctrl_hanlde, max_item, FALSE );
            for(i = 0; i < max_item; i++)
            {
                MMIRES_GetText(text_id[i], win_id, &text);
                MMIWWW_AppendOneListItem(ctrl_hanlde, &text, TXT_COMMON_OK, TXT_NULL, STXT_RETURN);
            }

            default_item = MMIWWW_GetCharsetIndex(MMIWWW_Netfront_GetActWinIndex());
            GUILIST_SetSelectedItem(ctrl_hanlde,default_item,TRUE);
            GUILIST_SetTopItemIndex(ctrl_hanlde,default_item);
            GUILIST_SetCurItemIndex(ctrl_hanlde,default_item);
            MMK_SetAtvCtrl(win_id, ctrl_hanlde);
        }
        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
        {
            int32 charset = -1;
            uint16 focus_item = 0;
            int32 tab_index = 0;

            focus_item = GUILIST_GetCurItemIndex(ctrl_hanlde);
            tab_index = MMIWWW_Netfront_GetActWinIndex();

            if (MMIWWW_GetCharsetIndex(tab_index) != focus_item)
            {
                MMIWWW_SetCharsetIndex(tab_index, focus_item);
                MMIWWW_SetCharsetChange(tab_index, TRUE);
                charset = MMIWWW_ConvertCharsetIndexToCharset(focus_item);
                if (charset >= 0)
                {
                    MMIWWW_Netfront_ChangeToEncoding(charset);
                }
                else
                {
                    MMIWWW_SetBrowserReloadFlag((uint8)tab_index, TRUE);
                }
            }
            MMK_CloseWin(win_id);
            MMIPUB_OpenAlertSuccessWin(TXT_SET_SUCCESS);
        }
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default :
        err_code = MMI_RESULT_FALSE;
        break;
    }    
    
    return err_code;
}

/*****************************************************************************/
//  Description :charset list
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePointerModeListWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E err_code = MMI_RESULT_TRUE;
    MMI_HANDLE_T ctrl_hanlde = PNULL;

    ctrl_hanlde = MMK_GetCtrlHandleByWin(win_id, MMIWWW_LISTBOX_POINTERMODE_ID);
    //SCI_TRACE_LOW:"mmiwww_wintab.c:HandleCharsetListWinMsg() msg_id=0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_9782_112_2_18_3_12_21_230,(uint8*)"d", msg_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            uint32 i = 0;
            MMI_STRING_T text = {0};
            uint32 max_item = 0;
            int32 default_item = 0;
            const MMI_TEXT_ID_T text_id[] = {TXT_WWW_POINTER_MODE_NORMAL, TXT_WWW_POINTER_MODE_VP};

            max_item = sizeof(text_id)/sizeof(text_id[0]);
            GUILIST_SetMaxItem(ctrl_hanlde, max_item, FALSE);
            for(i = 0; i < max_item; i++)
            {
                MMIRES_GetText(text_id[i], win_id, &text);
                MMIWWW_AppendOneListItem(ctrl_hanlde, &text, TXT_COMMON_OK, TXT_NULL, STXT_RETURN);
            }
            
            default_item = (uint16)(MMIWWW_GetPointerMode());
            GUILIST_SetTopItemIndex(ctrl_hanlde, 0);
            GUILIST_SetSelectedItem(ctrl_hanlde, default_item, TRUE);
            GUILIST_SetCurItemIndex(ctrl_hanlde, default_item);
            MMK_SetAtvCtrl(win_id, ctrl_hanlde);
        }
        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
        {
            uint16 focus_item = 0;
            MMIWWW_POINTER_MODE_E pointer_mode = MMIWWW_POINTER_MODE_NORMAL;

            focus_item = GUILIST_GetCurItemIndex(ctrl_hanlde);
            pointer_mode = MMIWWW_GetPointerMode();
            if (focus_item != (uint16)pointer_mode)
            {
                if (0 == focus_item)
                {
                    pointer_mode = MMIWWW_POINTER_MODE_NORMAL;
                }
                else
                {
                    pointer_mode = MMIWWW_POINTER_MODE_VIRTUAL_POINTER;
                }
                MMIWWW_SetPointerMode(pointer_mode);
                MMIWWW_Netfront_SetPointerMode(pointer_mode);
            }

            MMK_CloseWin(win_id);
            MMIPUB_OpenAlertSuccessWin(TXT_SET_SUCCESS);
        }
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default :
        err_code = MMI_RESULT_FALSE;
        break;
    }    
    
    return err_code;
}

/*****************************************************************************/
//  Description :Shotcut key menu
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWShotcutKeyWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"mmiwww_wintab.c:HandleWWWShotcutKeyWinMsg() msg_id=0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_9861_112_2_18_3_12_21_231,(uint8*)"d", msg_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        mmiwww_CreateShotcutMenu(win_id);
        MMK_SetActiveCtrl(MMK_GetCtrlHandleByWin(win_id, MMIWWW_SHUTCUT_KEY_OPT_CTRL_ID), FALSE);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        {
            uint32 menu_id = 0;
            MMI_HANDLE_T ctrl_handle = PNULL;

            ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SHUTCUT_KEY_OPT_CTRL_ID);
            menu_id = GUIMENU_GetCurNodeId(ctrl_handle);

            switch ( menu_id )
            {
            case ID_WWW_SHOTCUT_KEY_1:
            case ID_WWW_SHOTCUT_KEY_2:
            case ID_WWW_SHOTCUT_KEY_3:
            case ID_WWW_SHOTCUT_KEY_4:
            case ID_WWW_SHOTCUT_KEY_5:
            case ID_WWW_SHOTCUT_KEY_6:
            case ID_WWW_SHOTCUT_KEY_7:
            case ID_WWW_SHOTCUT_KEY_8:
            case ID_WWW_SHOTCUT_KEY_9:
                MMK_CloseWin(win_id);
                MMIWWW_PostMsgToBrowser(MSG_APP_1 + (menu_id - ID_WWW_SHOTCUT_KEY_1), PNULL, 0 );
                break;

            case ID_WWW_SHOTCUT_KEY_0:
                MMK_CloseWin(win_id);
                MMIWWW_PostMsgToBrowser(MSG_APP_0, PNULL, 0 );
                break;
            default:
                break;
            }
        }
        break;

    case MSG_APP_0:
        MMK_CloseWin(win_id);
        MMIWWW_PostMsgToBrowser(msg_id, PNULL, 0 );
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;

}

/*****************************************************************************/
//    Description : set WWW display settings param
//    Global resource dependence : 
//    Author: kun.yu
//    Note: 
/*****************************************************************************/
LOCAL void SetWWWDisplaySetting(MMI_WIN_ID_T win_id)
{
    uint32 i = 0;
    uint16 cur_index = 0;    
    MN_RETURN_RESULT_E nv_value = MN_RETURN_SUCCESS;   
    MMI_TEXT_ID_T script_text[] = {TXT_WWW_JAVA_SCRIPT,TXT_WWW_ECMA_SCRIPT,TXT_COMMON_COMMON_DISABLE};
    MMI_TEXT_ID_T animal_text[] = {TXT_WWW_DISPLAY,TXT_WWW_NODISPLAY};
    MMI_TEXT_ID_T image_text[] = {TXT_WWW_DISPLAY,TXT_WWW_NODISPLAY};
    MMI_TEXT_ID_T table_text[] = {TXT_WWW_ENABLE,TXT_COMMON_COMMON_DISABLE};
    MMI_TEXT_ID_T sound_text[] = {TXT_COMMON_STARTUP,TXT_COMMON_CLOSE};
//    MMI_TEXT_ID_T popup_text[] = {TXT_WWW_DISPLAY,TXT_WWW_NODISPLAY};
    MMI_STRING_T script_item[MMIWWW_SETTINGS_SCRIPT_TYPE_NUM] = {0};
    MMI_STRING_T animal_item[MMIWWW_SETTINGS_ANIMAL_TYPE_NUM] = {0};
    MMI_STRING_T image_item[MMIWWW_SETTINGS_IMAGE_TYPE_NUM] = {0};
    MMI_STRING_T table_item[MMIWWW_SETTINGS_TABLE_TYPE_NUM] = {0};
    MMI_STRING_T sound_item[MMIWWW_SETTINGS_SOUND_TYPE_NUM] = {0};
//    MMI_STRING_T popup_item[MMIWWW_SETTINGS_POPUP_TYPE_NUM] = {0};

    MMI_HANDLE_T script_form_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_SCRIPT_FORM_CTRL_ID);
    MMI_HANDLE_T script_lable_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_SCRIPT_LABLE_CTRL_ID);
    MMI_HANDLE_T script_list_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_SCRIPT_DROPLIST_CTRL_ID);
    MMI_HANDLE_T animal_form_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_ANIMAL_FORM_CTRL_ID);
    MMI_HANDLE_T animal_lable_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_ANIMAL_LABLE_CTRL_ID);
    MMI_HANDLE_T animal_list_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_ANIMAL_DROPLIST_CTRL_ID);
    MMI_HANDLE_T image_form_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_IMAGE_FORM_CTRL_ID);
    MMI_HANDLE_T image_lable_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_IMAGE_LABLE_CTRL_ID);
    MMI_HANDLE_T image_list_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_IMAGE_DROPLIST_CTRL_ID);
    MMI_HANDLE_T table_form_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_TABLE_FORM_CTRL_ID);
    MMI_HANDLE_T table_lable_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_TABLE_LABLE_CTRL_ID);
    MMI_HANDLE_T table_list_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_TABLE_DROPLIST_CTRL_ID);
    MMI_HANDLE_T sound_form_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_SOUND_FORM_CTRL_ID);
    MMI_HANDLE_T sound_lable_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_SOUND_LABLE_CTRL_ID);
    MMI_HANDLE_T sound_list_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_SOUND_DROPLIST_CTRL_ID);
//    MMI_HANDLE_T popup_lable_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_POPUP_LABLE_CTRL_ID);
//    MMI_HANDLE_T popup_list_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_POPUP_DROPLIST_CTRL_ID);

    GUIFORM_SetStyle(script_form_ctrl,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(animal_form_ctrl,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(image_form_ctrl,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(table_form_ctrl,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(sound_form_ctrl,GUIFORM_STYLE_UNIT);

    GUILABEL_SetTextById(script_lable_ctrl, TXT_WWW_SCRIPT, FALSE);
    GUILABEL_SetTextById(animal_lable_ctrl, TXT_COMM_ANIMATION, FALSE);
    GUILABEL_SetTextById(image_lable_ctrl, TXT_COMMON_PIC, FALSE);
    GUILABEL_SetTextById(table_lable_ctrl, TXT_WWW_TABLE, FALSE);
    GUILABEL_SetTextById(sound_lable_ctrl, TXT_WWW_BGMUSIC, FALSE);
//    GUILABEL_SetTextById(popup_lable_ctrl, TXT_WWW_POPUPWIN, FALSE);

    //script dropdownlist
    for (i = 0; i < MMIWWW_SETTINGS_SCRIPT_TYPE_NUM; i++)
    {
        MMI_GetLabelTextByLang(script_text[i], &script_item[i]);
    }

    GUIDROPDOWNLIST_AppendItemArray(script_list_ctrl, script_item, MMIWWW_SETTINGS_SCRIPT_TYPE_NUM);

    //get current method index
    MMINV_READ( MMINV_WWW_SCRIPT_MODE, &cur_index, nv_value );

    if(MN_RETURN_SUCCESS != nv_value)
    {
        cur_index = 0;
    }

    //set select method item
    GUIDROPDOWNLIST_SetCurItemIndex(script_list_ctrl, cur_index);

    //animal dropdownlist
    for (i = 0; i < MMIWWW_SETTINGS_ANIMAL_TYPE_NUM; i++)
    {
        MMI_GetLabelTextByLang(animal_text[i], &animal_item[i]);
    }

    GUIDROPDOWNLIST_AppendItemArray(animal_list_ctrl, animal_item, MMIWWW_SETTINGS_ANIMAL_TYPE_NUM);

    //get current method index
    MMINV_READ( MMINV_WWW_ANIMATION_MODE, &cur_index, nv_value );

    if(MN_RETURN_SUCCESS != nv_value)
    {
        cur_index = 0;
    }

    //set select method item
    GUIDROPDOWNLIST_SetCurItemIndex(animal_list_ctrl, cur_index);

    //image dropdownlist
    for (i = 0; i < MMIWWW_SETTINGS_IMAGE_TYPE_NUM; i++)
    {
        MMI_GetLabelTextByLang(image_text[i], &image_item[i]);
    }

    GUIDROPDOWNLIST_AppendItemArray(image_list_ctrl, image_item, MMIWWW_SETTINGS_IMAGE_TYPE_NUM);

    //get current method index
    MMINV_READ( MMINV_WWW_PHOTO_MODE, &cur_index, nv_value );

    if(MN_RETURN_SUCCESS != nv_value)
    {
        cur_index = 0;
    }

    //set select method item
    GUIDROPDOWNLIST_SetCurItemIndex(image_list_ctrl, cur_index);

    //table dropdownlist
    for (i = 0; i < MMIWWW_SETTINGS_TABLE_TYPE_NUM; i++)
    {
        MMI_GetLabelTextByLang(table_text[i], &table_item[i]);
    }

    GUIDROPDOWNLIST_AppendItemArray(table_list_ctrl, table_item, MMIWWW_SETTINGS_TABLE_TYPE_NUM);

    //get current method index
    MMINV_READ( MMINV_WWW_TABLE_MODE, &cur_index, nv_value );

    if(MN_RETURN_SUCCESS != nv_value)
    {
        cur_index = 0;
    }

    //set select method item
    GUIDROPDOWNLIST_SetCurItemIndex(table_list_ctrl, cur_index);

    //sound dropdownlist
    for (i = 0; i < MMIWWW_SETTINGS_SOUND_TYPE_NUM; i++)
    {
        MMI_GetLabelTextByLang(sound_text[i], &sound_item[i]);
    }

    GUIDROPDOWNLIST_AppendItemArray(sound_list_ctrl, sound_item, MMIWWW_SETTINGS_SOUND_TYPE_NUM);

    //get current method index
    MMINV_READ( MMINV_WWW_BGMUSIC_MODE, &cur_index, nv_value );

    if(MN_RETURN_SUCCESS != nv_value)
    {
        cur_index = 0;
    }

    //set select method item
    GUIDROPDOWNLIST_SetCurItemIndex(sound_list_ctrl, cur_index);

    //popup win dropdownlist
/*    for (i = 0; i < MMIWWW_SETTINGS_POPUP_TYPE_NUM; i++)
    {
        MMI_GetLabelTextByLang(popup_text[i], &item_str);

        popup_item[i].wstr_len = (uint16)MIN(DROPDOWNLIST_STRING_MAX_NUM, item_str.wstr_len);
        MMI_WSTRNCPY(popup_item[i].wstr,
                                DROPDOWNLIST_STRING_MAX_NUM,
                                item_str.wstr_ptr,
                                item_str.wstr_len,
                                popup_item[i].wstr_len);
    }

    GUIDROPDOWNLIST_AppendItemArray(popup_list_ctrl, popup_item, MMIWWW_SETTINGS_POPUP_TYPE_NUM);

    //get current method index
    MMINV_READ( MMINV_WWW_POPUPWIN_MODE, &cur_index, nv_value );

    if(MN_RETURN_SUCCESS != nv_value)
    {
        cur_index = 0;
    }

    //set select method item
    GUIDROPDOWNLIST_SetCurItemIndex(popup_list_ctrl, cur_index);*/
    
    return;
}

/*****************************************************************************/
//    Description : set WWW cookie settings param
//    Global resource dependence : 
//    Author: kun.yu
//    Note: 
/*****************************************************************************/
LOCAL void SetWWWCookieSetting(MMI_WIN_ID_T win_id)
{
    uint32 i = 0;
    uint16 cur_index = 0;    
    MN_RETURN_RESULT_E nv_value = MN_RETURN_SUCCESS;
#ifdef MMIWWW_SETTINGS_COOKIE_USE_QUERY
    MMI_TEXT_ID_T cookie_mode_text[] = {TXT_WWW_COOKIE_MODE_ACCEPT_ALL, TXT_WWW_COOKIE_MODE_REJECT_ALL};
#else
    MMI_TEXT_ID_T cookie_mode_text[] = {TXT_WWW_COOKIE_MODE_ACCEPT_ALL, TXT_WWW_COOKIE_MODE_REJECT_ALL, TXT_COMMON_PROMPT};
#endif

    MMI_STRING_T cookie_mode_item[MMIWWW_SETTINGS_COOKIE_MODE_NUM] = {0};

    MMI_HANDLE_T cookie_mode_form_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_COOKIE_MODE_FORM_CTRL_ID);
    MMI_HANDLE_T cookie_mode_lable_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_COOKIE_MODE_LABLE_CTRL_ID);
    MMI_HANDLE_T cookie_mode_list_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_COOKIE_MODE_DROPLIST_CTRL_ID);
    MMI_HANDLE_T clear_cookie_form_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_CLEAR_COOKIE_FORM_CTRL_ID);
    MMI_HANDLE_T clear_cookie_lable_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_CLEAR_COOKIE_LABLE_CTRL_ID);

    GUIFORM_SetStyle(cookie_mode_form_ctrl,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(clear_cookie_form_ctrl,GUIFORM_STYLE_UNIT);

    GUILABEL_SetTextById(cookie_mode_lable_ctrl, TXT_WWW_COOKIE_MODE, FALSE);
    GUILABEL_SetTextById(clear_cookie_lable_ctrl, TXT_WWW_CLEAR_COOKIE, FALSE);

    //cookie mode dropdowmlist
    for (i = 0; i < MMIWWW_SETTINGS_COOKIE_MODE_NUM; i++)
    {
        MMI_GetLabelTextByLang(cookie_mode_text[i], &cookie_mode_item[i]);
    }

    GUIDROPDOWNLIST_AppendItemArray(cookie_mode_list_ctrl, cookie_mode_item, MMIWWW_SETTINGS_COOKIE_MODE_NUM);

    //get current method index
    MMINV_READ( MMINV_WWW_COOKIE_MODE, &cur_index, nv_value );

    if(MN_RETURN_SUCCESS != nv_value)
    {
        cur_index = 0;
    }

     //set select method item
    GUIDROPDOWNLIST_SetCurItemIndex(cookie_mode_list_ctrl, cur_index);
}

/*****************************************************************************/
//    Description : set WWW cache settings param
//    Global resource dependence : 
//    Author: kun.yu
//    Note: 
/*****************************************************************************/
LOCAL void SetWWWCacheSetting(MMI_WIN_ID_T win_id)
{
    uint32 i = 0;
    uint16 cur_index = 0;    
    MN_RETURN_RESULT_E nv_value = MN_RETURN_SUCCESS;   

    MMI_TEXT_ID_T cache_mode_text[] = { TXT_WWW_CLOSE_CACHE, TXT_WWW_NETWORK_PRIORITY,
                                                                TXT_WWW_CACHE_PRIORITY,TXT_WWW_ONLY_USE_CACHE};
    MMI_TEXT_ID_T query_disk_text[] = { TXT_COMMON_STARTUP, TXT_COMMON_CLOSE };

    MMI_STRING_T cache_mode_item[MMIWWW_SETTINGS_CACHE_MODE_NUM] = {0};
    MMI_STRING_T query_disk_item[MMIWWW_SETTINGS_QUERY_DISK_NUM] = {0};

    MMI_HANDLE_T cache_mode_form_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_CACHE_MODE_FORM_CTRL_ID);
    MMI_HANDLE_T cache_mode_lable_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_CACHE_MODE_LABLE_CTRL_ID);
    MMI_HANDLE_T cache_mode_list_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_CACHE_MODE_DROPLIST_CTRL_ID);
    MMI_HANDLE_T query_disk_form_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_QUERY_DISK_FORM_CTRL_ID);
    MMI_HANDLE_T query_disk_lable_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_QUERY_DISK_LABLE_CTRL_ID);
    MMI_HANDLE_T query_disk_list_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_QUERY_DISK_DROPLIST_CTRL_ID);
    MMI_HANDLE_T clear_cache_form_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_CLEAR_CACHE_FORM_CTRL_ID);
    MMI_HANDLE_T clear_cache_lable_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_CLEAR_CACHE_LABLE_CTRL_ID);
    
    GUIFORM_SetStyle(cache_mode_form_ctrl,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(query_disk_form_ctrl,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(clear_cache_form_ctrl,GUIFORM_STYLE_UNIT);

    GUILABEL_SetTextById(cache_mode_lable_ctrl, TXT_WWW_CACHE_MODE, FALSE);
    GUILABEL_SetTextById(query_disk_lable_ctrl, TXT_WWW_IF_USE_DISK_CACHE, FALSE);
    GUILABEL_SetTextById(clear_cache_lable_ctrl, TXT_COMMON_COMMON_CLEARCACHE, FALSE);

    //cache mode dropdowmlist
    for (i = 0; i < MMIWWW_SETTINGS_CACHE_MODE_NUM; i++)
    {
        MMI_GetLabelTextByLang(cache_mode_text[i], &cache_mode_item[i]);
    }

    GUIDROPDOWNLIST_AppendItemArray(cache_mode_list_ctrl, cache_mode_item, MMIWWW_SETTINGS_CACHE_MODE_NUM);

    //get current method index
    MMINV_READ( MMINV_WWW_CACHE_MODE, &cur_index, nv_value );

    if(MN_RETURN_SUCCESS != nv_value)
    {
        cur_index = 1;
    }

     //set select method item
    GUIDROPDOWNLIST_SetCurItemIndex(cache_mode_list_ctrl, cur_index);

     //query disk dropdowmlist
    for (i = 0; i < MMIWWW_SETTINGS_QUERY_DISK_NUM; i++)
    {
        MMI_GetLabelTextByLang(query_disk_text[i], &query_disk_item[i]);
    }

    GUIDROPDOWNLIST_AppendItemArray(query_disk_list_ctrl, query_disk_item, MMIWWW_SETTINGS_QUERY_DISK_NUM);

    //get current method index
    MMINV_READ( MMINV_WWW_DISKCACHE_MODE, &cur_index, nv_value );

    if(MN_RETURN_SUCCESS != nv_value)
    {
        cur_index = 0;
    }

     //set select method item
    GUIDROPDOWNLIST_SetCurItemIndex(query_disk_list_ctrl, cur_index);
}

/*****************************************************************************/
//    Description : set WWW webdata settings param
//    Global resource dependence : 
//    Author: kun.yu
//    Note: 
/*****************************************************************************/
LOCAL void SetWWWWebdataSetting(MMI_WIN_ID_T win_id)
{
    uint32 i = 0;
    uint16 cur_index = 0;    
    MN_RETURN_RESULT_E nv_value = MN_RETURN_SUCCESS;

    MMI_TEXT_ID_T webdata_mode_text[] = {TXT_COMMON_STARTUP, TXT_COMMON_CLOSE};

    MMI_STRING_T webdata_mode_item[MMIWWW_SETTINGS_WEBDATA_MODE_NUM] = {0};

    MMI_HANDLE_T webdata_mode_form_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_WEBDATA_FORM1_CTRL_ID);
    MMI_HANDLE_T webdata_mode_lable_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_WEBDATA_LABLE_CTRL_ID);
    MMI_HANDLE_T webdata_mode_list_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_WEBDATA_DROPLIST_CTRL_ID);
    MMI_HANDLE_T clear_webdata_form_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_CLEAR_WEBDATA_FORM_CTRL_ID); 
    MMI_HANDLE_T clear_webdata_lable_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_CLEAR_WEBDATA_LABLE_CTRL_ID); 

    GUIFORM_SetStyle(webdata_mode_form_ctrl,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(clear_webdata_form_ctrl,GUIFORM_STYLE_UNIT);

    GUILABEL_SetTextById(webdata_mode_lable_ctrl, TXT_WWW_AUTO, FALSE);
    GUILABEL_SetTextById(clear_webdata_lable_ctrl, TXT_WWW_CLEAR_WEBDATA, FALSE);

    //webdata mode dropdowmlist
    for (i = 0; i < MMIWWW_SETTINGS_WEBDATA_MODE_NUM; i++)
    {
        MMI_GetLabelTextByLang(webdata_mode_text[i], &webdata_mode_item[i]);
    }

    GUIDROPDOWNLIST_AppendItemArray(webdata_mode_list_ctrl, webdata_mode_item, MMIWWW_SETTINGS_WEBDATA_MODE_NUM);

    //get current method index
    MMINV_READ( MMINV_WWW_WEBDATA_MODE, &cur_index, nv_value );

    if(MN_RETURN_SUCCESS != nv_value)
    {
        cur_index = 0;
    }

     //set select method item
    GUIDROPDOWNLIST_SetCurItemIndex(webdata_mode_list_ctrl, cur_index);
}

/*****************************************************************************/
//  Description : Open WMLS warning win
//  Global resource dependence : 
// Author: Kun.yu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_OpenWMLSAlertWin(slim_char  *in_message)
{
    MMI_STRING_T    text = {0};

    //SCI_TRACE_LOW:"MMIWWW_OpenWMLSAlertWin() enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_10278_112_2_18_3_12_22_232,(uint8*)"");
    if (PNULL != in_message)
    {
        MMIWWW_ConvertToMMIString(&text, in_message);
        MMIPUB_OpenAlertWinByTextPtr(PNULL, &text, PNULL, IMAGE_PUBWIN_WARNING,
                                     PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
        MMIWWW_FREE_STRING(text);
    }
    return;
}

/*****************************************************************************/
//  Description : Create WMLS confirmation msg win
//  Global resource dependence : 
//  Author: Kun.yu
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIWWW_OpenWMLSConfirmWin(slim_char  *in_message)
{
    MMI_WINDOW_TABLE_CREATE_T    win_table_create = {0};
    MMI_HANDLE_T                 win_handle = 0;
    MMI_HANDLE_T                 ctrl_handle = NULL;
    MMI_STRING_T                 text = {0};

    //SCI_TRACE_LOW:"MMIWWW_OpenWMLSConfirmWin enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_10300_112_2_18_3_12_22_233,(uint8*)"");
    
    //创建win
    win_table_create.applet_handle = MMIWWW_HANDLE(MMIWWW_APPLETINSTANCE());
    win_table_create.parent_win_handle = 0;
    win_table_create.win_table_ptr = MMIWWW_WMLSCONFIRM_WIN_TAB;
    win_handle = MMK_CreateWinTable( &win_table_create );
    //设置text数据
    if (PNULL != in_message)
    {
        MMIWWW_ConvertToMMIString(&text, in_message);
        ctrl_handle = MMK_GetCtrlHandleByWin(win_handle, MMIWWW_WMLSCONFIRM_TEXT_CTRL_ID);
        GUITEXT_SetString(ctrl_handle, text.wstr_ptr,text.wstr_len, FALSE);
        MMIWWW_FREE_STRING(text);
    }
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : Handle WMLS confirmation msg win
//  Global resource dependence : none
//  Author: Kun.yu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWWMLSConfirmWin(MMI_WIN_ID_T     win_id,
                                        MMI_MESSAGE_ID_E msg_id,
                                        DPARAM           param)
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_HANDLE_T    ctrl_handle = NULL;
    slim_int        ret = SLIM_E_OK;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_WMLSCONFIRM_TEXT_CTRL_ID);
        MMK_SetActiveCtrl(ctrl_handle, FALSE);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        ret = NFBrowser_ReplyRequestWMLSConfirm(MMIWWW_GETBROWSER, slim_true);
        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        ret = NFBrowser_ReplyRequestWMLSConfirm(MMIWWW_GETBROWSER, slim_false);
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return (recode);
}

/*****************************************************************************/
//  Description : Create JavaScript prompt msg win
//  Global resource dependence : 
//  Author: Kun.yu
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIWWW_OpenWMLSPromptWin(void)
{
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};
    MMI_HANDLE_T win_handle = 0;
    slim_char *in_message = PNULL;
    slim_char *in_value = PNULL;

    //SCI_TRACE_LOW:"MMIWWW_OpenWMLSPromptWin enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_10370_112_2_18_3_12_22_234,(uint8*)"");

    //初始化
    MMIWWW_GetJSPromtString(&in_message, &in_value);

    //创建win
    win_table_create.applet_handle = MMIWWW_HANDLE(MMIWWW_APPLETINSTANCE());
    win_table_create.parent_win_handle = 0;
    win_table_create.win_table_ptr = MMIWWW_WMLSPROMPT_WIN_TAB;
    win_handle = MMK_CreateWinTable( &win_table_create );
    mmiwww_HandleWWWWMLSPromptOpenWin(win_handle, in_message, in_value);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : Init ctrl data for WMLS prompt win
//  Global resource dependence : 
//  Author: Kun.yu
//  Note:
/*****************************************************************************/
LOCAL void mmiwww_HandleWWWWMLSPromptOpenWin(MMI_HANDLE_T  win_handle,
                                           slim_char    *in_message,
                                           slim_char    *in_value)
{
    MMI_HANDLE_T    ctrl_handle = NULL;
    MMI_STRING_T    text = {0};
    MMI_STRING_T    value = {0};
    //初始化
    ctrl_handle = MMK_GetCtrlHandleByWin(win_handle, MMIWWW_WMLSPROMPT_RICHTEXT_CTRL_ID);
    //in message
    if (PNULL != in_message)
    {
        MMIWWW_ConvertToMMIString(&text, in_message);
        MMIWWW_RichText_AddItem(ctrl_handle, &text, GUIRICHTEXT_FRAME_NONE, GUIRICHTEXT_TAG_NONE);
        MMIWWW_FREE_STRING(text);
    }
    //in value
    if (PNULL != in_value)
    {
        MMIWWW_ConvertToMMIString(&value, in_value);
        MMIWWW_RichText_AddItem(ctrl_handle, &value, GUIRICHTEXT_FRAME_NORMAL,
                                GUIRICHTEXT_TAG_NORMAL);
        MMIWWW_FREE_STRING(value);
    }
    //设置active控件
    MMK_SetActiveCtrl(ctrl_handle, FALSE);
    GUIRICHTEXT_SetFocusMode(ctrl_handle, GUIRICHTEXT_FOCUS_SHIFT_ADVANCED);
    MMK_PostMsg(ctrl_handle, MSG_APP_DOWN, PNULL, 0);
    return;
}

/*****************************************************************************/
//  Description : Handle WMLS prompt msg win
//  Global resource dependence : none
//  Author: Kun.yu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWWMLSPromptWin(MMI_WIN_ID_T     win_id,
                                        MMI_MESSAGE_ID_E msg_id,
                                        DPARAM           param)
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    uint8          *value_ptr = PNULL;
    slim_int        ret = SLIM_E_OK;
    slim_int in_self = MMIWWW_GETBROWSER;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        break;
    case MSG_CTL_OK:
    case MSG_APP_OK:
        //SCI_TRACE_LOW:"HandleWWWWMLSPromptWin enter OK"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_10439_112_2_18_3_12_22_235,(uint8*)"");
        value_ptr = mmiwww_HandleWMLSPromptOKWin(win_id);
        if (PNULL != value_ptr)
        {
            NFBrowser_ReplyRequestWMLSPrompt(in_self, slim_true, value_ptr);
            SCI_FREE(value_ptr);
        }
        else
        {
            NFBrowser_ReplyRequestWMLSPrompt(in_self, slim_true, PNULL);
        }
        //SCI_TRACE_LOW:"HandleWWWWMLSPromptWin enter OK ret=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_10450_112_2_18_3_12_22_236,(uint8*)"d", ret);
        MMK_CloseWin(win_id);
        break;
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        mmiwww_OpenWMLSPromptEditWin(win_id, 1);
        break;
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        //SCI_TRACE_LOW:"HandleWWWWMLSPromptWin enter CANCEL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_10460_112_2_18_3_12_23_237,(uint8*)"");
        NFBrowser_ReplyRequestWMLSPrompt(in_self, slim_false, PNULL);
        //SCI_TRACE_LOW:"HandleWWWWMLSPromptWin enter CANCEL ret=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_10462_112_2_18_3_12_23_238,(uint8*)"d", ret);
        MMK_CloseWin(win_id);
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return (recode);
}

/*****************************************************************************/
//  Description : Handle MSG_APP_OK msg for WMLS prompt win
//  Global resource dependence : 
//  Author: Kun.yu
//  Note:需调用者对返回的数据进行释放
/*****************************************************************************/
LOCAL uint8 *mmiwww_HandleWMLSPromptOKWin(MMI_WIN_ID_T  win_id)
{
    GUIRICHTEXT_ITEM_T    item = {0};/*lint !e64*/
    uint8                *ret_ptr = PNULL;
    uint16                buf_len = 0;
    MMI_HANDLE_T          ctrl_handle = NULL;
    //初始化
    ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_WMLSPROMPT_RICHTEXT_CTRL_ID);
    //获取输入的内容
    GUIRICHTEXT_GetItem(ctrl_handle, 1, &item);
    if (PNULL != item.text_data.buf.str_ptr && 0 != item.text_data.buf.len)
    {
        buf_len = (uint16)(item.text_data.buf.len * 3 + 1);
        ret_ptr = SCI_ALLOCA(buf_len+1);
        if (PNULL != ret_ptr)
        {
            //转换为utf8格式
            SCI_MEMSET(ret_ptr, 0x00, (buf_len+1));
            GUI_WstrToUTF8(ret_ptr, buf_len, item.text_data.buf.str_ptr,
                           item.text_data.buf.len);
        }
    }
    return ret_ptr;
}

/*****************************************************************************/
//  Description : Create WMLS prompt edit win
//  Global resource dependence : 
//  Author: Kun.yu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E mmiwww_OpenWMLSPromptEditWin(MMI_WIN_ID_T  win_id, uint16 index)
{
    WWW_APPLET_T *applet = (WWW_APPLET_T *)MMK_GetAppletPtrByWin(win_id);
    uint32 focus_index = index;
    MMI_WINDOW_TABLE_CREATE_T    win_table_create = {0};
    MMI_HANDLE_T                 win_handle = 0;
    MMI_HANDLE_T                 ctrl_handle = NULL;
    MMI_CONTROL_CREATE_T         create = {0};
    GUIEDIT_INIT_DATA_T      init_data = {0};
    GUIRICHTEXT_ITEM_T           item = {0};/*lint !e64*/
    GUI_RECT_T                   rect = MMITHEME_GetClientRect();

    //初始化edit控件数据
    //SCI_TRACE_LOW:"mmiwww_OpenWMLSPromptEditWin enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_10518_112_2_18_3_12_23_239,(uint8*)"");

    ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_WMLSPROMPT_RICHTEXT_CTRL_ID);
    GUIRICHTEXT_GetItem(ctrl_handle, index, &item);
    init_data.type = GUIEDIT_TYPE_TEXT;
    init_data.both_rect.h_rect = init_data.both_rect.v_rect = rect;
    init_data.str_max_len = WWW_MAX_JSPROMPT_STRING_LEN;

    //创建win
    win_table_create.applet_handle = MMIWWW_HANDLE(MMIWWW_APPLETINSTANCE());
    win_table_create.parent_win_handle = 0;
    win_table_create.win_table_ptr = MMIWWW_WMLSPROMPT_EDIT_WIN_TAB;
    win_table_create.add_data_ptr = (ADD_DATA)focus_index;
    win_handle = MMK_CreateWinTable( &win_table_create );

    //创建edit控件
    create.ctrl_id = MMIWWW_WMLSPROMPT_EDIT_CTRL_ID;
    create.guid = SPRD_GUI_EDITBOX_ID;
    create.parent_win_handle = win_handle;
    create.init_data_ptr = &init_data;
    MMK_CreateControl(&create);

    ctrl_handle = MMK_GetCtrlHandleByWin(MMK_GetWinHandle(MMIWWW_HANDLE(applet), MMIWWW_WMLSPROMPT_EDIT_WIN_ID), MMIWWW_WMLSPROMPT_EDIT_CTRL_ID); 

    //set string
    if (PNULL != item.text_data.buf.str_ptr && 0 != item.text_data.buf.len)
    {
        GUIEDIT_SetString(ctrl_handle, item.text_data.buf.str_ptr, item.text_data.buf.len);
    }
        
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : handle messages of WMLS edit win
//  Global resource dependence : none
//  Author: Kun.yu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWWMLSPromptEditWin(MMI_WIN_ID_T     win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM           param)
{
    MMI_RESULT_E          recode = MMI_RESULT_TRUE;
    MMI_STRING_T          edit_string = {0};
    MMI_HANDLE_T          ctrl_handle = NULL;
    GUIRICHTEXT_ITEM_T    new_item = {0};/*lint !e64*/
    GUIRICHTEXT_ITEM_T    org_item = {0};/*lint !e64*/
    uint32                index = 0;
    WWW_APPLET_T         *applet = PNULL;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_WMLSPROMPT_EDIT_CTRL_ID);
        MMK_SetActiveCtrl(ctrl_handle, FALSE);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        //get data from edit ctrl
        ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_WMLSPROMPT_EDIT_CTRL_ID);
        GUIEDIT_GetString(ctrl_handle, &edit_string);
        // set to richtext ctrl
        applet = (WWW_APPLET_T *)MMK_GetAppletPtrByWin(win_id);
        ctrl_handle = MMK_GetCtrlHandleByWin(MMK_GetWinHandle(MMIWWW_HANDLE(applet), MMIWWW_WMLSPROMPT_WIN_ID), MMIWWW_WMLSPROMPT_RICHTEXT_CTRL_ID);
        index = (uint32)MMK_GetWinAddDataPtr(win_id);
        GUIRICHTEXT_GetItem(ctrl_handle, (uint16)index, &org_item);
        SCI_MEMCPY(&new_item, &org_item, sizeof(GUIRICHTEXT_ITEM_T));
        new_item.text_data.buf.len = edit_string.wstr_len;
        new_item.text_data.buf.str_ptr = edit_string.wstr_ptr;
        new_item.text_type = GUIRICHTEXT_TEXT_BUF;
        GUIRICHTEXT_ModifyItem(ctrl_handle, (uint16)index, &new_item);
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
    return (recode);
}

/*****************************************************************************/
//  Description : Open WWW Authorize
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIWWW_OpenAuthWin(void)
{
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};
    WWW_APPLET_T *www_applet = MMIWWW_APPLETINSTANCE();

    win_table_create.applet_handle = MMIWWW_HANDLE(www_applet);
    win_table_create.parent_win_handle = 0;
    win_table_create.win_table_ptr = MMIWWW_AUTH_WIN_TAB;

    MMK_CreateWinTable(&win_table_create);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : handle message of WWW Auth win
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWAuthWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    MMI_CTRL_ID_T active_ctrl_id = 0;
    MMI_HANDLE_T account_handle = 0;
    MMI_HANDLE_T password_handle = 0;
    MMI_STRING_T title_string = {0};
    MMI_STRING_T account_value_string = {0};
    MMI_STRING_T password_value_string = {0};
    MMI_HANDLE_T account_value_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_AUTH_ACCOUNT_VALUE_CTRL_ID);
    MMI_HANDLE_T password_value_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_AUTH_PASSWORD_VALUE_CTRL_ID);
    slim_char *in_url = PNULL;
    slim_char *in_realm = PNULL;
    slim_char *in_user = PNULL;
    slim_char *in_pass = PNULL;

    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;

    //SCI_TRACE_LOW:"mmiwww_wintab.c:HandleWWWAuthWinMsg() msg_id=0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_10649_112_2_18_3_12_23_240,(uint8*)"d", msg_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIEDIT_SetPasswordStyle(password_value_handle, GUIEDIT_PASSWORD_STYLE_ALL);
        GUIEDIT_SetIm(password_value_handle, GUIIM_TYPE_ENGLISH | GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL, GUIIM_TYPE_ABC);//lint !e655
        GUIEDIT_SetIm(account_value_handle, GUIIM_TYPE_ENGLISH | GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL, GUIIM_TYPE_ABC);//lint !e655
        MMIWWW_GetAuthParam(&in_url, &in_realm, &in_user, &in_pass);

        if (PNULL != in_realm)
        {
            uint16 realm_len = 0;
            wchar *realm_ptr = PNULL;

            realm_len = strlen((char *)in_realm);

            realm_ptr = SCI_ALLOCA((realm_len + 1) * sizeof(wchar));

            if (PNULL == realm_ptr)
            {
                break;
            }

            SCI_MEMSET(realm_ptr, 0x00, ((realm_len + 1) * sizeof(wchar)));

            GUI_UTF8ToWstr(realm_ptr, realm_len, in_realm, realm_len);

            title_string.wstr_ptr = (wchar *)realm_ptr;
            title_string.wstr_len = realm_len;

            GUIWIN_SetTitleText(win_id, title_string.wstr_ptr, title_string.wstr_len, FALSE);

            SCI_FREE(realm_ptr);
        }

        account_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_AUTH_ACCOUNT_CTRL_ID);
        password_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_AUTH_PASSWORD_CTRL_ID);

        GUILABEL_SetTextById(account_handle, TXT_ACCOUNT, FALSE);
        GUILABEL_SetTextById(password_handle, TXT_COMMON_PASSWORD, FALSE);

        if (PNULL != in_user)
        {
            uint16 user_len = 0;
            wchar *user_ptr = PNULL;

            user_len = strlen((char *)in_user);

            user_ptr = SCI_ALLOCA((user_len + 1) * sizeof(wchar));

            if (PNULL == user_ptr)
            {
                break;
            }

            SCI_MEMSET(user_ptr, 0x00, ((user_len + 1) * sizeof(wchar)));

            GUI_UTF8ToWstr(user_ptr, user_len, in_user, user_len);

            account_value_string.wstr_ptr = (wchar *)user_ptr;
            account_value_string.wstr_len = user_len;

            GUIEDIT_SetString(account_value_handle, account_value_string.wstr_ptr, account_value_string.wstr_len);

            SCI_FREE(user_ptr);
        }

        if (PNULL != in_pass)
        {
            uint16 pass_len = 0;
            wchar *pass_ptr = PNULL;

            pass_len = strlen((char *)in_pass);

            pass_ptr = SCI_ALLOCA((pass_len + 1) * sizeof(wchar));

            if (PNULL == pass_ptr)
            {
                break;
            }

            SCI_MEMSET(pass_ptr, 0x00, ((pass_len + 1) * sizeof(wchar)));

            GUI_UTF8ToWstr(pass_ptr, pass_len, in_pass, pass_len);

            password_value_string.wstr_ptr = (wchar *)pass_ptr;
            password_value_string.wstr_len = pass_len;

            GUIEDIT_SetString(password_value_handle, password_value_string.wstr_ptr, password_value_string.wstr_len);
            SCI_FREE(pass_ptr);
        }
        
        MMK_SetAtvCtrl(win_id, account_value_handle);
        break;

    case MSG_APP_UP:
    case MSG_APP_DOWN:
        active_ctrl_id = MMK_GetActiveCtrl(win_id);

        if (account_value_handle == active_ctrl_id)
        {
            active_ctrl_id = password_value_handle;
        }
        else
        {
            active_ctrl_id = account_value_handle;
        }
        MMK_SetAtvCtrl(win_id, active_ctrl_id);
        
        MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
        break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
        GUIEDIT_GetString(account_value_handle,&account_value_string);
        GUIEDIT_GetString(password_value_handle, &password_value_string);
        if (0 == account_value_string.wstr_len)
        {
            MMIPUB_OpenAlertWarningWin(TXT_DL_DOWNLOAD_AUTH_ACCOUNT_EMPTY);
        }
        else if (0 == password_value_string.wstr_len)
        {
            MMIPUB_OpenAlertWarningWin(TXT_DL_DOWNLOAD_AUTH_PASSWORD_EMPTY);
        }
        else
        {
            MMIWWW_ReplyRequestAuth(TRUE, &account_value_string, &password_value_string);
            MMK_CloseWin(win_id);
        }
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMIWWW_ReplyRequestAuth(FALSE, &account_value_string, &password_value_string);
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        break;

    default:
        //SCI_TRACE_LOW:"mmiwww_wintab.c:HandleWWWAuthWinMsg() msg_id=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_10791_112_2_18_3_12_23_241,(uint8*)"d", msg_id);
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

#if 0//@fen.xie
/*****************************************************************************/
//  Description : handle message of CC select
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWCCSelectWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMIWWW_CC_INFO_T *cc_info = PNULL;

    cc_info = (MMIWWW_CC_INFO_T *)MMK_GetWinAddDataPtr(win_id);
    if (PNULL == cc_info)
    {
        return recode;
    }

    switch( msg_id )
    {       
    case MSG_OPEN_WINDOW:
        {
            MMI_HANDLE_T ctrl_handle = PNULL;
            GUIMENU_DYNA_ITEM_T node_item = {0};
            uint16 node_index = 0;
            MMI_STRING_T string = {0};
            //GUI_RECT_T rect = {0, MMI_CLIENT_RECT_TOP, MMI_MAINSCREEN_RIGHT_MAX_PIXEL, MMI_CLIENT_RECT_BOTTOM};
            GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);

            GUIMENU_CreatDynamic(&both_rect, win_id, MMIWWW_MENU_CC_SELECT_CTRL_ID, GUIMENU_STYLE_THIRD);
            ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_MENU_CC_SELECT_CTRL_ID);

            SCI_MEMSET(&string, 0x00, sizeof(MMI_STRING_T));
            MMIRES_GetText(TXT_CALL, ctrl_handle, &string);
            GUIMENU_SetMenuTitle(&string, ctrl_handle);

            if ((cc_info->cc_option & MMIWWW_CC_OPTION_VOICE) != 0)
            {

                SCI_MEMSET(&string, 0x00, sizeof(MMI_STRING_T));
                MMIRES_GetText(TXT_CALL_AUDIO, ctrl_handle, &string);
                node_item.item_text_ptr = &string;
                node_item.select_icon_id = 0;
                GUIMENU_InsertNode(node_index++, ID_WWW_CC_VOICE, 0, &node_item, ctrl_handle);
            }

#ifdef VT_SUPPORT
            if ((cc_info->cc_option & MMIWWW_CC_OPTION_VIDEO) != 0)
            {
                SCI_MEMSET(&string, 0x00, sizeof(MMI_STRING_T));
                MMIRES_GetText(TXT_CALL_VIDEO, ctrl_handle, &string);
                node_item.item_text_ptr = &string;
                node_item.select_icon_id = 0;
                GUIMENU_InsertNode(node_index++, ID_WWW_CC_VIDEO, 0, &node_item, ctrl_handle);
            }
#endif

            if ((cc_info->cc_option & MMIWWW_CC_OPTION_IP) != 0)
            {
                SCI_MEMSET(&string, 0x00, sizeof(MMI_STRING_T));
                MMIRES_GetText(TXT_CALL_IPCALL, ctrl_handle, &string);
                node_item.item_text_ptr = &string;
                node_item.select_icon_id = 0;
                GUIMENU_InsertNode(node_index++, ID_WWW_CC_IP, 0, &node_item, ctrl_handle);
            }

            MMK_SetActiveCtrl(ctrl_handle, FALSE);
        }
        break;

    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        {
            uint32 call_type = CC_CALL_NORMAL_CALL;
            uint32 menu_id = 0;
            MMI_HANDLE_T ctrl_handle = PNULL;

            ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_MENU_CC_SELECT_CTRL_ID);
            menu_id = GUIMENU_GetCurNodeId(ctrl_handle);

            switch(menu_id) {
            case ID_WWW_CC_IP:
                call_type = CC_CALL_IP_CALL;
                break;
            case ID_WWW_CC_VIDEO:
                call_type = CC_CALL_VIDEO_CALL;
                break;

            case ID_WWW_CC_VOICE:
            default:
                call_type = CC_CALL_NORMAL_CALL;
                break;
            }

            MMIAPICC_MakeCall(MN_DUAL_SYS_MAX, cc_info->phone_num_ptr, strlen((char *)cc_info->phone_num_ptr), PNULL, PNULL, CC_CALL_SIM_MAX, call_type, PNULL);
            MMK_CloseWin(win_id);
        }

        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        if (PNULL != cc_info->phone_num_ptr)
        {
            SCI_FREE(cc_info->phone_num_ptr);
        }
        SCI_FREE(cc_info);
       break;

    default:
        break;
    }

    return recode;
}
#endif

/*****************************************************************************/
//  Description : handle message of CC select
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWHomepageSetWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    switch( msg_id )
    {
    case MSG_OPEN_WINDOW:
        {
            MMI_HANDLE_T lable_handle = PNULL;
            MMI_HANDLE_T text_handle = PNULL;
            char temp[MAX_WEB_URL_LEN + 1] = {0};
            wchar *hompage = PNULL;
            uint16 str_len = 0;

            GUIFORM_SetStyle(MMK_GetCtrlHandleByWin(win_id,MMIWWW_SETTINGS_HOMEPAGE_FORM1_CTRL_ID),GUIFORM_STYLE_UNIT);

            MMIWWW_GetHomepageURL(temp, MAX_WEB_URL_LEN);
            str_len = strlen(temp);
   
            hompage = SCI_ALLOCA((str_len + 1) * sizeof(wchar));

            if (PNULL == hompage)
            {
                //SCI_TRACE_LOW:"HandleWWWHomepageSetWinMsg No mem"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_10948_112_2_18_3_12_24_242,(uint8*)"");
                break;
            }

            SCI_MEMSET(hompage, 0, str_len + 1);
            str_len = GUI_UTF8ToWstr(hompage, str_len, (uint8 *)temp, str_len);

            lable_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_HOMEPAGE_LABLE_CTRL_ID);
            text_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_HOMEPAGE_TEXT_CTRL_ID);

            GUILABEL_SetTextById(lable_handle, TXT_COMMON_URL, FALSE);
            GUIEDIT_SetIm(text_handle, GUIIM_TYPE_ENGLISH | GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL, GUIIM_TYPE_ABC);//lint !e655
            GUIEDIT_SetString(text_handle, hompage, str_len);
            SCI_FREE(hompage);

            MMK_SetActiveCtrl(text_handle, FALSE);
        }
        break;

    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        {
            MMI_HANDLE_T text_handle = PNULL;
            MMI_STRING_T  string_info = {0};
            char *temp = PNULL;
            uint16 size = 0;

            text_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SETTINGS_HOMEPAGE_TEXT_CTRL_ID);
            GUIEDIT_GetString(text_handle, &string_info);

            size = string_info.wstr_len * 3 + 1;

            temp = SCI_ALLOCA(size);

            if (PNULL == temp)
            {
                break;
            }
            SCI_MEMSET(temp, 0, size);

            if (PNULL != string_info.wstr_ptr)
            {
                GUI_WstrToUTF8((uint8 *)temp, string_info.wstr_len, string_info.wstr_ptr, string_info.wstr_len);
            }
            MMIWWW_SetHomepageURL(temp);
            SCI_FREE(temp);
            MMIPUB_OpenAlertSuccessWin(TXT_WWW_BOOKMARK_SAVE_SUCCESS);
            MMK_CloseWin(win_id);
        }
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : Check need stop applet
//  Global resource dependence : 
//  Author: wancan.you
//  Note: depend on windows (Mainmenu Browser Changebookmark)
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_IsNeedStopApplet(void)
{
    WWW_APPLET_T *applet = MMIWWW_APPLETINSTANCE();
    MMI_HANDLE_T applet_handle = PNULL;
    MMI_HANDLE_T mainmenu_handle = PNULL;
    MMI_HANDLE_T browser_handle = PNULL;
    MMI_HANDLE_T changeBM_handle = PNULL;
    BOOLEAN ret = TRUE;

    if (PNULL != applet)
    {
        applet_handle = MMIWWW_HANDLE(applet);
        mainmenu_handle = MMK_GetWinHandle(applet_handle, MMIWWW_MAINMENU_WIN_ID);
        browser_handle = mmiwww_GetBrowserWinhandle();
        changeBM_handle = MMK_GetWinHandle(applet_handle, MMIWWW_INPUT_BOOKMARK_WIN_ID);

        if (MMK_IsOpenWin(mainmenu_handle) || MMK_IsOpenWin(browser_handle) || MMK_IsOpenWin(changeBM_handle))
        {
            ret = FALSE;
        }
    }
    else
    {
        ret = FALSE;
    }

    return ret;
}

/*****************************************************************************/
//  Description : Tidy all windows except Mainmenu and browser window
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_TidyExistedWin(void)
{
    WWW_APPLET_T *www_applet_ptr = MMIWWW_APPLETINSTANCE();
    MMI_HANDLE_T applet_handle = PNULL;
    MMI_WIN_ID_T win_id = MMIWWW_WIN_ID_START;
    MMI_HANDLE_T win_handle = PNULL;

    if (PNULL != www_applet_ptr)
    {
        applet_handle = MMIWWW_HANDLE(www_applet_ptr);

        for (win_id = MMIWWW_WIN_ID_START + 1; win_id <  MMIWWW_MAX_WIN_ID; win_id++)
        {
            if (MMIWWW_MAINMENU_WIN_ID != win_id && MMIWWW_BROWSER_WIN_ID != win_id && MMIWWW_BOOKMARK_WIN_ID != win_id && MMIWWW_HISTORY_WIN_ID != win_id)
            {
                win_handle = MMK_GetWinHandle(applet_handle, win_id);
                if (MMK_IsOpenWin(win_handle))
                {
                    MMK_CloseWin(win_handle);
                }
            }
        }
    }
}

//fen.xie for bookmark
/*****************************************************************************/
//  Description : Create bookmark sub menu
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWWW_OpenWWWBookmarkOptWin(BOOLEAN is_web_open)
{
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};
    WWW_APPLET_T *www_applet = MMIWWW_APPLETINSTANCE();

    win_table_create.applet_handle = MMIWWW_HANDLE(www_applet);
    win_table_create.parent_win_handle = 0;
    //web open需新增menu item:回到网页浏览
    win_table_create.win_table_ptr = is_web_open ? MMIWWW_BOOKMARK_OPT_WIN_TAB2 : MMIWWW_BOOKMARK_OPT_WIN_TAB1;
    win_table_create.add_data_ptr = (ADD_DATA)(is_web_open ? MENU_WWW_BOOKMARK_OPT2 : MENU_WWW_BOOKMARK_OPT1);

    MMK_CreateWinTable( &win_table_create );

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : Create sub menu of bookmark detail
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWWW_OpenWWWBookmarkDetailSubMenuWin(void)
{
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};
    WWW_APPLET_T *www_applet = MMIWWW_APPLETINSTANCE();
    
    win_table_create.applet_handle = MMIWWW_HANDLE(www_applet);
    win_table_create.parent_win_handle = 0;
    win_table_create.win_table_ptr = MMIWWW_BOOKMARKDETAILSUBMENU_WIN_TAB;
    
    MMK_CreateWinTable( &win_table_create );
    
    return MMI_RESULT_TRUE;
}
/*****************************************************************************/
//     Description : bookmark url input
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWWW_OpenInputBookmarkWin(MMIWWW_BM_CHANGE_TYPE_E change_type)
{
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};
    WWW_APPLET_T *www_applet = MMIWWW_APPLETINSTANCE();
    MMI_HANDLE_T win_handle = PNULL;

    win_table_create.applet_handle = MMIWWW_HANDLE(www_applet);
    win_table_create.parent_win_handle = 0;
    win_table_create.win_table_ptr = MMIWWW_INPUTBOOKMARK_WIN_TAB;
    win_table_create.add_data_ptr = (ADD_DATA)(change_type);

    win_handle = MMK_GetWinHandle(MMIWWW_HANDLE(www_applet), MMIWWW_INPUT_BOOKMARK_WIN_ID);

    /* Renew a window */
    if (MMK_IsOpenWin(win_handle))
    {
        MMK_CloseWin(win_handle);
    }
    MMK_CreateWinTable( &win_table_create );

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : show bookmark
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWWW_ShowBookmarkWin(void)
{
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};
    WWW_APPLET_T *www_applet = MMIWWW_APPLETINSTANCE();

    win_table_create.applet_handle = MMIWWW_HANDLE(www_applet);
    win_table_create.parent_win_handle = 0;
    win_table_create.win_table_ptr = MMIWWW_SHOWBOOKMARK_WIN_TAB;

    MMK_CreateWinTable( &win_table_create );

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : some message of  main menu of browser can't handled within bookmark moving state
//  Global resource dependence : none
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleWWWMainMenuInBMMove(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    BOOLEAN result = TRUE;

    if (!s_is_in_bookmark_moving)
    {
        return FALSE;
    }

    switch(msg_id)
    {
    case MSG_KEYDOWN_LEFT:
    case MSG_KEYDOWN_RIGHT:
        break;
        
#ifdef WIN32
    case MSG_APP_1:
    case MSG_APP_2:
    case MSG_APP_3:
    case MSG_APP_4:
    case MSG_APP_5:
        break;
#endif

    default:
        result = FALSE;
        break;
    }

    return result;
}

/*****************************************************************************/
//  Description : Add or Edit bookmark
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_ChangeBookmarkWin(MMIWWW_BM_CHANGE_TYPE_E change_type)
{
    //限制目录数量
    if ((MMIWWW_BM_CHANGE_ADD_DIR == change_type) && (MMIWWW_BookmarkGetFolderNum() >= MMIWWW_MAX_BOOKMARK_DIR_NUM))
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_FOLDER_FULL);
        return;
    }

    //限制书签数量
    if(((MMIWWW_BM_CHANGE_ADD_CUR_PAGE == change_type) || (MMIWWW_BM_CHANGE_ADD_URL == change_type) || (MMIWWW_BM_CHANGE_ADD_NULL == change_type))
        && (MMIWWW_GetBookmarkTotalNumber() >= MMIWWW_MAX_BOOKMARK_NUM))
    {
        MMIPUB_OpenAlertWarningWin(TXT_WWW_BOOKMARK_FULL);
        
        if (MMIWWW_BM_CHANGE_ADD_URL == change_type)
        {
            MMI_HANDLE_T applet_handle = PNULL;
            uint8 * added_url = PNULL;
            uint8 * added_title = PNULL;

            applet_handle = MMIWWW_HANDLE(MMIWWW_APPLETINSTANCE());
            added_url = MMIWWW_NetfrontGetAddedUrl();
            added_title  = MMIWWW_NetfrontGetAddedTitle();

            if (PNULL != added_url)
            {
                SCI_FREE(added_url);
                MMIWWW_NetfrontSetAddedUrl(PNULL);
            }
            if (PNULL != added_title)
            {
                SCI_FREE(added_title);
                MMIWWW_NetfrontSetAddedTitle(PNULL);
            }
            if (!(MMK_IsOpenWin(MMK_GetWinHandle(applet_handle, MMIWWW_MAINMENU_WIN_ID)) || MMK_IsOpenWin(MMK_GetWinHandle(applet_handle, MMIWWW_BROWSER_WIN_ID))))
            {
                MMIAPIWWW_StopWWW();
            }
        }
    }
    else
    {
        MMIWWW_OpenInputBookmarkWin(change_type);
    }
}
/*****************************************************************************/
//  Description : handle message of bookmark list
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWBookmarkWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_HANDLE_T applet_handle = MMIWWW_HANDLE((WWW_APPLET_T *)MMK_GetAppletPtrByWin(win_id));
    WWW_BOOKMARK_T *bookmark_info_ptr = MMIWWW_BOOKMARK();
    MMI_TEXT_ID_T text1_id = STXT_OPTION;
    MMI_TEXT_ID_T text2_id = TXT_NULL;
    MMI_TEXT_ID_T text3_id = TXT_WWW_EXIT;
    LOCAL uint16 pre_path_index = 0;//用于记录之前的焦点项

    //SCI_TRACE_LOW:"mmiwww_wintab.c:HandleWWWBookmarkWinMsg() msg_id=0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_11259_112_2_18_3_12_24_243,(uint8*)"d", msg_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUI_BOTH_RECT_T bm_list_rect = {0};

            bm_list_rect.v_rect.left = MMIWWW_TAB_LIST_LEFT;
            bm_list_rect.v_rect.top = MMIWWW_TAB_LIST_TOP;
            bm_list_rect.v_rect.right = MMIWWW_TAB_LIST_RIGHT;
            bm_list_rect.v_rect.bottom = MMIWWW_TAB_LIST_BOTTOM;
            bm_list_rect.h_rect.left = MMIWWW_TAB_LIST_LEFT;
            bm_list_rect.h_rect.top = MMIWWW_TAB_LIST_TOP_H;
            bm_list_rect.h_rect.right = MMIWWW_TAB_LIST_RIGHT_H;
            bm_list_rect.h_rect.bottom = MMIWWW_TAB_LIST_BOTTOM_H;

        pre_path_index = 0;//init
        GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIWWW_BOOKMARK_LIST_CTRL_ID), &bm_list_rect);
        MMIWWW_LoadBookmarkFileArray(BOOKMARK_STEP_TYPE_ROOT, 0, FUNC_FIND_NULL);

        MMIWWW_AppendListboxitem(win_id, MMIWWW_BOOKMARK_LIST_CTRL_ID, (uint16)MMIWWW_GetBookmarkNumber(), TXT_WWW_BOOKMARK_EMPTY, STXT_OPTION, TXT_WWW_EXIT);
        GUILIST_SetCycScroll(MMK_GetCtrlHandleByWin(win_id, MMIWWW_BOOKMARK_LIST_CTRL_ID), FALSE);

#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
        }

        break;

    case MSG_GET_FOCUS:
        MMIWWW_AppendListboxitem(win_id, MMIWWW_BOOKMARK_LIST_CTRL_ID, (uint16)MMIWWW_GetBookmarkNumber(), TXT_WWW_BOOKMARK_EMPTY, STXT_OPTION, TXT_WWW_EXIT);
        
        if (MMIWWW_CTRL_BOOKMARK_LIST == MMIWWW_NetfrontGetMainMenuFocusCtrl())
        {
            MMK_SetActiveCtrl(MMK_GetCtrlHandleByWin(win_id, MMIWWW_BOOKMARK_LIST_CTRL_ID), TRUE);
        }
        break;

    case MSG_LOSE_FOCUS:
        if (s_is_in_bookmark_moving)//取消"移动至"的操作
        {
            //reset file_array为根目录
            GUILIST_SetCurItemIndex(MMK_GetCtrlHandleByWin(win_id, MMIWWW_BOOKMARK_LIST_CTRL_ID), 0);
            MMIWWW_LoadBookmarkFileArray(BOOKMARK_STEP_TYPE_ROOT, 0, FUNC_FIND_NULL);
            s_is_in_bookmark_moving = FALSE;
        }
        break;

    case MSG_CTL_LIST_NEED_ITEM_DATA:
        {
            MMI_STRING_T title_string_info = {0};
            GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;
            uint16 array_index = 0;
            FILEARRAY_TYPE_E type = FILEARRAY_TYPE_NULL;
            MMI_IMAGE_ID_T image_id = 0;
            wchar root_flag[] = {0x2F, 0x00};//"/" 

            if (PNULL == need_item_data_ptr)
            {
                break;
            }

            array_index = GetBookmarkArrayIndex(need_item_data_ptr->item_index);
            
            if ((1 == bookmark_info_ptr->file_array_step) && (0 == need_item_data_ptr->item_index))//二级目录的top 项"..."
            {
                MMI_GetLabelTextByLang(TXT_WWW_FOLDER_UP, &title_string_info);
                image_id = IMAGE_FMM_FOLDER_ICON;
                text2_id = TXT_COMM_OPEN;

                if (s_is_in_bookmark_moving)
                {
                    text1_id = STXT_SELECT;
                    text2_id = TXT_NULL;
                    text3_id = STXT_CANCEL;
                }
                MMIWWW_AppendTitleItemData(MMK_GetCtrlHandleByWin(win_id, MMIWWW_BOOKMARK_LIST_CTRL_ID), 
                                need_item_data_ptr->item_index, title_string_info, image_id,
                                text1_id, text2_id, text3_id );
            }
            else if (s_is_in_bookmark_moving && (0 == need_item_data_ptr->item_index))//增加moving  时root path 的显示
            {                
                title_string_info.wstr_ptr = root_flag;
                title_string_info.wstr_len = MMIAPICOM_Wstrlen(root_flag);
              
                image_id = IMAGE_FMM_FOLDER_ICON;

                text1_id = STXT_SELECT;
                text2_id = TXT_NULL;
                text3_id = STXT_CANCEL;

                MMIWWW_AppendTitleItemData(MMK_GetCtrlHandleByWin(win_id, MMIWWW_BOOKMARK_LIST_CTRL_ID), 
                            need_item_data_ptr->item_index, title_string_info, image_id,
                            text1_id, text2_id, text3_id );
            }
            else
            {
                type =MMIWWW_GetBookmarkTitle(array_index, &title_string_info);

                if (PNULL != title_string_info.wstr_ptr)
                {
                    if (FILEARRAY_TYPE_FOLDER == type)
                    {
                        image_id = IMAGE_FMM_FOLDER_ICON;
                        text2_id = TXT_COMM_OPEN;
                   }
                    else  if (FILEARRAY_TYPE_FILE == type)
                    {
                        image_id = IMAGE_WWW_BOOKMARK_CUSTOMIZE;
                        text2_id = TXT_COMMON_CONNECT_COMN;
                    }

                    if (s_is_in_bookmark_moving)
                    {
                        text1_id = STXT_SELECT;
                        text2_id = TXT_NULL;
                        text3_id = STXT_CANCEL;
                    }

                    MMIWWW_AppendTitleItemData(MMK_GetCtrlHandleByWin(win_id, MMIWWW_BOOKMARK_LIST_CTRL_ID), 
                                    need_item_data_ptr->item_index, title_string_info, image_id,
                                    text1_id, text2_id, text3_id );

                    SCI_FREE(title_string_info.wstr_ptr);
                }
                else
                {
                    //SCI_TRACE_LOW:"mmiwww_wintab.c:HandleWWWBookmarkWinMsg() String is NULL"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_11388_112_2_18_3_12_24_244,(uint8*)"");
                }
            }
        }
        break;

    case MSG_NOTIFY_GET_ACTIVE:
		{
			uint32 tmp_win_id = MMIWWW_BOOKMARK_WIN_ID;
			MMIWWW_NetfrontSetMainMenuFocusCtrl(MMIWWW_CTRL_BOOKMARK_LIST);
			MMK_PostMsg(MMK_GetWinHandle(applet_handle, MMIWWW_MAINMENU_WIN_ID), MSG_FULL_PAINT, (DPARAM)&tmp_win_id, sizeof(uint32));

		}
        return MMI_RESULT_FALSE;

    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        if (s_is_in_bookmark_moving)//同MSG_APP_OK开始move操作
        {
            uint16 cur_index = GUILIST_GetCurItemIndex(MMK_GetCtrlHandleByWin(win_id, MMIWWW_BOOKMARK_LIST_CTRL_ID));
            
            BookmarkBeginMoveTo(cur_index);
        }
        else
        {
            uint16 array_index = 0;
            FILEARRAY_DATA_T f_array_data = {0};
            uint16 cur_index = GUILIST_GetCurItemIndex(MMK_GetCtrlHandleByWin(win_id, MMIWWW_BOOKMARK_LIST_CTRL_ID));
            MMIFMM_FUNC_TYPE_E type = FUNC_FIND_NULL;//默认显示

            array_index = GetBookmarkArrayIndex(cur_index);
            if ((1 == bookmark_info_ptr->file_array_step) && 0 == cur_index)/*返回到上级目录(当前为根目录)*/
            {
                MMIWWW_LoadBookmarkFileArray(BOOKMARK_STEP_TYPE_PRE, array_index, type);

                MMIWWW_AppendListboxitem(win_id, MMIWWW_BOOKMARK_LIST_CTRL_ID, (uint16)bookmark_info_ptr->cur_index_num, TXT_WWW_BOOKMARK_EMPTY, STXT_OPTION, TXT_WWW_EXIT);
                GUILIST_SetCurItemIndex(MMK_GetCtrlHandleByWin(win_id, MMIWWW_BOOKMARK_LIST_CTRL_ID), pre_path_index);
 
                MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
            }
            else
            {
            
                MMIAPIFILEARRAY_Read(bookmark_info_ptr->file_array, array_index, &f_array_data);
                
                if (FILEARRAY_TYPE_FOLDER == f_array_data.type)//打开文件夹
                {
                    pre_path_index = cur_index;
                    MMIWWW_LoadBookmarkFileArray(BOOKMARK_STEP_TYPE_NEXT, array_index, type);
           
                    MMIWWW_AppendListboxitem(win_id, MMIWWW_BOOKMARK_LIST_CTRL_ID, (uint16)bookmark_info_ptr->cur_index_num, TXT_WWW_BOOKMARK_EMPTY, STXT_OPTION, TXT_WWW_EXIT);
                    GUILIST_SetCurItemIndex(MMK_GetCtrlHandleByWin(win_id, MMIWWW_BOOKMARK_LIST_CTRL_ID), 1);

                    MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);

                }
                else  if (FILEARRAY_TYPE_FILE == f_array_data.type)//选中当前的书签进行链接
                {
                    MMIWWW_Bookmark_GotoURL(array_index);
                }
            }
        }
        break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_MENU:
        if (s_is_in_bookmark_moving)
        {
            uint16 cur_index = GUILIST_GetCurItemIndex(MMK_GetCtrlHandleByWin(win_id, MMIWWW_BOOKMARK_LIST_CTRL_ID));
            
            BookmarkBeginMoveTo(cur_index);
        }
        else//enter bookmark opt window
        {
            uint16 cur_index = GUILIST_GetCurItemIndex(MMK_GetCtrlHandleByWin(win_id, MMIWWW_BOOKMARK_LIST_CTRL_ID));
            FILEARRAY_DATA_T f_array_data = {0};

            SCI_MEMSET(&s_bookmark_cur_index_info, 0x00, sizeof(s_bookmark_cur_index_info));
            
            if ((1 == bookmark_info_ptr->file_array_step) && (0 == cur_index))
            {
                //do nothing
            }
            else
            {
                s_bookmark_cur_index_info.array_index  = GetBookmarkArrayIndex(cur_index);
                if(MMIAPIFILEARRAY_Read(bookmark_info_ptr->file_array, GetBookmarkCurIndex(), &f_array_data))
                {
                    s_bookmark_cur_index_info.type = f_array_data.type;
                }
                else//error
                {
                    s_bookmark_cur_index_info.type = FILEARRAY_TYPE_NULL;
                }
            }
            MMIWWW_OpenWWWBookmarkOptWin(MMK_IsOpenWin(mmiwww_GetBrowserWinhandle()));
        }
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        if (s_is_in_bookmark_moving)//取消"move"操作
        {
            //reset file_array为根目录
            MMIWWW_LoadBookmarkFileArray(BOOKMARK_STEP_TYPE_ROOT, 0, FUNC_FIND_NULL);
            s_is_in_bookmark_moving = FALSE;

            //refresh the window
            MMIWWW_AppendListboxitem(win_id, MMIWWW_BOOKMARK_LIST_CTRL_ID, (uint16)bookmark_info_ptr->cur_index_num, TXT_WWW_BOOKMARK_EMPTY, STXT_OPTION, TXT_WWW_EXIT);
            GUILIST_SetCurItemIndex(MMK_GetCtrlHandleByWin(win_id, MMIWWW_BOOKMARK_LIST_CTRL_ID), 0);
            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
        }
        else
        {
            MMK_SetActiveCtrl(MMK_GetCtrlHandleByWin(MMK_GetWinHandle(applet_handle, MMIWWW_MAINMENU_WIN_ID), MMIWWW_MAINMENU_TAB_CTRL_ID), FALSE);
            MMIWWW_NetfrontSetMainMenuFocusCtrl(MMIWWW_CTRL_SEARCH_LABEL);
            MMK_PostMsg(MMK_GetWinHandle(applet_handle, MMIWWW_MAINMENU_WIN_ID), MSG_FULL_PAINT, PNULL, 0);
        }
        break;

    case MSG_NOTIFY_LIST_HEAD:
        if (s_is_in_bookmark_moving)//取消"移动至"的操作
        {
            break;
        }

        MMK_SetActiveCtrl(MMK_GetCtrlHandleByWin(MMK_GetWinHandle(applet_handle, MMIWWW_MAINMENU_WIN_ID), MMIWWW_MAINMENU_TAB_CTRL_ID), FALSE);
        MMIWWW_NetfrontSetMainMenuFocusCtrl(MMIWWW_CTRL_SEARCH_LABEL);
        MMK_PostMsg(MMK_GetWinHandle(applet_handle, MMIWWW_MAINMENU_WIN_ID), MSG_FULL_PAINT, PNULL, 0);
        break;

    case MSG_NOTIFY_LIST_TAIL:
        {
            MMI_HANDLE_T ctrl_handle = PNULL;

            ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_BOOKMARK_LIST_CTRL_ID);
            if (GUILIST_GetTotalItemNum(ctrl_handle) > 1)
            {
                GUILIST_SetCurItemIndex(ctrl_handle, 0);
                MMK_PostMsg(MMK_GetWinHandle(applet_handle, MMIWWW_MAINMENU_WIN_ID), MSG_FULL_PAINT, PNULL, 0);
            }
        }
        break;

    case MSG_CLOSE_WINDOW:
       s_is_in_bookmark_moving = FALSE;
       break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : Delete bookmark(one bookmark, one bookmark dir or delete all)
//  Global resource dependence : none
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL void HandleWWWDeleteBookmark(MMI_WIN_ID_T win_id, BOOLEAN is_delete_all)
{
    uint16 cur_index = 0;
    MMI_HANDLE_T win_handle = PNULL;
    MMI_HANDLE_T ctrl_handle = PNULL;
    WWW_APPLET_T *applet = (WWW_APPLET_T *)MMK_GetAppletPtrByWin(win_id);

    win_handle = MMK_GetWinHandle(MMIWWW_HANDLE(applet), MMIWWW_BOOKMARK_WIN_ID);
    ctrl_handle = MMK_GetCtrlHandleByWin(win_handle, MMIWWW_BOOKMARK_LIST_CTRL_ID);
    cur_index = GetBookmarkCurIndex();
    
    if (is_delete_all)
    {
        //需考虑删除中出现失败情况下,list的刷新

        if (MMIWWW_BookmarkIsInRoot())
        {
            MMIWWW_RemoveAllBookmark(TRUE);
            GUILIST_RemoveAllItems(ctrl_handle);
        }
        else
        {
            int32 i = 0;
            uint16 item_num = 0;

            item_num = GUILIST_GetTotalItemNum(ctrl_handle);
            MMIWWW_RemoveAllBookmarkOfSubFolder();
            for(i = item_num - 1; i > 0; i--)
            {
                GUILIST_RemoveItem(ctrl_handle, i);    
            }
        }
    }
    else
    {
        MMIWWW_RemoveBookmark(cur_index, FALSE);
        if (MMIWWW_BookmarkIsInRoot())
        {
            GUILIST_RemoveItem(ctrl_handle, cur_index);
        }
        else//非root,需排除"..."项
        {
            GUILIST_RemoveItem(ctrl_handle, cur_index + 1);
        }    
    }

    MMIPUB_OpenAlertSuccessWin(TXT_DELETED);
}
/*****************************************************************************/
//  Description : handle message of sub menu of bookmark
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWBookmarkOptWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T        menu_ctrl_id = MMIWWW_POPMENU_BOOKMARK_OPT_CTRL_ID;

    //SCI_TRACE_LOW:"mmiwww_wintab.c:HandleWWWBookmarkOptWinMsg() msg_id=0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_11607_112_2_18_3_12_25_245,(uint8*)"d", msg_id);

    switch(msg_id)
    {   
    case MSG_OPEN_WINDOW:
        {
            MMI_MENU_ID_T group_id = (MMI_MENU_ID_T)MMK_GetWinAddDataPtr(win_id);
            uint16 cur_bookmark_num = (uint16)MMIWWW_GetBookmarkNumber();
            WWW_BOOKMARK_T *bookmark_info_ptr = MMIWWW_BOOKMARK();

#if 0
            if (MMIWWW_CTRL_TAB == MMIWWW_NetfrontGetMainMenuFocusCtrl())
            {
                    GUIMENU_SetItemGrayed(MMIWWW_POPMENU_BOOKMARK_OPT_CTRL_ID, group_id, ID_WWW_BOOKMARK_SUBMENU_EDIT, TRUE);
                    GUIMENU_SetItemGrayed(MMIWWW_POPMENU_BOOKMARK_OPT_CTRL_ID, group_id, ID_WWW_BOOKMARK_SUBMENU_DELETE, TRUE);
                    GUIMENU_SetItemGrayed(MMIWWW_POPMENU_BOOKMARK_OPT_CTRL_ID, group_id, ID_WWW_BOOKMARK_SUBMENU_PROPERTY, TRUE);
                    GUIMENU_SetItemGrayed(MMIWWW_POPMENU_BOOKMARK_OPT_CTRL_ID, group_id, ID_WWW_OPT_SEND_LINK, TRUE);
                    if (bookmark_num < 1)
                    {
                        GUIMENU_SetItemGrayed(MMIWWW_POPMENU_BOOKMARK_OPT_CTRL_ID, group_id, ID_WWW_BOOKMARK_SUBMENU_DELETE_ALL, TRUE);
                    }
            }
            else
#endif                
            {
                if (0 != bookmark_info_ptr->file_array_step)//非根目录情况下
                {
                    GUIMENU_SetItemGrayed(menu_ctrl_id, MENU_WWW_BOOKMARK_ADD_SUB, ID_WWW_BOOKMARK_SUBMENU_ADD_FOLDER, TRUE);
                    if (cur_bookmark_num <= 1)
                    {
                        GUIMENU_SetItemGrayed(menu_ctrl_id, group_id, ID_WWW_BOOKMARK_SUBMENU_DELETE_ALL, TRUE);
                    }
                }
                else
                {
                    if (cur_bookmark_num < 1)
                    {
                        GUIMENU_SetItemGrayed(menu_ctrl_id, group_id, ID_WWW_BOOKMARK_SUBMENU_DELETE_ALL, TRUE);
                    }

                }
                if(0 == MMIWWW_BookmarkGetFolderNum())//无子文件夹
                {
                    GUIMENU_SetItemGrayed(menu_ctrl_id, group_id, ID_WWW_BOOKMARK_SUBMENU_MOVE_TO, TRUE);
                }

                if (cur_bookmark_num < 1)
                {
                    group_id = (MMI_MENU_ID_T)MMK_GetWinAddDataPtr(win_id);

                    GUIMENU_SetItemGrayed(menu_ctrl_id, group_id, ID_WWW_BOOKMARK_SUBMENU_EDIT, TRUE);
                    GUIMENU_SetItemGrayed(menu_ctrl_id, group_id, ID_WWW_BOOKMARK_SUBMENU_DELETE, TRUE);
                    GUIMENU_SetItemGrayed(menu_ctrl_id, group_id, ID_WWW_BOOKMARK_SUBMENU_PROPERTY, TRUE);
                    GUIMENU_SetItemGrayed(menu_ctrl_id, group_id, ID_WWW_OPT_SEND_LINK, TRUE);
                    GUIMENU_SetItemGrayed(menu_ctrl_id, group_id, ID_WWW_BOOKMARK_SUBMENU_MOVE_TO, TRUE);
                }
                else
                {
                    switch (s_bookmark_cur_index_info.type)
                    {
                    case FILEARRAY_TYPE_NULL://"上一层"
                        GUIMENU_SetItemGrayed(menu_ctrl_id, group_id, ID_WWW_BOOKMARK_SUBMENU_EDIT, TRUE);
                        GUIMENU_SetItemGrayed(menu_ctrl_id, group_id, ID_WWW_BOOKMARK_SUBMENU_DELETE, TRUE);
                        GUIMENU_SetItemGrayed(menu_ctrl_id, group_id, ID_WWW_BOOKMARK_SUBMENU_PROPERTY, TRUE);
                        GUIMENU_SetItemGrayed(menu_ctrl_id, group_id, ID_WWW_OPT_SEND_LINK, TRUE);
                        GUIMENU_SetItemGrayed(menu_ctrl_id, group_id, ID_WWW_BOOKMARK_SUBMENU_MOVE_TO, TRUE);
                        break;
                        
                    case FILEARRAY_TYPE_FOLDER://文件夹
                        GUIMENU_SetItemGrayed(menu_ctrl_id, group_id, ID_WWW_BOOKMARK_SUBMENU_PROPERTY, TRUE);
                        GUIMENU_SetItemGrayed(menu_ctrl_id, group_id, ID_WWW_OPT_SEND_LINK, TRUE);
                        GUIMENU_SetItemGrayed(menu_ctrl_id, group_id, ID_WWW_BOOKMARK_SUBMENU_MOVE_TO, TRUE);
                        break;

                    default://书签文件
                        break;
                    }
                    
                }
            }
            
            MMK_SetActiveCtrl(MMK_GetCtrlHandleByWin(win_id, MMIWWW_POPMENU_BOOKMARK_OPT_CTRL_ID), FALSE);
        }
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        {
            /* reset the global variable of menu */
            MMI_MENU_ID_T group_id = (MMI_MENU_ID_T)MMK_GetWinAddDataPtr(win_id);

            GUIMENU_SetItemGrayed(menu_ctrl_id, group_id, ID_WWW_BOOKMARK_SUBMENU_EDIT, FALSE);
            GUIMENU_SetItemGrayed(menu_ctrl_id, group_id, ID_WWW_BOOKMARK_SUBMENU_DELETE, FALSE);
            GUIMENU_SetItemGrayed(menu_ctrl_id, group_id, ID_WWW_BOOKMARK_SUBMENU_DELETE_ALL, FALSE);
            GUIMENU_SetItemGrayed(menu_ctrl_id, group_id, ID_WWW_BOOKMARK_SUBMENU_PROPERTY, FALSE);
            GUIMENU_SetItemGrayed(menu_ctrl_id, group_id, ID_WWW_OPT_SEND_LINK, FALSE);
            GUIMENU_SetItemGrayed(menu_ctrl_id, group_id, ID_WWW_BOOKMARK_SUBMENU_MOVE_TO, FALSE);
            GUIMENU_SetItemGrayed(menu_ctrl_id, MENU_WWW_BOOKMARK_ADD_SUB, ID_WWW_BOOKMARK_SUBMENU_ADD_FOLDER, FALSE);
        }
        break;

    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        {
            MMI_MENU_GROUP_ID_T group_id = 0;
            MMI_MENU_ID_T menu_id = 0;
            MMI_STRING_T url_string = {0};
            uint16 selected_item = 0;
            MMI_HANDLE_T applet_handle = MMIWWW_HANDLE((WWW_APPLET_T *)MMK_GetAppletPtrByWin(win_id));
            MMI_TEXT_ID_T              query_text_id = TXT_NULL;

            GUIMENU_GetId(MMK_GetCtrlHandleByWin(win_id, MMIWWW_POPMENU_BOOKMARK_OPT_CTRL_ID), &group_id, &menu_id);
            switch ( menu_id )
            {
            case ID_WWW_HOMPAGE:
                mmiwww_AccessHomepage();
                MMK_CloseWin(win_id);
                break;

            case ID_WWW_WINDOW_MANAGER:
                mmiwww_OpenWindowManagerWin();
                MMK_CloseWin(win_id);
                break;

            case ID_WWW_BOOKMARK_SUBMENU_ADD_FILE:
                MMIWWW_ChangeBookmarkWin(MMIWWW_BM_CHANGE_ADD_NULL);
                MMK_CloseWin(win_id);
                break;
                
            case ID_WWW_BOOKMARK_SUBMENU_ADD_FOLDER:
                MMIWWW_ChangeBookmarkWin(MMIWWW_BM_CHANGE_ADD_DIR);
                MMK_CloseWin(win_id);
                break;
                
            case ID_WWW_BOOKMARK_SUBMENU_MOVE_TO:
                BookmarkEnterMoveState(win_id, GetBookmarkCurIndex());
                MMK_CloseWin(win_id);
                break;

           case ID_WWW_BOOKMARK_SUBMENU_EDIT:
                if (FILEARRAY_TYPE_FOLDER == s_bookmark_cur_index_info.type)
                {
                    MMIWWW_ChangeBookmarkWin(MMIWWW_BM_CHANGE_EDIT_DIR);
                }
                else
                {
                    MMIWWW_ChangeBookmarkWin(MMIWWW_BM_CHANGE_EDIT);
                }                
                MMK_CloseWin(win_id);
                break;
#if 0
            case ID_WWW_BOOKMARK_SUBMENU_ADD:
            case ID_WWW_BOOKMARK_SUBMENU_EDIT:
                MMIWWW_ChangeBookmarkWin(menu_id == ID_WWW_BOOKMARK_SUBMENU_ADD ? MMIWWW_BM_CHANGE_ADD_NULL : MMIWWW_BM_CHANGE_EDIT);
                MMK_CloseWin(win_id);
                break;
#endif
            case ID_WWW_BOOKMARK_SUBMENU_DELETE:
            case ID_WWW_BOOKMARK_SUBMENU_DELETE_ALL:
                if (menu_id == ID_WWW_BOOKMARK_SUBMENU_DELETE_ALL)
                {
                    query_text_id = TXT_DELETE_ALL_QUERY;
                }
                else
                {
                    query_text_id = (FILEARRAY_TYPE_FILE == s_bookmark_cur_index_info.type)? TXT_WWW_BOOKMARK_ENSURE_DELETE: TXT_WWW_BOOKMARK_ENSURE_DELETE_DIR;
                }                
                    
                MMIPUB_OpenQueryWinByTextId(query_text_id,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
                break;
        
            case ID_WWW_BOOKMARK_SUBMENU_PROPERTY:
                MMIWWW_ShowBookmarkWin();
                MMK_CloseWin(win_id);
                break;

            case ID_WWW_SEND_OPT_VIA_SMS:
            case ID_WWW_SEND_OPT_VIA_MMS:
                selected_item = GetBookmarkCurIndex();
                MMIWWW_GetBookmarkUrl(selected_item, &url_string);
                if (ID_WWW_SEND_OPT_VIA_SMS == menu_id)
                {
                    MMIAPISMS_WriteNewMessage(MN_DUAL_SYS_MAX, &url_string, PNULL, 0);
                }
                else
                {
                    MMIAPIMMS_BrwSendUrlByMMS(&url_string);
                }
                if (PNULL != url_string.wstr_ptr)
                {
                   SCI_FREE(url_string.wstr_ptr);
                }
                MMK_CloseWin(win_id);
                break;
            
            case ID_WWW_SUBMENU_SAVED_PAGE:
                {
                    MMI_HANDLE_T win_handle = MMK_GetWinHandle(applet_handle, MMIWWW_PAGEMEMO_WIN_ID);
                    if( !MMK_IsOpenWin(win_handle) )
                    {
                        MMIWWW_OpenWWWPageMemoWin();
                    }
                    else
                    {
                        if (!MMK_IsFocusWin(win_handle))
                        {
                            MMK_WinGrabFocus(win_handle);
                        }
                    }
                    MMK_CloseWin(win_id);
                }
                break;

            case ID_WWW_DOWNLOAD_MANAGE:
                MMIAPIDL_StartDownload(MMIDL_OPEN_DOWNLOAD_MANAGER, PNULL);
                MMK_CloseWin(win_id);
                break;

            case ID_WWW_SUBMENU_SETTING:
                MMIWWW_OpenWWWSetMenuWin();
                MMK_CloseWin(win_id);
                break;

            case ID_WWW_MINIMIZE:
                MMK_CloseWin(win_id);
                MMIAPIWWW_MinimizeWWW();
                break;

            case ID_WWW_EXIT:
                if (MMK_IsOpenWin(mmiwww_GetBrowserWinhandle()))
                {
                    MMK_CloseWin(win_id);
                    MMIPUB_OpenQueryWinByTextId(TXT_EXIT_BROWSER, IMAGE_PUBWIN_QUERY,PNULL,PNULL);
                }
                else
                {
                    MMIAPIWWW_StopWWW();
                }
                break;

            default:
                break;
            }
        }
        break;

    case MSG_PROMPTWIN_OK:
        {
            MMI_MENU_ID_T menu_id = 0;
            MMI_MENU_GROUP_ID_T group_id = 0;
            
            MMIPUB_CloseQuerytWin(PNULL);
            GUIMENU_GetId(MMK_GetCtrlHandleByWin(win_id, MMIWWW_POPMENU_BOOKMARK_OPT_CTRL_ID), &group_id, &menu_id);
            if (ID_WWW_EXIT == menu_id)
            {
                MMIPUB_CloseQuerytWin(PNULL);
                MMIAPIWWW_StopWWW();
            }
            else
            {
                HandleWWWDeleteBookmark(win_id, (BOOLEAN)(menu_id == ID_WWW_BOOKMARK_SUBMENU_DELETE_ALL));
                MMK_CloseWin(win_id);
            }
        }
        break;

    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : handle message of sub menu of bookmark detail
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWBookmarkDetailSubMenuWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"mmiwww_wintab.c:HandleWWWBookmarkDetailSubMenuWinMsg() msg_id=0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_11901_112_2_18_3_12_25_246,(uint8*)"d", msg_id);

    switch(msg_id)
    {   
    case MSG_OPEN_WINDOW:
        MMK_SetActiveCtrl(MMK_GetCtrlHandleByWin(win_id, MMIWWW_BOOKMARK_DETAIL_SUBMENU_CTRL_ID), FALSE);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        {
            MMI_MENU_ID_T menu_id = 0;
            MMI_MENU_GROUP_ID_T group_id = 0;

            GUIMENU_GetId(MMK_GetCtrlHandleByWin(win_id, MMIWWW_BOOKMARK_DETAIL_SUBMENU_CTRL_ID), &group_id, &menu_id);
            switch ( menu_id )
            {
            case ID_WWW_BOOKMARK_SUBMENU_DETAIL_EDIT:
                MMIWWW_ChangeBookmarkWin(MMIWWW_BM_CHANGE_EDIT);
                MMK_CloseWin(win_id);
                break;

            case ID_WWW_BOOKMARK_SUBMENU_DETAIL_DELETE:
                MMIPUB_OpenQueryWinByTextId(TXT_COMMON_CONFIRM_DELETE,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
                break;

            default:
                break;
            }
        }
        break;
        
    case MSG_PROMPTWIN_OK:
        {
            MMI_MENU_ID_T menu_id = 0;
            MMI_MENU_GROUP_ID_T group_id = 0;
            MMI_HANDLE_T applet_handle = MMIWWW_HANDLE((WWW_APPLET_T *)MMK_GetAppletPtrByWin(win_id));
            MMIPUB_CloseQuerytWin(PNULL);
            GUIMENU_GetId(MMK_GetCtrlHandleByWin(win_id, MMIWWW_BOOKMARK_DETAIL_SUBMENU_CTRL_ID), &group_id, &menu_id);
            HandleWWWDeleteBookmark(win_id, (BOOLEAN)(menu_id == ID_WWW_BOOKMARK_SUBMENU_DELETE_ALL));
            MMK_CloseWin(win_id);
            MMK_CloseWin(MMK_GetWinHandle(applet_handle, MMIWWW_SHOW_BOOKMARK_WIN_ID));
        }
        break;

    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : get bookmark's file array index from bookmark list index
//  Global resource dependence : none
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL uint16 GetBookmarkArrayIndex(uint16 cur_item_index)
{
    uint16 array_index = 0;
    WWW_BOOKMARK_T *bookmark_info_ptr = MMIWWW_BOOKMARK();

    if ((1 == bookmark_info_ptr->file_array_step) && 0 != cur_item_index)//二级目录有多余top 项"..."
    {
        array_index = cur_item_index - 1;
    }
    else if (s_is_in_bookmark_moving)//move时有多余top 项"/"
    {
        array_index = cur_item_index - 1;
    }
    else
    {
        array_index = cur_item_index;
    }
    return array_index;
}

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
    BOOLEAN  copy_result = FALSE;
    WWW_BOOKMARK_T *bookmark_info_ptr = PNULL;
    MMI_HANDLE_T win_handle = PNULL;
    MMI_HANDLE_T ctrl_handle = PNULL;
    WWW_APPLET_T *applet = (WWW_APPLET_T *)MMK_GetAppletPtrByWin(win_id);

    switch( msg_id)
    {
    case MSG_PIC_COPY_FILE:
        copy_result=*(BOOLEAN *)param;

        //SCI_TRACE_LOW:"HandleBookmarkMoveWaitWinMsg  copy_result=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_12014_112_2_18_3_12_26_247,(uint8*)"d",copy_result);
        
        if(!copy_result)
        {
            MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_ERROR);               
            return (result);
        }
        else
        {
            bookmark_info_ptr = MMIWWW_BOOKMARK();
            win_handle = MMK_GetWinHandle(MMIWWW_HANDLE(applet), MMIWWW_BOOKMARK_WIN_ID);
            ctrl_handle = MMK_GetCtrlHandleByWin(win_handle, MMIWWW_BOOKMARK_LIST_CTRL_ID);

            //second:delete the old bookmark file
            if (MMIAPIFMM_DeleteFileSyn(s_bookmark_move_array_data.filename, s_bookmark_move_array_data.name_len))
            {
                MMIPUB_OpenAlertSuccessWin( TXT_WWW_BOOKMARK_MOVE_SUCC);        

                s_is_in_bookmark_moving = FALSE;
                
                //go back to root path
                MMIWWW_LoadBookmarkFileArray(BOOKMARK_STEP_TYPE_ROOT, 0, FUNC_FIND_NULL);
                MMIWWW_AppendListboxitem(win_handle, MMIWWW_BOOKMARK_LIST_CTRL_ID, (uint16)bookmark_info_ptr->cur_index_num, TXT_WWW_BOOKMARK_EMPTY, STXT_OPTION, TXT_WWW_EXIT);
                GUILIST_SetCurItemIndex(ctrl_handle, 0);
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_ERROR);               
                return (result);
            }            
       }

        MMK_CloseWin(win_id);
        break;
            
    case MSG_APP_CANCEL:
    case MSG_APP_RED:
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
LOCAL MMI_RESULT_E MMIWWW_HandleMoveBookmark(MMI_WIN_ID_T      win_id, 
                                          MMI_MESSAGE_ID_E  msg_id, 
                                          DPARAM            param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"MMIWWW_HandleMoveBookmark() msg_id=0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_12071_112_2_18_3_12_26_248,(uint8*)"d", msg_id);

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
            wchar *full_file_name = PNULL;

            win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
            if (PNULL != win_info_ptr)
            {
                full_file_name = (wchar *)win_info_ptr->user_data;
            }
 
            if (BookmarkMoveTo(full_file_name, MMIAPICOM_Wstrlen(full_file_name)))
            {
                //do nothing, waiting for asynchronous
            }
            else//error
            {
                MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_ERROR);               
            }
            MMK_CloseWin(win_id);
            break;
        }

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        {
            MMIPUB_INFO_T *win_info_ptr = PNULL;
            wchar *full_file_name = PNULL;

            win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
            if (PNULL != win_info_ptr)
            {
                SCI_FREE(full_file_name);
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
LOCAL BOOLEAN  BookmarkMoveTo(
                           const wchar *full_path_ptr,     //in
                           uint16     full_path_len       //in, 双字节为单位
                           )
{
    BOOLEAN result = TRUE; 
    uint32 time_out = 0;
    MMI_WIN_ID_T    wait_win_id = MMIWWW_BOOKMARK_MOVE_WIN_ID;
    MMI_HANDLE_T  win_handle = 0;

    if ((PNULL == full_path_ptr) || (0 == full_path_len))
    {
        return FALSE;
    }
    
    MMIPUB_OpenAlertWinByTextIdEx(MMIWWW_HANDLE(MMIWWW_APPLETINSTANCE()),
                    &time_out,
                    TXT_COMMON_WAITING,
                    TXT_NULL,
                    IMAGE_PUBWIN_WAIT,
                    &wait_win_id,
                    PNULL,
                    MMIPUB_SOFTKEY_NONE,
                    HandleBookmarkMoveWaitWinMsg,
                    PNULL);

    win_handle = MMK_GetWinHandle(MMIWWW_HANDLE(MMIWWW_APPLETINSTANCE()), wait_win_id);
    
    result = MMIAPIFMM_CopyOneFile(s_bookmark_move_array_data.filename, s_bookmark_move_array_data.name_len, 
                        full_path_ptr, full_path_len, win_handle, MSG_PIC_COPY_FILE);

    //如果失败，直接关闭等待窗口
    if(!result)        
    {
         MMK_CloseWin(win_handle);
    }        
        
    return result; 
}

/*****************************************************************************/
//  Description : begin to move one bookmark
//  Global resource dependence : none
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL void BookmarkBeginMoveTo(uint16 cur_index)
{
    WWW_BOOKMARK_T *bookmark_info_ptr = MMIWWW_BOOKMARK();
    uint16 file_array_index = 0;
    FILEARRAY_DATA_T f_array_data = {0};
    wchar   file_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16  file_name_len = MMIFILE_FULL_PATH_MAX_LEN;
    wchar   path_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16  path_name_len = MMIFILE_FULL_PATH_MAX_LEN;
    wchar   full_file_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16  full_file_name_len = MMIFILE_FULL_PATH_MAX_LEN;
    MMI_STRING_T string_1 ={0};

    //get new path
    if (0 == cur_index)//means:move to root
    {
        slimFile_CombineFullPath((slim_char *)BRSCFG_FAVORITES, path_name, &path_name_len);
        string_1.wstr_ptr = path_name;
        string_1.wstr_len = path_name_len;
    }
    else
    {
        file_array_index = GetBookmarkArrayIndex(cur_index);
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
    MMIWWW_BookmarkCombinePath(string_1.wstr_ptr, string_1.wstr_len, 
                    file_name, file_name_len, 
                    (wchar*)g_bookmark_file_suffix + 1, MMIAPICOM_Wstrlen(g_bookmark_file_suffix) -1, 
                    full_file_name, &full_file_name_len);
    
    if(0 == MMIAPICOM_Wstrcmp(s_bookmark_move_array_data.filename, full_file_name))
    {
        MMIPUB_OpenAlertWarningWin(TXT_WWW_BOOKMARK_SAME_PATH);
    }
    else if (MMIWWW_FindDuplicatedBookmarkIndex(string_1.wstr_ptr, string_1.wstr_len, bookmark_info_ptr->file_array_step, file_name) >= 0)//暂不支持替换
    {
        wchar * move_file_name = PNULL;

        move_file_name = SCI_ALLOCA((full_file_name_len + 1) * sizeof(wchar));
        if (PNULL == move_file_name)
        {
            return;
        }

        SCI_MEMSET(move_file_name, 0, (full_file_name_len + 1) * sizeof(wchar));
        SCI_MEMCPY(move_file_name, full_file_name, full_file_name_len * sizeof(wchar));
        MMIPUB_OpenAlertWinByTextIdEx(MMIWWW_HANDLE(MMIWWW_APPLETINSTANCE()), 0, TXT_WWW_BOOKMARK_QUERY_REPLACE, TXT_NULL, IMAGE_PUBWIN_QUERY, PNULL, PNULL, MMIPUB_SOFTKEY_OKCANCEL, MMIWWW_HandleMoveBookmark, (uint32)move_file_name);
        return;
    }
    else 
    {     
        if (BookmarkMoveTo(full_file_name, full_file_name_len))
        {
            //do nothing, waiting for asynchronous
        }
        else//error
        {
            MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_ERROR);               
        }
    }
}

/*****************************************************************************/
//  Description :enter the bookmark moving state and back to bookmark list window to choose
//  Global resource dependence : none
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL void BookmarkEnterMoveState(MMI_WIN_ID_T win_id, uint16 array_index)
{
    WWW_BOOKMARK_T *bookmark_info_ptr = MMIWWW_BOOKMARK();
    MMI_HANDLE_T win_handle = PNULL;
    MMI_HANDLE_T ctrl_handle = PNULL;
    WWW_APPLET_T *applet = (WWW_APPLET_T *)MMK_GetAppletPtrByWin(win_id);

    win_handle = MMK_GetWinHandle(MMIWWW_HANDLE(applet), MMIWWW_BOOKMARK_WIN_ID);
    ctrl_handle = MMK_GetCtrlHandleByWin(win_handle, MMIWWW_BOOKMARK_LIST_CTRL_ID);
    
    SCI_MEMSET(&s_bookmark_move_array_data, 0x00, sizeof(s_bookmark_move_array_data));
    if(MMIAPIFILEARRAY_Read(bookmark_info_ptr->file_array, array_index, &s_bookmark_move_array_data))
    {
        s_is_in_bookmark_moving = TRUE;
        //MMK_CloseWin(win_id);

        //back to bookmark list window to choose,update the list for choose
        if (MMIWWW_LoadBookmarkFileArray(BOOKMARK_STEP_TYPE_ROOT, 0, FUNC_FIND_FOLDER))
        {
            bookmark_info_ptr->cur_index_num++;//增加了"/" 根目录的显示
            GUILIST_SetCurItemIndex(ctrl_handle, 0);
            MMIWWW_AppendListboxitem(win_handle, MMIWWW_BOOKMARK_LIST_CTRL_ID, (uint16)bookmark_info_ptr->cur_index_num, TXT_WWW_BOOKMARK_EMPTY, STXT_OPTION, TXT_WWW_EXIT);
            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
        }
        else//error,do nothing
        {
            s_is_in_bookmark_moving = FALSE;
            SCI_MEMSET(&s_bookmark_move_array_data, 0x00, sizeof(s_bookmark_move_array_data));
        }
    }
}

/****************************************************************************/    
//  Description : Set richtext
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void SetInputBookmarkDetailEntry(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, MMIWWW_BM_CHANGE_TYPE_E change_type)
{
    MMI_STRING_T string = {0};
    BOOLEAN is_free = TRUE;
    uint8 * added_url = PNULL;
    uint8 * added_title = PNULL;
    WWW_APPLET_T *applet = PNULL;
    MMI_HANDLE_T win_handle = PNULL;
    MMI_HANDLE_T ctrl_handle = PNULL;
    MMI_HANDLE_T richtext_handle = PNULL;
    MMI_TEXT_ID_T title_id = TXT_NULL;
    MMI_TEXT_ID_T sl_text_id = TXT_NULL;
    uint16 index = 0;
    wchar http_head[] = {'h', 't', 't', 'p',':', '/', '/', '\0'};
    wchar *url = PNULL;
    uint16 url_len = 0;
    wchar *title = PNULL;
    uint16 title_len = 0;

    applet = MMIWWW_APPLETINSTANCE();
    richtext_handle = MMK_GetWinHandle(MMIWWW_HANDLE(applet), MMIWWW_INPUT_BOOKMARK_WIN_ID);
    win_handle = MMK_GetWinHandle(MMIWWW_HANDLE(applet), MMIWWW_BOOKMARK_WIN_ID);
    ctrl_handle = MMK_GetCtrlHandleByWin(win_handle, MMIWWW_BOOKMARK_LIST_CTRL_ID);

    /* Title */
    SCI_MEMSET(&string, 0, sizeof(MMI_STRING_T));
    if((MMIWWW_BM_CHANGE_ADD_DIR == change_type) || (MMIWWW_BM_CHANGE_EDIT_DIR == change_type))//folder without url
    {
        title_id = TXT_WWW_BOOKMARK_DIR_TITLE;
    }
    else
    {
        title_id = TXT_COMM_TITLE;
    }
    MMIRES_GetText(title_id, ctrl_id, &string);
    MMIWWW_RichText_AddItem(ctrl_id, &string, GUIRICHTEXT_FRAME_NONE, GUIRICHTEXT_TAG_NONE);

    SCI_MEMSET(&string, 0, sizeof(MMI_STRING_T));
    title_id = TXT_NULL;
    if (MMIWWW_BM_CHANGE_ADD_NULL == change_type)
    {
        title_id = TXT_WWW_BOOKMARK_NEW_BOOKMARK_TITLE;
        sl_text_id = TXT_NULL;
    }
    else if (MMIWWW_BM_CHANGE_ADD_DIR == change_type)//新增目录
    {
        title_id = TXT_WWW_BOOKMARK_NEW_DIR_TITLE;
        sl_text_id = TXT_NULL;
    }
    else if (MMIWWW_BM_CHANGE_ADD_CUR_PAGE == change_type)
    {
        title_id = TXT_WWW_BOOKMARK_NEW_BOOKMARK_TITLE;
        sl_text_id = STXT_SAVE;
        MMIWWW_Netfront_GetPageTitle(&string);
        if (PNULL != string.wstr_ptr && string.wstr_len > MAX_WEB_TITLE_LEN)
        {
            string.wstr_ptr[MAX_WEB_TITLE_LEN] = '\0';
            string.wstr_len = MAX_WEB_TITLE_LEN;
        }
    }
    else if (MMIWWW_BM_CHANGE_ADD_URL == change_type)
    {
        title_id = TXT_WWW_BOOKMARK_NEW_BOOKMARK_TITLE;
        sl_text_id = STXT_SAVE;

        added_title = MMIWWW_NetfrontGetAddedTitle();
        if (PNULL != added_title)
        {
            title_len = SCI_STRLEN((char *)added_title);
            title_len = (uint16)MIN(title_len, MAX_WEB_URL_LEN);
            title = SCI_ALLOCA((title_len + 1) * sizeof(wchar));
            if (PNULL != title)
            {
                SCI_MEMSET(title, 0x0, ((title_len + 1) * sizeof(wchar)));
                GUI_UTF8ToWstr(title, title_len , (const uint8 *)added_title, title_len);
                string.wstr_ptr = title;
                string.wstr_len = (uint16)MMIAPICOM_Wstrlen(title);
            }
            SCI_FREE(added_title);
            MMIWWW_NetfrontSetAddedTitle(PNULL);        
        }
    }
    else
    {
        if (MMIWWW_BM_CHANGE_EDIT_DIR == change_type)
        {
            title_id = TXT_WWW_BOOKMARK_EDIT_DIR_TITLE;
        }
        else
        {
            title_id = TXT_WWW_BOOKMARK_EDIT_BOOKMARK_TITLE;
        }
        sl_text_id = TXT_NULL;
        index = GetBookmarkCurIndex();
        MMIWWW_GetBookmarkTitle(index, &string);
    }

    GUIWIN_SetTitleTextId(richtext_handle, title_id, FALSE);
    MMIWWW_NetfrontSetBookmarkChange((BOOLEAN)(STXT_SAVE == sl_text_id));
    GUIWIN_SetSoftkeyTextId(win_id, sl_text_id, TXT_EDIT, STXT_RETURN, FALSE);
    MMIWWW_RichText_AddItem(ctrl_id, &string, GUIRICHTEXT_FRAME_NORMAL, GUIRICHTEXT_TAG_NORMAL);
    if (PNULL != string.wstr_ptr)
    {
        SCI_FREE(string.wstr_ptr);
        string.wstr_ptr = PNULL;
    }

    /* URL */
    if((MMIWWW_BM_CHANGE_ADD_DIR == change_type) || (MMIWWW_BM_CHANGE_EDIT_DIR == change_type))//folder without url
    {
        return;
    }

    is_free = TRUE;

    SCI_MEMSET(&string, 0, sizeof(MMI_STRING_T));
    MMIRES_GetText(TXT_COMMON_URL, ctrl_id, &string);
    MMIWWW_RichText_AddItem(ctrl_id, &string, GUIRICHTEXT_FRAME_NONE, GUIRICHTEXT_TAG_NONE);

    SCI_MEMSET(&string, 0, sizeof(MMI_STRING_T));
    if (MMIWWW_BM_CHANGE_ADD_NULL == change_type)
    {
        string.wstr_ptr = http_head;
        string.wstr_len = (uint16)MMIAPICOM_Wstrlen(http_head);
        is_free = FALSE;
    }
    else if (MMIWWW_BM_CHANGE_ADD_CUR_PAGE == change_type)
    {
        MMIWWW_Netfront_GetPageAddress(&string);
        if (PNULL != string.wstr_ptr && string.wstr_len > MAX_WEB_URL_LEN)
        {
            string.wstr_ptr[MAX_WEB_URL_LEN] = '\0';
            string.wstr_len = MAX_WEB_URL_LEN;
        }
    }
    else if (MMIWWW_BM_CHANGE_ADD_URL == change_type)
    {
        added_url = MMIWWW_NetfrontGetAddedUrl();
        if (PNULL != added_url)
        {
            title_len = SCI_STRLEN((char *)added_url);
            url_len = (uint16)MIN(title_len, MAX_WEB_URL_LEN);
            url = SCI_ALLOCA((url_len + 1) * sizeof(wchar));
            if (PNULL != url)
            {
                SCI_MEMSET(url, 0x0, ((url_len + 1) * sizeof(wchar)));
                GUI_UTF8ToWstr(url, url_len , (const uint8 *)added_url, url_len);
                string.wstr_ptr = url;
                string.wstr_len = (uint16)MMIAPICOM_Wstrlen(url);
            }

            SCI_FREE(added_url);
            MMIWWW_NetfrontSetAddedUrl(PNULL);        
        }
    }
    else
    {
        MMIWWW_GetBookmarkUrl(index, &string);
    }

    MMIWWW_RichText_AddItem(ctrl_id, &string, GUIRICHTEXT_FRAME_NORMAL, GUIRICHTEXT_TAG_NORMAL);
    if (is_free && PNULL != string.wstr_ptr)
    {
        SCI_FREE(string.wstr_ptr);
        string.wstr_ptr = PNULL;
    }
}

/*****************************************************************************/
//  Description : handle message of bookmark url input
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleInputBookmarkWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"mmiwww_wintab.c:HandleInputBookmarkWinMsg() msg_id=0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_12465_112_2_18_3_12_27_249,(uint8*)"d", msg_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            MMI_HANDLE_T ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_BOOKMARK_INPUT_RICHTEXT_CTRL_ID);
            MMIWWW_BM_CHANGE_TYPE_E change_type = (uint32)MMK_GetWinAddDataPtr(win_id);/*lint !e64*/

            SetInputBookmarkDetailEntry(win_id, ctrl_handle, change_type);
            GUIRICHTEXT_SetFocusMode(ctrl_handle, GUIRICHTEXT_FOCUS_SHIFT_ADVANCED);
            GUIRICHTEXT_SetFocusItem(ctrl_handle, MMIWWW_BOOKMARK_TITLE_INDEX);
            MMK_SetAtvCtrl(win_id, ctrl_handle);
        }
        break;

    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        {
            uint16 focus_index = 0;
            MMI_RESULT_E ret = MMI_RESULT_FALSE;

            ret = GUIRICHTEXT_GetFocusItemIndex(MMK_GetCtrlHandleByWin(win_id, MMIWWW_BOOKMARK_INPUT_RICHTEXT_CTRL_ID), &focus_index);
            if (MMI_RESULT_TRUE == ret)
            {
                if (MMIWWW_BOOKMARK_TITLE_INDEX == focus_index) /* Title is focused */
                {
                    MMIWWW_OpenEditWin(win_id, MMIWWW_BOOKMARK_LABEL_TITLE_INPUT_CTRL_ID);
                }
                else
                {
                    MMIWWW_OpenEditWin(win_id, MMIWWW_BOOKMARK_LABEL_URL_INPUT_CTRL_ID);
                }
            }
        }
        break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
        if (MMIWWW_NetfrontGetBookmarkChange())
        {
            slim_int    duplicated_index = 0;
            uint16 current_index = GetBookmarkCurIndex();
            uint16 new_index = 0;
            MMI_STRING_T url_text_str = {0};
            MMI_STRING_T title_text_str = {0};
            GUIRICHTEXT_ITEM_T item_data = {0};/*lint !e64*/
            MMI_HANDLE_T applet_handle = MMIWWW_HANDLE((WWW_APPLET_T *)MMK_GetAppletPtrByWin(win_id));
            MMI_HANDLE_T detail_win_handle = MMK_GetWinHandle(applet_handle, MMIWWW_SHOW_BOOKMARK_WIN_ID);
            MMI_HANDLE_T main_win_handle = MMK_GetWinHandle(applet_handle, MMIWWW_MAINMENU_WIN_ID);
            MMI_HANDLE_T richtext_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_BOOKMARK_INPUT_RICHTEXT_CTRL_ID);
            MMI_HANDLE_T bm_win_handle = MMK_GetWinHandle(applet_handle, MMIWWW_BOOKMARK_WIN_ID);
            MMI_HANDLE_T bm_ctrl_handle = PNULL;
            MMIWWW_BM_CHANGE_TYPE_E change_type = (uint32)MMK_GetWinAddDataPtr(win_id);/*lint !e64*/
            BOOLEAN is_with_url = TRUE;
            WWW_BOOKMARK_T *bookmark_info_ptr = MMIWWW_BOOKMARK();

            if ((MMIWWW_BM_CHANGE_ADD_DIR == change_type) || (MMIWWW_BM_CHANGE_EDIT_DIR == change_type))
            {
                is_with_url = FALSE;
            }

            GUIRICHTEXT_GetItem(richtext_handle, (uint16)MMIWWW_BOOKMARK_TITLE_INDEX, &item_data);
            title_text_str.wstr_ptr = item_data.text_data.buf.str_ptr;
            title_text_str.wstr_len = item_data.text_data.buf.len;

            if ((PNULL == title_text_str.wstr_ptr) || (0 == title_text_str.wstr_len))//标题不能为空
            {
                GUIRICHTEXT_SetFocusItem(richtext_handle, MMIWWW_BOOKMARK_TITLE_INDEX);
		        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
                MMIPUB_OpenAlertWarningWin(TXT_COMM_NO_TITLE);
                return recode;
            }

            if (!MMIAPICOM_FileNameIsValid(&title_text_str))
            {
                GUIRICHTEXT_SetFocusItem(richtext_handle, MMIWWW_BOOKMARK_TITLE_INDEX);
		        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
                MMIPUB_OpenAlertWarningWin( TXT_COMM_INVALID_TITLE);
                return recode;
            }

            if (is_with_url)
            {
                GUIRICHTEXT_GetItem(richtext_handle, (uint16)MMIWWW_BOOKMARK_URL_INDEX, &item_data);
                url_text_str.wstr_ptr = item_data.text_data.buf.str_ptr;
                url_text_str.wstr_len = item_data.text_data.buf.len;

            if ((PNULL == url_text_str.wstr_ptr) || (0 == url_text_str.wstr_len) || 
                ((PNULL != url_text_str.wstr_ptr) && (url_text_str.wstr_len == 7) && (0 == MMIAPICOM_WstrStrncmp(url_text_str.wstr_ptr, (uint8 *)MMIWWW_HTTP_HEADER, 7))))
            {
                GUIRICHTEXT_SetFocusItem(richtext_handle, MMIWWW_BOOKMARK_URL_INDEX);
		        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
                MMIPUB_OpenAlertWarningWin(TXT_COMM_URL_NULL);
                return recode;
            }
                #if 0//暂不作url有效性的判断
            else
            {
                if (!MMIWWW_IsURLValid(&url_text_str))
                {
                        GUIRICHTEXT_SetFocusItem(richtext_handle, MMIWWW_BOOKMARK_URL_INDEX);
		                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
                        MMIPUB_OpenAlertWarningWin(TXT_WWW_BOOKMARK_FAIL_URL);
                        return recode;
                    }
                }
                #endif
            }
#ifndef WIN32
            if ((MMIWWW_BM_CHANGE_ADD_NULL == change_type) || (MMIWWW_BM_CHANGE_ADD_CUR_PAGE == change_type) 
                || (MMIWWW_BM_CHANGE_ADD_URL == change_type) || (MMIWWW_BM_CHANGE_ADD_DIR == change_type))
            {
                uint32 free_high_word = 0;
                uint32 free_low_word  = 0;
                BOOLEAN ret = FALSE;

                ret = MMIAPIFMM_GetDeviceFreeSpace(MMIAPIFMM_GetDevicePath(MMI_DEVICE_SYSTEM), MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SYSTEM), &free_high_word, &free_low_word);
                //SCI_TRACE_LOW:"HandleInputBookmarkWinMsg: ret = %d, high = %d, low = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_12584_112_2_18_3_12_27_250,(uint8*)"ddd", ret, free_high_word, free_low_word);

                if (!ret || (0 == free_high_word && MMIWWW_DEVICE_SYS_LOW_SPACE > free_low_word))
                {
                    MMIPUB_OpenAlertWarningWin( TXT_NO_SPACE);
                    MMK_CloseWin(win_id);
                    return recode;
                }
            }
#endif
            //外部进入"新建书签",默认保存到bookmark的root目录下
            if ((MMIWWW_BM_CHANGE_ADD_CUR_PAGE == change_type) || (MMIWWW_BM_CHANGE_ADD_URL == change_type))
            {
                MMIWWW_LoadBookmarkFileArray(BOOKMARK_STEP_TYPE_ROOT, 0, FUNC_FIND_NULL);
            }

            if ((MMIWWW_BM_CHANGE_EDIT_DIR == change_type) || (MMIWWW_BM_CHANGE_ADD_DIR == change_type))//dir
            {
                duplicated_index = MMIWWW_FindDuplicatedBookmarkDirIndex(bookmark_info_ptr->full_path, bookmark_info_ptr->full_path_len, 0, title_text_str.wstr_ptr);
                bm_ctrl_handle = MMK_GetCtrlHandleByWin(bm_win_handle, MMIWWW_BOOKMARK_LIST_CTRL_ID);

                if (MMIWWW_BM_CHANGE_EDIT_DIR == change_type)
                {
                    if ((duplicated_index >= 0) && (duplicated_index != current_index))/*重复了,提示"已存在"*/
                    {
                        MMIPUB_OpenAlertWarningWin( TXT_WWW_BOOKMARK_DIR_EXIST);
                        return recode;
                    }
                    MMIWWW_ChangeBookmarkTitle(current_index, title_text_str, &new_index);
                }
                else
                {
                    if (duplicated_index >= 0)/*重复了,提示"已存在"*/
                    {
                        MMIPUB_OpenAlertWarningWin( TXT_WWW_BOOKMARK_DIR_EXIST);
                        return recode;
                    }
                
                    MMIWWW_AddBookmarkFolder(title_text_str, FALSE,  &new_index);
                }
            }
            else//bookmark
            {
                duplicated_index = MMIWWW_FindDuplicatedBookmarkIndex(bookmark_info_ptr->full_path, bookmark_info_ptr->full_path_len, bookmark_info_ptr->file_array_step, title_text_str.wstr_ptr);
                if(MMIWWW_BM_CHANGE_EDIT == change_type)
                {
                    bm_ctrl_handle = MMK_GetCtrlHandleByWin(bm_win_handle, MMIWWW_BOOKMARK_LIST_CTRL_ID);
                    if (duplicated_index >=0 && duplicated_index != current_index)/*重复了,提示"已存在"*/
                    {
                        MMIPUB_OpenAlertWarningWin( TXT_WWW_BOOKMARK_EXIST);
                        return recode;
                    }

                    MMIWWW_ChangeBookmarkTitle(current_index, title_text_str, &new_index);
                    MMIWWW_ChangeBookmarkUrl(new_index, url_text_str); /* Use the new index */       
                }
                else
                {
                    if (duplicated_index >= 0)//提示"是否替换"
                    {
                        MMI_STRING_T url_string = {0};

                        MMIWWW_GetBookmarkUrl(duplicated_index, &url_string);

                        if (url_string.wstr_len == url_text_str.wstr_len && (0 == MMIWWW_Wstricmp(url_string.wstr_ptr, url_text_str.wstr_ptr)))
                        {
                            MMIPUB_OpenAlertWarningWin(TXT_WWW_BOOKMARK_EXIST);
                        }
                        else
                        {
                            MMIPUB_OpenAlertWinByTextIdEx(applet_handle, 0, TXT_WWW_BOOKMARK_QUERY_REPLACE, TXT_NULL, IMAGE_PUBWIN_QUERY, PNULL, PNULL, MMIPUB_SOFTKEY_OKCANCEL, MMIWWW_HandleReplaceDuplicatedBookmark, duplicated_index);
                        }

                        if (PNULL != url_string.wstr_ptr)
                        {
                           SCI_FREE(url_string.wstr_ptr);
                        }

                        return recode;
                    }
                    else
                    {
                        MMIWWW_AddBookmark(url_text_str, title_text_str, FALSE, &new_index);
                    }
                }
            }

            MMIWWW_ReloadBookmarkFileArray(bookmark_info_ptr->full_path, bookmark_info_ptr->full_path_len, FUNC_FIND_NULL);
            if ((MMIWWW_BM_CHANGE_EDIT_DIR == change_type) || (MMIWWW_BM_CHANGE_ADD_DIR == change_type))
            {
                current_index = MMIWWW_FindDuplicatedBookmarkDirIndex(bookmark_info_ptr->full_path, bookmark_info_ptr->full_path_len, 0, title_text_str.wstr_ptr);
            }
            else
            {
                current_index = MMIWWW_FindDuplicatedBookmarkIndex(bookmark_info_ptr->full_path, bookmark_info_ptr->full_path_len, bookmark_info_ptr->file_array_step, title_text_str.wstr_ptr);
            }
                
            if (!MMIWWW_BookmarkIsInRoot())
            {
                current_index += 1;//非root需增加"..." item 项
            }
     
            if (MMK_IsOpenWin(bm_win_handle))
            {
                if ((MMIWWW_BM_CHANGE_ADD_URL == change_type) || (MMIWWW_BM_CHANGE_ADD_CUR_PAGE== change_type))
                {
                    //for reset the bookmark list
                    MMIWWW_LoadBookmarkFileArray(BOOKMARK_STEP_TYPE_ROOT, 0, FUNC_FIND_NULL);
                }

                bm_ctrl_handle = MMK_GetCtrlHandleByWin(bm_win_handle, MMIWWW_BOOKMARK_LIST_CTRL_ID);
                MMIWWW_AppendListboxitem(bm_win_handle, MMIWWW_BOOKMARK_LIST_CTRL_ID, (uint16)MMIWWW_GetBookmarkNumber(), TXT_WWW_BOOKMARK_EMPTY, STXT_OPTION, TXT_WWW_EXIT);
                GUILIST_SetCurItemIndex(bm_ctrl_handle, current_index);
            }

            if (MMIWWW_BM_CHANGE_ADD_URL == change_type)
            {
                if (!MMK_IsOpenWin(mmiwww_GetBrowserWinhandle()) && !MMK_IsOpenWin(main_win_handle))
                {
                    MMIPUB_OpenAlertSuccessWin(TXT_WWW_BOOKMARK_SAVE_SUCCESS);
                    MMIAPIWWW_StopWWW();
                    break;
                }
            }

            if (MMK_IsOpenWin(detail_win_handle))
            {
                GUIRICHTEXT_ITEM_T item_data1 = {0};/*lint !e64*/
                GUIRICHTEXT_ITEM_T org_item_data = {0};/*lint !e64*/
                MMI_HANDLE_T rich_ctrl_handle = MMK_GetCtrlHandleByWin(detail_win_handle, MMIWWW_BOOKMARK_SHOW_RICHTEXT_CTRL_ID); 

                GUIRICHTEXT_GetItem(rich_ctrl_handle, MMIWWW_BOOKMARK_TITLE_INDEX, &org_item_data);
                SCI_MEMCPY(&item_data1, &org_item_data, sizeof(GUIRICHTEXT_ITEM_T));
                item_data1.text_data.buf.len = title_text_str.wstr_len;
                item_data1.text_data.buf.str_ptr = title_text_str.wstr_ptr;
                item_data1.text_type = GUIRICHTEXT_TEXT_BUF;
                GUIRICHTEXT_ModifyItem(rich_ctrl_handle, MMIWWW_BOOKMARK_TITLE_INDEX, &item_data1);

                SCI_MEMSET(&item_data1, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
                SCI_MEMSET(&org_item_data, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
                GUIRICHTEXT_GetItem(rich_ctrl_handle, MMIWWW_BOOKMARK_URL_INDEX, &org_item_data);
                SCI_MEMCPY(&item_data1, &org_item_data, sizeof(GUIRICHTEXT_ITEM_T));
                item_data1.text_data.buf.len = url_text_str.wstr_len;
                item_data1.text_data.buf.str_ptr = url_text_str.wstr_ptr;
                item_data1.text_type = GUIRICHTEXT_TEXT_BUF;
                GUIRICHTEXT_ModifyItem(rich_ctrl_handle, MMIWWW_BOOKMARK_URL_INDEX, &item_data1);
            }
            MMK_CloseWin(win_id);
            MMIPUB_OpenAlertSuccessWin(TXT_WWW_BOOKMARK_SAVE_SUCCESS);
        }
        break;

    case MSG_CLOSE_WINDOW:
        MMIWWW_NetfrontSetBookmarkChange(FALSE);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        if (MMIWWW_NetfrontGetBookmarkChange())
        {
            MMIPUB_OpenQueryWinByTextId(TXT_EXIT_WITHOUT_SAVE,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
        }
        else
        {
            MMIWWW_BM_CHANGE_TYPE_E change_type = (uint32)MMK_GetWinAddDataPtr(win_id);/*lint !e64*/
            MMI_HANDLE_T applet_handle = MMIWWW_HANDLE((WWW_APPLET_T *)MMK_GetAppletPtrByWin(win_id));
            MMI_HANDLE_T main_win_handle = MMK_GetWinHandle(applet_handle, MMIWWW_MAINMENU_WIN_ID);

            MMK_CloseWin(win_id);
            if (MMIWWW_BM_CHANGE_ADD_URL == change_type)
            {
                if (!MMK_IsOpenWin(mmiwww_GetBrowserWinhandle()) && !MMK_IsOpenWin(main_win_handle))
                {
                    MMIAPIWWW_StopWWW();
                }
            }
        }
        break;

    case MSG_PROMPTWIN_OK:
        {
            MMI_HANDLE_T applet_handle = MMIWWW_HANDLE((WWW_APPLET_T *)MMK_GetAppletPtrByWin(win_id));
            MMI_HANDLE_T main_win_handle = MMK_GetWinHandle(applet_handle, MMIWWW_MAINMENU_WIN_ID);
            MMIWWW_BM_CHANGE_TYPE_E change_type = (uint32)MMK_GetWinAddDataPtr(win_id);/*lint !e64*/

            MMIPUB_CloseQuerytWin(PNULL);
            MMK_CloseWin(win_id);
            if (MMIWWW_BM_CHANGE_ADD_URL == change_type)
            {
                if (!MMK_IsOpenWin(mmiwww_GetBrowserWinhandle()) && !MMK_IsOpenWin(main_win_handle))
                {
                    MMIAPIWWW_StopWWW();
                }
            }
        }
        break;

    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/****************************************************************************/    
//  Description : show the bookmark
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void MMIWWW_SetShowBookmarkDetailEntry(MMI_CTRL_ID_T ctrl_id)
{
    uint16 index = 0;
    MMI_STRING_T string = {0};
    BOOLEAN is_free = TRUE;
    WWW_APPLET_T *applet = PNULL;
    MMI_HANDLE_T win_handle = PNULL;
    MMI_HANDLE_T ctrl_handle = PNULL;

    applet = MMIWWW_APPLETINSTANCE();
    win_handle = MMK_GetWinHandle(MMIWWW_HANDLE(applet), MMIWWW_BOOKMARK_WIN_ID);
    ctrl_handle = MMK_GetCtrlHandleByWin(win_handle, MMIWWW_BOOKMARK_LIST_CTRL_ID);
    index = GetBookmarkCurIndex();

    /* Title */
    SCI_MEMSET(&string, 0, sizeof(MMI_STRING_T));
    MMIRES_GetText(TXT_COMM_TITLE, ctrl_id, &string);
    MMIWWW_RichText_AddItem(ctrl_id, &string, GUIRICHTEXT_FRAME_NONE, GUIRICHTEXT_TAG_NONE);

    SCI_MEMSET(&string, 0, sizeof(MMI_STRING_T));
    MMIWWW_GetBookmarkTitle(index, &string);
    if (PNULL != string.wstr_ptr)
    {
        is_free = TRUE;
    }
    else
    {
        /*default title*/
        MMIRES_GetText(TXT_NO_TITLE, ctrl_id, &string);
        is_free = FALSE;
    }
    MMIWWW_RichText_AddItem(ctrl_id, &string, GUIRICHTEXT_FRAME_NONE, GUIRICHTEXT_TAG_NONE);
    if (is_free && PNULL != string.wstr_ptr)
    {
        SCI_FREE(string.wstr_ptr);
    }

    /* URL */
    SCI_MEMSET(&string, 0, sizeof(MMI_STRING_T));
    MMIRES_GetText(TXT_COMMON_URL, ctrl_id, &string);
    MMIWWW_RichText_AddItem(ctrl_id, &string, GUIRICHTEXT_FRAME_NONE, GUIRICHTEXT_TAG_NONE);

    SCI_MEMSET(&string, 0, sizeof(MMI_STRING_T));
    MMIWWW_GetBookmarkUrl(index, &string);
    MMIWWW_RichText_AddItem(ctrl_id, &string, GUIRICHTEXT_FRAME_NONE, GUIRICHTEXT_TAG_NONE);
    if (PNULL != string.wstr_ptr)
    {
        SCI_FREE(string.wstr_ptr);
    }
}

/*****************************************************************************/
//  Description : handle message of showing bookmark
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleShowBookmarkWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"mmiwww_wintab.c:HandleShowBookmarkWinMsg() msg_id=0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_12856_112_2_18_3_12_27_251,(uint8*)"d", msg_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
            {
                MMI_HANDLE_T ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_BOOKMARK_SHOW_RICHTEXT_CTRL_ID);

#ifdef MMI_PDA_SUPPORT
                GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif

                MMIWWW_SetShowBookmarkDetailEntry(ctrl_handle);
                MMK_SetAtvCtrl(win_id, ctrl_handle);
            }
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        MMIWWW_Bookmark_GotoURL(GetBookmarkCurIndex());
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_MENU:
       MMIWWW_OpenWWWBookmarkDetailSubMenuWin();
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

/*****************************************************************************/
// Description : Handle replace the duplicated bookmark 
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWWW_HandleReplaceDuplicatedBookmark(MMI_WIN_ID_T      win_id, 
                                          MMI_MESSAGE_ID_E  msg_id, 
                                          DPARAM            param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMIPUB_INFO_T *win_info_ptr = PNULL;

    //SCI_TRACE_LOW:"MMIWWW_HandleReplaceDuplicatedBookmark() msg_id=0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_12910_112_2_18_3_12_27_252,(uint8*)"d", msg_id);

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
            MMI_STRING_T url_text_str = {0};
            MMI_STRING_T title_text_str = {0};
            GUIRICHTEXT_ITEM_T item_data = {0};/*lint !e64*/
            uint16 current_index = 0;
            MMI_HANDLE_T applet_handle = MMIWWW_HANDLE((WWW_APPLET_T *)MMK_GetAppletPtrByWin(win_id));
            MMI_HANDLE_T detail_win_handle = MMK_GetWinHandle(applet_handle, MMIWWW_SHOW_BOOKMARK_WIN_ID);
            MMI_HANDLE_T main_win_handle = MMK_GetWinHandle(applet_handle, MMIWWW_MAINMENU_WIN_ID);
            MMI_HANDLE_T input_win_handle = MMK_GetWinHandle(applet_handle, MMIWWW_INPUT_BOOKMARK_WIN_ID);
            MMI_HANDLE_T richtext_handle = MMK_GetCtrlHandleByWin(input_win_handle, MMIWWW_BOOKMARK_INPUT_RICHTEXT_CTRL_ID);
            MMI_HANDLE_T bm_win_handle = MMK_GetWinHandle(applet_handle, MMIWWW_BOOKMARK_WIN_ID);
            MMI_HANDLE_T bm_ctrl_handle = PNULL;
            WWW_BOOKMARK_T *bookmark_info_ptr = MMIWWW_BOOKMARK();
            uint16 out_index = 0;
            FILEARRAY_DATA_T array_item = {0};

            win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
            if (PNULL != win_info_ptr)
            {
                current_index = (uint16)win_info_ptr->user_data;
            }
 
            GUIRICHTEXT_GetItem(richtext_handle, (uint16)MMIWWW_BOOKMARK_TITLE_INDEX, &item_data);
            title_text_str.wstr_ptr = item_data.text_data.buf.str_ptr;
            title_text_str.wstr_len = item_data.text_data.buf.len;

            GUIRICHTEXT_GetItem(richtext_handle, (uint16)MMIWWW_BOOKMARK_URL_INDEX, &item_data);
            url_text_str.wstr_ptr = item_data.text_data.buf.str_ptr;
            url_text_str.wstr_len = item_data.text_data.buf.len;

            //MMIWWW_ChangeBookmarkTitle(index, title_text_str, &out_index);

            
            if (MMIAPIFILEARRAY_Read(bookmark_info_ptr->file_array, (uint32)current_index, &array_item ))
            {
                if(0 != title_text_str.wstr_ptr && 0 != MMIAPICOM_Wstrcmp(title_text_str.wstr_ptr,array_item.filename))
                {
                    MMIWWW_ChangeBookmarkTitle(current_index, title_text_str, &out_index);
                }
            }
            
            
            MMIWWW_ChangeBookmarkUrl(current_index, url_text_str); /* Use the new index */
            //current_index = (uint16)out_index;

            current_index = MMIWWW_FindDuplicatedBookmarkIndex(bookmark_info_ptr->full_path, bookmark_info_ptr->full_path_len, bookmark_info_ptr->file_array_step, title_text_str.wstr_ptr);

            if (!MMIWWW_BookmarkIsInRoot())
            {
                current_index += 1;//非root需增加"..." item 项
            }

            if (MMK_IsOpenWin(bm_win_handle))
            {
                bm_ctrl_handle = MMK_GetCtrlHandleByWin(bm_win_handle, MMIWWW_BOOKMARK_LIST_CTRL_ID);
                MMIWWW_AppendListboxitem(bm_win_handle, MMIWWW_BOOKMARK_LIST_CTRL_ID, (uint16)MMIWWW_GetBookmarkNumber(), TXT_WWW_BOOKMARK_EMPTY, STXT_OPTION, TXT_WWW_EXIT);
                GUILIST_SetCurItemIndex(bm_ctrl_handle, current_index);
            }

            if (!MMK_IsOpenWin(mmiwww_GetBrowserWinhandle()) && !MMK_IsOpenWin(main_win_handle))
            {
                MMIPUB_OpenAlertSuccessWin(TXT_WWW_BOOKMARK_SAVE_SUCCESS);
                MMIAPIWWW_StopWWW();
                break;
            }

            if (MMK_IsOpenWin(detail_win_handle))
            {
                GUIRICHTEXT_ITEM_T item_data1 = {0};/*lint !e64*/
                GUIRICHTEXT_ITEM_T org_item_data = {0};/*lint !e64*/
                MMI_HANDLE_T rich_ctrl_handle = MMK_GetCtrlHandleByWin(detail_win_handle, MMIWWW_BOOKMARK_SHOW_RICHTEXT_CTRL_ID); 

                GUIRICHTEXT_GetItem(rich_ctrl_handle, MMIWWW_BOOKMARK_TITLE_INDEX, &org_item_data);
                SCI_MEMCPY(&item_data1, &org_item_data, sizeof(GUIRICHTEXT_ITEM_T));
                item_data1.text_data.buf.len = title_text_str.wstr_len;
                item_data1.text_data.buf.str_ptr = title_text_str.wstr_ptr;
                item_data1.text_type = GUIRICHTEXT_TEXT_BUF;
                GUIRICHTEXT_ModifyItem(rich_ctrl_handle, MMIWWW_BOOKMARK_TITLE_INDEX, &item_data1);

                SCI_MEMSET(&item_data1, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
                SCI_MEMSET(&org_item_data, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
                GUIRICHTEXT_GetItem(rich_ctrl_handle, MMIWWW_BOOKMARK_URL_INDEX, &org_item_data);
                SCI_MEMCPY(&item_data1, &org_item_data, sizeof(GUIRICHTEXT_ITEM_T));
                item_data1.text_data.buf.len = url_text_str.wstr_len;
                item_data1.text_data.buf.str_ptr = url_text_str.wstr_ptr;
                item_data1.text_type = GUIRICHTEXT_TEXT_BUF;
                GUIRICHTEXT_ModifyItem(rich_ctrl_handle, MMIWWW_BOOKMARK_URL_INDEX, &item_data1);
            }
            MMK_CloseWin(win_id);
            MMIPUB_OpenAlertSuccessWin(TXT_WWW_BOOKMARK_SAVE_SUCCESS);
            MMK_CloseWin(input_win_handle);
        }
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }
    
    return recode;
}

/*****************************************************************************/
// Description : get the bookmark filearray index that need to be handled
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
LOCAL uint16 GetBookmarkCurIndex(void)
{
    return s_bookmark_cur_index_info.array_index;
}

/*****************************************************************************/
//     Description : insert string to sms edit
//    Global resource dependence : none
//  Author:
//    Note: 
/*****************************************************************************/
BOOLEAN MMIAPIWWW_InsertURL(                                            //RETURN:
                             wchar *wstr_ptr,
							 uint16 wstr_len
                             )
{
	WWW_APPLET_T *www_applet = MMIWWW_APPLETINSTANCE();
	MMI_HANDLE_T applet_handle = PNULL;
	MMI_WIN_ID_T win_handle = PNULL;
	BOOLEAN result = FALSE;
	
	if(PNULL != www_applet)
	{
		applet_handle = MMIWWW_HANDLE(MMIWWW_APPLETINSTANCE());
        win_handle = MMK_GetWinHandle(applet_handle, MMIWWW_COMMON_EDIT_WIN_ID);
		if(MMK_IsOpenWin(win_handle))
		{
			result = GUIEDIT_InsertString(MMK_GetCtrlHandleByWin(win_handle, MMIWWW_COMMON_EDIT_CTRL_ID), wstr_ptr, wstr_len);  
		}
		MMK_SendMsg(win_handle, MSG_FULL_PAINT, PNULL);
	}

	return result;
}

/*****************************************************************************/
// Description : Access homepage
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
LOCAL void mmiwww_AccessHomepage(void)
{
    char hompage[MAX_WEB_URL_LEN + 1] = {0};

    MMIWWW_GetHomepageURL(hompage, MAX_WEB_URL_LEN);

    if (0 == strlen(hompage))
    {
        MMIWWW_OpenWWWBrowserWin(BROWSER_START_BY_URL, (char *)MMIWWW_ABOUT_BLANK);
    }
    else
    {
        MMIWWW_OpenWWWBrowserWin(BROWSER_START_BY_URL, (char *)hompage);
    }
}


/*****************************************************************************/
// Description : Handle query Use WIFI
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIWWW_HandleSelectWLAN(MMI_WIN_ID_T      win_id, 
                                          MMI_MESSAGE_ID_E  msg_id, 
                                          DPARAM            param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"MMIWWW_HandleSelectWLAN() msg_id=0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_13094_112_2_18_3_12_28_253,(uint8*)"d", msg_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {

            result = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        }
        break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        {
            WWW_APPLET_T* www_applet_ptr = MMIWWW_APPLETINSTANCE();

            if (PNULL != www_applet_ptr)
            {
                if (MSG_APP_CANCEL == msg_id || MSG_CTL_CANCEL == msg_id)
                {
                    www_applet_ptr->www_instance.customized_connection.bearer_type = MMIPDP_INTERFACE_GPRS;
                }
                else
                {
                    www_applet_ptr->www_instance.customized_connection.bearer_type = MMIPDP_INTERFACE_WIFI;
                }

                switch (www_applet_ptr->www_instance.enter_type)
                {
                case MMIBROWSER_ACCESS_MAINMENU:
                    MMIWWW_OpenWWWMainMenuWin(MMIWWW_CTRL_ADDRESS_LABEL);
                    break;

                case MMIBROWSER_ACCESS_URL:
                    {
                        uint8 *save_url = PNULL;
                        NFBrowserStartType start_type = BROWSER_START_NORMAL;

                        save_url = (uint8 *)MMIWWW_GetSavedUrl(&start_type);

                        if (!MMIWWW_OpenWWWBrowserWin(BROWSER_START_BY_URL, (char *)save_url))
                        {
                            if (MMIWWW_IsNeedStopApplet())
                            {
                                MMIAPIWWW_StopWWW();
                            }
                        }
                    }
                    break;

                 default:
                    break;
                }
                
                MMIWWW_NETCONNECT();
            }

            MMK_CloseWin(win_id);
        }
        break;

    default:
        result = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }
    return (result);
}
/*****************************************************************************/
//  Description : Create shotcut Menu
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void mmiwww_CreateShotcutMenu(MMI_WIN_ID_T  win_id)
{
    int32 i = 0;
    GUIMENU_DYNA_ITEM_T node_item = {0};
    int32 len = 0;
    wchar name_wchar[MMIWWW_SHOTCUTMENU_NAME_MAX + 1] = {0};
    wchar * temp = PNULL;
    MMI_STRING_T name_str = {0};
    MMI_HANDLE_T ctrl_handle = PNULL;
    MMI_CONTROL_CREATE_T create = {0};
    GUIMENU_INIT_DATA_T menu_data = {0};
    int32 item_num = 0;
    int32 menu_item[][3] = 
        {
#ifdef MOTION_SENSOR_TYPE
            {MMIWWW_SHOTCUT_HORIZONTAL_DISPLAY, TXT_DL_DOWNLOAD_MANAGER,   ID_WWW_SHOTCUT_KEY_1},
#else
            {MMIWWW_SHOTCUT_HORIZONTAL_DISPLAY, TXT_WWW_PAGE_HORIZONTAL_DISPLAY,   ID_WWW_SHOTCUT_KEY_1},
#endif /* MOTION_SENSOR_TYPE */
            {MMIWWW_SHOTCUT_PAGEUP,             TXT_WWW_PAGEUP,                     ID_WWW_SHOTCUT_KEY_2},
            {MMIWWW_SHOTCUT_REFRESH,            TXT_REFRESH,            ID_WWW_SHOTCUT_KEY_3},
            {MMIWWW_SHOTCUT_PAGE_BACK,          TXT_COMMON_COMMON_BACK,                  ID_WWW_SHOTCUT_KEY_4},
            {MMIWWW_SHOTCUT_FULL_DISPLAY,       TXT_WWW_PAGE_FULL_DISPLAY,          ID_WWW_SHOTCUT_KEY_5},
            {MMIWWW_SHOTCUT_PAGE_FORWARD,       TXT_COMMON_COMMON_FORWARD,               ID_WWW_SHOTCUT_KEY_6},
            {MMIWWW_SHOTCUT_ZOOMOUT,            TXT_WWW_ZOOMOUT,                    ID_WWW_SHOTCUT_KEY_7},
            {MMIWWW_SHOTCUT_PAGEDOWN,           TXT_WWW_PAGEDOWN,                   ID_WWW_SHOTCUT_KEY_8},
            {MMIWWW_SHOTCUT_ZOOMIN,             TXT_WWW_ZOOMIN,                     ID_WWW_SHOTCUT_KEY_9},
        {MMIWWW_SHOTCUT_SWITCH_PAGE,        TXT_WWW_WINDOW_MANAGER,             ID_WWW_SHOTCUT_KEY_0},
//        {MMIWWW_SHOTCUT_SWITCH_WINDOW,      TXT_WWW_WINDOW_MANAGER,             ID_WWW_SHOTCUT_KEY_STAR}
        };

    item_num = sizeof(menu_item)/sizeof(menu_item[0]);

    menu_data.is_static = FALSE;
    menu_data.menu_style = GUIMENU_STYLE_POPUP;
    menu_data.node_max_num = item_num + 1;

    create.ctrl_id = MMIWWW_SHUTCUT_KEY_OPT_CTRL_ID;
    create.guid = SPRD_GUI_MENU_ID;
    create.parent_win_handle = win_id;
    create.init_data_ptr = &menu_data;
    MMK_CreateControl(&create);

    ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_SHUTCUT_KEY_OPT_CTRL_ID);

    if (MMIWWW_IsFullDisplay())
    {
        for(i = 0; i < item_num; i++)
        {
            if (MMIWWW_SHOTCUT_FULL_DISPLAY == menu_item[i][0])
            {
                menu_item[i][1] = TXT_WWW_PAGE_EXIT_FULL_DISPLAY;
            }
        }

    }

    for(i = 0; i < item_num; i++)
    {
        SCI_MEMSET(&name_str, 0, sizeof(name_str));
        SCI_MEMSET(name_wchar, 0, sizeof(name_wchar));
        SCI_MEMSET(&node_item, 0, sizeof(node_item));

        temp = name_wchar;
        len = MMIWWW_SHOTCUTMENU_NAME_MAX - 2;
        MMIRES_GetText(menu_item[i][1], win_id, &name_str);

        if (ID_WWW_SHOTCUT_KEY_0 == menu_item[i][2])
        {
            *temp++ = '0';
        }
        else if (ID_WWW_SHOTCUT_KEY_STAR == menu_item[i][2])
        {
            *temp++ = '*';
        }
        else
        {
            *temp++ = '1' + i;
        }

        *temp++ = ' ';

        len = MIN(len, name_str.wstr_len);
        MMIAPICOM_Wstrncpy(temp, name_str.wstr_ptr, len);
        name_str.wstr_ptr = name_wchar;
        name_str.wstr_len = MMIAPICOM_Wstrlen(name_wchar);
        node_item.is_grayed = mmiwww_CheckMenuGrayed(menu_item[i][0]);
        node_item.item_text_ptr = &name_str;
        GUIMENU_InsertNode(i, menu_item[i][2], 0, &node_item, ctrl_handle);
    }
}

/*****************************************************************************/
//  Description : select network type(sim or wifi)
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_SelectNetworkType(BOOLEAN is_include_wifi)
{   
#if defined(MMI_WIFI_SUPPORT) || !defined(MMI_MULTI_SIM_SYS_SINGLE)
    if (MMK_IsOpenWin(MMIWWW_SELECTSIM_WIN_ID))
    {
        MMK_CloseWin(MMIWWW_SELECTSIM_WIN_ID);
    }

    MMK_CreateWin((uint32*)MMIWWW_SELECTSIM_WIN_TAB, (ADD_DATA)is_include_wifi);
#endif    
}


#if defined(MMI_WIFI_SUPPORT) || !defined(MMI_MULTI_SIM_SYS_SINGLE) 
/*****************************************************************************/
//  Description : handle sim select window message
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E WWWHandleSIMSelectWinMsg(
                                           MMI_WIN_ID_T	win_id, 
                                           MMI_MESSAGE_ID_E	msg_id, 
                                           DPARAM param
                                         )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    uint16          list_index = 0;
    uint32          item_data = 0;
    uint32 is_include_wifi = FALSE;   
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            is_include_wifi = (uint32)MMK_GetWinAddDataPtr(win_id);
#ifdef MMI_WIFI_SUPPORT
            if (is_include_wifi)
            {
                MMIAPIWIFI_AppendSelectListItem(MMIWWW_SELECT_SIM_CTRL_ID);
            }
            else
            {
                GUILIST_SetMaxItem(MMIWWW_SELECT_SIM_CTRL_ID, MMI_DUAL_SYS_MAX, FALSE);
                MMIAPISET_AppendMultiSimName(MMIWWW_SELECT_SIM_CTRL_ID, TXT_COMMON_OK, MMI_DUAL_SYS_MAX, MMISET_APPEND_SYS_OK);
            }
#else
            GUILIST_SetMaxItem(MMIWWW_SELECT_SIM_CTRL_ID, MMI_DUAL_SYS_MAX, FALSE);
            MMIAPISET_AppendMultiSimName(MMIWWW_SELECT_SIM_CTRL_ID, TXT_COMMON_OK, MMI_DUAL_SYS_MAX, MMISET_APPEND_SYS_OK);
#endif
#ifndef MMI_WIFI_SUPPORT
            GUITITLE_SetTextId(win_id,MMITHEME_GetTitleCtrlId(),TXT_SIM_SEL,FALSE);
#else
            GUITITLE_SetTextId(win_id,MMITHEME_GetTitleCtrlId(),TXT_NETWORK_SELECT,FALSE);
#endif
#if !defined(MMI_MULTI_SIM_SYS_SINGLE) 
            MMIAPIPHONE_SetNeedUpdateNetworkStatusWin(win_id);
#endif
            MMK_SetAtvCtrl(win_id, MMIWWW_SELECT_SIM_CTRL_ID);
        }
        break;
        
    case MSG_FULL_PAINT:
        {
            MMI_CTRL_ID_T ctrl_id = MMIWWW_SELECT_SIM_CTRL_ID;
            uint16 cur_selection = 0;
            cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
#ifdef MMI_WIFI_SUPPORT            
            MMIAPIWIFI_AppendSelectListItem(ctrl_id);
#endif            
            GUILIST_SetCurItemIndex(ctrl_id, cur_selection);
        }
        break;

#if !defined(MMI_MULTI_SIM_SYS_SINGLE) 
    case MSG_GET_FOCUS:
        MMIAPIPHONE_SetNeedUpdateNetworkStatusWin(win_id);
        break;
#endif          
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:    
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
        {
            WWW_APPLET_T* www_applet_ptr = MMIWWW_APPLETINSTANCE();

            if (PNULL != www_applet_ptr)
            {
                list_index = GUILIST_GetCurItemIndex(MMIWWW_SELECT_SIM_CTRL_ID);

                if(GUILIST_IsCurrItemInvalid(MMIWWW_SELECT_SIM_CTRL_ID))
                {
                    break;
                }
                
                GUILIST_GetItemData(MMIWWW_SELECT_SIM_CTRL_ID, list_index, &item_data);
                
                if( item_data < MMI_DUAL_SYS_MAX )
                {
                    www_applet_ptr->www_instance.customized_connection.bearer_type = MMIPDP_INTERFACE_GPRS;
                    www_applet_ptr->www_instance.customized_connection.dual_sys = (MN_DUAL_SYS_E)item_data;
                }    
                else//WIFI
                {
                    www_applet_ptr->www_instance.customized_connection.bearer_type = MMIPDP_INTERFACE_WIFI;
                }
                
                switch (www_applet_ptr->www_instance.enter_type)
                {
                case MMIBROWSER_ACCESS_MAINMENU:
                    MMIWWW_OpenWWWMainMenuWin(MMIWWW_CTRL_ADDRESS_LABEL);
                    break;

                case MMIBROWSER_ACCESS_URL:
                    {
                        uint8 *save_url = PNULL;
                        NFBrowserStartType start_type = BROWSER_START_NORMAL;

                        save_url = (uint8 *)MMIWWW_GetSavedUrl(&start_type);

                        if (!MMIWWW_OpenWWWBrowserWin(BROWSER_START_BY_URL, (char *)save_url))
                        {
                            if (MMIWWW_IsNeedStopApplet())
                            {
                                MMIAPIWWW_StopWWW();
                            }
                        }
                    }
                    break;

                 default:
                    break;
                }
                
                MMIWWW_NETCONNECT();
            }

            MMK_CloseWin(win_id);
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMIAPIWWW_StopWWW();
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
//  Description : handle message of Window mangaer
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWWindowManagerWinMsg(
                                           MMI_WIN_ID_T	win_id, 
                                           MMI_MESSAGE_ID_E	msg_id, 
                                           DPARAM param
                                         )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
        MMK_SetAtvCtrl(win_id, MMIWWW_WINDOWN_MANAER_LIST_CTRL_ID);
        MMK_SendMsg(win_id, MSG_WWW_RESET_LIST, PNULL);
        break;

    case MSG_WWW_RESET_LIST:
        {
            int32 win_num = 0;
            int32 active_index = 0;

            win_num = MMIWWW_Netfront_GetWinNum();
            active_index = MMIWWW_Netfront_GetActWinIndex();

            if (win_num < 0)
            {
                win_num = 0;
            }

            if (active_index < 0)
            {
                active_index = 0;
            }

            MMIWWW_AppendListboxitem(win_id, MMIWWW_WINDOWN_MANAER_LIST_CTRL_ID, win_num, TXT_NULL, STXT_OPTION, STXT_RETURN);
            GUILIST_SetCurItemIndex(MMK_GetCtrlHandleByWin(win_id, MMIWWW_WINDOWN_MANAER_LIST_CTRL_ID), active_index);
        }
        break;

    case MSG_CTL_LIST_NEED_ITEM_DATA:
        {
            MMI_STRING_T page_info = {0};
            BOOLEAN ret = FALSE;
            int32 index = 0;
            BOOLEAN is_free = TRUE;

            if (PNULL == param)
            {
                break;
            }
            
            index =  ((GUILIST_NEED_ITEM_DATA_T*)param)->item_index;

            ret = MMIWWW_Netfront_GetPageTitleByIndex(index, &page_info);
            if (!ret)
            {
                ret =MMIWWW_Netfront_GetPageAddressByIndex(index, &page_info);
            }
            
            if (!ret)
            {
                MMIRES_GetText(TXT_NO_TITLE, win_id, &page_info);
                is_free = FALSE;
            }

            MMIWWW_AppendTitleItemData(MMK_GetCtrlHandleByWin(win_id, MMIWWW_WINDOWN_MANAER_LIST_CTRL_ID), index, page_info, IMAGE_WWW_PAGE, STXT_OPTION, TXT_COMM_OPEN, STXT_RETURN);
            
            if (is_free && PNULL != page_info.wstr_ptr)
            {
                SCI_FREE(page_info.wstr_ptr);
            }
        }
        break;

    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        {
            uint16 current_index = 0;

            current_index = GUILIST_GetCurItemIndex(MMK_GetCtrlHandleByWin(win_id, MMIWWW_WINDOWN_MANAER_LIST_CTRL_ID));

            MMIWWW_Netfront_ActWinByIndex(current_index);
            MMK_CloseWin(win_id);
            MMK_WinGrabFocus(mmiwww_GetBrowserWinhandle());
        }
        break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_MENU:
        mmiwww_OpenWindowManagerOptWin();
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        break;
    }

    return result;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWWindowManagerOptWinMsg(
                                           MMI_WIN_ID_T	win_id, 
                                           MMI_MESSAGE_ID_E	msg_id, 
                                           DPARAM param
                                         )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            int32 num = 0;

            num = MMIWWW_Netfront_GetWinNum();
            if (num > 1)
            {
                GUIMENU_SetItemGrayed(MMIWWW_POPMENU_WINDOW_MANAGER_OPT_CTRL_ID, MENU_WWW_WINDOW_MANAGER, ID_WWW_CLOSE_SUB_WIN, FALSE);
            }
            else
            {
                GUIMENU_SetItemGrayed(MMIWWW_POPMENU_WINDOW_MANAGER_OPT_CTRL_ID, MENU_WWW_WINDOW_MANAGER, ID_WWW_CLOSE_SUB_WIN, TRUE);
            }

            if (num < MMIWWW_BROWSER_MAX_TAB)
            {
                GUIMENU_SetItemGrayed(MMIWWW_POPMENU_WINDOW_MANAGER_OPT_CTRL_ID, MENU_WWW_WINDOW_MANAGER, ID_WWW_NEW_SUB_WIN, FALSE);
            }
            else
            {
                GUIMENU_SetItemGrayed(MMIWWW_POPMENU_WINDOW_MANAGER_OPT_CTRL_ID, MENU_WWW_WINDOW_MANAGER, ID_WWW_NEW_SUB_WIN, TRUE);
            }

        }
        MMK_SetAtvCtrl(win_id, MMIWWW_POPMENU_WINDOW_MANAGER_OPT_CTRL_ID);

        break;

    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:  
        {
            MMI_MENU_ID_T          menu_id = 0;
            MMI_MENU_GROUP_ID_T    group_id = 0;

            GUIMENU_GetId(MMK_GetCtrlHandleByWin(win_id, MMIWWW_POPMENU_WINDOW_MANAGER_OPT_CTRL_ID),&group_id,&menu_id);
            switch ( menu_id )
            {
            case ID_WWW_NEW_SUB_WIN:
                {
                    WWW_APPLET_T *applet = PNULL;
                    MMI_HANDLE_T applet_handle = PNULL;
                    MMI_HANDLE_T win_mgr_handle = PNULL;

                    applet = MMIWWW_APPLETINSTANCE();
                    applet_handle = MMIWWW_HANDLE(applet);
                    win_mgr_handle = MMK_GetWinHandle(applet_handle, MMIWWW_WINDOW_MANAGER_WIN_ID);

                    MMIWWW_OpenWWWMainMenuWin(MMIWWW_CTRL_ADDRESS_LABEL);
                    MMK_CloseWin(win_id);
                    MMK_CloseWin(win_mgr_handle);
                }
                break;

            case ID_WWW_CLOSE_SUB_WIN:
                {
                    MMIPUB_OpenQueryWinByTextId(TXT_WWW_CLOSE_WEBPAGE, IMAGE_PUBWIN_QUERY,PNULL,PNULL);
                }
                break;

            default:
                break;
            }
        }
        break;

    case MSG_PROMPTWIN_OK:
        {
            WWW_APPLET_T *applet = PNULL;
            MMI_HANDLE_T applet_handle = PNULL;
            MMI_HANDLE_T win_mgr_handle = PNULL;
            MMI_HANDLE_T ctrl_hanle = PNULL;
            uint16 current_index = 0;
            int32 num = 0;

            applet = MMIWWW_APPLETINSTANCE();
            applet_handle = MMIWWW_HANDLE(applet);
            win_mgr_handle = MMK_GetWinHandle(applet_handle, MMIWWW_WINDOW_MANAGER_WIN_ID);
            ctrl_hanle = MMK_GetCtrlHandleByWin(win_mgr_handle, MMIWWW_WINDOWN_MANAER_LIST_CTRL_ID);

            MMIPUB_CloseQuerytWin(PNULL);
            MMK_CloseWin(win_id);
            num = MMIWWW_Netfront_GetWinNum();
            if (num > 0)
            {
                current_index = GUILIST_GetCurItemIndex(ctrl_hanle);
                mmiwww_DeleteSubWindow(current_index);
                GUILIST_RemoveItem(ctrl_hanle, current_index);
            }

            MMK_PostMsg(win_mgr_handle, MSG_WWW_RESET_LIST, PNULL, 0);
        }
        break;

    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        break;
    }

    return result;
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void mmiwww_DeleteSubWindow(int32 index)
{
    int32 win_num = 0;
    int32 i  = 0;
    MMIWWW_INSTANCE_T *www_instance = PNULL;
    WWW_APPLET_T *applet = MMIWWW_APPLETINSTANCE();
    MMI_HANDLE_T applet_handle = PNULL;
    MMI_HANDLE_T brw_win_handle = PNULL;

    win_num = MMIWWW_Netfront_GetWinNum();
    www_instance = &applet->www_instance;
    applet_handle = MMIWWW_HANDLE(applet);
    brw_win_handle = mmiwww_GetBrowserWinhandle();

    if (index >= win_num)
    {
        //SCI_TRACE_LOW:"mmiwww_DeleteSubWindow err index  = %d, win_num = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_WINTAB_13678_112_2_18_3_12_29_254,(uint8*)"dd", index, win_num);
        return;
    }

    if (win_num > 1)
    {
        //stop bg sound
        if (index < MMIWWW_BROWSER_MAX_TAB && www_instance->is_bg_sound[index])
        {
            www_instance->is_bg_sound[index] = slim_false;
            MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
        }

        MMIWWW_Netfront_PageZoom(MMIWWW_ZOOMORG);
        MMIWWW_Netfront_DelWin(index);

        for (i = index; i < MMIWWW_BROWSER_MAX_TAB -1; i++)
        {
            MMIWWW_SetPageSecureFlag(i, MMIWWW_GetPageSecureFlag(i + 1));
            MMIWWW_SetWWWState(i, MMIWWW_GetWWWState(i + 1));
        }
        MMIWWW_SetPageSecureFlag(i, FALSE);
        MMIWWW_SetWWWState(i, MMIWWW_STATE_PAGE_LOADING);
    }
    else
    {
        for (i =0; i< MMIWWW_BROWSER_MAX_TAB; i++)
        {
            MMIWWW_SetPageSecureFlag(i, FALSE);
            MMIWWW_SetWWWState(i, MMIWWW_STATE_PAGE_LOADING);
        }

        MMK_CloseWin(brw_win_handle);

        if (PNULL != applet_handle && !MMK_IsOpenWin(MMK_GetWinHandle(applet_handle, MMIWWW_MAINMENU_WIN_ID)))
        {
            MMIAPIWWW_StopWWW();
        }
    }
}

#ifdef MMI_WIFI_SUPPORT
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleQueryGPRSWinMsg(
                                           MMI_WIN_ID_T	win_id, 
                                           MMI_MESSAGE_ID_E	msg_id, 
                                           DPARAM param
                                         )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    static BOOLEAN is_check = TRUE;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        mmiwww_SetQueryGPRSWindowProperty(win_id);
        MMK_SetAtvCtrl(win_id, MMIWWW_QUERY_NEVER_NOTIFY_LABEL_CTRL_ID);
        is_check = TRUE;
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
        {
            WWW_APPLET_T* www_applet_ptr = MMIWWW_APPLETINSTANCE();

            if (PNULL != www_applet_ptr)
            {

                www_applet_ptr->www_instance.customized_connection.bearer_type = MMIPDP_INTERFACE_GPRS;

                switch (www_applet_ptr->www_instance.enter_type)
                {
                case MMIBROWSER_ACCESS_MAINMENU:
                    MMIWWW_OpenWWWMainMenuWin(MMIWWW_CTRL_ADDRESS_LABEL);
                    break;

                case MMIBROWSER_ACCESS_URL:
                    {
                    uint8 *save_url = PNULL;
                    NFBrowserStartType start_type = BROWSER_START_NORMAL;

                    save_url = (uint8 *)MMIWWW_GetSavedUrl(&start_type);

                    if (!MMIWWW_OpenWWWBrowserWin(BROWSER_START_BY_URL, (char *)save_url))
                    {
                        if (MMIWWW_IsNeedStopApplet())
                        {
                            MMIAPIWWW_StopWWW();
                        }
                    }
                    }
                    break;

                default:
                    break;
                }

                MMIWWW_NETCONNECT();
            }

            MMK_CloseWin(win_id);
        }
        break;

    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
            {
                MMI_HANDLE_T ctrl_handle = PNULL;
                MMI_IMAGE_ID_T image_id = IMAGE_NULL;

                ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_QUERY_NEVER_NOTIFY_LABEL_CTRL_ID);

                is_check = !is_check;

                if (is_check)
                {
                    image_id = IMAGE_CHECK_SELECTED_ICON;
                }
                else
                {
                    image_id = IMAGE_CHECK_UNSELECTED_ICON;
                }

                GUILABEL_SetIcon(ctrl_handle, image_id);
                GUILABEL_SetTextById(ctrl_handle, TXT_WWW_NEVER_NOTIFY, TRUE);

            }
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMIAPIWWW_StopWWW();
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        #ifdef WLAN_RESELECT_SUPPORT    
        MMIAPIWIFI_SetIsShowNoWlanPrompt((BOOLEAN)!is_check);
        #endif
        break;

    default:        
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return result;
}
#endif

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL void mmiwww_SetQueryGPRSWindowProperty(MMI_WIN_ID_T win_id)
{
    MMI_HANDLE_T ctrl_handle = PNULL;
    MMI_STRING_T notify_str = {0};

    ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_QUERY_GPRS_NOTIFICATION_LABEL_CTRL_ID);
    MMIRES_GetText(TXT_WWW_SELECT_NETWORK_NOTIFICATION, win_id, &notify_str);
    GUIEDIT_SetString(ctrl_handle, notify_str.wstr_ptr, notify_str.wstr_len);

    ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIWWW_QUERY_NEVER_NOTIFY_LABEL_CTRL_ID);
    GUILABEL_SetIcon(MMIWWW_QUERY_NEVER_NOTIFY_LABEL_CTRL_ID, IMAGE_CHECK_SELECTED_ICON);
    GUILABEL_SetTextById(MMIWWW_QUERY_NEVER_NOTIFY_LABEL_CTRL_ID, TXT_WWW_NEVER_NOTIFY, FALSE);
}

#endif //#ifdef BROWSER_SUPPORT_NETFRONT


/*Edit by script, ignore 10 case. Thu Apr 26 19:01:12 2012*/ //IGNORE9527


/*Edit by script, ignore 40 case. Fri Apr 27 09:38:56 2012*/ //IGNORE9527
