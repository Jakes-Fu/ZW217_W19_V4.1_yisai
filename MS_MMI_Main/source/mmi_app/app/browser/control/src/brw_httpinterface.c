/*****************************************************************
 **  File Name: 	brw_httpinterface.c							 *
 **  Author:		            								 *
 **  Date:		         										 *
 **  Description:implemention of http interface                  *
******************************************************************
******************************************************************
**  Edit History											     *
**---------------------------------------------------------------*
 **  DATE			NAME			DESCRIPTION				     *
 **  04/01/2008		yuzhi.chen      Create. 					 *
*****************************************************************/
#include "mmi_app_browser_trc.h"
#include "brw_utility.h"
#include "brw_control_api.h"
#include "brw_control.h"
#include "sfs.h"
#include "in_message.h"
#include "http_api.h"
#include "wsp_api.h"
#include "brw_dlinterface.h"
#include "mmibrowser_wintable.h"
#if defined(MMI_UPDATE_TIME_SUPPORT)
#include "mmiaut_export.h"
#endif
#ifdef DL_SUPPORT
#include "mmidl_export.h"
#endif
#define BRW_HTTP_MODULE_WAP_ID      MMIBRW_MODULE_ID		//browser module id

#define BRW_INVALID_SUBMODULE_ID    0x0	
#define BRW_INVALID_HTTP_REQ_ID          0x0
//#define BRW_DEFAULT_HTTP_ACCEPT_HEADER "text/vnd.wap.wml,application/vnd.wap.xhtml+xml,text/html,image/*,*/*" //淘宝登陆图片icon格式
#ifdef DRM_SUPPORT//MS00240328
#define BRW_DEFAULT_HTTP_ACCEPT_HEADER "text/vnd.wap.wml,application/vnd.wap.xhtml+xml,application/xhtml+xml,\
image/vnd.wap.wbmp,image/gif,image/jpg,image/jpeg,image/png,image/bmp,text/html,\
application/vnd.oma.drm.message,application/vnd.oma.drm.content,application/vnd.oma.drm.rights+xml,application/vnd.oma.drm.rights+wbxml,\
*/*;q=0.1"
#else
#if defined(JAVA_SUPPORT) && defined(CMCC_UI_STYLE)
#define BRW_DEFAULT_HTTP_ACCEPT_HEADER "text/vnd.wap.wml,application/vnd.wap.xhtml+xml,application/xhtml+xml, image/vnd.wap.wbmp, image/gif, image/jpg,image/jpeg, image/png, image/bmp, text/html, text/vnd.sun.j2me.app-descriptor,*/*;q=0.1"
#else
#define BRW_DEFAULT_HTTP_ACCEPT_HEADER "text/vnd.wap.wml,application/vnd.wap.xhtml+xml,application/xhtml+xml, image/vnd.wap.wbmp, image/gif, image/jpg,image/jpeg, image/png, image/bmp, text/html,*/*;q=0.1"
#endif
#endif
#define BRW_DEFAULT_HTTP_ACCEPT_CHARSET "UTF-8,ISO-8859-1,US-ASCII,windows-1252"

#define BRW_HTTP_STATUS_200 200
#define BRW_PER_ACCEPT_LANGUAGE_MAX 12//the max len of ACCEPT_LANGUAGE_HEADER_T's language_header_ptr and with ","
#define TOTAL_LANGUAGE_HEADER (sizeof(s_accept_language_header) / sizeof(s_accept_language_header[0]))

typedef struct
{
    MMISET_LANGUAGE_TYPE_E  language_type;
    char    *language_header_ptr;
}ACCEPT_LANGUAGE_HEADER_T;

//accept-language
LOCAL ACCEPT_LANGUAGE_HEADER_T s_accept_language_header[] = {
    {MMISET_LANGUAGE_ENGLISH, "en"}
#if defined(IM_SIMP_CHINESE_SUPPORT) || defined(IM_CHINESE_S_SUPPORT)
    ,{MMISET_LANGUAGE_SIMP_CHINESE, "zh-cn"}
#endif
#if defined(IM_TRAD_CHINESE_SUPPORT) || defined(IM_CHINESE_T_SUPPORT)
    ,{MMISET_LANGUAGE_TRAD_CHINESE, "zh-tw"}
#endif
#ifdef IM_ARABIC_SUPPORT
    ,{MMISET_LANGUAGE_ARABIC, "ar"}
#endif
#ifdef IM_FRENCH_SUPPORT
    ,{MMISET_LANGUAGE_FRENCH, "fr"}
#endif
#ifdef IM_GERMAN_SUPPORT
    ,{MMISET_LANGUAGE_GERMAN, "de"}
#endif
#ifdef IM_GREEK_SUPPORT
    ,{MMISET_LANGUAGE_GREEK, "el"}
#endif
#ifdef IM_HINDI_SUPPORT
    ,{MMISET_LANGUAGE_HINDI, "hi"}
#endif
#ifdef IM_HUNGARIAN_SUPPORT
    ,{MMISET_LANGUAGE_HUNGARIAN, "hu"}
#endif
#ifdef IM_INDONESIAN_SUPPORT
    ,{MMISET_LANGUAGE_INDONESIAN, "id"}
#endif
#ifdef IM_ITALIAN_SUPPORT
    ,{MMISET_LANGUAGE_ITALIAN, "it"}
#endif
#ifdef IM_MALAY_SUPPORT
    ,{MMISET_LANGUAGE_MALAY, "ms"}
#endif
#ifdef IM_PERSIAN_SUPPORT
    ,{MMISET_LANGUAGE_PERSIAN, "fa"}
#endif
#ifdef IM_PORTUGUESE_SUPPORT
    ,{MMISET_LANGUAGE_PORTUGUESE, "pt"}
    ,{MMISET_LANGUAGE_PORTUGUESE, "pt-bz"}
#endif
#ifdef IM_RUSSIAN_SUPPORT	
    ,{MMISET_LANGUAGE_RUSSIAN, "ru"}
#endif
#ifdef IM_SPANISH_SUPPORT
    ,{MMISET_LANGUAGE_SPANISH, "es"}
#endif
#ifdef IM_TAGALOG_SUPPORT
    ,{MMISET_LANGUAGE_TAGALOG, "tl"}
#endif
#ifdef IM_THAI_SUPPORT	
    ,{MMISET_LANGUAGE_THAI, "th"}
#endif
#ifdef IM_TURKISH_SUPPORT
    ,{MMISET_LANGUAGE_TURKISH, "tr"}
#endif
#ifdef IM_URDU_SUPPORT
    ,{MMISET_LANGUAGE_URDU, "ur"}
#endif
#ifdef IM_VIETNAMESE_SUPPORT
    ,{MMISET_LANGUAGE_VIETNAMESE, "vi"}
#endif
};
LOCAL char  *s_accept_language_ptr = PNULL;


LOCAL uint32 g_brw_next_submodule_id=1;		//used for generate browser sub module id
LOCAL BRW_LIST_T    g_http_req_list={0};	//to record the existing http request
LOCAL uint32 g_http_next_id = 1;			//used for generate the http request id
/*----------------LOCAL function declaration  begin--------------*/
LOCAL uint32 GenUniqueSubModuleID(void);
LOCAL BRW_INFO_T* BrwGetInstanceBySubModuleID(uint32 sub_module_id);
LOCAL BRW_INFO_T* GetInstanceByContextID(uint32 context_id);
LOCAL BOOLEAN   CompareSubModuleID(BRW_INFO_T *brw_info_ptr,uint32 sub_module_id);
LOCAL BOOLEAN   CompareHttpContextID(BRW_INFO_T *brw_info_ptr,uint32 http_context_id);
LOCAL void ProcInitSessionCnf(HTTP_CONTEXT_ID_T context_id,uint32 sub_module_id,HTTP_ERROR_E result);
LOCAL void ProcCloseSessionCnf(HTTP_CONTEXT_ID_T context_id);
LOCAL void ProcHeaderInd(HTTP_HEADER_IND_SIG_T* ind);
LOCAL void ProcErrInd(HTTP_ERROR_IND_SIG_T  *ind_ptr);
LOCAL void ProcRequestIDInd(HTTP_REQUEST_ID_IND_SIG_T* ind_ptr);
LOCAL void ProcNetProgressInd(HTTP_NET_PROG_IND_SIG_T* ind_ptr);
LOCAL void ProcRedirectInd(HTTP_REDIRECT_IND_SIG_T* ind_ptr);
LOCAL void ProcGetCnf(HTTP_GET_CNF_SIG_T *cnf);
LOCAL void ProcPostCnf(HTTP_POST_CNF_SIG_T *cnf);
LOCAL uint32 GetNextHttpReqID(void);
LOCAL BOOLEAN CompareReqID(HTTP_REQ_INFO_T* req_ptr,uint32 id);
LOCAL void ProcHttpErr(BRW_INFO_T* brw_info_ptr,HTTP_REQ_INFO_T* http_req_ptr,HTTP_ERROR_E err);
LOCAL BOOLEAN SetGetRequestParam(
					   HTTP_REQ_PARAM_T *param,//[IN]
					   BRW_PROTOCOL_TYPE_E protocol,//[IN]
					   void *get_param_ptr //[OUT]
					   );
LOCAL BOOLEAN SetPostRequestParam(
					   HTTP_REQ_PARAM_T *param,//[IN]
					   BRW_PROTOCOL_TYPE_E protocol,//[IN]
					   void *post_param_ptr //[OUT]
					   );
LOCAL void SendAuthRsp(
					   uint8* user_name_ptr,
					   uint8* pwd_ptr,
					   BRW_PROTOCOL_TYPE_E protocol,
					   HTTP_REQ_INFO_T *http_req_ptr
					   );

LOCAL void GetAcceptLanguageHeader(
            char **accept_language_pptr//[IN\OUT]:*accept_language_pptr need free by FreeAcceptLanguageHeader
            );
LOCAL void FreeAcceptLanguageHeader(
            char **accept_language_pptr//[IN\OUT]:free *accept_language_pptr
            );

extern BRW_INSTANCE_LIST_STRUCT g_brw_instance_list_info;
/*----------------LOCAL function declaration  end----------------*/
/*
 *	desc:init http session
 *  param:brw_info_ptr BRW_INFO_T struct
 */
PUBLIC BOOLEAN BrwInitHttpSession(BRW_INFO_T* brw_info_ptr)
{
    HTTP_INIT_PARAM_T http_init_param={0};
    WSP_INIT_PARAM_T wsp_init_param={0};
    uint32   sub_module_id = BRW_INVALID_SUBMODULE_ID;
    BRW_PUBLIC_PROFILE_T *profile_ptr = BrwGetPublicProfile();
    uint16  cache_path[BRW_FFS_MAX_PATH + 1] = {0};
    uint16  cache_path_len = BRW_FFS_MAX_PATH;
    BOOLEAN is_get_cache_path = FALSE;
    uint16  cookie_path[BRW_FFS_MAX_PATH + 1] = {0};
    uint16  cookie_path_len = BRW_FFS_MAX_PATH;
    BOOLEAN is_get_cookie_path = FALSE;

    if(PNULL == profile_ptr)
    {
        BRWDebug_PrintWarning("BrwInitHttpSession:profile_ptr null!");
        return FALSE;
    }

    if (!brw_info_ptr->is_fixed)
    {
        is_get_cache_path = BrwGetCachePath(cache_path, &cache_path_len);
    }
    else            
    {
        is_get_cache_path = BrwGetFixedCachePath(cache_path, &cache_path_len);
    }
    
    is_get_cookie_path = BrwGetCookiePath(cookie_path, &cookie_path_len);
   
    //BRWDebug_Printf:"BrwInitHttpSession"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_200_112_2_18_2_7_17_670,(uint8*)"");
    if (BRW_PROTOCOL_HTTP == profile_ptr->protocol)
    {
        BRW_Memset(&http_init_param,0,sizeof(HTTP_INIT_PARAM_T));
        http_init_param.net_id = g_brw_instance_list_info.net_id;

        if (!brw_info_ptr->is_fixed)
        {
            http_init_param.is_cache_enable = profile_ptr->is_cache_enable;
        }
        else            
        {
            http_init_param.is_cache_enable = TRUE;
        }
        if(http_init_param.is_cache_enable && is_get_cache_path)
        {
            BRW_WstrCat(http_init_param.cache_path, cache_path);
        }
        
        http_init_param.is_cookie_enable = profile_ptr->is_cookie_enable;
        if (http_init_param.is_cookie_enable && is_get_cookie_path)
        {
            BRW_WstrCat(http_init_param.cookie_file_name, cookie_path);
        }
        SCI_TRACE_LOW("BRW:BrwInitHttpSession is_cookie_enable:%d,is_get_cookie_path:%d\n",http_init_param.is_cookie_enable,is_get_cookie_path);
        
        if(profile_ptr->proxy_opt.is_use_proxy)
        {
            http_init_param.proxy.is_use_proxy=TRUE;
            http_init_param.proxy.http_port=profile_ptr->proxy_opt.proxy_port;
            http_init_param.proxy.proxy_host_ptr=(char*)profile_ptr->proxy_opt.proxy_addr_ptr;
            http_init_param.proxy.password_ptr = profile_ptr->psw_ptr;
            //SCI_TRACE_LOW:"BRW PASSWORD =%s"
            if (profile_ptr->psw_ptr != 0)
            {
            	SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_231_112_2_18_2_7_17_671,(uint8*)"s",profile_ptr->psw_ptr);
            }

            http_init_param.proxy.user_name_ptr = profile_ptr->username_ptr;
            //SCI_TRACE_LOW:"BRW user name =%s"
            if (profile_ptr->username_ptr != 0)
            {
            	SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_233_112_2_18_2_7_17_672,(uint8*)"s",profile_ptr->username_ptr);
            }
        }   

        http_init_param.time_zone = MMIAPIACC_GetLocalTimeZone();

        sub_module_id = GenUniqueSubModuleID();
        BrwSetSubModuleID(brw_info_ptr,sub_module_id);
        BrwSetHttpState(brw_info_ptr, BRW_HTTP_STATE_INITING);
        HTTP_InitRequest(BRW_HTTP_MODULE_WAP_ID,sub_module_id,&http_init_param);
   }
   else if (BRW_PROTOCOL_WAP == profile_ptr->protocol)
   {
        BRW_Memset(&wsp_init_param,0,sizeof(WSP_INIT_PARAM_T));
        
        wsp_init_param.net_id = g_brw_instance_list_info.net_id;
        
        if (!brw_info_ptr->is_fixed)
        {
            wsp_init_param.is_cache_enable = profile_ptr->is_cache_enable;
        }
        else            
        {
            wsp_init_param.is_cache_enable = TRUE;
        }
        if (wsp_init_param.is_cache_enable && is_get_cache_path)
        {
            BRW_WstrCat(wsp_init_param.cache_path,cache_path);
        }
        
        wsp_init_param.is_cookie_enable=profile_ptr->is_cookie_enable;
        if (wsp_init_param.is_cookie_enable && is_get_cookie_path)
        {
            BRW_WstrCat(wsp_init_param.cookie_file_name,cookie_path);
        }
        SCI_TRACE_LOW("BRW:BrwInitHttpSession wsp_init_param.is_cookie_enable:%d,is_get_cookie_path:%d\n",wsp_init_param.is_cookie_enable,is_get_cookie_path);
        
        if(profile_ptr->proxy_opt.is_use_proxy)
        {
            wsp_init_param.proxy.wsp_port=profile_ptr->proxy_opt.proxy_port;
            wsp_init_param.proxy.proxy_host_ptr=(char*)profile_ptr->proxy_opt.proxy_addr_ptr;
            wsp_init_param.proxy.password_ptr = profile_ptr->psw_ptr;
            wsp_init_param.proxy.user_name_ptr = profile_ptr->username_ptr;        
        }
//	   else
//	   {
//		   BRWDebug_Printf("BrwInitHttpSession error,wsp need proxy!!");
//		   return FALSE;
//	   }
        sub_module_id = GenUniqueSubModuleID();
        BrwSetSubModuleID(brw_info_ptr,sub_module_id);
        BrwSetHttpState(brw_info_ptr, BRW_HTTP_STATE_INITING);
        WSP_InitRequest(BRW_HTTP_MODULE_WAP_ID,sub_module_id,&wsp_init_param);
    }

   return TRUE;
}
/*
 *	desc:close exist http session
 *  
 */
