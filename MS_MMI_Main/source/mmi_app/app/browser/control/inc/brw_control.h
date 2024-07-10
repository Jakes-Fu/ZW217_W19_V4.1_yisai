/*****************************************************************
 **  File Name: 	brw_control.h    							 *
 **  Author:		            								 *
 **  Date:		         										 *
 **  Description:definition of structure and api of control      *
                    message sub module                           *
******************************************************************
******************************************************************
**  Edit History											     *
**---------------------------------------------------------------*
 **  DATE			NAME			DESCRIPTION				     *
 **  20/16/2008		yuzhi.chen      Create. 					 *
*****************************************************************/
#ifndef BRW_CONTROL_H
#define BRW_CONTROL_H

#include "brw_core_api.h"
#include "mime_type.h"

#define BRW_INVALID_REQ_ID  0
#define BRW_INSTANCE_NUM_MAX    3

typedef struct tagHTTP_REQ_PARAM_T
{
    uint8*                      uri_ptr;
    uint8*                      user_agent_ptr;
    uint8*                      ua_prof_ptr;
    uint8*                      refer_ptr;
    HTTP_BODY_DATA_T            body_data;
    HTTP_METHOD_E               method;
    BOOLEAN                     is_sub_media_req;//是否为子媒体请求
	BOOLEAN						is_using_relative_url;//是否http GET/POST使用相对路径，在使用gwt时须使用相对路径
}HTTP_REQ_PARAM_T;
typedef enum
{
    BRW_HTTP_ENCODING_TYPE_NULL=0,
    BRW_HTTP_ENCODING_TYPE_GZIP,      //accept encoding header value: gzip
    BRW_HTTP_ENCODING_TYPE_UNSUPPORT      //other is not supported
}BRW_HTTP_ENCODING_TYPE_E;
//下载子状态
typedef enum
{
    BRW_DL_STATE_UNKNOWN,
    BRW_DL_STATE_HTTP_CONTENT,
    BRW_DL_STATE_OMA_DD,
    BRW_DL_STATE_OMA_CONTENT,
    BRW_DL_STATE_MAX
}BRW_DOWNLOADING_STATE_E;

//http init state
typedef enum
{
	BRW_HTTP_STATE_NONE,
	BRW_HTTP_STATE_INITING,
	BRW_HTTP_STATE_INITED,
	BRW_HTTP_STATE_CLOSING
}BRW_HTTP_STATE_E;

//HTTP请求
typedef struct tagHTTP_REQ_INFO_T
{
    uint32                          id;//local id
	uint32					        http_request_id; 	 		
	uint8*					        resp_buf_ptr;//具体的buf空间等到收到http 头部才分配
    uint32                          resp_buf_max_len;
    uint32                          resp_buf_now_len;
	BRW_MIME_TYPE_E  				mime_type;
	uint8*      			        charset;
	uint32					        http_content_len;//http content-len请求头	
    BRW_HTTP_ENCODING_TYPE_E       transfer_encoding;
}HTTP_REQ_INFO_T;
typedef struct tagMEDIA_OBJ_T
{
	const uint8*			url_ptr; 	
	uint32					page_req_id;//page req id
    HTTP_REQ_INFO_T*        http_req_ptr;
    uint32					index; //资源索引，parser模块传过来,如果是页面请求，则无意义    
    BOOLEAN  		        is_req_handling;//请求是否正在被处理
    MEDIA_REQ_TYPE_E        media_req_type;//媒体请求类型
    BOOLEAN is_retried;//是否已经做过重试
}MEDIA_OBJ_T;
//media request list
typedef struct tagMEDIA_OBJ_INFO_T
{
	BRW_LIST_T		meida_list;// BRW_HTTP_REQ_INFO类型
	BRW_LIST_T 		*wait_process_obj_list_ptr;//下一个待处理请求对象，仅为索引
}MEDIA_OBJ_INFO_T;

