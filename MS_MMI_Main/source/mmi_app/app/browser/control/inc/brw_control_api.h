/*****************************************************************
 **  File Name: 	brw_control_api.h  							 *
 **  Author:		            								 *
 **  Date:		         										 *
 **  Description:definition of export structure and api of       *
     browser engine                                              *
******************************************************************
******************************************************************
**  Edit History											     *
**---------------------------------------------------------------*
 **  DATE			NAME			DESCRIPTION				     *
 **  20/12/2008		yuzhi.chen      Create. 					 *
*****************************************************************/
#ifndef BRW_CONTROL_API_H
#define BRW_CONTROL_API_H

#include "mn_type.h"
#include "brw_export.h"
#include "odm_api.h"
#include "mime_type.h"
#include "mmiconnection_export.h"

#define BRW_INSTANCE    uint32
#define BRW_INVALID_INSTANCE    0

//#define BRW_WTAI_MAX_PHONE_NUM_LEN  50
//#define BRW_WTAI_MAX_DTMF_LEN   BRW_WTAI_MAX_PHONE_NUM_LEN
//#define BRW_WTAI_MAX_PB_NAME_LEN    20
//#define BRW_WTAI_MAX_EMAIL_NAME_LEN   255
//#define BRW_WTAI_MAX_SMS_CONTENT_LEN   160

//according to MMIDORADO_MEMORY_SIZE

/*每个网页内容的最大值！
    for customer to open www.sina.com.cn page,<= MMIDORADO_MEMORY_GRANULARITY*/
#define BRW_DEFAULT_MAX_PAGE_CONTENT_LEN    (2*512*1024)

//每个网页内子媒体的最大值！可根据需要适当增减
#if defined(PLATFORM_SC8800H) || defined(PLATFORM_SC8800G)
#define BRW_DEFAULT_MAX_HTTP_CONTENT_LEN    (800*1024)

#ifdef MEMORY_CUT
#define BRW_NAV_MAX_CACHE_REQ_NUM	3
#else/*MEMORY_CUT*/
#define BRW_NAV_MAX_CACHE_REQ_NUM	20
#endif/*MEMORY_CUT*/

#elif defined PLATFORM_SC6800H//MS00204792
#ifdef NANDBOOT_SUPPORT
#define BRW_NAV_MAX_CACHE_REQ_NUM	6
#define BRW_DEFAULT_MAX_HTTP_CONTENT_LEN    (600*1024)
#else/*NANDBOOT_SUPPORT*/
#define BRW_NAV_MAX_CACHE_REQ_NUM	3
#define BRW_DEFAULT_MAX_HTTP_CONTENT_LEN    (100*1024)
#endif/*NANDBOOT_SUPPORT*/

#elif defined PLATFORM_SC6530
#ifdef NANDBOOT_SUPPORT
#define BRW_NAV_MAX_CACHE_REQ_NUM	6
#define BRW_DEFAULT_MAX_HTTP_CONTENT_LEN    (600*1024)
#else/*NANDBOOT_SUPPORT*/
#define BRW_NAV_MAX_CACHE_REQ_NUM	3
#define BRW_DEFAULT_MAX_HTTP_CONTENT_LEN    (300*1024)
#endif/*NANDBOOT_SUPPORT*/

#elif defined PLATFORM_SC7702
#ifdef MEMORY_CUT
#define BRW_DEFAULT_MAX_HTTP_CONTENT_LEN    (600*1024)
#else
#define BRW_DEFAULT_MAX_HTTP_CONTENT_LEN    (800*1024)
#endif
#define BRW_NAV_MAX_CACHE_REQ_NUM	10//3#define BRW_NAV_MAX_CACHE_REQ_NUM	10//3

#else
#define BRW_DEFAULT_MAX_HTTP_CONTENT_LEN    (300*1024)
#define BRW_NAV_MAX_CACHE_REQ_NUM	10//3
#endif

#ifdef CMCC_UI_STYLE
#ifdef BRW_NAV_MAX_CACHE_REQ_NUM
#undef BRW_NAV_MAX_CACHE_REQ_NUM
#endif
#define BRW_NAV_MAX_CACHE_REQ_NUM	20

#ifdef BRW_DEFAULT_MAX_HTTP_CONTENT_LEN
#undef BRW_DEFAULT_MAX_HTTP_CONTENT_LEN
#endif
#define BRW_DEFAULT_MAX_HTTP_CONTENT_LEN    (800*1024)
#endif
#define BRW_MAX_VISIT_HIS_ITEM_NUM    50//页面浏览记录的最大条数
#define BRW_MAX_URL_LEN         512//url最大长度
#define BRW_MAX_HIS_NAME_LEN    20

#define BRW_MAX_INPUT_HIS_ITEM_NUM    50//输入历史最大条目

typedef enum 
{
    BRW_MSG_INIT_CNF=0,
    BRW_MSG_EXIT_CNF,
    BRW_MSG_TYPE_ENTER_FILENM_REQ,	//http download请求UI输入文件名
    BRW_MSG_TYPE_DL_CONFIRM_REQ,	//http download请求UI确认
    BRW_MSG_TYPE_AUTH_REQ,			//鉴权
    BRW_MSG_TYPE_CHANGE_URL_IND,	//URL改变
    BRW_MSG_TYPE_NEW_PAGE_REQ_IND,	//新页面到达
    BRW_MSG_TYPE_UPDATE_CONTENT_IND,//reserved
    BRW_MSG_TYPE_PROGRESS_IND,		//进度报告
    BRW_MSG_TYPE_ERROR_IND,			//错误报告
    BRW_MSG_TYPE_DOWNLOAD_OK_IND,   //下载成功
    BRW_MSG_TYPE_DOWNLOAD_ERR_IND,  //下载失败
    BRW_MSG_TYPE_PAGE_GET_IND,		//获取到page的get cnf后利用此消息告知APP子媒体个数
    BRW_MSG_TYPE_MEDIA_GET_ONE_IND,	//获取到一个子媒体时利用此消息告知APP
    BRW_MSG_TYPE_DISPLAY_READY_IND, //显示
    BRW_MSG_TYPE_REPAINT_READY_IND, //Repaint页面
    BRW_MSG_TYPE_SSL_CERT_IND,		//安全链接证书确认
    BRW_MSG_TYPE_PAGE_STOP_IND,		//页面下载被中途停止
    BRW_MSG_TYPE_OMA_DL_CONFIRM_REQ,//OMA download请求UI确认
    BRW_MSG_TYPE_LAYOUTCTRL_DESTROY_REQ,   //销毁网页
    BRW_MSG_TYPE_PDP_ACTIVE_CNF, 
    BRW_MSG_TYPE_HTTP_REFRESH_IND,    //for http header refresh

    //add new here,MSG_BRW_START_ID need add one also
    BRW_MSG_TYPE_OPEN_RTSP_IND,
    BRW_MSG_TYPE_MAX
}BRW_EXPORT_MSG_E;

