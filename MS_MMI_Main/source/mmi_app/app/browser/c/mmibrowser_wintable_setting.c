/*****************************************************************************
** File Name:      mmibrowser_wintable_setting.c                             *
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
//#include "sci_types.h"
#include "mmibrowser_wintable.h" 
#include "browser_text.h"
#include "mmibrowser_id.h"
#include "guidropdownlist.h"
#include "mmiconnection_export.h"
#include "mmibrowser_setting.h"
#include "mmi_appmsg.h"
#include "mmipub.h"//MS00187371
#ifdef MMI_GUI_STYLE_TYPICAL
#include "guiform.h"
#include "guilabel.h"
#endif
#include "mmibrowser_func.h"
#include "mmiconnection_text.h"
#include "mmisms_text.h"
#include "mmi_resource.h"

#ifndef MMI_GUI_STYLE_TYPICAL
#include "mmibrowser_nv.h"
#endif /* MMI_GUI_STYLE_TYPICAL */
#include "mmi_text.h"
#include "brw_control_api.h"
#ifdef MMI_SSL_CERT_SUPPORT
#include "ssl_api.h"
#include "guirichtext.h"
#endif
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

#define MMIBROWSER_SETTING_IMAGE_NUM 2
#define MMIBROWSER_SETTING_GOOGLE_NUM 2
#define MMIBROWSER_SETTING_LIST_ITEM    20

typedef void (*SETTING_ITEM_FUNC)(void);

typedef enum
{
    MMIBROWSER_HOMEPAGE_TYPE_DEFAULT = 1,
    MMIBROWSER_HOMEPAGE_TYPE_CUSTOMIZE,

    MMIBROWSER_HOMEPAGE_TYPE_MAX
}MMIBROWSER_HOMEPAGE_TYPE_E;

typedef enum
{
    MMIBROWSER_FONT_TYPE_SMALL = 1,
    MMIBROWSER_FONT_TYPE_NORMAL,
    MMIBROWSER_FONT_TYPE_BIG,

    MMIBROWSER_FONT_TYPE_MAX
}MMIBROWSER_FONT_TYPE_E;

#ifndef MMI_GUI_STYLE_TYPICAL
LOCAL BOOLEAN   s_is_has_reset = FALSE;
#endif /* MMI_GUI_STYLE_TYPICAL */

#ifdef MMI_SSL_CERT_SUPPORT
SSL_CERTINFO_T *s_certInfo_ptr = PNULL;
#endif
 /**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
#ifdef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description : handle bookmark window message
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleSettingWinMsg(
                                   MMI_WIN_ID_T	win_id, 
                                   MMI_MESSAGE_ID_E	msg_id, 
                                   DPARAM		param
                                   );

 /*****************************************************************************/
// 	Description : handle message of input homepage window of browser
//	Global resource dependence : none
//  Author: fen.xie
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleInputHomepageWinMsg(
                                             MMI_WIN_ID_T	    win_id, 
                                             MMI_MESSAGE_ID_E	msg_id, 
                                             DPARAM				param
                                             );
								   
/*****************************************************************************/
//  Description : set setting window
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void BrwSetSettingWindow(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : set sim network account label text
//  Global resource dependence : 
//  Author: jixin.xu
//  Note: 
/*****************************************************************************/
LOCAL void BrwUpdateSimNetAccountLabelText(MMI_WIN_ID_T win_id);
#endif /* MMI_GUI_STYLE_TYPICAL */

/*****************************************************************************/
//  Description : execute the setting to profile and update web page for new settings
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN BrwExecuteSetting(BOOLEAN is_update_font);

#ifdef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description : set browser setting to nv
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL void BrwSetSetting(BOOLEAN *is_update_font_ptr);
#endif /* MMI_GUI_STYLE_TYPICAL */

/*****************************************************************************/
//  Description : set sim's network accout index setting for wap setting
//  Global resource dependence : s_nv_setting
//  Author: jixin.xu
//  Note: 
/*****************************************************************************/
LOCAL void BrwSetNetIndexSettingCallback(MN_DUAL_SYS_E dual_sys, uint32 index);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyouw.wu
//  Note: 
/*****************************************************************************/
LOCAL void BrwHandleSettingAccount1(void);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyouw.wu
//  Note: 
/*****************************************************************************/
LOCAL void BrwHandleSettingAccount2(void);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyouw.wu
//  Note: 
/*****************************************************************************/
LOCAL void BrwHandleSettingAccount3(void);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyouw.wu
//  Note: 
/*****************************************************************************/
LOCAL void BrwHandleSettingAccount4(void);

#ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description : handle Setting list msg
//  Global resource dependence : 
//  Author: Jiaoyouw.wu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleSettingListWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : handle Setting list msg
//  Global resource dependence : 
//  Author: Jiaoyouw.wu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleSettingHomepageTypeWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : handle Setting list msg
//  Global resource dependence : 
//  Author: Jiaoyouw.wu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleSettingHomepageWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : handle Setting list msg
//  Global resource dependence : 
//  Author: Jiaoyouw.wu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleSettingFontWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : handle Setting list msg
//  Global resource dependence : 
//  Author: Jiaoyouw.wu
//  Note: 
/*****************************************************************************/
LOCAL void BrwCreateHomePageTypePopMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : handle Setting list msg
//  Global resource dependence : 
//  Author: Jiaoyouw.wu
//  Note: 
/*****************************************************************************/
LOCAL void BrwCreateFontPopMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : handle Setting list msg
//  Global resource dependence : 
//  Author: Jiaoyouw.wu
//  Note: 
/*****************************************************************************/
LOCAL void BrwUpdateSettingList(MMI_CTRL_ID_T ctrl_id);


/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyouw.wu
//  Note: 
/*****************************************************************************/
LOCAL void BrwHandleSetttingHomageType(void);

#ifdef MMI_GPRS_DATA_SIM_SET
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyouw.wu
//  Note: 
/*****************************************************************************/
LOCAL void BrwHandleSetttingGPRSMode(void);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyouw.wu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN BrwIsSimChange(void);
#endif

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyouw.wu
//  Note: 
/*****************************************************************************/
LOCAL void BrwHandleSetttingHomageUrl(void);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyouw.wu
//  Note: 
/*****************************************************************************/
LOCAL void BrwHandleSetttingDownloadImage(void);

#ifdef MMIDORADO_BGSOUND_SUPPORT
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
LOCAL void BrwHandleSetttingBgsound(void);
#endif

#ifdef MMIDORADO_GOOGLE_TRANSCODE_SUPPORT
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyouw.wu
//  Note: 
/*****************************************************************************/
LOCAL void BrwHandleSetttingGoogleTranscode(void);
#endif
    
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyouw.wu
//  Note: 
/*****************************************************************************/
LOCAL void BrwHandleSetttingFont(void);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyouw.wu
//  Note: 
/*****************************************************************************/
LOCAL void BrwHandleSetttingReset(void);


#ifdef MMI_SSL_CERT_SUPPORT
/*****************************************************************************/
//  Description :orange project,add root certificates
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void BrwHandleSetttingCertificates(void);
#endif

/*****************************************************************************/
// Description : Handle query change service
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleSetttingResetAlertWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL void BrwHandleSetttingClear(void);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleSettingClearOptWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

#endif /* MMI_GUI_STYLE_TYPICAL */


#ifdef MMI_SSL_CERT_SUPPORT
/*****************************************************************************/
//  Description :root certificate win
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBRWCertificateWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);


/*****************************************************************************/
//  Description :root certificate win
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCerAttributeDetailWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : append certificate attribute to richtext
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
LOCAL void MMIBRW_Append_Certificate_Attribute( MMI_WIN_ID_T win_id );

/*****************************************************************************/
//  Description : add dynamic item to listbox when item_num is not zero
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void MMIBRW_AppendCertListItem( MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, uint16 item_num, MMI_TEXT_ID_T text1_id, MMI_TEXT_ID_T text2_id, MMI_TEXT_ID_T text3_id );

/*****************************************************************************/
//  Description :get num of root certificate
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL uint16 MMIBRW_GetCertInfoNum(void/*SSL_CERTINFO_T **cert_pptr*/);
#endif

/**--------------------------------------------------------------------------*
 **                         WINDOW DEFINITION                              *
 **--------------------------------------------------------------------------*/
#ifndef MMI_GUI_STYLE_TYPICAL
WINDOW_TABLE( MMIBROWSER_SETTING_WIN_TAB ) = 
{
    WIN_FUNC((uint32)BrwHandleSettingListWinMsg),    
    WIN_ID(MMIBROWSER_SETTING_WIN_ID),
    WIN_TITLE(STXT_MAIN_SETTING),

    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIBROWSER_SETTING_LIST_CTRL_ID),
#ifndef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    END_WIN
};

WINDOW_TABLE( MMIBROWSER_SETTING_HOMEPAGE_TYPE_OPT_WIN_TAB ) = 
{
    WIN_FUNC((uint32) BrwHandleSettingHomepageTypeWinMsg), 
    WIN_ID(MMIBROWSER_SETTING_HOMEPAGE_TYPE_OPT_WIN_ID),
    WIN_TITLE(TXT_BROWSER_SET_HOMEPAGE_TYPE),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIBROWSER_SETTING_HOMEPAGE_TYRE_MENU_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

WINDOW_TABLE( MMIBROWSER_SETTING_FONT_OPT_WIN_TAB ) = 
{
    WIN_FUNC((uint32) BrwHandleSettingFontWinMsg), 
    WIN_ID(MMIBROWSER_SETTING_FONT_OPT_WIN_ID),
    WIN_TITLE(TXT_COMMON_FONT_SIZE),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIBROWSER_SETTING_FONT_MENU_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

WINDOW_TABLE(MMIBROWSER_SETTING_HOMEPAGE_WIN_TAB) = 
{
    WIN_TITLE(TXT_BROWSER_CUSTOM_HOMEPAGE),
    WIN_FUNC((uint32)BrwHandleSettingHomepageWinMsg),    
    WIN_ID(MMIBROWSER_SETTING_HOMEPAGE_WIN_ID),
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_TEXT_CTRL(MMIBROWSER_HOMEPAGE_MAX_URL_LEN, MMIBROWSER_SETTING_EDIT_HOMEPAGE_CTRL_ID),
    END_WIN
};

WINDOW_TABLE( MMIBROWSER_SETTING_CLEAR_OPT_WIN_TAB ) = 
{
    WIN_FUNC((uint32)BrwHandleSettingClearOptWinMsg), 
    WIN_ID(MMIBROWSER_SETTING_CLEAR_OPT_WIN_ID),
    WIN_TITLE(TXT_BROWSER_PDA_CLEAR),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_CHECKLIST_E, MMIBROWSER_SETTING_CLEAR_OPT_CTRL_ID),
    END_WIN
};

#else
WINDOW_TABLE( MMIBROWSER_SETTING_WIN_TAB ) = 
{
    WIN_FUNC((uint32)BrwHandleSettingWinMsg),    
    WIN_ID(MMIBROWSER_SETTING_WIN_ID),
    WIN_TITLE(STXT_MAIN_SETTING),
    WIN_SOFTKEY(STXT_SAVE, TXT_NULL, STXT_RETURN),
    WIN_BACKGROUND_ID(IMAGE_COMMON_BG),

    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIBROWSER_SETTING_FORM_CTRL_ID),
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIBROWSER_SETTING_SIM1_FORM_CTRL_ID,MMIBROWSER_SETTING_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIBROWSER_SETTING_SIM1_TITLE_LABEL_CTRL_ID, MMIBROWSER_SETTING_SIM1_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, MMIBROWSER_SETTING_SIM1_LABEL_CTRL_ID, MMIBROWSER_SETTING_SIM1_FORM_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIBROWSER_SETTING_SIM2_FORM_CTRL_ID,MMIBROWSER_SETTING_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIBROWSER_SETTING_SIM2_TITLE_LABEL_CTRL_ID, MMIBROWSER_SETTING_SIM2_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, MMIBROWSER_SETTING_SIM2_LABEL_CTRL_ID, MMIBROWSER_SETTING_SIM1_FORM_CTRL_ID),

    #if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIBROWSER_SETTING_SIM3_FORM_CTRL_ID,MMIBROWSER_SETTING_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIBROWSER_SETTING_SIM3_TITLE_LABEL_CTRL_ID, MMIBROWSER_SETTING_SIM3_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, MMIBROWSER_SETTING_SIM3_LABEL_CTRL_ID, MMIBROWSER_SETTING_SIM1_FORM_CTRL_ID),

    #endif
    #if defined(MMI_MULTI_SIM_SYS_QUAD)
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIBROWSER_SETTING_SIM4_FORM_CTRL_ID,MMIBROWSER_SETTING_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIBROWSER_SETTING_SIM4_TITLE_LABEL_CTRL_ID, MMIBROWSER_SETTING_SIM4_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, MMIBROWSER_SETTING_SIM4_LABEL_CTRL_ID, MMIBROWSER_SETTING_SIM1_FORM_CTRL_ID),

    #endif
