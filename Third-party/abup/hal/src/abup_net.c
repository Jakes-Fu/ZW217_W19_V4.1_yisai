#include "abup_typedef.h"
#include "abup_debug.h"
#include "abup_net.h"
#include "os_api.h"
#include "in_message.h"
#include "socket_api.h"
#include "mn_type.h"
#include "mn_error.h"
#include "atc_plus_gprs.h"
#include "Sci_log.h"
#include "mmi_module.h"
#include "mmipdp_export.h"
#include "mmiconnection_export.h"

#ifdef FOTA_SUPPORT_ABUP
ABUP_BOOL s_abup_is_pdp_activated = ABUP_FALSE;
ABUP_BOOL s_abup_is_pdp_active_flag = ABUP_FALSE;
abup_uint8 s_abup_sock_index_activate_pdp = 0;
ABUP_BOOL s_abup_is_dns_act_pdp = ABUP_FALSE;  		//action: activate dns pdp
ABUP_BOOL s_abup_is_dns_pdp_actived = ABUP_FALSE;  	//status: dns pdp is actived or not
abup_common_callback s_abup_pdp_userevt;

static abup_socint g_tcp_socket_id = -1;
static abup_uint32 g_abup_net_id = 0;//5;
static abup_uint32 g_pdp_act = 0;
static ABUP_BOOL   b_abup_pdp_act = ABUP_FALSE;
ABUP_SOCKET_MGR_STRUCT abup_socket_mgr={NULL,NULL,NULL,NULL,0,0,0,0};

//for  socket
struct abup_socket_info
{
    abup_uint8 sock_index_id;   // add by longting.zhao
    abup_uint8 link_id;
    abup_uint16 lport;
    abup_uint16 fport;
    abup_uint32 fhost;
    //void *socket;
    TCPIP_SOCKET_T socket;
    abup_uint32 type;
    abup_uint32 state;
    TCPIP_HOST_HANDLE sock_request_id;   // add by longting.zhao    
    abup_char *addr_str;   
};

enum abup_cp_state_e
{
    ABUP_IDLE,
    ABUP_SET,
    ABUP_ACTIVE,
    ABUP_SEND
};

#define  MAX_RECEIVE_DATA_LENGTH   400
#define  MAX_SEND_DATA_LENGTH  1024
#define  UDP_DATACONF   1
#define  TCP_DATACONF   2
#define  MAX_PORT_NUM   65535
#define  MAX_CONNECT_ID_NUMBER  10
#define  default_netid   5     //default nsapi value = 5 when default cid =1
#define  MAX_TX_BUF_LEN   1024   
#define  MAX_RX_BUF_LEN    400   

#define  ABUP_MAX_TX_BUF_LEN               1024
#define  ABUP_MAX_RX_BUF_LEN               400   
#define  ABUP_MAX_SEND_DATA_LENGTH         1024
#define  ABUP_MAX_DATACONF_NUM             10
#define  ABUP_MAX_UDP_SEND_LENGTH          1096 //shijun modify for TFTP client cr35475
#define  ABUP_UDP_DATACONF                 1
#define  ABUP_TCP_DATACONF                 2
#define  ABUP_ATDATALEN                    500

#define  ABUP_DEFAULT_APN                   "CMNET"
#define  ABUP_DEFAULT_CID                   1
#define  ABUP_DEFAULT_NETID                 5     //default nsapi value = 5 when default cid =1
#define  ABUP_MAX_PORT_NUM                  65535

#define  ABUP_MAX_IP_STR_LEN            15  //just for IPV4
#define  ABUP_MAX_NETWORK_ADDR_LEN      255
#define  ABUP_MAX_PDP_ADDRESS_LEN 20
//Hyman we set at pdp max num as MN:MN_GPRS_MAX_PDP_ID_NUM
#define ABUP_MAX_PDP_CONTEXT_COUNT 15
#define ABUP_ERR_INVALID_INDEX 21
#define ABUP_MAX_PCO_LEN 253
#define ABUP_MAX_PDP_TYPE_LEN 7	 //longest str: ipv4v6
#define ABUP_MAX_APN_LEN 100
#define ABUP_MAX_PAP_USER_LEN 64
#define ABUP_MAX_PAP_PASSWD_LEN 64

#define ABUP_MAX_IMSI_LEN 16
#define ABUP_LACCELL_UPDATE_MAXNUM 5

#ifdef TCPIP_SUPPORT
LOCAL int32 ATC_CreateAndConnectTCPSocket(ATC_CONFIG_T *atc_config_ptr, unsigned long ip_addr, uint8 sock_index_id);
LOCAL int32 ATC_CreateUDPSocket(unsigned long ip_addr, uint8 sock_index_id);
LOCAL void ATC_SocketSend(ATC_CONFIG_T *atc_config_ptr,uint32 sock_id, uint8 sock_index);
LOCAL void ATC_CloseSocket(ATC_CONFIG_T *atc_config_ptr, uint32 id);
#endif

static uint8 s_abup_nsapi   = 0;  // record nsapi when PDP activated //s_nsapi
static struct abup_socket_info   s_abup_dataconf[ABUP_MAX_DATACONF_NUM+1] = {0};
static SCI_TIMER_PTR s_abup_ats7_timer[MN_GPRS_MAX_PDP_CONTEXT_COUNT] = {PNULL};//ATS7
static BOOLEAN s_sock_receive_close = FALSE;
abup_cell_info_struct s_abup_sal_cur_cell_info={0};
abup_cell_info_struct s_abup_sal_nbr_cell_info[6];
abup_uint16 s_abup_sal_cell_nbr_num = 0;
abup_uint8 s_abup_imsi[ABUP_MAX_IMSI_LEN+1] = {0};
static abup_uint8 abup_pdp_active_times = 0;

extern  MN_DUAL_SYS_E     g_current_card_id;
extern RSP_RESULT_STR     g_rsp_str; 
abup_uint32 abup_net_pdp_active(abup_common_callback userevt);
extern void abup_get_new_version_ex(void);
extern void abup_upgrade_thread_on_ex(void);
extern ABUP_BOOL  abup_fota_running(void);
extern void abup_register(void);
extern void ABUP_reset_stop_downlaod(void);

void abup_net_set_netid(abup_uint32 netid)
{
	g_abup_net_id = netid;
}

abup_uint32 abup_net_get_netid(void)
{
	return g_abup_net_id;
}

abup_uint32 abup_encode_dataaccount_id(void)
{
	static abup_uint32 account_id=0;

	abup_memset(&abup_socket_mgr, 0, sizeof(abup_socket_mgr));
	//abup_net_pdp_active();
	return account_id;
}

