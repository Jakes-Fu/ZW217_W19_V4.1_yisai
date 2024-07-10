/*
*
* 版权(c) 2015 红石阳光（北京）科技有限公司
* 版权(c) 2011-2015 红石阳光（北京）科技有限公司版权所有
* 
* 本文档包含信息是私密的且为红石阳光私有，且在民法和刑法中定义为商业秘密信息。
* 任何人士不得擅自复印，扫描或以其他任何方式进行传播，否则红石阳光有权追究法律责任。
* 阅读和使用本资料必须获得相应的书面授权，承担保密责任和接受相应的法律约束。
*
*/

#include <socket_api.h>
#include <socket_types.h>
#include <errno.h>
#include <stdio.h>

#include "rs_socket.h"
#include "rs_debug.h"
#include "rs_error.h"
#include "rs_std_fun.h"

unsigned long nIPAddr;
unsigned long nHostPort;
TCPIP_SOCKET_T g_socket_id = -1;
rs_u8 g_rsfota_nsapi = 0;

void *g_rs_handle = RS_NULL;
RS_SOCKET_MSG_HANDLER_CALLBACK g_socket_callback = RS_NULL;



rs_s32 rs_socket_extract_ip_addr(rs_s8* szDstBuf, const rs_s8* szSrcURL, rs_u32 *nHostPort )
{
	int ret = 0;
	char portBuf[10];
	char* portPtr;

	// check "http" 
	if ( rs_memcmp(szSrcURL, "http://", 7) == 0 )
	{
		szSrcURL += 7;
	}
	else if ( rs_memcmp(szSrcURL, "https://", 8) == 0 )
	{
		szSrcURL += 8;
		ret = 1;
	}

	// copy data 
	while ( *szSrcURL != 0 && *szSrcURL != ':' && *szSrcURL != '/' )
	{
		*szDstBuf++ = *szSrcURL++;
	}
	*szDstBuf = 0;

	if (*szSrcURL == ':')
	{
		long port = 0;
		portPtr = &portBuf[0];
		szSrcURL++;
		*portPtr++ = *szSrcURL++;
		while ( *szSrcURL != 0 && *szSrcURL != '/' )
		{
			*portPtr++ = *szSrcURL++;
		}
		*portPtr = 0;
		port = rs_atoi(portBuf);
		*nHostPort = port;
	}
	else
	{
		*nHostPort = 80;
	}

	return ret;
}

rs_s32 rs_socket_gethostbyname(const rs_s8 *uri)
{
	rs_s8 ip_buf[256];
	struct sci_hostent *pSciHost;
	TCPIP_HOST_HANDLE handle;
	rs_u32 nIPAddr;
    rs_u8 nNetID = rs_socket_get_netid();

	rs_strcpy(ip_buf, uri);
	rs_socket_extract_ip_addr(ip_buf, uri, &nHostPort);

	RS_PORITNG_LOG(RS_LOG_DEBUG"host:%s, net id is %d\n", ip_buf, nNetID); 

	handle = sci_async_gethostbyname(ip_buf,  rs_get_thread_id(), 10000, (TCPIP_NETID_T)nNetID);
	if (handle == 0)
	{
		RS_PORITNG_LOG(RS_LOG_DEBUG"gethostbyname failed\n");    
		return RS_ERR_FAILED;
	}
	else
	{
		RS_PORITNG_LOG(RS_LOG_DEBUG"wait gethostbyname callback\n");  
	}
	
	return RS_ERR_OK;
}

void rs_socket_setHostAndPort(rs_u32 host, rs_u32 port)
{
	nIPAddr = host;
	nHostPort = port;
}

rs_s32 rs_socket_init(RS_SOCKET_MSG_HANDLER_CALLBACK callback, void* handle)
{
	g_rs_handle = handle;
	g_socket_callback = callback;
	
	return RS_ERR_OK;
}

rs_s32 rs_socket_getMode()
{
	return 1;
}

