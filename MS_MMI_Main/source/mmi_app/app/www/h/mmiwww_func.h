/******************************************************************************
** File Name:      mmiwww_func.h                                              *
** Author:                                                                    *
** Date:           2009/06/26                                                 *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:    This file is used to describe www browser definitions      *
*******************************************************************************
**                         Important Edit History                             *
** -------------------------------------------------------------------------- *
** DATE           NAME             DESCRIPTION                                *
** 2009/06/26     kun.yu           Create                                     *
******************************************************************************/
#ifndef _MMIWWW_FUNC_H_
#define _MMIWWW_FUNC_H_
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "dal_time.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : add one item to listbox for www mdu 
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_AppendOneListItem(
                                    MMI_CTRL_ID_T ctrl_id,
                                    MMI_STRING_T *text_str,
                                    MMI_TEXT_ID_T left_softkey_id,
                                    MMI_TEXT_ID_T center_softkey_id,
                                    MMI_TEXT_ID_T right_softkey_id
                                   );

/*****************************************************************************/
//  Description : add item to listbox for www mdu 
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_AppendListItem(
                                  MMI_WIN_ID_T win_id, 
                                  MMI_CTRL_ID_T ctrl_id, 
                                  MMI_TEXT_ID_T *append_arr,
                                  uint16 max_item, 
                                  uint16 cur_item 
                                  );

/*****************************************************************************/
// Description : Start zoom operation timer
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_StartZoomTimer(MMI_HANDLE_T  win_handle, BOOLEAN is_zoomin);

/*****************************************************************************/
// Description : Stop zoom operation timer
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_StopZoomTimer(MMI_HANDLE_T  win_handle);

/*****************************************************************************/
//  Description : add history title item to listbox for www mdu 
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_AppendTitleItemData( MMI_CTRL_ID_T ctrl_id, uint16 index,  MMI_STRING_T string, MMI_IMAGE_ID_T image_id, MMI_TEXT_ID_T text1_id, MMI_TEXT_ID_T text2_id, MMI_TEXT_ID_T text3_id ); 

/*****************************************************************************/
//  Description : add dynamic item to listbox when item_num is not zero
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_AppendListboxitem( 
                                        MMI_WIN_ID_T win_id, 
                                        MMI_CTRL_ID_T ctrl_id, 
                                        uint16 item_num, 
                                        MMI_TEXT_ID_T text1_id,
                                        MMI_TEXT_ID_T text2_id,
                                        MMI_TEXT_ID_T text3_id
                                        );

/*****************************************************************************/
//  Description : Set url change flag
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_NetfrontSetUrlChange(BOOLEAN is_change);

/*****************************************************************************/
//  Description : Get url change flag
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_NetfrontGetUrlChange(void);

/*****************************************************************************/
//  Description : Set keywork change flag
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_NetfrontSetKeywordChange(BOOLEAN is_change);

/*****************************************************************************/
//  Description : Get keywork change flag
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_NetfrontGetKeywordChange(void);

/*****************************************************************************/
//  Description : Get link button status
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_NetfrontGetLinkButtonDisabled(void);

/*****************************************************************************/
//  Description : Get search button status
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_NetfrontGetSearchButtonDisabled(void);

/*****************************************************************************/
//  Description : Set bookmark change flag
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_NetfrontSetBookmarkChange(BOOLEAN is_change);

/*****************************************************************************/
//  Description : Get bookmark change flag
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_NetfrontGetBookmarkChange(void);

/*****************************************************************************/
//  Description : Set PDP info
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_NetfrontSetPDPInfo(MMIWWW_PDP_INFO_T *pdp_info);

/*****************************************************************************/
//  Description : Get PDP info
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_NetfrontGetPDPInfo(MMIWWW_PDP_INFO_T *pdp_info);

/*****************************************************************************/
//  Description : Get PDP info of settings
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_NetfrontGetSettingsPDPInfo(MMIWWW_PDP_INFO_T *pdp_info, MN_DUAL_SYS_E e_dual_sys);

/*****************************************************************************/
//  Description : Set added url address
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_NetfrontSetAddedUrl(uint8 *in_url);

/*****************************************************************************/
//  Description : Get added url address
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC uint8 *MMIWWW_NetfrontGetAddedUrl(void);

/*****************************************************************************/
//  Description : Set added url title
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_NetfrontSetAddedTitle(uint8 *in_title);

