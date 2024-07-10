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
    REG32 Reserved_00000018;                    //0x00000018
    REG32                          FLOW_PARA;                    //0x0000001C
    REG32                          RF_SEL;                       //0x00000020
    REG32                          NB_LTE_SEL;                   //0x00000024
    REG32                          GGENB_SEL;                    //0x00000028
    REG32                          RF_ANA_26M_CTRL;              //0x0000002C
    REG32                          RF_ANA_26M_CTRL_SET;          //0x00000030
    REG32                          RF_ANA_26M_CTRL_CLR;          //0x00000034
    REG32                          DDR_SLP_CTRL_ENABLE;          //0x00000038
    REG32                          DDR_WAKEUP_FORCE_EN;          //0x0000003C
    REG32                          DDR_WAKEUP_FORCE_ACK;         //0x00000040
    REG32                          DDR_SLP_WAIT_NUMBER;          //0x00000044
    REG32                          LVDS_SPI_SEL;                 //0x00000048
    REG32                          MONITOR_LTE_FINT_SEL;         //0x0000004C
    REG32                          RFMUX_IRQ_STA;                //0x00000050
    REG32                          APT_SEL;                      //0x00000054
    REG32                          RFSPI_MODE_SEL;               //0x00000058
    REG32 Reserved_0000005C;                    //0x0000005C
    REG32                          GGENB_SYS_CTRL;               //0x00000060
    REG32                          A5_STANDBYWFI_EN;             //0x00000064
} HWP_CP_SYSREG_T;

#define hwp_sysreg                 ((HWP_CP_SYSREG_T*) REG_ACCESS_ADDRESS(REG_SYSREG_BASE))


//ZSP_SVTADDR
typedef union {
    REG32 v;
    struct {
        REG32 __7_0 : 8;
        REG32 svtaddr : 24; // [31:8]
    } b;
} REG_CP_SYSREG_ZSP_SVTADDR_T;

//MEM_EMA_CFG_ZSP
typedef union {
    REG32 v;
    struct {
        REG32 radpd_rmea : 1; // [0]
        REG32 radpd_rma : 4; // [4:1]
        REG32 radpd_rmeb : 1; // [5]
        REG32 radpd_rmb : 4; // [9:6]
        REG32 raspu_rme : 1; // [10]
        REG32 raspu_rm : 4; // [14:11]
        REG32 rfspd_rme : 1; // [15]
        REG32 rfspd_rm : 4; // [19:16]
        REG32 rftpd_rmea : 1; // [20]
        REG32 rftpd_rma : 4; // [24:21]
        REG32 rftpd_rmeb : 1; // [25]
        REG32 rftpd_rmb : 4; // [29:26]
        REG32 __31_30 : 2;
    } b;
} REG_CP_SYSREG_MEM_EMA_CFG_ZSP_T;

//MEM_EMA_CFG_LTE
typedef union {
    REG32 v;
    struct {
        REG32 radpd_rmea : 1; // [0]
        REG32 radpd_rma : 4; // [4:1]
        REG32 radpd_rmeb : 1; // [5]
        REG32 radpd_rmb : 4; // [9:6]
        REG32 raspu_rme : 1; // [10]
        REG32 raspu_rm : 4; // [14:11]
        REG32 rfspd_rme : 1; // [15]
        REG32 rfspd_rm : 4; // [19:16]
        REG32 rftpd_rmea : 1; // [20]
        REG32 rftpd_rma : 4; // [24:21]
        REG32 rftpd_rmeb : 1; // [25]
        REG32 rftpd_rmb : 4; // [29:26]
        REG32 __31_30 : 2;
    } b;
} REG_CP_SYSREG_MEM_EMA_CFG_LTE_T;

//ROM_EMA_CFG_LTE
typedef union {
    REG32 v;
    struct {
        REG32 rom_rmea : 1; // [0]
        REG32 rom_rma : 4; // [4:1]
        REG32 __31_5 : 27;
    } b;
} REG_CP_SYSREG_ROM_EMA_CFG_LTE_T;

