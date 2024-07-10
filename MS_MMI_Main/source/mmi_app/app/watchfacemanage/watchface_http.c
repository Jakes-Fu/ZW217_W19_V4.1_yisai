/****************************************************************************
** File Name:      watchface_http.c                                         *
** Author:                                                                  *
** Date:           2021.08.25                                               *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to do provide network access api       *
*****************************************************************************
**                         Important Edit History                           *
** -------------------------------------------------------------------------*
** DATE           NAME                         DESCRIPTION                  *
** 2021.08.25      xiaoju.cheng                 Create
** 
******************************************************************************/
#ifdef DOWNLOAD_WATCHFACE_SUPPORT  //for 217 ram cutdown: unsupport ble,so move download watchface from mobile phone
/*---------------------------------------------------------------------------*/
/*                         Include Files                                     */
/*---------------------------------------------------------------------------*/
#include "std_header.h"
#include "os_api.h"
#include "sci_types.h"
#include "mmk_app.h"
#include "http_api.h"
#include "mmiconnection_export.h"
#include "in_message.h"
#include "mmi_module.h"
#include "os_param.h"
#include "mmk_timer.h"
#include "mmicom_trace.h"
#include "watchface_config.h"
#include "mmidl_internal.h"
#include "mime_type.h"
#include "watchface_config_adapter.h"

#ifdef THEME_UNZIP_SUPPORT
#include "theme_gzip_export.h"
#endif
#ifdef UAL_NETWORKMGR_SUPPORT
#include "ual_networkmgr.h"
#endif
/*---------------------------------------------------------------------------*/
/*                        Macro Declaration                                  */
/*---------------------------------------------------------------------------*/
#define     MAX_URL_LEN          (256)
#define     MAX_SUPPORT_NUM      (20)
#define     MAX_DOWLOADING_NUM   (3)

typedef enum
{
    HTTP_DOWNLOAD_NONE,
    HTTP_DOWNLOAD_DOWNLOADING,
    HTTP_DOWNLOAD_DOWNLOADED,
    HTTP_DOWNLOAD_FAILED,
    HTTP_DOWNLOAD_WAITING,
    HTTP_DOWNLOAD_STATUS_MAX
}HTTP_DOWNLOAD_STATUS_E;

typedef struct
{
    HTTP_CONTEXT_ID_T       context_id;
    HTTP_REQUEST_ID_T       request_id;
    HTTP_APP_INSTANCE_T     app_instance; //用于区分不同的HTTP请求。同时只有一个HTTP请求时可以为任意值。
    SFS_HANDLE              recv_file_handle;
    char                    url[MAX_URL_LEN];
    wchar                   save_fullpath[MAX_URL_LEN];
    BOOLEAN                 is_used;
    char                    file_name[MAX_URL_LEN];
    HTTP_DOWNLOAD_STATUS_E  download_status;
    uint32                  mime_type;
}HTTP_DOWNLOAD_TASK_T;
//该结构体需要空间大，如果api内部使用，需要alloc空间。
typedef struct
{
    uint32                  download_task_num;
    uint32                  total_task_num;
    uint32                  net_id;
    BOOLEAN                 is_pdp_actived;
    HTTP_DOWNLOAD_TASK_T    download_task[MAX_SUPPORT_NUM];
}HTTP_DOWNLOAD_INFO_T;

#define WATCH_ACCEPT_STRING "text/vnd.wap.wml,application/vnd.wap.xhtml+xml,application/xhtml+xml, image/vnd.wap.wbmp, image/gif, image/jpg,image/jpeg, image/png, image/bmp, text/html,*/*;q=0.1"//"application/json"
#define WATCH_ACCEPT_LANGUAGE_STRING  "zh-cn, en" //表示接受中文英文
//#define WATCH_ACCEPT_LANGUAGE_STRING  "gzip"//"identity"////表示接受中文英文
#define WATCH_USER_AGENT_STRING       "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.0)"
#define WATCH_ACCEPT_CHARSET_STRING   "1.0"//"UTF-8,ISO-8859-1,US-ASCII,windows-1252"//"*utf-8*"
//#define WATCH_HTTP_URI_STRING   "https://ses-test.o2.co.uk:443/generic_devices"//"http://www.kuaidi100.com/query?type=shunfeng&postid=SF1123989188840"
//#define HTTP_WATCH_CONTENT_TYPE  "application/json"
#define WATCH_STRING_HTTP  "http://"
#define WATCH_STRING_ZIP   ".zip"

#define HTTP_TIMEOUT_180S 180000
#define HTTP_START_TIMEOUT_60S 60000
#define MAX_ENTITLEMENT_BUF 800