typedef enum
{
    BRW_PROTOCOL_HTTP=0,
    BRW_PROTOCOL_WAP//wtp/wsp
}BRW_PROTOCOL_TYPE_E;

typedef enum  //browser engine status
{
    BRW_STATE_IDLE=0,
    BRW_STATE_PAGE_REQUESTING,
    BRW_STATE_MEDIA_REQUESTING,
    BRW_STATE_DOWNLOADING,
    BRW_STATE_ALL_FINISHED,
    BRW_STATE_MAX		
}BRW_REQ_STATE_E;

typedef enum  //error type
{
    BRW_ERR_NO_ERROR=0,
    BRW_ERR_NO_MEM,
    BRW_ERR_HTTP_INIT_FAILED,
    BRW_ERR_GPRS_LINK_FAILED,
    BRW_ERR_GPRS_LINK_FDN_ONLY,
    BRW_ERR_CONNECT_FAILED,
    BRW_ERR_CONNECT_TIMEOUT,
    BRW_ERR_SERVER_RESET,
    BRW_ERR_GPRS_LINK_DISCONNECT,
    BRW_ERR_PAGE_UNSUPPORT,
    BRW_ERR_PAGE_ERR,
    BRW_ERR_PAGE_TOO_BIG,//超过了能处理的网页的大小
    BRW_ERR_HTTP_RTN,
    BRW_ERR_GWT_URL_FAIL,//URL google转码失败（e.g.url为https）
    BRW_ERR_URL_INVALID,//url is invalid	
#ifdef DATA_ROAMING_SUPPORT
    BRW_ERR_DATA_ROAMING_DISABLED,//data roaming is disabled(added by feng.xiao)
#endif
    BRW_ERR_INVALID_DD,//invalid despritor
    BRW_ERR_UNKNOWN
}BRW_ERROR_E;

typedef enum 
{
    BRW_NULL_PROFILE_TYPE=0x00000000,
    BRW_CACHE_PROFILE=0x00000001,
    BRW_COOKIE_PROFILE=0x00000002,
    BRW_IMG_DL_PROFILE=0x00000004,
    BRW_BGSOUND_DL_PROFILE=0x00000008,
    BRW_USER_AGENT_PROFILE=0x00000010,
    BRW_UAPROF_PROFILE=0x00000020,
    BRW_PROXY_PROFILE=0x00000040,
    BRW_SCREEN_PROFILE=0x00000080,
    BRW_GWT_PROFILE=0x00000100,
    BRW_FONT_PROFILE=0x00000200,
}BRW_PROFILE_TYPE_E;

//下载过程类型
typedef enum
{
    BRW_DL_TYPE_HTTP_CONTENT = 0,
    BRW_DL_TYPE_OMA_DD,
    BRW_DL_TYPE_OMA_CONTENT,
    BRW_DL_TYPE_LOCAL_CONTENT,//本地下载(例如网页上的图片等)
    BRW_DL_TYPE_MAX
}BRW_DOWNLOADING_TYPE_E;

typedef BOOLEAN (*BRW_MSG_PROC_FUNC)(BRW_EXPORT_MSG_E message_id,void* param,uint16 size_of_param);
typedef void	(*BRW_SNAPSHOT_SAVE_CALLBACK)(SFS_ERROR_E sfs_error);
typedef void	(*BRW_SNAPSHOT_OPEN_CALLBACK)(char *url_ptr, BOOLEAN is_succ);

typedef struct tagBRW_PROXY_OPTION_T
{
    BOOLEAN     is_use_proxy;
    uint8*      proxy_addr_ptr;       
    uint16      proxy_port;
}BRW_PROXY_OPTION_T;

typedef struct tagBRW_PROFILE_T
{
#if 0
    BOOLEAN                 is_cache_enable;//enable http cache or not
    BOOLEAN                 is_cookie_enable;//enable http cookie or not
    BOOLEAN                 is_image_download;//download image of page or not
    BOOLEAN                 is_bgsound_download;//download bgsound or not
    BOOLEAN					is_gwt_enable;//is google wireless transcode enabled
    BOOLEAN                 is_use_wifi;
    BRW_PROTOCOL_TYPE_E     protocol;//protocol used
    uint8*                  user_agent_ptr;//user-agnet string
    uint8*                  ua_prof_ptr;//ua prof string
    MN_PCO_AUTH_TYPE_E auth_type;//auth type
    uint32					browser_pdp_priority;//browser pdp priority
    MN_DUAL_SYS_E			dual_sys;//dual sim choice
    char*					apn_ptr;//apn string
    char*					username_ptr;//user name string
    char*					psw_ptr;//password string
    BRW_PROXY_OPTION_T      proxy_opt;//proxy setting
    BRW_LANG_E				lang;//language setting
#endif    
    BRW_SCREEN_OPTION_T     screen_opt;//screen setting
    BRW_WIN_HANDLE          win_handle;//windows handle
    uint32					scroll_bar_handler;//window scroll bar handler
    uint32					scroll_bar_width;//scroll bar width
    uint32                  font_type;     //GUI_FONT_T font_type;

    BRW_MSG_PROC_FUNC       msg_proc_func;
}BRW_PROFILE_T;