PUBLIC void BrwCloseHttpSession(uint32 http_context_id,BRW_PROTOCOL_TYPE_E protocol)
{
    if(http_context_id != BrwGetInvalidHttpContextIDValue())
    {
        //BRWDebug_Printf:"BrwCloseHttpSession"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_292_112_2_18_2_7_17_673,(uint8*)"");
        if (protocol == BRW_PROTOCOL_HTTP)
        {
            HTTP_CloseRequest((HTTP_CONTEXT_ID_T)http_context_id);
        }
        else if (protocol == BRW_PROTOCOL_WAP)
        {
            WSP_CloseRequest((HTTP_CONTEXT_ID_T)http_context_id);
        }
        else
        {
            //BRWDebug_Printf:"[BRW]BrwCloseHttpSession: unsupported protocol"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_303_112_2_18_2_7_17_674,(uint8*)"");
        }
    }  
}
/*
 *desc:process cnf of BrwStartHttpSession
 *param: context_id  http session context id
 *       result      request result
 */
LOCAL void ProcInitSessionCnf(HTTP_CONTEXT_ID_T context_id,uint32 sub_module_id,HTTP_ERROR_E result)
{
    BRW_INFO_T  *brw_info_ptr=PNULL;
    brw_info_ptr = BrwGetInstanceBySubModuleID(sub_module_id);

    if (HTTP_SUCCESS == result)
    {
        BrwSetHttpContextID(brw_info_ptr,context_id);
        BrwProcInitHttpCnf(brw_info_ptr,TRUE);
    }     
    else
    {
        BrwProcInitHttpCnf(brw_info_ptr,FALSE);
    }
}
/*
 *desc:process cnf of BrwCloseHttpSession
 *param: context_id  http session context id
 */
LOCAL void ProcCloseSessionCnf(HTTP_CONTEXT_ID_T context_id)
{
    BRW_INFO_T  *brw_info_ptr=PNULL;
    
    brw_info_ptr = GetInstanceByContextID(context_id);
    BrwProcCloseHttpCnf(brw_info_ptr);

    if (PNULL != brw_info_ptr)
    {
        brw_info_ptr->http_context_id = 0;
        if (brw_info_ptr->is_need_to_clear_cookie)
        {
            BRW_ClearCookie(brw_info_ptr->instance_id);
            brw_info_ptr->is_need_to_clear_cookie = FALSE;
        }
    }
}
/*
 *	desc:process err_ind of http
 *  ind_ptr: err_ind消息指针
 */
LOCAL void ProcErrInd(HTTP_ERROR_IND_SIG_T  *ind_ptr)
{
    //BRWDebug_Printf:"[BRW]BrwProcErrInd"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_353_112_2_18_2_7_17_675,(uint8*)"");
    if (ind_ptr != PNULL)
    {
        BRW_INFO_T* brw_info_ptr=PNULL;
        HTTP_REQ_INFO_T*    http_req_ptr=PNULL;     
        uint32 context_id    = ind_ptr->context_id;
        uint32 req_id = (uint32)ind_ptr->app_instance;

        http_req_ptr = (HTTP_REQ_INFO_T*)BRWList_SearchObject(&g_http_req_list,(BRW_LIST_CMP_F)CompareReqID,(void*)req_id);
        if (PNULL == http_req_ptr)
        {
           //可能是之前已经发出过cancel请求的
           //BRWDebug_Printf:"[BRW]BrwProcErrInd http_req_ptr null"
           SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_365_112_2_18_2_7_17_676,(uint8*)"");
           return;
        }       
        
        brw_info_ptr = GetInstanceByContextID(context_id);
        if (PNULL == brw_info_ptr)
        {
           BRWDebug_PrintWarning("BrwProcErrInd:brw_info_ptr null");
           return;
        }
        ProcHttpErr(brw_info_ptr,http_req_ptr,ind_ptr->result);
    }
}
/*
 *	desc:process HTTP_REQUEST_ID_IND
 */
LOCAL void ProcRequestIDInd(HTTP_REQUEST_ID_IND_SIG_T* ind_ptr)
{
    if (ind_ptr != PNULL)
    {
        BRW_INFO_T  *brw_info_ptr=PNULL;
        HTTP_REQ_INFO_T *http_req_ptr=PNULL;
        uint32 req_id = (uint32)ind_ptr->app_instance;
        uint32 context_id = ind_ptr->context_id;
        BRW_PUBLIC_PROFILE_T    *public_profile_ptr = BrwGetPublicProfile();
            
        http_req_ptr = (HTTP_REQ_INFO_T*)BRWList_SearchObject(&g_http_req_list,(BRW_LIST_CMP_F)CompareReqID,(void*)req_id);
        if(PNULL == http_req_ptr)
        {
            //可能是之前已经释放掉了
            //BRWDebug_Printf:"[BRW]W ProcRequestIDInd http_req_ptr null"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_394_112_2_18_2_7_17_677,(uint8*)"");

            /*MS00218222:Protect,在协议上取消这个已经被释放掉的http request*/
            brw_info_ptr = BrwGetBrwInfoPtrByInstance(MMIBROWSER_GetInstance());
        
            //BRWDebug_Printf:"[BRW]ProcRequestIDInd request_id=0x%x, context_id=0x%x, app_instance=0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_400_112_2_18_2_7_17_678,(uint8*)"ddd", ind_ptr->request_id, ind_ptr->context_id, ind_ptr->app_instance);
            if (brw_info_ptr != PNULL)
            {
                if (BRW_PROTOCOL_HTTP == public_profile_ptr->protocol)
                {
                    HTTP_CancelRequest(context_id, PNULL, ind_ptr->request_id);      
                }
                else if (BRW_PROTOCOL_WAP == public_profile_ptr->protocol)
                {
                    WSP_CancelRequest(context_id, PNULL, ind_ptr->request_id);
                }
                //BRWDebug_Printf:"[BRW]ProcRequestIDInd profile.protocol =%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_411_112_2_18_2_7_17_679,(uint8*)"d", public_profile_ptr->protocol);
            }
            else
            {
                //BRWDebug_Printf:"[BRWW ProcRequestIDInd brw_info_ptr null"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_415_112_2_18_2_7_17_680,(uint8*)"");
            }     
            return;
        }       
        http_req_ptr->http_request_id = ind_ptr->request_id;
    }
}
/*
 *	process HTTP_NET_PROG_IND
 */
LOCAL void ProcNetProgressInd(HTTP_NET_PROG_IND_SIG_T* ind_ptr)
{
    if (ind_ptr != PNULL)
    {
        BRW_INFO_T  *brw_info_ptr=PNULL;
        HTTP_REQ_INFO_T *http_req_ptr=PNULL;
        WAP_REQUEST_T   *wap_req_ptr=PNULL;
        uint32 req_id = (uint32)ind_ptr->app_instance;
        uint32 context_id = ind_ptr->context_id;

        http_req_ptr = (HTTP_REQ_INFO_T*)BRWList_SearchObject(&g_http_req_list,(BRW_LIST_CMP_F)CompareReqID,(void*)req_id);
        if (PNULL == http_req_ptr)
        {
            //可能是之前已经释放掉了
            //BRWDebug_Printf:"[BRW]ProcNetProgressInd http_req_ptr null"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_438_112_2_18_2_7_17_681,(uint8*)"");
            return;
        }  
        
        brw_info_ptr = GetInstanceByContextID(context_id);
        if (PNULL == brw_info_ptr)
        {
           BRWDebug_PrintWarning("ProcNetProgressInd:brw_info_ptr null");
           return;
        }
        
        wap_req_ptr=BrwGetWapReqByHttpReq(brw_info_ptr,http_req_ptr);
//        if(ind_ptr->content_len==0)
//        {
//            ind_ptr->content_len=BRW_DEFAULT_MAX_HTTP_CONTENT_LEN;
//        }
        if(BrwGetWapReqState(wap_req_ptr) != BRW_STATE_MEDIA_REQUESTING)
        {
            BrwUpdateProgressInfo(brw_info_ptr,wap_req_ptr,ind_ptr->current_len,ind_ptr->content_len);
        }        
    }
}
/*
 *	process HTTP_REDIRECT_IND
 */
LOCAL void ProcRedirectInd(HTTP_REDIRECT_IND_SIG_T* ind_ptr)
{
    if (ind_ptr != PNULL)
    {
        BRW_INFO_T  *brw_info_ptr=PNULL;        
        HTTP_REQ_INFO_T *http_req_ptr=PNULL;
        WAP_REQUEST_T   *wap_req_ptr=PNULL;
        uint32 req_id = (uint32)ind_ptr->app_instance;
        uint32 context_id = ind_ptr->context_id;

        http_req_ptr = (HTTP_REQ_INFO_T*)BRWList_SearchObject(&g_http_req_list,(BRW_LIST_CMP_F)CompareReqID,(void*)req_id);
        if (PNULL == http_req_ptr)
        {
           //可能是之前已经释放掉了
           //BRWDebug_Printf:"[BRW]ProcRedirectInd http_req_ptr null"
           SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_476_112_2_18_2_7_17_682,(uint8*)"");
           return;
        } 
        
        brw_info_ptr = GetInstanceByContextID(context_id);
        if (PNULL ==brw_info_ptr)
        {
           BRWDebug_PrintWarning("ProcRedirectInd:brw_info_ptr null");
           return;
        }
        wap_req_ptr=BrwGetWapReqByHttpReq(brw_info_ptr,http_req_ptr);
        if(wap_req_ptr != PNULL)
        {
            uint32 url_len = 0;
            BRW_REQ_STATE_E state = BrwGetWapReqState(wap_req_ptr);

            if (BRW_STATE_PAGE_REQUESTING == state)
            {
                BRWMem_Free(wap_req_ptr->absolute_url);
                wap_req_ptr->absolute_url = PNULL;
                
                url_len = BRW_Strlen((const uint8*)ind_ptr->redirect_ptr);
                wap_req_ptr->absolute_url = BRWMem_Alloc(url_len+1);
                if (wap_req_ptr->absolute_url != PNULL)
                {
                    BRW_Memcpy(wap_req_ptr->absolute_url, ind_ptr->redirect_ptr, url_len);
                    *(wap_req_ptr->absolute_url+url_len) = 0;
                    if (!brw_info_ptr->is_fixed)
                    {
                        BrwUpdateUrl(brw_info_ptr,wap_req_ptr->absolute_url);
                    }
                }//if(wap_req_ptr->absolute_url!=PNULL)
                else
                {
                    BrwProcErr(brw_info_ptr,BRW_ERR_NO_MEM);
                }
            }//if(state==BRW_STATE_PAGE_REQUESTING)            
        }             
    }
}
/*
 *	desc:process auth_ind of http
 *  ind_ptr: auth_ind消息指针
 */