MMI_APPLICATION_T            g_watchhttp_app = {0};

LOCAL HTTP_DOWNLOAD_INFO_T   s_http_download_info = {0};

/*---------------------------------------------------------------------------*/
/*                        Function Declaration                               */
/*---------------------------------------------------------------------------*/

LOCAL void HttpDownload_GetStatus(int32 task_id,HTTP_DOWNLOAD_STATUS_E* p_download_status)
{
    if(PNULL != p_download_status)
    {
        *p_download_status = s_http_download_info.download_task[task_id].download_status;
        TRACE_WATCHFACE("[HTTP]:p_download_status = %d",*p_download_status);
    }
}

LOCAL void HttpDownload_SetStatus(int32 task_id,HTTP_DOWNLOAD_STATUS_E download_status)
{
    s_http_download_info.download_task[task_id].download_status = download_status;
    TRACE_WATCHFACE("[HTTP]:download_status = %d",download_status);
}

LOCAL void HttpInitRequest(int32 task_id)
{
    HTTP_INIT_PARAM_T* p_init_param = PNULL;

    p_init_param = SCI_ALLOC_APPZ(sizeof(HTTP_INIT_PARAM_T));
    if(PNULL == p_init_param)
    {
        TRACE_WATCHFACE("[HTTP] p_init_param is PNULL");
        return;
    }
    p_init_param->is_cookie_enable = FALSE;
    p_init_param->is_cache_enable = FALSE;
    p_init_param->net_id = s_http_download_info.net_id;
    TRACE_WATCHFACE("[HTTP] net_id = %d", p_init_param->net_id);

    HTTP_InitRequest(MMI_MODULE_WATCHFACE, task_id, p_init_param);

    SCI_FREE(p_init_param);
}

LOCAL void HttpCheckNextDownloadTask(void)
{
    uint32 i = 0;
    HTTP_DOWNLOAD_STATUS_E download_status = HTTP_DOWNLOAD_NONE;

    if (0 == s_http_download_info.total_task_num)
    {
        TRACE_WATCHFACE("[HTTP]:download complete");
        return;
    }

    for (i = 0; i < MAX_SUPPORT_NUM; i++)
    {
        HttpDownload_GetStatus(i,&download_status);

        if (HTTP_DOWNLOAD_WAITING == download_status)
        {
            HttpInitRequest(i);
            break;
        }
    }

}

LOCAL void HttpGetRequest(HTTP_DOWNLOAD_TASK_T* p_download_task )
{
    HTTP_GET_PARAM_T  get_param = {0};
    uint16 len = 0;

    if(PNULL == p_download_task)
    {
        TRACE_WATCHFACE("[HTTP]:p_download_task is PNULL");
        return;
    }
    get_param.connection = HTTP_CONNECTION_KEEP_ALIVE;
    get_param.need_net_prog_ind = FALSE;

    len=strlen(WATCH_ACCEPT_STRING);
    get_param.accept.accept_ptr = SCI_ALLOC_APPZ(len+1);
    memset(get_param.accept.accept_ptr,0,len+1);
    strcpy(get_param.accept.accept_ptr,WATCH_ACCEPT_STRING);

    len=strlen(WATCH_ACCEPT_LANGUAGE_STRING);
    get_param.accept_language.accept_language_ptr = SCI_ALLOC_APPZ(len+1);
    memset(get_param.accept_language.accept_language_ptr,0,len+1);
    strcpy(get_param.accept_language.accept_language_ptr,WATCH_ACCEPT_LANGUAGE_STRING);

    len=strlen(WATCH_USER_AGENT_STRING);
    get_param.user_agent.user_agent_ptr = SCI_ALLOC_APPZ(len+1);
    memset(get_param.user_agent.user_agent_ptr,0,len+1);
    strcpy(get_param.user_agent.user_agent_ptr,WATCH_USER_AGENT_STRING);

    len = strlen(WATCH_ACCEPT_CHARSET_STRING);
    get_param.accept_charset.accept_charset_ptr = SCI_ALLOC_APPZ(len + 1);
    memset(get_param.accept_charset.accept_charset_ptr,0,len + 1);
    strcpy(get_param.accept_charset.accept_charset_ptr,WATCH_ACCEPT_CHARSET_STRING);

    //拼接URL
    len=strlen(p_download_task->url);
    get_param.uri.uri_ptr= SCI_ALLOC_APPZ(len+1);
    memset(get_param.uri.uri_ptr,0,len+1);
    SCI_MEMCPY(get_param.uri.uri_ptr,p_download_task->url,len);
    TRACE_WATCHFACE("[HTTP]:uri = %s",get_param.uri.uri_ptr);

    //使用buffer的方式保存回应数据，不需要设置文件名称
    get_param.get_data.is_header_use_file = FALSE;
    get_param.get_data.style = HTTP_DATA_TRANS_STYLE_BUFFER;

    HTTP_GetRequest(p_download_task->context_id, &get_param, 0);

    SCI_FREE(get_param.accept.accept_ptr);
    SCI_FREE(get_param.accept_charset.accept_charset_ptr);
    SCI_FREE(get_param.accept_language.accept_language_ptr);
    SCI_FREE(get_param.user_agent.user_agent_ptr);
    SCI_FREE(get_param.uri.uri_ptr);

}