typedef struct tagWAP_REQUEST_T{
    BOOLEAN						is_cached_req;	//该req是否是后退缓存的req
    BOOLEAN						has_been_cached;//该req是否被缓存了，被缓存的req在释放的时候layout handler不能释放
    uint32						req_id;
    uint8*						absolute_url; //url of current page
    HTTP_REQ_INFO_T*            http_req_ptr;
    
    //如下三个变量主要是刷新的时候用（记录原来的url，体部数据，方法)
    uint8*                      original_url;
    HTTP_METHOD_E               method;
    HTTP_BODY_DATA_T            *body_data_info_ptr;

    BRW_REQ_STATE_E			    state; 
    MEDIA_OBJ_INFO_T   			media_obj_info;//about image list,css list etc.
    LAYOUT_HANDLE	  			layout_handle;
    uint32                      oma_dl_session_id;
    BRWPL_FFS_HANDLE			ffs_handler;//for wsp downloading
    uint16						*dl_file_name_ptr;//用户确认的下载文件全路径名
    uint16						*wsp_dl_temp_file_name_ptr;//WSP下载需要的temp文件名
        /*wsp_dl_temp_file_name_ptr is not only for WSP, also use for temp file name in download*/
    BRW_DOWNLOADING_STATE_E     download_state;
    
    BRW_NAVIGATE_EVENT_E        cur_nav_event;
    BOOLEAN is_retried;//是否已经做过重试
#ifdef DRM_SUPPORT
    uint32  drm_sd_time_out;//the Expired Time of the rights file,when download a sd drm
#endif
    BOOLEAN is_need_ico;//need to load the ico of the url
    uint16* ico_filename_ptr;//the ico data need to be saved by the full filename
}WAP_REQUEST_T;

typedef struct 
{
	uint8			*page_content_ptr;
	uint32			page_content_len;
	BRW_MIME_TYPE_E page_mime_type;
	uint8			*charset_ptr;
}BRW_PAGE_CONTENT_T;

typedef  struct tagBRW_INFO_T
{
	uint32					instance_id;
    BRW_PROFILE_T			profile;
   	BRW_LIST_T				dl_req_list;//下载请求队列
   	WAP_REQUEST_T  			*cur_req_ptr; //当前页面请求
    WAP_REQUEST_T			*cur_display_req_ptr;//当前显示的页面
	BRW_PAGE_CONTENT_T		cur_page_content;
	LAYOUT_HANDLE			snapshot_open_layout_handler;
    uint32					sub_http_module_id;
    uint32					http_context_id;
	BRW_HTTP_STATE_E		http_state;
	BOOLEAN					is_need_to_close_http; //在无法立即close http session时，设置此变量，等条件满足后据此关闭http session
	BOOLEAN                 is_need_to_clear_cookie;//在无法立即clear cookie的时候，设置此变量，等条件满足后据此clear cookie
    //uint32					net_id;
    BOOLEAN                 is_fixed;//TRUE:fixed instance
#ifdef HTTP2_SUPPORT
    uint8*				curr_host_url; 
    uint8                      curr_host_is_http2;
#endif
}BRW_INFO_T;

typedef struct 
{
    BRW_INFO_T  *browser_info_ptr[BRW_INSTANCE_NUM_MAX];
    int16  cur_instance_index;//the index in browser_info_ptr
    uint32  net_id;//the net id of active pdp
    BRW_PUBLIC_PROFILE_T   public_profile;
}BRW_INSTANCE_LIST_STRUCT;

/*
 *desc:	get WAP_REQUEST_T struct pointer according to request id
 *param: req_id         page request id
 *       brw_info_ptr   brw_info_t pointer
 *return:WAP_REQUEST_T struct pointer
 */
PUBLIC WAP_REQUEST_T* BrwGetWapReq(uint32 req_id,BRW_INFO_T *brw_info_ptr);
/*
 *	get instance list
 */
PUBLIC BRW_LIST_T* BrwGetInstanceList(void);
/*
 *	get wap request state
 */
PUBLIC BRW_REQ_STATE_E BrwGetWapReqState(WAP_REQUEST_T* req);
/*
 *	set wap request state
 */
PUBLIC void BrwSetWapReqState(WAP_REQUEST_T* req,BRW_REQ_STATE_E state);
/*
 *	free wap request memory
 */
