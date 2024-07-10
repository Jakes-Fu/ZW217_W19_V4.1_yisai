/*****************************************************************************
 ** File Name:     net_test.c                                                *
 ** Author:        yifeng.wang                                               *
 ** Date:          2008/03/03                                                *
 ** Copyright:     2008 Spreadtrum, Incorporated. All Rights Reserved.       *
 ** Description:   interfaces for network test                               *
 *****************************************************************************
 **                        Edit History                                      *
 ** -------------------------------------------------------------------------*
 ** DATE           NAME                   DESCRIPTION                        *
 ** 2008/03/03     yifeng.wang            Create                             *
 ** 2010/08/31     yifeng.wang            Modify                             *
 *****************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Libraries                                *
 **--------------------------------------------------------------------------*/
#include    <stdafx.h>

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include    "Packet32.h"        // WINPCAP SDK header
#include    "ntddndis.h"        // WINPCAP SDK header
#include    "net_test.h"        // ethernet para initialization from file

    
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define     MAX_ETHERNET_PKT_LEN        1514
#define     MAX_ETHERNET_PKT_CHAR_LEN   ((MAX_ETHERNET_PKT_LEN)*3 + 10)
#define     MAX_ADAPTER_BUFF            512000
#define     MAX_PACKET_BUFF             8192
#define     MAX_RECEIVE_BUFF            256000
#define     RECEIVE_TIME_OUT            50      // millisecond
#define     MAX_ADAPTER_NAME            256
#define     ADAPTER_NAME_PREFIX         "\\Device\\NPF_"
              
// ethernet receive task para
#define     ETHERNET_TASK_STACK_SIZE    0x400   // 1 KB
#define     ETHERNET_TASK_QUEUE_NUM     7
#define     ETHERNET_TASK_PRORITY       25


/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
static u_char   s_mac_src[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static u_char   s_mac_dst[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static DWORD    s_ip_src = 0;
static DWORD    s_ip_dst = 0;

static u_char   s_mac_peer[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static DWORD    s_ip_peer = 0;

static WORD     s_ip_id = 0;
static WORD     s_ping_id = 0, s_ping_seq = 0;

static char     s_adapter_name[MAX_ADAPTER_NAME];
static LPADAPTER s_adapter_ptr = NULL;

static CWinThread* s_ether_recv_task_ptr = NULL;
static HANDLE s_ether_task_event = NULL;
static BOOL   s_is_ether_task_run = FALSE;
static int    s_ether_task_errno = ETHERR_RECVTASK_NOTCREATED;
static HWND   s_window_handle = NULL;

static NETTEST_TYPE_E s_nettest_type = TYPE_IDLE;

static DWORD s_test_start_time = 0;
static DWORD s_test_holding_time = 0;

// it should be matched with NETTEST_TYPE_E
static char s_nettest_type_notes[][16] =
{
    "IDLE",
    "Conn Test",
    "IP Test",
    "MAC Test",
    "PING Test"
};


/*---------------------------------------------------------------------------*
/*                          static FUNCTION DECLARE                           *
/*---------------------------------------------------------------------------*/
static UINT EthernetReceiveTaskEntry( LPVOID pParam );
static int  SendEthernetPacket( unsigned char* pdu_ptr, unsigned long pdu_len );
static BOOL ParseReceivedData( LPPACKET packet_ptr );
static BOOL OpenEthernetAdapter( char* adapter_name, char* notes_ptr );
static BOOL CloseEthernetAdapter( void );
static void AnalyseRecvedPacket( u_char *pkt_header, unsigned int len );
static BOOL IsTestTimeOut( void );
static unsigned short CheckSum( void* ptr, unsigned count );
//static BOOL SetAdapterName( char* adapter_name );


/*---------------------------------------------------------------------------*
/*                          FUNCTIONS                                        *
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : get adapter name
// Global resource : s_adapter_name
// Author: yifeng.wang 
// Note:  
/*****************************************************************************/
const char* NETTEST_GetDeviceName( void )
{
    return s_adapter_name;
}

/*****************************************************************************/
// Description : 
// Global resource : 
// Author: yifeng.wang 
// Note:  
/*****************************************************************************/
int  NETTEST_SendNetTestData( NETTEST_TYPE_E type, u_char* mac_src, DWORD ip_src, DWORD ip_dst )
{
    int ret_val;
    if( NULL == mac_src || TYPE_IDLE == type )
    {
        return ETHERR_INVALID_INPUT;
    }

    // record test para
    s_nettest_type = type;
    memcpy( s_mac_src, mac_src, sizeof(s_mac_src) );
    s_ip_src = ip_src;
    s_ip_dst = ip_dst;

    // create and send ARP request
    unsigned char arpreq_pkt[] = /* raw ARP request packet */
    { 
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,   0xff, 0xff, 0xff, 0xff, 0x08, 0x06, 0x00, 0x01, 
        0x08, 0x00, 0x06, 0x04, 0x00, 0x01, 0xff, 0xff,   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,   0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00
    };

    memcpy( &arpreq_pkt[6],  mac_src, 6 );               // set sender MAC
    memcpy( &arpreq_pkt[22], mac_src, 6 );               // set ARP sender MAC
    memcpy( &arpreq_pkt[28], &ip_src, sizeof(ip_src) );  // set ARP sender IP
    memcpy( &arpreq_pkt[38], &ip_dst, sizeof(ip_dst) );  // set ARP target IP

    // send test data
    ret_val = SendEthernetPacket( arpreq_pkt, sizeof(arpreq_pkt) );
    if( ret_val != ETHERR_OK )
    {
        s_nettest_type = TYPE_IDLE;
    }
    return ret_val;
}

/*****************************************************************************/
// Description : send ping test packet
// Global resource : 
// Author: yifeng.wang 
// Note:  
/*****************************************************************************/
int  NETTEST_SendPingTestData( NETTEST_TYPE_E type, BYTE* mac_src, BYTE* mac_dst, DWORD ip_src, DWORD ip_dst )
{
    int ret_val = 0;
    unsigned short checksum = 0;

    if( NULL == mac_src || NULL == mac_dst || TYPE_IDLE == type )
    {
        return ETHERR_INVALID_INPUT;
    }

    // record test para
    s_nettest_type = type;
    memcpy( s_mac_src, mac_src, sizeof(s_mac_src) );
    memcpy( s_mac_dst, mac_dst, sizeof(s_mac_dst) );
    s_ip_src = ip_src;
    s_ip_dst = ip_dst;

    // create and send PING request data
    unsigned char packet[] = /* raw PING packet */
    { 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x45, 0x00, 
        0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01,   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
        0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,   0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
        0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,   0x61, 0x61
    };

    // set MAC header
    memcpy( &packet[0],  mac_dst, 6 );      // dest MAC address
    memcpy( &packet[6],  mac_src, 6 );      // source MAC address

    // set IP header
    s_ip_id++;
    memcpy( &packet[18], &s_ip_id, sizeof(s_ip_id) );   // set IP ID
    memcpy( &packet[26], &ip_src, sizeof(ip_src) );     // set source IP
    memcpy( &packet[30], &ip_dst, sizeof(ip_dst) );     // set dest IP
    checksum = ~CheckSum(packet + 14, 10);
    memcpy( &packet[24], &checksum, sizeof(checksum) );  // set IP checksum

    // set PING data
    srand(GetTickCount());
    s_ping_id = rand()%0xFFFF;
    s_ping_seq = rand()%0xFFFF;
    memcpy( &packet[38], &s_ping_id, sizeof(s_ping_id) );
    memcpy( &packet[40], &s_ping_seq, sizeof(s_ping_seq) );
    checksum = ~CheckSum(packet + 34, 20);
    memcpy( &packet[36], &checksum, sizeof(checksum) );  // set PING checksum

    // send packet
    ret_val = SendEthernetPacket( packet, sizeof(packet) );
    if( ret_val != ETHERR_OK )
    {
        s_nettest_type = TYPE_IDLE;
    }
    return ret_val;
}

/*****************************************************************************/
// Description : create ethernet recv task
// Global resource : s_nettest_type
// Author: yifeng.wang 
// Note:  
/*****************************************************************************/
BOOL NETTEST_CreateEthernetTask( HWND wnd_handle, char* device_name )
{
    // input check
    if( NULL == wnd_handle || NULL == device_name )
    {
        TRACE("[ ETHERNET TASK ] : NETTEST_CreateEthernetTask - invalid input!\n");
        s_ether_task_errno = ETHERR_RECVTASK_INVALIDPARAM;
        return FALSE;
    }

    // check if task is running 
    if( s_is_ether_task_run )
    {
        if( 0 == strcmp(device_name, s_adapter_name) )
        {
            TRACE("[ ETHERNET TASK ] : Ethernet task is already running!\n");
            s_ether_task_errno = ETHERR_OK;
            return TRUE;
        }
        else
        {
            TRACE("[ ETHERNET TASK ] : NETTEST_CreateEthernetTask - to close current ethernet task!\n");
            NETTEST_CloseEthernetTask();
            Sleep(500);   
        }
    }

    // close old net adapter
    CloseEthernetAdapter();

    // set and open new net adapter
    OpenEthernetAdapter(device_name, "NETTEST_CreateEthernetTask");

    // create ethernet task event
    if( s_ether_task_event == NULL )
    {
        s_ether_task_event = CreateEvent( NULL, FALSE, FALSE, "EtherTaskCloseEvent" );
        if ( NULL == s_ether_task_event )
        {
            TRACE("[ NET TEST ] Create ethernet task event FAILED! err = %d\n", GetLastError() );
            s_ether_task_errno = ETHERR_RECVTASK_EVENTINITFAILED;
            return FALSE;
        }
    }
    else
    {
        TRACE("[ ETHERNET TASK ] : NETTEST_CreateEthernetTask - old ethernet task is still running!\n");
        s_ether_task_errno = ETHERR_RECVTASK_ALREADYRUNNING;
        return FALSE;
    }

    // create ethernet thread
    s_window_handle = wnd_handle;
    s_ether_recv_task_ptr = AfxBeginThread( EthernetReceiveTaskEntry, wnd_handle );
    if( NULL == s_ether_recv_task_ptr )
    {
        TRACE("[ ETHERNET TASK ] : Create ethernet receive task failed!\n");
        s_ether_task_errno = ETHERR_RECVTASK_TASKCREATEFAILED;
        return FALSE;
    }
    else
    {
        s_is_ether_task_run = TRUE;
        return TRUE;
    }
}

/*****************************************************************************/
// Description : close ethernet receive task
// Global resource : 
// Author: yifeng.wang 
// Note:  
/*****************************************************************************/
void NETTEST_CloseEthernetTask( void )
{
    if ( !s_is_ether_task_run )
        return;

    // set ethernet task close event
    if ( !SetEvent( s_ether_task_event ) )
    {
        TRACE("[ ETHERNET TASK ] Set ethernet task event FAILED! err = %d\n", GetLastError());
    }
}

/*****************************************************************************/
// Description : check if the ethernet receive task is running
// Global resource : 
// Author: yifeng.wang 
// Note:  
/*****************************************************************************/
BOOL NETTEST_IsEthernetTaskRun( void )
{
    return s_is_ether_task_run;
}

/*****************************************************************************/
// Description : set test start and holding time
// Global resource : 
// Author: yifeng.wang 
// Note:  
/*****************************************************************************/
void NETTEST_SetTestTime( DWORD time_start, DWORD time_holding )
{
    s_test_start_time = GetTickCount();
    s_test_holding_time = time_holding;
    TRACE("testing notes(%ld): start_time(%ld), hold_time(%d)\n", GetTickCount(), s_test_start_time, time_holding);
}

/*****************************************************************************/
// Description : get ethernet receive task error
// Global resource : 
// Author: yifeng.wang 
// Note:  
/*****************************************************************************/
int  NETTEST_GetEthernetTaskError( void )
{
    return s_ether_task_errno;
}

/*****************************************************************************/
// Descrption : get ethernet error message
// Global resource : 
// Author: yifeng.wang 
// Note: 
/*****************************************************************************/
CString NETTEST_GetLastErrorMsg( void )
{
    return NETTEST_GetErrorMsg( NETTEST_GetEthernetTaskError() );
}

/*****************************************************************************/
// Descrption : get ethernet error message by error number
// Global resource : 
// Author: yifeng.wang 
// Note: 
/*****************************************************************************/
CString NETTEST_GetErrorMsg( int error_no )
{
    CString strMsg("");
    switch( error_no )
    {
    case ETHERR_OK:
        strMsg.Format(_T("no error!"), error_no);
        break;

    case ETHERR_INVALID_INPUT:
        strMsg.Format(_T("ERROR(%d): invalid input!"), error_no);
        break;

    case ETHERR_NO_AVAIL_IP:
        strMsg.Format(_T("ERROR(%d): no available IP found!"), error_no);
        break;

    case ETHERR_INVALID_PEER_MAC:
        strMsg.Format(_T("ERROR(%d): invalid peer MAC!"), error_no);
        break;

    case ETHERR_ADAPTER_OPEN_FAILED:
        strMsg.Format(_T("ERROR(%d): open adapter failed!"), error_no);
        break;
        
    case ETHERR_PKTALLOC_FAILED:
        strMsg.Format(_T("ERROR(%d): alloc packet buffer failed!"), error_no);
        break;
        
    case ETHERR_SETPKTNWRITES_FAILED:
        strMsg.Format(_T("ERROR(%d): set packet write times failed!"), error_no);
        break;

    case ETHERR_SEND_PACKET_FAILED:
        strMsg.Format(_T("ERROR(%d): send packet failed!"), error_no);
        break;
        
    case ETHERR_RECVTASK_NOTCREATED:
        strMsg.Format(_T("ERROR(%d): receiving task not created!"), error_no);
        break;
      
    case ETHERR_RECVTASK_EVENTINITFAILED:
        strMsg.Format(_T("ERROR(%d): receiving task event create failed!"), error_no);
        break;

    case ETHERR_RECVTASK_TASKCREATEFAILED:
        strMsg.Format(_T("ERROR(%d): create receiving task failed!"), error_no);
        break;
        
    case ETHERR_RECVTASK_SETRCVBUFFFAILED:
        strMsg.Format(_T("ERROR(%d): set receive buffer failed!"), error_no);
        break;

    case ETHERR_RECVTASK_SETRCVTIMEOUTFAILED:
        strMsg.Format(_T("ERROR(%d): set receive time out failed!"), error_no);
        break;

    case ETHERR_RECVTASK_ALREADYRUNNING:
        strMsg.Format(_T("ERROR(%d): old receiving task still running!"), error_no);
        break;

    case ETHERR_RECVTASK_INVALIDPARAM:
        strMsg.Format(_T("ERROR(%d): invalid param for receiving task!"), error_no);
        break;

    default:
        strMsg.Format(_T("ERROR(%d): unknown error!"), error_no);
        break;
    }
    return strMsg;
}

/*****************************************************************************/
// Description : set peer IP
// Global resource : 
// Author: yifeng.wang 
// Note: set peer IP is to record its MAC from ARP reply
/*****************************************************************************/
void NETTEST_SetPeerIp( DWORD ip_peer )
{
    s_ip_peer = ip_peer;
    //TRACE("yifeng debug: s_ip_peer = %u.%u.%u.%u\n", PUSH_IPADDR(s_ip_peer));
}

/*****************************************************************************/
// Description : get matched peer MAC
// Global resource : 
// Author: yifeng.wang 
// Note: get MAC address of given IP address
/*****************************************************************************/
BYTE* NETTEST_GetPeerMac( DWORD ip_peer )
{
    if( ip_peer == s_ip_peer )
    {
        return s_mac_peer;
    }
    else
    {
        return NULL;
    }
}

///////////////////////////////////////////////////////////////////////////////
/*****************************************************************************/
// Description : set net adapter name
// Global resource : s_adapter_name
// Author: yifeng.wang 
// Note:  
/*****************************************************************************/
//BOOL SetAdapterName( char* adapter_name )
//{
//    if( NULL == adapter_name )
//        return FALSE;
//
//    strncpy(s_adapter_name, adapter_name, MAX_ADAPTER_NAME);
//    return TRUE;
//}

/*****************************************************************************/
// Description: open ethernet adapter
// Global resource : s_adapter_name, s_adapter_ptr
// Author: yifeng.wang
/*****************************************************************************/
static BOOL OpenEthernetAdapter( char* adapter_name, char* notes_ptr )
{
    char device_name[MAX_ADAPTER_NAME] = {0};

    // check if adapter is opened
    if( NULL != s_adapter_ptr && 0 == strcmp(adapter_name, s_adapter_name) )
    {
        TRACE("OpenEthernetAdapter() - adapter is already opened!\n"); 
        return TRUE;
    }

    // adapter name check
    if( NULL == s_adapter_name )
    {
        TRACE("OpenEthernetAdapter() - adapter name is NULL!\n"); 
        return FALSE;
    }

    if( NULL != s_adapter_ptr )
    {
        CloseEthernetAdapter();
    }

    // update adapter name
    _snprintf(s_adapter_name, MAX_ADAPTER_NAME-1, "%s", adapter_name);

    // check device name space
    if( MAX_ADAPTER_NAME > (strlen(ADAPTER_NAME_PREFIX) + strlen(s_adapter_name)) )
    {
        sprintf(device_name, "%s%s", ADAPTER_NAME_PREFIX, s_adapter_name);
    }
    else
    {
        TRACE("OpenEthernetAdapter() - adapter name space %d is not big enough!\n", MAX_ADAPTER_NAME); 
        return FALSE;
    }

    // open net adapter
    s_adapter_ptr = PacketOpenAdapter( device_name );  // open network adapter
    if( NULL == s_adapter_ptr || INVALID_HANDLE_VALUE == s_adapter_ptr->hFile )
    {
        TRACE("Unable Open Ethernet Adapter \"%s\"! Error Code : 0x%lx!\n", device_name, GetLastError()); 
        return FALSE;
    }	    

    //TRACE("Open ethernet adapter(%s) : %s\n", notes_ptr, device_name);
    return TRUE;
}

/*****************************************************************************/
// Description: close ethernet adapter
// Global resource : s_adapter_ptr
// Author: yifeng.wang
/*****************************************************************************/
static BOOL CloseEthernetAdapter( void )
{
    PacketCloseAdapter( s_adapter_ptr );    // close the adapter and exit
    s_adapter_ptr = NULL;
    return TRUE;
}

/*****************************************************************************/
// Description: ethernet packet send
// Global resource : s_adapter_ptr
// Author: yifeng.wang  
/*****************************************************************************/
static int SendEthernetPacket( unsigned char* pdu_ptr, unsigned long pdu_len )
{
    char packet_char[MAX_ETHERNET_PKT_CHAR_LEN];
    unsigned char packet_buff[MAX_PACKET_BUFF];
    LPPACKET      packet_ptr = NULL;  // pointer to a PACKET structure
    
    ASSERT( NULL!=pdu_ptr && pdu_len>0 );
    
    // identify whether ethernet adapter is opened
    if ( NULL == s_adapter_ptr )    
    {
        if ( !OpenEthernetAdapter(s_adapter_name, "SendEthernetPacket") )
        {
            TRACE("%s(%i) : Open ethernet adapter failed!\n", __FILE__, __LINE__ );
            return ETHERR_ADAPTER_OPEN_FAILED;
        }
    }

    // allocate a packet memory
    packet_ptr = PacketAllocatePacket();
    if ( NULL == packet_ptr )
    {
        TRACE("%s(%i) : PacketAllocatePacket() error!\n", __FILE__, __LINE__ );
        return ETHERR_PKTALLOC_FAILED;
    }

    ASSERT( MAX_ETHERNET_PKT_LEN >= pdu_len );
    memcpy( packet_buff, pdu_ptr, pdu_len );
    PacketInitPacket( packet_ptr, packet_buff, pdu_len ); // initialize packet struct

    // set repeat times of packet send
    if ( !PacketSetNumWrites( s_adapter_ptr, 1 ) )
    {
        TRACE("%s(%i) : PacketSetNumWrites() failed!\n", __FILE__, __LINE__);
        PacketFreePacket( packet_ptr );     // free packet
        return ETHERR_SETPKTNWRITES_FAILED;
    }
    
    // send the packet
    if ( !PacketSendPacket( s_adapter_ptr, packet_ptr, TRUE ) )
    {
        TRACE("%s(%i) : PacketSendPacket() failed!\n", __FILE__, __LINE__);
        PacketFreePacket( packet_ptr );     // free packet
        return ETHERR_SEND_PACKET_FAILED;
    }

    TRACE("[ ETHERNET TEST ] %s - send packet(%ld):\n - ", s_nettest_type_notes[s_nettest_type], GetTickCount());
    for (int i = 0; i < (int)pdu_len; i++ )
    {
        ASSERT((3*i+2) < MAX_ETHERNET_PKT_CHAR_LEN);
        packet_char[3*i] = '0' + pdu_ptr[i]/16 + ((pdu_ptr[i]/16<10)? 0:7);
        packet_char[3*i+1] = '0' + pdu_ptr[i]%16 + ((pdu_ptr[i]%16<10)? 0:7);
        packet_char[3*i+2] = ' ';
    }
    ASSERT(3*pdu_len < MAX_ETHERNET_PKT_CHAR_LEN);
    packet_char[3*pdu_len] = 0x00;
    TRACE("%s\n", packet_char);

    //TRACE("yifengdebug: SendEthernetPacket - to PacketFreePacket!\n");
    PacketFreePacket( packet_ptr ); // free packet
    packet_ptr = NULL;
    
    /* 注意：这里不能进行 CloseEthernetAdapter 的操作，因为接收线程还需要用 */
    return ETHERR_OK;
}

/*****************************************************************************/
//  Description : ethernet receive task entry
//  Global resource : s_adapter_ptr, s_window_handle
//  Author : Yifeng.Wang
/*****************************************************************************/
static UINT EthernetReceiveTaskEntry( LPVOID pParam )
{
    s_is_ether_task_run = TRUE;         // 线程运行标志
    s_window_handle = (HWND)pParam;     // refresh Window Handle

    struct _PACKET  packet;                     // define a PACKET structure
    unsigned char   buffer[MAX_RECEIVE_BUFF];   // buffer to hold the data coming from the driver
    
    // identify whether ethernet adapter is selected
    if ( NULL == s_adapter_ptr )    
    {
        if ( !OpenEthernetAdapter(s_adapter_name, "EthernetReceiveTaskEntry") )
        {
            TRACE("[ ETHERNET TASK ] Open ethernet adapter failed!\n");
            s_ether_task_errno = ETHERR_ADAPTER_OPEN_FAILED;
            goto ETHRCVTASK_NODEVOPEN_END;
        }
    }

    // set the network adapter in promiscuous mode
    //TRACE("yifengdebug: [ ETHERNET TASK ] to PacketSetHwFilter!\n");
    if ( !PacketSetHwFilter(s_adapter_ptr, NDIS_PACKET_TYPE_PROMISCUOUS) )
    {
        TRACE("[ ETHERNET TASK ] Warning: unable to set promiscuous mode!\n");
    }
    
    // set adapter buffer
    //TRACE("yifengdebug: [ ETHERNET TASK ] to PacketSetBuff!\n");
    if ( !PacketSetBuff(s_adapter_ptr, MAX_ADAPTER_BUFF) )
    {
        TRACE("[ ETHERNET TASK ] Unable to set the kernel buffer!\n" );
        s_ether_task_errno = ETHERR_RECVTASK_SETRCVBUFFFAILED;
        goto ETHRCVTASK_DEVOPEN_END;
    }
    
    // set packet receive timeout
    //TRACE("yifengdebug: [ ETHERNET TASK ] to PacketSetReadTimeout!\n");
    if ( !PacketSetReadTimeout(s_adapter_ptr, RECEIVE_TIME_OUT) )
    {
        TRACE("[ ETHERNET TASK ] Warning: unable to set read time-out value!\n" );
        s_ether_task_errno = ETHERR_RECVTASK_SETRCVTIMEOUTFAILED;
        goto ETHRCVTASK_DEVOPEN_END;
    }

    // initial received packet
    //TRACE("yifengdebug: [ ETHERNET TASK ] to PacketInitPacket!\n");
    memset( &packet, 0x00, sizeof(packet) );
    PacketInitPacket( &packet, buffer, sizeof(buffer) );
    s_ether_task_errno = ETHERR_OK;

    // packet receive loop
    //TRACE("yifengdebug: [ ETHERNET TASK ] EthernetReceiveTaskEntry - to call while\n");
    while (1)
    {    
        // wait for the event to close current thread
        if ( WAIT_OBJECT_0 == WaitForSingleObject( s_ether_task_event, 20 ) )
        {
            //TRACE("[ ETHERNET TASK ] Get event for close ethernet receiving thread!\n" ); 
            s_ether_task_errno = ETHERR_RECVTASK_NOTCREATED;
            break;
        }
        
        // check whether the test is time-out
        //TRACE("yifengdebug: [ ETHERNET TASK ] in while - to PacketReceivePacket\n");
        if ( ( TYPE_IDLE != s_nettest_type ) && !IsTestTimeOut() )
        {   
            // get and analyse received packet
            //TRACE("yifengdebug: [ ETHERNET TASK ] in while - to PacketReceivePacket\n");
            if ( PacketReceivePacket(s_adapter_ptr, &packet, TRUE ) )
            {
                ParseReceivedData( &packet );
            }
        }
    }

ETHRCVTASK_DEVOPEN_END:
	// close network adapter
	CloseEthernetAdapter();
    
ETHRCVTASK_NODEVOPEN_END:
    TRACE("[ ETHERNET TASK ] ethernet receive task is to closed!\n");
    ::CloseHandle( s_ether_task_event ); // delete ethernet task close event 
    s_ether_task_event = NULL;
    s_is_ether_task_run = FALSE;         // reset task running flag
    return 0;
}

/*****************************************************************************/
// Description: check if test time is out
// Global resource :
// Note :  
/*****************************************************************************/
static BOOL IsTestTimeOut( void )
{
    DWORD cur_ticks = GetTickCount();
    DWORD last_ticks = ( cur_ticks >= s_test_start_time)?
        (cur_ticks - s_test_start_time) : (~(DWORD)(0L) - s_test_start_time + cur_ticks);
    
    if ( last_ticks > s_test_holding_time )
    {
        ASSERT( NULL!=s_window_handle );
        NETTEST_TYPE_E cur_nettest_type = s_nettest_type;
        
        TRACE("testing notes(%ld): %s timeout, start(%ld), end(%ld), last(%d)\n", 
            GetTickCount(), s_nettest_type_notes[cur_nettest_type], s_test_start_time, cur_ticks, last_ticks);
        s_nettest_type = TYPE_IDLE;     // 重置测试类型
        ::PostMessage( s_window_handle, WM_ETHERNET_RECV_TIMEOUT, (WPARAM)(cur_nettest_type), 0 );      
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
// Description: parse received data
// Global resource : none
/*****************************************************************************/
static BOOL ParseReceivedData( LPPACKET packet_ptr )
{
    unsigned long received_bytes = 0, offset = 0;
    unsigned char *buff_ptr = NULL, *data_ptr = NULL;
    unsigned int  cap_len = 0, data_len = 0;
    struct bpf_hdr *pkt_hdr_ptr = NULL;
    
    /* get bytes length of received data */
    received_bytes = packet_ptr->ulBytesReceived;

    /* get memory address of received data */
    buff_ptr = (unsigned char*)packet_ptr->Buffer;
    
    /* value from data buffer start address to current be parsed address */
    offset = 0;
    
    /* read by packet until the end of the buffer */
    while ( offset < received_bytes )
    {	
        /*********************************************************************************
        MEMORY of packet_ptr
        ----------------------------------------------------------------------------------
        struct bpf_hdr | packet data [ALIGN] | struct bpf_hdr | packet data [ALIGN] | ...  
        ----------------------------------------------------------------------------------
        <- bh_hdrlen ->|<----- cap_len ----->|<- bh_hdrlen -->|<----- cap_len ----->| ...
        <------------ PACKET 1 ------------->|<------------ PACKET 2 -------------->| ...
        <- start address: buff_ptr
        <---------------------------- received_bytes ------------------------------------>
        *********************************************************************************/
        pkt_hdr_ptr = (struct bpf_hdr*)( buff_ptr + offset );   // get packet info header
        data_len = pkt_hdr_ptr->bh_datalen;     // get packet full length
        cap_len = pkt_hdr_ptr->bh_caplen;       // get packet captured length
        offset += pkt_hdr_ptr->bh_hdrlen;       // calculate packet data memory address offset
        data_ptr = (unsigned char*)( buff_ptr + offset );

        /* Analyse received data packet */
        AnalyseRecvedPacket( data_ptr, cap_len );

        offset = Packet_WORDALIGN( offset + cap_len );  // find next packet memory offset
    }
    
    return TRUE;
}

/*****************************************************************************/
// Description: analyse received packet
// Global resource : none
// Author: yifeng.wang  
/*****************************************************************************/
static void AnalyseRecvedPacket( u_char *pkt_ptr, unsigned int len )
{
    BOOL is_matched = FALSE;

    // only when nettest is on, analyse received packet
    if( TYPE_IDLE == s_nettest_type )
    {
        //TRACE("AnalyseRecvedPacket - test type %d, no packet analyse!\n", s_nettest_type);
        return;
    }

    // check if it's an ARP packet
    if( ( len >= 42 ) &&    /* 42 - minimum ARP packet length */
        ( 0x08 == pkt_ptr[12] && 0x06 == pkt_ptr[13] )
      )
    {
        // deal with ARP reply
        if( ( 0x02 == pkt_ptr[21] ) &&      // ARP reply
            ( 0 == memcmp( &pkt_ptr[0],  s_mac_src, 6 ) ) &&    // dest MAC
            ( 0 == memcmp( &pkt_ptr[32], s_mac_src, 6 ) ) &&    // target MAC
            ( 0 == memcmp( &pkt_ptr[38], &s_ip_src, 4 ) ) &&    // target IP
            ( 0 == memcmp( &pkt_ptr[28], &s_ip_dst, 4 ) ) )     // sender IP 
        {
            is_matched = TRUE;
        }

        // deal with ARP reply on required MAC address
        if( ( 0x02 == pkt_ptr[21] ) )       // ARP reply
        {
//            TRACE("yifeng debug: get ARP reply on ip %u.%u.%u.%u, s_ip_peer %u.%u.%u.%u\n",
//                pkt_ptr[28], pkt_ptr[29], pkt_ptr[30], pkt_ptr[31], PUSH_IPADDR(s_ip_peer));
#if 1
            if( pkt_ptr[28] == (BYTE)(s_ip_peer>>24) &&
                pkt_ptr[29] == (BYTE)(s_ip_peer>>16) &&
                pkt_ptr[30] == (BYTE)(s_ip_peer>>8)  &&
                pkt_ptr[31] == (BYTE)(s_ip_peer) )
#else
            // ? this is always FALSE
            if( 0 == memcmp( &pkt_ptr[28], &s_ip_peer, 4 ) )     // sender IP 
#endif
            {
                // record peer MAC address
                //TRACE("yifeng debug: get ARP reply on matched peer MAC\n");
                memcpy( s_mac_peer, &pkt_ptr[22], 6 );
            }
        }

        // deal with matched ARP request and reply
        if( ( 0x01 == pkt_ptr[21] ) &&      // ARP request
            ( 0 == memcmp( &pkt_ptr[38], &s_ip_src, 4 ) ) )     // target IP
        {
            // create ARP packet
            unsigned char arprep_pkt[] =    /* raw ARP reply packet */
            { 
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00, 0x08, 0x06, 0x00, 0x01, 
                0x08, 0x00, 0x06, 0x04, 0x00, 0x02, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00
            };

            // set ARP reply
            memcpy( &arprep_pkt[0], &pkt_ptr[22], 6);
            memcpy( &arprep_pkt[6], &pkt_ptr[32], 6);
            memcpy( &arprep_pkt[22], s_mac_src, 6);
            memcpy( &arprep_pkt[28], &s_ip_src, 4);
            memcpy( &arprep_pkt[32], &pkt_ptr[22], 6);
            memcpy( &arprep_pkt[38], &pkt_ptr[28], 4);
            
            // send ARP reply
            int ret_val = SendEthernetPacket( arprep_pkt, sizeof(arprep_pkt) );
            if( ret_val != ETHERR_OK )
            {
                TRACE("AnalyseRecvedPacket: send ARP reply - %s", NETTEST_GetErrorMsg(ret_val));
            }            
        }
    }

    // check if it's PING echo reply
    if( ( len >= 42 ) &&    /* 42 - minimum ARP packet length */
        ( 0x08 == pkt_ptr[12] && 0x00 == pkt_ptr[13] &&    /* 0x0800 - IP MAC type */
          0x45 == pkt_ptr[14] && 0x00 == pkt_ptr[15] &&    /* 0x4500 - ICMP type */
          0x01 == pkt_ptr[23] &&                           /* ICMP type */
          0x00 == pkt_ptr[34] && 0x00 == pkt_ptr[35] )     /* ICMP PING echo reply */
      )
    {
        // check protocol data
        if( ( 0 == memcmp( &pkt_ptr[38], &s_ping_id, 2 ) ) &&   // PING identifier
            ( 0 == memcmp( &pkt_ptr[40], &s_ping_seq, 2 ) ) )   // PING sequence 
        {
            is_matched = TRUE;
        }
    }

    // send message
    if( is_matched )
    {
        NETTEST_TYPE_E cur_nettest_type = s_nettest_type;
        TRACE("testing notes(%ld): reply for %s\n", GetTickCount(), s_nettest_type_notes[cur_nettest_type]);
        s_nettest_type = TYPE_IDLE;     // 重置测试类型
        if( NULL != s_window_handle )
        {
            ::PostMessage( s_window_handle, WM_ETHERNET_RECV_PACKET, (WPARAM)(cur_nettest_type), 0 );
        }
    }
}

/*****************************************************************************/
// Description: calculate check sum
// Global resource : none
// Author: yifeng.wang  
/*****************************************************************************/
static unsigned short CheckSum( void* ptr, unsigned count )
{
    unsigned short  checksum;
    unsigned short* addr_ptr = (unsigned short*)ptr;

    /********** code from rfc1071 **********/
    {
        /* Compute Internet Checksum for "count" bytes begin at location "addr". */
        long sum = 0;
        count += count;     /* make passed word count into byte count */
        
        while( count > 1 )
        {
            /* This is the inner loop */
            sum += *addr_ptr++;
            count -= 2;
        }
        
        /* Add left-over byte, if any */
        if( count > 0 )
            sum += * (unsigned char*)addr_ptr;
        
        /* Fold 32-bit sum to 16 bits */
        while( sum>>16 )
        {
            sum = (sum & 0xffff) + (sum >> 16);
        }

        /* checksum = ~sum; */
        /* removed for MIT IP stack */
        checksum = (unsigned short)sum;
    }

    /******** end of RFC 1071 code **********/
    return checksum;
}
