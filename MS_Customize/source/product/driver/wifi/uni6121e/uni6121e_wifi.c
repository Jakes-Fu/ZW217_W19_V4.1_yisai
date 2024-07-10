#include "wifisupp_api.h"
#include "uni6121e_wifi.h"
#include "priority_system.h"
#include "../../../../../../chip_drv/chip_plf/uws6121e/uws6121e_int_cfg.h"
#include "isr_drvapi.h"
#include "../../../../../../chip_drv/chip_plf/common/inc/chip_drv_common_inter.h"
#define WF_OS_THREAD_STACK_SIZE 1024
#define WF_OS_THREAD_QUEUE_NUM 10

LOCAL BLOCK_ID wifiScan_taskid = SCI_INVALID_BLOCK_ID;
#define  TASK_WF_LOG_PRI   PRI_WIFISUPP_TASK  //26
int wifi8910_init_flag = 0;
wifiApInfo_t *aps = PNULL;

LOCAL SCI_SEMAPHORE_PTR uni8910_semaphore = SCI_NULL;
LOCAL SCI_TIMER_PTR uni8910_scantmr = SCI_NULL;
scanCtx_t uni8910_context;
int uni8910_g_chn = 0;
int uni8910_irq_stat = IRQ_DISABLED;

static void uni8910_prvWifiOpen()
{
	hwp_wcnRfIf->sys_control = 0x267;
	hwp_wcnWlan->config_reg = 0x802;
}

static void uni8910_prvWifiClose()
{
	hwp_wcnRfIf->wf_control = 0x2;
	hwp_wcnRfIf->sys_control = 0x63;
}
static void uni8910_prvWifiSetChannel(uint8 ch)
{
	REG_WCN_RF_IF_WF_CONTROL_T wf_control;
	wf_control.b.wf_tune = 0;
	wf_control.b.wf_chn = ch;
	hwp_wcnRfIf->wf_control = wf_control.v;
	SCI_Sleep(1);
	wf_control.b.wf_tune = 1;
	hwp_wcnRfIf->wf_control = wf_control.v;
}

static BOOLEAN uni8910_prvWifiDataReady()
{
	REG_WCN_WLAN_DATARDYINT_T dataready;
	dataready.v = hwp_wcnWlan->datardyint;
	//dataready = (hwp_wcnWlan->datardyint);
	return (dataready.b.datardyint == 1);
}

static void uni8910_prvWifiClearReady()
{
	uint32 i=0, j=0;
	REG_WCN_WLAN_CONFIG_REG_T conf_reg;
	conf_reg.v = hwp_wcnWlan->config_reg;
	// clear
	conf_reg.b.apb_clear = 1;
	hwp_wcnWlan->config_reg = conf_reg.v;
	while(((hwp_wcnWlan->config_reg & 0x1) != 0x1) && (i<=20)){i++;};//avoid endless loop
	// enable
	conf_reg.b.apb_clear = 0;
	hwp_wcnWlan->config_reg = conf_reg.v;
	while(((hwp_wcnWlan->config_reg & 0x1) != 0x0) && (j<=20)){j++;};//avoid endless loop
}

static void uni8910_prvLoadApInfo(wifiApInfo_t *info)
{
	uint32 i = 0, j=0;
	REG_WCN_WLAN_CONFIG_REG_T conf_reg;
	conf_reg.v = hwp_wcnWlan->config_reg;
	conf_reg.b.apb_hold = 1;
	hwp_wcnWlan->config_reg = conf_reg.v;
	while(((hwp_wcnWlan->config_reg & 0x2) != 0x2) && (i<=20)){i++;};//avoid endless loop

	info->bssid_low = hwp_wcnWlan->bssidaddr_l;
	info->bssid_high = (hwp_wcnWlan->bssidaddr_h & 0xffff);
	info->rssival = (int8)hwp_wcnWlan->rssival;

	conf_reg.b.apb_hold = 0;
	hwp_wcnWlan->config_reg = conf_reg.v;
	while(((hwp_wcnWlan->config_reg & 0x2) != 0x0) && (j<=20)){j++;};//avoid endless loop

}
/*ApInfo have handled */
static BOOLEAN uni8910_prvSaveApInfo(wifiApInfo_t *info, wifiScanRequest_t *req)
{
	if (req->found > req->max)
	    return false;
	else
	{
		BOOLEAN new_ap = true;
		uint8 i ;
		for (i= req->found; i > 0; --i)
		{
		    wifiApInfo_t *s = &req->aps[i - 1];
		    if (s->bssid_high == info->bssid_high && s->bssid_low == info->bssid_low)
		    {
		        if (info->rssival > s->rssival)
		        {
		            s->rssival = info->rssival;
		            s->channel = info->channel;
		        }
		        new_ap = false;
		        break;
		    }
		}

		if (new_ap && (req->found < req->max))
		    req->aps[req->found++] = *info;

		return true;
	}
}

