
#ifndef  _MMI_HTTP_NET_C  
#define _MMI_HTTP_NET_C  

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "zdthttp_app.h"
#include "zdthttp_net.h"
#include "http_api.h"

#ifdef WIFI_SUPPORT
#include "wifisupp_api.h"
#endif

#ifdef FILE_LOG_SUPPORT
#define HTTP_NET_TRACE			Trace_Log_Buf_Print
#else
#define HTTP_NET_TRACE            SCI_TRACE_LOW
#endif

#define ZDT_HTTP_APPFLAG (0xca9a)

#define HTTP_PDP_ACTIVE_TIMEOUT       30000  
#define HTTP_PDP_RETRY_TIMEOUT        2000 //重新尝试连接pdp，换卡连接

#define HTTP_PDP_DEFAULT_SIM_SYS MN_DUAL_SYS_1

LOCAL MN_DUAL_SYS_E       s_http_net_set_sys   = HTTP_PDP_DEFAULT_SIM_SYS;
LOCAL MN_DUAL_SYS_E       s_http_net_cur_sys   = MN_DUAL_SYS_1;
LOCAL uint8                       s_http_net_pdp_timer_id      = 0;
LOCAL uint8                       s_http_net_pdp_retry_timer_id = 0;
LOCAL uint32                      s_http_net_id            = 0;
LOCAL BOOLEAN                  s_http_net_pdp_state_is_ok        = FALSE;
LOCAL BOOLEAN                  s_http_net_is_init        = FALSE;
LOCAL BOOLEAN                  s_http_net_need_init        = TRUE;
LOCAL BOOLEAN                  s_http_net_gprs_status[MN_DUAL_SYS_MAX+1] = {0};
LOCAL BOOLEAN                  s_http_net_is_activing        = FALSE;
LOCAL BOOLEAN                  s_http_net_is_closing        = FALSE;

LOCAL BOOLEAN                  s_http_net_need_active_close        = FALSE;
LOCAL BOOLEAN                  s_http_net_need_close_active        = FALSE;

LOCAL BOOLEAN  HTTP_Net_PDPTry(MN_DUAL_SYS_E *  p_cur_sys);
LOCAL BOOLEAN HTTP_Net_PdpDeactive(void);
LOCAL BOOLEAN HTTP_Net_PdpActive(void);

LOCAL BOOLEAN HTTP_Net_Send_CallBack(BOOLEAN is_ok,uint32 err_id)
{
    if(MMIZDT_HTTP_IsSending())
    {
        if(is_ok)
        {
            MMIZDT_HTTP_PopSend_Result(FALSE,HTTP_SUCCESS);
        }
        else
        {
            MMIZDT_HTTP_PopSend_Result(FALSE,err_id);
        }
    }
}

#if 0
PUBLIC uint32 HTTP_Get_ContentLength(const char *data_str, int data_len)
{
    int ret = 0;
    uint8 * pTime = NULL;
    uint32 con_len = 0;
    char tmp_str[1001] = {0};
    
    if(data_str == NULL || data_len == 0)
    {
        return;
    }
    
    pTime = (uint8 *)data_str;
    con_len = data_len;
    while(ret != -1)
    {
        ret = YX_Func_CR_GetNextPara(&pTime,&con_len,tmp_str,1000);
        if(ret > 0)
        {
            if(
            SCI_TraceLow("[CR]:%s ",tmp_str);
        }
    }
}
#endif
#if 1
LOCAL void Http_GetSignalStruct(DPARAM param, void *signal_struct_ptr, uint16 struct_size);

#define CTREGHTTP_ACCEPT_STRING "text/vnd.wap.wml,application/vnd.wap.xhtml+xml,application/xhtml+xml,\
image/vnd.wap.wbmp,image/gif,image/jpg,image/jpeg,image/png,image/bmp,text/html,\
application/vnd.oma.drm.message,application/vnd.oma.drm.content,application/vnd.oma.drm.rights+xml,application/vnd.oma.drm.rights+wbxml,\
*/*;q=0.1"
/********/
#define CTREGHTTP_ACCEPT_CHARSET_STRING "UTF-8,ISO-8859-1,US-ASCII,windows-1252,windows-1251"

//#define CTREGHTTP_ACCEPT_STRING "text/plain,text/html"
#define CTREGHTTP_ACCEPT_LANGUAGE_STRING  "zh-cn, en" //表示接受中文英文
#define CTREGHTTP_USER_AGENT_STRING       "Nokia 220 4G/2.0(p)"
//#define CTREGHTTP_ACCEPT_CHARSET_STRING   "UTF-8"

#define CTREGHTTP_HTTP_SIGNAL_ANALYTIC_EX(param,signal,app_param,http_context_ptr,app_module_id,sig_type) do{\
	    Http_GetSignalStruct(param, &signal, sizeof(sig_type));\
	    if(signal.module_id != app_module_id)\
	    {\
	    	return MMI_RESULT_FALSE;\
	    }\
	    app_param = signal.app_param;\
	    http_context_ptr->context_id   = signal.context_id;\
	    http_context_ptr->app_instance = 0;\
	    http_context_ptr->request_id = 0;\
	    http_context_ptr->module_id    = signal.module_id;\
}while(0)\

#define CTREGHTTP_HTTP_SIGNAL_ANALYTIC(param,signal,http_context_ptr,sig_type) do{\
	Http_GetSignalStruct(param, &signal, sizeof(sig_type));\
	if(signal.context_id != http_context_ptr->context_id)\
	{\
	    	return MMI_RESULT_FALSE;\
	}\
}while(0)\

typedef struct __CTRegHttp
{
    BOOLEAN flags;
    MN_DUAL_SYS_E sim_sys;
    uint32 net_id;
    BOOLEAN need_refresh;
}CTRegHttp;

typedef struct __CTREGHTTP_HTTP_CONTEXT
{
    HTTP_APP_MODULE_ID_T module_id;
    HTTP_CONTEXT_ID_T    context_id;
    HTTP_REQUEST_ID_T    request_id;
    HTTP_APP_INSTANCE_T  app_instance; //用于区分不同的HTTP请求。同时只有一个HTTP请求时可以为任意值。
}CTREGHTTP_HTTP_CONTEXT;

uint32 g_cur_http_content_len = 0;
uint32 g_cur_http_file_save_len = 0;
LOCAL uint32 g_cur_http_rcv_count = 0;
LOCAL CTREGHTTP_HTTP_CONTEXT   s_ctreghttp_http_context      = {0};
LOCAL CTRegHttp s_ctreghttphttp={0};
LOCAL uint8 *  s_p_cur_url = PNULL;
LOCAL uint8 *  s_p_refresh_url = PNULL;

MMIFILE_HANDLE g_zdt_http_rcf_file_handle = SFS_INVALID_HANDLE;

void Http_GetSignalStruct(DPARAM param, void *signal_struct_ptr, uint16 struct_size)
{
    if((PNULL != param)&&(PNULL != signal_struct_ptr)&&(struct_size >= sizeof(xSignalHeaderRec)))
    {
        SCI_MEMCPY((uint8 *)((uint32)signal_struct_ptr), param, (struct_size ));
    }
}

