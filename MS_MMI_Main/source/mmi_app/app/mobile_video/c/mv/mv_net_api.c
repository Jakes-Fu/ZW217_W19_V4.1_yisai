#define _MV_NET_API_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_mobile_video_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "sfs.h"
#include "mv_debug.h"
#include "sci_api.h"
#include "mv_net_api.h"
#include "mv_timer_api.h"
#include "socket_types.h"
#include "socket_api.h"
#include "mn_type.h"
#include "mmipdp_export.h"
#include "mmi_module.h"
#include "in_message.h"
#include "tcpip_types.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mv_base_api.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define  NetHandle      888

#define    MV_SOCK_STREAM 0  //for UDP
#define    MV_SOCK_DGRAM   1   //for TCP
#define    MV_SOCK_RAW	    2


/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
SocketCallback  SocketCallbackproc;
NetWorkCallback NetWorkCallbackproc;

static TNetworkStatus         s_mv_pdp_status = ENetworkStatusCount; 
static TRMEAPP_IAP             s_mv_connect_type = RMEAPP_IAP_CMWAP;
uint32 s_mv_pdp_net_id = 0;


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

                                                 
/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : HandlePdpActiveConfirm
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void HandlePdpActiveConfirm(MMIPDP_CNF_INFO_T *msg_ptr);
/*****************************************************************************/
//  Description : HandleMVPDPMsg
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void HandleMVPDPMsg(MMIPDP_CNF_INFO_T *msg_ptr);


/*****************************************************************************/
//  Description : MV_SetPdpActiveStatus
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void  MV_SetPdpActiveStatus(TNetworkStatus   pdp_status);

/*****************************************************************************/
//  Description : MV_SetPdpConnectType
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void MV_SetPdpConnectType(TRMEAPP_IAP nIAP);

/*****************************************************************************/
//  Description : MV_GetPdpConnectType
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL TRMEAPP_IAP MV_GetPdpConnectType(void);
/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : CM_Socket_Create
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_HANDLE CM_Socket_Create(_UINT nSocketPort, 
                         _INT nSOcketType , 
                         SocketCallbackProc pCallbackProc,
                         _VOID* pCallbackProcParam)
{
     int32 socket_id = INVALID_SOCKET;
     int32 sock_type = 0;
     int iError = 0;

    
    switch(nSOcketType)
    {
    case 1:
       sock_type = SOCK_STREAM; //tcp 
       break;
       
    default:
       sock_type = SOCK_STREAM; //tcp 
#ifdef SOCK_DEBUG
        //MV_TRACE_LOW:"[MV]--WD[CM_Socket_Create],socket_type error,we are use tcp socket"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_NET_API_108_112_2_18_2_39_14_83,(uint8*)"");
#endif
       break;
    };
     
    socket_id  = sci_sock_socket(AF_INET, sock_type, 0, s_mv_pdp_net_id);
   
    if(INVALID_SOCKET == socket_id)
    { 
#ifdef SOCK_DEBUG
         //MV_TRACE_LOW:"[MV]--WD[CM_Socket_Create], Create socket INVALID e = %d"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_NET_API_118_112_2_18_2_39_14_84,(uint8*)"d",sci_sock_errno((long)socket_id));
#endif 
         socket_id = -1;
         SocketCallbackproc.CM_SocketCallback  = NULL;
         SocketCallbackproc.pSocketParam = 0;
         return NULL;
        
    }
    else
    {
        if(MV_GetMVSocketTaskID()==0)
        {
#ifdef SOCK_DEBUG        
           	//MV_TRACE_LOW:"[MV]--WD[CM_Socket_Create]socket task ID is  INVALID"
           	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_NET_API_131_112_2_18_2_39_14_85,(uint8*)"");
#endif            	
        }
        else
        {
#ifdef SOCK_DEBUG                 
            //MV_TRACE_LOW:"[MV]--WD[CM_Socket_Create] socket  thread ID = %x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_NET_API_137_112_2_18_2_39_14_86,(uint8*)"d",MV_GetMVSocketTaskID());
#endif 
	    	iError = sci_sock_asyncselect(socket_id,MV_GetMVSocketTaskID(),AS_READ|AS_WRITE|AS_CONNECT|AS_CLOSE);
	        SocketCallbackproc.CM_SocketCallback  = pCallbackProc;
	        SocketCallbackproc.pSocketParam = (int)pCallbackProcParam;
#ifdef SOCK_DEBUG  
	        //MV_TRACE_LOW:"[MV]--WD[CM_Socket_Create],socket_id = %x"
	        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_NET_API_143_112_2_18_2_39_14_87,(uint8*)"d",socket_id);
#endif 
        }
    }
    return (_HANDLE)socket_id;

}

