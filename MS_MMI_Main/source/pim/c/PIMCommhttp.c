/******************************************************************************
                                   ����WCDMA  ����ҵ��ģ��
        ����3G  ������𽥳��죬������ҵ�������Ϊ������
        �ƶ��ն˵����㣬�𽥳�Ϊ���ɻ�ȱ��һ���֡���
        ģ����ƻ���OMA  ��������Ļ���syncml Э������ݺ���
        �˹���ʵ���ڶ�ƽ̨�������ϵͳ�е�PIMҵ��Ŀ��
        ��Ҫ�ﵽ����ƽ̨�Ŀ���ֲ�����õĹ����ȶ���ģ�顣
        ��ģ���ʵ�ַ�Ϊ��������:
        1.ʵ��Data sync����
        2.ʵ��Device Management����
        3.ʵ�ֶ�ƽ̨��ƽ����ֲ
        ģ�鿪���ο�OMA��Ա��ͬ������SRT_c��
        Ŀǰģ��ƻ�����֧��OMA syncML Э��1.1


        ��Ȩ����:�������Ȩ���ں���ͨ�ţ�����һ�з���Ȩ��
        ������Ա:����
        ��������:2005��4��11�ա�
******************************************************************************/


/******************************************************************************
�ļ�����:PIMCommhttp.c
�ļ�����:HTTPͨѶ��ʽ��֧��
�ļ�˵��:�ṩHTTPͨѶ��ʽ�İ󶨣�ʵ����HTTP�µ�ͨѶ��
������Ա:����
�״ο�������:2005-7-9
******************************************************************************/


/******************************************************************************
                                                            �޸ļ�¼
  ����                   ����                �޸�ԭ��
  -----------------------------------------------------------------------------
  2006-6-8              ����                ��һ�ο������롣
  -----------------------------------------------------------------------------
  *****************************************************************************/
  /*  ����ͷ�ļ�*/
#include "mmi_pim_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
  #define _PIMCOMMHTTP_C_

  #include "../h/PIMCommhttp.h"
  #include "../h/PIMSyncmlComm.h"
  #include "../h/PIMsmlerr.h"
  #include "../h/PIMcallback_handler.h"
  #include "../h/PIMdatasynctask.h"
 
  #include "mmi_appmsg.h"

 #include  "os_api.h"
#ifdef WIN32
  #include "winsock.h"
#else
 #include  "app_tcp_if.h"
 #include "mmipdp_export.h"
#endif


#ifdef HS_FEATURE_COMM_HTTP_SUPPORT

  /*  ������*/
#define HS_HTTP_VERSION     "HTTP/1.1"
#define HS_HTTP_MIME_TYPES  "application/vnd.syncml+xml, application/vnd.syncml+wbxml"
#define HS_HTTP_CACHECONTROL "private"

#define HS_HTTP_WBXML_MIME_TYPES  "application/vnd.syncml+wbxml"
#define HS_HTTP_XML_MIME_TYPES  "application/vnd.syncml+xml"

#define HS_HTTP_LANGUAGE    "en"
#define HS_HTTP_CODEC   "UTF-8"

//#ifdef HS_PIM_UA_SETTING  //modify by wangxiaolin 2009.12.21
//#define CLIENT_NAME_UA_INFO_HS     "HS-N51/1.0 Release/10.25.2009 Browser/NF3.5 Profile/MIDP-2.0 Config/CLDC-1.1"
//#endif
//
//#define CLIENT_NAME_UA_INFO    "HS-N51/1.0 Release/10.25.2009 Browser/NF3.5 Profile/MIDP-2.0 Config/CLDC-1.1"
// 
#define HS_HEADER_BUFFER_LEN 600 // chenxiang 20081209 500-->600 ����headbuffer ��С��֧�ָ�����UA

  /*  ȫ�ֱ�������*/
  extern boolean hs_already_receive; // chenxiang_20080326

  
  extern syncml_Comm_type* hs_global_Comm_Ptr;
  extern task_relay_info_type*     hs_task_relay_info;
  http_globle_indicator_type* hs_http_globle=NULL;
  static boolean hs_bFirstConnection = TRUE;
  static uint32 hs_totalsend=0;
  static int hs_totalrec=0;                  //һ�ν��յ����ݴ�С
  static int hs_contentlength=0;         //http header�д�ŵı�ʾ�������ݵĴ�С��
  //��ʱ����أ��������ڲ鿴�Ƿ���Է��ͺͽ�������
 static int hs_socket_select_counts=0;
static boolean hs_is_socket_read=FALSE;
static char hs_SocketSelectTimerName[] = "PIMSockSelectTmr"; 

extern uint32 hs_resend_count; // chenxiang_20080326

#define TIMER_SELECT  300
  /*  ���ܺ���*/
  //#define HS_TCPINITIALIZED() (!hs_bFirstConnection)
  //#define TCPINITDONE() hs_bFirstConnection=FALSE
  //#define HS_TCPWILLINITAGAIN() hs_bFirstConnection=TRUE
extern void MMIAPICOM_TriggerMMITask (void);
  
#ifndef WIN32
 /******************************************************************* 
  ** ������:hs_GetserverAddrandPort
  ** ������������host�ַ����еõ�server��ַ�Ͷ˿ںš�
  ** ��  ��: 
  ** ��  ��:����
  ** ��  ��:2005-9-7
  ** �汾: ver 1.0
  *******************************************************************/
  static boolean hs_GetserverAddrandPort(const char* server_name, const char* server_port, uint32* serveraddr, uint16* serverport);
#endif

  /******************************************************************* 
  ** ������:hs_writeRequestHeader
  ** ����������write http request header.
  ** ��  ��:  
  ** ��  ��:����
  ** ��  ��:2005-9-7
  ** �汾: ver 1.0
  *******************************************************************/
static void hs_writeRequestHeader (void);

  /******************************************************************* 
  ** ������:hs_writeGeneralRequestHeader
  ** ����������write http general request header.
  ** ��  ��:  
  ** ��  ��:����
  ** ��  ��:2005-9-7
  ** �汾: ver 1.0
  *******************************************************************/
static   void hs_writeGeneralRequestHeader (char* pszURI, char *pszHeader);

  /******************************************************************* 
  ** ������:hs_writeContentRequestHeader
  ** ����������write http content request header.
  ** ��  ��:  
  ** ��  ��:����
  ** ��  ��:2005-9-7
  ** �汾: ver 1.0
  *******************************************************************/
  static void hs_writeContentRequestHeader (char *pszHeader);

  /******************************************************************* 
  ** ������:hs_ReadRequestHeader
  ** ����������read http request header, look if the http response is 200 ok
                              if not, return FALSE,else give the hs_contentlength to clength.
  ** ��  ��:  
  ** ��  ��:����
  ** ��  ��:2005-9-7
  ** �汾: ver 1.0
  *******************************************************************/
static int hs_ReadRequestHeader (int * clength,int* headersize); // chenxiang_20080326

  /******************************************************************* 
  ** ������:hs_AnalysisHeader
  ** ���������� look if the http response is 200 ok
                              if not, return FALSE,else give the hs_contentlength to clength.
                              if it is ok, get the content's length.
  ** ��  ��:  
  ** ��  ��:����
  ** ��  ��:2005-9-7
  ** �汾: ver 1.0
  *******************************************************************/
static int hs_AnalysisHeader (char* buffer, int  headersize, int* clength); // chenxiang_20080326

static int hs_pim_select_connect_socket(int ErrCode);  
static void HS_PIM_deactive_socket_select_timer(void);

/******************************************************************************
 * ����˵����	��ʱ���ص�����

 * ����˵����	
 
 * ����ֵ��		
 
* ����:������
********************************************************************************/
static void PIM_socket_select_timer_handler(uint32 state)
{
     HS_PIM_deactive_socket_select_timer();
     HS_Data_Sync_PostMsgToUI( PIM_MSG_SELECT_TIMER, NULL,NULL);
     MMIAPICOM_TriggerMMITask();

}

static void HS_PIM_active_socket_select_timer(void)
{
    PIM_T_P  pMe=MMIPIM_GetPIM();
    if(pMe->m_psocket_select_timer)
    {
        SCI_ChangeTimer(pMe->m_psocket_select_timer,PIM_socket_select_timer_handler,TIMER_SELECT);
        SCI_ActiveTimer(pMe->m_psocket_select_timer);
    }
}

static void HS_PIM_deactive_socket_select_timer(void)
{
    PIM_T_P  pMe=MMIPIM_GetPIM();
    if(pMe->m_psocket_select_timer)
        SCI_DeactiveTimer(pMe->m_psocket_select_timer);
}

static void HS_PIM_delete_socket_select_timer(void)
{
    PIM_T_P  pMe=MMIPIM_GetPIM();
    if(pMe->m_psocket_select_timer)
    {
        SCI_DeactiveTimer(pMe->m_psocket_select_timer);
        SCI_DeleteTimer(pMe->m_psocket_select_timer);
    }
}




