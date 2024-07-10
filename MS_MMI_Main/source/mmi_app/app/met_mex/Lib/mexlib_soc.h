/************************************************************************
* ��Ȩ���� (C)2010,��Ѷ�Ƽ���
* 
* �ļ����ƣ� // MexLib_Soc.h
* �ļ���ʶ��
* ����ժҪ�� // mexƽ̨soc�⺯��ͷ�ļ�
*
************************************************************************/
#if defined(MEX_TARGET_SERVER) && !defined(__MEXLIB_SOC_SERVER_H__) || defined(MEX_TARGET_CLIENT) && !defined(__MEXLIB_SOC_CLIENT_H__)

#if defined(MEX_TARGET_SERVER)
	#define __MEXLIB_SOC_SERVER_H__
#elif defined(MEX_TARGET_CLIENT)
	#define __MEXLIB_SOC_CLIENT_H__
#endif












//common macro definitions for SOC
#define MEX_H_LAUNCH_FUNC_SOC					MEX_H_LAUNCH_FUNC( Soc )
#define MEX_C_LIB_FUNC_NAME_SOC					MEX_C_LIB_FUNC_NAME( Soc )

#define MEX_H_GET_FUNC_DEFINE_SOC(x)			MEX_H_GET_FUNC_DEFINE( Soc, x )

#define MEX_H_ENUM_FUNC_START_SOC				MEX_H_ENUM_FUNC_START( Soc )
#define MEX_H_ENUM_FUNC_SOC(x)					MEX_H_ENUM_FUNC( Soc, x )
#define MEX_H_ENUM_FUNC_END_SOC					MEX_H_ENUM_FUNC_END( Soc )

#define MEX_H_NB_FUNC_SOC						MEX_H_NB_FUNC( Soc )
//end common macro definitions

///////////////////////////////////////////////////////////////////////////
// common socket definitions and functions, interface to mtk socket apis
///////////////////////////////////////////////////////////////////////////

typedef void (*soc_funcptr)(void *);


#ifdef MEX_TARGET_SERVER
#define	Convert mex_ConvertSysDef
#define GPRS_CMNET_ACCOUNT_NAME			"MEX_NET"	//cmnet�˺�APN����
#define GPRS_CMWAP_ACCOUNT_NAME			"MEX_WAP"	//cmwap�˺�APN����
#else
#define Convert ConvertSysDef
#endif



/************************************************************************/
/* socket����                                                           */
/************************************************************************/


#if defined (MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
/* stream socket, TCP */   //���Э���ǰ���˳��ġ��ɿ��ġ����������Ļ����ֽ��������ӡ�����һ��ʹ������socket���ͣ����socket��ʹ��TCP�����д��䡣
#define MEX_STREAM_SOCKET	( Convert(E_M2MDC_SOCK_STREAM) )		
/* datagram socket, UDP */ //���Э���������ӵġ��̶����ȵĴ�����á���Э���ǲ��ɿ��ģ�ʹ��UDP�������������ӡ�
#define MEX_DGRAM_SOCKET	( Convert(E_M2MDC_SOCK_DGRAM) )		
#define MEX_SMS_BEARER		( Convert(E_M2MDC_SOCK_SMS) )		/* SMS bearer */
#define MEX_RAM_SOCKET		( Convert(E_M2MDC_SOCK_RAW) )		/* raw socket */
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#define MEX_STREAM_SOCKET	( Convert(E_S2MDC_SOC_STREAM) )		
#define MEX_DGRAM_SOCKET	( Convert(E_S2MDC_SOC_DGRAM) )		
#define MEX_SMS_BEARER		( Convert(E_S2MDC_SOC_SMS) )		/* SMS bearer */
#define MEX_RAM_SOCKET		( Convert(E_S2MDC_SOC_RAW) )		/* raw socket */

#endif







//typedef enum
//{
//	E_MST_STREAM = 0,		/* stream socket, TCP */   //���Э���ǰ���˳��ġ��ɿ��ġ����������Ļ����ֽ��������ӡ�����һ��ʹ������socket���ͣ����socket��ʹ��TCP�����д��䡣
//	E_MST_DGRAM,			/* datagram socket, UDP */ //���Э���������ӵġ��̶����ȵĴ�����á���Э���ǲ��ɿ��ģ�ʹ��UDP�������������ӡ�
//	E_MST_SMS,				/* SMS bearer */
//	E_MST_RAW				/* raw socket */
//} int8;		// == socket_type_enum

#define MEX_MAX_SOCK_ADDR_LEN           (28)	//MAX_SOCK_ADDR_LEN

typedef struct
{
	int32				sock_type;				//socket���� 
	int16				addr_len;				//��ַ����  Ҳ��IP��ַ���ֽ���  IPV4�е�ip��ַ�� 4���ֽ�  32λ�� 
	uint16				port;					//�˿�
	uint8				addr[MEX_MAX_SOCK_ADDR_LEN]; //IP��ַ   ��ͬƽ̨Ӧ��ע���С������
} Mex_SockAddr_Struct;		// == sockaddr_struct


/*network type*/
typedef enum
{
	E_MNM_CMWAP,
	E_MNM_CMNET,
	E_MNM_MAX
}E_Mex_Net_Mode;

/*operator result*/
typedef enum
{
	E_MOR_SUCCESS		= 1,			//�ɹ�
	E_MOR_WAITING		= 0,			//��Ҫ�ȴ��ص�
	E_MOR_FAILED		= -1,			//��ͨ����   ��������������ִ��� Ӧ�ü��һ�³����߼��Ƿ����
	E_MOR_SIM_ERROR		= -2,			//sim������ һ������¾���sim����Ч
	E_MOR_MEM_ERROR		= -3,			//�ڴ����	һ�������Ϊ�����ڴ�ʧ��
	E_MOR_SYS_BUSY		= -4,			//ϵͳæ		һ������� ������Ӧ�ó������ڵ���
	E_MOR_INVALID		= -5,			//���Ϸ���   ���ֲ�������ŵ�����Ӧ�ó��� 
	E_MOR_OVER_LIMIT	= -6,			//socket��������ϵͳ֧�ֵ����ֵ������������appû���ͷ��Լ���socket
	E_MOR_ALREADY_REG	= -7,			//�Ѿ���ע���socket id
	E_MOR_MODE_ERROR	= -8,			//���������ģʽ���� 
}E_Mex_Operator_Result;

/************************************************************************/
/*Socket Options                                                        */
/************************************************************************/



#if defined (MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#define MEX_SOC_OOBINLINE		( Convert(E_M2MDC_SOC_OOBINLINE) )				
#define MEX_SOC_LINGER			( Convert(E_M2MDC_SOC_LINGER) )
#define MEX_SOC_NBIO			( Convert(E_M2MDC_SOC_NBIO) )
#define MEX_SOC_ASYNC			( Convert(E_M2MDC_SOC_ASYNC) )

#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#define MEX_SOC_OOBINLINE		( Convert(E_S2MDC_SOC_OOBINLINE) )				
#define MEX_SOC_LINGER			( Convert(E_S2MDC_SOC_LINGER) )
#define MEX_SOC_NBIO			( Convert(E_S2MDC_SOC_NBIO) )
#define MEX_SOC_ASYNC			( Convert(E_S2MDC_SOC_ASYNC) )
#endif



