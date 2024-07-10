#include "std_header.h"
#include "os_api.h"
#include "sci_types.h"
#include "mmk_app.h"
#include "mmiphone_export.h"
#include "mmi_filemgr.h"
#include "http_api.h"
#include "version.h"
#include "socket_api.h"
#include "mmipdp_export.h"
#include "in_message.h"
#include "mmieng_internal.h"
#include "mmisms_app.h"
#include "mmieng_nv.h"
#include "mmiconnection_export.h"
#include "mmipdp_internal.h"
#include "mmi_module.h"
#include "dal_time.h"
#include "mmk_authen.h"
#include "mmisms_set.h"
#include "mmi_nv.h"
#include "JSON_parser.h"

#include "sfr_carrier_manager.h"
#include "sfr_header.h"

#ifdef SFR_SUPPORT_CUCC
#define RETRY_SEC (3600)
#elif defined (SFR_SUPPORT_CTCC_SMS)
#define RETRY_SEC (3600)
#else
#define RETRY_SEC (20)
#endif

#define HTTP_SIGNAL_ANALYTIC_EX(param,signal,app_param,http_context_ptr,app_module_id,sig_type) \
do{\
    SlfReg_GetSignalStruct(param, &signal, sizeof(sig_type));\
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

#define HTTP_SIGNAL_ANALYTIC(param,signal,http_context_ptr,sig_type) \
do{\
    SlfReg_GetSignalStruct(param, &signal, sizeof(sig_type));\
    if(signal.context_id != http_context_ptr->context_id)\
    {\
        return MMI_RESULT_FALSE;\
    }\
}while(0)\

LOCAL void SlfReg_GetSignalStruct(DPARAM param, void *signal_struct_ptr, uint16 struct_size)
{
    if((PNULL != param)&&(PNULL != signal_struct_ptr)&&(struct_size >= sizeof(xSignalHeaderRec)))
    {
        SCI_MEMCPY((uint8 *)((uint32)signal_struct_ptr), param, (struct_size ));
    }
}

LOCAL MMI_RESULT_E _HandleHttpSigInitCnf (SFR_HTTP_CONTEXT * http_context_ptr, DPARAM param);
LOCAL MMI_RESULT_E _HandleHttpSigRequestIdInd (SFR_HTTP_CONTEXT *http_context_ptr, DPARAM param);
LOCAL MMI_RESULT_E _HandleHttpSigHeaderInd (SFR_HTTP_CONTEXT *http_context_ptr, DPARAM param);
LOCAL MMI_RESULT_E _HandleHttpSigDataInd (SFR_HTTP_CONTEXT *http_context_ptr, DPARAM param);
LOCAL MMI_RESULT_E _HandleHttpSigPostCnf (SFR_HTTP_CONTEXT *http_context_ptr, DPARAM param);
LOCAL MMI_RESULT_E _HandleHttpSigCloseCnf (SFR_HTTP_CONTEXT *http_context_ptr, DPARAM param);
LOCAL MMI_RESULT_E _HandleHttpSigErrorInd (SFR_HTTP_CONTEXT *http_context_ptr, DPARAM param);

PUBLIC MMI_RESULT_E SlfReg_HandleHttpMsg(PWND app_ptr, uint16 msg_id, DPARAM param, SFR_HTTP_CONTEXT *http_context_ptr)
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;

    switch(msg_id)
    {
        case HTTP_SIG_INIT_CNF:
            SCI_TraceLow("[Self Reg] http : HTTP_SIG_INIT_CNF");
            result = _HandleHttpSigInitCnf(http_context_ptr, param);
            break;
        case HTTP_SIG_REQUEST_ID_IND:
            SCI_TraceLow("[Self Reg] http : HTTP_SIG_REQUEST_ID_IND");
            result = _HandleHttpSigRequestIdInd(http_context_ptr, param);
            break;
        case HTTP_SIG_ERROR_IND:
            SCI_TraceLow("[Self Reg] http : HTTP_SIG_ERROR_IND");
            result = _HandleHttpSigErrorInd(http_context_ptr, param);
            break;
        case HTTP_SIG_HEADER_IND:
            SCI_TraceLow("[Self Reg] http : HTTP_SIG_HEADER_IND");
            result = _HandleHttpSigHeaderInd(http_context_ptr, param);
            break;
        case HTTP_SIG_DATA_IND:
            SCI_TraceLow("[Self Reg] http : HTTP_SIG_DATA_IND");
            result = _HandleHttpSigDataInd(http_context_ptr, param);
            break;
        case HTTP_SIG_POST_CNF:
            SCI_TraceLow("[Self Reg] http : HTTP_SIG_POST_CNF");
            result = _HandleHttpSigPostCnf(http_context_ptr, param);
            break;
        case HTTP_SIG_CLOSE_CNF:
            SCI_TraceLow("[Self Reg] http : HTTP_SIG_CLOSE_CNF");
            result = _HandleHttpSigCloseCnf(http_context_ptr, param);
            break;
        default:
            SCI_TraceLow("[Self Reg] http : msg-id = %d", msg_id);
            break;
    }
    return result;
}

