/*****************************************************************************
** File Name:      mmiwlan_portal_net.c                                         *
** Author:         wancan.you                                                     *
** Date:           2011.03.15                                                *
** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2011.03.15     wancan.you            Create                                    *
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_wlan_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "sci_types.h"
#include "mmk_type.h"
#include "window_parse.h"
#include "mmiwlan_id.h"
#include "mmiwlan_text.h"
#include "mmidisplay_data.h"
#include "mmk_app.h"
#include "mmi_modu_main.h"
#include "mmipub.h"
#include "mmiwlan_manager.h"
#include "mmiwlan_win.h"
#include "mmiwifi_export.h"
#include "mmiwlan_menutable.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmifilearray_export.h"
#include "mmi_default.h"
#include "mmifmm_export.h"
#include "mmiwlan_image.h"
#include "mmiwlan_anim.h"
#include "cfl_char.h"
#include "mmiwlan_nv.h"
#include "mmiidle_export.h"
#include "mmk_msg.h"
#include "mmi_mainmenu_export.h"
#include "mmiwlan_internal.h"
#include "mmiwlan_portal.h"
#include "mmiwifi_export.h"
#include "in_message.h"
#include "http_api.h"
//#include "mmieng.h"
#include "mmieng_export.h"
#include "mmi_common.h"
#include "mmi_nv.h"

/**-------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMIWLAN_PORTAL_URL  "http://www.baidu.com"
#ifdef ACCEPT_STRING
#undef ACCEPT_STRING
#endif

#define ACCEPT_STRING "Accept: */*"

#define MMIWLAN_PORTAL_DATA_LENGTH_MAX  (50*1024)

#define MMIWLAN_HTTP_STATUS_CODE_SUCCESS                  (200)
#define MMIWLAN_HTTP_STATUS_CODE_PARTIAL_CONTENT          (206)
#define MMIWLAN_HTTP_STATUS_CODE_MOVED_PERMANENTLY        (301)
#define MMIWLAN_HTTP_STATUS_CODE_FOUND                    (302)
#define MMIWLAN_HTTP_STATUS_CODE_TEMPORARY_REDIRECT       (307)
#define MMIWLAN_HTTP_STATUS_CODE_UNAUTHORIZED             (401)
#define MMIWLAN_HTTP_STATUS_CODE_UTHENTICATION_REQUIRED   (407)

#define MMIWLAN_HTTP_CODE_MOVED_TEMPORARILY "HTTP/1.1 302"
#define MMIWLAN_HTTP_CODE_OK "HTTP/1.1 200"
#define MMIWLAN_HTTP_LOCATION "Location: "

#define MMIWLAN_HTTP_CRLF_STRING "\r\n"
#define MMIWLAN_HTTP_QUOTES_MARK "\""
#define MMIWLAN_HTTP_QUESTION_MARK "?"
#define MMIWLAN_HTTP_EQUAL_MARK "="
#define MMIWLAN_HTTP_AND_MARK "&"
#define MMIWLAN_HTTP_VALUE "value="
#define MMIWLAN_HTTP_NAME "name="
#define MMIWLAN_HTTP_ACTION "action="

#define MMIWLAN_PORTAL_MYFORM "NAME=\"myForm\""
#define MMIWLAN_PORTAL_LOGINFORM "NAME=\"loginform\""
#define MMIWLAN_PORTAL_METHOD "method="
#define MMIWLAN_PORTAL_GET "get"
#define MMIWLAN_PORTAL_POST "post"

#define MMIWLAN_PORTAL_USER_MARK "name=\"USER\""
#define MMIWLAN_PORTAL_PWD_MARK "name=\"PWD\""
#define MMIWLAN_PORTAL_ACNAME_MARK "name=\"wlanacname\""
#define MMIWLAN_PORTAL_USERIP_MARK "name=\"wlanuserip\""
#define MMIWLAN_PORTAL_ACIP_MARK "name=\"wlanacip\""
#define MMIWLAN_PORTAL_ACTIONTYPE_MARK "name=\"actiontype\""
#define MMIWLAN_PORTAL_PWDTYPE_MARK "name=\"pwdtype\'"//可选(1或者为空为静态密码；2为动态密码)
#define MMIWLAN_PORTAL_AUTHEN_MARK "name=\"authen\""//可选(1或者为空为不申请免证登入；0为申请免证登入)
#define MMIWLAN_PORTAL_ACSSID_MARK "name=\"wlanacssid\""//可选
#define MMIWLAN_PORTAL_LOGONSESSID_MARK "name=\"logonsessid\""//可选
#define MMIWLAN_PORTAL_LANGUAGETYPE_MARK "name=\"languagetype\""//可选
//#define MMIWLAN_PORTAL_SUBMIT_MARK "name=\"Submit\""//可选
#define MMIWLAN_PORTAL_LOGINIP_MARK "name=\"LOGINIP\""
#define MMIWLAN_PORTAL_LOGINNAME_MARK "name=\"LOGINNAME\""
#define MMIWLAN_PORTAL_DEVICEID_MARK "name=\"DEVICEID\""
#define MMIWLAN_PORTAL_WLANDIG_MARK "name=\"WLANDIG\""
#define MMIWLAN_PORTAL_WLANSID_MARK "name=\"WLANSID\""

#define MMIWLAN_PORTAL_ACTIONTYPE_LOGIN "LOGIN"
#define MMIWLAN_PORTAL_ACTIONTYPE_LOGOUT "LOGOUT"

#define MMIWLAN_PORTAL_ACNAME "wlanacname"
#define MMIWLAN_PORTAL_USERIP "wlanuserip"
#define MMIWLAN_PORTAL_ACIP "wlanacip"
#define MMIWLAN_PORTAL_ACTIONTYPE "actiontype"
#define MMIWLAN_PORTAL_ACSSID "wlanacssid"
#define MMIWLAN_PORTAL_LOGONSESSID "logonsessid"
#define MMIWLAN_PORTAL_LANGUAGETYPE "languagetype"
#define MMIWLAN_PORTAL_USER "USER"
#define MMIWLAN_PORTAL_PWD "PWD"
#define MMIWLAN_PORTAL_SZ_USER "bpssUSERNAME"
#define MMIWLAN_PORTAL_SZ_PWD "bpssBUSPWD"
#define MMIWLAN_PORTAL_LOGINTYPE "bpssLoginType"
#define MMIWLAN_PORTAL_SUBMIT "Submit"
#define MMIWLAN_PORTAL_SMSRND "bpssSMSRND"
#define MMIWLAN_PORTAL_LOGOUTTYPE "logouttype"
#define MMIWLAN_PORTAL_LOGINIP "LOGINIP"
#define MMIWLAN_PORTAL_LOGINNAME "LOGINNAME"
#define MMIWLAN_PORTAL_DEVICEID "DEVICEID"
#define MMIWLAN_PORTAL_WLANDIG "WLANDIG"
#define MMIWLAN_PORTAL_WLANSID "WLANSID"


#define MMIWIFI_PORTAL_LOGOUTFORM "name=\"logoutForm\""

#define MMIWLAN_PORTAL_LOGIN_RES "login_res"//语法login_res=|0|
//#define MMIWLAN_PORTAL_APPLYPWD_RES "applypwd_res"
#define MMIWLAN_PORTAL_OFFLINE_RES "offline_res"


#define MMIWLAN_JUDGE_METHOD_LENGTH_MAX  (20)

/*var type*/
#define URL_ESCAPE       0    // $(var:escape)
#define URL_NOCHANGE     1    // $(var:nochange)
#define URL_UNESCAPE     2    // $(var:unescape)

/**-------------------------------------------------------------------------*
**                         ENUM DEFINITION                                  *
**--------------------------------------------------------------------------*/
typedef enum
{
    MMIWIFI_PDP_NONE,
    MMIWIFI_PDP_ACTIVING,
    MMIWIFI_PDP_ACTIVE_OK,        
    MMIWIFI_PDP_ACTIVE_FAILED,
    MMIWIFI_PDP_DEACTIVING,
    MMIWIFI_PDP_DEACTIVE_FAILED,
    MMIWIFI_PDP_DEACTIVE_OK = MMIWIFI_PDP_NONE
}MMIWIFI_PDP_STATE_E;

typedef enum
{
    MMIWIFI_HTTP_METHOD_NONE,
    MMIWIFI_HTTP_METHOD_GET,
    MMIWIFI_HTTP_METHOD_POST,
    MMIWIFI_HTTP_METHOD_MAX
}MMIWIFI_HTTP_METHOD_E;

typedef enum
{
    MMIWIFI_LOGINTYPE_NONE,     //未设置
    MMIWIFI_LOGINTYPE_NORMAL,      //普通login type
    MMIWIFI_LOGINTYPE_GD       //guangdong type
}MMIWIFI_LOGIN_TYPE_E;


/**-------------------------------------------------------------------------*
**                         TYPE DEFINITION                                  *
**--------------------------------------------------------------------------*/
typedef struct _MMIWIFI_WLAN_PORTAL_URL_T_
{
    uint8 * url_ptr;
    uint16 url_len;
    uint8 * body_ptr;
    uint16 body_len;
    MMIWIFI_HTTP_METHOD_E method;
}MMIWIFI_WLAN_PORTAL_URL_T;

typedef struct _MMIWIFI_WLAN_PORTAL_INFO_T_
{
    uint32 netid;
    HTTP_CONTEXT_ID_T context_id;
    HTTP_APP_INSTANCE_T app_instance;
    HTTP_REQUEST_ID_T request_id;
    MMIWIFI_PDP_STATE_E status;
    MMIWIFI_WLAN_PORTAL_URL_T logout_url;
    uint8 * cur_page_url_ptr;    //for Shenzhen WLAN
    uint16 cur_page_url_len;    //for Shenzhen WLAN
    uint8 * data_ptr;
    uint32 data_len;
    uint32 recv_len;
    MMIWIFI_LOGIN_TYPE_E login_type;
    uint8* cookie_ptr;
    uint32 response_code;
}MMIWIFI_WLAN_PORTAL_INFO_T;

/**-------------------------------------------------------------------------*
**                         GLOBAL VARIABLE DEFINITION                       *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         STATIC CLASS DEFINITION                          *
**--------------------------------------------------------------------------*/
MMIWIFI_WLAN_PORTAL_INFO_T s_wlan_portal_info = {0};

/**-------------------------------------------------------------------------*
**                         STATIC VARIABLE DEFINITION                       *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         LOCAL FUNCTION DECLARE                           *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Opera escape '!' '~' '*' '\'' '(' ')' 
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsUnreservedChar(uint8 c);

/*****************************************************************************/
//  Description : Escape URL
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL uint32 URL_EscapeUrl(uint8 *dst_ptr, uint32 dst_len, const uint8 *url_ptr, char var_type);

/*****************************************************************************/
// 	Description : process ps signal
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL void HandleWlanPortalPsMsg(MMIPDP_CNF_INFO_T *msg_ptr);

/*****************************************************************************/
//  Description : Start Http Request
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN StartPortalHttpRequest(uint32 net_id);

/*****************************************************************************/
//  Description : Get Signal Struct
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL void GetSignalStruct(DPARAM param, void *signal_struct_ptr, uint16 struct_size);

/*****************************************************************************/
//  Description : Get/Post Request
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN PortalRequest(uint8 * url_ptr, uint16 url_len, uint8 * body_ptr, uint16 body_len,
                                            MMIWIFI_HTTP_METHOD_E method, HTTP_CONTEXT_ID_T context_id, HTTP_APP_INSTANCE_T app_instance);

/*****************************************************************************/
//  Description : Handle Http Init Cnf
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePortalHttpInitCnf(DPARAM param);

/*****************************************************************************/
//  Description : Handle Http Request Id Ind
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePortalHttpRequestIdInd(DPARAM param);

/*****************************************************************************/
//  Description : Handle Http Need Auth Ind
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePortalHttpNeedAuthInd(DPARAM param);

/*****************************************************************************/
//  Description : Handle Http Get Cnf
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePortalHttpGetCnf(DPARAM param);

/*****************************************************************************/
//  Description : Handle Http Head Cnf
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePortalHttpHeadCnf(DPARAM param);

/*****************************************************************************/
//  Description : Handle Http Data Ind
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePortalHttpDataInd(DPARAM param);

/*****************************************************************************/
//  Description : Handle Http Header Ind
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePortalHttpHeaderInd(DPARAM param);

/*****************************************************************************/
//  Description : Handle Http Error Ind
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePortalHttpErrorInd(DPARAM param);

/*****************************************************************************/
//  Description : Handle Http Close Cnf
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePortalHttpCloseCnf(DPARAM param);