LOCAL MMI_RESULT_E Http_HandleGetRequest(CTREGHTTP_HTTP_CONTEXT * http_context_ptr, DPARAM param)
{
    int base_len = 0;
    MMI_RESULT_E        result = MMI_RESULT_FALSE;
    HTTP_INIT_CNF_SIG_T signal = {0};
    HTTP_APP_PARAM_T app_param = 0;
    HTTP_GET_PARAM_T* get_param_ptr= NULL;
    int file_read = 0;
    int len = 0;
    uint32 tickcount = 0;
    int url_len = 0;
    if(http_context_ptr == NULL)
    {
        return MMI_RESULT_FALSE;
    }

    if(s_p_cur_url != PNULL)
    {
        SCI_FREE(s_p_cur_url);
        s_p_cur_url = PNULL;
    }

    url_len = MMIZDT_HTTP_GetlCurAllUrl((uint8 **)&s_p_cur_url);
    if(url_len <= 0)
    {
        if(s_p_cur_url != NULL)
        {
            SCI_FREE(s_p_cur_url);
            s_p_cur_url = PNULL;
        }
        ZDTHTTP_LOG("[HTTP]:Http_HandleGetRequest Err URL Malloc");
        return MMI_RESULT_FALSE;
    }
    ZDTHTTP_LOG("[HTTP]:Http_HandleGetRequest");
    ZDT_HTTPSendTrace(s_p_cur_url,strlen(s_p_cur_url));

    CTREGHTTP_HTTP_SIGNAL_ANALYTIC_EX(param, signal, app_param, http_context_ptr,MMI_MODULE_ZDTHTTP,HTTP_INIT_CNF_SIG_T);
    
    get_param_ptr = (HTTP_GET_PARAM_T*)SCI_ALLOC_APPZ(sizeof(HTTP_GET_PARAM_T));

	if(get_param_ptr == NULL)
	{
		return MMI_RESULT_FALSE;
	}	

	get_param_ptr->connection = HTTP_CONNECTION_KEEP_ALIVE;
	get_param_ptr->need_net_prog_ind = FALSE;
	len=strlen(CTREGHTTP_ACCEPT_STRING);
	get_param_ptr->accept.accept_ptr = SCI_ALLOC(len+1);
	memset(get_param_ptr->accept.accept_ptr,0,len+1);
	strcpy(get_param_ptr->accept.accept_ptr,CTREGHTTP_ACCEPT_STRING);
#if 1
	len=strlen(CTREGHTTP_ACCEPT_LANGUAGE_STRING);
	get_param_ptr->accept_language.accept_language_ptr = SCI_ALLOC(len+1);
	memset(get_param_ptr->accept_language.accept_language_ptr,0,len+1);
	strcpy(get_param_ptr->accept_language.accept_language_ptr,CTREGHTTP_ACCEPT_LANGUAGE_STRING);
    
	len=strlen(CTREGHTTP_USER_AGENT_STRING);
	get_param_ptr->user_agent.user_agent_ptr = SCI_ALLOC(len+1);
	memset(get_param_ptr->user_agent.user_agent_ptr,0,len+1);
	strcpy(get_param_ptr->user_agent.user_agent_ptr,CTREGHTTP_USER_AGENT_STRING);	
#endif
    	//拼接URL
      get_param_ptr->uri.uri_ptr = (char *)s_p_cur_url;
    
      len = strlen(CTREGHTTP_ACCEPT_CHARSET_STRING);
      get_param_ptr->accept_charset.accept_charset_ptr = SCI_ALLOC(len + 1);
      memset(get_param_ptr->accept_charset.accept_charset_ptr,0,len + 1);
      strcpy(get_param_ptr->accept_charset.accept_charset_ptr,CTREGHTTP_ACCEPT_CHARSET_STRING);
      
#ifdef HTTP_SUPPORT
	if(!HTTP_GetRequest(http_context_ptr->context_id,get_param_ptr, http_context_ptr->app_instance))
	{
		result = MMI_RESULT_TRUE;
	}
#endif
    SCI_FREE(get_param_ptr->accept.accept_ptr);
    SCI_FREE(get_param_ptr->accept_charset.accept_charset_ptr);
    SCI_FREE(get_param_ptr->accept_language.accept_language_ptr);
    SCI_FREE(get_param_ptr->user_agent.user_agent_ptr);
    //SCI_FREE(get_param_ptr->uri.uri_ptr);
    SCI_FREE(get_param_ptr);
    return	result;	
}

LOCAL MMI_RESULT_E Http_HandlePostRequest(CTREGHTTP_HTTP_CONTEXT * http_context_ptr, DPARAM param)
{
    int base_len = 0;
    MMI_RESULT_E        result = MMI_RESULT_FALSE;
    HTTP_INIT_CNF_SIG_T signal = {0};
    HTTP_APP_PARAM_T app_param = 0;
    HTTP_POST_PARAM_T* post_param_ptr= NULL;
    int file_read = 0;
    int len = 0;
    uint32 tickcount = 0;
    int url_len = 0;
    if(http_context_ptr == NULL)
    {
        return MMI_RESULT_FALSE;
    }

    if(s_p_cur_url != PNULL)
    {
        SCI_FREE(s_p_cur_url);
        s_p_cur_url = PNULL;
    }

    url_len = MMIZDT_HTTP_GetCurUrl((uint8 **)&s_p_cur_url);
    if(url_len <= 0)
    {
        if(s_p_cur_url != NULL)
        {
            SCI_FREE(s_p_cur_url);
            s_p_cur_url = PNULL;
        }
        ZDTHTTP_LOG("[HTTP]:Http_HandlePostRequest Err URL Malloc");
        return MMI_RESULT_FALSE;
    }
    ZDTHTTP_LOG("[HTTP]:Http_HandlePostRequest %s",s_p_cur_url);
    CTREGHTTP_HTTP_SIGNAL_ANALYTIC_EX(param, signal, app_param, http_context_ptr,MMI_MODULE_ZDTHTTP,HTTP_INIT_CNF_SIG_T);
    
    post_param_ptr = (HTTP_POST_PARAM_T*)SCI_ALLOC_APPZ(sizeof(HTTP_POST_PARAM_T));

	if(post_param_ptr == NULL)
	{
		return MMI_RESULT_FALSE;
	}	

	post_param_ptr->connection = HTTP_CONNECTION_KEEP_ALIVE;
	post_param_ptr->need_net_prog_ind = FALSE;
	len=strlen(CTREGHTTP_ACCEPT_STRING);
	post_param_ptr->accept.accept_ptr = SCI_ALLOC(len+1);
	memset(post_param_ptr->accept.accept_ptr,0,len+1);
	strcpy(post_param_ptr->accept.accept_ptr,CTREGHTTP_ACCEPT_STRING);
#if 1
	len=strlen(CTREGHTTP_ACCEPT_LANGUAGE_STRING);
	post_param_ptr->accept_language.accept_language_ptr = SCI_ALLOC(len+1);
	memset(post_param_ptr->accept_language.accept_language_ptr,0,len+1);
	strcpy(post_param_ptr->accept_language.accept_language_ptr,CTREGHTTP_ACCEPT_LANGUAGE_STRING);
    
	len=strlen(CTREGHTTP_USER_AGENT_STRING);
	post_param_ptr->user_agent.user_agent_ptr = SCI_ALLOC(len+1);
	memset(post_param_ptr->user_agent.user_agent_ptr,0,len+1);
	strcpy(post_param_ptr->user_agent.user_agent_ptr,CTREGHTTP_USER_AGENT_STRING);	
#endif
    	//拼接URL
      post_param_ptr->uri.uri_ptr = (char *)s_p_cur_url;
    
      len = strlen(CTREGHTTP_ACCEPT_CHARSET_STRING);
      post_param_ptr->accept_charset.accept_charset_ptr = SCI_ALLOC(len + 1);
      memset(post_param_ptr->accept_charset.accept_charset_ptr,0,len + 1);
      strcpy(post_param_ptr->accept_charset.accept_charset_ptr,CTREGHTTP_ACCEPT_CHARSET_STRING);
      
    post_param_ptr->post_body.u.post_buffer.is_copied_by_http = FALSE;
    post_param_ptr->post_body.is_use_file = FALSE;
    post_param_ptr->post_body.body_type_ptr = "application/json;charset=utf-8";
    post_param_ptr->post_body.u.post_buffer.buffer_len = MMIZDT_HTTP_GetCurData(&post_param_ptr->post_body.u.post_buffer.buffer_ptr);
    post_param_ptr->is_use_post_after_redirect = TRUE;
    ZDT_HTTPSendTrace(post_param_ptr->post_body.u.post_buffer.buffer_ptr, post_param_ptr->post_body.u.post_buffer.buffer_len);
#ifdef HTTP_SUPPORT
	if(!HTTP_PostRequest(http_context_ptr->context_id,post_param_ptr, http_context_ptr->app_instance))
	{
		result = MMI_RESULT_TRUE;
	}
#endif
    SCI_FREE(post_param_ptr->accept.accept_ptr);
    SCI_FREE(post_param_ptr->accept_charset.accept_charset_ptr);
    SCI_FREE(post_param_ptr->accept_language.accept_language_ptr);
    SCI_FREE(post_param_ptr->user_agent.user_agent_ptr);
    //SCI_FREE(get_param_ptr->uri.uri_ptr);
    SCI_FREE(post_param_ptr);
    return result;
}

LOCAL MMI_RESULT_E Http_HandleRequestIdInd(CTREGHTTP_HTTP_CONTEXT *http_context_ptr, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;
    HTTP_REQUEST_ID_IND_SIG_T signal = {0};
	HTTP_REQUEST_ID_IND_SIG_T *cnf = (HTTP_REQUEST_ID_IND_SIG_T*)param;
   //当request_id为BILLING_REQUEST_ID_DEFAULT_VALUE时,则没被初始化过,属于本模块的消息
    CTREGHTTP_HTTP_SIGNAL_ANALYTIC(param,signal,http_context_ptr,HTTP_REQUEST_ID_IND_SIG_T);
	ZDTHTTP_LOG("[HTTP]:Http_HandleRequestIdInd receive HTTP_SIG_REQUEST_ID_IND");
	ZDTHTTP_LOG("[HTTP]:Http_HandleRequestIdInd receive http_context_ptr->context_id:%d",http_context_ptr->context_id);
    http_context_ptr->context_id = signal.context_id;
    http_context_ptr->app_instance = signal.app_instance;
    http_context_ptr->request_id = signal.request_id;
    ZDTHTTP_LOG("[HTTP]:Http_HandleRequestIdInd request_id:%ld,http_context_ptr->context_id:%d",signal.request_id,http_context_ptr->context_id);
    result = MMI_RESULT_TRUE;

    return result;
}

