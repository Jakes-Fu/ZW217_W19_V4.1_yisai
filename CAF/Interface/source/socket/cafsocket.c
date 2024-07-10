#include "caf_trc.h"
#ifdef DYNAMIC_MODULE_SUPPORT
/****************************************************************************
** File Name:      cafsocket.c                                             *
** Author:         James.Zhang                                             *
** Date:           10/16/2008                                              *
** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the application modual.   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 10/2008        James.Zhang       Create                                 *
** 02/2009        Wei.Zhou          Modified, Classfy 2 class, both        *
                                    NetMgr & Socket                        *
****************************************************************************/


/**---------------------------------------------------------------------------*
**                         Dependencies                                      *
**---------------------------------------------------------------------------*/
#include "cafmodule_gen.h"
#include "cafsocket.h"
#include "app_tcp_if.h"
#include "mmk_app.h"
#include "in_Message.h"
#include "sci_api.h"
#include "tasks_id.h"
#include "mmk_shell.h"
#include "priority_app.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define     P_ASYN_TASK_STACK_SIZE              1024
#define     P_ASYN_TASK_QUEUE_NUM               20
#define     P_ASYN_TASK_PRIORITY                PRI_ASYN_TASK_PRIORITY
#define     MAX_SOCKET_NUMS                     32

/**--------------------------------------------------------------------------*
**                         STURCT DEFINITION                                *
**--------------------------------------------------------------------------*/
typedef struct CAFSOCKET_T CAFSOCKET_T;

typedef struct
{
    CAF_VTBL_T(INETMGR_T)  *vtbl_ptr; // 函数列表
    CAF_GUID_T       guid;
    uint32           ref_num;
    BLOCK_ID         async_task_id;
    uint32           thread_start;
#ifdef  WIN32
    HWND             hWin;
#endif
    CAFSOCKET_T      *s_socket_ptr[MAX_SOCKET_NUMS];
}CAFNETMGR_T;

struct CAFSOCKET_T
{
    CAF_VTBL_T(ISOCKET_T)  *vtbl_ptr;    // 函数列表
    CAF_GUID_T       guid;
    uint32           ref_num;
    long             s;
    BLOCK_ID         async_task_id;
    uint32           regevent;
    CAF_HANDLE_T     hcafwin;
    CAF_HANDLE_T     hregread;
    CAF_HANDLE_T     hregwrite;
    CAF_HANDLE_T     hregconnect;
    CAF_HANDLE_T     hregclose;
    CAF_HANDLE_T     hregaccept;
#ifdef  WIN32
    HWND             hWin;
#endif
    uint16           msg_id;
    uint16           thread_start;
    CAFNETMGR_T      *cafnetmgr_ptr;
};

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL struct  hostent s_hostname;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/* compatible with win sock */
#ifdef WIN32

#include "windows.h"

#pragma comment(lib, "Ws2_32.lib")

#define     DECLSPEC_IMPORT     __declspec(dllimport)
#define     WSAAPI              far __stdcall

typedef unsigned char   u_char;
typedef unsigned short  u_short;
typedef unsigned int    u_int;
typedef unsigned long   u_long;

typedef u_int               SOCKET;

#define FD_READ_BIT      0
#define FD_READ          (1 << FD_READ_BIT)

#define FD_WRITE_BIT     1
#define FD_WRITE         (1 << FD_WRITE_BIT)

#define FD_ACCEPT_BIT    3
#define FD_ACCEPT        (1 << FD_ACCEPT_BIT)

#define FD_CONNECT_BIT   4
#define FD_CONNECT       (1 << FD_CONNECT_BIT)

#define FD_CLOSE_BIT     5
#define FD_CLOSE         (1 << FD_CLOSE_BIT)

#define WM_SOCKET        0x7070

#ifndef FD_SETSIZE
#define FD_SETSIZE      64
#endif /* FD_SETSIZE */

#define WSAGETSELECTERROR(lParam)           HIWORD(lParam)

/*
* All Windows Sockets error constants are biased by WSABASEERR from
* the "normal"
*/
#define WSABASEERR              10000

#define WSAEFAULT               (WSABASEERR+14)
#define WSAEINVAL               (WSABASEERR+22)

/*
* Windows Sockets definitions of regular Berkeley error constants
*/
#define WSAEWOULDBLOCK          (WSABASEERR+35)
#define WSAEINPROGRESS          (WSABASEERR+36)
#define WSAEALREADY             (WSABASEERR+37)
#define WSAENOTSOCK             (WSABASEERR+38)
#define WSAEDESTADDRREQ         (WSABASEERR+39)
#define WSAEMSGSIZE             (WSABASEERR+40)
#define WSAEPROTOTYPE           (WSABASEERR+41)
#define WSAENOPROTOOPT          (WSABASEERR+42)
#define WSAEPROTONOSUPPORT      (WSABASEERR+43)
#define WSAESOCKTNOSUPPORT      (WSABASEERR+44)
#define WSAEOPNOTSUPP           (WSABASEERR+45)
#define WSAEPFNOSUPPORT         (WSABASEERR+46)
#define WSAEAFNOSUPPORT         (WSABASEERR+47)
#define WSAEADDRINUSE           (WSABASEERR+48)
#define WSAEADDRNOTAVAIL        (WSABASEERR+49)
#define WSAENETDOWN             (WSABASEERR+50)
#define WSAENETUNREACH          (WSABASEERR+51)
#define WSAENETRESET            (WSABASEERR+52)
#define WSAECONNABORTED         (WSABASEERR+53)
#define WSAECONNRESET           (WSABASEERR+54)
#define WSAENOBUFS              (WSABASEERR+55)
#define WSAEISCONN              (WSABASEERR+56)
#define WSAENOTCONN             (WSABASEERR+57)
#define WSAESHUTDOWN            (WSABASEERR+58)
#define WSAETOOMANYREFS         (WSABASEERR+59)
#define WSAETIMEDOUT            (WSABASEERR+60)
#define WSAECONNREFUSED         (WSABASEERR+61)
#define WSAELOOP                (WSABASEERR+62)
#define WSAENAMETOOLONG         (WSABASEERR+63)
#define WSAEHOSTDOWN            (WSABASEERR+64)
#define WSAEHOSTUNREACH         (WSABASEERR+65)
#define WSAENOTEMPTY            (WSABASEERR+66)
#define WSAEPROCLIM             (WSABASEERR+67)
#define WSAEUSERS               (WSABASEERR+68)
#define WSAEDQUOT               (WSABASEERR+69)
#define WSAESTALE               (WSABASEERR+70)
#define WSAEREMOTE              (WSABASEERR+71)


struct timeval {
    long    tv_sec;         /* seconds */
    long    tv_usec;        /* and microseconds */
};

typedef struct fd_set {
    u_int fd_count;               /* how many are SET? */
    SOCKET  fd_array[FD_SETSIZE];   /* an array of SOCKETs */
} fd_set;

struct sockaddr {
    u_short sa_family;              /* address family */
    char    sa_data[14];            /* up to 14 bytes of direct address */
};


DECLSPEC_IMPORT SOCKET WSAAPI socket(int af, int type, int protocol);
DECLSPEC_IMPORT SOCKET WSAAPI accept(SOCKET s, struct sockaddr FAR * addr, int FAR * addrlen);
DECLSPEC_IMPORT int WSAAPI bind(SOCKET s, const struct sockaddr FAR * name, int namelen);
DECLSPEC_IMPORT int WSAAPI closesocket(SOCKET s);
DECLSPEC_IMPORT int WSAAPI connect(SOCKET s, const struct sockaddr FAR * name, int namelen);
DECLSPEC_IMPORT int WSAAPI getpeername(SOCKET s, struct sockaddr FAR * name, int FAR * namelen);
DECLSPEC_IMPORT int WSAAPI getsockname(SOCKET s, struct sockaddr FAR * name, int FAR * namelen);
DECLSPEC_IMPORT int WSAAPI getsockopt(SOCKET s, int level, int optname, char FAR * optval, int FAR * optlen);
DECLSPEC_IMPORT int WSAAPI gethostname(char FAR * name, int namelen);
DECLSPEC_IMPORT int WSAAPI listen(SOCKET s, int backlog);
DECLSPEC_IMPORT int WSAAPI recv(SOCKET s, char FAR * buf, int len, int flags);
DECLSPEC_IMPORT int WSAAPI recvfrom(SOCKET s, char FAR * buf, int len, int flags, struct sockaddr FAR * from, int FAR * fromlen);
DECLSPEC_IMPORT int WSAAPI send(SOCKET s, const char FAR * buf, int len, int flags);
DECLSPEC_IMPORT int WSAAPI sendto(SOCKET s, const char FAR * buf, int len, int flags, const struct sockaddr FAR * to, int tolen);
DECLSPEC_IMPORT int WSAAPI setsockopt(SOCKET s, int level, int optname, const char FAR * optval, int optlen);
DECLSPEC_IMPORT int WSAAPI shutdown(SOCKET s, int how);
DECLSPEC_IMPORT int WSAAPI WSAGetLastError(void);
DECLSPEC_IMPORT struct hostent * WSAAPI gethostbyname(const char FAR * name);
DECLSPEC_IMPORT int WSAAPI select(int nfds, fd_set FAR * readfds, fd_set FAR * writefds, fd_set FAR *exceptfds, const struct timeval FAR * timeout);
DECLSPEC_IMPORT int WSAAPI ioctlsocket(SOCKET s, long cmd, u_long FAR * argp);
DECLSPEC_IMPORT int WSAAPI WSAAsyncSelect(SOCKET s, HWND hWnd, u_int wMsg, long lEvent);

LOCAL HWND CAFSOCKET_CreateWindow(LPVOID argv, CHAR *lpszClassName, WNDCLASS *lpWndCls);
LOCAL LRESULT CALLBACK CAFSOCKET_WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam,LPARAM lParam);

#endif

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : register created socket, for searching
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL uint32 CAFINETMGR_RegCreatedSocket(CAFNETMGR_T *cafnetmgr_ptr, CAFSOCKET_T *cafsocket_ptr);

/*****************************************************************************/
//  Description : unregister created socket
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL uint32 CAFINETMGR_UnRegCreatedSocket(CAFNETMGR_T *cafnetmgr_ptr, CAFSOCKET_T *cafsocket_ptr);

