/****************************************************************************
** File Name:      http_test.c                                              *                                                *
** Date:           2020/09/03                                              *
** Copyright:      2020 uinsoc, Incorporated. All Rights Reserved.         *
** Description:                                                            *
****************************************************************************/
/**------------------------------------------------------------------------*
 **                         Include Files                                  *
 **------------------------------------------------------------------------*/
#include "socket_api.h"
#include "socket_types.h"
#include "ssl_api.h"
#include "in_message.h"
#include "http_test.h"
#include "sci_api.h"
#include "http_api.h"

/** 
  *   This is Appliaction level samle code of HTTP. 
  *     
  *   Module level:
  *       Application 
  *             |
  *          HTTP
  *             |
  *       TCP (socket)
  *             |
  *            IP
  *             |
  *           LTE
  *
  *   Function flow:
  *       Step1: Name - PDP actived
  *                 Function - get client IP/DNS.
  *       Step2: Name - HTTP Init Request
  *                 Function - HTTP_InitRequest
  *       Step3: Name - HTTP Get Request
  *                 Function - HTTP_GetRequest
  *       Step4: Name - Get HTTP_SIG_GET_CNF close http request
  *                 Function - HTTP_CloseRequest
  *       Step5: Name - PDP deactived
  *                 Function - release some net resources.
*/

/**-------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

#define MAX_URL_LEN                 (200)
#define MAX_HTTP_PACK_LEN           (4096)
/**
 * This is for thread. 
 *  P_HTTPTEST_IO_TASK_PRIORITY is for Temp.
*/
#define P_HTTPTEST_IO_STACK_SIZE     (1024*16)
#define P_HTTPTEST_IO_QUEUE_NUM      15    
#define P_HTTPTEST_IO_TASK_PRIORITY  231  // PRI_DAPS_SNTP_IO_TASK //231

#define ACCEPT_STRING           "*/*"
#define ACCEPT_LANGUAGE_STRING  "zh-cn, en"
#define USER_AGENT_STRING       "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.0)"
#define ACCEPT_CHARSET_STRING   "utf-8, utf-16, iso-8859-1, iso-8859-5, iso-10646-ucs-2, GB2312, windows-1252, us-ascii"
#define HTTP_CONTENT_TYPE "application/json"
#define HTTP_DELETE_DATA "1111"
//#define HTTP_TEST_IVP6 1

/**-------------------------------------------------------------------------*
**                         STRUCTURE DEFINITION                             *
**--------------------------------------------------------------------------*/
//the message body define begin
typedef struct HTTP_TEST_SIG
{
    _SIGNAL_VARS
}HTTP_TEST_SIG_T;

typedef struct _HTTP_TEST_CONTEXT_T_
{
    HTTP_APP_MODULE_ID_T module_id;
    HTTP_CONTEXT_ID_T    context_id;
    HTTP_REQUEST_ID_T    request_id;
    HTTP_APP_INSTANCE_T  app_instance; //用于区分不同的HTTP请求。同时只有一个HTTP请求时可以为任意值。
    uint8 method;
    uint32 break_start;
    uint32 break_end;

}HTTP_TEST_CONTEXT_T;


/**-------------------------------------------------------------------------*
**                         STATIC VARIABLE DEFINITION                       *
**--------------------------------------------------------------------------*/
LOCAL BLOCK_ID g_http_test_task_id = SCI_INVALID_BLOCK_ID;
LOCAL HTTP_TEST_CONTEXT_T s_http_context = {0};
LOCAL char *g_url_ptr = NULL;
LOCAL uint8 g_ssl_version = 0;
LOCAL uint8 g_test_result = 0;
LOCAL uint8 g_is_test_finished = 0;
LOCAL uint8 g_http_atcmd_link_id = 0;
LOCAL uint32 g_net_id = 0;
LOCAL HTTPTESTPARAM_T g_httptestparam={0};

/**-------------------------------------------------------------------------*
**                         LOCAL FUNCTION DECLARE                           *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description :Create  a Task to handle SSL/SOCKET events 
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/

/**-------------------------------------------------------------------------*
**                         IMPLEMENTATION                                   *
**--------------------------------------------------------------------------*/

