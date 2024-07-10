/*****************************************************************************
 ** File Name:     net_test.h                                                *
 ** Author:        yifeng.wang                                               *
 ** Date:          2008/03/03                                                *
 ** Copyright:     2008 Spreadtrum, Incorporated. All Rights Reserved.       *
 ** Description:   interfaces for network test                               *
 *****************************************************************************
 **                        Edit History                                      *
 ** -------------------------------------------------------------------------*
 ** DATE           NAME                   DESCRIPTION                        *
 ** 2008/02/27     yifeng.wang            Create                             *
 ** 2010/08/31     yifeng.wang            Modify                             *
 *****************************************************************************/

#ifndef _NET_TEST_H_
#define _NET_TEST_H_

/**--------------------------------------------------------------------------*
 **                          Include Files                                   *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                          MACRO DEFINITION                                *
 **--------------------------------------------------------------------------*/
#define ETHERR_OK                           0
#define ETHERR_INVALID_INPUT                -1
#define ETHERR_NO_AVAIL_IP                  -11
#define ETHERR_INVALID_PEER_MAC             -12
#define ETHERR_RECVTASK_NOTCREATED          -80
#define ETHERR_RECVTASK_EVENTINITFAILED     -81
#define ETHERR_RECVTASK_TASKCREATEFAILED    -82
#define ETHERR_RECVTASK_SETRCVBUFFFAILED    -83
#define ETHERR_RECVTASK_SETRCVTIMEOUTFAILED -84
#define ETHERR_RECVTASK_ALREADYRUNNING      -85
#define ETHERR_RECVTASK_INVALIDPARAM        -86
#define ETHERR_ADAPTER_OPEN_FAILED          -100
#define ETHERR_PKTALLOC_FAILED              -101
#define ETHERR_SETPKTNWRITES_FAILED         -102
#define ETHERR_SEND_PACKET_FAILED           -103

#define NET_CONN_TEST_TIME_OUT              2000
#define NET_IP_TEST_TIME_OUT                1500
#define NET_MAC_TEST_TIME_OUT               1500
#define NET_PING_TEST_TIME_OUT              2000


/**--------------------------------------------------------------------------*
 **                          TYPE DEFINITION                                 *
 **--------------------------------------------------------------------------*/
typedef enum    /* network test type */
{
    TYPE_IDLE       = 0,
    TYPE_TEST_CONN  = 1, 
    TYPE_TEST_IP    = 2,
    TYPE_TEST_MAC   = 3,
    TYPE_TEST_PING  = 4,
} NETTEST_TYPE_E;

/**--------------------------------------------------------------------------*
 **                           FUNCTIONS                                      *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : get adapter name
// Global resource : s_adapter_name
// Author: yifeng.wang 
// Note:  
/*****************************************************************************/
const char* NETTEST_GetDeviceName( void );

/*****************************************************************************/
// Description : send arp test packet
// Global resource : 
// Author: yifeng.wang 
// Note:  
/*****************************************************************************/
int  NETTEST_SendNetTestData( NETTEST_TYPE_E type, BYTE* mac_src, DWORD ip_src, DWORD ip_dst );

/*****************************************************************************/
// Description : send ping test packet
// Global resource : 
// Author: yifeng.wang 
// Note:  
/*****************************************************************************/
int  NETTEST_SendPingTestData( NETTEST_TYPE_E type, BYTE* mac_src, BYTE* mac_dst, DWORD ip_src, DWORD ip_dst );

/*****************************************************************************/
// Description : create ethernet recv task
// Global resource : 
// Author: yifeng.wang 
// Note:  
/*****************************************************************************/
BOOL NETTEST_CreateEthernetTask( HWND wnd_handle, char* device_name );

/*****************************************************************************/
// Description : close ethernet receive task
// Global resource : 
// Author: yifeng.wang 
// Note:  
/*****************************************************************************/
void NETTEST_CloseEthernetTask( void );

/*****************************************************************************/
// Description : check if the ethernet receive task is running
// Global resource : 
// Author: yifeng.wang 
// Note:  
/*****************************************************************************/
BOOL NETTEST_IsEthernetTaskRun( void );

/*****************************************************************************/
// Description : get ethernet receive task error
// Global resource : 
// Author: yifeng.wang 
// Note:  
/*****************************************************************************/
int  NETTEST_GetEthernetTaskError( void );

/*****************************************************************************/
// Description : set test start and holding time
// Global resource : 
// Author: yifeng.wang 
// Note:  
/*****************************************************************************/
void NETTEST_SetTestTime( DWORD time_start, DWORD time_holding );

/*****************************************************************************/
// Descrption : get last ethernet error message
// Global resource : 
// Author: yifeng.wang 
// Note: 
/*****************************************************************************/
CString NETTEST_GetLastErrorMsg( void );

/*****************************************************************************/
// Descrption : get ethernet error message by error number
// Global resource : 
// Author: yifeng.wang 
// Note: 
/*****************************************************************************/
CString NETTEST_GetErrorMsg( int error_no );

/*****************************************************************************/
// Description : set peer IP
// Global resource : 
// Author: yifeng.wang 
// Note: set peer IP is to record its MAC from ARP reply
/*****************************************************************************/
void NETTEST_SetPeerIp( DWORD ip_peer );

/*****************************************************************************/
// Description : get matched peer MAC
// Global resource : 
// Author: yifeng.wang 
// Note: get MAC address of given IP address
/*****************************************************************************/
BYTE* NETTEST_GetPeerMac( DWORD ip_peer );


///////////////////////////////////////////////////////////////////////////////
#endif  /* _NET_TEST_H */