/*****************************************************************************/
//  Description : CM_Socket_Connect
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_BOOLEAN CM_Socket_Connect(_HANDLE hSocket, _CHAR*dwIPv4Addr, _UINT nHostPort)
{
    struct sci_sockaddr sa = {0};
	uint32 IPAddr = 0;
	int err = 0;
	short int sock_state = 0;
	_BOOLEAN flag = _FALSE;
	int ret = 0;
	uint32 IPAddress = 0;
	//int reconnect=3;
#ifdef SOCK_DEBUG
	//MV_TRACE_LOW:"[MV]--WD[CM_Socket_Connect] ip dwIPv4Addr %s, nHostPort[%d]"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_NET_API_166_112_2_18_2_39_14_88,(uint8*)"sd",dwIPv4Addr,nHostPort);
#endif

    if (PNULL == dwIPv4Addr)
    {
        return _FALSE;
    }

	IPAddr = *((int*)dwIPv4Addr);
#ifndef CHIP_ENDIAN_LITTLE    
	IPAddress = lswap(IPAddr);
#ifdef SOCK_DEBUG
	//MV_TRACE_LOW:"[MV]-NOT -CHIP_ENDIAN_LITTLE-WD[CM_Socket_Connect]IPAddress[0x%x]"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_NET_API_178_112_2_18_2_39_14_89,(uint8*)"d",IPAddress);
#endif
#else
    IPAddress = IPAddr;
#ifdef SOCK_DEBUG
	//MV_TRACE_LOW:"[MV]-CHIP_ENDIAN_LITTLE-WD[CM_Socket_Connect] IPAddress[0x%x]"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_NET_API_183_112_2_18_2_39_14_90,(uint8*)"d",IPAddress);
#endif
#endif

#ifdef SOCK_DEBUG
	//MV_TRACE_LOW:"[MV]--WD[CM_Socket_Connect] ip address %x, Tempaddr= %x, nHostPort = %x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_NET_API_188_112_2_18_2_39_14_91,(uint8*)"ddd",IPAddr,IPAddress,nHostPort);
#endif

	sa.family = AF_INET;
	sa.ip_addr = htonl(IPAddress);
	sa.port = htons(nHostPort);
#ifdef SOCK_DEBUG
	//MV_TRACE_LOW:"[MV]--WD[CM_Socket_Connect] sa.family=0x%x ,sa.ip_addr=0x%x,sa.port=0x%x,sa.sa_data=%s"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_NET_API_195_112_2_18_2_39_14_92,(uint8*)"ddds",sa.family,sa.ip_addr,sa.port,sa.sa_data);
#endif

	err = sci_sock_connect((long)hSocket, &sa, sizeof(sa));

	if(err!=0)
    {
		ret=sci_sock_errno((long)hSocket);
		switch(ret)
        {
		case EINPROGRESS:
			flag = _TRUE;
			break;

		default:
			  flag = FALSE;
			  break;
		}
#ifdef SOCK_DEBUG		
		//MV_TRACE_LOW:"[MV]--WD[CM_Socket_Connect]  IP=%x, Port=%x, connect error, err = %d, ret = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_NET_API_214_112_2_18_2_39_14_93,(uint8*)"dddd", sa.ip_addr, sa.port,err,ret);
#endif		
	}
    else
    {
		 err =sci_sock_getsockstate((long)hSocket,&sock_state);
		 if((err == 0) && (sock_state & SS_ISCONNECTED))
		 {
			flag = _TRUE;
#ifdef SOCK_DEBUG
			//MV_TRACE_LOW:"[MV]--WD[CM_Socket_Connect] connect network use tcp socket ok"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_NET_API_224_112_2_18_2_39_14_94,(uint8*)"");
#endif						
		 }
         else
         {
			 flag = _FALSE;
#ifdef SOCK_DEBUG			  
            //MV_TRACE_LOW:"[MV]--WD[CM_Socket_Connect] 2 connect network use tcp socket error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_NET_API_231_112_2_18_2_39_14_95,(uint8*)"");
#endif            
		 }
	}
	return flag;

}

