
#ifndef  _MMI_ZDT_NET_C  
#define _MMI_ZDT_NET_C  

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "zdt_app.h"

#define ZDT_NET_TRACE            SCI_TRACE_LOW

#define ZDT_GPRS_CHECK_TIMEOUT       5000  

#define ZDT_PDP_ACTIVE_TIMEOUT       30000  
#define ZDT_PDP_RETRY_TIMEOUT        2000 //重新尝试连接pdp，换卡连接

#define ZDT_PDP_DEFAULT_SIM_SYS MN_DUAL_SYS_1

#define ZDT_NET_FIX_IMEI

#define y_abs(x)  ((x) >= 0 ? (x) : (-(x)))

#if !defined(ZDT_GPS_SUPPORT)&&!defined(ZDT_W217_FACTORY_GPS) // no define
typedef struct GPS_DATA_t
{
    uint8 is_valid;
    uint32 Latitude;
    uint8 Lat_Dir;
    uint32 Longitude;
    uint8 Long_Dir;
    uint32 date;
    uint32 time;
    uint32 speed;  //运行速度
    uint16 hdop;	// 水平精度
    uint16 cog;     //方向角度
    uint8 sate_num;	// 卫星颗数
    uint32 altitude;    //海拔高度
    uint8 alt_Dir;          //海拔方向
    uint32 Lat_m; 
} GPS_DATA_T;
#endif
#ifdef ZDT_GPS_SUPPORT_AGPS
#ifdef WIN32
GPS_EPH_DATA_T g_gps_eph_data = {0};
BOOLEAN g_gps_eph_is_set = FALSE;
BOOLEAN g_gps_eph_set_end_pwoff = FALSE;
#endif
#endif

#ifdef MEID_WRITE_SUPPORT
char         g_zdt_phone_meid[ZDT_IMEI_LEN+1] = {0};
#endif
char         g_zdt_phone_imei_1[ZDT_IMEI_LEN+1] = {0};
char         g_zdt_phone_imei_2[ZDT_IMEI_LEN+1] = {0};
char*       g_zdt_phone_imei = g_zdt_phone_imei_1;
char         g_zdt_sim_imsi[ZDT_IMSI_LEN+1] = {0};
char         g_zdt_sim_iccid[ZDT_ICCID_LEN+1] = {0};
char         g_zdt_sim_number[ZDT_SIM_NUMBER_LEN+1] = {0};
uint32         g_zdt_net_cell_id = 0;
ZDT_RFID         g_zdt_phone_rfid = {0};

LOCAL MN_DUAL_SYS_E       s_zdt_net_set_sys   = ZDT_PDP_DEFAULT_SIM_SYS;
LOCAL MN_DUAL_SYS_E       s_zdt_net_cur_sys   = MN_DUAL_SYS_1;
LOCAL uint8                       s_zdt_net_pdp_timer_id      = 0;
LOCAL uint8                       s_zdt_net_pdp_retry_timer_id = 0;
LOCAL uint8                       s_zdt_net_gprs_check_timer_id      = 0;
LOCAL uint8                       s_zdt_net_reset_timer_id      = 0;
LOCAL uint32                      s_zdt_net_id            = 0;
LOCAL BOOLEAN                  s_zdt_net_pdp_state_is_ok        = FALSE;
LOCAL BOOLEAN                  s_zdt_net_pdp_real_status       = FALSE;
LOCAL BOOLEAN                  s_zdt_net_is_init        = FALSE;
LOCAL BOOLEAN                  s_zdt_net_need_init        = TRUE;
LOCAL BOOLEAN                  s_zdt_net_gprs_status[MN_DUAL_SYS_MAX+1] = {0};
LOCAL BOOLEAN                  s_zdt_net_cur_gprs_status = FALSE;
LOCAL BOOLEAN                  s_zdt_net_is_activing        = FALSE;
LOCAL BOOLEAN                  s_zdt_net_is_closing        = FALSE;

LOCAL BOOLEAN                  s_zdt_net_need_active_close        = FALSE;
LOCAL BOOLEAN                  s_zdt_net_need_close_active        = FALSE;

MMIZDT_S_STATION_DATA_T g_zdt_s_station_data = {0};
MMIZDT_N_STATION_DATA_T g_zdt_n_station_data = {0};
MMIZDT_STATION_DATA_T g_zdt_station_data = {0};

static uint32 last_reset_msg_time = 0;
static SCI_MUTEX_PTR g_yx_mutexPtr = NULL;
static uint32 zdt_net_reset_interval_time = 10;  //秒
static uint32 zdt_net_reset_interval_num = 0;
static uint32 zdt_net_reset_last_land_time_ms = 0;

LOCAL BOOLEAN  ZDT_Net_PDPTry(MN_DUAL_SYS_E sim_sys);
LOCAL BOOLEAN ZDT_Net_PdpDeactive(void);
LOCAL BOOLEAN ZDT_Net_PdpActive(void);
static void MMIZDT_Net_ResetExt(void);

uint16  MMIZDT_Net_GetIMEI(MN_DUAL_SYS_E dual_sys,char * imei_ptr)
{
	MN_IMEI_T imei;
	char tmp_buf1[20] = {0};
	uint8 i=0, j=0;

	MNNV_GetIMEIEx(dual_sys, imei);		

	for(i = 0, j = 0; i < 8; i++)
	{
		tmp_buf1[j++] = (imei[i] & 0x0F) + '0';
		tmp_buf1[j++] = ((imei[i] >> 4) & 0x0F) + '0';
	}

	for( i=0; i<ZDT_IMEI_LEN; i++ )
	{
		imei_ptr[i] = tmp_buf1[i+1];
	}
    
	imei_ptr[ZDT_IMEI_LEN] = 0x00;

	return ZDT_IMEI_LEN;
}

uint16  MMIZDT_Get_SIM_IMSI(MN_DUAL_SYS_E dual_sys,char * imsi_ptr)
{
	uint8 i=0, j=0;
	MN_IMSI_T imsi = {0};
       char imsi_buf[ZDT_IMSI_LEN+1] = {0};
       
	imsi = MNSIM_GetImsiEx(dual_sys);

	if(imsi.imsi_val == NULL )
		return 0;

	for(i = 0, j = 0; i < 8; i++)
	{
		imsi_buf[j++] = (imsi.imsi_val[i] & 0x0F) + '0';
		imsi_buf[j++] = ((imsi.imsi_val[i] >> 4) & 0x0F) + '0';
	}
    
	for( i=0; i<ZDT_IMSI_LEN; i++ )
	{
		imsi_ptr[i] = imsi_buf[i+1];
	}
	imsi_ptr[ZDT_IMSI_LEN] = 0x00;
	
	return ZDT_IMSI_LEN;
}

uint16  MMIZDT_Get_SIM_ICCID(MN_DUAL_SYS_E dual_sys,char * iccid_ptr)
{
    uint16 len = 0;
    ERR_MNDATAMAG_CODE_E err_code = 0;
    char iccid_buf[ZDT_ICCID_LEN+1] = {0};
    MNSIM_ICCID_T iccid_t = {0};

    err_code = MNSIM_GetICCIDEx(dual_sys,&iccid_t);
    ZDT_NET_TRACE("MMIZDT_Get_SIM_ICCID dual_sys =%d,err_code = %d,id_num=0x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",dual_sys,err_code,iccid_t.id_num[9],iccid_t.id_num[8],iccid_t.id_num[7],iccid_t.id_num[6],iccid_t.id_num[5],iccid_t.id_num[4],iccid_t.id_num[3],iccid_t.id_num[2],iccid_t.id_num[1],iccid_t.id_num[0]);
    //MMIAPICOM_BcdToStr(PACKED_LSB_FIRST, iccid_t.id_num, ZDT_ICCID_LEN, iccid_buf);
	MMIAPICOM_BcdICCIDToStr(iccid_t.id_num, ZDT_ICCID_LEN, iccid_buf);
    len = strlen((char *)iccid_buf);
    ZDT_NET_TRACE("MMIZDT_Get_SIM_ICCID STR = %s",iccid_buf);
    SCI_MEMCPY(iccid_ptr,iccid_buf,len);
    
    return len;
}

uint16  MMIZDT_Get_IMSI_Str(MN_IMSI_T imsi,char * imsi_ptr)
{
	uint8 i=0, j=0;
       char imsi_buf[ZDT_IMSI_LEN+1] = {0};
       
	for(i = 0, j = 0; i < 8; i++)
	{
		imsi_buf[j++] = (imsi.imsi_val[i] & 0x0F) + '0';
		imsi_buf[j++] = ((imsi.imsi_val[i] >> 4) & 0x0F) + '0';
	}
    
	for( i=0; i<ZDT_IMSI_LEN; i++ )
	{
		imsi_ptr[i] = imsi_buf[i+1];
	}
	imsi_ptr[ZDT_IMSI_LEN] = 0x00;
	
	return ZDT_IMSI_LEN;
}
/*****************************************************************************/
//  Discription: Get apn str 
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
/*****************************************************************************/
LOCAL char* ZDT_Net_GetApnStr(MN_DUAL_SYS_E dual_sys)
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
        ZDT_NET_TRACE("ZDT NET Get APN %s",apn_str);
    }
    else
    {
        ZDT_NET_TRACE("ZDT NET Get APN NULL");
    }
    return apn_str;
}

/*****************************************************************************/
//  Discription: Get apn str 
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
/*****************************************************************************/
LOCAL char* ZDT_Net_GetUserNameStr(MN_DUAL_SYS_E dual_sys)
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
        ZDT_NET_TRACE("ZDT NET Get UserName %s",user_name_str);
    }
    else
    {
        ZDT_NET_TRACE("ZDT NET Get UserName NULL");
    }
    return user_name_str;
    
}

/*****************************************************************************/
//  Discription: Get apn str 
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
/*****************************************************************************/
LOCAL char* ZDT_Net_GetPasswordStr(MN_DUAL_SYS_E dual_sys)
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
        ZDT_NET_TRACE("ZDT NET Get Password %s",password_str);
    }
    else
    {
        ZDT_NET_TRACE("ZDT NET Get Password NULL");
    }
    return password_str;
}

/*****************************************************************************/
//  Description : handle socket connect timeout
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL void ZDT_Net_HandlePdpActiveTimer(
                                            uint8 timer_id,
                                            uint32 param
                                            )
{
    ZDT_NET_TRACE("ZDT NET PdpActive ERR TimeOut");
    ZDT_Net_PdpDeactive();
    MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_NET_ACTIVE_FAIL,PNULL,0);
}

/*****************************************************************************/
//  Discription: Start socket connect timer
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
/*****************************************************************************/
LOCAL void ZDT_Net_StartPdpActiveTimer(void)
{
    if(0 != s_zdt_net_pdp_timer_id)
    {
        MMK_StopTimer(s_zdt_net_pdp_timer_id);
        s_zdt_net_pdp_timer_id = 0;
    }
    
    s_zdt_net_pdp_timer_id = MMK_CreateTimerCallback(ZDT_PDP_ACTIVE_TIMEOUT, 
                                                                        ZDT_Net_HandlePdpActiveTimer, 
                                                                        PNULL, 
                                                                        FALSE);
}
/*****************************************************************************/
//  Discription: Start socket connect timer
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
/*****************************************************************************/
LOCAL void ZDT_Net_StopPdpActiveTimer(void)
{
    if(0 != s_zdt_net_pdp_timer_id)
    {
        MMK_StopTimer(s_zdt_net_pdp_timer_id);
        s_zdt_net_pdp_timer_id = 0;
    }
}

LOCAL void ZDT_Net_HandlePdpRetryTimer(
                                uint8 timer_id,
                                uint32 param
                                )
{
    MN_DUAL_SYS_E sys_sim = 0;
    BOOLEAN       sim_ok =  FALSE;

    if(timer_id == s_zdt_net_pdp_retry_timer_id && 0 != s_zdt_net_pdp_retry_timer_id)
    {
        MMK_StopTimer(s_zdt_net_pdp_retry_timer_id);
        s_zdt_net_pdp_retry_timer_id = 0;
    }
    s_zdt_net_cur_sys++;
    if(s_zdt_net_cur_sys < MMI_DUAL_SYS_MAX)
    {
        for(sys_sim = s_zdt_net_cur_sys; sys_sim < MMI_DUAL_SYS_MAX; sys_sim++)
        {
            if(MMIPHONE_IsSimOk(sys_sim))
            {
                s_zdt_net_cur_sys = sys_sim;
                sim_ok = TRUE;
                break;
            }
        }
        if(sim_ok)
        {
            if(!ZDT_Net_PDPTry(s_zdt_net_cur_sys))
            {
                ZDT_NET_TRACE("ZDT NET PdpActive Retry ERR cur_sys(%d),",s_zdt_net_cur_sys);
        	    s_zdt_net_pdp_retry_timer_id = MMK_CreateTimerCallback(
                                                                                                ZDT_PDP_RETRY_TIMEOUT, 
                                                                                                ZDT_Net_HandlePdpRetryTimer, 
                                                                                                PNULL, 
                                                                                                FALSE
                                                                                                );
            }
        }
        else
        {
            ZDT_NET_TRACE("ZDT NET PdpActive Retry ERR NO SIM");
            MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_NET_ACTIVE_FAIL,PNULL,0);
        }
    }
    else//找不到卡了
    {
        ZDT_NET_TRACE("ZDT NET PdpActive Retry ERR no_sys");
        MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_NET_ACTIVE_FAIL,PNULL,0);
    }
}

