/*****************************************************************************
** File Name:      mmisrvwifi.c                                              *
** Author:         bin.wang1                                                 *
** Date:           2020.07.24                                                *
** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2020.07.24     bin.wang1            Create                                *
******************************************************************************/

#include "base_trc.h"
#include "sci_api.h"
#include "sig_code.h"
#include "in_message.h"
#include "priority_system.h"

#include "mmisrvwifi.h"

/**-------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMISRV_WIFI_TASK_STACK_SIZE      (10*1024)
#define MMISRV_WIFI_TASK_PRI             PRI_WIFI_SERVICE
#define MMISRV_WIFI_TASK_QUEUE_NUM       0x80


#define MMISRVWIFI_MUTEX_LOCK     do{                                                                                   \
                                        if( PNULL == s_mmisrv_wifi_mutex_ptr )                                          \
                                        {                                                                               \
                                            s_mmisrv_wifi_mutex_ptr = SCI_CreateMutex("mmisrv_wifi_mutex", SCI_INHERIT);\
                                        }                                                                               \
                                        if( PNULL != s_mmisrv_wifi_mutex_ptr )                                          \
                                        {                                                                               \
                                            if( 0 != SCI_GetMutex(s_mmisrv_wifi_mutex_ptr,SCI_WAIT_FOREVER) )           \
                                            {                                                                           \
                                                TRACE_SRV_WIFI("SCI_GetMutex fail!!");                                  \
                                            }                                                                           \
                                        }                                                                               \
                                        else                                                                            \
                                        {                                                                               \
                                            TRACE_SRV_WIFI("SCI_CreateMutex fail!!");                                   \
                                        }                                                                               \
                                    }while(0)


#define MMISRVWIFI_MUTEX_UNLOCK   do{                                                                                   \
                                        if( PNULL != s_mmisrv_wifi_mutex_ptr )                                          \
                                        {                                                                               \
                                            if( 0 != SCI_PutMutex(s_mmisrv_wifi_mutex_ptr) )                            \
                                            {                                                                           \
                                                SCI_TRACE_LOW("SCI_GetMutex fail!!");                                   \
                                            }                                                                           \
                                        }                                                                               \
                                    }while(0)

#ifdef WIN32
#define TRACE_SRV_WIFI                  SCI_TRACE_LOW
#else
#define TRACE_SRV_WIFI(_format,...)     do                                                                                          \
                                        {                                                                                           \
                                            SCI_TraceLow("{%s}[%s]<%d> "_format, "WIFISRV", __FUNCTION__, __LINE__, ##__VA_ARGS__); \
                                        }while(0)
#endif
/**-------------------------------------------------------------------------*
**                         TYPE DEFINITION                                  *
**--------------------------------------------------------------------------*/
typedef enum MMISRV_WIFI_REQ
{
    MMISRV_WIFI_ON_REQ,
    MMISRV_WIFI_BACKGROUND_ON_REQ,
    MMISRV_WIFI_OFF_REQ,
    MMISRV_WIFI_BACKGROUND_OFF_REQ,
    MMISRV_WIFI_SCAN_REQ,
    MMISRV_WIFI_BACKGROUND_SCAN_REQ,
    MMISRV_WIFI_CONNECT_REQ,
    MMISRV_WIFI_DISCONNECT_REQ,
    MMISRV_WIFI_REQ_MAX
}MMISRV_WIFI_REQ_E;

typedef enum MMISRV_WIFI_STATUS
{
    MMISRV_WIFI_CLOSING,
    MMISRV_WIFI_CLOSED,
    MMISRV_WIFI_OPENING,
    MMISRV_WIFI_OPENED,
    MMISRV_WIFI_SCANING,
    MMISRV_WIFI_CONNECTING,
    MMISRV_WIFI_CONNECTED,
    MMISRV_WIFI_DISCONNECTING
}MMISRV_WIFI_STATUS_E;

typedef struct MMISRV_WIFI_AP_NODE
{
    WIFISUPP_SCAN_AP_INFO_T             ap_info;
    struct MMISRV_WIFI_AP_NODE*         next_ptr;
}MMISRV_WIFI_AP_NODE_T;

/**-------------------------------------------------------------------------*
**                         STATIC VARIABLE DEFINITION                       *
**--------------------------------------------------------------------------*/

LOCAL MMISRV_WIFI_STATUS_E      s_mmisrv_wifi_status = MMISRV_WIFI_CLOSED;  //driver status
LOCAL SCI_MUTEX_PTR             s_mmisrv_wifi_mutex_ptr = PNULL;
LOCAL BLOCK_ID                  s_mmisrv_wifi_thread_id = SCI_INVALID_BLOCK_ID;
LOCAL uint32                    s_mmisrv_wifi_bg_on_req_count = 0;
LOCAL BOOLEAN                   s_mmisrv_wifi_is_ui_on = FALSE;
LOCAL BOOLEAN                   s_mmisrv_wifi_is_connected = FALSE;
LOCAL BOOLEAN                   s_mmisrv_wifi_req_called[MMISRV_WIFI_REQ_MAX] = { 0 };
LOCAL MMISRV_WIFI_AP_NODE_T*    s_mmisrv_wifi_ap_list_ptr = PNULL;

/**-------------------------------------------------------------------------*
**                              FUNCTION                                    *
**--------------------------------------------------------------------------*/

LOCAL BLOCK_ID wifiGetThreadId( void )
{
    return s_mmisrv_wifi_thread_id;
}

