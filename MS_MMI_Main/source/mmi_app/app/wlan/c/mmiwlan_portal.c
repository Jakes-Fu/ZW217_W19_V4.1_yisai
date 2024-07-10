/*****************************************************************************
** File Name:      mmiwlan_portal.c                                         *
** Author:         wancan.you                                                     *
** Date:           2011.03.15                                                *
** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2011.03.15     wancan.you            Create                                    *
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_wlan_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "sci_types.h"
#include "mmk_type.h"
#include "window_parse.h"
#include "mmiwlan_id.h"
#include "mmiwlan_text.h"
#include "mmidisplay_data.h"
#include "guilistbox.h"
#include "mmk_app.h"
#include "mmi_modu_main.h"
#include "mmipub.h"
#include "mmiwlan_manager.h"
#include "mmiwlan_win.h"
#include "mmiwifi_export.h"
#include "mmiwlan_menutable.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmifilearray_export.h"
#include "guilabel.h"
#include "mmi_default.h"
#include "guidropdownlist.h"
#include "guirichtext.h"
#include "mmifmm_export.h"
#include "mmiwlan_image.h"
#include "mmiwlan_anim.h"
#include "cfl_char.h"
#include "mmiwlan_nv.h"
#include "mmiidle_export.h"
#include "mmk_msg.h"
#include "mmi_mainmenu_export.h"
#include "guiform.h"
#include "guitext.h"
#include "mmiwlan_internal.h"
#include "mmiwlan_portal.h"
#include "mmiwifi_export.h"
#include "mmisms_text.h"
#include "mmisms_export.h"
#include "mmistk_text.h"
#include "mmi_resource.h"

/**-------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMIWLAN_PORTAL_STATUS_TIMEOUT 500
#define MMIWLAN_PORTAL_TIME_STR_LEN_MAX 50 // 00:00:00
#define MMIWLAN_PORTAL_LOGIN_NOTIFICATION_MAX      (100)
/**-------------------------------------------------------------------------*
**                         ENUM DEFINITION                                  *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         TYPE DEFINITION                                  *
**--------------------------------------------------------------------------*/
typedef void (*PORTAL_ITEM_FUNC)(void);

/**-------------------------------------------------------------------------*
**                         GLOBAL VARIABLE DEFINITION                       *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         STATIC CLASS DEFINITION                          *
**--------------------------------------------------------------------------*/
LOCAL MMIWIFI_PORTAL_CONTEXT_T s_wlan_context;
LOCAL wchar s_update_file[]   = {'D' , ':', '\\', 'O','t','h','e','r', 's','\\', 'w', 'l', 'a','n','_','u','p','d','a','t','e','.','p','a','c',0 };

/**-------------------------------------------------------------------------*
**                         STATIC VARIABLE DEFINITION                       *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         LOCAL FUNCTION DECLARE                           *
**--------------------------------------------------------------------------*/
/****************************************************************************/
//  Description : handle portal main window
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E HandlePortalMainWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/****************************************************************************/
//  Description : Save Portal Config Input
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/****************************************************************************/
LOCAL void SavePortalConfigInput(void);

/****************************************************************************/
//  Description : Set Portal Main Win Property
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/****************************************************************************/
LOCAL void SetPortalMainWinProperty(void);

/****************************************************************************/
//  Description : handle portal status window
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E HandlePortalStatusWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/****************************************************************************/
//  Description : Set Portal Status Win Property
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/****************************************************************************/
LOCAL void SetPortalStatusWinProperty(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : handle Portal timer
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
LOCAL void HandleWlanPortalTimer(uint8 timer_id, uint32 param);

/****************************************************************************/
//  Description : handle portal help window
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E HandlePortalHelpWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/****************************************************************************/
//  Description : Set Portal Help Win Property
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/****************************************************************************/
LOCAL void SetPortalHelpWinProperty(MMI_CTRL_ID_T ctrl_id, MMI_TEXT_ID_T text_id);

/*****************************************************************************/
//     Description : Callback function to Wlan msg
//    Global resource dependence : 
//  Author:wancan.you
//    Note: 
/*****************************************************************************/ 
LOCAL void WlanMsgPortalCallback(MMIWIFI_NOTIFY_EVENT_E event_id, MMIWIFI_NOTIFY_PARAM_T* param_ptr);

/*****************************************************************************/
//     Description : Start Wlan Operation
//    Global resource dependence : 
//  Author:wancan.you
//    Note: 
/*****************************************************************************/ 
LOCAL void StartWlanOperation(void);

/*****************************************************************************/
//     Description : Implement Wlan Operation
//    Global resource dependence : 
//  Author:wancan.you
//    Note: 
/*****************************************************************************/ 
LOCAL void ImplementWlanOperation(void);

/*****************************************************************************/
//     Description :
//    Global resource dependence : 
//  Author:Jiaoyou.wu
//    Note: 
/*****************************************************************************/ 
LOCAL MMI_RESULT_E HandleCMCCAutoLoginQueryWinMsg(MMI_WIN_ID_T      win_id, 
                                          MMI_MESSAGE_ID_E  msg_id, 
                                          DPARAM            param);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePortalingWaitWinMsg(
                                              MMI_WIN_ID_T win_id, 
                                              MMI_MESSAGE_ID_E msg_id, 
                                              DPARAM param
                                              );

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleOfflineWaitWinMsg(
                                              MMI_WIN_ID_T win_id, 
                                              MMI_MESSAGE_ID_E msg_id, 
                                              DPARAM param
                                              );

/****************************************************************************/
//  Description : Open Portal Win
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/****************************************************************************/
LOCAL void WIFI_OpenPortalWin(MMIWLAN_PORTAL_WIN_E win_type);


/****************************************************************************/
//  Description : active service
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  HandlePortalActiveService(
                                              MMI_WIN_ID_T win_id, 
                                              MMI_MESSAGE_ID_E msg_id, 
                                              DPARAM param
                                              );

/****************************************************************************/
//  Description : modify password
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  HandlePortalResetPassword(
                                              MMI_WIN_ID_T win_id, 
                                              MMI_MESSAGE_ID_E msg_id, 
                                              DPARAM param
                                              );

/****************************************************************************/
//  Description : modify password
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/****************************************************************************/
#ifndef MMI_MULTI_SIM_SYS_SINGLE
LOCAL MMI_RESULT_E  PortalSelectSimCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param);
#endif

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePortalLoginQueryWinMsg(
                                           MMI_WIN_ID_T	win_id, 
                                           MMI_MESSAGE_ID_E	msg_id, 
                                           DPARAM param
                                         );

/****************************************************************************/
//  Description : modify password
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  HandlePasswordInputWinMsg(
                                              MMI_WIN_ID_T win_id, 
                                              MMI_MESSAGE_ID_E msg_id, 
                                              DPARAM param
                                              );

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL int32 wifi_ParseUpdateFile(char *file_ptr);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL BOOLEAN wifi_ParseXML(char *xml_file_ptr, char *tag, char *value, int32 max_len);

#ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL void AppendOneTextOneImageItem(MMI_CTRL_ID_T ctrl_id, MMI_TEXT_ID_T text_1, MMI_IMAGE_ID_T image_id, uint32 user_data);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL void  WifiPortalhandleAccount(void);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL void  WifiPortalhandleNotify(void);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL void  WifiPortalhandleActiveService(void);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL void  WifiPortalhandleUpdate(void);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL void  WifiPortalhandleHelp(void);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL void  WifiPortalhandleResetPassword(void);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL void  WifiPortalhandleModifyPassword(void);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL void  WifiPortalhandleSubscibeService(void);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E HandlePortalAccountWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#endif /* MMI_GUI_STYLE_TYPICAL */

/****************************************************************************/
//  Description : Select SIM API for WLAN app
//  Global resource dependence : 
//  Author:sam.hua
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E MMIWLAN_SelectSIMFunc(MMIAPIPHONE_SELECTSIM_FUNC MMIWLANSelectSIMWinCallback,
                                        MMISET_APPEND_SIM_TYPE_E append_type,
                                        ADD_DATA        add_data
                                        );


/*****************************************************************************/
//  Description : wlan send sms callback function
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN WlanSMSSendCnfCallback(BOOLEAN is_succss, DPARAM parm);

/**-------------------------------------------------------------------------*
**                         STATIC VARIABLE DEFINITION                       *
**--------------------------------------------------------------------------*/
WINDOW_TABLE(MMIWLAN_PORTAL_MAIN_WIN_TAB) = 
{
    WIN_FUNC((uint32) HandlePortalMainWinMsg), 
    WIN_ID(MMIWLAN_PORTAL_MAIN_WIN_ID),
    WIN_STATUSBAR,
    WIN_TITLE(TXT_WLAN_PORTAL),
    WIN_SOFTKEY(TXT_COMM_LOGIN, TXT_NULL, STXT_EXIT),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif /* MMI_PDA_SUPPORT */
#ifndef MMI_GUI_STYLE_TYPICAL
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIWLAN_LIST_PORTAL_MAIN_CTRL_ID),
#else
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIWIFI_WLAN_PORTAL_FORM_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_WLAN_PORTAL_USER_NAME_FORMCTRL_ID,MMIWIFI_WLAN_PORTAL_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_WLAN_PORTAL_CHILD_USER_NAME_LABEL_CTRL_ID, MMIWIFI_WLAN_PORTAL_USER_NAME_FORMCTRL_ID),
    CHILD_EDIT_TEXT_CTRL(TRUE, MMIWLAN_PORTAL_USER_NAME_MAX_LEN,MMIWIFI_WLAN_PORTAL_CHILD_USER_NAME_EDIT_CTRL_ID, MMIWIFI_WLAN_PORTAL_USER_NAME_FORMCTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_WLAN_PORTAL_PASSWORD_FORMCTRL_ID,MMIWIFI_WLAN_PORTAL_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_WLAN_PORTAL_CHILD_PASSOWRD_LABEL_CTRL_ID, MMIWIFI_WLAN_PORTAL_PASSWORD_FORMCTRL_ID),
    CHILD_EDIT_PASSWORD_CTRL(TRUE, MMIWLAN_PORTAL_PASSWORD_MAX_LEN, MMIWIFI_WLAN_PORTAL_CHILD_PASSOWRD_EDIT_CTRL_ID, MMIWIFI_WLAN_PORTAL_PASSWORD_FORMCTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_WLAN_PORTAL_REMEMBER_PASSWORD_FORMCTRL_ID,MMIWIFI_WLAN_PORTAL_FORM_CTRL_ID),    
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, MMIWIFI_WLAN_PORTAL_CHILD_REMEBER_PASSWORD_CTRL_ID, MMIWIFI_WLAN_PORTAL_REMEMBER_PASSWORD_FORMCTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_WLAN_PORTAL_AUTO_LOGIN_FORMCTRL_ID,MMIWIFI_WLAN_PORTAL_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, MMIWIFI_WLAN_PORTAL_CHILD_AUTO_LOGIN_CTRL_ID, MMIWIFI_WLAN_PORTAL_AUTO_LOGIN_FORMCTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_WLAN_PORTAL_NOTIFICATION_FORMCTRL_ID,MMIWIFI_WLAN_PORTAL_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, MMIWIFI_WLAN_PORTAL_CHILD_NOTIFICATION_CTRL_ID, MMIWIFI_WLAN_PORTAL_NOTIFICATION_FORMCTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_WLAN_PORTAL_ACTIVE_SERVICE_FORMCTRL_ID,MMIWIFI_WLAN_PORTAL_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, MMIWIFI_WLAN_PORTAL_CHILD_ACTIVE_SERVICE_CTRL_ID, MMIWIFI_WLAN_PORTAL_ACTIVE_SERVICE_FORMCTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_WLAN_PORTAL_SUBSCRIBE_SERVICE_FORMCTRL_ID,MMIWIFI_WLAN_PORTAL_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, MMIWIFI_WLAN_PORTAL_CHILD_SUBSCRIBE_SERVICE_CTRL_ID, MMIWIFI_WLAN_PORTAL_SUBSCRIBE_SERVICE_FORMCTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_WLAN_PORTAL_MODIFY_PASSWORD_FORMCTRL_ID,MMIWIFI_WLAN_PORTAL_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, MMIWIFI_WLAN_PORTAL_CHILD_MODIFY_PASSWORD_CTRL_ID, MMIWIFI_WLAN_PORTAL_MODIFY_PASSWORD_FORMCTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_WLAN_PORTAL_RESET_PASSWORD_FORMCTRL_ID,MMIWIFI_WLAN_PORTAL_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, MMIWIFI_WLAN_PORTAL_CHILD_RESET_PASSWORD_CTRL_ID, MMIWIFI_WLAN_PORTAL_RESET_PASSWORD_FORMCTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_WLAN_PORTAL_UPDATE_FORMCTRL_ID,MMIWIFI_WLAN_PORTAL_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, MMIWIFI_WLAN_PORTAL_CHILD_UPDATE_CTRL_ID, MMIWIFI_WLAN_PORTAL_UPDATE_FORMCTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_WLAN_PORTAL_HELP_FORMCTRL_ID,MMIWIFI_WLAN_PORTAL_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, MMIWIFI_WLAN_PORTAL_CHILD_HELP_CTRL_ID, MMIWIFI_WLAN_PORTAL_HELP_FORMCTRL_ID),

#endif /* MMI_GUI_STYLE_TYPICAL */
    END_WIN
};

#ifndef MMI_GUI_STYLE_TYPICAL

WINDOW_TABLE(MMIWLAN_PORTAL_ACCOUNT_WIN_TAB) = 
{
    WIN_FUNC((uint32) HandlePortalAccountWinMsg), 
    WIN_ID(MMIWLAN_PORTAL_ACCOUNT_WIN_ID),
    WIN_STATUSBAR,
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY|WS_DISABLE_SOFTKEY_TO_TITLE ),
    WIN_SOFTKEY( STXT_SAVE, TXT_NULL,TXT_NULL),
    WIN_TITLE(TXT_WLAN_PORTAL_ACCOUNT),

    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIWLAN_FORM_ACCOUNT_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWLAN_CHILD_FORM_USER_CTRL_ID,MMIWLAN_FORM_ACCOUNT_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWLAN_LABEL_PORTAL_USER_CTRL_ID, MMIWLAN_CHILD_FORM_USER_CTRL_ID),
    CHILD_EDIT_TEXT_CTRL(TRUE, MMIWLAN_PORTAL_USER_NAME_MAX_LEN,MMIWLAN_EDIT_PORTAL_USER_CTRL_ID, MMIWLAN_CHILD_FORM_USER_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWLAN_CHILD_FORM_PASSWORD_CTRL_ID,MMIWLAN_FORM_ACCOUNT_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWLAN_LABEL_PORTAL_PASSWORD_CTRL_ID, MMIWLAN_CHILD_FORM_PASSWORD_CTRL_ID),
    CHILD_EDIT_PASSWORD_CTRL(TRUE, MMIWLAN_PORTAL_PASSWORD_MAX_LEN, MMIWLAN_EDIT_PORTAL_PASSWORD_CTRL_ID, MMIWLAN_CHILD_FORM_PASSWORD_CTRL_ID),
    END_WIN
};
#endif /* MMI_GUI_STYLE_TYPICAL */