LOCAL void ProcNeedAuthInd(HTTP_AUTH_IND_SIG_T* ind_ptr)
{
    //BRWDebug_Printf:"[BRW]ProcNeedAuthInd"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_518_112_2_18_2_7_17_683,(uint8*)"");

    if (ind_ptr != PNULL)
    {
        BRW_INFO_T  *brw_info_ptr=PNULL;
        HTTP_REQ_INFO_T *http_req_ptr=PNULL;     
        uint32 context_id = ind_ptr->context_id;
        uint32 req_id = (uint32)ind_ptr->app_instance;

        http_req_ptr = (HTTP_REQ_INFO_T*)BRWList_SearchObject(&g_http_req_list,(BRW_LIST_CMP_F)CompareReqID,(void*)req_id);
        
        brw_info_ptr = GetInstanceByContextID(context_id);
        if (PNULL == brw_info_ptr)
        {
            BRWDebug_PrintWarning("ProcNeedAuthInd:brw_info_ptr null");
            return;
        }

        if (PNULL == http_req_ptr)
        {
            //可能是之前已经发出过cancel请求的
            //BRWDebug_Printf:"[BRW]ProcNeedAuthInd http_req_ptr null"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_539_112_2_18_2_7_17_684,(uint8*)"");
            return;
        } 
        
        if(!BrwProcHttpAuth(brw_info_ptr,http_req_ptr))
        {
            BrwHttpAuthResp(req_id,PNULL,PNULL,FALSE);
        }
    }
}
LOCAL BOOLEAN SetGetRequestParam(
					   HTTP_REQ_PARAM_T *param_ptr,//[IN]
					   BRW_PROTOCOL_TYPE_E protocol,//[IN]
					   void *get_param_ptr //[OUT]
					   )
{    
    if((PNULL == param_ptr) || (PNULL == get_param_ptr))
    {
        return FALSE;
    }
    
    switch(protocol)
    {
    case BRW_PROTOCOL_HTTP:
    {
        HTTP_GET_PARAM_T *http_get_param_ptr = (HTTP_GET_PARAM_T*)get_param_ptr;

        BRW_Memset(http_get_param_ptr,0,sizeof(HTTP_GET_PARAM_T));
        http_get_param_ptr->accept.accept_ptr = BRW_DEFAULT_HTTP_ACCEPT_HEADER;
        http_get_param_ptr->accept_charset.accept_charset_ptr = BRW_DEFAULT_HTTP_ACCEPT_CHARSET;
        http_get_param_ptr->accept_language.accept_language_ptr = s_accept_language_ptr;
#if (defined BROWSER_GZIP_SUPPORT) && (defined GZIP_DEC_SUPPORT_GZIP_SW)
        http_get_param_ptr->accept_encoding = HTTP_ENCODING_TYPE_GZIP;//HTTP_ENCODING_TYPE_GZIP;
#else
        http_get_param_ptr->accept_encoding = HTTP_ENCODING_TYPE_NULL;
#endif
        //			if(param_ptr->is_sub_media_req)
        //			{
        //				//子媒体请求可以直接确定以buffer方式传输
        //				http_get_param_ptr->get_data.style = HTTP_DATA_TRANS_STYLE_BUFFER;
        //				http_get_param_ptr->need_net_prog_ind=FALSE;
        //			}
        //			else
        {
            http_get_param_ptr->get_data.style =HTTP_DATA_TRANS_STYLE_UNKNOW;
            http_get_param_ptr->need_net_prog_ind=TRUE;
        }
        http_get_param_ptr->referer.referer_ptr = (char*)param_ptr->refer_ptr;
        http_get_param_ptr->ua_profile.ua_profile_ptr = (char*)param_ptr->ua_prof_ptr;
        http_get_param_ptr->uri.uri_ptr = (char*)param_ptr->uri_ptr;
        http_get_param_ptr->user_agent.user_agent_ptr = (char*)param_ptr->user_agent_ptr;
        http_get_param_ptr->connection = HTTP_CONNECTION_KEEP_ALIVE;
        http_get_param_ptr->is_use_relative_url = param_ptr->is_using_relative_url;

        http_get_param_ptr->auth.password_ptr = MMIBROWSER_GetPassword();
        http_get_param_ptr->auth.user_name_ptr = MMIBROWSER_GetUserName();
#ifdef HTTP2_SUPPORT
        http_get_param_ptr->is_need_check_protocol = TRUE;
#endif
    }
        break;
        
    case BRW_PROTOCOL_WAP:
    {
        WSP_GET_PARAM_T *wsp_get_param_ptr = (WSP_GET_PARAM_T*)get_param_ptr;

        BRW_Memset(wsp_get_param_ptr, 0, sizeof(WSP_GET_PARAM_T));
        wsp_get_param_ptr->accept.accept_ptr = BRW_DEFAULT_HTTP_ACCEPT_HEADER;
        //MS00215784:add accept_charset, accept_language
        wsp_get_param_ptr->accept_charset.accept_charset_ptr = BRW_DEFAULT_HTTP_ACCEPT_CHARSET;
        wsp_get_param_ptr->accept_language.accept_language_ptr = s_accept_language_ptr;

#if (defined BROWSER_GZIP_SUPPORT) && (defined GZIP_DEC_SUPPORT_GZIP_SW)
        wsp_get_param_ptr->accept_encoding = HTTP_ENCODING_TYPE_GZIP;//HTTP_ENCODING_TYPE_GZIP;
#else
        wsp_get_param_ptr->accept_encoding = HTTP_ENCODING_TYPE_NULL;
#endif
        if(param_ptr->is_sub_media_req)
        {
            wsp_get_param_ptr->need_net_prog_ind=FALSE;
        }
        else
        {
            wsp_get_param_ptr->need_net_prog_ind=TRUE;
        }
        wsp_get_param_ptr->referer.referer_ptr = (char*)param_ptr->refer_ptr;
        wsp_get_param_ptr->ua_profile.ua_profile_ptr = (char*)param_ptr->ua_prof_ptr;
        wsp_get_param_ptr->uri.uri_ptr = (char*)param_ptr->uri_ptr;
        wsp_get_param_ptr->user_agent.user_agent_ptr = (char*)param_ptr->user_agent_ptr;
        
        wsp_get_param_ptr->auth.password_ptr = MMIBROWSER_GetPassword();
        wsp_get_param_ptr->auth.user_name_ptr = MMIBROWSER_GetUserName();
    }	
        break;
        
    default:
        break;
    }

    return TRUE;
}
LOCAL BOOLEAN SetPostRequestParam(
					   HTTP_REQ_PARAM_T *param_ptr,//[IN]
					   BRW_PROTOCOL_TYPE_E protocol,//[IN]
					   void *post_param_ptr //[OUT]
					   )
{
    if((PNULL == param_ptr) || (PNULL == post_param_ptr))
    {
        return FALSE;
    }
    switch(protocol)
    {
    case BRW_PROTOCOL_HTTP:
    {
        HTTP_POST_PARAM_T *http_post_param_ptr=(HTTP_POST_PARAM_T*)post_param_ptr;

        BRW_Memset(http_post_param_ptr,0,sizeof(HTTP_POST_PARAM_T));

        http_post_param_ptr->accept.accept_ptr = BRW_DEFAULT_HTTP_ACCEPT_HEADER;
        //post请求的响应都以buffer方式传输
        http_post_param_ptr->get_data.style = HTTP_DATA_TRANS_STYLE_BUFFER;
        http_post_param_ptr->post_body.body_type_ptr=(char*)param_ptr->body_data.content_type_ptr;
        http_post_param_ptr->post_body.is_use_file=(param_ptr->body_data.mode==BRW_DATA_SAVE_IN_FILE);

        if(http_post_param_ptr->post_body.is_use_file)
        {
            uint16 file_name_len=0;
            if (PNULL == param_ptr->body_data.data_src.file_name_ptr)
            {
                BRWDebug_PrintWarning("SetPostRequestParam:file_name_ptr null");
                return FALSE;
            }
            file_name_len =(uint16)BRW_WStrlen(param_ptr->body_data.data_src.file_name_ptr);
            if ((file_name_len > 0)  && (file_name_len <= HTTP_MAX_FILE_NAME_LEN))
            {
                BRW_Memcpy(http_post_param_ptr->post_body.u.post_file.file_name,
                        param_ptr->body_data.data_src.file_name_ptr,
                        file_name_len+1);
                http_post_param_ptr->post_body.u.post_file.file_size = param_ptr->body_data.data_src_len;
            }
            else
            {
                //BRWDebug_Printf:"[BRW]W BrwSendHttpRequest:file_name_len=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_673_112_2_18_2_7_18_685,(uint8*)"d",file_name_len);
                return FALSE;
            }                
        }
        else
        {
            http_post_param_ptr->post_body.u.post_buffer.buffer_ptr = param_ptr->body_data.data_src.addr_ptr;
            http_post_param_ptr->post_body.u.post_buffer.buffer_len = param_ptr->body_data.data_src_len;
            //BRWDebug_Printf:"SetPostRequestParam,buf:%x,len:%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_683_112_2_18_2_7_18_686,(uint8*)"dd",http_post_param_ptr->post_body.u.post_buffer.buffer_ptr,http_post_param_ptr->post_body.u.post_buffer.buffer_len);
            http_post_param_ptr->post_body.u.post_buffer.is_copied_by_http = TRUE;
        }
        
        http_post_param_ptr->need_net_prog_ind = TRUE;
        http_post_param_ptr->referer.referer_ptr = (char*)param_ptr->refer_ptr;
        http_post_param_ptr->ua_profile.ua_profile_ptr = (char*)param_ptr->ua_prof_ptr;
        http_post_param_ptr->uri.uri_ptr = (char*)param_ptr->uri_ptr;
        http_post_param_ptr->user_agent.user_agent_ptr = (char*)param_ptr->user_agent_ptr;
        http_post_param_ptr->connection = HTTP_CONNECTION_KEEP_ALIVE;
        http_post_param_ptr->is_use_relative_url = param_ptr->is_using_relative_url;
#ifdef HTTP2_SUPPORT
        http_post_param_ptr->is_need_check_protocol = TRUE;
#endif
    }
        break;
        
    case BRW_PROTOCOL_WAP:
    {
        WSP_POST_PARAM_T *wsp_post_param_ptr=(WSP_POST_PARAM_T*)post_param_ptr;

        BRW_Memset(wsp_post_param_ptr,0,sizeof(WSP_POST_PARAM_T));

        wsp_post_param_ptr->accept.accept_ptr = BRW_DEFAULT_HTTP_ACCEPT_HEADER;
        wsp_post_param_ptr->post_body.body_type_ptr=(char*)param_ptr->body_data.content_type_ptr;
        wsp_post_param_ptr->post_body.is_use_file=(param_ptr->body_data.mode==BRW_DATA_SAVE_IN_FILE);
        if(wsp_post_param_ptr->post_body.is_use_file)
        {
            uint16 file_name_len=0;
            
            if (PNULL == param_ptr->body_data.data_src.file_name_ptr)
            {
                BRWDebug_PrintWarning("SetPostRequestParam:file_name_ptr null");
                return FALSE;
            }
            file_name_len =(uint16)BRW_WStrlen(param_ptr->body_data.data_src.file_name_ptr);
            if ((file_name_len > 0)  && (file_name_len <= HTTP_MAX_FILE_NAME_LEN))
            {
                BRW_Memcpy(wsp_post_param_ptr->post_body.u.post_file.file_name,
                        param_ptr->body_data.data_src.file_name_ptr,
                        file_name_len+1);
                wsp_post_param_ptr->post_body.u.post_file.file_size = param_ptr->body_data.data_src_len;
            }
            else
            {
                //BRWDebug_Printf:"[BRW]W SetPostRequestParam:file_name_len=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_725_112_2_18_2_7_18_687,(uint8*)"d",file_name_len);
                return FALSE;
            }                
        }
        else
        {
            wsp_post_param_ptr->post_body.u.post_buffer.buffer_ptr = param_ptr->body_data.data_src.addr_ptr;
            wsp_post_param_ptr->post_body.u.post_buffer.buffer_len = param_ptr->body_data.data_src_len;
            wsp_post_param_ptr->post_body.u.post_buffer.is_copied_by_http = TRUE;
        }
        wsp_post_param_ptr->need_net_prog_ind = TRUE;
        wsp_post_param_ptr->referer.referer_ptr = (char*)param_ptr->refer_ptr;
        wsp_post_param_ptr->ua_profile.ua_profile_ptr = (char*)param_ptr->ua_prof_ptr;
        wsp_post_param_ptr->uri.uri_ptr = (char*)param_ptr->uri_ptr;
        wsp_post_param_ptr->user_agent.user_agent_ptr = (char*)param_ptr->user_agent_ptr;
    }	
        break;
        
    default:
        break;
    }
    return TRUE;
}

/*
 *	get http request list
 */
PUBLIC BRW_LIST_T* BrwGetHttpReqList(void)
{
    return (BRW_LIST_T*)&g_http_req_list;
}

/*
 *	desc:send http request
 *  param:param  request parameter,such as uri etc
 *        context_id  http context id
 *  return:new HTTP_REQ_INFO_T struct pointer
 */

PUBLIC HTTP_REQ_INFO_T* BrwSendHttpRequest(HTTP_REQ_PARAM_T *param_ptr,uint32 context_id)
{
    HTTP_REQ_INFO_T *req = PNULL;
    BRW_INFO_T  *brw_info_ptr = GetInstanceByContextID(context_id);
    BRW_PUBLIC_PROFILE_T    *public_profile_ptr = BrwGetPublicProfile();

    if ((PNULL == param_ptr) || (PNULL == param_ptr->uri_ptr) || (PNULL == brw_info_ptr))
    {
        //BRWDebug_Printf:"[BRW]W BrwSendHttpRequest:param =0x%x!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_770_112_2_18_2_7_18_688,(uint8*)"d",param_ptr);
        return PNULL;
    }

    //generate http req
    req = BRWMem_Alloc(sizeof(HTTP_REQ_INFO_T));
    if(PNULL != req)
    {
        BRW_Memset(req,0,sizeof(HTTP_REQ_INFO_T)); 
        req->id = GetNextHttpReqID();
        //BRWDebug_Printf:"[BRW]BrwSendHttpRequest:BRWList_AppendObject req=0x%x!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_780_112_2_18_2_7_18_689,(uint8*)"d",(uint32)req);
        if(!BRWList_AppendObject(&g_http_req_list,req))
        {
            BRWMem_Free(req);
            //BRWDebug_Printf:"BrwSendHttpRequest:BRWList_AppendObject failed!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_784_112_2_18_2_7_18_690,(uint8*)"");
            return PNULL;
        }
        
	if (BRW_HTTP_METHOD_GET == param_ptr->method)
        {
            HTTP_GET_PARAM_T http_get_param={0};
            WSP_GET_PARAM_T wsp_get_param={0};
		
            if (BRW_PROTOCOL_HTTP == public_profile_ptr->protocol)
            {
                if(!SetGetRequestParam(param_ptr,public_profile_ptr->protocol,&http_get_param))
                {
                    BRWMem_Free(req);
                    return 0;
                }
                HTTP_GetRequest(context_id,&http_get_param,(HTTP_APP_INSTANCE_T)req->id);            
            }
            else if (BRW_PROTOCOL_WAP == public_profile_ptr->protocol)
            {
                if(!SetGetRequestParam(param_ptr,public_profile_ptr->protocol,&wsp_get_param))
                {
                    BRWMem_Free(req);
                    return 0;
                }
                WSP_GetRequest(context_id,&wsp_get_param,(HTTP_APP_INSTANCE_T)req->id);            
            }
            else
            {
                BRWDebug_PrintWarning("BrwSendHttpRequest,invalid protocol");
                BRWMem_Free(req);
                return 0;
            }
        }
        else if (BRW_HTTP_METHOD_POST == param_ptr->method)
        {
            HTTP_POST_PARAM_T http_post_param={0};
            WSP_POST_PARAM_T wsp_post_param={0};

            if (BRW_PROTOCOL_HTTP == public_profile_ptr->protocol)
            {
                if(!SetPostRequestParam(param_ptr,public_profile_ptr->protocol,&http_post_param))
                {
                    BRWMem_Free(req);
                    return 0;
                }
                HTTP_PostRequest(context_id,&http_post_param,(HTTP_APP_INSTANCE_T)req->id);            
            }
            else if (BRW_PROTOCOL_WAP == public_profile_ptr->protocol)
            {
                if(!SetPostRequestParam(param_ptr,public_profile_ptr->protocol,&wsp_post_param))
                {
                    BRWMem_Free(req);
                    return 0;
                }
                WSP_PostRequest(context_id,&wsp_post_param,(HTTP_APP_INSTANCE_T)req->id);            
            }
            else
            {
                BRWDebug_PrintWarning("BrwSendHttpRequest,invalid protocol");
                BRWMem_Free(req);
                return 0;
            }
        }
        else
        {
            //BRWDebug_Printf:"[BRW]W BrwSendHttpRequest:unsupport method=%d!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_850_112_2_18_2_7_18_691,(uint8*)"d",param_ptr->method);
            BRWMem_Free(req);
            return 0;
	}
        return req;
    }
    else
    {
        //BRWDebug_Printf:"BrwSendHttpRequest:malloc req failed!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_858_112_2_18_2_7_18_692,(uint8*)"");
        return PNULL;
    }   
}
/*
 *	desc:cancel a http request
 *  param:req           http request object pointer
 *        context_id    browser http context id
 */
