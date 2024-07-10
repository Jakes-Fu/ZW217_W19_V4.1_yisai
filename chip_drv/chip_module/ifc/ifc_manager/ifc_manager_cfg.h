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

#ifndef _IFC_MANAGER_CFG_H_
#define _IFC_MANAGER_CFG_H_
#include "../lpsifc/lpsifc_reg.h"
#include "../apifc/apifc_reg.h"
#include "../aonifc/aonifc_reg.h"

#define REG_ACCESS_ADDRESS(addr) (addr)
#define uint8 unsigned char
#define uint32 unsigned long int
typedef enum{
//lps_ifc_channel
	DMA_ID_TX_UART1 = 0,
	DMA_ID_RX_UART1 = 1,
	
//aon_ifc_channel
	DMA_ID_TX_UART2,
	DMA_ID_RX_UART2,
	DMA_ID_TX_UART3,
	DMA_ID_RX_UART3,
	DMA_ID_TX_DBG_UART,
	DMA_ID_RX_DBG_UART,

//ap_ifc_channel
	DMA_ID_TX_UART4,
	DMA_ID_RX_UART4,
	DMA_ID_TX_UART5,
	DMA_ID_RX_UART5,
	DMA_ID_TX_UART6,
	DMA_ID_RX_UART6,
	DMA_ID_TX_SDMMC,
	DMA_ID_RX_SDMMC,
	DMA_ID_RSVD,
	DMA_ID_RX_CAMERA,

	DMA_IFC_NO_REQWEST=0xff,
}IFC_MANAGER_CHANNEL_LIST;

typedef struct{
	IFC_MANAGER_CHANNEL_LIST logic_id;//logic num
	uint8 phy_num;//for ap,aon,or lps_ifc num of channel
	uint32 base_addr;
}IFC_MANAGER_CFG_INFO_T;


#endif // _AP_IFC_H_
