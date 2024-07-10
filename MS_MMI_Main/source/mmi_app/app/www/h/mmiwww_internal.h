/***************************************************************************
** File Name:      mmiwww_internal.h                                            *
** Author:                                                                 *
** Date:           15/06/2009                                              *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:    This file is used to www browser internal define                   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 06/2009        wancan.you        Create
****************************************************************************/
#ifndef  _MMIWWW_INTERNAL_H_    
#define  _MMIWWW_INTERNAL_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mn_type.h"
#include "guilcd.h"
#include "cafapplet_gen.h"
#include "nfbrowser_slimtypes.h"
#include "nfbrowser_interface.h"
#include "mmibrowser_export.h"
#include "mmiwww_slim_peer.h"
#include "mmiwww_netfront_cmd.h"
#include "mmiwww_nv.h"
#include "mmiwww_text.h"
#include "mmiwww_menutab.h"
#include "mmiconnection_export.h"
#include "mmk_kbd.h"
#include "mmifilearray_export.h"
#include "mmipdp_export.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMIWWW_INVALID_GATEWAY              "0.0.0.0"
#define MMIWWW_MAX_SETTING_NUM   (MMICONNECTION_MAX_SETTING_NUM)
#define MMIWWW_MAX_PAGEMEMO_NUM 50
#define MMIWWW_SOFTKEY_NUM 3
#define MMIWWW_MAX_TIMER_NUM       8
#define MMIWWW_MAX_FILEPATH         256
#define MMIWWW_MAX_KEYNAME_LEN 49
#define MMIWWW_KEYNAME_NUM 10
#define MMIWWW_INPUTHISTORY_URL_NUM_MAX     (30)
#define MMIWWW_INPUTHISTORY_URL_LEN_MAX     (MAX_WEB_URL_LEN/4)
#define MMIWWW_SETTINGS_SCRIPT_TYPE_NUM     3
#define MMIWWW_SETTINGS_ANIMAL_TYPE_NUM    2
#define MMIWWW_SETTINGS_IMAGE_TYPE_NUM   2
#define MMIWWW_SETTINGS_TABLE_TYPE_NUM   2
#define MMIWWW_SETTINGS_SOUND_TYPE_NUM  2
#define MMIWWW_SETTINGS_POPUP_TYPE_NUM  2
#define MMIWWW_SETTINGS_CACHE_MODE_NUM  4
#define MMIWWW_SETTINGS_QUERY_DISK_NUM   2

#ifdef MMIWWW_SETTINGS_COOKIE_USE_QUERY
#define MMIWWW_SETTINGS_COOKIE_MODE_NUM  3
#else
#define MMIWWW_SETTINGS_COOKIE_MODE_NUM  2
#endif

#define MMIWWW_SETTINGS_WEBDATA_MODE_NUM 2

#define MMIWWW_FILE_FREE_SIZE                      (10 * 1024)//剩余大小

#define MMIWWW_DNS_TICKET_MAX   2

#define MMIWWW_MAX_DEFAULT_BOOKMARK_NAME_LEN    (15)
#define MMIWWW_PAGE_ZOOM_PERCENT_START  (50)
#define MMIWWW_PAGE_ZOOM_PERCENT_STEP   (25)

// 网页相关的宏定义
#define WWW_MAX_SEARCH_STRING_LEN    20  // 网页查找字符串最大长度
#define WWW_MAX_JSPROMPT_STRING_LEN  20  // JSPROMPT中允许输入的字符串最大长度

#define MMIWWW_MAX_BOOKMARK_NUM       (50)
#define MMIWWW_MAX_BOOKMARK_DIR_NUM       (5)

#define MMIWWW_DEVICE_SYS_LOW_SPACE       (1024)

#define MMIWWW_HTTP_HEADER                  "http://"
#define MMIWWW_HTTPS_HEADER                 "https://"
#define MMIWWW_RTSP_HEADER                  "rtsp://"
#define MMIWWW_MMS_HEADER                   "mms://"
#define MMIWWW_FTP_HEADER                   "ftp://"
#define MMIWWW_TELENT_HEADER                "telnet://"
#define MMIWWW_FILE_HEADER                  "file:///"
#define MMIWWW_ABOUT_BLANK                  "about:blank"

#define MMIWWW_CAFEAPPLET(www_applet) ((www_applet)->caf_applet)
#define MMIWWW_NETFRONT(www_applet) (&(www_applet)->www_instance)
#define MMIWWW_HANDLE(www_applet) ((MMIWWW_CAFEAPPLET(www_applet)).app_handle)