LOCAL HTTP_TEST_CONTEXT_T* GetHttpContextPtr(void)
{
    return &s_http_context;
}

LOCAL void GetSignalStruct(xSignalHeaderRec* param, void *signal_struct_ptr, uint16 struct_size)
{
    if((PNULL != param)&&(PNULL != signal_struct_ptr)&&(struct_size >= sizeof(xSignalHeaderRec)))
    {
        SCI_MEMCPY((uint8 *)((uint32)signal_struct_ptr), param, (struct_size));
    }
}

LOCAL int http_test_HandleHttpInitCnf(HTTP_TEST_CONTEXT_T *http_context_ptr, xSignalHeaderRec* param)
{
    int result = -1;
    HTTP_INIT_CNF_SIG_T signal = {0};
    HTTP_GET_PARAM_T*   get_param_ptr = PNULL;
    HTTP_DELETE_PARAM_T*   delete_param_ptr = PNULL;
    HTTP_HEAD_PARAM_T*   head_param_ptr = PNULL;
    HTTP_APP_PARAM_T app_param = 0;
    int len =0;
    const char* s_http = "http://";
    const char* s_https = "https://";
    char contentRange[32];
    char begin_downLoad[16];
    char end_downLoad[16];

    if(PNULL == http_context_ptr)
    {
        return FALSE;
    }

    SCI_TRACE_LOW("[http test][%s %d] Enter\n", __FUNCTION__, __LINE__);

    //---1--- 
    GetSignalStruct(param, &signal, sizeof(HTTP_INIT_CNF_SIG_T));
    app_param = signal.app_param;   //可以传app需要的参数过来。暂未用到。

    http_context_ptr->context_id   = signal.context_id;
    http_context_ptr->app_instance = 0;
    http_context_ptr->module_id    = signal.module_id;
    SCI_TRACE_LOW("[http test][%s %d] signal.context_id=%x,method=%d\n", __FUNCTION__, __LINE__, signal.context_id,http_context_ptr->method);

    //---2---
    if (0 != http_context_ptr->module_id)
    {
        SCI_TRACE_LOW("[http test]HandleHttpInitCnf---Not http test---");
        return FALSE;
    }

    //---3---
    switch(http_context_ptr->method)
    {
    case HTTPTEST_METHOD_GET:
    {
        get_param_ptr = SCI_ALLOCA(sizeof(HTTP_GET_PARAM_T));

        if(PNULL == get_param_ptr) //lint !e774
        {
            SCI_TRACE_LOW("[http test][%s %d] malloc fail\n", __FUNCTION__, __LINE__);
            return FALSE;
        }
        SCI_MEMSET(get_param_ptr, 0x00, sizeof(HTTP_GET_PARAM_T));

        //accept
        len = strlen(ACCEPT_STRING);
        get_param_ptr->accept.accept_ptr = SCI_ALLOC_APPZ(len+1);
        strcpy(get_param_ptr->accept.accept_ptr,ACCEPT_STRING);
        //accept language
        len = strlen(ACCEPT_LANGUAGE_STRING);
        get_param_ptr->accept_language.accept_language_ptr = SCI_ALLOC_APPZ(len+1);
        strcpy(get_param_ptr->accept_language.accept_language_ptr,ACCEPT_LANGUAGE_STRING);
        //UA
        len = strlen(USER_AGENT_STRING);
        get_param_ptr->user_agent.user_agent_ptr = SCI_ALLOC_APPZ(len+1);
        strcpy(get_param_ptr->user_agent.user_agent_ptr,USER_AGENT_STRING);
        
        len = strlen(ACCEPT_CHARSET_STRING);
        get_param_ptr->accept_charset.accept_charset_ptr = SCI_ALLOC_APPZ(len + 1);
        strcpy(get_param_ptr->accept_charset.accept_charset_ptr,ACCEPT_CHARSET_STRING);

        //URI
        get_param_ptr->uri.uri_ptr = g_url_ptr;

        SCI_TRACE_LOW("[http test %s %d]uri_ptr:%s\n", __FUNCTION__, __LINE__, get_param_ptr->uri.uri_ptr);
        if (strncasecmp(get_param_ptr->uri.uri_ptr, s_http, strlen(s_http)) ==0 || strncasecmp(get_param_ptr->uri.uri_ptr, s_https, strlen(s_https)) == 0)
        {
            //accept charset

            //buffer store;
            get_param_ptr->get_data.is_header_use_file = FALSE;
            get_param_ptr->get_data.style = HTTP_DATA_TRANS_STYLE_BUFFER;

            //Connection
            get_param_ptr->connection = HTTP_CONNECTION_KEEP_ALIVE;
            get_param_ptr->need_net_prog_ind = FALSE;
            if(http_context_ptr->break_start >=0 && http_context_ptr->break_end >0)
            {

                itoa(http_context_ptr->break_start, begin_downLoad, 10);

                itoa(http_context_ptr->break_end, end_downLoad, 10);

                strcpy(contentRange, "bytes=");

                strcat(contentRange, begin_downLoad);

                strcat(contentRange, "-");

                strcat(contentRange, end_downLoad);
                get_param_ptr->extern_header.header_num = 1;
                get_param_ptr->extern_header.other_header_ptr = SCI_ALLOC(sizeof(HTTP_OTHER_HEADER_T));
                memset(get_param_ptr->extern_header.other_header_ptr,0,sizeof(HTTP_OTHER_HEADER_T));
                len = strlen("Range");
                get_param_ptr->extern_header.other_header_ptr[0].header_name_ptr = SCI_ALLOC(len+1);
                memset(get_param_ptr->extern_header.other_header_ptr[0].header_name_ptr,0,len + 1);
                strcpy(get_param_ptr->extern_header.other_header_ptr[0].header_name_ptr,"Range");
                len = strlen(contentRange);
                get_param_ptr->extern_header.other_header_ptr[0].header_value_ptr = SCI_ALLOC(len+1);
                memset(get_param_ptr->extern_header.other_header_ptr[0].header_value_ptr,0,len + 1);
                strcpy(get_param_ptr->extern_header.other_header_ptr[0].header_value_ptr,contentRange);
            }
            
            //---5---
            result = HTTP_GetRequest(http_context_ptr->context_id, get_param_ptr, http_context_ptr->app_instance);
        }
        else
        {
            result = -1;
        }
        //---6---
        SCI_FREE(get_param_ptr->accept.accept_ptr);
        SCI_FREE(get_param_ptr->accept_charset.accept_charset_ptr);
        SCI_FREE(get_param_ptr->accept_language.accept_language_ptr);
        SCI_FREE(get_param_ptr->user_agent.user_agent_ptr);
        SCI_FREE(get_param_ptr);
    }
    break;
    case HTTPTEST_METHOD_DELETE:
    {
        delete_param_ptr = SCI_ALLOCA(sizeof(HTTP_DELETE_PARAM_T));

        if(PNULL == delete_param_ptr) //lint !e774
        {
            SCI_TRACE_LOW("[http test][%s %d] malloc fail\n", __FUNCTION__, __LINE__);
            return FALSE;
        }
        SCI_MEMSET(delete_param_ptr, 0x00, sizeof(HTTP_DELETE_PARAM_T));

        //accept
        len = strlen(ACCEPT_STRING);
        delete_param_ptr->accept.accept_ptr = SCI_ALLOC_APPZ(len+1);
        strcpy(delete_param_ptr->accept.accept_ptr,ACCEPT_STRING);
        //accept language
        len = strlen(ACCEPT_LANGUAGE_STRING);
        delete_param_ptr->accept_language.accept_language_ptr = SCI_ALLOC_APPZ(len+1);
        strcpy(delete_param_ptr->accept_language.accept_language_ptr,ACCEPT_LANGUAGE_STRING);
        //UA
        len = strlen(USER_AGENT_STRING);
        delete_param_ptr->user_agent.user_agent_ptr = SCI_ALLOC_APPZ(len+1);
        strcpy(delete_param_ptr->user_agent.user_agent_ptr,USER_AGENT_STRING);
                //accept charset
        len = strlen(ACCEPT_CHARSET_STRING);
        delete_param_ptr->accept_charset.accept_charset_ptr = SCI_ALLOC_APPZ(len + 1);
        strcpy(delete_param_ptr->accept_charset.accept_charset_ptr,ACCEPT_CHARSET_STRING);

        //URI
        delete_param_ptr->uri.uri_ptr = g_url_ptr;

        SCI_TRACE_LOW("[http test %s %d]uri_ptr:%s\n", __FUNCTION__, __LINE__, delete_param_ptr->uri.uri_ptr);
        if (strncasecmp(delete_param_ptr->uri.uri_ptr, s_http, strlen(s_http)) ==0 || strncasecmp(delete_param_ptr->uri.uri_ptr, s_https, strlen(s_https)) == 0)
        {

            //buffer store;
            delete_param_ptr->get_data.is_header_use_file = FALSE;
            delete_param_ptr->get_data.style = HTTP_DATA_TRANS_STYLE_BUFFER;

            //Connection
            delete_param_ptr->connection = HTTP_CONNECTION_KEEP_ALIVE;
            delete_param_ptr->need_net_prog_ind = FALSE;
            delete_param_ptr->is_need_check_protocol = TRUE;
            //---5---
            delete_param_ptr->delete_body.is_use_file = FALSE;
            
            len = strlen(HTTP_CONTENT_TYPE);
            delete_param_ptr->delete_body.body_type_ptr = SCI_ALLOC_APPZ(len+1);
            strcpy(delete_param_ptr->delete_body.body_type_ptr,HTTP_CONTENT_TYPE);
            
            len = strlen(HTTP_DELETE_DATA);
            delete_param_ptr->delete_body.u.delete_buffer.buffer_ptr = SCI_ALLOC_APPZ(len+1);
            strcpy(delete_param_ptr->delete_body.u.delete_buffer.buffer_ptr,HTTP_DELETE_DATA);
            delete_param_ptr->delete_body.u.delete_buffer.buffer_len= len;
             delete_param_ptr->delete_body.u.delete_buffer.is_copied_by_http = TRUE;
            
            result = HTTP_DeleteRequest(http_context_ptr->context_id, delete_param_ptr, http_context_ptr->app_instance);
        }
        else
        {
            result = -1;
        }
        //---6---
        SCI_FREE(delete_param_ptr->accept.accept_ptr);
        SCI_FREE(delete_param_ptr->accept_charset.accept_charset_ptr);
        SCI_FREE(delete_param_ptr->accept_language.accept_language_ptr);
        SCI_FREE(delete_param_ptr->user_agent.user_agent_ptr);
        if(delete_param_ptr->delete_body.body_type_ptr!= NULL)
        {
            SCI_FREE(delete_param_ptr->delete_body.body_type_ptr);
        }
        if(delete_param_ptr->delete_body.u.delete_buffer.buffer_ptr =  NULL)
        {
            SCI_FREE(delete_param_ptr->delete_body.u.delete_buffer.buffer_ptr);
        }
        SCI_FREE(delete_param_ptr);
    }
    break;
    case HTTPTEST_METHOD_HEAD:
    {
        head_param_ptr = SCI_ALLOCA(sizeof(HTTP_HEAD_PARAM_T));

        if(PNULL == head_param_ptr) //lint !e774
        {
            SCI_TRACE_LOW("[http test][%s %d] malloc fail\n", __FUNCTION__, __LINE__);
            return FALSE;
        }
        SCI_MEMSET(head_param_ptr, 0x00, sizeof(HTTP_HEAD_PARAM_T));

        //accept
        len = strlen(ACCEPT_STRING);
        head_param_ptr->accept.accept_ptr = SCI_ALLOC_APPZ(len+1);
        strcpy(head_param_ptr->accept.accept_ptr,ACCEPT_STRING);
        //accept language
        len = strlen(ACCEPT_LANGUAGE_STRING);
        head_param_ptr->accept_language.accept_language_ptr = SCI_ALLOC_APPZ(len+1);
        strcpy(head_param_ptr->accept_language.accept_language_ptr,ACCEPT_LANGUAGE_STRING);
        //UA
        len = strlen(USER_AGENT_STRING);
        head_param_ptr->user_agent.user_agent_ptr = SCI_ALLOC_APPZ(len+1);
        strcpy(head_param_ptr->user_agent.user_agent_ptr,USER_AGENT_STRING);
        //accept charset
        len = strlen(ACCEPT_CHARSET_STRING);
        head_param_ptr->accept_charset.accept_charset_ptr = SCI_ALLOC_APPZ(len + 1);
        strcpy(head_param_ptr->accept_charset.accept_charset_ptr,ACCEPT_CHARSET_STRING);

        //URI
        head_param_ptr->uri.uri_ptr = g_url_ptr;

        SCI_TRACE_LOW("[http test %s %d]uri_ptr:%s\n", __FUNCTION__, __LINE__, head_param_ptr->uri.uri_ptr);
        if (strncasecmp(head_param_ptr->uri.uri_ptr, s_http, strlen(s_http)) ==0 || strncasecmp(head_param_ptr->uri.uri_ptr, s_https, strlen(s_https)) == 0)
        {


            //Connection
            head_param_ptr->connection = HTTP_CONNECTION_KEEP_ALIVE;
            head_param_ptr->need_net_prog_ind = FALSE;
            head_param_ptr->is_need_check_protocol = TRUE;
            head_param_ptr->head_trans_style.is_use_file = FALSE; 
            //---5---
            result = HTTP_HeadRequest(http_context_ptr->context_id, head_param_ptr, http_context_ptr->app_instance);
        }
        else
        {
            result = -1;
        }
        //---6---
        SCI_FREE(head_param_ptr->accept.accept_ptr);
        SCI_FREE(head_param_ptr->accept_charset.accept_charset_ptr);
        SCI_FREE(head_param_ptr->accept_language.accept_language_ptr);
        SCI_FREE(head_param_ptr->user_agent.user_agent_ptr);
        SCI_FREE(head_param_ptr);
    }
    break;
    default:
        break;
    }
    SCI_TRACE_LOW("[http test][%s %d] result=%d\n", __FUNCTION__, __LINE__, result);
    return result;
}
/*****************************************************************************/
//  Description : Task entry function
//                                                  
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
LOCAL void http_test_entry( uint32 argc, void* argv )
{
    int result = 0;
    xSignalHeaderRec *sig_ptr = PNULL;
    HTTP_INIT_PARAM_T *init_param_ptr = PNULL;
    HTTP_TEST_CONTEXT_T *http_context_ptr = NULL;
    char rsp[20] ={0};

    init_param_ptr = SCI_ALLOCA(sizeof(HTTP_INIT_PARAM_T));
    if (PNULL == init_param_ptr) //lint !e774
    {
        SCI_TRACE_LOW("[http test debug malloc fail]%s %d\n",__FUNCTION__,__LINE__);
        return ;
    }

    //Step2: Name - DNS parser
    SCI_TRACE_LOW("[http test debug Step2: HTTP_InitRequest]%s %d\n",__FUNCTION__,__LINE__);

    SCI_MEMSET(init_param_ptr, 0, sizeof(HTTP_INIT_PARAM_T));
    init_param_ptr->is_cookie_enable = FALSE;
    init_param_ptr->is_cache_enable = FALSE;
    init_param_ptr->net_id = (uint32)g_net_id;
    HTTP_InitRequest(0, g_http_test_task_id, init_param_ptr);
    SCI_FREE(init_param_ptr);
    init_param_ptr = PNULL;

    while (g_http_test_task_id != SCI_INVALID_BLOCK_ID)/*lint !e716*/
    {
        // Get sig From Queue
        sig_ptr = SCI_GetSignal( SCI_IdentifyThread());   
        if (sig_ptr == PNULL)
        {
            break;
        }
        http_context_ptr = GetHttpContextPtr();
        if(PNULL != http_context_ptr)
        {
                http_context_ptr->method =  g_httptestparam.method;
                http_context_ptr->break_start = g_httptestparam.break_start;
                http_context_ptr->break_end = g_httptestparam.break_end;
        }

        SCI_TRACE_LOW("[http test %s %d]sig_ptr->SignalCode is %x,method=%d\n", __FUNCTION__, __LINE__, sig_ptr->SignalCode,http_context_ptr->method);
        // Process sig
        switch (sig_ptr->SignalCode)
        {
            case HTTP_SIG_INIT_CNF:
                SCI_TRACE_LOW("[http test]HandleHttpMsg receive HTTP_SIG_INIT_CNF");
                result = http_test_HandleHttpInitCnf(http_context_ptr, sig_ptr);
                if (result != 0)
                {
                    HTTP_CloseRequest(http_context_ptr->context_id);
                    SCI_TRACE_LOW("[http test]HandleHttpMsg don't send HTTP_SIG_GET_REQ");
                }
                break;

            case HTTP_SIG_NEED_AUTH_IND:
                SCI_TRACE_LOW("[http test]HandleHttpMsg receive HTTP_SIG_NEED_AUTH_IND");
                break;

            case HTTP_SIG_REQUEST_ID_IND:
                SCI_TRACE_LOW("[http test]HandleHttpMsg receive HTTP_SIG_REQUEST_ID_IND");
                break;

            case HTTP_SIG_GET_CNF:
                {
                    HTTP_GET_CNF_SIG_T *cnf = (HTTP_GET_CNF_SIG_T*)sig_ptr;
                    SCI_TRACE_LOW("[http test]HandleHttpMsg receive HTTP_SIG_GET_CNF");
                    if(cnf->context_id ==http_context_ptr->context_id )
                    {
                        if(cnf->result == HTTP_SUCCESS)
                        {
                            g_test_result = 1;
                            snprintf( rsp ,20, "HTTP %d\r\n",cnf->rsp_header_info.response_code);
                            #ifdef PPP_USB
                            SIO_ATC_WriteCmdRes((uint8*)rsp, strlen((char*)rsp),ATC_GetSioPort());
                            #else
                            SIO_ATC_WriteCmdRes((uint8*)rsp, strlen((char*)rsp));
                            #endif
                        }
                        else
                        {
                            g_test_result = 0;
                        }
                    }
                    HTTP_CloseRequest(http_context_ptr->context_id);
                }
                break;
             case HTTP_SIG_DELETE_CNF:
                {
                    HTTP_DELETE_CNF_SIG_T *cnf = (HTTP_DELETE_CNF_SIG_T*)sig_ptr;
                    SCI_TRACE_LOW("[http test]HandleHttpMsg receive HTTP_SIG_DELETE_CNF");
                    if(cnf->context_id ==http_context_ptr->context_id )
                    {
                        if(cnf->result == HTTP_SUCCESS)
                        {
                            g_test_result = 1;
                            snprintf( rsp ,20, "HTTP %d\r\n",cnf->rsp_header_info.response_code);
                            #ifdef PPP_USB
                            SIO_ATC_WriteCmdRes((uint8*)rsp, strlen((char*)rsp),ATC_GetSioPort());
                            #else
                            SIO_ATC_WriteCmdRes((uint8*)rsp, strlen((char*)rsp) );
                            #endif
                        }
                        else
                        {
                            g_test_result = 0;
                        }
                    }
                    HTTP_CloseRequest(http_context_ptr->context_id);
                }
                break;

            case HTTP_SIG_HEAD_CNF:
                {
                    HTTP_HEAD_CNF_SIG_T *cnf = (HTTP_HEAD_CNF_SIG_T*)sig_ptr;
                    SCI_TRACE_LOW("[http test]HandleHttpMsg receive HTTP_SIG_HEAD_CNF");
                    if(cnf->context_id ==http_context_ptr->context_id )
                    {
                        if(cnf->result == HTTP_SUCCESS)
                        {
                            g_test_result = 1;
                            snprintf( rsp ,20, "HTTP %d\r\n",cnf->rsp_header_info.response_code);
                            #ifdef PPP_USB
                            SIO_ATC_WriteCmdRes((uint8*)rsp, strlen((char*)rsp),ATC_GetSioPort());
                            #else
                            SIO_ATC_WriteCmdRes((uint8*)rsp, strlen((char*)rsp) );
                            #endif
                        }
                        else
                        {
                            g_test_result = 0;
                        }
                    }
                    HTTP_CloseRequest(http_context_ptr->context_id);
                }               
            break;

            case HTTP_SIG_DATA_IND:
                SCI_TRACE_LOW("[http test]HandleHttpMsg receive HTTP_SIG_DATA_IND");
                break;

            case HTTP_SIG_HEADER_IND:
                SCI_TRACE_LOW("[http test]HandleHttpMsg receive HTTP_SIG_HEADER_IND");
                break;

            case HTTP_SIG_ERROR_IND:
                SCI_TRACE_LOW("[http test]HandleHttpMsg receive HTTP_SIG_ERROR_IND");
                HTTP_CloseRequest(http_context_ptr->context_id);
                break;

            case HTTP_SIG_CLOSE_CNF:
                SCI_TRACE_LOW("[http test]HandleHttpMsg receive HTTP_SIG_CLOSE_CNF");
                g_http_test_task_id = SCI_INVALID_BLOCK_ID;
                break;

            case HTTP_SIG_CANCEL_CNF:
                SCI_TRACE_LOW("[http test]HandleHttpMsg receive HTTP_SIG_CANCEL_CNF");
                break;

            case HTTP_SIG_NET_PROG_IND:
                SCI_TRACE_LOW("[http test]HandleHttpMsg receive HTTP_SIG_NET_PROG_IND");
                break;

            case HTTP_SIG_REDIRECT_IND:
                SCI_TRACE_LOW("[http test]HandleHttpMsg receive HTTP_SIG_REDIRECT_IND");
                break;

            case HTTP_SIG_POST_CNF:
                SCI_TRACE_LOW("[http test]HandleHttpMsg receive HTTP_SIG_POST_CNF");
                break;

            case HTTP_SIG_SSL_CERT_UNTRUST_IND:
                SCI_TRACE_LOW("[http test]HandleHttpMsg receive HTTP_SIG_SSL_CERT_UNTRUST_IND");
                break;

            default:
                SCI_TRACE_LOW("[http test]HandleHttpMsg Not handle msg_id = %x", sig_ptr->SignalCode);
                break;
        } 
        if (sig_ptr != PNULL)
        {
            SCI_FREE(sig_ptr);
            sig_ptr = PNULL;
        }
    } // while(1)

    if (g_test_result)
    {
        snprintf( rsp ,20, "HTTP test OK!");
    }
    else
    {
        snprintf( rsp ,20, "HTTP test Fail!");
    }
    //ATC_BuildInfoRsp(&s_config_info, rsp);
    #ifdef PPP_USB
    SIO_ATC_WriteCmdRes((uint8*)rsp, strlen((char*)rsp),ATC_GetSioPort());
    #else
    SIO_ATC_WriteCmdRes((uint8*)rsp, strlen((char*)rsp) );
    #endif
    g_http_test_task_id = SCI_INVALID_BLOCK_ID;
    g_is_test_finished = 1;
    g_http_atcmd_link_id = 0;
    if (g_url_ptr != NULL)
    {
        SCI_FREE(g_url_ptr);
        g_url_ptr = NULL;
    }   

    SCI_TRACE_LOW("[http test debug Exit test thread!!!]%s %d\n",__FUNCTION__,__LINE__);    
    SCI_ThreadExit();
}

