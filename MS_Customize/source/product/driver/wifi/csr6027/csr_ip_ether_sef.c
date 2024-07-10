/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

****************************************************************************/

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_sched.h"
#include "csr_framework_ext.h"

#include "csr_ip_handler.h"
#include "csr_ip_ether_prim.h"
#include "csr_ip_ether_lib.h"

#include "sci_types.h"
#include "tcpip_api.h"
#include "sci_api.h"
#include "threadx_os.h"

#include "csr_gpio_debug.h"
   
/* Synergy Platform Tools: Start */
#include "csr_spt_wifi_eth_raw.h"
/* Synergy Platform Tools: End */

/*lint -e838*/
/*lint -e551*/

#undef NOISY
#define CSR_WIFI_WORKAROUND_RETRY_DHCP_REQ
#ifdef CSR_WIFI_WORKAROUND_RETRY_DHCP_REQ
static void CsrIpMonitor(TCPIP_PACKET_INFO_T *p_pkt_info, int direction_is_tx);
static void CsrIpMonitorExpired(CsrUint16, void *);
static void CsrUpdateSchedDhcpTimer(CsrUint32);
static CsrSchedTid DhcpRetryTimerId = 0;

#define DHCP_MAXIMUM_TIMEOUT_MS 1000

#define ETH_HDR_SIZE                (12 + 2)
#define IP_HDR_SIZE                 (20)
#define UDP_HDR_SIZE                (8)

#define ETH_HDR_TYPE_IS_IP(p)       ((p)[12] == 0x08 && (p)[13] == 0x00)
#define IP_HDR_PROTO_IS_UDP(p)      ((p)[9] == 17)
#define UDP_HDR_PORT_IS_DHCP(p)     ((p)[3] == 67 || (p)[3] == 68)
#endif

extern void CsrDebugTraceLow(const char* format, ...);
extern void CsrDebugTraceHigh(const char* format, ...);

/* PORTING: Set to the desired encapsulation format for both tx and rx. Can be
   any of the CSR_IP_ETHER_ENCAPSULATION_* defines in csr_ip_ether_prim.h. This
   will override the encapsulation suggested by the ethernet interface. */
#define ENCAPSULATION CSR_IP_ETHER_ENCAPSULATION_ETHERNET
#define ETHERNET_ENCAPSULATION_HEADER_LEN     12
#define TCPIP_STACK_MAX_TX_FRAME_SIZE         1514

/* Mutex for synchronising between the threads */
static CsrMutexHandle mutex;

static BOOLEAN rxFlow = FALSE;
static CsrUint32 rxFlowWaiting = 0;
static CsrEventHandle rxFlowEvent;

static CsrUint32 flowMask;
/* Static local storage of the information received in the CsrIpEtherIfAddReq
   message. */
/* static CsrUint16 ifType; */
/* static CsrUint8 mac[6]; */
/* static CsrUint8 encapsulation; */
/* static CsrUint16 maxTxUnit; */
TCPIP_NETID_T *csrGlobalIpEtherContext; /* Context that must be passed to Spreadtrum TCP/IP stack */
static CsrIpEtherContext context; /* Context that must be passed to frameTxFunction */
static CsrIpEtherFrameTxFunction frameTxFunction; /* Function for transmitting frames */

/* State of the link between the IP stack and the ethernet interface */
#define STATE_UNAVAILABLE      0 /* Ethernet interface is unavailable */
#define STATE_LINK_DOWN        1 /* Ethernet interface is available, but link is down */
#define STATE_LINK_UP_OPEN     2 /* Link is up and frames can be transmitted */
#define STATE_LINK_UP_CLOSED   3 /* Link is up, but no frames can be transmitted */
static CsrUint8 linkState = STATE_UNAVAILABLE;

#ifdef CSR_WIFI_ANRITSU_TEST_MODE
extern CsrBool csrWifiAnritsuTestMode;
extern CsrUint32 csrWifiAnritsuCurrentRate;
extern void CsrWifiAnritsuRateMatch(void);
#endif

/*******************************************************************************

    NAME
        CsrIpEtherDeinit

    DESCRIPTION
        This function is called exactly once before the first message is 
        received.

*******************************************************************************/
void CsrIpEtherInit(void)
{
    CsrMutexCreate(&mutex);

    csrGlobalIpEtherContext = NULL;
    context = NULL;

#ifdef CSR_WIFI_WORKAROUND_RETRY_DHCP_REQ
	CsrUpdateSchedDhcpTimer(DHCP_MAXIMUM_TIMEOUT_MS);
#endif	
}


/*******************************************************************************

    NAME
        CsrIpEtherDeinit

    DESCRIPTION
        This function is called exactly once after the final message has been 
        received prior to the scheduler shutting down.

*******************************************************************************/
#ifdef ENABLE_SHUTDOWN
void CsrIpEtherDeinit(void)
{
    CsrMutexDestroy(&mutex);
#ifdef CSR_WIFI_WORKAROUND_RETRY_DHCP_REQ
	CsrSchedTimerCancel(DhcpRetryTimerId, 0, NULL);
#endif
}
#endif