typedef struct tagBRW_PUBLIC_PROFILE_T
{
    BOOLEAN                 is_cache_enable;//enable http cache or not
    BOOLEAN                 is_cookie_enable;//enable http cookie or not
    BOOLEAN                 is_image_download;//download image of page or not
    BOOLEAN                 is_preview_image;//previe image of page or not
    BOOLEAN                 is_bgsound_download;//download bgsound or not
    BOOLEAN					is_gwt_enable;//is google wireless transcode enabled
    BOOLEAN                 is_use_wifi;
    BRW_PROTOCOL_TYPE_E     protocol;//protocol used
    uint8*                  user_agent_ptr;//user-agnet string
    uint8*                  ua_prof_ptr;//ua prof string
    MN_PCO_AUTH_TYPE_E auth_type;//auth type
    uint32					browser_pdp_priority;//browser pdp priority
    MN_DUAL_SYS_E			dual_sys;//dual sim choice
    char*					apn_ptr;//apn string
    char*					username_ptr;//user name string,need free
    char*					psw_ptr;//password string,need free
    BRW_PROXY_OPTION_T      proxy_opt;//proxy setting
    BRW_LANG_E				lang;//language setting
#ifdef IPVERSION_SUPPORT_V4_V6
    MMICONNECTION_IP_TYPE_E ip_type;   //ip type
#endif
    //BRW_MSG_PROC_FUNC       msg_proc_func;
}BRW_PUBLIC_PROFILE_T;

typedef struct 
{
	uint8   day;
	uint8   mon;
	uint16  year;
}BRW_DATE_T;

typedef struct 
{
	uint16		*file_name;//file name:全路径名
	uint32		file_size;
	BRW_DATE_T	creat_date;
}BRW_SNAPSHOT_ITEM_INFO_T;

typedef struct tagBRW_VISIT_HIS_INDEX_QUE_T
{
    uint16 index_arr[BRW_MAX_VISIT_HIS_ITEM_NUM];
}BRW_VISIT_HIS_INDEX_QUE_T;

//页面浏览记录
typedef struct tagBRW_VISIT_HIS_INFO_T
{
    uint8      url_ptr[BRW_MAX_URL_LEN+1];//url
    uint16     name_ptr[BRW_MAX_HIS_NAME_LEN+1];//名字,unicode格式
    uint16     name_len;//名字长度
    uint32     visit_count;//访问次数
    uint32     last_visit_time;//最后访问时间
}BRW_VISIT_HIS_INFO_T;

//输入历史记录
typedef struct tagBRW_INPUT_HIS_INFO_T
{
    uint8      url_ptr[BRW_MAX_URL_LEN+1];//url
}BRW_INPUT_HIS_INFO_T;

//cnf of BRW_InitInstance
typedef struct tagBRW_MSG_INIT_CNF_T
{
    BOOLEAN result;    
}BRW_MSG_INIT_CNF_T;
//message struct of url change indication,app will 
// update it's address display to user usually
typedef struct tagBRW_MSG_CHANGE_URL_IND_T 
{
    uint8  url_arr[BRW_MAX_URL_LEN+1];//new url address,end with'\0'
}BRW_MSG_CHANGE_URL_IND_T;

typedef struct tagBRW_MSG_NEW_PAGE_REQ_IND_T
{
    uint8  url_arr[BRW_MAX_URL_LEN+1];
}BRW_MSG_NEW_PAGE_REQ_IND_T;

//message struct of browser progress info,app can
//display the progress status to user when receiving
//this message
typedef struct tagBRW_MSG_PROGRESS_IND_T
{
    BRW_REQ_STATE_E     state;//state of current page request
    BOOLEAN             is_download_req;//是否是下载请求
    uint32              download_req_handle;
    int32               total_data_len;//total len of 
    int32               received_data_len;
}BRW_MSG_PROGRESS_IND_T;

//error info struct
typedef struct tagBRW_MSG_ERROR_IND_T
{
    BRW_ERROR_E     error;  //error code
}BRW_MSG_ERROR_IND_T;

//download err indication
typedef struct tagBRW_MSG_DOWNLOAD_ERROR_IND_T
{
    uint32			handle;
    BRW_ERROR_E     error;  //error code
}BRW_MSG_DOWNLOAD_ERROR_IND_T;

typedef void (*BRW_AUTH_CBF)(uint32 handle,uint8* user_name,uint8* pwd,BOOLEAN result);
//http auth request info
typedef struct tagBRW_MSG_AUTH_REQ_T
{
    uint32          handle;
    //uint8			addr_arr[BRW_MAX_URL_LEN+1];//delete by fen.xie for useless
    BRW_AUTH_CBF    auth_cbf;
    uint32 auth_type;//add for OMA DL
}BRW_MSG_AUTH_REQ_T;

#ifdef SSL_SUPPORT
typedef void (*BRW_SSL_CERT_CBF)(uint32 handle,BOOLEAN result);
//http SSLCerl ind info
typedef struct tagBRW_MSG_SSL_CERT_IND_T
{
    uint32 handle;
    BRW_SSL_CERT_CBF  cert_cbf;
}BRW_MSG_SSL_CERT_IND_T;
#endif

//输入文件名操作后的回调接口形式
typedef void (*BRW_ENTER_FILENM_CBF)(uint32 handle,const uint16* file_name_ptr,uint16 file_name_len,BOOLEAN result);
//download enter path indication struct

typedef struct tagBRW_MSG_ENTER_FILENM_REQ_T
{
    uint32                  handle;//unique identification for download
    uint16                  res_name_arr[BRW_MAX_URL_LEN+1];//resource name of download object,end with '\0'
    BRW_MIME_TYPE_E         mime_type;//mime type of download obj
    uint32                  content_len;
    BRW_ENTER_FILENM_CBF    enter_filenm_cbf;
#ifdef DRM_SUPPORT    
    BRW_MIME_TYPE_E     drm_mo_mime_type;//mime type of drm(if download obj' tyep is DRM)    
    uint32  expired_time;//the expired_time of DCF's RO
#endif    
#ifdef OMADL_SUPPORT
    uint8   *next_url_ptr;//"NextURL" defined in DD
#endif
    BRW_DOWNLOADING_TYPE_E dl_type;
}BRW_MSG_ENTER_FILENM_REQ_T;

