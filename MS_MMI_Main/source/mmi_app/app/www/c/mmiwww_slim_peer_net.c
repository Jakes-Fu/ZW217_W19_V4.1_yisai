/****************************************************************************
** File Name:      mmiwww_peer_net.c                                            *
** Author:                                                                    *
** Date:           06/15/2009                                                *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:    This file is used to describe the slim net peer for WWW Browser            *
*****************************************************************************
**                         Important Edit History                            *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                *
** 06/2009        wancan.you        Create
** 
****************************************************************************/
#define _MMIWWW_SLIM_PEER_NET_C_  

/**-------------------------------------------------------------------------*
**                         Dependencies                                     *
**--------------------------------------------------------------------------*/
#include "mmi_app_www_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
#include "mmk_app.h"
#include "socket_api.h"
#include "mmiwww_internal.h"
#include "mmiwww_func.h"
#include "mmiwww_netfront.h"

#ifdef BROWSER_SUPPORT_NETFRONT
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define DEFAULT_SOCKET_TV     (0)

#define NETFRONT3_DEVICE_MAX_STREAM  (8)

/* network */
#define SOCKET_STATE_DISCONNECT 0x00
#define SOCKET_STATE_CONNECT    0x01
#define SOCKET_STATE_IOSLEEP    0x02
#define SOCKET_STATE_OPEN       0x04
#define SOCKET_STATE_ERROR      0x08
#define SOCKET_STATE_CONNECTED  0x10
#define SOCKET_STATE_NOTUSED    0x80

/*bind state*/
#define SOCKET_UNBOUND          0x00
#define SOCKET_BIND_OK          0x01
#define SOCKET_BIND_IOSLEEP     0x02
#define SOCKET_BIND_ERROR       0x04

/*realize state*/
#define SOCKET_UNREALIZE        0x00
#define SOCKET_REALIZE_OK       0x01
#define SOCKET_REALIZE_IOSLEEP  0x02
#define SOCKET_REALIZE_ERROR    0x04

/*listen state*/
#define SOCKET_UNLISTEN         0x00
#define SOCKET_LISTEN_OK        0x01
#define SOCKET_LISTEN_IOSLEEP   0x02
#define SOCKET_LISTEN_ERROR     0x04

#define ESUCCESS       0

#ifdef SLIM_CONFIG_USE_DNS
#define DNS_MAX_DOMAINNAME 255
#define GetTicket(self, id) &((DNSTicket*)self)[(id)]

#define MMIWWW_DNS_TIMEOUT 20000
#endif

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef struct SocketObj_
{
    slim_int fSocket;
    slim_int fState;
    slim_ip_addr fIPAddr;
    slim_int fPort;   
} SocketObj;

typedef struct sci_sockaddr SockAddr;

#ifdef SLIM_CONFIG_USE_DNS
enum
{
    DNS_DORMANT,
    DNS_REALIZING,
    DNS_REQUESTING,
    DNS_RESOLVED,
    DNS_ERROR,
    DNS_TIMEOUT
};

typedef struct _DNSTicket
{
    slim_word fResult;
    slim_char fName[DNS_MAX_DOMAINNAME];
    slim_bool fUse;
    slim_int fState;
    slim_int fError;
    TDNSWakeUpCallbackProc fDnsWakeUpProc;
    slim_opaque fcallback_aux;
    slim_int fTicket_id;
}DNSTicket;

struct DNSContext_
{
    DNSTicket fTicket[MMIWWW_DNS_TICKET_MAX];
};

typedef struct DNSContext_ DNSContext;
#endif

/**--------------------------------------------------------------------------*
 **                          EXTERN DECLARE                                *
 **--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                          GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL SocketObj s_sockets[NETFRONT3_DEVICE_MAX_STREAM] = {0};

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get unused sock object
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL slim_int _net_get_unused_sockobj(void);

/*****************************************************************************/
//  Description : get sock object
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL SocketObj *_net_sockobj(slim_int in_desc);

/*****************************************************************************/
//  Description : release sock object
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL void _net_release_sockobj(slim_int in_desc);

/*****************************************************************************/
//  Description : dup sock object
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL void _net_dup_ip_addr(slim_int in_desc, slim_ip_addr *in_addr, slim_int in_port);

/*****************************************************************************/
//  Description : socket open
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL slim_int SocketOpenPeer(slim_int in_domain, slim_int type);

/*****************************************************************************/
//  Description : socket Close
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL void SocketClosePeer(slim_int in_desc);

/*****************************************************************************/
//  Description : Checks whether reading/writing data is possible
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL slim_int SocketCanReadWritePeer(slim_int in_desc, slim_int in_rw);

/*****************************************************************************/
//  Description : Establishes connection with specified IP address
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL slim_int SocketConnectPeer(slim_int in_desc, slim_ip_addr *in_addr, slim_int in_port);

/*****************************************************************************/
//  Description : sci error to netfront error
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL slim_int sci_err_to_netfront_err(int in_err);

/*****************************************************************************/
//  Description : ip address convert to sock address
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ipaddr_to_sockaddr(SockAddr *inout_addr,slim_ip_addr *in_addr, slim_int in_port);

/*****************************************************************************/
//  Description : socket receive
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL slim_int SocketReceivePeer(slim_int in_desc, slim_byte *out_buf, slim_int in_len);

/*****************************************************************************/
//  Description : socket send
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL slim_int SocketSendPeer(slim_int in_desc, slim_byte *in_buf, slim_int in_len);

/*****************************************************************************/
//  Description : socket bind
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL slim_int SocketBindPeer(slim_int in_desc,slim_ip_addr *in_addr,slim_int in_port);

/*****************************************************************************/
//  Description : UDP Send to Addr
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL slim_int UDPSendToPeer(slim_int in_desc, void *in_buf, slim_int in_len, slim_ip_addr *in_addr, slim_int in_port);

