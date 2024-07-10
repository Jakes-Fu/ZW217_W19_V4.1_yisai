/****************************************************************************
** File Name:      mmiwww_netfront_cmd.c                                            *
** Author:                                                                    *
** Date:           06/15/2009                                                *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:    This file is used to describe the netfront cmd for WWW Browser            *
*****************************************************************************
**                         Important Edit History                            *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                *
** 06/2009        wancan.you        Create
** 
****************************************************************************/
#define _MMIWWW_NETFRONT_CMD_C_  

/**-------------------------------------------------------------------------*
**                         Dependencies                                     *
**--------------------------------------------------------------------------*/
#include "mmi_app_www_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmk_app.h"
#include "mmk_timer.h"
#include "mmiwww_internal.h"
#include "mmi_image.h"
#include "mmi_text.h"
#include "mmidl_export.h"
#include "mmi_appmsg.h"
#include "mmipub.h"
#include "mmiidle_export.h"
#include "mmipb_export.h"
#include "mmiwww_id.h"
#include "mmiwww_export.h"
#include "mmiwww_func.h"
#include "mmiclipbrd_export.h"
#include "gui_ucs2b_converter.h"
#ifdef BROWSER_SUPPORT_NETFRONT
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         LOCAL FUNCTION DECLARE                           *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : handle message wether save cookie
//  Global resource dependence : 
//  Author: kun.yu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWwwSaveCookie(MMI_WIN_ID_T      win_id, 
                                       MMI_MESSAGE_ID_E  msg_id, 
                                       DPARAM            param);

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Callback function which gives notification of error information.
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_NotifyErrorProc(slim_int in_index, slim_int in_code, slim_int in_nf)
{
    MMI_TEXT_ID_T alert_txt_id = TXT_NULL;
    WWW_APPLET_T *www_applet_ptr = MMIWWW_APPLETINSTANCE();
    BOOLEAN is_mem_alert  = FALSE;

    if (PNULL == www_applet_ptr)
    {
        return;
    }

    //SCI_TRACE_LOW:"MMIWWW_Netfront_NotifyErrorProc in_index=%d, in_code=%d, in_nf=%d, is_alert = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_NETFRONT_CMD_78_112_2_18_3_11_48_113,(uint8*)"dddd", in_index, in_code, in_nf, www_applet_ptr->www_instance.is_mem_alerted);

    switch (in_code)
    {
    case SLIM_BROWSER_ERROR_NOMEMORY:
        alert_txt_id = TXT_COMMON_NO_MEMORY;
        is_mem_alert  = TRUE;
        break;

    case SLIM_BROWSER_ERROR_NOMEMORY_IMAGE:
        alert_txt_id = TXT_COMMON_NO_MEMORY;
        is_mem_alert  = TRUE;
        break;

    case SLIM_BROWSER_ERROR_NOMEMORY_AWS:
        alert_txt_id = TXT_COMMON_NO_MEMORY;
        is_mem_alert  = TRUE;
        break;

    case SLIM_BROWSER_ERROR_TOOMANY_PANES:
        alert_txt_id = TXT_WWW_BROWSER_ERROR_TOOMANY_PANES;
        is_mem_alert  = TRUE;
        break;

    case SLIM_BROWSER_ERROR_TOTAL_SIZEOVER:
        alert_txt_id = TXT_WWW_CONTENT_E_SIZEOVER;
        is_mem_alert  = TRUE;
        break;

    case SLIM_BROWSER_ERROR_BADURL:
        alert_txt_id = TXT_WWW_BROWSER_ERROR_BADURL;
        break;

    case SLIM_BROWSER_ERROR_BADMIMETYPE:
        //alert_txt_id = TXT_WWW_BROWSER_ERROR_BADMIMETYPE;
        alert_txt_id = TXT_NULL;
        break;

    case SLIM_BROWSER_ERROR_MIXEDSECURETYPE:
        //alert_txt_id = TXT_WWW_BROWSER_ERROR_MIXEDSECURETYPE;
        alert_txt_id = TXT_NULL;
        break;

    case SLIM_BROWSER_ERROR_INPUTHISTORY_SIZEOVER:
        //alert_txt_id = TXT_UNKNOWN_ERROR;
        alert_txt_id = TXT_NULL;
        break;

    case SLIM_BROWSER_ERROR_INPUT_REQUIRED:
        alert_txt_id = TXT_INPUT_ERROR;
        break;

    case SLIM_E_GENERIC:
        alert_txt_id = TXT_NULL;
        break;

    default:
        alert_txt_id = TXT_UNKNOWN_ERROR;
        break;
    }

    NFBrowser_ReplyNotifyError(MMIWWW_GETBROWSER);

    if (TXT_NULL != alert_txt_id)
    {
        if (is_mem_alert)
        {
            if (www_applet_ptr->www_instance.is_mem_alerted)
            {
                //SCI_TRACE_LOW:"MMIWWW_Netfront_NotifyErrorProc in_index=%d, in_code=%d, in_nf=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_NETFRONT_CMD_147_112_2_18_3_11_48_114,(uint8*)"ddd", in_index, in_code, in_nf);
                return;
            }
            else
            {
                www_applet_ptr->www_instance.is_mem_alerted = TRUE;
            }
        }
        MMIWWW_OpenWWWAlertWin(PNULL, alert_txt_id, PNULL, IMAGE_PUBWIN_WARNING);
    }
}