ABUP_BOOL abup_GetHostByName_Notify(void *msg_ptr)
{
	abup_int32 i=0;
	ASYNC_GETHOSTBYNAME_CNF_SIG_T *dns_ind = (ASYNC_GETHOSTBYNAME_CNF_SIG_T*)msg_ptr;
	struct sci_hostent *phost = NULL;
	abup_uint32 addr_buf=0;
	abup_int32 addr_len=0;
	abup_uint16 port=0;
	abup_int32 result=0;

	if(dns_ind->request_id != abup_socket_mgr.abup_request_id)
	{
		abup_DebugPrint("abup_GetHostByName_Notify: reqid:%d, errcode:%d,netid:%d ***********", \
		dns_ind->request_id, dns_ind->error_code, dns_ind->netid);
		return;
	}

	abup_DebugPrint("abup_GetHostByName_Notify: reqid:%d, errcode:%d,netid:%d", \
	dns_ind->request_id, dns_ind->error_code, dns_ind->netid);

	phost = &(dns_ind->hostent);
	if(phost == NULL)
	{
		abup_DebugPrint("abup_GetHostByName_Notify: phost null fail!!!");
		return ABUP_FALSE;
	}

	addr_len = phost->h_length;
	if(addr_len <= 0)
	{
		abup_DebugPrint("abup_GetHostByName_Notify: addr len=0, fail!!!");
		return ABUP_FALSE;
	}
	else if(addr_len >= 4)
		addr_len = 4;

	for(i=0;i<addr_len;i++)
	{
		abup_uint8 ipval = (abup_uint8)(*(phost->h_addr_list[0]+i));
		abup_DebugPrint("abup_GetHostByName_Notify dns_ind->addr=%d", ipval);
	}

	if(addr_len == 4)
	{
		abup_save_download_ip((abup_uint8)(*(phost->h_addr_list[0])),\
			(abup_uint8)(*(phost->h_addr_list[0]+1)),\
			(abup_uint8)(*(phost->h_addr_list[0]+2)),\
			(abup_uint8)(*(phost->h_addr_list[0]+3)));
	}

	if(dns_ind)
	{
		abup_DebugPrint("abup_GetHostByName_Notify dns_ind->request_id=%d, g_request_id=%d", dns_ind->request_id, abup_socket_mgr.abup_request_id);

		if(dns_ind->request_id != abup_socket_mgr.abup_request_id)
		{
			return ABUP_FALSE;
		}
	}

	if (dns_ind && (dns_ind->error_code==0))
	{
		abup_net_set_state(NET_STAT_CONN_REQ);
		abup_memcpy((abup_uint8 *)&addr_buf, phost->h_addr_list[0], addr_len);///4

		if(abup_get_fota_state() != STATE_DL)
		{
			abup_set_ipaddr(addr_buf);
		}

		port = 80;
		result=abup_connect(dns_ind->request_id, addr_buf, addr_len, port);

		if(result != E_ABUP_SUCCESS)
		{
			abup_DebugPrint_Ext("[ABUP_DNS]:","abup_GetHostByName_Notify result!=E_abup_SUCCESS");
			abup_soc_callback_init(&abup_socket_mgr,ABUP_ERROR_NOTIFY);

			abup_socket_mgr.abup_net_error_notify(dns_ind->request_id); //fota_notify_soc_net_error_notify
			abup_net_set_state(NET_STAT_CONN_ERR);
		}

		abup_net_set_state(NET_STAT_CONN_SUCC);	
		return ABUP_TRUE;
	}
	else                                // dns analyse error.
	{
		abup_DebugPrint_Ext("[ABUP_DNS]:","abup_GetHostByName_Notify dns_ind->dns analyse error"); 
		abup_soc_callback_init(&abup_socket_mgr,ABUP_ERROR_NOTIFY);
		abup_socket_mgr.abup_net_error_notify(dns_ind->request_id);
		abup_net_set_state(NET_STAT_DNS_ERR);
		//display dns error
	}

	return ABUP_FALSE;
}

abup_int32 Abup_GetHostByName(abup_int32 request_id, const abup_char* domain, abup_uint32* addr_buf, abup_uint32 nwk_account_id)
{
	abup_uint32 ret=0;
	abup_int32 result=-1;
	abup_uint32 netid = abup_net_get_netid();
	abup_uint32 taskid = abup_task_id(); 

	if(!s_abup_is_dns_pdp_actived)
	{
		abup_DebugPrint("abup_GetHostByName: PDP not activated!");
		return E_ABUP_FAILURE;
	}

	abup_DebugPrint("abup_GetHostByName: netid:%d, taskid:%d", netid, taskid);

	abup_socket_mgr.abup_request_id= request_id;
	abup_net_set_state(NET_STAT_DNS_REQ);

	ret = sci_async_gethostbyname(domain, taskid, 20000, netid);

	abup_socket_mgr.abup_request_id = ret;

	abup_DebugPrint_Ext("[ABUP_DNS]:","abup_config_fota_addr soc_gethostbyname return =%d", ret);

    if(ret == 0)//error
    {
		abup_DebugPrint_Ext("[ABUP_DNS]:","abup_config_fota_addr soc_gethostbyname ERROR");
		abup_net_set_state(NET_STAT_DNS_ERR);
		result = E_ABUP_FAILURE;
    }
	else
    {
		abup_DebugPrint_Ext("[ABUP_DNS]:","abup_config_fota_addr soc_gethostbyname SOC_WOULDBLOCK");
		//abup_net_set_state(NET_STAT_DNS_SUCC);
		abup_net_set_state(NET_STAT_DNS_GETTING);

		result = E_ABUP_WOULDBLOCK;//E_ABUP_SUCCESS;
    } 

	return result;
}

TCPIP_SOCKET_T abup_open_socket(void)
{
	int32 soc_id = TCPIP_SOCKET_INVALID;
	abup_int8 option=0;
	abup_int8 result=0;
	abup_uint32 netid=abup_net_get_netid();
	abup_uint32 account_id=0;
	abup_uint32 abup_taskid = abup_task_id();

	abup_net_set_state(NET_STAT_NEWSOC_REQ);
	abup_DebugPrint( "[ABUP_SOC]open_socket: netid = %d ", netid);
	soc_id = sci_sock_socket(AF_INET, SOCK_STREAM, 0, netid);
	
	if(soc_id == TCPIP_SOCKET_INVALID)
	{
		abup_DebugPrint_Ext("[ABUP_SOC]:","open_socket: soc_create failed, sockid = %d", soc_id);
		abup_net_set_state(NET_STAT_NEWSOC_ERR);
		return 0;
	}
	
	abup_socket_mgr.abup_tcp_socket_id= soc_id;
	abup_DebugPrint_Ext("[ABUP_SOC]:","connect_tcp-->soc_id = %d ,taskid = %d", soc_id,abup_taskid);
	
	result = sci_sock_setsockopt(soc_id, SO_NBIO, NULL);
	abup_DebugPrint("[ABUP_SOC]:open_socket set non block: %s", (result==0)?"OK":"FAIL!!!");
	
    result = sci_sock_asyncselect(soc_id, abup_taskid, AS_CONNECT|AS_READ|AS_WRITE|AS_CLOSE); 
    if(0 != result)
    {
        abup_DebugPrint("[ABUP_SOC]:open_socket: asyncselect fail!");
        return 0;
    }
	
	abup_net_set_state(NET_STAT_NEWSOC_SUCC);

	return (TCPIP_SOCKET_T)soc_id;  
}

