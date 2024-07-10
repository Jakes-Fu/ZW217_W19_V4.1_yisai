
#ifndef _MMI_AI_HTTP_REG_C  
#define _MMI_AI_HTTP_REG_C  

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "ai_app.h"
#include "ai_http.h"
#ifdef  TULING_AI_SUPPORT
#include "std_header.h"
#include "os_api.h"
#include "sci_types.h"
#include "lcd_cfg.h"
#include "mmk_app.h"
#include "mmiphone_export.h"
#include "mmi_filemgr.h"
#include "http_api.h"
#include "version.h"
#include "socket_api.h"
#include "mmipdp_export.h"
#ifdef WIFI_SUPPORT
//#include "mmiwifi_export.h"
#endif
#include "in_message.h"
#include "mmiphone_export.h"
#include "mmi_default.h"
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
#include "cjson.h"
#include "ai_ase.h"

#ifdef WRE_SUPPORT
#include "mmiwre_export.h"
#endif

#include "gui_ucs2b_converter.h"
#include "mmisms_export.h"
#include "mmisms_export.h"
#include "os_param.h"

#define AI_HTTP_LOG            SCI_TRACE_LOW






#define BASE64LEN(x) (((((x) + 2) / 3) * 4) + 1)

#define AI_HTTP_URL_MAX_LEN   256


typedef enum
{
     AIREGHTTP_NONE,
     AIREGHTTP_ALIVE_MSG,
     AIREGHTTP_RESET_MSG,
     AIREGHTTP_12H_ALIVE_MSG,
     AIREGHTTP_TYPE_MAX
}AIREGHTTP_RESEND_TYPE_E;

typedef enum
{
    AIREGHTTP_RESET_NONE,
    AIREGHTTP_RESET_SEND,
    AIREGHTTP_RESET_RESEND,
    AIREGHTTP_RESET_SEND_END,
    AIREGHTTP_RESET_MAX
}AIREGHTTP_SEND_RESET_E;

typedef struct
{
    uint8 timer_id;
    uint8 *data_ptr;
    uint8 resend_count;
    uint8 timer_12h_id;
    uint8 retry_timer_id;
    uint8 sec_timer_id;
    AIREGHTTP_RESEND_TYPE_E resend_type;
    AIREGHTTP_SEND_RESET_E  reset_sendstatus;
    BOOLEAN assert_flag;
    uint8 send_count;
}AIREGHTTP_GLOBAL_T;

typedef struct _AI_HTTP_IND
{
    char url_arr[AI_HTTP_URL_MAX_LEN];
    uint16 refresh_time;
}AI_HTTP_REFRESH_IND_T;







PUBLIC void HandleAiHttpPdpEvent(void);
LOCAL void AiHttp_GetSignalStruct(DPARAM param, void *signal_struct_ptr, uint16 struct_size);
LOCAL int AiHttp_WritePostBody(uint8 ** ptr);
LOCAL int AiHttp_WritePostBodyBase64(uint8 ** ptr);
LOCAL void HandlePdpAiHttpTimer(uint8 timer_id,uint32 param);
LOCAL void MmiAiHttp_HandleRetryTimerMsg(uint8 timer_id, uint32 param);
PUBLIC BOOLEAN AI_HTTP_Refresh_Send(AI_HTTP_REFRESH_IND_T * pdata);
PUBLIC BOOLEAN AiHttpPDP();
//PUBLIC uint8 MMIAI_HTTP_Check_NeedRcv(MN_DUAL_SYS_E sim);
//PUBLIC BOOLEAN MMIAI_HTTP_RcvSet(uint32 count);





LOCAL int CTAIHttp_WritePostBody(uint8 * ptr) ;

#define AIREGHTTP_ACCEPT_STRING "*/*" 
#define AIREGHTTP_ACCEPT_LANGUAGE_STRING  "zh-cn, en" //表示接受中文英文
#define AIREGHTTP_USER_AGENT_STRING       "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.0)"
#define AIREGHTTP_ACCEPT_CHARSET_STRING   "utf-8"
#define AIREGHTTP_HTTP_URI_STRING_TEST "http://turing-iot.oss-cn-beijing.aliyuncs.com/tts/tts-86804674ce864045b696d5a58bb32988-ba941888ea714698930a7a3064b2ae1f.mp3"///  "http://3g.cn"
//#define AIREGHTTP_HTTP_URI_STRING_TEST_1   "http://wap.baidu.com/"
//#define AIREGHTTP_HTTP_URI_STRING_TEST_2   "http://wap.7999.com/"
//#define AIREGHTTP_HTTP_URI_STRING_TEST_3   "http://www.57616.com/"

#define HTTP_AIREGHTTP_CONTENT_TYPE  "multipart/form-data"//"application/x-www-form-urlencoded"
#define ACCEPT_STRING "*utf-8*"

#define DL_RANGE_HEADER_LEN  40
#define DL_RANGE_HEADER_PREFIX  "bytes="


#define AIREGHTTP_TIMEOUT_20S 20000

#define AIREGHTTP_TIMEOUT_60S 60000

#define AIREGHTTP_TIMEOUT_120S 120000

#define AIREGHTTP_TIMEOUT_180S 180000

#define AIREGHTTP_TIMEOUT_12HOUR 12*3600*1000

#define AIREGHTTP_SEND_COUNT 20

#define AIREGHTTP_RCV_COUNT (4*1024*1024)

#define AIREGHTTP_ALL_COUNT (150*1024*1024)

#define AI_MAX_RAM_BUF_SIZE  (4*128*1024)//(1024*1024 + 1*128*1024)




#define AIREGHTTP_HTTP_SIGNAL_ANALYTIC_EX(param,signal,app_param,http_context_ptr,app_module_id,sig_type) do{\
	    AiHttp_GetSignalStruct(param, &signal, sizeof(sig_type));\
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

#define AIREGHTTP_HTTP_SIGNAL_ANALYTIC(param,signal,http_context_ptr,sig_type) do{\
	AiHttp_GetSignalStruct(param, &signal, sizeof(sig_type));\
	if(signal.context_id != http_context_ptr->context_id)\
	{\
	    	return MMI_RESULT_FALSE;\
	}\
}while(0)\


typedef struct __AiHttp
{
    BOOLEAN flags;
    MN_DUAL_SYS_E sim_sys;
    uint32 net_id;
    BOOLEAN need_refresh;
}AiHttp;
typedef struct __AIREGHTTP_HTTP_CONTEXT
{
    HTTP_APP_MODULE_ID_T module_id;
    HTTP_CONTEXT_ID_T    context_id;
    HTTP_REQUEST_ID_T    request_id;
    HTTP_APP_INSTANCE_T  app_instance; //用于区分不同的HTTP请求。同时只有一个HTTP请求时可以为任意值。
}AIREGHTTP_HTTP_CONTEXT;

typedef struct _BREAK_CONTINUE
{
    uint32 already_rcv_len;
    uint16 break_count;
    BOOLEAN need_next_continue;
    BOOLEAN just_stop_by_break;
   
}BREAK_CONTINUE_T;



LOCAL MMI_RESULT_E HandleAiHttpDataInd(AIREGHTTP_HTTP_CONTEXT * http_context_ptr, DPARAM param);

extern PHONE_SERVICE_STATUS_T  g_service_status[MMI_DUAL_SYS_MAX];
BOOLEAN     m_ai_http_reg_pdpstate = FALSE;

AIREGHTTP_GLOBAL_T g_aireghttp_global = {0};

MMI_APPLICATION_T           g_aireghttphttp_app       = {0};
LOCAL AIREGHTTP_HTTP_CONTEXT   s_aireghttp_http_context      = {0};
LOCAL AiHttp s_aireghttphttp={0};
LOCAL AI_HTTP_REFRESH_IND_T s_refresh_ind ={0};
LOCAL AI_HTTP_REFRESH_IND_T ai_cur_url ={0};
LOCAL uint32 ai_cur_rcv_count = 0;
LOCAL uint32 ai_total_content_len = 0;
//LOCAL uint32 ai_total_rcv_len = 0;
//LOCAL uint16 ai_over_ram_count = 0;
LOCAL BREAK_CONTINUE_T ai_break_continue = {0};
LOCAL uint32 last_data_ind_time_sec = 0;
LOCAL uint8 ai_check_finish_timer_id = 0;




LOCAL uint16 ai_cur_select_url_idx = 0;
BOOLEAN     g_ai_http_rcv_status = FALSE;

static char ai_cur_http_url[AI_HTTP_URL_MAX_LEN]= {0};


LOCAL void AI_Http_CheckDownloadTimerFunc(uint8 timer_id,
                                uint32 param)
{
    AI_HTTP_LOG("AI__HTTP AI_Http_CheckDownloadTimerFunc ");
    if(AI_IsInAiChatWin())
    {
        if(ai_total_content_len > ai_break_continue.already_rcv_len && !ai_break_continue.need_next_continue)
        {
            AI_HTTP_LOG("AI__HTTP AI_Http_CheckDownloadTimerFunc exception stoped , need continue again");
            //AI_HttpCancelDownload();
            //AI_HttpForceClose();
            
            ai_break_continue.need_next_continue = TRUE;
            //AIHttpDefaultParamsInit();
            StartAiHttpRequest();
        }
    }
}

LOCAL void AI_Http_StopCheckDownloadFinishTimer()
{
    if(0 != ai_check_finish_timer_id)
    {
        MMK_StopTimer(ai_check_finish_timer_id);
        ai_check_finish_timer_id = 0;
    }
}


LOCAL void AI_Http_StartCheckDownloadFinishTimer()
{
    if(0 != ai_check_finish_timer_id)
    {
        MMK_StopTimer(ai_check_finish_timer_id);
        ai_check_finish_timer_id = 0;
    } 
    AI_HTTP_LOG("AI__HTTP AI_Http_StartCheckDownloadFinishTimer ");
    ai_check_finish_timer_id = MMK_CreateTimerCallback(1500, AI_Http_CheckDownloadTimerFunc, 0, FALSE);                                                                                                                                                                                                                   
}


LOCAL void ClearBreakContinue()
{
    //SCI_MEMSET(&ai_break_continue, 0, sizeof(BREAK_CONTINUE_T));//clear break continue
    ai_break_continue.already_rcv_len = 0;
    ai_break_continue.break_count = 0;
    ai_break_continue.just_stop_by_break = FALSE;
    ai_break_continue.need_next_continue = FALSE;
    ai_total_content_len = 0;
}

PUBLIC void AIHttpDefaultParamsInit()
{
    s_aireghttphttp.flags = FALSE;
    s_aireghttphttp.need_refresh = FALSE;
    s_aireghttphttp.sim_sys = MN_DUAL_SYS_1;
    s_aireghttphttp.net_id = MMIZDT_Net_GetNetID();
}

PUBLIC void AiSetHttpUrl(char* url)
{
    if(url && SCI_STRLEN(url) > 0)
    {
       SCI_MEMSET(ai_cur_http_url, 0, AI_HTTP_URL_MAX_LEN*sizeof(char));
       SCI_STRCPY(ai_cur_http_url, url);
       ClearBreakContinue();
    }
}




//yangyu add
PUBLIC void StartAiHttpRequest()
{
    HTTP_INIT_PARAM_T* init_param_ptr = PNULL;
    init_param_ptr = SCI_ALLOC_APPZ(sizeof(HTTP_INIT_PARAM_T));
    init_param_ptr->is_cookie_enable = FALSE;
    AI_HTTP_LOG("AI__HTTP [aireghttp]ai StartAiHttpRequest init_param_ptr->is_cookie_enable = FALSE; \n");
    init_param_ptr->is_cache_enable = FALSE;
#ifdef HTTP_SUPPORT
    AI_HTTP_LOG("AI__HTTP [aireghttp]ai HTTP_InitRequest");
    HTTP_InitRequest(MMI_MODULE_TULINGAI, 1, init_param_ptr);
#endif
    SCI_FREE(init_param_ptr);
    init_param_ptr = PNULL;
}

