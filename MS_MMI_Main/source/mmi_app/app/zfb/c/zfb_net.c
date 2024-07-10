
#ifndef  _MMI_ZFB_NET_C  
#define _MMI_ZFB_NET_C  

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "zfb_app.h"

#ifdef WIFI_SUPPORT
#include "wifisupp_api.h"
#endif

#define ZFB_NET_TRACE            SCI_TRACE_LOW

#define ZFB_PDP_DEFAULT_SIM_SYS MN_DUAL_SYS_1

#define ZFB_NET_RESET_TIMEOUT       10000  
#define ZFB_PDP_ACTIVE_TIMEOUT       10000  
#define ZFB_PDP_RETRY_TIMEOUT        2000 //重新尝试连接pdp，换卡连接
#define ZFB_NET_FIX_IMEI

#define y_abs(x)  ((x) >= 0 ? (x) : (-(x)))

char         g_zfb_phone_imei_1[ZFB_IMEI_LEN+1] = {0};
char         g_zfb_phone_imei_2[ZFB_IMEI_LEN+1] = {0};
char*       g_zfb_phone_imei = g_zfb_phone_imei_1;
char         g_zfb_sim_imsi[ZFB_IMSI_LEN+1] = {0};
char         g_zfb_sim_iccid[ZFB_ICCID_LEN+1] = {0};

LOCAL MN_DUAL_SYS_E       s_zfb_net_set_sys   = ZFB_PDP_DEFAULT_SIM_SYS;
LOCAL MN_DUAL_SYS_E       s_zfb_net_cur_sys   = MN_DUAL_SYS_1;
LOCAL uint8                       s_zfb_net_pdp_timer_id      = 0;
LOCAL uint8                       s_zfb_net_pdp_retry_timer_id = 0;
LOCAL uint8                       s_zfb_net_reset_timer_id      = 0;
LOCAL uint32                      s_zfb_net_id            = 0;
LOCAL BOOLEAN                  s_zfb_net_pdp_state_is_ok        = FALSE;
LOCAL BOOLEAN                  s_zfb_net_is_init        = FALSE;
LOCAL BOOLEAN                  s_zfb_net_is_activing        = FALSE;
LOCAL BOOLEAN                  s_zfb_net_is_closing        = FALSE;
LOCAL BOOLEAN                  s_zfb_net_need_active_close        = FALSE;
LOCAL BOOLEAN                  s_zfb_net_need_close_active        = FALSE;

static SCI_MUTEX_PTR g_zfb_mutexPtr = NULL;

ZFB_NET_LINK_DATA_T * g_zfb_net_cur_data = NULL;
ZFB_NET_LINK_NODE_T  * g_zfb_net_link_head = NULL;


LOCAL BOOLEAN  ZFB_Net_PDPTry(MN_DUAL_SYS_E sim_sys);
LOCAL BOOLEAN ZFB_Net_PdpDeactive(void);
LOCAL BOOLEAN ZFB_Net_PdpActive(void);

PUBLIC BOOLEAN  ZFB_NET_PostEvtTo_APP(ZFB_APP_SIG_E sig_id, void* p_data, uint16 type)
{
    uint8 * pstr = PNULL;
    ZFB_NET_DATA_SIG_T * psig = PNULL;    
    ZFB_NET_DATA_T * cur_data = PNULL;
    
    cur_data=(ZFB_NET_DATA_T *)SCI_ALLOC_APPZ(sizeof(ZFB_NET_DATA_T));
    if(cur_data == PNULL)
    {
        ZFB_NET_TRACE("ZFB_NET_PostEvtTo_APP ERR Malloc");
        return FALSE;
    }
    cur_data->data_p = p_data;
    cur_data->type = type;

    SCI_CREATE_SIGNAL((xSignalHeaderRec*)psig,sig_id,sizeof(ZFB_NET_DATA_SIG_T),SCI_IdentifyThread());
    psig->p_data = cur_data;
    SCI_SEND_SIGNAL((xSignalHeaderRec*)psig,P_APP);
    return TRUE;
}

uint16  MMIZFB_Net_GetIMEI(MN_DUAL_SYS_E dual_sys,char * imei_ptr)
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

	for( i=0; i<ZFB_IMEI_LEN; i++ )
	{
		imei_ptr[i] = tmp_buf1[i+1];
	}
    
	imei_ptr[ZFB_IMEI_LEN] = 0x00;

	return ZFB_IMEI_LEN;
}

uint16  MMIZFB_Get_SIM_IMSI(MN_DUAL_SYS_E dual_sys,char * imsi_ptr)
{
	uint8 i=0, j=0;
	MN_IMSI_T imsi = {0};
       char imsi_buf[ZFB_IMSI_LEN+1] = {0};
       
	imsi = MNSIM_GetImsiEx(dual_sys);

	if(imsi.imsi_val == NULL )
		return 0;

	for(i = 0, j = 0; i < 8; i++)
	{
		imsi_buf[j++] = (imsi.imsi_val[i] & 0x0F) + '0';
		imsi_buf[j++] = ((imsi.imsi_val[i] >> 4) & 0x0F) + '0';
	}
    
	for( i=0; i<ZFB_IMSI_LEN; i++ )
	{
		imsi_ptr[i] = imsi_buf[i+1];
	}
	imsi_ptr[ZFB_IMSI_LEN] = 0x00;
	
	return ZFB_IMSI_LEN;
}

uint16  MMIZFB_Get_SIM_ICCID(MN_DUAL_SYS_E dual_sys,char * iccid_ptr)
{
    uint16 len = 0;
    ERR_MNDATAMAG_CODE_E err_code = 0;
    char iccid_buf[ZFB_ICCID_LEN+1] = {0};
    MNSIM_ICCID_T iccid_t = {0};

    err_code = MNSIM_GetICCIDEx(dual_sys,&iccid_t);
    ZFB_NET_TRACE("MMIZFB_Get_SIM_ICCID dual_sys =%d,err_code = %d,id_num=0x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",dual_sys,err_code,iccid_t.id_num[9],iccid_t.id_num[8],iccid_t.id_num[7],iccid_t.id_num[6],iccid_t.id_num[5],iccid_t.id_num[4],iccid_t.id_num[3],iccid_t.id_num[2],iccid_t.id_num[1],iccid_t.id_num[0]);
    //MMIAPICOM_BcdToStr(PACKED_LSB_FIRST, iccid_t.id_num, ZFB_ICCID_LEN, iccid_buf);
    MMIAPICOM_BcdICCIDToStr(iccid_t.id_num, ZFB_ICCID_LEN, iccid_buf);
    len = strlen((char *)iccid_buf);
    ZFB_NET_TRACE("MMIZFB_Get_SIM_ICCID STR = %s",iccid_buf);
    SCI_MEMCPY(iccid_ptr,iccid_buf,len);
    
    return len;
}

uint16  MMIZFB_Get_IMSI_Str(MN_IMSI_T imsi,char * imsi_ptr)
{
	uint8 i=0, j=0;
       char imsi_buf[ZFB_IMSI_LEN+1] = {0};
       
	for(i = 0, j = 0; i < 8; i++)
	{
		imsi_buf[j++] = (imsi.imsi_val[i] & 0x0F) + '0';
		imsi_buf[j++] = ((imsi.imsi_val[i] >> 4) & 0x0F) + '0';
	}
    
	for( i=0; i<ZFB_IMSI_LEN; i++ )
	{
		imsi_ptr[i] = imsi_buf[i+1];
	}
	imsi_ptr[ZFB_IMSI_LEN] = 0x00;
	
	return ZFB_IMSI_LEN;
}

static void ZFB_NET_DelCurData(void)
{
    if(g_zfb_net_cur_data != NULL)
    {
        SCI_FREE(g_zfb_net_cur_data);
        g_zfb_net_cur_data = NULL;
    }
    return;
}

static BOOLEAN ZFB_NET_Link_AddData(ZFB_NET_LINK_DATA_T * pData)
{
    ZFB_NET_LINK_NODE_T  * p1 = NULL;
    ZFB_NET_LINK_NODE_T  * p2 = NULL;
    if(pData ==  NULL)
    {
        return FALSE;
    }
    
    p1=(ZFB_NET_LINK_NODE_T *)SCI_ALLOC_APPZ(sizeof(ZFB_NET_LINK_NODE_T));
    if(p1 == NULL)
    {
        return FALSE;
    }
    SCI_MEMSET(p1,0,sizeof(ZFB_NET_LINK_NODE_T));

    p1->data = pData;
    p1->next_ptr = NULL;
    
    if(g_zfb_net_link_head == NULL)
    {
        g_zfb_net_link_head = p1;
    }
    else
    {
        p2 = g_zfb_net_link_head;
        while(p2->next_ptr != NULL)
        {
            p2 = p2->next_ptr;
        }
        p2->next_ptr = p1;
    }
    
    return TRUE;
}

static BOOLEAN  ZFB_NET_Link_GetByPri(ZFB_NET_LINK_DATA_T ** pPostData,uint8 priority)
{
    ZFB_NET_LINK_NODE_T  * p1 = NULL;
    ZFB_NET_LINK_NODE_T  * p2 = NULL;
    BOOLEAN res = FALSE;
    
    p2 = p1 = g_zfb_net_link_head;
    
    while(p1 != NULL)
    {
        if(p1->data != NULL && p1->data->priority == priority)
        {
            *pPostData = p1->data;
            
            if(p1 == g_zfb_net_link_head)
            {
                g_zfb_net_link_head = p1->next_ptr;
            }
            else
            {
                p2->next_ptr = p1->next_ptr;
            }
            SCI_FREE(p1);
            res = TRUE;
            break;
        }
        p2 = p1;
        p1 = p2->next_ptr;
    }
        
    return res;
}

static BOOLEAN  ZFB_NET_Link_GetMaxPri(ZFB_NET_LINK_DATA_T ** pPostData)
{
    ZFB_NET_LINK_NODE_T  * p1 = NULL;
    ZFB_NET_LINK_NODE_T  * p2 = NULL;
    uint8 max_pri = 0;
    BOOLEAN res = FALSE;
    
    p2 = p1 = g_zfb_net_link_head;
    
    while(p1 != NULL)
    {
        if(p1->data != NULL && p1->data->priority > max_pri)
        {
            max_pri = p1->data->priority;
        }
        p2 = p1;
        p1 = p2->next_ptr;
    }
    
    res = ZFB_NET_Link_GetByPri(pPostData,max_pri);
    return res;
}

static BOOLEAN  ZFB_NET_Link_DelAll(void)
{
    ZFB_NET_LINK_NODE_T  * p1 = NULL;
    
    while(g_zfb_net_link_head != NULL)
    {
        p1 = g_zfb_net_link_head;
        g_zfb_net_link_head = p1->next_ptr;
        if(p1->data != NULL)
        {
            SCI_FREE(p1->data);
        }
        SCI_FREE(p1);
    }
    return TRUE;
}

static uint16  ZFB_NET_Link_Count(void)
{
    ZFB_NET_LINK_NODE_T  * p1 = NULL;
    uint16 num = 0;
    
    p1 = g_zfb_net_link_head;
    
    while(p1 != NULL)
    {
        num++;
        p1 = p1->next_ptr;
    }
    
    return num;
}

/*****************************************************************************/
//  Description : handle socket connect timeout
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL void ZFB_Net_HandlePdpActiveTimer(
                                            uint8 timer_id,
                                            uint32 param
                                            )
{
    ZFB_NET_TRACE("ZFB NET PdpActive ERR TimeOut");
    ZFB_Net_PdpDeactive();
    MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_NET_ACTIVE_FAIL,PNULL,0);
}

/*****************************************************************************/
//  Discription: Start socket connect timer
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
/*****************************************************************************/
LOCAL void ZFB_Net_StartPdpActiveTimer(void)
{
    uint32 cur_tm_out = ZFB_PDP_ACTIVE_TIMEOUT;
    if(0 != s_zfb_net_pdp_timer_id)
    {
        MMK_StopTimer(s_zfb_net_pdp_timer_id);
        s_zfb_net_pdp_timer_id = 0;
    }
    if(g_zfb_net_cur_data != NULL && g_zfb_net_cur_data->timeout != 0)
    {
        cur_tm_out = g_zfb_net_cur_data->timeout;
    }
    s_zfb_net_pdp_timer_id = MMK_CreateTimerCallback(cur_tm_out, 
                                                                        ZFB_Net_HandlePdpActiveTimer, 
                                                                        PNULL,
                                                                        FALSE);
}
/*****************************************************************************/
//  Discription: Start socket connect timer
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
/*****************************************************************************/
LOCAL void ZFB_Net_StopPdpActiveTimer(void)
{
    if(0 != s_zfb_net_pdp_timer_id)
    {
        MMK_StopTimer(s_zfb_net_pdp_timer_id);
        s_zfb_net_pdp_timer_id = 0;
    }
}

LOCAL void ZFB_Net_HandlePdpRetryTimer(
                                uint8 timer_id,
                                uint32 param
                                )
{
    MN_DUAL_SYS_E sys_sim = 0;
    BOOLEAN       sim_ok =  FALSE;

    if(timer_id == s_zfb_net_pdp_retry_timer_id && 0 != s_zfb_net_pdp_retry_timer_id)
    {
        MMK_StopTimer(s_zfb_net_pdp_retry_timer_id);
        s_zfb_net_pdp_retry_timer_id = 0;
    }
    s_zfb_net_cur_sys++;
    if(s_zfb_net_cur_sys < MMI_DUAL_SYS_MAX)
    {
        for(sys_sim = s_zfb_net_cur_sys; sys_sim < MMI_DUAL_SYS_MAX; sys_sim++)
        {
            if(MMIPHONE_IsSimOk(sys_sim))
            {
                s_zfb_net_cur_sys = sys_sim;
                sim_ok = TRUE;
                break;
            }
        }
        if(sim_ok)
        {
            if(!ZFB_Net_PDPTry(s_zfb_net_cur_sys))
            {
                ZFB_NET_TRACE("ZFB NET PdpActive Retry ERR cur_sys(%d),",s_zfb_net_cur_sys);
        	    s_zfb_net_pdp_retry_timer_id = MMK_CreateTimerCallback(
                                                                                                ZFB_PDP_RETRY_TIMEOUT, 
                                                                                                ZFB_Net_HandlePdpRetryTimer, 
                                                                                                PNULL, 
                                                                                                FALSE
                                                                                                );
            }
        }
        else
        {
            ZFB_NET_TRACE("ZFB NET PdpActive Retry ERR NO SIM");
            MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_NET_ACTIVE_FAIL,PNULL,0);
        }
    }
    else//找不到卡了
    {
        ZFB_NET_TRACE("ZFB NET PdpActive Retry ERR no_sys");
        MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_NET_ACTIVE_FAIL,PNULL,0);
    }
}

/*****************************************************************************
//  Description : handle pdp msg 
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
*****************************************************************************/
LOCAL void ZFB_Net_HandlePdpMsg(MMIPDP_CNF_INFO_T *msg_ptr)
{
    if(PNULL == msg_ptr)
    {
        return;
    }
    
    ZFB_NET_TRACE("ZFB NET HandlePdpMsg msg_id=%d,result=%d",msg_ptr->msg_id,msg_ptr->result);
    
    switch(msg_ptr->msg_id) 
    {
    case MMIPDP_ACTIVE_CNF:     //PDP激活成功
        ZFB_Net_StopPdpActiveTimer();
        
        if(MMIPDP_RESULT_SUCC == msg_ptr->result)
        {
            s_zfb_net_id = msg_ptr->nsapi;
            ZFB_NET_TRACE("ZFB NET MMIPDP_ACTIVE_CNF OK net_id=0x%x",s_zfb_net_id);
            MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_NET_ACTIVE_SUCCESS,PNULL,0);
        }
        else if (MMIPDP_RESULT_FAIL == msg_ptr->result)
        {
            ZFB_NET_TRACE("ZFB NET MMIPDP_ACTIVE_CNF FAIL");
            ZFB_Net_PdpDeactive();
            MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_NET_ACTIVE_FAIL,PNULL,0);
        }
        else 
        {
            ZFB_NET_TRACE("ZFB NET MMIPDP_ACTIVE_CNF FAIL Other");
            ZFB_Net_PdpDeactive();
            MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_NET_ACTIVE_FAIL,PNULL,0);
        }        
        break;
        
    case MMIPDP_DEACTIVE_CNF:   //PDP去激活成功。
            ZFB_NET_TRACE("ZFB NET MMIPDP_DEACTIVE_CNF");
            s_zfb_net_id = 0;
            MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_NET_CONNET_FAIL,PNULL,0);
        break;
        
    case MMIPDP_DEACTIVE_IND:   //PDP被网络端去激活。
            ZFB_NET_TRACE("ZFB NET MMIPDP_DEACTIVE_IND");
            s_zfb_net_id = 0;
            ZFB_Net_PdpDeactive();
            MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_NET_CONNET_FAIL,PNULL,0);
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
LOCAL BOOLEAN  ZFB_Net_PDPTry(MN_DUAL_SYS_E sim_sys)
{
#if defined(MMI_GPRS_SUPPORT)
    BOOLEAN                 return_val      = FALSE;
    MMIPDP_ACTIVE_INFO_T    active_info     = {0};
    MMICONNECTION_APNTYPEINDEX_T *apn_idx_arr;
    MMICONNECTION_LINKSETTING_DETAIL_T* linksetting = PNULL;

    ZFB_NET_TRACE("ZFB NET ConnectPDP");
    if(MMIAPISET_GetFlyMode())
    {
        SCI_TraceLow("ZFB NET ConnectPDP [Fly Mode]");
        return FALSE;
    }
    if (FALSE == MMIZFB_Net_GPRS_IsOn())
    {
        SCI_TraceLow("ZFB NET ConnectPDP [DATA ERROR]");
        return FALSE;
    }
    if (FALSE == MMIAPIPHONE_GetDataServiceSIM(&sim_sys))
    {
        SCI_TraceLow("ZFB NET ConnectPDP [SIM ERROR]");
        return FALSE;
    }
    if(sim_sys != s_zfb_net_set_sys)
    {
        MMIZFB_Net_SetActiveSys(sim_sys);
    }
    active_info.app_handler         = MMI_MODULE_ZFB;
    active_info.dual_sys            = sim_sys;
    active_info.apn_ptr             = NULL;
    active_info.user_name_ptr       = NULL;
    active_info.psw_ptr             = NULL;
    active_info.priority            = 3;
    active_info.ps_service_rat      = MN_TD_PREFER;
    active_info.ps_interface        = MMIPDP_INTERFACE_GPRS;
    active_info.handle_msg_callback = ZFB_Net_HandlePdpMsg;
    active_info.ps_service_type     = BROWSER_E;
    active_info.storage             = MN_GPRS_STORAGE_ALL;

    apn_idx_arr = MMIAPICONNECTION_GetApnTypeIndex(sim_sys);
    if (apn_idx_arr == NULL)
    {
        ZFB_NET_TRACE("ZFB NET ConnectPDP [APN IDX ERROR]");
        return FALSE;
    }
    ZFB_NET_TRACE("ZFB NET  ConnectPDP [sim = %d, Net Setting = %d]", sim_sys, apn_idx_arr->internet_index[sim_sys].index);
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
            ZFB_NET_TRACE("ZFB NET PdpActive GPRS sim_sys=%d",sim_sys);
            ZFB_Net_StartPdpActiveTimer();
            return_val = TRUE;
        }
    }
    return return_val;
#endif
    return FALSE;
}

LOCAL BOOLEAN ZFB_Net_PdpActive(void)
{
    BOOLEAN                 return_val      = FALSE;
    MN_DUAL_SYS_E           dual_sys        = MN_DUAL_SYS_MAX;
    MMIPDP_ACTIVE_INFO_T    active_info     = {0};
    
    ZFB_NET_TRACE("ZFB NET PdpActive Start is_activing=%d,is_ok=%d",s_zfb_net_is_activing,s_zfb_net_pdp_state_is_ok);
    if(s_zfb_net_is_activing)
    {
        s_zfb_net_need_active_close = FALSE;
        return TRUE;
    }
    s_zfb_net_is_activing = TRUE;
    if(s_zfb_net_pdp_state_is_ok)
    {
        MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_NET_ACTIVE_SUCCESS,PNULL,0);
        return TRUE;
    }

#if 0 //def WIFI_SUPPORT    
    //优先wifi
    if(s_zfb_net_set_sys == MN_DUAL_SYS_MAX && MMIWIFI_STATUS_CONNECTED == MMIAPIWIFI_GetStatus())
    {
        active_info.app_handler         = MMI_MODULE_ZFB;
        active_info.handle_msg_callback = ZFB_Net_HandlePdpMsg;
        active_info.ps_interface        = MMIPDP_INTERFACE_WIFI;
        if(MMIAPIPDP_Active(&active_info))
        {
            ZFB_NET_TRACE("ZFB NET PdpActive OK WIFI");
            ZFB_Net_StartPdpActiveTimer();
            return_val = TRUE;
        }
        else
        {
            ZFB_NET_TRACE("ZFB NET PdpActive ERR WIFI");
            MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_NET_ACTIVE_FAIL,PNULL,0);
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
            ZFB_NET_TRACE("ZFB NET PdpActive ERR NO SIM");
            MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_NET_ACTIVE_FAIL,PNULL,0);
            return FALSE;
        }
        
        if(s_zfb_net_is_init == FALSE)
        {
            ZFB_NET_TRACE("ZFB NET PdpActive ERR NO INIT");
            MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_NET_ACTIVE_FAIL,PNULL,0);
            return FALSE;
        }


        if(s_zfb_net_set_sys < MN_DUAL_SYS_MAX)
        {
            if(MMIAPIPHONE_IsSimAvailable(s_zfb_net_set_sys))
            {
                s_zfb_net_cur_sys = s_zfb_net_set_sys;
            }
            else
            {
                ZFB_NET_TRACE("ZFB NET PdpActive ERR set_sys(%d) NO SIM",s_zfb_net_set_sys);
                MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_NET_ACTIVE_FAIL,PNULL,0);
                return FALSE;
            }
        }
        else
        {
            s_zfb_net_cur_sys = sim_ok;
        }

        return_val = ZFB_Net_PDPTry(s_zfb_net_cur_sys);//第一张有效卡
        if(FALSE == return_val)
        {
            ZFB_NET_TRACE("ZFB NET PdpActive ERR cur_sys(%d/%d),",s_zfb_net_cur_sys,sim_ok_num);
            #if 0
            if(sim_ok_num > 1 && s_zfb_net_set_sys == MN_DUAL_SYS_MAX)
            {
                if(0 != s_zfb_net_pdp_retry_timer_id)
                {
                    MMK_StopTimer(s_zfb_net_pdp_retry_timer_id);
                    s_zfb_net_pdp_retry_timer_id = 0;
                }
                s_zfb_net_pdp_retry_timer_id = MMK_CreateTimerCallback(
                                                                                                ZFB_PDP_RETRY_TIMEOUT, 
                                                                                                ZFB_Net_HandlePdpRetryTimer, 
                                                                                                PNULL, 
                                                                                                FALSE
                                                                                                );
            }
            else
            #endif
            {
                MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_NET_ACTIVE_FAIL,PNULL,0);
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
LOCAL BOOLEAN ZFB_Net_PdpDeactive(void)
{
    BOOLEAN return_val = FALSE;
    
    return_val = MMIAPIPDP_Deactive(MMI_MODULE_ZFB);

    return return_val;
}
PUBLIC BOOLEAN ZFB_Net_IsAllow(void)
{
    if(s_zfb_net_is_init == FALSE)
    {
        ZFB_NET_TRACE("ZFB_Net_IsAllow FASE NoInit");
        return FALSE;
    }
    if(MMIAPISET_GetFlyMode())
    {
        ZFB_NET_TRACE("ZFB_Net_IsAllow FASE In Fly");
        return FALSE;
    }
    if(MMIZFB_Net_GPRS_IsOn() == FALSE)
    {
        ZFB_NET_TRACE("ZFB_Net_IsAllow FASE  GPRS OFF");
        return FALSE;
    }
    #ifndef WIN32
    if(0 == MNPHONE_GetAttState(MN_DUAL_SYS_1))
    {
        ZFB_NET_TRACE("ZFB_Net_IsAllow FASE  ATT");
        return FALSE;
    }
    #endif
    
    if(MMIZFB_Net_IsValid() == FALSE)
    {
		// 如果是默认IMEI不能进行初使化
        ZFB_NET_TRACE("MMIZFB_Net_IsValid FASE  No IMEI");
        return FALSE;
    }
    return TRUE;
}

LOCAL BOOLEAN ZFB_Net_Init(void)
{
    MN_DUAL_SYS_E dual_sys = MMIZFB_Net_GetActiveSys();
    ZFB_NET_TRACE("ZFB_Net_Init is_init=%d, dual_sys=%d",s_zfb_net_is_init,dual_sys);
    if(s_zfb_net_is_init == FALSE)
    {
        MMIZFB_Net_SetActiveSys(dual_sys);
        ZFB_NET_TRACE("ZFB_Net_Init imei=%s,imsi=%s,iccid=%s",g_zfb_phone_imei,g_zfb_sim_imsi,g_zfb_sim_iccid);
	if(g_zfb_mutexPtr == NULL) {
		g_zfb_mutexPtr = SCI_CreateMutex("zfbnet-mutex", SCI_INHERIT);
	}
        //MMIZFB_Pay_Init(); //del by bao 支付宝开机不初使化申请ID,使的时候再去申请
        s_zfb_net_is_init = TRUE;
    }
    return TRUE;
}

LOCAL BOOLEAN ZFB_Net_Open(void)
{
    ZFB_NET_TRACE("ZFB_Net_Open state=%d,is_activing=%d,is_closing=%d",s_zfb_net_pdp_state_is_ok,s_zfb_net_is_activing,s_zfb_net_is_closing);
    if(s_zfb_net_is_closing)
    {
        s_zfb_net_need_close_active = TRUE;
    }
    else
    {
        ZFB_Net_PdpActive();
    }
    return TRUE;
}

LOCAL BOOLEAN ZFB_Net_CloseEx(void)
{
    if(s_zfb_net_is_closing)
    {
        s_zfb_net_need_close_active = FALSE;
        return TRUE;
    }
    s_zfb_net_is_closing = TRUE;
    if(s_zfb_net_pdp_state_is_ok)
    {
        if(FALSE == ZFB_Net_PdpDeactive())
        {
            MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_NET_CONNET_FAIL,PNULL,0);
        }
    }
    else
    {
        ZFB_Net_PdpDeactive();
        MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_NET_CONNET_FAIL,PNULL,0);
    }
}

LOCAL BOOLEAN ZFB_Net_Close(void)
{
    ZFB_NET_TRACE("ZFB_Net_Close state=%d,is_activing=%d,is_closing=%d",s_zfb_net_pdp_state_is_ok,s_zfb_net_is_activing,s_zfb_net_is_closing);
        if(s_zfb_net_is_activing)
        {
            s_zfb_net_need_active_close = TRUE;
        }
        else
        {
            ZFB_Net_CloseEx();
        }
    return TRUE;
}

LOCAL BOOLEAN ZFB_Net_PdpStateChange(BOOLEAN is_ok)
{
    ZFB_NET_TRACE("ZFB NET PDPStateChange %d",is_ok);
    if(is_ok)
    {
        if(s_zfb_net_need_active_close == FALSE)
        {
        }
    }

    return TRUE;
}

LOCAL void ZFB_Net_PdpStateUpdate(BOOLEAN is_ok,BOOLEAN is_active)
{
    ZFB_NET_TRACE("ZFB_Net_PdpStateUpdate is_ok=%d,is_active=%d,activing=%d,closing=%d",is_ok,is_active,s_zfb_net_is_activing,s_zfb_net_is_closing);
    if(s_zfb_net_pdp_state_is_ok != is_ok)
    {
        s_zfb_net_pdp_state_is_ok = is_ok;
        ZFB_Net_PdpStateChange(is_ok);
    }
    
    if(is_active)
    {
        if(g_zfb_net_cur_data != NULL && g_zfb_net_cur_data->cb_handle != NULL)
        {
            uint32 cur_param = g_zfb_net_cur_data->type;
            g_zfb_net_cur_data->cb_handle(is_ok,(uint32)cur_param);
        }
        
        if(s_zfb_net_is_activing)
        {
            s_zfb_net_is_activing = FALSE;
            if(s_zfb_net_need_active_close)
            {
                ZFB_NET_TRACE("ZFB_Net_PdpStateUpdate Active Close");
                s_zfb_net_need_active_close = FALSE;
                MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_NET_CLOSE,PNULL,0);
            }
        }
        ZFB_NET_DelCurData();
        if(ZFB_NET_Link_Count())
        {
            ZFB_NET_PostEvtTo_APP(ZFB_APP_SIGNAL_NET_OPEN_START,PNULL,0);
        }

    }
    else
    {   
        s_zfb_net_is_activing = FALSE;
        if(s_zfb_net_is_closing)
        {
            s_zfb_net_is_closing = FALSE;
            if(s_zfb_net_need_close_active)
            {
                ZFB_NET_TRACE("ZFB_Net_PdpStateUpdate Close Active");
                s_zfb_net_need_close_active = FALSE;
                MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_NET_OPEN,PNULL,0);
            }
        }
    }
    return;
}

LOCAL void ZFB_Net_HandleResetTimer(
                                            uint8 timer_id,
                                            uint32 param
                                            )
{
    ZFB_NET_TRACE("ZFB_Net_HandleResetTimer");
    MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_NET_RESET,PNULL,0);
}

LOCAL void ZFB_Net_StartResetTimer(void)
{
    if(0 != s_zfb_net_reset_timer_id)
    {
        MMK_StopTimer(s_zfb_net_reset_timer_id);
        s_zfb_net_reset_timer_id = 0;
    }
    
    s_zfb_net_reset_timer_id = MMK_CreateTimerCallback(ZFB_NET_RESET_TIMEOUT, 
                                                                        ZFB_Net_HandleResetTimer, 
                                                                        PNULL, 
                                                                        FALSE);
}

LOCAL void ZFB_Net_StopResetTimer(void)
{
    if(0 != s_zfb_net_reset_timer_id)
    {
        MMK_StopTimer(s_zfb_net_reset_timer_id);
        s_zfb_net_reset_timer_id = 0;
    }
}

PUBLIC uint32 MMIZFB_Net_GetNetID(void)
{
    return s_zfb_net_id;
}

PUBLIC BOOLEAN MMIZFB_Net_PdpStateGet(void)
{
    return s_zfb_net_pdp_state_is_ok;
}

PUBLIC BOOLEAN MMIZFB_Net_IsActiving(void)
{
    return s_zfb_net_is_activing;
}

PUBLIC BOOLEAN MMIZFB_Net_IsClosing(void)
{
    return s_zfb_net_is_closing;
}


PUBLIC BOOLEAN MMIZFB_Net_IsInit(void)
{
    return s_zfb_net_is_init;
}

PUBLIC BOOLEAN MMIZFB_Net_Init(void)
{
    if(MMIZFB_Net_IsValid())
    {
        MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_NET_INIT,PNULL,0);
    }
    return TRUE;
}

PUBLIC BOOLEAN MMIZFB_Net_Reset(void)
{
    ZFB_LOG("MMIZFB_Net_Reset");
    if(FALSE == MMIZFB_Net_GPRS_IsOn() || MMIAPISET_GetFlyMode() || s_zfb_net_is_init == FALSE)
    {
        ZFB_LOG("MMIZFB_Net_Reset In Fly");
        return FALSE;
    }
    if(g_zfb_mutexPtr != NULL)
    {
        SCI_GetMutex(g_zfb_mutexPtr, SCI_WAIT_FOREVER);
    }
    MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_NET_RESET,PNULL,0);
    if(g_zfb_mutexPtr != NULL)
    {
        SCI_PutMutex(g_zfb_mutexPtr);
    }
    return TRUE;
}

PUBLIC BOOLEAN MMIZFB_Net_Reset_Check(void)
{
    if(s_zfb_net_is_init && s_zfb_net_id == 0)
    {
        ZFB_LOG("MMIZFB_Net_Reset_Check");
        MMIZFB_Net_Reset();
    }
    
    return TRUE;
}
PUBLIC BOOLEAN MMIZFB_Net_Open(void)
{
    MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_NET_OPEN,PNULL,0);
    return TRUE;
}

PUBLIC BOOLEAN MMIZFB_Net_Close(void)
{
    MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_NET_CLOSE,PNULL,0);
    return TRUE;
}

LOCAL MN_DUAL_SYS_E MMIZFB_Net_NV_GetActiveSys(void)
{
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    uint8 net_sys = ZFB_PDP_DEFAULT_SIM_SYS;
    MMINV_READ(MMI_ZFB_NV_NET_SYS,&net_sys,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        //自动识别
        net_sys = ZFB_PDP_DEFAULT_SIM_SYS;
        MMINV_WRITE(MMI_ZFB_NV_NET_SYS,&net_sys);
    }
    s_zfb_net_set_sys = net_sys;
    return net_sys;
}

LOCAL BOOLEAN  MMIZFB_Net_NV_SetActiveSys(MN_DUAL_SYS_E set_sys) 
{
    uint8 net_sys = set_sys;
    if(s_zfb_net_set_sys != set_sys)
    {
        MMINV_WRITE(MMI_ZFB_NV_NET_SYS,&net_sys);
        s_zfb_net_set_sys = set_sys;
        return TRUE;
    }
    return FALSE;
}

PUBLIC MN_DUAL_SYS_E MMIZFB_Net_GetActiveSys(void)
{
    return s_zfb_net_set_sys;
}

PUBLIC void MMIZFB_Net_SetActiveSys(MN_DUAL_SYS_E set_sys) 
{
    ZFB_NET_TRACE("MMIZFB_Net_SetActiveSys set_sys=%d",set_sys);
    SCI_MEMSET(g_zfb_sim_imsi,0,ZFB_IMSI_LEN+1);
    SCI_MEMSET(g_zfb_sim_iccid,0,ZFB_ICCID_LEN+1);
    MMIZFB_Net_NV_SetActiveSys(set_sys);
    MMIZFB_Get_SIM_IMSI(set_sys,g_zfb_sim_imsi);
    MMIZFB_Get_SIM_ICCID(set_sys,g_zfb_sim_iccid);
    #ifdef ZFB_NET_FIX_IMEI
        g_zfb_phone_imei = g_zfb_phone_imei_1;
    #else
    if(set_sys == MN_DUAL_SYS_1)
    {
        g_zfb_phone_imei = g_zfb_phone_imei_1;
    }
    else
    {
        g_zfb_phone_imei = g_zfb_phone_imei_2;
    }
    #endif
    return;
}

LOCAL BOOLEAN  MMIZFB_Net_Handle_Init(DPARAM param)
{
    ZFB_NET_TRACE("MMIZFB_Net_Handle_Init");
    ZFB_Net_Init();
    return TRUE;
}

LOCAL BOOLEAN  MMIZFB_Net_Handle_OpenAdd(DPARAM param)
{
    int res = 0;
    ZFB_NET_DATA_SIG_T * pp_getdata = (ZFB_NET_DATA_SIG_T *)(param);
    ZFB_NET_DATA_T * p_data = pp_getdata->p_data;
    ZFB_NET_LINK_DATA_T * pData = PNULL;

    ZFB_NET_TRACE("MMIZFB_Net_Handle_OpenAdd");
    if(p_data != NULL)
    {
        pData = (ZFB_NET_LINK_DATA_T *)(p_data->data_p);
    }
    if(pData != NULL)
    {
        if(ZFB_NET_Link_AddData(pData))
        {
            MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_NET_OPEN_START,PNULL,0);
        }
        else
        {
            SCI_FREE(pData);
            MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_NET_ACTIVE_FAIL,PNULL,0);
        }
    }
    if(p_data != NULL)
    {
        SCI_FREE(p_data);
    }
    return res;
}

LOCAL BOOLEAN  MMIZFB_Net_Handle_OpenStart(DPARAM param)
{
    ZFB_NET_TRACE("MMIZFB_Net_Handle_OpenStart");
    if(s_zfb_net_is_activing)
    {
        ZFB_NET_TRACE("MMIZFB_Net_Handle_OpenStart IsActiving...");
        return 0;
    }
    
    ZFB_NET_DelCurData();
    if(ZFB_NET_Link_GetMaxPri(&g_zfb_net_cur_data))
    {
        ZFB_Net_Open();
    }
    return TRUE;
}

LOCAL BOOLEAN  MMIZFB_Net_Handle_Open(DPARAM param)
{
    ZFB_Net_Open();
    return TRUE;
}

LOCAL BOOLEAN  MMIZFB_Net_Handle_Close(DPARAM param)
{
    ZFB_NET_TRACE("MMIZFB_Net_Handle_Close");
    ZFB_Net_Close();
    return TRUE;
}

LOCAL BOOLEAN  MMIZFB_Net_Handle_Reset(DPARAM param)
{
    ZFB_NET_TRACE("MMIZFB_Net_Handle_Reset");
    MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_NET_CLOSE,PNULL,0);
    MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_NET_OPEN,PNULL,0);
    return TRUE;
}

LOCAL BOOLEAN  MMIZFB_Net_Handle_PDPActiveOK(DPARAM param)
{
    ZFB_NET_TRACE("MMIZFB_Net_Handle_PDPActiveOK net_is_activing=%d",s_zfb_net_is_activing);
    ZFB_Net_PdpStateUpdate(TRUE,TRUE);
    return TRUE;
}

LOCAL BOOLEAN  MMIZFB_Net_Handle_PDPActiveFail(DPARAM param)
{
    ZFB_NET_TRACE("MMIZFB_Net_Handle_PDPActiveFail net_is_activing=%d",s_zfb_net_is_activing);
    ZFB_Net_PdpStateUpdate(FALSE,TRUE);
    return TRUE;
}

LOCAL BOOLEAN  MMIZFB_Net_Handle_PDPRespond(DPARAM param)
{
    ZFB_NET_TRACE("ZFB NET Handle_PDP_RSP net_is_activing=%d",s_zfb_net_is_activing);
    ZFB_Net_PdpStateUpdate(FALSE,FALSE);
    return TRUE;
}

PUBLIC MMI_RESULT_E MMIZFB_Net_Handle_AppMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
    MMI_RESULT_E res = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case ZFB_APP_SIGNAL_NET_INIT:
            MMIZFB_Net_Handle_Init(param);
        break;
        
    case ZFB_APP_SIGNAL_NET_OPEN_ADD:
            MMIZFB_Net_Handle_OpenAdd(param);
        break;
        
    case ZFB_APP_SIGNAL_NET_OPEN_START:
            MMIZFB_Net_Handle_OpenStart(param);
        break;
        
    case ZFB_APP_SIGNAL_NET_OPEN:
            MMIZFB_Net_Handle_Open(param);
        break;
    case ZFB_APP_SIGNAL_NET_CLOSE:
            MMIZFB_Net_Handle_Close(param);
        break;
        
    case ZFB_APP_SIGNAL_NET_RESET:
            MMIZFB_Net_Handle_Reset(param);
        break;
        
    case ZFB_APP_SIGNAL_NET_ACTIVE_SUCCESS:
            MMIZFB_Net_Handle_PDPActiveOK(param);
        break;
        
    case ZFB_APP_SIGNAL_NET_ACTIVE_FAIL:
            MMIZFB_Net_Handle_PDPActiveFail(param);
        break;
        
    case ZFB_APP_SIGNAL_NET_CONNET_FAIL:
            MMIZFB_Net_Handle_PDPRespond(param);
        break;
        
    default:
            res = MMI_RESULT_FALSE;
        break;
    }
    return res;
}

BOOLEAN MMIZFB_Net_GPRS_IsOn(void)
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

PUBLIC BOOLEAN MMIZFB_Net_IsValid(void)
{
    BOOLEAN res = TRUE;
    //如果是默认IMEI，不允许支付宝登陆
    if(0 == strncmp(g_zfb_phone_imei_1,"352273017386340",15))
    {
        res = FALSE;
    }
    return res;
}

PUBLIC BOOLEAN MMIZFB_Net_APP_Init(void)
{
    MMIZFB_Net_NV_GetActiveSys();
    MMIZFB_Net_GetIMEI(MN_DUAL_SYS_1,g_zfb_phone_imei_1);
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    MMIZFB_Net_GetIMEI(MN_DUAL_SYS_2,g_zfb_phone_imei_2);
#endif
    ZFB_NET_TRACE("ZFB APP NET Init imei=%s, imei2=%s",g_zfb_phone_imei,g_zfb_phone_imei_2);

    return TRUE;
}

uint32 MMIZFB_Net_API_Open(uint8 type,uint8 priority,uint8 repeat_times,uint32 timeout,ZFBNETCBHANDLER cb_handle)
{
    ZFB_NET_LINK_DATA_T * cur_data = NULL;
#ifdef WIN32
    if(cb_handle != PNULL)
    {
        cb_handle(1,PNULL);
    }
    return 12345;
#endif
    if(FALSE == ZFB_Net_IsAllow())
    {
        return 0;
    }

    cur_data=(ZFB_NET_LINK_DATA_T *)SCI_ALLOC_APPZ(sizeof(ZFB_NET_LINK_DATA_T));
    if(cur_data == NULL)
    {
        ZFB_NET_TRACE("MMIZFB_Net_API_Open ERR Malloc");
        return 0;
    }

    cur_data->type = type;
    cur_data->repeat_times = repeat_times;
    cur_data->timeout  =timeout;
    cur_data->priority = priority;
    cur_data->cb_handle =cb_handle;
    ZFB_NET_PostEvtTo_APP(ZFB_APP_SIGNAL_NET_OPEN_ADD,cur_data,type);
    return (uint32)cur_data;
}

BOOLEAN MMIZFB_Net_API_Close(void)
{
    if(s_zfb_net_pdp_state_is_ok == TRUE)
    {
        MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_NET_CLOSE,PNULL,0);
        return TRUE;
    }
    return FALSE;
}
#endif