PUBLIC int HS_PIM_socket_select_handle(void)
{
#ifndef WIN32
   int errorCode;
   sci_fd_set read_fdset;
   sci_fd_set write_fdset;
   sci_fd_set exp_fdset;
   long time_term = 0;

 
   int pim_socket;
   PIM_T_P  pMe=MMIPIM_GetPIM();
   
   //����˿��ܻ������Ͽ����ӣ�ƽ̨���ϱ�MMIPDP_DEACTIVE_IND
   if(pMe->gprs_state==PIM_GPRS_DEACTIVE_OK)//by wangxiaolin 2010.1.11
   {
     //syncml_http_message:"MMIPIM ** pdp has been deactived!"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_234_112_2_18_3_25_38_31,(uint8*)"");
     HS_Data_Sync_PostMsgToUI( PIM_MSG_COMM_ERROR, NULL,NULL);
     return HS_SML_ERR_A_COMM_ERROR;
   }

   pim_socket=pMe->m_pISocket;

    //��ѯʱ��Ϊ200*300���룬Ҳ����60��Ϊ��ʱ   
   if(hs_socket_select_counts<200)
   {
       /* test connection status */
    SCI_FD_ZERO(&read_fdset);
    SCI_FD_ZERO(&write_fdset);
    SCI_FD_ZERO(&exp_fdset);	
     
    if(hs_is_socket_read)
    {
      //syncml_http_message:"MMIPIM *********read socket select ..... "
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_251_112_2_18_3_25_38_32,(uint8*)"");
       SCI_FD_SET(pim_socket, &read_fdset);
       errorCode = sci_sock_select(&read_fdset, &write_fdset, &exp_fdset, time_term);

        //SOCKET ����
        if ( (errorCode >0) && (0<SCI_FD_ISSET(pim_socket, &read_fdset)))
        {    short  result=0;
              hs_socket_select_counts=0;
              result=HS_HTTP_RecData();
              return result;
        }
         else if (errorCode < 0)
         {
               hs_socket_select_counts=0;
               //syncml_http_message:"MMIPIM **receive data error: Error %d"
               SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_265_112_2_18_3_25_38_33,(uint8*)"d", errorCode);
               HS_Data_Sync_PostMsgToUI( PIM_MSG_COMM_ERROR, NULL,NULL);
		 return errorCode;
         }
         else
         {
             hs_socket_select_counts++;
             hs_is_socket_read=TRUE;
             HS_PIM_active_socket_select_timer(); 
             return 0;
         }
     }
     else
     {
           //syncml_http_message:"MMIPIM *********write socket select ..... "
           SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_279_112_2_18_3_25_38_34,(uint8*)"");
           SCI_FD_SET(pim_socket, &write_fdset);
           errorCode = sci_sock_select(&read_fdset, &write_fdset, &exp_fdset, time_term);
           //syncml_http_message(("MMIPIM errorCode=%d ",errorCode));
        //SOCKET ����
        if ( (errorCode >0) && (0<SCI_FD_ISSET(pim_socket, &write_fdset)))
        {    
              hs_socket_select_counts=0;
              if (hs_bFirstConnection)
              {
                  return hs_pim_select_connect_socket(errorCode);                  
              }
              else
              {
                   short result=0;
                   result=HS_HTTP_SendData();
                   return result;
              }
        }
         else if (errorCode < 0)
         {
               hs_socket_select_counts=0;
               //syncml_http_message:"MMIPIM ** send data error: Error %d"
               SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_301_112_2_18_3_25_38_35,(uint8*)"d", errorCode);
               HS_Data_Sync_PostMsgToUI( PIM_MSG_COMM_ERROR, NULL,NULL);
		 return errorCode;
         }
         else
         {
             hs_socket_select_counts++;
              hs_is_socket_read=FALSE;
             HS_PIM_active_socket_select_timer();     
             return 0;
         }
     }
   }
   else
   {
         hs_socket_select_counts=0;
         //syncml_http_message:"MMIPIM ** communication ,no socket is avilable"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_317_112_2_18_3_25_38_36,(uint8*)"");
         return -1;
   }  
 #else //FEATURE WIN32
   return 0;/*lint !e527*/
 #endif //FEATURE WIN32
}


/******************************************************************************
 * �������壺	pim_socket

 * ����˵����	����socket

 * ����˵����	af:	socket��ַ��ʽ��Ŀǰ��֧��AF_INET
				type: ����ָ��socketΪ���ݱ�������ʽsocket
				protocol: ָ��socket���õ�Э�飬IPPROTO_TCP��TCPЭ�飬IPPROTO_UDP��UDPЭ��

 * ����ֵ��		���óɹ����򷵻�Ϊһ���µ�socket������
				���򷵻�NULL
* ����:������
********************************************************************************/
static int hs_pim_socket_create(
   int af,      /* Protocol family */
   int type,      /* Protocol type */
   int protocol,      /* Which protocol */
   uint32 netid     /* net interface ID */
)
{
    //syncml_http_message:"mmipim create pim_socket"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_345_112_2_18_3_25_38_37,(uint8*)"");
 
#ifdef WIN32
	
   return socket( af, type, protocol );

#else
{

      int sock_index = 0;
      int err_opt;
      int arg_block = 1;      

      /* cr152116, sci_sock_socket should add net_id as the 4th parameter
       * to work compatable with multi-net interface, here just set with 
       * default value 0!                 NOTE : Use default value 0 may 
       * cause socket send error when more than one net interface in-use, 
       * the right way is to get netid from GPRS after PDP actived and 
       * set it when create socket! */
      /* create socket with net interface ID, cr166376, @yifeng.wang */
      sock_index = sci_sock_socket(AF_INET, SOCK_STREAM, 0, netid);
      
      if(sock_index == (-1))
      {
           //Socket open error
           return -1;
      }
      
      /*���˿���Ϊ�Ƕ�������*/
      err_opt = sci_sock_setsockopt((long) sock_index, SO_NONBLOCK, &arg_block);
      
      //���÷Ƕ�����ʽʧ�ܣ��ر�socket
      if(err_opt == (-1))
      {
            sci_sock_socketclose(sock_index);
            return -1;
      }
      
      return (int)sock_index;
 
}

#endif

} 

/* ======================================================================
DESCRIPTION:
  Closes socket

INPUT/OUPUT PARAMETERS:
  s - socket to close

RETURN VALUE:
  Returns 0 on success

AUTHER:
 wangxiaolin
========================================================================== */
static int hs_pim_socket_close(
   int s      /* Socket to close */
)
{
    //syncml_http_message:"mmipim socket close socket=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_407_112_2_18_3_25_38_38,(uint8*)"d",s);
    
#ifdef WIN32
   return 0;
    //return closesocket( s );

#else

    return sci_sock_socketclose( s );

#endif 

} 


