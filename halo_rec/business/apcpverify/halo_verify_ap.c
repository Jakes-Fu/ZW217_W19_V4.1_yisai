#include "rpc_mocor_platform.h"
#include "halo_verify_common.c"


typedef enum
{
    VERIFIY_UNSTART = 0,  
    VERIFIY_THREAD_START,   	
    VERIFIY_VERIFYAP,
    VERIFIY_VERIFYAP_FAIL,
    VERIFIY_VERIFYCP,
    VERIFIY_VERIFYCP_FAIL,
    VERIFIY_FEEDBACKRESULT,
    VERIFIY_FEEDBACK_FAIL,
    VERIFIY_SUCCESS
}halo_status_t;

#define HALO_START_RPC  0x02
#define HALO_MONITOR_TIMEOUT  0x03
LOCAL void halo_init_verify(void);
LOCAL int32 init_apcpverify(void);
LOCAL int32 deinit_apcpverify(void);
LOCAL uint8 is_apcpverify_finished(void);
LOCAL uint8 stop_timer_monitor(HALO_TMID id);
LOCAL uint8 delete_timer_monitor(HALO_TMID id);


LOCAL uint32 s_Halo_thread_id = HALO_INVALID_BLOCK_ID;
LOCAL HALO_RPC_SIGNAL_T *s_halo_rpc_sig_ptr =  PNULL;
LOCAL HALO_TMID      s_HaloTest_timer = NULL; 
LOCAL volatile halo_status_t halo_status = VERIFIY_UNSTART;
LOCAL HALO_TICK_TIME_T s_tick_time = {0};
LOCAL HALO_TMID      s_monitor_timer = NULL; 

extern void halo_set_apap_module(HALO_APCP_MODULE_T * mod);
extern uint8 halo_get_shared_data(HALO_CLASS_DATA_T** data, uint32 id);

LOCAL const HALO_APCP_MODULE_T Halo_apcp = {
	.Module_id = MODULE_ID,
	.name = "halo_apapverify",
	.apcpverify_init = init_apcpverify,
	.apcpverify_deinit = deinit_apcpverify,
	.is_apcpverify_finished = is_apcpverify_finished,
};

LOCAL struct APCP_CTRL
{
	HALO_CLASS_DATA_T * data_ptr;
}APCP_CTRL = {NULL}; 

PUBLIC uint8 apcp_init(void)
{
	uint8 ret = 0;
	halo_set_apap_module(&Halo_apcp);
	return ret;
}


LOCAL uint32 _HALO_Sendtimeout(void)
{
    HALO_RPC_SIGNAL_T *sig_ptr = NULL;
    xSignalHeaderRec *sig=NULL;
    PRTF("_HALO_Sendtimeout enter ");    

    sig_ptr = (HALO_RPC_SIGNAL_T *)HALO_ALLOCA(sizeof(HALO_RPC_SIGNAL_T));//IMG_ASSERT_CHECK

    HALO_PASSERT(SCI_NULL!= sig_ptr, ("HALO_RPC: the sig_ptr is PNULL"));    /*assert verified*/

    sig = sig_ptr;
    sig->SignalSize = sizeof(HALO_RPC_SIGNAL_T);
    sig->Sender     = s_Halo_thread_id;
    sig->SignalCode = HALO_MONITOR_TIMEOUT;//rpc test
   
    SCI_SendSignal((xSignalHeader)sig_ptr, s_Halo_thread_id);

    return HALO_SUCCESS;
}

LOCAL void  halo_init_timeout(uint32 lparam)
{
   
    PRTF("halo: enter halo_init_timeout expire");

	HALO_ASSERT(NULL != s_monitor_timer);/*assert verified: check null pointer*/
    stop_timer_monitor(s_monitor_timer);
	delete_timer_monitor(s_monitor_timer);
    PRTF("halo_init_timeout, need to power off ");   
	//HALO_Sleep(HALO_SLEEP_MS);
    //POWER_PowerOff();
    _HALO_Sendtimeout();
	
}

