/*****************************************************************************
 ** File Name:     atc_tcp_if.h                                              *
 ** Author:        William.Qian                                              *
 ** Date:          2006/02/27                                                *
 ** Copyright:     2006 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:   tcpip interfaces for ATC called.                          *
 *****************************************************************************
 *****************************************************************************
 **                        Edit History                                      *
 ** -------------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                              *
 ** 2006/02/27     William.Qian     Create.                                  *
 ** 2006/03/31     Fancier.Fan      Amend                                    *
 ** 2008/01/31     Hyman.Wu         Amend for PPP debug                      *
 ** 2009/08/26     Yifeng.Wang      Link to new interface file               *
 ** 2009/11/29     Yifeng.Wang      Amend for unified TCPIP                  *
 *****************************************************************************/
#ifndef _ATC_TCP_IF_H_
#define  _ATC_TCP_IF_H_

#include "sci_types.h"
#include "tcpip_types.h"
#include "tcpip_api.h"

/*---------------------------------------------------------------------------*
 *                         FUNCTION DEFINITION                               *
 *---------------------------------------------------------------------------*/
/* modem mode configuration */
typedef struct tcpipps_modem_cfg
{
    uint8               nsapi;      /* PS NSAPI */
    uint8               link_id;    /* link-layer ID, mark a modem link tunnel */
    uint8               cid;        /* PS context ID */
    TCPIP_PACKET_TYPE_E pkt_type;   /* packet type between tcpip and modem */
} TCPIPPS_MODEM_CFG_T;


/*---------------------------------------------------------------------------*
 *                         FUNCTION DEFINITION                               *
 *---------------------------------------------------------------------------*/
 #if 0
/*****************************************************************************/
// Description : start MODEM service
// Global : None 
// Author : yifeng.wang
// Note : called by AT module when its got PDP active CNF(confirm) and
//        to enter MODEM mode, this API for single-PPP with single-PDP
/*****************************************************************************/ 
PUBLIC void GPRSTCPIP_SetPPPService( void );
#endif
/*****************************************************************************/
// Description : end MODEM service
// Global : None 
// Author : yifeng.wang
// Note : called by AT module when its got PDP deactive CNF/IND and 
//        to exit PPP mode, this API is for single-PPP with single-PDP
/*****************************************************************************/ 
PUBLIC void GPRSTCPIP_SetTCPUDPService( void );

/*****************************************************************************/
// Description : start MODEM service
// Global : None 
// Author : yifeng.wang
// Note : called by AT module to enter MODEM mode when AT received 
//        PDP active confirm, this API is for GPRS multi-PDP!
//        PARAM1 : nsapi (IN) - GPRS NSAPI
//        RETURN : 0 if OK; else fail
/*****************************************************************************/
PUBLIC int32 TCPIP_SetPPPInterface( uint8 nsapi );

/*****************************************************************************/
// Description : end MODEM service
// Global : None 
// Author : yifeng.wang
// Note : called by AT module to exit MODEM mode when AT received
//        PDP deactive confirm/indication, this API is for GPRS multi-PDP!
//        RETURN : 0 if OK; else fail
/*****************************************************************************/ 
PUBLIC int32 TCPIP_ResetPPPInterface( void );

/*****************************************************************************/
// Description : start MODEM service
// Global : None 
// Author : yifeng.wang
// Note : called by AT module to enter MODEM mode when AT received 
//        PDP active confirm, this API is for GPRS multi-PDP with MUX support!
//        PARAM1 : nsapi (IN) - GPRS NSAPI
//        PARAM2 : link_id (IN) - MUX link ID
//        PARAM3 : cid (IN) - i.e. PDP ID
//        RETURN : 0 if OK; else fail
/*****************************************************************************/
PUBLIC int32 MPPP_SetPPPInterface( uint8 nsapi, uint8 link_id, uint8 cid );

/*****************************************************************************/
// Description : end MODEM service
// Global : None 
// Author : yifeng.wang
// Note : called by AT module to exit MODEM mode when AT received PDP deactive
//        confirm/indication, this API is for GPRS multi-PDP with MUX support!
//        PARAM1 : cid (IN) - i.e. PDP ID
//        RETURN : 0 if OK; else fail
/*****************************************************************************/
PUBLIC int32 MPPP_ResetPPPInterface( uint8 cid );

/*****************************************************************************/
// Description : register modem service
// Global : None 
// Author : yifeng.wang
// Note : PARAM : cfg_ptr (IN) -- modem service configuration pointer
//        RETURN: TCPIP_ERROR_OK - successful ; else - failed.
/*****************************************************************************/
PUBLIC TCPIP_ERROR_E TCPIPPS_SetModemService( const TCPIPPS_MODEM_CFG_T* cfg_ptr );

/*****************************************************************************/
// Description : deregister modem service
// Global : None 
// Author : yifeng.wang
// Note : PARAM : cid (IN) -- PS context ID
/*****************************************************************************/
PUBLIC void TCPIPPS_ResetModemService( uint8 cid );

/*****************************************************************************/
// Description : disconnect PPP and close MODEM service for MUX release
// Global : None 
// Author : yifeng.wang
// Note : called by AT module to disconnect PPP and close MODEM service
//        immediately when AT received MUX release event!
//        PARAM1 : cid (IN) - i.e. PDP ID
//        RETURN : 0 if OK; else fail
//        NOTE: disconnect immediately means no waiting for peer PPP IPCP/LCP ACK!
/*****************************************************************************/
PUBLIC void ppp_disconnect_for_mux_release( uint8 link_id );

/*****************************************************************************/
// Description : set a callback to indicate up layer.
// Global : None 
// Author : Shijun.cui
// Note : 2005-07-06 CR27556
//        PARAM1 : so (IN) - socket handle
//        RETURN : fin (IN) - socket fin flag, 1 - socket closed
//        NOTE: this interface is not used any more! Here just for PCLINT OK!
/*****************************************************************************/
PUBLIC void tcp_set_callback( void (*rcv_callback)(uint32 so, int32 fin) );


#endif /* _ATC_TCP_IF_H_ */
