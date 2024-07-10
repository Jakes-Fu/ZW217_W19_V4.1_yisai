/*****************************************************************************
** Copyright 2022 <Name of Copyright Owner: qi.zhang>                        *
** Licensed under the Unisoc General Software License,                       *
** version 1.0 (the License);                                                *
** you may not use this file except in compliance with the License.          *
** You may obtain a copy of the License at                                   *
** https://www.unisoc.com/en_us/license/UNISOC_GENERAL_LICENSE_V1.0-EN_US    *
** Software distributed under the License is distributed on an "AS IS" BASIS,*
** WITHOUT WARRANTIES OF ANY KIND, either express or implied.                *
** See the Unisoc General Software License, version 1.0 for more details.    *
******************************************************************************/
/*****************************************************************************
** File Name:      native_http.c                                             *
** Author:                                                                   *
** Date:           2022.08.25                                                *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to do provide network access api        *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME                         DESCRIPTION                   *
** 2021.11.30     qi.zhang                     Create                        *
**                                                                           *
******************************************************************************/
#ifdef JS_COMPENT_HTTP //for 217 ram cutdown
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
#include "mime_type.h"
#ifdef UAL_NETWORKMGR_SUPPORT
#include "ual_networkmgr.h"
#endif
#include "native_http.h"
#include "mmipdp_export.h"

/*---------------------------------------------------------------------------*/
/*                        Macro Declaration                                  */
/*---------------------------------------------------------------------------*/
#define MAX_URL_LEN             (512)
#define MAX_SUPPORT_NUM         (20)
#define MAX_DOWLOADING_NUM      (3)
#define HTTP_MODULE_ID          (0x04 << 16)//TODO:temp module_id

typedef enum
{
    HTTP_DOWNLOAD_NONE,
    HTTP_DOWNLOAD_DOWNLOADING,
    HTTP_DOWNLOAD_DOWNLOADED,
    HTTP_DOWNLOAD_FAILED,
    HTTP_DOWNLOAD_WAITING,
    HTTP_DOWNLOAD_STATUS_MAX
}http_download_status_e;

typedef struct
{
    HTTP_APP_INSTANCE_T                 app_instance; //用于区分不同的HTTP请求。同时只有一个HTTP请求时可以为任意值。
    char                                url[MAX_URL_LEN];
    uint32                              receive_data_len;
    char*                               p_request_data;
    char*                               p_receive_data;
    boolean                             is_used;
    uint32                              mime_type;
    boolean                             is_need_redirect;
    http_download_status_e              download_status;
    http_request_type_e                 request_type;
    http_register_callback              call_back;
}http_download_task_t;

typedef struct
{
    HTTP_CONTEXT_ID_T       context_id;
    uint32                  net_id;
    uint32                  download_task_num;
    uint32                  total_task_num;
    BOOLEAN                 is_pdp_actived;
    http_download_task_t    download_task[MAX_SUPPORT_NUM];
}http_download_info_t;

#define HTTP_ACCEPT_STRING            "text/vnd.wap.wml,application/vnd.wap.xhtml+xml,application/xhtml+xml, image/vnd.wap.wbmp, image/gif, image/jpg,image/jpeg, image/png, image/bmp, text/html,*/*;q=0.1"
#define HTTP_ACCEPT_LANGUAGE_STRING   "zh-cn, en" //表示接受中文英文
#define HTTP_USER_AGENT_STRING        "IOSMsfClient,Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.0)"
#define HTTP_ACCEPT_CHARSET_STRING    "UTF-8"//"UTF-8,ISO-8859-1,US-ASCII,windows-1252"//"*utf-8*"
#define HTTP_CONTENT_TYPE             "application/json"
#define HTTP_STRING_HTTP              "http://"
#define HTTP_STRING_HTTPS             "https://"
#define HTTP_STRING_ZIP               ".zip"
#define HTTP_HEADER_SUCCESS_PARAM1    (200)
#define HTTP_HEADER_SUCCESS_PARAM2    (206)
#define HTTP_CONTENT_TYPE_LEN         (100)
MMI_APPLICATION_T            g_http_app = {0};
LOCAL http_download_info_t   s_http_download_info = {0};
LOCAL uint8 s_http_init_timer_id = 0;
LOCAL char s_content_type[HTTP_CONTENT_TYPE_LEN + 1] = "application/json";
/*---------------------------------------------------------------------------*/
/*                        Function Declaration                               */
/*---------------------------------------------------------------------------*/

LOCAL void HttpDownload_GetStatus(int32 task_id,http_download_status_e* p_download_status)
{
    if(PNULL != p_download_status)
    {
        if(0 <= task_id)
        {
            *p_download_status = s_http_download_info.download_task[task_id].download_status;
            SCI_TRACE_LOW("[HTTP]:p_download_status = %d",*p_download_status);
        }
    }
}

LOCAL void HttpDownload_SetStatus(int32 task_id,http_download_status_e download_status)
{
    if(0 <= task_id)
    {
        s_http_download_info.download_task[task_id].download_status = download_status;
        SCI_TRACE_LOW("[HTTP]:download_status = %d",download_status);
    }
}

LOCAL void HttpInitRequest(uint32 task_id)
{
    HTTP_INIT_PARAM_T* p_init_param = PNULL;

    p_init_param = SCI_ALLOC_APPZ(sizeof(HTTP_INIT_PARAM_T));
    if(PNULL == p_init_param)
    {
        SCI_TRACE_LOW("[HTTP] p_init_param is PNULL");
        return;
    }
    p_init_param->is_cookie_enable = FALSE;
    p_init_param->is_cache_enable = FALSE;
    p_init_param->net_id = s_http_download_info.net_id;
    SCI_TRACE_LOW("[HTTP] net_id = %d", p_init_param->net_id);

    //TODO:app_param的值需要确认
    HTTP_InitRequest(HTTP_MODULE_ID, task_id, p_init_param);

    SCI_FREE(p_init_param);
}