LOCAL MMI_RESULT_E _HandleHttpSigInitCnf (SFR_HTTP_CONTEXT * http_context_ptr, DPARAM param)
{
    HTTP_INIT_CNF_SIG_T * cnf = (HTTP_INIT_CNF_SIG_T*)param;
    HTTP_INIT_CNF_SIG_T signal = {0};
    HTTP_APP_PARAM_T app_param = 0;

    if(cnf->module_id != MMI_MODULE_SFR || SFR_HTTP_APPFLAG != cnf->app_param || http_context_ptr == NULL)
    {
        return MMI_RESULT_FALSE;
    }

    HTTP_SIGNAL_ANALYTIC_EX(param, signal, app_param, http_context_ptr, MMI_MODULE_SFR, HTTP_INIT_CNF_SIG_T);

    if (signal.result != HTTP_SUCCESS)
    {
        SCI_TraceLow("[Self Reg] _HandleHttpSigInitCnf <error = %d> ", signal.result);
        return MMI_RESULT_FALSE;
    }

    SCI_TraceLow("[Self Reg] _HandleHttpSigInitCnf <context_id = 0x%x> ", http_context_ptr->context_id);

    g_sfr_linker.http_return= FALSE;
    //Send Self register package
    SlfReg_PostPackageRequest();

    return MMI_RESULT_TRUE;
}

LOCAL MMI_RESULT_E _HandleHttpSigRequestIdInd (SFR_HTTP_CONTEXT *http_context_ptr, DPARAM param)
{
    HTTP_REQUEST_ID_IND_SIG_T signal = {0};
    HTTP_REQUEST_ID_IND_SIG_T *cnf = (HTTP_REQUEST_ID_IND_SIG_T*)param;

    HTTP_SIGNAL_ANALYTIC(param,signal,http_context_ptr,HTTP_REQUEST_ID_IND_SIG_T);

    SCI_TraceLow("[Self Reg] _HandleHttpSigRequestIdInd signal.app_instance:%x (MMI_MODULE_SFR = %x)",signal.app_instance, MMI_MODULE_SFR);
    if (MMI_MODULE_SFR == signal.app_instance)
    {
        http_context_ptr->request_id = signal.request_id;
        SCI_TraceLow("[Self Reg] _HandleHttpSigRequestIdInd request_id:%ld,http_context_ptr->context_id:%x",signal.request_id,http_context_ptr->context_id);

        return MMI_RESULT_TRUE;
    }

    return MMI_RESULT_FALSE;
}

LOCAL MMI_RESULT_E _HandleHttpSigHeaderInd (SFR_HTTP_CONTEXT *http_context_ptr, DPARAM param)
{
    HTTP_HEADER_IND_SIG_T signal = {0};
    HTTP_HEADER_IND_SIG_T *cnf = (HTTP_HEADER_IND_SIG_T*)param;

    HTTP_SIGNAL_ANALYTIC(param, signal, http_context_ptr, HTTP_HEADER_IND_SIG_T);

    SCI_TraceLow("[Self Reg] _HandleHttpSigHeaderInd .");
    SCI_TraceLow("[Self Reg]  header_ptr :%s", signal.header_ptr);
    SCI_TraceLow("[Self Reg]  rsp_header_info.charset_ptr :%s", signal.rsp_header_info.charset_ptr);
    SCI_TraceLow("[Self Reg]  rsp_header_info.content_encoding_type :%d", signal.rsp_header_info.content_encoding_type);
    SCI_TraceLow("[Self Reg]  rsp_header_info.content_length :%d", signal.rsp_header_info.content_length);
    SCI_TraceLow("[Self Reg]  rsp_header_info.content_type_ptr :%s", signal.rsp_header_info.content_type_ptr);
    SCI_TraceLow("[Self Reg]  rsp_header_info.response_code :%d", signal.rsp_header_info.response_code);
    SCI_TraceLow("[Self Reg]  rsp_header_info.session_id_ptr :%s", signal.rsp_header_info.session_id_ptr);

     if (http_context_ptr->context_id != signal.context_id || http_context_ptr->request_id != signal.request_id)
    {
        return MMI_RESULT_FALSE;
    }

    if(signal.rsp_header_info.response_code == HTTP_RESPONSE_CODE_OK)
    {
        return HTTP_HeaderResponse(http_context_ptr->context_id, http_context_ptr->app_instance,
                        http_context_ptr->request_id, HTTP_DATA_TRANS_STYLE_BUFFER, PNULL, 0);
    }
    else if (signal.rsp_header_info.response_code == HTTP_RESPONSE_CODE_PARTIAL_CONTENT)
    {
        return HTTP_HeaderResponse(http_context_ptr->context_id, http_context_ptr->app_instance,
                        http_context_ptr->request_id, HTTP_DATA_TRANS_STYLE_BUFFER, PNULL, 0);
    }

    return MMI_RESULT_TRUE;
}

