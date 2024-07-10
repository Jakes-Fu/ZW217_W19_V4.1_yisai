//#include "rpc_mocor_platform.h"
#include "halo_verify_common.c"

LOCAL uint8 verify_ap_ret;
LOCAL uint32 cp_verifycode;
LOCAL uint8 verify_cp_ret;
HALO_OSI_HANDLE_T   g_auth_ipcTask;
HALO_OSI_HANDLE_T   g_auth_ipcQueue;
// ipc transform sema for ipc tran cmd, it is for auth aync interfaces 
HALO_OSI_HANDLE_T   g_auth_ipcTransSema;

LOCAL uint32 ap_ts = 0;
LOCAL uint32 cp_ts0 = 0;
LOCAL uint32 cp_ts1 = 0;
LOCAL uint32 cp_ts2 = 0;

void __set_ApSide_result(uint8 result )
{
      verify_ap_ret = result;
}

uint8 __get_ApSide_result(void)
{
      return verify_ap_ret;
}

LOCAL int8 auth_fail_handler(int8 ret)
{
		PRTF("@@@@ auth_fail_handler is checking result : %d", ret);
		SCI_PASSERT((ret != ERR_COMMON),"common error");   
		SCI_PASSERT((ret != ERR_TIMEOUT1),"time out for ap/cp interverify error");
		SCI_PASSERT((ret != ERR_TIMEOUT2),"time out for ap/cp interverify-long error");
		return ret;
}

uint8 __verify_ApSide(ap_pkt_t *verifycode)
{

	AUTH_CMD_REQ *p_send;
	PRTF("@@@@auth start to verify AP signature ");
	HALO_AUTH_MEMALLOC((void **)&p_send, sizeof(AUTH_CMD_REQ), HALO_NO_SUSPEND);
	p_send->authId = AUTH_ID;
	p_send->cmdId = AUTH_CMD_VERIFYAP;
	p_send->para0 = (UINT32)verifycode;
    HALO_AUTH_SENDMESSAGE(g_auth_ipcQueue, (u32)p_send, 1, HALO_NO_SUSPEND);
	HALO_AUTH_OBTAINSEMA(g_auth_ipcTransSema, HALO_SUSPEND);
	return __get_ApSide_result();
	//return get_pkt(verifycode);
}

uint8 __process_Verify_ApSide(ap_pkt_t *verifycode)
{
	__set_ApSide_result(auth_fail_handler(get_pkt(verifycode)));
	HALO_AUTH_RELEASESEMA(g_auth_ipcTransSema);
}


uint8  __verify_CpSide(ap_pkt_t *verifycode)
{
	AUTH_CMD_REQ *p_send;
	PRTF("@@@@auth start to get CP signature");
	HALO_AUTH_MEMALLOC((void **)&p_send, sizeof(AUTH_CMD_REQ), HALO_NO_SUSPEND);
	p_send->authId = AUTH_ID;
	p_send->cmdId = AUTH_CMD_VERIFYCP;
	p_send->para0 = (UINT32)verifycode;
    HALO_AUTH_SENDMESSAGE(g_auth_ipcQueue, (u32)p_send, 1, HALO_NO_SUSPEND);
	HALO_AUTH_OBTAINSEMA(g_auth_ipcTransSema, HALO_SUSPEND);
	return 0;
	
}

uint8 __process_Verify_CpSide_fake(ap_pkt_t *verifycode)
{
 	ap_pkt_t *exch_pkt = NULL;
	int i;
	PRTF( "@@@@auth start __process_Verify_CpSide ");
	exch_pkt = (ap_pkt_t *)HALO_Malloc(TOKEN_LEN);
	exch_pkt->head[0] = CMD >> 8;
	exch_pkt->head[1] = CMD & 0xff;
	exch_pkt->head[2] = 0xff; //reserve
	exch_pkt->head[3] = 144; //payload length

	for(i=0;i<144;i++)//gen 128 byte random code
	{
		exch_pkt->code[i] = i;
	}
	
	*verifycode = *exch_pkt;
	HALO_Free(exch_pkt);
	exch_pkt = NULL;
	HALO_AUTH_RELEASESEMA(g_auth_ipcTransSema);
	return 0;
}