PUBLIC void BrwCancelHttpRequest(HTTP_REQ_INFO_T* req,uint32 context_id,BRW_PROTOCOL_TYPE_E protocol)
{
    if (req != PNULL)
    {
        if (BRW_PROTOCOL_HTTP == protocol)
        {
            HTTP_CancelRequest(context_id,PNULL,req->http_request_id);      
        }
        else if (BRW_PROTOCOL_WAP == protocol)
        {
            WSP_CancelRequest(context_id,PNULL,req->http_request_id);
        }
        else
        {
            BRWDebug_PrintWarning("BrwCancelHttpRequest: unsupported protocol");
        }
    }
    else
    {
        BRWDebug_PrintWarning("BrwCancelHttpRequest,req null");
    }
}
/*
 *	desc:free memory of http request object
 *  param:req   http request object pointer
 */
PUBLIC void BrwFreeHttpRequest(HTTP_REQ_INFO_T* req)
{
    if (PNULL == req)
    {
        //BRWDebug_Printf:"BrwFreeHttpRequest:req null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_895_112_2_18_2_7_18_693,(uint8*)"");
        return;
    }
    
    if (req->charset != PNULL)
    {
        BRWMem_Free(req->charset);
        req->charset = PNULL;;
    }
    
    if (req->resp_buf_ptr != PNULL)
    {
        BRWMem_Free(req->resp_buf_ptr);	
        req->resp_buf_ptr = PNULL;;
    }
    
    //BRWDebug_Printf:"BrwFreeHttpRequest:req=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_911_112_2_18_2_7_18_694,(uint8*)"d", (uint32)req);
    BRWList_RemoveObject(&g_http_req_list,req);

    BRWMem_Free(req);
}

#ifdef DL_SUPPORT
/*
 *	desc:GetDownloadMimeType
 *  param:mime_type BRW_MIME_TYPE_E
*/
LOCAL MIME_TYPE_E GetDownloadMimeType(BRW_MIME_TYPE_E mime_type)
{
    MIME_TYPE_E dl_mime_type = MIME_TYPE_UNKNOWN;

    switch (mime_type)
    {
    case BRW_MIME_HTML:
        dl_mime_type = MIME_TYPE_TEXT_HTML;
        break;

    case BRW_MIME_WML:
        dl_mime_type = MIME_TYPE_TEXT_WML;
        break;

    case BRW_MIME_WMLS:
        dl_mime_type = MIME_TYPE_TEXT_WMLS;
        break;

    case BRW_MIME_WMLSC:
        dl_mime_type = MIME_TYPE_TEXT_WMLSC;
        break;

    case BRW_MIME_WMLC:
        dl_mime_type = MIME_TYPE_TEXT_WMLC;
        break;

    case BRW_MIME_PLAINTEXT:
        dl_mime_type = MIME_TYPE_TEXT_PLAIN;
        break;

    case BRW_MIME_CSS:
        dl_mime_type = MIME_TYPE_TEXT_CSS;
        break;

    case BRW_MIME_IMG_BMP:
        dl_mime_type = MIME_TYPE_IMAGE_BMP;
        break;

    case BRW_MIME_IMG_IEF:
        dl_mime_type = MIME_TYPE_IMAGE_IEF;
        break;

    case BRW_MIME_IMG_GIF:
        dl_mime_type = MIME_TYPE_IMAGE_GIF;
        break;

    case BRW_MIME_IMG_JPEG:
        dl_mime_type = MIME_TYPE_IMAGE_JPEG;
        break;

    case BRW_MIME_IMG_PNG:
        dl_mime_type = MIME_TYPE_IMAGE_PNG;
        break;

    case BRW_MIME_IMG_TIF:
        dl_mime_type = MIME_TYPE_IMAGE_TIFF;
        break;

    case BRW_MIME_IMG_WBMP:
        dl_mime_type = MIME_TYPE_IMAGE_VND_WAP_WBMP;
        break;

    case BRW_MIME_IMG_PCX:
        dl_mime_type = MIME_TYPE_UNKNOWN;
        break;

    case BRW_MIME_AUDIO_AMR:
        dl_mime_type = MIME_TYPE_AUDIO_AMR;
        break;

    case BRW_MIME_AUDIO_MIDI:
        dl_mime_type = MIME_TYPE_AUDIO_MIDI;
        break;

    case BRW_MIME_AUDIO_MID:
        dl_mime_type = MIME_TYPE_AUDIO_MIDI;
        break;

    case BRW_MIME_AUDIO_MPA:
        dl_mime_type = MIME_TYPE_AUDIO_MPA;
        break;

    case BRW_MIME_AUDIO_MPA_ROBUST:
        dl_mime_type = MIME_TYPE_AUDIO_MPA_ROBUST;
        break;

    case BRW_MIME_AUDIO_MPA_LATM:
        dl_mime_type = MIME_TYPE_AUDIO_MPA_LATM;
        break;

    case BRW_MIME_AUDIO_MPEG:
        dl_mime_type = MIME_TYPE_AUDIO_MPEG;
        break;

    case BRW_MIME_AUDIO_TONE:
        dl_mime_type = MIME_TYPE_AUDIO_TONE;
        break;

    case BRW_MIME_AUDIO_RM:
        dl_mime_type = MIME_TYPE_AUDIO_RM;
        break;

    case BRW_MIME_AUDIO_WAV:
        dl_mime_type = MIME_TYPE_AUDIO_X_WAV;
        break;

    case BRW_MIME_AUDIO_3GP:
        dl_mime_type = MIME_TYPE_VIDEO_3GPP;
        break;

    case BRW_MIME_AUDIO_X_AAC:
        dl_mime_type = MIME_TYPE_AUDIO_X_AAC;
        break;

    case BRW_MIME_AUDIO_WMA:
        dl_mime_type = MIME_TYPE_AUDIO_WMA;
        break;

    case BRW_MIME_AUDIO_UNKNOWN:
        dl_mime_type = MIME_TYPE_UNKNOWN;
        break;

    case BRW_MIME_VIDEO_3GP:
        dl_mime_type = MIME_TYPE_VIDEO_3GPP;
        break;

    case BRW_MIME_VIDEO_MPEG:
        dl_mime_type = MIME_TYPE_VIDEO_MPEG;
        break;

    case BRW_MIME_VIDEO_WMV:
        dl_mime_type = MIME_TYPE_VIDEO_WMV;
        break;

    case BRW_MIME_OMA_DD:
        dl_mime_type = MIME_TYPE_APPLICATION_VND_OMA_DD;
        break;

    case BRW_MIME_OMA_DRM:
        dl_mime_type = MIME_TYPE_APPLICATION_VND_OMA_DRM_MESSAGE;
        break;

    case BRW_MIME_OMA_DRM_MESSAGE:
        dl_mime_type = MIME_TYPE_APPLICATION_VND_OMA_DRM_MESSAGE;
        break;

    case BRW_MIME_OMA_DRM_CONTENT:
        dl_mime_type = MIME_TYPE_APPLICATION_VND_OMA_DRM_CONTENT;
        break;

    case BRW_MIME_OMA_DRM_RIGHTS_XML:
        dl_mime_type = MIME_TYPE_APPLICATION_VND_OMA_DRM_RIGHTS_XML;
        break;

    case BRW_MIME_OMA_DRM_RIGHTS_WBXML:
        dl_mime_type = MIME_TYPE_APPLICATION_VND_OMA_DRM_RIGHTS_WBXML;
        break;

    case BRW_MIME_JAD:
        dl_mime_type = MIME_TYPE_APPLICATION_JAVA_JAD;
        break;

    case BRW_MIME_JAR:
        dl_mime_type = MIME_TYPE_APPLICATION_JAVA_JAR;
        break;

    case BRW_MIME_SDP:
        dl_mime_type = MIME_TYPE_APPLICATION_SDP;
        break;

#ifdef MCARE_MINI_SUPPORT
    case BRW_MIME_MCARE:
        dl_mime_type = MIME_TYPE_UNKNOWN;
        break;
#endif

#ifdef QBTHEME_SUPPORT
    case BRW_MIME_QBT:
        dl_mime_type = MIME_TYPE_APPLICATION_QBT;
        break;
#endif

#ifdef HOME_LOCATION_INQUIRE_SUPPORT_TCARD
    case BRW_MIME_PNL:
        dl_mime_type = MIME_TYPE_APPLICATION_PNL;
        break;
#endif

#ifdef DATA_ACCOUNT_IN_TCARD_SUPPORT
    case BRW_MIME_NTAC:
        dl_mime_type = MIME_TYPE_APPLICATION_NATC;
        break;
#endif

    default:
        break;
    }

    return dl_mime_type;
}
#endif

/*
 *	process http header ind
 */