abup_int32 abup_connect(abup_int32 request_id,abup_uint32 addr_buf,abup_uint16 addr_len,abup_uint16 port)
{
	struct sci_sockaddr addr;
	abup_socint soc_id;
	abup_int8 result=-1;
	abup_int32 error_code = 0;

	abup_DebugPrint("[ABUP_SOC] connect: addr_buf=0x%08x,addr_len=%d,port:%d,reqid:%d",addr_buf,addr_len,port,request_id);
	abup_memset(&addr, 0x00, sizeof(struct sci_sockaddr));

	addr.port = htons(port); 
	addr.family = AF_INET;
	addr.ip_addr = addr_buf;
	
	soc_id = abup_open_socket();

	abup_DebugPrint("[ABUP_SOC] connect: addr.ip_addr=0x%08x,addr.port:%d,len:%d. oport:%d",\
		addr.ip_addr, addr.port, sizeof(addr), port);

	abup_net_set_state(NET_STAT_CONN_REQ);
	result = sci_sock_connect(soc_id,(struct sci_sockaddr *)&addr,sizeof(addr));//addr_len
	
	abup_DebugPrint_Ext("[ABUP_SOC]:","socket connect return %d ", result);
	
	if (result == 0)
	{
		abup_DebugPrint_Ext("[ABUP_SOC]:","socket connect   SOC_SUCCESS ");
		abup_net_set_state(NET_STAT_CONN_SUCC);
		abup_soc_callback_init(&abup_socket_mgr,ABUP_WRITE_NOTIFY);
		abup_socket_mgr.abup_write_notify(soc_id);	
	}
	else if (result < 0)
	{
		error_code = sci_sock_errno(soc_id);
		abup_DebugPrint("abup: socket connect error_code=%d ", error_code);
		if((error_code==EWOULDBLOCK) || (error_code==EINPROGRESS))
		{
			abup_DebugPrint_Ext("[ABUP_SOC]:","socket connect   SOC_WOULDBLOCK ");
			abup_net_set_state(NET_STAT_CONNECTING);
		}
		else
		{
			abup_DebugPrint_Ext("[ABUP_SOC]:","socket connect   SOC_ERROR "); 
			abup_net_set_state(NET_STAT_CONN_ERR);			
			abup_soc_close(soc_id);
			abup_soc_callback_init(&abup_socket_mgr,ABUP_CLOSE_NOTIFY);
			abup_socket_mgr.abup_close_notify(soc_id);
			return E_ABUP_FAILURE;
		}

	}
	result=E_ABUP_SUCCESS;

	if(result==0){
		abup_send_socket_msg();
	}
	
	return result;
}

void abup_soc_close(abup_socint soc_id)
{
	abup_int32 result = 0;
	abup_DebugPrint("abup soc_close: soc_id=%d, g_tcp_socket_id=%d", soc_id, abup_socket_mgr.abup_tcp_socket_id);
	if(soc_id == -1)
		return;
	
	if(soc_id != abup_socket_mgr.abup_tcp_socket_id ) 
		return;
	   
	abup_net_set_state(NET_STAT_CLOSE_REQ);
	result = sci_sock_socketclose(soc_id);
	abup_socket_mgr.abup_tcp_socket_id=-1;
	abup_DebugPrint("abup soc_close: result: %d! line=%d",result,__LINE__);

	if(result < 0)
	{
		abup_DebugPrint("abup soc_close   unsuccess! errno:%d", sci_sock_errno(soc_id));
		abup_net_set_state(NET_STAT_CLOSE_ERR);
	}
	else
	{
		abup_DebugPrint("abup soc_close   success");
		abup_net_set_state(NET_STAT_CLOSE_SUCC);
	}

}

ABUP_BOOL abup_soc_hdlr(void *msg_ptr)
{
#if 1
	xSignalHeaderRec *soc_notify = (xSignalHeaderRec*) msg_ptr;
	abup_uint16 sigcode =	soc_notify->SignalCode;
	abup_int32 socid = -1; //TCPIP_SOCKET_T
	abup_uint32 errcode = 0;
		
	abup_DebugPrint("abup_soc_hdlr soc_notify->SignalCode=%d", sigcode);

	switch (sigcode)
	{
		case SOCKET_WRITE_EVENT_IND:	
			socid = ((SOCKET_WRITE_EVENT_IND_SIG_T *)soc_notify)->socket_id;
			if(socid != abup_socket_mgr.abup_tcp_socket_id )
			{
				break;
			}
			abup_DebugPrint_Ext("[ABUP_SOC]:","abup_soc_hdlr SOC_WRITE: socid:%d", socid);
			if(errcode == 0)
			{
				abup_soc_callback_init(&abup_socket_mgr,ABUP_WRITE_NOTIFY);
				abup_socket_mgr.abup_write_notify(socid); 
			}
			break;
			
		case SOCKET_READ_EVENT_IND:
			socid = ((SOCKET_READ_EVENT_IND_SIG_T *)soc_notify)->socket_id;
			if(socid != abup_socket_mgr.abup_tcp_socket_id )
			{
				break;
			}
			abup_DebugPrint_Ext("[ABUP_SOC]:","abup_soc_hdlr SOC_READ: socid:%d", socid);
			if(errcode == 0)
			{
				abup_soc_callback_init(&abup_socket_mgr,ABUP_READ_NOTIFY);
				abup_socket_mgr.abup_read_notify(socid);
			}

			break;
		case SOCKET_CONNECT_EVENT_IND:
			socid = ((SOCKET_CONNECT_EVENT_IND_SIG_T *)soc_notify)->socket_id;
			if(socid != abup_socket_mgr.abup_tcp_socket_id )
			{
				break;
			}
			errcode = ((SOCKET_CONNECT_EVENT_IND_SIG_T *)soc_notify)->error_code;
			abup_DebugPrint_Ext("[ABUP_SOC]:","abup_soc_hdlr SOC_CONNECT: socid:%d, err:%d", socid, errcode);
			if(errcode == 0)
			{
				abup_net_set_state(NET_STAT_CONN_SUCC);
				abup_soc_callback_init(&abup_socket_mgr,ABUP_WRITE_NOTIFY);
				abup_socket_mgr.abup_write_notify(socid); 
			}
			else
				abup_net_set_state(NET_STAT_CONN_ERR);
			break;
		   
		case SOCKET_CONNECTION_CLOSE_EVENT_IND:
			socid = ((SOCKET_CONNECTION_CLOSE_EVENT_IND_SIG_T *)soc_notify)->socket_id;
			if(socid != abup_socket_mgr.abup_tcp_socket_id )
			{
				break;
			}
			errcode = ((SOCKET_CONNECTION_CLOSE_EVENT_IND_SIG_T *)soc_notify)->error_code;
			abup_DebugPrint_Ext("[ABUP_SOC]:","abup_soc_hdlr SOC_CLOSE: socid:%d, err:%d", socid, errcode);
			if(errcode == 0)
			{
				abup_soc_callback_init(&abup_socket_mgr,ABUP_CLOSE_NOTIFY);
				abup_socket_mgr.abup_close_notify(socid); 	
			}
			break;
	}
#endif
	abup_DebugPrint_Ext("[ABUP_SOC]:","abup_soc_hdlr : socid:%d, err:%d", socid, errcode);
	return ABUP_FALSE;
	
}