LOCAL MMI_RESULT_E Http_HandleDataInd(CTREGHTTP_HTTP_CONTEXT * http_context_ptr, DPARAM param)
{
    //---1--- 
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    HTTP_DATA_IND_SIG_T signal = {0};
    CTREGHTTP_HTTP_SIGNAL_ANALYTIC(param,signal,http_context_ptr,HTTP_DATA_IND_SIG_T);
    g_cur_http_rcv_count += signal.data_len;
    //ZDT_HTTP_File_Save(ZDTHTTP_RCV_TEMP_FULLPATCH,signal.data_ptr,signal.data_len,TRUE);
    g_cur_http_file_save_len += ZDT_HTTP_File_Write(g_zdt_http_rcf_file_handle,signal.data_ptr,signal.data_len);
    ZDTHTTP_LOG("[HTTP]:Http_HandleDataInd receive len=%d,g_cur_http_rcv_count=%d,%d",signal.data_len,g_cur_http_rcv_count,g_cur_http_file_save_len);
    ZDT_HTTPRcvTrace(signal.data_ptr, signal.data_len);
    ZDT_HTTP_RCV_Add(signal.data_ptr,signal.data_len);
    http_context_ptr->request_id = signal.request_id;
    http_context_ptr->app_instance = signal.app_instance;
    return result;
}

LOCAL MMI_RESULT_E Http_HandleGetCnf(CTREGHTTP_HTTP_CONTEXT * http_context_ptr, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
#ifdef HTTP_SUPPORT
    HTTP_GET_CNF_SIG_T signal = {0};
    CTREGHTTP_HTTP_SIGNAL_ANALYTIC(param,signal,http_context_ptr,HTTP_GET_CNF_SIG_T);
    ZDTHTTP_LOG("[HTTP]:HandleCTRegHttpGetCnf code=%d,need_refresh=%d",signal.rsp_header_info.response_code,s_ctreghttphttp.need_refresh);
    ZDT_HTTP_File_Close(g_zdt_http_rcf_file_handle);
    g_zdt_http_rcf_file_handle = SFS_INVALID_HANDLE;
    if(s_ctreghttphttp.need_refresh)
    {
        ZDT_HTTP_RCV_DelAll();
        MMIZDTHTTP_SendSig_Refresh(PNULL);
        s_ctreghttphttp.need_refresh = FALSE;
    }
    else
    {
        HTTP_CloseRequest(http_context_ptr->context_id);
    }
    
#endif
    return result;
}

LOCAL MMI_RESULT_E Http_HandlePostCnf(CTREGHTTP_HTTP_CONTEXT * http_context_ptr, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
#ifdef HTTP_SUPPORT
    HTTP_POST_CNF_SIG_T signal = {0};
    CTREGHTTP_HTTP_SIGNAL_ANALYTIC(param,signal,http_context_ptr,HTTP_POST_CNF_SIG_T);
    ZDTHTTP_LOG("[HTTP]:HandleCTRegHttpPostCnf code=%d",signal.rsp_header_info.response_code);
    ZDT_HTTP_File_Close(g_zdt_http_rcf_file_handle);
    g_zdt_http_rcf_file_handle = SFS_INVALID_HANDLE;

    if(s_ctreghttphttp.need_refresh)
    {
        ZDT_HTTP_RCV_DelAll();
        MMIZDTHTTP_SendSig_Refresh(PNULL);
        s_ctreghttphttp.need_refresh = FALSE;
    }
    else
    {
        HTTP_CloseRequest(http_context_ptr->context_id);
    }
#endif
    return result;
}
LOCAL BOOLEAN Http_File_Rcv_Full(uint8 f_status, uint32 need_len)
{
    BOOLEAN is_full = FALSE;
    if(f_status > 0)
    {
        if(f_status == 1)
        {
            is_full = ZDT_HTTP_MemFull(TRUE,need_len);
        }
        else
        {
            is_full = ZDT_HTTP_MemFull(FALSE,need_len);
        }                
    }
    return is_full;
}
LOCAL MMI_RESULT_E Http_HandleCloseCnf(CTREGHTTP_HTTP_CONTEXT *http_context_ptr, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;
    HTTP_CLOSE_CNF_SIG_T signal = {0};
    BOOLEAN nv_value = 1;
    uint8 * cur_filename = PNULL;
    uint32 cur_file_len = 0;
    uint32 err_id = HTTP_ERROR_HANDSHAKE;
    
    Http_GetSignalStruct(param, &signal, sizeof(HTTP_CLOSE_CNF_SIG_T));
    if(http_context_ptr->context_id == signal.context_id)
    {
        ZDTHTTP_LOG("[HTTP]:HandleCTRegHttpCloseCnf receive HTTP_SIG_CLOSE_CNF g_cur_http_rcv_count=%d,%d",g_cur_http_rcv_count,g_cur_http_file_save_len);
        ZDT_HTTP_File_Close(g_zdt_http_rcf_file_handle);
        g_zdt_http_rcf_file_handle = SFS_INVALID_HANDLE;
        http_context_ptr->context_id = 0;
        http_context_ptr->request_id = 0;
        result = MMI_RESULT_TRUE;
#if defined(MMI_GPRS_SUPPORT) || defined(WIFI_SUPPORT)
        MMIHTTP_Net_Close();
#endif
        
        cur_filename = MMIZDT_HTTP_GetCurFileFullname();
        cur_file_len = MMIZDT_HTTP_GetCurFileLen();
        if(cur_filename != PNULL)
        {
            uint32 need_len = cur_file_len;
            BOOLEAN is_full = FALSE;
            BOOLEAN is_ok = s_ctreghttphttp.flags;
            
            if(need_len == 0)
            {
                need_len = g_cur_http_content_len;
            }
            
            if(g_cur_http_rcv_count != g_cur_http_file_save_len)
            {
                ZDTHTTP_LOG("[HTTP]:Http_HandleCloseCnf FileLen Err %d,%d",g_cur_http_rcv_count,g_cur_http_file_save_len);
                is_ok = FALSE;
                ZDT_HTTP_File_Remove(cur_filename);
            }
            else
            {
                if(cur_file_len > 0)
                {
                    if(g_cur_http_rcv_count < cur_file_len)
                    {
                        ZDTHTTP_LOG("[HTTP]:Http_HandleCloseCnf RCVLen Err %d,%d",cur_file_len,g_cur_http_rcv_count);
                        is_ok = FALSE;
                        ZDT_HTTP_File_Remove(cur_filename);
                    }
                }
                else
                {
                    if(g_cur_http_content_len != 0 &&  g_cur_http_rcv_count < g_cur_http_content_len)
                    {
                        ZDTHTTP_LOG("[HTTP]:Http_HandleCloseCnf ContentLen Err %d,%d",g_cur_http_content_len,g_cur_http_rcv_count);
                        is_ok = FALSE;
                        ZDT_HTTP_File_Remove(cur_filename);
                    }
                }
            }
            is_full = Http_File_Rcv_Full(MMIZDT_HTTP_GetCurFileStatus(),need_len);
            if(is_full)
            {
                is_ok = FALSE;
                err_id = HTTP_ERROR_FILE_NO_SPACE;
                ZDTHTTP_LOG("[HTTP]:Http_HandleCloseCnf FULL Err %d,%d,%d",cur_file_len,g_cur_http_rcv_count,g_cur_http_file_save_len);
            }
            HTTP_Net_Send_CallBack(is_ok,err_id);
        }
        else
        {
            HTTP_Net_Send_CallBack(s_ctreghttphttp.flags,err_id);
        }

        
        ZDTHTTP_LOG("[HTTP]:HandleCTRegHttpCloseCnf send again=== ");
   }
    return result;
}
    
    
    