/* ======================================================================
DESCRIPTION:
   Creates a connection to a specified destination

INPUT/OUPUT PARAMETERS:
  s - socket to use
  peername - where you want to connect
  peernamelen - size of the address structure

RETURN VALUE:
  Returns 0 on success

AUTHER:
 wangxiaolin
========================================================================== */
static int hs_pim_socket_connect(
    int              s,    /* Socket to use    */
    struct sockaddr *peername,    /* Where you want to connect    */
    int              peernamelen    /* Size of the address structure    */
)
{
 
    //syncml_http_message:"mmipim socket_connect socket =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_443_112_2_18_3_25_39_39,(uint8*)"d",s);
    
 #ifdef WIN32
 
 return connect(s, peername, peernamelen);
    
#else
    {
	uint8 *paddr;
      sci_sa sock_addr;  
      memset(&sock_addr, 0, sizeof(sci_sa));

    if(NULL != peername)
    {
	    memcpy(&sock_addr, peername, sizeof(sci_sa));
    }
	//sock_addr.port = htons(sock_addr.port);
	//sock_addr.ip_addr = htonl(sock_addr.ip_addr);
    paddr = (uint8*)&sock_addr.ip_addr;
    sock_addr.sa_data[0] = paddr[0];
    sock_addr.sa_data[1] = paddr[1];
    sock_addr.sa_data[2] = paddr[2];
    sock_addr.sa_data[3] = paddr[3];
    sock_addr.sa_data[4] = 0;
	
    return  sci_sock_connect((long)s, &sock_addr, sizeof(sci_sa));
	
    }

#endif



}
/* ======================================================================
DESCRIPTION:
  Receive data on the specified TCP socket

INPUT/OUPUT PARAMETERS:
  s - socket
  buf - pointer to destination buffer
  len - number of bytes to receive
  flags - associated flags

RETURN VALUE:
  Number of bytes read

AUTHER:
 wangxiaolin
========================================================================== */
static int hs_pim_socket_recv(
   int   s,      /* Socket */
   void *buf,      /* Pointer to destination buffer */
   int   len,      /* Number of bytes to receive */
   int   flags      /* Associated flags */
)
{
    //syncml_http_message:"mmipim socket_recv socket =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_499_112_2_18_3_25_39_40,(uint8*)"d",s);

    if(NULL == buf)
	return -1;
	
#ifdef WIN32
    return recv( s, buf, len, flags );
#else

    return sci_sock_recv(s, buf, len, flags);
    
#endif /* NEVER */

}

 
  /******************************************************************* 
  ** ������:HS_HTTP_Prepare
  ** ������������ʼ��HTTP ������ݣ������������ַ  ���˿�,����PDP��
  ** ��  ��: ���ز���״̬
  ** ��  ��:wangxiaolin 
  ** ��  ��:2007-10-23
  ** �汾: ver 1.0
  *******************************************************************/
  short  HS_HTTP_Prepare()
  {
       int retVal = 0;
       PIM_T_P  pMe=MMIPIM_GetPIM();
       //syncml_http_message:"MMIPIM HS_HTTP_Prepare "
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_527_112_2_18_3_25_39_41,(uint8*)"");
  	hs_http_globle=(http_globle_indicator_type*)hs_smlLibMalloc(__FILE__, __LINE__, (long)sizeof(http_globle_indicator_type));
       if(NULL==hs_http_globle)
       {
             	return HS_SML_ERR_NOT_ENOUGH_SPACE;
       }
	hs_smlLibMemset(hs_http_globle,0,sizeof(http_globle_indicator_type));
	
	hs_http_globle->useagent=hs_task_relay_info->use_proxy;
       if(1==hs_http_globle->useagent)  //ʹ�ô��������
       {
            hs_http_globle->agent=(char*)hs_smlLibMalloc(__FILE__, __LINE__, MMIPIM_MAX_IP_LEN+1);
            if(NULL==hs_http_globle->agent)
            {
              	return HS_SML_ERR_NOT_ENOUGH_SPACE;
            }
            hs_smlLibMemset(hs_http_globle->agent,0,MMIPIM_MAX_IP_LEN+1);
            xppStrcpy(hs_http_globle->agent,hs_task_relay_info->proxy_ip);
            

            hs_http_globle->agentport=(char*)hs_smlLibMalloc(__FILE__, __LINE__, MMIPIM_MAX_PORT_LEN+1);
            if(NULL==hs_http_globle->agentport)
            {
              	return HS_SML_ERR_NOT_ENOUGH_SPACE;
            }
            
            hs_smlLibMemset(hs_http_globle->agentport,0,MMIPIM_MAX_PORT_LEN+1);
            xppStrcpy(hs_http_globle->agentport,hs_task_relay_info->proxy_port);

            hs_http_globle->ausername=(char*)hs_smlLibMalloc(__FILE__, __LINE__,  MMIPIM_MAX_USER_LEN + 1);
            if(NULL==hs_http_globle->ausername)
            {
                   return HS_SML_ERR_NOT_ENOUGH_SPACE;
            }
            hs_smlLibMemset(hs_http_globle->ausername,0, MMIPIM_MAX_USER_LEN + 1);
            xppStrcpy(hs_http_globle->ausername,hs_task_relay_info->proxy_username);

            hs_http_globle->apassword=(char*)hs_smlLibMalloc(__FILE__, __LINE__, MMIPIM_MAX_PASSWORD_LEN + 1);
            if(NULL==hs_http_globle->apassword)
            {
                   return HS_SML_ERR_NOT_ENOUGH_SPACE;
            }
            hs_smlLibMemset(hs_http_globle->apassword,0,MMIPIM_MAX_PASSWORD_LEN + 1);
            xppStrcpy(hs_http_globle->apassword,hs_task_relay_info->proxy_password);
      }
      else
      {   //�����ô��������
	     hs_http_globle->agent=NULL;
           hs_http_globle->agentport=NULL;
           hs_http_globle->ausername=NULL;
           hs_http_globle->apassword=NULL;
     }
	
	hs_http_globle->server=(char*)hs_smlLibMalloc(__FILE__, __LINE__, MMIPIM_MAX_URL_LEN + 1);
       if(NULL==hs_http_globle->server)
       {
              return HS_SML_ERR_NOT_ENOUGH_SPACE;
       }
	hs_smlLibMemset(hs_http_globle->server,0,MMIPIM_MAX_URL_LEN + 1);
	xppStrcpy(hs_http_globle->server,hs_task_relay_info->syncml_pim_server_name);

	hs_http_globle->serverport=(char*)hs_smlLibMalloc(__FILE__, __LINE__, MMIPIM_MAX_PORT_LEN + 1);
       if(NULL==hs_http_globle->serverport)
       {
              return HS_SML_ERR_NOT_ENOUGH_SPACE;
       }
	hs_smlLibMemset(hs_http_globle->serverport,0,MMIPIM_MAX_PORT_LEN + 1);
	xppStrcpy(hs_http_globle->serverport,hs_task_relay_info->syncml_pim_server_port);

     hs_http_globle->susername=(char*)hs_smlLibMalloc(__FILE__, __LINE__, MMIPIM_MAX_USER_LEN + 1);
       if(NULL==hs_http_globle->susername)
       {
              return HS_SML_ERR_NOT_ENOUGH_SPACE;
       }
	hs_smlLibMemset(hs_http_globle->susername,0,MMIPIM_MAX_USER_LEN + 1);
	xppStrcpy(hs_http_globle->susername,hs_task_relay_info->syncml_pim_username);

	hs_http_globle->spassword=(char*)hs_smlLibMalloc(__FILE__, __LINE__, MMIPIM_MAX_PASSWORD_LEN + 1);
       if(NULL==hs_http_globle->spassword)
       {
              return HS_SML_ERR_NOT_ENOUGH_SPACE;
       }
	hs_smlLibMemset(hs_http_globle->spassword,0,MMIPIM_MAX_PASSWORD_LEN + 1);
	xppStrcpy(hs_http_globle->spassword,hs_task_relay_info->syncml_pim_password);

        // chenxiang 20081209 ����headbuffer ��С��֧�ָ�����UA
	hs_http_globle->headerbuffer=(char*)hs_smlLibMalloc(__FILE__, __LINE__, HS_HEADER_BUFFER_LEN);
       if(NULL==hs_http_globle->headerbuffer)
       {
              return HS_SML_ERR_NOT_ENOUGH_SPACE;
       }
	hs_smlLibMemset(hs_http_globle->headerbuffer,0,HS_HEADER_BUFFER_LEN);
	hs_http_globle->usedsize=0;
	
	hs_global_Comm_Ptr->privateinfo=hs_http_globle;/*lint !e64*/

       pMe->m_psocket_select_timer=SCI_CreateTimer(hs_SocketSelectTimerName,
                                          PIM_socket_select_timer_handler,
                                          0,
                                          TIMER_SELECT,
                                          SCI_NO_ACTIVATE);
             
       //pdp active
	MMIPIM_ConnectNetwork(&retVal);
	if(retVal==1) //����ɹ�����֮ǰ�Ѿ�����
	{
 	      HS_Data_Sync_PostMsgToUI( MSG_PDP_ACTIVE_SUCCESS, NULL,NULL);
     
	}
    else
     {
            //char * dis_chr="pdp active...";
           // MMIPIM_DisplayProc(dis_chr);
     }

  	return HS_SML_ERR_OK;
  }

  /******************************************************************* 
  ** ������:HS_HTTP_Release
  ** �����������ͷ�HTTP ������ݣ������������ַ
                                  ���˿ڡ�
  ** ��  ��: ���ز���״̬
  ** ��  ��:wangxiaolin
  ** ��  ��:2007-10-25
  ** �汾: ver 1.0
  *******************************************************************/
  short  HS_HTTP_Release()
  {
 
      PIM_T_P  pMe=MMIPIM_GetPIM();
 //syncml_http_message:"MMIPIM HS_HTTP_Release "
 SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_658_112_2_18_3_25_39_42,(uint8*)"");
     if (!hs_bFirstConnection)
     {
         hs_bFirstConnection=TRUE;
     }
  	if(hs_http_globle!=NULL)
  	{
  		hs_global_Comm_Ptr->privateinfo=NULL;
           if(NULL!=hs_http_globle->agent)
           {
      		hs_smlLibFree((hs_http_globle->agent));
      		hs_http_globle->agent=NULL;
           }
 //syncml_http_message:"MMIPIM HS_HTTP_Release 1"
 SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_671_112_2_18_3_25_39_43,(uint8*)"");
           if(NULL!=hs_http_globle->agentport)
           {
      		hs_smlLibFree((hs_http_globle->agentport));
      		hs_http_globle->agentport=NULL;
           }
 //syncml_http_message:"MMIPIM HS_HTTP_Release 2"
 SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_677_112_2_18_3_25_39_44,(uint8*)"");
           if(NULL!=hs_http_globle->ausername)
           {
      		hs_smlLibFree((hs_http_globle->ausername));
      		hs_http_globle->ausername=NULL;
           }
 //syncml_http_message:"MMIPIM HS_HTTP_Release 3"
 SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_683_112_2_18_3_25_39_45,(uint8*)"");
           if(NULL!=hs_http_globle->apassword)
           {
      		hs_smlLibFree((hs_http_globle->apassword));
      		hs_http_globle->apassword=NULL;
           }

 //syncml_http_message:"MMIPIM HS_HTTP_Release 4"
 SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_690_112_2_18_3_25_39_46,(uint8*)"");
		if(NULL!=hs_http_globle->server)
		{
			hs_smlLibFree((hs_http_globle->server));
			hs_http_globle->server=NULL;
		}
 //syncml_http_message:"MMIPIM HS_HTTP_Release 5"
 SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_696_112_2_18_3_25_39_47,(uint8*)"");
		if(NULL!=hs_http_globle->serverport)
		{
			hs_smlLibFree((hs_http_globle->serverport));
			hs_http_globle->serverport=NULL;
		}
 //syncml_http_message:"MMIPIM HS_HTTP_Release 6"
 SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_702_112_2_18_3_25_39_48,(uint8*)"");
           if(NULL!=hs_http_globle->susername)
		{
			hs_smlLibFree((hs_http_globle->susername));
			hs_http_globle->susername=NULL;
		}
 //syncml_http_message:"MMIPIM HS_HTTP_Release 7"
 SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_708_112_2_18_3_25_39_49,(uint8*)"");
		if(NULL!=hs_http_globle->spassword)
		{
			hs_smlLibFree((hs_http_globle->spassword));
			hs_http_globle->spassword=NULL;
		}
 //syncml_http_message:"MMIPIM HS_HTTP_Release 8"
 SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_714_112_2_18_3_25_39_50,(uint8*)"");
      
		if(NULL!=hs_http_globle->headerbuffer)
		{
			hs_smlLibFree((hs_http_globle->headerbuffer));
			hs_http_globle->headerbuffer=NULL;
		}
 //syncml_http_message:"MMIPIM HS_HTTP_Release 9"
 SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_721_112_2_18_3_25_39_51,(uint8*)"");
		hs_smlLibFree(hs_http_globle);
		hs_http_globle=NULL;
 //syncml_http_message:"MMIPIM HS_HTTP_Release 10"
 SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_724_112_2_18_3_25_39_52,(uint8*)"");
 #ifndef WIN32
          if(pMe->m_pISocket>0)
           {  
                sci_sock_socketclose(pMe->m_pISocket);
	         pMe->m_pISocket=-1;
           }
            //pdpȥ����
            MMIPIM_DeactivePDP();
			
            pMe->gprs_state = PIM_GPRS_DEACTIVEING;
  #endif     		
            
             //only release once 
             HS_PIM_delete_socket_select_timer();
             pMe->m_psocket_select_timer=PNULL;
  	}

     //reset all the static variables.

     hs_totalsend=0;
     hs_totalrec=0;
     hs_contentlength=0;  

  	return HS_SML_ERR_OK;
  }
  
  /******************************************************************* 
  ** ������:hs_pim_select_connect_socket
  ** ������������ѯsokect�Ƿ��Ѿ����ӳɹ�
  ** ��  ��: ���ز���״̬��һֱ�ȣ������ֻ�п��úͲ���������״̬
  ** ��  ��:������
  ** ��  ��:2007-11- 1
  ** �汾: ver 1.0
  **��ע:��pim_select_send_socket�������·�װ�������������һ��
  ** ʱ�������ѯ����ѯ��������ʧ�ܣ�����Ϊ������
  *******************************************************************/  