LOCAL void HttpPostRequest(http_download_task_t* p_download_task )
{
    HTTP_POST_PARAM_T  post_param = {0};
    char  content_type[HTTP_CONTENT_TYPE_LEN + 1] = {0};
    uint16 len = 0;

    SCI_TRACE_LOW("[HTTP]:enter HttpPostRequest");

    if(PNULL == p_download_task)
    {
        SCI_TRACE_LOW("[HTTP]:p_download_task is PNULL");
        return;
    }
    post_param.connection = HTTP_CONNECTION_KEEP_ALIVE;
    post_param.need_net_prog_ind = TRUE;

    len=strlen(HTTP_ACCEPT_STRING);
    post_param.accept.accept_ptr = SCI_ALLOC_APPZ(len+1);
    if(PNULL != post_param.accept.accept_ptr)
    {
        SCI_MEMCPY(post_param.accept.accept_ptr,HTTP_ACCEPT_STRING,len);
    }
    else
    {
        SCI_TRACE_LOW("[HTTP]:post_param.accept.accept_ptr malloc fail");
        return;
    }

    len=strlen(HTTP_ACCEPT_LANGUAGE_STRING);
    post_param.accept_language.accept_language_ptr = SCI_ALLOC_APPZ(len+1);
    if(PNULL != post_param.accept_language.accept_language_ptr)
    {
        SCI_MEMCPY(post_param.accept_language.accept_language_ptr,HTTP_ACCEPT_LANGUAGE_STRING,len);
    }
    else
    {
        SCI_FREE(post_param.accept.accept_ptr);
        SCI_TRACE_LOW("[HTTP]:post_param.accept_language.accept_language_ptr malloc fail");
        return;
    }

    len=strlen(HTTP_USER_AGENT_STRING);
    post_param.user_agent.user_agent_ptr = SCI_ALLOC_APPZ(len+1);
    if(PNULL != post_param.user_agent.user_agent_ptr)
    {
        SCI_MEMCPY(post_param.user_agent.user_agent_ptr,HTTP_USER_AGENT_STRING,len);
    }
    else
    {
        SCI_TRACE_LOW("[HTTP]:post_param.user_agent.user_agent_ptr malloc fail");
        SCI_FREE(post_param.accept.accept_ptr);
        SCI_FREE(post_param.accept_language.accept_language_ptr);
        return;
    }

    len = strlen(HTTP_ACCEPT_CHARSET_STRING);
    post_param.accept_charset.accept_charset_ptr = SCI_ALLOC_APPZ(len + 1);
    if(PNULL != post_param.accept_charset.accept_charset_ptr)
    {
        SCI_MEMCPY(post_param.accept_charset.accept_charset_ptr,HTTP_ACCEPT_CHARSET_STRING,len);
    }
    else
    {
        SCI_FREE(post_param.accept.accept_ptr);
        SCI_FREE(post_param.accept_language.accept_language_ptr);
        SCI_FREE(post_param.user_agent.user_agent_ptr);
        SCI_TRACE_LOW("[HTTP]:post_param.accept_charset.accept_charset_ptr malloc fail");
        return;
    }
    SCI_MEMCPY(content_type, s_content_type, HTTP_CONTENT_TYPE_LEN + 1);
    len = strlen(content_type);
    post_param.post_body.body_type_ptr = SCI_ALLOC_APPZ(len + 1);
    if(PNULL != post_param.post_body.body_type_ptr)
    {
        SCI_MEMCPY(post_param.post_body.body_type_ptr,content_type,len);
    }
    else
    {
        SCI_TRACE_LOW("[HTTP]:post_param.post_body.body_type_ptr malloc fail");
        SCI_FREE(post_param.accept.accept_ptr);
        SCI_FREE(post_param.accept_language.accept_language_ptr);
        SCI_FREE(post_param.user_agent.user_agent_ptr);
        SCI_FREE(post_param.accept_charset.accept_charset_ptr);
        return;
    }

    //设置数据
    if(PNULL != p_download_task->p_request_data)
    {
        post_param.post_body.u.post_buffer.is_copied_by_http = TRUE;
        post_param.post_body.u.post_buffer.buffer_ptr = p_download_task->p_request_data;
        post_param.post_body.u.post_buffer.buffer_len = strlen(p_download_task->p_request_data);
    }

    //拼接URL
    len=strlen(p_download_task->url);
    post_param.uri.uri_ptr= SCI_ALLOC_APPZ(len+1);
    if(PNULL != post_param.uri.uri_ptr)
    {
        SCI_MEMCPY(post_param.uri.uri_ptr,p_download_task->url,len);
    }
    else
    {
        SCI_TRACE_LOW("[HTTP]:post_param.uri.uri_ptr malloc fail");
        SCI_FREE(post_param.accept.accept_ptr);
        SCI_FREE(post_param.accept_language.accept_language_ptr);
        SCI_FREE(post_param.user_agent.user_agent_ptr);
        SCI_FREE(post_param.accept_charset.accept_charset_ptr);
        SCI_FREE(post_param.post_body.body_type_ptr);
        return;
    }
    SCI_TRACE_LOW("[HTTP]:uri = %s",post_param.uri.uri_ptr);

    //使用buffer的方式保存回应数据，不需要设置文件名称
    post_param.get_data.is_header_use_file = FALSE;
    post_param.get_data.style = HTTP_DATA_TRANS_STYLE_BUFFER;

    HTTP_PostRequest(s_http_download_info.context_id, &post_param, p_download_task->app_instance);

    SCI_FREE(post_param.accept.accept_ptr);
    SCI_FREE(post_param.accept_language.accept_language_ptr);
    SCI_FREE(post_param.user_agent.user_agent_ptr);
    SCI_FREE(post_param.accept_charset.accept_charset_ptr);
    SCI_FREE(post_param.post_body.body_type_ptr);
    SCI_FREE(post_param.uri.uri_ptr);
}

LOCAL void HttpGetRequest(http_download_task_t* p_download_task )
{
    HTTP_GET_PARAM_T  get_param = {0};
    uint16 len = 0;

    SCI_TRACE_LOW("[HTTP]:enter HttpGetRequest");
    if(PNULL == p_download_task)
    {
        SCI_TRACE_LOW("[HTTP]:p_download_task is PNULL");
        return;
    }
    get_param.connection = HTTP_CONNECTION_KEEP_ALIVE;
    get_param.need_net_prog_ind = TRUE;

    len=strlen(HTTP_ACCEPT_STRING);
    get_param.accept.accept_ptr = SCI_ALLOC_APPZ(len+1);
    if(PNULL != get_param.accept.accept_ptr)
    {
        SCI_MEMCPY(get_param.accept.accept_ptr,HTTP_ACCEPT_STRING,len);
    }
    else
    {
        SCI_TRACE_LOW("[HTTP]:get_param.accept.accept_ptr malloc fail");
        return;
    }

    len=strlen(HTTP_ACCEPT_LANGUAGE_STRING);
    get_param.accept_language.accept_language_ptr = SCI_ALLOC_APPZ(len+1);
    if(PNULL != get_param.accept_language.accept_language_ptr)
    {
        SCI_MEMCPY(get_param.accept_language.accept_language_ptr,HTTP_ACCEPT_LANGUAGE_STRING,len);
    }
    else
    {
        SCI_FREE(get_param.accept.accept_ptr);
        SCI_TRACE_LOW("[HTTP]:get_param.accept_language.accept_language_ptr malloc fail");
        return;
    }

    len=strlen(HTTP_USER_AGENT_STRING);
    get_param.user_agent.user_agent_ptr = SCI_ALLOC_APPZ(len+1);
    if(PNULL != get_param.user_agent.user_agent_ptr)
    {
        SCI_MEMCPY(get_param.user_agent.user_agent_ptr,HTTP_USER_AGENT_STRING,len);
    }
    else
    {
        SCI_FREE(get_param.accept.accept_ptr);
        SCI_FREE(get_param.accept_language.accept_language_ptr);
        SCI_TRACE_LOW("[HTTP]:get_param.user_agent.user_agent_ptr malloc fail");
        return;
    }

    len = strlen(HTTP_ACCEPT_CHARSET_STRING);
    get_param.accept_charset.accept_charset_ptr = SCI_ALLOC_APPZ(len + 1);
    if(PNULL != get_param.accept_charset.accept_charset_ptr)
    {
        SCI_MEMCPY(get_param.accept_charset.accept_charset_ptr,HTTP_ACCEPT_CHARSET_STRING,len);
    }
    else
    {
        SCI_FREE(get_param.accept.accept_ptr);
        SCI_FREE(get_param.accept_language.accept_language_ptr);
        SCI_FREE(get_param.user_agent.user_agent_ptr);
        SCI_TRACE_LOW("[HTTP]:get_param.accept_charset.accept_charset_ptr malloc fail");
        return;
    }

    //拼接URL
    len=strlen(p_download_task->url);
    get_param.uri.uri_ptr= SCI_ALLOC_APPZ(len+1);
    if(PNULL != get_param.uri.uri_ptr)
    {
        SCI_MEMCPY(get_param.uri.uri_ptr,p_download_task->url,len);
    }
    else
    {
        SCI_FREE(get_param.accept.accept_ptr);
        SCI_FREE(get_param.accept_language.accept_language_ptr);
        SCI_FREE(get_param.user_agent.user_agent_ptr);
        SCI_FREE(get_param.accept_charset.accept_charset_ptr);
        SCI_TRACE_LOW("[HTTP]:get_param.uri.uri_ptr malloc fail");
        return;
    }
    SCI_TRACE_LOW("[HTTP]:uri = %s",get_param.uri.uri_ptr);

    //使用buffer的方式保存回应数据，不需要设置文件名称
    get_param.get_data.is_header_use_file = FALSE;
    get_param.get_data.style = HTTP_DATA_TRANS_STYLE_BUFFER;

    HTTP_GetRequest(s_http_download_info.context_id, &get_param, p_download_task->app_instance);

    SCI_FREE(get_param.accept.accept_ptr);
    SCI_FREE(get_param.accept_language.accept_language_ptr);
    SCI_FREE(get_param.user_agent.user_agent_ptr);
    SCI_FREE(get_param.accept_charset.accept_charset_ptr);
    SCI_FREE(get_param.uri.uri_ptr);
}

