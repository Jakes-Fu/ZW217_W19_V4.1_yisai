/*****************************************************************************
** File Name:      mmisfr_ctcc_http.c
** Author:
** Copyright (C) 2000-2100, UNISOC Technoloies Co.,Ltd.
** Description:    Reg by http
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                     NAME                         DESCRIPTION
** 2020/11/24        mark.zhang1            Create
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "os_api.h"
#include "mmi_module.h"
#include "mmk_app.h"
#include "mmk_timer.h"
#include "sci_types.h"
#include "version.h"
#include "in_message.h"
#include "mmiphone_export.h"
#include "mmisfr_ctcc_http.h"
#include "mmisfr_ctcc_main.h"
#include "mmisfr_ctcc_json.h"
#include "mmisfr_ctcc_pdp.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
/* Timer for posting data failed */
#define REG_RETRY_GENERAL_INTERVAL (60*60*1000) //milliseconds - 1 hour

/* Timer for update */
#if 0 // for test
#define RESEND_FIRST_INTERVAL (5*60*1000) //milliseconds - 5mins
#define RESEND_GENERAL_INTERVAL (5*60*1000) //milliseconds - 5mins
#else
#define RESEND_FIRST_INTERVAL (12*60*60*1000) //milliseconds - half day
#define RESEND_GENERAL_INTERVAL (30* 24*60*60*1000) //milliseconds - 30 days
#endif

/* App param when http init */
#define SFR_HTTP_APPFLAG (10)

/* copy data, param -> signal */
#define HTTP_SIGNAL_ANALYTIC_EX(param,signal,http_context_ptr,sig_type) \
{\
    get_signal_struct(param, &signal, sizeof(sig_type));\
    http_context_ptr->context_id   = signal.context_id;\
    http_context_ptr->app_instance = 0;\
    http_context_ptr->request_id = 0;\
    http_context_ptr->module_id    = signal.module_id;\
}

/**-------------------------------------------------------------------------*
**                         STATIC DEFINITION                       *
**--------------------------------------------------------------------------*/
LOCAL SFR_HTTP_CONTEXT s_sfr_http_context = {0};

LOCAL uint8 s_retry_when_failed;

LOCAL BOOLEAN s_is_fisrt = TRUE;

LOCAL BOOLEAN s_data_right = FALSE;

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
LOCAL void get_signal_struct(DPARAM param, void *signal_struct_ptr,
                             uint16 struct_size)
{
    if ((PNULL != param) && (PNULL != signal_struct_ptr)
            && (struct_size >= sizeof(xSignalHeaderRec)))
    {
        SCI_MEMCPY((uint8 *)((uint32)signal_struct_ptr), param, (struct_size));
    }
}

LOCAL void handle_timer_event(uint8  timer_id, uint32 param)
{
    SCI_TRACE_LOW("[DMCTCC] handle_timer_event...");
    MMK_StopTimer(timer_id);
    sfr_set_allow_pdp_flag(TRUE);
    MMISFR_CTCC_HandleNetworkStatus();
}

LOCAL void sfr_retry_register(void)
{
    SCI_TRACE_LOW("[DMCTCC] sfr_retry_register...");
    sfr_deactive_app();
    MMK_CreateTimerCallback(REG_RETRY_GENERAL_INTERVAL,
                            handle_timer_event, NULL,
                            FALSE);
}

LOCAL void sfr_resend(BOOLEAN first_update)
{
    SCI_TRACE_LOW("[DMCTCC] sfr_resend, first_update=%d", first_update);
    sfr_deactive_app();
    if (first_update)
    {
        MMK_CreateTimerCallback(RESEND_FIRST_INTERVAL, handle_timer_event,
                                NULL, FALSE);
    }
    else
    {
        MMK_CreateTimerCallback(RESEND_GENERAL_INTERVAL, handle_timer_event,
                                NULL, FALSE);
    }
}

LOCAL void handle_next_loop(void)
{
    HTTP_CloseRequest(s_sfr_http_context.context_id);
    sfr_resend(s_is_fisrt);
    s_is_fisrt = FALSE; //set when first coming
    s_retry_when_failed = 0; //RESET
}