#define MAC_ADDRESS_COPY(dst, src) do{ (dst)[0] = (src)[0]; (dst)[1] = (src)[1]; (dst)[2] = (src)[2]; (dst)[3] = (src)[3]; (dst)[4] = (src)[4]; (dst)[5] = (src)[5]; }while(0)

/*******************************************************************************

    NAME
        frameRxFunction

    DESCRIPTION
        This function is called by the ethernet interface to deliver a received
        ethernet frame to the IP stack.

    PARAMETERS
        destinationMac - Pointer to first byte of destination MAC address.
        sourceMac - Pointer to first byte of source MAC address.
        rssi - Radio Signal Strength Indication.
        frameLength - length in byte of frame pointer.
        frame - Pointer to first byte of frame (not including MAC addresses).
        context - Pointer to context passed in the CSR_IP_ETHER_IF_ADD_CFM. This
                  can be set to anything that is useful for this module.

*******************************************************************************/
static void frameRxFunction(CsrUint8 *destinationMac, CsrUint8 *sourceMac, CsrInt16 rssi, CsrUint16 frameLength, CsrUint8 *frame, CsrIpEtherContext ctx, CsrUint32 rate)
{
    /* PORTING: Add code to pass the received ethernet frame to the IP stack
       here. The frame pointer points at the first byte of the frame following
       the destination and source MAC addresses. This function must copy the
       contents before returning. The memory ownership remains with the caller,
       so no attempts must be made to free the memory of any of the arguments. */
    TCPIP_ERROR_E result = TCPIP_ERROR_OK;
    TCPIP_PACKET_INFO_T packet;
    CsrUint32 bits;

    CSR_GPIO_DEBUG_SET(0, CSR_GPIO_IP_ETHER_FRAME_RX);
    
    packet.net_id = *csrGlobalIpEtherContext;
    packet.data_len = frameLength + ETHERNET_ENCAPSULATION_HEADER_LEN;
    
    /* CSR: *frame is allocated by CsrMemAllocDma(), which adds space in front 
       of the buffer for a header, so we can use the buffer directly */
    packet.data_ptr = frame - ETHERNET_ENCAPSULATION_HEADER_LEN;

	SCI_DisableIRQ();
    if (rxFlow)
    {
        rxFlowWaiting++;
    	SCI_RestoreIRQ();
        CsrEventWait(&rxFlowEvent, CSR_EVENT_WAIT_INFINITE, &bits);
    }
    else
    {
    	SCI_RestoreIRQ();
    }

    /* Pack together data in data_ptr */
    MAC_ADDRESS_COPY(packet.data_ptr, destinationMac);
    MAC_ADDRESS_COPY(packet.data_ptr + 6, sourceMac);
#ifdef NOISY
    CsrDebugTraceLow("UNIFI: frameRxFunction - sending data to IP stack - rssi: %d - len: %d", rssi, packet.data_len);
#endif
#ifdef CSR_WIFI_WORKAROUND_RETRY_DHCP_REQ
    // CSR: Move condition outside call to avoid extra function call in data path
    if (ETH_HDR_TYPE_IS_IP(packet.data_ptr) && IP_HDR_PROTO_IS_UDP(packet.data_ptr + ETH_HDR_SIZE) && UDP_HDR_PORT_IS_DHCP(packet.data_ptr + ETH_HDR_SIZE + IP_HDR_SIZE))
    {
	    //CsrDebugTraceLow("~~~~ frameRxFunction()");
        CsrIpMonitor(&packet, 0);

        // CSR: Test function to start TX test after link is up
        //CsrSptWifiEthRawTxStart();
    }
#endif
    CSR_GPIO_DEBUG_SET(0, CSR_GPIO_IP_ETHER_TCPIP_RX_IND);
    result = TCPIP_RxInd(&packet);
    CSR_GPIO_DEBUG_SET(1, CSR_GPIO_IP_ETHER_TCPIP_RX_IND);

    switch (result)
    {
        case TCPIP_ERROR_OK:          /* no error */
        {
            // Success, do nothing
            break;
        }
        case TCPIP_ERROR_INVALPARAM:  /* invalid parameter */
        case TCPIP_ERROR_INVALNETID:  /* invalid net id */
        case TCPIP_ERROR_MEMALLOC:    /* memory alloc fail */
        case TCPIP_ERROR_LOGICAL:     /* calling or running logical error */
        case TCPIP_ERROR_TIMEOUT:     /* time out */
        {
            // Rx Flow control not yet possible - TBD and implemented
            CsrDebugTraceHigh("UNIFI: frameRxFunction - NOT SUCCESS - result: %d", result);
        }
        break;
        default:
            CsrDebugTraceLow("frameRxFunction switch entery default, error");
        break;
    }

    CSR_GPIO_DEBUG_SET(1, CSR_GPIO_IP_ETHER_FRAME_RX);

#ifdef CSR_WIFI_ANRITSU_TEST_MODE
    if (csrWifiAnritsuTestMode)
    {
        //CsrDebugTraceLow("UNIFI: frameRxFunction - rate: %ld, csrWifiAnritsuCurrentRate: %ld", rate, csrWifiAnritsuCurrentRate);
        if(rate != csrWifiAnritsuCurrentRate)
        {
            csrWifiAnritsuCurrentRate = rate;
            CsrWifiAnritsuRateMatch();
        }
    }
#endif  
}