LOCAL void ProcHeaderInd(HTTP_HEADER_IND_SIG_T* ind)
{
    BRW_PUBLIC_PROFILE_T    *public_profile_ptr = BrwGetPublicProfile();

#if defined(MMI_UPDATE_TIME_SUPPORT) && defined(MMI_NET_TIME_SUPPORT)
    if(PNULL!=ind)
    {
        /*get time infor from receive data, if get time,set time to system*/
        MMIAPIAUT_GetTimeFromRecvData(ind->header_ptr, ind->rsp_header_info.content_length);
    }
    
#endif
   if (ind != PNULL)
   {
       uint32 content_len = 0;
       WAP_REQUEST_T    *wap_req_ptr = PNULL;
       BRW_REQ_STATE_E  wap_req_state = BRW_STATE_MAX;
       BRW_INFO_T   *brw_info_ptr = PNULL;
       HTTP_REQ_INFO_T  *http_req_ptr = PNULL;
       uint32 context_id    = ind->context_id;
       uint32 request_id    = ind->request_id;
       uint32 req_id = (uint32)ind->app_instance;
       MEDIA_OBJ_T *media_obj_ptr = PNULL;
       uint32  max_content_size = 0;

        //BRWDebug_Printf:"[BRW] req_id:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_942_112_2_18_2_7_18_695,(uint8*)"d",req_id);

        brw_info_ptr = GetInstanceByContextID(context_id);        
        http_req_ptr = (HTTP_REQ_INFO_T*)BRWList_SearchObject(&g_http_req_list, (BRW_LIST_CMP_F)CompareReqID, (void*)req_id);
        wap_req_ptr =  BrwGetWapReqByHttpReqEx(brw_info_ptr, http_req_ptr, &media_obj_ptr);

        if (wap_req_ptr != PNULL)//MS00235059
        {
            if (PNULL == media_obj_ptr)//the http req is wap_req_ptr->http_req_ptr
            {
                wap_req_ptr->is_retried = FALSE;
                //BRWDebug_Printf:"[BRW]wap_req"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_953_112_2_18_2_7_18_696,(uint8*)"");
           }
            // bug 70086
//             else//the http req is for the media obj
//             {
//                 media_obj_ptr->is_retried = FALSE;
//                 //BRWDebug_Printf:"[BRW]media"
//                 SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_958_112_2_18_2_7_18_697,(uint8*)"");
//            }
        }

        if(
           (HTTP_RESPONSE_CODE_MOVED_PERMANENTLY == ind->rsp_header_info.response_code )
            || (HTTP_RESPONSE_CODE_FOUND == ind->rsp_header_info.response_code )
            || (HTTP_RESPONSE_CODE_UNAUTHORIZED == ind->rsp_header_info.response_code )
            || (HTTP_RESPONSE_CODE_PROXY_AUTHENTICATION_REQUIRED == ind->rsp_header_info.response_code )
            )
        {
            //BRWDebug_Printf:"ProcHeaderInd ind->rsp_header_info.response_code:%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_969_112_2_18_2_7_18_698,(uint8*)"d",ind->rsp_header_info.response_code);
            return;
        }
        
        if (PNULL == wap_req_ptr)
        {
           //BRWDebug_Printf:"ProcDataInd:wap_req_ptr null"
           SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_975_112_2_18_2_7_18_699,(uint8*)"");
           BrwFreeHttpRequest(http_req_ptr);
           return;
       }
       
        wap_req_state = BrwGetWapReqState(wap_req_ptr);

        http_req_ptr->transfer_encoding = (BRW_HTTP_ENCODING_TYPE_E)ind->rsp_header_info.content_encoding_type;
        if (ind->rsp_header_info.content_type_ptr!= PNULL)
        {
            //BRWDebug_Printf:"[BRW]ProcHeaderInd content_type_ptr = %s"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_985_112_2_18_2_7_18_700,(uint8*)"s", ind->rsp_header_info.content_type_ptr);
        }
        http_req_ptr->mime_type = BRW_GetMineType((uint8*)ind->rsp_header_info.content_type_ptr);
        //BRWDebug_Printf:"[BRW]ProcHeaderInd mime_type =%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_988_112_2_18_2_7_18_701,(uint8*)"d", http_req_ptr->mime_type);

        //begin MS00246813:display the error infomation in web page
        switch (ind->rsp_header_info.response_code)
        {
        case HTTP_RESPONSE_CODE_BAD_REQUEST: //400
        case HTTP_RESPONSE_CODE_PAYMENT_REQUIRED: //402
        case HTTP_RESPONSE_CODE_FORBIDDEN: //403
        case HTTP_RESPONSE_CODE_NOT_FOUND: //404
        case HTTP_RESPONSE_CODE_METHOD_NOT_ALLOWED: //405
        case HTTP_RESPONSE_CODE_NOT_ACCEPTABLE: //406
        case HTTP_RESPONSE_CODE_REQUEST_TIMEOUT: //408
        case HTTP_RESPONSE_CODE_CONFLICT: //409
        case HTTP_RESPONSE_CODE_GONE: //410
        case HTTP_RESPONSE_CODE_LENGTH_REQUIRED: //411
        case HTTP_RESPONSE_CODE_PRECONDITION_FAILED: //412
        case HTTP_RESPONSE_CODE_REQUEST_ENTITY_TOO_LARGE: //413
        case HTTP_RESPONSE_CODE_REQUEST_URI_TOO_LONG: //414
        case HTTP_RESPONSE_CODE_UNSUPPORTED_MEDIA_TYPE: //415
        case HTTP_RESPONSE_CODE_REQUESTED_RANGE_NOT_SATISFIABLE: //416
        case HTTP_RESPONSE_CODE_EXPECTATION_FAILED: //417
        case HTTP_RESPONSE_CODE_INTERNAL_SERVER_ERROR: //500
        case HTTP_RESPONSE_CODE_NOT_IMPLEMENTED: //501
        case HTTP_RESPONSE_CODE_BAD_GATEWAY: //502
        case HTTP_RESPONSE_CODE_SERVICE_UNAVAILABLE: //503
        case HTTP_RESPONSE_CODE_GATEWAY_TIMEOUT: //504
        case HTTP_RESPONSE_CODE_VERSION_NOT_SUPPORTED: //505
            if (0 == ind->rsp_header_info.content_length)
            {
                http_req_ptr->mime_type = BRW_MIME_HTML;
            }
            else
            {
                //BRWDebug_Printf:"[BRW]ProcHeaderInd content_length =%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_1021_112_2_18_2_7_18_702,(uint8*)"d", ind->rsp_header_info.content_length);
            }            
            break;            
        default:
            //do nothing
            break;
        }

        //BRWDebug_Printf:"[BRW]ProcHeaderInd mime_type(after) =%d, response_code= %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_1030_112_2_18_2_7_18_703,(uint8*)"dd", http_req_ptr->mime_type, ind->rsp_header_info.response_code);
        //end MS00246813

       if(http_req_ptr->charset != PNULL)
       {
            BRWMem_Free(http_req_ptr->charset);
            http_req_ptr->charset = PNULL;
       }       
       if(ind->rsp_header_info.charset_ptr!=PNULL)
       {
           uint32 charset_len = BRW_Strlen((const uint8*)ind->rsp_header_info.charset_ptr);           
           http_req_ptr->charset=BRWMem_Alloc(charset_len+1);
           if(http_req_ptr->charset != PNULL)
           {
               BRW_Memcpy(http_req_ptr->charset,ind->rsp_header_info.charset_ptr,charset_len);
               *(http_req_ptr->charset+charset_len)=0;
           }
           else
           {
               //BRWDebug_Printf:"[BRW]malloc charset failed"
               SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_1049_112_2_18_2_7_18_704,(uint8*)"");
           }
       }
       BRWMem_Free(http_req_ptr->resp_buf_ptr);
       http_req_ptr->resp_buf_ptr=PNULL;
       http_req_ptr->resp_buf_now_len=0;  
      // http_req_ptr->transfer_encoding=BRW_HTTP_ENCODING_TYPE_NULL;
        if(ind->rsp_header_info.content_length==-1)
        {
            http_req_ptr->http_content_len = 0;
        }
        else
        {
            http_req_ptr->http_content_len = ind->rsp_header_info.content_length;       
        }
        content_len = http_req_ptr->http_content_len;

       if ((BRW_STATE_PAGE_REQUESTING == wap_req_state) 
            && (BRW_HTTP_STATUS_200 == ind->rsp_header_info.response_code))
       {           
           if(!IsWebPageMimeType(http_req_ptr->mime_type) 
               ||(http_req_ptr->mime_type >= BRW_MIME_IMG_BMP 
                  && http_req_ptr->mime_type < BRW_MIME_IMG_UNKNOWN 
                  && !public_profile_ptr->is_preview_image
                  && http_req_ptr->mime_type != BRW_MIME_SDP))
           {
#ifdef DL_SUPPORT
                MMIDL_DOWNLOAD_INFO_T download_info = {0};

                download_info.mime_type = GetDownloadMimeType(http_req_ptr->mime_type);
                download_info.http_header = ind->header_ptr;
                download_info.url = wap_req_ptr->absolute_url;
                //download_info.is_chunk = FALSE;
                download_info.status_code = ind->rsp_header_info.response_code;
                //download_info.method = 0;
                download_info.buf = PNULL;

                if (public_profile_ptr->is_use_wifi)
                {
                    download_info.bearer_type = MMIPDP_INTERFACE_WIFI;
                }
                else
                {
                    download_info.dual_sys = public_profile_ptr->dual_sys;
                    download_info.bearer_type = MMIPDP_INTERFACE_GPRS;
                }

                MMIAPIDL_StartDownload(MMIDL_CREATE_NEW, &download_info);

                BRW_StopPageReq(brw_info_ptr->instance_id);
#else
                if (!BrwStartDL(brw_info_ptr, wap_req_ptr, ind->header_ptr))
                {
                    BrwProcErr(brw_info_ptr, BRW_ERR_UNKNOWN);
                }
#endif
                return;//start download
            }
#ifdef MMIDORADO_WRE_TRANSCODE_SUPPORT
            else
            {
                //for sprd wre transcoding in 移动梦网
                if (BRW_CaseStrstr((uint8 *)(public_profile_ptr->apn_ptr), (uint8 *)"cmwap") == (uint8 *)(public_profile_ptr->apn_ptr))
                {
                    uint8   *gwt_end_url = BrwGwtMovetoGwtEnd(wap_req_ptr->absolute_url);
                    uint8   *new_req_url = PNULL;
                    uint32   new_req_url_len = 0;
                 
                    if ((gwt_end_url != PNULL) && (gwt_end_url != wap_req_ptr->absolute_url))
                    {
                        //cancel the old http request, use url that without wre_gwt in the header to make new request
                        BrwCancelHttpRequest(wap_req_ptr->http_req_ptr,
                        			brw_info_ptr->http_context_id,
                        			public_profile_ptr->protocol);
                        BrwFreeHttpRequest(wap_req_ptr->http_req_ptr);
                        wap_req_ptr->http_req_ptr = PNULL;

                        new_req_url_len = BRW_Strlen(gwt_end_url);
                        new_req_url = BRWMem_Alloc(new_req_url_len + 1);
                        BRW_Memset(new_req_url, 0, new_req_url_len+1);
                        BRW_Memcpy(new_req_url, gwt_end_url, new_req_url_len);
                        BRWMem_Free(wap_req_ptr->absolute_url);
                        wap_req_ptr->absolute_url = new_req_url;
                        BrwStartPageRequest(brw_info_ptr);
                        return;
                    }
                }
            }
#endif            
        }  

        if (BRW_STATE_PAGE_REQUESTING == wap_req_state)
        {
            max_content_size = BRW_DEFAULT_MAX_PAGE_CONTENT_LEN;
        }
        else
        {
            max_content_size = BRW_DEFAULT_MAX_HTTP_CONTENT_LEN;
        }
       
        if(content_len>0)
        {
            if(content_len <= max_content_size)
            {
                http_req_ptr->resp_buf_ptr = (uint8*)BRWMem_Alloc(content_len+1);
                if(PNULL==http_req_ptr->resp_buf_ptr)
                {
                    //错误处理
                    BrwProcErr(brw_info_ptr,BRW_ERR_NO_MEM);           
                    return;
                }
            }
            else
            {
                BrwProcHttpErr(brw_info_ptr,http_req_ptr,BRW_ERR_PAGE_TOO_BIG);
                return;
            }
        }
        http_req_ptr->resp_buf_max_len = content_len;

        //发送HTTP_HEADER_RSP消息，传输方式为buf方式
        if (BRW_PROTOCOL_HTTP == public_profile_ptr->protocol)
        {
            if(HTTP_HeaderResponse(context_id,req_id,request_id,HTTP_DATA_TRANS_STYLE_BUFFER,PNULL,0)!=HTTP_SUCCESS)
            {
                BrwProcErr(brw_info_ptr,BRW_ERR_UNKNOWN);    
                return;
            }
        }

        //http refresh        
        if ((BRW_STATE_PAGE_REQUESTING == wap_req_state) 
            && (BRW_HTTP_STATUS_200 == ind->rsp_header_info.response_code)
            && (ind->rsp_header_info.refresh_url_ptr != PNULL))
        {
            BRW_MSG_HTTP_REFRESH_IND_T refresh_ind ={0};

            refresh_ind.refresh_time = ind->rsp_header_info.refresh_time;
            BRW_Strncpy(refresh_ind.url_arr, (uint8*)(ind->rsp_header_info.refresh_url_ptr), BRW_MAX_URL_LEN);
            brw_info_ptr->profile.msg_proc_func(BRW_MSG_TYPE_HTTP_REFRESH_IND,&refresh_ind,sizeof(BRW_MSG_HTTP_REFRESH_IND_T));
            SCI_TRACE_LOW("[BRW]ProcHeaderInd: refresh_time = %d", refresh_ind.refresh_time);
        }
   }
}

/*
 *	process http data ind
 */
LOCAL void ProcDataInd(HTTP_DATA_IND_SIG_T* ind)
{
    BRW_INFO_T  *brw_info_ptr = PNULL;
    uint32 data_len = 0;
    HTTP_REQ_INFO_T *http_req_ptr = PNULL;
    WAP_REQUEST_T   *wap_req_ptr = PNULL;
    uint32 req_id = BRW_INVALID_HTTP_REQ_ID;
    BRW_REQ_STATE_E req_state=BRW_STATE_MAX;
    BRW_PUBLIC_PROFILE_T    *public_profile_ptr = BrwGetPublicProfile();
    uint32  max_content_size = 0;

    if (PNULL == ind)
    {
        return;
    }
    
    req_id = (uint32)ind->app_instance;
    http_req_ptr = (HTTP_REQ_INFO_T*)BRWList_SearchObject(&g_http_req_list,(BRW_LIST_CMP_F)CompareReqID,(void*)req_id);
    if (PNULL == http_req_ptr)
    {
        //BRWDebug_Printf:"[BRW]ProcDataInd:http_req_ptr null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_1131_112_2_18_2_7_19_705,(uint8*)"");
        return;
    }
    brw_info_ptr = GetInstanceByContextID(ind->context_id);
    if (PNULL == brw_info_ptr)
    {
        //BRWDebug_Printf:"ProcDataInd:brw_info_ptr =0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_1137_112_2_18_2_7_19_706,(uint8*)"d",brw_info_ptr);
        BrwFreeHttpRequest(http_req_ptr);
        return;
    }   
    wap_req_ptr =  BrwGetWapReqByHttpReq(brw_info_ptr,http_req_ptr);
    if (PNULL == wap_req_ptr)
    {
       //BRWDebug_Printf:"ProcDataInd:wap_req_ptr null"
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_1144_112_2_18_2_7_19_707,(uint8*)"");
       BrwFreeHttpRequest(http_req_ptr);
       return;
    }
    
    req_state = BrwGetWapReqState(wap_req_ptr);
    if (BRW_STATE_DOWNLOADING == req_state)
    {
        if((public_profile_ptr->protocol != BRW_PROTOCOL_WAP)
            || (BRW_DL_STATE_OMA_DD == wap_req_ptr->download_state)
            || (BRW_DL_STATE_OMA_CONTENT ==wap_req_ptr->download_state))
        {
            BRWDebug_PrintWarning("ProcDataInd:BRW_STATE_DOWNLOADING,no data ind assumed");
            return;
        }
        
        if(!BrwWspDownloadDataIn(wap_req_ptr,ind->data_ptr,ind->data_len,http_req_ptr))
        {
            BRWDebug_PrintWarning("ProcDataInd:wsp write data to ffs failed");
            BrwProcHttpErr(brw_info_ptr,http_req_ptr,BRW_ERR_UNKNOWN);
        }
        return;
    }

    if (BRW_STATE_PAGE_REQUESTING == req_state)
    {
        max_content_size = BRW_DEFAULT_MAX_PAGE_CONTENT_LEN;
    }
    else
    {
        max_content_size = BRW_DEFAULT_MAX_HTTP_CONTENT_LEN;
    }

    data_len = ind->data_len; //BRW_Strlen(ind->data_ptr);
    if (http_req_ptr->resp_buf_now_len+data_len>http_req_ptr->resp_buf_max_len)
    {
        //BRWDebug_Printf:"[BRW]ProcDataInd,now_len=%d,data_len=%d,max_len=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_1172_112_2_18_2_7_19_708,(uint8*)"ddd",http_req_ptr->resp_buf_now_len,data_len,http_req_ptr->resp_buf_max_len);
                    
        if(http_req_ptr->resp_buf_now_len+data_len > max_content_size)
        {
            BrwProcHttpErr(brw_info_ptr,http_req_ptr,BRW_ERR_PAGE_TOO_BIG);
        }
        else
        {
            //重新分配内存
            uint8   *new_buf = BRWMem_Alloc(http_req_ptr->resp_buf_now_len+data_len);
            if (PNULL == new_buf)
            {
                BrwProcHttpErr(brw_info_ptr,http_req_ptr,BRW_ERR_NO_MEM);
            }
            else
            {
                BRW_Memset(new_buf,0,http_req_ptr->resp_buf_now_len+data_len);
                if(http_req_ptr->resp_buf_now_len>0 && http_req_ptr->resp_buf_ptr!=0)
                {
                    BRW_Memcpy(new_buf,http_req_ptr->resp_buf_ptr,http_req_ptr->resp_buf_now_len);
                }
                BRW_Memcpy((void*)(new_buf+http_req_ptr->resp_buf_now_len),ind->data_ptr,data_len);
                if(http_req_ptr->resp_buf_ptr != PNULL)
                {
                    BRWMem_Free(http_req_ptr->resp_buf_ptr);
                }
                http_req_ptr->resp_buf_ptr = new_buf;
                http_req_ptr->resp_buf_now_len += data_len;
                http_req_ptr->resp_buf_max_len = http_req_ptr->resp_buf_now_len;
//                //发送进度报告
//                if(BrwGetWapReqState(wap_req_ptr)==BRW_STATE_PAGE_REQUESTING)
//                {
//                    BrwUpdateProgressInfo(brw_info_ptr,wap_req_ptr,http_req_ptr->resp_buf_now_len,http_req_ptr->resp_buf_max_len);
//                }                
            }
        }
    }//if(http_req_ptr->resp_buf_now_len+data_len>=http_req_ptr->resp_buf_max_len)   
    else
    {
        BRW_Memcpy((void*)(http_req_ptr->resp_buf_ptr+http_req_ptr->resp_buf_now_len),ind->data_ptr,data_len);
        http_req_ptr->resp_buf_now_len+=data_len;
//        //发送进度报告
//        if(req_state==BRW_STATE_PAGE_REQUESTING)
//        {
//            BrwUpdateProgressInfo(brw_info_ptr,wap_req_ptr,http_req_ptr->resp_buf_now_len,http_req_ptr->resp_buf_max_len);
//        }        
    }    
}
/*
 *	process http get cnf
 */