/*****************************************************************************/
//  Description : Get added url title
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC uint8 *MMIWWW_NetfrontGetAddedTitle(void);

/*****************************************************************************/
//  Description : Generate search web
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC uint8 * MMIWWW_GenerateSearchWeb(MMIWWW_SEARCH_ENGINE_MODE_E search_engine, MMI_STRING_T *search_str);

/*****************************************************************************/
//  Description : Check the URL validity
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_IsURLValid(MMI_STRING_T *url_str);

/*****************************************************************************/
//  Description : Go to URL
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_ActionGoToURL(MMI_STRING_T *url_str);

/*****************************************************************************/
// Description : Get Match key num
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/
PUBLIC uint16 MMIWWW_GetMatchKeyNum(wchar *key_name_ptr);

/*****************************************************************************/
// Description : List match key name
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/
PUBLIC uint16 MMIWWW_ListMatchKeyName( wchar *key_name_ptr, MMI_HANDLE_T ctr_handle );

/*****************************************************************************/
// Description : Get Match key num
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/
PUBLIC uint16 MMIWWW_GetMatchURLNum(wchar *url_ptr);

/*****************************************************************************/
// Description : List match URL 
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/
PUBLIC uint16 MMIWWW_ListMatchURLName( wchar *url_ptr, MMI_HANDLE_T ctr_handle );

/*****************************************************************************/
//  Description : Get www font style
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIWWW_GetFontStyle(void);

/*****************************************************************************/
//  Description : Set charset
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_SetCharsetIndex(slim_int  index, uint16 charset_index);

/*****************************************************************************/
//  Description : Get charset
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIWWW_GetCharsetIndex(slim_int  index);

/*****************************************************************************/
//  Description : Set charset change
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_SetCharsetChange(slim_int  index, BOOLEAN is_change);

/*****************************************************************************/
//  Description : Get charset change
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_GetCharsetChange(slim_int  index);

/*****************************************************************************/
//  Description : Convert index to charset
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC int32 MMIWWW_ConvertCharsetIndexToCharset(uint16  charset_index);

/*****************************************************************************/
//  Description : Get www view mode
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIWWW_GetViewMode(void);

/*****************************************************************************/
//  Description : Get www cookie mode
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIWWW_GetCookieMode(void);

/*****************************************************************************/
//  Description : Get www cache mode
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIWWW_GetCacheMode(void);

/*****************************************************************************/
//  Description : Get www disk cache mode
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIWWW_GetDiskCacheMode(void);

/*****************************************************************************/
//  Description : Get www save WebData mode
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_GetWebDataMode(void);

/*****************************************************************************/
//  Description : Get www save script mode
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIWWW_GetScriptMode(void);

/*****************************************************************************/
//  Description : Get www image mode
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIWWW_GetImageMode(void);

/*****************************************************************************/
//  Description : Get www Animate mode
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_GetAnimateMode(void);

/*****************************************************************************/
//  Description : Get www bgsound mode
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_GetBGSoundMode(void);

/*****************************************************************************/
//  Description : Get www table mode
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_GetTableMode(void);

/*****************************************************************************/
//  Description : Set focus ctrl of main menu 
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_NetfrontSetMainMenuFocusCtrl(MMIWWW_CTRL_E in_ctrl);

/*****************************************************************************/
//  Description : Get focus ctrl of main menu
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC MMIWWW_CTRL_E MMIWWW_NetfrontGetMainMenuFocusCtrl(void);

/*****************************************************************************/
// Description : Set browser state
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_SetWWWState(slim_int  index, MMIWWW_STATE_T  state);

/*****************************************************************************/
// Description : Get browser state
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC MMIWWW_STATE_T MMIWWW_GetWWWState(slim_int  index);

/*****************************************************************************/
// Description : Set browser fullscreen display flag
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_SetFullDisplayFlag(BOOLEAN  is_full_display);

/*****************************************************************************/
// Description : Get browser fullscreen display flag
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_IsFullDisplay(void);

/*****************************************************************************/
// Description : Set exit browser flag
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_SetExitFlag(BOOLEAN  is_exit);

/*****************************************************************************/
// Description : Get exit browser flag
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_IsExit(void);

/*****************************************************************************/
// Description : Set page secure flag
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_SetPageSecureFlag(slim_int  index, slim_bool is_secure_page);

/*****************************************************************************/
// Description : Get page secure flag
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_bool MMIWWW_GetPageSecureFlag(slim_int  index);