LOCAL void HttpDownloadFail(uint32 task_id)
{
    TRACE_WATCHFACE("[HTTP]:download fail");

    HTTP_CloseRequest(s_http_download_info.download_task[task_id].context_id);

    if (0 != s_http_download_info.download_task[task_id].recv_file_handle)
    {
        SFS_CloseFile(s_http_download_info.download_task[task_id].recv_file_handle);
        s_http_download_info.download_task[task_id].recv_file_handle = 0;
    }

    if(0 < s_http_download_info.download_task_num)
    {
        s_http_download_info.download_task_num--;
    }
    if(0 < s_http_download_info.total_task_num)
    {
        s_http_download_info.total_task_num--;
    }

    s_http_download_info.download_task[task_id].is_used = FALSE;
    HttpDownload_SetStatus(task_id,HTTP_DOWNLOAD_FAILED);

    HttpCheckNextDownloadTask();
}

LOCAL void HandleHttpPdpMsg(MMIPDP_CNF_INFO_T*  p_msg_ptr)
{
    if(PNULL == p_msg_ptr)
    {
        TRACE_WATCHFACE("[HTTP]:p_msg_ptr is PNULL");
        return;
    }
    TRACE_WATCHFACE("[HTTP]:msg_id =%d,result=%d",p_msg_ptr->msg_id,p_msg_ptr->result);
    switch(p_msg_ptr->msg_id)
    {
        case MMIPDP_ACTIVE_CNF:
        {
            if(MMIPDP_RESULT_SUCC == p_msg_ptr->result)
            {
                uint8 i = 0;

                TRACE_WATCHFACE("[HTTP]:msg_ptr->nsapi=%d ",p_msg_ptr->nsapi);
                s_http_download_info.net_id = p_msg_ptr->nsapi;
                s_http_download_info.is_pdp_actived = TRUE;
                
                for(i = 0; i < MAX_SUPPORT_NUM; i++)
                {
                    if (HTTP_DOWNLOAD_DOWNLOADING == s_http_download_info.download_task[i].download_status)
                    {
                        HttpInitRequest(i);
                    }
                }
            }
            else
            {
                MMIAPIPDP_Deactive(MMI_MODULE_WATCHFACE);
                SCI_MEMSET(&s_http_download_info,0,sizeof(s_http_download_info));
            }
        }
        break;
        case MMIPDP_DEACTIVE_CNF:
        {
            TRACE_WATCHFACE("[HTTP]:receive MMIPDP_DEACTIVE_CNF");
        }
        break;
        case MMIPDP_DEACTIVE_IND:
        {
            MMIAPIPDP_Deactive(MMI_MODULE_WATCHFACE);
        }
        break;
        default:
            break;
    }
}