/*****************************************************************************/
//  Description : Callback function which gives notification of content error information.
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_NotifyContentErrorProc(slim_int in_index, slim_int in_code, slim_int in_nf)
{
    MMI_TEXT_ID_T alert_txt_id = TXT_NULL;
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (PNULL == www_instance)
    {
        return;
    }

    //SCI_TRACE_LOW:"MMIWWW_Netfront_NotifyContentErrorProc in_index=%d, in_code=%d, in_nf=%d,pdp_state=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_NETFRONT_CMD_173_112_2_18_3_11_48_115,(uint8*)"dddd", in_index, in_code, in_nf, www_instance->pdp_info.pdp_state);

    if((MMIWWW_PDP_ACTIVE_FAILED == www_instance->pdp_info.pdp_state) || (MMIWWW_PDP_DEACTIVE_OK == www_instance->pdp_info.pdp_state))
    {
        BOOLEAN is_need_alert = TRUE;

        if (MMIAPISET_GetFlyMode())
        {
            is_need_alert = FALSE;
        }
        else
        {
                if (MMIPDP_INTERFACE_WIFI == www_instance->customized_connection.bearer_type)
                {
                    is_need_alert = TRUE;
                }
                else
                {
                    if (!MMIAPIPHONE_IsSimOk(www_instance->customized_connection.dual_sys))
                    {
                        is_need_alert = FALSE;
                    }
            }

            if (is_need_alert)
            {
                alert_txt_id = TXT_DIAL_FAILED;
            }
            else
            {
                alert_txt_id = TXT_NULL;
            }
        }
    }
    else
    {
        switch (in_code)
        {
        case SLIM_HTTP_E_DNS_INVAL:
            //alert_txt_id = TXT_WWW_HTTP_E_DNS_INVAL;
            alert_txt_id = TXT_WWW_BROWSER_ERROR_BADURL;
            break;

        case SLIM_HTTP_E_DNS_NOSERVER:
            //alert_txt_id = TXT_WWW_HTTP_E_DNS_NOSERVER;
            alert_txt_id = TXT_WWW_BROWSER_ERROR_BADURL;
            break;

        case SLIM_HTTP_E_DNS_TIMEDOUT:
            alert_txt_id = TXT_COMM_SERVER_ERROR;
            break;

        case SLIM_HTTP_E_DNS_NOTFOUND:
            //alert_txt_id = TXT_WWW_HTTP_E_DNS_NOTFOUND;
            alert_txt_id = TXT_WWW_BROWSER_ERROR_BADURL;
            break;

        case SLIM_HTTP_E_TCP_OPEN:
            alert_txt_id = TXT_COMM_SERVER_ERROR;
            break;

        case SLIM_HTTP_E_TCP_CONNECT:
            alert_txt_id = TXT_COMM_SERVER_ERROR;
            break;

        case SLIM_HTTP_E_TCP_READ:
            alert_txt_id = TXT_COMM_SERVER_ERROR;
            break;

        case SLIM_HTTP_E_TCP_WRITE:
            alert_txt_id = TXT_COMM_SERVER_ERROR;
            break;

        case SLIM_HTTP_E_SSL_CONNECT:
            alert_txt_id = TXT_COMM_SERVER_ERROR;
            break;

        case SLIM_HTTP_E_SSL_HANDSHAKE:
            alert_txt_id = TXT_COMM_SERVER_ERROR;
            break;

        case SLIM_HTTP_E_SSL_READ:
            alert_txt_id = TXT_COMM_SERVER_ERROR;
            break;

        case SLIM_HTTP_E_SSL_WRITE:
            alert_txt_id = TXT_COMM_SERVER_ERROR;
            break;

        case SLIM_HTTP_E_REQTIMEDOUT:
            alert_txt_id = TXT_WWW_HTTP_E_TCP_CONNECT;
            break;

        case SLIM_HTTP_E_RSPTIMEDOUT:
            alert_txt_id = TXT_WWW_HTTP_E_TCP_CONNECT;
            break;

        case SLIM_HTTP_E_CACHE_NONE:
            alert_txt_id = TXT_NULL;//TXT_WWW_HTTP_E_CACHE_NONE;
            break;

        case SLIM_HTTP_E_CACHE_EXPIRE:
            alert_txt_id = TXT_NULL;//TXT_WWW_HTTP_E_CACHE_EXPIRE;
            break;

        case SLIM_HTTP_E_AUTH_UNKNOWN:
            alert_txt_id = TXT_NULL;//TXT_LICENSE_ERROR;
            break;

        case SLIM_HTTP_E_AUTH_NOHEADER:
            alert_txt_id = TXT_NULL;//TXT_LICENSE_ERROR;
            break;

        case SLIM_HTTP_E_AUTH_FORMAT:
            alert_txt_id = TXT_NULL;//TXT_LICENSE_ERROR;
            break;

        case SLIM_HTTP_E_AUTH_IGNORED:
            alert_txt_id = TXT_NULL;//TXT_WWW_HTTP_E_AUTH_IGNORED;
            break;

        case SLIM_HTTP_E_REDIRECT_FORMAT:
            alert_txt_id = TXT_NULL;//TXT_WWW_HTTP_E_REDIRECT_FORMAT;
            break;

        case SLIM_HTTP_E_REDIRECT_CANCELED:
            alert_txt_id = TXT_NULL;//TXT_WWW_HTTP_E_REDIRECT_CANCELED;
            break;

        case SLIM_HTTP_E_REDIRECT_IGNORED:
            alert_txt_id = TXT_NULL;//TXT_WWW_HTTP_E_REDIRECT_IGNORED;
            break;

        case SLIM_HTTP_E_REQ_HEADER_SIZEOVER:
            alert_txt_id = TXT_NULL;//TXT_WWW_HTTP_E_REQ_HEADER_SIZEOVER;
            break;

        case SLIM_HTTP_E_REQ_BODY_SIZEOVER:
            alert_txt_id = TXT_NULL;//TXT_WWW_HTTP_E_REQ_BODY_SIZEOVER;
            break;

        case SLIM_HTTP_E_RSP_HEADER_SIZEOVER:
            alert_txt_id = TXT_NULL;//TXT_WWW_HTTP_E_RSP_HEADER_SIZEOVER;
            break;

        case SLIM_HTTP_E_REQ_TOOMANYCONTINUE:
            alert_txt_id = TXT_NULL;//TXT_WWW_HTTP_E_REQ_TOOMANYCONTINUE;
            break;

        case SLIM_HTTP_E_TLS_INTOLERANT:
            alert_txt_id = TXT_NULL;//TXT_WWW_HTTP_E_TLS_INTOLERANT;
            break;

        case SLIM_HTTP_E_REQ_MSGHDR_SIZEOVER:
            alert_txt_id = TXT_NULL;//TXT_UNKNOWN_ERROR;
            break;

        case SLIM_HTTP_E_CONTENT_ENCODING:
            alert_txt_id = TXT_NULL;//TXT_WWW_HTTP_E_CONTENT_ENCODING;
            break;

        case SLIM_CONTENT_E_SIZEOVER:
            alert_txt_id = TXT_NULL;//TXT_COMM_SERVER_ERROR;//TXT_WWW_CONTENT_E_SIZEOVER;
            break;

        case SLIM_CONTENT_E_NO_CONTENT:
            alert_txt_id = TXT_NULL;//TXT_COMM_SERVER_ERROR;//TXT_WWW_CONTENT_E_NO_CONTENT;
            break;

        case SLIM_CONTENT_E_TOOMANYREDIRECT:
            alert_txt_id = TXT_NULL;//TXT_WWW_CONTENT_E_TOOMANYREDIRECT;
            break;

        case SLIM_CONTENT_E_TOOMANYAUTH:
            alert_txt_id = TXT_WWW_CONTENT_E_TOOMANYAUTHN;
            break;

        case SLIM_CONTENT_E_BADMIMETYPE:
            //alert_txt_id = TXT_WWW_CONTENT_E_BADMIMETYPE;
            alert_txt_id = TXT_NULL;
            break;

        case SLIM_E_GENERIC:
            alert_txt_id = TXT_NULL;
            break;

        default:
            alert_txt_id = TXT_UNKNOWN_ERROR;
            break;
        }
    }

    NFBrowser_ReplyNotifyContentError(MMIWWW_GETBROWSER);

    if (TXT_NULL != alert_txt_id)
    {
        MMIWWW_OpenWWWAlertWin(PNULL, alert_txt_id, PNULL, IMAGE_PUBWIN_WARNING);
    }
}