/*****************************************************************************/
//  Description : UDP Receive From Addr
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL slim_int UDPRecvFromPeer(slim_int in_desc, void *out_buf, slim_int in_len, slim_ip_addr *out_addr, slim_int *out_port);

#ifdef SLIM_CONFIG_USE_DNS
/*****************************************************************************/
//  Description : Get Room Ticket ID
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL slim_int GetRoomTicketID(TDNSPeer self);
#endif

/*****************************************************************************/
//  Description : Get Signal Struct
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL void GetSignalStruct(DPARAM param, void *signal_struct_ptr, uint16 struct_size);

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get unused sock object
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL slim_int _net_get_unused_sockobj(void)
{
    slim_int index = 0;
    SocketObj *object = &(s_sockets[0]);

    while (index < NETFRONT3_DEVICE_MAX_STREAM)
    {
        if( object->fState == SOCKET_STATE_NOTUSED )
        {
            return (index);
        }
        index++;
        object++;
    }

    //SCI_TRACE_LOW:"_net_get_unused_sockobj socket num exceed"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_NET_245_112_2_18_3_11_53_128,(uint8*)"");

    return -1;
}

/*****************************************************************************/
//  Description : get sock object
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL SocketObj *_net_sockobj(slim_int in_desc)
{
    slim_int index = in_desc;

    if (index < NETFRONT3_DEVICE_MAX_STREAM)
    {
        return &(s_sockets[index]);
    }

    return slim_nil;
}

/*****************************************************************************/
//  Description : release sock object
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL void _net_release_sockobj(slim_int in_desc)
{
    SocketObj *obj = _net_sockobj(in_desc);

    //SCI_TRACE_LOW:"_net_release_sockobj in_desc=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_NET_274_112_2_18_3_11_53_129,(uint8*)"d",in_desc);

    if(slim_nil != obj)
    {
        obj->fSocket = -1;
        obj->fState  = SOCKET_STATE_NOTUSED;
        obj->fPort  = -1;
        SCI_MEMSET(obj->fIPAddr.fAddr,0x00,SLIM_IP_ADDR_LEN_V6);
        obj->fIPAddr.fType = SLIM_IP_ADDR_TYPE_V4;
    }
}

/*****************************************************************************/
//  Description : dup sock object
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL void _net_dup_ip_addr(slim_int in_desc, slim_ip_addr *in_addr, slim_int in_port)
{
    SocketObj *obj = _net_sockobj(in_desc);
    //SCI_TRACE_LOW:"_net_dup_ip_addr in_desc=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_NET_293_112_2_18_3_11_53_130,(uint8*)"d",in_desc);

    if (slim_nil != obj)
    { 
        if (-1 == obj->fPort)
        {
            obj->fPort = in_port;
            SCI_MEMCPY(obj->fIPAddr.fAddr, in_addr->fAddr, SLIM_IP_ADDR_LEN_V6);
            obj->fIPAddr.fType = in_addr->fType;
        }
    }
}

/*****************************************************************************/
//  Description : socket open
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL slim_int SocketOpenPeer(slim_int in_domain, slim_int type)
{
    long sock = 0;
    int32 e = 0;
    int result = 0;

    //SCI_TRACE_LOW:"SocketOpenPeer: start in_domain =%d, type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_NET_316_112_2_18_3_11_53_131,(uint8*)"dd", in_domain, type);
    if (SLIM_IP_ADDR_TYPE_V4 == in_domain)
    {
        MMIWWW_INSTANCE_T *self = MMIWWW_INSTANCE();
        if (PNULL == self)
        {
            return SLIM_E_GENERIC;
        }
        
        sock = sci_sock_socket(AF_INET, (int)type, 0, self->pdp_info.net_id);
    }
    else
    {
        //SCI_TRACE_LOW:"SocketOpenPeer, only support SLIM_IP_ADDR_TYPE_V4"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_NET_329_112_2_18_3_11_54_132,(uint8*)"");

        return SLIM_E_GENERIC;
    }

    if (sock == -1)
    {
        e = sci_sock_errno((long)sock);
        //SCI_TRACE_LOW:"SocketOpenPeer, e = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_NET_337_112_2_18_3_11_54_133,(uint8*)"d", e);

        return SLIM_E_GENERIC;
    }
    else
    {
        result = sci_sock_setsockopt(sock, SO_NBIO, PNULL);

        if (result < 0)
        {
            sci_sock_socketclose(sock);
            //SCI_TRACE_LOW:"sci_sock_setsockopt, result=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_NET_348_112_2_18_3_11_54_134,(uint8*)"d", result);

            return SLIM_E_GENERIC;
        }
    }
    //SCI_TRACE_LOW:"SocketOpenPeer, return %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_NET_353_112_2_18_3_11_54_135,(uint8*)"d", (slim_int)sock);

    return (slim_int)sock;
}

/*****************************************************************************/
//  Description : socket Close
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL void SocketClosePeer(slim_int in_desc)
{
    //SCI_TRACE_LOW:"SocketClosePeer, close sokcet: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_NET_364_112_2_18_3_11_54_136,(uint8*)"d", in_desc);

    sci_sock_socketclose((long)in_desc);
}

/*****************************************************************************/
//  Description : Checks whether reading/writing data is possible
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL slim_int SocketCanReadWritePeer(slim_int in_desc, slim_int in_rw)
{
    sci_fd_set fds = {0};
    sci_fd_set wfds = {0};
    int selected = 0;

    SCI_FD_ZERO(&fds);
    SCI_FD_SET((long)in_desc, &fds);

    switch (in_rw)
    {
    case SLIM_STREAM_READ:
        selected = sci_sock_select(&fds, PNULL, PNULL, DEFAULT_SOCKET_TV /* 100 */);

        if (selected > 0)
        {
            selected = SLIM_STREAM_READ;
        }
        break;

    case SLIM_STREAM_WRITE:
        selected = sci_sock_select(PNULL, &fds, PNULL, DEFAULT_SOCKET_TV /* 100 */);

        if (selected > 0)
        {
            selected = SLIM_STREAM_WRITE;
        }
        break;

    default:
        SCI_FD_ZERO(&wfds);
        SCI_FD_SET((long)in_desc, &wfds);
        selected = sci_sock_select(&fds, &wfds, PNULL, DEFAULT_SOCKET_TV /* 100 */);

        if (2 == selected)
        {
            selected = SLIM_STREAM_READWRITE;
        }
        else
        {
            if (SCI_FD_ISSET((long)in_desc, &fds))
            {
                selected = SLIM_STREAM_READ;
            }
            else if (SCI_FD_ISSET((long)in_desc, &wfds))
            {
                selected = SLIM_STREAM_WRITE;
            }
        }
        break;
    }

    //SCI_TRACE_LOW:"SocketCanReadWritePeer, s=%d, return %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_NET_425_112_2_18_3_11_54_137,(uint8*)"dd", in_desc, selected);

    return selected;
}

