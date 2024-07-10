
#ifndef  _MMI_WLDT_NET_C  
#define _MMI_WLDT_NET_C  

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "wldt_app.h"
#include "cJSON.h"
#include "zdt_app.h"

#ifdef WIFI_SUPPORT
#include "wifisupp_api.h"
#endif

#define WLDT_NET_TRACE            SCI_TRACE_LOW

#define WLDT_GPRS_CHECK_TIMEOUT       5000  

#define WLDT_PDP_ACTIVE_TIMEOUT       30000  
#define WLDT_PDP_RETRY_TIMEOUT        2000 //重新尝试连接pdp，换卡连接

#define WLDT_PDP_DEFAULT_SIM_SYS MN_DUAL_SYS_1

#define WLDT_NET_FIX_IMEI

#define y_abs(x)  ((x) >= 0 ? (x) : (-(x)))


char         g_wldt_phone_imei_1[WLDT_IMEI_LEN+1] = {0};
char         g_wldt_phone_imei_2[WLDT_IMEI_LEN+1] = {0};
char*       g_wldt_phone_imei = g_wldt_phone_imei_1;
char         g_wldt_sim_imsi[WLDT_IMSI_LEN+1] = {0};
char         g_wldt_sim_iccid[WLDT_ICCID_LEN+1] = {0};
uint32         g_wldt_net_cell_id = 0;

LOCAL MN_DUAL_SYS_E       s_wldt_net_set_sys   = WLDT_PDP_DEFAULT_SIM_SYS;
LOCAL MN_DUAL_SYS_E       s_wldt_net_cur_sys   = MN_DUAL_SYS_1;
LOCAL uint8                       s_wldt_net_pdp_timer_id      = 0;
LOCAL uint8                       s_wldt_net_pdp_retry_timer_id = 0;
LOCAL uint8                       s_wldt_net_gprs_check_timer_id      = 0;
LOCAL uint8                       s_wldt_net_reset_timer_id      = 0;
LOCAL uint32                      s_wldt_net_id            = 0;
LOCAL BOOLEAN                  s_wldt_net_pdp_state_is_ok        = FALSE;
LOCAL BOOLEAN                  s_wldt_net_is_init        = FALSE;
LOCAL BOOLEAN                  s_wldt_net_need_init        = TRUE;
LOCAL BOOLEAN                  s_wldt_net_gprs_status[MN_DUAL_SYS_MAX+1] = {0};
LOCAL BOOLEAN                  s_wldt_net_cur_gprs_status = FALSE;
LOCAL BOOLEAN                  s_wldt_net_is_activing        = FALSE;
LOCAL BOOLEAN                  s_wldt_net_is_closing        = FALSE;

LOCAL BOOLEAN                  s_wldt_net_need_active_close        = FALSE;
LOCAL BOOLEAN                  s_wldt_net_need_close_active        = FALSE;

MMIWLDT_S_STATION_DATA_T g_wldt_s_station_data = {0};
MMIWLDT_STATION_DATA_T g_wldt_station_data = {0};

static uint32 last_reset_msg_time = 0;
static SCI_MUTEX_PTR g_wldt_net_mutexPtr = NULL;
static uint32 wldt_net_reset_interval_time = 10000;  //毫秒
static uint32 wldt_net_reset_interval_num = 0;
static uint32 wldt_net_reset_last_land_time_ms = 0;

LOCAL BOOLEAN  WLDT_Net_PDPTry(MN_DUAL_SYS_E sim_sys);
LOCAL BOOLEAN WLDT_Net_PdpDeactive(void);
LOCAL BOOLEAN WLDT_Net_PdpActive(void);
static void MMIWLDT_Net_ResetExt(void);

uint16  MMIWLDT_Net_GetIMEI(MN_DUAL_SYS_E dual_sys,char * imei_ptr)
{
	MN_IMEI_T imei;
	uint8 tmp_buf1[20] = {0};
	uint8 i=0, j=0;

	MNNV_GetIMEIEx(dual_sys, imei);		

	for(i = 0, j = 0; i < 8; i++)
	{
		tmp_buf1[j++] = (imei[i] & 0x0F) + '0';
		tmp_buf1[j++] = ((imei[i] >> 4) & 0x0F) + '0';
	}

	for( i=0; i<WLDT_IMEI_LEN; i++ )
	{
		imei_ptr[i] = tmp_buf1[i+1];
	}
    
	imei_ptr[WLDT_IMEI_LEN] = 0x00;

	return WLDT_IMEI_LEN;
}

uint16  MMIWLDT_Get_SIM_IMSI(MN_DUAL_SYS_E dual_sys,char * imsi_ptr)
{
	uint8 i=0, j=0;
	MN_IMSI_T imsi = {0};
       char imsi_buf[WLDT_IMSI_LEN+1] = {0};
       
	imsi = MNSIM_GetImsiEx(dual_sys);

	if(imsi.imsi_val == NULL )
		return 0;

	for(i = 0, j = 0; i < 8; i++)
	{
		imsi_buf[j++] = (imsi.imsi_val[i] & 0x0F) + '0';
		imsi_buf[j++] = ((imsi.imsi_val[i] >> 4) & 0x0F) + '0';
	}
    
	for( i=0; i<WLDT_IMSI_LEN; i++ )
	{
		imsi_ptr[i] = imsi_buf[i+1];
	}
	imsi_ptr[WLDT_IMSI_LEN] = 0x00;
	
	return WLDT_IMSI_LEN;
}

uint16  MMIWLDT_Get_SIM_ICCID(MN_DUAL_SYS_E dual_sys,char * iccid_ptr)
{
    uint16 len = 0;
    ERR_MNDATAMAG_CODE_E err_code = 0;
    char iccid_buf[WLDT_ICCID_LEN+1] = {0};
    MNSIM_ICCID_T iccid_t = {0};

    err_code = MNSIM_GetICCIDEx(dual_sys,&iccid_t);
    WLDT_NET_TRACE("MMIWLDT_Get_SIM_ICCID dual_sys =%d,err_code = %d,id_num=0x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",dual_sys,err_code,iccid_t.id_num[9],iccid_t.id_num[8],iccid_t.id_num[7],iccid_t.id_num[6],iccid_t.id_num[5],iccid_t.id_num[4],iccid_t.id_num[3],iccid_t.id_num[2],iccid_t.id_num[1],iccid_t.id_num[0]);
    //MMIAPICOM_BcdToStr(PACKED_LSB_FIRST, iccid_t.id_num, WLDT_ICCID_LEN, iccid_buf);
	MMIAPICOM_BcdICCIDToStr(iccid_t.id_num, WLDT_ICCID_LEN, iccid_buf);
    len = strlen((char *)iccid_buf);
    WLDT_NET_TRACE("MMIWLDT_Get_SIM_ICCID STR = %s",iccid_buf);
    SCI_MEMCPY(iccid_ptr,iccid_buf,len);
    
    return len;
}
uint16  MMIWLDT_Get_IMSI_Str(MN_IMSI_T imsi,char * imsi_ptr)
{
	uint8 i=0, j=0;
       char imsi_buf[WLDT_IMSI_LEN+1] = {0};
       
	for(i = 0, j = 0; i < 8; i++)
	{
		imsi_buf[j++] = (imsi.imsi_val[i] & 0x0F) + '0';
		imsi_buf[j++] = ((imsi.imsi_val[i] >> 4) & 0x0F) + '0';
	}
    
	for( i=0; i<WLDT_IMSI_LEN; i++ )
	{
		imsi_ptr[i] = imsi_buf[i+1];
	}
	imsi_ptr[WLDT_IMSI_LEN] = 0x00;
	
	return WLDT_IMSI_LEN;
}
/*****************************************************************************/
//  Discription: Get apn str 
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
/*****************************************************************************/
LOCAL char* WLDT_Net_GetApnStr(MN_DUAL_SYS_E dual_sys)
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
        WLDT_NET_TRACE("WLDT NET Get APN %s",apn_str);
    }
    else
    {
        WLDT_NET_TRACE("WLDT NET Get APN NULL");
    }
    return apn_str;
}

/*****************************************************************************/
//  Discription: Get apn str 
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
/*****************************************************************************/
LOCAL char* WLDT_Net_GetUserNameStr(MN_DUAL_SYS_E dual_sys)
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
        WLDT_NET_TRACE("WLDT NET Get UserName %s",user_name_str);
    }
    else
    {
        WLDT_NET_TRACE("WLDT NET Get UserName NULL");
    }
    return user_name_str;
    
}

/*****************************************************************************/
//  Discription: Get apn str 
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
/*****************************************************************************/
LOCAL char* WLDT_Net_GetPasswordStr(MN_DUAL_SYS_E dual_sys)
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
        WLDT_NET_TRACE("WLDT NET Get Password %s",password_str);
    }
    else
    {
        WLDT_NET_TRACE("WLDT NET Get Password NULL");
    }
    return password_str;
}