/*****************************************************************************/
//  Description : search socket class pointer for socket
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL CAFSOCKET_T *CAFINETMGR_GetSocketPtrFroms(CAFNETMGR_T *cafnetmgr_ptr, long s);

/*****************************************************************************/
//  Description : search socket class pointer by signal
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL CAFSOCKET_T *CAFINETMGR_GetSocketPtrFromSignal(CAFNETMGR_T *cafnetmgr_ptr, MmiSignalS* sig_ptr);

/*****************************************************************************/
//  Description : netmgr class created.
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/
LOCAL CAF_RESULT_E CAFINETMGR_ClassNew(CAF_GUID_T guid, void** object_pptr);

/*****************************************************************************/
//  Description : netmgr query interface
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/
LOCAL CAF_RESULT_E CAFINETMGR_QueryInterface(INETMGR_T * netmgr_ptr, CAF_GUID_T guid, void ** object_pptr );

/*****************************************************************************/
//  Description : netmgr add ref
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/
LOCAL uint32 CAFINETMGR_AddRef(INETMGR_T * netmgr_ptr);

/*****************************************************************************/
//  Description : netmgr release
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/
LOCAL uint32 CAFINETMGR_Release(INETMGR_T * netmgr_ptr);

/*****************************************************************************/
//  Description : CAF encapsulation of GetHostByName
//  Global resource dependence : 
//  Author:James.Zhang
//  Modify:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL struct hostent * CAFINETMGR_GetHostByName(INETMGR_T * netmgr_ptr, const char * sz_ptr);

/*****************************************************************************/
//  Description : CAF encapsulation of Select
//  Global resource dependence : 
//  Author:James.Zhang
//  Modify:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFINETMGR_Select(INETMGR_T * netmgr_ptr, int32 nfds, ISOCKET_FD_SET_T* in_ptr, ISOCKET_FD_SET_T* out_ptr, ISOCKET_FD_SET_T* ex_ptr, int32 timeout );

/*****************************************************************************/
//  Description : CAF encapsulation of OpenSocket
//  Global resource dependence : 
//  Author:James.Zhang
//  Modify:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL ISOCKET_T * CAFINETMGR_OpenSocket( INETMGR_T *netmgr_ptr, int32 family, int32 type, int32 protocol);

/*****************************************************************************/
//  Description : CAF encapsulation of GetSockOpt
//  Global resource dependence : 
//  Author:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFINETMGR_GetHostName(INETMGR_T * netmgr_ptr, char * sz_ptr, int32 namelen);

/*****************************************************************************/
//  Description : socket query interface
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/
LOCAL CAF_RESULT_E CAFSOCKET_QueryInterface( ISOCKET_T *socket_ptr, CAF_GUID_T guid, void ** object_pptr );

/*****************************************************************************/
//  Description : socket add ref
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/
LOCAL uint32 CAFSOCKET_AddRef( ISOCKET_T *socket_ptr );

/*****************************************************************************/
//  Description : socket release
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/
LOCAL uint32 CAFSOCKET_Release( ISOCKET_T *socket_ptr );

/*****************************************************************************/
//  Description : CAF encapsulation of Accept
//  Global resource dependence : 
//  Author:James.Zhang
//  Modify:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFSOCKET_Accept(ISOCKET_T *socket_ptr, ISOCKET_T **socket_pptr);

/*****************************************************************************/
//  Description : CAF encapsulation of Bind
//  Global resource dependence : 
//  Author:James.Zhang
//  Modify:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFSOCKET_Bind( ISOCKET_T* socket_ptr, ISOCKET_ADDR_T* addr_ptr, int32 addr_len );

/*****************************************************************************/
//  Description : CAF encapsulation of CloseSocket
//  Global resource dependence : 
//  Author:James.Zhang
//  Modify:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFSOCKET_CloseSocket( ISOCKET_T* socket_ptr );

/*****************************************************************************/
//  Description : CAF encapsulation of Connect
//  Global resource dependence : 
//  Author:James.Zhang
//  Modify:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFSOCKET_Connect( ISOCKET_T* socket_ptr, ISOCKET_ADDR_T* addr_ptr, int32 addr_len );

/*****************************************************************************/
//  Description : CAF encapsulation of GetPeerName
//  Global resource dependence : 
//  Author:James.Zhang
//  Modify:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFSOCKET_GetPeerName( ISOCKET_T* socket_ptr, ISOCKET_ADDR_T* addr_ptr, int32* addr_len_ptr );

/*****************************************************************************/
//  Description : CAF encapsulation of GetSockName
//  Global resource dependence : 
//  Author:James.Zhang
//  Modify:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFSOCKET_GetSockName( ISOCKET_T* socket_ptr, ISOCKET_ADDR_T* addr_ptr, int32* addr_len_ptr );

/*****************************************************************************/
//  Description : CAF encapsulation of GetSockOpt
//  Global resource dependence : 
//  Author:James.Zhang
//  Modify:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFSOCKET_GetSockOpt( ISOCKET_T* socket_ptr, int32 level, int32 opt_name, char* opt_ptr, int32* opt_len_ptr );

/*****************************************************************************/
//  Description : CAF encapsulation of Listen
//  Global resource dependence : 
//  Author:James.Zhang
//  Modify:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFSOCKET_Listen( ISOCKET_T* socket_ptr, int32 back_log );

/*****************************************************************************/
//  Description : CAF encapsulation of Recv
//  Global resource dependence : 
//  Author:James.Zhang
//  Modify:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFSOCKET_Recv( ISOCKET_T* socket_ptr, char* buf_ptr, int32 len, int32 flag );

/*****************************************************************************/
//  Description : CAF encapsulation of RecvFrom
//  Global resource dependence : 
//  Author:James.Zhang
//  Modify:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFSOCKET_RecvFrom( ISOCKET_T* socket_ptr, char* buf_ptr, int32 len, int32 flag, ISOCKET_ADDR_T* from_ptr, int32* from_len_ptr );

/*****************************************************************************/
//  Description : CAF encapsulation of Send
//  Global resource dependence : 
//  Author:James.Zhang
//  Modify:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFSOCKET_Send( ISOCKET_T* socket_ptr, const char *buf_ptr, int32 len, int32 flag );

/*****************************************************************************/
//  Description : CAF encapsulation of SendTo
//  Global resource dependence : 
//  Author:James.Zhang
//  Modify:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFSOCKET_SendTo( ISOCKET_T* socket_ptr, const char* buf_ptr, int32 len, int32 flag, const ISOCKET_ADDR_T* to_ptr, int32 to_len );

/*****************************************************************************/
//  Description : CAF encapsulation of SetSockOpt
//  Global resource dependence : 
//  Author:James.Zhang
//  Modify:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFSOCKET_SetSockOpt( ISOCKET_T* socket_ptr, int32 level, int32 opt_name, const char *opt_ptr, int32 opt_len );

/*****************************************************************************/
//  Description : CAF encapsulation of Shutdown
//  Global resource dependence : 
//  Author:James.Zhang
//  Modify:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFSOCKET_Shutdown( ISOCKET_T* socket_ptr, int32 how );

/*****************************************************************************/
//  Description : CAF encapsulation of GetLastError
//  Global resource dependence : 
//  Author:James.Zhang
//  Modify:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFSOCKET_GetLastError( ISOCKET_T* socket_ptr );

/*****************************************************************************/
//  Description : Convert system asynchronous select notify message to CAF
//  Global resource dependence : 
//  Author:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL uint16 Convert_sci_notify_to_caf(ISOCKET_T *socket_ptr, uint16 sci_asyn_msg);

/*****************************************************************************/
//  Description : Convert CAF registered event to destination asynchronous event
//  Global resource dependence : 
//  Author:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL uint32 Convert_caf_to_sci_asyn_event(ISOCKET_T *socket_ptr, CAF_HANDLE_T dest_handle, uint16 sci_asyn_msg);

/*****************************************************************************/
//  Description : Convert CAF registered event to destination asynchronous event
//  Global resource dependence : 
//  Author:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFSOCKET_RegAsynSocketEvent(ISOCKET_T *socket_ptr, CAF_HANDLE_T dest_handle, uint16 msg_id);

/*****************************************************************************/
//  Description : Create ASYN task, task to socket one by one.
//  Global resource dependence : 
//  Author:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL uint32 CAFSOCKET_CreateASYNTask(INETMGR_T *socket_ptr);

/*****************************************************************************/
//  Description : Delete ASYN task.
//  Global resource dependence : 
//  Author:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL uint32 CAFSOCKET_DeleteASYNTask(INETMGR_T *netmgr_ptr);

/*****************************************************************************/
//  Description : In ASYN task, send the message and data to APP task.
//  Global resource dependence : 
//  Author:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL uint32 CAFSOCKET_AsyncSendDataToApp(ISOCKET_T *socket_ptr, MMI_MESSAGE_ID_E msg_id);

/*****************************************************************************/
//  Description : ASYN task process process.
//  Global resource dependence : 
//  Author:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL void CAFSOCKET_ASYNTask_Proc(uint32 argc, void* argv);

/*****************************************************************************/
//  Description : process ps signal.
//  Global resource dependence : 
//  Author:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E CAFSOCKET_HandlePsMsg( PWND app_ptr, uint16 msg_id, DPARAM param);

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
// functions table

LOCAL const CAF_VTBL_T(INETMGR_T) s_netmgr_vtbl =
{
    CAFINETMGR_QueryInterface,
    CAFINETMGR_AddRef,
    CAFINETMGR_Release,
    CAFINETMGR_GetHostByName,
    CAFINETMGR_OpenSocket,
    CAFINETMGR_Select,
    CAFINETMGR_GetHostName
};