static void uni8910_prvWifiScanChannel_(uint8 ch, uint32 timeout, wifiScanRequest_t *req)
{

    uint32 ntick = 0;
    ntick = SCI_GetTickCount();
    while((SCI_GetTickCount()-ntick) < timeout)
    {
    	if (uni8910_prvWifiDataReady())
    	{
    		wifiApInfo_t info = {0};
    		info.channel = ch;
    		uni8910_prvLoadApInfo(&info);
    		uni8910_prvWifiClearReady();
    		uni8910_prvSaveApInfo(&info, req);
    	}
    }
}

static void uni8910_prvWifiScanChannel(uint8 ch, uint32 timeout, wifiScanRequest_t *req)
{
    uint32 tout1 = WIFI_SCAN_MIN_INTERVAL;
    uint32 tout2 = 0;
    if (tout1 > timeout)
        tout1 = timeout;
    else
        tout2 = timeout - tout1;

    uni8910_prvWifiSetChannel(ch);
    uni8910_prvWifiScanChannel_(ch, tout1, req);
    // the broadcast interval from wifi ap is 100 ms, if got none
    // during 120 ms, we can presume there is no ap on this channel
    if (req->found != 0 && tout2 != 0)
    {
        uni8910_prvWifiScanChannel_(ch, tout2, req);
    }
}

/*****************************************************************************/
//  Description:    This function is used to notify wifi data
/*****************************************************************************/

static void uni8910_handle_irq(wifiScanRequest_t *req)
{
	wifiApInfo_t info = {0};
	info.channel = uni8910_g_chn;
	uni8910_prvLoadApInfo(&info);
	uni8910_prvSaveApInfo(&info, req);
	uni8910_prvWifiClearReady();
}
static void WCN_WLAN_IrqHandler(uint32 num)
{
	uni8910_handle_irq(uni8910_context.req);
}
BOOLEAN uni8910_registerWlanIrq()
{
	uint32 status = 0;
	status = ISR_RegHandler(TB_WCN_WLAN_INT, (TB_ISR)WCN_WLAN_IrqHandler);
	SCI_TRACE_LOW("uni8910_regisIrq,stat=%d",status);
	if ((TB_SUCCESS == status) || (TB_ISR_ASSIGNED == status))
	{
		return true;
	}
	else
	{
		return false;
	}
}
void uni8910_enableWlanIrq()
{
	CHIPDRV_EnableIRQINT(TB_WCN_WLAN_INT);
	uni8910_irq_stat = IRQ_ENABLED;
	SCI_TRACE_LOW("uni8910_enIrq,stat=%d",uni8910_irq_stat);
}

void uni8910_disableWlanIrq()
{
	CHIPDRV_DisableIRQINT(TB_WCN_WLAN_INT);
	uni8910_irq_stat = IRQ_DISABLED;
	SCI_TRACE_LOW("uni8910_disIrq,stat=%d",uni8910_irq_stat);
}
uint32 uni8910_ActiveTimer(SCI_TIMER_PTR timer_ptr)
{
	uint32 status = 0;
	if(timer_ptr)
	{
		status = SCI_ActiveTimer(timer_ptr);
		SCI_TRACE_LOW("uni8910_ActiveTimer,stat=%d",status);
		if(status == SCI_SUCCESS)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		SCI_TRACE_LOW("uni8910_ActiveTimer null");
		return false;
	}
}
uint32 uni8910_DeactiveTimer(SCI_TIMER_PTR timer_ptr)
{
	uint32 status = 0;
	if(timer_ptr)
	{
		status = SCI_DeactiveTimer(timer_ptr);
		SCI_TRACE_LOW("uni8910_DeactiveTimer,stat=%d",status);
		if(status == SCI_SUCCESS)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		SCI_TRACE_LOW("uni8910_DeactiveTimer null");
		return false;
	}
}
void uni8910_setTimerChn(uint8 chn)
{
	uni8910_g_chn = chn;
	SCI_TRACE_LOW("uni8910_setTimerChn=%d",uni8910_g_chn);
}
//timer callback function, one channel
void uni8910_scantmr_func(uint32 input)
{
	SCI_PutSemaphore(uni8910_semaphore);
	SCI_TRACE_LOW("uni8910_scantmr_func,fin=%d",SCI_GetTickCount());
}