/*****************************************************************************/
//  Description : Establishes connection with specified IP address
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL slim_int SocketConnectPeer(slim_int in_desc, slim_ip_addr *in_addr, slim_int in_port)
{
    int err = -1;
    slim_int ret = SLIM_E_GENERIC;   
    SockAddr addr = {0};
    MMIWWW_INSTANCE_T *self = MMIWWW_INSTANCE();

    if(!(ipaddr_to_sockaddr(&addr,in_addr,in_port)))
    {
        return SLIM_E_GENERIC;
    }

    //modify Netid
    if (PNULL == self)
    {
        return SLIM_E_GENERIC;
    }

    if (0 != self->pdp_info.net_id)
    {
        //SCI_TRACE_LOW:"SocketConnectPeer():modify net_id=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_NET_454_112_2_18_3_11_54_138,(uint8*)"d", self->pdp_info.net_id);

        sci_sock_modifynetid((long)in_desc, self->pdp_info.net_id);
    }

    err = sci_sock_connect((long)in_desc, &addr, sizeof(SockAddr));
    if (err >= 0)
    {
        ret = SLIM_E_OK;
        //SCI_TRACE_LOW:"SocketConnectPeer(): connection is established, in_desc=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_NET_463_112_2_18_3_11_54_139,(uint8*)"d", in_desc);
    }
    else
    {
        err = sci_sock_errno((long)in_desc);
        ret = sci_err_to_netfront_err(err);
    }

    //SCI_TRACE_LOW:"SocketConnectPeer, s=%d, return %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_NET_471_112_2_18_3_11_54_140,(uint8*)"dd", in_desc, ret);

    return ret;
}

/*****************************************************************************/
//  Description : sci error to netfront error
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL slim_int sci_err_to_netfront_err(int in_err)
{
    slim_int ret = SLIM_E_GENERIC;

    //SCI_TRACE_LOW:"sci_err_to_netfront_err=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_NET_484_112_2_18_3_11_54_141,(uint8*)"d",in_err);

    if(EWOULDBLOCK == in_err)
    {
        ret = SLIM_E_WOULDBLOCK;
    }
    else if (EINPROGRESS == in_err)
    {
        ret = SLIM_E_WOULDBLOCK;
    }
    else
    {
        ret = SLIM_E_GENERIC;
    }

    return ret;
}

/*****************************************************************************/
//  Description : ip address convert to sock address
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ipaddr_to_sockaddr(SockAddr *inout_addr,slim_ip_addr *in_addr, slim_int in_port)
{
    slim_int i = 0;
    uint8 uint8_buf[SLIM_IP_ADDR_LEN_V6] = {0};

    if (PNULL == in_addr)
    {
        return FALSE;
    }

    //SCI_TRACE_LOW:"ipaddr_to_sockaddr,in_port=%d, fType=%d,fAddr=%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %dn"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_NET_519_112_2_18_3_11_54_142,(uint8*)"dddddddddddddddddd",in_port,in_addr->fType,in_addr->fAddr[0],in_addr->fAddr[1],in_addr->fAddr[2],in_addr->fAddr[3],in_addr->fAddr[4],in_addr->fAddr[5],in_addr->fAddr[6],in_addr->fAddr[7],in_addr->fAddr[8],in_addr->fAddr[9],in_addr->fAddr[10],in_addr->fAddr[11],in_addr->fAddr[12],in_addr->fAddr[13],in_addr->fAddr[14],in_addr->fAddr[15]);

    if (SLIM_IP_ADDR_TYPE_V4 != in_addr->fType)
    {
        return FALSE;
    }

    for (i = 0; i< SLIM_IP_ADDR_LEN_V4 ; i++)
    {
        uint8_buf[i]=in_addr->fAddr[i];
    }

    inout_addr->family = AF_INET;
    inout_addr->port = (uint16)htons(in_port);
    SCI_MEMSET((void*)inout_addr->sa_data, 0, sizeof(inout_addr->sa_data));
    SCI_MEMCPY(&(inout_addr->ip_addr), uint8_buf, SLIM_IP_ADDR_LEN_V4);

    return TRUE;
}

/*****************************************************************************/
//  Description : socket receive
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL slim_int SocketReceivePeer(slim_int in_desc, slim_byte *out_buf, slim_int in_len)
{
    int res = SLIM_E_GENERIC;
    int err = 0;

    res = sci_sock_recv((long)in_desc, (char *)out_buf, (int)in_len, 0);

   //SCI_TRACE_LOW:"SocketReceivePeer, res = %d"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_NET_550_112_2_18_3_11_54_143,(uint8*)"d", res);
    if (res < 0)
    {
        err = sci_sock_errno((long)in_desc);

        if (EWOULDBLOCK == err)
        {
            //SCI_TRACE_LOW:"SocketReceivePeer, error EWOULDBLOCK"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_NET_557_112_2_18_3_11_54_144,(uint8*)"");

            return SLIM_E_WOULDBLOCK;
        }
        else if (EPIPE == err)
        {
            //SCI_TRACE_LOW:"SocketReceivePeer, EPIPE"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_NET_563_112_2_18_3_11_54_145,(uint8*)"");

            return SLIM_E_OK;
        }
        else
        {
            //SCI_TRACE_LOW:"SocketReceivePeer, error SLIM_E_GENERIC code = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_NET_569_112_2_18_3_11_54_146,(uint8*)"d", err);

            return SLIM_E_GENERIC;
        }
    }

    //SCI_TRACE_LOW:"SocketReceivePeer, s=%d, return %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_NET_575_112_2_18_3_11_54_147,(uint8*)"dd", in_desc, res);

    return res;
}