WINDOW_TABLE(MMIWLAN_PASSWORD_MODIFY_WIN_TAB) = 
{
    WIN_FUNC((uint32) HandlePasswordInputWinMsg), 
    WIN_ID(MMIWLAN_PORTAL_PASSWORD_MODIFY_WIN_ID),
    WIN_STATUSBAR,
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif /* MMI_PDA_SUPPORT */

    WIN_TITLE(TXT_COMMON_MODIFY_PASSWORD),

    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),

    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIWIFI_WLAN_PASSWORD_MODIFY_FORM_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_CHILD_FORM_ORIGINAL_PASSWORD_CTRL_ID,MMIWIFI_WLAN_PASSWORD_MODIFY_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_WLAN_PASSWORD_CHILD_OLDPASSOWRD_LABEL_CTRL_ID, MMIWIFI_CHILD_FORM_ORIGINAL_PASSWORD_CTRL_ID),
    CHILD_EDIT_PASSWORD_CTRL(TRUE, MMIWLAN_PORTAL_PASSWORD_MAX_LEN, MMIWIFI_WLAN_PASSWORD_CHILD_OLDPASSOWRD_EDIT_CTRL_ID, MMIWIFI_CHILD_FORM_ORIGINAL_PASSWORD_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_CHILD_FORM_NEW_PASSWORD_CTRL_ID,MMIWIFI_WLAN_PASSWORD_MODIFY_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_WLAN_PASSWORD_CHILD_NEWPASSOWRD_LABEL_CTRL_ID, MMIWIFI_CHILD_FORM_NEW_PASSWORD_CTRL_ID),
    CHILD_EDIT_PASSWORD_CTRL(TRUE, MMIWLAN_PORTAL_PASSWORD_MAX_LEN, MMIWIFI_WLAN_PASSWORD_CHILD_NEWPASSOWRD_EDIT_CTRL_ID, MMIWIFI_CHILD_FORM_NEW_PASSWORD_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_CHILD_FORM_CONFIRM_NEW_PASSWORD_CTRL_ID,MMIWIFI_WLAN_PASSWORD_MODIFY_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_WLAN_PASSWORD_CHILD_NEWPASSOWRD_CONFIRM_LABEL_CTRL_ID, MMIWIFI_CHILD_FORM_CONFIRM_NEW_PASSWORD_CTRL_ID),
    CHILD_EDIT_PASSWORD_CTRL(TRUE, MMIWLAN_PORTAL_PASSWORD_MAX_LEN, MMIWIFI_WLAN_PASSWORD_CHILD_NEWPASSOWRD_CONFIRM_EDIT_CTRL_ID, MMIWIFI_CHILD_FORM_CONFIRM_NEW_PASSWORD_CTRL_ID),

    END_WIN
};

WINDOW_TABLE(MMIWLAN_PORTAL_STATUS_WIN_TAB) = 
{
    WIN_FUNC((uint32) HandlePortalStatusWinMsg), 
    WIN_ID(MMIWLAN_PORTAL_STATUS_WIN_ID),
    WIN_STATUSBAR,
    //WIN_TITLE(TXT_WLAN_PORTAL),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif /* MMI_PDA_SUPPORT */
    WIN_TITLE(TXT_WIFI_DEFAULT_AP_SETTING),
    WIN_SOFTKEY(TXT_WLAN_OFFLINE, TXT_NULL, TXT_HIDE),

    CREATE_RICHTEXT_CTRL(MMIWIFI_WLAN_PORTAL_STATUS_CTRL_ID),

    END_WIN
};

WINDOW_TABLE(MMIWLAN_PORTAL_HELP_WIN_TAB) = 
{
    WIN_FUNC((uint32) HandlePortalHelpWinMsg), 
    WIN_ID(MMIWLAN_PORTAL_HELP_WIN_ID),
    WIN_STATUSBAR,
    WIN_TITLE(TXT_HELP),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),

    CREATE_RICHTEXT_CTRL(MMIWIFI_WLAN_PORTAL_HELP_CTRL_ID),

    END_WIN
};

#ifndef MMI_MULTI_SIM_SYS_SINGLE
/*        //Sam.hua   use SelectSimFunc 
WINDOW_TABLE( MMIWLAN_PORTAL_SEND_SELECT_SIM_WIN_TAB ) = 
{
    WIN_TITLE( TXT_SIM_SEL ),
    WIN_FUNC((uint32) HandlePortalSelectSimWinMsg ),    
    WIN_ID( MMIWLAN_PORTAL_SEND_SELECT_SIM_WIN_ID ),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIWIFI_WLAN_SELECT_SIM_MENU_CTRL_ID),

    END_WIN
};
*/
#endif

WINDOW_TABLE(MMIWLAN_PORTAL_LOGIN_QUERY_WIN_TAB) = 
{
    WIN_FUNC((uint32) HandlePortalLoginQueryWinMsg), 
    WIN_ID(MMIWLAN_PORTAL_LOGIN_QUERY_WIN_ID),
    WIN_STATUSBAR,
    WIN_TITLE(TXT_WLAN_LOGIN_CHAEGE_QUERY),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif /* MMI_PDA_SUPPORT */
    WIN_SOFTKEY(TXT_YES, TXT_NULL, TXT_COMM_NO),

    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIWLAN_PORTAL_LOGIN_FORM_CTRL_ID),
	CHILD_EDIT_TEXT_CTRL(FALSE, MMIWLAN_PORTAL_LOGIN_NOTIFICATION_MAX, MMIWLAN_PORTAL_LOGIN_NOTIFICATION_LABEL_CTRL_ID, MMIWLAN_PORTAL_LOGIN_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, MMIWLAN_PORTAL_LOGIN_NEVER_NOTIFY_LABEL_CTRL_ID, MMIWLAN_PORTAL_LOGIN_FORM_CTRL_ID),

    END_WIN
};

/*****************************************************************************/
//  Description : wlan send sms callback function
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN WlanSMSSendCnfCallback(BOOLEAN is_succss, DPARAM parm)
{//dm 不需要做任何处理，直接结束流程
    BOOLEAN result = TRUE;
    
    return result;
}

/**-------------------------------------------------------------------------*
**                         IMPLEMENTATION                                   *
**--------------------------------------------------------------------------*/
/****************************************************************************/
//  Description : handle portal active service
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/****************************************************************************/
#ifndef MMI_MULTI_SIM_SYS_SINGLE
LOCAL MMI_RESULT_E  PortalSelectSimCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_PORTAL_TYPE_E info_ptr = MMI_PORTAL_SMS_ACTIVE;
    BOOLEAN ret = FALSE;
    MMI_STRING_T sms_int_ptr = {0};
    
    uint8 dest_address[MMIWIFI_PORTAL_SERVICE_PROVIDER_NUM_MAX + 1] = {0};
    char temp_cmd[MMIWIFI_PORTAL_SERVICE_PROVIDER_NUM_MAX + 1] = {0};
    wchar cmd[MMIWIFI_PORTAL_SERVICE_PROVIDER_NUM_MAX + 1] = {0};
    MMI_WIN_ID_T win_id=MMK_GetFocusWinId();

    //changed by minghu for sms port collection
    MMISMS_SEND_DATA_T send_data = {0};

    if (isReturnOK)
    {
        MMIWIFI_GetServiceProvider((char *)dest_address, MMIWIFI_PORTAL_SERVICE_PROVIDER_NUM_MAX);

        info_ptr = (uint32)MMK_GetWinAddDataPtr(win_id);
        if(MMI_PORTAL_SMS_ACTIVE == info_ptr)
        {
            MMIWIFI_GetActiveCMD(temp_cmd, MMIWIFI_PORTAL_COMMAND_LEN_MAX);
            MMIAPICOM_StrToWstr((uint8*)temp_cmd, cmd);
            sms_int_ptr.wstr_ptr = (wchar *)cmd;
            sms_int_ptr.wstr_len = MMIAPICOM_Wstrlen(cmd);
        }
        else if(MMI_PORTAL_SMS_RESET_PASSWORD == info_ptr)
        {
            MMIWIFI_GetResetPasswordCMD(temp_cmd, MMIWIFI_PORTAL_COMMAND_LEN_MAX);
            MMIAPICOM_StrToWstr((uint8*)temp_cmd, cmd);
            sms_int_ptr.wstr_ptr = (wchar *)cmd;
            sms_int_ptr.wstr_len = MMIAPICOM_Wstrlen(cmd);
        }
        else if(MMI_PORTAL_SMS_MODIFY_PASSWORD == info_ptr)
        {     
            sms_int_ptr.wstr_len = MMIAPICOM_Wstrlen(s_wlan_context.sms_content_wchar);               
            sms_int_ptr.wstr_ptr = (wchar *)s_wlan_context.sms_content_wchar;
        }

        send_data.dual_sys = (MN_DUAL_SYS_E)dual_sys;
        send_data.sms_content_ptr = &sms_int_ptr;
        send_data.dst_port = 0;
        send_data.src_port = 0;
        send_data.dest_addr_ptr = dest_address;
        send_data.dest_addr_len = strlen((char *)dest_address);
        send_data.is_need_packet = FALSE;
        send_data.send_callback = WlanSMSSendCnfCallback;

        ret = MMIAPISMS_SendSmsForOther(&send_data);//lint !e64

        if(ret)
        {
            MMIPUB_OpenAlertWinByTextId(PNULL, TXT_WLAN_PORTAL_SMS_REPLY, 0,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        }
        else
        {
            MMIPUB_OpenAlertWarningWin(TXT_COMM_SEND_FAILED);
        }
    }
    return recode;
}
#endif

/****************************************************************************/
//  Description : HandlePasswordInputWinMsg
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  HandlePasswordInputWinMsg(
                                              MMI_WIN_ID_T win_id, 
                                              MMI_MESSAGE_ID_E msg_id, 
                                              DPARAM param
                                              )
 {
   MMI_RESULT_E result = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifndef MMI_GUI_STYLE_TYPICAL
        GUIFORM_SetType(MMIWIFI_WLAN_PASSWORD_MODIFY_FORM_CTRL_ID, GUIFORM_TYPE_TP);
        GUIEDIT_SetAlign(MMIWIFI_WLAN_PASSWORD_CHILD_OLDPASSOWRD_EDIT_CTRL_ID, ALIGN_LVMIDDLE);
        GUIEDIT_SetAlign(MMIWIFI_WLAN_PASSWORD_CHILD_NEWPASSOWRD_EDIT_CTRL_ID, ALIGN_LVMIDDLE);
        GUIEDIT_SetAlign(MMIWIFI_WLAN_PASSWORD_CHILD_NEWPASSOWRD_CONFIRM_EDIT_CTRL_ID, ALIGN_LVMIDDLE);
#endif /* MMI_GUI_STYLE_TYPICAL */
        GUIFORM_SetStyle(MMIWIFI_CHILD_FORM_ORIGINAL_PASSWORD_CTRL_ID,GUIFORM_STYLE_UNIT);
        GUIFORM_SetStyle(MMIWIFI_CHILD_FORM_NEW_PASSWORD_CTRL_ID,GUIFORM_STYLE_UNIT);
        GUIFORM_SetStyle(MMIWIFI_CHILD_FORM_CONFIRM_NEW_PASSWORD_CTRL_ID,GUIFORM_STYLE_UNIT);
        GUIEDIT_SetIm(MMIWIFI_WLAN_PASSWORD_CHILD_OLDPASSOWRD_EDIT_CTRL_ID, GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL,GUIIM_TYPE_ABC);/*lint !e655*/
        GUIEDIT_SetIm(MMIWIFI_WLAN_PASSWORD_CHILD_NEWPASSOWRD_EDIT_CTRL_ID, GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL,GUIIM_TYPE_ABC);/*lint !e655*/
        GUIEDIT_SetIm(MMIWIFI_WLAN_PASSWORD_CHILD_NEWPASSOWRD_CONFIRM_EDIT_CTRL_ID, GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL,GUIIM_TYPE_ABC);/*lint !e655*/
        GUILABEL_SetTextById(MMIWIFI_WLAN_PASSWORD_CHILD_OLDPASSOWRD_LABEL_CTRL_ID, TXT_WIFI_ORIGINAL_PASSWORD, TRUE);
        GUILABEL_SetTextById(MMIWIFI_WLAN_PASSWORD_CHILD_NEWPASSOWRD_LABEL_CTRL_ID, TXT_WIFI_NEW_PASSWORD, TRUE);
        GUILABEL_SetTextById(MMIWIFI_WLAN_PASSWORD_CHILD_NEWPASSOWRD_CONFIRM_LABEL_CTRL_ID, TXT_WIFI_CONFIRM_NEW_PASSWORD, TRUE);

#ifndef MMI_GUI_STYLE_TYPICAL
#if defined(MMI_ISTYLE_SUPPORT)
        if (MMITHEME_IsIstyle())
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
        }
        else