//.....TODO


//typedef enum
//{
//	E_MEX_SOC_OOBINLINE     		= 0x01 << 0,  /* not support yet */
//	E_MEX_SOC_LINGER        		= 0x01 << 1,  /* linger on close */
//	E_MEX_SOC_NBIO          		= 0x01 << 2,  /* Nonblocking */
//	E_MEX_SOC_ASYNC         		= 0x01 << 3,  /* Asynchronous notification */   
//
//	E_MEX_SOC_NODELAY       		= 0x01 << 4,  /* disable Nagle algorithm or not */
//	E_MEX_SOC_KEEPALIVE     		= 0x01 << 5,  
//	E_MEX_SOC_RCVBUF        		= 0x01 << 6,  
//	E_MEX_SOC_SENDBUF       		= 0x01 << 7,
//
//	E_MEX_SOC_NREAD         		= 0x01 << 8,  /* no. of bytes for read, only for soc_getsockopt */
//	E_MEX_SOC_PKT_SIZE      		= 0x01 << 9,  /* datagram max packet size */
//	E_MEX_SOC_SILENT_LISTEN 		= 0x01 << 10, /* SOCK_SMS property */
//	E_MEX_SOC_QOS           		= 0x01 << 11,
//
//	E_MEX_SOC_TCP_MAXSEG    		= 0x01 << 12,
//	E_MEX_SOC_IP_TTL        		= 0x01 << 13,
//	E_MEX_SOC_LISTEN_BEARER 		= 0x01 << 14,
//	E_MEX_SOC_UDP_ANY_FPORT 		= 0x01 << 15,
//
//	E_MEX_SOC_WIFI_NOWAKEUP 		= 0x01 << 16, /* send packet in power saving mode */
//	E_MEX_SOC_UDP_NEED_ICMP 		= 0x01 << 17, /* deliver NOTIFY(close) for ICMP error */
//	E_MEX_SOC_IP_HDRINCL			= 0x01 << 18, /* IP header included for raw sockets */
//	E_MEX_SOC_NON_AUTO_ACT_BEARER	= 0x01 << 19, /* Non-auto activate bearer */
//	E_MEX_SOC_AUTO_DEACT_BEARER		= 0x01 << 20  /* auto deactivate bearer */
//}E_Mex_Soc_Option;


/************************************************************************/
/* socket notify                                                        */
/************************************************************************/


#if defined (MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#define MEX_SOC_NWPAUSE			(-4)					//������ʱ����
#define MEX_SOC_NWRESUME		(-3)					//����ָ�
#define MEX_SOC_NWCLOSED		(-2)					//���类ǿ�ƹر�
#define MEX_SOC_READ			( Convert(E_M2MDC_SOC_READ) )
#define MEX_SOC_WRITE			( Convert(E_M2MDC_SOC_WRITE) )
#define MEX_SOC_ACCEPT			( Convert(E_M2MDC_SOC_ACCEPT) )
#define MEX_SOC_CONNECT			( Convert(E_M2MDC_SOC_CONNECT) )
#define MEX_SOC_CLOSE			( Convert(E_M2MDC_SOC_CLOSE) )

#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)

#define MEX_SOC_NWPAUSE			(-4)					//������ʱ����
#define MEX_SOC_NWRESUME		(-3)					//����ָ�
#define MEX_SOC_NWCLOSED		(-2)					//���类ǿ�ƹر�

#define MEX_SOC_READ			( Convert(E_S2MDC_SOC_READ) )
#define MEX_SOC_WRITE			( Convert(E_S2MDC_SOC_WRITE) )
#define MEX_SOC_ACCEPT			( Convert(E_S2MDC_SOC_ACCEPT) )
#define MEX_SOC_CONNECT			( Convert(E_S2MDC_SOC_CONNECT) )
#define MEX_SOC_CLOSE			( Convert(E_S2MDC_SOC_CLOSE) )
#endif





//typedef enum
//{
//	E_MSE_SOC_ERROR   = -1,
//	E_MSE_SOC_READ	  = 0x01,  /* Notify for read */
//	E_MSE_SOC_WRITE   = 0x02,  /* Notify for write */
//	E_MSE_SOC_ACCEPT  = 0x04,  /* Notify for accept */
//	E_MSE_SOC_CONNECT = 0x08,  /* Notify for connect */
//	E_MSE_SOC_CLOSE   = 0x10   /* Notify for close */
//} E_Mex_Soc_Event;

typedef struct
{
	int8 socketId;
	int8 event;			
}Mex_Soc_Stat;

//��ǰ����
typedef struct 
{
	int8 nw_sim;					//ϵͳ��ǰ����sim��
	int8 nw_mode;					//ϵͳ��������
	uint32 nw_account;				//�����˺�
	bool bInit;						//�Ƿ��ѱ���ʼ��
}Mex_NetWork_Setting;