rs_socket_handle rs_socket_create()
{
	TCPIP_SOCKET_T socket_id = 0;

	socket_id = sci_sock_socket(AF_INET, SOCK_STREAM, 0, (TCPIP_NETID_T)rs_socket_get_netid()); 
	if(-1 == socket_id)
	{
		RS_PORITNG_LOG(RS_LOG_DEBUG"create socket failed\n");        
		return RS_SOC_INVALID_SOCKET_H;
	}

	g_socket_id = socket_id;

	RS_PORITNG_LOG(RS_LOG_DEBUG"create socket id = %d\n", socket_id);  

	return (rs_socket_handle)socket_id;
}

rs_s32 rs_socket_connect(rs_socket_handle  socketHandle)
{
	struct sci_sockaddr sa;
	rs_s32 result = 0;
	rs_s32 sock_error_code = 0; 

	memset(&sa, 0, sizeof(sa));
	sa.family  = AF_INET;
	sa.ip_addr = nIPAddr;
	sa.port    = htons(nHostPort); 

	result = sci_sock_setsockopt((TCPIP_SOCKET_T)socketHandle, SO_NBIO, PNULL);		
	if(0 != result)
	{
		sock_error_code = sci_sock_errno((TCPIP_SOCKET_T)socketHandle);
		RS_PORITNG_LOG(RS_LOG_DEBUG"socket set no block fail, ret=%d\n", sock_error_code);
		sci_sock_socketclose((TCPIP_SOCKET_T)socketHandle);
		return -1;
	}
			
	result = sci_sock_asyncselect((TCPIP_SOCKET_T)socketHandle, rs_get_thread_id(), AS_READ|AS_WRITE|AS_CONNECT|AS_CLOSE);
	if(0 != result)
	{
		RS_PORITNG_LOG(RS_LOG_DEBUG"socket set async fail, ret=%d\n", result);
		sci_sock_socketclose((TCPIP_SOCKET_T)socketHandle);
		return -1;
	}

	result = sci_sock_connect((TCPIP_SOCKET_T)socketHandle, &sa, sizeof(sa));
	if(-1 == result)
	{
		sock_error_code = sci_sock_errno((TCPIP_SOCKET_T)socketHandle);
		if(EWOULDBLOCK == sock_error_code || EINPROGRESS == sock_error_code || EISCONN == sock_error_code)
		{
			RS_PORITNG_LOG(RS_LOG_DEBUG"connect wait callback ....\n");
			return 0;
		}
		else
		{
			RS_PORITNG_LOG(RS_LOG_DEBUG"socket connect fail, ret=%d\n", sock_error_code);
			sci_sock_socketclose((TCPIP_SOCKET_T)socketHandle);
			return -1;
		}
	}

	return 1;
}

rs_s32 rs_socket_send(rs_socket_handle sockHndl, const rs_s8 *sendData, rs_s32 size)
{
	rs_s32 send_len     = 0;
	rs_s32 sock_error_code    = 0;

	send_len = sci_sock_send((TCPIP_SOCKET_T)sockHndl, sendData, size, 0);
	if(send_len <= 0)
	{
		sock_error_code = sci_sock_errno((TCPIP_SOCKET_T)sockHndl);
		RS_PORITNG_LOG(RS_LOG_DEBUG"sock send err = %d\n", sock_error_code);
		if(EWOULDBLOCK == sock_error_code || EINPROGRESS == sock_error_code || EISCONN == sock_error_code)
		{
			return 0;
		}
		else
		{
			return -1;
		}
	}

	RS_PORITNG_LOG(RS_LOG_DEBUG"sock send success, size = %ld\n", send_len);

	return send_len;
}

