/*****************************************************************************
** File Name:      mmibrowser_setting.h                                      *
** Author:          li.li                                                    *
** Date:           23/06/2009                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the browser setting         *
*****************************************************************************/
#ifndef MMIBROWSER_SETTING_H
#define MMIBROWSER_SETTING_H

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mn_type.h"
#include "mmibrowser_dorado_api.h"
#include "mmiconnection_export.h"
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
#define MMIBROWSER_SETTING_DEFAULT_INDEX    0
#define MMIBROWSER_HTTPS_STRING             "https"

#ifdef BROWSER_SEARCH_SUPPORT
#define MMIBROWSER_MAX_KEYNAME_LEN 49

#if defined(LOW_MEMORY_SUPPORT)  || !defined(NANDBOOT_SUPPORT)
#define MMIBROWSER_KEYNAME_NUM 20
#else
#define MMIBROWSER_KEYNAME_NUM 50
#endif
#endif

//CSTAR点阵字库只支持有限字号的中文
#ifdef FONT_TYPE_SUPPORT_CSTAR_BITMAP
#ifndef FONT_TYPE_SUPPORT_VECTOR
#if defined(IM_SIMP_CHINESE_SUPPORT) || defined(IM_TRAD_CHINESE_SUPPORT)
#define CSTAR_CHINESE_LIMITATION_SUPPORT
#endif
#endif
#endif

#define MMIBROWSER_HOMEPAGE_MAX_URL_LEN MMIBROWSER_MAX_URL_LEN

#define MMIBROWSER_PROPERTY_EXT_LEN             (50)
#define IS_VALID_CUSTOMIZED_CONNECTION(_CONNECTION_,_ACTIVE_SIM_) ((_CONNECTION_).is_connection_customized\
    && ((_CONNECTION_).dual_sys < MMI_DUAL_SYS_MAX)\
    && ((_ACTIVE_SIM_) == (_CONNECTION_).dual_sys)\
    && ((_CONNECTION_).connection_index < MMICONNECTION_MAX_SETTING_NUM)\
    )

#ifdef MMI_SSL_CERT_SUPPORT
#define MMIBROWSER_MAX_SSL_CERT_INFO    100
#define MMIBROWSER_MAX_STRING_INFO      50
#define MMIBROWSER_CHAR_COLON           0x3A
#define MMIBROWSER_SIG_HASH_OFFSET      3
#define MMIBROWSER_SSL_HASH_OFFSET      2
#endif
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef enum _MMIBROWSER_FONT
{
    MMIBROWSER_FONT_MINI  = 0,
#ifndef CSTAR_CHINESE_LIMITATION_SUPPORT
    MMIBROWSER_FONT_MIDDLE,
#endif
    MMIBROWSER_FONT_BIG,
    MMIBROWSER_FONT_MAX
}MMIBROWSER_FONT_E;

typedef enum _MMIBROWSER_ANIMATE_STATE
{
    MMIBROWSER_ANIMATE_STATE_NONE  = 0,
    MMIBROWSER_ANIMATE_STATE_ANIM,
    MMIBROWSER_ANIMATE_STATE_STOP,
    MMIBROWSER_ANIMATE_STATE_MAX
}MMIBROWSER_ANIMATE_STATE_E;

typedef enum _MMIBROWSER_BOOKMARK_STATE
{
    MMIBROWSER_BOOKMARK_STATE_NONE  = 0,
    MMIBROWSER_BOOKMARK_STATE_SAVE,
    MMIBROWSER_BOOKMARK_STATE_ADD,
    MMIBROWSER_BOOKMARK_STATE_EDIT,
    MMIBROWSER_BOOKMARK_STATE_MAX
}MMIBROWSER_BOOKMARK_STATE_E;

typedef enum
{
    MMIBROWSER_WEB_ENTRY_NONE,
    MMIBROWSER_WEB_ENTRY_URL,//access url
    MMIBROWSER_WEB_ENTRY_SNAPSHOT,//access snapshot

    MMIBROWSER_WEB_ENTRY_TYPE_MAX
}MMIBROWSER_WEB_ENTRY_TYPE_E;

typedef struct _MMIBROWSER_BOOKMARK_EDIT
{
    wchar* title_ptr;
    wchar* url_ptr;
    uint32 title_len;
    uint32 url_len;
} MMIBROWSER_BOOKMARK_EDIT_T; 

