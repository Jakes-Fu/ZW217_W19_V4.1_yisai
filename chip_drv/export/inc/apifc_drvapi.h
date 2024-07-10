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
//  $HeadURL: http://svn.rdamicro.com/svn/developing1/Sources/chip/branches/gallite441/hal/src/halp_sys_ifc.h $ //
//    $Author: huazeng $                                                        // 
//    $Date: 2014-08-22 18:26:33 +0800 (Fri, 22 Aug 2014) $                     //   
//    $Revision: 23356 $                                                         //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
///     @file hal_sys_ifc.h
///     Prototypes for private IFC functions and defines
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#ifndef  _HALP_SYS_IFC_H_
#define  _HALP_SYS_IFC_H_

#define HAL_UNKNOWN_CHANNEL      0xff
#ifndef WIN32
typedef unsigned char			BOOL;
#endif
typedef unsigned char			UINT8;
typedef unsigned short			UINT16;
typedef unsigned long			UINT32;
#define  VOID       void

#include "../../chip_module/ifc/apifc/apifc_reg.h"

/*as ap/cp all use apifc ip,so it is a better method of using some ch by oneself 
   AP use ch0-ch2(ch0:sdmmc1_tx,ch1:sdmmc1_rx,ch2:camera_rx),CP use ch3-ch6*/
#define     APIFC_UART4_TX_CH         (0)
#define     APIFC_UART4_RX_CH         (1)
#define     APIFC_UART5_RX_CH         (2)
#define     APIFC_UART5_TX_CH               (3)
#define     APIFC_UART6_RX_CH               (4)
#define     APIFC_UART6_TX_CH               (5)
#define     APIFC_SDMMC1_RX_CH               (6)
#define     APIFC_SDMMC1_TX_CH               (7)
#define     APIFC_RSVD_CH               (8)
#define     APIFC_CAM_RX_CH               (9)

#define     APIFC_REQUESTID_NUM       (0x14)

/// IFC transaction type Id
typedef enum {
	HAL_IFC_UART4_TX=0,
	HAL_IFC_UART4_RX,
	HAL_IFC_UART5_TX,
	HAL_IFC_UART5_RX,
	HAL_IFC_UART6_TX,
	HAL_IFC_UART6_RX,
	HAL_IFC_SDMMC1_TX,
	HAL_IFC_SDMMC1_RX,
	HAL_IFC_RSVD,
	HAL_IFC_CAM_RX,

    	HAL_IFC_NO_REQWEST
} HAL_APIFC_REQUEST_ID_T;

// =============================================================================
// HAL_IFC_MODE_T
// -----------------------------------------------------------------------------
/// Define the mode used to configure an IFC transfer. This enum describes
/// the width (8, 16 or 32 bits) and if the transfer is autodisabled or manually
/// disabled.
// =============================================================================
typedef enum
{
    HAL_IFC_SIZE_8_MODE_MANUAL  = (0 | 0),
    HAL_IFC_SIZE_8_MODE_AUTO    = (0 | AP_IFC_AUTODISABLE),
    //HAL_IFC_SIZE_16_MODE_MANUAL = (SYS_IFC_SIZE_HALF_WORD | 0),
    //HAL_IFC_SIZE_16_MODE_AUTO   = (SYS_IFC_SIZE_HALF_WORD | AP_IFC_AUTODISABLE),
    HAL_IFC_SIZE_16_MODE_MANUAL = (0 | 0),
    HAL_IFC_SIZE_16_MODE_AUTO   = (0 | AP_IFC_AUTODISABLE),
    HAL_IFC_SIZE_32_MODE_MANUAL = (AP_IFC_SIZE | 0),
    HAL_IFC_SIZE_32_MODE_AUTO   = (AP_IFC_SIZE | AP_IFC_AUTODISABLE),
} HAL_IFC_MODE_T;


// =============================================================================
// hal_IfcOpen
// -----------------------------------------------------------------------------
/// Open the System IFC. Initialize some state variables. 
// =============================================================================
VOID hal_ApIfcOpen(VOID);


// =============================================================================
// hal_IfcGetOwner
// -----------------------------------------------------------------------------
/// Get the owner ID of an IFC channel (i.e. the module using the channel).
/// @param channel number to check
/// @return The owner number associated with the channel
// =============================================================================
HAL_APIFC_REQUEST_ID_T hal_ApIfcGetOwner(UINT8 channel);


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
 VOID hal_ApIfcChannelRelease(HAL_APIFC_REQUEST_ID_T requestId, UINT8 channel);


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
VOID hal_ApIfcChannelFlush(HAL_APIFC_REQUEST_ID_T requestId, UINT8 channel);


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
BOOL hal_ApIfcChannelIsFifoEmpty(HAL_APIFC_REQUEST_ID_T requestId, UINT8 channel);


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
UINT8 hal_ApIfcTransferStart(HAL_APIFC_REQUEST_ID_T requestId, UINT8* memStartAddr, UINT32 xferSize, HAL_IFC_MODE_T ifcMode);


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
UINT32 hal_ApIfcGetTc(HAL_APIFC_REQUEST_ID_T requestId, UINT8 channel);


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
VOID hal_ApIfcExtendTransfer(UINT8 channel, UINT16 xferSize);


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
UINT8 hal_ApIfcGetChannel(HAL_APIFC_REQUEST_ID_T requestId, HAL_IFC_MODE_T ifcMode);


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
VOID hal_ApIfcSetStartAddress(UINT8 channel, CONST UINT8* startAddress);



#endif //  HAL_SYS_IFC_H