rs_s32 rs_socket_recv(rs_socket_handle sockHndl, rs_s8 *buffer, rs_s32 bufLen)
{
	rs_s32 ret = 0;

	ret = sci_sock_recv((TCPIP_SOCKET_T)sockHndl, buffer, bufLen, 0);
	if (ret <= 0)
	{
            	rs_s32 errcode = 0;
		errcode = sci_sock_errno((TCPIP_SOCKET_T)sockHndl);
		RS_PORITNG_LOG(RS_LOG_DEBUG"recv len = %d, error code is %d\n", ret, errcode);
		if(EWOULDBLOCK == errcode || EINPROGRESS == errcode || EISCONN == errcode)
		{
			return 0;
		}
		else
		{
			return -1;
		}
	}

	return ret;
}

rs_s32 rs_socket_select(rs_socket_handle sockHndl, rs_s32 nTimeOut)
{	
/*
	sci_fd_set fd_read, fd_exc;
	int ret;

	SCI_FD_ZERO(&fd_read);
	SCI_FD_ZERO(&fd_exc);
	SCI_FD_SET((TCPIP_SOCKET_T)sockHndl, &fd_read);

	ret = sci_sock_select(&fd_read , NULL,  &fd_exc,  nTimeOut / 100);
	if(0 == ret)
	{
		//select超时
		RS_PORITNG_LOG(RS_LOG_DEBUG"socket select timeout\n");
	}
	else if(0 > ret)
	{
		//select出错
		RS_PORITNG_LOG(RS_LOG_DEBUG"socket select error\n");
	}
	else
	{
		// 检查socket异常事件是否发生
		if(SCI_FD_ISSET((TCPIP_SOCKET_T)sockHndl, &fd_exc))
		{
			//异常事件
			ret = -1;
		}
		// 检查socket读事件是否发生
		if (SCI_FD_ISSET((TCPIP_SOCKET_T)sockHndl, &fd_read) ) 
		{

		}
	}

	return ret;
	*/

	// async mode not support select interface
	return -1;
}

rs_s32 rs_socket_close(rs_socket_handle sockHndl)
{	
	RS_PORITNG_LOG(RS_LOG_DEBUG"socket close\n");
	sci_sock_socketclose((TCPIP_SOCKET_T)sockHndl);
	g_socket_id = -1;
	
	return RS_ERR_OK;
}

// before next select wait moment
void rs_socket_wait_moment()
{

}

rs_s32 rs_socket_only_wait_callback()
{
	return 1;
}

void rs_socket_set_netid(rs_u8 netid)
{
	g_rsfota_nsapi = netid;
}

rs_u8 rs_socket_get_netid()
{
	return g_rsfota_nsapi;
}

/****************************************************************************************/


// gethostbyname handler
void fota_get_ip_handle(void* msg)
{
	uint32 request_id = 0;
	uint32 netid = 0;
	struct sci_hostent *hostent = PNULL;
	int error_code = 0;

	request_id = ((ASYNC_GETHOSTBYNAME_CNF_SIG_T*)msg)->request_id;
	error_code = ((ASYNC_GETHOSTBYNAME_CNF_SIG_T*)msg)->error_code;
	hostent = &(((ASYNC_GETHOSTBYNAME_CNF_SIG_T*)msg)->hostent);
	netid = ((ASYNC_GETHOSTBYNAME_CNF_SIG_T*)msg)->netid;

	RS_PORITNG_LOG(RS_LOG_DEBUG"host call back errcode is %d, \n", error_code);

	if (0 == error_code)
	{
		SCI_MEMCPY(&nIPAddr, hostent->h_addr_list[0], 4);

		if (g_socket_callback != RS_NULL && g_rs_handle != RS_NULL)
		{
			g_socket_callback(g_rs_handle, SOCKET_MSG_COME, 0, 0);
		}
	}
	else
	{
		if (g_socket_callback != RS_NULL && g_rs_handle != RS_NULL)
		{
			g_socket_callback(g_rs_handle, SOCKET_MSG_COME, 1, 0);
		}
	}
}