typedef struct _MMIBROWSER_CONTEXT
{
    BRW_INSTANCE                        browser_instance;
    BOOLEAN                             is_instance_inited;//is_instance_inited
    BOOLEAN                             is_loading;
    BOOLEAN                             is_need_display;
    BOOLEAN                             is_setting_changed;
    BOOLEAN                             is_setting_username;
    BOOLEAN                             is_setting_password;
    //BOOLEAN                           is_save_to_phone;
    BOOLEAN                             is_download_media;
    BOOLEAN                             is_need_clear;//clear all and repait, when open, lose focus, lcd switch...
    BOOLEAN                             is_begin_download;
    BOOLEAN                             is_snapshot;
    MMIBROWSER_ANIMATE_STATE_E          animate_state;
    char    *url_ptr;//the url for edit and new link
    char   *user_name_ptr;//utf8
    char    *password_ptr;//ascii
    BRW_MSG_AUTH_REQ_T  auth_req;
    MMI_STRING_T    save_file;
    BRW_MSG_ENTER_FILENM_REQ_T  enter_file_name;
    uint32  recv_data_len;
    uint32  total_data_len;
    uint32  curr_media_num;
    uint32  total_media_num;
    uint32  req_handle;
    uint32  scoll_handle;
#ifdef SSL_SUPPORT
    BRW_MSG_SSL_CERT_IND_T  ssl_cert;
#endif
    uint8   key_timer_id;
    MMI_MESSAGE_ID_E    key_msg;
    MMIBROWSER_MIME_TYPE_E  mime_type;
    MMIFILE_DEVICE_E    save_device;//add by charlie.wang for multi sd card 2011.03.02
    MMIBROWSER_WEB_ENTRY_TYPE_E entry_type;	//url, snapshot
    wchar   *snapshot_filename_ptr;	//snapshot file name
    char    *snapshot_url_ptr;//the url info of current snapshot
    wchar   *ico_filename_ptr;	//ico file name
} MMIBROWSER_CONTEXT_T;

typedef struct _MMIDORADO_CUSTOMIZED_CONNECTION_T_
{
    MN_DUAL_SYS_E dual_sys;
    BOOLEAN is_connection_customized;
    uint8 connection_index;//the connection index for dual_sys
}MMIDORADO_CUSTOMIZED_CONNECTION_T;

typedef struct _MMIBROWSER_START_CONTEXT
{
    MMIWWW_ENTRY_TYPE_E entry_type;	//homepage, url
    BOOLEAN                             is_browser_inited;
    BOOLEAN                             is_use_wifi;
    MN_DUAL_SYS_E                       sim_type;
    MMIBROWSER_BOOKMARK_STATE_E         bookmark_state;
    char    *entry_url_ptr;
    MMIBROWSER_BOOKMARK_EDIT_T  bookmark_edit;
    char    *user_agent_ptr;
    char    *ua_profile_ptr;
    BRW_INSTANCE        fixed_browser_instance;//the instance of the fixed
    MMIDORADO_CUSTOMIZED_CONNECTION_T   customized_connection;
} MMIBROWSER_START_CONTEXT_T;

typedef enum
{
    MMIBROWSER_HOMEPAGE_MODE_DEFAULT = 0,//默认
    MMIBROWSER_HOMEPAGE_MODE_CUSTOM,//自定义
    MMIBROWSER_HOMEPAGE_MODE_MAX, 
}MMIBROWSER_HOMEPAGE_MODE_E;

typedef enum
{
    MMIBROWSER_CLEAR_TYPE_HISTORY,
#ifdef MMIDORADO_INPUT_HISTORY_SUPPORT
    MMIBROWSER_CLEAR_TYPE_INPUT_HISTORY,
#endif    
#ifdef BROWSER_SEARCH_SUPPORT
    MMIBROWSER_CLEAR_TYPE_KEY,
#endif    
    MMIBROWSER_CLEAR_TYPE_CACHE,
    MMIBROWSER_CLEAR_TYPE_COOKIE,

    MMIBROWSER_CLEAR_TYPE_MAX
}MMIBROWSER_CLEAR_TYPE_E;