LOCAL const CAF_VTBL_T(ISOCKET_T) s_socket_vtbl =
{
    CAFSOCKET_QueryInterface,
    CAFSOCKET_AddRef,
    CAFSOCKET_Release,
    CAFSOCKET_Accept,
    CAFSOCKET_Bind,
    CAFSOCKET_CloseSocket,
    CAFSOCKET_Connect,
    CAFSOCKET_GetPeerName,
    CAFSOCKET_GetSockName,
    CAFSOCKET_GetSockOpt,
    CAFSOCKET_GetLastError,
    PNULL,      //IOCtl
    CAFSOCKET_Listen,
    CAFSOCKET_Recv,
    CAFSOCKET_RecvFrom,
    CAFSOCKET_Send,
    CAFSOCKET_SendTo,
    CAFSOCKET_SetSockOpt,
    CAFSOCKET_Shutdown,
    CAFSOCKET_RegAsynSocketEvent
};

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
//global variable declare
PUBLIC MMI_APPLICATION_T       g_caf_asyn_app = {CAFSOCKET_HandlePsMsg, CT_APPLICATION, PNULL};       // g_caf_asyn_app define

/*****************************************************************************/
//  Description : convert
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void ConvertCAFFdSetToSCI(
                                sci_fd_set* sci_fd_set_ptr,
                                const ISOCKET_FD_SET_T* fd_set_ptr
                                )
{
    uint32  i;

    if( PNULL == fd_set_ptr )
    {
        return;
    }
    
    sci_fd_set_ptr->fd_count = MIN( ISOCKET_FD_SETSIZE, fd_set_ptr->fd_count );//底层定义的fd最大值为12
    
    for(i = 0; i < sci_fd_set_ptr->fd_count; i++)
    {
        sci_fd_set_ptr->fd_array[i] = ((CAFSOCKET_T *)fd_set_ptr->fd_array[i])->s;
    }
}

LOCAL void ConvertSCIToCAF(
                            sci_fd_set* sci_fd_set_ptr,
                            ISOCKET_FD_SET_T* fd_set_ptr,
                            ISOCKET_FD_SET_T* dst_fd_set_ptr
                            )
{
    uint32  i, j, k;
    
    if( PNULL == fd_set_ptr )
    {
        return;
    }
    
    k = 0;
    
    for(i = 0; i < sci_fd_set_ptr->fd_count; i++)
    {
        for(j = 0; j < fd_set_ptr->fd_count; j++)
        {
            if(sci_fd_set_ptr->fd_array[i] == ((CAFSOCKET_T *)fd_set_ptr->fd_array[j])->s)
            {
                dst_fd_set_ptr->fd_array[k++] = fd_set_ptr->fd_array[j];
            }
        }
    }
    dst_fd_set_ptr->fd_count = k;
}




LOCAL const CAF_STATIC_CLASS_T s_netmgr_class_list[] = 
{
    { SPRD_GUID_NETMGR,      CAFINETMGR_ClassNew,       0 },
};

const CAF_STATIC_CLASS_INFO_T g_netmgr_static_class_info = 
{ 
    (CAF_STATIC_CLASS_T*)s_netmgr_class_list, 
    sizeof(s_netmgr_class_list) / sizeof(s_netmgr_class_list[0])
};

/*****************************************************************************/
//  Description : register created socket, for searching
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL uint32 CAFINETMGR_RegCreatedSocket(CAFNETMGR_T *cafnetmgr_ptr, CAFSOCKET_T *cafsocket_ptr)
{
    int32   i;

    for(i = 0; i < MAX_SOCKET_NUMS; i++)
    {
        if(cafnetmgr_ptr->s_socket_ptr[i] == NULL)
        {
            cafnetmgr_ptr->s_socket_ptr[i] = cafsocket_ptr;
            break;
        }
    }
    SCI_ASSERT( i < MAX_SOCKET_NUMS);

    return TRUE;
}

/*****************************************************************************/
//  Description : unregister created socket
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL uint32 CAFINETMGR_UnRegCreatedSocket(CAFNETMGR_T *cafnetmgr_ptr, CAFSOCKET_T *cafsocket_ptr)
{
    int32   i;
    
    for(i = 0; i < MAX_SOCKET_NUMS; i++)
    {
        if(cafnetmgr_ptr->s_socket_ptr[i] == cafsocket_ptr)
        {
            cafnetmgr_ptr->s_socket_ptr[i] = NULL;
            break;
        }
    }
    SCI_ASSERT( i < MAX_SOCKET_NUMS);

    return TRUE;
}

/*****************************************************************************/
//  Description : search socket class pointer for socket
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL CAFSOCKET_T *CAFINETMGR_GetSocketPtrFroms(CAFNETMGR_T *cafnetmgr_ptr, long s)
{
    int32       i;
    CAFSOCKET_T *cafsocket_ptr = NULL;

    for(i = 0; i < MAX_SOCKET_NUMS; i++)
    {
        if(cafnetmgr_ptr->s_socket_ptr[i] && (cafnetmgr_ptr->s_socket_ptr[i]->s == s))
        {
            cafsocket_ptr = cafnetmgr_ptr->s_socket_ptr[i];
            break;
        }
    }

    return cafsocket_ptr;
}

/*****************************************************************************/
//  Description : search socket class pointer by signal
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL CAFSOCKET_T *CAFINETMGR_GetSocketPtrFromSignal(CAFNETMGR_T *cafnetmgr_ptr, MmiSignalS* sig_ptr)
{
    CAFSOCKET_T                             *cafsocket_ptr       = PNULL;
    SOCKET_CONNECTION_CLOSE_EVENT_IND_SIG_T *close_ind_sig_ptr   = PNULL;
    SOCKET_CONNECT_EVENT_IND_SIG_T          *connect_ind_sig_ptr = PNULL;
    SOCKET_WRITE_EVENT_IND_SIG_T            *write_ind_sig_ptr   = PNULL;
    SOCKET_READ_EVENT_IND_SIG_T             *read_ind_sig_ptr    = PNULL;
    SOCKET_ACCEPT_EVENT_IND_SIG_T           *accept_ind_sig_ptr  = PNULL;

    switch(sig_ptr->signalCode)
    {
        case SOCKET_READ_EVENT_IND:
            read_ind_sig_ptr = (SOCKET_READ_EVENT_IND_SIG_T *)sig_ptr;
            cafsocket_ptr    = CAFINETMGR_GetSocketPtrFroms(cafnetmgr_ptr, read_ind_sig_ptr->socket_id);
            break;
        case SOCKET_WRITE_EVENT_IND:
            write_ind_sig_ptr = (SOCKET_WRITE_EVENT_IND_SIG_T *)sig_ptr;
            cafsocket_ptr    = CAFINETMGR_GetSocketPtrFroms(cafnetmgr_ptr, write_ind_sig_ptr->socket_id);
            break;
        case SOCKET_CONNECT_EVENT_IND:
            connect_ind_sig_ptr = (SOCKET_CONNECT_EVENT_IND_SIG_T *)sig_ptr;
            cafsocket_ptr    = CAFINETMGR_GetSocketPtrFroms(cafnetmgr_ptr, connect_ind_sig_ptr->socket_id);
            break;
        case SOCKET_CONNECTION_CLOSE_EVENT_IND:
            close_ind_sig_ptr = (SOCKET_CONNECTION_CLOSE_EVENT_IND_SIG_T *)sig_ptr;
            cafsocket_ptr    = CAFINETMGR_GetSocketPtrFroms(cafnetmgr_ptr, close_ind_sig_ptr->socket_id);
            break;
        case SOCKET_ACCEPT_EVENT_IND:
            accept_ind_sig_ptr = (SOCKET_ACCEPT_EVENT_IND_SIG_T *)sig_ptr;
            cafsocket_ptr    = CAFINETMGR_GetSocketPtrFroms(cafnetmgr_ptr, accept_ind_sig_ptr->socket_id);
            break;
        default:
            break;
    }

    return cafsocket_ptr;
}


/*****************************************************************************/
//  Description : new netmgr control
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL CAF_RESULT_E CAFINETMGR_ClassNew( 
                                      CAF_GUID_T guid, 
                                      void**     object_pptr 
                                      )
{
    CAF_RESULT_E  result  = CAF_RESULT_NOT_SUPPORT;
    CAFNETMGR_T   *cafnetmgr_ptr = PNULL;
    
    if( SPRD_GUID_NETMGR == guid )
    {
        cafnetmgr_ptr = SCI_ALLOC_APP(sizeof(CAFNETMGR_T));
        
        SCI_MEMSET((char *)cafnetmgr_ptr, 0, sizeof(CAFNETMGR_T));
        cafnetmgr_ptr->vtbl_ptr = (CAF_VTBL_T(INETMGR_T)*)&s_netmgr_vtbl;
        
        cafnetmgr_ptr->guid       = guid;
        cafnetmgr_ptr->ref_num    = 1;
        CAFSOCKET_CreateASYNTask((INETMGR_T *)cafnetmgr_ptr);
        
        result = CAF_RESULT_SUCCEEDED;
    }
    *object_pptr = cafnetmgr_ptr;
    return result;
}

/*****************************************************************************/
//  Description : netmgr query interface
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/
LOCAL CAF_RESULT_E CAFINETMGR_QueryInterface(INETMGR_T * netmgr_ptr, CAF_GUID_T guid, void ** object_pptr )
{
    CAF_RESULT_E result = CAF_RESULT_FAILED;
    CAFNETMGR_T *cafnetmgr_ptr = (CAFNETMGR_T *)netmgr_ptr;
    
    if ( IUNKNOWN_ID == guid ||  cafnetmgr_ptr->guid == guid )
    {
        *object_pptr = netmgr_ptr;
        
        result = CAF_RESULT_SUCCEEDED;
    }
    if( result )
    {
        IUNKNOWN_AddRef( *object_pptr );
    }
    return result;
}

/*****************************************************************************/
//  Description : netmgr add ref
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/
LOCAL uint32 CAFINETMGR_AddRef(INETMGR_T * netmgr_ptr)
{
    SCI_ASSERT(netmgr_ptr);
    return ++(((CAFNETMGR_T *)netmgr_ptr)->ref_num);
}

/*****************************************************************************/
//  Description : netmgr release
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/
LOCAL uint32 CAFINETMGR_Release(INETMGR_T * netmgr_ptr)
{
    CAFNETMGR_T * cafnetmgr_ptr = (CAFNETMGR_T *)netmgr_ptr;

    SCI_ASSERT(netmgr_ptr);
    if ( --cafnetmgr_ptr->ref_num )
    {
        return( cafnetmgr_ptr->ref_num );
    }
    else
    {
        CAFSOCKET_DeleteASYNTask(netmgr_ptr);
        while(TRUE)/*lint !e716*/
        {
            if(!cafnetmgr_ptr->thread_start)
            {
                break;
            }
            SCI_Sleep(100);
        }
        SCI_FREE( cafnetmgr_ptr );   
        return 0;
    }
}