void MMISRVWIFI_CallBackFunc(
                                         uint32 task_id,    // task_id which task want to receive the event
                                         uint32 event_id,   // event id
                                         void   *param_ptr  // data follow the event
                                         )
{

    uint16              sig_size           = 0;
    uint16              sig_code           = 0;
    xSignalHeaderRec*   dispatch_sig_ptr  = PNULL;

    SCI_ASSERT( PNULL != param_ptr );

    switch ( event_id )
    {
        case SRV_WIFI_SERVICE_ON_CNF:
        {
            sig_code = SRV_WIFI_SIG_ON_CNF;
            sig_size = sizeof( WIFISUPP_ON_CNF_T );
            break;
        }

        case SRV_WIFI_SERVICE_BACKGROUND_ON_CNF:
        {
            sig_code = SRV_WIFI_SIG_BACKGROUND_ON_CNF;
            sig_size = sizeof( WIFISUPP_ON_CNF_T );
            break;
        }

        case SRV_WIFI_SERVICE_OFF_CNF:
        {
            sig_code = SRV_WIFI_SIG_OFF_CNF;
            sig_size = sizeof( WIFISUPP_OFF_CNF_T );
            break;
        }

        case SRV_WIFI_SERVICE_BACKGROUND_OFF_CNF:
        {
            sig_code = SRV_WIFI_SIG_BACKGROUND_OFF_CNF;
            sig_size = sizeof( WIFISUPP_OFF_CNF_T );
            break;
        }

        case SRV_WIFI_SERVICE_SCAN_IND:
        {
            sig_code = SRV_WIFI_SIG_SCAN_IND;
            sig_size = sizeof( WIFISUPP_SCAN_IND_T );
            break;
        }

        case SRV_WIFI_SERVICE_BACKGROUND_SCAN_IND:
        {
            sig_code = SRV_WIFI_SIG_BACKGROUND_SCAN_IND;
            sig_size = sizeof( WIFISUPP_SCAN_IND_T );
            break;
        }

        case SRV_WIFI_SERVICE_SCAN_CNF:
        {
            sig_code = SRV_WIFI_SIG_SCAN_CNF;
            sig_size = sizeof( WIFISUPP_SCAN_CNF_T );
            break;
        }

        case SRV_WIFI_SERVICE_BACKGROUND_SCAN_CNF:
        {
            sig_code = SRV_WIFI_SIG_BACKGROUND_SCAN_CNF;
            sig_size = sizeof( WIFISUPP_SCAN_CNF_T );
            break;
        }

        case SRV_WIFI_SERVICE_CONNECT_CNF:
        {
            sig_code = SRV_WIFI_SIG_CONNECT_CNF;
            sig_size = sizeof( WIFISUPP_CONNECT_CNF_T );
            break;
        }

        case SRV_WIFI_SERVICE_DISCONNECT_IND:
        {
            sig_code = SRV_WIFI_SIG_DISCONNECT_IND;
            sig_size = sizeof( WIFISUPP_DISCONNECT_IND_T );
            break;
        }

        case SRV_WIFI_SERVICE_DISCONNECT_CNF:
        {
            sig_code = SRV_WIFI_SIG_DISCONNECT_CNF;
            sig_size = sizeof( WIFISUPP_DISCONNECT_CNF_T );
            break;
        }

        case SRV_WIFI_SERVICE_CALC_IND:
        {
            sig_code = SRV_WIFI_SIG_CALC_IND;
            sig_size = sizeof( WIFISUPP_CALC_IND_T );
            break;
        }

        default:
        {
            TRACE_SRV_WIFI("Error service type ");
            SCI_ASSERT(FALSE);
            break;
        }

    }

    TRACE_SRV_WIFI("send task 0x%x sig_code %d", task_id, (sig_code-SRV_WIFI_SIG_START) );

    SCI_CREATE_SIGNAL( dispatch_sig_ptr, sig_code, sig_size, wifiGetThreadId() );

    SCI_MEMCPY(
                (void*)((uint8*)dispatch_sig_ptr+sizeof(xSignalHeaderRec)),
                (void*)((uint8*)param_ptr+sizeof(xSignalHeaderRec)),
                sig_size-sizeof(xSignalHeaderRec)
               );

    SCI_SEND_SIGNAL( (xSignalHeaderRec*)dispatch_sig_ptr, task_id ) ;

}

LOCAL void WifiAddOneAp( WIFISUPP_SCAN_AP_INFO_T* wifi_scan_ap_ptr )
{
    MMISRV_WIFI_AP_NODE_T* pLastNode = PNULL;
    MMISRV_WIFI_AP_NODE_T* pApNode = PNULL;

    if( PNULL == wifi_scan_ap_ptr )
    {
        TRACE_SRV_WIFI("wifi_scan_ap_ptr is PNULL!!");
        return;
    }

    TRACE_SRV_WIFI("ssid        %s", wifi_scan_ap_ptr->ssid.ssid);
    TRACE_SRV_WIFI("ssid_len    %d", wifi_scan_ap_ptr->ssid.ssid_len);

    pApNode = (MMISRV_WIFI_AP_NODE_T*)SCI_ALLOC_APPZ( sizeof(MMISRV_WIFI_AP_NODE_T) );
    if( PNULL == pApNode )
    {
        TRACE_SRV_WIFI("MALLOC fail!!");
        return;
    }

    SCI_MEMCPY( &(pApNode->ap_info), wifi_scan_ap_ptr, sizeof(WIFISUPP_SCAN_AP_INFO_T) );
    pApNode->next_ptr = PNULL;

    if( PNULL == s_mmisrv_wifi_ap_list_ptr )
    {
        s_mmisrv_wifi_ap_list_ptr = pApNode;
    }
    else
    {
        pLastNode = s_mmisrv_wifi_ap_list_ptr;
        while( pLastNode->next_ptr != PNULL )
        {
            pLastNode = pLastNode->next_ptr;
        }
        pLastNode->next_ptr = pApNode;
    }

}

LOCAL void WifiDeleteAllAp( void )
{
    MMISRV_WIFI_AP_NODE_T* pFreeNode = PNULL;

    if( PNULL == s_mmisrv_wifi_ap_list_ptr )
    {
        TRACE_SRV_WIFI("s_mmisrv_wifi_ap_list_ptr is PNULL!!");
        return;
    }

    while( PNULL != s_mmisrv_wifi_ap_list_ptr )
    {
        pFreeNode = s_mmisrv_wifi_ap_list_ptr;
        s_mmisrv_wifi_ap_list_ptr = pFreeNode->next_ptr;

        SCI_FREE( pFreeNode );
    }
}

LOCAL void WifiSendAllAp( BOOLEAN is_background )
{
    WIFISUPP_SCAN_IND_T     scanInd = { 0 };
    MMISRV_WIFI_AP_NODE_T*  pNode = PNULL;

    if( PNULL == s_mmisrv_wifi_ap_list_ptr )
    {
        TRACE_SRV_WIFI("s_mmisrv_wifi_ap_list_ptr is PNULL!!");
        return;
    }

    pNode = s_mmisrv_wifi_ap_list_ptr;

    while( PNULL != pNode )
    {
        SCI_MEMCPY( &(scanInd.ap_info), &(pNode->ap_info), sizeof(WIFISUPP_SCAN_AP_INFO_T) );
        if( TRUE == is_background )
        {
            SCI_SendEventToClient( WIFI_SERVICE, SRV_WIFI_SERVICE_BACKGROUND_SCAN_IND, &scanInd );
        }
        else
        {
            SCI_SendEventToClient( WIFI_SERVICE, SRV_WIFI_SERVICE_SCAN_IND, &scanInd );
        }
        pNode = pNode->next_ptr;

        SCI_SLEEP( 10 );
    }
}