/*****************************************************************************/
//  Description : CM_Socket_Release
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_BOOLEAN CM_Socket_Release(_HANDLE hSocket)
{
#ifdef SOCK_DEBUG
	//MV_TRACE_LOW:"[MV]--WD[CM_Socket_Release] Release socket"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_NET_API_246_112_2_18_2_39_14_96,(uint8*)"");
#endif
	return _TRUE;
}

/*****************************************************************************/
//  Description : CM_Socket_Close
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_BOOLEAN CM_Socket_Close(_HANDLE hSocket)
{
    sci_sock_socketclose((long)hSocket);
    hSocket = 0;
#ifdef SOCK_DEBUG
    //MV_TRACE_LOW:"[MV]WD[CM_Socket_Close] close socket"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_NET_API_260_112_2_18_2_39_14_97,(uint8*)"");
#endif
    return _TRUE;
}

/*****************************************************************************/
//  Description : CM_Socket_Receive
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_INT CM_Socket_Receive(_HANDLE hSocket, _VOID* pBuf, _INT nBufSize)
{
    //_BOOLEAN  result = _TRUE;
    _INT nResLen = 0;    

    if (PNULL == pBuf)
    {
        return 0;
    }
#ifdef SOCK_DEBUG
    //MV_TRACE_LOW:"[MV]--WD[CM_Socket_Receive]nBufSize = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_NET_API_279_112_2_18_2_39_14_98,(uint8*)"d",nBufSize);
#endif  
    nResLen = sci_sock_recv((long)hSocket, (char*)pBuf, nBufSize,0);
#ifdef SOCK_DEBUG
    //MV_TRACE_LOW:"[MV]--WD[CM_Socket_Receive]: nResLen=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_NET_API_283_112_2_18_2_39_14_99,(uint8*)"d", nResLen);
#endif

    return nResLen;
}

/*****************************************************************************/
//  Description : CM_Socket_Send
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_INT CM_Socket_Send(_HANDLE hSocket, _VOID* pBuf, _INT nBufSize)
{
    unsigned int sent_len = 0;
    unsigned int total_len = nBufSize;
    unsigned int length = 0;

    if (PNULL == pBuf )
    {
        return _FALSE;
    }
    
#ifdef SOCK_DEBUG
    //MV_TRACE_LOW:"[MV]--WD[CM_Socket_Send], total_len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_NET_API_305_112_2_18_2_39_15_100,(uint8*)"d",total_len);
#endif
    while( total_len > 0)
    {
        
        length = sci_sock_send((long)hSocket, (char *)pBuf + sent_len, total_len, 0);
        total_len -= length;
        sent_len += length; 
    }
#ifdef SOCK_DEBUG
    //MV_TRACE_LOW:"[MV]--WD[CM_Socket_Send], sent_len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_NET_API_315_112_2_18_2_39_15_101,(uint8*)"d",sent_len);
#endif
   return sent_len;

}

/*****************************************************************************/
//  Description : MV_GetPdpConnectType
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL TRMEAPP_IAP MV_GetPdpConnectType(void)
{
    return s_mv_connect_type;
}
/*****************************************************************************/
//  Description : MV_SetPdpConnectType
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void MV_SetPdpConnectType(TRMEAPP_IAP nIAP )
{
    s_mv_connect_type = nIAP;   
#ifdef MV_NET_DEBUG
    //MV_TRACE_LOW:"[MV]--WD[MV_SetPdpConnectType] Set Pdp Connect Type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_NET_API_337_112_2_18_2_39_15_102,(uint8*)"d",s_mv_connect_type);
#endif  
}