#define MMIWWW_APPLETINSTANCE MMIWWW_GetAppletInstance
#define MMIWWW_INSTANCE MMIWWW_GetWWWInstance
#define MMIWWW_GETLCDDEVINFO MMIWWW_GetLcdDevInfo
#define MMIWWW_NETCONNECT() MMIWWW_NetConnect(MMIWWW_INSTANCE())
#define MMIWWW_NETDISCONNECT() MMIWWW_NetDisconnect(MMIWWW_INSTANCE())

#define MMIWWW_GETBROWSER MMIWWW_GetBrowser()

#define MMIWWW_BOOKMARK MMIWWW_GetWWWInstanceBM

#define MMIWWW_PDP_APPLET_CONNECT   //PDP在创建applet的时候就开始激活

#define USE_EXT_DISPLAY_BUFFER
#ifdef USE_EXT_DISPLAY_BUFFER
#define MMIWWW_DISPLAY_BUFFER_SIZE (MMI_MAINSCREEN_WIDTH * MMI_MAINSCREEN_HEIGHT * sizeof(GUI_COLOR_T))
#else
#define MMIWWW_DISPLAY_BUFFER_SIZE (0)
#endif /* USE_EXT_DISPLAY_BUFFER */
#if defined (MAINLCD_SIZE_176X220) || defined (MAINLCD_SIZE_128X160)
#define MMIWWW_MAINMENU_TAB_TEXT_FONT (14)
#else
#define MMIWWW_MAINMENU_TAB_TEXT_FONT (20)
#endif


// 释放MMI_STRING_T数据
#define MMIWWW_FREE_STRING(_STR_) \
        do\
        {\
            if (PNULL != _STR_.wstr_ptr) \
            {\
                SCI_FREE(_STR_.wstr_ptr);\
                _STR_.wstr_len = 0;\
            }\
        }while(0)

#define MMIWWW_MOUSE_ANTI_SHAKE_MOVE_X 5
#define MMIWWW_MOUSE_ANTI_SHAKE_MOVE_Y 5
#define MAX_WEB_TITLE_LEN (64)
#define MMIWWW_CMCC_URL     "http://wap.monternet.com"

#define MMIWWW_PAGE_ZOOM_PERCENT_START  (50)
#define MMIWWW_QUERY_GPRS_NOTIFICATION_MAX (100)

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef enum
{
    MMIWWW_BROWSER_TAB_1,
    MMIWWW_BROWSER_TAB_2,
    MMIWWW_BROWSER_TAB_3,
    MMIWWW_BROWSER_TAB_4,
    MMIWWW_BROWSER_TAB_5,

    MMIWWW_BROWSER_MAX_TAB
}MMIWWW_BROWSER_TAB_E;

typedef struct _MMIWWW_PEER_DATA_T_ {
    char fFileNameBuffer[MMIWWW_MAX_FILEPATH];
    slim_int  fFileNameLength;

    slim_int  fX;
    slim_int  fY;
    slim_int  fScreenWidth;
    slim_int  fScreenHeight;
    slim_int  fSoftkeyHeight;
    slim_int  fPeerPixelType;
    slim_int  fPeerPixelTypeA;
    slim_int  fArchCharSet;            /* AEE_ convert SLIM_CHARSET */
    slim_int  fColorDepth;
    slim_int  fLockScreenCount;
    slim_int  fDevEncCharSet;
    BOOLEAN   fNoUpdateDisplay;
    slim_int  fFontSetting;
    slim_int  fPixelMapType;
    void     *lcd_ptr;
    slim_int fZoom_percent[MMIWWW_BROWSER_MAX_TAB];
}MMIWWW_PEER_DATA_T;

typedef enum
{
    MMIWWW_ZOOMIN,
    MMIWWW_ZOOMOUT,
    MMIWWW_ZOOMORG,
    MMIWWW_ZOOMINIT,
    MMIWWW_ZOOMMAX
}MMIWWW_ZOOM_MODE_E;

typedef enum
{
    MMIWWW_SEARCH_ENGINE_CMCC,
    MMIWWW_SEARCH_ENGINE_BAIDU,
    MMIWWW_SEARCH_ENGINE_GOOGLE,
    MMIWWW_SEARCH_ENGINE_YAHOO,

    MMIWWW_SEARCH_ENGINES
}MMIWWW_SEARCH_ENGINE_MODE_E;