typedef struct _MMIBROWSER_NV_SETTING
{
    uint8   sim_net_index[MMI_DUAL_SYS_MAX];
    BOOLEAN             is_image_loaded;
    BOOLEAN             is_sound_played;
    BOOLEAN             is_cookie_enable;
    BOOLEAN             is_cache_enable;
    BOOLEAN             is_ssl_warning_enable;
    BOOLEAN             is_google_change_enable;
    MMIBROWSER_FONT_E   font;
    MMIBROWSER_HOMEPAGE_MODE_E  homepage_mode;
    uint8   homepage[MMIBROWSER_HOMEPAGE_MAX_URL_LEN+1];	//自定义主页
    BOOLEAN   clear_set_array[MMIBROWSER_CLEAR_TYPE_MAX];	//clear option setting
}MMIBROWSER_NV_SETTING_T;

#ifdef BROWSER_SEARCH_SUPPORT
typedef struct
{
    wchar wstr[MMIBROWSER_MAX_KEYNAME_LEN+1];
    uint16 wstr_len;
}MMIBROWSER_KEY_NAME_T;

typedef struct
{
    uint16 num;
    MMIBROWSER_KEY_NAME_T key_arr[MMIBROWSER_KEYNAME_NUM];
}MMIBROWSER_KEY_T;

typedef struct
{
    uint16 match_num;
    MMIBROWSER_KEY_NAME_T *match_key_ptr;
}MMIBROWSER_MATCH_KEY_T;

typedef enum
{
    MMIBROWSER_SEARCH_ENGINE_DEFAULT = 0,
    MMIBROWSER_SEARCH_ENGINE_BAIDU = 0,
    MMIBROWSER_SEARCH_ENGINE_SOSO,
    MMIBROWSER_SEARCH_ENGINE_MAX_NUM,
    MMIBROWSER_SEARCH_ENGINE_CMCC = MMIBROWSER_SEARCH_ENGINE_MAX_NUM,
    MMIBROWSER_SEARCH_ENGINE_YAHOO,
    MMIBROWSER_SEARCH_ENGINE_GOOGLE,

    MMIBROWSER_SEARCH_ENGINE_MAX, 
}MMIBROWSER_SEARCH_ENGINE_MODE_E;

typedef struct
{
    MMI_TEXT_ID_T   text_id;
    MMI_IMAGE_ID_T   img_id;
    char *search_head_ptr;
    char *search_tail_ptr;
}MMIBROWSER_SEARCH_INFO_T;
#endif/*BROWSER_SEARCH_SUPPORT*/

#ifdef MMI_SSL_CERT_SUPPORT
typedef enum
{
    MMIBROWSER_CERT_VIEW_MODE,
    MMIBROWSER_CERT_IMPORT_MODE,
		
    MMIBROWSER_CERT_MODE_MAX
}MMIBROWSER_CERT_MODE_E;
#endif
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : sett s_nv_setting from NV(MMIBROWSER_SETTING)
//  Global resource dependence : s_nv_setting
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_InitSetting(void);

/*****************************************************************************/
//  Description : get nv setting of browser
//  Global resource dependence : s_nv_setting
//  Author: fen.xie
//  Note: after MMIBROWSER_InitSetting
/*****************************************************************************/
PUBLIC MMIBROWSER_NV_SETTING_T* MMIBROWSER_GetNVSetting(void);

/*****************************************************************************/
//  Description : update nv setting(MMIBROWSER_SETTING) from s_nv_setting
//  Global resource dependence : s_nv_setting
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_UpdateNVSetting(void);

#ifndef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : set image setting
//  Global resource dependence : s_nv_setting
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetImageSetting(BOOLEAN is_image_loaded);

/*****************************************************************************/
//  Description : set bgsound setting
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetBgsoundSetting(BOOLEAN is_sound_played);

#ifdef MMIDORADO_GOOGLE_TRANSCODE_SUPPORT
/*****************************************************************************/
//  Description : set google change setting
//  Global resource dependence : s_nv_setting
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetGoogleSetting(BOOLEAN is_google_change_enable);
#endif
#endif

/*****************************************************************************/
//  Description : set font setting
//  Global resource dependence : s_nv_setting
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetFontSetting(MMIBROWSER_FONT_E font_type);

/*****************************************************************************/
//  Description : get font setting
//  Global resource dependence : s_nv_setting
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_FONT_E MMIBROWSER_GetFontSetting(void);