/*****************************************************************************/
//  Description : Callback function which requests network connection.
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_RequestConnectionProc(slim_int in_index, slim_int in_nf)
{
    PEER_DP(("MMIWWW_Netfront_RequestConnectionProc in_index=%d, in_nf=%d", in_index, in_nf));

}

/*****************************************************************************/
//  Description : Callback function which requests to get special scheme other than HTTP/HTTPS.
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_RequestExSchemeRequestProc(slim_int in_index, slim_char *in_url, slim_int in_nf)
{
    PEER_DP(("MMIWWW_Netfront_RequestExSchemeRequestProc in_index=%d, in_nf=%d", in_index, in_nf));
    //SCI_TRACE_LOW:"RequestExSchemeRequestProc: in_url=%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_NETFRONT_CMD_390_112_2_18_3_11_48_116,(uint8*)"s", in_url);

    NFBrowser_ReplyRequestExScheme(MMIWWW_GETBROWSER, MMIWWW_HandleExScheme(in_url));
    return;
}

/*****************************************************************************/
//  Description : Callback function which requests authentication information.
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_RequestAuthProc(slim_char *in_url, slim_char *in_realm, slim_char *in_user, slim_char *in_pass, slim_int in_nf)
{
    PEER_DP(("MMIWWW_Netfront_RequestAuthProc in_nf=%d", in_nf));

    MMIWWW_SetAuthParam(in_url, in_realm, in_user, in_pass);

    MMIWWW_PostMsgToWWWApplet(MSG_WWW_AUTH_MSG, PNULL, 0);
}