#endif			
#endif /* MMI_GUI_STYLE_TYPICAL */
        {
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
        }

        MMK_SetAtvCtrl(win_id, MMIWIFI_WLAN_PASSWORD_CHILD_OLDPASSOWRD_EDIT_CTRL_ID);
        break;

    case MSG_NOTIFY_EDITBOX_UPDATE_STRNUM:
         { 
            MMI_STRING_T      old_password_ptr = {0};
            MMI_STRING_T      new_password_ptr = {0};
            MMI_STRING_T      new_password_confirm_ptr = {0};

            GUIEDIT_GetString(MMIWIFI_WLAN_PASSWORD_CHILD_OLDPASSOWRD_EDIT_CTRL_ID, &old_password_ptr);
            GUIEDIT_GetString(MMIWIFI_WLAN_PASSWORD_CHILD_NEWPASSOWRD_EDIT_CTRL_ID, &new_password_ptr);
            GUIEDIT_GetString(MMIWIFI_WLAN_PASSWORD_CHILD_NEWPASSOWRD_CONFIRM_EDIT_CTRL_ID, &new_password_confirm_ptr);

            if( (0 == old_password_ptr.wstr_len) || (0 == new_password_ptr.wstr_len) || (0 == new_password_confirm_ptr.wstr_len))
            {
#ifndef MMI_GUI_STYLE_TYPICAL
#if defined(MMI_ISTYLE_SUPPORT)
                if (MMITHEME_IsIstyle())
                {
                    GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, TRUE);
                }
                else
#endif					
#endif /* MMI_GUI_STYLE_TYPICAL */
                {
                    GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, TRUE);
                }
            }
            else
            {
#ifndef MMI_GUI_STYLE_TYPICAL
#if defined(MMI_ISTYLE_SUPPORT)
                if (MMITHEME_IsIstyle())
                {
                    GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, TRUE);
                }
                else
#endif					
#endif /* MMI_GUI_STYLE_TYPICAL */
                {
                    GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_OK, TXT_NULL, STXT_RETURN, TRUE);
                }
            }
         }

    break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#ifndef MMI_GUI_STYLE_TYPICAL
        {
            MMI_CTRL_ID_T ctrl_id = 0;

            if (PNULL != param)
            {
    	        ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;

                if (ctrl_id == MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID)
                {
                    MMK_PostMsg(win_id, MSG_CTL_OK, PNULL, 0);
                }
                else if (ctrl_id == MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID)
                {
                    MMK_CloseWin(win_id);
                }
            }
        }
        break;