/*****************************************************************************/
//  Description : set sim's network accout index setting
//  Global resource dependence : s_nv_setting
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetNetIndexSetting(MN_DUAL_SYS_E dual_sys, uint8 index);

/*****************************************************************************/
//  Description :set homepage to s_nv_setting
//  Global resource dependence : s_nv_setting
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_SetHomepageSetting(const MMI_STRING_T *string_ptr);

/*****************************************************************************/
//  Description :Get homepage
//  Global resource dependence : s_nv_setting
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC uint8* MMIBROWSER_GetHomepageSetting(void);

/*****************************************************************************/
//  Description :set homepage mode to s_nv_setting
//  Global resource dependence : s_nv_setting
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_SetHomepageModeSetting(MMIBROWSER_HOMEPAGE_MODE_E homepage_mode);

/*****************************************************************************/
//  Description :Get homepage mode
//  Global resource dependence : s_nv_setting
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_HOMEPAGE_MODE_E MMIBROWSER_GetHomepageModeSetting(void);

/*****************************************************************************/
//  Description : set sim type
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_ERROR_E MMIBROWSER_SetSim(MN_DUAL_SYS_E sim);

/*****************************************************************************/
//  Description : get sim type
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIBROWSER_GetSim(void);

/*****************************************************************************/
//  Description : update the edit url in web
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_ERROR_E MMIBROWSER_UpdateUrl(char* url_ptr);

/*****************************************************************************/
//  Description : free the edit url in web
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_FreeUrl(void);

/*****************************************************************************/
//  Description : get the edit url
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC char* MMIBROWSER_GetUrl(void);

/*****************************************************************************/
//  Description : set browser init
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetKernelInit(BOOLEAN is_inited);

/*****************************************************************************/
//  Description : get browser is inited
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_GetKernelInit(void);

/*****************************************************************************/
//  Description : set browser instance init
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetIntanceInit(BOOLEAN is_inited);

/*****************************************************************************/
//  Description : get browser instance is inited
//  Global resource dependence : 
//  Author: fen,xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_GetIntanceInit(void);

/*****************************************************************************/
//  Description : set instance
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetInstance(BRW_INSTANCE brw_instace);

/*****************************************************************************/
//  Description : get instance
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC BRW_INSTANCE MMIBROWSER_GetInstance(void);

/*****************************************************************************/
//  Description : set browser is loading
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetLoading(BOOLEAN is_loading);

/*****************************************************************************/
//  Description : get the browser is loading
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_GetLoading(void);

/*****************************************************************************/
//  Description : set the browser is need display
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetNeedDisplay(BOOLEAN is_need_display);

/*****************************************************************************/
//  Description : get the browser is need display
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_GetNeedDisplay(void);

/*****************************************************************************/
//  Description : set animate connect control state
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetAnimateConnectCtrlState(MMIBROWSER_ANIMATE_STATE_E state, BOOLEAN is_update);

/*****************************************************************************/
//  Description : update the user name
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_ERROR_E MMIBROWSER_UpdateUserName(wchar* user_name_ptr);

/*****************************************************************************/
//  Description : update the password
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_ERROR_E MMIBROWSER_UpdatePassword(wchar* password_ptr);

/*****************************************************************************/
//  Description : get the username
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC char* MMIBROWSER_GetUserName(void);

/*****************************************************************************/
//  Description : get the password
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC char* MMIBROWSER_GetPassword(void);

/*****************************************************************************/
//  Description : free the user name
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_FreeUserName(void);

/*****************************************************************************/
//  Description : free the password
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_FreePassword(void);

/*****************************************************************************/
//  Description : free the user_agent_ptr
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_FreeUserAgent(void);

/*****************************************************************************/
//  Description : free the user_agent_ptr
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_FreeUaProfile(void);

/*****************************************************************************/
//  Description : get password setting
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_GetPasswordSetting(void);

/*****************************************************************************/
//  Description : get user name setting
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_GetUserNameSetting(void);

/*****************************************************************************/
//  Description : update auth req
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_ERROR_E MMIBROWSER_UpdateAuthReq(BRW_MSG_AUTH_REQ_T* req_ptr);

