/*****************************************************************************
** File Name:      mmibrowser_export.h                                          *
** Author:          li.li                                                    *
** Date:           23/06/2009                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the browser api             *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2011.11        fen.xie         modify for browser manager                 *
**                                                                           *
*****************************************************************************/
#ifndef MMIBROWSER_API_H
#define MMIBROWSER_API_H

#ifdef BROWSER_SUPPORT
/*****************************************************************************
NOTES:
1\browser manager 负责与外部应用交互
2\各browser app与browser manager交互
3\browser manager、dorado、www等命名规则说明:以修改最小为基准
  1) 保留了原API的命名,故本文件仍使用MMIAPIBROWSER_***(MMIBROWSER_***),
  以后新增API也用MMIAPIBROWSER_***;
  2) browser manager的命名规则为MMIBM_***;
  3) 各browser内部原冲突的API使用MMIDORADO_***，MMIWWW_***等新命名方式，
  以后新增的也请使用新的命名方式；原dorado使用MMIBROWSER_***命名方式，继续延用
*****************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mn_type.h"
#ifdef BROWSER_SUPPORT_NETFRONT	
#include "mmidl_export.h"
#endif
#include "mmifmm_export.h"

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
#ifdef MMI_BROWSER_MINI_SUPPORT
#define MMIBROWSER_MAX_URL_LEN                  128  
#else
#define MMIBROWSER_MAX_URL_LEN                  512//255 
#endif

#define MMIBROWSER_BOOKMARK_MAX_TITLE_LENGTH 16
#define MMIBROWSER_BOOKMARK_MAX_URL_LENGTH MMIBROWSER_MAX_URL_LEN

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef enum _MMIBROWSER_ERROR
{
    MMIBROWSER_SUCC  = 0,
    MMIBROWSER_FAIL,
    MMIBROWSER_ERROR_INPUT,
    MMIBROWSER_ERROR_MAX
}MMIBROWSER_ERROR_E;

//MMICMSWAP_GPRS_STATE_E is useless
typedef enum
{
    MMICMSWAP_GPRS_DEACTIVE_OK,	//DEACTIVE完成
	MMICMSWAP_GPRS_ACTIVE_OK,		//ACTIVE完成
	MMICMSWAP_GPRS_ACEIVE_FAILED,
	MMICMSWAP_GPRS_ACTIVING,
	MMICMSWAP_GPRS_DEACTIVEING		//DEACTIVE进行中
}MMICMSWAP_GPRS_STATE_E;//GPRS的状态

typedef struct
{
	uint8   user_agent[MMIBROWSER_MAX_URL_LEN];	
	uint8   uaprofile[MMIBROWSER_MAX_URL_LEN];
}MMIBROWSER_UA_T;//ua的信息

typedef enum
{
    MMIBROWSER_MIME_TYPE_NONE,  //invalid
    MMIBROWSER_MIME_TYPE_JAR,
    MMIBROWSER_MIME_TYPE_JAD,
    MMIBROWSER_MIME_TYPE_SDP,
    MMIBROWSER_MIME_TYPE_DD,
#ifdef DRM_SUPPORT    
    MMIBROWSER_MIME_TYPE_DM,//DRM message
    MMIBROWSER_MIME_TYPE_DCF,//DRM content
    MMIBROWSER_MIME_TYPE_DR,//DRM rights
    MMIBROWSER_MIME_TYPE_DRC,//DRM rights
#endif    
#ifdef QBTHEME_SUPPORT
	MMIBROWSER_MIME_TYPE_QBT,
#endif
#ifdef HOME_LOCATION_INQUIRE_SUPPORT_TCARD
	MMIBROWSER_MIME_TYPE_PNL,
#endif
#ifdef DATA_ACCOUNT_IN_TCARD_SUPPORT
	MMIBROWSER_MIME_TYPE_NTAC,  //network account
#endif
    MMIBROWSER_MIME_TYPE_TXT,
    MMIBROWSER_MIME_TYPE_IMAGE,
    MMIBROWSER_MIME_TYPE_AUDIO,
    MMIBROWSER_MIME_TYPE_VEDIO,

    MMIBROWSER_MIME_TYPE_MAX    //max
}MMIBROWSER_MIME_TYPE_E;

#ifndef BROWSER_SUPPORT_NETFRONT//for兼容(mmidl和dorado自己的download模块)
typedef struct 
{
    wchar                   file_name[MMIFILE_FULL_PATH_MAX_LEN + 1];
    MMIBROWSER_MIME_TYPE_E  mime_type;
    char*                   url_ptr;
    void *                  user_data;
    BOOLEAN                 download_result;
//#ifdef BROWSER_SUPPORT_NETFRONT	
//    MMIDL_DOWNLOAD_RESULT_E result_code;//added for cr240847    
//#endif    
}MMIBROWSER_MIME_CALLBACK_PARAM_T;/* Urge: Modify MMIDL_MIME_CALLBACK_PARAM_T simultaneously */
#else
typedef  MMIDL_MIME_CALLBACK_PARAM_T MMIBROWSER_MIME_CALLBACK_PARAM_T;
#endif

