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
#include "ifc_manager_cfg.h"
const IFC_MANAGER_CFG_INFO_T ifc_mgr_cfg_info[]={
/*|logic_num|phy_num|base_addr*/
	{DMA_ID_TX_UART1,0,REG_ACCESS_ADDRESS(REG_LPS_IFC_BASE)},
	{DMA_ID_RX_UART1,1,REG_ACCESS_ADDRESS(REG_LPS_IFC_BASE)},
	
	{DMA_ID_TX_UART2,0,REG_ACCESS_ADDRESS(REG_AON_IFC_BASE)},
	{DMA_ID_RX_UART2,1,REG_ACCESS_ADDRESS(REG_AON_IFC_BASE)},
	{DMA_ID_TX_UART3,2,REG_ACCESS_ADDRESS(REG_AON_IFC_BASE)},
	{DMA_ID_RX_UART3,3,REG_ACCESS_ADDRESS(REG_AON_IFC_BASE)},
	{DMA_ID_TX_DBG_UART,4,REG_ACCESS_ADDRESS(REG_AON_IFC_BASE)},
	{DMA_ID_RX_DBG_UART,5,REG_ACCESS_ADDRESS(REG_AON_IFC_BASE)},
	
	{DMA_ID_TX_UART4,0,REG_ACCESS_ADDRESS(REG_AP_IFC_BASE)},
	{DMA_ID_RX_UART4,1,REG_ACCESS_ADDRESS(REG_AP_IFC_BASE)},
	{DMA_ID_TX_UART5,2,REG_ACCESS_ADDRESS(REG_AP_IFC_BASE)},
	{DMA_ID_RX_UART5,3,REG_ACCESS_ADDRESS(REG_AP_IFC_BASE)},
	{DMA_ID_TX_UART6,4,REG_ACCESS_ADDRESS(REG_AP_IFC_BASE)},
	{DMA_ID_RX_UART6,5,REG_ACCESS_ADDRESS(REG_AP_IFC_BASE)},
	{DMA_ID_TX_SDMMC,6,REG_ACCESS_ADDRESS(REG_AP_IFC_BASE)},
	{DMA_ID_RX_SDMMC,7,REG_ACCESS_ADDRESS(REG_AP_IFC_BASE)},
	{DMA_ID_RSVD,8,REG_ACCESS_ADDRESS(REG_AP_IFC_BASE)},
	{DMA_ID_RX_CAMERA,9,REG_ACCESS_ADDRESS(REG_AP_IFC_BASE)},
	{DMA_IFC_NO_REQWEST,0xFF,0xFF},
};