/*****************************************************************************/
// Description : Set cuurent page zoom percent
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_SetCurrentPageZoomPercent(MMIWWW_ZOOM_E zoom_percent);

/*****************************************************************************/
// Description : Get cuurent page zoom percent
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
PUBLIC MMIWWW_ZOOM_E MMIWWW_GetCurrentPageZoomPercent(void);

/*****************************************************************************/
//  Description : Play BG sound
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_Play_BG_sound(slim_int in_index);

/*****************************************************************************/
//  Description : Calculate days from current year 1.1 to current day
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIWWW_CountDate_CurYear(SCI_DATE_T sys_date);

/*****************************************************************************/
//  Description : Calculate days from 1970.1.1 to current day
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIWWW_CountDate_From1970(SCI_DATE_T sys_date);

/*****************************************************************************/
//  Description : Get search engine
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIWWW_GetSearchEngine(void);

/*****************************************************************************/
//  Description : Set search engine
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_SetSearchEngine(uint8 *search_engine);

/*****************************************************************************/
//  Description : Get first entry flag
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_GetFirstEntry(void);

/*****************************************************************************/
//  Description : Set first entry flag
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_SetFirstEntry(BOOLEAN first_entry);

/*****************************************************************************/
//  Description : Get Pointer Mode flag
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC MMIWWW_POINTER_MODE_E MMIWWW_GetPointerMode(void);

/*****************************************************************************/
//  Description : Set Pointer Mode flag
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_SetPointerMode(MMIWWW_POINTER_MODE_E pointer_mode);

/*****************************************************************************/
//  Description : Set default user dada
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_SetDefaultUserData(MMIWWW_INSTANCE_T *self);

/*****************************************************************************/
//  Description : Set default bookmark
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_SetDefaultBookmartk(MMIWWW_INSTANCE_T *self);

/*****************************************************************************/
// Description : Set browser first open flag
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_SetIsFirstOpen(BOOLEAN  flag);

/*****************************************************************************/
// Description : Get browser first open flag
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_IsFirstOpen(void);

/*****************************************************************************/
// Description : Save url data
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_SaveUrl(NFBrowserStartType in_type, char *in_url);

/*****************************************************************************/
// Description : Get saved url data
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC uint8 *MMIWWW_GetSavedUrl(NFBrowserStartType  *out_type);

/*****************************************************************************/
//  Description : Set www net setting index 
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_SetNetSettingIndex(MN_DUAL_SYS_E dual_sys, uint8 index);

/*****************************************************************************/
//  Description : init connection setting 
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_InitConnectionSetting(void);

/*****************************************************************************/
//  Description : Get num from URL
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_GetNumFromURL(char *str_dest, const char* str_src, char *str, BOOLEAN is_dtmf);

/*****************************************************************************/
//  Description : Get weather mainmenu get focus
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_NetfrontGetMainMenuFocusState(void);

/*****************************************************************************/
//  Description : Set mainmenu focus state
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_NetfrontSetMainMenuFocusState( BOOLEAN bfocus );

/*****************************************************************************/
//  Description : Get selected mime type from content ptr
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIWWW_GetSelectedImageMIMEIndex(void);

/*****************************************************************************/
//  Description : Clean Usert Data Info
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_CleanUserInfo(void);

/*****************************************************************************/
//  Description : Set Http Proxy
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_SetHTTPProxy(char *in_url);

/*****************************************************************************/
//  Description : Set Init User Agent
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_SetInitUA(const char * user_agent_ptr);

/*****************************************************************************/
//  Description : Get Init User Agent
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC char* MMIWWW_GetInitUA(void);

/*****************************************************************************/
//  Description : Free Init User Agent
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_FreeInitUA(void);
/*****************************************************************************/
//  Description : free phone num memory
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIWWW_NetfrontGetPagememoSortType(void);

/*****************************************************************************/
//  Description : Set Search String
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_SetSearchString(MMI_STRING_T *search_string_ptr);

/*****************************************************************************/
//  Description : Get Search String
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC MMI_STRING_T * MMIWWW_GetSearchString(void);

/*****************************************************************************/
//  Description : Free Search String
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_FreeSearchString(void);

/*****************************************************************************/
//  Description : Set Js Promt String
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_SetJSPromtString(slim_char *in_message, slim_char *in_value);