/*****************************************************************************/
//  Description : Callback function which requests confirmation to accept cookie, it can only be called in prompt cookie mode.
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_RequestCookieConfirmProc(slim_char *in_url, slim_char *in_cookie, slim_int in_nf)
{
    PEER_DP(("MMIWWW_Netfront_RequestCookieConfirmProc in_nf=%d", in_nf));
    MMIPUB_OpenQueryWinByTextId(TXT_WWW_IF_SAVE_COOKIE,IMAGE_PUBWIN_QUERY,PNULL,HandleWwwSaveCookie);
    return;
}

/*****************************************************************************/
//  Description : Callback function which requests confirmation of server certificate, it's called due to server certificate verification problem during SSL communication.
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_RequestServerCertConfirmProc(slim_int in_verify_result, slim_int in_nf)
{
    BOOLEAN is_continue = FALSE;

    is_continue =  (0 != (in_verify_result & SLIM_CERT_WARNING_ERR_MASK)) || (0 != (in_verify_result & SLIM_CERT_CRL_MASK));

    PEER_DP(("MMIWWW_Netfront_RequestServerCertConfirmProc in_verify_result = %d, in_nf=%d, is_continue = %d", in_verify_result, in_nf, is_continue));

    if( is_continue )
    {
        NFBrowser_ReplyRequestServerCertConfirm(in_nf, TRUE, in_verify_result);
    }
    else
    {
        NFBrowser_ReplyRequestServerCertConfirm(in_nf, FALSE, in_verify_result);
    }
    return;
}

/*****************************************************************************/
//  Description : Callback function which requests displaying JavaScript
//                warning message, it's called when the JavaScript window.alert
//                method is called.
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_RequestJSAlertProc(slim_int   in_index,
                                               slim_char *in_message,
                                               slim_int   in_nf)
{
    slim_int        ret = SLIM_E_OK;

    PEER_DP(("MMIWWW_Netfront_RequestJSAlertProc index=%d", in_index));
    ret = NFBrowser_ReplyRequestJSAlert(in_nf);
    //SCI_TRACE_LOW:"MMIWWW_Netfront_RequestJSAlertProc ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_NETFRONT_CMD_457_112_2_18_3_11_49_117,(uint8*)"d", ret);
    if (PNULL != in_message)
    {
        MMIWWW_OpenJSAlertWin(in_index, in_message);
    }
    return;
}

/*****************************************************************************/
//  Description : Callback function which requests displaying of JavaScript
//                confirmation dialog box, it's called when the JavaScript
//                window.confirm method is called.
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_RequestJSConfirmProc(slim_int   in_index,
                                                 slim_char *in_message,
                                                 slim_int   in_nf)
{
    PEER_DP(("MMIWWW_Netfront_RequestJSConfirmProc index=%d", in_index));

    MMIWWW_PostMsgToWWWApplet(MSG_WWW_REQUEST_JSCONF, (DPARAM)in_message, (strlen((char *)in_message)*sizeof(slim_char)));
    return;
}