/*****************************************************************************/
//  Description : handle socket connect timeout
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL void WLDT_Net_HandlePdpActiveTimer(
                                            uint8 timer_id,
                                            uint32 param
                                            )
{
    WLDT_NET_TRACE("WLDT NET PdpActive ERR TimeOut");
    WLDT_Net_PdpDeactive();
    MMIWLDT_SendSigTo_APP(WLDT_APP_SIGNAL_NET_ACTIVE_FAIL,PNULL,0);
}

/*****************************************************************************/
//  Discription: Start socket connect timer
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
/*****************************************************************************/
LOCAL void WLDT_Net_StartPdpActiveTimer(void)
{
    if(0 != s_wldt_net_pdp_timer_id)
    {
        MMK_StopTimer(s_wldt_net_pdp_timer_id);
        s_wldt_net_pdp_timer_id = 0;
    }
    
    s_wldt_net_pdp_timer_id = MMK_CreateTimerCallback(WLDT_PDP_ACTIVE_TIMEOUT, 
                                                                        WLDT_Net_HandlePdpActiveTimer, 
                                                                        PNULL, 
                                                                        FALSE);
}
/*****************************************************************************/
//  Discription: Start socket connect timer
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
/*****************************************************************************/
LOCAL void WLDT_Net_StopPdpActiveTimer(void)
{
    if(0 != s_wldt_net_pdp_timer_id)
    {
        MMK_StopTimer(s_wldt_net_pdp_timer_id);
        s_wldt_net_pdp_timer_id = 0;
    }
}

LOCAL void WLDT_Net_HandlePdpRetryTimer(
                                uint8 timer_id,
                                uint32 param
                                )
{
    MN_DUAL_SYS_E sys_sim = 0;
    BOOLEAN       sim_ok =  FALSE;

    if(timer_id == s_wldt_net_pdp_retry_timer_id && 0 != s_wldt_net_pdp_retry_timer_id)
    {
        MMK_StopTimer(s_wldt_net_pdp_retry_timer_id);
        s_wldt_net_pdp_retry_timer_id = 0;
    }
    s_wldt_net_cur_sys++;
    if(s_wldt_net_cur_sys < MMI_DUAL_SYS_MAX)
    {
        for(sys_sim = s_wldt_net_cur_sys; sys_sim < MMI_DUAL_SYS_MAX; sys_sim++)
        {
            if(MMIPHONE_IsSimOk(sys_sim))
            {
                s_wldt_net_cur_sys = sys_sim;
                sim_ok = TRUE;
                break;
            }
        }
        if(sim_ok)
        {
            if(!WLDT_Net_PDPTry(s_wldt_net_cur_sys))
            {
                WLDT_NET_TRACE("WLDT NET PdpActive Retry ERR cur_sys(%d),",s_wldt_net_cur_sys);
        	    s_wldt_net_pdp_retry_timer_id = MMK_CreateTimerCallback(
                                                                                                WLDT_PDP_RETRY_TIMEOUT, 
                                                                                                WLDT_Net_HandlePdpRetryTimer, 
                                                                                                PNULL, 
                                                                                                FALSE
                                                                                                );
            }
        }
        else
        {
            WLDT_NET_TRACE("WLDT NET PdpActive Retry ERR NO SIM");
            MMIWLDT_SendSigTo_APP(WLDT_APP_SIGNAL_NET_ACTIVE_FAIL,PNULL,0);
        }
    }
    else//找不到卡了
    {
        WLDT_NET_TRACE("WLDT NET PdpActive Retry ERR no_sys");
        MMIWLDT_SendSigTo_APP(WLDT_APP_SIGNAL_NET_ACTIVE_FAIL,PNULL,0);
    }
}

/*****************************************************************************
//  Description : handle pdp msg 
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
*****************************************************************************/
LOCAL void WLDT_Net_HandlePdpMsg(MMIPDP_CNF_INFO_T *msg_ptr)
{
    if(PNULL == msg_ptr)
    {
        return;
    }
    
    WLDT_NET_TRACE("WLDT NET HandlePdpMsg msg_id=%d,result=%d",msg_ptr->msg_id,msg_ptr->result);
    
    switch(msg_ptr->msg_id) 
    {
    case MMIPDP_ACTIVE_CNF:     //PDP激活成功
        WLDT_Net_StopPdpActiveTimer();
        
        if(MMIPDP_RESULT_SUCC == msg_ptr->result)
        {
            s_wldt_net_id = msg_ptr->nsapi;
            WLDT_NET_TRACE("WLDT NET MMIPDP_ACTIVE_CNF OK net_id=0x%x",s_wldt_net_id);
            MMIWLDT_SendSigTo_APP(WLDT_APP_SIGNAL_NET_ACTIVE_SUCCESS,PNULL,0);
        }
        else if (MMIPDP_RESULT_FAIL == msg_ptr->result)
        {
            WLDT_NET_TRACE("WLDT NET MMIPDP_ACTIVE_CNF FAIL");
            WLDT_Net_PdpDeactive();
            MMIWLDT_SendSigTo_APP(WLDT_APP_SIGNAL_NET_ACTIVE_FAIL,PNULL,0);
        }
        else 
        {
            WLDT_NET_TRACE("WLDT NET MMIPDP_ACTIVE_CNF FAIL Other");
            WLDT_Net_PdpDeactive();
            MMIWLDT_SendSigTo_APP(WLDT_APP_SIGNAL_NET_ACTIVE_FAIL,PNULL,0);
        }        
        break;
        
    case MMIPDP_DEACTIVE_CNF:   //PDP去激活成功。
            WLDT_NET_TRACE("WLDT NET MMIPDP_DEACTIVE_CNF");
            s_wldt_net_id = 0;
            MMIWLDT_SendSigTo_APP(WLDT_APP_SIGNAL_NET_CONNET_FAIL,PNULL,0);
        break;
        
    case MMIPDP_DEACTIVE_IND:   //PDP被网络端去激活。
            WLDT_NET_TRACE("WLDT NET MMIPDP_DEACTIVE_IND");
            s_wldt_net_id = 0;
            WLDT_Net_PdpDeactive();
            MMIWLDT_SendSigTo_APP(WLDT_APP_SIGNAL_NET_CONNET_FAIL,PNULL,0);
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
LOCAL BOOLEAN  WLDT_Net_PDPTry(MN_DUAL_SYS_E sim_sys)
{
#if defined(MMI_GPRS_SUPPORT)
    BOOLEAN                 return_val      = FALSE;
    MMIPDP_ACTIVE_INFO_T    active_info     = {0};
    MMICONNECTION_APNTYPEINDEX_T *apn_idx_arr;
    MMICONNECTION_LINKSETTING_DETAIL_T* linksetting = PNULL;

    WLDT_NET_TRACE("WLDT NET ConnectPDP");
    if(MMIAPISET_GetFlyMode())
    {
        WLDT_NET_TRACE("WLDT NET ConnectPDP [Fly Mode]");
        return FALSE;
    }
    if (FALSE == MMIWLDT_Net_GPRS_IsOn())
    {
        WLDT_NET_TRACE("WLDT NET ConnectPDP [DATA ERROR]");
        return FALSE;
    }
    if (FALSE == MMIAPIPHONE_GetDataServiceSIM(&sim_sys))
    {
        WLDT_NET_TRACE("WLDT NET ConnectPDP [SIM ERROR]");
        return FALSE;
    }
    if(sim_sys != s_wldt_net_set_sys)
    {
        MMIWLDT_Net_SetActiveSys(sim_sys);
    }
    active_info.app_handler         = MMI_MODULE_WLDT;
    active_info.dual_sys            = sim_sys;
    active_info.apn_ptr             = NULL;
    active_info.user_name_ptr       = NULL;
    active_info.psw_ptr             = NULL;
    active_info.priority            = 3;
    active_info.ps_service_rat      = MN_TD_PREFER;
    active_info.ps_interface        = MMIPDP_INTERFACE_GPRS;
    active_info.handle_msg_callback = WLDT_Net_HandlePdpMsg;
    active_info.ps_service_type     = BROWSER_E;
    active_info.storage             = MN_GPRS_STORAGE_ALL;

    apn_idx_arr = MMIAPICONNECTION_GetApnTypeIndex(sim_sys);
    if (apn_idx_arr == NULL)
    {
        WLDT_NET_TRACE("WLDT NET ConnectPDP [APN IDX ERROR]");
        return FALSE;
    }
    WLDT_NET_TRACE("WLDT NET  ConnectPDP [sim = %d, Net Setting = %d]", sim_sys, apn_idx_arr->internet_index[sim_sys].index);
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
            WLDT_NET_TRACE("WLDT NET PdpActive GPRS sim_sys=%d",sim_sys);
            WLDT_Net_StartPdpActiveTimer();
            return_val = TRUE;
        }
    }
    return return_val;
#endif
    return FALSE;
}
#else
LOCAL BOOLEAN  WLDT_Net_PDPTry(MN_DUAL_SYS_E sim_sys)
{
    MMIPDP_ACTIVE_INFO_T    active_info     = {0};
    BOOLEAN                 return_val      = FALSE;
    
    active_info.app_handler         = MMI_MODULE_WLDT;
    active_info.dual_sys            = sim_sys;
    active_info.apn_ptr             = WLDT_Net_GetApnStr(sim_sys);
    active_info.user_name_ptr       = WLDT_Net_GetUserNameStr(sim_sys);
    active_info.psw_ptr             = WLDT_Net_GetPasswordStr(sim_sys);
    active_info.priority            = 3;
    active_info.ps_service_rat      = MN_UNSPECIFIED;
    active_info.ps_interface        = MMIPDP_INTERFACE_GPRS;
    active_info.handle_msg_callback = WLDT_Net_HandlePdpMsg;
    active_info.ps_service_type = BROWSER_E;
    active_info.storage = MN_GPRS_STORAGE_ALL;

    if(MMIAPIPDP_Active(&active_info))
    {
        WLDT_NET_TRACE("WLDT NET PdpActive GPRS sim_sys=%d",sim_sys);
        WLDT_Net_StartPdpActiveTimer();
        return_val = TRUE;
    }
    return return_val;
}
#endif