/*    
LOCAL void StartAiHttpRequest()
{
    HTTP_INIT_PARAM_T* init_param_ptr = PNULL;
    init_param_ptr = SCI_ALLOC_APPZ(sizeof(HTTP_INIT_PARAM_T));
    init_param_ptr->is_cookie_enable = FALSE;
    AI_HTTP_LOG("AI__HTTP [aireghttp] StartAiHttpRequest init_param_ptr->is_cookie_enable = FALSE; \n");
    init_param_ptr->is_cache_enable = FALSE;
#ifdef HTTP_SUPPORT
    HTTP_InitRequest(MMI_MODULE_TULINGAI, 1, init_param_ptr);
#endif
    SCI_FREE(init_param_ptr);
    init_param_ptr = PNULL;
}
*/
void AiHttp_GetSignalStruct(DPARAM param, void *signal_struct_ptr, uint16 struct_size)
{
    if((PNULL != param)&&(PNULL != signal_struct_ptr)&&(struct_size >= sizeof(xSignalHeaderRec)))
    {
        SCI_MEMCPY((uint8 *)((uint32)signal_struct_ptr), param, (struct_size ));
    }
}

LOCAL void HandleAiHttpPdpMsg(MMIPDP_CNF_INFO_T *msg_ptr)
{
	if(PNULL == msg_ptr)
    	{
        	return;
    	}
AI_HTTP_LOG("AI__HTTP HandleAiHttpPdpMsg ==msg_id =%d,result=%d",msg_ptr->msg_id,msg_ptr->result);
    	switch(msg_ptr->msg_id) 
    	{
	    case MMIPDP_ACTIVE_CNF:     //PDP激活成功
        {
            if(MMIPDP_RESULT_SUCC == msg_ptr->result)
            {
            AI_HTTP_LOG("AI__HTTP HandleAiHttpPdpMsg == msg_ptr->nsapi=%d ",msg_ptr->nsapi);
            s_aireghttphttp.net_id = msg_ptr->nsapi;
            StartAiHttpRequest();
            }
#if defined(MMI_GPRS_SUPPORT) || defined(WIFI_SUPPORT)
            else
            {
                MMIAPIPDP_Deactive(MMI_MODULE_TULINGAI);
            if (0 == g_aireghttp_global.retry_timer_id)
            {
                g_aireghttp_global.retry_timer_id = MMK_CreateTimerCallback(AIREGHTTP_TIMEOUT_180S, MmiAiHttp_HandleRetryTimerMsg, NULL, FALSE);
                AI_HTTP_LOG("AI__HTTP active fail 3 minite timer start, timer id = %d", g_aireghttp_global.retry_timer_id);
            }
            }
#endif
        }
            break;	        
        case MMIPDP_DEACTIVE_CNF:   //PDP去激活成功
            s_aireghttphttp.net_id = 0;
            break;
        
	    case MMIPDP_DEACTIVE_IND:   //PDP被网络端去激活
            s_aireghttphttp.net_id = 0;
#if defined(MMI_GPRS_SUPPORT) || defined(WIFI_SUPPORT)
			MMIAPIPDP_Deactive(MMI_MODULE_TULINGAI);
#endif
	        break;
	    default:
	        break;
	}

}

PUBLIC BOOLEAN AiHttpPDP()
{
    int i = 0;
    uint8 index = 0;
    uint8 setting_index = 0;
    BOOLEAN return_val = FALSE;
    MMIPDP_ACTIVE_INFO_T active_info = {0};
    MMICONNECTION_APNTYPEINDEX_T *apn_idx_arr;
    MMICONNECTION_LINKSETTING_DETAIL_T* linksetting = PNULL;
    MN_DUAL_SYS_E sim_sys = s_aireghttphttp.sim_sys;

    //return FALSE;
    if(MMIAPISET_GetFlyMode() || (MMIAPIPHONE_GetSimOkNum(NULL,0)==0)
        ||(MMICC_GetCallNum()>0))
    {
        return return_val;
    }
 
    s_aireghttphttp.flags = FALSE;   
    apn_idx_arr = MMIAPICONNECTION_GetApnTypeIndex(sim_sys);
    if (apn_idx_arr == NULL)
    {
        AI_HTTP_LOG("AI NET AiHttpPDP [APN IDX ERROR]");
        return FALSE;
    }
    AI_HTTP_LOG("AI NET  AiHttpPDP [sim = %d, Net Setting = %d]", sim_sys, apn_idx_arr->internet_index[sim_sys].index);
    linksetting = MMIAPICONNECTION_GetLinkSettingItemByIndex(sim_sys, apn_idx_arr->internet_index[sim_sys].index);
    active_info.app_handler         = MMI_MODULE_TULINGAI;
    active_info.dual_sys            = sim_sys;
    active_info.apn_ptr             = NULL;
    active_info.user_name_ptr       = NULL;
    active_info.psw_ptr             = NULL;
    active_info.priority            = 3;
    active_info.ps_service_rat      = MN_TD_PREFER;
    active_info.ps_interface        = MMIPDP_INTERFACE_GPRS;
    active_info.handle_msg_callback = HandleAiHttpPdpMsg;
    active_info.ps_service_type     = BROWSER_E;
    active_info.storage             = MN_GPRS_STORAGE_ALL;
    if(PNULL != linksetting && 0 != linksetting->apn_len)
    {
        active_info.apn_ptr = (char*)linksetting->apn;
        active_info.user_name_ptr = (char*)linksetting->username;
        active_info.psw_ptr = (char*)linksetting->password;
#ifdef IPVERSION_SUPPORT_V4_V6
        active_info.ip_type = linksetting->ip_type;
#endif
        if(MMIAPIPDP_Active(&active_info))
        {
            return_val = TRUE;
            g_aireghttp_global.send_count = 0;
            g_ai_http_rcv_status = TRUE;
        }
    }
    AI_HTTP_LOG("AI__HTTP AiHttpPDP return_val = %d,moduleid=0x%x",return_val,active_info.app_handler);
    return return_val;
}

LOCAL void MmiAiHttp_Handle12HourTimerMsg(uint8 timer_id, uint32 param)
{
    BOOLEAN sim_ok_num = FALSE;
    MN_DUAL_SYS_E sim_ok = MN_DUAL_SYS_1;
    BOOLEAN send_again = TRUE;	
    AI_HTTP_LOG("AI__HTTP  12 hour timer end!");

    if (timer_id == g_aireghttp_global.timer_12h_id)
    {
        sim_ok_num =MMIAPIPHONE_GetDataServiceSIM(&sim_ok);
        s_aireghttphttp.sim_sys = sim_ok;
        g_aireghttp_global.resend_type = AIREGHTTP_12H_ALIVE_MSG;	

        if(FALSE == sim_ok_num)
        {		
            AI_HTTP_LOG("AI__HTTP  can not send alive message because of no sim available!");
        }
        else if(MMIAPIPDP_GetPdpLinkSIM() != MMI_DUAL_SYS_MAX)
        {
            AI_HTTP_LOG("AI__HTTP can not send alive message because of pdp is linked"); 
        }
        else
        { 
            g_aireghttp_global.assert_flag = FALSE;
            //if(MMIAI_HTTP_Check_NeedRcv(sim_ok))
            {
                if(MMIDEFAULT_GetBackLightStateEx() == MMI_BACKLIGHT_OFF)
                {
                    if(!AiHttpPDP())
                    {
                        send_again = TRUE;
                    }
                }
                else
                {
                    send_again = TRUE;
                }
            }
        }
    }

    if(send_again)
    {
        send_again = FALSE;

        if (0 == g_aireghttp_global.retry_timer_id)
        {
            g_aireghttp_global.send_count = 0;
            g_aireghttp_global.retry_timer_id = MMK_CreateTimerCallback(AIREGHTTP_TIMEOUT_180S, MmiAiHttp_HandleRetryTimerMsg, NULL, FALSE);
            AI_HTTP_LOG("AI__HTTP 12h live 3 minite send again timer start, timer id = %d", g_aireghttp_global.retry_timer_id);
        }
    }
}
PUBLIC void MmiAutoSMsHttp_SendResetMessage()
{
    BOOLEAN enable_flag = FALSE;
    uint8   assert_mode = 0;
    BOOLEAN sim_ok_num = FALSE;
    MN_DUAL_SYS_E sim_ok = MN_DUAL_SYS_1;
	
    AI_HTTP_LOG("AI__HTTP  Start send reset message.");
    OSPARAM_GetAssertMode (&assert_mode);
    AI_HTTP_LOG("AI__HTTP  assert_mode = %d", assert_mode);
#if 0	
    if (SCI_ASSERT_DEBUG_MODE == assert_mode)
    {
        if ((POWER_GetRestartCondition() == RESTART_BY_SOFTWARE) || (POWER_GetRestartCondition() == RESTART_BY_POWER_BUTTON))
        {
            enable_flag = TRUE;
        }
    }
    else
#endif		
    {
        if (POWER_GetRestartCondition() == RESTART_BY_ASSERT)
        {
            enable_flag = TRUE;
        }
    }

    if (enable_flag)
    {	
        sim_ok_num =MMIAPIPHONE_GetDataServiceSIM(&sim_ok);	
        s_aireghttphttp.sim_sys = sim_ok;
        g_aireghttp_global.resend_type = AIREGHTTP_RESET_MSG;
        if(FALSE == sim_ok_num)
        {
            AI_HTTP_LOG("AI__HTTP  can not send alive message because of no sim available!");
        }
        else if(MMIAPIPDP_GetPdpLinkSIM() != MMI_DUAL_SYS_MAX)
        {
            AI_HTTP_LOG("AI__HTTP can not send alive message because of pdp is linked"); 
        }
        else
        {
            g_aireghttp_global.assert_flag = TRUE;
            //if(MMIAI_HTTP_Check_NeedRcv(sim_ok))
            {
                if(AiHttpPDP())
                {
                    g_aireghttp_global.reset_sendstatus = AIREGHTTP_RESET_SEND;
                }
            }
        }
    }
}
	