/*****************************************************************************/
//  Description : MV_SetPdpActiveStatus
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void  MV_SetPdpActiveStatus(TNetworkStatus   pdp_status)
{
#ifdef MV_NET_DEBUG
    //MV_TRACE_LOW:"[MV]--WD[MV_SetPdpActiveStatus]==> set pdp status = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_NET_API_348_112_2_18_2_39_15_103,(uint8*)"d",pdp_status);
#endif
    s_mv_pdp_status = pdp_status;

}

/*****************************************************************************/
//  Description : HandlePdpActiveConfirm
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void HandlePdpActiveConfirm(MMIPDP_CNF_INFO_T *msg_ptr)
{   
    if (PNULL == msg_ptr)
    {
        return;
    }

#ifdef MV_NET_DEBUG
    //MV_TRACE_LOW:"[MV]--WD[HandlePdpActiveCnf]==> pdp active result =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_NET_API_366_112_2_18_2_39_15_104,(uint8*)"d",msg_ptr->result);
#endif
    
    //get pdp active cnf 
    if (MMIPDP_RESULT_SUCC == msg_ptr->result)
    {
        s_mv_pdp_net_id = msg_ptr->nsapi;
        //pdp active success
#ifdef MV_NET_DEBUG
        //MV_TRACE_LOW:"[MV]--WD[HandlePdpActiveCnf]  pdp Active successfully"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_NET_API_375_112_2_18_2_39_15_105,(uint8*)"");
#endif
        MV_SetPdpActiveStatus(ENetworkStatus_OK);
        
	  if((NetWorkCallbackproc.pNetworkCallbackProc!=NULL)&&(NetWorkCallbackproc.pNetworkparam!=0))
      {
		   NetWorkCallbackproc.pNetworkCallbackProc((_HANDLE*)NetHandle,(_VOID*)NetWorkCallbackproc.pNetworkparam,ENetworkStatus_OK,0);
	  }
    }
    else
    {
         MV_SetPdpActiveStatus(ENetworkStatus_Error);
         if((NetWorkCallbackproc.pNetworkCallbackProc!=NULL)&&(NetWorkCallbackproc.pNetworkparam!=0))
         {
	   	    NetWorkCallbackproc.pNetworkCallbackProc((_HANDLE*)NetHandle,(_VOID*)NetWorkCallbackproc.pNetworkparam,ENetworkStatus_Error,0);
         }
    }

}

/*****************************************************************************/
//  Description : HandleMVPDPMsg
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void HandleMVPDPMsg(MMIPDP_CNF_INFO_T *msg_ptr)
{
    if (PNULL == msg_ptr)
    {
#ifdef MV_NET_DEBUG
        //MV_TRACE_LOW:"[MV]--WD[HandleMVPDPMsg]MV PDP Messsage msg_ptr=PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_NET_API_404_112_2_18_2_39_15_106,(uint8*)"");
#endif
        return;
    }
#ifdef MV_NET_DEBUG
    //MV_TRACE_LOW:"[MV]--WD[HandleMVPDPMsg]MV PDP Message msg_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_NET_API_409_112_2_18_2_39_15_107,(uint8*)"d",msg_ptr->msg_id);
#endif
    switch(msg_ptr->msg_id) 
    {
	case MMIPDP_ACTIVE_CNF:
#ifdef MV_NET_DEBUG	    
	    //MV_TRACE_LOW:"[MV]--WD[HandleMVPDPMsg]MV PDP Messsage MMIPDP_ACTIVE_CNF"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_NET_API_415_112_2_18_2_39_15_108,(uint8*)"");
#endif	        
	    HandlePdpActiveConfirm(msg_ptr);
	    break;
	    
	case MMIPDP_DEACTIVE_CNF:
	    MV_SetPdpActiveStatus(ENetworkStatus_Stop_OK);
#ifdef MV_NET_DEBUG	 
	     //MV_TRACE_LOW:"[MV]--WD[MV_ActivePdpContext]MV Deactive Pdp Context "
	     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_NET_API_423_112_2_18_2_39_15_109,(uint8*)"");
#endif	
         s_mv_pdp_net_id = 0;

	     if((NetWorkCallbackproc.pNetworkCallbackProc!=NULL)&&(NetWorkCallbackproc.pNetworkparam!=0))
         {
		    NetWorkCallbackproc.pNetworkCallbackProc((_HANDLE*)NetHandle, (_VOID*)NetWorkCallbackproc.pNetworkparam,ENetworkStatus_Stop_OK,0);
	     }
	    break;
	    
	case MMIPDP_DEACTIVE_IND:
	    MV_SetPdpActiveStatus(ENetworkStatus_Stop_OK);
#ifdef MV_NET_DEBUG	 	   
	     //MV_TRACE_LOW:"[MV]--WD[MV_ActivePdpContext]MV deactive Pdp Context fail"
	     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_NET_API_436_112_2_18_2_39_15_110,(uint8*)"");