abup_int32  abup_soc_send(abup_socint soc_id,Request* req )
{
	abup_int32 result;
	abup_int32 error_code = 0;   

	abup_DebugPrint("abup_soc_send  soc_id = %d,g_tcp_socket_id:%d", soc_id,abup_socket_mgr.abup_tcp_socket_id);
   
	abup_net_set_state(NET_STAT_SEND_REQ);
	result = sci_sock_send(soc_id, req->header.data, strlen(req->header.data),0);
   
	abup_DebugPrint(" abup_soc_send -------------  strlen(req->header.data) =%d",  strlen(req->header.data));
	abup_DebugPrint(" abup_soc_send ------------- header->data =%s", req->header.data);
	abup_DebugPrint(" %s", req->header.data+70);
	abup_DebugPrint(" %s", req->header.data+200);
	if(strlen(req->header.data) > 300)
		abup_DebugPrint(  " %s", req->header.data+300);
	abup_DebugPrint_Ext("[ABUP_SOC]:","abup_soc_send num of byte sent = %d", result);

	if (result > 0)
	{
		abup_DebugPrint("abup_soc_send   SOC_SUCCESS ");
		abup_net_set_state(NET_STAT_SEND_SUCC);
		return E_ABUP_SUCCESS;
	}
	else// if(result == SOC_WOULDBLOCK)
	{
		 error_code = sci_sock_errno(soc_id);
		 abup_DebugPrint("abup_soc_send: error_code=%d ", error_code);

		 //????????????????
		 //if(EPIPE==error_code || ENOBUFS == error_code || EWOULDBLOCK == error_code)
		 if(error_code == EWOULDBLOCK)
		 {

			abup_DebugPrint("abup_soc_send   SOC_WOULDBLOCK ");
			abup_net_set_state(NET_STAT_SENDING);
			//????????????????		abup_socket_mgr.abup_soc_state=SOC_WRITE;
			return E_ABUP_WOULDBLOCK;
		 }
		 else
		 {
			abup_DebugPrint("abup_soc_send	 SOC_ERROR "); 
			abup_net_set_state(NET_STAT_SEND_ERR);

			return E_ABUP_FAILURE;		
		 }
	 }
	/*
	 else
	 {
	       abup_DebugPrint("abup_soc_send   SOC_ERROR "); 
		abup_net_set_state(NET_STAT_SEND_ERR);

		return E_ABUP_FAILURE;		   
	  }*/

}



abup_int32 abup_soc_recv(abup_socint soc_id,abup_char* buf,abup_int32 len,abup_uint32 flag)
{
	abup_int32 ret = -1;
	abup_int32 error_code = 0;

	abup_DebugPrint("[ABUP_RECV]:soc_recv------>");	
	abup_net_set_state(NET_STAT_RECV_REQ);
	ret = sci_sock_recv(soc_id, buf, len, flag);
	
	abup_DebugPrint("[ABUP_RECV]: buf=%s,ret=%d",buf+20,ret);
	if (ret > 0)
	{
		abup_net_set_state(NET_STAT_RECV_SUCC);
		return 	ret;	
	}
	else
	{
		error_code = sci_sock_errno(soc_id);
		abup_DebugPrint("[ABUP_RECV]:abup soc_rev: error_code:%d line = %d", error_code,__LINE__);
		if(error_code == EWOULDBLOCK)
		{
			abup_DebugPrint("[ABUP_RECV]:abup_soc_rev   SOC_WOULDBLOCK ");
			abup_net_set_state(NET_STAT_RECVING);
			abup_DebugPrint("[abup_RECV]:abup soc_rev   SOC_WOULDBLOCK ----------->1 ");
			return E_ABUP_WOULDBLOCK;
		}
		else
		{
			abup_net_set_state(NET_STAT_RECV_ERR);
			return E_ABUP_FAILURE;
		}			
	}
}

abup_socint abup_net_socid(void)
{
	return abup_socket_mgr.abup_tcp_socket_id;
}
void abup_net_set_state(abup_int32 netsm)
{
	//abup_DebugPrint("abup_net:set sm: %d", netsm);
	abup_socket_mgr.abup_net_sm = netsm;
}
abup_int32 abup_net_get_state(void)
{
	abup_DebugPrint("abup_net:get sm: %d", abup_socket_mgr.abup_net_sm);
	return abup_socket_mgr.abup_net_sm;
}

void abup_net_cell_set_rxlev(abup_uint8 rxlev)
{
	s_abup_sal_cur_cell_info.rxlev = ((rxlev + 3) / 2);
	abup_DebugPrint("abup_net_cell_set_rxlev: rxlev:%d, raw lev:%d", \
		s_abup_sal_cur_cell_info.rxlev, rxlev);
}