MMI_RESULT_E Http_HandleHttpMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
    //当result为true时，后续注册的app不需要再响应http的消息
    MMI_RESULT_E result = MMI_RESULT_FALSE;
    void    *original_param = (void*)((uint32)param);
    CTREGHTTP_HTTP_CONTEXT *http_context_ptr = &s_ctreghttp_http_context;
    HTTP_SSL_CERT_UNTRUST_IND_SIG_T* cert_ptr = PNULL;
    uint8 * cur_filename = PNULL;
	ZDTHTTP_LOG("[HTTP]:Http_HandleHttpMsg == msg_id=0x%x ",msg_id);	
	if(msg_id == HTTP_SIG_INIT_CNF)
	{
	      HTTP_INIT_CNF_SIG_T * cnf = (HTTP_INIT_CNF_SIG_T*)original_param;
	      if(cnf->module_id != MMI_MODULE_ZDTHTTP || ZDT_HTTP_APPFLAG != cnf->app_param)
            {
                return MMI_RESULT_FALSE;
            }
            g_cur_http_content_len = 0;
            g_cur_http_rcv_count = 0;
            g_cur_http_file_save_len = 0;
            cur_filename = MMIZDT_HTTP_GetCurFileFullname();
            ZDT_HTTP_File_Close(g_zdt_http_rcf_file_handle);
            g_zdt_http_rcf_file_handle = SFS_INVALID_HANDLE;
            
            if(cur_filename != PNULL)
            {
                ZDTHTTP_LOG("[HTTP]: HTTP_SIG_INIT_CNF cur_filename=%s",cur_filename);	
                ZDT_HTTP_File_Remove(cur_filename);
                g_zdt_http_rcf_file_handle = ZDT_HTTP_File_Open(cur_filename, FALSE);
            }

            if(s_p_refresh_url != PNULL)
            {
                SCI_FREE(s_p_refresh_url);
                s_p_refresh_url = PNULL;
            }
            s_ctreghttphttp.need_refresh = FALSE;
            s_ctreghttphttp.flags = FALSE;
            if(MMIZDT_HTTP_IsGetType())
            {
        	      result = Http_HandleGetRequest(http_context_ptr, param);
            }
            else
            {
        	      result = Http_HandlePostRequest(http_context_ptr, param);
            }
            if(result == MMI_RESULT_FALSE)
            {
                HTTP_Net_Send_CallBack(FALSE,HTTP_ERROR_HANDSHAKE);
            }
	      return result;//后续app不用再响应
	 }
	 //第一次请求后，http分配给app的id
	 if(msg_id == HTTP_SIG_REQUEST_ID_IND)
	 {
	      result = Http_HandleRequestIdInd(http_context_ptr, param);
	      return result;
	 }	 
	 switch (msg_id)
	 {
	 case HTTP_SIG_HEADER_IND:
	 	{
#ifdef HTTP_SUPPORT
	 		HTTP_HEADER_IND_SIG_T signal = {0};
	 		CTREGHTTP_HTTP_SIGNAL_ANALYTIC(param,signal,http_context_ptr,HTTP_HEADER_IND_SIG_T);
	 		ZDTHTTP_LOG("[HTTP]:receive HTTP_SIG_HEADER_IND");
	 		ZDTHTTP_LOG("[HTTP]:response_code:%d",signal.rsp_header_info.response_code);
	 		ZDTHTTP_LOG("[HTTP]:context_id:%ld,app_instance:%ld,request_id:%ld",
	 						http_context_ptr->context_id,http_context_ptr->app_instance,http_context_ptr->request_id);
                   if(signal.header_ptr != PNULL)
                   {
                        g_cur_http_content_len = signal.rsp_header_info.content_length;
                       ZDTHTTP_LOG("[HTTP]:content_length=%d",g_cur_http_content_len);
                       //ZDT_TraceCR(signal.header_ptr,strlen(signal.header_ptr));
                   }
	 		if(signal.rsp_header_info.response_code == 200)
	 		{	 			
	 			result=HTTP_HeaderResponse(http_context_ptr->context_id,http_context_ptr->app_instance,
	 								http_context_ptr->request_id,HTTP_DATA_TRANS_STYLE_BUFFER,PNULL,0);
                          
                          if(signal.rsp_header_info.refresh_url_ptr != NULL)
                          {
                                uint32 len = strlen(signal.rsp_header_info.refresh_url_ptr);
                                if(s_p_refresh_url != PNULL)
                                {
                                    SCI_FREE(s_p_refresh_url);
                                    s_p_refresh_url = PNULL;
                                }
                                s_p_refresh_url = SCI_ALLOC_APPZ(len+1);
                                if(s_p_refresh_url != PNULL)
                                {
                                    strcpy((char*)s_p_refresh_url,(const char*)(signal.rsp_header_info.refresh_url_ptr));
                                }
                                s_ctreghttphttp.need_refresh = TRUE;
                                ZDTHTTP_LOG("[HTTP]: refresh time=%d, url_ptr:%s",signal.rsp_header_info.refresh_time,signal.rsp_header_info.refresh_url_ptr);
                                
                          }
                          else
                          {
                                s_ctreghttphttp.flags = TRUE;
                          }
	 		}
	 		else
	 		{
#ifndef ZDT_PLAT_YX_SUPPORT_CY
                       HTTP_Net_Send_CallBack(FALSE,HTTP_ERROR_HANDSHAKE);
#endif
	 		    HTTP_CloseRequest(http_context_ptr->context_id);
	 		}
#endif
	 		ZDTHTTP_LOG("[HTTP]:result:%d",result);
	 		result = MMI_RESULT_TRUE;
	 	}
	 	break;
        
	 case HTTP_SIG_ERROR_IND:
	 	{
                    HTTP_ERROR_IND_SIG_T signal = {0};
                    CTREGHTTP_HTTP_SIGNAL_ANALYTIC(param,signal,http_context_ptr,HTTP_ERROR_IND_SIG_T);
                    ZDTHTTP_LOG("[HTTP]:receive HTTP_SIG_ERROR_IND err_id=%d",signal.result);
                    ZDT_HTTP_File_Close(g_zdt_http_rcf_file_handle);
                    g_zdt_http_rcf_file_handle = SFS_INVALID_HANDLE;
                    HTTP_CloseRequest(http_context_ptr->context_id);
	 	}
	 	break;	
	 case HTTP_SIG_DATA_IND:
	 	{
                    ZDTHTTP_LOG("[HTTP]:receive HTTP_SIG_DATA_IND");
	             result=Http_HandleDataInd(http_context_ptr,param);
	 	}
	 	break;
	 case HTTP_SIG_GET_CNF:
	 	result =Http_HandleGetCnf(http_context_ptr,param);
             
	 	break;
	 case HTTP_SIG_POST_CNF:
	 	result =Http_HandlePostCnf(http_context_ptr,param);
	 	break;
   	 case HTTP_SIG_CLOSE_CNF://HTTP_SIG_CLOSE_REQ消息的回复,告知app,http服务关闭
        	result = Http_HandleCloseCnf(http_context_ptr, param);	
        	break;
     default:
     	break;
	 }
	 return result;
}

BOOLEAN  Http_StartRequest()
{
    HTTP_INIT_PARAM_T init_param = {0};
    init_param.is_cookie_enable = FALSE;
    init_param.is_cache_enable = FALSE;
    init_param.net_id = s_http_net_id;
    ZDTHTTP_LOG("[HTTP] StartCTRegHttpRequest init_param_ptr->is_cookie_enable = FALSE; \n");
#ifdef HTTP_SUPPORT
    if (HTTP_SUCCESS == HTTP_InitRequest(MMI_MODULE_ZDTHTTP, ZDT_HTTP_APPFLAG, &init_param))
#endif
    {
        return TRUE;
    }
    return FALSE;
}