/*****************************************************************************/
//  Description : Task function
//                                                  
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
LOCAL void http_test_StartTask(uint8 method,uint32 break_start, uint32 break_end)
{
    if (SCI_INVALID_BLOCK_ID != g_http_test_task_id)
    {
        SCI_TRACE_LOW("[http test debug task has already started]%s    %d\n",__FUNCTION__,__LINE__);
        return;
    }
    g_httptestparam.method = method;
        SCI_TRACE_LOW("[http test debug Step2: HTTP_InitRequest]%s %d,method=%d\n\n",__FUNCTION__,__LINE__,method);

    g_httptestparam.break_start = break_start;
    g_httptestparam.break_end = break_end;
    g_http_test_task_id = SCI_CreateThread(
        "T_P_httptest",
        "Q_P_httptest",
        http_test_entry,
        0,
        NULL,
        P_HTTPTEST_IO_STACK_SIZE,
        P_HTTPTEST_IO_QUEUE_NUM,
        P_HTTPTEST_IO_TASK_PRIORITY,
        SCI_PREEMPT,
        SCI_AUTO_START );


    return;
}

/*****************************************************************************/
//  Description :  set net id
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
PUBLIC void http_test_setnetid(uint32 netid)
{
    g_net_id = netid;
}

/*****************************************************************************/
//  Description :  set atcmd link id
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
PUBLIC void http_test_setLinkId(uint8 link_id)
{
    g_http_atcmd_link_id = link_id;
}
/*****************************************************************************/
//  Description :  main function
//           This API must called in MMI thread.(May be User press some key)
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
PUBLIC int http_test_main(uint8 method,uint8 *param,uint32 break_start, uint32 break_end)
{
    uint8 len = 0;

    if (param == NULL)
    {
        return -1;
    }
    if (SCI_INVALID_BLOCK_ID != g_http_test_task_id)
    {
        SCI_TRACE_LOW("[http test debug task has already started]%s    %d\n",__FUNCTION__,__LINE__);
        return -1;
    }
    SCI_TRACE_LOW("[http test]%s %d, param:%s\n",__FUNCTION__,__LINE__, param);

    //MMI has many app module, we just use MMI_MODULE_MMIBROWSER for test,
    //Note, maybe we must create a new app.
    g_test_result = 0;
    g_is_test_finished = 0;

    g_url_ptr = SCI_ALLOC_APPZ(MAX_URL_LEN);
    if (g_url_ptr == NULL)
    {
        SCI_TRACE_LOW("[http test debug malloc fail]\n");
        return FALSE;
    }
    
    len = strlen(param);
    if (len >= MAX_URL_LEN)
    {
        len = MAX_URL_LEN - 1;
    }
    memcpy(g_url_ptr, param, len);
    SCI_TRACE_LOW("[http test]%s , %s,method=%d\n",g_url_ptr, param,method);
    http_test_StartTask(method,break_start,break_end);
    return 1;
}