void abup_net_cell_req(void)
{
	abup_uint8 dual_sys = 0;
    MN_PHONE_CURRENT_PLMN_INFO_T   plmn_info;
    MN_PHONE_SCELL_MEAS_T          rssi;

	abup_stop_timer(abup_net_cell_req);
	
	abup_DebugPrint("abup_net_cell_req: b");

    plmn_info = MNPHONE_GetCurrentPLMNInfoEx(dual_sys);
    rssi      = MNPHONE_GetRSSIEx(dual_sys);

	s_abup_sal_cur_cell_info.arfcn = rssi.arfcn;
	s_abup_sal_cur_cell_info.bsic = rssi.bsic;
	s_abup_sal_cur_cell_info.mcc = plmn_info.mcc;
	s_abup_sal_cur_cell_info.mnc = plmn_info.mnc;
	s_abup_sal_cur_cell_info.lac = plmn_info.lac;
	s_abup_sal_cur_cell_info.ci = plmn_info.cell_id;
	s_abup_sal_cell_nbr_num = 0;

	s_abup_sal_cur_cell_info.updatetimes++;

	abup_DebugPrint("abup_net_cell_req[%d]: mcc:%d,mnc:%02d;rxlev:%d;lac:%d,cid:%d;bsic:%d,arfcn:%d",
		s_abup_sal_cur_cell_info.updatetimes, \
		s_abup_sal_cur_cell_info.mcc, s_abup_sal_cur_cell_info.mnc, \
		s_abup_sal_cur_cell_info.rxlev, \
		s_abup_sal_cur_cell_info.lac, s_abup_sal_cur_cell_info.ci, \
		s_abup_sal_cur_cell_info.bsic, s_abup_sal_cur_cell_info.arfcn);

    //abup_Debug_DeltaNew();
}

void abup_net_cell_dereq(void)
{
	abup_DebugPrint(  "abup_net_cell_dereq:  MSG_ID_L4C_NBR_CELL_INFO_DEREG_REQ");
}

abup_uint32 abup_net_apn_config(abup_uint8 simid, abup_char *apn, abup_char *username, abup_char *userpwd)
{
    ATC_SET_PDP_CONTEXT_REQ_T   mn_info;
    abup_uint32 result = 0;
    abup_uint8  user[ABUP_MAX_PAP_USER_LEN + 1];
    abup_uint8  passwd[ABUP_MAX_PAP_PASSWD_LEN + 1];
    abup_uint32 dual_sys = simid;

    ABUP_BOOL context_param[7] = {ABUP_TRUE,ABUP_TRUE,ABUP_TRUE,ABUP_TRUE,ABUP_TRUE,ABUP_TRUE,ABUP_TRUE};
    MN_GPRS_PDP_ADDR_T      pdp_addr = {0}; 
    ERR_MNGPRS_CODE_E       err = ERR_MNGPRS_NO_ERR;

	abup_uint8 len = 0;

    abup_memset(&mn_info, 0x00, sizeof(mn_info));
    
    if(apn)
    {
    	len = abup_strlen(apn);
        abup_strncpy((abup_char *)(mn_info.apn), apn, len);

		if(len < MAX_APN_LEN)
        	mn_info.apn[len] = '\0';
    }      
    else  
    {   
        //default apn setting and save
        abup_strncpy((abup_char *)(mn_info.apn),ABUP_DEFAULT_APN,abup_strlen(ABUP_DEFAULT_APN));
        mn_info.apn[abup_strlen(ABUP_DEFAULT_APN)] = '\0';
    }

    if(ERR_MNGPRS_NO_ERR!=(err=MNGPRS_SetPdpContextEx(dual_sys,context_param,ABUP_DEFAULT_CID,(abup_uint8 *)"IP",(abup_uint8 *)mn_info.apn, pdp_addr,0,0,0)))
    {
        //SCI_TRACE_LOW:"ATC: MNGPRS_SetPdpContextEx FAIL,%d"
        return ABUP_ERR_FAIL;
    }

    //param 2,3 --- USERNAME and password
    if(username && passwd)
    {
        //if((PARA2.str_ptr->str_len <= MAX_PAP_USER_LEN) && (PARA3.str_ptr->str_len <= MAX_PAP_PASSWD_LEN))
        {
        	len = abup_strlen(username);
            strncpy((char *)user, (char *)username, len);
            user[len] = '\0';

			len = abup_strlen(userpwd);
            abup_strncpy((char *)passwd, (char *)userpwd, len);
            passwd[len] = '\0';

            result = MNGPRS_SetPdpContextPcoEx(dual_sys,ABUP_DEFAULT_CID, user, passwd);   //default cid=1

            if(result != MN_GPRS_ERR_SUCCESS)
            {
                //SCI_TRACE_LOW:"ATC: set username or password error, result = %d"
                return (abup_uint32)ABUP_ERR_OPERATION_NOT_SUPPORTED;
            }
        }
    }

       return (abup_uint32)ABUP_ERR_SUCCESS;
}

void abup_pdp_actpdpcontext_cnf(APP_MN_GPRS_T *sig_ptr)
{
	abup_uint8 nsapi=0;
	abup_uint8 pdp_id = 0xFF;
	APP_MN_GPRS_EXT_T *sig = (APP_MN_GPRS_EXT_T *)sig_ptr;

	if(!sig || b_abup_pdp_act == ABUP_FALSE)
		return;

	b_abup_pdp_act = ABUP_FALSE;
	pdp_id = sig->pdp_id;

	s_abup_is_dns_act_pdp = ABUP_FALSE;
	
    if(SCI_IsTimerActive(s_abup_ats7_timer[pdp_id]))
    {
        SCI_DeactiveTimer(s_abup_ats7_timer[pdp_id]);
    }
	
    if((((APP_MN_GPRS_EXT_T*)sig_ptr)->result >= MN_GPRS_ERR_SUCCESS) && (((APP_MN_GPRS_EXT_T*)sig_ptr)->result <= MN_GPRS_ERR_SUCCESS_SINGLE_ONLY))
    {
        s_abup_is_pdp_activated = ABUP_TRUE;
        s_abup_is_dns_pdp_actived = ABUP_TRUE;
        nsapi = ((APP_MN_GPRS_EXT_T*)sig_ptr)->nsapi;
        abup_DebugPrint("ABUP: PDP activate success s_nsapi=%d", nsapi);
		abup_net_set_netid(nsapi);
		g_pdp_act = 0;
		abup_net_pdp_userevt_exe();
		s_abup_pdp_userevt = NULL;
    }
	else
    {
        s_abup_is_pdp_activated = ABUP_FALSE;
        abup_DebugPrint("ABUP: PDP CNF sock index result unsuccess tray again");
        SCI_SLEEP(2000);
        g_pdp_act ++;
        if(g_pdp_act < 3)
        {
        	abup_net_pdp_active(s_abup_pdp_userevt);
        }
        else
        {
        	g_pdp_act = 0;
        	s_abup_pdp_userevt = NULL;
        }
    }
	
}

ABUP_BOOL abup_net_pdp_is_active(void)
{ 
	return s_abup_is_dns_pdp_actived;
}

void abup_net_pdp_userevt_exe(void)//abup_uint8 simid
{ 
	if(s_abup_pdp_userevt)
		s_abup_pdp_userevt();
}