static int hs_pim_select_connect_socket(int ErrCode)
{

    PIM_T_P  pMe=MMIPIM_GetPIM();
    if ( pMe->m_pISocket <0 )
    {
	//syncml_http_message:"MMIPIM **Socket init fail!"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_767_112_2_18_3_25_39_53,(uint8*)"");
	 return -1;
    }

   if(ErrCode >0) /*�ɹ��������ӣ�����Ӧ����*/ 
   { 

	//syncml_http_message:"MMIPIM ** Connect Successful:"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_774_112_2_18_3_25_39_54,(uint8*)"");
       hs_bFirstConnection=FALSE;
	hs_http_globle->socket_step=SOCKET_STEP_1;
	HS_Data_Sync_PostMsgToUI( PIM_MSG_CONN_EST, NULL,NULL);				
	hs_totalsend=0;    //reset send total conter.
	return 1;
   } 
   else
   { 
    //syncml_http_message:"MMIPIM ** Connect Failed: Error %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_783_112_2_18_3_25_39_55,(uint8*)"d", ErrCode);
    HS_Data_Sync_PostMsgToUI( PIM_MSG_COMM_ERROR, NULL,NULL);	
    return -1;
   }
 }

  /******************************************************************* 
  ** ������:HS_HTTP_SendData
  ** �������������ӷ�����������httpheader�������ͣ�
                                  Ȼ������Ҫ���͵����ݡ�
  ** ��  ��: ���ز���״̬
  ** ��  ��:������
  ** ��  ��:2007-10- 24
  ** �汾: ver 1.0
  *******************************************************************/
  short  HS_HTTP_SendData()
  {
     char* sendbuffer;
     uint32 sendsize;
     http_globle_indicator_type* info;
	int  nSent=0;
  	short rc = HS_SML_ERR_OK;
	int pim_socket;
	int result=0;
       PIM_T_P  pMe;
       #ifndef WIN32
	sci_sa addr;    //����ȷ��Զ�˵�ַ�Ľṹ int result;
       #endif
       //syncml_http_message:"MMIPIM **HS_HTTP_SendData"
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_811_112_2_18_3_25_39_56,(uint8*)"");
	 pMe = MMIPIM_GetPIM();
     
       //����˿��ܻ������Ͽ����ӣ�ƽ̨���ϱ�MMIPDP_DEACTIVE_IND
       if(pMe->gprs_state==PIM_GPRS_DEACTIVE_OK)//by wangxiaolin 2010.1.11
       {
         //syncml_http_message:"MMIPIM ** pdp has been deactived!"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_817_112_2_18_3_25_39_57,(uint8*)"");
         HS_Data_Sync_PostMsgToUI( PIM_MSG_COMM_ERROR, NULL,NULL);
         return HS_SML_ERR_A_COMM_ERROR;
       }
       
       pim_socket=pMe->m_pISocket;
       #ifndef WIN32
       addr.family = AF_INET; //���������ֵ 
      #endif
      sendbuffer=hs_global_Comm_Ptr->cache;
      sendsize=hs_global_Comm_Ptr->cache_length;
      info=(http_globle_indicator_type*)(hs_global_Comm_Ptr->privateinfo);

  	switch(((http_globle_indicator_type*)info)->socket_step)
  	{
  		case SOCKET_STEP_CONNECT:  //���ӵ�������
		{
                 //syncml_http_message:"MMIPIM ** HS_HTTP_SendData SOCKET_STEP_CONNECT"
                 SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_834_112_2_18_3_25_39_58,(uint8*)"");
			   if (hs_bFirstConnection)
			  {
                        //syncml_http_message:"MMIPIM **Begin open socket\n"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_837_112_2_18_3_25_39_59,(uint8*)"");

                     #ifndef WIN32
                        if(1==info->useagent)  //ʹ�ô��������
                        {
      			    	   if(FALSE== (hs_GetserverAddrandPort(info->agent,
      					   	                                 info->agentport, (uint32*)&addr.ip_addr,(uint16*)&addr.port)))
      				   {
      				   	//syncml_http_message:"MMIPIM ** error! proxy address and port convert error."
      				   	SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_845_112_2_18_3_25_39_60,(uint8*)"");
                                 HS_Data_Sync_PostMsgToUI( PIM_MSG_COMM_ERROR, NULL,NULL);
      				   	return HS_SML_ERR_A_COMM_ERROR;
      				   }
			              //syncml_http_message:"MMIPIM **=======Connect %s:%s========="
			              SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_849_112_2_18_3_25_39_61,(uint8*)"ss",info->agent,info->agentport);
                        }
                        else
                        {
                              if(FALSE== (hs_GetserverAddrandPort(info->server,
      					   	                                 info->serverport, (uint32*)&addr.ip_addr,(uint16*)&addr.port)))
      				   {
      				   	//syncml_http_message:"MMIPIM ** error! server address and port convert error."
      				   	SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_856_112_2_18_3_25_39_62,(uint8*)"");
                                 HS_Data_Sync_PostMsgToUI( PIM_MSG_COMM_ERROR, NULL,NULL);
      				   	return HS_SML_ERR_A_COMM_ERROR;
      				   }
			              //syncml_http_message:"MMIPIM **=======Connect %s:%s========="
			              SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_860_112_2_18_3_25_39_63,(uint8*)"ss",info->server,info->serverport);
                        }
                  #endif
                             /* create socket with netid, cr166376, @yifeng.wang */
                             pMe->m_pISocket = hs_pim_socket_create(AF_INET, SOCK_STREAM, 0, pMe->m_netid);
                             pim_socket=pMe->m_pISocket;
                            
#ifndef WIN32
                             result = sci_sock_connect((long)pim_socket, &addr, sizeof(sci_sa));
#endif
                            // result = hs_pim_socket_connect((long)pim_socket, &addr, sizeof(sci_sa));
                             

                             if(result == 0)/*lint !e774*/  /*�ɹ��������ӣ�����Ӧ����*/ 
				{ 

					//syncml_http_message:"MMIPIM ** Connect Successful:"
					SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_876_112_2_18_3_25_39_64,(uint8*)"");
                                   hs_bFirstConnection=FALSE;
	                            hs_http_globle->socket_step=SOCKET_STEP_1;
					HS_Data_Sync_PostMsgToUI( PIM_MSG_CONN_EST, NULL,NULL);				
				       hs_totalsend=0;    //reset send total conter.
				} 
				else 
				{ 
				      int errno ;
                 #ifndef WIN32
				      errno = sci_sock_errno((long)pim_socket); /*��ô���ֵ*/ 

                                 //syncml_http_message:"MMIPIM ** HTTP protocol error code=%d"
                                 SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_888_112_2_18_3_25_39_65,(uint8*)"d",errno);
                                 if(errno == EISCONN) 
					{ /*�Ѿ���Զ�˽������ӣ�������Ӧ����*/ 
						//syncml_http_message:"MMIPIM ** Connect has been done:"
						SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_891_112_2_18_3_25_39_66,(uint8*)"");

	                            	 hs_bFirstConnection=FALSE;
	                          		 hs_http_globle->socket_step=SOCKET_STEP_1;
						HS_Data_Sync_PostMsgToUI( PIM_MSG_CONN_EST, NULL,NULL);				
				       	hs_totalsend=0;    //reset send total conter.
					} 
				     else if(errno == EWOULDBLOCK || errno == EINPROGRESS) 
				       { /*���ڽ������ӣ���Ҫ��ʱ��ѯ����״̬*/ 
                                     //��ʾ���ڽ�������
                                      hs_is_socket_read=FALSE;
                                    HS_PIM_active_socket_select_timer();
     					} 
					else 
					{ /*�������󣬱�ʾ����ʧ��*/
					  //�ͷ����ӣ�ȥ����PDP
					  //syncml_http_message:"MMIPIM **Socket init fail!"
					  SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_907_112_2_18_3_25_39_67,(uint8*)"");
                                    HS_Data_Sync_PostMsgToUI( PIM_MSG_COMM_ERROR, NULL,NULL);	
					  return HS_SML_ERR_A_COMM_ERROR;
					}
                    #endif
				}  
							 
		          }
			   else
			   {
				//syncml_http_message:"MMIPIM ** socket already connect!"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_917_112_2_18_3_25_40_68,(uint8*)"");
                      #ifdef PIM_HTTP_20_SUPPORT  //Added by wangxiaolin   2010.1.5 for HTTP 2.0
                          hs_bFirstConnection=FALSE;
                          hs_http_globle->socket_step=SOCKET_STEP_1;
                          HS_Data_Sync_PostMsgToUI( PIM_MSG_CONN_EST, NULL,NULL);				
                          hs_totalsend=0;    //reset send total conter.
                      #else
                           HS_Data_Sync_PostMsgToUI( PIM_MSG_COMM_ERROR, NULL,NULL);
      				return HS_SML_ERR_A_COMM_ERROR;
                      #endif  /*PIM_HTTP_20_SUPPORT*/
			   }
  		   }
		break;
		case SOCKET_STEP_1:   //����http ͷ����
		{
                   //syncml_http_message:"MMIPIM **SOCKET_STEP_1"
                   SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_932_112_2_18_3_25_40_69,(uint8*)"");
			hs_writeRequestHeader();
#ifndef WIN32
			nSent = sci_sock_send((long)pim_socket, (char *)(info->headerbuffer+hs_totalsend),
				                                (int)(info->usedsize-hs_totalsend), 0);