/*****************************************************************************/
//  Description : Callback function which requests displaying of JavaScript prompt,
//                it's called when the JavaScript window.prompt method is called.
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_RequestJSPromptProc(slim_int   in_index,
                                                slim_char *in_message,
                                                slim_char *in_value,
                                                slim_int   in_nf)
{
    PEER_DP(("MMIWWW_Netfront_RequestJSPromptProc index=%d", in_index));

    MMIWWW_SetJSPromtString(in_message, in_value);

    MMIWWW_PostMsgToWWWApplet(MSG_WWW_REQUEST_JSPROMPT, (DPARAM)in_message, (strlen((char *)in_message)*sizeof(slim_char)));
    return;
}

/*****************************************************************************/
//  Description : Callback function which requests to open a new window.
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_RequestOpenWindowProc(slim_int in_index, slim_int in_request, slim_int in_nf)
{
    slim_int    ret = SLIM_E_OK;

    PEER_DP(("MMIWWW_Netfront_RequestOpenWindowProc index=%d request=%d nf=%d",
            in_index, in_request, in_nf));

    MMIWWW_CreateBrowserTab();

    ret = NFBrowser_ReplyRequestOpenWindow(in_nf, in_request, slim_true, slim_false);
    PEER_DP(("MMIWWW_Netfront_RequestOpenWindowProc ret=%d", ret));
    return;
}

/*****************************************************************************/
//  Description : Callback function which notifies that NF browser engine started getting content in the window.
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_NotifyWindowStartProc(slim_int in_index, slim_int in_nf)
{
    PEER_DP(("MMIWWW_Netfront_NotifyWindowStartProc in_index = %d, in_nf=%d", in_index, in_nf));

    if(MMIWWW_GetBrowserstopFlag())
    {
        return;
    }

    MMIWWW_SendMsgToBrowser(MSG_WWW_PAGE_LOAD_START, in_index);
    return;
}

/*****************************************************************************/
//  Description : Callback function which notifies that document analysis within the target window is complete, and all linked content has been obtained.
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_NotifyWindowDoneProc(slim_int in_index, slim_int in_nf)
{
    PEER_DP(("MMIWWW_Netfront_NotifyWindowDoneProc in_index = %d, in_nf=%d", in_index, in_nf));

    if(MMIWWW_GetBrowserstopFlag())
    {
        return;
    }

    MMIWWW_SendMsgToBrowser(MSG_WWW_PAGE_LOAD_FINISH, in_index);
    return;
}

#ifdef SLIM_CONFIG_USE_WML
/*****************************************************************************/
//  Description : Callback function which notifies that an error occurred in WML content.
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_NotifyWMLErrorProc(slim_char *in_message, slim_int in_nf)
{
    PEER_DP(("MMIWWW_Netfront_NotifyWMLErrorProc in_nf=%d", in_nf));

}
#endif

/*****************************************************************************/
//  Description : Callback function which gives input format error notification. It's called when there is input that violates the format attribute specified in WML input element or the specification of the CSS -wap-input-format property.
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_NotifyInputFormatErrorProc(slim_int in_index, slim_char *in_format, slim_char *in_value, slim_int in_nf)
{
    PEER_DP(("MMIWWW_Netfront_NotifyInputFormatErrorProc in_index = %d, in_nf=%d", in_index, in_nf));

    NFBrowser_ReplyNotifyInputFormatError(in_nf);
    MMIWWW_OpenWWWAlertWin(PNULL, TXT_INPUT_ERROR, PNULL, IMAGE_PUBWIN_WARNING);
}

/*****************************************************************************/
//  Description : Callback function which notifies that a syntax error occurred in XML content.
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_NotifyXMLSAXErrorProc(slim_char *in_message, slim_int in_nf)
{
    PEER_DP(("MMIWWW_Netfront_NotifyXMLSAXErrorProc in_nf=%d", in_nf));

}

#ifdef SLIM_CONFIG_USE_WMLSCRIPT
/*****************************************************************************/
//  Description : Callback function which requests displaying WMLScript warning message, it's called when the WMLScript window.alert method is called.
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_RequestWMLSAlertProc(slim_char *in_message, slim_int in_nf)
{
   PEER_DP(("MMIWWW_Netfront_RequestWMLSAlertProc in_nf=%d", in_nf));
   
    NFBrowser_ReplyRequestWMLSAlert(in_nf);
    if (PNULL != in_message)
    {
        MMIWWW_OpenWMLSAlertWin(in_message);
    }
    return;
}

/*****************************************************************************/
//  Description : Callback function which requests displaying WMLScript confirmation message, it's called when the WMLScript window.confirm method is called.
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_RequestWMLSConfirmProc(slim_char *in_message, slim_int in_nf)
{
    PEER_DP(("MMIWWW_Netfront_RequestWMLSConfirmProc in_nf=%d", in_nf));

    MMIWWW_PostMsgToWWWApplet(MSG_WWW_REQUEST_WMLJSCONF, (DPARAM)in_message, (strlen((char *)in_message)*sizeof(slim_char)));
    return;
}