LOCAL BOOLEAN WLDT_Net_PdpActive(void)
{
    BOOLEAN                 return_val      = FALSE;
    MN_DUAL_SYS_E           dual_sys        = MN_DUAL_SYS_MAX;
    MMIPDP_ACTIVE_INFO_T    active_info     = {0};
    
    WLDT_NET_TRACE("WLDT NET PdpActive Start is_activing=%d,is_ok=%d",s_wldt_net_is_activing,s_wldt_net_pdp_state_is_ok);
    if(s_wldt_net_is_activing)
    {
        s_wldt_net_need_active_close = FALSE;
        return TRUE;
    }
    s_wldt_net_is_activing = TRUE;
    
    if(s_wldt_net_pdp_state_is_ok)
    {
        MMIWLDT_SendSigTo_APP(WLDT_APP_SIGNAL_NET_ACTIVE_SUCCESS,PNULL,0);
        return TRUE;
    }

#if 0 //def WIFI_SUPPORT    
    //优先wifi
    if(s_wldt_net_set_sys == MN_DUAL_SYS_MAX && MMIWIFI_STATUS_CONNECTED == MMIAPIWIFI_GetStatus())
    {
        active_info.app_handler         = MMI_MODULE_WLDT;
        active_info.handle_msg_callback = WLDT_Net_HandlePdpMsg;
        active_info.ps_interface        = MMIPDP_INTERFACE_WIFI;
        if(MMIAPIPDP_Active(&active_info))
        {
            WLDT_NET_TRACE("WLDT NET PdpActive OK WIFI");
            WLDT_Net_StartPdpActiveTimer();
            return_val = TRUE;
        }
        else
        {
            WLDT_NET_TRACE("WLDT NET PdpActive ERR WIFI");
            MMIWLDT_SendSigTo_APP(WLDT_APP_SIGNAL_NET_ACTIVE_FAIL,PNULL,0);
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
            WLDT_NET_TRACE("WLDT NET PdpActive ERR NO SIM");
            MMIWLDT_SendSigTo_APP(WLDT_APP_SIGNAL_NET_ACTIVE_FAIL,PNULL,0);
            return FALSE;
        }
        
        if(s_wldt_net_is_init == FALSE)
        {
            WLDT_NET_TRACE("WLDT NET PdpActive ERR NO INIT");
            MMIWLDT_SendSigTo_APP(WLDT_APP_SIGNAL_NET_ACTIVE_FAIL,PNULL,0);
            return FALSE;
        }

        if(s_wldt_net_set_sys < MN_DUAL_SYS_MAX)
        {
            if(MMIAPIPHONE_IsSimAvailable(s_wldt_net_set_sys))
            {
                s_wldt_net_cur_sys = s_wldt_net_set_sys;
            }
            else
            {
                WLDT_NET_TRACE("WLDT NET PdpActive ERR set_sys(%d) NO SIM",s_wldt_net_set_sys);
                MMIWLDT_SendSigTo_APP(WLDT_APP_SIGNAL_NET_ACTIVE_FAIL,PNULL,0);
                return FALSE;
            }
        }
        else
        {
            s_wldt_net_cur_sys = sim_ok;
        }

        return_val = WLDT_Net_PDPTry(s_wldt_net_cur_sys);//第一张有效卡
        if(FALSE == return_val)
        {
            WLDT_NET_TRACE("WLDT NET PdpActive ERR cur_sys(%d/%d),",s_wldt_net_cur_sys,sim_ok_num);
            #if 0
            if(sim_ok_num > 1 && s_wldt_net_set_sys == MN_DUAL_SYS_MAX)
            {
                if(0 != s_wldt_net_pdp_retry_timer_id)
                {
                    MMK_StopTimer(s_wldt_net_pdp_retry_timer_id);
                    s_wldt_net_pdp_retry_timer_id = 0;
                }
                s_wldt_net_pdp_retry_timer_id = MMK_CreateTimerCallback(
                                                                                                WLDT_PDP_RETRY_TIMEOUT, 
                                                                                                WLDT_Net_HandlePdpRetryTimer, 
                                                                                                PNULL, 
                                                                                                FALSE
                                                                                                );
            }
            else
            #endif
            {
                MMIWLDT_SendSigTo_APP(WLDT_APP_SIGNAL_NET_ACTIVE_FAIL,PNULL,0);
            }
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
LOCAL BOOLEAN WLDT_Net_PdpDeactive(void)
{
    BOOLEAN return_val = FALSE;
    
    return_val = MMIAPIPDP_Deactive(MMI_MODULE_WLDT);

    return return_val;
}
PUBLIC BOOLEAN WLDT_Net_IsAllow(void)
{
    if(MMIAPISET_GetFlyMode())
    {
        WLDT_NET_TRACE("WLDT_Net_IsAllow FASE In Fly");
        return FALSE;
    }
    if(MMIWLDT_Net_GPRS_IsOn() == FALSE)
    {
        WLDT_NET_TRACE("WLDT_Net_IsAllow FASE  GPRS OFF");
        return FALSE;
    }
#ifdef WLDT_TCP_SUPPORT
#ifdef WLDT_PLAT_CY_SUPPORT
    if(CY_Net_IsAllow(&g_cy_app))
    {
        return TRUE;
    }
#endif
#endif
    return FALSE;

}
LOCAL BOOLEAN WLDT_Net_Init(void)
{
    MN_DUAL_SYS_E dual_sys = MMIWLDT_Net_GetActiveSys();
    WLDT_NET_TRACE("WLDT_Net_Init is_init=%d, dual_sys=%d",s_wldt_net_is_init,dual_sys);
    if(s_wldt_net_is_init == FALSE)
    {
        WLDT_NET_TRACE("WLDT_Net_Init imei=%s,imsi=%s,iccid=%s",g_wldt_phone_imei,g_wldt_sim_imsi,g_wldt_sim_iccid);
	if(g_wldt_net_mutexPtr == NULL) {
		g_wldt_net_mutexPtr = SCI_CreateMutex("wldtnet-mutex", SCI_INHERIT);
	}
#ifdef MMI_GPRS_SUPPORT
        //MMIAPICONNECTION_SetGPRSStatus(MMICONNECTION_GPRS_ALWAYS); //set GPRS alwarys connection
#endif

#ifdef WLDT_TCP_SUPPORT
        WldtTCP_NetInit();
#endif
        if(WLDT_Net_IsAllow())
        {
            MMIWLDT_Net_Open();
        }
        s_wldt_net_is_init = TRUE;
    }
    return TRUE;
}

LOCAL BOOLEAN WLDT_Net_Open(void)
{
    WLDT_NET_TRACE("WLDT_Net_Open state=%d,is_activing=%d,is_closing=%d",s_wldt_net_pdp_state_is_ok,s_wldt_net_is_activing,s_wldt_net_is_closing);
    if(s_wldt_net_is_closing)
    {
        s_wldt_net_need_close_active = TRUE;
    }
    else
    {
        WLDT_Net_PdpActive();
    }
    return TRUE;
}

LOCAL BOOLEAN WLDT_Net_CloseEx(void)
{
    if(s_wldt_net_is_closing)
    {
        s_wldt_net_need_close_active = FALSE;
        return TRUE;
    }
    s_wldt_net_is_closing = TRUE;
    if(s_wldt_net_pdp_state_is_ok)
    {
        if(FALSE == WLDT_Net_PdpDeactive())
        {
            MMIWLDT_SendSigTo_APP(WLDT_APP_SIGNAL_NET_CONNET_FAIL,PNULL,0);
        }
    }
    else
    {
        WLDT_Net_PdpDeactive();
        MMIWLDT_SendSigTo_APP(WLDT_APP_SIGNAL_NET_CONNET_FAIL,PNULL,0);
    }
}

LOCAL BOOLEAN WLDT_Net_Close(void)
{
    WLDT_NET_TRACE("WLDT_Net_Close state=%d,is_activing=%d,is_closing=%d",s_wldt_net_pdp_state_is_ok,s_wldt_net_is_activing,s_wldt_net_is_closing);
    if(s_wldt_net_is_activing)
    {
        s_wldt_net_need_active_close = TRUE;
    }
    else
    {
        WLDT_Net_CloseEx();
    }
    return TRUE;
}


LOCAL BOOLEAN WLDT_Net_PdpStateChange(BOOLEAN is_ok)
{
    WLDT_NET_TRACE("WLDT NET PDPStateChange %d",is_ok);
    if(is_ok)
    {
        if(s_wldt_net_need_active_close == FALSE)
        {
#ifdef WLDT_TCP_SUPPORT
            WldtTCP_NetReset();
#endif
        }
    }

    return TRUE;
}

LOCAL void WLDT_Net_PdpStateUpdate(BOOLEAN is_ok,BOOLEAN is_active)
{
    WLDT_NET_TRACE("WLDT_Net_PdpStateUpdate is_ok=%d,is_active=%d,activing=%d,closing=%d",is_ok,is_active,s_wldt_net_is_activing,s_wldt_net_is_closing);
    if(s_wldt_net_pdp_state_is_ok != is_ok)
    {
        s_wldt_net_pdp_state_is_ok = is_ok;
        WLDT_Net_PdpStateChange(is_ok);
    }
    
    if(is_active)
    {
        if(s_wldt_net_is_activing)
        {
            s_wldt_net_is_activing = FALSE;
            if(s_wldt_net_need_active_close)
            {
                WLDT_NET_TRACE("WLDT_Net_PdpStateUpdate Active Close");
                s_wldt_net_need_active_close = FALSE;
                MMIWLDT_SendSigTo_APP(WLDT_APP_SIGNAL_NET_CLOSE,PNULL,0);
            }
        }
    }
    else
    {   
        s_wldt_net_is_activing = FALSE;
        if(s_wldt_net_is_closing)
        {
            s_wldt_net_is_closing = FALSE;
            if(s_wldt_net_need_close_active)
            {
                WLDT_NET_TRACE("WLDT_Net_PdpStateUpdate Close Active");
                s_wldt_net_need_close_active = FALSE;
                MMIWLDT_SendSigTo_APP(WLDT_APP_SIGNAL_NET_OPEN,PNULL,0);
            }
        }
    }
    return;
}

PUBLIC uint32 MMIWLDT_Net_GetNetID(void)
{
    return s_wldt_net_id;
}

PUBLIC BOOLEAN MMIWLDT_Net_PdpStateGet(void)
{
    return s_wldt_net_pdp_state_is_ok;
}

PUBLIC BOOLEAN MMIWLDT_Net_IsActiving(void)
{
    return s_wldt_net_is_activing;
}

PUBLIC BOOLEAN MMIWLDT_Net_IsClosing(void)
{
    return s_wldt_net_is_closing;
}


PUBLIC BOOLEAN MMIWLDT_Net_IsInit(void)
{
    return s_wldt_net_is_init;
}

PUBLIC BOOLEAN MMIWLDT_Net_Init(void)
{
    MMIWLDT_SendSigTo_APP(WLDT_APP_SIGNAL_NET_INIT,PNULL,0);
    return TRUE;
}

LOCAL void WLDT_Net_HandleResetTimer(
                                            uint8 timer_id,
                                            uint32 param
                                            )
{
    WLDT_NET_TRACE("WLDT_Net_HandleResetTimer");
    if(g_wldt_net_mutexPtr != NULL)
    {
        SCI_GetMutex(g_wldt_net_mutexPtr, SCI_WAIT_FOREVER);
    }
    MMIWLDT_Net_ResetExt();
    if(g_wldt_net_mutexPtr != NULL)
    {
        SCI_PutMutex(g_wldt_net_mutexPtr);
    }
}

LOCAL void WLDT_Net_StartResetTimer(uint32 time_ms)
{
    if(0 != s_wldt_net_reset_timer_id)
    {
        return;
    }
    s_wldt_net_reset_timer_id = MMK_CreateTimerCallback(wldt_net_reset_interval_time, 
                                                                        WLDT_Net_HandleResetTimer, 
                                                                        PNULL, 
                                                                        FALSE);
}

LOCAL void WLDT_Net_StopResetTimer(void)
{
    if(0 != s_wldt_net_reset_timer_id)
    {
        MMK_StopTimer(s_wldt_net_reset_timer_id);
        s_wldt_net_reset_timer_id = 0;
    }
}

static void MMIWLDT_Net_ResetExt(void)
{
    uint32 cur_time = SCI_GetTickCount();
    uint32 offset_time_ms = 0;
    offset_time_ms = (y_abs(cur_time - wldt_net_reset_last_land_time_ms));
    
    WLDT_Net_StopResetTimer();
    if(offset_time_ms > 3600000 && 0 == MNPHONE_GetAttState(MMIWLDT_Net_GetActiveSys()))
    {

            MMIWLDT_SendSigTo_APP(WLDT_APP_SIGNAL_NET_RESET,PNULL,0);
    }
    else
    {
        MMIWLDT_SendSigTo_APP(WLDT_APP_SIGNAL_NET_RESET,PNULL,0);
    }
    last_reset_msg_time = SCI_GetTickCount();
}

PUBLIC void MMIWLDT_Net_Reset_Restore(void)
{
    //网络登陆成功后复位参数
    if(g_wldt_net_mutexPtr != NULL)
    {
        SCI_GetMutex(g_wldt_net_mutexPtr, SCI_WAIT_FOREVER);
    }
    WLDT_Net_StopResetTimer();
    wldt_net_reset_interval_time = 10000;
    wldt_net_reset_interval_num = 0;
    wldt_net_reset_last_land_time_ms = SCI_GetTickCount();
    if(g_wldt_net_mutexPtr != NULL)
    {
        SCI_PutMutex(g_wldt_net_mutexPtr);
    }
    return;
}

static void MMIWLDT_Net_Reset_CheckMode(void)
{
    uint32 cur_time = SCI_GetTickCount();
    uint32 offset_time_ms = 0;
    offset_time_ms = (y_abs(cur_time - wldt_net_reset_last_land_time_ms));
    
    if(offset_time_ms < 600000)
    {
        //    10分钟内没有登陆每30 秒复位一次
        wldt_net_reset_interval_time = 30000;
    }
    else if(offset_time_ms < 3600000)
    {
        //    1小时内没有登陆每5分钟复位一次
        wldt_net_reset_interval_time = 300000;
    }
    else
    {
        //其它每两个小时一次
        wldt_net_reset_interval_time = 7200000;
    }
    return;
}

PUBLIC BOOLEAN MMIWLDT_Net_Reset(BOOLEAN is_now)
{
    uint32 cur_time = SCI_GetTickCount();
    uint32 offset_times_ms = 0;
    if(FALSE == MMIWLDT_Net_GPRS_IsOn() || MMIAPISET_GetFlyMode() || s_wldt_net_is_init == FALSE)
    {
        WLDT_LOG("MMIWLDT_Net_Reset In Fly");
        return FALSE;
    }
    
    if(g_wldt_net_mutexPtr != NULL)
    {
        SCI_GetMutex(g_wldt_net_mutexPtr, SCI_WAIT_FOREVER);
    }
    offset_times_ms = y_abs(cur_time - last_reset_msg_time);
    MMIWLDT_Net_Reset_CheckMode();
    WLDT_LOG("MMIWLDT_Net_Reset cur_time=%d,%d,interval=%d",cur_time,last_reset_msg_time,wldt_net_reset_interval_time);
    if(is_now == FALSE && offset_times_ms < wldt_net_reset_interval_time)
    {
        WLDT_LOG("MMIWLDT_Net_Reset reset too fast");
        WLDT_Net_StartResetTimer(offset_times_ms);
    }
    else
    {
        MMIWLDT_Net_ResetExt();
    }
    
    if(g_wldt_net_mutexPtr != NULL)
    {
        SCI_PutMutex(g_wldt_net_mutexPtr);
    }
    return TRUE;
}

PUBLIC BOOLEAN MMIWLDT_Net_Reset_Check(void)
{
    if(s_wldt_net_is_init && s_wldt_net_id == 0)
    {
        WLDT_LOG("MMIWLDT_Net_Reset_Check");
        MMIWLDT_Net_Reset(FALSE);
    }
    
    return TRUE;
}
PUBLIC BOOLEAN MMIWLDT_Net_Open(void)
{
    MMIWLDT_SendSigTo_APP(WLDT_APP_SIGNAL_NET_OPEN,PNULL,0);
    return TRUE;
}

PUBLIC BOOLEAN MMIWLDT_Net_Close(void)
{
    MMIWLDT_SendSigTo_APP(WLDT_APP_SIGNAL_NET_CLOSE,PNULL,0);
    return TRUE;
}

LOCAL MN_DUAL_SYS_E MMIWLDT_Net_NV_GetActiveSys(void)
{
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    uint8 net_sys = WLDT_PDP_DEFAULT_SIM_SYS;
    //get if is flip answer
    MMINV_READ(MMI_WLDT_NV_NET_SYS,&net_sys,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        //自动识别
        net_sys = WLDT_PDP_DEFAULT_SIM_SYS;
        MMINV_WRITE(MMI_WLDT_NV_NET_SYS,&net_sys);
    }
    s_wldt_net_set_sys = net_sys;
    return net_sys;
}

LOCAL BOOLEAN  MMIWLDT_Net_NV_SetActiveSys(MN_DUAL_SYS_E set_sys) 
{
    uint8 net_sys = set_sys;
    if(s_wldt_net_set_sys != set_sys)
    {
        MMINV_WRITE(MMI_WLDT_NV_NET_SYS,&net_sys);
        s_wldt_net_set_sys = set_sys;
        return TRUE;
    }
    return FALSE;
}

PUBLIC MN_DUAL_SYS_E MMIWLDT_Net_GetActiveSys(void)
{
    return s_wldt_net_set_sys;
}

PUBLIC void MMIWLDT_Net_SetActiveSys(MN_DUAL_SYS_E set_sys) 
{
    WLDT_NET_TRACE("MMIWLDT_Net_SetActiveSys set_sys=%d",set_sys);
    SCI_MEMSET(g_wldt_sim_imsi,0,WLDT_IMSI_LEN+1);
    SCI_MEMSET(g_wldt_sim_iccid,0,WLDT_ICCID_LEN+1);
    MMIWLDT_Net_NV_SetActiveSys(set_sys);
    MMIWLDT_Get_SIM_IMSI(set_sys,g_wldt_sim_imsi);
    MMIWLDT_Get_SIM_ICCID(set_sys,g_wldt_sim_iccid);
    #ifdef WLDT_NET_FIX_IMEI
        g_wldt_phone_imei = g_wldt_phone_imei_1;
    #else
    if(set_sys == MN_DUAL_SYS_1)
    {
        g_wldt_phone_imei = g_wldt_phone_imei_1;
    }
    else
    {
        g_wldt_phone_imei = g_wldt_phone_imei_2;
    }
    #endif
    return;
}

LOCAL BOOLEAN  MMIWLDT_Net_Handle_Init(DPARAM param)
{
    WLDT_NET_TRACE("MMIWLDT_Net_Handle_Init");
    WLDT_Net_Init();
    return TRUE;
}

LOCAL BOOLEAN  MMIWLDT_Net_Handle_Open(DPARAM param)
{
    WLDT_NET_TRACE("MMIWLDT_Net_Handle_Open");
    WLDT_Net_Open();
    return TRUE;
}

LOCAL BOOLEAN  MMIWLDT_Net_Handle_Close(DPARAM param)
{
    WLDT_NET_TRACE("MMIWLDT_Net_Handle_Close");
    WLDT_Net_Close();
    return TRUE;
}

LOCAL BOOLEAN  MMIWLDT_Net_Handle_Reset(DPARAM param)
{
    WLDT_NET_TRACE("MMIWLDT_Net_Handle_Reset");
    MMIWLDT_SendSigTo_APP(WLDT_APP_SIGNAL_NET_CLOSE,PNULL,0);
    MMIWLDT_SendSigTo_APP(WLDT_APP_SIGNAL_NET_OPEN,PNULL,0);
    return TRUE;
}

LOCAL BOOLEAN  MMIWLDT_Net_Handle_PDPActiveOK(DPARAM param)
{
    WLDT_NET_TRACE("MMIWLDT_Net_Handle_PDPActiveOK net_is_activing=%d",s_wldt_net_is_activing);
    WLDT_Net_PdpStateUpdate(TRUE,TRUE);

#ifdef WLDT_TCP_SUPPORT
    MMIWLDT_TCP_Handle_PDPActiveOK();
#endif
    return TRUE;
}

LOCAL BOOLEAN  MMIWLDT_Net_Handle_PDPActiveFail(DPARAM param)
{
    WLDT_NET_TRACE("MMIWLDT_Net_Handle_PDPActiveFail net_is_activing=%d",s_wldt_net_is_activing);
    WLDT_Net_PdpStateUpdate(FALSE,TRUE);
#ifdef WLDT_TCP_SUPPORT
    MMIWLDT_TCP_Handle_PDPActiveFail();
#endif
    return TRUE;
}

LOCAL BOOLEAN  MMIWLDT_Net_Handle_PDPRespond(DPARAM param)
{
    WLDT_NET_TRACE("WLDT NET Handle_PDP_RSP net_is_activing=%d",s_wldt_net_is_activing);
    WLDT_Net_PdpStateUpdate(FALSE,FALSE);
#ifdef WLDT_TCP_SUPPORT
    MMIWLDT_TCP_Handle_PDPRespond();
#endif
    if(s_wldt_net_cur_gprs_status && WLDT_Net_IsAllow())
    {
        //MMIWLDT_Net_Reset(FALSE);
    }
    return TRUE;
}


PUBLIC MMI_RESULT_E MMIWLDT_Net_Handle_AppMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
    MMI_RESULT_E res = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case WLDT_APP_SIGNAL_NET_INIT:
            MMIWLDT_Net_Handle_Init(param);
        break;
        
    case WLDT_APP_SIGNAL_NET_OPEN:
            MMIWLDT_Net_Handle_Open(param);
        break;
    case WLDT_APP_SIGNAL_NET_CLOSE:
            MMIWLDT_Net_Handle_Close(param);
        break;
        
    case WLDT_APP_SIGNAL_NET_RESET:
            MMIWLDT_Net_Handle_Reset(param);
        break;
        
    case WLDT_APP_SIGNAL_NET_ACTIVE_SUCCESS:
            MMIWLDT_Net_Handle_PDPActiveOK(param);
        break;
        
    case WLDT_APP_SIGNAL_NET_ACTIVE_FAIL:
            MMIWLDT_Net_Handle_PDPActiveFail(param);
        break;
        
    case WLDT_APP_SIGNAL_NET_CONNET_FAIL:
            MMIWLDT_Net_Handle_PDPRespond(param);
        break;
    
    default:
            res = MMI_RESULT_FALSE;
        break;
    }

    if(res == MMI_RESULT_FALSE)
    {
        #ifdef WLDT_TCP_SUPPORT
            res = MMIWLDT_TCP_Handle_AppMsg(app_ptr,msg_id,param);
        #endif
    }
    if(res == MMI_RESULT_FALSE)
    {
        #ifdef WLDT_PLAT_CY_SUPPORT
            res = CY_Handle_AppMsg(app_ptr,msg_id,param);
        #endif
    }
    return res;
}

// 0: 相同
LOCAL uint8  MMIWLDT_Station_IsSame(MMIWLDT_STATION_DATA_T  * station1_ptr,MMIWLDT_STATION_DATA_T  * station2_ptr,int num)
{
    uint8 is_same = 1;
    uint16 same_num = 0;
    uint16 valid_num = 0;
    uint16 station_num = 0;
    uint16 i = 0;
    uint16 j = 0;

    if(num > 0)
    {
        station_num = num;
    }
    
    if(station1_ptr->s_cell.is_valid && station2_ptr->s_cell.is_valid )
    {
        if( //station1_ptr->s_cell.mnc == station2_ptr->s_cell.mnc  &&
            //station1_ptr->s_cell.mcc == station2_ptr->s_cell.mcc &&
            station1_ptr->s_cell.lac == station2_ptr->s_cell.lac 
            )
        {
            for(i = 0; i < WLDT_N_STATION_NUM; i++)
            {
                if(station1_ptr->n_cell[i].is_valid )
                {
                    valid_num++;
                    for(j = 0; j < WLDT_N_STATION_NUM; j++)
                    {
                        if(station2_ptr->n_cell[j].is_valid 
                            && station2_ptr->n_cell[j].cell_id == station1_ptr->n_cell[i].cell_id
                            )
                        {
                            same_num++;
                            break;
                        }
                    }
                }
            }

            if(station_num == 0) //需要全部相同
            {
                if(valid_num == same_num)
                {
                    is_same = 0;
                }
                else
                {
                    is_same = 1;
                }
            }
            else
            {
                if(same_num >= station_num)
                {
                    is_same = 0;
                }
                else
                {
                    is_same = 1;
                }
            }
        }
        else
        {
            is_same = 1;
        }
    }
    else
    {
        is_same = 2;
    }
             
    return is_same;
}

LOCAL BOOLEAN  MMIWLDT_Station_GetSCNetInfo(MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN res = FALSE;
    if(g_wldt_station_data.s_cell.cell_id  != 0)

    {
        res = TRUE;
        g_wldt_station_data.s_cell.is_valid = TRUE;
    }
    else
    {
        g_wldt_station_data.s_cell.is_valid = FALSE;
    }
    return res;
}

LOCAL BOOLEAN   MMIWLDT_Station_GetNCNetInfo(MN_DUAL_SYS_E dual_sys)
{
    APP_MN_NCELL_RSSI_CNF_T ncell = {0};
    MN_PHONE_CELL_MEAS_T    scell = {0};
    int i = 0;
    int index_present[WLDT_N_STATION_NUM] = {0};
    int num_present = 0;
    BOOLEAN res = FALSE;
    
#ifndef WIN32
    if (MNPHONE_GetMeasReportEx(dual_sys, &ncell,&scell) == MN_RETURN_SUCCESS)
#else 
    for (i = 0; i < WLDT_N_STATION_NUM; i++)		// this is for example for simulator
    {
        ncell.ncell_power[i].cell_present = TRUE;
        ncell.ncell_power[i].arfcn = i * 10;
        ncell.ncell_power[i].rxlev = i * 50;
    }
#endif
    {
        for (i = 0; i < WLDT_N_STATION_NUM; i++)		// this is for example for simulator
        {
            g_wldt_station_data.n_cell[i].is_valid = FALSE;
            if (ncell.ncell_power[i].cell_present && ncell.ncell_power[i].cell_id != 0)	// this channel is served
            {
                res = TRUE;
                index_present[num_present++] = i;
            }
        }
        for (i = 0; i < num_present; i++)
        {
          g_wldt_station_data.n_cell[i].cell_id = ncell.ncell_power[index_present[i]].cell_id;
          g_wldt_station_data.n_cell[i].rssiLevel = ncell.ncell_power[index_present[i]].rxlev;
          g_wldt_station_data.n_cell[i].dual_sys = dual_sys;
          g_wldt_station_data.n_cell[i].is_valid = TRUE;
        }
    }
    return res;
}

LOCAL BOOLEAN  MMIWLDT_Station_IsNew(APP_MN_NETWORK_STATUS_IND_T *status_ptr)
{
    BOOLEAN res = FALSE;
    BOOLEAN need_check = FALSE;
    if(s_wldt_net_need_init)
    {
        need_check = TRUE;
    }
    else if(status_ptr->dual_sys == s_wldt_net_set_sys)
    {
        need_check = TRUE;
    }
    
    if(need_check)
    {
        if(g_wldt_s_station_data.mcc != status_ptr->mcc
            || g_wldt_s_station_data.mnc != status_ptr->mnc 
            || g_wldt_s_station_data.mnc_digit_num != status_ptr->mnc_digit_num
            || g_wldt_s_station_data.lac != status_ptr->lac
            || g_wldt_s_station_data.cell_id != status_ptr->cell_id
            )
        {
            if(status_ptr->cell_id != 0)
            {
                res = TRUE;
                g_wldt_s_station_data.dual_sys = status_ptr->dual_sys;
                g_wldt_s_station_data.lac = status_ptr->lac;
                g_wldt_s_station_data.mcc = status_ptr->mcc;
                g_wldt_s_station_data.mnc = status_ptr->mnc;
                g_wldt_s_station_data.mnc_digit_num = status_ptr->mnc_digit_num;
                g_wldt_s_station_data.cell_id = status_ptr->cell_id;
                g_wldt_s_station_data.is_valid = TRUE;
            }
            else
            {
                g_wldt_s_station_data.is_valid = FALSE;
            }
        }
    }
    return res;
}

BOOLEAN MMIWLDT_Station_Update(void)
{
    BOOLEAN res = FALSE;
    g_wldt_station_data.s_cell = g_wldt_s_station_data;
    if(MMIWLDT_Station_GetSCNetInfo(s_wldt_net_set_sys))
    {
        res = MMIWLDT_Station_GetNCNetInfo(s_wldt_net_set_sys);
    }
    else
    {
        uint16 i = 0;
        for (i = 0; i < WLDT_N_STATION_NUM; i++)		// this is for example for simulator
        {
            g_wldt_station_data.n_cell[i].is_valid = FALSE;
        }
    }
    
    return res;
}

#ifdef WLDT_TCP_SUPPORT
static int wldt_net_tcp_test_receive(void *pUser,uint8 * pRcv,uint32 Rcv_len)
{
    WLDT_TCP_INTERFACE_T * pMe = (WLDT_TCP_INTERFACE_T *)pUser;
    WLDT_LOG("TCP TEST Rcv_len=%d",Rcv_len);
    return WLDT_TCP_RET_OK;
}

static int wldt_net_tcp_test_callback(void * pUser,WLDT_TCP_CB_TYPE_E cb_type,uint16 wParam, void * dwParam)
{
    WLDT_TCP_INTERFACE_T * pMe = (WLDT_TCP_INTERFACE_T *)pUser;
    WLDT_LOG("TCP TEST Callback: cb_type=%d,Result=%d,Data=0x%x",cb_type,wParam,dwParam);
    switch(cb_type)
    {
        case WLDT_TCP_CB_REG:
            {
                uint32 send_hande = 0;
                uint16 ret = wParam;
                uint32 err_id =  wParam;
                if(ret == WLDT_TCP_SUCCESS)
                {
                    //注册成功
                    WLDT_LOG("TCP TEST REG OK");
                    send_hande = WLDT_API_TCP_SendData(pMe,(uint8 *)"Test TCP\r\n",10,0,0,0,wldt_net_tcp_test_receive);
                    WLDT_LOG("TCP TEST Send Test TCP send_hande=0x%x",send_hande);
                }
                else
                {
                    //注册失败
                    WLDT_LOG("TCP TEST REG FAIL");
                }
            }
            break;
            
        case WLDT_TCP_CB_UNREG:
            {
                uint16 ret = wParam;
                uint32 err_id =  wParam;
                if(ret == WLDT_TCP_SUCCESS)
                {
                    //注销成功
                    WLDT_LOG("TCP TEST UNREG OK");
                }
                else
                {
                    //注销失败
                    WLDT_LOG("TCP TEST UNREG FAIL");
                }
            }
            break;
            
        case WLDT_TCP_CB_SEND:
            {
                uint16 ret = wParam;
                WLDT_TCP_LINK_DATA_T * SendData = (WLDT_TCP_LINK_DATA_T*)dwParam;
                if(ret == WLDT_TCP_SUCCESS)
                {
                    //发送成功
                    WLDT_LOG("TCP TEST Send OK Handle=0x%x",SendData);
                    if(SendData != NULL && SendData->str != NULL)
                    {
                        WLDT_LOG("%s",SendData->str);
                    }
                }
                else
                {
                    //发送失败
                    WLDT_LOG("TCP TEST Send FAIL Handle=0x%x",SendData);
                    if(SendData != NULL && SendData->str != NULL)
                    {
                        WLDT_LOG("%s",SendData->str);
                    }
                }
            }
            break;
            
        case WLDT_TCP_CB_RCV:
            {
                uint16 ret = wParam;
                WLDT_TCP_RCV_DATA_T * RcvData = (WLDT_TCP_RCV_DATA_T *) dwParam;
                if(ret == WLDT_TCP_SUCCESS)
                {
                    //接收成功
                    WLDT_LOG("TCP TEST RCV OK len=%d",RcvData->len);
                    if(RcvData != NULL && RcvData->pRcv!= NULL)
                    {
                        //WLDT_LOG("%s",RcvData->pRcv);
                    }
                }
                else
                {
                    //接收失败
                    WLDT_LOG("TCP TEST RCV FAIL");
                    if(RcvData != NULL && RcvData->pRcv!= NULL)
                    {
                        //WLDT_LOG("%s",RcvData->pRcv);
                    }
                }
            }
            break;
                        
        default:
            break;
    }
    return 0;
}

void wldt_net_tcp_test_start(WLDT_TCP_INTERFACE_T *pMe)
{
    WLDT_LOG("wldt_net_tcp_test_start");
    WLDT_API_TCP_UnReg(pMe);
    WLDT_API_TCP_Reg(pMe,"120.24.172.55",9103,wldt_net_tcp_test_callback);
}
#endif


LOCAL void WLDT_Net_HandleGPRSCheckTimer(
                                            uint8 timer_id,
                                            uint32 param
                                            )
{
    WLDT_NET_TRACE("WLDT NET GPRSCheck ERR TimeOut");
    s_wldt_net_cur_gprs_status = FALSE;
    if(s_wldt_net_need_init == FALSE)
    {
        MMIWLDT_Net_Close();
    }
}

/*****************************************************************************/
//  Discription: Start socket connect timer
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
/*****************************************************************************/
LOCAL void WLDT_Net_StartGPRSCheckTimer(void)
{
    if(0 != s_wldt_net_gprs_check_timer_id)
    {
        MMK_StopTimer(s_wldt_net_gprs_check_timer_id);
        s_wldt_net_gprs_check_timer_id = 0;
    }
    
    s_wldt_net_gprs_check_timer_id = MMK_CreateTimerCallback(WLDT_GPRS_CHECK_TIMEOUT, 
                                                                        WLDT_Net_HandleGPRSCheckTimer, 
                                                                        PNULL, 
                                                                        FALSE);
}
/*****************************************************************************/
//  Discription: Start socket connect timer
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
/*****************************************************************************/
LOCAL void WLDT_Net_StopGPRSCheckTimer(void)
{
    if(0 != s_wldt_net_gprs_check_timer_id)
    {
        MMK_StopTimer(s_wldt_net_gprs_check_timer_id);
        s_wldt_net_gprs_check_timer_id = 0;
    }
}

LOCAL  BOOLEAN MMIWLDT_Net_StatusChange(MN_DUAL_SYS_E dual_sys,BOOLEAN is_ok)
{
    BOOLEAN last_gprs_status = s_wldt_net_cur_gprs_status;
    WLDT_NET_TRACE("WLDT NET StatusChange set_sys=%d,dual_sys=%d,is_ok=%d,need_init=%d,last_gprs_status=%d",s_wldt_net_set_sys,dual_sys,is_ok,s_wldt_net_need_init,last_gprs_status);
    if(FALSE == MMIWLDT_Net_GPRS_IsOn() || MMIAPISET_GetFlyMode())
    {
        WLDT_LOG("MMIWLDT_Net_StatusChange In Fly");
        s_wldt_net_cur_gprs_status = FALSE;
        return FALSE;
    }
    if(s_wldt_net_need_init == FALSE)
    {
        if(s_wldt_net_set_sys != dual_sys)
        {
            return FALSE;
        }
    }
    if(is_ok)
    {
        WLDT_Net_StopGPRSCheckTimer();
        s_wldt_net_cur_gprs_status = TRUE;
        if(s_wldt_net_need_init )
        {
            MMISET_MAJOR_SIM_SEL_TYPE_E major_sim_type = MMISET_MAJOR_SIM_SEL_MANUAL_SEL;
            major_sim_type = MMIAPISET_GetMajorSim(MMISET_MAJOR_SIM_SEL_FUNC_WAP);
            if ((major_sim_type < MMISET_MAJOR_SIM_SEL_MANUAL_SEL)
                && MMIAPIPHONE_IsSimAvailable(MN_DUAL_SYS_1+(major_sim_type-MMISET_MAJOR_SIM_SEL_SIM1))/*lint !e656*/ 
                )
            {
                MMIWLDT_Net_SetActiveSys(major_sim_type);
            }
            else
            {
                MMIWLDT_Net_SetActiveSys(dual_sys);
            }
            if(s_wldt_net_set_sys != dual_sys)
            {
                s_wldt_net_cur_gprs_status = FALSE;
            }
            else
            {
                MMIWLDT_Net_Init();
                s_wldt_net_need_init = FALSE;
            }
        }
        else
        {
            if(WLDT_Net_IsAllow() && last_gprs_status == FALSE)
            {
                MMIWLDT_Net_Reset(FALSE);
            }
        }
    }
    else
    {
        WLDT_Net_StartGPRSCheckTimer();
    }
    return TRUE;
}

PUBLIC BOOLEAN MMIWLDT_Net_StatusInd(APP_MN_NETWORK_STATUS_IND_T *status_ptr)
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
		status_ptr->plmn_status = PLMN_NORMAL_GSM_GPRS_BOTH;
    }
    #endif
    //WLDT_LOG("MMIWLDT_Net_StatusInd dual_sys=%d(%d),selecting=%d,plmn=%d,gprs_support=%d",status_ptr->dual_sys,s_wldt_net_gprs_status[status_ptr->dual_sys],status_ptr->plmn_is_selecting,status_ptr->plmn_status,status_ptr->gprs_support);
    //WLDT_LOG("MMIWLDT_Net_StatusInd mcc=%d,mnc=%d,lac=%d,mnc_digit_num=%d,cell_id=%d",status_ptr->mcc,status_ptr->mnc,status_ptr->lac,status_ptr->mnc_digit_num,status_ptr->cell_id);
    
    if (status_ptr->plmn_is_selecting)
    {
        if(s_wldt_net_gprs_status[status_ptr->dual_sys])
        {
            s_wldt_net_gprs_status[status_ptr->dual_sys] = FALSE;
            MMIWLDT_Net_StatusChange(status_ptr->dual_sys,FALSE);
        }
    }
    else 
    {
        if(s_wldt_net_gprs_status[status_ptr->dual_sys] == FALSE)
        {
            if(
              	status_ptr->gprs_support
                    && MMIAPIPHONE_IsSimAvailable(status_ptr->dual_sys)
                    && (status_ptr->plmn_status == PLMN_NORMAL_GPRS_ONLY || status_ptr->plmn_status == PLMN_NORMAL_GSM_GPRS_BOTH)
                )
            {
                g_wldt_net_cell_id = status_ptr->cell_id;
                WLDT_LOG("MMIWLDT_Net_StatusInd OK dual_sys=%d,plmn=%d,gprs_support=%d,cell_id = %d",status_ptr->dual_sys,status_ptr->plmn_status,status_ptr->gprs_support,g_wldt_net_cell_id);
                s_wldt_net_gprs_status[status_ptr->dual_sys] = TRUE;
                MMIWLDT_Net_StatusChange(status_ptr->dual_sys,TRUE);
            }
        }
        else
        {
            if(status_ptr->gprs_support == 0 || (status_ptr->plmn_status != PLMN_NORMAL_GSM_ONLY && status_ptr->plmn_status != PLMN_NORMAL_GPRS_ONLY && status_ptr->plmn_status != PLMN_NORMAL_GSM_GPRS_BOTH))
            {
                WLDT_LOG("MMIWLDT_Net_StatusInd FAIL dual_sys=%d, plmn=%d,gprs_support=%d",status_ptr->dual_sys,status_ptr->plmn_status,status_ptr->gprs_support);
                s_wldt_net_gprs_status[status_ptr->dual_sys] = FALSE;
                MMIWLDT_Net_StatusChange(status_ptr->dual_sys,FALSE);
            }
        }
    }
    return TRUE;
}