//下载确认操作的回调接口形式
#ifdef OMADL_SUPPORT
typedef void (*BRW_DL_CONFIRM_CBF)(uint32 handle,const uint16* file_name_ptr,uint16 file_name_len,BOOLEAN result);
//oma download confirm message struct(参加ODM_DOWNLOAD_DESCRIPTION_T,对应的数据结构的长度要一致)
typedef struct tagBRW_MSG_DL_CONFIRM_REQ_T
{
    uint32      handle;//unique identification for download:wap_req_id
    BRW_DL_CONFIRM_CBF      dl_confirm_cbf;//回调接口
    //uint8       uri_arr[ODM_DD_SCHEMA_URI+1];//uri of download object,end with '\0'
    char    next_url[ ODM_DD_SCHEMA_URI+1];	// next url after download completed	
    uint16      name_arr[ODM_DD_SCHEMA_SHORT_STRING+1];// Media object name
    uint32      size;	// media object size	
    uint16      description_arr[ODM_DD_SCHEMA_LONG_STRING+1];//detail description(such as oma dd),in unicode format
    uint16      vendor_arr[ODM_DD_SCHEMA_SHORT_STRING+1];		// media object vendor	
    char        type[ODM_DD_SCHEMA_SHORT_STRING+1];	// Media object mime type	
    char        dd_version_arr[ODM_DD_SCHEMA_SHORT_STRING+1];	// dd version		
    BRW_MIME_TYPE_E     mime_type;//mime type of download obj    
#ifdef DRM_SUPPORT    
    BRW_MIME_TYPE_E     drm_mo_mime_type;//mime type of drm(if download obj' tyep is DRM)    
    uint32  expired_time;//the expired_time of DCF's RO
#endif    
}BRW_MSG_OMADL_CONFIRM_REQ_T;
#endif

//下载OK消息
typedef struct tagBRW_MSG_DL_OK_IND_T
{
    uint32 handle;//unique identification for download
}BRW_MSG_DL_OK_IND_T;

#if 0
//make call请求(wtai)
typedef struct tagBRW_MSG_MAKE_CALL_REQ_T
{
    uint8 phone_num_arr[BRW_WTAI_MAX_PHONE_NUM_LEN+1];
}BRW_MSG_MAKE_CALL_REQ_T;

//send dtmf请求(wtai)
typedef struct tagBRW_MSG_SEND_DTMF_REQ_T
{
    uint8 dtmf_arr[BRW_WTAI_MAX_DTMF_LEN+1];
}BRW_MSG_SEND_DTMF_REQ_T;

//add pb entry请求(wtai)
typedef struct tagBRW_MSG_ADD_PB_REQ_T
{
    uint8 phone_num_arr[BRW_WTAI_MAX_PHONE_NUM_LEN+1];
    uint8 name[BRW_WTAI_MAX_PB_NAME_LEN+1];
}BRW_MSG_ADD_PB_REQ_T;

//send sms请求
typedef struct tagBRW_MSG_SEND_SMS_REQ_T
{
    uint8 phone_num_arr[BRW_WTAI_MAX_PHONE_NUM_LEN+1];
    uint8 sms_content_arr[BRW_WTAI_MAX_SMS_CONTENT_LEN+1];
}BRW_MSG_SEND_SMS_REQ_T;

//send email请求
typedef struct tagBRW_MSG_SEND_MAILTO_REQ_T
{
    uint8 email_name_arr[BRW_WTAI_MAX_EMAIL_NAME_LEN+1];
}BRW_MSG_SEND_MAILTO_REQ_T;
#endif

typedef struct 
{
	uint32 callback_msg_id;
	uint32 callback_win_id;
}BRW_SAVE_IMAGE_CB_PARAM_T;

typedef enum
{
    BRW_PDP_STATUS_NONE,
    BRW_PDP_STATUS_CONNECTING,
    BRW_PDP_STATUS_CONNECT_FAIL,
    BRW_PDP_STATUS_CONNECT_OK,
    BRW_PDP_STATUS_MAX
}BRW_PDP_STATUS_E;

typedef struct 
{
    uint16  *name_ptr;
    uint8   *data_ptr;
    uint32  data_len;
    BRW_MIME_TYPE_E mime_type;
}BRW_IMAGE_LIST_ITEM_INFO_T;

typedef struct tagBRW_MSG_HTTP_REFRESH_IND_T
{
    uint32                  refresh_time;           //refresh time: seconds
    uint8  url_arr[BRW_MAX_URL_LEN+1];
}BRW_MSG_HTTP_REFRESH_IND_T;

/****************************************************************************/
// Description:API for init browser engine
// Param:   option_ptr  option of browser
// Return:  instance handle of browser,0 if FALSE   
/****************************************************************************/
PUBLIC BRW_INSTANCE BRW_InitInstance(BRW_PROFILE_T *option_ptr, BOOLEAN is_fixed);
/****************************************************************************/
// Description:release relevant resouce and memory of browser engine
// Param: instance   instance handle of browser
// Return:    
/****************************************************************************/
PUBLIC void BRW_DestroyInstance(BRW_INSTANCE instance);

/****************************************************************************/
// Description:release relevant resouce and memory of g_brw_instance_list
// Param: 
// Return:    
/****************************************************************************/
PUBLIC void BRW_DestroyInstanceList(void);

/****************************************************************************/
//Description : To disconnect the network
//Global resource dependence : 
//Author: juan.zhang
//Note:
/****************************************************************************/
PUBLIC void BRW_DisconnetNetwork(BRW_INSTANCE instance);

/****************************************************************************/
// Description:call this api to start a web page request
// Param:instance   instance handle of browse 
//       uint8* url_ptr         url of web page,end with '\0'
// Return:  TRUE if success,else FALSE  
/****************************************************************************/
PUBLIC uint32 BRW_AccessPage(BRW_INSTANCE instance,uint8* url_ptr);

PUBLIC uint32 BRW_AccessPageEx(BRW_INSTANCE instance,
            uint8* url_ptr,//[in]
            uint16* ico_filename_ptr//[in]full filename:if not null,need to load ico of the url and save by it
            );

/****************************************************************************/
// Description:display current web page
// Param: instance   instance handle of browse 
// Return: id of page req   
/****************************************************************************/
PUBLIC BOOLEAN BRW_DisplayPage(BRW_INSTANCE instance);