/************************************************************************/
/* socket error code                                                    */
/************************************************************************/
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	#define MEX_SOC_SUCCESS				( Convert(E_M2MDC_SOC_SUCCESS) )				//�����ɹ�
	#define MEX_SOC_ERROR				( Convert(E_M2MDC_SOC_ERROR) )				//����ʧ��
	#define MEX_SOC_WOULDBLOCK			( Convert(E_M2MDC_SOC_WOULDBLOCK) )			//����
	#define MEX_SOC_LIMIT_RESOURCE		( Convert(E_M2MDC_SOC_LIMIT_RESOURCE) )		//���޵���Դ
	#define MEX_SOC_INVALID_SOCKET		( Convert(E_M2MDC_SOC_INVALID_SOCKET) )		//�Ƿ���socket
	#define MEX_SOC_INVALID_ACCOUNT		( Convert(E_M2MDC_SOC_INVALID_ACCOUNT) )		//�Ƿ��������˺�
	#define MEX_SOC_NAMETOOLONG			( Convert(E_M2MDC_SOC_NAMETOOLONG) )			//��ַ����
	#define MEX_SOC_ALREADY				( Convert(E_M2MDC_SOC_ALREADY) )				//�����Ѿ���ִ��
	#define MEX_SOC_OPNOTSUPP			( Convert(E_M2MDC_SOC_OPNOTSUPP) )			//��֧�ָò���
	#define MEX_SOC_CONNABORTED			( Convert(E_M2MDC_SOC_CONNABORTED) )			//����ճɵ������ж�			
	#define MEX_SOC_INVAL				( Convert(E_M2MDC_SOC_INVAL) )				//�Ƿ��Ĳ���
	#define MEX_SOC_PIPE				( Convert(E_M2MDC_SOC_PIPE) )					//�ܵ�����
	#define MEX_SOC_NOTCONN				( Convert(E_M2MDC_SOC_NOTCONN) )				//δ���ӵ�socket
	#define MEX_SOC_MSGSIZE				( Convert(E_M2MDC_SOC_MSGSIZE) )				//��Ϣ����
	#define MEX_SOC_BEARER_FAIL			( Convert(E_M2MDC_SOC_BEARER_FAIL) )			//����������
	#define MEX_SOC_CONNRESET			( Convert(E_M2MDC_SOC_CONNRESET) )			
	#define MEX_SOC_DHCP_ERROR			( Convert(E_M2MDC_SOC_DHCP_ERROR) )
	#define MEX_SOC_IP_CHANGED			( Convert(E_M2MDC_SOC_IP_CHANGED) )
	#define MEX_SOC_ADDRINUSE			( Convert(E_M2MDC_SOC_ADDRINUSE) )
	#define MEX_SOC_CANCEL_ACT_BEARER	( Convert(E_M2MDC_SOC_CANCEL_ACT_BEARER) )


	#define MEX_PF_INET			( Convert(E_M2MDC_PF_INET) )			/*currently only support Internet*/

	/* currently, only allow applicaion to set this type when RAW_SOCKET is used */    
	#define MEX_IPPROTO_ICMP	( Convert(E_M2MDC_IPPROTO_ICMP) ) /* control message protocol */

	/* for SOC/TCP/IP internal use, application shall NOT use */
	#define MEX_IPPROTO_IP      ( Convert(E_M2MDC_IPPROTO_IP) )     /* dummy for IP */
	#define MEX_IPPROTO_HOPOPTS	( Convert(E_M2MDC_IPPROTO_HOPOPTS) )     /* IP6 hop-by-hop options */
	#define MEX_IPPROTO_IGMP	( Convert(E_M2MDC_IPPROTO_IGMP) )     /* group mgmt protocol */
	#define MEX_IPPROTO_IPV4	( Convert(E_M2MDC_IPPROTO_IPV4) )     /* IPv4 encapsulation */
	#define MEX_IPPROTO_IPIP	( Convert(E_M2MDC_IPPROTO_IPIP) )    /* for compatibility */
	#define MEX_IPPROTO_TCP		( Convert(E_M2MDC_IPPROTO_TCP) )             /* tcp */
	#define MEX_IPPROTO_UDP		( Convert(E_M2MDC_IPPROTO_UDP) )    /* user datagram protocol */
	#define MEX_IPPROTO_RAW		( Convert(E_M2MDC_IPPROTO_RAW) )   /* raw IP packet */

#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
	#define MEX_PF_INET					( Convert(E_S2MDC_PF_INET))						//pf_inet
	#define MEX_IPPROTO_IP				( Convert(E_S2MDC_IPPROTO_IP))
	#define MEX_SOC_SUCCESS				( Convert(E_S2MDC_SOC_SUCCESS) )				//�����ɹ�
	#define MEX_SOC_ERROR				( Convert(E_S2MDC_SOC_ERROR) )					//����ʧ��
	#define MEX_SOC_WOULDBLOCK			( Convert(E_S2MDC_SOC_WOULDBLOCK))
#endif
















MEX_H_DECLARE_FUNC( int8,				soc_create,								(uint8 domain, int8 type, uint8 protocol, uint8 dummy, uint32 nwk_account_id) )
MEX_H_DECLARE_FUNC( int8,				soc_close,								(int8 s) )
MEX_H_DECLARE_FUNC( int8,				soc_bind,								(int8 s, Mex_SockAddr_Struct *addr) )
MEX_H_DECLARE_FUNC( int8,				soc_listen,								(int8 s, uint8 backlog) )
MEX_H_DECLARE_FUNC( int8,				soc_accept,								(int8 s, Mex_SockAddr_Struct *addr) )
MEX_H_DECLARE_FUNC( int8,				soc_connect,							(int8 s, Mex_SockAddr_Struct *addr) )
MEX_H_DECLARE_FUNC( int32,				soc_sendto,								(int8 s, uint8 *buf, int32 len, uint8 flags, Mex_SockAddr_Struct *addr) )
MEX_H_DECLARE_FUNC( int32,				soc_send,								(int8 s, uint8 *buf, int32 len, uint8 flags) )
MEX_H_DECLARE_FUNC( int32,				soc_recvfrom,							(int8 s, uint8 *buf, int32 len, uint8 flags, Mex_SockAddr_Struct *fromaddr) )
MEX_H_DECLARE_FUNC( int32,				soc_recv,								(int8 s, uint8 *buf, int32 len, uint8 flags) )

MEX_H_DECLARE_FUNC( int8,				soc_setsockopt,							(int8 s, uint32 option, void *val, uint8 val_size) )
MEX_H_DECLARE_FUNC( int8,				soc_getsockopt,							(int8 s, uint32 option, void *val, uint8 val_size) )

MEX_H_DECLARE_FUNC( int8,				soc_gethostbyname,						(bool is_blocking, uint8 mod_id, int32 request_id, const char *domain_name, uint8 *addr, uint8 *addr_len,\
																 uint8 access_id, uint32 nwk_account_id) )
MEX_H_DECLARE_FUNC( int8,				soc_gethostbyname2,						(bool is_blocking, uint8 mod_id, int32 request_id, const char *domain_name, uint8 *addr, uint8 *addr_len,\
																 uint8 access_id, uint32 nwk_account_id, uint8 in_entry_num, uint8 *out_entry_num) )
MEX_H_DECLARE_FUNC( int8,				soc_gethostbyaddr,						(bool is_blocking, uint8 mod_id, int32 request_id, char *domain_name, uint32 *domain_name_len, \
																 const uint8 *addr, uint8 addr_len, uint8 access_id, uint32 nwk_account_id) )

MEX_H_DECLARE_FUNC( int8,				soc_open_nwk_account,					(uint8 mod_id, uint32 account_id) )
MEX_H_DECLARE_FUNC( void,				soc_close_nwk_account,					(uint8 mod_id) )
MEX_H_DECLARE_FUNC( void,				soc_close_nwk_account_by_id,			(uint8 mod_id, uint32 account_id) )

MEX_H_DECLARE_FUNC( int8,				soc_getlocalip,							(uint8 *local_ip) )
MEX_H_DECLARE_FUNC( int8,				soc_get_account_localip,				(int8 s, uint8 *local_ip) )

MEX_H_DECLARE_FUNC( int8,				soc_getsockaddr,						(int8 s, bool is_local, Mex_SockAddr_Struct *addr) )

MEX_H_DECLARE_FUNC( int8,				soc_get_last_error,						(int8 s, int8 *error, int32 *detail_cause) )
MEX_H_DECLARE_FUNC( int8,				soc_set_last_error,						(int8 s, int8 error, int32 detail_cause) )

MEX_H_DECLARE_FUNC( int8,				soc_open_bearer,						(int8 s) )
MEX_H_DECLARE_FUNC( int8,				soc_close_bearer,						(int8 s) )

MEX_H_DECLARE_FUNC( bool,				soc_ready_for_read,						(int8 s) )
MEX_H_DECLARE_FUNC( uint16,				soc_cksum,								(uint16 *buf, int32 len) )
MEX_H_DECLARE_FUNC( int8,				init_network,							(uint32 *nwk_account_id, int8 sim_id, soc_funcptr cb, E_Mex_Net_Mode mode, int32 iTimeout, int32 app_id) )
MEX_H_DECLARE_FUNC( int8,				soc_get_host_by_name_event_handle,		(soc_funcptr func, int32 iTimeout, int32 app_id) )