/*****************************************************************************
//  Description : handle pdp msg 
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
*****************************************************************************/
LOCAL void ZDT_Net_HandlePdpMsg(MMIPDP_CNF_INFO_T *msg_ptr)
{
    if(PNULL == msg_ptr)
    {
        return;
    }
    
    ZDT_NET_TRACE("ZDT NET HandlePdpMsg msg_id=%d,result=%d",msg_ptr->msg_id,msg_ptr->result);
    
    switch(msg_ptr->msg_id) 
    {
    case MMIPDP_ACTIVE_CNF:     //PDP激活成功
        ZDT_Net_StopPdpActiveTimer();
        
        if(MMIPDP_RESULT_SUCC == msg_ptr->result)
        {
            s_zdt_net_id = msg_ptr->nsapi;
            s_zdt_net_pdp_real_status = TRUE;
            ZDT_NET_TRACE("ZDT NET MMIPDP_ACTIVE_CNF OK net_id=0x%x",s_zdt_net_id);
            MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_NET_ACTIVE_SUCCESS,PNULL,0);
        }
        else if (MMIPDP_RESULT_FAIL == msg_ptr->result)
        {
            ZDT_NET_TRACE("ZDT NET MMIPDP_ACTIVE_CNF FAIL");
            ZDT_Net_PdpDeactive();
            s_zdt_net_pdp_real_status = FALSE;
            MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_NET_ACTIVE_FAIL,PNULL,0);
        }
        else 
        {
            ZDT_NET_TRACE("ZDT NET MMIPDP_ACTIVE_CNF FAIL Other");
            ZDT_Net_PdpDeactive();
            s_zdt_net_pdp_real_status = FALSE;
            MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_NET_ACTIVE_FAIL,PNULL,0);
        }        
        break;
        
    case MMIPDP_DEACTIVE_CNF:   //PDP去激活成功。
            ZDT_NET_TRACE("ZDT NET MMIPDP_DEACTIVE_CNF");
            s_zdt_net_id = 0;
            s_zdt_net_pdp_real_status = FALSE;
            MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_NET_CONNET_FAIL,PNULL,0);
        break;
        
    case MMIPDP_DEACTIVE_IND:   //PDP被网络端去激活。
            ZDT_NET_TRACE("ZDT NET MMIPDP_DEACTIVE_IND");
            s_zdt_net_id = 0;
            ZDT_Net_PdpDeactive();
            s_zdt_net_pdp_real_status = FALSE;
            MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_NET_CONNET_FAIL,PNULL,0);
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
LOCAL BOOLEAN  ZDT_Net_PDPTry(MN_DUAL_SYS_E sim_sys)
{
#if defined(MMI_GPRS_SUPPORT)
    BOOLEAN                 return_val      = FALSE;
    MMIPDP_ACTIVE_INFO_T    active_info     = {0};
    MMICONNECTION_APNTYPEINDEX_T *apn_idx_arr;
    MMICONNECTION_LINKSETTING_DETAIL_T* linksetting = PNULL;

    ZDT_NET_TRACE("ZDT NET ConnectPDP");
    if(MMIAPISET_GetFlyMode())
    {
        ZDT_NET_TRACE("ZDT NET ConnectPDP [Fly Mode]");
        return FALSE;
    }
    if (FALSE == MMIZDT_Net_GPRS_IsOn())
    {
        ZDT_NET_TRACE("ZDT NET ConnectPDP [DATA ERROR]");
        return FALSE;
    }
    if (FALSE == MMIAPIPHONE_GetDataServiceSIM(&sim_sys))
    {
        ZDT_NET_TRACE("ZDT NET ConnectPDP [SIM ERROR]");
        return FALSE;
    }
    if(sim_sys != s_zdt_net_set_sys)
    {
        MMIZDT_Net_SetActiveSys(sim_sys);
    }
    active_info.app_handler         = MMI_MODULE_ZDT;
    active_info.dual_sys            = sim_sys;
    active_info.apn_ptr             = NULL;
    active_info.user_name_ptr       = NULL;
    active_info.psw_ptr             = NULL;
    active_info.priority            = 3;
    active_info.ps_service_rat      = MN_TD_PREFER;
    active_info.ps_interface        = MMIPDP_INTERFACE_GPRS;
    active_info.handle_msg_callback = ZDT_Net_HandlePdpMsg;
    active_info.ps_service_type     = BROWSER_E;
    active_info.storage             = MN_GPRS_STORAGE_ALL;

    apn_idx_arr = MMIAPICONNECTION_GetApnTypeIndex(sim_sys);
    if (apn_idx_arr == NULL)
    {
        ZDT_NET_TRACE("ZDT NET ConnectPDP [APN IDX ERROR]");
        return FALSE;
    }
    ZDT_NET_TRACE("ZDT NET  ConnectPDP [sim = %d, Net Setting = %d]", sim_sys, apn_idx_arr->internet_index[sim_sys].index);
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
            ZDT_NET_TRACE("ZDT NET PdpActive GPRS sim_sys=%d",sim_sys);
            ZDT_Net_StartPdpActiveTimer();
            return_val = TRUE;
        }
    }
    return return_val;
#endif
    return FALSE;
}
#else
LOCAL BOOLEAN  ZDT_Net_PDPTry(MN_DUAL_SYS_E sim_sys)
{
    MMIPDP_ACTIVE_INFO_T    active_info     = {0};
    BOOLEAN                 return_val      = FALSE;
    
    active_info.app_handler         = MMI_MODULE_ZDT;
    active_info.dual_sys            = sim_sys;
    active_info.apn_ptr             = ZDT_Net_GetApnStr(sim_sys);
    active_info.user_name_ptr       = ZDT_Net_GetUserNameStr(sim_sys);
    active_info.psw_ptr             = ZDT_Net_GetPasswordStr(sim_sys);
    active_info.priority            = 3;
    active_info.ps_service_rat      = MN_UNSPECIFIED;
    active_info.ps_interface        = MMIPDP_INTERFACE_GPRS;
    active_info.handle_msg_callback = ZDT_Net_HandlePdpMsg;
    active_info.ps_service_type = BROWSER_E;
    active_info.storage = MN_GPRS_STORAGE_ALL;

    if(MMIAPIPDP_Active(&active_info))
    {
        ZDT_NET_TRACE("ZDT NET PdpActive GPRS sim_sys=%d",sim_sys);
        ZDT_Net_StartPdpActiveTimer();
        return_val = TRUE;
    }
    return return_val;
}
#endif

LOCAL BOOLEAN ZDT_Net_PdpActive(void)
{
    BOOLEAN                 return_val      = FALSE;
    MN_DUAL_SYS_E           dual_sys        = MN_DUAL_SYS_MAX;
    MMIPDP_ACTIVE_INFO_T    active_info     = {0};
    
    ZDT_NET_TRACE("ZDT NET PdpActive Start is_activing=%d,is_ok=%d",s_zdt_net_is_activing,s_zdt_net_pdp_state_is_ok);
    if(s_zdt_net_is_activing)
    {
        s_zdt_net_need_active_close = FALSE;
        return TRUE;
    }
    s_zdt_net_is_activing = TRUE;
    
    if(s_zdt_net_pdp_state_is_ok || s_zdt_net_pdp_real_status)
    {
        MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_NET_ACTIVE_SUCCESS,PNULL,0);
        return TRUE;
    }

#if 0 //def WIFI_SUPPORT    
    //优先wifi
    if(s_zdt_net_set_sys == MN_DUAL_SYS_MAX && MMIWIFI_STATUS_CONNECTED == MMIAPIWIFI_GetStatus())
    {
        active_info.app_handler         = MMI_MODULE_ZDT;
        active_info.handle_msg_callback = ZDT_Net_HandlePdpMsg;
        active_info.ps_interface        = MMIPDP_INTERFACE_WIFI;
        if(MMIAPIPDP_Active(&active_info))
        {
            ZDT_NET_TRACE("ZDT NET PdpActive OK WIFI");
            ZDT_Net_StartPdpActiveTimer();
            return_val = TRUE;
        }
        else
        {
            ZDT_NET_TRACE("ZDT NET PdpActive ERR WIFI");
            MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_NET_ACTIVE_FAIL,PNULL,0);
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
            ZDT_NET_TRACE("ZDT NET PdpActive ERR NO SIM");
            MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_NET_ACTIVE_FAIL,PNULL,0);
            return FALSE;
        }
        
        if(s_zdt_net_is_init == FALSE)
        {
            ZDT_NET_TRACE("ZDT NET PdpActive ERR NO INIT");
            MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_NET_ACTIVE_FAIL,PNULL,0);
            return FALSE;
        }

        if(s_zdt_net_set_sys < MN_DUAL_SYS_MAX)
        {
            if(MMIAPIPHONE_IsSimAvailable(s_zdt_net_set_sys))
            {
                s_zdt_net_cur_sys = s_zdt_net_set_sys;
            }
            else
            {
                ZDT_NET_TRACE("ZDT NET PdpActive ERR set_sys(%d) NO SIM",s_zdt_net_set_sys);
                MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_NET_ACTIVE_FAIL,PNULL,0);
                return FALSE;
            }
        }
        else
        {
            s_zdt_net_cur_sys = sim_ok;
        }

        return_val = ZDT_Net_PDPTry(s_zdt_net_cur_sys);//第一张有效卡
        if(FALSE == return_val)
        {
            ZDT_NET_TRACE("ZDT NET PdpActive ERR cur_sys(%d/%d),",s_zdt_net_cur_sys,sim_ok_num);
            #if 0
            if(sim_ok_num > 1 && s_zdt_net_set_sys == MN_DUAL_SYS_MAX)
            {
                if(0 != s_zdt_net_pdp_retry_timer_id)
                {
                    MMK_StopTimer(s_zdt_net_pdp_retry_timer_id);
                    s_zdt_net_pdp_retry_timer_id = 0;
                }
                s_zdt_net_pdp_retry_timer_id = MMK_CreateTimerCallback(
                                                                                                ZDT_PDP_RETRY_TIMEOUT, 
                                                                                                ZDT_Net_HandlePdpRetryTimer, 
                                                                                                PNULL, 
                                                                                                FALSE
                                                                                                );
            }
            else
            #endif
            {
                MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_NET_ACTIVE_FAIL,PNULL,0);
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
LOCAL BOOLEAN ZDT_Net_PdpDeactive(void)
{
    BOOLEAN return_val = FALSE;
    
    return_val = MMIAPIPDP_Deactive(MMI_MODULE_ZDT);

    return return_val;
}
PUBLIC BOOLEAN ZDT_Net_IsAllow(void)
{
    if(MMIAPISET_GetFlyMode())
    {
        ZDT_NET_TRACE("ZDT_Net_IsAllow FASE In Fly");
        return FALSE;
    }
    if(MMIZDT_Net_GPRS_IsOn() == FALSE)
    {
        ZDT_NET_TRACE("ZDT_Net_IsAllow FASE  GPRS OFF");
        return FALSE;
    }
#ifdef ZDT_TCP_SUPPORT
#ifdef ZDT_PLAT_YX_SUPPORT
    if(YX_Net_IsAllow(&g_yx_app))
    {
        return TRUE;
    }
#endif
#ifdef ZDT_PLAT_HYHL_SUPPORT
    if(HYHL_Net_IsAllow(&g_hyhl_app))
    {
        return TRUE;
    }
#endif
#endif
    return FALSE;

}
LOCAL BOOLEAN ZDT_Net_Init(void)
{
    MN_DUAL_SYS_E dual_sys = MMIZDT_Net_GetActiveSys();
    ZDT_NET_TRACE("ZDT_Net_Init is_init=%d, dual_sys=%d",s_zdt_net_is_init,dual_sys);
    #if (defined(ZDT_WIFI_SUPPORT)||defined(ZDT_PLAT_YX_SUPPORT_ZTE)||defined(ZDT_W217_FACTORY_WIFI))//by bao .  add by wuxx_20231219 , // 4==2S, 6==3S // here and  modem_nv_UWS6121E_static.nvm
#ifndef WIN32
    MNPHONE_SetRRCRel(4,MN_DUAL_SYS_1); // 4 改为1节省电流 by bao  //LTE RRC层链路释放时间设置，每级500ms // 因 LTE,GPS,WIFI 天线共用, 可加快 WIFI 扫描结果取得
#endif
    #endif
    
    if(s_zdt_net_is_init == FALSE)
    {
        ZDT_NET_TRACE("ZDT_Net_Init imei=%s,imsi=%s,iccid=%s",g_zdt_phone_imei,g_zdt_sim_imsi,g_zdt_sim_iccid);
#ifdef FOTA_SUPPORT_CMIOT
        //MMIZDT_StartFirstFotaVerTimer();
#endif
	if(g_yx_mutexPtr == NULL) {
		g_yx_mutexPtr = SCI_CreateMutex("zdtnet-mutex", SCI_INHERIT);
	}
#ifdef MMI_GPRS_SUPPORT
        //MMIAPICONNECTION_SetGPRSStatus(MMICONNECTION_GPRS_ALWAYS); //set GPRS alwarys connection
#endif

#ifdef ZDT_UDP_SUPPORT
        MMIZDT_UDP_Init();
#endif
#ifdef ZDT_TCP_SUPPORT
        ZdtTCP_NetInit();
#endif

#ifdef ZDT_PLAT_SHB_SUPPORT
        SHB_Net_Init();
#endif
#ifdef ZDT_GPS_SUPPORT_AGPS
        AGPS_TCP_Init();
#ifndef WIN32
        if(ZDT_GPS_Hw_IsOn())
        {
            if(gps_hwic_type_is_td())
            {
                TD_AGPS_TCP_Send();
            }
            else if(gps_hwic_type_is_ubx())
            {
                UBX_AGPS_TCP_Send();
            }
            else if(gps_hwic_type_is_casic())
            {
                CASIC_AGPS_TCP_Send();
            }
            else if(gps_hwic_type_is_allystar())
            {
                ALLYSTAR_AGPS_TCP_Send();
            }
        }
#endif
#endif

        if(ZDT_Net_IsAllow())
        {
            MMIZDT_Net_Open();
        }
#ifdef ZDT_ZFB_SUPPORT
        MMIZFB_Net_Init();
#endif

        s_zdt_net_is_init = TRUE;
    }
    return TRUE;
}

LOCAL BOOLEAN ZDT_Net_Open(void)
{
    ZDT_NET_TRACE("ZDT_Net_Open state=%d,is_activing=%d,is_closing=%d",s_zdt_net_pdp_state_is_ok,s_zdt_net_is_activing,s_zdt_net_is_closing);
    #ifdef ZDT_PLAT_SHB_SUPPORT
        SHB_Net_Open(&g_shb_app);
    #endif
        if(s_zdt_net_is_closing)
        {
            s_zdt_net_need_close_active = TRUE;
        }
        else
        {
            ZDT_Net_PdpActive();
        }
    return TRUE;
}

LOCAL BOOLEAN ZDT_Net_CloseEx(void)
{
    if(s_zdt_net_is_closing)
    {
        s_zdt_net_need_close_active = FALSE;
        return TRUE;
    }
    s_zdt_net_is_closing = TRUE;
#ifdef ZDT_TCP_SUPPORT
    ZdtTCP_Net_UnReg(g_yx_app.m_yx_tcp_reg_interface);
    SCI_Sleep(200);
#endif
    if(s_zdt_net_pdp_state_is_ok)
    {
        //if(FALSE == ZDT_Net_PdpDeactive()) //delete by bao no deactive
        {
            MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_NET_CONNET_FAIL,PNULL,0);
        }
    }
    else
    {
        //ZDT_Net_PdpDeactive();
        MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_NET_CONNET_FAIL,PNULL,0);
    }
}