static void abup_net_pdp_reactive(void)
{
	if(abup_net_pdp_is_active() == ABUP_FALSE && s_abup_pdp_userevt)
	{
		abup_net_pdp_active(s_abup_pdp_userevt);
	}
}

LOCAL void abupFotaPdpMsg(MMIPDP_CNF_INFO_T *msg_ptr)
{
	abup_DebugPrint("abupFotaPdpMsg, msg_id = %d, result = %d", msg_ptr->msg_id, msg_ptr->result);
    if(PNULL == msg_ptr)
    {
        return;
    }
    switch(msg_ptr->msg_id) 
    {
        case MMIPDP_ACTIVE_CNF:     //PDP激活成功
            if(MMIPDP_RESULT_SUCC == msg_ptr->result)
            {
				abup_DebugPrint("abup-->pdp active success, net id=%d\n", msg_ptr->nsapi);
				abup_stop_timer(abup_net_pdp_reactive);
				abup_pdp_active_times = 0;
				s_abup_is_dns_pdp_actived = TRUE;
				abup_net_set_netid(msg_ptr->nsapi);
				abup_net_pdp_userevt_exe();
            }
            else
            {
				abup_DebugPrint("abup-->pdp active failed");
				if (abup_pdp_active_times < 4)
				{
					abup_pdp_active_times++;
					abup_start_timer(30*1000, abup_net_pdp_reactive, ABUP_FALSE); 
				}
				else
				{
					abup_pdp_active_times = 0;
				}
            }
            break;

        case MMIPDP_DEACTIVE_CNF:   //PDP去激活成功
            break;
        case MMIPDP_DEACTIVE_IND:   //PDP被网络端去激活
            break;	        

        default:
            break;
    }
}

abup_uint32 abup_net_pdp_active(abup_common_callback userevt)//abup_uint8 simid
{ 
    abup_uint32 id = 0;
	MMIPDP_ACTIVE_INFO_T	active_info 	= {0};
	MMICONNECTION_APNTYPEINDEX_T *apn_idx_arr;
	MMICONNECTION_LINKSETTING_DETAIL_T* linksetting = PNULL;
	ERR_MNGPRS_CODE_E	ret 	= ERR_MNGPRS_NO_ERR;
	MN_DUAL_SYS_E sim_sys = MN_DUAL_SYS_1;

	s_abup_pdp_userevt = userevt;
	b_abup_pdp_act = ABUP_TRUE;
	abup_DebugPrint("abup: s_abup_is_dns_act_pdp %d, s_abup_is_dns_pdp_actived %d", s_abup_is_dns_act_pdp, s_abup_is_dns_pdp_actived);
	
	if(!s_abup_is_dns_pdp_actived)
	{	
		abup_DebugPrint("abup_net_pdp_active --->");
	#if 1//defined(MMI_GPRS_SUPPORT) || defined(WIFI_SUPPORT)
		if (FALSE == MMIAPIPHONE_GetDataServiceSIM(&sim_sys))
		{
			abup_DebugPrint("abup_net_pdp_active [SIM ERROR]");
			return ABUP_ERR_FAIL;
		}

		active_info.app_handler 		= MMI_MODULE_COMMON;
		active_info.dual_sys			= sim_sys;
		active_info.apn_ptr 			= NULL;
		active_info.user_name_ptr		= NULL;
		active_info.psw_ptr 			= NULL;
		active_info.priority			= 3;
		active_info.ps_service_rat		= MN_UNSPECIFIED;
		active_info.ps_interface		= MMIPDP_INTERFACE_GPRS;
		active_info.handle_msg_callback = abupFotaPdpMsg;
		active_info.ps_service_type = BROWSER_E;
		active_info.storage = MN_GPRS_STORAGE_ALL;

		apn_idx_arr = MMIAPICONNECTION_GetApnTypeIndex(sim_sys);
		
		if (apn_idx_arr == NULL)
		{
			SCI_TraceLow("abup:pdp_active [APN IDX ERROR]");
			return ABUP_ERR_FAIL;
		}
			abup_DebugPrint("abup:pdp_active [Net Setting = %d]", apn_idx_arr->internet_index[sim_sys].index);
			linksetting = MMIAPICONNECTION_GetLinkSettingItemByIndex(sim_sys, apn_idx_arr->internet_index[sim_sys].index);

		if(PNULL != linksetting && 0 != linksetting->apn_len)
		{
			active_info.apn_ptr = (char*)linksetting->apn;
			active_info.user_name_ptr = (char*)linksetting->username;
			active_info.psw_ptr = (char*)linksetting->password;
            #if 0//def IPVERSION_SUPPORT_V4_V6
			active_info.ip_type = linksetting->ip_type;
            #endif
			if(MMIAPIPDP_Active(&active_info))
			{
                abup_DebugPrint("ABUP: ip_type = %d",active_info.ip_type);
				return ABUP_ERR_SUCCESS;
			}
		}
			abup_DebugPrint("ABUP: connectPDP [Net Setting ERROR]");
	#endif
		}
		else
		{
			abup_DebugPrint("ABUP: PDP is already activated");
		}

     return ABUP_ERR_SUCCESS;
}