/*******************************************************************************

    NAME
        txFlowControlResumeFunction

    DESCRIPTION
        This function is called by the ethernet interface to indicate that the 
        link is open for transmission of frames again. This will only be called 
        after the frameTxFunction returned either 
        CSR_IP_ETHER_RESULT_PAUSE_NOT_SENT or CSR_IP_ETHER_RESULT_PAUSE_SENT.

    PARAMETERS
        context - Pointer to context passed in the CSR_IP_ETHER_IF_ADD_CFM. This
                  can be set to anything that is useful for this module.

*******************************************************************************/
static void txFlowControlResumeFunction(CsrIpEtherContext ctx)
{
#if 0
    TCPIP_NETID_T *net_id = (TCPIP_NETID_T *)ctx;
    CsrMutexLock(&mutex);

    CsrDebugTraceLow("UNIFI: txFlowControlResumeFunction called - linkState: %d", linkState);

    switch (linkState)
    {
        case STATE_UNAVAILABLE:
            break;
        case STATE_LINK_DOWN:
            break;
        case STATE_LINK_UP_CLOSED:
            linkState = STATE_LINK_UP_OPEN;
            /* PORTING: Dequeue and transmit any frames that were queued, and/or
               notify the IP stack that it is possible to transmit frames again,
               causing it to retransmit any frames it may have enqueued. */
            CsrDebugTraceLow("UNIFI: txFlowControlResumeFunction: calling Spreadtrum TCP/IP stack to inform that flow control is disabled - net_id: 0x%08X", *net_id);
            TCPIP_SetTxFlowCtrl(FALSE, *net_id);
            break;
        case STATE_LINK_UP_OPEN:
            break;
        default:
            break;

    }

    CsrMutexUnlock(&mutex);
#endif
}

void rxFlowControlFunction(
    BOOLEAN         is_set,    /* flow control flag: TRUE - set; FALSE - not set */
    TCPIP_NETID_T   net_id )   /* net interface ID */
{
#ifdef NOISY
    CsrDebugTraceLow("UNIFI: rxFlowControlFunction: Spreadtrum TCP/IP stack informs that flow control for net_id: 0x%08X is **%s**", net_id, is_set ? "enabled" : "disabled");
#endif
	SCI_DisableIRQ();
    rxFlow = is_set;
    if (!rxFlow && rxFlowWaiting)
    {
        rxFlowWaiting--;
    	SCI_RestoreIRQ();
        CsrEventSet(&rxFlowEvent, 1);
    }
    else
    {
    	SCI_RestoreIRQ();
    }
}

void CsrIpEtherIfFlowControlPause(CsrUint32 ifHandle, CsrIpEtherPriority priority)
{
	 TCPIP_NETID_T *net_id = csrGlobalIpEtherContext;

#ifdef NOISY
	CsrDebugTraceLow("UNIFI: CsrIpEtherIfFlowControlPause called - linkState: %d flowMask: %d pri: %d", linkState, flowMask, priority);
#endif

	SCI_DisableIRQ();
	flowMask |= priority;
    if (flowMask)
    {
    	SCI_RestoreIRQ();
        TCPIP_SetTxFlowCtrl( TRUE, *net_id);
        CsrSptWifiEthRawFlowPause();
    }
    else
    {
    	SCI_RestoreIRQ();
    }
}
void CsrIpEtherIfFlowControlResume(CsrUint32 ifHandle, CsrIpEtherPriority priority)
{
	TCPIP_NETID_T *net_id = csrGlobalIpEtherContext;
    
#ifdef NOISY
	CsrDebugTraceLow("UNIFI: CsrIpEtherIfFlowControlResume called - linkState: %d flowMask: %d pri: %d", linkState, flowMask, priority);
#endif

	SCI_DisableIRQ();
	flowMask &= ~priority;
	if (!flowMask)
    {
    	SCI_RestoreIRQ();
        TCPIP_SetTxFlowCtrl( FALSE, *net_id);
        CsrSptWifiEthRawFlowResume();
    }
    else
    {
    	SCI_RestoreIRQ();
    }
}