PUBLIC void BrwFreeWapReqStruct(WAP_REQUEST_T* req);

#if 0 /* Reduce Code size */
/*
 *	check if cache is enabled,TRUE if enabled
 */
PUBLIC BOOLEAN BrwIsCacheEnable(BRW_INFO_T *brw_info_ptr);
/*
 *	check if cookie is enabled,TRUE if enabled
 */
PUBLIC BOOLEAN BrwIsCookieEnable(BRW_INFO_T *brw_info_ptr);
/*
 *	check if use proxy
 */
PUBLIC BOOLEAN BrwIsUseProxy(BRW_INFO_T *brw_info_ptr);
/*
 *	get proxy port
 */
PUBLIC uint16 BrwGetProxyPort(BRW_INFO_T *brw_info_ptr);
/*
 *	get proxy addr
 */
PUBLIC uint8*  BrwGetProxyAddr(BRW_INFO_T* brw_info_ptr);
#endif /* Reduce Code size */
/*
 *	if image is set to download
 */
PUBLIC BOOLEAN BrwIsImgDL(BRW_INFO_T *brw_info_ptr);

#ifdef MMIDORADO_BGSOUND_SUPPORT
/*
 *	if bgsound is set to download
 */
PUBLIC BOOLEAN BrwIsBgsoundDL(BRW_INFO_T *brw_info_ptr);
#endif

/*
 *	get user-agent string
 */
PUBLIC uint8*  BrwGetUserAgentStr(BRW_INFO_T *brw_info_ptr);
/*
 *	get uaprof string
 */
PUBLIC uint8*  BrwGetUAProfStr(BRW_INFO_T* brw_info_ptr);
/*
 *desc:	process Init Http cnf
 *param:brw_info_ptr browser instance
 *      is_success is http init success
 */
PUBLIC void BrwProcInitHttpCnf(BRW_INFO_T* brw_info_ptr,BOOLEAN is_success);
/*
 *	process error situation
 */
PUBLIC void BrwProcErr(BRW_INFO_T *brw_info_ptr,BRW_ERROR_E err);
/*
 *	process download error situation
 */
PUBLIC void BrwProcDownloadErr(BRW_INFO_T* brw_info_ptr,WAP_REQUEST_T* dl_req_ptr,BRW_ERROR_E err);
/*
 *desc:	process close Http cnf
 *param:brw_info_ptr browser instance
 */
PUBLIC void BrwProcCloseHttpCnf(BRW_INFO_T* brw_info_ptr);
/*
 *	desc：process http data，called when http data recv finished
 *  param:brw_info_ptr  browser instance object pointer
 *        req_pptr      http request object pointer's address      
 */
PUBLIC void BrwProcHttpReqDone(BRW_INFO_T *brw_info_ptr,HTTP_REQ_INFO_T** req_pptr);
/*
 *	if http task is inited
 */
PUBLIC BOOLEAN BrwIsHttpInited(BRW_INFO_T *brw_info_ptr);
/*
 *	desc:init http session
 *  param:profile_ptr relevant setting
 */
PUBLIC BOOLEAN BrwInitHttpSession(BRW_INFO_T* brw_info_ptr);
/*
 *	desc:close exist http session
 *  
 */
PUBLIC void BrwCloseHttpSession(uint32 http_context_id,BRW_PROTOCOL_TYPE_E protocol);

/*
 *	get http request list
 */
PUBLIC BRW_LIST_T* BrwGetHttpReqList(void);
/*
 *	desc:send http request
 *  param:param  request parameter,such as uri etc
 *        context_id  http context id
 *  return:new HTTP_REQ_INFO_T struct pointer
 */
PUBLIC HTTP_REQ_INFO_T* BrwSendHttpRequest(HTTP_REQ_PARAM_T *param,uint32 context_id);
/*
 *	desc:cancel a http request
 *  param:req           http request object pointer
 *        context_id    browser http context id
 */