LOCAL void HttpOptionsRequest(http_download_task_t* p_download_task )
{
    HTTP_OPTIONS_PARAM_T  options_param = {0};
    uint16 len = 0;

    SCI_TRACE_LOW("[HTTP]:enter HttpOptionsRequest");
    if(PNULL == p_download_task)
    {
        SCI_TRACE_LOW("[HTTP]:p_download_task is PNULL");
        return;
    }
    options_param.connection = HTTP_CONNECTION_KEEP_ALIVE;
    options_param.need_net_prog_ind = TRUE;

    len=strlen(HTTP_ACCEPT_STRING);
    options_param.accept.accept_ptr = SCI_ALLOC_APPZ(len+1);
    if(PNULL != options_param.accept.accept_ptr)
    {
        SCI_MEMCPY(options_param.accept.accept_ptr,HTTP_ACCEPT_STRING,len);
    }
    else
    {
        SCI_TRACE_LOW("[HTTP]:options_param.accept.accept_ptr malloc fail");
        return;
    }

    len=strlen(HTTP_ACCEPT_LANGUAGE_STRING);
    options_param.accept_language.accept_language_ptr = SCI_ALLOC_APPZ(len+1);
    if(PNULL != options_param.accept_language.accept_language_ptr)
    {
        SCI_MEMCPY(options_param.accept_language.accept_language_ptr,HTTP_ACCEPT_LANGUAGE_STRING,len);
    }
    else
    {
        SCI_FREE(options_param.accept.accept_ptr);
        SCI_TRACE_LOW("[HTTP]:options_param.accept_language.accept_language_ptr malloc fail");
        return;
    }

    len=strlen(HTTP_USER_AGENT_STRING);
    options_param.user_agent.user_agent_ptr = SCI_ALLOC_APPZ(len+1);
    if(PNULL != options_param.user_agent.user_agent_ptr)
    {
        SCI_MEMCPY(options_param.user_agent.user_agent_ptr,HTTP_USER_AGENT_STRING,len);
    }
    else
    {
        SCI_FREE(options_param.accept.accept_ptr);
        SCI_FREE(options_param.accept_language.accept_language_ptr);
        SCI_TRACE_LOW("[HTTP]:options_param.user_agent.user_agent_ptr malloc fail");
        return;
    }

    len = strlen(HTTP_ACCEPT_CHARSET_STRING);
    options_param.accept_charset.accept_charset_ptr = SCI_ALLOC_APPZ(len + 1);
    if(PNULL != options_param.accept_charset.accept_charset_ptr)
    {
        SCI_MEMCPY(options_param.accept_charset.accept_charset_ptr,HTTP_ACCEPT_CHARSET_STRING,len);
    }
    else
    {
        SCI_FREE(options_param.accept.accept_ptr);
        SCI_FREE(options_param.accept_language.accept_language_ptr);
        SCI_FREE(options_param.user_agent.user_agent_ptr);
        SCI_TRACE_LOW("[HTTP]:options_param.accept_charset.accept_charset_ptr malloc fail");
        return;
    }

    //拼接URL
    len=strlen(p_download_task->url);
    options_param.uri.uri_ptr= SCI_ALLOC_APPZ(len+1);
    if(PNULL != options_param.uri.uri_ptr)
    {
        SCI_MEMCPY(options_param.uri.uri_ptr,p_download_task->url,len);
    }
    else
    {
        SCI_FREE(options_param.accept.accept_ptr);
        SCI_FREE(options_param.accept_language.accept_language_ptr);
        SCI_FREE(options_param.user_agent.user_agent_ptr);
        SCI_FREE(options_param.accept_charset.accept_charset_ptr);
        SCI_TRACE_LOW("[HTTP]:options_param.uri.uri_ptr malloc fail");
        return;
    }
    SCI_TRACE_LOW("[HTTP]:uri = %s",options_param.uri.uri_ptr);

    //使用buffer的方式保存回应数据，不需要设置文件名称
    options_param.get_data.is_header_use_file = FALSE;
    options_param.get_data.style = HTTP_DATA_TRANS_STYLE_BUFFER;

    HTTP_OptionsRequest(s_http_download_info.context_id, &options_param, p_download_task->app_instance);

    SCI_FREE(options_param.accept.accept_ptr);
    SCI_FREE(options_param.accept_language.accept_language_ptr);
    SCI_FREE(options_param.user_agent.user_agent_ptr);
    SCI_FREE(options_param.accept_charset.accept_charset_ptr);
    SCI_FREE(options_param.uri.uri_ptr);
}


LOCAL void HttpCheckNextDownloadTask(void)
{
    uint32 i = 0;
    http_download_status_e download_status = HTTP_DOWNLOAD_NONE;

    if (0 == s_http_download_info.total_task_num)
    {
        SCI_TRACE_LOW("[HTTP]:download complete");
        return;
    }

    for (i = 0; i < MAX_SUPPORT_NUM; i++)
    {
        HttpDownload_GetStatus(i,&download_status);

        if (HTTP_DOWNLOAD_WAITING == download_status)
        {
            if(HTTP_POST_REQUEST == s_http_download_info.download_task[i].request_type)
            {
                HttpPostRequest(&s_http_download_info.download_task[i]);
            }
            else if(HTTP_GET_REQUEST == s_http_download_info.download_task[i].request_type)
            {
                HttpGetRequest(&s_http_download_info.download_task[i]);
            }
            else if(HTTP_OPTIONS_REQUEST == s_http_download_info.download_task[i].request_type)
            {
                HttpOptionsRequest(&s_http_download_info.download_task[i]);
            }
            break;
        }
    }
}