LOCAL void WifiHandleOnCnf( WIFISUPP_ON_CNF_T* wifi_on_cnf_ptr )
{
    BOOLEAN isPostMsg = FALSE;
    BOOLEAN isPostBackgroundMsg = FALSE;

    TRACE_SRV_WIFI("on_result.wifi_on_result %d", (int32)(wifi_on_cnf_ptr->on_result.wifi_on_result) );

    MMISRVWIFI_MUTEX_LOCK;

    TRACE_SRV_WIFI("s_mmisrv_wifi_status %d", s_mmisrv_wifi_status);

    if( WIFISUPP_RESULT_SUCC == wifi_on_cnf_ptr->on_result.wifi_on_result )
    {
        if( MMISRV_WIFI_OPENING == s_mmisrv_wifi_status )
        {
            s_mmisrv_wifi_status = MMISRV_WIFI_OPENED;
            TRACE_SRV_WIFI("s_mmisrv_wifi_status change to %d", s_mmisrv_wifi_status);
        }

        if( TRUE == s_mmisrv_wifi_req_called[MMISRV_WIFI_ON_REQ] )
        {
            isPostMsg = TRUE;
            s_mmisrv_wifi_req_called[MMISRV_WIFI_ON_REQ] = FALSE;
        }

        if( TRUE == s_mmisrv_wifi_req_called[MMISRV_WIFI_BACKGROUND_ON_REQ] )
        {
            isPostBackgroundMsg = TRUE;
            s_mmisrv_wifi_req_called[MMISRV_WIFI_BACKGROUND_ON_REQ] = FALSE;
        }
    }
    else
    {
        s_mmisrv_wifi_is_ui_on = FALSE;
        s_mmisrv_wifi_bg_on_req_count = 0;
    }

    MMISRVWIFI_MUTEX_UNLOCK;

    if( TRUE == isPostMsg )
    {
        SCI_SendEventToClient( WIFI_SERVICE, SRV_WIFI_SERVICE_ON_CNF, wifi_on_cnf_ptr );
    }

    if( TRUE == isPostBackgroundMsg )
    {
        SCI_SendEventToClient( WIFI_SERVICE, SRV_WIFI_SERVICE_BACKGROUND_ON_CNF, wifi_on_cnf_ptr );
    }
}

LOCAL void WifiHandleOffCnf( WIFISUPP_OFF_CNF_T* wifi_off_cnf_ptr )
{
    BOOLEAN isPostMsg = FALSE;
    BOOLEAN isPostBackgroundMsg = FALSE;

    TRACE_SRV_WIFI("off_result.wifi_off_result %d", (int32)(wifi_off_cnf_ptr->off_result.wifi_off_result) );

    MMISRVWIFI_MUTEX_LOCK;

    if( MMISRV_WIFI_CLOSING == s_mmisrv_wifi_status )
    {
        s_mmisrv_wifi_status = MMISRV_WIFI_CLOSED;
        TRACE_SRV_WIFI("s_mmisrv_wifi_status change to %d", s_mmisrv_wifi_status);
    }
    else
    {
        TRACE_SRV_WIFI("some one call on req s_mmisrv_wifi_status %d", s_mmisrv_wifi_status);
    }

    if( TRUE == s_mmisrv_wifi_req_called[MMISRV_WIFI_OFF_REQ] )
    {
        isPostMsg = TRUE;
        s_mmisrv_wifi_req_called[MMISRV_WIFI_OFF_REQ] = FALSE;
    }

    if( TRUE == s_mmisrv_wifi_req_called[MMISRV_WIFI_BACKGROUND_OFF_REQ] )
    {
        isPostBackgroundMsg = TRUE;
        s_mmisrv_wifi_req_called[MMISRV_WIFI_BACKGROUND_OFF_REQ] = FALSE;
    }

    MMISRVWIFI_MUTEX_UNLOCK;

    if( TRUE == isPostMsg )
    {
        SCI_SendEventToClient( WIFI_SERVICE, SRV_WIFI_SERVICE_OFF_CNF, wifi_off_cnf_ptr );
    }

    if( TRUE == isPostBackgroundMsg )
    {
        SCI_SendEventToClient( WIFI_SERVICE, SRV_WIFI_SERVICE_BACKGROUND_OFF_CNF, wifi_off_cnf_ptr );
    }
}

LOCAL void WifiHandleScanInd( WIFISUPP_SCAN_IND_T* wifi_scan_ind_ptr )
{
    BOOLEAN isPostMsg = FALSE;
    BOOLEAN isPostBackgroundMsg = FALSE;

    WifiAddOneAp( &(wifi_scan_ind_ptr->ap_info) );
#if 0
    MMISRVWIFI_MUTEX_LOCK;

    if( MMISRV_WIFI_SCANING != s_mmisrv_wifi_status )
    {
        TRACE_SRV_WIFI("some one call other req when scaning s_mmisrv_wifi_status %d", s_mmisrv_wifi_status);
    }

    if( TRUE == s_mmisrv_wifi_req_called[MMISRV_WIFI_SCAN_REQ] )
    {
        isPostMsg = TRUE;
    }

    if( TRUE == s_mmisrv_wifi_req_called[MMISRV_WIFI_BACKGROUND_SCAN_REQ] )
    {
        isPostBackgroundMsg = TRUE;
    }

    MMISRVWIFI_MUTEX_UNLOCK;

    if( TRUE == isPostMsg )
    {
        SCI_SendEventToClient( WIFI_SERVICE, MMISRV_WIFI_SERVICE_SCAN_IND, wifi_scan_ind_ptr );
    }

    if( TRUE == isPostBackgroundMsg )
    {
        SCI_SendEventToClient( WIFI_SERVICE, MMISRV_WIFI_SERVICE_BACKGROUND_SCAN_IND, wifi_scan_ind_ptr );
    }
#endif
}

LOCAL void WifiHandleScanCnf( WIFISUPP_SCAN_CNF_T* wifi_scan_cnf_ptr )
{
    BOOLEAN                 isPostMsg = FALSE;
    BOOLEAN                 isPostBackgroundMsg = FALSE;

    TRACE_SRV_WIFI("scan_result %d", (int32)(wifi_scan_cnf_ptr->scan_result) );

    MMISRVWIFI_MUTEX_LOCK;

    if( MMISRV_WIFI_SCANING == s_mmisrv_wifi_status )
    {
        if( TRUE == s_mmisrv_wifi_is_connected )
        {
            s_mmisrv_wifi_status = MMISRV_WIFI_CONNECTED;
        }
        else
        {
            s_mmisrv_wifi_status = MMISRV_WIFI_OPENED;
        }
        TRACE_SRV_WIFI("s_mmisrv_wifi_status change to %d", s_mmisrv_wifi_status);
    }
    else
    {
        TRACE_SRV_WIFI("some one call other req when scaning s_mmisrv_wifi_status %d", s_mmisrv_wifi_status);
    }

    if( TRUE == s_mmisrv_wifi_req_called[MMISRV_WIFI_SCAN_REQ] )
    {
        isPostMsg = TRUE;
        s_mmisrv_wifi_req_called[MMISRV_WIFI_SCAN_REQ] = FALSE;
    }

    if( TRUE == s_mmisrv_wifi_req_called[MMISRV_WIFI_BACKGROUND_SCAN_REQ] )
    {
        isPostBackgroundMsg = TRUE;
        s_mmisrv_wifi_req_called[MMISRV_WIFI_BACKGROUND_SCAN_REQ] = FALSE;
    }

    MMISRVWIFI_MUTEX_UNLOCK;

    if( TRUE == isPostMsg )
    {
        WifiSendAllAp( FALSE );
        SCI_SendEventToClient( WIFI_SERVICE, SRV_WIFI_SERVICE_SCAN_CNF, wifi_scan_cnf_ptr );
    }

    if( TRUE == isPostBackgroundMsg )
    {
        WifiSendAllAp( TRUE );
        SCI_SendEventToClient( WIFI_SERVICE, SRV_WIFI_SERVICE_BACKGROUND_SCAN_CNF, wifi_scan_cnf_ptr );
    }

    WifiDeleteAllAp();
}

LOCAL void WifiHandleConnectCnf( WIFISUPP_CONNECT_CNF_T* wifi_connect_cnf_ptr )
{
    TRACE_SRV_WIFI("connect_result.result %d", (int32)(wifi_connect_cnf_ptr->connect_result.result) );

    MMISRVWIFI_MUTEX_LOCK;

    TRACE_SRV_WIFI("s_mmisrv_wifi_status %d", s_mmisrv_wifi_status);

    if( WIFISUPP_RESULT_SUCC == wifi_connect_cnf_ptr->connect_result.result )
    {
        s_mmisrv_wifi_is_connected = TRUE;
        if( MMISRV_WIFI_CONNECTING == s_mmisrv_wifi_status )
        {
            s_mmisrv_wifi_status = MMISRV_WIFI_CONNECTED;
            TRACE_SRV_WIFI("s_mmisrv_wifi_status change to %d", s_mmisrv_wifi_status);
        }
    }
    else
    {
        if( MMISRV_WIFI_CONNECTING == s_mmisrv_wifi_status )
        {
            s_mmisrv_wifi_status = MMISRV_WIFI_OPENED;
            TRACE_SRV_WIFI("s_mmisrv_wifi_status change to %d", s_mmisrv_wifi_status);
        }
    }

    s_mmisrv_wifi_req_called[MMISRV_WIFI_CONNECT_REQ] = FALSE;

    MMISRVWIFI_MUTEX_UNLOCK;

    SCI_SendEventToClient( WIFI_SERVICE, SRV_WIFI_SERVICE_CONNECT_CNF, wifi_connect_cnf_ptr );
}

LOCAL void WifiHandleDisconnectInd( WIFISUPP_DISCONNECT_IND_T* wifi_disconnect_ind_ptr )
{
    TRACE_SRV_WIFI("ssid        %s", wifi_disconnect_ind_ptr->disc_info.ssid.ssid);
    TRACE_SRV_WIFI("ssid_len    %d", (int32)(wifi_disconnect_ind_ptr->disc_info.ssid.ssid_len) );

    MMISRVWIFI_MUTEX_LOCK;

    s_mmisrv_wifi_is_connected = FALSE;
    if( MMISRV_WIFI_CONNECTED == s_mmisrv_wifi_status )
    {
        s_mmisrv_wifi_status = MMISRV_WIFI_OPENED;
        TRACE_SRV_WIFI("s_mmisrv_wifi_status change to %d", s_mmisrv_wifi_status);
    }
    else
    {
        TRACE_SRV_WIFI("some one call other req when connected s_mmisrv_wifi_status %d", s_mmisrv_wifi_status);
    }

    MMISRVWIFI_MUTEX_UNLOCK;

    SCI_SendEventToClient( WIFI_SERVICE, SRV_WIFI_SERVICE_DISCONNECT_IND, wifi_disconnect_ind_ptr );
}

LOCAL void WifiHandleDisconnectCnf( WIFISUPP_DISCONNECT_CNF_T* wifi_disconnect_cnf_ptr )
{
    TRACE_SRV_WIFI("ssid        %s", wifi_disconnect_cnf_ptr->disconnect_result.ssid.ssid);
    TRACE_SRV_WIFI("ssid_len    %d", (int32)(wifi_disconnect_cnf_ptr->disconnect_result.ssid.ssid_len) );
    TRACE_SRV_WIFI("scan_result %d", (int32)(wifi_disconnect_cnf_ptr->disconnect_result.result) );

    MMISRVWIFI_MUTEX_LOCK;

    s_mmisrv_wifi_is_connected = FALSE;
    if( MMISRV_WIFI_DISCONNECTING == s_mmisrv_wifi_status )
    {
        s_mmisrv_wifi_status = MMISRV_WIFI_OPENED;
        TRACE_SRV_WIFI("s_mmisrv_wifi_status change to %d", s_mmisrv_wifi_status);
    }
    else
    {
        TRACE_SRV_WIFI("some one call other req when disconnecting s_mmisrv_wifi_status %d", s_mmisrv_wifi_status);
    }

    s_mmisrv_wifi_req_called[MMISRV_WIFI_DISCONNECT_REQ] = FALSE;

    MMISRVWIFI_MUTEX_UNLOCK;

    SCI_SendEventToClient( WIFI_SERVICE, SRV_WIFI_SERVICE_DISCONNECT_CNF, wifi_disconnect_cnf_ptr );
}

LOCAL void WifiHandleCalcInd( WIFISUPP_CALC_IND_T* wifi_calc_ind_ptr )
{
    if( PNULL == wifi_calc_ind_ptr )
    {
        TRACE_SRV_WIFI("wifi_calc_ind_ptr id PNULL");
        return;
    }

    TRACE_SRV_WIFI("wifi_calc_result %d", wifi_calc_ind_ptr->calc_result.wifi_calc_result);

    SCI_SendEventToClient( WIFI_SERVICE, SRV_WIFI_SERVICE_CALC_IND, wifi_calc_ind_ptr );
}

void MMISRVWIFI_TaskEntry( uint32 argc, void *argv )
{
    xSignalHeaderRec*       sig_ptr = PNULL;
    BLOCK_ID                task_id = SCI_IdentifyThread();

    //create server
    SCI_CreateClientList( WIFI_SERVICE, (SRV_WIFI_SERVICE_END-SRV_WIFI_SERVICE_START+1), MMISRVWIFI_CallBackFunc );

    while( TRUE )
    {

        SCI_RECEIVE_SIGNAL( sig_ptr, task_id );
        SCI_ASSERT( sig_ptr!=PNULL );

        switch( sig_ptr->SignalCode )
        {

            case WIFISUPP_SIG_ON_CNF:
            {
                WIFISUPP_ON_CNF_T* pOnCnf = (WIFISUPP_ON_CNF_T*)sig_ptr;

                WifiHandleOnCnf( pOnCnf );

                break;
            }

            case WIFISUPP_SIG_OFF_CNF:
            {
                WIFISUPP_OFF_CNF_T* pOffCnf = (WIFISUPP_OFF_CNF_T*)sig_ptr;

                WifiHandleOffCnf( pOffCnf );

                break;
            }

            case WIFISUPP_SIG_SCAN_IND:
            {
                WIFISUPP_SCAN_IND_T* pScanInd = (WIFISUPP_SCAN_IND_T*)sig_ptr;

                WifiHandleScanInd( pScanInd );

                break;
            }

            case WIFISUPP_SIG_SCAN_CNF:
            {
                WIFISUPP_SCAN_CNF_T* pScanCnf = (WIFISUPP_SCAN_CNF_T*)sig_ptr;

                WifiHandleScanCnf( pScanCnf );

                break;
            }

            case WIFISUPP_SIG_CONNECT_CNF:
            {
                WIFISUPP_CONNECT_CNF_T* pConnectCnf = (WIFISUPP_CONNECT_CNF_T*)sig_ptr;

                WifiHandleConnectCnf( pConnectCnf );

                break;
            }

            case WIFISUPP_SIG_DISCONNECT_IND:
            {
                WIFISUPP_DISCONNECT_IND_T* pDisconnectInd = (WIFISUPP_DISCONNECT_IND_T*)sig_ptr;

                WifiHandleDisconnectInd( pDisconnectInd );

                break;
            }

            case WIFISUPP_SIG_DISCONNECT_CNF:
            {
                WIFISUPP_DISCONNECT_CNF_T* pDisconnectCnf = (WIFISUPP_DISCONNECT_CNF_T*)sig_ptr;

                WifiHandleDisconnectCnf( pDisconnectCnf );

                break;
            }

            case WIFISUPP_SIG_CALC_IND:
            {
                WIFISUPP_CALC_IND_T* pCalcInd = (WIFISUPP_CALC_IND_T*)sig_ptr;

                WifiHandleCalcInd( pCalcInd );

                break;
            }

            default:
            {
                TRACE_SRV_WIFI("invalid cnf 0x%x WIFISUPP_SIG_ON_CNF is 0x%x", sig_ptr->SignalCode, WIFISUPP_SIG_ON_CNF);
                break;
            }

        }

        SCI_FREE_SIGNAL( sig_ptr );
    }
}

/*****************************************************************************/
//  Description: MMISRVWIFI_TaskInit
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
PUBLIC void MMISRVWIFI_TaskInit( void )
{
    if( s_mmisrv_wifi_thread_id == SCI_INVALID_BLOCK_ID )
    {
        s_mmisrv_wifi_thread_id = SCI_CreateThread("T_MMISRV_WIFI", "Q_MMISRV_WIFI", MMISRVWIFI_TaskEntry, 0, NULL, MMISRV_WIFI_TASK_STACK_SIZE,
                                        MMISRV_WIFI_TASK_QUEUE_NUM, MMISRV_WIFI_TASK_PRI, SCI_PREEMPT, SCI_AUTO_START);

        SCI_ASSERT( SCI_INVALID_BLOCK_ID != s_mmisrv_wifi_thread_id );
    }
}

/****************************************************************************/
//  Description : MMISRVWIFI_Init
//  Global resource dependence :
//  Author: bin.wang1
//  Note:
/****************************************************************************/
PUBLIC void MMISRVWIFI_Init( void )
{
    uint16 index = 0;

    WIFISUPP_Init();

    for( index = 0; index < MMISRV_WIFI_REQ_MAX; index++ )
    {
        s_mmisrv_wifi_req_called[index] = FALSE;
    }
    s_mmisrv_wifi_bg_on_req_count   = 0;
    s_mmisrv_wifi_status                    = MMISRV_WIFI_CLOSED;
    s_mmisrv_wifi_ap_list_ptr               = PNULL;
    s_mmisrv_wifi_is_ui_on                  = FALSE;
    s_mmisrv_wifi_is_connected = FALSE;

    MMISRVWIFI_TaskInit();
}

LOCAL BOOLEAN WifiOnReq( BOOLEAN is_background )
{
    BOOLEAN         reCode = TRUE;
    xSignalHeader   pSig  = PNULL;

    MMISRVWIFI_MUTEX_LOCK;

    TRACE_SRV_WIFI("is_background %d s_mmisrv_wifi_status %d s_mmisrv_wifi_bg_on_req_count %d",
            (int32)is_background, s_mmisrv_wifi_status, s_mmisrv_wifi_bg_on_req_count);

    switch( s_mmisrv_wifi_status )
    {

        case MMISRV_WIFI_CLOSING:
        case MMISRV_WIFI_CLOSED:
        {
            TRACE_SRV_WIFI("Call WIFISUPP_OnReq");
            reCode = WIFISUPP_OnReq( s_mmisrv_wifi_thread_id );

            if( TRUE == reCode )
            {
                s_mmisrv_wifi_status = MMISRV_WIFI_OPENING;
                TRACE_SRV_WIFI("s_mmisrv_wifi_status change to %d", s_mmisrv_wifi_status);
                if( TRUE == is_background )
                {
                    s_mmisrv_wifi_bg_on_req_count++;
                    s_mmisrv_wifi_req_called[MMISRV_WIFI_BACKGROUND_ON_REQ] = TRUE;
                }
                else
                {
                    s_mmisrv_wifi_is_ui_on = TRUE;
                    s_mmisrv_wifi_req_called[MMISRV_WIFI_ON_REQ] = TRUE;
                }
            }
            else
            {
                TRACE_SRV_WIFI("Call WIFISUPP_OnReq fail");
                reCode = FALSE;
            }

            break;
        }

        case MMISRV_WIFI_OPENING:
        {
            if( TRUE == is_background )
            {
                s_mmisrv_wifi_bg_on_req_count++;
                s_mmisrv_wifi_req_called[MMISRV_WIFI_BACKGROUND_ON_REQ] = TRUE;
            }
            else
            {
                s_mmisrv_wifi_is_ui_on = TRUE;
                s_mmisrv_wifi_req_called[MMISRV_WIFI_ON_REQ] = TRUE;
            }
            break;
        }

        case MMISRV_WIFI_OPENED:
        case MMISRV_WIFI_SCANING:
        case MMISRV_WIFI_CONNECTING:
        case MMISRV_WIFI_CONNECTED:
        case MMISRV_WIFI_DISCONNECTING:
        {
            SCI_CREATE_SIGNAL( pSig, WIFISUPP_SIG_ON_CNF, sizeof(WIFISUPP_ON_CNF_T), SCI_IdentifyThread() );
            ((WIFISUPP_ON_CNF_T*)pSig)->on_result.wifi_on_result = WIFISUPP_RESULT_SUCC;
            SCI_SEND_SIGNAL( pSig, wifiGetThreadId() ) ;

            if( TRUE == is_background )
            {
                s_mmisrv_wifi_bg_on_req_count++;
                s_mmisrv_wifi_req_called[MMISRV_WIFI_BACKGROUND_ON_REQ] = TRUE;
            }
            else
            {
                s_mmisrv_wifi_is_ui_on = TRUE;
                s_mmisrv_wifi_req_called[MMISRV_WIFI_ON_REQ] = TRUE;
            }
            break;
        }

        default:
        {
            TRACE_SRV_WIFI("s_mmisrv_wifi_status error %d", s_mmisrv_wifi_status);
            reCode = FALSE;
            break;
        }

    }

    MMISRVWIFI_MUTEX_UNLOCK;

    TRACE_SRV_WIFI("reCode %d", (int32)reCode);

    return reCode;
}
/****************************************************************************/
//  Description : MMISRVWIFI_OnReq
//  Global resource dependence :
//  Author: bin.wang1
//  Note:
/****************************************************************************/
PUBLIC BOOLEAN MMISRVWIFI_OnReq( void )
{
    return WifiOnReq( FALSE );
}

/****************************************************************************/
//  Description : MMISRVWIFI_BackgroundOnReq
//  Global resource dependence :
//  Author: bin.wang1
//  Note:
/****************************************************************************/
PUBLIC BOOLEAN MMISRVWIFI_BackgroundOnReq( void )
{
    return WifiOnReq( TRUE );
}

LOCAL BOOLEAN WifiOffReq( BOOLEAN is_background )
{
    BOOLEAN         reCode = TRUE;
    xSignalHeader   pSig  = PNULL;

    MMISRVWIFI_MUTEX_LOCK;

    TRACE_SRV_WIFI("is_background %d s_mmisrv_wifi_is_ui_on %d s_mmisrv_wifi_bg_on_req_count %d", (int32)is_background, (int32)s_mmisrv_wifi_is_ui_on, s_mmisrv_wifi_bg_on_req_count);

    if( TRUE == is_background )
    {
        if( s_mmisrv_wifi_bg_on_req_count > 0 )
        {
            s_mmisrv_wifi_bg_on_req_count--;
            s_mmisrv_wifi_req_called[MMISRV_WIFI_BACKGROUND_OFF_REQ] = TRUE;

            if( 0 == s_mmisrv_wifi_bg_on_req_count && FALSE == s_mmisrv_wifi_is_ui_on )
            {
                TRACE_SRV_WIFI("Call WIFISUPP_OffReq");
                reCode = WIFISUPP_OffReq();

                s_mmisrv_wifi_status = MMISRV_WIFI_CLOSING;
                TRACE_SRV_WIFI("s_mmisrv_wifi_status change to %d", s_mmisrv_wifi_status);
            }
            else
            {
                SCI_CREATE_SIGNAL( pSig, WIFISUPP_SIG_OFF_CNF, sizeof(WIFISUPP_OFF_CNF_T), SCI_IdentifyThread() );
                ((WIFISUPP_OFF_CNF_T*)pSig)->off_result.wifi_off_result = WIFISUPP_RESULT_SUCC;
                SCI_SEND_SIGNAL( pSig, wifiGetThreadId() );
            }
        }
        else
        {
            TRACE_SRV_WIFI("Invalid call!!");
            reCode = FALSE;
        }
    }
    else
    {
        if( TRUE == s_mmisrv_wifi_is_ui_on )
        {
            s_mmisrv_wifi_is_ui_on = FALSE;
            s_mmisrv_wifi_req_called[MMISRV_WIFI_OFF_REQ] = TRUE;
            if( 0 == s_mmisrv_wifi_bg_on_req_count )
            {
                TRACE_SRV_WIFI("Call WIFISUPP_OffReq");
                reCode = WIFISUPP_OffReq();

                s_mmisrv_wifi_status = MMISRV_WIFI_CLOSING;
                TRACE_SRV_WIFI("s_mmisrv_wifi_status change to %d", s_mmisrv_wifi_status);
            }
            else
            {
                SCI_CREATE_SIGNAL( pSig, WIFISUPP_SIG_OFF_CNF, sizeof(WIFISUPP_OFF_CNF_T), SCI_IdentifyThread() );
                ((WIFISUPP_OFF_CNF_T*)pSig)->off_result.wifi_off_result = WIFISUPP_RESULT_SUCC;
                SCI_SEND_SIGNAL( pSig, wifiGetThreadId() );
            }
        }
        else
        {
            TRACE_SRV_WIFI("Invalid call!!");
            reCode = FALSE;
        }
    }

    MMISRVWIFI_MUTEX_UNLOCK;

    TRACE_SRV_WIFI("reCode %d", (int32)reCode);

    return reCode;
}

/****************************************************************************/
//  Description : MMISRVWIFI_OffReq
//  Global resource dependence :
//  Author: bin.wang1
//  Note:
/****************************************************************************/
PUBLIC BOOLEAN MMISRVWIFI_OffReq( void )
{
    return WifiOffReq( FALSE );
}

/****************************************************************************/
//  Description : MMISRVWIFI_BackgroundOffReq
//  Global resource dependence :
//  Author: bin.wang1
//  Note:
/****************************************************************************/
PUBLIC BOOLEAN MMISRVWIFI_BackgroundOffReq( void )
{
    return WifiOffReq( TRUE );
}

LOCAL BOOLEAN WifiScanReq( BOOLEAN is_background, WIFISUPP_SCAN_REQ_INFO_T *scan_info_ptr )
{
    BOOLEAN reCode = TRUE;

    if( PNULL == scan_info_ptr )
    {
        TRACE_SRV_WIFI("scan_info_ptr is PNULL!!");
        return FALSE;
    }

    TRACE_SRV_WIFI("ssid     %s", scan_info_ptr->ssid.ssid);
    TRACE_SRV_WIFI("ssid_len %d", (int32)(scan_info_ptr->ssid.ssid_len) );

    MMISRVWIFI_MUTEX_LOCK;

    TRACE_SRV_WIFI("is_background %d s_mmisrv_wifi_status %d", (int32)is_background, s_mmisrv_wifi_status);

    switch( s_mmisrv_wifi_status )
    {

        case MMISRV_WIFI_CLOSING:
        case MMISRV_WIFI_CLOSED:
        case MMISRV_WIFI_OPENING:
        {
            TRACE_SRV_WIFI("Invalid call");
            reCode = FALSE;

            break;
        }

        case MMISRV_WIFI_OPENED:
        case MMISRV_WIFI_CONNECTED:
        {
            TRACE_SRV_WIFI("Call WIFISUPP_ScanReq");
            reCode = WIFISUPP_ScanReq( scan_info_ptr );

            s_mmisrv_wifi_status = MMISRV_WIFI_SCANING;
            TRACE_SRV_WIFI("s_mmisrv_wifi_status change to %d", s_mmisrv_wifi_status);
            if( TRUE == is_background )
            {
                s_mmisrv_wifi_req_called[MMISRV_WIFI_BACKGROUND_SCAN_REQ] = TRUE;
            }
            else
            {
                s_mmisrv_wifi_req_called[MMISRV_WIFI_SCAN_REQ] = TRUE;
            }

            break;
        }

        case MMISRV_WIFI_SCANING:
        {
            if( TRUE == is_background )
            {
                s_mmisrv_wifi_req_called[MMISRV_WIFI_BACKGROUND_SCAN_REQ] = TRUE;
            }
            else
            {
                s_mmisrv_wifi_req_called[MMISRV_WIFI_SCAN_REQ] = TRUE;
            }

            break;
        }

        case MMISRV_WIFI_CONNECTING:
        case MMISRV_WIFI_DISCONNECTING:
        {
            if( TRUE == s_mmisrv_wifi_req_called[MMISRV_WIFI_SCAN_REQ] || TRUE == s_mmisrv_wifi_req_called[MMISRV_WIFI_BACKGROUND_SCAN_REQ] )
            {
                TRACE_SRV_WIFI("Driver scaning, connecting or disconnecting req is waiting");
                if( TRUE == is_background )
                {
                    s_mmisrv_wifi_req_called[MMISRV_WIFI_BACKGROUND_SCAN_REQ] = TRUE;
                }
                else
                {
                    s_mmisrv_wifi_req_called[MMISRV_WIFI_SCAN_REQ] = TRUE;
                }
            }
            else
            {
                TRACE_SRV_WIFI("Call WIFISUPP_ScanReq");
                reCode = WIFISUPP_ScanReq( scan_info_ptr );

                s_mmisrv_wifi_status = MMISRV_WIFI_SCANING;
                TRACE_SRV_WIFI("s_mmisrv_wifi_status change to %d", s_mmisrv_wifi_status);
                if( TRUE == is_background )
                {
                    s_mmisrv_wifi_req_called[MMISRV_WIFI_BACKGROUND_SCAN_REQ] = TRUE;
                }
                else
                {
                    s_mmisrv_wifi_req_called[MMISRV_WIFI_SCAN_REQ] = TRUE;
                }
            }

            break;
        }

        default:
        {
            TRACE_SRV_WIFI("s_mmisrv_wifi_status error %d", s_mmisrv_wifi_status);
            reCode = FALSE;
            break;
        }

    }

    MMISRVWIFI_MUTEX_UNLOCK;

    TRACE_SRV_WIFI("reCode %d", (int32)reCode);

    return reCode;
}
/****************************************************************************/
//  Description : MMISRVWIFI_ScanReq
//  Global resource dependence : 
//  Author: bin.wang1
//  Note:
/****************************************************************************/
PUBLIC BOOLEAN MMISRVWIFI_ScanReq( WIFISUPP_SCAN_REQ_INFO_T *scan_info_ptr )
{
    return WifiScanReq( FALSE, scan_info_ptr );
}

/****************************************************************************/
//  Description : MMISRVWIFI_BackgroundScanReq
//  Global resource dependence :
//  Author: bin.wang1
//  Note:
/****************************************************************************/
PUBLIC BOOLEAN MMISRVWIFI_BackgroundScanReq( WIFISUPP_SCAN_REQ_INFO_T *scan_info_ptr )
{
    return WifiScanReq( TRUE, scan_info_ptr );
}

/****************************************************************************/
//  Description : MMISRVWIFI_ConnectReq
//  Global resource dependence :
//  Author: bin.wang1
//  Note:
/****************************************************************************/
PUBLIC BOOLEAN MMISRVWIFI_ConnectReq( WIFISUPP_SSID_CONFIG_T *connect_info_ptr )
{
    BOOLEAN reCode = TRUE;

    if( PNULL == connect_info_ptr )
    {
        TRACE_SRV_WIFI("connect_info_ptr is PNULL!!");
        return FALSE;
    }

    TRACE_SRV_WIFI("ssid     %s", connect_info_ptr->ssid.ssid);
    TRACE_SRV_WIFI("ssid_len %d", (int32)(connect_info_ptr->ssid.ssid_len) );

    MMISRVWIFI_MUTEX_LOCK;

    TRACE_SRV_WIFI("s_mmisrv_wifi_status %d", s_mmisrv_wifi_status);

    switch( s_mmisrv_wifi_status )
    {

        case MMISRV_WIFI_CLOSING:
        case MMISRV_WIFI_CLOSED:
        case MMISRV_WIFI_OPENING:
        case MMISRV_WIFI_CONNECTED:
        case MMISRV_WIFI_CONNECTING:
        {
            TRACE_SRV_WIFI("Invalid call");
            reCode = FALSE;

            break;
        }

        case MMISRV_WIFI_OPENED:
        case MMISRV_WIFI_SCANING:
        case MMISRV_WIFI_DISCONNECTING:
        {
            TRACE_SRV_WIFI("Call WIFISUPP_ConnectReq ssid %s", connect_info_ptr->ssid.ssid);
            reCode = WIFISUPP_ConnectReq( connect_info_ptr );

            s_mmisrv_wifi_status = MMISRV_WIFI_CONNECTING;
            TRACE_SRV_WIFI("s_mmisrv_wifi_status change to %d", s_mmisrv_wifi_status);
            s_mmisrv_wifi_req_called[MMISRV_WIFI_CONNECT_REQ] = TRUE;

            break;
        }

        default:
        {
            TRACE_SRV_WIFI("s_mmisrv_wifi_status error %d", s_mmisrv_wifi_status);
            reCode = FALSE;
            break;
        }

    }

    MMISRVWIFI_MUTEX_UNLOCK;

    TRACE_SRV_WIFI("reCode %d", (int32)reCode);

    return reCode;
}

/****************************************************************************/
//  Description : MMISRVWIFI_DisConnectReq
//  Global resource dependence :
//  Author: bin.wang1
//  Note:
/****************************************************************************/
PUBLIC BOOLEAN MMISRVWIFI_DisconnectReq( WIFISUPP_DISCONNECT_REQ_INFO_T* disconnect_info_ptr )
{
    BOOLEAN reCode = TRUE;

    if( PNULL == disconnect_info_ptr )
    {
        TRACE_SRV_WIFI("disconnect_info_ptr is PNULL!!");
        return FALSE;
    }

    TRACE_SRV_WIFI("ssid     %s", disconnect_info_ptr->ssid.ssid);
    TRACE_SRV_WIFI("ssid_len %d", (int32)(disconnect_info_ptr->ssid.ssid_len) );

    MMISRVWIFI_MUTEX_LOCK;

    TRACE_SRV_WIFI("s_mmisrv_wifi_status %d", s_mmisrv_wifi_status);

    switch( s_mmisrv_wifi_status )
    {

        case MMISRV_WIFI_CLOSING:
        case MMISRV_WIFI_CLOSED:
        case MMISRV_WIFI_OPENING:
        case MMISRV_WIFI_OPENED:
        {
            TRACE_SRV_WIFI("Invalid call");
            reCode = FALSE;

            break;
        }

        case MMISRV_WIFI_DISCONNECTING:
        {
            // do nothing

            break;
        }

        case MMISRV_WIFI_CONNECTING:
        case MMISRV_WIFI_CONNECTED:
        case MMISRV_WIFI_SCANING:
        {
            TRACE_SRV_WIFI("Call WIFISUPP_DisconnectReq %s", disconnect_info_ptr->ssid.ssid);
            reCode = WIFISUPP_DisconnectReq( disconnect_info_ptr );

            s_mmisrv_wifi_status = MMISRV_WIFI_DISCONNECTING;
            TRACE_SRV_WIFI("s_mmisrv_wifi_status change to %d", s_mmisrv_wifi_status);
            s_mmisrv_wifi_req_called[MMISRV_WIFI_DISCONNECT_REQ] = TRUE;

            break;
        }

        default:
        {
            TRACE_SRV_WIFI("s_mmisrv_wifi_status error %d", s_mmisrv_wifi_status);
            reCode = FALSE;
            break;
        }

    }

    MMISRVWIFI_MUTEX_UNLOCK;

    TRACE_SRV_WIFI("reCode %d", (int32)reCode);

    return reCode;
}

/****************************************************************************/
//  Description : MMISRVWIFI_StartCalcWakeupPacket
//  Global resource dependence :
//  Author: bin.wang1
//  Note: 开始统计唤醒数据包
/****************************************************************************/
PUBLIC BOOLEAN MMISRVWIFI_StartCalcWakeupPacket( uint32 time_range,  uint32 wake_times )
{
    BOOLEAN reCode = TRUE;

    MMISRVWIFI_MUTEX_LOCK;

    TRACE_SRV_WIFI("Call WIFISUPP_CalcOn time_range %d wake_times %d", time_range, wake_times);
    reCode = WIFISUPP_CalcOn( time_range, wake_times );

    MMISRVWIFI_MUTEX_UNLOCK;

    TRACE_SRV_WIFI("reCode %d", (int32)reCode);

    return reCode;
}

/****************************************************************************/
//  Description : MMISRVWIFI_StopCalcWakeupPacket
//  Global resource dependence :
//  Author: bin.wang1
//  Note: 停止统计唤醒数据包
/****************************************************************************/
PUBLIC BOOLEAN MMISRVWIFI_StopCalcWakeupPacket( void )
{
    BOOLEAN reCode = TRUE;

    MMISRVWIFI_MUTEX_LOCK;

    TRACE_SRV_WIFI("Call WIFISUPP_CalcOff");
    reCode = WIFISUPP_CalcOff();

    MMISRVWIFI_MUTEX_UNLOCK;

    TRACE_SRV_WIFI("reCode %d", (int32)reCode);

    return reCode;
}