MEX_H_DECLARE_FUNC( int8,				cancel_gethost,							(int32 app_id) )
MEX_H_DECLARE_FUNC( int8,				cancel_soc_notify,						(int8 socket_id, int32 app_id) )
MEX_H_DECLARE_FUNC( int8,				cancel_init_network,					(int32 app_id) )

MEX_H_DECLARE_FUNC( int8,				reg_soc_notify,							(int8 socket_id, soc_funcptr cb, int32 app_id) )
MEX_H_DECLARE_FUNC( int8,				cancel_all_soc_notify,					(int32 app_id) )
MEX_H_DECLARE_FUNC( void,				close_network,							(uint32 nwk_account_id,int8 sim_id) )
MEX_H_DECLARE_FUNC( int32,				soc_get_nw_account,						(void) )
MEX_H_DECLARE_FUNC( E_Mex_Net_Mode,		soc_get_nw_mode,						(void) )
MEX_H_DECLARE_FUNC( int8,				soc_get_nw_sim,							(void) )
MEX_H_DECLARE_FUNC( void,				soc_set_nw_mode,						(E_Mex_Net_Mode mode) )
MEX_H_DECLARE_FUNC( void,				soc_set_nw_sim,							(int8 sim) )
MEX_H_DECLARE_FUNC( void,				soc_reset_nw,							(void) )
MEX_H_DECLARE_FUNC( uint32,				soc_htonl,								(uint32 uHostlong) )
MEX_H_DECLARE_FUNC( uint32,				soc_ntohl,								(uint32 uNetlong) )
MEX_H_DECLARE_FUNC( uint16,				soc_htons,								(uint16 uHostshort) )
MEX_H_DECLARE_FUNC( uint16,				soc_ntohs,								(uint16 uNetshort) )

#if MEX_SDK_VER >= 1003
MEX_H_DECLARE_FUNC( int8,				soc_get_open_socket_num,				(void) )
#endif



MEX_H_ENUM_FUNC_START_SOC
	MEX_H_ENUM_FUNC_SOC( soc_create )
	MEX_H_ENUM_FUNC_SOC( soc_close )
	MEX_H_ENUM_FUNC_SOC( soc_bind )
	MEX_H_ENUM_FUNC_SOC( soc_listen )
	MEX_H_ENUM_FUNC_SOC( soc_accept )
	MEX_H_ENUM_FUNC_SOC( soc_connect )
	MEX_H_ENUM_FUNC_SOC( soc_sendto )
	MEX_H_ENUM_FUNC_SOC( soc_send )
	MEX_H_ENUM_FUNC_SOC( soc_recvfrom )
	MEX_H_ENUM_FUNC_SOC( soc_recv )
	MEX_H_ENUM_FUNC_SOC( soc_setsockopt )
	MEX_H_ENUM_FUNC_SOC( soc_getsockopt )
	MEX_H_ENUM_FUNC_SOC( soc_gethostbyname )
	MEX_H_ENUM_FUNC_SOC( soc_gethostbyname2 )
	MEX_H_ENUM_FUNC_SOC( soc_gethostbyaddr )
	MEX_H_ENUM_FUNC_SOC( soc_open_nwk_account )
	MEX_H_ENUM_FUNC_SOC( soc_close_nwk_account )
	MEX_H_ENUM_FUNC_SOC( soc_close_nwk_account_by_id )
	MEX_H_ENUM_FUNC_SOC( soc_getlocalip )
	MEX_H_ENUM_FUNC_SOC( soc_get_account_localip )
	MEX_H_ENUM_FUNC_SOC( soc_getsockaddr )
	MEX_H_ENUM_FUNC_SOC( soc_get_last_error )
	MEX_H_ENUM_FUNC_SOC( soc_set_last_error )
	MEX_H_ENUM_FUNC_SOC( soc_open_bearer )
	MEX_H_ENUM_FUNC_SOC( soc_close_bearer )
	MEX_H_ENUM_FUNC_SOC( soc_ready_for_read )
	MEX_H_ENUM_FUNC_SOC( soc_cksum )
	MEX_H_ENUM_FUNC_SOC( init_network )
	MEX_H_ENUM_FUNC_SOC( soc_get_host_by_name_event_handle )
	MEX_H_ENUM_FUNC_SOC( cancel_gethost )
	MEX_H_ENUM_FUNC_SOC( cancel_soc_notify )
	MEX_H_ENUM_FUNC_SOC( cancel_init_network )
	MEX_H_ENUM_FUNC_SOC( reg_soc_notify )
	MEX_H_ENUM_FUNC_SOC( cancel_all_soc_notify )
	MEX_H_ENUM_FUNC_SOC( close_network )
	MEX_H_ENUM_FUNC_SOC( soc_get_nw_account )
	MEX_H_ENUM_FUNC_SOC( soc_get_nw_mode )
	MEX_H_ENUM_FUNC_SOC( soc_get_nw_sim )
	MEX_H_ENUM_FUNC_SOC( soc_set_nw_mode )
	MEX_H_ENUM_FUNC_SOC( soc_set_nw_sim )
	MEX_H_ENUM_FUNC_SOC( soc_reset_nw )
	MEX_H_ENUM_FUNC_SOC( soc_htonl )
	MEX_H_ENUM_FUNC_SOC( soc_ntohl )
	MEX_H_ENUM_FUNC_SOC( soc_htons )
	MEX_H_ENUM_FUNC_SOC( soc_ntohs )
#if MEX_SDK_VER >= 1003
	MEX_H_ENUM_FUNC_SOC( soc_get_open_socket_num )
#endif

MEX_H_ENUM_FUNC_END_SOC


