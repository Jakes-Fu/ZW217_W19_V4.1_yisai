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


#ifndef _CP_SYSREG_H_
#define _CP_SYSREG_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'cp_sysreg'."
#endif



// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// CP_SYSREG_T
// -----------------------------------------------------------------------------
///
// =============================================================================
#if defined(REG_ADDRESS_FOR_GGE)
#define REG_SYSREG_BASE            0x05080000
#else
#define REG_SYSREG_BASE            0x50080000
#endif

typedef volatile struct
{
    REG32                          ZSP_SVTADDR;                  //0x00000000
    REG32                          MEM_EMA_CFG_ZSP;              //0x00000004
    REG32                          MEM_EMA_CFG_LTE;              //0x00000008
    REG32                          ROM_EMA_CFG_LTE;              //0x0000000C
    REG32                          MEM_EMA_CFG_BBSYS;            //0x00000010
    REG32                          ZSP_QOS;                      //0x00000014
    REG32                          GSM_EN_SEL;                   //0x00000018
    REG32                          FLOW_PARA;                    //0x0000001C
    REG32                          RF_SEL;                       //0x00000020
    REG32                          NB_LTE_SEL;                   //0x00000024
    REG32                          GGENB_SEL;                    //0x00000028
} HWP_CP_SYSREG_T;

#define hwp_sysreg                 ((HWP_CP_SYSREG_T*) REG_ACCESS_ADDRESS(REG_SYSREG_BASE))


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





#endif