PUBLIC BOOLEAN BRW_RepaintPage(BRW_INSTANCE instance);

/****************************************************************************/
// Description:stop current page request
// Param:instance   instance handle of browse 
// Return:    
/****************************************************************************/
PUBLIC BOOLEAN BRW_StopPageReq(BRW_INSTANCE instance);
/****************************************************************************/
//	desc:stop download which req_id equal req_id
//  param:instance   instance handle of browser 
//        req_id     request id which identify download request
//  return:TRUE if success,else false
/****************************************************************************/
PUBLIC BOOLEAN BRW_StopDL(BRW_INSTANCE instance,uint32 req_id);
/****************************************************************************/
//	desc:to check if current displaying page is Blank
//  param:instance   instance handle of browser 
//  return:TRUE is blank, FALSE is NOT
/****************************************************************************/
PUBLIC BOOLEAN BRW_IsPageBlank(BRW_INSTANCE instance);

/****************************************************************************/
//desc:get state of current page request
//param:instance   instance handle of browse 
//return:state of current page request;
/****************************************************************************/
PUBLIC BRW_REQ_STATE_E Brw_GetPageReqState(BRW_INSTANCE instance);
/****************************************************************************/
//Desc:process the key event of ui
//param: instance   instance handle of browse 
//       key_event   key event type
//return:TRUE if success,else FALSE
/****************************************************************************/
PUBLIC BOOLEAN  BRW_ProcKeyEvent(BRW_INSTANCE instance,BRW_KEY_EVENT_E key_event);
/****************************************************************************/
//Desc:process the pen down event
//param: instance   instance handle of browse 
//        pen down relevant info
//return:TRUE if success,else FALSE
/****************************************************************************/
PUBLIC BOOLEAN  BRW_ProcPenDownEvent(BRW_INSTANCE instance,BRW_PEN_CLICK_INFO_T *pendow_info_ptr);

PUBLIC BOOLEAN  BRW_ProcPenUpEvent(BRW_INSTANCE instance,BRW_PEN_CLICK_INFO_T *penup_info_ptr);
/****************************************************************************/
//desc:process page navigate event(such as forward,backward)
//param:instance   instance handle of browse 
//       nav_event  page navigate event type
//return:TRUE if success,else FALSE
/****************************************************************************/
PUBLIC BOOLEAN  BRW_ProcNavEvent(BRW_INSTANCE instance,BRW_NAVIGATE_EVENT_E nav_event);
/****************************************************************************/
//desc:process page scroll event
//param:instance   instance handle of browse 
//       scroll_event  scroll event type
//       top_index top index, only useful when scroll_event is BRW_SCROLL_EVENT_MOVE_*
//return:TRUE if success,else FALSE
/****************************************************************************/
PUBLIC BOOLEAN  BRW_ProcScrollEvent(BRW_INSTANCE instance,BRW_SCROLL_EVENT_E scroll_event, uint32 top_index);

/****************************************************************************/
//desc:process tp scroll event
//param:instance   instance handle of browse 
//        move_y  vertical move position
//return:TRUE if success,else FALSE
/****************************************************************************/
PUBLIC BOOLEAN  BRW_ProcTpScrollEvent(BRW_INSTANCE instance, int16 move_y);

/****************************************************************************/
//desc:get current page title
//param: instance   instance handle of browse 
//        buf         input buffer to save title
//        title_len_ptr   real title len(output)
//        buf_max_len max len of buf
//return:TRUE if success,else FALSE
/****************************************************************************/
PUBLIC BOOLEAN  BRW_GetPageTitle(BRW_INSTANCE instance,
                uint16* buf,//[IN/OUT]
                uint16* title_len_ptr,//[IN/OUT]
                uint16 buf_max_len//not include an end flag
                );

/****************************************************************************/
//desc:get current page url(without snapshot)
//param: instance   instance handle of browse 
//        url_pptr 
//        is_snapshot_ptr
//return:TRUE if success,else FALSE
/****************************************************************************/
PUBLIC BOOLEAN  BRW_GetPageUrl(BRW_INSTANCE instance,
                uint8 *url_buf_ptr, //[IN/OUT]
                uint16 buf_max_len, //not include an end flag
                BOOLEAN *is_snapshot_ptr,//[IN/OUT]
                BOOLEAN *is_blank_page_ptr//[IN/OUT]
                );

/****************************************************************************/
//desc:set param for the browser instance
//param: instance    instance of browser
//        profile     parameter
//        profile_type   the param want to set,the flag may be bit and
//return:TRUE if success,else FALSE
/****************************************************************************/
PUBLIC BOOLEAN  BRW_SetParam(BRW_INSTANCE instance,BRW_PROFILE_T* profile,BRW_PROFILE_TYPE_E profile_type);

PUBLIC BOOLEAN  BRW_SetPublicParam(BRW_PUBLIC_PROFILE_T* public_profile,BRW_PROFILE_TYPE_E profile_type);

/****************************************************************************/
//	get MIME TYPE   value according mime_type string
/****************************************************************************/
PUBLIC  BRW_MIME_TYPE_E BRW_GetMineType(uint8*   mime_type_str);
/****************************************************************************/
//	desc:process message
//  param:msg_id  message id
//        param   message parameter
//  TRUE if msg is browser's message,else FALSE
/****************************************************************************/
PUBLIC BOOLEAN BRW_ProcMsg(uint32 msg_id,void* param);
/****************************************************************************/
//	desc:check if backward enable
//  return:return TRUE if enable,else FALSE
/****************************************************************************/
PUBLIC BOOLEAN BRW_NavHisIsBackwardEnable(void);
/****************************************************************************/
//	desc:check if forward enable
// return:return TRUE if enable,else FALSE
/****************************************************************************/
PUBLIC BOOLEAN BRW_NavHisIsForwardEnable(void);

/****************************************************************************/
//Description : layout timer handler
//Global resource dependence : 
//Author: juan.zhang
//Note:	
/****************************************************************************/
PUBLIC BOOLEAN BRW_ProcLayoutTimer(BRW_INSTANCE instance,uint32 timer_id);