LOCAL BOOLEAN HttpActivePdp(void)
{
    uint8 i = 0;
    uint8 index = 0;
    uint8 setting_index = 0;
    BOOLEAN return_val = FALSE;
    MMIPDP_ACTIVE_INFO_T active_info = {0};
    char* p_apn_str = PNULL;
    MMICONNECTION_LINKSETTING_DETAIL_T* p_linksetting = PNULL;
    MMICONNECTION_APNTYPEINDEX_T* p_apntype_index = PNULL;

    p_apntype_index = MMIAPICONNECTION_GetApnTypeIndex(MN_DUAL_SYS_1);
    if (NULL != p_apntype_index)
    {
        TRACE_WATCHFACE("[HTTP]:is_valid=%d, internet_idx:%d\n", p_apntype_index->internet_index[MN_DUAL_SYS_1].is_valid, p_apntype_index->internet_index[MN_DUAL_SYS_1].index);
        if(TRUE == p_apntype_index->internet_index[MN_DUAL_SYS_1].is_valid)
        {
            setting_index = p_apntype_index->internet_index[MN_DUAL_SYS_1].index;
        }
    }

    p_linksetting = MMIAPICONNECTION_GetLinkSettingItemByIndex(MN_DUAL_SYS_1, setting_index);
    if(PNULL != p_linksetting)
    {
        if(0 != p_linksetting->apn_len)
        {
            p_apn_str = (char*)p_linksetting->apn;
        }
    }
    else
    {
        TRACE_WATCHFACE("[HTTP]:p_linksetting is NULL");
    }
    if(PNULL != p_apn_str)
    {
        TRACE_WATCHFACE("[HTTP]:apn=%s,auth_type=%d,ip_type=%d",p_apn_str,p_linksetting->auth_type,p_linksetting->ip_type);
        active_info.app_handler         = MMI_MODULE_WATCHFACE;
        active_info.dual_sys            = MN_DUAL_SYS_1;
        active_info.apn_ptr             = p_apn_str;
        active_info.user_name_ptr       = NULL;
        active_info.psw_ptr             = NULL;
        active_info.priority            = 3;
        active_info.ps_service_rat      = MN_UNSPECIFIED;
        active_info.ps_interface        = MMIPDP_INTERFACE_GPRS;
        active_info.handle_msg_callback = HandleHttpPdpMsg;
        active_info.ps_service_type = BROWSER_E;
        active_info.storage = MN_GPRS_STORAGE_ALL;
        active_info.auth_type = p_linksetting->auth_type;
#ifdef IPVERSION_SUPPORT_V4_V6
        active_info.ip_type = p_linksetting->ip_type;
#endif
        if(MMIAPIPDP_Active(&active_info))
        {
            return_val = TRUE;
        }
        TRACE_WATCHFACE("[HTTP]:return_val = %d,moduleid=0x%x",return_val,active_info.app_handler);
    }
    else
    {
        TRACE_WATCHFACE("[HTTP]:p_apn_str is NULL");
    }
    return return_val;
}

LOCAL MMI_RESULT_E HandleHttpGetCnf(DPARAM param)
{
    HTTP_GET_CNF_SIG_T signal = {0};
    int32 task_id = -1;
    uint32 i = 0;
    HTTP_DOWNLOAD_TASK_T *download_task = PNULL;

    if(PNULL == param)
    {
        TRACE_WATCHFACE("[HTTP]:param is PNULL");
        return MMI_RESULT_FALSE;
    }

    signal = *(HTTP_GET_CNF_SIG_T *)param;

    for(i = 0; i < MAX_SUPPORT_NUM; i++)
    {
        if (signal.context_id == s_http_download_info.download_task[i].context_id)
        {
            download_task = &s_http_download_info.download_task[i];
            task_id = i;
            break;
        }
    }

    if(MAX_SUPPORT_NUM <= i)
    {
        TRACE_WATCHFACE("[HTTP]:download_task is not compare");
        return MMI_RESULT_FALSE;
    }
    
    if (signal.result == HTTP_SUCCESS)
    {
        if(MIME_TYPE_TEXT_HTML == download_task->mime_type)
        {
            HttpGetRequest(download_task);
        }
        else
        {
            char *p_file_dir = PNULL;
            wchar base_dir[MAX_URL_LEN] = {0};
            wchar file_name[MAX_URL_LEN] = {0};
            
            s_http_download_info.download_task_num--;
            s_http_download_info.total_task_num--;
            download_task->is_used = FALSE;

            if (0 != download_task->recv_file_handle)
            {
                SFS_CloseFile(download_task->recv_file_handle);
                download_task->recv_file_handle = 0;
            }

            HttpDownload_SetStatus(task_id,HTTP_DOWNLOAD_DOWNLOADED);
            TRACE_WATCHFACE("[HTTP]:begin zip,base_dir is %s,file_name is %s",p_file_dir,download_task->file_name);
            //todo:begin zip
            WATCHFACE_StartUnzip(download_task->file_name);

            HTTP_CloseRequest(download_task->context_id);
            HttpCheckNextDownloadTask();
        }
    }
    else
    {
        HttpDownloadFail(task_id);
        return MMI_RESULT_FALSE;
    }
    return MMI_RESULT_TRUE;
}

LOCAL MMI_RESULT_E HandleHttpRequestIdInd(DPARAM param)
{
    HTTP_REQUEST_ID_IND_SIG_T signal = {0};
    uint32 i = 0;

    if(PNULL == param)
    {
        TRACE_WATCHFACE("[HTTP]:param is PNULL");
        return MMI_RESULT_FALSE;
    }

    signal = *(HTTP_REQUEST_ID_IND_SIG_T *)param;

    for(i = 0; i < MAX_SUPPORT_NUM; i++)
    {
        if (signal.context_id == s_http_download_info.download_task[i].context_id)
        {
            s_http_download_info.download_task[i].app_instance = signal.app_instance;
            s_http_download_info.download_task[i].request_id = signal.request_id;
            
            TRACE_WATCHFACE("[HTTP]:app_instance is %d,request_id is %d",signal.app_instance,signal.request_id);
            return MMI_RESULT_TRUE;
        }
    }
    return MMI_RESULT_FALSE;
}