#if 0//@fen.xie
typedef enum
{
    MMIWWW_CC_OPTION_VOICE = 0x0001,
    MMIWWW_CC_OPTION_IP = 0x0001 << 1,
    MMIWWW_CC_OPTION_VIDEO = 0x0001 << 2,

    MMIWWW_CC_OPTION_MAX
}MMIWWW_CC_OPTION_E;
#endif

typedef enum
{
    MMIWWW_POINTER_MODE_NORMAL,
    MMIWWW_POINTER_MODE_VIRTUAL_POINTER,

    MMIWWW_POINTER_MODES
}MMIWWW_POINTER_MODE_E;

typedef struct _MMIWWW_PAGEINFO_T_
{
    uint8 timer_id;
    MMIWWW_ZOOM_MODE_E zoom_mode;
} MMIWWW_PAGEINFO_T;

#if 0//@fen.xie
typedef struct _MMIWWW_CC_INFO_T_
{
    uint8 * phone_num_ptr;
    uint32 cc_option;
} MMIWWW_CC_INFO_T;
#endif

typedef struct _MMIWWW_DEVICE_INFO_T_
{
    slim_int screen_width;
    slim_int screen_height;
    slim_int color_depth;
    slim_int dw_lang;
}MMIWWW_DEVICE_INFO_T;
 
typedef enum
{
    MMIWWW_PDP_NONE,
    MMIWWW_PDP_ACTIVING,
    MMIWWW_PDP_ACTIVE_OK,        
    MMIWWW_PDP_ACTIVE_FAILED,
    MMIWWW_PDP_DEACTIVING,
    MMIWWW_PDP_DEACTIVE_FAILED,
    MMIWWW_PDP_DEACTIVE_OK = MMIWWW_PDP_NONE
}MMIWWW_PDP_STATE_E;

typedef enum
{
    MMIWWW_FONT_TYPE_BIG,
    MMIWWW_FONT_TYPE_NORMAL,
    MMIWWW_FONT_TYPE_SMALL,

    MMIWWW_FONT_TYPE_MAX
}MMIWWW_FONT_TYPE_E;

typedef enum
{
    MMIWWW_ZOOM_50 = 0,
    MMIWWW_ZOOM_75,
    MMIWWW_ZOOM_100,
    MMIWWW_ZOOM_125,
    MMIWWW_ZOOM_150,

    MMIWWW_ZOOM_MIN = MMIWWW_ZOOM_50,
    MMIWWW_ZOOM_ORG = MMIWWW_ZOOM_100,
    MMIWWW_ZOOM_MAX = MMIWWW_ZOOM_150
}MMIWWW_ZOOM_E;

typedef enum
{
    MMIWWW_COOKIE_MODE_ACCEPTALL,
    MMIWWW_COOKIE_MODE_REJECTALL,
    MMIWWW_COOKIE_MODE_REQUEST,

    MMIWWW_COOKIE_MODE_MAX
}MMIWWW_COOKIE_MODE_E;

typedef enum
{
    MMIWWW_CACHE_MODE_CLOSE,
    MMIWWW_CACHE_MODE_NETWORKPRIO,
    MMIWWW_CACHE_MODE_CACHEPRIO,
    MMIWWW_CACHE_MODE_ONLYCACHE,

    MMIWWW_CACHE_MODE_MAX
}MMIWWW_CACHE_MODE_E;

typedef enum
{
    MMIWWW_DISK_CACHE_MODE_OPEN,
    MMIWWW_DISK_CACHE_MODE_CLOSE,

    MMIWWW_DISK_CACHE_MODE_MAX
}MMIWWW_DISK_CACHE_MODE_E;

typedef enum
{
    MMIWWW_CERT_VIEW_MODE,
    MMIWWW_CERT_IMPORT_MODE,

    MMIWWW_CERT_MODE_MAX
}MMIWWW_CERT_MODE_E;

typedef enum
{
    MMIWWW_BUTTON_MODE_DEFAULT,
    MMIWWW_BUTTON_MODE_PRESSED,
    MMIWWW_BUTTON_MODE_DISABLED,
    MMIWWW_BUTTON_MODE_FOCUSED,

    MMIWWW_BUTTON_MODE_MAX
}MMIWWW_BUTTON_MODE_E;