/*******************************************************************************

    NAME
        CsrPacketTransmit

    DESCRIPTION
        PORTING: This function can be called by the IP stack to deliver an 
        ethernet frame to the interface for transmission. The parameter list and 
        return value can be adjusted, along with the implementation, to anything 
        that is suitable for the particular IP stack.

    PARAMETERS
        TCPIP_PACKET_INFO_T - Pointer 

*******************************************************************************/
CsrInt32 CsrPacketTransmit(TCPIP_PACKET_INFO_T *packet)
{
    CsrResult result = CSR_RESULT_SUCCESS;

    CsrMutexLock(&mutex);
#ifdef NOISY    
    CsrDebugTraceLow("UNIFI: CsrPacketTransmit - TCP/IP stack sending data - linkState: %d, net_id: 0x%08X - data_len: %d - context: 0x%08X!", linkState, packet->net_id, packet->data_len, context);
    CsrDebugTraceLow("UNIFI: CsrPacketTransmit - tx_func: 0x%08X", CsrPacketTransmit);
#endif    

#if 0
    {
        unsigned int stackPtr;
        extern BLOCK_ID to_nettick;
        SCI_THREAD_PTR threadPtr;

        if (SCI_IdentifyThread() == to_nettick)
        {
            __asm
            {
                mov stackPtr, r13;
            }

            //SCI_TRACE_LOW("stackPtr: 0x%08X", stackPtr);

            threadPtr = SCI_GetThreadBlockFromId(SCI_IdentifyThread());

            if (threadPtr != NULL)
            {
                CsrUint32 *ptr = (CsrUint32 *) threadPtr;
                SCI_TRACE_LOW("STACK: current stack ptr: 0x%08X | stack_size = %d",
                              stackPtr, (*(ptr + 6) - stackPtr));
            }
        }
    }
#endif

    switch (linkState)
    {
        case STATE_UNAVAILABLE:
            /* The interface is not yet available. */
            break;
        case STATE_LINK_DOWN:
            /* The link is not up. */
            break;
        case STATE_LINK_UP_CLOSED:
            /* PORTING: The link is closed due to flow control. The frame must
               either be enqueued or dropped here or the return value from
               this function must cause the IP stack to enqueue (or drop) it. */
            break;
        case STATE_LINK_UP_OPEN:
        {
            /* Spreadtrum platform uses ETHERNET frame encapsulation */
            CsrUint16 frameLength = 0;

			/*Make sure the packet length is not 0, if it is 0, 0-ETHERNET_ENCAPSULATION_HEADER_LEN will be equal to 65524. DMA malloc will fail. */
			if (packet->data_len == 0)
				break;
			
			frameLength = packet->data_len - ETHERNET_ENCAPSULATION_HEADER_LEN;

			/*Make sure the frame length will be less than DMA BLOCK length*/
			if (frameLength > CSR_MEM_DMA_BLOCK_SIZE)
				break;

			if(context == NULL)
				break;
			
            /* Transmit the frame. The callee will copy the contents, and memory
               ownership remains with the caller (here), so it is the
               responsibility of this module to free the memory, if and only if
               necessary. */
            // CSR: Do not look at the result value, this is not reliable, instead use the fow control callbacks
            frameTxFunction(&packet->data_ptr[0],
                                     &packet->data_ptr[6],
                                     frameLength,
                                     &packet->data_ptr[ETHERNET_ENCAPSULATION_HEADER_LEN],
                                     context);

            // Do not free packet, memory owned by Spreadtrum TCP/IP stack
            break;
        }
        default:
            break;

    }

    CsrMutexUnlock(&mutex);

    return (CsrInt32)result;
}


CsrInt32 CsrPacketTransmitWrapper(TCPIP_PACKET_INFO_T *packet)
{
#ifdef CSR_WIFI_WORKAROUND_RETRY_DHCP_REQ
    // CSR: Move condition outside call to avoid extra function call in data path
    if (ETH_HDR_TYPE_IS_IP(packet->data_ptr) && IP_HDR_PROTO_IS_UDP(packet->data_ptr + ETH_HDR_SIZE) && UDP_HDR_PORT_IS_DHCP(packet->data_ptr + ETH_HDR_SIZE + IP_HDR_SIZE))
    {
        CsrIpMonitor(packet, 1/*TX*/);
    }
#endif               
    return CsrPacketTransmit(packet);
}