void MMIWLDT_Net_RssiInd(MN_DUAL_SYS_E dual_sys,uint8 rssi)
{
    if(dual_sys == s_wldt_net_set_sys)
    {
        g_wldt_s_station_data.rssiLevel = rssi;
    }
}

BOOLEAN MMIWLDT_Net_GPRS_SimSwitch(MN_DUAL_SYS_E dual_sys)
{
    WLDT_NET_TRACE("MMIWLDT_Net_GPRS_SimSwitch dual_sys=%s,s_wldt_net_need_init=%d",dual_sys,s_wldt_net_need_init);
    if(s_wldt_net_need_init == FALSE)
    {
        MMIWLDT_Net_SetActiveSys(dual_sys);
        if(WLDT_Net_IsAllow())
        {
            MMIWLDT_Net_Reset(TRUE);
        }
    }
    return TRUE;
}

BOOLEAN MMIWLDT_Net_GPRS_IsOn(void)
{
#ifdef MMI_GPRS_SUPPORT
    MMICONNECTION_SETTING_GPRS_SWITCH_E gprs_switch_settings;
    gprs_switch_settings = MMIAPICONNECTION_GetGPRSSwitchStatus();

    if (gprs_switch_settings == MMICONNECTION_SETTING_GPRS_SWITCH_OFF)
    {
        return FALSE;
    }
#endif
    return TRUE;
}