LOCAL void MmiAiHttp_HandleRetryTimerMsg(uint8 timer_id, uint32 param)
{
    BOOLEAN sim_ok_num = FALSE;
    MN_DUAL_SYS_E sim_ok = MN_DUAL_SYS_1;
    BOOLEAN send_again = FALSE;	
    AI_HTTP_LOG("AI__HTTP  MMIAIREGHTTP_HandleRetryTimerMsg Enter...");

    if (timer_id == g_aireghttp_global.retry_timer_id)
    {
        sim_ok_num =MMIAPIPHONE_GetDataServiceSIM(&sim_ok);	
        s_aireghttphttp.sim_sys = sim_ok;
        if(FALSE == sim_ok_num)
        { 
            if (AIREGHTTP_RESET_RESEND == g_aireghttp_global.reset_sendstatus)
            {
                g_aireghttp_global.reset_sendstatus = AIREGHTTP_RESET_SEND_END;
            }
            g_aireghttp_global.resend_count = 3;
            send_again = TRUE;		
            AI_HTTP_LOG("AI__HTTP  retry send fail because of no sim available!");
        }
        else if(MMIAPIPDP_GetPdpLinkSIM() != MMI_DUAL_SYS_MAX)
        {
            if (AIREGHTTP_RESET_RESEND == g_aireghttp_global.reset_sendstatus)
            {
                g_aireghttp_global.reset_sendstatus = AIREGHTTP_RESET_SEND_END;
            }
            g_aireghttp_global.resend_count = 3;
            send_again = TRUE;	
            AI_HTTP_LOG("AI__HTTP can not send alive message because of pdp is linked"); 
        }
        else
        {     
            if (AIREGHTTP_RESET_MSG == g_aireghttp_global.resend_type)
            {
                AI_HTTP_LOG("AI__HTTP  resend reset message ...");
                g_aireghttp_global.assert_flag = TRUE;
            } 
            else
            {
                AI_HTTP_LOG("AI__HTTP  resend alive message ...");
                g_aireghttp_global.assert_flag = FALSE;
            }
            g_aireghttp_global.resend_count = 3;
            //if(MMIAI_HTTP_Check_NeedRcv(sim_ok))
            {
                if(MMIDEFAULT_GetBackLightStateEx() == MMI_BACKLIGHT_OFF)
                {
                    if(!AiHttpPDP())
                    {
                        send_again = TRUE;
                    }
                }
                else
                {
                    send_again = TRUE;
                }
            }
        }
    }

    AI_HTTP_LOG("AI__HTTP  timer_id = %d, retry timer id = %d", timer_id, g_aireghttp_global.retry_timer_id);
    MMK_StopTimer(g_aireghttp_global.retry_timer_id);
    g_aireghttp_global.retry_timer_id = 0;
    if(send_again)
    {
        send_again = FALSE;
        if(g_aireghttp_global.send_count > AIREGHTTP_SEND_COUNT)
        {
                g_aireghttp_global.send_count = 0;
                if(g_aireghttp_global.resend_type == AIREGHTTP_ALIVE_MSG)
                {
                    //MmiAutoSMsHttp_SendResetMessage();
                }
                return;	   
        }
        if (0 == g_aireghttp_global.retry_timer_id)
        {
            g_aireghttp_global.send_count ++;
            g_aireghttp_global.retry_timer_id = MMK_CreateTimerCallback(AIREGHTTP_TIMEOUT_180S, MmiAiHttp_HandleRetryTimerMsg, NULL, FALSE);
            AI_HTTP_LOG("AI__HTTP retry timer send again timer start, timer id = %d,send_count=%d", g_aireghttp_global.retry_timer_id,g_aireghttp_global.send_count);
        }
    }
	
}
LOCAL void MmiAiHttp_postcallback(uint8 timer_id,uint32 param)
{
  if (timer_id == g_aireghttp_global.sec_timer_id)
  {
    if(s_aireghttphttp.flags == TRUE)
    {
        AI_HTTP_LOG("AI__HTTP  g_aireghttp_global.reset_sendstatus = %d", g_aireghttp_global.reset_sendstatus);
        if (AIREGHTTP_RESET_SEND == g_aireghttp_global.reset_sendstatus || AIREGHTTP_RESET_RESEND == g_aireghttp_global.reset_sendstatus)
        {
            g_aireghttp_global.reset_sendstatus = AIREGHTTP_RESET_SEND_END;
            AI_HTTP_LOG("AI__HTTP  reset message have been send success.");
        }
        else
        {
            AI_HTTP_LOG("AI__HTTP  alive message Send success!!!");
            
            if (AIREGHTTP_RESET_NONE == g_aireghttp_global.reset_sendstatus)
            {
                MmiAutoSMsHttp_SendResetMessage();
            }
        }
        
        g_aireghttp_global.resend_count = 3;
    	}
	else
    	{	
        //start timer and retry
        AI_HTTP_LOG("AI__HTTP  resend count leave times = %d", g_aireghttp_global.resend_count);
        
        g_aireghttp_global.reset_sendstatus = (g_aireghttp_global.reset_sendstatus == AIREGHTTP_RESET_SEND) ? AIREGHTTP_RESET_RESEND:g_aireghttp_global.reset_sendstatus;
        
        if (g_aireghttp_global.resend_count > 0)
        {
            g_aireghttp_global.resend_count--;
            if(g_aireghttp_global.retry_timer_id  == 0)		
                g_aireghttp_global.retry_timer_id = MMK_CreateTimerCallback(AIREGHTTP_TIMEOUT_180S, MmiAiHttp_HandleRetryTimerMsg, NULL, FALSE);
        }
        else
        {
            g_aireghttp_global.resend_count = 3;
            
            if (AIREGHTTP_RESET_NONE == g_aireghttp_global.reset_sendstatus)
            {
                MmiAutoSMsHttp_SendResetMessage();
            }
            else
            {
                g_aireghttp_global.reset_sendstatus = AIREGHTTP_RESET_SEND_END;
            }
        }
    }
  }
     AI_HTTP_LOG("AI__HTTP  timer_id = %d, sec_timer_id = %d", timer_id, g_aireghttp_global.sec_timer_id);
    MMK_StopTimer(g_aireghttp_global.sec_timer_id);
    g_aireghttp_global.sec_timer_id = 0; 	
}

LOCAL void MmiAiHttp_getcallback(uint8 timer_id,uint32 param)
{
    if (timer_id == g_aireghttp_global.sec_timer_id)
    {
        if(s_aireghttphttp.flags == TRUE)
        {
            AI_HTTP_LOG("AI__HTTP  getcallback OK");
            g_aireghttp_global.resend_count = 3;
        }
        else
        {	
            AI_HTTP_LOG("AI__HTTP  getcallback FAIL resend_count=%d",g_aireghttp_global.resend_count);
            if (g_aireghttp_global.resend_count > 0)
            {
                g_aireghttp_global.resend_count--;
                if(g_aireghttp_global.retry_timer_id  == 0)		
                    g_aireghttp_global.retry_timer_id = MMK_CreateTimerCallback(AIREGHTTP_TIMEOUT_180S, MmiAiHttp_HandleRetryTimerMsg, NULL, FALSE);
            }
            else
            {
                g_aireghttp_global.resend_count = 3;
            }
        }
    }
    AI_HTTP_LOG("AI__HTTP  getcallback timer_id = %d, sec_timer_id = %d,flags=%d", timer_id, g_aireghttp_global.sec_timer_id,s_aireghttphttp.flags);
    MMK_StopTimer(g_aireghttp_global.sec_timer_id);
    g_aireghttp_global.sec_timer_id = 0; 	
}
LOCAL MMI_RESULT_E HandleAiHttpPostCnf(AIREGHTTP_HTTP_CONTEXT * http_context_ptr, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
#ifdef HTTP_SUPPORT
    HTTP_POST_CNF_SIG_T signal = {0};
    AIREGHTTP_HTTP_SIGNAL_ANALYTIC(param,signal,http_context_ptr,HTTP_POST_CNF_SIG_T);
	AI_HTTP_LOG("AI__HTTP HandleAiHttpPostCnf code=%d",signal.rsp_header_info.response_code);
    if(signal.rsp_header_info.response_code == 200)
    {
        s_aireghttphttp.flags = TRUE;
    }
    else
    {	
        s_aireghttphttp.flags = FALSE;
    }	
    AI_HTTP_LOG("AI__HTTP HandleAiHttpPostCnf HTTP_CloseRequest");
    HTTP_CloseRequest(http_context_ptr->context_id);   
#endif
    return result;
}

LOCAL MMI_RESULT_E HandleAiHttpGetCnf(AIREGHTTP_HTTP_CONTEXT * http_context_ptr, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
#ifdef HTTP_SUPPORT
    HTTP_GET_CNF_SIG_T signal = {0};
    AIREGHTTP_HTTP_SIGNAL_ANALYTIC(param,signal,http_context_ptr,HTTP_GET_CNF_SIG_T);
		AI_HTTP_LOG("AI__HTTP HandleAiHttpGetCnf code=%d,need_refresh=%d , content_length=%ld",signal.rsp_header_info.response_code,s_aireghttphttp.need_refresh
		    , signal.rsp_header_info.content_length);
	#if 0
	AI_HTTP_LOG("AI__HTTP HandleAiHttpGetCnf code=%d,need_refresh=%d",signal.rsp_header_info.response_code,s_aireghttphttp.need_refresh);
    if(s_aireghttphttp.need_refresh)
    {
        AI_HTTP_Refresh_Send(&s_refresh_ind);
        s_aireghttphttp.need_refresh = FALSE;
    }
    else
    {
        if(ai_cur_rcv_count < AIREGHTTP_RCV_COUNT)
        {
            if(signal.rsp_header_info.response_code != 200)
            {
                SCI_MEMSET(&ai_cur_url,0,sizeof(AI_HTTP_REFRESH_IND_T));
                if(ai_cur_select_url_idx == 0)
                {
                    ai_cur_select_url_idx = 1;
                    strcpy((char*)ai_cur_url.url_arr,AIREGHTTP_HTTP_URI_STRING_TEST);
                }
                else
                {
                    ai_cur_select_url_idx = 0;
                    strcpy((char*)ai_cur_url.url_arr,AIREGHTTP_HTTP_URI_STRING_TEST);
                }
                ai_cur_url.refresh_time = 0;
            }
            s_refresh_ind = ai_cur_url;
            AI_HTTP_Refresh_Send(&s_refresh_ind);
        }
        else
        {
            MMIAI_HTTP_RcvSet(ai_cur_rcv_count);
            ai_cur_rcv_count = 0;
            if(signal.rsp_header_info.response_code == 200)
            {
                s_aireghttphttp.flags  = TRUE;
            }
            else
            {	
                s_aireghttphttp.flags = FALSE;
            }		
            HTTP_CloseRequest(http_context_ptr->context_id);
        }
    }
	#else
	 if(signal.rsp_header_info.response_code == 200)
    {
        s_aireghttphttp.flags = TRUE;
    }
    else
    {	
        s_aireghttphttp.flags = FALSE;
    }		
    HTTP_CloseRequest(http_context_ptr->context_id);
    AI_HTTP_LOG("AI__HTTP HandleAiHttpGetCnf HTTP_CloseRequest");
	#endif
#endif
    return result;
}
LOCAL MMI_RESULT_E HandleAiHttpPostRequest(AIREGHTTP_HTTP_CONTEXT * http_context_ptr, DPARAM param)
{
    int base_len = 0;
    MMI_RESULT_E        result = MMI_RESULT_FALSE;
    HTTP_INIT_CNF_SIG_T signal = {0};
    HTTP_APP_PARAM_T app_param = 0;
    HTTP_POST_PARAM_T* post_param_ptr= NULL;
    uint8* ptr = NULL;
    int file_read = 0;
    int len = 0;
    uint32 tickcount = 0;
    uint8 ai_ptr[1024]= {""};

    if(http_context_ptr == NULL)
    {
        return MMI_RESULT_FALSE;
    }

	AIREGHTTP_HTTP_SIGNAL_ANALYTIC_EX(param, signal, app_param, http_context_ptr,MMI_MODULE_TULINGAI,HTTP_INIT_CNF_SIG_T);
	AI_HTTP_LOG("AI__HTTP HandleAiHttpPostRequest receive HTTP_SIG_INIT_CNF");
	post_param_ptr = (HTTP_POST_PARAM_T*)SCI_ALLOC_APPZ(sizeof(HTTP_POST_PARAM_T));

#if 0
	ptr = SCI_ALLOC_APPZ(MAX_AIREGHTTP_BUF);
	if(ptr == NULL) return result;
	SCI_MEMSET(ptr,0x0,MAX_AIREGHTTP_BUF);
#endif
	if(post_param_ptr == NULL)
	{
		return MMI_RESULT_FALSE;
	}	
	memset(post_param_ptr,0,sizeof(HTTP_POST_PARAM_T));
	
	post_param_ptr->connection = HTTP_CONNECTION_KEEP_ALIVE;
	post_param_ptr->need_net_prog_ind = FALSE;
	
	len=strlen(AIREGHTTP_ACCEPT_STRING);
	post_param_ptr->accept.accept_ptr = SCI_ALLOC(len+1);
	memset(post_param_ptr->accept.accept_ptr,0,len+1);
	strcpy(post_param_ptr->accept.accept_ptr,AIREGHTTP_ACCEPT_STRING);
#if 0

	len=strlen(AIREGHTTP_ACCEPT_LANGUAGE_STRING);
	post_param_ptr->accept_language.accept_language_ptr = SCI_ALLOC(len+1);
	memset(post_param_ptr->accept_language.accept_language_ptr,0,len+1);
	strcpy(post_param_ptr->accept_language.accept_language_ptr,AIREGHTTP_ACCEPT_LANGUAGE_STRING);	
	len=strlen(AIREGHTTP_USER_AGENT_STRING);
	post_param_ptr->user_agent.user_agent_ptr = SCI_ALLOC(len+1);
	memset(post_param_ptr->user_agent.user_agent_ptr,0,len+1);
	strcpy(post_param_ptr->user_agent.user_agent_ptr,AIREGHTTP_USER_AGENT_STRING);	
#endif
	/***************数据体*****************/
    len=strlen(HTTP_AIREGHTTP_CONTENT_TYPE);
    post_param_ptr->post_body.body_type_ptr= SCI_ALLOC(len+1);
    memset(post_param_ptr->post_body.body_type_ptr,0,len+1);
    strcpy(post_param_ptr->post_body.body_type_ptr,HTTP_AIREGHTTP_CONTENT_TYPE);
       //base_len = AiHttp_WritePostBodyBase64(&ptr);
      // base_len = AiHttp_WritePostBody(&ptr);

    CTAIHttp_WritePostBody(ai_ptr);
    AI_HTTP_LOG("AI__HTTP [aireghttp] ai_ptr=%s\n",ai_ptr);
    base_len = strlen(ai_ptr);
    AI_HTTP_LOG("AI__HTTP [aireghttp] base_len = %d",base_len);


       
    if(base_len <= 0)
    {
            SCI_FREE(post_param_ptr->accept.accept_ptr);
            SCI_FREE(post_param_ptr->post_body.body_type_ptr);
            SCI_Free(post_param_ptr);
            return MMI_RESULT_FALSE;
    }
    post_param_ptr->post_body.is_use_file = FALSE;
    post_param_ptr->post_body.u.post_buffer.is_copied_by_http= TRUE;
    post_param_ptr->post_body.u.post_buffer.buffer_len=base_len;
    post_param_ptr->post_body.u.post_buffer.buffer_ptr= ai_ptr ;///ptr;

    AI_HTTP_LOG("AI__HTTP [aireghttp] len=%d,ptr =%s",post_param_ptr->post_body.u.post_buffer.buffer_len,post_param_ptr->post_body.u.post_buffer.buffer_ptr);

    //拼接URL
    len=strlen(AIREGHTTP_HTTP_URI_STRING_TEST);
    post_param_ptr->uri.uri_ptr= SCI_ALLOC(len+1);
    memset(post_param_ptr->uri.uri_ptr,0,len+1);
    strcpy(post_param_ptr->uri.uri_ptr,AIREGHTTP_HTTP_URI_STRING_TEST);
    AI_HTTP_LOG("AI__HTTP HandleAiHttpPostRequest == post_param_ptr->uri.uri_ptr=%s,len=%d ",post_param_ptr->uri.uri_ptr,len);
#if 0
    len = strlen(AIREGHTTP_ACCEPT_CHARSET_STRING);
    post_param_ptr->accept_charset.accept_charset_ptr = SCI_ALLOC(len + 1);
    memset(post_param_ptr->accept_charset.accept_charset_ptr,0,len + 1);
    strcpy(post_param_ptr->accept_charset.accept_charset_ptr,AIREGHTTP_ACCEPT_CHARSET_STRING);
#endif
#ifdef HTTP_SUPPORT
	if(!HTTP_PostRequest(http_context_ptr->context_id,post_param_ptr, http_context_ptr->app_instance))
	{
		result = MMI_RESULT_TRUE;
	}
#endif
    SCI_FREE(post_param_ptr->accept.accept_ptr);
    //SCI_FREE(post_param_ptr->accept_charset.accept_charset_ptr);
    //SCI_FREE(post_param_ptr->accept_language.accept_language_ptr);
    SCI_FREE(post_param_ptr->post_body.body_type_ptr);
    //SCI_FREE(post_param_ptr->user_agent.user_agent_ptr);
    SCI_FREE(post_param_ptr->uri.uri_ptr);
    SCI_Free(post_param_ptr);
    SCI_Free(ptr);
    return	result;	
}