/*******************************************************************************

    NAME
        CsrIpEtherIfAddReqHandler

    DESCRIPTION
        This function is called when a CSR_IP_ETHER_IF_ADD_REQ message is 
        received. This message is received when the interface becomes available.

    PARAMETERS
        instanceData - Task instance data.

*******************************************************************************/
void CsrIpEtherIfAddReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpEtherIfAddReq *request = (CsrIpEtherIfAddReq *) instanceData->msg;

    CsrMutexLock(&mutex);

    CsrDebugTraceLow("UNIFI: CsrIpEtherIfAddReqHandler - linkState: %d", linkState);

    switch (linkState)
    {
        case STATE_UNAVAILABLE:
        {
            TCPIP_NETIF_IPADDR_T saddr;
            static CsrUint8 mac[6];
            TCPIP_NETIF_CFG_T cfg_ptr;

            /* ifType = request->ifType; */ /* Type of ethernet interface */
            CsrMemCpy(mac, request->mac, 6); /* MAC address of the ethernet interface */
            /* encapsulation = request->encapsulation; */ /* Encapsulation suggested by ethernet interface */
/* Synergy Platform Tools: Start */
#if (CSR_SPT_CONFIG_ENABLE_SPT == TRUE) && (CSR_SPT_CONFIG_ENABLE_ETH_RAW == TRUE)
            CsrSptWifiEthRawRegister(request->frameTxFunction, request->context, request->mac, request->maxTxUnit, frameRxFunction);
            frameTxFunction = CsrSptWifiEthRawTx;
#else
            frameTxFunction = request->frameTxFunction;
#endif
/* Synergy Platform Tools: End */
            csrGlobalIpEtherContext = CsrPmemAlloc(sizeof(CsrUint32));
            context = request->context;
            linkState = STATE_LINK_DOWN;

            CsrMemSet(&saddr, 0, 5 * sizeof(CsrUint32));
            
            /* PORTING: The IP stack should be notified that the interface is
               available. The above information can be passed into the IP
               stack if required. Note that the suggested encapsulation is
               ignored and the define ENCAPSULATION is used for forcing the
               ethernet interface to use the enapsulation format that suits
               the IP stack. The final argument in the confirm message is a
               void pointer that can be set to anything. The void pointer
               will then be passed to the frameRxFunction above, when a
               frame is received. */
            cfg_ptr.pkt_type = TCPIP_PKTTYPE_ETHER;
            cfg_ptr.tx_req_fptr = CsrPacketTransmitWrapper; /*lint !e158 !e605*/
            cfg_ptr.is_async_tx = FALSE;
            cfg_ptr.saddr = saddr;
            cfg_ptr.haddr.addr_ptr = mac;
            cfg_ptr.haddr.addr_len = 6;
            cfg_ptr.mtu = CSRMIN(request->maxTxUnit, TCPIP_STACK_MAX_TX_FRAME_SIZE); /* The maximum transmission unit */
            cfg_ptr.tx_rate = 54000000; // 54 MB/s for now, can change if necessary
            cfg_ptr.rx_flctrl_fptr = rxFlowControlFunction;  // Function called when Spreadtrum TCP/IP stack can/cannot handle more data (rx flow control)
            cfg_ptr.netif_name_ptr = "CSR_WF"; /*lint !e1778*/

            CsrEventCreate(&rxFlowEvent);
            // Notify router of Spreadtrum RX function
            *csrGlobalIpEtherContext =  TCPIP_RegNetInterface(&cfg_ptr);

            CsrDebugTraceLow("UNIFI: CsrIpEtherIfAddReqHandler: have registered interface with Spreadtrum TCP/IP stack - net_id: 0%08X", *csrGlobalIpEtherContext);

            if (*((CsrUint32 *) csrGlobalIpEtherContext) == TCPIP_NETID_NULL)
            {
                CsrDebugTraceHigh("UNIFI: CsrIpEtherIfAddReqHandler: Failed to register with Spreadtrum TCP/IP stack");

                CsrIpEtherIfAddCfmSend(request->appHandle,
                                       0/*ifHandle*/,
                                       CSR_RESULT_FAILURE,
                                       ENCAPSULATION,
                                       NULL,
                                       NULL,
                                       0);
                CsrPmemFree(csrGlobalIpEtherContext);
                csrGlobalIpEtherContext = NULL;
            }
            else
            {
/* Synergy Platform Tools: Start */
#if (CSR_SPT_CONFIG_ENABLE_SPT == TRUE) && (CSR_SPT_CONFIG_ENABLE_ETH_RAW == TRUE)
                CsrIpEtherIfAddCfmSend(request->appHandle,
                                       0/*ifHandle*/,
                                       CSR_RESULT_SUCCESS,
                                       ENCAPSULATION,
                                       CsrSptWifiEthRawRx,
                                       txFlowControlResumeFunction,
                                       csrGlobalIpEtherContext); /*lint !e64*/
#else
                CsrIpEtherIfAddCfmSend(request->appHandle,
                                       0/*ifHandle*/,
                                       CSR_RESULT_SUCCESS,
                                       ENCAPSULATION,
                                       frameRxFunction,
                                       txFlowControlResumeFunction,
                                       csrGlobalIpEtherContext); /*lint !e64*/
#endif
/* Synergy Platform Tools: End */
            }
            break;
        }
        case STATE_LINK_DOWN:
            /* Fallthrough */
        case STATE_LINK_UP_CLOSED:
            /* Fallthrough */
        case STATE_LINK_UP_OPEN:
            /* Fallthrough */
        default:
            CsrIpEtherIfAddCfmSend(request->appHandle, 0, CSR_IP_ETHER_RESULT_NO_MORE_INTERFACES, ENCAPSULATION, NULL, NULL, NULL);
            break;
    }

    CsrMutexUnlock(&mutex);
}


/*******************************************************************************

    NAME
        CsrIpEtherIfRemoveReqHandler

    DESCRIPTION
        This function is called when a CSR_IP_ETHER_IF_REMOVE_REQ message is 
        received. This message is received if the interface becomes unavailable.

    PARAMETERS
        instanceData - Task instance data.

*******************************************************************************/
void CsrIpEtherIfRemoveReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpEtherIfRemoveReq *request = (CsrIpEtherIfRemoveReq *) instanceData->msg;

    CsrMutexLock(&mutex);

    switch (linkState)
    {
        case STATE_UNAVAILABLE:
            CsrIpEtherIfRemoveCfmSend(request->appHandle, request->ifHandle, CSR_IP_ETHER_RESULT_INVALID_HANDLE);
            break;
        case STATE_LINK_DOWN:
            /* Fallthrough */
        case STATE_LINK_UP_CLOSED:
            /* Fallthrough */
        case STATE_LINK_UP_OPEN:
            /* Fallthrough */
        default:
            frameTxFunction = NULL;
            context = NULL;
            linkState = STATE_UNAVAILABLE;
            /* PORTING: The IP stack should be notified that the interface is no
               longer available. */
            CsrDebugTraceLow("UNIFI: CsrIpEtherIfRemoveReqHandler: calling Spreadtrum TCP/IP stack to deregister interface - netId: %d", *csrGlobalIpEtherContext);

            TCPIP_DeregNetInterface(*csrGlobalIpEtherContext);

            CsrPmemFree(csrGlobalIpEtherContext);
            csrGlobalIpEtherContext = NULL;

            CsrEventDestroy(&rxFlowEvent);

            CsrIpEtherIfRemoveCfmSend(request->appHandle, request->ifHandle, CSR_RESULT_SUCCESS);
            break;
    }

    CsrMutexUnlock(&mutex);
}


