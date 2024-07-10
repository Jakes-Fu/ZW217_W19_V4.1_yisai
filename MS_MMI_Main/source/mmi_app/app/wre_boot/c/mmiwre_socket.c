#include "mmi_app_wre_boot_trc.h"
#ifdef WRE_SUPPORT

#include "sci_types.h"
#include "sci_api.h"
#include "os_api.h"
#include "mn_type.h"
#include "mmk_type.h"
#include "mmi_module.h"
#include "mmk_app.h"
#include "in_Message.h"
#include "tasks_id.h"
#include "app_tcp_if.h"
#include "mmiwre_socket.h"
#include "mmiwre_adaptor.h"
#include "mmiwre_event.h"
#include "mmiwre_other.h"

extern void WRE_MSG_Charge_Handle(uint32 msg);
extern void WRE_MSG_USB_Handle(uint32 msg);
extern void WBOOT_MSG_Charge_Handle(uint32 msg);
extern void WBOOT_MSG_USB_Handle(uint32 msg);


/*****************************************************************************/
//  Description : adapt_socktest
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

/*检测变量参数及宏是否有改变*/
#ifdef DEBUG
void adapt_socktest()
{
    SCI_ASSERT(ADAPT_AS_READ == AS_READ);           /*assert verified*/
    SCI_ASSERT(ADAPT_AS_WRITE == AS_WRITE);         /*assert verified*/
    SCI_ASSERT(ADAPT_AS_CONNECT == AS_CONNECT);     /*assert verified*/
    SCI_ASSERT(ADAPT_AS_CLOSE == AS_CLOSE);         /*assert verified*/
    SCI_ASSERT(ADAPT_AS_ACCEPT == AS_ACCEPT);       /*assert verified*/
    SCI_ASSERT(ADAPT_AS_READ_BUFFER_STATUS == AS_READ_BUFFER_STATUS);   /*assert verified*/
    SCI_ASSERT(ADAPT_AS_FULLCLOSE == AS_FULLCLOSE); /*assert verified*/
}
#endif


/*****************************************************************************/
//  Description : adapt_sock_socket
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

int adapt_sock_socket(
    int family, int type, int proto, uint32 net_id )
{
	return (int)sci_sock_socket(family,type,proto,net_id);
}


/*****************************************************************************/
//  Description : adapt_sock_connect
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

int adapt_sock_connect(
    int so, void* addr_ptr, int addr_len )
{
	return sci_sock_connect(so,(struct sci_sockaddr *)addr_ptr,addr_len);
}


/*****************************************************************************/
//  Description : adapt_sock_errno
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

int adapt_sock_errno(int so)
{
	return sci_sock_errno(so);
}


/*****************************************************************************/
//  Description : adapt_gethostbyname
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

void * adapt_gethostbyname(char *name_ptr,uint32 net_id)
{
	/*目前版本的结构大小一致，将来需要修改时，请在这里调整*/
	return (void*)sci_gethostbyname_ext(name_ptr,net_id);
}


/*****************************************************************************/
//  Description : adapt_gethostname
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

int adapt_gethostname(char *hostname, int namelen)
{
	return sci_gethostname(hostname,namelen);
}


/*****************************************************************************/
//  Description : adapt_sock_socketclose
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

int adapt_sock_socketclose( int so )
{
	return sci_sock_socketclose(so);
}


/*****************************************************************************/
//  Description : adapt_sock_send
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

int adapt_sock_send( int so, char* buf, int len, int flags )
{
	return sci_sock_send(so,buf,len,flags);
}


/*****************************************************************************/
//  Description : adapt_sock_recv
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

int adapt_sock_recv( int so, char* buf, int len, int flag )
{
	return sci_sock_recv(so,buf,len,flag);
}


/*****************************************************************************/
//  Description : adapt_sock_select
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

int adapt_sock_select(
    void *    in,
    void *    out,
    void *    ex,
    long            tv)
{
	return sci_sock_select((sci_fd_set *)in,(sci_fd_set*)out,(sci_fd_set*)ex,tv);
}


/*****************************************************************************/
//  Description : adapt_sock_sendto
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

int adapt_sock_sendto(
    int so, char* buf, int len, int flag, void * to )
{
	return sci_sock_sendto(so,buf,len,flag,(struct sci_sockaddr* )to);
}


/*****************************************************************************/
//  Description : adapt_sock_recvfrom
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

int adapt_sock_recvfrom(
    int so, char* buf, int len, int flag, void * from )
{
	return sci_sock_recvfrom(so,buf,len,flag,(struct sci_sockaddr*)from);
}