typedef void (*MMIBROWSER_MIME_CALLBACK)(MMIBROWSER_MIME_CALLBACK_PARAM_T* param_ptr);
typedef struct 
{
    MMIBROWSER_MIME_TYPE_E      mime_type;
    MMIBROWSER_MIME_CALLBACK    call_back;
}MMIBROWSER_MIME_DOWNLOAD_PARAM_T;


typedef enum
{
    MMIBROWSER_ACCESS_MAINMENU,
    MMIBROWSER_ACCESS_URL,
    MMIBROWSER_HOMEPAGE,
    MMIBROWSER_ADD_BOOKMARK,
    MMIBROWSER_ACCESS_FLOWRATE,//useless,please call MMIAPICL_OpenGprsFlowRateWin
    MMIBROWSER_ACCESS_DOWNLOAD_BY_URL,
#ifdef BROWSER_SEARCH_SUPPORT
    MMIBROWSER_ACCESS_SEARCH,
#endif    
    MMIBROWSER_ENTRY_TYPE_MAX
}MMIWWW_ENTRY_TYPE_E;

typedef enum
{
#ifdef BROWSER_SUPPORT_DORADO    
    MMIBROWSER_TYPE_DORADO,
#endif    
#ifdef OPERA_MINI_SUPPORT
    MMIBROWSER_TYPE_OPERA_MINI,
#endif    
#ifdef BROWSER_SUPPORT_NETFRONT
    MMIBROWSER_TYPE_NF,
#endif    
#ifdef BROWSER_SUPPORT_WEBKIT
    MMIBROWSER_TYPE_WK,
#endif  
    MMIBROWSER_TYPE_MAX,
}MMIBROWSER_BROWSER_TYPE_E;//the type of browser

typedef struct
{
    MMIBROWSER_BROWSER_TYPE_E browser_type;	//dorado,opear mini,NF......
    BOOLEAN is_browser_type_customized;//used customized browser_type;TRUE, use browser_type
    MMIWWW_ENTRY_TYPE_E type;	//mainmenu, homepage, url
    BOOLEAN is_use_wifi;//priority:is_use_wifi > dual_sys
    MN_DUAL_SYS_E dual_sys;//sim:dual_sys
    char* url_ptr;
    char *url_title_ptr;//only use for MMIBROWSER_ADD_BOOKMARK
    char* user_agent_ptr;
    BOOLEAN is_connection_customized;//used customized connection_index(only netfront now)
    uint8 connection_index;//(only netfront now)
#ifdef  CMCC_UI_STYLE
    BOOLEAN is_alert;
#endif
}MMIBROWSER_ENTRY_PARAM;

//browser process function
typedef struct
{
    void    (*InitModule)(void);
    void    (*InitApp)(void);
    void    (*UninitApp)(void);
    MMIBROWSER_ERROR_E    (*Entry)(const MMIBROWSER_ENTRY_PARAM *);
    MMIBROWSER_ERROR_E    (*Exit)(void);
    BOOLEAN    (*HandleAppMsg)(MMI_MESSAGE_ID_E, void*);
    char*    (*GetBrowserVersion)(void);
    void    (*SimPlugNotify)(void);
    void    (*SDCardPlugOutNotify)(void);
    void    (*StorageFormatNotify)(void);
    void    (*SetNetAccountBySim)(MN_DUAL_SYS_E, uint8);
    uint8    (*GetNetAccountBySim)(MN_DUAL_SYS_E);
    BOOLEAN    (*IsRunnning)(void);
    BOOLEAN    (*IsActive)(void);
    void    (*ResetFactorySetting)(void);
    void    (*CleanUserInfo)(void);
    BOOLEAN    (*RegMimeHandleFunc)(MMIBROWSER_MIME_DOWNLOAD_PARAM_T*);
    void    (*CleanCache)(uint8*, uint16);
    void    (*DeleteCompletedDownloadTask)(uint32);
    void    (*CheckTimerDisable)(void);
    //begin add new
    void    (*SetDefaultBookmark)(void);
    
    //end:add new
    //begin:for AT TEST
    BOOLEAN (*Atest_BROWSERInsertURL)(wchar*,uint32);
    BOOLEAN (*Atest_BROWSERBookmarkTitleInput)(wchar*,uint16);
    BOOLEAN (*Atest_BROWSERBookmarkURLInput)(wchar*,uint16);
    //end:for AT TEST
} MMIBROWSER_PROCESS_T;

