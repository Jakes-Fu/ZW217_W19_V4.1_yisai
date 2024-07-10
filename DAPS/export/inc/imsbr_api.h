/*****************************************************************************
** File Name:   imsbr_api.h
** Author:      yalk.wang
** Date:        2016/01/25
** Copyright:   2016 Spreadtrum, Incorporated. All Rights Reserved.
** Description: IMS bridge filter interfaces for VoLTE/VoWifi handover
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE             NAME                 DESCRIPTION
** 2016/01/25       Yalk.Wang            Create
******************************************************************************/

#ifndef _IMSBR_API_H
#define _IMSBR_API_H

/*---------------------------------------------------------------------------*
 *                          Include Files                                    *
 *---------------------------------------------------------------------------*/
#include "socket_types.h"
#include "tcpip_types.h"

/*---------------------------------------------------------------------------*
 *                          MACRO DEFINITION                                 *
 *---------------------------------------------------------------------------*/
#define IMSBR_OK            0

#define IMSBR_IP_4          AF_INET
#define IMSBR_IP_6          AF_INET6

#define IMSBR_PROTO_NONE    0xFF
#define IMSBR_PROTO_IP4     0x00
#define IMSBR_PROTO_ICMP4   0x01
#define IMSBR_PROTO_TCP     0x06
#define IMSBR_PROTO_UDP     0x11
#define IMSBR_PROTO_IP6     0x29
#define IMSBR_PROTO_ESP     0x32
#define IMSBR_PROTO_ICMP6   0x3A

#define IMSBR_CONN_HANDLE_NULL      0
#define IMSBR_FILTER_HANDLE_NULL    0


/*---------------------------------------------------------------------------*
 *                          TYPE DECLARATION                                 *
 *---------------------------------------------------------------------------*/
typedef uint32 IMSBR_CONN_HANDLE;
typedef IMSBR_CONN_HANDLE IMSBR_FILTER_HANDLE;


/**  
 * IMSBR_SIMCARD_1: simcard 1
 * IMSBR_SIMCARD_2: simcard 2
 */
typedef enum imsbr_sim_card
{
    IMSBR_SIM_1 = 0,
    IMSBR_SIM_2,
    IMSBR_SIM_TEST1,  /* virtual sim1 for unit test */
    IMSBR_SIM_TEST2,  /* virtual sim2 for unit test */
    IMSBR_SIM_MAX,
} IMSBR_SIM_E;

#define IMSBR_SIM_DEFAULT   IMSBR_SIM_1


/**  
 * IMSBR_LINK_AP: link data is sent/received at AP
 * IMSBR_LINK_CP: link data is sent/received at CP
 */
typedef enum imsbr_link_type
{
    IMSBR_LINK_NA = 0,
    IMSBR_LINK_AP,
    IMSBR_LINK_CP,
    IMSBR_LINK_MAX
} IMSBR_LINK_E;

/**
 * IMSBR_SOCKET_AP: socket is located at AP
 * IMSBR_SOCKET_CP: socket is located at CP
 */
typedef enum imsbr_socket_type
{
    IMSBR_SOCKET_NA = 0,
    IMSBR_SOCKET_AP,
    IMSBR_SOCKET_CP,
    IMSBR_SOCKET_MAX
} IMSBR_SOCKET_E;

/**
 * IMSBR_IMS_SIP:     Data connection is for SIP
 * IMSBR_IMS_RTP_AD:  Data connection is for audio RTP
 * IMSBR_IMS_RTP_VD:  Data connection is for video RTP
 * IMSBR_IMS_RTCP_AD: Data connection is for audio RTCP
 * IMSBR_IMS_RTCP_VD: Data connection is for video RTCP
 */
typedef enum
{
    IMSBR_IMS_NA = 0,
    IMSBR_IMS_SIP,
    IMSBR_IMS_RTP_AD,
    IMSBR_IMS_RTP_VD,
    IMSBR_IMS_RTCP_AD,
    IMSBR_IMS_RTCP_VD,
    IMSBR_IMS_IKE,
    IMSBR_IMS_MAX
} IMSBR_IMS_TYPE_E;

typedef union imsbr_ipaddr {
    TCPIP_IPADDR_T  addr4;
    TCPIP_IPADDR6_T addr6;
} IMSBR_IPADDR_U;


/**
 * connection info used in IMS session
 */
typedef struct imsbr_conn {
    uint8  sim_card;        /* IMSBR_SIMCARD_E */
    uint8  link_type;       /* IMSBR_LINK_TYPE_E */
    uint8  sock_type;       /* IMSBR_SOCKET_TYPE_E */
    uint8  ims_type;        /* IMSBR_IMS_xxx */
    uint8  ip_type;         /* IMSBR_IP_xxx */
    uint8  proto;           /* IMSBR_PROTO_xxx */
    uint16 local_port;
    uint16 peer_port;
    IMSBR_IPADDR_U local_ip;
    IMSBR_IPADDR_U peer_ip;
} IMSBR_CONN_INFO_T;

typedef IMSBR_CONN_INFO_T   IMSBR_FILTER_T;


///////////////////////////////////////////////////////////////////////////////
/* NOTES: enum values here must be identical with elements of
   MN_PHONE_IPC_IMS_CHANNEL_OPER_E */
typedef enum
{
    OP_CREATE = 1,
    OP_DELETE = 2,
    OP_CHANGE = 3,
    OP_MAX
} IMSBR_MEDIA_OPTYPE_E;

typedef struct imsbr_media {
    IMSBR_MEDIA_OPTYPE_E op_type;   /* OP_CREATE/DELETE/CHANGE */
    uint8  ip_type;         /* IMSBR_IP_4/6 */
    uint8  proto;           /* should be IMSBR_PROTO_UDP */
    uint16 rtp_local_port;
    uint16 rtp_peer_port;
    uint16 rtcp_local_port;
    uint16 rtcp_peer_port;
    uint8* local_ip;
    uint8* peer_ip;
} IMSBR_MEDIA_T;


///////////////////////////////////////////////////////////////////////////////
/* NOTES: for PSIT use */
typedef int32 (*IMSBR_TX2AP_FPTR)(uint8* ptr, uint32 len, TCPIP_NETID_T netid);


/*---------------------------------------------------------------------------*
 *                          FUNCTION DECLARATION                             *
 *---------------------------------------------------------------------------*/
PUBLIC const char* IMSBR_GetVersion(void);

PUBLIC IMSBR_CONN_HANDLE IMSBR_AddConn(IMSBR_CONN_INFO_T* pinfo);
PUBLIC void              IMSBR_DeleteConn(IMSBR_CONN_INFO_T* pinfo);
PUBLIC void              IMSBR_DeleteConnByHandle(IMSBR_CONN_HANDLE handle);
PUBLIC void              IMSBR_ClearConn(IMSBR_SOCKET_E sock, IMSBR_LINK_E link, IMSBR_SIM_E sim);

PUBLIC void              IMSBR_SetVolteNetid(TCPIP_NETID_T netid);
PUBLIC void              IMSBR_ResetVolteNetid(void);
PUBLIC TCPIP_NETID_T     IMSBR_GetVolteNetid(void);

PUBLIC void              IMSBR_SetVolteNetidEx(TCPIP_NETID_T netid, IMSBR_SIM_E sim);
PUBLIC void              IMSBR_ResetVolteNetidEx(IMSBR_SIM_E sim);
PUBLIC TCPIP_NETID_T     IMSBR_GetVolteNetidEx(IMSBR_SIM_E sim);

PUBLIC void              IMSBR_SetVowifiNetid(TCPIP_NETID_T netid);
PUBLIC void              IMSBR_ResetVowifiNetid(void);
PUBLIC TCPIP_NETID_T     IMSBR_GetVowifiNetid(void);


///////////////////////////////////////////////////////////////////////////////
// for PSIT
PUBLIC int32 IMSBR_SendToTcpip(uint8* ptr, uint32 len, TCPIP_NETID_T netid);
PUBLIC int32 IMSBR_SendToTcpipEx(uint8* ptr, uint32 len, TCPIP_NETID_T netid, IMSBR_SIM_E sim);
PUBLIC int32 IMSBR_RegTx2ApFptr(IMSBR_TX2AP_FPTR fptr);
PUBLIC int32 IMSBR_RegTx2ApFptrEx(IMSBR_TX2AP_FPTR fptr, IMSBR_SIM_E sim);

///////////////////////////////////////////////////////////////////////////////
// for PS
PUBLIC TCPIP_NETID_T     IMSBR_SetVowifiAudioInfo(IMSBR_MEDIA_T* info_ptr);

PUBLIC IMSBR_CONN_HANDLE IMSBR_AddVolteConn(IMSBR_CONN_INFO_T* pinfo);
PUBLIC void              IMSBR_DeleteVolteConn(IMSBR_CONN_INFO_T* pinfo);
PUBLIC void              IMSBR_DeleteVolteConnByHandle(IMSBR_CONN_HANDLE handle);
PUBLIC void              IMSBR_ClearVolteConn(IMSBR_SIM_E sim);

// adaptive with the old APIs
#define IMSBR_SetNetif              IMSBR_SetVolteNetid
#define IMSBR_ResetNetif            IMSBR_ResetVolteNetid
#define IMSBR_GetNetif              IMSBR_GetVolteNetid

#define IMSBR_AddFilter             IMSBR_AddVolteConn
#define IMSBR_RemoveFilter          IMSBR_DeleteVolteConn
#define IMSBR_RemoveFilterHandle    IMSBR_DeleteVolteConnByHandle
#define IMSBR_ClearFilter           IMSBR_ClearVolteConn


///////////////////////////////////////////////////////////////////////////////
#endif /* _IMSBR_API_H */