//MEM_EMA_CFG_BBSYS
typedef union {
    REG32 v;
    struct {
        REG32 radpd_rmea : 1; // [0]
        REG32 radpd_rma : 4; // [4:1]
        REG32 radpd_rmeb : 1; // [5]
        REG32 radpd_rmb : 4; // [9:6]
        REG32 raspu_rme : 1; // [10]
        REG32 raspu_rm : 4; // [14:11]
        REG32 rfspd_rme : 1; // [15]
        REG32 rfspd_rm : 4; // [19:16]
        REG32 rftpd_rmea : 1; // [20]
        REG32 rftpd_rma : 4; // [24:21]
        REG32 rftpd_rmeb : 1; // [25]
        REG32 rftpd_rmb : 4; // [29:26]
        REG32 __31_30 : 2;
    } b;
} REG_CP_SYSREG_MEM_EMA_CFG_BBSYS_T;

//ZSP_QOS
typedef union {
    REG32 v;
    struct {
        REG32 arqos_zsp_dbus : 4; // [3:0]
        REG32 awqos_zsp_dbus : 4; // [7:4]
        REG32 arqos_zsp_ibus : 4; // [11:8]
        REG32 awqos_zsp_ibus : 4; // [15:12]
        REG32 arqos_zsp_axidma : 4; // [19:16]
        REG32 awqos_zsp_axidma : 4; // [23:20]
        REG32 __31_24 : 8;
    } b;
} REG_CP_SYSREG_ZSP_QOS_T;