/*****************************************************************************/
//  Description : adapt_sock_setsockopt
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

int adapt_sock_setsockopt( int so, int name, void* arg_ptr )
{
	return sci_sock_setsockopt(so,name,arg_ptr);
}

/*****************************************************************************/
//  Description : adapt_sock_getsockopt
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

int adapt_sock_getsockopt( int so, int name, void* arg_ptr )
{
	return sci_sock_getsockopt(so,name,arg_ptr);
}

/*****************************************************************************/
//  Description : adapt_sock_listen
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

int adapt_sock_listen( int so, int backlog )
{
	return sci_sock_listen(so,backlog);
}

/*****************************************************************************/
//  Description : adapt_sock_bind
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

int adapt_sock_bind(int so, void * addr_ptr, int addr_len )
{
	return sci_sock_bind(so,(struct sci_sockaddr*)addr_ptr,addr_len);
}

/*****************************************************************************/
//  Description : adapt_sock_accept
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

int adapt_sock_accept(int so, void* addr_ptr, int* addr_len )
{
	return sci_sock_accept(so,(struct sci_sockaddr*)addr_ptr,addr_len);
}

/*****************************************************************************/
//  Description : adapt_sock_shutdown
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

int adapt_sock_shutdown( int so, int how )
{
	return sci_sock_shutdown(so,how);
}

/*****************************************************************************/
//  Description : adapt_sock_asyncrecv
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

int adapt_sock_asyncrecv(
    int             so,
    char*           buf,
    int             len, 
    int             flag, 
    int*            left_len_ptr)
{
	return sci_sock_asyncrecv(so,buf,len,flag,left_len_ptr);
}

/*****************************************************************************/
//  Description : adapt_sock_asyncrecvfrom
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

PUBLIC int adapt_sock_asyncrecvfrom(
    int             so, 
    char*           buf, 
    int             len,
    int             flag,
    void*           from, 
    int*            left_len_ptr)
{
	return sci_sock_asyncrecvfrom(so,buf,len,flag,(struct sci_sockaddr*)from,left_len_ptr);
}

/*****************************************************************************/
//  Description : adapt_sock_asyncselect
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

int adapt_sock_asyncselect(
    int so, uint32 task_id, uint32 event)
{
	return sci_sock_asyncselect(so,task_id,event);
}

/*****************************************************************************/
//  Description : adapt_async_gethostbyname
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

void * adapt_async_gethostbyname(
    char*           name_ptr,
    uint32          task_id,
    uint32          time_out,
    uint32          netid )
{
	return (void*)sci_async_gethostbyname(name_ptr,task_id,time_out,netid);
}

/*****************************************************************************/
//  Description : adapt_sock_getpeername
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

int adapt_sock_getpeername(
    int so, void * addr_ptr )
{
	return sci_sock_getpeername(so,(struct sci_sockaddr*)addr_ptr);
}

/*****************************************************************************/
//  Description : adapt_sock_getsockname
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

int adapt_sock_getsockname(int so,void* addr_ptr)
{
	return sci_sock_getsockname(so,(struct sci_sockaddr*)addr_ptr);
}


/*****************************************************************************/
//  Description : adapt_gethostaddress
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

uint32 adapt_gethostaddress( void )
{
	return sci_gethostaddress();
}

/*****************************************************************************/
//  Description : adapt_gethostaddress_ext
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

uint32 adapt_gethostaddress_ext( uint32 netid )
{
	return sci_gethostaddress_ext(netid);
}

/*****************************************************************************/
//  Description : adapt_inet_addr
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

unsigned long adapt_inet_addr(char * cp)
{
	unsigned long ip = 0;
	if(0 == inet_aton(cp,&ip))
		return 0;

     return ip;
}

/*****************************************************************************/
//  Description : adapt_inet_ntoa
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

char * adapt_inet_ntoa(unsigned long in)
{
    return inet_ntoa(in);
}

/*****************************************************************************/
//  Description : adapt_htonl
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

uint32 adapt_htonl(uint32 l)
{
	return htonl(l);
}

/*****************************************************************************/
//  Description : adapt_ntohl
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

uint32 adapt_ntohl(uint32 l)
{
	return ntohl(l);
}

/*****************************************************************************/
//  Description : adapt_ntohs
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

uint16 adapt_ntohs(uint16 s)
{
	return ntohs(s);
}

/*****************************************************************************/
//  Description : adapt_htons
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

uint16 adapt_htons(uint16 s)
{
	return htons(s);
}

/*****************************************************************************/
//  Description : Adapt_PostMessage
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

