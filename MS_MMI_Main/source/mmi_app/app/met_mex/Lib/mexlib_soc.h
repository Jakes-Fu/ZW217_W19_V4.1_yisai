/************************************************************************
* 版权所有 (C)2010,扬讯科技。
* 
* 文件名称： // MexLib_Soc.h
* 文件标识：
* 内容摘要： // mex平台soc库函数头文件
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
#define GPRS_CMNET_ACCOUNT_NAME			"MEX_NET"	//cmnet账号APN名称
#define GPRS_CMWAP_ACCOUNT_NAME			"MEX_WAP"	//cmwap账号APN名称
#else
#define Convert ConvertSysDef
#endif



/************************************************************************/
/* socket类型                                                           */
/************************************************************************/


#if defined (MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
/* stream socket, TCP */   //这个协议是按照顺序的、可靠的、数据完整的基于字节流的连接。这是一个使用最多的socket类型，这个socket是使用TCP来进行传输。
#define MEX_STREAM_SOCKET	( Convert(E_M2MDC_SOCK_STREAM) )		
/* datagram socket, UDP */ //这个协议是无连接的、固定长度的传输调用。该协议是不可靠的，使用UDP来进行它的连接。
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
//	E_MST_STREAM = 0,		/* stream socket, TCP */   //这个协议是按照顺序的、可靠的、数据完整的基于字节流的连接。这是一个使用最多的socket类型，这个socket是使用TCP来进行传输。
//	E_MST_DGRAM,			/* datagram socket, UDP */ //这个协议是无连接的、固定长度的传输调用。该协议是不可靠的，使用UDP来进行它的连接。
//	E_MST_SMS,				/* SMS bearer */
//	E_MST_RAW				/* raw socket */
//} int8;		// == socket_type_enum

#define MEX_MAX_SOCK_ADDR_LEN           (28)	//MAX_SOCK_ADDR_LEN