/*******************************************************************************

    NAME
        CsrIpEtherIfLinkUpReqHandler

    DESCRIPTION
        This function is called when a CSR_IP_ETHER_IF_LINK_UP_REQ message is 
        received. This message is received when the media is connected on the 
        interface, indicating that it is ready to transmit/receive ethernet 
        frames.

    PARAMETERS
        instanceData - Task instance data.

*******************************************************************************/
void CsrIpEtherIfLinkUpReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpEtherIfLinkUpReq *request = (CsrIpEtherIfLinkUpReq *) instanceData->msg;

    CsrMutexLock(&mutex);
	flowMask = 0;

    CsrDebugTraceLow("UNIFI: CsrIpEtherIfLinkUpReqHandler - linkState: %d", linkState);

    switch (linkState)
    {
        case STATE_UNAVAILABLE:
            CsrIpEtherIfLinkUpCfmSend(request->appHandle, request->ifHandle, CSR_IP_ETHER_RESULT_INVALID_HANDLE);
            break;
        case STATE_LINK_DOWN:
            /* Fallthrough */
        case STATE_LINK_UP_CLOSED:
            /* Fallthrough */
        case STATE_LINK_UP_OPEN:
            /* Fallthrough */
        default:
            linkState = STATE_LINK_UP_OPEN;
            /* PORTING: The IP stack should be notified that the link is now
               up and open, and frame transmission can begin. */
            CsrIpEtherIfLinkUpCfmSend(request->appHandle, request->ifHandle, CSR_RESULT_SUCCESS);

            break;
    }

    CsrMutexUnlock(&mutex);
}


/*******************************************************************************

    NAME
        CsrIpEtherIfLinkDownReqHandler

    DESCRIPTION
        This function is called when a CSR_IP_ETHER_IF_LINK_DOWN_REQ message is
        received. This message is received when the media is disconnected on the
        interface, indicating that it is not possible to transmit/receive
        ethernet frames.

    PARAMETERS
        instanceData - Task instance data.

*******************************************************************************/
void CsrIpEtherIfLinkDownReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpEtherIfLinkDownReq *request = (CsrIpEtherIfLinkDownReq *) instanceData->msg;

    CsrDebugTraceLow("UNIFI: CsrIpEtherIfLinkDownReqHandler - linkState: %d", linkState);

    CsrMutexLock(&mutex);

    switch (linkState)
    {
        case STATE_UNAVAILABLE:
            CsrIpEtherIfLinkDownCfmSend(request->appHandle, request->ifHandle, CSR_IP_ETHER_RESULT_INVALID_HANDLE);
            break;
        case STATE_LINK_DOWN:
            /* Fallthrough */
        case STATE_LINK_UP_CLOSED:
            /* Fallthrough */
        case STATE_LINK_UP_OPEN:
            /* Fallthrough */
        default:
            linkState = STATE_LINK_DOWN;
            /* PORTING: The IP stack should be notified that the link is now
               down, and frame transmission should stop. */

            /* Spreadtrum platform does not have "up/down" concept, so do nothing here */
            CsrIpEtherIfLinkDownCfmSend(request->appHandle, request->ifHandle, CSR_RESULT_SUCCESS);
            break;
    }

    CsrMutexUnlock(&mutex);
}


/*******************************************************************************

    NAME
        CsrIpEtherIfMulticastAddrAddResHandler

    DESCRIPTION
        This function is called when a CSR_IP_ETHER_IF_MULTICAST_ADDR_ADD_RES
        message is received. This message will be received in response to a
        CSR_IP_ETHER_IF_MULTICAST_ADDR_ADD_IND.

    PARAMETERS
        instanceData - Task instance data.

*******************************************************************************/
void CsrIpEtherIfMulticastAddrAddResHandler(CsrIpInstanceData *instanceData)
{
    /* CsrIpEtherIfMulticastAddrAddRes *response = (CsrIpEtherIfMulticastAddrAddRes *) instanceData->msg; */
    CsrDebugTraceLow("UNIFI: CsrIpEtherIfMulticastAddrAddResHandler - linkState: %d", linkState);
}


/*******************************************************************************

    NAME
        CsrIpEtherIfMulticastAddrRemoveResHandler

    DESCRIPTION
        This function is called when a CSR_IP_ETHER_IF_MULTICAST_ADDR_REMOVE_RES
        message is received. This message will be received in response to a
        CSR_IP_ETHER_IF_MULTICAST_ADDR_REMOVE_IND.

    PARAMETERS
        instanceData - Task instance data.

*******************************************************************************/
void CsrIpEtherIfMulticastAddrRemoveResHandler(CsrIpInstanceData *instanceData)
{
    /* CsrIpEtherIfMulticastAddrRemoveRes *response = (CsrIpEtherIfMulticastAddrRemoveRes *) instanceData->msg; */
    CsrDebugTraceLow("UNIFI: CsrIpEtherIfMulticastAddrRemoveResHandler - linkState: %d", linkState);
}