#if defined (MEX_TARGET_CLIENT)
	#define soc_create								MEX_H_GET_FUNC_DEFINE_SOC( soc_create )
	#define soc_close								MEX_H_GET_FUNC_DEFINE_SOC( soc_close )
	#define soc_bind								MEX_H_GET_FUNC_DEFINE_SOC( soc_bind )
	#define soc_listen								MEX_H_GET_FUNC_DEFINE_SOC( soc_listen )
	#define soc_accept								MEX_H_GET_FUNC_DEFINE_SOC( soc_accept )
	#define soc_connect								MEX_H_GET_FUNC_DEFINE_SOC( soc_connect )
	#define soc_sendto								MEX_H_GET_FUNC_DEFINE_SOC( soc_sendto )
	#define soc_send								MEX_H_GET_FUNC_DEFINE_SOC( soc_send )
	#define soc_recvfrom							MEX_H_GET_FUNC_DEFINE_SOC( soc_recvfrom )
	#define soc_recv								MEX_H_GET_FUNC_DEFINE_SOC( soc_recv )
	#define soc_setsockopt							MEX_H_GET_FUNC_DEFINE_SOC( soc_setsockopt )
	#define soc_getsockopt							MEX_H_GET_FUNC_DEFINE_SOC( soc_getsockopt )
	#define soc_gethostbyname						MEX_H_GET_FUNC_DEFINE_SOC( soc_gethostbyname )
	#define soc_gethostbyname2						MEX_H_GET_FUNC_DEFINE_SOC( soc_gethostbyname2 )
	#define soc_gethostbyaddr						MEX_H_GET_FUNC_DEFINE_SOC( soc_gethostbyaddr )
	#define soc_open_nwk_account					MEX_H_GET_FUNC_DEFINE_SOC( soc_open_nwk_account )
	#define soc_close_nwk_account					MEX_H_GET_FUNC_DEFINE_SOC( soc_close_nwk_account )
	#define soc_close_nwk_account_by_id				MEX_H_GET_FUNC_DEFINE_SOC( soc_close_nwk_account_by_id )
	#define soc_getlocalip							MEX_H_GET_FUNC_DEFINE_SOC( soc_getlocalip )
	#define soc_get_account_localip					MEX_H_GET_FUNC_DEFINE_SOC( soc_get_account_localip )
	#define soc_getsockaddr							MEX_H_GET_FUNC_DEFINE_SOC( soc_getsockaddr )
	#define soc_get_last_error						MEX_H_GET_FUNC_DEFINE_SOC( soc_get_last_error )
	#define soc_set_last_error						MEX_H_GET_FUNC_DEFINE_SOC( soc_set_last_error )
	#define soc_open_bearer							MEX_H_GET_FUNC_DEFINE_SOC( soc_open_bearer )
	#define soc_close_bearer						MEX_H_GET_FUNC_DEFINE_SOC( soc_close_bearer )
	#define soc_ready_for_read						MEX_H_GET_FUNC_DEFINE_SOC( soc_ready_for_read )
	#define soc_cksum								MEX_H_GET_FUNC_DEFINE_SOC( soc_cksum )
	#define init_network							MEX_H_GET_FUNC_DEFINE_SOC( init_network )
	#define soc_get_host_by_name_event_handle		MEX_H_GET_FUNC_DEFINE_SOC( soc_get_host_by_name_event_handle )
	#define cancel_gethost							MEX_H_GET_FUNC_DEFINE_SOC( cancel_gethost )
	#define cancel_soc_notify						MEX_H_GET_FUNC_DEFINE_SOC( cancel_soc_notify )
	#define cancel_init_network						MEX_H_GET_FUNC_DEFINE_SOC( cancel_init_network )
	#define reg_soc_notify							MEX_H_GET_FUNC_DEFINE_SOC( reg_soc_notify )
	#define cancel_all_soc_notify					MEX_H_GET_FUNC_DEFINE_SOC( cancel_all_soc_notify )
	#define close_network							MEX_H_GET_FUNC_DEFINE_SOC( close_network )
	#define soc_get_nw_account						MEX_H_GET_FUNC_DEFINE_SOC( soc_get_nw_account )
	#define soc_get_nw_mode							MEX_H_GET_FUNC_DEFINE_SOC( soc_get_nw_mode )
	#define soc_get_nw_sim 							MEX_H_GET_FUNC_DEFINE_SOC( soc_get_nw_sim  )
	#define soc_set_nw_mode							MEX_H_GET_FUNC_DEFINE_SOC( soc_set_nw_mode )
	#define soc_set_nw_sim 							MEX_H_GET_FUNC_DEFINE_SOC( soc_set_nw_sim  )
	#define soc_reset_nw 							MEX_H_GET_FUNC_DEFINE_SOC( soc_reset_nw ) 
	#define soc_htonl 								MEX_H_GET_FUNC_DEFINE_SOC( soc_htonl )
	#define soc_ntohl 								MEX_H_GET_FUNC_DEFINE_SOC( soc_ntohl )
	#define soc_htons 								MEX_H_GET_FUNC_DEFINE_SOC( soc_htons )
	#define soc_ntohs 								MEX_H_GET_FUNC_DEFINE_SOC( soc_ntohs )
	#if MEX_SDK_VER >= 1003
	#define soc_get_open_socket_num					MEX_H_GET_FUNC_DEFINE_SOC( soc_get_open_socket_num )
	#endif
	
#endif