#endif
                    //syncml_http_message:"MMIPIM ** send length=%d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_938_112_2_18_3_25_40_70,(uint8*)"d",nSent);
			if( nSent >= 0)/*lint !e774*/  //���ͳɹ� 
			{
              
              #ifdef FEATURE_PIM_DEBUG
                   //syncml_task_message:"MMIPIM  ^_^ Begin Write xml logs.\n"
                   SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_943_112_2_18_3_25_40_71,(uint8*)"");
                    //hs_myPrintxmlorwbxml("HS_HTTP_SendData",hs_task_relay_info->workspaceid);
                    hs_myhttplogs((char *)(hs_global_Comm_Ptr->cache), hs_global_Comm_Ptr->cache_length);
                    //syncml_task_message:"MMIPIM  ^_^ Write xml logs complete.\n"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_946_112_2_18_3_25_40_72,(uint8*)"");
                       //write http logs to efs files.
                        //syncml_http_message(("MMIPIM ** Begin write http logs!\n"));  
                        //hs_myhttplogs(PNULL,HTTPLOGNAME,(char *)(info->headerbuffer+hs_totalsend), nSent);
                       // syncml_http_message(("MMIPIM ** http logs write complete!\n"));
                 #endif
			      hs_totalsend+=nSent;/*lint !e737*/
			      if(hs_totalsend!=info->usedsize)
			      {
			      		//not all data send over, send those left.
			      		//HS_HTTP_SendData();
			      		//syncml_http_message:"MMIPIM ** send next packet"
			      		SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_957_112_2_18_3_25_40_73,(uint8*)"");
			             HS_Data_Sync_PostMsgToUI( PIM_MSG_DATA_SENT, NULL,NULL);
			             
					return rc;
			      	}
			      //syncml_http_message:"MMIPIM ** writing http header complete..."
			      SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_962_112_2_18_3_25_40_74,(uint8*)"");

			      /* Post an event done data transfer at this time and will be handled
			      ** based on the test case to be run.
			      */
			      info->socket_step=SOCKET_STEP_2;
			      hs_totalsend=0;  //reset send total conunter.
			      //syncml_http_message:"MMIPIM ** post data sent event"
			      SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_969_112_2_18_3_25_40_75,(uint8*)"");
			      HS_Data_Sync_PostMsgToUI( PIM_MSG_DATA_SENT, NULL,NULL);
			     
		
			      rc=HS_SML_ERR_OK;
			      return rc;
			  }
			else
			{
			    int errno1 ;
              #ifndef WIN32
			    errno1 = sci_sock_errno((long)pim_socket);

                        //syncml_http_message:"MMIPIM ** HTTP protocol error code=%d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_982_112_2_18_3_25_40_76,(uint8*)"d",errno1);
				
			    if(errno1 == EWOULDBLOCK || errno1 == EINPROGRESS) 
			     { 
			        //���ͻ���������Ҫ�Ժ��������
			          //syncml_http_message:"MMIPIM ** writing header..."
			          SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_987_112_2_18_3_25_40_77,(uint8*)"");
                              hs_is_socket_read=FALSE;
                              HS_PIM_active_socket_select_timer();
			     } 
			     else 
			     { 
			          //����ʧ�� 
			     	  //syncml_http_message:"MMIPIM ** Write Data Failed"
			     	  SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_994_112_2_18_3_25_40_78,(uint8*)"");
                             HS_Data_Sync_PostMsgToUI( PIM_MSG_COMM_ERROR, NULL,NULL);	
			         hs_bFirstConnection=TRUE;
			          rc=HS_SML_ERR_A_COMM_ERROR;
			          return rc;
			     }
                 #endif
			}
		}
		break;
		case SOCKET_STEP_2:   //������ʽsyncml����
		{
                  //syncml_http_message:"MMIPIM **HS_HTTP_SendData SOCKET_STEP_2"
                  SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1006_112_2_18_3_25_40_79,(uint8*)"");
#ifndef WIN32
			nSent = sci_sock_send((long)pim_socket, (char *)(sendbuffer+hs_totalsend),
				                                (int)(sendsize-hs_totalsend),0);
#endif
			//syncml_http_message:"MMIPIM ** send length=%d"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1011_112_2_18_3_25_40_80,(uint8*)"d",nSent);
			if( nSent >= 0)/*lint !e774*/  //���ͳɹ� 
			{
                                   
#ifdef FEATURE_PIM_DEBUG          
                       //write http logs to efs files.
                        //syncml_http_message(("MMIPIM ** Begin write syncml body data logs!\n"));
                       // hs_myhttplogs(PNULL,HTTPLOGNAME,(char *)(info->headerbuffer+hs_totalsend), nSent);
                       // syncml_http_message(("MMIPIM ** syncml body data  write complete!\n"));
#endif

			      hs_totalsend+=nSent;/*lint !e737*/
			      if(hs_totalsend!=sendsize)
			      {
			      		//not all data send over, send those left.
			      		//syncml_http_message:"MMIPIM ** send next packet"
			      		SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1026_112_2_18_3_25_40_81,(uint8*)"");

			      	      HS_Data_Sync_PostMsgToUI( PIM_MSG_DATA_SENT, NULL,NULL);

			             
			      		//HS_HTTP_SendData();
					return rc;
			      	}
			      //syncml_http_message:"MMIPIM ** writing syncml body data complete..."
			      SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1034_112_2_18_3_25_40_82,(uint8*)"");

			      /* Post an event done data transfer at this time and will be handled
			      ** based on the test case to be run.
			      */
			      info->socket_step=SOCKET_STEP_3;
			      hs_totalsend=0;  //reset send total conunter.
			      //syncml_http_message:"MMIPIM **post data sent event"
			      SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1041_112_2_18_3_25_40_83,(uint8*)"");
			      HS_Data_Sync_PostMsgToUI( PIM_MSG_DATA_SENT, NULL,NULL);
			      
		
			      rc=HS_SML_ERR_OK;
			      return rc;
			  }
			else
			{
			    int errno1 ;
#ifndef WIN32
			    errno1 = sci_sock_errno((long)pim_socket );

			    //syncml_http_message:"MMIPIM ** HTTP protocol error code=%d"
			    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1054_112_2_18_3_25_40_84,(uint8*)"d",errno1);
			    if(errno1 == EWOULDBLOCK || errno1 == EINPROGRESS) 
			     { 
			        //���ͻ���������Ҫ�Ժ��������
			        //syncml_http_message:"MMIPIM ** writing contents..."
			        SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1058_112_2_18_3_25_40_85,(uint8*)"");
                             hs_is_socket_read=FALSE;
                            HS_PIM_active_socket_select_timer();
			     } 
			     else 
			     { 
			          //����ʧ�� 
			     	  //syncml_http_message:"MMIPIM ** Write Failed: Error %d\n"
			     	  SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1065_112_2_18_3_25_40_86,(uint8*)"d");
                               //�ͷ�socket
                             HS_Data_Sync_PostMsgToUI( PIM_MSG_COMM_ERROR, NULL,NULL);	
                             hs_bFirstConnection=TRUE;
			          rc=HS_SML_ERR_A_COMM_ERROR;
			          return rc;
			     }
#endif
			}


		}break;
		case SOCKET_STEP_3:  //֪ͨsyncml taskģ�������һ�����������η���ȫ��������
		{
                    //syncml_http_message:"MMIPIM **HS_HTTP_SendData SOCKET_STEP_3"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1079_112_2_18_3_25_40_87,(uint8*)"");
			info->socket_step=SOCKET_STEP_1;  //next step is for receive data, not set this to setp_connet because the connection alread exit.
			hs_totalsend=0;  //reset send total conunter.
			//syncml_http_message:"MMIPIM **Send all data ok!"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1082_112_2_18_3_25_40_88,(uint8*)"");
            if (0 == hs_resend_count) // chenxiang_20080326
                //reset the communication's cache(xml buffer)and http cache(header buffer)
                hs_smlUnlockReadBuffer(hs_task_relay_info->workspaceid,hs_global_Comm_Ptr->cache_length);
                hs_smlLibMemset(hs_http_globle->headerbuffer,0,HS_HEADER_BUFFER_LEN); // chenxiang 20081209 ����headbuffer ��С��֧�ָ�����UA
                info->usedsize=0;

                hs_already_receive=FALSE; // chenxiang_20080326
		   HS_Data_Sync_PostMsgToUI( PIM_MSG_NEXT_STEP, NULL,NULL);

		}
		break;
	     default:
		{
                     //syncml_http_message:"MMIPIM **HS_HTTP_SendData ,step type error!"
                     SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1096_112_2_18_3_25_40_89,(uint8*)"");

                    HS_Data_Sync_PostMsgToUI( PIM_MSG_COMM_ERROR, NULL,NULL);
			rc=HS_SML_ERR_A_COMM_ERROR;
			return rc;
		}
	   break;/*lint !e527*/
  	}
  	return HS_SML_ERR_OK;
  }

  /******************************************************************* 
  ** ������:HS_HTTP_RecData
  ** �������������շ��ص�httpheader��Ȼ�����header��
                                  ָ�������ݡ�
  ** ��  ��: ���ز���״̬
  ** ��  ��:������
  ** ��  ��:2007-11-1
  ** �汾: ver 1.0
  *******************************************************************/