/*****************************************************************************/
//  Description :  exit function
//           This API must called in MMI thread.(May be User press some key)
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
PUBLIC int http_test_exit(void)
{
    if (g_http_test_task_id != SCI_INVALID_BLOCK_ID)
    {
        HTTP_TEST_SIG_T *send_sig = PNULL;
        
        send_sig = malloc(sizeof(HTTP_TEST_SIG_T));
        if (send_sig == NULL)
        {    
            SCI_TRACE_LOW("[http test debug malloc fail]%s %d\n",__FUNCTION__,__LINE__);
            return 0;
        }
        SCI_MEMSET(send_sig, 0x00,sizeof(HTTP_TEST_SIG_T));
        
        //make signal header
        send_sig->Sender = SCI_IdentifyThread();
        send_sig->SignalCode = HTTP_SIG_ERROR_IND;
        send_sig->SignalSize = sizeof(HTTP_TEST_SIG_T);
        SCI_TRACE_LOW("[http test callback debug  dispatch message_id %d]%s %d\n",HTTP_SIG_ERROR_IND, __FUNCTION__,__LINE__);
        SCI_SendSignal((xSignalHeader)(send_sig), g_http_test_task_id);        
        SCI_Sleep(1000);
    }

    if (g_url_ptr != NULL)
    {
        SCI_FREE(g_url_ptr);
        g_url_ptr = NULL;
    }

    return 0;
}

PUBLIC int http_test_get_result(char *at_rsp)
{
    int timeout = 20;

    if (at_rsp == NULL)
    {
        return -1;
    }

    while ((g_is_test_finished == 0) && timeout--) //wait http_test_entry exit
    {
        SCI_Sleep(1000);
    }
    if (g_test_result)
    {
        snprintf(at_rsp, MAX_RESULT_BUFFER_LEN, "%s", "HTTP test OK!");
    }
    else
    {
        snprintf(at_rsp, MAX_RESULT_BUFFER_LEN, "%s", "HTTP test Fail!");
    }
    http_test_exit();
    SCI_TRACE_LOW("[http test debug]%s %d\n",__FUNCTION__,__LINE__);

    return 0;
}