/*****************************************************************************/
//  Description : CAF encapsulation of GetHostByName
//  Global resource dependence : 
//  Author:James.Zhang
//  Modify:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL struct hostent * CAFINETMGR_GetHostByName(INETMGR_T * netmgr_ptr, const char * sz_ptr)
{
#ifdef WIN32
    struct hostent *pHost;
#endif
    struct sci_hostent *pSciHost;

    SCI_ASSERT(netmgr_ptr);
    SCI_ASSERT(((CAFNETMGR_T *)netmgr_ptr)->ref_num);

    /* the hostent is the same as the sci_hostent */
    #ifdef WIN32
    pHost = gethostbyname(sz_ptr);
    memcpy(&s_hostname, pHost, sizeof(struct hostent));
    #else
    pSciHost = sci_gethostbyname((char *)sz_ptr);
    s_hostname.h_name = (char *)sz_ptr;
    s_hostname.h_aliases = pSciHost->h_aliases;     /* we don't do the aliases */
    s_hostname.h_addrtype = pSciHost->h_addrtype;   /* AF_INET */
    s_hostname.h_length = pSciHost->h_length;       /* length of IP address */
    s_hostname.h_addr_list = pSciHost->h_addr_list;
    #endif

    return &s_hostname;
}

/*****************************************************************************/
//  Description : CAF encapsulation of Select
//  Global resource dependence : 
//  Author:James.Zhang
//  Modify:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFINETMGR_Select(INETMGR_T * netmgr_ptr, int32 nfds, ISOCKET_FD_SET_T* in_ptr, ISOCKET_FD_SET_T* out_ptr, ISOCKET_FD_SET_T* ex_ptr, int32 timeout )
{
    int32              ret;
    ISOCKET_FD_SET_T rev_in;
    ISOCKET_FD_SET_T rev_out;
    ISOCKET_FD_SET_T rev_ex;
    sci_fd_set sci_in  = {0};
    sci_fd_set sci_out = {0};
    sci_fd_set sci_ex  = {0};

    UNUSED(nfds);
    SCI_ASSERT(netmgr_ptr);
    SCI_ASSERT(((CAFNETMGR_T *)netmgr_ptr)->ref_num);
    if(in_ptr)
    {
        SCI_MEMCPY((char *)&rev_in,  (void *)in_ptr, sizeof(ISOCKET_FD_SET_T));
        ConvertCAFFdSetToSCI( &sci_in, in_ptr );
    }
    if(out_ptr)
    {
        SCI_MEMCPY((char *)&rev_out, (void *)out_ptr, sizeof(ISOCKET_FD_SET_T));
        ConvertCAFFdSetToSCI( &sci_out, out_ptr );
    }
    if(ex_ptr)
    {
        SCI_MEMCPY((char *)&rev_ex,  (void *)ex_ptr, sizeof(ISOCKET_FD_SET_T));
        ConvertCAFFdSetToSCI( &sci_ex, ex_ptr );
    }
    
    /* millisecond need convert to tickets */
    #ifdef WIN32
    {
        struct timeval st_timeout, *ptime;
        if(timeout == -1)
        {
            ptime = NULL;
        }
        else
        {
            ptime = &st_timeout;
            ptime->tv_sec = timeout/1000;
            ptime->tv_usec = (timeout%1000)*1000;
        }
        ret = select(0, in_ptr ? (fd_set *)&sci_in : NULL , out_ptr ? (fd_set *)&sci_out : NULL, ex_ptr ? (fd_set *)&sci_ex : NULL, ptime);
    }
    #else
    ret = sci_sock_select(in_ptr ? &sci_in : NULL , out_ptr ? &sci_out : NULL, ex_ptr ? &sci_ex : NULL, timeout);
    #endif

    if(ret > 0)
    {
        if(in_ptr)
        {
            ConvertSCIToCAF(&sci_in, &rev_in, in_ptr);
        }
        if(out_ptr)
        {
            ConvertSCIToCAF(&sci_out, &rev_out, out_ptr);
        }
        if(ex_ptr)
        {
            ConvertSCIToCAF(&sci_ex, &rev_ex, ex_ptr);
        }
    }

    return ret;
}

/*****************************************************************************/
//  Description : CAF encapsulation of OpenSocket
//  Global resource dependence : 
//  Author:James.Zhang
//  Modify:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL ISOCKET_T * CAFINETMGR_OpenSocket( INETMGR_T *netmgr_ptr, int32 family, int32 type, int32 protocol)
{
    CAFSOCKET_T *cafsocketctrl_ptr;
    CAFNETMGR_T *cafnetmgr_ptr;

    SCI_ASSERT(netmgr_ptr);
    SCI_ASSERT(((CAFNETMGR_T *)netmgr_ptr)->ref_num);

    cafnetmgr_ptr = (CAFNETMGR_T *)netmgr_ptr;
    cafsocketctrl_ptr = (CAFSOCKET_T *)SCI_ALLOC_APP(sizeof(CAFSOCKET_T));
    SCI_MEMSET(cafsocketctrl_ptr, 0, sizeof(CAFSOCKET_T));

    cafsocketctrl_ptr->vtbl_ptr = (CAF_VTBL_T(ISOCKET_T)*)&s_socket_vtbl;
    #ifdef WIN32
    cafsocketctrl_ptr->s = socket( family, type, protocol );
    #else
    /* cr152116, sci_sock_socket should add net_id as the 4th parameter
     * to work compatable with multi-net interface, here just set with 
     * default value 0!
     * NOTE that this is just a temporary amend, the default value 0 may 
     * cause socket transfer error when more than one net interface setup, 
     * the right way is to get net_id from GPRS after PDP actived and 
     * set it when create sockett! */
    cafsocketctrl_ptr->s = sci_sock_socket( family, type, protocol, 0 );
    #endif
    cafsocketctrl_ptr->guid       = SPRD_GUID_SOCKET;
    cafsocketctrl_ptr->ref_num    = 1;

    #ifdef  WIN32
    cafsocketctrl_ptr->hWin = cafnetmgr_ptr->hWin;
    #else
    cafsocketctrl_ptr->async_task_id = cafnetmgr_ptr->async_task_id;
    #endif

    cafsocketctrl_ptr->cafnetmgr_ptr = cafnetmgr_ptr;
    CAFINETMGR_RegCreatedSocket(cafnetmgr_ptr, cafsocketctrl_ptr);

    return (ISOCKET_T *)cafsocketctrl_ptr;
}



/*****************************************************************************/
//  Description : CAF encapsulation of GetSockOpt
//  Global resource dependence : 
//  Author:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFINETMGR_GetHostName(INETMGR_T * netmgr_ptr, char * sz_ptr, int32 namelen)
{
    int32 iRet;
    
    SCI_ASSERT(netmgr_ptr);
    SCI_ASSERT(((CAFNETMGR_T *)netmgr_ptr)->ref_num);
    
    /* the hostent is the same as the sci_hostent */
    #ifdef WIN32
    iRet = gethostname( sz_ptr, namelen);
    #else
    iRet = sci_gethostname( sz_ptr, namelen);
    #endif
    
    return iRet;
}


/*****************************************************************************/
//  Description : structure convert between CAF and SCI
//  Global resource dependence : 
//  Author:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL void ConvertCAFAddrToSCI(
                               struct sci_sockaddr* sci_addr_ptr,
                               const ISOCKET_ADDR_T* addr_ptr
                               )
{
    SCI_ASSERT( PNULL != addr_ptr && PNULL != sci_addr_ptr );
    
    sci_addr_ptr->family  = addr_ptr->family;
    sci_addr_ptr->port    = addr_ptr->port;
    sci_addr_ptr->ip_addr = addr_ptr->ip_addr;
    
    SCI_MEMCPY( sci_addr_ptr->sa_data, addr_ptr->sa_data, 8 * sizeof(char) );
}

LOCAL void ConvertCAFSCIToAddr(
                               ISOCKET_ADDR_T* addr_ptr,
                               struct sci_sockaddr* sci_addr_ptr
                               )
{
    SCI_ASSERT( PNULL != addr_ptr && PNULL != sci_addr_ptr );
    
    addr_ptr->family = sci_addr_ptr->family;
    addr_ptr->port   = sci_addr_ptr->port;
    addr_ptr->ip_addr = sci_addr_ptr->ip_addr;
    
    SCI_MEMCPY( addr_ptr->sa_data, sci_addr_ptr->sa_data, 8 * sizeof(char) );
}

/*****************************************************************************/
//  Description : socket query interface
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/
LOCAL CAF_RESULT_E CAFSOCKET_QueryInterface( ISOCKET_T *socket_ptr, CAF_GUID_T guid, void ** object_pptr ) 
{
    CAF_RESULT_E result = CAF_RESULT_FAILED;
    CAFSOCKET_T* cafsocket_ptr = (CAFSOCKET_T*)socket_ptr;
    
    if ( IUNKNOWN_ID == guid ||  cafsocket_ptr->guid == guid )
    {
        *object_pptr = socket_ptr;
        result = CAF_RESULT_SUCCEEDED;
    }
    
    if( result )
    {
        IUNKNOWN_AddRef( *object_pptr );
    }
    
    return result;
}

/*****************************************************************************/
//  Description : socket add ref
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/
LOCAL uint32 CAFSOCKET_AddRef( ISOCKET_T *socket_ptr ) 
{
    SCI_ASSERT(socket_ptr);
    return ++(((CAFSOCKET_T *)socket_ptr)->ref_num);
}