typedef enum
{
    MMIWWW_CTRL_NULL,
    MMIWWW_CTRL_ADDRESS_LABEL,
    MMIWWW_CTRL_LINK_BUTTON,
    MMIWWW_CTRL_SEARCH_LABEL,
    MMIWWW_CTRL_SEARCH_BUTTON,
    MMIWWW_CTRL_TAB,
    MMIWWW_CTRL_BOOKMARK_LIST,
    MMIWWW_CTRL_HISTORY_LIST,

    MMIWWW_CTRL_MAX
}MMIWWW_CTRL_E;

typedef enum
{
    MMIWWW_BM_CHANGE_ADD_NULL,      /*Title and Url is null*/
    MMIWWW_BM_CHANGE_ADD_DIR,      /*folder*/
    MMIWWW_BM_CHANGE_ADD_CUR_PAGE,  /*Title and Url is from current web*/
    MMIWWW_BM_CHANGE_ADD_URL,       /*Url is from other app*/
    MMIWWW_BM_CHANGE_EDIT,          /*Title and Url is from bookmark*/
    MMIWWW_BM_CHANGE_EDIT_DIR,     /*Title  is from bookmark folder*/

    MMIWWW_BM_CHANGE_MAX
}MMIWWW_BM_CHANGE_TYPE_E;

typedef enum
{
    MMIWWW_PAGEMEMO_SORT_BY_DATE,
    MMIWWW_PAGEMEMO_SORT_BY_NAME,

    MMIWWW_PAGEMEMO_SORT_MAX
}MMIWWW_PAGEMEMO_SORT_TYPE_E;

typedef enum
{
    MMIWWW_PAGE_ATTRIBUTE_HISTORY,
    MMIWWW_PAGE_ATTRIBUTE_PAGEMEMO,

    MMIWWW_PAGE_ATTRIBUTE_MAX
}MMIWWW_PAGEMEMO_ATTRIBUTE_TYPE_E;

typedef enum
{
    MMIWWW_COPY_MODE_NONE,
    MMIWWW_COPY_MODE_CHOSE_START,
    MMIWWW_COPY_MODE_CHOSE_END,

    MMIWWW_COPY_MODE_MAX
}MMIWWW_COPY_MODE_E;

typedef struct _MMIWWW_PDP_INFO_T_
{
    MMICONNECTION_LINKSETTING_DETAIL_T connect_setting;
    MMIWWW_PDP_STATE_E pdp_state;
    uint32 net_id;
}MMIWWW_PDP_INFO_T;

typedef struct _MMIWWW_DNS_INFO_T_
{
    uint32 dns_request_id[MMIWWW_DNS_TICKET_MAX];
    void * ticket[MMIWWW_DNS_TICKET_MAX];
}MMIWWW_DNS_INFO_T;

typedef struct _MMIWWW_LINK_SETTING_T_
{
    uint8 sim1_index;
    uint8 sim2_index;
}MMIWWW_LINK_SETTING_T;

typedef enum _MMIWWW_STATE_T_
{
    MMIWWW_STATE_PAGE_LOADING,
    MMIWWW_STATE_PAGE_VIEW,

    MMIWWW_STATE_MAX
}MMIWWW_STATE_T;

typedef struct _MMIWWW_TIMER_T_
{
    uint8 timer_id;
    NFBrowserTimer_TimerProc in_proc;
    slim_opaque in_arg;
}MMIWWW_TIMER_T;


typedef struct _MMIWWW_IME_STARTPARAM_T_
{
    slim_char *ime_init_text;
    slim_int max_length;
}MMIWWW_IME_STARTPARAM_T;

typedef struct _MMIWWW_IME_ENDPARAM_T_
{
    slim_char *ime_result_text;
    slim_int ime_result_bytes;
}MMIWWW_IME_ENDPARAM_T;

typedef struct _MMIWWW_UPLOAD_PARAM_T_
{
    slim_int in_select;
    slim_char *upload_path;
}MMIWWW_UPLOAD_PARAM_T;

typedef struct _MMIWWW_JSPROMT_PARAM_T_
{
    slim_char *in_message;
    slim_char *in_value;
}MMIWWW_JSPROMT_PARAM_T;

typedef struct _MMIWWW_CUSTOMIZED_CONNECTION_T_
{
    MN_DUAL_SYS_E dual_sys;
    BOOLEAN is_connection_customized;
    uint8 connection_index;
    uint8 default_connection_index;
    MMIPDP_INTERFACE_E bearer_type;
}MMIWWW_CUSTOMIZED_CONNECTION_T;