#endif
         s_mv_pdp_net_id = 0;
	     if((NetWorkCallbackproc.pNetworkCallbackProc!=NULL)&&(NetWorkCallbackproc.pNetworkparam!=0))
         {
		    NetWorkCallbackproc.pNetworkCallbackProc((_HANDLE*)NetHandle, (_VOID*)NetWorkCallbackproc.pNetworkparam,ENetworkStatus_Stop_OK,0);
	      }
	    break;
	    
	default:
	    break;
    }

    //SCI_FREE(msg_ptr);
}

/*****************************************************************************/
//  Description : MV_ActivePdpContext
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MV_ActivePdpContext(void)
{ 
    MN_DUAL_SYS_E dual_sys = MMIAPISET_GetActiveSim();
    char mv_apn[MV_APN_MAX_LEN] = {0};
    MMIPDP_ACTIVE_INFO_T app_info = {0};
    
    if(RMEAPP_IAP_CMWAP == MV_GetPdpConnectType())
    {
        SCI_MEMCPY(mv_apn,MV_APN_CMWAP_INFO,sizeof(MV_APN_CMWAP_INFO));
    }
    else
    {
        SCI_MEMCPY(mv_apn,MV_APN_CMNET_INFO,sizeof(MV_APN_CMNET_INFO));
    }   

#ifdef MV_NET_DEBUG
    //MV_TRACE_LOW:"[MV]--WD[MV_ActivePdpContext]MV Active Pdp Context"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_NET_API_472_112_2_18_2_39_15_111,(uint8*)"");
#endif     
    app_info.app_handler = MMI_MODULE_MV;
    app_info.apn_ptr = mv_apn;
    app_info.user_name_ptr = PNULL;
    app_info.psw_ptr = PNULL;
    app_info.dual_sys = dual_sys;
    app_info.priority = 1;
    app_info.ps_service_type = BROWSER_E;
    app_info.handle_msg_callback = HandleMVPDPMsg;	
    app_info.storage = MN_GPRS_STORAGE_ALL;

    if(!MMIAPIPDP_Active(&app_info))
    {
#ifdef MV_NET_DEBUG
        //MV_TRACE_LOW:"[MV]--WD[MV_ActivePdpContext]MV Active Pdp Context fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_NET_API_488_112_2_18_2_39_15_112,(uint8*)"");
#endif   
         MV_SetPdpActiveStatus(ENetworkStatus_Stop_OK);
         if((NetWorkCallbackproc.pNetworkCallbackProc!=NULL)&&(NetWorkCallbackproc.pNetworkparam!=0))
         {
	   	    NetWorkCallbackproc.pNetworkCallbackProc((_HANDLE*)NetHandle, (_VOID*)NetWorkCallbackproc.pNetworkparam,ENetworkStatus_Error,0);
         }
    }
    
}
 
/*****************************************************************************/
//  Description : MV_DeactivePdpContext
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MV_DeactivePdpContext(void)
{
    if (!MMIAPIPDP_Deactive(MMI_MODULE_MV))
    {
#ifdef MV_NET_DEBUG
        //MV_TRACE_LOW:"[MV]--WD[MV_DeactivePdpContext] Deactive Pdp fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_NET_API_508_112_2_18_2_39_15_113,(uint8*)"");
#endif
    }
    
}