/*****************************************************************************/
//  Description : socket release
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/
LOCAL uint32 CAFSOCKET_Release( ISOCKET_T *socket_ptr ) 
{
    CAFSOCKET_T * cafsocket_ptr = (CAFSOCKET_T *)socket_ptr;
    
    SCI_ASSERT(socket_ptr);
    if ( --cafsocket_ptr->ref_num )
    {
        return( cafsocket_ptr->ref_num );
    }
    else
    {
        CAFINETMGR_UnRegCreatedSocket(cafsocket_ptr->cafnetmgr_ptr, cafsocket_ptr);
        SCI_ASSERT(cafsocket_ptr->s == -1);
        SCI_FREE( cafsocket_ptr );   
        return 0;
    }
}

/*****************************************************************************/
//  Description : CAF encapsulation of Accept
//  Global resource dependence : 
//  Author:James.Zhang
//  Modify:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFSOCKET_Accept(ISOCKET_T *socket_ptr, ISOCKET_T **socket_pptr)
{
    int         addr_len;
    CAFSOCKET_T *cafsocketctrl_ptr;
    CAFSOCKET_T *cafsocketaceptctrl_ptr;
    struct sci_sockaddr sci_addr = {0};
    
    SCI_ASSERT(socket_ptr);
    SCI_ASSERT(((CAFSOCKET_T *)socket_ptr)->ref_num);
    
    addr_len = sizeof(sci_addr);
    cafsocketctrl_ptr = (CAFSOCKET_T *)socket_ptr;

    /* create the accept socket */
    cafsocketaceptctrl_ptr = (CAFSOCKET_T *)SCI_ALLOC_APP(sizeof(CAFSOCKET_T));
    SCI_MEMSET(cafsocketaceptctrl_ptr, 0, sizeof(CAFSOCKET_T));

    cafsocketaceptctrl_ptr->vtbl_ptr = (CAF_VTBL_T(ISOCKET_T)*)&s_socket_vtbl;
    #ifdef WIN32
    cafsocketaceptctrl_ptr->s = accept(cafsocketctrl_ptr->s, NULL, NULL);
    #else
    cafsocketaceptctrl_ptr->s = sci_sock_accept(cafsocketctrl_ptr->s, &sci_addr, &addr_len);
    #endif
    if(-1 == cafsocketaceptctrl_ptr->s)
    {
        SCI_FREE(cafsocketaceptctrl_ptr);
        *socket_pptr = NULL;
        return ISOCKET_ERROR;
    }
    cafsocketaceptctrl_ptr->guid       = SPRD_GUID_SOCKET;
    cafsocketaceptctrl_ptr->ref_num    = 1;
    #ifdef  WIN32
    cafsocketaceptctrl_ptr->hWin = cafsocketctrl_ptr->hWin;
    #else
    cafsocketaceptctrl_ptr->async_task_id = cafsocketctrl_ptr->async_task_id;
    #endif

    cafsocketaceptctrl_ptr->cafnetmgr_ptr = cafsocketctrl_ptr->cafnetmgr_ptr;
    *socket_pptr = (ISOCKET_T *)cafsocketaceptctrl_ptr;
    CAFINETMGR_RegCreatedSocket(cafsocketctrl_ptr->cafnetmgr_ptr, cafsocketaceptctrl_ptr);

    return ISOCKET_SUCCESS;
}

/*****************************************************************************/
//  Description : CAF encapsulation of Bind
//  Global resource dependence : 
//  Author:James.Zhang
//  Modify:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFSOCKET_Bind( ISOCKET_T* socket_ptr, ISOCKET_ADDR_T* addr_ptr, int32 addr_len )
{
    int32    iRet;
    struct sci_sockaddr sci_addr = {0};

    SCI_ASSERT(socket_ptr);
    SCI_ASSERT(((CAFSOCKET_T *)socket_ptr)->ref_num);
    
    ConvertCAFAddrToSCI( &sci_addr, addr_ptr );
    #ifdef WIN32
    iRet = bind( ((CAFSOCKET_T *)socket_ptr)->s, (struct sockaddr *)&sci_addr, addr_len );
    #else
    iRet = sci_sock_bind( ((CAFSOCKET_T *)socket_ptr)->s, &sci_addr, addr_len );
    #endif

    return iRet;
}

/*****************************************************************************/
//  Description : CAF encapsulation of CloseSocket
//  Global resource dependence : 
//  Author:James.Zhang
//  Modify:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFSOCKET_CloseSocket( ISOCKET_T* socket_ptr )
{
    int32 iRet;
    CAFSOCKET_T *cafsocket_ptr;

    SCI_ASSERT(socket_ptr);
    SCI_ASSERT(((CAFSOCKET_T *)socket_ptr)->ref_num);

    cafsocket_ptr = (CAFSOCKET_T *)socket_ptr;

    #ifdef WIN32
    iRet = closesocket( ((CAFSOCKET_T *)socket_ptr)->s );
    #else
    iRet = sci_sock_socketclose( ((CAFSOCKET_T *)socket_ptr)->s );
    #endif
    if(ISOCKET_SUCCESS == iRet)
    {
        cafsocket_ptr->s = -1;
    }

    return iRet;
}

/*****************************************************************************/
//  Description : CAF encapsulation of Connect
//  Global resource dependence : 
//  Author:James.Zhang
//  Modify:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFSOCKET_Connect( ISOCKET_T* socket_ptr, ISOCKET_ADDR_T* addr_ptr, int32 addr_len )
{
    int32     iRet;
    struct sci_sockaddr sci_addr = {0};
    
    SCI_ASSERT(socket_ptr);
    SCI_ASSERT(((CAFSOCKET_T *)socket_ptr)->ref_num);
    ConvertCAFAddrToSCI( &sci_addr, addr_ptr );
    #ifdef WIN32
    iRet = connect( ((CAFSOCKET_T *)socket_ptr)->s, (struct sockaddr *)&sci_addr, addr_len );
    #else
    iRet = sci_sock_connect( ((CAFSOCKET_T *)socket_ptr)->s, &sci_addr, addr_len );
    #endif    

    return iRet;
}

/*****************************************************************************/
//  Description : CAF encapsulation of GetPeerName
//  Global resource dependence : 
//  Author:James.Zhang
//  Modify:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFSOCKET_GetPeerName( ISOCKET_T* socket_ptr, ISOCKET_ADDR_T* addr_ptr, int32* addr_len_ptr )
{
    int32     iRet;
    struct sci_sockaddr sci_addr = {0};
    
    SCI_ASSERT(socket_ptr);
    SCI_ASSERT(((CAFSOCKET_T *)socket_ptr)->ref_num);
    ConvertCAFAddrToSCI( &sci_addr, addr_ptr );

    if(addr_len_ptr)
    {
        *addr_len_ptr = sizeof(ISOCKET_ADDR_T);
    }

    #ifdef WIN32
    iRet = getpeername( ((CAFSOCKET_T *)socket_ptr)->s, (struct sockaddr *)&sci_addr, (int *)addr_len_ptr);
    #else
    iRet = sci_sock_getpeername( ((CAFSOCKET_T *)socket_ptr)->s, &sci_addr );
    #endif

    return iRet;
}

/*****************************************************************************/
//  Description : CAF encapsulation of GetSockName
//  Global resource dependence : 
//  Author:James.Zhang
//  Modify:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFSOCKET_GetSockName( ISOCKET_T* socket_ptr, ISOCKET_ADDR_T* addr_ptr, int32* addr_len_ptr )
{
    int32     iRet;
    struct sci_sockaddr sci_addr = {0};
    
    SCI_ASSERT(socket_ptr);
    SCI_ASSERT(((CAFSOCKET_T *)socket_ptr)->ref_num);
    ConvertCAFAddrToSCI( &sci_addr, addr_ptr );

    if(addr_len_ptr)
    {
        *addr_len_ptr = sizeof(ISOCKET_ADDR_T);
    }

    #ifdef WIN32
    iRet = getsockname( ((CAFSOCKET_T *)socket_ptr)->s, (struct sockaddr *)&sci_addr, (int *)addr_len_ptr );
    #else
    iRet = sci_sock_getsockname( ((CAFSOCKET_T *)socket_ptr)->s, &sci_addr );
    #endif

    return iRet;
}

/*****************************************************************************/
//  Description : CAF encapsulation of GetSockOpt
//  Global resource dependence : 
//  Author:James.Zhang
//  Modify:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFSOCKET_GetSockOpt( ISOCKET_T* socket_ptr, int32 level, int32 opt_name, char* opt_ptr, int32* opt_len_ptr )
{
    int32  iRet;

    SCI_ASSERT(socket_ptr);
    SCI_ASSERT(((CAFSOCKET_T *)socket_ptr)->ref_num);

    #ifdef WIN32
    iRet = getsockopt( ((CAFSOCKET_T *)socket_ptr)->s, level, opt_name, opt_ptr, (int *)opt_len_ptr);
    #else
    iRet = sci_sock_getsockopt( ((CAFSOCKET_T *)socket_ptr)->s, opt_name, (void*)opt_ptr );
    #endif
    return iRet;
}

/*****************************************************************************/
//  Description : CAF encapsulation of Listen
//  Global resource dependence : 
//  Author:James.Zhang
//  Modify:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFSOCKET_Listen( ISOCKET_T* socket_ptr, int32 back_log )
{
    int32     iRet;

    SCI_ASSERT(socket_ptr);
    SCI_ASSERT(((CAFSOCKET_T *)socket_ptr)->ref_num);
    #ifdef WIN32
    iRet = listen( ((CAFSOCKET_T *)socket_ptr)->s, back_log );
    #else
    iRet = sci_sock_listen( ((CAFSOCKET_T *)socket_ptr)->s, back_log );
    #endif

    return iRet;
}

/*****************************************************************************/
//  Description : CAF encapsulation of Recv
//  Global resource dependence : 
//  Author:James.Zhang
//  Modify:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFSOCKET_Recv( ISOCKET_T* socket_ptr, char* buf_ptr, int32 len, int32 flag )
{
    int32     iRet;
    SCI_ASSERT(socket_ptr);
    SCI_ASSERT(((CAFSOCKET_T *)socket_ptr)->ref_num);

    #ifdef WIN32
    iRet = recv( ((CAFSOCKET_T *)socket_ptr)->s, buf_ptr, len, flag );
    #else
    iRet = sci_sock_recv( ((CAFSOCKET_T *)socket_ptr)->s, buf_ptr, len, flag );
    #endif

    return iRet;
}

