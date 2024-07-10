/*****************************************************************************
** File Name:       tcpip_types.h
** Author:          Yifeng.Wang
** Date:            2009/07/22
** Copyright:       2009 Spreadtrum, Incorporated. All Rights Reserved.
** Description:     This file defines tcpip types and macro definitions
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE             NAME                DESCRIPTION                          
** 2009/07/22       Yifeng.Wang         Create
******************************************************************************/

#ifndef  _TCPIP_TYPES_H_
#define  _TCPIP_TYPES_H_

/*---------------------------------------------------------------------------*
 *                          Include Files                                    *
 *---------------------------------------------------------------------------*/
#include "sci_types.h"

/*---------------------------------------------------------------------------*
 *                          Compiler Flag                                    *
 *---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
/*---------------------------------------------------------------------------*
 *                          MACRO DEFINITION                                 *
 *---------------------------------------------------------------------------*/
/*
 * convert little/big endian - these should be efficient,
 * inline code or MACROs
 *
 * 1.__BIG_ENDIAN and/or __BigEndian macros appear to be
 *   auto-defined by ARM C in big-endian mode.
 * 2.network endian is big endian.
 */
#if defined(__BIG_ENDIAN) || defined(__BigEndian)
#define htonl(l) (l)
#define ntohl(l) (l)
#define htons(s) (s)
#define ntohs(s) (s)
#else
#define lswap(x) ((((x) & 0xff000000) >> 24) | \
                  (((x) & 0x00ff0000) >>  8) | \
                  (((x) & 0x0000ff00) <<  8) | \
                  (((x) & 0x000000ff) << 24))
#define htonl(l) (lswap(l))
#define ntohl(l) (lswap(l))
#define htons(s) ((((s) & 0xff00) >> 8) | \
                  (((s) & 0x00ff) << 8))
#define ntohs(s) htons(s)
#endif


/* IP address definition in BIG-ENDING */
#define TCPIP_IP_A_SUBMASK      htonl(0xff000000L)  /* 255.0.0.0 */
#define TCPIP_IP_B_SUBMASK      htonl(0xffff0000L)  /* 255.255.0.0 */
#define TCPIP_IP_C_SUBMASK      htonl(0xffffff00L)  /* 255.255.255.0 */
#define TCPIP_IP_NULL_ADDR      htonl(0x00000000L)  /* 0.0.0.0 */
#define TCPIP_IP_LOOPBACK       htonl(0x7f000000L)  /* 127.0.0.0 */

#define TCPIP_NETID_DEFAULT     0       /* default net interface ID */
#define TCPIP_NETID_NULL        0       /* NULL net interface ID */
#define TCPIP_MTU_DEFAULT       0       /* default MTU */
#define TCPIP_TXRATE_DEFAULT    0       /* default tx rate */
#define TCPIP_HOST_HANDLE_NULL  0       /* NULL async host handle */
#define TCPIP_IPCONFLICT_HANDLE_NULL    0   /* NULL IP conflict handle */
#define TCPIP_MODEMID_NULL      (-1)    /* invalid modem ID */
#define TCPIP_HADDR_MAX_LEN     6       /* MAX hard address length */


/* NetPort stack generic error codes: generally full success is 0,
 * definite errors are negative numbers, and indeterminate conditions
 * are positive numbers. These may be changed if they conflict with
 * defines in the target system. They apply to the NetPort IP stack,
 * the IPX logic, and the portable SNMP Agent. If you have to change
 * these values, be sure to recompile ALL NetPort sources. 
 */
#define SUCCESS         0       /* whatever the call was, it worked */

/* programming errors */
#define ENP_PARAM       (-10)   /* bad parameter */
#define ENP_LOGIC       (-11)   /* sequence of events that shouldn't happen */

/* system errors */
#define ENP_NOMEM       (-20)   /* malloc or calloc failed */
#define ENP_NOBUFFER    (-21)   /* ran out of free packets */
#define ENP_RESOURCE    (-22)   /* ran out of other queue-able resource */
#define ENP_BAD_STATE   (-23)   /* TCP layer error */
#define ENP_TIMEOUT     (-24)   /* TCP layer error */

#define ENP_NOFILE      (-25)   /* expected file was missing */
#define ENP_FILEIO      (-26)   /* file IO error */

/* net errors */
#define ENP_SENDERR     (-30)   /* send to net failed at low layer */
#define ENP_NOARPREP    (-31)   /* no ARP for a given host */
#define ENP_BAD_HEADER  (-32)   /* bad header at upper layer (for upcalls) */
#define ENP_NO_ROUTE    (-33)   /* can't find a reasonable next IP hop */
#define ENP_NO_IFACE    (-34)   /* can't find a reasonable interface */
#define ENP_HARDWARE    (-35)   /* detected hardware failure */

/* conditions that are not really fatal OR success: */
#define ENP_SEND_PENDING        1  /* packet queued pending an ARP reply */
#define ENP_NOT_MINE            2  /* packet was not of interest (upcall reply) */
#define ENP_NAME_OR_FORMAT_ERR  3  /* response rcode from dns server is format error or Name error */


/*---------------------------------------------------------------------------*
 *                          DECLARATION                                      *
 *---------------------------------------------------------------------------*/
// data types
typedef  uint32     TCPIP_NETID_T;              /* TCPIP net interface ID */
typedef  uint32     TCPIP_MODEM_ID_T;           /* modem ID */
typedef  uint32     TCPIP_IPADDR_T;             /* integar IPv4 address */
typedef  uint16     TCPIP_PING_HANDLE;          /* PING request handle */
typedef  uint32     TCPIP_HOST_HANDLE;          /* async gethostbyname request handle */
typedef  uint32     TCPIP_IPCONFLICT_HANDLE;    /* IP conflict request handle */


// TCPIP interface error enum
typedef enum
{
    TCPIP_ERROR_OK = 0,     /* no error */
    TCPIP_ERROR_INVALPARAM, /* invalid parameter */
    TCPIP_ERROR_INVALNETID, /* invalid net id */
    TCPIP_ERROR_MEMALLOC,   /* memory alloc fail */
    TCPIP_ERROR_LOGICAL,    /* calling or running logical error */
    TCPIP_ERROR_TIMEOUT,    /* time out */
    TCPIP_ERROR_MAX
} TCPIP_ERROR_E;


// TCPIP net interface packet type
typedef enum
{
    TCPIP_PKTTYPE_NULL = 0,
    TCPIP_PKTTYPE_IP,       /* packet is encapsulated as IP */
    TCPIP_PKTTYPE_ETHER,    /* packet is encapsulated as Ethernet */
    TCPIP_PKTTYPE_PPP,      /* packet is encapsulated as PPP */
    TCPIP_PKTTYPE_MAX
} TCPIP_PACKET_TYPE_E;

// TCPIP net interface address
// NOTES : IP addresses should be in Big-Ending
typedef struct netif_addrs_tag
{
    TCPIP_IPADDR_T  ipaddr;     /* host IP */
    TCPIP_IPADDR_T  snmask;     /* subnet mask */
    TCPIP_IPADDR_T  gateway;    /* gateway */
    TCPIP_IPADDR_T  dns1;       /* primary DNS */
    TCPIP_IPADDR_T  dns2;       /* secondary DNS */
    uint8           haddr[TCPIP_HADDR_MAX_LEN];
                                /* hard address */
    uint8           haddr_len;  /* hard address length */
} TCPIP_NETIF_ADDRS_T;

// TCPIP net interface IP addresses
// NOTES : IP addresses should be in Big-Ending
typedef struct netif_ipaddr_tag
{
    TCPIP_IPADDR_T  ipaddr;     /* host IP */
    TCPIP_IPADDR_T  snmask;     /* subnet mask */
    TCPIP_IPADDR_T  gateway;    /* gateway */
    TCPIP_IPADDR_T  dns1;       /* primary DNS */
    TCPIP_IPADDR_T  dns2;       /* secondary DNS */
} TCPIP_NETIF_IPADDR_T;


// TCPIP net interface hardware address
typedef struct netif_haddr_tag
{
    uint8*  addr_ptr;           /* hard address pointer */
    uint32  addr_len;           /* hard address length, unit: byte */
} TCPIP_NETIF_HADDR_T;


// TCPIP packet info struct
typedef struct packet_info_tag
{
    uint8*          data_ptr;       /* data pointer */
    uint32          data_len;       /* data length - full packet encapsulation length */
    TCPIP_NETID_T   net_id;         /* net interface ID */
    uint32          link_handle;    /* link layer handle */
} TCPIP_PACKET_INFO_T;


// TCPIP data send function
// PARAM1: pkt_info_ptr (IN) -- packet data info pointer
// RETURN: 0 - send OK; else - send fail
// NOTES : if async send, send OK just means succeed in sending data to low bearer,
//         when low bearer send packet over, it should called TCPIP_TxCnf to notify
//         TCPIP release the packet buffer.
typedef int32 (*TCPIP_TX_REQ_FPTR)( 
    const TCPIP_PACKET_INFO_T*  pkt_info_ptr );


// TCPIP receive flow control setting function 
typedef void (*TCPIP_RX_FLCTRL_FPTR)(
    BOOLEAN         is_set,     /* flow control flag: TRUE - set; FALSE - not set */
    TCPIP_NETID_T   net_id );   /* net interface ID */


// EAPOL data receive upcall function
typedef void (*TCPIP_EAPOL_RX_CALLBACK_FPTR)( 
    const uint8*    data_ptr,       /* EAPOL data pointer */
    uint32          data_len,       /* EAPOL data length */
    const uint8*    src_mac_ptr,    /* source mac address pointer */
    uint32          src_mac_len,    /* source mac length, unit: byte 
                                     * for ethernet, it should be 6 */
    TCPIP_NETID_T   net_id );       /* net interface ID */


// TCPIP net interface configuration struct
typedef struct netif_cfg_tag
{
    TCPIP_PACKET_TYPE_E     pkt_type;       /* packet type between tcpip and low layer */
    BOOLEAN                 is_async_tx;    /* tx mode: TRUE - async send; FALSE - sync send */
    TCPIP_TX_REQ_FPTR       tx_req_fptr;    /* tx function pointer for data sending from tcpip to low layer */ 
    TCPIP_NETIF_IPADDR_T    saddr;          /* soft (IP) address */
    TCPIP_NETIF_HADDR_T     haddr;          /* hard (MAC) address */
    uint32                  mtu;            /* MTU of the low layer */
    uint32                  tx_rate;        /* max tx send rate - it's useful in tx flow control */
    TCPIP_RX_FLCTRL_FPTR    rx_flctrl_fptr; /* rx flow control function pointer, 
                                             * this is optional, can be NULL */
    char*                   netif_name_ptr; /* net interface name, given by low layer,
                                             * it can be NULL, but we recommend caller set one for better debug */
} TCPIP_NETIF_CFG_T;


/*---------------------------------------------------------------------------*
 *                          GLOBAL DEFINITION                                *
 *---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*
 *                          Compiler Flag                                    *
 *---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif  /* _TCPIP_TYPES_H_ */