LOCAL MMI_RESULT_E HandleHttpHeaderInd(DPARAM param)
{
    HTTP_HEADER_IND_SIG_T signal = {0};
    int32 task_id = -1;
    uint32 i = 0;

    if(PNULL == param)
    {
        TRACE_WATCHFACE("[HTTP]:param is PNULL");
        return MMI_RESULT_FALSE;
    }
    signal = *(HTTP_HEADER_IND_SIG_T *)param;
    TRACE_WATCHFACE("[HTTP]:response_code:%d",signal.rsp_header_info.response_code);
    TRACE_WATCHFACE("[HTTP]:header_ptr:%s",signal.header_ptr);
    for(i = 0; i < MAX_SUPPORT_NUM; i++)
    {
        if (signal.context_id == s_http_download_info.download_task[i].context_id)
        {
            task_id = i;
            break;
        }
    }
    if(MAX_SUPPORT_NUM <= i)
    {
        TRACE_WATCHFACE("[HTTP]:context_id = %d,not compare",signal.context_id);
        return MMI_RESULT_FALSE;
    }
    if(FALSE == ((signal.rsp_header_info.response_code == 200)||(signal.rsp_header_info.response_code == 206)))
    {
        HttpDownloadFail(task_id);
        return MMI_RESULT_FALSE;
    }
    else
    {
#ifdef DL_SUPPORT
        s_http_download_info.download_task[i].mime_type = MMIDL_GetMimeTypeByhttpHeader(signal.header_ptr);
        TRACE_WATCHFACE("[HTTP]:task_id is %d,mime_type is %d",i,s_http_download_info.download_task[i].mime_type);
#endif
    }
    return MMI_RESULT_TRUE;
}

LOCAL MMI_RESULT_E HandleHttpErrorInd(DPARAM param)
{
    uint32 i = 0;
    int32 task_id = -1;
    HTTP_ERROR_IND_SIG_T signal = {0};

    if(PNULL == param)
    {
        TRACE_WATCHFACE("[HTTP]:param is PNULL");
        return MMI_RESULT_FALSE;
    }

    signal = *(HTTP_ERROR_IND_SIG_T*)param;

    for(i = 0; i < MAX_SUPPORT_NUM; i++)
    {
        if (signal.context_id == s_http_download_info.download_task[i].context_id)
        {
            task_id = i;
            break;
        }
    }
    
    if(MAX_SUPPORT_NUM <= i)
    {
        TRACE_WATCHFACE("[HTTP]:context_id = %d,not compare",signal.context_id);
        return MMI_RESULT_FALSE;
    }

    HttpDownloadFail(task_id);
    return MMI_RESULT_FALSE;
}


LOCAL MMI_RESULT_E HandleHttpCloseCnf(DPARAM param)
{
    uint32              i = 0;
    HTTP_DOWNLOAD_TASK_T *download_task = PNULL;
    HTTP_CLOSE_CNF_SIG_T signal = {0};

    if(PNULL == param)
    {
        TRACE_WATCHFACE("[HTTP]:param is PNULL");
        return MMI_RESULT_FALSE;
    }

    signal = *(HTTP_CLOSE_CNF_SIG_T *)param;

    for(i = 0; i < MAX_SUPPORT_NUM; i++)
    {
        if (signal.context_id == s_http_download_info.download_task[i].context_id)
        {
            download_task = &s_http_download_info.download_task[i];
            break;
        }
    }
    
    if(MAX_SUPPORT_NUM <= i)
    {
        TRACE_WATCHFACE("[HTTP]:context_id = %d,not compare",signal.context_id);
        return MMI_RESULT_FALSE;
    }
    download_task->context_id = 0;

    return MMI_RESULT_TRUE;
}


LOCAL void HttpGetAbsolutePath(uint8*   p_data, uint8* p_dest_str)
{
    uint8 * dest_str1 = PNULL;
    uint8 * dest_str2 = PNULL;
    uint16 dest_len = 0;

    if(PNULL == p_data)
    {
        TRACE_WATCHFACE("[HTTP]:p_data is PNULL");
        return;
    }
    
    if(PNULL == p_dest_str)
    {
        TRACE_WATCHFACE("[HTTP]:p_dest_str is PNULL");
        return;
    }
    dest_str1 = (uint8 *)MMIAPICOM_Stristr((char *)p_data, WATCH_STRING_HTTP);
    dest_str2 = (uint8 *)MMIAPICOM_Stristr((char *)p_data, WATCH_STRING_ZIP);
    dest_str2 += strlen(WATCH_STRING_ZIP);

    dest_len = dest_str2 - dest_str1;
    
    SCI_MEMCPY(p_dest_str,dest_str1,dest_len);
    TRACE_WATCHFACE("[HTTP]:dest_str1 is %s",dest_str1);
    TRACE_WATCHFACE("[HTTP]:dest_str2 is %s",dest_str2);
    TRACE_WATCHFACE("[HTTP]:p_dest_str is %s",p_dest_str);

}