BOOLEAN MMIWLDT_Net_GPRS_Attach(MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN res = FALSE;
    WLDT_NET_TRACE("MMIWLDT_Net_GPRS_Attach dual_sys=%s,s_wldt_net_set_sys=%d",dual_sys,s_wldt_net_set_sys);
    if(s_wldt_net_need_init == FALSE)
    {
        MMIWLDT_Net_SetActiveSys(dual_sys);
        if(WLDT_Net_IsAllow())
        {
            MMIWLDT_Net_Reset(FALSE);
        }
    }
    return res;
}

BOOLEAN MMIWLDT_Net_GPRS_Detach(MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN res = FALSE;
    WLDT_NET_TRACE("MMIWLDT_Net_GPRS_Detach dual_sys=%s,s_wldt_net_set_sys=%d",dual_sys,s_wldt_net_set_sys);
    return res;
}

PUBLIC BOOLEAN MMIWLDT_Net_APP_Init(void)
{
    MMIWLDT_Net_NV_GetActiveSys();
    MMIWLDT_Net_GetIMEI(MN_DUAL_SYS_1,g_wldt_phone_imei_1);
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    MMIWLDT_Net_GetIMEI(MN_DUAL_SYS_2,g_wldt_phone_imei_2);
#endif
    WLDT_NET_TRACE("WLDT APP NET Init imei=%s, imei2=%s",g_wldt_phone_imei,g_wldt_phone_imei_2);

#ifdef WLDT_TCP_SUPPORT
    WldtTCP_AppInit();
#endif
    return TRUE;
}