/*****************************************************************************/
//  Description : CM_Network_Init
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_HANDLE CM_Network_Init(NetworkCallbackProc pCallbackProc, _VOID* pNetworkparam)
{
    NetWorkCallbackproc.pNetworkCallbackProc = pCallbackProc;
	NetWorkCallbackproc.pNetworkparam = (int)pNetworkparam;
#ifdef MV_NET_DEBUG
	//MV_TRACE_LOW:"[MV]--WD[CM_Network_Init] network initalize ok"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_NET_API_523_112_2_18_2_39_15_114,(uint8*)"");
#endif
    MV_SetPdpActiveStatus(ENetworkStatusCount);
	return (_HANDLE)NetHandle;
}


/*****************************************************************************/
//  Description : CM_Network_Release
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_BOOLEAN CM_Network_Release(_HANDLE hNetwork)
{
#ifdef MV_NET_DEBUG
	//MV_TRACE_LOW:"[MV]--WD[CM_Network_Release] network release ok"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_NET_API_537_112_2_18_2_39_15_115,(uint8*)"");
#endif	
	return _TRUE;
}

/*****************************************************************************/
//  Description : CM_Network_Start
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_BOOLEAN CM_Network_Start(_HANDLE hNetwork, TRMEAPP_IAP nIAP)
{     
#ifdef MV_NET_DEBUG
    //MV_TRACE_LOW:"[MV]--WD[CM_Network_Start]mobile video network start hNetwork = %d,nIAP=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_NET_API_549_112_2_18_2_39_15_116,(uint8*)"dd",hNetwork,nIAP);
#endif
    if(ENetworkStatus_Stop_OK == CM_GetNetworkStatus(hNetwork)
        ||ENetworkStatusCount == CM_GetNetworkStatus(hNetwork))
    {   
        MV_SetPdpConnectType(nIAP);
        MV_SetPdpActiveStatus(ENetworkStatus_Starting);
        CM_SendSignalToMMI(MVIDEO_PDP_ACTIVE_REQ,0);
        if((NetWorkCallbackproc.pNetworkCallbackProc!=NULL)&&(NetWorkCallbackproc.pNetworkparam!=0))
        {
        	NetWorkCallbackproc.pNetworkCallbackProc((_HANDLE*)hNetwork,(_VOID*)NetWorkCallbackproc.pNetworkparam,ENetworkStatus_Starting,0);
        }
        
        return _TRUE;
         //未连接状态下，激活PDP进行连接
    }
    else
    {
        return _FALSE;
    }
    

}

/*****************************************************************************/
//  Description : CM_Network_Stop
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_BOOLEAN CM_Network_Stop(_HANDLE hNetwork)
{
#ifdef MV_NET_DEBUG
    //MV_TRACE_LOW:"[MV]--WD[CM_Network_Stop]mobile video network stop"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_NET_API_580_112_2_18_2_39_15_117,(uint8*)"");
#endif
    CM_SendSignalToMMI(MVIDEO_PDP_DEACTIVE_REQ,0);    
	MV_SetPdpActiveStatus(ENetworkStatus_Stopping);
	return _TRUE;
}

/*****************************************************************************/
//  Description : CM_GetNetworkStatus
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_INT CM_GetNetworkStatus(_HANDLE hNetwork)
{
#ifdef MV_NET_DEBUG
    //MV_TRACE_LOW:"[MV]--WD[CM_GetNetworkStatus] get network status = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_NET_API_594_112_2_18_2_39_15_118,(uint8*)"d",s_mv_pdp_status);
#endif
    return s_mv_pdp_status;

}

