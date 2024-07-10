////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//            Copyright (C) 2003-2007, Coolsand Technologies, Inc.            //
//                            All Rights Reserved                             //
//                                                                            //
//      This source code is the property of Coolsand Technologies and is      //
//      confidential.  Any  modification, distribution,  reproduction or      //
//      exploitation  of  any content of this file is totally forbidden,      //
//      except  with the  written permission  of  Coolsand Technologies.      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  $HeadURL: http://svn.rdamicro.com/svn/developing1/Sources/chip/branches/gallite441/hal/src/hal_sys_ifc.c $ //
//    $Author: jingyuli $                                                        //
//    $Date: 2016-06-07 16:38:14 +0800 (Bi., 07 Nhlang. 2016) $                     //
//    $Revision: 32439 $                                                          //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file hal_sys_ifc.h                                                       //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "os_api.h"
#include "chip_plf_export.h"
#include "apifc_reg.h"
#include "apifc_drvapi.h"
//#include "halp_debug.h"
#include "timer_drvapi.h"

#define  HAL_ASSERT             SCI_PASSERT

#if defined OSI_SXR_OS
#include "osiapi.h"
#endif
#define IFC_ASSERT(BOOL,param)  HAL_ASSERT(BOOL, ("Ifc param = %d", param))
//bring_tmp, control reg: bit8~12
//#define DMA_ID_RX_CAMERA(APIFC_CH)   (APIFC_CH==0x13)? 0x9: (APIFC_CH)


/// That structure is used to memorize the owner of
/// each of the IFC channel.
HAL_APIFC_REQUEST_ID_T g_halApIfcChannelOwner[AP_IFC_STD_CHAN_NB];

UINT8   g_apifc_ch[APIFC_REQUESTID_NUM] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,0xFF, 0xFF, 0xFF,0xFF, 0xFF, 0xFF,0xFF, 0xFF, 0xFF, 0xFF,0xFF};
extern uint32 hal_SysEnterCriticalSectionifc(void);
extern void hal_SysExitCriticalSectionifc(uint32 status);
/*
uint32 hal_SysEnterCriticalSectionifc(void)
{
    SCI_DisableIRQ();
    //SCI_DisableFIQ();
    return 0;
}
void hal_SysExitCriticalSectionifc(uint32 status)
{
    SCI_RestoreIRQ();
    //SCI_RestoreFIQ();
}
*/
// =============================================================================
// hal_IfcOpen
// -----------------------------------------------------------------------------
/// Open the System IFC. Initialize some state variables.
// =============================================================================
void hal_ApIfcOpen(void)
{
    uint8 channel;

    // Initialize the channel table with unknown requests. This is more
    // of a sanity initialization.
    for (channel = 0; channel < AP_IFC_STD_CHAN_NB; channel++)
    {
        g_halApIfcChannelOwner[channel] = HAL_IFC_NO_REQWEST;
    }
}


// =============================================================================
// hal_IfcGetOwner
// -----------------------------------------------------------------------------
/// Get the owner ID of an IFC channel (i.e. the module using the channel).
/// @param channel number to check
/// @return The owner number associated with the channel
// =============================================================================
HAL_APIFC_REQUEST_ID_T hal_ApIfcGetOwner(uint8 channel)
{
    // Here, we consider the transfer as previously finished.
    if (channel == HAL_UNKNOWN_CHANNEL) return HAL_IFC_NO_REQWEST;

    // Channel number too big.
    IFC_ASSERT(channel < AP_IFC_STD_CHAN_NB, channel);

    return g_halApIfcChannelOwner[channel];
}

// =============================================================================
// hal_IfcChannelRelease
// -----------------------------------------------------------------------------
/// Force the release of a channel owned by a request.
///
/// The channel is only released if the specified request
/// owns the channel.
///
/// @param requestId Describe the direction of the tranfer (rx or
/// tx) and the module to or from which data are to be moved.
/// @param channel Channel to release
// =============================================================================
void hal_ApIfcChannelRelease(HAL_APIFC_REQUEST_ID_T requestId, uint8 channel)
{
    uint32 status;

    // Here, we consider the transfer as previously finished.
    if (channel == HAL_UNKNOWN_CHANNEL) return;

    // Channel number too big.
    IFC_ASSERT(channel < AP_IFC_STD_CHAN_NB, channel);

    status = hal_SysEnterCriticalSectionifc();
    if (g_halApIfcChannelOwner[channel] == requestId)
    {
        // Disable this channel
        // CAUTION: It needs several cycles to disable IFC, but the FLUSH bit
        // will take effect immediately. If the FLUSH bit is cleared while
        // the DISABLE bit is set, IFC will still work for the next few cycles,
        // which might lead to data loss!
        hwp_apIfc->std_ch[channel].control |= AP_IFC_REQ_SRC(0x1F);    /*将req_src设置为无效0x1F，避免双核平台requestid与Enable不同步导致start接口中对id通道使用的误判*/
        hwp_apIfc->std_ch[channel].control |= AP_IFC_DISABLE;
    }
    hal_SysExitCriticalSectionifc(status);
}