LOCAL void HttpDownloadFail(int32 task_id)
{
    http_download_task_t* p_download_task = PNULL;
    HTTP_CloseRequest(s_http_download_info.context_id);
    if(0 > task_id)
    {
        SCI_TRACE_LOW("[HTTP]:HttpDownloadFail task_id error");
        return;
    }

    SCI_TRACE_LOW("[HTTP]:download fail,task_id is %d",task_id);

    //下载失败，清空全局数据，并查看下一个正在等待状态的任务。
    if(0 < s_http_download_info.download_task_num)
    {
        s_http_download_info.download_task_num--;
    }
    SCI_TRACE_LOW("[HTTP]:HttpDownloadFail, download_task_num = %d", s_http_download_info.download_task_num);
    if(0 < s_http_download_info.total_task_num)
    {
        s_http_download_info.total_task_num--;
    }
    SCI_TRACE_LOW("[HTTP]:HttpDownloadFail, total_task_num = %d", s_http_download_info.total_task_num);

    HttpDownload_SetStatus(task_id,HTTP_DOWNLOAD_FAILED);
    p_download_task = &s_http_download_info.download_task[task_id];

    if(PNULL != p_download_task->call_back)
    {
        http_callback_param_t callback_param = {0};
        callback_param.result = HTTP_REQUEST_ERROR;
        callback_param.handle = p_download_task->app_instance;
        callback_param.p_body = FALSE;
        p_download_task->call_back(callback_param);
    }

    SCI_MEMSET(p_download_task,0,sizeof(http_download_task_t));
    if(PNULL != p_download_task->p_receive_data)
    {
        SCI_FREE(p_download_task->p_receive_data);
    }

    if(PNULL != p_download_task->p_request_data)
    {
        SCI_FREE(p_download_task->p_request_data);
    }

    HttpCheckNextDownloadTask();
}

LOCAL MMI_RESULT_E HandleHttpPostCnf(DPARAM param)
{
    HTTP_POST_CNF_SIG_T signal = {0};
    int32 task_id = -1;
    uint32 i = 0;
    http_download_task_t* p_download_task = PNULL;

    if(PNULL == param)
    {
        SCI_TRACE_LOW("[HTTP]:param is PNULL");
        return MMI_RESULT_FALSE;
    }

    signal = *(HTTP_POST_CNF_SIG_T *)param;

    SCI_TRACE_LOW("[HTTP]:HandleHttpPostCnf,app_instance = %d",signal.app_instance);
    for(i = 0; i < MAX_SUPPORT_NUM; i++)
    {
        if (signal.app_instance == s_http_download_info.download_task[i].app_instance)
        {
            p_download_task = &s_http_download_info.download_task[i];
            task_id = i;
            break;
        }
    }

    if(MAX_SUPPORT_NUM <= i)
    {
        SCI_TRACE_LOW("[HTTP]:download_task is not compare");
        return MMI_RESULT_FALSE;
    }

    if (HTTP_SUCCESS == signal.result)
    {
        //如果是网址的重定向，需要根据新的网址重新发起下载。
        if(TRUE == p_download_task->is_need_redirect)
        {
            HttpPostRequest(p_download_task);
        }
        else
        {
            HttpDownload_SetStatus(task_id,HTTP_DOWNLOAD_DOWNLOADED);
            if(0 < s_http_download_info.download_task_num)
            {
                s_http_download_info.download_task_num--;
            }
            SCI_TRACE_LOW("[HTTP]:HandleHttpPostCnf, download_task_num = %d", s_http_download_info.download_task_num);
            if(0 < s_http_download_info.total_task_num)
            {
                s_http_download_info.total_task_num--;
            }
            SCI_TRACE_LOW("[HTTP]:HandleHttpPostCnf, total_task_num = %d", s_http_download_info.total_task_num);

            //下载成功，触发callback。
            if(PNULL != p_download_task->call_back)
            {
                http_callback_param_t callback_param = {0};
                callback_param.result = HTTP_REQUEST_SUCESS;
                callback_param.handle = p_download_task->app_instance;
                callback_param.p_body = (void*)p_download_task->p_receive_data;
                callback_param.body_size= p_download_task->receive_data_len;
                p_download_task->call_back(callback_param);
            }

            SCI_TRACE_LOW("[HTTP]:download complete,task_id is %d,download result is %d",i,signal.result);
            SCI_MEMSET(p_download_task,0,sizeof(http_download_task_t));

            if(PNULL != p_download_task->p_receive_data)
            {
                SCI_FREE(p_download_task->p_receive_data);
            }

            if(PNULL != p_download_task->p_request_data)
            {
                SCI_FREE(p_download_task->p_request_data);
            }
            HTTP_CloseRequest(s_http_download_info.context_id);
            //查看下一个等待状态的任务。
            HttpCheckNextDownloadTask();
        }
    }
    else
    {
        HttpDownloadFail(task_id);
    }

    return MMI_RESULT_TRUE;
}

LOCAL MMI_RESULT_E HandleHttpGetCnf(DPARAM param)
{
    HTTP_GET_CNF_SIG_T signal = {0};
    int32 task_id = -1;
    uint32 i = 0;
    http_download_task_t* p_download_task = PNULL;

    if(PNULL == param)
    {
        SCI_TRACE_LOW("[HTTP]:param is PNULL");
        return MMI_RESULT_FALSE;
    }

    signal = *(HTTP_GET_CNF_SIG_T *)param;

    SCI_TRACE_LOW("[HTTP]:HandleHttpGetCnf,app_instance = %d",signal.app_instance);
    for(i = 0; i < MAX_SUPPORT_NUM; i++)
    {
        if (signal.app_instance == s_http_download_info.download_task[i].app_instance)
        {
            p_download_task = &s_http_download_info.download_task[i];
            task_id = i;
            break;
        }
    }

    if(MAX_SUPPORT_NUM <= i)
    {
        SCI_TRACE_LOW("[HTTP]:download_task is not compare");
        return MMI_RESULT_FALSE;
    }

    if (HTTP_SUCCESS == signal.result)
    {
        //如果是网址的重定向，需要根据新的网址重新发起下载。
        if(TRUE == p_download_task->is_need_redirect)
        {
            HttpGetRequest(p_download_task);
        }
        else
        {
            HttpDownload_SetStatus(task_id,HTTP_DOWNLOAD_DOWNLOADED);
            if(0 < s_http_download_info.download_task_num)
            {
                s_http_download_info.download_task_num--;
            }
            SCI_TRACE_LOW("[HTTP]:HandleHttpGetCnf, download_task_num = %d", s_http_download_info.download_task_num);
            if(0 < s_http_download_info.total_task_num)
            {
                s_http_download_info.total_task_num--;
            }
            SCI_TRACE_LOW("[HTTP]:HandleHttpGetCnf, total_task_num = %d", s_http_download_info.total_task_num);

            //下载成功，触发callback。
            if(PNULL != p_download_task->call_back)
            {
                http_callback_param_t callback_param = {0};
                callback_param.result = HTTP_REQUEST_SUCESS;
                callback_param.handle = p_download_task->app_instance;
                callback_param.p_body = (void*)p_download_task->p_receive_data;
                callback_param.body_size= p_download_task->receive_data_len;
                p_download_task->call_back(callback_param);
            }

            SCI_TRACE_LOW("[HTTP]:download complete,task_id is %d,download result is %d",i,signal.result);
            SCI_MEMSET(p_download_task,0,sizeof(http_download_task_t));

            if(PNULL != p_download_task->p_receive_data)
            {
                SCI_FREE(p_download_task->p_receive_data);
            }

            if(PNULL != p_download_task->p_request_data)
            {
                SCI_FREE(p_download_task->p_request_data);
            }
            HTTP_CloseRequest(s_http_download_info.context_id);
            //查看下一个等待状态的任务。
            HttpCheckNextDownloadTask();
        }
    }
    else
    {
        HttpDownloadFail(task_id);
    }

    return MMI_RESULT_TRUE;
}