MMI_RESULT_E Http_StartRequestRefresh(CTREGHTTP_HTTP_CONTEXT * http_context_ptr, char * url)
{
      int base_len = 0;
	MMI_RESULT_E        result = MMI_RESULT_FALSE;
	HTTP_GET_PARAM_T* get_param_ptr= NULL;
	int len = 0;

        if(http_context_ptr == NULL || url == PNULL)
        {
            return MMI_RESULT_FALSE;
        }

	ZDTHTTP_LOG("[HTTP]:HandleCTRegHttpGetRefresh url =%s",url);
	get_param_ptr = (HTTP_GET_PARAM_T*)SCI_ALLOC_APPZ(sizeof(HTTP_GET_PARAM_T));

	if(get_param_ptr == NULL)
	{
		return MMI_RESULT_FALSE;
	}	
	memset(get_param_ptr,0,sizeof(HTTP_GET_PARAM_T));
	
	get_param_ptr->connection = HTTP_CONNECTION_KEEP_ALIVE;
	get_param_ptr->need_net_prog_ind = FALSE;
	len=strlen(CTREGHTTP_ACCEPT_STRING);
	get_param_ptr->accept.accept_ptr = SCI_ALLOC(len+1);
	memset(get_param_ptr->accept.accept_ptr,0,len+1);
	strcpy(get_param_ptr->accept.accept_ptr,CTREGHTTP_ACCEPT_STRING);
	len=strlen(CTREGHTTP_ACCEPT_LANGUAGE_STRING);
	get_param_ptr->accept_language.accept_language_ptr = SCI_ALLOC(len+1);
	memset(get_param_ptr->accept_language.accept_language_ptr,0,len+1);
	strcpy(get_param_ptr->accept_language.accept_language_ptr,CTREGHTTP_ACCEPT_LANGUAGE_STRING);	
	len=strlen(CTREGHTTP_USER_AGENT_STRING);
	get_param_ptr->user_agent.user_agent_ptr = SCI_ALLOC(len+1);
	memset(get_param_ptr->user_agent.user_agent_ptr,0,len+1);
	strcpy(get_param_ptr->user_agent.user_agent_ptr,CTREGHTTP_USER_AGENT_STRING);	

	//拼接URL
	len=strlen(url);
	get_param_ptr->uri.uri_ptr= SCI_ALLOC(len+1);
	memset(get_param_ptr->uri.uri_ptr,0,len+1);
	strcpy(get_param_ptr->uri.uri_ptr,url);
	ZDTHTTP_LOG("[HTTP]:HandleCTRegHttpGetRefresh uri.uri_ptr=%s,len=%d ",get_param_ptr->uri.uri_ptr,len);
    
      len = strlen(CTREGHTTP_ACCEPT_CHARSET_STRING);
      get_param_ptr->accept_charset.accept_charset_ptr = SCI_ALLOC(len + 1);
      memset(get_param_ptr->accept_charset.accept_charset_ptr,0,len + 1);
      strcpy(get_param_ptr->accept_charset.accept_charset_ptr,CTREGHTTP_ACCEPT_CHARSET_STRING);
      
#ifdef HTTP_SUPPORT
	if(!HTTP_GetRequest(http_context_ptr->context_id,get_param_ptr, http_context_ptr->app_instance))
	{
		result = MMI_RESULT_TRUE;
	}
#endif
    SCI_FREE(get_param_ptr->accept.accept_ptr);
    SCI_FREE(get_param_ptr->accept_charset.accept_charset_ptr);
    SCI_FREE(get_param_ptr->accept_language.accept_language_ptr);
    SCI_FREE(get_param_ptr->user_agent.user_agent_ptr);
    SCI_FREE(get_param_ptr->uri.uri_ptr);
    SCI_Free(get_param_ptr);
    return	result;	
}

BOOLEAN MMIZDT_HTTP_Handle_RefreshInd(DPARAM param)
{
    #if 1
        if(MMI_RESULT_FALSE == Http_StartRequestRefresh(&s_ctreghttp_http_context,s_p_refresh_url))
        {
            HTTP_CloseRequest(s_ctreghttp_http_context.context_id);
        }
    #else
    ZDTHTTP_REFRESH_DATA_T ** pp_getdata = (ZDTHTTP_REFRESH_DATA_T **)(param);
    ZDTHTTP_REFRESH_DATA_T * ind_ptr = *pp_getdata;
    uint32  url_len = 0;
    if(ind_ptr == PNULL)
    {
        ZDTHTTP_LOG("[HTTP] MMIZDTHTTP_HTTP_CTREG_HandleRefreshInd ERR NULL DATA");
        return FALSE;
    }
    if(ind_ptr->url_arr != PNULL)
    {
        url_len = SCI_STRLEN((char*)ind_ptr->url_arr);
    }
    
    if (0 == url_len)
    {
        ZDTHTTP_LOG("[HTTP]MMIZDTHTTP_HTTP_CTREG_HandleRefreshInd: url len == 0");
        SCI_FREE(ind_ptr);
        return FALSE;
    }

    if (ind_ptr->refresh_time > 0)
    {
        MMK_CreateTimerCallback(ind_ptr->refresh_time * 1000, (MMI_TIMER_FUNC)ZDT_HttpRefreshTimerCallback, (uint32)ind_ptr->url_arr, FALSE);
    }
    else
    {
        HandleCTRegHttpGetRefresh(&s_ctreghttp_http_context,ind_ptr->url_arr); 
        if(ind_ptr->url_arr != NULL)
        {
            SCI_FREE(ind_ptr->url_arr);
        }
    }        
    SCI_FREE(ind_ptr);
    #endif
    return TRUE;
}
#endif
/*****************************************************************************/
//  Discription: Get apn str 
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
/*****************************************************************************/
LOCAL char* HTTP_Net_GetApnStr(MN_DUAL_SYS_E dual_sys)
{
    MMICONNECTION_LINKSETTING_DETAIL_T* linksetting = PNULL;
    char*   apn_str = PNULL;
    uint8   index   = MMIAPIBROWSER_GetNetSettingIndex(dual_sys);

    linksetting = MMIAPICONNECTION_GetLinkSettingItemByIndex(dual_sys, index);

    if(PNULL != linksetting && 0 != linksetting->apn_len)
    {
        apn_str = (char*)linksetting->apn;
    }
    if(apn_str != PNULL)
    {
        HTTP_NET_TRACE("HTTP NET Get APN %s",apn_str);
    }
    else
    {
        HTTP_NET_TRACE("HTTP NET Get APN NULL");
    }
    return apn_str;
}

/*****************************************************************************/
//  Discription: Get apn str 
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
/*****************************************************************************/
LOCAL char* HTTP_Net_GetUserNameStr(MN_DUAL_SYS_E dual_sys)
{
    MMICONNECTION_LINKSETTING_DETAIL_T* linksetting = PNULL;
    char*   user_name_str = PNULL;
    uint8   index   = 0;

    linksetting = MMIAPICONNECTION_GetLinkSettingItemByIndex(dual_sys, index);

    if(PNULL != linksetting && 0 != linksetting->username_len)
    {
        user_name_str = (char*)linksetting->username;
    }
    if(user_name_str != PNULL)
    {
        HTTP_NET_TRACE("HTTP NET Get UserName %s",user_name_str);
    }
    else
    {
        HTTP_NET_TRACE("HTTP NET Get UserName NULL");
    }
    return user_name_str;
    
}

/*****************************************************************************/
//  Discription: Get apn str 
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
/*****************************************************************************/
LOCAL char* HTTP_Net_GetPasswordStr(MN_DUAL_SYS_E dual_sys)
{
    MMICONNECTION_LINKSETTING_DETAIL_T* linksetting = PNULL;
    char*   password_str = PNULL;
    uint8   index   = 0;

    linksetting = MMIAPICONNECTION_GetLinkSettingItemByIndex(dual_sys, index);

    if(PNULL != linksetting && 0 != linksetting->password_len)
    {
        password_str = (char*)linksetting->password;
    }
    if(password_str != PNULL)
    {
        HTTP_NET_TRACE("HTTP NET Get Password %s",password_str);
    }
    else
    {
        HTTP_NET_TRACE("HTTP NET Get Password NULL");
    }
    return password_str;
}

/*****************************************************************************/
//  Description : handle socket connect timeout
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL void HTTP_Net_HandlePdpActiveTimer(
                                            uint8 timer_id,
                                            uint32 param
                                            )
{
    HTTP_NET_TRACE("HTTP NET PdpActive ERR TimeOut");
    HTTP_Net_PdpDeactive();
    MMIZDTHTTP_SendSigTo_APP(ZDTHTTP_APP_SIGNAL_PDP_ACTIVE_FAIL,PNULL,0);
}

/*****************************************************************************/
//  Discription: Start socket connect timer
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
/*****************************************************************************/
LOCAL void HTTP_Net_StartPdpActiveTimer(void)
{
    if(0 != s_http_net_pdp_timer_id)
    {
        MMK_StopTimer(s_http_net_pdp_timer_id);
        s_http_net_pdp_timer_id = 0;
    }
    
    s_http_net_pdp_timer_id = MMK_CreateTimerCallback(HTTP_PDP_ACTIVE_TIMEOUT, 
                                                                        HTTP_Net_HandlePdpActiveTimer, 
                                                                        PNULL, 
                                                                        FALSE);
}
/*****************************************************************************/
//  Discription: Start socket connect timer
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
/*****************************************************************************/
LOCAL void HTTP_Net_StopPdpActiveTimer(void)
{
    if(0 != s_http_net_pdp_timer_id)
    {
        MMK_StopTimer(s_http_net_pdp_timer_id);
        s_http_net_pdp_timer_id = 0;
    }
}