LOCAL void ProcGetCnf(HTTP_GET_CNF_SIG_T *cnf)
{
    BRW_INFO_T  *brw_info_ptr = PNULL;
    HTTP_REQ_INFO_T *http_req_ptr = PNULL;
    uint32 req_id = BRW_INVALID_HTTP_REQ_ID;
    
    if (PNULL == cnf)
    {
        return;
    }
    req_id = (uint32)cnf->app_instance;
    http_req_ptr = (HTTP_REQ_INFO_T*)BRWList_SearchObject(&g_http_req_list,(BRW_LIST_CMP_F)CompareReqID,(void*)req_id);
    brw_info_ptr = GetInstanceByContextID(cnf->context_id);
    if ((PNULL == brw_info_ptr) || (PNULL == http_req_ptr))
    {
        //SCI_TRACE_LOW:"ProcGetCnf:brw_info_ptr =0x%x,http_req_ptr:%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_1237_112_2_18_2_7_19_709,(uint8*)"dd",brw_info_ptr,http_req_ptr);
        BrwFreeHttpRequest(http_req_ptr);
        return;
    } 
    if(cnf->result != HTTP_SUCCESS)
    {
        ProcHttpErr(brw_info_ptr,http_req_ptr,(HTTP_ERROR_E)cnf->result);
    }
    else
    {
        BrwProcHttpReqDone(brw_info_ptr,&http_req_ptr);
    }    
}
/*
 *	process http post cnf
 */
LOCAL void ProcPostCnf(HTTP_POST_CNF_SIG_T *cnf)
{
    BRW_INFO_T  *brw_info_ptr = PNULL;
    HTTP_REQ_INFO_T *http_req_ptr = PNULL;
    uint32 req_id = BRW_INVALID_HTTP_REQ_ID;
    
    if (PNULL == cnf)
    {
        return;
    }
    req_id = (uint32)cnf->app_instance;
    http_req_ptr = (HTTP_REQ_INFO_T*)BRWList_SearchObject(&g_http_req_list,(BRW_LIST_CMP_F)CompareReqID,(void*)req_id);
    brw_info_ptr = GetInstanceByContextID(cnf->context_id);
    if ((PNULL == brw_info_ptr) || (PNULL == http_req_ptr))
    {
        //BRWDebug_Printf:"ProcGetCnf:brw_info_ptr =0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_1267_112_2_18_2_7_19_710,(uint8*)"d",brw_info_ptr);
        BrwFreeHttpRequest(http_req_ptr);
        return;
    } 
    //SCI_TRACE_LOW:"brw ProcPostCnf cnf->result=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_1271_112_2_18_2_7_19_711,(uint8*)"d",cnf->result);
    if(cnf->result != HTTP_SUCCESS)
    {
        ProcHttpErr(brw_info_ptr,http_req_ptr,(HTTP_ERROR_E)cnf->result);
    }
    else
    {
        BrwProcHttpReqDone(brw_info_ptr,&http_req_ptr);
    }    
}

#ifdef HTTP2_SUPPORT
LOCAL void ProcVersionInd(HTTP_VERSION_IND_SIG_T* ind)
{
    BRW_INFO_T  *brw_info_ptr = PNULL;
    uint32 data_len = 0;
    HTTP_REQ_INFO_T *http_req_ptr = PNULL;
    WAP_REQUEST_T   *wap_req_ptr = PNULL;
    uint32 req_id = BRW_INVALID_HTTP_REQ_ID;
    uint32 url_len = 0;

    if (PNULL == ind)
    {
        SCI_TRACE_LOW("BRW [%s:%d] ind is null\n",__FUNCTION__,__LINE__);
        return;
    }
    
    req_id = (uint32)ind->app_instance;
    http_req_ptr = (HTTP_REQ_INFO_T*)BRWList_SearchObject(&g_http_req_list,(BRW_LIST_CMP_F)CompareReqID,(void*)req_id);
    if (PNULL == http_req_ptr)
    {
        SCI_TRACE_LOW("BRW [%s:%d] http_req_ptr is null\n",__FUNCTION__,__LINE__);
        return;
    }
    brw_info_ptr = GetInstanceByContextID(ind->context_id);
    if (PNULL == brw_info_ptr)
    {
        SCI_TRACE_LOW("BRW [%s:%d] brw_info_ptr is null\n",__FUNCTION__,__LINE__);
        BrwFreeHttpRequest(http_req_ptr);
        return;
    }   
    wap_req_ptr =  BrwGetWapReqByHttpReq(brw_info_ptr,http_req_ptr);
    if (PNULL == wap_req_ptr)
    {
       SCI_TRACE_LOW("BRW [%s:%d] wap_req_ptr is null\n",__FUNCTION__,__LINE__);
       BrwFreeHttpRequest(http_req_ptr);
       return;
    }

    if (BRW_Strcmp(ind->version,"HTTP/2"))
    {
        brw_info_ptr->curr_host_is_http2 = FALSE;
    }
    else
    {
        brw_info_ptr->curr_host_is_http2 = TRUE;
    }

    BRWMem_Free(brw_info_ptr->curr_host_url);
    brw_info_ptr->curr_host_url = PNULL;
    
    url_len = BRW_Strlen((const uint8*)ind->host_url);
    brw_info_ptr->curr_host_url = BRWMem_Alloc(url_len+1);
    if (brw_info_ptr->curr_host_url != PNULL)
    {
        BRW_Memcpy(brw_info_ptr->curr_host_url, ind->host_url, url_len);
        *(brw_info_ptr->curr_host_url+url_len) = 0;
    }
    else
    {
        BrwProcErr(brw_info_ptr,BRW_ERR_NO_MEM);
    }

}
#endif

/*
 *	desc:generate a unique id to identify different init http session req
 */
LOCAL uint32 GenUniqueSubModuleID(void)
{
    return g_brw_next_submodule_id++;
}
/*
 *	desc:compare request id
 *  param:req_ptr http request pointer
 *        id      http request local id
 */
LOCAL BOOLEAN CompareReqID(HTTP_REQ_INFO_T* req_ptr,uint32 id)
{
    if(req_ptr && id!=BRW_INVALID_HTTP_REQ_ID)
    {
        return (req_ptr->id==id);
    }
    return FALSE;
}
/*
 *	check if brw_info_ptr's sub_http_module_id equal sub_module_id
 */
LOCAL BOOLEAN   CompareSubModuleID(BRW_INFO_T *brw_info_ptr,uint32 sub_module_id)
{
    if (brw_info_ptr != PNULL)
    {
        return (brw_info_ptr->sub_http_module_id == sub_module_id);
    }
    else
    {
        return FALSE;
    }    
}
/*
 *	check if brw_info_ptr's http_context_id equal http_context_id
 */
LOCAL BOOLEAN   CompareHttpContextID(BRW_INFO_T *brw_info_ptr,uint32 http_context_id)
{
    if (brw_info_ptr != PNULL)
    {
        return (brw_info_ptr->http_context_id==http_context_id);
    }
    else
    {
        return FALSE;
    }    
}
/*
 *	desc:get corresponding BRW_INFO_T pointer according to sub_module_id
 */
LOCAL BRW_INFO_T* BrwGetInstanceBySubModuleID(uint32 sub_module_id)
{
/*
     BRW_LIST_T *instance_list_ptr=PNULL;
     BRW_INFO_T *rtn_brw_info_ptr=PNULL;
     
     if(BRW_INVALID_SUBMODULE_ID == sub_module_id)
     {
         BRWDebug_PrintWarning("BrwGetInstanceBySubModuleID:sub_module_id invalid");
         return PNULL;
     }
     instance_list_ptr = BrwGetInstanceList();
     
     rtn_brw_info_ptr =BRWList_SearchObject(instance_list_ptr,
                            (BRW_LIST_CMP_F)CompareSubModuleID,
                            (void*)sub_module_id);

     return rtn_brw_info_ptr;
     */
     BRW_INFO_T *rtn_brw_info_ptr=PNULL;
     
     if(BRW_INVALID_SUBMODULE_ID == sub_module_id)
     {
         BRWDebug_PrintWarning("BrwGetInstanceBySubModuleID:sub_module_id invalid");
         return PNULL;
     }     
     rtn_brw_info_ptr =BRW_SearchInstance((BRW_LIST_CMP_F)CompareSubModuleID,(void*)sub_module_id);

     return rtn_brw_info_ptr;
}
/*
 *	desc:get corresponding BRW_INFO_T pointer according to http_context_id
 */
LOCAL BRW_INFO_T* GetInstanceByContextID(uint32 context_id)
{
/*
     BRW_LIST_T *instance_list_ptr=PNULL;
     BRW_INFO_T *rtn_brw_info_ptr=PNULL;
     
     if (context_id == BrwGetInvalidHttpContextIDValue())
     {
         BRWDebug_PrintWarning("GetInstanceByContextID:context_id invalid");
         return PNULL;
     }
     instance_list_ptr = BrwGetInstanceList();
     
     rtn_brw_info_ptr =(BRW_INFO_T*)BRWList_SearchObject(instance_list_ptr,
                            (BRW_LIST_CMP_F)CompareHttpContextID,
                            (void*)context_id);

     return rtn_brw_info_ptr;
     */
     BRW_INFO_T *rtn_brw_info_ptr=PNULL;
     
     if (context_id == BrwGetInvalidHttpContextIDValue())
     {
         BRWDebug_PrintWarning("GetInstanceByContextID:context_id invalid");
         return PNULL;
     }
     
     rtn_brw_info_ptr =BRW_SearchInstance((BRW_LIST_CMP_F)CompareHttpContextID,(void*)context_id);

     return rtn_brw_info_ptr;
}
/*
 *	if http task is inited
 */
PUBLIC BOOLEAN BrwIsHttpInited(BRW_INFO_T *brw_info_ptr)
{
    if (BRW_HTTP_STATE_INITED == brw_info_ptr->http_state)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}
/*
 *	get http context id of brw_info_ptr
 */
PUBLIC uint32 BrwGetHttpContextID(BRW_INFO_T *brw_info_ptr)
{
    if(brw_info_ptr != PNULL)
    {
        return brw_info_ptr->http_context_id;
    }
    else
    {
        return HTTP_INVALID_CONTEXT_ID;
    }    
}
/*
 *	set http context id of brw_info_ptr
 */
PUBLIC void BrwSetHttpContextID(BRW_INFO_T *brw_info_ptr,uint32 context_id)
{
    if(brw_info_ptr != PNULL)
    {
        brw_info_ptr->http_context_id = context_id;
    }
}
/*
 *	desc:get invalid context id value
 */
PUBLIC uint32 BrwGetInvalidHttpContextIDValue(void)
{
    return HTTP_INVALID_CONTEXT_ID;
}
LOCAL void SendAuthRsp(
					   uint8* user_name_ptr,
					   uint8* pwd_ptr,
					   BRW_PROTOCOL_TYPE_E protocol,
					   HTTP_REQ_INFO_T *http_req_ptr
					   )
{
    BRW_INFO_T  *brw_info_ptr = PNULL;
    HTTP_AUTH_PARAM_T auth_parm={0};
	
    if (PNULL == http_req_ptr)
    {
        return;
    }

    brw_info_ptr = BrwGetBrwInfoPtrByHttpReq(http_req_ptr);
    if (PNULL == brw_info_ptr)
    {
        return;
    }
  
    auth_parm.password_ptr = (char*)pwd_ptr;
    auth_parm.user_name_ptr = (char*)user_name_ptr;

    switch(protocol)
    {
    case BRW_PROTOCOL_HTTP:
        HTTP_AuthResponse(brw_info_ptr->http_context_id,&auth_parm,http_req_ptr->id, http_req_ptr->http_request_id);
        break;

    case BRW_PROTOCOL_WAP:
        WSP_AuthResponse(brw_info_ptr->http_context_id,&auth_parm,http_req_ptr->id, http_req_ptr->http_request_id);
        break;

    default:
        //BRWDebug_Printf:"[BRW]W SendAuthRsp: unsupported protocol"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_1464_112_2_18_2_7_19_712,(uint8*)"");
        break;
    }
}
/*
 *	desc:发送http auth resp消息进行鉴权
 *  参数：req_ptr  http请求指针
 *        
 */