typedef struct
{
    MMIBROWSER_PROCESS_T*   process_func[MMIBROWSER_TYPE_MAX];
    MMIBROWSER_BROWSER_TYPE_E   cur_browser_type;
}MMIBROWSER_MANAGER_INFO_T;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : init browser
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBROWSER_InitApp(void);

/*****************************************************************************/
//  Description : uninit browser
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: useless
/*****************************************************************************/
PUBLIC void MMIAPIBROWSER_UninitApp(void);

/*****************************************************************************/
//  Description : check if browser is running
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBROWSER_IsRunning(void);

/*****************************************************************************/
//  Description : check if browser is the focused win
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBROWSER_IsActive(void);

/*****************************************************************************/
//  Description : the entry of browser 
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_ERROR_E MMIAPIBROWSER_Entry(const MMIBROWSER_ENTRY_PARAM * entry_param_ptr);

/*****************************************************************************/
//  Description : Clean cache
//  Global resource dependence : 
//  Author: minghu.mao
//  Note: 清除浏览器和http的cache
/*****************************************************************************/
PUBLIC void MMIAPIBROWSER_CleanCache(uint8 * url_ptr, uint16 url_len);

/*****************************************************************************/
//  Description : close browser 
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_ERROR_E MMIAPIBROWSER_Close(void);

/*****************************************************************************/
//  Description : init browser module
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBROWSER_InitModule(void);

/*****************************************************************************/
//  Description : for reset to factory setting
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBROWSER_ResetFactorySetting(void);

/*****************************************************************************/
//  Description : handle app message
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBROWSER_HandleAppMsg(MMI_MESSAGE_ID_E msg_id,void* param);

/*****************************************************************************/
//  Description : register the callback function of mime type
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBROWSER_RegMimeFunc(MMIBROWSER_MIME_DOWNLOAD_PARAM_T* param_ptr);

/*****************************************************************************/
//  Description : Clean Usert Data Info
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBROWSER_CleanUserInfo(void);

/*****************************************************************************/
//  Description : Get browser Version of one browser
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC char* MMIAPIBROWSER_GetVersion(MMIBROWSER_BROWSER_TYPE_E browser_type);