/*****************************************************************************/
//  Description : Callback function which requests displaying of WMLScript prompt dialog box, it's called when the WMLScript window.prompt method is called.
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_RequestWMLSPromptProc(slim_char *in_message, slim_char *in_value, slim_int in_nf)
{
    PEER_DP(("MMIWWW_Netfront_RequestWMLSPromptProc in_nf=%d", in_nf));

    MMIWWW_SetJSPromtString(in_message, in_value);

    MMIWWW_PostMsgToWWWApplet(MSG_WWW_REQUEST_WMLJSPROMPT, (DPARAM)in_message, (strlen((char *)in_message)*sizeof(slim_char)));
    return;
}
#endif

/*****************************************************************************/
//  Description : notify helper app start 
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_HelperProc(NFBrowserDownloadInfo *in_info, slim_int in_self)
{
    MMIDL_DOWNLOAD_INFO_T download_info = {0};
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    PEER_DP(("MMIWWW_Netfront_HelperProc in_self=%d", in_self));

    if (PNULL == in_info)
    {
        PEER_DP(("MMIWWW_Netfront_HelperProc in_info is PNULL"));
        return;
    }

    if (PNULL == www_instance)
    {
        PEER_DP(("MMIWWW_Netfront_HelperProc www_instance is PNULL"));
        return;
    }

    if (-1 == in_info->fType)
    {
        //SCI_TRACE_LOW:"MMIWWW_Netfront_HelperProc Neednot Download!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_NETFRONT_CMD_654_112_2_18_3_11_49_118,(uint8*)"");
        return;
    }

    download_info.mime_type = in_info->fType;
    download_info.http_header = in_info->fHTTPHeader;
    download_info.url = in_info->fURL;
    download_info.is_chunk = (BOOLEAN)in_info->fIsChunk;
    download_info.status_code = in_info->fStatusCode;
    download_info.method = in_info->fMethod;
    download_info.buf = in_info->fDDBuf;
    download_info.bearer_type = www_instance->customized_connection.bearer_type;

    MMIAPIDL_StartDownload(MMIDL_CREATE_NEW, &download_info);
}

/*****************************************************************************/
//  Description : notify helper app end
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_HelperDoneProc (NFBrowserDownloadInfo *in_info, slim_int in_nf)
{
    PEER_DP(("MMIWWW_Netfront_HelperProc in_nf=%d", in_nf));

}

/*****************************************************************************/
//  Description : notify entering text control, inline IME should be started in this callback
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_EnterTextControlProc(void* in_arg, NFBrowserControlInfo *in_info)
{
    MMIWWW_InlineIME_EnterTextControl(in_info);
}

/*****************************************************************************/
//  Description : notify leaving text control, inline IME should be quited in this callback
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_LeaveTextControlProc(void* in_arg)
{
    NFBrowserInputParam user_input = {0};/*lint !e64*/
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (PNULL == www_instance)
    {
        return;
    }

    if (PNULL != www_instance->ime_win_id && MMK_IsOpenWin(www_instance->ime_win_id))
    {
        MMK_CloseWin(www_instance->ime_win_id);
        www_instance->ime_win_id = PNULL;
    }

    if (!MMIWWW_GetBrowserstopFlag() && MMIWWW_STATE_PAGE_VIEW == MMIWWW_GetWWWState(NFBrowser_ActiveWindow(www_instance->browser_instance)))
    {
        /* Make Text CTRL Lose Focus */
        user_input.u.fMouse.fX = -1;
        user_input.u.fMouse.fY = -1;
        user_input.fType = INPUT_TYPE_MOUSE_DOWN ;
        user_input.u.fMouse.fButton = SLIM_MOUSEBUTTON_LEFT;
        NFBrowser_UserInput(www_instance->browser_instance, &user_input);

        user_input.u.fMouse.fX = -1;
        user_input.u.fMouse.fY = -1;
        user_input.fType = INPUT_TYPE_MOUSE_UP;
        user_input.u.fMouse.fButton = SLIM_MOUSEBUTTON_LEFT;
        NFBrowser_UserInput(www_instance->browser_instance, &user_input);
    }

    return;
}

/*****************************************************************************/
//  Description : notify text control position moved,  inline IME should be moved accordingly
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_MoveTextControlProc(void* in_arg, NFBrowserControlInfo *in_info)
{
    return;
}