short  HS_HTTP_RecData(void)
{
    char* recbuffer=PNULL;
    uint32 recsize=0;
    http_globle_indicator_type* info=PNULL;
    int  nrec=0;
    int headersize=0;
    short rc = HS_SML_ERR_OK;
    int pim_socket=0;
    PIM_T_P  pMe=PNULL;
    int head_result=0; 
    pMe = MMIPIM_GetPIM();
    pim_socket=pMe->m_pISocket;
    if(pim_socket<0)
    {
        //syncml_http_message:"MMIPIM ** socket=-1"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1131_112_2_18_3_25_40_90,(uint8*)"");
        HS_Data_Sync_PostMsgToUI( PIM_MSG_COMM_ERROR, NULL,NULL);
        return HS_SML_ERR_A_COMM_ERROR;
    }
    else if(pMe->gprs_state==PIM_GPRS_DEACTIVE_OK)//by wangxiaolin 2010.1.11
    {
        //syncml_http_message:"MMIPIM ** pdp has been deactived!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1137_112_2_18_3_25_40_91,(uint8*)"");
        HS_Data_Sync_PostMsgToUI( PIM_MSG_COMM_ERROR, NULL,NULL);
        return HS_SML_ERR_A_COMM_ERROR;
    }
    recbuffer=hs_global_Comm_Ptr->cache;
    recsize=hs_global_Comm_Ptr->cache_length;
    info=(http_globle_indicator_type*)(hs_global_Comm_Ptr->privateinfo);
    switch(((http_globle_indicator_type*)info)->socket_step)
    {
        case SOCKET_STEP_1:   //����http ͷ����
        {
            //syncml_http_message:"MMIPIM ** HS_HTTP_RecData   SOCKET_STEP_1"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1148_112_2_18_3_25_40_92,(uint8*)"");
#ifndef WIN32

            //syncml_http_message:"MMIPIM chenxiang !!!!!!!hs_totalrec == %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1151_112_2_18_3_25_40_93,(uint8*)"d", hs_totalrec);
            nrec = sci_sock_recv((long)pim_socket, (info->headerbuffer+hs_totalrec), (int)(HS_HEADER_BUFFER_LEN-hs_totalrec), 0);  // chenxiang 20081209 ����headbuffer ��С��֧�ָ�����UA
#endif
            //syncml_http_message:"MMIPIM ** http contents received is %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1154_112_2_18_3_25_40_94,(uint8*)"d",nrec);
            //�ɹ�����
            if( nrec >= 0 ) /*lint !e774*/
            { /*���ճɹ�*/ 
#ifdef FEATURE_PIM_DEBUG
                //write http logs to efs files.
                //syncml_http_message:"MMIPIM ** Begin read  http head data !\n"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1160_112_2_18_3_25_40_95,(uint8*)"");
                hs_myhttplogs((char *)(info->headerbuffer+hs_totalrec), nrec);
                //syncml_http_message:"MMIPIM ** http head data  read complete!\n"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1162_112_2_18_3_25_40_96,(uint8*)"");
#endif
                hs_totalrec+=nrec;
                //first receive HS_HEADER_BUFFER_LEN bytes,this for http header is enough.
                if(nrec==0)/*lint !e774*/
                {
                    //if http response is not 200 ok,so the response will not more than HS_HEADER_BUFFER_LEN bytes.
                    //then analysis the http header.
                    //syncml_http_message:"MMIPIM ** All packets less than HS_HEADER_BUFFER_LEN bytes!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1170_112_2_18_3_25_40_97,(uint8*)"");
                    HS_Data_Sync_PostMsgToUI( PIM_MSG_COMM_ERROR, NULL,NULL);
                    return HS_SML_ERR_A_COMM_ERROR;
                }

                head_result = hs_ReadRequestHeader(&hs_contentlength,&headersize);
#if 0 /* ���� HTTP 500 ����ʱ���·��� chenxiang_20080326 */
                if ((500 == head_result) && (2>= hs_resend_count))
                {
                    PIM_T_P  pMe = MMIPIM_GetPIM();
                    //syncml_http_message:"MMIPIM ---------------(HTTP 500)---------------------"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1180_112_2_18_3_25_40_98,(uint8*)"");

                    hs_totalrec=0;  //reset receive total conunter.
                    //reset the communication's cache(xml buffer)and http cache(header buffer)
                    hs_smlUnlockWriteBuffer(hs_task_relay_info->workspaceid,hs_contentlength);
                    hs_smlLibMemset(hs_http_globle->headerbuffer,0,HS_HEADER_BUFFER_LEN); // chenxiang 20081209 ����headbuffer ��С��֧�ָ�����UA
                    info->usedsize=0;
                    //reset hs_contentlength
                    hs_contentlength=0;
#ifndef WIN32
                    sci_sock_socketclose(pMe->m_pISocket);
#endif
                    pMe->m_pISocket=-1;
                    //Socket �ÿ�
                    //syncml_http_message:"MMIPIM **socket connection closed."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1194_112_2_18_3_25_40_99,(uint8*)"");
                    hs_bFirstConnection=TRUE;
                    hs_syncml_Comm_ReSendData();
                    return HS_SML_ERR_OK;
                }
#endif

                //phrase the http header.
                if(200 != head_result) // chenxiang_20080326
                {
                    //syncml_http_message:"MMIPIM ** HTTP protocol error!return status is not 200 ok!..."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1204_112_2_18_3_25_40_100,(uint8*)"");
                    HS_Data_Sync_PostMsgToUI( PIM_MSG_COMM_ERROR, NULL,NULL);
                    return HS_SML_ERR_A_COMM_ERROR;
                }
                //syncml_http_message:"MMIPIM ** http headersize=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1208_112_2_18_3_25_40_101,(uint8*)"d",headersize);
                //syncml_http_message:"MMIPIM ** http contents size=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1209_112_2_18_3_25_40_102,(uint8*)"d",hs_contentlength);
                //copy the already received contents to content buffer
                xppMemcpy(recbuffer,(info->headerbuffer+headersize) , hs_totalrec-headersize);

                if(hs_contentlength == (hs_totalrec - headersize))
                {
                    /* Post an event done data transfer at this time and will be handled
                    ** based on the test case to be run.
                    */
                    //syncml_http_message:"MMIPIM ** reading syncml contents complete..."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1218_112_2_18_3_25_40_103,(uint8*)"");
                    info->socket_step=SOCKET_STEP_3;

                    hs_totalrec=0;  //reset rec total conunter.
                    HS_Data_Sync_PostMsgToUI( PIM_MSG_RCV_DONE, NULL,NULL);

                    rc=HS_SML_ERR_OK;
                    return rc;
                }
                      
                if(hs_totalrec < headersize) // chenxiang 20081209 ����headbuffer ��С��֧�ָ�����UA
                {
                    //syncml_http_message:"MMIPIM ** Reading http header less than HS_HEADER_BUFFER_LEN"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1230_112_2_18_3_25_40_104,(uint8*)"");
                    HS_Data_Sync_PostMsgToUI( PIM_MSG_COMM_ERROR, NULL,NULL);
                    return HS_SML_ERR_A_COMM_ERROR;
                }
                
                /* Post an event done data transfer at this time and will be handled
                ** based on the test case to be run.
                */
                info->socket_step=SOCKET_STEP_2;

                hs_totalrec=hs_totalrec-headersize;  //reset send total conunter.

                HS_Data_Sync_PostMsgToUI( PIM_MSG_RCV_DONE, NULL,NULL);

                rc=HS_SML_ERR_OK;
                return rc;
			  
            }
            else 
            {
                int errno1; 
#ifndef WIN32
                errno1 = sci_sock_errno((long)pim_socket);

                //syncml_http_message:"MMIPIM ** HTTP protocol error code=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1254_112_2_18_3_25_40_105,(uint8*)"d",errno1);
                if(errno1 == EWOULDBLOCK || errno1 == EINPROGRESS)
                { 
                    /*�Ժ���н���*/
                    //syncml_http_message:"MMIPIM ** Reading syncml body data ..."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1258_112_2_18_3_25_40_106,(uint8*)"");
                    hs_is_socket_read=TRUE;
                    HS_PIM_active_socket_select_timer();
                } 
#if 0//del by wangxiaolin  2009.10.14 //ȥ����ʱ�ط�����
                else if ((errno1 == EPIPE) && (2 >= hs_resend_count)) // chenxiang_20080326
                {
                    PIM_T_P  pMe = MMIPIM_GetPIM();
                    //syncml_http_message:"MMIPIM -------------(errno == EPIPE)---------------------"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1266_112_2_18_3_25_40_107,(uint8*)"");

                    hs_totalrec=0;  //reset receive total conunter.
                    //reset the communication's cache(xml buffer)and http cache(header buffer)
                    hs_smlUnlockWriteBuffer(hs_task_relay_info->workspaceid,hs_contentlength);
                    hs_smlLibMemset(hs_http_globle->headerbuffer,0,HS_HEADER_BUFFER_LEN); // chenxiang 20081209 ����headbuffer ��С��֧�ָ�����UA
                    info->usedsize=0;
                    //reset hs_contentlength
                    hs_contentlength=0;

                    sci_sock_socketclose(pMe->m_pISocket);

                    pMe->m_pISocket=-1;
                    //Socket �ÿ�
                    //syncml_http_message:"MMIPIM **socket connection closed."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1280_112_2_18_3_25_40_108,(uint8*)"");
                    hs_bFirstConnection=TRUE;
                    hs_syncml_Comm_ReSendData();
                }
#endif
                else 
                { 
                    /*���ճ����ر�socket*/
                    //syncml_http_message:"MMIPIM ** Read Failed"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1288_112_2_18_3_25_40_109,(uint8*)"");
                    HS_Data_Sync_PostMsgToUI( PIM_MSG_COMM_ERROR, NULL,NULL);
                    //sci_sock_socketclose(pim_socket);
                    hs_bFirstConnection=TRUE;
                    rc=HS_SML_ERR_A_COMM_ERROR;
                    return rc;
                } 
 #endif
            }	
        }
        break;
		 
         case SOCKET_STEP_2:  //����xml����
         {
             //syncml_http_message:"MMIPIM ** HS_HTTP_RecData   SOCKET_STEP_2"
             SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1302_112_2_18_3_25_40_110,(uint8*)"");
#ifndef WIN32
 		nrec = sci_sock_recv((long)pim_socket, (recbuffer+hs_totalrec),(int)(recsize-hs_totalrec), 0); /*lint !e737*/
#endif
              //syncml_http_message:"MMIPIM ** http contents received is %d"
              SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1306_112_2_18_3_25_40_111,(uint8*)"d",nrec);
		//�ɹ�����
		if( nrec >= 0 ) /*lint !e774*/
		{ /*���ճɹ�*/ 
#ifdef FEATURE_PIM_DEBUG
                       //write http logs to efs files.
                        //syncml_http_message(("MMIPIM ** Begin write http logs!\n"));                           
                       // hs_myhttplogs(PNULL,HTTPLOGNAME, (char *)(info->headerbuffer+hs_totalrec), nrec);
                       //  syncml_http_message(("MMIPIM ** http logs write complete!\n"));
#endif
			      hs_totalrec+=nrec;
                  //syncml_http_message:"MMIPIM ** content_length in header is %d"
                  SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1317_112_2_18_3_25_40_112,(uint8*)"d",hs_contentlength);
                  //syncml_http_message:"MMIPIM ** http contents received is %d"
                  SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1318_112_2_18_3_25_40_113,(uint8*)"d",hs_totalrec);
                  
                  if(hs_contentlength==hs_totalrec)
                  {
                          /* Post an event done data transfer at this time and will be handled
			      ** based on the test case to be run.
			      */
			      //syncml_http_message:"MMIPIM ** reading syncml contents complete..."
			      SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1325_112_2_18_3_25_40_114,(uint8*)"");
			      info->socket_step=SOCKET_STEP_3;

				hs_totalrec=0;  //reset rec total conunter.
				HS_Data_Sync_PostMsgToUI( PIM_MSG_RCV_DONE, NULL,NULL);
				
			      rc=HS_SML_ERR_OK;
			      return rc;
                  }
                  else
                  {
                       if(nrec!=0)/*lint !e774*/
			      {
			      		//not all data read over, read those left.
			      	 //syncml_http_message(("MMIPIM ** reading left..."));
			      	 //syncml_http_message:"MMIPIM ** receive next packet"
			      	 SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1340_112_2_18_3_25_40_115,(uint8*)"");
			        HS_Data_Sync_PostMsgToUI( PIM_MSG_RCV_DONE, NULL,NULL);
                           return rc;
			      }   
                       else
                       {
                           //syncml_http_message:"MMIPIM ** HTTP protocol error!receive count not equal to content length"
                           SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1346_112_2_18_3_25_40_116,(uint8*)"");
                           HS_Data_Sync_PostMsgToUI( PIM_MSG_COMM_ERROR, NULL,NULL);
                           return HS_SML_ERR_A_COMM_ERROR;
                       }

                  }
			  
		}
		else 
		{
			int errno1; 
#ifndef WIN32
			errno1 = sci_sock_errno((long)pim_socket);
                    //syncml_http_message:"MMIPIM ** HTTP protocol error code=%d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1359_112_2_18_3_25_40_117,(uint8*)"d",errno1);
			if(errno1 == EWOULDBLOCK || errno1 == EINPROGRESS)
			{ 
			     /*�Ժ���н���*/
			    //syncml_http_message:"MMIPIM ** Reading contents..."
			    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1363_112_2_18_3_25_40_118,(uint8*)"");
                        hs_is_socket_read=TRUE;
                        HS_PIM_active_socket_select_timer();
			} 
                     else if (errno1 == EPIPE) // chenxiang_20080326
                     {
                       // PIM_T_P  pMe = MMIPIM_GetPIM();
                        //syncml_http_message:"MMIPIM -------------(errno == EPIPE)---------------------"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1370_112_2_18_3_25_40_119,(uint8*)"");
            
                        hs_totalrec=0;  //reset receive total conunter.
                        //reset the communication's cache(xml buffer)and http cache(header buffer)
                        hs_smlUnlockWriteBuffer(hs_task_relay_info->workspaceid,hs_contentlength);
                        hs_smlLibMemset(hs_http_globle->headerbuffer,0,HS_HEADER_BUFFER_LEN); // chenxiang 20081209 ����headbuffer ��С��֧�ָ�����UA
                        info->usedsize=0;
                        //reset hs_contentlength
                        hs_contentlength=0;
                        hs_already_receive=FALSE;
                        sci_sock_socketclose(pMe->m_pISocket);
                        pMe->m_pISocket=-1;
                        //Socket �ÿ�
                        //syncml_http_message:"MMIPIM **socket connection closed."
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1383_112_2_18_3_25_41_120,(uint8*)"");
                        hs_bFirstConnection=TRUE;
                        hs_syncml_Comm_ReSendData();
                     }
			else 
			{ 
			    /*���ճ����ر�socket*/
			    //syncml_http_message:"MMIPIM ** Read Failed: Error %d\n"
			    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1390_112_2_18_3_25_41_121,(uint8*)"d",errno1);
                        HS_Data_Sync_PostMsgToUI( PIM_MSG_COMM_ERROR, NULL,NULL);
                        // sci_sock_socketclose(pim_socket);
			    hs_bFirstConnection=TRUE;
			    rc=HS_SML_ERR_A_COMM_ERROR;
			    return rc;
			} 