void uni8910_createScanTimer()
{
	uni8910_scantmr = SCI_CreateTimer("uni8910_scantmr", uni8910_scantmr_func, 0, WIFI_SCAN_MIN_INTERVAL, SCI_NO_ACTIVATE);
	SCI_PASSERT((SCI_NULL != uni8910_scantmr), ("uni8910_createScanTimer failed!"));/*assert verified*/
}
uint32 uni8910_changeScanTimer(uint32 timeout)
{
	uint32 status = 0;
	if(uni8910_scantmr)
	{
		status = SCI_ChangeTimer (uni8910_scantmr, uni8910_scantmr_func, timeout);
		SCI_TRACE_LOW("uni8910_changeScanTimer,time=%d,stat=%d",timeout,status);
		if(status == SCI_SUCCESS)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		SCI_TRACE_LOW("uni8910_changeScanTimer null");
		return false;
	}
}
void uni8910_prvWifiScanChannel_irq(scanCtx_t context, uint8 chn)
{
	uni8910_prvWifiSetChannel(chn);
	uni8910_setTimerChn(chn);
	uni8910_changeScanTimer((context.req)->maxtimeout);//change timer
	uni8910_ActiveTimer(uni8910_scantmr);//enable timer
	SCI_GetSemaphore(uni8910_semaphore, SCI_WAIT_FOREVER);//wait semaphore
}
void func_byIrq(scanCtx_t context)
{
	uni8910_enableWlanIrq();//enable wlan IRQ
	SCI_WIFI_EnableDeepSleep(0);
	uni8910_prvWifiOpen();
	SCI_MEMSET((context.req)->aps, 0, ((context.req)->max)*sizeof(wifiApInfo_t));

	if (context.scan_all)
	{
		uint8 n;
		for (n = 0; n < context.round; ++n)
		{
			uint8 c;
			for (c = 1; c <= WIFI_CHANNEL_MAX; ++c)
			{
				uni8910_prvWifiScanChannel_irq(context, c);
			}
		}
	}
	else
	{
		uni8910_prvWifiScanChannel_irq(context, context.channel);
	}

	uni8910_disableWlanIrq();//disable wlan IRQ
	uni8910_prvWifiClose();
	SCI_WIFI_EnableDeepSleep(1);
	SCI_TRACE_LOW("uni8910 func_byIrq fin=%d",SCI_GetTickCount());
}
void func(scanCtx_t context)
{
    SCI_WIFI_EnableDeepSleep(0);
    uni8910_prvWifiOpen();
    SCI_MEMSET((context.req)->aps, 0, ((context.req)->max)*sizeof(wifiApInfo_t));
    if (context.scan_all)
    {
        uint8 n;
        for (n = 0; n < context.round; ++n)
        {
            uint8 c;
            for (c = 1; c <= WIFI_CHANNEL_MAX; ++c)
                uni8910_prvWifiScanChannel(c, (context.req)->maxtimeout,context.req);
        }
    }
    else
    {
        uni8910_prvWifiScanChannel(context.channel, (context.req)->maxtimeout, context.req);
    }
    uni8910_prvWifiClose();
    SCI_WIFI_EnableDeepSleep(1);
}


static void prvWifiScanThread(uint32 argc, void *argv)
{
	scanCtx_t context;
	APP_SIG_T* signal = NULL;
	for(;;)
	{
		signal = (APP_SIG_T *) SCI_GetSignal (wifiScan_taskid);

		SCI_TRACE_LOW("prvWifiScanThread SignalCode=%d\n", signal->SignalCode);

		switch(signal->SignalCode)
		{
			case APP_SCAN_MSG:
				SCI_WIFI_EnableDeepSleep(0);
				uni8910_prvWifiOpen();
				context = *(signal->scanCtx);
				SCI_MEMSET((context.req)->aps, 0, ((context.req)->max)*sizeof(wifiApInfo_t));
				if (context.scan_all)
				{
					uint8 n;
					for (n = 0; n < context.round; ++n)
					{
						uint8 c;
						for (c = 1; c <= WIFI_CHANNEL_MAX; ++c)
							uni8910_prvWifiScanChannel(c, (context.req)->maxtimeout,context.req);
					}
				}
				else
				{
			    		uni8910_prvWifiScanChannel(context.channel, (context.req)->maxtimeout, context.req);
				}
				uni8910_prvWifiClose();
				SCI_WIFI_EnableDeepSleep(1);
				break;
			default:
				SCI_TRACE_LOW("prvWifiScanThread SignalCode is default\n");
				break;
		}

		SCI_FREE(signal);
		signal = NULL;
		SCI_TRACE_LOW("prvWifiScanThread free Signal\n");
	}
}