LOCAL void  halo_verify_timeout(uint32 lparam)
{
   
    PRTF("halo: enter halo_verify_timeout expire");

    HALO_ASSERT(NULL != s_monitor_timer);/*assert verified: check null pointer*/
	
	stop_timer_monitor(s_monitor_timer);
	delete_timer_monitor(s_monitor_timer);
	PRTF("halo_verify_timeout, need to power off "); 
	//HALO_Sleep(HALO_SLEEP_MS);	   
	//POWER_PowerOff();
    _HALO_Sendtimeout();
	
}

LOCAL uint8 init_timer_monitor(uint period, void (*timeoout)(uint32 param))
{
 	if(NULL == s_monitor_timer)
 	{
 		if(period ==  0)
 			s_monitor_timer = HALO_CreateTimer("halo Timer monitor",timeoout,(uint32)0, HALO_INIT_TIMEOUT_MS,	HALO_NO_ACTIVATE);
		else
			s_monitor_timer = HALO_CreateTimer("halo Timer monitor",timeoout,(uint32)0, period,	HALO_NO_ACTIVATE);
	}
	else
	{
		PRTF("s_monitor_timer is already init ");
	}
	if(SCI_IsTimerActive(s_monitor_timer))	
	{		
		HALO_DeactiveTimer(s_monitor_timer);	
	}
	return 0;
}

LOCAL uint8 start_timer_monitor(HALO_TMID id )	
{
   if(id == HALO_NULL)
	id = s_monitor_timer;
   HALO_ASSERT(NULL != id);/*assert verified: check null pointer*/
   HALO_ActiveTimer(id);
   HALO_GetTickTime(s_tick_time);
   PRTF("start_timer_monitor enter with time %d.%d", s_tick_time.second, s_tick_time.milliseconds);
   return 0;  
}

LOCAL uint8 change_timer_monitor(HALO_TMID id,uint period, void (*timeoout)(uint32 param) )	
{
   if(id == HALO_NULL)
    id = s_monitor_timer;
   HALO_ASSERT(NULL != id);/*assert verified: check null pointer*/
   if(period ==  0)
   HALO_ChangeTimer(id, timeoout, START_DELAY_MS);
   else 
   HALO_ChangeTimer(id, timeoout, period);
   return 0;
}


LOCAL uint8 stop_timer_monitor(HALO_TMID id)
{
	if(id == HALO_NULL)
	id = s_monitor_timer;
		HALO_ASSERT(NULL != id);/*assert verified: check null pointer*/
	HALO_DeactiveTimer(id);
	HALO_GetTickTime(s_tick_time);
	PRTF("start_timer_monitor enter with time %d.%d", s_tick_time.second, s_tick_time.milliseconds);
	return 0;
}
LOCAL uint8 delete_timer_monitor(HALO_TMID id)
{
	if(id == HALO_NULL)
		id = s_monitor_timer;
	HALO_ASSERT(NULL != id);/*assert verified: check null pointer*/
   	HALO_DeleteTimer(id);
	return 0;
}


LOCAL uint8 halo_setVerifyStatus(halo_status_t status)
{
	PRTF("halo setVerifyStatus start with value %d\n", status);
	halo_status = status;
	return 0;
}

LOCAL halo_status_t halo_getVerifyStatus(void)
{
	PRTF("halo getVerifyStatus start with return  %d\n ", halo_status);
	return halo_status;
}

LOCAL uint8 is_apcpverify_finished(void)
{
	halo_status_t ret = 0;
	ret = halo_getVerifyStatus();
	if(ret == VERIFIY_SUCCESS)
    {
		PRTF("is_apcpverify_finished ret  %d", ret);
		return TRUE;
	}
	else
	{
		PRTF("is_apcpverify_finished failed ret  %d", ret);
		return FALSE;
	}
}

LOCAL void halo_sayHello2Cp(void)
{
uint8 val = 0;
uint8 result = 0;
PRTF("halo_init start to call rpc_hello function with val: 0x%x", val);
result = rpcHelloOnCP(val) ;
PRTF("halo_init finish to call rpc_hello function with val: 0x%x", result);
}



