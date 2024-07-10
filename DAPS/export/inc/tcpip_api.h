/*****************************************************************************
** File Name:       tcpip_api.h
** Author:          Yifeng.Wang
** Date:            2009/06/08
** Copyright:       2009 Spreadtrum, Incorporated. All Rights Reserved.
** Description:     This file defines external TCPIP function
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE             NAME                DESCRIPTION
** 2009/06/08       Yifeng.Wang         Create
** 2010/04/28       Yifeng.Wang         Amend for unified bearer
******************************************************************************/

#ifndef  _TCPIP_API_H_
#define  _TCPIP_API_H_

/*---------------------------------------------------------------------------*
 *                          Include Files                                    *
 *---------------------------------------------------------------------------*/
#include "tcpip_types.h"

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


/*---------------------------------------------------------------------------*
 *                          DECLARATION                                      *
 *---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*
 *                          GLOBAL DEFINITION                                *
 *---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*
 *                          FUNCTIONS                                        *
 *---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : TCPIP stack start entry
// Global : None 
// Author : 
// Note : this function should be called before all other tcpip interface!
/*****************************************************************************/
PUBLIC void tcpip_tasks_entry( void );

/*****************************************************************************/
// Description : TCPIP stack is start or not
// Global : None 
// Author : 
// Note : this function should be called before all other tcpip interface!
/*****************************************************************************/
PUBLIC BOOLEAN tcpip_is_startup( void );

/*****************************************************************************/
// Description: register tcpip net interface
// Global : none
// Author : yifeng.wang
// Note : for unified multi-bearer use
//        PARAM1: cfg_ptr (IN) -- netif config pointer
//        RETURN: net interface ID, return TCPIP_NETID_NULL if register failed
/*****************************************************************************/
PUBLIC TCPIP_NETID_T TCPIP_RegNetInterface( 
    const TCPIP_NETIF_CFG_T* cfg_ptr );

/*****************************************************************************/
// Description: deregister tcpip net interface
// Global : none
// Author : yifeng.wang
// Note : for unified multi-bearer use
//        PARAM1: net_id (IN) -- net interface ID
/*****************************************************************************/
PUBLIC void TCPIP_DeregNetInterface( 
    TCPIP_NETID_T   net_id );

/*****************************************************************************/
// Description: get address info of given net interface
// Global : none
// Author : yifeng.wang
// Note : PARAM1: local_addr_ptr (OUT) -- peer address pointer
//        PARAM2: peer_addr_ptr (OUT) -- local address pointer
//        PARAM3: net_id (IN) -- net interface ID
//        RETURN: TCPIP_ERROR_OK - succeed; else - fail
/*****************************************************************************/
PUBLIC TCPIP_ERROR_E TCPIP_GetModemAddress( 
    TCPIP_NETIF_ADDRS_T* peer_addr_ptr,
    TCPIP_NETIF_ADDRS_T* local_addr_ptr,
    TCPIP_NETID_T        net_id );
#ifdef IP_V6
/*****************************************************************************/
// Description: set  dnsv6 addresses on given net interface ID
// Global resource : none
// Author: shuyun.fu
// Note :  PARAM1: dns (IN) --  dns
//         PARAM2: net_id (IN) -- net interface ID
//         Input IP addresses should be in Big-Endian.
//         RETURN: TCPIP_ERROR_OK - succeed; else - fail
/*****************************************************************************/
PUBLIC TCPIP_ERROR_E TCPIP_SetDnsv6(
    TCPIP_IPADDR6_T*  dns,
    TCPIP_NETID_T   net_id);

/*****************************************************************************/
// Description: get dnsv6 addresses on given net interface ID
// Global : none
// Author : shuyun.fu
//        PARAM1: dns_ptr (OUT) -- dns addresses  pointer
//        PARAM2: net_id (IN) -- net interface ID
//         RETURN: TRUE - success; FALSE - fail
//         Returned IP addresses are in Big-Endian.
/*****************************************************************************/
PUBLIC BOOLEAN TCPIP_GetDnsv6( 
    TCPIP_IPADDR6_T* dns_ptr,
    TCPIP_NETID_T  net_id );
#endif

/*****************************************************************************/
// Description: set IPv4 addresses on given net interface
// Global : none
// Author : yifeng.wang
// Note : for unified multi-bearer use
//        PARAM1: addr_ptr (IN) -- addresses configure pointer
//        PARAM2: net_id (IN) -- net interface ID
//        RETURN: TCPIP_ERROR_OK - succeed; else - fail
/*****************************************************************************/
PUBLIC TCPIP_ERROR_E TCPIP_SetIpAddress( 
    const TCPIP_NETIF_IPADDR_T*	addr_ptr,
    TCPIP_NETID_T               net_id );

/*****************************************************************************/
// Description: TCPIP data tx confirm call-back
// Global : none
// Author : yifeng.wang
// Note : for unified multi-bearer use
//        PARAM1: pkt_data_ptr (IN) -- packet data info pointer been sent over
//        RETURN: TCPIP_ERROR_OK - tx confirm OK; else - fail
//        In asyncronous send, low bearer should call this interface to tell 
//        TCPIP release the packet buffer whether packet sending is OK or failed!
/*****************************************************************************/
PUBLIC TCPIP_ERROR_E TCPIP_TxCnf(
    const TCPIP_PACKET_INFO_T* pkt_data_ptr );

/*****************************************************************************/
// Description: TCPIP data receive upcall
// Global : none
// Author : yifeng.wang
// Note : for unified multi-bearer use
//        PARAM1: pkt_data_ptr (IN) -- packet data info pointer to be received
//        RETURN: TCPIP_ERROR_OK - data receive OK; else - fail
//        if pkt_data_ptr use dynamic memory, who allocate the memory, then who
//        should free them correspondingly!
/*****************************************************************************/
PUBLIC TCPIP_ERROR_E TCPIP_RxInd(
    const TCPIP_PACKET_INFO_T* pkt_data_ptr );

/*****************************************************************************/
// Description: TCPIP modem data receive upcall
// Global : none
// Author : yifeng.wang
// Note : PARAM : pkt_data_ptr (IN) -- packet data to be received
//        RETURN: TCPIP_ERROR_OK - data receive OK; else - fail
/*****************************************************************************/
PUBLIC TCPIP_ERROR_E TCPIP_ModemRxInd(
    const TCPIP_PACKET_INFO_T* pkt_info_ptr );

/*****************************************************************************/
// Description: set data tx flow control flag
// Global : none
// Author : yifeng.wang
// Note : for unified multi-bearer use
//        PARAM1: tx_rate (IN) -- max tx rate, unit( byte/s ), 0 means default
//        PARAM2: net_id (IN) -- net interface ID
/*****************************************************************************/
PUBLIC void TCPIP_SetTxFlowCtrl( BOOLEAN is_set, TCPIP_NETID_T net_id );

/*****************************************************************************/
// Description : set global TCP send window size
// Global resource : 
// Author : yifeng.wang
// Note : PARAM : size (IN) -- send window size, unit: bytes
//                0 means to set with default window size
/*****************************************************************************/
PUBLIC void TCPIP_SetGlobalSendWin( uint32 size );

/*****************************************************************************/
// Description: set pclink net interface
// Global : none
// Author : yifeng.wang
// Note : for unified multi-bearer use
//        RETURN: net interface ID for pclink use, 
//        if fail, return TCPIP_NETID_NULL
/*****************************************************************************/
PUBLIC TCPIP_NETID_T TCPIPPCLINK_RegNetInterface( void );

/*****************************************************************************/
// Description: reset pclink net interface
// Global : none
// Author : yifeng.wang
// Note : for unified multi-bearer use
//        PARAM1: net_id (IN) -- net interface ID
/*****************************************************************************/
PUBLIC void TCPIPPCLINK_DeregNetInterface( void );

/*****************************************************************************/
// Description : return TCPIP version information
// Global resource : s_tcpip_version_info
// Author: fancier.fan, yifeng.wang
// Note : RETURN : char pointer to TCPIP version string
/*****************************************************************************/
PUBLIC const char* TCPIP_GetVersionInfo( void );

/*****************************************************************************/
// Description : set cap output enable flag
// Global resource :
// Author : yifeng.wang
// Notes : 
/*****************************************************************************/
PUBLIC void TCPIPDEBUG_EnableCap( BOOLEAN is_enable );

/*****************************************************************************/
// Description : get cap output enable flag
// Global resource :
// Author : yifeng.wang
// Notes : 
/*****************************************************************************/
PUBLIC BOOLEAN TCPIPDEBUG_IsCapEnable( void );

/*****************************************************************************/
// Description : set modem cap output enable flag
// Global resource :
// Author : yifeng.wang
// Notes : 
/*****************************************************************************/
PUBLIC void TCPIPDEBUG_EnablePktOutput( BOOLEAN is_enable );

/*****************************************************************************/
// Description : get modem cap output enable flag
// Global resource :
// Author : yifeng.wang
// Notes : 
/*****************************************************************************/
PUBLIC BOOLEAN TCPIPDEBUG_IsPktOutputEnable( void );

/*****************************************************************************/
// Description : enable ethernet data send ( only valid in WIN32 )
// Global resource : s_ether_send_pkttype_filter
// Author : yifeng.wang
// Notes : if disable, ethernet will drop all packets from upper layer
//         PARAM1 : is_enable (IN) -- enable flag, TRUE - enable; FALSE - disable
//         PARAM2 : data_type (IN) -- packet types that the operation to influence,
//                  use macros defined with prefix ETHER_PKTTYPE in tcpip_types.h,
//                  it can be combinitions of any type macros.
/*****************************************************************************/
PUBLIC void TCPIPETHER_EnableSend( BOOLEAN is_enable, uint32 data_type );

/*****************************************************************************/
// Description : enable ethernet data receive ( only valid in WIN32 )
// Global resource : s_ether_recv_pkttype_filter
// Author : yifeng.wang
// Notes : if disable, ethernet will drop all packets from ethernet
//         PARAM1 : is_enable (IN) -- enable flag, TRUE - enable; FALSE - disable
//         PARAM2 : data_type (IN) -- packet types that the operation to influence,
//                  use macros defined with prefix ETHER_PKTTYPE in tcpip_types.h,
//                  it can be combinitions of any type macros.
/*****************************************************************************/
PUBLIC void TCPIPETHER_EnableRecv( BOOLEAN is_enable, uint32 data_type );

/*****************************************************************************/
// Description: check whether IPv6 addresses is ok on given net_id
// Global resource : none
// Author: rongjun.zhang 
// Note : PARAM1: net_id (IN) -- net id
//          RETURN:  NULL - fail    else - success
/*****************************************************************************/
PUBLIC void* TCPIP_CheckIpv6Prefix(
    TCPIP_NETID_T net_id );

/*----------------------------------------------------------------------------*
 *                          Compiler Flag                                     *
 *----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

///////////////////////////////////////////////////////////////////////////////
#endif  /* _TCPIP_API_H_ */