/*****************************************************************************/
//  Description : Handle Http Cancel Cnf
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePortalHttpCancelCnf(DPARAM param);

/*****************************************************************************/
//  Description : Handle Http Net Prog Ind
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePortalHttpNetProgInd(DPARAM param);

/*****************************************************************************/
//  Description : Handle Http Redirect Ind
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePortalHttpRedirectInd(DPARAM param);

/*****************************************************************************/
//  Description : Handle Http Redirect Ind
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePortalHttpPostCnf(DPARAM param);

/*****************************************************************************/
//  Description : Handle Portal Get Or Post Cnf
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL void HandlePortalGetPostCnf(HTTP_CONTEXT_ID_T context_id);

#ifdef SSL_SUPPORT
/*****************************************************************************/
//  Description : Handle Http SSL Cert Untrust Ind
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePortalHttpSSLCertUntrustInd(DPARAM param);
#endif

/*****************************************************************************/
//  Description : find action url
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL uint8* WlanPortalFindActionUrl(uint8 * data_ptr, uint32 data_len, uint16 *url_len_ptr, BOOLEAN *is_login_form);

/*****************************************************************************/
//  Description : find Name Value
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL uint8* WlanPortalFindNameValue(uint8 * data_ptr, uint32 data_len, const char *name_ptr, uint16 *acname_len_ptr);

/*****************************************************************************/
//  Description : find Method
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMIWIFI_HTTP_METHOD_E WlanPortalFindMethod(uint8 * data_ptr, uint32 data_len);

/*****************************************************************************/
//  Description : find Login_res
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMIWIFI_PORTAL_LOGIN_RES_E WlanPortalLoginRes(uint8 * data_ptr, uint32 data_len);

/*****************************************************************************/
//  Description : find Offline_res
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMIWIFI_PORTAL_OFFLINE_RES_E WlanPortalOfflineRes(uint8 * data_ptr, uint32 data_len);

/*****************************************************************************/
//  Description : Cat Http data
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL void CatHttpData(uint8 ** dst_data_ptr, uint16 * dst_pos, uint8 *data, uint16 data_len, const char *data_mark, uint16 data_mark_len);

/*****************************************************************************/
//  Description : parse data
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMIWIFI_HTTP_METHOD_E WlanPortalParseXhtmlData(uint8 * data_ptr, uint32 data_len,
                                                                                                        uint8 ** url_ptr, uint16 *url_len_ptr,
                                                                                                        uint8 ** body_ptr, uint16 *body_len_ptr);

/*****************************************************************************/
//  Description : parse data
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMIWIFI_HTTP_METHOD_E WlanPortalParseLoginSuccessData(uint8 * data_ptr, uint32 data_len,
                                                                                                        uint8 ** url_ptr, uint16 *url_len_ptr,
                                                                                                        uint8 ** body_ptr, uint16 *body_len_ptr);                                                                                                        

/*****************************************************************************/
//  Description : Is Url Absolute
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN URL_IsAbsolute(const uint8 * url_ptr);

/**-------------------------------------------------------------------------*
**                         IMPLEMENTATION                                   *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Opera escape '!' '~' '*' '\'' '(' ')' 
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsUnreservedChar(uint8 c)
{
    if( (c>='0'&&c<='9') ||
        (c>='a'&&c<='z') ||
        (c>='A'&&c<='Z') ||
        c=='-' || c=='_' || c=='.'/*|| c=='!' || c=='~' || c=='*' || c=='\''  || c=='(' || c==')'*/) {
        return TRUE;
    }
    return FALSE;
}

/*****************************************************************************/
//  Description : Escape URL
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL uint32 URL_EscapeUrl(uint8 *dst_ptr, uint32 dst_len, const uint8 *url_ptr, char var_type)
{
    uint8 c = 0;
    uint32 ret_len = 0;

    if(PNULL == dst_ptr || PNULL == url_ptr)
    {
        return 0;
    }

    while (0 != (c = *url_ptr++))
    {
        if (IsUnreservedChar(c) || URL_UNESCAPE == var_type)
        {
            if (ret_len + 1 <= dst_len)
            {
                dst_ptr[ret_len ++] = c;
            }
            else
            {
                break;
            }
        }
        else if (' ' == c)
        {
            if (ret_len + 1 <= dst_len)
            {
                dst_ptr[ret_len ++] = '+';
            }
            else
            {
                break;
            }
        }
        else
        {
            char str[10] = {0};
            uint16 len = 0;

            sprintf(str, "%02X", c);
            len = strlen(str);

            if (ret_len + len <= dst_len)
            {
                dst_ptr[ret_len ++] = '%';

                SCI_MEMCPY(dst_ptr+ ret_len, str, len);

                ret_len += len;
            }
            else
            {
                break;
            }
        }
    }

    return ret_len;
}

/*****************************************************************************/
// 	Description : process ps signal
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL void HandleWlanPortalPsMsg(MMIPDP_CNF_INFO_T *msg_ptr)
{
    if (PNULL == msg_ptr)
    {
        return;
    }
    
    //check the param
    //SCI_TRACE_LOW:"HandleWlanPortalPsMsg() enter msg_id= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_492_112_2_18_3_9_44_113,(uint8*)"d", msg_ptr->msg_id);
    
    switch(msg_ptr->msg_id)
    {
    case MMIPDP_ACTIVE_CNF:
        {
            //SCI_TRACE_LOW:"HandleWlanPortalPsMsg: MMIPDP_ACTIVE_CNF nsapi=0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_498_112_2_18_3_9_44_114,(uint8*)"d", msg_ptr->nsapi);

            if (MMIPDP_RESULT_SUCC == msg_ptr->result)
            {
                s_wlan_portal_info.netid = msg_ptr->nsapi;
                s_wlan_portal_info.status = MMIWIFI_PDP_ACTIVE_OK;

                StartPortalHttpRequest(s_wlan_portal_info.netid);
            }
            else
            {
                s_wlan_portal_info.status = MMIWIFI_PDP_DEACTIVE_OK;
#ifdef FDN_SUPPORT
                if(MMIPDP_RESULT_FDN_NOT_PERMIT == msg_ptr->result)
                {
                    MMIWIFI_NotifyPortalFailed(TXT_CC_FDN_ONLY);
                }
                else
#endif        
                {
                    MMIWIFI_NotifyPortalFailed(TXT_DIAL_FAILED);
                }
            }
        }
        break;
        
    case MMIPDP_DEACTIVE_CNF:
        {
            s_wlan_portal_info.status = MMIWIFI_PDP_DEACTIVE_OK;

            //SCI_TRACE_LOW:"HandleWlanPortalPsMsg: MMIPDP_DEACTIVE_CNF"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_528_112_2_18_3_9_44_115,(uint8*)"");
        }
        break;
        
    case MMIPDP_DEACTIVE_IND:
        {
            s_wlan_portal_info.status = MMIWIFI_PDP_DEACTIVE_OK;

            MMIWIFI_NotifyPortalFailed(TXT_COMMON_LINK_DISCONNECTED);

            //SCI_TRACE_LOW:"HandleWlanPortalPsMsg: MMIPDP_DEACTIVE_IND"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_538_112_2_18_3_9_44_116,(uint8*)"");
        }
        break;

    default:
        break;
    }
}

/*****************************************************************************/
//  Description : Get Signal Struct
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL void GetSignalStruct(DPARAM param, void *signal_struct_ptr, uint16 struct_size)
{
    if (PNULL == param || PNULL == signal_struct_ptr || struct_size < sizeof(xSignalHeaderRec))
    {
        return;
    }
    
    SCI_MEMCPY((uint8 *)((uint32)signal_struct_ptr), param, (struct_size));
}

