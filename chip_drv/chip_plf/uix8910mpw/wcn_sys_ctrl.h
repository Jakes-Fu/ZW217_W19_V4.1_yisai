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


#ifndef _WCN_SYS_CTRL_H_
#define _WCN_SYS_CTRL_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'wcn_sys_ctrl'."
#endif



// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// WCN_SYS_CTRL_T
// -----------------------------------------------------------------------------
///
// =============================================================================
#define REG_WCN_SYS_CTRL_BASE      0x15000000

typedef volatile struct
{
    REG32                          soft_reset;                   //0x00000000
    REG32                          clk_cfg;                      //0x00000004
    REG32                          clk_cfg1;                     //0x00000008
    REG32                          clk_208m_cfg;                 //0x0000000C
    REG32                          sys_conn;                     //0x00000010
    REG32                          riscv_boot_addr;              //0x00000014
    REG32                          rf_cfg;                       //0x00000018
    REG32                          lvds_cfg;                     //0x0000001C
    REG32                          debug_config;                 //0x00000020
    REG32                          wakeup_ctrl_0;                //0x00000024
    REG32                          wakeup_ctrl_1;                //0x00000028
    REG32                          wakeup_status;                //0x0000002C
    REG32                          extmem_offset;                //0x00000030
    REG32                          wcn2sys_offset;               //0x00000034
    REG32                          audio_out_cfg;                //0x00000038
    REG32                          pulp_tag_mem_cfg;             //0x0000003C
    REG32                          pulp_data_mem_cfg;            //0x00000040
    REG32                          wcn_rom_mem_cfg;              //0x00000044
    REG32                          wcn_dram_mem_cfg;             //0x00000048
    REG32                          wcn_bram_mem_cfg;             //0x0000004C
    REG32                          bt_link_mem_cfg;              //0x00000050
    REG32                          bt_modem_mem_cfg;             //0x00000054
    REG32                          wlan_mem_cfg;                 //0x00000058
    REG32                          plc_mem_cfg;                  //0x0000005C
    REG32                          hclk_freq;                    //0x00000060
    REG32                          branch_addr;                  //0x00000064
    REG32                          revision_id;                  //0x00000068
    REG32 Reserved_0000006C;                    //0x0000006C
    REG32                          simu_rsvd;                    //0x00000070
} HWP_WCN_SYS_CTRL_T;

#define hwp_wcnSysCtrl             ((HWP_WCN_SYS_CTRL_T*) REG_ACCESS_ADDRESS(REG_WCN_SYS_CTRL_BASE))


//soft_reset
typedef union {
    REG32 v;
    struct {
        REG32 reg_apb_presetn : 1; // [0]
        REG32 reg_mem_hresetn : 1; // [1]
        REG32 reg_pulp_mcu_hresetn : 1; // [2]
        REG32 reg_jtag_hresetn : 1; // [3]
        REG32 reg_sys_ifc_hresetn : 1; // [4]
        REG32 reg_aud_ifc_hresetn : 1; // [5]
        REG32 reg_bt_dbm_hresetn : 1; // [6]
        REG32 reg_bt_master_rstb : 1; // [7]
        REG32 reg_wdt_clk_rstb : 1; // [8]
        REG32 reg_uart_clk_rstb : 1; // [9]
        REG32 reg_low_power_rstb : 1; // [10]
        REG32 reg_bt_hresetn : 1; // [11]
        REG32 reg_wakeup_ctrl_rstb : 1; // [12]
        REG32 reg_wdt_rst_sys_en : 1; // [13]
        REG32 reg_pulp_dbg_rstb : 1; // [14]
        REG32 reg_dbg_hst_uart_rstb : 1; // [15]
        REG32 __30_16 : 15;
        REG32 reg_sys_resetb : 1; // [31]
    } b;
} REG_WCN_SYS_CTRL_SOFT_RESET_T;