LOCAL BOOLEAN ZDT_Net_Close(void)
{
    ZDT_NET_TRACE("ZDT_Net_Close state=%d,is_activing=%d,is_closing=%d",s_zdt_net_pdp_state_is_ok,s_zdt_net_is_activing,s_zdt_net_is_closing);
    #ifdef ZDT_PLAT_SHB_SUPPORT
        SHB_Net_Close(&g_shb_app);
    #endif
        if(s_zdt_net_is_activing)
        {
            s_zdt_net_need_active_close = TRUE;
        }
        else
        {
            ZDT_Net_CloseEx();
        }
    return TRUE;
}


LOCAL BOOLEAN ZDT_Net_PdpStateChange(BOOLEAN is_ok)
{
    ZDT_NET_TRACE("ZDT NET PDPStateChange %d",is_ok);
    if(is_ok)
    {
        if(s_zdt_net_need_active_close == FALSE)
        {
#ifdef ZDT_TCP_SUPPORT
            ZdtTCP_NetReset();
#endif
        }
    }

    return TRUE;
}

LOCAL void ZDT_Net_PdpStateUpdate(BOOLEAN is_ok,BOOLEAN is_active)
{
    ZDT_NET_TRACE("ZDT_Net_PdpStateUpdate is_ok=%d,is_active=%d,activing=%d,closing=%d",is_ok,is_active,s_zdt_net_is_activing,s_zdt_net_is_closing);
    if(s_zdt_net_pdp_state_is_ok != is_ok)
    {
        s_zdt_net_pdp_state_is_ok = is_ok;
        ZDT_Net_PdpStateChange(is_ok);
    }
    
    if(is_active)
    {
        if(s_zdt_net_is_activing)
        {
            s_zdt_net_is_activing = FALSE;
            if(s_zdt_net_need_active_close)
            {
                ZDT_NET_TRACE("ZDT_Net_PdpStateUpdate Active Close");
                s_zdt_net_need_active_close = FALSE;
                MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_NET_CLOSE,PNULL,0);
            }
        }
    }
    else
    {   
        s_zdt_net_is_activing = FALSE;
        if(s_zdt_net_is_closing)
        {
            s_zdt_net_is_closing = FALSE;
            if(s_zdt_net_need_close_active)
            {
                ZDT_NET_TRACE("ZDT_Net_PdpStateUpdate Close Active");
                s_zdt_net_need_close_active = FALSE;
                MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_NET_OPEN,PNULL,0);
            }
        }
    }
    return;
}

PUBLIC uint32 MMIZDT_Net_GetNetID(void)
{
    return s_zdt_net_id;
}

PUBLIC BOOLEAN MMIZDT_Net_PdpStateGet(void)
{
    return s_zdt_net_pdp_state_is_ok;
}

PUBLIC BOOLEAN MMIZDT_Net_IsActiving(void)
{
    return s_zdt_net_is_activing;
}

PUBLIC BOOLEAN MMIZDT_Net_IsClosing(void)
{
    return s_zdt_net_is_closing;
}


PUBLIC BOOLEAN MMIZDT_Net_IsInit(void)
{
    return s_zdt_net_is_init;
}

PUBLIC BOOLEAN MMIZDT_Net_Init(void)
{
    MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_NET_INIT,PNULL,0);
    return TRUE;
}

LOCAL void ZDT_Net_HandleResetTimer(
                                            uint8 timer_id,
                                            uint32 param
                                            )
{
    ZDT_NET_TRACE("ZDT_Net_HandleResetTimer");
    if(g_yx_mutexPtr != NULL)
    {
        SCI_GetMutex(g_yx_mutexPtr, SCI_WAIT_FOREVER);
    }
    MMIZDT_Net_ResetExt();
    if(g_yx_mutexPtr != NULL)
    {
        SCI_PutMutex(g_yx_mutexPtr);
    }
}

LOCAL void ZDT_Net_StartResetTimer(uint32 time_ms)
{
    if(0 != s_zdt_net_reset_timer_id)
    {
        return;
    }
    s_zdt_net_reset_timer_id = MMK_CreateTimerCallback(zdt_net_reset_interval_time*1000, 
                                                                        ZDT_Net_HandleResetTimer, 
                                                                        PNULL, 
                                                                        FALSE);
}

LOCAL void ZDT_Net_StopResetTimer(void)
{
    if(0 != s_zdt_net_reset_timer_id)
    {
        MMK_StopTimer(s_zdt_net_reset_timer_id);
        s_zdt_net_reset_timer_id = 0;
    }
}

static void MMIZDT_Net_ResetExt(void)
{
    uint32 cur_time = ZDT_Get_TimeSecond();
    uint32 offset_time_ms = 0;
    offset_time_ms = (y_abs(cur_time - zdt_net_reset_last_land_time_ms));
    
    ZDT_Net_StopResetTimer();
    if(offset_time_ms > 3600 && 0 == MNPHONE_GetAttState(MMIZDT_Net_GetActiveSys()))
    {
        MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_NET_RESET,PNULL,0);
    }
    else
    {
        MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_NET_RESET,PNULL,0);
    }
    last_reset_msg_time = ZDT_Get_TimeSecond();
}

PUBLIC void MMIZDT_Net_Reset_Restore(void)
{
    //网络登陆成功后复位参数
    if(g_yx_mutexPtr != NULL)
    {
        SCI_GetMutex(g_yx_mutexPtr, SCI_WAIT_FOREVER);
    }
    ZDT_Net_StopResetTimer();
    zdt_net_reset_interval_time = 10;
    zdt_net_reset_interval_num = 0;
    zdt_net_reset_last_land_time_ms = ZDT_Get_TimeSecond();
    if(g_yx_mutexPtr != NULL)
    {
        SCI_PutMutex(g_yx_mutexPtr);
    }
    return;
}

PUBLIC BOOLEAN MMIZDT_Net_ResetDelay(void)
{
    zdt_net_reset_last_land_time_ms = ZDT_Get_TimeSecond();
    MMIZDT_Net_Reset(FALSE);
    return TRUE;
}

void MMI_Send_Heart(void)
{
	MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_YX_HEART,NULL,0);
	return;
}


static void MMIZDT_Net_Reset_CheckMode(void)
{
    uint32 cur_time = ZDT_Get_TimeSecond();
    uint32 offset_time_ms = 0;
    offset_time_ms = (y_abs(cur_time - zdt_net_reset_last_land_time_ms));
    
    if(offset_time_ms < 600)
    {
        //    10分钟内没有登陆每30 秒复位一次
        zdt_net_reset_interval_time = 30;
    }
    else if(offset_time_ms < 3600)
    {
        //    1小时内没有登陆每5分钟复位一次
        zdt_net_reset_interval_time = 300;
    }
    else
    {
        //其它每三十分钟一次
        zdt_net_reset_interval_time = 1800;
    }
    return;
}

PUBLIC BOOLEAN MMIZDT_Net_Reset(BOOLEAN is_now)
{
    uint32 cur_time = ZDT_Get_TimeSecond();
    uint32 offset_times_ms = 0;
    if(FALSE == MMIZDT_Net_GPRS_IsOn() || MMIAPISET_GetFlyMode() || s_zdt_net_is_init == FALSE)
    {
        ZDT_LOG("MMIZDT_Net_Reset In Fly");
        return FALSE;
    }
    
    if(g_yx_mutexPtr != NULL)
    {
        SCI_GetMutex(g_yx_mutexPtr, SCI_WAIT_FOREVER);
    }
    offset_times_ms = y_abs(cur_time - last_reset_msg_time);
    MMIZDT_Net_Reset_CheckMode();
    ZDT_LOG("MMIZDT_Net_Reset cur_time=%d,%d,interval=%d",cur_time,last_reset_msg_time,zdt_net_reset_interval_time);
    if(is_now == FALSE && offset_times_ms < zdt_net_reset_interval_time)
    {
        ZDT_LOG("MMIZDT_Net_Reset reset too fast");
        ZDT_Net_StartResetTimer(offset_times_ms);
    }
    else
    {
        MMIZDT_Net_ResetExt();
    }
    
    if(g_yx_mutexPtr != NULL)
    {
        SCI_PutMutex(g_yx_mutexPtr);
    }
    return TRUE;
}

PUBLIC BOOLEAN MMIZDT_Net_Reset_Check(void)
{
    if(s_zdt_net_is_init && s_zdt_net_id == 0)
    {
        ZDT_LOG("MMIZDT_Net_Reset_Check");
        MMIZDT_Net_Reset(FALSE);
    }
    
    return TRUE;
}
PUBLIC BOOLEAN MMIZDT_Net_Open(void)
{
    MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_NET_OPEN,PNULL,0);
    return TRUE;
}

PUBLIC BOOLEAN MMIZDT_Net_Close(void)
{
    MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_NET_CLOSE,PNULL,0);
    return TRUE;
}

LOCAL MN_DUAL_SYS_E MMIZDT_Net_NV_GetActiveSys(void)
{
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    uint8 net_sys = ZDT_PDP_DEFAULT_SIM_SYS;
    //get if is flip answer
    MMINV_READ(MMI_ZDT_NV_NET_SYS,&net_sys,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        //自动识别
        net_sys = ZDT_PDP_DEFAULT_SIM_SYS;
        MMINV_WRITE(MMI_ZDT_NV_NET_SYS,&net_sys);
    }
    s_zdt_net_set_sys = net_sys;
    return net_sys;
}

LOCAL BOOLEAN  MMIZDT_Net_NV_SetActiveSys(MN_DUAL_SYS_E set_sys) 
{
    uint8 net_sys = set_sys;
    if(s_zdt_net_set_sys != set_sys)
    {
        MMINV_WRITE(MMI_ZDT_NV_NET_SYS,&net_sys);
        s_zdt_net_set_sys = set_sys;
        return TRUE;
    }
    return FALSE;
}

PUBLIC MN_DUAL_SYS_E MMIZDT_Net_GetActiveSys(void)
{
    return s_zdt_net_set_sys;
}

PUBLIC void MMIZDT_Net_SetActiveSys(MN_DUAL_SYS_E set_sys) 
{
    ZDT_NET_TRACE("MMIZDT_Net_SetActiveSys set_sys=%d",set_sys);
    SCI_MEMSET(g_zdt_sim_imsi,0,ZDT_IMSI_LEN+1);
    SCI_MEMSET(g_zdt_sim_iccid,0,ZDT_ICCID_LEN+1);
    MMIZDT_Net_NV_SetActiveSys(set_sys);
    MMIZDT_Get_SIM_IMSI(set_sys,g_zdt_sim_imsi);
    MMIZDT_Get_SIM_ICCID(set_sys,g_zdt_sim_iccid);
    #ifdef ZDT_NET_FIX_IMEI
        g_zdt_phone_imei = g_zdt_phone_imei_1;
    #else
    if(set_sys == MN_DUAL_SYS_1)
    {
        g_zdt_phone_imei = g_zdt_phone_imei_1;
    }
    else
    {
        g_zdt_phone_imei = g_zdt_phone_imei_2;
    }
    #endif
    return;
}

LOCAL BOOLEAN  MMIZDT_Net_Handle_Init(DPARAM param)
{
    ZDT_NET_TRACE("MMIZDT_Net_Handle_Init");
    ZDT_Net_Init();
    return TRUE;
}

LOCAL BOOLEAN  MMIZDT_Net_Handle_Open(DPARAM param)
{
    ZDT_NET_TRACE("MMIZDT_Net_Handle_Open");
    ZDT_Net_Open();
    return TRUE;
}

LOCAL BOOLEAN  MMIZDT_Net_Handle_Close(DPARAM param)
{
    ZDT_NET_TRACE("MMIZDT_Net_Handle_Close");
    ZDT_Net_Close();
    return TRUE;
}

LOCAL BOOLEAN  MMIZDT_Net_Handle_Reset(DPARAM param)
{
    ZDT_NET_TRACE("MMIZDT_Net_Handle_Reset");
    MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_NET_CLOSE,PNULL,0);
    MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_NET_OPEN,PNULL,0);
    return TRUE;
}

LOCAL BOOLEAN  MMIZDT_Net_Handle_PDPActiveOK(DPARAM param)
{
    ZDT_NET_TRACE("MMIZDT_Net_Handle_PDPActiveOK net_is_activing=%d",s_zdt_net_is_activing);
    ZDT_Net_PdpStateUpdate(TRUE,TRUE);
#ifdef ZDT_UDP_SUPPORT
    MMIZDT_UDP_Handle_PDPActiveOK();
#endif
#ifdef ZDT_TCP_SUPPORT
    MMIZDT_TCP_Handle_PDPActiveOK();
#endif
    return TRUE;
}

