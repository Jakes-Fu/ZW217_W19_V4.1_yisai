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

#include "sfr_carrier_manager.h"
#include "sfr_header.h"

PUBLIC void SlfReg_StartHttpRequest(int net_id)
{
    HTTP_INIT_PARAM_T init_param = {0};
    init_param.is_cookie_enable = FALSE;
    init_param.is_cache_enable = FALSE;
    init_param.net_id = net_id;

    SCI_TraceLow("[Self Reg] SlfReg_StartHttpRequest(net id : %d) module id (%d)", net_id, MMI_MODULE_SFR);

   if (HTTP_SUCCESS != HTTP_InitRequest(MMI_MODULE_SFR, SFR_HTTP_APPFLAG, &init_param))
    {
        SCI_TraceLow("[Self Reg] SlfReg_StartHttpRequest ---- Error");
        MMISFR_ReTry(10);
    }
}

PUBLIC MMI_RESULT_E SlfReg_CloseRequest(void)
{
    MMI_RESULT_E        result = MMI_RESULT_FALSE;

     if(!HTTP_CloseRequest(g_sfr_linker.http_ctx.context_id))
    {
        result = MMI_RESULT_TRUE;
    }
    SCI_TraceLow("[Self Reg] SlfReg_CloseRequest ");
    return result;
}

PUBLIC MMI_RESULT_E SlfReg_PostPackageRequest(void)
{
#ifndef WIN32
    int len = 0;
    MMI_RESULT_E result = MMI_RESULT_FALSE;
    HTTP_POST_PARAM_T *post_param = NULL;
    if (g_sfr_linker.carrier == NULL)
    {
        return result;
    }
 post_param = (HTTP_POST_PARAM_T*)SCI_ALLOC_APPZ(sizeof(HTTP_POST_PARAM_T));
    if (post_param != NULL )
    {
        post_param->connection = HTTP_CONNECTION_KEEP_ALIVE;
        post_param->get_data.style = HTTP_DATA_TRANS_STYLE_UNKNOW;
        post_param->post_body.u.post_buffer.is_copied_by_http= TRUE;
        post_param->need_net_prog_ind = FALSE;
        post_param->accept.accept_ptr = g_sfr_linker.carrier->carrier_get_accept(g_sfr_linker.carrier->carr_context, &len);
        post_param->user_agent.user_agent_ptr = g_sfr_linker.carrier->carrier_get_agent(g_sfr_linker.carrier->carr_context, &len);
        post_param->get_data.is_header_use_file = FALSE;
        post_param->post_body.is_use_file = FALSE;
        post_param->post_body.body_type_ptr = g_sfr_linker.carrier->carrier_get_bodytype(g_sfr_linker.carrier->carr_context, &len);
        post_param->post_body.u.post_buffer.buffer_ptr = g_sfr_linker.carrier->carrier_get_package(g_sfr_linker.carrier->carr_context, &len);
        post_param->post_body.u.post_buffer.buffer_len = len;
        post_param->is_use_post_after_redirect = TRUE;

        post_param->uri.uri_ptr = g_sfr_linker.carrier->carrier_get_url(g_sfr_linker.carrier->carr_context, &len);
        SCI_TraceLow("[Self Reg]  Post url (%s)", post_param->uri.uri_ptr);

        //SFR_LongTrace(post_param->post_body.u.post_buffer.buffer_ptr, len);

        if(HTTP_SUCCESS == HTTP_PostRequest(g_sfr_linker.http_ctx.context_id, post_param, MMI_MODULE_SFR))
        {
            result = MMI_RESULT_TRUE;
        }

	 SCI_FREE(post_param);
    }

    return result;
#endif
}