/****************************************************************************/
//Description : to stop layout timer
//Global resource dependence : 
//Author: juan.zhang
//Note:	
/****************************************************************************/
PUBLIC BOOLEAN BRW_StopLayoutTimerById(BRW_INSTANCE instance, uint32 timer_id);

#if 0 /* Reduce Code size */
/****************************************************************************/
//Description : to get the image url in webpage
//Global resource dependence : 
//Author: juan.zhang
//Note:	
/****************************************************************************/
PUBLIC BOOLEAN BRW_GetPageImageUrl(
								   BRW_INSTANCE instance, //[IN]
								   uint8 **url_ptr, //[OUT]
								   uint32 *index_ptr //[OUT]
								   );
/****************************************************************************/
//Description : to save one image in current webpage
//Global resource dependence : 
//Author: juan.zhang
//Note:	cb_param_ptr在收到保存完成的消息之前应该一直有效，不能是临时变量
/****************************************************************************/
PUBLIC BOOLEAN BRW_SavePageImage(
								 BRW_INSTANCE instance, //[in]
								 uint32 index,			//[in]
								 uint16 *full_path_ptr,	//[in]
								 BRW_SAVE_IMAGE_CB_PARAM_T *cb_param_ptr//[in]:if 0, using sync ffs.
								 );
#endif /* Reduce Code size */
PUBLIC BOOLEAN BRW_ClearCache(BRW_INSTANCE instance);
PUBLIC BOOLEAN BRW_ClearCookie(BRW_INSTANCE instance);
/****************************************************************************/
//Description : to Clean up the ram
//Global resource dependence : 
//Author: juan.zhang
//Note:	APP在收到内存不足时先调用该函数，函数返回值FALSE代表无内存可整理，TRUE代表
//		进行过内存整理。APP应视情况提示。
/****************************************************************************/
PUBLIC BOOLEAN BRW_RamCleanUp(void);

/*****************************************************************************/
//  Description : search the url that is include invalid char
//  Global resource dependence : 
//  Author: fen.xie
//  Return: TRUE: contain invalid char. FALSE: do not contain invalid char
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN Brw_IsIncludeInvalidUrl(const uint8* buf_ptr);

PUBLIC BOOLEAN BRW_IsNetLinked(void);                                             
PUBLIC void BRW_SetNetLinked(BOOLEAN is_net_linked);
PUBLIC BRW_PDP_STATUS_E BRW_GetPdpStatus(void);

/*****************************************************************************/
// Description : convert browser internal mine type to mime type in platform
// Global resource dependence :
// Author:  fen.xie
// Note:
/*****************************************************************************/
PUBLIC MIME_TYPE_E BrwConvertMimeType(BRW_MIME_TYPE_E brw_mime_type);

/*****************************************************************************/
// Description: get extension of a special mime type(BRW_MIME_TYPE_E)
// Global resource dependence: 
// Author: fenxie
// Note: 
/*****************************************************************************/
PUBLIC const char *BrwMimeToExt( uint32 mime_value );

/****************************************************************************/
/***********************SNAPSHOT FUNCTIONS BEGIN*****************************/
/****************************************************************************/

/****************************************************************************/
//Description : init the snapshot
//Global resource dependence : 
//Author: juan.zhang
//Note:	
/****************************************************************************/
PUBLIC void BRW_SnapshotInit(void);

/****************************************************************************/
//Description : exit the snapshot
//Global resource dependence : 
//Author: juan.zhang
//Note:	
/****************************************************************************/
PUBLIC void BRW_SnapshotExit(void);

/****************************************************************************/
//Description : to save a WebPage
//Global resource dependence : 
//Author: juan.zhang
//Note:	
/****************************************************************************/
PUBLIC SFS_ERROR_E BRW_SnapshotSaveCurWebPage(
												   BRW_INSTANCE	instance,
												   const uint16 *save_route_ptr, //[in]
												   const uint16 *file_name_ptr,	//[in]file name WITHOUT suffix
												   BRW_SNAPSHOT_SAVE_CALLBACK callback
												   );
/****************************************************************************/
//Description : to check if the name is exist
//Global resource dependence : 
//Author: juan.zhang
//Note:	return val: 1--file exist; 2--folder exist; 0--either file or folder is NOT exist
/****************************************************************************/
PUBLIC uint32 BRW_SnapshotCheckIfNameExist(
											const uint16 *save_route_ptr, //[in]
											const uint16 *file_name_ptr//[in]file name without suffix
											);

/****************************************************************************/
//Description : to handle the ffs message
//Global resource dependence : 
//Author: juan.zhang
//Note:	
/****************************************************************************/
PUBLIC void BRW_SnapshotHandleFfsMsg(uint32 msg_id, SFS_ERROR_E sfs_error);

/****************************************************************************/
//Description : Parse an offline WebPage
//Global resource dependence : 
//Author: juan.zhang
//Note:	This is the 1st step to browse an offline WebPage
/****************************************************************************/
PUBLIC BOOLEAN BRW_SnapshotParseOfflinePage(
										 const uint16 *file_path_ptr, //[IN]file path including route and file name WITH suffix
										 BRW_INSTANCE browser_instance,
										 BRW_SNAPSHOT_OPEN_CALLBACK callback
										 );

/****************************************************************************/
//Description : Parse an offline WebPage by buffer
//Global resource dependence : 
//Author: fen.xie
//Note:	This is the 1st step to browse an offline WebPage
/****************************************************************************/
PUBLIC BOOLEAN BRW_SnapshotParseOfflinePageEx(
            	uint8 *file_buf_ptr,
            	uint32 file_size,
                BRW_INSTANCE browser_instance,
                BRW_SNAPSHOT_OPEN_CALLBACK callback, 
                BOOLEAN is_fixed
                );

/****************************************************************************/
//Description : Layout an offline WebPage
//Global resource dependence : 
//Author: juan.zhang
//Note:	This is the 2nd step to browse an offline WebPage, APP should call this
//		function when callback success
/****************************************************************************/
PUBLIC void BRW_SnapshotLayoutOfflinePage(void);
/****************************************************************************/
//Description : To end an offline WebPage
//Global resource dependence : 
//Author: juan.zhang
//Note:	This is the last step to browse an offline WebPage, APP should call
//		this function when closing an offline Webpage
/****************************************************************************/
PUBLIC void BRW_SnapshotEndOfflinePage(void);

