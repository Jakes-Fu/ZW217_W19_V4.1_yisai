/*****************************************************************************
 ** File Name:     ps_tcp_ip.h                                               *
 ** Author:        William.Qian                                              *
 ** Date:          2006/02/28                                                *
 ** Copyright:     2001 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:   tcpip interfaces for PS use.                              *
 *****************************************************************************
 *****************************************************************************
 **                        Edit History                                      *
 ** -------------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                              *
 ** 2006/02/28     William.Qian     Create                                   *
 ** 2006/03/01     Fancier.Fan      Amend                                    *
 ** 2009/05/31     Yifeng.Wang      Add interface for Multi-PDP              *
 ** 2009/08/26     Yifeng.Wang      Link to new interface file               *
 ** 2009/11/22     Yifeng.Wang      Amend for unified TCPIP                  *
 *****************************************************************************/
#ifndef _PS_TCP_IF_H_
#define _PS_TCP_IF_H_

#include "sci_types.h"
#include "tcpip_types.h"
#include "tcpip_api.h"


/*---------------------------------------------------------------------------*
 *                          FUNCTIONS                                        *
 *---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : set tcpip net parameters
// Global : None 
// Author : yifeng.wang
// Note : called by GPRS
//        PARAM1 : nsapi (IN) - main NSAPI of actived GPRS
//        PARAM2 : nsapi_p (IN) - parent NSAPI of main NSAPI
//        PARAM3 : ipaddr (IN) - IP address
//        PARAM4 : snmask (IN) - subnet mask, default 0
//        PARAM5 : dns1 (IN) - first DNS server address
//        PARAM6 : dns2 (IN) - second DNS server address
//        PARAM7 : gateway (IN) - gateway, default 0
//        RETURN : 0 if OK, else error!      
/*****************************************************************************/
PUBLIC int32 TCPIP_SetNetInterface(  /*lint -esym(765,TCPIP_SetNetInterface) */
    uint8  nsapi,    uint8  nsapi_p,
    uint32 ipaddr,   uint32 snmask,
    uint32 dns1,     uint32 dns2,
    uint32 gateway );

/*****************************************************************************/
// Description : set tcpip net parameters both for ipv4 and ipv6
// Global : None 
// Author : zhaodi.chen
// Note : called by GPRS, all input IP addresses are the same as 'Local-Endian'.
//        PARAM : ipaddr_cfg_ptr  (IN) - register net interface's ip information     
//        RETURN : 0 if OK, else error!      
/*****************************************************************************/
PUBLIC int32 TCPIP_SetNetInterfaceExt(TCPIP_NETIF_IPADDR_CFG_T* ipaddr_cfg_ptr);

/*****************************************************************************/
// Description : reset tcpip net parameters
// Global : None 
// Author : yifeng.wang
// Note : called by GPRS
//        PARAM1 : nsapi (IN) - NSAPI of deactived GPRS
//        RETURN : 0 if OK, else error!
/*****************************************************************************/
PUBLIC int32 TCPIP_ResetNetInterface( uint8 nsapi ); /*lint -esym(765,TCPIP_ResetNetInterface) */
#if 0
/*****************************************************************************/
// Description : send backup packet
// Global resource : 
// Author : yifeng.wang
// Note :  PARAM1 : nsapi (IN) - GPRS NSAPI
//         when mobile network switched(e.g. TD->GSM) or updated, GPRS should
//         call this to upload backup packet to hold data downlink.
/*****************************************************************************/
PUBLIC void TCPIPPS_SendBackupPkt( uint8 nsapi ); /*lint -esym(765,TCPIPPS_SendBackupPkt) */
#endif
/*****************************************************************************/
// Description : transfer packet from AP to PS through TCPIP
// Global : none
// Author : yifeng.wang
// Notes  : PARAM1: data_ptr (IN) -- packet pointer
//          PARAM2: data_len (IN) -- packet length
//          PARAM3: nsapi (IN) -- NSAPI of wireless stack
//          RETURN: 0 - successfully received; else - error
/*****************************************************************************/
PUBLIC int32 TCPIPPS_RecvFromAp(uint8* data_ptr, uint32 data_len, uint8 nsapi);

/*************************************************************************/
// Description : set pclink net interface
// Global resource : 
// Author : yifeng.wang
// Note : This interface is just left to adapt to the old platform.
//        We recommend using TCPIPPCLINK_RegNetInterface() instead.
/*************************************************************************/
PUBLIC BOOLEAN TCPIPPCLINK_SetNetInterface( void );

/*************************************************************************/
// Description : reset pclink net interface
// Global resource : 
// Author : yifeng.wang
// Note : This interface is just left to adapt to the old platform.
//        We recommend using TCPIPPCLINK_DeregNetInterface() instead.
/*************************************************************************/
PUBLIC BOOLEAN TCPIPPCLINK_ResetNetInterface( void );

/*****************************************************************************/
// Description: set data tx rate
// Global : none
// Author : wei.chen
// Note : 
//        PARAM1: tx_rate (IN) -- max tx rate, unit( byte/s ), 0 means default
//        PARAM2: nsapi (IN) - NSAPI of deactived GPRS
//        when low bearer set tx flow control, all packets from app will be 
//        held in tcpip to tx pending queue; when flow control be reset, 
//        tcpip will send these pending packets at maximum rate as tx_rate set!
/*****************************************************************************/
PUBLIC void TCPIPPS_SetTxRate( uint32 tx_rate, uint8 nsapi );


#endif  /* _PS_TCP_IF_H_ */