/*****************************************************************************/
//  Description : get net setting index for browser(priority :default > MMIBROWSER_BROWSER_TYPE_E )
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIAPIBROWSER_GetNetSettingIndex(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : Delete completed download task of all browsers
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: only use for netfront
/*****************************************************************************/
PUBLIC void MMIAPIBROWSER_DeleteCompletedDownloadTask(uint32 task_index);

/*****************************************************************************/
//  Description : Check availability of Timer(only valid for Netfront)
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: only use for netfront
/*****************************************************************************/
PUBLIC void MMIAPIBROWSER_CheckTimerDisable(void);

/*****************************************************************************/
//  Description : set net setting index for all browser
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBROWSER_SetNetSettingIndex(MN_DUAL_SYS_E dual_sys, uint8 index);

/*****************************************************************************/
//  Description : handle plug out sd card
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBROWSER_SDCardPlugOut(void);

/*****************************************************************************/
//  Description : handle browser when format the storage 
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBROWSER_HandleStorageFormat(void);

/*****************************************************************************/
//  Description : open the browser type setting window
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBROWSER_OpenSetBrowserWin(void);

/*****************************************************************************/
//  Description : Handle special scheme(wati,rtsp......)
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBROWSER_HandleExScheme(uint8 *in_url);

/*****************************************************************************/
//  Description : check if the customized browser is running
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBROWSER_IsRunningEx(MMIBROWSER_BROWSER_TYPE_E browser_type);

/*****************************************************************************/
//  Description : check if the customized browser is the focused win
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBROWSER_IsActiveEx(MMIBROWSER_BROWSER_TYPE_E browser_type);

/*****************************************************************************/
//  Description : Delete completed download task of the customized browser
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBROWSER_DelCompletedDlTaskEx(uint32 task_index, MMIBROWSER_BROWSER_TYPE_E browser_type);

/*****************************************************************************/
//  Description : set net setting index fo customized browser
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBROWSER_SetNetSettingIndexEx(MN_DUAL_SYS_E dual_sys, uint8 index, MMIBROWSER_BROWSER_TYPE_E browser_type);

/*****************************************************************************/
//  Description : handle download callback
//  Global resource dependence : 
//  Author: fen.xie
//  Note: only used by dorado and opera
/*****************************************************************************/
PUBLIC void MMIAPIBROWSER_MimeDownlaodCallBack(MMIBROWSER_MIME_CALLBACK_PARAM_T* param_ptr);

#if 0
/*****************************************************************************/
//  Description : set net setting index fo customized browser
//  Global resource dependence : 
//  Author: dave.ruan
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBROWSER_SetDefaultBookmark(MMIBROWSER_BROWSER_TYPE_E browser_type);
#endif

/*****************************************************************************/
// Description : get the url info by the url file buffer
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC uint8* MMIAPIBROWSER_GetUrlByFile(uint8* buf_ptr, //in
                    uint32 size,//in
                    uint32 *url_len_ptr
                    );

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                                    Start of Atest  Function  Declare                                                              */
/* Description:  Hanlde at cmd for browser auto test cases                                                     */
/* Author: Shuyan.hu                                                                                                          */
/* Date: 30/12/2010                                                                                                           */ 
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : handle url input from at+sytin cmd
//  Global resource dependence : none
//  Author: shuyan.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN Atest_BROWSERInsertURL(
                wchar * url_ptr,                 //input url 
                uint32 len                          //input url length 
                );

/*****************************************************************************/
//  Description : input bookmark at cmd into bookmark title edit box
//  Global resource dependence : 
//  Author: shuyan.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN Atest_BROWSERBookmarkTitleInput(
                wchar * wstr_ptr,             //point to the bookmark title information buffer    
                uint16  ilen                      //length of the information buffer
                );
/*****************************************************************************/
//  Description : input bookmark at cmd into bookmark URL edit box
//  Global resource dependence : 
//  Author: shuyan.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN Atest_BROWSERBookmarkURLInput(
                wchar * wstr_ptr,             //point to the bookmakr url buffer
                uint16  ilen                      //length of the information buffer
                );

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                                    End of Atest  Function  Declare                                                                  */
/* Description:  Hanlde at cmd for browser auto test cases                                                     */
/* Author: Shuyan.hu                                                                                                          */
/* Date: 30/12/2010                                                                                                           */ 
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

  
/**---------------------------------------------------------------------------*
 **                         Macro Declaration    				   *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  如下宏定义用以实现对外接口更名前后的兼容
/*****************************************************************************/
#ifdef MMI_FUNC_COMPATIBLE_SUPPORT

#define 	MMIBROWSER_InitApp							MMIAPIBROWSER_InitApp

#define 	MMIBROWSER_UninitApp						MMIAPIBROWSER_UninitApp

#define 	MMIBROWSER_IsRunning						MMIAPIBROWSER_IsRunning

#define 	MMIBROWSER_IsActive						MMIAPIBROWSER_IsActive

#define 	MMIBROWSER_Entry							MMIAPIBROWSER_Entry

#define 	MMIBROWSER_Close							MMIAPIBROWSER_Close

#define 	MMIBROWSER_InitModule						MMIAPIBROWSER_InitModule

#define 	MMIBROWSER_ResetFactorySetting				MMIAPIBROWSER_ResetFactorySetting

#define 	MMIBROWSER_HandleAppMsg					MMIAPIBROWSER_HandleAppMsg

#define 	MMIBROWSER_CleanUserInfo					MMIAPIBROWSER_CleanUserInfo

#define 	MMIBROWSER_GetVersion						MMIAPIBROWSER_GetVersion

#define 	MMIBROWSER_GetNetSettingIndex				MMIAPIBROWSER_GetNetSettingIndex

#define 	MMIBROWSER_DeleteCompletedDownloadTask		MMIAPIBROWSER_DeleteCompletedDownloadTask

#endif //MMI_FUNC_COMPATIBLE_SUPPORT

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif//BROWSER_SUPPORT

#endif