// send recv connect handler
void fota_event_handle(void *msg_ptr, RS_SOCKET_MSG_TYPE msgType)
{

	switch (msgType) 
	{
		case RS_SOCKET_MSG_WRITE:
			RS_PORITNG_LOG("socket read event\n\r");
			
			if (g_socket_id !=((SOCKET_WRITE_EVENT_IND_SIG_T*)msg_ptr)->socket_id)
			{
				RS_PORITNG_LOG("socket write event, but socket not match\n\r");
				return;
			}
	
			if (g_socket_callback != RS_NULL && g_rs_handle != RS_NULL)
			{
				g_socket_callback(g_rs_handle, SOCKET_MSG_COME, 0, 0);
			}
			break;
		
		case RS_SOCKET_MSG_READ:
			RS_PORITNG_LOG("socket read event\n\r");
			if (g_socket_id !=((SOCKET_READ_EVENT_IND_SIG_T*)msg_ptr)->socket_id)
			{
				RS_PORITNG_LOG("socket read event, but socket not match\n\r");
				return;
			}

			if (g_socket_callback != RS_NULL && g_rs_handle != RS_NULL)
			{
				g_socket_callback(g_rs_handle, SOCKET_MSG_COME, 0, 0);
			}
			break;
			
		case RS_SOCKET_MSG_CONNECT:
			{
				rs_u32 errcode = ((SOCKET_CONNECT_EVENT_IND_SIG_T*)msg_ptr)->error_code;
				if (g_socket_id != ((SOCKET_CONNECT_EVENT_IND_SIG_T*)msg_ptr)->socket_id)
				{
					RS_PORITNG_LOG(RS_LOG_DEBUG"recv connect msg, but not mach\n");
			                return;
				}

				if(errcode == 0)
				{
					rs_s32 result = 0;
					RS_PORITNG_LOG("socket connect successfully\n\r");
					result = sci_sock_asyncselect((TCPIP_SOCKET_T)g_socket_id, rs_get_thread_id(), AS_READ|AS_WRITE|AS_CONNECT|AS_CLOSE);
					if(0 != result)
					{
						RS_PORITNG_LOG(RS_LOG_DEBUG"socket set async fail, ret=%d\n", result);
						sci_sock_socketclose((TCPIP_SOCKET_T)g_socket_id);
						if (g_socket_callback != RS_NULL && g_rs_handle != RS_NULL)
						{
							g_socket_callback(g_rs_handle, SOCKET_MSG_COME, 1, 0);
						}
						
						return;
					}
					
					if (g_socket_callback != RS_NULL && g_rs_handle != RS_NULL)
					{
						g_socket_callback(g_rs_handle, SOCKET_MSG_COME, 0, 0);
					}
				} 
				else 
				{
					sci_sock_socketclose((TCPIP_SOCKET_T)g_socket_id);
					RS_PORITNG_LOG("SOC_CONNECT error and soc_close\n\r");

					if (g_socket_callback != RS_NULL && g_rs_handle != RS_NULL)
					{
						g_socket_callback(g_rs_handle, SOCKET_MSG_COME, 1, 0);
					}
				}
			}
			break;
		
		case RS_SOCKET_MSG_CLOSE:
			if (g_socket_id !=((SOCKET_CONNECTION_CLOSE_EVENT_IND_SIG_T*)msg_ptr)->socket_id)
			{
				RS_PORITNG_LOG("socket connection close, but socket not match\n\r");
				return;
			}

			RS_PORITNG_LOG("socket connection close, error code is %d\n\r", ((SOCKET_CONNECTION_CLOSE_EVENT_IND_SIG_T*)msg_ptr)->error_code );
			sci_sock_socketclose((TCPIP_SOCKET_T)g_socket_id);
			if (g_socket_callback != RS_NULL && g_rs_handle != RS_NULL)
			{
				g_socket_callback(g_rs_handle, SOCKET_MSG_COME, 1, 0);
			}
			break;
	}
}