LOCAL void halo_mainProcess(void)
{
	uint8 val = 0;
	uint8 result = 0;
	uint8 result1 = 0;
	ap_pkt_t *exch_pkt = NULL;
	ap_pkt_t *verify_pkt = NULL;
	HALO_GetTickTime(s_tick_time);
	PRTF("halo_mainProcess enter with time %d.%d", s_tick_time.second, s_tick_time.milliseconds);

	exch_pkt = (ap_pkt_t *)HALO_Malloc(TOKEN_LEN);
	halo_setVerifyStatus(VERIFIY_VERIFYAP);
	PRTF("halo_mainProcess start to send AP certicode to CP");

	result = set_pkt(exch_pkt);

		if(result != 0)
		{				
			HALO_Free(exch_pkt);
			exch_pkt = NULL;	
			halo_setVerifyStatus(VERIFIY_VERIFYAP_FAIL);
			PRTF("halo_mainProcess verify AP set_pkt failed");
			return;
		
		}else{
			
			result = RPC_VERIFYAP(exch_pkt);
			
			if (result == 0)
				PRTF("halo_mainProcess send CP verify success");
			else 
			{	
				halo_setVerifyStatus(VERIFIY_VERIFYAP_FAIL);
				PRTF("halo_mainProcess send CP verify failed result %d",  result);
				return;
				
			}
			
			HALO_Free(exch_pkt);
			exch_pkt = NULL;
		
		}
		PRTF("halo_mainProcess start to recieve certificate code from CP");
		halo_setVerifyStatus(VERIFIY_VERIFYCP);
		verify_pkt = (ap_pkt_t *)HALO_Malloc(TOKEN_LEN);
		result =  RPC_VERIFYCP(verify_pkt);	
		
		//SCI_MEMCPY(verify_pkt,exch_pkt,148);
		
		result =  get_pkt(verify_pkt);

		if (result == 0)
			PRTF("halo_mainProcess recieve CP verify success");
		else 
		{
			halo_setVerifyStatus(VERIFIY_VERIFYCP_FAIL);
			PRTF("halo_mainProcess recieve CP verify failed result %d",  result);
			return;
		}	

		PRTF("halo_mainProcess start to send checking result to CP");
		halo_setVerifyStatus(VERIFIY_FEEDBACKRESULT);
		result1 = RPC_VERIFYCPRESULT(result);

		if (result1 == 0)
		{	
			halo_setVerifyStatus(VERIFIY_SUCCESS);
			PRTF("halo_mainProcess verify_Cp_result success");	
		}	
		else 
		{
			halo_setVerifyStatus(VERIFIY_FEEDBACK_FAIL);
			PRTF("halo_mainProcess verify_Cp_result failed result %d",  result1);
			return;
		}	
		HALO_Free(verify_pkt);
		verify_pkt = NULL;
		
		HALO_GetTickTime(s_tick_time);
		PRTF("halo_mainProcess exit with time %d.%d", s_tick_time.second, s_tick_time.milliseconds);
		return;
}

LOCAL uint _HALO_RPCReady_SendMsg(void)
{
    HALO_RPC_SIGNAL_T    *sig_ptr = NULL;
    HALO_RPC_SIGNAL_T    *sig = NULL;
    HALO_GetTickTime(s_tick_time);
    PRTF("_HALO_RPCtest_SendMsg with time %d.%d ", s_tick_time.second, s_tick_time.milliseconds);    

    sig_ptr = (HALO_RPC_SIGNAL_T *)HALO_ALLOCA(sizeof(HALO_RPC_SIGNAL_T));//IMG_ASSERT_CHECK

    HALO_PASSERT(SCI_NULL!= sig_ptr, ("HALO_RPC: the sig_ptr is PNULL"));    /*assert verified*/

    sig = sig_ptr;
    sig->SignalSize = sizeof(HALO_RPC_SIGNAL_T);
    sig->Sender     = s_Halo_thread_id;
    sig->SignalCode = HALO_START_RPC;//CP is ready 
   
    HALO_SendSignal((HALO_RPC_SIGNAL_T*)sig_ptr, s_Halo_thread_id);

    return SCI_SUCCESS;
}

LOCAL void _HALO_GetEfuseUid(uint8 *uid)
{
	HALO_ReadEfuseUID(uid);
	PRTF("halo_init EFUSE_HAL_ReadUid %02x %02x %02x %02x %02x %02x %02x %02x\n", uid[0], uid[1], uid[2], uid[3], uid[4], uid[5], uid[6], uid[7]);
}