typedef struct _MMIWWW_AUTH_PARAM_T_
{
    slim_char *in_url;
    slim_char *in_realm;
    slim_char *in_user;
    slim_char *in_pass;
}MMIWWW_AUTH_PARAM_T;

typedef struct WWW_BOOKMARK_T_
{
    uint32 total;//total
    uint32 cur_index_num;//current bookmark list 's total items num
    wchar full_path[MMIFILE_FULL_PATH_MAX_LEN + 1] ;//current fullpath
    uint16 full_path_len;//current fullpath len
    FILEARRAY file_array;
    uint32 file_array_step;
}WWW_BOOKMARK_T;
typedef enum WWW_BOOKMARK_STEP_TYPE_E_
{
    BOOKMARK_STEP_TYPE_ROOT,
    BOOKMARK_STEP_TYPE_NEXT,
    BOOKMARK_STEP_TYPE_PRE,
    BOOKMARK_STEP_TYPE_MAX
}WWW_BOOKMARK_STEP_TYPE_E;


typedef struct MMI_USER_INPUT_MESSAGE_T_ {
    MMI_MESSAGE_ID_E msg_type;
    MMI_MULTI_KEY_TP_MSG_PARAM_T tp_msg_param; /* Use only when TP msg*/
} MMI_USER_INPUT_MESSAGE_T;

typedef struct _MMIWWW_INSTANCE_T_
{
    MMIWWW_PEER_DATA_T peer_data;

    MMIWWW_IME_ENDPARAM_T ime_endparam;
    MMI_HANDLE_T ime_win_id;
    slim_int browser_instance;
    slim_int suspend_time;
    slim_int suspend_tickcount;    
    slim_bool is_suspend;

    slim_bool is_inited;
    slim_bool is_active;
    slim_bool is_inime;
    MMIWWW_TIMER_T timer_proc[MMIWWW_MAX_TIMER_NUM];

    MN_DUAL_SYS_E dual_sys;

    MMIWWW_STATE_T www_state[MMIWWW_BROWSER_MAX_TAB];
    BOOLEAN        is_full_display;
    BOOLEAN        is_exit;
    slim_bool is_bg_sound[MMIWWW_BROWSER_MAX_TAB];
    slim_bool is_reload[MMIWWW_BROWSER_MAX_TAB];

#ifdef USE_EXT_DISPLAY_BUFFER
    GUI_COLOR_T *display_buf_ptr;
    uint32 display_buf_len;
#endif

    MMIWWW_ENTRY_TYPE_E enter_type;
    MMIWWW_CUSTOMIZED_CONNECTION_T customized_connection;
    uint32 www_mem_count;
    BOOLEAN is_mem_alerted;
    NFBrowserImageMode image_display_mode;

    MMIWWW_PDP_INFO_T pdp_info;
    MMIWWW_DNS_INFO_T dns_info;

    MMI_STRING_T search_string;
    MMIWWW_JSPROMT_PARAM_T js_promt;
    MMIWWW_AUTH_PARAM_T auth_param;
    slim_int cur_lang_set;
    uint8 *added_url;
    uint8 *added_title;
    BOOLEAN is_url_change;
    BOOLEAN is_keyword_change;
    BOOLEAN is_bookmark_change;
    BOOLEAN is_first_open;
    BOOLEAN is_mainmenu_focus;
    BOOLEAN is_nf_init;
    NFBrowserStartType first_type;
    uint8     *first_url;
    uint8     *str_callnum;
    MMIWWW_CTRL_E focus_ctrl;
    MMIWWW_UPLOAD_PARAM_T upload_param;
    BOOLEAN secure_page[MMIWWW_BROWSER_MAX_TAB];
    int32 charset_index[MMIWWW_BROWSER_MAX_TAB];
    BOOLEAN is_charset_change[MMIWWW_BROWSER_MAX_TAB];
    BOOLEAN is_focus;
    BOOLEAN is_stop_nf;
    BOOLEAN is_disable_timer;
    MMIWWW_COPY_MODE_E copy_mode;
    WWW_BOOKMARK_T bookmark_info;
    MMI_USER_INPUT_MESSAGE_T user_input_msg;
    BOOLEAN is_in_cur_win;
    GUI_LCD_DEV_INFO btn_layer_info;
}MMIWWW_INSTANCE_T;