PUBLIC void BrwHttpAuthResp(uint32 http_req_id,uint8* user_name_ptr,uint8* pwd_ptr,BOOLEAN result)
{
    HTTP_REQ_INFO_T *http_req_ptr = PNULL;
    BRW_PUBLIC_PROFILE_T    *public_profile_ptr = BrwGetPublicProfile();

    http_req_ptr = (HTTP_REQ_INFO_T*)BRWList_SearchObject(&g_http_req_list,(BRW_LIST_CMP_F)CompareReqID,(void*)http_req_id);
    
    if (http_req_ptr != PNULL)
    {
        BRW_INFO_T  *brw_info_ptr = BrwGetBrwInfoPtrByHttpReq(http_req_ptr);
        
        if (brw_info_ptr != PNULL)
        {
            if (!result)
            {
                BRW_REQ_STATE_E state = BRW_STATE_MAX;
                WAP_REQUEST_T   *wap_req_ptr = PNULL;  
                
                wap_req_ptr = BrwGetWapReqByHttpReq(brw_info_ptr,http_req_ptr);
                state = BrwGetWapReqState(wap_req_ptr);
                //BRWDebug_Printf:"[BRW]BrwHttpAuthResp:Cancel state =%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_1492_112_2_18_2_7_19_713,(uint8*)"d", state);

                switch (state)
                {
                case BRW_STATE_PAGE_REQUESTING:
                    BRW_StopPageReq(brw_info_ptr->instance_id);
                    break;
                    
                case BRW_STATE_MEDIA_REQUESTING:
                    BrwProcHttpErr(brw_info_ptr, http_req_ptr, BRW_ERR_UNKNOWN);
                    //BrwCancelMediaReq(brw_info_ptr,wap_req_ptr,http_req_ptr);
                    break;
                    
                case BRW_STATE_DOWNLOADING:
                    BrwProcDownloadErr(brw_info_ptr, wap_req_ptr, BRW_ERR_UNKNOWN);
                    break;
                    
                default: //can't find ,so handle the http request only.
                    if (BRW_HTTP_STATE_INITED == brw_info_ptr->http_state)
                    {
                        BrwCancelHttpRequest(http_req_ptr,brw_info_ptr->http_context_id,public_profile_ptr->protocol);
                    }
                    BrwFreeHttpRequest(http_req_ptr);
                    break;
                }
            }//if(!result)
            else
            {
                SendAuthRsp(
                        user_name_ptr,
                        pwd_ptr,
                        public_profile_ptr->protocol,
                        http_req_ptr
                        );
            }         
        }
        else
        {            
            //BRWDebug_Printf:"[BRW]W BrwHttpAuthResp:brw_info_ptr null"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_1524_112_2_18_2_7_19_714,(uint8*)"");
        }        
    }
    else
    {            
        //BRWDebug_Printf:"[BRW]W BrwHttpAuthResp:http_req_ptr null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_1529_112_2_18_2_7_19_715,(uint8*)"");
    }      
}
/*
 *	desc:clear cache
 */
PUBLIC BOOLEAN BrwClearCache(uint16 *cache_path_ptr, BRW_PROTOCOL_TYPE_E protocol)
{
    BOOLEAN result = TRUE;
    
    if (PNULL == cache_path_ptr)
    {
        return FALSE;
    }

    switch (protocol)
    {
    case BRW_PROTOCOL_HTTP:
        if(HTTP_SUCCESS != HTTP_CacheClear(cache_path_ptr))
        {
            result = FALSE;
        }
        break;
        
    case BRW_PROTOCOL_WAP:
        if(HTTP_SUCCESS != WSP_CacheClear(cache_path_ptr))
        {
            result = FALSE;
        }
        break;
        
    default:
        //BRWDebug_Printf:"BrwClearCache error: unsupported protocol"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_1560_112_2_18_2_7_20_716,(uint8*)"");
        result = FALSE;
        break;
    }
    
    return result;
}
/*
 *	desc:clear cookie
 */
PUBLIC BOOLEAN BrwClearCookie(uint16 *cookie_path_ptr,BRW_PROTOCOL_TYPE_E protocol,uint32 http_context_id)
{
    BOOLEAN result = TRUE;
    
    if (PNULL == cookie_path_ptr)
    {
        return FALSE;
    }

    switch (protocol)
    {
    case BRW_PROTOCOL_HTTP:
        if(HTTP_SUCCESS != HTTP_CookieClear(http_context_id,cookie_path_ptr))
        {
            result = FALSE;
        }
        break;
        
    case BRW_PROTOCOL_WAP:
        if(HTTP_SUCCESS != WSP_CookieClear(http_context_id,cookie_path_ptr))
        {
            result = FALSE;
        }
        break;
        
    default:
        //BRWDebug_Printf:"BrwClearCookie error: unsupported protocol"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_1595_112_2_18_2_7_20_717,(uint8*)"");
        result = FALSE;
        break;
    }
    
    return result;
}

/*
 *	desc:set cookie
 */
PUBLIC BOOLEAN BrwSetCookie(uint32 http_context_id, uint8 *cookie_ptr)
{
    HTTP_ERROR_E            error_code    = HTTP_SUCCESS;
    HTTP_COOKIE_SET_PARAM_T cookie_param  = {0};
    BRW_INFO_T              *brw_info_ptr = GetInstanceByContextID(http_context_id);

    if (NULL == brw_info_ptr || NULL == cookie_ptr)
    {
        return FALSE;
    }   

    cookie_param.context_id = (HTTP_CONTEXT_ID_T)http_context_id;
    cookie_param.cookie_value_ptr = cookie_ptr;
    cookie_param.url_ptr = (char *)brw_info_ptr->cur_req_ptr->absolute_url;
    cookie_param.app_instance = brw_info_ptr->cur_req_ptr->req_id;
    
    error_code = HTTP_CookieSetReq(&cookie_param);
    if(HTTP_SUCCESS != error_code)
    {
        //set cookie error
        BRWDebug_Printf("[BRW]BrwSetCookie err code=%d",error_code);
        return FALSE;
    }

    return TRUE;
}

/*
 *	desc:设置http下载的内容保存的全文件名
 *  param: context_id       http 的context id
 *         http_req_ptr     http请求指针
 *         file_name_ptr    文件名
 *         file_name_len    文件名长度  
 */
PUBLIC BOOLEAN BrwSetHttpDLFileNm(uint32 context_id,HTTP_REQ_INFO_T* http_req_ptr,const uint16* file_name_ptr,uint16 file_name_len)
{
    if ((http_req_ptr != PNULL) && (file_name_ptr != PNULL))
    {
        if(HTTP_SUCCESS != HTTP_HeaderResponse(context_id,http_req_ptr->id,http_req_ptr->http_request_id,
                            HTTP_DATA_TRANS_STYLE_FILE,(HTTP_UNICODE_T*)file_name_ptr,file_name_len*sizeof(uint16)))
        {
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }
    return FALSE;
}

#ifdef SSL_SUPPORT
/*
 *	desc:process HTTP_SIG_SSL_CERT_IND
 */
LOCAL void ProcSSLCertInd(HTTP_SSL_CERT_UNTRUST_IND_SIG_T* ind_ptr)
{
    //BRWDebug_Printf:"[BRW]ProcSSLCertInd"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_1630_112_2_18_2_7_20_718,(uint8*)"");
    
    if (ind_ptr != PNULL)
    {
        BRW_INFO_T  *brw_info_ptr = PNULL;
        HTTP_REQ_INFO_T *http_req_ptr = PNULL;     
        uint32 context_id = ind_ptr->context_id;
        uint32 req_id = (uint32)ind_ptr->app_instance;
        
        http_req_ptr = (HTTP_REQ_INFO_T*)BRWList_SearchObject(&g_http_req_list,(BRW_LIST_CMP_F)CompareReqID,(void*)req_id);

        if (PNULL == http_req_ptr)
        {
           //可能是之前已经发出过cancel请求的
           //BRWDebug_Printf:"[BRW]ProcSSLCertInd http_req_ptr null"
           SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_1644_112_2_18_2_7_20_719,(uint8*)"");
           return;
        }       
        brw_info_ptr = GetInstanceByContextID(context_id);
        BrwProcSSLCert(brw_info_ptr,http_req_ptr);
    }
}
/*
 *	desc:进行不安全证书继续的确认
 *       http_req_id  http请求本地id
 *       result       是否继续
 */
PUBLIC void BrwHttpSSLCertResp(uint32 http_req_id,BOOLEAN result)
{
    HTTP_REQ_INFO_T *http_req_ptr = PNULL;
    BRW_PUBLIC_PROFILE_T    *public_profile_ptr = BrwGetPublicProfile();

    http_req_ptr = (HTTP_REQ_INFO_T*)BRWList_SearchObject(&g_http_req_list,(BRW_LIST_CMP_F)CompareReqID,(void*)http_req_id);

    if (http_req_ptr != PNULL)
    {
        BRW_INFO_T  *brw_info_ptr = BrwGetBrwInfoPtrByHttpReq(http_req_ptr);
        
        if(brw_info_ptr != PNULL)
        {
            switch (public_profile_ptr->protocol)
            {
            case BRW_PROTOCOL_HTTP:
            {
                HTTP_SslCertResponse(brw_info_ptr->http_context_id,(HTTP_APP_INSTANCE_T)http_req_id,
                        http_req_ptr->http_request_id,result);
            }            
                break;
                
            case BRW_PROTOCOL_WAP:
            {
                WSP_WtlsCertResponse(brw_info_ptr->http_context_id, (HTTP_APP_INSTANCE_T)http_req_id,
                        http_req_ptr->http_request_id,result);
            }
                break;
                
            default:
                //BRWDebug_Printf:"[BRW]BrwHttpSSLCertResp: invalid protocol"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_1685_112_2_18_2_7_20_720,(uint8*)"");
                break;
            }
                
            if (!result)//@fen.xie MS00218501
            {
                BrwProcErr(brw_info_ptr, BRW_ERR_NO_ERROR);
            }

        }
        else
        {
            //BRWDebug_Printf:"[BRW]BrwHttpSSLCertResp:brw_info_ptr null"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_1697_112_2_18_2_7_20_721,(uint8*)"");
        }        
    }
}
#endif
/*
 *	get next http request id
 */
LOCAL uint32 GetNextHttpReqID(void)
{
    return g_http_next_id++;
}
/*
 *	功能：处理http返回的错误
 *        brw_info_ptr  浏览器句柄
 *        err           HTTP返回的错误类型
 */
LOCAL void ProcHttpErr(BRW_INFO_T* brw_info_ptr,HTTP_REQ_INFO_T* http_req_ptr,HTTP_ERROR_E err)
{
    BRW_ERROR_E err_code = BRW_ERR_NO_ERROR;
    
    //BRWDebug_Printf:"[BRW]W ProcHttpErr http err=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_1716_112_2_18_2_7_20_722,(uint8*)"d", err);
    
    switch(err)
    {
    case HTTP_ERROR_NO_MEMORY:
        err_code = BRW_ERR_NO_MEM;
        break;
        
    case HTTP_ERROR_SERVER_CLOSE:
        err_code = BRW_ERR_SERVER_RESET;
        break;
        
    case HTTP_ERROR_TIMEOUT:
        err_code = BRW_ERR_CONNECT_TIMEOUT;
        break;
        
    case HTTP_ERROR_CONNECT:
        err_code = BRW_ERR_CONNECT_FAILED;
        break;
        
    default:
        err_code = BRW_ERR_HTTP_RTN;
        break;
    }
    BrwProcHttpErr(brw_info_ptr,http_req_ptr,err_code);
}
/*
 *	process HTTP_SIG_CANCEL_CNF
 *	notes:MS00233946 HTTP为修改下载的实现,只写下载的文件,故文件的删除需要在APP来完成
 */
LOCAL void ProcCancelCnf(HTTP_CANCEL_CNF_SIG_T* cnf)
{
}

/*
 *	desc:process http task's signal
 *  param:msg_id  message id
 *        param   message parameter
 *  TRUE if msg is http's message,else FALSE
 */