PUBLIC void  sendMsgToScanTask(APP_MSG_E sig, scanCtx_t *scanCtxPtr)
{
    APP_SIG_T *signal = PNULL;
    uint32 sendsigsts = SCI_SUCCESS;
    SCI_CREATE_SIGNAL(
        signal,
        sig,
        sizeof (APP_SIG_T),
        SCI_IDENTIFY_THREAD());

    signal->scanCtx= scanCtxPtr;

    SCI_SEND_SIGNAL((xSignalHeaderRec *)signal, wifiScan_taskid);
    SCI_TRACE_LOW("uni8910wifirx,sendMsgToScanTask");
}



void drvWifirxInit()
{
    if(!wifi8910_init_flag)
    {
#if 0
        if (SCI_INVALID_BLOCK_ID == wifiScan_taskid)
        {
            wifiScan_taskid = SCI_CREATE_THREAD("T-WiFiScan", "Q-WiFiScan",
                                                  prvWifiScanThread/*void(* entry)(uint32,void *)*/,
                                                  0, NULL, WF_OS_THREAD_STACK_SIZE, 20,
                                                  TASK_WF_LOG_PRI,  // same priority as sme
                                                  1,
                                                  1);
        }
        SCI_PASSERT ( (wifiScan_taskid != SCI_INVALID_BLOCK_ID), ("Create WiFi Rx task failed!"));/*assert verified*/
#endif
		aps = (wifiApInfo_t *)SCI_ALLOC(WF_AP_MAX * sizeof(wifiApInfo_t));
		SCI_ASSERT(SCI_NULL != aps);

		if (SCI_NULL == uni8910_semaphore)
		{
			uni8910_semaphore = SCI_CreateSemaphore ("uni8910_semaphore", 0);
		}
		SCI_PASSERT ((uni8910_semaphore != SCI_NULL), ("Create uni8910_semaphore failed!"));/*assert verified*/

		uni8910_registerWlanIrq();//register WLAN IRQ
		uni8910_createScanTimer();//create SCAN Timer
    }
    wifi8910_init_flag = 1;
    SCI_TRACE_LOW("uni8910wifirx,drvwifirxInit");
}
void drvWifiOpen()
{
    SCI_TRACE_LOW("uni8910wifirx, drvWifiOpen");
    uni8910_wcnStart();
}
void drvWifiClose()
{
    SCI_TRACE_LOW("uni8910wifirx, drvWifiClose");
    uni8910_wcnStop();
}


void drvWifiScanAllChannel(wifiScanRequest_t *req)
{

    scanCtx_t context;
    context.scan_all = true;
    context.req = req;
    context.round = 1;
    context.req->found = 0;

    SCI_TRACE_LOW("uni8910wifirx,ScanAllChannel");
    uni8910_context = context;
    func_byIrq(uni8910_context);
    context = uni8910_context;
    SCI_TRACE_LOW("uni8910wifirx,ScanAllChannel finish");
}

void prvWifiScanAllChannelT()
{
    //SCI_TraceLow( "wifi scan all channel  rounds\n");
    wifiApInfo_t *aps = (wifiApInfo_t *)SCI_ALLOC(WF_AP_MAX * sizeof(wifiApInfo_t));
    wifiScanRequest_t req;
    req.aps = aps;
    req.max = WF_AP_MAX;
    req.found = 0;
    req.maxtimeout = 240;//multiply of 120ms is better

    SCI_ASSERT(SCI_NULL != aps);
    drvWifiScanAllChannel(&req);
    if (req.found != 0)
    {
        //qsort(&req.aps[0], req.found, sizeof(wifiApInfo_t), prvWifiApInfoCompare);
       	uint32 i ;
    	for (i = 0; i < req.found; i++)
        	{
    		wifiApInfo_t *apRes = &req.aps[i];
    		SCI_TRACE_LOW("uni8910wifirxTest,found ap - {mac address : %04x%08lx, rssival: %i dBm, channel: %u}",
    	                 apRes->bssid_high, apRes->bssid_low, apRes->rssival, apRes->channel);
        	}
    }
    else
    {
    	SCI_TRACE_LOW( "uni8910wifirx,wifi scan not found ap\n");
    }
       SCI_FREE(aps);
}