LOCAL MMI_RESULT_E HandleAiHttpGetRequest(AIREGHTTP_HTTP_CONTEXT * http_context_ptr, DPARAM param)
{
      int base_len = 0;
    MMI_RESULT_E        result = MMI_RESULT_FALSE;
    HTTP_INIT_CNF_SIG_T signal = {0};
    HTTP_APP_PARAM_T app_param = 0;
    HTTP_GET_PARAM_T* get_param_ptr= NULL;
    int file_read = 0;
    int len = 0;
    uint32 tickcount = 0;
    AI_HTTP_LOG("AI__HTTP [aireghttp]HandleAiHttpGetRequest");

        if(http_context_ptr == NULL)
        {
            return MMI_RESULT_FALSE;
        }
    AI_HTTP_LOG("AI__HTTP [aireghttp]HandleAiHttpGetRequest 000");

    AIREGHTTP_HTTP_SIGNAL_ANALYTIC_EX(param, signal, app_param, http_context_ptr,MMI_MODULE_TULINGAI,HTTP_INIT_CNF_SIG_T);
    AI_HTTP_LOG("AI__HTTP HandleAiHttpGetRequest receive HTTP_SIG_INIT_CNF");
    get_param_ptr = (HTTP_GET_PARAM_T*)SCI_ALLOC_APPZ(sizeof(HTTP_GET_PARAM_T));

    if(get_param_ptr == NULL)
    {
    	return MMI_RESULT_FALSE;
    }	
    memset(get_param_ptr,0,sizeof(HTTP_GET_PARAM_T));

    get_param_ptr->connection = HTTP_CONNECTION_KEEP_ALIVE;
    get_param_ptr->need_net_prog_ind = FALSE;
    len=strlen(AIREGHTTP_ACCEPT_STRING);
    get_param_ptr->accept.accept_ptr = SCI_ALLOC(len+1);
    memset(get_param_ptr->accept.accept_ptr,0,len+1);
    strcpy(get_param_ptr->accept.accept_ptr,AIREGHTTP_ACCEPT_STRING);
    len=strlen(AIREGHTTP_ACCEPT_LANGUAGE_STRING);
    get_param_ptr->accept_language.accept_language_ptr = SCI_ALLOC(len+1);
    memset(get_param_ptr->accept_language.accept_language_ptr,0,len+1);
    strcpy(get_param_ptr->accept_language.accept_language_ptr,AIREGHTTP_ACCEPT_LANGUAGE_STRING);	
    len=strlen(AIREGHTTP_USER_AGENT_STRING);
    get_param_ptr->user_agent.user_agent_ptr = SCI_ALLOC(len+1);
    memset(get_param_ptr->user_agent.user_agent_ptr,0,len+1);
    strcpy(get_param_ptr->user_agent.user_agent_ptr,AIREGHTTP_USER_AGENT_STRING);	

    //拼接URL
    len=strlen((char *)ai_cur_url.url_arr);
    get_param_ptr->uri.uri_ptr= SCI_ALLOC(len+1);
    memset(get_param_ptr->uri.uri_ptr,0,len+1);
    strcpy(get_param_ptr->uri.uri_ptr,(char *)ai_cur_url.url_arr);
    AI_HTTP_LOG("AI__HTTP HandleAiHttpGetRequest == get_param_ptr->uri.uri_ptr=%s,len=%d ",get_param_ptr->uri.uri_ptr,len);

    len = strlen(AIREGHTTP_ACCEPT_CHARSET_STRING);
    get_param_ptr->accept_charset.accept_charset_ptr = SCI_ALLOC(len + 1);
    memset(get_param_ptr->accept_charset.accept_charset_ptr,0,len + 1);
    strcpy(get_param_ptr->accept_charset.accept_charset_ptr,AIREGHTTP_ACCEPT_CHARSET_STRING);

    if(ai_break_continue.already_rcv_len >= ai_total_content_len && ai_total_content_len > 0)
    {
        AI_HTTP_LOG("AI__HTTP HandleAiHttpGetRequest malloc header  fail");
        //return MMI_RESULT_FALSE;
        result = MMI_RESULT_FALSE;
        goto FREE_ALL;
    }
    //if(MMIAI_DataRcvIsBreak())
    if(!MMIAI_IsDownloadFinish())
    {

        char begin_bytes[DL_RANGE_HEADER_LEN] = {0};
        uint32 end_pos = 0;
        if(ai_total_content_len == 0)
        {
            end_pos = MP3_HTTP_BUFF_SIZE -1;
        }
        else
        {
            end_pos = ai_break_continue.already_rcv_len + MP3_HTTP_BUFF_SIZE -1;
            if(end_pos > ai_total_content_len - 1)
            {
                end_pos = ai_total_content_len - 1;
            }

        }
       

        get_param_ptr->extern_header.header_num = 1;
        get_param_ptr->extern_header.other_header_ptr = (HTTP_OTHER_HEADER_T *)SCI_ALLOCAZ(sizeof(HTTP_OTHER_HEADER_T));
        //SCI_MEMSET(get_param_ptr->extern_header.other_header_ptr, 0, sizeof(HTTP_OTHER_HEADER_T));
        if(PNULL ==  get_param_ptr->extern_header.other_header_ptr)
        {
            AI_HTTP_LOG("AI__HTTP [aireghttp]HandleAiHttpGetRequest malloc header  fail");
            //return MMI_RESULT_FALSE; //TODO
            result = MMI_RESULT_FALSE;
            goto FREE_ALL;
        }

        len = strlen("Range");
        get_param_ptr->extern_header.other_header_ptr->header_name_ptr = SCI_ALLOCAZ(len + 1);
        //SCI_MEMSET(get_param_ptr->extern_header.other_header_ptr->header_name_ptr, 0, (len + 1));
        if(PNULL ==  get_param_ptr->extern_header.other_header_ptr->header_name_ptr)
        {
            SCI_FREE(get_param_ptr->extern_header.other_header_ptr);
            AI_HTTP_LOG("AI__HTTP [aireghttp]HandleAiHttpGetRequest malloc header name ptr fail");
            //return MMI_RESULT_FALSE; //TODO
            result = MMI_RESULT_FALSE;
            goto FREE_ALL;
        }
        strcpy(get_param_ptr->extern_header.other_header_ptr->header_name_ptr, "Range");
        AI_HTTP_LOG("AI__HTTP HandleAiHttpGetRequest get, start_pos = %ld , end_pos=%ld" ,ai_break_continue.already_rcv_len, end_pos);
        sprintf(begin_bytes, "%ld-%ld", ai_break_continue.already_rcv_len, end_pos);
        len = strlen(DL_RANGE_HEADER_PREFIX) + strlen(begin_bytes);
        get_param_ptr->extern_header.other_header_ptr->header_value_ptr = SCI_ALLOCAZ(len + 1);
        //SCI_MEMSET(get_param_ptr->extern_header.other_header_ptr->header_value_ptr, 0, (len + 1));
        if(PNULL ==  get_param_ptr->extern_header.other_header_ptr->header_value_ptr)
        {
            SCI_FREE(get_param_ptr->extern_header.other_header_ptr);
            SCI_FREE(get_param_ptr->extern_header.other_header_ptr->header_name_ptr);
            AI_HTTP_LOG("AI__HTTP [aireghttp]HandleAiHttpGetRequest malloc header value ptr fail");
            //return MMI_RESULT_FALSE; //TODO
            result = MMI_RESULT_FALSE;
            goto FREE_ALL;
        }
        strcpy(get_param_ptr->extern_header.other_header_ptr->header_value_ptr, DL_RANGE_HEADER_PREFIX);
        strcat(get_param_ptr->extern_header.other_header_ptr->header_value_ptr, begin_bytes);
        AI_HTTP_LOG("AI__HTTP [aireghttp]HandleAiHttpGetRequest already_rcv_len=%ld, name=%s, value=%s", ai_break_continue.already_rcv_len
        ,get_param_ptr->extern_header.other_header_ptr->header_name_ptr
        ,get_param_ptr->extern_header.other_header_ptr->header_value_ptr);
    }
    get_param_ptr->get_data.style = HTTP_DATA_TRANS_STYLE_BUFFER; //yangyu add
      
#ifdef HTTP_SUPPORT
    AI_HTTP_LOG("AI__HTTP ai HTTP_GetRequest");
	if(!HTTP_GetRequest(http_context_ptr->context_id,get_param_ptr, http_context_ptr->app_instance))
	{
	    AI_HTTP_LOG("AI__HTTP HTTP_GetRequest success");
		result = MMI_RESULT_TRUE;
		//AI_Http_StopCheckDownloadFinishTimer();
		if(MMIAI_DataRcvIsBreak())
		{
            ai_break_continue.need_next_continue = FALSE;
		}
	}
#endif
FREE_ALL:
    SCI_FREE(get_param_ptr->accept.accept_ptr);
    SCI_FREE(get_param_ptr->accept_charset.accept_charset_ptr);
    SCI_FREE(get_param_ptr->accept_language.accept_language_ptr);
    SCI_FREE(get_param_ptr->user_agent.user_agent_ptr);
    SCI_FREE(get_param_ptr->uri.uri_ptr);
    
    if (PNULL != get_param_ptr->extern_header.other_header_ptr)
    {
        if (PNULL != get_param_ptr->extern_header.other_header_ptr->header_name_ptr)
        {
            SCI_FREE(get_param_ptr->extern_header.other_header_ptr->header_name_ptr);
        }

        if (PNULL != get_param_ptr->extern_header.other_header_ptr->header_value_ptr)
        {
            SCI_FREE(get_param_ptr->extern_header.other_header_ptr->header_value_ptr);
        }

        SCI_FREE(get_param_ptr->extern_header.other_header_ptr);
    }
    
    SCI_Free(get_param_ptr);
    return	result;	
}