LOCAL void handle_failed_event(void) //retry when failed
{
    s_retry_when_failed++;
    if (s_retry_when_failed <= 10)
    {
        HTTP_CloseRequest(s_sfr_http_context.context_id);
        sfr_retry_register();
    }
    else
    {
        handle_next_loop();
    }
}

LOCAL char *sfr_get_accept(void)
{
    return "text/html, application/json, */*";
}

LOCAL char *sfr_get_agent(void)
{
    return MMIAPICOM_GetUserAgent();
}

LOCAL char *sfr_get_bodytype(void)
{
    return "application/encrypted-json";
}

LOCAL char *sfr_get_url(void)
{
    return "http://zzhc.vnet.cn:9998/";
}

LOCAL void sfr_post_request(void)
{
    uint32 len = 0;
    HTTP_POST_PARAM_T *post_param = NULL;

    s_data_right = FALSE;
    post_param = (HTTP_POST_PARAM_T *)SCI_ALLOC_APPZ(sizeof(HTTP_POST_PARAM_T));
    if (post_param != NULL)
    {
        post_param->uri.uri_ptr = sfr_get_url();
        post_param->accept.accept_ptr = sfr_get_accept();
        post_param->connection = HTTP_CONNECTION_KEEP_ALIVE;
        post_param->user_agent.user_agent_ptr = sfr_get_agent();
        post_param->get_data.is_header_use_file = FALSE; // use buffer
        //post_param->get_data.style = HTTP_DATA_TRANS_STYLE_UNKNOW;

        post_param->post_body.is_use_file = FALSE;
        post_param->post_body.body_type_ptr = sfr_get_bodytype();
        post_param->post_body.u.post_buffer.buffer_ptr = sfr_get_post_data(&len);
        post_param->post_body.u.post_buffer.buffer_len = len;
        post_param->post_body.u.post_buffer.is_copied_by_http = TRUE;

        post_param->need_net_prog_ind = FALSE;
        post_param->is_use_post_after_redirect = TRUE;

        SCI_TraceLow("[DMCTCC] sfr_post_request, buffer_len (%d)", len);
        HTTP_PostRequest(s_sfr_http_context.context_id, post_param,
                         MMI_MODULE_SFR_CTCC);

        SCI_FREE(post_param->post_body.u.post_buffer.buffer_ptr);
        post_param->post_body.u.post_buffer.buffer_ptr = NULL;
        SCI_FREE(post_param);
        post_param = NULL;
    }
}

LOCAL MMI_RESULT_E handle_init_cnf(SFR_HTTP_CONTEXT *http_context_ptr,
                                   DPARAM param)
{
    HTTP_INIT_CNF_SIG_T *cnf = (HTTP_INIT_CNF_SIG_T *)param;
    HTTP_INIT_CNF_SIG_T signal = {0};

    if (cnf->module_id != MMI_MODULE_SFR_CTCC
            || cnf->app_param != SFR_HTTP_APPFLAG)
    {
        SCI_TraceLow("[DMCTCC] InitCnf return");
        return MMI_RESULT_FALSE;
    }

    HTTP_SIGNAL_ANALYTIC_EX(param, signal, http_context_ptr, HTTP_INIT_CNF_SIG_T);
    if (HTTP_SUCCESS == signal.result)
    {
        SCI_TraceLow("[DMCTCC] InitCnf <context_id = 0x%x> ",
                     http_context_ptr->context_id);
        sfr_post_request();
    }
    else
    {
        SCI_TraceLow("[DMCTCC] InitCnf <error = %d> ", signal.result);
    }

    return MMI_RESULT_TRUE;
}