/*******************************************************************************

    NAME
        CsrIpEtherIfMulticastAddrFlushResHandler

    DESCRIPTION
        This function is called when a CSR_IP_ETHER_IF_MULTICAST_ADDR_FLUSH_RES
        message is received. This message will be received in response to a
        CSR_IP_ETHER_IF_MULTICAST_ADDR_FLUSH_IND.

    PARAMETERS
        instanceData - Task instance data.

*******************************************************************************/
void CsrIpEtherIfMulticastAddrFlushResHandler(CsrIpInstanceData *instanceData)
{
    /* CsrIpEtherIfMulticastAddrFlushRes *response = (CsrIpEtherIfMulticastAddrFlushRes *) instanceData->msg; */
    CsrDebugTraceLow("UNIFI: CsrIpEtherIfMulticastAddrFlushResHandler - linkState: %d", linkState);
}


/*******************************************************************************

    NAME
        CsrIpEtherIfMulticastAddrGetResHandler

    DESCRIPTION
        This function is called when a CSR_IP_ETHER_IF_MULTICAST_ADDR_GET_RES
        message is received. This message will be received in response to a
        CSR_IP_ETHER_IF_MULTICAST_ADDR_GET_IND.

    PARAMETERS
        instanceData - Task instance data.

*******************************************************************************/
void CsrIpEtherIfMulticastAddrGetResHandler(CsrIpInstanceData *instanceData)
{
    /* CsrIpEtherIfMulticastAddrGetRes *response = (CsrIpEtherIfMulticastAddrGetRes *) instanceData->msg; */
    CsrDebugTraceLow("UNIFI: CsrIpEtherIfMulticastAddrGetResHandler - linkState: %d", linkState);
}


/*
 * Some APs send DHCP-ACK as a broacast type. But specially with A2DP working, 
 * Wi-Fi is likely to miss those packets, as their transmission time is not controlled
 * by STA. As a simpe workaround, DHCP-DISCOVER/REQ is retransmitted if there is no
 * response within a specified time.
 */
#ifdef CSR_WIFI_WORKAROUND_RETRY_DHCP_REQ

#define DHCP_HDR_GET_TYPE(p)        ((p)[242])
#define DHCP_HDR_GET_XID_PTR(p)     (&(p)[4])

#define DHCP_TYPE_DISCOVER          (1)
#define DHCP_TYPE_OFFER             (2)
#define DHCP_TYPE_REQ               (3)
#define DHCP_TYPE_ACK               (5)


/*
 * If DHCP-ACK is a broadcast, DTIM ITV >= 4 and there is a STA in PS mode,
 * then, there is a chance that DHCP-ACK is not received within DHCP_ACK_WAIT_TIME_MS
 */
#define DHCP_MAX_ACK_WAIT_TIME_MS (2000)
#define DHCP_MIN_ACK_WAIT_TIME_MS (100)
#define DHCP_REQ_RETRY_COUNT_MAX (19)
#define DHCP_ACK_WAIT_TIME_STEP_MS ((DHCP_MAX_ACK_WAIT_TIME_MS - DHCP_MIN_ACK_WAIT_TIME_MS) / DHCP_REQ_RETRY_COUNT_MAX)

static CsrUint32 dhcp_retx_expire_ms = DHCP_MAX_ACK_WAIT_TIME_MS;
static CsrUint32 dhcp_xid = 0;
static CsrUint8 dhcp_type = 0;
static CsrUint16 dhcp_retry_count = 0;
static TCPIP_PACKET_INFO_T dhcp_req_pkt_info = {NULL, 0, 0};
#define DHCP_IS_CACHED()    (dhcp_type != 0 && dhcp_req_pkt_info.data_ptr != NULL)

static void CsrIpMonitorCleanDhcpRes(void);

static void CsrUpdateSchedDhcpTimer(CsrUint32 delayms)
{
    DhcpRetryTimerId = CsrSchedTimerSet(delayms*1000,CsrIpMonitorExpired,0,NULL);
}

static void CsrIpMonitorCleanDhcpRes(void)
{
	CsrMutexLock(&mutex);
	
    dhcp_xid = 0;
    dhcp_type = 0;
    dhcp_retry_count = 0;
    if (dhcp_req_pkt_info.data_ptr != NULL)
        CsrPmemFree(dhcp_req_pkt_info.data_ptr);
    CsrMemSet (&dhcp_req_pkt_info, 0, sizeof(dhcp_req_pkt_info));

	dhcp_retx_expire_ms = 0;

	CsrMutexUnlock(&mutex);
}