LOCAL void _HALO_RPC_Task_Routine(uint32 argc, void *argv)
{
	uint8 uid[8] = {0};
	uint8 ret = 0;

	PRTF("_HALO_RPC_Task_Routine enter");
	s_halo_rpc_sig_ptr = (HALO_RPC_SIGNAL_T *)HALO_GetSignal(s_Halo_thread_id);

	if(s_halo_rpc_sig_ptr->SignalCode == HALO_START_RPC)
	{
		PRTF("_HALO_RPC_Task_Routine: Rpc process activated");
		stop_timer_monitor(s_monitor_timer);
		change_timer_monitor(s_monitor_timer,HALO_APCPVERIFY_TIMEOUT_MS,halo_verify_timeout);
		    
		//halo_sayHello2Cp();
		_HALO_GetEfuseUid(uid);
		halo_mainProcess();
		stop_timer_monitor(s_monitor_timer);

		ret = is_apcpverify_finished();
		if(ret != TRUE)
		{
			PRTF("_HALO_RPC_Task_Routine is_apcpverify_finished failed, need to power off ");  
			HALO_Sleep(HALO_SLEEP_MS);		 
			POWER_PowerOff();
		}
		
		
		
	}
	else if(s_halo_rpc_sig_ptr->SignalCode == HALO_MONITOR_TIMEOUT)
	{
		 PRTF("_HALO_timeout, need to power off ");  
         HALO_Sleep(HALO_SLEEP_MS);		 
         POWER_PowerOff();
	}
          
		 
    HALO_Free(s_halo_rpc_sig_ptr);
	s_halo_rpc_sig_ptr = PNULL;
}


uint8 notifyCpReady2AP(void)
{
	halo_status_t ret = 0;
	PRTF("AP acknowledge CP's handshakes\n");
	ret = halo_getVerifyStatus();
	if(ret == VERIFIY_UNSTART)
	{
		PRTF("system in VERIFIY_UNSTART status, can't do verify\n");
		return 0xe0;
	}
	halo_get_shared_data(&APCP_CTRL.data_ptr, Halo_apcp.Module_id);
	if(APCP_CTRL.data_ptr->calib_mode == 0xca)
	{
		PRTF("system in calibration mode, need to bybass RPC_ready Message\n");
		return 0xca;
	}
	_HALO_RPCReady_SendMsg();
	

return 0;
}

 
LOCAL void halo_init_verify(void)
{
	uint8 val = 0;
	uint8 result = 0;

	HALO_GetTickTime(s_tick_time);
	PRTF("halo_init enter with time %d.%d",s_tick_time.second, s_tick_time.milliseconds);
           
	s_Halo_thread_id = HALO_CreateThread(
	                    "halo_task", 
	                    "halo_queue",
	                    _HALO_RPC_Task_Routine,
	                    0,
	                    0,
	                    HALO_STACKSIZE,
	                    HALO_QUEUESIZE,
	                    HALO_PRIORITY,
	                    HALO_PREEMPT,
	                    HALO_AUTO_START);
	halo_setVerifyStatus(VERIFIY_THREAD_START);
	init_timer_monitor(HALO_NULL, halo_init_timeout);
	start_timer_monitor(s_monitor_timer);
PRTF("halo_init leave");

return;
}



LOCAL void halo_release_verify(void)
{
	PRTF("halo halo_release start");
}

LOCAL int32 init_apcpverify(void)
{
	PRTF("halo init_ap start");
	halo_setVerifyStatus(VERIFIY_UNSTART);
	
	halo_get_shared_data(&APCP_CTRL.data_ptr, Halo_apcp.Module_id);
	if(APCP_CTRL.data_ptr->calib_mode == 0xca)
	{
		PRTF("system in calibration mode ,need to bypass the apcp process!");
		return 0xe0;
	}
	halo_init_verify();	
	return 0;
}

LOCAL int32 deinit_apcpverify(void)
{
	
	PRTF("halo deinit_ap start");
	halo_setVerifyStatus(VERIFIY_UNSTART);
	halo_release_verify();
	delete_timer_monitor(s_monitor_timer);
	return 0;
}