LOCAL MMI_RESULT_E HandleHttpDataInd(DPARAM param)
{
    HTTP_DATA_IND_SIG_T signal = {0};
    uint32              bytes_writen  = 0;
    uint32              i = 0;
    HTTP_DOWNLOAD_TASK_T *download_task = PNULL;
    char temp_name[MAX_URL_LEN] = {0};

    if(PNULL == param)
    {
        TRACE_WATCHFACE("[HTTP]:param is PNULL");
        return MMI_RESULT_FALSE;
    }

    signal = *(HTTP_DATA_IND_SIG_T *)param;

    for(i = 0; i < MAX_SUPPORT_NUM; i++)
    {
        if (signal.context_id == s_http_download_info.download_task[i].context_id)
        {
            download_task = &s_http_download_info.download_task[i];
            TRACE_WATCHFACE("[HTTP]:download_task index is %d",i);
            break;
        }
    }
    
    if(MAX_SUPPORT_NUM <= i)
    {
        TRACE_WATCHFACE("[HTTP]:download_task is not compare");
        return MMI_RESULT_FALSE;
    }
    TRACE_WATCHFACE("[HTTP]:data_len=%d, data_ptr=%s/r/n",signal.data_len,signal.data_ptr);

    if(MIME_TYPE_TEXT_HTML == download_task->mime_type)
    {
        uint8 dest_url[MAX_URL_LEN] = {0};

        HttpGetAbsolutePath(signal.data_ptr,&dest_url);
        SCI_MEMCPY(download_task->url,dest_url,sizeof(dest_url));
    }
    else
    {
        if (0 == download_task->recv_file_handle)
        {
            //first create file
            TRACE_WATCHFACE("[HTTP]:create a new handle");
            MMIAPICOM_WstrToStr(download_task->save_fullpath,temp_name);
            TRACE_WATCHFACE("[HTTP]:save_fullpath is %s",temp_name);
            download_task->recv_file_handle = SFS_CreateFile(download_task->save_fullpath,SFS_MODE_WRITE | SFS_MODE_OPEN_ALWAYS | SFS_MODE_APPEND,NULL,NULL);
            if (0 == download_task->recv_file_handle)
            {
                TRACE_WATCHFACE("[HTTP]:recv_file_handle is NULL");
                return MMI_RESULT_TRUE;
            }
        }

        //save as file
        if (SFS_NO_ERROR != SFS_WriteFile(download_task->recv_file_handle,signal.data_ptr,signal.data_len,&bytes_writen,PNULL))
        {
            SFS_CloseFile(download_task->recv_file_handle);
            download_task->recv_file_handle = 0;
            TRACE_WATCHFACE("[HTTP]:SFS_WriteFile error");
            return MMI_RESULT_TRUE;
        }
    }
    return MMI_RESULT_TRUE;
}