uint8 WLDT_Func_GetDateTime(uint8 * pData)
{
    SCI_TIME_T          time;
    SCI_DATE_T          date;
    char tem_buf[100] = {0};
    uint16 len = 0; 

    TM_GetSysDate(&date);
    TM_GetSysTime(&time);
    
    sprintf(tem_buf,"%04d%02d%02d%02d%02d%02d",date.year,date.mon,date.mday,time.hour,time.min,time.sec);
    len = SCI_STRLEN(tem_buf);
    SCI_MEMCPY(pData,tem_buf,len);
    pData[len] = 0;
    return len;
}

uint8 WLDT_Func_ResultStr(uint16 dt_result,uint8 * pData)
{
    uint8 high = 0;
    uint8 low = 0;
    uint8 temp = 0;
    char out_buf[100] = {0};
    char ch[2] = {0};
    uint8 i = 0;
    uint8 len = 0;
    
    high = (dt_result >> 8) & 0x00FF;
    low = dt_result & 0x00FF;
    if(high >  0)
    {
        if(high == 2)
        {
            ch[0] = '1';
        }
        else
        {
            ch[0] = '0';
        }
        strcat(out_buf,ch);
    }
    else
    {
        for(i = 0; i < 6; i++)
        {
            temp = 0x01<< i;
            if((dt_result&temp) > 0)
            {
                ch[0] = 'A'+i;
                strcat(out_buf,ch);
            }
        }
    }
    len = SCI_STRLEN(out_buf);
    SCI_MEMCPY(pData,out_buf,len);
    pData[len] = 0;
    return len;
}