/*****************************************************************************/
//  Description : get auth req
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC BRW_MSG_AUTH_REQ_T* MMIBROWSER_GetAuthReq(void);


/*****************************************************************************/
//  Description : update save file
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_UpdateSaveFile(MMI_STRING_T save_file);

/*****************************************************************************/
//  Description : get save file
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC MMI_STRING_T* MMIBROWSER_GetSaveFile(void);

/*****************************************************************************/
//  Description : free save file
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_FreeSaveFile(void);

/*****************************************************************************/
//  Description : update bookmark edit
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_ERROR_E MMIBROWSER_UpdateBookmarkEdit(MMIBROWSER_BOOKMARK_EDIT_T* bookmark_edit_ptr);

/*****************************************************************************/
//  Description : get bookmark edit
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_BOOKMARK_EDIT_T* MMIBROWSER_GetBookmarkEdit(void);

/*****************************************************************************/
//  Description : free bookmark edit
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_FreeBookmarkEdit(void);

/*****************************************************************************/
//  Description : set browser bookmark state
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetBookmarkState(MMIBROWSER_BOOKMARK_STATE_E bookmark_state);

/*****************************************************************************/
//  Description : get the browser bookmark state
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_BOOKMARK_STATE_E MMIBROWSER_GetBookmarkState(void);

/*****************************************************************************/
//  Description : set setting change
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetSettingChange(BOOLEAN is_setting_changed);

/*****************************************************************************/
//  Description : get setting change
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_GetSettingChange(void);

/*****************************************************************************/
//  Description : update enter file name req's file name arr
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_ERROR_E MMIBROWSER_UpdateEnterFileName(uint16 *res_name_ptr);

/*****************************************************************************/
//  Description : update enter file name req
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_ERROR_E MMIBROWSER_UpdateFileNameReq(BRW_MSG_ENTER_FILENM_REQ_T* req_ptr);

/*****************************************************************************/
//  Description : get entry file name req
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC BRW_MSG_ENTER_FILENM_REQ_T* MMIBROWSER_GetFileNameReq(void);

#ifdef SSL_SUPPORT
/*****************************************************************************/
//  Description : update ssl cert
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_ERROR_E MMIBROWSER_UpdateSSLCert(BRW_MSG_SSL_CERT_IND_T* ssl_cert_ptr);

/*****************************************************************************/
//  Description : get ssl cert
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC BRW_MSG_SSL_CERT_IND_T* MMIBROWSER_GetSSLCert(void);
#endif

/*****************************************************************************/
//  Description : reset context
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_ResetContext(void);

/*****************************************************************************/
//  Description : free enter file name
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_FreeEnterFileName(void);

/*****************************************************************************/
//  Description : free auth req
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_FreeAuthReq(void);

/*****************************************************************************/
//  Description : set browser is save to phone
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
//PUBLIC void MMIBROWSER_SetSaveToPhone(BOOLEAN is_save_to_phone);

/*****************************************************************************/
//  Description : get the browser is save to phone
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
//PUBLIC BOOLEAN MMIBROWSER_GetSaveToPhone(void);

/*****************************************************************************/
//  Description : set browser is save to phone
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetSaveDeviceType(MMIFILE_DEVICE_E dev);

/*****************************************************************************/
//  Description : get the browser is save to phone
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIBROWSER_GetSaveDeviceType(void);

/*****************************************************************************/
//  Description : set is download media
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetDownloadMedia(BOOLEAN is_download_media);

/*****************************************************************************/
//  Description : get is download media
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_GetDownloadMedia(void);

/*****************************************************************************/
//  Description : update recv data len
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_UpdateRecvDataLen(uint32 curr_recv_data_len);

/*****************************************************************************/
//  Description : get recv data len
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIBROWSER_GetRecvDataLen(void);

/*****************************************************************************/
//  Description : clear recv data len
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_ClearRecvDataLen(void);

/*****************************************************************************/
//  Description : update req handle
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_UpdateReqHandle(uint32 req_handle);

/*****************************************************************************/
//  Description : get req handle
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIBROWSER_GetReqHandle(void);

/*****************************************************************************/
//  Description : set total data len
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetTotalDataLen(uint32 total_data_len);

/*****************************************************************************/
//  Description : get total data len
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIBROWSER_GetTotalDataLen(void);