/*****************************************************************************/
//  Description : CAF encapsulation of RecvFrom
//  Global resource dependence : 
//  Author:James.Zhang
//  Modify:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFSOCKET_RecvFrom( ISOCKET_T* socket_ptr, char* buf_ptr, int32 len, int32 flag, ISOCKET_ADDR_T* from_ptr, int32* from_len_ptr )
{
    int32     iRet;
    struct sci_sockaddr sci_addr = {0};
    
    SCI_ASSERT(socket_ptr);
    SCI_ASSERT(((CAFSOCKET_T *)socket_ptr)->ref_num);
    ConvertCAFAddrToSCI( &sci_addr, from_ptr );
    
    #ifdef WIN32
    iRet = recvfrom( ((CAFSOCKET_T *)socket_ptr)->s, buf_ptr, len, flag, (struct sockaddr *)&sci_addr, (int *)from_len_ptr );
    #else
    iRet = sci_sock_recvfrom( ((CAFSOCKET_T *)socket_ptr)->s, buf_ptr, len, flag, &sci_addr );
    #endif

    ConvertCAFSCIToAddr(from_ptr, &sci_addr);

    return iRet;
}


/*****************************************************************************/
//  Description : CAF encapsulation of Send
//  Global resource dependence : 
//  Author:James.Zhang
//  Modify:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFSOCKET_Send( ISOCKET_T* socket_ptr, const char *buf_ptr, int32 len, int32 flag )
{
    int32     iRet;

    SCI_ASSERT(socket_ptr);
    SCI_ASSERT(((CAFSOCKET_T *)socket_ptr)->ref_num);

    #ifdef WIN32
    iRet = send( ((CAFSOCKET_T *)socket_ptr)->s, (char*)buf_ptr, len, flag );
    #else
    iRet = sci_sock_send( ((CAFSOCKET_T *)socket_ptr)->s, (char*)buf_ptr, len, flag );
    #endif

    return iRet;
}

/*****************************************************************************/
//  Description : CAF encapsulation of SendTo
//  Global resource dependence : 
//  Author:James.Zhang
//  Modify:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFSOCKET_SendTo( ISOCKET_T* socket_ptr, const char* buf_ptr, int32 len, int32 flag, const ISOCKET_ADDR_T* to_ptr, int32 to_len )
{
    int32     iRet;
    struct sci_sockaddr sci_addr = {0};
    
    SCI_ASSERT(socket_ptr);
    SCI_ASSERT(((CAFSOCKET_T *)socket_ptr)->ref_num);
    ConvertCAFAddrToSCI( &sci_addr, to_ptr );

    #ifdef WIN32
    iRet = sendto( ((CAFSOCKET_T *)socket_ptr)->s, (char*)buf_ptr, len, flag, (struct sockaddr *)&sci_addr, to_len);
    #else
    iRet = sci_sock_sendto( ((CAFSOCKET_T *)socket_ptr)->s, (char*)buf_ptr, len, flag, &sci_addr );
    #endif

    return iRet;
}

/*****************************************************************************/
//  Description : CAF encapsulation of SetSockOpt
//  Global resource dependence : 
//  Author:James.Zhang
//  Modify:Wei.zhou
//  Note:
/*****************************************************************************/
#ifdef WIN32
/* by wei.zhou, defined by windows, for pc simulator, no self-defined MACRO */
#define IOCPARM_MASK    0x7f            /* parameters must be < 128 bytes */
#define IOC_IN          0x80000000      /* copy in parameters */
#define _IOW(x,y,t)     (long)(IOC_IN|(((long)sizeof(t)&IOCPARM_MASK)<<16)|((x)<<8)|(y))
#define FIONBIO     _IOW('f', 126, uint32) /* set/clear non-blocking i/o */

#endif

LOCAL int32 CAFSOCKET_SetSockOpt( ISOCKET_T* socket_ptr, int32 level, int32 opt_name, const char *opt_ptr, int32 opt_len )
{
    int32     iRet;
    UNUSED(level);
    UNUSED(opt_len);

    SCI_ASSERT(socket_ptr);
    SCI_ASSERT(((CAFSOCKET_T *)socket_ptr)->ref_num);

    #ifdef WIN32
    if((opt_name == ISOCKET_SO_BIO) || (opt_name == ISOCKET_SO_NBIO))
    {
        uint32  iMode;
        if(opt_name == ISOCKET_SO_BIO)
        {
            iMode = 0;
        }
        else
        {
            iMode = 1;
        }

        iRet = ioctlsocket(((CAFSOCKET_T *)socket_ptr)->s, FIONBIO, &iMode);
    }
    else
    {
        iRet = setsockopt( ((CAFSOCKET_T *)socket_ptr)->s, level, opt_name, opt_ptr, opt_len);
    }
    #else
    iRet = sci_sock_setsockopt( ((CAFSOCKET_T *)socket_ptr)->s, opt_name, (void*)opt_ptr );
    #endif

    return iRet;
}

/*****************************************************************************/
//  Description : CAF encapsulation of Shutdown
//  Global resource dependence : 
//  Author:James.Zhang
//  Modify:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFSOCKET_Shutdown( ISOCKET_T* socket_ptr, int32 how )
{
    int32     iRet;

    SCI_ASSERT(socket_ptr);
    SCI_ASSERT(((CAFSOCKET_T *)socket_ptr)->ref_num);

    #ifdef WIN32
    iRet = shutdown( ((CAFSOCKET_T *)socket_ptr)->s, how );
    #else
    iRet = sci_sock_shutdown( ((CAFSOCKET_T *)socket_ptr)->s, how );
    #endif

    return iRet;
}

/***********************************************************************************/
//  Description : Convert the socket errors(both MOCOR and WIN32), to CAF net error
//  Global resource dependence : 
//  Author:Wei.zhou
//  Note:
/***********************************************************************************/
LOCAL int32 ConvertSocketErrToCAF(int32 sock_err)
{
    int32   caf_err;

    switch(sock_err)
    {
#ifdef WIN32
        case WSAENOBUFS:
            caf_err = ISOCKET_ENOBUFS;
            break;
        case WSAETIMEDOUT:
            caf_err = ISOCKET_ETIMEDOUT;
            break;
        case WSAEISCONN:
            caf_err = ISOCKET_EISCONN;
            break;
        case WSAEOPNOTSUPP:
            caf_err = ISOCKET_EOPNOTSUPP;
            break;
        case WSAECONNABORTED:
            caf_err = ISOCKET_ECONNABORTED;
            break;
        case WSAEWOULDBLOCK:
            caf_err = ISOCKET_EWOULDBLOCK;
            break;
        case WSAECONNREFUSED:
            caf_err = ISOCKET_ECONNREFUSED;
            break;
        case WSAECONNRESET:
            caf_err = ISOCKET_ECONNRESET;
            break;
        case WSAENOTCONN:
            caf_err = ISOCKET_ENOTCONN;
            break;
        case WSAEALREADY:
            caf_err = ISOCKET_EALREADY;
            break;
        case WSAEINVAL:
            caf_err = ISOCKET_EINVAL;
            break;
        case WSAEMSGSIZE:
            caf_err = ISOCKET_EMSGSIZE;
            break;
        case WSAEDESTADDRREQ:
            caf_err = ISOCKET_EDESTADDRREQ;
            break;
        case WSAESHUTDOWN:
            caf_err = ISOCKET_ESHUTDOWN;
            break;
        case WSAENOPROTOOPT:
            caf_err = ISOCKET_ENOPROTOOPT;
            break;
        case WSAEADDRNOTAVAIL:
            caf_err = ISOCKET_EADDRNOTAVAIL;
            break;
        case WSAEADDRINUSE:
            caf_err = ISOCKET_EADDRINUSE;
            break;
        case WSAEAFNOSUPPORT:
            caf_err = ISOCKET_EAFNOSUPPORT;
            break;
        case WSAEINPROGRESS:
            caf_err = ISOCKET_EINPROGRESS;
            break;
        case WSAENOTSOCK:
            caf_err = ISOCKET_ENOTSOCK;
            break;
        case WSAETOOMANYREFS:
            caf_err = ISOCKET_ETOOMANYREFS;
            break;
        case WSAEFAULT:
            caf_err = ISOCKET_EFAULT;
            break;
        case WSAENETUNREACH:
            caf_err = ISOCKET_ENETUNREACH;
            break;
#else
        case ENOBUFS:
            caf_err = ISOCKET_ENOBUFS;
            break;
        case ETIMEDOUT:
            caf_err = ISOCKET_ETIMEDOUT;
            break;
        case EISCONN:
            caf_err = ISOCKET_EISCONN;
            break;
        case EOPNOTSUPP:
            caf_err = ISOCKET_EOPNOTSUPP;
            break;
        case ECONNABORTED:
            caf_err = ISOCKET_ECONNABORTED;
            break;
        case EWOULDBLOCK:
            caf_err = ISOCKET_EWOULDBLOCK;
            break;
        case ECONNREFUSED:
            caf_err = ISOCKET_ECONNREFUSED;
            break;
        case ECONNRESET:
            caf_err = ISOCKET_ECONNRESET;
            break;
        case ENOTCONN:
            caf_err = ISOCKET_ENOTCONN;
            break;
        case EALREADY:
            caf_err = ISOCKET_EALREADY;
            break;
        case EINVAL:
            caf_err = ISOCKET_EINVAL;
            break;
        case EMSGSIZE:
            caf_err = ISOCKET_EMSGSIZE;
            break;
        case EPIPE:
            caf_err = ISOCKET_EPIPE;
            break;
        case EDESTADDRREQ:
            caf_err = ISOCKET_EDESTADDRREQ;
            break;
        case ESHUTDOWN:
            caf_err = ISOCKET_ESHUTDOWN;
            break;
        case ENOPROTOOPT:
            caf_err = ISOCKET_ENOPROTOOPT;
            break;
        case EHAVEOOB:
            caf_err = ISOCKET_EHAVEOOB;
            break;
        case ENOMEM:
            caf_err = ISOCKET_ENOMEM;
            break;
        case EADDRNOTAVAIL:
            caf_err = ISOCKET_EADDRNOTAVAIL;
            break;
        case EADDRINUSE:
            caf_err = ISOCKET_EADDRINUSE;
            break;
        case EAFNOSUPPORT:
            caf_err = ISOCKET_EAFNOSUPPORT;
            break;
        case EINPROGRESS:
            caf_err = ISOCKET_EINPROGRESS;
            break;
        case ELOWER:
            caf_err = ISOCKET_ELOWER;
            break;
        case ENOTSOCK:
            caf_err = ISOCKET_ENOTSOCK;
            break;
        case EIEIO:
            caf_err = ISOCKET_EIEIO;
            break;
        case ETOOMANYREFS:
            caf_err = ISOCKET_ETOOMANYREFS;
            break;
        case EFAULT:
            caf_err = ISOCKET_EFAULT;
            break;
        case ENETUNREACH:
            caf_err = ISOCKET_ENETUNREACH;
            break;
#endif
        default:
            caf_err = ISOCKET_UNKNOWERR;
            break;
    }

    return caf_err;
}