//clk_cfg
typedef union {
    REG32 v;
    struct {
        REG32 reg_mem_auto_ckg : 1; // [0]
        REG32 reg_apb_reg_clk_force_on : 1; // [1]
        REG32 reg_pulp_mcu_hclk_en : 1; // [2]
        REG32 reg_wdt_clk_en : 1; // [3]
        REG32 reg_uart_sys_clk_force_on : 1; // [4]
        REG32 reg_uart_clk_force_on : 1; // [5]
        REG32 reg_bt_dbm_hclk_en : 1; // [6]
        REG32 reg_bt_hclk_en : 1; // [7]
        REG32 reg_mem_hclk_en : 1; // [8]
        REG32 reg_aud_ifc_ch2_hclk_force_on : 1; // [9]
        REG32 reg_aud_ifc_hclk_force_on : 1; // [10]
        REG32 reg_sys_ifc_ch_hclk_force_on : 2; // [12:11]
        REG32 reg_sys_ifc_hclk_force_on : 1; // [13]
        REG32 reg_jtag_hclk_en : 1; // [14]
        REG32 reg_low_power_clk_en : 1; // [15]
        REG32 reg_master_clk_en : 1; // [16]
        REG32 reg_bt_master_clk_ld : 1; // [17], write set
        REG32 reg_bt_master_clk_denom : 4; // [21:18]
        REG32 bt_clksel : 6; // [27:22]
        REG32 reg_wakeup_ctrl_clk_en : 1; // [28]
        REG32 reg_bus_clk_sel : 3; // [31:29]
    } b;
} REG_WCN_SYS_CTRL_CLK_CFG_T;

//clk_cfg1
typedef union {
    REG32 v;
    struct {
        REG32 reg_uart_clk_denom : 13; // [12:0]
        REG32 reg_uart_clk_div_en : 1; // [13]
        REG32 reg_uart_clk_div_ld_cfg : 1; // [14], write set
        REG32 dbg_hst_uart_clk_en_force_on : 1; // [15]
        REG32 dbg_uart_sys_clk_en_force_on : 1; // [16]
        REG32 dbg_hst_sys_clk_en_force_on : 1; // [17]
        REG32 sysifc_dbg_hclk_clk_en_force_on : 1; // [18]
        REG32 reg_bt_clk_en : 1; // [19]
        REG32 reg_bt_master_clk_sel : 2; // [21:20]
        REG32 reg_bt_13m_clk_en : 1; // [22]
        REG32 __31_23 : 9;
    } b;
} REG_WCN_SYS_CTRL_CLK_CFG1_T;

//clk_208m_cfg
typedef union {
    REG32 v;
    struct {
        REG32 reg_clk_208m_denom : 4; // [3:0]
        REG32 reg_clk_208m_num : 4; // [7:4]
        REG32 reg_clk_208m_div_en : 1; // [8]
        REG32 reg_clk_208m_div_ld_cfg : 1; // [9], write set
        REG32 reg_dbg_hst_div_en : 1; // [10]
        REG32 reg_dbg_hst_uart_clk_denom : 10; // [20:11]
        REG32 reg_dbg_hst_uart_clk_num : 10; // [30:21]
        REG32 reg_dbg_hst_uart_clk_ld_cfg : 1; // [31], write set
    } b;
} REG_WCN_SYS_CTRL_CLK_208M_CFG_T;

//sys_conn
typedef union {
    REG32 v;
    struct {
        REG32 bt2sys_soft_int : 1; // [0]
        REG32 bt2sys_wakeup : 1; // [1]
        REG32 reg_wdt_gen_irq2sys_en : 1; // [2]
        REG32 sys2bt_irq : 1; // [3], read only
        REG32 __31_4 : 28;
    } b;
} REG_WCN_SYS_CTRL_SYS_CONN_T;