/*****************************************************************************/
//  Description : get text control value from inline IME, charset of out_string must be utf-8
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_bool MMIWWW_GetTextControlValueProc(void *in_arg, slim_char *out_string)
{
    return MMIWWW_InlineIME_GetTextControlValue(out_string);
}

/*****************************************************************************/
//  Description : get text control value length from inline IME
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_int MMIWWW_GetTextControlValueLengthProc(void *in_arg)
{
    return MMIWWW_InlineIME_GetTextControlValueLength();
}

/*****************************************************************************/
//  Description : set text control value to inline IME, charset of in_string is utf-8
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_bool MMIWWW_SetTextControlValueProc(void *in_arg, slim_char *in_string)
{
    return TRUE;
}

/*****************************************************************************/
//  Description : notify inline IME visible or invisible
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_ShowTextControlProc(void *in_arg, slim_bool in_show)
{

}

/*****************************************************************************/
//  Description : enter the file selection process
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_bool MMIWWW_Netfront_FileSelectEnterProc(slim_int in_select, slim_int in_nf)
{
    PEER_DP(("MMIWWW_Netfront_FileSelectEnterProc in_select=%d,in_self=%d", in_select, in_nf));

    MMIWWW_PostMsgToWWWApplet(MSG_WWW_NOTIFY_UPLOAD, (DPARAM)&in_select, sizeof(slim_int));

    return slim_true;
}

/*****************************************************************************/
//  Description : leave the file selection process
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_bool MMIWWW_Netfront_FileSelectLeaveProc(slim_int in_nf)
{
    PEER_DP(("MMIWWW_Netfront_FileSelectLeaveProc in_nf=%d", in_nf));

    MMIWWW_PostMsgToWWWApplet(MSG_WWW_NOTIFY_UPLOAD_END, PNULL, 0);//added by rui.li 20100128

    return slim_true;
}

/*****************************************************************************/
//  Description : get display name based on full path of file
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_bool MMIWWW_Netfront_FileSelectQueryNameProc(slim_char* in_fullpath, slim_char* out_name, slim_int in_nf)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();
    uint16 upload_path_len = 0;
    
    PEER_DP(("MMIWWW_Netfront_FileSelectQueryNameProc in_nf=%d", in_nf));

    if (PNULL != www_instance && PNULL != www_instance->upload_param.upload_path)
    {
        upload_path_len = SCI_STRLEN((char *)www_instance->upload_param.upload_path);
        SCI_MEMCPY(in_fullpath, www_instance->upload_param.upload_path, upload_path_len);
    }

    return slim_true;
}

/*****************************************************************************/
//  Description : show favicon callback
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_FaviconNotifyShowProc(slim_int in_index, NFBrowserPixelMap *out_pixelmap, slim_int in_nf)
{
    PEER_DP(("MMIWWW_Netfront_FaviconNotifyShowProc in_nf=%d", in_nf));

}

/*****************************************************************************/
//  Description : notify callback for copy selected text
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_CopySelectedTextProc(slim_char* in_text, slim_int in_len, slim_int in_nf)
{
    wchar *wstr_ptr = PNULL;

    PEER_DP(("MMIWWW_Netfront_CopySelectedTextProc in_nf=%d", in_nf));
    if (PNULL != in_text && in_len > 0)
    {
        wstr_ptr = SCI_ALLOCA((uint32)(in_len + 1) * sizeof(wchar));
        if (PNULL == wstr_ptr)
        {
            return;
        }
        SCI_MEMSET(wstr_ptr, 0x00, ((in_len + 1) * sizeof(wchar)));/*lint !e737*/
        in_len = GUI_UTF8ToWstr(wstr_ptr, (uint16)in_len, in_text, (uint16)in_len);
        MMIAPICLIPBRD_SetData(CLIPBRD_FORMAT_TEXT, wstr_ptr, (uint32)(in_len * sizeof(wchar)));/*lint !e737*/
        SCI_FREE(wstr_ptr);
        MMIPUB_OpenAlertSuccessWin( TXT_WWW_COPY_SUCCESS);
    }
}

#ifdef SLIM_CONFIG_USE_RSS
/*****************************************************************************/
//  Description : notify callback for RSS auto discovery
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_FeedNotifyItemCountProc(slim_int in_index, NFBrowserPixelMap *out_pixelmap, slim_int in_nf)
{
    PEER_DP(("MMIWWW_Netfront_FeedNotifyItemCountProc in_nf=%d", in_nf));

}
#endif