/******************************************************************************/
//  DESCRIPTION:This function handles command AT+NETAPN, set net access
//  Global resource dependence : 
//  AUTHOR: 
//  Note : 
/******************************************************************************/
//ABUP_BOOL Abup_ProcessNETAPN(
//        ATC_CONFIG_T         *atc_config_ptr, // point to the ATC_CONFIG_T struct
//        ATC_CUR_INFO_LIST_T  *cur_cmd_info    // point to the atc cmd info
//)
ABUP_BOOL Abup_ProcessNETAPN(abup_char *apn, abup_char *uname, abup_char *upwd)
{
    abup_uint32                      i       = 0;
    ATC_SET_PDP_CONTEXT_REQ_T   mn_info;
    abup_uint32 result                       = 0;
    abup_uint8  user[MAX_PAP_USER_LEN + 1];
    abup_uint8  passwd[MAX_PAP_PASSWD_LEN + 1];
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

    ABUP_BOOL                 context_param[7] = {TRUE,TRUE,TRUE,FALSE,FALSE,FALSE,FALSE};
    MN_GPRS_PDP_ADDR_T      pdp_addr = {0}; 
    ERR_MNGPRS_CODE_E       err = ERR_MNGPRS_NO_ERR;
	abup_uint8 len=0, len1=0;

    abup_DebugPrint("Abup_ProcessNETAPN b");

	abup_DebugPrint("ABUP: apn:%s, username:%s, password:%s", apn, uname, upwd);

    abup_memset(&mn_info, 0x00, sizeof(mn_info));
    
    if(apn)
    {
    	len = abup_strlen(apn);
		if(len > MAX_APN_LEN)
			len = MAX_APN_LEN;
		
        abup_strncpy((char *)(mn_info.apn), apn, len);

        mn_info.apn[len] = '\0';
    }      
    else  
    {   
        //default apn setting and save
        abup_strncpy((char *)(mn_info.apn),ABUP_DEFAULT_APN,strlen(ABUP_DEFAULT_APN));
        mn_info.apn[strlen(ABUP_DEFAULT_APN)] = '\0';
    }

    if(ERR_MNGPRS_NO_ERR!=(err=MNGPRS_SetPdpContextEx(dual_sys,context_param,ABUP_DEFAULT_CID,(abup_uint8 *)"IP",(abup_uint8 *)mn_info.apn, pdp_addr,0,0,0)))
    {
		abup_DebugPrint("ABUP: MNGPRS_SetPdpContextEx fail! err:%d", (abup_uint32)err);
        return ABUP_FALSE;
    }

    //USERNAME and password
    if(uname && upwd)
    {
    	len = abup_strlen(uname);
    	len1 = abup_strlen(upwd);
        if((len <= MAX_PAP_USER_LEN) && (len1 <= MAX_PAP_PASSWD_LEN))
        {
            abup_strncpy((abup_char *)user, uname, len);
            user[len] = '\0';

            abup_strncpy((abup_char *)passwd, upwd, len1);
            passwd[len1] = '\0';

            result = MNGPRS_SetPdpContextPcoEx(dual_sys,ABUP_DEFAULT_CID, user, passwd);   //default cid=1

            if(result != MN_GPRS_ERR_SUCCESS)
            {
                abup_DebugPrint("ABUP: set username or password error, result = %d", result);
                return ABUP_FALSE;//ERR_OPERATION_NOT_SUPPORTED;
            }
        }
        else
        {
            abup_DebugPrint("ABUP: username or password too long.");
            return ABUP_FALSE;//ERR_INVALID_INDEX;
        }
    }

	return ABUP_TRUE;//S_ATC_SUCCESS;
}


/******************************************************************************/
//  DESCRIPTION:This function handles command AT+SOCKET,  
//  Global resource dependence : 
//  AUTHOR: 
//  Note : 
/******************************************************************************/
abup_uint32 Abup_ProcessSOCKET(abup_uint8 sidx, abup_char *type, abup_char *domain, abup_uint16 port)
{
    abup_char *udp_str = "UDP";
    abup_char *udp_str1 = "udp";
    abup_char *tcp_str = "TCP";
    abup_char *tcp_str1 = "tcp";
    abup_uint8  id        = 0;
	abup_uint16 len = 0;
        
	abup_DebugPrint("Abup_ProcessSOCKET: soc idx: %d", sidx);
	if(0 <= sidx && sidx <= ABUP_MAX_DATACONF_NUM-1)
	{
	    id = sidx;

	    // if the socket is still alive, not allowed to change its parameters.
	    // u must firstly do AT+IPCLOSE=ID to close this socket
	    //if(NULL != s_dataconf[id].socket)
	    if(0 != s_abup_dataconf[id].socket)
	    {
	        //SCI_TRACE_LOW:"atc: socket %d is still alive, not allowed to change it"
	        return 21;//ERR_INVALID_INDEX;
	    }
	    
	    s_abup_dataconf[id].sock_index_id = id;
	}
	else
	{
	    return 4;//ERR_OPERATION_NOT_SUPPORTED;
	}

	//param 2 --- TYPE
    abup_DebugPrint("abup: connection type=%s", type);
	if(type)
	{
	    if((0 == abup_strcmp((abup_char *)type, udp_str)) ||
	       (0 == abup_strcmp((abup_char *)type, udp_str1)))
	    {
	        s_abup_dataconf[id].type = ABUP_UDP_DATACONF; 
	    }
	    else if((0==abup_strcmp((abup_char *)type, tcp_str))||
	            (0==abup_strcmp((abup_char *)type, tcp_str1)))
	    {
	        s_abup_dataconf[id].type = ABUP_TCP_DATACONF;  
	    }
	    else
	    {
	        return 4;//ERR_OPERATION_NOT_SUPPORTED;
	    }
	}

	//param 3 --- ADDR
	if(domain)
	{ 
		len = abup_strlen(domain);
		if(len > 64)
			len = 64;
		
	    if(NULL == s_abup_dataconf[id].addr_str)
	    {
	        s_abup_dataconf[id].addr_str = (abup_char *)abup_memalloc(len*sizeof(abup_char)+1); 
	        if(NULL==s_abup_dataconf[id].addr_str)
	        {
	            abup_DebugPrint("abup: malloc for sock idx: %d, IP address fail", id);
	            return 21;//ERR_INVALID_INDEX;
	        }

	        abup_DebugPrint("abup: sock idx: %d, IP address malloc here", id);
	    }
	    else
	    {
	        abup_DebugPrint("abup: sock idx: %d, IP address has been malloc, only memset it", id);
	        abup_memset(s_abup_dataconf[id].addr_str, 0x00, sizeof(s_abup_dataconf[id].addr_str));
	    }

	    abup_memcpy(s_abup_dataconf[id].addr_str, domain, len); 
		abup_DebugPrint("abup: sock idx: %d, addr_str:%s", id, s_abup_dataconf[id].addr_str);
	}

	//param 4 --- PORT
    if(0 <= port && port <= ABUP_MAX_PORT_NUM)
    {
        s_abup_dataconf[id].fport = port;   
        abup_DebugPrint("abup: socket idx: %d, port = %d", id, s_abup_dataconf[id].fport);
    }

	s_abup_dataconf[id].state = ABUP_SET; 

	return 0;

}