PUBLIC BOOLEAN BrwProcHttpMsg(uint32 msg_id,void* param)
{
    BOOLEAN ret=TRUE;
    void    *original_param = (void*)((uint32)param);
    BRW_INFO_T  *brw_info_ptr = PNULL;

    switch(msg_id)
    {
    case HTTP_SIG_INIT_CNF:
    case WSP_SIG_INIT_CNF:
    {
        HTTP_INIT_CNF_SIG_T *cnf = (HTTP_INIT_CNF_SIG_T*)original_param;

        if(cnf->module_id != BRW_HTTP_MODULE_WAP_ID)
        {
            return FALSE;
        }

        brw_info_ptr = BrwGetInstanceBySubModuleID((uint32)cnf->app_param);
        //The brw instance has been destroyed, to close http session directly
        if (PNULL == brw_info_ptr)
        {
            //SCI_TRACE_LOW:"BRW ProcInitSessionCnf, the session inited has been destroyed!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_1776_112_2_18_2_7_20_723,(uint8*)"");
            if (HTTP_SIG_INIT_CNF == msg_id)
            {
                BrwCloseHttpSession(cnf->context_id, BRW_PROTOCOL_HTTP);
            }
            else if (WSP_SIG_INIT_CNF == msg_id)
            {
                BrwCloseHttpSession(cnf->context_id, BRW_PROTOCOL_WAP);
            }
            else
            {
                //do nothing
            }
            return TRUE;
        }

        //BRWDebug_Printf:"BrwProcHttpMsg HTTP_SIG_INIT_CNF,context_id:%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_1792_112_2_18_2_7_20_724,(uint8*)"d", cnf->context_id);
        ProcInitSessionCnf(cnf->context_id,(uint32)cnf->app_param,cnf->result);
    }        
        break;
        
    case HTTP_SIG_CLOSE_CNF:
    case WSP_SIG_CLOSE_CNF:
    {
        HTTP_CLOSE_CNF_SIG_T *cnf = (HTTP_CLOSE_CNF_SIG_T*)original_param;

        brw_info_ptr = GetInstanceByContextID(cnf->context_id);
        if(PNULL == brw_info_ptr)
        {
            return FALSE;
        }
        //BRWDebug_Printf:"BrwProcHttpMsg HTTP_SIG_CLOSE_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_1807_112_2_18_2_7_20_725,(uint8*)"");
        ProcCloseSessionCnf(cnf->context_id);
    }
        break;
        
    case HTTP_SIG_CANCEL_CNF:
    case WSP_SIG_CANCEL_CNF:
    {
        HTTP_CANCEL_CNF_SIG_T *cnf = (HTTP_CANCEL_CNF_SIG_T*)original_param;

        MMIBROWSER_DelInvalidDlFile();
#ifdef BROWSER_GZIP_SUPPORT
        BrwDelDlTempFile();
#endif

        brw_info_ptr = GetInstanceByContextID(cnf->context_id);
        if(PNULL == brw_info_ptr)
        {
            return FALSE;
        }

        //BRWDebug_Printf:"BrwProcHttpMsg HTTP_SIG_CANCEL_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_1828_112_2_18_2_7_20_726,(uint8*)"");
        ProcCancelCnf((HTTP_CANCEL_CNF_SIG_T*)original_param);
    }
        break;
        
    case HTTP_SIG_DATA_IND:
    case WSP_SIG_DATA_IND:
    {
        HTTP_DATA_IND_SIG_T *cnf = (HTTP_DATA_IND_SIG_T*)original_param;

        brw_info_ptr = GetInstanceByContextID(cnf->context_id);
        if(PNULL == brw_info_ptr)
        {
            return FALSE;
        }
        //BRWDebug_Printf:"BrwProcHttpMsg HTTP_SIG_DATA_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_1843_112_2_18_2_7_20_727,(uint8*)"");
        ProcDataInd((HTTP_DATA_IND_SIG_T*)original_param);
    }
        break;

    case HTTP_SIG_ERROR_IND:
    case WSP_SIG_ERROR_IND:
    {
        HTTP_ERROR_IND_SIG_T *cnf = (HTTP_ERROR_IND_SIG_T*)original_param;

        brw_info_ptr = GetInstanceByContextID(cnf->context_id);
        if(PNULL == brw_info_ptr)
        {
            return FALSE;
        }
        //BRWDebug_Printf:"BrwProcHttpMsg HTTP_SIG_ERROR_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_1858_112_2_18_2_7_20_728,(uint8*)"");
        ProcErrInd((HTTP_ERROR_IND_SIG_T*)original_param);
    }
        break;
        
    case HTTP_SIG_GET_CNF:
    case WSP_SIG_GET_CNF:
    {
        HTTP_GET_CNF_SIG_T *cnf = (HTTP_GET_CNF_SIG_T*)original_param;

        brw_info_ptr = GetInstanceByContextID(cnf->context_id);
        //BRWDebug_Printf:"BrwProcHttpMsg HTTP_SIG_GET_CNF,context_id:%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_1869_112_2_18_2_7_20_729,(uint8*)"d",cnf->context_id);
        if(PNULL == brw_info_ptr)
        {
            return FALSE;
        }
        //BRWDebug_Printf:"BrwProcHttpMsg HTTP_SIG_GET_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_1874_112_2_18_2_7_20_730,(uint8*)"");
        ProcGetCnf((HTTP_GET_CNF_SIG_T*)original_param);
    }
        break;

    case HTTP_SIG_HEADER_IND:
    case WSP_SIG_HEADER_IND:
    {
        HTTP_HEADER_IND_SIG_T *cnf = (HTTP_HEADER_IND_SIG_T*)original_param;

        brw_info_ptr = GetInstanceByContextID(cnf->context_id);
        if(PNULL == brw_info_ptr)
        {
            return FALSE;
        }
        //BRWDebug_Printf:"BrwProcHttpMsg HTTP_SIG_HEADER_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_1889_112_2_18_2_7_20_731,(uint8*)"");
        ProcHeaderInd((HTTP_HEADER_IND_SIG_T*)original_param);
    }
        break;
        
    case HTTP_SIG_NEED_AUTH_IND:
    case WSP_SIG_NEED_AUTH_IND:
    {
        HTTP_AUTH_IND_SIG_T *cnf = (HTTP_AUTH_IND_SIG_T*)original_param;

        brw_info_ptr = GetInstanceByContextID(cnf->context_id);
        if(PNULL == brw_info_ptr)
        {
            return FALSE;
        }
        //BRWDebug_Printf:"BrwProcHttpMsg HTTP_SIG_NEED_AUTH_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_1904_112_2_18_2_7_20_732,(uint8*)"");
        ProcNeedAuthInd((HTTP_AUTH_IND_SIG_T*)original_param);
    }
        break;
        
    case HTTP_SIG_NET_PROG_IND:
    case WSP_SIG_NET_PROG_IND:
    {
        HTTP_NET_PROG_IND_SIG_T *cnf = (HTTP_NET_PROG_IND_SIG_T*)original_param;

        brw_info_ptr = GetInstanceByContextID(cnf->context_id);
        if(PNULL == brw_info_ptr)
        {
        	return FALSE;
        }
        
        if (NET_PROGRESS_RECEIVING == cnf->net_prog_op_code)
        {
            ProcNetProgressInd((HTTP_NET_PROG_IND_SIG_T*)original_param);
        }
#ifdef HTTP2_SUPPORT
        else if (NET_PROGRESS_DISCONNECTED == cnf->net_prog_op_code)
        {
            brw_info_ptr->curr_host_is_http2 = FALSE;
            BRWMem_Free(brw_info_ptr->curr_host_url);
            brw_info_ptr->curr_host_url = PNULL;
        }
#endif
    }
        break;
    case HTTP_SIG_POST_CNF:
    case WSP_SIG_POST_CNF:
    {
        HTTP_POST_CNF_SIG_T *cnf = (HTTP_POST_CNF_SIG_T*)original_param;

        brw_info_ptr = GetInstanceByContextID(cnf->context_id);
        if(PNULL == brw_info_ptr)
        {
            return FALSE;
        }
        //BRWDebug_Printf:"BrwProcHttpMsg HTTP_SIG_POST_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_1936_112_2_18_2_7_20_733,(uint8*)"");
        ProcPostCnf((HTTP_POST_CNF_SIG_T*)original_param);
    }
        break;
        
    case HTTP_SIG_REDIRECT_IND:
    case WSP_SIG_REDIRECT_IND:
    {
        HTTP_REDIRECT_IND_SIG_T *cnf = (HTTP_REDIRECT_IND_SIG_T*)original_param;

        brw_info_ptr = GetInstanceByContextID(cnf->context_id);
        if(PNULL == brw_info_ptr)
        {
            return FALSE;
        }
        //BRWDebug_Printf:"BrwProcHttpMsg HTTP_SIG_REDIRECT_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_1951_112_2_18_2_7_20_734,(uint8*)"");
        ProcRedirectInd((HTTP_REDIRECT_IND_SIG_T*)original_param);
    }
        break;

#ifdef HTTP2_SUPPORT
    case HTTP_SIG_VERSION_IND:
    {
         HTTP_VERSION_IND_SIG_T *cnf = (HTTP_VERSION_IND_SIG_T*)original_param;

        brw_info_ptr = GetInstanceByContextID(cnf->context_id);
        if(PNULL == brw_info_ptr)
        {
            return FALSE;
        }
        SCI_TRACE_LOW("[%s:%d]HTTP_SIG_VERSION_IND version: %s, host_url: %s",__FUNCTION__,__LINE__,cnf->version,cnf->host_url);
        ProcVersionInd((HTTP_VERSION_IND_SIG_T*)original_param);
    }
    break;
#endif
   
    case HTTP_SIG_REQUEST_ID_IND:
    case WSP_SIG_REQUEST_ID_IND:
    {
        HTTP_REQUEST_ID_IND_SIG_T *cnf = (HTTP_REQUEST_ID_IND_SIG_T*)original_param;

        brw_info_ptr = GetInstanceByContextID(cnf->context_id);
        if(PNULL == brw_info_ptr)
        {
            return FALSE;
        }
        //BRWDebug_Printf:"BrwProcHttpMsg HTTP_SIG_REQUEST_ID_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_1966_112_2_18_2_7_20_735,(uint8*)"");
        ProcRequestIDInd((HTTP_REQUEST_ID_IND_SIG_T*)original_param);
    }
    break;
    
#ifdef SSL_SUPPORT
    case HTTP_SIG_SSL_CERT_UNTRUST_IND:
    case WSP_SIG_WTLS_UNTRUST_IND:
        //BRWDebug_Printf:"BrwProcHttpMsg HTTP_SIG_SSL_CERT_UNTRUST_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HTTPINTERFACE_1974_112_2_18_2_7_20_736,(uint8*)"");
        ProcSSLCertInd((HTTP_SSL_CERT_UNTRUST_IND_SIG_T*)original_param);
        break;
        
    case HTTP_SIG_SSL_GET_CERT_CNF:
        break;
#endif

    default:
        ret=FALSE;
        break;
    }
    
    return ret;
}
/*
 *	desc:get the accept language header's index array
 *	notes:
 *  param:
 *  return:
 */
LOCAL void GetLanguageHeaderArrByType(
                MMISET_LANGUAGE_TYPE_E language_type, 
                uint16  **language_header_arr_pptr,//[IN/OUT]
                uint32  *language_header_arr_len_ptr//[IN/OUT]
                )
{
    uint32  i = 0;
    uint16  *language_header_arr_ptr = PNULL;

    if ((PNULL == language_header_arr_pptr) || (PNULL == *language_header_arr_pptr) 
        || (PNULL == language_header_arr_len_ptr) || (0 == *language_header_arr_len_ptr))
    {
        BRWDebug_PrintWarning("GetLanguageHeaderArrByType:param error");
        return;
    }
    
    language_header_arr_ptr = *language_header_arr_pptr;
    
    for(i = 0;i < TOTAL_LANGUAGE_HEADER;i++)
    {
        if (language_type == s_accept_language_header[i].language_type)
        {
            if (*language_header_arr_len_ptr > 0)
            {
                *language_header_arr_ptr = i;
                (*language_header_arr_len_ptr)--;
                language_header_arr_ptr++;
            }
        }
    }
    
    *language_header_arr_pptr = language_header_arr_ptr;
}

/*
 *	desc:get the accept language header by the platform
 *	notes:
 *  param:
 *  return:result
 */
LOCAL void GetAcceptLanguageHeader(
            char **accept_language_pptr//[IN/OUT]:*accept_language_pptr need free by FreeAcceptLanguageHeader
            )
{
    MMISET_LANGUAGE_TYPE_E  cur_language = MMISET_MAX_LANGUAGE;
    uint32  i = 0;
    uint16  language_header_num = 0;
    uint32  language_header_arr_len = 2*MMISET_MAX_LANGUAGE;
    uint16  language_header_arr[2*MMISET_MAX_LANGUAGE] = {0};
    uint16  *language_header_arr_ptr = language_header_arr;
    char    *accept_language_ptr = PNULL;
    uint32  accept_language_total_len = 0;
    char    *pre_header_ptr = PNULL;
    uint16    pre_header_len = 0;

    if (PNULL == accept_language_pptr)
    {
        BRWDebug_PrintWarning("GetAcceptLanguageHeader:param error");
        return;
    }
    //FreeAcceptLanguageHeader(accept_language_pptr);

    //begin:get the arr            
    MMIAPISET_GetLanguageType(&cur_language);
    if (cur_language < MMISET_MAX_LANGUAGE)
    {
        GetLanguageHeaderArrByType(cur_language, &language_header_arr_ptr, &language_header_arr_len);
    }
    
    for(i = 0;i < MMISET_MAX_LANGUAGE;i++)
    {    
        if (0 == language_header_arr_len)
        {
            break;
        }
        
        if (i != cur_language)
        {
        	if (MMIAPISET_CheckLanguageStatus((MMISET_LANGUAGE_TYPE_E)i))
    	    {
                GetLanguageHeaderArrByType(i, &language_header_arr_ptr, &language_header_arr_len);
            }
        }
    }
    
    language_header_num = 2*MMISET_MAX_LANGUAGE - language_header_arr_len;
    if (0 == language_header_num)
    {
        language_header_arr[0] = 0;//default set en
        language_header_num = 1;
    }
    //end:get the arr            

    //combine the accept language header string
    accept_language_total_len = language_header_num * BRW_PER_ACCEPT_LANGUAGE_MAX;
    accept_language_ptr = BRWMem_Alloc(accept_language_total_len + 1);
    if (PNULL == accept_language_ptr )
    {
        BRWDebug_PrintWarning("GetAcceptLanguageHeader:alloc error");
        return;
    }  
    *accept_language_pptr = accept_language_ptr;//save it
    BRW_Memset(accept_language_ptr, 0x00, (accept_language_total_len + 1));

    pre_header_ptr = s_accept_language_header[language_header_arr[0]].language_header_ptr;
    pre_header_len = BRW_Strlen((uint8*)pre_header_ptr);
        
    BRW_Strncpy((uint8*)accept_language_ptr , 
        (uint8*)pre_header_ptr, 
        pre_header_len);
    accept_language_total_len -= pre_header_len;
    accept_language_ptr += pre_header_len;
        
    for(i = 1;i < language_header_num;i++)
    {
        pre_header_ptr = s_accept_language_header[language_header_arr[i]].language_header_ptr;
        pre_header_len = BRW_Strlen((uint8*)pre_header_ptr);
        
        if (accept_language_total_len > pre_header_len)//include ","
        {
            BRW_Strcpy((uint8*)accept_language_ptr, (const uint8*)",");
            accept_language_total_len--;
            accept_language_ptr++;
            
            BRW_Strcpy((uint8*)accept_language_ptr, (const uint8*)pre_header_ptr);
            accept_language_total_len -= pre_header_len;
            accept_language_ptr += pre_header_len;
        }
    }
}

/*
 *	desc:get the accept language header by the platform
 *	notes:
 *  param:
 *  return:result
 */
LOCAL void FreeAcceptLanguageHeader(
            char **accept_language_pptr//[IN\OUT]:free *accept_language_pptr
            )
{
    if ((accept_language_pptr != PNULL) && (*accept_language_pptr != PNULL))
    {
        BRWMem_Free(*accept_language_pptr);
		*accept_language_pptr = PNULL;
    }
}

/*
 *	desc:the init operations in http interface module
 *	notes:
 *  param:
 *  return:result
 */
PUBLIC void Brw_HttpInit(void)
{
    GetAcceptLanguageHeader(&s_accept_language_ptr);
    //add new
}

/*
 *	desc:the exit operations in http interface module
 *	notes:
 *  param:
 *  return:result
 */
PUBLIC void Brw_HttpExit(void)
{
    FreeAcceptLanguageHeader(&s_accept_language_ptr);
    //add new
}