LOCAL BOOLEAN  MMIZDT_Net_Handle_PDPActiveFail(DPARAM param)
{
    ZDT_NET_TRACE("MMIZDT_Net_Handle_PDPActiveFail net_is_activing=%d",s_zdt_net_is_activing);
    ZDT_Net_PdpStateUpdate(FALSE,TRUE);
#ifdef ZDT_UDP_SUPPORT
    MMIZDT_UDP_Handle_PDPActiveFail();
#endif
#ifdef ZDT_TCP_SUPPORT
    MMIZDT_TCP_Handle_PDPActiveFail();
#endif
    return TRUE;
}

LOCAL BOOLEAN  MMIZDT_Net_Handle_PDPRespond(DPARAM param)
{
    ZDT_NET_TRACE("ZDT NET Handle_PDP_RSP net_is_activing=%d",s_zdt_net_is_activing);
    ZDT_Net_PdpStateUpdate(FALSE,FALSE);
#ifdef ZDT_UDP_SUPPORT
    MMIZDT_UDP_Handle_PDPRespond();
#endif
#ifdef ZDT_TCP_SUPPORT
    MMIZDT_TCP_Handle_PDPRespond();
#endif
    if(s_zdt_net_cur_gprs_status && ZDT_Net_IsAllow())
    {
        //MMIZDT_Net_Reset(FALSE);
    }
    return TRUE;
}


PUBLIC MMI_RESULT_E MMIZDT_Net_Handle_AppMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
    MMI_RESULT_E res = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case ZDT_APP_SIGNAL_NET_INIT:
            MMIZDT_Net_Handle_Init(param);
        break;
        
    case ZDT_APP_SIGNAL_NET_OPEN:
            MMIZDT_Net_Handle_Open(param);
        break;
        
    case ZDT_APP_SIGNAL_NET_CLOSE:
            MMIZDT_Net_Handle_Close(param);
        break;
        
    case ZDT_APP_SIGNAL_NET_RESET:
            MMIZDT_Net_Handle_Reset(param);
        break;
        
    case ZDT_APP_SIGNAL_NET_ACTIVE_SUCCESS:
            MMIZDT_Net_Handle_PDPActiveOK(param);
        break;
        
    case ZDT_APP_SIGNAL_NET_ACTIVE_FAIL:
            MMIZDT_Net_Handle_PDPActiveFail(param);
        break;
        
    case ZDT_APP_SIGNAL_NET_CONNET_FAIL:
            MMIZDT_Net_Handle_PDPRespond(param);
        break;
    
    default:
            res = MMI_RESULT_FALSE;
        break;
    }

    if(res == MMI_RESULT_FALSE)
    {
        #ifdef ZDT_UDP_SUPPORT
            res = MMIZDT_UDP_Handle_AppMsg(app_ptr,msg_id,param);
        #endif
    }
    if(res == MMI_RESULT_FALSE)
    {
        #ifdef ZDT_TCP_SUPPORT
            res = MMIZDT_TCP_Handle_AppMsg(app_ptr,msg_id,param);
        #endif
    }
    if(res == MMI_RESULT_FALSE)
    {
        #ifdef ZDT_PLAT_HYHL_SUPPORT
            res = HYHL_Handle_AppMsg(app_ptr,msg_id,param);
        #endif
    }
    if(res == MMI_RESULT_FALSE)
    {
        #ifdef ZDT_PLAT_YX_SUPPORT
            res = YX_Handle_AppMsg(app_ptr,msg_id,param);
        #endif
    }
    return res;
}

void MMIZDT_Station_Trace(const char *  buf,MMIZDT_STATION_DATA_T * station_ptr)
{
    #if 0
    char msg[600] = {0};
    char* pMsg = msg;
    uint16 ext_addr = 0;
    uint8 i = 0;
    uint8 j = 0;

   if(buf != NULL)
   {
       ext_addr = strlen(buf);
       strcpy(msg,buf);
       pMsg = msg+ext_addr;
   }

    sprintf(pMsg, "Station:%d,%d,%02d,%d,%d;", 
                            station_ptr->s_cell.is_valid,
                            station_ptr->s_cell.mcc,
                            station_ptr->s_cell.mnc,
                            station_ptr->s_cell.lac,
                            station_ptr->s_cell.cell_id
                            );

    for (i = 0; i < ZDT_N_STATION_NUM; i++)		// this is for example for simulator
    {
        if (station_ptr->n_cell[i].is_valid)	// this channel is served
        {
            ext_addr = strlen(msg);
            pMsg = msg+ext_addr;
            sprintf(pMsg, "%d;", station_ptr->n_cell[i].cell_id);
        }
    }
   
   ext_addr = strlen(msg);

    #endif
    
}

