//author: Justin, for watch version

#ifndef ___HTTP_SOCKET_H
#define ___HTTP_SOCKET_H
#pragma  once

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
	#if defined(WIN32) && (_WIN32_WINNT < 0x0501)
		#include <winsock.h>
	#else
		#include <winsock2.h>
		#include <ws2tcpip.h>
	#endif

	#define HTTP_SOCKET SOCKET
	#define HTTP_INVALID_SOCKET INVALID_SOCKET
#else
	#define HTTP_SOCKET int
	#define HTTP_INVALID_SOCKET -1
#endif

#include "sds.h"

#define	SOCK_LEBAO_NULL		0
#define	SOCK_LEBAO_READ		0x01
#define	SOCK_LEBAO_WRITE	0x02
#define	SOCK_LEBAO_CONNECT	0x04
#define	SOCK_LEBAO_CLOSE	0x08
#define	SOCK_LEBAO_ACCEPT	0x10
#define	SOCK_LEBAO_RDBUF	0x20
#define	SOCK_LEBAO_DNS		0x40

#define	SOCK_LEBAO_SSL_CNT	0x100
#define	SOCK_LEBAO_SSL_RECV	0x200
#define	SOCK_LEBAO_SSL_SEND	0x400

#define	SOCK_LEBAO_MASK		0x1000
#define	SOCK_LEBAO_EXCEPT	0x2000

typedef int(*HTTP_EVENT_CALLBACK_T)(const int evt, const int status, void* param, void* data);
typedef int(*CANCEL_SOCKET_CONNECT) (void* data);

typedef struct {
	HTTP_SOCKET sock;
	sds host;
	unsigned int ip;
	unsigned int port;
	int isSSL;
	int sslConnected;
	int needEvent;
    CANCEL_SOCKET_CONNECT isCanceled;
    void* extData;

	int taskId;
	int netId;
	void* dnsHostHandle;
	int waitingSignal;
	int selectTaskId;

	int bufferIsFull;
} http_socket_t;

unsigned int http_socket_dns(char const* address, const int netId);
unsigned int http_socket_async_dns(char const* address, const int taskId, const int netId);

http_socket_t* http_socket_create(char const* address, const unsigned int port, const int taskId, const int netId);
int http_socket_connect(http_socket_t* ctx);
void http_socket_close(http_socket_t* ctx);

int http_socket_send(http_socket_t* ctx, char const* buf, const size_t len);
int http_socket_recv(http_socket_t* ctx, char* buf, const size_t len);

int http_socket_select_task_run(http_socket_t* ctx, HTTP_EVENT_CALLBACK_T callback, void* data);
int http_socket_select_task_stop(http_socket_t* ctx, const int taskId);

int http_socket_ssl_connect(http_socket_t* ctx);

#ifdef __cplusplus
}
#endif
#endif