LOCAL MMI_RESULT_E HandleHttpOptionsCnf(DPARAM param)
{
    HTTP_OPTIONS_CNF_SIG_T signal = {0};
    int32 task_id = -1;
    uint32 i = 0;
    http_download_task_t* p_download_task = PNULL;

    if(PNULL == param)
    {
        SCI_TRACE_LOW("[HTTP]:param is PNULL");
        return MMI_RESULT_FALSE;
    }

    signal = *(HTTP_OPTIONS_CNF_SIG_T *)param;

    SCI_TRACE_LOW("[HTTP]:HandleHttpOptionsCnf,app_instance = %d",signal.app_instance);
    for(i = 0; i < MAX_SUPPORT_NUM; i++)
    {
        if (signal.app_instance == s_http_download_info.download_task[i].app_instance)
        {
            p_download_task = &s_http_download_info.download_task[i];
            task_id = i;
            break;
        }
    }

    if(MAX_SUPPORT_NUM <= i)
    {
        SCI_TRACE_LOW("[HTTP]:download_task is not compare");
        return MMI_RESULT_FALSE;
    }

    if (HTTP_SUCCESS == signal.result)
    {
        //如果是网址的重定向，需要根据新的网址重新发起下载。
        if(TRUE == p_download_task->is_need_redirect)
        {
            HttpOptionsRequest(p_download_task);
        }
        else
        {
            HttpDownload_SetStatus(task_id,HTTP_DOWNLOAD_DOWNLOADED);
            if(0 < s_http_download_info.download_task_num)
            {
                s_http_download_info.download_task_num--;
            }
            SCI_TRACE_LOW("[HTTP]:HandleHttpOptionsCnf, download_task_num = %d", s_http_download_info.download_task_num);
            if(0 < s_http_download_info.total_task_num)
            {
                s_http_download_info.total_task_num--;
            }
            SCI_TRACE_LOW("[HTTP]:HandleHttpOptionsCnf, total_task_num = %d", s_http_download_info.total_task_num);

            //下载成功，触发callback。
            if(PNULL != p_download_task->call_back)
            {
                http_callback_param_t callback_param = {0};
                callback_param.result = HTTP_REQUEST_SUCESS;
                callback_param.handle = p_download_task->app_instance;
                callback_param.p_body = (void*)p_download_task->p_receive_data;
                callback_param.body_size= p_download_task->receive_data_len;
                p_download_task->call_back(callback_param);
            }

            SCI_TRACE_LOW("[HTTP]:download complete,task_id is %d,download result is %d",i,signal.result);
            SCI_MEMSET(p_download_task,0,sizeof(http_download_task_t));

            if(PNULL != p_download_task->p_receive_data)
            {
                SCI_FREE(p_download_task->p_receive_data);
            }

            if(PNULL != p_download_task->p_request_data)
            {
                SCI_FREE(p_download_task->p_request_data);
            }
            HTTP_CloseRequest(s_http_download_info.context_id);
            //查看下一个等待状态的任务。
            HttpCheckNextDownloadTask();
        }
    }
    else
    {
        HttpDownloadFail(task_id);
    }

    return MMI_RESULT_TRUE;
}

LOCAL MMI_RESULT_E HandleHttpHeaderInd(DPARAM param)
{
    HTTP_HEADER_IND_SIG_T signal = {0};
    http_download_task_t* p_download_task = PNULL;
    int32 task_id = -1;
    uint32 i = 0;

    if(PNULL == param)
    {
        SCI_TRACE_LOW("[HTTP]:param is PNULL");
        return MMI_RESULT_FALSE;
    }
    signal = *(HTTP_HEADER_IND_SIG_T *)param;

    SCI_TRACE_LOW("[HTTP]:response_code:%d",signal.rsp_header_info.response_code);
    SCI_TRACE_LOW("[HTTP]:app_instance = %d,header_ptr:%s",signal.app_instance,signal.header_ptr);
    for(i = 0; i < MAX_SUPPORT_NUM; i++)
    {
        if (signal.app_instance == s_http_download_info.download_task[i].app_instance)
        {
            task_id = i;
            break;
        }
    }
    if(MAX_SUPPORT_NUM <= i)
    {
        SCI_TRACE_LOW("[HTTP]:app_instance = %d,not compare",signal.app_instance);
        return MMI_RESULT_FALSE;
    }

    SCI_TRACE_LOW("[HTTP]:response_code = %d",signal.rsp_header_info.response_code);

    //如果header信息正确，保存mime_type的值，在data_ind中判断是否需要网址重定向。
    if(FALSE == ((HTTP_HEADER_SUCCESS_PARAM1 == signal.rsp_header_info.response_code)||(HTTP_HEADER_SUCCESS_PARAM2 == signal.rsp_header_info.response_code)))
    {
        HTTP_CloseRequest(s_http_download_info.context_id);
        if(0 > task_id)
        {
            SCI_TRACE_LOW("[HTTP]:HttpDownloadFail task_id error");
            return MMI_RESULT_FALSE;
        }

        SCI_TRACE_LOW("[HTTP]:download fail,task_id is %d",task_id);

        //下载失败，清空全局数据，并查看下一个正在等待状态的任务。
        if(0 < s_http_download_info.download_task_num)
        {
            s_http_download_info.download_task_num--;
        }
        SCI_TRACE_LOW("[HTTP]:HttpDownloadFail, download_task_num = %d", s_http_download_info.download_task_num);
        if(0 < s_http_download_info.total_task_num)
        {
            s_http_download_info.total_task_num--;
        }
        SCI_TRACE_LOW("[HTTP]:HttpDownloadFail, total_task_num = %d", s_http_download_info.total_task_num);

        HttpDownload_SetStatus(task_id,HTTP_DOWNLOAD_FAILED);
        p_download_task = &s_http_download_info.download_task[task_id];

        if(PNULL != p_download_task->call_back)
        {
            http_callback_param_t callback_param = {0};
            callback_param.result = HTTP_REQUEST_ERROR;
            callback_param.handle = p_download_task->app_instance;
            callback_param.p_body = (void*)signal.rsp_header_info.response_code;
            p_download_task->call_back(callback_param);
        }

        SCI_MEMSET(p_download_task,0,sizeof(http_download_task_t));
        if(PNULL != p_download_task->p_receive_data)
        {
            SCI_FREE(p_download_task->p_receive_data);
        }

        if(PNULL != p_download_task->p_request_data)
        {
            SCI_FREE(p_download_task->p_request_data);
        }

        HttpCheckNextDownloadTask();
    }
    else
    {
    #ifdef DL_SUPPORT
        s_http_download_info.download_task[i].mime_type = MMIDL_GetMimeTypeByhttpHeader(signal.header_ptr);
    #endif
        SCI_TRACE_LOW("[HTTP]:task_id is %d,mime_type is %d",i,s_http_download_info.download_task[i].mime_type);
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
        SCI_TRACE_LOW("[HTTP]:param is PNULL");
        return MMI_RESULT_FALSE;
    }

    signal = *(HTTP_ERROR_IND_SIG_T*)param;

    for(i = 0; i < MAX_SUPPORT_NUM; i++)
    {
        if (signal.app_instance == s_http_download_info.download_task[i].app_instance)
        {
            task_id = i;
            break;
        }
    }

    if(MAX_SUPPORT_NUM <= i)
    {
        SCI_TRACE_LOW("[HTTP]:app_instance = %d,not compare",signal.app_instance);
        return MMI_RESULT_FALSE;
    }

    HttpDownloadFail(task_id);
    return MMI_RESULT_TRUE;
}