typedef struct WWW_APPLET_T_
{
    CAF_APPLET_T caf_applet;
    MMIWWW_INSTANCE_T www_instance;
}WWW_APPLET_T;

typedef struct
{
    wchar wstr[MMIWWW_MAX_KEYNAME_LEN+1];
    uint16 wstr_len;
}MMIWWW_KEY_NAME_T;

typedef struct
{
    uint16 num;
    MMIWWW_KEY_NAME_T key_arr[MMIWWW_KEYNAME_NUM];
}MMIWWW_KEY_T;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
 /*****************************************************************************/
// 	Description: Create Memory Handle
//	Global resource dependence:
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_CreateMemory(void);

/*****************************************************************************/
// 	Description: Destroy Memory Handle
//	Global resource dependence:
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIWWW_DestroyMemory(void);

/*****************************************************************************/
//  Description : Check Is Network Ready
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_NetworkIsReady(MMIWWW_INSTANCE_T *self);

/*****************************************************************************/
//  Description : Get WWW Netfront Instance
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC MMIWWW_INSTANCE_T *MMIWWW_GetWWWInstance(void);

/*****************************************************************************/
//  Description : Get WWW Netfront Instance bookmark info
//  Global resource dependence : 
//  Author:fen.xie
//  Note:
/*****************************************************************************/
PUBLIC WWW_BOOKMARK_T *MMIWWW_GetWWWInstanceBM(void);

/*****************************************************************************/
//  Description : Get WWW Applet Instance
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC WWW_APPLET_T *MMIWWW_GetAppletInstance(void);

/*****************************************************************************/
//  Description : Get LCD Info
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_GetLcdDevInfo(GUI_LCD_ID_E id, GUI_LCD_DEV_INFO* lcd_dev_info);

/*****************************************************************************/
//  Description : Net Connect
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC int MMIWWW_NetConnect(MMIWWW_INSTANCE_T *self);

/*****************************************************************************/
//  Description : Net Disconnect
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_NetDisconnect(MMIWWW_INSTANCE_T *self);

/*****************************************************************************/
//  Description : set PDP state
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_SetPDPState(MMIWWW_INSTANCE_T *self, slim_int in_state);

/*****************************************************************************/
//  Description : Check Is Network Accessible
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_NetworkIsAccessible(MMIWWW_INSTANCE_T *self);

/*****************************************************************************/
//  Description : Create WWW main menu
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIWWW_OpenWWWMainMenuWin(MMIWWW_CTRL_E focus_ctrl);

/*****************************************************************************/
// Description : Chose Network
// Global resource dependence : none
// Author:
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_SelectNetWork(void *user_data);

/*****************************************************************************/
//  Description : Create Browser Win
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIWWW_OpenWWWBrowserWin(NFBrowserStartType in_type, char *in_url);

/*****************************************************************************/
//  Description : Add or Edit bookmark
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_ChangeBookmarkWin(MMIWWW_BM_CHANGE_TYPE_E change_type);

/*****************************************************************************/
//  Description : Create InlineIME
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIWWW_InlineIME_EnterTextControl(NFBrowserControlInfo *in_info);

/*****************************************************************************/
//  Description : Get InlineIME Value
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC slim_bool MMIWWW_InlineIME_GetTextControlValue(slim_char *out_string);

/*****************************************************************************/
//  Description : Get InlineIME Value Length
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC slim_int MMIWWW_InlineIME_GetTextControlValueLength(void);

/*****************************************************************************/
//  Description : Create WWW set menu
//  Global resource dependence : 
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIWWW_OpenWWWSetMenuWin(void);

/*****************************************************************************/
//  Description : Create WWW set win
//  Global resource dependence : 
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIWWW_OpenWWWSetWin(MMIWWW_ID_MENU_E menu_id);

/*****************************************************************************/
//  Description : register mmiwww module nv len and max item
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_RegNv(void);

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
                                                );

/*****************************************************************************/
// Description : Send message to browser page handle
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_SendMsgToBrowser(MMI_MESSAGE_ID_E  msg_id, uint32 data);

/*****************************************************************************/
// Description : Post message to browser page handle
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
 PUBLIC void MMIWWW_PostMsgToBrowser(MMI_MESSAGE_ID_E msg_id, DPARAM param_ptr, uint32 size_of_param);
 
