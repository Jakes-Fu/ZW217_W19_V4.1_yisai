/* Copyright (C) 2016 RDA Technologies Limited and/or its affiliates("RDA").
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


#ifndef _CP_SYSREG_ASM_ARM_H_
#define _CP_SYSREG_ASM_ARM_H_

//THIS FILE HAS BEEN GENERATED WITH COOLWATCHER. PLEASE EDIT WITH CARE !

#include "global_macros.h"

//==============================================================================
// cp_sysreg
//------------------------------------------------------------------------------
///
//==============================================================================
#if defined(REG_ADDRESS_FOR_GGE)
#define REG_SYSREG_BASE            0x05080000
#else
#define REG_SYSREG_BASE            0x50080000
#endif

#define REG_SYSREG_ZSP_SVTADDR        0x00000000
#define REG_SYSREG_MEM_EMA_CFG_ZSP    0x00000004
#define REG_SYSREG_MEM_EMA_CFG_LTE    0x00000008
#define REG_SYSREG_ROM_EMA_CFG_LTE    0x0000000C
#define REG_SYSREG_MEM_EMA_CFG_BBSYS    0x00000010
#define REG_SYSREG_ZSP_QOS            0x00000014
#define REG_SYSREG_GSM_EN_SEL         0x00000018
#define REG_SYSREG_FLOW_PARA          0x0000001C
#define REG_SYSREG_RF_SEL             0x00000020
#define REG_SYSREG_NB_LTE_SEL         0x00000024
#define REG_SYSREG_GGENB_SEL          0x00000028
#define REG_SYSREG_RF_ANA_26M_CTRL    0x0000002C
#define REG_SYSREG_RF_ANA_26M_CTRL_SET    0x00000030
#define REG_SYSREG_RF_ANA_26M_CTRL_CLR    0x00000034
#define REG_SYSREG_DDR_SLP_CTRL_ENABLE    0x00000038
#define REG_SYSREG_DDR_WAKEUP_FORCE_EN    0x0000003C
#define REG_SYSREG_DDR_WAKEUP_FORCE_ACK    0x00000040
#define REG_SYSREG_DDR_SLP_WAIT_NUMBER    0x00000044
#define REG_SYSREG_LVDS_SPI_SEL       0x00000048
#define REG_SYSREG_MONITOR_LTE_FINT_SEL    0x0000004C
#define REG_SYSREG_RFMUX_IRQ_STA      0x00000050

//ZSP_SVTADDR
#define CP_SYSREG_SVTADDR(n)       (((n)&0xFFFFFF)<<8)
#define CP_SYSREG_RESERVED_0(n)    (((n)&0xFF)<<0)

//MEM_EMA_CFG_ZSP
#define CP_SYSREG_RESERVED_0_X1(n) (((n)&3)<<30)
#define CP_SYSREG_RFTPD_RMB(n)     (((n)&15)<<26)
#define CP_SYSREG_RFTPD_RMEB       (1<<25)
#define CP_SYSREG_RFTPD_RMA(n)     (((n)&15)<<21)
#define CP_SYSREG_RFTPD_RMEA       (1<<20)
#define CP_SYSREG_RFSPD_RM(n)      (((n)&15)<<16)
#define CP_SYSREG_RFSPD_RME        (1<<15)
#define CP_SYSREG_RASPU_RM(n)      (((n)&15)<<11)
#define CP_SYSREG_RASPU_RME        (1<<10)
#define CP_SYSREG_RADPD_RMB(n)     (((n)&15)<<6)
#define CP_SYSREG_RADPD_RMEB       (1<<5)
#define CP_SYSREG_RADPD_RMA(n)     (((n)&15)<<1)
#define CP_SYSREG_RADPD_RMEA       (1<<0)

//MEM_EMA_CFG_LTE
//#define CP_SYSREG_RESERVED_0_X1(n) (((n)&3)<<30)
//#define CP_SYSREG_RFTPD_RMB(n)   (((n)&15)<<26)
//#define CP_SYSREG_RFTPD_RMEB     (1<<25)
//#define CP_SYSREG_RFTPD_RMA(n)   (((n)&15)<<21)
//#define CP_SYSREG_RFTPD_RMEA     (1<<20)
//#define CP_SYSREG_RFSPD_RM(n)    (((n)&15)<<16)
//#define CP_SYSREG_RFSPD_RME      (1<<15)
//#define CP_SYSREG_RASPU_RM(n)    (((n)&15)<<11)
//#define CP_SYSREG_RASPU_RME      (1<<10)
//#define CP_SYSREG_RADPD_RMB(n)   (((n)&15)<<6)
//#define CP_SYSREG_RADPD_RMEB     (1<<5)
//#define CP_SYSREG_RADPD_RMA(n)   (((n)&15)<<1)
//#define CP_SYSREG_RADPD_RMEA     (1<<0)

//ROM_EMA_CFG_LTE
#define CP_SYSREG_RESERVED_0_X2(n) (((n)&0x7FFFFFF)<<5)
#define CP_SYSREG_ROM_RMA(n)       (((n)&15)<<1)
#define CP_SYSREG_ROM_RMEA         (1<<0)

//MEM_EMA_CFG_BBSYS
//#define CP_SYSREG_RESERVED_0_X1(n) (((n)&3)<<30)
//#define CP_SYSREG_RFTPD_RMB(n)   (((n)&15)<<26)
//#define CP_SYSREG_RFTPD_RMEB     (1<<25)
//#define CP_SYSREG_RFTPD_RMA(n)   (((n)&15)<<21)
//#define CP_SYSREG_RFTPD_RMEA     (1<<20)
//#define CP_SYSREG_RFSPD_RM(n)    (((n)&15)<<16)
//#define CP_SYSREG_RFSPD_RME      (1<<15)
//#define CP_SYSREG_RASPU_RM(n)    (((n)&15)<<11)
//#define CP_SYSREG_RASPU_RME      (1<<10)
//#define CP_SYSREG_RADPD_RMB(n)   (((n)&15)<<6)
//#define CP_SYSREG_RADPD_RMEB     (1<<5)
//#define CP_SYSREG_RADPD_RMA(n)   (((n)&15)<<1)
//#define CP_SYSREG_RADPD_RMEA     (1<<0)

//ZSP_QOS
#define CP_SYSREG_RESERVED_0_X3(n) (((n)&0xFF)<<24)
#define CP_SYSREG_AWQOS_ZSP_AXIDMA(n) (((n)&15)<<20)
#define CP_SYSREG_ARQOS_ZSP_AXIDMA(n) (((n)&15)<<16)
#define CP_SYSREG_AWQOS_ZSP_IBUS(n) (((n)&15)<<12)
#define CP_SYSREG_ARQOS_ZSP_IBUS(n) (((n)&15)<<8)
#define CP_SYSREG_AWQOS_ZSP_DBUS(n) (((n)&15)<<4)
#define CP_SYSREG_ARQOS_ZSP_DBUS(n) (((n)&15)<<0)

//GSM_EN_SEL
#define CP_SYSREG_RESERVED_0_X4(n) (((n)&0x7FFFFFFF)<<1)
#define CP_SYSREG_SELECT           (1<<0)

//FLOW_PARA
#define CP_SYSREG_FLOWPARA(n)      (((n)&0xFFFFFFFF)<<0)

//RF_SEL
//#define CP_SYSREG_RESERVED_0_X4(n) (((n)&0x7FFFFFFF)<<1)
#define CP_SYSREG_RFSEL            (1<<0)

//NB_LTE_SEL
//#define CP_SYSREG_RESERVED_0_X4(n) (((n)&0x7FFFFFFF)<<1)
#define CP_SYSREG_NBLTE_SEL        (1<<0)

//GGENB_SEL
//#define CP_SYSREG_RESERVED_0_X4(n) (((n)&0x7FFFFFFF)<<1)
#define CP_SYSREG_GGE_NB_SEL       (1<<0)

//RF_ANA_26M_CTRL
#define CP_SYSREG_RESERVED_0_X5(n) (((n)&0x3FFFFFF)<<6)
#define CP_SYSREG_MUXSEL_CLK26M_AUD2AD (1<<5)
#define CP_SYSREG_ENABLE_CLK_26M_AUD2AD (1<<4)
#define CP_SYSREG_MUXSEL_CLK26M_AUDIO (1<<3)
#define CP_SYSREG_ENABLE_CLK_26M_AUDIO (1<<2)
#define CP_SYSREG_ENABLE_CLK_26M_DSI (1<<1)
#define CP_SYSREG_ENABLE_CLK_26M_USB (1<<0)

//RF_ANA_26M_CTRL_SET
//#define CP_SYSREG_RESERVED_0_X5(n) (((n)&0x3FFFFFF)<<6)
#define CP_SYSREG_SET(n)           (((n)&0x3F)<<0)

//RF_ANA_26M_CTRL_CLR
//#define CP_SYSREG_RESERVED_0_X5(n) (((n)&0x3FFFFFF)<<6)
#define CP_SYSREG_CLEAR(n)         (((n)&0x3F)<<0)

//DDR_SLP_CTRL_ENABLE
//#define CP_SYSREG_RESERVED_0_X4(n) (((n)&0x7FFFFFFF)<<1)
#define CP_SYSREG_ENABLE           (1<<0)

//DDR_WAKEUP_FORCE_EN
//#define CP_SYSREG_RESERVED_0_X4(n) (((n)&0x7FFFFFFF)<<1)
//#define CP_SYSREG_ENABLE         (1<<0)

//DDR_WAKEUP_FORCE_ACK
//#define CP_SYSREG_RESERVED_0_X4(n) (((n)&0x7FFFFFFF)<<1)
#define CP_SYSREG_ACK              (1<<0)

//DDR_SLP_WAIT_NUMBER
#define CP_SYSREG_RESERVED_0_X6(n) (((n)&0xFFFF)<<16)
#define CP_SYSREG_N(n)             (((n)&0xFFFF)<<0)

//LVDS_SPI_SEL
//#define CP_SYSREG_RESERVED_0_X4(n) (((n)&0x7FFFFFFF)<<1)
#define CP_SYSREG_SEL              (1<<0)

//MONITOR_LTE_FINT_SEL
//#define CP_SYSREG_RESERVED_0_X4(n) (((n)&0x7FFFFFFF)<<1)
//#define CP_SYSREG_SEL            (1<<0)

//RFMUX_IRQ_STA
#define CP_SYSREG_RESERVED_0_X7(n) (((n)&0x3FFFFFFF)<<2)
#define CP_SYSREG_RFSPI_CONFLICT_IRQ (1<<0)
#define CP_SYSREG_TXFIFO_FULL_IRQ  (1<<0)
#define CP_SYSREG_RESERVED_1       (1<<1)




#endif