LOCAL MMI_RESULT_E HandleHttpCloseCnf(DPARAM param)
{
    uint32              i = 0;
    HTTP_CLOSE_CNF_SIG_T signal = {0};

    if(PNULL == param)
    {
        SCI_TRACE_LOW("[HTTP]:param is PNULL");
        return MMI_RESULT_FALSE;
    }

    signal = *(HTTP_CLOSE_CNF_SIG_T *)param;

    if (signal.context_id == s_http_download_info.context_id)
    {
        s_http_download_info.context_id = 0;
        return MMI_RESULT_TRUE;
    }

    return MMI_RESULT_FALSE;
}


LOCAL boolean HttpGetAbsolutePath(uint8*   p_data, uint8* p_dest_str)
{
    uint8*  dest_str1 = PNULL;
    uint8*  dest_str2 = PNULL;
    uint16  dest_len = 0;
    boolean ret = FALSE;

    if(PNULL == p_data)
    {
        SCI_TRACE_LOW("[HTTP]:p_data is PNULL");
        return ret;
    }

    if(PNULL == p_dest_str)
    {
        SCI_TRACE_LOW("[HTTP]:p_dest_str is PNULL");
        return ret;
    }

    //网址重定向，获取新的网址。
    dest_str1 = (uint8 *)MMIAPICOM_Stristr((char *)p_data, HTTP_STRING_HTTP);
    if(PNULL == dest_str1)
    {
        dest_str1 = (uint8 *)MMIAPICOM_Stristr((char *)p_data, HTTP_STRING_HTTPS);
        if(PNULL == dest_str1)
        {
            SCI_TRACE_LOW("[HTTP]:dest_str1 is PNULL");
            return ret;
        }
    }
    dest_str2 = (uint8 *)MMIAPICOM_Stristr((char *)p_data, HTTP_STRING_ZIP);//TODO:公版重定向，HTTP_STRING_ZIP的值需要重新讨论定义。
    if(PNULL == dest_str2)
    {
        SCI_TRACE_LOW("[HTTP]:dest_str2 is PNULL");
        return ret;
    }

    dest_str2 += strlen(HTTP_STRING_ZIP);

    dest_len = dest_str2 - dest_str1;

    SCI_MEMCPY(p_dest_str,dest_str1,dest_len);
    SCI_TRACE_LOW("[HTTP]:dest_str1 is %s",dest_str1);
    SCI_TRACE_LOW("[HTTP]:dest_str2 is %s",dest_str2);
    SCI_TRACE_LOW("[HTTP]:p_dest_str is %s",p_dest_str);

    ret = TRUE;
    return ret;
}

LOCAL MMI_RESULT_E HandleHttpDataInd(DPARAM param)
{
    HTTP_DATA_IND_SIG_T signal = {0};
    uint32              bytes_writen  = 0;
    uint32              i = 0;
    http_download_task_t* p_download_task = PNULL;
    char temp_name[MAX_URL_LEN] = {0};

    if(PNULL == param)
    {
        SCI_TRACE_LOW("[HTTP]:param is PNULL");
        return MMI_RESULT_FALSE;
    }

    signal = *(HTTP_DATA_IND_SIG_T *)param;

    for(i = 0; i < MAX_SUPPORT_NUM; i++)
    {
        if (signal.app_instance == s_http_download_info.download_task[i].app_instance)
        {
            p_download_task = &s_http_download_info.download_task[i];
            SCI_TRACE_LOW("[HTTP]:download_task index is %d",i);
            break;
        }
    }
    if(MAX_SUPPORT_NUM <= i)
    {
        SCI_TRACE_LOW("[HTTP]:download_task is not compare");
        return MMI_RESULT_FALSE;
    }
    SCI_TRACE_LOW("[HTTP]:data_len=%d, data_ptr=%s, app_instance=%d",signal.data_len,signal.data_ptr,signal.app_instance);

    if(PNULL == signal.data_ptr)
    {
        SCI_TRACE_LOW("[HTTP]:signal.data_ptr is PNULL");
        return MMI_RESULT_FALSE;
    }

    //如果mime_type为HTML的类型，需要网址重定向，有时候服务器对mime_type会赋值错误，我们需要加入容错处理。
    if(MIME_TYPE_TEXT_HTML == p_download_task->mime_type)
    {
        uint8 dest_url[MAX_URL_LEN] = {0};

        if(TRUE == HttpGetAbsolutePath(signal.data_ptr,&dest_url))
        {
            SCI_MEMCPY(p_download_task->url,dest_url,sizeof(dest_url));
            p_download_task->is_need_redirect = TRUE;
        }
    }
    if(FALSE == p_download_task->is_need_redirect)
    {
        //收到的数据可能存在分包的情况，数据大小需要动态分配。
        //TODO:后续可以改进，如果APP传入filename，那么HTTP将数据直接保存在filename中。
        p_download_task->receive_data_len += signal.data_len;
        SCI_TRACE_LOW("[HTTP]:receive_data_len is %d",p_download_task->receive_data_len);
        if(PNULL == p_download_task->p_receive_data)
        {
            p_download_task->p_receive_data = SCI_ALLOC_APPZ(p_download_task->receive_data_len+1);
            if(PNULL != p_download_task->p_receive_data)
            {
                SCI_MEMCPY(p_download_task->p_receive_data,signal.data_ptr,p_download_task->receive_data_len);
            }
            else
            {
                SCI_TRACE_LOW("[HTTP]:download_task->p_receive_data alloc fail");
                return MMI_RESULT_TRUE;
            }
        }
        else
        {
            char* p_new_receive_data = PNULL;

            p_new_receive_data = SCI_ALLOC_APPZ(p_download_task->receive_data_len+1);
            if(PNULL != p_new_receive_data)
            {
                SCI_MEMSET(p_new_receive_data, '\0', p_download_task->receive_data_len + 1);//cid609623
                SCI_MEMCPY(p_new_receive_data,p_download_task->p_receive_data,strlen(p_download_task->p_receive_data));
                SCI_MEMCPY(p_new_receive_data+strlen(p_download_task->p_receive_data),signal.data_ptr,signal.data_len);
                SCI_FREE(p_download_task->p_receive_data);
                p_download_task->p_receive_data = SCI_ALLOC_APPZ(p_download_task->receive_data_len+1);
                if(PNULL != p_download_task->p_receive_data)
                {
                    SCI_MEMCPY(p_download_task->p_receive_data,p_new_receive_data,p_download_task->receive_data_len);
                }
                else
                {
                    SCI_TRACE_LOW("[HTTP]:download_task->p_receive_data alloc fail");
                    SCI_FREE(p_new_receive_data);
                    return MMI_RESULT_TRUE;
                }
                SCI_FREE(p_new_receive_data);
            }
            else
            {
                SCI_TRACE_LOW("[HTTP]:p_new_receive_data alloc fail");
                return MMI_RESULT_TRUE;
            }
        }
    }
    return MMI_RESULT_TRUE;
}