PUBLIC void BrwCancelHttpRequest(HTTP_REQ_INFO_T* req,uint32 context_id,BRW_PROTOCOL_TYPE_E protocol);
/*
 *	desc:cancel media request which belong to brw_info_ptr and wap_req_ptr,and it's http request equal to  http_req_ptr
 */
PUBLIC void BrwCancelMediaReq(BRW_INFO_T* brw_info_ptr,WAP_REQUEST_T* wap_req_ptr,HTTP_REQ_INFO_T* http_req_ptr);
/*
 *	desc:process no memory case
 */
PUBLIC void BrwProcNoMem(BRW_INFO_T *brw_info_ptr);
/*
 *	desc:发送http auth resp消息进行鉴权
 *  参数：req_ptr  http请求指针
 *        
 */
PUBLIC void BrwHttpAuthResp(uint32 http_req_id,uint8* user_name_ptr,uint8* pwd_ptr,BOOLEAN result);
/*
 *	desc:设置http下载的内容保存的全文件名
 *  param: context_id       http 的context id
 *         http_req_ptr     http请求指针
 *         file_name_ptr    文件名
 *         file_name_len    文件名长度  
 */
PUBLIC BOOLEAN BrwSetHttpDLFileNm(uint32 context_id,HTTP_REQ_INFO_T* http_req_ptr,const uint16* file_name_ptr,uint16 file_name_len);
/*
 *	desc:进行不安全证书继续的确认
 *       http_req_id  http请求本地id
 *       result       是否继续
 */
#ifdef SSL_SUPPORT
PUBLIC void BrwHttpSSLCertResp(uint32 http_req_id,BOOLEAN result);
/*
 *	desc:处理不安全证书的确认
 *  参数：brw_info_ptr  浏览器句柄
 *        http_req_ptr  http请求指针
 */
PUBLIC BOOLEAN BrwProcSSLCert(BRW_INFO_T*   brw_info_ptr,HTTP_REQ_INFO_T*   http_req_ptr);
#endif
/*
 *	desc:free memory of http request object
 *  param:req   http request object pointer
 */
PUBLIC void BrwFreeHttpRequest(HTTP_REQ_INFO_T* req);
/*
 *	desc:get invalid context id value
 */
PUBLIC uint32 BrwGetInvalidHttpContextIDValue(void);
/*
 *	desc:get profile of BRW_INFO_T struct
 */
PUBLIC BRW_PROFILE_T*  BrwGetProfile(BRW_INFO_T *brw_info_ptr);
/*
 *	set value of  member sub_http_module_id of BRW_INFO_T
 */
PUBLIC void BrwSetSubModuleID(BRW_INFO_T *brw_info_ptr,uint32 sub_module_id);
/*
 *	get value of  member sub_http_module_id of BRW_INFO_T
 */
PUBLIC uint32 BrwGetSubModuleID(BRW_INFO_T *brw_info_ptr);
/*
 *	get http context id of brw_info_ptr
 */
PUBLIC uint32 BrwGetHttpContextID(BRW_INFO_T *brw_info_ptr);
/*
 *	set http context id of brw_info_ptr
 */
PUBLIC void BrwSetHttpContextID(BRW_INFO_T *brw_info_ptr,uint32 context_id);
/*
 *	get wap request object which oma download session id member equal to session_id
 */
PUBLIC WAP_REQUEST_T* BrwGetWapReqByDLSessionID(BRW_INFO_T* brw_info_ptr,uint32 session_id);

/*
 *	get wap request object which http request member equal to req_ptr
 */
PUBLIC WAP_REQUEST_T* BrwGetWapReqByHttpReqEx(BRW_INFO_T *brw_info_ptr,
                                            HTTP_REQ_INFO_T* req_ptr, 
                                            MEDIA_OBJ_T **media_obj_pptr//[IN]/[OUT]:if not pnull,means the http req is for a media obj
                                            );
/*
 *	get wap request object which http request member equal to req_ptr
 */
PUBLIC WAP_REQUEST_T* BrwGetWapReqByHttpReq(BRW_INFO_T *brw_info_ptr,HTTP_REQ_INFO_T* req_ptr);
/*
 *	get wap request's parent browser instance pointer
 */