/*****************************************************************************/
//  Description : set scroll handle
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetScrollHandle(uint32 scoll_handle);

/*****************************************************************************/
//  Description : get scroll handle
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIBROWSER_GetScrollHandle(void);

/*****************************************************************************/
//  Description : free bookmark edit title
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_FreeBookmarkEditTitle(void);

/*****************************************************************************/
//  Description : set browser key timer id  
//  Global resource dependence : none
//  Author: li.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetKeyTimerID(uint8 key_timer_id);

/*****************************************************************************/
//  Description : set browser key msg
//  Global resource dependence : none
//  Author: li.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetKeyMSG(MMI_MESSAGE_ID_E key_msg);

/*****************************************************************************/
//  Description : get browser key timer id  
//  Global resource dependence : none
//  Author: li.li
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIBROWSER_GetKeyTimerID(void);

/*****************************************************************************/
//  Description : get browser key msg
//  Global resource dependence : none
//  Author: li.li
//  Note:
/*****************************************************************************/
PUBLIC MMI_MESSAGE_ID_E MMIBROWSER_GetKeyMSG(void);

/*****************************************************************************/
//  Description : set total media num
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetTotalMediaNum(uint32 total_media_num);

/*****************************************************************************/
//  Description : get total media num
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIBROWSER_GetTotalMediaNum(void);

/*****************************************************************************/
//  Description : update curr media num
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_UpdateCurrMediaNum(uint32 curr_media_num);

/*****************************************************************************/
//  Description : get current media num
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIBROWSER_GetCurrMediaNum(void);

/*****************************************************************************/
//  Description : set curr media num
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetCurrMediaNum(uint32 curr_media_num);

/*****************************************************************************/
//  Description : get the browser is need clear
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_GetIsNeedClear(void);

/*****************************************************************************/
//  Description : set the browser is need clear
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetIsNeedClear(BOOLEAN is_need_clear);

/*****************************************************************************/
//  Description : set begin download
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetBeginDownload(BOOLEAN is_begin_download);

/*****************************************************************************/
//  Description : get begin download
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_GetBeginDownload(void);

/*****************************************************************************/
//  Description : set is snapshot
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetIsSnapshot(BOOLEAN is_snapshot);

/*****************************************************************************/
//  Description : get is snapshot
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_GetIsSnapshot(void);

/*****************************************************************************/
//  Description : set mime type
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetMimeType(MMIBROWSER_MIME_TYPE_E mime_type);

/*****************************************************************************/
//  Description : get mime type
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_MIME_TYPE_E MMIBROWSER_GetMimeType(void);

/*****************************************************************************/
//  Description : update file name
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_UpdateFileName(wchar* file_name_ptr);

/*****************************************************************************/
//  Description : get file name
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC wchar* MMIBROWSER_GetFileName(void);

/*****************************************************************************/
//  Description : update the user agent
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_ERROR_E MMIBROWSER_UpdateUserAgent(char* user_agent_ptr);

/*****************************************************************************/
//  Description : get the user agent
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC char* MMIBROWSER_GetUserAgent(void);

/*****************************************************************************/
//  Description : update browser net setting when the index list item is deleted  
//  Global resource dependence : none
//  Author: li.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIBROWSER_Setting_ConnectionCallback(MMICONNECTION_NOTIFY_EVENT_E notify_event, uint32 param, MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : set is use wifi
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetIsUseWifi(BOOLEAN is_use_wifi);

/*****************************************************************************/
//  Description : get is use wifi
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_GetIsUseWifi(void);

/*****************************************************************************/
//  Description : update the ua profile
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_ERROR_E MMIBROWSER_UpdateUaProfile(char* ua_profile_ptr);

/*****************************************************************************/
//  Description : get the ua profile
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC char* MMIBROWSER_GetUaProfile(void);

#ifdef BROWSER_SEARCH_SUPPORT
/*****************************************************************************/
//  Description : get search_engine
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_SEARCH_ENGINE_MODE_E MMIBROWSER_GetSearchEngine(void);

/*****************************************************************************/
//  Description : Set search_engine
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetSearchEngine(MMIBROWSER_SEARCH_ENGINE_MODE_E search_engine);

/*****************************************************************************/
// 	Description : clean the key info in NV
//	Global resource dependence : none
//   Author: fen.xie
//	Note:
/*****************************************************************************/
PUBLIC void MMIBROWSER_CleanKeyInfo(void);