LOCAL MMI_RESULT_E HandleHttpInitCnf(DPARAM param)
{
    HTTP_INIT_CNF_SIG_T signal = {0};
    uint32  task_id = 0;
    uint32              i = 0;
    HTTP_DOWNLOAD_TASK_T *p_download_task = PNULL;
    char temp_name[MAX_URL_LEN] = {0};

    if(PNULL == param)
    {
        TRACE_WATCHFACE("[HTTP]:param is PNULL");
        return MMI_RESULT_FALSE;
    }
    signal = *(HTTP_INIT_CNF_SIG_T *)param;
    task_id = signal.app_param;
    p_download_task = &s_http_download_info.download_task[task_id];       

    TRACE_WATCHFACE("[HTTP]:task_id = %d",task_id);

    if(signal.module_id != MMI_MODULE_WATCHFACE)
    {
        TRACE_WATCHFACE("[HTTP]:module_id = %d",signal.module_id);
        return MMI_RESULT_FALSE;
    }

    p_download_task->context_id   = signal.context_id;

    HttpGetRequest(p_download_task);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : 获取未使用的task_id
//  Parameter: None
//  Author: xiaoju.cheng
//  Note:
/*****************************************************************************/
LOCAL int32 GetUnusedDownloadTaskId(void)
{
    int32 unused_id = -1;
    int32 i = 0;

    for (i = 0; i < MAX_SUPPORT_NUM; i++)
    {
        if (FALSE == s_http_download_info.download_task[i].is_used)
        {
            unused_id = i;
            break;
        }
    }
    TRACE_WATCHFACE("[HTTP]:unused_id is %d",unused_id);
    return unused_id;
}

/*****************************************************************************/
//  Description : 判断是否是相同的url，防止重复下载。
//  Parameter: None
//  Author: xiaoju.cheng
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsSameDownloadDestUrl(char* p_url,uint32* p_same_task_index)
{
    uint32 i = 0;
    BOOLEAN is_same_url = FALSE;

    if (PNULL == p_url)
    {
        TRACE_WATCHFACE("[HTTP]:p_url is PNULL");
        return is_same_url;
    }

    if (PNULL == p_same_task_index)
    {
        TRACE_WATCHFACE("[HTTP]:p_same_task_index is PNULL");
        return is_same_url;
    }

    for (i = 0; i < MAX_SUPPORT_NUM; i++)
    {
        if (0 == strcmp(p_url, s_http_download_info.download_task[i].url))
        {
            *p_same_task_index = i;
            is_same_url = TRUE;
            break;
        }
    }

    TRACE_WATCHFACE("[HTTP]:same_url index is %d",i);

    return is_same_url;
}

LOCAL MMI_RESULT_E HandleWatchfaceHttpMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
    //当result为true时，后续注册的app不需要再响应http的消息
    MMI_RESULT_E result = MMI_RESULT_FALSE;

    TRACE_WATCHFACE("[HTTP]:msg_id=0x%x ",msg_id);
    switch (msg_id)
    {
        case HTTP_SIG_INIT_CNF:
        {
            //第一次发送的消息比较特殊
            //http协议服务已经准备好，开始连接服务器
            //初始化要发送的数据,get or post
            TRACE_WATCHFACE("[HTTP]:receive HTTP_SIG_INIT_CNF");
            result = HandleHttpInitCnf(param);
        }
        break;
        case HTTP_SIG_REQUEST_ID_IND:
        {
            //第一次请求后，http分配给app的id
            TRACE_WATCHFACE("[HTTP]:receive HTTP_SIG_REQUEST_ID_IND");
            result = HandleHttpRequestIdInd(param);
        }
        break;
        case HTTP_SIG_HEADER_IND:
        {
            TRACE_WATCHFACE("[HTTP]:receive HTTP_SIG_HEADER_IND");
            result = HandleHttpHeaderInd(param);
        }
        break;
        case HTTP_SIG_ERROR_IND:
        {
            TRACE_WATCHFACE("[HTTP]:receive HTTP_SIG_ERROR_IND");
            result = HandleHttpErrorInd(param);
        }
        break;
        
        case HTTP_SIG_DATA_IND:
        {
            TRACE_WATCHFACE("[HTTP]:receive HTTP_SIG_DATA_IND");
            result=HandleHttpDataInd(param);
        }
        break;
        case HTTP_SIG_GET_CNF:
        {
            TRACE_WATCHFACE("[HTTP] receive HTTP_SIG_GET_CNF");
            result = HandleHttpGetCnf(param);
        }
        break;
        case HTTP_SIG_CLOSE_CNF://HTTP_SIG_CLOSE_REQ消息的回复,告知app,http服务关闭
        {
            TRACE_WATCHFACE("[HTTP]:receive HTTP_SIG_CLOSE_CNF");
            result = HandleHttpCloseCnf(param);
        }
        break;
     default:
        break;
    }

    TRACE_WATCHFACE("[HTTP]:result = %d",result);
    if(MMI_RESULT_FALSE == result)
    {
        WATCHFACE_Set_InstallStatus(WATCHFACE_INSTALL_STATUS_IDLE);
    }
    return result;
}