static void CsrIpMonitorExpired(CsrUint16 fniarg, void *fnvarg)
{
	CsrUint32 dhcp_current_ms = 0;
	
    if (dhcp_retx_expire_ms == 0 || (!DHCP_IS_CACHED()))
	{
		CsrUpdateSchedDhcpTimer(DHCP_MAXIMUM_TIMEOUT_MS);
		return;
    }
	
    /* Check if link is still open */
    if (linkState != STATE_LINK_UP_OPEN)
    {
    	CsrUpdateSchedDhcpTimer(DHCP_MAXIMUM_TIMEOUT_MS);
        CsrIpMonitorCleanDhcpRes();
        return;
    }    

	dhcp_current_ms = SCI_GetTickCount();
	
	if(dhcp_retx_expire_ms > dhcp_current_ms)
	{
		CsrUpdateSchedDhcpTimer(dhcp_retx_expire_ms - dhcp_current_ms);	
		return;
	}
	
    /* Retransmit */
    dhcp_retry_count++;

	CsrDebugTraceLow("linkState is STATE_LINK_UP_OPEN and retry count:%d--expired tick:%d", dhcp_retry_count, dhcp_retx_expire_ms);
	
    CsrPacketTransmit(&dhcp_req_pkt_info);
    
    /* If retry exceeds a limitation, give up and clean resource. Otherwise, prepare for next expiration */    
    if (dhcp_retry_count >= DHCP_REQ_RETRY_COUNT_MAX){
		CsrUpdateSchedDhcpTimer(DHCP_MAXIMUM_TIMEOUT_MS);
        CsrIpMonitorCleanDhcpRes();
    }
    else
    {
		CsrUint32 dhcp_retx_delta_ms = 0;

		CsrMutexLock(&mutex);
		if(dhcp_retry_count == 0)	
		{
			CsrMutexUnlock(&mutex);		
			CsrUpdateSchedDhcpTimer(DHCP_MAXIMUM_TIMEOUT_MS);
        	return;
		}

		dhcp_retx_delta_ms =(DHCP_MAX_ACK_WAIT_TIME_MS - DHCP_ACK_WAIT_TIME_STEP_MS)/(dhcp_retry_count*dhcp_retry_count);

		if (dhcp_retx_delta_ms < DHCP_MIN_ACK_WAIT_TIME_MS)
		{
			srand(dhcp_current_ms);
			dhcp_retx_delta_ms = DHCP_MIN_ACK_WAIT_TIME_MS + ((uint32)((uint16)rand()*DHCP_MIN_ACK_WAIT_TIME_MS)/65535);
		}
		CsrUpdateSchedDhcpTimer(dhcp_retx_delta_ms);
		dhcp_retx_expire_ms = dhcp_retx_delta_ms + SCI_GetTickCount();	

		CsrMutexUnlock(&mutex);
    }
}

static void CsrIpMonitor(TCPIP_PACKET_INFO_T *p_pkt_info, int direction_is_tx)
{
    CsrUint8 pkt_dhcp_type;
    CsrUint32 pkt_xid;
    CsrUint8 *eth = p_pkt_info->data_ptr;
	
    // CSR: At this point, we can guarantee that the packet is DHCP
    pkt_dhcp_type = DHCP_HDR_GET_TYPE(eth + ETH_HDR_SIZE + IP_HDR_SIZE + UDP_HDR_SIZE);
    CsrMemCpy (&pkt_xid, DHCP_HDR_GET_XID_PTR(eth + ETH_HDR_SIZE + IP_HDR_SIZE + UDP_HDR_SIZE), sizeof(pkt_xid));    
    CsrDebugTraceLow("DIR=%d, T=%d, XID=0x%X", direction_is_tx, pkt_dhcp_type, pkt_xid);
    /*
     * As for Spreadtrum, XID increased every DHCP packets. So here XID is used to 
     * differentiate the new DHCP req from the cached one
     */
    if (direction_is_tx && 
        (pkt_dhcp_type == DHCP_TYPE_REQ || pkt_dhcp_type == DHCP_TYPE_DISCOVER))
    {
        CsrIpMonitorCleanDhcpRes();

		dhcp_retx_expire_ms = SCI_GetTickCount() + DHCP_MAX_ACK_WAIT_TIME_MS;
			
        dhcp_req_pkt_info.data_ptr = CsrPmemAlloc(p_pkt_info->data_len);
        if (dhcp_req_pkt_info.data_ptr == NULL)
        {
            CsrDebugTraceLow("~~~~ dhcp_req_pkt_info.data_ptr == NULL");
            return;
        }
        CsrMemCpy(dhcp_req_pkt_info.data_ptr, p_pkt_info->data_ptr, p_pkt_info->data_len);
        dhcp_req_pkt_info.data_len = p_pkt_info->data_len;
        dhcp_req_pkt_info.net_id = p_pkt_info->net_id;
        dhcp_xid = pkt_xid;
        dhcp_type = pkt_dhcp_type;
    }
    else if (!direction_is_tx && dhcp_xid == pkt_xid)
    {
        if ((dhcp_type == DHCP_TYPE_DISCOVER && pkt_dhcp_type == DHCP_TYPE_OFFER) ||
            (dhcp_type == DHCP_TYPE_REQ && pkt_dhcp_type == DHCP_TYPE_ACK))
        {
            CsrDebugTraceLow("~~~~ DHCP, Matched type(%d) was received(XID) = 0x%X", dhcp_type, dhcp_xid);
            CsrIpMonitorCleanDhcpRes();
        }
    }
}
#endif