// 0: 相同
LOCAL uint8  MMIZDT_Station_IsSame(MMIZDT_STATION_DATA_T  * station1_ptr,MMIZDT_STATION_DATA_T  * station2_ptr,int num)
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
            for(i = 0; i < ZDT_N_STATION_NUM; i++)
            {
                if(station1_ptr->n_cell[i].is_valid )
                {
                    valid_num++;
                    for(j = 0; j < ZDT_N_STATION_NUM; j++)
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

LOCAL BOOLEAN  MMIZDT_Station_GetSCNetInfo(MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN res = FALSE;
    if(g_zdt_station_data.s_cell.cell_id  != 0)

    {
        res = TRUE;
        g_zdt_station_data.s_cell.is_valid = TRUE;
    }
    else
    {
        g_zdt_station_data.s_cell.is_valid = FALSE;
    }
    return res;
}

static void  MMIZDT_Station_NC_ReBuild_Data(uint8 cnt)
{
    uint8 i;
    uint8 j;
    MMIZDT_N_STATION_DATA_T tmp;
    
    if(cnt == 0)
    {
        return;
    }
    
    for(i=0;i<cnt-1;i++) 
    {
        for(j=i+1;j<cnt;j++)
        {
            if(g_zdt_station_data.n_cell[i].rssiLevel > g_zdt_station_data.n_cell[j].rssiLevel)
            { 
                tmp=g_zdt_station_data.n_cell[i];
                g_zdt_station_data.n_cell[i]=g_zdt_station_data.n_cell[j]; 
                g_zdt_station_data.n_cell[j]=tmp;
            }
        }
    }
    return;
}

LOCAL BOOLEAN   MMIZDT_Station_GetNCNetInfo(MN_DUAL_SYS_E dual_sys)
{
    APP_MN_NCELL_RSSI_CNF_T ncell = {0};
    MN_PHONE_CELL_MEAS_T    scell = {0};
    int i = 0;
    int index_present[ZDT_N_STATION_NUM] = {0};
    int num_present = 0;
    BOOLEAN res = FALSE;
    
#ifndef WIN32
    if (MNPHONE_GetMeasReportEx(dual_sys, &ncell,&scell) == MN_RETURN_SUCCESS)
#else 
    for (i = 0; i < ZDT_N_STATION_NUM; i++)		// this is for example for simulator
    {
        ncell.ncell_power[i].cell_present = TRUE;
        ncell.ncell_power[i].arfcn = i * 10;
        ncell.ncell_power[i].rxlev = i * 50;
    }
#endif
    {
        for (i = 0; i < ZDT_N_STATION_NUM; i++)		// this is for example for simulator
        {
            g_zdt_station_data.n_cell[i].is_valid = FALSE;
            if (ncell.ncell_power[i].cell_present && ncell.ncell_power[i].cell_id != 0)	// this channel is served
            {
                res = TRUE;
                index_present[num_present++] = i;
            }
        }
        
        for (i = 0; i < num_present; i++)
        {
            g_zdt_station_data.n_cell[i].cell_id = ncell.ncell_power[index_present[i]].cell_id;
            g_zdt_station_data.n_cell[i].rssiLevel = ncell.ncell_power[index_present[i]].rxlev;
            g_zdt_station_data.n_cell[i].dual_sys = dual_sys;
            g_zdt_station_data.n_cell[i].is_valid = TRUE;
        }
        
        if(num_present > 0)
        {
            MMIZDT_Station_NC_ReBuild_Data(num_present);
        }
    }
    return res;
}

LOCAL BOOLEAN   MMIZDT_Station_N_IsNew(MN_DUAL_SYS_E dual_sys)
{
    MN_PHONE_CELL_MEAS_T    scell = {0};
    APP_MN_NCELL_RSSI_CNF_T ncell = {0};
    int i = 0;
    int j = 0;
    int index_present[ZDT_N_STATION_NUM] = {0};
    int num_present = 0;
    BOOLEAN res = FALSE;
    MN_PHONE_CELL_MEAS_T            ncell_power = {0};
    uint8 min_rxlev = 0xff;
#ifndef WIN32
    if (MNPHONE_GetMeasReportEx(dual_sys, &ncell,&scell) == MN_RETURN_SUCCESS)
#else 
    for (i = 0; i < ZDT_N_STATION_NUM; i++)		// this is for example for simulator
    {
        ncell.ncell_power[i].cell_present = TRUE;
        ncell.ncell_power[i].arfcn = i * 10;
        ncell.ncell_power[i].rxlev = i * 50;
    }
#endif
    {
        for (i = 0; i < ZDT_N_STATION_NUM; i++)		// this is for example for simulator
        {
            if (ncell.ncell_power[i].cell_present && ncell.ncell_power[i].cell_id != 0)	// this channel is served
            {
                res = TRUE;
                index_present[num_present++] = i;
            }
        }
 
        for (i = 0; i < num_present; i++)
        {
            if(ncell.ncell_power[index_present[i]].rxlev < min_rxlev)
            {
                ncell_power = ncell.ncell_power[index_present[i]];
                min_rxlev = ncell.ncell_power[index_present[i]].rxlev;
            }
        }
        
        if(g_zdt_n_station_data.cell_id != ncell_power.cell_id)
        {
            res = TRUE;
            g_zdt_n_station_data.cell_id = ncell_power.cell_id;
            g_zdt_n_station_data.rssiLevel = ncell_power.rxlev;
        }
    }
    return res;
}
LOCAL uint8  MMIZDT_Station_IsNew(APP_MN_NETWORK_STATUS_IND_T *status_ptr)
{
    uint8 res = FALSE;
    BOOLEAN need_check = FALSE;
    if(s_zdt_net_need_init)
    {
        need_check = TRUE;
    }
    else if(status_ptr->dual_sys == s_zdt_net_set_sys)
    {
        need_check = TRUE;
    }
    
    if(need_check)
    {
        if(g_zdt_s_station_data.mcc != status_ptr->mcc
            || g_zdt_s_station_data.mnc != status_ptr->mnc 
            || g_zdt_s_station_data.mnc_digit_num != status_ptr->mnc_digit_num
            || g_zdt_s_station_data.lac != status_ptr->lac
            || g_zdt_s_station_data.cell_id != status_ptr->cell_id
            )
        {
            if(status_ptr->cell_id != 0)
            {
                if(MMIZDT_Station_N_IsNew(status_ptr->dual_sys))
                {
                    res = 2;
                }
                else
                {
                    if(g_zdt_s_station_data.lac != status_ptr->lac)
                    {
                        res = 2;
                    }
                    else
                    {
                        res = 1;
                    }
                }
                g_zdt_s_station_data.dual_sys = status_ptr->dual_sys;
                g_zdt_s_station_data.lac = status_ptr->lac;
                g_zdt_s_station_data.mcc = status_ptr->mcc;
                g_zdt_s_station_data.mnc = status_ptr->mnc;
                g_zdt_s_station_data.mnc_digit_num = status_ptr->mnc_digit_num;
                g_zdt_s_station_data.cell_id = status_ptr->cell_id;
                g_zdt_s_station_data.is_valid = TRUE;
            }
            else
            {
                g_zdt_s_station_data.is_valid = FALSE;
            }
        }
    }
    return res;
}

BOOLEAN MMIZDT_Station_Update(void)
{
    BOOLEAN res = FALSE;
    g_zdt_station_data.s_cell = g_zdt_s_station_data;
    if(MMIZDT_Station_GetSCNetInfo(s_zdt_net_set_sys))
    {
        res = MMIZDT_Station_GetNCNetInfo(s_zdt_net_set_sys);
    }
    else
    {
        uint16 i = 0;
        for (i = 0; i < ZDT_N_STATION_NUM; i++)		// this is for example for simulator
        {
            g_zdt_station_data.n_cell[i].is_valid = FALSE;
        }
    }
    
    return res;
}

#ifdef ZDT_UDP_SUPPORT
uint16 MMIZDT_GPS_UDP_GetSSInfo(uint8 * pData)
{
    uint8 cell_num = 0;
    uint16 i = 0;
    uint16 data_len = 0;
    
    MMIZDT_Station_Update();


    pData[0] = g_zdt_station_data.s_cell.mcc >> 8;
    pData[1] = g_zdt_station_data.s_cell.mcc & 0x00FF;
    pData[2] = g_zdt_station_data.s_cell.mnc >> 8;
    pData[3] = g_zdt_station_data.s_cell.mnc & 0x00FF;
    pData[4] = g_zdt_station_data.s_cell.lac >> 8;
    pData[5] = g_zdt_station_data.s_cell.lac & 0x00FF;
    data_len = 7;
    if(g_zdt_station_data.s_cell.is_valid)
    {
        pData[data_len++] = g_zdt_station_data.s_cell.cell_id >> 8;
        pData[data_len++] = g_zdt_station_data.s_cell.cell_id & 0x00FF;
        pData[data_len++] = g_zdt_station_data.s_cell.rssiLevel;
        cell_num = 1;
        for (i = 0; i < ZDT_N_STATION_NUM; i++)		// this is for example for simulator
        {
            if (g_zdt_station_data.n_cell[i].is_valid)	// this channel is served
            {
                pData[data_len++] = g_zdt_station_data.n_cell[i].cell_id >> 8;
                pData[data_len++] = g_zdt_station_data.n_cell[i].cell_id & 0x00FF;
                pData[data_len++] = g_zdt_station_data.n_cell[i].rssiLevel;
                cell_num++;
            }
        }
        pData[6] = cell_num;
    }
    else
    {
        pData[6] = 0;
    }
    return data_len;
}
static uint16 MMIZDT_GPS_UDP_GetCRC(const uint8* pData, int nLength)
{
    uint16 i = 0;
    uint8 ch_a = 0;
    uint8 ch_b = 0;
    uint16 crc = 0;
    for(i = 0; i < nLength; i++)
    {
        ch_a = ch_a + pData[i];
        ch_b = ch_b + ch_a;
    }
// ch_a 是CRC的高位
// ch_b 是CRC的低位
    crc = ch_a;
    crc = crc << 8;
    crc += ch_b;
    return crc;
}

BOOLEAN MMIZDT_GPS_UDP_Send(uint8 protocol_id,uint8 * pContent,uint16 ContentLen)
{
/*
    格式 长度(byte)
    起始位 2    (0xEA 0xEA)
    协议号 1
    内容长度 2
    内容: N 
    错误校验码 2
    结束位 2 (0xEB,0xEB)
*/

    char msg_buf[300] = {0};
    uint16 len = 0;
    uint16 msg_len = 0;
    uint16 crc = 0;
    
    if(ContentLen == 0 || pContent == NULL )
    {
        return 0;
    }
    
    //起始位
    msg_buf[0] = 0xEA;
    msg_buf[1] = 0xEA;
    
    //协议号
    msg_buf[2] = protocol_id;
    
    //包长度
    msg_len = ContentLen;
    msg_buf[3] = msg_len>>8;
    msg_buf[4] = msg_len & 0x00FF;
    
    //数据
    SCI_MEMCPY(msg_buf+5,pContent,ContentLen);
    len = ContentLen + 5;
    
    //校验码
    crc = MMIZDT_GPS_UDP_GetCRC(msg_buf+2,len);
    msg_buf[len++] = crc>>8;
    msg_buf[len++] = crc&0x00FF;
    
    //结束位
    msg_buf[len++] = 0xEB;
    msg_buf[len++] = 0xEB;
#ifdef ZDT_PRODUCT_Z805
    return MMIZDT_UDP_AppSend((uint8 *)msg_buf,len,g_z805_cfg_data.ip_str,g_z805_cfg_data.port);
#else
    return MMIZDT_UDP_AppSend((uint8 *)msg_buf,len,"test",8080);
#endif
}

BOOLEAN  MMIZDT_GPS_UDP_Send_Loc(BOOLEAN loc_ok)
{
#ifdef ZDT_PRODUCT_Z805
    BOOLEAN res = FALSE;
    uint8 send_buf[200] = {0};
    uint16 send_len = 0;
    uint16 num_len = 0;
    GPS_DATA_T pos_data = {0};
    
    if(FALSE == MMIAPIPHONE_IsSimCardOk(MN_DUAL_SYS_1))
    {
        return FALSE;
    }
    
    SCI_MEMCPY(send_buf,g_zdt_phone_imei,15);
    SCI_MEMCPY(send_buf+15,g_zdt_sim_imsi,15);
    SCI_MEMCPY(send_buf+30,g_z805_cfg_data.gps_num,20);
    send_len = 50;

    GPS_API_GetLastPos(&pos_data);
    //GPS状态
    if(pos_data.is_valid && loc_ok)
    {
        send_buf[send_len++] = 1;
    }
    else
    {
        send_buf[send_len++] = 0;
    }

    send_len += GPS_UDP_Get_StrDateTime(send_buf+send_len,pos_data.date,pos_data.time);
    send_buf[send_len++] = pos_data.sate_num;
    if(pos_data.Lat_Dir == 'N') //N-S 北纬
    {
        send_buf[send_len++] = 1;
    }
    else
    {
        send_buf[send_len++] = 0;
    }

    send_len += GPS_UDP_Get_GPSLat(send_buf+send_len,pos_data.Latitude);
    
    if(pos_data.Long_Dir == 'W') //W-E 西经
    {
        send_buf[send_len++] = 1;
    }
    else
    {
        send_buf[send_len++] = 0;
    }
    send_len += GPS_UDP_Get_GPSLong(send_buf+send_len,pos_data.Longitude);
    send_len += GPS_UDP_Get_GPSSpeed(send_buf+send_len,pos_data.speed);
    send_len += GPS_UDP_Get_GPSCog(send_buf+send_len,pos_data.cog);
    send_len += MMIZDT_GPS_UDP_GetSSInfo(send_buf+send_len);
    res = MMIZDT_GPS_UDP_Send(0xA1,send_buf,send_len);
    return res;
#else
    return FALSE;
#endif
}
#endif

#ifdef ZDT_TCP_SUPPORT
#ifdef ZDT_GPS_SUPPORT_AGPS
#define AGPS_TCP_MAX_RCV_BUF_LEN 10240
ZDT_TCP_INTERFACE_T * m_tcp_gps_interface = PNULL;
static uint8 gps_tcp_rcvbuf[AGPS_TCP_MAX_RCV_BUF_LEN+1]={0};
static uint32 gps_tcp_rcvbuf_len = 0;
static BOOLEAN UBX_AGPS_TCP_RCV(ZDT_TCP_INTERFACE_T * pMe,uint8 * pRcv,uint32 Rcv_len)
{
    char * pStart = NULL;
    char * pHeadEnd = NULL;
    char * pLen = NULL;
    char * pData = NULL;
    uint16 payloadLen = 0;
    uint16 expectedLength = 0;
    GPS_TRACE("ZDT_TCP_GPS_RCV Len = %d",Rcv_len);
    if(pRcv == NULL || Rcv_len < 4)
    {
        GPS_TRACE("ZDT_TCP_GPS_RCV ERR Len");
        return FALSE;
    }
    pStart = (char *)pRcv;
    pHeadEnd = strstr(pStart,"\r\n\r\n");
    if(pHeadEnd)
    {
        pLen = strstr(pStart,"Content-Length");
        if(pLen == NULL)
        {
            GPS_TRACE("ZDT_TCP_GPS_RCV ERR No Content-Length");
            return FALSE;
        }
        sscanf(pLen,"Content-Length: %u",&payloadLen);
        if(payloadLen > 0)
        {
            expectedLength = pHeadEnd - pStart;
            expectedLength +=4;
            expectedLength +=payloadLen;
            if(Rcv_len >=  expectedLength)
            {
                *pHeadEnd = 0;
                GPS_TRACE("ZDT_TCP_GPS_RCV Over expectedLength=%d,%d,payloadLen=%d",expectedLength,Rcv_len,payloadLen);
                if(strstr(pStart,"Content-Type: application/ubx"))
                {
                    g_gps_eph_data.eph_data_len = 0;
                    if(g_gps_eph_is_set)
                    {
                        #ifndef WIN32
                        ZDT_GPS_EphSetEnd();
                        #endif
                    }
                    if(g_gps_eph_data.eph_data != NULL)
                    {
                        SCI_FREE(g_gps_eph_data.eph_data);
                        g_gps_eph_data.eph_data = NULL;
                    }
                    g_gps_eph_data.eph_times = 0;
                    g_gps_eph_data.eph_data = SCI_ALLOC_APPZ(payloadLen+1);
                    if(g_gps_eph_data.eph_data != NULL)
                    {
                        SCI_MEMCPY(g_gps_eph_data.eph_data,pHeadEnd+4,payloadLen);
                        g_gps_eph_data.eph_data[payloadLen] = 0;
                        g_gps_eph_data.eph_data_len = payloadLen;
                        g_gps_eph_data.eph_times = SCI_GetCurrentTime();
                        #ifndef WIN32
                        ZDT_GPS_EphSetStart();
                        #endif
                    }
                    GPS_TRACE("ZDT_TCP_GPS_RCV OK");
                }
                return TRUE;
            }
        }
        else
        {
            GPS_TRACE("ZDT_TCP_GPS_RCV ERR Content-Length==0");
        }
    }
    return FALSE;
}
static int UBX_AGPS_TCP_Receive(void * pUser,uint8 * pRcv,uint32 Rcv_len)
{
    ZDT_TCP_INTERFACE_T * pMe = (ZDT_TCP_INTERFACE_T *)pUser;
    GPS_TRACE("TCP GPS Rcv_len=%d,all_len=%d",Rcv_len,gps_tcp_rcvbuf_len);
    if((gps_tcp_rcvbuf_len + Rcv_len) > AGPS_TCP_MAX_RCV_BUF_LEN)
    {
        GPS_TRACE("TCP AGPS FULL %d",gps_tcp_rcvbuf_len + Rcv_len);
        SCI_MEMSET(gps_tcp_rcvbuf, 0, AGPS_TCP_MAX_RCV_BUF_LEN+1);
        gps_tcp_rcvbuf_len = 0;
        return 1;
    }
    
    SCI_MEMCPY(gps_tcp_rcvbuf+gps_tcp_rcvbuf_len,pRcv,Rcv_len);
    gps_tcp_rcvbuf_len += Rcv_len;
    
    if(UBX_AGPS_TCP_RCV(pMe,(uint8 *)gps_tcp_rcvbuf,(uint16)gps_tcp_rcvbuf_len))
    {
        return 0;
    }
    return 1;
}

static void UBX_AGPS_TCP_SendExt(ZDT_TCP_INTERFACE_T *pMe)
{
    char send_buf[230] = {0};
    char buf_lat[20] = {0};
    char buf_long[20] = {0};
    uint16 send_len = 0;
    GPS_DATA_T last_gps = {0};
    GPS_TRACE("UBX_AGPS_TCP_SendExt");
    GPS_API_GetLastPos(&last_gps);
    if(last_gps.is_valid == 0)
    {
        last_gps.Lat_m = 331243;
        last_gps.Latitude = 22.552072 * 180000;
        last_gps.Longitude = 113.921288 * 180000;
    }
    //pMe->m_agps_is_get = TRUE;
    #ifndef WIN32
    GPS_Data_Get_StrLatD(last_gps.Latitude,last_gps.Lat_m,buf_lat);
    GPS_Data_Get_StrLongD(last_gps.Longitude,buf_long);
    #endif
    //SPRINTF(send_buf,"cmd=aid;user=luoxu@cnedc.com;pwd=Bouqa;lat=%s;lon=%s;alt=74.4;pacc=500.00",buf_lat,buf_long);
    sprintf(send_buf,"cmd=aid;user=zdtagps@zdtone.com.cn;pwd=Ozdcs;lat=%s;lon=%s;alt=75.0;pacc=500.00",buf_lat,buf_long);
    send_len = strlen(send_buf);
    ZDT_API_TCP_SendData(pMe,(uint8 *)send_buf,send_len,0,0,0,UBX_AGPS_TCP_Receive);
   return;
}

static int UBX_AGPS_TCP_Callback(void * pUser,TCP_CB_TYPE_E cb_type,uint16 wParam, void * dwParam)
{
    ZDT_TCP_INTERFACE_T * pMe = (ZDT_TCP_INTERFACE_T *)pUser;
    GPS_TRACE("TCP GPS Callback: cb_type=%d,Result=%d,Data=0x%x",cb_type,wParam,dwParam);
    switch(cb_type)
    {
        case TCP_CB_REG:
            {
                uint32 send_hande = 0;
                uint16 ret = wParam;
                uint32 err_id =  wParam;
                if(ret == TCP_SUCCESS)
                {
                    //注册成功
                    GPS_TRACE("TCP GPS REG OK");
                    SCI_MEMSET(gps_tcp_rcvbuf, 0, AGPS_TCP_MAX_RCV_BUF_LEN+1);
                    gps_tcp_rcvbuf_len = 0;
                    UBX_AGPS_TCP_SendExt(pMe);
                }
                else
                {
                    //注册失败
                    GPS_TRACE("TCP GPS REG FAIL");
                }
            }
            break;
            
        case TCP_CB_UNREG:
            {
                uint16 ret = wParam;
                uint32 err_id =  wParam;
                if(ret == TCP_SUCCESS)
                {
                    //注销成功
                    GPS_TRACE("TCP GPS UNREG OK");
                }
                else
                {
                    //注销失败
                    GPS_TRACE("TCP GPS UNREG FAIL");
                }
            }
            break;
            
        case TCP_CB_SEND:
            {
                uint16 ret = wParam;
                ZDT_TCP_LINK_DATA_T * SendData = (ZDT_TCP_LINK_DATA_T*)dwParam;
                if(ret == TCP_SUCCESS)
                {
                    //发送成功
                    GPS_TRACE("TCP GPS Send OK Handle=0x%x",SendData);
                    if(SendData != NULL && SendData->str != NULL)
                    {
                        GPS_TRACE("%s",SendData->str);
                    }
                }
                else
                {
                    //发送失败
                    GPS_TRACE("TCP GPS Send FAIL Handle=0x%x",SendData);
                    if(SendData != NULL && SendData->str != NULL)
                    {
                        GPS_TRACE("%s",SendData->str);
                    }
                }
            }
            break;
            
        case TCP_CB_RCV:
            {
                uint16 ret = wParam;
                ZDT_TCP_RCV_DATA_T * RcvData = (ZDT_TCP_RCV_DATA_T *) dwParam;
                if(ret == TCP_SUCCESS)
                {
                    //接收成功
                    GPS_TRACE("TCP GPS RCV OK len=%d",RcvData->len);
                    if(RcvData != NULL && RcvData->pRcv!= NULL)
                    {
                        //GPS_TRACE("%s",RcvData->pRcv);
                    }
                }
                else
                {
                    //接收失败
                    GPS_TRACE("TCP GPS RCV FAIL");
                    if(RcvData != NULL && RcvData->pRcv!= NULL)
                    {
                        //GPS_TRACE("%s",RcvData->pRcv);
                    }
                }
            }
            break;
                        
        default:
            break;
    }
    return 0;
}

void UBX_AGPS_TCP_Send(void)
{
    if(m_tcp_gps_interface != PNULL)
    {
        ZDT_API_TCP_UnReg(m_tcp_gps_interface);
        ZDT_API_TCP_Reg(m_tcp_gps_interface,"agps.u-blox.com",46434,UBX_AGPS_TCP_Callback);
    }
}

static BOOLEAN TD_AGPS_TCP_RCV(ZDT_TCP_INTERFACE_T * pMe,uint8 * pRcv,uint32 Rcv_len)
{
    char * pStart = NULL;
    char * pHeadEnd = NULL;
    char * pHeadStart = NULL;
    char * pLen = NULL;
    char * pData = NULL;
    uint16 payloadLen = 0;
    uint16 expectedLength = 0;
    char len_buf[10] = {0};
    int buf_len = 0;
    GPS_TRACE("ZDT_TCP_GPS_RCV Len = %d",Rcv_len);
    if(pRcv == NULL || Rcv_len < 100)
    {
        GPS_TRACE("ZDT_TCP_GPS_RCV ERR Len");
        return FALSE;
    }
    pStart = (char *)pRcv;
    pHeadStart = strstr(pStart,"Data Length:");
    if(pHeadStart)
    {
        pHeadEnd = strstr(pHeadStart,"\n");
        if(pHeadEnd == NULL)
        {
            return FALSE;
        }
        buf_len = pHeadEnd - pHeadStart;
        buf_len = buf_len-12;
        if(buf_len <= 0 || buf_len > 6)
        {
            return FALSE;
        }
        SCI_MEMCPY(len_buf,pHeadStart+12,buf_len);
        payloadLen = (uint16)atoi(len_buf);
        GPS_TRACE("TD_AGPS_TCP_RCV payloadLen = %d",payloadLen);
        if(payloadLen > 0)
        {
            expectedLength = pHeadEnd - pStart;
            expectedLength +=payloadLen;
            payloadLen--;
            GPS_TRACE("TD_AGPS_TCP_RCV expectedLength=%d,%d,payloadLen=%d",expectedLength,Rcv_len,payloadLen);
            if(Rcv_len >=  expectedLength)
            {
                *pHeadEnd = 0;
                if(strstr(pStart,"Techtotop A-GNSS Server"))
                {
                    g_gps_eph_data.eph_data_len = 0;
                    if(g_gps_eph_is_set)
                    {
                        #ifndef WIN32
                        ZDT_GPS_EphSetEnd();
                        #endif
                    }
                    if(g_gps_eph_data.eph_data != NULL)
                    {
                        SCI_FREE(g_gps_eph_data.eph_data);
                        g_gps_eph_data.eph_data = NULL;
                    }
                    g_gps_eph_data.eph_times = 0;
                    g_gps_eph_data.eph_data = SCI_ALLOC_APPZ(payloadLen+1);
                    if(g_gps_eph_data.eph_data != NULL)
                    {
                        SCI_MEMCPY(g_gps_eph_data.eph_data,pHeadEnd+1,payloadLen);
                        g_gps_eph_data.eph_data[payloadLen] = 0;
                        g_gps_eph_data.eph_data_len = payloadLen;
                        g_gps_eph_data.eph_times = SCI_GetCurrentTime();
                        #ifndef WIN32
                        ZDT_GPS_EphSetStart();
                        #endif
                    }
                    GPS_TRACE("ZDT_TCP_GPS_RCV OK");
                }
                return TRUE;
            }
        }
        else
        {
            GPS_TRACE("ZDT_TCP_GPS_RCV ERR Data Length==0");
        }
    }
    return FALSE;
}

static int TD_AGPS_TCP_Receive(void * pUser,uint8 * pRcv,uint32 Rcv_len)
{
    ZDT_TCP_INTERFACE_T * pMe = (ZDT_TCP_INTERFACE_T *)pUser;
    GPS_TRACE("TCP GPS Rcv_len=%d,all_len=%d",Rcv_len,gps_tcp_rcvbuf_len);
    if((gps_tcp_rcvbuf_len + Rcv_len) > AGPS_TCP_MAX_RCV_BUF_LEN)
    {
        GPS_TRACE("TCP AGPS FULL %d",gps_tcp_rcvbuf_len + Rcv_len);
        SCI_MEMSET(gps_tcp_rcvbuf, 0, AGPS_TCP_MAX_RCV_BUF_LEN+1);
        gps_tcp_rcvbuf_len = 0;
        return 1;
    }
    
    SCI_MEMCPY(gps_tcp_rcvbuf+gps_tcp_rcvbuf_len,pRcv,Rcv_len);
    gps_tcp_rcvbuf_len += Rcv_len;
    
    if(TD_AGPS_TCP_RCV(pMe,(uint8 *)gps_tcp_rcvbuf,(uint16)gps_tcp_rcvbuf_len))
    {
        return 0;
    }
    return 1;
}

static void TD_AGPS_TCP_SendExt(ZDT_TCP_INTERFACE_T *pMe)
{
    char send_buf[230] = {0};
    uint16 send_len = 0;
    GPS_TRACE("TD_AGPS_TCP_SendExt");
    sprintf(send_buf,"all",3);
    send_len = strlen(send_buf);
    ZDT_API_TCP_SendData(pMe,(uint8 *)send_buf,send_len,0,0,0,TD_AGPS_TCP_Receive);
   return;
}

static int TD_AGPS_TCP_Callback(void * pUser,TCP_CB_TYPE_E cb_type,uint16 wParam, void * dwParam)
{
    ZDT_TCP_INTERFACE_T * pMe = (ZDT_TCP_INTERFACE_T *)pUser;
    GPS_TRACE("TCP GPS Callback: cb_type=%d,Result=%d,Data=0x%x",cb_type,wParam,dwParam);
    switch(cb_type)
    {
        case TCP_CB_REG:
            {
                uint32 send_hande = 0;
                uint16 ret = wParam;
                uint32 err_id =  wParam;
                if(ret == TCP_SUCCESS)
                {
                    //注册成功
                    GPS_TRACE("TCP GPS REG OK");
                    SCI_MEMSET(gps_tcp_rcvbuf, 0, AGPS_TCP_MAX_RCV_BUF_LEN+1);
                    gps_tcp_rcvbuf_len = 0;
                    TD_AGPS_TCP_SendExt(pMe);
                }
                else
                {
                    //注册失败
                    GPS_TRACE("TCP GPS REG FAIL");
                }
            }
            break;
            
        case TCP_CB_UNREG:
            {
                uint16 ret = wParam;
                uint32 err_id =  wParam;
                if(ret == TCP_SUCCESS)
                {
                    //注销成功
                    GPS_TRACE("TCP GPS UNREG OK");
                }
                else
                {
                    //注销失败
                    GPS_TRACE("TCP GPS UNREG FAIL");
                }
            }
            break;
            
        case TCP_CB_SEND:
            {
                uint16 ret = wParam;
                ZDT_TCP_LINK_DATA_T * SendData = (ZDT_TCP_LINK_DATA_T*)dwParam;
                if(ret == TCP_SUCCESS)
                {
                    //发送成功
                    GPS_TRACE("TCP GPS Send OK Handle=0x%x",SendData);
                    if(SendData != NULL && SendData->str != NULL)
                    {
                        GPS_TRACE("%s",SendData->str);
                    }
                }
                else
                {
                    //发送失败
                    GPS_TRACE("TCP GPS Send FAIL Handle=0x%x",SendData);
                    if(SendData != NULL && SendData->str != NULL)
                    {
                        GPS_TRACE("%s",SendData->str);
                    }
                }
            }
            break;
            
        case TCP_CB_RCV:
            {
                uint16 ret = wParam;
                ZDT_TCP_RCV_DATA_T * RcvData = (ZDT_TCP_RCV_DATA_T *) dwParam;
                if(ret == TCP_SUCCESS)
                {
                    //接收成功
                    GPS_TRACE("TCP GPS RCV OK len=%d",RcvData->len);
                    if(RcvData != NULL && RcvData->pRcv!= NULL)
                    {
                        //GPS_TRACE("%s",RcvData->pRcv);
                    }
                }
                else
                {
                    //接收失败
                    GPS_TRACE("TCP GPS RCV FAIL");
                    if(RcvData != NULL && RcvData->pRcv!= NULL)
                    {
                        //GPS_TRACE("%s",RcvData->pRcv);
                    }
                }
            }
            break;
                        
        default:
            break;
    }
    return 0;
}

void TD_AGPS_TCP_Send(void)
{
    if(m_tcp_gps_interface != PNULL)
    {
        ZDT_API_TCP_UnReg(m_tcp_gps_interface);
        ZDT_API_TCP_Reg(m_tcp_gps_interface,"agnss.techtotop.com",9011,TD_AGPS_TCP_Callback);
    }
}

static BOOLEAN CASIC_AGPS_TCP_RCV(ZDT_TCP_INTERFACE_T * pMe,uint8 * pRcv,uint32 Rcv_len)
{
    char * pStart = NULL;
    char * pLimitHead = NULL;
    char * pHeadEnd = NULL;
    char * pLen = NULL;
    char * pData = NULL;
    uint16 payloadLen = 0;
    uint16 expectedLength = 0;
    char cr_str[3] = {0x0A,0};
    
    GPS_TRACE("ZDT_TCP_GPS_RCV Len = %d",Rcv_len);
    
    if(pRcv == NULL || Rcv_len < 4)
    {
        GPS_TRACE("ZDT_TCP_GPS_RCV ERR Len");
        return FALSE;
    }
    pStart = (char *)pRcv;
    pLimitHead = strstr(pStart,"Limitation");
    if(pLimitHead)
    {
        pLen = strstr(pStart,"DataLength:");
        if(pLen == NULL)
        {
            GPS_TRACE("ZDT_TCP_GPS_RCV ERR No DataLength");
            return FALSE;
        }
        sscanf(pLen,"DataLength: %u",&payloadLen);
        pHeadEnd = strstr(pLimitHead,cr_str);
    }
    
    GPS_TRACE("ZDT_TCP_GPS_RCV DataLength=%d",payloadLen);
    if(pHeadEnd)
    {
        if(payloadLen > 0)
        {
            expectedLength = pHeadEnd - pStart;
            expectedLength ++;
            expectedLength +=payloadLen;
            if(Rcv_len >=  expectedLength)
            {
                *pHeadEnd = 0;
                GPS_TRACE("ZDT_TCP_GPS_RCV Over expectedLength=%d,%d,payloadLen=%d, 0x%02X,0x%02X",expectedLength,Rcv_len,payloadLen,pHeadEnd[1],pHeadEnd[2]);
                if(pHeadEnd[1] == 0xBA && pHeadEnd[2] == 0xCE )
                {
                    g_gps_eph_data.eph_data_len = 0;
                    if(g_gps_eph_is_set)
                    {
                        #ifndef WIN32
                        ZDT_GPS_EphSetEnd();
                        #endif
                    }
                    if(g_gps_eph_data.eph_data != NULL)
                    {
                        SCI_FREE(g_gps_eph_data.eph_data);
                        g_gps_eph_data.eph_data = NULL;
                    }
                    g_gps_eph_data.eph_times = 0;
                    g_gps_eph_data.eph_data = SCI_ALLOC_APPZ(payloadLen+1);
                    if(g_gps_eph_data.eph_data != NULL)
                    {
                        SCI_MEMCPY(g_gps_eph_data.eph_data,pHeadEnd+1,payloadLen);
                        g_gps_eph_data.eph_data[payloadLen] = 0;
                        g_gps_eph_data.eph_data_len = payloadLen;
                        g_gps_eph_data.eph_times = SCI_GetCurrentTime();
                        #ifndef WIN32
                        ZDT_GPS_EphSetStart();
                        #endif
                    }
                    GPS_TRACE("ZDT_TCP_GPS_RCV OK");
                }
                return TRUE;
            }
        }
        else
        {
            GPS_TRACE("ZDT_TCP_GPS_RCV ERR Content-Length==0");
        }
    }
    return FALSE;
}
static int CASIC_AGPS_TCP_Receive(void * pUser,uint8 * pRcv,uint32 Rcv_len)
{
    ZDT_TCP_INTERFACE_T * pMe = (ZDT_TCP_INTERFACE_T *)pUser;
    GPS_TRACE("TCP GPS Rcv_len=%d,all_len=%d",Rcv_len,gps_tcp_rcvbuf_len);
    //ZDT_EPHRcvTrace(pRcv,Rcv_len);
    //ZDT_HexTraceU8(pRcv,Rcv_len);
    if((gps_tcp_rcvbuf_len + Rcv_len) > AGPS_TCP_MAX_RCV_BUF_LEN)
    {
        GPS_TRACE("TCP AGPS FULL %d",gps_tcp_rcvbuf_len + Rcv_len);
        SCI_MEMSET(gps_tcp_rcvbuf, 0, AGPS_TCP_MAX_RCV_BUF_LEN+1);
        gps_tcp_rcvbuf_len = 0;
        return 1;
    }
    
    SCI_MEMCPY(gps_tcp_rcvbuf+gps_tcp_rcvbuf_len,pRcv,Rcv_len);
    gps_tcp_rcvbuf_len += Rcv_len;
    
    if(CASIC_AGPS_TCP_RCV(pMe,(uint8 *)gps_tcp_rcvbuf,(uint16)gps_tcp_rcvbuf_len))
    {
        return 0;
    }
    return 1;
}

static void CASIC_AGPS_TCP_SendExt(ZDT_TCP_INTERFACE_T *pMe)
{
    char send_buf[230] = {0};
    uint16 send_len = 0;
    GPS_TRACE("CASIC_AGPS_TCP_SendExt");
    //SPRINTF(send_buf,"cmd=aid;user=luoxu@cnedc.com;pwd=Bouqa;lat=%s;lon=%s;alt=74.4;pacc=500.00",buf_lat,buf_long);
    sprintf(send_buf,"user=chenzhijian@zdtone.com.cn;pwd=Ecell888888;cmd=full;");
    send_len = strlen(send_buf);
    ZDT_API_TCP_SendData(pMe,(uint8 *)send_buf,send_len,0,0,0,CASIC_AGPS_TCP_Receive);
   return;
}

static int CASIC_AGPS_TCP_Callback(void * pUser,TCP_CB_TYPE_E cb_type,uint16 wParam, void * dwParam)
{
    ZDT_TCP_INTERFACE_T * pMe = (ZDT_TCP_INTERFACE_T *)pUser;
    GPS_TRACE("TCP GPS Callback: cb_type=%d,Result=%d,Data=0x%x",cb_type,wParam,dwParam);
    switch(cb_type)
    {
        case TCP_CB_REG:
            {
                uint32 send_hande = 0;
                uint16 ret = wParam;
                uint32 err_id =  wParam;
                if(ret == TCP_SUCCESS)
                {
                    //注册成功
                    GPS_TRACE("TCP GPS REG OK");
                    SCI_MEMSET(gps_tcp_rcvbuf, 0, AGPS_TCP_MAX_RCV_BUF_LEN+1);
                    gps_tcp_rcvbuf_len = 0;
                    CASIC_AGPS_TCP_SendExt(pMe);
                }
                else
                {
                    //注册失败
                    GPS_TRACE("TCP GPS REG FAIL");
                }
            }
            break;
            
        case TCP_CB_UNREG:
            {
                uint16 ret = wParam;
                uint32 err_id =  wParam;
                if(ret == TCP_SUCCESS)
                {
                    //注销成功
                    GPS_TRACE("TCP GPS UNREG OK");
                }
                else
                {
                    //注销失败
                    GPS_TRACE("TCP GPS UNREG FAIL");
                }
            }
            break;
            
        case TCP_CB_SEND:
            {
                uint16 ret = wParam;
                ZDT_TCP_LINK_DATA_T * SendData = (ZDT_TCP_LINK_DATA_T*)dwParam;
                if(ret == TCP_SUCCESS)
                {
                    //发送成功
                    GPS_TRACE("TCP GPS Send OK Handle=0x%x",SendData);
                    if(SendData != NULL && SendData->str != NULL)
                    {
                        GPS_TRACE("%s",SendData->str);
                    }
                }
                else
                {
                    //发送失败
                    GPS_TRACE("TCP GPS Send FAIL Handle=0x%x",SendData);
                    if(SendData != NULL && SendData->str != NULL)
                    {
                        GPS_TRACE("%s",SendData->str);
                    }
                }
            }
            break;
            
        case TCP_CB_RCV:
            {
                uint16 ret = wParam;
                ZDT_TCP_RCV_DATA_T * RcvData = (ZDT_TCP_RCV_DATA_T *) dwParam;
                if(ret == TCP_SUCCESS)
                {
                    //接收成功
                    GPS_TRACE("TCP GPS RCV OK len=%d",RcvData->len);
                    if(RcvData != NULL && RcvData->pRcv!= NULL)
                    {
                        //GPS_TRACE("%s",RcvData->pRcv);
                    }
                }
                else
                {
                    //接收失败
                    GPS_TRACE("TCP GPS RCV FAIL");
                    if(RcvData != NULL && RcvData->pRcv!= NULL)
                    {
                        //GPS_TRACE("%s",RcvData->pRcv);
                    }
                }
            }
            break;
                        
        default:
            break;
    }
    return 0;
}

void CASIC_AGPS_TCP_Send(void)
{
    if(m_tcp_gps_interface != PNULL)
    {
        ZDT_API_TCP_UnReg(m_tcp_gps_interface);
        ZDT_API_TCP_Reg(m_tcp_gps_interface,"121.41.40.95",2621,CASIC_AGPS_TCP_Callback);
    }
}


static BOOLEAN ALLYSTAR_AGPS_TCP_RCV(ZDT_TCP_INTERFACE_T * pMe,uint8 * pRcv,uint32 Rcv_len)
{
    char * pStart = NULL;
    char * pLimitHead = NULL;
    char * pHeadEnd = NULL;
    char * pLen = NULL;
    char * pData = NULL;
    uint16 payloadLen = 0;
    uint16 expectedLength = 0;
    char cr_str[3] = {0x0A,0};
    
    GPS_TRACE("ZDT_TCP_GPS_RCV Len = %d",Rcv_len);
    
    if(pRcv == NULL || Rcv_len < 4)
    {
        GPS_TRACE("ZDT_TCP_GPS_RCV ERR Len");
        return FALSE;
    }
    pStart = (char *)pRcv;
    pLimitHead = strstr(pStart,"Limitation");
    if(pLimitHead)
    {
        pLen = strstr(pStart,"DataLength:");
        if(pLen == NULL)
        {
            GPS_TRACE("ZDT_TCP_GPS_RCV ERR No DataLength");
            return FALSE;
        }
        sscanf(pLen,"DataLength: %u",&payloadLen);
        pHeadEnd = strstr(pLimitHead,cr_str);
    }
    
    GPS_TRACE("ZDT_TCP_GPS_RCV DataLength=%d",payloadLen);
    if(pHeadEnd)
    {
        if(payloadLen > 0)
        {
            expectedLength = pHeadEnd - pStart;
            expectedLength ++;
            expectedLength +=payloadLen;
            if(Rcv_len >=  expectedLength)
            {
                *pHeadEnd = 0;
                GPS_TRACE("ZDT_TCP_GPS_RCV Over expectedLength=%d,%d,payloadLen=%d, 0x%02X,0x%02X",expectedLength,Rcv_len,payloadLen,pHeadEnd[1],pHeadEnd[2]);
                if(pHeadEnd[1] == 0xBA && pHeadEnd[2] == 0xCE )
                {
                    g_gps_eph_data.eph_data_len = 0;
                    if(g_gps_eph_is_set)
                    {
                        #ifndef WIN32
                        ZDT_GPS_EphSetEnd();
                        #endif
                    }
                    if(g_gps_eph_data.eph_data != NULL)
                    {
                        SCI_FREE(g_gps_eph_data.eph_data);
                        g_gps_eph_data.eph_data = NULL;
                    }
                    g_gps_eph_data.eph_times = 0;
                    g_gps_eph_data.eph_data = SCI_ALLOC_APPZ(payloadLen+1);
                    if(g_gps_eph_data.eph_data != NULL)
                    {
                        SCI_MEMCPY(g_gps_eph_data.eph_data,pHeadEnd+1,payloadLen);
                        g_gps_eph_data.eph_data[payloadLen] = 0;
                        g_gps_eph_data.eph_data_len = payloadLen;
                        g_gps_eph_data.eph_times = SCI_GetCurrentTime();
                        #ifndef WIN32
                        ZDT_GPS_EphSetStart();
                        #endif
                    }
                    GPS_TRACE("ZDT_TCP_GPS_RCV OK");
                }
                return TRUE;
            }
        }
        else
        {
            GPS_TRACE("ZDT_TCP_GPS_RCV ERR Content-Length==0");
        }
    }
    return FALSE;
}

static int ALLYSTAR_AGPS_TCP_Receive(void * pUser,uint8 * pRcv,uint32 Rcv_len)
{
    ZDT_TCP_INTERFACE_T * pMe = (ZDT_TCP_INTERFACE_T *)pUser;
    GPS_TRACE("TCP GPS Rcv_len=%d,all_len=%d",Rcv_len,gps_tcp_rcvbuf_len);
    //ZDT_EPHRcvTrace(pRcv,Rcv_len);
    //ZDT_HexTraceU8(pRcv,Rcv_len);
    if((gps_tcp_rcvbuf_len + Rcv_len) > AGPS_TCP_MAX_RCV_BUF_LEN)
    {
        GPS_TRACE("TCP AGPS FULL %d",gps_tcp_rcvbuf_len + Rcv_len);
        SCI_MEMSET(gps_tcp_rcvbuf, 0, AGPS_TCP_MAX_RCV_BUF_LEN+1);
        gps_tcp_rcvbuf_len = 0;
        return 1;
    }
    
    SCI_MEMCPY(gps_tcp_rcvbuf+gps_tcp_rcvbuf_len,pRcv,Rcv_len);
    gps_tcp_rcvbuf_len += Rcv_len;
    
    if(ALLYSTAR_AGPS_TCP_RCV(pMe,(uint8 *)gps_tcp_rcvbuf,(uint16)gps_tcp_rcvbuf_len))
    {
        return 0;
    }
    return 1;
}

static void ALLYSTAR_AGPS_TCP_SendExt(ZDT_TCP_INTERFACE_T *pMe)
{
    char send_buf[230] = {0};
    uint16 send_len = 0;
    GPS_TRACE("ALLYSTAR_AGPS_TCP_SendExt");
    //SPRINTF(send_buf,"cmd=aid;user=luoxu@cnedc.com;pwd=Bouqa;lat=%s;lon=%s;alt=74.4;pacc=500.00",buf_lat,buf_long);
    sprintf(send_buf,"user=chenzhijian@zdtone.com.cn;pwd=Ecell888888;cmd=full;");
    send_len = strlen(send_buf);
    ZDT_API_TCP_SendData(pMe,(uint8 *)send_buf,send_len,0,0,0,ALLYSTAR_AGPS_TCP_Receive);
   return;
}

static int ALLYSTAR_AGPS_TCP_Callback(void * pUser,TCP_CB_TYPE_E cb_type,uint16 wParam, void * dwParam)
{
    ZDT_TCP_INTERFACE_T * pMe = (ZDT_TCP_INTERFACE_T *)pUser;
    GPS_TRACE("TCP GPS Callback: cb_type=%d,Result=%d,Data=0x%x",cb_type,wParam,dwParam);
    switch(cb_type)
    {
        case TCP_CB_REG:
            {
                uint32 send_hande = 0;
                uint16 ret = wParam;
                uint32 err_id =  wParam;
                if(ret == TCP_SUCCESS)
                {
                    //注册成功
                    GPS_TRACE("TCP GPS REG OK");
                    SCI_MEMSET(gps_tcp_rcvbuf, 0, AGPS_TCP_MAX_RCV_BUF_LEN+1);
                    gps_tcp_rcvbuf_len = 0;
                    ALLYSTAR_AGPS_TCP_SendExt(pMe);
                }
                else
                {
                    //注册失败
                    GPS_TRACE("TCP GPS REG FAIL");
                }
            }
            break;
            
        case TCP_CB_UNREG:
            {
                uint16 ret = wParam;
                uint32 err_id =  wParam;
                if(ret == TCP_SUCCESS)
                {
                    //注销成功
                    GPS_TRACE("TCP GPS UNREG OK");
                }
                else
                {
                    //注销失败
                    GPS_TRACE("TCP GPS UNREG FAIL");
                }
            }
            break;
            
        case TCP_CB_SEND:
            {
                uint16 ret = wParam;
                ZDT_TCP_LINK_DATA_T * SendData = (ZDT_TCP_LINK_DATA_T*)dwParam;
                if(ret == TCP_SUCCESS)
                {
                    //发送成功
                    GPS_TRACE("TCP GPS Send OK Handle=0x%x",SendData);
                    if(SendData != NULL && SendData->str != NULL)
                    {
                        GPS_TRACE("%s",SendData->str);
                    }
                }
                else
                {
                    //发送失败
                    GPS_TRACE("TCP GPS Send FAIL Handle=0x%x",SendData);
                    if(SendData != NULL && SendData->str != NULL)
                    {
                        GPS_TRACE("%s",SendData->str);
                    }
                }
            }
            break;
            
        case TCP_CB_RCV:
            {
                uint16 ret = wParam;
                ZDT_TCP_RCV_DATA_T * RcvData = (ZDT_TCP_RCV_DATA_T *) dwParam;
                if(ret == TCP_SUCCESS)
                {
                    //接收成功
                    GPS_TRACE("TCP GPS RCV OK len=%d",RcvData->len);
                    if(RcvData != NULL && RcvData->pRcv!= NULL)
                    {
                        //GPS_TRACE("%s",RcvData->pRcv);
                    }
                }
                else
                {
                    //接收失败
                    GPS_TRACE("TCP GPS RCV FAIL");
                    if(RcvData != NULL && RcvData->pRcv!= NULL)
                    {
                        //GPS_TRACE("%s",RcvData->pRcv);
                    }
                }
            }
            break;
                        
        default:
            break;
    }
    return 0;
}

void ALLYSTAR_AGPS_TCP_Send(void)
{
    if(m_tcp_gps_interface != PNULL)
    {
        ZDT_API_TCP_UnReg(m_tcp_gps_interface);
        ZDT_API_TCP_Reg(m_tcp_gps_interface,"121.41.40.95",2621,ALLYSTAR_AGPS_TCP_Callback);
    }
}


BOOLEAN AGPS_TCP_Init(void)
{
    if(ZdtTCP_NetOpen(&m_tcp_gps_interface,PNULL,10240))
    {
        return TRUE;
    }
    return FALSE;

}
#endif

static int zdt_net_tcp_test_receive(void *pUser,uint8 * pRcv,uint32 Rcv_len)
{
    ZDT_TCP_INTERFACE_T * pMe = (ZDT_TCP_INTERFACE_T *)pUser;
    ZDT_LOG("TCP TEST Rcv_len=%d",Rcv_len);
    return ZDT_TCP_RET_OK;
}

static int zdt_net_tcp_test_callback(void * pUser,TCP_CB_TYPE_E cb_type,uint16 wParam, void * dwParam)
{
    ZDT_TCP_INTERFACE_T * pMe = (ZDT_TCP_INTERFACE_T *)pUser;
    ZDT_LOG("TCP TEST Callback: cb_type=%d,Result=%d,Data=0x%x",cb_type,wParam,dwParam);
    switch(cb_type)
    {
        case TCP_CB_REG:
            {
                uint32 send_hande = 0;
                uint16 ret = wParam;
                uint32 err_id =  wParam;
                if(ret == TCP_SUCCESS)
                {
                    //注册成功
                    ZDT_LOG("TCP TEST REG OK");
                    send_hande = ZDT_API_TCP_SendData(pMe,(uint8 *)"Test TCP\r\n",10,0,0,0,zdt_net_tcp_test_receive);
                    ZDT_LOG("TCP TEST Send Test TCP send_hande=0x%x",send_hande);
                }
                else
                {
                    //注册失败
                    ZDT_LOG("TCP TEST REG FAIL");
                }
            }
            break;
            
        case TCP_CB_UNREG:
            {
                uint16 ret = wParam;
                uint32 err_id =  wParam;
                if(ret == TCP_SUCCESS)
                {
                    //注销成功
                    ZDT_LOG("TCP TEST UNREG OK");
                }
                else
                {
                    //注销失败
                    ZDT_LOG("TCP TEST UNREG FAIL");
                }
            }
            break;
            
        case TCP_CB_SEND:
            {
                uint16 ret = wParam;
                ZDT_TCP_LINK_DATA_T * SendData = (ZDT_TCP_LINK_DATA_T*)dwParam;
                if(ret == TCP_SUCCESS)
                {
                    //发送成功
                    ZDT_LOG("TCP TEST Send OK Handle=0x%x",SendData);
                    if(SendData != NULL && SendData->str != NULL)
                    {
                        ZDT_LOG("%s",SendData->str);
                    }
                }
                else
                {
                    //发送失败
                    ZDT_LOG("TCP TEST Send FAIL Handle=0x%x",SendData);
                    if(SendData != NULL && SendData->str != NULL)
                    {
                        ZDT_LOG("%s",SendData->str);
                    }
                }
            }
            break;
            
        case TCP_CB_RCV:
            {
                uint16 ret = wParam;
                ZDT_TCP_RCV_DATA_T * RcvData = (ZDT_TCP_RCV_DATA_T *) dwParam;
                if(ret == TCP_SUCCESS)
                {
                    //接收成功
                    ZDT_LOG("TCP TEST RCV OK len=%d",RcvData->len);
                    if(RcvData != NULL && RcvData->pRcv!= NULL)
                    {
                        //ZDT_LOG("%s",RcvData->pRcv);
                    }
                }
                else
                {
                    //接收失败
                    ZDT_LOG("TCP TEST RCV FAIL");
                    if(RcvData != NULL && RcvData->pRcv!= NULL)
                    {
                        //ZDT_LOG("%s",RcvData->pRcv);
                    }
                }
            }
            break;
                        
        default:
            break;
    }
    return 0;
}

void zdt_net_tcp_test_start(ZDT_TCP_INTERFACE_T *pMe)
{
    ZDT_LOG("zdt_net_tcp_test_start");
    ZDT_API_TCP_UnReg(pMe);
    ZDT_API_TCP_Reg(pMe,"120.24.172.55",9103,zdt_net_tcp_test_callback);
}
#endif


LOCAL void ZDT_Net_HandleGPRSCheckTimer(
                                            uint8 timer_id,
                                            uint32 param
                                            )
{
    ZDT_NET_TRACE("ZDT NET GPRSCheck ERR TimeOut");
    s_zdt_net_cur_gprs_status = FALSE;
    if(s_zdt_net_need_init == FALSE)
    {
        MMIZDT_Net_Close();
    }
}

/*****************************************************************************/
//  Discription: Start socket connect timer
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
/*****************************************************************************/
LOCAL void ZDT_Net_StartGPRSCheckTimer(void)
{
    if(0 != s_zdt_net_gprs_check_timer_id)
    {
        MMK_StopTimer(s_zdt_net_gprs_check_timer_id);
        s_zdt_net_gprs_check_timer_id = 0;
    }
    
    s_zdt_net_gprs_check_timer_id = MMK_CreateTimerCallback(ZDT_GPRS_CHECK_TIMEOUT, 
                                                                        ZDT_Net_HandleGPRSCheckTimer, 
                                                                        PNULL, 
                                                                        FALSE);
}
/*****************************************************************************/
//  Discription: Start socket connect timer
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
/*****************************************************************************/
LOCAL void ZDT_Net_StopGPRSCheckTimer(void)
{
    if(0 != s_zdt_net_gprs_check_timer_id)
    {
        MMK_StopTimer(s_zdt_net_gprs_check_timer_id);
        s_zdt_net_gprs_check_timer_id = 0;
    }
}

LOCAL  BOOLEAN MMIZDT_Net_StatusChange(MN_DUAL_SYS_E dual_sys,BOOLEAN is_ok)
{
    BOOLEAN last_gprs_status = s_zdt_net_cur_gprs_status;
    ZDT_NET_TRACE("ZDT NET StatusChange set_sys=%d,dual_sys=%d,is_ok=%d,need_init=%d,last_gprs_status=%d",s_zdt_net_set_sys,dual_sys,is_ok,s_zdt_net_need_init,last_gprs_status);
    if(FALSE == MMIZDT_Net_GPRS_IsOn() || MMIAPISET_GetFlyMode())
    {
        ZDT_LOG("MMIZDT_Net_StatusChange In Fly");
        s_zdt_net_cur_gprs_status = FALSE;
        return FALSE;
    }
    if(s_zdt_net_need_init == FALSE)
    {
        if(s_zdt_net_set_sys != dual_sys)
        {
            return FALSE;
        }
    }
    if(is_ok)
    {
        ZDT_Net_StopGPRSCheckTimer();
        s_zdt_net_cur_gprs_status = TRUE;
        if(s_zdt_net_need_init )
        {
            MMISET_MAJOR_SIM_SEL_TYPE_E major_sim_type = MMISET_MAJOR_SIM_SEL_MANUAL_SEL;
            major_sim_type = MMIAPISET_GetMajorSim(MMISET_MAJOR_SIM_SEL_FUNC_WAP);
            if ((major_sim_type < MMISET_MAJOR_SIM_SEL_MANUAL_SEL)
                && MMIAPIPHONE_IsSimAvailable(MN_DUAL_SYS_1+(major_sim_type-MMISET_MAJOR_SIM_SEL_SIM1))/*lint !e656*/ 
                )
            {
                MMIZDT_Net_SetActiveSys(major_sim_type);
            }
            else
            {
                MMIZDT_Net_SetActiveSys(dual_sys);
            }
            if(s_zdt_net_set_sys != dual_sys)
            {
                s_zdt_net_cur_gprs_status = FALSE;
            }
            else
            {
                MMIZDT_Net_Init();
                s_zdt_net_need_init = FALSE;
            }
        }
        else
        {
            if(YX_Net_IsAllow(&g_yx_app) && (FALSE == YX_Net_Is_Land()) && (last_gprs_status == FALSE || s_zdt_net_id == 0))
            {
                MMIZDT_Net_Reset(TRUE);
            }
        }
    }
    else
    {
        ZDT_Net_StartGPRSCheckTimer();
    }
    return TRUE;
}

PUBLIC BOOLEAN MMIZDT_Net_StatusInd(APP_MN_NETWORK_STATUS_IND_T *status_ptr)
{
    uint8 status_change = 0;
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
    //ZDT_LOG("MMIZDT_Net_StatusInd dual_sys=%d(%d),selecting=%d,plmn=%d,gprs_support=%d",status_ptr->dual_sys,s_zdt_net_gprs_status[status_ptr->dual_sys],status_ptr->plmn_is_selecting,status_ptr->plmn_status,status_ptr->gprs_support);
    //ZDT_LOG("MMIZDT_Net_StatusInd mcc=%d,mnc=%d,lac=%d,mnc_digit_num=%d,cell_id=%d",status_ptr->mcc,status_ptr->mnc,status_ptr->lac,status_ptr->mnc_digit_num,status_ptr->cell_id);
    status_change = MMIZDT_Station_IsNew(status_ptr);
    if(status_change == 2)
    {
#ifdef ZDT_PLAT_YX_SUPPORT
        YX_API_LBS_Change_Post();
#endif
    }
    
    if (status_ptr->plmn_is_selecting)
    {
        if(s_zdt_net_gprs_status[status_ptr->dual_sys])
        {
            s_zdt_net_gprs_status[status_ptr->dual_sys] = FALSE;
            MMIZDT_Net_StatusChange(status_ptr->dual_sys,FALSE);
        }
    }
    else 
    {
        if(s_zdt_net_gprs_status[status_ptr->dual_sys] == FALSE)
        {
            if(
              	status_ptr->gprs_support
                    && MMIAPIPHONE_IsSimAvailable(status_ptr->dual_sys)
                    && (status_ptr->plmn_status == PLMN_NORMAL_GPRS_ONLY || status_ptr->plmn_status == PLMN_NORMAL_GSM_GPRS_BOTH)
                )
            {
                g_zdt_net_cell_id = status_ptr->cell_id;
                ZDT_LOG("MMIZDT_Net_StatusInd OK dual_sys=%d,plmn=%d,gprs_support=%d,cell_id = %d",status_ptr->dual_sys,status_ptr->plmn_status,status_ptr->gprs_support,g_zdt_net_cell_id);
                s_zdt_net_gprs_status[status_ptr->dual_sys] = TRUE;
                MMIZDT_Net_StatusChange(status_ptr->dual_sys,TRUE);
            }
        }
        else
        {
            if(status_ptr->gprs_support == 0 || (status_ptr->plmn_status != PLMN_NORMAL_GSM_ONLY && status_ptr->plmn_status != PLMN_NORMAL_GPRS_ONLY && status_ptr->plmn_status != PLMN_NORMAL_GSM_GPRS_BOTH))
            {
                ZDT_LOG("MMIZDT_Net_StatusInd FAIL dual_sys=%d, plmn=%d,gprs_support=%d",status_ptr->dual_sys,status_ptr->plmn_status,status_ptr->gprs_support);
                s_zdt_net_gprs_status[status_ptr->dual_sys] = FALSE;
                MMIZDT_Net_StatusChange(status_ptr->dual_sys,FALSE);
            }
        }
    }
    return TRUE;
}

void MMIZDT_Net_RssiInd(MN_DUAL_SYS_E dual_sys,uint8 rssi)
{
    if(dual_sys == s_zdt_net_set_sys)
    {
        g_zdt_s_station_data.rssiLevel = rssi;
    }
}

uint16 MMIZDT_Net_GetRssiInd(void)
{
    return g_zdt_s_station_data.rssiLevel;
}

BOOLEAN MMIZDT_Net_Rssi_IsLow(void)
{
    BOOLEAN is_low_rssi = FALSE;
    if(ZDT_SIM_Exsit())
    {
        if(MNPHONE_GetRat(s_zdt_net_set_sys) == 2)
        {
            // 2G
            if(g_zdt_s_station_data.rssiLevel < 13)
            {
                is_low_rssi = TRUE;
            }
        }
        else
        {
            // 4G
            if(g_zdt_s_station_data.rssiLevel < 40)
            {
                is_low_rssi = TRUE;
            }
        }
    }
    return is_low_rssi;
}

BOOLEAN MMIZDT_Net_GPRS_SimSwitch(MN_DUAL_SYS_E dual_sys)
{
    ZDT_NET_TRACE("MMIZDT_Net_GPRS_SimSwitch dual_sys=%s,s_zdt_net_need_init=%d",dual_sys,s_zdt_net_need_init);
    if(s_zdt_net_need_init == FALSE)
    {
        MMIZDT_Net_SetActiveSys(dual_sys);
        if(ZDT_Net_IsAllow())
        {
            MMIZDT_Net_Reset(TRUE);
        }
    }
    return TRUE;
}

BOOLEAN MMIZDT_Net_GPRS_IsOn(void)
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

BOOLEAN MMIZDT_Net_GPRS_Attach(MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN res = FALSE;
    ZDT_NET_TRACE("MMIZDT_Net_GPRS_Attach dual_sys=%s,s_zdt_net_set_sys=%d",dual_sys,s_zdt_net_set_sys);
    if(s_zdt_net_need_init == FALSE)
    {
        MMIZDT_Net_SetActiveSys(dual_sys);
        if(ZDT_Net_IsAllow())
        {
            MMIZDT_Net_Reset(FALSE);
        }
    }
    return res;
}

BOOLEAN MMIZDT_Net_GPRS_Detach(MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN res = FALSE;
    ZDT_NET_TRACE("MMIZDT_Net_GPRS_Detach dual_sys=%s,s_zdt_net_set_sys=%d",dual_sys,s_zdt_net_set_sys);
    return res;
}

PUBLIC BOOLEAN MMIZDT_Net_APP_Init(void)
{
    MMIZDT_Net_NV_GetActiveSys();
    MMIZDT_Net_GetIMEI(MN_DUAL_SYS_1,g_zdt_phone_imei_1);
	#ifndef MMI_MULTI_SIM_SYS_SINGLE
    MMIZDT_Net_GetIMEI(MN_DUAL_SYS_2,g_zdt_phone_imei_2);
	#endif
#ifdef MEID_WRITE_SUPPORT
    MMIAPICOM_MEID_GetStr((uint8 *)g_zdt_phone_meid);
    g_zdt_phone_meid[14] = 0;
    ZDT_NET_TRACE("ZDT APP NET Init meid=%s, imei=%s,",g_zdt_phone_meid,g_zdt_phone_imei);
#else
    ZDT_NET_TRACE("ZDT APP NET Init imei=%s, imei2=%s",g_zdt_phone_imei,g_zdt_phone_imei_2);
#endif

#ifdef ZDT_UDP_SUPPORT
    MMIZDT_UDP_APP_Init();
#endif
#ifdef ZDT_TCP_SUPPORT
    ZdtTCP_AppInit();
#endif

#if defined(ZDT_PLAT_YX_SUPPORT_YS) || defined(ZDT_PLAT_YX_SUPPORT_FZD) || defined(ZDT_PLAT_YX_SUPPORT_TX)
    MMIZDT_Check2VMFile();
#endif
    return TRUE;
}


#endif