/*****************************************************************************/
//  Description : get the key info in NV 
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_GetKeyInfo(MMIBROWSER_KEY_T *key_data_ptr);

/*****************************************************************************/
//  Description : Set the key info in NV 
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetKeyInfo(MMIBROWSER_KEY_T *key_data_ptr);
#endif

/*****************************************************************************/
//  Description : set entry_type
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetEntryType(MMIWWW_ENTRY_TYPE_E entry_type);

/*****************************************************************************/
//  Description : get entry_type
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC MMIWWW_ENTRY_TYPE_E MMIBROWSER_GetEntryType(void);

/*****************************************************************************/
//  Description : update the snapshot file name in web
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_ERROR_E MMIBROWSER_UpdateSnapshotFilename(wchar* file_name_ptr);

/*****************************************************************************/
//  Description : free the snapshot file name in web
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_FreeSnapshotFilename(void);

/*****************************************************************************/
//  Description : get the snapshot file name in web
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC wchar* MMIBROWSER_GetSnapshotFilename(void);

/*****************************************************************************/
//  Description : set entry_type
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetEntryType(MMIWWW_ENTRY_TYPE_E entry_type);

/*****************************************************************************/
//  Description : get entry_type
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC MMIWWW_ENTRY_TYPE_E MMIBROWSER_GetEntryType(void);

/*****************************************************************************/
//  Description : set entry_type when open the web
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetWebEntryType(MMIBROWSER_WEB_ENTRY_TYPE_E entry_type);

/*****************************************************************************/
//  Description : get entry_type when open the web
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_WEB_ENTRY_TYPE_E MMIBROWSER_GetWebEntryType(void);

/*****************************************************************************/
//  Description : free the url for entry browser
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_FreeEntryUrl(void);

/*****************************************************************************/
//  Description : update the url for entry browser
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_ERROR_E MMIBROWSER_UpdateEntryUrl(char* url_ptr);

/*****************************************************************************/
//  Description : get the url for entry browser
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC char* MMIBROWSER_GetEntryUrl(void);

/*****************************************************************************/
//  Description : reset browser start infomation
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_ResetStartInfo(void);

/*****************************************************************************/
//  Description : reset the param for reset factory setting
//  Global resource dependence : s_nv_setting
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_ResetNvSetting(void);

/*****************************************************************************/
//  Description : current sim's index is modfied with default setting
//  Global resource dependence : 
//  Author: fen.xie
//  Note: MS00243346
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_IsSimIndexModifyByDefault(MN_DUAL_SYS_E cur_sim_type);
/*****************************************************************************/
//  Description : current font is modfied with default setting
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_IsFontModifyByDefault(void);

/*****************************************************************************/
//  Description : update the snapshot_url_ptr
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_ERROR_E MMIBROWSER_UpdateSnapshotUrl(char* url_ptr);

/*****************************************************************************/
//  Description : free the snapshot_url_ptr
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_FreeSnapshotUrl(void);

/*****************************************************************************/
//  Description : get the snapshot_url_ptr
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC char* MMIBROWSER_GetSnapshotUrl(void);


/*****************************************************************************/
//  Description : set fixed instance
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetFixedInstance(BRW_INSTANCE brw_instace);

/*****************************************************************************/
//  Description : get fixed instance
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BRW_INSTANCE MMIBROWSER_GetFixedInstance(void);


/*****************************************************************************/
//  Description : update the ico file name 
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_ERROR_E MMIBROWSER_UpdateIcoFilename(wchar* file_name_ptr);

/*****************************************************************************/
//  Description : free the ico file name
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_FreeIcoFilename(void);

/*****************************************************************************/
//  Description : get the ico file name
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC wchar* MMIBROWSER_GetIcoFilename(void);

/*****************************************************************************/
//  Description : set customized Connection Settings
//  Global resource dependence : s_browser_start_info
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDORADO_SetConnectionSettings(MMIDORADO_CUSTOMIZED_CONNECTION_T customized_connection);

/*****************************************************************************/
//  Description : Get customized Connection Settings
//  Global resource dependence : s_browser_start_info
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC MMIDORADO_CUSTOMIZED_CONNECTION_T MMIDORADO_GetConnectionSettings(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif










