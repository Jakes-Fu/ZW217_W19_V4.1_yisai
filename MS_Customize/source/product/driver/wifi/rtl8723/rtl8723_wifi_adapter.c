#if 1

//#include "os.h"
#include "ms_customize_trc.h"
#include "usblist.h"
#include "rtl8723_wifi_adapter.h"

#include "os_api.h"
#include "tb_drv.h"
#include "tb_comm.h"
//#include "os_function_if.h"
#include "in_message.h"
#include "tcpip_types.h"
#include "tcpip_api.h"
#include "app_tcp_if.h"
#include "wifisupp_api.h"
#include "wifi_drv.h"

#if defined (PLATFORM_SC6530) || defined(PLATFORM_SC6531) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
#include "priority_system.h"
#endif


typedef struct
{
    int(*wifi_support_iperf_func)(void);
} RTL_PRIV_IPERF_T;

int SPI_CARD_DEFAULT_CLOCK = 24000000;
extern RTL_PRIV_OPS_T rtw_priv_ops;
extern RTL_PRIV_IPERF_T rtw_priv_iperf_ops;
extern int32 SCI_MEMCMP(void * _BUF1, void * _BUF2, uint32 _SIZE);

RTL_PRIV_OPS_T *p_rtl8723_priv_ops = &rtw_priv_ops;
RTL_PRIV_IPERF_T *p_rtl8723_priv_iperf_ops = &rtw_priv_iperf_ops;

/****************************************************************************/
//Description : to send signal to app
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC void SUPPMAIN_NotifyApp(
                            uint32      msg_id, 
                            void        *msg_info_ptr
                             );

/**---------------------------------------------------------------------------*
 **                         Time adapter functions                                                                           *
 **---------------------------------------------------------------------------*/

void rtlwifi_sleep_time(os_time_t sec, os_time_t usec)
{
#if 0
    if (sec || usec)
        SCI_Sleep(sec*1000+usec/1000);
#endif
}

int rtl_get_time(struct rtl_os_time *t)
{
    unsigned int res = 0;

    res = SCI_GetTickCount();
    t->sec = res/1000;
    t->usec = (res%1000)*1000;
   
    return res;
}

unsigned int rtl_getSystemTickCount(void)
{
    return SCI_GetTickCount();
}

/**---------------------------------------------------------------------------*
 **                         Memory adapter  functions                                                                     *
 **---------------------------------------------------------------------------*/

void * rtl_zalloc(unsigned int size)
{
    void* ptr = 0;
    ptr = SCI_ALLOCA(size);
    if (ptr != 0)
    {
        SCI_MEMSET(ptr,0,(size_t)size);
    }
    return ptr;
}


void * rtl_malloc(unsigned int size)
{
    void* ptr = 0;
    ptr = SCI_ALLOCA(size);
    if (ptr != 0)
    {
        SCI_MEMSET(ptr,0,(size_t)size);
    }
    return ptr;
}


void * rtl_realloc(void *ptr, unsigned int size)
{
       void *temp = SCI_ALLOCA(size);

    if (temp != 0)
    {
        SCI_MEMCPY(temp, ptr, (size_t)size);
    }
    
    SCI_FREE(ptr);

    return temp;
}

void rtl_free(void *ptr)
{
     if(ptr == NULL)
        return;
     SCI_FREE(ptr);
}


void * rtl_memcpy(void *dest, const void *src, size_t n)
{
    SCI_MEMCPY(dest,src,n);

    return dest;
}

int rtl_memcmp(const void*  dest,const void*  src,size_t  count)   
 {   

    return memcmp(dest,src,count);
}   


int rtl_memcpy_to_user(void *dest, void *src, unsigned int count)
{
#if 0
    if(count)
    {
        SCI_MEMCPY(dest,src,count);
        return 0;
    }
#endif
    return 1;
}

void *rtl_malloc_debug(unsigned int size,const char * file,unsigned int line)
{
    return 0;
    //return SCI_Malloc(size,file,line);
}


/**---------------------------------------------------------------------------*
 **                         OS   adapter functions                                                                           *
 **---------------------------------------------------------------------------*/


void rtl_sleep_ms(unsigned int msecs)
{
    SCI_Sleep(msecs);
}


extern uint32 OS_TickDelay(uint32 ticks);
void rtl_delay_ms(unsigned int time)
{
        if (SCI_InThreadContext()) 
        {
            SCI_Sleep(time);
        } 
        else 
        {
            OS_TickDelay(time);
        }
}

void rtl_delay_us(unsigned int us)
{
   uint32 l,m,k;
   k = CHIP_GetArmClk()/ARM_CLK_13M;
   for (l = 0; l < us; l++) 
   {
     m = k; 
     while(m--){ };
   }
}
 
int rtl_memset(void *dest, int value, unsigned int len)
{
    if (dest != 0 && len != 0)
    {
        SCI_MEMSET(dest,value,(size_t)len);
    }
    return TRUE;
}

void rtl_init_listhead(struct list_head *list)
{
    _INIT_LIST_HEAD(list);
}

uint32 rtl_is_list_empty(struct list_head *phead)
{
    if (list_empty(phead))
        return TRUE;
    else
        return FALSE;
}

void rtl_list_insert_tail(struct list_head *plist, struct list_head *phead)
{
    list_add_tail(plist, phead);
}

void rtl_list_delete(struct list_head *plist)
{
    list_del_init(plist);
}

RTL_SCI_SEMAPHORE_PTR rtl_createSemaphore(char* pName, unsigned int count)
{
    return SCI_CreateSemaphore(pName,count );

}

unsigned int rtl_getSemaphore(RTL_SCI_SEMAPHORE_PTR pSem,unsigned int option)
{
        if (pSem == NULL)
            return SCI_ERROR;
    return SCI_GetSemaphore ( pSem, option);
}

unsigned int rtl_putSemaphore(RTL_SCI_SEMAPHORE_PTR pSem)
{
    if (pSem == NULL)
        return SCI_ERROR;
    
    return SCI_PutSemaphore(pSem);
}

void rtl_freeSemaphore(RTL_SCI_SEMAPHORE_PTR pSem)
{
    if(pSem == NULL)
        return;
    SCI_DeleteSemaphore(pSem);
}

/* based on spectrum tcp/ip doc, tcp/ip priority is 18, so we should
  * higher priority in driver for some thread */
RTL_BLOCK_ID rtl_createThread(
    const char  *thread_name,           
    const char  *queue_name,            
    void    (*entry)(uint32, void*),    
    uint32  argc,                       
    void    *argv,                      
    uint32  stack_size,                 
    uint32  queue_num,   
    RTL_ADAP_TASK_PRIO_E  para_priority,    
    uint32  preempt,     
    uint32  auto_start   
    )
{
    uint32 priority = 0;
    
    switch(para_priority)
    {
     case RTL_ADAP_TASK_PRIO_1:
#if defined (PLATFORM_SC6530) || defined(PLATFORM_SC6531) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
     priority = PRI_RTL_HIGH_TASK;
#else
     priority = 17;
#endif        
     break;

     case RTL_ADAP_TASK_PRIO_2:
#if defined (PLATFORM_SC6530) || defined(PLATFORM_SC6531) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
     priority = PRI_RTL_NORMAL_TASK; //PRI_RTL_NORMAL_TASK;
#else
     priority = 26;
#endif        
     break;

     case RTL_ADAP_TASK_PRIO_3:
        priority = 27;
     break;

     case RTL_ADAP_TASK_PRIO_4:
        priority = 29;
     break;

        default:
        priority = 29;
        break;
    }

   SCI_TRACE_LOW("&&&&&&&&rtl_createThread(): priority is %d\n",priority);
   return SCI_CreateAppThread(thread_name,queue_name,entry,argc,argv,stack_size,queue_num,priority,preempt,auto_start);
}

void rtl_deleteThread(BLOCK_ID thread_id)
{
    if (thread_id != SCI_INVALID_BLOCK_ID)
    SCI_DeleteThread(thread_id);
}
void rtl_exitThread(void)
{
    SCI_ThreadExit();
}

unsigned int rtl_suspendThread(RTL_BLOCK_ID    threadID)
{
    return SCI_SuspendThread(threadID);
}

unsigned int rtl_resumeThread(RTL_BLOCK_ID    threadID)
{
    return SCI_ResumeThread(threadID);
}

rtl_xSignalHeader rtl_getSignal(RTL_BLOCK_ID threadId)
{
    return 0;
    //return (para_xSignalHeader)SCI_GetSignal(threadId);

}

unsigned int rtl_sendSignal(rtl_xSignalHeader   pSignal, RTL_BLOCK_ID threadId)
{
        return 0;
#if 0
    if (SCI_SUCCESS == SCI_IsThreadQueueAvilable(threadId))
        return SCI_SendSignal(pSignal,threadId);
    else
        return SCI_ERROR;
#endif
}

void *rtl_createTimer(
    const char  *timer_name,    // Name string of the timer
    RTL_TIMER_FUN   timer_fun,      // Timer callback function
    uint32      input,          // Input value for timer callback function 
    uint32      timer_expire,   // Specifies the timer expire value in     
                                // milliseconds.                           
    uint32      auto_activate   // Option to check if auto active the timer
)
{
    return SCI_CreateTimer(timer_name,timer_fun,input,timer_expire,auto_activate);
}

uint32 rtl_changeTimer(RTL_TIMER_PTR timer_ptr,RTL_TIMER_FUN timer_fun,uint32 timer_expire) 
{
    if (timer_ptr)
        return SCI_ChangeTimer(timer_ptr,timer_fun,timer_expire);
    else
        return FALSE;
}

uint32 rtl_activeTimer(RTL_TIMER_PTR timer_ptr)
{
    if (timer_ptr)
        return SCI_ActiveTimer(timer_ptr);
    else
        return FALSE;
}

unsigned int rtl_deactiveTimer(RTL_TIMER_PTR timer_ptr)
{
    if (timer_ptr)
        return SCI_DeactiveTimer(timer_ptr);
    else
        return FALSE;
}

uint32 rtl_deleteTimer(RTL_TIMER_PTR timer_ptr)
{
    if (timer_ptr)
        return SCI_DeleteTimer(timer_ptr);
    else
        return SCI_PTR_ERROR;
}

int RTL_LOGGING_DEFAULT_LEVEL = RTL_LOGGING_LEVEL_NORMAL;

RTL_SCI_MUTEX_PTR rtl_createMutex(const char* pName,unsigned int priority)
{
    return SCI_CreateMutex(pName,priority);
}

unsigned int rtl_getMutex(RTL_SCI_MUTEX_PTR pMutex,unsigned int option )
{
    if (SCI_InThreadContext())
        return SCI_GetMutex(pMutex,option);
    else
        return TRUE;
}

unsigned int rtl_putMutex(RTL_SCI_MUTEX_PTR pMutex)
{
    if (SCI_InThreadContext())
        return SCI_PutMutex(pMutex);
    else
        return TRUE;
}

unsigned int rtl_deleteMutex(RTL_SCI_MUTEX_PTR pMutex)
{
    if(pMutex == NULL)
        return SCI_SUCCESS;
    return SCI_DeleteMutex(pMutex);
}


RTL_EVENT_GROUP_PTR rtl_createEvent(char *pName)
{
    return SCI_CreateEvent(pName);
}

uint32 rtl_setEvent(SCI_EVENT_GROUP_PTR pGroup, uint32 flags,uint32 option )
{
    if (pGroup == NULL)
        return SCI_ERROR;
    
    return SCI_SetEvent(pGroup,flags,option);
}

uint32 rtl_getEvent(SCI_EVENT_GROUP_PTR pGroup, uint32 flags,uint32 getOption,uint32 *pActualFlags, uint32 waitOption)
{
    if (pGroup == NULL)
        return SCI_ERROR;
    
    return SCI_GetEvent(pGroup,flags,getOption,pActualFlags,waitOption);
}

uint32 rtl_deleteEvent(SCI_EVENT_GROUP_PTR pGroup)
{
    if(pGroup == NULL)
        return SCI_SUCCESS;
    return SCI_DeleteEvent(pGroup);
}

/**---------------------------------------------------------------------------*
 **                         TCPIP adapter functions                                                                         *
 **---------------------------------------------------------------------------*/

/****************************************************************************/
//Description : wifi on
//Global resource dependence :
//Author: juan.zhang
//Note:
/****************************************************************************/
BOOLEAN rtl8723_SetMAC(unsigned char *maddr)
{
    uint32 ret;
    WIFI_ADDR_T mac = {0};
    unsigned char *pmac;
    unsigned char mac_zero[WIFI_NV_ADDR_LEN] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    
    ret = WIFI_GetNvParam (&mac, NULL);
    pmac = mac.address;
    if (ret != WIFI_RET_OK || pmac[0] == 0xff || rtl_memcmp (pmac, mac_zero, WIFI_NV_ADDR_LEN) == 0 ||
        rtl_memcmp (pmac, maddr, WIFI_NV_ADDR_LEN) != 0) {
#if 0
        /* Set to default value */
        srand (SCI_GetTickCount ());        
        pmac[0] = 0x00;
        pmac[1] = 0xe0;
        pmac[2] = 0x4c;
        pmac[3] = (unsigned long)rand() & 0xff;
        pmac[4] = (unsigned long)rand() & 0xff;
        pmac[5] = (unsigned long)rand() & 0xff;
#else
     rtl_memcpy(pmac, maddr, WIFI_NV_ADDR_LEN);
#endif
        /* Save to NV RAM */
        WIFI_SaveNvParam (&mac, NULL);

     SCI_TraceLow("MAC ADDR =   %02X:%02X:%02X:%02X:%02X:%02X\n",
            maddr[0], maddr[1], maddr[2], maddr[3], maddr[4], maddr[5]);
    }   
    
    return TRUE;
}

/* we should call this function when driver init to regsiter net to
 * TCP/IP stack, we will get a net_id form this function,
 * and we also register pkt xmit function: tx_req_fptr */
RTL_TCPIP_NET_ID_T rtl_tcpipRegNetInterface(RTL_TCPIP_NETIF_CFG_T* pConfig)
{
    TCPIP_NETIF_CFG_T config;

    if (pConfig == 0)
    {
        return 0;
    }
    rtl_memset(&config, 0, sizeof(TCPIP_NETIF_CFG_T));
    
    config.pkt_type = (TCPIP_PACKET_TYPE_E)pConfig->pkt_type;
    config.tx_req_fptr = (TCPIP_TX_REQ_FPTR)pConfig->tx_req_fptr;
    config.is_async_tx = pConfig->is_async_tx;
    //config.saddr = pConfig->saddr;
    config.saddr.ipaddr = pConfig->saddr.ipaddr;
    config.saddr.snmask = pConfig->saddr.snmask;
    config.saddr.gateway = pConfig->saddr.gateway;
    config.saddr.dns1 = pConfig->saddr.dns1;
    config.saddr.dns2 =pConfig->saddr.dns2;
    
    config.haddr.addr_ptr= pConfig->haddr.addr_ptr;
    config.haddr.addr_len= pConfig->haddr.addr_len;

   
    config.mtu = pConfig->mtu;
    config.tx_rate = pConfig->tx_rate;
    config.rx_flctrl_fptr = pConfig->rx_flctrl_fptr;
    config.netif_name_ptr = pConfig->netif_name_ptr;

    /* Save to NV RAM */
    rtl8723_SetMAC(config.haddr.addr_ptr);
    
    return TCPIP_RegNetInterface(&config);
}

/* we should call this function in driver deinit to
 * deregsiter net_id from TCP/IP stack */
void rtl_TCPIP_DeregNetInterface(RTL_TCPIP_NET_ID_T net_id)
{
    TCPIP_DeregNetInterface(net_id);
}

/* this function used for indicate pkt to TCP/IP */
RTL_TCPIP_ERROR_E rtl_tcpipRX(RTL_TCPIP_PACKET_INFO_T* pPacket)
{  
    TCPIP_PACKET_INFO_T packet;

    rtl_memset(&packet, 0, sizeof(TCPIP_PACKET_INFO_T));
    packet.data_ptr = pPacket->data_ptr;
    packet.data_len = pPacket->data_len;
    packet.net_id = pPacket->net_id;

    return (RTL_TCPIP_ERROR_E)TCPIP_RxInd(&packet);
}

void rtl8723_supp_notifyOnCnf(const RTL_WIFISUPP_ON_RESULT_T *pResult)
{
    RTL_WIFISUPP_ON_RESULT_T result ;
    int i = 0;

    if (pResult == 0)
    {
        return;
    }

    SCI_TRACE_LOW("======>rtl8723_supp_notifyOnCnf result=%d, pResult->net_id is 0x%x", pResult->wifi_on_result, pResult->net_id);

    rtl_memset(&result, 0, sizeof(RTL_WIFISUPP_ON_RESULT_T));
    result.wifi_on_result = (RTL_WIFISUPP_RESULT_E)pResult->wifi_on_result;
    result.net_id = pResult->net_id;
    result.is_support_roam = pResult->is_support_roam;
    //result.is_support_protocol = pResult->is_support_protocol;
    for (i = 0; i < WIFISUPP_ENCRYP_PROTOCOL_MAX; i++)
    {
        result.is_support_protocol[i] = pResult->is_support_protocol[i];
    }

    //result.is_support_eap = pResult->is_support_eap;
    for (i = 0; i < WIFISUPP_WPA_EAP_TYPE_MAX; i++)
    {
        result.is_support_eap[i] = pResult->is_support_eap[i];
    }

    //result.is_support_inner_eap = pResult->is_support_inner_eap;
    for (i = 0; i < WIFISUPP_INNER_CREDENTIAL_TYPE_MAX; i++)
    {
        result.is_support_inner_eap[i] = pResult->is_support_inner_eap[i];
    }

    SUPPMAIN_NotifyApp(WIFISUPP_SIG_ON_CNF, &result);
    SCI_TRACE_LOW("<=======rtl8723_supp_notifyOnCnf\n");
}

void rtl8723_supp_notifyOffCnf(const RTL_WIFISUPP_OFF_RESULT_T *pResult)
{
    RTL_WIFISUPP_OFF_RESULT_T result;

    if (pResult == 0)
    {
        return;
    }
    SCI_TRACE_LOW("WIFI para_supp_notifyOffCnf result=%d", pResult->wifi_off_result);
    rtl_memset(&result, 0, sizeof(RTL_WIFISUPP_OFF_RESULT_T));
    result.wifi_off_result = (RTL_WIFISUPP_RESULT_E)pResult->wifi_off_result;

    SUPPMAIN_NotifyApp(WIFISUPP_SIG_OFF_CNF, &result);
}

void rtl8723_supp_notifyConnectCnf(const RTL_WIFISUPP_CONNECT_RESULT_T *pResult )
{
        RTL_WIFISUPP_CONNECT_RESULT_T result ;

        if (pResult == 0)
        {
            return;
        }

        SCI_TRACE_LOW("WIFI para_supp_notifyConnectCnf result=%d", pResult->result);
        rtl_memset(&result, 0, sizeof(RTL_WIFISUPP_CONNECT_RESULT_T));
        result.result = (RTL_WIFISUPP_RESULT_E)pResult->result;
        //result.ssid = pResult->ssid;
        rtl_memcpy(result.ssid.ssid, pResult->ssid.ssid, sizeof(result.ssid.ssid));
        result.ssid.ssid_len = pResult->ssid.ssid_len;
        
        //result.bssid_info = pResult->bssid_info;
        rtl_memcpy(result.bssid_info.bssid, pResult->bssid_info.bssid, sizeof(result.bssid_info.bssid));
     result.channel = pResult->channel;
     result.rssi = pResult->rssi;

        SUPPMAIN_NotifyApp(WIFISUPP_SIG_CONNECT_CNF, &result);
}


void rtl8723_supp_notifyScanInd(const RTL_WIFISUPP_SCAN_AP_INFO_T*pScanInfo )
{
    RTL_WIFISUPP_SCAN_AP_INFO_T scan_info = {0};

    if (pScanInfo == 0)
    {
        return;
    }
    SCI_TRACE_LOW("WIFI para_supp_notifyScanInd");
    rtl_memset(&scan_info, 0, sizeof(RTL_WIFISUPP_SCAN_AP_INFO_T));
    //scan_info.ssid = pScanInfo->ssid;
    rtl_memcpy(scan_info.ssid.ssid, pScanInfo->ssid.ssid, sizeof(scan_info.ssid.ssid));
    scan_info.ssid.ssid_len = pScanInfo->ssid.ssid_len;
    
    //scan_info.bssid_info = pScanInfo->bssid_info;
    rtl_memcpy(scan_info.bssid_info.bssid , pScanInfo->bssid_info.bssid, sizeof(scan_info.bssid_info.bssid));
    
    scan_info.signal_qua = pScanInfo->signal_qua;
    scan_info.noise = pScanInfo->noise;
    scan_info.beacon_interval = pScanInfo->beacon_interval;
    scan_info.channel = pScanInfo->channel;
    scan_info.encryp_protocol = (RTL_WIFISUPP_ENCRYP_PROTOCOL_E)pScanInfo->encryp_protocol;
    scan_info.rate = pScanInfo->rate;
    scan_info.network_mode = (RTL_WIFISUPP_NETWORK_MODE_E)pScanInfo->network_mode;
    scan_info.credential_type = pScanInfo->credential_type;

    SUPPMAIN_NotifyApp(WIFISUPP_SIG_SCAN_IND, &scan_info);
}

void rtl8723_supp_notifyScanCnf(const RTL_WIFISUPP_RESULT_E *pResult)
{
    RTL_WIFISUPP_RESULT_E result = RTL_WIFISUPP_RESULT_SUCC;

    if (pResult == 0)
    {
        return;
    }

    SCI_TRACE_LOW("WIFI param_supp_notifyScanCnf");
    result = (RTL_WIFISUPP_RESULT_E)*pResult;

    SUPPMAIN_NotifyApp(WIFISUPP_SIG_SCAN_CNF, &result);
}

void rtl8723_supp_notifyDisconnectCnf(const RTL_WIFISUPP_DISCONNECT_RESULT_T *pDiscCnf)
{
    RTL_WIFISUPP_DISCONNECT_RESULT_T result ;

    if (pDiscCnf == 0)
    {
        return;
    }
    
    //SCI_TRACE_LOW:"WIFI param_supp_notifyDisconnectCnf result=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,RTL8723_WIFI_ADAPTER_661_112_2_18_0_36_17_2223,(uint8*)"d", pDiscCnf->result);
    rtl_memset(&result, 0, sizeof(RTL_WIFISUPP_DISCONNECT_RESULT_T));
    result.result = (RTL_WIFISUPP_RESULT_E)pDiscCnf->result;
//    result.ssid = pDiscCnf->ssid;
    rtl_memcpy(result.ssid.ssid, pDiscCnf->ssid.ssid, sizeof(result.ssid.ssid));
    result.ssid.ssid_len = pDiscCnf->ssid.ssid_len;
    
    SUPPMAIN_NotifyApp(WIFISUPP_SIG_DISCONNECT_CNF, &result);
}

void rtl8723_supp_notifyDisconnectInd(const RTL_WIFISUPP_DISCONNECT_INFO_T *pDiscInd)
{
    RTL_WIFISUPP_DISCONNECT_INFO_T disc_ind;

    if (pDiscInd == 0)
    {
        return;
    }
    
    //SCI_TRACE_LOW:"WIFI param_supp_notifyDisconnectInd"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,RTL8723_WIFI_ADAPTER_680_112_2_18_0_36_17_2224,(uint8*)"");
    rtl_memset(&disc_ind, 0, sizeof(RTL_WIFISUPP_DISCONNECT_INFO_T));
//    disc_ind.ssid = pDiscInd.ssid;
    rtl_memcpy(disc_ind.ssid.ssid, pDiscInd->ssid.ssid, sizeof(disc_ind.ssid.ssid));
    disc_ind.ssid.ssid_len = pDiscInd->ssid.ssid_len;

    SUPPMAIN_NotifyApp(WIFISUPP_SIG_DISCONNECT_IND, &disc_ind);
}

void rtl8723_supp_notifyRssiChangedInd(const RTL_WIFISUPP_RSSI_CHANGED_INFO_T *pRssiInd)
{
    RTL_WIFISUPP_RSSI_CHANGED_INFO_T rssi_ind;

    if (pRssiInd == 0)
    {
        return;
    }

    SCI_TRACE_LOW("###WIFI param_supp_notifyRssiChanged");
	rtl_memset(&rssi_ind, 0, sizeof(RTL_WIFISUPP_RSSI_CHANGED_INFO_T));
	rtl_memcpy(rssi_ind.ssid.ssid, pRssiInd->ssid.ssid, sizeof(rssi_ind.ssid.ssid));
	rssi_ind.ssid.ssid_len = pRssiInd->ssid.ssid_len;
	rtl_memcpy(rssi_ind.bssid_info.bssid, pRssiInd->bssid_info.bssid, sizeof(rssi_ind.bssid_info.bssid));
	rssi_ind.new_signal_qua= pRssiInd->new_signal_qua;
	SCI_TRACE_LOW("###WIFI param_supp_notifyRssiChanged mac=%02x:%02x:%02x:%02x:%02x:%02x\n",
		rssi_ind.bssid_info.bssid[0], rssi_ind.bssid_info.bssid[1], rssi_ind.bssid_info.bssid[2], rssi_ind.bssid_info.bssid[3], rssi_ind.bssid_info.bssid[4], rssi_ind.bssid_info.bssid[5]);
	SCI_TRACE_LOW("###WIFI param_supp_notifyRssiChanged ssid is %s, ssid_len is %d, new_rssi is %d",
		rssi_ind.ssid.ssid, rssi_ind.ssid.ssid_len, rssi_ind.new_signal_qua);

    SUPPMAIN_NotifyApp(WIFISUPP_SIG_RSSI_CHANGED_IND, &rssi_ind);
}

/**---------------------------------------------------------------------------*
 **                         Driver adapter functions                                                                         *
 **---------------------------------------------------------------------------*/

void rtl8723_gpioEnableIntCtl()
{
     //GPIO_EnableIntCtl (GPIO_WIFI_IRQ_ID);
 }

void rtl8723_gpioDisableIntCtl()
{
    //GPIO_DisableIntCtl(GPIO_WIFI_IRQ_ID);
}


//void para_getMac_from_NV(unsigned char* mac)
//{
    //
//}


void wifi_set_BT_status(int status)// 1: start 0: stop
{
#if 0
    ar6000_set_BT_status(status);   
#endif
}


BOOLEAN rtl8723_GetMAC(unsigned char hwmac[6])
{
    WIFI_ADDR_T addr = {0};
    uint32 ret = 0;
    unsigned char *pmac;
    unsigned char mac_zero[WIFI_NV_ADDR_LEN] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    
    ret = WIFI_GetNvParam(&addr, NULL);
    pmac = addr.address;
    if (WIFI_RET_OK != ret 
        || (pmac[0] == 0xff) || (SCI_MEMCMP(pmac, mac_zero, WIFI_NV_ADDR_LEN) == 0))   
        //No default mac or defaut mac is 0xFF-FF-FF-FF-FF-FF or 0x00-00-00-00-00-00
    {
        //generate random mac
        int     random_num = 0;
        srand(SCI_GetTickCount());
        random_num = rand();        
        //high 3, stand for Realtek serial chip
        pmac[0] = 0x00;
        pmac[1] = 0xe0;
        pmac[2] = 0x4c;

        pmac[3] = (uint8)((random_num & 0x00FF0000) >> 16);
        pmac[4] = (uint8)((random_num & 0x0000FF00) >> 8);
        pmac[5] = (uint8)(random_num & 0x000000FF);

        //save into nv
        WIFI_SaveNvParam(&addr, NULL);
     }

    //copy return
    SCI_MEMCPY(hwmac, pmac, 6); 
    
    SCI_TRACE_LOW("wifi: para_GetMAC is %x-%x-%x-%x-%x-%x, ret=%d", hwmac[0], hwmac[1], hwmac[2], hwmac[3], hwmac[4], hwmac[5], ret);

    return TRUE;
}

LOCAL uint32 rtl8723SpiGpioIrq(uint32  param)
{
    p_rtl8723_priv_ops->wifi_interrupt_func();
    return TRUE;
}
unsigned char rtl8723_wifi_supplicant_turn_on(unsigned char from_mp)
{
    RTL_WIFISUPP_ON_RESULT_T Result;
    int i = 0;
    int ret = 0;
    RTL_TCPIP_NET_ID_T netid = 0;
    SCI_TRACE_LOW("&&&&&&&&&rtl8723_wifi_supplicant_turn_on()\n");
    rtl_memset(&Result, 0, sizeof(RTL_WIFISUPP_ON_RESULT_T));
    if (WIFI_Open(WIFI_PORT_TYPE_SPI, rtl8723SpiGpioIrq) != WIFI_RET_OK)
    {
        SCI_TRACE_LOW("&&&&&&&&&&&&Failed to Open SPI Port\n");
        //return FALSE;
        goto open_fail;
    }
    
    ret = p_rtl8723_priv_ops->wifi_hw_open_func(&netid);
    if (ret < 0) {
        //hw_open fail, we'd better do WIFI_Close at first jacky_20120913
        WIFI_Close();
        SCI_TRACE_LOW("rtl8723_wifi_supplicant_turn_on wifi_hw_open_func fail\n");
        //return FALSE;
        goto open_fail;
    }

    ret = p_rtl8723_priv_iperf_ops->wifi_support_iperf_func();
    if (ret < 0) {
        //iperf_entry fail, we'd better do wifi_hw_close&WIFI_Close at first jacky_20120913
        p_rtl8723_priv_ops->wifi_hw_close_func();
        WIFI_Close();
        SCI_TRACE_LOW("wifi_support_iperf_func fail\n");
        goto open_fail;
    }

    SCI_TraceLow("&&&&&rtl8723_wifi_supplicant_turn_on(): netid is 0x%x",netid);
    WIFI_interrupt_enable(TRUE);

    //spi_mac_init();
    if (!from_mp) {
        Result.wifi_on_result = RTL_WIFISUPP_RESULT_SUCC;
        Result.net_id = netid;
        Result.is_support_roam = 0;
        //result.is_support_protocol = pResult->is_support_protocol;
        for (i = 0; i < WIFISUPP_ENCRYP_PROTOCOL_MAX; i++)
        {
            Result.is_support_protocol[i] = FALSE;
        }

        //result.is_support_eap = pResult->is_support_eap;
        for (i = 0; i < WIFISUPP_WPA_EAP_TYPE_MAX; i++)
        {
            Result.is_support_eap[i] = FALSE;
        }

        //result.is_support_inner_eap = pResult->is_support_inner_eap;
        for (i = 0; i < WIFISUPP_INNER_CREDENTIAL_TYPE_MAX; i++)
        {
            Result.is_support_inner_eap[i] = FALSE;
        }
        rtl8723_supp_notifyOnCnf(&Result);
    }

    return TRUE;

open_fail:
    Result.wifi_on_result = RTL_WIFISUPP_RESULT_FAIL;
    rtl8723_supp_notifyOnCnf(&Result);
    return FALSE;
}
unsigned char rtl8723_wifi_supplicant_turn_off(unsigned char from_mp)
{
    RTL_WIFISUPP_OFF_RESULT_T Result;
    
    WIFI_interrupt_enable(FALSE);
    p_rtl8723_priv_ops->wifi_hw_close_func();
    WIFI_Close();

    if (!from_mp) {
        Result.wifi_off_result = RTL_WIFISUPP_RESULT_SUCC;
        rtl8723_supp_notifyOffCnf(&Result);
    }

    return TRUE;
}

unsigned char rtl8723_wifi_supplicant_scan_proc(RTL_WIFISUPP_SCAN_REQ_INFO_T *scanReqInfo)
{
    int ret = 0;
    RTL_WIFISUPP_RESULT_E result = RTL_WIFISUPP_RESULT_SUCC;
    ret = p_rtl8723_priv_ops->wifi_scan_proc_func(scanReqInfo);
    if (ret == -1) {
        result = RTL_WIFISUPP_RESULT_FAIL;
        rtl8723_supp_notifyScanCnf(&result);
    }
    return TRUE;
}

unsigned char rtl8723_wifi_supplicant_connect_proc(RTL_WIFISUPP_SSID_CONFIG_T* connectInfo)
{
    int ret = 0;
    ret = p_rtl8723_priv_ops->wifi_connect_req_func(connectInfo);
    return ret;
}
unsigned char rtl8723_wifi_supplicant_disconnect_proc(RTL_WIFISUPP_DISCONNECT_REQ_INFO_T* disconnectInfo)
{
    int ret = 0;
    ret = p_rtl8723_priv_ops->wifi_disconnect_req_func(disconnectInfo);
    return ret;
}
/*---------------------------------------------------------------------*/
/* mmi -- wifi_supplicant*/
unsigned char rtl8723_wifi_on()
{
    //SCI_TRACE_LOW:"&&&&&&&&&rtl8723_wifi_on()\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,RTL8723_WIFI_ADAPTER_900_112_2_18_0_36_18_2231,(uint8*)"");
    
    return rtl8723_wifi_supplicant_turn_on(0);
}

unsigned char rtl8723_wifi_off()
{
    //RTL_WIFISUPP_OFF_RESULT_T Result;
    //SCI_TRACE_LOW:"&&&&&&&&&rtl8723_wifi_off()\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,RTL8723_WIFI_ADAPTER_908_112_2_18_0_36_18_2232,(uint8*)"");
    return rtl8723_wifi_supplicant_turn_off(0);
}

unsigned char rtl8723_wifi_scan(WIFISUPP_SCAN_REQ_INFO_T* scan_info_ptr)
{
    RTL_WIFISUPP_SCAN_REQ_INFO_T scanReqInfo;

    rtl_memset(&scanReqInfo,0,sizeof(RTL_WIFISUPP_SCAN_REQ_INFO_T));
    rtl_memcpy(&scanReqInfo, scan_info_ptr,sizeof(RTL_WIFISUPP_SCAN_REQ_INFO_T));
    return rtl8723_wifi_supplicant_scan_proc(&scanReqInfo);
}

unsigned char rtl8723_wifi_connect(WIFISUPP_SSID_CONFIG_T *connect_info_ptr)
{
    //SCI_TRACE_LOW:"&&&&&&&&&rtl8723_wifi_connect()\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,RTL8723_WIFI_ADAPTER_923_112_2_18_0_36_18_2233,(uint8*)"");
    return rtl8723_wifi_supplicant_connect_proc((RTL_WIFISUPP_SSID_CONFIG_T*)connect_info_ptr);
}

unsigned char rtl8723_wifi_disconnect(WIFISUPP_DISCONNECT_REQ_INFO_T *disconnect_info_ptr)
{
    //SCI_TRACE_LOW:"&&&&&&&&&rtl8723_wifi_disconnect()\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,RTL8723_WIFI_ADAPTER_929_112_2_18_0_36_18_2234,(uint8*)"");
    return rtl8723_wifi_supplicant_disconnect_proc(disconnect_info_ptr);
}

void rtl8723_handle_cus_msg(uint32 msg_id, void *msg_body_ptr)
{
    //SCI_TRACE_LOW:"&&&&&&&&&rtl8723_handle_cus_msg()\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,RTL8723_WIFI_ADAPTER_935_112_2_18_0_36_18_2235,(uint8*)"");
}

unsigned char rtl8723_get_connection_info(WIFISUPP_SCAN_AP_INFO_T *connection_info_ptr)
{
    //SCI_TRACE_LOW:"&&&&&&&&&rtl8723_get_connection_info()\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,RTL8723_WIFI_ADAPTER_940_112_2_18_0_36_18_2236,(uint8*)"");
    return 0;
}

unsigned int rtl8723_getThread_Identify(void)
{
    //SCI_TRACE_LOW:"&&&&&&&&&para_getThread_Identify()\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,RTL8723_WIFI_ADAPTER_946_112_2_18_0_36_18_2237,(uint8*)"");
    return 0;
    //return SCI_IdentifyThread();
}

/*------------------------------------------------------------------------------*/
/*----------------------- EUT functions--------------------------*/

#define RTL_EUT_TX_PKT_SINE    "sine"
#define RTL_EUT_TX_PKT_FRAME "frame"
#define RTL_EUT_TX_PKT_TX99    "tx99"
#define RTL_EUT_TX_PKT_TX100   "tx100"
#define RTL_EUT_TX_PKT_OFF "off"
#define RTL_EUT_RX_PKT_PROMIS      "promis"
#define RTL_EUT_RX_PKT_FILTER      "filter"
#define RTL_EUT_RX_PKT_REPORT      "report"
#define RTL_EUT_RX_PKT_OFF     "off"


#define RTL_EUT_TX_PKT_SIZE    1500    //[32-1500]
#define RTL_EUT_TX_PATTERN 1   //0: all zeros; 1: all ones; 2: repeating 10; 3: PN7; 4: PN9; 5: PN15
int g_wifi_eut_mode =0;
unsigned int g_rx_total_packets = 0;
unsigned int g_rx_err_packets = 0;

unsigned char wifi_eut_setmac(unsigned char* mac_addr)
{
    char str[6]={0};
    char i = 0;

    //sprintf(str,"macaddr=%x:%x:%x:%x:%x:%x\n", 
    //  mac_addr[0],mac_addr[1],mac_addr[2],
    //  mac_addr[3],mac_addr[4],mac_addr[5]);
    
    //SCI_TraceLow(":%s str = %s \n",__func__, str);
    for (i = 0; i  < 6; i++)
        str[i] = mac_addr[i];
    
        return (unsigned char)p_rtl8723_priv_ops->wifi_mp_handle[RTL_MP_SET_MACADDR](str);
}

int rtl8723_set_eut_rxtx_channel(int channel)
{
#if 1
    char str[128]={0};

    sprintf(str,"channel=%x\n", channel);
    
    //SCI_TraceLow:":%s(%d) str = %s \n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,RTL8723_WIFI_ADAPTER_993_112_2_18_0_36_18_2238,(uint8*)"sds",__func__,channel, str);
    return p_rtl8723_priv_ops->wifi_mp_handle[RTL_MP_SET_CHANNEL](str);
#endif
}

int rtl8723_set_eut_tx_rate(long ratio)
{
#if 1
    char str[128]={0};

    sprintf(str,"rate=%d\n", ratio);

    //SCI_TraceLow:":%s(%d) str = %s\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,RTL8723_WIFI_ADAPTER_1005_112_2_18_0_36_18_2239,(uint8*)"sds",__func__,ratio, str);
    return p_rtl8723_priv_ops->wifi_mp_handle[RTL_MP_SET_RATE](str);
#endif
}

int rtl8723_set_eut_tx_pwr(long pwr)
{
#if 1
    char str[128]={0};

    sprintf(str,"txpower=%d\n", pwr);

    //SCI_TraceLow:":%s(%d) str = %s\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,RTL8723_WIFI_ADAPTER_1017_112_2_18_0_36_18_2240,(uint8*)"sds",__func__,pwr, str);
    return p_rtl8723_priv_ops->wifi_mp_handle[RTL_MP_TX_POWER](str);
#endif
}

unsigned char rtl8723_eut_set_TX_param(unsigned char on_off, int tx_packet_mode)
{
#if 1
    char tx_str[128]={0};
    if (g_wifi_eut_mode) {
        if (on_off) {
            switch (tx_packet_mode) {
            case 0: //FRAME
                sprintf(tx_str,"count=%d,pkt\n", 1000);
            break;

            case 1://TX99               
          sprintf(tx_str,"background,cs\n");
            break;

            case 2://TX100
                sprintf(tx_str,"background,ct\n");
            break;

            case 3://SINE
                 sprintf(tx_str,"background,stone\n");
            break;
            
            default:
                sprintf(tx_str,"background,sc\n");
            break;
            }

            //SCI_TraceLow:":%s(%s)\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,RTL8723_WIFI_ADAPTER_1050_112_2_18_0_36_18_2241,(uint8*)"ss",__func__,tx_str);
        }
        else
        {
            sprintf(tx_str,"stop\n");
            //SCI_TraceLow:":%s(%s)\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,RTL8723_WIFI_ADAPTER_1055_112_2_18_0_36_18_2242,(uint8*)"ss",__func__,tx_str);
        }
        p_rtl8723_priv_ops->wifi_mp_handle[RTL_MP_TX_PKT](tx_str);

    }
#endif
    return 0;
}


unsigned char rtl8723_eut_set_RX_param(int rx_command)
{
#if 1
    char rx_str[64]={0};
    if(g_wifi_eut_mode)
    {
        switch(rx_command)
        {
        case 1: //off
            sprintf(rx_str,"start\n");
            //SCI_TraceLow:":%s(%s)\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,RTL8723_WIFI_ADAPTER_1075_112_2_18_0_36_18_2243,(uint8*)"ss",__func__,rx_str);

        break;

        case 0: //on
            sprintf(rx_str,"stop\n");
            //SCI_TraceLow:":%s(%s)\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,RTL8723_WIFI_ADAPTER_1081_112_2_18_0_36_18_2244,(uint8*)"ss",__func__,rx_str);

        break;

        case 2: //clear
            sprintf(rx_str,"clear\n");
            //SCI_TraceLow:":%s(%s)\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,RTL8723_WIFI_ADAPTER_1087_112_2_18_0_36_18_2245,(uint8*)"ss",__func__,rx_str);

        break;

        default:
        break;
        }       
    }
    p_rtl8723_priv_ops->wifi_mp_handle[RTL_MP_SET_RX](rx_str);
#endif
    return 0;
}

unsigned char rtl8723_eut_get_RX_packets(unsigned long *tatal_packets_ptr, unsigned long *err_packets_ptr)
{
#if 1
    if(g_wifi_eut_mode)
    {
             char rx_str[128]={0};
         unsigned long rxok = 0;
         unsigned long rxfail = 0;

            sprintf(rx_str,"rxreport\n");

            p_rtl8723_priv_ops->wifi_mp_handle[RTL_MP_QUERY_TRX](rx_str);

            sscanf(rx_str, "Rx OK:%d, CRC error:%d ", &rxok, &rxfail);
            if (tatal_packets_ptr != 0)
            {
                *tatal_packets_ptr = rxok + rxfail;
            }

            if (err_packets_ptr != 0)
            {
                *err_packets_ptr = rxfail;
            }
        }
#endif
    return 0;
}
#ifdef AMY_TODO
unsigned int eeprom_data_len = 1024;
unsigned char rtl8723_eut_get_cal_data(char* pBuf, unsigned int len)
{
    if (!pBuf)
        return 0;

    if (len != eeprom_data_len)
    {
        //SCI_TRACE_LOW:"length not right, should be %d\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,RTL8723_WIFI_ADAPTER_1136_112_2_18_0_36_18_2246,(uint8*)"d", eeprom_data_len);
        return 0;
    }
    
    rtl_memcpy(pBuf, eeprom_data, len);
    return 1;
}

unsigned char rtl8723_eut_update_cal_data(const char* pBuf, unsigned int len)
{
    if (!pBuf)
        return 0;

    if (len != eeprom_data_len)
    {
        //SCI_TRACE_LOW:"length not right, should be %d\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,RTL8723_WIFI_ADAPTER_1151_112_2_18_0_36_18_2247,(uint8*)"d", eeprom_data_len);
        return 0;
    }

    //verify checksum
    {
        uint16 original_checksum = 0;
        uint16        checksum;
        uint16        *ptr_crc = NULL;
        uint16        *ptr16_eeprom;
        uint32        i;
        uint32        eeprom_size = eeprom_data_len;


        eeprom_size = 1024;
        ptr_crc = (uint16 *)((uint8 *)pBuf + 0x04);

        //crc to be verified
        original_checksum = *ptr_crc;
        
        // Clear the crc
        *ptr_crc = 0;

        // Recalculate new CRC
        checksum = 0;
        ptr16_eeprom = (uint16 *)pBuf;
        for (i = 0;i < eeprom_size; i += 2)
        {
            checksum = checksum ^ (*ptr16_eeprom);
            ptr16_eeprom++;
        }
        checksum = 0xFFFF ^ checksum;
        *ptr_crc = checksum;
        if (original_checksum == checksum)
        {
            ath_TraceLow("[%s]check sum is right\n", __func__);
        }
        else
        {
            ath_TraceLow("[%s]check sum is wrong\n", __func__);
            return 0;
        }
    }
    rtl_memcpy(eeprom_data, pBuf, len);
    return 1;
}
#endif

int rtl8723_eut_custom_data(const uint* pBuf)
{
    int ret = 0;
	char *cpBuf = (char *) pBuf;
    ret = p_rtl8723_priv_ops->wifi_mp_handle[RTL_MP_CUSTOM](cpBuf);
    return ret;
}

#define  RTL_WIFI_WORK_MODE_NORMAL 0
#define  RTL_WIFI_WORK_MODE_TCMD 1

unsigned char rtl8723_set_eut_mode(unsigned char is_enter)
{
	int RTL_WIFI_WORK_MODE_DEFAULT = RTL_WIFI_WORK_MODE_NORMAL;
#if 1
    if(is_enter)
    {
        //SCI_TraceLow:"rtl8723_set_eut_mode enter =====>\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,RTL8723_WIFI_ADAPTER_1216_112_2_18_0_36_18_2248,(uint8*)"");
        RTL_WIFI_WORK_MODE_DEFAULT = RTL_WIFI_WORK_MODE_TCMD;
        rtl8723_wifi_supplicant_turn_on(1);
        p_rtl8723_priv_ops->wifi_mp_handle[RTL_MP_OPEN](NULL);
        g_wifi_eut_mode = 1;
    }
    else
    {
        //SCI_TraceLow:"rtl8723_set_eut_mode leave =====>\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,RTL8723_WIFI_ADAPTER_1224_112_2_18_0_36_18_2249,(uint8*)"");
        p_rtl8723_priv_ops->wifi_mp_handle[RTL_MP_CLOSE](NULL);
        rtl8723_wifi_supplicant_turn_off(1);
        RTL_WIFI_WORK_MODE_DEFAULT = RTL_WIFI_WORK_MODE_NORMAL;
        g_wifi_eut_mode = 0;
    }
#endif
    return 0;
}

char rtl8723_release_version[64] = "WIFI_RTL_1.0";
char * WIFI_GetVersion(void)
{   
    //SCI_TraceLow:"&&&&&&&&&&&&&WIFI_GetVersion: is %s\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,RTL8723_WIFI_ADAPTER_1237_112_2_18_0_36_18_2250,(uint8*)"s",rtl8723_release_version);
    return rtl8723_release_version;
}
void iperf_test_start(char* param)
{
}
#endif