uint8 __process_Verify_CpSide_real(ap_pkt_t *verifycode)
{
 	ap_pkt_t *exch_pkt = NULL;
	int32 i;
	uint8 ret = 0;
	PRTF( "@@@@auth start __process_Verify_CpSide ");
	exch_pkt = (ap_pkt_t *)HALO_Malloc(TOKEN_LEN);
	ret = auth_fail_handler(set_pkt(exch_pkt));
	*verifycode = *exch_pkt;
	HALO_Free(exch_pkt);
	exch_pkt = NULL;
	HALO_AUTH_RELEASESEMA(g_auth_ipcTransSema);
	return ret;
}


uint8  __verify_Cp_result(uint8 result)
{
	AUTH_CMD_REQ *p_send;
	PRTF("@@@@auth verify_CpSide  result from ap is 0x%x", result);	
	PRTF("@@@@auth result from ap %d", result);
	cp_ts2 = SCI_GetTickCount();
	HALO_AUTH_MEMALLOC((void **)&p_send, sizeof(AUTH_CMD_REQ), HALO_NO_SUSPEND);
	p_send->authId = AUTH_ID;
	p_send->cmdId = AUTH_CMD_VERICPRET;
	p_send->para0 = (uint)result;
    HALO_AUTH_SENDMESSAGE(g_auth_ipcQueue, (uint)p_send, 1, HALO_NO_SUSPEND);
	HALO_AUTH_OBTAINSEMA(g_auth_ipcTransSema, HALO_SUSPEND);

	return 0;
}


uint8 __process_verify_Cp_result(uint8 result)
{
        int  ret = 0;
	unsigned int t1 = cp_ts2 -cp_ts0;
	unsigned int t2 = cp_ts0 -ap_ts;
	ret = result;
	PRTF("@@@@auth __process_verify_Cp_result is 0x%x, elapse time is %d ms", result, (t1+t2));
	
	if(ret  !=  0)
		ret = auth_fail_handler(ERR_COMMON);
	
	if(t1 > 3*SECOND)
		ret = auth_fail_handler(ERR_TIMEOUT1);

	if(t2 > 3*SECOND)
		ret = auth_fail_handler(ERR_TIMEOUT2);
	
	HALO_AUTH_RELEASESEMA(g_auth_ipcTransSema);
	return ret;
}



VOID auth_task_entry(uint argc, VOID *argv)
{
    uint   v_receiveMsg;
    uint   v_receiveSize;
    AUTH_CMD_REQ *p_rec;
    uint i;

    while(1)
    {
        HALO_AUTH_RECIEVEMESSAGE(g_auth_ipcQueue, &v_receiveMsg, 1, &v_receiveSize, HALO_SUSPEND);

      	p_rec = (AUTH_CMD_REQ *)v_receiveMsg;
		
		HALO_PASSERT(p_rec->authId == AUTH_ID, ("message auth ID is invalid"));    /*assert verified*/
		
        switch(p_rec->cmdId)
        {
		case AUTH_CMD_VERIFYAP:
			__process_Verify_ApSide((ap_pkt_t *)p_rec->para0);
			break;

		case AUTH_CMD_VERIFYCP:
			//__process_Verify_CpSide_fake((ap_pkt_t *)p_rec->para0); for RPC tunnel test only
			__process_Verify_CpSide_real((ap_pkt_t *)p_rec->para0);
			break;

		case AUTH_CMD_VERICPRET:
			__process_verify_Cp_result(0xff & p_rec->para0);
			break;
		default :
		break;	
        }

        HALO_AUTH_MEMFREE((void **)&p_rec);
	   }

}

void  auth_shakeHands2Ap(void)
{
	uint8 result = 0;
	uint8 val = 0;
	result = rpcHelloOnAP(val);
	PRTF("auth_shakeHands2Ap result is %c \n ", result);
}
void auth_init(void)
{ 
  // create auth  task and queue
    g_auth_ipcQueue = HALO_INVALID_HANDLE;
    HALO_AUTH_CREATEMESSAGE(&g_auth_ipcQueue, "authQUE", AUTH_MESSAGE_QUEUE_LEN, HALO_FIFO);
	
   	g_auth_ipcTask = HALO_INVALID_HANDLE;
    HALO_AUTH_CREATETASK(&g_auth_ipcTask, "authTASK",  auth_task_entry, 0, NULL, AUTH_STACKSIZE, AUTH_PRIORITY, AUTH_TIMESLICE, HALO_PREEMPT, HALO_START, HALO_STACK_CORE);

    g_auth_ipcTransSema = HALO_INVALID_HANDLE;
    HALO_AUTH_CREATESEMA(&g_auth_ipcTransSema, "authTrans", 0, HALO_FIFO);

    auth_shakeHands2Ap();
    return;
} 