#endif
		}	
         }

         break;
         case SOCKET_STEP_3:  ////֪ͨsyncml taskģ�������һ�����������ν���ȫ��������
         {
                //syncml_http_message:"MMIPIM ** HS_HTTP_RecData   SOCKET_STEP_3"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1404_112_2_18_3_25_41_122,(uint8*)"");
                info->socket_step=SOCKET_STEP_CONNECT;  //next step is for send data,set this to setp_connet because the connection terminatied.
			//syncml_http_message:"MMIPIM **Receive all data ok!"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1406_112_2_18_3_25_41_123,(uint8*)"");
    			hs_totalrec=0;  //reset receive total conunter.
                //reset the communication's cache(xml buffer)and http cache(header buffer)
                hs_smlUnlockWriteBuffer(hs_task_relay_info->workspaceid,hs_contentlength);
                hs_smlLibMemset(hs_http_globle->headerbuffer,0,HS_HEADER_BUFFER_LEN); // chenxiang 20081209 ����headbuffer ��С��֧�ָ�����UA
                info->usedsize=0;
                //reset hs_contentlength
                hs_contentlength=0;

                HS_HTTP_CloseConnection();
         }break;
         default:
         {
            rc=HS_SML_ERR_A_COMM_ERROR;
            HS_Data_Sync_PostMsgToUI( PIM_MSG_COMM_ERROR, NULL,NULL);
		 return rc;
         }break;/*lint !e527*/
      }
           
  	return HS_SML_ERR_OK;
  }

 /******************************************************************* 
  ** ������:HS_HTTP_CloseConnection
  ** ����������close connection.
  ** ��  ��: ���ز���״̬
  ** ��  ��:wangxiaolin
  ** ��  ��:2007-11-14
  ** �汾: ver 1.0
  *******************************************************************/
  void  HS_HTTP_CloseConnection()
   {
   #ifndef WIN32
#ifndef PIM_HTTP_20_SUPPORT  //Added by wangxiaolin   2010.1.5 for HTTP 2.0
     PIM_T_P  pMe = MMIPIM_GetPIM();

     sci_sock_socketclose(pMe->m_pISocket);
     pMe->m_pISocket=-1;
  
     //Socket �ÿ�
    //syncml_http_message:"MMIPIM **socket connection closed."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1446_112_2_18_3_25_41_124,(uint8*)"");
    hs_bFirstConnection=TRUE;
#endif /* PIM_HTTP_20_SUPPORT */
   
    HS_Data_Sync_PostMsgToUI( PIM_MSG_NEXT_STEP, NULL,NULL);
  #endif
   }

#ifndef WIN32
  /******************************************************************* 
  ** ������:hs_GetserverAddrandPort
  ** ������������host�ַ����еõ�server��ַ�Ͷ˿ںš�
  ** ��  ��: 
  ** ��  ��:wangxiaolin
  ** ��  ��:2007-11-17
  ** �汾: ver 1.0
  *******************************************************************/
 static boolean hs_GetserverAddrandPort(const char* server_name, const char* server_port, uint32* serveraddr, uint16* serverport)
  {
  	char* addr;
	char* port;
	const char* position;
	const char* position2;
	int i=0;
	int serv_port=0;

	position=server_name;
	position2=server_name;
    #if 0
	while(1)
	{
		if(*position==':')
		{
			position++;
			i++;
			if(*position=='/')
			{
				position++;
				i++;
				if(*position=='/')
				{
					position++;
					i++;
					break;
				}
			}
		}
		position++;
		i++;
		if(i>xppStrlen(server_name))
		{
			return FALSE;
		}
	}
    #endif
	position2=position;
	i=1;
	while(*position2!='/')
	{
		position2++;
		i++;
		if(*position2=='\0')
		{
			break;
		}
	}

	addr=xppMalloc(i+1);
	xppMemset(addr, 0, i+1);
	xppStrncpy(addr, position, i);

	port=xppMalloc(xppStrlen(server_port)+1);
	xppMemset(port, 0, xppStrlen(server_port)+1);
	xppStrncpy(port, server_port, xppStrlen(server_port));

       
#ifndef WIN32
	inet_aton(addr, serveraddr);
	serv_port=atoi(port);
    *serverport = htons(serv_port);
#endif
       xppFree(addr); // chenxiang_memory 20080312
       xppFree(port); // chenxiang_memory 20080312
	 return TRUE;
  }
#endif
/******************************************************************* 
** ������:hs_writeRequestHeader
** ����������write http request header.
** ��  ��:  
** ��  ��:wangxiaolin
** ��  ��:2007-11-7
** �汾: ver 1.0
*******************************************************************/
static void hs_writeRequestHeader (void)
{
    char *pszURI;
    char *pszMode = "POST";
    char *pszHeader = (char *) hs_http_globle->headerbuffer;
    pszURI=hs_http_globle->server;

    /*************************************************************/
    /* Write the request header line. Use absolute URI addresses */
    /* if the connection goes via a proxy.                       */
    /*************************************************************/
    if (HS_HEADER_BUFFER_LEN > (xppStrlen (pszHeader) + xppStrlen(pszMode) + xppStrlen(pszURI) + xppStrlen("  "HS_HTTP_VERSION"\r\n")))
    {
        sprintf(pszHeader, "%s %s "HS_HTTP_VERSION"\r\n",pszMode, pszURI);
    }
    else
    {
        //syncml_http_message:"MMIPIM chenxiang !!!!!!! headerbuffer over flow !!! FILE: %s , LINE: %s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1556_112_2_18_3_25_41_125,(uint8*)"ss", __FILE__, __LINE__);
    }

    /*************************************************/
    /* Write general header and content header data. */
    /*************************************************/

    hs_writeGeneralRequestHeader (pszURI, pszHeader);

    hs_writeContentRequestHeader (pszHeader);


    if (HS_HEADER_BUFFER_LEN > (xppStrlen (pszHeader) + 2))
    {
        sprintf (pszHeader + xppStrlen (pszHeader), "\r\n");
    }
    else
    {
        //syncml_http_message:"MMIPIM chenxiang !!!!!!! headerbuffer over flow !!! FILE: %s , LINE: %s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1574_112_2_18_3_25_41_126,(uint8*)"ss", __FILE__, __LINE__);
    }

    hs_http_globle->usedsize= xppStrlen (pszHeader);

    //SCI_ASSERT(hs_http_globle->usedsize < HS_HEADER_BUFFER_LEN); // chenxiang 20081209 ����headbuffer ��С��֧�ָ�����UA

    //syncml_http_message:"MMIPIM chenxiang !!!!!!! hs_http_globle->usedsize == %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1581_112_2_18_3_25_41_127,(uint8*)"d", hs_http_globle->usedsize);
    {
        uint16 i = 0;
        //syncml_http_message:"MMIPIM chenxiang !!!!!!! ------------headerbuffer------start------"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1584_112_2_18_3_25_41_128,(uint8*)"");
        for (i = 0; i < HS_HEADER_BUFFER_LEN + 50; i+=50)
            //syncml_http_message:"%s"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1586_112_2_18_3_25_41_129,(uint8*)"s", hs_http_globle->headerbuffer + i);
        //syncml_http_message:"MMIPIM chenxiang !!!!!!! ------------headerbuffer-------end-----"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1587_112_2_18_3_25_41_130,(uint8*)"");
    }
}