LOCAL void SlfReg_ParseReturnData(char *_data, int _len);

LOCAL MMI_RESULT_E _HandleHttpSigDataInd (SFR_HTTP_CONTEXT *http_context_ptr, DPARAM param)
{
    HTTP_DATA_IND_SIG_T signal = {0};
    HTTP_DATA_IND_SIG_T *cnf = (HTTP_DATA_IND_SIG_T*)param;

    HTTP_SIGNAL_ANALYTIC(param, signal, http_context_ptr, HTTP_DATA_IND_SIG_T);

    SCI_TraceLow("[Self Reg] _HandleHttpSigDataInd response_code :%d, data_len : %d", cnf->response_code, cnf->data_len);

    if (http_context_ptr->context_id != signal.context_id || http_context_ptr->request_id != signal.request_id)
    {
        return MMI_RESULT_FALSE;
    }

    SCI_TraceLow("[Self Reg] _HandleHttpSigDataInd : [%s]", cnf->data_ptr);

    if (cnf->response_code == 200)
    {
        SlfReg_ParseReturnData(cnf->data_ptr, cnf->data_len);
    }
    return MMI_RESULT_TRUE;
}

LOCAL MMI_RESULT_E _HandleHttpSigPostCnf (SFR_HTTP_CONTEXT *http_context_ptr, DPARAM param)
{
    HTTP_POST_CNF_SIG_T signal = {0};
    HTTP_POST_CNF_SIG_T *cnf = (HTTP_POST_CNF_SIG_T*)param;

    HTTP_SIGNAL_ANALYTIC(param, signal, http_context_ptr, HTTP_POST_CNF_SIG_T);

    SCI_TraceLow("[Self Reg] _HandleHttpSigPostCnf result :%d", signal.result);

   if (http_context_ptr->context_id != signal.context_id || http_context_ptr->request_id != signal.request_id)
    {
        return MMI_RESULT_FALSE;
    }

    SCI_TraceLow("[Self Reg] _HandleHttpSigPostCnf http return :%d", g_sfr_linker.http_return);

    if(signal.result == HTTP_SUCCESS && g_sfr_linker.http_return == TRUE)
    {
        MMISFR_End();
        return MMI_RESULT_TRUE;
    }

    MMISFR_ReTry(RETRY_SEC);
    return MMI_RESULT_TRUE;
}

LOCAL MMI_RESULT_E _HandleHttpSigCloseCnf (SFR_HTTP_CONTEXT *http_context_ptr, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;
    HTTP_CLOSE_CNF_SIG_T signal = {0};
    HTTP_CLOSE_CNF_SIG_T *cnf = (HTTP_CLOSE_CNF_SIG_T*)param;

    HTTP_SIGNAL_ANALYTIC(param, signal, http_context_ptr, HTTP_CLOSE_CNF_SIG_T);

    SCI_TraceLow("[Self Reg] _HandleHttpSigCloseCnf result :%d", signal.result);

    //TODO

    return result;
}

LOCAL MMI_RESULT_E _HandleHttpSigErrorInd (SFR_HTTP_CONTEXT *http_context_ptr, DPARAM param)
{
    HTTP_ERROR_IND_SIG_T signal = {0};
    HTTP_ERROR_IND_SIG_T *cnf = (HTTP_ERROR_IND_SIG_T*)param;

    HTTP_SIGNAL_ANALYTIC(param, signal, http_context_ptr, HTTP_ERROR_IND_SIG_T);

    SCI_TraceLow("[Self Reg] _HandleHttpSigErrorInd request_id : %d,  error :%d, context request_id : %d",
		signal.request_id, signal.result, http_context_ptr->request_id);

    if (http_context_ptr->context_id != signal.context_id || http_context_ptr->request_id != signal.request_id)
    {
        return MMI_RESULT_FALSE;
    }
    MMISFR_ReTry(RETRY_SEC);

    return MMI_RESULT_TRUE;
}

LOCAL void SlfReg_ParseReturnData(char *_data, int _len)
{
    if (g_sfr_linker.carrier != NULL && g_sfr_linker.carrier->carrier_return_parser != NULL)
    {
        g_sfr_linker.http_return = g_sfr_linker.carrier->carrier_return_parser(g_sfr_linker.carrier->carr_context, _data, _len);
    }
 }