/*****************************************************************************
//  Description : handle pdp msg 
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
*****************************************************************************/
LOCAL void HTTP_Net_HandlePdpMsg(MMIPDP_CNF_INFO_T *msg_ptr)
{
    if(PNULL == msg_ptr)
    {
        return;
    }
    
    HTTP_NET_TRACE("HTTP NET HandlePdpMsg msg_id=%d,result=%d",msg_ptr->msg_id,msg_ptr->result);
    
    switch(msg_ptr->msg_id) 
    {
    case MMIPDP_ACTIVE_CNF:     //PDP激活成功
        HTTP_Net_StopPdpActiveTimer();
        
        if(MMIPDP_RESULT_SUCC == msg_ptr->result)
        {
            s_http_net_id = msg_ptr->nsapi;
            HTTP_NET_TRACE("HTTP NET MMIPDP_ACTIVE_CNF OK net_id=0x%x",s_http_net_id);
            MMIZDTHTTP_SendSigTo_APP(ZDTHTTP_APP_SIGNAL_PDP_ACTIVE_SUCCESS,PNULL,0);
        }
        else if (MMIPDP_RESULT_FAIL == msg_ptr->result)
        {
            HTTP_NET_TRACE("HTTP NET MMIPDP_ACTIVE_CNF FAIL");
            HTTP_Net_PdpDeactive();
            MMIZDTHTTP_SendSigTo_APP(ZDTHTTP_APP_SIGNAL_PDP_ACTIVE_FAIL,PNULL,0);
        }
        else 
        {
            HTTP_NET_TRACE("HTTP NET MMIPDP_ACTIVE_CNF FAIL Other");
            HTTP_Net_PdpDeactive();
            MMIZDTHTTP_SendSigTo_APP(ZDTHTTP_APP_SIGNAL_PDP_ACTIVE_FAIL,PNULL,0);
        }        
        break;
        
    case MMIPDP_DEACTIVE_CNF:   //PDP去激活成功。
            HTTP_NET_TRACE("HTTP NET MMIPDP_DEACTIVE_CNF");
            s_http_net_id = 0;
            MMIZDTHTTP_SendSigTo_APP(ZDTHTTP_APP_SIGNAL_PDP_CONNECT_FAIL,PNULL,0);
        break;
        
    case MMIPDP_DEACTIVE_IND:   //PDP被网络端去激活。
            HTTP_NET_TRACE("HTTP NET MMIPDP_DEACTIVE_IND");
            s_http_net_id = 0;
            HTTP_Net_PdpDeactive();
            MMIZDTHTTP_SendSigTo_APP(ZDTHTTP_APP_SIGNAL_PDP_CONNECT_FAIL,PNULL,0);
        break;
        
    default:
        break;
    }
    
}
/*****************************************************************************/
//  Description : Pdp Active, 
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
#if 1
LOCAL BOOLEAN  HTTP_Net_PDPTry(MN_DUAL_SYS_E *  p_cur_sys)
{
#if defined(MMI_GPRS_SUPPORT)
    BOOLEAN                 return_val      = FALSE;
    MMIPDP_ACTIVE_INFO_T    active_info     = {0};
    MMICONNECTION_APNTYPEINDEX_T *apn_idx_arr;
    MMICONNECTION_LINKSETTING_DETAIL_T* linksetting = PNULL;
    MN_DUAL_SYS_E sim_sys;
        
    HTTP_NET_TRACE("HTTP NET ConnectPDP");

    if (FALSE == MMIAPIPHONE_GetDataServiceSIM(&sim_sys))
    {
        SCI_TraceLow("HTTP NET ConnectPDP [SIM ERROR]");
        return FALSE;
    }
    *p_cur_sys = sim_sys;
    active_info.app_handler         = MMI_MODULE_ZDTHTTP;
    active_info.dual_sys            = sim_sys;
    active_info.apn_ptr             = NULL;
    active_info.user_name_ptr       = NULL;
    active_info.psw_ptr             = NULL;
    active_info.priority            = 3;
    active_info.ps_service_rat      = MN_TD_PREFER;
    active_info.ps_interface        = MMIPDP_INTERFACE_GPRS;
    active_info.handle_msg_callback = HTTP_Net_HandlePdpMsg;
    active_info.ps_service_type     = BROWSER_E;
    active_info.storage             = MN_GPRS_STORAGE_ALL;

    apn_idx_arr = MMIAPICONNECTION_GetApnTypeIndex(sim_sys);
    if (apn_idx_arr == NULL)
    {
        HTTP_NET_TRACE("HTTP NET ConnectPDP [APN IDX ERROR]");
        return FALSE;
    }
    HTTP_NET_TRACE("HTTP NET  ConnectPDP [sim = %d, Net Setting = %d]", sim_sys, apn_idx_arr->internet_index[sim_sys].index);
    linksetting = MMIAPICONNECTION_GetLinkSettingItemByIndex(sim_sys, apn_idx_arr->internet_index[sim_sys].index);

    if(PNULL != linksetting && 0 != linksetting->apn_len)
    {
        active_info.apn_ptr = (char*)linksetting->apn;
        active_info.user_name_ptr = (char*)linksetting->username;
        active_info.psw_ptr = (char*)linksetting->password;
#ifdef IPVERSION_SUPPORT_V4_V6
        active_info.ip_type   = linksetting->ip_type;
#endif
        if(MMIAPIPDP_Active(&active_info))
        {
            HTTP_NET_TRACE("HTTP NET PdpActive GPRS sim_sys=%d",sim_sys);
            HTTP_Net_StartPdpActiveTimer();
            return_val = TRUE;
        }
    }
    return return_val;
#endif
    return FALSE;
}
#else
LOCAL BOOLEAN  HTTP_Net_PDPTry(MN_DUAL_SYS_E sim_sys)
{
    MMIPDP_ACTIVE_INFO_T    active_info     = {0};
    BOOLEAN                 return_val      = FALSE;
    
    active_info.app_handler         = MMI_MODULE_ZDTHTTP;
    active_info.dual_sys            = sim_sys;
    active_info.apn_ptr             = HTTP_Net_GetApnStr(sim_sys);
    active_info.user_name_ptr       = HTTP_Net_GetUserNameStr(sim_sys);
    active_info.psw_ptr             = HTTP_Net_GetPasswordStr(sim_sys);
    active_info.priority            = 3;
    active_info.ps_service_rat      = MN_UNSPECIFIED;
    active_info.ps_interface        = MMIPDP_INTERFACE_GPRS;
    active_info.handle_msg_callback = HTTP_Net_HandlePdpMsg;
    active_info.ps_service_type = BROWSER_E;
    active_info.storage = MN_GPRS_STORAGE_ALL;

    if(MMIAPIPDP_Active(&active_info))
    {
        HTTP_NET_TRACE("HTTP NET PdpActive GPRS sim_sys=%d",sim_sys);
        HTTP_Net_StartPdpActiveTimer();
        return_val = TRUE;
    }
    return return_val;
}
#endif

LOCAL BOOLEAN HTTP_Net_PdpActive(void)
{
    BOOLEAN                 return_val      = FALSE;
    MN_DUAL_SYS_E           dual_sys        = MN_DUAL_SYS_MAX;
    MMIPDP_ACTIVE_INFO_T    active_info     = {0};
    
    HTTP_NET_TRACE("HTTP NET PdpActive Start is_activing=%d,is_ok=%d",s_http_net_is_activing,s_http_net_pdp_state_is_ok);
    if(s_http_net_is_activing)
    {
        s_http_net_need_active_close = FALSE;
        return TRUE;
    }

    s_http_net_is_activing = TRUE;
    
    if(s_http_net_pdp_state_is_ok)
    {
        MMIZDTHTTP_SendSigTo_APP(ZDTHTTP_APP_SIGNAL_PDP_ACTIVE_SUCCESS,PNULL,0);
        return TRUE;
    }

    if(MMIAPISET_GetFlyMode() 
      || (MMIAPIPHONE_GetSimOkNum(NULL,0)==0)
      /*||(MMICC_GetCallNum()>0)*/
    )
    {
        HTTP_NET_TRACE("HTTP NET PdpActive ERR FLY");
        MMIZDTHTTP_SendSigTo_APP(ZDTHTTP_APP_SIGNAL_PDP_ACTIVE_FAIL,PNULL,0);
        return FALSE;
    }

#if 0 //def WIFI_SUPPORT    
    //优先wifi
    if(s_http_net_set_sys == MN_DUAL_SYS_MAX && MMIWIFI_STATUS_CONNECTED == MMIAPIWIFI_GetStatus())
    {
        active_info.app_handler         = MMI_MODULE_ZDTHTTP;
        active_info.handle_msg_callback = HTTP_Net_HandlePdpMsg;
        active_info.ps_interface        = MMIPDP_INTERFACE_WIFI;
        if(MMIAPIPDP_Active(&active_info))
        {
            HTTP_NET_TRACE("HTTP NET PdpActive OK WIFI");
            HTTP_Net_StartPdpActiveTimer();
            return_val = TRUE;
        }
        else
        {
            HTTP_NET_TRACE("HTTP NET PdpActive ERR WIFI");
            MMIZDTHTTP_SendSigTo_APP(ZDTHTTP_APP_SIGNAL_PDP_ACTIVE_FAIL,PNULL,0);
            return FALSE;
        }
    }
    else
#endif
    {
        uint32 sim_ok_num = 0;
        uint16 sim_ok = 0;
        
        sim_ok_num = MMIAPIPHONE_GetSimAvailableNum(&sim_ok,1);/*lint !e64*/
        if(0 == sim_ok_num)
        {
            HTTP_NET_TRACE("HTTP NET PdpActive ERR NO SIM");
            MMIZDTHTTP_SendSigTo_APP(ZDTHTTP_APP_SIGNAL_PDP_ACTIVE_FAIL,PNULL,0);
            return FALSE;
        }
        #if 0
        if(s_http_net_is_init == FALSE)
        {
            HTTP_NET_TRACE("HTTP NET PdpActive ERR NO INIT");
            MMIZDTHTTP_SendSigTo_APP(ZDTHTTP_APP_SIGNAL_PDP_ACTIVE_FAIL,PNULL,0);
            return FALSE;
        }
        #endif

        return_val = HTTP_Net_PDPTry(&s_http_net_cur_sys);//第一张有效卡
        if(FALSE == return_val)
        {
            HTTP_NET_TRACE("HTTP NET PdpActive ERR cur_sys(%d/%d),",s_http_net_cur_sys,sim_ok_num);
            MMIZDTHTTP_SendSigTo_APP(ZDTHTTP_APP_SIGNAL_PDP_ACTIVE_FAIL,PNULL,0);
        }
        else
        {
            MMIHTTP_Net_NV_SetActiveSys(s_http_net_cur_sys);
        }
    }

    return return_val;
}

