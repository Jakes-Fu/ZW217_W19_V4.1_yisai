#ifndef _MMIWRE_SOCKET_H_
#define _MMIWRE_SOCKET_H_

#define ADAPT_SUCCESS    0

#define ADAPT_AS_NONE                      0
#define ADAPT_AS_READ                      0x01
#define ADAPT_AS_WRITE                     0x02
#define ADAPT_AS_CONNECT                   0x04
#define ADAPT_AS_CLOSE                     0x08
#define ADAPT_AS_ACCEPT                    0x10
#define ADAPT_AS_FULLCLOSE                 0x20
#define ADAPT_AS_READ_BUFFER_STATUS        0x40

#define ADAPT_EVENT_READ                    1
#define ADAPT_EVENT_WRITE                   2
#define ADAPT_EVENT_CONNECT                 3
#define ADAPT_EVENT_ACCEPT                  4
#define ADAPT_EVENT_FULL_CLOSED             5   
#define ADAPT_EVENT_ASYNC_GETHOSTBYNAME     6
#define ADAPT_EVENT_CONNECTION_CLOSE_EVENT  7


int adapt_sock_socket(int family, int type, int proto, uint32 net_id );

int adapt_sock_connect(int so, void* addr_ptr, int addr_len );

int adapt_sock_errno(int so);

void * adapt_gethostbyname(char *name_ptr,uint32 net_id);

int adapt_gethostname(char *hostname, int namelen);

int adapt_sock_socketclose( int so );

int adapt_sock_send( int so, char* buf, int len, int flags );

int adapt_sock_recv( int so, char* buf, int len, int flag );

int adapt_sock_select(
    void *    in,
    void *    out,
    void *    ex,
    long            tv);


int adapt_sock_sendto(
    int so, char* buf, int len, int flag, void * to );

int adapt_sock_recvfrom(
    int so, char* buf, int len, int flag, void * from );

int adapt_sock_setsockopt( int so, int name, void* arg_ptr );

int adapt_sock_getsockopt( int so, int name, void* arg_ptr );

int adapt_sock_listen( int so, int backlog );

int adapt_sock_bind(int so, void * addr_ptr, int addr_len );

int adapt_sock_accept(int so, void* addr_ptr, int* addr_len );

int adapt_sock_shutdown( int so, int how );

int adapt_sock_asyncrecv(
    int             so,
    char*           buf,
    int             len, 
    int             flag, 
    int*            left_len_ptr);

int adapt_sock_asyncselect(
    int so, uint32 task_id, uint32 event);

void * adapt_async_gethostbyname(
    char*           name_ptr,
    uint32          task_id,
    uint32          time_out,
    uint32          netid );

int adapt_sock_getpeername(int so, void * addr_ptr );

int adapt_sock_getsockname(int so,void* addr_ptr);

uint32 adapt_gethostaddress( void );

uint32 adapt_gethostaddress_ext( uint32 netid );

unsigned long adapt_inet_addr(char * cp);

char * adapt_inet_ntoa(unsigned long in);

uint32 adapt_htonl(uint32 l);

uint32 adapt_ntohl(uint32 l);

uint16 adapt_ntohs(uint16 s);

uint16 adapt_htons(uint16 s);

//=========================== asyncmsg.c ===============================
void Adapt_PostMessage(uint32 hWnd,uint32 wMsg,uint32 wParam,uint32 lParam);

uint32 Adapt_InitSockTask(void);//≥ı ºªØWRE socket.

void Adapt_DeinitSockTask(uint32 task_id);

#endif




