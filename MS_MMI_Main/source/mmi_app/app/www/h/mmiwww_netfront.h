/***************************************************************************
** File Name:      mmiwww_netfront.h                                            *
** Author:                                                                 *
** Date:           15/06/2009                                              *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:    This file is used to www browser netfront define                   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 06/2009        wancan.you        Create
****************************************************************************/
#ifndef  _MMIWWW_NETFRONT_H_    
#define  _MMIWWW_NETFRONT_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmiwww_internal.h"

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
#define BRSCFG_HTTP_HEADER_ACCEPT_FIELD            ("ACCEPT: text/html, application/xml;q=0.9, text/vnd.wap.wml, application/vnd.wap.xhtml+xml,application/xhtml+xml, image/png, image/jpeg, image/gif, image/x-xbitmap, */*;q=0.1\015\012")
#define BRSCFG_HTTP_HEADER_ACCEPT_LANGUAGE_FIELD   ("Accept-Language: zh-CN,zh;q=0.9,en;q=0.8\r\n")
#define BRSCFG_HTTP_HEADER_ACCEPT_CHARSET_FIELD     ("Accept-Charset: UTF-8,gb2312,gbk,ISO-8859-1,US-ASCII,Big5,ISO-10646-UCS-2;q=0.600\r\n")
#define BRSCFG_VISITHISTORY_FILENAME    ("VISITLIST.DAT")
#define BRSCFG_COOKIE_FILENAME          ("COOKIE.DAT")
#define BRSCFG_AUTH_FILENAME            ("AUTH.DAT")
#define BRSCFG_FAVORITES                ("Favorites")
//#define BRSCFG_BOOKMARK_DIRNAME         ("BOOKMARK")
//#define BRSCFG_BOOKMARK_FILENAME        ("BOOKMARK.DAT")
#define BRSCFG_CACHE_DIR                ("CACHE")
#define BRSCFG_VISITEDPAGE_THUMBNAIL_DIR    ("THUMBNAIL\\")
#define BRSCFG_WEBDATA_FILENAME     ("WEBDATA.DAT")
#define BRSCFG_CMCC_SEARCH_HEAD   ("http://s.139.com/search.do?q=")
#define BRSCFG_GOOGLE_SEARCH_HEAD   ("http://www.google.cn/search?hl=zh-CN&q=")
#define BRSCFG_BAIDU_SEARCH_HEAD    ("http://www.baidu.com/s?wd=")
#define BRSCFG_YAHOO_SEARCH_HEAD    ("http://one.cn.yahoo.com/s?p=")
#define BRSCFG_PAGEMEMO_FILENAME    ("PAGEMEMO.DAT")
#define BRSCFG_URLHISTORY_FILENAME  ("URLHISTORY.DAT")
#define BRSCFG_CMCC_UTF8_SEARCH_TAIL     ("&category=downloadable%7Cweb%7Cbrowseable&tid=2123%2C2124%2C2125%2C2126&fr=portalcustom2")
#define BRSCFG_CMCC_GB2312_SEARCH_TAIL     ("&category=downloadable%7Cweb%7Cbrowseable&tid=2123%2C2124%2C2125%2C2126&fr=portalcustom2")
#define BRSCFG_BAIDU_UTF8_SEARCH_TAIL     ("&ie=utf-8")
#define BRSCFG_BAIDU_GB2312_SEARCH_TAIL     ("&ie=gb2312")
#define BRSCFG_GOOGLE_UTF8_SEARCH_TAIL     ("&ie=utf-8&oe=utf-8")
#define BRSCFG_GOOGLE_GB2312_SEARCH_TAIL     ("&ie=gb2312&oe=gb2312")
#define BRSCFG_YAHOO_UTF8_SEARCH_TAIL     ("&ei=UTF-8")
#define BRSCFG_YAHOO_GB2312_SEARCH_TAIL     ("&ei=gb2312")
#define BRSCFG_CACHE_SIZE                   (1.5 * 1024 * 1024) /* 1.5M--2M Or it will be set to 1.5M */
#define BRSCFG_MIN_SUBTANK_SIZE             (1024 * 1024)
#define BRSCFG_MIN_SUBTANK_NUM              (0)

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Netfront Initialize
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_Netfront_Initialize(MMIWWW_INSTANCE_T *self);