/****************************************************************************/
//Description : to get an item info
//Global resource dependence : 
//Author: juan.zhang
//Note:
/****************************************************************************/
PUBLIC const BRW_SNAPSHOT_ITEM_INFO_T* BRW_SnapshotGetItemInfo(uint32 index);

/****************************************************************************/
//Description : to delete a snapshot item
//Global resource dependence : 
//Author: juan.zhang
//Note:
/****************************************************************************/
PUBLIC BOOLEAN BRW_SnapshotDeleteItem(uint32 index);

/****************************************************************************/
//Description : to get snapshot item count
//Global resource dependence : 
//Author: juan.zhang
//Note:
/****************************************************************************/
PUBLIC uint32 BRW_SnapshotGetCount(void);

/****************************************************************************/
//Description : snapshot file rename
//Global resource dependence : 
//Author: juan.zhang
//Note:new_name_path_ptr is the full path including the new file name
/****************************************************************************/
PUBLIC BOOLEAN BRW_SnapshotRenameFile(uint32 index, uint16 *new_name_path_ptr);
/****************************************************************************/
/***********************SNAPSHOT FUNCTIONS END*******************************/
/****************************************************************************/



/****************************************************************************/
/***********************HISTORY FUNCTIONS BEGIN******************************/
/****************************************************************************/
/*
 *	desc:init visit history
 */
PUBLIC BOOLEAN BRW_VisitHisInit(void);
/*
 *	desc:exist history,store data to persistent storage
 */
PUBLIC void BRW_VisitHisExit(void);

/****************************************************************************/
//	desc:delete an visit history item
//  param:url_ptr  url of item
//  return: 
/****************************************************************************/
PUBLIC BOOLEAN BRW_VisitHisDelItem(uint16 index);

/****************************************************************************/
//	desc:delete an visit history
//  param:url_ptr  url of item
//  return: 
/****************************************************************************/
PUBLIC void BRW_VisitHisDel(uint8* url_ptr);

/****************************************************************************/
//	desc:clean visit history list
/****************************************************************************/
PUBLIC void Brw_VisitHisClean(void);

/****************************************************************************/
//	desc:获得页面浏览记录的索引队列
//  param:index_arr_ptr 索引队列指针
//  return:页面浏览记录的个数
/****************************************************************************/
PUBLIC uint16 BRW_VisitHisGenIndexArr(BRW_VISIT_HIS_INDEX_QUE_T* index_arr_ptr);

/****************************************************************************/
//desc:获得页面浏览记录的总数
//param:
//return:页面浏览记录的个数
/****************************************************************************/
PUBLIC uint16 BRW_VisitHisGetTotal(void);

/****************************************************************************/
//	desc:返回某个索引对应的页面浏览历史记录条目信息
//  param: index  索引
//  return:成功返回对应的页面浏览历史记录条目指针,失败返回NULL
/****************************************************************************/
PUBLIC BRW_VISIT_HIS_INFO_T* BRW_VisitHisGetHisInfoByIndex(uint16 index);

PUBLIC uint8* BRW_NavHisGetCurUrl(void);

/****************************************************************************/
// author:fen.xie
// desc:clean the history file
//  notice:
// return:BOOLEAN,result
/****************************************************************************/
PUBLIC BOOLEAN BRW_ClearVisitHis(void);

/****************************************************************************/
/***********************HISTORY FUNCTIONS END********************************/
/****************************************************************************/
/*
 *	desc:wap 清理cache
 *  param:
 *  result：
 */
PUBLIC BOOLEAN BRW_ClearAllCache(void);

/*****************************************************************************/
// Description : Get  File Name for download by mime type
// Global resource dependence :
// Author: fen.xie
// Note:  
/*****************************************************************************/
PUBLIC BOOLEAN  BrwGetDlFileNameByMimeType(uint16 *file_name_ptr, uint32 name_len, uint32 mime_type, uint16 *default_file_name_ptr);

/*
 *	desc:wap 清理cookie
 *  param:
 *  result：
 */
PUBLIC BOOLEAN BRW_ClearAllCookie(void);

#ifdef MMIDORADO_INPUT_HISTORY_SUPPORT
/*
 *	desc: free input history list
 */
 /*
 *	desc:init input history
 */
PUBLIC BOOLEAN BRW_InputHisInit(void);

/*
 *	desc:exist input history,store data to persistent storage
 */
PUBLIC void BRW_InputHisExit(void);

/*
 *	desc:Delete input history
 */
PUBLIC void BrwDeleteInputHis(void);

/*
@desc:delete the input history file
@param: 
@return:TRUE if success,else FALSE
*/
PUBLIC BOOLEAN BRW_ClearInputHis(void);

/*
 *	desc:add input history item
 *  param: url_ptr  url
 *  return:TRUE if success,else FALSE
 */
PUBLIC BOOLEAN BrwInputHisAddItem(const uint8* url_ptr, uint16 url_len);

/*
 *	desc:Get match input history num
 *  param: url_ptr  url
 *  return:TRUE if success,else FALSE
 */
PUBLIC uint16 BRW_GetMatchInputHisNum(const uint8 *url, uint16 url_len);

/*
 *	desc:Get match input history num by index
 *  param: url_ptr  url
 *  return:TRUE if success,else FALSE
 */
PUBLIC uint8* BRW_GetMatchInputHisUrlByIndex(const uint8 *url, uint16 url_len, uint16 index);
#endif

/*
 *  desc:init the memory for browser
 *  param: url_ptr  url
 *  return:TRUE if success,else FALSE
 */
PUBLIC BOOLEAN BRW_InitMem(void);

/*
 *  desc:free the memory for browser
 *  param: url_ptr  url
 *  return:
 */
PUBLIC void BRW_DeleteMem(void);

/*
* Description:for BrwHashDestroy
* Param: 
* Return:    
*/
PUBLIC void BRW_DestroyHash(void);    

/*
* Description:for BrwParseDestroy
* Param: 
* Return:    
*/
PUBLIC void BRW_DestroyParse(void);