LOCAL MMI_RESULT_E HandleHttpInitCnf(DPARAM param)
{
    HTTP_INIT_CNF_SIG_T signal = {0};
    uint32              task_id = 0;

    if(PNULL == param)
    {
        SCI_TRACE_LOW("[HTTP]:param is PNULL");
        return MMI_RESULT_FALSE;
    }
    signal = *(HTTP_INIT_CNF_SIG_T *)param;
    task_id = signal.app_param;
    SCI_TRACE_LOW("[HTTP]:task_id = %d",task_id);
    if(signal.module_id != HTTP_MODULE_ID)
    {
        SCI_TRACE_LOW("[HTTP]:module_id = %d",signal.module_id);
        return MMI_RESULT_FALSE;
    }

    s_http_download_info.context_id   = signal.context_id;
    if(s_http_download_info.download_task[task_id].request_type == HTTP_POST_REQUEST)
    {
        HttpPostRequest(&s_http_download_info.download_task[task_id]);
    }
    else if(s_http_download_info.download_task[task_id].request_type == HTTP_GET_REQUEST)
    {
        HttpGetRequest(&s_http_download_info.download_task[task_id]);
    }
    else if(s_http_download_info.download_task[task_id].request_type == HTTP_OPTIONS_REQUEST)
    {
        HttpOptionsRequest(&s_http_download_info.download_task[task_id]);
    }

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : 获取未使用的task_id
//  Parameter: None
//  Author: qi.zhang
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
    SCI_TRACE_LOW("[HTTP]:unused_id is %d",unused_id);
    return unused_id;
}

/*****************************************************************************/
//  Description : 判断是否是相同的task，防止重复下载。
//  Parameter: None
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
LOCAL boolean IsSameDownloadTask(char* p_url, uint32 request_id, uint32* p_same_task_index)
{
    uint32 i = 0;
    BOOLEAN is_same_download_task = FALSE;

    if (PNULL == p_url)
    {
        SCI_TRACE_LOW("[HTTP]:p_url is PNULL");
        return is_same_download_task;
    }

    if (PNULL == p_same_task_index)
    {
        SCI_TRACE_LOW("[HTTP]:p_same_task_index is PNULL");
        return is_same_download_task;
    }

    for (i = 0; i < MAX_SUPPORT_NUM; i++)
    {
        if ((0 == strcmp(p_url, s_http_download_info.download_task[i].url))&&(request_id == s_http_download_info.download_task[i].app_instance))
        {
            *p_same_task_index = i;
            is_same_download_task = TRUE;
            break;
        }
    }

    SCI_TRACE_LOW("[HTTP]:same task index is %d,is_same_download_task is %d",i,is_same_download_task);

    return is_same_download_task;
}

LOCAL MMI_RESULT_E HandleHttpMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
    //当result为true时，后续注册的app不需要再响应http的消息
    MMI_RESULT_E result = MMI_RESULT_FALSE;

    SCI_TRACE_LOW("[HTTP]:msg_id=0x%x ",msg_id);
    switch (msg_id)
    {
        case HTTP_SIG_INIT_CNF:
        {
            //第一次发送的消息比较特殊
            //http协议服务已经准备好，开始连接服务器
            //初始化要发送的数据,get or post
            SCI_TRACE_LOW("[HTTP]:receive HTTP_SIG_INIT_CNF");
            result = HandleHttpInitCnf(param);
        }
        break;
        case HTTP_SIG_REQUEST_ID_IND:
        {
            //第一次请求后，http分配给app的id
            SCI_TRACE_LOW("[HTTP]:receive HTTP_SIG_REQUEST_ID_IND");
        }
        break;
        case HTTP_SIG_HEADER_IND:
        {
            SCI_TRACE_LOW("[HTTP]:receive HTTP_SIG_HEADER_IND");
            result = HandleHttpHeaderInd(param);
        }
        break;
        case HTTP_SIG_ERROR_IND:
        {
            SCI_TRACE_LOW("[HTTP]:receive HTTP_SIG_ERROR_IND");
            result = HandleHttpErrorInd(param);
        }
        break;
        case HTTP_SIG_DATA_IND:
        {
            SCI_TRACE_LOW("[HTTP]:receive HTTP_SIG_DATA_IND");
            result=HandleHttpDataInd(param);
        }
        break;
        case HTTP_SIG_GET_CNF:
        {
            SCI_TRACE_LOW("[HTTP]:receive HTTP_SIG_GET_CNF");
            result = HandleHttpGetCnf(param);
        }
        break;
        case HTTP_SIG_POST_CNF:
        {
            SCI_TRACE_LOW("[HTTP]:receive HTTP_SIG_POST_CNF");
            result = HandleHttpPostCnf(param);
        }
        break;
        case HTTP_SIG_OPTIONS_CNF:
        {
            SCI_TRACE_LOW("[HTTP]:receive HTTP_SIG_OPTIONS_CNF");
            result = HandleHttpOptionsCnf(param);
        }
        case HTTP_SIG_CLOSE_CNF://HTTP_SIG_CLOSE_REQ消息的回复,告知app,http服务关闭
        {
            SCI_TRACE_LOW("[HTTP]:receive HTTP_SIG_CLOSE_CNF");
            result = HandleHttpCloseCnf(param);
        }
        break;
     default:
        break;
    }
    return result;
}

LOCAL void HandleHttpPdpMsg(MMIPDP_CNF_INFO_T*  p_msg_ptr)
{
    if(PNULL == p_msg_ptr)
    {
        SCI_TRACE_LOW("[HTTP]:p_msg_ptr is PNULL");
        return;
    }
    SCI_TRACE_LOW("[HTTP]:msg_id =%d,result=%d",p_msg_ptr->msg_id,p_msg_ptr->result);
    switch(p_msg_ptr->msg_id)
    {
        case MMIPDP_ACTIVE_CNF:
        {
            if(MMIPDP_RESULT_SUCC == p_msg_ptr->result)
            {
                uint8 i = 0;

                SCI_TRACE_LOW("[HTTP]:msg_ptr->nsapi=%d ",p_msg_ptr->nsapi);
                s_http_download_info.net_id = p_msg_ptr->nsapi;
                s_http_download_info.is_pdp_actived = TRUE;

                HttpInitRequest(s_http_download_info.net_id);
            }
            else
            {
                MMIAPIPDP_Deactive(HTTP_MODULE_ID);
            }
        }
        break;
        case MMIPDP_DEACTIVE_CNF:
        {
            SCI_TRACE_LOW("[HTTP]:receive MMIPDP_DEACTIVE_CNF");
        }
        break;
        case MMIPDP_DEACTIVE_IND:
        {
            MMIAPIPDP_Deactive(HTTP_MODULE_ID);
        }
        break;
        default:
            break;
    }
}