/*****************************************************************************/
//  Description : Netfront Finalize
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_Netfront_Finalize(MMIWWW_INSTANCE_T *self);

/*****************************************************************************/
//  Description : Netfront Start Browser
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_int MMIWWW_Netfront_StartBrowser(MMIWWW_INSTANCE_T *self, NFBrowserStartType in_type, char *in_url);

/*****************************************************************************/
//  Description : Netfront Stop Browser
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_int MMIWWW_Netfront_StopBrowser(void);

/*****************************************************************************/
//  Description : Netfront Suspend Browser
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_int MMIWWW_Netfront_SuspendBrowser(void);

/*****************************************************************************/
//  Description : Netfront Resume Browser
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_int MMIWWW_Netfront_ResumeBrowser(void);

/*****************************************************************************/
//  Description : Netfront Leave Text Control Edit Mode
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_bool MMIWWW_Netfront_LeaveTextControlEditMode(slim_char *in_str);

/*****************************************************************************/
//  Description : handle msg event
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_bool MMIWWW_HandleMsgEvent(uint32 msg_id, DPARAM param);

/*****************************************************************************/
//  Description : handle backward forward
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_bool MMIWWW_HandleBackForward(slim_bool is_backward);

/*****************************************************************************/
//  Description : Check browser whether or not back/forward
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_bool MMIWWW_BrowserCanBackForward(slim_bool  is_backward);

/*****************************************************************************/
// Description : Reload displayed content in active browsing window
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_int MMIWWW_Netfront_ReloadPage(void);

/*****************************************************************************/
// Description : Cancel loading content in active browsing window
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_int MMIWWW_Netfront_Abort(void);

/*****************************************************************************/
// Description : Cancel loading content in active browsing window
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_bool MMIWWW_Netfront_Pagepilot(void);

/*****************************************************************************/
// Description : Cancel loading content in active browsing window
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_bool MMIWW_Netfront_Pagepilot(void);

/*****************************************************************************/
// Description : Set zoom ratio of active browsing window
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_bool MMIWWW_Netfront_PageZoom(MMIWWW_ZOOM_MODE_E  mode);

/*****************************************************************************/
// Description : Set zoom percent browsing window
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
PUBLIC slim_bool MMIWWW_Netfront_PageZoomPercent(slim_int percent);

/*****************************************************************************/
// Description : Search specified string in active browsing window
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_bool MMIWWW_Netfront_SearchText(MMI_STRING_T *str_ptr, slim_int flag);

/*****************************************************************************/
// Description : Set NF browser view mode in actvie browsing window
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_ViewMode(slim_int in_mode);

/*****************************************************************************/
// Description : Set www cookie mode
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC void MMIWWW_Netfront_SetCookieMode(slim_int in_mode);

/*****************************************************************************/
// Description : clear www browser cookie
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_ClearCookie(void);

/*****************************************************************************/
// Description : Set www cookie mode
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC void MMIWWW_Netfront_SetCacheMode(slim_int in_mode);

/*****************************************************************************/
// Description : clear www browser cache
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_ClearCache(void);

/*****************************************************************************/
// Description : Set www wether use disk cache
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC void MMIWWW_Netfront_SetUseDiskCacheMode(slim_int in_use);

/*****************************************************************************/
// Generates visit history instance
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/
PUBLIC slim_bool MMIWWW_Netfront_NewVisitHistory(void);

/*****************************************************************************/
// Deletes visit history instance
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_DeleteVisitHistory(void);