/*****************************************************************************/
//  Description : Deactive Pdp
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HTTP_Net_PdpDeactive(void)
{
    BOOLEAN return_val = FALSE;
    HTTP_NET_TRACE("HTTP_Net_PdpDeactive");
    return_val = MMIAPIPDP_Deactive(MMI_MODULE_ZDTHTTP);

    return return_val;
}

LOCAL BOOLEAN HTTP_Net_Init(void)
{
    MN_DUAL_SYS_E dual_sys = MMIHTTP_Net_GetActiveSys();
    HTTP_NET_TRACE("HTTP_Net_Init is_init=%d, dual_sys=%d",s_http_net_is_init,dual_sys);
    if(s_http_net_is_init == FALSE)
    {
        MMIHTTP_Get_SIM_IMSI(dual_sys,g_http_sim_imsi);
        MMIHTTP_Get_SIM_ICCID(dual_sys,g_http_sim_iccid);
        s_http_net_is_init = TRUE;
    }
    return TRUE;
}

LOCAL BOOLEAN HTTP_Net_Open(void)
{
    HTTP_NET_TRACE("HTTP_Net_Open state=%d,is_activing=%d,is_closing=%d",s_http_net_pdp_state_is_ok,s_http_net_is_activing,s_http_net_is_closing);
    if(s_http_net_is_closing)
    {
        s_http_net_need_close_active = TRUE;
    }
    else
    {
        HTTP_Net_PdpActive();
    }
    return TRUE;
}

LOCAL BOOLEAN HTTP_Net_CloseEx(void)
{
    if(s_http_net_is_closing)
    {
        s_http_net_need_close_active = FALSE;
        return TRUE;
    }
    s_http_net_is_closing = TRUE;
    if(s_http_net_pdp_state_is_ok)
    {
        if(FALSE == HTTP_Net_PdpDeactive())
        {
            MMIZDTHTTP_SendSigTo_APP(ZDTHTTP_APP_SIGNAL_PDP_CONNECT_FAIL,PNULL,0);
        }
    }
    else
    {
        HTTP_Net_PdpDeactive();
        MMIZDTHTTP_SendSigTo_APP(ZDTHTTP_APP_SIGNAL_PDP_CONNECT_FAIL,PNULL,0);
    }
}

LOCAL BOOLEAN HTTP_Net_Close(void)
{
    HTTP_NET_TRACE("HTTP_Net_Close state=%d,is_activing=%d,is_closing=%d",s_http_net_pdp_state_is_ok,s_http_net_is_activing,s_http_net_is_closing);
    if(s_http_net_is_activing)
    {
        s_http_net_need_active_close = TRUE;
    }
    else
    {
        HTTP_Net_CloseEx();
    }
    return TRUE;
}

LOCAL BOOLEAN HTTP_Net_Reset(void)
{
    HTTP_NET_TRACE("HTTP_Net_Reset state=%d",s_http_net_pdp_state_is_ok);
    MMIHTTP_Net_Close();
    MMIHTTP_Net_Open();
    return TRUE;
}

LOCAL BOOLEAN HTTP_Net_CallBack(BOOLEAN is_ok)
{
    if(MMIZDT_HTTP_IsSending())
    {
        if(is_ok)
        {
            if(FALSE == Http_StartRequest())
            {
                MMIZDT_HTTP_PopSend_Result(FALSE,HTTP_ERROR_CONNECT);
            }
        }
        else
        {
            MMIZDT_HTTP_PopSend_Result(FALSE,HTTP_ERROR_CONNECT);
        }
    }
}


LOCAL BOOLEAN HTTP_Net_PdpStateChange(BOOLEAN is_ok)
{
    HTTP_NET_TRACE("HTTP NET PDPStateChange %d",is_ok);
    return TRUE;
}

LOCAL void HTTP_Net_PdpStateUpdate(BOOLEAN is_ok,BOOLEAN is_active)
{
    HTTP_NET_TRACE("HTTP_Net_PdpStateUpdate is_ok=%d,is_active=%d,activing=%d,closing=%d",is_ok,is_active,s_http_net_is_activing,s_http_net_is_closing);
    if(s_http_net_pdp_state_is_ok != is_ok)
    {
        s_http_net_pdp_state_is_ok = is_ok;
        HTTP_Net_PdpStateChange(is_ok);
    }
    
    if(is_active)
    {
        if(s_http_net_is_activing)
        {
            s_http_net_is_activing = FALSE;
            
            if(s_http_net_need_active_close)
            {
                HTTP_NET_TRACE("HTTP_Net_PdpStateUpdate Active Close");
                s_http_net_need_active_close = FALSE;
                MMIHTTP_Net_Close();
                HTTP_Net_CallBack(FALSE);
            }
            else
            {
                if(is_ok)
                {
                    HTTP_Net_CallBack(TRUE);
                }
            }
        }
    }
    else
    {   
        s_http_net_is_activing = FALSE;
        if(s_http_net_is_closing)
        {
            s_http_net_is_closing = FALSE;
            if(s_http_net_need_close_active)
            {
                HTTP_NET_TRACE("HTTP_Net_PdpStateUpdate Close Active");
                s_http_net_need_close_active = FALSE;
                MMIHTTP_Net_Open();
            }
        }
    }
    return;
}

PUBLIC uint32 MMIHTTP_Net_GetNetID(void)
{
    return s_http_net_id;
}

PUBLIC BOOLEAN MMIHTTP_Net_PdpStateGet(void)
{
    return s_http_net_pdp_state_is_ok;
}

PUBLIC BOOLEAN MMIHTTP_Net_IsActiving(void)
{
    return s_http_net_is_activing;
}

PUBLIC BOOLEAN MMIHTTP_Net_IsClosing(void)
{
    return s_http_net_is_closing;
}

PUBLIC BOOLEAN MMIHTTP_Net_IsInit(void)
{
    return s_http_net_is_init;
}

PUBLIC BOOLEAN MMIHTTP_Net_Init(void)
{
    MMIZDTHTTP_SendSigTo_APP(ZDTHTTP_APP_SIGNAL_NET_INIT,PNULL,0);
    return TRUE;
}

PUBLIC BOOLEAN MMIHTTP_Net_Reset(void)
{
    MMIZDTHTTP_SendSigTo_APP(ZDTHTTP_APP_SIGNAL_NET_RESET,PNULL,0);
    return TRUE;
}

PUBLIC BOOLEAN MMIHTTP_Net_Open(void)
{
    MMIZDTHTTP_SendSigTo_APP(ZDTHTTP_APP_SIGNAL_NET_OPEN,PNULL,0);
    return TRUE;
}

PUBLIC BOOLEAN MMIHTTP_Net_Close(void)
{
    MMIZDTHTTP_SendSigTo_APP(ZDTHTTP_APP_SIGNAL_NET_CLOSE,PNULL,0);
    return TRUE;
}

LOCAL MN_DUAL_SYS_E MMIHTTP_Net_NV_GetActiveSys(void)
{
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    uint8 net_sys = HTTP_PDP_DEFAULT_SIM_SYS;
    //get if is flip answer
    MMINV_READ(MMI_ZDTHTTP_NV_NET_SYS,&net_sys,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        //自动识别
        net_sys = HTTP_PDP_DEFAULT_SIM_SYS;
        MMINV_WRITE(MMI_ZDTHTTP_NV_NET_SYS,&net_sys);
    }
    s_http_net_set_sys = net_sys;
    return net_sys;
}

PUBLIC BOOLEAN  MMIHTTP_Net_NV_SetActiveSys(MN_DUAL_SYS_E set_sys) 
{
    uint8 net_sys = set_sys;
    if(s_http_net_set_sys != set_sys)
    {
        MMINV_WRITE(MMI_ZDTHTTP_NV_NET_SYS,&net_sys);
        s_http_net_set_sys = set_sys;
        return TRUE;
    }
    return FALSE;
}

PUBLIC MN_DUAL_SYS_E MMIHTTP_Net_GetActiveSys(void)
{
    return s_http_net_set_sys;
}

LOCAL BOOLEAN  MMIHTTP_Net_Handle_Init(DPARAM param)
{
    HTTP_NET_TRACE("MMIHTTP_Net_Handle_Init");
    HTTP_Net_Init();
    return TRUE;
}

LOCAL BOOLEAN  MMIHTTP_Net_Handle_Open(DPARAM param)
{
    HTTP_NET_TRACE("MMIHTTP_Net_Handle_Open");
    HTTP_Net_Open();
    return TRUE;
}

LOCAL BOOLEAN  MMIHTTP_Net_Handle_Close(DPARAM param)
{
    HTTP_NET_TRACE("MMIHTTP_Net_Handle_Close");
    HTTP_Net_Close();
    return TRUE;
}

LOCAL BOOLEAN  MMIHTTP_Net_Handle_Reset(DPARAM param)
{
    HTTP_NET_TRACE("MMIHTTP_Net_Handle_Reset");
    HTTP_Net_Reset();
    return TRUE;
}

LOCAL BOOLEAN  MMIHTTP_Net_Handle_PDPActiveOK(DPARAM param)
{
    HTTP_NET_TRACE("MMIHTTP_Net_Handle_PDPActiveOK net_is_activing=%d",s_http_net_is_activing);
    HTTP_Net_PdpStateUpdate(TRUE,TRUE);
    return TRUE;
}

LOCAL BOOLEAN  MMIHTTP_Net_Handle_PDPActiveFail(DPARAM param)
{
    HTTP_NET_TRACE("MMIHTTP_Net_Handle_PDPActiveFail net_is_activing=%d",s_http_net_is_activing);
    HTTP_Net_PdpStateUpdate(FALSE,TRUE);
    return TRUE;
}

LOCAL BOOLEAN  MMIHTTP_Net_Handle_PDPRespond(DPARAM param)
{
    HTTP_NET_TRACE("HTTP NET Handle_PDP_RSP net_is_activing=%d",s_http_net_is_activing);
    HTTP_Net_PdpStateUpdate(FALSE,FALSE);
    return TRUE;
}

PUBLIC MMI_RESULT_E MMIHTTP_Net_Handle_AppMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
    MMI_RESULT_E res = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case ZDTHTTP_APP_SIGNAL_NET_INIT:
            MMIHTTP_Net_Handle_Init(param);
        break;
        
    case ZDTHTTP_APP_SIGNAL_NET_OPEN:
            MMIHTTP_Net_Handle_Open(param);
        break;
    case ZDTHTTP_APP_SIGNAL_NET_CLOSE:
            MMIHTTP_Net_Handle_Close(param);
        break;
        
    case ZDTHTTP_APP_SIGNAL_NET_RESET:
            MMIHTTP_Net_Handle_Reset(param);
        break;
        
    case ZDTHTTP_APP_SIGNAL_PDP_ACTIVE_SUCCESS:
            MMIHTTP_Net_Handle_PDPActiveOK(param);
        break;
        
    case ZDTHTTP_APP_SIGNAL_PDP_ACTIVE_FAIL:
            MMIHTTP_Net_Handle_PDPActiveFail(param);
        break;
        
    case ZDTHTTP_APP_SIGNAL_PDP_CONNECT_FAIL:
            MMIHTTP_Net_Handle_PDPRespond(param);
        break;
    
    default:
            res = MMI_RESULT_FALSE;
        break;
    }

    if(res == MMI_RESULT_FALSE)
    {
    }
    return res;
}

LOCAL  BOOLEAN MMIHTTP_Net_StatusChange(MN_DUAL_SYS_E dual_sys,BOOLEAN is_ok)
{
    HTTP_NET_TRACE("HTTP NET StatusChange set_sys=%d,dual_sys=%d,is_ok=%d,is_init=%d",s_http_net_set_sys,dual_sys,is_ok,s_http_net_need_init);
    if(is_ok)
    {
        if(s_http_net_need_init)
        {
            MMIHTTP_Net_NV_SetActiveSys(dual_sys);
        }
        if(s_http_net_set_sys < MN_DUAL_SYS_MAX)
        {
            if(dual_sys == s_http_net_set_sys)
            {
                if(s_http_net_need_init )
                {
                    MMIHTTP_Net_Init();
                    s_http_net_need_init = FALSE;
                }
            }
        }
        else
        {
            if(s_http_net_need_init)
            {
                MMIHTTP_Net_Init();
                s_http_net_need_init = FALSE;
            }
            //MMIHTTP_Net_Open();
        }
    }
    else
    {
        if(s_http_net_set_sys < MN_DUAL_SYS_MAX)
        {
            if(s_http_net_need_init == FALSE)
            {
                MMIHTTP_Net_Close();
            }
        }
        else
        {
            if(s_http_net_need_init == FALSE)
            {
                MMIHTTP_Net_Close();
            }
        }
    }
    return TRUE;
}

PUBLIC BOOLEAN MMIHTTP_Net_StatusInd(APP_MN_NETWORK_STATUS_IND_T *status_ptr)
{
    if(status_ptr->dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return FALSE;
    }
    #ifdef WIN32
    if(status_ptr->dual_sys == MN_DUAL_SYS_1)
    {
        status_ptr->plmn_is_selecting = FALSE;
        status_ptr->gprs_support = TRUE;
    }
    #endif
    //ZDTHTTP_LOG("MMIHTTP_Net_StatusInd dual_sys=%d(%d),selecting=%d,plmn=%d,gprs_support=%d",status_ptr->dual_sys,s_http_net_gprs_status[status_ptr->dual_sys],status_ptr->plmn_is_selecting,status_ptr->plmn_status,status_ptr->gprs_support);
    
    if (status_ptr->plmn_is_selecting)
    {
        if(s_http_net_gprs_status[status_ptr->dual_sys])
        {
            s_http_net_gprs_status[status_ptr->dual_sys] = FALSE;
            MMIHTTP_Net_StatusChange(status_ptr->dual_sys,FALSE);
        }
    }
    else 
    {
        if(s_http_net_gprs_status[status_ptr->dual_sys] == FALSE)
        {
            if(
#ifndef WIN32
              	status_ptr->gprs_support && 
#endif            
                    MMIAPIPHONE_IsSimAvailable(status_ptr->dual_sys)
                )
            {
                ZDTHTTP_LOG("MMIHTTP_Net_StatusInd OK dual_sys=%d,plmn=%d,gprs_support=%d",status_ptr->dual_sys,status_ptr->plmn_status,status_ptr->gprs_support);
                s_http_net_gprs_status[status_ptr->dual_sys] = TRUE;
                MMIHTTP_Net_StatusChange(status_ptr->dual_sys,TRUE);
            }
        }
        else
        {
            if(status_ptr->gprs_support == 0 || (status_ptr->plmn_status != PLMN_NORMAL_GSM_ONLY && status_ptr->plmn_status != PLMN_NORMAL_GPRS_ONLY && status_ptr->plmn_status != PLMN_NORMAL_GSM_GPRS_BOTH))
            {
                ZDTHTTP_LOG("MMIHTTP_Net_StatusInd FAIL dual_sys=%d, plmn=%d,gprs_support=%d",status_ptr->dual_sys,status_ptr->plmn_status,status_ptr->gprs_support);
                s_http_net_gprs_status[status_ptr->dual_sys] = FALSE;
                MMIHTTP_Net_StatusChange(status_ptr->dual_sys,FALSE);
            }
        }
    }
    return TRUE;
}

BOOLEAN MMIHTTP_Net_GPRS_IsOK(MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN res = FALSE;
    if(dual_sys < MN_DUAL_SYS_MAX)
    {
        res = s_http_net_gprs_status[dual_sys];
    }
    return res;
}

PUBLIC BOOLEAN MMIHTTP_Net_APP_Init(void)
{
    MMIHTTP_Net_NV_GetActiveSys();
    return TRUE;
}

PUBLIC BOOLEAN MMIHTTP_File_Close(void)
{
    ZDT_HTTP_File_Close(g_zdt_http_rcf_file_handle);
    g_zdt_http_rcf_file_handle = SFS_INVALID_HANDLE;
    return TRUE;
}
#endif