//=================== asyncmsg.c ====================
void Adapt_PostMessage(uint32 hWnd,uint32 wMsg,uint32 wParam,uint32 lParam)
{
    MmiOtherTaskMsgS *sendSignal = PNULL;
	uint32     *wre_run_flag;

	//remember we extend the mmiother signal for checking the message if it is valid.
    MmiCreateSignal(APP_WASYN_MSG_IND, (sizeof(MmiOtherTaskMsgS)+4), (MmiSignalS**)&sendSignal);
    if (sendSignal == PNULL)
		return ;

    sendSignal->Sender = WBOOT_MWIN_GetAsyncTaskId();

    sendSignal->handle         = hWnd;
    sendSignal->msg_id         = wMsg;
    sendSignal->param_ptr      = (void *)wParam;
    sendSignal->size_of_param  = lParam;

	wre_run_flag = (uint32*)((char *)((uint32)sendSignal+sizeof(MmiOtherTaskMsgS)));
	*wre_run_flag = WRE_GetWreRuningFlag();

    MmiSendSignal(P_APP, (MmiSignalS*)sendSignal);

}


/*异步socket消息*/
LOCAL MMI_RESULT_E MWIN_ASYNCSOCK_HandlePsMsg( PWND app_ptr, uint16 msg_id, DPARAM param);

PUBLIC MMI_APPLICATION_T       g_wre_asyn_app = {MWIN_ASYNCSOCK_HandlePsMsg, CT_APPLICATION, PNULL}; 



// g_caf_asyn_app define
/*****************************************************************************/
//  Description : process ps signal.
//  Global resource dependence :
//  Author:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MWIN_ASYNCSOCK_HandlePsMsg( PWND app_ptr, uint16 msg_id, DPARAM param)
{
    MMI_RESULT_E      result = MMI_RESULT_FALSE;
    MmiOtherTaskMsgS* signal_ptr = (MmiOtherTaskMsgS*)param;
    uint32             msg = CHR_MSG_CHARGE_FAULT_IND;
    uint32             msg = CHR_MSG_CHARGE_FAULT_OVERTEMP_IND;

    //UNUSED(param);	// changed by yifei

    //如果WRE没有启动，直接返回
	if(FALSE == WBOOT_IsWRERunning())
	{
		return MMI_RESULT_FALSE;
    }

    if( APP_WASYN_MSG_IND == msg_id )
    {
		uint32 * wre_run_flags = (uint32*)((char*)signal_ptr + sizeof(MmiOtherTaskMsgS));

		if(*wre_run_flags == WRE_GetWreRuningFlag())
		{
			SCI_ASSERT( PNULL != signal_ptr );  /*assert verified*/
			//SCI_TRACE_LOW:"ASYNCSOCK_HandlePsMsg, signal_ptr->msg_id = %lx"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_SOCKET_456_112_2_18_3_10_59_191,(uint8*)"d", signal_ptr->msg_id);
			WBOOT_MWIN_XXX_PostMessage((uint32)signal_ptr->handle,(uint32)signal_ptr->msg_id,(uint32)(signal_ptr->param_ptr),(uint32)(signal_ptr->size_of_param));
		}
	}
	else if(msg_id >= CHR_MSG_CAP_IND && msg_id <= CHR_MSG_CHARGE_FAULT_OVERTEMP_IND)
	{
		switch(msg_id)
		{
			//CHARGER 消息
			case CHR_MSG_CAP_IND:
				msg = WRE_CHR_MSG_CAP_IND;
				break;
			case CHR_MSG_CHARGE_START_IND:
				msg = WRE_CHR_MSG_CHARGE_START_IND;
				break;

			case CHR_MSG_CHARGE_END_IND:
				msg = WRE_CHR_MSG_CHARGE_END_IND;
				break;

			case CHR_MSG_WARNING_IND:
				msg = WRE_CHR_MSG_WARNING_IND;
				break;
			case CHR_MSG_SHUTDOWN_IND:
				msg = WRE_CHR_MSG_SHUTDOWN_IND;
				break;

			case CHR_MSG_CHARGE_FINISH_IND:
				msg = WRE_CHR_MSG_CHARGE_FINISH_IND;
				break;

			case CHR_MSG_CHARGE_DISCONNECT_IND:
				msg = WRE_CHR_MSG_CHARGE_DISCONNECT_IND;
				break;
			case CHR_MSG_CHARGE_FAULT_IND:
				msg = WRE_CHR_MSG_CHARGE_FAULT_IND;
				break;
			case CHR_MSG_CHARGE_FAULT_OVERTEMP_IND:
				msg = WRE_CHR_MSG_CHARGE_FAULT_OVERTEMP_IND;
				break;
			default:
				break;
		}


		WRE_MSG_Charge_Handle(msg);


	}
	else if(msg_id >= USB_PLUGIN_IND && msg_id <= USB_SERVICE_UPCC_STOPPED_IND)
	{
		switch(msg_id)
		{
					//USB 消息
			case USB_PLUGIN_IND:
				msg = WRE_USB_PLUGIN_IND;
				break;
				
			case USB_PLUGOUT_IND:
				msg = WRE_USB_PLUGOUT_IND;
				break;
				
			case USB_READY_IND:
				msg = WRE_USB_READY_IND;
				break;
			case USB_ERROR_IND:
				msg = WRE_USB_ERROR_IND;
				break;
					
			case USB_SERVICE_UDISK_STARTED_IND:
				msg = WRE_USB_SERVICE_UDISK_STARTED_IND;
				break;
				
			case USB_SERVICE_UDISK_STOPPED_IND:
				msg = WRE_USB_SERVICE_UDISK_STOPPED_IND;
				break;
				
			case USB_SERVICE_UCOM_STARTED_IND:
				msg = WRE_USB_SERVICE_UCOM_STARTED_IND;
				break;
				
			case USB_SERVICE_UCOM_STOPPED_IND:
				msg = WRE_USB_SERVICE_UCOM_STOPPED_IND;
				break;
				
			case USB_SERVICE_UPCC_STARTED_IND:
				msg = WRE_USB_SERVICE_UPCC_STARTED_IND;
				break;
				
			case USB_SERVICE_UPCC_STOPPED_IND:
				msg = WRE_USB_SERVICE_UPCC_STOPPED_IND;
				break;

			case USB_SERVICE_LOG_STARTED_IND:
				msg = WRE_USB_SERVICE_LOG_STARTED_IND;
				break;
				
			case USB_SERVICE_LOG_STOPPED_IND:
				msg = WRE_USB_SERVICE_LOG_STOPPED_IND;
				break;
			default:
				break;
		}
	
		WRE_MSG_USB_Handle(msg);
	
	}
	else if(msg_id > HTTP_SIGNAL_BEG && msg_id < HTTP_SIGNAL_END)
	{
//		WRE_MSG_HTTP_Handle(msg_id,(void*)signal_ptr);

		return result;
	
	}
	else
	{
		result = MMI_RESULT_FALSE;
	}
    
    return result;
}