#endif /* MMI_GUI_STYLE_TYPICAL */
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_CTL_OK:
    case MSG_APP_OK:  
    case MSG_APP_WEB:
         { 
            MMI_STRING_T      old_password_ptr = {0};
            MMI_STRING_T      new_password_ptr = {0};
            MMI_STRING_T      new_password_confirm_ptr = {0};
            GUIEDIT_GetString(MMIWIFI_WLAN_PASSWORD_CHILD_OLDPASSOWRD_EDIT_CTRL_ID,&old_password_ptr);
            GUIEDIT_GetString(MMIWIFI_WLAN_PASSWORD_CHILD_NEWPASSOWRD_EDIT_CTRL_ID,&new_password_ptr);
            GUIEDIT_GetString(MMIWIFI_WLAN_PASSWORD_CHILD_NEWPASSOWRD_CONFIRM_EDIT_CTRL_ID,&new_password_confirm_ptr);
            if( (0 == old_password_ptr.wstr_len) || (0 == new_password_ptr.wstr_len) || (0 == new_password_confirm_ptr.wstr_len))
            {
                 MMIPUB_OpenAlertWarningWin(TXT_PASSWORD_INVALID);

            }    
            else if(MMIAPICOM_Wstrcmp(new_password_ptr.wstr_ptr,new_password_confirm_ptr.wstr_ptr))
            {
                 MMIPUB_OpenAlertWarningWin(TXT_MSGBOX_PWD_CONFLICT);
            }
            else
            {
                 MMIPUB_OpenQueryWinByTextId(TXT_WLAN_ACCOUNT_OPERATION,IMAGE_PUBWIN_QUERY,PNULL,PNULL);                 
            }
         }
         break;
        
    case MSG_PROMPTWIN_OK:
         {             
         #ifndef MMI_MULTI_SIM_SYS_SINGLE
         MMISET_MAJOR_SIM_SEL_TYPE_E major_select_sim = MMISET_MAJOR_SIM_SEL_SIM1;    
         #endif
         MMI_STRING_T      old_password_ptr = {0};
         MMI_STRING_T      new_password_ptr = {0};
         
         char sms_content[MMIWLAN_PORTAL_SMS_CONTENT_MAX_LEN + 1] = {0};
         char old_password[MMIWLAN_PORTAL_PASSWORD_MAX_LEN + 1] = {0};
         char new_password[MMIWLAN_PORTAL_PASSWORD_MAX_LEN + 1] = {0};
         
         MMI_STRING_T sms_int_ptr = {0};
         BOOLEAN ret = FALSE;
         uint8 dest_address[MMIWIFI_PORTAL_SERVICE_PROVIDER_NUM_MAX + 1] = {0};
         char modify_password[MMIWIFI_PORTAL_COMMAND_LEN_MAX + 1] = {0};

         MMISMS_SEND_DATA_T send_data = {0};

         MMIWIFI_GetServiceProvider((char *)dest_address, MMIWIFI_PORTAL_SERVICE_PROVIDER_NUM_MAX);
         MMIWIFI_GetModifyPasswordCMD((char *)modify_password, MMIWIFI_PORTAL_COMMAND_LEN_MAX);

         GUIEDIT_GetString(MMIWIFI_WLAN_PASSWORD_CHILD_OLDPASSOWRD_EDIT_CTRL_ID,&old_password_ptr);
         GUIEDIT_GetString(MMIWIFI_WLAN_PASSWORD_CHILD_NEWPASSOWRD_EDIT_CTRL_ID,&new_password_ptr);

         if((PNULL == old_password_ptr.wstr_ptr || 0 == old_password_ptr.wstr_len) || (PNULL == new_password_ptr.wstr_ptr || 0 == new_password_ptr.wstr_len) )
         {
             break;
         }
         MMIAPICOM_WstrToStr(old_password_ptr.wstr_ptr,(uint8 *)old_password);
         MMIAPICOM_WstrToStr(new_password_ptr.wstr_ptr,(uint8 *)new_password);
         
         sprintf(sms_content,"%s %s %s",(uint8 *)modify_password,(uint8 *)old_password,(uint8 *)new_password);
         MMIAPICOM_StrToWstr((uint8*)sms_content, s_wlan_context.sms_content_wchar);
         if(0 != MMIAPICOM_Wstrlen(s_wlan_context.sms_content_wchar))
         {
             sms_int_ptr.wstr_ptr = s_wlan_context.sms_content_wchar;
         }
         sms_int_ptr.wstr_len = MMIAPICOM_Wstrlen(s_wlan_context.sms_content_wchar);
         if (MMIAPIPHONE_GetSimAvailableNum(NULL, 0) == 0)
         {
             MMIAPIPHONE_AlertSimNorOKStatus();
             break;
         }
         
         send_data.sms_content_ptr = &sms_int_ptr;
         send_data.dst_port = 0;
         send_data.src_port = 0;
         send_data.dest_addr_ptr = dest_address;
         send_data.dest_addr_len = strlen((char *)dest_address);
         send_data.is_need_packet = FALSE;
         send_data.send_callback = WlanSMSSendCnfCallback;

         #ifndef MMI_MULTI_SIM_SYS_SINGLE
         major_select_sim = MMIAPISET_GetMajorSim(MMISET_MAJOR_SIM_SEL_FUNC_SMS);
         if(major_select_sim < MMISET_MAJOR_SIM_SEL_MANUAL_SEL)
         {
             MN_DUAL_SYS_E         dual_sys = MN_DUAL_SYS_1+((MN_DUAL_SYS_E)major_select_sim-MMISET_MAJOR_SIM_SEL_SIM1);

             send_data.dual_sys = dual_sys;
             ret = MMIAPISMS_SendSmsForOther(&send_data);//lint !e64
         }
         else
         {   
              MMI_PORTAL_TYPE_E portal_sms_param = MMI_PORTAL_SMS_MODIFY_PASSWORD;
              MMIWLAN_SelectSIMFunc(PortalSelectSimCallback,MMISET_APPEND_SYS_OK,(ADD_DATA)portal_sms_param);
              break;
         }
         #else
         send_data.dual_sys = MN_DUAL_SYS_1;
         ret = MMIAPISMS_SendSmsForOther(&send_data);//lint !e64
         #endif
         if(ret)
         {
             MMIPUB_OpenAlertWinByTextId(PNULL, TXT_WLAN_PORTAL_SMS_REPLY, 0,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
         }
         else
         {
             MMIPUB_OpenAlertWarningWin(TXT_COMM_SEND_FAILED);
         }
         MMIPUB_CloseQuerytWin(PNULL);
         MMK_CloseWin(win_id);
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

    return (result);    
 }

/****************************************************************************/
//  Description : handle portal active service
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/****************************************************************************/

LOCAL MMI_RESULT_E  HandlePortalActiveService(
                                              MMI_WIN_ID_T win_id, 
                                              MMI_MESSAGE_ID_E msg_id, 
                                              DPARAM param
                                              )
{


    MMI_RESULT_E result = MMI_RESULT_TRUE;

    switch (msg_id)
    {    
        case MSG_CTL_OK:
        case MSG_APP_OK:  
        case MSG_APP_WEB:
        {             
             #ifndef MMI_MULTI_SIM_SYS_SINGLE
             MMISET_MAJOR_SIM_SEL_TYPE_E major_select_sim = MMISET_MAJOR_SIM_SEL_SIM1;    
             #endif
             

             MMI_STRING_T sms_int_ptr = {0};
             BOOLEAN ret = FALSE;
             uint8 dest_address[MMIWIFI_PORTAL_SERVICE_PROVIDER_NUM_MAX + 1] = {0};
             char temp_active_cmd[MMIWIFI_PORTAL_COMMAND_LEN_MAX + 1] = {0};
             wchar active_cmd[MMIWIFI_PORTAL_COMMAND_LEN_MAX + 1] = {0};

             MMISMS_SEND_DATA_T send_data = {0};

             MMIWIFI_GetServiceProvider((char *)dest_address, MMIWIFI_PORTAL_SERVICE_PROVIDER_NUM_MAX);
             MMIWIFI_GetActiveCMD(temp_active_cmd, MMIWIFI_PORTAL_COMMAND_LEN_MAX);
             MMIAPICOM_StrToWstr((const uint8*)temp_active_cmd, active_cmd);
             sms_int_ptr.wstr_ptr = (wchar *)active_cmd;
             sms_int_ptr.wstr_len = MMIAPICOM_Wstrlen(active_cmd);
             MMIPUB_CloseQuerytWin(PNULL);
             if (MMIAPIPHONE_GetSimAvailableNum(NULL, 0) == 0)
             {
                    MMIAPIPHONE_AlertSimNorOKStatus();
                    break;
             }

             send_data.sms_content_ptr = &sms_int_ptr;
             send_data.dst_port = 0;
             send_data.src_port = 0;
             send_data.dest_addr_ptr = dest_address;
             send_data.dest_addr_len = strlen((char *)dest_address);
             send_data.is_need_packet = FALSE;
             send_data.send_callback = WlanSMSSendCnfCallback;


             #ifndef MMI_MULTI_SIM_SYS_SINGLE
             major_select_sim = MMIAPISET_GetMajorSim(MMISET_MAJOR_SIM_SEL_FUNC_SMS);
             if(major_select_sim < MMISET_MAJOR_SIM_SEL_MANUAL_SEL)
             {
                 MN_DUAL_SYS_E         dual_sys = MN_DUAL_SYS_1+((MN_DUAL_SYS_E)major_select_sim-MMISET_MAJOR_SIM_SEL_SIM1);

                 send_data.dual_sys = dual_sys;
                 ret = MMIAPISMS_SendSmsForOther(&send_data);//lint !e64
             }
             else
             {   
                  MMI_PORTAL_TYPE_E portal_sms_param = MMI_PORTAL_SMS_RESET_PASSWORD;
                  MMIWLAN_SelectSIMFunc(PortalSelectSimCallback,MMISET_APPEND_SYS_OK,(ADD_DATA)portal_sms_param);
                  break;
             }
             #else
             send_data.dual_sys = MN_DUAL_SYS_1;
             ret = MMIAPISMS_SendSmsForOther(&send_data);//lint !e64
             #endif
             if(ret)
             {
                 MMIPUB_OpenAlertWinByTextId(PNULL, TXT_WLAN_PORTAL_SMS_REPLY, 0,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
             }
             else
             {
                 MMIPUB_OpenAlertWarningWin(TXT_COMM_SEND_FAILED);
             }
        }

            //to do send PORTAL short msg
            break;

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMIPUB_CloseQuerytWin(PNULL);
            break;


        default:
            result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
            break;

   }
    return (result);    

}

/****************************************************************************/
//  Description : handle portal modify password
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  HandlePortalResetPassword(
                                              MMI_WIN_ID_T win_id, 
                                              MMI_MESSAGE_ID_E msg_id, 
                                              DPARAM param
                                              )
{
    
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    switch (msg_id)
    {    
        case MSG_CTL_OK:
        case MSG_APP_OK:  
        case MSG_APP_WEB:
        {             
             #ifndef MMI_MULTI_SIM_SYS_SINGLE
             MMISET_MAJOR_SIM_SEL_TYPE_E major_select_sim = MMISET_MAJOR_SIM_SEL_SIM1;    
             #endif
             MMI_STRING_T sms_int_ptr = {0};
             BOOLEAN ret = FALSE;
             uint8 dest_address[MMIWIFI_PORTAL_SERVICE_PROVIDER_NUM_MAX + 1] = {0};
             char temp_cmd[MMIWIFI_PORTAL_COMMAND_LEN_MAX + 1] = {0};
             wchar cmd[MMIWIFI_PORTAL_COMMAND_LEN_MAX + 1] = {0};

             MMISMS_SEND_DATA_T send_data = {0};

             MMIWIFI_GetServiceProvider((char *)dest_address, MMIWIFI_PORTAL_SERVICE_PROVIDER_NUM_MAX);
             MMIWIFI_GetResetPasswordCMD(temp_cmd, MMIWIFI_PORTAL_COMMAND_LEN_MAX);
             MMIAPICOM_StrToWstr((const uint8*)temp_cmd, cmd);             

             sms_int_ptr.wstr_ptr = (wchar *)cmd;
             sms_int_ptr.wstr_len = MMIAPICOM_Wstrlen(cmd);
             MMIPUB_CloseQuerytWin(PNULL);
             if (MMIAPIPHONE_GetSimAvailableNum(NULL, 0) == 0)
             {
                    MMIAPIPHONE_AlertSimNorOKStatus();
                    break;
             }

             send_data.sms_content_ptr = &sms_int_ptr;
             send_data.dst_port = 0;
             send_data.src_port = 0;
             send_data.dest_addr_ptr = dest_address;
             send_data.dest_addr_len = strlen((char *)dest_address);
             send_data.is_need_packet = FALSE;
             send_data.send_callback = WlanSMSSendCnfCallback;

             #ifndef MMI_MULTI_SIM_SYS_SINGLE
             major_select_sim = MMIAPISET_GetMajorSim(MMISET_MAJOR_SIM_SEL_FUNC_SMS);
             if(major_select_sim < MMISET_MAJOR_SIM_SEL_MANUAL_SEL)
             {
                 MN_DUAL_SYS_E         dual_sys = MN_DUAL_SYS_1+((MN_DUAL_SYS_E)major_select_sim-MMISET_MAJOR_SIM_SEL_SIM1);

                 send_data.dual_sys = dual_sys;
                 ret = MMIAPISMS_SendSmsForOther(&send_data);//lint !e64
             }
             else
             {   
                  MMI_PORTAL_TYPE_E portal_sms_param = MMI_PORTAL_SMS_RESET_PASSWORD;
                  MMIWLAN_SelectSIMFunc(PortalSelectSimCallback,MMISET_APPEND_SYS_OK,(ADD_DATA)portal_sms_param);
                  break;
             }
             #else
             send_data.dual_sys = MN_DUAL_SYS_1;
             ret = MMIAPISMS_SendSmsForOther(&send_data);//lint !e64
             #endif
             if(ret)
             {
                 MMIPUB_OpenAlertWinByTextId(PNULL, TXT_WLAN_PORTAL_SMS_REPLY, 0,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
             }
             else
             {
                 MMIPUB_OpenAlertWarningWin(TXT_COMM_SEND_FAILED);
             }
        }
            //to do send PORTAL short msg
        break;

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMIPUB_CloseQuerytWin(PNULL);
            break;


        default:
            result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
            break;

   }
    return (result);    

}


/****************************************************************************/
//  Description : handle portal main window
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E HandlePortalMainWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMIWIFI_PORTAL_CONFIG_T portal_cfg = {0};
    MMI_STRING_T user_name_string = {0};
    MMI_STRING_T password_string = {0};
    uint32 win_type = MMIWLAN_PORTAL_WIN_LOGIN;
#ifdef MMI_GUI_STYLE_TYPICAL
    MMIWIFI_CONTROL_ID_E active_ctrl_id = 0;
    MMI_IMAGE_ID_T image_id = IMAGE_NULL;
#endif /* MMI_GUI_STYLE_TYPICAL */

    win_type = (uint32)MMK_GetWinAddDataPtr(win_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifndef MMI_GUI_STYLE_TYPICAL
        SetPortalMainWinProperty();

        MMK_SetAtvCtrl(win_id, MMIWLAN_LIST_PORTAL_MAIN_CTRL_ID);
#else
        GUIEDIT_SetIm(MMIWIFI_WLAN_PORTAL_CHILD_USER_NAME_EDIT_CTRL_ID, GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL,GUIIM_TYPE_ABC);/*lint !e655*/
        GUIEDIT_SetIm(MMIWIFI_WLAN_PORTAL_CHILD_PASSOWRD_EDIT_CTRL_ID, GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL,GUIIM_TYPE_ABC);/*lint !e655*/

        SetPortalMainWinProperty();

        MMIWIFI_GetPortalConfig(&portal_cfg);

/*
        if (portal_cfg.is_auto_login)
        {
            StartWlanOperation();
        }
*/
        MMK_SetAtvCtrl(win_id, MMIWIFI_WLAN_PORTAL_CHILD_USER_NAME_EDIT_CTRL_ID);
        if (MMIWLAN_PORTAL_WIN_LOGIN == win_type)
        {
            GUIWIN_SetSoftkeyTextId(win_id, TXT_COMM_LOGIN, TXT_NULL, STXT_EXIT, FALSE);
        }
        else
        {
            GUIWIN_SetSoftkeyTextId(win_id, STXT_SAVE, TXT_NULL, STXT_RETURN, FALSE);
        }
#endif /* MMI_GUI_STYLE_TYPICAL */
        break;

    case MSG_NOTIFY_FORM_SWITCH_ACTIVE:
        SavePortalConfigInput();
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
        SavePortalConfigInput();
        MMIWIFI_GetPortalConfig(&portal_cfg);
#ifdef MMI_GUI_STYLE_TYPICAL    
        GUIEDIT_GetString(MMIWIFI_WLAN_PORTAL_CHILD_USER_NAME_EDIT_CTRL_ID, &user_name_string);
        GUIEDIT_GetString(MMIWIFI_WLAN_PORTAL_CHILD_PASSOWRD_EDIT_CTRL_ID, &password_string);

        if (0 == user_name_string.wstr_len || 0 == password_string.wstr_len)
#else
        if (0 == portal_cfg.user_name_len || 0 == portal_cfg.password_len)
#endif /*  MMI_GUI_STYLE_TYPICAL */
        {
            MMIPUB_OpenAlertWarningWin(TXT_WLAN_PORTAL_ALERT_INPUT);         
            break;
        }
        if (MMIWLAN_PORTAL_WIN_LOGIN == win_type)
        {
            MMIPUB_OpenQueryWinByTextId(TXT_WIFI_LOGIN_QUERY, IMAGE_PUBWIN_QUERY, PNULL, HandleCMCCAutoLoginQueryWinMsg);
        }
        else
        {
            if (MMIWIFI_IsCMCCAvailable() && portal_cfg.is_auto_login && portal_cfg.is_notify)
            {
                MMIPUB_OpenQueryWinByTextId(TXT_WIFI_LOGIN_QUERY, IMAGE_PUBWIN_QUERY, PNULL, HandleCMCCAutoLoginQueryWinMsg);
            }
            MMK_CloseWin(win_id);
        }
        break;

    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
#ifndef MMI_GUI_STYLE_TYPICAL
        if (PNULL != param)
        {
            MMI_CTRL_ID_T  ctrl_id = 0;

		    ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;

            if (MSG_CTL_MIDSK == msg_id)
            {
                ctrl_id = MMK_GetActiveCtrlId(win_id);
            }

            if (ctrl_id == MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID)
            {
                MMK_PostMsg(win_id, MSG_CTL_OK, PNULL, 0);
            }
            else if (ctrl_id == MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID)
            {
                MMK_PostMsg(win_id, MSG_CTL_CANCEL, PNULL, 0);
            }
            else if (ctrl_id == MMIWLAN_LIST_PORTAL_MAIN_CTRL_ID)
            {
                uint32 index = 0;
                PORTAL_ITEM_FUNC item_func = PNULL;
                uint32 user_data = 0;

                index = GUILIST_GetCurItemIndex(ctrl_id);
                GUILIST_GetItemData(ctrl_id, index, &user_data);
                item_func = (PORTAL_ITEM_FUNC)user_data;

                if (PNULL != item_func)
                {
                    (*item_func)();
                }
            }
        }
#else
        active_ctrl_id = (MMIWIFI_CONTROL_ID_E)MMK_GetActiveCtrlId(win_id);

        switch(active_ctrl_id)
        {
        case MMIWIFI_WLAN_PORTAL_CHILD_REMEBER_PASSWORD_CTRL_ID:
            MMIWIFI_GetPortalConfig(&portal_cfg);
            portal_cfg.is_remember_password = !portal_cfg.is_remember_password;

            if (!portal_cfg.is_remember_password)//不选择记住密码，无法自动登录
            {
                portal_cfg.is_auto_login = FALSE;
                portal_cfg.is_notify = FALSE;
            }

            MMIWIFI_SetPortalConfig(&portal_cfg);

            //remeber password
            if (portal_cfg.is_remember_password)
            {
                image_id = IMAGE_CHECK_SELECTED_ICON;
            }
            else
            {
                image_id = IMAGE_CHECK_UNSELECTED_ICON;
            }

            GUILABEL_SetIcon(MMIWIFI_WLAN_PORTAL_CHILD_REMEBER_PASSWORD_CTRL_ID, image_id);
            GUILABEL_SetTextById(MMIWIFI_WLAN_PORTAL_CHILD_REMEBER_PASSWORD_CTRL_ID, TXT_WLAN_REMEMBER_PASSWORD, TRUE);

            //auto login
            if (portal_cfg.is_auto_login)
            {
                image_id = IMAGE_CHECK_SELECTED_ICON;
            }
            else
            {
                image_id = IMAGE_CHECK_UNSELECTED_ICON;
            }

            GUILABEL_SetIcon(MMIWIFI_WLAN_PORTAL_CHILD_AUTO_LOGIN_CTRL_ID, image_id);
            GUILABEL_SetTextById(MMIWIFI_WLAN_PORTAL_CHILD_AUTO_LOGIN_CTRL_ID, TXT_WLAN_AUTO_CERTIFICATE, TRUE);

            //Notification
            if (portal_cfg.is_notify)
            {
                image_id = IMAGE_CHECK_SELECTED_ICON;
            }
            else
            {
                image_id = IMAGE_CHECK_UNSELECTED_ICON;
            }

            GUILABEL_SetIcon(MMIWIFI_WLAN_PORTAL_CHILD_NOTIFICATION_CTRL_ID, image_id);
            GUILABEL_SetTextById(MMIWIFI_WLAN_PORTAL_CHILD_NOTIFICATION_CTRL_ID, TXT_WIFI_LOGIN_NOTIFICATION, TRUE);

            break;

        case MMIWIFI_WLAN_PORTAL_CHILD_AUTO_LOGIN_CTRL_ID:
            MMIWIFI_GetPortalConfig(&portal_cfg);
            GUIEDIT_GetString(MMIWIFI_WLAN_PORTAL_CHILD_USER_NAME_EDIT_CTRL_ID, &user_name_string);
            GUIEDIT_GetString(MMIWIFI_WLAN_PORTAL_CHILD_PASSOWRD_EDIT_CTRL_ID, &password_string);

            if (portal_cfg.is_remember_password && 0 != user_name_string.wstr_len && 0 != password_string.wstr_len)//只有选择了记住密码才能选择自动登录
            {
                portal_cfg.is_auto_login = !portal_cfg.is_auto_login;
                if (!portal_cfg.is_auto_login)//不选择记住密码，无法自动登录
                {
                    portal_cfg.is_notify = FALSE;
                }

                MMIWIFI_SetPortalConfig(&portal_cfg);

                //auto login
                if (portal_cfg.is_auto_login)
                {
                    image_id = IMAGE_CHECK_SELECTED_ICON;
                }
                else
                {
                    image_id = IMAGE_CHECK_UNSELECTED_ICON;
                }

                GUILABEL_SetIcon(MMIWIFI_WLAN_PORTAL_CHILD_AUTO_LOGIN_CTRL_ID, image_id);
                GUILABEL_SetTextById(MMIWIFI_WLAN_PORTAL_CHILD_AUTO_LOGIN_CTRL_ID, TXT_WLAN_AUTO_CERTIFICATE, TRUE);

                //Notification
                if (portal_cfg.is_notify)
                {
                    image_id = IMAGE_CHECK_SELECTED_ICON;
                }
                else
                {
                    image_id = IMAGE_CHECK_UNSELECTED_ICON;
                }

                GUILABEL_SetIcon(MMIWIFI_WLAN_PORTAL_CHILD_NOTIFICATION_CTRL_ID, image_id);
                GUILABEL_SetTextById(MMIWIFI_WLAN_PORTAL_CHILD_NOTIFICATION_CTRL_ID, TXT_WIFI_LOGIN_NOTIFICATION, TRUE);
            }
            break;

        case MMIWIFI_WLAN_PORTAL_CHILD_ACTIVE_SERVICE_CTRL_ID:

            
            //MMIWIFI_OpenPortalHelpWin(TXT_WIFI_ACTIVE_SERVICE_HELP, TXT_WIFI_ACTIVE_SERVICE);

            MMIPUB_OpenQueryWinByTextId(TXT_WLAN_ACCOUNT_OPERATION,IMAGE_PUBWIN_QUERY,PNULL,HandlePortalActiveService);          
            break;


        case MMIWIFI_WLAN_PORTAL_CHILD_SUBSCRIBE_SERVICE_CTRL_ID:
            MMIWIFI_OpenPortalHelpWin(TXT_WIFI_SUBSCRIBE_SERVICE_HELP, TXT_WIFI_SUBSCRIBE);
            break;

        case MMIWIFI_WLAN_PORTAL_CHILD_MODIFY_PASSWORD_CTRL_ID:

            MMK_CreateWin((uint32 *)MMIWLAN_PASSWORD_MODIFY_WIN_TAB,PNULL);
           // MMIPUB_OpenQueryWinByTextId(TXT_WIFI_MODIFY_PASSWORD_HELP,IMAGE_PUBWIN_QUERY,PNULL,HandlePortalResetPassword);  
            break;

       case MMIWIFI_WLAN_PORTAL_CHILD_RESET_PASSWORD_CTRL_ID:
            MMIPUB_OpenQueryWinByTextId(TXT_WLAN_ACCOUNT_OPERATION,IMAGE_PUBWIN_QUERY,PNULL,HandlePortalResetPassword);
            break;

       case MMIWIFI_WLAN_PORTAL_CHILD_UPDATE_CTRL_ID:
            MMIWIFI_Update();
            break;

        case MMIWIFI_WLAN_PORTAL_CHILD_NOTIFICATION_CTRL_ID:
            MMIWIFI_GetPortalConfig(&portal_cfg);

            if (portal_cfg.is_auto_login)
            {
                portal_cfg.is_notify = !portal_cfg.is_notify;

                MMIWIFI_SetPortalConfig(&portal_cfg);

                //Notification
                if (portal_cfg.is_notify)
                {
                    image_id = IMAGE_CHECK_SELECTED_ICON;
                }
                else
                {
                    image_id = IMAGE_CHECK_UNSELECTED_ICON;
                }

                GUILABEL_SetIcon(MMIWIFI_WLAN_PORTAL_CHILD_NOTIFICATION_CTRL_ID, image_id);
                GUILABEL_SetTextById(MMIWIFI_WLAN_PORTAL_CHILD_NOTIFICATION_CTRL_ID, TXT_WIFI_LOGIN_NOTIFICATION, TRUE);
            }
            break;

        case MMIWIFI_WLAN_PORTAL_CHILD_HELP_CTRL_ID:
            MMIWIFI_OpenPortalHelpWin(TXT_WLAN_PORTAL_HELP, TXT_HELP);
            break;

        default:
            break;
        }
#endif /* MMI_GUI_STYLE_TYPICAL */
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        s_wlan_context.portal_status = MMIWLAN_PORTAL_STATUS_DISCONNECTED;
        MMK_CloseWin(win_id);
        break;

   case MSG_CLOSE_WINDOW:
#ifdef MMI_GUI_STYLE_TYPICAL
        SavePortalConfigInput();
        MMIWIFI_GetPortalConfig(&portal_cfg);
        if (0 == portal_cfg.password_len || 0 == portal_cfg.user_name_len)
        {
            portal_cfg.is_remember_password = FALSE;
        }

        if (!portal_cfg.is_remember_password)
        {
            SCI_MEMSET(portal_cfg.password, 0x00, sizeof(portal_cfg.password));

            portal_cfg.password_len = 0;
            portal_cfg.is_auto_login = FALSE;

            MMIWIFI_SetPortalConfig(&portal_cfg);
        }

        MMIWIFI_SetPortalConfig(&portal_cfg);
#endif /* MMI_GUI_STYLE_TYPICAL */
        if (MMIWLAN_PORTAL_WIN_LOGIN == win_type)
        {
            if (MMIWLAN_PORTAL_STATUS_PORTALED != s_wlan_context.portal_status)
            {
                s_wlan_context.portal_status = MMIWLAN_PORTAL_STATUS_DISCONNECTED;

                MMIAPIWIFI_DeRegNotifyInfo(MMI_MODULE_WIFI);

                MMIWIFI_WlanPortalNetDisconnect();
            }
        }
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/****************************************************************************/
//  Description : Save Portal Config Input
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/****************************************************************************/
LOCAL void SavePortalConfigInput(void)
{
#ifdef MMI_GUI_STYLE_TYPICAL
    MMIWIFI_PORTAL_CONFIG_T portal_cfg = {0};
    MMI_STRING_T user_name_string = {0};
    MMI_STRING_T password_string = {0};

    GUIEDIT_GetString(MMIWIFI_WLAN_PORTAL_CHILD_USER_NAME_EDIT_CTRL_ID, &user_name_string);
    GUIEDIT_GetString(MMIWIFI_WLAN_PORTAL_CHILD_PASSOWRD_EDIT_CTRL_ID, &password_string);

    MMIWIFI_GetPortalConfig(&portal_cfg);
 
    SCI_MEMSET(portal_cfg.user_name, 0x00, sizeof(portal_cfg.user_name));
    portal_cfg.user_name_len = user_name_string.wstr_len;

    MMI_WSTRNCPY(portal_cfg.user_name, MMIWLAN_PORTAL_USER_NAME_MAX_LEN, user_name_string.wstr_ptr, user_name_string.wstr_len, portal_cfg.user_name_len);

    SCI_MEMSET(portal_cfg.password, 0x00, sizeof(portal_cfg.password));
    portal_cfg.password_len = password_string.wstr_len;

    MMI_WSTRNCPY(portal_cfg.password, MMIWLAN_PORTAL_USER_NAME_MAX_LEN, password_string.wstr_ptr, password_string.wstr_len, portal_cfg.password_len);

    MMIWIFI_SetPortalConfig(&portal_cfg);
#endif /* MMI_GUI_STYLE_TYPICAL */
}

/****************************************************************************/
//  Description : Set Portal Main Win Property
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/****************************************************************************/
LOCAL void SetPortalMainWinProperty(void)
{
    MMIWIFI_PORTAL_CONFIG_T portal_cfg = {0};
    MMI_IMAGE_ID_T image_id = IMAGE_NULL;
#ifndef MMI_GUI_STYLE_TYPICAL
    uint16 current_index = 0;
    uint16 top_index = 0;

    MMIWIFI_GetPortalConfig(&portal_cfg);

    current_index = GUILIST_GetCurItemIndex(MMIWLAN_LIST_PORTAL_MAIN_CTRL_ID);
    top_index = GUILIST_GetTopItemIndex(MMIWLAN_LIST_PORTAL_MAIN_CTRL_ID);
    GUILIST_RemoveAllItems(MMIWLAN_LIST_PORTAL_MAIN_CTRL_ID);
    GUILIST_SetMaxItem(MMIWLAN_LIST_PORTAL_MAIN_CTRL_ID, 10, FALSE);

    /* Active Service */
    AppendOneTextOneImageItem(MMIWLAN_LIST_PORTAL_MAIN_CTRL_ID, TXT_WIFI_ACTIVE_SERVICE, IMAGE_COMMON_RIGHT_ARROW, (uint32)WifiPortalhandleActiveService);

    /* Account */
    AppendOneTextOneImageItem(MMIWLAN_LIST_PORTAL_MAIN_CTRL_ID, TXT_WLAN_PORTAL_ACCOUNT, IMAGE_COMMON_RIGHT_ARROW, (uint32)WifiPortalhandleAccount);

    /*Subscribe Service */
    AppendOneTextOneImageItem(MMIWLAN_LIST_PORTAL_MAIN_CTRL_ID, TXT_WIFI_SUBSCRIBE, IMAGE_COMMON_RIGHT_ARROW, (uint32)WifiPortalhandleSubscibeService);

    /* Modify PWD */
    AppendOneTextOneImageItem(MMIWLAN_LIST_PORTAL_MAIN_CTRL_ID, TXT_COMMON_MODIFY_PASSWORD, IMAGE_COMMON_RIGHT_ARROW, (uint32)WifiPortalhandleModifyPassword);

    /* Reset PWD */
    AppendOneTextOneImageItem(MMIWLAN_LIST_PORTAL_MAIN_CTRL_ID, TXT_WLAN_RESET_PASSWORD, IMAGE_COMMON_RIGHT_ARROW, (uint32)WifiPortalhandleResetPassword);

    /* Notify when login */
    image_id = portal_cfg.is_notify ? IMAGE_CHECK_SELECTED_ICON : IMAGE_CHECK_UNSELECTED_ICON;
    AppendOneTextOneImageItem(MMIWLAN_LIST_PORTAL_MAIN_CTRL_ID, TXT_WIFI_LOGIN_NOTIFICATION, image_id, (uint32)WifiPortalhandleNotify);

    /* Update */
    AppendOneTextOneImageItem(MMIWLAN_LIST_PORTAL_MAIN_CTRL_ID, TXT_WLAN_UPDATE, IMAGE_COMMON_RIGHT_ARROW, (uint32)WifiPortalhandleUpdate);

    /* Help */
    AppendOneTextOneImageItem(MMIWLAN_LIST_PORTAL_MAIN_CTRL_ID, TXT_HELP, IMAGE_COMMON_RIGHT_ARROW, (uint32)WifiPortalhandleHelp);

    GUILIST_SetCurItemIndex(MMIWLAN_LIST_PORTAL_MAIN_CTRL_ID, current_index);
    GUILIST_SetTopItemIndex(MMIWLAN_LIST_PORTAL_MAIN_CTRL_ID, top_index);

#else
    GUIFORM_SetType(MMIWIFI_WLAN_PORTAL_FORM_CTRL_ID, GUIFORM_TYPE_TP);
    GUIEDIT_SetAlign(MMIWIFI_WLAN_PORTAL_CHILD_USER_NAME_EDIT_CTRL_ID, ALIGN_LVMIDDLE);
    GUIEDIT_SetAlign(MMIWIFI_WLAN_PORTAL_CHILD_PASSOWRD_EDIT_CTRL_ID, ALIGN_LVMIDDLE);

    MMIWIFI_GetPortalConfig(&portal_cfg);

    //user name
    GUILABEL_SetTextById(MMIWIFI_WLAN_PORTAL_CHILD_USER_NAME_LABEL_CTRL_ID, TXT_COMMON_USERNAME, FALSE);

    GUIEDIT_SetString(MMIWIFI_WLAN_PORTAL_CHILD_USER_NAME_EDIT_CTRL_ID, portal_cfg.user_name, portal_cfg.user_name_len);

    //password
    GUILABEL_SetTextById(MMIWIFI_WLAN_PORTAL_CHILD_PASSOWRD_LABEL_CTRL_ID, TXT_COMMON_PASSWORD, FALSE);

    GUIEDIT_SetString(MMIWIFI_WLAN_PORTAL_CHILD_PASSOWRD_EDIT_CTRL_ID, portal_cfg.password, portal_cfg.password_len);

    //remember password
    if (portal_cfg.is_remember_password)
    {
        image_id = IMAGE_CHECK_SELECTED_ICON;
    }
    else
    {
        image_id = IMAGE_CHECK_UNSELECTED_ICON;
    }

    GUILABEL_SetIcon(MMIWIFI_WLAN_PORTAL_CHILD_REMEBER_PASSWORD_CTRL_ID, image_id);
    GUILABEL_SetTextById(MMIWIFI_WLAN_PORTAL_CHILD_REMEBER_PASSWORD_CTRL_ID, TXT_WLAN_REMEMBER_PASSWORD, FALSE);

    //auto login
    if (portal_cfg.is_auto_login)
    {
        image_id = IMAGE_CHECK_SELECTED_ICON;
    }
    else
    {
        image_id = IMAGE_CHECK_UNSELECTED_ICON;
    }

    GUILABEL_SetIcon(MMIWIFI_WLAN_PORTAL_CHILD_AUTO_LOGIN_CTRL_ID, image_id);
    GUILABEL_SetTextById(MMIWIFI_WLAN_PORTAL_CHILD_AUTO_LOGIN_CTRL_ID, TXT_WLAN_AUTO_CERTIFICATE, FALSE);

    if (portal_cfg.is_notify)
    {
        image_id = IMAGE_CHECK_SELECTED_ICON;
    }
    else
    {
        image_id = IMAGE_CHECK_UNSELECTED_ICON;
    }

    GUILABEL_SetIcon(MMIWIFI_WLAN_PORTAL_CHILD_NOTIFICATION_CTRL_ID, image_id);
    GUILABEL_SetTextById(MMIWIFI_WLAN_PORTAL_CHILD_NOTIFICATION_CTRL_ID, TXT_WIFI_LOGIN_NOTIFICATION, FALSE);

    GUILABEL_SetTextById(MMIWIFI_WLAN_PORTAL_CHILD_ACTIVE_SERVICE_CTRL_ID, TXT_WIFI_ACTIVE_SERVICE, FALSE);
    GUILABEL_SetTextById(MMIWIFI_WLAN_PORTAL_CHILD_SUBSCRIBE_SERVICE_CTRL_ID, TXT_WIFI_SUBSCRIBE, FALSE);
    GUILABEL_SetTextById(MMIWIFI_WLAN_PORTAL_CHILD_MODIFY_PASSWORD_CTRL_ID, TXT_COMMON_MODIFY_PASSWORD, FALSE);
    GUILABEL_SetTextById(MMIWIFI_WLAN_PORTAL_CHILD_RESET_PASSWORD_CTRL_ID, TXT_WLAN_RESET_PASSWORD, FALSE);
    GUILABEL_SetTextById(MMIWIFI_WLAN_PORTAL_CHILD_UPDATE_CTRL_ID, TXT_WLAN_UPDATE, FALSE);
    GUILABEL_SetTextById(MMIWIFI_WLAN_PORTAL_CHILD_HELP_CTRL_ID, TXT_HELP, FALSE);
#endif /* MMI_GUI_STYLE_TYPICAL */

    GUIFORM_SetStyle(MMIWIFI_WLAN_PORTAL_USER_NAME_FORMCTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIWIFI_WLAN_PORTAL_PASSWORD_FORMCTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIWIFI_WLAN_PORTAL_REMEMBER_PASSWORD_FORMCTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIWIFI_WLAN_PORTAL_AUTO_LOGIN_FORMCTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIWIFI_WLAN_PORTAL_ACTIVE_SERVICE_FORMCTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIWIFI_WLAN_PORTAL_SUBSCRIBE_SERVICE_FORMCTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIWIFI_WLAN_PORTAL_NOTIFICATION_FORMCTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIWIFI_WLAN_PORTAL_MODIFY_PASSWORD_FORMCTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIWIFI_WLAN_PORTAL_UPDATE_FORMCTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIWIFI_WLAN_PORTAL_RESET_PASSWORD_FORMCTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIWIFI_WLAN_PORTAL_HELP_FORMCTRL_ID,GUIFORM_STYLE_UNIT);
}

/****************************************************************************/
//  Description : handle portal status window
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E HandlePortalStatusWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_WIN_ID_T potral_query_id = MMIWLAN_PORTAL_STATUS_DISCONNECT_QUERY_WIN_ID;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MMIWIFI_WLAN_PORTAL_STATUS_CTRL_ID);
        break;

    case MSG_FULL_PAINT:
        SetPortalStatusWinProperty(MMIWIFI_WLAN_PORTAL_STATUS_CTRL_ID);
        break;

#ifndef MMI_GUI_STYLE_TYPICAL
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
            MMI_CTRL_ID_T ctrl_id = 0;

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
#endif /* MMI_GUI_STYLE_TYPICAL */

    case MSG_PROMPTWIN_OK:
        if (MMIWIFI_PortalOffline())
        {
            MMI_STRING_T wait_string = {0};

            MMI_GetLabelTextByLang(TXT_WLAN_PORTAL_LOGOUTING,&wait_string);

            MMIPUB_OpenWaitWin(1,&wait_string,PNULL,PNULL,MMIWLAN_PORTAL_WAIT_WIN_ID,IMAGE_NULL,
                                                    ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandleOfflineWaitWinMsg);

            MMK_SetWinDisplayStyleState(MMIWLAN_PORTAL_WAIT_WIN_ID, WS_DISABLE_RETURN_WIN, TRUE);

        }

        MMK_CloseWin(potral_query_id);

        if (PNULL != s_wlan_context.status_timer_id)
        {
            MMK_StopTimer(s_wlan_context.status_timer_id);
            s_wlan_context.status_timer_id = 0;
        }

        s_wlan_context.portal_status = MMIWLAN_PORTAL_STATUS_LOGOUTING;
        break;

    case MSG_PROMPTWIN_CANCEL:
        MMK_CloseWin(potral_query_id);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef MMI_GUI_STYLE_TYPICAL
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527    
#endif /* MMI_GUI_STYLE_TYPICAL */
        MMIPUB_OpenQueryTextWinByTextId(TXT_WLAN_PORTAL_DISCONNECT_QUERY, &potral_query_id, PNULL);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

   case MSG_CLOSE_WINDOW:
        if (MMIWLAN_PORTAL_STATUS_PORTALED != s_wlan_context.portal_status)
        {
            MMIAPIWIFI_DeRegNotifyInfo(MMI_MODULE_WIFI);
        }
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/****************************************************************************/
//  Description : Set Portal Status Win Property
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/****************************************************************************/
LOCAL void SetPortalStatusWinProperty(MMI_CTRL_ID_T ctrl_id)
{
    GUIRICHTEXT_ITEM_T item_data = {0};
    uint8 portal_time_str[MMIWLAN_PORTAL_TIME_STR_LEN_MAX + 1] = {0};
    wchar portal_time_wstr[MMIWLAN_PORTAL_TIME_STR_LEN_MAX + 1] = {0};
    uint16 portal_time_str_len = 0;
    uint16 index = 0;
    //uint32 portal_days = 0;
    uint32 portal_hours = 0;
    uint32 portal_minutes = 0;
    uint32 portal_seconds = 0;
    uint32 portal_time = 0;
    uint32 rem = 0;

    portal_time = s_wlan_context.status_time / 2;

    GUIRICHTEXT_DeleteAllItems(ctrl_id);

    //status
    item_data.text_type = GUIRICHTEXT_TEXT_RES;
    item_data.text_data.res.id = TXT_WLAN_PORTAL_STATUS;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

    item_data.text_type = GUIRICHTEXT_TEXT_RES;
    item_data.text_data.res.id = TXT_WLAN_PORTAL_STATUS_CONNECTED;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

    item_data.text_type = GUIRICHTEXT_TEXT_RES;
    item_data.text_data.res.id = TXT_WLAN_PORTAL_INLINE_TIME;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

    //time
    portal_seconds = portal_time%60;
    rem = portal_time/60;
    portal_minutes = rem%60;
    rem /= 60;
    portal_hours = rem;
    //portal_hours = rem%24;
    //portal_days = rem/24;

    //if (1 < portal_days)
    //{
    //    sprintf((char *)portal_time_str, "%02dDays %02d:%02d:%02d", portal_days, portal_hours, portal_minutes, portal_seconds);
    //}
    //else if (1 == portal_days)
    //{
    //    sprintf((char *)portal_time_str, "%02dDay %02d:%02d:%02d", portal_days, portal_hours, portal_minutes, portal_seconds);
    //}
    //else
    //{
        sprintf((char *)portal_time_str, "%02d:%02d:%02d", portal_hours, portal_minutes, portal_seconds);
    //}

    portal_time_str_len = strlen((char *)portal_time_str);

    MMI_STRNTOWSTR(portal_time_wstr, MMIWLAN_PORTAL_TIME_STR_LEN_MAX, portal_time_str, MMIWLAN_PORTAL_TIME_STR_LEN_MAX, portal_time_str_len);

    item_data.text_type = GUIRICHTEXT_TEXT_BUF;
    item_data.text_data.buf.str_ptr = (wchar *)portal_time_wstr;
    item_data.text_data.buf.len = portal_time_str_len;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

    //description
    item_data.text_type = GUIRICHTEXT_TEXT_RES;
    item_data.text_data.res.id = TXT_WLAN_PORTAL_INLINE_DESCRIPTION;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
}

/*****************************************************************************/
//  Description : handle Portal timer
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
LOCAL void HandleWlanPortalTimer(uint8 timer_id, uint32 param )
{
    //SCI_TRACE_LOW:"HandleWlanPortalTimer timer_id=%d, s_wlan_context.status_timer_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_1645_112_2_18_3_9_41_99,(uint8*)"dd", timer_id, s_wlan_context.status_timer_id);

    if (s_wlan_context.status_timer_id == timer_id)
    {
        s_wlan_context.status_time ++;

        if (MMK_IsFocusWin(MMIWLAN_PORTAL_STATUS_WIN_ID))
        {
            MMK_SendMsg(MMIWLAN_PORTAL_STATUS_WIN_ID, MSG_FULL_PAINT, PNULL);
        }
    }
}

/****************************************************************************/
//  Description : handle portal help window
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E HandlePortalHelpWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MMIWIFI_WLAN_PORTAL_HELP_CTRL_ID);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
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
//  Description : Set Portal Help Win Property
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/****************************************************************************/
LOCAL void SetPortalHelpWinProperty(MMI_CTRL_ID_T ctrl_id, MMI_TEXT_ID_T text_id)
{
    GUIRICHTEXT_ITEM_T item_data = {0};
    uint16 index = 0;

    GUIRICHTEXT_DeleteAllItems(ctrl_id);

    //Help
    item_data.text_type = GUIRICHTEXT_TEXT_RES;
    item_data.text_data.res.id = text_id;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
}

/*****************************************************************************/
//     Description : Callback function to Wlan msg
//    Global resource dependence : 
//  Author:wancan.you
//    Note: 
/*****************************************************************************/ 
LOCAL void WlanMsgPortalCallback(MMIWIFI_NOTIFY_EVENT_E event_id, MMIWIFI_NOTIFY_PARAM_T* param_ptr)
{
    //SCI_TRACE_LOW:"WlanMsgPortalCallback event_id=%d,result=%d,s_wlan_context.portal_status=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_1715_112_2_18_3_9_41_100,(uint8*)"ddd", event_id, param_ptr->result, s_wlan_context.portal_status);

    switch (event_id)
    {
    case MMIWIFI_NOTIFY_EVENT_ON:
        if (MMIWLAN_PORTAL_STATUS_OPENING == s_wlan_context.portal_status)
        {
            if (WIFISUPP_RESULT_SUCC == param_ptr->result)
            {
                s_wlan_context.portal_status = MMIWLAN_PORTAL_STATUS_OPENED;

                ImplementWlanOperation();
            }
            else
            {
                MMIPUB_OpenAlertWarningWin( TXT_COMM_OPEN_FAIL);
                MMK_CloseWin(MMIWLAN_PORTAL_WAIT_WIN_ID);
            }
        }
        break;

    case MMIWIFI_NOTIFY_EVENT_OFF:
        if (MMIWLAN_PORTAL_STATUS_DISCONNECTED != s_wlan_context.portal_status)
        {
            if (WIFISUPP_RESULT_SUCC == param_ptr->result)
            {
                s_wlan_context.portal_status = MMIWLAN_PORTAL_STATUS_DISCONNECTED;

                MMIWIFI_WlanPortalRequestClose();

                MMIWIFI_WlanPortalNetDisconnect();

                if (0 != s_wlan_context.status_timer_id)
                {
                    MMK_StopTimer(s_wlan_context.status_timer_id);
                    s_wlan_context.status_timer_id = 0;
                }

                MMK_CloseWin(MMIWLAN_PORTAL_WAIT_WIN_ID);
            }
            else
            {
                MMK_CloseWin(MMIWLAN_PORTAL_WAIT_WIN_ID);
            }
        }
        break;

    case MMIWIFI_NOTIFY_EVENT_CONNECT:
        if (MMIWLAN_PORTAL_STATUS_CONNECTING == s_wlan_context.portal_status)
        {
            if (WIFISUPP_RESULT_SUCC == param_ptr->result
                && MMIWIFI_IsCMCCAvailable())
            {
                s_wlan_context.portal_status = MMIWLAN_PORTAL_STATUS_CONNECTED;

                ImplementWlanOperation();
            }
            else
            {
                MMIPUB_OpenAlertWarningWin( TXT_WLAN_PORTAL_FIND_CMCC_CONNECT_FAIL);
                MMK_CloseWin(MMIWLAN_PORTAL_WAIT_WIN_ID);
            }
        }
        break;

    case MMIWIFI_NOTIFY_EVENT_DISCONNECT:
        if (WIFISUPP_RESULT_SUCC == param_ptr->result)
        {
            if (MMK_IsOpenWin(MMIWLAN_PORTAL_WAIT_WIN_ID) 
                || MMK_IsOpenWin(MMIWLAN_PORTAL_STATUS_WIN_ID)
                || MMIWLAN_PORTAL_STATUS_PORTALED == s_wlan_context.portal_status)
            {
                MMIWIFI_WlanPortalRequestClose();

                MMIWIFI_WlanPortalNetDisconnect();

                MMIWIFI_NotifyPortalFailed(TXT_COMMON_LINK_DISCONNECTED);
            }
        }
        else
        {
            MMK_CloseWin(MMIWLAN_PORTAL_WAIT_WIN_ID);
        }
        break;

    case MMIWIFI_NOTIFY_EVENT_SCAN_CNF:
        if (MMIWLAN_PORTAL_STATUS_SCANING == s_wlan_context.portal_status)
        {
            if (WIFISUPP_RESULT_SUCC == param_ptr->result
                && PNULL != MMIWIFI_GetCMCCProfile())
            {
                s_wlan_context.portal_status = MMIWLAN_PORTAL_STATUS_SCANED;

                ImplementWlanOperation();
            }
            else
            {
                MMIPUB_OpenAlertWarningWin( TXT_WLAN_PORTAL_FIND_CMCC_AP_FAIL);
                MMK_CloseWin(MMIWLAN_PORTAL_WAIT_WIN_ID);
            }
        }
        break;

    default:
        break;
    }
}

/*****************************************************************************/
//     Description : Start Wlan Operation
//    Global resource dependence : 
//  Author:wancan.you
//    Note: 
/*****************************************************************************/ 
LOCAL void StartWlanOperation(void)
{
    if (MMIAPIWIFI_GetIsWlanOn())
    {
        s_wlan_context.portal_status = MMIWLAN_PORTAL_STATUS_OPENED;

        if (MMIWIFI_IsCMCCAvailable())
        {
            s_wlan_context.portal_status = MMIWLAN_PORTAL_STATUS_CONNECTED;
        }
        else if (PNULL != MMIWIFI_GetCMCCProfile())
        {
            s_wlan_context.portal_status = MMIWLAN_PORTAL_STATUS_SCANED;
        }
    }
    else
    {
        s_wlan_context.portal_status = MMIWLAN_PORTAL_STATUS_DISCONNECTED;
    }

    ImplementWlanOperation();
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePortalingWaitWinMsg(
                                              MMI_WIN_ID_T win_id, 
                                              MMI_MESSAGE_ID_E msg_id, 
                                              DPARAM param
                                              )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    
    switch(msg_id) 
    {
    case MSG_OPEN_WINDOW:
        MMIPUB_SetWinSoftkey(win_id, TXT_NULL, STXT_CANCEL, FALSE);
        break;

    case MSG_APP_OK:
    case MSG_APP_WEB:
        break;

    case MSG_APP_RED:
    case MSG_APP_FLIP:
        MMIWIFI_WlanPortalRequestCancel();

        recode= FALSE;
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMIWIFI_WlanPortalRequestCancel();

        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMIPUB_HandleWaitWinMsg( win_id, msg_id, param );
        break;    
        
    }
    return recode;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleOfflineWaitWinMsg(
                                              MMI_WIN_ID_T win_id, 
                                              MMI_MESSAGE_ID_E msg_id, 
                                              DPARAM param
                                              )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    
    switch(msg_id) 
    {
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_APP_RED:
    case MSG_APP_FLIP:
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        break;
        
    default:
        recode = MMIPUB_HandleWaitWinMsg( win_id, msg_id, param );
        break;    
        
    }
    return recode;
}

/*****************************************************************************/
//     Description : Implement Wlan Operation
//    Global resource dependence : 
//  Author:wancan.you
//    Note: 
/*****************************************************************************/ 
LOCAL void ImplementWlanOperation(void)
{
    MMIWIFI_NOTIFY_PARAM_T notify = {0};

    //SCI_TRACE_LOW:"ImplementWlanOperation s_wlan_context.portal_status=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_1932_112_2_18_3_9_41_101,(uint8*)"d", s_wlan_context.portal_status);

    switch (s_wlan_context.portal_status)
    {
    case MMIWLAN_PORTAL_STATUS_DISCONNECTED:
        if (MMIAPIWIFI_On(TRUE))
        {
            s_wlan_context.portal_status = MMIWLAN_PORTAL_STATUS_OPENING;
        }
        else
        {
            notify.result = WIFISUPP_RESULT_FAIL;
            WlanMsgPortalCallback(MMIWIFI_NOTIFY_EVENT_ON, &notify);
        }
        break;

    case MMIWLAN_PORTAL_STATUS_OPENED:
        if (MMIWIFI_Scan(PNULL))
        {
            s_wlan_context.portal_status = MMIWLAN_PORTAL_STATUS_SCANING;
        }
        else
        {
            notify.result = WIFISUPP_RESULT_FAIL;
            WlanMsgPortalCallback(MMIWIFI_NOTIFY_EVENT_FIND_AP, &notify);
        }
        break;

    case MMIWLAN_PORTAL_STATUS_SCANED:
        if (MMIAPIWIFI_Connect(MMIWIFI_GetCMCCProfile()))
        {
            s_wlan_context.portal_status = MMIWLAN_PORTAL_STATUS_CONNECTING;
        }
        else
        {
            notify.result = WIFISUPP_RESULT_FAIL;
            WlanMsgPortalCallback(MMIWIFI_NOTIFY_EVENT_CONNECT, &notify);
        }
        break;

    case MMIWLAN_PORTAL_STATUS_CONNECTED:
        {
            MMI_STRING_T wait_string = {0};

            MMI_GetLabelTextByLang(TXT_WLAN_PORTAL_LOGINING,&wait_string);

            MMIPUB_OpenWaitWin(1,&wait_string,PNULL,PNULL,MMIWLAN_PORTAL_WAIT_WIN_ID,IMAGE_NULL,
                                                    ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_CUSTOMER,HandlePortalingWaitWinMsg);

            //MMK_SetWinDisplayStyleState(MMIWLAN_PORTAL_WAIT_WIN_ID, WS_DISABLE_RETURN_WIN, TRUE);

            if (MMIWIFI_WlanPortalNetConnect())
            {
                s_wlan_context.portal_status = MMIWLAN_PORTAL_STATUS_PORTALING;
            }
            else
            {
                s_wlan_context.portal_status = MMIWLAN_PORTAL_STATUS_DISCONNECTED;
                MMIPUB_OpenAlertWarningWin( TXT_DIAL_FAILED);
            }
        }
        break;

    default:
        break;
    }
}

/*****************************************************************************/
//     Description :
//    Global resource dependence : 
//  Author:Jiaoyou.wu
//    Note: 
/*****************************************************************************/ 
LOCAL MMI_RESULT_E HandleCMCCAutoLoginQueryWinMsg(MMI_WIN_ID_T      win_id, 
                                          MMI_MESSAGE_ID_E  msg_id, 
                                          DPARAM            param)
{
    MMI_RESULT_E          result = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"HandleCMCCAutoLoginQueryWinMsg() msg_id=0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_2010_112_2_18_3_9_41_102,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        MMK_CloseWin(win_id);
        StartWlanOperation();
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

/****************************************************************************/
//  Description : Open Portal Login Win
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_OpenPortalLoginWin(void)
{
    WIFI_OpenPortalWin(MMIWLAN_PORTAL_WIN_LOGIN);
}

/****************************************************************************/
//  Description : Open Portal Status Win
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
PUBLIC void MMIAPIWIFI_OpenPortalStatusWin(void)
{
    WIFI_OpenPortalWin(MMIWLAN_PORTAL_WIN_STATUS);
}

/****************************************************************************/
//  Description : Open Portal Settings win
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_OpenPortalSettingsWin(void)
{
    WIFI_OpenPortalWin(MMIWLAN_PORTAL_WIN_SETTINGS);
}

/****************************************************************************/
//  Description : Open Portal Win
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/****************************************************************************/
LOCAL void WIFI_OpenPortalWin(MMIWLAN_PORTAL_WIN_E win_type)
{
    MMIWIFI_NOTIFY_INFO_T notify_info = {0};

    //SCI_TRACE_LOW:"WIFI_OpenPortalWin win_type= %d, s_wlan_context.portal_status=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_2068_112_2_18_3_9_41_103,(uint8*)"dd", win_type, s_wlan_context.portal_status);

    switch (win_type)
    {
    case MMIWLAN_PORTAL_WIN_STATUS:
        MMIWIFI_OpenPortalStatusWin();
        break;

    case MMIWLAN_PORTAL_WIN_LOGIN:
        if (MMIWLAN_PORTAL_STATUS_PORTALED == s_wlan_context.portal_status)
        {
            MMIWIFI_OpenPortalStatusWin();
        }
        else
        {
            notify_info.module_id = MMI_MODULE_WIFI;
            notify_info.notify_func = WlanMsgPortalCallback;
            MMIAPIWIFI_RegNotifyInfo(&notify_info);
            MMK_CreateWin((uint32 *)MMIWLAN_PORTAL_MAIN_WIN_TAB, (ADD_DATA)win_type);
        }
        break;

    case MMIWLAN_PORTAL_WIN_SETTINGS:
        MMK_CreateWin((uint32 *)MMIWLAN_PORTAL_MAIN_WIN_TAB, (ADD_DATA)win_type);
        break;

    default:
        break;
    }
}

/****************************************************************************/
//  Description : Auto Login CMCC
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_CMCCAutoLogin(void)
{
    MMIWIFI_PORTAL_CONFIG_T portal_cfg = {0};

    MMIWIFI_GetPortalConfig(&portal_cfg);

    //SCI_TRACE_LOW:"MMIWIFI_CMCCAutoLogin login=%d, notify=%d, s_wlan_context.portal_status=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_2109_112_2_18_3_9_41_104,(uint8*)"ddd", portal_cfg.is_auto_login, portal_cfg.is_notify, s_wlan_context.portal_status);

    if (!portal_cfg.is_auto_login ||
        !portal_cfg.is_remember_password ||
        (0 == portal_cfg.password_len || 0 == portal_cfg.user_name_len) ||
        MMIWLAN_PORTAL_STATUS_DISCONNECTED != s_wlan_context.portal_status)
    {
        return;
    }

    if(portal_cfg.is_notify)
    {
        MMK_CreateWin((uint32 *)MMIWLAN_PORTAL_LOGIN_QUERY_WIN_TAB, PNULL);
        return;
    }

    StartWlanOperation();
}

/****************************************************************************/
//  Description : Log off
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_CMCCLogoff(void)
{
    //SCI_TRACE_LOW:"MMIWIFI_CMCCLogoff status = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_2134_112_2_18_3_9_41_105,(uint8*)"d", s_wlan_context.portal_status);

    if (MMIWLAN_PORTAL_STATUS_PORTALED == s_wlan_context.portal_status)
    {
        MMIWIFI_PortalOffline();

        if (PNULL != s_wlan_context.status_timer_id)
        {
            MMK_StopTimer(s_wlan_context.status_timer_id);
            s_wlan_context.status_timer_id = 0;
        }

        s_wlan_context.portal_status = MMIWLAN_PORTAL_STATUS_DISCONNECTED;
    }
}

/****************************************************************************/
//  Description : Start Portal Status Timer
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_StartPortalStatusTimer(void)
{
    if (0 != s_wlan_context.status_timer_id)
    {
        MMK_StopTimer(s_wlan_context.status_timer_id);
    }

    s_wlan_context.status_time = 0;
    s_wlan_context.status_timer_id = MMK_CreateTimerCallback(MMIWLAN_PORTAL_STATUS_TIMEOUT, HandleWlanPortalTimer, NULL, TRUE);

    MMK_CloseWin(MMIWLAN_PORTAL_MAIN_WIN_ID);
}

/****************************************************************************/
//  Description : Open Portal Status Win
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_OpenPortalStatusWin(void)
{
    MMK_CreateWin((uint32 *)MMIWLAN_PORTAL_STATUS_WIN_TAB, PNULL);
}

/****************************************************************************/
//  Description : Open Portal Help Win
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_OpenPortalHelpWin(MMI_TEXT_ID_T text_id, MMI_TEXT_ID_T title_id)
{
    MMK_CreateWin((uint32 *)MMIWLAN_PORTAL_HELP_WIN_TAB, PNULL);

    SetPortalHelpWinProperty(MMIWIFI_WLAN_PORTAL_HELP_CTRL_ID, text_id);

    GUIWIN_SetTitleTextId(MMIWLAN_PORTAL_HELP_WIN_ID, title_id, FALSE);
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_Update(void)
{
    SFS_HANDLE file_handle = PNULL;
    char *file_buf = PNULL;
    uint32 file_size = 0;
    uint32 read_size = 0;
    MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;
    int32 ret = MMIWIFI_PORTAL_UPDATE_OK;

    file_handle = SFS_CreateFile(s_update_file, SFS_MODE_READ, PNULL, PNULL);

    do
    {
        file_handle = SFS_CreateFile(s_update_file, SFS_MODE_OPEN_EXISTING | SFS_MODE_READ, PNULL, PNULL);

        if (PNULL == file_handle)
        {
            //SCI_TRACE_LOW:"MMIWIFI_Update File_handle is NULL"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_2210_112_2_18_3_9_42_106,(uint8*)"");
            ret = MMIWIFI_PORTAL_UPDATE_ERROR_FILE;
            break;
        }

        file_size = MMIAPIFMM_GetFileSize(file_handle);

        if (file_size == 0 || file_size >= MMIWLAN_PORTAL_UPDATE_FILE_SIZE_MAX)
        {
            //SCI_TRACE_LOW:"MMIWIFI_Update File size Incorrect(%d)"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_2219_112_2_18_3_9_42_107,(uint8*)"d", file_size);
            ret = MMIWIFI_PORTAL_UPDATE_ERROR_FILE;
            break;
        }

        file_buf = SCI_ALLOCA(file_size);
        if (PNULL == file_buf)
        {
            ret = MMIWIFI_PORTAL_UPDATE_ERROR_FILE;
            //SCI_TRACE_LOW:"MMIWIFI_Update No mem"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_2228_112_2_18_3_9_42_108,(uint8*)"");
            break;
        }

        file_ret = MMIAPIFMM_ReadFile(file_handle, file_buf, file_size, &read_size, PNULL);
        if (SFS_NO_ERROR != file_ret)
        {
            ret = MMIWIFI_PORTAL_UPDATE_ERROR_FILE;
            //SCI_TRACE_LOW:"MMIWIFI_Update Read File error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_2236_112_2_18_3_9_42_109,(uint8*)"");
            break;
        }

        ret = wifi_ParseUpdateFile(file_buf);

    } while (0);
    
    if (PNULL != file_handle)
    {
        MMIAPIFMM_CloseFile(file_handle);
        file_handle = PNULL;
    }

    if (PNULL != file_buf)
    {
        SCI_FREE(file_buf);
    }

    switch (ret)
    {
    case MMIWIFI_PORTAL_UPDATE_OK:
        MMIPUB_OpenAlertSuccessWin( TXT_WLAN_UPDATE_SUCCESS);
        break;

    case MMIWIFI_PORTAL_UPDATE_ERROR_FILE:
        MMIPUB_OpenAlertWarningWin( TXT_WLAN_UPDATE_FAIL);
        break;

    case MMIWIFI_PORTAL_UPDATE_ERROR_UPDATED:
        MMIPUB_OpenAlertWarningWin( TXT_WLAN_UPDATE_UPDATED);
        break;
    default:
        break;
    }
}

/****************************************************************************/
//  Description : Handle Portal Login
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_HandlePortalLogin(MMIWIFI_PORTAL_LOGIN_RES_E login_res)
{
    const MMI_TEXT_ID_T login_res_text[MMIWIFI_PORTAL_LOGIN_MAX] = 
                                                                                                                {
                                                                                                                    TXT_WLAN_PORTAL_SUCCESS,
                                                                                                                    TXT_WLAN_PORTAL_USER_ERROR,
                                                                                                                    TXT_WLAN_PORTAL_FAILED_OTHER,
                                                                                                                    TXT_WLAN_PORTAL_PWD_ERROR,
                                                                                                                    TXT_WLAN_PORTAL_FAILED_OTHER,
                                                                                                                    TXT_WLAN_PORTAL_FAILED_OTHER,
                                                                                                                    TXT_WLAN_PORTAL_FAILED_OTHER,
                                                                                                                    TXT_WLAN_PORTAL_FAILED_OTHER,
                                                                                                                    TXT_WLAN_PORTAL_FAILED_OTHER,
                                                                                                                    TXT_WLAN_PORTAL_FAILED_OTHER,
                                                                                                                    TXT_WLAN_PORTAL_FAILED_OTHER
                                                                                                                };
    MMI_IMAGE_ID_T image_id = IMAGE_PUBWIN_WARNING;

    //SCI_TRACE_LOW:"MMIWIFI_HandlePortalLogin login_res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_2295_112_2_18_3_9_42_110,(uint8*)"d", login_res);

    if (MMIWIFI_PORTAL_LOGIN_MAX != login_res)
    {
        if (MMIWIFI_PORTAL_LOGIN_SUCCESS == login_res)
        {
            s_wlan_context.portal_status = MMIWLAN_PORTAL_STATUS_PORTALED;

            image_id = IMAGE_PUBWIN_SUCCESS;

            MMIWIFI_StartPortalStatusTimer();
            WIFI_OpenPortalWin(MMIWLAN_PORTAL_WIN_STATUS);
        }
        else
        {
            image_id = IMAGE_PUBWIN_WARNING;

            s_wlan_context.portal_status = MMIWLAN_PORTAL_STATUS_PORTAL_FAILED;
        }

        MMIDEFAULT_TurnOnBackLight();

        MMK_CloseWin(MMIWLAN_PORTAL_WAIT_WIN_ID);

        MMIPUB_OpenAlertWinByTextId(PNULL, login_res_text[login_res],TXT_NULL,image_id,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
    }
}

/****************************************************************************/
//  Description : Handle Portal Offline
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_HandlePortalOffline(MMIWIFI_PORTAL_OFFLINE_RES_E offline_res)
{
    MMI_IMAGE_ID_T image_id = IMAGE_PUBWIN_WARNING;

    const MMI_TEXT_ID_T offline_res_text[MMIWIFI_PORTAL_OFFLINE_MAX] = 
                                                                                                                {
                                                                                                                    TXT_WLAN_PORTAL_OFFLINE_SUCCESS,
                                                                                                                    TXT_WLAN_PORTAL_FAILED_OTHER,
                                                                                                                    TXT_WLAN_PORTAL_FAILED_OTHER,
                                                                                                                    TXT_WLAN_PORTAL_FAILED_OTHER,
                                                                                                                    TXT_WLAN_PORTAL_FAILED_OTHER,
                                                                                                                    TXT_WLAN_PORTAL_FAILED_OTHER
                                                                                                                };

    //SCI_TRACE_LOW:"MMIWIFI_HandlePortalOffline offline_res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_2341_112_2_18_3_9_42_111,(uint8*)"d", offline_res);

    if (MMIWIFI_PORTAL_OFFLINE_MAX != offline_res)
    {
        if (MMIWIFI_PORTAL_OFFLINE_SUCCESS == offline_res)
        {
            image_id = IMAGE_PUBWIN_SUCCESS;
        }
        else
        {
            image_id = IMAGE_PUBWIN_WARNING;
        }

        s_wlan_context.portal_status = MMIWLAN_PORTAL_STATUS_DISCONNECTED;

        MMK_CloseWin(MMIWLAN_PORTAL_WAIT_WIN_ID);
        MMK_CloseWin(MMIWLAN_PORTAL_STATUS_WIN_ID);

        MMIDEFAULT_TurnOnBackLight();

        MMIWIFI_WlanPortalNetDisconnect();

        MMIPUB_OpenAlertWinByTextId(PNULL, offline_res_text[offline_res],TXT_NULL,image_id,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
    }
}

/****************************************************************************/
//  Description : get portal status
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/****************************************************************************/
PUBLIC MMIWLAN_PORTAL_STATUS_E MMIWIFI_GetPortalStatus(void)
{
    return s_wlan_context.portal_status;
}

/****************************************************************************/
//  Description : Notify Portal Failed
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_NotifyPortalFailed(MMI_TEXT_ID_T text_id)
{
    if (0 != s_wlan_context.status_timer_id)
    {
        MMK_StopTimer(s_wlan_context.status_timer_id);
        s_wlan_context.status_timer_id = 0;
    }

    s_wlan_context.portal_status = MMIWLAN_PORTAL_STATUS_DISCONNECTED;

    MMK_CloseWin(MMIWLAN_PORTAL_STATUS_WIN_ID);
    MMK_CloseWin(MMIWLAN_PORTAL_WAIT_WIN_ID);

    MMIPUB_OpenAlertWarningWin( text_id);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePortalLoginQueryWinMsg(
                                           MMI_WIN_ID_T	win_id, 
                                           MMI_MESSAGE_ID_E	msg_id, 
                                           DPARAM param
                                         )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    static BOOLEAN is_check = FALSE;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            MMI_HANDLE_T ctrl_handle = PNULL;
            MMI_STRING_T notify_str = {0};

            ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIWLAN_PORTAL_LOGIN_NOTIFICATION_LABEL_CTRL_ID);
            MMIRES_GetText(TXT_WIFI_LOGIN_QUERY, win_id, &notify_str);
            GUIEDIT_SetString(ctrl_handle, notify_str.wstr_ptr, notify_str.wstr_len);

            is_check = TRUE;
            ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIWLAN_PORTAL_LOGIN_NEVER_NOTIFY_LABEL_CTRL_ID);
            GUILABEL_SetIcon(MMIWLAN_PORTAL_LOGIN_NEVER_NOTIFY_LABEL_CTRL_ID, IMAGE_CHECK_SELECTED_ICON);
            GUILABEL_SetTextById(MMIWLAN_PORTAL_LOGIN_NEVER_NOTIFY_LABEL_CTRL_ID, TXT_WLAN_LOGIN_NEVER_NOTIFY, FALSE);
            MMK_SetAtvCtrl(win_id, MMIWLAN_PORTAL_LOGIN_NEVER_NOTIFY_LABEL_CTRL_ID);
        }
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
        {
            MMIWIFI_PORTAL_CONFIG_T portal_cfg = {0};
 
            MMIWIFI_GetPortalConfig(&portal_cfg);
            portal_cfg.is_notify = (BOOLEAN)!is_check;
            MMIWIFI_SetPortalConfig(&portal_cfg);

            StartWlanOperation();
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
#ifndef MMI_GUI_STYLE_TYPICAL
            MMI_CTRL_ID_T       id = 0;

            if (PNULL != param)
            {
		        id = ((MMI_NOTIFY_T*) param)->src_id;

                if (id == MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID)
                {
                    MMK_PostMsg(win_id, MSG_CTL_OK, PNULL, 0);
                    break;
                }
                else if (id == MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID)
                {
                    MMK_PostMsg(win_id, MSG_CTL_CANCEL, PNULL, 0);
                    break;
                }            
            }
#endif /* MMI_GUI_STYLE_TYPICAL */

            ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIWLAN_PORTAL_LOGIN_NEVER_NOTIFY_LABEL_CTRL_ID);

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
            GUILABEL_SetTextById(ctrl_handle, TXT_WLAN_LOGIN_NEVER_NOTIFY, TRUE);
        }
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:        
        result = MMI_RESULT_FALSE;
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
LOCAL int32 wifi_ParseUpdateFile(char *file_ptr)
{
    char sp[MMIWIFI_PORTAL_SERVICE_PROVIDER_NUM_MAX + 1] = {0};
    char act_cmd[MMIWIFI_PORTAL_COMMAND_LEN_MAX + 1] = {0};
    char mod_cmd[MMIWIFI_PORTAL_COMMAND_LEN_MAX + 1] = {0};
    char res_cmd[MMIWIFI_PORTAL_COMMAND_LEN_MAX + 1] = {0};
    char my_form[MMIWIFI_PORTAL_LOGIN_FORM_LEN_MAX + 1] = {0};
    char login_form[MMIWIFI_PORTAL_LOGIN_FORM_LEN_MAX + 1] = {0};
#if 0
    char old_ver[MMIWIFI_PORTAL_VERSION_LEN_MAX + 1] = {0};
#endif
    char ver[MMIWIFI_PORTAL_VERSION_LEN_MAX + 1] = {0};
    BOOLEAN ret = TRUE;

    ret = ret && wifi_ParseXML(file_ptr, MMIWIFI_TAG_VERSION, ver, MMIWIFI_PORTAL_SERVICE_PROVIDER_NUM_MAX);

#if 0
    if (ret)
    {
        MMIWIFI_GetPortalVersion(old_ver, MMIWIFI_PORTAL_VERSION_LEN_MAX);
        if (strcmp(old_ver, ver) > 0)
        {
            return MMIWIFI_PORTAL_UPDATE_ERROR_UPDATED;
        }
    }
#endif

    ret = ret && wifi_ParseXML(file_ptr, MMIWIFI_TAG_SERVICE_PROVIDER, sp, MMIWIFI_PORTAL_SERVICE_PROVIDER_NUM_MAX);
    ret = ret && wifi_ParseXML(file_ptr, MMIWIFI_TAG_ACTIVE, act_cmd, MMIWIFI_PORTAL_COMMAND_LEN_MAX);
    ret = ret && wifi_ParseXML(file_ptr, MMIWIFI_TAG_MODITY_PASSWORD, mod_cmd, MMIWIFI_PORTAL_COMMAND_LEN_MAX);
    ret = ret && wifi_ParseXML(file_ptr, MMIWIFI_TAG_RESET_PASSWORD, res_cmd, MMIWIFI_PORTAL_COMMAND_LEN_MAX);
    ret = ret && wifi_ParseXML(file_ptr, MMIWIFI_TAG_PORTAL_MYFORM, my_form, MMIWIFI_PORTAL_COMMAND_LEN_MAX);
    ret = ret && wifi_ParseXML(file_ptr, MMIWIFI_TAG_LOGINFORM, login_form, MMIWIFI_PORTAL_COMMAND_LEN_MAX);

    if (ret)
    {
        MMIWIFI_SetServiceProvider(sp);
        MMIWIFI_SetActiveCMD(act_cmd);
        MMIWIFI_SetModifyPasswordCMD(mod_cmd);
        MMIWIFI_SetResetPasswordCMD(res_cmd);
        MMIWIFI_SetPortalVersion(ver);
        MMIWIFI_SetLoginFormTag(login_form);
        MMIWIFI_SetMyFormTag(my_form);

        return MMIWIFI_PORTAL_UPDATE_OK;
    }
    else
    {
        return MMIWIFI_PORTAL_UPDATE_ERROR_FILE;
    }


}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL BOOLEAN wifi_ParseXML(char *xml_file_ptr, char *tag, char *value, int32 max_len)
{
    char *tag_begin = PNULL;
    char *tag_end = PNULL;
    int32 tag_len = 0;
    int32 len = 0;

    if (PNULL == xml_file_ptr || PNULL == tag)
    {
        //SCI_TRACE_LOW:"wifi_ParseXML Bad param"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_2569_112_2_18_3_9_42_112,(uint8*)"");
        return FALSE;
    }

    tag_len = strlen(tag);
    
    /* Only support <tag>value</tag> */
    if (PNULL != (tag_begin = strstr(xml_file_ptr, tag)))
    {
        if (PNULL != (tag_end = strstr(tag_begin + tag_len, tag)))
        {
            len = tag_end - tag_begin - tag_len;
            if (len > 3)
            {
                len -= 3;/* exlude "></" */
                len = MIN(len, max_len);
                SCI_MEMSET(value, 0, max_len);
                SCI_MEMCPY(value, tag_begin + tag_len + 1, len);
                return TRUE;
            }
        }
    }

    return FALSE;
}

#ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL void AppendOneTextOneImageItem(MMI_CTRL_ID_T ctrl_id, MMI_TEXT_ID_T text_1, MMI_IMAGE_ID_T image_id, uint32 user_data)
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};

    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_ICON;
    item_t.item_data_ptr = &item_data;
    item_t.user_data     = user_data;

    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_1;

    //anim
    item_data.item_content[1].item_data_type = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[1].item_data.text_id = image_id;
    
    GUILIST_AppendItem(ctrl_id, &item_t);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL void  WifiPortalhandleAccount(void)
{
    MMK_CreateWin((uint32 *)MMIWLAN_PORTAL_ACCOUNT_WIN_TAB, PNULL);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL void  WifiPortalhandleNotify(void)
{
    MMIWIFI_PORTAL_CONFIG_T portal_cfg = {0};

    MMIWIFI_GetPortalConfig(&portal_cfg);

    portal_cfg.is_notify = !portal_cfg.is_notify;

    MMIWIFI_SetPortalConfig(&portal_cfg);

    SetPortalMainWinProperty();

    MMK_PostMsg(MMIWLAN_PORTAL_MAIN_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL void  WifiPortalhandleActiveService(void)
{
    MMIPUB_OpenQueryWinByTextId(TXT_WLAN_ACCOUNT_OPERATION, IMAGE_PUBWIN_QUERY, PNULL, HandlePortalActiveService);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL void  WifiPortalhandleUpdate(void)
{
    MMIWIFI_Update();
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL void  WifiPortalhandleHelp(void)
{
    MMIWIFI_OpenPortalHelpWin(TXT_WLAN_PORTAL_HELP, TXT_HELP);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL void  WifiPortalhandleResetPassword(void)
{
    MMIPUB_OpenQueryWinByTextId(TXT_WLAN_ACCOUNT_OPERATION, IMAGE_PUBWIN_QUERY, PNULL, HandlePortalResetPassword);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL void  WifiPortalhandleModifyPassword(void)
{
    MMK_CreateWin((uint32 *)MMIWLAN_PASSWORD_MODIFY_WIN_TAB, PNULL);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL void  WifiPortalhandleSubscibeService(void)
{
    MMIWIFI_OpenPortalHelpWin(TXT_WIFI_SUBSCRIBE_SERVICE_HELP, TXT_WIFI_SUBSCRIBE);
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E HandlePortalAccountWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
   MMI_RESULT_E result = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            MMIWIFI_PORTAL_CONFIG_T portal_cfg = {0};

            MMIWIFI_GetPortalConfig(&portal_cfg);

            GUIFORM_SetType(MMIWLAN_FORM_ACCOUNT_CTRL_ID, GUIFORM_TYPE_TP);
            GUIEDIT_SetAlign(MMIWLAN_EDIT_PORTAL_USER_CTRL_ID, ALIGN_LVMIDDLE);
            GUIEDIT_SetAlign(MMIWLAN_EDIT_PORTAL_PASSWORD_CTRL_ID, ALIGN_LVMIDDLE);
            GUIFORM_SetStyle(MMIWLAN_CHILD_FORM_USER_CTRL_ID, GUIFORM_STYLE_UNIT);
            GUIFORM_SetStyle(MMIWLAN_CHILD_FORM_PASSWORD_CTRL_ID, GUIFORM_STYLE_UNIT);

            GUILABEL_SetTextById(MMIWLAN_LABEL_PORTAL_USER_CTRL_ID, TXT_COMMON_USERNAME, FALSE);

            GUILABEL_SetTextById(MMIWLAN_LABEL_PORTAL_PASSWORD_CTRL_ID, TXT_COMMON_PASSWORD, FALSE);

            GUIEDIT_SetIm(MMIWLAN_EDIT_PORTAL_USER_CTRL_ID, GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL,GUIIM_TYPE_ABC);/*lint !e655*/
            GUIEDIT_SetString(MMIWLAN_EDIT_PORTAL_USER_CTRL_ID, portal_cfg.user_name, portal_cfg.user_name_len);

            GUIEDIT_SetPasswordStyle(MMIWLAN_EDIT_PORTAL_PASSWORD_CTRL_ID, GUIEDIT_PASSWORD_STYLE_ALL);
            GUIEDIT_SetString(MMIWLAN_EDIT_PORTAL_PASSWORD_CTRL_ID, portal_cfg.password, portal_cfg.password_len);
            GUIEDIT_SetIm(MMIWLAN_EDIT_PORTAL_PASSWORD_CTRL_ID, GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL,GUIIM_TYPE_ABC);/*lint !e655*/
            MMK_SetAtvCtrl(win_id, MMIWLAN_EDIT_PORTAL_USER_CTRL_ID);
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
            MMI_CTRL_ID_T ctrl_id = 0;

            if (PNULL != param)
            {
    	        ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;

                if (ctrl_id == MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID || ctrl_id == MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID)
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

    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_CTL_MIDSK:
        {
            MMI_STRING_T user = {0};
            MMI_STRING_T password = {0};

            GUIEDIT_GetString(MMIWLAN_EDIT_PORTAL_USER_CTRL_ID, &user);
            GUIEDIT_GetString(MMIWLAN_EDIT_PORTAL_PASSWORD_CTRL_ID, &password);

            if (0 == user.wstr_len || 0 == password.wstr_len)
            {
                MMIPUB_OpenAlertWarningWin(TXT_WLAN_PORTAL_ALERT_INPUT);         
                break;
            }
            else
            {
                MMIWIFI_PORTAL_CONFIG_T portal_cfg = {0};

                MMIWIFI_GetPortalConfig(&portal_cfg);

                SCI_MEMSET(portal_cfg.user_name, 0x00, sizeof(portal_cfg.user_name));
                portal_cfg.user_name_len = user.wstr_len;
                MMI_WSTRNCPY(portal_cfg.user_name, MMIWLAN_PORTAL_USER_NAME_MAX_LEN, user.wstr_ptr, user.wstr_len, portal_cfg.user_name_len);

                SCI_MEMSET(portal_cfg.password, 0x00, sizeof(portal_cfg.password));
                portal_cfg.password_len = password.wstr_len;
                MMI_WSTRNCPY(portal_cfg.password, MMIWLAN_PORTAL_USER_NAME_MAX_LEN, password.wstr_ptr, password.wstr_len, portal_cfg.password_len);

                MMIWIFI_SetPortalConfig(&portal_cfg);
                MMK_CloseWin(win_id);
            }
        }
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        result = MMI_RESULT_FALSE;
     break;

    }

    return (result);
}

#endif /* MMI_GUI_STYLE_TYPICAL */


/****************************************************************************/
//  Description : Select SIM API for WLAN app
//  Global resource dependence : 
//  Author:sam.hua
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E MMIWLAN_SelectSIMFunc(MMIAPIPHONE_SELECTSIM_FUNC MMIWLANSelectSIMWinCallback,
                                        MMISET_APPEND_SIM_TYPE_E append_type,
                                        ADD_DATA    add_data
                                        )
{
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
    MMI_WIN_ID_T  selectSimWinID = MMIWLAN_PORTAL_COM_SELECT_SIM_WIN_ID;
    MMIPHONE_SELECT_SIM_DATA_T  caller_data={0};

    caller_data.append_type = (uint32)append_type;
    caller_data.callback_func =MMIWLANSelectSIMWinCallback ;
    caller_data.select_type= MMIPHONE_SELECTION_SIM_ONLY;

    caller_data.publist_id = MMIWIFI_WLAN_COM_SELECT_SIM_MENU_CTRL_ID;
    caller_data.win_id_ptr = &selectSimWinID;
    caller_data.user_data  = add_data;
    caller_data.extra_data = PNULL;
    
    MMIAPIPHONE_OpenSelectSimWin(&caller_data);

#endif
    return MMI_RESULT_TRUE;
}



/*Edit by script, ignore 7 case. Fri Apr 27 09:38:56 2012*/ //IGNORE9527