/*****************************************************************************/
//  Description : watchface http start
//  Parameter: [In] p_url:url to be download
//             [In] p_id:download file name
//             [Return] None
//  Author: xiaoju.cheng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN WATCHFACE_HttpStart(char* p_url, char* p_id)
{
    BOOLEAN send_again = FALSE;
    uint16 file_name_len = 0;
    int32 task_id = -1;
    BOOLEAN is_same_dest_url = 0;
    uint32 same_task_index = 0;
    HTTP_DOWNLOAD_STATUS_E download_status = HTTP_DOWNLOAD_NONE;
    char fullpath[MAX_URL_LEN+1] = {0};

    BOOLEAN result = TRUE;

    if(PNULL == p_url)
    {
        TRACE_WATCHFACE("[HTTP]:p_url is null");
        result = FALSE;
        return result;
    }
    if(PNULL == p_id)
    {
        TRACE_WATCHFACE("[HTTP]:p_id is null");
        result = FALSE;
        return result;
    }

    is_same_dest_url = IsSameDownloadDestUrl(p_url,&same_task_index);
    if (TRUE == is_same_dest_url)
    {
        HttpDownload_GetStatus(same_task_index,&download_status);

        if(HTTP_DOWNLOAD_DOWNLOADING == download_status)
        {
            TRACE_WATCHFACE("[HTTP]:is_same_dest_url");
            result = FALSE;
            return result;
        }
    }

    if(MAX_SUPPORT_NUM <= s_http_download_info.total_task_num)
    {
        TRACE_WATCHFACE("[HTTP]:total_task_num = %d,exceed max number",s_http_download_info.total_task_num);
        result = FALSE;
        return result;
    }
    s_http_download_info.total_task_num++;

    task_id = GetUnusedDownloadTaskId();

    TRACE_WATCHFACE("task_id:%d",task_id);

    if(0 <= task_id)
    {
        //bug id:1941096
        SCI_MEMSET(s_http_download_info.download_task[task_id].url,0,MAX_URL_LEN);
		SCI_MEMSET(s_http_download_info.download_task[task_id].file_name,0,MAX_URL_LEN);
        SCI_MEMCPY(s_http_download_info.download_task[task_id].url,p_url,strlen(p_url));
        SCI_MEMCPY(s_http_download_info.download_task[task_id].file_name,p_id,strlen(p_id));
        SCI_MEMSET(fullpath,0,MAX_URL_LEN+1);
        sprintf(fullpath,"%s\\%s.zip",WATCHFACE_Get_UnintalledFileFolder(),p_id);
        TRACE_WATCHFACE("[HTTP]:url is %s,fullpath is %s",p_url,fullpath);
		SCI_MEMSET(s_http_download_info.download_task[task_id].save_fullpath,0,sizeof(wchar)*MAX_URL_LEN);
        MMI_STRNTOWSTR(s_http_download_info.download_task[task_id].save_fullpath,
                    MAX_URL_LEN,
                    fullpath,
                    MAX_URL_LEN,
                    SCI_STRLEN(fullpath));
        s_http_download_info.download_task[task_id].is_used = TRUE;
    }

    if (MAX_DOWLOADING_NUM <= s_http_download_info.download_task_num)
    {
        TRACE_WATCHFACE("[HTTP]:download_task_num = %d,exceed multi_max num",s_http_download_info.download_task_num);
        HttpDownload_SetStatus(task_id,HTTP_DOWNLOAD_WAITING);
    }
    else
    {
#ifdef UAL_NETWORKMGR_SUPPORT    
        if(TRUE == s_http_download_info.is_pdp_actived)
        {
            s_http_download_info.download_task_num++;
            HttpDownload_SetStatus(task_id,HTTP_DOWNLOAD_DOWNLOADING);
            HttpInitRequest(task_id);
        }
        else
        {
            ual_networkmgr_status_t  networkmgr_info = {0};
            uint8 i = 0;

            ual_networkmgr_get_net_info_by_route(UAL_NETWORKMGR_ROUTE_DEFAULT, &networkmgr_info);
            TRACE_WATCHFACE("[HTTP]:net_id is %d",networkmgr_info.net_id);

            if(0 < networkmgr_info.net_id)
            {
                s_http_download_info.is_pdp_actived = TRUE;
                s_http_download_info.net_id = networkmgr_info.net_id;
                s_http_download_info.download_task_num++;
                HttpDownload_SetStatus(task_id,HTTP_DOWNLOAD_DOWNLOADING);
                HttpInitRequest(task_id);
            }
            else
            {
                SCI_MEMSET(&s_http_download_info,0,sizeof(s_http_download_info));
                TRACE_WATCHFACE("[HTTP]:pdp not active");
            }

        }
#else
        s_http_download_info.download_task_num++;
        HttpDownload_SetStatus(task_id,HTTP_DOWNLOAD_DOWNLOADING);
        if(TRUE == s_http_download_info.is_pdp_actived)
        {
            HttpInitRequest(task_id);
        }
        else
        {
            result = HttpActivePdp();
        }
#endif
    }
	return result;
}

/*****************************************************************************/
//  Description : watchface http module init
//  Parameter: None
//  Author: xiaoju.cheng
//  Note:
/*****************************************************************************/
PUBLIC void  WATCHFACE_HTTP_InitModule(void)
{
    TRACE_WATCHFACE("[HTTP]:init module");
    g_watchhttp_app.ProcessMsg = HandleWatchfaceHttpMsg;
}
#endif