PUBLIC BRW_INFO_T* BrwGetBrwInfoPtrByWapReqID(uint32 req_id);
/*
 *	get browser instance which download session with seesion_id belong to
 */
PUBLIC BRW_INFO_T*   BrwGetBrwInfoPtrByDLSessionID(uint32 session_id);
/*
 *	get browser instance which http_req_ptr belongs to
 */
PUBLIC BRW_INFO_T*   BrwGetBrwInfoPtrByHttpReq(HTTP_REQ_INFO_T* http_req_ptr);
/*
 *	check if mime_type is a webpage mime type
 */
PUBLIC BOOLEAN IsWebPageMimeType(BRW_MIME_TYPE_E mime_type);
/*
 *功能：更新进度消息
 *参数：brw_info_ptr  浏览器句柄
 *      wap_req_ptr   请求句柄
 *      received_data_len:已经收到的数据长度
 *      total_len:总的数据长度，如果无法获知，则传BRW_DEFAULT_MAX_HTTP_CONTENT_LEN
 */
PUBLIC void BrwUpdateProgressInfo(BRW_INFO_T* brw_info_ptr,WAP_REQUEST_T* wap_req_ptr,uint32 received_data_len,uint32 total_len);
/*
 *	功能：更新url信息
 *  参数：brw_info_ptr  浏览器句柄
 *        url_ptr       url指针
 */
PUBLIC void BrwUpdateUrl(BRW_INFO_T* brw_info_ptr,uint8* url_ptr);
/*
 *	处理http错误的情况
 *  参数：brw_info_ptr  浏览器句柄
 *        http_req_ptr  http请求指针
 *        err           错误类型
 */
PUBLIC void BrwProcHttpErr(BRW_INFO_T* brw_info_ptr,HTTP_REQ_INFO_T* http_req_ptr,BRW_ERROR_E err);
/*
 *	处理http鉴权
 *  参数：brw_info_ptr  浏览器句柄
 *        http_req_ptr  http请求指针
 */
PUBLIC BOOLEAN BrwProcHttpAuth(BRW_INFO_T* brw_info_ptr,HTTP_REQ_INFO_T* http_req_ptr);
/*
 *	desc:应用输入保存的文件名后的处理
 *  param: wap_req_id       请求id
 *         file_name_ptr    文件名(全路径,unicode)
 *         file_name_len    文件名长度
 */
PUBLIC void BrwEnterFileNmCbf(uint32 wap_req_id,const uint16* file_name_ptr,uint16 file_name_len, BOOLEAN result);

#ifdef OMADL_SUPPORT
/*
 *	desc:DD类型文件,应用输入保存的文件名后的处理
 *  param: wap_req_id       请求id
 *         file_name_ptr    文件名(全路径,unicode)
 *         file_name_len    文件名长度
 */
PUBLIC void BrwEnterDdFileNmCbf(uint32 wap_req_id,const uint16* file_name_ptr,uint16 file_name_len, BOOLEAN result);
#endif
/*
 *	desc:oma download 应用输入文件名确认后的处理
 *  param: wap_req_id       请求id
 *         file_name_ptr    文件名(全路径,unicode)
 *         file_name_len    文件名长度
 */
PUBLIC void BrwOMADLConfirmCbf(uint32 wap_req_id,const uint16* file_name_ptr,uint16 file_name_len,BOOLEAN result);
/*
 *	desc:process http task's signal
 *  param:msg_id  message id
 *        param   message parameter
 *  TRUE if msg is http's message,else FALSE
 */
PUBLIC BOOLEAN BrwProcHttpMsg(uint32 msg_id,void* param);
/*
 *	desc:to start a new page, this function is called by RENDER
 */
PUBLIC BOOLEAN BrwStartNewPageReq(const LAYOUT_REQ_PARAM_T* param_ptr,const uint32 control_handler);

/*
 *	start new page request to open a new instance
 */
PUBLIC BOOLEAN BrwStartNewInstance(const LAYOUT_REQ_PARAM_T* param_ptr,const uint32 control_handler);