/*****************************************************************************/
// Gets number of visit history entries
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/
PUBLIC slim_int MMIWWW_Netfront_GetVisitHistoryNumber(void);

/*****************************************************************************/
// Gets URL of specified visit history entry
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/
 PUBLIC slim_bool MMIWWW_Netfront_GetVisitHistoryUrl( slim_int in_index, MMI_STRING_T *out_url );

/*****************************************************************************/
// Gets title of specified visit history entry
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/
 PUBLIC slim_bool MMIWWW_Netfront_GetVisitHistoryTitle( slim_int in_index, MMI_STRING_T *out_title );

/*****************************************************************************/
// Deletes visit history entry specified by in_index
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_RemoveVisitHistory( slim_int in_index );

/*****************************************************************************/
// Deletes all the visit history entries
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_RemoveAllVisitHistory(void );

/*****************************************************************************/
// Description : Set Charset
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/ 
PUBLIC void MMIWWW_Netfront_ChangeToEncoding(int32 charset);

/*****************************************************************************/
// Gets visit history time
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC slim_bool MMIWWW_Netfront_GetHistoryTime(slim_int in_index, slim_time *out_time);

/*****************************************************************************/
// Description : Generates page memo instance
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC slim_bool MMIWWW_Netfront_NewPageMemo(void);

/*****************************************************************************/
// Description : Deletes page memo instance
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC void MMIWWW_Netfront_DeletePageMemo(void);

/*****************************************************************************/
// Description : Gets number of page memo entries
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC slim_int MMIWWW_Netfront_GetPageMemoNumber(void);

/*****************************************************************************/
// Description : Gets URL of specified page memo entry
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC slim_bool MMIWWW_Netfront_GetPageMemoUrl(slim_int in_index, MMI_STRING_T *out_url);

/*****************************************************************************/
// Description : Gets title of specified page memo entry
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC slim_bool MMIWWW_Netfront_GetPageMemoTitle(slim_int in_index, MMI_STRING_T *out_title);

/*****************************************************************************/
// Description : Gets title of specified page memo entry
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC slim_int MMIWWW_Netfront_GetPageMemoSortType(void);

/*****************************************************************************/
// Description : Sets sort type of page memo instance
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC slim_bool MMIWWW_Netfront_SetPageMemoSortType(slim_int in_sort_type);

/*****************************************************************************/
// Description : Deletes page memo entry specified by in_index
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC slim_bool MMIWWW_Netfront_RemovePageMemo(slim_int in_index);

/*****************************************************************************/
// Description : Deletes all entries of page memo
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC slim_bool MMIWWW_Netfront_RemoveAllPageMemo(void);

/*****************************************************************************/
// Description : Saves current web page to page memo
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC slim_bool MMIWWW_Netfront_AddPageMemo(MMI_STRING_T *in_title_ptr);

/*****************************************************************************/
// Description : Loads the page memo specified by in_index
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC slim_int MMIWWW_Netfront_LoadPageMemo(slim_int in_index);

/*****************************************************************************/
// Description : Generates input URL history instance
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC slim_bool MMIWWW_Netfront_NewInputURLHistory(void);

/*****************************************************************************/
// Description : Generates input URL history instance
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC void MMIWWW_Netfront_DeleteInputURLHistory(void);

/*****************************************************************************/
// Description : Gets number of  input URL history entries
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC slim_int MMIWWW_Netfront_GetInputURLHistoryNumber(void);

/*****************************************************************************/
// Description : Gets URL of specified input URL history entry
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC slim_bool MMIWWW_Netfront_GetInputURLHistoryUrl(slim_int in_index, MMI_STRING_T *out_url);

/*****************************************************************************/
// Description : Deletes input URL history entry specified by in_index
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC slim_bool MMIWWW_Netfront_RemoveInputURLHistory(slim_int in_index);