/*****************************************************************************/
//  Description : Get/Post Request
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN PortalRequest(uint8 * url_ptr, uint16 url_len, uint8 * body_ptr, uint16 body_len,
                                            MMIWIFI_HTTP_METHOD_E method, HTTP_CONTEXT_ID_T context_id, HTTP_APP_INSTANCE_T app_instance)
{
    MMIENG_UA_T ua = {0};
    uint8 * portal_url_ptr = PNULL;
    uint16 portal_url_len = 0;
    BOOLEAN is_need_free = FALSE;

    if (PNULL == url_ptr)
    {
        //SCI_TRACE_LOW:"PortalRequest Input Invalid"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_575_112_2_18_3_9_44_117,(uint8*)"");
    
        return FALSE;
    }

    //For Shenzhen WLAN
    if (URL_IsAbsolute(url_ptr))
    {
        portal_url_len = url_len;
        portal_url_ptr = url_ptr;
        //SCI_TRACE_LOW:"PortalRequest method=%d, url_len=%d, body_len=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_584_112_2_18_3_9_44_118,(uint8*)"ddd", method, url_len, body_len);

        is_need_free = FALSE;
    }
    else if (PNULL != s_wlan_portal_info.cur_page_url_ptr && s_wlan_portal_info.cur_page_url_len > 0)
    {
        uint16 i = s_wlan_portal_info.cur_page_url_len - 1;
        uint8 * cur_page_url_ptr = s_wlan_portal_info.cur_page_url_ptr + i;

        if (*url_ptr != '/')
        {
            while (*cur_page_url_ptr && i > 0)
            {
                if ('/' == *cur_page_url_ptr)
                {
                    break;
                }

                cur_page_url_ptr--;

                i --;
            }
        }
        else
        {
            uint16 last_slash_pos = 0;
            
             while (*cur_page_url_ptr && i > 0)
            {
                if ('/' == *cur_page_url_ptr)
                {
                    if ('/' == *(cur_page_url_ptr-1))
                        break;

                    last_slash_pos = i;
                    
                }

                cur_page_url_ptr--;

                i --;
            }

            i = last_slash_pos-1;
        }

        if (i > 0)
        {
            portal_url_len = url_len + (i + 1);
            portal_url_ptr = SCI_ALLOCA(portal_url_len + 1);

            if (PNULL != portal_url_ptr)
            {
                SCI_MEMSET(portal_url_ptr, 0x00, (portal_url_len + 1));

                SCI_MEMCPY(portal_url_ptr, s_wlan_portal_info.cur_page_url_ptr, (i + 1));

                SCI_MEMCPY(portal_url_ptr + (i + 1), url_ptr, url_len);

                is_need_free = TRUE;
            }
            else
            {
                return FALSE;
            }
        }
        else
        {
            //SCI_TRACE_LOW:"PortalRequest url error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_652_112_2_18_3_9_44_119,(uint8*)"");

            return FALSE;
        }
    }
    else
    {
        portal_url_len = url_len;

        portal_url_ptr = url_ptr;

        is_need_free = FALSE;
    }

    SCI_TRACE_LOW("PortalRequest method=%d, url_len=%d, body_len=%d", method, portal_url_len, body_len);

    if (portal_url_len < 200)
    {
        SCI_TRACE_LOW("PortalRequest portal_url_ptr = %s", portal_url_ptr);
    }
    else
    {
        SCI_TRACE_BUF("portal_url_ptr=", portal_url_ptr, portal_url_len);
    }

    MMIAPIENG_GetUa(&ua);

    if (MMIWIFI_HTTP_METHOD_POST == method)
    {
        HTTP_POST_PARAM_T *post_param_ptr = PNULL;

        post_param_ptr = SCI_ALLOCA(sizeof(HTTP_POST_PARAM_T));

        if(PNULL == post_param_ptr)
        {
            //SCI_TRACE_LOW:"PortalRequest no memory"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_676_112_2_18_3_9_45_121,(uint8*)"");

            if (is_need_free)
            {
                SCI_FREE(portal_url_ptr);
            }

            return FALSE;
        }

        SCI_MEMSET(post_param_ptr, 0x00, sizeof(HTTP_POST_PARAM_T));

        if (s_wlan_portal_info.login_type != MMIWIFI_LOGINTYPE_GD)  //gongdong needn't user agent.
        {
            if (0 != SCI_STRLEN((char *)ua.user_agent))
            {
                post_param_ptr->user_agent.user_agent_ptr = (char *)ua.user_agent;
            }
            else
            {
                post_param_ptr->user_agent.user_agent_ptr = (char *)MMIAPICOM_GetUserAgent();
            }
        }

        post_param_ptr->accept.accept_ptr = ACCEPT_STRING;

        post_param_ptr->connection = HTTP_CONNECTION_CLOSE;

        post_param_ptr->uri.uri_ptr = (char *)portal_url_ptr;

        post_param_ptr->accept_encoding = HTTP_ENCODING_TYPE_NULL;
        post_param_ptr->get_data.style = HTTP_DATA_TRANS_STYLE_BUFFER;

        post_param_ptr->get_data.is_header_use_file = FALSE;
        post_param_ptr->need_net_prog_ind = FALSE;
        post_param_ptr->is_use_relative_url = TRUE;

        post_param_ptr->post_body.is_use_file = FALSE;

        if (s_wlan_portal_info.cookie_ptr != PNULL)
        {
            post_param_ptr->extern_header.other_header_ptr = SCI_ALLOC_APPZ(sizeof(HTTP_OTHER_HEADER_T));
            post_param_ptr->extern_header.header_num = 1;
            post_param_ptr->extern_header.other_header_ptr->header_name_ptr = "Cookie";
            post_param_ptr->extern_header.other_header_ptr->header_value_ptr = (char *)s_wlan_portal_info.cookie_ptr;//SCI_ALLOC_APP(strlen(s_cookie)+2);
        }
        
        if (PNULL != body_ptr)
        {
            post_param_ptr->post_body.u.post_buffer.buffer_ptr = body_ptr;
            post_param_ptr->post_body.u.post_buffer.buffer_len = body_len;
            post_param_ptr->post_body.u.post_buffer.is_copied_by_http = TRUE;
        }

        HTTP_PostRequest(context_id, post_param_ptr, app_instance);

        if (post_param_ptr->extern_header.other_header_ptr != PNULL)
            SCI_FREE(post_param_ptr->extern_header.other_header_ptr);
        SCI_FREE(post_param_ptr);
    }
    else
    {
        HTTP_GET_PARAM_T* get_param_ptr = PNULL;
        uint8 * temp_url_ptr  = PNULL;
        uint16 temp_url_len = 0;
        uint16 question_mark_len = 0;

        get_param_ptr = SCI_ALLOCA(sizeof(HTTP_GET_PARAM_T));

        if(PNULL == get_param_ptr)
        {
            //SCI_TRACE_LOW:"PortalRequest no memory"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_748_112_2_18_3_9_45_122,(uint8*)"");

            if (is_need_free)
            {
                SCI_FREE(portal_url_ptr);
            }

            return FALSE;
        }

        SCI_MEMSET(get_param_ptr, 0x00, sizeof(HTTP_GET_PARAM_T));

        if (0 != SCI_STRLEN((char *)ua.user_agent))
        {
            get_param_ptr->user_agent.user_agent_ptr = (char *)ua.user_agent;
        }
        else
        {
            get_param_ptr->user_agent.user_agent_ptr = (char *)MMIAPICOM_GetUserAgent();
        }

        get_param_ptr->accept.accept_ptr = ACCEPT_STRING;

        get_param_ptr->connection = HTTP_CONNECTION_CLOSE;

        if (PNULL != body_ptr)
        {
            question_mark_len = strlen((char *)MMIWLAN_HTTP_QUESTION_MARK);

            temp_url_len = portal_url_len + question_mark_len + body_len;

            temp_url_ptr = SCI_ALLOCA(temp_url_len + 1);

            if (PNULL != temp_url_ptr)
            {
                SCI_MEMSET(temp_url_ptr, 0x00, (temp_url_len + 1));

                SCI_MEMCPY(temp_url_ptr, portal_url_ptr, portal_url_len);
                SCI_MEMCPY(temp_url_ptr + portal_url_len, (uint8 *)MMIWLAN_HTTP_QUESTION_MARK, question_mark_len);
                SCI_MEMCPY(temp_url_ptr + portal_url_len + question_mark_len, body_ptr, body_len);

                get_param_ptr->uri.uri_ptr = (char *)temp_url_ptr;
            }
            else
            {
                //SCI_TRACE_LOW:"PortalRequest no memory"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_793_112_2_18_3_9_45_123,(uint8*)"");

                if (is_need_free)
                {
                    SCI_FREE(portal_url_ptr);
                }

                return FALSE;
            }
        }
        else
        {
            get_param_ptr->uri.uri_ptr = (char *)portal_url_ptr;
        }

        get_param_ptr->accept_encoding = HTTP_ENCODING_TYPE_NULL;
        get_param_ptr->get_data.style = HTTP_DATA_TRANS_STYLE_BUFFER;

        get_param_ptr->get_data.is_header_use_file = FALSE;
        get_param_ptr->need_net_prog_ind = FALSE;
        get_param_ptr->is_use_relative_url = TRUE;

        HTTP_GetRequest(context_id, get_param_ptr, app_instance);

        SCI_FREE(get_param_ptr);

        if (PNULL != temp_url_ptr)
        {
            SCI_FREE(temp_url_ptr);
        }
    }

    if (PNULL != s_wlan_portal_info.cur_page_url_ptr)
    {
        SCI_FREE(s_wlan_portal_info.cur_page_url_ptr);
        s_wlan_portal_info.cur_page_url_ptr = PNULL;
    }

    s_wlan_portal_info.cur_page_url_len = portal_url_len;
    s_wlan_portal_info.cur_page_url_ptr = SCI_ALLOCA(portal_url_len + 1);

    if (PNULL != s_wlan_portal_info.cur_page_url_ptr)
    {
        SCI_MEMSET(s_wlan_portal_info.cur_page_url_ptr, 0x00, (portal_url_len + 1));

        SCI_MEMCPY(s_wlan_portal_info.cur_page_url_ptr, portal_url_ptr, portal_url_len);
    }
    else
    {
        if (is_need_free)
        {
            SCI_FREE(portal_url_ptr);
        }

        return FALSE;
    }

    if (is_need_free)
    {
        SCI_FREE(portal_url_ptr);
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : Handle Http Init Cnf
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePortalHttpInitCnf(DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    HTTP_INIT_CNF_SIG_T signal = {0};
    HTTP_APP_MODULE_ID_T module_id = 0;
    HTTP_APP_PARAM_T app_param = 0;
    HTTP_CONTEXT_ID_T context_id = 0;
    uint32 app_instance = 0;
    uint16 url_len = 0;

    GetSignalStruct(param, &signal, sizeof(HTTP_INIT_CNF_SIG_T));

    module_id = signal.module_id;
    app_param = signal.app_param;
    context_id = signal.context_id;

    if (MMI_MODULE_WIFI != module_id)
    {
        //SCI_TRACE_LOW:"HandlePortalHttpInitCnf not Portal task"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_880_112_2_18_3_9_45_124,(uint8*)"");

        return MMI_RESULT_FALSE;
    }

    s_wlan_portal_info.context_id = context_id;
    s_wlan_portal_info.app_instance = app_instance;

    url_len = strlen((char *)MMIWLAN_PORTAL_URL);

    PortalRequest((uint8 *)MMIWLAN_PORTAL_URL, url_len, PNULL, 0, MMIWIFI_HTTP_METHOD_GET, context_id, app_instance);

    return result;
}

/*****************************************************************************/
//  Description : Handle Http Request Id Ind
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePortalHttpRequestIdInd(DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    HTTP_REQUEST_ID_IND_SIG_T signal = {0};
    HTTP_CONTEXT_ID_T context_id = 0;
    HTTP_REQUEST_ID_T request_id = 0;
    HTTP_APP_INSTANCE_T app_instance = 0;

    GetSignalStruct(param, &signal, sizeof(HTTP_REQUEST_ID_IND_SIG_T));

    context_id = signal.context_id;
    request_id = signal.request_id;
    app_instance = signal.app_instance;

    if (context_id != s_wlan_portal_info.context_id)
    {
        return MMI_RESULT_FALSE;
    }

    s_wlan_portal_info.request_id = request_id;
    s_wlan_portal_info.app_instance = app_instance;

    return result;
}

/*****************************************************************************/
//  Description : Handle Http Need Auth Ind
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePortalHttpNeedAuthInd(DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    HTTP_AUTH_IND_SIG_T signal = {0};
    HTTP_CONTEXT_ID_T context_id = 0;

    GetSignalStruct(param, &signal, sizeof(HTTP_AUTH_IND_SIG_T));

    context_id = signal.context_id;

    if (context_id != s_wlan_portal_info.context_id)
    {
        return MMI_RESULT_FALSE;
    }

    return result;
}

/*****************************************************************************/
//  Description : Handle Http Get Cnf
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePortalHttpGetCnf(DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    HTTP_GET_CNF_SIG_T signal = {0};
    HTTP_CONTEXT_ID_T context_id = 0;
    GetSignalStruct(param, &signal, sizeof(HTTP_GET_CNF_SIG_T));

    context_id = signal.context_id;

    if (context_id != s_wlan_portal_info.context_id)
    {
        return MMI_RESULT_FALSE;
    }

    HandlePortalGetPostCnf(context_id);

    return result;
}

/*****************************************************************************/
//  Description : Handle Http Head Cnf
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePortalHttpHeadCnf(DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    HTTP_HEAD_CNF_SIG_T signal = {0};
    HTTP_CONTEXT_ID_T context_id = 0;

    GetSignalStruct(param, &signal, sizeof(HTTP_HEAD_CNF_SIG_T));

    context_id = signal.context_id;

    if (context_id != s_wlan_portal_info.context_id)
    {
        return MMI_RESULT_FALSE;
    }

    HTTP_CloseRequest(context_id);

    MMIWIFI_WlanPortalNetDisconnect();

    return result;
}

/*****************************************************************************/
//  Description : Handle Http Data Ind
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePortalHttpDataInd(DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    HTTP_DATA_IND_SIG_T signal = {0};
    HTTP_REQUEST_ID_T request_id = 0;
    HTTP_CONTEXT_ID_T context_id = 0;
    uint32 response_code = 0;
    uint32 data_len = 0;
    uint8 *data_ptr = PNULL;

    GetSignalStruct(param, &signal, sizeof(HTTP_DATA_IND_SIG_T));

    request_id = signal.request_id;
    context_id = signal.context_id;
    response_code = signal.response_code;
    data_len = signal.data_len;
    data_ptr = signal.data_ptr;

    if (context_id != s_wlan_portal_info.context_id)
    {
        return MMI_RESULT_FALSE;
    }

    //SCI_TRACE_LOW:"HandlePortalHttpDataInd recv_len=%d, total_len=%d, data_len=%d, response_code=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_1023_112_2_18_3_9_45_125,(uint8*)"dddd",s_wlan_portal_info.recv_len, s_wlan_portal_info.data_len, data_len, response_code);

    if (MMIWLAN_HTTP_STATUS_CODE_SUCCESS == response_code
        || MMIWLAN_HTTP_STATUS_CODE_PARTIAL_CONTENT == response_code
        || MMIWLAN_HTTP_STATUS_CODE_MOVED_PERMANENTLY == response_code
        || MMIWLAN_HTTP_STATUS_CODE_FOUND == response_code
        || MMIWLAN_HTTP_STATUS_CODE_TEMPORARY_REDIRECT == response_code
        || MMIWLAN_HTTP_STATUS_CODE_UNAUTHORIZED == response_code
        || MMIWLAN_HTTP_STATUS_CODE_UTHENTICATION_REQUIRED == response_code)
    {
        if (s_wlan_portal_info.recv_len + data_len <= s_wlan_portal_info.data_len)
        {
            SCI_MEMCPY(s_wlan_portal_info.data_ptr + s_wlan_portal_info.recv_len, data_ptr, data_len);
            s_wlan_portal_info.recv_len += data_len;
        }

        s_wlan_portal_info.response_code = response_code;
    }

    return result;
}



LOCAL void get_cookie(char* header)
{
    uint8* cookie_str_begin = PNULL;
    uint8* cookie_str_end = PNULL;
    uint16 cookie_len=0;
    char* cookie_mark="Set-Cookie: ";
    //uint8* dest_ptr = s_cookie;
    cookie_str_begin = (uint8 *)MMIAPICOM_Stristr((char *)header, cookie_mark);

    if (PNULL == cookie_str_begin)
    {
        return;
    }

    if (s_wlan_portal_info.cookie_ptr != PNULL)
    {
        SCI_FREE(s_wlan_portal_info.cookie_ptr);
        s_wlan_portal_info.cookie_ptr = PNULL;
    }

    cookie_str_begin += strlen(cookie_mark);

    cookie_str_end = (uint8 *)strchr((char *)cookie_str_begin,';');

    if (PNULL == cookie_str_end)
    {
        cookie_str_end = (uint8 *)strchr((char *)cookie_str_begin,'\r');

        if (PNULL == cookie_str_end)
        {
            return;
        }
    }

    cookie_len = cookie_str_end-cookie_str_begin;

    s_wlan_portal_info.cookie_ptr = SCI_ALLOC_APPZ(cookie_len+1);

    SCI_MEMCPY(s_wlan_portal_info.cookie_ptr,cookie_str_begin,cookie_len);

}

/*****************************************************************************/
//  Description : Handle Http Header Ind
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePortalHttpHeaderInd(DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    HTTP_HEADER_IND_SIG_T signal = {0};
    HTTP_REQUEST_ID_T request_id = 0;
    HTTP_CONTEXT_ID_T context_id = 0;
    uint32 app_instance = 0;
    uint32 response_code = 0;
    int32 content_len = 0;

    GetSignalStruct(param, &signal, sizeof(HTTP_HEADER_IND_SIG_T));

    request_id = signal.request_id;
    context_id = signal.context_id;
    app_instance = signal.app_instance;
    response_code = signal.rsp_header_info.response_code;
    content_len = signal.rsp_header_info.content_length;

    if (context_id != s_wlan_portal_info.context_id)
    {
        return MMI_RESULT_FALSE;
    }

    //SCI_TRACE_LOW:"HandlePortalHttpHeaderInd response_code=%d, content_len=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_1109_112_2_18_3_9_46_126,(uint8*)"dd", response_code, content_len);


    get_cookie(signal.header_ptr);

#if 0
    if (MMIWLAN_HTTP_STATUS_CODE_MOVED_PERMANENTLY == response_code
        || MMIWLAN_HTTP_STATUS_CODE_FOUND == response_code
        || MMIWLAN_HTTP_STATUS_CODE_TEMPORARY_REDIRECT == response_code
        || MMIWLAN_HTTP_STATUS_CODE_UNAUTHORIZED == response_code
        || MMIWLAN_HTTP_STATUS_CODE_UTHENTICATION_REQUIRED == response_code)
    {
        return MMI_RESULT_TRUE;
    }
#endif

    if (PNULL != s_wlan_portal_info.data_ptr)
    {
        SCI_FREE(s_wlan_portal_info.data_ptr);
        s_wlan_portal_info.data_ptr = PNULL;
    }

    s_wlan_portal_info.recv_len = 0;
    if (content_len == -1) content_len = MMIWLAN_PORTAL_DATA_LENGTH_MAX; //sometimes there is NO content length specified.
    s_wlan_portal_info.data_len = MIN(content_len, MMIWLAN_PORTAL_DATA_LENGTH_MAX);
    s_wlan_portal_info.data_ptr = SCI_ALLOCA(s_wlan_portal_info.data_len + 1);

    if (PNULL != s_wlan_portal_info.data_ptr)
    {
        SCI_MEMSET(s_wlan_portal_info.data_ptr, 0x00, (s_wlan_portal_info.data_len + 1));
    }
    else
    {
        HTTP_CloseRequest(context_id);

        MMIWIFI_WlanPortalNetDisconnect();

        //SCI_TRACE_LOW:"HandlePortalHttpHeaderInd No Memory"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_1146_112_2_18_3_9_46_127,(uint8*)"");
    }

    return result;
}

/*****************************************************************************/
//  Description : Handle Http Error Ind
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePortalHttpErrorInd(DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    HTTP_ERROR_IND_SIG_T signal = {0};
    HTTP_CONTEXT_ID_T context_id = 0;
    MMIWLAN_PORTAL_STATUS_E portal_status = MMIWIFI_GetPortalStatus();

    GetSignalStruct(param, &signal, sizeof(HTTP_ERROR_IND_SIG_T));

    context_id = signal.context_id;

    if (context_id != s_wlan_portal_info.context_id)
    {
        return MMI_RESULT_FALSE;
    }

    HTTP_CloseRequest(context_id);

    if (MMIWLAN_PORTAL_STATUS_LOGOUTING == portal_status)
    {
        MMIWIFI_HandlePortalOffline(MMIWIFI_PORTAL_OFFLINE_FAILED);
    }
    else
    {
        MMIWIFI_HandlePortalLogin(MMIWIFI_PORTAL_LOGIN_FAILED);

        MMIWIFI_WlanPortalNetDisconnect();
    }

     return result;
}

/*****************************************************************************/
//  Description : Handle Http Close Cnf
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePortalHttpCloseCnf(DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    HTTP_CLOSE_CNF_SIG_T signal = {0};
    HTTP_CONTEXT_ID_T context_id = 0;

    GetSignalStruct(param, &signal, sizeof(HTTP_CLOSE_CNF_SIG_T));

    context_id = signal.context_id;

    if (context_id != s_wlan_portal_info.context_id)
    {
        return MMI_RESULT_FALSE;
    }

    s_wlan_portal_info.context_id = 0;

    if (PNULL != s_wlan_portal_info.data_ptr)
    {
        SCI_FREE(s_wlan_portal_info.data_ptr);
        s_wlan_portal_info.data_ptr = PNULL;
    }

    if (PNULL != s_wlan_portal_info.logout_url.url_ptr)
    {
        SCI_FREE(s_wlan_portal_info.logout_url.url_ptr);
        s_wlan_portal_info.logout_url.url_ptr = PNULL;
    }

    if (PNULL != s_wlan_portal_info.logout_url.body_ptr)
    {
        SCI_FREE(s_wlan_portal_info.logout_url.body_ptr);
        s_wlan_portal_info.logout_url.body_ptr= PNULL;
    }

    //for Shenzhen WLAN
    if (PNULL != s_wlan_portal_info.cur_page_url_ptr)
    {
        SCI_FREE(s_wlan_portal_info.cur_page_url_ptr);
        s_wlan_portal_info.cur_page_url_ptr = PNULL;
    }

    SCI_MEMSET(&s_wlan_portal_info, 0x00, sizeof(s_wlan_portal_info));

    return result;
}

/*****************************************************************************/
//  Description : Handle Http Cancel Cnf
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePortalHttpCancelCnf(DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    HTTP_CANCEL_CNF_SIG_T signal = {0};
    HTTP_CONTEXT_ID_T context_id = 0;

    GetSignalStruct(param, &signal, sizeof(HTTP_CANCEL_CNF_SIG_T));

    context_id = signal.context_id;

    if (context_id != s_wlan_portal_info.context_id)
    {
        return MMI_RESULT_FALSE;
    }

    HTTP_CloseRequest(context_id);

    MMIWIFI_WlanPortalNetDisconnect();

    return result;
}

/*****************************************************************************/
//  Description : Handle Http Net Prog Ind
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePortalHttpNetProgInd(DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    HTTP_NET_PROG_IND_SIG_T signal = {0};
    HTTP_CONTEXT_ID_T context_id = 0;
    NET_PROGRESS_E net_prog_op_code = NET_PROGRESS_CONNECTING;
    uint32 content_len = 0;
    uint32 current_len = 0;

    GetSignalStruct(param, &signal, sizeof(HTTP_NET_PROG_IND_SIG_T));

    context_id = signal.context_id;
    net_prog_op_code = signal.net_prog_op_code;
    content_len = signal.content_len;
    current_len = signal.current_len;

    if (context_id != s_wlan_portal_info.context_id)
    {
        return MMI_RESULT_FALSE;
    }

    if (NET_PROGRESS_RECEIVING != net_prog_op_code)
    {
        return result;
    }

    return result;
}

/*****************************************************************************/
//  Description : Handle Http Redirect Ind
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePortalHttpRedirectInd(DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    HTTP_REDIRECT_IND_SIG_T signal = {0};
    HTTP_CONTEXT_ID_T context_id = 0;

    GetSignalStruct(param, &signal, sizeof(HTTP_REDIRECT_IND_SIG_T));

    context_id = signal.context_id;

    if (context_id != s_wlan_portal_info.context_id)
    {
        return MMI_RESULT_FALSE;
    }

    //for Shenzhen WLAN
    if (PNULL != signal.redirect_ptr)
    {
        uint16 redirect_url_len = 0;
    
        if (PNULL != s_wlan_portal_info.cur_page_url_ptr)
        {
            SCI_FREE(s_wlan_portal_info.cur_page_url_ptr);
            s_wlan_portal_info.cur_page_url_ptr = PNULL;
        }

        redirect_url_len = strlen(signal.redirect_ptr);

        s_wlan_portal_info.cur_page_url_len = redirect_url_len;
        s_wlan_portal_info.cur_page_url_ptr = SCI_ALLOCA(redirect_url_len + 1);

        if (PNULL != s_wlan_portal_info.cur_page_url_ptr)
        {
            SCI_MEMSET(s_wlan_portal_info.cur_page_url_ptr, 0x00, (redirect_url_len + 1));

            SCI_MEMCPY(s_wlan_portal_info.cur_page_url_ptr, signal.redirect_ptr, redirect_url_len);
        }

        //SCI_TRACE_LOW:"HandlePortalHttpRedirectInd cur_page_url_ptr=%s"
        //SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_1340_112_2_18_3_9_46_128,(uint8*)"s", s_wlan_portal_info.cur_page_url_ptr);
    }

    return result;
}

/*****************************************************************************/
//  Description : Handle Http Redirect Ind
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePortalHttpPostCnf(DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    HTTP_POST_CNF_SIG_T signal = {0};
    HTTP_CONTEXT_ID_T context_id = 0;

    GetSignalStruct(param, &signal, sizeof(HTTP_POST_CNF_SIG_T));

    context_id = signal.context_id;

    if (context_id != s_wlan_portal_info.context_id)
    {
        return MMI_RESULT_FALSE;
    }

    HandlePortalGetPostCnf(context_id);

    return result;
}

/*****************************************************************************/
//  Description : Get Login Type
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMIWIFI_LOGIN_TYPE_E WlanPortal_GetLoginType(uint8 * data_ptr, uint32 data_len)
{
    uint8 * dest_str = PNULL;

    MMIWIFI_LOGIN_TYPE_E login_type = MMIWIFI_LOGINTYPE_NORMAL;

    if (s_wlan_portal_info.login_type != MMIWIFI_LOGINTYPE_NONE)
    {
        return s_wlan_portal_info.login_type;
    }

    if (PNULL == data_ptr)
    {
        return MMIWIFI_LOGINTYPE_NONE;
    }

    dest_str = (uint8 *)MMIAPICOM_Stristr((char *)data_ptr, "guangdong");

    if (PNULL != dest_str)
    {
        login_type =  MMIWIFI_LOGINTYPE_GD;
    }

    //SCI_TRACE_LOW:"JXC wlan login type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_1387_112_2_18_3_9_46_129,(uint8*)"d",login_type);

    return login_type;
}

/*****************************************************************************/
//  Description : Handle Portal Get Or Post Cnf
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL void HandlePortalGetPostCnf(HTTP_CONTEXT_ID_T context_id)
{
    MMIWIFI_HTTP_METHOD_E http_method = MMIWIFI_HTTP_METHOD_NONE;
    uint8 *next_url_ptr = PNULL;
    uint16 url_len = 0;
    uint8 *body_ptr = PNULL;
    uint16 body_len = 0;
    MMIWIFI_PORTAL_LOGIN_RES_E login_res = MMIWIFI_PORTAL_LOGIN_MAX;
    MMIWIFI_PORTAL_OFFLINE_RES_E offline_res = MMIWIFI_PORTAL_OFFLINE_MAX;

    login_res = WlanPortalLoginRes(s_wlan_portal_info.data_ptr, s_wlan_portal_info.recv_len);
    offline_res = WlanPortalOfflineRes(s_wlan_portal_info.data_ptr, s_wlan_portal_info.recv_len);

    s_wlan_portal_info.login_type = WlanPortal_GetLoginType(s_wlan_portal_info.data_ptr, s_wlan_portal_info.recv_len);

    //SCI_TRACE_LOW:"HandlePortalHttpPostCnf login_res=%d, offline_res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_1411_112_2_18_3_9_46_130,(uint8*)"dd", login_res, offline_res);

    if (MMIWIFI_PORTAL_OFFLINE_MAX != offline_res)//offline
    {
        HTTP_CloseRequest(context_id);

        MMIWIFI_HandlePortalOffline(offline_res);

        return;
    }

    if ((s_wlan_portal_info.login_type == MMIWIFI_LOGINTYPE_GD) && (login_res == MMIWIFI_PORTAL_LOGIN_SUCCESS))
    {
        http_method = WlanPortalParseLoginSuccessData(s_wlan_portal_info.data_ptr, s_wlan_portal_info.recv_len,
                                                                    &next_url_ptr, &url_len, &body_ptr, &body_len);    
    }
    else
    {
        http_method = WlanPortalParseXhtmlData(s_wlan_portal_info.data_ptr, s_wlan_portal_info.recv_len,
                                                                    &next_url_ptr, &url_len, &body_ptr, &body_len);
    }
    
    if (PNULL != next_url_ptr)
    {
        if (MMIWIFI_PORTAL_LOGIN_MAX != login_res)
        {
            MMIWIFI_HandlePortalLogin(login_res);

            if (MMIWIFI_PORTAL_LOGIN_SUCCESS == login_res)
            {
                s_wlan_portal_info.logout_url.method = http_method;
            
                //record logout url
                if (PNULL != s_wlan_portal_info.logout_url.url_ptr)
                {
                    SCI_FREE(s_wlan_portal_info.logout_url.url_ptr);
                    s_wlan_portal_info.logout_url.url_ptr = PNULL;
                }

                s_wlan_portal_info.logout_url.url_len = url_len;
                s_wlan_portal_info.logout_url.url_ptr = SCI_ALLOCA(url_len + 1);

                if (PNULL != s_wlan_portal_info.logout_url.url_ptr)
                {
                    SCI_MEMSET(s_wlan_portal_info.logout_url.url_ptr, 0x00, (url_len + 1));

                    SCI_MEMCPY(s_wlan_portal_info.logout_url.url_ptr, next_url_ptr, url_len);
                }
                else
                {
                    HTTP_CloseRequest(context_id);

                    MMIWIFI_WlanPortalNetDisconnect();

                    return;
                }

                //record logout body
                if (PNULL != s_wlan_portal_info.logout_url.body_ptr)
                {
                    SCI_FREE(s_wlan_portal_info.logout_url.body_ptr);
                    s_wlan_portal_info.logout_url.body_ptr= PNULL;
                }

                s_wlan_portal_info.logout_url.body_len = body_len;
                s_wlan_portal_info.logout_url.body_ptr = SCI_ALLOCA(body_len + 1);

                if (PNULL != s_wlan_portal_info.logout_url.body_ptr)
                {
                    SCI_MEMSET(s_wlan_portal_info.logout_url.body_ptr, 0x00, (body_len + 1));

                    SCI_MEMCPY(s_wlan_portal_info.logout_url.body_ptr, body_ptr, body_len);
                }
                else
                {
                    HTTP_CloseRequest(context_id);

                    MMIWIFI_WlanPortalNetDisconnect();

                    return;
                }
            }
            else
            {
                HTTP_CloseRequest(context_id);

                MMIWIFI_WlanPortalNetDisconnect();
            }
        }
        else
        {
            PortalRequest(next_url_ptr, url_len, body_ptr, body_len, http_method, s_wlan_portal_info.context_id, s_wlan_portal_info.app_instance);
        }

        SCI_FREE(next_url_ptr);
        next_url_ptr = PNULL;
    }
    else
    {
        //SCI_TRACE_LOW:"HandlePortalHttpPostCnf url = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_1510_112_2_18_3_9_47_131,(uint8*)"d", s_wlan_portal_info.logout_url.url_ptr);

        if (PNULL == s_wlan_portal_info.logout_url.url_ptr) /* Login by WEB Portal */
        {
            if (MMIWLAN_HTTP_STATUS_CODE_SUCCESS == s_wlan_portal_info.response_code
                && 0 != s_wlan_portal_info.recv_len)
            {
                MMIPUB_OpenAlertWarningWin( TXT_WLAN_PORTAL_HAS_PORTALED);

                //MMIWIFI_HandlePortalLogin(MMIWIFI_PORTAL_LOGIN_FAILED);
            }
            else
            {
                MMIWIFI_HandlePortalLogin(MMIWIFI_PORTAL_LOGIN_FAILED);
            }

            HTTP_CloseRequest(context_id);

            MMIWIFI_WlanPortalNetDisconnect();
        }
        else
        {
            MMIWIFI_StartPortalStatusTimer();
            MMIAPIWIFI_OpenPortalStatusWin();
            MMIPUB_OpenAlertSuccessWin( TXT_WLAN_PORTAL_SUCCESS);
        }

        MMK_CloseWin(MMIWLAN_PORTAL_WAIT_WIN_ID);
    }

    if (PNULL != body_ptr)
    {
        SCI_FREE(body_ptr);

        body_ptr = PNULL;
    }
}

#ifdef SSL_SUPPORT
/*****************************************************************************/
//  Description : Handle Http SSL Cert Untrust Ind
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePortalHttpSSLCertUntrustInd(DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    HTTP_SSL_CERT_UNTRUST_IND_SIG_T signal = {0};
    HTTP_CONTEXT_ID_T context_id = 0;

    GetSignalStruct(param, &signal, sizeof(HTTP_SSL_CERT_UNTRUST_IND_SIG_T));

    context_id = signal.context_id;

    if (context_id != s_wlan_portal_info.context_id)
    {
        return MMI_RESULT_FALSE;
    }

    result = HTTP_SslCertResponse(context_id, s_wlan_portal_info.app_instance, s_wlan_portal_info.request_id, TRUE);

    return result;
}
#endif

/*****************************************************************************/
//  Description : Portal Offline
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_PortalOffline(void)
{
     return PortalRequest(s_wlan_portal_info.logout_url.url_ptr, s_wlan_portal_info.logout_url.url_len,
                            s_wlan_portal_info.logout_url.body_ptr, s_wlan_portal_info.logout_url.body_len,
                            s_wlan_portal_info.logout_url.method, s_wlan_portal_info.context_id, s_wlan_portal_info.app_instance);
}

/*****************************************************************************/
//  Description : Handle Http Msg
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIWIFI_HandleHttpMsg(uint16 msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case HTTP_SIG_INIT_CNF:
        //SCI_TRACE_LOW:"MMIWIFI_HandleHttpMsg receive HTTP_SIG_INIT_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_1595_112_2_18_3_9_47_132,(uint8*)"");
        result = HandlePortalHttpInitCnf(param);
        break;

    case HTTP_SIG_REQUEST_ID_IND:
        //SCI_TRACE_LOW:"MMIWIFI_HandleHttpMsg receive HTTP_SIG_REQUEST_ID_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_1600_112_2_18_3_9_47_133,(uint8*)"");
        result = HandlePortalHttpRequestIdInd(param);
        break;

    case HTTP_SIG_NEED_AUTH_IND:
        //SCI_TRACE_LOW:"MMIWIFI_HandleHttpMsg receive HTTP_SIG_NEED_AUTH_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_1605_112_2_18_3_9_47_134,(uint8*)"");
        result = HandlePortalHttpNeedAuthInd(param);
        break;

    case HTTP_SIG_GET_CNF:
        //SCI_TRACE_LOW:"MMIWIFI_HandleHttpMsg receive HTTP_SIG_GET_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_1610_112_2_18_3_9_47_135,(uint8*)"");
        result = HandlePortalHttpGetCnf(param);
        break;

    case HTTP_SIG_HEAD_CNF:
        //SCI_TRACE_LOW:"MMIWIFI_HandleHttpMsg receive HTTP_SIG_HEAD_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_1615_112_2_18_3_9_47_136,(uint8*)"");
        result = HandlePortalHttpHeadCnf(param);
        break;

    case HTTP_SIG_DATA_IND:
        //SCI_TRACE_LOW:"MMIWIFI_HandleHttpMsg receive HTTP_SIG_DATA_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_1620_112_2_18_3_9_47_137,(uint8*)"");
        result = HandlePortalHttpDataInd(param);
        break;

    case HTTP_SIG_HEADER_IND:
        //SCI_TRACE_LOW:"MMIWIFI_HandleHttpMsg receive HTTP_SIG_HEADER_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_1625_112_2_18_3_9_47_138,(uint8*)"");
        result = HandlePortalHttpHeaderInd(param);
        break;

    case HTTP_SIG_ERROR_IND:
        //SCI_TRACE_LOW:"MMIWIFI_HandleHttpMsg receive HTTP_SIG_ERROR_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_1630_112_2_18_3_9_47_139,(uint8*)"");
        result = HandlePortalHttpErrorInd(param);
        break;

    case HTTP_SIG_CLOSE_CNF:
        //SCI_TRACE_LOW:"MMIWIFI_HandleHttpMsg receive HTTP_SIG_CLOSE_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_1635_112_2_18_3_9_47_140,(uint8*)"");
        result = HandlePortalHttpCloseCnf(param);
        break;

    case HTTP_SIG_CANCEL_CNF:
        //SCI_TRACE_LOW:"MMIWIFI_HandleHttpMsg receive HTTP_SIG_CANCEL_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_1640_112_2_18_3_9_47_141,(uint8*)"");
        result = HandlePortalHttpCancelCnf(param);
        break;

    case HTTP_SIG_NET_PROG_IND:
        //SCI_TRACE_LOW:"MMIWIFI_HandleHttpMsg receive HTTP_SIG_NET_PROG_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_1645_112_2_18_3_9_47_142,(uint8*)"");
        result = HandlePortalHttpNetProgInd(param);
        break;

    case HTTP_SIG_REDIRECT_IND:
        //SCI_TRACE_LOW:"MMIWIFI_HandleHttpMsg receive HTTP_SIG_REDIRECT_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_1650_112_2_18_3_9_47_143,(uint8*)"");
        result = HandlePortalHttpRedirectInd(param);
        break;

    case HTTP_SIG_POST_CNF:
        //SCI_TRACE_LOW:"MMIWIFI_HandleHttpMsg receive HTTP_SIG_POST_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_1655_112_2_18_3_9_47_144,(uint8*)"");
        result = HandlePortalHttpPostCnf(param);
        break;

#ifdef SSL_SUPPORT
    case HTTP_SIG_SSL_CERT_UNTRUST_IND:
        //SCI_TRACE_LOW:"MMIWIFI_HandleHttpMsg receive HTTP_SIG_SSL_CERT_UNTRUST_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_1661_112_2_18_3_9_47_145,(uint8*)"");
        result = HandlePortalHttpSSLCertUntrustInd(param);
        break;
#endif

    default:
        //SCI_TRACE_LOW:"MMIWIFI_HandleHttpMsg Not handle msg_id = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_1667_112_2_18_3_9_47_146,(uint8*)"d", msg_id);
        result = MMI_RESULT_FALSE;
        break;
    }

    return result;
}

/*****************************************************************************/
//  Description : Start Http Request
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN StartPortalHttpRequest(uint32 net_id)
{
    HTTP_INIT_PARAM_T* init_param_ptr = PNULL;
    HTTP_ERROR_E http_error = HTTP_SUCCESS;
    MMIWLAN_PROXY_SETTING_DETAIL_T detail_struct = {0};
    
    init_param_ptr = SCI_ALLOCA(sizeof(HTTP_INIT_PARAM_T));

    if (PNULL == init_param_ptr)
    {
        //SCI_TRACE_LOW:"StartPortalHttpRequest No Memory"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_1689_112_2_18_3_9_47_147,(uint8*)"");
        return FALSE;
    }

    SCI_MEMSET(init_param_ptr, 0, sizeof(HTTP_INIT_PARAM_T));
    init_param_ptr->is_cookie_enable = FALSE;
    init_param_ptr->is_cache_enable = FALSE;
    init_param_ptr->net_id = net_id;

    MMIAPIWIFI_GetProxySettingDetail(&detail_struct);

    if (detail_struct.is_use_proxy && !MMIAPIWIFI_IsExistInExcludingList((char *)MMIWLAN_PORTAL_URL))
    {
        init_param_ptr->proxy.is_use_proxy = TRUE;

        init_param_ptr->proxy.user_name_ptr = (char *)detail_struct.username;
        init_param_ptr->proxy.password_ptr = (char *)detail_struct.password;
        init_param_ptr->proxy.proxy_host_ptr = (char *)detail_struct.gateway;
        init_param_ptr->proxy.http_port = detail_struct.port;
    }
    else
    {
        init_param_ptr->proxy.is_use_proxy = FALSE;
    }

    http_error = HTTP_InitRequest(MMI_MODULE_WIFI, 1, init_param_ptr);

    SCI_FREE(init_param_ptr);
    //SCI_TRACE_LOW:"StartPortalHttpRequest http_error=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_1717_112_2_18_3_9_47_148,(uint8*)"d", http_error);


    s_wlan_portal_info.login_type = MMIWIFI_LOGINTYPE_NONE;
    if (s_wlan_portal_info.cookie_ptr != PNULL)
    {
        SCI_FREE(s_wlan_portal_info.cookie_ptr);
        s_wlan_portal_info.cookie_ptr = PNULL;
    }

    if (HTTP_SUCCESS != http_error)
    {
        return FALSE;
    }

    return  TRUE;
}

/*****************************************************************************/
//  Description : Net Connect
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_WlanPortalNetConnect(void)
{
    MMIPDP_ACTIVE_INFO_T app_info = {0};

    //SCI_TRACE_LOW:"MMIWIFI_WlanPortalNetConnect status=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_1743_112_2_18_3_9_47_149,(uint8*)"d", s_wlan_portal_info.status);

    if (MMIWIFI_PDP_ACTIVE_OK == s_wlan_portal_info.status)
    {
        StartPortalHttpRequest(s_wlan_portal_info.netid);
    }
    else if (MMIWIFI_PDP_ACTIVING != s_wlan_portal_info.status)
    {
        app_info.app_handler = MMI_MODULE_WIFI;
        app_info.handle_msg_callback = HandleWlanPortalPsMsg;
        app_info.ps_interface = MMIPDP_INTERFACE_WIFI;

        s_wlan_portal_info.status = MMIWIFI_PDP_ACTIVING;

        if (!MMIAPIPDP_Active(&app_info))
        {
            //SCI_TRACE_LOW:"WlanPortalNetConnect:MMIAPIPDP_Active fail"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_1759_112_2_18_3_9_47_150,(uint8*)"");
            
            return FALSE;
        }
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : Net Disconnect
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWIFI_WlanPortalNetDisconnect(void)
{
    MMIAPIPDP_Deactive(MMI_MODULE_WIFI);
}

/*****************************************************************************/
//  Description : Net Disconnect
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWIFI_WlanPortalRequestCancel(void)
{
    if (0 != s_wlan_portal_info.context_id)
    {
        HTTP_CancelRequest(s_wlan_portal_info.context_id, s_wlan_portal_info.app_instance, s_wlan_portal_info.request_id);
    }
}

/*****************************************************************************/
//  Description : Net Disconnect
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWIFI_WlanPortalRequestClose(void)
{
    if (0 != s_wlan_portal_info.context_id)
    {
        HTTP_CloseRequest(s_wlan_portal_info.context_id);
    }
}

/*****************************************************************************/
//  Description : find action url
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL uint8* WlanPortalFindActionUrl(uint8 * data_ptr, uint32 data_len, uint16 *url_len_ptr, BOOLEAN *is_login_form)
{
    uint8 * dest_str1 = PNULL;
    uint8 * dest_str2 = PNULL;
    uint8 * dest_str3 = PNULL;
    uint8 * url_ptr = PNULL;
    char my_form[MMIWIFI_PORTAL_LOGIN_FORM_LEN_MAX + 1] = {0};
    char login_form[MMIWIFI_PORTAL_LOGIN_FORM_LEN_MAX + 1] = {0};

    if (PNULL == data_ptr || PNULL == url_len_ptr || PNULL == is_login_form)
    {
        //SCI_TRACE_LOW:"WlanPortalFindActionUrl Input Invalid"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_1816_112_2_18_3_9_47_151,(uint8*)"");

        return PNULL;
    }

    MMIWIFI_GetMyFormTag(my_form, MMIWIFI_PORTAL_LOGIN_FORM_LEN_MAX);
    MMIWIFI_GetLoginFormTag(login_form, MMIWIFI_PORTAL_LOGIN_FORM_LEN_MAX);

    if (s_wlan_portal_info.login_type == MMIWIFI_LOGINTYPE_GD)
    {
        dest_str1 = (uint8 *)MMIAPICOM_Stristr((char *)data_ptr, login_form);
    }
    else
    {
        dest_str1 = (uint8 *)MMIAPICOM_Stristr((char *)data_ptr, my_form);
    }
    
    if (PNULL != dest_str1)
    {
        //SCI_TRACE_LOW:"WlanPortalFindActionUrl Find Myform"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_1835_112_2_18_3_9_47_152,(uint8*)"");

        dest_str2 = (uint8 *)MMIAPICOM_Stristr((char *)dest_str1, (char *)MMIWLAN_HTTP_ACTION);

        if (PNULL != dest_str2)
        {
            dest_str2 += strlen(MMIWLAN_HTTP_ACTION) + strlen(MMIWLAN_HTTP_QUOTES_MARK);

            dest_str3 = (uint8 *)MMIAPICOM_Stristr((char *)dest_str2, (char *)MMIWLAN_HTTP_QUOTES_MARK);

            url_ptr = dest_str2;

            *url_len_ptr = dest_str3 - url_ptr;
        }

        if (s_wlan_portal_info.login_type == MMIWIFI_LOGINTYPE_GD)
        {
            *is_login_form = TRUE;
        }
        else
        {
             *is_login_form = FALSE;
        }
        
    }
    else
    {
        dest_str1 = (uint8 *)MMIAPICOM_Stristr((char *)data_ptr, login_form);

        if (PNULL != dest_str1)
        {
            //SCI_TRACE_LOW:"WlanPortalFindActionUrl Find LoginForm"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_1867_112_2_18_3_9_47_153,(uint8*)"");

                dest_str2 = (uint8 *)MMIAPICOM_Stristr((char *)dest_str1, (char *)MMIWLAN_HTTP_ACTION);

                if (PNULL != dest_str2)
                {
                    dest_str2 += strlen(MMIWLAN_HTTP_ACTION) + strlen(MMIWLAN_HTTP_QUOTES_MARK);

                    dest_str3 = (uint8 *)MMIAPICOM_Stristr((char *)dest_str2, (char *)MMIWLAN_HTTP_QUOTES_MARK);

                    url_ptr = dest_str2;

                    *url_len_ptr = dest_str3 - url_ptr;
                }

            *is_login_form = TRUE;
        }
    }

    //SCI_TRACE_LOW:"JXC WlanPortalFindActionUrl urlptr=%s,len=%d,isloginform=%d"
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_1887_112_2_18_3_9_47_154,(uint8*)"sdd",url_ptr,*url_len_ptr,*is_login_form);

    return url_ptr;
}

/*****************************************************************************/
//  Description : find Name Value
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL uint8* WlanPortalFindNameValue(uint8 * data_ptr, uint32 data_len, const char *name_ptr, uint16 *acname_len_ptr)
{
    uint8 * dest_str1 = PNULL;
    uint8 * dest_str2 = PNULL;
    uint8 * dest_str3 = PNULL;
    uint8 * acname_ptr = PNULL;

    if (PNULL == data_ptr || PNULL == acname_len_ptr || PNULL == name_ptr)
    {
        //SCI_TRACE_LOW:"WlanPortalFindNameValue Input Invalid"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_1907_112_2_18_3_9_47_155,(uint8*)"");

        return PNULL;
    }

    dest_str1 = (uint8 *)MMIAPICOM_Stristr((char *)data_ptr, (char *)name_ptr);

    if (PNULL != dest_str1)
    {
        //SCI_TRACE_LOW:"WlanPortalFindNameValue Find!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_1916_112_2_18_3_9_47_156,(uint8*)"");

        dest_str2 = (uint8 *)MMIAPICOM_Stristr((char *)dest_str1, (char *)MMIWLAN_HTTP_VALUE);

        if (PNULL != dest_str2)
        {
            dest_str2 += strlen(MMIWLAN_HTTP_VALUE) + strlen(MMIWLAN_HTTP_QUOTES_MARK);

            dest_str3 = (uint8 *)MMIAPICOM_Stristr((char *)dest_str2, (char *)MMIWLAN_HTTP_QUOTES_MARK);

            acname_ptr = dest_str2;

            *acname_len_ptr = dest_str3 - dest_str2;
        }
    }

    return acname_ptr;
}

/*****************************************************************************/
//  Description : find Method
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMIWIFI_HTTP_METHOD_E WlanPortalFindMethod(uint8 * data_ptr, uint32 data_len)
{
    uint8 * dest_str1 = PNULL;
    uint8 * dest_str2 = PNULL;
    uint16 len = 0;
    MMIWIFI_HTTP_METHOD_E http_method = MMIWIFI_HTTP_METHOD_NONE;

    if (PNULL == data_ptr)
    {
        //SCI_TRACE_LOW:"WlanPortalFindMethod Input Invalid"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_1948_112_2_18_3_9_48_157,(uint8*)"");

        return http_method;
    }

    dest_str1 = (uint8 *)MMIAPICOM_Stristr((char *)data_ptr, (char *)MMIWLAN_PORTAL_METHOD);

    if (PNULL != dest_str1)
    {
        dest_str2 = (uint8 *)MMIAPICOM_Stristr((char *)dest_str1, (char *)MMIWLAN_PORTAL_GET);

        if (PNULL != dest_str2)
        {
            len = dest_str2 - dest_str1;

            if (len < MMIWLAN_JUDGE_METHOD_LENGTH_MAX)
            {
                http_method = MMIWIFI_HTTP_METHOD_GET;
            }
        }

        if (MMIWIFI_HTTP_METHOD_NONE == http_method)
        {
            dest_str2 = (uint8 *)MMIAPICOM_Stristr((char *)dest_str1, (char *)MMIWLAN_PORTAL_POST);

            if (PNULL != dest_str2)
            {
                len = dest_str2 - dest_str1;

                if (len < MMIWLAN_JUDGE_METHOD_LENGTH_MAX)
                {
                    http_method = MMIWIFI_HTTP_METHOD_POST;
                }
            }
        }
    }

    return http_method;
}

/*****************************************************************************/
//  Description : find Login_res
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMIWIFI_PORTAL_LOGIN_RES_E WlanPortalLoginRes(uint8 * data_ptr, uint32 data_len)
{
    uint8 * dest_str1 = PNULL;
    uint8 * dest_str2 = PNULL;
    uint16 len = 0;
    uint32 res_num = 0;
    char *stop_str_ptr = PNULL;
    MMIWIFI_PORTAL_LOGIN_RES_E login_res = MMIWIFI_PORTAL_LOGIN_MAX;

    if (PNULL == data_ptr)
    {
        //SCI_TRACE_LOW:"WlanPortalLoginRes Input Invalid"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_2003_112_2_18_3_9_48_158,(uint8*)"");

        return login_res;
    }

    dest_str1 = (uint8 *)MMIAPICOM_Stristr((char *)data_ptr, (char *)MMIWLAN_PORTAL_LOGIN_RES);

    if (PNULL != dest_str1)
    {
        len = strlen( (char *)MMIWLAN_PORTAL_LOGIN_RES);
        dest_str2 = (uint8 *)(dest_str1 + len + 1);//语法login_res|0|

        res_num = (uint32)strtol((char *)dest_str2, &stop_str_ptr, 10);

        //SCI_TRACE_LOW:"WlanPortalLoginRes res_num=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_2017_112_2_18_3_9_48_159,(uint8*)"d", res_num);

        switch (res_num)
        {
        case 0:
            login_res= MMIWIFI_PORTAL_LOGIN_SUCCESS;
            break;

        case 1:
            login_res= MMIWIFI_PORTAL_LOGIN_USER_ERROR;
            break;

        case 2:
            login_res= MMIWIFI_PORTAL_LOGIN_USER_ABNORMAL_STATE;
            break;

        case 3:
            login_res= MMIWIFI_PORTAL_LOGIN_PWD_ERROR;
            break;

        case 7:
            login_res= MMIWIFI_PORTAL_LOGIN_IP_NOT_MATCH;
            break;

        case 8:
            login_res= MMIWIFI_PORTAL_LOGIN_AC_NOT_MATCH;
            break;

        case 15:
            login_res= MMIWIFI_PORTAL_LOGIN_PORTAL_REJECT;
            break;

        case 17:
            login_res= MMIWIFI_PORTAL_LOGIN_PORTALING;
            break;

        case 26:
            login_res= MMIWIFI_PORTAL_LOGIN_USER_CONFLICT;
            break;

        case 40:
            login_res= MMIWIFI_PORTAL_LOGIN_UNPROTAL_EXPIRED;
            break;

        default:
            login_res= MMIWIFI_PORTAL_LOGIN_FAILED;
            break;
        }
    }

    return login_res;
}

/*****************************************************************************/
//  Description : find Offline_res
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMIWIFI_PORTAL_OFFLINE_RES_E WlanPortalOfflineRes(uint8 * data_ptr, uint32 data_len)
{
    uint8 * dest_str1 = PNULL;
    uint8 * dest_str2 = PNULL;
    uint16 len = 0;
    uint32 res_num = 0;
    char *stop_str_ptr = PNULL;
    MMIWIFI_PORTAL_OFFLINE_RES_E offline_res = MMIWIFI_PORTAL_OFFLINE_MAX;

    if (PNULL == data_ptr)
    {
        //SCI_TRACE_LOW:"WlanPortalOfflineRes Input Invalid"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_2085_112_2_18_3_9_48_160,(uint8*)"");

        return offline_res;
    }

    dest_str1 = (uint8 *)MMIAPICOM_Stristr((char *)data_ptr, (char *)MMIWLAN_PORTAL_OFFLINE_RES);

    if (PNULL != dest_str1)
    {
        len = strlen( (char *)MMIWLAN_PORTAL_OFFLINE_RES);
        dest_str2 = (uint8 *)(dest_str1 + len + 1);//语法offline_res|0|

        res_num = (uint32)strtol((char *)dest_str2, &stop_str_ptr, 10);

        //SCI_TRACE_LOW:"WlanPortalOfflineRes res_num=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_2099_112_2_18_3_9_48_161,(uint8*)"d", res_num);

        switch (res_num)
        {
        case 0:
            offline_res= MMIWIFI_PORTAL_OFFLINE_SUCCESS;
            break;

        case 7:
            offline_res= MMIWIFI_PORTAL_OFFLINE_IP_NOT_MATCH;
            break;

        case 9:
            offline_res= MMIWIFI_PORTAL_OFFLINE_AC_NOT_MATCH;
            break;

        case 11:
            offline_res= MMIWIFI_PORTAL_OFFLINE_AUTO_OFFLINE;
            break;

        case 18:
            offline_res= MMIWIFI_PORTAL_OFFLINE_REJECT;
            break;

        default:
            offline_res= MMIWIFI_PORTAL_OFFLINE_FAILED;
            break;
        }
    }

    return offline_res;
}

/*****************************************************************************/
//  Description : Cat Http data
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL void CatHttpData(uint8 ** dst_data_ptr, uint16 * dst_pos, uint8 *data, uint16 data_len, const char *data_mark, uint16 data_mark_len)
{
    uint16 equal_mark_len = 0;
    uint16 and_mark_len = 0;

    if (PNULL == dst_data_ptr || PNULL == dst_pos || PNULL == data || PNULL == data_mark)
    {
        return;
    }

    equal_mark_len = strlen((char *)MMIWLAN_HTTP_EQUAL_MARK);
    and_mark_len = strlen((char *)MMIWLAN_HTTP_AND_MARK);

    if (* dst_pos != 0)
    {
        SCI_MEMCPY(*dst_data_ptr + *dst_pos, (uint8 *)MMIWLAN_HTTP_AND_MARK, and_mark_len);
        *dst_pos += and_mark_len;
     }
    SCI_MEMCPY(*dst_data_ptr + *dst_pos, (uint8 *)data_mark, data_mark_len);
    *dst_pos += data_mark_len;
    SCI_MEMCPY(*dst_data_ptr + *dst_pos, (uint8 *)MMIWLAN_HTTP_EQUAL_MARK, equal_mark_len);
    *dst_pos += equal_mark_len;
    SCI_MEMCPY(*dst_data_ptr + *dst_pos, data, data_len);
    *dst_pos += data_len;
}

/*****************************************************************************/
//  Description : parse data
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMIWIFI_HTTP_METHOD_E WlanPortalParseXhtmlData(uint8 * data_ptr, uint32 data_len,
                                                                                                        uint8 ** url_ptr, uint16 *url_len_ptr,
                                                                                                        uint8 ** body_ptr, uint16 *body_len_ptr)
{
    uint8 * parse_url_ptr = PNULL;
    uint16 parse_url_len = 0;
    uint8 * acname_ptr = PNULL;
    uint16 acname_len = 0;
    uint8 * userip_ptr = PNULL;
    uint16 userip_len = 0;
    uint8 * acip_ptr = PNULL;
    uint16 acip_len = 0;
    uint8 * actiontype_ptr = PNULL;
    uint16 actiontype_len = 0;
    uint8 * acssid_ptr = PNULL;
    uint16 acssid_len = 0;
    uint8 * logonsessid_ptr = PNULL;
    uint16 logonsessid_len = 0;
    uint8 * languagetype_ptr = PNULL;
    uint16 languagetype_len = 0;
    uint16 temp_body_len = 0;
    uint16 equal_mark_len = 0;
    uint16 and_mark_len = 0;
    uint16 acname_mark_len = 0;
    uint16 actiontype_mark_len = 0;
    uint16 acssid_mark_len = 0;
    uint16 logonsessid_mark_len = 0;
    uint16 languagetype_mark_len = 0;
    uint16 user_mark_len = 0;
    uint16 pwd_mark_len = 0;
    uint16 userip_mark_len = 0;
    uint16 acip_mark_len = 0;
    BOOLEAN is_login_form = FALSE;
    MMIWIFI_HTTP_METHOD_E http_method = MMIWIFI_HTTP_METHOD_NONE;
    MMIWIFI_PORTAL_CONFIG_T portal_cfg = {0};

    uint16 logintype_len = 2;
    uint16 logintype_mark_len = 0;
    uint16 smsrnd_mark_len = 0;
    


    if (PNULL == data_ptr || 0 == data_len
        ||PNULL == url_ptr || PNULL == url_len_ptr
        || PNULL == body_ptr || PNULL == body_len_ptr)
    {
        //SCI_TRACE_LOW:"WlanPortalParseXhtmlData Input Invalid"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_2212_112_2_18_3_9_48_162,(uint8*)"");

        return PNULL;
    }

    parse_url_ptr = WlanPortalFindActionUrl(data_ptr, data_len, &parse_url_len, &is_login_form);
    acname_ptr = WlanPortalFindNameValue(data_ptr, data_len, (const char *)MMIWLAN_PORTAL_ACNAME_MARK, &acname_len);
    userip_ptr = WlanPortalFindNameValue(data_ptr, data_len, (const char *)MMIWLAN_PORTAL_USERIP_MARK, &userip_len);
    acip_ptr = WlanPortalFindNameValue(data_ptr, data_len, (const char *)MMIWLAN_PORTAL_ACIP_MARK, &acip_len);
    actiontype_ptr = WlanPortalFindNameValue(data_ptr, data_len, (const char *)MMIWLAN_PORTAL_ACTIONTYPE_MARK, &actiontype_len);
    acssid_ptr = WlanPortalFindNameValue(data_ptr, data_len, (const char *)MMIWLAN_PORTAL_ACSSID_MARK, &acssid_len);
    logonsessid_ptr = WlanPortalFindNameValue(data_ptr, data_len, (const char *)MMIWLAN_PORTAL_LOGONSESSID_MARK, &logonsessid_len);
    languagetype_ptr = WlanPortalFindNameValue(data_ptr, data_len, (const char *)MMIWLAN_PORTAL_LANGUAGETYPE_MARK, &languagetype_len);
    http_method = WlanPortalFindMethod(data_ptr, data_len);
    //submit_ptr  = WlanPortalFindNameValue(data_ptr,data_len,(const char *)MMIWLAN_PORTAL_SUBMIT_MARK, &submit_len);
    
    if (PNULL != parse_url_ptr && PNULL != acname_ptr && PNULL != userip_ptr)//必选
    {
        uint8 user_name[MMIWLAN_PORTAL_USER_NAME_MAX_LEN + 1] = {0};
        uint8 password[MMIWLAN_PORTAL_PASSWORD_MAX_LEN + 1] = {0};
        uint8 escape_user_name[3 * MMIWLAN_PORTAL_USER_NAME_MAX_LEN + 1] = {0};
        uint16 escape_user_name_len = 0;
        uint8 escape_password[3 * MMIWLAN_PORTAL_PASSWORD_MAX_LEN + 1] = {0};
        uint16 escape_password_len = 0;

        equal_mark_len = strlen((char *)MMIWLAN_HTTP_EQUAL_MARK);
        and_mark_len = strlen((char *)MMIWLAN_HTTP_AND_MARK);
        acname_mark_len = strlen((char *)MMIWLAN_PORTAL_ACNAME);
        userip_mark_len = strlen((char *)MMIWLAN_PORTAL_USERIP);
        acip_mark_len = strlen((char *)MMIWLAN_PORTAL_ACIP);
        actiontype_mark_len = strlen((char *)MMIWLAN_PORTAL_ACTIONTYPE);
        acssid_mark_len = strlen((char *)MMIWLAN_PORTAL_ACSSID);
        logonsessid_mark_len = strlen((char *)MMIWLAN_PORTAL_LOGONSESSID);
        languagetype_mark_len = strlen((char *)MMIWLAN_PORTAL_LANGUAGETYPE);
        user_mark_len = strlen((char *)MMIWLAN_PORTAL_USER);
        pwd_mark_len = strlen((char *)MMIWLAN_PORTAL_PWD);
        //submit_mark_len = strlen((char *)MMIWLAN_PORTAL_SUBMIT);
        logintype_mark_len = strlen((char *)MMIWLAN_PORTAL_LOGINTYPE);
        smsrnd_mark_len = strlen((char *)MMIWLAN_PORTAL_SMSRND);

        MMIWIFI_GetPortalConfig(&portal_cfg);

        //url
        *url_len_ptr = parse_url_len;

        *url_ptr = SCI_ALLOCA(*url_len_ptr + 1);

        if (PNULL != *url_ptr)
        {
            SCI_MEMSET(*url_ptr, 0x00, *url_len_ptr + 1);

            SCI_MEMCPY(*url_ptr, (uint8 *)parse_url_ptr, *url_len_ptr);
        }
        else
        {
            //SCI_TRACE_LOW:"WlanPortalParseXhtmlData No Memory"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_2267_112_2_18_3_9_48_163,(uint8*)"");

            return MMIWIFI_HTTP_METHOD_NONE;
        }

        //body
        *body_len_ptr = 2 * equal_mark_len + and_mark_len + acname_len + acname_mark_len + userip_len + userip_mark_len;

        if (PNULL != acip_ptr)
        {
            *body_len_ptr += equal_mark_len + and_mark_len + acip_len + acip_mark_len;
        }
        
        if (PNULL != actiontype_ptr && is_login_form)//actiontype
        {
            if (0 != portal_cfg.user_name_len)
            {
                MMI_WSTRNTOSTR(user_name, MMIWLAN_PORTAL_USER_NAME_MAX_LEN,
                                            portal_cfg.user_name, MMIWLAN_PORTAL_USER_NAME_MAX_LEN,
                                            portal_cfg.user_name_len);

                escape_user_name_len = URL_EscapeUrl(escape_user_name, 3 * MMIWLAN_PORTAL_USER_NAME_MAX_LEN, user_name, URL_ESCAPE);
            }

            if (0 != portal_cfg.password_len)
            {
                MMI_WSTRNTOSTR(password, MMIWLAN_PORTAL_PASSWORD_MAX_LEN,
                                            portal_cfg.password, MMIWLAN_PORTAL_PASSWORD_MAX_LEN,
                                            portal_cfg.password_len);

                escape_password_len = URL_EscapeUrl(escape_password, 3 * MMIWLAN_PORTAL_PASSWORD_MAX_LEN, password, URL_ESCAPE);
            }

            

            if (s_wlan_portal_info.login_type == MMIWIFI_LOGINTYPE_GD)
            {

                //*body_len_ptr += equal_mark_len + and_mark_len + logintype_len + logintype_mark_len;
                //*body_len_ptr += equal_mark_len + and_mark_len + submit_len*4 + submit_mark_len;

                 *body_len_ptr += equal_mark_len + and_mark_len + escape_password_len + smsrnd_mark_len;
                 *body_len_ptr += equal_mark_len + and_mark_len + logintype_len + logintype_mark_len;

                  user_mark_len = strlen((char *)MMIWLAN_PORTAL_SZ_USER);
                  pwd_mark_len = strlen((char *)MMIWLAN_PORTAL_SZ_PWD);
            }
            else
            {
                *body_len_ptr += equal_mark_len + and_mark_len +actiontype_len + actiontype_mark_len;
                
                if (PNULL != acssid_ptr)
                {
                    *body_len_ptr += equal_mark_len + and_mark_len +acssid_len + acssid_mark_len;
                }

                if (PNULL != logonsessid_ptr)
                {
                    *body_len_ptr += equal_mark_len + and_mark_len +logonsessid_len + logonsessid_mark_len;
                }

                if (PNULL != languagetype_ptr)
                {
                    *body_len_ptr += equal_mark_len + and_mark_len +languagetype_len + languagetype_mark_len;
                }            
            }

            *body_len_ptr += equal_mark_len + and_mark_len + escape_user_name_len + user_mark_len;//user

            *body_len_ptr += equal_mark_len + and_mark_len + escape_password_len + pwd_mark_len;//pwd
        }

        *body_ptr = SCI_ALLOCA(*body_len_ptr + 1);

        if (PNULL != *body_ptr)
        {
            SCI_MEMSET(*body_ptr, 0x00, *body_len_ptr + 1);

            if (s_wlan_portal_info.login_type != MMIWIFI_LOGINTYPE_GD)
            {
                //ac name
                SCI_MEMCPY(*body_ptr, (uint8 *)MMIWLAN_PORTAL_ACNAME, acname_mark_len);
                temp_body_len = acname_mark_len;
                SCI_MEMCPY(*body_ptr + temp_body_len, (uint8 *)MMIWLAN_HTTP_EQUAL_MARK, equal_mark_len);
                temp_body_len += equal_mark_len;
                SCI_MEMCPY(*body_ptr + temp_body_len, acname_ptr, acname_len);
                temp_body_len += acname_len;

                //user ip
                CatHttpData(body_ptr, &temp_body_len, userip_ptr, userip_len, MMIWLAN_PORTAL_USERIP, userip_mark_len);

                //ac ip
                if (PNULL != acip_ptr)
                {
                    CatHttpData(body_ptr, &temp_body_len, acip_ptr, acip_len, MMIWLAN_PORTAL_ACIP, acip_mark_len);
                }                
            }

            if (PNULL != actiontype_ptr && is_login_form)
            {
                uint8 *temp_ptr = PNULL;

                if (s_wlan_portal_info.login_type == MMIWIFI_LOGINTYPE_GD)
                {
                    CatHttpData(body_ptr, &temp_body_len, escape_user_name, escape_user_name_len, MMIWLAN_PORTAL_SZ_USER, user_mark_len);

                    //pwd
                    CatHttpData(body_ptr, &temp_body_len, escape_password, escape_password_len, MMIWLAN_PORTAL_SZ_PWD, pwd_mark_len);
                
                    CatHttpData(body_ptr, &temp_body_len, escape_password,escape_password_len, MMIWLAN_PORTAL_SMSRND, smsrnd_mark_len);         
                        //logintype
                    CatHttpData(body_ptr, &temp_body_len, (uint8 *)"", 0, MMIWLAN_PORTAL_LOGINTYPE, logintype_mark_len);         
                        
                     //ac name
                    CatHttpData(body_ptr, &temp_body_len, acname_ptr, acname_len, MMIWLAN_PORTAL_ACNAME, acname_mark_len);

                    //user ip
                    CatHttpData(body_ptr, &temp_body_len, userip_ptr, userip_len, MMIWLAN_PORTAL_USERIP, userip_mark_len);
                    
                    *body_len_ptr = temp_body_len;                    
                }
                else
                {
                    //ac ssid
                    if (PNULL != acssid_ptr)
                    {
                        CatHttpData(body_ptr, &temp_body_len, acssid_ptr, acssid_len, MMIWLAN_PORTAL_ACSSID, acssid_mark_len);
                    }

                    //logon sessid
                    if (PNULL != logonsessid_ptr)
                    {
                        CatHttpData(body_ptr, &temp_body_len, logonsessid_ptr, logonsessid_len, MMIWLAN_PORTAL_LOGONSESSID, logonsessid_mark_len);
                    }

                    //langguage type
                    if (PNULL != languagetype_ptr)
                    {
                        CatHttpData(body_ptr, &temp_body_len, languagetype_ptr, languagetype_len, MMIWLAN_PORTAL_LANGUAGETYPE, languagetype_mark_len);
                    }

                    //action type
                    CatHttpData(body_ptr, &temp_body_len, actiontype_ptr, actiontype_len, MMIWLAN_PORTAL_ACTIONTYPE, actiontype_mark_len);

                    temp_ptr = (uint8 *)MMIAPICOM_Stristr((char *)actiontype_ptr, (char *)MMIWLAN_PORTAL_ACTIONTYPE_LOGIN);

                    if (PNULL != temp_ptr)//login
                    {
                        //user
                        CatHttpData(body_ptr, &temp_body_len, escape_user_name, escape_user_name_len, MMIWLAN_PORTAL_USER, user_mark_len);

                        //pwd
                        CatHttpData(body_ptr, &temp_body_len, escape_password, escape_password_len, MMIWLAN_PORTAL_PWD, pwd_mark_len);
                    }
                }
                
            }


            //SCI_TRACE_LOW:"WlanPortalParseXhtmlData *body_len_ptr=%d, temp_body_len=%d, url=%s, body=%s"
            //SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_2426_112_2_18_3_9_49_164,(uint8*)"ddss", *body_len_ptr, temp_body_len, * url_ptr, *body_ptr);
        }
        else
        {
            //SCI_TRACE_LOW:"WlanPortalParseXhtmlData No Memory"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_2430_112_2_18_3_9_49_165,(uint8*)"");

            SCI_FREE(*url_ptr);

            *url_ptr = PNULL;

            return MMIWIFI_HTTP_METHOD_NONE;
        }
    }

    return http_method;
}

/*only for guangdong wlan*/
/*****************************************************************************/
//  Description : parse data
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMIWIFI_HTTP_METHOD_E WlanPortalParseLoginSuccessData(uint8 * data_ptr, uint32 data_len,
                                                                                                        uint8 ** url_ptr, uint16 *url_len_ptr,
                                                                                                        uint8 ** body_ptr, uint16 *body_len_ptr)
{
    uint8 * parse_url_ptr = PNULL;
    uint16 parse_url_len = 0;
    uint16 temp_body_len = 0;
    uint16 equal_mark_len = 0;
    uint16 and_mark_len = 0;
    uint8* loginip_ptr =PNULL;
    uint16 loginip_len = 0;
    uint16 loginip_mark_len=0;
    uint8* loginname_ptr =PNULL;
    uint16 loginname_len = 0;
    uint16 loginname_mark_len=0;
    uint8* deviceid_ptr =PNULL;
    uint16 deviceid_len = 0;
    uint16 deviceid_mark_len=0;
    uint8* wlandig_ptr =PNULL;
    uint16 wlandig_len = 0;
    uint16 wlandig_mark_len=0;
    uint8* wlansid_ptr =PNULL;
    uint16 wlansid_len = 0;
    uint16 wlansid_mark_len=0;
    uint16 logouttype_len = 0;
    uint16 logouttype_mark_len = 0;
    char* logouttype_value = "TYPESUBMIT";

    BOOLEAN is_login_form = FALSE;
    MMIWIFI_HTTP_METHOD_E http_method = MMIWIFI_HTTP_METHOD_NONE;
    //MMIWIFI_PORTAL_CONFIG_T portal_cfg = {0};

    if (PNULL == data_ptr || 0 == data_len
        ||PNULL == url_ptr || PNULL == url_len_ptr
        || PNULL == body_ptr || PNULL == body_len_ptr)
    {
        //SCI_TRACE_LOW:"WlanPortalParseLoginSuccessData Input Invalid"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_2486_112_2_18_3_9_49_166,(uint8*)"");

        return PNULL;
    }

    parse_url_ptr = WlanPortalFindActionUrl(data_ptr, data_len, &parse_url_len, &is_login_form);
    loginip_ptr = WlanPortalFindNameValue(data_ptr, data_len, (const char *)MMIWLAN_PORTAL_LOGINIP_MARK, &loginip_len);
    loginname_ptr = WlanPortalFindNameValue(data_ptr, data_len, (const char *)MMIWLAN_PORTAL_LOGINNAME_MARK, &loginname_len);
    deviceid_ptr = WlanPortalFindNameValue(data_ptr, data_len, (const char *)MMIWLAN_PORTAL_DEVICEID_MARK, &deviceid_len);
    wlandig_ptr = WlanPortalFindNameValue(data_ptr, data_len, (const char *)MMIWLAN_PORTAL_WLANDIG_MARK, &wlandig_len);
    wlansid_ptr = WlanPortalFindNameValue(data_ptr, data_len, (const char *)MMIWLAN_PORTAL_WLANSID_MARK, &wlansid_len);
    
    http_method = WlanPortalFindMethod(data_ptr, data_len);

    if (PNULL != parse_url_ptr)//必选
    {
        equal_mark_len = strlen((char *)MMIWLAN_HTTP_EQUAL_MARK);
        and_mark_len = strlen((char *)MMIWLAN_HTTP_AND_MARK);

        loginip_mark_len = strlen((char *)MMIWLAN_PORTAL_LOGINIP);
        loginname_mark_len = strlen((char *)MMIWLAN_PORTAL_LOGINNAME);
        deviceid_mark_len = strlen((char *)MMIWLAN_PORTAL_DEVICEID);
        wlandig_mark_len = strlen((char *)MMIWLAN_PORTAL_WLANDIG);
        wlansid_mark_len = strlen((char *)MMIWLAN_PORTAL_WLANSID);
        logouttype_mark_len = strlen((char *)MMIWLAN_PORTAL_LOGOUTTYPE);
        logouttype_len = strlen(logouttype_value);
        
        //url
        *url_len_ptr = parse_url_len;

        *url_ptr = SCI_ALLOCA(*url_len_ptr + 1);

        if (PNULL != *url_ptr)
        {
            SCI_MEMSET(*url_ptr, 0x00, *url_len_ptr + 1);

            SCI_MEMCPY(*url_ptr, (uint8 *)parse_url_ptr, *url_len_ptr);
        }
        else
        {
            //SCI_TRACE_LOW:"WlanPortalParseXhtmlData No Memory"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_2530_112_2_18_3_9_49_167,(uint8*)"");

            return MMIWIFI_HTTP_METHOD_NONE;
        }

        //body
        //*body_len_ptr = 2 * equal_mark_len + and_mark_len + acname_len + acname_mark_len + userip_len + userip_mark_len;
        *body_len_ptr =  logouttype_len+equal_mark_len +  strlen(logouttype_value);
        *body_len_ptr +=  and_mark_len+loginip_len+equal_mark_len +  loginip_len;
        *body_len_ptr +=  and_mark_len+loginname_len+equal_mark_len +  loginname_len;
        *body_len_ptr +=  and_mark_len+deviceid_len+equal_mark_len +  deviceid_len;
        *body_len_ptr +=  and_mark_len+3*wlandig_len+equal_mark_len +  wlandig_len;
        *body_len_ptr +=  and_mark_len+wlansid_len+equal_mark_len +  wlansid_len;
        
        *body_ptr = SCI_ALLOCA(*body_len_ptr + 1);

        if (PNULL != *body_ptr)
        {
            SCI_MEMSET(*body_ptr, 0x00, *body_len_ptr + 1);

            CatHttpData(body_ptr, &temp_body_len, (uint8 *)logouttype_value, logouttype_len, MMIWLAN_PORTAL_LOGOUTTYPE, logouttype_mark_len);
            CatHttpData(body_ptr, &temp_body_len, loginip_ptr, loginip_len, MMIWLAN_PORTAL_LOGINIP, loginip_mark_len);
            CatHttpData(body_ptr, &temp_body_len, loginname_ptr, loginname_len, MMIWLAN_PORTAL_LOGINNAME, loginname_mark_len);
            CatHttpData(body_ptr, &temp_body_len, deviceid_ptr, deviceid_len, MMIWLAN_PORTAL_DEVICEID, deviceid_mark_len);
            {
                uint16 escape_wlandig_len = 0;
                uint8* escape_wlandig_ptr = PNULL;
                uint8* wlandig_ptr_tmp = PNULL;
                wlandig_ptr_tmp = SCI_ALLOC_APPZ(wlandig_len+1);
                SCI_MEMCPY(wlandig_ptr_tmp,wlandig_ptr,wlandig_len);
                escape_wlandig_ptr = SCI_ALLOC_APPZ(3*wlandig_len+1);                
                escape_wlandig_len = URL_EscapeUrl(escape_wlandig_ptr, 3 * wlandig_len, wlandig_ptr_tmp, URL_ESCAPE);
                CatHttpData(body_ptr, &temp_body_len, escape_wlandig_ptr, escape_wlandig_len, MMIWLAN_PORTAL_WLANDIG, wlandig_mark_len);
                SCI_FREE(escape_wlandig_ptr);
                SCI_FREE(wlandig_ptr_tmp);
            }           
            CatHttpData(body_ptr, &temp_body_len, wlansid_ptr, wlansid_len, MMIWLAN_PORTAL_WLANSID, wlansid_mark_len);

                    
             *body_len_ptr = temp_body_len;                    
                             
            //SCI_TRACE_LOW:"WlanPortalParseLoginSuccessData *body_len_ptr=%d, temp_body_len=%d, url=%s, body=%s"
            //SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_2574_112_2_18_3_9_49_168,(uint8*)"ddss", *body_len_ptr, temp_body_len, * url_ptr, *body_ptr);
        }
        else
        {
            //SCI_TRACE_LOW:"WlanPortalParseLoginSuccessData No Memory"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PORTAL_NET_2578_112_2_18_3_9_49_169,(uint8*)"");

            SCI_FREE(*url_ptr);

            *url_ptr = PNULL;

            return MMIWIFI_HTTP_METHOD_NONE;
        }
    }

    return http_method;
}

/*****************************************************************************/
//  Description : Is Url Absolute
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN URL_IsAbsolute(const uint8 * url_ptr)
{
    if (PNULL != url_ptr)
    {
        const uint8 * ptr = url_ptr;

        while (*ptr)
        {
            if (':' == *ptr)
            {
                return TRUE;
            }

            if ('/' == *ptr || '?' == *ptr || '#' == *ptr)
            {
                break;
            }

            ptr ++;
        }
    }

    return FALSE;
}