// =============================================================================
// hal_IfcChannelFlush
// -----------------------------------------------------------------------------
/// Empty the FIFO a specified channel.
///
/// If #requestId does not match #channel owner, does not do anything.
///
/// @param requestId Describe the direction of the tranfer (rx or
/// tx) and the module to or from which data are to be moved.
/// @param channel Channel whose FIFO is to be clear.
// =============================================================================
void hal_ApIfcChannelFlush(HAL_APIFC_REQUEST_ID_T requestId, uint8 channel)
{
    uint32 status;

    // Here, we consider the transfer as previously finished.
    if (channel == HAL_UNKNOWN_CHANNEL) return;

    // Channel number too big.
    IFC_ASSERT(channel < AP_IFC_STD_CHAN_NB, channel);

    // Check that the channel is really owned by the peripheral
    // which is doing the request, it could have been release
    // automatically or by an IRQ handler.
    status = hal_SysEnterCriticalSectionifc();//close irq
    if (g_halApIfcChannelOwner[channel] == requestId)
    {
        // Always flush IFC regardless of whether FIFO is currently empty.
        // In case of RX, if some data is being received, it has
        // NOT been put in FIFO yet.
        hwp_apIfc->std_ch[channel].control |= AP_IFC_FLUSH;
    }
    hal_SysExitCriticalSectionifc(status);//resume irq
}


// =============================================================================
// hal_IfcChannelIsFifoEmpty
// -----------------------------------------------------------------------------
/// Returns \c TRUE when the FIFO is empty.
///
/// If #requestId does not match #channel owner, returns TRUE.
///
/// @param requestId Describe the direction of the tranfer (rx or
/// tx) and the module to or from which data are to be moved.
/// @param channel Channel whose FIFO is to be checked.
// =============================================================================
BOOLEAN hal_ApIfcChannelIsFifoEmpty(HAL_APIFC_REQUEST_ID_T requestId, uint8 channel)
{
    uint32 status;
    BOOL fifoIsEmpty = TRUE;

    // Here, we consider the transfer as previously finished.
    if (channel == HAL_UNKNOWN_CHANNEL) return fifoIsEmpty;

    // Channel number too big.
    IFC_ASSERT(channel < AP_IFC_STD_CHAN_NB, channel);

    // Check that the channel is really owned by the peripheral
    // which is doing the request, it could have been release
    // automatically or by an IRQ handler.
    status = hal_SysEnterCriticalSectionifc();
    if (g_halApIfcChannelOwner[channel] == requestId)
    {
        fifoIsEmpty =
            (0 != (hwp_apIfc->std_ch[channel].status & AP_IFC_FIFO_EMPTY));
    }
    hal_SysExitCriticalSectionifc(status);

    return fifoIsEmpty;
}


// =============================================================================
// hal_IfcTransferStart
// -----------------------------------------------------------------------------
/// Start an IFC transfer
///
/// This is a non blocking function that starts the transfer
/// and returns the hand.
///
/// @param requestId Describe the direction of the tranfer (rx or
/// tx) and the module to or from which data are to be moved.
/// @param memStartAddr. Start address of the buffer where data
/// to be sent are located or where to put the data read, according
/// to the request defined by the previous parameter
/// @param xferSize Number of bytes to transfer. The maximum size
/// is 2^20 - 1 bytes.
/// @param ifcMode Mode of the transfer (Autodisable or not, 8 or 32 bits)
/// @return Channel got or HAL_UNKNOWN_CHANNEL.
// =============================================================================
uint8 hal_ApIfcTransferStart(HAL_APIFC_REQUEST_ID_T requestId, uint8* memStartAddr, uint32 xferSize, HAL_IFC_MODE_T ifcMode)
{
    uint32 status;
    uint8 channel = HAL_UNKNOWN_CHANNEL;

#if (CHIP_HAS_SYS_IFC_SIZE)
    // Check buffer alignment depending on the mode
    if (ifcMode == HAL_IFC_SIZE_32_MODE_MANUAL || ifcMode == HAL_IFC_SIZE_32_MODE_AUTO)
    {
        // check word alignment
        HAL_ASSERT(((UINT32)memStartAddr%4) == 0,
                   ("HAL IFC: 32 bits transfer misaligned 0x@%08X", memStartAddr));
    }
#if (CHIP_HAS_SYS_IFC_SIZE == 2)
    else if (ifcMode == HAL_IFC_SIZE_16_MODE_MANUAL || ifcMode == HAL_IFC_SIZE_16_MODE_AUTO)
    {
        // check half word alignment
        HAL_ASSERT(((UINT32)memStartAddr%2) == 0,
                   ("HAL IFC: 16 bits transfer misaligned 0x@%08X", memStartAddr));
    }
#endif
    else
    {
        // ifcMode == HAL_IFC_SIZE_8, nothing to check
    }
#endif

    HAL_ASSERT(xferSize < (1<<AP_IFC_TC_LEN),
               ("HAL IFC: Transfer size too large: %d", xferSize));

    HAL_ASSERT((requestId < APIFC_REQUESTID_NUM), ("apifc requestid %d is error", requestId));

#if(XCPU_CACHE_MODE_WRITE_BACK==1)
#if !defined OSI_SXR_OS    // not armcc
    if((U32)memStartAddr&0x20000000==0)
        hal_DcacheFlushAddrRange((UINT32)memStartAddr, (UINT32)memStartAddr+xferSize);
#endif
#endif
   MMU_DmaCacheSync((UINT32)memStartAddr, xferSize,DMABUFFER_BIDIRECTIONAL);

    status = hal_SysEnterCriticalSectionifc();
#if defined OSI_SXR_OS
    OSI_DCACHE_FLUSH_INVAL(memStartAddr,xferSize);
#error
#endif

    channel = g_apifc_ch[requestId];
   SCI_TRACE_LOW("rui.zhang1 channel=%d",channel);
    HAL_ASSERT((channel < AP_IFC_STD_CHAN_NB), ("apifc requestId %d ch %d is error", requestId, channel));

    // Check the requested id is not currently already used.
    HAL_ASSERT((hwp_apIfc->std_ch[channel].status & AP_IFC_ENABLE) == 0,
                       ("HAL: Attempt to use the IFC to deal with a %d"
                       " request still active on channel %d", requestId, channel));

    g_halApIfcChannelOwner[channel]     = requestId;
    hwp_apIfc->std_ch[channel].start_addr  =  (UINT32) memStartAddr;
    hwp_apIfc->std_ch[channel].tc          =  xferSize;
    hwp_apIfc->std_ch[channel].control     = (AP_IFC_REQ_SRC(requestId)
            | ifcMode
            | AP_IFC_CH_RD_HW_EXCH
            | AP_IFC_ENABLE);

    hal_SysExitCriticalSectionifc(status);
    return channel;
}


// =============================================================================
// hal_IfcGetTc
// -----------------------------------------------------------------------------
/// Returns the transfer count of the IFC.
/// If #requestId does not match #channel owner, returns 0.
///
/// @param requestId Describe the direction of the tranfer (rx or
/// tx) and the module to or from which data are to be moved.
/// @param channel Number of the channel whose transfer we are concerned for.
/// @return The number of bytes remaining to be transfered.
// =============================================================================
uint32 hal_ApIfcGetTc(HAL_APIFC_REQUEST_ID_T requestId, uint8 channel)
{
    uint32 status;
    uint32 tc = 0;
    //mon_Event(0xaa001111);
    // Here, we consider the transfer as previously finished.
    if (channel == HAL_UNKNOWN_CHANNEL) return tc;
  // mon_Event(channel);
    // Channel number too big.
    IFC_ASSERT(channel < AP_IFC_STD_CHAN_NB, channel);

    // Check that the channel is really owned by the peripheral
    // which is doing the request, it could have been release
    // automatically or by an IRQ handler.
    status = hal_SysEnterCriticalSectionifc();
    if (g_halApIfcChannelOwner[channel] == requestId)
    {
        tc = hwp_apIfc->std_ch[channel].tc;
    }
    hal_SysExitCriticalSectionifc(status);

    return tc;
}