#define MWINSOCK_TASK_PRIORITY   SCI_SYSTEM_PRI(26)
#define MWINSOCK_TASK_STACK_SIZE 2096
#define MWINSOCK_TASK_QUEUE_NUM 500
/*****************************************************************************/
//  Description : MWIN_HandleSocketSig handle all socket msg from ps
//  Global resource dependence : 
//  Note:
/*****************************************************************************/
LOCAL void MWIN_HandleSocketSig(xSignalHeaderRec * signal_ptr)
{
	int                     sock = -1;
	int                     error = -1;
	int                     event = AS_NULL;
	uint32                  request_id;

	switch(signal_ptr->SignalCode)
	{
		case SOCKET_READ_EVENT_IND:
			{
				sock = ((SOCKET_READ_EVENT_IND_SIG_T *)signal_ptr)->socket_id;
				
				event = ADAPT_EVENT_READ;
				error = 0;
				WBOOT_MWIN_XXX_HandleSocketMsg(sock,event,error,NULL);
			}
			break;

		case SOCKET_WRITE_EVENT_IND:
			{
				sock = ((SOCKET_WRITE_EVENT_IND_SIG_T *)signal_ptr)->socket_id;
				
				
				event = ADAPT_EVENT_WRITE;
				error = 0;
				WBOOT_MWIN_XXX_HandleSocketMsg(sock,event,error,NULL);

			}
			break;

		case SOCKET_CONNECT_EVENT_IND:
			{
				sock = ((SOCKET_CONNECT_EVENT_IND_SIG_T *)signal_ptr)->socket_id;
				
				event = ADAPT_EVENT_CONNECT;
				error = ((SOCKET_CONNECT_EVENT_IND_SIG_T *)signal_ptr)->error_code;

				WBOOT_MWIN_XXX_HandleSocketMsg(sock,event,error,NULL);
			}
			break;

		case SOCKET_CONNECTION_CLOSE_EVENT_IND:
			{
				sock = ((SOCKET_CONNECTION_CLOSE_EVENT_IND_SIG_T *)signal_ptr)->socket_id;
				
				event = ADAPT_EVENT_CONNECTION_CLOSE_EVENT;
				error = ((SOCKET_CONNECTION_CLOSE_EVENT_IND_SIG_T *)signal_ptr)->error_code;

				WBOOT_MWIN_XXX_HandleSocketMsg(sock,event,error,NULL);
			}
			break;

		case SOCKET_ACCEPT_EVENT_IND:
			{

				sock = ((SOCKET_ACCEPT_EVENT_IND_SIG_T *)signal_ptr)->socket_id;
				
				event = ADAPT_EVENT_ACCEPT;
				error = 0;

				WBOOT_MWIN_XXX_HandleSocketMsg(sock,event,error,NULL);				
			}
			break;

		case SOCKET_FULL_CLOSED_EVENT_IND:
			{
				sock = ((SOCKET_FULL_CLOSED_EVENT_IND_SIG_T *)signal_ptr)->socket_id;
				
				event = ADAPT_EVENT_FULL_CLOSED;
				error = ((SOCKET_FULL_CLOSED_EVENT_IND_SIG_T *)signal_ptr)->close_reason;
				
				WBOOT_MWIN_XXX_HandleSocketMsg(sock,event,error,NULL);				
			}
			break;

		case SOCKET_ASYNC_GETHOSTBYNAME_CNF:
			{
				void * param = NULL;
				ASYNC_GETHOSTBYNAME_CNF_SIG_T * p = (ASYNC_GETHOSTBYNAME_CNF_SIG_T *)signal_ptr;
				
				request_id = ((ASYNC_GETHOSTBYNAME_CNF_SIG_T *)signal_ptr)->request_id;
				event = ADAPT_EVENT_ASYNC_GETHOSTBYNAME;
				error = ((ASYNC_GETHOSTBYNAME_CNF_SIG_T *)signal_ptr)->error_code;
				if(error == 0)
				{
					param = &p->hostent;
				}					
	
				WBOOT_MWIN_XXX_HandleSocketMsg((int)request_id,event,error,param);				
			}
			break;

		default:
			break;

	}

}