/*****************************************************************************/
//  Description : socket send
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL slim_int SocketSendPeer(slim_int in_desc, slim_byte *in_buf, slim_int in_len)
{
    int nwritten = -1;
    int err;

    nwritten = sci_sock_send((long)in_desc, (char *)in_buf, in_len, 0);

    if (nwritten < 0)
    {
        err = sci_sock_errno((long)in_desc);
        if (EWOULDBLOCK == err)
        {
            //SCI_TRACE_LOW:"SocketSendPeer, error SLIM_E_WOULDBLOCK\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_NET_596_112_2_18_3_11_54_148,(uint8*)"");
            return SLIM_E_WOULDBLOCK;
        }
        else if (EPIPE == err)
        {
            //SCI_TRACE_LOW:"SocketSendPeer, error SLIM_E_STRPIPE\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_NET_601_112_2_18_3_11_54_149,(uint8*)"");
            return SLIM_E_STRPIPE;
        }
        else
        {
            //SCI_TRACE_LOW:"SocketSendPeer, error SLIM_E_GENERIC code = %d\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_NET_606_112_2_18_3_11_54_150,(uint8*)"d", err);
            return SLIM_E_GENERIC;
        }
    }

    //SCI_TRACE_LOW:"SocketSendPeer, s=%d, return %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_NET_611_112_2_18_3_11_54_151,(uint8*)"dd", in_desc, nwritten);

    return nwritten;
}

/*****************************************************************************/
//  Description : socket bind
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL slim_int SocketBindPeer(slim_int in_desc,slim_ip_addr *in_addr,slim_int in_port)
{
    SockAddr addr = {0};

    if(!(ipaddr_to_sockaddr(&addr,in_addr,in_port)))
    {
        return SLIM_E_GENERIC;
    }

    if(ESUCCESS == sci_sock_bind((long)in_desc,&addr,sizeof(SockAddr)))
    {
        return SLIM_E_OK;
    }

    return SLIM_E_GENERIC;
}

/*****************************************************************************/
//  Description : UDP Send to Addr
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL slim_int UDPSendToPeer(slim_int in_desc, void *in_buf, slim_int in_len, slim_ip_addr *in_addr, slim_int in_port)
{
    SockAddr addr = {0};
    slim_int ret = 0;

    if(!(ipaddr_to_sockaddr(&addr,in_addr,in_port)))
    {
        return SLIM_E_GENERIC;
    }

    ret = sci_sock_sendto((long)in_desc, in_buf, in_len, 0, &addr);
    if(ret < 0)
    {
        int err = 0;

        err = sci_sock_errno((long)in_desc);

        ret = sci_err_to_netfront_err(err);
    }

    return ret;
}

/*****************************************************************************/
//  Description : UDP Receive From Addr
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL slim_int UDPRecvFromPeer(slim_int in_desc, void *out_buf, slim_int in_len, slim_ip_addr *out_addr, slim_int *out_port)
{
    SockAddr addr = {0};
    slim_int ret = 0;

    ret = sci_sock_recvfrom((long)in_desc,out_buf,in_len,0,&addr);
    if(ret < 0)
    {
        int err = 0;

        err = sci_sock_errno((long)in_desc);

        ret = sci_err_to_netfront_err(err);
    }
    else
    {
        out_addr->fType = SLIM_IP_ADDR_TYPE_V4;
        SCI_MEMCPY(out_addr->fAddr, addr.sa_data, SLIM_IP_ADDR_LEN_V4);

        *out_port = ntohs(addr.port);
    }

    return ret;
}


/*---------------------------------------------------------------------------*/
/*                          Interface Implement For Netfont Slim Net Peer                                      */
/*---------------------------------------------------------------------------*/

//Net Refer
/*****************************************************************************/
//  Description : slim net peer initlialize
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_int slimNetInitializePeer(void)
{
    slim_int i = 0;
    for (i = 0; i <= NETFRONT3_DEVICE_MAX_STREAM; i++)
    {
        _net_release_sockobj(i);
    }

    return SLIM_E_OK;
}

/*****************************************************************************/
//  Description : slim net peer finalize
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_int slimNetFinalizePeer(void)
{
    return SLIM_E_OK;
}

//TCP Refer
/*****************************************************************************/
//  Description : Generates TCP protocol endpoint
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_int slimTCPOpenPeer(slim_int in_domain)
{
    slim_int ret = 0;

    //SCI_TRACE_LOW:"slimTCPOpenPeer() start"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_NET_730_112_2_18_3_11_55_152,(uint8*)"");

    MMIWWW_NETCONNECT();
    {
        slim_int desc = _net_get_unused_sockobj();

        //SCI_TRACE_LOW:"slimTCPOpenPeer:desc=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_NET_736_112_2_18_3_11_55_153,(uint8*)"d",desc);
        if (-1 == desc)
        {
            return SLIM_E_GENERIC;
        }
        else
        {
            SocketObj *obj = _net_sockobj(desc);

            if (slim_nil == obj)
            {
                return SLIM_E_GENERIC;
            }

            obj->fState = SOCKET_STATE_OPEN;
            //if (MMIWWW_NetworkIsReady(MMIWWW_INSTANCE()))
            {
                obj->fSocket = SocketOpenPeer(in_domain, SOCK_STREAM);

                if(obj->fSocket >= 0)
                {
                    obj->fState = SOCKET_STATE_CONNECT;
                }
                else
                {
                    obj->fState = SOCKET_STATE_ERROR;

                    return SLIM_E_GENERIC;
                }
            }
        }
        ret = desc;
    }

    return ret;
}

/*****************************************************************************/
//  Description : Terminates TCP communication channel
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void slimTCPClosePeer(slim_int in_desc)
{
    SocketObj *obj = _net_sockobj(in_desc);

    //SCI_TRACE_LOW:"[wv_peer_net.c] slimTCPClosePeer() start"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_NET_781_112_2_18_3_11_55_154,(uint8*)"");

    if (slim_nil == obj)
    {
        return;
    }

    if (obj->fSocket >= 0)
    {
        SocketClosePeer(obj->fSocket);
    }

    _net_release_sockobj(in_desc);
}

/*****************************************************************************/
//  Description : Checks whether reading/writing data is possible
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_int slimTCPCanReadWritePeer(slim_int in_desc, slim_int in_rw)
{
    slim_int ret = 0;
    SocketObj *obj = _net_sockobj(in_desc);

    //SCI_TRACE_LOW:"slimTCPCanReadWritePeer, s=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_NET_805_112_2_18_3_11_55_155,(uint8*)"d", in_desc);

    if (slim_nil == obj)
    {
        return SLIM_E_GENERIC;
    }

    if (obj->fSocket >= 0)
    {
        ret = SocketCanReadWritePeer(obj->fSocket, in_rw);
    }

    return ret;
}

/*****************************************************************************/
//  Description : Checks whether TCP connection was established
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_int slimTCPIsConnectedPeer(slim_int in_desc)
{

    slim_int ret = 0;
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();
    //SCI_TRACE_LOW:"slimTCPIsConnectedPeer:desc=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_NET_829_112_2_18_3_11_55_156,(uint8*)"d", in_desc);

    if (MMIWWW_NetworkIsReady(www_instance))
    {
        SocketObj *obj = _net_sockobj(in_desc);

        if (slim_nil == obj)
        {
            return SLIM_E_GENERIC;
        }

        switch(obj->fState)
        {
        case SOCKET_STATE_OPEN:
            /* this state from slimTCPOpenPeer */
            if (obj->fSocket < 0 )
            {
                obj->fSocket = SocketOpenPeer(SLIM_IP_ADDR_TYPE_V4, SOCK_STREAM);
                if(obj->fSocket >= 0)
                {
                    //SCI_TRACE_LOW:"slimTCPIsConnectPeer:Open success"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_NET_849_112_2_18_3_11_55_157,(uint8*)"");
                    obj->fState = SOCKET_STATE_CONNECT;

                    return slimTCPIsConnectedPeer(in_desc);
                }
                else
                {
                    //SCI_TRACE_LOW:"slimTCPIsConnectPeer:Open err"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_NET_856_112_2_18_3_11_55_158,(uint8*)"");
                    obj->fState = SOCKET_STATE_ERROR;
                    return SLIM_E_GENERIC;
                }
            }
            else
            {
                //SCI_TRACE_LOW:"slimTCPIsConnectPeer:Open success already"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_NET_863_112_2_18_3_11_55_159,(uint8*)"");
                obj->fState = SOCKET_STATE_CONNECT;
                return slimTCPIsConnectedPeer(in_desc);
            }

        case SOCKET_STATE_CONNECT:
            /* maby this state from slimTCPOpenPeer  and myself */
            ret = SocketConnectPeer(obj->fSocket,&(obj->fIPAddr), obj->fPort);
            if (SLIM_E_OK == ret)
            {
                obj->fState = SOCKET_STATE_CONNECTED;
                return 1;
            }
            else if (SLIM_E_WOULDBLOCK == ret)
            {
                return 0;
            }
            else
            {
                return SLIM_E_GENERIC;
            }
            //break;

        case SOCKET_STATE_CONNECTED:
            return 1;
            //break;

        default:
            return SLIM_E_GENERIC;
            //break;
        }
    }
    else if (MMIWWW_NetworkIsAccessible(www_instance))
    {
        return SLIM_E_IOSLEEP;
    }
    else
    {
        return SLIM_E_GENERIC;
    }
}

/*****************************************************************************/
//  Description : Establishes connection with specified IP address
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_int slimTCPConnectPeer(slim_int in_desc, slim_ip_addr *in_addr, slim_int in_port)
{
    slim_int ret = 0;
    SocketObj *obj = _net_sockobj(in_desc);

    if (slim_nil == obj)
    {
        return SLIM_E_GENERIC;
    }

    //SCI_TRACE_LOW:"slimTCPConnectPeer:decs=%d,sock=%d fType=%d,in_port=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_NET_919_112_2_18_3_11_55_160,(uint8*)"dddd",in_desc,obj->fSocket,in_addr->fType,in_port);

    _net_dup_ip_addr(in_desc,in_addr,in_port);

    ret = slimTCPIsConnectedPeer(in_desc);
    if (ret > 0)
    {
        return SLIM_E_OK;
    }
    else if (ret < 0)
    {
        return ret;
    }

    return SLIM_E_WOULDBLOCK;
}

/*****************************************************************************/
//  Description : Reads using TCP protocol
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_int slimTCPReadPeer(slim_int in_desc, slim_byte *out_buf, slim_int in_len)
{
    slim_int ret = 0;
    SocketObj *obj = _net_sockobj(in_desc);

    if (slim_nil == obj)
    {
        return SLIM_E_GENERIC;
    }

    if (obj->fSocket >= 0)
    {
        ret = SocketReceivePeer(obj->fSocket, out_buf, in_len);
    }

    return ret;
}

/*****************************************************************************/
//  Description : Writes using TCP protocol
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_int slimTCPWritePeer(slim_int in_desc, slim_byte *in_buf, slim_int in_len)
{
    slim_int ret = 0;
    SocketObj *obj = _net_sockobj(in_desc);

    if (slim_nil == obj)
    {
        return SLIM_E_GENERIC;
    }

    if (obj->fSocket >= 0)
    {
        ret = SocketSendPeer(obj->fSocket, in_buf, in_len);
    }

    return ret;
}

/*****************************************************************************/
//  Description : Generates TCP protocol communication endpoint for server side
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_int slimTCPServerOpenPeer(slim_int in_domain)
{
    return 0;
}

/*****************************************************************************/
//  Description : Accepts connection request to TCP protocol communication endpoint
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_int slimTCPAcceptPeer(slim_int in_desc, slim_ip_addr *out_addr, slim_int *out_port)
{
    return 0;
}

/*****************************************************************************/
//  Description : Binds IP address, port number with TCP protocol communication endpoint
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_int slimTCPBindPeer(slim_int in_desc, slim_ip_addr *in_addr, slim_int in_port)
{
    //SCI_TRACE_LOW:"slimTCPBindPeer in_addr=%s,in_port=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_NET_1004_112_2_18_3_11_55_161,(uint8*)"sd", in_addr, in_port);

    return SocketBindPeer(in_desc, in_addr, in_port);
}

/*****************************************************************************/
//  Description : Waits for connection to TCP protocol communication endpoint
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_int slimTCPListenPeer(slim_int in_desc, slim_int in_queuelen)
{
    return SLIM_E_GENERIC;
}

//UDP Refer
/*****************************************************************************/
//  Description : Prepares for UDP protocol communication
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_int slimUDPOpenPeer(slim_int in_domain)
{
    slim_int ret = 0;

    ret = SocketOpenPeer(in_domain, SOCK_DGRAM);

    return ret;
}

/*****************************************************************************/
//  Description : Ends UDP protocol communication
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_int slimUDPClosePeer(slim_int in_desc)
{
    SocketClosePeer(in_desc);

    return SLIM_E_OK;
}

/*****************************************************************************/
//  Description : Determines whether reading/writing data is possible
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_int slimUDPCanReadWritePeer(slim_int in_desc, slim_int in_rw)
{
    return SocketCanReadWritePeer(in_desc, in_rw);
}

/*****************************************************************************/
//  Description : Sends to specified IP address via UDP
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_int slimUDPSendToPeer(slim_int in_desc, void *in_buf, slim_int in_len, slim_ip_addr *in_addr, slim_int in_port)
{
    return UDPSendToPeer(in_desc, in_buf, in_len, in_addr, in_port);
}

/*****************************************************************************/
//  Description : Receives UDP from host
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_int slimUDPRecvFromPeer(slim_int in_desc, void *out_buf, slim_int in_len, slim_ip_addr *out_addr, slim_int *out_port)
{
    return UDPRecvFromPeer(in_desc, out_buf, in_len, out_addr, out_port);
}

/*****************************************************************************/
//  Description : Gets local IP address and port number of UDP protocol communication endpoint
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_int slimUDPGetLocalAddrPortPeer(slim_int in_desc, slim_ip_addr *out_addr, slim_int *out_port)
{
    return SLIM_E_GENERIC;
}

/*****************************************************************************/
//  Description : Gets option set for UDP protocol communication endpoint
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_int slimUDPGetOptionPeer(slim_int in_desc, slim_int in_opt)
{
    return SLIM_E_GENERIC;
}

/*****************************************************************************/
//  Description : Binds IP address, port number with UDP protocol communication endpoint
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_int slimUDPBindPeer(slim_int in_desc, slim_ip_addr *in_addr, slim_int in_port)
{
    return slimTCPBindPeer(in_desc, in_addr, in_port);
}

/*****************************************************************************/
//  Description : Checks whether binding was completed
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_int slimUDPIsBoundPeer(slim_int in_desc)
{
    return SLIM_E_GENERIC;
}

#ifdef SLIM_CONFIG_USE_DNS
//DNS Refer
/*****************************************************************************/
//  Description : Allocates and initializes DNS Peer
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_int slimDNSInitializePeer(TDNSWakeUpCallbackProc in_callback, slim_opaque in_callback_aux, TDNSPeer *out_obj)
{
    DNSContext *ctx = PNULL;
    int i = 0;
    
    //SCI_TRACE_LOW:"slimDNSInitializePeer"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_NET_1117_112_2_18_3_11_55_162,(uint8*)"");
    
    ctx = SCI_ALLOCA(sizeof(DNSContext));

    if(PNULL == ctx)
    {
        return SLIM_E_GENERIC;
    }

    SCI_MEMSET(ctx->fTicket, 0, sizeof(ctx->fTicket));
    for (i = 0; i < MMIWWW_DNS_TICKET_MAX; i++)
    {
        ctx->fTicket[i].fDnsWakeUpProc = in_callback;
        ctx->fTicket[i].fcallback_aux = in_callback_aux;
        ctx->fTicket[i].fTicket_id = -1;
    }
    
    *out_obj=(TDNSPeer)ctx;

    return SLIM_E_OK;
}

/*****************************************************************************/
//  Description : Releases DNS Peer
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void slimDNSFinalizePeer(TDNSPeer self)
{
    //SCI_TRACE_LOW:"slimDNSFinalizePeer"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_NET_1145_112_2_18_3_11_55_163,(uint8*)"");

    SCI_FREE(self);

    self = PNULL;
}

/*****************************************************************************/
//  Description : Sets DNS server name
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void slimDNSSetServerPeer(TDNSPeer self, slim_ip_addr* a, slim_ip_addr* b)
{
    //SCI_TRACE_LOW:"slimDNSSetServerPeer not supported"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_NET_1158_112_2_18_3_11_55_164,(uint8*)"");
}

/*****************************************************************************/
//  Description : Get Room Ticket ID
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL slim_int GetRoomTicketID(TDNSPeer self)
{
    slim_int i = 0;
    DNSTicket *ticket_array = PNULL;

    if (PNULL == self)
    {
        return -1;
    }

    ticket_array = (DNSTicket *)self;

    for (i = 0; i < MMIWWW_DNS_TICKET_MAX; i++)
    {
        if (!ticket_array[i].fUse)
        {
            return i;
        }
    }

    return -1;
}

/*****************************************************************************/
//  Description : Requests name resolution
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_int slimDNSRequestResolvePeer(TDNSPeer self, slim_char* in_domain, slim_int in_len)
{
    DNSContext *ctx = PNULL;
    DNSTicket *ticket = PNULL;
    slim_int id = -1;
    MMIWWW_PDP_INFO_T pdp_info = {0};
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();
    
    //SCI_TRACE_LOW:"slimDNSRequestResolvePeer in_domain=%s, in_len=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_NET_1200_112_2_18_3_11_56_165,(uint8*)"sd", in_domain, in_len);
    
    MMIWWW_NETCONNECT();
    
    if (in_len > DNS_MAX_DOMAINNAME || in_len <= 0 || PNULL == www_instance)
    {
        return SLIM_E_GENERIC;
    }

    ctx = (DNSContext*)self;
    id = GetRoomTicketID((TDNSPeer) ctx);

    if (-1 == id)
    {
        return SLIM_E_IOSLEEP;
    }

    MMIWWW_NetfrontGetPDPInfo(&pdp_info);

    ticket = GetTicket(ctx,id);
    SCI_MEMSET(ticket->fName, 0, DNS_MAX_DOMAINNAME);

    //SCI_TRACE_LOW:"slimDNSRequestResolvePeer gateway=%s, gateway_len=%d, bearer_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_NET_1223_112_2_18_3_11_56_166,(uint8*)"sdd",pdp_info.connect_setting.gateway, pdp_info.connect_setting.gateway_len, www_instance->customized_connection.bearer_type);

#ifndef WIN32
    if (MMIPDP_INTERFACE_GPRS == www_instance->customized_connection.bearer_type
        && 0 != pdp_info.connect_setting.gateway_len
        && 0 != strcmp((char *)pdp_info.connect_setting.gateway, (char *)MMIWWW_INVALID_GATEWAY))
    {
        SCI_MEMCPY(ticket->fName, pdp_info.connect_setting.gateway, pdp_info.connect_setting.gateway_len);
    }
    else
#endif
    {
        SCI_MEMCPY(ticket->fName, in_domain, in_len);
    }

    ticket->fName[in_len] = '\0';
    ticket->fUse = TRUE;
    ticket->fState = DNS_REALIZING;
    ticket->fTicket_id = id;

    return id;
}

/*****************************************************************************/
//  Description : Cancels name resolution request
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void slimDNSRequestCancelPeer(TDNSPeer self, slim_int in_ticket_id)
{
    DNSContext *ctx = PNULL;
    DNSTicket  *ticket = PNULL;
    
    //SCI_TRACE_LOW:"slimDNSRequestCancelPeer"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_NET_1255_112_2_18_3_11_56_167,(uint8*)"");

    if (in_ticket_id < 0)
    {
        return;
    }

    ctx = (DNSContext*)self;
    ticket = GetTicket(ctx, in_ticket_id);

    if (!ticket->fUse)
    {
        return;
    }
    
    ticket->fUse = FALSE;
    ticket->fState = DNS_DORMANT;
    ticket->fTicket_id = -1;
    SCI_MEMSET(ticket->fName, 0, DNS_MAX_DOMAINNAME);
}

/*****************************************************************************/
//  Description : Checks whether name resolution was completed
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_int slimDNSIsResolvedPeer(TDNSPeer self, slim_int in_ticket_id)
{
    DNSContext *ctx = PNULL;
    DNSTicket *ticket = PNULL;
    uint32 get_dns_result = 0;
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    //SCI_TRACE_LOW:"slimDNSIsResolvedPeer in_ticket_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_NET_1287_112_2_18_3_11_56_168,(uint8*)"d", in_ticket_id);

    if (in_ticket_id >= MMIWWW_DNS_TICKET_MAX || PNULL == www_instance)
    {
        return SLIM_E_GENERIC;
    }

    if (MMIWWW_NetworkIsReady(www_instance))
    {
        /* go */
    }
    else if (!MMIWWW_NetworkIsAccessible(www_instance))
    {
        return SLIM_E_GENERIC;
    }
    else
    {
        return SLIM_E_WOULDBLOCK;
    }

    ctx = (DNSContext*)self;
    ticket = GetTicket(ctx, in_ticket_id);
    switch(ticket->fState)
    {
    case DNS_REALIZING:
        {
#if 0
            struct sci_hostent *hostent = PNULL;
            hostent = sci_gethostbyname((char *)ticket->fName);

            if (PNULL != hostent)
            {
                ticket->fState = DNS_RESOLVED;
                SCI_MEMCPY(&(ticket->fResult), hostent->h_addr_list[0], SLIM_IP_ADDR_LEN_V4);
            }
            else
            {
                ticket->fState = DNS_REALIZING;
                return SLIM_E_WOULDBLOCK;
            }
#endif
            get_dns_result = sci_async_gethostbyname((char *)ticket->fName, P_APP, MMIWWW_DNS_TIMEOUT, (uint32)www_instance->pdp_info.net_id);

            //SCI_TRACE_LOW:"slimDNSIsResolvedPeer get_dns_result=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_NET_1330_112_2_18_3_11_56_169,(uint8*)"d", get_dns_result);

            if (0 == get_dns_result)
            {
                ticket->fState = DNS_ERROR;
                return SLIM_E_GENERIC;
            }
            else
            {
                www_instance->dns_info.dns_request_id[in_ticket_id] = get_dns_result;
                www_instance->dns_info.ticket[in_ticket_id] = (void *)ticket;
                ticket->fState = DNS_REQUESTING;
                return SLIM_E_WOULDBLOCK;
            }
        }

    case DNS_REQUESTING:
        return SLIM_E_WOULDBLOCK;

    case DNS_RESOLVED:
        return SLIM_E_OK;

    case DNS_ERROR:
        return SLIM_E_GENERIC;

    case DNS_TIMEOUT:
        return SLIM_E_TIMEDOUT;

    default:
        break;
    }

    return SLIM_E_WOULDBLOCK;
}

/*****************************************************************************/
//  Description : Gets resolved IP address
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_int slimDNSGetAddressPeer(TDNSPeer self, slim_int in_ticket_id, slim_ip_addr* out_addr, slim_int in_n,slim_word *out_ttl)
{
    DNSContext *ctx = PNULL;
    DNSTicket *ticket = PNULL;
    slim_int i, n = 0;
    
    //SCI_TRACE_LOW:"slimDNSGetAddressPeer"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_NET_1375_112_2_18_3_11_56_170,(uint8*)"");

    ctx = (DNSContext*)self;
    ticket = GetTicket(ctx, in_ticket_id);
    if (DNS_ERROR == ticket->fState)
    {
        return SLIM_E_GENERIC;
    }

    n = SLIM_MIN(1, in_n);

    for (i = 0; i < n; i++)
    {
        out_addr[i].fType = SLIM_IP_ADDR_TYPE_V4;

        SCI_MEMCPY(out_addr[i].fAddr, &ticket->fResult, SLIM_IP_ADDR_LEN_V4);
    }
    ticket->fUse = FALSE;
    ticket->fState = DNS_DORMANT;
    ticket->fTicket_id = -1;
    SCI_MEMSET(ticket->fName, 0, DNS_MAX_DOMAINNAME);
    
    return SLIM_E_OK;
}

/*****************************************************************************/
//  Description : Sets DNS mode
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_int slimDNSSetModePeer(TDNSPeer self, slim_int in_mode)
{
    //SCI_TRACE_LOW:"slimDNSSetModePeer"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_NET_1406_112_2_18_3_11_56_171,(uint8*)"");

    return SLIM_E_OK;
}

/*****************************************************************************/
//  Description : Sets timeout value
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void slimDNSSetTimeout(TDNSPeer self, slim_int in_timeout)
{
    //SCI_TRACE_LOW:"slimDNSSetTimeout"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_NET_1417_112_2_18_3_11_56_172,(uint8*)"");
}

/*****************************************************************************/
//  Description : Sets DNS port number
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void slimDNSSetDNSPortPeer(TDNSPeer self, slim_int in_port)
{
    //SCI_TRACE_LOW:"slimDNSSetDNSPortPeer"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_NET_1426_112_2_18_3_11_56_173,(uint8*)"");
}

/*****************************************************************************/
//  Description : Sets maximum number of DNS searches
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void slimDNSSetMaxRetryPeer(TDNSPeer self, slim_int in_retry)
{
    //SCI_TRACE_LOW:"slimDNSSetMaxRetryPeer"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_NET_1435_112_2_18_3_11_56_174,(uint8*)"");
}

/*****************************************************************************/
//  Description : Sets DNS search retry interval
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void slimDNSSetRetryIntervalPeer(TDNSPeer self, slim_int in_interval)
{
    //SCI_TRACE_LOW:"slimDNSSetRetryIntervalPeer"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_NET_1444_112_2_18_3_11_56_175,(uint8*)"");
}
#endif

/*****************************************************************************/
//  Description : Get Signal Struct
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL void GetSignalStruct(DPARAM param, void *signal_struct_ptr, uint16 struct_size)
{
    if (PNULL == param || PNULL == signal_struct_ptr || struct_size < sizeof(xSignalHeaderRec))
    {
        return;
    }
    
    SCI_MEMCPY((uint8 *)((uint32)signal_struct_ptr), param, struct_size);
}

/*****************************************************************************/
//  Description : Handle DNS Cnf
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIWWW_HandleDNSCnf(MMIWWW_INSTANCE_T *www_instance, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    ASYNC_GETHOSTBYNAME_CNF_SIG_T signal = {0};
    uint32 request_id = 0;
    slim_int error_code = 0;
    uint32 netid = 0;
    struct sci_hostent *hostent = PNULL;
    uint32 i =0;
    DNSTicket *ticket = PNULL;

    if (PNULL == www_instance)
    {
        return MMI_RESULT_FALSE;
    }

    GetSignalStruct(param, &signal, sizeof(ASYNC_GETHOSTBYNAME_CNF_SIG_T));

    request_id = signal.request_id;
    error_code = signal.error_code;
    hostent = &signal.hostent;
    netid = signal.netid;

    //SCI_TRACE_LOW:"MMIWWW_HandleDNSCnf request_id=%d, error_code=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_NET_1489_112_2_18_3_11_56_176,(uint8*)"dd", request_id, error_code);

    for (i = 0; i < MMIWWW_DNS_TICKET_MAX; i++)
    {
        if (request_id == www_instance->dns_info.dns_request_id[i])
        {
            break;
        }
    }

    if (MMIWWW_DNS_TICKET_MAX == i)
    {
        return FALSE;
    }

    if (0 != www_instance->browser_instance)
    {
        MMIWWW_Netfront_Wakeup(www_instance->browser_instance);
    }

    ticket = (DNSTicket *)www_instance->dns_info.ticket[i];

    if (0 == error_code)
    {
        ticket->fState = DNS_RESOLVED;
        SCI_MEMCPY(&(ticket->fResult), hostent->h_addr_list[0], SLIM_IP_ADDR_LEN_V4);
    }
    else if (-24 == error_code)//time out ENP_TIMEOUT
    {
        ticket->fState = DNS_TIMEOUT;
    }
    else
    {
        ticket->fState = DNS_ERROR;
    }

    if (PNULL != ticket->fDnsWakeUpProc)
    {
        ticket->fDnsWakeUpProc(ticket->fcallback_aux, ticket->fTicket_id);
    }
    else
    {
        if (0 != www_instance->browser_instance)
        {
            MMIWWW_Netfront_Wakeup(www_instance->browser_instance);
        }
    }

    return result;
}
#endif //#ifdef BROWSER_SUPPORT_NETFRONT