/*****************************************************************************/
//  Description : CAF encapsulation of GetLastError
//  Global resource dependence : 
//  Author:James.Zhang
//  Modify:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFSOCKET_GetLastError( ISOCKET_T* socket_ptr )
{
    int32     iRet;

    SCI_ASSERT(socket_ptr);
    SCI_ASSERT(((CAFSOCKET_T *)socket_ptr)->ref_num);

#ifdef WIN32
    iRet = WSAGetLastError();
#else
    iRet = sci_sock_errno(((CAFSOCKET_T *)socket_ptr)->s);
#endif    
    
    return ConvertSocketErrToCAF(iRet);
}

/*****************************************************************************/
//  Description : Convert system asynchronous select notify message to CAF
//  Global resource dependence : 
//  Author:Wei.zhou
//  Note:
/*****************************************************************************/
uint16 Convert_sci_notify_to_caf(ISOCKET_T *socket_ptr, uint16 sci_asyn_msg)
{
    CAFSOCKET_T * cafsocket_ptr = (CAFSOCKET_T *)socket_ptr;

#ifdef WIN32
    switch(sci_asyn_msg)
    {
        case FD_READ:
            cafsocket_ptr->msg_id = MSG_ASYN_SOCKET_READ_IND;
            cafsocket_ptr->hcafwin = cafsocket_ptr->hregread;
            break;
        case FD_WRITE:
            cafsocket_ptr->msg_id = MSG_ASYN_SOCKET_WRITE_IND;
            cafsocket_ptr->hcafwin = cafsocket_ptr->hregwrite;
            break;
        case FD_CONNECT:
            cafsocket_ptr->msg_id = MSG_ASYN_SOCKET_CONNECT_IND;
            cafsocket_ptr->hcafwin = cafsocket_ptr->hregconnect;
            break;
        case FD_CLOSE:
            cafsocket_ptr->msg_id = MSG_ASYN_SOCKET_CLOSE_IND;
            cafsocket_ptr->hcafwin = cafsocket_ptr->hregclose;
            break;
        case FD_ACCEPT:
            cafsocket_ptr->msg_id = MSG_ASYN_SOCKET_ACCEPT_IND;
            cafsocket_ptr->hcafwin = cafsocket_ptr->hregaccept;
            break;
        default:
            SCI_ASSERT(0);
            break;
    }
#else
    switch(sci_asyn_msg)
    {
        case SOCKET_READ_EVENT_IND:
            cafsocket_ptr->msg_id = MSG_ASYN_SOCKET_READ_IND;
            cafsocket_ptr->hcafwin = cafsocket_ptr->hregread;
            break;
        case SOCKET_WRITE_EVENT_IND:
            cafsocket_ptr->msg_id = MSG_ASYN_SOCKET_WRITE_IND;
            cafsocket_ptr->hcafwin = cafsocket_ptr->hregwrite;
            break;
        case SOCKET_CONNECT_EVENT_IND:
            cafsocket_ptr->msg_id = MSG_ASYN_SOCKET_CONNECT_IND;
            cafsocket_ptr->hcafwin = cafsocket_ptr->hregconnect;
            break;
        case SOCKET_CONNECTION_CLOSE_EVENT_IND:
            cafsocket_ptr->msg_id = MSG_ASYN_SOCKET_CLOSE_IND;
            cafsocket_ptr->hcafwin = cafsocket_ptr->hregclose;
            break;
        case SOCKET_ACCEPT_EVENT_IND:
            cafsocket_ptr->msg_id = MSG_ASYN_SOCKET_ACCEPT_IND;
            cafsocket_ptr->hcafwin = cafsocket_ptr->hregaccept;
            break;
        default:
            SCI_ASSERT(0);
            break;
    }
#endif
    return  cafsocket_ptr->msg_id;
}

/*****************************************************************************/
//  Description : Convert CAF registered event to destination asynchronous event
//  Global resource dependence : 
//  Author:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL uint32 Convert_caf_to_sci_asyn_event(ISOCKET_T *socket_ptr, CAF_HANDLE_T dest_handle, uint16 sci_asyn_msg)
{
    CAFSOCKET_T * cafsocket_ptr = (CAFSOCKET_T *)socket_ptr;

#ifdef WIN32
    switch(sci_asyn_msg)
    {
        case MSG_ASYN_SOCKET_READ_IND:
            if(dest_handle)
            {
                cafsocket_ptr->regevent |= FD_READ;
                cafsocket_ptr->hregread  = dest_handle;
            }
            else
            {
                cafsocket_ptr->regevent &= ~FD_READ;
                cafsocket_ptr->hregread  = 0;
            }
            break;
        case MSG_ASYN_SOCKET_WRITE_IND:
            if(dest_handle)
            {
                cafsocket_ptr->regevent |= FD_WRITE;
                cafsocket_ptr->hregwrite  = dest_handle;
            }
            else
            {
                cafsocket_ptr->regevent &= ~FD_WRITE;
                cafsocket_ptr->hregwrite  = 0;
            }
            break;
        case MSG_ASYN_SOCKET_CONNECT_IND:
            if(dest_handle)
            {
                cafsocket_ptr->regevent |= FD_CONNECT;
                cafsocket_ptr->hregconnect  = dest_handle;
            }
            else
            {
                cafsocket_ptr->regevent &= ~FD_CONNECT;
                cafsocket_ptr->hregconnect  = 0;
            }
            break;
        case MSG_ASYN_SOCKET_CLOSE_IND:
            if(dest_handle)
            {
                cafsocket_ptr->regevent |= FD_CLOSE;
                cafsocket_ptr->hregclose  = dest_handle;
            }
            else
            {
                cafsocket_ptr->regevent &= ~FD_CLOSE;
                cafsocket_ptr->hregclose  = 0;
            }
            break;
        case MSG_ASYN_SOCKET_ACCEPT_IND:
            cafsocket_ptr->regevent |= FD_ACCEPT;
            if(dest_handle)
            {
                cafsocket_ptr->regevent |= FD_ACCEPT;
                cafsocket_ptr->hregaccept  = dest_handle;
            }
            else
            {
                cafsocket_ptr->regevent &= ~FD_ACCEPT;
                cafsocket_ptr->hregaccept  = 0;
            }
            break;
        default:
            SCI_ASSERT(0);
            break;
    }
#else
    switch(sci_asyn_msg)
    {
        case MSG_ASYN_SOCKET_READ_IND:
            if(dest_handle)
            {
                cafsocket_ptr->regevent |= AS_READ;
                cafsocket_ptr->hregread  = dest_handle;
            }
            else
            {
                cafsocket_ptr->regevent &= ~AS_READ;
                cafsocket_ptr->hregread  = 0;
            }
            break;
        case MSG_ASYN_SOCKET_WRITE_IND:
            if(dest_handle)
            {
                cafsocket_ptr->regevent |= AS_WRITE;
                cafsocket_ptr->hregwrite  = dest_handle;
            }
            else
            {
                cafsocket_ptr->regevent &= ~AS_WRITE;
                cafsocket_ptr->hregwrite  = 0;
            }
            break;
        case MSG_ASYN_SOCKET_CONNECT_IND:
            if(dest_handle)
            {
                cafsocket_ptr->regevent |= AS_CONNECT;
                cafsocket_ptr->hregconnect  = dest_handle;
            }
            else
            {
                cafsocket_ptr->regevent &= ~AS_CONNECT;
                cafsocket_ptr->hregconnect  = 0;
            }
            break;
        case MSG_ASYN_SOCKET_CLOSE_IND:
            cafsocket_ptr->regevent |= AS_CLOSE;
            if(dest_handle)
            {
                cafsocket_ptr->regevent |= AS_CLOSE;
                cafsocket_ptr->hregclose  = dest_handle;
            }
            else
            {
                cafsocket_ptr->regevent &= ~AS_CLOSE;
                cafsocket_ptr->hregclose  = 0;
            }
            break;
        case MSG_ASYN_SOCKET_ACCEPT_IND:
            cafsocket_ptr->regevent |= AS_ACCEPT;
            if(dest_handle)
            {
                cafsocket_ptr->regevent |= AS_ACCEPT;
                cafsocket_ptr->hregaccept  = dest_handle;
            }
            else
            {
                cafsocket_ptr->regevent &= ~AS_ACCEPT;
                cafsocket_ptr->hregaccept  = 0;
            }
            break;
        default:
            SCI_ASSERT(0);
            break;
    }
#endif
    return  cafsocket_ptr->regevent;
}

/*****************************************************************************/
//  Description : Convert CAF registered event to destination asynchronous event
//  Global resource dependence : 
//  Author:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFSOCKET_RegAsynSocketEvent(ISOCKET_T *socket_ptr, CAF_HANDLE_T dest_handle, uint16 msg_id)
{
    int32         result;
    uint32        regevent;
    CAFSOCKET_T * cafsocket_ptr = (CAFSOCKET_T *)socket_ptr;

    regevent = Convert_caf_to_sci_asyn_event(socket_ptr, dest_handle, msg_id);

    //SCI_TRACE_LOW:"CAFSOCKET_RegAsynSocketEvent regevent = %lx"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CAFSOCKET_1826_112_2_17_23_1_1_2,(uint8*)"d", regevent);
    #ifdef  WIN32
    result = WSAAsyncSelect(cafsocket_ptr->s, cafsocket_ptr->hWin, WM_SOCKET, regevent);
    #else
    result = sci_sock_asyncselect(cafsocket_ptr->s, cafsocket_ptr->async_task_id, regevent);
    #endif

    if(result != 0)
    {
        result = ISOCKET_ERROR;
    }
    
    return result;
}