LOCAL MMI_RESULT_E HandleAiHttpGetRefresh(AIREGHTTP_HTTP_CONTEXT * http_context_ptr, char * url)
{
      int base_len = 0;
	MMI_RESULT_E        result = MMI_RESULT_FALSE;
	HTTP_GET_PARAM_T* get_param_ptr= NULL;
	int len = 0;

        if(http_context_ptr == NULL)
        {
            return MMI_RESULT_FALSE;
        }

	AI_HTTP_LOG("AI__HTTP HandleAiHttpGetRefresh url =%s",url);
	get_param_ptr = (HTTP_GET_PARAM_T*)SCI_ALLOC_APPZ(sizeof(HTTP_GET_PARAM_T));

	if(get_param_ptr == NULL)
	{
		return MMI_RESULT_FALSE;
	}	
	memset(get_param_ptr,0,sizeof(HTTP_GET_PARAM_T));
	
    get_param_ptr->connection = HTTP_CONNECTION_KEEP_ALIVE;
    get_param_ptr->need_net_prog_ind = FALSE;
    len=strlen(AIREGHTTP_ACCEPT_STRING);
    get_param_ptr->accept.accept_ptr = SCI_ALLOC(len+1);
    memset(get_param_ptr->accept.accept_ptr,0,len+1);
    strcpy(get_param_ptr->accept.accept_ptr,AIREGHTTP_ACCEPT_STRING);
    len=strlen(AIREGHTTP_ACCEPT_LANGUAGE_STRING);
    get_param_ptr->accept_language.accept_language_ptr = SCI_ALLOC(len+1);
    memset(get_param_ptr->accept_language.accept_language_ptr,0,len+1);
    strcpy(get_param_ptr->accept_language.accept_language_ptr,AIREGHTTP_ACCEPT_LANGUAGE_STRING);	
    len=strlen(AIREGHTTP_USER_AGENT_STRING);
    get_param_ptr->user_agent.user_agent_ptr = SCI_ALLOC(len+1);
    memset(get_param_ptr->user_agent.user_agent_ptr,0,len+1);
    strcpy(get_param_ptr->user_agent.user_agent_ptr,AIREGHTTP_USER_AGENT_STRING);	

    //拼接URL
    len=strlen(url);
    get_param_ptr->uri.uri_ptr= SCI_ALLOC(len+1);
    memset(get_param_ptr->uri.uri_ptr,0,len+1);
    strcpy(get_param_ptr->uri.uri_ptr,url);
    AI_HTTP_LOG("AI__HTTP HandleAiHttpGetRefresh uri.uri_ptr=%s,len=%d ",get_param_ptr->uri.uri_ptr,len);
    
    len = strlen(AIREGHTTP_ACCEPT_CHARSET_STRING);
    get_param_ptr->accept_charset.accept_charset_ptr = SCI_ALLOC(len + 1);
    memset(get_param_ptr->accept_charset.accept_charset_ptr,0,len + 1);
    strcpy(get_param_ptr->accept_charset.accept_charset_ptr,AIREGHTTP_ACCEPT_CHARSET_STRING);
      
#ifdef HTTP_SUPPORT
    if(!HTTP_GetRequest(http_context_ptr->context_id,get_param_ptr, http_context_ptr->app_instance))
    {
        AI_HTTP_LOG("AI__HTTP HandleAiHttpGetRefresh HTTP_GetRequest");
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
LOCAL MMI_RESULT_E HandleAiHttpRequestIdInd(AIREGHTTP_HTTP_CONTEXT *http_context_ptr, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;
    HTTP_REQUEST_ID_IND_SIG_T signal = {0};
    HTTP_REQUEST_ID_IND_SIG_T *cnf = (HTTP_REQUEST_ID_IND_SIG_T*)param;
    //当request_id为BILLING_REQUEST_ID_DEFAULT_VALUE时,则没被初始化过,属于本模块的消息
    AIREGHTTP_HTTP_SIGNAL_ANALYTIC(param,signal,http_context_ptr,HTTP_REQUEST_ID_IND_SIG_T);
    AI_HTTP_LOG("AI__HTTP HandleAiHttpRequestIdInd receive HTTP_SIG_REQUEST_ID_IND");
    AI_HTTP_LOG("AI__HTTP HandleAiHttpRequestIdInd receive http_context_ptr->context_id:%d",http_context_ptr->context_id);
    http_context_ptr->context_id = signal.context_id;
    http_context_ptr->app_instance = signal.app_instance;
    http_context_ptr->request_id = signal.request_id;
    AI_HTTP_LOG("AI__HTTP HandleAiHttpRequestIdInd request_id:%ld,http_context_ptr->context_id:%d",signal.request_id,http_context_ptr->context_id);
    result = MMI_RESULT_TRUE;

    return result;
}

LOCAL MMI_RESULT_E HandleAiHttpCloseCnf(AIREGHTTP_HTTP_CONTEXT *http_context_ptr, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;
    HTTP_CLOSE_CNF_SIG_T signal = {0};
    BOOLEAN nv_value = 1;
    AI_HTTP_LOG("AI__HTTP  HandleAiHttpCloseCnf ");
    AiHttp_GetSignalStruct(param, &signal, sizeof(HTTP_CLOSE_CNF_SIG_T));
    if(http_context_ptr->context_id == signal.context_id)
    {
        AI_HTTP_LOG("AI__HTTP HandleAiHttpCloseCnf receive HTTP_SIG_CLOSE_CNF");
        if(ai_cur_rcv_count > 0)
        {
            
            //MMIAI_HTTP_RcvSet(ai_cur_rcv_count);
            ai_cur_rcv_count = 0;
        }
        http_context_ptr->context_id = 0;
        http_context_ptr->request_id = 0;
        result = MMI_RESULT_TRUE;
        // MMINV_WRITE(SALES_NV_FLAG,&nv_value);
#if defined(MMI_GPRS_SUPPORT) || defined(WIFI_SUPPORT)
        //MMIAPIPDP_Deactive(MMI_MODULE_TULINGAI);//bug , not deactive pdp
#endif
        AI_HTTP_LOG("AI__HTTP HandleAiHttpCloseCnf send again sec_timer_id =%d",g_aireghttp_global.sec_timer_id);
        g_ai_http_rcv_status = FALSE;
        if (0 == g_aireghttp_global.sec_timer_id)
        {
            //yangyu delete ,not retry
            //g_aireghttp_global.sec_timer_id = MMK_CreateTimerCallback(AIREGHTTP_TIMEOUT_60S, MmiAiHttp_getcallback, NULL, FALSE);
        }
   }
    return result;
}

LOCAL void AI_HttpRefreshTimerCallback(uint8 timer_id, uint32 param)
{
    uint8   *url_ptr = (uint8*)param;

    if (url_ptr != PNULL)
    {
        SCI_TRACE_LOW("[BRW]HttpRefreshTimerCallback");
        HandleAiHttpGetRefresh(&s_aireghttp_http_context,url_ptr);     
        SCI_FREE(url_ptr);
    }
    else
    {
        SCI_TRACE_LOW("[BRW]W HttpRefreshTimerCallback: param err");
    }
}

PUBLIC BOOLEAN AI_HTTP_Refresh_Send(AI_HTTP_REFRESH_IND_T * pdata)
{
    #if 1
        //return MMIAI_HTTP_SendSig_Refresh(PNULL);
        return FALSE;
    #else
    AI_HTTP_REFRESH_DATA_T * p_http_data = PNULL;
    BOOLEAN res = FALSE;
    uint16 str_len = 0;

    p_http_data = SCI_ALLOC_APPZ(sizeof(AI_HTTP_REFRESH_DATA_T));
    if (p_http_data == PNULL)
    {
        AI_HTTP_LOG("AI__HTTP AI_HTTP_Refresh_Send ERR ALLOC");
        return FALSE;
    }
    p_http_data->refresh_time = pdata->refresh_time;
    str_len = strlen((char *)pdata->url_arr);
    p_http_data->url_arr = SCI_ALLOC_APPZ(str_len+1);
    if (p_http_data->url_arr == PNULL)
    {
        AI_HTTP_LOG("AI__HTTP AI_HTTP_Refresh_Send ERR ALLOC URL");
        SCI_FREE(p_http_data);
        return FALSE;
    }

    SCI_MEMCPY(p_http_data->url_arr,pdata->url_arr,str_len);
    res = MMIAI_HTTP_SendSig_Refresh(p_http_data);
    return res;
    #endif
}

BOOLEAN MMIAI_HTTP_CTREG_HandleRefreshInd(DPARAM param)
{
    #if 1
        HandleAiHttpGetRefresh(&s_aireghttp_http_context,s_refresh_ind.url_arr);
    #else
    AI_HTTP_REFRESH_DATA_T ** pp_getdata = (AI_HTTP_REFRESH_DATA_T **)(param);
    AI_HTTP_REFRESH_DATA_T * ind_ptr = *pp_getdata;
    uint32  url_len = 0;
    if(ind_ptr == PNULL)
    {
        AI_HTTP_LOG("AI__HTTP [aireghttp] MMIAI_HTTP_CTREG_HandleRefreshInd ERR NULL DATA");
        return FALSE;
    }
    if(ind_ptr->url_arr != PNULL)
    {
        url_len = SCI_STRLEN((char*)ind_ptr->url_arr);
    }
    
    if (0 == url_len)
    {
        AI_HTTP_LOG("AI__HTTP [aireghttp]MMIAI_HTTP_CTREG_HandleRefreshInd: url len == 0");
        SCI_FREE(ind_ptr);
        return FALSE;
    }

    if (ind_ptr->refresh_time > 0)
    {
        MMK_CreateTimerCallback(ind_ptr->refresh_time * 1000, (MMI_TIMER_FUNC)AI_HttpRefreshTimerCallback, (uint32)ind_ptr->url_arr, FALSE);
    }
    else
    {
        HandleAiHttpGetRefresh(&s_aireghttp_http_context,ind_ptr->url_arr); 
        if(ind_ptr->url_arr != NULL)
        {
            SCI_FREE(ind_ptr->url_arr);
        }
    }        
    SCI_FREE(ind_ptr);
    #endif
    return TRUE;
}

PUBLIC void AI_HttpClose()
{
    AIREGHTTP_HTTP_CONTEXT *http_context_ptr = &s_aireghttp_http_context;
    HTTP_ERROR_E ret = HTTP_SUCCESS;
    AI_HTTP_LOG("AI__HTTP [aireghttp] AI_HttpClose flags=%d", s_aireghttphttp.flags);
    if(s_aireghttphttp.flags)
    {
        ret = HTTP_CloseRequest(http_context_ptr->context_id); 
        AI_HTTP_LOG("AI__HTTP [aireghttp] AI_HttpClose HTTP_CloseRequest ret=%d", ret); 
        s_aireghttphttp.flags = FALSE;
    }
    
}

PUBLIC void AI_HttpForceClose()
{
    AIREGHTTP_HTTP_CONTEXT *http_context_ptr = &s_aireghttp_http_context;
    HTTP_ERROR_E ret = HTTP_SUCCESS;
    AI_HTTP_LOG("AI__HTTP [aireghttp] AI_HttpForceClose flags=%d", s_aireghttphttp.flags);
    //if(s_aireghttphttp.flags)
    {
        ret = HTTP_CloseRequest(http_context_ptr->context_id); 
        AI_HTTP_LOG("AI__HTTP [aireghttp] AI_HttpForceClose HTTP_CloseRequest ret=%d", ret); 
        s_aireghttphttp.flags = FALSE;
    }
    
}

LOCAL void AI_HttpCancelDownload()
{
    AIREGHTTP_HTTP_CONTEXT *http_context_ptr = &s_aireghttp_http_context;
    HTTP_ERROR_E ret = HTTP_SUCCESS;
    AI_HTTP_LOG("AI__HTTP [aireghttp] AI_HttpCancelDownload flags=%d", s_aireghttphttp.flags);
    //if(s_aireghttphttp.flags)
    {
    
        ret = HTTP_CancelRequest(http_context_ptr->context_id, http_context_ptr->app_instance, http_context_ptr->request_id);
        AI_HTTP_LOG("AI__HTTP [aireghttp] AI_HttpCancelDownload HTTP_CancelRequest ret=%d", ret); 
        s_aireghttphttp.flags = FALSE;
    }
    
}



PUBLIC MMI_RESULT_E HandleAiHttpMsgExt(PWND app_ptr, uint16 msg_id, DPARAM param)
{
	//当result为true时，后续注册的app不需要再响应http的消息
    MMI_RESULT_E result = MMI_RESULT_FALSE;
    void    *original_param = (void*)((uint32)param);
    AIREGHTTP_HTTP_CONTEXT *http_context_ptr = &s_aireghttp_http_context;
    HTTP_SSL_CERT_UNTRUST_IND_SIG_T* cert_ptr = PNULL;
    //yangyu add for module check
    /*HTTP_INIT_CNF_SIG_T * cnf1 = (HTTP_INIT_CNF_SIG_T*)original_param;
    
    if(cnf1->module_id != MMI_MODULE_TULINGAI || 1 != cnf1->app_param)
    {
        return MMI_RESULT_FALSE;
    }*/

    //yangyu end
    AI_HTTP_LOG("AI__HTTP HandleAiHttpMsgExt == msg_id=0x%x ",msg_id);
    //第一次发送的消息比较特殊
    if(msg_id == HTTP_SIG_INIT_CNF)
    {
        //http协议服务已经准备好，开始连接服务器
        //初始化要发送的数据,get or post
        HTTP_INIT_CNF_SIG_T * cnf = (HTTP_INIT_CNF_SIG_T*)original_param;
        if(cnf->module_id != MMI_MODULE_TULINGAI || 1 != cnf->app_param)
        {
            return MMI_RESULT_FALSE;
        }
        s_aireghttphttp.need_refresh = FALSE;
        ai_cur_rcv_count = 0;
        SCI_MEMSET(&ai_cur_url,0,sizeof(AI_HTTP_REFRESH_IND_T));
        ai_cur_url.refresh_time = 0;
        ai_cur_select_url_idx = 0;
        //strcpy((char*)ai_cur_url.url_arr,AIREGHTTP_HTTP_URI_STRING_TEST);
        strcpy((char*)ai_cur_url.url_arr, ai_cur_http_url);

        AI_HTTP_LOG("AI__HTTP [aireghttp]HandleAiHttpGetRequest begin----");
        result = HandleAiHttpGetRequest(http_context_ptr, param);
        // result = HandleAiHttpPostRequest(http_context_ptr, param);

  
            return result;//后续app不用再响应
     }
     //第一次请求后，http分配给app的id
     if(msg_id == HTTP_SIG_REQUEST_ID_IND)
     {
          result = HandleAiHttpRequestIdInd(http_context_ptr, param);
          return result;
     }	 
     switch (msg_id)
     {
     case HTTP_SIG_HEADER_IND:
     	{
#ifdef HTTP_SUPPORT
            HTTP_HEADER_IND_SIG_T signal = {0};
            AIREGHTTP_HTTP_SIGNAL_ANALYTIC(param,signal,http_context_ptr,HTTP_HEADER_IND_SIG_T);
            AI_HTTP_LOG("AI__HTTP HandleAiHttpMsgExt receive HTTP_SIG_HEADER_IND");
            AI_HTTP_LOG("AI__HTTP response_code:%d, , content_length=%ld",signal.rsp_header_info.response_code, signal.rsp_header_info.content_length);
            AI_HTTP_LOG("AI__HTTP context_id:%ld,app_instance:%ld,request_id:%ld",
                            http_context_ptr->context_id,http_context_ptr->app_instance,http_context_ptr->request_id);
            
            if(signal.rsp_header_info.response_code == 200 || signal.rsp_header_info.response_code == 206)
            {
                if(signal.rsp_header_info.response_code == 200)
                {
                    ai_total_content_len = signal.rsp_header_info.content_length;//maybe is -1
                    ai_break_continue.already_rcv_len = 0;//solve bug
                }
                else
                {
                    AI_HTTP_LOG("AI__HTTP HandleAiHttpMsgExt receive remain equal=%d" , ai_break_continue.already_rcv_len+signal.rsp_header_info.content_length == ai_total_content_len);
                }
                
                //result=HTTP_HeaderResponse(http_context_ptr->context_id,http_context_ptr->app_instance,
                //            http_context_ptr->request_id,HTTP_DATA_TRANS_STYLE_BUFFER,PNULL,0);
                if(signal.rsp_header_info.refresh_url_ptr != NULL)
                {
                    //SCI_MEMSET(&s_refresh_ind,0,sizeof(AI_HTTP_REFRESH_IND_T));//yangyu delete
                    s_aireghttphttp.need_refresh = TRUE;
                    AI_HTTP_LOG("AI__HTTP  refresh time=%d, refresh_url_ptr:%s",signal.rsp_header_info.refresh_time,signal.rsp_header_info.refresh_url_ptr);
                    s_refresh_ind.refresh_time = signal.rsp_header_info.refresh_time;
                    //strcpy((char*)s_refresh_ind.url_arr,(const char*)(signal.rsp_header_info.refresh_url_ptr));//yangyu delete
                }
                else
                {
                    s_aireghttphttp.need_refresh = FALSE;
                }
            }
            else
            {
                s_aireghttphttp.flags = FALSE;
                HTTP_CloseRequest(http_context_ptr->context_id);
            }
#endif
            //return MMI_RESULT_TRUE;
            AI_HTTP_LOG("AI__HTTP result:%d",result);
            result = MMI_RESULT_TRUE;
        }
        break;
    case HTTP_SIG_ERROR_IND:
        {
            s_aireghttphttp.flags = FALSE;

            HTTP_CloseRequest(http_context_ptr->context_id);
        }
        break;	
    case HTTP_SIG_DATA_IND:
        {
            //HTTP_DATA_IND_SIG_T *cnf = (HTTP_DATA_IND_SIG_T*)original_param;
            AI_HTTP_LOG("AI__HTTP HTTP_SIG_DATA_IND");
             result=HandleAiHttpDataInd(http_context_ptr,param);
             //return result;
        }
        break;
    case HTTP_SIG_GET_CNF:
        result =HandleAiHttpGetCnf(http_context_ptr,param);
        //return result;
        break;
    case HTTP_SIG_POST_CNF:
        result =HandleAiHttpPostCnf(http_context_ptr,param);
        //return result;
        break;
     case HTTP_SIG_CLOSE_CNF://HTTP_SIG_CLOSE_REQ消息的回复,告知app,http服务关闭
        result = HandleAiHttpCloseCnf(http_context_ptr, param);	
        break;	 	
     default:
        break;
    }
    return result;
}
LOCAL void HandlePdpAiHttpTimer(uint8 timer_id,uint32 param)
{
    BOOLEAN sim_ok_num = FALSE;
    MN_DUAL_SYS_E sim_ok = MN_DUAL_SYS_1;
    BOOLEAN send_again = FALSE;	
    if (timer_id == g_aireghttp_global.timer_id)
    {
        sim_ok_num =MMIAPIPHONE_GetDataServiceSIM(&sim_ok);
        s_aireghttphttp.sim_sys = sim_ok;
        g_aireghttp_global.resend_type = AIREGHTTP_ALIVE_MSG;
        AI_HTTP_LOG("AI__HTTP HandleAiHttpPdpEvent sim_ok_num=%d,sim_ok=%d",sim_ok_num,sim_ok);					
        if(FALSE == sim_ok_num)
        {	
            send_again = TRUE;
            AI_HTTP_LOG("AI__HTTP  can not send alive message because of no sim available!");
        }
        else if(MMIAPIPDP_GetPdpLinkSIM() != MMI_DUAL_SYS_MAX)
        {
            send_again = TRUE;
            AI_HTTP_LOG("AI__HTTP can not send alive message because of pdp is linked"); 
        }
        else
        {
            g_aireghttp_global.assert_flag = FALSE;
            //if(MMIAI_HTTP_Check_NeedRcv(sim_ok))
            {
                if(MMIDEFAULT_GetBackLightStateEx() == MMI_BACKLIGHT_OFF)
                {
                    if(!AiHttpPDP())
                    {
                        send_again = TRUE;
                    }
                }
                else
                {
                    send_again = TRUE;
                }
            }
        }
    }
    MMK_StopTimer(timer_id);
    g_aireghttp_global.timer_id = 0;
    if(send_again)
    {
        send_again = FALSE;
        if (0 == g_aireghttp_global.retry_timer_id)
        {
            g_aireghttp_global.send_count = 0;
            g_aireghttp_global.retry_timer_id = MMK_CreateTimerCallback(AIREGHTTP_TIMEOUT_120S, MmiAiHttp_HandleRetryTimerMsg, NULL, FALSE);
            AI_HTTP_LOG("AI__HTTP 3 minite send again timer start, timer id = %d", g_aireghttp_global.retry_timer_id);
        }
    }
}




MMI_RESULT_E HandleAiHttpMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;
    if (HTTP_SIGNAL_BEG <= msg_id && msg_id <= HTTP_SIGNAL_END)
    {
    	result = HandleAiHttpMsgExt(app_ptr,msg_id,param);		
    }
    return result;
}

//Big_endian的，则返回0；若是Little_endian的，则返回1
LOCAL int checkCPU( )
{
    {
           union w
           {  
                  int a;
                  char b;
           } c;
           c.a = 1;
           return(c.b ==1);
    }
}

#define AUTO_SMS_TEST_STRING "{\"SIM1CDMAIMSI\":\"460036230790413\",\"OSVER\":\"android6.0.1\",\"SIM1CELLID\":\"\",\"ACCESSTYPE\":\"1\",\"DATASIM\":\"1\",\"UETYPE\":\"1\",\"SWVER\":\"Z609_V1.0.0\",\"RAM\":\"512M\",\"IMEI2\":\"855865068160367\",\"SIM2CDMAIMSI\":\"\",\"SIM1ICCID\":\"89860319957555166890\",\"SIM1LTEIMSI\":\"460110847549875\",\"SIM2ICCID\":\"\",\"IMEI1\":\"355865068160362\",\"ROM\":\"4G\",\"SIM2CELLID\":\"\",\"REGDATE\":\"2019-11-06 19:26:09\",\"MODEL\":\"BK-Coobe A560\",\"REGVER\":\"8.0\",\"MEID\":\"A10000FC59A2FB\",\"MACID\":\"02:00:00:00:00:00\"}"
LOCAL int AiHttp_WritePostBody(uint8 ** ptr)
{
    uint16 j_len = 0;
    uint16 b64_max_len = 0;
    int b64_len = 0;
    int err_result = 0;
    char * json_str = NULL;
    uint8 * base64_str = NULL;
    char timer_str[50] = {0};
    SCI_TIME_T          time;
    SCI_DATE_T      date;
    TM_GetSysDate(&date);
    TM_GetSysTime(&time);
    sprintf((char*)timer_str,"%04d-%02d-%02d %02d:%02d:%02d", date.year,date.mon,date.mday,time.hour,time.min,time.sec);

    json_str = SCI_ALLOC_APPZ(2048);
    if(json_str == NULL)
    {
        AI_HTTP_LOG("AI__HTTP [aireghttp] : AiHttp_WritePostBody Err No Mem 1");
        return 0;
    }
    json_str[j_len++] = '{';
    //sprintf(&json_str[j_len],"\"SIM1CDMAIMSI\":\"460036230790413\",\"OSVER\":\"android6.0.1\",\"SIM1CELLID\":\"\",\"ACCESSTYPE\":\"1\",\"DATASIM\":\"1\",\"UETYPE\":\"1\",\"SWVER\":\"Z609_V1.0.0\",\"RAM\":\"512M\",\"IMEI2\":\"%s\",",g_zdt_phone_imei_2);
    j_len = strlen(json_str);
    //sprintf(&json_str[j_len],"\"SIM2CDMAIMSI\":\"\",\"SIM1ICCID\":\"89860319957555166890\",\"SIM1LTEIMSI\":\"%s\",\"SIM2ICCID\":\"\",\"IMEI1\":\"%s\",",g_zdt_sim_imsi,g_zdt_phone_imei);
    j_len = strlen(json_str);
    json_str[j_len++] = '}';
    b64_max_len = BASE64LEN(j_len)+1;
    base64_str = SCI_ALLOC_APPZ(b64_max_len);
    if(base64_str == NULL)
    {
        AI_HTTP_LOG("AI__HTTP [aireghttp] : AiHttp_WritePostBody Err No Mem 2");
        return 0;
    }
    AI_HTTP_LOG("AI__HTTP [aireghttp] : AiHttp_WritePostBody SRC j_len=%d",j_len);
    b64_len = b64_max_len;
    err_result = base64_encode(base64_str,&b64_len,json_str,j_len);
    *ptr = base64_str;
    AI_HTTP_LOG("AI__HTTP [aireghttp] : AiHttp_WritePostBody DST b64_len=%d,max_len=%d,err_result=%d",b64_len,b64_max_len,err_result);
    SCI_FREE(json_str);
    return b64_len;
    
    
}



LOCAL int CTAIHttp_WritePostBody(uint8 * ptr)
{
	uint8_t in[17] = {"0"};    
	uint8_t out[64] = {'0'};	
	uint8_t aes_key_1[17];	  
	uint8_t iv[17]={0}; 
	uint8_t outStr[64] = {'0'};
	int i,aseLen=0;
	char *parameter_data = NULL;
	char aes_key[30] ="k6j48bR66PgQF2yS";
    char api_key[50] ="86804674ce864045b696d5a58bb32988";
	char   m_string[20]  = {"你好"}; 
	char *user_id  = "";
	char token[64]={0};

	
	cJSON	*pJsonRoot = NULL;
	char* str_js  = NULL;

	pJsonRoot = cJSON_CreateObject();
	if(NULL == pJsonRoot)
	{
		AI_HTTP_LOG("AI__HTTP  [zdtai]:makeJson NULL == pJsonRoot");
		return PNULL;
	}
	cJSON_AddStringToObject(pJsonRoot,"ak",api_key);
	cJSON_AddNumberToObject(pJsonRoot,"asr", 0);
	cJSON_AddNumberToObject(pJsonRoot,"tts", 3);
	cJSON_AddNumberToObject(pJsonRoot,"type", 5);
	cJSON_AddNumberToObject(pJsonRoot,"tone", 21);  
	cJSON_AddNumberToObject(pJsonRoot,"volume",3); 
	cJSON_AddNumberToObject(pJsonRoot,"flag", 3);
	cJSON_AddStringToObject(pJsonRoot,"textStr", m_string);
#if 0
	if(identify)
	{
	cJSON_AddStringToObject(root,"identify",identify);
	}
	if(realtime)
	{
	cJSON_AddNumberToObject(root,"realTime", 1);
	}
	if(index)
	{
	cJSON_AddNumberToObject(root,"index", index);
	}
#endif

    if(strlen(user_id) != 0)
    {
    memcpy(in, user_id, strlen(user_id));
    }
    memcpy(aes_key_1, aes_key, strlen(aes_key));
    memcpy(iv, api_key, 16);
    AES128_CBC_encrypt_buffer(out, in, 16, aes_key_1, iv);
    for(i=0;i < 16;i++)
    {
    	aseLen+=snprintf(outStr+aseLen,64,"%.2x",out[i]); 
    }
    cJSON_AddStringToObject(pJsonRoot,"uid", outStr);
    cJSON_AddStringToObject(pJsonRoot,"token", token);
    str_js = cJSON_Print(pJsonRoot);
    cJSON_Delete(pJsonRoot);
    AI_HTTP_LOG("AI__HTTP [zdtai]:parameters is :%s\n",str_js);

#if 0
	parameter_data = SCI_ALLOCA(strlen(str_js)+ strlen(upload_parameters) + strlen(boundary) + strlen(end)*2 + strlen(twoHyphens) +1);
	sprintf(parameter_data, upload_parameters, str_js);
	strcat(parameter_data, secondBoundary);

	strcat(tmp_ptr, firstBoundary);
	strcat(tmp_ptr, parameter_data);
	strcat(tmp_ptr, upload_speech);
	#else
	//parameter_data = SCI_ALLOCA(strlen(str_js));
	//memset(parameter_data,0,strlen(str_js));
	strcpy(ptr,str_js);	
	
#endif

}


LOCAL int AiHttp_WritePostBodyBase64(uint8 ** ptr)
{
    uint16 j_len = 0;
    uint16 b64_max_len = 0;
    int b64_len = 0;
    int err_result = 0;
    uint8 * json_str = NULL;
    uint8 * base64_str = NULL;
    j_len = strlen(AUTO_SMS_TEST_STRING);
    json_str = SCI_ALLOC_APPZ(j_len+1);
    if(json_str == NULL)
    {
        AI_HTTP_LOG("AI__HTTP [aireghttp] : AiHttp_WritePostBodyBase64 Err No Mem 1");
        return 0;
    }
    SCI_MEMSET(json_str,0x0,j_len+1);
    strcpy(json_str, AUTO_SMS_TEST_STRING);

    b64_max_len = BASE64LEN(j_len)+1;
    base64_str = SCI_ALLOC_APPZ(b64_max_len);
    if(base64_str == NULL)
    {
        AI_HTTP_LOG("AI__HTTP [aireghttp] : AiHttp_WritePostBodyBase64 Err No Mem 2");
        return 0;
    }
    AI_HTTP_LOG("AI__HTTP [aireghttp] : AiHttp_WritePostBodyBase64 SRC j_len=%d",j_len);
    b64_len = b64_max_len;
    err_result = base64_encode(base64_str,&b64_len,json_str,j_len);
    *ptr = base64_str;
    AI_HTTP_LOG("AI__HTTP [aireghttp] : AiHttp_WritePostBodyBase64 DST b64_len=%d,max_len=%d,err_result=%d",b64_len,b64_max_len,err_result);
    SCI_FREE(json_str);
    return b64_len;
}

PUBLIC void HandleAiHttpPdpEvent(void)
{
    BOOLEAN aireghttp_enable = TRUE;

    if (aireghttp_enable)
    {
        if (0 == g_aireghttp_global.timer_id)
        {
            AI_HTTP_LOG("AI__HTTP HandleAiHttpPdpEvent dual_sys=%d  ",s_aireghttphttp.sim_sys);
            g_aireghttp_global.timer_id = MMK_CreateTimerCallback(AIREGHTTP_TIMEOUT_20S, HandlePdpAiHttpTimer, NULL, FALSE);
            AI_HTTP_LOG("AI__HTTP  2 Minutes timer start, timer id = %d", g_aireghttp_global.timer_id);
        }
        if (0 == g_aireghttp_global.timer_12h_id)
        {
            g_aireghttp_global.timer_12h_id = MMK_CreateTimerCallback(AIREGHTTP_TIMEOUT_12HOUR, MmiAiHttp_Handle12HourTimerMsg, NULL, TRUE);
            AI_HTTP_LOG("AI__HTTP  12 Hours timer start, timer id = %d", g_aireghttp_global.timer_12h_id);
        }
    }
}






LOCAL MMI_RESULT_E HandleAiHttpDataInd(AIREGHTTP_HTTP_CONTEXT * http_context_ptr, DPARAM param)
{
    //---1--- 
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    HTTP_DATA_IND_SIG_T signal = {0};
    //uint32 response_code = 0;
   // uint32 data_len = 0;
    //uint8 *data_ptr = PNULL;
    char* tmp_data_buff=NULL;
    uint8 buf_list_size = 0;
    BOOLEAN insert_ret = FALSE;
    AI_HTTP_LOG("AI__HTTP HandleAiHttpDataInd start");
    AIREGHTTP_HTTP_SIGNAL_ANALYTIC(param,signal,http_context_ptr,HTTP_DATA_IND_SIG_T);
    ai_cur_rcv_count += signal.data_len;
    //yangyu add later
    if (http_context_ptr->context_id != signal.context_id || http_context_ptr->request_id != signal.request_id)
    {
        return MMI_RESULT_FALSE;
    }
    AI_HTTP_LOG("AI__HTTP HandleHttpMsg receive HTTP_SIG_DATA_IND");
    AI_HTTP_LOG("AI__HTTP HandleHttpMsg receive , len=%d,ai_cur_rcv_count=%d",signal.data_len,ai_cur_rcv_count);
    //AI_HTTP_LOG("AI__HTTP HandleHttpMsg receive signal.data=%s, len=%d,ai_cur_rcv_count=%d",signal.data_ptr,signal.data_len,ai_cur_rcv_count);
    //load_mp3_Save(signal.data_ptr,signal.data_len);
    //AI_Save_Mp3(signal.data_ptr,signal.data_len);

    last_data_ind_time_sec = MMIAPICOM_GetCurTime();
    if(signal.data_len > 0)
    {
        uint32 old_len = ai_break_continue.already_rcv_len;
        BOOLEAN download_finish = FALSE;
        uint32 inserted_len = 0;
        ai_break_continue.already_rcv_len += signal.data_len;
        download_finish = (ai_break_continue.already_rcv_len == ai_total_content_len);
        if(download_finish)
        {
            AI_HTTP_LOG("AI__HTTP HandleAiHttpDataInd download_finish,  rcv_len= %ld", ai_break_continue.already_rcv_len);
        }
        buf_list_size = AI_Mp3BufList_InsertData(signal.data_ptr, signal.data_len, &insert_ret, &inserted_len, download_finish);
        if(!insert_ret)
        {
            AI_HTTP_LOG("AI__HTTP HandleAiHttpDataInd insert fail , inserted_len = %ld", inserted_len);
            ai_break_continue.already_rcv_len = old_len + inserted_len;
            AI_HttpForceClose();
        }
    }
    if(ai_break_continue.already_rcv_len == ai_total_content_len || ai_total_content_len == -1)
    {
        AI_LOG("AI__HTTP AI_HandleHttpDataInd download finished ,clear len");
        ai_break_continue.already_rcv_len = ai_total_content_len = 0;
    }
    
    /*
    AI_HTTP_LOG("AI__HTTP HandleHttpMsg receive HTTP_SIG_DATA_IND buf_list_size=%d, insert_ret=%d, break_cont=%d",
    buf_list_size ,insert_ret , ai_break_continue.break_count);
    if(buf_list_size == 1 && insert_ret)
    {
        //AI_API_Rcv_Mp3_PlayStart();
    }
    ai_break_continue.already_rcv_len += signal.data_len;//ai_cur_rcv_count;
    AI_HTTP_LOG("AI__HTTP HandleHttpMsg receive ai_total_content_len=%ld, already_rcv_len=%ld ",
            ai_total_content_len , ai_break_continue.already_rcv_len);
    if((ai_break_continue.already_rcv_len  >= AI_MAX_RAM_BUF_SIZE*(ai_break_continue.break_count + 1)
        && ai_break_continue.already_rcv_len < ai_total_content_len)
    )
    {
           ai_break_continue.break_count++;
           ai_break_continue.need_next_continue = TRUE;
           AI_HTTP_LOG("AI__HTTP HandleHttpMsg receive break_count=%d", ai_break_continue.break_count);
           AI_HttpForceClose();
          // AI_HttpCancelDownload();
           
    }
    else if(ai_break_continue.already_rcv_len == ai_total_content_len)
    {
        AI_HTTP_LOG("AI__HTTP HandleHttpMsg receive finish");
        ClearBreakContinue();
        AI_Http_StopCheckDownloadFinishTimer();
    }
    else if(ai_break_continue.already_rcv_len > ai_total_content_len)
    {
        AI_HTTP_LOG("AI__HTTP HandleHttpMsg receive error ,overflow");
        ClearBreakContinue();
        AI_HttpForceClose();
        //AI_HttpCancelDownload();
        AI_Http_StopCheckDownloadFinishTimer();

    }
    if(ai_break_continue.already_rcv_len < ai_total_content_len)
    {
        //AI_Http_StartCheckDownloadFinishTimer();

    }*/
    //AI_Mp3FixBuf_InsertData(signal.data_ptr, signal.data_len);
    //AI_Mp3InsertData(signal.data_ptr, signal.data_len);// need wait ,also need double ram memory
    http_context_ptr->request_id = signal.request_id;
    http_context_ptr->app_instance = signal.app_instance;
    return result;
}
PUBLIC BOOLEAN AI_HTTP_IsMp3DownloadStopedLongTime()
{
    BOOLEAN ret = FALSE;
    if(ai_total_content_len > MP3_HTTP_BUFF_SIZE)
    {
        uint32 cur_time = MMIAPICOM_GetCurTime(); 
        AI_LOG("AI__HTTP_IsMp3DownloadStopedLongTime ,cur_time=%ld, last_data_time=%ld" ,cur_time,last_data_ind_time_sec);
        if(cur_time - last_data_ind_time_sec >= 2)
        {
            ret = TRUE;
        }
        
    }
    AI_LOG("AI__HTTP_IsMp3DownloadStopedLongTime ,ret=%ld " ,ret);
    return ret;
}

PUBLIC BOOLEAN MMIAI_IsDownloadFinish()
{
    return ai_break_continue.already_rcv_len >= ai_total_content_len;
}

PUBLIC void MMIAI_SetContinueAgain()
{
    ai_break_continue.need_next_continue = TRUE;
}

PUBLIC BOOLEAN MMIAI_DataRcvIsBreak()
{
    AI_HTTP_LOG("AI__HTTP  MMIAI_DataRcvIsBreak need_next_continue=%d", ai_break_continue.need_next_continue);

    return ai_break_continue.need_next_continue;
}

static BOOLEAN MMIAI_HTTP_CTREG_PdpStateChange(BOOLEAN is_ok,BOOLEAN is_active, BOOLEAN is_handled)
{
    AI_HTTP_LOG("AI__HTTP MMI_ZDT_TCP_PdpStateChange is_ok=%d, is_active=%d,is_handled=%d",is_ok,is_active,is_handled);
    if(is_ok)
    {
    }
    else
    {
    }

    return TRUE;
}

static BOOLEAN MMIAI_HTTP_CTREG_PdpStateUpdate(BOOLEAN is_ok,BOOLEAN is_active)
{
    BOOLEAN res = FALSE;
    AI_HTTP_LOG("AI__HTTP MMIAI_HTTP_CTREG_PdpStateUpdate %d, is_active=%d",is_active);
    if(m_ai_http_reg_pdpstate != is_ok)
    {
        m_ai_http_reg_pdpstate = is_ok;
        MMIAI_HTTP_CTREG_PdpStateChange(is_ok,is_active,res);
    }
    return TRUE;
}
PUBLIC BOOLEAN  MMIAI_HTTP_CTREG_Handle_PDPActiveOK(void)
{
    AI_HTTP_LOG("AI__HTTP MMIAI_HTTP_CTREG_Handle_PDPActiveOK");
    MMIAI_HTTP_CTREG_PdpStateUpdate(TRUE,TRUE);
    return TRUE;
}

PUBLIC BOOLEAN  MMIAI_HTTP_CTREG_Handle_PDPActiveFail(void)
{
    AI_HTTP_LOG("AI__HTTP MMIAI_HTTP_CTREG_Handle_PDPActiveFail");
    MMIAI_HTTP_CTREG_PdpStateUpdate(FALSE,TRUE);
    return TRUE;
}

PUBLIC BOOLEAN  MMIAI_HTTP_CTREG_Handle_PDPRespond(void)
{
    AI_HTTP_LOG("AI__HTTP ZDT NET MMIAI_HTTP_CTREG_Handle_PDPRespond");
    MMIAI_HTTP_CTREG_PdpStateUpdate(FALSE,FALSE);
    return TRUE;
}

/*
BOOLEAN  MMIAI_HTTP_NV_SetRcvSwitch(uint8 on_off) 
{
    uint8 cur_on_off = on_off;
    MMINV_WRITE(MMI_ZDT_NV_HTTP_RCV_SWITCH,&cur_on_off);
    return TRUE;
}

uint8 MMIAI_HTTP_NV_GetRcvSwitch(void)
{
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    uint8 cur_on_off = 1;
    //get if is flip answer
    MMINV_READ(MMI_ZDT_NV_HTTP_RCV_SWITCH,&cur_on_off,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        cur_on_off = 1;
        MMINV_WRITE(MMI_ZDT_NV_HTTP_RCV_SWITCH,&cur_on_off);
    }
    return cur_on_off;
}

uint32 MMIAI_HTTP_NV_GetRcvCount(void)
{
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    uint32 cur_count = 0;
    //get if is flip answer
    MMINV_READ(MMI_ZDT_NV_HTTP_RCV_COUNT,&cur_count,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        cur_count = 0;
        MMINV_WRITE(MMI_ZDT_NV_HTTP_RCV_COUNT,&cur_count);
    }
    return cur_count;
}

BOOLEAN  MMIAI_HTTP_NV_SetRcvCount(uint32 count) 
{
    uint32 cur_count = count;
    MMINV_WRITE(MMI_ZDT_NV_HTTP_RCV_COUNT,&cur_count);
    return TRUE;
}

BOOLEAN  MMIAI_HTTP_NV_SetRcvMonth(uint8 month) 
{
    uint8 cur_month = month;
    MMINV_WRITE(MMI_ZDT_NV_HTTP_RCV_MONTH,&cur_month);
    return TRUE;
}

uint8 MMIAI_HTTP_NV_GetRcvMonth(void)
{
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    uint8 cur_month = 0;
    //get if is flip answer
    MMINV_READ(MMI_ZDT_NV_HTTP_RCV_MONTH,&cur_month,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        cur_month = 1;
        MMINV_WRITE(MMI_ZDT_NV_HTTP_RCV_MONTH,&cur_month);
    }
    return cur_month;
}

uint32 MMIAI_HTTP_NV_GetRcvDateEnd(void)
{
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    uint32 cur_count = 0;
    MMINV_READ(MMI_ZDT_NV_HTTP_RCV_DATEEND,&cur_count,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        cur_count = 0;
        MMINV_WRITE(MMI_ZDT_NV_HTTP_RCV_DATEEND,&cur_count);
    }
    return cur_count;
}

BOOLEAN  MMIAI_HTTP_NV_SetRcvDateEnd(uint32 count) 
{
    uint32 cur_count = count;
    MMINV_WRITE(MMI_ZDT_NV_HTTP_RCV_DATEEND,&cur_count);
    return TRUE;
}

uint32 MMIAI_HTTP_NV_GetRcvDateStart(void)
{
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    uint32 cur_count = 0;
    MMINV_READ(MMI_ZDT_NV_HTTP_RCV_DATESTART,&cur_count,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        cur_count = 0;
        MMINV_WRITE(MMI_ZDT_NV_HTTP_RCV_DATESTART,&cur_count);
    }
    return cur_count;
}

BOOLEAN  MMIAI_HTTP_NV_SetRcvDateStart(uint32 count) 
{
    uint32 cur_count = count;
    MMINV_WRITE(MMI_ZDT_NV_HTTP_RCV_DATESTART,&cur_count);
    return TRUE;
}
*/

uint32  MMIAI_HTTP_Get_TimeSecond(void)
{
    SCI_TM_T sys_time = {0};
    uint32   second = 0;

    TM_GetTime(&sys_time);
    second = MMIAPICOM_Tm2Second(sys_time.tm_sec, sys_time.tm_min, sys_time.tm_hour, sys_time.tm_mday, sys_time.tm_mon, sys_time.tm_year);

    return second;
}

BOOLEAN MMIAI_HTTP_IsCMCC(MN_DUAL_SYS_E sim)
{
    PHONE_SERVICE_STATUS_T      service_status= {0};

    MMIAPIPHONE_GetCurServiceStatus(sim, &service_status);
    AI_HTTP_LOG("AI__HTTP [aireghttp] sim [%d] mcc =%d, mnc = %d", sim, service_status.plmn.mcc, service_status.plmn.mnc);

    if( (460 == service_status.plmn.mcc) &&
        ((0 == service_status.plmn.mnc ) || (2 == service_status.plmn.mnc ) || (4 == service_status.plmn.mnc ) || (7 == service_status.plmn.mnc ) || (8 == service_status.plmn.mnc )))
    {
        return TRUE;
    }
    #if 0
    if( (454 == service_status.plmn.mcc) &&
        ((12 == service_status.plmn.mnc ) || (13 == service_status.plmn.mnc ) ))
    {
        return TRUE;
    }
    #endif
    return FALSE;
}
/*
BOOLEAN MMIAI_HTTP_RcvSet(uint32 count)
{
    SCI_DATE_T      date;
    uint32 cur_rcv_count = 0;
    uint8 cur_rcv_month = 0;
    TM_GetSysDate(&date);
    cur_rcv_month = MMIAI_HTTP_NV_GetRcvMonth();
    cur_rcv_count = MMIAI_HTTP_NV_GetRcvCount();
    if(MMIAI_HTTP_NV_GetRcvDateStart() == 0)
    {
        uint32 cur_sec = MMIAI_HTTP_Get_TimeSecond();
        MMIAI_HTTP_NV_SetRcvDateStart(cur_sec);
        MMIAI_HTTP_NV_SetRcvDateEnd(cur_sec+31536000); //12个月
    }
    if(cur_rcv_month == date.mon)
    {
        cur_rcv_count += count;
        MMIAI_HTTP_NV_SetRcvCount(cur_rcv_count);
    }
    else
    {
        cur_rcv_count = count;
        MMIAI_HTTP_NV_SetRcvCount(cur_rcv_count);
        MMIAI_HTTP_NV_SetRcvMonth(date.mon);
    }
    return FALSE;
}

PUBLIC uint8 MMIAI_HTTP_Check_NeedRcv(MN_DUAL_SYS_E sim)
{
    uint8 res = 1;
    SCI_DATE_T      date;
    uint32 cur_rcv_count = 0;
    uint8 cur_rcv_month = 0;
    uint32 cur_sec = MMIAI_HTTP_Get_TimeSecond();
    uint32 cur_end_sec = MMIAI_HTTP_NV_GetRcvDateEnd();
    if(FALSE == MMIAI_HTTP_IsCMCC(sim))
    {
        AI_HTTP_LOG("[aireghttp] sim=%d,NOT CMCC",sim);
        return 0;
    }
    if(cur_end_sec != 0 && cur_sec >= cur_end_sec)
    {
        AI_HTTP_LOG("[aireghttp] sim=%d,NOT In DateTime",sim);
        return 0;
    }
    TM_GetSysDate(&date);
    cur_rcv_month = MMIAI_HTTP_NV_GetRcvMonth();
    cur_rcv_count = MMIAI_HTTP_NV_GetRcvCount();
    res = MMIAI_HTTP_NV_GetRcvSwitch();
    if(cur_rcv_month == date.mon)
    {
        if(cur_rcv_count >= AIREGHTTP_ALL_COUNT)
        {
            AI_HTTP_LOG("[aireghttp] cur_rcv_month=%d, Count FULL",cur_rcv_month);
            return 0;
        }
    }
    return res;
}

PUBLIC void MMIAI_HTTP_SetEnable(void)
{
    MMIAI_HTTP_NV_SetRcvSwitch(1);
    MMIAI_HTTP_NV_SetRcvDateStart(0);
    MMIAI_HTTP_NV_SetRcvDateEnd(0);
    AI_HTTP_LOG("[aireghttp] MMIAI_HTTP_SetEnable");
}

PUBLIC void MMIAI_HTTP_SetDisable(void)
{
    MMIAI_HTTP_NV_SetRcvSwitch(0);
    AI_HTTP_LOG("[aireghttp] MMIAI_HTTP_SetDisEnable");
}

PUBLIC BOOLEAN MMIAI_HTTP_GetRecieveStatus(void)
{
    return g_ai_http_rcv_status;
}
*/

PUBLIC void MmiStartREHttp(void)
{
    BOOLEAN sim_ok_num = FALSE;
    MN_DUAL_SYS_E sim_ok = MN_DUAL_SYS_1;
    BOOLEAN send_again = TRUE;	
    AI_HTTP_LOG("AI__HTTP [zdtai]: MmiStartREHttp!");

  
        sim_ok_num =MMIAPIPHONE_GetDataServiceSIM(&sim_ok);

        if(FALSE == sim_ok_num)
        {		
            AI_HTTP_LOG("AI__HTTP [zdtai]: can not send alive message because of no sim available!");
        }
        else if(MMIAPIPDP_GetPdpLinkSIM() != MMI_DUAL_SYS_MAX)
        {
            AI_HTTP_LOG("AI__HTTP [zdtai]:can not send alive message because of pdp is linked"); 
        }
        else
        { 
              ///  if(MMIDEFAULT_GetBackLightStateEx() == MMI_BACKLIGHT_OFF)
                {
                    if(!AiHttpPDP())
                    {
                        AI_HTTP_LOG("AI__HTTP [zdtai]: http error "); 
                    }
                }
               // else
               // {
               //      AI_HTTP_LOG("[zdtai]: http  OK"); 
              //  }
            
        }
 
}

#endif
#endif
