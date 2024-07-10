/*****************************************************************************
** File Name:      mmiwifi_win.c                                             *
** Author:         li.li                                                     *
** Date:           2009.11.18                                                *
** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2009.11.18     li.li            Create                                    *
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_wlan_trc.h"
#include <string.h>
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
#include "watch_commonwin_export.h"
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
#include "guires.h"
#include "guictrl_api.h"
#include "guiform.h"
#include "guitext.h"
#include "mmipdp_export.h"
#include "tcpip_api.h"
#include "guitab.h"
#include "mmiwlan_internal.h"
#include "mmisms_image.h"
#include "in_message.h"
#include "mmiset_text.h"
#include "mmi_resource.h"
#ifdef DL_SUPPORT
#include "mmidl_export.h"
#endif
#include "gui_ucs2b_converter.h"
#include "mmiphone_export.h"
#include "mmiwlan_portal.h"
#include "mmiudisk_export.h"
#include "mmistk_text.h"
#include "mmiidle_statusbar.h"
#include "mmicom_trace.h"

#include "mmisrvwifi.h"
#include "mmieng_export.h"

#if defined(WIFI_VENDOR_CSR)
#ifndef WIN32
extern void CsrWifiSupplicantInitialize(unsigned char enableStrictMode);
#endif
#endif

/**-------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMIWIFI_MAIN_LIST_NUM       2
#define MMIWIFI_MAIN_MEUN_NUM       2
#define MMIWIFI_SETTING_MENU_NUM    7
#define MMIWIFI_NETWORK_STATE_MAX   2
#define MMIWIFI_LIST_PERMANENT_ITEM_NUM     (2)
//只通过ASCII认证
//#define MMI_WLAN_AUTHORITY_COMPLEX
//#define MMI_WLAN_TAB_STYLE
/*#if 0*/
#define MMIWIFI_WEPAUTH_SETTING_LABEL_LEFT   10
#ifdef MAINLCD_SIZE_128X160
#define MMIWIFI_WEPAUTH_SETTING_LABEL_RIGHT  100
#define MMIWIFI_WEPAUTH_SETTING_LABEL_TOP    20
#define MMIWIFI_WEPAUTH_SETTING_LABEL_HIGHT  30
#ifdef MAINLCD_SIZE_176X220
#define MMIWIFI_WEPAUTH_SETTING_LABEL_RIGHT  156
#define MMIWIFI_WEPAUTH_SETTING_LABEL_TOP    20
#define MMIWIFI_WEPAUTH_SETTING_LABEL_HIGHT  30
#else
#define MMIWIFI_WEPAUTH_SETTING_LABEL_RIGHT  220
#define MMIWIFI_WEPAUTH_SETTING_LABEL_TOP    30
#define MMIWIFI_WEPAUTH_SETTING_LABEL_HIGHT  30
#endif
#endif

#define MMIWIFI_WEP_INDEX_NUM               4
#define MMIWIFI_WEP_BITSTYLE_NUM            2
#define MMIWIFI_WEP_KEYSTYLE_NUM            2
#define MMIWIFI_WEP_BIT_NUM                 2
#define MMIWIFI_WEP_KEY_MODE_NUM            2
#define MMIWIFI_IP_SETING_MAX               2
#define MMIWIFI_WPAAUTH_MAX                 2
#define MMIWIFI_WPAEAP_MAX                  7
#define MMIWIFI_WPA_EAP_TTLS_MAX            4
#define MMIWIFI_SAVE_SETTING_MAX            2
#define MMIWIFI_WPA_EAP_PEAP_MAX            2
#define MMIWIFI_AUTH_TYPE_MAX               4
#define MMIWIFI_STATIC_IP_MEUN_NUM          5
#define MMIWIFI_SET_MENU_NUM                7
#define MMIWIFI_WPA_EAP_MEUN_NUM            2
#define MMIWIFI_WEP_BIT_SETTING_MAX         2
#define MMIWIFI_WEP_64BIT_KEY_LEN           (WIFISUPP_WEP_64BITS_KEY_LEN - WIFISUPP_WEP_IV_LEN)
#define MMIWIFI_WEP_128BIT_KEY_LEN          (WIFISUPP_WEP_128BITS_KEY_LEN - WIFISUPP_WEP_IV_LEN)
#define MMIWIFI_RSSI_0                      -95  //从-90修改为-95,其他级别暂时不变
#define MMIWIFI_RSSI_100                    -10
#define MMIWIFI_RSSI_20                     -85//(MMIWIFI_RSSI_0 + (MMIWIFI_RSSI_100 - MMIWIFI_RSSI_0)/5)
#define MMIWIFI_RSSI_40                     -75//(MMIWIFI_RSSI_0 + (MMIWIFI_RSSI_100 - MMIWIFI_RSSI_0)*2/5)
#define MMIWIFI_RSSI_60                     -67//(MMIWIFI_RSSI_0 + (MMIWIFI_RSSI_100 - MMIWIFI_RSSI_0)*3/5)
#define MMIWIFI_RSSI_80                     -55//(MMIWIFI_RSSI_0 + (MMIWIFI_RSSI_100 - MMIWIFI_RSSI_0)*4/5)
#define MMIWIFI_DETAIL_TEMP_LEN             40
#define MMIWIFI_BIT64_PSW                   "**********"
#define MMIWIFI_BIT128_PSW                  "**************************"
#define MMIWIFI_DEF_AP_SETTING_MENU_NUM    ((uint32)MMIWIFI_DEF_AP_SETTING_NODE_AUTO_CONNECTING - (uint32)MMIWIFI_DEF_AP_SETTING_NODE_ACTIVE + 1)
#define MMIWIFI_CERTIFICATE_FILE_SIZE_MAX   (4 * 1024)
#define MMIWIFI_FIND_AP_NOTFY_TEXT_MAX   (130)
#define MMIWIFI_MAX_PORT_NUM    (0xFFFF)

/**-------------------------------------------------------------------------*
**                         ENUM DEFINITION                                  *
**--------------------------------------------------------------------------*/
typedef enum
{
    MMIWLAN_TRUST_OPT_ID_EDIT = 1,
    MMIWLAN_TRUST_OPT_ID_ADD,
    MMIWLAN_TRUST_OPT_ID_CHANGE_PRIORITY_HIGH,
    MMIWLAN_TRUST_OPT_ID_CHANGE_PRIORITY_LOW,
    MMIWLAN_TRUST_OPT_ID_DELETE,

    MMIWLAN_TRUST_OPT_ID_MAX
}MMIWLAN_TRUST_OPT_E;

typedef enum
{
    MMIWLAN_LIST_OPT_ID_REFRESH = 0,
    MMIWLAN_LIST_OPT_ID_SPRCIFIED_SEARCH,
    MMIWLAN_LIST_OPT_ID_DETAILS,
    MMIWLAN_LIST_OPT_ID_DEFAULT_AP_SETTING,

    MMIWLAN_LIST_OPT_ID_MAX
}MMIWLAN_LIST_OPT_E;

typedef enum
{
    MMIWLAN_AUTOSCAN_OPT_OFF = 1,
    MMIWLAN_AUTOSCAN_OPT_1_MIN,
    MMIWLAN_AUTOSCAN_OPT_2_MIN,
    MMIWLAN_AUTOSCAN_OPT_5_MIN,
    MMIWLAN_AUTOSCAN_OPT_10_MIN,

    MMIWLAN_AUTOSCAN_OPT_MAX
}MMIWLAN_AUTOSCAN_OPT_E;

typedef enum
{
    MMIWLAN_GPRSTOWLAN_OPT_ALWAYS_INQUIRY = 1,
    MMIWLAN_GPRSTOWLAN_OPT_AUTO_RESELCT,
    MMIWLAN_GPRSTOWLAN_OPT_MANUAL_RESELCT,

    MMIWLAN_GPRSTOWLAN_OPT_MAX
}MMIWLAN_GPRSTOWLAN_OPT_E;

typedef enum
{
    MMIWLAN_RESELECT_AP_OPT_AUTO = 1,
    MMIWLAN_RESELECT_AP_OPT_MANUALT,

    MMIWLAN_RESELECT_AP_OPT_MAX
}MMIWLAN_RESELECT_AP_OPT_E;

typedef enum
{
    MMIWLAN_PROMPT_NO_WLAN_OPT_PROMPT = 1,
    MMIWLAN_PROMPT_NO_WLAN_OPT_NOT_PROMPT,

    MMIWLAN_PROMPT_NO_WLAN_OPT_MAX
}MMIWLAN_PROMPT_NO_WLAN_OPT_E;

typedef enum
{
    MMIWIFI_REJECT_LIST_POPUP_EDIT = 1,
    MMIWIFI_REJECT_LIST_POPUP_DELETE,

    MMIWIFI_REJECT_LIST_POPUP_MAX
}MMIWIFI_REJECT_LIST_POPUP_E;

typedef enum
{
    MMIWIFI_AP_LIST_POPUP_CONNECT = 1,
    MMIWIFI_AP_LIST_POPUP_FORGET_NETWORK,
    MMIWIFI_AP_LIST_POPUP_EDIT_NETWORK,
    MMIWIFI_AP_LIST_POPUP_DEFAULT_SETTING,

    MMIWIFI_AP_LIST_POPUP_MAX
}MMIWIFI_AP_LIST_POPUP_E;

typedef enum
{
    MMIWIFI_WPA_STATE_KEY_HEX = 1,
    MMIWIFI_WPA_STATE_KEY_ASCII,

    MMIWIFI_WPA_STATE_KEY_MAX
}MMIWIFI_WPA_STATE_KEY_E;

typedef enum
{
    MMIWIFI_AP_NETWORK_STATE_OPEN = 1,
    MMIWIFI_AP_NETWORK_STATE_HIDE,

    MMIWIFI_AP_NETWORK_STATE_MAX
}MMIWIFI_AP_NETWORK_STATE_E;

typedef enum
{
    MMIWIFI_AP_AUTH_TYPE_OPEN = 1,
    MMIWIFI_AP_AUTH_TYPE_WEP,
    MMIWIFI_AP_AUTH_TYPE_WPA,
    MMIWIFI_AP_AUTH_TYPE_WPA2,
    MMIWIFI_AP_AUTH_TYPE_WAPI,
#ifdef WLAN_EAP_SUPPORT
    MMIWIFI_AP_AUTH_TYPE_EAP,
#endif /* WLAN_EAP_SUPPORT */

    MMIWIFI_AP_AUTH_TYPE_MAX
}MMIWIFI_AP_AUTH_TYPE_E;

#ifdef WLAN_EAP_SUPPORT
typedef enum
{
    MMIWIFI_EAP_SIM_INDEX_1 = 1,
    MMIWIFI_EAP_SIM_INDEX_2,
    MMIWIFI_EAP_SIM_INDEX_3,
    MMIWIFI_EAP_SIM_INDEX_4,

    MMIWIFI_EAP_SIM_INDEX_MAX
}MMIWIFI_EAP_SIM_INDEX_E;

typedef enum
{
    MMIWIFI_EAP_TYPE_SIM = 1,
    MMIWIFI_EAP_TYPE_ACCOUNT,

    MMIWIFI_EAP_TYPE_MAX
}MMIWIFI_EAP_TYPE_E;

#endif /* WLAN_EAP_SUPPORT */

typedef enum
{
    MMIWIFI_AP_CONNECT_STATE_OPEN = 1,
    MMIWIFI_AP_CONNECT_STATE_HIDE,

    MMIWIFI_AP_CONNECT_STATE_MAX
}MMIWIFI_AP_CONNECT_STATE_E;

typedef enum
{
    MMIWIFI_WAPI_STATE_KEY_HEX = 1,
    MMIWIFI_WAPI_STATE_KEY_ASCII,

    MMIWIFI_WAPI_STATE_KEY_MAX
}MMIWIFI_WAPI_STATE_KEY_E;

typedef enum
{
    MMIWIFI_WAPI_TYPE_PSK = 1,
    MMIWIFI_WAPI_TYPE_CER,

    MMIWIFI_WAPI_TYPE_MAX
}MMIWIFI_WAPI_TYPE_E;

typedef enum
{
    MMIWIFI_WEP_STATE_KEY_HEX = 1,
    MMIWIFI_WEP_STATE_KEY_ASCII,

    MMIWIFI_WEP_STATE_KEY_MAX
}MMIWIFI_WEP_STATE_KEY_E;

typedef enum
{
    MMIWIFI_WEP_ENCYPT_BIT64 = 1,
    MMIWIFI_WEP_ENCYPT_BIT128,

    MMIWIFI_WEP_ENCYPT_MAX
}MMIWIFI_WEP_ENCYPT_E;

typedef enum
{
    MMIWIFI_WEP_MODE_SHARE = 1,
    MMIWIFI_WEP_MODE_OPEN,

    MMIWIFI_WEP_MODE_MAX
}MMIWIFI_WEP_MODE_E;

typedef enum
{
    MMIWIFI_WEP_KEY_INDEX_1 = 1,
    MMIWIFI_WEP_KEY_INDEX_2,
    MMIWIFI_WEP_KEY_INDEX_3,
    MMIWIFI_WEP_KEY_INDEX_4,

    MMIWIFI_WEP_KEY_INDEX_MAX
}MMIWIFI_WEP_KEY_INDEX_E;

typedef void (*SETTING_ITEM_FUNC)(void);

typedef enum
{
    MMIWLAN_LIST_ITEM_ON = 1,
    MMIWLAN_LIST_ITEM_AP,
    MMIWLAN_LIST_ITEM_ADD_AP,

    MMIWLAN_LIST_ITEM_MAX
}MMIWLAN_LIST_ITEM_E;


/**--------------------------------------------------------------------------*
 **                          EXTERN DECLARE                                *
 **--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         TYPE DEFINITION                                  *
**--------------------------------------------------------------------------*/
typedef BOOLEAN (* WIFI_FILTER_CALLBACK)(MMIWIFI_PROFILE_T* profile_ptr);

/**-------------------------------------------------------------------------*
**                         GLOBAL VARIABLE DEFINITION                       *
**--------------------------------------------------------------------------*/
MMI_APPLICATION_T    g_mmiwifi_app = {0}; //lint -e552
LOCAL MMIWIFI_UI_CONTEXT_T s_ui_context = {0};
#ifdef WLAN_RESELECT_SUPPORT
LOCAL int64 s_last_tick_time = 0;
#endif
#ifndef MMI_GUI_STYLE_TYPICAL
LOCAL BOOLEAN s_ip_type = TRUE;
LOCAL BOOLEAN s_is_use_proxy = TRUE;
#endif /* MMI_GUI_STYLE_TYPICAL */
/**-------------------------------------------------------------------------*
**                         STATIC CLASS DEFINITION                          *
**--------------------------------------------------------------------------*/
/**-------------------------------------------------------------------------*
**                         LOCAL FUNCTION DECLARE                           *
**--------------------------------------------------------------------------*/

#ifdef MMI_WLAN_TAB_STYLE
/****************************************************************************/
//  Description : handle wifi main window
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleMainParentWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/****************************************************************************/
//  Description : handle wifi trusted list window
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleTrustedlistChildWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/****************************************************************************/
//  Description : Set Main Property
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  MMIWIFI_SetMainTabProperty(MMI_CTRL_ID_T ctrl_id);

/****************************************************************************/
//  Description : WifiUpdateTrustedList
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiUpdateTrustedList(MMI_CTRL_ID_T ctrl_id);

/****************************************************************************/
//  Description : WifiUpdateListTravCallback
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL BOOLEAN WifiUpdateTrustedListTravCallback(MMIWIFI_PROFILE_T *profile_ptr, void *find_data);

/****************************************************************************/
//  Description : WifiAppendListItem
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void WifiAppendTrustedListItem(MMI_CTRL_ID_T ctrl_id, MMIWIFI_PROFILE_T *profile_ptr);

/****************************************************************************/
//  Description : WifiHandleTrustedlistPopMenuWinMsg
//  Global resource dependence : 
//  Author:
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleTrustedlistPopMenuWinMsg(
                                                MMI_WIN_ID_T    win_id, 
                                                MMI_MESSAGE_ID_E   msg_id, 
                                                DPARAM             param
                                                );

/*****************************************************************************/
//  Discription: WifiCreateTrustedListPopMenu
//  Global resource dependence: None
//  Author: 
//  Note : 
//  Return: 
/*****************************************************************************/
LOCAL void WifiCreateTrustedListPopMenu(
                            MMI_WIN_ID_T    win_id, //IN: list menu's window id
                            MMI_CTRL_ID_T   ctrl_id
                            );

#endif /*MMI_WLAN_TAB_STYLE*/

/****************************************************************************/
//  Description : open wifi main window
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_OpenMainWin(MMIWIFI_ENTRY_TYPE_E entry_type);

/****************************************************************************/
//  Description : open default access point setting window
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void MMIWIFI_OpenDefAPSettingWin(void);

/*****************************************************************************/
//  Discription: WifiCreateListPopMenu
//  Global resource dependence: None
//  Author: 
//  Note : 
//  Return: 
/*****************************************************************************/
LOCAL void WifiCreateListPopMenu(
                            MMI_WIN_ID_T    win_id, //IN: list menu's window id
                            MMI_CTRL_ID_T   ctrl_id
                            );

/*****************************************************************************/
//  Discription: Handle searching new device waiting win msg
//  Global resource dependence: none 
//  Author: li.li 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleCancelScanWaitWinMsg(
                                                    MMI_WIN_ID_T win_id, 
                                                    MMI_MESSAGE_ID_E msg_id, 
                                                    DPARAM param 
                                                    );

/****************************************************************************/
//  Description : handle wifi list window
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleListWinMsg(
                                            MMI_WIN_ID_T    win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param
                                            );

/*****************************************************************************
 Prototype    : WifiUpdateList
 Description  : 刷新List窗口
 Input        : void  
 Output       : None
 Return Value : LOCAL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2010/8/31
    Author       : George.Liu 01526
    Modification : Created function

*****************************************************************************/
LOCAL void WifiUpdateList(MMI_CTRL_ID_T ctrl_id);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiUpdateSelectAPList(MMI_CTRL_ID_T ctrl_id);

/****************************************************************************/
//  Description : handle wifi list popmenu window
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandlelistPopMenuWinMsg(
                                            MMI_WIN_ID_T    win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param
                                            );

/*****************************************************************************/
//  Discription: Handle cancel connect waiting win msg
//  Global resource dependence: none 
//  Author: li.li 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleCancelConnectWaitWinMsg(
                                                    MMI_WIN_ID_T win_id, 
                                                    MMI_MESSAGE_ID_E msg_id, 
                                                    DPARAM param 
                                                    );

/****************************************************************************/
//  Description : handle wifi detail window
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  HandleWifiDetailWinMsg(
                                            MMI_WIN_ID_T    win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param
                                            );

/****************************************************************************/
//  Description : WifiHandleAuthSettingWEPWinMsg
//  Global resource dependence : 
//  Author:
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleAuthSettingWEPWinMsg(
                                        MMI_WIN_ID_T    win_id, 
                                        MMI_MESSAGE_ID_E   msg_id, 
                                        DPARAM             param
                                        );

/****************************************************************************/
//  Description : WifiHandleAuthSettingWEPForConnectWinMsg
//  Global resource dependence : 
//  Author:
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleAuthSettingWEPForConnectWinMsg(
                                        MMI_WIN_ID_T    win_id, 
                                        MMI_MESSAGE_ID_E   msg_id, 
                                        DPARAM             param
                                        );

/****************************************************************************/
//  Description : WifiHandleAuthSettingWPAWPA2WinMsg
//  Global resource dependence : 
//  Author:
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleAuthSettingWPAWPA2WinMsg(
                                        MMI_WIN_ID_T    win_id, 
                                        MMI_MESSAGE_ID_E   msg_id, 
                                        DPARAM             param
                                        );

/****************************************************************************/
//  Description : WifiHandleAuthSettingWPAWPA2ForConnectWinMsg
//  Global resource dependence : 
//  Author:
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleAuthSettingWPAWPA2ForConnectWinMsg(
                                        MMI_WIN_ID_T    win_id, 
                                        MMI_MESSAGE_ID_E   msg_id, 
                                        DPARAM             param
                                        );

/****************************************************************************/
//  Description : WifiHandleAuthSettingWAPIWinMsg
//  Global resource dependence : 
//  Author:
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleAuthSettingWAPIWinMsg(
                                        MMI_WIN_ID_T    win_id, 
                                        MMI_MESSAGE_ID_E   msg_id, 
                                        DPARAM             param
                                        );

/****************************************************************************/
//  Description : WifiHandleAuthSettingWAPIPskWinMsg
//  Global resource dependence : 
//  Author:
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleAuthSettingWAPIPskWinMsg(
                                        MMI_WIN_ID_T    win_id, 
                                        MMI_MESSAGE_ID_E   msg_id, 
                                        DPARAM             param
                                        );

/****************************************************************************/
//  Description : WifiHandleAuthSettingWAPIPskForConnectWinMsg
//  Global resource dependence : 
//  Author:
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleAuthSettingWAPIPskForConnectWinMsg(
                                        MMI_WIN_ID_T    win_id, 
                                        MMI_MESSAGE_ID_E   msg_id, 
                                        DPARAM             param
                                        );

/****************************************************************************/
//  Description : WifiHandleAuthSettingWAPICERTIFICATEWinMsg
//  Global resource dependence : 
//  Author:
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleAuthSettingWAPICERTIFICATEWinMsg(
                                        MMI_WIN_ID_T    win_id, 
                                        MMI_MESSAGE_ID_E   msg_id, 
                                        DPARAM             param
                                        );

/****************************************************************************/
//  Description : WifiHandleAuthSettingWAPICERTIFICATEForConnectWinMsg
//  Global resource dependence : 
//  Author:
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleAuthSettingWAPICERTIFICATEForConnectWinMsg(
                                        MMI_WIN_ID_T    win_id, 
                                        MMI_MESSAGE_ID_E   msg_id, 
                                        DPARAM             param
                                        );

/****************************************************************************/
//  Description : handle wifi new window
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleNewWinMsg(
                                            MMI_WIN_ID_T    win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param
                                            );

/****************************************************************************/
//  Description : handle wifi edit window
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleEditWinMsg(
                                            MMI_WIN_ID_T    win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param
                                            );


/*****************************************************************************/
//  Description : open select path win
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void WifiOpenSelectFileWin(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : open select path win for connect
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void WifiOpenSelectFileForConnectWin(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : open select file win callback
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void OpenSelectClientCertificateFileWinCallBack(
                                      BOOLEAN   is_success, 
                                      FILEARRAY file_array
                                      );

/*****************************************************************************/
//  Description : open select file win callback for connect
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void OpenSelectClientCertificateFileForConnectWinCallBack(
                                      BOOLEAN   is_success, 
                                      FILEARRAY file_array
                                      );

/*****************************************************************************/
//  Description : open select file win callback
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void OpenSelectParentCertificateFileWinCallBack(
                                      BOOLEAN   is_success, 
                                      FILEARRAY file_array
                                      );

/*****************************************************************************/
//  Description : open select file win callback for connect
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void OpenSelectParentCertificateFileForconnectWinCallBack(
                                      BOOLEAN   is_success, 
                                      FILEARRAY file_array
                                      );

/*****************************************************************************/
//  Description : wifi application's message handler
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWIFI_HandleMsg(PWND app_ptr, uint16 msg_id, DPARAM param);

/*****************************************************************************/
//  Discription: wifi handle manager msg
//  Global resource dependence: None
//  Author: 
//  Note : 
//  Return: 
/*****************************************************************************/
LOCAL BOOLEAN WifiHandleManagerMsg(MMIWIFI_MSG_E message_id,void* param,uint16 size_of_param);

/*****************************************************************************/
//  Description : open wifi popmenu
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E WifiOpenConnectMenuWin(MMI_WIN_ID_T win_id);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiSaveOwnAttributeSettings(MMI_WIN_ID_T win_id);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiSaveProxySettings(MMI_WIN_ID_T win_id);

#ifndef MMI_GUI_STYLE_TYPICAL
/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleSettinglistChildWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL void InsertTwoTextOneImageItem(MMI_CTRL_ID_T ctrl_id, uint16 index, MMI_TEXT_ID_T text_1, MMI_TEXT_ID_T text_2, MMI_IMAGE_ID_T image_id, uint32 user_data);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL void AppendTwoTextOneImageItem(MMI_CTRL_ID_T ctrl_id, MMI_TEXT_ID_T text_1, MMI_TEXT_ID_T text_2, MMI_IMAGE_ID_T image_id, uint32 user_data);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL void InsertTwoTextOneImageItemByString(MMI_CTRL_ID_T ctrl_id, uint16 index, MMI_STRING_T *text_1, MMI_STRING_T *text_2, MMI_IMAGE_ID_T image_id, uint32 user_data);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL void AppendTwoTextOneImageItemByString(MMI_CTRL_ID_T ctrl_id, MMI_STRING_T *text_1, MMI_STRING_T *text_2, MMI_IMAGE_ID_T image_id, uint32 user_data);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void WifiSetSettingListProperty(MMI_WIN_ID_T win_id);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void WifiAppendSettingListItem(MMI_CTRL_ID_T ctrl_id);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void WifiSettingOpenOwnAttributeWin(void);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void WifiSettingOpenProxyWin(void);

#ifdef WLAN_RESELECT_SUPPORT        
/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void WifiSettingOpenGPRSTOWLANReselectWin(void);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void WifiSettingOpenAPReselectWin(void);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void WifiSettingOpenInqueryNoWLANWin(void);

#endif /* WLAN_RESELECT_SUPPORT */

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void WifiSettingHandleSetAutoLogin(void);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void WifiSettingHandleReset(void);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void WifiSettingHandleOn(void);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void WifiSettingHandleAutoConnectOn(void);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void WifiSettingHandleSearch(void);
#if 0
/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void wifiHandleAutoConnectOn(void);   
#endif
/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleSearchWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleWpaStateSetWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleAPEditWpaStateSetWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleAPNetworkStateWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleAPAuthTypeWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleAPConnectStatetWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleWapiStateSetWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleAPEditWapiStateSetWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleAPEditWapiTypeWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleWepStateSetWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleWepEncyptSetWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleWepModeSetWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleWepKeyIndexSetWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleAPEditWepStateSetWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleAPEditWepEncyptSetWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleAPEditWepModeSetWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleAPEditWepKeyIndexSetWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiCreateRejectPopAutoMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiCreateAPListPopAutoMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiCreateAPTrustListPopAutoMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiCreateSearchMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiCreateWpaStateSetMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiCreateAPEditWpaStateSetMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiCreateAPNetworkStateMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiCreateAPConnectStateMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiCreateAPAuthTypeMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

#ifdef WLAN_EAP_SUPPORT
/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleAPEAPSimIndexWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleWifiEapTypeSelectWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  HandleEAPAccountWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiCreateSimIndexMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void WifiCreateEapTypeMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
// Description :
// Global resource dependence : none
// Author:Jiaoyou.wu
// Note:    
/*****************************************************************************/
LOCAL void WifiHandleAPEditEAPSimIndex();

/*****************************************************************************/
//  Description : wifi get Sim Index
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL uint32 WifiGetEAPSimIndex(void);

/*****************************************************************************/
//  Description : wifi set Sim Index
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetEAPSimIndex(uint32 eap_sim_index);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL void  WifiOpenEAPAccountWin(ADD_DATA add_data);

#endif /* WLAN_EAP_SUPPORT */

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiCreateWapiStateSetMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiCreateAPEditWapiStateSetMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiCreateAPEditWapiTypeMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiCreateWepStateSetMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiCreateWepEncyptSetMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiCreateWepModeSetMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiCreateWepKeyIndexSetMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiCreateAPEditWepStateSetMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiCreateAPEditWepEncyptSetMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiCreateAPEditWepModeSetMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiCreateAPEditWepKeyIndexSetMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);
#if 0
/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiCreateRejectPopMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);
#endif
/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleGPRSToWLANWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#if 0
/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiCreateGPRSWLANMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);
#endif
/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleReselectAPWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiCreateReselectAPMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandlePromptNoWLANWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#if 0
/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiCreatePromptNoWLANMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);
#endif
/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
void WifiCreateRadioListByTextId(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, uint16 node_num, MMI_TEXT_ID_T *text_id_array, uint32 *node_array, uint32 select_index);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
void WifiCreatePopupAutoMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, MMI_STRING_T *title_str, uint16 node_num, MMI_TEXT_ID_T *text_id_array, uint32 *node_array);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleRejectListPopWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E WifiHandleApListPopWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E WifiHandleApTrustListPopWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E WifiHandleAPEditWAPIWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E WifiHandleEditAPSSIDWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void WifiSetApEditWAPIWin(void);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void WifiOpenAPPropertyWin(MMIWIFI_PROFILE_T *profile_ptr);

/*****************************************************************************/
// Description :
// Global resource dependence : none
// Author:Jiaoyou.wu
// Note:    
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAPPropertyAlertWinMsg(
                                        MMI_WIN_ID_T     win_id, 
                                        MMI_MESSAGE_ID_E msg_id, 
                                        DPARAM           param 
                                        );

/*****************************************************************************/
// Description :
// Global resource dependence : none
// Author:Jiaoyou.wu
// Note:    
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleQueryEditConnectedAP(
                                        MMI_WIN_ID_T     win_id, 
                                        MMI_MESSAGE_ID_E msg_id, 
                                        DPARAM           param 
                                        );

/*****************************************************************************/
// Description :
// Global resource dependence : none
// Author:Jiaoyou.wu
// Note:    
/*****************************************************************************/
LOCAL void WifiHandleChangeWPAState();

/*****************************************************************************/
// Description :
// Global resource dependence : none
// Author:Jiaoyou.wu
// Note:    
/*****************************************************************************/
LOCAL void WifiHandleAPEditChangeWPAState();

/*****************************************************************************/
// Description :
// Global resource dependence : none
// Author:Jiaoyou.wu
// Note:    
/*****************************************************************************/
LOCAL void WifiHandleAPEditAuthType();

/*****************************************************************************/
// Description :
// Global resource dependence : none
// Author:Jiaoyou.wu
// Note:    
/*****************************************************************************/
LOCAL void WifiHandleAPEditAuthSetting();

/*****************************************************************************/
// Description :
// Global resource dependence : none
// Author:Jiaoyou.wu
// Note:    
/*****************************************************************************/
LOCAL void WifiHandleAPEditConnectState();

/*****************************************************************************/
// Description :
// Global resource dependence : none
// Author:Jiaoyou.wu
// Note:    
/*****************************************************************************/
LOCAL void WifiHandleAPEditNetworkState();

/*****************************************************************************/
// Description :
// Global resource dependence : none
// Author:Jiaoyou.wu
// Note:    
/*****************************************************************************/
LOCAL void WifiHandleChangeWAPIState();

/*****************************************************************************/
// Description :
// Global resource dependence : none
// Author:Jiaoyou.wu
// Note:    
/*****************************************************************************/
LOCAL void WifiHandleAPEditChangeWAPIState();

/*****************************************************************************/
// Description :
// Global resource dependence : none
// Author:Jiaoyou.wu
// Note:    
/*****************************************************************************/
LOCAL void WifiHandleChangeAPWAPIType();

/*****************************************************************************/
// Description :
// Global resource dependence : none
// Author:Jiaoyou.wu
// Note:    
/*****************************************************************************/
LOCAL void WifiHandleWepModelChange();

/*****************************************************************************/
// Description :
// Global resource dependence : none
// Author:Jiaoyou.wu
// Note:    
/*****************************************************************************/
LOCAL void WifiHandleWepKeyIndexChange();

/*****************************************************************************/
// Description :
// Global resource dependence : none
// Author:Jiaoyou.wu
// Note:    
/*****************************************************************************/
LOCAL void WifiHandleWepEncyptChange();

/*****************************************************************************/
// Description :
// Global resource dependence : none
// Author:Jiaoyou.wu
// Note:    
/*****************************************************************************/
LOCAL void WifiHandleWepStateChange();

/*****************************************************************************/
// Description :
// Global resource dependence : none
// Author:Jiaoyou.wu
// Note:    
/*****************************************************************************/
LOCAL void WifiHandleAPEditWepModelChange();

/*****************************************************************************/
// Description :
// Global resource dependence : none
// Author:Jiaoyou.wu
// Note:    
/*****************************************************************************/
LOCAL void WifiHandleAPEditWepKeyIndexChange();

/*****************************************************************************/
// Description :
// Global resource dependence : none
// Author:Jiaoyou.wu
// Note:    
/*****************************************************************************/
LOCAL void WifiHandleAPEditWepEncyptChange();

/*****************************************************************************/
// Description :
// Global resource dependence : none
// Author:Jiaoyou.wu
// Note:    
/*****************************************************************************/
LOCAL void WifiHandleAPEditWepStateChange();

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void WifiEditAPSSIDWin(void);

#endif /* MMI_GUI_STYLE_TYPICAL */

/****************************************************************************/
//  Description : handle default Accesspoint window
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleDefAPSettingWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/****************************************************************************/
//  Description : handle change user info window
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  HandleModPersonalInfoWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/****************************************************************************/
//  Description : handle auto connecting select
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  HandleAutoConnectSelectWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/****************************************************************************/
//  Description : Set Default acess point settingmenu Property
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  MMIWIFI_CreateDefAPSetetingMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

/****************************************************************************/
//  Description : Handle menu action
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void WifiHandleHandleMenuAction(uint32 node_id);

/****************************************************************************/
//  Description : Set Main Property
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  MMIWIFI_SetInputInfoProperty(MMI_WIN_ID_T win_id, BOOLEAN is_verify_win);

/****************************************************************************/
//  Description : Check account
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL BOOLEAN MMIWIFI_IsvalidAccount(MMIWIFI_ACCOUNT_INFO_T *account_info);

/*****************************************************************************/
//  Description : set auto connecting list
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void MMIWIFI_SetAutoConnectList(MMI_CTRL_ID_T ctrl_id);


/*****************************************************************************/
// Description : Handle query acitve service
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWIFI_HandleQueryActiveService(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
// Description : Acitve CMCC WLAN service
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
LOCAL void MMIWIFI_ActiveService(void);

/****************************************************************************/
//  Description : Wifi Handle Wlan Active Query
//  Global resource dependence : 
//  Author: 
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleWlanActiveQuery(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              );

/****************************************************************************/
//  Description : handle wlan setting window
//  Global resource dependence : 
//  Author: 
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleWlanSettingWin(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             );

/*****************************************************************************/
//  Description : wlan set auth window
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetWlanWindow(void);

/****************************************************************************/
//  Description : handle own attribute window
//  Global resource dependence : 
//  Author: 
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleOwnAttributeSettingWin(
                                                     MMI_WIN_ID_T    win_id, 
                                                     MMI_MESSAGE_ID_E   msg_id, 
                                                     DPARAM             param
                                                     );

/*****************************************************************************/
//  Description : wlan set own attribute window
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetOwnAttributeWindow(void);

/*****************************************************************************/
//  Description : handle display label ctrl text in own attribute window
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void WifiOwnAttribute_DisplayEditIPCtrlText(MMI_CTRL_ID_T   ctrl_id);


/****************************************************************************/
//  Description : handle MAC setting window
//  Global resource dependence : 
//  Author: 
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleMacWinMsg_OwnAttribute(
                                                     MMI_WIN_ID_T    win_id, 
                                                     MMI_MESSAGE_ID_E   msg_id, 
                                                     DPARAM             param
                                                     );

/****************************************************************************/
//  Description : handle proxy setting window
//  Global resource dependence : 
//  Author: 
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleProxySettingWin(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              );

/****************************************************************************/
//  Description : handle proxy setting reject list window
//  Global resource dependence : 
//  Author: 
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleProxyRejectListWin(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              );

/****************************************************************************/
//  Description : handle proxy setting reject list pop menu window
//  Global resource dependence : 
//  Author: 
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleProxyRejectListPopMenuWin(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              );

/****************************************************************************/
//  Description : handle proxy setting reject list edit window
//  Global resource dependence : 
//  Author: 
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleProxyRejectListEditWin(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              );

/*****************************************************************************/
//  Discription: WifiCreateRejectListPopMenu
//  Global resource dependence: None
//  Author: 
//  Note : 
//  Return: 
/*****************************************************************************/
LOCAL void WifiCreateRejectListPopMenu(
                            MMI_WIN_ID_T    win_id, //IN: list menu's window id
                            MMI_CTRL_ID_T   ctrl_id
                            );

/****************************************************************************/
//  Description : handle proxy setting reject list add window
//  Global resource dependence : 
//  Author: 
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleProxyRejectListAddWin(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              );

/****************************************************************************/
//  Description : handle web key window
//  Global resource dependence : 
//  Author: 
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleWepKeyWin(
                                        MMI_WIN_ID_T    win_id, 
                                        MMI_MESSAGE_ID_E   msg_id, 
                                        DPARAM             param
                                        );

/****************************************************************************/
//  Description : handle web key window
//  Global resource dependence : 
//  Author: 
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleSpectifySearchSSID(
                                                 MMI_WIN_ID_T    win_id, 
                                                 MMI_MESSAGE_ID_E   msg_id, 
                                                 DPARAM             param
                                                 );
                                                 
/*****************************************************************************/
//  Description : wlan set proxy setting window
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetProxySettingWindow(void);

/*****************************************************************************/
//  Description : WifiSetRejectListWindow
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetRejectListWindow(void);

/*****************************************************************************/
//  Description : WifiSetEditAPWindow
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetEditAPWindow(void);

/*****************************************************************************/
//  Description : WifiSetEditWEPWindow
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetEditWEPWindow(void);

/*****************************************************************************/
//  Description : WifiSetEditWEPWindowForConnect
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetEditWEPWindowForConnect(void);

/*****************************************************************************/
//  Description : WifiSetEditWPAWPA2Window
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetEditWPAWPA2Window(void);

/*****************************************************************************/
//  Description : WifiSetEditWPAWPA2WindowForConnect
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetEditWPAWPA2WindowForConnect(void);

/*****************************************************************************/
//  Description : WifiSetEditWAPIWindow
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetEditWAPIWindow(void);

/*****************************************************************************/
//  Description : WifiSetEditWAPIPskWindow
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetEditWAPIPskWindow(void);

/*****************************************************************************/
//  Description : WifiSetEditWAPIPskForConnectWindow
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetEditWAPIPskForConnectWindow(void);

/*****************************************************************************/
//  Description : WifiSetEditWAPICertificateWindow
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetEditWAPICertificateWindow(void);

/*****************************************************************************/
//  Description : WifiSetEditWAPICertificateForConnectWindow
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetEditWAPICertificateForConnectWindow(void);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL int32 WifiGetExcludingListIndex(char *str_ptr);

/*****************************************************************************/
//  Description : WifiOwnAttribute_SetChildCtrlDisplay
//  Global resource dependence : none
//  Author:
//  Note://设置本机属性窗口子控件是否置灰
/*****************************************************************************/
LOCAL void WifiOwnAttribute_SetChildCtrlDisplay(void);

/*****************************************************************************/
//  Description : WifiOwnAttribute_SetChildCtrlDisplay
//  Global resource dependence : none
//  Author:
//  Note://设置代理设置窗口子控件是否置灰
/*****************************************************************************/
LOCAL void WifiProxy_SetChildCtrlDisplay(void);

/*****************************************************************************/
//  Description : Get List User data 
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL uint32 WifiGetListUserdata(MMI_CTRL_ID_T ctrl_id, uint16 index);

/****************************************************************************/
//  Description : set richtext item
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
LOCAL void WifiSetDetailRichtextItem(GUIRICHTEXT_ITEM_T*  item_data_ptr, GUIRICHTEXT_TEXT_TYPE_E text_type);

/****************************************************************************/
//  Description : set wifi detail
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
LOCAL void WifiSetDetail(MMI_CTRL_ID_T ctrl_id, MMIWIFI_PROFILE_T *profile_ptr);

/*****************************************************************************/
//  Discription: wifi find rssi image
//  Global resource dependence: None
//  Author: 
//  Note : 
//  Return: 
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T WifiFindRssiImage(int32 rssi);

/*****************************************************************************/
//  Description : wifi set ui option 
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetUiOption(MMIWIFI_OPTION_FOR_WIFION_E option);

/*****************************************************************************/
//  Description : wifi get ui option 
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL MMIWIFI_OPTION_FOR_WIFION_E WifiGetUiOption(void);

/*****************************************************************************/
//  Description : wifi get profile
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL MMIWIFI_PROFILE_T* WifiGetProfile(void);

/*****************************************************************************/
//  Description : wifi get ssid len
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL uint32 WifiGetSsidLen(void);

/*****************************************************************************/
//  Description : wifi set ssid len
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetSsidLen(uint32 len);

/*****************************************************************************/
//  Description : wifi get ssid
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL uint8* WifiGetSsid(void);

/*****************************************************************************/
//  Description : wifi set ssid
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetSsid(uint8* ssid_ptr);

/*****************************************************************************/
//  Description : wifi set is wep ascii
//  Global resource dependence : 
//  Author: li.li
//  Note: //输入1 表示是ASCII,0表示是HEX
/*****************************************************************************/
LOCAL void WifiSetIsWepAscii(MMIWIFI_INDEX_T index, BOOLEAN is_ascii);

/*****************************************************************************/
//  Description : wifi get is wep ascii
//  Global resource dependence : 
//  Author: li.li
//  Note:   //输出1 表示是ASCII,0表示是HEX
/*****************************************************************************/
LOCAL BOOLEAN WifiGetIsWepAscii(MMIWIFI_INDEX_T index);

/*****************************************************************************/
//  Description : wifi set is wep ascii
//  Global resource dependence : 
//  Author:
//  Note: //输入1 表示是ASCII,0表示是HEX
/*****************************************************************************/
LOCAL void WifiSetIsWpaAscii(BOOLEAN is_ascii);

/*****************************************************************************/
//  Description : wifi get is wpa ascii
//  Global resource dependence : 
//  Author:
//  Note:   //输出1 表示是ASCII,0表示是HEX
/*****************************************************************************/
LOCAL BOOLEAN WifiGetIsWpaAscii(void);

/*****************************************************************************/
//  Description : wifi set is wapi ascii
//  Global resource dependence : 
//  Author:
//  Note: //输入1 表示是ASCII,0表示是HEX
/*****************************************************************************/
LOCAL void WifiSetIsWapiAscii(BOOLEAN is_ascii);

/*****************************************************************************/
//  Description : wifi get is wapi ascii
//  Global resource dependence : 
//  Author:
//  Note:   //输出1 表示是ASCII,0表示是HEX
/*****************************************************************************/
LOCAL BOOLEAN WifiGetIsWapiAscii(void);

/*****************************************************************************/
//  Description : wifi set current wep index
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetWepCurrIndex(uint16 index);

/*****************************************************************************/
//  Description : wifi get current wep index
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL uint16 WifiGetWepCurrIndex(void);

/*****************************************************************************/
//  Description : wifi get encrypt protocol
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL WIFISUPP_ENCRYP_PROTOCOL_E WifiGetEncryptProtocol(void);

/*****************************************************************************/
//  Description : wifi set encrypt protocol
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetEncryptProtocol(WIFISUPP_ENCRYP_PROTOCOL_E protocol);

/*****************************************************************************/
//  Description : wifi get encrypt protocol
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN WifiGetIsAutoConnect(void);

/*****************************************************************************/
//  Description : WifiSetIsAutoConnect
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN WifiSetIsAutoConnect(BOOLEAN is_auto_connect);

/*****************************************************************************/
//  Description : wifi get wep 64bit
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL uint8* WifiGetWep64Bit(uint16 index);

/*****************************************************************************/
//  Description : wifi set wep 64bit
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetWep64Bit(uint16 index, uint8* wep_ptr);

/*****************************************************************************/
//  Description : wifi get wep 128bit
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL uint8* WifiGetWep128Bit(uint16 index);

/*****************************************************************************/
//  Description : wifi set wep 128bit
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetWep128Bit(uint16 index, uint8* wep_ptr);

/*****************************************************************************/
//  Description : wifi get wpa psk
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL uint8* WifiGetWpaPsk(void);

/*****************************************************************************/
//  Description : wifi set wpa psk
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetWpaPsk(uint8* psk_ptr);

/*****************************************************************************/
//  Description : wifi get wapi psk
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL uint8* WifiGetWapiPsk(void);

/*****************************************************************************/
//  Description : wifi set wapi psk
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetWapiPsk(uint8* psk_ptr);

/*****************************************************************************/
//  Description : wifi set is ssid hide
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetIsSsidHide(BOOLEAN is_hide);

/*****************************************************************************/
//  Description : wifi get is ssid hide
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN WifiGetIsSsidHide(void);

/*****************************************************************************/
//  Description : wifi set wpa eap peap credential
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetWpaEapPeapCredential(WIFISUPP_INNER_CREDENTIAL_TYPE_E credential_type);

/*****************************************************************************/
//  Description : wifi get wpa eap peap credential
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL WIFISUPP_INNER_CREDENTIAL_TYPE_E WifiGetWpaEapPeapCredential(void);

/*****************************************************************************/
//  Description : wifi set ip type
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetIpType(MMIWIFI_IPGET_TYPE_E ip_type);

/*****************************************************************************/
//  Description : wifi get ip type
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL MMIWIFI_IPGET_TYPE_E WifiGetIpType(void);

/*****************************************************************************/
//  Description : wifi set network mode
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetNetworkMode(WIFISUPP_NETWORK_MODE_E network_mode);

/*****************************************************************************/
//  Description : wifi get network mode
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL WIFISUPP_NETWORK_MODE_E WifiGetNetworkMode(void);

/*****************************************************************************/
//  Description : wifi set wpa credential
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetWpaCredential(WIFISUPP_WPA_CREDENTIAL_TYPE_E credential_type);

/*****************************************************************************/
//  Description : wifi get wpa credential
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL WIFISUPP_WPA_CREDENTIAL_TYPE_E WifiGetWpaCredential(void);

/*****************************************************************************/
//  Description : wifi set wapi credential
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetWapiCredential(WIFISUPP_WAPI_CREDENTIAL_TYPE_E credential_type);

/*****************************************************************************/
//  Description : wifi get wapi credential
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL WIFISUPP_WAPI_CREDENTIAL_TYPE_E WifiGetWapiCredential(void);

/*****************************************************************************/
//  Description : wifi set wpa eap type
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetWpaEapType(WIFISUPP_WPA_EAP_TYPE_E eap_type);

/*****************************************************************************/
//  Description : wifi get wpa eap type
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL WIFISUPP_WPA_EAP_TYPE_E WifiGetWpaEapType(void);

/*****************************************************************************/
//  Description : wifi set wpa eap ttls credential
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetWpaEapTtlsCredential(WIFISUPP_INNER_CREDENTIAL_TYPE_E credential_type);

/*****************************************************************************/
//  Description : wifi get wpa eap ttls credential
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL WIFISUPP_INNER_CREDENTIAL_TYPE_E WifiGetWpaEapTtlsCredential(void);

/*****************************************************************************/
//  Description : wifi set wep key type
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetWepKeyType(WIFISUPP_WEP_KEY_TYPE_E key_type);

/*****************************************************************************/
//  Description : wifi get wep key type
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL WIFISUPP_WEP_KEY_TYPE_E WifiGetWepKeyType(void);

/*****************************************************************************/
//  Description : WifiGetWapiCertiClientFilePath
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL uint16* WifiGetWapiCertiClientFilePath(void);

/*****************************************************************************/
//  Description : WifiSetWapiCertiClientFilePath
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetWapiCertiClientFilePath(uint16* path_ptr, uint32 file_name_len);

/*****************************************************************************/
//  Description : WifiGetWapiCertiParentFilePath
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL uint16* WifiGetWapiCertiParentFilePath(void);

/*****************************************************************************/
//  Description : WifiSetWapiCertiClientFilePath
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetWapiCertiParentFilePath(uint16* path_ptr, uint32 file_name_len);

/*****************************************************************************/
//  Description : wifi get ip address
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL char* WifiGetIpAddr(void);

/*****************************************************************************/
//  Description : wifi set ip address
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetIpAddr(char* ip_ptr);

/*****************************************************************************/
//  Description : wifi get ip gateway
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL char* WifiGetIpGateway(void);

/*****************************************************************************/
//  Description : wifi set ip gateway
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetIpGateway(char* ip_ptr);

/*****************************************************************************/
//  Description : wifi get ip submask
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL char* WifiGetIpSubmask(void);

/*****************************************************************************/
//  Description : wifi set ip submask
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetIpSubmask(char* ip_ptr);

/*****************************************************************************/
//  Description : wifi get ip dns1
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL char* WifiGetIpDns1(void);

/*****************************************************************************/
//  Description : wifi set ip dns1
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetIpDns1(char* ip_ptr);

/*****************************************************************************/
//  Description : wifi get ip dns2
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL char* WifiGetIpDns2(void);

/*****************************************************************************/
//  Description : wifi set ip dns2
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetIpDns2(char* ip_ptr);

/*****************************************************************************/
//  Description : hex to num
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL int32 WifiHexToNum(char c);

/*****************************************************************************/
//  Description : hex to byte
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL int32 WifiHexToByte(const char* hex_ptr);

/*****************************************************************************/
//  Description : hex to bin
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL int32 WifiHexStrToBin(const char* hex_ptr, uint8* buf_ptr, int32 len);

/*****************************************************************************/
//  Description : reset encrypt and credential
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiResetEncryptAndCredential(void);

/*****************************************************************************/
//  Description : reset credential
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiResetCredential(void);

/*****************************************************************************/
//  Description : reset WAPI credential
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void WifiResetWAPICredential(void);

/*****************************************************************************/
//  Description : reset wep key
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiResetWepKey(void);

/*****************************************************************************/
//  Description : reset wpa credential
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiResetWpaCredential(void);

/*****************************************************************************/
//  Description : set wep bit and mode window
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetWepBitAndModeWindow(void);

/*****************************************************************************/
//  Description : wifi reset wep 64bit
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiResetWep64Bit(uint16 index);

/*****************************************************************************/
//  Description : wifi reset wep 128bit
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiResetWep128Bit(uint16 index);

/*****************************************************************************/
//  Description : wifi check is hex
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN WifiCheckIsHex(uint8* str_ptr, uint32 len);

LOCAL BOOLEAN WifiGetIsWepOpen(void);

LOCAL void WifiSetWepOpen(BOOLEAN is_open_mode);

/****************************************************************************/
//  Description : WifiUpdateListTravCallback
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL BOOLEAN WifiUpdateListTravCallback(MMIWIFI_PROFILE_T *profile_ptr, void *find_data);

/****************************************************************************/
//  Description : WifiUpdateListTravCallback
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL BOOLEAN WifiUpdateSelectAPListTravCallback(MMIWIFI_PROFILE_T *profile_ptr, void *find_data);

/****************************************************************************/
//  Description : WifiAppendListItem
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void WifiAppendListItem(MMI_CTRL_ID_T ctrl_id, MMIWIFI_PROFILE_T *profile_ptr);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void WifiAppendSelectAPListItem(MMI_CTRL_ID_T ctrl_id, MMIWIFI_PROFILE_T *profile_ptr);

/****************************************************************************/
//  Description : handle select ap window
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E WifiHandleSelectAPWinMsg(MMI_WIN_ID_T    win_id, MMI_MESSAGE_ID_E   msg_id, DPARAM);

/*****************************************************************************/
// Description : Handle query change service
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWIFI_HandleChangeService(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

#ifndef MMI_GUI_STYLE_TYPICAL
/****************************************************************************/
//  Description : WifiAppendListItem
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL uint16 WifiListGetProfileOrderIndex(MMI_CTRL_ID_T ctrl_id, MMIWIFI_PROFILE_T *in_profile_ptr);

/****************************************************************************/
//  Description : WifiAppendListItem
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void WifiListAppendPermanentItem(MMI_CTRL_ID_T ctrl_id);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void WifiAddTrustedAp(void);

/*****************************************************************************/
// Description : Handle query acitve service
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E WifiHandleQueryForgetAP(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
// Description : Handle query acitve service
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E WifiHandleQueryEditAP(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

#endif /* MMI_GUI_STYLE_TYPICAL */

/****************************************************************************/
//  Description : WifiAppendListItem
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL BOOLEAN WifiListIsAPItemSelected(uint16 index);

/****************************************************************************/
//  Description : WifiUpdateTrustedList
//  Global resource dependence : 
//  Author:
//  Note: 
/****************************************************************************/
LOCAL void  WifiUpdateRejectList(MMI_CTRL_ID_T ctrl_id);

/****************************************************************************/
//  Description : WifiUpdateTrustedList
//  Global resource dependence : 
//  Author:
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  wifiSIMSelectCallback(uint32 dual_sys, BOOLEAN isOK, DPARAM param);

/*****************************************************************************/
//  Description : Check WEP key
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN wifi_CheckWepKeyValid(MMI_STRING_T *key, BOOLEAN *out_is_128, BOOLEAN *out_is_asscii);

/****************************************************************************/
//  Description : Tidy window
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  wifi_TidyWindow(void);

/**-------------------------------------------------------------------------*
**                         STATIC VARIABLE DEFINITION                       *
**--------------------------------------------------------------------------*/
#ifdef MMI_WLAN_TAB_STYLE
//the wifi Main Window
WINDOW_TABLE(MMIWIFI_MAIN_PARENT_WIN_TAB) = 
{
    WIN_FUNC((uint32)WifiHandleMainParentWinMsg),    
    WIN_ID(MMIWIFI_MAIN_PARENT_WIN_ID),
    WIN_STATUSBAR,
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN ),
#ifdef MMI_GUI_STYLE_TYPICAL
    CREATE_TAB_CTRL(GUI_TAB_ITEM_LEFT, GUITAB_STATE_NULL, MMIWIFI_TAB_MAIN_CTRL_ID),
#else
	CREATE_TAB_CTRL(GUI_TAB_ITEM_LEFT, GUITAB_STATE_NONEED_SCROLL|GUITAB_STATE_SINGLE_LINE, MMIWIFI_TAB_MAIN_CTRL_ID),
#endif /* MMI_GUI_STYLE_TYPICAL */
    END_WIN
};

WINDOW_TABLE(MMIWIFI_TRUSTED_LIST_CHILD_WIN_TAB) = 
{
    WIN_FUNC((uint32) WifiHandleTrustedlistChildWinMsg),    
    WIN_ID(MMIWIFI_TRUSTED_LIST_CHILD_WIN_ID),
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIWIFI_LIST_TRUSTED_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIWIFI_TRUSTED_LIST_POPMENU_WIN_TAB) = 
{
    WIN_FUNC((uint32)WifiHandleTrustedlistPopMenuWinMsg),
        WIN_ID(MMIWIFI_TRUSTED_LIST_POPMENU_WIN_ID),
        WIN_STYLE(WS_HAS_TRANSPARENT),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
//        CREATE_POPMENU_CTRL(MMIWIFI_TRUSTED_LIST_MENU, MMIWIFI_TRUSTED_LIST_POP_MENU_CTRL_ID),
        END_WIN
};

#endif /*MMI_WLAN_TAB_STYLE*/

#ifndef MMI_GUI_STYLE_TYPICAL
WINDOW_TABLE(MMIWIFI_SETTING_LIST_CHILD_WIN_TAB) = 
{
    WIN_FUNC((uint32) WifiHandleSettinglistChildWinMsg),    
    WIN_ID(MMIWIFI_SETTING_LIST_CHILD_WIN_ID),
#ifndef MMI_WLAN_TAB_STYLE
    WIN_TITLE(STXT_MAIN_SETTING),
#endif /* MMI_WLAN_TAB_STYLE */
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIWIFI_LIST_SETTING_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIWIFI_SEARCH_SET_WIN_TAB) = 
{
    WIN_FUNC((uint32) WifiHandleSearchWinMsg), 
    WIN_ID(MMIWIFI_SEARCH_SET_WIN_ID),
    WIN_TITLE(TXT_WIFI_SEARCH_WLAN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIWIFI_MENU_SEARCH_CTRL_ID),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

WINDOW_TABLE(MMIWIFI_WPA_STATE_SET_WIN_TAB) = 
{
    WIN_FUNC((uint32) WifiHandleWpaStateSetWinMsg), 
    WIN_ID(MMIWIFI_WPA_STATE_SET_WIN_ID),
    WIN_TITLE(TXT_WIFI_WPAAUTH_PSK_STATE),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIWIFI_MENU_WPA_STATE_CTRL_ID),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};


WINDOW_TABLE(MMIWIFI_AP_EDIT_STATE_SET_WIN_TAB) = 
{
    WIN_FUNC((uint32) WifiHandleAPEditWpaStateSetWinMsg), 
    WIN_ID(MMIWIFI_WPA_STATE_SET_WIN_ID),
    WIN_TITLE(TXT_WIFI_WPAAUTH_PSK_STATE),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIWIFI_MENU_WPA_STATE_CTRL_ID),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

WINDOW_TABLE(MMIWIFI_AP_NETWORK_STATE_WIN_TAB) = 
{
    WIN_FUNC((uint32) WifiHandleAPNetworkStateWinMsg), 
    WIN_ID(MMIWIFI_AP_NETWORK_STATE_WIN_ID),
    WIN_TITLE(TXT_WIFI_NETWORK_STATE),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIWIFI_MENU_AP_NETWORK_STATE_CTRL_ID),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

WINDOW_TABLE(MMIWIFI_AP_AUTH_TYPE_WIN_TAB) = 
{
    WIN_FUNC((uint32) WifiHandleAPAuthTypeWinMsg), 
    WIN_ID(MMIWIFI_AP_AUTH_TYPE_WIN_ID),
    WIN_TITLE(TXT_COMM_SECURITY_TYPE),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIWIFI_MENU_AP_AUTH_TYPE_CTRL_ID),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

#ifdef WLAN_EAP_SUPPORT
WINDOW_TABLE(MMIWIFI_AP_EAP_SIM_INDEX_WIN_TAB) = 
{
    WIN_FUNC((uint32) WifiHandleAPEAPSimIndexWinMsg), 
    WIN_ID(MMIWIFI_AP_EAP_SIM_INDEX_WIN_ID),
    WIN_TITLE(TXT_WLAN_SIM),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIWIFI_MENU_AP_EAP_SIM_INDEX_CTRL_ID),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

WINDOW_TABLE(MMIWIFI_EAP_TYPE_SELECT_WIN_TAB) = 
{
    WIN_FUNC((uint32) WifiHandleWifiEapTypeSelectWinMsg), 
    WIN_ID(MMIWIFI_EAP_TYPE_SELECT_WIN_ID),
    WIN_TITLE(TXT_WIFI_DETAIL_CRED),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIWIFI_MENU_EAP_TYPE_CTRL_ID),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

WINDOW_TABLE(MMIWIFI_EAP_ACCOUNT_WIN_TAB) = 
{
    WIN_FUNC((uint32) HandleEAPAccountWinMsg), 
    WIN_ID(MMIWIFI_EAP_ACCOUNT_WIN_ID),
    WIN_STATUSBAR,
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY|WS_DISABLE_SOFTKEY_TO_TITLE ),
    WIN_SOFTKEY(STXT_OK, TXT_NULL,TXT_NULL),
    WIN_TITLE(TXT_ACCOUNT),

    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIWIFI_FORM_EAP_ACCOUNT_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_CHILD_FORM_EAP_USER_CTRL_ID,MMIWIFI_FORM_EAP_ACCOUNT_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_LABEL_EAP_USER_CTRL_ID, MMIWIFI_CHILD_FORM_EAP_USER_CTRL_ID),
    CHILD_EDIT_TEXT_CTRL(TRUE, WIFISUPP_MAX_USERNAME_LEN,MMIWIFI_EDIT_EAP_USER_CTRL_ID, MMIWIFI_CHILD_FORM_EAP_USER_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_CHILD_FORM_EAP_PASSWORD_CTRL_ID,MMIWIFI_FORM_EAP_ACCOUNT_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_LABEL_EAP_PASSWORD_CTRL_ID, MMIWIFI_CHILD_FORM_EAP_PASSWORD_CTRL_ID),
    CHILD_EDIT_PASSWORD_CTRL(TRUE, WIFISUPP_MAX_PSW_LEN, MMIWIFI_EDIT_EAP_PASSWORD_CTRL_ID, MMIWIFI_CHILD_FORM_EAP_PASSWORD_CTRL_ID),
    END_WIN
};
#endif /* WLAN_EAP_SUPPORT */

WINDOW_TABLE(MMIWIFI_AP_CONNECT_STATE_WIN_TAB) = 
{
    WIN_FUNC((uint32) WifiHandleAPConnectStatetWinMsg), 
    WIN_ID(MMIWIFI_AP_CONNECT_STATE_WIN_ID),
    WIN_TITLE(TXT_WIFI_CONNECT_STATE_SETTING),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIWIFI_MENU_AP_CONNECT_STATE_CTRL_ID),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

WINDOW_TABLE(MMIWIFI_WAPI_STATE_SET_WIN_TAB) = 
{
    WIN_FUNC((uint32) WifiHandleWapiStateSetWinMsg), 
    WIN_ID(MMIWIFI_WAPI_STATE_SET_WIN_ID),
    WIN_TITLE(TXT_WIFI_WPAAUTH_PSK_STATE),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIWIFI_MENU_WAPI_STATE_CTRL_ID),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

WINDOW_TABLE(MMIWIFI_AP_EDIT_WAPI_STATE_SET_WIN_TAB) = 
{
    WIN_FUNC((uint32) WifiHandleAPEditWapiStateSetWinMsg), 
    WIN_ID(MMIWIFI_WAPI_STATE_SET_WIN_ID),
    WIN_TITLE(TXT_WIFI_WPAAUTH_PSK_STATE),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIWIFI_MENU_WAPI_STATE_CTRL_ID),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};


WINDOW_TABLE(MMIWIFI_AP_EDIT_WAPI_TYPE_WIN_TAB) = 
{
    WIN_FUNC((uint32) WifiHandleAPEditWapiTypeWinMsg), 
    WIN_ID(MMIWIFI_AP_EDIT_WAPI_TYPE_WIN_ID),
    WIN_TITLE(TXT_WIFI_WPAAUTH_PSK_STATE),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIWIFI_MENU_WAPI_STATE_CTRL_ID),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

WINDOW_TABLE(MMIWIFI_WEP_STATE_SET_WIN_TAB) = 
{
    WIN_FUNC((uint32) WifiHandleWepStateSetWinMsg), 
    WIN_ID(MMIWIFI_WEP_STATE_SET_WIN_ID),
    WIN_TITLE(TXT_WEP_KEY_STYLE),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIWIFI_MENU_WEP_STATE_CTRL_ID),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

WINDOW_TABLE(MMIWIFI_WEP_ENCYPT_SET_WIN_TAB) = 
{
    WIN_FUNC((uint32) WifiHandleWepEncyptSetWinMsg), 
    WIN_ID(MMIWIFI_WEP_ENCYPT_SET_WIN_ID),
    WIN_TITLE(TXT_WIFI_BIT_SETTING),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIWIFI_MENU_WEP_ENCYPT_CTRL_ID),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

WINDOW_TABLE(MMIWIFI_WEP_MODE_SET_WIN_TAB) = 
{
    WIN_FUNC((uint32) WifiHandleWepModeSetWinMsg), 
    WIN_ID(MMIWIFI_WEP_MODE_SET_WIN_ID),
    WIN_TITLE(TXT_DETAIL_MODE),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIWIFI_MENU_WEP_MODE_CTRL_ID),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

WINDOW_TABLE(MMIWIFI_WEP_KEY_INDEX_SET_WIN_TAB) = 
{
    WIN_FUNC((uint32) WifiHandleWepKeyIndexSetWinMsg), 
    WIN_ID(MMIWIFI_WEP_KEY_INDEX_SET_WIN_ID),
    WIN_TITLE(TXT_WEP_KEY_INDEX),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIWIFI_MENU_WEP_KEY_INDEX_CTRL_ID),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

WINDOW_TABLE(MMIWIFI_AP_EDIT_WEP_STATE_SET_WIN_TAB) = 
{
    WIN_FUNC((uint32) WifiHandleAPEditWepStateSetWinMsg), 
    WIN_ID(MMIWIFI_AP_EDIT_WEP_STATE_SET_WIN_ID),
    WIN_TITLE(TXT_WEP_KEY_STYLE),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIWIFI_MENU_WEP_STATE_CTRL_ID),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

WINDOW_TABLE(MMIWIFI_AP_EDIT_WEP_ENCYPT_SET_WIN_TAB) = 
{
    WIN_FUNC((uint32) WifiHandleAPEditWepEncyptSetWinMsg), 
    WIN_ID(MMIWIFI_AP_EDIT_WEP_ENCYPT_SET_WIN_ID),
    WIN_TITLE(TXT_WIFI_BIT_SETTING),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIWIFI_MENU_WEP_ENCYPT_CTRL_ID),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

WINDOW_TABLE(MMIWIFI_AP_EDIT_WEP_MODE_SET_WIN_TAB) = 
{
    WIN_FUNC((uint32) WifiHandleAPEditWepModeSetWinMsg), 
    WIN_ID(MMIWIFI_AP_EDIT_WEP_MODE_SET_WIN_ID),
    WIN_TITLE(TXT_DETAIL_MODE),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIWIFI_MENU_WEP_MODE_CTRL_ID),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

WINDOW_TABLE(MMIWIFI_AP_EDIT_WEP_KEY_INDEX_SET_WIN_TAB) = 
{
    WIN_FUNC((uint32) WifiHandleAPEditWepKeyIndexSetWinMsg), 
    WIN_ID(MMIWIFI_AP_EDIT_WEP_KEY_INDEX_SET_WIN_ID),
    WIN_TITLE(TXT_WEP_KEY_INDEX),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIWIFI_MENU_WEP_KEY_INDEX_CTRL_ID),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

WINDOW_TABLE(MMIWIFI_GPRS_TO_WLAN_SET_WIN_TAB) = 
{
    WIN_FUNC((uint32) WifiHandleGPRSToWLANWinMsg), 
    WIN_ID(MMIWIFI_GPRS_TO_WLAN_SET_WIN_ID),
    WIN_TITLE(TXT_WLAN_GPRSTOWLAN_RESELCT),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIWIFI_MENU_GPRS_TO_WLAN_CTRL_ID),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

WINDOW_TABLE(MMIWIFI_RELECT_AP_SET_WIN_TAB) = 
{
    WIN_FUNC((uint32) WifiHandleReselectAPWinMsg), 
    WIN_ID(MMIWIFI_RELECT_AP_SET_WIN_ID),
    WIN_TITLE(TXT_WLAN_AP_RESELCT),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIWIFI_MENU_RELECT_AP_SET_CTRL_ID),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

WINDOW_TABLE(MMIWIFI_PROMPT_NO_WLAN_SET_WIN_TAB) = 
{
    WIN_FUNC((uint32) WifiHandlePromptNoWLANWinMsg), 
    WIN_ID(MMIWIFI_PROMPT_NO_WLAN_SET_WIN_ID),
    WIN_TITLE(TXT_WLAN_NO_WLAN_USEGPRS),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIWIFI_MENU_PROMPT_NO_WLAN_CTRL_ID),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

WINDOW_TABLE(MMIWIFI_REJECT_LIST_POP_WIN_TAB) = 
{
    WIN_FUNC((uint32) WifiHandleRejectListPopWinMsg), 
    WIN_ID(MMIWIFI_REJECT_LIST_POP_WIN_ID),
    END_WIN
};

WINDOW_TABLE(MMIWIFI_AP_LIST_POP_WIN_TAB) = 
{
    WIN_FUNC((uint32) WifiHandleApListPopWinMsg),
#ifdef MMI_PDA_SUPPORT
    WIN_HIDE_STATUS,
#endif
    WIN_ID(MMIWIFI_AP_LIST_POP_WIN_ID),
    END_WIN
};

WINDOW_TABLE(MMIWIFI_AP_TRUST_LIST_POP_WIN_TAB) = 
{
    WIN_FUNC((uint32) WifiHandleApTrustListPopWinMsg), 
    WIN_ID(MMIWIFI_AP_TRUST_LIST_POP_WIN_ID),
    END_WIN
};

//the wlan auth setting win---WAPI
WINDOW_TABLE(MMIWIFI_AP_EDIT_WAPI_WIN_TAB) = 
{
    WIN_FUNC((uint32)WifiHandleAPEditWAPIWinMsg),    
        WIN_ID(MMIWIFI_AP_EDIT_WAPI_WIN_ID),
        WIN_TITLE(TXT_COMM_AUTH_SETTING),
        WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),

        CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIWIFI_FORM_AP_EDIT_WAPI_ID),

        CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, MMIWLAN_LIST_WAPI_STATE_CTRL_ID, MMIWIFI_FORM_AP_EDIT_WAPI_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_CHILD_FORM_WAPI_PASSWORD_CTRL_ID,MMIWIFI_FORM_AP_EDIT_WAPI_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_EDIT_WAPI_PRESHAREKEY_LABEL_CTRL_ID, MMIWIFI_CHILD_FORM_WAPI_PASSWORD_CTRL_ID),//预共享密钥
        CHILD_EDIT_PASSWORD_CTRL(TRUE, WIFISUPP_WPA_PSK_LEN, MMIWIFI_EDIT_WAPI_PRESHAREKEY_EDIT_CTRL_ID, MMIWIFI_CHILD_FORM_WAPI_PASSWORD_CTRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_CHILD_FORM_CLIENT_CTRL_ID,MMIWIFI_FORM_AP_EDIT_WAPI_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_EDIT_WAPI_CLIENT_CERTIFICATE_TEXT_LABEL_CTRL_ID, MMIWIFI_CHILD_FORM_CLIENT_CTRL_ID),//WAPI客户端证书
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, MMIWIFI_EDIT_WAPI_CLIENT_CERTIFICATE_LABEL_CTRL_ID, MMIWIFI_CHILD_FORM_CLIENT_CTRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_CHILD_FORM_PARENT_CTRL_ID,MMIWIFI_FORM_AP_EDIT_WAPI_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_EDIT_WAPI_PARENT_CERTIFICATE_TEXT_LABEL_CTRL_ID, MMIWIFI_CHILD_FORM_PARENT_CTRL_ID),//WAPI根证书
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, MMIWIFI_EDIT_WAPI_PARENT_CERTIFICATE_LBAEL_CTRL_ID, MMIWIFI_CHILD_FORM_PARENT_CTRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_CHILD_FORM_UNINSTALL_CTRL_ID,MMIWIFI_FORM_AP_EDIT_WAPI_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, MMIWIFI_LABEL_WAPI_UNINSTALL_CTRL_ID, MMIWIFI_CHILD_FORM_UNINSTALL_CTRL_ID),

        WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
        END_WIN
};



WINDOW_TABLE(MMIWIFI_EDIT_AP_SSID_WIN_TAB) = 
{
    WIN_FUNC((uint32) WifiHandleEditAPSSIDWinMsg), 
    WIN_ID(MMIWIFI_EDIT_AP_SSID_WIN_ID),
    WIN_STATUSBAR,
    WIN_TITLE(TXT_WIFI_SSID),
    CREATE_EDIT_TEXT_CTRL(WIFISUPP_SSID_MAX_LEN, MMIWIFI_EDIT_SSID_DATA_EDIT_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

#endif /* MMI_GUI_STYLE_TYPICAL */

WINDOW_TABLE(MMIWIFI_DEF_AP_SET_WIN_TAB) = 
{
    WIN_FUNC((uint32) WifiHandleDefAPSettingWinMsg), 
    WIN_ID(MMIWIFI_DEF_AP_SET_WIN_ID),
    WIN_STATUSBAR,
    WIN_TITLE(TXT_WIFI_DEFAULT_AP_SETTING),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};


WINDOW_TABLE(MMIWIFI_MOD_PERSONAL_INFO_WIN_TAB) = 
{
    WIN_FUNC((uint32) HandleModPersonalInfoWinMsg), 
    WIN_ID(MMIWIFI_MOD_PERSONAL_INFO_WIN_ID),
    WIN_TITLE(TXT_WIFI_PERSONAL_INFO_SETTING),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),

    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIWIFI_MOD_PERSONAL_INFO_FORM_CTRL_ID),

    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_LABELT_USER_NAME_CTRL_ID, MMIWIFI_MOD_PERSONAL_INFO_FORM_CTRL_ID),
    CHILD_EDIT_TEXT_CTRL(TRUE, MMIWIFI_USER_NAME_MAX_LEN,MMIWIFI_EDIT_USER_NAME_CTRL_ID, MMIWIFI_MOD_PERSONAL_INFO_FORM_CTRL_ID),

    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_LABELT_PASSWORD_CTRL_ID, MMIWIFI_MOD_PERSONAL_INFO_FORM_CTRL_ID),
    CHILD_EDIT_PASSWORD_CTRL(TRUE, MMIWIFI_PASSWORD_MAX_LEN, MMIWIFI_EDIT_PASSWORD_CTRL_ID, MMIWIFI_MOD_PERSONAL_INFO_FORM_CTRL_ID),

    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE,MMIWIFI_LABELT_RE_PASSWORD_CTRL_ID, MMIWIFI_MOD_PERSONAL_INFO_FORM_CTRL_ID),
    CHILD_EDIT_PASSWORD_CTRL(TRUE, MMIWIFI_PASSWORD_MAX_LEN, MMIWIFI_EDIT_RE_PASSWORD_CTRL_ID, MMIWIFI_MOD_PERSONAL_INFO_FORM_CTRL_ID),

    END_WIN
}; 

WINDOW_TABLE(MMIWIFI_AUTO_CONNECT_SELECT_WIN_TAB ) = 
{
    WIN_TITLE(TXT_WIFI_AUTO_CONNECTING_SETTING),
    WIN_FUNC((uint32)HandleAutoConnectSelectWinMsg),    
    WIN_ID(MMIWIFI_AUTO_CONNECT_SELECT_WIN_ID),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMIWIFI_LIST_AUTO_CONNECT_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

WINDOW_TABLE(MMIWIFI_WLAN_SETTING_WIN_TAB) = //WLAN设置
{
    WIN_FUNC((uint32)WifiHandleWlanSettingWin),
        WIN_ID(MMIWLAN_SETTING_WIN_ID),
        WIN_STATUSBAR,
        //WIN_TITLE(STXT_MAIN_SETTING),
        CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIWIFI_WLAN_SETTING_FORMCTRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_WLAN_SETTING_ONOFF_FORMCTRL_ID,MMIWIFI_WLAN_SETTING_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_WLAN_LABEL_CTRL_ID, MMIWIFI_WLAN_SETTING_ONOFF_FORMCTRL_ID),
        CHILD_DROPDOWNLIST_CTRL(TRUE, MMIWIFI_WLAN_ON_OFF_DROPDOWNLIST_CTRL_ID, MMIWIFI_WLAN_SETTING_ONOFF_FORMCTRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_WLAN_SETTING_AUTO_CONNECT_FORMCTRL_ID,MMIWIFI_WLAN_SETTING_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_AUTO_CONNECT_LABEL_CTRL_ID, MMIWIFI_WLAN_SETTING_AUTO_CONNECT_FORMCTRL_ID),
        CHILD_DROPDOWNLIST_CTRL(TRUE, MMIWIFI_AUTO_CONNECT_DROPDOWNLIST_CTRL_ID, MMIWIFI_WLAN_SETTING_AUTO_CONNECT_FORMCTRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_WLAN_SETTING_SEARCH_WLAN_FORMCTRL_ID,MMIWIFI_WLAN_SETTING_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_SEARCH_WLAN_LABEL_CTRL_ID, MMIWIFI_WLAN_SETTING_SEARCH_WLAN_FORMCTRL_ID),
        CHILD_DROPDOWNLIST_CTRL(TRUE, MMIWIFI_SEARCH_PERIOD_DROPDOWNLIST_CTRL_ID, MMIWIFI_WLAN_SETTING_SEARCH_WLAN_FORMCTRL_ID),
#ifdef WLAN_RESELECT_SUPPORT    
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_WLAN_SETTING_GPRSTOWLAN_RESELECT_FORMCTRL_ID,MMIWIFI_WLAN_SETTING_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_GPRSTOWLAN_RESELECT_LABEL_CTRL_ID, MMIWIFI_WLAN_SETTING_GPRSTOWLAN_RESELECT_FORMCTRL_ID),
        CHILD_DROPDOWNLIST_CTRL(TRUE, MMIWIFI_GPRSTOWLAN_RESELECT_DROPDOWNLIST_CTRL_ID, MMIWIFI_WLAN_SETTING_GPRSTOWLAN_RESELECT_FORMCTRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_WLAN_SETTING_AP_RESELECT_FORMCTRL_ID,MMIWIFI_WLAN_SETTING_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_AP_RESELECT_LABEL_CTRL_ID, MMIWIFI_WLAN_SETTING_AP_RESELECT_FORMCTRL_ID),
        CHILD_DROPDOWNLIST_CTRL(TRUE, MMIWIFI_AP_RESELECT_DROPDOWNLIST_CTRL_ID, MMIWIFI_WLAN_SETTING_AP_RESELECT_FORMCTRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_WLAN_SETTING_INQUIRE_SETTING_FORMCTRL_ID,MMIWIFI_WLAN_SETTING_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_NO_WLAN_INQUIRE_LABEL_CTRL_ID, MMIWIFI_WLAN_SETTING_INQUIRE_SETTING_FORMCTRL_ID),
        CHILD_DROPDOWNLIST_CTRL(TRUE, MMIWIFI_NO_WLAN_INQUIRE_DROPDOWNLIST_CTRL_ID, MMIWIFI_WLAN_SETTING_INQUIRE_SETTING_FORMCTRL_ID),
#endif
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_WLAN_SETTING_ATTRIBUTE_FORMCTRL_ID,MMIWIFI_WLAN_SETTING_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, MMIWIFI_OWN_ATTRIBUTE_LABEL_CTRL_ID, MMIWIFI_WLAN_SETTING_ATTRIBUTE_FORMCTRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_WLAN_SETTING_PROXY_SETTING_FORMCTRL_ID,MMIWIFI_WLAN_SETTING_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, MMIWIFI_PROXY_SETTING_LABEL_CTRL_ID, MMIWIFI_WLAN_SETTING_PROXY_SETTING_FORMCTRL_ID),
        WIN_SOFTKEY(STXT_SELECT,TXT_NULL,STXT_RETURN),
        END_WIN
};

WINDOW_TABLE(MMIWIFI_OWN_ATTRIBUTE_WIN_TAB) = //本机属性
{
    WIN_FUNC((uint32)WifiHandleOwnAttributeSettingWin),
        WIN_ID(MMIWLAN_OWN_ATTRIBUTE_WIN_ID),
        WIN_TITLE(TXT_WIFI_OWN_ATTRIBUTE),
        
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    WIN_SOFTKEY(STXT_SAVE,TXT_NULL,STXT_RETURN),
#else
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
#endif /* MMI_PDA_SUPPORT */

        CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIWIFI_OWN_ATTRIBUTE_FORMCTRL_ID),
#ifndef MMI_GUI_STYLE_TYPICAL
        CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, MMIWIFI_LIST_IP_TYPE_CTRL_ID, MMIWIFI_OWN_ATTRIBUTE_FORMCTRL_ID),
#else
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_CHILD_FORM_STATIC_IP_CTRL_ID,MMIWIFI_OWN_ATTRIBUTE_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_IP_TYPE_LABEL_CTRL_ID, MMIWIFI_CHILD_FORM_STATIC_IP_CTRL_ID),
        CHILD_DROPDOWNLIST_CTRL(TRUE, MMIWIFI_IP_TYPE_DROPDOWNLIST_CTRL_ID, MMIWIFI_CHILD_FORM_STATIC_IP_CTRL_ID),
#endif /* MMI_GUI_STYLE_TYPICAL */
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_CHILD_FORM_MAC_CTRL_ID,MMIWIFI_OWN_ATTRIBUTE_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_MAC_LABEL_CTRL_ID, MMIWIFI_CHILD_FORM_MAC_CTRL_ID),//MAC 地址
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_MAC_DATA_LABEL_CTRL_ID, MMIWIFI_CHILD_FORM_MAC_CTRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_CHILD_FORM_IP_ADDRESS_CTRL_ID,MMIWIFI_OWN_ATTRIBUTE_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_IP_LABEL_CTRL_ID, MMIWIFI_CHILD_FORM_IP_ADDRESS_CTRL_ID),//IP
        CHILD_EDIT_IP_CTRL(TRUE, MMIWIFI_IP_DATA_EDIT_CTRL_ID, MMIWIFI_CHILD_FORM_IP_ADDRESS_CTRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_CHILD_FORM_GATEWAY_CTRL_ID,MMIWIFI_OWN_ATTRIBUTE_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_GATEWAY_LABEL_CTRL_ID, MMIWIFI_CHILD_FORM_GATEWAY_CTRL_ID),//网关
        CHILD_EDIT_IP_CTRL(TRUE, MMIWIFI_GATEWAY_DATA_EDIT_CTRL_ID, MMIWIFI_CHILD_FORM_GATEWAY_CTRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_CHILD_FORM_MASK_CTRL_ID,MMIWIFI_OWN_ATTRIBUTE_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_SUBMASK_LABEL_CTRL_ID, MMIWIFI_CHILD_FORM_MASK_CTRL_ID),//子网掩码
        CHILD_EDIT_IP_CTRL(TRUE, MMIWIFI_SUBMASK_DATA_EDIT_CTRL_ID, MMIWIFI_CHILD_FORM_MASK_CTRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_CHILD_FORM_DNS1_CTRL_ID,MMIWIFI_OWN_ATTRIBUTE_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_DNS1_LABEL_CTRL_ID, MMIWIFI_CHILD_FORM_DNS1_CTRL_ID),//DNS1
        CHILD_EDIT_IP_CTRL(TRUE, MMIWIFI_DNS1_DATA_EDIT_CTRL_ID, MMIWIFI_CHILD_FORM_DNS1_CTRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_CHILD_FORM_DNS2_CTRL_ID,MMIWIFI_OWN_ATTRIBUTE_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_DNS2_LABEL_CTRL_ID, MMIWIFI_CHILD_FORM_DNS2_CTRL_ID),//DNS2
        CHILD_EDIT_IP_CTRL(TRUE, MMIWIFI_DNS2_DATA_EDIT_CTRL_ID, MMIWIFI_CHILD_FORM_DNS2_CTRL_ID),
        

        END_WIN
};

WINDOW_TABLE(MMIWIFI_MAC_WIN_TAB) = //设置MAC地址窗口
{
    WIN_FUNC((uint32)WifiHandleMacWinMsg_OwnAttribute),
        WIN_ID(MMIWIFI_MAC_WIN_ID),
        WIN_TITLE(TXT_WIFI_MAC),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        CREATE_EDIT_IP_CTRL(MMIWIFI_MAC_CTRL_ID),
        END_WIN
};

WINDOW_TABLE(MMIWIFI_PROXY_SETTING_WIN_TAB) = //代理设置
{
    WIN_FUNC((uint32)WifiHandleProxySettingWin),
        WIN_ID(MMIWLAN_PROXY_SETTING_WIN_ID),
        WIN_TITLE(TXT_COMM_PROXY),

#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    WIN_SOFTKEY(STXT_SAVE,TXT_NULL,STXT_RETURN),
#else
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
#endif /* MMI_PDA_SUPPORT */

        CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIWIFI_PROXY_SETTING_FORMCTRL_ID),
#ifndef MMI_GUI_STYLE_TYPICAL
        CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, MMIWIFI_LIST_PROXY_CTRL_ID, MMIWIFI_PROXY_SETTING_FORMCTRL_ID),
#else
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_CHILD_FORM_PROXY_CTRL_ID,MMIWIFI_PROXY_SETTING_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_PROXY_QUERY_LABEL_CTRL_ID, MMIWIFI_CHILD_FORM_PROXY_CTRL_ID),
        CHILD_DROPDOWNLIST_CTRL(TRUE, MMIWIFI_PROXY_QUERY_DROPDOWNLIST_CTRL_ID, MMIWIFI_CHILD_FORM_PROXY_CTRL_ID),
#endif /* MMI_GUI_STYLE_TYPICAL */
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_CHILD_FORM_PROXY_IP_CTRL_ID,MMIWIFI_PROXY_SETTING_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_PROXY_IP_LABEL_CTRL_ID, MMIWIFI_CHILD_FORM_PROXY_IP_CTRL_ID),//代理IP
        CHILD_EDIT_TEXT_CTRL(TRUE, MMIWIFI_URL_MAX_LEN, MMIWIFI_PROXY_IP_DATA_EDIT_CTRL_ID, MMIWIFI_CHILD_FORM_PROXY_IP_CTRL_ID),
       
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_CHILD_FORM_PROXY_PORT_TRL_ID,MMIWIFI_PROXY_SETTING_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_PROXY_PORT_LABEL_CTRL_ID, MMIWIFI_CHILD_FORM_PROXY_PORT_TRL_ID),//代理端口
        CHILD_EDIT_DIGITAL_CTRL(TRUE,PROXY_PORT_LEN,MMIWIFI_PROXY_PORT_EDIT_CTRL_ID,MMIWIFI_CHILD_FORM_PROXY_PORT_TRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_CHILD_FORM_PROXY_USER_CTRL_ID,MMIWIFI_PROXY_SETTING_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_PROXY_NAME_LABEL_CTRL_ID, MMIWIFI_CHILD_FORM_PROXY_USER_CTRL_ID),//用户名
        CHILD_EDIT_TEXT_CTRL(TRUE,PROXY_NAME_LEN,MMIWIFI_PROXY_NAME_EDIT_CTRL_ID,MMIWIFI_CHILD_FORM_PROXY_USER_CTRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_CHILD_FORM_PROXY_PASSWORD_CTRL_ID,MMIWIFI_PROXY_SETTING_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_PROXY_PASSWORD_LABEL_CTRL_ID, MMIWIFI_CHILD_FORM_PROXY_PASSWORD_CTRL_ID),//密码
        CHILD_EDIT_PASSWORD_CTRL(TRUE, PROXY_PASSWORD_LEN, MMIWIFI_PROXY_PASSWORD_EDIT_CTRL_ID, MMIWIFI_CHILD_FORM_PROXY_PASSWORD_CTRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_CHILD_FORM_PROXY_REJECTLIST_CTRL_ID,MMIWIFI_PROXY_SETTING_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, MMIWIFI_PROXY_REJECTLIST_LABEL_CTRL_ID, MMIWIFI_CHILD_FORM_PROXY_REJECTLIST_CTRL_ID),//排除列表
        END_WIN
};

WINDOW_TABLE(MMIWIFI_PROXY_REJECTLIST_WIN_TAB) = //排除列表
{
    WIN_FUNC((uint32)WifiHandleProxyRejectListWin),
        WIN_ID(MMIWLAN_PROXY_REJECTLIST_WIN_ID),
        WIN_TITLE(TXT_WIFI_PROXY_REJECTLIST),
              CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIWIFI_LIST_REJECT_LIST_CTRL_ID),
        WIN_SOFTKEY(STXT_OPTION,TXT_EDIT,STXT_RETURN),
        END_WIN
};

WINDOW_TABLE(MMIWIFI_REJECTLIST_POPMENU_WIN_TAB) = //排除列表选项菜单
{
    WIN_FUNC((uint32)WifiHandleProxyRejectListPopMenuWin),
        WIN_ID(MMIWLAN_PROXY_REJECTLIST_POPMENU_WIN_ID),
        WIN_STYLE(WS_HAS_TRANSPARENT),
        WIN_SOFTKEY(STXT_OPTION,TXT_EDIT,STXT_RETURN),
        END_WIN
};

WINDOW_TABLE(MMIWIFI_REJECTLIST_EDIT_WIN_TAB) = //排除列表编辑窗口
{
    WIN_FUNC((uint32)WifiHandleProxyRejectListEditWin),
        WIN_ID(MMIWLAN_PROXY_REJECTLIST_EDIT_WIN_ID),
        WIN_TITLE(TXT_WIFI_PROXY_REJECTLIST),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif /* MMI_PDA_SUPPORT */
        //CREATE_EDIT_TEXT_CTRL(MMIWIFI_URL_MAX_LEN, MMIWIFI_PROXY_REJECTLIST_EDIT_CTRL_ID),
        CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIWIFI_PROXY_REJECTLIST_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_PROXY_REJECTLIST_EDIT_LABEL_CTRL_ID, MMIWIFI_PROXY_REJECTLIST_FORMCTRL_ID),//代理端口
        CHILD_EDIT_TEXT_CTRL(TRUE,MMIWIFI_URL_MAX_LEN,MMIWIFI_PROXY_REJECTLIST_NEW_EDIT_CTRL_ID,MMIWIFI_PROXY_REJECTLIST_FORMCTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
        END_WIN
};

WINDOW_TABLE(MMIWIFI_REJECTLIST_ADD_WIN_TAB) = //排除列表 添加窗口
{
    WIN_FUNC((uint32)WifiHandleProxyRejectListAddWin),
        WIN_ID(MMIWLAN_PROXY_REJECTLIST_ADD_WIN_ID),
        WIN_TITLE(TXT_WIFI_PROXY_REJECTLIST),
#ifndef MMI_GUI_STYLE_TYPICAL
    CREATE_EDIT_TEXT_CTRL(MMIWIFI_URL_MAX_LEN, MMIWIFI_PROXY_REJECTLIST_NEW_EDIT_CTRL_ID),
#else
        CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIWIFI_PROXY_REJECTLIST_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_PROXY_REJECTLIST_EDIT_LABEL_CTRL_ID, MMIWIFI_PROXY_REJECTLIST_FORMCTRL_ID),//代理端口
        CHILD_EDIT_TEXT_CTRL(TRUE,MMIWIFI_URL_MAX_LEN,MMIWIFI_PROXY_REJECTLIST_NEW_EDIT_CTRL_ID,MMIWIFI_PROXY_REJECTLIST_FORMCTRL_ID),
#endif /* MMI_GUI_STYLE_TYPICAL */

        WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
        END_WIN
};

WINDOW_TABLE(MMIWIFI_SPECTIFY_SEARCH_SSID_WIN_TAB) = //搜索指定接入点
{
    WIN_FUNC((uint32)WifiHandleSpectifySearchSSID),
        WIN_ID(MMIWIFI_SPECTIFY_SEARCH_SSID_WIN_ID),
        WIN_TITLE(TXT_WIFI_SPECTIFY_SEARCH_SSID),
#ifndef MMI_GUI_STYLE_TYPICAL
    CREATE_EDIT_TEXT_CTRL(WIFISUPP_SSID_MAX_LEN, MMIWIFI_SPECTIFY_SEARCH_SSID_EDIT_CTRL_ID),
#else
        CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIWIFI_SPECTIFY_SEARCH_SSID_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_SPECTIFY_SEARCH_SSID_LABEL_CTRL_ID, MMIWIFI_SPECTIFY_SEARCH_SSID_FORMCTRL_ID),
        CHILD_EDIT_TEXT_CTRL(TRUE, WIFISUPP_SSID_MAX_LEN, MMIWIFI_SPECTIFY_SEARCH_SSID_EDIT_CTRL_ID, MMIWIFI_SPECTIFY_SEARCH_SSID_FORMCTRL_ID),
#endif /* MMI_GUI_STYLE_TYPICAL */

        WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
        END_WIN
};

//the wifi list win
WINDOW_TABLE(MMIWIFI_LIST_WIN_TAB) = 
{
    WIN_FUNC((uint32)WifiHandleListWinMsg),    
    WIN_ID(MMIWIFI_LIST_WIN_ID),
    WIN_STATUSBAR,
#ifndef MMI_WLAN_TAB_STYLE
    WIN_TITLE(TXT_COMM_LIST),
#endif /* MMI_WLAN_TAB_STYLE */
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIWIFI_LIST_CTRL_ID),
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
    END_WIN
};

//the wifi detail win
WINDOW_TABLE(MMIWIFI_DETAIL_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleWifiDetailWinMsg),    
    WIN_ID(MMIWIFI_DETAIL_WIN_ID),
    WIN_TITLE(STXT_DETAIL),
    CREATE_RICHTEXT_CTRL(MMIWIFI_DETAIL_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

//the wlan edit win
WINDOW_TABLE(MMIWIFI_EDIT_WIN_TAB) = 
{
    WIN_FUNC((uint32)WifiHandleEditWinMsg), 
        WIN_ID(MMIWIFI_EDIT_WIN_ID),
        WIN_TITLE(TXT_EDIT),
#ifdef MMI_PDA_SUPPORT
        WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif /* MMI_PDA_SUPPORT */

        WIN_SOFTKEY(STXT_SAVE, TXT_NULL, STXT_RETURN),

#ifndef MMI_GUI_STYLE_TYPICAL
        CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIWLAN_LIST_AP_EDIT_CTRL_ID),
#else
        CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIWIFI_EDIT_AP_FORMCTRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_CHILD_FORM_SSID_CTRL_ID,MMIWIFI_EDIT_AP_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_EDIT_SSID_LABEL_CTRL_ID, MMIWIFI_CHILD_FORM_SSID_CTRL_ID),//网络名称SSID
        CHILD_EDIT_TEXT_CTRL(TRUE, WIFISUPP_SSID_MAX_LEN, MMIWIFI_EDIT_SSID_DATA_EDIT_CTRL_ID, MMIWIFI_CHILD_FORM_SSID_CTRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_CHILD_FORM_NETWORK_STATE_CTRL_ID,MMIWIFI_EDIT_AP_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_EDIT_NETWORK_STATE_LABEL_CTRL_ID, MMIWIFI_CHILD_FORM_NETWORK_STATE_CTRL_ID),//网络状态
        CHILD_DROPDOWNLIST_CTRL(TRUE, MMIWIFI_EDIT_NETWORK_STATE_DROPDOWNLIST_CTRL_ID, MMIWIFI_CHILD_FORM_NETWORK_STATE_CTRL_ID),

        CHILD_FORM_CTRL(FALSE,GUIFORM_LAYOUT_ORDER,MMIWIFI_CHILD_FORM_NETWORK_TYPE_CTRL_ID,MMIWIFI_EDIT_AP_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_EDIT_NETWORK_TYPE_LABEL_CTRL_ID, MMIWIFI_CHILD_FORM_NETWORK_TYPE_CTRL_ID),//WLAN网络模式
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, MMIWIFI_EDIT_NETWORK_TYPE_DATA_LABEL_CTRL_ID, MMIWIFI_CHILD_FORM_NETWORK_TYPE_CTRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_CHILD_FORM_AUTH_TYPE_CTRL_ID,MMIWIFI_EDIT_AP_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_EDIT_AUTH_TYPE_LABEL_CTRL_ID, MMIWIFI_CHILD_FORM_AUTH_TYPE_CTRL_ID),//WLAN安全模式
        CHILD_DROPDOWNLIST_CTRL(TRUE,MMIWIFI_EDIT_AUTH_TYPE_DROPDOWNLIST_CTRL_ID,MMIWIFI_CHILD_FORM_AUTH_TYPE_CTRL_ID),

#ifdef WLAN_EAP_SUPPORT
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_CHILD_FORM_EAP_SIM_INDEX_CTRL_ID,MMIWIFI_EDIT_AP_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_EDIT_EAP_SIM_INDEX_LABEL_CTRL_ID, MMIWIFI_CHILD_FORM_EAP_SIM_INDEX_CTRL_ID),//EAP
        CHILD_DROPDOWNLIST_CTRL(TRUE,MMIWIFI_MENU_AP_EAP_SIM_INDEX_CTRL_ID,MMIWIFI_CHILD_FORM_EAP_SIM_INDEX_CTRL_ID),
#endif /* WLAN_EAP_SUPPORT */

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_CHILD_FORM_AUTH_SETTING_CTRL_ID,MMIWIFI_EDIT_AP_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, MMIWIFI_EDIT_AUTH_SETTING_LABEL_CTRL_ID, MMIWIFI_CHILD_FORM_AUTH_SETTING_CTRL_ID),//WLAN安全设置

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_CHILD_FORM_CONNECT_STATE_CTRL_ID,MMIWIFI_EDIT_AP_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_EDIT_CONNECT_STATE_LABEL_CTRL_ID, MMIWIFI_CHILD_FORM_CONNECT_STATE_CTRL_ID),//连接状态
        CHILD_DROPDOWNLIST_CTRL(TRUE,  MMIWIFI_EDIT_CONNECT_STATE_DROPDOWNLIST_CTRL_ID, MMIWIFI_CHILD_FORM_CONNECT_STATE_CTRL_ID),

        WIN_SOFTKEY(STXT_SAVE, TXT_NULL, STXT_RETURN),
#endif /* MMI_GUI_STYLE_TYPICAL */
        
        END_WIN
};

//the wlan new win
WINDOW_TABLE(MMIWIFI_NEW_WIN_TAB) = 
{
    WIN_FUNC((uint32)WifiHandleNewWinMsg),    
        WIN_ID(MMIWIFI_NEW_WIN_ID),
        WIN_TITLE(TXT_NEW),
#ifdef MMI_PDA_SUPPORT
        WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif /* MMI_PDA_SUPPORT */
        WIN_SOFTKEY(STXT_SAVE, TXT_NULL, STXT_RETURN),
#ifndef MMI_GUI_STYLE_TYPICAL
        CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIWLAN_LIST_AP_EDIT_CTRL_ID),
#else
        CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIWIFI_EDIT_AP_FORMCTRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_CHILD_FORM_SSID_CTRL_ID,MMIWIFI_EDIT_AP_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_EDIT_SSID_LABEL_CTRL_ID, MMIWIFI_CHILD_FORM_SSID_CTRL_ID),//网络名称SSID
        CHILD_EDIT_TEXT_CTRL(TRUE, WIFISUPP_SSID_MAX_LEN, MMIWIFI_EDIT_SSID_DATA_EDIT_CTRL_ID, MMIWIFI_CHILD_FORM_SSID_CTRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_CHILD_FORM_NETWORK_STATE_CTRL_ID,MMIWIFI_EDIT_AP_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_EDIT_NETWORK_STATE_LABEL_CTRL_ID, MMIWIFI_CHILD_FORM_NETWORK_STATE_CTRL_ID),//网络状态
        CHILD_DROPDOWNLIST_CTRL(TRUE, MMIWIFI_EDIT_NETWORK_STATE_DROPDOWNLIST_CTRL_ID, MMIWIFI_CHILD_FORM_NETWORK_STATE_CTRL_ID),

        CHILD_FORM_CTRL(FALSE,GUIFORM_LAYOUT_ORDER,MMIWIFI_CHILD_FORM_NETWORK_TYPE_CTRL_ID,MMIWIFI_EDIT_AP_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_EDIT_NETWORK_TYPE_LABEL_CTRL_ID, MMIWIFI_CHILD_FORM_NETWORK_TYPE_CTRL_ID),//WLAN网络模式
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, MMIWIFI_EDIT_NETWORK_TYPE_DATA_LABEL_CTRL_ID, MMIWIFI_CHILD_FORM_NETWORK_TYPE_CTRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_CHILD_FORM_AUTH_TYPE_CTRL_ID,MMIWIFI_EDIT_AP_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_EDIT_AUTH_TYPE_LABEL_CTRL_ID, MMIWIFI_CHILD_FORM_AUTH_TYPE_CTRL_ID),//WLAN安全模式
        CHILD_DROPDOWNLIST_CTRL(TRUE,MMIWIFI_EDIT_AUTH_TYPE_DROPDOWNLIST_CTRL_ID,MMIWIFI_CHILD_FORM_AUTH_TYPE_CTRL_ID),

#ifdef WLAN_EAP_SUPPORT
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_CHILD_FORM_EAP_SIM_INDEX_CTRL_ID,MMIWIFI_EDIT_AP_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_EDIT_EAP_SIM_INDEX_LABEL_CTRL_ID, MMIWIFI_CHILD_FORM_EAP_SIM_INDEX_CTRL_ID),//EAP
        CHILD_DROPDOWNLIST_CTRL(TRUE,MMIWIFI_MENU_AP_EAP_SIM_INDEX_CTRL_ID,MMIWIFI_CHILD_FORM_EAP_SIM_INDEX_CTRL_ID),
#endif /* WLAN_EAP_SUPPORT */

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_CHILD_FORM_AUTH_SETTING_CTRL_ID,MMIWIFI_EDIT_AP_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, MMIWIFI_EDIT_AUTH_SETTING_LABEL_CTRL_ID, MMIWIFI_CHILD_FORM_AUTH_SETTING_CTRL_ID),//WLAN安全设置

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_CHILD_FORM_CONNECT_STATE_CTRL_ID,MMIWIFI_EDIT_AP_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_EDIT_CONNECT_STATE_LABEL_CTRL_ID, MMIWIFI_CHILD_FORM_CONNECT_STATE_CTRL_ID),//连接状态
        CHILD_DROPDOWNLIST_CTRL(TRUE,  MMIWIFI_EDIT_CONNECT_STATE_DROPDOWNLIST_CTRL_ID, MMIWIFI_CHILD_FORM_CONNECT_STATE_CTRL_ID),

        WIN_SOFTKEY(STXT_SAVE, TXT_NULL, STXT_RETURN),
#endif /* MMI_GUI_STYLE_TYPICAL */
        END_WIN
};

//the wlan auth setting win---WEP
WINDOW_TABLE(MMIWIFI_AUTH_SETTING_WEP_WIN_TAB) = 
{
    WIN_FUNC((uint32)WifiHandleAuthSettingWEPWinMsg),    
        WIN_ID(MMIWIFI_AUTH_SETTING_WEP_WIN_ID),
        WIN_TITLE(TXT_COMM_AUTH_SETTING),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif /* MMI_PDA_SUPPORT */
        CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIWIFI_EDIT_WEP_FORMCTRL_ID),

#ifndef MMI_GUI_STYLE_TYPICAL
#ifdef MMI_WLAN_AUTHORITY_COMPLEX
        CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, MMIWLAN_LIST_WEP_STATE_CTRL_ID, MMIWIFI_EDIT_WEP_FORMCTRL_ID),
#endif /* MMI_WLAN_AUTHORITY_COMPLEX */
#else
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_CHILD_FORM_MODEL_CTRL_ID,MMIWIFI_EDIT_WEP_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_EDIT_WEP_KEY_OPEN_LABEL_CTRL_ID, MMIWIFI_CHILD_FORM_MODEL_CTRL_ID),//Open
        CHILD_DROPDOWNLIST_CTRL(TRUE, MMIWIFI_EDIT_WEP_KEY_OPEN_DROPDOWNLIST_CTRL_ID, MMIWIFI_CHILD_FORM_MODEL_CTRL_ID),    

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_CHILD_FORM_KEY_INDEX_CTRL_ID,MMIWIFI_EDIT_WEP_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_EDIT_WEP_KEY_INDEX_LABEL_CTRL_ID, MMIWIFI_CHILD_FORM_KEY_INDEX_CTRL_ID),//选择的WEP密钥
        CHILD_DROPDOWNLIST_CTRL(TRUE, MMIWIFI_EDIT_WEP_KEY_INDEX_DROPDOWNLIST_CTRL_ID, MMIWIFI_CHILD_FORM_KEY_INDEX_CTRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_CHILD_FORM_ENCRYPT_CTRL_ID,MMIWIFI_EDIT_WEP_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_EDIT_WEP_ENCRYPT_LABEL_CTRL_ID, MMIWIFI_CHILD_FORM_ENCRYPT_CTRL_ID),//WEP加密
        CHILD_DROPDOWNLIST_CTRL(TRUE,MMIWIFI_EDIT_WEP_ENCRYPT_DROPDOWNLIST_CTRL_ID,MMIWIFI_CHILD_FORM_ENCRYPT_CTRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_CHILD_FORM_STATE_CTRL_ID,MMIWIFI_EDIT_WEP_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_EDIT_WEP_KEY_STATE_LABEL_CTRL_ID, MMIWIFI_CHILD_FORM_STATE_CTRL_ID),//WEP密钥格式
        CHILD_DROPDOWNLIST_CTRL(TRUE,  MMIWIFI_EDIT_WEP_KEY_STATE_DROPDOWNLIST_CTRL_ID, MMIWIFI_CHILD_FORM_STATE_CTRL_ID),
#endif /* MMI_GUI_STYLE_TYPICAL */
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_CHILD_FORM_KEY_CTRL_ID,MMIWIFI_EDIT_WEP_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_EDIT_WEP_KEY_LABEL_CTRL_ID, MMIWIFI_CHILD_FORM_KEY_CTRL_ID),//WEP密钥
        CHILD_EDIT_PASSWORD_CTRL(TRUE, MMIWIFI_WEP_64BIT_KEY_LEN, MMIWIFI_EDIT_WEP_KEY_EDIT_CTRL_ID, MMIWIFI_CHILD_FORM_KEY_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};

//the wlan auth setting win---WEP  for connect
WINDOW_TABLE(MMIWIFI_AUTH_SETTING_WEP_FOR_CONNECT_WIN_TAB) = 
{
    WIN_FUNC((uint32)WifiHandleAuthSettingWEPForConnectWinMsg),    
        WIN_ID(MMIWIFI_AUTH_SETTING_WEP_FOR_CONNECT_WIN_ID),
        WIN_TITLE(TXT_COMM_AUTH_SETTING),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif /* MMI_PDA_SUPPORT */
        CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIWIFI_EDIT_WEP_FORMCTRL_ID),

#ifndef MMI_GUI_STYLE_TYPICAL
#ifdef MMI_WLAN_AUTHORITY_COMPLEX
        CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, MMIWLAN_LIST_WEP_STATE_CTRL_ID, MMIWIFI_EDIT_WEP_FORMCTRL_ID),
#endif /* MMI_WLAN_AUTHORITY_COMPLEX */
#else
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_CHILD_FORM_MODEL_CTRL_ID,MMIWIFI_EDIT_WEP_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_EDIT_WEP_KEY_OPEN_LABEL_CTRL_ID, MMIWIFI_CHILD_FORM_MODEL_CTRL_ID),//Open
        CHILD_DROPDOWNLIST_CTRL(TRUE, MMIWIFI_EDIT_WEP_KEY_OPEN_DROPDOWNLIST_CTRL_ID, MMIWIFI_CHILD_FORM_MODEL_CTRL_ID),    

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_CHILD_FORM_KEY_INDEX_CTRL_ID,MMIWIFI_EDIT_WEP_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_EDIT_WEP_KEY_INDEX_LABEL_CTRL_ID, MMIWIFI_CHILD_FORM_KEY_INDEX_CTRL_ID),//选择的WEP密钥
        CHILD_DROPDOWNLIST_CTRL(TRUE, MMIWIFI_EDIT_WEP_KEY_INDEX_DROPDOWNLIST_CTRL_ID, MMIWIFI_CHILD_FORM_KEY_INDEX_CTRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_CHILD_FORM_ENCRYPT_CTRL_ID,MMIWIFI_EDIT_WEP_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_EDIT_WEP_ENCRYPT_LABEL_CTRL_ID, MMIWIFI_CHILD_FORM_ENCRYPT_CTRL_ID),//WEP加密
        CHILD_DROPDOWNLIST_CTRL(TRUE,MMIWIFI_EDIT_WEP_ENCRYPT_DROPDOWNLIST_CTRL_ID,MMIWIFI_CHILD_FORM_ENCRYPT_CTRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_CHILD_FORM_STATE_CTRL_ID,MMIWIFI_EDIT_WEP_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_EDIT_WEP_KEY_STATE_LABEL_CTRL_ID, MMIWIFI_CHILD_FORM_STATE_CTRL_ID),//WEP密钥格式
        CHILD_DROPDOWNLIST_CTRL(TRUE,  MMIWIFI_EDIT_WEP_KEY_STATE_DROPDOWNLIST_CTRL_ID, MMIWIFI_CHILD_FORM_STATE_CTRL_ID),
#endif /* MMI_GUI_STYLE_TYPICAL */
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_CHILD_FORM_KEY_CTRL_ID,MMIWIFI_EDIT_WEP_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_EDIT_WEP_KEY_LABEL_CTRL_ID, MMIWIFI_CHILD_FORM_KEY_CTRL_ID),//WEP密钥
        CHILD_EDIT_PASSWORD_CTRL(TRUE, MMIWIFI_WEP_64BIT_KEY_LEN, MMIWIFI_EDIT_WEP_KEY_EDIT_CTRL_ID, MMIWIFI_CHILD_FORM_KEY_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};

//the wlan auth setting win---WPA/WPA2
WINDOW_TABLE(MMIWIFI_AUTH_SETTING_WPA_WPA2_WIN_TAB) = 
{
    WIN_FUNC((uint32)WifiHandleAuthSettingWPAWPA2WinMsg),    
        WIN_ID(MMIWIFI_AUTH_SETTING_WPA_WPA2_WIN_ID),
        WIN_TITLE(TXT_COMM_AUTH_SETTING),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif /* MMI_PDA_SUPPORT */
        CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIWIFI_EDIT_WPA_WPA2_FORMCTRL_ID),

#ifndef MMI_GUI_STYLE_TYPICAL
#ifdef MMI_WLAN_AUTHORITY_COMPLEX
        CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, MMIWLAN_LIST_WPA_STATE_CTRL_ID, MMIWIFI_EDIT_WPA_WPA2_FORMCTRL_ID),
#endif /* MMI_WLAN_AUTHORITY_COMPLEX */
#else
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_CHILD_FORM_PRESHAREKEY_STATE_CTRL_ID,MMIWIFI_EDIT_WPA_WPA2_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_EDIT_WPA_WPA2_PRESHAREKEY_STATE_LABEL_CTRL_ID, MMIWIFI_CHILD_FORM_PRESHAREKEY_STATE_CTRL_ID),//预共享密钥格式
        CHILD_DROPDOWNLIST_CTRL(TRUE,  MMIWIFI_EDIT_WPA_WPA2_PRESHAREKEY_STATE_DROPDOWNLIST_CTRL_ID, MMIWIFI_CHILD_FORM_PRESHAREKEY_STATE_CTRL_ID),
#endif /* MMI_GUI_STYLE_TYPICAL */
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_CHILD_FORM_PRESHAREKEY_PASSWORD_CTRL_ID,MMIWIFI_EDIT_WPA_WPA2_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_EDIT_WPA_WPA2_PRESHAREKEY_LABEL_CTRL_ID, MMIWIFI_CHILD_FORM_PRESHAREKEY_PASSWORD_CTRL_ID),//预共享密钥
        CHILD_EDIT_PASSWORD_CTRL(TRUE, WIFISUPP_WPA_PSK_LEN, MMIWIFI_EDIT_WPA_WPA2_PRESHAREKEY_EDIT_CTRL_ID, MMIWIFI_CHILD_FORM_PRESHAREKEY_PASSWORD_CTRL_ID),

        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};

//the wlan auth setting win---WPA/WPA2 for connect
WINDOW_TABLE(MMIWIFI_AUTH_SETTING_WPA_WPA2_FOR_CONNECT_WIN_TAB) = 
{
    WIN_FUNC((uint32)WifiHandleAuthSettingWPAWPA2ForConnectWinMsg),    
        WIN_ID(MMIWIFI_AUTH_SETTING_WPA_WPA2_FOR_CONNECT_WIN_ID),
        WIN_TITLE(TXT_COMM_AUTH_SETTING),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif /* MMI_PDA_SUPPORT */
        CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIWIFI_EDIT_WPA_WPA2_FORMCTRL_ID),

#ifndef MMI_GUI_STYLE_TYPICAL
#ifdef MMI_WLAN_AUTHORITY_COMPLEX
        CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, MMIWLAN_LIST_WPA_STATE_CTRL_ID, MMIWIFI_EDIT_WPA_WPA2_FORMCTRL_ID),
#endif /* MMI_WLAN_AUTHORITY_COMPLEX */
#else
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_CHILD_FORM_PRESHAREKEY_STATE_CTRL_ID,MMIWIFI_EDIT_WPA_WPA2_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_EDIT_WPA_WPA2_PRESHAREKEY_STATE_LABEL_CTRL_ID, MMIWIFI_CHILD_FORM_PRESHAREKEY_STATE_CTRL_ID),//预共享密钥格式
        CHILD_DROPDOWNLIST_CTRL(TRUE,  MMIWIFI_EDIT_WPA_WPA2_PRESHAREKEY_STATE_DROPDOWNLIST_CTRL_ID, MMIWIFI_CHILD_FORM_PRESHAREKEY_STATE_CTRL_ID),
#endif /* MMI_GUI_STYLE_TYPICAL */
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_CHILD_FORM_PRESHAREKEY_PASSWORD_CTRL_ID,MMIWIFI_EDIT_WPA_WPA2_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_EDIT_WPA_WPA2_PRESHAREKEY_LABEL_CTRL_ID, MMIWIFI_CHILD_FORM_PRESHAREKEY_PASSWORD_CTRL_ID),//预共享密钥
        CHILD_EDIT_PASSWORD_CTRL(TRUE, WIFISUPP_WPA_PSK_LEN, MMIWIFI_EDIT_WPA_WPA2_PRESHAREKEY_EDIT_CTRL_ID, MMIWIFI_CHILD_FORM_PRESHAREKEY_PASSWORD_CTRL_ID),

        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};

//the wlan auth setting win---WAPI
WINDOW_TABLE(MMIWIFI_AUTH_SETTING_WAPI_WIN_TAB) = 
{
    WIN_FUNC((uint32)WifiHandleAuthSettingWAPIWinMsg),    
        WIN_ID(MMIWIFI_AUTH_SETTING_WAPI_WIN_ID),
        WIN_TITLE(TXT_COMM_AUTH_SETTING),
              CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMIWIFI_EDIT_WAPI_LABEL_ID),
              CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIWIFI_EDIT_WAPI_LISTBOX_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};

//the wlan auth setting win---WAPI_PSK
WINDOW_TABLE(MMIWIFI_AUTH_SETTING_WAPI_PSK_WIN_TAB) = 
{
    WIN_FUNC((uint32)WifiHandleAuthSettingWAPIPskWinMsg),    
        WIN_ID(MMIWIFI_AUTH_SETTING_WAPI_PSK_WIN_ID),
        WIN_TITLE(TXT_COMM_AUTH_SETTING),
        CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIWIFI_EDIT_WAPI_PSK_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_EDIT_WAPI_PRESHAREKEY_STATE_LABEL_CTRL_ID, MMIWIFI_EDIT_WAPI_PSK_FORMCTRL_ID),//预共享密钥格式
        CHILD_DROPDOWNLIST_CTRL(TRUE,  MMIWIFI_EDIT_WAPI_PRESHAREKEY_STATE_DROPDOWNLIST_CTRL_ID, MMIWIFI_EDIT_WAPI_PSK_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_EDIT_WAPI_PRESHAREKEY_LABEL_CTRL_ID, MMIWIFI_EDIT_WAPI_PSK_FORMCTRL_ID),//预共享密钥
        CHILD_EDIT_PASSWORD_CTRL(TRUE, WIFISUPP_WPA_PSK_LEN, MMIWIFI_EDIT_WAPI_PRESHAREKEY_EDIT_CTRL_ID, MMIWIFI_EDIT_WAPI_PSK_FORMCTRL_ID),
              WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};
    
//the wlan auth setting win---WAPI_PSK FOR CONNECT
WINDOW_TABLE(MMIWIFI_AUTH_SETTING_WAPI_PSK_FOR_CONNECT_WIN_TAB) = 
{
    WIN_FUNC((uint32)WifiHandleAuthSettingWAPIPskForConnectWinMsg),    
        WIN_ID(MMIWIFI_AUTH_SETTING_WAPI_PSK_FOR_CONNECT_WIN_ID),
        WIN_TITLE(TXT_COMM_AUTH_SETTING),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif /* MMI_PDA_SUPPORT */
        CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIWIFI_EDIT_WAPI_PSK_FORMCTRL_ID),

#ifndef MMI_GUI_STYLE_TYPICAL
#ifdef MMI_WLAN_AUTHORITY_COMPLEX
        CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, MMIWLAN_LIST_WAPI_STATE_CTRL_ID, MMIWIFI_EDIT_WAPI_PSK_FORMCTRL_ID),
#endif /* MMI_WLAN_AUTHORITY_COMPLEX */
#else
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_CHILD_WAPI_STATE_CTRL_ID,MMIWIFI_EDIT_WAPI_PSK_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_EDIT_WAPI_PRESHAREKEY_STATE_LABEL_CTRL_ID, MMIWIFI_CHILD_WAPI_STATE_CTRL_ID),//预共享密钥格式
        CHILD_DROPDOWNLIST_CTRL(TRUE,  MMIWIFI_EDIT_WAPI_PRESHAREKEY_STATE_DROPDOWNLIST_CTRL_ID, MMIWIFI_CHILD_WAPI_STATE_CTRL_ID),
#endif /* MMI_GUI_STYLE_TYPICAL */
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_CHILD_FORM_WAPI_PASSWORD_CTRL_ID,MMIWIFI_EDIT_WAPI_PSK_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_EDIT_WAPI_PRESHAREKEY_LABEL_CTRL_ID, MMIWIFI_CHILD_FORM_WAPI_PASSWORD_CTRL_ID),//预共享密钥
        CHILD_EDIT_PASSWORD_CTRL(TRUE, WIFISUPP_WPA_PSK_LEN, MMIWIFI_EDIT_WAPI_PRESHAREKEY_EDIT_CTRL_ID, MMIWIFI_CHILD_FORM_WAPI_PASSWORD_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};

//the wlan auth setting win---WAPI_certificate 
WINDOW_TABLE(MMIWIFI_AUTH_SETTING_WAPI_CERTIFICATE_WIN_TAB) = 
{
    WIN_FUNC((uint32)WifiHandleAuthSettingWAPICERTIFICATEWinMsg),    
        WIN_ID(MMIWIFI_AUTH_SETTING_WAPI_CERTIFICATE_WIN_ID),
        WIN_TITLE(TXT_COMM_AUTH_SETTING),
            CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIWIFI_EDIT_WAPI_CERTIFICATE_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_EDIT_WAPI_CLIENT_CERTIFICATE_TEXT_LABEL_CTRL_ID, MMIWIFI_EDIT_WAPI_CERTIFICATE_FORMCTRL_ID),//WAPI客户端证书
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, MMIWIFI_EDIT_WAPI_CLIENT_CERTIFICATE_LABEL_CTRL_ID, MMIWIFI_EDIT_WAPI_CERTIFICATE_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_EDIT_WAPI_PARENT_CERTIFICATE_TEXT_LABEL_CTRL_ID, MMIWIFI_EDIT_WAPI_CERTIFICATE_FORMCTRL_ID),//WAPI根证书
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, MMIWIFI_EDIT_WAPI_PARENT_CERTIFICATE_LBAEL_CTRL_ID, MMIWIFI_EDIT_WAPI_CERTIFICATE_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, MMIWIFI_LABEL_WAPI_UNINSTALL_CTRL_ID, MMIWIFI_EDIT_WAPI_CERTIFICATE_FORMCTRL_ID),        
        WIN_SOFTKEY(TXT_COMMON_OK,TXT_WIFI_SELECT_PATH,STXT_RETURN),
        END_WIN
};

//the wlan auth setting win---WAPI_certificate  for connect
WINDOW_TABLE(MMIWIFI_AUTH_SETTING_WAPI_CERTIFICATE_FOR_CONNECT_WIN_TAB) = 
{
    WIN_FUNC((uint32)WifiHandleAuthSettingWAPICERTIFICATEForConnectWinMsg),    
        WIN_ID(MMIWIFI_AUTH_SETTING_WAPI_CERT_CONNECT_WIN_ID),
        WIN_TITLE(TXT_COMM_AUTH_SETTING),
#ifdef MMI_PDA_SUPPORT
        WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif /* MMI_PDA_SUPPORT */
        CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIWIFI_EDIT_WAPI_CERTIFICATE_FORMCTRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_CHILD_FORM_CLIENT_CTRL_ID,MMIWIFI_EDIT_WAPI_CERTIFICATE_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_EDIT_WAPI_CLIENT_CERTIFICATE_TEXT_LABEL_CTRL_ID, MMIWIFI_CHILD_FORM_CLIENT_CTRL_ID),//WAPI客户端证书
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, MMIWIFI_EDIT_WAPI_CLIENT_CERTIFICATE_LABEL_CTRL_ID, MMIWIFI_CHILD_FORM_CLIENT_CTRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_CHILD_FORM_PARENT_CTRL_ID,MMIWIFI_EDIT_WAPI_CERTIFICATE_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIFI_EDIT_WAPI_PARENT_CERTIFICATE_TEXT_LABEL_CTRL_ID, MMIWIFI_CHILD_FORM_PARENT_CTRL_ID),//WAPI根证书
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, MMIWIFI_EDIT_WAPI_PARENT_CERTIFICATE_LBAEL_CTRL_ID, MMIWIFI_CHILD_FORM_PARENT_CTRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIFI_CHILD_FORM_UNINSTALL_CTRL_ID,MMIWIFI_EDIT_WAPI_CERTIFICATE_FORMCTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, MMIWIFI_LABEL_WAPI_UNINSTALL_CTRL_ID, MMIWIFI_CHILD_FORM_UNINSTALL_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
        WIN_SOFTKEY(STXT_SAVE,TXT_NULL,STXT_RETURN),
#else
        WIN_SOFTKEY(STXT_SAVE,TXT_WIFI_SELECT_PATH,STXT_RETURN),
#endif /* MMI_PDA_SUPPORT */
        END_WIN
};

WINDOW_TABLE(MMIWIFI_LIST_POPMENU_WIN_TAB) = 
{
    WIN_FUNC((uint32)WifiHandlelistPopMenuWinMsg),
        WIN_ID(MMIWIFI_POPMENU_WIN_ID),
        WIN_STYLE(WS_HAS_TRANSPARENT),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
//        CREATE_POPMENU_CTRL(MMIWIFI_LIST_MENU, MMIWIFI_MENU_CTRL_ID),
        END_WIN
};

//the wifi list win
WINDOW_TABLE(MMIWIFI_SELECT_AP_WIN_TAB) = 
{
    WIN_FUNC((uint32)WifiHandleSelectAPWinMsg),    
    WIN_ID(MMIWIFI_SELECT_AP_WIN_ID),
    WIN_STATUSBAR,
    WIN_TITLE(TXT_WLAN_SELECT_AP),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIWIFI_SELECT_AP_LIST_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_CONNECT_COMN, TXT_NULL, STXT_RETURN),
    END_WIN
};

/**-------------------------------------------------------------------------*
**                         LOCAL FUNCTION DECLARE                           *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Discription: Handle searching new device waiting win msg
//  Global resource dependence: none 
//  Author: li.li 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleCancelScanWaitWinMsg(
                                                    MMI_WIN_ID_T        win_id, 
                                                    MMI_MESSAGE_ID_E    msg_id, 
                                                    DPARAM              param 
                                                    )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;   
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIDEFAULT_AllowOpenKeylocWin(FALSE);
        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_CANCEL, FALSE);
        break;
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        //if we want cancel scan, the ui close waiting win, but the wifi is still scaning
        MMK_CloseWin( win_id );     
        break;
    case MSG_CLOSE_WINDOW:
        MMIDEFAULT_AllowOpenKeylocWin(TRUE);
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    default:
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }
    
    return result;  
}

/****************************************************************************/
//  Description : wifi on function
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIAPIWIFI_GetIsWlanOn(void)
{
    return MMIWIFI_GetIsWlanOn();
}

/****************************************************************************/
//  Description : wifi on function
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIAPIWIFI_On(BOOLEAN is_need_wait_win)
{
    if(MMIAPISET_GetFlyMode()&& (FALSE == MMIAPIENG_GetIsFlightModeWifi()))
    {
#ifdef ADULT_WATCH_SUPPORT
        MMI_STRING_T    text_str = {0};
        MMI_GetLabelTextByLang( TXT_CLOSE_FLY_MODE_FIRST, &text_str );
        Adult_WatchCOM_NoteWin_1Line_Toast_Enter( MMIWIFI_CLOSE_FLY_MODE_WIN_ID,&text_str,PNULL,PNULL);
#else
        MMIPUB_OpenAlertWarningWin(TXT_CLOSE_FLY_MODE_FIRST); 
#endif
        return FALSE;
    }
    
    if (is_need_wait_win)
    {
        WifiSetUiOption(MMIWIFI_UI_OPTION_PROMPT);
        MMIWIFI_OpenOnWaitWin();
    }

    return MMIWIFI_On();
}

/****************************************************************************/
//  Description : open wifi main window
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC void MMIAPIWIFI_OpenMainWin(void)
{
#ifdef MMI_WLAN_TAB_STYLE
    MMIWIFI_OpenMainWin(MMIWIFI_ENTRY_TYPE_LIST);
#else
    MMK_CreateWin((uint32*)MMIWIFI_LIST_WIN_TAB, PNULL);
#endif /* MMI_WLAN_TAB_STYLE */
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note: Jiaoyou.wu
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleAuthSettingWAPIWinMsg(
                                                    MMI_WIN_ID_T    win_id, 
                                                    MMI_MESSAGE_ID_E   msg_id, 
                                                    DPARAM             param
                                                    )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        WifiSetEditWAPIWindow();
        MMK_SetAtvCtrl(win_id,MMIWIFI_EDIT_WAPI_LISTBOX_ID);
        break;        

    case MSG_FULL_PAINT:
        break;
        
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
        {
            uint16 cur_sel = 0;
            
            WifiSetEncryptProtocol(WIFISUPP_ENCRYP_PROTOCOL_WAPI);

            cur_sel = GUILIST_GetCurItemIndex(MMIWIFI_EDIT_WAPI_LISTBOX_ID);
            if(cur_sel != WifiGetWapiCredential())
            {
                WifiResetWAPICredential();
            }

            if(0 == GUILIST_GetCurItemIndex(MMIWIFI_EDIT_WAPI_LISTBOX_ID))
            {
                MMK_CreateWin((uint32 *)MMIWIFI_AUTH_SETTING_WAPI_PSK_WIN_TAB, PNULL);
            }
            else
            {
                MMK_CreateWin((uint32 *)MMIWIFI_AUTH_SETTING_WAPI_CERTIFICATE_WIN_TAB, PNULL);
            }
            MMK_CloseWin( win_id );      
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );      
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/****************************************************************************/
//  Description : open wifi edit window
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_OpenEditWin(MMIWIFI_PROFILE_T *profile_ptr)
{
    MMK_CreateWin((uint32*)MMIWIFI_EDIT_WIN_TAB, (ADD_DATA)profile_ptr);
}

/****************************************************************************/
//  Description : open wifi new window
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_OpenNewWin(void)
{
    MMK_CreateWin((uint32*)MMIWIFI_NEW_WIN_TAB, PNULL);
}

/****************************************************************************/
//  Description : open wifi connect wait window
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_OpenConnectWaitWin(void)
{
    MMI_STRING_T text_str = {0};

    MMI_GetLabelTextByLang(TXT_WIFI_CONNECTING, &text_str);
#ifdef ADULT_WATCH_SUPPORT
    AdultWatchCOM_WaitingWin_1Line_Enter(MMIWIFI_CONNECT_WAIT_WIN_ID,&text_str,0,(MMIPUB_HANDLE_FUNC)WifiHandleCancelConnectWaitWinMsg);//Bug 1587060
#else
    MMIPUB_OpenWaitWin(1, &text_str, PNULL, PNULL, MMIWIFI_CONNECT_WAIT_WIN_ID,IMAGE_NULL,
        ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_ONE,(MMIPUB_HANDLE_FUNC)WifiHandleCancelConnectWaitWinMsg);
#endif
}

/****************************************************************************/
//  Description : open wifi open wait window
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_OpenOnWaitWin(void)
{
    MMI_STRING_T    prompt_str = {0};
#ifdef ADULT_WATCH_SUPPORT
    MMI_GetLabelTextByLang(TXT_COMMON_SEARCHING, &prompt_str);
    AdultWatchCOM_WaitingWin_1Line_Enter(MMIWIFI_OPEN_WAIT_WIN_ID,&prompt_str,0,PNULL);//1731721
#else
    //开启等待窗口
    MMI_GetLabelTextByLang(TXT_OPENING_WLAN, &prompt_str);
    MMIPUB_OpenWaitWin(1,&prompt_str,PNULL,PNULL,MMIWIFI_OPEN_WAIT_WIN_ID,IMAGE_NULL,
            ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,PNULL);
#endif
}
/****************************************************************************/
//  Description : open wifi scan wait window
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_OpenScanWaitWin(void)
{
    MMI_STRING_T text_str = {0};
    
    MMI_GetLabelTextByLang(TXT_SCANING_WLAN, &text_str);
#ifdef ADULT_WATCH_SUPPORT
    AdultWatchCOM_WaitingWin_1Line_Enter(MMIWIFI_SCAN_WAIT_WIN_ID,&text_str,IMAGE_NULL,(MMIPUB_HANDLE_FUNC)WifiHandleCancelScanWaitWinMsg);
#else

    MMIPUB_OpenWaitWin(1, &text_str, PNULL, PNULL, MMIWIFI_SCAN_WAIT_WIN_ID,IMAGE_NULL,
            ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_ONE,(MMIPUB_HANDLE_FUNC)WifiHandleCancelScanWaitWinMsg);
#endif
}

/****************************************************************************/
//  Description : open wifi list window
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC void MMIAPIWIFI_OpenListWin(void)
{
//    MMI_WIN_ID_T query_win_id = MMIWIFI_QUERY_IFON_WIN_ID;

    if(MMIAPISET_GetFlyMode() && (FALSE == MMIAPIENG_GetIsFlightModeWifi()) )
    {
        MMIPUB_OpenAlertWarningWin(TXT_CLOSE_FLY_MODE_FIRST); 
        return;
    }

    //initialize wifi profile
    if (!MMIWIFI_GetIsInitialized())
    {
        if (!MMIWIFI_ProfileInitialize())
        {
            //SCI_TRACE_LOW:"WIFI:error:%s Profile Initialize faild!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_3303_112_2_18_3_9_58_176,(uint8*)"s", __func__);
            return;
        }
        
        MMIWIFI_SetIsInitialized(TRUE);
    }

    MMIWIFI_OpenMainWin(MMIWIFI_ENTRY_TYPE_LIST);
}

/****************************************************************************/
//  Description : open wifi wep key input for connect window
//  Global resource dependence : 
//  Author:
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_OpenWepForConnWin(void)
{
    MMK_CreateWin((uint32*)MMIWIFI_AUTH_SETTING_WEP_FOR_CONNECT_WIN_TAB, PNULL);
}

/****************************************************************************/
//  Description : open wifi WPA key input for connect window
//  Global resource dependence : 
//  Author:
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_OpenWPAForConnWin(void)
{
    MMK_CreateWin((uint32*)MMIWIFI_AUTH_SETTING_WPA_WPA2_FOR_CONNECT_WIN_TAB, PNULL);
}

/****************************************************************************/
//  Description : open wifi wapi psk input for connect window
//  Global resource dependence : 
//  Author:
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_OpenWAPIPskForConnWin(void)
{
    MMK_CreateWin((uint32*)MMIWIFI_AUTH_SETTING_WAPI_PSK_FOR_CONNECT_WIN_TAB, PNULL);
}

/****************************************************************************/
//  Description : open wifi wapi certificate input for connect window
//  Global resource dependence : 
//  Author:
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_OpenWAPICertificateForConnWin(void)
{
    MMK_CreateWin((uint32*)MMIWIFI_AUTH_SETTING_WAPI_CERTIFICATE_FOR_CONNECT_WIN_TAB, PNULL);
}

/****************************************************************************/
//  Description : open select ap win
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_OpenSelectAPWin(void)
{
    MMK_CreateWin((uint32*)MMIWIFI_SELECT_AP_WIN_TAB, PNULL);
}
#ifdef WLAN_RESELECT_SUPPORT
/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_OpenSelectServiceWin(void)
{
    MMIWIFI_PROFILE_T * profile_ptr = PNULL;
    
    int64 tick_time = 0;
    MMI_STRING_T string_head = {0};
    MMI_STRING_T string_tail = {0};
    MMI_STRING_T ap_name = {0};
    MMI_STRING_T string_notify = {0};
    wchar wstr_ap_name[WIFISUPP_SSID_MAX_LEN + 1] = {0};
    wchar notify_str[WIFISUPP_SSID_MAX_LEN + MMIWIFI_FIND_AP_NOTFY_TEXT_MAX + 1] = {0};

    MMI_GPRSTOWLAN_RESELECT reselect = MMI_GPRSTOWLAN_ALWAYS_INQUIRY;

    reselect = MMIWIFI_GetGPRSToWlanReselect();
    tick_time = SCI_GetTickCount();

    //SCI_TRACE_LOW:"MMIWIFI_OpenSelectServiceWin reselect = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_3379_112_2_18_3_9_58_177,(uint8*)"d", reselect);

    profile_ptr = MMIWIFI_FindProfileByStatus(MMIWIFI_PROFILE_STATE_DHCPED);

    if (PNULL == profile_ptr)
    {
        profile_ptr = MMIWIFI_FindProfileByStatus(MMIWIFI_PROFILE_STATE_CONNECTED);
    }

    if (PNULL == profile_ptr)
    {
        //SCI_TRACE_LOW:"MMIWIFI_OpenSelectServiceWin No Profile exist"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_3390_112_2_18_3_9_58_178,(uint8*)"");
        return;
    }

    if (MMIWIFI_IsNeedServiceChange())
    {
        if (MMI_GPRSTOWLAN_ALWAYS_INQUIRY == reselect)
        {
            //SCI_TRACE_LOW:"MMIWIFI_OpenSelectServiceWin s_last_tick_time = %ld, tick_time = %ld"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_3398_112_2_18_3_9_58_179,(uint8*)"dd", s_last_tick_time, tick_time);
            if(MMIWIFI_GetPromptResult() || (!MMIWIFI_GetPromptResult() &&  (0 == s_last_tick_time || tick_time - s_last_tick_time > 3600000 || tick_time < s_last_tick_time) ))
            {
                ap_name.wstr_ptr = wstr_ap_name;
                ap_name.wstr_len = GUI_UTF8ToWstr(wstr_ap_name, WIFISUPP_SSID_MAX_LEN, profile_ptr->config_info.ssid.ssid, profile_ptr->config_info.ssid.ssid_len);
    
                MMIRES_GetText(TXT_WLAN_FIND_WLAN_NOTIFY_HEAD, PNULL, &string_head);
                MMIRES_GetText(TXT_WLAN_FIND_WLAN_NOTIFY_TAIL, PNULL, &string_tail);
    
                /* 发现WLAN信号TESTSSID,是否使用WLAN网络，使用该网络可能使部分数据业务中断 */
                if ((ap_name.wstr_len + string_head.wstr_len + string_tail.wstr_len) > (WIFISUPP_SSID_MAX_LEN + MMIWIFI_FIND_AP_NOTFY_TEXT_MAX)) /* Potect */
                {
                    string_notify.wstr_ptr = ap_name.wstr_ptr;
                    string_notify.wstr_len = ap_name.wstr_len;
                }
                else
                {
                    SCI_MEMCPY(notify_str, string_head.wstr_ptr, string_head.wstr_len * sizeof(wchar));
                    SCI_MEMCPY(notify_str + string_head.wstr_len, ap_name.wstr_ptr, ap_name.wstr_len * sizeof(wchar));
                    SCI_MEMCPY(notify_str + string_head.wstr_len + ap_name.wstr_len, string_tail.wstr_ptr, string_tail.wstr_len * sizeof(wchar));
                    string_notify.wstr_ptr = notify_str;
                    string_notify.wstr_len = MMIAPICOM_Wstrlen(notify_str);
                }  
                MMIPUB_OpenQueryWinByTextPtrEx(MMK_GetFirstAppletHandle(), &string_notify, PNULL, IMAGE_PUBWIN_QUERY, PNULL, MMIWIFI_HandleChangeService, (uint32)FALSE);              
                s_last_tick_time = tick_time;
            }


        }
        else if (MMI_GPRSTOWLAN_AUTO_RESELCET == reselect)
        {
            #ifdef DL_SUPPORT
            MMIAPIDL_HandleServiceChange(MMIPDP_INTERFACE_WIFI);
            #endif
        }
    }
}
#endif
/*****************************************************************************/
//  Discription: WifiCreateListPopMenu

//  Global resource dependence: None
//  Author: 
//  Note : 
//  Return: 
/*****************************************************************************/
LOCAL void WifiCreateListPopMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    MMI_TEXT_ID_T    text_id;
    MMI_STRING_T    kstring = {0};
    uint32          node_id = 0;
    uint16          i = 0;
    uint16          node_index = 0;
#ifndef MMI_PDA_SUPPORT
    MMIWIFI_PROFILE_T    *profile_ptr = PNULL;
#endif /* MMI_PDA_SUPPORT */
    uint16          list_index = 0;
    BOOLEAN         is_wlan_on = TRUE;

    uint32          wifi_menu[][3] ={
#ifdef MMI_PDA_SUPPORT
                                                        {MMIWIFI_POPMENU_ID_REFRESH, TXT_REFRESH, IMAGE_COMMON_OPTMENU_REFRESH},
#ifndef MMI_WLAN_TAB_STYLE
                                                        {MMIWIFI_POPMENU_ID_ADD_AP, TXT_WLAN_ADD_AP, IMAGE_COMMON_OPTMENU_ADD},
                                                        {MMIWIFI_POPMENU_ID_SETTING, STXT_MAIN_SETTING, IMAGE_COMMON_OPTMENU_SET},
#endif
#else
                                                        {MMIWIFI_POPMENU_ID_REFRESH, TXT_REFRESH, IMAGE_NULL},
#ifndef MMI_WLAN_TAB_STYLE
                                                        {MMIWIFI_POPMENU_ID_ADD_AP, TXT_WLAN_ADD_AP, IMAGE_NULL},
                                                        {MMIWIFI_POPMENU_ID_FORGET_AP, TXT_WLAN_FORGET_AP, IMAGE_NULL},
                                                        {MMIWIFI_POPMENU_ID_EDIT,   TXT_EDIT, IMAGE_NULL},
#endif

#ifdef WLAN_PORTAL_SUPPORT
                                                        {MMIWIFI_POPMENU_ID_DEFAULT_AP_SETTING, TXT_WIFI_DEFAULT_AP_SETTING,IMAGE_NULL},
#endif
                                                        {MMIWIFI_POPMENU_ID_DETAIL, STXT_DETAIL, IMAGE_NULL},
#ifndef MMI_WLAN_TAB_STYLE
                                                        {MMIWIFI_POPMENU_ID_SETTING, STXT_MAIN_SETTING, IMAGE_NULL},
#endif
#endif
                                                    };

    GUIMENU_DYNA_ITEM_T     node_item = {0};

    if(!MMK_IsOpenWin(win_id))
    {
        //SCI_TRACE_LOW:"WifiCreateListPopMenu window already opened !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_3484_112_2_18_3_9_59_180,(uint8*)"");
        return;
    }

    GUIMENU_CreatDynamic(PNULL,win_id,ctrl_id,GUIMENU_STYLE_POPUP);
    GUIMENU_SetDynamicMenuSoftkey(ctrl_id, TXT_COMMON_OK, TXT_NULL, STXT_RETURN);
    MMK_SetAtvCtrl(win_id, ctrl_id);

    list_index = GUILIST_GetCurItemIndex(MMIWIFI_LIST_CTRL_ID);
    is_wlan_on = MMIWIFI_GetIsWlanOn();

    for(i = 0; i < sizeof(wifi_menu)/sizeof(wifi_menu[0]); i++)
    {
        if (!is_wlan_on)
        {
            if (MMIWIFI_POPMENU_ID_SETTING != wifi_menu[i][0])
            {
                continue;
            }
        }

#ifndef MMI_PDA_SUPPORT

        if (WifiListIsAPItemSelected(list_index))
        {
            profile_ptr = (MMIWIFI_PROFILE_T *)WifiGetListUserdata(MMIWIFI_LIST_CTRL_ID, list_index);

#ifdef WLAN_PORTAL_SUPPORT
            if (MMIWIFI_APIsCMCC(profile_ptr))
            {
                if (MMIWIFI_POPMENU_ID_FORGET_AP  == wifi_menu[i][0]||
                    MMIWIFI_POPMENU_ID_EDIT == wifi_menu[i][0])
                {
                    continue;
                }
            }
            else
            {
                if (MMIWIFI_APIsCMCCEDU(profile_ptr))
                {
                    if (MMIWIFI_POPMENU_ID_FORGET_AP  == wifi_menu[i][0]||
                        MMIWIFI_POPMENU_ID_EDIT == wifi_menu[i][0])
                    {
                        continue;
                    }
                }

                if (MMIWIFI_POPMENU_ID_DEFAULT_AP_SETTING  == wifi_menu[i][0])
                {
                    continue;
                }
            }
#endif

            if (!(profile_ptr->state == MMIWIFI_PROFILE_STATE_DISCONNECTED || profile_ptr->state == MMIWIFI_PROFILE_STATE_DISCONNECTING))
            {
                if (MMIWIFI_POPMENU_ID_EDIT == wifi_menu[i][0])
                {
                    continue;
                }
            }

            if(!profile_ptr->is_trusted)
            {
                if (MMIWIFI_POPMENU_ID_FORGET_AP  == wifi_menu[i][0]||
                    MMIWIFI_POPMENU_ID_EDIT == wifi_menu[i][0])
                {
                    continue;
                }
            }

        }
        else
        {
            if (MMIWIFI_POPMENU_ID_FORGET_AP == wifi_menu[i][0] ||
                MMIWIFI_POPMENU_ID_EDIT == wifi_menu[i][0] ||
                MMIWIFI_POPMENU_ID_DEFAULT_AP_SETTING == wifi_menu[i][0] ||
                MMIWIFI_POPMENU_ID_DETAIL  == wifi_menu[i][0])
            {
                continue;
            }
        }
#endif

        node_id = wifi_menu[i][0];
        text_id = wifi_menu[i][1];
                          
        MMI_GetLabelTextByLang(text_id, &kstring);
        node_item.item_text_ptr = &kstring;
#ifdef MMI_PDA_SUPPORT
        node_item.select_icon_id = wifi_menu[i][2];
#endif /* MMI_PDA_SUPPORT */
        GUIMENU_InsertNode(node_index, node_id, 0, &node_item, ctrl_id);
        node_index ++;
    }
}

/*****************************************************************************/
//  Discription: WifiCreateRejectListPopMenu
//  Global resource dependence: None
//  Author: 
//  Note : 
//  Return: 
/*****************************************************************************/
LOCAL void WifiCreateRejectListPopMenu(
                            MMI_WIN_ID_T    win_id, //IN: list menu's window id
                            MMI_CTRL_ID_T   ctrl_id
                            )
{
    MMI_TEXT_ID_T    text_id;
    MMI_STRING_T    kstring = {0};
    uint32          node_id = 0;
    uint16          i = 0;
    uint16          node_index = 0;
#ifdef MMI_PDA_SUPPORT
    uint32          wifi_menu[][3] ={
                                                        {MMIWIFI_POPMENU_ID_ADD, STXT_ADD, IMAGE_COMMON_OPTMENU_ADD},
                                                        {MMIWIFI_POPMENU_ID_CLEAR, TXT_COMMON_CLEAR, IMAGE_COMMON_OPTMENU_REMOVE_ALL}
	};
#else
    uint32          wifi_menu[3][2] ={
                                                        {MMIWIFI_POPMENU_ID_ADD, STXT_ADD},
                                                        {MMIWIFI_POPMENU_ID_DELETE, TXT_DELETE},
                                                        {MMIWIFI_POPMENU_ID_CLEAR, TXT_COMMON_CLEAR}
	};
#endif /* MMI_PDA_SUPPORT */
                                                    
    GUIMENU_DYNA_ITEM_T     node_item = {0};

    GUIMENU_CreatDynamic(PNULL,win_id,ctrl_id,GUIMENU_STYLE_POPUP);
    GUIMENU_SetDynamicMenuSoftkey(ctrl_id, TXT_COMMON_OK, TXT_NULL, STXT_RETURN);
    MMK_SetAtvCtrl(win_id, ctrl_id);

    for(i = 0; i < sizeof(wifi_menu)/sizeof(wifi_menu[0]); i++)
    {
        node_id = wifi_menu[i][0];
        text_id = wifi_menu[i][1];
                            
        MMI_GetLabelTextByLang(text_id, &kstring);
        node_item.item_text_ptr = &kstring;
#ifdef MMI_PDA_SUPPORT
        node_item.select_icon_id = wifi_menu[i][2];
#endif /* MMI_PDA_SUPPORT */
        GUIMENU_InsertNode(node_index, node_id, 0, &node_item, ctrl_id);
        node_index ++;
    }

    /*if(0 == GUILIST_GetTotalItemNum(MMIWIFI_LIST_CTRL_ID))
    {
        GUIMENU_SetNodeGrayed(TRUE, 0, NULL, ctrl_id);//0对应连接/断开
        GUIMENU_SetNodeGrayed(TRUE, 4, NULL, ctrl_id);//4//4对应详情
    }
    else
    {
        GUIMENU_SetNodeGrayed(FALSE, 0, NULL, ctrl_id);
        GUIMENU_SetNodeGrayed(FALSE, 4, NULL, ctrl_id);
    }*/
 }

/*****************************************************************************/
//  Discription: wifi find rssi image
//  Global resource dependence: None
//  Author: 
//  Note : 
//  Return: 
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T WifiGetConnectedRssiImage( int32 rssi,BOOLEAN isEncrypt )
{
    MMI_IMAGE_ID_T image = IMAGE_NULL;

    SCI_TRACE_LOW("WifiGetConnectedRssiImage rssi = %d", rssi);

    if( rssi <= MMIWIFI_RSSI_20 )
    {
        image = res_stat_wifi_0;
    }
    else if( rssi <= MMIWIFI_RSSI_40 )
    {
        image = res_stat_wifi_1;
    }
    else if( rssi <= MMIWIFI_RSSI_60 )
    {
        image = res_stat_wifi_2;
    }
    else if( rssi <= MMIWIFI_RSSI_80 )
    {
        image = res_stat_wifi_3;
    }
    else 
    {
        image = res_stat_wifi_4;
    }

    return image;
}

/*****************************************************************************/
//  Discription: wifi find rssi image
//  Global resource dependence: None
//  Author: 
//  Note : 
//  Return: 
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T WifiFindRssiImage(int32 rssi)
{
    MMI_IMAGE_ID_T image = IMAGE_NULL;

    if(rssi <= MMIWIFI_RSSI_40)
    {
        image = IMAGE_WIFI_RSSI0;
    }
    else if (rssi <= MMIWIFI_RSSI_80)
    {
        image = IMAGE_WIFI_RSSI1;
    }
    else
    {
        image = IMAGE_WIFI_RSSI2;
    }
    
    return image;
}

/*****************************************************************************/
//  Discription: wifi handle manager msg
//  Global resource dependence: None
//  Author: 
//  Note : Modified by George.Liu 01526, 2010/9/2
//  Return: 
/*****************************************************************************/
LOCAL BOOLEAN WifiHandleManagerMsg(MMIWIFI_MSG_E message_id,void* param,uint16 size_of_param)
{
    BOOLEAN         result = TRUE;
    MMI_STRING_T    node_text = {0};
    MMIWIFI_PROFILE_T           *profile_ptr = PNULL;
    MMIWIFI_DHCP_CNF_T          *dhcp_ptr = PNULL;
    MMIWIFI_RSSI_CHANGED_IND_T  *rssi_ptr = PNULL;
    MMIWIFI_DISCONNECT_CNF_T    *disconnection_ptr = PNULL;
    MMIWIFI_CONNECT_CNF_T       *connect_ptr = PNULL;
    MMIWIFI_ON_CNF_T            *on_ptr = PNULL;
    MMIWIFI_OFF_CNF_T           *off_ptr = PNULL;
    MMIWIFI_OPTION_FOR_WIFION_E ui_option = MMIWIFI_UI_OPTION_MAX;
    MMI_IMAGE_ID_T image = 0;
#ifdef ADULT_WATCH_SUPPORT
    MMI_STRING_T     text_str = {0};
#endif

    if (PNULL == param)
    {
        TRACE_APP_WLAN("param is PNULL!!");
        return FALSE;
    }

    switch(message_id) 
    {
    case MMIWIFI_MSG_ON_CNF:
        on_ptr = (MMIWIFI_ON_CNF_T*)param;
        
        //SCI_TRACE_LOW:"WIFI:info:MMI recv MMIWIFI_MSG_ON_CNF, result is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_3726_112_2_18_3_9_59_181,(uint8*)"d", on_ptr->result);
        MMIWIFI_SetScanStatus(FALSE);
        MMK_CloseWin(MMIWIFI_OPEN_WAIT_WIN_ID);
        
        if (MMIWIFI_RESULT_RETRICTED == on_ptr->result)
        {
            MMIWIFI_SetIsWlanOn(FALSE);
#ifdef ADULT_WATCH_SUPPORT
            MMI_GetLabelTextByLang( TXT_WIFI_CONNECT_BUSY, &text_str );
            Adult_WatchCOM_NoteWin_1Line_Toast_Enter( MMIWIFI_CONNECT_BUSY_WIN_ID,&text_str,PNULL,PNULL);
#else
            MMIPUB_OpenAlertWarningWin(TXT_WIFI_CONNECT_BUSY);
#endif
            break;
        }

        if (on_ptr->result != MMIWIFI_RESULT_SUCC)
        {
            MMIWIFI_SetIsWlanOn(FALSE);
#ifdef ADULT_WATCH_SUPPORT
            MMI_GetLabelTextByLang( TXT_COMM_OPEN_FAIL, &text_str );
            Adult_WatchCOM_NoteWin_1Line_Toast_Enter( MMIWIFI_OPEN_FAIL_WIN_ID,&text_str,PNULL,PNULL);
#else
            MMIPUB_OpenAlertWarningWin(TXT_COMM_OPEN_FAIL);
#endif
            break;
        }
        MMIWIFI_SetIsWlanOffReqHandled(TRUE);

        ui_option = WifiGetUiOption();

        if (MMIWIFI_UI_OPTION_PROMPT == ui_option)
        {
#ifdef MMI_GUI_STYLE_TYPICAL
            MMIPUB_OpenAlertWinByTextId(PNULL, TXT_COMM_HAS_OPEN, 0,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
#endif        
        }
#ifdef WLAN_RESELECT_SUPPORT
        s_last_tick_time = 0;
#endif
        MMIWIFI_SetIsWlanOn(TRUE);
        MMIWIFI_StartAutoScan();
        break;

    case MMIWIFI_MSG_OFF_CNF:
        //SCI_TRACE_LOW:"WIFI:info:MMI recv MMIWIFI_MSG_OFF_CNF, result is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_3761_112_2_18_3_9_59_182,(uint8*)"d", ((MMIWIFI_OFF_CNF_T *)param)->result);
        if (MMK_IsFocusWin(MMIWIFI_LIST_WIN_ID))
        {
            MMK_PostMsg(MMIWIFI_LIST_CTRL_ID, MSG_FULL_PAINT, PNULL, 0);
        }
        else if (MMK_IsFocusWin(MMIWIFI_MAIN_WIN_ID))
        {
            MMK_PostMsg(MMIWIFI_MAIN_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
        }
        MMIWIFI_SetIsWlanOffReqHandled(TRUE);
        MMIWIFI_SetScanStatus(FALSE);
        MMIWIFI_StopAutoScan();
        off_ptr = (MMIWIFI_OFF_CNF_T*)param;
        if (off_ptr->result != MMIWIFI_RESULT_SUCC)
        {
#ifdef ADULT_WATCH_SUPPORT
            MMI_GetLabelTextByLang( TXT_WIFI_WLAN_COMMON_CLOSE_FAIL, &text_str );
            Adult_WatchCOM_NoteWin_1Line_Toast_Enter( MMIWIFI_CLOSE_FAIL_WIN_ID,&text_str,PNULL,PNULL);
#else
            MMIPUB_OpenAlertWarningWin(TXT_WIFI_WLAN_COMMON_CLOSE_FAIL);
#endif
            break;
        }
        if (!MMK_IsOpenWin(PHONE_POWER_OFF_WIN_ID))
        {
            MMIWIFI_SetIsWlanOn(FALSE);
#ifdef WLAN_RESELECT_SUPPORT
            if (!MMIWIFI_ChangeServiceToGPRS())
            {
                MMIAPIPDP_WifiDeactiveInd();
            } 
#else
						MMIAPIPDP_WifiDeactiveInd();
#endif         
        }
        else
        {
            MMIAPIPDP_WifiDeactiveInd();
        }
#ifdef WLAN_RESELECT_SUPPORT
        s_last_tick_time = 0;
#endif
#ifdef MMI_GUI_STYLE_TYPICAL
        MMIPUB_OpenAlertWinByTextId(PNULL, TXT_COMM_HAS_CLOSE, 0,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
#else
        MMK_PostMsg(MMIWIFI_SETTING_LIST_CHILD_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
#endif 
        MAIN_SetIdleWifiIcon(IMAGE_NULL);
        MMIWIFI_SetCurrProfile(PNULL);
        wifi_TidyWindow();

        break;

    case MMIWIFI_MSG_SCAN_CNF:
        //SCI_TRACE_LOW:"WIFI:info:MMI recv MMIWIFI_MSG_SCAN_CNF, result is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_3801_112_2_18_3_9_59_183,(uint8*)"d", ((MMIWIFI_SCAN_CNF_T *)param)->result);
        MMK_CloseWin(MMIWIFI_SCAN_WAIT_WIN_ID);       
        MMK_CloseWin(MMIWIFI_SPECTIFY_SEARCH_SSID_WIN_ID);
        
        if(MMIWIFI_GetSpecifiedScanStatus() && ((MMIWIFI_SCAN_CNF_T *)param)->result != MMIWIFI_RESULT_SUCC)
        {
#ifdef ADULT_WATCH_SUPPORT
            MMI_GetLabelTextByLang( TXT_WLAN_SCAN_FAIL, &text_str );
            Adult_WatchCOM_NoteWin_1Line_Toast_Enter( MMIWIFI_SCAN_FAIL_WIN_ID,&text_str,PNULL,PNULL);
#else
            MMIPUB_OpenAlertWarningWin(TXT_WLAN_SCAN_FAIL);
#endif
            break;
        }

        if (MMK_IsFocusWin(MMIWIFI_LIST_WIN_ID))
        {
            MMK_PostMsg(MMIWIFI_LIST_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
        }
        else if (MMK_IsFocusWin(MMIWIFI_MAIN_WIN_ID))
        {
            MMK_PostMsg(MMIWIFI_MAIN_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
        }
        else if (MMK_IsFocusWin(MMIWIFI_TRUSTED_LIST_CHILD_WIN_ID))
        {
            MMK_PostMsg(MMIWIFI_TRUSTED_LIST_CHILD_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
        }

        MMIWIFI_AutoConnect();
        break;

    case MMIWIFI_MSG_SCAN_BEGIN_IND:
        {       
            uint32 num = 0;
            //SCI_TRACE_LOW:"WIFI:info:MMI recv MMIWIFI_MSG_SCAN_BEGIN_IND sig"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_3826_112_2_18_3_9_59_184,(uint8*)"");

            num = MMIWIFI_GetProfileNum();
            while(num != 0)
            {
                profile_ptr = MMIWIFI_GetProfileByIndex(num-1);
                if (PNULL != profile_ptr
                    && !MMIWIFI_IsEditing(profile_ptr) && !MMIWIFI_IsTrustedProfile(profile_ptr) 
                    &&(profile_ptr->state == MMIWIFI_PROFILE_STATE_DISCONNECTED ||profile_ptr->state == MMIWIFI_PROFILE_STATE_DISCONNECTING))
                {
                    MMIWIFI_DeleteProfileByIndex(num - 1);
                }                
                num --;
            }
            
            if (MMK_IsFocusWin(MMIWIFI_LIST_WIN_ID))
            {
                MMK_PostMsg(MMIWIFI_LIST_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
            }
            else if (MMK_IsFocusWin(MMIWIFI_MAIN_WIN_ID))
            {
                MMK_PostMsg(MMIWIFI_MAIN_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
            }
        }
        break;

        
    case MMIWIFI_MSG_SCAN_IND:
        {
            //SCI_TRACE_LOW:"\n mmiwifi qq recv MMIWIFI_MSG_SCAN_IND"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_3851_112_2_18_3_9_59_185,(uint8*)"");
            if (MMK_IsFocusWin(MMIWIFI_LIST_WIN_ID))
            {
                MMK_PostMsg(MMIWIFI_LIST_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
            }
            else if (MMK_IsFocusWin(MMIWIFI_MAIN_WIN_ID))
            {
                MMK_PostMsg(MMIWIFI_MAIN_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
            }
        }
        break;
        
    case MMIWIFI_MSG_CONNECT_CNF:
        connect_ptr = (MMIWIFI_CONNECT_CNF_T*)param;
        //SCI_TRACE_LOW:"WIFI:info:MMI recv MMIWIFI_MSG_CONNECT_CNF, result is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_3861_112_2_18_3_9_59_186,(uint8*)"d", connect_ptr->result);
        profile_ptr = MMIWIFI_GetProfileByIndex(connect_ptr->index);
        if (PNULL == profile_ptr)
        {
            //SCI_TRACE_LOW:"\n mmiwifi error MMIWIFI_MSG_CONNECT_CNF profile_ptr is PNULL!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_3865_112_2_18_3_9_59_187,(uint8*)"");
            MMK_CloseWin(MMIWIFI_CONNECT_WAIT_WIN_ID);
#ifdef ADULT_WATCH_SUPPORT
            MMI_GetLabelTextByLang( TXT_COMMON_LINK_FAILED, &text_str );
            Adult_WatchCOM_NoteWin_1Line_Toast_Enter( MMIWIFI_LINK_FAILED_WIN_ID,&text_str,PNULL,PNULL);
#else
            MMIPUB_OpenAlertWarningWin(TXT_COMMON_LINK_FAILED);
#endif
            result = FALSE;
            break;
        }

        switch (connect_ptr->result)
        {
            case MMIWIFI_RESULT_SUCC:
                if (profile_ptr->is_ssid_hide)
                {
                    profile_ptr->is_on_air = TRUE;
                }
                if (MMIWIFI_IPGET_SELFDEFINED == profile_ptr->ip_get.ip_get_type)
                {
                    if (WIFISUPP_ENCRYP_PROTOCOL_OPENSYS == profile_ptr->config_info.encryp_protocol)
                    {                       
                        image = WifiGetConnectedRssiImage(profile_ptr->rssi,FALSE);
                    }
                    else
                    {
                        image = WifiGetConnectedRssiImage(profile_ptr->rssi,TRUE);                        
                    }             
                    MAIN_SetIdleWifiIcon(image);

                    if( MMK_IsOpenWin(MMIWIFI_LIST_WIN_ID) )
                    {
                        GUILIST_SetCurItemIndex(MMIWIFI_LIST_CTRL_ID, 1);
                    }
                    else if( MMK_IsOpenWin(MMIWIFI_MAIN_WIN_ID) )
                    {
                        GUILIST_SetCurItemIndex(MMIWIFI_MAIN_LIST_CTRL_ID, 1);
                    }

                    MMK_CloseWin(MMIWIFI_CONNECT_WAIT_WIN_ID);

                    if (MMK_IsOpenWin(MMIWIFI_SELECT_AP_WIN_ID))
                    {
                        MMK_CloseWin(MMIWIFI_SELECT_AP_WIN_ID);
                    }

#ifdef MMI_GUI_STYLE_TYPICAL
                    MMIPUB_OpenAlertWinByTextId(PNULL, TXT_WIFI_CONNECT_SUCC, 0,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
#else
                    if (MMK_IsFocusWin(MMIWIFI_LIST_WIN_ID))
                    {
                        MMK_PostMsg(MMIWIFI_LIST_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
                    }
#endif
                    break;
                }
                
                if (MMIWIFI_IPGET_AUTO == profile_ptr->ip_get.ip_get_type)
                {
                    MMI_GetLabelTextByLang(TXT_WIFI_DHCPING, &node_text);
                    MMIPUB_SetWaitWinText(TRUE, 1, &node_text, PNULL, PNULL, MMIWIFI_CONNECT_WAIT_WIN_ID);
                    break;
                }

                MMK_CloseWin(MMIWIFI_CONNECT_WAIT_WIN_ID);
#ifdef ADULT_WATCH_SUPPORT
                MMI_GetLabelTextByLang( TXT_COMMON_LINK_FAILED, &text_str );

                Adult_WatchCOM_NoteWin_1Line_Toast_Enter( MMIWIFI_LINK_FAILED_WIN_ID,&text_str,PNULL,PNULL);
#else
                MMIPUB_OpenAlertWarningWin(TXT_COMMON_LINK_FAILED);
#endif
                result = FALSE;
                break;
                
            case MMIWIFI_RESULT_RETRICTED:
                MMK_CloseWin(MMIWIFI_CONNECT_WAIT_WIN_ID);
#ifdef ADULT_WATCH_SUPPORT
                MMI_GetLabelTextByLang( TXT_WIFI_CONNECT_BUSY, &text_str );
                Adult_WatchCOM_NoteWin_1Line_Toast_Enter( MMIWIFI_CONNECT_BUSY_WIN_ID,&text_str,PNULL,PNULL);
#else
                MMIPUB_OpenAlertWarningWin(TXT_WIFI_CONNECT_BUSY);
#endif
                break;

            case MMIWIFI_RESULT_PWD_ERROR:
                MMK_CloseWin(MMIWIFI_CONNECT_WAIT_WIN_ID);
#ifdef ADULT_WATCH_SUPPORT
                MMI_GetLabelTextByLang( TXT_KL_PSW_WRONG_PSW, &text_str );
                Adult_WatchCOM_NoteWin_1Line_Toast_Enter( MMIWIFI_CLOSE_KL_PSW_WRONG_WIN_ID,&text_str,PNULL,PNULL);
#else
                MMIPUB_OpenAlertWarningWin(TXT_KL_PSW_WRONG_PSW);
#endif
                break;
            
            default:
                MMK_CloseWin(MMIWIFI_CONNECT_WAIT_WIN_ID);
                if (MMIWIFI_IsConnectByAutoConnecting())
                {
                    MMIWIFI_AutoConnect();
                }
                else
                {
#ifdef ADULT_WATCH_SUPPORT
                    MMI_GetLabelTextByLang( TXT_COMMON_LINK_FAILED, &text_str );

                    Adult_WatchCOM_NoteWin_1Line_Toast_Enter( MMIWIFI_LINK_FAILED_WIN_ID,&text_str,PNULL,PNULL);
#else
                    MMIPUB_OpenAlertWarningWin(TXT_COMMON_LINK_FAILED);
#endif
                }
                break;
                
        }
        break;
        
    case MMIWIFI_MSG_DISCONNECT_CNF:
        disconnection_ptr = (MMIWIFI_DISCONNECT_CNF_T*)param;
        //SCI_TRACE_LOW:"WIFI:info:MMI:recv MMIWIFI_MSG_DISCONNECT_CNF, result is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_3945_112_2_18_3_10_0_188,(uint8*)"d", disconnection_ptr->result);

        if (MMK_IsFocusWin(MMIWIFI_LIST_WIN_ID))
        {
            MMK_PostMsg(MMIWIFI_LIST_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
        }
        else if (MMK_IsFocusWin(MMIWIFI_TRUSTED_LIST_CHILD_WIN_ID))
        {
            MMK_PostMsg(MMIWIFI_TRUSTED_LIST_CHILD_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
        }
        else if (MMK_IsFocusWin(MMIWIFI_MAIN_WIN_ID))
        {
            MMK_PostMsg(MMIWIFI_MAIN_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
        }

#ifdef WLAN_RESELECT_SUPPORT
        MMIWIFI_ReSelectAP();
#endif
        MAIN_SetIdleWifiIcon(IMAGE_NULL);
        break;
        
    case MMIWIFI_MSG_DISCONNECT_IND:
        //SCI_TRACE_LOW:"WIFI:info:MMI:recv MMIWIFI_MSG_DISCONNECT_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_3962_112_2_18_3_10_0_189,(uint8*)"");

        profile_ptr = MMIWIFI_GetProfileByIndex(*((uint32 *)param));

        if (profile_ptr == MMIWIFI_GetCurrProfile())
        {
            MAIN_SetIdleWifiIcon(IMAGE_NULL);

            MMK_CloseWin(MMIWIFI_CONNECT_WAIT_WIN_ID);

#ifdef MMI_GUI_STYLE_TYPICAL
#ifdef ADULT_WATCH_SUPPORT
            MMI_GetLabelTextByLang( TXT_COMMON_LINK_FAILED, &text_str );

            Adult_WatchCOM_NoteWin_1Line_Toast_Enter( MMIWIFI_LINK_FAILED_WIN_ID,&text_str,PNULL,PNULL);
#else
            MMIPUB_OpenAlertWarningWin(TXT_COMMON_LINK_FAILED);
#endif
#else
            if (MMK_IsFocusWin(MMIWIFI_LIST_WIN_ID))
            {
                MMK_PostMsg(MMIWIFI_LIST_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
            }
            else if (MMK_IsFocusWin(MMIWIFI_MAIN_WIN_ID))
            {
                MMK_PostMsg(MMIWIFI_MAIN_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
            }
#endif /* MMI_GUI_STYLE_TYPICAL */

#ifdef WLAN_RESELECT_SUPPORT
            MMIWIFI_ReSelectAP();
#else
            MMIAPIPDP_WifiDeactiveInd();
#endif
        }
        break;

    case MMIWIFI_MSG_DHCP_CNF:
        dhcp_ptr = (MMIWIFI_DHCP_CNF_T *)param;
        //SCI_TRACE_LOW:"WIFI:info:MMI recv MMIWIFI_MSG_DHCP_CNF, result is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_3985_112_2_18_3_10_0_190,(uint8*)"d", dhcp_ptr->result);
        MMK_CloseWin(MMIWIFI_CONNECT_WAIT_WIN_ID);
        
        if (dhcp_ptr->result != MMIWIFI_RESULT_SUCC)
        {
            MAIN_SetIdleWifiIcon(IMAGE_NULL);

            if (MMIWIFI_IsConnectByAutoConnecting())
            {
                MMIWIFI_AutoConnect();
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_WIFI_DHCP_FAIL);
            }

            break;
        }
        
        if (MMIWIFI_IsConnectByAutoConnecting())
        {
            MMIWIFI_SetAutoConnectStatus(MMIWIFI_AUTO_CONNECT_STATUS_CONNECTED);
        }

        profile_ptr = MMIWIFI_GetProfileByIndex(dhcp_ptr->index);
        if (PNULL == profile_ptr)
        {
            //SCI_TRACE_LOW:"WIFI:error:MMI recv MMIWIFI_MSG_DHCP_CNF, but profile_ptr is PNULL!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_4009_112_2_18_3_10_0_191,(uint8*)"");
            break;
        }

        if (WIFISUPP_ENCRYP_PROTOCOL_OPENSYS == profile_ptr->config_info.encryp_protocol)
        {                       
            image = WifiGetConnectedRssiImage(profile_ptr->rssi,FALSE);
        }
        else
        {
            image = WifiGetConnectedRssiImage(profile_ptr->rssi,TRUE);                        
        }             
        MAIN_SetIdleWifiIcon(image);

        if (MMK_IsOpenWin(MMIWIFI_SELECT_AP_WIN_ID))
        {
            MMK_CloseWin(MMIWIFI_SELECT_AP_WIN_ID);
        }

        if ( MMK_IsOpenWin(MMIWIFI_LIST_WIN_ID) )
        {
            GUILIST_SetCurItemIndex(MMIWIFI_LIST_CTRL_ID, 1);
            GUILIST_SetTopItemIndex( MMIWIFI_LIST_CTRL_ID, 0 );
        }
        else if( MMK_IsOpenWin(MMIWIFI_MAIN_WIN_ID) )
        {
            GUILIST_SetCurItemIndex(MMIWIFI_MAIN_LIST_CTRL_ID, 1);
            GUILIST_SetTopItemIndex( MMIWIFI_MAIN_LIST_CTRL_ID, 0 );
        }

#ifdef MMI_GUI_STYLE_TYPICAL
        MMIPUB_OpenAlertWinByTextId(PNULL, TXT_WIFI_CONNECT_SUCC, 0, IMAGE_PUBWIN_SUCCESS, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
#else
        if (MMK_IsFocusWin(MMIWIFI_LIST_WIN_ID))
        {
            MMK_PostMsg(MMIWIFI_LIST_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
        }
        else if( MMK_IsFocusWin(MMIWIFI_MAIN_WIN_ID) )
        {
            MMK_PostMsg( MMIWIFI_MAIN_WIN_ID, MSG_FULL_PAINT, PNULL, 0 );
        }
#endif
        break;

    case MMIWIFI_MSG_BSSID_CHANGED_IND:
        //SCI_TRACE_LOW:"WIFI:info:MMI recv MMIWIFI_MSG_BSSID_CHANGED_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_4041_112_2_18_3_10_0_192,(uint8*)"");
        break;

    case MMIWIFI_MSG_RSSI_CHANGED_IND:
        rssi_ptr = (MMIWIFI_RSSI_CHANGED_IND_T*)param;
        //SCI_TRACE_LOW:"WIFI:info:MMI recv MMIWIFI_MSG_RSSI_CHANGED_IND new ssi is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_4046_112_2_18_3_10_0_193,(uint8*)"d", rssi_ptr->new_rssi);

        if (0 == rssi_ptr->new_rssi)
        {
            MMIWIFI_DeleteProfileByIndex(rssi_ptr->index);
            GUILIST_RemoveItem(MMIWIFI_LIST_CTRL_ID,rssi_ptr->index);
            break;
        }
        
        if (MMK_IsFocusWin(MMIWIFI_LIST_WIN_ID))
        {
            MMK_PostMsg(MMIWIFI_LIST_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
        }
        else if (MMK_IsFocusWin(MMIWIFI_TRUSTED_LIST_CHILD_WIN_ID))
        {
            MMK_PostMsg(MMIWIFI_TRUSTED_LIST_CHILD_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
        }
        else if (MMK_IsFocusWin(MMIWIFI_MAIN_WIN_ID))
        {
            MMK_PostMsg(MMIWIFI_MAIN_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
        }
        break;
        
    default:
        break;
    }
    return result;
}

/****************************************************************************/
//  Description : handle wifi list window
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleListWinMsg(MMI_WIN_ID_T        win_id, 
                                         MMI_MESSAGE_ID_E    msg_id, 
                                         DPARAM              param)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    uint16          index = 0;

    //SCI_TRACE_LOW:"WifiHandleListWinMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_4082_112_2_18_3_10_0_194,(uint8*)"d", msg_id);

    switch(msg_id) 
    {
    case MSG_OPEN_WINDOW:
        {
            GUI_BOTH_RECT_T both_rect = {0};
            both_rect = MMITHEME_GetWinClientBothRect(win_id);
            GUIAPICTRL_SetBothRect(MMIWIFI_LIST_CTRL_ID,&both_rect);
            GUILIST_SetNeedPrgbar(MMIWIFI_LIST_CTRL_ID, TRUE);
            MMK_SetAtvCtrl(win_id, MMIWIFI_LIST_CTRL_ID);
#ifdef MMI_WLAN_TAB_STYLE
            GUILIST_SetMaxItem(MMIWIFI_LIST_CTRL_ID, MMIWIFI_MAX_LIST_NUM, FALSE);
#else
            GUILIST_SetMaxItem(MMIWIFI_LIST_CTRL_ID, MMIWIFI_MAX_PROFILE_NUM + MMIWIFI_LIST_PERMANENT_ITEM_NUM, FALSE);
#endif /* MMI_WLAN_TAB_STYLE */

#ifdef MMI_PDA_SUPPORT
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
            GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, STXT_OPTION, FALSE);
            GUIWIN_SetTitleButtonImage(win_id, GUITITLE_BUTTON_RIGHT, IMAGE_ISTYLE_TITLE_MENU, IMAGE_ISTYLE_TITLE_MENU_DOWN, FALSE);
            GUIWIN_SetTitleButtonHandleMsgInfo(win_id, GUITITLE_BUTTON_RIGHT, MSG_APP_MENU, MMIWIFI_LIST_CTRL_ID);
#endif
            MMIWIFI_StartAutoScan();//Reset period
        }
        break;

    case MSG_FULL_PAINT:
        WifiUpdateList(MMIWIFI_LIST_CTRL_ID);  
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_MENU:
            WifiOpenConnectMenuWin(win_id);
        break;

#ifdef MMI_GUI_STYLE_TYPICAL
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        {
            BOOLEAN             is_wlan_on = TRUE;
            BOOLEAN             is_wifi_on = TRUE;
            uint32              total_num = 0;
            MMIWIFI_PROFILE_T   * profile_ptr = PNULL;

            is_wlan_on = MMIWIFI_GetIsWlanOn();
            is_wifi_on = MMIAPIWIFI_GetStatus();
            //SCI_TRACE_LOW:"WifiHandleListWinMsg wlan_on = %d, wifi_on = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_4141_112_2_18_3_10_0_195,(uint8*)"dd", is_wlan_on, is_wifi_on);
            if (!is_wlan_on)
            {
                MMIAPIWIFI_On(TRUE);
            }
            else
            {
                total_num = GUILIST_GetTotalItemNum(MMIWIFI_LIST_CTRL_ID);
                if (0 == total_num)
                {
                    MMIWIFI_Scan(PNULL);
                }
                else
                {
                    index = GUILIST_GetCurItemIndex(MMIWIFI_LIST_CTRL_ID);

                    profile_ptr = (MMIWIFI_PROFILE_T *)WifiGetListUserdata(MMIWIFI_LIST_CTRL_ID, index);

                    if (PNULL == profile_ptr)
                    {
                        break;
                    }

                    if ((MMIWIFI_PROFILE_STATE_CONNECTED == profile_ptr->state) ||
                        (MMIWIFI_PROFILE_STATE_DHCPED    == profile_ptr->state))
                    {
                        MMIWIFI_DisConnect(profile_ptr);
                    }
                    else 
                    {
                        if (MMIWIFI_Connect(profile_ptr))
                        {
                            MMIWIFI_OpenConnectWaitWin();
                        }
                    }
                }
            }
        }
        break;
#endif /* MMI_GUI_STYLE_TYPICAL */

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
#ifdef MMI_WLAN_TAB_STYLE
        MMK_CloseParentWin(MMIWIFI_MAIN_PARENT_WIN_ID);
#else
        MMK_CloseWin(win_id);
#endif /* MMI_WLAN_TAB_STYLE */
        break;

#ifndef MMI_GUI_STYLE_TYPICAL
    case MSG_CTL_LIST_LONGOK:
        index = GUILIST_GetCurItemIndex(MMIWIFI_LIST_CTRL_ID);
        if (WifiListIsAPItemSelected(index))
        {
            MMK_CreateWin((uint32*)MMIWIFI_AP_LIST_POP_WIN_TAB, PNULL);
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        {
            uint32 user_data = 0;
            SETTING_ITEM_FUNC   item_func = PNULL;

            index = GUILIST_GetCurItemIndex(MMIWIFI_LIST_CTRL_ID);
            user_data = WifiGetListUserdata(MMIWIFI_LIST_CTRL_ID, index);

            if (WifiListIsAPItemSelected(index))
            {
                WifiOpenAPPropertyWin((MMIWIFI_PROFILE_T *)user_data);
            }
            else
            {
                item_func = (SETTING_ITEM_FUNC)user_data;

                if (PNULL != item_func)
                {
                    (*item_func)();
                }
            }
        }
        break;

#endif /* MMI_GUI_STYLE_TYPICAL */

    case MSG_CLOSE_WINDOW:
        MMIAPIPDP_WifiActiveCnf(MMIWIFI_STATUS_CONNECTED == MMIAPIWIFI_GetStatus() ? MMIPDP_RESULT_SUCC : MMIPDP_RESULT_FAIL);
        MMIWIFI_StartAutoScan();//Restore period
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;

    }
    return result;
}

/*****************************************************************************
 Prototype    : WifiUpdateListTravCallback
 Description  : 更新一条list项
 Input        : MMIWIFI_PROFILE_T *pstProfile  
                void *find_data                
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2010/10/3
    Author       : George.Liu 01526
    Modification : Created function

*****************************************************************************/
LOCAL BOOLEAN WifiUpdateListTravCallback(MMIWIFI_PROFILE_T *profile_ptr, void *find_data)
{
    if(PNULL != profile_ptr) {
#ifdef MMI_WLAN_TAB_STYLE
        if(profile_ptr->is_on_air || (MMIWIFI_IsTrustedProfile(profile_ptr) && MMIWIFI_PROFILE_STATE_DHCPED == profile_ptr->state)) {
            WifiAppendListItem((MMI_CTRL_ID_T)find_data, profile_ptr);
        }
#else
            WifiAppendListItem((MMI_CTRL_ID_T)find_data, profile_ptr);
#endif /* MMI_WLAN_TAB_STYLE */
    }  else {
        //SCI_TRACE_LOW:"mmipdp error %s: profile_ptr is PNULL!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_4267_112_2_18_3_10_0_196,(uint8*)"s", __func__);
    }

    /*必须返回FALSE才能继续遍历*/
    return FALSE;
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL BOOLEAN WifiUpdateSelectAPListTravCallback(MMIWIFI_PROFILE_T *profile_ptr, void *find_data)
{
    if(PNULL != profile_ptr) {
        if(MMIWIFI_IsTrustedProfile(profile_ptr) && MMIWIFI_IsOnAir(profile_ptr)) {
                WifiAppendSelectAPListItem((MMI_CTRL_ID_T)find_data, profile_ptr);
        }
    }  else {
        //SCI_TRACE_LOW:"mmipdp error %s: profile_ptr is PNULL!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_4285_112_2_18_3_10_0_197,(uint8*)"s", __func__);
    }

    /*必须返回FALSE才能继续遍历*/
    return FALSE;
}

/****************************************************************************/
//  Description : WifiAppendListItem
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void WifiAppendListItem(MMI_CTRL_ID_T ctrl_id, MMIWIFI_PROFILE_T *profile_ptr)
{
    GUILIST_ITEM_T       item_t = {0};
    GUILIST_ITEM_DATA_T  item_data = {0};
    uint16               pw_ssid[WIFISUPP_SSID_MAX_LEN * 2 + 4] = {0};
    uint32               ulSsidLen = 0;
    uint32               order_index = 0;
    uint8                essid[WIFISUPP_SSID_MAX_LEN + 1] = {0};

    if (PNULL == profile_ptr)
    {
        return;
    }

#ifndef MMI_GUI_STYLE_TYPICAL
    item_t.item_style    = GUIITEM_STYLE_TWO_LINE_TEXT_2ANIM_AND_TEXT;

    ulSsidLen = GUI_GBToWstr(pw_ssid, profile_ptr->config_info.ssid.ssid, MIN(profile_ptr->config_info.ssid.ssid_len, WIFISUPP_SSID_MAX_LEN));
    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer.wstr_len = (uint16)ulSsidLen;
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = (wchar *)pw_ssid;

    if (profile_ptr->is_on_air)
    {
        item_data.item_content[1].item_data_type    = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[1].item_data.image_id = WifiFindRssiImage(profile_ptr->rssi);

        if (profile_ptr->config_info.encryp_protocol != WIFISUPP_ENCRYP_PROTOCOL_OPENSYS) {

            item_data.item_content[2].item_data_type    = GUIITEM_DATA_IMAGE_ID;
            item_data.item_content[2].item_data.image_id = IMAGE_WIFI_ENCRYPT;
        }

        item_data.item_content[3].item_data_type    = GUIITEM_DATA_TEXT_ID;

        if ((MMIWIFI_PROFILE_STATE_CONNECTED == profile_ptr->state) ||
            (MMIWIFI_PROFILE_STATE_DHCPED    == profile_ptr->state))
        {
            MMI_IMAGE_ID_T  image = IMAGE_NULL;

            item_data.item_content[3].item_data.text_id = TXT_COMMON_CONNECTED;
            image = WifiGetConnectedRssiImage(profile_ptr->rssi, FALSE);
            MAIN_SetIdleWifiIcon(image);
        }
        else
        {
            switch(profile_ptr->config_info.encryp_protocol) 
            {
            case WIFISUPP_ENCRYP_PROTOCOL_OPENSYS:
                item_data.item_content[3].item_data.text_id = TXT_WIFI_AUTH_TYPE_OPENSYS;
                break;

            case WIFISUPP_ENCRYP_PROTOCOL_WEP:
                item_data.item_content[3].item_data.text_id = TXT_WIFI_AUTH_TYPE_WEP;
                break;

            case WIFISUPP_ENCRYP_PROTOCOL_WPA:
                item_data.item_content[3].item_data.text_id = TXT_WIFI_AUTH_TYPE_WPA;
                break;

            case WIFISUPP_ENCRYP_PROTOCOL_WPA2:
                item_data.item_content[3].item_data.text_id = TXT_WIFI_AUTH_TYPE_WPA2;
                break;

            case WIFISUPP_ENCRYP_PROTOCOL_WAPI:
                item_data.item_content[3].item_data.text_id = TXT_WIFI_AUTH_TYPE_WAPI;
                break;

#ifdef WLAN_EAP_SUPPORT
            case WIFISUPP_ENCRYP_PROTOCOL_EAP:
                item_data.item_content[3].item_data.text_id = TXT_WLAN_EAP_SIM;
                break;
#endif // WLAN_EAP_SUPPORT

            default:
                item_data.item_content[3].item_data.text_id = TXT_NULL;
                break;
            }
        }
    }
    else
    {
        item_data.item_content[3].item_data_type = GUIITEM_DATA_TEXT_ID;

        if (profile_ptr->is_ssid_hide)
        {
            item_data.item_content[3].item_data.text_id = TXT_UNKNOWN;
        }
        else
        {
            item_data.item_content[3].item_data.text_id = TXT_WIFI_NOT_IN_RANGE;
        }    
    }    
#else
    item_data.softkey_id[0] = STXT_OPTION;
    item_data.softkey_id[1] = TXT_COMMON_CONNECT_COMN;
    item_data.softkey_id[2] = STXT_RETURN;

    item_data.item_content[0].item_data_type = GUIITEM_DATA_NONE;

    ulSsidLen = GUI_GBToWstr(pw_ssid, profile_ptr->config_info.ssid.ssid, MIN(profile_ptr->config_info.ssid.ssid_len, WIFISUPP_SSID_MAX_LEN));
    item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[1].item_data.text_buffer.wstr_len = (uint16)ulSsidLen;
    item_data.item_content[1].item_data.text_buffer.wstr_ptr = (wchar *)pw_ssid;

    item_data.item_content[3].item_data_type     = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[3].item_data.image_id = WifiFindRssiImage(profile_ptr->rssi);

    item_data.item_content[4].item_data_type = GUIITEM_DATA_NONE;

    if (profile_ptr->config_info.encryp_protocol != WIFISUPP_ENCRYP_PROTOCOL_OPENSYS) {
        item_data.item_content[4].item_data_type     = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[4].item_data.image_id = IMAGE_WIFI_ENCRYPT;
    }

    item_data.item_content[5].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[5].item_data.text_id = TXT_DISCONNECTED;

    if ((MMIWIFI_PROFILE_STATE_CONNECTED == profile_ptr->state) ||
        (MMIWIFI_PROFILE_STATE_DHCPED    == profile_ptr->state)) {
        item_data.softkey_id[1] = TXT_COMM_DISCONNECT;     
        item_data.item_content[5].item_data.text_id  = TXT_COMMON_CONNECTED;
        item_data.item_content[0].item_data_type     = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[0].item_data.image_id = IMAGE_WIFI_AP_CONNECTED;
    }

    item_t.item_style    = GUIITEM_STYLE_ONE_TWO_ANIM_TEXT_AND_TEXT_3ICON;
#endif /* MMI_GUI_STYLE_TYPICAL */

    item_t.item_data_ptr = &item_data;
    item_t.user_data = (uint32)profile_ptr;

#ifdef MMI_WLAN_TAB_STYLE
#ifdef WLAN_PORTAL_SUPPORT
    if(MMIWIFI_APIsCMCC(profile_ptr))
    {
        if (!GUILIST_InsertItem(ctrl_id, &item_t, 0))
        {
            SCI_TRACE_LOW("WifiAppendListItem failed");
        }
    }
    else
#endif        
    {
        if (!GUILIST_AppendItem(ctrl_id, &item_t))
        {
            SCI_TRACE_LOW("WifiAppendListItem failed");
        }
    }
#else
    order_index = WifiListGetProfileOrderIndex(ctrl_id, profile_ptr);

    SCI_MEMCPY(essid, profile_ptr->config_info.ssid.ssid, WIFISUPP_SSID_MAX_LEN);
    //SCI_TRACE_LOW:"WifiAppendListItem ESSID =%s order_index = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_4425_112_2_18_3_10_1_198,(uint8*)"sd", essid, order_index);

    if (!GUILIST_InsertItem(MMIWIFI_LIST_CTRL_ID, &item_t, order_index))
    {
        SCI_TRACE_LOW("WifiAppendListItem failed");
    }
#endif /* MMI_WLAN_TAB_STYLE */
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void WifiAppendSelectAPListItem(MMI_CTRL_ID_T ctrl_id, MMIWIFI_PROFILE_T *profile_ptr)
{
    GUILIST_ITEM_T       item_t = {0};
    GUILIST_ITEM_DATA_T  item_data = {0};
    uint16               pw_ssid[WIFISUPP_SSID_MAX_LEN * 2 + 4] = {0};
    uint32               ulSsidLen = 0;

    if (PNULL == profile_ptr)
    {
        return;
    }

    item_data.softkey_id[0] = TXT_NULL;
    item_data.softkey_id[1] = TXT_COMMON_CONNECT_COMN;
    item_data.softkey_id[2] = STXT_RETURN;

    item_data.item_content[0].item_data_type = GUIITEM_DATA_NONE;

    ulSsidLen = GUI_GBToWstr(pw_ssid, profile_ptr->config_info.ssid.ssid, MIN(profile_ptr->config_info.ssid.ssid_len, WIFISUPP_SSID_MAX_LEN));
    item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[1].item_data.text_buffer.wstr_len = (uint16)ulSsidLen;
    item_data.item_content[1].item_data.text_buffer.wstr_ptr = (wchar *)pw_ssid;

    item_data.item_content[3].item_data_type     = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[3].item_data.image_id = WifiFindRssiImage(profile_ptr->rssi);

    item_data.item_content[4].item_data_type = GUIITEM_DATA_NONE;

    if (profile_ptr->config_info.encryp_protocol != WIFISUPP_ENCRYP_PROTOCOL_OPENSYS) {
        item_data.item_content[4].item_data_type     = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[4].item_data.image_id = IMAGE_WIFI_ENCRYPT;
    }

    item_data.item_content[5].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[5].item_data.text_id = TXT_DISCONNECTED;


    if ((MMIWIFI_PROFILE_STATE_CONNECTED == profile_ptr->state) ||
        (MMIWIFI_PROFILE_STATE_DHCPED    == profile_ptr->state)) {
        item_data.item_content[5].item_data.text_id  = TXT_COMMON_CONNECTED;
        item_data.item_content[0].item_data_type     = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[0].item_data.image_id = IMAGE_WIFI_AP_CONNECTED;
        //SCI_TRACE_LOW:"WifiAppendSelectAPListItem Error "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_4477_112_2_18_3_10_1_199,(uint8*)"");
    }

    item_t.item_style    = GUIITEM_STYLE_ONE_TWO_ANIM_TEXT_AND_TEXT_3ICON;
    item_t.item_data_ptr = &item_data;
    item_t.user_data = (uint32)profile_ptr;

    if (!GUILIST_AppendItem(ctrl_id, &item_t))
    {
        SCI_TRACE_LOW("WifiAppendSelectAPListItem failed");
    }
}

/*****************************************************************************
 Prototype    : WifiUpdateList
 Description  : 刷新list窗口
 Input        : void  
 Output       : None
 Return Value : LOCAL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2010/8/31
    Author       : George.Liu 01526
    Modification : Created function

*****************************************************************************/
LOCAL void WifiUpdateList(MMI_CTRL_ID_T ctrl_id)
{
    uint32  num = 0;
    uint16  current_index = 0;
    uint16  top_index = 0;

    current_index = GUILIST_GetCurItemIndex(ctrl_id);
    top_index = GUILIST_GetTopItemIndex(ctrl_id);
    GUILIST_RemoveAllItems(ctrl_id);

#ifdef MMI_WLAN_TAB_STYLE
    if (!MMIWIFI_GetIsWlanOn())
    {
        GUIWIN_SetSoftkeyTextId(MMIWIFI_LIST_WIN_ID,  TXT_WIFI_ENABLE, TXT_NULL, STXT_RETURN, FALSE);
        return;
    }

    num = MMIWIFI_DisplayProfileNum();

    if (0 == num)
    {
        GUIWIN_SetSoftkeyTextId(MMIWIFI_LIST_WIN_ID,  STXT_OPTION, TXT_REFRESH, STXT_RETURN, FALSE);
        return;
    }
    else
    {
        GUIWIN_SetSoftkeyTextId(MMIWIFI_LIST_WIN_ID,  STXT_OPTION, TXT_COMMON_CONNECT_COMN, STXT_RETURN, FALSE);
    }

    //将数据库中的AP list同步到list界面上来
    MMIWIFI_TraverseProfile(WifiUpdateListTravCallback, (void *)ctrl_id);

    GUILIST_SetTopItemIndex(ctrl_id, top_index);
    GUILIST_SetCurItemIndex(ctrl_id, current_index);
#else
    num = MMIWIFI_GetProfileNum();

    if (MMIWIFI_GetIsWlanOn() && num > 0)
    {
        MMIWIFI_TraverseProfile(WifiUpdateListTravCallback, (void *)ctrl_id);
    }

    WifiListAppendPermanentItem(ctrl_id);
            
    if (current_index >= (num + MMIWIFI_LIST_PERMANENT_ITEM_NUM))
    {
        current_index = 0;
    }

    GUILIST_SetTopItemIndex(ctrl_id, top_index);
    GUILIST_SetCurItemIndex(ctrl_id, current_index);

    MMK_SendMsg( ctrl_id, MSG_CTL_PAINT, 0 );

#endif /* MMI_GUI_STYLE_TYPICAL */
    return;
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiUpdateSelectAPList(MMI_CTRL_ID_T ctrl_id)
{
    uint32  num = 0;
    uint16  current_item_index = 0;

    //记录当前选中项的索引
    current_item_index = GUILIST_GetCurItemIndex(ctrl_id);

    GUILIST_RemoveAllItems(ctrl_id);

    num = MMIWIFI_GetOnAirTrustedProfileNum();

    if (0 == num)
    {
        return;
    }

    //将数据库中的AP list同步到list界面上来
    MMIWIFI_TraverseProfile(WifiUpdateSelectAPListTravCallback, (void *)ctrl_id);

    //根据记录的选中项的索引，恢复list的选中项
    GUILIST_SetCurItemIndex(ctrl_id, current_item_index);
    return;
}

/*****************************************************************************/
//  Description : open wifi popmenu
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E WifiOpenConnectMenuWin(MMI_WIN_ID_T win_id)
{
    switch(win_id)
    {
    case MMIWIFI_LIST_WIN_ID:
        MMK_CreateWin((uint32*)MMIWIFI_LIST_POPMENU_WIN_TAB, PNULL);
        break;

#ifdef MMI_WLAN_TAB_STYLE
    case MMIWIFI_TRUSTED_LIST_CHILD_WIN_ID:
        MMK_CreateWin((uint32*)MMIWIFI_TRUSTED_LIST_POPMENU_WIN_TAB, PNULL);
        break;
#endif /* MMI_WLAN_TAB_STYLE */
    default:
        break;
    }

    return MMI_RESULT_TRUE;
}

/****************************************************************************/
//  Description : handle wifi list popmenu window
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandlelistPopMenuWinMsg( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    switch(msg_id) 
    {
    case MSG_OPEN_WINDOW:
        WifiCreateListPopMenu(win_id, MMIWIFI_MENU_CTRL_ID);
        MMK_SetAtvCtrl(win_id, MMIWIFI_MENU_CTRL_ID);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_KEYDOWN_WEB:
        {
            switch (GUIMENU_GetCurNodeId(MMIWIFI_MENU_CTRL_ID))
            {
            case MMIWIFI_POPMENU_ID_REFRESH://刷新
                MMK_CloseWin(win_id);
                if(MMIAPISET_GetFlyMode()&& (FALSE == MMIAPIENG_GetIsFlightModeWifi()))
                {
                     MMIPUB_OpenAlertWarningWin(TXT_CLOSE_FLY_MODE_FIRST); 
                }
                else
                {
                    if (MMIAPIWIFI_GetStatus())
                    {
                        MMIWIFI_ScanEx(PNULL, FALSE);
                    }
                    else
                    {
                        MMIAPIWIFI_On(TRUE);
                    }
                }
                break;
                    
                case MMIWIFI_POPMENU_ID_SPECTIFY_SEARCH_SSID://搜索指定接入点，需完善
                    MMK_CloseWin(win_id);
                    if(MMIAPISET_GetFlyMode()&& (FALSE == MMIAPIENG_GetIsFlightModeWifi()))
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_CLOSE_FLY_MODE_FIRST); 
                    }
                    else
                    {
                        MMK_CreatePubEditWin((uint32*)MMIWIFI_SPECTIFY_SEARCH_SSID_WIN_TAB,PNULL); 
                    }
                    break;
                
                case MMIWIFI_POPMENU_ID_DETAIL://详情
                    {
                        uint16 index = 0;
                        MMIWIFI_PROFILE_T * profile_ptr = PNULL;

                        MMK_CloseWin(win_id);

                        index = GUILIST_GetCurItemIndex(MMIWIFI_LIST_CTRL_ID);

                        if (0 != GUILIST_GetTotalItemNum(MMIWIFI_LIST_CTRL_ID) && WifiListIsAPItemSelected(index))
                        {
                            profile_ptr = (MMIWIFI_PROFILE_T *)WifiGetListUserdata(MMIWIFI_LIST_CTRL_ID, index);

                            MMK_CreateWin((uint32*)MMIWIFI_DETAIL_WIN_TAB,(ADD_DATA)profile_ptr);  
                        }
                    }
                    break;

#ifdef WLAN_PORTAL_SUPPORT
            case MMIWIFI_POPMENU_ID_DEFAULT_AP_SETTING://默认接入点设置
                MMK_CloseWin(win_id);
                MMIWIFI_OpenPortalLoginWin();
                break;
#endif

#ifndef MMI_GUI_STYLE_TYPICAL
           case MMIWIFI_POPMENU_ID_SETTING://默认接入点设置
                MMK_CloseWin(win_id);
                MMK_CreateWin((uint32 *)MMIWIFI_SETTING_LIST_CHILD_WIN_TAB, NULL);
                break;
#endif

            case MMIWIFI_POPMENU_ID_FORGET_AP:
                {
                    uint16 index = 0;
                    MMIWIFI_PROFILE_T * profile_ptr = PNULL;

                    MMK_CloseWin(win_id);
                    index = GUILIST_GetCurItemIndex(MMIWIFI_LIST_CTRL_ID);

                    if (!WifiListIsAPItemSelected(index))
                    {
                        break;
                    }

                    profile_ptr = (MMIWIFI_PROFILE_T *)WifiGetListUserdata(MMIWIFI_LIST_CTRL_ID, index);

                    if (PNULL != profile_ptr && !(profile_ptr->state == MMIWIFI_PROFILE_STATE_DISCONNECTED || profile_ptr->state == MMIWIFI_PROFILE_STATE_DISCONNECTING))
                    {
                        MMIPUB_OpenQueryWinByTextIdEx(MMK_GetFirstAppletHandle(),TXT_WLAN_IS_DELETE_CONNECTED, IMAGE_PUBWIN_QUERY, PNULL, WifiHandleQueryForgetAP, (uint32)profile_ptr);
                        break;
                    }

                    if(PNULL != profile_ptr && profile_ptr ->is_on_air)//如所删AP是当前搜索结果，不删除AP，将其is_trusted标志置否
                    {
                        profile_ptr ->is_trusted = FALSE;
                        MMIWIFI_UpdateProfileFile();
                    }
                    else
                    {
                        if (MMIWIFI_DeleteProfileByAp(profile_ptr))
                        {
                            MMIWIFI_UpdateProfileFile();
                        }
                    }
                }
                break;

            case MMIWIFI_POPMENU_ID_EDIT:
                {
                    uint16 index = 0;
                    MMIWIFI_PROFILE_T * profile_ptr = PNULL;

                    MMK_CloseWin(win_id);

                    index = GUILIST_GetCurItemIndex(MMIWIFI_LIST_CTRL_ID);

                    if (!WifiListIsAPItemSelected(index))
                    {
                        break;
                    }

                    profile_ptr = (MMIWIFI_PROFILE_T *)WifiGetListUserdata(MMIWIFI_LIST_CTRL_ID, index);

                    if (PNULL != profile_ptr)
                    {
                        if (profile_ptr->state == MMIWIFI_PROFILE_STATE_DISCONNECTED || profile_ptr->state == MMIWIFI_PROFILE_STATE_DISCONNECTING)
                        {
                            MMIWIFI_OpenEditWin(profile_ptr);
                        }
                        else
                        {
                            MMIPUB_OpenAlertWarningWin(TXT_COMMON_DEL_FAIL);
                        }
                    }
                }    
                break;

            case MMIWIFI_POPMENU_ID_ADD_AP:
                MMK_CloseWin(win_id);
                WifiAddTrustedAp();
                break;

            default:
                break;

            }
        }
        break;

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
//  Discription: Handle cancel connect waiting win msg
//  Global resource dependence: none 
//  Author: li.li 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleCancelConnectWaitWinMsg(
                                                    MMI_WIN_ID_T        win_id, 
                                                    MMI_MESSAGE_ID_E    msg_id, 
                                                    DPARAM              param 
                                                    )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;   
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIDEFAULT_AllowOpenKeylocWin(FALSE);
        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_CANCEL, FALSE);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        {
            MMIWIFI_PROFILE_T *profile_ptr = PNULL;
            MMI_CTRL_ID_T     ctrl_id = MMIWIFI_LIST_CTRL_ID;
            uint32            index = 0;

            index = GUILIST_GetCurItemIndex(ctrl_id);

            if (WifiListIsAPItemSelected(index))
            {
                profile_ptr = (MMIWIFI_PROFILE_T *)WifiGetListUserdata(ctrl_id, index);

                MMIWIFI_DisConnect(profile_ptr);
            }

            MMK_CloseWin( win_id );     
        }
        break;

    case MSG_APP_RED:
        {
            MMIWIFI_PROFILE_T *profile_ptr = PNULL;
            MMI_CTRL_ID_T     ctrl_id = MMIWIFI_LIST_CTRL_ID;
            uint32            index = 0;

            index = GUILIST_GetCurItemIndex(ctrl_id);

            if (WifiListIsAPItemSelected(index))
            {
                profile_ptr = (MMIWIFI_PROFILE_T *)WifiGetListUserdata(ctrl_id, index);

                MMIWIFI_DisConnect(profile_ptr);
            }

            MMK_CloseWin( win_id );     

            result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        }
        break;

    case MSG_CLOSE_WINDOW:
        MMIDEFAULT_AllowOpenKeylocWin(TRUE);
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
        
    default:
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }
    
    return result;  
}
/****************************************************************************/
//  Description : set richtext item
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
LOCAL void WifiSetDetailRichtextItem(GUIRICHTEXT_ITEM_T*  item_data_ptr, GUIRICHTEXT_TEXT_TYPE_E text_type)
{
    if (PNULL == item_data_ptr)
    {
        return;
    }
    SCI_MEMSET(item_data_ptr, 0x00, sizeof(GUIRICHTEXT_ITEM_T));

    item_data_ptr->img_type  = GUIRICHTEXT_IMAGE_NONE;
    item_data_ptr->text_type = text_type;
    item_data_ptr->align_type = GUIRICHTEXT_ALIGN_DEFAULT;
}
/****************************************************************************/
//  Description : set wifi detail
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
LOCAL void WifiSetDetail(MMI_CTRL_ID_T ctrl_id, MMIWIFI_PROFILE_T *profile_ptr)
{
    MMI_STRING_T        detail_str = {0};
    GUIRICHTEXT_ITEM_T  item_data = {0};
    uint16              uint16_str[MMIWIFI_DETAIL_TEMP_LEN+1] = {0};
    char                temp_buf[MMIWIFI_DETAIL_TEMP_LEN+1] = {0};
    uint32              len = 0;
    uint16              nIndex = 0;
    wchar               item_text[GUIRICHTEXT_TEXT_MAX_LEN + 1] = {0};
    MMI_TEXT_ID_T text_id = TXT_NULL;

    if (PNULL != profile_ptr)
    {
        //.add ssid
        //header
        WifiSetDetailRichtextItem(&item_data, GUIRICHTEXT_TEXT_RES);
        item_data.text_data.res.id = TXT_WIFI_DETAIL_NETWORK;
        GUIRICHTEXT_AddItem(ctrl_id,&item_data,&nIndex);
        
        //value
        WifiSetDetailRichtextItem(&item_data, GUIRICHTEXT_TEXT_BUF);
        SCI_MEMSET(item_text, 0, sizeof(item_text));

        SCI_MEMSET(uint16_str,0,sizeof(uint16_str));
        len = GUI_GBToWstr(uint16_str, profile_ptr->config_info.ssid.ssid, MIN(profile_ptr->config_info.ssid.ssid_len, WIFISUPP_SSID_MAX_LEN));

        MMIAPICOM_Wstrncpy(item_text,uint16_str, GUIRICHTEXT_TEXT_MAX_LEN);
        //set to richtext
        item_data.text_data.buf.str_ptr = item_text;
        item_data.text_data.buf.len = MMIAPICOM_Wstrlen(item_text);
        GUIRICHTEXT_AddItem (ctrl_id,&item_data,&nIndex);
        
        //.add bssid
        //header
        WifiSetDetailRichtextItem(&item_data, GUIRICHTEXT_TEXT_RES);
        item_data.text_data.res.id = TXT_WIFI_DETAIL_BSSID;
        GUIRICHTEXT_AddItem(ctrl_id,&item_data,&nIndex);

        //value
        WifiSetDetailRichtextItem(&item_data, GUIRICHTEXT_TEXT_BUF);
        SCI_MEMSET(item_text, 0, sizeof(item_text));

        CFL_Snprintf(temp_buf,MMIWIFI_DETAIL_TEMP_LEN,"%02x:%02x:%02x:%02x:%02x:%02x",profile_ptr->config_info.bssid_info.bssid[0],
            profile_ptr->config_info.bssid_info.bssid[1],profile_ptr->config_info.bssid_info.bssid[2],
            profile_ptr->config_info.bssid_info.bssid[3],profile_ptr->config_info.bssid_info.bssid[4],profile_ptr->config_info.bssid_info.bssid[5]);
        len = strlen(temp_buf);
        SCI_MEMSET(uint16_str,0,sizeof(uint16_str));
        MMI_STRNTOWSTR(uint16_str, MMIWIFI_DETAIL_TEMP_LEN, (uint8 *)temp_buf, len, MIN(len,MMIWIFI_DETAIL_TEMP_LEN));
        //cat
        MMIAPICOM_Wstrncpy(item_text,uint16_str,GUIRICHTEXT_TEXT_MAX_LEN);
        //set to richtext
        item_data.text_data.buf.str_ptr = item_text;
        item_data.text_data.buf.len = MMIAPICOM_Wstrlen(item_text);
        GUIRICHTEXT_AddItem (ctrl_id,&item_data,&nIndex);

        //.add channel
        //header
        //set to richtext
        WifiSetDetailRichtextItem(&item_data, GUIRICHTEXT_TEXT_RES);
        item_data.text_data.res.id = TXT_WIFI_DETAIL_CHANNEL;
        GUIRICHTEXT_AddItem(ctrl_id,&item_data,&nIndex);

        //value
        WifiSetDetailRichtextItem(&item_data, GUIRICHTEXT_TEXT_BUF);
        SCI_MEMSET(item_text, 0, sizeof(item_text));

        SCI_MEMSET(temp_buf,0,sizeof(temp_buf));
        CFL_Snprintf(temp_buf, MMIWIFI_DETAIL_TEMP_LEN,"%d",profile_ptr->config_info.channel);
        len = strlen(temp_buf);
        SCI_MEMSET(uint16_str,0,sizeof(uint16_str));
        MMI_STRNTOWSTR(uint16_str, MMIWIFI_DETAIL_TEMP_LEN, (uint8 *)temp_buf, len,MIN(len,MMIWIFI_DETAIL_TEMP_LEN));
        //cat
        MMIAPICOM_Wstrncpy(item_text, uint16_str, GUIRICHTEXT_TEXT_MAX_LEN);
        //set to richtext
        item_data.text_data.buf.str_ptr = item_text;
        item_data.text_data.buf.len = MMIAPICOM_Wstrlen(item_text);
        GUIRICHTEXT_AddItem (ctrl_id,&item_data,&nIndex);
        
        //.add rssi
        //header
        WifiSetDetailRichtextItem(&item_data, GUIRICHTEXT_TEXT_RES);
        item_data.text_data.res.id = TXT_WIFI_DETAIL_RSSI;
        item_data.img_type = GUIRICHTEXT_IMAGE_RES;
        item_data.align_type = GUIRICHTEXT_ALIGN_LEFT;
        item_data.img_data.res.id = WifiFindRssiImage(profile_ptr->rssi);
        GUIRICHTEXT_AddItem(ctrl_id,&item_data,&nIndex);

        //value
        WifiSetDetailRichtextItem(&item_data, GUIRICHTEXT_TEXT_BUF);
        SCI_MEMSET(item_text, 0, sizeof(item_text));

        if (profile_ptr->rssi <= MMIWIFI_RSSI_40)
        {
            MMI_GetLabelTextByLang( TXT_WIFI_DETAIL_RSSI_WEAK, &detail_str );
        }
        else
        {
            MMI_GetLabelTextByLang( TXT_WIFI_DETAIL_RSSI_STRONG, &detail_str );
        }

        MMIAPICOM_Wstrncpy(item_text, detail_str.wstr_ptr, detail_str.wstr_len);

        //信号强度数值


        SCI_MEMSET(temp_buf,0,sizeof(temp_buf));
        CFL_Snprintf(temp_buf, MMIWIFI_DETAIL_TEMP_LEN,"%d dBm",profile_ptr->rssi);
        len = strlen(temp_buf);
        SCI_MEMSET(uint16_str,0,sizeof(uint16_str));
        MMI_STRNTOWSTR(uint16_str, MMIWIFI_DETAIL_TEMP_LEN, (uint8 *)temp_buf, len,MIN(len,MMIWIFI_DETAIL_TEMP_LEN));
        MMIAPICOM_Wstrncat(item_text, uint16_str, GUIRICHTEXT_TEXT_MAX_LEN);


        item_data.text_data.buf.str_ptr = item_text;
        item_data.text_data.buf.len = MMIAPICOM_Wstrlen(item_text);
        GUIRICHTEXT_AddItem (ctrl_id,&item_data,&nIndex);
    
        //set mode
        WifiSetDetailRichtextItem(&item_data, GUIRICHTEXT_TEXT_RES);
        item_data.text_data.res.id = TXT_DETAIL_MODE;
        GUIRICHTEXT_AddItem(ctrl_id,&item_data,&nIndex);

        WifiSetDetailRichtextItem(&item_data, GUIRICHTEXT_TEXT_RES);

        if (profile_ptr->config_info.network_mode == WIFISUPP_NETWORK_MODE_INFRASTRUCTURE)
        {
            item_data.text_data.res.id = TXT_WIFI_NETWORK_TYPE_INFRASTRUCTURE;
        }
        else
        {
            item_data.text_data.res.id = TXT_WIFI_NETWORK_TYPE_ADHOC;
        }
        GUIRICHTEXT_AddItem(ctrl_id,&item_data,&nIndex);
        
        //set credential
        WifiSetDetailRichtextItem(&item_data, GUIRICHTEXT_TEXT_RES);
        item_data.text_data.res.id = TXT_WIFI_DETAIL_CRED;
        GUIRICHTEXT_AddItem(ctrl_id,&item_data,&nIndex);

        switch(profile_ptr->config_info.encryp_protocol) 
        {
        case WIFISUPP_ENCRYP_PROTOCOL_OPENSYS:
            text_id = TXT_WIFI_AUTH_TYPE_OPENSYS;
            break;

        case WIFISUPP_ENCRYP_PROTOCOL_WEP:
            text_id = TXT_WIFI_AUTH_TYPE_WEP;
            break;

        case WIFISUPP_ENCRYP_PROTOCOL_WPA:
            text_id = TXT_WIFI_AUTH_TYPE_WPA;
            break;

        case WIFISUPP_ENCRYP_PROTOCOL_WPA2:
            text_id = TXT_WIFI_AUTH_TYPE_WPA2;
            break;

        case WIFISUPP_ENCRYP_PROTOCOL_WAPI:
            text_id = TXT_WIFI_AUTH_TYPE_WAPI;
            break;

#ifdef WLAN_EAP_SUPPORT
        case WIFISUPP_ENCRYP_PROTOCOL_EAP:
            text_id = TXT_WLAN_EAP_SIM;
            break;
#endif // WLAN_EAP_SUPPORT

        default:
            text_id = TXT_NULL;
            break;
        }

        WifiSetDetailRichtextItem(&item_data, GUIRICHTEXT_TEXT_RES);
        item_data.text_data.res.id = text_id;
        GUIRICHTEXT_AddItem(ctrl_id,&item_data,&nIndex);
        
        //set rate
        WifiSetDetailRichtextItem(&item_data, GUIRICHTEXT_TEXT_RES);
        item_data.text_data.res.id = TXT_WIFI_DETAIL_RATE;
        GUIRICHTEXT_AddItem(ctrl_id,&item_data,&nIndex);

        MMI_GetLabelTextByLang( TXT_WIFI_DETAIL_RATE, &detail_str );
        WifiSetDetailRichtextItem(&item_data, GUIRICHTEXT_TEXT_BUF);
        SCI_MEMSET(item_text, 0, sizeof(item_text));

        SCI_MEMSET(temp_buf,0,sizeof(temp_buf));
        CFL_Snprintf(temp_buf, MMIWIFI_DETAIL_TEMP_LEN,"%d%s",profile_ptr->rate,"Mbit/s");
        len = strlen(temp_buf);
        SCI_MEMSET(uint16_str,0,sizeof(uint16_str));
        MMI_STRNTOWSTR(uint16_str, MMIWIFI_DETAIL_TEMP_LEN, (uint8 *)temp_buf,len,MIN(len,MMIWIFI_DETAIL_TEMP_LEN));
        MMIAPICOM_Wstrncpy(item_text,uint16_str,len);

        item_data.text_data.buf.str_ptr = item_text;
        item_data.text_data.buf.len = MMIAPICOM_Wstrlen(item_text);
        GUIRICHTEXT_AddItem (ctrl_id,&item_data,&nIndex);

        
        if (profile_ptr->is_dhcp_success || profile_ptr->ip_get.ip_get_type == MMIWIFI_IPGET_SELFDEFINED)
        {            
            //add ip
            WifiSetDetailRichtextItem(&item_data, GUIRICHTEXT_TEXT_RES);
            item_data.text_data.res.id = TXT_WIFI_DETAIL_IP;
            GUIRICHTEXT_AddItem(ctrl_id,&item_data,&nIndex);

            WifiSetDetailRichtextItem(&item_data, GUIRICHTEXT_TEXT_BUF);
            SCI_MEMSET(item_text, 0, sizeof(item_text));

            len = strlen(profile_ptr->ip_get.ip.ip_addr);
            SCI_MEMSET(uint16_str,0,sizeof(uint16_str));
            MMI_STRNTOWSTR(uint16_str, MMIWIFI_DETAIL_TEMP_LEN, (uint8 *)profile_ptr->ip_get.ip.ip_addr, len, MIN(len,MMIWIFI_DETAIL_TEMP_LEN));
            MMIAPICOM_Wstrncpy(item_text,uint16_str,GUIRICHTEXT_TEXT_MAX_LEN);
            
            item_data.text_data.buf.str_ptr = item_text;
            item_data.text_data.buf.len = MMIAPICOM_Wstrlen(item_text);
            GUIRICHTEXT_AddItem (ctrl_id,&item_data,&nIndex);

            //add submask
            WifiSetDetailRichtextItem(&item_data, GUIRICHTEXT_TEXT_RES);
            item_data.text_data.res.id = TXT_WIFI_DETAIL_SUBMASK;
            GUIRICHTEXT_AddItem(ctrl_id,&item_data,&nIndex);

            WifiSetDetailRichtextItem(&item_data, GUIRICHTEXT_TEXT_BUF);
            SCI_MEMSET(item_text, 0, sizeof(item_text));

            len = strlen(profile_ptr->ip_get.ip.subnet_mask);
            SCI_MEMSET(uint16_str,0,sizeof(uint16_str));
            MMI_STRNTOWSTR(uint16_str, MMIWIFI_DETAIL_TEMP_LEN, (uint8 *)profile_ptr->ip_get.ip.subnet_mask, len, MIN(len,MMIWIFI_DETAIL_TEMP_LEN));
            
            MMIAPICOM_Wstrncpy(item_text, uint16_str, GUIRICHTEXT_TEXT_MAX_LEN);

            item_data.text_data.buf.str_ptr = item_text;
            item_data.text_data.buf.len = MMIAPICOM_Wstrlen(item_text);
            GUIRICHTEXT_AddItem (ctrl_id,&item_data,&nIndex);

            //add gateway
            WifiSetDetailRichtextItem(&item_data, GUIRICHTEXT_TEXT_RES);
            item_data.text_data.res.id = TXT_WIFI_DETAIL_GATEWAY;
            GUIRICHTEXT_AddItem(ctrl_id,&item_data,&nIndex);
            
            WifiSetDetailRichtextItem(&item_data, GUIRICHTEXT_TEXT_BUF);
            SCI_MEMSET(item_text, 0, sizeof(item_text));

            len = strlen(profile_ptr->ip_get.ip.default_gateway);
            SCI_MEMSET(uint16_str,0,sizeof(uint16_str));
            MMI_STRNTOWSTR(uint16_str, MMIWIFI_DETAIL_TEMP_LEN, (uint8 *)profile_ptr->ip_get.ip.default_gateway, len, MIN(len,MMIWIFI_DETAIL_TEMP_LEN));
            
            MMIAPICOM_Wstrncpy(item_text, uint16_str, GUIRICHTEXT_TEXT_MAX_LEN);

            item_data.text_data.buf.str_ptr = item_text;
            item_data.text_data.buf.len = MMIAPICOM_Wstrlen(item_text);
            GUIRICHTEXT_AddItem (ctrl_id,&item_data,&nIndex);

            //add dns1
            WifiSetDetailRichtextItem(&item_data, GUIRICHTEXT_TEXT_RES);
            item_data.text_data.res.id = TXT_WIFI_DETAIL_DNS1;
            GUIRICHTEXT_AddItem(ctrl_id,&item_data,&nIndex);
            
            WifiSetDetailRichtextItem(&item_data, GUIRICHTEXT_TEXT_BUF);
            SCI_MEMSET(item_text, 0, sizeof(item_text));

            len = strlen(profile_ptr->ip_get.ip.first_dns);
            SCI_MEMSET(uint16_str,0,sizeof(uint16_str));
            MMI_STRNTOWSTR(uint16_str, MMIWIFI_DETAIL_TEMP_LEN, (uint8 *)profile_ptr->ip_get.ip.first_dns, len, MIN(len,MMIWIFI_DETAIL_TEMP_LEN));
            
            MMIAPICOM_Wstrncpy(item_text,uint16_str,GUIRICHTEXT_TEXT_MAX_LEN);
            
            item_data.text_data.buf.str_ptr = item_text;
            item_data.text_data.buf.len = MMIAPICOM_Wstrlen(item_text);
            GUIRICHTEXT_AddItem (ctrl_id,&item_data,&nIndex);

            //add dns2
            WifiSetDetailRichtextItem(&item_data, GUIRICHTEXT_TEXT_RES);
            item_data.text_data.res.id = TXT_WIFI_DETAIL_DNS2;
            GUIRICHTEXT_AddItem(ctrl_id,&item_data,&nIndex);

            WifiSetDetailRichtextItem(&item_data, GUIRICHTEXT_TEXT_BUF);
            SCI_MEMSET(item_text, 0, sizeof(item_text));

            len = strlen(profile_ptr->ip_get.ip.second_dns);
            SCI_MEMSET(uint16_str,0,sizeof(uint16_str));
            MMI_STRNTOWSTR(uint16_str, MMIWIFI_DETAIL_TEMP_LEN, (uint8 *)profile_ptr->ip_get.ip.second_dns, len, MIN(len,MMIWIFI_DETAIL_TEMP_LEN));

            MMIAPICOM_Wstrncpy(item_text,uint16_str,GUIRICHTEXT_TEXT_MAX_LEN);
            
            item_data.text_data.buf.str_ptr = item_text;
            item_data.text_data.buf.len = MMIAPICOM_Wstrlen(item_text);
            GUIRICHTEXT_AddItem (ctrl_id,&item_data,&nIndex);
        }
    }
}

/****************************************************************************/
//  Description : handle wifi detail window
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  HandleWifiDetailWinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM              param
                                            )
{

    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    MMI_CTRL_ID_T       ctrl_id = MMIWIFI_DETAIL_CTRL_ID;
    GUI_POINT_T         dis_point = {0};
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        WifiSetDetail(ctrl_id, (MMIWIFI_PROFILE_T *)MMK_GetWinAddDataPtr(win_id));
        GUIRICHTEXT_SetFocusMode(ctrl_id, GUIRICHTEXT_FOCUS_SHIFT_NONE);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        //set softkey
        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL,TXT_NULL,STXT_RETURN,FALSE);
        break;
        
    case MSG_FULL_PAINT:
        lcd_dev_info.lcd_id   = GUI_MAIN_LCD_ID;
        lcd_dev_info.block_id = GUI_BLOCK_MAIN;
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
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return result;
}

/****************************************************************************/
//  Description : handle wifi new window
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
/****************************************************************************/
//  Description : WifiHandleAuthSettingWEPWinMsg
//  Global resource dependence : 
//  Author:
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleAuthSettingWEPWinMsg(
                                        MMI_WIN_ID_T    win_id, 
                                        MMI_MESSAGE_ID_E   msg_id, 
                                        DPARAM             param
                                        )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        WifiSetEditWEPWindow();

        GUIEDIT_SetIm(MMIWIFI_EDIT_WEP_KEY_EDIT_CTRL_ID, GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL,GUIIM_TYPE_ABC); /*lint !e655*/
#ifndef MMI_GUI_STYLE_TYPICAL
        MMK_SetAtvCtrl(win_id,MMIWIFI_EDIT_WEP_KEY_EDIT_CTRL_ID);
#else
        MMK_SetAtvCtrl(win_id,MMIWIFI_EDIT_WEP_KEY_OPEN_DROPDOWNLIST_CTRL_ID);
#endif /* MMI_GUI_STYLE_TYPICAL */
        break;        
        
#ifndef MMI_GUI_STYLE_TYPICAL
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        {
            MMI_CTRL_ID_T       ctrl_id = 0;

            if (PNULL != param)
            {
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
                else if (ctrl_id == MMIWLAN_LIST_WEP_STATE_CTRL_ID)
                {
                    uint32              index = 0;
                    SETTING_ITEM_FUNC   item_func = PNULL;

                    index = GUILIST_GetCurItemIndex(MMIWLAN_LIST_WEP_STATE_CTRL_ID);
                    item_func = (SETTING_ITEM_FUNC)WifiGetListUserdata(MMIWLAN_LIST_WEP_STATE_CTRL_ID, index);

                    if (PNULL != item_func)
                    {
                        (*item_func)();
                    }
                }
            }
        }
        break;

    case MSG_FULL_PAINT:
        WifiSetEditWEPWindow();
        break;
#endif /* MMI_GUI_STYLE_TYPICAL */

    case MSG_NOTIFY_DROPDOWNLIST_SELECTED:
        //改变EDIT控件可输入长度
        //改变密钥项时，各控件如何显示
        {
            uint16 index1 = 0;
            uint16 index2 = 0;
            uint16 index3 = 0;
            BOOLEAN is_need_clear = FALSE;
            
            index1 = GUIDROPDOWNLIST_GetCurItemIndex(MMIWIFI_EDIT_WEP_KEY_INDEX_DROPDOWNLIST_CTRL_ID);
            index2 = GUIDROPDOWNLIST_GetCurItemIndex(MMIWIFI_EDIT_WEP_ENCRYPT_DROPDOWNLIST_CTRL_ID);
            index3 = GUIDROPDOWNLIST_GetCurItemIndex(MMIWIFI_EDIT_WEP_KEY_STATE_DROPDOWNLIST_CTRL_ID);
            if(MMIWIFI_EDIT_WEP_KEY_INDEX_DROPDOWNLIST_CTRL_ID == MMK_GetActiveCtrlId(win_id))
            {
                if(index1 != WifiGetWepCurrIndex())
                {
                    GUIDROPDOWNLIST_SetCurItemIndex(MMIWIFI_EDIT_WEP_KEY_STATE_DROPDOWNLIST_CTRL_ID, 1);//ASCII
                    GUIDROPDOWNLIST_SetCurItemIndex(MMIWIFI_EDIT_WEP_ENCRYPT_DROPDOWNLIST_CTRL_ID, WIFISUPP_WEP_KEY_TYPE_64BITS);//64位
                }
            }
            if (index1 != WifiGetWepCurrIndex())
            {
                WifiSetWepCurrIndex(index1);
                is_need_clear = TRUE;
            }
            if (index2 != WifiGetWepKeyType())
            {
                WifiSetWepKeyType((WIFISUPP_WEP_KEY_TYPE_E)index2);
                is_need_clear = TRUE;
            }
            if (index3 != WifiGetIsWepAscii(index1))
            {
                WifiSetIsWepAscii(index1, (BOOLEAN)index3);
                is_need_clear = TRUE;
            }         
            if (is_need_clear)
            {   
                GUIEDIT_ClearAllStr(MMIWIFI_EDIT_WEP_KEY_EDIT_CTRL_ID);
            }

            if(0 == index2)
            {//64位
                if(1 == index3)
                {//ASCII
                    GUIEDIT_SetMaxLen(MMIWIFI_EDIT_WEP_KEY_EDIT_CTRL_ID,MMIWIFI_WEP_64BIT_KEY_LEN);
                }
                else
                {//HEX
                    GUIEDIT_SetMaxLen(MMIWIFI_EDIT_WEP_KEY_EDIT_CTRL_ID,MMIWIFI_WEP_64BIT_KEY_LEN*2);
                }
            }
            else
            {//128位
                if(1 == index3)
                {//ASCII
                    GUIEDIT_SetMaxLen(MMIWIFI_EDIT_WEP_KEY_EDIT_CTRL_ID,MMIWIFI_WEP_128BIT_KEY_LEN);
                }
                else
                {//HEX
                    GUIEDIT_SetMaxLen(MMIWIFI_EDIT_WEP_KEY_EDIT_CTRL_ID,MMIWIFI_WEP_128BIT_KEY_LEN*2);
                }
            }
        }
        break;
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
        {
            MMI_STRING_T str_ptr = {0};
            uint16 wep_index = 0;
            uint16 cur_index2 = 0;
            uint16 cur_index3 = 0;
            uint16 open_index = 0;

#ifndef MMI_GUI_STYLE_TYPICAL
            open_index = WifiGetIsWepOpen();
            wep_index = WifiGetWepCurrIndex();//选择的WEP密钥
            cur_index2 = (uint16)WifiGetWepKeyType();//WEP加密
            cur_index3 =  (uint16)(WifiGetIsWepAscii(WifiGetWepCurrIndex()));
#else
            open_index = GUIDROPDOWNLIST_GetCurItemIndex(MMIWIFI_EDIT_WEP_KEY_OPEN_DROPDOWNLIST_CTRL_ID);
            wep_index = GUIDROPDOWNLIST_GetCurItemIndex(MMIWIFI_EDIT_WEP_KEY_INDEX_DROPDOWNLIST_CTRL_ID);//选择的WEP密钥
            cur_index2 = GUIDROPDOWNLIST_GetCurItemIndex(MMIWIFI_EDIT_WEP_ENCRYPT_DROPDOWNLIST_CTRL_ID);//WEP加密
            cur_index3 = GUIDROPDOWNLIST_GetCurItemIndex(MMIWIFI_EDIT_WEP_KEY_STATE_DROPDOWNLIST_CTRL_ID);//WEP密钥格式
#endif /* MMI_GUI_STYLE_TYPICAL */
            GUIEDIT_GetString(MMIWIFI_EDIT_WEP_KEY_EDIT_CTRL_ID, &str_ptr);//WEP密钥
            GUIEDIT_GetString(MMIWIFI_EDIT_WEP_KEY_EDIT_CTRL_ID, &str_ptr);//WEP密钥
            if (PNULL == str_ptr.wstr_ptr || 0 == str_ptr.wstr_len)
            {
                MMIPUB_OpenAlertWarningWin(TXT_WIFI_KEY_EMPTY);
            }
            else
            {
#ifdef MMI_GUI_STYLE_TYPICAL
                WifiSetWepCurrIndex(wep_index);
                WifiSetWepKeyType((WIFISUPP_WEP_KEY_TYPE_E) cur_index2);
                WifiSetIsWepAscii(wep_index, (BOOLEAN)cur_index3);//cur_index3为1时表示ASCII格式
                WifiSetWepOpen((BOOLEAN) (0 != open_index));
#endif /* MMI_GUI_STYLE_TYPICAL */


#ifndef MMI_WLAN_AUTHORITY_COMPLEX
                BOOLEAN is_ascii = TRUE;
                BOOLEAN is_128  = TRUE;

                if (!wifi_CheckWepKeyValid(&str_ptr, &is_128, &is_ascii))
                {
                    MMIPUB_OpenAlertWinByTextId(PNULL, TXT_PASSWORD_INVALID, 0,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                    break;
                }

                cur_index2 = is_128;
                cur_index3 = is_ascii;

                WifiSetWepKeyType((WIFISUPP_WEP_KEY_TYPE_E) cur_index2);
#endif

                if(0 == cur_index2)//64位//处理密钥str_ptr
                {
                    uint8           temp_buf_char[MMIWIFI_WEP_64BIT_KEY_LEN + 1] = {0};
                    uint8           temp_hex_char[MMIWIFI_WEP_64BIT_KEY_LEN * 2 + 1] = {0};
                    
                    if(cur_index3)//ASCII
                    {
                        MMI_WSTRNTOSTR(temp_buf_char, MMIWIFI_WEP_64BIT_KEY_LEN, str_ptr.wstr_ptr, str_ptr.wstr_len, str_ptr.wstr_len);
                        
                        if (MMIWIFI_WEP_64BIT_KEY_LEN != str_ptr.wstr_len)
                        {
                            MMIPUB_OpenAlertWarningWin(TXT_WIFI_KEY_INPUT_5);
                            break;
                        }
                    }
                    else//HEX
                    {
                        MMI_WSTRNTOSTR(temp_hex_char, MMIWIFI_WEP_64BIT_KEY_LEN*2, str_ptr.wstr_ptr, str_ptr.wstr_len, str_ptr.wstr_len);
                        if (!WifiCheckIsHex(temp_hex_char,str_ptr.wstr_len))
                        {
                            MMIPUB_OpenAlertWarningWin(TXT_WIFI_KEY_HEX_INPUT);
                            break;
                        }
                        if (MMIWIFI_WEP_64BIT_KEY_LEN*2 != str_ptr.wstr_len)
                        {
                            MMIPUB_OpenAlertWarningWin(TXT_WIFI_KEY_INPUT_10);
                            break;
                        }
                        WifiHexStrToBin((const char*)temp_hex_char,temp_buf_char,MMIWIFI_WEP_64BIT_KEY_LEN);
                    }
                    WifiSetWep64Bit(wep_index,temp_buf_char);
                }
                else//128位
                {
                    uint8           temp_buf_char[MMIWIFI_WEP_128BIT_KEY_LEN + 1] = {0};
                    uint8           temp_hex_char[MMIWIFI_WEP_128BIT_KEY_LEN * 2 + 1] = {0};
                    
                    if(cur_index3)//ASCII
                    {
                        MMI_WSTRNTOSTR(temp_buf_char, MMIWIFI_WEP_128BIT_KEY_LEN, str_ptr.wstr_ptr, str_ptr.wstr_len, str_ptr.wstr_len);
                        if (MMIWIFI_WEP_128BIT_KEY_LEN != str_ptr.wstr_len)
                        {
                            MMIPUB_OpenAlertWarningWin(TXT_WIFI_KEY_INPUT_13);
                            break;
                        }
                    }
                    else//HEX
                    {
                        MMI_WSTRNTOSTR(temp_hex_char, MMIWIFI_WEP_128BIT_KEY_LEN*2, str_ptr.wstr_ptr, str_ptr.wstr_len, str_ptr.wstr_len);
                        if (!WifiCheckIsHex(temp_hex_char,str_ptr.wstr_len))
                        {
                            MMIPUB_OpenAlertWarningWin(TXT_WIFI_KEY_HEX_INPUT);
                            break;
                        }
                        if (MMIWIFI_WEP_128BIT_KEY_LEN*2 != str_ptr.wstr_len)
                        {
                            MMIPUB_OpenAlertWarningWin(TXT_WIFI_KEY_INPUT_26);
                            break;
                        }
                        WifiHexStrToBin((const char*)temp_hex_char,temp_buf_char,MMIWIFI_WEP_128BIT_KEY_LEN);
                    }
                    WifiSetWep128Bit(wep_index, temp_buf_char);
                }
                s_ui_context.s_mmiwifi_profile.is_auth_input = TRUE;
                MMK_CloseWin(win_id);
            }
        } 
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );      
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/****************************************************************************/
//  Description : WifiHandleAuthSettingWEPForConnectWinMsg
//  Global resource dependence : 
//  Author:
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleAuthSettingWEPForConnectWinMsg(
                                        MMI_WIN_ID_T    win_id, 
                                        MMI_MESSAGE_ID_E   msg_id, 
                                        DPARAM             param
                                        )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    MMIWIFI_PROFILE_T *cur_profile_ptr = PNULL;
    MMIWIFI_PROFILE_T *profile_ptr = PNULL;

    cur_profile_ptr = MMIWIFI_GetCurrProfile();
    profile_ptr = MMIWIFI_FindProfileByAP(cur_profile_ptr);

    if (PNULL == profile_ptr && MSG_CLOSE_WINDOW != msg_id)
    {
        MMK_CloseWin(win_id);

        return recode;
    }
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        profile_ptr->is_editing = TRUE; /*lint !e831 !e613*/
        cur_profile_ptr->config_info.credential.wep_credential.is_open_mode = TRUE;
        WifiSetEditWEPWindowForConnect();
        GUIEDIT_SetIm(MMIWIFI_EDIT_WEP_KEY_EDIT_CTRL_ID, GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL,GUIIM_TYPE_ABC); /*lint !e655*/
#ifndef MMI_GUI_STYLE_TYPICAL
        MMK_SetAtvCtrl(win_id,MMIWIFI_EDIT_WEP_KEY_EDIT_CTRL_ID);
#else
        MMK_SetAtvCtrl(win_id,MMIWIFI_EDIT_WEP_KEY_OPEN_DROPDOWNLIST_CTRL_ID);
#endif /* MMI_GUI_STYLE_TYPICAL */
        break;        
        
#ifndef MMI_GUI_STYLE_TYPICAL
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        {
            MMI_CTRL_ID_T       ctrl_id = 0;

            if (PNULL != param)
            {
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
                else if (ctrl_id == MMIWLAN_LIST_WEP_STATE_CTRL_ID)
                {
                    uint32              index = 0;
                    SETTING_ITEM_FUNC   item_func = PNULL;

                    index = GUILIST_GetCurItemIndex(MMIWLAN_LIST_WEP_STATE_CTRL_ID);
                    item_func = (SETTING_ITEM_FUNC)WifiGetListUserdata(MMIWLAN_LIST_WEP_STATE_CTRL_ID, index);

                    if (PNULL != item_func)
                    {
                        (*item_func)();
                    }
                }
            }
        }
        break;

    case MSG_FULL_PAINT:
        WifiSetEditWEPWindowForConnect();
        break;
#endif /* MMI_GUI_STYLE_TYPICAL */

    case MSG_NOTIFY_DROPDOWNLIST_SELECTED:
        //改变EDIT控件可输入长度
        //改变密钥项时，各控件如何显示
        {
            uint16 index1 = 0;
            uint16 index2 = 0;
            uint16 index3 = 0;
            BOOLEAN is_need_clear = FALSE;
            index1 = GUIDROPDOWNLIST_GetCurItemIndex(MMIWIFI_EDIT_WEP_KEY_INDEX_DROPDOWNLIST_CTRL_ID);
            index2 = GUIDROPDOWNLIST_GetCurItemIndex(MMIWIFI_EDIT_WEP_ENCRYPT_DROPDOWNLIST_CTRL_ID);
            index3 = GUIDROPDOWNLIST_GetCurItemIndex(MMIWIFI_EDIT_WEP_KEY_STATE_DROPDOWNLIST_CTRL_ID);
            if(MMIWIFI_EDIT_WEP_KEY_INDEX_DROPDOWNLIST_CTRL_ID == MMK_GetActiveCtrlId(win_id))
            {
                index1 = GUIDROPDOWNLIST_GetCurItemIndex(MMIWIFI_EDIT_WEP_KEY_INDEX_DROPDOWNLIST_CTRL_ID);
                if(index1 != profile_ptr->cur_wep_index) /*lint !e831 !e613*/
                {
                    GUIDROPDOWNLIST_SetCurItemIndex(MMIWIFI_EDIT_WEP_KEY_STATE_DROPDOWNLIST_CTRL_ID, 1);
                    GUIDROPDOWNLIST_SetCurItemIndex(MMIWIFI_EDIT_WEP_ENCRYPT_DROPDOWNLIST_CTRL_ID, WIFISUPP_WEP_KEY_TYPE_64BITS);
                }
            }
            if (index1 != WifiGetWepCurrIndex())
            {
                WifiSetWepCurrIndex(index1);
                is_need_clear = TRUE;
            }
            if (index2 != WifiGetWepKeyType())
            {
                WifiSetWepKeyType((WIFISUPP_WEP_KEY_TYPE_E)index2);
                is_need_clear = TRUE;
            }
            if (index3 != WifiGetIsWepAscii(index1))
            {
                WifiSetIsWepAscii(index1, (BOOLEAN)index3);
                is_need_clear = TRUE;
            }         
            if (is_need_clear)
            {   
                GUIEDIT_ClearAllStr(MMIWIFI_EDIT_WEP_KEY_EDIT_CTRL_ID);
            }
          
            if(0 == index2)
            {//64位
                if(1 == index3)
                {//ASCII
                    GUIEDIT_SetMaxLen(MMIWIFI_EDIT_WEP_KEY_EDIT_CTRL_ID,MMIWIFI_WEP_64BIT_KEY_LEN);
                }
                else
                {//HEX
                    GUIEDIT_SetMaxLen(MMIWIFI_EDIT_WEP_KEY_EDIT_CTRL_ID,MMIWIFI_WEP_64BIT_KEY_LEN*2);
                }
            }
            else
            {//128位
                if(1 == index3)
                {//ASCII
                    GUIEDIT_SetMaxLen(MMIWIFI_EDIT_WEP_KEY_EDIT_CTRL_ID,MMIWIFI_WEP_128BIT_KEY_LEN);
                }
                else
                {//HEX
                    GUIEDIT_SetMaxLen(MMIWIFI_EDIT_WEP_KEY_EDIT_CTRL_ID,MMIWIFI_WEP_128BIT_KEY_LEN*2);
                }
            }
        }
        break;
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
        {
            MMI_STRING_T str_ptr = {0};
            uint16 wep_index = 0;
            uint16 cur_index2 = 0;
            uint16 cur_index3 = 0;
            uint16 open_index = 0;
                     
            GUIEDIT_GetString(MMIWIFI_EDIT_WEP_KEY_EDIT_CTRL_ID, &str_ptr);//WEP密钥
#ifndef MMI_GUI_STYLE_TYPICAL
            open_index = cur_profile_ptr->config_info.credential.wep_credential.is_open_mode;
            wep_index = cur_profile_ptr->cur_wep_index;
            cur_index2 = profile_ptr->config_info.credential.wep_credential.key_type; /*lint !e831 !e613*/
            cur_index3 = profile_ptr->is_wep_ascii[cur_profile_ptr->cur_wep_index]; /*lint !e831 !e613*/
#ifndef MMI_WLAN_AUTHORITY_COMPLEX
            cur_index3 = TRUE;
#endif /* MMI_WLAN_AUTHORITY_COMPLEX */
#else
            open_index = GUIDROPDOWNLIST_GetCurItemIndex(MMIWIFI_EDIT_WEP_KEY_OPEN_DROPDOWNLIST_CTRL_ID);
            wep_index = GUIDROPDOWNLIST_GetCurItemIndex(MMIWIFI_EDIT_WEP_KEY_INDEX_DROPDOWNLIST_CTRL_ID);//选择的WEP密钥
            cur_index2 = GUIDROPDOWNLIST_GetCurItemIndex(MMIWIFI_EDIT_WEP_ENCRYPT_DROPDOWNLIST_CTRL_ID);//WEP加密
            cur_index3 = GUIDROPDOWNLIST_GetCurItemIndex(MMIWIFI_EDIT_WEP_KEY_STATE_DROPDOWNLIST_CTRL_ID);//WEP密钥格式
#endif /* MMI_GUI_STYLE_TYPICAL */


            if (PNULL == str_ptr.wstr_ptr || 0 == str_ptr.wstr_len)
            {
                MMIPUB_OpenAlertWarningWin(TXT_WIFI_KEY_EMPTY);
            }
            else
            {
#ifndef MMI_WLAN_AUTHORITY_COMPLEX
                BOOLEAN is_ascii = TRUE;
                BOOLEAN is_128  = TRUE;

                if (!wifi_CheckWepKeyValid(&str_ptr, &is_128, &is_ascii))
                {
                    MMIPUB_OpenAlertWinByTextId(PNULL, TXT_PASSWORD_INVALID, 0,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                    break;
                }

                cur_index2 = is_128;
                cur_index3 = is_ascii;
#endif
                profile_ptr->config_info.credential.wep_credential.is_open_mode = (BOOLEAN) (0 != open_index); /*lint !e831 !e613*/
                profile_ptr->cur_wep_index = wep_index; /*lint !e831 !e613*/
                profile_ptr->config_info.credential.wep_credential.key_in_use = wep_index; /*lint !e831 !e613*/
                profile_ptr->config_info.credential.wep_credential.key_type = (WIFISUPP_WEP_KEY_TYPE_E)cur_index2; /*lint !e831 !e613*/
                profile_ptr->is_wep_ascii[wep_index] = cur_index3;/*lint !e831 !e613*///cur_index3为1时表示ASCII格式 
                if(0 == cur_index2)//64位//处理密钥str_ptr
                {
                    uint8           temp_buf_char[MMIWIFI_WEP_64BIT_KEY_LEN + 1] = {0};
                    uint8           temp_hex_char[MMIWIFI_WEP_64BIT_KEY_LEN * 2 + 1] = {0};
                    
                    if(cur_index3)/*lint !e774*/ //ASCII
                    {
                        MMI_WSTRNTOSTR(temp_buf_char, MMIWIFI_WEP_64BIT_KEY_LEN, str_ptr.wstr_ptr, str_ptr.wstr_len, str_ptr.wstr_len);
                        
                        if (MMIWIFI_WEP_64BIT_KEY_LEN != str_ptr.wstr_len)
                        {
                            MMIPUB_OpenAlertWarningWin(TXT_WIFI_KEY_INPUT_5);
                            break;
                        }
                    }
                    else//HEX
                    {
                        MMI_WSTRNTOSTR(temp_hex_char, MMIWIFI_WEP_64BIT_KEY_LEN*2, str_ptr.wstr_ptr, str_ptr.wstr_len, str_ptr.wstr_len);
                        if (!WifiCheckIsHex(temp_hex_char,str_ptr.wstr_len))
                        {
                            MMIPUB_OpenAlertWarningWin(TXT_WIFI_KEY_HEX_INPUT);
                            break;
                        }
                        if (MMIWIFI_WEP_64BIT_KEY_LEN*2 != str_ptr.wstr_len)
                        {
                            MMIPUB_OpenAlertWarningWin(TXT_WIFI_KEY_INPUT_10);
                            break;
                        }
                        WifiHexStrToBin((const char*)temp_hex_char,temp_buf_char,MMIWIFI_WEP_64BIT_KEY_LEN);
                    }
                    SCI_MEMCPY(profile_ptr->config_info.credential.wep_credential.key.key_64bits_arr[wep_index], temp_buf_char, MMIWIFI_WEP_64BIT_KEY_LEN); /*lint !e831 !e613*/
                }
                else//128位
                {
                    uint8           temp_buf_char[MMIWIFI_WEP_128BIT_KEY_LEN + 1] = {0};
                    uint8           temp_hex_char[MMIWIFI_WEP_128BIT_KEY_LEN * 2 + 1] = {0};
                    
                    if(cur_index3)/*lint !e774*/ //ASCII
                    {
                        MMI_WSTRNTOSTR(temp_buf_char, MMIWIFI_WEP_128BIT_KEY_LEN, str_ptr.wstr_ptr, str_ptr.wstr_len, str_ptr.wstr_len);
                        if (MMIWIFI_WEP_128BIT_KEY_LEN != str_ptr.wstr_len)
                        {
                            MMIPUB_OpenAlertWarningWin(TXT_WIFI_KEY_INPUT_13);
                            break;
                        }
                    }
                    else//HEX
                    {
                        MMI_WSTRNTOSTR(temp_hex_char, MMIWIFI_WEP_128BIT_KEY_LEN*2, str_ptr.wstr_ptr, str_ptr.wstr_len, str_ptr.wstr_len);
                        if (!WifiCheckIsHex(temp_hex_char,str_ptr.wstr_len))
                        {
                            MMIPUB_OpenAlertWarningWin(TXT_WIFI_KEY_HEX_INPUT);
                            break;
                        }
                        if (MMIWIFI_WEP_128BIT_KEY_LEN*2 != str_ptr.wstr_len)
                        {
                            MMIPUB_OpenAlertWarningWin(TXT_WIFI_KEY_INPUT_26);
                            break;
                        }
                        WifiHexStrToBin((const char*)temp_hex_char,temp_buf_char,MMIWIFI_WEP_128BIT_KEY_LEN);
                    }
                    SCI_MEMCPY(profile_ptr->config_info.credential.wep_credential.key.key_128bits_arr[wep_index], temp_buf_char, MMIWIFI_WEP_128BIT_KEY_LEN);/*lint !e831 !e613*/
                }
                MMK_CloseWin(win_id);
                MMIWIFI_OpenConnectWaitWin();
                (void)MMIWIFI_DirectConnect(&(profile_ptr->config_info));/*lint !e831 !e613*/
                profile_ptr->state = MMIWIFI_PROFILE_STATE_CONNECTING;/*lint !e831 !e613*/
                profile_ptr->is_editing = FALSE;/*lint !e831 !e613*/
            }
        } 
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        profile_ptr->is_editing = FALSE;/*lint !e831 !e613*/
        MMK_CloseWin( win_id );      
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}                                        

/****************************************************************************/
//  Description : WifiHandleAuthSettingWPAWPA2WinMsg
//  Global resource dependence : 
//  Author:
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleAuthSettingWPAWPA2WinMsg(
                                        MMI_WIN_ID_T    win_id, 
                                        MMI_MESSAGE_ID_E   msg_id, 
                                        DPARAM             param
                                        )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIEDIT_SetIm(MMIWIFI_EDIT_WPA_WPA2_PRESHAREKEY_EDIT_CTRL_ID, GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL,GUIIM_TYPE_ABC); /*lint !e655*/
        WifiSetEditWPAWPA2Window();
        MMK_SetAtvCtrl(win_id,MMIWIFI_EDIT_WPA_WPA2_PRESHAREKEY_EDIT_CTRL_ID);
        break;        

#ifndef MMI_GUI_STYLE_TYPICAL
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        {
            MMI_CTRL_ID_T       ctrl_id = 0;

            if (PNULL != param)
            {
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
                else if (ctrl_id == MMIWLAN_LIST_WPA_STATE_CTRL_ID)
                {
                    uint32              index = 0;
                    SETTING_ITEM_FUNC   item_func = PNULL;

                    index = GUILIST_GetCurItemIndex(MMIWLAN_LIST_WPA_STATE_CTRL_ID);
                    item_func = (SETTING_ITEM_FUNC)WifiGetListUserdata(MMIWLAN_LIST_WPA_STATE_CTRL_ID, index);

                    if (PNULL != item_func)
                    {
                        (*item_func)();
                    }
                }
            }
        }
        break;

    case MSG_FULL_PAINT:
        WifiSetEditWPAWPA2Window();
        break;
#endif /* MMI_GUI_STYLE_TYPICAL */

    case MSG_NOTIFY_DROPDOWNLIST_SELECTED:
        {
            uint16 index = 0;
            index = GUIDROPDOWNLIST_GetCurItemIndex(MMIWIFI_EDIT_WPA_WPA2_PRESHAREKEY_STATE_DROPDOWNLIST_CTRL_ID);
            if(index != WifiGetIsWpaAscii())
            {
                GUIEDIT_ClearAllStr(MMIWIFI_EDIT_WPA_WPA2_PRESHAREKEY_EDIT_CTRL_ID);
            }
            if(index)
            {
                GUIEDIT_SetMaxLen(MMIWIFI_EDIT_WPA_WPA2_PRESHAREKEY_EDIT_CTRL_ID,WIFISUPP_WPA_PSK_LEN - 1);
            }
            else
            {
                GUIEDIT_SetMaxLen(MMIWIFI_EDIT_WPA_WPA2_PRESHAREKEY_EDIT_CTRL_ID,WIFISUPP_WPA_PSK_LEN);
            }
        }
        break;
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
        {
            MMI_STRING_T str_ptr = {0};
            uint16 cur_index1 = 0;
            
            uint8 temp_buf_char[WIFISUPP_WPA_PSK_LEN + 1] = {0};
            uint8 temp_hex_char[WIFISUPP_WPA_PSK_LEN + 1] = {0};        

            GUIEDIT_GetString(MMIWIFI_EDIT_WPA_WPA2_PRESHAREKEY_EDIT_CTRL_ID, &str_ptr);//预共享密钥

#ifndef MMI_GUI_STYLE_TYPICAL
            cur_index1 = WifiGetIsWpaAscii();
            WifiSetWpaCredential(WIFISUPP_WPA_CREDENTIAL_TYPE_PSK);
#else
            cur_index1 = GUIDROPDOWNLIST_GetCurItemIndex(MMIWIFI_EDIT_WPA_WPA2_PRESHAREKEY_STATE_DROPDOWNLIST_CTRL_ID);//预共享密钥格式
            WifiSetWpaCredential(WIFISUPP_WPA_CREDENTIAL_TYPE_PSK);
            WifiSetIsWpaAscii((BOOLEAN)cur_index1);
#endif /* MMI_GUI_STYLE_TYPICAL */
            if (PNULL == str_ptr.wstr_ptr || 0 == str_ptr.wstr_len)
            {
                MMIPUB_OpenAlertWarningWin(TXT_WIFI_KEY_EMPTY);
            }
            else
            {
                 if(!cur_index1)
                {
                    if (str_ptr.wstr_len < 8 || str_ptr.wstr_len > 64)
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_WIFI_WPA_HEX_RANGE_OUT);
                        break;
                    }
                    MMI_WSTRNTOSTR(temp_hex_char, WIFISUPP_WPA_PSK_LEN, str_ptr.wstr_ptr, str_ptr.wstr_len, str_ptr.wstr_len);
                    if (!WifiCheckIsHex(temp_hex_char,str_ptr.wstr_len))
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_WIFI_KEY_HEX_INPUT);
                        break;
                    }
                    WifiHexStrToBin((const char*)temp_hex_char,temp_buf_char,WIFISUPP_WPA_PSK_LEN / 2);
                    WifiSetWpaPsk(temp_buf_char);
                }
                else
                {
                    if (str_ptr.wstr_len < 8 || str_ptr.wstr_len > 63)
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_WIFI_KEY_RANGE_OUT);
                        break;
                    }
                    MMI_WSTRNTOSTR(temp_buf_char, WIFISUPP_WPA_PSK_LEN, str_ptr.wstr_ptr, str_ptr.wstr_len, str_ptr.wstr_len);
                    WifiSetWpaPsk(temp_buf_char);
                }
                s_ui_context.s_mmiwifi_profile.is_auth_input = TRUE;
                MMK_CloseWin( win_id ); 
            }                
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );      
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/****************************************************************************/
//  Description : WifiHandleAuthSettingWPAWPA2ForConnectWinMsg
//  Global resource dependence : 
//  Author:
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleAuthSettingWPAWPA2ForConnectWinMsg(
                                        MMI_WIN_ID_T    win_id, 
                                        MMI_MESSAGE_ID_E   msg_id, 
                                        DPARAM             param
                                        )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    MMIWIFI_PROFILE_T *cur_profile_ptr = PNULL;
    MMIWIFI_PROFILE_T *profile_ptr = PNULL;

    cur_profile_ptr = MMIWIFI_GetCurrProfile();
    profile_ptr = MMIWIFI_FindProfileByAP(cur_profile_ptr);

    if (PNULL == profile_ptr && MSG_CLOSE_WINDOW != msg_id)
    {
        MMK_CloseWin(win_id);

        return recode;
    }
    else if (PNULL == profile_ptr)
    {
        return recode;
    }
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        profile_ptr->is_editing = TRUE;/*lint !e831 !e613*/
        cur_profile_ptr->is_wpa_ascii = TRUE;
        WifiSetEditWPAWPA2WindowForConnect();
        GUIEDIT_SetIm(MMIWIFI_EDIT_WPA_WPA2_PRESHAREKEY_EDIT_CTRL_ID, GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL,GUIIM_TYPE_ABC); /*lint !e655*/
        MMK_SetAtvCtrl(win_id,MMIWIFI_EDIT_WPA_WPA2_PRESHAREKEY_EDIT_CTRL_ID);
        break;        

    case MSG_NOTIFY_DROPDOWNLIST_SELECTED:
        {
            uint16 index = 0;
            index = GUIDROPDOWNLIST_GetCurItemIndex(MMIWIFI_EDIT_WPA_WPA2_PRESHAREKEY_STATE_DROPDOWNLIST_CTRL_ID);
            if(index != profile_ptr->is_wpa_ascii)/*lint !e831 !e613*/
            {
                GUIEDIT_ClearAllStr(MMIWIFI_EDIT_WPA_WPA2_PRESHAREKEY_EDIT_CTRL_ID);
            }
            if(index)
            {
                GUIEDIT_SetMaxLen(MMIWIFI_EDIT_WPA_WPA2_PRESHAREKEY_EDIT_CTRL_ID,WIFISUPP_WPA_PSK_LEN);
            }
            else
            {
                GUIEDIT_SetMaxLen(MMIWIFI_EDIT_WPA_WPA2_PRESHAREKEY_EDIT_CTRL_ID,WIFISUPP_WPA_PSK_LEN);
            }
        }
        break;
        
#ifndef MMI_GUI_STYLE_TYPICAL
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        {
            MMI_CTRL_ID_T       ctrl_id = 0;

            if (PNULL != param)
            {
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
                else if (ctrl_id == MMIWLAN_LIST_WPA_STATE_CTRL_ID)
                {
                    uint32              index = 0;
                    SETTING_ITEM_FUNC   item_func = PNULL;

                    index = GUILIST_GetCurItemIndex(MMIWLAN_LIST_WPA_STATE_CTRL_ID);
                    item_func = (SETTING_ITEM_FUNC)WifiGetListUserdata(MMIWLAN_LIST_WPA_STATE_CTRL_ID, index);

                    if (PNULL != item_func)
                    {
                        (*item_func)();
                    }
                }
            }
        }
        break;

    case MSG_FULL_PAINT:
        WifiSetEditWPAWPA2WindowForConnect();
        break;
#endif /* MMI_GUI_STYLE_TYPICAL */

    case MSG_CTL_OK:
    case MSG_APP_OK:
        {
            MMI_STRING_T str_ptr = {0};
            uint16 cur_index1 = 0;
            
            uint8 temp_buf_char[WIFISUPP_WPA_PSK_LEN + 1] = {0};
            uint8 temp_hex_char[WIFISUPP_WPA_PSK_LEN  + 1] = {0};

            GUIEDIT_GetString(MMIWIFI_EDIT_WPA_WPA2_PRESHAREKEY_EDIT_CTRL_ID, &str_ptr);//预共享密钥

#ifndef MMI_GUI_STYLE_TYPICAL
            profile_ptr->config_info.credential.wpa_credential.credential_type = WIFISUPP_WPA_CREDENTIAL_TYPE_PSK;/*lint !e831 !e613*/
            profile_ptr->is_wpa_ascii = cur_profile_ptr->is_wpa_ascii;/*lint !e831 !e613*/
            cur_index1 = cur_profile_ptr->is_wpa_ascii;
#ifndef MMI_WLAN_AUTHORITY_COMPLEX
            profile_ptr->is_wpa_ascii = TRUE;
            cur_index1 = TRUE;
#endif /* MMI_WLAN_AUTHORITY_COMPLEX */
#else
            cur_index1 = GUIDROPDOWNLIST_GetCurItemIndex(MMIWIFI_EDIT_WPA_WPA2_PRESHAREKEY_STATE_DROPDOWNLIST_CTRL_ID);//预共享密钥格式/*lint !e831 !e613*/
            
            profile_ptr->config_info.credential.wpa_credential.credential_type = WIFISUPP_WPA_CREDENTIAL_TYPE_PSK;
            profile_ptr->is_wpa_ascii = (BOOLEAN)cur_index1;/*lint !e831 !e613*/
#endif /* MMI_GUI_STYLE_TYPICAL */

            if (PNULL == str_ptr.wstr_ptr || 0 == str_ptr.wstr_len)
            {
                MMIPUB_OpenAlertWarningWin(TXT_WIFI_KEY_EMPTY);
                break;
            }
            else
            {
                if(!cur_index1)/*lint !e774*/
                {
                    if (str_ptr.wstr_len < 8 || str_ptr.wstr_len > 64)
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_WIFI_WPA_HEX_RANGE_OUT);
                        break;
                    }
                    MMI_WSTRNTOSTR(temp_hex_char, WIFISUPP_WPA_PSK_LEN, str_ptr.wstr_ptr, str_ptr.wstr_len, str_ptr.wstr_len);
                    if (!WifiCheckIsHex(temp_hex_char,str_ptr.wstr_len))
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_WIFI_KEY_HEX_INPUT);
                        break;
                    }
                    WifiHexStrToBin((const char*)temp_hex_char,temp_buf_char,WIFISUPP_WPA_PSK_LEN / 2);
                    SCI_MEMSET(profile_ptr->config_info.credential.wpa_credential.credential_info.wpa_psk_info.psk,0,WIFISUPP_WPA_PSK_LEN);/*lint !e831 !e613*/
                    SCI_MEMCPY(profile_ptr->config_info.credential.wpa_credential.credential_info.wpa_psk_info.psk,temp_buf_char,WIFISUPP_WPA_PSK_LEN);/*lint !e831 !e613*/
                }
                else
                {
                    if (str_ptr.wstr_len < 8 || str_ptr.wstr_len > 63)
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_WIFI_KEY_RANGE_OUT);
                        break;
                    }
                    MMI_WSTRNTOSTR(temp_buf_char, WIFISUPP_WPA_PSK_LEN, str_ptr.wstr_ptr, str_ptr.wstr_len, str_ptr.wstr_len);
                    SCI_MEMSET(profile_ptr->config_info.credential.wpa_credential.credential_info.wpa_psk_info.psk,0,WIFISUPP_WPA_PSK_LEN);/*lint !e831 !e613*/
                    SCI_MEMCPY(profile_ptr->config_info.credential.wpa_credential.credential_info.wpa_psk_info.psk,temp_buf_char,WIFISUPP_WPA_PSK_LEN);/*lint !e831 !e613*/
                }
            }
            MMK_CloseWin( win_id );      
            MMIWIFI_OpenConnectWaitWin();
            (void)MMIWIFI_DirectConnect(&(profile_ptr->config_info));/*lint !e831 !e613*/
            profile_ptr->state = MMIWIFI_PROFILE_STATE_CONNECTING;/*lint !e831 !e613*/
            profile_ptr->is_editing = FALSE;/*lint !e831 !e613*/
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
	    profile_ptr->is_editing = FALSE;/*lint !e831 !e613*/  
        MMK_CloseWin( win_id );      
        break;

    case MSG_CLOSE_WINDOW:
	    profile_ptr->is_editing = FALSE;/*lint !e831 !e613*/   
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/****************************************************************************/
//  Description : WifiHandleAuthSettingWAPIPskWinMsg
//  Global resource dependence : 
//  Author:
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleAuthSettingWAPIPskWinMsg(
                                        MMI_WIN_ID_T    win_id, 
                                        MMI_MESSAGE_ID_E   msg_id, 
                                        DPARAM             param
                                        )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        WifiSetEditWAPIPskWindow();
        
        MMK_SetAtvCtrl(win_id,MMIWIFI_EDIT_WAPI_PRESHAREKEY_STATE_DROPDOWNLIST_CTRL_ID);
        break;        
        
    case MSG_NOTIFY_DROPDOWNLIST_SELECTED:
        {
            uint16 index = 0;
            index = GUIDROPDOWNLIST_GetCurItemIndex(MMIWIFI_EDIT_WAPI_PRESHAREKEY_STATE_DROPDOWNLIST_CTRL_ID);
            if(index != WifiGetIsWapiAscii())
            {
                GUIEDIT_ClearAllStr(MMIWIFI_EDIT_WAPI_PRESHAREKEY_EDIT_CTRL_ID);
            }
            if(index)
            {
                GUIEDIT_SetMaxLen(MMIWIFI_EDIT_WAPI_PRESHAREKEY_EDIT_CTRL_ID,WIFISUPP_WPA_PSK_LEN);
            }
            else
            {
                GUIEDIT_SetMaxLen(MMIWIFI_EDIT_WAPI_PRESHAREKEY_EDIT_CTRL_ID,WIFISUPP_WPA_PSK_LEN*2);
            }
        }
        break;
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
        {
            MMI_STRING_T str_ptr = {0};
            uint16 cur_index1 = 0;
            
            uint8 temp_buf_char[WIFISUPP_WPA_PSK_LEN + 1] = {0};
            uint8 temp_hex_char[WIFISUPP_WPA_PSK_LEN * 2 + 1] = {0};
            
            GUIEDIT_GetString(MMIWIFI_EDIT_WAPI_PRESHAREKEY_EDIT_CTRL_ID, &str_ptr);//预共享密钥
            cur_index1 = GUIDROPDOWNLIST_GetCurItemIndex(MMIWIFI_EDIT_WAPI_PRESHAREKEY_STATE_DROPDOWNLIST_CTRL_ID);//预共享密钥格式

            WifiSetWapiCredential(WIFISUPP_WAPI_CREDENTIAL_TYPE_PSK);
            WifiSetIsWapiAscii((BOOLEAN)cur_index1);
            if (PNULL == str_ptr.wstr_ptr || 0 == str_ptr.wstr_len)
            {
                MMIPUB_OpenAlertWarningWin(TXT_WIFI_KEY_EMPTY);
            }
            else
            {
                if(!cur_index1)
                {
                    if (str_ptr.wstr_len < 16 || str_ptr.wstr_len > 126)
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_WIFI_KEY_RANGE_OUT_1);
                        break;
                    }
                    MMI_WSTRNTOSTR(temp_hex_char, WIFISUPP_WPA_PSK_LEN*2, str_ptr.wstr_ptr, str_ptr.wstr_len, str_ptr.wstr_len);
                    if (!WifiCheckIsHex(temp_hex_char,str_ptr.wstr_len))
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_WIFI_KEY_HEX_INPUT);
                        break;
                    }
                    WifiHexStrToBin((const char*)temp_hex_char,temp_buf_char,WIFISUPP_WPA_PSK_LEN);
                    WifiSetWapiPsk(temp_buf_char);
                }
                else
                {
                    if (str_ptr.wstr_len < 8 || str_ptr.wstr_len > 63)
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_WIFI_KEY_RANGE_OUT);
                        break;
                    }
                    MMI_WSTRNTOSTR(temp_buf_char, WIFISUPP_WPA_PSK_LEN, str_ptr.wstr_ptr, str_ptr.wstr_len, str_ptr.wstr_len);
                    WifiSetWapiPsk(temp_buf_char);
                }
                s_ui_context.s_mmiwifi_profile.is_auth_input = TRUE;
                MMK_CloseWin( win_id );      
            }
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );      
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/****************************************************************************/
//  Description : WifiHandleAuthSettingWAPIPskForConnectWinMsg
//  Global resource dependence : 
//  Author:
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleAuthSettingWAPIPskForConnectWinMsg(
                                        MMI_WIN_ID_T    win_id, 
                                        MMI_MESSAGE_ID_E   msg_id, 
                                        DPARAM             param
                                        )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    MMIWIFI_PROFILE_T *cur_profile_ptr = PNULL;
    MMIWIFI_PROFILE_T *profile_ptr = PNULL;

    cur_profile_ptr = MMIWIFI_GetCurrProfile();
    profile_ptr = MMIWIFI_FindProfileByAP(cur_profile_ptr);

    if (PNULL == profile_ptr && MSG_CLOSE_WINDOW != msg_id)
    {
        MMK_CloseWin(win_id);

        return recode;
    }

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        if (PNULL !=profile_ptr)
        {
            profile_ptr->is_editing = TRUE;/*lint !e831 !e613*/
    #ifndef MMI_WLAN_AUTHORITY_COMPLEX
            profile_ptr->is_wapi_ascii = TRUE;
    #endif /* MMI_WLAN_AUTHORITY_COMPLEX */
        }    
        WifiSetEditWAPIPskForConnectWindow();
        GUIEDIT_SetIm(MMIWIFI_EDIT_WAPI_PRESHAREKEY_EDIT_CTRL_ID, GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL,GUIIM_TYPE_ABC); /*lint !e655*/
#ifndef MMI_GUI_STYLE_TYPICAL
        MMK_SetAtvCtrl(win_id,MMIWLAN_LIST_WAPI_STATE_CTRL_ID);
#else
        MMK_SetAtvCtrl(win_id,MMIWIFI_EDIT_WAPI_PRESHAREKEY_STATE_DROPDOWNLIST_CTRL_ID);
#endif /* MMI_GUI_STYLE_TYPICAL */
        break;        
        
#ifndef MMI_GUI_STYLE_TYPICAL
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        {
            MMI_CTRL_ID_T       ctrl_id = 0;

            if (PNULL != param)
            {
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
                else if (ctrl_id == MMIWLAN_LIST_WAPI_STATE_CTRL_ID)
                {
                    uint32              index = 0;
                    SETTING_ITEM_FUNC   item_func = PNULL;

                    index = GUILIST_GetCurItemIndex(MMIWLAN_LIST_WAPI_STATE_CTRL_ID);
                    item_func = (SETTING_ITEM_FUNC)WifiGetListUserdata(MMIWLAN_LIST_WAPI_STATE_CTRL_ID, index);

                    if (PNULL != item_func)
                    {
                        (*item_func)();
                    }
                }
            }
        }
        break;

    case MSG_FULL_PAINT:
        WifiSetEditWAPIPskForConnectWindow();
        break;
#endif /* MMI_GUI_STYLE_TYPICAL */

    case MSG_NOTIFY_DROPDOWNLIST_SELECTED:
        {
            uint16 index = 0;
            index = GUIDROPDOWNLIST_GetCurItemIndex(MMIWIFI_EDIT_WAPI_PRESHAREKEY_STATE_DROPDOWNLIST_CTRL_ID);
            if(index != profile_ptr->is_wapi_ascii)/*lint !e831 !e613*/
            {
                GUIEDIT_ClearAllStr(MMIWIFI_EDIT_WAPI_PRESHAREKEY_EDIT_CTRL_ID);
            }
            if(index)
            {
                GUIEDIT_SetMaxLen(MMIWIFI_EDIT_WAPI_PRESHAREKEY_EDIT_CTRL_ID,WIFISUPP_WPA_PSK_LEN);
            }
            else
            {
                GUIEDIT_SetMaxLen(MMIWIFI_EDIT_WAPI_PRESHAREKEY_EDIT_CTRL_ID,WIFISUPP_WPA_PSK_LEN*2);
            }
        }
        break;
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
        {
            MMI_STRING_T str_ptr = {0};
            uint16 cur_index1 = 0;
            
            uint8 temp_buf_char[WIFISUPP_WPA_PSK_LEN + 1] = {0};
            uint8 temp_hex_char[WIFISUPP_WPA_PSK_LEN * 2 + 1] = {0};
            
            GUIEDIT_GetString(MMIWIFI_EDIT_WAPI_PRESHAREKEY_EDIT_CTRL_ID, &str_ptr);//预共享密钥

#ifndef MMI_GUI_STYLE_TYPICAL
            cur_index1 = cur_profile_ptr->is_wapi_ascii;//预共享密钥格式
#ifndef MMI_WLAN_AUTHORITY_COMPLEX
            cur_index1 = TRUE;
#endif /* MMI_WLAN_AUTHORITY_COMPLEX */
#else
            cur_index1 = GUIDROPDOWNLIST_GetCurItemIndex(MMIWIFI_EDIT_WAPI_PRESHAREKEY_STATE_DROPDOWNLIST_CTRL_ID);//预共享密钥格式
#endif /* MMI_GUI_STYLE_TYPICAL */


            profile_ptr->config_info.credential.wapi_credential.credential_type = WIFISUPP_WAPI_CREDENTIAL_TYPE_PSK;/*lint !e831 !e613*/
            profile_ptr->is_wapi_ascii = (BOOLEAN)cur_index1;/*lint !e831 !e613*/
            if (PNULL == str_ptr.wstr_ptr || 0 == str_ptr.wstr_len)
            {
                MMIPUB_OpenAlertWarningWin(TXT_WIFI_KEY_EMPTY);
                break;
            }
            else
            {
                if(!cur_index1)/*lint !e774*/
                {

                        if (str_ptr.wstr_len < 16 || str_ptr.wstr_len > 126)
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_WIFI_KEY_RANGE_OUT_1);
                        break;
                    }
                    MMI_WSTRNTOSTR(temp_hex_char, WIFISUPP_WPA_PSK_LEN*2, str_ptr.wstr_ptr, str_ptr.wstr_len, str_ptr.wstr_len);
                    if (!WifiCheckIsHex(temp_hex_char,str_ptr.wstr_len))
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_WIFI_KEY_HEX_INPUT);
                        break;
                    }
                    WifiHexStrToBin((const char*)temp_hex_char,temp_buf_char,WIFISUPP_WPA_PSK_LEN);
                    SCI_MEMSET(profile_ptr->config_info.credential.wapi_credential.credential_info.wapi_psk_info.psk,0,WIFISUPP_WPA_PSK_LEN);/*lint !e831 !e613*/
                    SCI_MEMCPY(profile_ptr->config_info.credential.wapi_credential.credential_info.wapi_psk_info.psk,temp_buf_char,WIFISUPP_WPA_PSK_LEN);/*lint !e831 !e613*/
                }
                else
                {
                    if (str_ptr.wstr_len < 8 || str_ptr.wstr_len > 63)
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_WIFI_KEY_RANGE_OUT);
                        break;
                    }
                    MMI_WSTRNTOSTR(temp_buf_char, WIFISUPP_WPA_PSK_LEN, str_ptr.wstr_ptr, str_ptr.wstr_len, str_ptr.wstr_len);

                    SCI_MEMSET(profile_ptr->config_info.credential.wapi_credential.credential_info.wapi_psk_info.psk,0,WIFISUPP_WPA_PSK_LEN);/*lint !e831 !e613*/
                    SCI_MEMCPY(profile_ptr->config_info.credential.wapi_credential.credential_info.wapi_psk_info.psk,temp_buf_char,WIFISUPP_WPA_PSK_LEN);/*lint !e831 !e613*/
                }
            }
            MMK_CloseWin( win_id );
            MMIWIFI_OpenConnectWaitWin();
            (void)MMIWIFI_DirectConnect(&(profile_ptr->config_info));/*lint !e831 !e613*/
            profile_ptr->state = MMIWIFI_PROFILE_STATE_CONNECTING;/*lint !e831 !e613*/
            profile_ptr->is_editing = FALSE;/*lint !e831 !e613*/
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        profile_ptr->is_editing = FALSE;/*lint !e831 !e613*/
        MMK_CloseWin( win_id );      
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/****************************************************************************/
//  Description : WifiHandleAuthSettingWAPICERTIFICATEWinMsg
//  Global resource dependence : 
//  Author:
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleAuthSettingWAPICERTIFICATEWinMsg(
                                        MMI_WIN_ID_T    win_id, 
                                        MMI_MESSAGE_ID_E   msg_id, 
                                        DPARAM             param
                                        )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;    

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        WifiSetEditWAPICertificateWindow();
        MMK_SetAtvCtrl(win_id, MMIWIFI_EDIT_WAPI_CLIENT_CERTIFICATE_LABEL_CTRL_ID);
        break;        

    case MSG_GET_FOCUS:
    case MSG_NOTIFY_GET_ACTIVE:
        if(MMIWIFI_LABEL_WAPI_UNINSTALL_CTRL_ID == MMK_GetActiveCtrlId(win_id))
        {
            GUIWIN_SetSoftkeyTextId(win_id,TXT_COMMON_OK, TXT_WLAN_UNINSTALL, STXT_RETURN, TRUE);
        }
        break;
        
    case MSG_FULL_PAINT:
        {
            uint16* client_path = PNULL;
            uint16* parent_path = PNULL;
            client_path = WifiGetWapiCertiClientFilePath();
            if (PNULL != client_path)
            {
                MMI_STRING_T mmi_string = {0};
                if (0 != MMIAPICOM_Wstrlen(client_path))
                {
                    mmi_string.wstr_ptr = client_path;
                    mmi_string.wstr_len = MMIAPICOM_Wstrlen(client_path);
                    GUILABEL_SetText(MMIWIFI_EDIT_WAPI_CLIENT_CERTIFICATE_LABEL_CTRL_ID, &mmi_string, FALSE);     
                }
            }
            
            parent_path = WifiGetWapiCertiParentFilePath();
            if (PNULL != parent_path)
            {
                MMI_STRING_T mmi_string = {0};
                if (0 != MMIAPICOM_Wstrlen(parent_path))
                {
                    mmi_string.wstr_ptr = parent_path;
                    mmi_string.wstr_len = MMIAPICOM_Wstrlen(parent_path);
                    GUILABEL_SetText(MMIWIFI_EDIT_WAPI_PARENT_CERTIFICATE_LBAEL_CTRL_ID, &mmi_string, FALSE);
                }
            }
            
            if ((0 == MMIAPICOM_Wstrlen(client_path)) && (0 == MMIAPICOM_Wstrlen(parent_path)))
            {
                GUIFORM_SetChildDisplay(MMIWIFI_EDIT_WAPI_CERTIFICATE_FORMCTRL_ID, MMIWIFI_CHILD_FORM_UNINSTALL_CTRL_ID, GUIFORM_CHILD_DISP_GRAY);
            }
            else
            {
                GUIFORM_SetChildDisplay(MMIWIFI_EDIT_WAPI_CERTIFICATE_FORMCTRL_ID, MMIWIFI_CHILD_FORM_UNINSTALL_CTRL_ID, GUIFORM_CHILD_DISP_NORMAL);
            }

            if(MMIWIFI_LABEL_WAPI_UNINSTALL_CTRL_ID == MMK_GetActiveCtrlId(win_id))
            {
                GUIWIN_SetSoftkeyTextId(win_id,TXT_COMMON_OK, TXT_WLAN_UNINSTALL, STXT_RETURN, FALSE);
            }
        }
        break;

    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        if(MMIWIFI_LABEL_WAPI_UNINSTALL_CTRL_ID != MMK_GetActiveCtrlId(win_id))
        {
            if((MSG_APP_WEB == msg_id) || (MSG_CTL_MIDSK == msg_id) || ((MSG_CTL_PENOK == msg_id) && (MMIWIFI_LABEL_WAPI_UNINSTALL_CTRL_ID != ((MMI_NOTIFY_T*)param)->src_id)))
            {
                if(!MMIAPIUDISK_UdiskIsRun())
                {
                    WifiOpenSelectFileWin(MMK_GetActiveCtrlId(win_id));
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
                }
            }
        }
        else
        {
            MMIPUB_OpenQueryWinByTextId(TXT_WLAN_UNINSTALL_QUERY, IMAGE_PUBWIN_QUERY, PNULL, PNULL);;
        }
        
        break;
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
        WifiSetWapiCredential(WIFISUPP_WAPI_CREDENTIAL_TYPE_CER);
        if(0 == MMIAPICOM_Wstrlen(WifiGetWapiCertiClientFilePath()) 
            || 0 == MMIAPICOM_Wstrlen(WifiGetWapiCertiParentFilePath()))
        {
            MMIPUB_OpenAlertWarningWin(TXT_WIFI_CERTIFICATE_INVALID);
        }
        else
        {
            s_ui_context.s_mmiwifi_profile.is_auth_input = TRUE;
            MMK_CloseWin( win_id );      
        }

        break;
    case MSG_PROMPTWIN_OK:
        {
            SCI_MEMSET(s_ui_context.s_mmiwifi_profile.client_file_name, 0 ,sizeof(s_ui_context.s_mmiwifi_profile.client_file_name));
            SCI_MEMSET(s_ui_context.s_mmiwifi_profile.parent_file_name, 0 ,sizeof(s_ui_context.s_mmiwifi_profile.parent_file_name));

            s_ui_context.s_mmiwifi_profile.is_auth_input = FALSE;

            MMK_SetAtvCtrl(win_id, MMIWIFI_EDIT_WAPI_CLIENT_CERTIFICATE_LABEL_CTRL_ID);
            MMIPUB_CloseQuerytWin(PNULL);

            if (MMK_IsOpenWin(MMIWIFI_EDIT_WIN_ID)) //Edit Ap
            {
                MMIWIFI_PROFILE_T       *profile_ptr = PNULL;   
                uint16                  index = 0;
                MMI_CTRL_ID_T           ctrl_id = 0;

#ifdef MMI_WLAN_TAB_STYLE
                ctrl_id = MMIWIFI_LIST_TRUSTED_CTRL_ID;
#else
                ctrl_id = MMIWIFI_LIST_CTRL_ID;
#endif /* MMI_WLAN_TAB_STYLE */

                index = GUILIST_GetCurItemIndex(ctrl_id);

                profile_ptr = (MMIWIFI_PROFILE_T *)WifiGetListUserdata(ctrl_id, index);

                if (!WifiListIsAPItemSelected(index) || PNULL == profile_ptr)
                {
                    //SCI_TRACE_LOW:"WifiHandleAuthSettingWAPICERTIFICATEWinMsg profile_ptr is Error index = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_6609_112_2_18_3_10_5_200,(uint8*)"d", index);
                    break;
                }

                if(MMIAPICOM_Wstrlen(profile_ptr->client_file_name) && MMIAPICOM_Wstrlen(profile_ptr->parent_file_name) )
                {
                    SCI_MEMSET(profile_ptr->client_file_name, 0, sizeof(profile_ptr->client_file_name));
                    SCI_MEMSET(profile_ptr->parent_file_name, 0, sizeof(profile_ptr->parent_file_name));
                    profile_ptr->is_trusted = FALSE;
                    MMIWIFI_UpdateProfileFile();
                }
            }

#ifdef MMI_GUI_STYLE_TYPICAL
            MMIPUB_OpenAlertWinByTextId(PNULL, TXT_WLAN_UNINSTALL_SUCC, 0, IMAGE_PUBWIN_SUCCESS, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
#endif 
        }
        break;

    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
        break;
 
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );      
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/****************************************************************************/
//  Description : WifiHandleAuthSettingWAPICERTIFICATEForConnectWinMsg
//  Global resource dependence : 
//  Author:
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleAuthSettingWAPICERTIFICATEForConnectWinMsg(
                                        MMI_WIN_ID_T    win_id, 
                                        MMI_MESSAGE_ID_E   msg_id, 
                                        DPARAM             param
                                        )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;             
    MMIWIFI_PROFILE_T   *profile_ptr = PNULL;
    SFS_HANDLE          file_handle = 0;
    uint16              *file_name = PNULL;
    uint32              buf_len1 = 0;
    uint32              buf_len2 = 0;
    uint32              read_len = 0;
    BOOLEAN             is_cer_ok = FALSE;

    profile_ptr = MMIWIFI_GetCurrProfile();

    if (PNULL == profile_ptr && MSG_CLOSE_WINDOW != msg_id)
    {
        MMK_CloseWin(win_id);

        return recode;
    }

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        if (PNULL != profile_ptr)
        {
            profile_ptr->is_editing = TRUE;
        }
        WifiSetEditWAPICertificateForConnectWindow();
        MMK_SetAtvCtrl(win_id, MMIWIFI_EDIT_WAPI_CLIENT_CERTIFICATE_LABEL_CTRL_ID);
        break;        

#ifndef MMI_GUI_STYLE_TYPICAL
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
                else
                {
                    if (MMIWIFI_LABEL_WAPI_UNINSTALL_CTRL_ID != MMK_GetActiveCtrlId(win_id))
                    {
                        if(!MMIAPIUDISK_UdiskIsRun())
                        {
                            WifiOpenSelectFileForConnectWin(MMK_GetActiveCtrlId(win_id));
                        }
                        else
                        {
                            MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
                        }              
                    }
                    else
                    {
                        MMIPUB_OpenQueryWinByTextId(TXT_WLAN_UNINSTALL_QUERY, IMAGE_PUBWIN_QUERY, PNULL, PNULL);;
                    }
                }
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527        
#endif /* MMI_GUI_STYLE_TYPICAL */

    case MSG_GET_FOCUS:
        break;

    case MSG_FULL_PAINT:
        {
            MMI_STRING_T mmi_string = {0};
            uint16* client_path = PNULL;
            uint16* parent_path = PNULL;

            client_path = profile_ptr->client_file_name;/*lint !e831 !e613*/
            mmi_string.wstr_ptr = client_path;
            mmi_string.wstr_len = MMIAPICOM_Wstrlen(client_path);
            GUILABEL_SetText(MMIWIFI_EDIT_WAPI_CLIENT_CERTIFICATE_LABEL_CTRL_ID, &mmi_string, FALSE);

            parent_path = profile_ptr->parent_file_name;/*lint !e831 !e613*/
            mmi_string.wstr_ptr = parent_path;
            mmi_string.wstr_len = MMIAPICOM_Wstrlen(parent_path);
            GUILABEL_SetText(MMIWIFI_EDIT_WAPI_PARENT_CERTIFICATE_LBAEL_CTRL_ID, &mmi_string, FALSE);

            if ((0 == MMIAPICOM_Wstrlen(client_path)) && (0 == MMIAPICOM_Wstrlen(parent_path)))
            {
                GUIFORM_SetChildDisplay(MMIWIFI_EDIT_WAPI_CERTIFICATE_FORMCTRL_ID, MMIWIFI_CHILD_FORM_UNINSTALL_CTRL_ID, GUIFORM_CHILD_DISP_GRAY);
            }
            else
            {
                GUIFORM_SetChildDisplay(MMIWIFI_EDIT_WAPI_CERTIFICATE_FORMCTRL_ID, MMIWIFI_CHILD_FORM_UNINSTALL_CTRL_ID, GUIFORM_CHILD_DISP_NORMAL);
            }

#ifdef MMI_GUI_STYLE_TYPICAL
            if (MMIWIFI_LABEL_WAPI_UNINSTALL_CTRL_ID != MMK_GetActiveCtrlId(win_id))
            {
                GUIWIN_SetSoftkeyTextId(win_id,STXT_SAVE, TXT_WIFI_SELECT_PATH, STXT_RETURN, FALSE);
            }
            else
            {
                GUIWIN_SetSoftkeyTextId(win_id,STXT_SAVE, TXT_WLAN_UNINSTALL, STXT_RETURN, FALSE);
            }
#endif /* MMI_GUI_STYLE_TYPICAL */


        }
        break;

    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
#ifdef MMI_GUI_STYLE_TYPICAL
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif /* MMI_GUI_STYLE_TYPICAL */
        if (MMIWIFI_LABEL_WAPI_UNINSTALL_CTRL_ID != MMK_GetActiveCtrlId(win_id))
        {
            if((MSG_APP_WEB == msg_id) || (MSG_CTL_MIDSK == msg_id)  ||( (MSG_CTL_PENOK == msg_id) && (MMIWIFI_LABEL_WAPI_UNINSTALL_CTRL_ID != ((MMI_NOTIFY_T*)param)->src_id)))
            {

                if(!MMIAPIUDISK_UdiskIsRun())
                {
                    WifiOpenSelectFileForConnectWin(MMK_GetActiveCtrlId(win_id));
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
                }              
            }
        }
        else
        {
            MMIPUB_OpenQueryWinByTextId(TXT_WLAN_UNINSTALL_QUERY, IMAGE_PUBWIN_QUERY, PNULL, PNULL);;
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_CTL_OK:
    case MSG_APP_OK:
        do
        {
            profile_ptr->config_info.credential.wapi_credential.credential_type = WIFISUPP_WAPI_CREDENTIAL_TYPE_CER;/*lint !e831 !e613*/
            if( 0 == MMIAPICOM_Wstrlen(profile_ptr->client_file_name) || 0 == MMIAPICOM_Wstrlen(profile_ptr->parent_file_name))/*lint !e831 !e613*/
            {
                break;
            }

            //加载证书begin
            file_name = profile_ptr->client_file_name;/*lint !e831 !e613*///客户端证书加载
            file_handle = SFS_CreateFile(file_name, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, PNULL, PNULL);
            if(SFS_ERROR_NONE == SFS_GetFileSize(file_handle,&buf_len1) && buf_len1 > 0 && buf_len1 < MMIWIFI_CERTIFICATE_FILE_SIZE_MAX)
            {
                profile_ptr->config_info.credential.wapi_credential.credential_info.wapi_cer_info.asue_certificate_buf_ptr = (uint8 *)SCI_ALLOC(buf_len1);/*lint !e831 !e613*/
                if (PNULL == profile_ptr->config_info.credential.wapi_credential.credential_info.wapi_cer_info.asue_certificate_buf_ptr)/*lint !e831 !e613*/
                {
                    break;
                }

                if(SFS_ERROR_NONE == SFS_ReadFile(file_handle, (char*)(profile_ptr->config_info.credential.wapi_credential.credential_info.wapi_cer_info.asue_certificate_buf_ptr), buf_len1, (uint32*)(&read_len), PNULL))/*lint !e831 !e613*/
                {
                    if (read_len == buf_len1)
                    {
                        profile_ptr->config_info.credential.wapi_credential.credential_info.wapi_cer_info.asue_certificate_buf_len = read_len;/*lint !e831 !e613*/
                    }
                    SFS_CloseFile(file_handle);
                }
                else
                {
                    SFS_CloseFile(file_handle);
                    break;
                }
            }
            else
            {
                SFS_CloseFile(file_handle);
                break;
            }

            file_name = PNULL;
            file_name = profile_ptr->parent_file_name;/*lint !e831 !e613*///根证书加载
            file_handle = SFS_CreateFile(file_name, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, PNULL, PNULL);
            if(SFS_ERROR_NONE == SFS_GetFileSize(file_handle,&buf_len2) && buf_len2 > 0 && buf_len2 < MMIWIFI_CERTIFICATE_FILE_SIZE_MAX)
            {
                profile_ptr->config_info.credential.wapi_credential.credential_info.wapi_cer_info.as_certificate_buf_ptr = (uint8 *)SCI_ALLOC(buf_len2);/*lint !e831 !e613*/
                if (PNULL == profile_ptr->config_info.credential.wapi_credential.credential_info.wapi_cer_info.as_certificate_buf_ptr)/*lint !e831 !e613*/
                {
                    break;
                }
                if(SFS_ERROR_NONE == SFS_ReadFile(file_handle, (char*)(profile_ptr->config_info.credential.wapi_credential.credential_info.wapi_cer_info.as_certificate_buf_ptr), buf_len2, (uint32*)(&read_len), PNULL))/*lint !e831 !e613*/
                {
                    if (read_len == buf_len2)
                    {
                        profile_ptr->config_info.credential.wapi_credential.credential_info.wapi_cer_info.as_certificate_buf_len = read_len;/*lint !e831 !e613*/
                    }
                    SFS_CloseFile(file_handle);
                }
                else
                {
                    SFS_CloseFile(file_handle);
                    break;
                }
            }
            else
            {
                SFS_CloseFile(file_handle);
                break;
            }

            is_cer_ok = TRUE;

        } while (0);

        //加载证书end
        if (is_cer_ok)
        {
            MMK_CloseWin( win_id );
            MMIWIFI_OpenConnectWaitWin();
            (void)MMIWIFI_DirectConnect(&(profile_ptr->config_info));/*lint !e831 !e613*/
            profile_ptr->state = MMIWIFI_PROFILE_STATE_CONNECTING;/*lint !e831 !e613*/
        }
        else
        {
            MMIPUB_OpenAlertWarningWin(TXT_WIFI_CERTIFICATE_INVALID);
        }

        if (PNULL != profile_ptr->config_info.credential.wapi_credential.credential_info.wapi_cer_info.asue_certificate_buf_ptr && 0 != buf_len1)/*lint !e831 !e613*/
        {
            SCI_FREE(profile_ptr->config_info.credential.wapi_credential.credential_info.wapi_cer_info.asue_certificate_buf_ptr);/*lint !e831 !e613*/
            profile_ptr->config_info.credential.wapi_credential.credential_info.wapi_cer_info.asue_certificate_buf_ptr = PNULL;/*lint !e831 !e613*/
        }

        if (PNULL != profile_ptr->config_info.credential.wapi_credential.credential_info.wapi_cer_info.as_certificate_buf_ptr && 0 != buf_len2)/*lint !e831 !e613*/
        {
            SCI_FREE(profile_ptr->config_info.credential.wapi_credential.credential_info.wapi_cer_info.as_certificate_buf_ptr);/*lint !e831 !e613*/
            profile_ptr->config_info.credential.wapi_credential.credential_info.wapi_cer_info.as_certificate_buf_ptr = PNULL;/*lint !e831 !e613*/
        }
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );      
        break;

    case MSG_NOTIFY_FORM_SWITCH_ACTIVE:
        MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
        break;

    case MSG_PROMPTWIN_OK:
        SCI_MEMSET(profile_ptr->client_file_name, 0, sizeof(profile_ptr->client_file_name));/*lint !e831 !e613*/
        SCI_MEMSET(profile_ptr->parent_file_name, 0, sizeof(profile_ptr->parent_file_name));/*lint !e831 !e613*/
        profile_ptr->is_trusted = FALSE;/*lint !e831 !e613*/
        MMIWIFI_UpdateProfileFile();
        MMK_SetAtvCtrl(win_id, MMIWIFI_EDIT_WAPI_CLIENT_CERTIFICATE_LABEL_CTRL_ID);
        MMIPUB_CloseQuerytWin(PNULL);
#ifdef MMI_GUI_STYLE_TYPICAL
        MMIPUB_OpenAlertWinByTextId(PNULL, TXT_WLAN_UNINSTALL_SUCC, 0, IMAGE_PUBWIN_SUCCESS, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
#endif 
        break;

    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
        break;

    case MSG_CLOSE_WINDOW:
        if (PNULL != profile_ptr)
        {
            profile_ptr->is_editing = FALSE;
        }

        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/****************************************************************************/
//  Description : handle wifi new window
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleNewWinMsg(
                                        MMI_WIN_ID_T        win_id, 
                                        MMI_MESSAGE_ID_E    msg_id, 
                                        DPARAM              param
                                        )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        SCI_MEMSET(&(s_ui_context.s_mmiwifi_profile), 0, sizeof(MMIWIFI_PROFILE_T));
        s_ui_context.s_mmiwifi_profile.is_auto_connect = TRUE;
        WifiSetEditAPWindow();
#ifndef MMI_GUI_STYLE_TYPICAL
        MMK_SetAtvCtrl(win_id, MMIWLAN_LIST_AP_EDIT_CTRL_ID);
#else
        MMK_SetAtvCtrl(win_id,MMIWIFI_EDIT_SSID_DATA_EDIT_CTRL_ID);
        GUIEDIT_SetTextMaxLen(MMIWIFI_EDIT_SSID_DATA_EDIT_CTRL_ID, WIFISUPP_SSID_MAX_LEN / 2 - 1, WIFISUPP_SSID_MAX_LEN);
#endif /* MMI_GUI_STYLE_TYPICAL */
        break;        
        
#ifndef MMI_GUI_STYLE_TYPICAL
    case MSG_FULL_PAINT:
        WifiSetEditAPWindow();
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:
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
                else if (ctrl_id == MMIWLAN_LIST_AP_EDIT_CTRL_ID)
                {
                    uint32              index = 0;
                    SETTING_ITEM_FUNC   item_func = PNULL;

                    index = GUILIST_GetCurItemIndex(MMIWLAN_LIST_AP_EDIT_CTRL_ID);
                    item_func = (SETTING_ITEM_FUNC)WifiGetListUserdata(MMIWLAN_LIST_AP_EDIT_CTRL_ID, index);

                    if (PNULL != item_func)
                    {
                        (*item_func)();
                    }
                }
            }
        }
        break;

#endif /* MMI_GUI_STYLE_TYPICAL */

    case MSG_NOTIFY_DROPDOWNLIST_SELECTED:
        if(MMIWIFI_EDIT_AUTH_TYPE_DROPDOWNLIST_CTRL_ID == MMK_GetActiveCtrlId(win_id))
        {
            if(WIFISUPP_ENCRYP_PROTOCOL_OPENSYS == GUIDROPDOWNLIST_GetCurItemIndex(MMIWIFI_EDIT_AUTH_TYPE_DROPDOWNLIST_CTRL_ID))
            {
#ifdef WLAN_EAP_SUPPORT
                GUIFORM_SetChildDisplay(MMIWIFI_EDIT_AP_FORMCTRL_ID, 
                    MMIWIFI_CHILD_FORM_EAP_SIM_INDEX_CTRL_ID,
                    GUIFORM_CHILD_DISP_HIDE);
#endif /* WLAN_EAP_SUPPORT */
                GUIFORM_SetChildDisplay(MMIWIFI_EDIT_AP_FORMCTRL_ID, 
                    MMIWIFI_CHILD_FORM_AUTH_SETTING_CTRL_ID,
                    GUIFORM_CHILD_DISP_GRAY);
            }
#ifdef WLAN_EAP_SUPPORT
            else if (MMIWIFI_MENU_AP_EAP_SIM_INDEX_CTRL_ID == MMK_GetActiveCtrlId(win_id))
            {
                s_ui_context.s_mmiwifi_profile.is_auth_input = FALSE;

                GUIFORM_SetChildDisplay(MMIWIFI_EDIT_AP_FORMCTRL_ID, 
                    MMIWIFI_CHILD_FORM_EAP_SIM_INDEX_CTRL_ID,
                    GUIFORM_CHILD_DISP_NORMAL);

                GUIFORM_SetChildDisplay(MMIWIFI_EDIT_AP_FORMCTRL_ID, 
                    MMIWIFI_CHILD_FORM_AUTH_SETTING_CTRL_ID,
                    GUIFORM_CHILD_DISP_HIDE);
            }
#endif /* WLAN_EAP_SUPPORT */
            else
            {
                s_ui_context.s_mmiwifi_profile.is_auth_input = FALSE;
#ifdef WLAN_EAP_SUPPORT
                GUIFORM_SetChildDisplay(MMIWIFI_EDIT_AP_FORMCTRL_ID, 
                    MMIWIFI_CHILD_FORM_EAP_SIM_INDEX_CTRL_ID,
                    GUIFORM_CHILD_DISP_HIDE);
#endif /* WLAN_EAP_SUPPORT */
                GUIFORM_SetChildDisplay(MMIWIFI_EDIT_AP_FORMCTRL_ID, 
                    MMIWIFI_CHILD_FORM_AUTH_SETTING_CTRL_ID,
                    GUIFORM_CHILD_DISP_NORMAL);
            }
            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
        }
        break;
       
#ifdef MMI_GUI_STYLE_TYPICAL
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        if(MMIWIFI_EDIT_AUTH_SETTING_LABEL_CTRL_ID == MMK_GetActiveCtrlId(win_id))
        {
            //打开安全设置窗口
            uint16 cur_sel = 0;

            cur_sel = GUIDROPDOWNLIST_GetCurItemIndex(MMIWIFI_EDIT_AUTH_TYPE_DROPDOWNLIST_CTRL_ID);
            switch(cur_sel)
            {
            case WIFISUPP_ENCRYP_PROTOCOL_WEP:
                MMK_CreateWin((uint32 *)MMIWIFI_AUTH_SETTING_WEP_WIN_TAB, PNULL);
                break;

            case WIFISUPP_ENCRYP_PROTOCOL_WPA:
            case WIFISUPP_ENCRYP_PROTOCOL_WPA2:
                MMK_CreateWin((uint32 *)MMIWIFI_AUTH_SETTING_WPA_WPA2_WIN_TAB, PNULL);
                break;

            case WIFISUPP_ENCRYP_PROTOCOL_WAPI:
                MMK_CreateWin((uint32 *)MMIWIFI_AUTH_SETTING_WAPI_WIN_TAB, PNULL);
                break;

            default:
                break;
            }
        }
        break;
#endif /* MMI_GUI_STYLE_TYPICAL */

    case MSG_CTL_OK:
    case MSG_APP_OK:
        {
#ifdef MMI_GUI_STYLE_TYPICAL
            MMI_STRING_T str_ptr = {0};
            uint16              cur_index = 0;
#endif
            MMIWIFI_PROFILE_T *find_profile_ptr = PNULL;
    
#ifdef MMI_GUI_STYLE_TYPICAL
            GUIEDIT_GetString(MMIWIFI_EDIT_SSID_DATA_EDIT_CTRL_ID, &str_ptr);
#endif /* MMI_GUI_STYLE_TYPICAL */

#ifndef MMI_GUI_STYLE_TYPICAL
            if (0 == WifiGetSsidLen())
#else
            if (0 == str_ptr.wstr_len)
#endif /* MMI_GUI_STYLE_TYPICAL */
            {
                MMIPUB_OpenAlertWarningWin(TXT_WIFI_SSID_INVALID);
            }
            else
            {
                WIFISUPP_ENCRYP_PROTOCOL_E auth_type = WIFISUPP_ENCRYP_PROTOCOL_OPENSYS;

#ifndef MMI_GUI_STYLE_TYPICAL
                auth_type = WifiGetEncryptProtocol();
#else
                auth_type = (WIFISUPP_ENCRYP_PROTOCOL_E)GUIDROPDOWNLIST_GetCurItemIndex(MMIWIFI_EDIT_AUTH_TYPE_DROPDOWNLIST_CTRL_ID);
#endif // MMI_GUI_STYLE_TYPICAL

                if (WIFISUPP_ENCRYP_PROTOCOL_OPENSYS != auth_type &&
#ifdef WLAN_EAP_SUPPORT
                    WIFISUPP_ENCRYP_PROTOCOL_EAP != auth_type &&
#endif /* WLAN_EAP_SUPPORT */
                    !s_ui_context.s_mmiwifi_profile.is_auth_input)
                {
                    MMIPUB_OpenAlertWarningWin(TXT_WLAN_AUTH_SETTING_EMPTY);
                    break;
                }

#ifdef MMI_GUI_STYLE_TYPICAL
                //保存网络名称SSID和长度
                SCI_MEMSET(s_ui_context.s_mmiwifi_profile.config_info.ssid.ssid, 0, sizeof(s_ui_context.s_mmiwifi_profile.config_info.ssid.ssid));
                GUI_WstrToGB(s_ui_context.s_mmiwifi_profile.config_info.ssid.ssid, str_ptr.wstr_ptr, str_ptr.wstr_len);
                s_ui_context.s_mmiwifi_profile.config_info.ssid.ssid_len = strlen((char*)s_ui_context.s_mmiwifi_profile.config_info.ssid.ssid);
                
                //网络状态
                cur_index = GUIDROPDOWNLIST_GetCurItemIndex(MMIWIFI_EDIT_NETWORK_STATE_DROPDOWNLIST_CTRL_ID);//(MMIWIFI_EDIT_NETWORK_STATE_DROPDOWNLIST_CTRL_ID, index);
                s_ui_context.s_mmiwifi_profile.is_ssid_hide = (BOOLEAN)cur_index;
                
                //WLAN网络模式,基础网络，移动规范要求，不可更改
                s_ui_context.s_mmiwifi_profile.config_info.network_mode = WIFISUPP_NETWORK_MODE_INFRASTRUCTURE;
                
                //WLAN安全模式
                cur_index = GUIDROPDOWNLIST_GetCurItemIndex(MMIWIFI_EDIT_AUTH_TYPE_DROPDOWNLIST_CTRL_ID);
                s_ui_context.s_mmiwifi_profile.config_info.encryp_protocol = (WIFISUPP_ENCRYP_PROTOCOL_E)cur_index;
                
#ifdef WLAN_EAP_SUPPORT
                s_ui_context.s_mmiwifi_profile.config_info.credential.wpa_credential.credential_info.wpa_eap_info.eap_info.eapsim_info.simcard_num = GUIDROPDOWNLIST_GetCurItemIndex(MMIWIFI_MENU_AP_EAP_SIM_INDEX_CTRL_ID);
#endif /* WLAN_EAP_SUPPORT */

                //WLAN连接状态
                cur_index = GUIDROPDOWNLIST_GetCurItemIndex(MMIWIFI_EDIT_CONNECT_STATE_DROPDOWNLIST_CTRL_ID);
                s_ui_context.s_mmiwifi_profile.is_auto_connect= cur_index;
#endif /* MMI_GUI_STYLE_TYPICAL */
                s_ui_context.s_mmiwifi_profile.rssi = MMIWIFI_RSSI_0;

                s_ui_context.s_mmiwifi_profile.is_trusted = TRUE;//新建接入点，在信任列表中，将对应标志置位
                s_ui_context.s_mmiwifi_profile.state = MMIWIFI_PROFILE_STATE_DISCONNECTED;

                find_profile_ptr = MMIWIFI_FindProfileByAP(&(s_ui_context.s_mmiwifi_profile));
                if (PNULL != find_profile_ptr)
                {
                     MMIPUB_OpenAlertWarningWin(TXT_WIFI_PROFILE_EXISTED);
                }
                else
                {
                    if(MMIWIFI_AddProfileToList(&(s_ui_context.s_mmiwifi_profile))) 
                    {
                        MMIWIFI_UpdateProfileFile();
#ifdef MMI_GUI_STYLE_TYPICAL
                        MMIPUB_OpenAlertWinByTextId(PNULL, TXT_COMPLETE, 0, IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
#endif                   
                        MMK_CloseWin(win_id);
                    }
                    else
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_MMI_SAVEFAILED);
                    }
                }

            }
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );      
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/****************************************************************************/
//  Description : handle wifi edit window
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleEditWinMsg(
                                         MMI_WIN_ID_T        win_id, 
                                         MMI_MESSAGE_ID_E    msg_id, 
                                         DPARAM              param
                                         )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            MMIWIFI_PROFILE_T*  profile_ptr = (MMIWIFI_PROFILE_T*)MMK_GetWinAddDataPtr(win_id);

            profile_ptr->is_editing = TRUE;
            SCI_MEMCPY(&(s_ui_context.s_mmiwifi_profile), profile_ptr, sizeof(MMIWIFI_PROFILE_T));
            s_ui_context.s_mmiwifi_profile.is_auth_input = TRUE;
            WifiSetEditAPWindow();
#ifndef MMI_GUI_STYLE_TYPICAL
            MMK_SetAtvCtrl(win_id, MMIWLAN_LIST_AP_EDIT_CTRL_ID);
#else
            MMK_SetAtvCtrl(win_id,MMIWIFI_EDIT_SSID_DATA_EDIT_CTRL_ID);
            GUIEDIT_SetTextMaxLen(MMIWIFI_EDIT_SSID_DATA_EDIT_CTRL_ID, WIFISUPP_SSID_MAX_LEN / 2 - 1, WIFISUPP_SSID_MAX_LEN);
#endif /* MMI_GUI_STYLE_TYPICAL */
        }
        break;        
        
#ifndef MMI_GUI_STYLE_TYPICAL
    case MSG_FULL_PAINT:
        WifiSetEditAPWindow();
        break;

    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
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
                else if (ctrl_id == MMIWLAN_LIST_AP_EDIT_CTRL_ID)
                {
                    uint32              index = 0;
                    SETTING_ITEM_FUNC   item_func = PNULL;

                    index = GUILIST_GetCurItemIndex(MMIWLAN_LIST_AP_EDIT_CTRL_ID);
                    item_func = (SETTING_ITEM_FUNC)WifiGetListUserdata(MMIWLAN_LIST_AP_EDIT_CTRL_ID, index);

                    if (PNULL != item_func)
                    {
                        (*item_func)();
                    }
                }
            }
        }
        break;

#endif /* MMI_GUI_STYLE_TYPICAL */

    case MSG_NOTIFY_DROPDOWNLIST_SELECTED:
        if(MMIWIFI_EDIT_AUTH_TYPE_DROPDOWNLIST_CTRL_ID == MMK_GetActiveCtrlId(win_id))
        {
            if(WIFISUPP_ENCRYP_PROTOCOL_OPENSYS == GUIDROPDOWNLIST_GetCurItemIndex(MMIWIFI_EDIT_AUTH_TYPE_DROPDOWNLIST_CTRL_ID))
            {
#ifdef WLAN_EAP_SUPPORT
                GUIFORM_SetChildDisplay(MMIWIFI_EDIT_AP_FORMCTRL_ID, 
                    MMIWIFI_CHILD_FORM_EAP_SIM_INDEX_CTRL_ID,
                    GUIFORM_CHILD_DISP_HIDE);
#endif /* WLAN_EAP_SUPPORT */
                GUIFORM_SetChildDisplay(MMIWIFI_EDIT_AP_FORMCTRL_ID, 
                    MMIWIFI_CHILD_FORM_AUTH_SETTING_CTRL_ID,
                    GUIFORM_CHILD_DISP_GRAY);
            }
#ifdef WLAN_EAP_SUPPORT
            else if (MMIWIFI_MENU_AP_EAP_SIM_INDEX_CTRL_ID == MMK_GetActiveCtrlId(win_id))
            {
                s_ui_context.s_mmiwifi_profile.is_auth_input = FALSE;

                GUIFORM_SetChildDisplay(MMIWIFI_EDIT_AP_FORMCTRL_ID, 
                    MMIWIFI_CHILD_FORM_EAP_SIM_INDEX_CTRL_ID,
                    GUIFORM_CHILD_DISP_NORMAL);

                GUIFORM_SetChildDisplay(MMIWIFI_EDIT_AP_FORMCTRL_ID, 
                    MMIWIFI_CHILD_FORM_AUTH_SETTING_CTRL_ID,
                    GUIFORM_CHILD_DISP_HIDE);
            }
#endif /* WLAN_EAP_SUPPORT */
            else
            {
                s_ui_context.s_mmiwifi_profile.is_auth_input = FALSE;
#ifdef WLAN_EAP_SUPPORT
                GUIFORM_SetChildDisplay(MMIWIFI_EDIT_AP_FORMCTRL_ID, 
                    MMIWIFI_CHILD_FORM_EAP_SIM_INDEX_CTRL_ID,
                    GUIFORM_CHILD_DISP_HIDE);
#endif /* WLAN_EAP_SUPPORT */
                GUIFORM_SetChildDisplay(MMIWIFI_EDIT_AP_FORMCTRL_ID, 
                    MMIWIFI_CHILD_FORM_AUTH_SETTING_CTRL_ID,
                    GUIFORM_CHILD_DISP_NORMAL);
            }
            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
        }
        break;

#ifdef MMI_GUI_STYLE_TYPICAL
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        if(MMIWIFI_EDIT_AUTH_SETTING_LABEL_CTRL_ID == MMK_GetActiveCtrlId(win_id))
        {
            //打开安全设置窗口
            uint16 cur_sel = 0;
           
            cur_sel = GUIDROPDOWNLIST_GetCurItemIndex(MMIWIFI_EDIT_AUTH_TYPE_DROPDOWNLIST_CTRL_ID);
            if(cur_sel != WifiGetEncryptProtocol())
            {
                WifiResetCredential();
            }
            switch(cur_sel)
            {
            case WIFISUPP_ENCRYP_PROTOCOL_WEP:
                MMK_CreateWin((uint32 *)MMIWIFI_AUTH_SETTING_WEP_WIN_TAB, PNULL);
                break;

            case WIFISUPP_ENCRYP_PROTOCOL_WPA:
            case WIFISUPP_ENCRYP_PROTOCOL_WPA2:
                MMK_CreateWin((uint32 *)MMIWIFI_AUTH_SETTING_WPA_WPA2_WIN_TAB, PNULL);
                break;

            case WIFISUPP_ENCRYP_PROTOCOL_WAPI:
                MMK_CreateWin((uint32 *)MMIWIFI_AUTH_SETTING_WAPI_WIN_TAB, PNULL);
                break;

            default:
                break;
            }
        }
        break;
#endif /* MMI_GUI_STYLE_TYPICAL */

    case MSG_APP_OK:
    case MSG_CTL_OK:
        {
#ifdef MMI_GUI_STYLE_TYPICAL
            MMI_STRING_T str_ptr = {0};
#endif
            uint16              cur_index = 0;
            uint32              num = 0;
            MMIWIFI_PROFILE_T   *find_profile_ptr = PNULL;
            MMIWIFI_PROFILE_T *profile_ptr = (MMIWIFI_PROFILE_T *)MMK_GetWinAddDataPtr(win_id);
            MMI_CTRL_ID_T       ctrl_id = 0;
            
#ifdef MMI_GUI_STYLE_TYPICAL
            GUIEDIT_GetString(MMIWIFI_EDIT_SSID_DATA_EDIT_CTRL_ID, &str_ptr);
#endif /* MMI_GUI_STYLE_TYPICAL */

            if (PNULL == profile_ptr)
            {
                MMK_CloseWin(win_id);
                break;
            }

#ifndef MMI_GUI_STYLE_TYPICAL
            if (0 == WifiGetSsidLen())
#else
            if (0 == str_ptr.wstr_len)
#endif /* MMI_GUI_STYLE_TYPICAL */
            {
                MMIPUB_OpenAlertWarningWin(TXT_WIFI_SSID_INVALID);
            }
            else
            {
                WIFISUPP_ENCRYP_PROTOCOL_E auth_type = WIFISUPP_ENCRYP_PROTOCOL_OPENSYS;

#ifndef MMI_GUI_STYLE_TYPICAL
                auth_type = WifiGetEncryptProtocol();
#else
                auth_type = (WIFISUPP_ENCRYP_PROTOCOL_E)GUIDROPDOWNLIST_GetCurItemIndex(MMIWIFI_EDIT_AUTH_TYPE_DROPDOWNLIST_CTRL_ID);
#endif // MMI_GUI_STYLE_TYPICAL

                if (WIFISUPP_ENCRYP_PROTOCOL_OPENSYS != auth_type &&
#ifdef WLAN_EAP_SUPPORT
                    WIFISUPP_ENCRYP_PROTOCOL_EAP != auth_type &&
#endif /* WLAN_EAP_SUPPORT */
                    !s_ui_context.s_mmiwifi_profile.is_auth_input)
                {
                    MMIPUB_OpenAlertWarningWin(TXT_WLAN_AUTH_SETTING_EMPTY);
                    break;
                }
#ifdef MMI_GUI_STYLE_TYPICAL
                //保存网络名称SSID和长度
                SCI_MEMSET(s_ui_context.s_mmiwifi_profile.config_info.ssid.ssid, 0, sizeof(s_ui_context.s_mmiwifi_profile.config_info.ssid.ssid));
                GUI_WstrToGB(s_ui_context.s_mmiwifi_profile.config_info.ssid.ssid, str_ptr.wstr_ptr, str_ptr.wstr_len);
                s_ui_context.s_mmiwifi_profile.config_info.ssid.ssid_len = strlen((char*)s_ui_context.s_mmiwifi_profile.config_info.ssid.ssid);
                
                //网络状态
                cur_index = GUIDROPDOWNLIST_GetCurItemIndex(MMIWIFI_EDIT_NETWORK_STATE_DROPDOWNLIST_CTRL_ID);//(MMIWIFI_EDIT_NETWORK_STATE_DROPDOWNLIST_CTRL_ID, index);
                s_ui_context.s_mmiwifi_profile.is_ssid_hide = (BOOLEAN)cur_index;
                
                //WLAN网络模式,基础网络，移动规范要求，不可更改
                s_ui_context.s_mmiwifi_profile.config_info.network_mode = WIFISUPP_NETWORK_MODE_INFRASTRUCTURE;
                
                //WLAN安全模式
                cur_index = GUIDROPDOWNLIST_GetCurItemIndex(MMIWIFI_EDIT_AUTH_TYPE_DROPDOWNLIST_CTRL_ID);
                s_ui_context.s_mmiwifi_profile.config_info.encryp_protocol = (WIFISUPP_ENCRYP_PROTOCOL_E)cur_index;
                
#ifdef WLAN_EAP_SUPPORT
                s_ui_context.s_mmiwifi_profile.config_info.credential.wpa_credential.credential_info.wpa_eap_info.eap_info.eapsim_info.simcard_num = GUIDROPDOWNLIST_GetCurItemIndex(MMIWIFI_MENU_AP_EAP_SIM_INDEX_CTRL_ID);
#endif /* WLAN_EAP_SUPPORT */

                //WLAN连接状态
                cur_index = GUIDROPDOWNLIST_GetCurItemIndex(MMIWIFI_EDIT_CONNECT_STATE_DROPDOWNLIST_CTRL_ID);
                s_ui_context.s_mmiwifi_profile.is_auto_connect= cur_index;
#endif /* MMI_GUI_STYLE_TYPICAL */
                
                s_ui_context.s_mmiwifi_profile.rssi = MMIWIFI_RSSI_0;

                num = MMIWIFI_GetProfileNum();
                while(num != 0)
                {
                    find_profile_ptr = MMIWIFI_GetProfileByIndex(num - 1);
                    
                    if (MMIWIFI_APIsSame(find_profile_ptr, &(s_ui_context.s_mmiwifi_profile)))
                    {
#ifdef MMI_WLAN_TAB_STYLE
                        ctrl_id = MMIWIFI_LIST_TRUSTED_CTRL_ID;
#else
                        ctrl_id = MMIWIFI_LIST_CTRL_ID;
#endif /* MMI_WLAN_TAB_STYLE */
                        cur_index = GUILIST_GetCurItemIndex(ctrl_id);
                        if (find_profile_ptr  != (MMIWIFI_PROFILE_T *)WifiGetListUserdata(ctrl_id, cur_index)) /* Not itself */
                        {
                            MMIPUB_OpenAlertWarningWin(TXT_WIFI_PROFILE_EXISTED);
                            return recode;
                        }
                    }
                    num--;
                }

                if(MMIWIFI_UpdateProfileByAP(profile_ptr, &s_ui_context.s_mmiwifi_profile))
                {
                    MMIWIFI_UpdateProfileFile();
#ifdef MMI_GUI_STYLE_TYPICAL
                    MMIPUB_OpenAlertWinByTextId(PNULL, TXT_COMPLETE, 0,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
#endif
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_MMI_SAVEFAILED);
                }
                MMK_CloseWin(win_id);
            }
        }
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );      
        break;

    case MSG_CLOSE_WINDOW:
            {
                MMIWIFI_PROFILE_T*  profile_ptr = (MMIWIFI_PROFILE_T*)MMK_GetWinAddDataPtr(win_id);

                if (PNULL != profile_ptr)
                {
                    profile_ptr->is_editing = FALSE;
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
//  Description : handle wifi wep 128bit data window
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleWep128BitDataWinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM              param
                                            )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_STRING_T    string_info = {0};
    MMI_CTRL_ID_T   ctrl_id = MMIWIFI_WEP_128BIT_DATA_CTRL_ID;
    uint16          temp_buf[MMIWIFI_WEP_128BIT_KEY_LEN + 1] = {0};
    uint8           temp_buf_char[MMIWIFI_WEP_128BIT_KEY_LEN + 1] = {0};
    uint8           temp_hex_char[MMIWIFI_WEP_128BIT_KEY_LEN * 2 + 1] = {0};
    uint16          temp_hex[MMIWIFI_WEP_128BIT_KEY_LEN*2 + 1] = {0};
    uint16          index = 0;
    uint16          style_index = 0;
    uint16          wep_index = 0;
    uint8*          wep_ptr = PNULL;
    uint32          len = 0;
    uint32          i = 0;
    uint32          curr_len = 0;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIEDIT_SetIm(ctrl_id, GUIIM_TYPE_ABC | GUIIM_TYPE_ENGLISH | GUIIM_TYPE_DIGITAL, GUIIM_TYPE_ABC); /*lint !e655*/
        index = GUIDROPDOWNLIST_GetCurItemIndex(MMIWIFI_WEPKEY_INDEX_DROPDOWNLIST_CTRL_ID);
        WifiSetWepCurrIndex(index);

        style_index = GUIDROPDOWNLIST_GetCurItemIndex(MMIWIFI_WEPKEY_STYLE_DROPDOWNLIST_CTRL_ID);
        if (0 == style_index)
        {
            WifiSetIsWepAscii(index,TRUE);
            GUIEDIT_SetMaxLen(ctrl_id,MMIWIFI_WEP_128BIT_KEY_LEN);
        }
        else
        {
            WifiSetIsWepAscii(index,FALSE);
            GUIEDIT_SetMaxLen(ctrl_id,MMIWIFI_WEP_128BIT_KEY_LEN*2);
        }
        wep_ptr = WifiGetWep128Bit(index);
        if (PNULL != wep_ptr)
        {
            len = strlen((char*)wep_ptr);
            if (0 != len)
            {
                if (!WifiGetIsWepAscii(index))
                {
                    for(i=0; i<len; i++)
                    {
                        curr_len = strlen((char*)temp_hex_char);
                        CFL_Snprintf((char*)temp_hex_char + curr_len ,MMIWIFI_WEP_128BIT_KEY_LEN * 2 + 1 - curr_len,"%02x",wep_ptr[i]);
                    }
                    len = strlen((char*)temp_hex_char);
                    MMI_STRNTOWSTR(temp_hex, MMIWIFI_WEP_128BIT_KEY_LEN * 2, temp_hex_char,len, MIN(len,MMIWIFI_WEP_128BIT_KEY_LEN * 2));
                    GUIEDIT_SetString(ctrl_id,temp_hex,len);
                }
                else
                {
                    MMI_STRNTOWSTR(temp_buf, MMIWIFI_WEP_128BIT_KEY_LEN, wep_ptr,len, MIN(len,MMIWIFI_WEP_128BIT_KEY_LEN));
                    GUIEDIT_SetString(ctrl_id,temp_buf,len);
                }
                
            }
        }
        
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
        GUIEDIT_GetString(ctrl_id,&string_info);
        if (PNULL == string_info.wstr_ptr || 0 == string_info.wstr_len)
        {
            MMIPUB_OpenAlertWarningWin(TXT_WIFI_KEY_EMPTY);
        }
        else
        {
            wep_index = WifiGetWepCurrIndex();
            if (!WifiGetIsWepAscii(wep_index))
            {
                MMI_WSTRNTOSTR(temp_hex_char, MMIWIFI_WEP_128BIT_KEY_LEN*2, string_info.wstr_ptr, string_info.wstr_len, string_info.wstr_len);
                if (!WifiCheckIsHex(temp_hex_char,string_info.wstr_len))
                {
                    MMIPUB_OpenAlertWarningWin(TXT_WIFI_KEY_HEX_INPUT);
                    break;
                }
                if (MMIWIFI_WEP_128BIT_KEY_LEN*2 != string_info.wstr_len)
                {
                    MMIPUB_OpenAlertWarningWin(TXT_WIFI_KEY_INPUT_26);
                    break;
                }
                WifiHexStrToBin((const char*)temp_hex_char,temp_buf_char,MMIWIFI_WEP_128BIT_KEY_LEN);
            }
            else
            {
                MMI_WSTRNTOSTR(temp_buf_char, MMIWIFI_WEP_128BIT_KEY_LEN, string_info.wstr_ptr, string_info.wstr_len, string_info.wstr_len);
                if (MMIWIFI_WEP_128BIT_KEY_LEN != string_info.wstr_len)
                {
                    MMIPUB_OpenAlertWarningWin(TXT_WIFI_KEY_INPUT_13);
                    break;
                }
            }
            
            WifiSetWep128Bit(wep_index, temp_buf_char);
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
//  Description : set wep bit and mode window
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetWepBitAndModeWindow(void)
{
    uint32                      i = 0;
    MMI_STRING_T                item_bit[MMIWIFI_WEP_BIT_NUM] = {0};
    MMI_STRING_T                item_mode[MMIWIFI_WEP_KEY_MODE_NUM] = {0};
    MMI_TEXT_ID_T               wep_bit[MMIWIFI_WEP_BIT_NUM] = {TXT_WIFI_WEP_BIT64,TXT_WIFI_WEP_BIT128};
    MMI_TEXT_ID_T               wep_mode[MMIWIFI_WEP_KEY_MODE_NUM] = {TXT_WIFI_WEP_KEY_SHARE_MODE,TXT_WIFI_WEP_KEY_OPEN_MODE};
    
    GUILABEL_SetTextById(MMIWIFI_WEPKEY_BIT_LABEL_CTRL_ID,TXT_TYPE,TRUE);
    GUILABEL_SetTextById(MMIWIFI_WEPKEY_MODE_LABEL_CTRL_ID,TXT_DETAIL_MODE,TRUE);
    
    for (i = 0; i < MMIWIFI_WEP_BIT_NUM; i++)
    {
        MMI_GetLabelTextByLang(wep_bit[i], &item_bit[i]);
    }
    
    
    for (i= 0; i < MMIWIFI_WEP_KEY_MODE_NUM; i++)
    {
        MMI_GetLabelTextByLang(wep_mode[i], &item_mode[i]);
    }

    GUIDROPDOWNLIST_AppendItemArray(MMIWIFI_WEPKEY_BIT_DROPDOWNLIST_CTRL_ID, item_bit, MMIWIFI_WEP_BIT_NUM );
                 
    if (WIFISUPP_WEP_KEY_TYPE_64BITS == WifiGetWepKeyType())
    {
        GUIDROPDOWNLIST_SetCurItemIndex(MMIWIFI_WEPKEY_BIT_DROPDOWNLIST_CTRL_ID, 0 );
    }
    else
    {
        GUIDROPDOWNLIST_SetCurItemIndex(MMIWIFI_WEPKEY_BIT_DROPDOWNLIST_CTRL_ID, 1 );
    }
    
    //bruce temp
    //GUIDROPDOWNLIST_SetDisplayDirection(MMIWIFI_WEPKEY_BIT_DROPDOWNLIST_CTRL_ID, TRUE );

    GUIDROPDOWNLIST_AppendItemArray(MMIWIFI_WEPKEY_MODE_DROPDOWNLIST_CTRL_ID, item_mode, MMIWIFI_WEP_KEY_MODE_NUM );
                 
    if (!WifiGetIsWepOpen())
    {
        GUIDROPDOWNLIST_SetCurItemIndex(MMIWIFI_WEPKEY_MODE_DROPDOWNLIST_CTRL_ID, 0 );
    }
    else
    {
        GUIDROPDOWNLIST_SetCurItemIndex(MMIWIFI_WEPKEY_MODE_DROPDOWNLIST_CTRL_ID, 1 );
    }
    
    //bruce temp
    //GUIDROPDOWNLIST_SetDisplayDirection(MMIWIFI_WEPKEY_MODE_DROPDOWNLIST_CTRL_ID, TRUE );
}

LOCAL BOOLEAN WifiGetIsWepOpen(void)
{
#ifdef MMI_WLAN_AUTHORITY_COMPLEX
    return s_ui_context.s_mmiwifi_profile.config_info.credential.wep_credential.is_open_mode;
#else
    return FALSE;
#endif /* MMI_WLAN_AUTHORITY_COMPLEX */
}

LOCAL void WifiSetWepOpen(BOOLEAN is_open_mode)
{
#ifndef MMI_WLAN_AUTHORITY_COMPLEX
    is_open_mode = FALSE;
#endif /* MMI_WLAN_AUTHORITY_COMPLEX */
    s_ui_context.s_mmiwifi_profile.config_info.credential.wep_credential.is_open_mode = is_open_mode;
}

/*****************************************************************************/
//  Description : open select path win
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void WifiOpenSelectFileWin(MMI_CTRL_ID_T ctrl_id)
{
    FILEARRAY                   ret_array    = MMIAPIFILEARRAY_Create();
    MMIFMM_FILTER_T             filter       = {0};

    //set list enable mark
    SCI_MEMCPY(filter.filter_str, "*", sizeof("*"));

    if(MMIWIFI_EDIT_WAPI_CLIENT_CERTIFICATE_LABEL_CTRL_ID == ctrl_id)
    {
        if(FMM_OPENWIN_SUCCESS != MMIAPIFMM_OpenFileWinByLayer(PNULL,
            0,
            &filter, FUNC_FIND_FILE_AND_ALLFOLER,
            FALSE,
            PNULL,
            ret_array,
            OpenSelectClientCertificateFileWinCallBack,
            FALSE))
        {
            MMIAPIFILEARRAY_Destroy(&ret_array);
        }
    }
    else if(MMIWIFI_EDIT_WAPI_PARENT_CERTIFICATE_LBAEL_CTRL_ID == ctrl_id)
    {
        if(FMM_OPENWIN_SUCCESS != MMIAPIFMM_OpenFileWinByLayer(PNULL,
            0,
            &filter, FUNC_FIND_FILE_AND_ALLFOLER,
            FALSE,
            PNULL,
            ret_array,
            OpenSelectParentCertificateFileWinCallBack,
            FALSE))
        {
            MMIAPIFILEARRAY_Destroy(&ret_array);
        }
    }
   
}

/*****************************************************************************/
//  Description : open select path win for connect
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void WifiOpenSelectFileForConnectWin(MMI_CTRL_ID_T ctrl_id)
{
    FILEARRAY                   ret_array    = MMIAPIFILEARRAY_Create();
    MMIFMM_FILTER_T             filter       = {0};

    //set list enable mark
    SCI_MEMCPY(filter.filter_str, "*", sizeof("*"));

    if(MMIWIFI_EDIT_WAPI_CLIENT_CERTIFICATE_LABEL_CTRL_ID == ctrl_id)
    {
        if(FMM_OPENWIN_SUCCESS != MMIAPIFMM_OpenFileWinByLayer(PNULL,
            0,
            &filter, FUNC_FIND_FILE_AND_ALLFOLER,
            FALSE,
            PNULL,
            ret_array,
            OpenSelectClientCertificateFileForConnectWinCallBack,
            FALSE))
        {
            MMIAPIFILEARRAY_Destroy(&ret_array);
        }
    }
    else if(MMIWIFI_EDIT_WAPI_PARENT_CERTIFICATE_LBAEL_CTRL_ID == ctrl_id)
    {
        if(FMM_OPENWIN_SUCCESS != MMIAPIFMM_OpenFileWinByLayer(PNULL,
            0,
            &filter, FUNC_FIND_FILE_AND_ALLFOLER,
            FALSE,
            PNULL,
            ret_array,
            OpenSelectParentCertificateFileForconnectWinCallBack,
            FALSE))
        {
            MMIAPIFILEARRAY_Destroy(&ret_array);
        }
    }
   
}

/*****************************************************************************/
//  Description : open select file win callback
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void OpenSelectClientCertificateFileWinCallBack(
                                      BOOLEAN   is_success, 
                                      FILEARRAY file_array
                                      )
{
    if (is_success)
    {
        if (0 < MMIAPIFILEARRAY_GetArraySize(file_array))
        {
            
            FILEARRAY_DATA_T          file_array_data = {0};
            
            //read user selected dir
            MMIAPIFILEARRAY_Read( file_array, 0, &file_array_data );
            WifiSetWapiCertiClientFilePath(file_array_data.filename, file_array_data.name_len);
        }
        else
        {
            //SCI_TRACE_LOW:"OpenSelectClientCertificateFileWinCallBack array size == 0"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_7786_112_2_18_3_10_8_201,(uint8*)"");
        }
    }
    else
    {
        //SCI_TRACE_LOW:"OpenSelectClientCertificateFileWinCallBack failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_7791_112_2_18_3_10_8_202,(uint8*)"");
    }

    MMIAPIFILEARRAY_Destroy(&file_array);
    file_array = PNULL;
}

/*****************************************************************************/
//  Description : open select file win callback for connect
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void OpenSelectClientCertificateFileForConnectWinCallBack(
                                      BOOLEAN   is_success, 
                                      FILEARRAY file_array
                                      )
{
    MMIWIFI_PROFILE_T *profile_ptr = MMIWIFI_GetCurrProfile();

    if (is_success)
    {
        if (0 < MMIAPIFILEARRAY_GetArraySize(file_array))
        {
            
            FILEARRAY_DATA_T          file_array_data = {0};
            
            //read user selected dir
            MMIAPIFILEARRAY_Read( file_array, 0, &file_array_data );
            if (0 != file_array_data.name_len && PNULL != profile_ptr)
            {
                SCI_MEMSET(profile_ptr->client_file_name, 0 ,sizeof(s_ui_context.s_mmiwifi_profile.client_file_name));

                SCI_TRACE_LOW("OpenSelectClientCertificateFileForConnectWinCallBack file_name_len = %d", file_array_data.name_len);

                file_array_data.name_len = MIN(file_array_data.name_len, MMIWIFI_MAX_FILE_NAME_LEN);

                MMIAPICOM_Wstrncpy(profile_ptr->client_file_name, file_array_data.filename, file_array_data.name_len);
            }
        }
        else
        {
            //SCI_TRACE_LOW:"OpenSelectClientCertificateFileForConnectWinCallBack array size == 0"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_7826_112_2_18_3_10_8_203,(uint8*)"");
        }
    }
    else
    {
        //SCI_TRACE_LOW:"OpenSelectClientCertificateFileForConnectWinCallBack failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_7831_112_2_18_3_10_8_204,(uint8*)"");
    }

    MMIAPIFILEARRAY_Destroy(&file_array);
    file_array = PNULL;
}

/*****************************************************************************/
//  Description : open select file win callback
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void OpenSelectParentCertificateFileWinCallBack(
                                      BOOLEAN   is_success, 
                                      FILEARRAY file_array
                                      )
{
    if (is_success)
    {
        if (0 < MMIAPIFILEARRAY_GetArraySize(file_array))
        {
            
            FILEARRAY_DATA_T          file_array_data = {0};
            
            //read user selected dir
            MMIAPIFILEARRAY_Read( file_array, 0, &file_array_data );
            WifiSetWapiCertiParentFilePath(file_array_data.filename, file_array_data.name_len);
        }
        else
        {
            //SCI_TRACE_LOW:"OpenSelectParentCertificateFileWinCallBack array size == 0"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_7860_112_2_18_3_10_8_205,(uint8*)"");
        }
    }
    else
    {
        //SCI_TRACE_LOW:"OpenSelectParentCertificateFileWinCallBack failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_7865_112_2_18_3_10_8_206,(uint8*)"");
    }

    MMIAPIFILEARRAY_Destroy(&file_array);
    file_array = PNULL;
}

/*****************************************************************************/
//  Description : open select file win callback for  connect
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void OpenSelectParentCertificateFileForconnectWinCallBack(
                                      BOOLEAN   is_success, 
                                      FILEARRAY file_array
                                      )
{
    MMIWIFI_PROFILE_T *profile_ptr = MMIWIFI_GetCurrProfile();
    
    if (is_success)
    {
        if (0 < MMIAPIFILEARRAY_GetArraySize(file_array))
        {
            
            FILEARRAY_DATA_T          file_array_data = {0};
            
            //read user selected dir
            MMIAPIFILEARRAY_Read( file_array, 0, &file_array_data );
            if (0 != file_array_data.name_len && PNULL != profile_ptr)
            {
                SCI_MEMSET(profile_ptr->parent_file_name, 0 ,sizeof(s_ui_context.s_mmiwifi_profile.parent_file_name));

                SCI_TRACE_LOW("OpenSelectParentCertificateFileForconnectWinCallBack file_name_len = %d", file_array_data.name_len);

                file_array_data.name_len = MIN(file_array_data.name_len, MMIWIFI_MAX_FILE_NAME_LEN);

                MMIAPICOM_Wstrncpy(profile_ptr->parent_file_name, file_array_data.filename, file_array_data.name_len);
            }
        }
        else
        {
            //SCI_TRACE_LOW:"OpenSelectParentCertificateFileForconnectWinCallBack array size == 0"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_7900_112_2_18_3_10_8_207,(uint8*)"");
        }
    }
    else
    {
        //SCI_TRACE_LOW:"OpenSelectParentCertificateFileForconnectWinCallBack failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_7905_112_2_18_3_10_8_208,(uint8*)"");
    }

    MMIAPIFILEARRAY_Destroy(&file_array);
    file_array = PNULL;
}

/*****************************************************************************/
//  Description : init wifi module
//  Global resource dependence : none
//  Author: li.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIWIFI_InitModule(void)
{
    g_mmiwifi_app.ProcessMsg = MMIWIFI_HandleMsg;
    MMIWIFI_RegMenuGroup();
    MMIWIFI_RegWifiNv();
    MMIWIFI_RegWinIdNameArr();
    
    //wifi ui reg msg handle function
    MMIWIFI_RegHandleMsg(WifiHandleManagerMsg);
}

/*****************************************************************************/
//  Description : wifi application's message handler
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWIFI_HandleMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
    MMI_RESULT_E ret=MMI_RESULT_TRUE;

    if (HTTP_SIGNAL_BEG <= msg_id && msg_id <= HTTP_SIGNAL_END)
    {
        return MMIWIFI_HandleHttpMsg(msg_id,param);
    }
    else if (MMIWIFI_HandleSuppMsg(msg_id,param))
    {
        ret = MMI_RESULT_TRUE;
    }
    else
    {
        ret = MMI_RESULT_FALSE;
    }

    return ret;
}

/*****************************************************************************/
//  Description : set if wifi is on when power off 
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIWIFI_SetInfo(BOOLEAN result)
{
    MMIWIFI_SetIsWlanOn(result);
}

/*****************************************************************************/
//  Description : init wifi 
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIWIFI_Init(void)
{

#if defined(WIFI_VENDOR_CSR)
    // Initialize CSR WiFi Supplicant
#ifndef WIN32
    CsrWifiSupplicantInitialize(FALSE);
#endif
#endif

    //register wifi service
    SCI_RegisterTaskMsg(    WIFI_SERVICE,
                            SRV_WIFI_SERVICE_START,
                            SRV_WIFI_SERVICE_END,
                            SCI_IdentifyThread(),
                            PNULL );
    
    if (!MMIAPISET_GetFlyMode() && MMIWIFI_GetIsWlanOn())
    {
        MMIAPIWIFI_On(FALSE);
    }
}

/*****************************************************************************/
//  Description : wifi set ui option 
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetUiOption(MMIWIFI_OPTION_FOR_WIFION_E option)
{
    s_ui_context.option = option;
}

/*****************************************************************************/
//  Description : wifi get ui option 
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL MMIWIFI_OPTION_FOR_WIFION_E WifiGetUiOption(void)
{
    return s_ui_context.option;
}

/*****************************************************************************/
//  Description : wifi get profile
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL MMIWIFI_PROFILE_T* WifiGetProfile(void)
{
    return &(s_ui_context.s_mmiwifi_profile);
}

/*****************************************************************************/
//  Description : wifi get ssid len
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL uint32 WifiGetSsidLen(void)
{
    return s_ui_context.s_mmiwifi_profile.config_info.ssid.ssid_len;
}

/*****************************************************************************/
//  Description : wifi set ssid len
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetSsidLen(uint32 len)
{
    s_ui_context.s_mmiwifi_profile.config_info.ssid.ssid_len = len;
}

/*****************************************************************************/
//  Description : wifi get ssid
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL uint8* WifiGetSsid(void)
{
    return s_ui_context.s_mmiwifi_profile.config_info.ssid.ssid;
}

/*****************************************************************************/
//  Description : wifi set ssid
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetSsid(uint8* ssid_ptr)
{
    if (PNULL == ssid_ptr)
    {
        return;
    }

    SCI_MEMCPY(s_ui_context.s_mmiwifi_profile.config_info.ssid.ssid, ssid_ptr, WIFISUPP_SSID_MAX_LEN);
}

/*****************************************************************************/
//  Description : wifi set is wep ascii
//  Global resource dependence : 
//  Author: li.li
//  Note: //输入1 表示是ASCII,0表示是HEX
/*****************************************************************************/
LOCAL void WifiSetIsWepAscii(MMIWIFI_INDEX_T index, BOOLEAN is_ascii)
{
#ifndef MMI_WLAN_AUTHORITY_COMPLEX
    is_ascii = TRUE;
#endif /* MMI_WLAN_AUTHORITY_COMPLEX */
    s_ui_context.s_mmiwifi_profile.is_wep_ascii[index] = is_ascii;
}

/*****************************************************************************/
//  Description : wifi get is wep ascii
//  Global resource dependence : 
//  Author: li.li
//  Note:   //输出1 表示是ASCII,0表示是HEX
/*****************************************************************************/
LOCAL BOOLEAN WifiGetIsWepAscii(MMIWIFI_INDEX_T index)
{
#ifdef MMI_WLAN_AUTHORITY_COMPLEX
    return s_ui_context.s_mmiwifi_profile.is_wep_ascii[index];
#else
    return TRUE;
#endif /* MMI_WLAN_AUTHORITY_COMPLEX */
}

/*****************************************************************************/
//  Description : wifi set is wep ascii
//  Global resource dependence : 
//  Author:
//  Note: //输入1 表示是ASCII,0表示是HEX
/*****************************************************************************/
LOCAL void WifiSetIsWpaAscii(BOOLEAN is_ascii)
{
#ifndef MMI_WLAN_AUTHORITY_COMPLEX
    is_ascii = TRUE;
#endif /* MMI_WLAN_AUTHORITY_COMPLEX */
    s_ui_context.s_mmiwifi_profile.is_wpa_ascii = is_ascii;
}

/*****************************************************************************/
//  Description : wifi get is wpa ascii
//  Global resource dependence : 
//  Author:
//  Note:   //输出1 表示是ASCII,0表示是HEX
/*****************************************************************************/
LOCAL BOOLEAN WifiGetIsWpaAscii(void)
{
#ifdef MMI_WLAN_AUTHORITY_COMPLEX
    return s_ui_context.s_mmiwifi_profile.is_wpa_ascii;
#else
    return TRUE;
#endif /* MMI_WLAN_AUTHORITY_COMPLEX */
}

/*****************************************************************************/
//  Description : wifi set is wapi ascii
//  Global resource dependence : 
//  Author:
//  Note: //输入1 表示是ASCII,0表示是HEX
/*****************************************************************************/
LOCAL void WifiSetIsWapiAscii(BOOLEAN is_ascii)
{
#ifndef MMI_WLAN_AUTHORITY_COMPLEX
    is_ascii = TRUE;
#endif /* MMI_WLAN_AUTHORITY_COMPLEX */
    s_ui_context.s_mmiwifi_profile.is_wapi_ascii = is_ascii;
}

/*****************************************************************************/
//  Description : wifi get is wapi ascii
//  Global resource dependence : 
//  Author:
//  Note:   //输出1 表示是ASCII,0表示是HEX
/*****************************************************************************/
LOCAL BOOLEAN WifiGetIsWapiAscii(void)
{
#ifdef MMI_WLAN_AUTHORITY_COMPLEX
    return s_ui_context.s_mmiwifi_profile.is_wapi_ascii;
#else
    return TRUE;
#endif /* MMI_WLAN_AUTHORITY_COMPLEX */
}

/*****************************************************************************/
//  Description : wifi set current wep index
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetWepCurrIndex(uint16 index)
{
    s_ui_context.s_mmiwifi_profile.cur_wep_index = index;
    s_ui_context.s_mmiwifi_profile.config_info.credential.wep_credential.key_in_use = index;//index + 1;
}

/*****************************************************************************/
//  Description : wifi get current wep index
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL uint16 WifiGetWepCurrIndex(void)
{
    return s_ui_context.s_mmiwifi_profile.cur_wep_index;
}

/*****************************************************************************/
//  Description : wifi get encrypt protocol
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL WIFISUPP_ENCRYP_PROTOCOL_E WifiGetEncryptProtocol(void)
{
    return s_ui_context.s_mmiwifi_profile.config_info.encryp_protocol;
}

/*****************************************************************************/
//  Description : wifi set encrypt protocol
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetEncryptProtocol(WIFISUPP_ENCRYP_PROTOCOL_E protocol)
{
    s_ui_context.s_mmiwifi_profile.config_info.encryp_protocol = protocol;
}

/*****************************************************************************/
//  Description : WifiGetIsAutoConnect
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN WifiGetIsAutoConnect(void)
{
#ifdef MMI_WLAN_AUTHORITY_COMPLEX
    return s_ui_context.s_mmiwifi_profile.is_auto_connect;
#else
    return TRUE;
#endif /* MMI_WLAN_AUTHORITY_COMPLEX */
}

/*****************************************************************************/
//  Description : WifiSetIsAutoConnect
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN WifiSetIsAutoConnect(BOOLEAN is_auto_connect)
{
#ifndef MMI_WLAN_AUTHORITY_COMPLEX
    is_auto_connect = TRUE;
#endif /* MMI_WLAN_AUTHORITY_COMPLEX */
    return s_ui_context.s_mmiwifi_profile.is_auto_connect = is_auto_connect;
}

/*****************************************************************************/
//  Description : wifi get wep 64bit
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL uint8* WifiGetWep64Bit(uint16 index)
{
    return s_ui_context.s_mmiwifi_profile.config_info.credential.wep_credential.key.key_64bits_arr[index];
}

/*****************************************************************************/
//  Description : wifi set wep 64bit
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetWep64Bit(uint16 index, uint8* wep_ptr)
{
    if (PNULL == wep_ptr)
    {
        return;
    }
    
    SCI_MEMCPY(&s_ui_context.s_mmiwifi_profile.config_info.credential.wep_credential.key.key_64bits_arr[index], wep_ptr, MMIWIFI_WEP_64BIT_KEY_LEN);
}

/*****************************************************************************/
//  Description : wifi get wep 128bit
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL uint8* WifiGetWep128Bit(uint16 index)
{
    return s_ui_context.s_mmiwifi_profile.config_info.credential.wep_credential.key.key_128bits_arr[index];
}

/*****************************************************************************/
//  Description : wifi set wep 128bit
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetWep128Bit(uint16 index, uint8* wep_ptr)
{
    if (PNULL == wep_ptr)
    {
        return;
    }
    
    SCI_MEMCPY(&s_ui_context.s_mmiwifi_profile.config_info.credential.wep_credential.key.key_128bits_arr[index], wep_ptr, MMIWIFI_WEP_128BIT_KEY_LEN);
}

/*****************************************************************************/
//  Description : wifi get wpa psk
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL uint8* WifiGetWpaPsk(void)
{
    return s_ui_context.s_mmiwifi_profile.config_info.credential.wpa_credential.credential_info.wpa_psk_info.psk;
}

/*****************************************************************************/
//  Description : wifi set wpa psk
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetWpaPsk(uint8* psk_ptr)
{
    if (PNULL == psk_ptr)
    {
        return;
    }
    
    SCI_MEMCPY(s_ui_context.s_mmiwifi_profile.config_info.credential.wpa_credential.credential_info.wpa_psk_info.psk,psk_ptr,WIFISUPP_WPA_PSK_LEN);
}

/*****************************************************************************/
//  Description : wifi get wapi psk
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL uint8* WifiGetWapiPsk(void)
{
    return s_ui_context.s_mmiwifi_profile.config_info.credential.wapi_credential.credential_info.wapi_psk_info.psk;
}

/*****************************************************************************/
//  Description : wifi set wapi psk
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetWapiPsk(uint8* psk_ptr)
{
    if (PNULL != psk_ptr)
    {
        SCI_MEMSET(s_ui_context.s_mmiwifi_profile.config_info.credential.wapi_credential.credential_info.wapi_psk_info.psk,0,WIFISUPP_WPA_PSK_LEN);
        SCI_MEMCPY(s_ui_context.s_mmiwifi_profile.config_info.credential.wapi_credential.credential_info.wapi_psk_info.psk,psk_ptr,WIFISUPP_WPA_PSK_LEN);
    }
}

/*****************************************************************************/
//  Description : wifi set is ssid hide
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetIsSsidHide(BOOLEAN is_hide)
{
    s_ui_context.s_mmiwifi_profile.is_ssid_hide = is_hide;
}

/*****************************************************************************/
//  Description : wifi get is ssid hide
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN WifiGetIsSsidHide(void)
{
    return s_ui_context.s_mmiwifi_profile.is_ssid_hide;
}

/*****************************************************************************/
//  Description : wifi set wpa eap peap credential
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetWpaEapPeapCredential(WIFISUPP_INNER_CREDENTIAL_TYPE_E credential_type)
{
    s_ui_context.s_mmiwifi_profile.config_info.credential.wpa_credential.credential_info.wpa_eap_info.eap_info.peap_info.inner_credential_type = credential_type;
}

/*****************************************************************************/
//  Description : wifi get wpa eap peap credential
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL WIFISUPP_INNER_CREDENTIAL_TYPE_E WifiGetWpaEapPeapCredential(void)
{
    return s_ui_context.s_mmiwifi_profile.config_info.credential.wpa_credential.credential_info.wpa_eap_info.eap_info.peap_info.inner_credential_type;
}

/*****************************************************************************/
//  Description : wifi set ip type
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetIpType(MMIWIFI_IPGET_TYPE_E ip_type)
{
    s_ui_context.s_mmiwifi_profile.ip_get.ip_get_type = ip_type;
}

/*****************************************************************************/
//  Description : wifi get ip type
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL MMIWIFI_IPGET_TYPE_E WifiGetIpType(void)
{
    return s_ui_context.s_mmiwifi_profile.ip_get.ip_get_type;
}

/*****************************************************************************/
//  Description : wifi set network mode
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetNetworkMode(WIFISUPP_NETWORK_MODE_E network_mode)
{
    s_ui_context.s_mmiwifi_profile.config_info.network_mode = network_mode;
}

/*****************************************************************************/
//  Description : wifi get network mode
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL WIFISUPP_NETWORK_MODE_E WifiGetNetworkMode(void)
{
    return s_ui_context.s_mmiwifi_profile.config_info.network_mode;
}

/*****************************************************************************/
//  Description : wifi set wpa credential
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetWpaCredential(WIFISUPP_WPA_CREDENTIAL_TYPE_E credential_type)
{
    s_ui_context.s_mmiwifi_profile.config_info.credential.wpa_credential.credential_type = credential_type;
}

/*****************************************************************************/
//  Description : wifi get wpa credential
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL WIFISUPP_WPA_CREDENTIAL_TYPE_E WifiGetWpaCredential(void)
{
    return s_ui_context.s_mmiwifi_profile.config_info.credential.wpa_credential.credential_type;
}

/*****************************************************************************/
//  Description : wifi set wapi credential
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetWapiCredential(WIFISUPP_WAPI_CREDENTIAL_TYPE_E credential_type)
{
    s_ui_context.s_mmiwifi_profile.config_info.credential.wapi_credential.credential_type = credential_type;
}

/*****************************************************************************/
//  Description : wifi get wapi credential
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL WIFISUPP_WAPI_CREDENTIAL_TYPE_E WifiGetWapiCredential(void)
{
    return s_ui_context.s_mmiwifi_profile.config_info.credential.wapi_credential.credential_type;
}

/*****************************************************************************/
//  Description : wifi set wpa eap type
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetWpaEapType(WIFISUPP_WPA_EAP_TYPE_E eap_type)
{
    s_ui_context.s_mmiwifi_profile.config_info.credential.wpa_credential.credential_info.wpa_eap_info.eap_type = eap_type;
}

/*****************************************************************************/
//  Description : wifi get wpa eap type
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL WIFISUPP_WPA_EAP_TYPE_E WifiGetWpaEapType(void)
{
    return s_ui_context.s_mmiwifi_profile.config_info.credential.wpa_credential.credential_info.wpa_eap_info.eap_type;
}

/*****************************************************************************/
//  Description : wifi set wpa eap ttls credential
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetWpaEapTtlsCredential(WIFISUPP_INNER_CREDENTIAL_TYPE_E credential_type)
{
    s_ui_context.s_mmiwifi_profile.config_info.credential.wpa_credential.credential_info.wpa_eap_info.eap_info.ttls_info.inner_credential_type = credential_type;
}

/*****************************************************************************/
//  Description : wifi get wpa eap ttls credential
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL WIFISUPP_INNER_CREDENTIAL_TYPE_E WifiGetWpaEapTtlsCredential(void)
{
    return s_ui_context.s_mmiwifi_profile.config_info.credential.wpa_credential.credential_info.wpa_eap_info.eap_info.ttls_info.inner_credential_type;
}

/*****************************************************************************/
//  Description : wifi set wep key type
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetWepKeyType(WIFISUPP_WEP_KEY_TYPE_E key_type)
{
    s_ui_context.s_mmiwifi_profile.config_info.credential.wep_credential.key_type = key_type;
}

/*****************************************************************************/
//  Description : wifi get wep key type
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL WIFISUPP_WEP_KEY_TYPE_E WifiGetWepKeyType(void)
{
    return s_ui_context.s_mmiwifi_profile.config_info.credential.wep_credential.key_type;
}

/*****************************************************************************/
//  Description : WifiGetWapiCertiClientFilePath
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL uint16* WifiGetWapiCertiClientFilePath(void)
{
    return s_ui_context.s_mmiwifi_profile.client_file_name;
}

/*****************************************************************************/
//  Description : WifiSetWapiCertiClientFilePath
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetWapiCertiClientFilePath(uint16* path_ptr, uint32 file_name_len)
{
    if (PNULL != path_ptr && 0 != file_name_len)
    {
        SCI_MEMSET(s_ui_context.s_mmiwifi_profile.client_file_name, 0 ,sizeof(s_ui_context.s_mmiwifi_profile.client_file_name));

        SCI_TRACE_LOW("WifiSetWapiCertiClientFilePath file_name_len = %d", file_name_len);

        file_name_len = MIN(file_name_len, MMIWIFI_MAX_FILE_NAME_LEN);

        MMIAPICOM_Wstrncpy( s_ui_context.s_mmiwifi_profile.client_file_name, path_ptr, file_name_len );
    }
}

/*****************************************************************************/
//  Description : WifiGetWapiCertiParentFilePath
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL uint16* WifiGetWapiCertiParentFilePath(void)
{
    return s_ui_context.s_mmiwifi_profile.parent_file_name;
}

/*****************************************************************************/
//  Description : WifiSetWapiCertiClientFilePath
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetWapiCertiParentFilePath(uint16* path_ptr, uint32 file_name_len)
{
    if (PNULL != path_ptr && 0 != file_name_len)
    {
        SCI_MEMSET(s_ui_context.s_mmiwifi_profile.parent_file_name, 0 ,sizeof(s_ui_context.s_mmiwifi_profile.parent_file_name));

        SCI_TRACE_LOW("WifiSetWapiCertiParentFilePath file_name_len = %d", file_name_len);

        file_name_len = MIN(file_name_len, MMIWIFI_MAX_FILE_NAME_LEN);

        MMIAPICOM_Wstrncpy( s_ui_context.s_mmiwifi_profile.parent_file_name, path_ptr, file_name_len );
    }
}

/*****************************************************************************/
//  Description : wifi get ip address
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL char* WifiGetIpAddr(void)
{
    return s_ui_context.s_mmiwifi_profile.ip_get.ip.ip_addr;
}

/*****************************************************************************/
//  Description : wifi set ip address
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetIpAddr(char* ip_ptr)
{
    if (PNULL != ip_ptr)
    {
        uint16 ip_str_len = 0;
        
        SCI_MEMSET(s_ui_context.s_mmiwifi_profile.ip_get.ip.ip_addr,0,sizeof(s_ui_context.s_mmiwifi_profile.ip_get.ip.ip_addr));
        ip_str_len = SCI_STRLEN(ip_ptr);
        strncpy(s_ui_context.s_mmiwifi_profile.ip_get.ip.ip_addr, ip_ptr, MIN(ip_str_len, MMIWIFI_IP_LEN));
    }
}

/*****************************************************************************/
//  Description : wifi get ip gateway
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL char* WifiGetIpGateway(void)
{
    return s_ui_context.s_mmiwifi_profile.ip_get.ip.default_gateway;
}

/*****************************************************************************/
//  Description : wifi set ip gateway
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetIpGateway(char* ip_ptr)
{
    if (PNULL != ip_ptr)
    {
        uint16 ip_str_len = 0;
        
        SCI_MEMSET(s_ui_context.s_mmiwifi_profile.ip_get.ip.default_gateway,0,sizeof(s_ui_context.s_mmiwifi_profile.ip_get.ip.default_gateway));
        ip_str_len = SCI_STRLEN(ip_ptr);
        strncpy(s_ui_context.s_mmiwifi_profile.ip_get.ip.default_gateway, ip_ptr, MIN(ip_str_len, MMIWIFI_IP_LEN));
    }
}

/*****************************************************************************/
//  Description : wifi get ip submask
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL char* WifiGetIpSubmask(void)
{
    return s_ui_context.s_mmiwifi_profile.ip_get.ip.subnet_mask;
}

/*****************************************************************************/
//  Description : wifi set ip submask
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetIpSubmask(char* ip_ptr)
{
    if (PNULL != ip_ptr)
    {
        uint16 ip_str_len = 0;
        
        SCI_MEMSET(s_ui_context.s_mmiwifi_profile.ip_get.ip.subnet_mask,0,sizeof(s_ui_context.s_mmiwifi_profile.ip_get.ip.subnet_mask));
        ip_str_len = SCI_STRLEN(ip_ptr);
        strncpy(s_ui_context.s_mmiwifi_profile.ip_get.ip.subnet_mask, ip_ptr, MIN(ip_str_len, MMIWIFI_IP_LEN));
    }
}

/*****************************************************************************/
//  Description : wifi get ip dns1
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL char* WifiGetIpDns1(void)
{
    return s_ui_context.s_mmiwifi_profile.ip_get.ip.first_dns;
}

/*****************************************************************************/
//  Description : wifi set ip dns1
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetIpDns1(char* ip_ptr)
{
    if (PNULL != ip_ptr)
    {
        uint16 ip_str_len = 0;
        
        SCI_MEMSET(s_ui_context.s_mmiwifi_profile.ip_get.ip.first_dns,0,sizeof(s_ui_context.s_mmiwifi_profile.ip_get.ip.first_dns));
        ip_str_len = SCI_STRLEN(ip_ptr);
        strncpy(s_ui_context.s_mmiwifi_profile.ip_get.ip.first_dns, ip_ptr, MIN(ip_str_len, MMIWIFI_IP_LEN));
    }
}

/*****************************************************************************/
//  Description : wifi get ip dns2
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL char* WifiGetIpDns2(void)
{
    return s_ui_context.s_mmiwifi_profile.ip_get.ip.second_dns;
}

/*****************************************************************************/
//  Description : wifi set ip dns2
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetIpDns2(char* ip_ptr)
{
    if (PNULL != ip_ptr)
    {
        uint16 ip_str_len = 0;
        
        SCI_MEMSET(s_ui_context.s_mmiwifi_profile.ip_get.ip.second_dns,0,sizeof(s_ui_context.s_mmiwifi_profile.ip_get.ip.second_dns));
        ip_str_len = SCI_STRLEN(ip_ptr);
        strncpy(s_ui_context.s_mmiwifi_profile.ip_get.ip.second_dns, ip_ptr, MIN(ip_str_len, MMIWIFI_IP_LEN));
    }
}

/*****************************************************************************/
//  Description : hex to num
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL int32 WifiHexToNum(char c)
{
    if (c >= '0' && c <= '9')
    {
        return c - '0';
    }
    
    if (c >= 'a' && c <= 'f')
    {
        return c - 'a' + 10;
    }
    
    if (c >= 'A' && c <= 'F')
    {
        return c - 'A' + 10;
    }
    
    return -1;
}

/*****************************************************************************/
//  Description : hex to byte
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL int32 WifiHexToByte(const char* hex_ptr)
{
    int32 a = 0;
    int32 b = 0;
    a = WifiHexToNum(*hex_ptr++);
    if (a < 0)
    {
        return -1;
    }
    
    b = WifiHexToNum(*hex_ptr++);
    if (b < 0)
    {
        return -1;
    }
    
    return (a << 4) | b;
}

/*****************************************************************************/
//  Description : hex to bin
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL int32 WifiHexStrToBin(const char* hex_ptr, uint8* buf_ptr, int32 len)
{
    int32 i = 0;
    int32 a = 0;
    const char *ipos_ptr = hex_ptr;
    uint8* opos_ptr = buf_ptr;
    
    for (i = 0; i < len; i++)
    {
        a = WifiHexToByte(ipos_ptr);
        if (a < 0)
        {
            return -1;
        }
        
        *opos_ptr++ = a;
        ipos_ptr += 2;
    }
    return 0;
}

/*****************************************************************************/
//  Description : reset encrypt and credential
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiResetEncryptAndCredential(void)
{
    s_ui_context.s_mmiwifi_profile.config_info.encryp_protocol = WIFISUPP_ENCRYP_PROTOCOL_OPENSYS;
    SCI_MEMSET(&(s_ui_context.s_mmiwifi_profile.config_info.credential),0,sizeof(s_ui_context.s_mmiwifi_profile.config_info.credential));
    s_ui_context.s_mmiwifi_profile.cur_wep_index = 0;
    SCI_MEMSET(&(s_ui_context.s_mmiwifi_profile.is_wep_ascii),0,sizeof(s_ui_context.s_mmiwifi_profile.is_wep_ascii));
}

/*****************************************************************************/
//  Description : reset credential
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiResetCredential(void)
{
    SCI_MEMSET(&(s_ui_context.s_mmiwifi_profile.config_info.credential),0,sizeof(s_ui_context.s_mmiwifi_profile.config_info.credential));
    s_ui_context.s_mmiwifi_profile.cur_wep_index = 0;
    SCI_MEMSET(&(s_ui_context.s_mmiwifi_profile.is_wep_ascii),0,sizeof(s_ui_context.s_mmiwifi_profile.is_wep_ascii));
    s_ui_context.s_mmiwifi_profile.is_wpa_ascii= 0;
    s_ui_context.s_mmiwifi_profile.is_wapi_ascii= 0;
    SCI_MEMSET(&(s_ui_context.s_mmiwifi_profile.parent_file_name),0,sizeof(s_ui_context.s_mmiwifi_profile.parent_file_name));
    SCI_MEMSET(&(s_ui_context.s_mmiwifi_profile.client_file_name),0,sizeof(s_ui_context.s_mmiwifi_profile.client_file_name));
}

/*****************************************************************************/
//  Description : reset WAPI credential
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void WifiResetWAPICredential(void)
{
    SCI_MEMSET(&(s_ui_context.s_mmiwifi_profile.config_info.credential.wapi_credential.credential_info),0,sizeof(s_ui_context.s_mmiwifi_profile.config_info.credential.wapi_credential.credential_info));
    s_ui_context.s_mmiwifi_profile.is_wapi_ascii= 0;
    SCI_MEMSET(&(s_ui_context.s_mmiwifi_profile.parent_file_name),0,sizeof(s_ui_context.s_mmiwifi_profile.parent_file_name));
    SCI_MEMSET(&(s_ui_context.s_mmiwifi_profile.client_file_name),0,sizeof(s_ui_context.s_mmiwifi_profile.client_file_name));
}

/*****************************************************************************/
//  Description : reset wep key
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiResetWepKey(void)
{
    SCI_MEMSET(&(s_ui_context.s_mmiwifi_profile.config_info.credential.wep_credential.key),0,sizeof(s_ui_context.s_mmiwifi_profile.config_info.credential.wep_credential.key));
    s_ui_context.s_mmiwifi_profile.cur_wep_index = 0;
    SCI_MEMSET(&(s_ui_context.s_mmiwifi_profile.is_wep_ascii),0,sizeof(s_ui_context.s_mmiwifi_profile.is_wep_ascii));
}

/*****************************************************************************/
//  Description : reset wpa credential
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiResetWpaCredential(void)
{
    SCI_MEMSET(&(s_ui_context.s_mmiwifi_profile.config_info.credential.wpa_credential.credential_info),0,sizeof(s_ui_context.s_mmiwifi_profile.config_info.credential.wpa_credential.credential_info));
}

/*****************************************************************************/
//  Description : wifi reset wep 64bit
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiResetWep64Bit(uint16 index)
{
    SCI_MEMSET(&s_ui_context.s_mmiwifi_profile.config_info.credential.wep_credential.key.key_64bits_arr[index],0,MMIWIFI_WEP_64BIT_KEY_LEN);
}

/*****************************************************************************/
//  Description : wifi reset wep 128bit
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void WifiResetWep128Bit(uint16 index)
{
    SCI_MEMSET(&s_ui_context.s_mmiwifi_profile.config_info.credential.wep_credential.key.key_128bits_arr[index],0,MMIWIFI_WEP_128BIT_KEY_LEN);
}

/*****************************************************************************/
//  Description : wifi check is hex
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN WifiCheckIsHex(uint8* str_ptr, uint32 len)
{
    BOOLEAN result = FALSE;
    uint32  i = 0;
    uint8*  temp_ptr = PNULL;

    if (PNULL != str_ptr && 0 != len)
    {
        temp_ptr = str_ptr;
        for (i = 0; i< len; i++)
        {
            
            if ((temp_ptr[i] >= '0' && temp_ptr[i] <= '9')||(temp_ptr[i] >= 'a' && temp_ptr[i] <= 'f')||(temp_ptr[i] >= 'A' && temp_ptr[i] <= 'F'))
            {
                result = TRUE;
            }
            else
            {
                result = FALSE;
                break;
            }
        }
    }
    return result;
}

/*****************************************************************************/
//  Description : 恢复WiFi模块到出厂模式
//  Global resource dependence : 
//  Author: George.liu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIWIFI_Reset(void)
{
    MMIWIFI_ACCOUNT_INFO_T init_info = {0};
    uint16 port = 0;
    MMIWIFI_PROXY_NAME_E proxy_name = {0};
    MMIWIFI_PROXY_PASSWORD_E proxy_pwd = {0};
    MMIWIFI_PROXY_REJECTLIST_E proxy_reject = {0};
    MMIWIFI_PORTAL_CONFIG_T portal_cfg_ptr = {0};
    MMI_GPRSTOWLAN_RESELECT  gprstowlan = MMI_GPRSTOWLAN_ALWAYS_INQUIRY;
    MMI_AP_RESELECT ap_select = MMI_AP_AUTO_RESELCET;

    TRACE_APP_WLAN("Enter");

    /* 清除AP列表 */
    MMIWIFI_Clear();

    /* 关闭wifi */
    if (MMIAPIWIFI_GetStatus() != MMIWIFI_STATUS_OFF)
    {
        MMIAPIWIFI_SetInfo(FALSE);
        MMIAPIWIFI_Off();
    }

    MMIWIFI_SetIsAutoConnectingOn(FALSE);
    MMIWIFI_SetIsAutoConnect(TRUE);
    MMIWIFI_SetAccountInfo(&init_info);
    MMIWIFI_SetIPType(FALSE);
    MMIWIFI_SetProxyQuery(FALSE);
    MMIWIFI_SetIPData(0);
    MMIWIFI_SetGateWay(0);
    MMIWIFI_SetSubMask(0);
    MMIWIFI_SetDNS1(0);
    MMIWIFI_SetDNS2(0);
    MMIWIFI_SetProxyURL("");
    MMIWIFI_SetProxyPort(&port);
    MMIWIFI_SetProxyName(&proxy_name);
    MMIWIFI_SetProxyPassword(&proxy_pwd);
    MMIWIFI_SetProxyRejectList(&proxy_reject);
    MMIWIFI_SetWlanAutoScanStatus(1);

#ifndef MMI_GUI_STYLE_TYPICAL
    portal_cfg_ptr.is_remember_password = TRUE;
    portal_cfg_ptr.is_notify = TRUE;
#endif /* MMI_GUI_STYLE_TYPICAL */
    MMIWIFI_SetPortalConfig(&portal_cfg_ptr);

    MMIWIFI_SetGPRSToWlanReselect(&gprstowlan);
    MMIWIFI_SetApReselect(&ap_select);
    MMIWIFI_SetIsShowNoWlanPrompt(TRUE);
    MMIWIFI_SetServiceProvider(MMIWIFI_PORTAL_SERVICE_PROVIDER_CHINA_MOBILE);
    MMIWIFI_SetActiveCMD(MMIWIFI_PORTAL_COMMAND_ACTIVE_STRING);
    MMIWIFI_SetResetPasswordCMD(MMIWIFI_PORTAL_COMMAND_RESET_PASSWORD_STRING);
    MMIWIFI_SetModifyPasswordCMD(MMIWIFI_PORTAL_COMMAND_MODIFY_PASSWORD_STRING);
    MMIWIFI_SetMyFormTag(MMIWIFI_PORTAL_MYFORM);
    MMIWIFI_SetLoginFormTag(MMIWIFI_PORTAL_LOGINFORM);
    MMIWIFI_SetPortalVersion(MMIWIFI_PORTAL_VERSION_INFO);
}

/*****************************************************************************/
//  Description : set ui option max
//  Global resource dependence : 
//  Author: bin.ji
//  Note: fix MS00206348
/*****************************************************************************/
PUBLIC void MMIAPIWIFI_SetUiOptionMAX(void)
{
    WifiSetUiOption(MMIWIFI_UI_OPTION_MAX);
}

/****************************************************************************/
//  Description : open wifi Main window
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_OpenMainWin(MMIWIFI_ENTRY_TYPE_E entry_type)
{
    //SCI_TRACE_LOW:"MMIWIFI_OpenMainWin in_focus_win = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_8863_112_2_18_3_10_11_209,(uint8*)"d", entry_type);

    if(MMIAPISET_GetFlyMode()&& (FALSE == MMIAPIENG_GetIsFlightModeWifi()))
    {
        MMIPUB_OpenAlertWarningWin(TXT_CLOSE_FLY_MODE_FIRST); 
        return;
    }

    if (!MMIWIFI_GetIsInitialized()) {
        MMIWIFI_ProfileInitialize();
        MMIWIFI_SetIsInitialized(TRUE);
    }

#ifdef MMI_WLAN_TAB_STYLE
    if (!MMK_IsOpenWin(MMIWIFI_MAIN_PARENT_WIN_ID)) {

        MMK_CreateParentWin((uint32 *)MMIWIFI_MAIN_PARENT_WIN_TAB, NULL);
        MMK_CreateChildWin(MMIWIFI_MAIN_PARENT_WIN_ID, (uint32 *)MMIWIFI_LIST_WIN_TAB, NULL);
        MMK_CreateChildWin(MMIWIFI_MAIN_PARENT_WIN_ID, (uint32 *)MMIWIFI_TRUSTED_LIST_CHILD_WIN_TAB, NULL);
#ifndef MMI_GUI_STYLE_TYPICAL
        MMK_CreateChildWin(MMIWIFI_MAIN_PARENT_WIN_ID, (uint32 *)MMIWIFI_SETTING_LIST_CHILD_WIN_TAB, NULL);
#else
        MMK_CreateChildWin(MMIWIFI_MAIN_PARENT_WIN_ID, (uint32 *)MMIWIFI_WLAN_SETTING_WIN_TAB, NULL);
#endif /* MMI_GUI_STYLE_TYPICAL */
    }

    if (!MMIWIFI_GetIsWlanOn()) {
        entry_type = MMIWIFI_ENTRY_TYPE_SETTINGS;
    }

    switch(entry_type) {
    case MMIWIFI_ENTRY_TYPE_LIST:
        MMK_SetChildWinFocus(MMIWIFI_MAIN_PARENT_WIN_ID, MMIWIFI_LIST_WIN_ID);
        break;

    case MMIWIFI_ENTRY_TYPE_TRUSTED_LIST:
         MMK_SetChildWinFocus(MMIWIFI_MAIN_PARENT_WIN_ID, MMIWIFI_TRUSTED_LIST_CHILD_WIN_ID);
        break;

    case MMIWIFI_ENTRY_TYPE_SETTINGS:
#ifndef MMI_GUI_STYLE_TYPICAL
        MMK_SetChildWinFocus(MMIWIFI_MAIN_PARENT_WIN_ID, MMIWIFI_SETTING_LIST_CHILD_WIN_ID);
#else
        MMK_SetChildWinFocus(MMIWIFI_MAIN_PARENT_WIN_ID, MMIWLAN_SETTING_WIN_ID);
#endif /* MMI_GUI_STYLE_TYPICAL */
        break;

    default:
        MMK_SetChildWinFocus(MMIWIFI_MAIN_PARENT_WIN_ID, MMIWIFI_LIST_WIN_ID);
        //SCI_TRACE_LOW:"MMIWIFI_OpenMainWin Bad param"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_8912_112_2_18_3_10_11_210,(uint8*)"");
        return;
    }
#else
    if (MMK_IsOpenWin(MMIWIFI_LIST_WIN_ID))
    {
        MMK_WinGrabFocus(MMIWIFI_LIST_WIN_ID);
    }
    else
    {
        MMK_CreateWin((uint32*)MMIWIFI_LIST_WIN_TAB, PNULL);
    }
    return;

#endif /*MMI_WLAN_TAB_STYLE*/
}

/****************************************************************************/
//  Description : open default access point setting window
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void MMIWIFI_OpenDefAPSettingWin(void)
{
    //SCI_TRACE_LOW:"MMIWIFI_OpenDefAPSettingWin "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_8935_112_2_18_3_10_11_211,(uint8*)"");

    if (!MMIWIFI_GetIsInitialized()) {
        MMIWIFI_ProfileInitialize();
        MMIWIFI_SetIsInitialized(TRUE);
    }

    if (MMK_IsOpenWin(MMIWIFI_DEF_AP_SET_WIN_ID)) {

        MMK_WinGrabFocus(MMIWIFI_DEF_AP_SET_WIN_ID);
    }
    else
    {
        MMK_CreateWin((uint32 *)MMIWIFI_DEF_AP_SET_WIN_TAB,PNULL);
    }
}

#ifndef MMI_GUI_STYLE_TYPICAL
/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleSettinglistChildWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"WifiHandleSettinglistChildWinMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_8961_112_2_18_3_10_11_212,(uint8*)"d", msg_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        WifiSetSettingListProperty(win_id);
        MMK_SetAtvCtrl(win_id, MMIWIFI_LIST_SETTING_CTRL_ID);
        break;

    case MSG_FULL_PAINT:
        WifiAppendSettingListItem(MMIWIFI_LIST_SETTING_CTRL_ID);
        break;

    case MSG_GET_FOCUS:
#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
#endif
        break;

    case MSG_APP_MENU:
        WifiOpenConnectMenuWin(win_id);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:
        {
            uint32 index = 0;
            SETTING_ITEM_FUNC item_func = PNULL;

            index = GUILIST_GetCurItemIndex(MMIWIFI_LIST_SETTING_CTRL_ID);
            item_func = (SETTING_ITEM_FUNC)WifiGetListUserdata(MMIWIFI_LIST_SETTING_CTRL_ID, index);

            if (PNULL != item_func)
            {
                (*item_func)();
            }
        }
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
#ifdef MMI_WLAN_TAB_STYLE
        MMK_CloseParentWin(MMIWIFI_MAIN_PARENT_WIN_ID);
#else
        MMK_CloseWin(win_id);
#endif /* MMI_WLAN_TAB_STYLE */
        break;

    case MSG_CLOSE_WINDOW:
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
LOCAL void  WifiCreateSearchMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    MMI_TEXT_ID_T auto_scan_index[] = {TXT_CLOSE, TXT_WIFI_PER_1_MIN, TXT_WIFI_PER_2_MIN, TXT_WIFI_PER_5_MIN, TXT_WIFI_PER_10_MIN};
    uint32 node_index[] = {MMIWLAN_AUTOSCAN_OPT_OFF, MMIWLAN_AUTOSCAN_OPT_1_MIN, MMIWLAN_AUTOSCAN_OPT_2_MIN, MMIWLAN_AUTOSCAN_OPT_5_MIN, MMIWLAN_AUTOSCAN_OPT_10_MIN};
    uint16 select_index = 0;
    uint32 num = 0;

    num = sizeof(auto_scan_index) / sizeof(auto_scan_index[0]);
  
    select_index = MMIWIFI_GetWlanAutoScanStatus();
    WifiCreateRadioListByTextId(win_id, ctrl_id, num, auto_scan_index, node_index, select_index);
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiCreateWpaStateSetMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    MMI_TEXT_ID_T       state_index[] = {TXT_WIFI_WEP_KEY_HEX, TXT_WIFI_WEP_KEY_ASCII};
    uint32              node_index[] = {MMIWIFI_WPA_STATE_KEY_HEX, MMIWIFI_WPA_STATE_KEY_ASCII};
    uint16              select_index = 0;
    uint32              num = 0;
    MMIWIFI_PROFILE_T   *profile_ptr = PNULL;
        
    profile_ptr = MMIWIFI_GetCurrProfile();

    num = sizeof(state_index) / sizeof(state_index[0]);

    if (PNULL != profile_ptr)
    {
        select_index = (uint16)profile_ptr->is_wpa_ascii;
    }
    WifiCreateRadioListByTextId(win_id, ctrl_id, num, state_index, node_index, select_index);
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiCreateAPEditWpaStateSetMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    MMI_TEXT_ID_T       state_index[] = {TXT_WIFI_WEP_KEY_HEX, TXT_WIFI_WEP_KEY_ASCII};
    uint32              node_index[] = {MMIWIFI_WPA_STATE_KEY_HEX, MMIWIFI_WPA_STATE_KEY_ASCII};
    uint16              select_index = 0;
    uint32              num = 0;

    num = sizeof(state_index) / sizeof(state_index[0]);

    select_index = (uint16)WifiGetIsWpaAscii();
    WifiCreateRadioListByTextId(win_id, ctrl_id, num, state_index, node_index, select_index);
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiCreateAPNetworkStateMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    MMI_TEXT_ID_T       network_state_index[2] = {TXT_WIFI_NETWORK_STATE_OPEN,TXT_HIDE};
    uint32              node_index[] = {MMIWIFI_AP_NETWORK_STATE_OPEN, MMIWIFI_AP_NETWORK_STATE_HIDE};
    uint16              select_index = 0;
    uint32              num = 0;

    num = sizeof(node_index) / sizeof(node_index[0]);

    select_index = (uint16)WifiGetIsSsidHide();

    WifiCreateRadioListByTextId(win_id, ctrl_id, num, network_state_index, node_index, select_index);
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiCreateAPConnectStateMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    MMI_TEXT_ID_T       connect_state_index[2] = {TXT_MANUAL,TXT_AUTO};
    uint32              node_index[] = {MMIWIFI_AP_CONNECT_STATE_OPEN, MMIWIFI_AP_CONNECT_STATE_HIDE};
    uint16              select_index = 0;
    uint32              num = 0;

    num = sizeof(node_index) / sizeof(node_index[0]);

    select_index = (uint16)WifiGetIsAutoConnect();

    WifiCreateRadioListByTextId(win_id, ctrl_id, num, connect_state_index, node_index, select_index);
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiCreateAPAuthTypeMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
#if 0
    MMI_TEXT_ID_T       auth_type_index[] = {TXT_WIFI_AUTH_TYPE_OPENSYS,TXT_WIFI_AUTH_TYPE_WEP,TXT_WIFI_AUTH_TYPE_WPA,TXT_WIFI_AUTH_TYPE_WPA2,TXT_WIFI_AUTH_TYPE_WAPI, TXT_WLAN_EAP_SIM};
    uint32              node_index[] = {MMIWIFI_AP_AUTH_TYPE_OPEN, MMIWIFI_AP_AUTH_TYPE_WEP, MMIWIFI_AP_AUTH_TYPE_WPA, MMIWIFI_AP_AUTH_TYPE_WPA2, MMIWIFI_AP_AUTH_TYPE_WAPI, MMIWIFI_AP_AUTH_TYPE_EAP};
#else
    MMI_TEXT_ID_T       auth_type_index[] = {TXT_WIFI_AUTH_TYPE_OPENSYS,TXT_WIFI_AUTH_TYPE_WEP,TXT_WIFI_AUTH_TYPE_WPA,TXT_WIFI_AUTH_TYPE_WPA2,TXT_WIFI_AUTH_TYPE_WAPI};
    uint32              node_index[] = {MMIWIFI_AP_AUTH_TYPE_OPEN, MMIWIFI_AP_AUTH_TYPE_WEP, MMIWIFI_AP_AUTH_TYPE_WPA, MMIWIFI_AP_AUTH_TYPE_WPA2, MMIWIFI_AP_AUTH_TYPE_WAPI};
#endif
    uint16              select_index = 0;
    uint32              num = 0;

    num = sizeof(node_index) / sizeof(node_index[0]);

    select_index = (uint16)WifiGetEncryptProtocol();
    WifiCreateRadioListByTextId(win_id, ctrl_id, num, auth_type_index, node_index, select_index);
}
#ifdef WLAN_EAP_SUPPORT
/*****************************************************************************/
//  Description : wifi get Sim Index
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL uint32 WifiGetEAPSimIndex(void)
{
    return s_ui_context.s_mmiwifi_profile.config_info.credential.wpa_credential.credential_info.wpa_eap_info.eap_info.eapsim_info.simcard_num;
}

/*****************************************************************************/
//  Description : wifi set Sim Index
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetEAPSimIndex(uint32 eap_sim_index)
{
    s_ui_context.s_mmiwifi_profile.config_info.credential.wpa_credential.credential_info.wpa_eap_info.eap_info.eapsim_info.simcard_num = eap_sim_index;
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiCreateSimIndexMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    MMI_TEXT_ID_T       sim_index[] = {TXT_SIM_SEL_SIM1, TXT_SIM_SEL_SIM2, TXT_SIM_SEL_SIM3, TXT_SIM_SEL_SIM4};
    uint32              node_index[] = {MMIWIFI_EAP_SIM_INDEX_1, MMIWIFI_EAP_SIM_INDEX_2, MMIWIFI_EAP_SIM_INDEX_3, MMIWIFI_EAP_SIM_INDEX_4};
    uint16              select_index = 0;
    uint32              num = 0;

    num = MMI_DUAL_SYS_MAX;

    select_index = (uint16)WifiGetEAPSimIndex();

    WifiCreateRadioListByTextId(win_id, ctrl_id, num, sim_index, node_index, select_index);
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleAPEAPSimIndexWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"WifiHandleAPEAPSimIndexWinMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_9182_112_2_18_3_10_12_213,(uint8*)"d", msg_id);

    switch (msg_id) {
    case MSG_OPEN_WINDOW:
        WifiCreateSimIndexMenu(win_id, MMIWIFI_MENU_AP_EAP_SIM_INDEX_CTRL_ID);
        MMK_SetAtvCtrl(win_id, MMIWIFI_MENU_AP_EAP_SIM_INDEX_CTRL_ID);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        {
            uint32              sim_index = 0;

            sim_index = GUILIST_GetCurItemIndex(MMIWIFI_MENU_AP_EAP_SIM_INDEX_CTRL_ID);

            WifiSetEAPSimIndex(sim_index);

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
// Description :
// Global resource dependence : none
// Author:Jiaoyou.wu
// Note:    
/*****************************************************************************/
LOCAL void WifiHandleAPEditEAPSimIndex()
{
    MMK_CreatePubListWin((uint32 *)MMIWIFI_AP_EAP_SIM_INDEX_WIN_TAB, PNULL);
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void WifiCreateEapTypeMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    MMI_TEXT_ID_T       sim_index[] = {TXT_WLAN_SIM, TXT_ACCOUNT};

    uint32              node_index[] = {MMIWIFI_EAP_TYPE_SIM, MMIWIFI_EAP_TYPE_ACCOUNT};
    uint16              select_index = 0;
    uint32              num = 0;

    num = sizeof(node_index) / sizeof(node_index[0]);

    select_index = (uint16)WifiGetEAPSimIndex();

    WifiCreateRadioListByTextId(win_id, ctrl_id, num, sim_index, node_index, select_index);

}
/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleWifiEapTypeSelectWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    SCI_TRACE_LOW("WifiHandleWifiEapTypeSelectWinMsg msg_id = %x", msg_id);

    switch (msg_id) {
    case MSG_OPEN_WINDOW:
        WifiCreateEapTypeMenu(win_id, MMIWIFI_MENU_EAP_TYPE_CTRL_ID);
        MMK_SetAtvCtrl(win_id, MMIWIFI_MENU_EAP_TYPE_CTRL_ID);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        {
            uint32              sim_index = 0;
			ADD_DATA			add_data = MMK_GetWinAddDataPtr(win_id);

            sim_index = GUILIST_GetCurItemIndex(MMIWIFI_MENU_EAP_TYPE_CTRL_ID);

			if (0 == sim_index)
			{
				MMIWLAN_OpenSimSelectWin(add_data);
			}
			else
			{
				WifiOpenEAPAccountWin(add_data);
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
// Description :
// Global resource dependence : none
// Author:Jiaoyou.wu
// Note:    
/*****************************************************************************/
PUBLIC BOOLEAN MMIWLAN_OpenEAPTypeSelectWin(ADD_DATA user_data)
{
    MMK_CreatePubListWin((uint32 *)MMIWIFI_EAP_TYPE_SELECT_WIN_TAB, user_data);
	return TRUE;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL void  WifiOpenEAPAccountWin(ADD_DATA add_data)
{
    MMK_CreateWin((uint32 *)MMIWIFI_EAP_ACCOUNT_WIN_TAB, add_data);
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E HandleEAPAccountWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
   MMI_RESULT_E result = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIFORM_SetType(MMIWIFI_FORM_EAP_ACCOUNT_CTRL_ID, GUIFORM_TYPE_TP);
        GUIEDIT_SetAlign(MMIWIFI_EDIT_EAP_USER_CTRL_ID, ALIGN_LVMIDDLE);
        GUIEDIT_SetAlign(MMIWIFI_EDIT_EAP_PASSWORD_CTRL_ID, ALIGN_LVMIDDLE);
        GUIFORM_SetStyle(MMIWIFI_CHILD_FORM_EAP_USER_CTRL_ID, GUIFORM_STYLE_UNIT);
        GUIFORM_SetStyle(MMIWIFI_CHILD_FORM_EAP_USER_CTRL_ID, GUIFORM_STYLE_UNIT);
        GUILABEL_SetTextById(MMIWIFI_LABEL_EAP_USER_CTRL_ID, TXT_COMMON_USERNAME, FALSE);
        GUILABEL_SetTextById(MMIWIFI_LABEL_EAP_PASSWORD_CTRL_ID, TXT_COMMON_PASSWORD, FALSE);

        GUIEDIT_SetIm(MMIWIFI_EDIT_EAP_USER_CTRL_ID, GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL,GUIIM_TYPE_ABC);/*lint !e655*/

        GUIEDIT_SetPasswordStyle(MMIWIFI_EDIT_EAP_PASSWORD_CTRL_ID, GUIEDIT_PASSWORD_STYLE_ALL);
        GUIEDIT_SetIm(MMIWIFI_EDIT_EAP_PASSWORD_CTRL_ID, GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL,GUIIM_TYPE_ABC);/*lint !e655*/
        MMK_SetAtvCtrl(win_id, MMIWIFI_EDIT_EAP_USER_CTRL_ID);
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
            MMI_STRING_T pwd = {0};

            GUIEDIT_GetString(MMIWIFI_EDIT_EAP_USER_CTRL_ID, &user);
            GUIEDIT_GetString(MMIWIFI_EDIT_EAP_PASSWORD_CTRL_ID, &pwd);

            if (0 == user.wstr_len || 0 == pwd.wstr_len)
            {
                MMIPUB_OpenAlertWarningWin(TXT_WLAN_PORTAL_ALERT_INPUT);         
                break;
            }
            else
            {
				MMIWIFI_PROFILE_T   *profile_ptr = PNULL;
                char                *username = PNULL;
                char                *password = PNULL;

				profile_ptr = (MMIWIFI_PROFILE_T *)MMK_GetWinAddDataPtr(win_id);
				
                if (PNULL != profile_ptr)
                {
                    profile_ptr->config_info.credential.wpa_credential.credential_info.wpa_eap_info.eap_type = WIFISUPP_WPA_EAP_TYPE_PEAP;
                    username = profile_ptr->config_info.credential.wpa_credential.credential_info.wpa_eap_info.eap_info.peap_info.username_arr;
                    password = profile_ptr->config_info.credential.wpa_credential.credential_info.wpa_eap_info.eap_info.peap_info.psw_arr;

                    GUI_WstrToUTF8((uint8 *)username, WIFISUPP_MAX_USERNAME_LEN, user.wstr_ptr, user.wstr_len);
                    GUI_WstrToUTF8((uint8 *)password, WIFISUPP_MAX_PSW_LEN, pwd.wstr_ptr, pwd.wstr_len);

				    MMIWIFI_OpenConnectWaitWin();
				    MMK_CloseWin(win_id);
				    (void)MMIWIFI_DirectConnect(&(profile_ptr->config_info));/*lint !e831 !e613*/
				    profile_ptr->state = MMIWIFI_PROFILE_STATE_CONNECTING;/*lint !e831 !e613*/
                }
            }
        }
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        {
			MMIWIFI_PROFILE_T *profile_ptr = PNULL;

			profile_ptr = (MMIWIFI_PROFILE_T *)MMK_GetWinAddDataPtr(win_id);

            if (PNULL != profile_ptr)
            {
                profile_ptr->is_editing = FALSE;
            }
        }

    default:
        result = MMI_RESULT_FALSE;
     break;

    }

    return (result);
}

#endif /* WLAN_EAP_SUPPORT */

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiCreateWapiStateSetMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    MMI_TEXT_ID_T       state_index[] = {TXT_WIFI_WEP_KEY_HEX, TXT_WIFI_WEP_KEY_ASCII};
    uint32              node_index[] = {MMIWIFI_WAPI_STATE_KEY_HEX, MMIWIFI_WAPI_STATE_KEY_ASCII};
    uint16              select_index = 0;
    uint32              num = 0;
    MMIWIFI_PROFILE_T   *profile_ptr = PNULL;
        
    profile_ptr = MMIWIFI_GetCurrProfile();

    num = sizeof(state_index) / sizeof(state_index[0]);

    if (PNULL != profile_ptr)
    {
        select_index = (uint16)profile_ptr->is_wapi_ascii;
    }
    WifiCreateRadioListByTextId(win_id, ctrl_id, num, state_index, node_index, select_index);
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiCreateAPEditWapiStateSetMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    MMI_TEXT_ID_T       state_index[] = {TXT_WIFI_WEP_KEY_HEX, TXT_WIFI_WEP_KEY_ASCII};
    uint32              node_index[] = {MMIWIFI_WAPI_STATE_KEY_HEX, MMIWIFI_WAPI_STATE_KEY_ASCII};
    uint16              select_index = 0;
    uint32              num = 0;

    num = sizeof(state_index) / sizeof(state_index[0]);

    select_index = (uint16)WifiGetIsWapiAscii();
    WifiCreateRadioListByTextId(win_id, ctrl_id, num, state_index, node_index, select_index);
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiCreateAPEditWapiTypeMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    MMI_TEXT_ID_T       type_text_array[] = {TXT_COMMON_PASSWORD, TXT_WIFI_CERTIFICATE};
    uint32              node_index[] = {MMIWIFI_WAPI_TYPE_PSK, MMIWIFI_WAPI_TYPE_CER};
    uint16              select_index = 0;
    uint32              num = 0;

    num = sizeof(type_text_array) / sizeof(type_text_array[0]);

    select_index = (uint16)WifiGetWapiCredential();

    WifiCreateRadioListByTextId(win_id, ctrl_id, num, type_text_array, node_index, select_index);
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiCreateWepStateSetMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    MMI_TEXT_ID_T       wep_key_state_index[] = {TXT_WIFI_WEP_KEY_HEX,TXT_WIFI_WEP_KEY_ASCII};
    uint32              node_index[] = {MMIWIFI_WEP_STATE_KEY_HEX, MMIWIFI_WEP_STATE_KEY_ASCII};
    uint16              select_index = 0;
    uint32              num = 0;
    MMIWIFI_PROFILE_T   *profile_ptr = PNULL;

    profile_ptr = MMIWIFI_GetCurrProfile();

    num = sizeof(wep_key_state_index) / sizeof(wep_key_state_index[0]);

    if (PNULL != profile_ptr)
    {
        select_index = (uint16)profile_ptr->is_wep_ascii[profile_ptr->cur_wep_index];
    }

    WifiCreateRadioListByTextId(win_id, ctrl_id, num, wep_key_state_index, node_index, select_index);
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiCreateWepEncyptSetMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    MMI_TEXT_ID_T       wep_encrypt_index[] = {TXT_WIFI_WEP_BIT64,TXT_WIFI_WEP_BIT128};
    uint32              node_index[] = {MMIWIFI_WEP_ENCYPT_BIT64, MMIWIFI_WEP_ENCYPT_BIT128};
    uint16              select_index = 0;
    uint32              num = 0;
    MMIWIFI_PROFILE_T   *profile_ptr = PNULL;
        
    profile_ptr = MMIWIFI_GetCurrProfile();

    num = sizeof(wep_encrypt_index) / sizeof(wep_encrypt_index[0]);

    if (PNULL != profile_ptr)
    {
        select_index = (uint16)profile_ptr->config_info.credential.wep_credential.key_type;
    }
    WifiCreateRadioListByTextId(win_id, ctrl_id, num, wep_encrypt_index, node_index, select_index);
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiCreateWepModeSetMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    MMI_TEXT_ID_T       wep_key_open_index[] = {TXT_WIFI_WEP_KEY_SHARE_MODE, TXT_WIFI_WEP_KEY_OPEN_MODE};
    uint32              node_index[] = {MMIWIFI_WEP_MODE_SHARE, MMIWIFI_WEP_MODE_OPEN};
    uint16              select_index = 0;
    uint32              num = 0;
    MMIWIFI_PROFILE_T   *profile_ptr = PNULL;
        
    profile_ptr = MMIWIFI_GetCurrProfile();

    num = sizeof(wep_key_open_index) / sizeof(wep_key_open_index[0]);

    if (PNULL != profile_ptr)
    {
        select_index = profile_ptr->config_info.credential.wep_credential.is_open_mode;
    }
    WifiCreateRadioListByTextId(win_id, ctrl_id, num, wep_key_open_index, node_index, select_index);
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiCreateWepKeyIndexSetMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    MMI_TEXT_ID_T       wep_key_index[] = {TXT_WIFI_WEP_INDEX1,TXT_WIFI_WEP_INDEX2,TXT_WIFI_WEP_INDEX3,TXT_WIFI_WEP_INDEX4};
    uint32              node_index[] = {MMIWIFI_WEP_KEY_INDEX_1, MMIWIFI_WEP_KEY_INDEX_2, MMIWIFI_WEP_KEY_INDEX_3, MMIWIFI_WEP_KEY_INDEX_4};
    uint16              select_index = 0;
    uint32              num = 0;
    MMIWIFI_PROFILE_T   *profile_ptr = PNULL;
        
    profile_ptr = MMIWIFI_GetCurrProfile();

    num = sizeof(wep_key_index) / sizeof(wep_key_index[0]);

    if (PNULL != profile_ptr)
    {
        select_index = profile_ptr->cur_wep_index;
    }
    WifiCreateRadioListByTextId(win_id, ctrl_id, num, wep_key_index, node_index, select_index);
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiCreateAPEditWepStateSetMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    MMI_TEXT_ID_T       wep_key_state_index[] = {TXT_WIFI_WEP_KEY_HEX,TXT_WIFI_WEP_KEY_ASCII};
    uint32              node_index[] = {MMIWIFI_WEP_STATE_KEY_HEX, MMIWIFI_WEP_STATE_KEY_ASCII};
    uint16              select_index = 0;
    uint32              num = 0;

    num = sizeof(wep_key_state_index) / sizeof(wep_key_state_index[0]);

    select_index = (uint16)(WifiGetIsWepAscii(WifiGetWepCurrIndex()));

    WifiCreateRadioListByTextId(win_id, ctrl_id, num, wep_key_state_index, node_index, select_index);
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiCreateAPEditWepEncyptSetMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    MMI_TEXT_ID_T       wep_encrypt_index[] = {TXT_WIFI_WEP_BIT64,TXT_WIFI_WEP_BIT128};
    uint32              node_index[] = {MMIWIFI_WEP_ENCYPT_BIT64, MMIWIFI_WEP_ENCYPT_BIT128};
    uint16              select_index = 0;
    uint32              num = 0;

    num = sizeof(wep_encrypt_index) / sizeof(wep_encrypt_index[0]);

    select_index = (uint16)WifiGetWepKeyType();
    WifiCreateRadioListByTextId(win_id, ctrl_id, num, wep_encrypt_index, node_index, select_index);
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiCreateAPEditWepModeSetMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    MMI_TEXT_ID_T       wep_key_open_index[] = {TXT_WIFI_WEP_KEY_SHARE_MODE, TXT_WIFI_WEP_KEY_OPEN_MODE};
    uint32              node_index[] = {MMIWIFI_WEP_MODE_SHARE, MMIWIFI_WEP_MODE_OPEN};
    uint16              select_index = 0;
    uint32              num = 0;

    num = sizeof(wep_key_open_index) / sizeof(wep_key_open_index[0]);

    select_index = WifiGetIsWepOpen();
    WifiCreateRadioListByTextId(win_id, ctrl_id, num, wep_key_open_index, node_index, select_index);
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiCreateAPEditWepKeyIndexSetMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    MMI_TEXT_ID_T       wep_key_index[] = {TXT_WIFI_WEP_INDEX1,TXT_WIFI_WEP_INDEX2,TXT_WIFI_WEP_INDEX3,TXT_WIFI_WEP_INDEX4};
    uint32              node_index[] = {MMIWIFI_WEP_KEY_INDEX_1, MMIWIFI_WEP_KEY_INDEX_2, MMIWIFI_WEP_KEY_INDEX_3, MMIWIFI_WEP_KEY_INDEX_4};
    uint16              select_index = 0;
    uint32              num = 0;

    num = sizeof(wep_key_index) / sizeof(wep_key_index[0]);

    select_index = WifiGetWepCurrIndex();
    WifiCreateRadioListByTextId(win_id, ctrl_id, num, wep_key_index, node_index, select_index);
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiCreateRejectPopAutoMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    uint32 num = 0;
    uint16 index = 0;
    uint16 url_len = 0;
    MMI_STRING_T title_str = {0};
    wchar url_name[MMIWIFI_URL_MAX_LEN + 1] = {0};
    MMIWIFI_PROXY_REJECTLIST_E rejectlist = {0};

    MMI_TEXT_ID_T tex_index[] = {TXT_EDIT, TXT_DELETE};
    uint32 node_index[] = {MMIWIFI_REJECT_LIST_POPUP_EDIT, MMIWIFI_REJECT_LIST_POPUP_DELETE};

    MMIWIFI_GetProxyRejectList(&rejectlist);
    index = GUILIST_GetCurItemIndex(MMIWIFI_LIST_REJECT_LIST_CTRL_ID);
    url_len = strlen(rejectlist.proxy_rejectlist[index]);

    MMI_STRNTOWSTR(url_name, MMIWIFI_URL_MAX_LEN, (uint8 *)rejectlist.proxy_rejectlist[index], url_len, MIN(url_len, MMIWIFI_URL_MAX_LEN));

    title_str.wstr_ptr = url_name;
    title_str.wstr_len =  MMIAPICOM_Wstrlen(url_name);

    num = sizeof(tex_index) / sizeof(tex_index[0]);

    WifiCreatePopupAutoMenu(win_id, ctrl_id, &title_str, num, tex_index, node_index);
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiCreateAPListPopAutoMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    uint32 num = 0;
    uint16 index = 0;
    MMI_STRING_T title_str = {0};
    MMIWIFI_PROFILE_T *profile_ptr = PNULL;
    MMI_TEXT_ID_T tex_index[] = {TXT_COMMON_CONNECT_COMN, TXT_WLAN_FORGET_AP, TXT_EDIT, TXT_WIFI_DEFAULT_AP_SETTING};
    uint32 node_index[] = {MMIWIFI_AP_LIST_POPUP_CONNECT, MMIWIFI_AP_LIST_POPUP_FORGET_NETWORK, MMIWIFI_AP_LIST_POPUP_EDIT_NETWORK, MMIWIFI_AP_LIST_POPUP_DEFAULT_SETTING};
    uint16 ssid[WIFISUPP_SSID_MAX_LEN * 2 + 4] = {0};
    uint32 ssid_Len = 0;

    index = GUILIST_GetCurItemIndex(MMIWIFI_LIST_CTRL_ID);

    if (!WifiListIsAPItemSelected(index))
    {
        return;
    }    

    profile_ptr = (MMIWIFI_PROFILE_T *)WifiGetListUserdata(MMIWIFI_LIST_CTRL_ID, index);

    if (PNULL == profile_ptr)
    {
        return;
    }

    ssid_Len = GUI_GBToWstr(ssid, profile_ptr->config_info.ssid.ssid, MIN(profile_ptr->config_info.ssid.ssid_len, WIFISUPP_SSID_MAX_LEN));

    title_str.wstr_len = (uint16)ssid_Len;
    title_str.wstr_ptr = (wchar *)ssid;

    if ((MMIWIFI_PROFILE_STATE_CONNECTED == profile_ptr->state) ||
        (MMIWIFI_PROFILE_STATE_DHCPED == profile_ptr->state))
    {
        tex_index[0] = TXT_COMM_DISCONNECT;
    }

    num = sizeof(tex_index) / sizeof(tex_index[0]);

#ifdef WLAN_PORTAL_SUPPORT
    if (!MMIWIFI_APIsCMCC(profile_ptr))
    {
        num -= 1; /* No "DEFAULT SETTING" */

        if (profile_ptr->is_trusted)
        {
            if (!(profile_ptr->state == MMIWIFI_PROFILE_STATE_DISCONNECTED || profile_ptr->state == MMIWIFI_PROFILE_STATE_DISCONNECTING))
            {
                num -= 1; /* No EDIT */
            }
        }
        else
        {
            num -= 2; /* No "FORGET && EDIT" */
        }
    }
    else
    {
        tex_index[1] = tex_index[3];
        node_index[1] = node_index[3];
        num -= 2;  /* No "FORGET && EDIT" */
    }
#else
    num -= 1;

    if (profile_ptr->is_trusted)
    {
        if (!(profile_ptr->state == MMIWIFI_PROFILE_STATE_DISCONNECTED || profile_ptr->state == MMIWIFI_PROFILE_STATE_DISCONNECTING))
        {
            num -= 1;
        }
    }
    else
    {
        num -= 2;
    }
#endif

    WifiCreatePopupAutoMenu(win_id, ctrl_id, &title_str, num, tex_index, node_index);
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiCreateAPTrustListPopAutoMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    uint32 num = 0;
    uint16 index = 0;
    MMI_STRING_T title_str = {0};
    MMIWIFI_PROFILE_T *profile_ptr = PNULL;

    MMI_TEXT_ID_T tex_index[] = {TXT_WIFI_CHANGE_PRIORITY_HIGH, TXT_WIFI_CHANGE_PRIORITY_LOW, TXT_EDIT,TXT_DELETE};
    uint32 node_index[] = {MMIWLAN_TRUST_OPT_ID_CHANGE_PRIORITY_HIGH, MMIWLAN_TRUST_OPT_ID_CHANGE_PRIORITY_LOW, MMIWLAN_TRUST_OPT_ID_EDIT, MMIWLAN_TRUST_OPT_ID_DELETE};
    uint16 ssid[WIFISUPP_SSID_MAX_LEN * 2 + 4] = {0};
    uint32 ssid_Len = 0;

    index = GUILIST_GetCurItemIndex(MMIWIFI_LIST_TRUSTED_CTRL_ID);
    profile_ptr = (MMIWIFI_PROFILE_T *)WifiGetListUserdata(MMIWIFI_LIST_TRUSTED_CTRL_ID, index);

    if (PNULL == profile_ptr)
    {
        return;
    }

    ssid_Len = GUI_GBToWstr(ssid, profile_ptr->config_info.ssid.ssid, MIN(profile_ptr->config_info.ssid.ssid_len, WIFISUPP_SSID_MAX_LEN));

    title_str.wstr_len = (uint16)ssid_Len;
    title_str.wstr_ptr = (wchar *)ssid;

    num = sizeof(tex_index) / sizeof(tex_index[0]);

    if (MMIWIFI_APIsCMCC(profile_ptr) || MMIWIFI_APIsCMCCEDU(profile_ptr))
    {
        num -= 2;
    }

    WifiCreatePopupAutoMenu(win_id, ctrl_id, &title_str, num, tex_index, node_index);
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleSearchWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"WifiHandleSearchWinMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_9610_112_2_18_3_10_12_214,(uint8*)"d", msg_id);

    switch (msg_id) {
    case MSG_OPEN_WINDOW:
        WifiCreateSearchMenu(win_id, MMIWIFI_MENU_SEARCH_CTRL_ID);
        MMK_SetAtvCtrl(win_id, MMIWIFI_MENU_SEARCH_CTRL_ID);
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

            select_index = GUILIST_GetCurItemIndex(MMIWIFI_MENU_SEARCH_CTRL_ID);

            if(select_index != MMIWIFI_GetWlanAutoScanStatus())
            {
                MMIWIFI_SetWlanAutoScanStatus(select_index);
                if(MMIWIFI_GetIsWlanOn())
                {
                    if (0 != select_index)
                    {
                        MMIWIFI_StartAutoScan();
                    }
                    else
                    {
                        MMIWIFI_StopAutoScan();
                    }
                }
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

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleWpaStateSetWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"WifiHandleWpaStateSetWinMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_9669_112_2_18_3_10_13_215,(uint8*)"d", msg_id);

    switch (msg_id) {
    case MSG_OPEN_WINDOW:
        WifiCreateWpaStateSetMenu(win_id, MMIWIFI_MENU_WPA_STATE_CTRL_ID);
        MMK_SetAtvCtrl(win_id, MMIWIFI_MENU_WPA_STATE_CTRL_ID);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        {
            MMIWIFI_PROFILE_T   *profile_ptr = PNULL;
            BOOLEAN             is_ascii = TRUE;

            profile_ptr = MMIWIFI_GetCurrProfile();
            if (PNULL != profile_ptr)
            {
                is_ascii = (BOOLEAN)GUILIST_GetCurItemIndex(MMIWIFI_MENU_WPA_STATE_CTRL_ID);

                if (profile_ptr->is_wpa_ascii != is_ascii)
                {
                    profile_ptr->is_wpa_ascii = is_ascii;

                    GUIEDIT_ClearAllStr(MMIWIFI_EDIT_WPA_WPA2_PRESHAREKEY_EDIT_CTRL_ID);
                }
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

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleAPEditWpaStateSetWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"WifiHandleAPEditWpaStateSetWinMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_9723_112_2_18_3_10_13_216,(uint8*)"d", msg_id);

    switch (msg_id) {
    case MSG_OPEN_WINDOW:
        WifiCreateAPEditWpaStateSetMenu(win_id, MMIWIFI_MENU_WPA_STATE_CTRL_ID);
        MMK_SetAtvCtrl(win_id, MMIWIFI_MENU_WPA_STATE_CTRL_ID);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        {
            BOOLEAN             is_ascii = TRUE;

            is_ascii = (BOOLEAN)GUILIST_GetCurItemIndex(MMIWIFI_MENU_WPA_STATE_CTRL_ID);
            if (WifiGetIsWpaAscii() != is_ascii)
            {
                WifiSetIsWpaAscii(is_ascii);
                GUIEDIT_ClearAllStr(MMIWIFI_EDIT_WPA_WPA2_PRESHAREKEY_EDIT_CTRL_ID);
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

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleAPNetworkStateWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"WifiHandleAPNetworkStateWinMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_9771_112_2_18_3_10_13_217,(uint8*)"d", msg_id);

    switch (msg_id) {
    case MSG_OPEN_WINDOW:
        WifiCreateAPNetworkStateMenu(win_id, MMIWIFI_MENU_AP_NETWORK_STATE_CTRL_ID);
        MMK_SetAtvCtrl(win_id, MMIWIFI_MENU_AP_NETWORK_STATE_CTRL_ID);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        {
            BOOLEAN             is_hide = TRUE;

            is_hide = (BOOLEAN)GUILIST_GetCurItemIndex(MMIWIFI_MENU_AP_NETWORK_STATE_CTRL_ID);

            s_ui_context.s_mmiwifi_profile.is_ssid_hide = is_hide;

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

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleAPAuthTypeWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"WifiHandleAPAuthTypeWinMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_9816_112_2_18_3_10_13_218,(uint8*)"d", msg_id);

    switch (msg_id) {
    case MSG_OPEN_WINDOW:
        WifiCreateAPAuthTypeMenu(win_id, MMIWIFI_MENU_AP_AUTH_TYPE_CTRL_ID);
        MMK_SetAtvCtrl(win_id, MMIWIFI_MENU_AP_AUTH_TYPE_CTRL_ID);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        {
            uint16              auth_type = 0;

            auth_type = GUILIST_GetCurItemIndex(MMIWIFI_MENU_AP_AUTH_TYPE_CTRL_ID);
            s_ui_context.s_mmiwifi_profile.config_info.encryp_protocol = (WIFISUPP_ENCRYP_PROTOCOL_E)auth_type;

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

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleAPConnectStatetWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"WifiHandleAPConnectStatetWinMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_9860_112_2_18_3_10_13_219,(uint8*)"d", msg_id);

    switch (msg_id) {
    case MSG_OPEN_WINDOW:
        WifiCreateAPConnectStateMenu(win_id, MMIWIFI_MENU_AP_CONNECT_STATE_CTRL_ID);
        MMK_SetAtvCtrl(win_id, MMIWIFI_MENU_AP_CONNECT_STATE_CTRL_ID);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        {
            BOOLEAN             is_auto_connect = FALSE;

            is_auto_connect = (BOOLEAN)GUILIST_GetCurItemIndex(MMIWIFI_MENU_AP_CONNECT_STATE_CTRL_ID);
            s_ui_context.s_mmiwifi_profile.is_auto_connect = is_auto_connect;

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

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleWapiStateSetWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"WifiHandleWapiStateSetWinMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_9904_112_2_18_3_10_13_220,(uint8*)"d", msg_id);

    switch (msg_id) {
    case MSG_OPEN_WINDOW:
        WifiCreateWapiStateSetMenu(win_id, MMIWIFI_MENU_WAPI_STATE_CTRL_ID);
        MMK_SetAtvCtrl(win_id, MMIWIFI_MENU_WAPI_STATE_CTRL_ID);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        {
            MMIWIFI_PROFILE_T   *profile_ptr = PNULL;
            BOOLEAN             is_ascii = TRUE;

            profile_ptr = MMIWIFI_GetCurrProfile();
            if (PNULL != profile_ptr)
            {
                is_ascii = (BOOLEAN)GUILIST_GetCurItemIndex(MMIWIFI_MENU_WAPI_STATE_CTRL_ID);
                if (profile_ptr->is_wapi_ascii != is_ascii)
                {
                    profile_ptr->is_wapi_ascii = is_ascii;

                    GUIEDIT_ClearAllStr(MMIWIFI_EDIT_WAPI_PRESHAREKEY_EDIT_CTRL_ID);
                }
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

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleAPEditWapiStateSetWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"WifiHandleAPEditWapiStateSetWinMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_9957_112_2_18_3_10_13_221,(uint8*)"d", msg_id);

    switch (msg_id) {
    case MSG_OPEN_WINDOW:
        WifiCreateAPEditWapiStateSetMenu(win_id, MMIWIFI_MENU_WAPI_STATE_CTRL_ID);
        MMK_SetAtvCtrl(win_id, MMIWIFI_MENU_WAPI_STATE_CTRL_ID);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        {
            BOOLEAN             is_ascii = TRUE;

            is_ascii = (BOOLEAN)GUILIST_GetCurItemIndex(MMIWIFI_MENU_WAPI_STATE_CTRL_ID);
            if (WifiGetIsWapiAscii() != is_ascii)
            {
                WifiSetIsWapiAscii(is_ascii);
                GUIEDIT_ClearAllStr(MMIWIFI_EDIT_WAPI_PRESHAREKEY_EDIT_CTRL_ID);
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

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleAPEditWapiTypeWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"WifiHandleAPEditWapiTypeWinMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_10004_112_2_18_3_10_13_222,(uint8*)"d", msg_id);

    switch (msg_id) {
    case MSG_OPEN_WINDOW:
        WifiCreateAPEditWapiTypeMenu(win_id, MMIWIFI_MENU_WAPI_STATE_CTRL_ID);
        MMK_SetAtvCtrl(win_id, MMIWIFI_MENU_WAPI_STATE_CTRL_ID);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        {
            WIFISUPP_WAPI_CREDENTIAL_TYPE_E     type = WIFISUPP_WAPI_CREDENTIAL_TYPE_PSK;

            type = (WIFISUPP_WAPI_CREDENTIAL_TYPE_E)GUILIST_GetCurItemIndex(MMIWIFI_MENU_WAPI_STATE_CTRL_ID);

            if (WifiGetWapiCredential() != type)
            {
                WifiSetWapiCredential(type);
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

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleWepStateSetWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"WifiHandleWepStateSetWinMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_10051_112_2_18_3_10_13_223,(uint8*)"d", msg_id);

    switch (msg_id) {
    case MSG_OPEN_WINDOW:
        WifiCreateWepStateSetMenu(win_id, MMIWIFI_MENU_WEP_STATE_CTRL_ID);
        MMK_SetAtvCtrl(win_id, MMIWIFI_MENU_WEP_STATE_CTRL_ID);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        {
            MMIWIFI_PROFILE_T   *profile_ptr = PNULL;
            BOOLEAN             is_ascii = TRUE;

            profile_ptr = MMIWIFI_GetCurrProfile();

            if (PNULL != profile_ptr)
            {
                is_ascii = (BOOLEAN)GUILIST_GetCurItemIndex(MMIWIFI_MENU_WEP_STATE_CTRL_ID);

                if (profile_ptr->is_wep_ascii[profile_ptr->cur_wep_index] != is_ascii)
                {
                    profile_ptr->is_wep_ascii[profile_ptr->cur_wep_index] = is_ascii;

                    GUIEDIT_ClearAllStr(MMIWIFI_EDIT_WEP_KEY_EDIT_CTRL_ID);
                }
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

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleWepEncyptSetWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"WifiHandleWepEncyptSetWinMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_10106_112_2_18_3_10_13_224,(uint8*)"d", msg_id);

    switch (msg_id) {
    case MSG_OPEN_WINDOW:
        WifiCreateWepEncyptSetMenu(win_id, MMIWIFI_MENU_WEP_ENCYPT_CTRL_ID);
        MMK_SetAtvCtrl(win_id, MMIWIFI_MENU_WEP_ENCYPT_CTRL_ID);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        {
            MMIWIFI_PROFILE_T   *profile_ptr = PNULL;
            WIFISUPP_WEP_KEY_TYPE_E              key_type = 0;

            profile_ptr = MMIWIFI_GetCurrProfile();
            if (PNULL != profile_ptr)
            {
                key_type = (WIFISUPP_WEP_KEY_TYPE_E)GUILIST_GetCurItemIndex(MMIWIFI_MENU_WEP_ENCYPT_CTRL_ID);

                if (profile_ptr->config_info.credential.wep_credential.key_type != key_type)
                {
                    profile_ptr->config_info.credential.wep_credential.key_type = key_type;
                    GUIEDIT_ClearAllStr(MMIWIFI_EDIT_WEP_KEY_EDIT_CTRL_ID);
                }
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

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleWepModeSetWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"WifiHandleWepModeSetWinMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_10160_112_2_18_3_10_13_225,(uint8*)"d", msg_id);

    switch (msg_id) {
    case MSG_OPEN_WINDOW:
        WifiCreateWepModeSetMenu(win_id, MMIWIFI_MENU_WEP_MODE_CTRL_ID);
        MMK_SetAtvCtrl(win_id, MMIWIFI_MENU_WEP_MODE_CTRL_ID);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        {
            MMIWIFI_PROFILE_T   *profile_ptr = PNULL;
            uint16              open_mode = 0;

            profile_ptr = MMIWIFI_GetCurrProfile();
            if (PNULL != profile_ptr)
            {                open_mode = GUILIST_GetCurItemIndex(MMIWIFI_MENU_WEP_MODE_CTRL_ID);

                if (profile_ptr->config_info.credential.wep_credential.is_open_mode != open_mode)
                {
                    profile_ptr->config_info.credential.wep_credential.is_open_mode = open_mode;
                }
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

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleWepKeyIndexSetWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"WifiHandleWepKeyIndexSetWinMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_10212_112_2_18_3_10_14_226,(uint8*)"d", msg_id);

    switch (msg_id) {
    case MSG_OPEN_WINDOW:
        WifiCreateWepKeyIndexSetMenu(win_id, MMIWIFI_MENU_WEP_KEY_INDEX_CTRL_ID);
        MMK_SetAtvCtrl(win_id, MMIWIFI_MENU_WEP_KEY_INDEX_CTRL_ID);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        {
            MMIWIFI_PROFILE_T   *profile_ptr = PNULL;
            uint16              index = 0;

            profile_ptr = MMIWIFI_GetCurrProfile();
            if (PNULL != profile_ptr)
            {
                index = GUILIST_GetCurItemIndex(MMIWIFI_MENU_WEP_KEY_INDEX_CTRL_ID);

                if (profile_ptr->cur_wep_index != index)
                {
                    profile_ptr->cur_wep_index = index;
                    GUIEDIT_ClearAllStr(MMIWIFI_EDIT_WEP_KEY_EDIT_CTRL_ID);
                }
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

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleAPEditWepStateSetWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"WifiHandleWepStateSetWinMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_10265_112_2_18_3_10_14_227,(uint8*)"d", msg_id);

    switch (msg_id) {
    case MSG_OPEN_WINDOW:
        WifiCreateAPEditWepStateSetMenu(win_id, MMIWIFI_MENU_WEP_STATE_CTRL_ID);
        MMK_SetAtvCtrl(win_id, MMIWIFI_MENU_WEP_STATE_CTRL_ID);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        {
            BOOLEAN             is_ascii = TRUE;
            uint16              index = 0;

            index = WifiGetWepCurrIndex();
            is_ascii = (BOOLEAN)GUILIST_GetCurItemIndex(MMIWIFI_MENU_WEP_STATE_CTRL_ID);

            if (WifiGetIsWepAscii(index) != is_ascii)
            {
                WifiSetIsWepAscii(index, is_ascii);
                GUIEDIT_ClearAllStr(MMIWIFI_EDIT_WEP_KEY_EDIT_CTRL_ID);
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

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleAPEditWepEncyptSetWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"WifiHandleWepEncyptSetWinMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_10316_112_2_18_3_10_14_228,(uint8*)"d", msg_id);

    switch (msg_id) {
    case MSG_OPEN_WINDOW:
        WifiCreateAPEditWepEncyptSetMenu(win_id, MMIWIFI_MENU_WEP_ENCYPT_CTRL_ID);
        MMK_SetAtvCtrl(win_id, MMIWIFI_MENU_WEP_ENCYPT_CTRL_ID);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        {
            WIFISUPP_WEP_KEY_TYPE_E              key_type = 0;

            key_type = (WIFISUPP_WEP_KEY_TYPE_E)GUILIST_GetCurItemIndex(MMIWIFI_MENU_WEP_ENCYPT_CTRL_ID);

            if (WifiGetWepKeyType() != key_type)
            {
                WifiSetWepKeyType(key_type);
                GUIEDIT_ClearAllStr(MMIWIFI_EDIT_WEP_KEY_EDIT_CTRL_ID);
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

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleAPEditWepModeSetWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"WifiHandleWepModeSetWinMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_10365_112_2_18_3_10_14_229,(uint8*)"d", msg_id);

    switch (msg_id) {
    case MSG_OPEN_WINDOW:
        WifiCreateAPEditWepModeSetMenu(win_id, MMIWIFI_MENU_WEP_MODE_CTRL_ID);
        MMK_SetAtvCtrl(win_id, MMIWIFI_MENU_WEP_MODE_CTRL_ID);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        {
            uint16              open_mode = 0;

            open_mode = GUILIST_GetCurItemIndex(MMIWIFI_MENU_WEP_MODE_CTRL_ID);

            if (WifiGetIsWepOpen() != open_mode)
            {
                WifiSetWepOpen(open_mode);
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

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleAPEditWepKeyIndexSetWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"WifiHandleWepKeyIndexSetWinMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_10413_112_2_18_3_10_14_230,(uint8*)"d", msg_id);

    switch (msg_id) {
    case MSG_OPEN_WINDOW:
        WifiCreateAPEditWepKeyIndexSetMenu(win_id, MMIWIFI_MENU_WEP_KEY_INDEX_CTRL_ID);
        MMK_SetAtvCtrl(win_id, MMIWIFI_MENU_WEP_KEY_INDEX_CTRL_ID);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        {
            uint16              index = 0;

            index = GUILIST_GetCurItemIndex(MMIWIFI_MENU_WEP_KEY_INDEX_CTRL_ID);

            if (WifiGetWepCurrIndex() != index)
            {
                WifiSetWepCurrIndex(index);
                GUIEDIT_ClearAllStr(MMIWIFI_EDIT_WEP_KEY_EDIT_CTRL_ID);
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

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleRejectListPopWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"WifiHandleRejectListPopWinMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_10462_112_2_18_3_10_14_231,(uint8*)"d", msg_id);

    switch (msg_id) {
    case MSG_OPEN_WINDOW:
        WifiCreateRejectPopAutoMenu(win_id, MMIWIFI_MENU_REJECT_POPUPAUTO_ID);
        MMK_SetAtvCtrl(win_id, MMIWIFI_MENU_REJECT_POPUPAUTO_ID);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        {
            MMI_MENU_ID_T menu_id = 0;

            menu_id = GUIMENU_GetCurNodeId(MMIWIFI_MENU_REJECT_POPUPAUTO_ID);

            switch (menu_id)
            {
            case MMIWIFI_REJECT_LIST_POPUP_EDIT:
                MMK_CreateWin((uint32 *)MMIWIFI_REJECTLIST_EDIT_WIN_TAB, PNULL);//编辑窗口
                break;

            case MMIWIFI_REJECT_LIST_POPUP_DELETE:
                {
                    MMIWIFI_PROXY_REJECTLIST_E rejectlist = {0};
                    uint16 index = 0;
            
                    MMIPUB_CloseQuerytWin(PNULL);
                    MMIWIFI_GetProxyRejectList(&rejectlist);

                    index = GUILIST_GetCurItemIndex(MMIWIFI_LIST_REJECT_LIST_CTRL_ID);
  
                    //删除当前项，将后面项依次向前一项复制
                    SCI_MEMSET(rejectlist.proxy_rejectlist[index], 0, sizeof(rejectlist.proxy_rejectlist[index]));
                    index++;
                    for(;index<MMIWIFI_MAX_REJECTLIST_NUM;index++)
                    {
                        uint16 proxy_rejectlist_len = strlen(rejectlist.proxy_rejectlist[index]);

                        if(0 != proxy_rejectlist_len)
                        {
                            SCI_MEMCPY(rejectlist.proxy_rejectlist[index-1], rejectlist.proxy_rejectlist[index], proxy_rejectlist_len);
                            SCI_MEMSET(rejectlist.proxy_rejectlist[index], 0, sizeof(rejectlist.proxy_rejectlist[index]));
                        }
                    }

                    MMIWIFI_SetProxyRejectList(&rejectlist);
                }

                break;
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
LOCAL MMI_RESULT_E WifiHandleApListPopWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"WifiHandleApListPopWinMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_10543_112_2_18_3_10_14_232,(uint8*)"d", msg_id);

    switch (msg_id) {

    case MSG_OPEN_WINDOW:
        WifiCreateAPListPopAutoMenu(win_id, MMIWIFI_MENU_AP_LIST_POPUPAUTO_ID);
        MMK_SetAtvCtrl(win_id, MMIWIFI_MENU_AP_LIST_POPUPAUTO_ID);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        {
            MMI_MENU_ID_T menu_id = 0;

            menu_id = GUIMENU_GetCurNodeId(MMIWIFI_MENU_AP_LIST_POPUPAUTO_ID);

            //SCI_TRACE_LOW:"MMIWIFI WifiHandleApListPopWinMsg menu_id=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_10562_112_2_18_3_10_14_233,(uint8*)"d", menu_id);

            switch (menu_id)
            {
            case MMIWIFI_AP_LIST_POPUP_CONNECT:
                {
                    uint16 index = 0;
                    MMIWIFI_PROFILE_T * profile_ptr = PNULL;

                    index = GUILIST_GetCurItemIndex(MMIWIFI_LIST_CTRL_ID);

                    if (!WifiListIsAPItemSelected(index))
                    {
                        break;
                    }

                    profile_ptr = (MMIWIFI_PROFILE_T *)WifiGetListUserdata(MMIWIFI_LIST_CTRL_ID, index);

                    if (PNULL == profile_ptr)
                    {
                        break;
                    }

                    if ((MMIWIFI_PROFILE_STATE_CONNECTED == profile_ptr->state) ||
                        (MMIWIFI_PROFILE_STATE_DHCPED    == profile_ptr->state))
                    {
                        MMIWIFI_DisConnect(profile_ptr);
                    }
                    else 
                    {
                        if (MMIWIFI_GetScanStatus())
                        {
                            //SCI_TRACE_LOW:"MMIWIFI_Connect Scaning"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_10594_112_2_18_3_10_14_234,(uint8*)"");
                            MMIPUB_OpenAlertWarningWin(TXT_WIFI_CONNECT_BUSY);
                            break;
                        }

                        if (MMIWIFI_Connect(profile_ptr))
                        {
                            MMIWIFI_OpenConnectWaitWin();
                        }
                    }
                }
                break;

            case MMIWIFI_AP_LIST_POPUP_FORGET_NETWORK:
                {
                    uint16 index = 0;
                    MMIWIFI_PROFILE_T * profile_ptr = PNULL;

                    index = GUILIST_GetCurItemIndex(MMIWIFI_LIST_CTRL_ID);

                    if (!WifiListIsAPItemSelected(index))
                    {
                        break;
                    }

                    profile_ptr = (MMIWIFI_PROFILE_T *)WifiGetListUserdata(MMIWIFI_LIST_CTRL_ID, index);

                    if (PNULL != profile_ptr && !(profile_ptr->state == MMIWIFI_PROFILE_STATE_DISCONNECTED || profile_ptr->state == MMIWIFI_PROFILE_STATE_DISCONNECTING))
                    {
                        MMIPUB_OpenQueryWinByTextIdEx(MMK_GetFirstAppletHandle(),TXT_WLAN_IS_DELETE_CONNECTED, IMAGE_PUBWIN_QUERY, PNULL, WifiHandleQueryForgetAP, (uint32)profile_ptr);
                        break;
                    }

                    if(PNULL != profile_ptr && profile_ptr ->is_on_air)//如所删AP是当前搜索结果，不删除AP，将其is_trusted标志置否
                    {
                        profile_ptr ->is_trusted = FALSE;
                        MMIWIFI_UpdateProfileFile();
                    }
                    else
                    {
                        if (MMIWIFI_DeleteProfileByAp(profile_ptr))
                        {
                            MMIWIFI_UpdateProfileFile();
                        }
                    }
                }
                break;

            case MMIWIFI_AP_LIST_POPUP_EDIT_NETWORK:
                {
                    uint16 index = 0;
                    MMIWIFI_PROFILE_T * profile_ptr = PNULL;

                    index = GUILIST_GetCurItemIndex(MMIWIFI_LIST_CTRL_ID);

                    if (!WifiListIsAPItemSelected(index))
                    {
                        break;
                    }

                    profile_ptr = (MMIWIFI_PROFILE_T *)WifiGetListUserdata(MMIWIFI_LIST_CTRL_ID, index);

                    if (PNULL != profile_ptr)
                    {
                        if (profile_ptr->state == MMIWIFI_PROFILE_STATE_DISCONNECTED || profile_ptr->state == MMIWIFI_PROFILE_STATE_DISCONNECTING)
                        {
                            MMIWIFI_OpenEditWin(profile_ptr);
                        }
                        else
                        {
                            MMIPUB_OpenQueryWinByTextIdEx(MMK_GetFirstAppletHandle(),TXT_WLAN_IS_DELETE_CONNECTED, IMAGE_PUBWIN_QUERY, PNULL, WifiHandleQueryEditAP, (uint32)profile_ptr);
                        }
                    }
                }    
                break;

            case MMIWIFI_AP_LIST_POPUP_DEFAULT_SETTING:
                {
                    MMIWIFI_OpenPortalLoginWin();
                }

                break;
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
LOCAL MMI_RESULT_E WifiHandleApTrustListPopWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"WifiHandleApTrustListPopWinMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_10705_112_2_18_3_10_15_235,(uint8*)"d", msg_id);

    switch(msg_id) 
    {
        case MSG_OPEN_WINDOW:
            WifiCreateAPTrustListPopAutoMenu(win_id, MMIWIFI_MENU_AP_TRUST_LIST_POPUPAUTO_ID);
            MMK_SetAtvCtrl(win_id, MMIWIFI_MENU_AP_TRUST_LIST_POPUPAUTO_ID);
            break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
        case MSG_CTL_OK:
        case MSG_KEYDOWN_WEB:
            {
                uint16                  index = 0;
                MMIWIFI_PROFILE_T*      profile_ptr = PNULL;
                
                index = GUILIST_GetCurItemIndex(MMIWIFI_LIST_TRUSTED_CTRL_ID);
                profile_ptr = (MMIWIFI_PROFILE_T *)WifiGetListUserdata(MMIWIFI_LIST_TRUSTED_CTRL_ID, index);

                switch ( GUIMENU_GetCurNodeId(MMIWIFI_MENU_AP_TRUST_LIST_POPUPAUTO_ID))
                {
                case MMIWLAN_TRUST_OPT_ID_EDIT://编辑，信任列表中
                    if (PNULL != profile_ptr)
                    {
                        if (profile_ptr->state == MMIWIFI_PROFILE_STATE_DISCONNECTED || profile_ptr->state == MMIWIFI_PROFILE_STATE_DISCONNECTING)
                        {
                            MMIWIFI_OpenEditWin(profile_ptr);
                            MMK_CloseWin(win_id);
                        }
                        else
                        {
                            MMIPUB_OpenQueryWinByTextId(TXT_WLAN_IS_DELETE_CONNECTED,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
                        }
                    }
                    break;

                case MMIWLAN_TRUST_OPT_ID_ADD://新建，信任列表中
                    {
                       if(MMIWIFI_MAX_TRUSTED_LIST_NUM > MMIWIFI_GetTrustedProfileNum())//MMIWIFI_MAX_TRUSTED_LIST_NUM
                        {
                            MMIWIFI_OpenNewWin();
                        }
                        else
                        {
                            MMIPUB_OpenAlertWarningWin(TXT_WIFI_TRUSTED_LIST_FULL); 
                        }
                    }
                    MMK_CloseWin(win_id);
                    break;

                case MMIWLAN_TRUST_OPT_ID_DELETE://删除，信任列表中
                    //SCI_TRACE_LOW:"MMIWIFI WifiHandlelistPopMenuWinMsg MMIWIFI_POPMENU_ID_DELETE index=%d,profile_ptr=%d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_10757_112_2_18_3_10_15_236,(uint8*)"dd",index, profile_ptr);
                    if (PNULL != profile_ptr && (profile_ptr->state == MMIWIFI_PROFILE_STATE_DISCONNECTED || profile_ptr->state == MMIWIFI_PROFILE_STATE_DISCONNECTING))
                    {
                        MMIPUB_OpenQueryWinByTextId(TXT_WIFI_IS_DELETE,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
                    }
                    else
                    {
                        MMIPUB_OpenQueryWinByTextId(TXT_WLAN_IS_DELETE_CONNECTED,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
                    }
                    break;

                case MMIWLAN_TRUST_OPT_ID_CHANGE_PRIORITY_HIGH://优先级上移，信任列表中
                    {
                        MMIWIFI_PROFILE_T *profile_ptr1 = PNULL;
                        profile_ptr1 = (MMIWIFI_PROFILE_T *)WifiGetListUserdata(MMIWIFI_LIST_TRUSTED_CTRL_ID, index -1);

                        if(MMIWIFI_SwapProfile(profile_ptr, profile_ptr1))
                        {
                            MMIWIFI_UpdateProfileFile();
                        }
                        GUILIST_SetCurItemIndex(MMIWIFI_LIST_TRUSTED_CTRL_ID, index - 1);
                        MMK_CloseWin(win_id);
                    }
                    break;

                case MMIWLAN_TRUST_OPT_ID_CHANGE_PRIORITY_LOW://优先级下移，信任列表中
                    {
                        MMIWIFI_PROFILE_T *profile_ptr1 = PNULL;
                        profile_ptr1 = (MMIWIFI_PROFILE_T *)WifiGetListUserdata(MMIWIFI_LIST_TRUSTED_CTRL_ID, index + 1);

                        if(MMIWIFI_SwapProfile(profile_ptr, profile_ptr1))
                        {
                            MMIWIFI_UpdateProfileFile();
                        }
                        GUILIST_SetCurItemIndex(MMIWIFI_LIST_TRUSTED_CTRL_ID, index + 1);
                        MMK_CloseWin(win_id);
                    }
                    break;
                
                default:
                    break;
                }
            }
            break;
            
        case MSG_PROMPTWIN_OK:
            {
                uint16 index = 0;
                MMIWIFI_PROFILE_T*      profile_ptr = PNULL;
                
                MMIPUB_CloseQuerytWin(PNULL);
                index = GUILIST_GetCurItemIndex(MMIWIFI_LIST_TRUSTED_CTRL_ID);
                profile_ptr = (MMIWIFI_PROFILE_T *)WifiGetListUserdata(MMIWIFI_LIST_TRUSTED_CTRL_ID, index);

                switch ( GUIMENU_GetCurNodeId(MMIWIFI_MENU_AP_TRUST_LIST_POPUPAUTO_ID))
                {
                case MMIWLAN_TRUST_OPT_ID_EDIT:
                    if (PNULL != profile_ptr && (profile_ptr->state != MMIWIFI_PROFILE_STATE_DISCONNECTED && profile_ptr->state != MMIWIFI_PROFILE_STATE_DISCONNECTING))
                    {
                        MMIWIFI_DisConnect(profile_ptr);
                    }
                    MMIWIFI_OpenEditWin(profile_ptr);
                    break;
                    
                case MMIWLAN_TRUST_OPT_ID_DELETE:
                    if (PNULL != profile_ptr && (profile_ptr->state != MMIWIFI_PROFILE_STATE_DISCONNECTED && profile_ptr->state != MMIWIFI_PROFILE_STATE_DISCONNECTING))
                    {
                        MMIWIFI_DisConnect(profile_ptr);
                    }
                    if(PNULL != profile_ptr && profile_ptr ->is_on_air)//如所删AP是当前搜索结果，不删除AP，将其is_trusted标志置否
                    {
                        profile_ptr ->is_trusted = FALSE;
                        MMIWIFI_UpdateProfileFile();
#ifdef MMI_GUI_STYLE_TYPICAL
                        MMIPUB_OpenAlertWinByTextId(PNULL, TXT_COMMON_DELETESUCESS, 0,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
#endif
                    }
                    else
                    {
                        if (MMIWIFI_DeleteProfileByAp(profile_ptr))
                        {
                            MMIWIFI_UpdateProfileFile();
#ifdef MMI_GUI_STYLE_TYPICAL
                            MMIPUB_OpenAlertWinByTextId(PNULL, TXT_WIFI_DELETE_SUCC, 0,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
#endif                       
                        }
                        else
                        {
                            MMIPUB_OpenAlertWarningWin(TXT_COMMON_DEL_FAIL);
                        }
                    }
                    break;
                    
                default:
                    break;
                }
                MMK_CloseWin(win_id);
            }
            break;
    
        case MSG_PROMPTWIN_CANCEL:
            MMIPUB_CloseQuerytWin(PNULL);
            MMK_CloseWin(win_id);
            break;
            
        case MSG_CTL_CANCEL:
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
LOCAL void WifiSetApEditWAPIWin(void)
{
    MMI_STRING_T                        mmi_string = {0};
    MMI_TEXT_ID_T                       type_text_array[] = {TXT_COMMON_PASSWORD, TXT_WIFI_CERTIFICATE};
#ifdef MMI_WLAN_AUTHORITY_COMPLEX
    MMI_TEXT_ID_T                       wpa_wpa2_presharekey_state_index[] = {TXT_WIFI_WEP_KEY_HEX,TXT_WIFI_WEP_KEY_ASCII};
#endif /* MMI_WLAN_AUTHORITY_COMPLEX */
    WIFISUPP_WAPI_CREDENTIAL_TYPE_E     type = WIFISUPP_WAPI_CREDENTIAL_TYPE_PSK;
    BOOLEAN                             is_wapi_ascii = TRUE;
    uint16 current_index = 0;
    uint16 top_index = 0;

    //SCI_TRACE_LOW:"WifiSetApEditWAPIWin called"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_10888_112_2_18_3_10_15_237,(uint8*)"");
    
    type = WifiGetWapiCredential();
    is_wapi_ascii = WifiGetIsWapiAscii();

    GUIFORM_SetType(MMIWIFI_FORM_AP_EDIT_WAPI_ID, GUIFORM_TYPE_TP);
//    GUISETLIST_SetFontAlign(MMIWLAN_LIST_WAPI_STATE_CTRL_ID, ALIGN_LVMIDDLE);

    GUIFORM_SetStyle(MMIWIFI_CHILD_FORM_CLIENT_CTRL_ID, GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIWIFI_CHILD_FORM_PARENT_CTRL_ID, GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIWIFI_CHILD_FORM_UNINSTALL_CTRL_ID, GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIWIFI_CHILD_FORM_WAPI_PASSWORD_CTRL_ID, GUIFORM_STYLE_UNIT);

    current_index = GUILIST_GetCurItemIndex(MMIWLAN_LIST_WAPI_STATE_CTRL_ID);
    top_index = GUILIST_GetTopItemIndex(MMIWLAN_LIST_WAPI_STATE_CTRL_ID);
    GUILIST_SetMaxItem(MMIWLAN_LIST_WAPI_STATE_CTRL_ID, 10, FALSE);
    GUILIST_RemoveAllItems(MMIWLAN_LIST_WAPI_STATE_CTRL_ID);
    GUILIST_SetSlideState(MMIWLAN_LIST_WAPI_STATE_CTRL_ID, FALSE);

    GUIEDIT_SetAlign(MMIWIFI_EDIT_WAPI_PRESHAREKEY_EDIT_CTRL_ID, ALIGN_LVMIDDLE);

    /* TYPE*/
    AppendTwoTextOneImageItem(MMIWLAN_LIST_WAPI_STATE_CTRL_ID, TXT_WIFI_AUTH_TYPE_WAPI, type_text_array[type], IMAGE_COMMON_RIGHT_ARROW, (uint32)WifiHandleChangeAPWAPIType);

    if (WIFISUPP_WAPI_CREDENTIAL_TYPE_PSK == type)
    {
        uint8*      psk_ptr = PNULL;
        uint8       temp_hex_char[WIFISUPP_WPA_PSK_LEN * 2 + 1] = {0};
        uint16      temp_hex[WIFISUPP_WPA_PSK_LEN*2 + 1] = {0};
        uint16      temp_buf[WIFISUPP_WPA_PSK_LEN + 1] = {0};
        uint32      curr_len = 0;
        uint16      len = 0;
        int32       i = 0;;

        GUILABEL_SetTextById(MMIWIFI_EDIT_WAPI_PRESHAREKEY_LABEL_CTRL_ID,TXT_COMMON_PASSWORD,FALSE);

        psk_ptr = WifiGetWapiPsk();

        if (PNULL != psk_ptr && 0 != strlen((char*)psk_ptr))
        {
            GUIDROPDOWNLIST_SetCurItemIndex(MMIWIFI_EDIT_WAPI_PRESHAREKEY_STATE_DROPDOWNLIST_CTRL_ID, (uint16)is_wapi_ascii);
            if(!is_wapi_ascii)
            {
                GUIEDIT_SetMaxLen(MMIWIFI_EDIT_WAPI_PRESHAREKEY_EDIT_CTRL_ID,WIFISUPP_WPA_PSK_LEN*2);
                len = strlen((char*)psk_ptr);
                if (0 != len)
                {
                    for(i=0; i<len; i++)
                    {
                        curr_len = strlen((char*)temp_hex_char);
                        CFL_Snprintf((char*)temp_hex_char + curr_len ,WIFISUPP_WPA_PSK_LEN * 2 + 1 - curr_len,"%02x",psk_ptr[i]);
                    }
                    len = strlen((char*)temp_hex_char);
                    MMI_STRNTOWSTR(temp_hex, WIFISUPP_WPA_PSK_LEN*2, temp_hex_char, len, MIN(len,WIFISUPP_WPA_PSK_LEN*2));
                    GUIEDIT_SetString(MMIWIFI_EDIT_WAPI_PRESHAREKEY_EDIT_CTRL_ID,temp_hex,len);
                }
            }
            else
            {
                GUIEDIT_SetMaxLen(MMIWIFI_EDIT_WAPI_PRESHAREKEY_EDIT_CTRL_ID,WIFISUPP_WPA_PSK_LEN);
                len = strlen((char*)psk_ptr);
                if (0 != len)
                {
                    MMI_STRNTOWSTR(temp_buf, WIFISUPP_WPA_PSK_LEN, psk_ptr, len, MIN(len,WIFISUPP_WPA_PSK_LEN));
                    GUIEDIT_SetString(MMIWIFI_EDIT_WAPI_PRESHAREKEY_EDIT_CTRL_ID,temp_buf,len);
                }
            }
        }


#ifdef MMI_WLAN_AUTHORITY_COMPLEX
        /* STATE */
        AppendTwoTextOneImageItem(MMIWLAN_LIST_WAPI_STATE_CTRL_ID, TXT_WIFI_WPAAUTH_PSK_STATE, wpa_wpa2_presharekey_state_index[is_wapi_ascii], IMAGE_COMMON_RIGHT_ARROW, (uint32)WifiHandleAPEditChangeWAPIState);
#endif /* MMI_WLAN_AUTHORITY_COMPLEX */
        GUIFORM_SetChildDisplay(MMIWIFI_FORM_AP_EDIT_WAPI_ID, MMIWIFI_CHILD_FORM_WAPI_PASSWORD_CTRL_ID, GUIFORM_CHILD_DISP_NORMAL);
        GUIFORM_SetChildDisplay(MMIWIFI_FORM_AP_EDIT_WAPI_ID, MMIWIFI_CHILD_FORM_CLIENT_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
        GUIFORM_SetChildDisplay(MMIWIFI_FORM_AP_EDIT_WAPI_ID, MMIWIFI_CHILD_FORM_PARENT_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
        GUIFORM_SetChildDisplay(MMIWIFI_FORM_AP_EDIT_WAPI_ID, MMIWIFI_CHILD_FORM_UNINSTALL_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);

    }
    else
    {
        uint16  *clint_path_ptr = PNULL;
        uint16  *parent_path_ptr = PNULL;
        
        GUILABEL_SetTextById(MMIWIFI_EDIT_WAPI_CLIENT_CERTIFICATE_TEXT_LABEL_CTRL_ID,TXT_WIFI_WAPI_CLIENT_CERTIFICATE, FALSE);
        GUILABEL_SetTextById(MMIWIFI_EDIT_WAPI_PARENT_CERTIFICATE_TEXT_LABEL_CTRL_ID,TXT_WIFI_WAPI_PARENT_CERTIFICATE, FALSE);
        GUILABEL_SetTextById(MMIWIFI_LABEL_WAPI_UNINSTALL_CTRL_ID, TXT_WLAN_UNINSTALL, FALSE);

        clint_path_ptr = WifiGetWapiCertiClientFilePath();

        if (PNULL != clint_path_ptr)
        {
            mmi_string.wstr_ptr = clint_path_ptr;
            mmi_string.wstr_len = MMIAPICOM_Wstrlen(clint_path_ptr);
            GUILABEL_SetText(MMIWIFI_EDIT_WAPI_CLIENT_CERTIFICATE_LABEL_CTRL_ID, &mmi_string, FALSE);
        }

        parent_path_ptr = WifiGetWapiCertiParentFilePath();
        if (PNULL != parent_path_ptr)
        {
            mmi_string.wstr_ptr = parent_path_ptr;
            mmi_string.wstr_len = MMIAPICOM_Wstrlen(parent_path_ptr);
            GUILABEL_SetText(MMIWIFI_EDIT_WAPI_PARENT_CERTIFICATE_LBAEL_CTRL_ID, &mmi_string, FALSE);
        }

        GUIFORM_SetChildDisplay(MMIWIFI_FORM_AP_EDIT_WAPI_ID, MMIWIFI_CHILD_FORM_WAPI_PASSWORD_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);

        GUIFORM_SetChildDisplay(MMIWIFI_FORM_AP_EDIT_WAPI_ID, MMIWIFI_CHILD_FORM_CLIENT_CTRL_ID, GUIFORM_CHILD_DISP_NORMAL);
        GUIFORM_SetChildDisplay(MMIWIFI_FORM_AP_EDIT_WAPI_ID, MMIWIFI_CHILD_FORM_PARENT_CTRL_ID, GUIFORM_CHILD_DISP_NORMAL);
        GUIFORM_SetChildDisplay(MMIWIFI_FORM_AP_EDIT_WAPI_ID, MMIWIFI_CHILD_FORM_UNINSTALL_CTRL_ID, GUIFORM_CHILD_DISP_NORMAL);

        if ((0 == MMIAPICOM_Wstrlen(clint_path_ptr)) && (0 == MMIAPICOM_Wstrlen(parent_path_ptr)))
        {
            GUIFORM_SetChildDisplay(MMIWIFI_FORM_AP_EDIT_WAPI_ID, MMIWIFI_CHILD_FORM_UNINSTALL_CTRL_ID, GUIFORM_CHILD_DISP_GRAY);
        }
        else
        {
            GUIFORM_SetChildDisplay(MMIWIFI_FORM_AP_EDIT_WAPI_ID, MMIWIFI_CHILD_FORM_UNINSTALL_CTRL_ID, GUIFORM_CHILD_DISP_NORMAL);
        }
            
    }
    GUILIST_SetCurItemIndex(MMIWLAN_LIST_WAPI_STATE_CTRL_ID, current_index);
    GUILIST_SetTopItemIndex(MMIWLAN_LIST_WAPI_STATE_CTRL_ID, top_index);
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E WifiHandleAPEditWAPIWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;


    //SCI_TRACE_LOW:"WifiHandleAPEditWAPIWinMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_11023_112_2_18_3_10_15_238,(uint8*)"d", msg_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIEDIT_SetIm(MMIWIFI_EDIT_WAPI_PRESHAREKEY_EDIT_CTRL_ID, GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL,GUIIM_TYPE_ABC); /*lint !e655*/
        WifiSetApEditWAPIWin();
        MMK_SetAtvCtrl(win_id, MMIWLAN_LIST_WAPI_STATE_CTRL_ID);
        break;        

    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        {
            MMI_CTRL_ID_T       ctrl_id = 0;

            if (PNULL != param)
            {
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
                else if (MMIWIFI_EDIT_WAPI_CLIENT_CERTIFICATE_LABEL_CTRL_ID == ctrl_id|| MMIWIFI_EDIT_WAPI_PARENT_CERTIFICATE_LBAEL_CTRL_ID == ctrl_id)
                {
                    if(!MMIAPIUDISK_UdiskIsRun())
                    {
                        WifiOpenSelectFileWin(ctrl_id);
                    }
                    else
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
                    }
                }
                else if (ctrl_id == MMIWIFI_LABEL_WAPI_UNINSTALL_CTRL_ID)
                {
                    MMIPUB_OpenQueryWinByTextId(TXT_WLAN_UNINSTALL_QUERY, IMAGE_PUBWIN_QUERY, PNULL, PNULL);;
                }
                else if (ctrl_id == MMIWLAN_LIST_WAPI_STATE_CTRL_ID)
                {
                    uint32              index = 0;
                    SETTING_ITEM_FUNC   item_func = PNULL;

                    index = GUILIST_GetCurItemIndex(MMIWLAN_LIST_WAPI_STATE_CTRL_ID);
                    item_func = (SETTING_ITEM_FUNC)WifiGetListUserdata(MMIWLAN_LIST_WAPI_STATE_CTRL_ID, index);

                    if (PNULL != item_func)
                    {
                        (*item_func)();
                    }
                }
            }
        }
        break;

    case MSG_FULL_PAINT:
        WifiSetApEditWAPIWin();
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
        if (WIFISUPP_WAPI_CREDENTIAL_TYPE_CER == WifiGetWapiCredential())
        {
            if(0 == MMIAPICOM_Wstrlen(WifiGetWapiCertiClientFilePath()) 
                || 0 == MMIAPICOM_Wstrlen(WifiGetWapiCertiParentFilePath()))
            {
                MMIPUB_OpenAlertWarningWin(TXT_WIFI_CERTIFICATE_INVALID);
            }
            else
            {
                s_ui_context.s_mmiwifi_profile.is_auth_input = TRUE;
                MMK_CloseWin( win_id );      
            }
        }
        else
        {
            MMI_STRING_T str_ptr = {0};
            uint16 cur_index1 = 0;

            uint8 temp_buf_char[WIFISUPP_WPA_PSK_LEN + 1] = {0};
            uint8 temp_hex_char[WIFISUPP_WPA_PSK_LEN * 2 + 1] = {0};
            
            GUIEDIT_GetString(MMIWIFI_EDIT_WAPI_PRESHAREKEY_EDIT_CTRL_ID, &str_ptr);//预共享密钥
            cur_index1 = WifiGetIsWapiAscii();//预共享密钥格式

            WifiSetWapiCredential(WIFISUPP_WAPI_CREDENTIAL_TYPE_PSK);

            if (PNULL == str_ptr.wstr_ptr || 0 == str_ptr.wstr_len)
            {
                MMIPUB_OpenAlertWarningWin(TXT_WIFI_KEY_EMPTY);
            }
            else
            {
                if(!cur_index1)
                {
                    if (str_ptr.wstr_len < 16 || str_ptr.wstr_len > 126)
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_WIFI_KEY_RANGE_OUT_1);
                        break;
                    }
                    MMI_WSTRNTOSTR(temp_hex_char, WIFISUPP_WPA_PSK_LEN*2, str_ptr.wstr_ptr, str_ptr.wstr_len, str_ptr.wstr_len);
                    if (!WifiCheckIsHex(temp_hex_char,str_ptr.wstr_len))
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_WIFI_KEY_HEX_INPUT);
                        break;
                    }
                    WifiHexStrToBin((const char*)temp_hex_char,temp_buf_char,WIFISUPP_WPA_PSK_LEN);
                    WifiSetWapiPsk(temp_buf_char);
                }
                else
                {
                    if (str_ptr.wstr_len < 8 || str_ptr.wstr_len > 63)
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_WIFI_KEY_RANGE_OUT);
                        break;
                    }
                    MMI_WSTRNTOSTR(temp_buf_char, WIFISUPP_WPA_PSK_LEN, str_ptr.wstr_ptr, str_ptr.wstr_len, str_ptr.wstr_len);
                    WifiSetWapiPsk(temp_buf_char);
                }
                s_ui_context.s_mmiwifi_profile.is_auth_input = TRUE;
                MMK_CloseWin( win_id );      
            }
        }
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );      
        break;

    case MSG_PROMPTWIN_OK:
        {
            SCI_MEMSET(s_ui_context.s_mmiwifi_profile.client_file_name, 0 ,sizeof(s_ui_context.s_mmiwifi_profile.client_file_name));
            SCI_MEMSET(s_ui_context.s_mmiwifi_profile.parent_file_name, 0 ,sizeof(s_ui_context.s_mmiwifi_profile.parent_file_name));

            s_ui_context.s_mmiwifi_profile.is_auth_input = FALSE;
            MMK_SetAtvCtrl(win_id, MMIWIFI_EDIT_WAPI_CLIENT_CERTIFICATE_LABEL_CTRL_ID);
            MMIPUB_CloseQuerytWin(PNULL);

            if (MMK_IsOpenWin(MMIWIFI_EDIT_WIN_ID)) //Edit Ap
            {
                MMIWIFI_PROFILE_T       *profile_ptr = PNULL;   
                uint16                  index = 0;
                MMI_CTRL_ID_T           ctrl_id = 0;

#ifdef MMI_WLAN_TAB_STYLE
                ctrl_id = MMIWIFI_LIST_TRUSTED_CTRL_ID;
#else
                ctrl_id = MMIWIFI_LIST_CTRL_ID;
#endif /* MMI_WLAN_TAB_STYLE */

                index = GUILIST_GetCurItemIndex(ctrl_id);

                profile_ptr = (MMIWIFI_PROFILE_T *)WifiGetListUserdata(ctrl_id, index);

                if (!WifiListIsAPItemSelected(index) || PNULL == profile_ptr)
                {
                    //SCI_TRACE_LOW:"WifiHandleAuthSettingWAPICERTIFICATEWinMsg profile_ptr is Error index = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_11198_112_2_18_3_10_16_239,(uint8*)"d", index);
                    break;
                }

                if(MMIAPICOM_Wstrlen(profile_ptr->client_file_name) && MMIAPICOM_Wstrlen(profile_ptr->parent_file_name) )
                {
                    SCI_MEMSET(profile_ptr->client_file_name, 0, sizeof(profile_ptr->client_file_name));
                    SCI_MEMSET(profile_ptr->parent_file_name, 0, sizeof(profile_ptr->parent_file_name));
                    profile_ptr->is_trusted = FALSE;
                    MMIWIFI_UpdateProfileFile();
                }
            }

#ifdef MMI_GUI_STYLE_TYPICAL
            MMIPUB_OpenAlertWinByTextId(PNULL, TXT_WLAN_UNINSTALL_SUCC, 0, IMAGE_PUBWIN_SUCCESS, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
#endif
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
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E WifiHandleEditAPSSIDWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"WifiHandleEditAPSSIDWinMsg win_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_11234_112_2_18_3_10_16_240,(uint8*)"d",win_id);
    
    switch(msg_id) 
    {
    case MSG_OPEN_WINDOW:
        {
            uint8       *ssid_ptr = PNULL;
            uint8       ssid_len = 0;
            uint16      temp_buf[WIFISUPP_SSID_MAX_LEN + 1] = {0};

            GUIEDIT_SetTextMaxLen(MMIWIFI_EDIT_SSID_DATA_EDIT_CTRL_ID, WIFISUPP_SSID_MAX_LEN / 2 - 1, WIFISUPP_SSID_MAX_LEN);

            ssid_ptr = WifiGetSsid();
            ssid_len = WifiGetSsidLen();

            if (0 != ssid_len && PNULL != ssid_ptr)
            {
                GUI_GBToWstr(temp_buf, ssid_ptr, MIN(ssid_len, WIFISUPP_SSID_MAX_LEN));
            }

            GUIEDIT_SetString(MMIWIFI_EDIT_SSID_DATA_EDIT_CTRL_ID, temp_buf, MMIAPICOM_Wstrlen(temp_buf));

            MMK_SetAtvCtrl(win_id,MMIWIFI_EDIT_SSID_DATA_EDIT_CTRL_ID);
        }
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
        {
            MMI_STRING_T string = {0};
            uint8  temp_buf_char[WIFISUPP_SSID_MAX_LEN * 2 + 2] = {0};
            uint16 ssid_len = 0;

            GUIEDIT_GetString(MMIWIFI_EDIT_SSID_DATA_EDIT_CTRL_ID, &string);

            if (PNULL == string.wstr_ptr || 0 == string.wstr_len)
            {
                MMIPUB_OpenAlertWarningWin(TXT_WIFI_SSID_EMPTY);
            }
            else
            {
                (void)GUI_WstrToGB(temp_buf_char, string.wstr_ptr, string.wstr_len);
                WifiSetSsid(temp_buf_char);
                ssid_len = SCI_STRLEN((char *)temp_buf_char);
                WifiSetSsidLen(MIN(WIFISUPP_SSID_MAX_LEN, ssid_len));

                MMK_CloseWin(win_id);
            }
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

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void WifiOpenAPPropertyWin(MMIWIFI_PROFILE_T *profile_ptr)
{
    MMI_STRING_T        str = {0};
    uint16              uint16_str[MMIWIFI_DETAIL_TEMP_LEN+1] = {0};
    char                temp_buf[MMIWIFI_DETAIL_TEMP_LEN+1] = {0};
    uint32              len = 0;
    MMI_TEXT_ID_T       text_id = TXT_NULL;
    wchar               ap_property[GUIRICHTEXT_TEXT_MAX_LEN] = {0};
    wchar               *property_ptr = PNULL;
    BOOLEAN             is_connected = TRUE;
    uint32              time_out = 0;
    uint8               essid[WIFISUPP_SSID_MAX_LEN + 1] = {0};
    MMI_WIN_ID_T        win_id = MMIWIFI_AP_PROPERTY_WIN_ID;
    
    if (PNULL == profile_ptr)
    {
        return;
    }

    profile_ptr->is_editing = TRUE; /* Cannot to be deleted by Auto Scan */

    property_ptr = ap_property;

    SCI_MEMCPY(essid, profile_ptr->config_info.ssid.ssid, WIFISUPP_SSID_MAX_LEN);
    //SCI_TRACE_LOW:"WIFI:%s ESSID =%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_11325_112_2_18_3_10_16_241,(uint8*)"ss", __func__, essid);

    /* Status */
    MMIRES_GetText(TXT_WLAN_AP_STATUS, PNULL, &str);
    MMIAPICOM_Wstrncpy(property_ptr, str.wstr_ptr, str.wstr_len);
    property_ptr += str.wstr_len;

    is_connected = (MMIWIFI_PROFILE_STATE_CONNECTED == profile_ptr->state) || (MMIWIFI_PROFILE_STATE_DHCPED == profile_ptr->state);

    if (is_connected)
    {
        MMIRES_GetText(TXT_COMMON_CONNECTED, PNULL, &str);
    }
    else
    {
        MMIRES_GetText(TXT_DISCONNECTED, PNULL, &str);
    }

    MMIAPICOM_Wstrncpy(property_ptr, str.wstr_ptr, str.wstr_len);
    property_ptr += str.wstr_len;

    property_ptr[0] = '\n';
    property_ptr += 1;

    if (is_connected)
    {
        /* Rate */
        MMIRES_GetText(TXT_WIFI_DETAIL_RATE, PNULL, &str);
        MMIAPICOM_Wstrncpy(property_ptr, str.wstr_ptr, str.wstr_len);
        property_ptr += str.wstr_len;

        SCI_MEMSET(temp_buf,0,sizeof(temp_buf));
        CFL_Snprintf(temp_buf, MMIWIFI_DETAIL_TEMP_LEN,"%d%s",profile_ptr->rate,"Mbps");
        len = strlen(temp_buf);
        SCI_MEMSET(uint16_str,0,sizeof(uint16_str));
        MMI_STRNTOWSTR(uint16_str, MMIWIFI_DETAIL_TEMP_LEN, (uint8 *)temp_buf,len,MIN(len,MMIWIFI_DETAIL_TEMP_LEN));
        MMIAPICOM_Wstrncpy(property_ptr, uint16_str, len);
        property_ptr += len;

        property_ptr[0] = '\n';
        property_ptr += 1;

        /* Channel */
        MMIRES_GetText(TXT_WIFI_DETAIL_CHANNEL, PNULL, &str);
        MMIAPICOM_Wstrncpy(property_ptr, str.wstr_ptr, str.wstr_len);
        property_ptr += str.wstr_len;

        SCI_MEMSET(temp_buf,0,sizeof(temp_buf));
        CFL_Snprintf(temp_buf, MMIWIFI_DETAIL_TEMP_LEN,"%d",profile_ptr->config_info.channel);
        len = strlen(temp_buf);
        SCI_MEMSET(uint16_str,0,sizeof(uint16_str));
        MMI_STRNTOWSTR(uint16_str, MMIWIFI_DETAIL_TEMP_LEN, (uint8 *)temp_buf, len,MIN(len,MMIWIFI_DETAIL_TEMP_LEN));
        MMIAPICOM_Wstrncpy(property_ptr, uint16_str, len);
        property_ptr += len;

        property_ptr[0] = '\n';
        property_ptr += 1;
    }

    /* Singal Intensity */
    MMIRES_GetText(TXT_WIFI_DETAIL_RSSI, PNULL, &str);
    MMIAPICOM_Wstrncpy(property_ptr, str.wstr_ptr, str.wstr_len);
    property_ptr += str.wstr_len;

    if (profile_ptr->is_on_air)
    {
        if (profile_ptr->rssi <= MMIWIFI_RSSI_40)
        {
            MMI_GetLabelTextByLang(TXT_WIFI_DETAIL_RSSI_WEAK, &str);
        }
        else
        {
            MMI_GetLabelTextByLang(TXT_WIFI_DETAIL_RSSI_STRONG, &str);
        }
    }
    else
    {
        if (profile_ptr->is_ssid_hide)
        {
            MMI_GetLabelTextByLang(TXT_UNKNOWN, &str);
        }
        else
        {
            MMI_GetLabelTextByLang(TXT_NONE, &str);
        }    
    }    
    MMIAPICOM_Wstrncpy(property_ptr, str.wstr_ptr, str.wstr_len);
    property_ptr += str.wstr_len;

    property_ptr[0] = '\n';
    property_ptr += 1;

    if (is_connected)
    {
        /* Credential */
        MMIRES_GetText(TXT_WIFI_DETAIL_CRED, PNULL, &str);
        MMIAPICOM_Wstrncpy(property_ptr, str.wstr_ptr, str.wstr_len);
        property_ptr += str.wstr_len;

        switch(profile_ptr->config_info.encryp_protocol) 
        {
        case WIFISUPP_ENCRYP_PROTOCOL_OPENSYS:
            text_id = TXT_WIFI_AUTH_TYPE_OPENSYS;
            break;

        case WIFISUPP_ENCRYP_PROTOCOL_WEP:
            text_id = TXT_WIFI_AUTH_TYPE_WEP;
            break;

        case WIFISUPP_ENCRYP_PROTOCOL_WPA:
            text_id = TXT_WIFI_AUTH_TYPE_WPA;
            break;

        case WIFISUPP_ENCRYP_PROTOCOL_WPA2:
            text_id = TXT_WIFI_AUTH_TYPE_WPA2;
            break;

        case WIFISUPP_ENCRYP_PROTOCOL_WAPI:
            text_id = TXT_WIFI_AUTH_TYPE_WAPI;
            break;

#ifdef WLAN_EAP_SUPPORT
        case WIFISUPP_ENCRYP_PROTOCOL_EAP:
            text_id = TXT_WLAN_EAP_SIM;
            break;
#endif // WLAN_EAP_SUPPORT

        default:
            text_id = TXT_NULL;
            break;
        }
        MMIRES_GetText(text_id, PNULL, &str);
        MMIAPICOM_Wstrncpy(property_ptr, str.wstr_ptr, str.wstr_len);
        property_ptr += str.wstr_len;

        property_ptr[0] = '\n';
        property_ptr += 1;

        /* IP Address */
        MMIRES_GetText(TXT_WIFI_DETAIL_IP, PNULL, &str);
        MMIAPICOM_Wstrncpy(property_ptr, str.wstr_ptr, str.wstr_len);
        property_ptr += str.wstr_len;

        len = strlen(profile_ptr->ip_get.ip.ip_addr);
        SCI_MEMSET(uint16_str,0,sizeof(uint16_str));
        MMI_STRNTOWSTR(uint16_str, MMIWIFI_DETAIL_TEMP_LEN, (uint8 *)profile_ptr->ip_get.ip.ip_addr, len, MIN(len,MMIWIFI_DETAIL_TEMP_LEN));

        MMIAPICOM_Wstrncpy(property_ptr, uint16_str, len);
        property_ptr += len;

        property_ptr[0] = '\n';
        property_ptr += 1;
    }

    str.wstr_ptr = ap_property;
    str.wstr_len = MMIAPICOM_Wstrlen(ap_property);

    MMIPUB_OpenAlertWinByTextPtrEx(MMK_GetFirstAppletHandle(), &time_out, &str, PNULL, IMAGE_PUBWIN_SUCCESS, &win_id, PNULL, MMIPUB_SOFTKEY_CUSTOMER, HandleAPPropertyAlertWinMsg, (uint32)profile_ptr);
}

/*****************************************************************************/
// Description :
// Global resource dependence : none
// Author:Jiaoyou.wu
// Note:    
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAPPropertyAlertWinMsg(
                                        MMI_WIN_ID_T     win_id, 
                                        MMI_MESSAGE_ID_E msg_id, 
                                        DPARAM           param 
                                        )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"HandleAPPropertyAlertWinMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_11490_112_2_18_3_10_16_242,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            MMI_STRING_T        title_str       = {0};
            MMIPUB_INFO_T       *win_info_ptr   = PNULL;
            MMIWIFI_PROFILE_T   *profile_ptr    = PNULL;
            uint32              ssid_Len        = 0;
            uint16              ssid[WIFISUPP_SSID_MAX_LEN * 2 + 4] = {0};

            win_info_ptr = (MMIPUB_INFO_T *)MMIPUB_GetWinAddDataPtr(win_id);
            if(PNULL == win_info_ptr)
            {
                break;
            }
            profile_ptr  = (MMIWIFI_PROFILE_T *)(win_info_ptr->user_data);

            if (PNULL == profile_ptr)
            {
                MMIPUB_SetWinSoftkey(win_id, TXT_NULL, STXT_RETURN, FALSE);
                break;
            }

            ssid_Len = GUI_GBToWstr(ssid, profile_ptr->config_info.ssid.ssid, MIN(profile_ptr->config_info.ssid.ssid_len, WIFISUPP_SSID_MAX_LEN));
            title_str.wstr_len = (uint16)ssid_Len;
            title_str.wstr_ptr = (wchar *)ssid;

            MMIPUB_SetWinTitleText(win_id, &title_str, FALSE);

            profile_ptr->is_editing = TRUE;

            if (profile_ptr->is_on_air || profile_ptr->is_ssid_hide)
            {
                if (MMIWIFI_PROFILE_STATE_CONNECTED == profile_ptr->state || MMIWIFI_PROFILE_STATE_DHCPED == profile_ptr->state)
                {
                    MMIPUB_SetWinSoftkey(win_id, TXT_COMM_DISCONNECT, STXT_RETURN, FALSE);
                }
                else
                {
                    MMIPUB_SetWinSoftkey(win_id, TXT_COMMON_CONNECT_COMN, STXT_RETURN, FALSE);
                }
            }
            else
            {
                MMIPUB_SetWinSoftkey(win_id, TXT_NULL, STXT_RETURN, FALSE);
            }    
        }
        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
        {
            MMIPUB_INFO_T       *win_info_ptr = PNULL;
            MMIWIFI_PROFILE_T   *profile_ptr  = PNULL;

            win_info_ptr = (MMIPUB_INFO_T *)MMIPUB_GetWinAddDataPtr(win_id);
            if(PNULL == win_info_ptr)
            {
                break;
            }
            profile_ptr  = (MMIWIFI_PROFILE_T *)(win_info_ptr->user_data);
 
            MMK_CloseWin(win_id);

            if (PNULL == profile_ptr)
            {
                break;
            }

            if (!(profile_ptr->is_on_air || profile_ptr->is_ssid_hide))
            {
                break;
            }

            if (MMIWIFI_PROFILE_STATE_CONNECTED == profile_ptr->state || MMIWIFI_PROFILE_STATE_DHCPED == profile_ptr->state)
            {
                MMIWIFI_DisConnect(profile_ptr);
            }
            else 
            {
                if (MMIWIFI_GetScanStatus())
                {
                    //SCI_TRACE_LOW:"MMIWIFI_Connect Scaning"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_11575_112_2_18_3_10_16_243,(uint8*)"");
                    MMIPUB_OpenAlertWarningWin(TXT_WIFI_CONNECT_BUSY);
                    break;
                }

                if (MMIWIFI_Connect(profile_ptr))
                {
                    MMIWIFI_OpenConnectWaitWin();
                }
            }
        }
        
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        {
            MMIPUB_INFO_T *win_info_ptr = PNULL;
            MMIWIFI_PROFILE_T *profile_ptr = PNULL;

            win_info_ptr = (MMIPUB_INFO_T *)MMIPUB_GetWinAddDataPtr(win_id);
            if(PNULL == win_info_ptr)
            {
                break;
            }            
            profile_ptr = (MMIWIFI_PROFILE_T *)(win_info_ptr->user_data);
            if (PNULL != profile_ptr)
            {
                profile_ptr->is_editing = FALSE;
            }
        }
        /* Go Through */
        break;
    default:
        recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    }

    return (recode);
}

/*****************************************************************************/
// Description :
// Global resource dependence : none
// Author:Jiaoyou.wu
// Note:    
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleQueryEditConnectedAP(
                                        MMI_WIN_ID_T     win_id, 
                                        MMI_MESSAGE_ID_E msg_id, 
                                        DPARAM           param 
                                        )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"HandleQueryEditConnectedAP msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_11632_112_2_18_3_10_16_244,(uint8*)"d", msg_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            MMIPUB_INFO_T       *win_info_ptr   = PNULL;
            MMIWIFI_PROFILE_T   *profile_ptr    = PNULL;


            win_info_ptr = (MMIPUB_INFO_T *)MMIPUB_GetWinAddDataPtr(win_id);
            if(PNULL == win_info_ptr)
            {
                break;
            }            
            profile_ptr  = (MMIWIFI_PROFILE_T *)(win_info_ptr->user_data);

            if (PNULL != profile_ptr)
            {
                profile_ptr->is_editing = TRUE;
            }
        }
        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
        {
            MMIPUB_INFO_T *win_info_ptr = PNULL;
            MMIWIFI_PROFILE_T *profile_ptr = PNULL;

            win_info_ptr = (MMIPUB_INFO_T *)MMIPUB_GetWinAddDataPtr(win_id);
            if(PNULL == win_info_ptr)
            {
                break;
            }            
            profile_ptr = (MMIWIFI_PROFILE_T *)(win_info_ptr->user_data);

            MMIPUB_CloseQuerytWin(PNULL);

            if (PNULL != profile_ptr && (profile_ptr->state != MMIWIFI_PROFILE_STATE_DISCONNECTED && profile_ptr->state != MMIWIFI_PROFILE_STATE_DISCONNECTING))
            {
                MMIWIFI_DisConnect(profile_ptr);
                MMIWIFI_OpenEditWin(profile_ptr);
            }
        }
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        {
            MMIPUB_INFO_T *win_info_ptr = PNULL;
            MMIWIFI_PROFILE_T *profile_ptr = PNULL;

            win_info_ptr = (MMIPUB_INFO_T *)MMIPUB_GetWinAddDataPtr(win_id);
            if(PNULL == win_info_ptr)
            {
                break;
            }            
            profile_ptr = (MMIWIFI_PROFILE_T *)(win_info_ptr->user_data);
            if (PNULL != profile_ptr)
            {
                profile_ptr->is_editing = FALSE;
            }
        }
        /* Go Through */
        break;
    default:
        recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    }

    return (recode);
}
/*****************************************************************************/
// Description :
// Global resource dependence : none
// Author:Jiaoyou.wu
// Note:    
/*****************************************************************************/
LOCAL void WifiHandleChangeWPAState()
{
    MMK_CreatePubListWin((uint32 *)MMIWIFI_WPA_STATE_SET_WIN_TAB, PNULL);
}

/*****************************************************************************/
// Description :
// Global resource dependence : none
// Author:Jiaoyou.wu
// Note:    
/*****************************************************************************/
LOCAL void WifiHandleAPEditChangeWPAState()
{
    MMK_CreatePubListWin((uint32 *)MMIWIFI_AP_EDIT_STATE_SET_WIN_TAB, PNULL);
}

/*****************************************************************************/
// Description :
// Global resource dependence : none
// Author:Jiaoyou.wu
// Note:    
/*****************************************************************************/
LOCAL void WifiHandleAPEditAuthType()
{
    MMK_CreatePubListWin((uint32 *)MMIWIFI_AP_AUTH_TYPE_WIN_TAB, PNULL);
}

/*****************************************************************************/
// Description :
// Global resource dependence : none
// Author:Jiaoyou.wu
// Note:    
/*****************************************************************************/
LOCAL void WifiHandleAPEditAuthSetting()
{
    WIFISUPP_ENCRYP_PROTOCOL_E auth_type = WIFISUPP_ENCRYP_PROTOCOL_WEP;

    auth_type = WifiGetEncryptProtocol();

    //SCI_TRACE_LOW:"WifiHandleAPEditAuthSetting auth_type = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_11748_112_2_18_3_10_17_245,(uint8*)"d", auth_type);

    switch(auth_type) 
    {
        case WIFISUPP_ENCRYP_PROTOCOL_WEP:
            MMK_CreateWin((uint32 *)MMIWIFI_AUTH_SETTING_WEP_WIN_TAB, PNULL);
            break;

        case WIFISUPP_ENCRYP_PROTOCOL_WPA:
        case WIFISUPP_ENCRYP_PROTOCOL_WPA2:
            MMK_CreateWin((uint32 *)MMIWIFI_AUTH_SETTING_WPA_WPA2_WIN_TAB, PNULL);
            break;

        case WIFISUPP_ENCRYP_PROTOCOL_WAPI:
            MMK_CreateWin((uint32 *)MMIWIFI_AP_EDIT_WAPI_WIN_TAB, PNULL);
            break;

    default:
        break;
    }
}

/*****************************************************************************/
// Description :
// Global resource dependence : none
// Author:Jiaoyou.wu
// Note:    
/*****************************************************************************/
LOCAL void WifiHandleAPEditConnectState()
{
    MMK_CreatePubListWin((uint32 *)MMIWIFI_AP_CONNECT_STATE_WIN_TAB, PNULL);
}

/*****************************************************************************/
// Description :
// Global resource dependence : none
// Author:Jiaoyou.wu
// Note:    
/*****************************************************************************/
LOCAL void WifiHandleAPEditNetworkState()
{
    MMK_CreatePubListWin((uint32 *)MMIWIFI_AP_NETWORK_STATE_WIN_TAB, PNULL);
}

/*****************************************************************************/
// Description :
// Global resource dependence : none
// Author:Jiaoyou.wu
// Note:    
/*****************************************************************************/
LOCAL void WifiHandleChangeWAPIState()
{
    MMK_CreatePubListWin((uint32 *)MMIWIFI_WAPI_STATE_SET_WIN_TAB, PNULL);
}

/*****************************************************************************/
// Description :
// Global resource dependence : none
// Author:Jiaoyou.wu
// Note:    
/*****************************************************************************/
LOCAL void WifiHandleAPEditChangeWAPIState()
{
    MMK_CreatePubListWin((uint32 *)MMIWIFI_AP_EDIT_WAPI_STATE_SET_WIN_TAB, PNULL);
}

/*****************************************************************************/
// Description :
// Global resource dependence : none
// Author:Jiaoyou.wu
// Note:    
/*****************************************************************************/
LOCAL void WifiHandleChangeAPWAPIType()
{
    MMK_CreatePubListWin((uint32 *)MMIWIFI_AP_EDIT_WAPI_TYPE_WIN_TAB, PNULL);
}

/*****************************************************************************/
// Description :
// Global resource dependence : none
// Author:Jiaoyou.wu
// Note:    
/*****************************************************************************/
LOCAL void WifiHandleWepModelChange()
{
    MMK_CreatePubListWin((uint32 *)MMIWIFI_WEP_MODE_SET_WIN_TAB, PNULL);
}

/*****************************************************************************/
// Description :
// Global resource dependence : none
// Author:Jiaoyou.wu
// Note:    
/*****************************************************************************/
LOCAL void WifiHandleWepKeyIndexChange()
{
    MMK_CreatePubListWin((uint32 *)MMIWIFI_WEP_KEY_INDEX_SET_WIN_TAB, PNULL);
}

/*****************************************************************************/
// Description :
// Global resource dependence : none
// Author:Jiaoyou.wu
// Note:    
/*****************************************************************************/
LOCAL void WifiHandleWepEncyptChange()
{
    MMK_CreatePubListWin((uint32 *)MMIWIFI_WEP_ENCYPT_SET_WIN_TAB, PNULL);
}

/*****************************************************************************/
// Description :
// Global resource dependence : none
// Author:Jiaoyou.wu
// Note:    
/*****************************************************************************/
LOCAL void WifiHandleWepStateChange()
{
    MMK_CreatePubListWin((uint32 *)MMIWIFI_WEP_STATE_SET_WIN_TAB, PNULL);
}

/*****************************************************************************/
// Description :
// Global resource dependence : none
// Author:Jiaoyou.wu
// Note:    
/*****************************************************************************/
LOCAL void WifiHandleAPEditWepModelChange()
{
    MMK_CreatePubListWin((uint32 *)MMIWIFI_AP_EDIT_WEP_MODE_SET_WIN_TAB, PNULL);
}

/*****************************************************************************/
// Description :
// Global resource dependence : none
// Author:Jiaoyou.wu
// Note:    
/*****************************************************************************/
LOCAL void WifiHandleAPEditWepKeyIndexChange()
{
    MMK_CreatePubListWin((uint32 *)MMIWIFI_AP_EDIT_WEP_KEY_INDEX_SET_WIN_TAB, PNULL);
}

/*****************************************************************************/
// Description :
// Global resource dependence : none
// Author:Jiaoyou.wu
// Note:    
/*****************************************************************************/
LOCAL void WifiHandleAPEditWepEncyptChange()
{
    MMK_CreatePubListWin((uint32 *)MMIWIFI_AP_EDIT_WEP_ENCYPT_SET_WIN_TAB, PNULL);
}

/*****************************************************************************/
// Description :
// Global resource dependence : none
// Author:Jiaoyou.wu
// Note:    
/*****************************************************************************/
LOCAL void WifiHandleAPEditWepStateChange()
{
    MMK_CreatePubListWin((uint32 *)MMIWIFI_AP_EDIT_WEP_STATE_SET_WIN_TAB, PNULL);
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void WifiEditAPSSIDWin(void)
{
    MMK_CreatePubEditWin((uint32 *)MMIWIFI_EDIT_AP_SSID_WIN_TAB, PNULL);
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiCreateGPRSToWLANMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    MMI_TEXT_ID_T gprs2wlan_index[] = {TXT_WLAN_ALWAYS_INQUIRY,TXT_WLAN_AUTO_RESELCT,TXT_WLAN_MANUAL_RESELCT};
    uint32 node_index[] = {MMIWLAN_GPRSTOWLAN_OPT_ALWAYS_INQUIRY, MMIWLAN_GPRSTOWLAN_OPT_AUTO_RESELCT, MMIWLAN_GPRSTOWLAN_OPT_MANUAL_RESELCT};
    uint16 select_index = 0;
    uint32 num = 0;

    num = sizeof(gprs2wlan_index) / sizeof(gprs2wlan_index[0]);
  
    select_index = (uint16)(MMIWIFI_GetGPRSToWlanReselect() - MMI_GPRSTOWLAN_ALWAYS_INQUIRY);/*lint !e656*/

    WifiCreateRadioListByTextId(win_id, ctrl_id, num, gprs2wlan_index, node_index, select_index);
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleGPRSToWLANWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"WifiHandleGPRSToWLANWinMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_11922_112_2_18_3_10_17_246,(uint8*)"d", msg_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        WifiCreateGPRSToWLANMenu(win_id, MMIWIFI_MENU_GPRS_TO_WLAN_CTRL_ID);
        MMK_SetAtvCtrl(win_id, MMIWIFI_MENU_GPRS_TO_WLAN_CTRL_ID);
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

            select_index = GUILIST_GetCurItemIndex(MMIWIFI_MENU_GPRS_TO_WLAN_CTRL_ID);

            if(select_index != MMIWIFI_GetGPRSToWlanReselect())
            {
               MMIWIFI_SetGPRSToWlanReselect(&select_index);
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

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiCreateReselectAPMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    MMI_TEXT_ID_T ap_reselect_index[] = {TXT_WLAN_AUTO_RESELCT,TXT_WLAN_MANUAL_RESELCT};
    uint32 node_index[] = {MMIWLAN_RESELECT_AP_OPT_AUTO, MMIWLAN_RESELECT_AP_OPT_MANUALT};
    uint16 select_index = 0;
    uint32 num = 0;

    num = sizeof(ap_reselect_index) / sizeof(ap_reselect_index[0]);
  
    select_index = (uint16)(MMIWIFI_GetApReselect() - MMI_AP_AUTO_RESELCET);/*lint !e656*/
    WifiCreateRadioListByTextId(win_id, ctrl_id, num, ap_reselect_index, node_index, select_index);
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleReselectAPWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"WifiHandleGPRSToWLANWinMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_11988_112_2_18_3_10_17_247,(uint8*)"d", msg_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        WifiCreateReselectAPMenu(win_id, MMIWIFI_MENU_RELECT_AP_SET_CTRL_ID);
        MMK_SetAtvCtrl(win_id, MMIWIFI_MENU_RELECT_AP_SET_CTRL_ID);
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

            select_index = (MMI_AP_RESELECT)(MMI_AP_AUTO_RESELCET + GUILIST_GetCurItemIndex(MMIWIFI_MENU_RELECT_AP_SET_CTRL_ID));

            if(select_index != MMIWIFI_GetApReselect())
            {
                MMIWIFI_SetApReselect(&select_index);
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

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiCreatePromptNoWlanMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    MMI_TEXT_ID_T nowlan_index[] = {TXT_COMMON_PROMPT, TXT_COMMON_NOT_PROMPT};
    uint32 node_index[] = {MMIWLAN_PROMPT_NO_WLAN_OPT_PROMPT, MMIWLAN_PROMPT_NO_WLAN_OPT_NOT_PROMPT};
    uint16 select_index = 0;
    uint32 num = 0;

    num = sizeof(nowlan_index) / sizeof(nowlan_index[0]);
  
    select_index = (uint16)(!MMIWIFI_GetIsShowNoWlanPrompt());

    WifiCreateRadioListByTextId(win_id, ctrl_id, num, nowlan_index, node_index, select_index);
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandlePromptNoWLANWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"WifiHandleGPRSToWLANWinMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_12055_112_2_18_3_10_17_248,(uint8*)"d", msg_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        WifiCreatePromptNoWlanMenu(win_id, MMIWIFI_MENU_PROMPT_NO_WLAN_CTRL_ID);
        MMK_SetAtvCtrl(win_id, MMIWIFI_MENU_PROMPT_NO_WLAN_CTRL_ID);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        {
            BOOLEAN is_prompt = TRUE;

            is_prompt = (BOOLEAN)(0 == GUILIST_GetCurItemIndex(MMIWIFI_MENU_PROMPT_NO_WLAN_CTRL_ID));

            if(is_prompt != MMIWIFI_GetIsShowNoWlanPrompt())
            {
               MMIWIFI_SetIsShowNoWlanPrompt(is_prompt);
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

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
void WifiCreateRadioListByTextId(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, uint16 node_num, MMI_TEXT_ID_T *text_id_array, uint32 *node_array, uint32 select_index)
{
    GUILIST_ITEM_T          item_t = {0};
    GUILIST_ITEM_DATA_T     item_data = {0};
    uint16                  index = {0};

    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
    item_t.item_data_ptr = &item_data;

    GUILIST_SetMaxItem(ctrl_id, node_num, FALSE);

    for (index = 0; index < node_num; index++)
    {
        item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[0].item_data.text_id = text_id_array[index];
        item_t.user_data                            = node_array[index];

        if (!GUILIST_AppendItem(ctrl_id, &item_t))
        {
            SCI_TRACE_LOW("WifiCreateRadioListByTextId failed");
        }
    }

    GUILIST_SetSelectedItem(ctrl_id, select_index, TRUE);
    GUILIST_SetCurItemIndex(ctrl_id, select_index);
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
void WifiCreatePopupAutoMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, MMI_STRING_T *title_str, uint16 node_num, MMI_TEXT_ID_T *text_id_array, uint32 *node_array)
{
    uint16 index = 0;
    MMI_STRING_T string = {0};
    GUIMENU_DYNA_ITEM_T node_item = {0};
    GUIMENU_BUTTON_INFO_T button_info = {0};

    GUIMENU_CreatDynamic(PNULL, win_id, ctrl_id, GUIMENU_STYLE_POPUP_AUTO);

    if (PNULL != title_str)
    {
        GUIMENU_SetMenuTitle(title_str, ctrl_id);
    }

    for (index = 0; index < node_num; index ++)
    {
        MMI_GetLabelTextByLang(text_id_array[index], &string);
        node_item.item_text_ptr = &string;
        node_item.is_grayed = FALSE;
        GUIMENU_InsertNode(index, node_array[index], 0, &node_item, ctrl_id);
    }

    button_info.is_static   = FALSE;
    button_info.button_style = GUIMENU_BUTTON_STYLE_CANCEL;
    GUIMENU_SetButtonStyle(ctrl_id, &button_info);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL void InsertTwoTextOneImageItem(MMI_CTRL_ID_T ctrl_id, uint16 index, MMI_TEXT_ID_T text_1, MMI_TEXT_ID_T text_2, MMI_IMAGE_ID_T image_id, uint32 user_data)
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};

    if(TXT_NULL == text_2)
    {
        item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_ICON;
    }
    else
    {
        item_t.item_style    = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM;
    }    

    item_t.item_data_ptr = &item_data;
    item_t.user_data     = user_data;

    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_1;

    if(TXT_NULL != text_2)
    {
        item_data.item_content[2].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[2].item_data.text_id = text_2;
    }

    if (IMAGE_NULL != image_id)
    {
        item_data.item_content[1].item_data_type = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[1].item_data.text_id = image_id;
    }

    if (!GUILIST_InsertItem(ctrl_id, &item_t, index))
    {
        SCI_TRACE_LOW("InsertTwoTextOneImageItem failed");
    }
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL void AppendTwoTextOneImageItem(MMI_CTRL_ID_T ctrl_id, MMI_TEXT_ID_T text_1, MMI_TEXT_ID_T text_2, MMI_IMAGE_ID_T image_id, uint32 user_data)
{
    InsertTwoTextOneImageItem(ctrl_id, GUILIST_GetTotalItemNum(ctrl_id), text_1, text_2, image_id, user_data);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL void InsertTwoTextOneImageItemByString(MMI_CTRL_ID_T ctrl_id, uint16 index, MMI_STRING_T *text_1, MMI_STRING_T *text_2, MMI_IMAGE_ID_T image_id, uint32 user_data)
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};

    item_t.item_style    = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM;
    item_t.item_data_ptr = &item_data;
    item_t.user_data     = user_data;

    //text 1
    if(PNULL != text_1)
    {
        item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[0].item_data.text_buffer.wstr_len = text_1->wstr_len;
        item_data.item_content[0].item_data.text_buffer.wstr_ptr = text_1->wstr_ptr;
    }

    //text 2
    if(PNULL != text_2)
    {
        item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[2].item_data.text_buffer.wstr_len = text_2->wstr_len;
        item_data.item_content[2].item_data.text_buffer.wstr_ptr = text_2->wstr_ptr;
    }

    if (IMAGE_NULL != image_id)
    {
        item_data.item_content[1].item_data_type = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[1].item_data.text_id = image_id;
    }

    if (!GUILIST_InsertItem(ctrl_id, &item_t, index))
    {
        SCI_TRACE_LOW("InsertTwoTextOneImageItem failed");
    }
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL void AppendTwoTextOneImageItemByString(MMI_CTRL_ID_T ctrl_id, MMI_STRING_T *text_1, MMI_STRING_T *text_2, MMI_IMAGE_ID_T image_id, uint32 user_data)
{
    InsertTwoTextOneImageItemByString(ctrl_id, GUILIST_GetTotalItemNum(ctrl_id), text_1, text_2, image_id, user_data);
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void WifiSetSettingListProperty(MMI_WIN_ID_T win_id)
{
    GUI_BOTH_RECT_T both_rect = {0};

    both_rect = MMITHEME_GetWinClientBothRect(win_id);

    GUIAPICTRL_SetBothRect(MMIWIFI_LIST_SETTING_CTRL_ID, &both_rect);

    GUILIST_SetNeedPrgbar(MMIWIFI_LIST_SETTING_CTRL_ID, TRUE);

    GUILIST_SetMaxItem(MMIWIFI_LIST_SETTING_CTRL_ID, MMIWIFI_MAX_TRUSTED_LIST_NUM, FALSE);

    WifiAppendSettingListItem(MMIWIFI_LIST_SETTING_CTRL_ID);
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void WifiAppendSettingListItem(MMI_CTRL_ID_T ctrl_id)
{
    BOOLEAN                 is_auto_connect = FALSE;
    uint16                  text_index = 0;
    uint16                  auto_scan_status = 0;
    uint16                  current_index = 0;
    uint16                  top_index = 0;
    MMI_IMAGE_ID_T          image_id = IMAGE_NULL;
    MMIWIFI_PORTAL_CONFIG_T portal_cfg = {0};
#ifdef WLAN_RESELECT_SUPPORT
    BOOLEAN                 is_Prompt = TRUE;
    MMI_AP_RESELECT         ap_select = MMI_AP_AUTO_RESELCET;
    MMI_GPRSTOWLAN_RESELECT gprs_wlan_select = MMI_GPRSTOWLAN_ALWAYS_INQUIRY;
    MMI_TEXT_ID_T           gprs2wlan_index[] = {TXT_WLAN_ALWAYS_INQUIRY,TXT_WLAN_AUTO_RESELCT,TXT_WLAN_MANUAL_RESELCT};
    MMI_TEXT_ID_T           ap_reselect_index[] = {TXT_WLAN_AUTO_RESELCT,TXT_WLAN_MANUAL_RESELCT};
    MMI_TEXT_ID_T           nowlan_index[] = {TXT_COMMON_PROMPT,TXT_COMMON_NOT_PROMPT};
#endif // WLAN_RESELECT_SUPPORT
#ifdef MMI_WLAN_TAB_STYLE
    BOOLEAN                 is_wlan_on = FALSE;
    MMI_TEXT_ID_T           auto_scan_index[] = {TXT_WIFI_AUTO_SEARCH_OFF,TXT_WIFI_PER_1_MIN,TXT_WIFI_PER_2_MIN,TXT_WIFI_PER_5_MIN,TXT_WIFI_PER_10_MIN};                                                                                //自动搜索:关闭，每1分钟，每2分钟，每5分钟，每10分钟，                                                                               
#endif // MMI_WLAN_TAB_STYLE
    MMI_TEXT_ID_T           on_index[] = {TXT_CLOSE, TXT_COMMON_ENABLE_ON};

    current_index = GUILIST_GetCurItemIndex(ctrl_id);
    top_index = GUILIST_GetTopItemIndex(ctrl_id);
    GUILIST_RemoveAllItems(ctrl_id);

#ifdef MMI_WLAN_TAB_STYLE
    /* WLAN ON OFF */
    is_wlan_on = MMIWIFI_GetIsWlanOn();
    image_id = is_wlan_on ? IMAGE_CHECK_SELECTED_ICON : IMAGE_CHECK_UNSELECTED_ICON;
    text_index = is_wlan_on;
    AppendTwoTextOneImageItem(ctrl_id, TXT_WIFI_WLAN, on_index[text_index], image_id, (uint32)WifiSettingHandleOn);
#endif /* MMI_WLAN_TAB_STYLE */

    /* Auto Connect */
    is_auto_connect = MMIWIFI_GetIsAutoConnect();
    image_id = is_auto_connect ? IMAGE_CHECK_SELECTED_ICON : IMAGE_CHECK_UNSELECTED_ICON;
    text_index = is_auto_connect;
    AppendTwoTextOneImageItem(ctrl_id, TXT_WIFI_WLAN_AUTO_CONNECT, on_index[text_index], image_id, (uint32)WifiSettingHandleAutoConnectOn);

#ifdef WLAN_RESELECT_SUPPORT
    /* GPRS->WLAN */
    gprs_wlan_select= MMIWIFI_GetGPRSToWlanReselect();
    text_index = (uint16)(gprs_wlan_select - MMI_GPRSTOWLAN_ALWAYS_INQUIRY);/*lint !e656*/
    AppendTwoTextOneImageItem(ctrl_id, TXT_WLAN_GPRSTOWLAN_RESELCT, gprs2wlan_index[text_index], IMAGE_COMMON_RIGHT_ARROW, (uint32)WifiSettingOpenGPRSTOWLANReselectWin);

    /* AP Reselect */
    ap_select = MMIWIFI_GetApReselect();
    text_index = (uint16)(ap_select - MMI_AP_AUTO_RESELCET);/*lint !e656*/
    AppendTwoTextOneImageItem(ctrl_id, TXT_WLAN_AP_RESELCT, ap_reselect_index[text_index], IMAGE_COMMON_RIGHT_ARROW, (uint32)WifiSettingOpenAPReselectWin);

    /* Using GPRS when No WLAN */
    is_Prompt = MMIWIFI_GetIsShowNoWlanPrompt();
    text_index = (uint16)is_Prompt;
    AppendTwoTextOneImageItem(ctrl_id, TXT_WLAN_NO_WLAN_USEGPRS, nowlan_index[text_index], IMAGE_COMMON_RIGHT_ARROW, (uint32)WifiSettingOpenInqueryNoWLANWin);

#endif /* WLAN_RESELECT_SUPPORT */

    /* Auto Certificate */
#ifdef CMCC_UI_STYLE
    MMIWIFI_GetPortalConfig(&portal_cfg);
    image_id = portal_cfg.is_auto_login ? IMAGE_CHECK_SELECTED_ICON : IMAGE_CHECK_UNSELECTED_ICON;
    AppendTwoTextOneImageItem(ctrl_id, TXT_WLAN_AUTO_CERTIFICATE, TXT_NULL, image_id, (uint32)WifiSettingHandleSetAutoLogin);
#endif
    /* Auto Search */
    auto_scan_status = MMIWIFI_GetWlanAutoScanStatus();

    text_index = auto_scan_status;
#ifdef MMI_WLAN_TAB_STYLE
    AppendTwoTextOneImageItem(ctrl_id, TXT_WIFI_SEARCH_WLAN,  auto_scan_index[text_index], IMAGE_COMMON_RIGHT_ARROW, (uint32)WifiSettingHandleSearch);
#else

    if (auto_scan_status > 0)
    {
        text_index = 1;
        image_id = IMAGE_CHECK_SELECTED_ICON;   
    }
    else
    {
        text_index = 0;
        image_id = IMAGE_CHECK_UNSELECTED_ICON;
    }

    AppendTwoTextOneImageItem(ctrl_id, TXT_WIFI_SEARCH_WLAN,  on_index[text_index], image_id, (uint32)WifiSettingHandleSearch);
#endif /* MMI_WLAN_TAB_STYLE */

    /* Phone attribute */
    AppendTwoTextOneImageItem(ctrl_id, TXT_WIFI_OWN_ATTRIBUTE, TXT_NULL, IMAGE_COMMON_RIGHT_ARROW, (uint32)WifiSettingOpenOwnAttributeWin);

    /* Proxy */
    AppendTwoTextOneImageItem(ctrl_id, TXT_COMM_PROXY, TXT_NULL, IMAGE_COMMON_RIGHT_ARROW, (uint32)WifiSettingOpenProxyWin);

    /* Reset*/
//    image_id = portal_cfg.is_auto_login ? IMAGE_CHECK_SELECTED_ICON : IMAGE_CHECK_UNSELECTED_ICON;
//    AppendTwoTextOneImageItem(ctrl_id, TXT_RESET_DEFAULT, TXT_NULL, IMAGE_COMMON_RIGHT_ARROW, (uint32)WifiSettingHandleReset);


    GUILIST_SetCurItemIndex(ctrl_id, current_index);
    GUILIST_SetTopItemIndex(ctrl_id, top_index);
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void WifiSettingOpenOwnAttributeWin(void)
{
    MMK_CreateWin((uint32 *)MMIWIFI_OWN_ATTRIBUTE_WIN_TAB,PNULL);
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void WifiSettingOpenProxyWin(void)
{
    MMK_CreateWin((uint32 *)MMIWIFI_PROXY_SETTING_WIN_TAB,PNULL);
}

#ifdef WLAN_RESELECT_SUPPORT        
/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void WifiSettingOpenGPRSTOWLANReselectWin(void)
{
    MMK_CreatePubListWin((uint32*)MMIWIFI_GPRS_TO_WLAN_SET_WIN_TAB, PNULL);
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void WifiSettingOpenAPReselectWin(void)
{
    MMK_CreatePubListWin((uint32*)MMIWIFI_RELECT_AP_SET_WIN_TAB, PNULL);
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void WifiSettingOpenInqueryNoWLANWin(void)
{
    MMK_CreateWin((uint32*)MMIWIFI_PROMPT_NO_WLAN_SET_WIN_TAB, PNULL);
}
#endif /* WLAN_RESELECT_SUPPORT */

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void WifiSettingHandleSetAutoLogin(void)
{
    MMIWIFI_PORTAL_CONFIG_T portal_cfg = {0};

    MMIWIFI_GetPortalConfig(&portal_cfg);

    portal_cfg.is_auto_login = !portal_cfg.is_auto_login;

    MMIWIFI_SetPortalConfig(&portal_cfg);

    MMK_PostMsg(MMIWIFI_SETTING_LIST_CHILD_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void WifiSettingHandleReset(void)
{

}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void WifiSettingHandleOn(void)
{
    BOOLEAN is_wifi_on = TRUE;

    is_wifi_on = MMIWIFI_GetIsWlanOn();

    if (is_wifi_on)
    {
        MMIAPIWIFI_Off();
    }
    else
    {
        if(MMIAPISET_GetFlyMode()&& (FALSE == MMIAPIENG_GetIsFlightModeWifi()))
        {
            MMIPUB_OpenAlertWarningWin(TXT_CLOSE_FLY_MODE_FIRST); 
            return;
        }

        MMIAPIWIFI_On(TRUE);
    }   
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void WifiSettingHandleAutoConnectOn(void)
{
    BOOLEAN is_auto_connect_on  = TRUE;

    is_auto_connect_on = MMIWIFI_GetIsAutoConnect();

    MMIWIFI_SetIsAutoConnect((BOOLEAN)!is_auto_connect_on);

    if (!is_auto_connect_on)
    {
        //Start directly
        MMIWIFI_AutoConnect();
    }

    MMK_PostMsg(MMIWIFI_SETTING_LIST_CHILD_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void WifiSettingHandleSearch(void)
{
#ifdef MMI_WLAN_TAB_STYLE
    MMK_CreatePubListWin((uint32*)MMIWIFI_SEARCH_SET_WIN_TAB, PNULL);
#else
    int32   index_period = 0;

    index_period = MMIWIFI_GetWlanAutoScanStatus();

    index_period = (0 == index_period);

    MMIWIFI_SetWlanAutoScanStatus(index_period);

    if(MMIWIFI_GetIsWlanOn())
    {
        if (0 != index_period)
        {
            MMIWIFI_StartAutoScan();
        }
        else
        {
            MMIWIFI_StopAutoScan();
        }
    }

    MMK_PostMsg(MMIWIFI_SETTING_LIST_CHILD_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
#endif /* MMI_WLAN_TAB_STYLE */
}

#endif /* MMI_GUI_STYLE_TYPICAL */
/****************************************************************************/
//  Description : handle default Accesspoint window
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleDefAPSettingWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"WifiHandleDefAPSettingWinMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_12536_112_2_18_3_10_18_249,(uint8*)"d", msg_id);

    switch (msg_id) {
    case MSG_OPEN_WINDOW:
        MMIWIFI_CreateDefAPSetetingMenu(win_id, MMIWIFI_MENU_DEF_AP_SET_CTRL_ID);
        MMK_SetAtvCtrl(win_id, MMIWIFI_MENU_DEF_AP_SET_CTRL_ID);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        WifiHandleHandleMenuAction(GUIMENU_GetCurNodeId(MMIWIFI_MENU_DEF_AP_SET_CTRL_ID));
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
//  Description : Set Default acess point settingmenu Property
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  MMIWIFI_CreateDefAPSetetingMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    GUI_BOTH_RECT_T both_rect = {0};
    MMI_STRING_T string = {0};

    MMI_TEXT_ID_T text_id[MMIWIFI_DEF_AP_SETTING_MENU_NUM] = {TXT_WIFI_ACTIVE_SERVICE, TXT_WIFI_SUBSCRIBE, TXT_WIFI_PERSONAL_INFO_SETTING, TXT_WIFI_AUTO_CONNECTING_SETTING};
    uint32 node_id[MMIWIFI_DEF_AP_SETTING_MENU_NUM] = {MMIWIFI_DEF_AP_SETTING_NODE_ACTIVE, MMIWIFI_DEF_AP_SETTING_NODE_SUBSCRIBE, MMIWIFI_DEF_AP_SETTING_NODE_PERSONAL_INFO, MMIWIFI_DEF_AP_SETTING_NODE_AUTO_CONNECTING};
    int16 index = 0;
    GUIMENU_DYNA_ITEM_T node_item = {0};
    
    both_rect = MMITHEME_GetWinClientBothRect(win_id);

    //creat dynamic menu
    GUIMENU_CreatDynamic(&both_rect, win_id, ctrl_id, GUIMENU_STYLE_THIRD);

    //Title
    MMI_GetLabelTextByLang(TXT_WIFI_DEFAULT_AP_SETTING, &string);
    GUIMENU_SetMenuTitle(&string, ctrl_id);

    //Softkey
    GUIMENU_SetDynamicMenuSoftkey(ctrl_id, TXT_COMMON_OK, TXT_NULL, STXT_RETURN);

    //Text Id of Active/Deactive
    if (MMIWIFI_GetIsServiceOn()) {
        text_id[0] = TXT_WIFI_DEACTIVE_SERVICE;
    }

    for (index = 0; index < (int16)MMIWIFI_DEF_AP_SETTING_MENU_NUM; index ++) {
        SCI_MEMSET(&string, 0, sizeof(string));
        SCI_MEMSET(&node_item, 0, sizeof(node_item));
        MMI_GetLabelTextByLang(text_id[index], &string);
        node_item.item_text_ptr = &string;
        node_item.select_icon_id = IMAGE_WLAN_SETTINGS_TAB_OFF_ICON;
        GUIMENU_InsertNode(index, node_id[index], 0, &node_item, ctrl_id);
    }
}

/****************************************************************************/
//  Description : Handle menu action
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void WifiHandleHandleMenuAction(uint32 node_id)
{
    //SCI_TRACE_LOW:"WifiHandleHandleMenuAction node_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_12612_112_2_18_3_10_18_250,(uint8*)"d", node_id);
    switch (node_id) {
    case MMIWIFI_DEF_AP_SETTING_NODE_ACTIVE:
        MMIPUB_OpenQueryWinByTextId(TXT_WIFI_QUERY_ACTIVE_SERVICE, IMAGE_PUBWIN_QUERY, PNULL, MMIWIFI_HandleQueryActiveService);;
        break;

    case MMIWIFI_DEF_AP_SETTING_NODE_SUBSCRIBE:
        break;

    case MMIWIFI_DEF_AP_SETTING_NODE_PERSONAL_INFO:
        MMK_CreateWin((uint32*)MMIWIFI_MOD_PERSONAL_INFO_WIN_TAB, PNULL);
        break;

    case  MMIWIFI_DEF_AP_SETTING_NODE_AUTO_CONNECTING:
        MMK_CreateWin((uint32*)MMIWIFI_AUTO_CONNECT_SELECT_WIN_TAB, PNULL);
        break;

    default:
        break;
    }
}

/****************************************************************************/
//  Description : handle change user info window
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  HandleModPersonalInfoWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    LOCAL BOOLEAN is_vavify_win = TRUE;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        is_vavify_win = TRUE;
        GUIEDIT_SetIm(MMIWIFI_EDIT_PASSWORD_CTRL_ID, GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL,GUIIM_TYPE_ABC); /*lint !e655*/
        GUIEDIT_SetIm(MMIWIFI_EDIT_RE_PASSWORD_CTRL_ID, GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL,GUIIM_TYPE_ABC); /*lint !e655*/
        MMK_SetAtvCtrl(win_id, MMIWIFI_EDIT_USER_NAME_CTRL_ID);
        break;

    case MSG_FULL_PAINT:
        MMIWIFI_SetInputInfoProperty(win_id, is_vavify_win);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
        {
            MMI_STRING_T name_string = {0};
            MMI_STRING_T psw_string = {0};
            MMI_STRING_T re_psw_String = {0};

            GUIEDIT_GetString(MMIWIFI_EDIT_USER_NAME_CTRL_ID, &name_string);
            GUIEDIT_GetString(MMIWIFI_EDIT_PASSWORD_CTRL_ID, &psw_string);
            GUIEDIT_GetString(MMIWIFI_EDIT_RE_PASSWORD_CTRL_ID, &re_psw_String);

            if (is_vavify_win)
            {
                if (MMIWIFI_TSTRINGISNULL(name_string) || MMIWIFI_TSTRINGISNULL(psw_string))
                {
                    MMIPUB_OpenAlertWarningWin(TXT_WIFI_INVALID_ACCOUNT);
                }
                else
                {
                    MMIWIFI_ACCOUNT_INFO_T account_info = {0};

                    SCI_MEMCPY(account_info.user_name, name_string.wstr_ptr, MIN(MMIWIFI_USER_NAME_MAX_LEN, name_string.wstr_len))
                    SCI_MEMCPY(account_info.user_psw, psw_string.wstr_ptr, MIN(MMIWIFI_PASSWORD_MAX_LEN, psw_string.wstr_len))
                    if (MMIWIFI_IsvalidAccount(&account_info))
                    {
                        is_vavify_win = FALSE;
                        GUIEDIT_ClearAllStr(MMIWIFI_EDIT_USER_NAME_CTRL_ID);
                        GUIEDIT_ClearAllStr(MMIWIFI_EDIT_PASSWORD_CTRL_ID);
                        MMK_SetAtvCtrl(win_id, MMIWIFI_EDIT_USER_NAME_CTRL_ID);
                        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
                    }
                    else
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_WIFI_INVALID_ACCOUNT);
                    }
                }
            } 
            else
            {
                if (MMIWIFI_TSTRINGISNULL(name_string) || MMIWIFI_TSTRINGISNULL(psw_string) ||MMIWIFI_TSTRINGISNULL(re_psw_String))
                {
                    MMIPUB_OpenAlertWarningWin(TXT_WIFI_INVALID_ACCOUNT);
                }
                else if (0 != MMIAPICOM_Wstrcmp(psw_string.wstr_ptr, re_psw_String.wstr_ptr))
                {
                    MMIPUB_OpenAlertWarningWin(TXT_MSGBOX_PWD_CONFLICT);
                }
                else
                {
#ifdef MMI_GUI_STYLE_TYPICAL
                    MMIPUB_OpenAlertWinByTextId(PNULL, TXT_WIFI_SAVE_OK, 0, IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
#endif
                    MMK_CloseWin(win_id);
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

/****************************************************************************/
//  Description : handle auto connecting select
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E HandleAutoConnectSelectWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    switch(msg_id) {
    case MSG_OPEN_WINDOW:
        MMIWIFI_SetAutoConnectList(MMIWIFI_LIST_AUTO_CONNECT_CTRL_ID);
        MMK_SetAtvCtrl(win_id, MMIWIFI_LIST_AUTO_CONNECT_CTRL_ID);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:    
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
        {
            uint16 list_index = 0;
            MMI_TEXT_ID_T text_id = TXT_NULL;
            BOOLEAN is_on = FALSE;

            list_index = GUILIST_GetCurItemIndex(MMIWIFI_LIST_AUTO_CONNECT_CTRL_ID);

            if (MMI_AUTO_CONNECTING_CHOICE_ON == list_index) {
                is_on = TRUE;
                text_id = TXT_WIFI_AUTO_CONNECT_ON_SUCCESSFULLY;
            } else {
                is_on= FALSE;
                text_id = TXT_WIFI_AUTO_CONNECT_OFF_SUCCESSFULLY;
            }

            MMIWIFI_SetIsAutoConnectingOn(is_on);
#ifdef MMI_GUI_STYLE_TYPICAL
            MMIPUB_OpenAlertWinByTextId(PNULL, text_id, 0, IMAGE_PUBWIN_SUCCESS, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
#endif
            MMK_CloseWin(win_id);
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
//  Description : Set Main Property
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  MMIWIFI_SetInputInfoProperty(MMI_WIN_ID_T win_id, BOOLEAN is_verify_win)
{
    MMI_STRING_T string = {0};


    if (is_verify_win) {
        SCI_MEMSET(&string, 0, sizeof(string));
        MMI_GetLabelTextByLang(TXT_WIFI_ORIGINAL_USER_NAME, &string);
        GUILABEL_SetText(MMIWIFI_LABELT_USER_NAME_CTRL_ID, &string, FALSE);

        SCI_MEMSET(&string, 0, sizeof(string));
        MMI_GetLabelTextByLang(TXT_WIFI_ORIGINAL_PASSWORD, &string);
        GUILABEL_SetText(MMIWIFI_LABELT_PASSWORD_CTRL_ID, &string, FALSE);
        
        GUIFORM_SetChildDisplay(MMIWIFI_MOD_PERSONAL_INFO_FORM_CTRL_ID, MMIWIFI_LABELT_RE_PASSWORD_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
        GUIFORM_SetChildDisplay(MMIWIFI_MOD_PERSONAL_INFO_FORM_CTRL_ID, MMIWIFI_EDIT_RE_PASSWORD_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
    } else {
        SCI_MEMSET(&string, 0, sizeof(string));
        MMI_GetLabelTextByLang(TXT_WIFI_NEW_USER_NAME, &string);
        GUILABEL_SetText(MMIWIFI_LABELT_USER_NAME_CTRL_ID, &string, FALSE);

        SCI_MEMSET(&string, 0, sizeof(string));
        MMI_GetLabelTextByLang(TXT_WIFI_NEW_PASSWORD, &string);
        GUILABEL_SetText(MMIWIFI_LABELT_PASSWORD_CTRL_ID, &string, FALSE);

        SCI_MEMSET(&string, 0, sizeof(string));
        MMI_GetLabelTextByLang(TXT_WIFI_CONFIRM_NEW_PASSWORD, &string);
        GUILABEL_SetText(MMIWIFI_LABELT_RE_PASSWORD_CTRL_ID, &string, FALSE);

        GUIFORM_SetChildDisplay(MMIWIFI_MOD_PERSONAL_INFO_FORM_CTRL_ID, MMIWIFI_LABELT_RE_PASSWORD_CTRL_ID, GUIFORM_CHILD_DISP_NORMAL);
        GUIFORM_SetChildDisplay(MMIWIFI_MOD_PERSONAL_INFO_FORM_CTRL_ID, MMIWIFI_EDIT_RE_PASSWORD_CTRL_ID, GUIFORM_CHILD_DISP_NORMAL);
    }
}


/****************************************************************************/
//  Description : Set Main Property
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL BOOLEAN MMIWIFI_IsvalidAccount(MMIWIFI_ACCOUNT_INFO_T *account_info)
{
    BOOLEAN ret = TRUE;
    MMIWIFI_ACCOUNT_INFO_T saved_info = {0};

    MMIWIFI_GetAccountInfo(&saved_info);

    ret = ret && (0 == MMIAPICOM_Wstrcmp(account_info->user_name, saved_info.user_name));
    ret = ret && (0 == MMIAPICOM_Wstrcmp(account_info->user_psw, saved_info.user_psw));

    return ret;
}

/*****************************************************************************/
//  Description : set auto connecting list
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void MMIWIFI_SetAutoConnectList(MMI_CTRL_ID_T ctrl_id)
{
    uint16 cur_index = 0;

    GUILIST_SetMaxItem(ctrl_id, MMI_AUTO_CONNECTING_CHOICE_MAX, FALSE);

    MMIAPISET_AppendListItemByTextId(TXT_COMMON_ENABLE_ON, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_CLOSE, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);

    cur_index = MMIWIFI_GetIsAutoConnectingOn() ? MMI_AUTO_CONNECTING_CHOICE_ON : MMI_AUTO_CONNECTING_CHOICE_OFF;

    GUILIST_SetSelectedItem(ctrl_id,cur_index,TRUE);
    GUILIST_SetCurItemIndex(ctrl_id,cur_index);
}

/*****************************************************************************/
// Description : Handle query acitve service
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWIFI_HandleQueryActiveService(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    switch(msg_id) {
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        {
            MMI_STRING_T text_str = {0};

            MMI_GetLabelTextByLang(TXT_WIFI_ACTIVING_SERVICE, &text_str);
            MMIPUB_OpenWaitWin(1, &text_str, PNULL, PNULL, MMIWIFI_ACTIVE_SERVICE_WAIT_WIN_ID, IMAGE_NULL, ANIM_PUBWIN_WAIT, WIN_ONE_LEVEL, MMIPUB_SOFTKEY_NONE, PNULL);
            MMIWIFI_ActiveService();
            MMK_CloseWin(win_id);
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
// Description : Acitve CMCC WLAN service
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
LOCAL void MMIWIFI_ActiveService(void)
{
    return;
}

/*****************************************************************************/
// Description : Acitve CMCC WLAN service
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWIFI_ActiveServiceConfirm(void * param)
{
//    MMIPUB_OpenAlertWinByTextId(PNULL, text_id, 0, IMAGE_PUBWIN_SUCCESS, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);

    return MMI_RESULT_TRUE;
}

/****************************************************************************/
//  Description : Wifi Handle Wlan Active Query
//  Global resource dependence : 
//  Author: 
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleWlanActiveQuery(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    //uint16 index = 0;
    
    switch (msg_id)
    {        
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
        MMIAPIWIFI_On(TRUE);
        MMK_CloseWin(win_id);
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMIPUB_HandleQueryWinMsg( win_id, msg_id, param );
        break;
    }
    return recode;
}

/****************************************************************************/
//  Description : handle wlan setting window
//  Global resource dependence : 
//  Author: 
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleWlanSettingWin(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    BOOLEAN             is_on = FALSE;
    uint16              index_period=0;
    MMI_CTRL_ID_T       active_ctrl_id;

    //SCI_TRACE_LOW:"WifiHandleWlanSettingWin win_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_12957_112_2_18_3_10_19_251,(uint8*)"d",win_id);
    
    switch(msg_id) 
    {
    case MSG_OPEN_WINDOW:
        WifiSetWlanWindow();
        MMK_SetAtvCtrl(win_id, MMIWIFI_WLAN_ON_OFF_DROPDOWNLIST_CTRL_ID);
        break;

    case MSG_GET_FOCUS:
        //如果通过其他接口改变状态，在这里更新
        if(MMIWIFI_GetIsWlanOn() != GUIDROPDOWNLIST_GetCurItemIndex(MMIWIFI_WLAN_ON_OFF_DROPDOWNLIST_CTRL_ID))
        {
            GUIDROPDOWNLIST_SetCurItemIndex(MMIWIFI_WLAN_ON_OFF_DROPDOWNLIST_CTRL_ID,(uint16)MMIWIFI_GetIsWlanOn());
            if(!MMIWIFI_GetIsWlanOn())
            {
                GUIFORM_SetChildDisplay(MMIWIFI_WLAN_SETTING_FORMCTRL_ID, MMIWIFI_AUTO_CONNECT_DROPDOWNLIST_CTRL_ID, GUIFORM_CHILD_DISP_GRAY);
                GUIFORM_SetChildDisplay(MMIWIFI_WLAN_SETTING_FORMCTRL_ID, MMIWIFI_SEARCH_PERIOD_DROPDOWNLIST_CTRL_ID, GUIFORM_CHILD_DISP_GRAY);
            }
            else
            {
                GUIFORM_SetChildDisplay(MMIWIFI_WLAN_SETTING_FORMCTRL_ID, MMIWIFI_AUTO_CONNECT_DROPDOWNLIST_CTRL_ID, GUIFORM_CHILD_DISP_NORMAL);
                GUIFORM_SetChildDisplay(MMIWIFI_WLAN_SETTING_FORMCTRL_ID, MMIWIFI_SEARCH_PERIOD_DROPDOWNLIST_CTRL_ID, GUIFORM_CHILD_DISP_NORMAL);
            }
        }
        break;
        
    case MSG_FULL_PAINT:
        break;
        
    case MSG_NOTIFY_DROPDOWNLIST_SELECTED:
        if(MMIWIFI_WLAN_ON_OFF_DROPDOWNLIST_CTRL_ID == MMK_GetActiveCtrlId(win_id))
        {
            is_on =(BOOLEAN)GUIDROPDOWNLIST_GetCurItemIndex(MMIWIFI_WLAN_ON_OFF_DROPDOWNLIST_CTRL_ID);           
            if(MMIAPISET_GetFlyMode()&& (FALSE == MMIAPIENG_GetIsFlightModeWifi()))
            {
                //如果是飞行模式，WLAN开关状态不能改变，且应该为关
                GUIDROPDOWNLIST_SetCurItemIndex(MMIWIFI_WLAN_ON_OFF_DROPDOWNLIST_CTRL_ID, 1);
                MMIPUB_OpenAlertWarningWin(TXT_CLOSE_FLY_MODE_FIRST); 
                break;
            }
            else
            {
                if(is_on != MMIWIFI_GetIsWlanOn())
                {
                    if(is_on)
                    {
                        GUIFORM_SetChildDisplay(MMIWIFI_WLAN_SETTING_FORMCTRL_ID, MMIWIFI_AUTO_CONNECT_DROPDOWNLIST_CTRL_ID, GUIFORM_CHILD_DISP_NORMAL);
                        GUIFORM_SetChildDisplay(MMIWIFI_WLAN_SETTING_FORMCTRL_ID, MMIWIFI_SEARCH_PERIOD_DROPDOWNLIST_CTRL_ID, GUIFORM_CHILD_DISP_NORMAL);

                        MMIAPIWIFI_On(TRUE);
                    }
                    else
                    {
                        GUIFORM_SetChildDisplay(MMIWIFI_WLAN_SETTING_FORMCTRL_ID, MMIWIFI_AUTO_CONNECT_DROPDOWNLIST_CTRL_ID, GUIFORM_CHILD_DISP_GRAY);
                        GUIFORM_SetChildDisplay(MMIWIFI_WLAN_SETTING_FORMCTRL_ID, MMIWIFI_SEARCH_PERIOD_DROPDOWNLIST_CTRL_ID, GUIFORM_CHILD_DISP_GRAY);

                        MMIAPIWIFI_Off();
                    }
                }
            }
        }
        else if(MMIWIFI_AUTO_CONNECT_DROPDOWNLIST_CTRL_ID == MMK_GetActiveCtrlId(win_id))
        {
           is_on =(BOOLEAN)GUIDROPDOWNLIST_GetCurItemIndex(MMIWIFI_AUTO_CONNECT_DROPDOWNLIST_CTRL_ID);    

            if(is_on != MMIWIFI_GetIsAutoConnect())//自动连接
            {
                MMIWIFI_SetIsAutoConnect(is_on);
            }

            if (is_on)
            {
                MMIWIFI_AutoConnect();
            }

        }
        else if(MMIWIFI_SEARCH_PERIOD_DROPDOWNLIST_CTRL_ID == MMK_GetActiveCtrlId(win_id))
        {
            index_period=GUIDROPDOWNLIST_GetCurItemIndex(MMIWIFI_SEARCH_PERIOD_DROPDOWNLIST_CTRL_ID);
            if(index_period!= MMIWIFI_GetWlanAutoScanStatus())//自动搜索状态
            {
                MMIWIFI_SetWlanAutoScanStatus(index_period);
                if(MMIWIFI_GetIsWlanOn())
                {
                    if (0 != index_period)
                    {
                        MMIWIFI_StartAutoScan();
                    }
                    else
                    {
                        MMIWIFI_StopAutoScan();
                    }
                }
            }
        }      
#ifdef WLAN_RESELECT_SUPPORT        
        else if(MMIWIFI_GPRSTOWLAN_RESELECT_DROPDOWNLIST_CTRL_ID == MMK_GetActiveCtrlId(win_id))
        {
           MMI_GPRSTOWLAN_RESELECT  index = MMI_GPRSTOWLAN_ALWAYS_INQUIRY;
           index = (MMI_GPRSTOWLAN_RESELECT)GUIDROPDOWNLIST_GetCurItemIndex(MMIWIFI_GPRSTOWLAN_RESELECT_DROPDOWNLIST_CTRL_ID);
           if(index != MMIWIFI_GetGPRSToWlanReselect())
           {
               MMIWIFI_SetGPRSToWlanReselect(&index);
           }
        }
        else if (MMIWIFI_AP_RESELECT_DROPDOWNLIST_CTRL_ID == MMK_GetActiveCtrlId(win_id))
        {
           MMI_AP_RESELECT  index = MMI_AP_AUTO_RESELCET;
           index = (MMI_AP_RESELECT)GUIDROPDOWNLIST_GetCurItemIndex(MMIWIFI_AP_RESELECT_DROPDOWNLIST_CTRL_ID);
           if(index != MMIWIFI_GetApReselect())
           {
               MMIWIFI_SetApReselect(&index);
           }
        }
        else if(MMIWIFI_NO_WLAN_INQUIRE_DROPDOWNLIST_CTRL_ID == MMK_GetActiveCtrlId(win_id))
        {
            uint16 index = 0;
            BOOLEAN is_prompt = TRUE;

            index = GUIDROPDOWNLIST_GetCurItemIndex(MMIWIFI_NO_WLAN_INQUIRE_DROPDOWNLIST_CTRL_ID);
            is_prompt = (BOOLEAN)(0 == index);
            if(is_prompt != MMIWIFI_GetIsShowNoWlanPrompt())
            {
               MMIWIFI_SetIsShowNoWlanPrompt(is_prompt);
            }
        }
#endif        
    break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        active_ctrl_id = MMK_GetActiveCtrlId(win_id);
        //处理两个LABLE控件，本机属性和代理设置
        if(MMIWIFI_OWN_ATTRIBUTE_LABEL_CTRL_ID==active_ctrl_id)
        {
            MMK_CreateWin((uint32 *)MMIWIFI_OWN_ATTRIBUTE_WIN_TAB,PNULL);
        }
        else if(MMIWIFI_PROXY_SETTING_LABEL_CTRL_ID==active_ctrl_id)
        {
            MMK_CreateWin((uint32 *)MMIWIFI_PROXY_SETTING_WIN_TAB,PNULL);
        }
        break;
        
    case MSG_CTL_OK:
    case MSG_APP_OK:

        active_ctrl_id = MMK_GetActiveCtrlId(win_id);

        if(MMIWIFI_WLAN_ON_OFF_DROPDOWNLIST_CTRL_ID == active_ctrl_id
            ||MMIWIFI_AUTO_CONNECT_DROPDOWNLIST_CTRL_ID == active_ctrl_id
            ||MMIWIFI_SEARCH_PERIOD_DROPDOWNLIST_CTRL_ID == active_ctrl_id
            ||MMIWIFI_GPRSTOWLAN_RESELECT_DROPDOWNLIST_CTRL_ID == active_ctrl_id
            ||MMIWIFI_AP_RESELECT_DROPDOWNLIST_CTRL_ID == active_ctrl_id
            ||MMIWIFI_NO_WLAN_INQUIRE_DROPDOWNLIST_CTRL_ID == active_ctrl_id
        )
        {              
            MMK_SendMsg( active_ctrl_id,MSG_NOTIFY_MIDSK,NULL);
        }
            //处理两个LABLE控件，本机属性和代理设置
        else if(MMIWIFI_OWN_ATTRIBUTE_LABEL_CTRL_ID==active_ctrl_id)
        {
            MMK_CreateWin((uint32 *)MMIWIFI_OWN_ATTRIBUTE_WIN_TAB,PNULL);
        }
        else if(MMIWIFI_PROXY_SETTING_LABEL_CTRL_ID==active_ctrl_id)
        {
            MMK_CreateWin((uint32 *)MMIWIFI_PROXY_SETTING_WIN_TAB,PNULL);
        }
        break;
       
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseParentWin(MMIWIFI_MAIN_PARENT_WIN_ID);
        break;
        
    case MSG_CLOSE_WINDOW:
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return result;
}

/*****************************************************************************/
//  Description : WifiSetWlanWindow
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetWlanWindow(void)
{
    uint32                      i = 0;
    MMI_STRING_T                item_index[5] = {0};
    MMI_TEXT_ID_T               wlan_index[2] = {TXT_CLOSE,TXT_COMMON_ENABLE_ON};
    MMI_TEXT_ID_T               auto_connect_index[2] = {TXT_CLOSE,TXT_COMMON_ENABLE_ON};
    MMI_TEXT_ID_T               auto_scan_index[5] = {TXT_CLOSE,TXT_WIFI_PER_1_MIN,TXT_WIFI_PER_2_MIN,TXT_WIFI_PER_5_MIN,TXT_WIFI_PER_10_MIN};                                                                                //自动搜索:关闭，每1分钟，每2分钟，每5分钟，每10分钟，                                                                               
#ifdef WLAN_RESELECT_SUPPORT
    MMI_TEXT_ID_T               gprs2wlan_index[3] = {TXT_WLAN_ALWAYS_INQUIRY,TXT_WLAN_AUTO_RESELCT,TXT_WLAN_MANUAL_RESELCT};
    MMI_TEXT_ID_T               ap_reselect_index[2] = {TXT_WLAN_AUTO_RESELCT,TXT_WLAN_MANUAL_RESELCT};
    MMI_TEXT_ID_T               nowlan_index[2] = {TXT_COMMON_PROMPT,TXT_COMMON_NOT_PROMPT};
#endif
                                                                            
    //SCI_TRACE_LOW:"WifiSetWlanWindow called"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_13161_112_2_18_3_10_19_252,(uint8*)"");

    //设置label控件显示的字符  
    GUILABEL_SetTextById(MMIWIFI_WLAN_LABEL_CTRL_ID,TXT_WIFI_WLAN,TRUE);
    GUILABEL_SetTextById(MMIWIFI_AUTO_CONNECT_LABEL_CTRL_ID,TXT_WIFI_WLAN_AUTO_CONNECT,TRUE);
    GUILABEL_SetTextById(MMIWIFI_SEARCH_WLAN_LABEL_CTRL_ID,TXT_WIFI_SEARCH_WLAN,TRUE);

    GUILABEL_SetTextById(MMIWIFI_GPRSTOWLAN_RESELECT_LABEL_CTRL_ID,TXT_WLAN_GPRSTOWLAN_RESELCT,TRUE);
    GUILABEL_SetTextById(MMIWIFI_AP_RESELECT_LABEL_CTRL_ID,TXT_WLAN_AP_RESELCT,TRUE);
    GUILABEL_SetTextById(MMIWIFI_NO_WLAN_INQUIRE_LABEL_CTRL_ID,TXT_WLAN_NO_WLAN_USEGPRS,TRUE);
    
    GUILABEL_SetTextById(MMIWIFI_OWN_ATTRIBUTE_LABEL_CTRL_ID,TXT_WIFI_OWN_ATTRIBUTE,TRUE);
    GUILABEL_SetTextById(MMIWIFI_PROXY_SETTING_LABEL_CTRL_ID,TXT_COMM_PROXY  ,TRUE);

    GUIFORM_SetStyle(MMIWIFI_WLAN_SETTING_ONOFF_FORMCTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIWIFI_WLAN_SETTING_AUTO_CONNECT_FORMCTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIWIFI_WLAN_SETTING_SEARCH_WLAN_FORMCTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIWIFI_WLAN_SETTING_ATTRIBUTE_FORMCTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIWIFI_WLAN_SETTING_PROXY_SETTING_FORMCTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIWIFI_WLAN_SETTING_GPRSTOWLAN_RESELECT_FORMCTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIWIFI_WLAN_SETTING_AP_RESELECT_FORMCTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIWIFI_WLAN_SETTING_INQUIRE_SETTING_FORMCTRL_ID,GUIFORM_STYLE_UNIT);
    
    //设置下拉控件的显示项及焦点项
    for (i = 0; i < 2; i++)
    {
        MMI_GetLabelTextByLang(wlan_index[i], &item_index[i]);
    }
    GUIDROPDOWNLIST_AppendItemArray(MMIWIFI_WLAN_ON_OFF_DROPDOWNLIST_CTRL_ID, item_index, 2 );
    GUIDROPDOWNLIST_SetCurItemIndex(MMIWIFI_WLAN_ON_OFF_DROPDOWNLIST_CTRL_ID, (uint16)MMIWIFI_GetIsWlanOn());
 
    for (i = 0; i < 2; i++)
    {
        MMI_GetLabelTextByLang(auto_connect_index[i], &item_index[i]);
    }
    GUIDROPDOWNLIST_AppendItemArray(MMIWIFI_AUTO_CONNECT_DROPDOWNLIST_CTRL_ID, item_index, 2 );
    GUIDROPDOWNLIST_SetCurItemIndex(MMIWIFI_AUTO_CONNECT_DROPDOWNLIST_CTRL_ID, (uint16)MMIWIFI_GetIsAutoConnect());

    for (i= 0; i < 5; i++)
    {
        MMI_GetLabelTextByLang(auto_scan_index[i], & item_index[i]);
    }    
    GUIDROPDOWNLIST_AppendItemArray(MMIWIFI_SEARCH_PERIOD_DROPDOWNLIST_CTRL_ID, item_index, 5 );
    GUIDROPDOWNLIST_SetCurItemIndex(MMIWIFI_SEARCH_PERIOD_DROPDOWNLIST_CTRL_ID, MMIWIFI_GetWlanAutoScanStatus());

#ifdef WLAN_RESELECT_SUPPORT
    for (i = 0; i < 3; i++)
    {
        MMI_GetLabelTextByLang(gprs2wlan_index[i], &item_index[i]);
    }
    GUIDROPDOWNLIST_AppendItemArray(MMIWIFI_GPRSTOWLAN_RESELECT_DROPDOWNLIST_CTRL_ID, item_index, 3 );
    GUIDROPDOWNLIST_SetCurItemIndex(MMIWIFI_GPRSTOWLAN_RESELECT_DROPDOWNLIST_CTRL_ID, (uint16)MMIWIFI_GetGPRSToWlanReselect());
 
    for (i = 0; i < 2; i++)
    {
        MMI_GetLabelTextByLang(ap_reselect_index[i], &item_index[i]);
    }
    GUIDROPDOWNLIST_AppendItemArray(MMIWIFI_AP_RESELECT_DROPDOWNLIST_CTRL_ID, item_index, 2 );
    GUIDROPDOWNLIST_SetCurItemIndex(MMIWIFI_AP_RESELECT_DROPDOWNLIST_CTRL_ID, (uint16)MMIWIFI_GetApReselect());
 
    for (i = 0; i < 2; i++)
    {
        MMI_GetLabelTextByLang(nowlan_index[i], &item_index[i]);
    }
    GUIDROPDOWNLIST_AppendItemArray(MMIWIFI_NO_WLAN_INQUIRE_DROPDOWNLIST_CTRL_ID, item_index, 2 );
    GUIDROPDOWNLIST_SetCurItemIndex(MMIWIFI_NO_WLAN_INQUIRE_DROPDOWNLIST_CTRL_ID, (uint16)!MMIWIFI_GetIsShowNoWlanPrompt());
 #endif
 
    if(!MMIWIFI_GetIsWlanOn())
    {
        GUIFORM_SetChildDisplay(MMIWIFI_WLAN_SETTING_FORMCTRL_ID, MMIWIFI_AUTO_CONNECT_DROPDOWNLIST_CTRL_ID, GUIFORM_CHILD_DISP_GRAY);
        GUIFORM_SetChildDisplay(MMIWIFI_WLAN_SETTING_FORMCTRL_ID, MMIWIFI_SEARCH_PERIOD_DROPDOWNLIST_CTRL_ID, GUIFORM_CHILD_DISP_GRAY);
    }
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiSaveOwnAttributeSettings(MMI_WIN_ID_T win_id)
{
#ifdef MMI_GUI_STYLE_TYPICAL
     BOOLEAN IP_type=0;
#endif
     uint32 temp_IP,temp_GateWay,temp_Submask,temp_DNS1,temp_DNS2 = 0;
     MMIWIFI_PROFILE_T *profile_ptr = PNULL;
     uint32 total_num=0;
     uint32 i=0;
     char *ip_ptr = PNULL;
     TCPIP_NETIF_IPADDR_T ip_addr = {0};
     uint32  ret = 0;

     //保存IP类型信息到NV
#ifndef MMI_GUI_STYLE_TYPICAL
     MMIWIFI_SetIPType(s_ip_type);
#else
     IP_type=(BOOLEAN)GUIDROPDOWNLIST_GetCurItemIndex(MMIWIFI_IP_TYPE_DROPDOWNLIST_CTRL_ID);
     MMIWIFI_SetIPType(IP_type);
#endif /* MMI_GUI_STYLE_TYPICAL */

     //依次保存各项内容
     temp_IP=GUIEDIT_GetIP(MMIWIFI_IP_DATA_EDIT_CTRL_ID);
     MMIWIFI_SetIPData(temp_IP);
     temp_GateWay=GUIEDIT_GetIP(MMIWIFI_GATEWAY_DATA_EDIT_CTRL_ID);
     MMIWIFI_SetGateWay(temp_GateWay);
     temp_Submask=GUIEDIT_GetIP(MMIWIFI_SUBMASK_DATA_EDIT_CTRL_ID);
     MMIWIFI_SetSubMask(temp_Submask);
     temp_DNS1=GUIEDIT_GetIP(MMIWIFI_DNS1_DATA_EDIT_CTRL_ID);
     MMIWIFI_SetDNS1(temp_DNS1);
     temp_DNS2=GUIEDIT_GetIP(MMIWIFI_DNS2_DATA_EDIT_CTRL_ID);
     MMIWIFI_SetDNS2(temp_DNS2);

#ifndef MMI_GUI_STYLE_TYPICAL
     MMIWIFI_SetIPType(s_ip_type);
    if (s_ip_type)
#else
     if(IP_type)
#endif /* MMI_GUI_STYLE_TYPICAL */
     {//本机属性改变后，若设置为静态IP，需要更新所有PROFILE的ip_get
        total_num = MMIWIFI_GetProfileNum();
        for(i=0;i<total_num;i++)
        {
            uint16 ip_len = 0;

            profile_ptr = MMIWIFI_GetProfileByIndex(i);

            if (PNULL == profile_ptr)
            {
                continue;
            }

            profile_ptr->ip_get.ip_get_type = MMIWIFI_IPGET_SELFDEFINED;//@discuss 0105

            ip_ptr = inet_ntoa(ntohl(temp_IP));
            if (PNULL != ip_ptr)
            {
                SCI_MEMSET(profile_ptr->ip_get.ip.ip_addr,0,sizeof((profile_ptr->ip_get.ip.ip_addr)));
                ip_len = strlen(ip_ptr);
                strncpy(profile_ptr->ip_get.ip.ip_addr, ip_ptr, MIN(ip_len, MMIWIFI_IP_LEN));
            }

            ip_ptr = inet_ntoa(ntohl(temp_GateWay));
            if (PNULL != ip_ptr)
            {
                SCI_MEMSET(profile_ptr->ip_get.ip.default_gateway,0,sizeof((profile_ptr->ip_get.ip.default_gateway)));
                ip_len = strlen(ip_ptr);
                strncpy(profile_ptr->ip_get.ip.default_gateway, ip_ptr, MIN(ip_len, MMIWIFI_IP_LEN));
            }

            ip_ptr = inet_ntoa(ntohl(temp_Submask));
            if (PNULL != ip_ptr)
            {
                SCI_MEMSET(profile_ptr->ip_get.ip.subnet_mask,0,sizeof((profile_ptr->ip_get.ip.subnet_mask)));
                ip_len = strlen(ip_ptr);
                strncpy(profile_ptr->ip_get.ip.subnet_mask, ip_ptr, MIN(ip_len, MMIWIFI_IP_LEN));
            }

            ip_ptr = inet_ntoa(ntohl(temp_DNS1));
            if (PNULL != ip_ptr)
            {
                SCI_MEMSET(profile_ptr->ip_get.ip.first_dns,0,sizeof((profile_ptr->ip_get.ip.first_dns)));
                ip_len = strlen(ip_ptr);
                strncpy(profile_ptr->ip_get.ip.first_dns, ip_ptr, MIN(ip_len, MMIWIFI_IP_LEN));
            }

            ip_ptr = inet_ntoa(ntohl(temp_DNS2));
            if (PNULL != ip_ptr)
            {
                SCI_MEMSET(profile_ptr->ip_get.ip.second_dns,0,sizeof((profile_ptr->ip_get.ip.second_dns)));
                ip_len = strlen(ip_ptr);
                strncpy(profile_ptr->ip_get.ip.second_dns, ip_ptr, MIN(ip_len, MMIWIFI_IP_LEN));
            }

            inet_aton(profile_ptr->ip_get.ip.ip_addr,&(ip_addr.ipaddr)); 
            inet_aton(profile_ptr->ip_get.ip.subnet_mask,&(ip_addr.snmask));
            inet_aton(profile_ptr->ip_get.ip.default_gateway,&(ip_addr.gateway));
            inet_aton(profile_ptr->ip_get.ip.first_dns,&(ip_addr.dns1));
            inet_aton(profile_ptr->ip_get.ip.second_dns,&(ip_addr.dns2));

            //SCI_TRACE_LOW:"WIFI:info:MMI:ip address   - %s"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_13340_112_2_18_3_10_20_253,(uint8*)"s", profile_ptr->ip_get.ip.ip_addr);
            //SCI_TRACE_LOW:"WIFI:info:MMI:subnet mask  - %s"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_13341_112_2_18_3_10_20_254,(uint8*)"s", profile_ptr->ip_get.ip.subnet_mask);
            //SCI_TRACE_LOW:"WIFI:info:MMI:gateway      - %s"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_13342_112_2_18_3_10_20_255,(uint8*)"s", profile_ptr->ip_get.ip.default_gateway);
            //SCI_TRACE_LOW:"WIFI:info:MMI:first dns    - %s"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_13343_112_2_18_3_10_20_256,(uint8*)"s", profile_ptr->ip_get.ip.first_dns);
            //SCI_TRACE_LOW:"WIFI:info:MMI:second dns   - %s"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_13344_112_2_18_3_10_20_257,(uint8*)"s", profile_ptr->ip_get.ip.second_dns);

            ret = TCPIP_SetIpAddress(&ip_addr, MMIAPIWIFI_GetNetId());
            if (ret != TCPIP_ERROR_OK)
            {
                //SCI_TRACE_LOW:"mmiwifi error T_P_APP WifiHandleSuppSigConnectCnf: TCPIP_SetIpAddress failed(%d)!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_13349_112_2_18_3_10_20_258,(uint8*)"d", ret);
            }
        }
    }
    else
    {//若设置为动态IP，需要更新所有PROFILE的ip_get.ip_get_type
        total_num = MMIWIFI_GetProfileNum();
        for(i=0;i<total_num;i++)
        {
            profile_ptr = MMIWIFI_GetProfileByIndex(i);

            if (PNULL != profile_ptr)
            {
                profile_ptr->ip_get.ip_get_type = MMIWIFI_IPGET_AUTO;//@discuss 0105
            }
        }
    }
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiSaveProxySettings(MMI_WIN_ID_T win_id)
{
#ifdef MMI_GUI_STYLE_TYPICAL
    BOOLEAN index=0;
#endif
    MMIWIFI_PROXY_PASSWORD_E      password = {0};
    uint16 port = 0;
    uint32 max_port = 0;
    char temp_port[6] = {0};
    MMIWIFI_PROXY_NAME_E    name = {0};
    MMI_STRING_T temp_string={0};
    char url[MMIWIFI_URL_MAX_LEN + 1] = {0};
    
    if(MMIWIFI_PROXY_REJECTLIST_LABEL_CTRL_ID == MMK_GetActiveCtrlId(win_id))
    {
        MMK_CreateWin((uint32 *)MMIWIFI_PROXY_REJECTLIST_WIN_TAB,PNULL);
        return;
    }

#ifndef MMI_GUI_STYLE_TYPICAL
    MMIWIFI_SetProxyQuery(s_is_use_proxy);
#else
    index=(BOOLEAN)GUIDROPDOWNLIST_GetCurItemIndex(MMIWIFI_PROXY_QUERY_DROPDOWNLIST_CTRL_ID);
    MMIWIFI_SetProxyQuery(index);
#endif /* MMI_GUI_STYLE_TYPICAL */
    //保存是否使用代理信息到NV

    //保存代理IP
    GUIEDIT_GetString(MMIWIFI_PROXY_IP_DATA_EDIT_CTRL_ID, &temp_string); 
    GUI_WstrToUTF8((uint8 *)url, MMIWIFI_URL_MAX_LEN, temp_string.wstr_ptr, temp_string.wstr_len);
    MMIWIFI_SetProxyURL(url);

    //保存代理端口
    GUIEDIT_GetString(MMIWIFI_PROXY_PORT_EDIT_CTRL_ID, &temp_string); 
    MMIAPICOM_WstrToStr(temp_string.wstr_ptr, (uint8 *)temp_port);

    max_port = (uint32)atoi(temp_port);
    port = MIN(MMIWIFI_MAX_PORT_NUM, max_port);
    //SCI_TRACE_LOW:"WLAN port is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_13410_112_2_18_3_10_20_259,(uint8*)"d",port);
    MMIWIFI_SetProxyPort((uint16 *)&port);
    //保存代理用户名
    GUIEDIT_GetString(MMIWIFI_PROXY_NAME_EDIT_CTRL_ID, &temp_string); 
    MMIAPICOM_Wstrcpy(name.proxy_name,temp_string.wstr_ptr);
    MMIWIFI_SetProxyName(&name);
    //保存代理密码
    GUIEDIT_GetString(MMIWIFI_PROXY_PASSWORD_EDIT_CTRL_ID,&temp_string); 
    MMIAPICOM_Wstrcpy(password.proxy_password,temp_string.wstr_ptr);
    MMIWIFI_SetProxyPassword(&password);
}

/****************************************************************************/
//  Description : handle own attribute window
//  Global resource dependence : 
//  Author: 
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleOwnAttributeSettingWin(
                                                     MMI_WIN_ID_T    win_id, 
                                                     MMI_MESSAGE_ID_E   msg_id, 
                                                     DPARAM             param
                                                     )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    
    //SCI_TRACE_LOW:"WifiHandleWlanSettingWin win_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_13434_112_2_18_3_10_20_260,(uint8*)"d",win_id);
    
    switch(msg_id) 
    {        
    case MSG_OPEN_WINDOW:
#ifndef MMI_GUI_STYLE_TYPICAL
        s_ip_type = MMIWIFI_GetIPType();
        WifiSetOwnAttributeWindow();
        MMK_SetAtvCtrl(win_id, MMIWIFI_LIST_IP_TYPE_CTRL_ID);
#else
        WifiSetOwnAttributeWindow();
        MMK_SetAtvCtrl(win_id, MMIWIFI_IP_TYPE_DROPDOWNLIST_CTRL_ID);
#endif /* MMI_GUI_STYLE_TYPICAL */
        break;

    case MSG_NOTIFY_DROPDOWNLIST_SELECTED:
         if(MMIWIFI_STATUS_CONNECTED == MMIAPIWIFI_GetStatus())
         {
             MMIPUB_OpenQueryWinByTextId(TXT_WLAN_IS_DELETE_CONNECTED,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
         }
         else
         {
             WifiOwnAttribute_SetChildCtrlDisplay();
            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
         }
        break;

   case MSG_PROMPTWIN_OK:
            {
                MMIWIFI_PROFILE_T*      profile_ptr = PNULL;
                uint32 num = 0;
                MMIPUB_CloseQuerytWin(PNULL);
                num = MMIWIFI_GetProfileNum();
                while(num != 0)
                {
                    profile_ptr = MMIWIFI_GetProfileByIndex(num-1);
                    if (PNULL != profile_ptr && (profile_ptr->state != MMIWIFI_PROFILE_STATE_DISCONNECTED) &&(profile_ptr->state != MMIWIFI_PROFILE_STATE_DISCONNECTING))
                    {
                        MMIWIFI_DisConnect(profile_ptr);
                    }
                    num --;
                }
#ifndef MMI_GUI_STYLE_TYPICAL
                s_ip_type = !s_ip_type;
                WifiSetOwnAttributeWindow();
#else
                WifiOwnAttribute_SetChildCtrlDisplay();
#endif /* MMI_GUI_STYLE_TYPICAL */

                MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
            }
            break;

    case MSG_PROMPTWIN_CANCEL:
       {
            GUIDROPDOWNLIST_SetCurItemIndex(MMIWIFI_IP_TYPE_DROPDOWNLIST_CTRL_ID,(uint16)MMIWIFI_GetIPType());
            WifiOwnAttribute_SetChildCtrlDisplay();
            MMIPUB_CloseQuerytWin(PNULL);
            break;
       }
#ifndef MMI_GUI_STYLE_TYPICAL
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        {
            MMI_CTRL_ID_T ctrl_id = 0;

            if (PNULL != param)
            {
		        ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;

                if (MSG_CTL_MIDSK == msg_id)
                {
                    ctrl_id = MMK_GetActiveCtrlId(win_id);
                }

                if (ctrl_id == MMIWIFI_LIST_IP_TYPE_CTRL_ID)
                {
                     if(MMIWIFI_STATUS_CONNECTED == MMIAPIWIFI_GetStatus())
                     {
                        MMIPUB_OpenQueryWinByTextId(TXT_WLAN_IS_DELETE_CONNECTED, IMAGE_PUBWIN_QUERY, PNULL, PNULL);
                     }
                     else
                     {
                        s_ip_type = !s_ip_type;
                        WifiSetOwnAttributeWindow();
                        MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
                     }
                }
                else if (ctrl_id == MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID)
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
#else
#endif /* MMI_GUI_STYLE_TYPICAL */

    case MSG_CTL_OK:
    case MSG_APP_OK:
        WifiSaveOwnAttributeSettings(win_id);
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

#ifndef MMI_GUI_STYLE_TYPICAL
    case MSG_CLOSE_WINDOW:
        s_ip_type = MMIWIFI_GetIPType();
        break;
#endif

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return result;
}

/*****************************************************************************/
//  Description : wlan set own attribute window
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetOwnAttributeWindow(void)
{
#ifndef MMI_GUI_STYLE_TYPICAL
    MMI_IMAGE_ID_T              image_id = IMAGE_NULL;
#else
    uint32                      i = 0;
    MMI_STRING_T                item_index[2] = {0};
    MMI_TEXT_ID_T               wep_index[2] = {TXT_WIFI_IP_AUTO,TXT_WIFI_IP_SELFDEF};    
#endif /* MMI_GUI_STYLE_TYPICAL */

    //SCI_TRACE_LOW:"WifiSetOwnAttributeWindow called"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_13578_112_2_18_3_10_20_261,(uint8*)"");
    
    GUIFORM_SetStyle(MMIWIFI_CHILD_FORM_STATIC_IP_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIWIFI_CHILD_FORM_IP_ADDRESS_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIWIFI_CHILD_FORM_MAC_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIWIFI_CHILD_FORM_DNS1_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIWIFI_CHILD_FORM_DNS2_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIWIFI_CHILD_FORM_MASK_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIWIFI_CHILD_FORM_GATEWAY_CTRL_ID,GUIFORM_STYLE_UNIT);

#ifndef MMI_GUI_STYLE_TYPICAL
    GUIFORM_SetType(MMIWIFI_OWN_ATTRIBUTE_FORMCTRL_ID, GUIFORM_TYPE_TP);
//    GUISETLIST_SetFontAlign(MMIWIFI_LIST_IP_TYPE_CTRL_ID, ALIGN_LVMIDDLE);
    GUIEDIT_SetAlign(MMIWIFI_IP_DATA_EDIT_CTRL_ID, ALIGN_LVMIDDLE);
    GUIEDIT_SetAlign(MMIWIFI_GATEWAY_DATA_EDIT_CTRL_ID, ALIGN_LVMIDDLE);
    GUIEDIT_SetAlign(MMIWIFI_SUBMASK_DATA_EDIT_CTRL_ID, ALIGN_LVMIDDLE);
    GUIEDIT_SetAlign(MMIWIFI_DNS1_DATA_EDIT_CTRL_ID, ALIGN_LVMIDDLE);
    GUIEDIT_SetAlign(MMIWIFI_DNS2_DATA_EDIT_CTRL_ID, ALIGN_LVMIDDLE);
#endif

    //设置label控件显示的字符  
#ifdef MMI_GUI_STYLE_TYPICAL
    GUILABEL_SetTextById(MMIWIFI_IP_TYPE_LABEL_CTRL_ID,TXT_WIFI_IP_TYPE,FALSE);
#endif /* MMI_GUI_STYLE_TYPICAL */
    GUILABEL_SetTextById(MMIWIFI_IP_LABEL_CTRL_ID,TXT_COMM_WIFI_IP_ADDRESS,FALSE);
    GUILABEL_SetTextById(MMIWIFI_MAC_LABEL_CTRL_ID,TXT_WIFI_MAC,FALSE);
    GUILABEL_SetTextById(MMIWIFI_GATEWAY_LABEL_CTRL_ID,TXT_COMMON_GATEWAY,FALSE);
    GUILABEL_SetTextById(MMIWIFI_SUBMASK_LABEL_CTRL_ID,TXT_WIFI_SUBMASK,FALSE);
    GUILABEL_SetTextById(MMIWIFI_DNS1_LABEL_CTRL_ID,TXT_WIFI_DNS1,FALSE);
    GUILABEL_SetTextById(MMIWIFI_DNS2_LABEL_CTRL_ID,TXT_WIFI_DNS2,FALSE);
    
#ifndef MMI_GUI_STYLE_TYPICAL
    GUILIST_SetSlideState(MMIWIFI_LIST_IP_TYPE_CTRL_ID, FALSE);
    GUILIST_SetMaxItem(MMIWIFI_LIST_IP_TYPE_CTRL_ID, 1, FALSE);
    GUILIST_RemoveAllItems(MMIWIFI_LIST_IP_TYPE_CTRL_ID);
    image_id = s_ip_type ? IMAGE_CHECK_SELECTED_ICON : IMAGE_CHECK_UNSELECTED_ICON;
    AppendTwoTextOneImageItem(MMIWIFI_LIST_IP_TYPE_CTRL_ID, TXT_WIFI_IP_SELFDEF, TXT_NULL, image_id, 0);
#else
    //设置下拉控件的显示项及焦点项
    for (i = 0; i < 2; i++)
    {
        MMI_GetLabelTextByLang(wep_index[i], &item_index[i]);
    }
    GUIDROPDOWNLIST_AppendItemArray(MMIWIFI_IP_TYPE_DROPDOWNLIST_CTRL_ID, item_index, 2 );
    GUIDROPDOWNLIST_SetCurItemIndex(MMIWIFI_IP_TYPE_DROPDOWNLIST_CTRL_ID, (uint16)MMIWIFI_GetIPType());
#endif /* MMI_GUI_STYLE_TYPICAL */
    //处理IP地址
    WifiOwnAttribute_DisplayEditIPCtrlText(MMIWIFI_IP_DATA_EDIT_CTRL_ID);
    //处理MAC地址
    WifiOwnAttribute_DisplayEditIPCtrlText(MMIWIFI_MAC_DATA_LABEL_CTRL_ID);
    //处理网关
    WifiOwnAttribute_DisplayEditIPCtrlText(MMIWIFI_GATEWAY_DATA_EDIT_CTRL_ID);
    //处理子网掩码
    WifiOwnAttribute_DisplayEditIPCtrlText(MMIWIFI_SUBMASK_DATA_EDIT_CTRL_ID);
    //处理DNS1
    WifiOwnAttribute_DisplayEditIPCtrlText(MMIWIFI_DNS1_DATA_EDIT_CTRL_ID);
    //处理DNS2
    WifiOwnAttribute_DisplayEditIPCtrlText(MMIWIFI_DNS2_DATA_EDIT_CTRL_ID);
    //设置子控件是否置灰
    WifiOwnAttribute_SetChildCtrlDisplay();
    
}

/*****************************************************************************/
//  Description : handle display Edit IP ctrl text in own attribute window
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void WifiOwnAttribute_DisplayEditIPCtrlText(MMI_CTRL_ID_T   ctrl_id)
{
    uint32         temp=0;
    uint8 mac_addr[6] = {0};
    BOOLEAN iptype = TRUE;

    iptype = MMIWIFI_GetIPType();

    switch(ctrl_id)
    {
    case MMIWIFI_IP_DATA_EDIT_CTRL_ID:
        if (iptype)
        {
            temp = MMIWIFI_GetIPData();
        }
        else
        {
            temp = s_ui_context.addrs.ipaddr;
        }
        break;

    case MMIWIFI_GATEWAY_DATA_EDIT_CTRL_ID:
        if (iptype)
        {
            temp = MMIWIFI_GetGateWay();
        }
        else
        {
            temp = s_ui_context.addrs.gateway;
        }
        break;

    case MMIWIFI_SUBMASK_DATA_EDIT_CTRL_ID:
        if (iptype)
        {
            temp = MMIWIFI_GetSubMask();
        }
        else
        {
            temp = s_ui_context.addrs.snmask;
        }

        break;

    case MMIWIFI_DNS1_DATA_EDIT_CTRL_ID:
        if (iptype)
        {
            temp = MMIWIFI_GetDNS1();
        }
        else
        {

            temp = s_ui_context.addrs.dns1;
        }

        break;

    case MMIWIFI_DNS2_DATA_EDIT_CTRL_ID:
        if (iptype)
        {
            temp = MMIWIFI_GetDNS2();
        }
        else
        {
            temp = s_ui_context.addrs.dns2;
        }

        break;

    case MMIWIFI_PROXY_IP_DATA_EDIT_CTRL_ID:
        {
            wchar   wchar_url[MMIWIFI_URL_MAX_LEN + 1] = {0};
            char    char_url[MMIWIFI_URL_MAX_LEN + 1] = {0};
    
            MMIWIFI_GetProxyURL(char_url, MMIWIFI_URL_MAX_LEN);
            GUI_UTF8ToWstr(wchar_url, MMIWIFI_URL_MAX_LEN, (uint8 *)char_url, SCI_STRLEN(char_url));
            GUIEDIT_SetString(ctrl_id, wchar_url, MMIAPICOM_Wstrlen(wchar_url)); 
        }
        return;

    case MMIWIFI_MAC_DATA_LABEL_CTRL_ID:
        if (WIFISUPP_GetMac(mac_addr))
        {
            char temp_buf[MMIWIFI_DETAIL_TEMP_LEN + 1] = {0};
            wchar temp_wchar_buf[MMIWIFI_DETAIL_TEMP_LEN + 1] = {0};
            uint16 len = 0;
            MMI_STRING_T label_str = {0};

            CFL_Snprintf(temp_buf, MMIWIFI_DETAIL_TEMP_LEN, "%02x:%02x:%02x:%02x:%02x:%02x", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);

            len = SCI_STRLEN(temp_buf);
            MMI_STRNTOWSTR(temp_wchar_buf, MMIWIFI_DETAIL_TEMP_LEN, (uint8 *)temp_buf, MMIWIFI_DETAIL_TEMP_LEN, len);

            label_str.wstr_ptr = (wchar *)temp_wchar_buf;
            label_str.wstr_len = len;

            GUILABEL_SetText(ctrl_id, &label_str, FALSE);
        }
        break;
    default:
        break;
    }

    //设置控件的显示值
    if (MMIWIFI_MAC_DATA_LABEL_CTRL_ID != ctrl_id)
    {
        GUIEDIT_SetIP(ctrl_id,temp);
    }
}

/****************************************************************************/
//  Description : handle MAC setting window
//  Global resource dependence : 
//  Author: 
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleMacWinMsg_OwnAttribute(
                                                     MMI_WIN_ID_T    win_id, 
                                                     MMI_MESSAGE_ID_E   msg_id, 
                                                     DPARAM             param
                                                     )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        break;
        
    case MSG_CTL_OK:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CLOSE_WINDOW:
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return result;
    
}

/****************************************************************************/
//  Description : handle proxy setting window
//  Global resource dependence : 
//  Author: 
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleProxySettingWin(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              )
{    
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    
    //SCI_TRACE_LOW:"WifiHandleProxySettingWin win_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_13805_112_2_18_3_10_21_262,(uint8*)"d",win_id);
    
    switch(msg_id) 
    {        
    case MSG_OPEN_WINDOW:
#ifndef MMI_GUI_STYLE_TYPICAL
        s_is_use_proxy = MMIWIFI_GetProxyQuery();
        GUIEDIT_SetPasswordStyle(MMIWIFI_PROXY_PASSWORD_EDIT_CTRL_ID, GUIEDIT_PASSWORD_STYLE_ALL);
        WifiSetProxySettingWindow();        
        GUIEDIT_SetIm(MMIWIFI_PROXY_PASSWORD_EDIT_CTRL_ID, GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL,GUIIM_TYPE_ABC); /*lint !e655*/
        MMK_SetAtvCtrl(win_id, MMIWIFI_LIST_PROXY_CTRL_ID);
#else
        GUIEDIT_SetPasswordStyle(MMIWIFI_PROXY_PASSWORD_EDIT_CTRL_ID, GUIEDIT_PASSWORD_STYLE_ALL);
        WifiSetProxySettingWindow();        
        GUIEDIT_SetIm(MMIWIFI_PROXY_PASSWORD_EDIT_CTRL_ID, GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL,GUIIM_TYPE_ABC); /*lint !e655*/
        MMK_SetAtvCtrl(win_id, MMIWIFI_PROXY_QUERY_DROPDOWNLIST_CTRL_ID);
#endif /* MMI_GUI_STYLE_TYPICAL */

        break;
        
    case MSG_NOTIFY_DROPDOWNLIST_SELECTED:
        WifiProxy_SetChildCtrlDisplay();
        MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
#ifndef MMI_GUI_STYLE_TYPICAL
        {
            MMI_CTRL_ID_T ctrl_id = 0;

            if (PNULL != param)
            {
		        ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;

                if (MSG_CTL_MIDSK == msg_id)
                {
                    ctrl_id = MMK_GetActiveCtrlId(win_id);
                }

                if (ctrl_id == MMIWIFI_LIST_PROXY_CTRL_ID)
                {
                    s_is_use_proxy = !s_is_use_proxy;

                    WifiSetProxySettingWindow();      
                    MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
                }
                else if (ctrl_id == MMIWIFI_PROXY_REJECTLIST_LABEL_CTRL_ID)
                {
                    if (s_is_use_proxy)
                    {
                        MMK_CreateWin((uint32 *)MMIWIFI_PROXY_REJECTLIST_WIN_TAB,PNULL);
                    }
                }
                else if (ctrl_id == MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID)
                {
                    MMK_PostMsg(win_id, MSG_CTL_OK, PNULL, 0);
                }
                else if (ctrl_id == MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID)
                {
                    MMK_PostMsg(win_id, MSG_CTL_CANCEL, PNULL, 0);
                }
            }
        }
#else
        if(MMIWIFI_PROXY_REJECTLIST_LABEL_CTRL_ID == MMK_GetActiveCtrlId(win_id))
        {
            MMK_CreateWin((uint32 *)MMIWIFI_PROXY_REJECTLIST_WIN_TAB,PNULL);
        }
#endif /* MMI_GUI_STYLE_TYPICAL */
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
        {
            MMI_STRING_T proxy_url = {0};

            GUIEDIT_GetString(MMIWIFI_PROXY_IP_DATA_EDIT_CTRL_ID, &proxy_url);

            if (0 == proxy_url.wstr_len)
            {
                MMIPUB_OpenAlertWarningWin(TXT_WLAN_INVALID_PROXY);
                break;
            }

            WifiSaveProxySettings(win_id);
            MMK_CloseWin(win_id);
        }
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
#ifndef MMI_GUI_STYLE_TYPICAL
    case MSG_CLOSE_WINDOW:
        s_is_use_proxy = MMIWIFI_GetProxyQuery();
        break;
#endif /* MMI_GUI_STYLE_TYPICAL */
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return result;
}

/****************************************************************************/
//  Description : handle proxy setting reject list window
//  Global resource dependence : 
//  Author: 
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleProxyRejectListWin(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"WifiHandleProxyRejectListWin win_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_13927_112_2_18_3_10_21_263,(uint8*)"d",win_id);
    
    switch(msg_id) 
    {        
    case MSG_OPEN_WINDOW:
        WifiSetRejectListWindow();
#ifndef MMI_GUI_STYLE_TYPICAL
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
        MMK_SetAtvCtrl(win_id, MMIWIFI_LIST_REJECT_LIST_CTRL_ID);
        break;

    case MSG_FULL_PAINT:
        WifiUpdateRejectList(MMIWIFI_LIST_REJECT_LIST_CTRL_ID);
        if(0 == GUILIST_GetTotalItemNum(MMIWIFI_LIST_REJECT_LIST_CTRL_ID))
        {
            GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
        }
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        MMK_CreateWin((uint32 *)MMIWIFI_REJECTLIST_EDIT_WIN_TAB, PNULL);//编辑窗口
        break;
        
#ifndef MMI_GUI_STYLE_TYPICAL
    case MSG_CTL_LIST_LONGOK:
            MMK_CreateWin((uint32*)MMIWIFI_REJECT_LIST_POP_WIN_TAB, PNULL);
        break;
#endif /* MMI_GUI_STYLE_TYPICAL */

    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_MENU:
        MMK_CreateWin((uint32 *)MMIWIFI_REJECTLIST_POPMENU_WIN_TAB, PNULL);//选项菜单
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

/****************************************************************************/
//  Description : WifiUpdateTrustedList
//  Global resource dependence : 
//  Author:
//  Note: 
/****************************************************************************/
LOCAL void  WifiUpdateRejectList(MMI_CTRL_ID_T ctrl_id)
{
    MMIWIFI_PROXY_REJECTLIST_E rejectlist = {0};
    uint32 i = 0;
    GUILIST_ITEM_DATA_T  item_data = {0};
    GUILIST_ITEM_T       item_t = {0};
    wchar url_name[MMIWIFI_URL_MAX_LEN+1] = {0};
    uint16 url_len = 0;
    uint16 proxy_rejectlist_len = 0;
    uint16 current_index = 0;
    uint16 top_index = 0;

    current_index = GUILIST_GetCurItemIndex(ctrl_id);
    top_index = GUILIST_GetTopItemIndex(ctrl_id);
    GUILIST_RemoveAllItems(ctrl_id);
    MMIWIFI_GetProxyRejectList(&rejectlist);
    for(i=0;i<MMIWIFI_MAX_REJECTLIST_NUM;i++)
    {
        if (0 == strlen(rejectlist.proxy_rejectlist[i]))
        {
            break;
        }
        SCI_MEMSET(url_name, 0, sizeof(url_name));
        
        item_data.softkey_id[0] = STXT_OPTION;
        item_data.softkey_id[1] = TXT_EDIT;
        item_data.softkey_id[2] = STXT_RETURN;

        proxy_rejectlist_len = strlen(rejectlist.proxy_rejectlist[i]);
        
        MMI_STRNTOWSTR(url_name, MMIWIFI_URL_MAX_LEN, (uint8 *)rejectlist.proxy_rejectlist[i], proxy_rejectlist_len, MIN(MMIWIFI_URL_MAX_LEN, proxy_rejectlist_len));
        url_len = MMIAPICOM_Wstrlen(url_name);
        item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[0].item_data.text_buffer.wstr_len = (uint16)url_len;
        item_data.item_content[0].item_data.text_buffer.wstr_ptr = (wchar *)url_name;
        item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT;
        item_t.item_data_ptr = &item_data;

        if (!GUILIST_AppendItem(ctrl_id, &item_t))
        {
            SCI_TRACE_LOW("WifiCreateRadioListByTextId failed");
        }
    }
    GUILIST_SetCurItemIndex(ctrl_id, current_index);
    GUILIST_SetTopItemIndex(ctrl_id, top_index);
}

/****************************************************************************/
//  Description : handle proxy setting reject list pop menu window
//  Global resource dependence : 
//  Author: 
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleProxyRejectListPopMenuWin(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"WifiHandleProxyRejectListPopMenuWin win_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_14038_112_2_18_3_10_21_264,(uint8*)"d",win_id);
    
    switch(msg_id) 
    {        
    case MSG_OPEN_WINDOW:
        WifiCreateRejectListPopMenu(win_id, MMIWIFI_REJECTLIST_POP_MENU_CTRL_ID);
        MMK_SetAtvCtrl(win_id, MMIWIFI_REJECTLIST_POP_MENU_CTRL_ID);
        break;

    case MSG_FULL_PAINT:
        {  
            uint16 totalnum = 0;
            totalnum = GUILIST_GetTotalItemNum(MMIWIFI_LIST_REJECT_LIST_CTRL_ID);
            if(0 == totalnum)
            {
                GUIMENU_SetNodeGrayed(TRUE, 1, NULL, MMIWIFI_REJECTLIST_POP_MENU_CTRL_ID);
                GUIMENU_SetNodeGrayed(TRUE, 2, NULL, MMIWIFI_REJECTLIST_POP_MENU_CTRL_ID);
            }
        }
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_CTL_MIDSK:    
        {
            MMIWIFI_PROXY_REJECTLIST_E rejectlist = {0};
            uint16 index = 0;

            MMIWIFI_GetProxyRejectList(&rejectlist);
            index = GUILIST_GetCurItemIndex(MMIWIFI_LIST_REJECT_LIST_CTRL_ID);
            switch (GUIMENU_GetCurNodeId(MMIWIFI_REJECTLIST_POP_MENU_CTRL_ID))
            {
            case MMIWIFI_POPMENU_ID_ADD:
                MMK_CloseWin(win_id);
                if(MMIWIFI_MAX_REJECTLIST_NUM == GUILIST_GetTotalItemNum(MMIWIFI_LIST_REJECT_LIST_CTRL_ID))
                {//列表已满，不再保存
                    MMIPUB_OpenAlertWarningWin(TXT_WIFI_REJECTLIST_FULL);
                }
                else
                {
                    MMK_CreatePubEditWin((uint32 *)MMIWIFI_REJECTLIST_ADD_WIN_TAB, PNULL);
                }
                break;
                
             case MMIWIFI_POPMENU_ID_DELETE:
                MMIPUB_OpenQueryWinByTextId(TXT_COMMON_CONFIRM_DELETE,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
                break;
                
            case MMIWIFI_POPMENU_ID_CLEAR:
                MMIPUB_OpenQueryWinByTextId(TXT_COMMON_ENSURE_CLEAR,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
                break;
                
            default:
                break;
            }
        }
        break;
        
    case MSG_PROMPTWIN_OK:
        {
            MMIWIFI_PROXY_REJECTLIST_E rejectlist = {0};
            uint16 index = 0;
            
            MMIPUB_CloseQuerytWin(PNULL);
            MMIWIFI_GetProxyRejectList(&rejectlist);
            index = GUILIST_GetCurItemIndex(MMIWIFI_LIST_REJECT_LIST_CTRL_ID);
            switch (GUIMENU_GetCurNodeId(MMIWIFI_REJECTLIST_POP_MENU_CTRL_ID))
            {
             case MMIWIFI_POPMENU_ID_DELETE:
                //删除当前项，将后面项依次向前一项复制
                SCI_MEMSET(rejectlist.proxy_rejectlist[index], 0, sizeof(rejectlist.proxy_rejectlist[index]));
                index++;
                for(;index<MMIWIFI_MAX_REJECTLIST_NUM;index++)
                {
                    uint16 proxy_rejectlist_len = strlen(rejectlist.proxy_rejectlist[index]);

                    if(0 != proxy_rejectlist_len)
                    {
                        SCI_MEMCPY(rejectlist.proxy_rejectlist[index-1], rejectlist.proxy_rejectlist[index], proxy_rejectlist_len);
                        SCI_MEMSET(rejectlist.proxy_rejectlist[index], 0, sizeof(rejectlist.proxy_rejectlist[index]));
                    }
                }
                MMIWIFI_SetProxyRejectList(&rejectlist);
                break;
                
            case MMIWIFI_POPMENU_ID_CLEAR:
                SCI_MEMSET(&rejectlist, 0, sizeof(MMIWIFI_PROXY_REJECTLIST_E));
                MMIWIFI_SetProxyRejectList(&rejectlist);
                break;
                
            default:
                break;
            }
            MMK_CloseWin(win_id);
        }
        break;

    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
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

/****************************************************************************/
//  Description : handle proxy setting reject list edit window
//  Global resource dependence : 
//  Author: 
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleProxyRejectListEditWin(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"WifiHandleProxyRejectListPopMenuWin win_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_14167_112_2_18_3_10_22_265,(uint8*)"d",win_id);
    
    switch(msg_id) 
    {        
    case MSG_OPEN_WINDOW:
        {
            MMIWIFI_PROXY_REJECTLIST_E rejectlist = {0};
            uint16 index = 0;
            wchar url_name[MMIWIFI_URL_MAX_LEN+1] = {0};
            uint16 url_len = 0;

            MMIWIFI_GetProxyRejectList(&rejectlist);
            index = GUILIST_GetCurItemIndex(MMIWIFI_LIST_REJECT_LIST_CTRL_ID);
            url_len = strlen(rejectlist.proxy_rejectlist[index]);
            GUIEDIT_SetIm(MMIWIFI_PROXY_REJECTLIST_NEW_EDIT_CTRL_ID, GUIIM_TYPE_ENGLISH | GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL, GUIIM_TYPE_ABC); /*lint !e655*/
            GUIEDIT_SetSoftkey(MMIWIFI_PROXY_REJECTLIST_NEW_EDIT_CTRL_ID,0,1,TXT_NULL,TXT_COMMON_OK,PNULL);
            MMI_STRNTOWSTR(url_name, MMIWIFI_URL_MAX_LEN, (uint8 *)rejectlist.proxy_rejectlist[index], url_len, MIN(url_len, MMIWIFI_URL_MAX_LEN));
            GUIEDIT_SetString(MMIWIFI_PROXY_REJECTLIST_NEW_EDIT_CTRL_ID, url_name, MMIAPICOM_Wstrlen(url_name));
            GUILABEL_SetTextById(MMIWIFI_PROXY_REJECTLIST_EDIT_LABEL_CTRL_ID, TXT_WIFI_INPUT_NETNAME, TRUE);
            MMK_SetAtvCtrl(win_id, MMIWIFI_PROXY_REJECTLIST_NEW_EDIT_CTRL_ID);
        }
        break;

    case MSG_FULL_PAINT:
        break;
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
        {
            MMIWIFI_PROXY_REJECTLIST_E rejectlist = {0};
            uint16 index = 0;
            int32 find_index = 0;
            MMI_STRING_T string = {0};
            
            MMIWIFI_GetProxyRejectList(&rejectlist);
            GUIEDIT_GetString(MMIWIFI_PROXY_REJECTLIST_NEW_EDIT_CTRL_ID, &string);
            index = GUILIST_GetCurItemIndex(MMIWIFI_LIST_REJECT_LIST_CTRL_ID);
            SCI_MEMSET(rejectlist.proxy_rejectlist[index], 0, sizeof(rejectlist.proxy_rejectlist[index]));
            MMI_WSTRNTOSTR((uint8 *)rejectlist.proxy_rejectlist[index], MMIWIFI_URL_MAX_LEN, string.wstr_ptr, string.wstr_len, MIN(MMIWIFI_URL_MAX_LEN,string.wstr_len));
            find_index = WifiGetExcludingListIndex(rejectlist.proxy_rejectlist[index]);

            if(find_index < 0 || find_index == index)
            {
                MMIWIFI_SetProxyRejectList(&rejectlist);
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_WIFI_NETNAME_EXISTED);
            }
            MMK_CloseWin(win_id);
        }
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
#ifndef MMI_GUI_STYLE_TYPICAL
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
#endif /* MMI_GUI_STYLE_TYPICAL */

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return result;
}

/****************************************************************************/
//  Description : handle proxy setting reject list add window
//  Global resource dependence : 
//  Author: 
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleProxyRejectListAddWin(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"WifiHandleProxyRejectListPopMenuWin win_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_14267_112_2_18_3_10_22_266,(uint8*)"d",win_id);
    
    switch(msg_id) 
    {
    case MSG_OPEN_WINDOW:
        GUILABEL_SetTextById(MMIWIFI_PROXY_REJECTLIST_EDIT_LABEL_CTRL_ID, TXT_WIFI_INPUT_NETNAME, TRUE);
        GUIEDIT_SetSoftkey(MMIWIFI_PROXY_REJECTLIST_NEW_EDIT_CTRL_ID,0,1,TXT_NULL,TXT_COMMON_OK,PNULL);
        GUIEDIT_SetIm(MMIWIFI_PROXY_REJECTLIST_NEW_EDIT_CTRL_ID, GUIIM_TYPE_ENGLISH | GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL, GUIIM_TYPE_ABC); /*lint !e655*/
        MMK_SetAtvCtrl(win_id, MMIWIFI_PROXY_REJECTLIST_NEW_EDIT_CTRL_ID);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
        {
            MMIWIFI_PROXY_REJECTLIST_E rejectlist = {0};
            MMI_STRING_T string = {0};
            uint16 next_index = 0;

            MMIWIFI_GetProxyRejectList(&rejectlist);
            for(next_index=0;next_index<MMIWIFI_MAX_REJECTLIST_NUM;next_index++)
            {
                if (0 == strlen(rejectlist.proxy_rejectlist[next_index]))
                {
                    break;
                }
            }
            GUIEDIT_GetString(MMIWIFI_PROXY_REJECTLIST_NEW_EDIT_CTRL_ID, &string);

            if (next_index < MMIWIFI_MAX_REJECTLIST_NUM)
            {
                //MMI_WSTRNTOSTR，无法转换保存汉字的WCHAR.
                MMI_WSTRNTOSTR((uint8 *)rejectlist.proxy_rejectlist[next_index], MMIWIFI_URL_MAX_LEN, string.wstr_ptr, string.wstr_len, MIN(MMIWIFI_URL_MAX_LEN, string.wstr_len));
                if(!MMIAPIWIFI_IsExistInExcludingList(rejectlist.proxy_rejectlist[next_index]))
                {
                    MMIWIFI_SetProxyRejectList(&rejectlist);
                }
                else
                {//列表中已有此项，提示，不再保存
                    MMIPUB_OpenAlertWarningWin(TXT_WIFI_NETNAME_EXISTED);
                }
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

/****************************************************************************/
//  Description : handle web key window
//  Global resource dependence : 
//  Author: 
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleWepKeyWin(
                                        MMI_WIN_ID_T    win_id, 
                                        MMI_MESSAGE_ID_E   msg_id, 
                                        DPARAM             param
                                        )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_STRING_T temp_string={0};
    
    switch(msg_id) 
    {        
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MMIWIFI_WEP_KEY_EDIT_CTRL_ID);
        GUILABEL_SetTextById(MMIWIFI_WEP_KEY_LABEL_CTRL_ID,TXT_COMMON_PASSWORD,TRUE);
        break;
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
        GUIEDIT_GetString(MMIWIFI_WEP_KEY_EDIT_CTRL_ID,&temp_string); 
        if(0!= temp_string.wstr_len)
        {
            MMI_STRING_T prompt_str = {0};
            MMI_GetLabelTextByLang(TXT_WIFI_CONNECTING, &prompt_str);
            MMIPUB_OpenWaitWin(1,&prompt_str,PNULL,PNULL,MMIWIFI_CONNECTING_WAIT_WIN_ID,IMAGE_NULL,
                ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_ONE,PNULL);
        }
        break;
                
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        break;
    }
    
    return result;
}

/****************************************************************************/
//  Description : handle web key window
//  Global resource dependence : 
//  Author: 
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleSpectifySearchSSID(
                                                 MMI_WIN_ID_T    win_id, 
                                                 MMI_MESSAGE_ID_E   msg_id, 
                                                 DPARAM             param
                                                 )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    MMI_STRING_T string_info = {0};
    uint8  temp_buf_char[WIFISUPP_SSID_MAX_LEN + 1] = {0};
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MMIWIFI_SPECTIFY_SEARCH_SSID_EDIT_CTRL_ID);
        GUILABEL_SetTextById(MMIWIFI_SPECTIFY_SEARCH_SSID_LABEL_CTRL_ID,TXT_WIFI_INPUT_SSID,TRUE);
        GUIEDIT_SetTextMaxLen(MMIWIFI_SPECTIFY_SEARCH_SSID_EDIT_CTRL_ID, WIFISUPP_SSID_MAX_LEN / 2 - 1, WIFISUPP_SSID_MAX_LEN);
        GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;     
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
        //设定SSID
        GUIEDIT_GetString(MMIWIFI_SPECTIFY_SEARCH_SSID_EDIT_CTRL_ID,&string_info);
        if (PNULL == string_info.wstr_ptr || 0 == string_info.wstr_len)
        {
            MMIPUB_OpenAlertWarningWin(TXT_WIFI_SSID_EMPTY);
        }
        else
        {
            (void)GUI_WstrToGB(temp_buf_char, string_info.wstr_ptr, string_info.wstr_len);
            MMIWIFI_Scan(temp_buf_char);
            
        }
        break;

            
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin( win_id );      
            break;
            
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : wlan set proxy setting window
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetProxySettingWindow(void)
{
#ifndef MMI_GUI_STYLE_TYPICAL
    MMI_IMAGE_ID_T              image_id = IMAGE_NULL;
#else
    uint32                      i = 0;
#endif /* MMI_GUI_STYLE_TYPICAL */
    MMIWIFI_PROXY_PASSWORD_E      password = {0};
    uint16      port = 0;
    MMIWIFI_PROXY_NAME_E      name = {0};
#ifdef MMI_GUI_STYLE_TYPICAL
    MMI_STRING_T      item_index[2] = {0};
    MMI_TEXT_ID_T               wep_index[2] = {TXT_WIFI_PROXY_NO,TXT_PROXY_ON};
#endif
    char str[6] = {0};
    wchar wstr[6] = {0};
    
    //SCI_TRACE_LOW:"WifiSetProxySettingWindow called"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_14445_112_2_18_3_10_22_267,(uint8*)"");
    
    GUIEDIT_SetIm(MMIWIFI_PROXY_IP_DATA_EDIT_CTRL_ID, GUIIM_TYPE_ABC | GUIIM_TYPE_ENGLISH | GUIIM_TYPE_DIGITAL, GUIIM_TYPE_ABC);
#ifndef MMI_GUI_STYLE_TYPICAL
    GUIFORM_SetType(MMIWIFI_PROXY_SETTING_FORMCTRL_ID, GUIFORM_TYPE_TP);
//    GUISETLIST_SetFontAlign(MMIWIFI_LIST_PROXY_CTRL_ID, ALIGN_LVMIDDLE);
    GUIEDIT_SetAlign(MMIWIFI_PROXY_IP_DATA_EDIT_CTRL_ID, ALIGN_LVMIDDLE);
    GUIEDIT_SetAlign(MMIWIFI_PROXY_PORT_EDIT_CTRL_ID, ALIGN_LVMIDDLE);
    GUIEDIT_SetAlign(MMIWIFI_PROXY_NAME_EDIT_CTRL_ID, ALIGN_LVMIDDLE);
    GUIEDIT_SetAlign(MMIWIFI_PROXY_PASSWORD_EDIT_CTRL_ID, ALIGN_LVMIDDLE);
#endif

    GUIFORM_SetStyle(MMIWIFI_CHILD_FORM_PROXY_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIWIFI_CHILD_FORM_PROXY_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIWIFI_CHILD_FORM_PROXY_IP_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIWIFI_CHILD_FORM_PROXY_PORT_TRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIWIFI_CHILD_FORM_PROXY_USER_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIWIFI_CHILD_FORM_PROXY_PASSWORD_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIWIFI_CHILD_FORM_PROXY_REJECTLIST_CTRL_ID,GUIFORM_STYLE_UNIT);

    //设置label控件显示的字符  
#ifdef MMI_GUI_STYLE_TYPICAL
    GUILABEL_SetTextById(MMIWIFI_PROXY_QUERY_LABEL_CTRL_ID,TXT_PROXY_USE_QUERY,FALSE);
#endif /* MMI_GUI_STYLE_TYPICAL */
    GUILABEL_SetTextById(MMIWIFI_PROXY_IP_LABEL_CTRL_ID,TXT_WIFI_PROXY_IP,FALSE);
    GUILABEL_SetTextById(MMIWIFI_PROXY_PORT_LABEL_CTRL_ID,TXT_WIFI_PROXY_PORT,FALSE);
    GUILABEL_SetTextById(MMIWIFI_PROXY_NAME_LABEL_CTRL_ID,TXT_COMMON_USERNAME,FALSE);
    GUILABEL_SetTextById(MMIWIFI_PROXY_PASSWORD_LABEL_CTRL_ID,TXT_COMMON_PASSWORD,FALSE);
    GUILABEL_SetTextById(MMIWIFI_PROXY_REJECTLIST_LABEL_CTRL_ID,TXT_WIFI_PROXY_REJECTLIST,FALSE);

    GUIEDIT_SetIm(MMIWIFI_PROXY_NAME_EDIT_CTRL_ID, GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL,GUIIM_TYPE_ABC); /*lint !e655*/

#ifndef MMI_GUI_STYLE_TYPICAL
    GUILIST_SetMaxItem(MMIWIFI_LIST_PROXY_CTRL_ID, 1, FALSE);
    GUILIST_RemoveAllItems(MMIWIFI_LIST_PROXY_CTRL_ID);
    GUILIST_SetSlideState(MMIWIFI_LIST_PROXY_CTRL_ID, FALSE);  
    image_id = s_is_use_proxy ? IMAGE_CHECK_SELECTED_ICON : IMAGE_CHECK_UNSELECTED_ICON;
    AppendTwoTextOneImageItem(MMIWIFI_LIST_PROXY_CTRL_ID, TXT_PROXY_USE_QUERY, TXT_NULL, image_id, 0);
#else
    //设置下拉控件的显示项及焦点项
    for (i = 0; i < 2; i++)
    {
     MMI_GetLabelTextByLang(wep_index[i], &item_index[i]);
    }
    GUIDROPDOWNLIST_AppendItemArray(MMIWIFI_PROXY_QUERY_DROPDOWNLIST_CTRL_ID, item_index, 2 );
    GUIDROPDOWNLIST_SetCurItemIndex(MMIWIFI_PROXY_QUERY_DROPDOWNLIST_CTRL_ID, (uint16)MMIWIFI_GetProxyQuery());
#endif /* MMI_GUI_STYLE_TYPICAL */

    //代理IP地址
    WifiOwnAttribute_DisplayEditIPCtrlText(MMIWIFI_PROXY_IP_DATA_EDIT_CTRL_ID);
    
    //代理端口
    MMIWIFI_GetProxyPort(&port);

    //    itoa(port, str, 10);
    sprintf(str, "%d", port);//@discuss 0105
    
    MMIAPICOM_StrToWstr((uint8 *)str, wstr);
    GUIEDIT_SetString(MMIWIFI_PROXY_PORT_EDIT_CTRL_ID, wstr, MMIAPICOM_Wstrlen(wstr)); 

    //用户名
    MMIWIFI_GetProxyName(&name);
    GUIEDIT_SetString(MMIWIFI_PROXY_NAME_EDIT_CTRL_ID, name.proxy_name, MMIAPICOM_Wstrlen(name.proxy_name)); 
    
    //密码
    MMIWIFI_GetProxyPassword(&password);
    GUIEDIT_SetString(MMIWIFI_PROXY_PASSWORD_EDIT_CTRL_ID, password.proxy_password, MMIAPICOM_Wstrlen(password.proxy_password)); 
    
    WifiProxy_SetChildCtrlDisplay();    

}

/*****************************************************************************/
//  Description : WifiSetRejectListWindow
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetRejectListWindow(void)
{
    GUI_BOTH_RECT_T both_rect = {0};
    
    both_rect = MMITHEME_GetWinClientBothRect(MMIWLAN_PROXY_REJECTLIST_WIN_ID);
    GUIAPICTRL_SetBothRect(MMIWIFI_LIST_REJECT_LIST_CTRL_ID,&both_rect);
    GUILIST_SetNeedPrgbar(MMIWIFI_LIST_REJECT_LIST_CTRL_ID, TRUE);
    //排除列表最多可设置10个
    GUILIST_SetMaxItem(MMIWIFI_LIST_REJECT_LIST_CTRL_ID, MMIWIFI_MAX_REJECTLIST_NUM, FALSE);
}

/*****************************************************************************/
//  Description : WifiOwnAttribute_SetChildCtrlDisplay
//  Global resource dependence : none
//  Author:
//  Note://设置本机属性窗口子控件是否置灰
/*****************************************************************************/
LOCAL void WifiOwnAttribute_SetChildCtrlDisplay(void)
{
    GUIFORM_CHILD_DISPLAY_E dsp_type = GUIFORM_CHILD_DISP_GRAY;
#ifndef MMI_GUI_STYLE_TYPICAL
    if(!s_ip_type)     
#else
    uint16                index=0;

    index=GUIDROPDOWNLIST_GetCurItemIndex(MMIWIFI_IP_TYPE_DROPDOWNLIST_CTRL_ID);

    if(0==index)        
#endif /* MMI_GUI_STYLE_TYPICAL */
    {
        dsp_type = GUIFORM_CHILD_DISP_GRAY;
    }
    else
    {
        dsp_type = GUIFORM_CHILD_DISP_NORMAL;
    }

    GUIFORM_SetChildDisplay(MMIWIFI_OWN_ATTRIBUTE_FORMCTRL_ID, MMIWIFI_CHILD_FORM_IP_ADDRESS_CTRL_ID, dsp_type);
    GUIFORM_SetChildDisplay(MMIWIFI_OWN_ATTRIBUTE_FORMCTRL_ID, MMIWIFI_CHILD_FORM_DNS1_CTRL_ID, dsp_type);
    GUIFORM_SetChildDisplay(MMIWIFI_OWN_ATTRIBUTE_FORMCTRL_ID, MMIWIFI_CHILD_FORM_DNS2_CTRL_ID, dsp_type);
    GUIFORM_SetChildDisplay(MMIWIFI_OWN_ATTRIBUTE_FORMCTRL_ID, MMIWIFI_CHILD_FORM_MASK_CTRL_ID, dsp_type);
    GUIFORM_SetChildDisplay(MMIWIFI_OWN_ATTRIBUTE_FORMCTRL_ID, MMIWIFI_CHILD_FORM_GATEWAY_CTRL_ID, dsp_type);

    GUIFORM_SetChildDisplay(MMIWIFI_OWN_ATTRIBUTE_FORMCTRL_ID, MMIWIFI_CHILD_FORM_MAC_CTRL_ID, GUIFORM_CHILD_DISP_GRAY);
}

/*****************************************************************************/
//  Description : WifiOwnAttribute_SetChildCtrlDisplay
//  Global resource dependence : none
//  Author:
//  Note://设置代理设置窗口子控件是否置灰
/*****************************************************************************/
LOCAL void WifiProxy_SetChildCtrlDisplay(void)
{
    GUIFORM_CHILD_DISPLAY_E dsp_type = GUIFORM_CHILD_DISP_GRAY;
#ifndef MMI_GUI_STYLE_TYPICAL
    if(!s_is_use_proxy)     
#else
    uint16                index=0;
    
    index=GUIDROPDOWNLIST_GetCurItemIndex(MMIWIFI_PROXY_QUERY_DROPDOWNLIST_CTRL_ID);
    //若不使用代理,其他控件均置灰,index为0时不使用代理
    if(0==index)   
#endif /* MMI_GUI_STYLE_TYPICAL */
    {
        dsp_type = GUIFORM_CHILD_DISP_GRAY;
    }
    else
    {
        dsp_type = GUIFORM_CHILD_DISP_NORMAL;
    }

    GUIFORM_SetChildDisplay(MMIWIFI_PROXY_SETTING_FORMCTRL_ID, MMIWIFI_CHILD_FORM_PROXY_IP_CTRL_ID, dsp_type);
    GUIFORM_SetChildDisplay(MMIWIFI_PROXY_SETTING_FORMCTRL_ID, MMIWIFI_CHILD_FORM_PROXY_PORT_TRL_ID, dsp_type);
    GUIFORM_SetChildDisplay(MMIWIFI_PROXY_SETTING_FORMCTRL_ID, MMIWIFI_CHILD_FORM_PROXY_USER_CTRL_ID, dsp_type);
    GUIFORM_SetChildDisplay(MMIWIFI_PROXY_SETTING_FORMCTRL_ID, MMIWIFI_CHILD_FORM_PROXY_PASSWORD_CTRL_ID, dsp_type);
    GUIFORM_SetChildDisplay(MMIWIFI_PROXY_SETTING_FORMCTRL_ID, MMIWIFI_CHILD_FORM_PROXY_REJECTLIST_CTRL_ID, dsp_type); 
}

/*****************************************************************************/
//  Description : Get List User data 
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL uint32 WifiGetListUserdata(MMI_CTRL_ID_T ctrl_id, uint16 index)
{
    uint32 user_data = 0;
    
    GUILIST_GetItemData(ctrl_id, index, (uint32*)(&user_data));
    
    return user_data;
}

/*****************************************************************************/
//  Description : WifiSetEditAPWindow
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetEditAPWindow(void)
{
    //if(PNULL != profile_ptr),则为编辑，否则为新建
#ifdef MMI_GUI_STYLE_TYPICAL
    uint32                      i = 0;
    MMI_STRING_T                item_index[5] = {0};
#endif
    uint16                          index=0;
    uint8* ssid_ptr = PNULL;
    uint8 ssid_len = 0;
    uint16 temp_buf[WIFISUPP_SSID_MAX_LEN + 1] = {0};
    MMI_TEXT_ID_T               network_state_index[2] = {TXT_WIFI_NETWORK_STATE_OPEN,TXT_HIDE};
#ifdef MMI_WLAN_AUTHORITY_COMPLEX
    MMI_TEXT_ID_T               connect_state_index[2] = {TXT_WIFI_CONNECT_STATE_MANUAL,TXT_AUTO};
#endif /* MMI_WLAN_AUTHORITY_COMPLEX */
#if 0
    MMI_TEXT_ID_T               auth_type_index[] = {TXT_WIFI_AUTH_TYPE_OPENSYS,TXT_WIFI_AUTH_TYPE_WEP,TXT_WIFI_AUTH_TYPE_WPA,TXT_WIFI_AUTH_TYPE_WPA2,TXT_WIFI_AUTH_TYPE_WAPI, TXT_WLAN_EAP_SIM};
#else
    MMI_TEXT_ID_T               auth_type_index[] = {TXT_WIFI_AUTH_TYPE_OPENSYS,TXT_WIFI_AUTH_TYPE_WEP,TXT_WIFI_AUTH_TYPE_WPA,TXT_WIFI_AUTH_TYPE_WPA2,TXT_WIFI_AUTH_TYPE_WAPI};
#endif 
#ifndef MMI_GUI_STYLE_TYPICAL
    MMI_STRING_T                string_1 = {0};
    MMI_STRING_T                string_2 = {0};
    uint32                      item_cnt = 0;
    uint16 current_index = 0;
    uint16 top_index = 0;
#endif /* MMI_GUI_STYLE_TYPICAL */

    //SCI_TRACE_LOW:"WifiSetEditAPWindow called"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_14641_112_2_18_3_10_23_268,(uint8*)"");

#ifndef MMI_GUI_STYLE_TYPICAL
    current_index = GUILIST_GetCurItemIndex(MMIWLAN_LIST_AP_EDIT_CTRL_ID);
    top_index = GUILIST_GetTopItemIndex(MMIWLAN_LIST_AP_EDIT_CTRL_ID);
    GUILIST_SetMaxItem(MMIWLAN_LIST_AP_EDIT_CTRL_ID, 10, FALSE);
    GUILIST_RemoveAllItems(MMIWLAN_LIST_AP_EDIT_CTRL_ID);
    GUILIST_PermitBorder(MMIWLAN_LIST_AP_EDIT_CTRL_ID, FALSE);

    /* SSID */
    MMIRES_GetText(TXT_WIFI_SSID, PNULL, &string_1);

    ssid_ptr = WifiGetSsid();
    ssid_len = WifiGetSsidLen();

    if (0 != ssid_len && PNULL != ssid_ptr)
    {
        GUI_GBToWstr(temp_buf, ssid_ptr, MIN(ssid_len, WIFISUPP_SSID_MAX_LEN));
    }

    string_2.wstr_ptr = temp_buf;
    string_2.wstr_len = MMIAPICOM_Wstrlen(temp_buf);

    AppendTwoTextOneImageItemByString(MMIWLAN_LIST_AP_EDIT_CTRL_ID, &string_1,&string_2, IMAGE_COMMON_RIGHT_ARROW, (uint32)WifiEditAPSSIDWin);
    ++item_cnt;

    /* State */
    index = (uint16)WifiGetIsSsidHide();
    AppendTwoTextOneImageItem(MMIWLAN_LIST_AP_EDIT_CTRL_ID, TXT_WIFI_NETWORK_STATE, network_state_index[index], IMAGE_COMMON_RIGHT_ARROW, (uint32)WifiHandleAPEditNetworkState);
    ++item_cnt;

#ifdef MMI_WLAN_AUTHORITY_COMPLEX
    /* Type */
    AppendTwoTextOneImageItem(MMIWLAN_LIST_AP_EDIT_CTRL_ID, TXT_WLAN_NETWORK_TYPE, TXT_WIFI_NETWORK_TYPE_INFRASTRUCTURE, IMAGE_NULL, (uint32)0);
    GUILIST_SetItemGray(MMIWLAN_LIST_AP_EDIT_CTRL_ID, item_cnt, TRUE);
    ++item_cnt;

#endif /* MMI_WLAN_AUTHORITY_COMPLEX */

    /* Auth Type */
    index = (uint16)WifiGetEncryptProtocol();
    AppendTwoTextOneImageItem(MMIWLAN_LIST_AP_EDIT_CTRL_ID, TXT_COMM_SECURITY_TYPE, auth_type_index[index], IMAGE_COMMON_RIGHT_ARROW, (uint32)WifiHandleAPEditAuthType);
    ++item_cnt;

#ifdef WLAN_EAP_SUPPORT
    if (WIFISUPP_ENCRYP_PROTOCOL_EAP == index)
    {
        MMI_TEXT_ID_T       Method_text_id[] = {TXT_WLAN_SIM};
        MMI_TEXT_ID_T       sim_text_id[] = {TXT_SIM_SEL_SIM1, TXT_SIM_SEL_SIM2, TXT_SIM_SEL_SIM3, TXT_SIM_SEL_SIM4};

        AppendTwoTextOneImageItem(MMIWLAN_LIST_AP_EDIT_CTRL_ID, TXT_WLAN_EAP_METHOD, Method_text_id[0], IMAGE_COMMON_RIGHT_ARROW, (uint32)PNULL);
        GUILIST_SetItemGray(MMIWLAN_LIST_AP_EDIT_CTRL_ID, item_cnt, TRUE);
        ++item_cnt;

#ifndef MMI_MULTI_SIM_SYS_SINGLE
        index = (uint16)WifiGetEAPSimIndex();
        AppendTwoTextOneImageItem(MMIWLAN_LIST_AP_EDIT_CTRL_ID, TXT_WLAN_SIM, sim_text_id[index], IMAGE_COMMON_RIGHT_ARROW, (uint32)WifiHandleAPEditEAPSimIndex);
        ++item_cnt;
#endif /* MMI_MULTI_SIM_SYS_SINGLE */
    }
    else
    {
        /* Auth Setting */
        AppendTwoTextOneImageItem(MMIWLAN_LIST_AP_EDIT_CTRL_ID, TXT_COMM_AUTH_SETTING, TXT_NULL, IMAGE_COMMON_RIGHT_ARROW, (uint32)WifiHandleAPEditAuthSetting);
        GUILIST_SetItemGray(MMIWLAN_LIST_AP_EDIT_CTRL_ID, item_cnt, (BOOLEAN)(0 == index));
        ++item_cnt;
    }
#else
    /* Auth Setting */
    AppendTwoTextOneImageItem(MMIWLAN_LIST_AP_EDIT_CTRL_ID, TXT_COMM_AUTH_SETTING, TXT_NULL, IMAGE_COMMON_RIGHT_ARROW, (uint32)WifiHandleAPEditAuthSetting);
    GUILIST_SetItemGray(MMIWLAN_LIST_AP_EDIT_CTRL_ID, item_cnt, (BOOLEAN)(0 == index));
    ++item_cnt;
#endif /* WLAN_EAP_SUPPORT */

#ifdef MMI_WLAN_AUTHORITY_COMPLEX
    /* Connect Setting */
    index = (uint16)WifiGetIsAutoConnect();
    AppendTwoTextOneImageItem(MMIWLAN_LIST_AP_EDIT_CTRL_ID, TXT_WIFI_CONNECT_STATE_SETTING, connect_state_index[index], IMAGE_COMMON_RIGHT_ARROW, (uint32)WifiHandleAPEditConnectState);
    ++item_cnt;
#endif /* MMI_WLAN_AUTHORITY_COMPLEX */

    GUILIST_SetCurItemIndex(MMIWLAN_LIST_AP_EDIT_CTRL_ID, current_index);
    GUILIST_SetTopItemIndex(MMIWLAN_LIST_AP_EDIT_CTRL_ID, top_index);
#else
    GUIFORM_SetStyle(MMIWIFI_CHILD_FORM_SSID_CTRL_ID, GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIWIFI_CHILD_FORM_NETWORK_STATE_CTRL_ID, GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIWIFI_CHILD_FORM_NETWORK_TYPE_CTRL_ID, GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIWIFI_CHILD_FORM_AUTH_TYPE_CTRL_ID, GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIWIFI_CHILD_FORM_AUTH_SETTING_CTRL_ID, GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIWIFI_CHILD_FORM_CONNECT_STATE_CTRL_ID, GUIFORM_STYLE_UNIT);

    GUILABEL_SetTextById(MMIWIFI_EDIT_SSID_LABEL_CTRL_ID,TXT_WIFI_SSID,TRUE);

    //网络名称SSID
    ssid_ptr = WifiGetSsid();
    ssid_len = WifiGetSsidLen();
    if (0 != ssid_len && PNULL != ssid_ptr)
    {
        GUI_GBToWstr(temp_buf, ssid_ptr, MIN(ssid_len, WIFISUPP_SSID_MAX_LEN));
    }
    GUIEDIT_SetString(MMIWIFI_EDIT_SSID_DATA_EDIT_CTRL_ID,temp_buf, MMIAPICOM_Wstrlen(temp_buf));

    //设置label控件显示的字符  
    GUILABEL_SetTextById(MMIWIFI_EDIT_NETWORK_STATE_LABEL_CTRL_ID,TXT_WIFI_NETWORK_STATE,TRUE);
    GUILABEL_SetTextById(MMIWIFI_EDIT_NETWORK_TYPE_LABEL_CTRL_ID,TXT_WLAN_NETWORK_TYPE,TRUE);
    GUILABEL_SetTextById(MMIWIFI_EDIT_NETWORK_TYPE_DATA_LABEL_CTRL_ID,TXT_WIFI_NETWORK_TYPE_INFRASTRUCTURE,TRUE);
    GUILABEL_SetTextById(MMIWIFI_EDIT_AUTH_TYPE_LABEL_CTRL_ID,TXT_COMM_SECURITY_TYPE,TRUE);
    GUILABEL_SetTextById(MMIWIFI_EDIT_AUTH_SETTING_LABEL_CTRL_ID,TXT_COMM_AUTH_SETTING,TRUE);
    GUILABEL_SetTextById(MMIWIFI_EDIT_EAP_SIM_INDEX_LABEL_CTRL_ID,TXT_WLAN_EAP_METHOD,TRUE);
    GUILABEL_SetTextById(MMIWIFI_EDIT_CONNECT_STATE_LABEL_CTRL_ID,TXT_WIFI_CONNECT_STATE_SETTING,TRUE);
    
    //网络状态
    for (i = 0; i <  sizeof(network_state_index) / sizeof(network_state_index[0]); i++)
    {
        MMI_GetLabelTextByLang(network_state_index[i], &item_index[i]);
    }
    GUIDROPDOWNLIST_AppendItemArray(MMIWIFI_EDIT_NETWORK_STATE_DROPDOWNLIST_CTRL_ID, item_index, 2 );
    index = (uint16)WifiGetIsSsidHide();
    GUIDROPDOWNLIST_SetCurItemIndex(MMIWIFI_EDIT_NETWORK_STATE_DROPDOWNLIST_CTRL_ID, index);

    //WLAN网络模式
    GUIFORM_SetChildDisplay(MMIWIFI_EDIT_AP_FORMCTRL_ID, 
        MMIWIFI_CHILD_FORM_NETWORK_TYPE_CTRL_ID,
        GUIFORM_CHILD_DISP_GRAY);

    //WLAN安全模式
    for (i = 0; i < sizeof(auth_type_index) / sizeof(auth_type_index[0]); i++)
    {
        MMI_GetLabelTextByLang(auth_type_index[i], &item_index[i]);
    }
    GUIDROPDOWNLIST_AppendItemArray(MMIWIFI_EDIT_AUTH_TYPE_DROPDOWNLIST_CTRL_ID, item_index, 5 );
    index = (uint16)WifiGetEncryptProtocol();
    GUIDROPDOWNLIST_SetCurItemIndex(MMIWIFI_EDIT_AUTH_TYPE_DROPDOWNLIST_CTRL_ID, index);

#ifdef WLAN_EAP_SUPPORT
    //依据WLAN安全模式选项决定安全设置是否置灰
    if(WIFISUPP_ENCRYP_PROTOCOL_OPENSYS == index)
    {
        GUIFORM_SetChildDisplay(MMIWIFI_EDIT_AP_FORMCTRL_ID, 
            MMIWIFI_CHILD_FORM_AUTH_SETTING_CTRL_ID,
            GUIFORM_CHILD_DISP_GRAY);

        GUIFORM_SetChildDisplay(MMIWIFI_EDIT_AP_FORMCTRL_ID, 
            MMIWIFI_CHILD_FORM_EAP_SIM_INDEX_CTRL_ID,
            GUIFORM_CHILD_DISP_HIDE);
    }
    else if (WIFISUPP_ENCRYP_PROTOCOL_EAP == index)
    {
        MMI_TEXT_ID_T       sim_text_id[] = {TXT_WLAN_SIM1, TXT_WLAN_SIM2, TXT_WLAN_SIM3, TXT_WLAN_SIM4};

        for (i = 0; i < MMI_DUAL_SYS_MAX && i < sizeof(sim_text_id) / sizeof(sim_text_id[0]); i++)
        {
            MMI_GetLabelTextByLang(sim_text_id[i], &item_index[i]);
        }

        GUIDROPDOWNLIST_AppendItemArray(MMIWIFI_MENU_AP_EAP_SIM_INDEX_CTRL_ID, item_index, MMI_DUAL_SYS_MAX);
        index = (uint16)WifiGetEAPSimIndex();
        GUIDROPDOWNLIST_SetCurItemIndex(MMIWIFI_MENU_AP_EAP_SIM_INDEX_CTRL_ID, index);

        GUIFORM_SetChildDisplay(MMIWIFI_EDIT_AP_FORMCTRL_ID, 
            MMIWIFI_CHILD_FORM_AUTH_SETTING_CTRL_ID,
            GUIFORM_CHILD_DISP_HIDE);
    }
    else
    {
        GUIFORM_SetChildDisplay(MMIWIFI_EDIT_AP_FORMCTRL_ID, 
            MMIWIFI_CHILD_FORM_EAP_SIM_INDEX_CTRL_ID,
            GUIFORM_CHILD_DISP_HIDE);

        GUIFORM_SetChildDisplay(MMIWIFI_EDIT_AP_FORMCTRL_ID, 
            MMIWIFI_CHILD_FORM_AUTH_SETTING_CTRL_ID,
            GUIFORM_CHILD_DISP_NORMAL);
    }
#else
    //依据WLAN安全模式选项决定安全设置是否置灰
    if(WIFISUPP_ENCRYP_PROTOCOL_OPENSYS == index)
    {
        GUIFORM_SetChildDisplay(MMIWIFI_EDIT_AP_FORMCTRL_ID, 
            MMIWIFI_CHILD_FORM_AUTH_SETTING_CTRL_ID,
            GUIFORM_CHILD_DISP_GRAY);
    }
    else
    {
        GUIFORM_SetChildDisplay(MMIWIFI_EDIT_AP_FORMCTRL_ID, 
            MMIWIFI_CHILD_FORM_AUTH_SETTING_CTRL_ID,
            GUIFORM_CHILD_DISP_NORMAL);
    }
#endif /* WLAN_EAP_SUPPORT */
    //连接状态
    for (i = 0; i < 2; i++)
    {
        MMI_GetLabelTextByLang(connect_state_index[i], &item_index[i]);
    }
    GUIDROPDOWNLIST_AppendItemArray(MMIWIFI_EDIT_CONNECT_STATE_DROPDOWNLIST_CTRL_ID, item_index, 2 );
    index = (uint16)WifiGetIsAutoConnect();
    GUIDROPDOWNLIST_SetCurItemIndex(MMIWIFI_EDIT_CONNECT_STATE_DROPDOWNLIST_CTRL_ID, index);
#endif /* MMI_GUI_STYLE_TYPICAL */
}

/*****************************************************************************/
//  Description : WifiSetEditWEPWindow
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetEditWEPWindow(void)
{
    uint32                      i = 0;
    uint16          index = WifiGetWepCurrIndex();
#ifdef MMI_GUI_STYLE_TYPICAL
    MMI_STRING_T                item_index[4] = {0};
#endif
#ifdef MMI_WLAN_AUTHORITY_COMPLEX
    MMI_TEXT_ID_T               wep_key_index[4] = {TXT_WIFI_WEP_INDEX1,TXT_WIFI_WEP_INDEX2,TXT_WIFI_WEP_INDEX3,TXT_WIFI_WEP_INDEX4};
    MMI_TEXT_ID_T               wep_encrypt_index[2] = {TXT_WIFI_WEP_BIT64,TXT_WIFI_WEP_BIT128};
    MMI_TEXT_ID_T               wep_key_state_index[2] = {TXT_WIFI_WEP_KEY_HEX,TXT_WIFI_WEP_KEY_ASCII};
    MMI_TEXT_ID_T               wep_key_open_index[2] = {TXT_WIFI_WEP_KEY_SHARE_MODE, TXT_WIFI_WEP_KEY_OPEN_MODE};
#endif /* MMI_WLAN_AUTHORITY_COMPLEX */
#ifndef MMI_GUI_STYLE_TYPICAL
#ifdef MMI_WLAN_AUTHORITY_COMPLEX
    uint32                      open_index = 0;
    uint32                      key_type = 0;
    BOOLEAN                     is_ascii = TRUE;
#endif /* MMI_WLAN_AUTHORITY_COMPLEX */
    uint16 current_index = 0;
    uint16 top_index = 0;
#endif

    //SCI_TRACE_LOW:"WifiSetEditWEPWindow called"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_14867_112_2_18_3_10_23_269,(uint8*)"");

    GUILABEL_SetTextById(MMIWIFI_EDIT_WEP_KEY_OPEN_LABEL_CTRL_ID,TXT_DETAIL_MODE,TRUE);
    GUILABEL_SetTextById(MMIWIFI_EDIT_WEP_KEY_INDEX_LABEL_CTRL_ID,TXT_WEP_KEY_INDEX,TRUE);
    GUILABEL_SetTextById(MMIWIFI_EDIT_WEP_ENCRYPT_LABEL_CTRL_ID,TXT_WIFI_BIT_SETTING,TRUE);
    GUILABEL_SetTextById(MMIWIFI_EDIT_WEP_KEY_STATE_LABEL_CTRL_ID,TXT_WEP_KEY_STYLE,TRUE);
    GUILABEL_SetTextById(MMIWIFI_EDIT_WEP_KEY_LABEL_CTRL_ID,TXT_COMMON_PASSWORD,TRUE);

#ifndef MMI_GUI_STYLE_TYPICAL
    GUIFORM_SetType(MMIWIFI_EDIT_WEP_FORMCTRL_ID, GUIFORM_TYPE_TP);
//    GUISETLIST_SetFontAlign(MMIWLAN_LIST_WEP_STATE_CTRL_ID, ALIGN_LVMIDDLE);
    GUIEDIT_SetAlign(MMIWIFI_EDIT_WEP_KEY_EDIT_CTRL_ID, ALIGN_LVMIDDLE);

    current_index = GUILIST_GetCurItemIndex(MMIWLAN_LIST_WEP_STATE_CTRL_ID);
    top_index = GUILIST_GetTopItemIndex(MMIWLAN_LIST_WEP_STATE_CTRL_ID);
    GUILIST_SetMaxItem(MMIWLAN_LIST_WEP_STATE_CTRL_ID, 10, FALSE);
    GUILIST_RemoveAllItems(MMIWLAN_LIST_WEP_STATE_CTRL_ID);
    GUILIST_SetSlideState(MMIWLAN_LIST_WEP_STATE_CTRL_ID, FALSE); 

#ifdef MMI_WLAN_AUTHORITY_COMPLEX
    /* Model */
    open_index = WifiGetIsWepOpen();
    AppendTwoTextOneImageItem(MMIWLAN_LIST_WEP_STATE_CTRL_ID, TXT_DETAIL_MODE, wep_key_open_index[open_index], IMAGE_COMMON_RIGHT_ARROW, (uint32)WifiHandleAPEditWepModelChange);

    /* Key Index */
    AppendTwoTextOneImageItem(MMIWLAN_LIST_WEP_STATE_CTRL_ID, TXT_WEP_KEY_INDEX, wep_key_index[index], IMAGE_COMMON_RIGHT_ARROW, (uint32)WifiHandleAPEditWepKeyIndexChange);

    /* Encypt */
    key_type = WifiGetWepKeyType();
    AppendTwoTextOneImageItem(MMIWLAN_LIST_WEP_STATE_CTRL_ID, TXT_WIFI_BIT_SETTING, wep_encrypt_index[key_type], IMAGE_COMMON_RIGHT_ARROW, (uint32)WifiHandleAPEditWepEncyptChange);

    /* State */
    is_ascii = WifiGetIsWepAscii(index);
    AppendTwoTextOneImageItem(MMIWLAN_LIST_WEP_STATE_CTRL_ID, TXT_WEP_KEY_STYLE, wep_key_state_index[is_ascii], IMAGE_COMMON_RIGHT_ARROW, (uint32)WifiHandleAPEditWepStateChange);
#endif /* MMI_WLAN_AUTHORITY_COMPLEX */

    /* Password */
    GUILABEL_SetTextById(MMIWIFI_EDIT_WEP_KEY_LABEL_CTRL_ID, TXT_COMMON_PASSWORD, FALSE);

    GUILIST_SetCurItemIndex(MMIWLAN_LIST_WEP_STATE_CTRL_ID, current_index);
    GUILIST_SetTopItemIndex(MMIWLAN_LIST_WEP_STATE_CTRL_ID, top_index);
#else

    //WEP Open
    for (i = 0; i < 2; i++)
    {
        MMI_GetLabelTextByLang(wep_key_open_index[i], &item_index[i]);
    }
    GUIDROPDOWNLIST_AppendItemArray(MMIWIFI_EDIT_WEP_KEY_OPEN_DROPDOWNLIST_CTRL_ID, item_index, 2);
    GUIDROPDOWNLIST_SetCurItemIndex(MMIWIFI_EDIT_WEP_KEY_OPEN_DROPDOWNLIST_CTRL_ID, (uint16)WifiGetIsWepOpen());
    
    //选择的WEP密钥
    for (i = 0; i < 4; i++)
    {
        MMI_GetLabelTextByLang(wep_key_index[i], &item_index[i]);
    }
    GUIDROPDOWNLIST_AppendItemArray(MMIWIFI_EDIT_WEP_KEY_INDEX_DROPDOWNLIST_CTRL_ID, item_index, 4);
    GUIDROPDOWNLIST_SetCurItemIndex(MMIWIFI_EDIT_WEP_KEY_INDEX_DROPDOWNLIST_CTRL_ID, index);
    
    //WEP加密
    for (i = 0; i < 2; i++)
    {
        MMI_GetLabelTextByLang(wep_encrypt_index[i], &item_index[i]);
    }
    GUIDROPDOWNLIST_AppendItemArray(MMIWIFI_EDIT_WEP_ENCRYPT_DROPDOWNLIST_CTRL_ID, item_index, 2);
    GUIDROPDOWNLIST_SetCurItemIndex(MMIWIFI_EDIT_WEP_ENCRYPT_DROPDOWNLIST_CTRL_ID, (uint16)WifiGetWepKeyType());
    
    //WEP密钥格式
    for (i = 0; i < 2; i++)
    {
        MMI_GetLabelTextByLang(wep_key_state_index[i], &item_index[i]);
    }
    GUIDROPDOWNLIST_AppendItemArray(MMIWIFI_EDIT_WEP_KEY_STATE_DROPDOWNLIST_CTRL_ID, item_index, 2);
    GUIDROPDOWNLIST_SetCurItemIndex(MMIWIFI_EDIT_WEP_KEY_STATE_DROPDOWNLIST_CTRL_ID, (uint16)WifiGetIsWepAscii(index));//是ASCII则选第二个

#endif /* MMI_GUI_STYLE_TYPICAL */

    //WEP密钥的内容,在窗口打开时要读出，显示，注意区分64位和128位
    if(0 == WifiGetWepKeyType())
    {
        uint16          temp_buf[MMIWIFI_WEP_64BIT_KEY_LEN + 1] = {0};
        uint8           temp_hex_char[MMIWIFI_WEP_64BIT_KEY_LEN * 2 + 1] = {0};
        uint16          temp_hex[MMIWIFI_WEP_64BIT_KEY_LEN*2 + 1] = {0};
        uint8*          wep_ptr = PNULL;
        uint32          curr_len = 0;
        uint32          len = 0;
        
        wep_ptr = WifiGetWep64Bit(index);
        if (PNULL != wep_ptr && 0 != strlen((char*)wep_ptr))
        {
            if (!WifiGetIsWepAscii(index))
            {
                GUIEDIT_SetMaxLen(MMIWIFI_EDIT_WEP_KEY_EDIT_CTRL_ID,MMIWIFI_WEP_64BIT_KEY_LEN*2);
                len = strlen((char*)wep_ptr);
                if (0 != len)
                {
                    for(i=0; i<len; i++)
                    {
                        curr_len = strlen((char*)temp_hex_char);
                        CFL_Snprintf((char*)temp_hex_char + curr_len ,MMIWIFI_WEP_64BIT_KEY_LEN * 2 + 1 - curr_len,"%02x",wep_ptr[i]);
                    }
                    len = strlen((char*)temp_hex_char);
                    MMI_STRNTOWSTR(temp_hex, MMIWIFI_WEP_64BIT_KEY_LEN * 2, temp_hex_char, len, MIN(len,MMIWIFI_WEP_64BIT_KEY_LEN * 2));
                    GUIEDIT_SetString(MMIWIFI_EDIT_WEP_KEY_EDIT_CTRL_ID,temp_hex,len);
                }
            }
            else
            {
                len = strlen((char*)wep_ptr);
#ifdef MMI_WLAN_AUTHORITY_COMPLEX
    GUIEDIT_SetMaxLen(MMIWIFI_EDIT_WEP_KEY_EDIT_CTRL_ID,MMIWIFI_WEP_64BIT_KEY_LEN);
#else
    GUIEDIT_SetMaxLen(MMIWIFI_EDIT_WEP_KEY_EDIT_CTRL_ID, MMIWIFI_WEP_128BIT_KEY_LEN * 2);
#endif /* MMI_WLAN_AUTHORITY_COMPLEX */
                MMI_STRNTOWSTR(temp_buf, MMIWIFI_WEP_64BIT_KEY_LEN, wep_ptr,len, MIN(len,MMIWIFI_WEP_64BIT_KEY_LEN));
                GUIEDIT_SetString(MMIWIFI_EDIT_WEP_KEY_EDIT_CTRL_ID,temp_buf,len);
            }
        }
        else
        {
#ifdef MMI_GUI_STYLE_TYPICAL
            WifiSetIsWepAscii(index, TRUE);
            GUIDROPDOWNLIST_SetCurItemIndex(MMIWIFI_EDIT_WEP_KEY_STATE_DROPDOWNLIST_CTRL_ID, 1);//是ASCII则选第二个
#endif /* MMI_GUI_STYLE_TYPICAL */
#ifdef MMI_WLAN_AUTHORITY_COMPLEX
            GUIEDIT_SetMaxLen(MMIWIFI_EDIT_WEP_KEY_EDIT_CTRL_ID,MMIWIFI_WEP_64BIT_KEY_LEN);
#else
            GUIEDIT_SetMaxLen(MMIWIFI_EDIT_WEP_KEY_EDIT_CTRL_ID, MMIWIFI_WEP_128BIT_KEY_LEN * 2);
#endif /* MMI_WLAN_AUTHORITY_COMPLEX */


        }
    }
    else
    {
        uint16          temp_buf[MMIWIFI_WEP_128BIT_KEY_LEN + 1] = {0};
        uint8           temp_hex_char[MMIWIFI_WEP_128BIT_KEY_LEN * 2 + 1] = {0};
        uint16          temp_hex[MMIWIFI_WEP_128BIT_KEY_LEN*2 + 1] = {0};
        uint8*          wep_ptr = PNULL;
        uint32          curr_len = 0;
        uint32          len = 0;
        
        wep_ptr = WifiGetWep128Bit(index);
        if (PNULL != wep_ptr && 0 != strlen((char*)wep_ptr))
        {
            if (!WifiGetIsWepAscii(index))
            {
                GUIEDIT_SetMaxLen(MMIWIFI_EDIT_WEP_KEY_EDIT_CTRL_ID,MMIWIFI_WEP_128BIT_KEY_LEN*2);
                len = strlen((char*)wep_ptr);
                if (0 != len)
                { 
                    for(i=0; i<len; i++)
                    {
                        curr_len = strlen((char*)temp_hex_char);
                        CFL_Snprintf((char*)temp_hex_char + curr_len ,MMIWIFI_WEP_128BIT_KEY_LEN * 2 + 1 - curr_len,"%02x",wep_ptr[i]);
                    }
                    len = strlen((char*)temp_hex_char);
                    MMI_STRNTOWSTR(temp_hex, MMIWIFI_WEP_128BIT_KEY_LEN * 2, temp_hex_char, len, MIN(len,MMIWIFI_WEP_128BIT_KEY_LEN * 2));
                    GUIEDIT_SetString(MMIWIFI_EDIT_WEP_KEY_EDIT_CTRL_ID,temp_hex,len);
                }
            }
            else
            {
                len = strlen((char*)wep_ptr);
                GUIEDIT_SetMaxLen(MMIWIFI_EDIT_WEP_KEY_EDIT_CTRL_ID,MMIWIFI_WEP_128BIT_KEY_LEN);
                MMI_STRNTOWSTR(temp_buf, MMIWIFI_WEP_128BIT_KEY_LEN, wep_ptr,len, MIN(len,MMIWIFI_WEP_128BIT_KEY_LEN));
                GUIEDIT_SetString(MMIWIFI_EDIT_WEP_KEY_EDIT_CTRL_ID,temp_buf,len);
            }
        }
        else
        {
#ifdef MMI_GUI_STYLE_TYPICAL
            WifiSetIsWepAscii(index, TRUE);
            GUIDROPDOWNLIST_SetCurItemIndex(MMIWIFI_EDIT_WEP_KEY_STATE_DROPDOWNLIST_CTRL_ID, 1);//是ASCII则选第二个
#endif /* MMI_GUI_STYLE_TYPICAL */
            GUIEDIT_SetMaxLen(MMIWIFI_EDIT_WEP_KEY_EDIT_CTRL_ID,MMIWIFI_WEP_128BIT_KEY_LEN);
        }
        
    }

#ifndef MMI_WLAN_AUTHORITY_COMPLEX
    GUIEDIT_SetMaxLen(MMIWIFI_EDIT_WEP_KEY_EDIT_CTRL_ID, MMIWIFI_WEP_128BIT_KEY_LEN * 2);
#endif /* MMI_WLAN_AUTHORITY_COMPLEX */
}

/*****************************************************************************/
//  Description : WifiSetEditWEPWindowForConnect
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetEditWEPWindowForConnect(void)
{
    MMIWIFI_PROFILE_T *profile_ptr = MMIWIFI_GetCurrProfile();
#ifdef MMI_GUI_STYLE_TYPICAL
    uint32                      i = 0;
    MMI_STRING_T                item_index[4] = {0};
#endif
    uint16          index = 0;

#ifdef MMI_WLAN_AUTHORITY_COMPLEX
    MMI_TEXT_ID_T               wep_key_index[4] = {TXT_WIFI_WEP_INDEX1,TXT_WIFI_WEP_INDEX2,TXT_WIFI_WEP_INDEX3,TXT_WIFI_WEP_INDEX4};
    MMI_TEXT_ID_T               wep_encrypt_index[2] = {TXT_WIFI_WEP_BIT64,TXT_WIFI_WEP_BIT128};
    MMI_TEXT_ID_T               wep_key_state_index[2] = {TXT_WIFI_WEP_KEY_HEX,TXT_WIFI_WEP_KEY_ASCII};
    MMI_TEXT_ID_T               wep_key_open_index[2] = {TXT_WIFI_WEP_KEY_SHARE_MODE, TXT_WIFI_WEP_KEY_OPEN_MODE};
    uint16 open_index = 0;
#endif // MMI_WLAN_AUTHORITY_COMPLEX
    uint16                      key_type = 0;
    BOOLEAN                     is_ascii = TRUE;
    uint16 current_index = 0;
    uint16 top_index = 0;

    //SCI_TRACE_LOW:"WifiSetEditWEPWindow called"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_15077_112_2_18_3_10_23_270,(uint8*)"");

    if (PNULL == profile_ptr)
    {
        MMK_CloseWin(MMIWIFI_AUTH_SETTING_WEP_FOR_CONNECT_WIN_ID);

        return;
    }

    index = profile_ptr->cur_wep_index;
    key_type = (uint16)profile_ptr->config_info.credential.wep_credential.key_type;
    is_ascii = profile_ptr->is_wep_ascii[index];
//#ifndef MMI_WLAN_AUTHORITY_COMPLEX
//    is_ascii = TRUE;
//#endif /* MMI_WLAN_AUTHORITY_COMPLEX */

#ifndef MMI_GUI_STYLE_TYPICAL
    GUIFORM_SetType(MMIWIFI_EDIT_WEP_FORMCTRL_ID, GUIFORM_TYPE_TP);
//    GUISETLIST_SetFontAlign(MMIWLAN_LIST_WEP_STATE_CTRL_ID, ALIGN_LVMIDDLE);
    GUIEDIT_SetAlign(MMIWIFI_EDIT_WEP_KEY_EDIT_CTRL_ID, ALIGN_LVMIDDLE);

#ifdef MMI_WLAN_AUTHORITY_COMPLEX
    current_index = GUILIST_GetCurItemIndex(MMIWLAN_LIST_WEP_STATE_CTRL_ID);
    top_index = GUILIST_GetTopItemIndex(MMIWLAN_LIST_WEP_STATE_CTRL_ID);
    GUILIST_SetMaxItem(MMIWLAN_LIST_WEP_STATE_CTRL_ID, 10, FALSE);
    GUILIST_RemoveAllItems(MMIWLAN_LIST_WEP_STATE_CTRL_ID);
    GUILIST_SetSlideState(MMIWLAN_LIST_WEP_STATE_CTRL_ID, FALSE); 

    /* Model */
    open_index = profile_ptr->config_info.credential.wep_credential.is_open_mode;
    AppendTwoTextOneImageItem(MMIWLAN_LIST_WEP_STATE_CTRL_ID, TXT_DETAIL_MODE, wep_key_open_index[open_index], IMAGE_COMMON_RIGHT_ARROW, (uint32)WifiHandleWepModelChange);

    /* Key Index */
    AppendTwoTextOneImageItem(MMIWLAN_LIST_WEP_STATE_CTRL_ID, TXT_WEP_KEY_INDEX, wep_key_index[index], IMAGE_COMMON_RIGHT_ARROW, (uint32)WifiHandleWepKeyIndexChange);

    /* Encypt */
    AppendTwoTextOneImageItem(MMIWLAN_LIST_WEP_STATE_CTRL_ID, TXT_WIFI_BIT_SETTING, wep_encrypt_index[key_type], IMAGE_COMMON_RIGHT_ARROW, (uint32)WifiHandleWepEncyptChange);

    /* State */
    AppendTwoTextOneImageItem(MMIWLAN_LIST_WEP_STATE_CTRL_ID, TXT_WEP_KEY_STYLE, wep_key_state_index[is_ascii], IMAGE_COMMON_RIGHT_ARROW, (uint32)WifiHandleWepStateChange);
#endif
    /* Password */
    GUILABEL_SetTextById(MMIWIFI_EDIT_WEP_KEY_LABEL_CTRL_ID, TXT_COMMON_PASSWORD, FALSE);

    GUILIST_SetCurItemIndex(MMIWLAN_LIST_WEP_STATE_CTRL_ID, current_index);
    GUILIST_SetTopItemIndex(MMIWLAN_LIST_WEP_STATE_CTRL_ID, top_index);

#else
    //设置label控件显示的字符  
    
    GUIFORM_SetStyle(MMIWIFI_CHILD_FORM_MODEL_CTRL_ID, GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIWIFI_CHILD_FORM_KEY_INDEX_CTRL_ID, GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIWIFI_CHILD_FORM_ENCRYPT_CTRL_ID, GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIWIFI_CHILD_FORM_STATE_CTRL_ID, GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIWIFI_CHILD_FORM_KEY_CTRL_ID, GUIFORM_STYLE_UNIT);

    GUILABEL_SetTextById(MMIWIFI_EDIT_WEP_KEY_OPEN_LABEL_CTRL_ID,TXT_DETAIL_MODE,TRUE);
    GUILABEL_SetTextById(MMIWIFI_EDIT_WEP_KEY_INDEX_LABEL_CTRL_ID,TXT_WEP_KEY_INDEX,TRUE);
    GUILABEL_SetTextById(MMIWIFI_EDIT_WEP_ENCRYPT_LABEL_CTRL_ID,TXT_WIFI_BIT_SETTING,TRUE);
    GUILABEL_SetTextById(MMIWIFI_EDIT_WEP_KEY_STATE_LABEL_CTRL_ID,TXT_WEP_KEY_STYLE,TRUE);
    GUILABEL_SetTextById(MMIWIFI_EDIT_WEP_KEY_LABEL_CTRL_ID,TXT_COMMON_PASSWORD,TRUE);
        
     //WEP Open
    for (i = 0; i < 2; i++)
    {
        MMI_GetLabelTextByLang(wep_key_open_index[i], &item_index[i]);
    }
    
    open_index = (uint16)profile_ptr->config_info.credential.wep_credential.is_open_mode;
    GUIDROPDOWNLIST_AppendItemArray(MMIWIFI_EDIT_WEP_KEY_OPEN_DROPDOWNLIST_CTRL_ID, item_index, 2);
    GUIDROPDOWNLIST_SetCurItemIndex(MMIWIFI_EDIT_WEP_KEY_OPEN_DROPDOWNLIST_CTRL_ID, open_index);
    
    //选择的WEP密钥
    for (i = 0; i < 4; i++)
    {
        MMI_GetLabelTextByLang(wep_key_index[i], &item_index[i]);
    }
    GUIDROPDOWNLIST_AppendItemArray(MMIWIFI_EDIT_WEP_KEY_INDEX_DROPDOWNLIST_CTRL_ID, item_index, 4);
    GUIDROPDOWNLIST_SetCurItemIndex(MMIWIFI_EDIT_WEP_KEY_INDEX_DROPDOWNLIST_CTRL_ID, index);
    WifiSetWepCurrIndex(index);

    //WEP加密
    for (i = 0; i < 2; i++)
    {
        MMI_GetLabelTextByLang(wep_encrypt_index[i], &item_index[i]);
    }
    GUIDROPDOWNLIST_AppendItemArray(MMIWIFI_EDIT_WEP_ENCRYPT_DROPDOWNLIST_CTRL_ID, item_index, 2);
    GUIDROPDOWNLIST_SetCurItemIndex(MMIWIFI_EDIT_WEP_ENCRYPT_DROPDOWNLIST_CTRL_ID, key_type);
    WifiSetWepKeyType((WIFISUPP_WEP_KEY_TYPE_E)key_type);
    //WEP密钥格式
    for (i = 0; i < 2; i++)
    {
        MMI_GetLabelTextByLang(wep_key_state_index[i], &item_index[i]);
    }
    GUIDROPDOWNLIST_AppendItemArray(MMIWIFI_EDIT_WEP_KEY_STATE_DROPDOWNLIST_CTRL_ID, item_index, 2);
    GUIDROPDOWNLIST_SetCurItemIndex(MMIWIFI_EDIT_WEP_KEY_STATE_DROPDOWNLIST_CTRL_ID, (uint16)is_ascii);// 1表示是ASCII
    WifiSetIsWepAscii(index,is_ascii);

    //WEP密钥的内容,在窗口打开时要读出，显示，注意区分64位和128位
#endif

//确保输入的密码长度为各种类型（64bits /128bits中的最大值，符合控件最大值设置）
    GUIEDIT_SetMaxLen(MMIWIFI_EDIT_WEP_KEY_EDIT_CTRL_ID,MMIWIFI_WEP_128BIT_KEY_LEN*2);
}
/*****************************************************************************/
//  Description : WifiSetEditWPAWPA2Window
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetEditWPAWPA2Window(void)
{
    uint32                      i = 0;
#ifdef MMI_GUI_STYLE_TYPICAL
    MMI_STRING_T                item_index[2] = {0};
#endif
    MMI_TEXT_ID_T               wpa_wpa2_presharekey_state_index[2] = {TXT_WIFI_WEP_KEY_HEX,TXT_WIFI_WEP_KEY_ASCII};
    BOOLEAN is_wpa_ascii = FALSE;
    uint32          len = 0;
    uint8*          psk_ptr = PNULL;
    uint8           temp_hex_char[WIFISUPP_WPA_PSK_LEN + 1] = {0};
    uint16          temp_hex[WIFISUPP_WPA_PSK_LEN + 1] = {0};
    uint16          temp_buf[WIFISUPP_WPA_PSK_LEN + 1] = {0};
    uint32          curr_len = 0;
    uint16 current_index = 0;
    uint16 top_index = 0;

    //SCI_TRACE_LOW:"WifiSetEditWPAWPA2Window called"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_15222_112_2_18_3_10_24_271,(uint8*)"");

    GUIFORM_SetStyle(MMIWIFI_CHILD_FORM_PRESHAREKEY_PASSWORD_CTRL_ID, GUIFORM_STYLE_UNIT);
    is_wpa_ascii = WifiGetIsWpaAscii();

#ifndef MMI_GUI_STYLE_TYPICAL
    GUIFORM_SetType(MMIWIFI_EDIT_WPA_WPA2_FORMCTRL_ID, GUIFORM_TYPE_TP);
//    GUISETLIST_SetFontAlign(MMIWLAN_LIST_WPA_STATE_CTRL_ID, ALIGN_LVMIDDLE);
    GUIEDIT_SetAlign(MMIWIFI_EDIT_WPA_WPA2_PRESHAREKEY_EDIT_CTRL_ID, ALIGN_LVMIDDLE);
    GUILABEL_SetTextById(MMIWIFI_EDIT_WPA_WPA2_PRESHAREKEY_LABEL_CTRL_ID,TXT_COMMON_PASSWORD,FALSE);
    /* STATE */
    current_index = GUILIST_GetCurItemIndex(MMIWLAN_LIST_WPA_STATE_CTRL_ID);
    top_index = GUILIST_GetTopItemIndex(MMIWLAN_LIST_WPA_STATE_CTRL_ID);
    GUILIST_SetMaxItem(MMIWLAN_LIST_WPA_STATE_CTRL_ID, 1, FALSE);
    GUILIST_RemoveAllItems(MMIWLAN_LIST_WPA_STATE_CTRL_ID);
    GUILIST_SetSlideState(MMIWLAN_LIST_WPA_STATE_CTRL_ID, FALSE);  

    AppendTwoTextOneImageItem(MMIWLAN_LIST_WPA_STATE_CTRL_ID, TXT_WIFI_WPAAUTH_PSK_STATE, wpa_wpa2_presharekey_state_index[is_wpa_ascii], IMAGE_COMMON_RIGHT_ARROW, (uint32)WifiHandleAPEditChangeWPAState);

    if(!is_wpa_ascii)
    {
        GUIEDIT_SetMaxLen(MMIWIFI_EDIT_WPA_WPA2_PRESHAREKEY_EDIT_CTRL_ID,WIFISUPP_WPA_PSK_LEN);
    }
    else
    {
        GUIEDIT_SetMaxLen(MMIWIFI_EDIT_WPA_WPA2_PRESHAREKEY_EDIT_CTRL_ID,WIFISUPP_WPA_PSK_LEN - 1);
    }

    psk_ptr = WifiGetWpaPsk();

    if (PNULL != psk_ptr && 0 != strlen((char*)psk_ptr))
    {
        if(!is_wpa_ascii)
        {
            len = strlen((char*)psk_ptr);
            if (0 != len)
            {
                for(i=0; i<len; i++)
                {
                    curr_len = strlen((char*)temp_hex_char);
                    CFL_Snprintf((char*)temp_hex_char + curr_len ,WIFISUPP_WPA_PSK_LEN  + 1 - curr_len,"%02x",psk_ptr[i]);
                }
                len = strlen((char*)temp_hex_char);
                MMI_STRNTOWSTR(temp_hex, WIFISUPP_WPA_PSK_LEN, temp_hex_char, len, MIN(len,WIFISUPP_WPA_PSK_LEN));
                GUIEDIT_SetString(MMIWIFI_EDIT_WPA_WPA2_PRESHAREKEY_EDIT_CTRL_ID,temp_hex,len);
            }
        }
        else
        {
            len = strlen((char*)psk_ptr);
            if (0 != len)
            {
                MMI_STRNTOWSTR(temp_buf, WIFISUPP_WPA_PSK_LEN, psk_ptr, len, MIN(len,WIFISUPP_WPA_PSK_LEN));
                GUIEDIT_SetString(MMIWIFI_EDIT_WPA_WPA2_PRESHAREKEY_EDIT_CTRL_ID,temp_buf,len);
            }
        }
    }

    GUILIST_SetCurItemIndex(MMIWLAN_LIST_WPA_STATE_CTRL_ID, current_index);
    GUILIST_SetTopItemIndex(MMIWLAN_LIST_WPA_STATE_CTRL_ID, top_index);
#else
    GUIFORM_SetStyle(MMIWIFI_CHILD_FORM_PRESHAREKEY_STATE_CTRL_ID, GUIFORM_STYLE_UNIT);

    //设置label控件显示的字符  
    GUILABEL_SetTextById(MMIWIFI_EDIT_WPA_WPA2_PRESHAREKEY_STATE_LABEL_CTRL_ID,TXT_WIFI_WPAAUTH_PSK_STATE,TRUE);

    GUILABEL_SetTextById(MMIWIFI_EDIT_WPA_WPA2_PRESHAREKEY_LABEL_CTRL_ID,TXT_COMMON_PASSWORD,TRUE);

    //预共享密钥格式
    for (i = 0; i < 2; i++)
    {
        MMI_GetLabelTextByLang(wpa_wpa2_presharekey_state_index[i], &item_index[i]);
    }
    GUIDROPDOWNLIST_AppendItemArray(MMIWIFI_EDIT_WPA_WPA2_PRESHAREKEY_STATE_DROPDOWNLIST_CTRL_ID, item_index, 2);

    psk_ptr = WifiGetWpaPsk();
    if (PNULL != psk_ptr && 0 != strlen((char*)psk_ptr))
    {
        GUIDROPDOWNLIST_SetCurItemIndex(MMIWIFI_EDIT_WPA_WPA2_PRESHAREKEY_STATE_DROPDOWNLIST_CTRL_ID, (uint16)is_wpa_ascii);
        if(!is_wpa_ascii)
        {
            GUIEDIT_SetMaxLen(MMIWIFI_EDIT_WPA_WPA2_PRESHAREKEY_EDIT_CTRL_ID,WIFISUPP_WPA_PSK_LEN);
            len = strlen((char*)psk_ptr);
            if (0 != len)
            {
                for(i=0; i<len; i++)
                {
                    curr_len = strlen((char*)temp_hex_char);
                    CFL_Snprintf((char*)temp_hex_char + curr_len ,WIFISUPP_WPA_PSK_LEN  + 1 - curr_len,"%02x",psk_ptr[i]);
                }
                len = strlen((char*)temp_hex_char);
                MMI_STRNTOWSTR(temp_hex, WIFISUPP_WPA_PSK_LEN, temp_hex_char, len, MIN(len,WIFISUPP_WPA_PSK_LEN));
                GUIEDIT_SetString(MMIWIFI_EDIT_WPA_WPA2_PRESHAREKEY_EDIT_CTRL_ID,temp_hex,len);
            }
        }
        else
        {
            len = strlen((char*)psk_ptr);
            if (0 != len)
            {
                GUIEDIT_SetMaxLen(MMIWIFI_EDIT_WPA_WPA2_PRESHAREKEY_EDIT_CTRL_ID,WIFISUPP_WPA_PSK_LEN - 1);
                MMI_STRNTOWSTR(temp_buf, WIFISUPP_WPA_PSK_LEN, psk_ptr, len, MIN(len,WIFISUPP_WPA_PSK_LEN));
                GUIEDIT_SetString(MMIWIFI_EDIT_WPA_WPA2_PRESHAREKEY_EDIT_CTRL_ID,temp_buf,len);
            }
        }
    }
    else
    {
        //若没有保存过密码，初始密钥格式设置为ASCII
        GUIDROPDOWNLIST_SetCurItemIndex(MMIWIFI_EDIT_WPA_WPA2_PRESHAREKEY_STATE_DROPDOWNLIST_CTRL_ID, 1);//ASCII
        GUIEDIT_SetMaxLen(MMIWIFI_EDIT_WPA_WPA2_PRESHAREKEY_EDIT_CTRL_ID,WIFISUPP_WPA_PSK_LEN - 1);
    }
#endif /* MMI_GUI_STYLE_TYPICAL */
}

/*****************************************************************************/
//  Description : WifiSetEditWPAWPA2WindowForConnect
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetEditWPAWPA2WindowForConnect(void)
{
#ifdef MMI_GUI_STYLE_TYPICAL
    uint32                      i = 0;
    MMI_STRING_T                item_index[2] = {0};
#endif
    MMI_TEXT_ID_T               wpa_wpa2_presharekey_state_index[2] = {TXT_WIFI_WEP_KEY_HEX,TXT_WIFI_WEP_KEY_ASCII};
    MMIWIFI_PROFILE_T *profile_ptr = PNULL;
    BOOLEAN is_wpa_ascii = FALSE;
        
    profile_ptr = MMIWIFI_GetCurrProfile();
    //SCI_TRACE_LOW:"WifiSetEditWPAWPA2Window called"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_15352_112_2_18_3_10_24_272,(uint8*)"");

    if (PNULL == profile_ptr)
    {
        MMK_CloseWin(MMIWIFI_AUTH_SETTING_WPA_WPA2_FOR_CONNECT_WIN_ID);

        return;
    }

    is_wpa_ascii = profile_ptr->is_wpa_ascii;
// #ifndef MMI_WLAN_AUTHORITY_COMPLEX
//     is_wpa_ascii = TRUE;
// #endif /* MMI_WLAN_AUTHORITY_COMPLEX */

#ifndef MMI_GUI_STYLE_TYPICAL
    GUIFORM_SetType(MMIWIFI_EDIT_WPA_WPA2_FORMCTRL_ID, GUIFORM_TYPE_TP);
//    GUISETLIST_SetFontAlign(MMIWLAN_LIST_WPA_STATE_CTRL_ID, ALIGN_LVMIDDLE);
    GUIEDIT_SetAlign(MMIWIFI_EDIT_WPA_WPA2_PRESHAREKEY_EDIT_CTRL_ID, ALIGN_LVMIDDLE);

    /* STATE */
    GUILIST_SetMaxItem(MMIWLAN_LIST_WPA_STATE_CTRL_ID, 1, FALSE);
    GUILIST_RemoveAllItems(MMIWLAN_LIST_WPA_STATE_CTRL_ID);
    GUILIST_SetSlideState(MMIWLAN_LIST_WPA_STATE_CTRL_ID, FALSE);  

    AppendTwoTextOneImageItem(MMIWLAN_LIST_WPA_STATE_CTRL_ID, TXT_WIFI_WPAAUTH_PSK_STATE, wpa_wpa2_presharekey_state_index[is_wpa_ascii], IMAGE_COMMON_RIGHT_ARROW, (uint32)WifiHandleChangeWPAState);
#else
    GUIFORM_SetStyle(MMIWIFI_CHILD_FORM_PRESHAREKEY_STATE_CTRL_ID, GUIFORM_STYLE_UNIT);
    //设置label控件显示的字符  
    GUILABEL_SetTextById(MMIWIFI_EDIT_WPA_WPA2_PRESHAREKEY_STATE_LABEL_CTRL_ID,TXT_WIFI_WPAAUTH_PSK_STATE,TRUE);

    //预共享密钥格式
    for (i = 0; i < 2; i++)
    {
        MMI_GetLabelTextByLang(wpa_wpa2_presharekey_state_index[i], &item_index[i]);
    }
    GUIDROPDOWNLIST_AppendItemArray(MMIWIFI_EDIT_WPA_WPA2_PRESHAREKEY_STATE_DROPDOWNLIST_CTRL_ID, item_index, 2);

    //初始密钥格式设置为ASCII
    GUIDROPDOWNLIST_SetCurItemIndex(MMIWIFI_EDIT_WPA_WPA2_PRESHAREKEY_STATE_DROPDOWNLIST_CTRL_ID, 1);

    GUIFORM_SetStyle(MMIWIFI_CHILD_FORM_PRESHAREKEY_PASSWORD_CTRL_ID, GUIFORM_STYLE_UNIT);
#endif

    GUILABEL_SetTextById(MMIWIFI_EDIT_WPA_WPA2_PRESHAREKEY_LABEL_CTRL_ID,TXT_COMMON_PASSWORD,TRUE);

    GUIEDIT_SetMaxLen(MMIWIFI_EDIT_WPA_WPA2_PRESHAREKEY_EDIT_CTRL_ID,WIFISUPP_WPA_PSK_LEN);
}

/*****************************************************************************/
//  Description : WifiSetEditWAPIWindow
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetEditWAPIWindow(void)
{
    MMI_STRING_T    str_t = {0};
    GUI_RECT_T  lab_rect = {0};
    GUI_RECT_T  list_rect = {0};
    GUI_BG_T label_bg = {0};
    uint16          select_item = 0;

    //SCI_TRACE_LOW:"WifiSetEditWAPIWindow called"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_15409_112_2_18_3_10_24_273,(uint8*)"");
    
    //设置LABEL控件
    lab_rect.left = 0;
    lab_rect.top = MMI_CLIENT_RECT_TOP;
    lab_rect.right = MMI_MAINSCREEN_RIGHT_MAX_PIXEL;
    lab_rect.bottom = lab_rect.top + 24 - 1;

    label_bg.bg_type = GUI_BG_COLOR;
    label_bg.color = MMI_BACKGROUND_COLOR;
    label_bg.shape = GUI_SHAPE_RECT;
    GUILABEL_SetRect(MMIWIFI_EDIT_WAPI_LABEL_ID, &lab_rect, FALSE);
    GUILABEL_SetBg(MMIWIFI_EDIT_WAPI_LABEL_ID,&label_bg);
    MMI_GetLabelTextByLang(TXT_WIFI_WAPI_SETTING, &str_t);
    GUILABEL_SetText(MMIWIFI_EDIT_WAPI_LABEL_ID, &str_t, FALSE);

    //设置LISTBOX控件
    list_rect.left = 0;
    list_rect.top = lab_rect.bottom + 1;
    list_rect.right = MMI_MAINSCREEN_RIGHT_MAX_PIXEL;
    list_rect.bottom = MMI_CLIENT_RECT_BOTTOM;
    
    GUILIST_SetRect(MMIWIFI_EDIT_WAPI_LISTBOX_ID,&list_rect);
    GUILIST_SetMaxItem(MMIWIFI_EDIT_WAPI_LISTBOX_ID, 2, FALSE );//max item 2
    MMIAPISET_AppendListItemByTextId(TXT_COMMON_PASSWORD,TXT_COMMON_OK,MMIWIFI_EDIT_WAPI_LISTBOX_ID,GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_WIFI_CERTIFICATE,TXT_COMMON_OK,MMIWIFI_EDIT_WAPI_LISTBOX_ID,GUIITEM_STYLE_ONE_LINE_RADIO);
    
    if (WIFISUPP_WPA_CREDENTIAL_TYPE_PSK == WifiGetWpaCredential())
    {
        select_item = 0; /* PSK */
    }
    else
    {
        select_item = 1;/* CER */
    }

    GUILIST_SetCurItemIndex(MMIWIFI_EDIT_WAPI_LISTBOX_ID, select_item);
    GUILIST_SetSelectedItem(MMIWIFI_EDIT_WAPI_LISTBOX_ID, select_item, TRUE);
}

/*****************************************************************************/
//  Description : WifiSetEditWAPIPskWindow
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetEditWAPIPskWindow(void)
{
    uint32                      i = 0;
    MMI_STRING_T                item_index[2] = {0};
    MMI_TEXT_ID_T               wpa_wpa2_presharekey_state_index[2] = {TXT_WIFI_WEP_KEY_HEX,TXT_WIFI_WEP_KEY_ASCII};
    BOOLEAN is_wapi_ascii = FALSE;
    uint32          len = 0;
    uint8*          psk_ptr = PNULL;
    uint8           temp_hex_char[WIFISUPP_WPA_PSK_LEN * 2 + 1] = {0};
    uint16          temp_hex[WIFISUPP_WPA_PSK_LEN*2 + 1] = {0};
    uint16          temp_buf[WIFISUPP_WPA_PSK_LEN + 1] = {0};
    uint32          curr_len = 0;

    //SCI_TRACE_LOW:"WifiSetEditWPAWPA2Window called"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_15466_112_2_18_3_10_24_274,(uint8*)"");

    GUIEDIT_SetIm(MMIWIFI_EDIT_WAPI_PRESHAREKEY_EDIT_CTRL_ID, GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL,GUIIM_TYPE_ABC); /*lint !e655*/
    
    is_wapi_ascii = WifiGetIsWapiAscii();
    //设置label控件显示的字符  
    GUILABEL_SetTextById(MMIWIFI_EDIT_WAPI_PRESHAREKEY_STATE_LABEL_CTRL_ID,TXT_WIFI_WPAAUTH_PSK_STATE,TRUE);
    GUILABEL_SetTextById(MMIWIFI_EDIT_WAPI_PRESHAREKEY_LABEL_CTRL_ID,TXT_COMMON_PASSWORD,TRUE);

    //预共享密钥格式
    for (i = 0; i < 2; i++)
    {
        MMI_GetLabelTextByLang(wpa_wpa2_presharekey_state_index[i], &item_index[i]);
    }
    GUIDROPDOWNLIST_AppendItemArray(MMIWIFI_EDIT_WAPI_PRESHAREKEY_STATE_DROPDOWNLIST_CTRL_ID, item_index, 2);

    psk_ptr = WifiGetWapiPsk();
    if (PNULL != psk_ptr && 0 != strlen((char*)psk_ptr))
    {
        GUIDROPDOWNLIST_SetCurItemIndex(MMIWIFI_EDIT_WAPI_PRESHAREKEY_STATE_DROPDOWNLIST_CTRL_ID, (uint16)is_wapi_ascii);
        if(!is_wapi_ascii)
        {
            GUIEDIT_SetMaxLen(MMIWIFI_EDIT_WAPI_PRESHAREKEY_EDIT_CTRL_ID,WIFISUPP_WPA_PSK_LEN*2);
            len = strlen((char*)psk_ptr);
            if (0 != len)
            {
                for(i=0; i<len; i++)
                {
                    curr_len = strlen((char*)temp_hex_char);
                    CFL_Snprintf((char*)temp_hex_char + curr_len ,WIFISUPP_WPA_PSK_LEN * 2 + 1 - curr_len,"%02x",psk_ptr[i]);
                }
                len = strlen((char*)temp_hex_char);
                MMI_STRNTOWSTR(temp_hex, WIFISUPP_WPA_PSK_LEN*2, temp_hex_char, len, MIN(len,WIFISUPP_WPA_PSK_LEN*2));
                GUIEDIT_SetString(MMIWIFI_EDIT_WAPI_PRESHAREKEY_EDIT_CTRL_ID,temp_hex,len);
            }
        }
        else
        {
            GUIEDIT_SetMaxLen(MMIWIFI_EDIT_WAPI_PRESHAREKEY_EDIT_CTRL_ID,WIFISUPP_WPA_PSK_LEN);
            len = strlen((char*)psk_ptr);
            if (0 != len)
            {
                MMI_STRNTOWSTR(temp_buf, WIFISUPP_WPA_PSK_LEN, psk_ptr, len, MIN(len,WIFISUPP_WPA_PSK_LEN));
                GUIEDIT_SetString(MMIWIFI_EDIT_WAPI_PRESHAREKEY_EDIT_CTRL_ID,temp_buf,len);
            }
        }
    }
    else
    {
        //若没有保存过密码，初始密钥格式设置为ASCII
        GUIDROPDOWNLIST_SetCurItemIndex(MMIWIFI_EDIT_WAPI_PRESHAREKEY_STATE_DROPDOWNLIST_CTRL_ID, 1);//ASCII
        GUIEDIT_SetMaxLen(MMIWIFI_EDIT_WAPI_PRESHAREKEY_EDIT_CTRL_ID,WIFISUPP_WPA_PSK_LEN);
    }
}

/*****************************************************************************/
//  Description : WifiSetEditWAPIPskForConnectWindow
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetEditWAPIPskForConnectWindow(void)
{
#ifdef MMI_GUI_STYLE_TYPICAL
    uint32                      i = 0;
    MMI_STRING_T                item_index[2] = {0};
#endif
    MMI_TEXT_ID_T               wpa_wpa2_presharekey_state_index[2] = {TXT_WIFI_WEP_KEY_HEX,TXT_WIFI_WEP_KEY_ASCII};
    MMIWIFI_PROFILE_T *profile_ptr = PNULL;
    BOOLEAN is_wapi_ascii = FALSE;

    //SCI_TRACE_LOW:"WifiSetEditWPAWPA2Window called"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_15535_112_2_18_3_10_25_275,(uint8*)"");
    
    profile_ptr = MMIWIFI_GetCurrProfile();

    if (PNULL == profile_ptr)
    {
        MMK_CloseWin(MMIWIFI_AUTH_SETTING_WAPI_PSK_FOR_CONNECT_WIN_ID);

        return;
    }

    is_wapi_ascii = profile_ptr->is_wapi_ascii;

#ifndef MMI_GUI_STYLE_TYPICAL
    GUIFORM_SetType(MMIWIFI_EDIT_WAPI_PSK_FORMCTRL_ID, GUIFORM_TYPE_TP);
//    GUISETLIST_SetFontAlign(MMIWLAN_LIST_WAPI_STATE_CTRL_ID, ALIGN_LVMIDDLE);
    GUIEDIT_SetAlign(MMIWIFI_EDIT_WAPI_PRESHAREKEY_EDIT_CTRL_ID, ALIGN_LVMIDDLE);

    /* STATE */
    GUILIST_SetMaxItem(MMIWLAN_LIST_WAPI_STATE_CTRL_ID, 1, FALSE);
    GUILIST_RemoveAllItems(MMIWLAN_LIST_WAPI_STATE_CTRL_ID);
    GUILIST_SetSlideState(MMIWLAN_LIST_WAPI_STATE_CTRL_ID, FALSE);
    AppendTwoTextOneImageItem(MMIWLAN_LIST_WAPI_STATE_CTRL_ID, TXT_WIFI_WPAAUTH_PSK_STATE, wpa_wpa2_presharekey_state_index[is_wapi_ascii], IMAGE_COMMON_RIGHT_ARROW, (uint32)WifiHandleChangeWAPIState);
#else
    GUIFORM_SetStyle(MMIWIFI_CHILD_WAPI_STATE_CTRL_ID, GUIFORM_STYLE_UNIT);

    GUILABEL_SetTextById(MMIWIFI_EDIT_WAPI_PRESHAREKEY_STATE_LABEL_CTRL_ID,TXT_WIFI_WPAAUTH_PSK_STATE,TRUE);

    //预共享密钥格式
    for (i = 0; i < 2; i++)
    {
        MMI_GetLabelTextByLang(wpa_wpa2_presharekey_state_index[i], &item_index[i]);
    }
    GUIDROPDOWNLIST_AppendItemArray(MMIWIFI_EDIT_WAPI_PRESHAREKEY_STATE_DROPDOWNLIST_CTRL_ID, item_index, 2);

#endif

    //设置label控件显示的字符  
    GUILABEL_SetTextById(MMIWIFI_EDIT_WAPI_PRESHAREKEY_LABEL_CTRL_ID,TXT_COMMON_PASSWORD,TRUE);

    GUIFORM_SetStyle(MMIWIFI_CHILD_FORM_WAPI_PASSWORD_CTRL_ID, GUIFORM_STYLE_UNIT);

    //初始密钥格式设置为ASCII
    GUIDROPDOWNLIST_SetCurItemIndex(MMIWIFI_EDIT_WAPI_PRESHAREKEY_STATE_DROPDOWNLIST_CTRL_ID, 1);//ASCII
    GUIEDIT_SetMaxLen(MMIWIFI_EDIT_WAPI_PRESHAREKEY_EDIT_CTRL_ID,WIFISUPP_WPA_PSK_LEN);
}

/*****************************************************************************/
//  Description : WifiSetEditWAPICertificateWindow
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetEditWAPICertificateWindow(void)
{
    uint16* path_ptr = PNULL;
    MMI_STRING_T        mmi_string = {0};
    
    //SCI_TRACE_LOW:"WifiSetEditWAPICertificateWindow called"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_15591_112_2_18_3_10_25_276,(uint8*)"");
    
    //设置label控件显示的字符  
    GUILABEL_SetTextById(MMIWIFI_EDIT_WAPI_CLIENT_CERTIFICATE_TEXT_LABEL_CTRL_ID,TXT_WIFI_WAPI_CLIENT_CERTIFICATE,TRUE);
    GUILABEL_SetTextById(MMIWIFI_EDIT_WAPI_PARENT_CERTIFICATE_TEXT_LABEL_CTRL_ID,TXT_WIFI_WAPI_PARENT_CERTIFICATE,TRUE);
    GUILABEL_SetTextById(MMIWIFI_LABEL_WAPI_UNINSTALL_CTRL_ID, TXT_WLAN_UNINSTALL, TRUE);
    
    path_ptr = WifiGetWapiCertiClientFilePath();
    if (PNULL != path_ptr)
    {
        if (0 != MMIAPICOM_Wstrlen(path_ptr))
        {
            mmi_string.wstr_ptr = path_ptr;
            mmi_string.wstr_len = MMIAPICOM_Wstrlen(path_ptr);
            GUILABEL_SetText(MMIWIFI_EDIT_WAPI_CLIENT_CERTIFICATE_LABEL_CTRL_ID, &mmi_string, FALSE);
        }
    }
    
    path_ptr = WifiGetWapiCertiParentFilePath();
    if (PNULL != path_ptr)
    {
        if (0 != MMIAPICOM_Wstrlen(path_ptr))
        {
            mmi_string.wstr_ptr = path_ptr;
            mmi_string.wstr_len = MMIAPICOM_Wstrlen(path_ptr);
            GUILABEL_SetText(MMIWIFI_EDIT_WAPI_PARENT_CERTIFICATE_LBAEL_CTRL_ID, &mmi_string, FALSE);
        }
    }
}
/*****************************************************************************/
//  Description : WifiSetEditWAPICertificateForConnectWindow
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void WifiSetEditWAPICertificateForConnectWindow(void)
{
    //SCI_TRACE_LOW:"WifiSetEditWAPICertificateForConnectWindow called"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_15626_112_2_18_3_10_25_277,(uint8*)"");

    GUIFORM_SetStyle(MMIWIFI_CHILD_FORM_CLIENT_CTRL_ID, GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIWIFI_CHILD_FORM_PARENT_CTRL_ID, GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIWIFI_CHILD_FORM_UNINSTALL_CTRL_ID, GUIFORM_STYLE_UNIT);

#ifndef MMI_GUI_STYLE_TYPICAL
    GUIFORM_SetType(MMIWIFI_EDIT_WAPI_CERTIFICATE_FORMCTRL_ID, GUIFORM_TYPE_TP);
#endif

    //设置label控件显示的字符  
    GUILABEL_SetTextById(MMIWIFI_EDIT_WAPI_CLIENT_CERTIFICATE_TEXT_LABEL_CTRL_ID,TXT_WIFI_WAPI_CLIENT_CERTIFICATE,TRUE);
    GUILABEL_SetTextById(MMIWIFI_EDIT_WAPI_PARENT_CERTIFICATE_TEXT_LABEL_CTRL_ID,TXT_WIFI_WAPI_PARENT_CERTIFICATE,TRUE);
    GUILABEL_SetTextById(MMIWIFI_LABEL_WAPI_UNINSTALL_CTRL_ID, TXT_WLAN_UNINSTALL, TRUE);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL int32 WifiGetExcludingListIndex(char *str_ptr)
{
    MMIWIFI_PROXY_REJECTLIST_E rejectlist = {0};
    uint16 index = 0;
    BOOLEAN result = FALSE;
    char * lower_str = PNULL;
    uint16 len = 0;

    if (PNULL == str_ptr)
    {
        return -1;
    }

    len = strlen(str_ptr);

    lower_str = SCI_ALLOCA(len+1);

    if (PNULL == lower_str)
    {
        return -1;
    }

    SCI_MEMSET(lower_str, 0, (len + 1));

    SCI_MEMCPY(lower_str, str_ptr, len);
    MMIAPICOM_Strlower(lower_str);

    MMIWIFI_GetProxyRejectList(&rejectlist);
    for(index=0;index<MMIWIFI_MAX_REJECTLIST_NUM;index++)
    {
        if(0 == strlen(rejectlist.proxy_rejectlist[index]))
        {
            break;
        }
        else if(PNULL != strstr(lower_str, rejectlist.proxy_rejectlist[index]))//(len == strlen(rejectlist.proxy_rejectlist[index])) && (0 == stricmp(lower_str , rejectlist.proxy_rejectlist[index])))
        {
            result = TRUE;
            break;
        }
    }

    SCI_FREE(lower_str);

    //SCI_TRACE_LOW:"MMIAPIWIFI_IsExistInExcludingList result is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_15689_112_2_18_3_10_25_278,(uint8*)"d", result);

    return result ? index : -1;
}

/*****************************************************************************/
//  Description : MMIAPIWIFI_IsExistInExcludingList
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIWIFI_IsExistInExcludingList(char *str_ptr)
{
    return (WifiGetExcludingListIndex(str_ptr) >= 0);
}

/*****************************************************************************/
//  Description : MMIAPIWIFI_IsChlidListWinOpen
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIWIFI_IsChildListWinFocus(void)
{
    return MMK_IsFocusWin(MMIWIFI_LIST_WIN_ID);
}

/*****************************************************************************/
//  Description : Check List Win
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_IsListWinOpen(void)
{
    return MMK_IsOpenWin(MMIWIFI_LIST_WIN_ID);
}

/*****************************************************************************/
//  Description : append item to listbox 
//  Global resource dependence : none
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIWIFI_AppendSelectListItem(MMI_CTRL_ID_T ctrl_id)
{
    MMI_STRING_T sim_str = {0};
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
#ifdef MMI_MULTI_SIM_SYS_SINGLE 
    wchar opn_spn_name[MMIPHONE_MAX_OPER_NAME_LEN] = {0};
    MMI_STRING_T network_str = {0};
#endif
    uint16 list_item_max = MMI_DUAL_SYS_MAX;

#ifdef MMI_WIFI_SUPPORT
    list_item_max += 1;
#endif
    GUILIST_SetMaxItem(ctrl_id, list_item_max, FALSE);


#ifndef MMI_MULTI_SIM_SYS_SINGLE 
#ifdef MMI_GPRS_DATA_SIM_SET
    MMIAPISET_AppendMultiSimName(ctrl_id, TXT_COMMON_OK, list_item_max, MMISET_APPEND_GPRS_MODE);
#else
    MMIAPISET_AppendMultiSimName(ctrl_id, TXT_COMMON_OK, list_item_max, MMISET_APPEND_SYS_OK);
#endif
#else
    item_t.item_style = GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TEXT;
    item_t.item_data_ptr = &item_data;
    item_t.user_data = MN_DUAL_SYS_1;

    item_data.item_content[0].item_data_type = GUIITEM_DATA_NONE;

    MMI_GetLabelTextByLang(TXT_GPRS,&sim_str);
    
    item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[1].item_data.text_buffer.wstr_len = sim_str.wstr_len;
    
    item_data.item_content[1].item_data.text_buffer.wstr_ptr = sim_str.wstr_ptr;

    //get opn_spn information for display in sim selection page!!
    SCI_MEMSET(&network_str, 0x00, sizeof(network_str));
    network_str.wstr_ptr = (wchar *)opn_spn_name;
    SCI_MEMSET(network_str.wstr_ptr, 0x00, sizeof(opn_spn_name));
    MMIAPISET_GetSimServiceString(MN_DUAL_SYS_1, &network_str);

    item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[2].item_data.text_buffer.wstr_len = network_str.wstr_len;
    
    item_data.item_content[2].item_data.text_buffer.wstr_ptr = network_str.wstr_ptr;

    //set softkey
    item_data.softkey_id[0] = TXT_COMMON_OK;
    item_data.softkey_id[1] = TXT_NULL;
    item_data.softkey_id[2] = STXT_RETURN;
    
    if (!GUILIST_AppendItem(ctrl_id, &item_t))
    {
        SCI_TRACE_LOW("MMIAPIWIFI_AppendSelectListItem failed");
    }
#endif

#ifdef MMI_WIFI_SUPPORT
    SCI_MEMSET(&item_t, 0x00, sizeof(item_t));
    SCI_MEMSET(&item_data, 0x00, sizeof(item_data));

    item_t.item_style = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
    item_t.item_data_ptr = &item_data;
    item_t.user_data = MMI_DUAL_SYS_MAX;//WIFI
    MMI_GetLabelTextByLang(TXT_WIFI,&sim_str);
    
    item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
//    item_data.item_content[0].item_data.image_id = MAIN_GetIdleWifiIcon();
                                 

    item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[1].item_data.text_buffer.wstr_len = sim_str.wstr_len;
    item_data.item_content[1].item_data.text_buffer.wstr_ptr = sim_str.wstr_ptr;

    //set softkey
    item_data.softkey_id[0] = TXT_COMMON_OK;
    item_data.softkey_id[1] = TXT_NULL;
    item_data.softkey_id[2] = STXT_RETURN;
    
    if (!GUILIST_AppendItem(ctrl_id, &item_t))
    {
        SCI_TRACE_LOW("MMIAPIWIFI_AppendSelectListItem failed");
    }
#endif

    GUILIST_SetOwnSofterKey(ctrl_id, TRUE);

    return TRUE;
}

/*****************************************************************************/
//  Description : Set addrs
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_SetIpAddress(TCPIP_NETIF_IPADDR_T *addrs)
{
    if (PNULL == addrs)
    {
        return FALSE;
    }

    s_ui_context.addrs.dns1 = ntohl(addrs->dns1);
    s_ui_context.addrs.dns2 = ntohl(addrs->dns2);
    s_ui_context.addrs.gateway = ntohl(addrs->gateway);
    s_ui_context.addrs.ipaddr = ntohl(addrs->ipaddr);
    s_ui_context.addrs.snmask = ntohl(addrs->snmask);

    return TRUE;
}

#ifdef WLAN_RESELECT_SUPPORT
/****************************************************************************/
//  Description : Reselect Ap
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note:
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_ReSelectAP(void)
{
    //SCI_TRACE_LOW:"MMIWIFI_ReSelectAP"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_15825_112_2_18_3_10_25_279,(uint8*)"");

    if (MMIWIFI_GetOnAirTrustedProfileNum() > 0)
    {
        if (MMI_AP_AUTO_RESELCET == MMIWIFI_GetApReselect())
        {
            //SCI_TRACE_LOW:"MMIWIFI_ReSelectAP MMIWIFI_AutoConnect"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_15831_112_2_18_3_10_25_280,(uint8*)"");
            MMIWIFI_AutoConnect();
        }
        else
        {
            MMIWIFI_OpenSelectAPWin();
        }
    }
    else
    {
        if (!MMIWIFI_ChangeServiceToGPRS())
        {
            MMIAPIPDP_WifiDeactiveInd();
        }
    }

    return TRUE;
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note:
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_ChangeServiceToGPRS(void)
{
    //SCI_TRACE_LOW:"MMIWIFI_ChangeServiceToGPRS"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_15856_112_2_18_3_10_25_281,(uint8*)"");

    if (MMIAPIPHONE_IsSimAvailable(MN_DUAL_SYS_1) && MMIWIFI_IsNeedServiceChange())
    {
        MMI_STRING_T string_notify = {0};

        //SCI_TRACE_LOW:"MMIWIFI_ChangeServiceToGPRS Query"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_15862_112_2_18_3_10_25_282,(uint8*)"");
        MMIRES_GetText(TXT_WLAN_CHANGE_TO_CHINAMOBILE_NET, PNULL, &string_notify);
        MMIPUB_OpenQueryWinByTextPtrEx(MMK_GetFirstAppletHandle(), &string_notify, PNULL, IMAGE_PUBWIN_QUERY, PNULL, MMIWIFI_HandleChangeService, (uint32)TRUE);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
#endif

/****************************************************************************/
//  Description : handle select ap window
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E WifiHandleSelectAPWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"WifiHandleListWinMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_15882_112_2_18_3_10_25_283,(uint8*)"d", msg_id);

    switch(msg_id) 
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(MMIWIFI_SELECT_AP_LIST_CTRL_ID, MMIWIFI_MAX_LIST_NUM, FALSE);
        MMK_SetAtvCtrl(win_id, MMIWIFI_SELECT_AP_LIST_CTRL_ID);
        break;

    case MSG_FULL_PAINT:
        WifiUpdateSelectAPList(MMIWIFI_SELECT_AP_LIST_CTRL_ID);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        {
            uint16 index = 0;
            MMIWIFI_PROFILE_T *profile_ptr;

            index = GUILIST_GetCurItemIndex(MMIWIFI_SELECT_AP_LIST_CTRL_ID);
            profile_ptr = (MMIWIFI_PROFILE_T *)WifiGetListUserdata(MMIWIFI_SELECT_AP_LIST_CTRL_ID, index);

            if (PNULL == profile_ptr)
            {
                break;
            }

            if (MMIWIFI_Connect(profile_ptr))
            {
                MMIWIFI_OpenConnectWaitWin();
            }
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

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_IsNeedServiceChange(void)
{
    BOOLEAN ret = FALSE;

    /* using GPRS */
#ifdef DL_SUPPORT
    if (MMIAPIDL_GetDownloadStatus()) /* Only DL support */
    {
        ret = TRUE;
    }
#endif

    //SCI_TRACE_LOW:"MMIWIFI_IsNeedServiceChange ret = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_15948_112_2_18_3_10_25_284,(uint8*)"d", ret);
    return ret;
}

/*****************************************************************************/
// Description : Handle query change service
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWIFI_HandleChangeService(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;   
    BOOLEAN is_to_gprs = FALSE;
    MMIPUB_INFO_T *win_info_ptr = PNULL;
    
    win_info_ptr = (MMIPUB_INFO_T *)MMK_GetWinAddDataPtr(win_id);
    
    if (PNULL != win_info_ptr)
    {
        is_to_gprs = (BOOLEAN)win_info_ptr->user_data;
    }
    //SCI_TRACE_LOW:"MMIWIFI_HandleChangeService msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_15968_112_2_18_3_10_25_285,(uint8*)"d", msg_id);
 
    switch(msg_id)
    {  
    
    case MSG_OPEN_WINDOW:
        GUIWIN_SetSoftkeyTextId(win_id, TXT_YES, TXT_NULL, TXT_COMM_NO, FALSE);
        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        if (is_to_gprs)
        {
#ifdef DL_SUPPORT
            MMIAPIDL_HandleServiceChange(MMIPDP_INTERFACE_GPRS);
#endif
        }
        else
        {
            MMIWIFI_SetPromptResult(TRUE);
#ifdef DL_SUPPORT  
            MMIAPIDL_HandleServiceChange(MMIPDP_INTERFACE_WIFI);
#endif
        }

        MMK_CloseWin(win_id);
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        if(!is_to_gprs)
        {
           MMIWIFI_SetPromptResult(FALSE);
        } 
        MMK_CloseWin(win_id);
        break;

    default:
        result = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }
    return (result);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIWIFI_SetIsShowNoWlanPrompt(BOOLEAN flag)
{
    MMIWIFI_SetIsShowNoWlanPrompt(flag);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIWIFI_GetIsShowNoWlanPrompt(void)
{
    return MMIWIFI_GetIsShowNoWlanPrompt();
}

#ifndef MMI_GUI_STYLE_TYPICAL
/****************************************************************************/
//  Description : WifiAppendListItem
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: Ordered by state:Connected Trusted Untrusted Unavailable
//           Ordered by rate if the State is the same
/****************************************************************************/
LOCAL uint16 WifiListGetProfileOrderIndex(MMI_CTRL_ID_T ctrl_id, MMIWIFI_PROFILE_T *in_profile_ptr)
{
    uint16              order_index = 0;
    uint16              item_num = 0;
    MMIWIFI_PROFILE_T   *profile_ptr = PNULL;
    uint16              i = 0;     

    if (PNULL == in_profile_ptr)
    {
        return 0;
    }

    item_num = GUILIST_GetTotalItemNum(ctrl_id);

    if ((MMIWIFI_PROFILE_STATE_CONNECTED == in_profile_ptr->state) ||
        (MMIWIFI_PROFILE_STATE_DHCPED    == in_profile_ptr->state)) {
        return 0;
    }

    if (!in_profile_ptr->is_on_air)
    {
        order_index = GUILIST_GetTotalItemNum(ctrl_id);
        return order_index;

    }

    for (i =0; i < item_num; i++)
    {
        profile_ptr = (MMIWIFI_PROFILE_T *)WifiGetListUserdata(ctrl_id, i);

        if  ((PNULL == profile_ptr) ||
             (in_profile_ptr->is_on_air && !profile_ptr->is_on_air) ||
             (in_profile_ptr->is_on_air && profile_ptr->is_on_air && in_profile_ptr->is_trusted && !profile_ptr->is_trusted) ||
             (in_profile_ptr->is_on_air && profile_ptr->is_on_air && in_profile_ptr->is_trusted && profile_ptr->is_trusted && in_profile_ptr->rssi > profile_ptr->rssi) ||
             (in_profile_ptr->is_on_air && profile_ptr->is_on_air && !in_profile_ptr->is_trusted && !profile_ptr->is_trusted && in_profile_ptr->rssi > profile_ptr->rssi))
        {
            break;
        }

        order_index++;
    }

    return order_index;
}

/****************************************************************************/
//  Description : WifiAppendListItem
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void WifiListAppendPermanentItem(MMI_CTRL_ID_T ctrl_id)
{
    MMI_IMAGE_ID_T      image_id = IMAGE_NULL;
    BOOLEAN             is_wlan_on = FALSE;
    MMI_STRING_T        string1 = {0};
    MMI_STRING_T        string2 = {0};
    wchar               disp_wchar[WIFISUPP_SSID_MAX_LEN + 1] = {0};
    wchar               *temp_ptr = PNULL;
    uint16              str_len = 0;
    MMIWIFI_PROFILE_T   *profile_ptr = PNULL;
    uint16              item_num = 0;

    temp_ptr = disp_wchar;

    is_wlan_on = MMIWIFI_GetIsWlanOn();

    image_id = is_wlan_on ? IMAGE_CHECK_SELECTED_ICON : IMAGE_CHECK_UNSELECTED_ICON;

    /* ON/OFF */
    if (is_wlan_on)
    {
        if (MMIWIFI_STATUS_CONNECTED == MMIAPIWIFI_GetStatus())
        {
            profile_ptr = (MMIWIFI_PROFILE_T *)WifiGetListUserdata(ctrl_id, 0);

            MMIRES_GetText(TXT_COMMON_CONNECTED, PNULL, &string2);

            if (PNULL != profile_ptr && (MMIWIFI_PROFILE_STATE_CONNECTED == profile_ptr->state || MMIWIFI_PROFILE_STATE_DHCPED == profile_ptr->state))
            {
                if (profile_ptr->config_info.ssid.ssid_len + string2.wstr_len + 1 <= WIFISUPP_SSID_MAX_LEN)
                {
                    str_len = GUI_UTF8ToWstr(temp_ptr, profile_ptr->config_info.ssid.ssid_len, profile_ptr->config_info.ssid.ssid, profile_ptr->config_info.ssid.ssid_len);

                    temp_ptr += str_len;

                    *temp_ptr = ' ';

                    temp_ptr++;

                    MMIAPICOM_Wstrncpy(temp_ptr, string2.wstr_ptr, string2.wstr_len);
                }
                else
                {
                    str_len = MIN(WIFISUPP_SSID_MAX_LEN, string2.wstr_len);
                    MMIAPICOM_Wstrncpy(temp_ptr, string2.wstr_ptr, str_len);
                }
            }
            else
            {
                str_len = MIN(WIFISUPP_SSID_MAX_LEN, string2.wstr_len);
                MMIAPICOM_Wstrncpy(temp_ptr, string2.wstr_ptr, str_len);
            }

            string2.wstr_ptr = disp_wchar;
            string2.wstr_len = MMIAPICOM_Wstrlen(disp_wchar);
            string2.wstr_len = MIN(WIFISUPP_SSID_MAX_LEN, string2.wstr_len);
        }
        else
        {
            if (MMIWIFI_GetScanStatus())
            {
                MMIRES_GetText(TXT_SCANING_WLAN, PNULL, &string2);
            }
            else
            {
                MMIRES_GetText(TXT_DISCONNECTED, PNULL, &string2);
            }
        }
    }
    else
    {
        MMIRES_GetText(TXT_CLOSE, PNULL, &string2);
    }

    MMIRES_GetText(TXT_WIFI_WLAN, PNULL, &string1);

    InsertTwoTextOneImageItemByString(ctrl_id, 0, &string1,  &string2, image_id, (uint32)WifiSettingHandleOn);

    item_num = GUILIST_GetTotalItemNum(ctrl_id);

    InsertTwoTextOneImageItem(ctrl_id, item_num, TXT_WLAN_ADD_AP, TXT_NULL, IMAGE_NULL, (uint32)WifiAddTrustedAp);

    if (!is_wlan_on)
    {
        GUILIST_SetItemGray(ctrl_id, item_num, TRUE);
    }
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void WifiAddTrustedAp(void)
{
    if(MMIWIFI_MAX_TRUSTED_LIST_NUM > MMIWIFI_GetTrustedProfileNum())
    {
        MMIWIFI_OpenNewWin();
    }
    else
    {
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_WIFI_TRUSTED_LIST_FULL,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL); 
    }
}

/*****************************************************************************/
// Description : Handle query acitve service
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E WifiHandleQueryForgetAP(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        {
            MMIPUB_INFO_T *win_info_ptr = PNULL;
            MMIWIFI_PROFILE_T *profile_ptr = PNULL;

            win_info_ptr = (MMIPUB_INFO_T *)MMIPUB_GetWinAddDataPtr(win_id);

            if(PNULL == win_info_ptr)
            {
                break;
            }

            profile_ptr = (MMIWIFI_PROFILE_T *)(win_info_ptr->user_data);

            if (PNULL != profile_ptr && (profile_ptr->state != MMIWIFI_PROFILE_STATE_DISCONNECTED && profile_ptr->state != MMIWIFI_PROFILE_STATE_DISCONNECTING))
            {
                MMIWIFI_DisConnect(profile_ptr);
            }

            if(PNULL != profile_ptr && profile_ptr ->is_on_air)
            {
                profile_ptr ->is_trusted = FALSE;
                MMIWIFI_UpdateProfileFile();
            }
            else
            {
                if (MMIWIFI_DeleteProfileByAp(profile_ptr))
                {
                    MMIWIFI_UpdateProfileFile();
                }
            }

            MMK_CloseWin(win_id);
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
// Description : Handle query acitve service
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E WifiHandleQueryEditAP(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        {
            MMIPUB_INFO_T *win_info_ptr = PNULL;
            MMIWIFI_PROFILE_T *profile_ptr = PNULL;

            win_info_ptr = (MMIPUB_INFO_T *)MMIPUB_GetWinAddDataPtr(win_id);

            if(PNULL == win_info_ptr)
            {
                break;
            }

            profile_ptr = (MMIWIFI_PROFILE_T *)(win_info_ptr->user_data);

            if (PNULL != profile_ptr && (profile_ptr->state != MMIWIFI_PROFILE_STATE_DISCONNECTED && profile_ptr->state != MMIWIFI_PROFILE_STATE_DISCONNECTING))
            {
                MMIWIFI_DisConnect(profile_ptr);
            }

            MMIWIFI_OpenEditWin(profile_ptr);

            MMK_CloseWin(win_id);
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
/****************************************************************************/
//  Description : WifiAppendListItem
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL BOOLEAN WifiListIsAPItemSelected(uint16 index)
{
#ifdef MMI_WLAN_TAB_STYLE
        return TRUE;
#else
    BOOLEAN         ret = TRUE;
    GUILIST_ITEM_T* item_ptr = PNULL;

    item_ptr = (GUILIST_ITEM_T*)GUILIST_GetItemPtrByIndex(MMIWIFI_LIST_CTRL_ID, index);

    if (PNULL == item_ptr ||
        GUIITEM_STYLE_ONE_LINE_TEXT_ICON == item_ptr->item_style || // "Add AP" Item
        GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM == item_ptr->item_style) // "On/Off" Item
    {
        ret = FALSE;
    }
#endif

    //SCI_TRACE_LOW:"WifiListIsAPItemSelected index = %d, ret = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_16313_112_2_18_3_10_26_286,(uint8*)"dd", index, ret);

    return ret;
}

/*****************************************************************************/
//  Description :Get Wlan Status icon
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T   MMIAPIWIFI_GetWifiStatusIcon(void)
{

        if (MMIAPIWIFI_GetStatus() != MMIWIFI_STATUS_OFF)
        {
            return IMAGE_WIFI_ON_ICON;
        }
        else
        {
            return IMAGE_WIFI_OFF_ICON;
        }

}

/****************************************************************************/
//  Description : WifiUpdateTrustedList
//  Global resource dependence : 
//  Author:
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  wifiSIMSelectCallback(uint32 dual_sys, BOOLEAN isOK, DPARAM param)
{
    MMIWIFI_PROFILE_T *profile_ptr = PNULL;

    profile_ptr = (MMIWIFI_PROFILE_T *)param;

    if (PNULL == profile_ptr)
    {
        return MMI_RESULT_TRUE;
    }

    if (isOK)
    {
#ifdef WLAN_EAP_SUPPORT
        profile_ptr->config_info.credential.wpa_credential.credential_info.wpa_eap_info.eap_info.eapsim_info.simcard_num = dual_sys;
        profile_ptr->config_info.credential.wpa_credential.credential_info.wpa_eap_info.eap_type = WIFISUPP_WPA_EAP_TYPE_SIM;
#endif /* WLAN_EAP_SUPPORT */
        MMIWIFI_OpenConnectWaitWin();
        (void)MMIWIFI_DirectConnect(&(profile_ptr->config_info));/*lint !e831 !e613*/
        profile_ptr->state = MMIWIFI_PROFILE_STATE_CONNECTING;/*lint !e831 !e613*/
    }

    profile_ptr->is_editing = FALSE;/*lint !e831 !e613*/

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : open the sim select window of browser
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWLAN_OpenSimSelectWin(ADD_DATA user_data)
{
    MMI_WIN_ID_T	selectSimWinID = MMIWIFI_SIM_SELECT_WIN_ID;
    MMIPHONE_SELECT_SIM_DATA_T  caller_data={0};

    caller_data.append_type = (uint32)MMISET_APPEND_SIM_OK;
    caller_data.callback_func =wifiSIMSelectCallback ;
    caller_data.select_type= MMIPHONE_SELECTION_SIM_ONLY;
    caller_data.publist_id = MMIWIFI_COM_SELECT_SIM_CTRL_ID;
    caller_data.win_id_ptr = &selectSimWinID;
    caller_data.user_data  = PNULL;
    caller_data.extra_data = user_data;

    MMIAPIPHONE_OpenSelectSimWin(&caller_data);
}

/*****************************************************************************/
//  Description : Check WEP key
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN wifi_CheckWepKeyValid(MMI_STRING_T *key, BOOLEAN *out_is_128, BOOLEAN *out_is_asscii)
{
    BOOLEAN ret = FALSE;

    if (PNULL == out_is_128 || PNULL == out_is_asscii)
    {
        return FALSE;
    }

    if (MMIWIFI_WEP_64BIT_KEY_LEN == key->wstr_len)
    {
        *out_is_asscii = TRUE;
        *out_is_128 = FALSE;
        ret = TRUE;
    }
    else if (MMIWIFI_WEP_128BIT_KEY_LEN == key->wstr_len) 
    {
        *out_is_asscii = TRUE;
        *out_is_128 = TRUE;
        ret = TRUE;
    }
    else if (MMIWIFI_WEP_64BIT_KEY_LEN * 2 == key->wstr_len) 
    {
        *out_is_asscii = FALSE;
        *out_is_128 = FALSE;
        ret = TRUE;
    }
    else if (MMIWIFI_WEP_128BIT_KEY_LEN * 2 == key->wstr_len) 
    {
        *out_is_asscii = FALSE;
        *out_is_128 = TRUE;
        ret = TRUE;
    }
    else
    {
        ret = FALSE;
    }    

    if (ret && !(*out_is_asscii))
    {
        uint8   temp_char[MMIWIFI_WEP_128BIT_KEY_LEN * 2 + 1] = {0};

        MMI_WSTRNTOSTR(temp_char, MMIWIFI_WEP_128BIT_KEY_LEN * 2, key->wstr_ptr, key->wstr_len, key->wstr_len);

        ret = WifiCheckIsHex(temp_char, key->wstr_len);
    }

    SCI_TRACE_LOW("WifiHandleMainParentWinMsg ret = %d, is_ascii = %d, is_128 = %d", ret, *out_is_asscii, *out_is_128);
 
    return ret;
}

/****************************************************************************/
//  Description : Tidy window
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: when
/****************************************************************************/
LOCAL void  wifi_TidyWindow(void)
{
    MMI_WIN_ID_T win_id = MMIWIFI_WIN_ID_START;

    for (win_id = MMIWIFI_WIN_ID_START + 1; win_id <  MMIWIFI_MAX_WIN_ID; win_id++)
    {
        if (MMIWIFI_LIST_WIN_ID == win_id || MMIWIFI_MAIN_WIN_ID == win_id)
        {
            continue;
        }

#ifdef MMI_WLAN_TAB_STYLE
        if (MMIWIFI_MAIN_PARENT_WIN_ID == win_id
            && MMIWIFI_TRUSTED_LIST_CHILD_WIN_ID == win_id
            && MMIWIFI_SETTING_LIST_CHILD_WIN_ID == win_id)
        {
            continue;
        }
#endif


        if (MMK_IsOpenWin(win_id))
        {
            MMK_CloseWin(win_id);
        }
    }
}

/*****************************************************************************/
//  Description : check whether user is operating wifi list
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_IsOperating(void)
{
    BOOLEAN ret = FALSE;

    if (MMK_IsOpenWin(MMIWIFI_NEW_WIN_ID)
        || MMK_IsOpenWin(MMIWIFI_EDIT_WIN_ID)
        || MMK_IsOpenWin(MMIWIFI_AUTH_SETTING_WEP_FOR_CONNECT_WIN_ID)
        || MMK_IsOpenWin(MMIWIFI_AUTH_SETTING_WPA_WPA2_FOR_CONNECT_WIN_ID)
        || MMK_IsOpenWin(MMIWIFI_AUTH_SETTING_WAPI_PSK_FOR_CONNECT_WIN_ID)
        || MMK_IsOpenWin(MMIWIFI_AUTH_SETTING_WAPI_CERT_CONNECT_WIN_ID)
#ifdef WLAN_EAP_SUPPORT
        || MMK_IsOpenWin(MMIWIFI_EAP_TYPE_SELECT_WIN_ID)
        || MMK_IsOpenWin(MMIWIFI_EAP_ACCOUNT_WIN_ID)
#endif /* WLAN_EAP_SUPPORT */
        || MMK_IsOpenWin(MMIWIFI_AP_PROPERTY_WIN_ID)
        || MMK_IsOpenWin(MMIWIFI_SIM_SELECT_WIN_ID)
        || MMK_IsOpenWin(MMIWIFI_AP_CONNECT_WIN_ID)
        || MMK_IsOpenWin(MMIWIFI_PASSWORD_WIN_ID)
        )
    {
        ret = TRUE;
    }

    SCI_TRACE_LOW("MMIWIFI_IsOperating ret = %x", ret);
    return ret;
}

#ifdef MMI_WLAN_TAB_STYLE
/****************************************************************************/
//  Description : handle wifi main window
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleMainParentWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"WifiHandleMainParentWinMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_16395_112_2_18_3_10_26_287,(uint8*)"d", msg_id);

    switch (msg_id) {
    case MSG_OPEN_WINDOW:
        MMIWIFI_SetMainTabProperty(MMIWIFI_TAB_MAIN_CTRL_ID);
        MMK_SetAtvCtrl(win_id, MMIWIFI_TAB_MAIN_CTRL_ID);
        break;


    case MSG_LCD_SWITCH:
        {
            uint16 screen_width = 0;
            uint16 screen_height = 0;

            GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &screen_width, &screen_height);
//            GUITAB_SetItemWidth(MMIWIFI_TAB_MAIN_CTRL_ID, screen_width / MMIWIFI_ENTRY_TYPE_MAX);
        }
        break;

    case MSG_CLOSE_WINDOW:
        if(!MMIWIFI_GetIsWlanOn())
        {
            MMIWIFI_ProfileFinalize();
            MMIWIFI_SetIsInitialized(FALSE);
        }
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/****************************************************************************/
//  Description : handle wifi trusted list window
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleTrustedlistChildWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"WifiHandleTrustedlistChildWinMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_16438_112_2_18_3_10_26_288,(uint8*)"d", msg_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUI_BOTH_RECT_T both_rect = {0};

            both_rect = MMITHEME_GetWinClientBothRect(win_id);

            GUIAPICTRL_SetBothRect(MMIWIFI_LIST_TRUSTED_CTRL_ID, &both_rect);
            GUILIST_SetNeedPrgbar(MMIWIFI_LIST_TRUSTED_CTRL_ID, TRUE);
            MMK_SetAtvCtrl(win_id, MMIWIFI_LIST_TRUSTED_CTRL_ID);
            GUILIST_SetMaxItem(MMIWIFI_LIST_TRUSTED_CTRL_ID, MMIWIFI_MAX_TRUSTED_LIST_NUM, FALSE);
        }
        break;

    case MSG_FULL_PAINT:
        WifiUpdateTrustedList(MMIWIFI_LIST_TRUSTED_CTRL_ID);
        break;

    case MSG_GET_FOCUS:
#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
        GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, TXT_NULL, FALSE);
        GUIWIN_SetTitleButtonImage(win_id, GUITITLE_BUTTON_RIGHT, IMAGE_ISTYLE_TITLE_ADD, IMAGE_ISTYLE_TITLE_ADD_DOWN, FALSE);
        GUIWIN_SetTitleButtonHandleMsgInfo(win_id, GUITITLE_BUTTON_RIGHT, MSG_NOTIFY_TITLE_MENU_ICON, MMIWIFI_LIST_TRUSTED_CTRL_ID);
#endif
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_MENU:
        WifiOpenConnectMenuWin(win_id);
        break;

#ifndef MMI_GUI_STYLE_TYPICAL
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        {
            uint16                  index = 0;
            MMIWIFI_PROFILE_T*      profile_ptr = PNULL;
            
            index = GUILIST_GetCurItemIndex(MMIWIFI_LIST_TRUSTED_CTRL_ID);
            profile_ptr = (MMIWIFI_PROFILE_T *)WifiGetListUserdata(MMIWIFI_LIST_TRUSTED_CTRL_ID, index);

            if (PNULL != profile_ptr)
            {
#ifdef WLAN_PORTAL_SUPPORT
                if(MMIWIFI_APIsCMCC(profile_ptr))
                {
                    MMIAPIWIFI_OpenPortalSettingsWin();
                    break;
                }

                if(MMIWIFI_APIsCMCCEDU(profile_ptr))
                {
                    break;
                }
#endif /* WLAN_PORTAL_SUPPORT */
                if (profile_ptr->state == MMIWIFI_PROFILE_STATE_DISCONNECTED || profile_ptr->state == MMIWIFI_PROFILE_STATE_DISCONNECTING)
                {
                    MMIWIFI_OpenEditWin(profile_ptr);
                }
                else
                {
                    MMIPUB_OpenQueryWinByTextIdEx(MMK_GetFirstAppletHandle(), TXT_WLAN_IS_DELETE_CONNECTED, IMAGE_PUBWIN_QUERY, PNULL, HandleQueryEditConnectedAP, (uint32)profile_ptr);
                }
            }
        }
        break;


    case MSG_CTL_LIST_LONGOK:
            if (0 != GUILIST_GetTotalItemNum(MMIWIFI_LIST_TRUSTED_CTRL_ID))
            {
                MMK_CreateWin((uint32*)MMIWIFI_AP_TRUST_LIST_POP_WIN_TAB, PNULL);
            }
            break;
    case MSG_NOTIFY_TITLE_MENU_ICON:
        if(MMIWIFI_MAX_TRUSTED_LIST_NUM > MMIWIFI_GetTrustedProfileNum())//MMIWIFI_MAX_TRUSTED_LIST_NUM
        {
            MMIWIFI_OpenNewWin();
        }
        else
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_WIFI_TRUSTED_LIST_FULL,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL); 
        }
        break;
        
#endif /* MMI_GUI_STYLE_TYPICAL */

    case MSG_APP_WEB:
        {
#ifdef    WLAN_PORTAL_SUPPORT
            uint32 index = 0;
            MMIWIFI_PROFILE_T *profile_ptr = PNULL;

            index = GUILIST_GetCurItemIndex(MMIWIFI_LIST_TRUSTED_CTRL_ID);
            profile_ptr = (MMIWIFI_PROFILE_T *)WifiGetListUserdata(MMIWIFI_LIST_TRUSTED_CTRL_ID, index);

            if(MMIWIFI_APIsCMCC(profile_ptr))
            {
                MMIAPIWIFI_OpenPortalSettingsWin();
            }
#endif            
        }

        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseParentWin(MMIWIFI_MAIN_PARENT_WIN_ID);
        break;

    case MSG_CLOSE_WINDOW:
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/****************************************************************************/
//  Description : Set Main Property
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  MMIWIFI_SetMainTabProperty(MMI_CTRL_ID_T ctrl_id)
{
    MMI_STRING_T str_text = {0};
    uint32 index = 0;
    uint16 screen_width = 0;
    uint16 screen_height = 0;
    MMI_IMAGE_ID_T wlan_pda_tab[][2] = 
        {{IMAGE_WLANLIST_TAB_OFF_ICON, IMAGE_WLANLIST_TAB_ON_ICON},
        {IMAGE_TRUSTLIST_TAB_OFF_ICON, IMAGE_TRUSTLIST_TAB_ON_ICON},
        {IMAGE_WLAN_SETTINGS_TAB_OFF_ICON, IMAGE_WLAN_SETTINGS_TAB_ON_ICON}};

#ifdef MMI_PDA_SUPPORT
    MMI_IMAGE_ID_T istyle_wlan_pda_tab[][2] = 
        {{ISTYLE_IMAGE_WLANLIST_TAB_ON_ICON, ISTYLE_IMAGE_WLANLIST_TAB_OFF_ICON},
        {ISTYLE_IMAGE_TRUSTLIST_TAB_ON_ICON, ISTYLE_IMAGE_TRUSTLIST_TAB_OFF_ICON},
        {ISTYLE_IMAGE_WLAN_SETTINGS_TAB_ON_ICON, ISTYLE_IMAGE_WLAN_SETTINGS_TAB_OFF_ICON}};
#endif

    MMI_IMAGE_ID_T (*pda_tab_ptr)[2] = {PNULL};

#if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)
    if (MMITHEME_IsIstyle())
    {
        pda_tab_ptr = istyle_wlan_pda_tab;
    }
    else
#endif
    {
        pda_tab_ptr = wlan_pda_tab;
    }

    GUITAB_SetMaxItemNum(ctrl_id, MMIWIFI_ENTRY_TYPE_MAX);

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &screen_width, &screen_height);
//    GUITAB_SetItemWidth(ctrl_id, screen_width / MMIWIFI_ENTRY_TYPE_MAX);

    SCI_MEMSET(&str_text, 0, sizeof(str_text));
    MMI_GetLabelTextByLang(TXT_COMM_LIST, &str_text);
    GUITAB_AppendSimpleItem(ctrl_id, &str_text, pda_tab_ptr[index][0], pda_tab_ptr[index][1]);
    GUITAB_AddChildWindow(ctrl_id, MMIWIFI_LIST_WIN_ID, index++);

    MMI_GetLabelTextByLang(TXT_WIFI_TRUSTED_LIST , &str_text);
    GUITAB_AppendSimpleItem(ctrl_id, &str_text, pda_tab_ptr[index][0], pda_tab_ptr[index][1]);
    GUITAB_AddChildWindow(ctrl_id, MMIWIFI_TRUSTED_LIST_CHILD_WIN_ID, index++);
    
    MMI_GetLabelTextByLang(STXT_MAIN_SETTING , &str_text);
    GUITAB_AppendSimpleItem(ctrl_id, &str_text, pda_tab_ptr[index][0], pda_tab_ptr[index][1]);
#ifndef MMI_GUI_STYLE_TYPICAL
    GUITAB_AddChildWindow(ctrl_id, MMIWIFI_SETTING_LIST_CHILD_WIN_ID, index++);
#else
    GUITAB_AddChildWindow(ctrl_id, MMIWLAN_SETTING_WIN_ID, index++);
#endif /* MMI_GUI_STYLE_TYPICAL */

    if (MMK_IsFocusWin(MMIWIFI_LIST_WIN_ID)) {
        index = MMIWIFI_ENTRY_TYPE_LIST;
    }

    if (MMK_IsFocusWin(MMIWIFI_TRUSTED_LIST_CHILD_WIN_ID)) {
        index = MMIWIFI_ENTRY_TYPE_TRUSTED_LIST;
    }

#ifndef MMI_GUI_STYLE_TYPICAL
    if (MMK_IsFocusWin(MMIWIFI_SETTING_LIST_CHILD_WIN_ID))
#else
    if (MMK_IsFocusWin(MMIWLAN_SETTING_WIN_ID))
#endif /* MMI_GUI_STYLE_TYPICAL */
    {
        index = MMIWIFI_ENTRY_TYPE_SETTINGS;
    }

    GUITAB_SetCurSel(MMIWIFI_TAB_MAIN_CTRL_ID, index);
}

/****************************************************************************/
//  Description : WifiUpdateTrustedList
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  WifiUpdateTrustedList(MMI_CTRL_ID_T ctrl_id)
{
    uint32  num = 0;
    uint16  current_item_index = 0;

    //记录当前选中项的索引
    current_item_index = GUILIST_GetCurItemIndex(ctrl_id);

    GUILIST_RemoveAllItems(ctrl_id);

    num = MMIWIFI_GetTrustedProfileNum();

    if (0 == num)
    {
        GUIWIN_SetSoftkeyTextId(MMIWIFI_TRUSTED_LIST_CHILD_WIN_ID,  STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
        //由于list中没有内容，所以此处不需要恢复list的选中项
        return;
    }
    else
    {
        GUIWIN_SetSoftkeyTextId(MMIWIFI_TRUSTED_LIST_CHILD_WIN_ID,  STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
    }

    //将数据库中的AP list同步到list界面上来
    MMIWIFI_TraverseProfile(WifiUpdateTrustedListTravCallback, (void *)ctrl_id);

    //根据记录的选中项的索引，恢复list的选中项
    GUILIST_SetCurItemIndex(ctrl_id, current_item_index);
    return;
}

/****************************************************************************/
//  Description : WifiUpdateListTravCallback
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL BOOLEAN WifiUpdateTrustedListTravCallback(MMIWIFI_PROFILE_T *profile_ptr, void *find_data)
{
    if(PNULL != profile_ptr) {
        if(MMIWIFI_IsTrustedProfile(profile_ptr)) {
                WifiAppendTrustedListItem((MMI_CTRL_ID_T)find_data, profile_ptr);
        }
    }  else {
        //SCI_TRACE_LOW:"mmipdp error %s: profile_ptr is PNULL!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_16687_112_2_18_3_10_27_289,(uint8*)"s", __func__);
    }

    /*必须返回FALSE才能继续遍历*/
    return FALSE;
}

/****************************************************************************/
//  Description : WifiAppendListItem
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void WifiAppendTrustedListItem(MMI_CTRL_ID_T ctrl_id, MMIWIFI_PROFILE_T *profile_ptr)
{
    GUILIST_ITEM_T       item_t = {0};
    GUILIST_ITEM_DATA_T  item_data = {0};
    uint16               pw_ssid[WIFISUPP_SSID_MAX_LEN * 2 + 4] = {0};
    uint32               ulSsidLen = 0;

    if (PNULL == profile_ptr)
    {
        return;
    }

    item_data.softkey_id[0] = STXT_OPTION;
#ifdef WLAN_PORTAL_SUPPORT
    if (MMIWIFI_APIsCMCC(profile_ptr))
    {
        item_data.softkey_id[1] = STXT_MAIN_SETTING;
    }
    else
#endif        
    {
        item_data.softkey_id[1] = TXT_NULL;
    }

    item_data.softkey_id[2] = STXT_RETURN;

    ulSsidLen = GUI_GBToWstr(pw_ssid, profile_ptr->config_info.ssid.ssid, MIN(profile_ptr->config_info.ssid.ssid_len, WIFISUPP_SSID_MAX_LEN));
    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer.wstr_len = (uint16)ulSsidLen;
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = (wchar *)pw_ssid;

    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT;
    item_t.item_data_ptr = &item_data;
    item_t.user_data = (uint32)profile_ptr;

    if (!GUILIST_AppendItem(ctrl_id, &item_t))
    {
        SCI_TRACE_LOW("WifiAppendTrustedListItem  failed");
    }
}

/****************************************************************************/
//  Description : WifiHandleTrustedlistPopMenuWinMsg
//  Global resource dependence : 
//  Author:
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  WifiHandleTrustedlistPopMenuWinMsg(
                                                MMI_WIN_ID_T    win_id, 
                                                MMI_MESSAGE_ID_E   msg_id, 
                                                DPARAM             param
                                                )
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;

    switch(msg_id) 
    {
        case MSG_OPEN_WINDOW:
            WifiCreateTrustedListPopMenu(win_id, MMIWIFI_TRUSTED_LIST_POP_MENU_CTRL_ID);
            MMK_SetAtvCtrl(win_id, MMIWIFI_TRUSTED_LIST_POP_MENU_CTRL_ID);
            break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
        case MSG_CTL_OK:
        case MSG_KEYDOWN_WEB:
            {
                uint16                  index = 0;
                MMIWIFI_PROFILE_T*      profile_ptr = PNULL;
                
                index = GUILIST_GetCurItemIndex(MMIWIFI_LIST_TRUSTED_CTRL_ID);
                profile_ptr = (MMIWIFI_PROFILE_T *)WifiGetListUserdata(MMIWIFI_LIST_TRUSTED_CTRL_ID, index);

                switch ( GUIMENU_GetCurNodeId(MMIWIFI_TRUSTED_LIST_POP_MENU_CTRL_ID))
                {
                case MMIWLAN_TRUST_OPT_ID_EDIT://编辑，信任列表中
                    if (PNULL != profile_ptr)
                    {
                        if (profile_ptr->state == MMIWIFI_PROFILE_STATE_DISCONNECTED || profile_ptr->state == MMIWIFI_PROFILE_STATE_DISCONNECTING)
                        {
                            MMIWIFI_OpenEditWin(profile_ptr);
                            MMK_CloseWin(win_id);
                        }
                        else
                        {
                            MMIPUB_OpenQueryWinByTextId(TXT_WLAN_IS_DELETE_CONNECTED,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
                        }
                    }
                    break;

                case MMIWLAN_TRUST_OPT_ID_ADD://新建，信任列表中
                    {
                       if(MMIWIFI_MAX_TRUSTED_LIST_NUM > MMIWIFI_GetTrustedProfileNum())//MMIWIFI_MAX_TRUSTED_LIST_NUM
                        {
                            MMIWIFI_OpenNewWin();
                        }
                        else
                        {
                            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_WIFI_TRUSTED_LIST_FULL,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL); 
                        }
                    }
                    MMK_CloseWin(win_id);
                    break;

                case MMIWLAN_TRUST_OPT_ID_DELETE://删除，信任列表中
                    //SCI_TRACE_LOW:"MMIWIFI WifiHandlelistPopMenuWinMsg MMIWIFI_POPMENU_ID_DELETE index=%d,profile_ptr=%d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_16798_112_2_18_3_10_27_290,(uint8*)"dd",index, profile_ptr);
                    if (PNULL != profile_ptr && (profile_ptr->state == MMIWIFI_PROFILE_STATE_DISCONNECTED || profile_ptr->state == MMIWIFI_PROFILE_STATE_DISCONNECTING))
                    {
                        MMIPUB_OpenQueryWinByTextId(TXT_WIFI_IS_DELETE,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
                    }
                    else
                    {
                        MMIPUB_OpenQueryWinByTextId(TXT_WLAN_IS_DELETE_CONNECTED,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
                    }
                    break;

                case MMIWLAN_TRUST_OPT_ID_CHANGE_PRIORITY_HIGH://优先级上移，信任列表中
                    {
                        MMIWIFI_PROFILE_T *profile_ptr1 = PNULL;
                        profile_ptr1 = (MMIWIFI_PROFILE_T *)WifiGetListUserdata(MMIWIFI_LIST_TRUSTED_CTRL_ID, index -1);

                        if(MMIWIFI_SwapProfile(profile_ptr, profile_ptr1))
                        {
                            MMIWIFI_UpdateProfileFile();
                        }
                        GUILIST_SetCurItemIndex(MMIWIFI_LIST_TRUSTED_CTRL_ID, index - 1);
                        MMK_CloseWin(win_id);
                    }
                    break;

                case MMIWLAN_TRUST_OPT_ID_CHANGE_PRIORITY_LOW://优先级下移，信任列表中
                    {
                        MMIWIFI_PROFILE_T *profile_ptr1 = PNULL;
                        profile_ptr1 = (MMIWIFI_PROFILE_T *)WifiGetListUserdata(MMIWIFI_LIST_TRUSTED_CTRL_ID, index + 1);

                        if(MMIWIFI_SwapProfile(profile_ptr, profile_ptr1))
                        {
                            MMIWIFI_UpdateProfileFile();
                        }
                        GUILIST_SetCurItemIndex(MMIWIFI_LIST_TRUSTED_CTRL_ID, index + 1);
                        MMK_CloseWin(win_id);
                    }
                    break;
                
                default:
                    break;
                }
            }
            break;
            
        case MSG_PROMPTWIN_OK:
            {
                uint16 index = 0;
                MMIWIFI_PROFILE_T*      profile_ptr = PNULL;
                
                MMIPUB_CloseQuerytWin(PNULL);
                index = GUILIST_GetCurItemIndex(MMIWIFI_LIST_TRUSTED_CTRL_ID);
                profile_ptr = (MMIWIFI_PROFILE_T *)WifiGetListUserdata(MMIWIFI_LIST_TRUSTED_CTRL_ID, index);

                switch ( GUIMENU_GetCurNodeId(MMIWIFI_TRUSTED_LIST_POP_MENU_CTRL_ID))
                {
                case MMIWLAN_TRUST_OPT_ID_EDIT:
                    if (PNULL != profile_ptr && (profile_ptr->state != MMIWIFI_PROFILE_STATE_DISCONNECTED && profile_ptr->state != MMIWIFI_PROFILE_STATE_DISCONNECTING))
                    {
                        MMIWIFI_DisConnect(profile_ptr);
                    }
                    MMIWIFI_OpenEditWin(profile_ptr);
                    break;
                    
                case MMIWLAN_TRUST_OPT_ID_DELETE:
                    if (PNULL != profile_ptr && (profile_ptr->state != MMIWIFI_PROFILE_STATE_DISCONNECTED && profile_ptr->state != MMIWIFI_PROFILE_STATE_DISCONNECTING))
                    {
                        MMIWIFI_DisConnect(profile_ptr);
                    }
                    if(PNULL != profile_ptr && profile_ptr ->is_on_air)//如所删AP是当前搜索结果，不删除AP，将其is_trusted标志置否
                    {
                        profile_ptr ->is_trusted = FALSE;
                        MMIWIFI_UpdateProfileFile();
#ifdef MMI_GUI_STYLE_TYPICAL
                        MMIPUB_OpenAlertWinByTextId(PNULL, TXT_WIFI_DELETE_SUCC, 0,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
#endif
                    }
                    else
                    {
                        if (MMIWIFI_DeleteProfileByAp(profile_ptr))
                        {
                            MMIWIFI_UpdateProfileFile();
#ifdef MMI_GUI_STYLE_TYPICAL
                            MMIPUB_OpenAlertWinByTextId(PNULL, TXT_WIFI_DELETE_SUCC, 0,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
#endif
                        }
                        else
                        {
                            MMIPUB_OpenAlertWinByTextId(PNULL, TXT_COMMON_DEL_FAIL, 0,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                        }
                    }
                    break;
                    
                default:
                    break;
                }
                MMK_CloseWin(win_id);
            }
            break;
    
        case MSG_PROMPTWIN_CANCEL:
            MMIPUB_CloseQuerytWin(PNULL);
            MMK_CloseWin(win_id);
            break;
            
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
//  Discription: WifiCreateTrustedListPopMenu
//  Global resource dependence: None
//  Author: 
//  Note : 
//  Return: 
/*****************************************************************************/
LOCAL void WifiCreateTrustedListPopMenu(
                            MMI_WIN_ID_T    win_id,
                            MMI_CTRL_ID_T   ctrl_id
                            )
{
    MMI_TEXT_ID_T    text_id;
    MMI_STRING_T    kstring = {0};
    uint32          node_id = 0;
    uint16          i = 0;
    uint16          index = 0;
    uint16          total_num = 0;
    GUIMENU_DYNA_ITEM_T     node_item = {0};
#ifndef MMI_GUI_STYLE_TYPICAL
    uint32          wifi_menu[][3] ={
                                                        {MMIWLAN_TRUST_OPT_ID_ADD, TXT_WIFI_ADD_APN, IMAGE_COMMON_OPTMENU_ADD}

                                                    };
#else
    uint32          wifi_menu[][2] ={
                                                        {MMIWLAN_TRUST_OPT_ID_EDIT, TXT_EDIT},
                                                        {MMIWLAN_TRUST_OPT_ID_ADD, TXT_WIFI_ADD_APN},
                                                        {MMIWLAN_TRUST_OPT_ID_CHANGE_PRIORITY_HIGH, TXT_WIFI_CHANGE_PRIORITY_HIGH},
                                                        {MMIWLAN_TRUST_OPT_ID_CHANGE_PRIORITY_LOW, TXT_WIFI_CHANGE_PRIORITY_LOW},
                                                        {MMIWLAN_TRUST_OPT_ID_DELETE, TXT_DELETE}
                                                    };
#endif /* MMI_PDA_SUPPORT */


    if(!MMK_IsOpenWin(win_id))
    {
        //SCI_TRACE_LOW:"WifiCreateListMenu window already opened !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_WIN_16945_112_2_18_3_10_27_291,(uint8*)"");
        MMK_WinGrabFocus(win_id);
        return;
    }

    GUIMENU_CreatDynamicEx(MMIWLAN_TRUST_OPT_ID_MAX,NULL, win_id, ctrl_id, GUIMENU_STYLE_POPUP);
    GUIMENU_SetDynamicMenuSoftkey(ctrl_id, TXT_COMMON_OK, TXT_NULL, STXT_RETURN);

    total_num = GUILIST_GetTotalItemNum(MMIWIFI_LIST_TRUSTED_CTRL_ID);
    index = GUILIST_GetCurItemIndex(MMIWIFI_LIST_TRUSTED_CTRL_ID);

    for(i = 0; i < sizeof(wifi_menu) / sizeof(wifi_menu[0]); i++)
    {
        SCI_MEMSET(&node_item, 0, sizeof(node_item));
        text_id = wifi_menu[i][1];
        node_id = wifi_menu[i][0];
        MMI_GetLabelTextByLang(text_id, &kstring);
        node_item.item_text_ptr = &kstring;
#ifndef MMI_GUI_STYLE_TYPICAL
        node_item.select_icon_id = wifi_menu[i][2];
#endif /* MMI_GUI_STYLE_TYPICAL */
        GUIMENU_InsertNode(i, node_id, 0, &node_item, ctrl_id);
    }
#ifdef MMI_GUI_STYLE_TYPICAL
    if(0 == total_num)
    {
        GUIMENU_SetNodeGrayed(TRUE, MMIWLAN_TRUST_OPT_ID_EDIT -1, NULL, ctrl_id);/*lint !e778*/
        GUIMENU_SetNodeGrayed(TRUE, MMIWLAN_TRUST_OPT_ID_CHANGE_PRIORITY_HIGH-1, NULL, ctrl_id);
        GUIMENU_SetNodeGrayed(TRUE, MMIWLAN_TRUST_OPT_ID_CHANGE_PRIORITY_LOW-1, NULL, ctrl_id);
        GUIMENU_SetNodeGrayed(TRUE, MMIWLAN_TRUST_OPT_ID_DELETE-1, NULL, ctrl_id);
    }
    else
    {
        GUIMENU_SetNodeGrayed(FALSE, MMIWLAN_TRUST_OPT_ID_EDIT-1, NULL, ctrl_id);/*lint !e778*/
        GUIMENU_SetNodeGrayed(FALSE, MMIWLAN_TRUST_OPT_ID_CHANGE_PRIORITY_HIGH-1, NULL, ctrl_id);
        GUIMENU_SetNodeGrayed(FALSE, MMIWLAN_TRUST_OPT_ID_CHANGE_PRIORITY_LOW-1, NULL, ctrl_id);
        GUIMENU_SetNodeGrayed(FALSE, MMIWLAN_TRUST_OPT_ID_DELETE-1, NULL, ctrl_id);

#ifdef WLAN_PORTAL_SUPPORT
        if(MMIWIFI_APIsCMCC((MMIWIFI_PROFILE_T *)WifiGetListUserdata(MMIWIFI_LIST_TRUSTED_CTRL_ID,index)))
        {
            GUIMENU_SetNodeGrayed(TRUE, MMIWLAN_TRUST_OPT_ID_DELETE-1, NULL, ctrl_id);
            GUIMENU_SetNodeGrayed(TRUE, MMIWLAN_TRUST_OPT_ID_EDIT -1, NULL, ctrl_id);/*lint !e778*/
        }
        if(MMIWIFI_APIsCMCCEDU((MMIWIFI_PROFILE_T *)WifiGetListUserdata(MMIWIFI_LIST_TRUSTED_CTRL_ID,index)))
        {
            GUIMENU_SetNodeGrayed(TRUE, MMIWLAN_TRUST_OPT_ID_DELETE-1, NULL, ctrl_id);
            GUIMENU_SetNodeGrayed(TRUE, MMIWLAN_TRUST_OPT_ID_EDIT -1, NULL, ctrl_id);/*lint !e778*/
        }
#endif

        if (0 == index)
        {
            GUIMENU_SetNodeGrayed(TRUE, MMIWLAN_TRUST_OPT_ID_CHANGE_PRIORITY_HIGH-1, NULL, ctrl_id);
        }

        if ((total_num - 1) == index)
        {
            GUIMENU_SetNodeGrayed(TRUE, MMIWLAN_TRUST_OPT_ID_CHANGE_PRIORITY_LOW-1, NULL, ctrl_id);
        }
    }
#endif /* MMI_GUI_STYLE_TYPICAL */
}

#endif /*MMI_WLAN_TAB_STYLE*/

/****************************************************************************/
//  Description : get the wifi profile num
//  Global resource dependence :
//  Author:bin.wang1
//  Note:
/****************************************************************************/
PUBLIC uint32 MMIAPIWIFI_GetSearchedAPNum( void )
{
    return MMIWIFI_GetProfileNum();
}

/****************************************************************************/
//  Description : get the profile ssid by index
//  Global resource dependence :
//  IN/OUT: ssid_t_ptr searched ssid information
//  Author:bin.wang1
//  Note: length is 0 mean hide ssid
/****************************************************************************/
PUBLIC BOOLEAN MMIAPIWIFI_GetSearchedSsidByIndex( MMIWIFI_INDEX_T index, WIFISUPP_SSID_T* ssid_t_ptr )
{
    int32               len = 0;
    uint32              profileNum = MMIWIFI_GetProfileNum();
    MMIWIFI_PROFILE_T*  pProfile = PNULL;

    if( index >= profileNum )
    {
        TRACE_APP_WLAN(" index error %d profileNum %d", (int32)index, profileNum);
        return FALSE;
    }

    if( PNULL == ssid_t_ptr )
    {
        TRACE_APP_WLAN("ssid_t_ptr is PNULL!!");
        return FALSE;
    }

    pProfile = MMIWIFI_GetProfileByIndex( index );
    if( PNULL == pProfile )
    {
        TRACE_APP_WLAN("pProfile is PNULL!!");
        return FALSE;
    }

    len = MIN( pProfile->config_info.ssid.ssid_len, WIFISUPP_SSID_MAX_LEN-1 );
    SCI_MEMCPY( ssid_t_ptr->ssid, pProfile->config_info.ssid.ssid, len );
    ssid_t_ptr->ssid_len = len;

    return TRUE;
}


/*Edit by script, ignore 52 case. Fri Apr 27 09:38:56 2012*/ //IGNORE9527