/*
 *	desc: free input history list
 */
 /*
 *	desc:init input history
 */
PUBLIC BOOLEAN BRW_InputHisInit(void);

/*
 *	desc:exist input history,store data to persistent storage
 */
PUBLIC void BRW_InputHisExit(void);

/*
 *	desc:获取存放浏览器系统文件的设备名(unicode格式)
 *  param:device_name_len_ptr[out]  设备名长度
 *  return:unicode格式的设备名
 */
PUBLIC uint16* BRW_GetSysDevName(uint16 *device_name_len_ptr);
/*
 *	desc:获取存放浏览器cookie的设备名(unicode格式)
 *  param:device_name_len_ptr[out]  设备名长度
 *  return:unicode格式的设备名
*/
PUBLIC uint16* BRW_GetCookieDevName(uint16 *device_name_len_ptr);
/*
 *  desc:judge that whether the mime type of focus hot link is an image
 *  param: url_ptr  url
 *  return:TRUE: contain invalid char. FALSE: do not contain invalid char
 */
PUBLIC  BOOLEAN BRW_IsFocusHotLinkImg(BRW_INSTANCE instance);

/*
 *  desc:get the info of focus hot link
 *  param: 
 *  return:
 */
PUBLIC  BOOLEAN BRW_GetFocusLinkInfo(BRW_INSTANCE instance,
                uint16 *file_name_ptr,//[in/out]
                uint16 file_name_len_max,
                BRW_MIME_TYPE_E *mime_type_ptr,//[in/out]
                uint8   **data_pptr,    //[in/out]image data
                uint32  *data_len_ptr//[in/out]
                );

#ifdef TOUCH_PANEL_SUPPORT
/*
 *  desc:get the info of current page by tp point
 *  param:
 *  return:
 */
PUBLIC  BOOLEAN BRW_GetTpPointInfo(BRW_INSTANCE instance,
                uint16 *file_name_ptr,//[in/out]
                uint16 file_name_len_max,
                BRW_MIME_TYPE_E *mime_type_ptr,//[in/out]
                uint8   **data_pptr,    //[in/out]image data
                uint32  *data_len_ptr,//[in/out]
                uint32 x, 
                uint32 y
                );
                
/*
 *  desc:judge that whether the mime type of focus hot link is an image
 *  param: url_ptr  url
 *  return:TRUE: contain invalid char. FALSE: do not contain invalid char
 */
PUBLIC  BOOLEAN BRW_IsTpPointImg(BRW_INSTANCE instance,uint32 x, uint32 y);
#endif

#ifdef JS_SUPPORT
PUBLIC BOOLEAN BrwCancelWait(void *param, uint16 size_of_param);
#endif
/*
 *	desc:set profile for g_brw_instance_list_info
 *  param:profile      profile pointer
 *  return:if SUCCESS return TRUE
 *  
 */
PUBLIC BOOLEAN BrwSetPublicProfile(BRW_PUBLIC_PROFILE_T* profile_ptr);

/*
 *	desc:free profile for g_brw_instance_list_info
 *  param:profile      profile pointer
 *  
 */
PUBLIC void BrwDestoryPublicProfile(void);

PUBLIC void BRW_DestructInstanceList(void);

/*
 *	desc:get profile of BRW_PUBLIC_PROFILE_T struct
 */
PUBLIC BRW_PUBLIC_PROFILE_T* BrwGetPublicProfile(void);

PUBLIC BOOLEAN ActiveNetwork(BRW_INSTANCE instance);
PUBLIC BOOLEAN DeactiveNetwork(BRW_INSTANCE instance);

/*
 *	process  after  net link request 
 */
PUBLIC BOOLEAN Brw_ProcAfterNetLinkCnf(BRW_INSTANCE instance);

/*
 *	free BRW_INFO_T relevant memory
 */
PUBLIC void BRW_FreeBrwInfoStructForFixed(BRW_INSTANCE instance);

/*
* Description:BRW_ParseInit
*/
PUBLIC void BRW_ParseInit(void);

/*****************************************************************************/
//  Description : stop BG sound and free the bgsound data
//  Global resource dependence : none
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN Brw_StopAndFreeBGsound(BRW_INSTANCE instance);
/*
*   desc:get the full file name of the ico of current web page
*   param:
*   return:result
*/
PUBLIC BOOLEAN BrwGetWebIcoFullName(
                uint16 *ico_name_ptr,//[in/out]
                uint16   *ico_name_len_ptr  //[in/out], 双字节为单位(the max len of cache_path_ptr)
                );

/*
 *	desc:the init operations in http interface module
 *	notes:
 *  param:
 *  return:result
 */
PUBLIC void Brw_HttpInit(void);

/*
 *	desc:the exit operations in http interface module
 *	notes:
 *  param:
 *  return:result
 */
PUBLIC void Brw_HttpExit(void);

/*
 *	desc:check if layout_handle is belong to browser instance brw_info_ptr->cur_display_req_ptr 
 *  input: brw_info_ptr browser instance
 *         layout_handle
 *  return:TRUE if layout belong to brw_info_ptr->cur_display_req_ptr ,else false
 */
PUBLIC BOOLEAN BrwIsLayoutBelongToCurDisplay(void* layout_handle);

/*
 *  desc:
 *  input: 
 *  return:
 */
#ifndef MMI_BROWSER_MINI_SUPPORT
PUBLIC uint32 Brw_InitImageListInfo(BRW_INSTANCE instance);
PUBLIC uint32 Brw_GetImageListCount(void);
PUBLIC void Brw_FreeImageListInfo(void);
#endif

PUBLIC BOOLEAN Brw_ResetLinkSettings(BRW_INSTANCE instance);
PUBLIC const BRW_IMAGE_LIST_ITEM_INFO_T* BRW_GetImageListItemInfo(uint32 index);

#ifdef MMIDORADO_WRE_TRANSCODE_SUPPORT//just for chinese 
/*****************************************************************************/
// Description : move to gwt end.
// Global resource dependence :
// Author: fen.xie
// Note:      
/*****************************************************************************/
PUBLIC uint8* BrwGwtMovetoGwtEnd(uint8 *absolute_url_ptr);
#endif

#endif