LOCAL boolean HttpActivePdp(void)
{
    uint8 setting_index = 0;
    BOOLEAN return_val = FALSE;
    MMIPDP_ACTIVE_INFO_T active_info = {0};
    char* p_apn_str = PNULL;
    MMICONNECTION_LINKSETTING_DETAIL_T* p_linksetting = PNULL;
    MMICONNECTION_APNTYPEINDEX_T* p_apntype_index = PNULL;

    p_apntype_index = MMIAPICONNECTION_GetApnTypeIndex(MN_DUAL_SYS_1);//TODO:simcard需要通过接口获取
    if (NULL != p_apntype_index)
    {
        SCI_TRACE_LOW("[HTTP]:is_valid=%d, internet_idx:%d\n", p_apntype_index->internet_index[MN_DUAL_SYS_1].is_valid, p_apntype_index->internet_index[MN_DUAL_SYS_1].index);
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
        SCI_TRACE_LOW("[HTTP]:p_linksetting is NULL");
    }
    if(PNULL != p_apn_str)
    {
        SCI_TRACE_LOW("[HTTP]:apn=%s,auth_type=%d,ip_type=%d",p_apn_str,p_linksetting->auth_type,p_linksetting->ip_type);
        active_info.app_handler         = HTTP_MODULE_ID;
        active_info.dual_sys            = MN_DUAL_SYS_1;//TODO:get simcard
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
        if(TRUE == MMIAPIPDP_Active(&active_info))
        {
            return_val = TRUE;
        }
        SCI_TRACE_LOW("[HTTP]:return_val = %d,moduleid=0x%x",return_val,active_info.app_handler);
    }
    else
    {
        SCI_TRACE_LOW("[HTTP]:p_apn_str is NULL");
    }
    return return_val;
}

/*****************************************************************************/
//  Description : http start request
//  Parameter: [In] request_id:different app instance
//             [In] p_url:url to be download
//             [In] p_content:download data
//             [In] http_callback:callback
//             [Return] None
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
//TOTO:后续需要将p_url和p_content整合为一个结构体，并添加get/post参数和file_fullpath参数
PUBLIC http_request_result_e http_start_request(uint32 request_id, char* p_url, char* p_content, http_request_type_e request_type,http_register_callback http_callback)
{
    int32 task_id = -1;
    BOOLEAN is_same_download_task = 0;
    uint32 same_task_index = 0;
    http_download_status_e download_status = HTTP_DOWNLOAD_NONE;
    char* p_request_data = PNULL;

    SCI_TRACE_LOW("[HTTP]:enter http_start_request");

    if(PNULL == p_url)
    {
        SCI_TRACE_LOW("[HTTP]:p_url is null");
        return HTTP_REQUEST_PARAM_ERROR;
    }

    //1.判断是否相同的task，并且正在下载。
    is_same_download_task = IsSameDownloadTask(p_url, request_id, &same_task_index);
    if (TRUE == is_same_download_task)
    {
        HttpDownload_GetStatus(same_task_index,&download_status);

        if(HTTP_DOWNLOAD_DOWNLOADING == download_status)
        {
            SCI_TRACE_LOW("[HTTP]:is_same_dest_url");
            return HTTP_REQUEST_PARAM_ERROR;
        }
    }

    if(MAX_SUPPORT_NUM <= s_http_download_info.total_task_num)
    {
        SCI_TRACE_LOW("[HTTP]:total_task_num = %d,exceed max number",s_http_download_info.total_task_num);
        return HTTP_REQUEST_ERROR;
    }
    s_http_download_info.total_task_num++;

    //2.获取空闲的task_id。
    task_id = GetUnusedDownloadTaskId();

    if(0 > task_id)
    {
        SCI_TRACE_LOW("[HTTP]:task_id error");
        return HTTP_REQUEST_PARAM_ERROR;
    }

    //3.保存content和url的数据。
    if(PNULL != p_content)
    {
        p_request_data = SCI_ALLOC_APPZ(strlen(p_content)+1);
        if(PNULL != p_request_data)
        {
            SCI_MEMCPY(p_request_data,p_content,strlen(p_content));
            s_http_download_info.download_task[task_id].p_request_data = p_request_data;
            SCI_TRACE_LOW("[HTTP]:request content is %s",p_request_data);
        }
        else
        {
            SCI_TRACE_LOW("[HTTP]:p_request_data alloc fail");
            return HTTP_REQUEST_ERROR;
        }
    }
    s_http_download_info.download_task[task_id].request_type = request_type;
    SCI_MEMCPY(s_http_download_info.download_task[task_id].url,p_url,strlen(p_url));
    SCI_TRACE_LOW("[HTTP]:url is %s",p_url);
    s_http_download_info.download_task[task_id].is_used = TRUE;
    s_http_download_info.download_task[task_id].app_instance = request_id;
    if(PNULL != http_callback)
    {
        s_http_download_info.download_task[task_id].call_back = http_callback;
    }

    //4.设置下载状态并开始下载。
    if (MAX_DOWLOADING_NUM <= s_http_download_info.download_task_num)
    {
        SCI_TRACE_LOW("[HTTP]:download_task_num = %d,exceed multi_max num",s_http_download_info.download_task_num);
        HttpDownload_SetStatus(task_id,HTTP_DOWNLOAD_WAITING);
        s_http_download_info.download_task_num++;
    }
    else
    {
        if(TRUE == s_http_download_info.is_pdp_actived)
        {
            s_http_download_info.download_task_num++;
            HttpDownload_SetStatus(task_id,HTTP_DOWNLOAD_DOWNLOADING);
            HttpInitRequest(task_id);
        }
        else
#ifdef UAL_NETWORKMGR_SUPPORT
        {
            ual_networkmgr_status_t  networkmgr_info = {0};
            uint8 i = 0;

            ual_networkmgr_get_net_info_by_route(UAL_NETWORKMGR_ROUTE_DEFAULT, &networkmgr_info);
            SCI_TRACE_LOW("[HTTP]:net_id is %d",networkmgr_info.net_id);

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
                SCI_TRACE_LOW("[HTTP]:pdp not active");
                return HTTP_REQUEST_ERROR;
            }

        }
#else
        {
            HttpActivePdp();
        }
#endif
    }
    return HTTP_REQUEST_SUCESS;
}

/*****************************************************************************/
//  Description : http module init
//  Parameter: None
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
PUBLIC void  http_init_module(void)
{
    SCI_TRACE_LOW("[HTTP]:init module");
    g_http_app.ProcessMsg = HandleHttpMsg;
}
/*****************************************************************************/
//  Description : http set content type
//  Parameter: p_content_type:content type
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
PUBLIC void http_set_content_type(char* p_content_type)
{
    uint32 wf_len = 0;
    if(PNULL == p_content_type)
    {
        SCI_TRACE_LOW("[http_set_content_type]:content_type is pnull.");
        return;
    }
    wf_len = SCI_STRLEN(p_content_type);
    if(0 == wf_len || wf_len > HTTP_CONTENT_TYPE_LEN)
    {
        SCI_TRACE_LOW("[http_set_content_type]:content_type length error");
        return;
    }
    SCI_MEMSET(s_content_type, 0, HTTP_CONTENT_TYPE_LEN + 1);
    SCI_MEMCPY(s_content_type, p_content_type, wf_len);
    SCI_TRACE_LOW("[http_set_content_type]:s_content_type is %s.",s_content_type);
}
#endif