#else
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIBROWSER_SETTING_SIM1_FORM_CTRL_ID,MMIBROWSER_SETTING_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIBROWSER_SETTING_SIM1_TITLE_LABEL_CTRL_ID, MMIBROWSER_SETTING_SIM1_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, MMIBROWSER_SETTING_SIM1_LABEL_CTRL_ID, MMIBROWSER_SETTING_SIM1_FORM_CTRL_ID),

#endif
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIBROWSER_SETTING_HOME_TYPE_FORM_CTRL_ID,MMIBROWSER_SETTING_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIBROWSER_SETTING_HOME_TYPE_LABEL_CTRL_ID, MMIBROWSER_SETTING_HOME_TYPE_FORM_CTRL_ID),
        CHILD_DROPDOWNLIST_CTRL(TRUE, MMIBROWSER_SETTING_HOME_TYPE_DROPDOWNLIST_CTRL_ID, MMIBROWSER_SETTING_HOME_TYPE_FORM_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIBROWSER_SETTING_HOME_FORM_CTRL_ID,MMIBROWSER_SETTING_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIBROWSER_SETTING_HOME_LABEL_CTRL_ID, MMIBROWSER_SETTING_HOME_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, MMIBROWSER_SETTING_HOME_CONTENT_LABEL_CTRL_ID, MMIBROWSER_SETTING_HOME_FORM_CTRL_ID),
        //CHILD_EDIT_TEXT_CTRL(TRUE, MMIBROWSER_HOMEPAGE_MAX_URL_LEN, MMIBROWSER_SETTING_HOME_EDIT_CTRL_ID, MMIBROWSER_SETTING_HOME_FORM_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIBROWSER_SETTING_IMAGE_FORM_CTRL_ID,MMIBROWSER_SETTING_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIBROWSER_SETTING_IMAGE_LABEL_CTRL_ID, MMIBROWSER_SETTING_IMAGE_FORM_CTRL_ID),
        CHILD_DROPDOWNLIST_CTRL(TRUE, MMIBROWSER_SETTING_IMAGE_DROPDOWNLIST_CTRL_ID, MMIBROWSER_SETTING_IMAGE_FORM_CTRL_ID),

#ifdef MMIDORADO_BGSOUND_SUPPORT
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIBROWSER_SETTING_BGSOUND_FORM_CTRL_ID,MMIBROWSER_SETTING_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIBROWSER_SETTING_BGSOUND_LABEL_CTRL_ID, MMIBROWSER_SETTING_BGSOUND_FORM_CTRL_ID),
        CHILD_DROPDOWNLIST_CTRL(TRUE, MMIBROWSER_SETTING_BGSOUND_DROPDOWNLIST_CTRL_ID, MMIBROWSER_SETTING_BGSOUND_FORM_CTRL_ID),
#endif

#ifdef MMIDORADO_GOOGLE_TRANSCODE_SUPPORT
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIBROWSER_SETTING_GOOGLE_FORM_CTRL_ID,MMIBROWSER_SETTING_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIBROWSER_SETTING_GOOGLE_LABEL_CTRL_ID, MMIBROWSER_SETTING_GOOGLE_FORM_CTRL_ID),
        CHILD_DROPDOWNLIST_CTRL(TRUE, MMIBROWSER_SETTING_GOOGLE_DROPDOWNLIST_CTRL_ID, MMIBROWSER_SETTING_GOOGLE_FORM_CTRL_ID),
#endif

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIBROWSER_SETTING_FONT_FORM_CTRL_ID,MMIBROWSER_SETTING_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIBROWSER_SETTING_FONT_LABEL_CTRL_ID, MMIBROWSER_SETTING_FONT_FORM_CTRL_ID),
        CHILD_DROPDOWNLIST_CTRL(TRUE, MMIBROWSER_SETTING_FONT_DROPDOWNLIST_CTRL_ID, MMIBROWSER_SETTING_FONT_FORM_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIBROWSER_SETTING_CLEAR_FORM_CTRL_ID,MMIBROWSER_SETTING_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, MMIBROWSER_SETTING_CLEAR_LABEL_CTRL_ID, MMIBROWSER_SETTING_CLEAR_FORM_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIBROWSER_SETTING_DEFAULT_FORM_CTRL_ID,MMIBROWSER_SETTING_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, MMIBROWSER_SETTING_DEFAULT_LABEL_CTRL_ID, MMIBROWSER_SETTING_DEFAULT_FORM_CTRL_ID),

    END_WIN
};

//the window for input homepage
WINDOW_TABLE(MMIBROWSER_INPUTHOMEPAGE_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleInputHomepageWinMsg),    
    WIN_ID(MMIBROWSER_INPUTHOMEPAGE_WIN_ID),
    WIN_TITLE(TXT_BROWSER_CUSTOM_HOMEPAGE),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_TEXT_CTRL(MMIBROWSER_HOMEPAGE_MAX_URL_LEN, MMIBROWSER_SETTING_HOME_EDIT_CTRL_ID),
    END_WIN
};
#endif /* MMI_GUI_STYLE_TYPICAL */

#ifdef MMI_SSL_CERT_SUPPORT
//the window for certificate win
WINDOW_TABLE( MMIBRW_CERTIFICATE_WIN_TAB ) =
{
    WIN_FUNC((uint32)HandleBRWCertificateWinMsg),
	WIN_ID(MMIBRW_CERTIFICATE_WIN_ID),
    WIN_TITLE(TXT_BROWSER_CERTICIFATES),
	WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
	CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIBRW_LISTBOX_CERTIFICATE_CTRL_ID),
	END_WIN
};