int MMIWLDT_HTTP_Rcv_Handle(uint8 * pContent,uint32 ContentLen)
{
    int ret = -1;
    char *msg_char;
    uint32 msg_len = 0;
    int val = 0;
    cJSON *json = PNULL;
    cJSON *item_json = PNULL;
    uint16 content_wstr[100] = {0};
    MMI_STRING_T alert_string = {0};
    uint32 ms = 1000;
    if(pContent == PNULL || ContentLen < 2)
    {
        return 0;
    }
    json=cJSON_Parse(pContent);
    if (json) 
    {
        item_json = cJSON_GetObjectItem(json,"code");
        if(item_json)
        {
            val = cJSON_GetObjectInt(item_json);
            WLDT_LOG("code = %d",val);
        }
        else
        {
            WLDT_LOG("code NONE");
        }
        item_json = cJSON_GetObjectItem(json,"msg");
        if(item_json)
        {
            msg_char = cJSON_GetObjectString(item_json);
            if(msg_char != PNULL)
            {
                msg_len = strlen(msg_char);
                WLDT_LOG("msg=%s",msg_char);
            }
        }
        else
        {
            WLDT_LOG("msg NONE");
        }
        if(val == 200)
        {
            #if 0
            if(msg_len > 0)
            {
                GUI_UTF8ToWstr(content_wstr, 100, msg_char, msg_len);
                alert_string.wstr_ptr =  content_wstr;
                alert_string.wstr_len = MMIAPICOM_Wstrlen(alert_string.wstr_ptr);
                MMIPUB_OpenAlertWinByTextPtr(&ms,&alert_string,PNULL,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            }
            else
            #endif
            {
                MMIPUB_OpenAlertWinByTextId(&ms,TXT_WLDT_SEND_OK,TXT_NULL,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            }
        }
        else
        {
            if(msg_len > 0)
            {
                GUI_UTF8ToWstr(content_wstr, 100, msg_char, msg_len);
                alert_string.wstr_ptr =  content_wstr;
                alert_string.wstr_len = MMIAPICOM_Wstrlen(alert_string.wstr_ptr);
                MMIPUB_OpenAlertWinByTextPtr(&ms,&alert_string,PNULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            }
            else
            {
                MMIPUB_OpenAlertWinByTextId(&ms,TXT_WLDT_SEND_FAIL,TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            }
        }
        cJSON_Delete(json);
    }
    else
    {
        MMIPUB_OpenAlertWinByTextId(&ms,TXT_WLDT_SEND_FAIL,TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        WLDT_LOG("MMIWLDT_HTTP_Send_CB ERR  cJSON_Parse");
    }
    return ret;
}

int MMIWLDT_HTTP_Send_CB(BOOLEAN is_ok,uint8 * pRcv,uint32 Rcv_len,uint32 err_id)
{
    int ret = -1;
    uint32 ms = 1000;
    WLDT_LOG("MMIWLDT_HTTP_Send_CB(%d) Rcv_len=%d,%s",is_ok,Rcv_len,pRcv);
    if(pRcv != NULL && Rcv_len > 2)
    {
        ret = MMIWLDT_HTTP_Rcv_Handle(pRcv,Rcv_len);
    }
    else
    {
        MMIPUB_OpenAlertWinByTextId(&ms,TXT_WLDT_SEND_FAIL,TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
    }
    return ret;
}

BOOLEAN  MMIWLDT_HTTP_Send(uint16 dt_result)
{
    BOOLEAN res = FALSE;
    cJSON *root = PNULL;
    cJSON *reqContent = PNULL;
    char * pContent = PNULL;
    char app_domain[100] = {0};
    char content_buf[1024] = {0};
    uint16 content_len = 0;
    uint32 ms = 1000;

    WLDT_LOG("MMIWLDT_HTTP_Send");
    if(FALSE == WLDT_SIM_Exsit())
    {
        WLDT_LOG("MMIWLDT_HTTP_Send Err NO SIM");
        MMIPUB_OpenAlertWinByTextId(&ms,TXT_WLDT_NO_SIM,TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        return FALSE;
    }
    if (MMIAPICONNECTION_isGPRSSwitchedOff())
    {
        MMIPUB_OpenAlertWinByTextId(&ms,TXT_WLDT_NO_NET,TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        return FALSE;
    }
    //加密
    reqContent=cJSON_CreateObject();
    if(reqContent)
    {
        cJSON_AddItemToObject(reqContent, "imei", cJSON_CreateString(g_wldt_phone_imei));
        content_len = WLDT_Func_ResultStr(dt_result,content_buf);
        cJSON_AddItemToObject(reqContent, "content", cJSON_CreateString(content_buf));
        content_len = WLDT_Func_GetDateTime(content_buf);
        cJSON_AddItemToObject(reqContent, "time", cJSON_CreateString(content_buf));
        pContent=cJSON_PrintUnformatted(reqContent);
        if(pContent)
        {
            content_len = strlen(pContent);
            //http://api.hdkt100.net:8088/hd/api/course/cardUploadData/v1
            sprintf(app_domain,"http://%s:%d/hd/api/course/cardUploadData/v1",yx_DB_Set_Rec.app_domain,yx_DB_Set_Rec.app_port);
            //sprintf(app_domain,"http://cardapi.hejiaoyu.com.cn/hd/api/course/cardUploadData/v1");
            res = MMIZDT_HTTP_AppSend(FALSE,app_domain,(uint8 *)pContent,content_len,1000,0,0,0,0,0,MMIWLDT_HTTP_Send_CB);
            SCI_FREE(pContent);
        }
        cJSON_Delete(reqContent);
    }
    if (res == FALSE)
    {
        MMIPUB_OpenAlertWinByTextId(&ms,TXT_WLDT_SEND_FAIL,TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        return FALSE;
    }
    return res;
}

#endif