/*
 *	append a new media request
 */
PUBLIC BOOLEAN BrwAddNewMediaReq(const uint8 *url_ptr,
                                 MEDIA_REQ_TYPE_E          meida_type,
                                 const uint32              control_handler,
                                 const uint32              layout_handle);
/*
 *	desc:to set the layout handler of the snapshot
 */
PUBLIC void BrwSetSnapshotOpenLayoutHandler(BRW_INSTANCE instance,LAYOUT_HANDLE handler);
/*
 *	desc:to clear cache
 */
PUBLIC BOOLEAN BrwClearCache(uint16 *cache_path_ptr, BRW_PROTOCOL_TYPE_E protocol);
/*
 *	desc:to clear cookie
 */
PUBLIC BOOLEAN BrwClearCookie(uint16 *cookie_path_ptr, BRW_PROTOCOL_TYPE_E protocol,uint32 http_context_id);
/*
 *	desc:set cookie
 */
PUBLIC BOOLEAN BrwSetCookie(uint32 http_context_id, uint8 *cookie_ptr);
/*
 *	desc:to get the brw info by brw instance id
 */
PUBLIC BRW_INFO_T* BrwGetBrwInfoPtrByInstance(BRW_INSTANCE instance);

/*
*   desc:get the path of cookie(actually the cookie full path name)
*   param:
*   return:result
*/
PUBLIC BOOLEAN BrwGetCookiePath(
                uint16 *cookie_path_ptr,//[in/out]
                uint16   *cookie_path_len_ptr  //[in/out], 双字节为单位(the max len of cookie_path_ptr)
                );

/*
*   desc:get the path of cache
*   param:
*   return:result
*/
PUBLIC BOOLEAN BrwGetCachePath(
                uint16 *cache_path_ptr,//[in/out]
                uint16   *cache_path_len_ptr  //[in/out], 双字节为单位(the max len of cache_path_ptr)
                );

/*
*   desc:get the path of cache for fixed
*   param:
*   return:result
*/
PUBLIC BOOLEAN BrwGetFixedCachePath(
                uint16 *cache_path_ptr,//[in/out]
                uint16   *cache_path_len_ptr  //[in/out], 双字节为单位(the max len of cache_path_ptr)
                );

/*
 *	desc:to backup request body for POST
 */
PUBLIC HTTP_BODY_DATA_T* BrwBackupRequestBody(const HTTP_BODY_DATA_T* src_body_ptr);
/*
 *	desc:to free the backup request body
 */
PUBLIC void BrwFreeRequestBody(HTTP_BODY_DATA_T* body_ptr);
/*
 *	desc:to get wether the cache is using
 */
PUBLIC BOOLEAN BrwIsCacheUsing(WAP_REQUEST_T* cached_wap_req_ptr);
/*
 *	desc:to set the http init state
 */
PUBLIC void BrwSetHttpState(BRW_INFO_T *brw_info_ptr, BRW_HTTP_STATE_E http_state);

/****************************************************************************/
//Description : to backup the wap request, including the info of: http page data
//				layout, url
//Global resource dependence : 
//Author: juan.zhang
/****************************************************************************/
PUBLIC WAP_REQUEST_T* BrwBackupWapRequest(WAP_REQUEST_T *req);

/***************************************************************************************/
/***GOOGLE_URL+GOOGLE_QUERY+src_url_ptr+&+MBROWSER_ID+&+FISRT_PAGE_ID+&+GOOGLE_LANG_CH**/
/***************************************************************************************/
PUBLIC char* BrwGwtChangeUrl(char *src_url_ptr,BRW_LANG_E lang);

PUBLIC BOOLEAN BrwGwtIsUsingGwt(char *url_ptr);

PUBLIC BRW_INFO_T *BrwGetCurInstanceInfo(void);
PUBLIC BRW_INFO_T* BRW_SearchInstance(BRW_LIST_CMP_F cmp, void* object_ptr);
PUBLIC BOOLEAN BrwStartPageRequest(BRW_INFO_T	*brw_info_ptr);

#endif