//the window for brw certificate attribute win
WINDOW_TABLE( MMIBRW_CERTIFICATE_ATTRIBUTE_WIN_TAB ) =
{
    WIN_TITLE(TXT_BROWSER_CERT_ATTRIBUTE),
    WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
    WIN_FUNC((uint32)HandleCerAttributeDetailWinMsg),    
    WIN_ID(MMIBRW_CERTIFICATE_ATTRIBUTE_WIN_ID),
    CREATE_RICHTEXT_CTRL(MMIBRW_RICHTEXT_CERTIFICATE_ATTRIBUTE_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    END_WIN
};
#endif
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : open the setting window
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_OpenSettingWin(void)
{
    MMK_CreateWin((uint32*)MMIBROWSER_SETTING_WIN_TAB,PNULL);
}

#ifdef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description : handle setting window message
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleSettingWinMsg(
                                   MMI_WIN_ID_T	win_id, 
                                   MMI_MESSAGE_ID_E	msg_id, 
                                   DPARAM		param
                                   )
{
    MMI_RESULT_E    result=MMI_RESULT_TRUE;
    BOOLEAN is_update_font = FALSE;
    BOOLEAN is_execute_setting_success = FALSE;
    LOCAL BOOLEAN   s_is_close_by_save = FALSE;

    switch(msg_id) 
    {
    case MSG_OPEN_WINDOW:
        s_is_close_by_save = FALSE;
        MMIBROWSER_SetSettingChange(FALSE);
        BrwSetSettingWindow(win_id);
        break;

    case MSG_FULL_PAINT:
        BrwUpdateSimNetAccountLabelText(win_id);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK://save
        BrwSetSetting(&is_update_font);
        
        is_execute_setting_success = BrwExecuteSetting(is_update_font);//execute the setting:set profile, update url......

        if (is_execute_setting_success)
        {
#ifdef MMI_GUI_STYLE_TYPICAL
            MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
#endif /* MMI_GUI_STYLE_TYPICAL */
        }
        s_is_close_by_save = TRUE;
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        switch (MMK_GetActiveCtrlId(win_id))
        {
        case MMIBROWSER_SETTING_DEFAULT_LABEL_CTRL_ID://恢复默认设置
            MMIBROWSER_OpenQueryWinByTextId(TXT_RESET_DEFAULT,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
            break;
            
        case MMIBROWSER_SETTING_HOME_CONTENT_LABEL_CTRL_ID:
            MMK_CreateWin((uint32*)MMIBROWSER_INPUTHOMEPAGE_WIN_TAB,PNULL);
            break;

        case MMIBROWSER_SETTING_SIM1_LABEL_CTRL_ID:
            BrwHandleSettingAccount1();
            break;

        case MMIBROWSER_SETTING_SIM2_LABEL_CTRL_ID:
            BrwHandleSettingAccount2();
            break; 

        case MMIBROWSER_SETTING_SIM3_LABEL_CTRL_ID:
            BrwHandleSettingAccount3();
            break;

        case MMIBROWSER_SETTING_SIM4_LABEL_CTRL_ID:
            BrwHandleSettingAccount4();
            break;

        case MMIBROWSER_SETTING_CLEAR_LABEL_CTRL_ID:
            break; 
            
        default:
            break;
        }
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
     case MSG_CTL_PENOK:
        if (param != PNULL)//恢复默认设置
        {
            if (MMIBROWSER_SETTING_DEFAULT_LABEL_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)
            {
                MMIBROWSER_OpenQueryWinByTextId(TXT_RESET_DEFAULT,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
            }
            else if (MMIBROWSER_SETTING_HOME_CONTENT_LABEL_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)
            {
                MMK_CreateWin((uint32*)MMIBROWSER_INPUTHOMEPAGE_WIN_TAB,PNULL);
            }
            else if (MMIBROWSER_SETTING_SIM1_LABEL_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)
            {
                BrwHandleSettingAccount1();
            }
            else if (MMIBROWSER_SETTING_SIM2_LABEL_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)
            {
                BrwHandleSettingAccount2();
            }
            else if (MMIBROWSER_SETTING_SIM3_LABEL_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)
            {
                BrwHandleSettingAccount3();
            }
            else if (MMIBROWSER_SETTING_SIM4_LABEL_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)
            {
                BrwHandleSettingAccount4();
            }
            else if (MMIBROWSER_SETTING_CLEAR_LABEL_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)
            {
                
            }
            else
            {
                
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_PROMPTWIN_OK:
        MMIBROWSER_CloseQuerytWin(PNULL);
#ifdef MMI_GUI_STYLE_TYPICAL
        MMIPUB_OpenAlertSuccessWin(TXT_SUCCESS);
#endif /* MMI_GUI_STYLE_TYPICAL */

        //todo 恢复到默认配置值
        MMIBROWSER_SetSettingChange(MMIBROWSER_IsSimIndexModifyByDefault(MMIBROWSER_GetSim()));
        is_update_font = MMIBROWSER_IsFontModifyByDefault();
        MMIDORADO_ResetFactorySetting();
        
        BrwExecuteSetting(is_update_font);
        s_is_close_by_save = TRUE;
        MMK_CloseWin(win_id);
        break;
        
    case MSG_PROMPTWIN_CANCEL:
        MMIBROWSER_CloseQuerytWin(PNULL);
        break;

    case MSG_CLOSE_WINDOW:
        if (s_is_close_by_save)
        {
            //do nothing
        }
        else//don/t save the modify data to nv
        {
            MMIBROWSER_InitSetting();//comeback the s_nv_setting from nv
        }
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}

/*****************************************************************************/
// 	Description : handle message of input homepage window of browser
//	Global resource dependence : none
//  Author: fen.xie
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleInputHomepageWinMsg(
                                             MMI_WIN_ID_T	    win_id, 
                                             MMI_MESSAGE_ID_E	msg_id, 
                                             DPARAM				param
                                             )
{
    MMI_RESULT_E	recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIBROWSER_SETTING_HOME_EDIT_CTRL_ID;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
    {
        GUIIM_TYPE_T allow_im = GUIIM_TYPE_NONE;
        GUIIM_TYPE_T init_im = GUIIM_TYPE_NONE;
        wchar tempbuf[MMIBROWSER_HOMEPAGE_MAX_URL_LEN+1]= {0};

        allow_im = GUIIM_TYPE_ENGLISH | GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL;/*lint !e655*/
        init_im = GUIIM_TYPE_ABC;
        GUIEDIT_SetIm(ctrl_id,allow_im,init_im);
        GUIEDIT_SetImTag(ctrl_id, GUIIM_TAG_URL);
        MMIAPICOM_StrToWstr(MMIBROWSER_GetHomepageSetting(), tempbuf);
        GUIEDIT_SetString(ctrl_id, tempbuf, MMIAPICOM_Wstrlen(tempbuf));
        MMK_SetAtvCtrl(win_id,ctrl_id);
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
        
        GUIEDIT_GetString(ctrl_id, &string_info);
        MMIBROWSER_SetHomepageSetting(&string_info);
        GUILABEL_SetText(MMIBROWSER_SETTING_HOME_CONTENT_LABEL_CTRL_ID, &string_info, FALSE);//update the label
        MMK_CloseWin(win_id);
    }
        break;    
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);//返回上一级
        break;
        
    default:	
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
    
}

/*****************************************************************************/
//  Description : set setting window
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void BrwSetSettingWindow(MMI_WIN_ID_T win_id)
{
    uint8   i = 0;    
    wchar   temp_arr[MMIBROWSER_HOMEPAGE_MAX_URL_LEN+1]= {0};
    uint16  ver_space = 2;
    MMI_CTRL_ID_T avtive_ctrl_id = 0;
    MMIBROWSER_NV_SETTING_T *nv_setting_ptr = PNULL;
    MMI_STRING_T    str_item[MMIBROWSER_SETTING_IMAGE_NUM] = {0};
    MMI_STRING_T    font_item[MMIBROWSER_FONT_MAX] = {0};
    MMI_TEXT_ID_T   font_text_arr[MMIBROWSER_FONT_MAX] = {TXT_FONT_SMALL, TXT_COMMON_NORMAL, TXT_COMM_FONT_BIG};
    MMI_CTRL_ID_T sim_label_ctrl_id[] = {
            MMIBROWSER_SETTING_SIM1_LABEL_CTRL_ID,
            MMIBROWSER_SETTING_SIM2_LABEL_CTRL_ID,
            MMIBROWSER_SETTING_SIM3_LABEL_CTRL_ID,
            MMIBROWSER_SETTING_SIM4_LABEL_CTRL_ID
        };
    MMI_STRING_T    str = {0};

    GUIFORM_SetSpace(MMIBROWSER_SETTING_FORM_CTRL_ID, PNULL, &ver_space);
    
    //BEGIN:sim network account
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    {
        MMI_CTRL_ID_T text_ctrl_id[]={
            MMIBROWSER_SETTING_SIM1_TITLE_LABEL_CTRL_ID,
            MMIBROWSER_SETTING_SIM2_TITLE_LABEL_CTRL_ID,
            MMIBROWSER_SETTING_SIM3_TITLE_LABEL_CTRL_ID,
            MMIBROWSER_SETTING_SIM4_TITLE_LABEL_CTRL_ID
        };

        MMI_CTRL_ID_T form_ctrl_id[]={
            MMIBROWSER_SETTING_SIM1_FORM_CTRL_ID,
            MMIBROWSER_SETTING_SIM2_FORM_CTRL_ID,
            MMIBROWSER_SETTING_SIM3_FORM_CTRL_ID,
            MMIBROWSER_SETTING_SIM4_FORM_CTRL_ID
        };
        
        MMI_TEXT_ID_T text_id[]={
                TXT_COMMON_SIM1_NETWORK_ACCOUNT,
                TXT_COMMON_SIM2_NETWORK_ACCOUNT,
                TXT_BROWSER_SIM3,
                TXT_BROWSER_SIM4
        };
        
        for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
        {
            GUIFORM_SetStyle(form_ctrl_id[i],GUIFORM_STYLE_UNIT);
            GUILABEL_SetTextById(text_ctrl_id[i],text_id[i],FALSE);
            if (!MMIAPIPHONE_IsSimAvailable((MN_DUAL_SYS_E)i))
            {
                GUIFORM_SetChildDisplay(MMIBROWSER_SETTING_FORM_CTRL_ID, form_ctrl_id[i], GUIFORM_CHILD_DISP_HIDE);
            }
            else
            {
                if (0 == avtive_ctrl_id)
                {
                    avtive_ctrl_id = sim_label_ctrl_id[i];
                }
            }
        }
    }
#else
    GUIFORM_SetStyle(MMIBROWSER_SETTING_SIM1_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUILABEL_SetTextById(MMIBROWSER_SETTING_SIM1_TITLE_LABEL_CTRL_ID,TXT_COMMON_WWW_ACCOUNT,FALSE);
    if (!MMIAPIPHONE_IsSimAvailable(MN_DUAL_SYS_1))
    {
        GUIFORM_SetChildDisplay(MMIBROWSER_SETTING_FORM_CTRL_ID, MMIBROWSER_SETTING_SIM1_FORM_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
    }
    else
    {
        if (0 == avtive_ctrl_id)
        {
            avtive_ctrl_id = MMIBROWSER_SETTING_SIM1_FORM_CTRL_ID;
        }
    }
#endif
    nv_setting_ptr = MMIBROWSER_GetNVSetting();    
    if (0 == avtive_ctrl_id)//without sim setting,set the first crtl to active
    {
        avtive_ctrl_id = MMIBROWSER_SETTING_HOME_TYPE_FORM_CTRL_ID;
    }

    //other setting option
    GUIFORM_SetStyle(MMIBROWSER_SETTING_HOME_TYPE_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIBROWSER_SETTING_HOME_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIBROWSER_SETTING_IMAGE_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
#ifdef MMIDORADO_GOOGLE_TRANSCODE_SUPPORT
    GUIFORM_SetStyle(MMIBROWSER_SETTING_GOOGLE_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
#endif
    GUIFORM_SetStyle(MMIBROWSER_SETTING_FONT_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIBROWSER_SETTING_DEFAULT_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
#ifdef MMIDORADO_BGSOUND_SUPPORT
    GUIFORM_SetStyle(MMIBROWSER_SETTING_BGSOUND_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
#endif
    GUIFORM_SetStyle(MMIBROWSER_SETTING_CLEAR_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);

    GUILABEL_SetTextById(MMIBROWSER_SETTING_HOME_TYPE_LABEL_CTRL_ID,TXT_BROWSER_SET_HOMEPAGE_TYPE,FALSE);
    GUILABEL_SetTextById(MMIBROWSER_SETTING_HOME_LABEL_CTRL_ID,TXT_BROWSER_CUSTOM_HOMEPAGE,FALSE);
    GUILABEL_SetTextById(MMIBROWSER_SETTING_IMAGE_LABEL_CTRL_ID,TXT_BROWSER_PIC_DOWNLOAD,FALSE);
#ifdef MMIDORADO_GOOGLE_TRANSCODE_SUPPORT
    GUILABEL_SetTextById(MMIBROWSER_SETTING_GOOGLE_LABEL_CTRL_ID,TXT_BROWSER_GOOGLE,FALSE);
#endif
    GUILABEL_SetTextById(MMIBROWSER_SETTING_FONT_LABEL_CTRL_ID,TXT_COMMON_FONT_SIZE,FALSE);
    GUILABEL_SetTextById(MMIBROWSER_SETTING_DEFAULT_LABEL_CTRL_ID,TXT_RESET_DEFAULT,FALSE);
#ifdef MMIDORADO_BGSOUND_SUPPORT
    GUILABEL_SetTextById(MMIBROWSER_SETTING_BGSOUND_LABEL_CTRL_ID,TXT_WWW_BGMUSIC,FALSE);
#endif
    GUILABEL_SetTextById(MMIBROWSER_SETTING_CLEAR_LABEL_CTRL_ID,TXT_BROWSER_PDA_CLEAR,FALSE);

    //image download and google change
    MMI_GetLabelTextByLang(TXT_COMMON_OPEN, &str_item[0]);
    MMI_GetLabelTextByLang(TXT_COMMON_CLOSE, &str_item[1]);
    
    GUIDROPDOWNLIST_AppendItemArray(MMIBROWSER_SETTING_IMAGE_DROPDOWNLIST_CTRL_ID,str_item,MMIBROWSER_SETTING_IMAGE_NUM);
    if (nv_setting_ptr->is_image_loaded)
    {
        GUIDROPDOWNLIST_SetCurItemIndex(MMIBROWSER_SETTING_IMAGE_DROPDOWNLIST_CTRL_ID,0);
    }
    else
    {
        GUIDROPDOWNLIST_SetCurItemIndex(MMIBROWSER_SETTING_IMAGE_DROPDOWNLIST_CTRL_ID,1);
    }

#ifdef MMIDORADO_GOOGLE_TRANSCODE_SUPPORT
    GUIDROPDOWNLIST_AppendItemArray(MMIBROWSER_SETTING_GOOGLE_DROPDOWNLIST_CTRL_ID,str_item,MMIBROWSER_SETTING_GOOGLE_NUM);
    if (nv_setting_ptr->is_google_change_enable)
    {
        GUIDROPDOWNLIST_SetCurItemIndex(MMIBROWSER_SETTING_GOOGLE_DROPDOWNLIST_CTRL_ID,0);
    }
    else
    {
        GUIDROPDOWNLIST_SetCurItemIndex(MMIBROWSER_SETTING_GOOGLE_DROPDOWNLIST_CTRL_ID,1);
    }
#endif    
    
#ifdef MMIDORADO_BGSOUND_SUPPORT
    GUIDROPDOWNLIST_AppendItemArray(MMIBROWSER_SETTING_BGSOUND_DROPDOWNLIST_CTRL_ID,str_item,MMIBROWSER_SETTING_IMAGE_NUM);
    if (nv_setting_ptr->is_sound_played)
    {
        GUIDROPDOWNLIST_SetCurItemIndex(MMIBROWSER_SETTING_BGSOUND_DROPDOWNLIST_CTRL_ID,0);
    }
    else
    {
        GUIDROPDOWNLIST_SetCurItemIndex(MMIBROWSER_SETTING_BGSOUND_DROPDOWNLIST_CTRL_ID,1);
    }
#endif
  
    //font
    for (i=0; i<MMIBROWSER_FONT_MAX; i++)
    {
        MMI_GetLabelTextByLang(font_text_arr[i], &font_item[i]);
    }
    GUIDROPDOWNLIST_AppendItemArray(MMIBROWSER_SETTING_FONT_DROPDOWNLIST_CTRL_ID,font_item,MMIBROWSER_FONT_MAX);
    GUIDROPDOWNLIST_SetCurItemIndex(MMIBROWSER_SETTING_FONT_DROPDOWNLIST_CTRL_ID,nv_setting_ptr->font);

    //set homepage mode
    MMI_GetLabelTextByLang(TXT_EXPORT_LOC_DEFAULT, &str_item[0]);
    MMI_GetLabelTextByLang(TXT_EXPORT_LOC_CUSTOM, &str_item[1]);
    GUIDROPDOWNLIST_AppendItemArray(MMIBROWSER_SETTING_HOME_TYPE_DROPDOWNLIST_CTRL_ID,str_item,MMIBROWSER_SETTING_IMAGE_NUM);
    GUIDROPDOWNLIST_SetCurItemIndex(MMIBROWSER_SETTING_HOME_TYPE_DROPDOWNLIST_CTRL_ID,nv_setting_ptr->homepage_mode);
    
    //set custom homepage label
    MMIAPICOM_StrToWstr(MMIBROWSER_GetHomepageSetting(), temp_arr);
    str.wstr_ptr = temp_arr;
    str.wstr_len = MMIAPICOM_Wstrlen(temp_arr);
    GUILABEL_SetText(MMIBROWSER_SETTING_HOME_CONTENT_LABEL_CTRL_ID, &str, FALSE);
    
    MMK_SetAtvCtrl(win_id, avtive_ctrl_id);
}

/*****************************************************************************/
//  Description : set sim network account label text
//  Global resource dependence : 
//  Author: jixin.xu
//  Note: 
/*****************************************************************************/
LOCAL void BrwUpdateSimNetAccountLabelText(MMI_WIN_ID_T win_id)
{
    uint8   i = 0;    
    MMICONNECTION_LINKSETTING_DETAIL_T  *link_setting_ptr = PNULL;
    MMIBROWSER_NV_SETTING_T *nv_setting_ptr = PNULL;
    MMI_CTRL_ID_T sim_label_ctrl_id[] = {
            MMIBROWSER_SETTING_SIM1_LABEL_CTRL_ID,
            MMIBROWSER_SETTING_SIM2_LABEL_CTRL_ID,
            MMIBROWSER_SETTING_SIM3_LABEL_CTRL_ID,
            MMIBROWSER_SETTING_SIM4_LABEL_CTRL_ID
        };
    MMI_STRING_T    string = {0};
    uint32          select_index = 0;

    nv_setting_ptr = MMIBROWSER_GetNVSetting();    
    for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {      
        /*set SIM label*/
        select_index = nv_setting_ptr->sim_net_index[i];

        link_setting_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex((MN_DUAL_SYS_E)i,select_index);

        string.wstr_len = link_setting_ptr->name_len;
        string.wstr_ptr = (wchar *)link_setting_ptr->name;

        GUILABEL_SetText(sim_label_ctrl_id[i], &string, FALSE); // text
    }
}
#endif /* MMI_GUI_STYLE_TYPICAL */

/*****************************************************************************/
//  Description : execute the setting to profile and update web page for new settings
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN BrwExecuteSetting(BOOLEAN is_update_font)
{
    BRW_PROFILE_T   profile = {0};
    BRW_INSTANCE    brw_instance = 0;
    char    *url_ptr = PNULL;
    BRW_PUBLIC_PROFILE_T   public_profile = MMIBROWSER_SetPublicProfile();
    MN_DUAL_SYS_E   active_sim = MMIBROWSER_GetSim();
    BOOLEAN result =TRUE;

    if (!MMIBROWSER_GetIsUseWifi() && (active_sim < MMI_DUAL_SYS_MAX))
    {//reset the customized connection for update the network status  
        MMIDORADO_CUSTOMIZED_CONNECTION_T   customized_connection = MMIDORADO_GetConnectionSettings();
        BOOLEAN is_valid_customized_connection = FALSE;

        if (IS_VALID_CUSTOMIZED_CONNECTION(customized_connection, active_sim))
        {
            is_valid_customized_connection = TRUE;
            //SCI_TRACE_LOW:"[BRW]BrwExecuteSetting:reset customized_connection"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_SETTING_851_112_2_18_2_6_42_317,(uint8*)"");
        }      
        
        if (is_valid_customized_connection)//reset
        {
            customized_connection.is_connection_customized = FALSE;
            MMIDORADO_SetConnectionSettings(customized_connection);
            MMIBROWSER_SetSettingChange(TRUE);
        }
    }

#ifdef MMI_GPRS_DATA_SIM_SET
    if (BrwIsSimChange())
    {
        MN_DUAL_SYS_E               dual_sys = MN_DUAL_SYS_1;
        MMISET_GPRS_MODE_TYPE_E     mode = DATA_CARD_MODE_1;

        mode = MMIAPISET_GetGPRSModeType();

        dual_sys = MN_DUAL_SYS_1 + (mode - DATA_CARD_MODE_1);/*lint !e656*/

        Brw_ResetLinkSettings(MMIBROWSER_GetInstance());

        MMIBROWSER_SetSim(dual_sys);
        public_profile.dual_sys = dual_sys;
        BRW_SetPublicParam(&public_profile, BRW_PROXY_PROFILE);

        MMIBROWSER_SetSettingChange(FALSE);
    }
#endif

    if (!MMIBROWSER_GetIsUseWifi() && MMIBROWSER_GetSettingChange()
        && MMK_IsOpenWin(MMIBROWSER_MAIN_WIN_ID)//MS00246891
        )
    {
        profile = MMIBROWSER_SetProfile();

        MMIBROWSER_SetSettingChange(FALSE);
        BRW_SetNetLinked(FALSE);//MS00223556,避免此时net_linked的状态错误.同一张卡的不需要等待deactive
        DeactiveNetwork(MMIBROWSER_GetInstance());
        BRW_DestroyInstance(MMIBROWSER_GetInstance());
        BrwSetPublicProfile(&public_profile);
        brw_instance=BRW_InitInstance(&profile, FALSE);
        if (0 == brw_instance)//init error
        {
            MMIPUB_OpenAlertFailWin(TXT_BROWSER_INIT_FAIL);
            MMIBROWSER_Exit();
            //SCI_TRACE_LOW:"[BRW]W BrwExecuteSetting:BRW_InitInstance error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_SETTING_877_112_2_18_2_6_42_318,(uint8*)"");
            result = FALSE;
        }
        else
        {
            MMIBROWSER_SetInstance(brw_instance);

            url_ptr = MMIBROWSER_GetUrl();
            if (PNULL == url_ptr)
            {
                url_ptr = (char*)MMIBROWSER_GetHomepageUrl();
                if (PNULL != url_ptr)
                {
                    MMIBROWSER_StartWeb(MMIBROWSER_WEB_ENTRY_URL, (uint8*)url_ptr, PNULL);//此处不判断url有效性,在access时判断
                }
                else
                {
                    //SCI_TRACE_LOW:"[BRW]W BrwExecuteSetting MMIBROWSER_GetHomepageUrl ERROR"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_SETTING_892_112_2_18_2_6_42_319,(uint8*)"");
                }
            }
            else
            {
                MMIBROWSER_StartWeb(MMIBROWSER_WEB_ENTRY_URL, (uint8*)url_ptr, PNULL);//此处不判断url有效性,在access时判断
            }
        }
    }
    else
    {
        if (!MMIBROWSER_GetIsUseWifi() && MMIBROWSER_GetSettingChange())
        {
            DeactiveNetwork(BRW_INVALID_INSTANCE);;
            BRW_SetPublicParam(&public_profile, BRW_PROXY_PROFILE);
        }
        BRW_SetPublicParam(&public_profile, BRW_CACHE_PROFILE|BRW_COOKIE_PROFILE|BRW_IMG_DL_PROFILE|BRW_BGSOUND_DL_PROFILE|BRW_GWT_PROFILE);/*lint !e655*/
        
        if (is_update_font)
        {
            profile = MMIBROWSER_SetProfile();
            BRW_SetParam(MMIBROWSER_GetInstance(),&profile,BRW_FONT_PROFILE);
        }
    }

    MMIBROWSER_FreePublicProfile(public_profile);
    
    return result;
}

#ifdef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description : set browser setting to nv
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL void BrwSetSetting(BOOLEAN *is_update_font_ptr)
{
    uint16  index_id = 0;
    uint32 i = MN_DUAL_SYS_1;
    MMIBROWSER_NV_SETTING_T *nv_ptr = MMIBROWSER_GetNVSetting();
    //MMI_STRING_T string_info = {0};

    index_id = GUIDROPDOWNLIST_GetCurItemIndex(MMIBROWSER_SETTING_IMAGE_DROPDOWNLIST_CTRL_ID);
    if (0 == index_id)
    {
        MMIBROWSER_SetImageSetting(TRUE);
    }
    else
    {
        MMIBROWSER_SetImageSetting(FALSE);
    }
    
#ifdef MMIDORADO_GOOGLE_TRANSCODE_SUPPORT
    index_id = GUIDROPDOWNLIST_GetCurItemIndex(MMIBROWSER_SETTING_GOOGLE_DROPDOWNLIST_CTRL_ID);
    if (0 == index_id)
    {
        MMIBROWSER_SetGoogleSetting(TRUE);
    }
    else
    {
        MMIBROWSER_SetGoogleSetting(FALSE);
    }
#endif

#ifdef MMIDORADO_BGSOUND_SUPPORT
    index_id = GUIDROPDOWNLIST_GetCurItemIndex(MMIBROWSER_SETTING_BGSOUND_DROPDOWNLIST_CTRL_ID);
    if (0 == index_id)
    {
        MMIBROWSER_SetBgsoundSetting(TRUE);
    }
    else
    {
        MMIBROWSER_SetBgsoundSetting(FALSE);
    }
#endif    
    
    index_id = GUIDROPDOWNLIST_GetCurItemIndex(MMIBROWSER_SETTING_FONT_DROPDOWNLIST_CTRL_ID);
    if (nv_ptr->font != index_id)
    {
        if (is_update_font_ptr != PNULL)
        {
            *is_update_font_ptr = TRUE;
        }
        MMIBROWSER_SetFontSetting((MMIBROWSER_FONT_E)index_id);
    }
    
    //homepage mode
    index_id = GUIDROPDOWNLIST_GetCurItemIndex(MMIBROWSER_SETTING_HOME_TYPE_DROPDOWNLIST_CTRL_ID);
    MMIBROWSER_SetHomepageModeSetting((MMIBROWSER_HOMEPAGE_MODE_E)index_id);
    
    //custom homepage
    //GUIEDIT_GetString(MMIBROWSER_SETTING_HOME_EDIT_CTRL_ID, &string_info);
    //MMIBROWSER_SetHomepageSetting(&string_info);

    MMIBROWSER_UpdateNVSetting();//save nv setting
}
#endif /* MMI_GUI_STYLE_TYPICAL */

/*****************************************************************************/
//  Description : set sim's network accout index setting for wap setting
//  Global resource dependence : s_nv_setting
//  Author: jixin.xu
//  Note: 
/*****************************************************************************/
LOCAL void BrwSetNetIndexSettingCallback(MN_DUAL_SYS_E dual_sys, uint32 index)
{
    if (dual_sys < MMI_DUAL_SYS_MAX)
    {
        MMIBROWSER_NV_SETTING_T *nv_ptr = MMIBROWSER_GetNVSetting();
        MN_DUAL_SYS_E   sim_type = MMIBROWSER_GetSim();

        if ((dual_sys == sim_type) &&(index != nv_ptr->sim_net_index[dual_sys]))
        {
            MMIBROWSER_SetSettingChange(TRUE);
        }
        MMIBROWSER_SetNetIndexSetting(dual_sys, index);
    }
    
    return;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyouw.wu
//  Note: 
/*****************************************************************************/
LOCAL void BrwHandleSettingAccount1(void)
{
    MMIAPICONNECTION_CreateLinklistWin(MMICONNECTION_ACCOUNT_BROWSER, 
        MMIAPICONNECTION_GetListIndexByAPNIndex(MN_DUAL_SYS_1, MMIDORADO_GetNetSettingIndex((MN_DUAL_SYS_E)MN_DUAL_SYS_1)),
        MN_DUAL_SYS_1, BrwSetNetIndexSettingCallback);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyouw.wu
//  Note: 
/*****************************************************************************/
LOCAL void BrwHandleSettingAccount2(void)
{
    MMIAPICONNECTION_CreateLinklistWin(MMICONNECTION_ACCOUNT_BROWSER, 
        MMIAPICONNECTION_GetListIndexByAPNIndex((MN_DUAL_SYS_1 + 1), MMIDORADO_GetNetSettingIndex((MN_DUAL_SYS_E)(MN_DUAL_SYS_1 + 1))),
        (MN_DUAL_SYS_1 + 1), BrwSetNetIndexSettingCallback);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyouw.wu
//  Note: 
/*****************************************************************************/
LOCAL void BrwHandleSettingAccount3(void)
{
    MMIAPICONNECTION_CreateLinklistWin(MMICONNECTION_ACCOUNT_BROWSER, 
        MMIAPICONNECTION_GetListIndexByAPNIndex((MN_DUAL_SYS_1 + 2), MMIDORADO_GetNetSettingIndex((MN_DUAL_SYS_E)(MN_DUAL_SYS_1 + 2))),
        (MN_DUAL_SYS_1 + 2), BrwSetNetIndexSettingCallback);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyouw.wu
//  Note: 
/*****************************************************************************/
LOCAL void BrwHandleSettingAccount4(void)
{
	MMIAPICONNECTION_CreateLinklistWin(MMICONNECTION_ACCOUNT_BROWSER, 
        MMIAPICONNECTION_GetListIndexByAPNIndex((MN_DUAL_SYS_1 + 3), MMIDORADO_GetNetSettingIndex((MN_DUAL_SYS_E)(MN_DUAL_SYS_1 + 3))),
        (MN_DUAL_SYS_1 + 3), BrwSetNetIndexSettingCallback);
}

#ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description : handle Setting list msg
//  Global resource dependence : 
//  Author: Jiaoyouw.wu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleSettingListWinMsg(
                                   MMI_WIN_ID_T	win_id, 
                                   MMI_MESSAGE_ID_E	msg_id, 
                                   DPARAM		param
                                   )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
	LOCAL int32        list_offset_y = 0;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
		list_offset_y = 0;
        MMIBROWSER_SetSettingChange(FALSE);
        s_is_has_reset = FALSE;
        GUILIST_SetMaxItem(MMIBROWSER_SETTING_LIST_CTRL_ID, MMIBROWSER_SETTING_LIST_ITEM, FALSE);
        MMK_SetAtvCtrl(win_id, MMIBROWSER_SETTING_LIST_CTRL_ID);
        break;
    case MSG_FULL_PAINT:
        GUILIST_GetTopItemOffset(MMIBROWSER_SETTING_LIST_CTRL_ID, &list_offset_y);
        BrwUpdateSettingList(MMIBROWSER_SETTING_LIST_CTRL_ID);
        //set list offset
        GUILIST_SetTopItemOffset(MMIBROWSER_SETTING_LIST_CTRL_ID, list_offset_y);
        break;
	case MSG_LOSE_FOCUS:
        GUILIST_GetTopItemOffset(MMIBROWSER_SETTING_LIST_CTRL_ID, &list_offset_y);
        break;
    case MSG_GET_FOCUS:
		//set list offset
        GUILIST_SetTopItemOffset(MMIBROWSER_SETTING_LIST_CTRL_ID, list_offset_y);
        break;
    case MSG_CTL_LIST_LONGOK:
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        {
            uint32              index = 0;
            SETTING_ITEM_FUNC   item_func = PNULL;

            index = GUILIST_GetCurItemIndex(MMIBROWSER_SETTING_LIST_CTRL_ID);

            GUILIST_GetItemData(MMIBROWSER_SETTING_LIST_CTRL_ID, index, (uint32*)(&item_func));

            if (PNULL != item_func)
            {
                (*item_func)();
            }
        }

        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        if (!s_is_has_reset)
        {
            BOOLEAN is_update_font = FALSE;
            //MN_DUAL_SYS_E   sim_type = 0;
            MMIBROWSER_NV_SETTING_T     nv_setting = {0};
            MN_RETURN_RESULT_E          nv_result = MN_RETURN_FAILURE;

            MMINV_READ(MMIBROWSER_SETTING, &nv_setting, nv_result);
            
            if (MN_RETURN_SUCCESS == nv_result)
            {
                if (MMIBROWSER_GetFontSetting() != nv_setting.font)
                {
                    is_update_font = TRUE;
                }
                
                //sim_type = MMIBROWSER_GetSim();
                //if ((sim_type < MMI_DUAL_SYS_MAX) 
                //    && (MMIDORADO_GetNetSettingIndex(sim_type) != nv_setting.sim_net_index[sim_type]))//current used sim setting has been changed
                //{
                //    MMIBROWSER_SetSettingChange(TRUE);
                //}
                //else
                //{
                //    MMIBROWSER_SetSettingChange(FALSE);
                //}
            }
            else//protect
            {
                is_update_font = TRUE;
                MMIBROWSER_SetSettingChange(TRUE);
            }
            
            MMIBROWSER_UpdateNVSetting();

            BrwExecuteSetting(is_update_font);
        }
        else
        {
            //has BrwExecuteSetting by reset, do nothing
            s_is_has_reset = FALSE;//reset the param
        }
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : handle Setting list msg
//  Global resource dependence : 
//  Author: Jiaoyouw.wu
//  Note: 
/*****************************************************************************/
LOCAL void BrwCreateHomePageTypePopMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    MMI_TEXT_ID_T       text_index[] = {TXT_EXPORT_LOC_DEFAULT, TXT_EXPORT_LOC_CUSTOM};
    uint32              node_index[] = {MMIBROWSER_HOMEPAGE_TYPE_DEFAULT, MMIBROWSER_HOMEPAGE_TYPE_CUSTOMIZE};
    uint16              select_index = 0;
    uint32              num = 0;
    MMIBROWSER_NV_SETTING_T     *nv_setting_ptr = PNULL;

    nv_setting_ptr = MMIBROWSER_GetNVSetting();

    num = sizeof(node_index) / sizeof(node_index[0]);

    select_index = nv_setting_ptr->homepage_mode;

    if (select_index >= num)
    {
        select_index = 0;
    }

    MMIBROWSER_CreateRadioListByTextId(win_id, ctrl_id, num, text_index, node_index, select_index);
}

/*****************************************************************************/
//  Description : handle Setting list msg
//  Global resource dependence : 
//  Author: Jiaoyouw.wu
//  Note: 
/*****************************************************************************/
LOCAL void BrwCreateFontPopMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
#ifdef CSTAR_CHINESE_LIMITATION_SUPPORT
    MMI_TEXT_ID_T       text_index[] = {TXT_FONT_SMALL, TXT_COMM_FONT_BIG};
    uint32              node_index[] = {MMIBROWSER_FONT_TYPE_SMALL, MMIBROWSER_FONT_TYPE_BIG};
#else
	MMI_TEXT_ID_T       text_index[] = {TXT_FONT_SMALL, TXT_COMMON_NORMAL, TXT_COMM_FONT_BIG};
    uint32              node_index[] = {MMIBROWSER_FONT_TYPE_SMALL, MMIBROWSER_FONT_TYPE_NORMAL, MMIBROWSER_FONT_TYPE_BIG};
#endif
    uint16              select_index = 0;
    uint32              num = 0;
    MMIBROWSER_NV_SETTING_T     *nv_setting_ptr = PNULL;

    nv_setting_ptr = MMIBROWSER_GetNVSetting();

    num = sizeof(node_index) / sizeof(node_index[0]);

    select_index = nv_setting_ptr->font;

    MMIBROWSER_CreateRadioListByTextId(win_id, ctrl_id, num, text_index, node_index, select_index);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyouw.wu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleSettingHomepageTypeWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"BrwHandleSettingHomepageTypeWinMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_SETTING_1213_112_2_18_2_6_43_320,(uint8*)"d", msg_id);

    switch (msg_id) {
    case MSG_OPEN_WINDOW:
        BrwCreateHomePageTypePopMenu(win_id, MMIBROWSER_SETTING_HOMEPAGE_TYRE_MENU_CTRL_ID);
        MMK_SetAtvCtrl(win_id, MMIBROWSER_SETTING_HOMEPAGE_TYRE_MENU_CTRL_ID);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        {
            uint32 select_index = 0;
            MMIBROWSER_NV_SETTING_T     *nv_setting_ptr = PNULL;

            nv_setting_ptr = MMIBROWSER_GetNVSetting();

            select_index = GUILIST_GetCurItemIndex(MMIBROWSER_SETTING_HOMEPAGE_TYRE_MENU_CTRL_ID);

            if (nv_setting_ptr->homepage_mode != select_index)
            {
                nv_setting_ptr->homepage_mode = select_index;
            }

            MMK_CloseWin(win_id);
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
//  Description : handle Setting list msg
//  Global resource dependence : 
//  Author: Jiaoyouw.wu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleSettingHomepageWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E    ret=MMI_RESULT_TRUE;    

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUIIM_TYPE_T            allow_im = GUIIM_TYPE_NONE;
            GUIIM_TYPE_T            init_im = GUIIM_TYPE_NONE;
            MMI_CTRL_ID_T           ctrl_id = MMIBROWSER_SETTING_EDIT_HOMEPAGE_CTRL_ID;
            wchar                   temp_arr[MMIBROWSER_HOMEPAGE_MAX_URL_LEN + 1]= {0};

            allow_im = GUIIM_TYPE_ENGLISH | GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL;/*lint !e655*/
            init_im = GUIIM_TYPE_ABC;

            GUIEDIT_SetIm(ctrl_id, allow_im, init_im);
            GUIEDIT_SetImTag(ctrl_id, GUIIM_TAG_URL);

            MMIAPICOM_StrToWstr(MMIBROWSER_GetHomepageSetting(), temp_arr);
            GUIEDIT_SetString(ctrl_id, temp_arr, MMIAPICOM_Wstrlen(temp_arr));
            MMK_SetAtvCtrl(win_id,ctrl_id);
        }
        break;

    case MSG_CTL_OK:
        {
            MMI_STRING_T    string_info = {0};

            GUIEDIT_GetString(MMIBROWSER_SETTING_EDIT_HOMEPAGE_CTRL_ID, &string_info);
            MMIBROWSER_SetHomepageSetting(&string_info);

            MMK_CloseWin(win_id);
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
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
                else if (ctrl_id == MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID)
                {
                    MMK_PostMsg(win_id, MSG_CTL_MIDSK, PNULL, 0);
                }
            }
        }
        break;

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
//  Description : handle Setting list msg
//  Global resource dependence : 
//  Author: Jiaoyouw.wu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleSettingFontWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"BrwHandleSettingFontWinMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_SETTING_1342_112_2_18_2_6_43_321,(uint8*)"d", msg_id);

    switch (msg_id) {
    case MSG_OPEN_WINDOW:
        BrwCreateFontPopMenu(win_id, MMIBROWSER_SETTING_FONT_MENU_CTRL_ID);
        MMK_SetAtvCtrl(win_id, MMIBROWSER_SETTING_FONT_MENU_CTRL_ID);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        {
            uint32 select_index = 0;

            select_index = GUILIST_GetCurItemIndex(MMIBROWSER_SETTING_FONT_MENU_CTRL_ID);

            if (select_index < MMIBROWSER_FONT_MAX)
            {
                MMIBROWSER_SetFontSetting((MMIBROWSER_FONT_E)select_index);
            }

            MMK_CloseWin(win_id);
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
//  Description :
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleSettingClearOptWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   win_ctrl_id = MMIBROWSER_SETTING_CLEAR_OPT_CTRL_ID;

    //SCI_TRACE_LOW:"BrwHandleSettingHomepageTypeWinMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_SETTING_1390_112_2_18_2_6_43_322,(uint8*)"d", msg_id);

    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        {    
            uint16                  index = 0;
            uint16                  node_num = 0;
            GUILIST_ITEM_T          item = {0};
            GUILIST_ITEM_DATA_T     item_data = {0};
            MMIBROWSER_NV_SETTING_T *nv_setting_ptr = MMIBROWSER_GetNVSetting();

            MMI_TEXT_ID_T   text_id_array[] = {TXT_COMMON_HISTORY, 
#ifdef MMIDORADO_INPUT_HISTORY_SUPPORT
				TXT_BROWSER_INPUT_HISTORY, 
#endif
#ifdef BROWSER_SEARCH_SUPPORT
                            TXT_BROWSER_KEY, 
#endif            
                            TXT_BROWSER_CACHE, 
                            TXT_COOKIE
                            };
            
            MMIBROWSER_CLEAR_TYPE_E  node_array[] = {MMIBROWSER_CLEAR_TYPE_HISTORY, 
#ifdef MMIDORADO_INPUT_HISTORY_SUPPORT
                MMIBROWSER_CLEAR_TYPE_INPUT_HISTORY, 
#endif                
#ifdef BROWSER_SEARCH_SUPPORT
                            MMIBROWSER_CLEAR_TYPE_KEY, 
#endif
                            MMIBROWSER_CLEAR_TYPE_CACHE, 
                            MMIBROWSER_CLEAR_TYPE_COOKIE
                            };

            node_num = sizeof(text_id_array) / sizeof(text_id_array[0]);

            item.item_data_ptr = &item_data;
            item.item_style = GUIITEM_STYLE_ONE_LINE_CHECK;
            GUILIST_SetMaxItem(win_ctrl_id, node_num, FALSE);
            GUILIST_SetMaxSelectedItem(win_ctrl_id, node_num);
            GUILIST_SetOwnSofterKey(win_ctrl_id, TRUE);
#if defined MMI_BROWSER_MINI_SUPPORT || defined MMI_GUI_STYLE_MINISCREEN
            GUILIST_ChangeCheckListSoftkeyIndex(win_ctrl_id, MIDDLE_BUTTON);
            GUILIST_ChangeCheckListDefSoftkeyID(win_ctrl_id, STXT_CANCEL, STXT_MARK, STXT_RETURN);            
#endif
            for (index = 0; index < node_num; index++)
            {
                item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
                item_data.item_content[0].item_data.text_id = text_id_array[index];

#if defined MMI_BROWSER_MINI_SUPPORT || defined MMI_GUI_STYLE_MINISCREEN
                item_data.softkey_id[0] = TXT_DELETE;
                item_data.softkey_id[1] = STXT_MARK;
                item_data.softkey_id[2] = STXT_RETURN;
#endif

                item.user_data  = node_array[index];
                
                GUILIST_AppendItem(win_ctrl_id, &item);
                if (nv_setting_ptr->clear_set_array[node_array[index]])
                {
                    GUILIST_SetSelectedItem(win_ctrl_id, index, TRUE);
                }
            }

            GUILIST_SetCurItemIndex(win_ctrl_id, 0);
            MMK_SetAtvCtrl(win_id, win_ctrl_id);
        }
        break;

#ifdef MMI_BROWSER_MINI_SUPPORT
	case MSG_NOTIFY_LIST_SELECT_QUERY:
		{
			GUILIST_QRERY_SELECT_ITEM_T *item_it_ptr = PNULL;

			item_it_ptr = (GUILIST_QRERY_SELECT_ITEM_T*)param;
			
			if (PNULL != item_it_ptr)
			{
				if (MSG_APP_WEB == item_it_ptr->msg_id || MSG_TP_PRESS_UP == item_it_ptr->msg_id)
				{
					item_it_ptr->is_check_select = TRUE;
				}
			}
		}
		break;
#endif

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
#ifndef MMI_BROWSER_MINI_SUPPORT
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
                else if (ctrl_id == win_ctrl_id)
                {
                    uint16          current_index = 0;

                    current_index = GUILIST_GetCurItemIndex(ctrl_id);

                    if (GUILIST_GetItemState(GUILIST_GetItemPtrByIndex(ctrl_id, current_index), GUIITEM_STATE_SELECTED))
                    {
                        GUILIST_SetSelectedItem(ctrl_id, current_index, FALSE);
                    }
                    else
                    {
                        GUILIST_SetSelectedItem(ctrl_id, current_index, TRUE);
                    }

                    MMK_SendMsg(ctrl_id, MSG_CTL_PAINT, PNULL);
                }    
            }
        }
#endif
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
        {
            uint16                      checked_count = 0;
            uint16                      check_info_ptr[MMIBROWSER_CLEAR_TYPE_MAX] = {0};
            
            checked_count = GUILIST_GetSelectedItemIndex(win_ctrl_id, check_info_ptr, MMIBROWSER_CLEAR_TYPE_MAX);
            if (checked_count > 0)
            {
                MMIBROWSER_OpenQueryWinByTextId(TXT_DELETE, IMAGE_PUBWIN_QUERY, PNULL, PNULL);
                break;
            }
            else//NEWMS00200883
            {
                MMIBROWSER_NV_SETTING_T     *nv_setting_ptr = MMIBROWSER_GetNVSetting();
                SCI_MEMSET(nv_setting_ptr->clear_set_array, 0x00, sizeof(nv_setting_ptr->clear_set_array));
            }

            MMK_CloseWin(win_id);
        }        
        break;

    case MSG_PROMPTWIN_OK:
        {
            int32                       i = 0;
            uint16                      checked_count = 0;
            uint16                      check_info_ptr[MMIBROWSER_CLEAR_TYPE_MAX] = {0};
            MMIBROWSER_NV_SETTING_T     *nv_setting_ptr = MMIBROWSER_GetNVSetting();
            MMIBROWSER_CLEAR_TYPE_E     select_clear_type = MMIBROWSER_CLEAR_TYPE_MAX;
                
            checked_count = GUILIST_GetSelectedItemIndex(win_ctrl_id, check_info_ptr, MMIBROWSER_CLEAR_TYPE_MAX);

            SCI_MEMSET(nv_setting_ptr->clear_set_array, 0x00, sizeof(nv_setting_ptr->clear_set_array));//reset

            if (checked_count > 0)
            {
                for (i = 0; i < checked_count; i++)
                {
                    GUILIST_GetItemData(win_ctrl_id, check_info_ptr[i], &select_clear_type);
#ifndef MMI_BROWSER_MINI_SUPPORT
                    if(select_clear_type < MMIBROWSER_CLEAR_TYPE_MAX)
                    {
                        nv_setting_ptr->clear_set_array[select_clear_type] = TRUE;//save the setting param
                    }
#endif
                    switch (select_clear_type)
                    {
                    case MMIBROWSER_CLEAR_TYPE_HISTORY:
                        MMIBROWSER_DeleteAllHistory();
                        if (MMK_IsOpenWin(MMIBROWSER_HISTORYLIST_WIN_ID))
                        {
                            MMIBROWSER_UpdateHistoryList(MMIBROWSER_HISTORYLIST_WIN_ID);
                        }
                        break;

#ifdef MMIDORADO_INPUT_HISTORY_SUPPORT
                    case MMIBROWSER_CLEAR_TYPE_INPUT_HISTORY:
                        BrwDeleteInputHis();
                        break;
#endif

#ifdef BROWSER_SEARCH_SUPPORT
                    case MMIBROWSER_CLEAR_TYPE_KEY:
                        MMIBROWSER_CleanKeyInfo();
                        break;
#endif/*BROWSER_SEARCH_SUPPORT*/
                    
                    case MMIBROWSER_CLEAR_TYPE_CACHE:
                        BRW_ClearAllCache();
                        break;

                    case MMIBROWSER_CLEAR_TYPE_COOKIE:
                        BRW_ClearAllCookie();
                        break;

                    default:
                        break;
                    }
                }

                MMIBROWSER_CloseQuerytWin(PNULL);
                MMK_CloseWin(win_id);
#if defined (MMI_GUI_STYLE_TYPICAL) || defined (MMI_BROWSER_MINI_SUPPORT)
                MMIPUB_OpenAlertSuccessWin(TXT_BROWSER_PDA_CLEAR_SUCCESS);
#endif /* MMI_GUI_STYLE_TYPICAL */
            }
        }    
        break;
        
    case MSG_PROMPTWIN_CANCEL:
        MMIBROWSER_CloseQuerytWin(PNULL);
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
//  Description : handle Setting list msg
//  Global resource dependence : 
//  Author: Jiaoyouw.wu
//  Note: 
/*****************************************************************************/
LOCAL void BrwUpdateSettingList(MMI_CTRL_ID_T ctrl_id)
{
    uint32              sim_index = 0;
    MMI_STRING_T        string_1 = {0};
    MMI_STRING_T        string_2 = {0};
    uint32              select_index = 0;
    uint8               link_setting_num = 0;
    MMI_IMAGE_ID_T      image_id = IMAGE_NULL;
    uint16              current_index = 0;
    uint16              top_index = 0;
    wchar               temp_arr[MMIBROWSER_HOMEPAGE_MAX_URL_LEN + 1]= {0};
    MMI_TEXT_ID_T       sim_text_id[] = {TXT_COMMON_SIM1_NETWORK_ACCOUNT, TXT_COMMON_SIM2_NETWORK_ACCOUNT, TXT_BROWSER_SIM3, TXT_BROWSER_SIM4};
    //MMI_TEXT_ID_T       on_text_id[] = {TXT_COMMON_CLOSE, TXT_COMMON_OPEN};
#ifdef CSTAR_CHINESE_LIMITATION_SUPPORT
    MMI_TEXT_ID_T       font_text_arr[] = {TXT_FONT_SMALL, TXT_COMM_FONT_BIG};
#else
	MMI_TEXT_ID_T       font_text_arr[] = {TXT_FONT_SMALL, TXT_COMMON_NORMAL, TXT_COMM_FONT_BIG};
#endif
    MMI_TEXT_ID_T       lang_text_id[] = {TXT_EXPORT_LOC_DEFAULT, TXT_EXPORT_LOC_CUSTOM};
    SETTING_ITEM_FUNC   sim_setting_func[] = {BrwHandleSettingAccount1,
                                              BrwHandleSettingAccount2,
                                              BrwHandleSettingAccount3,
                                              BrwHandleSettingAccount4};

    MMICONNECTION_LINKSETTING_DETAIL_T  *link_setting_ptr = PNULL;
    MMIBROWSER_NV_SETTING_T             *nv_setting_ptr = PNULL;
    uint32  list_index = 0;

    current_index = GUILIST_GetCurItemIndex(ctrl_id);
    top_index = GUILIST_GetTopItemIndex(ctrl_id);
    GUILIST_RemoveAllItems(ctrl_id);

    /* User Account */
    MMIBROWSER_AppendSplitListItem(ctrl_id, TXT_ACCOUNT_SETTING);
    list_index++;

    nv_setting_ptr = MMIBROWSER_GetNVSetting();   

    for (sim_index = MN_DUAL_SYS_1; sim_index < MMI_DUAL_SYS_MAX; sim_index++)
    {
        /* Append list*/
        MMIRES_GetText(sim_text_id[sim_index], PNULL, &string_1);

        //LINK SETTING
        link_setting_num = MMIAPICONNECTION_GetLinkSettingNum((MN_DUAL_SYS_E)sim_index);

        select_index = nv_setting_ptr->sim_net_index[sim_index];

        link_setting_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex((MN_DUAL_SYS_E)sim_index,select_index);

        string_2.wstr_len = link_setting_ptr->name_len;
        string_2.wstr_ptr = (wchar *)link_setting_ptr->name;

        MMIBROWSER_AppendTwoTextOneImageItem(ctrl_id, &string_1, &string_2, IMAGE_COMMON_RIGHT_ARROW, (uint32)sim_setting_func[sim_index]);

        if  (!MMIAPIPHONE_IsSimAvailable(sim_index))
        {
            GUILIST_SetItemGray(ctrl_id, list_index, TRUE);
        }
        list_index++;
  }

#ifdef CMCC_UI_STYLE
#ifdef MMI_GPRS_DATA_SIM_SET
    if (MMIAPIPHONE_GetSimExistedNum(PNULL, 0) > 1)
    {
        MMIRES_GetText(TXT_CONNECTION_GPRS_MODE_SETTING, PNULL, &string_1);
        MMIBROWSER_AppendTwoTextOneImageItem(ctrl_id, &string_1, PNULL, IMAGE_COMMON_RIGHT_ARROW, (uint32)BrwHandleSetttingGPRSMode);
    }
#endif
#else
    /* Start Page */
    MMIBROWSER_AppendSplitListItem(ctrl_id, TXT_COMMON_HOMEPAGE_SETTING);

    MMIRES_GetText(TXT_BROWSER_SET_HOMEPAGE_TYPE, PNULL, &string_1);
    MMIRES_GetText(lang_text_id[nv_setting_ptr->homepage_mode], PNULL, &string_2);
    MMIBROWSER_AppendTwoTextOneImageItem(ctrl_id, &string_1, &string_2, IMAGE_COMMON_RIGHT_ARROW, (uint32)BrwHandleSetttingHomageType);

    MMIRES_GetText(TXT_BROWSER_CUSTOM_HOMEPAGE, PNULL, &string_1);
    MMIAPICOM_StrToWstr(MMIBROWSER_GetHomepageSetting(), temp_arr);
    string_2.wstr_ptr =  temp_arr;
    string_2.wstr_len =  MMIAPICOM_Wstrlen(temp_arr);
    MMIBROWSER_AppendTwoTextOneImageItem(ctrl_id, &string_1, &string_2, IMAGE_COMMON_RIGHT_ARROW, (uint32)BrwHandleSetttingHomageUrl);
#endif

    /* Image */
    MMIBROWSER_AppendSplitListItem(ctrl_id, TXT_BROWSER_CONTENT_SETTING);

    image_id = nv_setting_ptr->is_image_loaded ? IMAGE_CHECK_SELECTED_ICON : IMAGE_CHECK_UNSELECTED_ICON;
#ifdef CMCC_UI_STYLE
    MMIRES_GetText(TXT_DISPLAY_IMAGE, PNULL, &string_1);
#else
    MMIRES_GetText(TXT_BROWSER_PIC_DOWNLOAD, PNULL, &string_1);
#endif
    //MMIRES_GetText(on_text_id[nv_setting_ptr->is_image_loaded], PNULL, &string_2);
    MMIBROWSER_AppendTwoTextOneImageItem(ctrl_id, &string_1, PNULL, image_id, (uint32)BrwHandleSetttingDownloadImage);

#ifdef MMIDORADO_BGSOUND_SUPPORT
    image_id = nv_setting_ptr->is_sound_played ? IMAGE_CHECK_SELECTED_ICON : IMAGE_CHECK_UNSELECTED_ICON;
    MMIRES_GetText(TXT_WWW_BGMUSIC, PNULL, &string_1);
    //MMIRES_GetText(on_text_id[nv_setting_ptr->is_sound_played], PNULL, &string_2);
    MMIBROWSER_AppendTwoTextOneImageItem(ctrl_id, &string_1, PNULL, image_id, (uint32)BrwHandleSetttingBgsound);
#endif

#ifdef MMIDORADO_GOOGLE_TRANSCODE_SUPPORT
    image_id = nv_setting_ptr->is_google_change_enable ? IMAGE_CHECK_SELECTED_ICON : IMAGE_CHECK_UNSELECTED_ICON;
    MMIRES_GetText(TXT_BROWSER_GOOGLE, PNULL, &string_1);
    //MMIRES_GetText(on_text_id[nv_setting_ptr->is_google_change_enable], PNULL, &string_2);
    MMIBROWSER_AppendTwoTextOneImageItem(ctrl_id, &string_1, PNULL, image_id, (uint32)BrwHandleSetttingGoogleTranscode);
#endif

    MMIRES_GetText(TXT_COMMON_FONT_SIZE, PNULL, &string_1);
    MMIRES_GetText(font_text_arr[nv_setting_ptr->font], PNULL, &string_2);
    MMIBROWSER_AppendTwoTextOneImageItem(ctrl_id, &string_1, &string_2, IMAGE_COMMON_RIGHT_ARROW, (uint32)BrwHandleSetttingFont);

    MMIRES_GetText(TXT_BROWSER_PDA_CLEAR, PNULL, &string_1);
    MMIBROWSER_AppendTwoTextOneImageItem(ctrl_id, &string_1, PNULL, IMAGE_COMMON_RIGHT_ARROW, (uint32)BrwHandleSetttingClear);

#ifdef MMI_SSL_CERT_SUPPORT
	/*orange root certification*/
	MMIRES_GetText(TXT_BROWSER_CERTICIFATES, PNULL, &string_1);
    MMIBROWSER_AppendTwoTextOneImageItem(ctrl_id, &string_1, PNULL, IMAGE_NULL, (uint32)BrwHandleSetttingCertificates);
#endif

    /* Default Setting */
    MMIBROWSER_AppendSplitListItem(ctrl_id, TXT_RESET_DEFAULT);

    MMIRES_GetText(TXT_RESET_DEFAULT, PNULL, &string_1);
    MMIBROWSER_AppendTwoTextOneImageItem(ctrl_id, &string_1, PNULL, IMAGE_NULL, (uint32)BrwHandleSetttingReset);

    GUILIST_SetCurItemIndex(ctrl_id, current_index);
    GUILIST_SetTopItemIndex(ctrl_id, top_index);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyouw.wu
//  Note: 
/*****************************************************************************/
LOCAL void BrwHandleSetttingHomageType(void)
{
    MMK_CreatePubListWin((uint32 *)MMIBROWSER_SETTING_HOMEPAGE_TYPE_OPT_WIN_TAB, PNULL);
}

#ifdef MMI_GPRS_DATA_SIM_SET
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyouw.wu
//  Note: 
/*****************************************************************************/
LOCAL void BrwHandleSetttingGPRSMode(void)
{
    MMIAPISET_OpenGPRSModeSetting();
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyouw.wu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN BrwIsSimChange(void)
{
    MN_DUAL_SYS_E               dual_sys = MN_DUAL_SYS_1;
    MMISET_GPRS_MODE_TYPE_E     mode = DATA_CARD_MODE_1;
    BOOLEAN                     ret = FALSE;

    mode = MMIAPISET_GetGPRSModeType();

    if (DATA_CARD_MANUAL_SELECT != mode)
    {
        dual_sys = MN_DUAL_SYS_1 + (mode - DATA_CARD_MODE_1);/*lint !e656*/

        if (dual_sys != MMIBROWSER_GetSim())
        {
            ret = TRUE;
        }
    }

    SCI_TRACE_LOW("BrwIsSimChange ret = %x", ret);

    return ret;
}

#endif

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyouw.wu
//  Note: 
/*****************************************************************************/
LOCAL void BrwHandleSetttingHomageUrl(void)
{
    MMK_CreatePubEditWin((uint32 *)MMIBROWSER_SETTING_HOMEPAGE_WIN_TAB, PNULL);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyouw.wu
//  Note: 
/*****************************************************************************/
LOCAL void BrwHandleSetttingDownloadImage(void)
{
    MMIBROWSER_NV_SETTING_T     *nv_setting_ptr = PNULL;

    nv_setting_ptr = MMIBROWSER_GetNVSetting();

    nv_setting_ptr->is_image_loaded = !nv_setting_ptr->is_image_loaded;

    MMK_PostMsg(MMIBROWSER_SETTING_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
}

#ifdef MMIDORADO_BGSOUND_SUPPORT
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
LOCAL void BrwHandleSetttingBgsound(void)
{
    MMIBROWSER_NV_SETTING_T     *nv_setting_ptr = PNULL;

    nv_setting_ptr = MMIBROWSER_GetNVSetting();

    nv_setting_ptr->is_sound_played = !nv_setting_ptr->is_sound_played;

    MMK_PostMsg(MMIBROWSER_SETTING_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
}
#endif

#ifdef MMIDORADO_GOOGLE_TRANSCODE_SUPPORT
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyouw.wu
//  Note: 
/*****************************************************************************/
LOCAL void BrwHandleSetttingGoogleTranscode(void)
{
    MMIBROWSER_NV_SETTING_T     *nv_setting_ptr = PNULL;

    nv_setting_ptr = MMIBROWSER_GetNVSetting();

    nv_setting_ptr->is_google_change_enable = !nv_setting_ptr->is_google_change_enable;

    MMK_PostMsg(MMIBROWSER_SETTING_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
}
#endif

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyouw.wu
//  Note: 
/*****************************************************************************/
LOCAL void BrwHandleSetttingFont(void)
{
    MMK_CreatePubListWin((uint32 *)MMIBROWSER_SETTING_FONT_OPT_WIN_TAB, PNULL);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL void BrwHandleSetttingClear(void)
{
    MMK_CreatePubListWin((uint32 *)MMIBROWSER_SETTING_CLEAR_OPT_WIN_TAB, PNULL);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyouw.wu
//  Note: 
/*****************************************************************************/
LOCAL void BrwHandleSetttingReset(void)
{
    MMIBROWSER_OpenQueryWinByTextId(TXT_RESET_DEFAULT,IMAGE_PUBWIN_QUERY,PNULL,BrwHandleSetttingResetAlertWinMsg);
}

#ifdef MMI_SSL_CERT_SUPPORT
//start orange certification
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void BrwHandleSetttingCertificates(void)
{
    uint16 cert_num = 0;

    cert_num = MMIBRW_GetCertInfoNum();

    if (0 == cert_num)
    {
        MMIPUB_OpenAlertFailWin(TXT_EMPTY_LIST);
    } 
    else
    {
        MMK_CreateWin((uint32*)MMIBRW_CERTIFICATE_WIN_TAB, (ADD_DATA)cert_num);  
    }
      
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIBRW_OpenCertificateAttributeWin(void)
{
    MMK_CreateWin((uint32*)MMIBRW_CERTIFICATE_ATTRIBUTE_WIN_TAB, PNULL);    
}


/*****************************************************************************/
//  Description : add dynamic item to listbox when item_num is not zero
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void MMIBRW_AppendCertListItem( MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, uint16 item_num, MMI_TEXT_ID_T text1_id, MMI_TEXT_ID_T text2_id, MMI_TEXT_ID_T text3_id )
{
    uint16 i = 0;
    GUILIST_ITEM_T item_t = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    MMI_HANDLE_T ctrl_handle = MMK_GetCtrlHandleByWin(win_id, ctrl_id);
    
    if( item_num > 0)
    {
        uint16 cur_index = 0;
        uint16 top_index = 0;
        
        cur_index = GUILIST_GetCurItemIndex(ctrl_handle);
        top_index = GUILIST_GetTopItemIndex(ctrl_handle);
        if (GUILIST_GetTotalItemNum(ctrl_handle) > 0)
        {
            GUILIST_RemoveAllItems(ctrl_handle);
        }
        GUILIST_SetMaxItem( ctrl_handle, item_num, TRUE );
        
        item_t.item_style = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
        item_t.item_data_ptr = &item_data;
        
        for( i = 0; i < item_num; i++ )
        {
            GUILIST_AppendItem( ctrl_handle, &item_t );
        }
        GUILIST_SetTopItemIndex(ctrl_handle, top_index);
        GUILIST_SetCurItemIndex(ctrl_handle, cur_index);
    }
    else
    {
        GUILIST_SetMaxItem( ctrl_handle, item_num, FALSE );
        
        item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT;
        item_t.item_data_ptr = &item_data;
        
        item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[0].item_data.text_id = text1_id;
        
        item_data.softkey_id[0] = text2_id;
        item_data.softkey_id[1] = IMAGE_NULL;
        item_data.softkey_id[2] = text3_id;
        
        GUILIST_AppendItem( ctrl_handle, &item_t );  
    }
}

/*****************************************************************************/
//  Description : add history title item to listbox for www mdu 
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC void MMIBRW_AppendTitleItemData( MMI_CTRL_ID_T ctrl_id, uint16 index,  MMI_STRING_T string, MMI_IMAGE_ID_T image_id, MMI_TEXT_ID_T text1_id, MMI_TEXT_ID_T text2_id, MMI_TEXT_ID_T text3_id )
{
    GUILIST_ITEM_T item_t = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    uint16 str_len = string.wstr_len;
    
    item_t.item_style                        = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
    item_t.item_data_ptr                     = &item_data;
    item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    
    item_data.softkey_id[0] = text1_id;
    item_data.softkey_id[1] = text2_id;
    item_data.softkey_id[2] = text3_id;
    item_data.item_content[0].item_data.image_id = image_id;
    
    item_data.item_content[1].item_data.text_buffer.wstr_len = str_len;
    item_data.item_content[1].item_data.text_buffer.wstr_ptr = string.wstr_ptr;
    GUILIST_SetItemData( ctrl_id, &item_data, index );
} 


/*****************************************************************************/
//  Description :get num of root certificate
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL uint16 MMIBRW_GetCertInfoNum(void/*SSL_CERTINFO_T **cert_pptr*/)
{
    uint16 i = 1;
    SFS_HANDLE      file_handle = NULL;
#ifdef MMI_PRELOAD_CERT_SUPPORT
    wchar           full_path[MMIFMM_PATHNAME_LEN + 1] = L"C:\\SSL_CERT\\*.*";
#else    
    wchar           full_path[MMIFMM_PATHNAME_LEN + 1] = L"D:\\SSL_CERT\\*.*";
#endif
    SFS_FIND_DATA_T find_data = {0};

    file_handle = MMIAPIFMM_FindFirstFile(full_path, MMIAPICOM_Wstrlen(full_path), &find_data);
    
    if(SFS_INVALID_HANDLE != file_handle)
    {          
        while(SFS_ERROR_NONE == MMIAPIFMM_FindNextFile(file_handle, &find_data))
        {
            i++;
            //查找下一个文件
        }
    }
    else
    {
        return 0;
    }

    return i;
}


/*****************************************************************************/
//  Description :root certificate win
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBRWCertificateWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    uint32 cer_num = (uint32)MMK_GetWinAddDataPtr(win_id);/*lint !e64*/
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
//    MMIBROWSER_CERT_MODE_E mode = (uint32)MMK_GetWinAddDataPtr(win_id);/*lint !e64*/
    SSL_CERTINFO_T *ssl_cert_ptr = PNULL;
    BOOLEAN is_cert = FALSE;
    //SCI_TRACE_LOW:"mmiwww_wintab.c:HandleWWWCertificateWinMsg() msg_id=0x%04x"
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIBRW_AppendCertListItem( win_id, MMIBRW_LISTBOX_CERTIFICATE_CTRL_ID, cer_num, STXT_OK/*TXT_WWW_NO_HISTORY*/, 0, STXT_RETURN);
        MMK_SetAtvCtrl(win_id, MMK_GetCtrlHandleByWin(win_id, MMIBRW_LISTBOX_CERTIFICATE_CTRL_ID));
        break;
        
    case MSG_FULL_PAINT:
        break;
        
    case MSG_CTL_LIST_NEED_ITEM_DATA:
        {
            MMI_STRING_T            title_string_info = {0};
            GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;
            SFS_HANDLE              file_handle = NULL;
            SFS_FIND_DATA_T         find_data = {0};
#ifdef MMI_PRELOAD_CERT_SUPPORT            
            wchar                   full_path[MMIFMM_PATHNAME_LEN + 1] = L"C:\\SSL_CERT\\*.*";
#else
            wchar                   full_path[MMIFMM_PATHNAME_LEN + 1] = L"D:\\SSL_CERT\\*.*";
#endif
            uint16                  i = 0;
            wchar	                name_ptr[MMIFMM_FILE_FILE_NAME_MAX_LEN]	= {0};      //out, one of name_ptr and suffix_ptr can be NULL
            uint16	                name_len = MMIFMM_FILE_FILE_NAME_MAX_LEN;      //out, 双字节为单位
            wchar	                suffix_ptr[MMIBROWSER_MAX_STRING_INFO]	= {0};        //out
            uint16	                suffix_len = MMIFMM_FILE_FILE_NAME_MAX_LEN;     //in/out, 双字节为单位, in表示后缀名的最大长度	
 
            if (PNULL == need_item_data_ptr)
            {
                break;
            }
			
	        i = need_item_data_ptr->item_index;
            file_handle = MMIAPIFMM_FindFirstFile(full_path, MMIAPICOM_Wstrlen(full_path), &find_data);
            
            if(SFS_INVALID_HANDLE != file_handle)
            {          
                MMIFILE_ERROR_E sfs_error = SFS_ERROR_NONE;

                do
                {
                    if (0 < i)
                    {
                        i--;
                    }
                    else
                    {
                        MMIAPIFMM_SplitFileName(
                            find_data.name,     //in
                            MMIAPICOM_Wstrlen(find_data.name),      //in, 双字节为单位
                            name_ptr,          //out, one of name_ptr and suffix_ptr can be NULL
                            &name_len,      //out, 双字节为单位
                            suffix_ptr,        //out
                            &suffix_len     //in/out, 双字节为单位, in表示后缀名的最大长度
                            );

                        title_string_info.wstr_ptr = name_ptr;
                        title_string_info.wstr_len = name_len;
                        break;
                    }
                    
                    //查找下一个文件
                    sfs_error = MMIAPIFMM_FindNextFile(file_handle, &find_data);
                }while(SFS_ERROR_NONE == sfs_error);
                
                SFS_FindClose(file_handle);
            }

            MMIBRW_AppendTitleItemData( MMK_GetCtrlHandleByWin(win_id, MMIBRW_LISTBOX_CERTIFICATE_CTRL_ID), need_item_data_ptr->item_index, title_string_info, IMAGE_COMMON_SOFTKEY_URL_ICON/*IMAGE_WWW_CERT_ITEM*/, TXT_VIEW, 0, STXT_RETURN );

            SFS_FindClose(file_handle);
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
        MMIBRW_OpenCertificateAttributeWin();
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
//  Description :root certificate win
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCerAttributeDetailWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    
    //SCI_TRACE_LOW:"mmiwww_wintab.c:HandleCerAttributeDetailWinMsg() msg_id=0x%04x"
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIBRW_Append_Certificate_Attribute(win_id);
        MMK_SetAtvCtrl(win_id, MMK_GetCtrlHandleByWin(win_id, MMIBRW_RICHTEXT_CERTIFICATE_ATTRIBUTE_CTRL_ID));
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
        
    case MSG_CLOSE_WINDOW:
        if (SSL_DestroyAllCertInfo(&s_certInfo_ptr))
        {
            SCI_TRACE_LOW("MMIBRW: SSL_DestroyAllCertInfo");
        }
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : append certificate attribute to richtext
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
LOCAL void MMIBRW_Append_Certificate_Attribute( MMI_WIN_ID_T win_id )
{
    uint16 index = 0;
    uint16 str_len = 0;
    uint16 selected_item = 0;

    GUIRICHTEXT_ITEM_T  item_data = {0};/*lint !e64*/
    MMI_STRING_T        string_lab = {0};
    char                data_str[GUIRICHTEXT_TEXT_MAX_LEN + 1] = {0};
    char                date[MMISET_DATE_STR_LEN] = {0};
    char                time[MMISET_TIME_STR_12HOURS_LEN + 1] = {0};
    wchar               item_text[GUIRICHTEXT_TEXT_MAX_LEN + 1] = {0};
    BOOLEAN             is_cert = FALSE;
	SSL_CERTINFO_T      *current_certInfo_ptr = PNULL;
	uint32				i = 0;
    
    is_cert = SSL_GetAllCertInfo(&s_certInfo_ptr);

    if (!is_cert || PNULL == s_certInfo_ptr)
    {
        SCI_TRACE_LOW("[MMIBRW]:MMIBRW_Append_Certificate_Attribute");
        return;
    }
    
    item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
    item_data.text_type = GUIRICHTEXT_TEXT_BUF; 

    selected_item = GUILIST_GetCurItemIndex(MMIBRW_LISTBOX_CERTIFICATE_CTRL_ID);

	current_certInfo_ptr = s_certInfo_ptr;

	for (i = 0; i < selected_item; i++)
	{
		if (PNULL != current_certInfo_ptr->next)
		{
			current_certInfo_ptr = current_certInfo_ptr->next;
		}
	}

    MMI_GetLabelTextByLang(TXT_BROWSER_CERT_ISSUER, &string_lab);

    SCI_MEMSET(item_text, 0, sizeof(item_text));
    item_data.text_data.buf.len = MMIAPICOM_CopyStr2Buf(item_text, &string_lab, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = item_text;

    GUIRICHTEXT_AddItem(MMK_GetCtrlHandleByWin(win_id, MMIBRW_RICHTEXT_CERTIFICATE_ATTRIBUTE_CTRL_ID), &item_data, &index); 

    SCI_MEMSET(item_text, 0, sizeof(item_text));
    if(PNULL != current_certInfo_ptr->issuer.organization_ptr)
    {
        str_len = (uint16)strlen((char *)current_certInfo_ptr->issuer.organization_ptr);
        GUI_UTF8ToWstr(item_text, str_len, current_certInfo_ptr->issuer.organization_ptr, str_len);
        item_data.text_data.buf.len = str_len;
        item_data.text_data.buf.str_ptr = item_text;

        GUIRICHTEXT_AddItem(MMK_GetCtrlHandleByWin(win_id, MMIBRW_RICHTEXT_CERTIFICATE_ATTRIBUTE_CTRL_ID), &item_data, &index); 
    }

    MMI_GetLabelTextByLang(TXT_BROWSER_CERT_SUBJECT, &string_lab);

    SCI_MEMSET(item_text, 0, sizeof(item_text));
    item_data.text_data.buf.len = MMIAPICOM_CopyStr2Buf(item_text, &string_lab, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = item_text;

    GUIRICHTEXT_AddItem(MMK_GetCtrlHandleByWin(win_id, MMIBRW_RICHTEXT_CERTIFICATE_ATTRIBUTE_CTRL_ID), &item_data, &index); 

    SCI_MEMSET(item_text, 0, sizeof(item_text));

    if(PNULL != current_certInfo_ptr->subject.commoname_ptr)
    {
        str_len = (uint16)strlen((char *)current_certInfo_ptr->subject.commoname_ptr);
        GUI_UTF8ToWstr(item_text, str_len, current_certInfo_ptr->subject.commoname_ptr, str_len);
        item_data.text_data.buf.len = str_len;
        item_data.text_data.buf.str_ptr = item_text;

        GUIRICHTEXT_AddItem(MMK_GetCtrlHandleByWin(win_id, MMIBRW_RICHTEXT_CERTIFICATE_ATTRIBUTE_CTRL_ID), &item_data, &index); 
    }

    MMI_GetLabelTextByLang(TXT_BROWSER_CERT_VALID_AFTER, &string_lab);

    SCI_MEMSET(item_text, 0, sizeof(item_text));
    item_data.text_data.buf.len = MMIAPICOM_CopyStr2Buf(item_text, &string_lab, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = item_text;

    GUIRICHTEXT_AddItem(MMK_GetCtrlHandleByWin(win_id, MMIBRW_RICHTEXT_CERTIFICATE_ATTRIBUTE_CTRL_ID), &item_data, &index); 

    SCI_MEMSET(item_text, 0, sizeof(item_text));

    if(PNULL != current_certInfo_ptr->not_before_ptr)
    {
        uint32 start_time = atoi(current_certInfo_ptr->not_before_ptr);
        MMI_TM_T TM_begin = MMIAPICOM_Second2Tm(start_time);
        char valid_begin_time[MMIBROWSER_MAX_STRING_INFO] = {0};
        
        sprintf((char*)valid_begin_time, "%02d.%02d.%02d %02d:%02d:%02d", TM_begin.tm_year, TM_begin.tm_mon, TM_begin.tm_mday, TM_begin.tm_hour, TM_begin.tm_min, TM_begin.tm_sec);

        str_len = (uint16)strlen((char *)valid_begin_time);
        GUI_UTF8ToWstr(item_text, str_len, valid_begin_time, str_len);
        item_data.text_data.buf.len = str_len;
        item_data.text_data.buf.str_ptr = item_text;

        GUIRICHTEXT_AddItem(MMK_GetCtrlHandleByWin(win_id, MMIBRW_RICHTEXT_CERTIFICATE_ATTRIBUTE_CTRL_ID), &item_data, &index); 
    }

    MMI_GetLabelTextByLang(TXT_BROWSER_CERT_VALID_UNTIL, &string_lab);

    SCI_MEMSET(item_text, 0, sizeof(item_text));
    item_data.text_data.buf.len = MMIAPICOM_CopyStr2Buf(item_text, &string_lab, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = item_text;

    GUIRICHTEXT_AddItem(MMK_GetCtrlHandleByWin(win_id, MMIBRW_RICHTEXT_CERTIFICATE_ATTRIBUTE_CTRL_ID), &item_data, &index); 

    SCI_MEMSET(item_text, 0, sizeof(item_text));
    if(PNULL != current_certInfo_ptr->not_after_ptr)
    {
        uint32 start_time = atoi(current_certInfo_ptr->not_after_ptr);
        MMI_TM_T TM_begin = MMIAPICOM_Second2Tm(start_time);
        char valid_end_time[MMIBROWSER_MAX_STRING_INFO] = {0};

        sprintf((char*)valid_end_time, "%02d.%02d.%02d %02d:%02d:%02d", TM_begin.tm_year, TM_begin.tm_mon, TM_begin.tm_mday, TM_begin.tm_hour, TM_begin.tm_min, TM_begin.tm_sec);

        str_len = (uint16)strlen((char *)valid_end_time);
        GUI_UTF8ToWstr(item_text, str_len, valid_end_time, str_len);
        item_data.text_data.buf.len = str_len;
        item_data.text_data.buf.str_ptr = item_text;

        GUIRICHTEXT_AddItem(MMK_GetCtrlHandleByWin(win_id, MMIBRW_RICHTEXT_CERTIFICATE_ATTRIBUTE_CTRL_ID), &item_data, &index); 
    }

    MMI_GetLabelTextByLang(TXT_BROWSER_CERT_FINGER_PRINT, &string_lab);

    SCI_MEMSET(item_text, 0, sizeof(item_text));
    item_data.text_data.buf.len = MMIAPICOM_CopyStr2Buf(item_text, &string_lab, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = item_text;

    GUIRICHTEXT_AddItem(MMK_GetCtrlHandleByWin(win_id, MMIBRW_RICHTEXT_CERTIFICATE_ATTRIBUTE_CTRL_ID), &item_data, &index); 


//    if(PNULL != s_certInfo_ptr->sig_hash)
    {
        char finger_str[GUIRICHTEXT_TEXT_MAX_LEN + 1] = {0};
        char fingerprint_str[GUIRICHTEXT_TEXT_MAX_LEN + 1] = {0};
        uint16 i = 0;
        uint16 j = 0;
        uint16 finger_len = 0;

        str_len = (uint16)strlen((char *)current_certInfo_ptr->sig_hash);
        MMIAPICOM_BinaryToHexChar(current_certInfo_ptr->sig_hash, str_len, finger_str, &finger_len);

        //不能出现奇数情况
        for (i = 0; i < finger_len; i = i + MMIBROWSER_SSL_HASH_OFFSET)
        {
            fingerprint_str[j * MMIBROWSER_SIG_HASH_OFFSET] = finger_str[i];
            fingerprint_str[j * MMIBROWSER_SIG_HASH_OFFSET + 1] = finger_str[i + 1];

            if (i <  (finger_len - MMIBROWSER_SSL_HASH_OFFSET))
            {
                fingerprint_str[j * MMIBROWSER_SIG_HASH_OFFSET + MMIBROWSER_SSL_HASH_OFFSET] = MMIBROWSER_CHAR_COLON;
            }
            j++;
        }

        finger_len = (finger_len / MMIBROWSER_SSL_HASH_OFFSET - 1 + finger_len) * MMIBROWSER_SSL_HASH_OFFSET;

        GUI_UTF8ToWstr(item_text, GUIRICHTEXT_TEXT_MAX_LEN , fingerprint_str, finger_len);

        item_data.text_data.buf.len = finger_len;
        item_data.text_data.buf.str_ptr = item_text;
        
        GUIRICHTEXT_AddItem(MMK_GetCtrlHandleByWin(win_id, MMIBRW_RICHTEXT_CERTIFICATE_ATTRIBUTE_CTRL_ID), &item_data, &index); 
    }
//    MMIWWW_Netfront_DeleteX509CertData(s_certInfo_ptr);
}

//end orange certification
#endif
/*****************************************************************************/
// Description : Handle query change service
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleSetttingResetAlertWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;   
    BOOLEAN is_update_font = FALSE;

    //SCI_TRACE_LOW:"BrwHandleSetttingResetAlertWinMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_SETTING_1753_112_2_18_2_6_44_323,(uint8*)"d", msg_id);
 
    switch(msg_id)
    {
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        {
#ifdef MMI_GUI_STYLE_TYPICAL
            MMIPUB_OpenAlertSuccessWin(TXT_SUCCESS);
#endif /* MMI_GUI_STYLE_TYPICAL */

            //todo 恢复到默认配置值
            MMIBROWSER_SetSettingChange(MMIBROWSER_IsSimIndexModifyByDefault(MMIBROWSER_GetSim()));
            is_update_font = MMIBROWSER_IsFontModifyByDefault();
            MMIDORADO_ResetFactorySetting();
            BrwExecuteSetting(is_update_font);
        
            s_is_has_reset = TRUE;
            
            MMK_CloseWin(win_id);
            MMK_CloseWin(MMIBROWSER_SETTING_WIN_ID);
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

#endif /* MMI_GUI_STYLE_TYPICAL */
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif


/*Edit by script, ignore 7 case. Fri Apr 27 09:38:48 2012*/ //IGNORE9527
