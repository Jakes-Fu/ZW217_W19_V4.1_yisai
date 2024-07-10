/* Copyright (C) 2018 RDA Technologies Limited and/or its affiliates("RDA").
 * All rights reserved.
 *
 * This software is supplied "AS IS" without any warranties.
 * RDA assumes no responsibility or liability for the use of the software,
 * conveys no license or title under any patent, copyright, or mask work
 * right to the product. RDA reserves the right to make changes in the
 * software without notification.  RDA also make no representation or
 * warranty that such application will be suitable for the specified use
 * without further testing or modification.
 */

#ifndef _IFC_DRVAPI_H_
#define _IFC_DRVAPI_H_
#include "../../chip_module/ifc/ifc_manager/ifc_manager_cfg.h"
#define IFC_MANAGER_CHAN_NB (18)
#define IFC_UNKNOWN_CHANNEL 0xFF
#define HAL_UNKNOWN_CHANNEL      0xff
#define CHIP_HAS_SYS_IFC_SIZE 0
#define XCPU_CACHE_MODE_WRITE_BACK 0
// get_ch
#define IFC_CH_TO_USE(n) (((n)&0x1f) << 0)

// dma_status
#define IFC_CH_ENABLE(n) (((n)&0x3) << 0)
#define IFC_CH_BUSY(n) (((n)&0x3) << 16)

// debug_status
#define IFC_DBG_STATUS (1 << 0)

// ifc_sec
#define IFC_STD_CH_REG_SEC(n) (((n)&0x3) << 0)
#define IFC_STD_CH_DMA_SEC(n) (((n)&0x3) << 16)

// control
#define IFC_ENABLE (1 << 0)
#define IFC_DISABLE (1 << 1)
#define IFC_CH_RD_HW_EXCH (1 << 2)
#define IFC_CH_WR_HW_EXCH (1 << 3)
#define IFC_AUTODISABLE (1 << 4)
#define IFC_SIZE (1 << 5)
#define IFC_REQ_SRC(n) (((n)&0x1f) << 8)
#define IFC_REQ_SRC_DMA_ID_TX_UART1 (0 << 8)
#define IFC_REQ_SRC_DMA_ID_RX_UART1 (1 << 8)
#define IFC_FLUSH (1 << 16)
#define IFC_MAX_BURST_LENGTH(n) (((n)&0x3) << 17)

#define IFC_REQ_SRC_V_DMA_ID_TX_UART1 (0)
#define IFC_REQ_SRC_V_DMA_ID_RX_UART1 (1)

// status
#define IFC_ENABLE (1 << 0)
#define IFC_FIFO_EMPTY (1 << 4)

// start_addr
#define IFC_START_ADDR(n) (((n)&0xffffffff) << 0)

// tc
#define IFC_TC(n) (((n)&0x7fffff) << 0)

// tc_threshold
#define IFC_TC_THRESHOLD(n) (((n)&0x7fffff) << 0)

extern const IFC_MANAGER_CFG_INFO_T ifc_mgr_cfg_info_t[];
typedef enum
{
	LPS_IFC,
	AON_IFC,
	AP_IFC,
	IFC_MAX,
}IFC_MANAGER_HWP_LIST;
typedef enum
{
    HAL_IFC_SIZE_8_MODE_MANUAL  = (0 | 0),
    HAL_IFC_SIZE_8_MODE_AUTO    = (0 | IFC_AUTODISABLE),
    //HAL_IFC_SIZE_16_MODE_MANUAL = (SYS_IFC_SIZE_HALF_WORD | 0),
    //HAL_IFC_SIZE_16_MODE_AUTO   = (SYS_IFC_SIZE_HALF_WORD | AP_IFC_AUTODISABLE),
    HAL_IFC_SIZE_16_MODE_MANUAL = (0 | 0),
    HAL_IFC_SIZE_16_MODE_AUTO   = (0 | IFC_AUTODISABLE),
    HAL_IFC_SIZE_32_MODE_MANUAL = (IFC_SIZE | 0),
    HAL_IFC_SIZE_32_MODE_AUTO   = (IFC_SIZE | IFC_AUTODISABLE),
} HAL_IFC_MODE_T;

#if defined(W217_UWS6121EG_GNSS_PATCH_20231019)// wuxx add PATCH01241811_20231019_V1
typedef struct
{
    uint8 channel_id;
    uint8 req_id;
}HAL_IFC_CHANNEL_INFO;

#endif
void hal_IfcOpen(void);
void hal_IfcChannelRelease(IFC_MANAGER_CHANNEL_LIST requestId);
void hal_IfcChannelFlush(IFC_MANAGER_CHANNEL_LIST requestId);
BOOLEAN hal_IfcChannelIsFifoEmpty(IFC_MANAGER_CHANNEL_LIST requestId);
uint8 hal_IfcTransferStart(IFC_MANAGER_CHANNEL_LIST requestId, uint8* memStartAddr, uint32 xferSize, HAL_IFC_MODE_T ifcMode);
uint32 hal_IfcGetTc(IFC_MANAGER_CHANNEL_LIST requestId);
void hal_IfcExtendTransfer(IFC_MANAGER_CHANNEL_LIST requestId, uint16 xferSize);
uint8 hal_IfcGetChannel(IFC_MANAGER_CHANNEL_LIST requestId, HAL_IFC_MODE_T ifcMode);
void hal_IfcSetStartAddress(IFC_MANAGER_CHANNEL_LIST requestId, CONST uint8* startAddress);
uint32 hal_IfcGetRunStatus(IFC_MANAGER_CHANNEL_LIST requestId);
void hal_IfcOpenedChnl(void);

#endif // _AP_IFC_H_