/*****************************************************************************/
//  Description : MWIN_Sock Task
//  Global resource dependence : 
//  Note:
/*****************************************************************************/
LOCAL void MWINSockTaskEntry (uint32 argc, void * argv)
{
	xSignalHeaderRec		*sig_ptr = 0;
	uint32      this_thread_id = SCI_IdentifyThread();
	uint8       bFlags = 1;

	while(1)/*lint !e716*/
	{
		sig_ptr = SCI_GetSignal(this_thread_id);

		switch(sig_ptr->SignalCode)
		{
		case SOCKET_READ_EVENT_IND:
		case SOCKET_WRITE_EVENT_IND:
		case SOCKET_CONNECT_EVENT_IND:
		case SOCKET_ACCEPT_EVENT_IND:
		case SOCKET_CONNECTION_CLOSE_EVENT_IND:
		case SOCKET_ASYNC_GETHOSTBYNAME_CNF:
			//handle socket related signal
			//SCI_TRACE_LOW:"MWIN TASK ENTRY SOCK, EVENT:%d"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_SOCKET_699_112_2_18_3_10_59_192,(uint8*)"d",sig_ptr->SignalCode);
			MWIN_HandleSocketSig(sig_ptr);

			break;

		case 0xFFF0:
			bFlags = 0;
			break;
		
		default:
			//SCI_TRACE_LOW:"DEFAULT:MWIN TASK ENTRY APP, EVENT:%d"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_SOCKET_709_112_2_18_3_10_59_193,(uint8*)"d",sig_ptr->SignalCode);
			break;
		}
		SCI_FREE(sig_ptr);

		if(bFlags == 0)
		{
			break;
		}
	}
	SCI_ThreadExit();
}


/*****************************************************************************/
//  Description : Adapt_InitSockTask
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

uint32 Adapt_InitSockTask(void)
{
	return SCI_CreateAppThread(
		"MWINSOCK",
		"MWINSOCK_QUEUE",
		MWINSockTaskEntry,
		0,0,
		MWINSOCK_TASK_STACK_SIZE,
		MWINSOCK_TASK_QUEUE_NUM,
		MWINSOCK_TASK_PRIORITY,            
		SCI_PREEMPT, 
		SCI_AUTO_START
		);
}


/*****************************************************************************/
//  Description : Adapt_DeinitSockTask
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

void Adapt_DeinitSockTask(uint32 task_id)
{
	MmiSignalS *sendSignal = PNULL;
	
	MmiCreateSignal(0xFFF0, sizeof(MmiSignalS), &sendSignal);
	
	sendSignal->Sender = task_id;

	SCI_SendSignal((xSignalHeader)sendSignal,task_id);
	SCI_Sleep(20);
}

#endif