/******************************************************************* 
** ������:hs_writeGeneralRequestHeader
** ����������write http general request header.
** ��  ��:  
** ��  ��:����
** ��  ��:2005-9-7
** �汾: ver 1.0
*******************************************************************/
static   void hs_writeGeneralRequestHeader (char* pszURI, char *pszHeader)
{
    MMIPIM_UA_INFO_T custom_ua_info={0};
    #ifdef HS_PIM_UA_SETTING
    PIM_T_P  pMe=MMIPIM_GetPIM();
    #endif
    if (HS_HEADER_BUFFER_LEN > (xppStrlen (pszHeader) + xppStrlen(pszURI) + xppStrlen("Host: \r\n")))
    {
        sprintf (pszHeader + xppStrlen (pszHeader),
               "Host: %s\r\n", pszURI);
    }
    else
    {
        //syncml_http_message:"MMIPIM chenxiang !!!!!!! headerbuffer over flow !!! FILE: %s , LINE: %s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1611_112_2_18_3_25_41_131,(uint8*)"ss", __FILE__, __LINE__);
    }

    if (HS_HEADER_BUFFER_LEN > (xppStrlen (pszHeader) + xppStrlen("Accept: "HS_HTTP_MIME_TYPES"\r\n")))
    {
        sprintf(pszHeader + xppStrlen (pszHeader),
          "Accept: "HS_HTTP_MIME_TYPES"\r\n");
    }
    else
    {
        //syncml_http_message:"MMIPIM chenxiang !!!!!!! headerbuffer over flow !!! FILE: %s , LINE: %s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1621_112_2_18_3_25_41_132,(uint8*)"ss", __FILE__, __LINE__);
    }

    if (HS_HEADER_BUFFER_LEN > (xppStrlen (pszHeader) + xppStrlen("Accept-Charset: UTF-8\r\n")))
    {
        sprintf(pszHeader + xppStrlen (pszHeader),
        	   "Accept-Charset: UTF-8\r\n");
    }
    else
    {
        //syncml_http_message:"MMIPIM chenxiang !!!!!!! headerbuffer over flow !!! FILE: %s , LINE: %s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1631_112_2_18_3_25_41_133,(uint8*)"ss", __FILE__, __LINE__);
    }

    MMIPIM_GetUAInfo(pMe->m_ua_type,&custom_ua_info);
//#ifdef HS_PIM_UA_SETTING
//  
//    if(1 == pMe->m_ua_type)
//    {
//        if (HS_HEADER_BUFFER_LEN > (xppStrlen (pszHeader) + xppStrlen("User-Agent: ") + xppStrlen(CLIENT_NAME_UA_INFO_HS) + 2))
//        {
//            sprintf(pszHeader + xppStrlen (pszHeader),
//            	"User-Agent: ");
//            sprintf(pszHeader + xppStrlen (pszHeader),
//            	custom_ua_info.ua);
//            sprintf(pszHeader + xppStrlen (pszHeader),
//            	"\r\n");
//        }       
//        else
//        {
//            syncml_http_message(("MMIPIM chenxiang !!!!!!! headerbuffer over flow !!! FILE: %s , LINE: %s", __FILE__, __LINE__));
//        }
//    }
//    else if(2 == pMe->m_ua_type)
//    {
//        if (HS_HEADER_BUFFER_LEN > (xppStrlen (pszHeader) + xppStrlen("User-Agent: ") + xppStrlen(custom_ua_info.ua) + 2))
//        {
//            sprintf(pszHeader + xppStrlen (pszHeader),
//            	"User-Agent: ");
//            sprintf(pszHeader + xppStrlen (pszHeader),
//            	custom_ua_info.ua);
//            sprintf(pszHeader + xppStrlen (pszHeader),
//            	"\r\n");
//        }       
//        else
//        {
//            syncml_http_message(("MMIPIM chenxiang !!!!!!! headerbuffer over flow !!! FILE: %s , LINE: %s", __FILE__, __LINE__));
//        }
//    }
//    else
// #endif //end of HS_PIM_UA_SETTING  
    {
        if (HS_HEADER_BUFFER_LEN > (xppStrlen (pszHeader) + xppStrlen("User-Agent: ") + xppStrlen(custom_ua_info.ua) + 2))
        {
            sprintf(pszHeader + xppStrlen (pszHeader),
            	"User-Agent: ");
            sprintf(pszHeader + xppStrlen (pszHeader),
            	custom_ua_info.ua);
            sprintf(pszHeader + xppStrlen (pszHeader),
            	"\r\n");
        } 
                else
        {
            //syncml_http_message:"MMIPIM chenxiang !!!!!!! headerbuffer over flow !!! FILE: %s , LINE: %s"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1683_112_2_18_3_25_41_134,(uint8*)"ss", __FILE__, __LINE__);
        }
    }


    if (HS_HEADER_BUFFER_LEN > (xppStrlen (pszHeader) + xppStrlen("Cache-Control: no-store\r\n")))
    {
        sprintf(pszHeader + xppStrlen (pszHeader),
        	"Cache-Control: no-store\r\n");
    }
    else
    {
        //syncml_http_message:"MMIPIM chenxiang !!!!!!! headerbuffer over flow !!! FILE: %s , LINE: %s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1695_112_2_18_3_25_41_135,(uint8*)"ss", __FILE__, __LINE__);
    }
    return;
}
/******************************************************************* 
** ������:hs_writeContentRequestHeader
** ����������write http content request header.
** ��  ��:  
** ��  ��:����
** ��  ��:2005-9-7
** �汾: ver 1.0
*******************************************************************/
static void hs_writeContentRequestHeader (char *pszHeader)
{
    char* content_type;
    char* charset;

    if(hs_global_Comm_Ptr->codetype==XML_WBXML)
    {
        content_type=HS_HTTP_WBXML_MIME_TYPES;
    }
    else
    {
        content_type=HS_HTTP_XML_MIME_TYPES;
    }

    charset=HS_HTTP_CODEC;
	   
    if (HS_HEADER_BUFFER_LEN > (xppStrlen (pszHeader) + xppStrlen("Content-Type: ; charset=\r\n") + xppStrlen(content_type) + xppStrlen(charset)))
    {
    sprintf(pszHeader + xppStrlen (pszHeader),
        "Content-Type: %s; charset=%s\r\n",content_type, charset);
    }
    else
    {
        //syncml_http_message:"MMIPIM chenxiang !!!!!!! headerbuffer over flow !!! FILE: %s , LINE: %s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1729_112_2_18_3_25_41_136,(uint8*)"ss", __FILE__, __LINE__);
    }

    if (hs_global_Comm_Ptr->cache_length > 0)
    {
        if (HS_HEADER_BUFFER_LEN > (xppStrlen (pszHeader) + xppStrlen("Content-Length: \r\n") + 10))
        {
            sprintf (pszHeader + xppStrlen (pszHeader),
                "Content-Length: %ld\r\n", hs_global_Comm_Ptr->cache_length );
        }
        else
        {
            //syncml_http_message:"MMIPIM chenxiang !!!!!!! headerbuffer over flow !!! FILE: %s , LINE: %s"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCOMMHTTP_1741_112_2_18_3_25_41_137,(uint8*)"ss", __FILE__, __LINE__);
        }
    }

    /*******************************************************************/
    return;
}

  /******************************************************************* 
  ** ������:hs_ReadRequestHeader
  ** ����������read http request header, look if the http response is 200 ok
                              if not, return FALSE,else give the hs_contentlength to clength.
  ** ��  ��:  
  ** ��  ��:����
  ** ��  ��:2005-9-7
  ** �汾: ver 1.0
  *******************************************************************/
static int hs_ReadRequestHeader (int * clength, int* headersize) // chenxiang_20080326
  {
     char* buffer;
     int i;
     http_globle_indicator_type* info;
     boolean findhead=FALSE;
     info=(http_globle_indicator_type*)(hs_global_Comm_Ptr->privateinfo);
     buffer=  info->headerbuffer;
     for(i=0;i<HS_HEADER_BUFFER_LEN-4;i++) // chenxiang 20081209 ����headbuffer ��С��֧�ָ�����UA
     {
         if((*(buffer+i)=='\r')&&(*(buffer+i+1)=='\n')&&(*(buffer+i+2)=='\r')&&(*(buffer+i+3)=='\n'))
         {
            i+=4;  //make i to the content  first position.
            *headersize=i;
            findhead=TRUE;
            break;
         }
     }
     if(FALSE==findhead)
     {
         return 0;
     }
     return hs_AnalysisHeader(buffer,*headersize,clength);
     
  }

  /******************************************************************* 
  ** ������:hs_AnalysisHeader
  ** ���������� look if the http response is 200 ok
                              if not, return FALSE,else give the hs_contentlength to clength.
                              if it is ok, get the content's length.
  ** ��  ��:  
  ** ��  ��:����
  ** ��  ��:2005-9-7
  ** �汾: ver 1.0
  *******************************************************************/
static int hs_AnalysisHeader (char* buffer, int  headersize, int* clength) // chenxiang_20080326
  {
      char* postion;
	char response[5]={0};
	char length[5]={0};
	int res;
	int i;

	//set i to 0
	i=0;
	postion=xppStrstr(buffer,"HTTP/1.1");
	if(NULL==postion)
	{
		return 0;
	}
	postion+=9;
	while(*(postion+i)!=' ')
	{
		response[i]=*(postion+i);
		i++;
	}
	for(;i<5;i++)
	{
		response[i]=0;
	}

	//reset i to 0
	i=0;
	postion=xppStrstr(buffer,"Content-Length:");
     	//SCI_TRACE_MID_PIM("MMIPIM chenxiang ----hs_AnalysisHeader -2-postion = %d ", postion);
	if(NULL==postion) // chenxiang 20080228
	{
		postion=xppStrstr(buffer,"Content-length:");
	}
	if(NULL==postion)
	{
		return 0;
	}
	postion+=16;
	while(*(postion+i)!='\r')
	{
		length[i]=*(postion+i);
		i++;
	}
	for(;i<5;i++)
	{
		length[i]=0;
	}

	res=xppAtoi(response);
//	if(200!=res)
//	{
//		return 0;
//	}
	*clength=xppAtoi(length);
	return res;
  }

  #endif