#if defined(_MEX_COMMENTS_NEVER_DEFINE_THIS_)
	//�����Ǿ���ʹ�õ��ĺ���˵��
	//��������Э��һЩĬ�ϵĶ��� ���Բο� RFC1700 �ĵ�
	
	/**************************************************************************
	* �������ƣ� soc_create
	* ���������� ����һ��socket
	* ����˵���� [IN:domain]	�ϲ㲻��Ҫ���ĸò��� ����ʱ������д 0 ���ߺ� MEX_PF_INET ��ʵ�Ĳ����� MEX_PF_INET  MEX_PF_INET6  ����ָ��socketЭ�� ΪIPV4 IPV6 MTKĬ��Ϊipv4 
	*            [IN:type]	socket������ �ο�socket����  
	*			 [IN:protocol] Э������ �ϲ㲻��Ҫ���� MTKĬ��ΪIPЭ�� Ĭ��ֵΪ ��MEX_IPPROTO_IP ���� 0
	*			 [IN:dummy] ģ���� �ϲ㲻��Ҫ���� ��MTK�� Ĭ��ΪMMI  ����ʱ������д����ֵ
	*			 [IN:nwk_account_id] gprs������˺� ����ʱ����Ϸ����Ѿ��򿪹���gprs�˺�
	* �� �� ֵ�� [int8:��С��0]  ��ʱ˵��socket�Ѿ������ɹ� ����ֵΪsocket id Ҳ���Ǹ�socket��Ψһ��ʾ
	*            [int8:С����] ����ʧ��  ��ʱӦ��ȥ���һ�²����Ƿ����� gprs�Ƿ����ɹ� ������ �Ѿ��򿪵�socket��������ϵͳ������
	*
	* ����˵���� ��һ��socket����ʹ�õ�ʱ��һ��Ҫ�ر���
	**************************************************************************/
	int8 soc_create(uint8 domain, int8 type, uint8 protocol, uint8 dummy, uint32 nwk_account_id);

	/**************************************************************************
	* �������ƣ� soc_close
	* ���������� �ر�һ����������socket
	* ����˵���� [int8:s] ��Ҫ�رյ�socket   �ڲ�����Ҫʹ��socket������£�һ��Ҫ�ر��� 
	*           
	* �� �� ֵ�� [int8:] �رյĽ��   ����Ҫ����
	*    
	* ����˵���� ��
	**************************************************************************/
	int8 soc_close(int8 s);
	
	/**************************************************************************
	* �������ƣ� soc_connect
	* ���������� ���ӵ�������
	* ����˵���� [int8:s] socket id  ��socket�����ӵ�ָ���ķ������˿�
	*           [Mex_SockAddr_Struct*:addr] ��Ҫ���ӵķ�������Ϣ �ο��ṹ Mex_SockAddr_Struct
	* �� �� ֵ�� [int8:] ���ӵĽ��  ���Բο� ö��E_Mex_Soc_Error_Enum
	*    
	* ����˵���� ������ص���E_MSEE_SOC_SUCCESS ˵�����������˷����� 
	*			���socket����������ģʽ �ú�����һֱ�����Ϸ���������������ʧ�ܵ�ʱ��Ż��з���ֵ  Ϊ��ϵͳ�������� ������ʹ��������socketģʽ
	*			������ģʽ�� ���᷵��E_MSEE_SOC_WOULDBLOCK
	*			���ʱ�� ��Ҫ���ĵĵȴ�socket�ص����� ���س���E_MSE_SOC_CONNECT����Ϣ �������������Ϊ����ʧ��
	**************************************************************************/
	int8 soc_connect(int8 s, Mex_SockAddr_Struct *addr);
	
	/**************************************************************************
	* �������ƣ� soc_send
	* ���������� ��һ���Ѿ��ͷ��������������ӵ�socket�ܵ���д����
	* ����˵���� [int8:s]  �Ѿ��ͷ������佨�����ӵ�socket
	*           [uint8*:buf] ��Ҫд��socket�ܵ��е�����
	*			[int32:len] ���ݵĳ���
	*			[uint8 flags] ��־ һ�������Ϊ 0 			
	* �� �� ֵ�� [int8:����0] д��ɹ����ֽ��� 
	*			[int8:����0 ������ E_MSEE_SOC_WOULDBLOCK] ����ڷ�����ģʽ�µ�socket��˵����ʱsocket������ ��Ҫ�ȴ�socket�ص����� ���س���E_MSE_SOC_WRITE���ź� ����д��
	*			[int8:����] ��������� ˵�������˴��� ����ͨ��soc_get_last_error ��ȡ�������Լ�ԭ��
	*
	* ����˵���� ����ɹ���д����socket�Ĺܵ����ϲ㼴����Ϊ�˶������Ѿ��ɹ�������������������ڵ��ĸ�����flags������ͨ����������һ����ѭPOSIX�Ĳ���ϵͳ(linux,windows)���纯���ֲ����˽���
	*			
	**************************************************************************/
	int32 soc_send(int8 s, uint8 *buf, int32 len, uint8 flags); 
	

	/**************************************************************************
	* �������ƣ� soc_recv
	* ���������� ��һ���Ѿ��ͷ��������������ӵ�socket�ܵ��������
	* ����˵���� [int8:s]  �Ѿ��ͷ������佨�����ӵ�socket
	*           [uint8*:buf] ������������ݵĻ���
	*			[int32:len] ���ݵĳ���
	*			[uint8 flags] ��־ һ�������Ϊ 0 			
	* �� �� ֵ�� [int8:����0] ��ȡ�ɹ����ֽ��� 
	*			[int8:����0 ������ E_MSEE_SOC_WOULDBLOCK] ����ڷ�����ģʽ�µ�socket��˵����ʱsocket������ ��Ҫ�ȴ�socket�ص����� ���س���E_MSE_SOC_READ���ź� ���ܶ�ȡ
	*			[int8:����] ��������� ˵�������˴��� ����ͨ��soc_get_last_error ��ȡ�������Լ�ԭ��
	*
	* ����˵�����ɲο�soc_send
	*			
	**************************************************************************/
	int32 soc_recv(int8 s, uint8 *buf, int32 len, uint8 flags); 

	/**************************************************************************
	* �������ƣ� soc_setsockopt
	* ���������� ����socket������
	* ����˵���� [int8:s]  ��Ҫ���õ�socket id
	*           [uint32:option] ���õ�����  �ο�ö��E_Mex_Soc_Option  һ���� E_MEX_SOC_NBIO (������)  E_MEX_SOC_ASYNC(�첽)
	*			[void *:val] �������Ե����� �����Ҫ���� ���� ������ ֱ������0����1 ��������첽�¼� �ο�ö��E_Mex_Soc_Event    
	*			[uint8 val_size] val�ĳ��� 			
	* �� �� ֵ��[int32:Ϊ0] ���óɹ�
	*			[int32:����] ����Ϊ����ʧ��			 
	* ����˵����
	*			
	**************************************************************************/
	int8 soc_setsockopt(int8 s, uint32 option, void *val, uint8 val_size); 

	
	
	/**************************************************************************
	* �������ƣ� soc_close_nwk_account_by_id
	* ���������� �ر�ָ���������˺�
	* ����˵���� [uint8:mod_id] ģ�� �ϲ㲻��Ҫ����
	*			[uint32:account_id] �˺�ID
	* �� �� ֵ�� 	 
	* ����˵�����ú�����ֱ�ӹر�ָ����gors�˺� ����ȫ ���ܻ�Ӱ�쵽�������� ��ʹ���������colse_network
	*			
	**************************************************************************/
	void soc_close_nwk_account_by_id(uint8 mod_id, uint32 account_id);


	/**************************************************************************
	* �������ƣ� init_network
	* ���������� ��һ��gprs�˺�
	* ����˵���� [uint32 *:nwk_account_id] ָ����Ҫ�������˺ŵĵ�ַ  Ĭ�ϵ��˺����ϵͳ�����е� E_MPC_Master_NWK_ACCOUNT����E_MPC_Slave_NWK_ACCOUNT��ȡ
	*			[int8:sim_id]sim�� �ο�ö��E_MEX_Sim_Card_ID
	*			[soc_funcptr:cb]�ص����� ϵͳ��ɿ����˺�֮����������ص�֪ͨ�ɹ���ʧ��
	*			[E_Mex_Net_Mode:mode]����ģʽ �ο�ö��E_Mex_Net_Mode
	*			[int32:iTimeout]��ʱ֡��������MTK�д���Ļ��ң�����ע��Ļص��ᱻ�������򸲸ǵ��������ͻ����ʧ�ܣ����볬ʱ���ƣ���һ���ı�������
	*			[int32:app_id]���õĳ���ID 
		
	* �� �� ֵ��[int8:]�ο�ö��E_Mex_Operator_Result	 
	*			E_MOR_SUCCESS ˵����ǰ���˺Ż��ߵ�ǰsim���Ѿ�������Ӧ�ó�������gprs������ֱ������ʹ�ã�����������	* ��Ҫ����colse_network�ر�
	*			E_MOR_WAITING ˵����Ҫ�ȴ��ص�����
	*			E_MOR_MODE_ERROR ˵����ǰsim���Ѿ�������gprs�����ǵ����ߺ�Ŀǰ������ģʽ��ͬ����Ҫ�л�һ��ģʽ�����л�sim��������˵����ϵͳsim1������cmnet������
	*							��ʱ�е����ߵ���init_network()������� ��1 �� cmwap  ��ʱ�ķ���ֵ��ΪE_MOR_MODE_ERROR����Ϊ��1��������ȥ����һ��cmwap�����˺�
	*			����������Բο�ö��E_Mex_Operator_Result	��ע��
	* ����˵���������㴫����˺���ͨ�� GetPhoneConfig(E_MPC_Master_NWK_ACCOUNT),����ͨ�� GetPhoneConfig(E_MPC_Slave_NWK_ACCOUNT) ��ȡ���˺ţ������ϵͳĬ�ϵ��˺�
	*			����㴫�����������˺ţ��п��ܻᱻ�ı䣬��ע�⡣
	*			�ص�����������Ϊ void cd(void *msg) msgӦ�ñ�ǿ��תΪint32�� ��Ϊ1ʱ ��ʾ�ɹ� ��Ϊ0ʱ˵��ʧ��
	**************************************************************************/
	int8 init_network(uint32 *nwk_account_id, int8 sim_id, soc_funcptr cb, E_Mex_Net_Mode mode, int32 iTimeout, int32 app_id); 

	/**************************************************************************
	* �������ƣ� colse_network
	* �����������ر�����
	* ����˵����
	*			[uint32:nwk_account_id]���򿪵������˺�
	*			[int8:sim_id]ע�����ĸ�sim���� �ο�ö��E_MEX_Sim_Card_ID
	* �� �� ֵ��
	* ����˵�������������init_network �ҽ���ɹ��ˣ�һ��Ҫ���øú����ر����磬���ÿ��Ƕ�����app��Ӱ�죬�ײ����������֤������
	*			
	**************************************************************************/
	void close_network(uint32 nwk_account_id,int8 sim_id);

	/**************************************************************************
	* �������ƣ� soc_gethostbyname
	* ���������� ��������������������ȡ������IP
	* ����˵���� [bool:is_blocking]  �Ƿ����� һ�㶼��false ������
	*           [uint8:mod_id] ģ�� �ϲ㲻��Ҫ����
	*			[int32:request_id] �����ʾ ���������Ϊ�����Ψһ��ʾ �������첽�ص��м�ⷵ��ֵ�ǲ����Լ������
	*			[const char *:domain_name]domain ������ acsii����ʽ���ַ��� ���� "www.xxxx.com" 
	*			[uint8 *:addr] һ��������ǳ���Ϊ4������ �������IP
	*			[uint8 *:addr_len] addr�ĳ���
	*			[uint8:access_id] ��ʼ��־
	*			[uint32:nwk_account_id] �ѿ����������˺�
	* �� �� ֵ��[int8:E_MOR_SYS_BUSY] ϵͳæ ����Ϊ����������DNS����
	*			[int8:����] �����ɲο�ö��E_Mex_Soc_Error_Enum  ���� E_MSEE_SOC_WOULDBLOCKΪ���� ��ʱ����Ҫ�����첽�ص�	 soc_get_host_by_name_event_handle		 
	* ����˵����
	*			
	**************************************************************************/
	int8 soc_gethostbyname(bool is_blocking, uint8 mod_id, int32 request_id, const char *domain_name, uint8 *addr, uint8 *addr_len,\
		uint8 access_id, uint32 nwk_account_id);

	/**************************************************************************
	* �������ƣ� soc_get_host_by_name_event_handle
	* ���������� ������soc_gethostbyname����ʱ��Ҫ���õĻص�
	* ����˵���� [soc_funcptr:func] ���õĻص�  ��ʽ void cb(void *msg)   msgӦ��ǿ��ת��Ϊint8 *    ��������0 ˵��ʧ��  �����Ϊ�� ��ô�������һ��4�ֽڵ�IP��ַ
	*			[int32:iTimeout]��ʱ֡��������MTK�д���Ļ��ң�����ע��Ļص��ᱻ�������򸲸ǵ��������ͻ����ʧ�ܣ����볬ʱ���ƣ���һ���ı�������
	*			[int32:app_id]���õĳ���ID 

	* �� �� ֵ��[int8:]�ο�ö��E_Mex_Operator_Result	 
	*			E_MOR_SUCCESS ˵�����óɹ�
	*			E_MOR_SYS_BUSY ˵����ǰ������app��������ʹ�÷������Ժ�����
	*			����������Բο�ö��E_Mex_Operator_Result	��ע��
	* ����˵����
	*			
	**************************************************************************/	
	int8 soc_get_host_by_name_event_handle(soc_funcptr func, int32 iTimeout, int32 app_id)

	/**************************************************************************
	* �������ƣ� cancel_gethost
	* ���������� ȡ��soc_get_host_by_name_event_handle
	* ����˵���� 
	*			[int32:app_id]���õĳ���ID 
	* �� �� ֵ��[int8:]�ο�ö��E_Mex_Operator_Result	 
	*			E_MOR_INVALID ˵���Ƿ������������ڵ�ǰapp�Ļص���ûȨ��ȥȡ��
	*			����������Բο�ö��E_Mex_Operator_Result	��ע��
	* ����˵�������app������soc_get_host_by_name_event_handle ����û�ȵ��ص����� ��Ҫ�˳� �����ж�  ��Ҫ���ô˺��� ��Ȼ��Ӱ������appʹ��dns����
	*			
	**************************************************************************/
	int8 cancel_gethost(int32 app_id);

	/**************************************************************************
	* �������ƣ� cancel_soc_notify
	* ���������� ȡ��reg_soc_notify
	* ����˵���� [int8:socket_id] ��Ҫȡ����socket id
	*			[int32:app_id]���õĳ���ID 
	* �� �� ֵ��[int8:]�ο�ö��E_Mex_Operator_Result	 
	*			E_MOR_INVALID ˵���Ƿ������������ڵ�ǰapp�Ļص���ûȨ��ȥȡ��
	*			����������Բο�ö��E_Mex_Operator_Result	��ע��
	* ����˵�������app������reg_soc_notify ��������Ҫsocket�첽�����ʱ��Ӧ�õ��øú���ȡ��ע�ᣬ��Ȼ������ڴ�й©���Լ�Ӱ��ϵͳЧ��
	*			
	**************************************************************************/
	int8 cancel_soc_notify(int8 socket_id, int32 app_id); 

	/**************************************************************************
	* �������ƣ� reg_soc_notify
	* ���������� ����socket�첽ʱ��
	* ����˵���� [int8:socket_id]���õ�socket id
	*			[soc_funcptr:cb]�첽�ص�����  ÿ��ϵͳ��Ҫ֪ͨsocket���첽�¼� ������øú��� 
	*			[int32:app_id]���õĳ���ID 
	* �� �� ֵ��[int8:]�ο�ö��E_Mex_Operator_Result	 
	*			E_MOR_ALREADY_REG ˵����ǰsocket�Ѿ���ע�� ��ʱ�ص������ᱻ�滻Ϊ�µĻص�����
	*			E_MOR_INVALID ˵����ǰsocket�Ѿ�������Ӧ�ó���ע�ᣬ
	*			����������Բο�ö��E_Mex_Operator_Result	��ע��
	* ����˵����
	*			
	**************************************************************************/
	int8 reg_soc_notify(int8 socket_id, soc_funcptr cb, int32 app_id);

	/**************************************************************************
	* �������ƣ� cancel_all_soc_notify
	* ���������� ȡ����ǰapp�����е�reg_soc_notify
	* ����˵����
	*		    [int32:app_id]���õĳ���ID 
	* �� �� ֵ��[int8:]��Զ���ǳɹ�
	* ����˵�����ú���Ϊȡ����ǰapp�µ�����ע�����socket�첽�ص�
	*			
	**************************************************************************/
	int8 cancel_all_soc_notify(int32 app_id);


	/************************************************************************
	* �������ƣ� soc_get_nw_account
	* ���������� ��ȡϵͳ�Ѿ��򿪵�����ID
	* ����˵���� 
	* �� �� ֵ��[int32:]��С��0 Ϊ�Ѵ򿪵�����id  С��0Ϊϵͳע������idʧ��
	*			
	* ����˵����ϵͳ���ڳ�ʼ����ʱ��������ÿ���һ������id��ͨ���ú������Ի�ȡ�Ѿ��򿪵�
	*		    ����id��������idС��0��˵��ϵͳ��������ʧ��
	************************************************************************/
	int32 soc_get_nw_account(void);

	/************************************************************************
	* �������ƣ� soc_get_nw_mode
	* ���������� ��ȡϵͳ��ǰ�����������
	* ����˵���� 
	* �� �� ֵ��[E_Mex_Net_Mode:]�ο�ö��E_Mex_Net_Mode
	*			
	* ����˵�������ͨ��soc_get_nw_account��ȡ�˺ųɹ����ú����ķ���ֵ����Ч
	*		   
	************************************************************************/
	E_Mex_Net_Mode soc_get_nw_mode(void);

	/************************************************************************
	* �������ƣ� soc_get_nw_sim
	* ���������� ��ȡϵͳ��ǰ������ʹ�õ�sim��
	* ����˵���� 
	* �� �� ֵ��[E_MEX_Sim_Card_ID:]�ο�ö��E_MEX_Sim_Card_ID
	*			
	* ����˵�������ͨ��soc_get_nw_account��ȡ�˺ųɹ����ú����ķ���ֵ����Ч
	*
	************************************************************************/	
	int8 soc_get_nw_sim (void);

	/************************************************************************
	* �������ƣ� soc_set_nw_mode
	* ���������� ����ϵͳ����򿪵�����
	* ����˵����[E_Mex_Net_Mode] �ο�ö��E_Mex_Net_Mode
	* �� �� ֵ��
	*			
	* ����˵�������Ĺ�ϵͳ�������ͺ��뼰ʱ����soc_reset_nw���´����磬�����Ӱ��soc_get_nw_mode�Ľ��
	*
	************************************************************************/
	void soc_set_nw_mode (E_Mex_Net_Mode mode);

	/************************************************************************
	* �������ƣ� soc_set_nw_sim
	* ���������� ����ϵͳ������ʹ�õ�sim��
	* ����˵���� [E_MEX_Sim_Card_ID] �ο�ö��E_MEX_Sim_Card_ID
	* �� �� ֵ��
	*			
	* ����˵�������Ĺ�ϵͳsim�����뼰ʱ����soc_reset_nw���´����磬�����Ӱ��soc_get_nw_sim�Ľ��
	*
	************************************************************************/
	void soc_set_nw_sim (int8 sim);

	/************************************************************************
	* �������ƣ� soc_reset_nw
	* ���������� ���¿�������
	* ����˵���� 
	* �� �� ֵ��
	*			
	* ����˵�����ú�����رյ�ǰ�Ѵ򿪵����磬Ȼ����� soc_set_nw_mode ��soc_set_nw_sim���������´�����,
	* �����������ʹ�������Ӧ�ó������Ӱ��
	*
	************************************************************************/
	void soc_reset_nw (void);

	/************************************************************************
	* �������ƣ� soc_bind
	* ���������� Ϊsocket��һ��IP��ַ�Ͷ˿�
	* ����˵����[int8 s]�Ѵ�����socket
			   [Mex_SockAddr_Struct *addr] �󶨵��ĵ�ַ
	* �� �� ֵ��0Ϊ�ɹ�
	*			
	************************************************************************/
	int8 soc_bind(int8 s, Mex_SockAddr_Struct *addr);

	/************************************************************************
	* �������ƣ� soc_sendto
	* ���������� ��ĳ����ַ����һ�����ݰ�
	* ����˵����[int8 s]�Ѵ�����socket
				[uint8 *buf]���ݰ�
				[int32 len] ���ݳ���
				[Mex_SockAddr_Struct *addr] ���͵��ĵ�ַ���˿�
	* �� �� ֵ�����ͽ��   С��0Ϊʧ��������  ����0Ϊ���͵ĳ���
	*			
	************************************************************************/
	int32 soc_sendto(int8 s, uint8 *buf, int32 len, uint8 flags, Mex_SockAddr_Struct *addr);

	/************************************************************************
	* �������ƣ� soc_recvfrom
	* ���������� ��ÿ����ַ����һ�����ݰ�
	* ����˵����[int8 s]�Ѵ�����socket
				[uint8 *buf]���ݰ�������
				[int32 len] ����������
				[Mex_SockAddr_Struct *addr] ���ͷ��ĵ�ַ���˿�
	* �� �� ֵ�����ͽ��   С��0Ϊʧ��������  ����0Ϊ���յĳ���
	*			
	************************************************************************/
	int32 soc_recvfrom(int8 s, uint8 *buf, int32 len, uint8 flags, Mex_SockAddr_Struct *fromaddr);

	/************************************************************************
	* �������ƣ� soc_htonl
	* ���������� ��һ��32λ���������ֽ�˳��ת���������ֽ�˳��
	*			
	************************************************************************/
	uint32 soc_htonl(uint32 uHostlong);

	/************************************************************************
	* �������ƣ� soc_ntohl
	* ���������� ��һ��32λ���������ֽ�˳��ת���������ֽ�˳��
	*			
	************************************************************************/
	uint32 soc_ntohl(uint32 uNetlong);

	/************************************************************************
	* �������ƣ� soc_htons
	* ���������� ��һ��16λ���������ֽ�˳��ת���������ֽ�˳��
	*			
	************************************************************************/
	uint16 soc_htonl(uint16 uHostshort);

	/************************************************************************
	* �������ƣ� soc_ntohs
	* ���������� ��һ��16λ���������ֽ�˳��ת���������ֽ�˳��
	*			
	************************************************************************/
	uint16 soc_ntohs(uint16 uNetshort);

	/************************************************************************
	* �������ƣ� soc_get_open_socket_num
	* ���������� ��ȡ��ǰ�Ѵ򿪵�socket������
	* ͨ������GetPhoneConfig(E_MPC_MaxSocNum) ���Ի�ȡϵͳ֧�ֵ����ͬʱ�򿪵�socket��
	* ���򿪵�socket�����Ѿ�������������ֵʱ��socket��������ʧ��
	*			
	************************************************************************/
	int8 soc_get_open_socket_num(void);

	/************************************************************************/
	/* ���������õĺ��� ���Բο�MTK�еĳ���                                   */
	/************************************************************************/
	/************************************************************************/
	/* ���鲻Ҫ�ڻص������ܶ�������ص������Ĳ���������ʱ�ģ��뿪�˺����Ͳ�������*/
	/* ��Ҫ�ڻص���������Ƕ�׵Ĳ������������߼��ϵ���ѭ��						*/
	/* ������Ϣ ��ο����� Soc_Test.c											*/
	/************************************************************************/


#endif

#endif