// =============================================================================
// hal_IfcExtendTransfer
// -----------------------------------------------------------------------------
/// Extend the transfer size of the channel of
/// the number ox bytes passed as a parameter.
///
/// Should be used only when a channel has been locked.
///
/// @param xferSize Number of bytes to add to the
/// transfer size of this channel
// =============================================================================
void hal_ApIfcExtendTransfer(uint8 channel, uint16 xferSize)
{
    // Here, we consider the transfer as previously finished.
    if (channel == HAL_UNKNOWN_CHANNEL) return;

    // Channel number too big.
    IFC_ASSERT(channel < AP_IFC_STD_CHAN_NB, channel);

    // Writing in the Tc register adds the number written
    // to the counter of bytes yet to transfer
    hwp_apIfc->std_ch[channel].tc = xferSize;
}


// =============================================================================
// hal_IfcGetChannel
// -----------------------------------------------------------------------------
/// Get an IFC channel opend in NO auto-disable mode
///
/// @param requestId Describe the direction of the tranfer (rx or
/// tx) and the module to or from which data are to be moved.
/// @param ifcMode Mode of the transfer (Autodisable or not, 8 or 32 bits)
/// @return The channel number got or HAL_UNKNOWN_CHANNEL
// =============================================================================
uint8 hal_ApIfcGetChannel(HAL_APIFC_REQUEST_ID_T requestId, HAL_IFC_MODE_T ifcMode)
{
    uint8 channel;
    uint32 status;

    // This function is only relevant/available for manual transferts.
    HAL_ASSERT( ifcMode == HAL_IFC_SIZE_8_MODE_MANUAL
#if (CHIP_HAS_SYS_IFC_SIZE)
#if (CHIP_HAS_SYS_IFC_SIZE == 2)
                || ifcMode == HAL_IFC_SIZE_16_MODE_MANUAL
#endif
                || ifcMode == HAL_IFC_SIZE_32_MODE_MANUAL
#endif
                ,
                ("hal_IfcGetChannel called with an autodisable %d mode", ifcMode));

    status = hal_SysEnterCriticalSectionifc();
    channel = g_apifc_ch[requestId];

    if (channel >= AP_IFC_STD_CHAN_NB)
    {
        hal_SysExitCriticalSectionifc(status);
        return HAL_UNKNOWN_CHANNEL;
    }

    hwp_apIfc->std_ch[channel].tc =  0;
    hwp_apIfc->std_ch[channel].start_addr = 0;
    g_halApIfcChannelOwner[channel] = requestId;
    hwp_apIfc->std_ch[channel].control = (AP_IFC_REQ_SRC(requestId)
                                           | ifcMode
                                           | AP_IFC_CH_RD_HW_EXCH
                                           | AP_IFC_ENABLE);

    hal_SysExitCriticalSectionifc(status);

    return channel;
}


// =============================================================================
// hal_IfcSetStartAddress
// -----------------------------------------------------------------------------
/// Set the start address of a previously opened (with hal_IfcGetChannel)
/// IFC channel.
///
/// Should be used only when a channel has been locked.
///
/// @param channel Opened channel whose start address will be set.
/// @param startAddress Set start address of the channel.
// =============================================================================
void hal_ApIfcSetStartAddress(uint8 channel, CONST uint8* startAddress)
{
    // channel number too big
    IFC_ASSERT(channel < AP_IFC_STD_CHAN_NB, channel);
    HAL_ASSERT(hwp_apIfc->std_ch[channel].tc == 0, ("Attempt to change an IFC start_address "
               "while tc != 0 (==%d), channel=%d", hwp_apIfc->std_ch[channel].tc, channel));

    // Writing in the Tc register adds the number written
    // to the counter of bytes yet to transfer
    hwp_apIfc->std_ch[channel].start_addr = (UINT32) startAddress;
}

uint32 hal_ApIfcGetRunStatus(HAL_APIFC_REQUEST_ID_T requestId, uint8 channel)
{
    uint32 status;
    uint32 tc = 0;

    // Here, we consider the transfer as previously finished.
    if (channel == HAL_UNKNOWN_CHANNEL) return tc;

    // Channel number too big.
    IFC_ASSERT(channel < AP_IFC_STD_CHAN_NB, channel);

    // Check that the channel is really owned by the peripheral
    // which is doing the request, it could have been release
    // automatically or by an IRQ handler.
    status = hal_SysEnterCriticalSectionifc();
    if (g_halApIfcChannelOwner[channel] == requestId)
    {
        tc = hwp_apIfc->std_ch[channel].status & AP_IFC_ENABLE;
    }
    hal_SysExitCriticalSectionifc(status);

    return tc;
}