/*****************************************************************************/
//  Description : Create ASYN task, task to socket one by one.
//  Global resource dependence : 
//  Author:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL uint32 CAFSOCKET_CreateASYNTask(INETMGR_T *netmgr_ptr)
{
    CAFNETMGR_T * cafnetmgr_ptr = (CAFNETMGR_T *)netmgr_ptr;
    
    cafnetmgr_ptr->async_task_id = SCI_CreateThread(
    "T_ASYN_TASK",
    "Q_ASYN_TASK",
    CAFSOCKET_ASYNTask_Proc,
    1,
    (void *)netmgr_ptr,
    P_ASYN_TASK_STACK_SIZE,
    P_ASYN_TASK_QUEUE_NUM,
    P_ASYN_TASK_PRIORITY,
    TRUE,
    TRUE);

    while(TRUE)/*lint !e716*/
    {
        if(cafnetmgr_ptr->thread_start)
        {
            break;
        }
        SCI_Sleep(100);
    }

    return ISOCKET_SUCCESS;
}

/*****************************************************************************/
//  Description : Delete ASYN task.
//  Global resource dependence : 
//  Author:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL uint32 CAFSOCKET_DeleteASYNTask(INETMGR_T *netmgr_ptr)
{
    CAFNETMGR_T * cafnetmgr_ptr = (CAFNETMGR_T *)netmgr_ptr;
    xSignalHeaderRec* psig = PNULL;

    SCI_ASSERT(cafnetmgr_ptr->thread_start);
    #ifdef WIN32
    PostMessage(cafnetmgr_ptr->hWin, WM_CLOSE, 0, 0);
    #else
    if(SCI_INVALID_BLOCK_ID != cafnetmgr_ptr->async_task_id)
    {
        SCI_CREATE_SIGNAL(psig,APP_WASYN_MSG_IND, sizeof(MmiOtherTaskMsgS), SCI_IdentifyThread());
        SCI_SEND_SIGNAL(psig, cafnetmgr_ptr->async_task_id);
    }
    #endif
    return 0;
}

/*****************************************************************************/
//  Description : In ASYN task, send the message and data to APP task.
//  Global resource dependence : 
//  Author:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL uint32 CAFSOCKET_AsyncSendDataToApp(ISOCKET_T *socket_ptr, MMI_MESSAGE_ID_E msg_id)
{
    MmiSignalS *sendSignal = PNULL;
    CAFSOCKET_T * cafsocket_ptr = (CAFSOCKET_T *)socket_ptr;
    
    MmiCreateSignal(APP_WASYN_MSG_IND, sizeof(MmiOtherTaskMsgS), &sendSignal);

    sendSignal->send = SCI_IdentifyThread();

    sendSignal->sig.otherTaskMsg.handle         = cafsocket_ptr->hcafwin;
    sendSignal->sig.otherTaskMsg.msg_id         = cafsocket_ptr->msg_id;
    sendSignal->sig.otherTaskMsg.size_of_param  = sizeof(ISOCKET_T *);

    sendSignal->sig.otherTaskMsg.param_ptr = SCI_ALLOC_APP(sendSignal->sig.otherTaskMsg.size_of_param);
    *(uint32 *)(sendSignal->sig.otherTaskMsg.param_ptr) = (uint32)socket_ptr;

    MmiSendSignal(P_APP, sendSignal);
    
    return ISOCKET_SUCCESS;
}

/*****************************************************************************/
//  Description : ASYN task process process.
//  Global resource dependence : 
//  Author:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL void CAFSOCKET_ASYNTask_Proc(uint32 argc, void* argv)
{
    #ifdef WIN32
    int                    nErr;
    MSG                    msg;
    HWND                   hWin;
    WNDCLASS               wndclass;
    CHAR ProviderClass[] = "AsyncSelect";
    #endif
    MMI_MESSAGE_ID_E        msg_id = 0;
    xSignalHeaderRec*       sig_ptr = PNULL;
    CAFNETMGR_T*            cafnetmgr_ptr = (CAFNETMGR_T*)argv;
    CAFSOCKET_T*            cafsocket_ptr;
    BLOCK_ID                thead_id = 0;

    cafnetmgr_ptr->thread_start = TRUE;
    #ifdef WIN32
    if((hWin = CAFSOCKET_CreateWindow(argv, ProviderClass, &wndclass)) == NULL)
    {
        SCI_ASSERT(hWin);
    }
    cafnetmgr_ptr->hWin = hWin;

    while(GetMessage(&msg,NULL,0,0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    UnregisterClass(ProviderClass, wndclass.hInstance);
    #else
    while(TRUE)/*lint !e716*/
    {
        thead_id = SCI_IdentifyThread();
        SCI_RECEIVE_SIGNAL(sig_ptr, thead_id);
        if(APP_WASYN_MSG_IND == ((MmiSignalS *)sig_ptr)->signalCode)
        {
            SCI_FREE(sig_ptr);
            break;
        }
        cafsocket_ptr = CAFINETMGR_GetSocketPtrFromSignal(cafnetmgr_ptr, (MmiSignalS *)sig_ptr);
        if(cafsocket_ptr)
        {
            msg_id = Convert_sci_notify_to_caf((ISOCKET_T *)cafsocket_ptr, ((MmiSignalS *)sig_ptr)->signalCode);
            CAFSOCKET_AsyncSendDataToApp((ISOCKET_T *)cafsocket_ptr, msg_id);
        }
        //SCI_TRACE_LOW:"CAFSOCKET_ASYNTask_Proc signalCode=%lx, sig_ptr=%lx, msg_id=%lx"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,CAFSOCKET_1970_112_2_17_23_1_1_3,(uint8*)"ddd", ((MmiSignalS *)sig_ptr)->signalCode, sig_ptr, msg_id);
        SCI_FREE(sig_ptr);
    }
    cafnetmgr_ptr->async_task_id = SCI_INVALID_BLOCK_ID;
    #endif
    cafnetmgr_ptr->thread_start = FALSE;
    //SCI_TRACE_LOW:"CAFSOCKET_ASYNTask_Proc exit! sig_ptr = %lx"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CAFSOCKET_1976_112_2_17_23_1_1_4,(uint8*)"d", sig_ptr);
    SCI_ThreadExit();
}


#ifdef  WIN32

/*****************************************************************************/
//  Description : In Windows, Create the window, for the ASYN message carrier.
//  Global resource dependence : 
//  Author:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL HWND CAFSOCKET_CreateWindow(LPVOID argv, CHAR *lpszClassName, WNDCLASS *lpWndCls)
{
    WNDCLASS wndclass= {0};
    HWND hWin;
    int  nErr;

    wndclass.style = CS_HREDRAW;
    wndclass.lpfnWndProc = (WNDPROC)CAFSOCKET_WindowProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = GetModuleHandle(NULL);
    wndclass.hIcon = NULL;
    wndclass.hCursor = NULL;
    wndclass.hbrBackground = NULL;
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = lpszClassName;
    
    if(RegisterClass(&wndclass) == 0)
    {
        nErr = GetLastError();
        return NULL;
    }
    
    if((hWin = CreateWindow(lpszClassName, "", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, NULL, argv)) == NULL)
    {
        return NULL;
    }

    SetWindowLong(hWin, GWL_USERDATA, (DWORD)argv);

    *lpWndCls = wndclass;

    return hWin;
}

/*****************************************************************************/
//  Description : In Windows, the asynchronous select window process.
//  Global resource dependence : 
//  Author:Wei.zhou
//  Note:
/*****************************************************************************/

LOCAL LRESULT CALLBACK CAFSOCKET_WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam,LPARAM lParam)
{
    ISOCKET_T        *socket_ptr;
    MMI_MESSAGE_ID_E msg_id;
    LRESULT          ret = 0;
    CAFNETMGR_T      *cafnetmgr_ptr = NULL;

    switch(uMsg)
    {
        case WM_SOCKET:
            if(WSAGETSELECTERROR(lParam))
            {
                //SCI_TRACE_LOW:"Socket failed\r\n"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,CAFSOCKET_2039_112_2_17_23_1_1_5,(uint8*)"");
            }
            else
            {
                cafnetmgr_ptr = (CAFNETMGR_T *)GetWindowLong(hwnd, GWL_USERDATA);
                socket_ptr = (ISOCKET_T*)CAFINETMGR_GetSocketPtrFroms(cafnetmgr_ptr, wParam);
                if(socket_ptr)
                {
                    msg_id = Convert_sci_notify_to_caf(socket_ptr, (uint16)lParam);
                    CAFSOCKET_AsyncSendDataToApp(socket_ptr, msg_id);
                }
            }
            break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            ret = DefWindowProc(hwnd,uMsg,wParam,lParam);
            break;
    }

    return ret;
}

#endif

/*****************************************************************************/
//  Description : process ps signal.
//  Global resource dependence : 
//  Author:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E CAFSOCKET_HandlePsMsg( PWND app_ptr, uint16 msg_id, DPARAM param)
{
    MMI_RESULT_E      result = MMI_RESULT_FALSE;
    MmiOtherTaskMsgS* signal_ptr = (MmiOtherTaskMsgS*)param;
    
    UNUSED(param);
    
    switch( msg_id )
    {
        case APP_WASYN_MSG_IND:
            SCI_ASSERT( PNULL != signal_ptr );
            //SCI_TRACE_LOW:"CAFSOCKET_HandlePsMsg, signal_ptr->msg_id = %lx"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,CAFSOCKET_2083_112_2_17_23_1_1_6,(uint8*)"d", signal_ptr->msg_id);
            ISHELL_SendMessage( signal_ptr->handle, signal_ptr->msg_id, signal_ptr->param_ptr);
            if(signal_ptr->param_ptr)
            {
                SCI_FREE(signal_ptr->param_ptr);
            }
            break;
        default:
            break;
    }
    
    return result;
}
#endif