typedef struct
{
	int32				sock_type;				//socket类型 
	int16				addr_len;				//地址长度  也是IP地址的字节数  IPV4中的ip地址是 4个字节  32位的 
	uint16				port;					//端口
	uint8				addr[MEX_MAX_SOCK_ADDR_LEN]; //IP地址   不同平台应该注意大小端问题
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
	E_MOR_SUCCESS		= 1,			//成功
	E_MOR_WAITING		= 0,			//需要等待回调
	E_MOR_FAILED		= -1,			//普通错误   如果经常出现这种错误 应该检查一下程序逻辑是否合理
	E_MOR_SIM_ERROR		= -2,			//sim卡错误 一般情况下就是sim卡无效
	E_MOR_MEM_ERROR		= -3,			//内存错误	一般情况下为申请内存失败
	E_MOR_SYS_BUSY		= -4,			//系统忙		一般情况下 是其他应用程序正在调用
	E_MOR_INVALID		= -5,			//不合法的   这种操作会干扰到其他应用程序 
	E_MOR_OVER_LIMIT	= -6,			//socket数超过了系统支持的最大值，可能是其他app没有释放自己的socket
	E_MOR_ALREADY_REG	= -7,			//已经被注册的socket id
	E_MOR_MODE_ERROR	= -8,			//开启网络的模式错误 
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
#define MEX_SOC_NWPAUSE			(-4)					//网络暂时挂起
#define MEX_SOC_NWRESUME		(-3)					//网络恢复
#define MEX_SOC_NWCLOSED		(-2)					//网络被强制关闭
#define MEX_SOC_READ			( Convert(E_M2MDC_SOC_READ) )
#define MEX_SOC_WRITE			( Convert(E_M2MDC_SOC_WRITE) )
#define MEX_SOC_ACCEPT			( Convert(E_M2MDC_SOC_ACCEPT) )
#define MEX_SOC_CONNECT			( Convert(E_M2MDC_SOC_CONNECT) )
#define MEX_SOC_CLOSE			( Convert(E_M2MDC_SOC_CLOSE) )

#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)

#define MEX_SOC_NWPAUSE			(-4)					//网络暂时挂起
#define MEX_SOC_NWRESUME		(-3)					//网络恢复
#define MEX_SOC_NWCLOSED		(-2)					//网络被强制关闭

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

//当前设置
typedef struct 
{
	int8 nw_sim;					//系统当前联网sim卡
	int8 nw_mode;					//系统网络类型
	uint32 nw_account;				//网络账号
	bool bInit;						//是否已被初始化
}Mex_NetWork_Setting;






/************************************************************************/
/* socket error code                                                    */
/************************************************************************/
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	#define MEX_SOC_SUCCESS				( Convert(E_M2MDC_SOC_SUCCESS) )				//操作成功
	#define MEX_SOC_ERROR				( Convert(E_M2MDC_SOC_ERROR) )				//操作失败
	#define MEX_SOC_WOULDBLOCK			( Convert(E_M2MDC_SOC_WOULDBLOCK) )			//阻塞
	#define MEX_SOC_LIMIT_RESOURCE		( Convert(E_M2MDC_SOC_LIMIT_RESOURCE) )		//受限的资源
	#define MEX_SOC_INVALID_SOCKET		( Convert(E_M2MDC_SOC_INVALID_SOCKET) )		//非法的socket
	#define MEX_SOC_INVALID_ACCOUNT		( Convert(E_M2MDC_SOC_INVALID_ACCOUNT) )		//非法的联网账号
	#define MEX_SOC_NAMETOOLONG			( Convert(E_M2MDC_SOC_NAMETOOLONG) )			//地址过长
	#define MEX_SOC_ALREADY				( Convert(E_M2MDC_SOC_ALREADY) )				//操作已经在执行
	#define MEX_SOC_OPNOTSUPP			( Convert(E_M2MDC_SOC_OPNOTSUPP) )			//不支持该操作
	#define MEX_SOC_CONNABORTED			( Convert(E_M2MDC_SOC_CONNABORTED) )			//软件照成的连接中断			
	#define MEX_SOC_INVAL				( Convert(E_M2MDC_SOC_INVAL) )				//非法的参数
	#define MEX_SOC_PIPE				( Convert(E_M2MDC_SOC_PIPE) )					//管道已损坏
	#define MEX_SOC_NOTCONN				( Convert(E_M2MDC_SOC_NOTCONN) )				//未连接的socket
	#define MEX_SOC_MSGSIZE				( Convert(E_M2MDC_SOC_MSGSIZE) )				//消息过长
	#define MEX_SOC_BEARER_FAIL			( Convert(E_M2MDC_SOC_BEARER_FAIL) )			//发送者已损坏
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
	#define MEX_SOC_SUCCESS				( Convert(E_S2MDC_SOC_SUCCESS) )				//操作成功
	#define MEX_SOC_ERROR				( Convert(E_S2MDC_SOC_ERROR) )					//操作失败
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
	//以下是经常使用到的函数说明
	//关于网络协议一些默认的定义 可以参考 RFC1700 文档
	
	/**************************************************************************
	* 函数名称： soc_create
	* 功能描述： 创建一个socket
	* 参数说明： [IN:domain]	上层不需要关心该参数 调用时可以填写 0 或者宏 MEX_PF_INET 真实的参数是 MEX_PF_INET  MEX_PF_INET6  就是指定socket协议 为IPV4 IPV6 MTK默认为ipv4 
	*            [IN:type]	socket的类型 参考socket类型  
	*			 [IN:protocol] 协议类型 上层不需要关心 MTK默认为IP协议 默认值为 宏MEX_IPPROTO_IP 或者 0
	*			 [IN:dummy] 模块名 上层不需要关心 在MTK中 默认为MMI  调用时可以填写任意值
	*			 [IN:nwk_account_id] gprs接入点账号 调用时填入合法的已经打开过的gprs账号
	* 返 回 值： [int8:不小于0]  此时说明socket已经创建成功 返回值为socket id 也就是该socket的唯一标示
	*            [int8:小于零] 创建失败  此时应该去检测一下参数是否有误 gprs是否开启成功 或者是 已经打开的socket数超过了系统的限制
	*
	* 其它说明： 当一个socket不再使用的时候，一定要关闭它
	**************************************************************************/
	int8 soc_create(uint8 domain, int8 type, uint8 protocol, uint8 dummy, uint32 nwk_account_id);

	/**************************************************************************
	* 函数名称： soc_close
	* 功能描述： 关闭一个创建过的socket
	* 参数说明： [int8:s] 需要关闭的socket   在不再需要使用socket的情况下，一定要关闭它 
	*           
	* 返 回 值： [int8:] 关闭的结果   不需要关心
	*    
	* 其它说明： 无
	**************************************************************************/
	int8 soc_close(int8 s);
	
	/**************************************************************************
	* 函数名称： soc_connect
	* 功能描述： 连接到服务器
	* 参数说明： [int8:s] socket id  该socket会连接到指定的服务器端口
	*           [Mex_SockAddr_Struct*:addr] 需要连接的服务器信息 参考结构 Mex_SockAddr_Struct
	* 返 回 值： [int8:] 连接的结果  可以参考 枚举E_Mex_Soc_Error_Enum
	*    
	* 其它说明： 如果返回的是E_MSEE_SOC_SUCCESS 说明立即连上了服务器 
	*			如果socket采用了阻塞模式 该函数会一直到连上服务器或者是连接失败的时候才会有返回值  为了系统的流畅性 不建议使用阻塞的socket模式
	*			非阻塞模式下 都会返回E_MSEE_SOC_WOULDBLOCK
	*			这个时候 需要耐心的等待socket回调函数 返回出来E_MSE_SOC_CONNECT的信息 其他情况均可认为连接失败
	**************************************************************************/
	int8 soc_connect(int8 s, Mex_SockAddr_Struct *addr);
	
	/**************************************************************************
	* 函数名称： soc_send
	* 功能描述： 向一个已经和服务器建立好连接的socket管道里写数据
	* 参数说明： [int8:s]  已经和服务器间建好连接的socket
	*           [uint8*:buf] 需要写入socket管道中的数据
	*			[int32:len] 数据的长度
	*			[uint8 flags] 标志 一般情况下为 0 			
	* 返 回 值： [int8:大于0] 写入成功的字节数 
	*			[int8:等于0 或者是 E_MSEE_SOC_WOULDBLOCK] 如果在非阻塞模式下的socket，说明此时socket被阻塞 需要等待socket回调函数 返回出来E_MSE_SOC_WRITE的信号 才能写入
	*			[int8:其他] 其他情况下 说明发生了错误 可以通过soc_get_last_error 获取错误码以及原因
	*
	* 其它说明： 如果成功的写入了socket的管道，上层即可认为此段数据已经成功发往服务器，另外关于第四个参数flags，可以通过查阅任意一个遵循POSIX的操作系统(linux,windows)网络函数手册来了解它
	*			
	**************************************************************************/
	int32 soc_send(int8 s, uint8 *buf, int32 len, uint8 flags); 
	

	/**************************************************************************
	* 函数名称： soc_recv
	* 功能描述： 从一个已经和服务器建立好连接的socket管道里读数据
	* 参数说明： [int8:s]  已经和服务器间建好连接的socket
	*           [uint8*:buf] 保存读到的数据的缓存
	*			[int32:len] 数据的长度
	*			[uint8 flags] 标志 一般情况下为 0 			
	* 返 回 值： [int8:大于0] 读取成功的字节数 
	*			[int8:等于0 或者是 E_MSEE_SOC_WOULDBLOCK] 如果在非阻塞模式下的socket，说明此时socket被阻塞 需要等待socket回调函数 返回出来E_MSE_SOC_READ的信号 才能读取
	*			[int8:其他] 其他情况下 说明发生了错误 可以通过soc_get_last_error 获取错误码以及原因
	*
	* 其它说明：可参考soc_send
	*			
	**************************************************************************/
	int32 soc_recv(int8 s, uint8 *buf, int32 len, uint8 flags); 

	/**************************************************************************
	* 函数名称： soc_setsockopt
	* 功能描述： 设置socket的属性
	* 参数说明： [int8:s]  需要设置的socket id
	*           [uint32:option] 设置的属性  参考枚举E_Mex_Soc_Option  一般是 E_MEX_SOC_NBIO (非阻塞)  E_MEX_SOC_ASYNC(异步)
	*			[void *:val] 设置属性的内容 如果需要设置 阻塞 非阻塞 直接填入0或者1 如果设置异步事件 参考枚举E_Mex_Soc_Event    
	*			[uint8 val_size] val的长度 			
	* 返 回 值：[int32:为0] 设置成功
	*			[int32:其他] 其他为设置失败			 
	* 其它说明：
	*			
	**************************************************************************/
	int8 soc_setsockopt(int8 s, uint32 option, void *val, uint8 val_size); 

	
	
	/**************************************************************************
	* 函数名称： soc_close_nwk_account_by_id
	* 功能描述： 关闭指定的网络账号
	* 参数说明： [uint8:mod_id] 模块 上层不需要关心
	*			[uint32:account_id] 账号ID
	* 返 回 值： 	 
	* 其它说明：该函数会直接关闭指定的gors账号 不安全 可能会影响到其他程序 请使用替代函数colse_network
	*			
	**************************************************************************/
	void soc_close_nwk_account_by_id(uint8 mod_id, uint32 account_id);


	/**************************************************************************
	* 函数名称： init_network
	* 功能描述： 打开一个gprs账号
	* 参数说明： [uint32 *:nwk_account_id] 指向需要开启的账号的地址  默认的账号请从系统配置中的 E_MPC_Master_NWK_ACCOUNT或者E_MPC_Slave_NWK_ACCOUNT获取
	*			[int8:sim_id]sim卡 参考枚举E_MEX_Sim_Card_ID
	*			[soc_funcptr:cb]回调函数 系统完成开启账号之后会调用这个回调通知成功或失败
	*			[E_Mex_Net_Mode:mode]网络模式 参考枚举E_Mex_Net_Mode
	*			[int32:iTimeout]超时帧数：用于MTK中代码的混乱，可能注册的回调会被其他程序覆盖掉，这样就会造成失败，引入超时机制，起到一定的保护作用
	*			[int32:app_id]调用的程序ID 
		
	* 返 回 值：[int8:]参考枚举E_Mex_Operator_Result	 
	*			E_MOR_SUCCESS 说明当前的账号或者当前sim卡已经被其他应用程序开启了gprs，可以直接拿来使用，但是用完后必	* 须要调用colse_network关闭
	*			E_MOR_WAITING 说明需要等待回调函数
	*			E_MOR_MODE_ERROR 说明当前sim卡已经开启了gprs，但是调用者和目前的网络模式不同，需要切换一下模式或者切换sim卡，比如说，当系统sim1正在用cmnet联网，
	*							此时有调用者调用init_network()传入参数 卡1 和 cmwap  此时的返回值就为E_MOR_MODE_ERROR，因为卡1不可能再去创建一个cmwap联网账号
	*			其他错误可以参考枚举E_Mex_Operator_Result	的注释
	* 其它说明：建议你传入的账号是通过 GetPhoneConfig(E_MPC_Master_NWK_ACCOUNT),或者通过 GetPhoneConfig(E_MPC_Slave_NWK_ACCOUNT) 获取的账号，这个是系统默认的账号
	*			如果你传入了其他的账号，有可能会被改变，请注意。
	*			回调函数的类型为 void cd(void *msg) msg应该被强制转为int32型 当为1时 表示成功 当为0时说明失败
	**************************************************************************/
	int8 init_network(uint32 *nwk_account_id, int8 sim_id, soc_funcptr cb, E_Mex_Net_Mode mode, int32 iTimeout, int32 app_id); 

	/**************************************************************************
	* 函数名称： colse_network
	* 功能描述：关闭网络
	* 参数说明：
	*			[uint32:nwk_account_id]被打开的网络账号
	*			[int8:sim_id]注册在哪个sim卡上 参考枚举E_MEX_Sim_Card_ID
	* 返 回 值：
	* 其它说明：如果调用了init_network 且结果成功了，一定要调用该函数关闭网络，不用考虑对其他app的影响，底层会做处理，保证不掉线
	*			
	**************************************************************************/
	void close_network(uint32 nwk_account_id,int8 sim_id);

	/**************************************************************************
	* 函数名称： soc_gethostbyname
	* 功能描述： 根据域名或者主机名获取服务器IP
	* 参数说明： [bool:is_blocking]  是否阻塞 一般都是false 非阻塞
	*           [uint8:mod_id] 模块 上层不需要关心
	*			[int32:request_id] 请求标示 即这个变量为请求的唯一表示 可以在异步回调中检测返回值是不是自己请求的
	*			[const char *:domain_name]domain 主机名 acsii码形式的字符串 形如 "www.xxxx.com" 
	*			[uint8 *:addr] 一般情况下是长度为4的数组 用来存放IP
	*			[uint8 *:addr_len] addr的长度
	*			[uint8:access_id] 初始标志
	*			[uint32:nwk_account_id] 已开启的网络账号
	* 返 回 值：[int8:E_MOR_SYS_BUSY] 系统忙 正在为其他程序做DNS服务
	*			[int8:其他] 其他可参考枚举E_Mex_Soc_Error_Enum  其中 E_MSEE_SOC_WOULDBLOCK为阻塞 此时你需要设置异步回调	 soc_get_host_by_name_event_handle		 
	* 其它说明：
	*			
	**************************************************************************/
	int8 soc_gethostbyname(bool is_blocking, uint8 mod_id, int32 request_id, const char *domain_name, uint8 *addr, uint8 *addr_len,\
		uint8 access_id, uint32 nwk_account_id);

	/**************************************************************************
	* 函数名称： soc_get_host_by_name_event_handle
	* 功能描述： 当调用soc_gethostbyname阻塞时需要设置的回调
	* 参数说明： [soc_funcptr:func] 设置的回调  形式 void cb(void *msg)   msg应该强制转换为int8 *    如果结果是0 说明失败  如果不为零 那么结果就是一个4字节的IP地址
	*			[int32:iTimeout]超时帧数：用于MTK中代码的混乱，可能注册的回调会被其他程序覆盖掉，这样就会造成失败，引入超时机制，起到一定的保护作用
	*			[int32:app_id]调用的程序ID 

	* 返 回 值：[int8:]参考枚举E_Mex_Operator_Result	 
	*			E_MOR_SUCCESS 说明设置成功
	*			E_MOR_SYS_BUSY 说明当前其他的app程序正在使用服务，请稍后再试
	*			其他错误可以参考枚举E_Mex_Operator_Result	的注释
	* 其它说明：
	*			
	**************************************************************************/	
	int8 soc_get_host_by_name_event_handle(soc_funcptr func, int32 iTimeout, int32 app_id)

	/**************************************************************************
	* 函数名称： cancel_gethost
	* 功能描述： 取消soc_get_host_by_name_event_handle
	* 参数说明： 
	*			[int32:app_id]调用的程序ID 
	* 返 回 值：[int8:]参考枚举E_Mex_Operator_Result	 
	*			E_MOR_INVALID 说明非法操作，不属于当前app的回调，没权限去取消
	*			其他错误可以参考枚举E_Mex_Operator_Result	的注释
	* 其它说明：如果app设置完soc_get_host_by_name_event_handle 但是没等到回调函数 需要退出 或者中断  需要调用此函数 不然会影响其他app使用dns服务
	*			
	**************************************************************************/
	int8 cancel_gethost(int32 app_id);

	/**************************************************************************
	* 函数名称： cancel_soc_notify
	* 功能描述： 取消reg_soc_notify
	* 参数说明： [int8:socket_id] 需要取消的socket id
	*			[int32:app_id]调用的程序ID 
	* 返 回 值：[int8:]参考枚举E_Mex_Operator_Result	 
	*			E_MOR_INVALID 说明非法操作，不属于当前app的回调，没权限去取消
	*			其他错误可以参考枚举E_Mex_Operator_Result	的注释
	* 其它说明：如果app设置完reg_soc_notify 当不再需要socket异步处理的时候应该调用该函数取消注册，不然会造成内存泄漏，以及影响系统效率
	*			
	**************************************************************************/
	int8 cancel_soc_notify(int8 socket_id, int32 app_id); 

	/**************************************************************************
	* 函数名称： reg_soc_notify
	* 功能描述： 设置socket异步时间
	* 参数说明： [int8:socket_id]设置的socket id
	*			[soc_funcptr:cb]异步回调函数  每当系统需要通知socket的异步事件 都会调用该函数 
	*			[int32:app_id]调用的程序ID 
	* 返 回 值：[int8:]参考枚举E_Mex_Operator_Result	 
	*			E_MOR_ALREADY_REG 说明当前socket已经被注册 此时回调函数会被替换为新的回调函数
	*			E_MOR_INVALID 说明当前socket已经被其他应用程序注册，
	*			其他错误可以参考枚举E_Mex_Operator_Result	的注释
	* 其它说明：
	*			
	**************************************************************************/
	int8 reg_soc_notify(int8 socket_id, soc_funcptr cb, int32 app_id);

	/**************************************************************************
	* 函数名称： cancel_all_soc_notify
	* 功能描述： 取消当前app下所有的reg_soc_notify
	* 参数说明：
	*		    [int32:app_id]调用的程序ID 
	* 返 回 值：[int8:]永远都是成功
	* 其它说明：该函数为取消当前app下的所有注册过的socket异步回调
	*			
	**************************************************************************/
	int8 cancel_all_soc_notify(int32 app_id);


	/************************************************************************
	* 函数名称： soc_get_nw_account
	* 功能描述： 获取系统已经打开的网络ID
	* 参数说明： 
	* 返 回 值：[int32:]不小于0 为已打开的网络id  小于0为系统注册网络id失败
	*			
	* 其它说明：系统会在初始化的时候根据配置开启一个网络id，通过该函数可以获取已经打开的
	*		    网络id，如果这个id小于0，说明系统开启网络失败
	************************************************************************/
	int32 soc_get_nw_account(void);

	/************************************************************************
	* 函数名称： soc_get_nw_mode
	* 功能描述： 获取系统当前打开网络的类型
	* 参数说明： 
	* 返 回 值：[E_Mex_Net_Mode:]参考枚举E_Mex_Net_Mode
	*			
	* 其它说明：如果通过soc_get_nw_account获取账号成功，该函数的返回值才有效
	*		   
	************************************************************************/
	E_Mex_Net_Mode soc_get_nw_mode(void);

	/************************************************************************
	* 函数名称： soc_get_nw_sim
	* 功能描述： 获取系统当前打开网络使用的sim卡
	* 参数说明： 
	* 返 回 值：[E_MEX_Sim_Card_ID:]参考枚举E_MEX_Sim_Card_ID
	*			
	* 其它说明：如果通过soc_get_nw_account获取账号成功，该函数的返回值才有效
	*
	************************************************************************/	
	int8 soc_get_nw_sim (void);

	/************************************************************************
	* 函数名称： soc_set_nw_mode
	* 功能描述： 设置系统网络打开的类型
	* 参数说明：[E_Mex_Net_Mode] 参考枚举E_Mex_Net_Mode
	* 返 回 值：
	*			
	* 其它说明：更改过系统网络类型后，请及时调用soc_reset_nw重新打开网络，否则会影响soc_get_nw_mode的结果
	*
	************************************************************************/
	void soc_set_nw_mode (E_Mex_Net_Mode mode);

	/************************************************************************
	* 函数名称： soc_set_nw_sim
	* 功能描述： 设置系统打开网络使用的sim卡
	* 参数说明： [E_MEX_Sim_Card_ID] 参考枚举E_MEX_Sim_Card_ID
	* 返 回 值：
	*			
	* 其它说明：更改过系统sim卡后，请及时调用soc_reset_nw重新打开网络，否则会影响soc_get_nw_sim的结果
	*
	************************************************************************/
	void soc_set_nw_sim (int8 sim);

	/************************************************************************
	* 函数名称： soc_reset_nw
	* 功能描述： 重新开启网络
	* 参数说明： 
	* 返 回 值：
	*			
	* 其它说明：该函数会关闭当前已打开的网络，然后根据 soc_set_nw_mode 和soc_set_nw_sim的设置重新打开网络,
	* 会对其他正在使用网络的应用程序造成影响
	*
	************************************************************************/
	void soc_reset_nw (void);

	/************************************************************************
	* 函数名称： soc_bind
	* 功能描述： 为socket绑定一个IP地址和端口
	* 参数说明：[int8 s]已创建的socket
			   [Mex_SockAddr_Struct *addr] 绑定到的地址
	* 返 回 值：0为成功
	*			
	************************************************************************/
	int8 soc_bind(int8 s, Mex_SockAddr_Struct *addr);

	/************************************************************************
	* 函数名称： soc_sendto
	* 功能描述： 往某个地址发送一个数据包
	* 参数说明：[int8 s]已创建的socket
				[uint8 *buf]数据包
				[int32 len] 数据长度
				[Mex_SockAddr_Struct *addr] 发送到的地址及端口
	* 返 回 值：发送结果   小于0为失败阻塞等  大于0为发送的长度
	*			
	************************************************************************/
	int32 soc_sendto(int8 s, uint8 *buf, int32 len, uint8 flags, Mex_SockAddr_Struct *addr);

	/************************************************************************
	* 函数名称： soc_recvfrom
	* 功能描述： 从每个地址接收一个数据包
	* 参数说明：[int8 s]已创建的socket
				[uint8 *buf]数据包缓冲区
				[int32 len] 缓冲区长度
				[Mex_SockAddr_Struct *addr] 发送方的地址及端口
	* 返 回 值：发送结果   小于0为失败阻塞等  大于0为接收的长度
	*			
	************************************************************************/
	int32 soc_recvfrom(int8 s, uint8 *buf, int32 len, uint8 flags, Mex_SockAddr_Struct *fromaddr);

	/************************************************************************
	* 函数名称： soc_htonl
	* 功能描述： 将一个32位数从主机字节顺序转换成网络字节顺序。
	*			
	************************************************************************/
	uint32 soc_htonl(uint32 uHostlong);

	/************************************************************************
	* 函数名称： soc_ntohl
	* 功能描述： 将一个32位数从网络字节顺序转换成主机字节顺序。
	*			
	************************************************************************/
	uint32 soc_ntohl(uint32 uNetlong);

	/************************************************************************
	* 函数名称： soc_htons
	* 功能描述： 将一个16位数从主机字节顺序转换成网络字节顺序。
	*			
	************************************************************************/
	uint16 soc_htonl(uint16 uHostshort);

	/************************************************************************
	* 函数名称： soc_ntohs
	* 功能描述： 将一个16位数从网络字节顺序转换成主机字节顺序。
	*			
	************************************************************************/
	uint16 soc_ntohs(uint16 uNetshort);

	/************************************************************************
	* 函数名称： soc_get_open_socket_num
	* 功能描述： 获取当前已打开的socket的数，
	* 通过调用GetPhoneConfig(E_MPC_MaxSocNum) 可以获取系统支持的最大同时打开的socket数
	* 当打开的socket的数已经超过或等于最大值时，socket创建将会失败
	*			
	************************************************************************/
	int8 soc_get_open_socket_num(void);

	/************************************************************************/
	/* 其他不常用的函数 可以参考MTK中的程序                                   */
	/************************************************************************/
	/************************************************************************/
	/* 建议不要在回调中做很多操作，回调函数的参数都是临时的，离开了函数就不存在了*/
	/* 不要在回调函数中做嵌套的操作，会引起逻辑上的死循环						*/
	/* 更过信息 请参考程序 Soc_Test.c											*/
	/************************************************************************/


#endif

#endif
