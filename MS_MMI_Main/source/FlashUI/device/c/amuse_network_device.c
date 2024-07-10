/*
* amuse_network_device.c
* Copyright (C) 2011 Microrapid Inc
* Author: gentoo <gentoo@Microrapid.com>
* 
* This file implementes network interface (dummy)
*   for mtk platform.
*/

#include "amuse_network_device.h"
#include "stdio.h"

#ifdef WIN32
#include <winsock2.h>
#endif

typedef unsigned int UI32, U32,u32;

typedef signed int SI32, S32,s32;

typedef unsigned short UI16, U16, u16;

typedef signed short SI16, S16, s16;

typedef unsigned char UI8, U8, u8;

typedef signed char SI8, S8, s8;
//typedefs

typedef SI32						socket_t;
typedef const char *				amuse_c_buffer_p_t;
typedef struct hostent *			amuse_hostent_p_t;
typedef const struct sockaddr *		amuse_c_sockaddr_p_t;
typedef char *						amuse_buffer_p_t;

//marcos

socket_t			amuse_socket(SI32 af, SI32 type, SI32 protocol);
SI32					amuse_closesocket(socket_t s);
UI16					amuse_htons(UI16 hostshort);
UI32					amuse_inet_addr(amuse_c_buffer_p_t cp);
amuse_hostent_p_t		amuse_gethostbyname(amuse_c_buffer_p_t name);
SI32					amuse_bind(socket_t s, amuse_c_sockaddr_p_t addr, SI32 len);
SI32					amuse_listen(socket_t s, SI32 backlog);
SI32					amuse_connect(socket_t s, amuse_c_sockaddr_p_t name, SI32 namelen);
SI32					amuse_send(socket_t s, amuse_c_buffer_p_t buf, SI32 len, SI32 flags);
SI32					amuse_recv(socket_t s, amuse_buffer_p_t buf, SI32 len, SI32 flags);
SI32					amuse_setsockopt(socket_t s, SI32 level, SI32 optname, amuse_c_buffer_p_t optval, SI32 optlen);
//SI32                    amuse_network_getlasterror();
socket_t			amuse_accept(socket_t s, struct sockaddr* addr, SI32* len);


//func definitions
socket_t amuse_socket(SI32 af, SI32 type, SI32 protocol)
{
	return 0;
}

int 
amuse_closesocket(socket_t s)
{
	return 0;//succeed.
}

UI16 
amuse_htons(UI16 hostshort)
{
	return hostshort;
}

UI32
amuse_inet_addr(amuse_c_buffer_p_t cp)
{
	return 0;
}

amuse_hostent_p_t 
amuse_gethostbyname(amuse_c_buffer_p_t name)
{
	return 0;
}

SI32
amuse_bind(socket_t s, amuse_c_sockaddr_p_t addr, SI32 len)
{
	return -1;
}

SI32
amuse_listen(socket_t s, SI32 backlog)
{
	return -1;
}

socket_t
amuse_accept(socket_t s, struct sockaddr* addr, SI32* len)
{
	return -1;
}

SI32 
amuse_connect(socket_t s, amuse_c_sockaddr_p_t name, SI32 namelen)
{
	return 1;
}

SI32
amuse_send(socket_t s, amuse_c_buffer_p_t buf, SI32 len, SI32 flags)
{
	return 0;
}

SI32
amuse_recv(socket_t s, amuse_buffer_p_t buf, SI32 len, SI32 flags)
{
	return 0;
}

SI32
amuse_setsockopt(socket_t s, SI32 level, SI32 optname, amuse_c_buffer_p_t optval, SI32 optlen)
{
	return 0;
}

int
amuse_network_getlasterror(void)
{
	return 0;
}


amuse_socket_t amuse_network_socket_open()
{
	return NULL;
}

int amuse_network_socket_close(amuse_socket_t sock)
{
	return 0;
}

//=================================================================
// basic socket functions.
// 
//
//=================================================================
//windows socket start up.
//return: 0, start up OK.
//        1, start up failed.
int amuse_network_socket_startup(void)
{
	return 1;
}

//windows socket shut down.
//return: 0, shut down OK.
//        1, shut down failed.
int amuse_network_socket_shutdown(void)
{
	return 0;
}


//marcos
#define TIMEOUTS		1500



int amuse_network_create_socket(amuse_socket_t sock)
{
	return 0;
}


int amuse_network_close_socket(amuse_socket_t sock)
{
	return amuse_closesocket((socket_t)*sock);
}

//connect to network.
//in: sock
//    shost:connecting destination.
//    nport:
//return: 1, OK.
//        0, failed.
int amuse_network_socket_connect( amuse_socket_t sock, char *shost, int nport)
{
	//amuse_sockaddr_in_t saddr;
	return 0;//(Network_InitSocketAddr(shost,nport,&saddr))&& (Network_SocketConnectEx(sock,&saddr));
}

//send data to network.
//return : data length.
int amuse_network_socket_send( amuse_socket_t sock, char *packet, int buflen)
{
	return amuse_send((socket_t)*sock,packet,buflen,0);
}

// recv data, this one is block mode
int amuse_network_socket_recv(amuse_socket_t sock, char *packet, int buflen)
{
	return amuse_recv((socket_t)*sock, packet, buflen, 0);
}


int amuse_network_recv_packet(amuse_socket_t sock,char* packet, int maxlen)
{
	 return -1;
}

// set socket option.
// in: to, time out (m.seconds)
int amuse_network_set_nonblock(amuse_socket_t sock,int to)
{
	return 0;//amuse_setsockopt((socket_t)*sock, SOL_SOCKET, SO_RCVTIMEO,(char*) &to, sizeof(int));
}


char *
amuse_network_get_host_ip(char *hostname)
{
	char *ptr = 0;
	
	//ToDo: mtk network
#ifdef WIN32
	char **str;
	struct hostent *host = NULL;
	WSADATA WSAData;
	int WSA_return;

	WSA_return = WSAStartup(0x0101, &WSAData);

	if((host = gethostbyname(hostname)) == NULL){
		return 0; 
	}

	switch(host->h_addrtype){
	case 2:
	case 23:
		str = host->h_addr_list;

		if(*str != NULL)
		{
			struct in_addr in = {0};

			in.S_un.S_addr = ((ULONG)(host->h_addr_list[0][0] & 0x00ff) | ((ULONG)host->h_addr_list[0][1] & 0x00ff) <<8 |
				(ULONG)(host->h_addr_list[0][2] & 0x00ff) << 16 | (ULONG)(host->h_addr_list[0][3] & 0x00ff) << 24);

			ptr = inet_ntoa(in);
		}
		break;
	default:
		break;
	}	

	WSACleanup();
#endif
	return ptr;
}