/******************************************************************************/
//  DESCRIPTION:This function handles command AT+IPOPEN, activate PDP firstly if it is 
//                         not  activated, if PDP is activated already, DNS directly    
//  Global resource dependence : 
//  AUTHOR: 
//  Note : 
/******************************************************************************/
abup_uint32 abup_ProcessIPOPEN(abup_uint8 sidx)
{
    abup_uint32 id = 0;
    MN_DUAL_SYS_E dual_sys = 0;//ATC_CURRENT_SIM_ID_MUX;
    abup_uint32  pdp_id_arr[MN_GPRS_MAX_PDP_CONTEXT_COUNT] = {0};
    
	abup_DebugPrint("ABUP: ProcessIPOPEN sock idx: %d", sidx);

    if(0 <= sidx && sidx <= ABUP_MAX_DATACONF_NUM-1)
    {
        id = sidx;                   

        if(s_abup_dataconf[id].sock_index_id != id/* || IDLE==s_dataconf[id].state*/)
        {
            abup_DebugPrint("abup: ProcessIPOPEN socidx %d is set previously!", sidx);
            return 4;//ERR_OPERATION_NOT_SUPPORTED;
        }

         if(0 != s_abup_dataconf[id].socket)
        {
            abup_DebugPrint("abup: ProcessIPOPEN socidx %d is still alive, not allowed to reopen it", sidx);
            return 100;//S_ATC_DEFAULT_HANDLE;
        }
    }
    else 
    {  
        abup_DebugPrint("abup: ProcessIPOPEN connect socket id error, not (0--9)");
        return 21;//ERR_INVALID_INDEX;
    }

#if 1//ndef ATC_SAT_ENABLE
    if(!s_abup_is_dns_pdp_actived)
    {
        MN_GPRS_CON_STATE_T *pdp_con_state_arr  = PNULL;
        ERR_MNGPRS_CODE_E   ret     = ERR_MNGPRS_NO_ERR;
        abup_uint32    pdp_con_num = 0;
        abup_uint32    i;

        ret = MNGPRS_ReadPdpContextStateEx(dual_sys, &pdp_con_state_arr, &pdp_con_num);
		abup_DebugPrint("abup: ProcessIPOPEN ReadPdpContextStateEx ret:%d", ret);
        if(ret != ERR_MNGPRS_NO_ERR)
        {
            if(pdp_con_state_arr != PNULL)
            {
                abup_memfree(pdp_con_state_arr);
                pdp_con_state_arr = PNULL;
            }
        }
        else
        {

            for(i = 0; i < pdp_con_num; i++)
            {
                if((pdp_con_state_arr[i].pdp_id == 1) && (pdp_con_state_arr[i].pdp_state == MN_CONTEXT_ACTIVATED))
                {
                    s_abup_is_dns_pdp_actived = TRUE;
					abup_DebugPrint("abup: ProcessIPOPEN s_abup_is_dns_pdp_actived=TRUE");
                    break;
                }
            }

            if(pdp_con_state_arr != PNULL)
            {
                abup_memfree(pdp_con_state_arr);
                pdp_con_state_arr = PNULL;
            }
        }

    }
#endif /*ATC_SAT_ENABLE*/ 
    
	abup_DebugPrint("abup: s_abup_is_dns_act_pdp %d, s_abup_is_dns_pdp_actived %d", s_abup_is_dns_act_pdp, s_abup_is_dns_pdp_actived);
	
    // PDP not activated, activate PDP firstly
    if(!s_abup_is_dns_pdp_actived)
    {
        ERR_MNGPRS_CODE_E ret = ERR_MNGPRS_NO_ERR;
        
        //SCI_TRACE_LOW:"atc:PDP not activated, id = %x is set"
            
        pdp_id_arr[0]=1;  //active NO.1 pdp id
        ret= MNGPRS_ActivatePdpContextEx(dual_sys, ABUP_FALSE, pdp_id_arr, POC_E, MN_UNSPECIFIED);
        //ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_ACTIVATE_PDP_CONTEXT_CNF,pdp_id_arr[0],ATC_DOMAIN_PS);
        //SCI_TRACE_LOW:"atc: active pdp ret =%d"

		abup_DebugPrint("abup: ProcessIPOPEN ActivatePdpContextEx ret:%d", ret);
        
        if(ERR_MNGPRS_NO_ERR != ret)
        {
            //SCI_TRACE_LOW:"atc: active pdp fail"
            return ERR_GPRS_OPERATION_FAILURE;
        }

        s_abup_sock_index_activate_pdp = id;  // record id to activate PDP //s_abup_sock_index_activate_pdp
        s_abup_is_dns_act_pdp = TRUE;
        //SCI_TRACE_LOW:"atc: s_is_pdp_active_flag = TRUE"
    }
    // PDP is already activated, directly DNS 
    else
    {
        TCPIP_HOST_HANDLE ret = 0;

        ret = sci_async_gethostbyname(s_abup_dataconf[id].addr_str, abup_task_id(),20000, s_abup_nsapi);
        abup_DebugPrint("abup: ProcessIPOPEN: requested IP= %s, ret request id=%d", s_abup_dataconf[id].addr_str,ret);

        if(0 == ret)
        {
            abup_DebugPrint("abup: ProcessIPOPEN: sock index %d DNS fail", id);
            return 100;//S_ATC_DEFAULT_HANDLE;
        }
        else
        {                               
            s_abup_dataconf[id].sock_request_id = ret;
        }
    }

	//abup_DebugPrint("abup: ProcessIPOPEN e");
     return 0;
}

abup_int32 abup_get_rcvd_block_size(void)
{
	return RECV_BLOCK_SIZE;
}

void abup_boot_hal_init(void)
{
	abup_DebugPrint_Ext("[ABUP_COMMON]:","abup curr ver: %s", abup_get_device_version());	
	abup_init_fs();

	abup_start_timer(5000, abup_net_cell_req, ABUP_FALSE);
	abup_start_timer(20*1000, abup_upgrade_thread_on_ex, ABUP_FALSE);
	abup_start_timer(60*1000, abup_get_new_version_ex, ABUP_FALSE);
}

void abup_test_nv(void)
{
	abup_uint32 write_buf=0;
	abup_uint32 read_buf=NULL;

	read_buf = abup_get_ipaddr();
	abup_DebugPrint("abup_test_nv-->read_buf:%d",read_buf);

	write_buf=12345678;
	abup_DebugPrint("abup_test_nv-->write_buf:%d",write_buf);
	abup_set_ipaddr(write_buf);

	read_buf = abup_get_ipaddr();
	abup_DebugPrint("abup_test_nv-->read_buf:%d",read_buf);
}

void abup_test_nv_flash(void)
{
	abup_DebugPrint("abup_test_nv_flash-->");

	abup_test_nv();
}

void abup_write_delta_to_flash(void)
{
	abup_int32 handerFile;
	abup_int32 ret;
	abup_uint32 rlen = 0, tlen = 0;
	abup_char pbuf[256] = {0};

	handerFile = Abup_Open_File(L"E:\\@fota_abup\\abupfota.delta", ABUP_READONLY);
	abup_DebugPrint("abup_write_delta_to_flash: handerFile = %d FS_Open result!",handerFile);
	if (handerFile < 0)
	{
		return;
	}

	abup_init_flash();

	ret = Abup_Read_File(handerFile, pbuf, 256, &rlen);
	while (ret != -1 && rlen > 0)
	{
		abup_write_flash(tlen, pbuf, rlen);
		tlen += rlen;
		Abup_Seek_File(handerFile, tlen, ABUP_SEEK_START);
		ret = Abup_Read_File(handerFile, pbuf, 256, &rlen);
	}

	Abup_Close_File(handerFile);
}

#endif