/*****************************************************************************/
// Description : Deletes all the input URL history entries
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC slim_bool MMIWWW_Netfront_RemoveAllInputURLHistory(void);

/*****************************************************************************/
// Description : Saves new input URL
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC slim_bool MMIWWW_Netfront_AddInputURLHistory(MMI_STRING_T *add_url_ptr);

/*****************************************************************************/
// Description : Access network by history
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC void MMIWWW_Netfront_History_GotoURL(slim_int in_index);

/*****************************************************************************/
// Description : Get webpage title
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC slim_bool MMIWWW_Netfront_GetPageTitle(MMI_STRING_T *out_title);

/*****************************************************************************/
// Description : Get webpage title
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/ 
PUBLIC slim_bool MMIWWW_Netfront_GetPageTitleByIndex(int32 index, MMI_STRING_T *out_title);

/*****************************************************************************/
// Description : Change size of all browsing windows
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_int MMIWWW_Netfront_Resize(void);

/*****************************************************************************/
// Description : Gets number of entries in certificate database by certificate type
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC slim_int MMIWWW_Netfront_GetX509CertNumber(slim_int in_type);

/*****************************************************************************/
// Description : Gets name of issuer/subject
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC slim_bool MMIWWW_Netfront_GetX509CertName(slim_int in_type, slim_int in_index, MMI_STRING_T *out_name);

/*****************************************************************************/
// Description : Gets certificate data
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC NFBrowserCertInfo *MMIWWW_Netfront_GetX509CertData(slim_int in_type, slim_int in_index);

/*****************************************************************************/
// Description : Releases certificate data information
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC void MMIWWW_Netfront_DeleteX509CertData(NFBrowserCertInfo *CertInfo_ptr);

/*****************************************************************************/
// Description : Send HTTP Get request to specified URL.
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_int MMIWWW_Netfront_SetLocation(slim_char *in_url);

/*****************************************************************************/
// Description : Get number of browsing windows.
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_int MMIWWW_Netfront_GetWinNum(void);

/*****************************************************************************/
// Description : Delete a browsing window.
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_bool MMIWWW_Netfront_DelWin(slim_int  in_index);

/*****************************************************************************/
// Description : Create a new browsing window.
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_int MMIWWW_Netfront_NewWin(slim_int  *out_index);

/*****************************************************************************/
// Description : Get ID of active browsing window.
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_int MMIWWW_Netfront_GetActWinIndex(void);

/*****************************************************************************/
// Description : Activate a specified ID browsing window.
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_bool MMIWWW_Netfront_ActWinByIndex(slim_int  in_index);

/*****************************************************************************/
// Description : Get current background sound information.
// Global resource dependence :
// Author: jiaoyou.wu
// Note: 
/*****************************************************************************/ 
PUBLIC slim_bool MMIWWW_Netfront_GetBGSoundInfo(slim_int in_index, slim_int in_snd_id, NFBrowserBGSoundInfo *out_info);

/*****************************************************************************/
// Description : Get current page size.
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_int MMIWWW_Netfront_GetPageSize(void);

/*****************************************************************************/
// Description : Get page MIME type.
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_bool MMIWWW_Netfront_GetPageMIMEType(MMI_STRING_T  *out_mimetype);

/*****************************************************************************/
// Description : Get webpage address.
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_bool MMIWWW_Netfront_GetPageAddress(MMI_STRING_T  *out_address);

/*****************************************************************************/
// Description : Get webpage address.
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
PUBLIC slim_bool MMIWWW_Netfront_GetPageAddressByIndex(int32 index, MMI_STRING_T  *out_address);

/*****************************************************************************/
// Description : Set NF browser to save form data of webpages
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_SetWebData(slim_bool in_save);

/*****************************************************************************/
// Description : Set NF browser to save form data of webpages
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_ClearWebData(void);

/*****************************************************************************/
// Description : Set script mode of NF browser engine in actvie browsing window.
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_SetScriptMode(slim_int in_mode);