//rf_cfg
typedef union {
    REG32 v;
    struct {
        REG32 rf_reg_src_sel : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_WCN_SYS_CTRL_RF_CFG_T;

//lvds_cfg
typedef union {
    REG32 v;
    struct {
        REG32 reg_lvds_adc : 1; // [0]
        REG32 reg_lvds_dac : 1; // [1]
        REG32 reg_lvds_out_sel : 1; // [2]
        REG32 reg_lvds_rx_mode : 2; // [4:3]
        REG32 sdm_clk_sel : 1; // [5]
        REG32 sdm_clk_div_sel : 1; // [6]
        REG32 __31_7 : 25;
    } b;
} REG_WCN_SYS_CTRL_LVDS_CFG_T;

//debug_config
typedef union {
    REG32 v;
    struct {
        REG32 reg_dbg_out_sel : 4; // [3:0]
        REG32 reg_dbg_out_en : 1; // [4]
        REG32 reg_dbg_out_nibble_swap : 1; // [5]
        REG32 reg_dbg_out_byte_swap : 1; // [6]
        REG32 reg_dbg_out_nibble_shift : 1; // [7]
        REG32 reg_dbg_out_nibble_shift_mode : 1; // [8]
        REG32 __21_9 : 13;
        REG32 reg_dbg_clk_en : 1; // [22]
        REG32 reg_dbg_clk_sel : 4; // [26:23]
        REG32 reg_dbg_trig_en : 1; // [27]
        REG32 reg_dbg_trig_sel : 4; // [31:28]
    } b;
} REG_WCN_SYS_CTRL_DEBUG_CONFIG_T;

//wakeup_ctrl_0
typedef union {
    REG32 v;
    struct {
        REG32 bt2host_wakeup_period : 10; // [9:0]
        REG32 bt2host_wakeup_mode : 1; // [10]
        REG32 host2bt_hci_activity_wakeup_en : 1; // [11]
        REG32 host2bt_hci_break_wakeup_en : 1; // [12]
        REG32 host2bt_ext_wakeup_en : 1; // [13]
        REG32 __31_14 : 18;
    } b;
} REG_WCN_SYS_CTRL_WAKEUP_CTRL_0_T;

//wakeup_ctrl_1
typedef union {
    REG32 v;
    struct {
        REG32 bt2host_wakeup_trig : 1; // [0], write set
        REG32 __31_1 : 31;
    } b;
} REG_WCN_SYS_CTRL_WAKEUP_CTRL_1_T;

//wakeup_status
typedef union {
    REG32 v;
    struct {
        REG32 bt2host_wakeup_status : 1; // [0], read only
        REG32 __31_1 : 31;
    } b;
} REG_WCN_SYS_CTRL_WAKEUP_STATUS_T;

//audio_out_cfg
typedef union {
    REG32 v;
    struct {
        REG32 reg_i2s_sel : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_WCN_SYS_CTRL_AUDIO_OUT_CFG_T;

//pulp_tag_mem_cfg
typedef union {
    REG32 v;
    struct {
        REG32 tag_mem_cfg_shrink : 12; // [11:0]
        REG32 __31_12 : 20;
    } b;
} REG_WCN_SYS_CTRL_PULP_TAG_MEM_CFG_T;

//pulp_data_mem_cfg
typedef union {
    REG32 v;
    struct {
        REG32 data_mem_cfg_shrink : 12; // [11:0]
        REG32 __31_12 : 20;
    } b;
} REG_WCN_SYS_CTRL_PULP_DATA_MEM_CFG_T;

//wcn_rom_mem_cfg
typedef union {
    REG32 v;
    struct {
        REG32 rom_mem_cfg_shrink : 11; // [10:0]
        REG32 __31_11 : 21;
    } b;
} REG_WCN_SYS_CTRL_WCN_ROM_MEM_CFG_T;

//wcn_dram_mem_cfg
typedef union {
    REG32 v;
    struct {
        REG32 dram_mem_cfg_shrink : 12; // [11:0]
        REG32 __31_12 : 20;
    } b;
} REG_WCN_SYS_CTRL_WCN_DRAM_MEM_CFG_T;

//wcn_bram_mem_cfg
typedef union {
    REG32 v;
    struct {
        REG32 bram_mem_cfg_shrink : 12; // [11:0]
        REG32 __31_12 : 20;
    } b;
} REG_WCN_SYS_CTRL_WCN_BRAM_MEM_CFG_T;

//bt_link_mem_cfg
typedef union {
    REG32 v;
    struct {
        REG32 bt_link_mem_cfg_shrink : 12; // [11:0]
        REG32 __31_12 : 20;
    } b;
} REG_WCN_SYS_CTRL_BT_LINK_MEM_CFG_T;

//bt_modem_mem_cfg
typedef union {
    REG32 v;
    struct {
        REG32 bt_modem_mem_cfg_shrink : 12; // [11:0]
        REG32 __31_12 : 20;
    } b;
} REG_WCN_SYS_CTRL_BT_MODEM_MEM_CFG_T;

//wlan_mem_cfg
typedef union {
    REG32 v;
    struct {
        REG32 wlan_mem_cfg_shrink : 17; // [16:0]
        REG32 __31_17 : 15;
    } b;
} REG_WCN_SYS_CTRL_WLAN_MEM_CFG_T;

//plc_mem_cfg
typedef union {
    REG32 v;
    struct {
        REG32 plc_mem_cfg_shrink : 12; // [11:0]
        REG32 __31_12 : 20;
    } b;
} REG_WCN_SYS_CTRL_PLC_MEM_CFG_T;

//hclk_freq
typedef union {
    REG32 v;
    struct {
        REG32 wcn_hclk_freq : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_WCN_SYS_CTRL_HCLK_FREQ_T;

//revision_id
typedef union {
    REG32 v;
    struct {
        REG32 wcn_id : 8; // [7:0], read only
        REG32 __31_8 : 24;
    } b;
} REG_WCN_SYS_CTRL_REVISION_ID_T;

//simu_rsvd
typedef union {
    REG32 v;
    struct {
        REG32 simu_rsvd_0 : 8; // [7:0]
        REG32 simu_rsvd_1 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_WCN_SYS_CTRL_SIMU_RSVD_T;


//soft_reset
#define WCN_SYS_CTRL_REG_SYS_RESETB (1<<31)
#define WCN_SYS_CTRL_SOFT_RESET_RESERVED_0(n) (((n)&0x7FFF)<<16)
#define WCN_SYS_CTRL_REG_DBG_HST_UART_RSTB (1<<15)
#define WCN_SYS_CTRL_REG_PULP_DBG_RSTB (1<<14)
#define WCN_SYS_CTRL_REG_WDT_RST_SYS_EN (1<<13)
#define WCN_SYS_CTRL_REG_WAKEUP_CTRL_RSTB (1<<12)
#define WCN_SYS_CTRL_REG_BT_HRESETN (1<<11)
#define WCN_SYS_CTRL_REG_LOW_POWER_RSTB (1<<10)
#define WCN_SYS_CTRL_REG_UART_CLK_RSTB (1<<9)
#define WCN_SYS_CTRL_REG_WDT_CLK_RSTB (1<<8)
#define WCN_SYS_CTRL_REG_BT_MASTER_RSTB (1<<7)
#define WCN_SYS_CTRL_REG_BT_DBM_HRESETN (1<<6)
#define WCN_SYS_CTRL_REG_AUD_IFC_HRESETN (1<<5)
#define WCN_SYS_CTRL_REG_SYS_IFC_HRESETN (1<<4)
#define WCN_SYS_CTRL_REG_JTAG_HRESETN (1<<3)
#define WCN_SYS_CTRL_REG_PULP_MCU_HRESETN (1<<2)
#define WCN_SYS_CTRL_REG_MEM_HRESETN (1<<1)
#define WCN_SYS_CTRL_REG_APB_PRESETN (1<<0)

//clk_cfg
#define WCN_SYS_CTRL_REG_BUS_CLK_SEL(n) (((n)&7)<<29)
#define WCN_SYS_CTRL_REG_WAKEUP_CTRL_CLK_EN (1<<28)
#define WCN_SYS_CTRL_BT_CLKSEL(n)  (((n)&0x3F)<<22)
#define WCN_SYS_CTRL_REG_BT_MASTER_CLK_DENOM(n) (((n)&15)<<18)
#define WCN_SYS_CTRL_REG_BT_MASTER_CLK_LD (1<<17)
#define WCN_SYS_CTRL_REG_MASTER_CLK_EN (1<<16)
#define WCN_SYS_CTRL_REG_LOW_POWER_CLK_EN (1<<15)
#define WCN_SYS_CTRL_REG_JTAG_HCLK_EN (1<<14)
#define WCN_SYS_CTRL_REG_SYS_IFC_HCLK_FORCE_ON (1<<13)
#define WCN_SYS_CTRL_REG_SYS_IFC_CH_HCLK_FORCE_ON(n) (((n)&3)<<11)
#define WCN_SYS_CTRL_REG_AUD_IFC_HCLK_FORCE_ON (1<<10)
#define WCN_SYS_CTRL_REG_AUD_IFC_CH2_HCLK_FORCE_ON (1<<9)
#define WCN_SYS_CTRL_REG_MEM_HCLK_EN (1<<8)
#define WCN_SYS_CTRL_REG_BT_HCLK_EN (1<<7)
#define WCN_SYS_CTRL_REG_BT_DBM_HCLK_EN (1<<6)
#define WCN_SYS_CTRL_REG_UART_CLK_FORCE_ON (1<<5)
#define WCN_SYS_CTRL_REG_UART_SYS_CLK_FORCE_ON (1<<4)
#define WCN_SYS_CTRL_REG_WDT_CLK_EN (1<<3)
#define WCN_SYS_CTRL_REG_PULP_MCU_HCLK_EN (1<<2)
#define WCN_SYS_CTRL_REG_APB_REG_CLK_FORCE_ON (1<<1)
#define WCN_SYS_CTRL_REG_MEM_AUTO_CKG (1<<0)

//clk_cfg1
#define WCN_SYS_CTRL_CLK_CFG1_RESERVED_0(n) (((n)&0x1FF)<<23)
#define WCN_SYS_CTRL_REG_BT_13M_CLK_EN (1<<22)
#define WCN_SYS_CTRL_REG_BT_MASTER_CLK_SEL(n) (((n)&3)<<20)
#define WCN_SYS_CTRL_REG_BT_CLK_EN (1<<19)
#define WCN_SYS_CTRL_SYSIFC_DBG_HCLK_CLK_EN_FORCE_ON (1<<18)
#define WCN_SYS_CTRL_DBG_HST_SYS_CLK_EN_FORCE_ON (1<<17)
#define WCN_SYS_CTRL_DBG_UART_SYS_CLK_EN_FORCE_ON (1<<16)
#define WCN_SYS_CTRL_DBG_HST_UART_CLK_EN_FORCE_ON (1<<15)
#define WCN_SYS_CTRL_REG_UART_CLK_DIV_LD_CFG (1<<14)
#define WCN_SYS_CTRL_REG_UART_CLK_DIV_EN (1<<13)
#define WCN_SYS_CTRL_REG_UART_CLK_DENOM(n) (((n)&0x1FFF)<<0)

//clk_208m_cfg
#define WCN_SYS_CTRL_REG_DBG_HST_UART_CLK_LD_CFG (1<<31)
#define WCN_SYS_CTRL_REG_DBG_HST_UART_CLK_NUM(n) (((n)&0x3FF)<<21)
#define WCN_SYS_CTRL_REG_DBG_HST_UART_CLK_DENOM(n) (((n)&0x3FF)<<11)
#define WCN_SYS_CTRL_REG_DBG_HST_DIV_EN (1<<10)
#define WCN_SYS_CTRL_REG_CLK_208M_DIV_LD_CFG (1<<9)
#define WCN_SYS_CTRL_REG_CLK_208M_DIV_EN (1<<8)
#define WCN_SYS_CTRL_REG_CLK_208M_NUM(n) (((n)&15)<<4)
#define WCN_SYS_CTRL_REG_CLK_208M_DENOM(n) (((n)&15)<<0)

//sys_conn
#define WCN_SYS_CTRL_SYS_CONN_RESERVED_0(n) (((n)&0xFFFFFFF)<<4)
#define WCN_SYS_CTRL_SYS2BT_IRQ    (1<<3)
#define WCN_SYS_CTRL_REG_WDT_GEN_IRQ2SYS_EN (1<<2)
#define WCN_SYS_CTRL_BT2SYS_WAKEUP (1<<1)
#define WCN_SYS_CTRL_BT2SYS_SOFT_INT (1<<0)

//riscv_boot_addr
#define WCN_SYS_CTRL_PULP_MCU_BOOT_ADDR(n) (((n)&0xFFFFFFFF)<<0)

//rf_cfg
#define WCN_SYS_CTRL_RF_CFG_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define WCN_SYS_CTRL_RF_REG_SRC_SEL (1<<0)

//lvds_cfg
#define WCN_SYS_CTRL_LVDS_CFG_RESERVED_0(n) (((n)&0x1FFFFFF)<<7)
#define WCN_SYS_CTRL_SDM_CLK_DIV_SEL (1<<6)
#define WCN_SYS_CTRL_SDM_CLK_SEL   (1<<5)
#define WCN_SYS_CTRL_REG_LVDS_RX_MODE(n) (((n)&3)<<3)
#define WCN_SYS_CTRL_REG_LVDS_OUT_SEL (1<<2)
#define WCN_SYS_CTRL_REG_LVDS_DAC  (1<<1)
#define WCN_SYS_CTRL_REG_LVDS_ADC  (1<<0)

//debug_config
#define WCN_SYS_CTRL_REG_DBG_TRIG_SEL(n) (((n)&15)<<28)
#define WCN_SYS_CTRL_REG_DBG_TRIG_EN (1<<27)
#define WCN_SYS_CTRL_REG_DBG_CLK_SEL(n) (((n)&15)<<23)
#define WCN_SYS_CTRL_REG_DBG_CLK_EN (1<<22)
#define WCN_SYS_CTRL_DEBUG_CONFIG_RESERVED_0(n) (((n)&0x1FFF)<<9)
#define WCN_SYS_CTRL_REG_DBG_OUT_NIBBLE_SHIFT_MODE (1<<8)
#define WCN_SYS_CTRL_REG_DBG_OUT_NIBBLE_SHIFT (1<<7)
#define WCN_SYS_CTRL_REG_DBG_OUT_BYTE_SWAP (1<<6)
#define WCN_SYS_CTRL_REG_DBG_OUT_NIBBLE_SWAP (1<<5)
#define WCN_SYS_CTRL_REG_DBG_OUT_EN (1<<4)
#define WCN_SYS_CTRL_REG_DBG_OUT_SEL(n) (((n)&15)<<0)

//wakeup_ctrl_0
#define WCN_SYS_CTRL_WAKEUP_CTRL_0_RESERVED_0(n) (((n)&0x3FFFF)<<14)
#define WCN_SYS_CTRL_HOST2BT_EXT_WAKEUP_EN (1<<13)
#define WCN_SYS_CTRL_HOST2BT_HCI_BREAK_WAKEUP_EN (1<<12)
#define WCN_SYS_CTRL_HOST2BT_HCI_ACTIVITY_WAKEUP_EN (1<<11)
#define WCN_SYS_CTRL_BT2HOST_WAKEUP_MODE (1<<10)
#define WCN_SYS_CTRL_BT2HOST_WAKEUP_PERIOD(n) (((n)&0x3FF)<<0)

//wakeup_ctrl_1
#define WCN_SYS_CTRL_WAKEUP_CTRL_1_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define WCN_SYS_CTRL_BT2HOST_WAKEUP_TRIG (1<<0)

//wakeup_status
#define WCN_SYS_CTRL_WAKEUP_STATUS_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define WCN_SYS_CTRL_BT2HOST_WAKEUP_STATUS (1<<0)

//extmem_offset
#define WCN_SYS_CTRL_REG_EXTMEM_OFFSET_ADDR(n) (((n)&0xFFFFFFFF)<<0)

//wcn2sys_offset
#define WCN_SYS_CTRL_REG_WCN2SYS_OFFSET_ADDR(n) (((n)&0xFFFFFFFF)<<0)

//audio_out_cfg
#define WCN_SYS_CTRL_AUDIO_OUT_CFG_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define WCN_SYS_CTRL_REG_I2S_SEL   (1<<0)

//pulp_tag_mem_cfg
#define WCN_SYS_CTRL_PULP_TAG_MEM_CFG_RESERVED_0(n) (((n)&0xFFFFF)<<12)
#define WCN_SYS_CTRL_TAG_MEM_CFG_SHRINK(n) (((n)&0xFFF)<<0)

//pulp_data_mem_cfg
#define WCN_SYS_CTRL_PULP_DATA_MEM_CFG_RESERVED_0(n) (((n)&0xFFFFF)<<12)
#define WCN_SYS_CTRL_DATA_MEM_CFG_SHRINK(n) (((n)&0xFFF)<<0)

//wcn_rom_mem_cfg
#define WCN_SYS_CTRL_WCN_ROM_MEM_CFG_RESERVED_0(n) (((n)&0x1FFFFF)<<11)
#define WCN_SYS_CTRL_ROM_MEM_CFG_SHRINK(n) (((n)&0x7FF)<<0)

//wcn_dram_mem_cfg
#define WCN_SYS_CTRL_WCN_DRAM_MEM_CFG_RESERVED_0(n) (((n)&0xFFFFF)<<12)
#define WCN_SYS_CTRL_DRAM_MEM_CFG_SHRINK(n) (((n)&0xFFF)<<0)

//wcn_bram_mem_cfg
#define WCN_SYS_CTRL_WCN_BRAM_MEM_CFG_RESERVED_0(n) (((n)&0xFFFFF)<<12)
#define WCN_SYS_CTRL_BRAM_MEM_CFG_SHRINK(n) (((n)&0xFFF)<<0)

//bt_link_mem_cfg
#define WCN_SYS_CTRL_BT_LINK_MEM_CFG_RESERVED_0(n) (((n)&0xFFFFF)<<12)
#define WCN_SYS_CTRL_BT_LINK_MEM_CFG_SHRINK(n) (((n)&0xFFF)<<0)

//bt_modem_mem_cfg
#define WCN_SYS_CTRL_BT_MODEM_MEM_CFG_RESERVED_0(n) (((n)&0xFFFFF)<<12)
#define WCN_SYS_CTRL_BT_MODEM_MEM_CFG_SHRINK(n) (((n)&0xFFF)<<0)

//wlan_mem_cfg
#define WCN_SYS_CTRL_WLAN_MEM_CFG_RESERVED_0(n) (((n)&0x7FFF)<<17)
#define WCN_SYS_CTRL_WLAN_MEM_CFG_SHRINK(n) (((n)&0x1FFFF)<<0)

//plc_mem_cfg
#define WCN_SYS_CTRL_PLC_MEM_CFG_RESERVED_0(n) (((n)&0xFFFFF)<<12)
#define WCN_SYS_CTRL_PLC_MEM_CFG_SHRINK(n) (((n)&0xFFF)<<0)

//hclk_freq
#define WCN_SYS_CTRL_HCLK_FREQ_RESERVED_0(n) (((n)&0xFFFFFF)<<8)
#define WCN_SYS_CTRL_WCN_HCLK_FREQ(n) (((n)&0xFF)<<0)

//branch_addr
#define WCN_SYS_CTRL_RISC_BRANCH_ADDR(n) (((n)&0xFFFFFFFF)<<0)

//revision_id
#define WCN_SYS_CTRL_REVISION_ID_RESERVED_0(n) (((n)&0xFFFFFF)<<8)
#define WCN_SYS_CTRL_WCN_ID(n)     (((n)&0xFF)<<0)

//simu_rsvd
#define WCN_SYS_CTRL_SIMU_RSVD_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define WCN_SYS_CTRL_SIMU_RSVD_1(n) (((n)&0xFF)<<8)
#define WCN_SYS_CTRL_SIMU_RSVD_0(n) (((n)&0xFF)<<0)





#endif