//RF_SEL
typedef union {
    REG32 v;
    struct {
        REG32 rfsel : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_CP_SYSREG_RF_SEL_T;

//NB_LTE_SEL
typedef union {
    REG32 v;
    struct {
        REG32 nblte_sel : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_CP_SYSREG_NB_LTE_SEL_T;

//GGENB_SEL
typedef union {
    REG32 v;
    struct {
        REG32 gge_nb_sel : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_CP_SYSREG_GGENB_SEL_T;

//RF_ANA_26M_CTRL
typedef union {
    REG32 v;
    struct {
        REG32 enable_clk_26m_usb : 1; // [0]
        REG32 enable_clk_26m_dsi : 1; // [1]
        REG32 enable_clk_26m_audio : 1; // [2]
        REG32 muxsel_clk26m_audio : 1; // [3]
        REG32 enable_clk_26m_aud2ad : 1; // [4]
        REG32 muxsel_clk26m_aud2ad : 1; // [5]
        REG32 enable_clk_26m_vad : 1; // [6]
        REG32 enable_clk_26m_adi : 1; // [7]
        REG32 enable_clk_26m_wcn : 1; // [8]
        REG32 __31_9 : 23;
    } b;
} REG_CP_SYSREG_RF_ANA_26M_CTRL_T;

//RF_ANA_26M_CTRL_SET
typedef union {
    REG32 v;
    struct {
        REG32 set : 9; // [8:0], write set
        REG32 __31_9 : 23;
    } b;
} REG_CP_SYSREG_RF_ANA_26M_CTRL_SET_T;

//RF_ANA_26M_CTRL_CLR
typedef union {
    REG32 v;
    struct {
        REG32 clear : 9; // [8:0], write clear
        REG32 __31_9 : 23;
    } b;
} REG_CP_SYSREG_RF_ANA_26M_CTRL_CLR_T;

//DDR_SLP_CTRL_ENABLE
typedef union {
    REG32 v;
    struct {
        REG32 enable : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_CP_SYSREG_DDR_SLP_CTRL_ENABLE_T;

//DDR_WAKEUP_FORCE_EN
typedef union {
    REG32 v;
    struct {
        REG32 enable : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_CP_SYSREG_DDR_WAKEUP_FORCE_EN_T;

//DDR_WAKEUP_FORCE_ACK
typedef union {
    REG32 v;
    struct {
        REG32 ack : 1; // [0], read only
        REG32 __31_1 : 31;
    } b;
} REG_CP_SYSREG_DDR_WAKEUP_FORCE_ACK_T;

//DDR_SLP_WAIT_NUMBER
typedef union {
    REG32 v;
    struct {
        REG32 n : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_CP_SYSREG_DDR_SLP_WAIT_NUMBER_T;

//LVDS_SPI_SEL
typedef union {
    REG32 v;
    struct {
        REG32 sel : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_CP_SYSREG_LVDS_SPI_SEL_T;

//MONITOR_LTE_FINT_SEL
typedef union {
    REG32 v;
    struct {
        REG32 sel : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_CP_SYSREG_MONITOR_LTE_FINT_SEL_T;

//RFMUX_IRQ_STA
typedef union {
    REG32 v;
    struct {
        REG32 txfifo_full_irq : 1; // [0], write clear
        REG32 rfspi_conflict_irq : 1; // [1], write clear
        REG32 __31_2 : 30;
    } b;
} REG_CP_SYSREG_RFMUX_IRQ_STA_T;

//APT_SEL
typedef union {
    REG32 v;
    struct {
        REG32 sel : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_CP_SYSREG_APT_SEL_T;

//RFSPI_MODE_SEL
typedef union {
    REG32 v;
    struct {
        REG32 sel : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_CP_SYSREG_RFSPI_MODE_SEL_T;

//GGENB_SYS_CTRL
typedef union {
    REG32 v;
    struct {
        REG32 ggenb_ctrl : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_CP_SYSREG_GGENB_SYS_CTRL_T;

//A5_STANDBYWFI_EN
typedef union {
    REG32 v;
    struct {
        REG32 ap_a5_standby_en : 1; // [0]
        REG32 cp_a5_standby_en : 1; // [1]
        REG32 __31_2 : 30;
    } b;
} REG_CP_SYSREG_A5_STANDBYWFI_EN_T;


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

//FLOW_PARA
#define CP_SYSREG_FLOWPARA(n)      (((n)&0xFFFFFFFF)<<0)

//RF_SEL
#define CP_SYSREG_RESERVED_0_X4(n) (((n)&0x7FFFFFFF)<<1)
#define CP_SYSREG_RFSEL            (1<<0)

//NB_LTE_SEL
//#define CP_SYSREG_RESERVED_0_X4(n) (((n)&0x7FFFFFFF)<<1)
#define CP_SYSREG_NBLTE_SEL        (1<<0)

//GGENB_SEL
//#define CP_SYSREG_RESERVED_0_X4(n) (((n)&0x7FFFFFFF)<<1)
#define CP_SYSREG_GGE_NB_SEL       (1<<0)

//RF_ANA_26M_CTRL
#define CP_SYSREG_RESERVED_0_X5(n) (((n)&0x7FFFFF)<<9)
#define CP_SYSREG_ENABLE_CLK_26M_WCN (1<<8)
#define CP_SYSREG_ENABLE_CLK_26M_ADI (1<<7)
#define CP_SYSREG_ENABLE_CLK_26M_VAD (1<<6)
#define CP_SYSREG_MUXSEL_CLK26M_AUD2AD (1<<5)
#define CP_SYSREG_ENABLE_CLK_26M_AUD2AD (1<<4)
#define CP_SYSREG_MUXSEL_CLK26M_AUDIO (1<<3)
#define CP_SYSREG_ENABLE_CLK_26M_AUDIO (1<<2)
#define CP_SYSREG_ENABLE_CLK_26M_DSI (1<<1)
#define CP_SYSREG_ENABLE_CLK_26M_USB (1<<0)

//RF_ANA_26M_CTRL_SET
//#define CP_SYSREG_RESERVED_0_X5(n) (((n)&0x7FFFFF)<<9)
#define CP_SYSREG_SET(n)           (((n)&0x1FF)<<0)

//RF_ANA_26M_CTRL_CLR
//#define CP_SYSREG_RESERVED_0_X5(n) (((n)&0x7FFFFF)<<9)
#define CP_SYSREG_CLEAR(n)         (((n)&0x1FF)<<0)

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
#define CP_SYSREG_RFSPI_CONFLICT_IRQ (1<<1)
#define CP_SYSREG_TXFIFO_FULL_IRQ  (1<<0)

//APT_SEL
//#define CP_SYSREG_RESERVED_0_X4(n) (((n)&0x7FFFFFFF)<<1)
//#define CP_SYSREG_SEL            (1<<0)

//RFSPI_MODE_SEL
//#define CP_SYSREG_RESERVED_0_X4(n) (((n)&0x7FFFFFFF)<<1)
//#define CP_SYSREG_SEL            (1<<0)

//GGENB_SYS_CTRL
//#define CP_SYSREG_RESERVED_0_X6(n) (((n)&0xFFFF)<<16)
#define CP_SYSREG_GGENB_CTRL(n)    (((n)&0xFFFF)<<0)

//A5_STANDBYWFI_EN
//#define CP_SYSREG_RESERVED_0_X7(n) (((n)&0x3FFFFFFF)<<2)
#define CP_SYSREG_CP_A5_STANDBY_EN (1<<1)
#define CP_SYSREG_AP_A5_STANDBY_EN (1<<0)





#endif