/*****************************************************************************/
//  Description : mv_socket_task
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MV_socket_task(uint32 argc, void* argv)
 {
    xSignalHeaderRec* sig_ptr = 0;
    uint32            ticktime = 0; 

    while(1)/*lint !e716*/
    {
	 sig_ptr = SCI_GetSignal(SCI_IdentifyThread());
#ifdef MV_NET_DEBUG
     //MV_TRACE_LOW:"[MV]--WD[mv_socket_task]: begin - %d "
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_NET_API_613_112_2_18_2_39_15_119,(uint8*)"d",sig_ptr->SignalCode);
#endif
	 switch(sig_ptr->SignalCode)
     {
	 case SOCKET_CONNECT_EVENT_IND:
            //tcp connect successfully
#ifdef MV_NET_DEBUG
        ticktime = SCI_GetTickCount();
        //MV_TRACE_LOW:"[MV]--WD[mv_socket_task] connect event flag,time=%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_NET_API_621_112_2_18_2_39_15_120,(uint8*)"d",ticktime);
#endif
	     if((SocketCallbackproc.CM_SocketCallback!=NULL)&&(SocketCallbackproc.pSocketParam!=0)
	     && (((SOCKET_CONNECT_EVENT_IND_SIG_T*) (sig_ptr))->error_code == 0))
	     {
			SocketCallbackproc.CM_SocketCallback((void*)(((SOCKET_CONNECT_EVENT_IND_SIG_T*) (sig_ptr))->socket_id),Msg_Socket_Connect,0,0,(void *)(SocketCallbackproc.pSocketParam));
			
	     }
         else
         {
	     		//tcp connet fail
#ifdef MV_NET_DEBUG
	     	//MV_TRACE_LOW:"[MV]--WD[mv_socket_task] connect failed"
	     	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_NET_API_633_112_2_18_2_39_15_121,(uint8*)"");
#endif
	     } 
	     break;

	 case SOCKET_CONNECTION_CLOSE_EVENT_IND:
#ifdef MV_NET_DEBUG
         ticktime = SCI_GetTickCount();
         //MV_TRACE_LOW:"[MV]--WD[mv_socket_task] close event flag, time=%x"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_NET_API_641_112_2_18_2_39_15_122,(uint8*)"d",ticktime);
#endif
	     if((SocketCallbackproc.CM_SocketCallback!=NULL)&&(SocketCallbackproc.pSocketParam!=0))
	     {
			SocketCallbackproc.CM_SocketCallback((void*)(((SOCKET_READ_EVENT_IND_SIG_T*) 
			(sig_ptr))->socket_id),Msg_Socket_Close,0,0,(void*)(SocketCallbackproc.pSocketParam));
			
	     }	 
	     break;

     case SOCKET_READ_EVENT_IND:
           //there are data in socket  
#ifdef MV_NET_DEBUG
        ticktime = SCI_GetTickCount();
        //MV_TRACE_LOW:"[MV]--WD[mv_socket_task] read event flag, time=%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_NET_API_655_112_2_18_2_39_15_123,(uint8*)"d",ticktime);
#endif
	     if((SocketCallbackproc.CM_SocketCallback!=NULL)&&(SocketCallbackproc.pSocketParam!=0))
	     {
			SocketCallbackproc.CM_SocketCallback((void*)(((SOCKET_READ_EVENT_IND_SIG_T*) (sig_ptr))->socket_id),Msg_Socket_OnReceive,0,0,(void*)(SocketCallbackproc.pSocketParam));
			
	     }	 
         break;

	 case SOCKET_WRITE_EVENT_IND:
#ifdef MV_NET_DEBUG
          ticktime = SCI_GetTickCount();
	      //MV_TRACE_LOW:"[MV]--WD[mv_socket_task] write event flag"
	      SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_NET_API_667_112_2_18_2_39_15_124,(uint8*)"",ticktime);
#endif
         if((SocketCallbackproc.CM_SocketCallback!=NULL)&&(SocketCallbackproc.pSocketParam!=0))
	     {
			SocketCallbackproc.CM_SocketCallback((void*)(((SOCKET_WRITE_EVENT_IND_SIG_T*) (sig_ptr))->socket_id),Msg_Socket_OnSend,0,0,(void*)SocketCallbackproc.pSocketParam);
			
	     }
         break;

	 default:
#ifdef MV_NET_DEBUG
	     //MV_TRACE_LOW:"[MV]--WD[mv_socket_task]: unresolved EVENT - %d "
	     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_NET_API_678_112_2_18_2_39_15_125,(uint8*)"d",sig_ptr->SignalCode);
#endif
	     break;
	 }

	 SCI_FREE(sig_ptr);
	 
    }
    //SCI_ThreadExit();
    
}