LOCAL MMI_RESULT_E handle_request_id_ind(SFR_HTTP_CONTEXT *http_context_ptr,
        DPARAM param)
{
    HTTP_REQUEST_ID_IND_SIG_T *cnf = (HTTP_REQUEST_ID_IND_SIG_T *)param;
    HTTP_REQUEST_ID_IND_SIG_T signal = {0};

    if (cnf->app_instance != MMI_MODULE_SFR_CTCC)
    {
        SCI_TraceLow("[DMCTCC] RequestIdInd return");
        return MMI_RESULT_FALSE;
    }

    get_signal_struct(param, &signal, sizeof(HTTP_REQUEST_ID_IND_SIG_T));
    http_context_ptr->request_id = signal.request_id;
    SCI_TraceLow("[DMCTCC] RequestIdInd <param request_id = %d>, <param context_id = 0x%x>",
                 signal.request_id, signal.context_id);

    return MMI_RESULT_TRUE;
}

LOCAL MMI_RESULT_E handle_header_ind(SFR_HTTP_CONTEXT *http_context_ptr,
                                     DPARAM param)
{
    HTTP_HEADER_IND_SIG_T signal = {0};
	uint16 rsp_code = 0;
    get_signal_struct(param, &signal, sizeof(HTTP_HEADER_IND_SIG_T));
    rsp_code = signal.rsp_header_info.response_code;
    SCI_TraceLow("[DMCTCC]  header_ptr :%s", signal.header_ptr);
    SCI_TraceLow("[DMCTCC]  rsp_header_info.charset_ptr :%s",
                 signal.rsp_header_info.charset_ptr);
    SCI_TraceLow("[DMCTCC]  rsp_header_info.content_encoding_type :%d",
                 signal.rsp_header_info.content_encoding_type);
    SCI_TraceLow("[DMCTCC]  rsp_header_info.content_length :%d",
                 signal.rsp_header_info.content_length);
    SCI_TraceLow("[DMCTCC]  rsp_header_info.content_type_ptr :%s",
                 signal.rsp_header_info.content_type_ptr);
    SCI_TraceLow("[DMCTCC]  rsp_header_info.response_code :%d",
                 rsp_code);

    if (http_context_ptr->context_id != signal.context_id
            || http_context_ptr->request_id != signal.request_id)
    {
        SCI_TraceLow("[DMCTCC] HeaderInd, request_id or context_id error!");
        return MMI_RESULT_FALSE;
    }

    if (HTTP_RESPONSE_CODE_OK == rsp_code
            || HTTP_RESPONSE_CODE_PARTIAL_CONTENT == rsp_code)
    {
        HTTP_HeaderResponse(http_context_ptr->context_id,
                            http_context_ptr->app_instance,
                            http_context_ptr->request_id, HTTP_DATA_TRANS_STYLE_BUFFER, PNULL, 0);
    }

    return MMI_RESULT_TRUE;
}

LOCAL MMI_RESULT_E handle_data_ind(SFR_HTTP_CONTEXT *http_context_ptr,
                                   DPARAM param)
{
    HTTP_DATA_IND_SIG_T *cnf = (HTTP_DATA_IND_SIG_T *)param;

    SCI_TraceLow("[DMCTCC] handle_data_ind, context_id :0x%x, request_id : %d",
                 cnf->context_id, cnf->request_id);
    if (cnf->context_id != http_context_ptr->context_id
            ||  cnf->request_id != http_context_ptr->request_id)
    {
        SCI_TraceLow("[DMCTCC] handle_data_ind RETURN");
        return MMI_RESULT_FALSE;
    }

    SCI_TraceLow("[DMCTCC] response_code :%d", cnf->response_code);
    SCI_TraceLow("[DMCTCC] cnf->data_ptr : [%s]", cnf->data_ptr);
    if (cnf->response_code == 200)
    {
        s_data_right = sfr_parse_data(cnf->data_ptr, cnf->data_len);
    }
    return MMI_RESULT_TRUE;
}

LOCAL MMI_RESULT_E handle_post_cnf(SFR_HTTP_CONTEXT *http_context_ptr,
                                   DPARAM param)
{
    HTTP_POST_CNF_SIG_T *cnf = (HTTP_POST_CNF_SIG_T *)param;

    if (cnf->context_id != http_context_ptr->context_id
            ||  cnf->request_id != http_context_ptr->request_id)
    {
        SCI_TraceLow("[DMCTCC] PostCnf return");
        return MMI_RESULT_FALSE;
    }

    SCI_TraceLow("[DMCTCC] PostCnf, result :%d, s_data_right:%d",
                 cnf->result, s_data_right);
    if (HTTP_SUCCESS == cnf->result && s_data_right)
    {
        sfr_save_iccid(MN_DUAL_SYS_1);
        handle_next_loop();
    }
    else
    {
        handle_failed_event();
    }

    return MMI_RESULT_TRUE;
}

LOCAL MMI_RESULT_E handle_error_ind(SFR_HTTP_CONTEXT *http_context_ptr,
                                    DPARAM param)
{
    HTTP_ERROR_IND_SIG_T *cnf = (HTTP_ERROR_IND_SIG_T *)param;

    SCI_TraceLow("[DMCTCC] ErrorInd, context_id : %d,  error :%d",
                 cnf->context_id, cnf->result);

    if (cnf->app_instance != MMI_MODULE_SFR_CTCC)
    {
        SCI_TraceLow("[DMCTCC] ErrorInd return");
        return MMI_RESULT_FALSE;
    }
    handle_failed_event();

    return MMI_RESULT_TRUE;
}

PUBLIC MMI_RESULT_E sfr_handle_http_msg(PWND app_ptr, uint16 msg_id,
                                        DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;
    SFR_HTTP_CONTEXT *http_context_ptr = &s_sfr_http_context;

    SCI_TraceLow("[DMCTCC] sfr_handle_http_msg : msg-id = %d",
                 (msg_id - HTTP_SIGNAL_BEG));
    switch (msg_id)
    {
        case HTTP_SIG_INIT_CNF:
            SCI_TraceLow("[DMCTCC] http : HTTP_SIG_INIT_CNF");
            result = handle_init_cnf(http_context_ptr, param);
            break;

        case HTTP_SIG_REQUEST_ID_IND:
            SCI_TraceLow("[DMCTCC] http : HTTP_SIG_REQUEST_ID_IND");
            result = handle_request_id_ind(http_context_ptr, param);
            break;

        case HTTP_SIG_HEADER_IND:
            SCI_TraceLow("[DMCTCC] http : HTTP_SIG_HEADER_IND");
            result = handle_header_ind(http_context_ptr, param);
            break;

        case HTTP_SIG_DATA_IND:
            SCI_TraceLow("[DMCTCC] http : HTTP_SIG_DATA_IND");
            result = handle_data_ind(http_context_ptr, param);
            break;

        case HTTP_SIG_POST_CNF:
            SCI_TraceLow("[DMCTCC] http : HTTP_SIG_POST_CNF");
            result = handle_post_cnf(http_context_ptr, param);
            break;
        case HTTP_SIG_CLOSE_CNF:
            SCI_TraceLow("[DMCTCC] http : HTTP_SIG_CLOSE_CNF");
            break;

        case HTTP_SIG_ERROR_IND:
            SCI_TraceLow("[DMCTCC] http : HTTP_SIG_ERROR_IND");
            result = handle_error_ind(http_context_ptr, param);
            break;

        default:
            break;
    }

    return result;
}

PUBLIC void sfr_reg_by_http(uint32 net_id)
{
    HTTP_INIT_PARAM_T init_param = {0};

    init_param.is_cookie_enable = FALSE;
    init_param.is_cache_enable = FALSE;
    init_param.net_id = net_id;

    SCI_TraceLow("[DMCTCC] sfr_reg_by_http, net_id=%d", net_id);
    if (HTTP_SUCCESS != HTTP_InitRequest(MMI_MODULE_SFR_CTCC, SFR_HTTP_APPFLAG,
                                         &init_param))
    {
        SCI_TraceLow("[DMCTCC] sfr_reg_by_http, INIT Error!");
        handle_failed_event();
    }
}

PUBLIC void sfr_notify_deactive_ind()
{
    SCI_TraceLow("[DMCTCC] sfr_notify_deactive_ind......");
    //Reserved
}