/*****************************************************************************/
//  Description : Get Js Promt String
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_GetJSPromtString(slim_char  **in_message, slim_char  **in_value);

/*****************************************************************************/
//  Description : Free Js Promt String
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_FreeJSPromtString(void);

/*****************************************************************************/
//  Description : Set Customized Connection
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_SetCustomizedConnection(DPARAM param);

/*****************************************************************************/
//  Description : Get Customized Connection
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_GetCustomizedConnection(uint8 * connection_index_ptr);

/*****************************************************************************/
//  Description : Reset Net Connect
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_ResetNetConnect(void);

/*****************************************************************************/
//  Description : Reset Connection Settings
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_ResetConnectionSettings(void);

/*****************************************************************************/
//  Description : Set Browser Reload Flag
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_SetBrowserReloadFlag(uint8 act_index, BOOLEAN is_reload);

/*****************************************************************************/
//  Description : Get Browser Reload Flag
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_GetBrowserReloadFlag(uint8 act_index);

/*****************************************************************************/
//  Description : Set Browser Focus Flag
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_SetBrowserFocusFlag(BOOLEAN is_focus);

/*****************************************************************************/
//  Description : Get Browser Focus Flag
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_GetBrowserFocusFlag(void);

/*****************************************************************************/
//  Description : Set Browser stop Flag
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_SetBrowserstopFlag(BOOLEAN is_stop);

/*****************************************************************************/
//  Description : Get Browser stop Flag
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_GetBrowserstopFlag(void);

/*****************************************************************************/
//  Description : Handle special scheme
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_HandleExScheme(uint8 *in_url);

#ifdef USE_EXT_DISPLAY_BUFFER
/*****************************************************************************/
//  Description : Handle special scheme
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_FreeDisplayBuf(void);
#endif

/*****************************************************************************/
//  Description : Set Auth Param
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_SetAuthParam(slim_char *in_url, slim_char *in_realm, slim_char *in_user, slim_char *in_pass);

/*****************************************************************************/
//  Description : Get Auth Param
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_GetAuthParam(slim_char **in_url, slim_char **in_realm, slim_char **in_user, slim_char **in_pass);

/*****************************************************************************/
//  Description : Free Auth Param
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_FreeAuthParam(void);

/*****************************************************************************/
// Description : Set image displaying mode.
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_SetImageDisplayMode(NFBrowserImageMode in_mode);

/*****************************************************************************/
// Description : Get image displaying mode.
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/
PUBLIC NFBrowserImageMode MMIWWW_GetImageDisplayMode(void);

/*****************************************************************************/
// Description : Convert str to Escape str(%X)
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: be sure of the range of dst
/*****************************************************************************/
PUBLIC char* MMIAPICOM_StrToEscapeStr(char *dst, const char *src);

/*****************************************************************************/
// Description : Set copy mode.
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_SetCopyMode(MMIWWW_COPY_MODE_E in_mode);

/*****************************************************************************/
// Description : Get copy mode.
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
PUBLIC MMIWWW_COPY_MODE_E MMIWWW_GetCopyMode(void);

/*****************************************************************************/
// Description : get valid file name 
// Global resource dependence : 
// Author: Jiaoyou.wu
// Note:
/*****************************************************************************/ 
PUBLIC void MMIWWW_GetValidFileName(wchar *file_name, uint32 file_len, wchar *new_file_name, uint32 new_file_len);
/*****************************************************************************/
// Description : Get User input msg
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_GetUserInputMSG(MMI_USER_INPUT_MESSAGE_T *input_msg);

/*****************************************************************************/
// Description : Set User input msg
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_SetUserInputMSG(MMI_USER_INPUT_MESSAGE_T *input_msg);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_FreeFirstURL(void);

/*****************************************************************************/
//  Description : Get Homepage
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_GetHomepageURL(char *url, int32 max_len);

/*****************************************************************************/
//  Description : Get Homepage
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_SetHomepageURL(char *url);

/*****************************************************************************/
//  Description : Wstricmp
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC int MMIWWW_Wstricmp(const wchar * string1, const wchar * string2);

/*****************************************************************************/
//  Description : Set Font Setting
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_SetFontSetting(slim_int font_set);

/*****************************************************************************/
//  Description : Set Font Setting
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_GetIsCreateNewWin(void);

/*****************************************************************************/
//  Description : Set Font Setting
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_SetIsCreateNewWin(BOOLEAN is_new_win);

#endif