/*****************************************************************************/
//  Description : set timer proc
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC NFBrowserTimer MMIWWW_Netfront_SetTimer(NFBrowserTimerImpl *self, slim_int in_time, NFBrowserTimer_TimerProc in_proc, slim_opaque in_arg)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();
    int32 i = 0;

    if(PNULL == www_instance || www_instance->is_disable_timer)
    {
    	return PNULL;
    }

    if (0 > in_time)
    {
        return PNULL;
    }
    else if (50 > in_time)
    {
        in_time = 50;
    }

    for (i = 0; i < MMIWWW_MAX_TIMER_NUM; i++)
    {
        if (0 == www_instance->timer_proc[i].timer_id)
        {
            www_instance->timer_proc[i].timer_id = MMK_CreateTimerCallback((uint32)in_time, MMIAPIWWW_HandleWWWTimer, NULL, FALSE);
            www_instance->timer_proc[i].in_proc = in_proc;
            www_instance->timer_proc[i].in_arg = in_arg;

            self->fUserData = (slim_opaque)&www_instance->timer_proc[i].timer_id;
            break;
        }
    }

    return (NFBrowserTimer)self->fUserData;
}

/*****************************************************************************/
//  Description : kill timer proc
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_bool MMIWWW_Netfront_KillTimer(NFBrowserTimerImpl *self, NFBrowserTimer in_timer)
{
    slim_int i = 0;
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();
    uint8 timer_id = 0;

    timer_id = *(uint8 *)in_timer;

    if (0 == timer_id)
    {
        return slim_true;
    }

    if (PNULL == www_instance)
    {
        return slim_false;
    }

    for (i = 0; i < MMIWWW_MAX_TIMER_NUM; i ++)
    {
        if (timer_id == www_instance->timer_proc[i].timer_id)
        {
            MMK_StopTimer(timer_id);
            www_instance->timer_proc[i].timer_id = 0;
            www_instance->timer_proc[i].in_proc = PNULL;
            www_instance->timer_proc[i].in_arg = PNULL;
            break;
        }
    }

    return slim_true;
}

/*****************************************************************************/
// Description : Update down/up progress call back
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_UpdateWindowProgressProc(slim_int  in_downlink_percent, 
                                            slim_int  in_uplink_percent,
                                            slim_int  in_nf)
{
    PEER_DP(("MMIWWW_UpdateWindowProgressProc down=%d", in_downlink_percent));

    if(MMIWWW_GetBrowserstopFlag())
    {
        return;
    }

    MMIWWW_SendMsgToBrowser(MSG_WWW_UPDATE_LOAD_PERCENT, in_downlink_percent);
    return;
}

/*****************************************************************************/
// Description : Update page title call back
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_UpdateWinTitleProc(slim_int   in_index,
                                      slim_char *in_title, 
                                      slim_int   in_charset, 
                                      slim_int   in_nf)
{
    PEER_DP(("MMIWWW_UpdateWinTitleProc"));
    return;
}

/*****************************************************************************/
//  Description : NF browser engine notifies NF browser application whether current webpage is secure.
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_NotifySecurePageProc(slim_bool in_secure, slim_int in_nf)
{
    PEER_DP(("MMIWWW_NotifySecurePageProc in_secure = %d, in_nf=%d", in_secure, in_nf));

    if(MMIWWW_GetBrowserstopFlag())
    {
        return;
    }

    MMIWWW_SendMsgToBrowser( MSG_WWW_PAGE_SECURE_INFO, in_secure);

    return;
}

/*****************************************************************************/
//  Description : NF browser engine notifies NF browser application whether bgsound exists in the window where webpage was just now loaded.
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_NotifyBGSoundExistProc(slim_bool in_exist, slim_int in_index, slim_int in_nf)
{
    PEER_DP(("MMIWWW_NotifyBGSoundExistProc in_exist = %d, in_index = %d, in_nf=%d", in_exist, in_index, in_nf));

    if(MMIWWW_GetBrowserstopFlag())
    {
        return;
    }

    MMIWWW_SendMsgToBrowser( MSG_WWW_NOTIFY_BG_SOUND, (uint32)(((uint8)in_exist & 0x00FF) | ((uint8)in_index << 8 & 0xFF00)));
    return;
}

/*****************************************************************************/
//  Description : handle message wether save cookie
//  Global resource dependence : 
//  Author: kun.yu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWwwSaveCookie(MMI_WIN_ID_T      win_id, 
                                       MMI_MESSAGE_ID_E  msg_id, 
                                       DPARAM            param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        NFBrowser_ReplyRequestCookieConfirm(MMIWWW_GETBROWSER,slim_true);
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_CANCEL:
        NFBrowser_ReplyRequestCookieConfirm(MMIWWW_GETBROWSER,slim_false);
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}

#endif //#ifdef BROWSER_SUPPORT_NETFRONT


/*Edit by script, ignore 1 case. Fri Apr 27 09:38:56 2012*/ //IGNORE9527