/*****************************************************************************/
//  Description : Open JavaScript warning win
//  Global resource dependence : 
// Author: Bill.Ji
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_OpenJSAlertWin(slim_int  in_index, slim_char  *in_message);

/*****************************************************************************/
//  Description : Convert UTF8 string to WString
//  Global resource dependence : 
// Author: Bill.Ji
//  Note:需调用者对out_string进行释放
/*****************************************************************************/
PUBLIC void MMIWWW_ConvertToMMIString(MMI_STRING_T  *out_string,
                                      slim_char     *in_string);

/*****************************************************************************/
//  Description : Create JavaScript confirmation msg win
//  Global resource dependence : 
//  Author: Bill.Ji
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIWWW_OpenJSConfirmWin(slim_char  *in_message);

/*****************************************************************************/
//  Description : Create JavaScript prompt msg win
//  Global resource dependence : 
//  Author: Bill.Ji
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIWWW_OpenJSPromptWin(void);

/*****************************************************************************/
// Description : Reset Default Settings
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_ResetDefaultSettings(void);

/*****************************************************************************/
// Description : Send message to www applet
// Global resource dependence :
// Author: wancan.you
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_SendMsgToWWWApplet(MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
// Description : Post message to www applet
// Global resource dependence :
// Author: wancan.you
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_PostMsgToWWWApplet(MMI_MESSAGE_ID_E msg_id, DPARAM param, uint32 size_of_param);

/*****************************************************************************/
//  Description : Upload Enter Explorer
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_UploadEnterExplorer(DPARAM param);

/*****************************************************************************/
//  Description : Open WMLS warning win
//  Global resource dependence : 
// Author: Kun.yu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_OpenWMLSAlertWin(slim_char  *in_message);

/*****************************************************************************/
//  Description : Create JavaScript prompt msg win
//  Global resource dependence : 
//  Author: Kun.yu
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIWWW_OpenWMLSPromptWin(void);

/*****************************************************************************/
//  Description : Create WMLS confirmation msg win
//  Global resource dependence : 
//  Author: Kun.yu
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIWWW_OpenWMLSConfirmWin(slim_char  *in_message);

/*****************************************************************************/
//  Description : Handle DNS Cnf
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIWWW_HandleDNSCnf(MMIWWW_INSTANCE_T *www_instance, DPARAM param);

/*****************************************************************************/
//  Description : Clean Netftont Data Info
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 清空书签、历史记录、保存网页...
/*****************************************************************************/
PUBLIC void MMIWWW_CleanNetfrontData(void);

/*****************************************************************************/
//  Description : Open WWW Authorize
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIWWW_OpenAuthWin(void);



/*****************************************************************************/
//  Description : Create a new browser tab
//  Global resource dependence : 
//  Author: Bill.Ji
//  Note:
/*****************************************************************************/
PUBLIC slim_int MMIWWW_CreateBrowserTab(void);

/*****************************************************************************/
//  Description : Check need stop applet
//  Global resource dependence : 
//  Author: wancan.you
//  Note: depend on windows (Mainmenu Browser Changebookmark)
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_IsNeedStopApplet(void);

/*****************************************************************************/
//  Description : Set systerm frequency for mmi www
//  Global resource dependence :                     
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIWWW_SetFreq(void);
 
/*****************************************************************************/
//  Description : Get systerm frequency for mmi www
//  Global resource dependence :                     
//  Author: wancan.you
//  Note: 
/*****************************************************************************/ 
PUBLIC uint32 MMIWWW_RestoreFreq(void);

/*****************************************************************************/
//  Description : Tidy all windows except Mainmenu and browser window
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_TidyExistedWin(void);

#if 0//@fen.xie
/*****************************************************************************/
//  Description : Create CC select win
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIWWW_OpenCCSelectWin(MMIWWW_CC_INFO_T *in_cc_info);
#endif

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIWWW_OpenQueryGPRSWin(void);

/*****************************************************************************/
// Description : Handle query Use WIFI
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIWWW_HandleSelectWLAN(MMI_WIN_ID_T      win_id, 
                                          MMI_MESSAGE_ID_E  msg_id, 
                                          DPARAM            param);

/*****************************************************************************/
//  Description : select network type(sim or wifi)
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_SelectNetworkType(BOOLEAN is_include_wife);

/*****************************************************************************/
//  Description : Get Browser Instance
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC slim_int MMIWWW_GetBrowser(void);
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif

