/****************************************************************************************
** File Name:      wifisupp_adap_csr.c                                                  *
** Author:         lasse.bigum                                                          *
** Date:           2009.12.15                                                           *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.                  *
** Description:    CSR Synergy WiFi Supplicant implementation for Spreadtrum platform   *
*****************************************************************************************
**                         Important Edit History                                       *
** -------------------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                          *
** 2009.12        lasse.bigum      Create                                               *
**                                                                                      *
*****************************************************************************************/


/**-------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
/* Platform includes */
#include "sci_api.h"
#include "sfs.h"
#include "wifisupp_internal.h"
#include "wifi_drv.h"

/* CSR includes */
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_panic.h"
#include "csr_gsched_init.h"
#include "platform/csr_panic_init.h"
#include "platform/csr_framework_ext_init.h"

/* Wifi Stack tasks declarations */
#include "csr_wifi_tasks.h"

/* IP Stack task declarations */
#include "csr_ip_task.h"

/* Framework tasks declarations */

/* Wifi libs and types */
#include "csr_wifi_sme_prim.h"
#include "csr_wifi_sme_lib.h"
#include "csr_wifi_nme_prim.h"
#include "csr_wifi_nme_lib.h"

#include "csr_wifi_mib_repository.h"

#include "csr_spt.h"

// Enable frame_transmitter for throughput testing directly on the HIP
//#define USE_FRAME_TRANSMITTER

#ifdef USE_FRAME_TRANSMITTER
#include "csr_wifi_frame_transmitter_task.h"
#endif

#ifdef CSR_WIFI_NME_ENABLE
#else
#error_undef
#endif

#include "wifisupp_adap_csr_sef.h"
#ifdef CSR_LOG_ENABLE
#include "csr_log_version.h"
#include "csr_log.h"
#include "csr_log_pcap.h"
#include "csr_log_configure.h"
#include "csr_log_register.h"
#include "platform/csr_logtransport_init.h"
#endif

/* ptest */
#include "csr_wifi_ptest_task.h"
#include "csr_wifi_ptest_app_task.h"
#if defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
#include "csr_framework_ext.h"

extern void *CsrWiFiSchedInit(void);
#endif

extern void CsrDebugTraceLow(const char* format, ...);
extern void CsrDebugTraceHigh(const char* format, ...);
extern void CsrWifiCmdCallbackReg(void);

/**-------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#undef CSR_TRY_TO_GET_PMK_AS_FOR_64HEX_OR_32ASCII
/*lint -e835*/
/*lint -e838*/
extern const CsrWifiSmeStateHandlerType CsrWifiSuppSmeHandlers[CSR_WIFI_SME_PRIM_UPSTREAM_COUNT];
extern const CsrWifiSmeStateHandlerType CsrWifiSuppNmeHandlers[CSR_WIFI_NME_PRIM_UPSTREAM_COUNT];
LOCAL csrWifiSupplicantInstanceData instanceData;

static CsrSchedTid RSSIPollingTimerID = 0;
CsrRssiPollTimerStatus CsrRSSITimerBeSet = RSSI_POLLING_TIMER_DEACTIVE;

/**-------------------------------------------------------------------------*
**                         STRUCTURE DEFINITION                             *
**--------------------------------------------------------------------------*/
/**-------------------------------------------------------------------------*
**                         LOCAL FUNCTION DECLARE                           *
**--------------------------------------------------------------------------*/
LOCAL BOOLEAN CsrWifiSuppOn(void);
LOCAL BOOLEAN CsrWifiSuppOff(void);
//LOCAL BOOLEAN CsrWifiSuppScan(void);
LOCAL BOOLEAN CsrWifiSuppScan(WIFISUPP_SCAN_REQ_INFO_T *scan_req_ptr);

LOCAL BOOLEAN CsrWifiSuppConnect(WIFISUPP_SSID_CONFIG_T *conn_req_ptr);
LOCAL BOOLEAN CsrWifiSuppDisconnect(WIFISUPP_DISCONNECT_REQ_INFO_T *disconn_req_ptr);
LOCAL void CsrWifiHandleCusmsg(uint32 msg_id, void *msg_body_ptr);
LOCAL BOOLEAN CsrWifiSuppGetConnectionInfo(WIFISUPP_SCAN_AP_INFO_T *connection_info_ptr /*[OUT]*/);
LOCAL BOOLEAN CsrWifiSuppCommand(WIFISUPP_COMMAND_INFO_T *command_info_ptr);
#ifdef CSR_TRY_TO_GET_PMK_AS_FOR_64HEX_OR_32ASCII
LOCAL void CsrConvertHex2Bin (CsrUint8* dest_bin, int dest_bin_size, CsrUint8 *src_hex, int src_hex_len);
#endif

/**-------------------------------------------------------------------------*
**                         GLOBAL VARIABLE DEFINITION                       *
**--------------------------------------------------------------------------*/
SUPPADAP_CUSTOM_INFO_T g_suppadap_csr_info =
{
    {
        CsrWifiSuppOn,
        CsrWifiSuppOff,
        CsrWifiSuppScan,
        CsrWifiSuppConnect,
        CsrWifiSuppDisconnect,
        CsrWifiHandleCusmsg,
        CsrWifiSuppGetConnectionInfo,
        CsrWifiSuppCommand
    },  //SUPPADAP_REG_OPER_FUNC_T
    TRUE,//is_support_roam
    {
        TRUE, //WIFISUPP_ENCRYP_PROTOCOL_OPENSYS
        TRUE, //WIFISUPP_ENCRYP_PROTOCOL_WEP
        TRUE, //WIFISUPP_ENCRYP_PROTOCOL_WPA
        TRUE, //WIFISUPP_ENCRYP_PROTOCOL_WPA2
        TRUE  //WIFISUPP_ENCRYP_PROTOCOL_WAPI
    },  //is_support_protocol
    {
        FALSE,   //WIFISUPP_WPA_EAP_TYPE_TLS
        FALSE,   //WIFISUPP_WPA_EAP_TYPE_TTLS
        FALSE,   //WIFISUPP_WPA_EAP_TYPE_PEAP
        FALSE,   //WIFISUPP_WPA_EAP_TYPE_LEAP
        FALSE,   //WIFISUPP_WPA_EAP_TYPE_SIM
        FALSE,   //WIFISUPP_WPA_EAP_TYPE_AKA
        FALSE    //WIFISUPP_WPA_EAP_TYPE_FAST
    },  //is_support_eap
    {
        FALSE,   //WIFISUPP_INNER_CREDENTIAL_TYPE_CHAP
        FALSE,   //WIFISUPP_INNER_CREDENTIAL_TYPE_PAP
        FALSE,   //WIFISUPP_INNER_CREDENTIAL_TYPE_MSCHAP
        FALSE,   //WIFISUPP_INNER_CREDENTIAL_TYPE_MSCHAP_V2
        FALSE    //WIFISUPP_INNER_CREDENTIAL_TYPE_GTC
    }//is_support_inner_eap
};

int wifi_supp_is_on = 0;
CsrBool csrWifiEnableStrictComplianceMode = FALSE;
void CsrWifiOnVarsInit(void);
void CsrWifiOffVarsDeInit(void);
#ifdef CSR_WIFI_ANRITSU_TEST_MODE
CsrBool csrWifiAnritsuTestMode = FALSE;
CsrUint32 csrWifiAnritsuCurrentRate = 0;
void CsrWifiAnritsuRateMatch(void);
#endif

/**-------------------------------------------------------------------------*
**                         STATIC CLASS DEFINITION                          *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         STATIC VARIABLE DEFINITION                       *
**--------------------------------------------------------------------------*/
LOCAL void *schedInstance = NULL;                             /* Scheduler instance */
static BLOCK_ID csrSchedTask = SCI_INVALID_BLOCK_ID;          /* Scheduler task id */
static BLOCK_ID csrWifiMsgHandlerTask = SCI_INVALID_BLOCK_ID; /* Scheduler instance */

/* Define queues for all task */
CsrSchedQid CSR_WIFI_SME_IFACEQUEUE;
CsrSchedQid CSR_WIFI_NME_IFACEQUEUE;
CsrSchedQid CSR_WIFI_ROUTER_IFACEQUEUE;
CsrSchedQid CSR_IP_IFACEQUEUE;
CsrSchedQid CSR_WIFI_FRAME_TRANSMITTER_IFACEQUEUE;
CsrSchedQid CSR_WIFI_PTEST_IFACEQUEUE;
CsrSchedQid CSR_WIFI_PTEST_APP_IFACEQUEUE;

/**-------------------------------------------------------------------------*
**                         IMPLEMENTATION                                   *
**--------------------------------------------------------------------------*/

/****************************************************************************/
//Description : Setup CSR Synergy WiFi scheduler tasks
//Global resource dependence :
//Author: lasse.bigum@
//Note:
/****************************************************************************/
void CsrSchedTaskInit(void *data)
{
#ifdef CSR_LOG_ENABLE
    CSR_SPT_TASK_INIT_FIRST((data, 1, 0));
#endif
	CsrDebugTraceHigh("[wifi]  CsrSchedTaskInit ");

    CsrSchedRegisterTask(&CSR_WIFI_SME_IFACEQUEUE, CsrWifiSmeInit, NULL, CsrWifiSmeHandler, "CSR_WIFI_SME", data, 0);
    CsrSchedRegisterTask(&CSR_WIFI_ROUTER_IFACEQUEUE, CsrWifiRouterInit, NULL, CsrWifiRouterHandler, "CSR_WIFI_ROUTER", data, 0);
#ifndef USE_FRAME_TRANSMITTER
    CsrSchedRegisterTask(&CSR_IP_IFACEQUEUE, CsrIpInit, NULL, CsrIpHandler, "CSR_IP", data, 0);
#endif
    CsrSchedRegisterTask(&CSR_WIFI_NME_IFACEQUEUE, CsrWifiNmeInit, NULL, CsrWifiNmeHandler, "CSR_WIFI_NME", data, 0);
#ifdef USE_FRAME_TRANSMITTER
    CsrSchedRegisterTask(&CSR_WIFI_FRAME_TRANSMITTER_IFACEQUEUE, CsrWifiFrameTransmitterInit, CsrWifiFrameTransmitterDeinit, CsrWifiFrameTransmitterHandler, "FRAME_TRANS", data, 0);
#endif
#ifdef CSR_LOG_ENABLE
    CSR_SPT_TASK_INIT_LAST((data, 1, 0));	
#endif
    /* Ptest */
    CsrSchedRegisterTask(&CSR_WIFI_PTEST_IFACEQUEUE, CsrWifiPtestInit, NULL, CsrWifiPtestHandler, "CSR_WIFI_PTEST", data, 0);
    CsrSchedRegisterTask(&CSR_WIFI_PTEST_APP_IFACEQUEUE, CsrWifiPtestAppInit, NULL, CsrWifiPtestAppHandler, "CSR_WIFI_PTEST_APP", data, 0);
}

/*The call back function for polling RSSI periodically*/
static void CsrRSSIPollCallBack(CsrUint16 fniarg, void *fnvarg)
{
	switch (CsrRSSITimerBeSet)
	{
		case RSSI_POLLING_TIMER_INIT:
			RSSIPollingTimerID = CsrSchedTimerSet(RSSI_POLL_INTERVAL_MS*1000, CsrRSSIPollCallBack, 0, NULL);
			break;
		case RSSI_POLLING_TIMER_ACTIVE:
			CsrWifiSmeLinkQualityGetReqSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE);
			RSSIPollingTimerID = CsrSchedTimerSet(RSSI_POLL_INTERVAL_MS*1000, CsrRSSIPollCallBack, 0, NULL);
			break;
		case RSSI_POLLING_TIMER_CANCEL:
			CsrSchedTimerCancel(RSSIPollingTimerID, 0, NULL);
			RSSIPollingTimerID = 0;
			break;
		default:
			CsrDebugTraceLow("CsrRSSIPollCallBack: Not active timer:%d", CsrRSSITimerBeSet);
			break;
	}
}

/****************************************************************************/
//Description : Handles messages from CSR Synergy WiFi
//Global resource dependence :
//Author: lasse.bigum@
//Note: Since there might not be a one-to-one mapping between Spreadtrum's
//      Wifi supplicant API and CSR Synergy WiFi, not all messages will result
//      in a message for the MMI
/****************************************************************************/
PUBLIC BOOLEAN csrWifiExternalMessage(uint16 q, uint16 eventClass, void *message)
{
    uint32                  sig_size = 0;
    CsrMessageSignalType    *sig_ptr = SCI_NULL;
    BLOCK_ID                sender_thread_id = SCI_IdentifyThread();
	CsrWifiSmePrim *ExPrimType = (CsrWifiSmePrim *) message;

    if (SCI_INVALID_BLOCK_ID == csrWifiMsgHandlerTask)
    {
        CsrPanic(123, 9999, "csrWifiExternalMessage - csrWifiMsgHandlerTask not ready yet!");
    }


	if(*ExPrimType == CSR_WIFI_SME_LINK_QUALITY_GET_CFM && 
		RSSIPollingTimerID == 0 && 
		CsrRSSITimerBeSet == RSSI_POLLING_TIMER_INIT){
		RSSIPollingTimerID = CsrSchedTimerSet(RSSI_POLL_INTERVAL_MS*1000,CsrRSSIPollCallBack,0,NULL);
		CsrDebugTraceLow("csrWifiExternalMessage:start rssi timer:%d, timer id:%d", SCI_GetTickCount(), RSSIPollingTimerID);
	}

    sig_size = sizeof(CsrMessageSignalType);
    SCI_CREATE_SIGNAL(sig_ptr,
                      eventClass,
                      sig_size,
                      sender_thread_id);

    if (SCI_NULL == sig_ptr) /*lint !e774*/
    {
        CsrDebugTraceHigh("csrWifiExternalMessage - alloc sig_ptr fail!");
        return FALSE;
    }
    sig_ptr->message = message;
    SCI_SEND_SIGNAL(sig_ptr, csrWifiMsgHandlerTask);

    return TRUE;

}

#if 0
// Unused for now
LOCAL CsrUint32 csrHelperOpenSpreadtrumFile(CFLWCHAR fileName, CsrChar *fileBuffer, CsrUint32 bytesToRead)
{
    CsrUint32 bytesRead = 0;
    SFS_HANDLE handle = 0;
    SFS_ERROR_E result = SFS_NO_ERROR;
    handle = SFS_CreateFile(fileName, SFS_MODE_READ, NULL, NULL);

    if (handle != 0)
    {
        result = SFS_ReadFile(handle, fileBuffer, bytesToRead, &bytesRead, NULL);

        if (result != SFS_NO_ERROR)
        {
            CsrDebugTraceHigh("csrHelperOpenSpreadtrumFile - failed to read from file: %s, bytes: %d", fileName, bytesToRead);
            fileBuffer = NULL;
            bytesRead = 0;
        }
    }

    return result;
}
#endif

#if defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
PUBLIC void csrWifiMsgHandler(void* ptr)
#else
PUBLIC void csrWifiMsgHandler(uint32 argc, void* argv)
#endif
{
    BLOCK_ID task_id = SCI_IdentifyThread();
    CsrMessageSignalType *sig_ptr = 0;

    CsrDebugTraceLow("csrWifiMsgHandler - started - waiting for events");

#if 0
Overview of when the following events should be sent to the MMI:
===
WIFISUPP_OFF_CNF_T;   //wifi off confirmation = Maps to WifiOffCfm
WIFISUPP_ON_CNF_T;    //wifi on confirmation = Maps to WifiOnCfm
WIFISUPP_DISCONNECT_CNF_T;    //disconnect confirmation = Maps to DisconnectCfm
WIFISUPP_CONNECT_CNF_T;   //connect confirmation = Maps to ConnectCfm
WIFISUPP_SCAN_CNF_T;  //a scan is finished in a period = Maps to ScanFullCfm
WIFISUPP_SCAN_IND_T;  //scan indication when new ap is scanned = Maps to ScanResultInd
WIFISUPP_DISCONNECT_IND_T;    //disconnect indication = Maps to MediaStatusInd + IbssStationInd
WIFISUPP_RSSI_CHANGED_IND_T;  //RSSI changed indication = Maps to ConnectionQualityInd
WIFISUPP_BSSID_CHANGED_IND_T; //BSSID changed indication = Maps to IbssStationInd
#endif

    while (TRUE) /*lint !e716*/
    {
        CsrUint16 eventClass = 0;
        // Wait for signal to arrive from CSR Synergy WiFi external message handler
        sig_ptr = (CsrMessageSignalType *) SCI_GetSignal(task_id);

        // Retreive data from signal and free signal pointer
        eventClass = sig_ptr->SignalCode;
        instanceData.recvMessage = sig_ptr->message;
        SCI_FREE(sig_ptr);

        //CsrDebugTraceLow("csrWifiMsgHandler - eventClass: 0x%04X, message: 0x%08X (prim: 0x%04X)", eventClass,  instanceData.recvMessage, *((CsrUint16 *)  instanceData.recvMessage));

        switch (eventClass)
        {
            case CSR_WIFI_SME_PRIM:
            {
                CsrWifiSmePrim *primType = (CsrWifiSmePrim *) instanceData.recvMessage;
                if ((*primType - CSR_WIFI_SME_PRIM_UPSTREAM_LOWEST)< CSR_WIFI_SME_PRIM_UPSTREAM_COUNT)
                {
                    if ((CsrWifiSuppSmeHandlers[*primType - CSR_WIFI_SME_PRIM_UPSTREAM_LOWEST]) != NULL)
                    {
                        CsrWifiSuppSmeHandlers[*primType - CSR_WIFI_SME_PRIM_UPSTREAM_LOWEST](&instanceData);
                    }
                    else
                    {
                        CsrDebugTraceLow("Unhandled CSR_WIFI_SME_PRIM prim - empty handler!");
                    }
                    /* clean msg content of downstream messages. Some handlers want to keep to use message
                     * by setting instanceData.recvMessage to NULL
                     */
                    if (instanceData.recvMessage != NULL)
                        CsrWifiSmeFreeUpstreamMessageContents(eventClass, instanceData.recvMessage);
                }
                else
                {
                    CsrPanic(CSR_TECH_WIFI, CSR_PANIC_FW_UNEXPECTED_VALUE, "Unknown CSR_WIFI_SME message");
                }
                break;
            }
            case CSR_WIFI_NME_PRIM:
            {
                CsrWifiNmePrim *primType = (CsrWifiNmePrim *)  instanceData.recvMessage;

                if ((*primType - CSR_WIFI_NME_PRIM_UPSTREAM_LOWEST)< CSR_WIFI_NME_PRIM_UPSTREAM_COUNT)
                {
                    if ((CsrWifiSuppNmeHandlers[*primType - CSR_WIFI_NME_PRIM_UPSTREAM_LOWEST]) != NULL)
                    {
                        CsrWifiSuppNmeHandlers[*primType - CSR_WIFI_NME_PRIM_UPSTREAM_LOWEST](&instanceData);
                    }
                    else
                    {
                        CsrDebugTraceLow("Unhandled CSR_WIFI_NME_PRIM prim - empty handler!");
                    }
                    /* clean msg content of downstream messages. Some handlers want to keep to use message
                     * by setting instanceData.recvMessage to NULL
                     */
                    if (instanceData.recvMessage != NULL)
                        CsrWifiNmeFreeUpstreamMessageContents(eventClass, instanceData.recvMessage);
                }
                else
                {
                    CsrPanic(CSR_TECH_WIFI, CSR_PANIC_FW_UNEXPECTED_VALUE, "Unknown CSR_WIFI_NME message");
                }

                break;
            }
            default:
            {
                CsrDebugTraceLow("csrWifiExternalMessage - Unhandled eventClass: 0x%04X", eventClass);
                break;
            }
        }
        /* Some handlers want to keep to use message by setting instanceData.recvMessage to NULL */
        if (instanceData.recvMessage != NULL)
            CsrPmemFree( instanceData.recvMessage);
    }
}

/* Panic handlers */
static void CsrPanicCb(CsrUint8 tech, CsrUint16 reason, const char *logtext)
{
    CsrDebugTraceHigh("PANIC: %s (tech %hhu, reason %hu)\n", logtext, tech, reason);
    switch(reason)
    {
        case CSR_PANIC_FW_UNEXPECTED_VALUE:
        case CSR_PANIC_FW_HEAP_EXHAUSTION:
        case CSR_PANIC_FW_INVALID_PFREE_POINTER:
        {   /* Make Spreadtrum platform assert, always */
            //SCI_ASSERT(FALSE);
            /* SR28237 - Ignore these events at your own peril! */
            CsrDebugTraceLow("PANIC: Ignoring....");
            break;
        }
        case CSR_PANIC_FW_EXCEPTION:
        default:
        {   /* Ignore these events at your own peril! */
            CsrDebugTraceLow("PANIC: Ignoring....");
            break;
        }
    }
}

/* Dumb wrapper to start scheduler */
#if defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
LOCAL void csrSchedStart(void *ptr)
#else
LOCAL void csrSchedStart(uint32 argc, void *argv)
#endif
{
    CsrDebugTraceHigh("csrSchedStart \n");
    CsrSched(schedInstance);
}
extern void CsrMemAllocDmaInit(void);

/****************************************************************************/
//Description : Initialize CSR Synergy WiFi supplicant
//Global resource dependence :
//Author: lasse.bigum@
//Note: Initialize CSR Synergy WiFi supplicant
/****************************************************************************/
PUBLIC void CsrWifiSupplicantInitialize(CsrBool enableStrictMode)
{

#if defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
CsrResult result;
#endif

    // Initialize platform random number generator
    srand(SCI_GetTickCount()); //we often use the current tick to be the seed, current tick can be get by SCI_GetTickCount()

    // Initialize CSR WiFi Supplicant instance data
    instanceData.wifiOn = FALSE;
    instanceData.connectReq = NULL;
    instanceData.sharedConnect = FALSE;
    instanceData.disconnectReq = NULL;
    instanceData.currentConnectionInfo = NULL;
    instanceData.currentConnectionQuality = NULL;
    //instanceData.autoScan_info_ptr = NULL;
#ifdef CSR_WIFI_NME_AUTOCONNECT
    instanceData.profileIdentitys = NULL;
    instanceData.profileIdentitysCount = 0;
#endif
    instanceData.scan_req_info_ptr = NULL;
    instanceData.command_info_ptr = NULL;

    // Set the compliance mode parameter
    csrWifiEnableStrictComplianceMode =  enableStrictMode;

    //*** Initialize scheduler start ***//
    CsrPanicInit(CsrPanicCb);

	CsrMemAllocDmaInit();
#ifdef CSR_LOG_ENABLE
     //SPT
    CsrSptInit();

    // Use the one of the below functions to change the log level of Synergy - default is Medium
//    CsrSptLogLevelLow()
//    CsrSptLogLevelMedium()
//    CsrSptLogLevelHigh()
#endif
    /* Super important - create global lock before HIP starts using it! */
    if (CsrGlobalMutexCreate() != CSR_RESULT_SUCCESS)
    {
        CsrPanic(123, 9999, "CsrWifiSupplicantInitialize - failed to create global mutex!");
    }

    /* Start scheduler */
#if defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
    schedInstance = CsrWiFiSchedInit();

    result = CsrThreadCreateExt(csrSchedStart,
        NULL, 
        0, // Set in CsrThreadCreateExt()
        0, // Set in CsrThreadCreateExt()
        "CSR_WIFI_SCH",
        (CsrThreadHandle *)&csrSchedTask);
    
    if(result != CSR_RESULT_SUCCESS)
    {
        CsrPanic(123, 9999, "failed to create thread");
    }
#else
    schedInstance = CsrSchedInit(0, CSR_DEFAULT_PRIORITY, CSR_DEFAULT_STACK_SIZE);

    csrSchedTask = SCI_CreateThread("CSR_SCH",
                                    "CSR_SCQ",
                                    csrSchedStart,
                                    0,
                                    NULL,
                                    CSR_DEFAULT_STACK_SIZE,
                                    CSR_DEFAULT_QUEUE_NUM,
                                    CSR_DEFAULT_PRIORITY,
                                    SCI_PREEMPT,
                                    SCI_AUTO_START);
#endif
    if (csrSchedTask == SCI_INVALID_BLOCK_ID) {
        CsrPanic(123, 9999, "failed to create thread");
    }
    
    //*** Initialize scheduler end ***//

    // Call function that creates WifiSupplicant which will use CSR Synergy WiFi Supplicant implementation
    WIFISUPP_Init();

    // Register handler function for messages coming from CSR Synergy WiFi
    CsrSchedRegisterExternalSend(csrWifiExternalMessage);

    // Create thread that will receive messages from external send handler function
#if defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
    result = CsrThreadCreateExt(csrWifiMsgHandler,
        NULL, 
        0, // Set in CsrThreadCreateExt()
        0, // Set in CsrThreadCreateExt()
        "CSR_WIFI_WMH",
        (CsrThreadHandle *)&csrWifiMsgHandlerTask);
    
    if(result != CSR_RESULT_SUCCESS)
    {
        CsrPanic(123, 9999, "failed to create thread");
    }
#else
    csrWifiMsgHandlerTask = SCI_CreateThread("CSR_WMH_T",
                                             "CSR_WMH_Q",
                                             csrWifiMsgHandler,
                                             0,
                                             NULL,
                                             CSR_DEFAULT_STACK_SIZE,
                                             CSR_DEFAULT_QUEUE_NUM,
                                             CSR_DEFAULT_PRIORITY,
                                             SCI_PREEMPT,
                                             SCI_AUTO_START);
#endif

    // Register WiFi command callback
    CsrWifiCmdCallbackReg();

    // Activate SME  
    CsrWifiSmeActivateReqSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE);
}

/****************************************************************************/
//Description : wifi on
//Global resource dependence :
//Author: juan.zhang
//Note:
/****************************************************************************/
void CsrWifiGetMacAddress (unsigned char *maddr)
{
    uint32 ret;
    WIFI_ADDR_T mac = {0};
    unsigned char *pmac;
    unsigned char mac_zero[WIFI_NV_ADDR_LEN] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    ret = WIFI_GetNvParam (&mac, NULL);
    pmac = mac.address;
    if (ret != WIFI_RET_OK || pmac[0] == 0xff || CsrMemCmp (pmac, mac_zero, WIFI_NV_ADDR_LEN) == 0) {
        /* Set to default value */
        srand (SCI_GetTickCount ());        
        pmac[0] = 0x00;
        pmac[1] = 0x02;
        pmac[2] = 0x5b;
        pmac[3] = (unsigned long)rand() & 0xff;
        pmac[4] = (unsigned long)rand() & 0xff;
        pmac[5] = (unsigned long)rand() & 0xff;

        /* Save to NV RAM */
        WIFI_SaveNvParam (&mac, NULL);
    }

    CsrMemCpy (maddr, pmac, WIFI_NV_ADDR_LEN);
    CsrDebugTraceLow("MAC ADDR =   %02X:%02X:%02X:%02X:%02X:%02X\n",
        maddr[0], maddr[1], maddr[2], maddr[3], maddr[4], maddr[5]);
    return;
}
         
   
void CsrWifiOn (void)
{
    CsrWifiMacAddress macAddress;

    /* MAC */
    CsrWifiGetMacAddress (macAddress.a);

#ifdef CSR_LOG_ENABLE
    {
    extern void CsrWifiSmeRegisterLog(void);
#if 0
    extern void CsrWifiHipConfigureLog (void);
    CsrWifiHipConfigureLog ();
#endif
    CsrWifiSmeRegisterLog ();
    }
#endif
    /* MIB will be chosen inside the SME */
   CsrWifiSmeWifiOnReqSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE, macAddress, 0, NULL);
}


LOCAL BOOLEAN CsrWifiSuppOn(void)
{
    if (wifi_supp_is_on)
    {
        WIFISUPP_ON_RESULT_T resultData;
        resultData.wifi_on_result = WIFISUPP_RESULT_FAIL;
        CsrDebugTraceLow("%s ERROR - wifi_supp_is_on: %d",  __func__, wifi_supp_is_on);
        SUPPMAIN_NotifyApp(WIFISUPP_SIG_ON_CNF, &resultData);
        return FALSE;
    }
    CsrDebugTraceLow("CSR_WIFI_SUPP_ON");
    CsrWifiOnVarsInit();
    CsrWifiOn();
    return TRUE;
}

/****************************************************************************/
//Description : wifi off
//Global resource dependence :
//Author: juan.zhang
//Note:
/****************************************************************************/
LOCAL BOOLEAN CsrWifiSuppOff(void)
{
    if (!wifi_supp_is_on)
    {
        WIFISUPP_OFF_RESULT_T resultData;
        resultData.wifi_off_result = SCI_ERROR;
        CsrDebugTraceLow("%s ERROR - wifi_supp_is_on: %d", __func__, wifi_supp_is_on);
        SUPPMAIN_NotifyApp(WIFISUPP_SIG_OFF_CNF, &resultData);
        return FALSE;
    }
    CsrDebugTraceLow("CSR_WIFI_SUPP_OFF");
    CsrWifiSmeWifiOffReqSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE);
    CsrWifiOffVarsDeInit();
    return TRUE;
}

/****************************************************************************/
//Description : scan
//Global resource dependence :
//Author: juan.zhang
//Note:
/****************************************************************************/
//LOCAL BOOLEAN CsrWifiSuppScan(void)
LOCAL BOOLEAN CsrWifiSuppScan(WIFISUPP_SCAN_REQ_INFO_T *scan_req_ptr)
{
    CsrWifiSsid *ssid = NULL;
    CsrWifiMacAddress bssid;

    if (!wifi_supp_is_on || (instanceData.scan_req_info_ptr != NULL))
    {
        WIFISUPP_SCAN_CNF_T resultData;
        resultData.scan_result = WIFISUPP_RESULT_FAIL;
        CsrDebugTraceLow("%s ERROR - wifi_supp_is_on: %d, instanceData.scan_req_info_ptr: %ld",
            __func__,
            wifi_supp_is_on,
            instanceData.scan_req_info_ptr);
        SUPPMAIN_NotifyApp(WIFISUPP_SIG_SCAN_CNF, &resultData);
        return FALSE;
    }
    CsrDebugTraceLow("CSR_WIFI_SUPP_SCAN - ssid: %s, length: %d", scan_req_ptr->ssid.ssid, scan_req_ptr->ssid.ssid_len);
    instanceData.scan_req_info_ptr = CsrPmemAlloc(sizeof(WIFISUPP_SCAN_REQ_INFO_T));
    if (scan_req_ptr->ssid.ssid_len != 0)
    {
        // For scan specified AP
        CsrMemCpy(instanceData.scan_req_info_ptr, scan_req_ptr, sizeof(WIFISUPP_SCAN_REQ_INFO_T));
        ssid = CsrPmemAlloc(sizeof(CsrWifiSsid));
        CsrMemSet(ssid, 0, sizeof(CsrWifiSsid));
        ssid->length = scan_req_ptr->ssid.ssid_len;
        CsrMemCpy(ssid->ssid,scan_req_ptr->ssid.ssid , WIFISUPP_SSID_MAX_LEN); 
        CsrMemSet(bssid.a, 0xFF, WIFISUPP_BSSID_LEN);
        CsrWifiSmeScanFullReqSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE, 1, ssid, bssid, TRUE, 0, 0, 0, NULL, 0, NULL);
    }
    else
    {
        // For scan full
        CsrMemSet(instanceData.scan_req_info_ptr, 0, sizeof(WIFISUPP_SCAN_REQ_INFO_T));
        CsrMemSet(bssid.a, 0xFF, WIFISUPP_BSSID_LEN);
        CsrWifiSmeScanFullReqSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE, 0, NULL, bssid, TRUE, 0, 0, 0, NULL, 0, NULL);
    }
    return TRUE;
}

#if 0
/* Select the WEP key that is NOT '00000' - or if all are '00000' select index  */
CsrUint8 getWepKeyIndex(WIFISUPP_SSID_CONFIG_T *ssidConfig)
{
    CsrUint8 index = 1;

    CsrDebugTraceLow("getWepKeyIndex - keyType: %d", ssidConfig->credential.wep_credential.key_type);

    switch (ssidConfig->credential.wep_credential.key_type)
    {
        case WIFISUPP_WEP_KEY_TYPE_64BITS:
        {
            CsrUint8 ii = 0;
            for (ii = 0; ii < 4; ii++)
            {
                CsrUint8 emptyKey[5] = { 0 };
                /* If WEP key at index ii is NOT 0, use this index */
                if (CsrMemCmp(ssidConfig->credential.wep_credential.key.key_64bits_arr[ii], emptyKey, 5) != 0)
                {
                    index = ii + 1;
                    CsrDebugTraceLow("getWepKeyIndex64 - found key at index: %d", index);
                    break;
                }
            }

            break;
        }
        case WIFISUPP_WEP_KEY_TYPE_128BITS:
        {
            CsrUint8 ii = 0;
            for (ii = 0; ii < 4; ii++)
            {
                CsrUint8 emptyKey[13] = { 0 };
                /* If WEP key at index ii is NOT 0, use this index */
                if (CsrMemCmp(ssidConfig->credential.wep_credential.key.key_128bits_arr[ii], emptyKey, 13) != 0)
                {
                    index = ii + 1;
                    CsrDebugTraceLow("getWepKeyIndex128 - found key at index: %d", index);
                    break;
                }
            }
            break;
        }
        default:
            CsrDebugTraceLow("Error, Unknown keytype=%d", ssidConfig->credential.wep_credential.key_type);
            break;
    }
    return index;
}
#endif

/****************************************************************************/
//Description : connect
//Global resource dependence :
//Author: juan.zhang
//Note:
/****************************************************************************/
LOCAL BOOLEAN CsrWifiSuppConnect(WIFISUPP_SSID_CONFIG_T *conn_req_ptr)
{
    CsrWifiNmeProfile profile;
    CsrBool securitySupported = TRUE;

    // Copy info needed for connect_cnf
    instanceData.connectReq = CsrPmemAlloc(sizeof(WIFISUPP_SSID_CONFIG_T));
    CsrMemCpy(instanceData.connectReq, conn_req_ptr, sizeof(WIFISUPP_SSID_CONFIG_T));

    // Setup NME profile information
    CsrMemCpy(&profile.profileIdentity.ssid, &conn_req_ptr->ssid, sizeof(WIFISUPP_SSID_T));
//    CsrMemCpy(profile.profileIdentity.bssid.a, conn_req_ptr->bssid_info.bssid, WIFISUPP_BSSID_LEN);
    CsrMemSet(profile.profileIdentity.bssid.a, 0xFF, WIFISUPP_BSSID_LEN);

    profile.bssType = conn_req_ptr->network_mode == WIFISUPP_NETWORK_MODE_INFRASTRUCTURE ? CSR_WIFI_NME_BSS_TYPE_INFRASTRUCTURE : CSR_WIFI_NME_BSS_TYPE_ADHOC;
    profile.ccxOptionsMask = 0;
    profile.channelNo = conn_req_ptr->channel;
    profile.cloakedSsid = FALSE;
    profile.wmmQosCapabilitiesMask = 0;

    switch (conn_req_ptr->encryp_protocol)
    {
        case WIFISUPP_ENCRYP_PROTOCOL_WEP:
        {
            CsrDebugTraceLow("WEP Selected: keyType: %d", conn_req_ptr->credential.wep_credential.key_type);

            /* Always use Open, can only use Open *or* Shared*. Cannot detect at runtime,
               so use Open as that *might* be accepted by an AP running Shared */
            if (conn_req_ptr->credential.wep_credential.key_type == WIFISUPP_WEP_KEY_TYPE_64BITS)
            {
                profile.credentials.credentialType = CSR_WIFI_NME_CREDENTIAL_TYPE_WEP64;
                //profile.credentials.credential.wep64Key.selectedWepKey = getWepKeyIndex(conn_req_ptr);
			    profile.credentials.credential.wep64Key.selectedWepKey = conn_req_ptr->credential.wep_credential.key_in_use + 1;

				profile.credentials.credential.wep64Key.wepAuthType = CSR_WIFI_NME_AUTH_MODE_80211_OPEN;   //peishan pm07

                CsrMemCpy(profile.credentials.credential.wep64Key.key1, &conn_req_ptr->credential.wep_credential.key.key_64bits_arr[0][0], WIFISUPP_WEP_64BITS_KEY_LEN - WIFISUPP_WEP_IV_LEN);
                CsrMemCpy(profile.credentials.credential.wep64Key.key2, &conn_req_ptr->credential.wep_credential.key.key_64bits_arr[1][0], WIFISUPP_WEP_64BITS_KEY_LEN - WIFISUPP_WEP_IV_LEN);
                CsrMemCpy(profile.credentials.credential.wep64Key.key3, &conn_req_ptr->credential.wep_credential.key.key_64bits_arr[2][0], WIFISUPP_WEP_64BITS_KEY_LEN - WIFISUPP_WEP_IV_LEN);
                CsrMemCpy(profile.credentials.credential.wep64Key.key4, &conn_req_ptr->credential.wep_credential.key.key_64bits_arr[3][0], WIFISUPP_WEP_64BITS_KEY_LEN - WIFISUPP_WEP_IV_LEN);
            }
            else
            {
                profile.credentials.credentialType = CSR_WIFI_NME_CREDENTIAL_TYPE_WEP128;
                //profile.credentials.credential.wep128Key.selectedWepKey = getWepKeyIndex(conn_req_ptr);
			    profile.credentials.credential.wep128Key.selectedWepKey = conn_req_ptr->credential.wep_credential.key_in_use + 1;
				//CsrDebugTraceLow("profile.credentials.credential.wep128Key.selectedWepKey is %d", profile.credentials.credential.wep64Key.selectedWepKey);
                
				profile.credentials.credential.wep128Key.wepAuthType = CSR_WIFI_NME_AUTH_MODE_80211_OPEN;   //peishan pm07
                	

                CsrMemCpy(profile.credentials.credential.wep128Key.key1, &conn_req_ptr->credential.wep_credential.key.key_128bits_arr[0][0], WIFISUPP_WEP_128BITS_KEY_LEN - WIFISUPP_WEP_IV_LEN);
                CsrMemCpy(profile.credentials.credential.wep128Key.key2, &conn_req_ptr->credential.wep_credential.key.key_128bits_arr[1][0], WIFISUPP_WEP_128BITS_KEY_LEN - WIFISUPP_WEP_IV_LEN);
                CsrMemCpy(profile.credentials.credential.wep128Key.key3, &conn_req_ptr->credential.wep_credential.key.key_128bits_arr[2][0], WIFISUPP_WEP_128BITS_KEY_LEN - WIFISUPP_WEP_IV_LEN);
                CsrMemCpy(profile.credentials.credential.wep128Key.key4, &conn_req_ptr->credential.wep_credential.key.key_128bits_arr[3][0], WIFISUPP_WEP_128BITS_KEY_LEN - WIFISUPP_WEP_IV_LEN);
            }
            break;
        }
        case WIFISUPP_ENCRYP_PROTOCOL_WPA:
        {
            // Pre-Shared Key (PSK) or EAP
            CsrDebugTraceLow("WPA Selected: keyType: %d", conn_req_ptr->credential.wpa_credential.credential_type);
            if (conn_req_ptr->credential.wpa_credential.credential_type == WIFISUPP_WPA_CREDENTIAL_TYPE_EAP)
            {
                // EAPOL == Enterprise, not needed yet
                // Not Supported
                securitySupported = FALSE;
                switch (conn_req_ptr->credential.wpa_credential.credential_info.wpa_eap_info.eap_type)
                {
                    case WIFISUPP_WPA_EAP_TYPE_TLS:
                    {
                        profile.credentials.credentialType = CSR_WIFI_NME_CREDENTIAL_TYPE_8021X_TLS;
/*
                        profile.credentials.credential.tls.authMode = CSR_WIFI_NME_AUTH_MODE_8021X_WPA;
                        profile.credentials.credential.tls.encryptionMode = CSR_WIFI_SME_ENCRYPTION_CIPHER_PAIRWISE_TKIP |
                                                                            CSR_WIFI_SME_ENCRYPTION_CIPHER_PAIRWISE_CCMP |
                                                                            CSR_WIFI_SME_ENCRYPTION_CIPHER_GROUP_TKIP |
                                                                            CSR_WIFI_SME_ENCRYPTION_CIPHER_GROUP_CCMP;
                        profile.credentials.credential.tls.username = "";        // Not necessary? Check up on this
                        profile.credentials.credential.tls.sessionLength = 0;    // Possibly stored from a NME_PROFILE_UPDATE_IND
                        profile.credentials.credential.tls.session = NULL;       // Possibly stored from a NME_PROFILE_UPDATE_IND

                        if (conn_req_ptr->credential.wpa_credential.credential_info.wpa_eap_info.eap_info.tls_info.is_use_file)
                        {
                            CsrUint8 buffer[256];
                            CsrUint32 bytesRead;
                            SFS_HANDLE handle;
                            handle = csrHelperOpenSpreadtrumFile(conn_req_ptr->credential.wpa_credential.credential_info.wpa_eap_info.eap_info.tls_info.certificate_file_path);

                            SFS_ReadFile(handle, &buffer, 256, &bytesRead, NULL);
                        }
                        else
                        {
                            profile.credentials.credential.tls.certificateLength = conn_req_ptr->credential.wpa_credential.credential_info.wpa_eap_info.tls_info.certificate_buf_len;
                            profile.credentials.credential.tls.certificate = conn_req_ptr->credential.wpa_credential.credential_info.wpa_eap_info.tls_info.certificate_buf_ptr;
                        }

                        profile.credentials.credential.tls.privateKeyLength = 0; // Read this from the certificate - certificate is in PEM format
                        profile.credentials.credential.tls.privateKey = NULL;    // Read this from the certificate - certificate is in PEM format
*/
                        break;
                    }
                    case WIFISUPP_WPA_EAP_TYPE_TTLS:
                    {
                        profile.credentials.credentialType = CSR_WIFI_NME_CREDENTIAL_TYPE_8021X_TTLS;
/*
                        profile.credentials.credential.ttls.authMode = CSR_WIFI_NME_AUTH_MODE_8021X_WPA;
                        profile.credentials.credential.ttls.encryptionMode = CSR_WIFI_SME_ENCRYPTION_CIPHER_PAIRWISE_TKIP |
                                                                             CSR_WIFI_SME_ENCRYPTION_CIPHER_PAIRWISE_CCMP |
                                                                             CSR_WIFI_SME_ENCRYPTION_CIPHER_GROUP_TKIP |
                                                                             CSR_WIFI_SME_ENCRYPTION_CIPHER_GROUP_CCMP;
                        profile.credentials.credential.ttls.authServerUserIdentity = "";
                        profile.credentials.credential.ttls.username = CsrPmemAlloc(WIFISUPP_MAX_USERNAME_LEN+1);
                        profile.credentials.credential.ttls.password = CsrPmemAlloc(WIFISUPP_MAX_PSW_LEN+1);
                        CsrMemCpy(profile.credentials.credential.ttls.username, conn_req_ptr->credential.wpa_credential.credential_info.wpa_eap_info.ttls_info.username_arr, WIFISUPP_MAX_USERNAME_LEN+1);
                        CsrMemCpy(profile.credentials.credential.ttls.password, conn_req_ptr->credential.wpa_credential.credential_info.wpa_eap_info.ttls_info.psw_arr, WIFISUPP_MAX_PSW_LEN+1);
                        profile.credentials.credential.ttls.sessionLength = 0; // Possibly stored from a NME_PROFILE_UPDATE_IND
                        profile.credentials.credential.ttls.session = NULL; // Possibly stored from a NME_PROFILE_UPDATE_IND
*/
                        break;
                    }
                    case WIFISUPP_WPA_EAP_TYPE_PEAP:
                    {
                        profile.credentials.credentialType = CSR_WIFI_NME_CREDENTIAL_TYPE_8021X_PEAP_GTC; //Or CSR_WIFI_NME_CREDENTIAL_TYPE_8021X_PEAP_MSCHAPV2??
/*
                        profile.credentials.credential.peapGtc.authMode = CSR_WIFI_NME_AUTH_MODE_8021X_WPA;
                        profile.credentials.credential.peapGtc.encryptionMode = CSR_WIFI_SME_ENCRYPTION_CIPHER_PAIRWISE_TKIP |
                                                                                CSR_WIFI_SME_ENCRYPTION_CIPHER_PAIRWISE_CCMP |
                                                                                CSR_WIFI_SME_ENCRYPTION_CIPHER_GROUP_TKIP |
                                                                                CSR_WIFI_SME_ENCRYPTION_CIPHER_GROUP_CCMP;
                        profile.credentials.credential.peapGtc.authServerUserIdentity = ;
                        profile.credentials.credential.peapGtc.username = ;
                        profile.credentials.credential.peapGtc.password = ;
                        profile.credentials.credential.peapGtc.sessionLength = 0; // Possibly stored from a NME_PROFILE_UPDATE_IND
                        profile.credentials.credential.peapGtc.session = NULL; // Possibly stored from a NME_PROFILE_UPDATE_IND
*/
                        break;
                    }
                    case WIFISUPP_WPA_EAP_TYPE_LEAP:
                    {
                        profile.credentials.credentialType = CSR_WIFI_NME_CREDENTIAL_TYPE_8021X_LEAP;
/*
                        profile.credentials.credential.leap.encryptionMode = CSR_WIFI_SME_ENCRYPTION_CIPHER_PAIRWISE_TKIP |
                                                                             CSR_WIFI_SME_ENCRYPTION_CIPHER_PAIRWISE_CCMP |
                                                                             CSR_WIFI_SME_ENCRYPTION_CIPHER_GROUP_TKIP |
                                                                             CSR_WIFI_SME_ENCRYPTION_CIPHER_GROUP_CCMP;
                        profile.credentials.credential.leap.username = ;
                        profile.credentials.credential.leap.password = ;
*/
                        break;
                    }
                    case WIFISUPP_WPA_EAP_TYPE_SIM:
                    {
                        // NOT SUPPORTED
                        profile.credentials.credentialType = 0; // Available only in Synergy WiFi 4.0
                        break;
                    }
                    case WIFISUPP_WPA_EAP_TYPE_AKA:
                    {
                        // NOT SUPPORTED
                        profile.credentials.credentialType = 0; // Available only in Synergy WiFi 4.0
                        break;
                    }
                    case WIFISUPP_WPA_EAP_TYPE_FAST:
                    {
                        profile.credentials.credentialType = CSR_WIFI_NME_CREDENTIAL_TYPE_8021X_FAST;
/*
                        profile.credentials.credential.fast.eapCredentials.authMode = CSR_WIFI_NME_AUTH_MODE_8021X_WPA;
                        profile.credentials.credential.fast.eapCredentials.encryptionMode = CSR_WIFI_SME_ENCRYPTION_CIPHER_PAIRWISE_TKIP |
                                                                                            CSR_WIFI_SME_ENCRYPTION_CIPHER_PAIRWISE_CCMP |
                                                                                            CSR_WIFI_SME_ENCRYPTION_CIPHER_GROUP_TKIP |
                                                                                            CSR_WIFI_SME_ENCRYPTION_CIPHER_GROUP_CCMP;
                        profile.credentials.credential.fast.eapCredentials.authServerUserIdentity = ;
                        profile.credentials.credential.fast.eapCredentials.username = ;
                        profile.credentials.credential.fast.eapCredentials.password = ;
                        profile.credentials.credential.fast.allowPacProvisioning = FALSE;
                        profile.credentials.credential.fast.pacLength = ;
                        profile.credentials.credential.fast.pac = ;
                        profile.credentials.credential.fast.eapCredentials.sessionLength = 0; // Possibly stored from a NME_PROFILE_UPDATE_IND
                        profile.credentials.credential.fast.eapCredentials.session = NULL; // Possibly stored from a NME_PROFILE_UPDATE_IND
*/

                        break;
                    }
                    default:
                    {
                        CsrDebugTraceHigh("Unknown EAP Type: %d");
                        break;
                    }
                }
            }
#ifdef CSR_TRY_TO_GET_PMK_AS_FOR_64HEX_OR_32ASCII
            else if (conn_req_ptr->credential.wpa_credential.credential_info.wpa_psk_info.psk[63] != 0 ||
                strlen(conn_req_ptr->credential.wpa_credential.credential_info.wpa_psk_info.psk) == 32)
            {
                profile.credentials.credentialType = CSR_WIFI_NME_CREDENTIAL_TYPE_WPA_PSK;
                profile.credentials.credential.wpaPsk.encryptionMode = CSR_WIFI_SME_ENCRYPTION_CIPHER_PAIRWISE_TKIP |
                            CSR_WIFI_SME_ENCRYPTION_CIPHER_PAIRWISE_CCMP | CSR_WIFI_SME_ENCRYPTION_CIPHER_GROUP_TKIP | CSR_WIFI_SME_ENCRYPTION_CIPHER_GROUP_CCMP;
                if (strlen(conn_req_ptr->credential.wpa_credential.credential_info.wpa_psk_info.psk) == 32)
                    memcpy (profile.credentials.credential.wpaPsk.psk, conn_req_ptr->credential.wpa_credential.credential_info.wpa_psk_info.psk, 32);
                else
                    CsrConvertHex2Bin(profile.credentials.credential.wpaPsk.psk, 32,   conn_req_ptr->credential.wpa_credential.credential_info.wpa_psk_info.psk, WIFISUPP_WPA_PSK_LEN);
            }
#endif            
            else            
            {
                profile.credentials.credentialType = CSR_WIFI_NME_CREDENTIAL_TYPE_WPA_PASSPHRASE;
                profile.credentials.credential.wpaPassphrase.encryptionMode = CSR_WIFI_SME_ENCRYPTION_CIPHER_PAIRWISE_TKIP |
                            CSR_WIFI_SME_ENCRYPTION_CIPHER_PAIRWISE_CCMP | CSR_WIFI_SME_ENCRYPTION_CIPHER_GROUP_TKIP | CSR_WIFI_SME_ENCRYPTION_CIPHER_GROUP_CCMP;
                /* SR 22183, NME requires string type for the pass-pharase */
                profile.credentials.credential.wpaPassphrase.passphrase = CsrPmemAlloc(WIFISUPP_WPA_PSK_LEN * sizeof(CsrCharString) + 1);
                CsrMemSet (profile.credentials.credential.wpaPassphrase.passphrase, 0, WIFISUPP_WPA_PSK_LEN * sizeof(CsrCharString) + 1);
                CsrMemCpy (profile.credentials.credential.wpaPassphrase.passphrase, conn_req_ptr->credential.wpa_credential.credential_info.wpa_psk_info.psk, WIFISUPP_WPA_PSK_LEN);
            }
            break;
        }
        case WIFISUPP_ENCRYP_PROTOCOL_WPA2:
        {
            // Pre-Shared Key (PSK) or EAP
            if (conn_req_ptr->credential.wpa_credential.credential_type == WIFISUPP_WPA_CREDENTIAL_TYPE_EAP)
            {
                // Not Supported
                securitySupported = FALSE;

                switch (conn_req_ptr->credential.wpa_credential.credential_info.wpa_eap_info.eap_type)
                {
                    case WIFISUPP_WPA_EAP_TYPE_TLS:
                    {
                        profile.credentials.credentialType = CSR_WIFI_NME_CREDENTIAL_TYPE_8021X_TLS;
                        break;
                    }
                    case WIFISUPP_WPA_EAP_TYPE_TTLS:
                    {
                        profile.credentials.credentialType = CSR_WIFI_NME_CREDENTIAL_TYPE_8021X_TTLS;
                        break;
                    }
                    case WIFISUPP_WPA_EAP_TYPE_PEAP:
                    {
                        profile.credentials.credentialType = CSR_WIFI_NME_CREDENTIAL_TYPE_8021X_PEAP_GTC; //Or CSR_WIFI_NME_CREDENTIAL_TYPE_8021X_PEAP_MSCHAPV2??
                        break;
                    }
                    case WIFISUPP_WPA_EAP_TYPE_LEAP:
                    {
                        profile.credentials.credentialType = CSR_WIFI_NME_CREDENTIAL_TYPE_8021X_LEAP;
                        break;
                    }
                    case WIFISUPP_WPA_EAP_TYPE_SIM:
                    {
                        profile.credentials.credentialType = 0; // Not supported until Synergy WiFi 4.0.0
                        break;
                    }
                    case WIFISUPP_WPA_EAP_TYPE_AKA:
                    {
                        profile.credentials.credentialType = 0; // Not supported until Synergy WiFi 4.0.0
                        break;
                    }
                    case WIFISUPP_WPA_EAP_TYPE_FAST:
                    {
                        profile.credentials.credentialType = CSR_WIFI_NME_CREDENTIAL_TYPE_8021X_FAST;
                        break;
                    }
                    default:
                    {
                        CsrDebugTraceHigh("Unknown EAP Type: %d");
                        break;
                    }
                }
            }
#ifdef CSR_TRY_TO_GET_PMK_AS_FOR_64HEX_OR_32ASCII            
            else if (conn_req_ptr->credential.wpa_credential.credential_info.wpa_psk_info.psk[63] != 0 ||
                strlen(conn_req_ptr->credential.wpa_credential.credential_info.wpa_psk_info.psk) == 32)
            {
                profile.credentials.credentialType = CSR_WIFI_NME_CREDENTIAL_TYPE_WPA2PSK;
                profile.credentials.credential.wpa2Psk.encryptionMode = CSR_WIFI_SME_ENCRYPTION_CIPHER_PAIRWISE_TKIP |
                            CSR_WIFI_SME_ENCRYPTION_CIPHER_PAIRWISE_CCMP | CSR_WIFI_SME_ENCRYPTION_CIPHER_GROUP_TKIP | CSR_WIFI_SME_ENCRYPTION_CIPHER_GROUP_CCMP;
                if (strlen(conn_req_ptr->credential.wpa_credential.credential_info.wpa_psk_info.psk) == 32)
                    memcpy (profile.credentials.credential.wpa2Psk.psk, conn_req_ptr->credential.wpa_credential.credential_info.wpa_psk_info.psk, 32);
                else
                    CsrConvertHex2Bin(profile.credentials.credential.wpa2Psk.psk, 32,   conn_req_ptr->credential.wpa_credential.credential_info.wpa_psk_info.psk, WIFISUPP_WPA_PSK_LEN);
            }
#endif            
            else
            {
                profile.credentials.credentialType = CSR_WIFI_NME_CREDENTIAL_TYPE_WPA2PASSPHRASE;
                profile.credentials.credential.wpa2Passphrase.encryptionMode = CSR_WIFI_SME_ENCRYPTION_CIPHER_PAIRWISE_TKIP |
                            CSR_WIFI_SME_ENCRYPTION_CIPHER_PAIRWISE_CCMP | CSR_WIFI_SME_ENCRYPTION_CIPHER_GROUP_TKIP | CSR_WIFI_SME_ENCRYPTION_CIPHER_GROUP_CCMP;
                /* SR 22183, NME requires string type for the pass-pharase */
                profile.credentials.credential.wpa2Passphrase.passphrase = CsrPmemAlloc(WIFISUPP_WPA_PSK_LEN * sizeof(CsrCharString) + 1);
                CsrMemSet (profile.credentials.credential.wpa2Passphrase.passphrase, 0, WIFISUPP_WPA_PSK_LEN * sizeof(CsrCharString) + 1);
                CsrMemCpy (profile.credentials.credential.wpa2Passphrase.passphrase, conn_req_ptr->credential.wpa_credential.credential_info.wpa_psk_info.psk, WIFISUPP_WPA_PSK_LEN);
            }
            break;
        }
        case WIFISUPP_ENCRYP_PROTOCOL_WAPI:
        {
            //securitySupported = FALSE;
			if (conn_req_ptr->credential.wapi_credential.credential_type == WIFISUPP_WAPI_CREDENTIAL_TYPE_PSK)
            {
				profile.credentials.credentialType = CSR_WIFI_NME_CREDENTIAL_TYPE_WAPI_PASSPHRASE;
				profile.credentials.credential.wapiPassphrase.encryptionMode = CSR_WIFI_SME_ENCRYPTION_CIPHER_PAIRWISE_TKIP |
                            CSR_WIFI_SME_ENCRYPTION_CIPHER_PAIRWISE_CCMP | CSR_WIFI_SME_ENCRYPTION_CIPHER_GROUP_TKIP | CSR_WIFI_SME_ENCRYPTION_CIPHER_GROUP_CCMP;
				profile.credentials.credential.wapiPassphrase.passphrase = CsrPmemAlloc(WIFISUPP_WAPI_PSK_LEN * sizeof(CsrCharString) + 1);
                CsrMemSet (profile.credentials.credential.wapiPassphrase.passphrase, 0, WIFISUPP_WAPI_PSK_LEN * sizeof(CsrCharString) + 1);
                CsrMemCpy (profile.credentials.credential.wapiPassphrase.passphrase, conn_req_ptr->credential.wapi_credential.credential_info.wapi_psk_info.psk, WIFISUPP_WAPI_PSK_LEN);
			}
			else if (conn_req_ptr->credential.wapi_credential.credential_type == WIFISUPP_WAPI_CREDENTIAL_TYPE_CER)
			{
				profile.credentials.credentialType = CSR_WIFI_NME_CREDENTIAL_TYPE_WAPI;
                profile.credentials.credential.wapi.caCertificate = CsrMemDup(conn_req_ptr->credential.wapi_credential.credential_info.wapi_cer_info.as_certificate_buf_ptr, conn_req_ptr->credential.wapi_credential.credential_info.wapi_cer_info.as_certificate_buf_len);
				profile.credentials.credential.wapi.caCertificateLength = conn_req_ptr->credential.wapi_credential.credential_info.wapi_cer_info.as_certificate_buf_len;

				profile.credentials.credential.wapi.certificate = CsrMemDup(conn_req_ptr->credential.wapi_credential.credential_info.wapi_cer_info.asue_certificate_buf_ptr, conn_req_ptr->credential.wapi_credential.credential_info.wapi_cer_info.asue_certificate_buf_len);
				profile.credentials.credential.wapi.certificateLength = conn_req_ptr->credential.wapi_credential.credential_info.wapi_cer_info.asue_certificate_buf_len;

                profile.credentials.credential.wapi.privateKeyLength = 0;
				profile.credentials.credential.wapi.privateKey = NULL;
			}
            break;
        }
        case WIFISUPP_ENCRYP_PROTOCOL_OPENSYS:
        default:
        {
            profile.credentials.credentialType = CSR_WIFI_NME_CREDENTIAL_TYPE_OPEN_SYSTEM;
            break;

        }
    }

    CsrDebugTraceLow("Security conf - protocol: %d - wpa-credential_type: %d, credentialType: %d, ", conn_req_ptr->encryp_protocol, conn_req_ptr->credential.wpa_credential.credential_type, profile.credentials.credentialType);

    if (FALSE == securitySupported)
    {
        WIFISUPP_CONNECT_RESULT_T resultData;

        resultData.result = WIFISUPP_RESULT_RESTRICTED;
        CsrMemCpy(&(resultData.ssid), &(conn_req_ptr->ssid), sizeof(WIFISUPP_SSID_T));
        CsrMemCpy(resultData.bssid_info.bssid, conn_req_ptr->bssid_info.bssid, WIFISUPP_BSSID_LEN);

        SUPPMAIN_NotifyApp(WIFISUPP_SIG_CONNECT_CNF, &resultData);

        return FALSE;
    }

    CsrWifiNmeProfileSetReqSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE, profile);

    return TRUE;
}

/****************************************************************************/
//Description : disconnect
//Global resource dependence :
//Author: juan.zhang
//Note:
/****************************************************************************/
LOCAL BOOLEAN CsrWifiSuppDisconnect(WIFISUPP_DISCONNECT_REQ_INFO_T *disconn_req_ptr)
{
    CsrDebugTraceLow("CSR_WIFI_SUPP_DISCONNECT - ssid_len: %d, ssid: %.*s",
                     disconn_req_ptr->ssid.ssid_len,
                     disconn_req_ptr->ssid.ssid_len,
                     disconn_req_ptr->ssid.ssid);

    // Copy info needed for disconnect_cnf
    instanceData.disconnectReq = CsrPmemAlloc(sizeof(WIFISUPP_DISCONNECT_RESULT_T));
    CsrMemCpy(&(instanceData.disconnectReq->ssid), &(disconn_req_ptr->ssid), sizeof(WIFISUPP_SSID_T));
    instanceData.disconnectReq->result = 0;

#ifdef CSR_WIFI_NME_AUTOCONNECT
    CsrDebugTraceLow("CSR_WIFI_SUPP_DISCONNECT - calling NmeProfileOrderSet");
    CsrWifiNmeProfileOrderSetReqSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE, 0, NULL);
#else
    CsrDebugTraceLow("CSR_WIFI_SUPP_DISCONNECT - calling SmeDisconnectReq");
    CsrWifiSmeDisconnectReqSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE);
#endif
    return TRUE;
}

/****************************************************************************/
//Description : to handle the command message
//Global resource dependence :
//Author: 
//Note:
/****************************************************************************/
LOCAL BOOLEAN CsrWifiSuppCommand(WIFISUPP_COMMAND_INFO_T *command_info_ptr)
{
    if (!wifi_supp_is_on)
        return FALSE;
    CsrDebugTraceLow("CSR_WIFI_SUPP_COMMAND - command_id: %d, para1: %d",
        command_info_ptr->command_id,
        command_info_ptr->para1);
    // Copy info needed for command
    CsrMemCpy(&(instanceData.command_info_ptr->command_id), &(command_info_ptr->command_id), sizeof(WIFISUPP_WIFIDRV_COMMAND_TYPE_E));
    // Workaround MMI send commands with the same pars at once
    // CsrMemCpy(&(instanceData.command_info_ptr->para1), &(command_info_ptr->para1), sizeof(uint32));
    switch (command_info_ptr->command_id)
    {
        case WIFIDRV_COMMAND_POWER_SAVE:
            // Workaround MMI send commands with the same pars at once
            CsrMemCpy(&(instanceData.command_info_ptr->para1), &(command_info_ptr->para1), sizeof(uint32));
            CsrWifiSmePowerConfigGetReqSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE);
            break;
        case WIFIDRV_COMMAND_SET_ROAMING_MODE:
            // Workaround MMI send commands with the same pars at once
            CsrMemCpy(&(instanceData.command_info_ptr->para2), &(command_info_ptr->para1), sizeof(uint32));
            CsrWifiSmeRoamingConfigGetReqSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE);
            break;
        default:
            return FALSE;
    }
    return TRUE;
}


void CsrWifiOnVarsInit(void)
{
    instanceData.command_info_ptr = CsrPmemAlloc(sizeof(WIFISUPP_COMMAND_INFO_T));
    wifi_supp_is_on = 1;
}

void CsrWifiOffVarsDeInit(void)
{
    wifi_supp_is_on = 0;
    CsrPmemFree(instanceData.command_info_ptr);
    instanceData.command_info_ptr = NULL;
}

#ifdef CSR_WIFI_ANRITSU_TEST_MODE
void CsrWifiAnritsuRateMatch(void)
{
    CsrDebugTraceLow("CsrWifiAnritsuRateMatch");
    CsrWifiSmeMibConfigGetReqSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE);
}
#endif


/****************************************************************************/
//Description : to handle the customarized message
//Global resource dependence :
//Author: juan.zhang
//Note:
/****************************************************************************/
LOCAL void CsrWifiHandleCusmsg(uint32 msg_id, void *msg_body_ptr)
{

}

/****************************************************************************/
//Description : get the current connected AP's information
//Global resource dependence :
//Author: juan.zhang
//Note:
/****************************************************************************/
LOCAL BOOLEAN CsrWifiSuppGetConnectionInfo(WIFISUPP_SCAN_AP_INFO_T *connection_info_ptr)
{
    BOOLEAN result = TRUE;

    CsrDebugTraceLow("CSR_WIFI_SUPP_GET_CONNECTION_INFO");

    if (instanceData.currentConnectionInfo != NULL &&
        instanceData.currentConnectionQuality != NULL)
    {
        // Copy info from currentConnectionInfo first
        CsrMemCpy(&connection_info_ptr->ssid, &instanceData.currentConnectionInfo->ssid, sizeof(CsrWifiSsid));
        CsrMemCpy(connection_info_ptr->bssid_info.bssid, instanceData.currentConnectionInfo->bssid.a, WIFISUPP_BSSID_LEN);
        connection_info_ptr->beacon_interval= instanceData.currentConnectionInfo->assocReqListenIntervalBeacons;
        connection_info_ptr->channel = instanceData.currentConnectionInfo->channelNumber;
        connection_info_ptr->rate = getRate(instanceData.currentConnectionInfo->assocRspInfoElementsLength, instanceData.currentConnectionInfo->assocRspInfoElements);

        // Next, copy currentConnectionQuality
        connection_info_ptr->signal_qua = instanceData.currentConnectionQuality->unifiRssi;
        connection_info_ptr->noise = instanceData.currentConnectionQuality->unifiSnr;

        // Hardcode to infrastructure as there is no easy way of knowing - could get connected to another AP using auto-connect
        // and that does not tell us the mode
        connection_info_ptr->network_mode = WIFISUPP_NETWORK_MODE_INFRASTRUCTURE;

        if ((instanceData.currentConnectionInfo->assocRspCapabilityInfo & 0x0010) == 0)
        {
            connection_info_ptr->encryp_protocol = WIFISUPP_ENCRYP_PROTOCOL_OPENSYS;
        }
        else
        {
            switch (instanceData.currentConnectionInfo->authMode)
            {
                case CSR_WIFI_SME_AUTH_MODE_80211_OPEN:
                case CSR_WIFI_SME_AUTH_MODE_80211_SHARED:
                {
                    connection_info_ptr->encryp_protocol = WIFISUPP_ENCRYP_PROTOCOL_WEP;
                    break;
                }
                case CSR_WIFI_SME_AUTH_MODE_8021X_WPA:
                case CSR_WIFI_SME_AUTH_MODE_8021X_WPAPSK:
                {
                    connection_info_ptr->encryp_protocol = WIFISUPP_ENCRYP_PROTOCOL_WPA;
                    break;
                }
                case CSR_WIFI_SME_AUTH_MODE_8021X_WPA2:
                case CSR_WIFI_SME_AUTH_MODE_8021X_WPA2PSK:
                {
                    connection_info_ptr->encryp_protocol = WIFISUPP_ENCRYP_PROTOCOL_WPA2;
                    break;
                }
                default:
                    break;
            }
        }
    }
    else
    {
        connection_info_ptr = NULL;
        result = FALSE;
    }

    return result; /*lint !e438*/
}

#ifdef CSR_TRY_TO_GET_PMK_AS_FOR_64HEX_OR_32ASCII
LOCAL void CsrConvertHex2Bin (CsrUint8* dest_bin, int dest_bin_size, CsrUint8 *src_hex, int src_hex_len)
{
    CsrUint8 u8;
    int i, i_max;    
    char *p;

    i = 0;
    i_max = MIN(dest_bin_size, src_hex_len/2);
    p = (char *)src_hex;
    for (i=0; i<i_max; i++, p+=2) {
        sscanf(p, "%02x", &u8);
        dest_bin[i] = u8;
     }
}
#endif

#if !defined(BLUETOOTH_SUPPORT)
void *CsrBtCmSetEventMaskReq_struct (CsrUint32 a, CsrUint32 b, CsrUint32 c) {} /*lint !e533*/

void *CsrBtCmSetAfhChannelClassReq_struct(CsrUint32 a, CsrUint8 *b) {} /*lint !e533*/

void OS_Msg_Transport(CsrUint16 a, CsrUint16 b, void* c) {}
#endif