/*****************************************************************************/
// Description : Set NF browser engine to display animation image in actvie browsing window.
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_SetDisplayAnimateImage(slim_bool in_display);

/*****************************************************************************/
// Description : Set image displaying mode of NF browser engine in actvie browsing window.
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_SetImageDisplayMode(slim_int in_mode);

/*****************************************************************************/
// Description : Set NF browser engine to display tables in actvie browsing window.
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_SetDisplayTable(slim_bool in_display);

/*****************************************************************************/
// Description : Enable background soud of webpages in actvie browsing window.
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_SetEnableBGSound(slim_bool in_enable);

/*****************************************************************************/
// Description : Get image number in current page.
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_int MMIWWW_Netfront_GetImageNumber(void);

/*****************************************************************************/
// Description : Set browser image selection mode and select one image before
//               get the image information.
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_bool MMIWWW_Netfront_SetImageSelectMode(slim_int  is_select);

/*****************************************************************************/
// Description : Determine image saving mode.
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_bool MMIWWW_Netfront_IsImageSelectMode(void);

/*****************************************************************************/
// Description : Get size information of the selected image in the current page.
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_int MMIWWW_Netfront_GetSelectedImageSize(void);

/*****************************************************************************/
// Description : Get selected image name.
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_bool MMIWWW_Netfront_GetImageName(MMI_STRING_T  *out_name);

/*****************************************************************************/
// Description : Get address information of the selected image.
// Global resource dependence :
// Author: Bill.Ji
// Note: 需调用者对返回数据进行释放
/*****************************************************************************/
PUBLIC uint8 *MMIWWW_Netfront_GetImageAddress(void);

/*****************************************************************************/
// Description : Get selected image mimetype.
// Global resource dependence :
// Author: Bill.Ji
// Note: 需调用者对返回数据进行释放
/*****************************************************************************/
PUBLIC slim_bool MMIWWW_Netfront_GetImageContentType(uint8  **content_type_ptr);

/*****************************************************************************/
// Description : Get selected image data.
// Global resource dependence :
// Author: Bill.Ji
// Note: 需调用者对返回数据进行释放
/*****************************************************************************/
PUBLIC uint32 MMIWWW_Netfront_GetImageData(uint8  **out_ptr);

/*****************************************************************************/
// Description : Sets the full path of the selected file to NF browser engine.
// Global resource dependence :
// Author: wancan.you
// Note: 
/*****************************************************************************/ 
PUBLIC slim_bool MMIWWW_Netfront_SetFileSelectFullPath(slim_int in_select, slim_char *in_path);

/*****************************************************************************/
// Description : Sets the full path of the selected file display to NF browser engine.
// Global resource dependence :
// Author: wancan.you
// Note: 
/*****************************************************************************/ 
PUBLIC slim_bool MMIWWW_Netfront_SetFileSelcetDisplayName(slim_int in_select, slim_char *in_name);

/*****************************************************************************/
// Description : Wakeup Netfront Browser
// Global resource dependence :
// Author: wancan.you
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_Wakeup(slim_int in_slef);

/*****************************************************************************/
// Description : Reply Request Auth
// Global resource dependence :
// Author: wancan.you
// Note: 
/*****************************************************************************/
PUBLIC slim_int MMIWWW_ReplyRequestAuth(slim_bool in_confirmed, MMI_STRING_T *in_user, MMI_STRING_T *in_pass);

/*****************************************************************************/
// Description : Set Pointer Mode
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_SetPointerMode(slim_int pointer_mode);

/*****************************************************************************/
// Description : Enter copy mode
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_EnterCopyMode(void);

/*****************************************************************************/
// Description : Leave copy mode
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_LeaveCopyMode(void);

/*****************************************************************************/
// Description : PageUp
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_PageUp(void);

/*****************************************************************************/
// Description : PageDown
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_PageDown(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif


