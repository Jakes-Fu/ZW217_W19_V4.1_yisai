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


#ifndef _SYS_CTRL_H_
#define _SYS_CTRL_H_

#include "globals.h"
#include "uws6121e_module_ver_extern.h"

#define REG_SYS_CTRL_SET_OFFSET (1024)
#define REG_SYS_CTRL_CLR_OFFSET (2048)

#define REG_SYS_CTRL_BASE (0x51500000)

typedef volatile struct
{
    REG32 aon_soft_rst_ctrl0;                // 0x00000000
    REG32 clken_lte;                         // 0x00000004
    REG32 clken_lte_intf;                    // 0x00000008
    REG32 rstctrl_lte;                       // 0x0000000c
    REG32 lte_autogate_mode;                 // 0x00000010
    REG32 lte_autogate_en;                   // 0x00000014
    REG32 lte_autogate_delay_num;            // 0x00000018
    REG32 aon_lpc_ctrl;                      // 0x0000001c
    REG32 aon_clock_en0;                     // 0x00000020
    REG32 aon_clock_en1;                     // 0x00000024
    REG32 aon_clock_auto_sel0;               // 0x00000028
    REG32 aon_clock_auto_sel1;               // 0x0000002c
    REG32 aon_clock_auto_sel2;               // 0x00000030
    REG32 aon_clock_auto_sel3;               // 0x00000034
    REG32 aon_clock_force_en0;               // 0x00000038
    REG32 aon_clock_force_en1;               // 0x0000003c
    REG32 aon_clock_force_en2;               // 0x00000040
    REG32 aon_clock_force_en3;               // 0x00000044
    REG32 aon_soft_rst_ctrl1;                // 0x00000048
    REG32 mipi_csi_cfg_reg;                  // 0x0000004c
    REG32 cfg_clk_uart2;                     // 0x00000050
    REG32 cfg_clk_uart3;                     // 0x00000054
    REG32 cfg_clk_debug_host;                // 0x00000058
    REG32 __92[1];                           // 0x0000005c
    REG32 rc_calib_ctrl;                     // 0x00000060
    REG32 rc_calib_th_val;                   // 0x00000064
    REG32 rc_calib_out_val;                  // 0x00000068
    REG32 emmc_slice_phy_ctrl;               // 0x0000006c
    REG32 dma_req_ctrl;                      // 0x00000070
    REG32 apt_trigger_sel;                   // 0x00000074
    REG32 ahb2ahb_ab_funcdma_ctrl;           // 0x00000078
    REG32 ahb2ahb_ab_funcdma_sts;            // 0x0000007c
    REG32 ahb2ahb_ab_dap_ctrl;               // 0x00000080
    REG32 ahb2ahb_ab_dap_sts;                // 0x00000084
    REG32 ahb2axi_pub_ctrl;                  // 0x00000088
    REG32 ahb2axi_pub_sts;                   // 0x0000008c
    REG32 axi2axi_pub_sts_0;                 // 0x00000090
    REG32 axi2axi_pub_sts_1;                 // 0x00000094
    REG32 ahb2ahb_ab_aon2lps_ctrl;           // 0x00000098
    REG32 ahb2ahb_ab_aon2lps_sts;            // 0x0000009c
    REG32 ahb2ahb_ab_lps2aon_ctrl;           // 0x000000a0
    REG32 ahb2ahb_ab_lps2aon_sts;            // 0x000000a4
    REG32 sysctrl_reg0;                      // 0x000000a8
    REG32 plls_sts;                          // 0x000000ac
    REG32 cfg_aon_anti_hang;                 // 0x000000b0
    REG32 cfg_aon_qos;                       // 0x000000b4
    REG32 aon_ahb_mtx_slice_autogate_en;     // 0x000000b8
    REG32 dap_djtag_en_cfg;                  // 0x000000bc
    REG32 lte_ahb2ahb_sync_cfg;              // 0x000000c0
    REG32 cfg_aon_io_core_ie_0;              // 0x000000c4
    REG32 cfg_aon_io_core_ie_1;              // 0x000000c8
    REG32 cfg_aon_io_core_ie_2;              // 0x000000cc
    REG32 cfg_aon_io_core_ie_3;              // 0x000000d0
    REG32 __212[203];                        // 0x000000d4
    REG32 aon_soft_rst_ctrl0_set;            // 0x00000400
    REG32 clken_lte_set;                     // 0x00000404
    REG32 clken_lte_intf_set;                // 0x00000408
    REG32 rstctrl_lte_set;                   // 0x0000040c
    REG32 __1040[1];                         // 0x00000410
    REG32 lte_autogate_en_set;               // 0x00000414
    REG32 __1048[1];                         // 0x00000418
    REG32 aon_lpc_ctrl_set;                  // 0x0000041c
    REG32 aon_clock_en0_set;                 // 0x00000420
    REG32 aon_clock_en1_set;                 // 0x00000424
    REG32 aon_clock_auto_sel0_set;           // 0x00000428
    REG32 aon_clock_auto_sel1_set;           // 0x0000042c
    REG32 aon_clock_auto_sel2_set;           // 0x00000430
    REG32 aon_clock_auto_sel3_set;           // 0x00000434
    REG32 aon_clock_force_en0_set;           // 0x00000438
    REG32 aon_clock_force_en1_set;           // 0x0000043c
    REG32 aon_clock_force_en2_set;           // 0x00000440
    REG32 aon_clock_force_en3_set;           // 0x00000444
    REG32 aon_soft_rst_ctrl1_set;            // 0x00000448
    REG32 mipi_csi_cfg_reg_set;              // 0x0000044c
    REG32 cfg_clk_uart2_set;                 // 0x00000450
    REG32 cfg_clk_uart3_set;                 // 0x00000454
    REG32 cfg_clk_debug_host_set;            // 0x00000458
    REG32 __1116[1];                         // 0x0000045c
    REG32 rc_calib_ctrl_set;                 // 0x00000460
    REG32 __1124[2];                         // 0x00000464
    REG32 emmc_slice_phy_ctrl_set;           // 0x0000046c
    REG32 dma_req_ctrl_set;                  // 0x00000470
    REG32 apt_trigger_sel_set;               // 0x00000474
    REG32 ahb2ahb_ab_funcdma_ctrl_set;       // 0x00000478
    REG32 __1148[1];                         // 0x0000047c
    REG32 ahb2ahb_ab_dap_ctrl_set;           // 0x00000480
    REG32 __1156[1];                         // 0x00000484
    REG32 ahb2axi_pub_ctrl_set;              // 0x00000488
    REG32 __1164[3];                         // 0x0000048c
    REG32 ahb2ahb_ab_aon2lps_ctrl_set;       // 0x00000498
    REG32 __1180[1];                         // 0x0000049c
    REG32 ahb2ahb_ab_lps2aon_ctrl_set;       // 0x000004a0
    REG32 __1188[1];                         // 0x000004a4
    REG32 sysctrl_reg0_set;                  // 0x000004a8
    REG32 __1196[1];                         // 0x000004ac
    REG32 cfg_aon_anti_hang_set;             // 0x000004b0
    REG32 __1204[1];                         // 0x000004b4
    REG32 aon_ahb_mtx_slice_autogate_en_set; // 0x000004b8
    REG32 dap_djtag_en_cfg_set;              // 0x000004bc
    REG32 lte_ahb2ahb_sync_cfg_set;          // 0x000004c0
    REG32 cfg_aon_io_core_ie_0_set;          // 0x000004c4
    REG32 cfg_aon_io_core_ie_1_set;          // 0x000004c8
    REG32 cfg_aon_io_core_ie_2_set;          // 0x000004cc
    REG32 cfg_aon_io_core_ie_3_set;          // 0x000004d0
    REG32 __1236[203];                       // 0x000004d4
    REG32 aon_soft_rst_ctrl0_clr;            // 0x00000800
    REG32 clken_lte_clr;                     // 0x00000804
    REG32 clken_lte_intf_clr;                // 0x00000808
    REG32 rstctrl_lte_clr;                   // 0x0000080c
    REG32 __2064[1];                         // 0x00000810
    REG32 lte_autogate_en_clr;               // 0x00000814
    REG32 __2072[1];                         // 0x00000818
    REG32 aon_lpc_ctrl_clr;                  // 0x0000081c
    REG32 aon_clock_en0_clr;                 // 0x00000820
    REG32 aon_clock_en1_clr;                 // 0x00000824
    REG32 aon_clock_auto_sel0_clr;           // 0x00000828
    REG32 aon_clock_auto_sel1_clr;           // 0x0000082c
    REG32 aon_clock_auto_sel2_clr;           // 0x00000830
    REG32 aon_clock_auto_sel3_clr;           // 0x00000834
    REG32 aon_clock_force_en0_clr;           // 0x00000838
    REG32 aon_clock_force_en1_clr;           // 0x0000083c
    REG32 aon_clock_force_en2_clr;           // 0x00000840
    REG32 aon_clock_force_en3_clr;           // 0x00000844
    REG32 aon_soft_rst_ctrl1_clr;            // 0x00000848
    REG32 mipi_csi_cfg_reg_clr;              // 0x0000084c
    REG32 cfg_clk_uart2_clr;                 // 0x00000850
    REG32 cfg_clk_uart3_clr;                 // 0x00000854
    REG32 cfg_clk_debug_host_clr;            // 0x00000858
    REG32 __2140[1];                         // 0x0000085c
    REG32 rc_calib_ctrl_clr;                 // 0x00000860
    REG32 __2148[2];                         // 0x00000864
    REG32 emmc_slice_phy_ctrl_clr;           // 0x0000086c
    REG32 dma_req_ctrl_clr;                  // 0x00000870
    REG32 apt_trigger_sel_clr;               // 0x00000874
    REG32 ahb2ahb_ab_funcdma_ctrl_clr;       // 0x00000878
    REG32 __2172[1];                         // 0x0000087c
    REG32 ahb2ahb_ab_dap_ctrl_clr;           // 0x00000880
    REG32 __2180[1];                         // 0x00000884
    REG32 ahb2axi_pub_ctrl_clr;              // 0x00000888
    REG32 __2188[3];                         // 0x0000088c
    REG32 ahb2ahb_ab_aon2lps_ctrl_clr;       // 0x00000898
    REG32 __2204[1];                         // 0x0000089c
    REG32 ahb2ahb_ab_lps2aon_ctrl_clr;       // 0x000008a0
    REG32 __2212[1];                         // 0x000008a4
    REG32 sysctrl_reg0_clr;                  // 0x000008a8
    REG32 __2220[1];                         // 0x000008ac
    REG32 cfg_aon_anti_hang_clr;             // 0x000008b0
    REG32 __2228[1];                         // 0x000008b4
    REG32 aon_ahb_mtx_slice_autogate_en_clr; // 0x000008b8
    REG32 dap_djtag_en_cfg_clr;              // 0x000008bc
    REG32 lte_ahb2ahb_sync_cfg_clr;          // 0x000008c0
    REG32 cfg_aon_io_core_ie_0_clr;          // 0x000008c4
    REG32 cfg_aon_io_core_ie_1_clr;          // 0x000008c8
    REG32 cfg_aon_io_core_ie_2_clr;          // 0x000008cc
    REG32 cfg_aon_io_core_ie_3_clr;          // 0x000008d0
} HWP_SYS_CTRL_T;

#define hwp_sysCtrl ((HWP_SYS_CTRL_T *)REG_ACCESS_ADDRESS(REG_SYS_CTRL_BASE))

// aon_soft_rst_ctrl0
typedef union {
    REG32 v;
    struct
    {
        REG32 ahbmux_soft_rst : 1;       // [0]
        REG32 ahb2axi_soft_rst : 1;      // [1]
        REG32 async_bridge_soft_rst : 1; // [2]
        REG32 dap_soft_rst : 1;          // [3]
        REG32 djtag_ctrl_soft_rst : 1;   // [4]
        REG32 efuse_soft_rst : 1;        // [5]
        REG32 lps_ifc_soft_rst : 1;      // [6]
        REG32 aon2lps_soft_rst : 1;      // [7]
        REG32 lps2aon_soft_rst : 1;      // [8]
        REG32 adimst_soft_rst : 1;       // [9]
        REG32 spinlock_soft_rst : 1;     // [10]
        REG32 aon_ifc_soft_rst : 1;      // [11]
        REG32 dbg_host_soft_rst : 1;     // [12]
        REG32 aif_soft_rst : 1;          // [13]
        REG32 uart2_soft_rst : 1;        // [14]
        REG32 uart3_soft_rst : 1;        // [15]
        REG32 idle_timer_soft_rst : 1;   // [16]
        REG32 aud_2ad_soft_rst : 1;      // [17]
        REG32 gpio2_soft_rst : 1;        // [18]
        REG32 gpt2_soft_rst : 1;         // [19]
        REG32 i2c3_soft_rst : 1;         // [20]
        REG32 mon_ctrl_soft_rst : 1;     // [21]
        REG32 sysmail_soft_rst : 1;      // [22]
        REG32 spi2_soft_rst : 1;         // [23]
        REG32 iomux_soft_rst : 1;        // [24]
        REG32 aon_imem_soft_rst : 1;     // [25]
        REG32 ana_wrap1_soft_rst : 1;    // [26]
        REG32 ana_wrap2_soft_rst : 1;    // [27]
        REG32 usbphy_soft_rst : 1;       // [28]
        REG32 scc_soft_rst : 1;          // [29]
        REG32 __31_30 : 2;               // [31:30]
    } b;
} REG_SYS_CTRL_AON_SOFT_RST_CTRL0_T;

// clken_lte
typedef union {
    REG32 v;
    struct
    {
        REG32 txrx_func_en : 1;    // [0]
        REG32 coeff_func_en : 1;   // [1]
        REG32 ldtc_func_en : 1;    // [2]
        REG32 ldtc1_func_en : 1;   // [3]
        REG32 measpwr_func_en : 1; // [4]
        REG32 iddet_func_en : 1;   // [5]
        REG32 otdoa_func_en : 1;   // [6]
        REG32 uldft_func_en : 1;   // [7]
        REG32 pusch_func_en : 1;   // [8]
        REG32 csirs_func_en : 1;   // [9]
        REG32 dlfft_func_en : 1;   // [10]
        REG32 rfad_func_en : 1;    // [11]
        REG32 rxcapt_func_en : 1;  // [12]
        REG32 hsdl_func_en : 1;    // [13]
        REG32 dbgio_func_en : 1;   // [14]
        REG32 __31_15 : 17;        // [31:15]
    } b;
} REG_SYS_CTRL_CLKEN_LTE_T;

// clken_lte_intf
typedef union {
    REG32 v;
    struct
    {
        REG32 txrx_intf_en : 1;    // [0]
        REG32 coeff_intf_en : 1;   // [1]
        REG32 ldtc_intf_en : 1;    // [2]
        REG32 ldtc1_intf_en : 1;   // [3]
        REG32 measpwr_intf_en : 1; // [4]
        REG32 iddet_intf_en : 1;   // [5]
        REG32 otdoa_intf_en : 1;   // [6]
        REG32 uldft_intf_en : 1;   // [7]
        REG32 pusch_intf_en : 1;   // [8]
        REG32 csirs_intf_en : 1;   // [9]
        REG32 dlfft_intf_en : 1;   // [10]
        REG32 rfad_intf_en : 1;    // [11]
        REG32 rxcapt_intf_en : 1;  // [12]
        REG32 hsdl_intf_en : 1;    // [13]
        REG32 dbgio_intf_en : 1;   // [14]
        REG32 __31_15 : 17;        // [31:15]
    } b;
} REG_SYS_CTRL_CLKEN_LTE_INTF_T;

// rstctrl_lte
typedef union {
    REG32 v;
    struct
    {
        REG32 txrx_tx_soft_rst : 1; // [0]
        REG32 txrx_rx_soft_rst : 1; // [1]
        REG32 coeff_soft_rst : 1;   // [2]
        REG32 ldtc_soft_rst : 1;    // [3]
        REG32 ldtc1_soft_rst : 1;   // [4]
        REG32 measpwr_soft_rst : 1; // [5]
        REG32 iddet_soft_rst : 1;   // [6]
        REG32 otdoa_soft_rst : 1;   // [7]
        REG32 uldft_soft_rst : 1;   // [8]
        REG32 pusch_soft_rst : 1;   // [9]
        REG32 csirs_soft_rst : 1;   // [10]
        REG32 dlfft_soft_rst : 1;   // [11]
        REG32 rfad_soft_rst : 1;    // [12]
        REG32 rxcapt_soft_rst : 1;  // [13]
        REG32 hsdl_soft_rst : 1;    // [14]
        REG32 dbgio_soft_rst : 1;   // [15]
        REG32 __31_16 : 16;         // [31:16]
    } b;
} REG_SYS_CTRL_RSTCTRL_LTE_T;

// lte_autogate_mode
typedef union {
    REG32 v;
    struct
    {
        REG32 lte_autogate_mode : 1; // [0]
        REG32 __31_1 : 31;           // [31:1]
    } b;
} REG_SYS_CTRL_LTE_AUTOGATE_MODE_T;

// lte_autogate_en
typedef union {
    REG32 v;
    struct
    {
        REG32 txrx_func_autogate_en : 1;     // [0]
        REG32 coeff_func_autogate_en : 1;    // [1]
        REG32 ldtc_func_autogate_en : 1;     // [2]
        REG32 ldtc1_func_autogate_en : 1;    // [3]
        REG32 measpwr_func_autogate_en : 1;  // [4]
        REG32 iddet_func_autogate_en : 1;    // [5]
        REG32 otdoa_func_autogate_en : 1;    // [6]
        REG32 uldft_func_autogate_en : 1;    // [7]
        REG32 pusch_func_autogate_en : 1;    // [8]
        REG32 csirs_func_autogate_en : 1;    // [9]
        REG32 dlfft_func_autogate_en : 1;    // [10]
        REG32 txrx_intf_autogate_en : 1;     // [11]
        REG32 coeff_intf_autogate_en : 1;    // [12]
        REG32 ldtc_intf_autogate_en : 1;     // [13]
        REG32 ldtc1_intf_autogate_en : 1;    // [14]
        REG32 measpwr_intf_autogate_en : 1;  // [15]
        REG32 iddet_intf_autogate_en : 1;    // [16]
        REG32 otdoa_intf_autogate_en : 1;    // [17]
        REG32 uldft_intf_autogate_en : 1;    // [18]
        REG32 pusch_intf_autogate_en : 1;    // [19]
        REG32 csirs_intf_autogate_en : 1;    // [20]
        REG32 dlfft_intf_autogate_en : 1;    // [21]
        REG32 __23_22 : 2;                   // [23:22]
        REG32 downlink_func_autogate_en : 1; // [24]
        REG32 uplink_func_autogate_en : 1;   // [25]
        REG32 downlink_intf_autogate_en : 1; // [26]
        REG32 uplink_intf_autogate_en : 1;   // [27]
        REG32 __31_28 : 4;                   // [31:28]
    } b;
} REG_SYS_CTRL_LTE_AUTOGATE_EN_T;

// lte_autogate_delay_num
typedef union {
    REG32 v;
    struct
    {
        REG32 lte_autogate_delay_number : 8; // [7:0]
        REG32 __31_8 : 24;                   // [31:8]
    } b;
} REG_SYS_CTRL_LTE_AUTOGATE_DELAY_NUM_T;

// aon_lpc_ctrl
typedef union {
    REG32 v;
    struct
    {
        REG32 lpc_en : 1;      // [0]
        REG32 lpc_frc_en : 1;  // [1]
        REG32 __7_2 : 6;       // [7:2]
        REG32 lpc_pu_num : 8;  // [15:8]
        REG32 lpc_pd_num : 16; // [31:16]
    } b;
} REG_SYS_CTRL_AON_LPC_CTRL_T;

// aon_clock_en0
typedef union {
    REG32 v;
    struct
    {
        REG32 aon_ahb_matrix_en : 1; // [0]
        REG32 aon_ahbmux_en : 1;     // [1]
        REG32 aon2lps_en : 1;        // [2]
        REG32 lps2aon_en : 1;        // [3]
        REG32 aon_imem_en : 1;       // [4]
        REG32 spinlock_en : 1;       // [5]
        REG32 efuse_ctrl_en : 1;     // [6]
        REG32 adimst_en : 1;         // [7]
        REG32 aon2pub_en : 1;        // [8]
        REG32 aonifc_en : 1;         // [9]
        REG32 lpsifc_en : 1;         // [10]
        REG32 gpt2_en : 1;           // [11]
        REG32 aud2ad_en : 1;         // [12]
        REG32 spi2_en : 1;           // [13]
        REG32 gpio2_en : 1;          // [14]
        REG32 mon_ctrl_en : 1;       // [15]
        REG32 aif_en : 1;            // [16]
        REG32 idle_timer_en : 1;     // [17]
        REG32 uart2_en : 1;          // [18]
        REG32 uart3_en : 1;          // [19]
        REG32 dbg_host_en : 1;       // [20]
        REG32 funcdma_en : 1;        // [21]
        REG32 dap_en : 1;            // [22]
        REG32 gnss_32k_en : 1;       // [23]
        REG32 usb_32k_en : 1;        // [24]
        REG32 sdio_1x_ap_en : 1;     // [25]
        REG32 sdio_1x_lte_en : 1;    // [26]
        REG32 sdio_aon_en : 1;       // [27]
        REG32 djtag_cfg_en : 1;      // [28]
        REG32 codec_mclock_en : 1;   // [29]
        REG32 clock_out_dbg_en : 1;  // [30]
        REG32 tsx_cal_en : 1;        // [31]
    } b;
} REG_SYS_CTRL_AON_CLOCK_EN0_T;

// aon_clock_en1
typedef union {
    REG32 v;
    struct
    {
        REG32 djtag_tck_en : 1;   // [0]
        REG32 usb_ref_en : 1;     // [1]
        REG32 psram_en : 1;       // [2]
        REG32 aon_ahb_ap_en : 1;  // [3]
        REG32 aon_ahb_cp_en : 1;  // [4]
        REG32 aon_ahb_pub_en : 1; // [5]
        REG32 aon_ahb_rf_en : 1;  // [6]
        REG32 calib_rc_en : 1;    // [7]
        REG32 fw_aon_en : 1;      // [8]
        REG32 scc_en : 1;         // [9]
        REG32 usb_ahb_usb_en : 1; // [10]
        REG32 usb_ahb_ap_en : 1;  // [11]
        REG32 __31_12 : 20;       // [31:12]
    } b;
} REG_SYS_CTRL_AON_CLOCK_EN1_T;

// aon_soft_rst_ctrl1
typedef union {
    REG32 v;
    struct
    {
        REG32 aon_djtag_soft_rst : 1;  // [0]
        REG32 ap_djtag_soft_rst : 1;   // [1]
        REG32 cp_djtag_soft_rst : 1;   // [2]
        REG32 rf_djtag_soft_rst : 1;   // [3]
        REG32 gnss_djtag_soft_rst : 1; // [4]
        REG32 pub_djtag_soft_rst : 1;  // [5]
        REG32 lte_djtag_soft_rst : 1;  // [6]
        REG32 usb_djtag_soft_rst : 1;  // [7]
        REG32 emmc_phy_soft_rst : 1;   // [8]
        REG32 rc_calib_soft_rst : 1;   // [9]
        REG32 __31_10 : 22;            // [31:10]
    } b;
} REG_SYS_CTRL_AON_SOFT_RST_CTRL1_T;

// mipi_csi_cfg_reg
typedef union {
    REG32 v;
    struct
    {
        REG32 csi_lvds_mode_sel : 1;       // [0]
        REG32 lvds_rx_terminal_enable : 1; // [1]
        REG32 __31_2 : 30;                 // [31:2]
    } b;
} REG_SYS_CTRL_MIPI_CSI_CFG_REG_T;

// cfg_clk_uart2
typedef union {
    REG32 v;
    struct
    {
        REG32 cfg_clk_uart2_num : 10;   // [9:0]
        REG32 __15_10 : 6;              // [15:10]
        REG32 cfg_clk_uart2_demod : 14; // [29:16]
        REG32 __30_30 : 1;              // [30]
        REG32 cfg_clk_uart2_update : 1; // [31]
    } b;
} REG_SYS_CTRL_CFG_CLK_UART2_T;

// cfg_clk_uart3
typedef union {
    REG32 v;
    struct
    {
        REG32 cfg_clk_uart3_num : 10;   // [9:0]
        REG32 __15_10 : 6;              // [15:10]
        REG32 cfg_clk_uart3_demod : 14; // [29:16]
        REG32 __30_30 : 1;              // [30]
        REG32 cfg_clk_uart3_update : 1; // [31]
    } b;
} REG_SYS_CTRL_CFG_CLK_UART3_T;

// cfg_clk_debug_host
typedef union {
    REG32 v;
    struct
    {
        REG32 cfg_clk_debug_host_num : 10;   // [9:0]
        REG32 __15_10 : 6;                   // [15:10]
        REG32 cfg_clk_debug_host_demod : 14; // [29:16]
        REG32 __30_30 : 1;                   // [30]
        REG32 cfg_clk_debug_host_update : 1; // [31]
    } b;
} REG_SYS_CTRL_CFG_CLK_DEBUG_HOST_T;

// rc_calib_ctrl
typedef union {
    REG32 v;
    struct
    {
        REG32 rc_calib_en : 1;      // [0]
        REG32 rc_calib_int_en : 1;  // [1]
        REG32 rc_calib_int_clr : 1; // [2]
        REG32 __31_3 : 29;          // [31:3]
    } b;
} REG_SYS_CTRL_RC_CALIB_CTRL_T;

// emmc_slice_phy_ctrl
typedef union {
    REG32 v;
    struct
    {
        REG32 emmc_module_sel : 1;   // [0]
        REG32 emmc_lte_slice_en : 1; // [1]
        REG32 __31_2 : 30;           // [31:2]
    } b;
} REG_SYS_CTRL_EMMC_SLICE_PHY_CTRL_T;

// dma_req_ctrl
typedef union {
    REG32 v;
    struct
    {
        REG32 busmon_dma_sel : 1; // [0]
        REG32 spi2_dma_sel : 1;   // [1]
        REG32 __31_2 : 30;        // [31:2]
    } b;
} REG_SYS_CTRL_DMA_REQ_CTRL_T;

// apt_trigger_sel
typedef union {
    REG32 v;
    struct
    {
        REG32 apt_trig_sel : 1; // [0]
        REG32 __31_1 : 31;      // [31:1]
    } b;
} REG_SYS_CTRL_APT_TRIGGER_SEL_T;

// ahb2ahb_ab_funcdma_ctrl
typedef union {
    REG32 v;
    struct
    {
        REG32 funcdma_bridge_incr_r_byte : 2;  // [1:0]
        REG32 funcdma_bridge_incr_r_half : 2;  // [3:2]
        REG32 funcdma_bridge_incr_r_word : 2;  // [5:4]
        REG32 funcdma_bridge_pause_req : 1;    // [6]
        REG32 funcdma_bridge_sleep_req : 1;    // [7]
        REG32 funcdma_bridge_timeout_en : 1;   // [8]
        REG32 funcdma_bridge_mode : 1;         // [9]
        REG32 funcdma_bridge_bypass : 1;       // [10]
        REG32 funcdma_bridge_en : 1;           // [11]
        REG32 funcdma_bridge_s_valid : 1;      // [12]
        REG32 funcdma_bridge_s_endian_sel : 1; // [13]
        REG32 funcdma_bridge_m_endian_sel : 1; // [14]
        REG32 __31_15 : 17;                    // [31:15]
    } b;
} REG_SYS_CTRL_AHB2AHB_AB_FUNCDMA_CTRL_T;

// ahb2ahb_ab_funcdma_sts
typedef union {
    REG32 v;
    struct
    {
        REG32 funcdma_bridge_sts_m_st : 2;            // [1:0], read only
        REG32 funcdma_bridge_pause_ready : 1;         // [2], read only
        REG32 funcdma_bridge_sleep_ready : 1;         // [3], read only
        REG32 funcdma_bridge_sts_m_idle : 1;          // [4], read only
        REG32 funcdma_bridge_sts_m_rfifo_empty : 1;   // [5], read only
        REG32 funcdma_bridge_sts_m_rfifo_full : 1;    // [6], read only
        REG32 funcdma_bridge_sts_m_cmdfifo_empty : 1; // [7], read only
        REG32 funcdma_bridge_sts_m_cmdfifo_full : 1;  // [8], read only
        REG32 funcdma_bridge_sts_s_idle : 1;          // [9], read only
        REG32 funcdma_bridge_sts_s_rfifo_empty : 1;   // [10], read only
        REG32 funcdma_bridge_sts_s_rfifo_full : 1;    // [11], read only
        REG32 funcdma_bridge_sts_s_cmdfifo_empty : 1; // [12], read only
        REG32 funcdma_bridge_sts_s_cmdfifo_full : 1;  // [13], read only
        REG32 __31_14 : 18;                           // [31:14]
    } b;
} REG_SYS_CTRL_AHB2AHB_AB_FUNCDMA_STS_T;

// ahb2ahb_ab_dap_ctrl
typedef union {
    REG32 v;
    struct
    {
        REG32 dap_bridge_incr_r_byte : 2;  // [1:0]
        REG32 dap_bridge_incr_r_half : 2;  // [3:2]
        REG32 dap_bridge_incr_r_word : 2;  // [5:4]
        REG32 dap_bridge_pause_req : 1;    // [6]
        REG32 dap_bridge_sleep_req : 1;    // [7]
        REG32 dap_bridge_timeout_en : 1;   // [8]
        REG32 dap_bridge_mode : 1;         // [9]
        REG32 dap_bridge_bypass : 1;       // [10]
        REG32 dap_bridge_en : 1;           // [11]
        REG32 dap_bridge_s_valid : 1;      // [12]
        REG32 dap_bridge_s_endian_sel : 1; // [13]
        REG32 dap_bridge_m_endian_sel : 1; // [14]
        REG32 __31_15 : 17;                // [31:15]
    } b;
} REG_SYS_CTRL_AHB2AHB_AB_DAP_CTRL_T;

// ahb2ahb_ab_dap_sts
typedef union {
    REG32 v;
    struct
    {
        REG32 dap_bridge_sts_m_st : 2;            // [1:0], read only
        REG32 dap_bridge_pause_ready : 1;         // [2], read only
        REG32 dap_bridge_sleep_ready : 1;         // [3], read only
        REG32 dap_bridge_sts_m_idle : 1;          // [4], read only
        REG32 dap_bridge_sts_m_rfifo_empty : 1;   // [5], read only
        REG32 dap_bridge_sts_m_rfifo_full : 1;    // [6], read only
        REG32 dap_bridge_sts_m_cmdfifo_empty : 1; // [7], read only
        REG32 dap_bridge_sts_m_cmdfifo_full : 1;  // [8], read only
        REG32 dap_bridge_sts_s_idle : 1;          // [9], read only
        REG32 dap_bridge_sts_s_rfifo_empty : 1;   // [10], read only
        REG32 dap_bridge_sts_s_rfifo_full : 1;    // [11], read only
        REG32 dap_bridge_sts_s_cmdfifo_empty : 1; // [12], read only
        REG32 dap_bridge_sts_s_cmdfifo_full : 1;  // [13], read only
        REG32 __31_14 : 18;                       // [31:14]
    } b;
} REG_SYS_CTRL_AHB2AHB_AB_DAP_STS_T;

// ahb2axi_pub_ctrl
typedef union {
    REG32 v;
    struct
    {
        REG32 ahb2axi_pub_mclk_next_on : 1;         // [0]
        REG32 ahb2axi_pub_sclk_next_on : 1;         // [1]
        REG32 ahb2axi_pub_clk_auto_gate_en : 1;     // [2]
        REG32 ahb2axi_pub_slv_disable_req : 1;      // [3]
        REG32 ahb2axi_pub_nonbuf_early_reqp_en : 1; // [4]
        REG32 ahb2axi_pub_trans_fencing_req : 1;    // [5]
        REG32 __31_6 : 26;                          // [31:6]
    } b;
} REG_SYS_CTRL_AHB2AXI_PUB_CTRL_T;

// ahb2axi_pub_sts
typedef union {
    REG32 v;
    struct
    {
        REG32 ahb2axi_pub_slv_disable_ack : 1;   // [0], read only
        REG32 ahb2axi_pub_bus_busy : 1;          // [1], read only
        REG32 ahb2axi_pub_trans_fencing_ack : 1; // [2], read only
        REG32 ahb2axi_pub_mclk_req : 1;          // [3], read only
        REG32 __31_4 : 28;                       // [31:4]
    } b;
} REG_SYS_CTRL_AHB2AXI_PUB_STS_T;

// axi2axi_pub_sts_0
typedef union {
    REG32 v;
    struct
    {
        REG32 axi2axi_pub_axi_detector_overflow : 1; // [0], read only
        REG32 axi2axi_pub_pwr_handshk_clk_req : 1;   // [1], read only
        REG32 axi2axi_pub_bridge_trans_idle : 1;     // [2], read only
        REG32 __31_3 : 29;                           // [31:3]
    } b;
} REG_SYS_CTRL_AXI2AXI_PUB_STS_0_T;

// ahb2ahb_ab_aon2lps_ctrl
typedef union {
    REG32 v;
    struct
    {
        REG32 ahb2ahb_ab_aon2lps_slv_disable_req : 1;      // [0]
        REG32 ahb2ahb_ab_aon2lps_nonbuf_early_resp_en : 1; // [1]
        REG32 ahb2ahb_ab_aon2lps_sync_mode : 1;            // [2]
        REG32 ahb2ahb_ab_aon2lps_fifo_clr : 1;             // [3]
        REG32 ahb2ahb_ab_aon2lps_mclk_auto_gate_en : 1;    // [4]
        REG32 ahb2ahb_ab_aon2lps_sclk_auto_gate_en : 1;    // [5]
        REG32 ahb2ahb_ab_aon2lps_trans_fencing_req : 1;    // [6]
        REG32 __31_7 : 25;                                 // [31:7]
    } b;
} REG_SYS_CTRL_AHB2AHB_AB_AON2LPS_CTRL_T;

// ahb2ahb_ab_aon2lps_sts
typedef union {
    REG32 v;
    struct
    {
        REG32 ahb2ahb_ab_aon2lps_slv_disable_ack : 1;   // [0], read only
        REG32 ahb2ahb_ab_aon2lps_m_bus_busy : 1;        // [1], read only
        REG32 ahb2ahb_ab_aon2lps_mclk_req : 1;          // [2], read only
        REG32 ahb2ahb_ab_aon2lps_sclk_req : 1;          // [3], read only
        REG32 ahb2ahb_ab_aon2lps_s_bus_busy : 1;        // [4], read only
        REG32 ahb2ahb_ab_aon2lps_trans_fencing_ack : 1; // [5], read only
        REG32 __31_6 : 26;                              // [31:6]
    } b;
} REG_SYS_CTRL_AHB2AHB_AB_AON2LPS_STS_T;

// ahb2ahb_ab_lps2aon_ctrl
typedef union {
    REG32 v;
    struct
    {
        REG32 ahb2ahb_ab_lps2aon_slv_disable_req : 1;      // [0]
        REG32 ahb2ahb_ab_lps2aon_nonbuf_early_resp_en : 1; // [1]
        REG32 ahb2ahb_ab_lps2aon_sync_mode : 1;            // [2]
        REG32 ahb2ahb_ab_lps2aon_fifo_clr : 1;             // [3]
        REG32 ahb2ahb_ab_lps2aon_mclk_auto_gate_en : 1;    // [4]
        REG32 ahb2ahb_ab_lps2aon_sclk_auto_gate_en : 1;    // [5]
        REG32 ahb2ahb_ab_lps2aon_trans_fencing_req : 1;    // [6]
        REG32 __31_7 : 25;                                 // [31:7]
    } b;
} REG_SYS_CTRL_AHB2AHB_AB_LPS2AON_CTRL_T;

// ahb2ahb_ab_lps2aon_sts
typedef union {
    REG32 v;
    struct
    {
        REG32 ahb2ahb_ab_lps2aon_slv_disable_ack : 1;   // [0], read only
        REG32 ahb2ahb_ab_lps2aon_m_bus_busy : 1;        // [1], read only
        REG32 ahb2ahb_ab_lps2aon_mclk_req : 1;          // [2], read only
        REG32 ahb2ahb_ab_lps2aon_sclk_req : 1;          // [3], read only
        REG32 ahb2ahb_ab_lps2aon_s_bus_busy : 1;        // [4], read only
        REG32 ahb2ahb_ab_lps2aon_trans_fencing_ack : 1; // [5], read only
        REG32 __31_6 : 26;                              // [31:6]
    } b;
} REG_SYS_CTRL_AHB2AHB_AB_LPS2AON_STS_T;

// sysctrl_reg0
typedef union {
    REG32 v;
    struct
    {
        REG32 spiflash2_nand_sel : 1;        // [0]
        REG32 ptest_func_atspeed_sel : 1;    // [1]
        REG32 exit_suspend_wait_xtal26m : 1; // [2]
        REG32 usb20_vbus_valid_sw : 1;       // [3]
        REG32 usb20_vbus_valid_sel : 1;      // [4]
        REG32 usb20_iddig : 1;               // [5]
        REG32 usb20_con_testmode : 1;        // [6]
        REG32 usb20_utmi_width_sel : 1;      // [7]
        REG32 aud_sclk_o_pn_sel : 1;         // [8]
        REG32 apll_ref_en : 1;               // [9]
        REG32 mpll_ref_en : 1;               // [10]
        REG32 iis_pll_ref_en : 1;            // [11]
        REG32 pmic_26m_en : 1;               // [12]
        REG32 rf_idle_enable : 1;            // [13]
        REG32 __31_14 : 18;                  // [31:14]
    } b;
} REG_SYS_CTRL_SYSCTRL_REG0_T;

// plls_sts
typedef union {
    REG32 v;
    struct
    {
        REG32 apll_state : 3;   // [2:0], read only
        REG32 __3_3 : 1;        // [3]
        REG32 mpll_state : 3;   // [6:4], read only
        REG32 __7_7 : 1;        // [7]
        REG32 iispll_state : 3; // [10:8], read only
        REG32 __31_11 : 21;     // [31:11]
    } b;
} REG_SYS_CTRL_PLLS_STS_T;

// cfg_aon_anti_hang
typedef union {
    REG32 v;
    struct
    {
        REG32 aon_ahbmux_err_resp_en : 1;        // [0]
        REG32 aon_apbmux_err_resp_en : 1;        // [1]
        REG32 aonifc_err_resp_en : 1;            // [2]
        REG32 lpsifc_err_resp_en : 1;            // [3]
        REG32 aon2pub_slv_disable_req_force : 1; // [4]
        REG32 aon2pub_slv_disable_req_sel : 1;   // [5]
        REG32 lte_err_resp_en : 1;               // [6]
        REG32 aon2ap_err_resp_en : 1;            // [7]
        REG32 aon2cp_err_resp_en : 1;            // [8]
        REG32 aon2rf_err_resp_en : 1;            // [9]
        REG32 aon2ap_slv_disable_req_force : 1;  // [10]
        REG32 aon2ap_slv_disable_req_sel : 1;    // [11]
        REG32 aon2cp_slv_disable_req_force : 1;  // [12]
        REG32 aon2cp_slv_disable_req_sel : 1;    // [13]
        REG32 aon2rf_slv_disable_req_force : 1;  // [14]
        REG32 aon2rf_slv_disable_req_sel : 1;    // [15]
        REG32 __31_16 : 16;                      // [31:16]
    } b;
} REG_SYS_CTRL_CFG_AON_ANTI_HANG_T;

// cfg_aon_qos
typedef union {
    REG32 v;
    struct
    {
        REG32 awqos_aon : 4; // [3:0]
        REG32 arqos_aon : 4; // [7:4]
        REG32 __31_8 : 24;   // [31:8]
    } b;
} REG_SYS_CTRL_CFG_AON_QOS_T;

// aon_ahb_mtx_slice_autogate_en
typedef union {
    REG32 v;
    struct
    {
        REG32 aon_ahb_mtx_slice_s0_auto_gate_en : 1; // [0]
        REG32 aon_ahb_mtx_slice_s1_auto_gate_en : 1; // [1]
        REG32 aon_ahb_mtx_slice_s2_auto_gate_en : 1; // [2]
        REG32 aon_ahb_mtx_slice_s3_auto_gate_en : 1; // [3]
        REG32 aon_ahb_mtx_slice_s4_auto_gate_en : 1; // [4]
        REG32 aon_ahb_mtx_slice_s5_auto_gate_en : 1; // [5]
        REG32 aon_ahb_mtx_slice_m0_auto_gate_en : 1; // [6]
        REG32 aon_ahb_mtx_slice_m1_auto_gate_en : 1; // [7]
        REG32 aon_ahb_mtx_slice_m2_auto_gate_en : 1; // [8]
        REG32 aon_ahb_mtx_slice_m3_auto_gate_en : 1; // [9]
        REG32 aon_ahb_mtx_slice_m4_auto_gate_en : 1; // [10]
        REG32 aon_ahb_mtx_slice_m5_auto_gate_en : 1; // [11]
        REG32 __31_12 : 20;                          // [31:12]
    } b;
} REG_SYS_CTRL_AON_AHB_MTX_SLICE_AUTOGATE_EN_T;

// dap_djtag_en_cfg
typedef union {
    REG32 v;
    struct
    {
        REG32 dap_djtag_en : 1; // [0]
        REG32 __31_1 : 31;      // [31:1]
    } b;
} REG_SYS_CTRL_DAP_DJTAG_EN_CFG_T;

// lte_ahb2ahb_sync_cfg
typedef union {
    REG32 v;
    struct
    {
        REG32 dma2phy_wr_early_resp_en : 1; // [0]
        REG32 dma2phy_auto_gating_en : 1;   // [1]
        REG32 cpu2phy_wr_early_resp_en : 1; // [2]
        REG32 cpu2phy_auto_gating_en : 1;   // [3]
        REG32 __31_4 : 28;                  // [31:4]
    } b;
} REG_SYS_CTRL_LTE_AHB2AHB_SYNC_CFG_T;

// aon_soft_rst_ctrl0
#define SYS_CTRL_AHBMUX_SOFT_RST (1 << 0)
#define SYS_CTRL_AHB2AXI_SOFT_RST (1 << 1)
#define SYS_CTRL_ASYNC_BRIDGE_SOFT_RST (1 << 2)
#define SYS_CTRL_DAP_SOFT_RST (1 << 3)
#define SYS_CTRL_DJTAG_CTRL_SOFT_RST (1 << 4)
#define SYS_CTRL_EFUSE_SOFT_RST (1 << 5)
#define SYS_CTRL_LPS_IFC_SOFT_RST (1 << 6)
#define SYS_CTRL_AON2LPS_SOFT_RST (1 << 7)
#define SYS_CTRL_LPS2AON_SOFT_RST (1 << 8)
#define SYS_CTRL_ADIMST_SOFT_RST (1 << 9)
#define SYS_CTRL_SPINLOCK_SOFT_RST (1 << 10)
#define SYS_CTRL_AON_IFC_SOFT_RST (1 << 11)
#define SYS_CTRL_DBG_HOST_SOFT_RST (1 << 12)
#define SYS_CTRL_AIF_SOFT_RST (1 << 13)
#define SYS_CTRL_UART2_SOFT_RST (1 << 14)
#define SYS_CTRL_UART3_SOFT_RST (1 << 15)
#define SYS_CTRL_IDLE_TIMER_SOFT_RST (1 << 16)
#define SYS_CTRL_AUD_2AD_SOFT_RST (1 << 17)
#define SYS_CTRL_GPIO2_SOFT_RST (1 << 18)
#define SYS_CTRL_GPT2_SOFT_RST (1 << 19)
#define SYS_CTRL_I2C3_SOFT_RST (1 << 20)
#define SYS_CTRL_MON_CTRL_SOFT_RST (1 << 21)
#define SYS_CTRL_SYSMAIL_SOFT_RST (1 << 22)
#define SYS_CTRL_SPI2_SOFT_RST (1 << 23)
#define SYS_CTRL_IOMUX_SOFT_RST (1 << 24)
#define SYS_CTRL_AON_IMEM_SOFT_RST (1 << 25)
#define SYS_CTRL_ANA_WRAP1_SOFT_RST (1 << 26)
#define SYS_CTRL_ANA_WRAP2_SOFT_RST (1 << 27)
#define SYS_CTRL_USBPHY_SOFT_RST (1 << 28)
#define SYS_CTRL_SCC_SOFT_RST (1 << 29)

// clken_lte
#define SYS_CTRL_TXRX_FUNC_EN (1 << 0)
#define SYS_CTRL_COEFF_FUNC_EN (1 << 1)
#define SYS_CTRL_LDTC_FUNC_EN (1 << 2)
#define SYS_CTRL_LDTC1_FUNC_EN (1 << 3)
#define SYS_CTRL_MEASPWR_FUNC_EN (1 << 4)
#define SYS_CTRL_IDDET_FUNC_EN (1 << 5)
#define SYS_CTRL_OTDOA_FUNC_EN (1 << 6)
#define SYS_CTRL_ULDFT_FUNC_EN (1 << 7)
#define SYS_CTRL_PUSCH_FUNC_EN (1 << 8)
#define SYS_CTRL_CSIRS_FUNC_EN (1 << 9)
#define SYS_CTRL_DLFFT_FUNC_EN (1 << 10)
#define SYS_CTRL_RFAD_FUNC_EN (1 << 11)
#define SYS_CTRL_RXCAPT_FUNC_EN (1 << 12)
#define SYS_CTRL_HSDL_FUNC_EN (1 << 13)
#define SYS_CTRL_DBGIO_FUNC_EN (1 << 14)

// clken_lte_intf
#define SYS_CTRL_TXRX_INTF_EN (1 << 0)
#define SYS_CTRL_COEFF_INTF_EN (1 << 1)
#define SYS_CTRL_LDTC_INTF_EN (1 << 2)
#define SYS_CTRL_LDTC1_INTF_EN (1 << 3)
#define SYS_CTRL_MEASPWR_INTF_EN (1 << 4)
#define SYS_CTRL_IDDET_INTF_EN (1 << 5)
#define SYS_CTRL_OTDOA_INTF_EN (1 << 6)
#define SYS_CTRL_ULDFT_INTF_EN (1 << 7)
#define SYS_CTRL_PUSCH_INTF_EN (1 << 8)
#define SYS_CTRL_CSIRS_INTF_EN (1 << 9)
#define SYS_CTRL_DLFFT_INTF_EN (1 << 10)
#define SYS_CTRL_RFAD_INTF_EN (1 << 11)
#define SYS_CTRL_RXCAPT_INTF_EN (1 << 12)
#define SYS_CTRL_HSDL_INTF_EN (1 << 13)
#define SYS_CTRL_DBGIO_INTF_EN (1 << 14)

// rstctrl_lte
#define SYS_CTRL_TXRX_TX_SOFT_RST (1 << 0)
#define SYS_CTRL_TXRX_RX_SOFT_RST (1 << 1)
#define SYS_CTRL_COEFF_SOFT_RST (1 << 2)
#define SYS_CTRL_LDTC_SOFT_RST (1 << 3)
#define SYS_CTRL_LDTC1_SOFT_RST (1 << 4)
#define SYS_CTRL_MEASPWR_SOFT_RST (1 << 5)
#define SYS_CTRL_IDDET_SOFT_RST (1 << 6)
#define SYS_CTRL_OTDOA_SOFT_RST (1 << 7)
#define SYS_CTRL_ULDFT_SOFT_RST (1 << 8)
#define SYS_CTRL_PUSCH_SOFT_RST (1 << 9)
#define SYS_CTRL_CSIRS_SOFT_RST (1 << 10)
#define SYS_CTRL_DLFFT_SOFT_RST (1 << 11)
#define SYS_CTRL_RFAD_SOFT_RST (1 << 12)
#define SYS_CTRL_RXCAPT_SOFT_RST (1 << 13)
#define SYS_CTRL_HSDL_SOFT_RST (1 << 14)
#define SYS_CTRL_DBGIO_SOFT_RST (1 << 15)

// lte_autogate_mode
#define SYS_CTRL_LTE_AUTOGATE_MODE (1 << 0)

// lte_autogate_en
#define SYS_CTRL_TXRX_FUNC_AUTOGATE_EN (1 << 0)
#define SYS_CTRL_COEFF_FUNC_AUTOGATE_EN (1 << 1)
#define SYS_CTRL_LDTC_FUNC_AUTOGATE_EN (1 << 2)
#define SYS_CTRL_LDTC1_FUNC_AUTOGATE_EN (1 << 3)
#define SYS_CTRL_MEASPWR_FUNC_AUTOGATE_EN (1 << 4)
#define SYS_CTRL_IDDET_FUNC_AUTOGATE_EN (1 << 5)
#define SYS_CTRL_OTDOA_FUNC_AUTOGATE_EN (1 << 6)
#define SYS_CTRL_ULDFT_FUNC_AUTOGATE_EN (1 << 7)
#define SYS_CTRL_PUSCH_FUNC_AUTOGATE_EN (1 << 8)
#define SYS_CTRL_CSIRS_FUNC_AUTOGATE_EN (1 << 9)
#define SYS_CTRL_DLFFT_FUNC_AUTOGATE_EN (1 << 10)
#define SYS_CTRL_TXRX_INTF_AUTOGATE_EN (1 << 11)
#define SYS_CTRL_COEFF_INTF_AUTOGATE_EN (1 << 12)
#define SYS_CTRL_LDTC_INTF_AUTOGATE_EN (1 << 13)
#define SYS_CTRL_LDTC1_INTF_AUTOGATE_EN (1 << 14)
#define SYS_CTRL_MEASPWR_INTF_AUTOGATE_EN (1 << 15)
#define SYS_CTRL_IDDET_INTF_AUTOGATE_EN (1 << 16)
#define SYS_CTRL_OTDOA_INTF_AUTOGATE_EN (1 << 17)
#define SYS_CTRL_ULDFT_INTF_AUTOGATE_EN (1 << 18)
#define SYS_CTRL_PUSCH_INTF_AUTOGATE_EN (1 << 19)
#define SYS_CTRL_CSIRS_INTF_AUTOGATE_EN (1 << 20)
#define SYS_CTRL_DLFFT_INTF_AUTOGATE_EN (1 << 21)
#define SYS_CTRL_DOWNLINK_FUNC_AUTOGATE_EN (1 << 24)
#define SYS_CTRL_UPLINK_FUNC_AUTOGATE_EN (1 << 25)
#define SYS_CTRL_DOWNLINK_INTF_AUTOGATE_EN (1 << 26)
#define SYS_CTRL_UPLINK_INTF_AUTOGATE_EN (1 << 27)

// lte_autogate_delay_num
#define SYS_CTRL_LTE_AUTOGATE_DELAY_NUMBER(n) (((n)&0xff) << 0)

// aon_lpc_ctrl
#define SYS_CTRL_LPC_EN (1 << 0)
#define SYS_CTRL_LPC_FRC_EN (1 << 1)
#define SYS_CTRL_LPC_PU_NUM(n) (((n)&0xff) << 8)
#define SYS_CTRL_LPC_PD_NUM(n) (((n)&0xffff) << 16)

// aon_clock_en0
#define SYS_CTRL_AON_AHB_MATRIX_EN (1 << 0)
#define SYS_CTRL_AON_AHBMUX_EN (1 << 1)
#define SYS_CTRL_AON2LPS_EN (1 << 2)
#define SYS_CTRL_LPS2AON_EN (1 << 3)
#define SYS_CTRL_AON_IMEM_EN (1 << 4)
#define SYS_CTRL_SPINLOCK_EN (1 << 5)
#define SYS_CTRL_EFUSE_CTRL_EN (1 << 6)
#define SYS_CTRL_ADIMST_EN (1 << 7)
#define SYS_CTRL_AON2PUB_EN (1 << 8)
#define SYS_CTRL_AONIFC_EN (1 << 9)
#define SYS_CTRL_LPSIFC_EN (1 << 10)
#define SYS_CTRL_GPT2_EN (1 << 11)
#define SYS_CTRL_AUD2AD_EN (1 << 12)
#define SYS_CTRL_SPI2_EN (1 << 13)
#define SYS_CTRL_GPIO2_EN (1 << 14)
#define SYS_CTRL_MON_CTRL_EN (1 << 15)
#define SYS_CTRL_AIF_EN (1 << 16)
#define SYS_CTRL_IDLE_TIMER_EN (1 << 17)
#define SYS_CTRL_UART2_EN (1 << 18)
#define SYS_CTRL_UART3_EN (1 << 19)
#define SYS_CTRL_DBG_HOST_EN (1 << 20)
#define SYS_CTRL_FUNCDMA_EN (1 << 21)
#define SYS_CTRL_DAP_EN (1 << 22)
#define SYS_CTRL_GNSS_32K_EN (1 << 23)
#define SYS_CTRL_USB_32K_EN (1 << 24)
#define SYS_CTRL_SDIO_1X_AP_EN (1 << 25)
#define SYS_CTRL_SDIO_1X_LTE_EN (1 << 26)
#define SYS_CTRL_SDIO_AON_EN (1 << 27)
#define SYS_CTRL_DJTAG_CFG_EN (1 << 28)
#define SYS_CTRL_CODEC_MCLOCK_EN (1 << 29)
#define SYS_CTRL_CLOCK_OUT_DBG_EN (1 << 30)
#define SYS_CTRL_TSX_CAL_EN (1 << 31)

// aon_clock_en1
#define SYS_CTRL_DJTAG_TCK_EN (1 << 0)
#define SYS_CTRL_USB_REF_EN (1 << 1)
#define SYS_CTRL_PSRAM_EN (1 << 2)
#define SYS_CTRL_AON_AHB_AP_EN (1 << 3)
#define SYS_CTRL_AON_AHB_CP_EN (1 << 4)
#define SYS_CTRL_AON_AHB_PUB_EN (1 << 5)
#define SYS_CTRL_AON_AHB_RF_EN (1 << 6)
#define SYS_CTRL_CALIB_RC_EN (1 << 7)
#define SYS_CTRL_FW_AON_EN (1 << 8)
#define SYS_CTRL_SCC_EN (1 << 9)
#define SYS_CTRL_USB_AHB_USB_EN (1 << 10)
#define SYS_CTRL_USB_AHB_AP_EN (1 << 11)

// aon_soft_rst_ctrl1
#define SYS_CTRL_AON_DJTAG_SOFT_RST (1 << 0)
#define SYS_CTRL_AP_DJTAG_SOFT_RST (1 << 1)
#define SYS_CTRL_CP_DJTAG_SOFT_RST (1 << 2)
#define SYS_CTRL_RF_DJTAG_SOFT_RST (1 << 3)
#define SYS_CTRL_GNSS_DJTAG_SOFT_RST (1 << 4)
#define SYS_CTRL_PUB_DJTAG_SOFT_RST (1 << 5)
#define SYS_CTRL_LTE_DJTAG_SOFT_RST (1 << 6)
#define SYS_CTRL_USB_DJTAG_SOFT_RST (1 << 7)
#define SYS_CTRL_EMMC_PHY_SOFT_RST (1 << 8)
#define SYS_CTRL_RC_CALIB_SOFT_RST (1 << 9)

// mipi_csi_cfg_reg
#define SYS_CTRL_CSI_LVDS_MODE_SEL (1 << 0)
#define SYS_CTRL_LVDS_RX_TERMINAL_ENABLE (1 << 1)

// cfg_clk_uart2
#define SYS_CTRL_CFG_CLK_UART2_NUM(n) (((n)&0x3ff) << 0)
#define SYS_CTRL_CFG_CLK_UART2_DEMOD(n) (((n)&0x3fff) << 16)
#define SYS_CTRL_CFG_CLK_UART2_UPDATE (1 << 31)

// cfg_clk_uart3
#define SYS_CTRL_CFG_CLK_UART3_NUM(n) (((n)&0x3ff) << 0)
#define SYS_CTRL_CFG_CLK_UART3_DEMOD(n) (((n)&0x3fff) << 16)
#define SYS_CTRL_CFG_CLK_UART3_UPDATE (1 << 31)

// cfg_clk_debug_host
#define SYS_CTRL_CFG_CLK_DEBUG_HOST_NUM(n) (((n)&0x3ff) << 0)
#define SYS_CTRL_CFG_CLK_DEBUG_HOST_DEMOD(n) (((n)&0x3fff) << 16)
#define SYS_CTRL_CFG_CLK_DEBUG_HOST_UPDATE (1 << 31)

// rc_calib_ctrl
#define SYS_CTRL_RC_CALIB_EN (1 << 0)
#define SYS_CTRL_RC_CALIB_INT_EN (1 << 1)
#define SYS_CTRL_RC_CALIB_INT_CLR (1 << 2)

// emmc_slice_phy_ctrl
#define SYS_CTRL_EMMC_MODULE_SEL (1 << 0)
#define SYS_CTRL_EMMC_LTE_SLICE_EN (1 << 1)

// dma_req_ctrl
#define SYS_CTRL_BUSMON_DMA_SEL (1 << 0)
#define SYS_CTRL_SPI2_DMA_SEL (1 << 1)

// apt_trigger_sel
#define SYS_CTRL_APT_TRIG_SEL (1 << 0)

// ahb2ahb_ab_funcdma_ctrl
#define SYS_CTRL_FUNCDMA_BRIDGE_INCR_R_BYTE(n) (((n)&0x3) << 0)
#define SYS_CTRL_FUNCDMA_BRIDGE_INCR_R_HALF(n) (((n)&0x3) << 2)
#define SYS_CTRL_FUNCDMA_BRIDGE_INCR_R_WORD(n) (((n)&0x3) << 4)
#define SYS_CTRL_FUNCDMA_BRIDGE_PAUSE_REQ (1 << 6)
#define SYS_CTRL_FUNCDMA_BRIDGE_SLEEP_REQ (1 << 7)
#define SYS_CTRL_FUNCDMA_BRIDGE_TIMEOUT_EN (1 << 8)
#define SYS_CTRL_FUNCDMA_BRIDGE_MODE (1 << 9)
#define SYS_CTRL_FUNCDMA_BRIDGE_BYPASS (1 << 10)
#define SYS_CTRL_FUNCDMA_BRIDGE_EN (1 << 11)
#define SYS_CTRL_FUNCDMA_BRIDGE_S_VALID (1 << 12)
#define SYS_CTRL_FUNCDMA_BRIDGE_S_ENDIAN_SEL (1 << 13)
#define SYS_CTRL_FUNCDMA_BRIDGE_M_ENDIAN_SEL (1 << 14)

// ahb2ahb_ab_funcdma_sts
#define SYS_CTRL_FUNCDMA_BRIDGE_STS_M_ST(n) (((n)&0x3) << 0)
#define SYS_CTRL_FUNCDMA_BRIDGE_PAUSE_READY (1 << 2)
#define SYS_CTRL_FUNCDMA_BRIDGE_SLEEP_READY (1 << 3)
#define SYS_CTRL_FUNCDMA_BRIDGE_STS_M_IDLE (1 << 4)
#define SYS_CTRL_FUNCDMA_BRIDGE_STS_M_RFIFO_EMPTY (1 << 5)
#define SYS_CTRL_FUNCDMA_BRIDGE_STS_M_RFIFO_FULL (1 << 6)
#define SYS_CTRL_FUNCDMA_BRIDGE_STS_M_CMDFIFO_EMPTY (1 << 7)
#define SYS_CTRL_FUNCDMA_BRIDGE_STS_M_CMDFIFO_FULL (1 << 8)
#define SYS_CTRL_FUNCDMA_BRIDGE_STS_S_IDLE (1 << 9)
#define SYS_CTRL_FUNCDMA_BRIDGE_STS_S_RFIFO_EMPTY (1 << 10)
#define SYS_CTRL_FUNCDMA_BRIDGE_STS_S_RFIFO_FULL (1 << 11)
#define SYS_CTRL_FUNCDMA_BRIDGE_STS_S_CMDFIFO_EMPTY (1 << 12)
#define SYS_CTRL_FUNCDMA_BRIDGE_STS_S_CMDFIFO_FULL (1 << 13)

// ahb2ahb_ab_dap_ctrl
#define SYS_CTRL_DAP_BRIDGE_INCR_R_BYTE(n) (((n)&0x3) << 0)
#define SYS_CTRL_DAP_BRIDGE_INCR_R_HALF(n) (((n)&0x3) << 2)
#define SYS_CTRL_DAP_BRIDGE_INCR_R_WORD(n) (((n)&0x3) << 4)
#define SYS_CTRL_DAP_BRIDGE_PAUSE_REQ (1 << 6)
#define SYS_CTRL_DAP_BRIDGE_SLEEP_REQ (1 << 7)
#define SYS_CTRL_DAP_BRIDGE_TIMEOUT_EN (1 << 8)
#define SYS_CTRL_DAP_BRIDGE_MODE (1 << 9)
#define SYS_CTRL_DAP_BRIDGE_BYPASS (1 << 10)
#define SYS_CTRL_DAP_BRIDGE_EN (1 << 11)
#define SYS_CTRL_DAP_BRIDGE_S_VALID (1 << 12)
#define SYS_CTRL_DAP_BRIDGE_S_ENDIAN_SEL (1 << 13)
#define SYS_CTRL_DAP_BRIDGE_M_ENDIAN_SEL (1 << 14)

// ahb2ahb_ab_dap_sts
#define SYS_CTRL_DAP_BRIDGE_STS_M_ST(n) (((n)&0x3) << 0)
#define SYS_CTRL_DAP_BRIDGE_PAUSE_READY (1 << 2)
#define SYS_CTRL_DAP_BRIDGE_SLEEP_READY (1 << 3)
#define SYS_CTRL_DAP_BRIDGE_STS_M_IDLE (1 << 4)
#define SYS_CTRL_DAP_BRIDGE_STS_M_RFIFO_EMPTY (1 << 5)
#define SYS_CTRL_DAP_BRIDGE_STS_M_RFIFO_FULL (1 << 6)
#define SYS_CTRL_DAP_BRIDGE_STS_M_CMDFIFO_EMPTY (1 << 7)
#define SYS_CTRL_DAP_BRIDGE_STS_M_CMDFIFO_FULL (1 << 8)
#define SYS_CTRL_DAP_BRIDGE_STS_S_IDLE (1 << 9)
#define SYS_CTRL_DAP_BRIDGE_STS_S_RFIFO_EMPTY (1 << 10)
#define SYS_CTRL_DAP_BRIDGE_STS_S_RFIFO_FULL (1 << 11)
#define SYS_CTRL_DAP_BRIDGE_STS_S_CMDFIFO_EMPTY (1 << 12)
#define SYS_CTRL_DAP_BRIDGE_STS_S_CMDFIFO_FULL (1 << 13)

// ahb2axi_pub_ctrl
#define SYS_CTRL_AHB2AXI_PUB_MCLK_NEXT_ON (1 << 0)
#define SYS_CTRL_AHB2AXI_PUB_SCLK_NEXT_ON (1 << 1)
#define SYS_CTRL_AHB2AXI_PUB_CLK_AUTO_GATE_EN (1 << 2)
#define SYS_CTRL_AHB2AXI_PUB_SLV_DISABLE_REQ (1 << 3)
#define SYS_CTRL_AHB2AXI_PUB_NONBUF_EARLY_REQP_EN (1 << 4)
#define SYS_CTRL_AHB2AXI_PUB_TRANS_FENCING_REQ (1 << 5)

// ahb2axi_pub_sts
#define SYS_CTRL_AHB2AXI_PUB_SLV_DISABLE_ACK (1 << 0)
#define SYS_CTRL_AHB2AXI_PUB_BUS_BUSY (1 << 1)
#define SYS_CTRL_AHB2AXI_PUB_TRANS_FENCING_ACK (1 << 2)
#define SYS_CTRL_AHB2AXI_PUB_MCLK_REQ (1 << 3)

// axi2axi_pub_sts_0
#define SYS_CTRL_AXI2AXI_PUB_AXI_DETECTOR_OVERFLOW (1 << 0)
#define SYS_CTRL_AXI2AXI_PUB_PWR_HANDSHK_CLK_REQ (1 << 1)
#define SYS_CTRL_AXI2AXI_PUB_BRIDGE_TRANS_IDLE (1 << 2)

// ahb2ahb_ab_aon2lps_ctrl
#define SYS_CTRL_AHB2AHB_AB_AON2LPS_SLV_DISABLE_REQ (1 << 0)
#define SYS_CTRL_AHB2AHB_AB_AON2LPS_NONBUF_EARLY_RESP_EN (1 << 1)
#define SYS_CTRL_AHB2AHB_AB_AON2LPS_SYNC_MODE (1 << 2)
#define SYS_CTRL_AHB2AHB_AB_AON2LPS_FIFO_CLR (1 << 3)
#define SYS_CTRL_AHB2AHB_AB_AON2LPS_MCLK_AUTO_GATE_EN (1 << 4)
#define SYS_CTRL_AHB2AHB_AB_AON2LPS_SCLK_AUTO_GATE_EN (1 << 5)
#define SYS_CTRL_AHB2AHB_AB_AON2LPS_TRANS_FENCING_REQ (1 << 6)

// ahb2ahb_ab_aon2lps_sts
#define SYS_CTRL_AHB2AHB_AB_AON2LPS_SLV_DISABLE_ACK (1 << 0)
#define SYS_CTRL_AHB2AHB_AB_AON2LPS_M_BUS_BUSY (1 << 1)
#define SYS_CTRL_AHB2AHB_AB_AON2LPS_MCLK_REQ (1 << 2)
#define SYS_CTRL_AHB2AHB_AB_AON2LPS_SCLK_REQ (1 << 3)
#define SYS_CTRL_AHB2AHB_AB_AON2LPS_S_BUS_BUSY (1 << 4)
#define SYS_CTRL_AHB2AHB_AB_AON2LPS_TRANS_FENCING_ACK (1 << 5)

// ahb2ahb_ab_lps2aon_ctrl
#define SYS_CTRL_AHB2AHB_AB_LPS2AON_SLV_DISABLE_REQ (1 << 0)
#define SYS_CTRL_AHB2AHB_AB_LPS2AON_NONBUF_EARLY_RESP_EN (1 << 1)
#define SYS_CTRL_AHB2AHB_AB_LPS2AON_SYNC_MODE (1 << 2)
#define SYS_CTRL_AHB2AHB_AB_LPS2AON_FIFO_CLR (1 << 3)
#define SYS_CTRL_AHB2AHB_AB_LPS2AON_MCLK_AUTO_GATE_EN (1 << 4)
#define SYS_CTRL_AHB2AHB_AB_LPS2AON_SCLK_AUTO_GATE_EN (1 << 5)
#define SYS_CTRL_AHB2AHB_AB_LPS2AON_TRANS_FENCING_REQ (1 << 6)

// ahb2ahb_ab_lps2aon_sts
#define SYS_CTRL_AHB2AHB_AB_LPS2AON_SLV_DISABLE_ACK (1 << 0)
#define SYS_CTRL_AHB2AHB_AB_LPS2AON_M_BUS_BUSY (1 << 1)
#define SYS_CTRL_AHB2AHB_AB_LPS2AON_MCLK_REQ (1 << 2)
#define SYS_CTRL_AHB2AHB_AB_LPS2AON_SCLK_REQ (1 << 3)
#define SYS_CTRL_AHB2AHB_AB_LPS2AON_S_BUS_BUSY (1 << 4)
#define SYS_CTRL_AHB2AHB_AB_LPS2AON_TRANS_FENCING_ACK (1 << 5)

// sysctrl_reg0
#define SYS_CTRL_SPIFLASH2_NAND_SEL (1 << 0)
#define SYS_CTRL_PTEST_FUNC_ATSPEED_SEL (1 << 1)
#define SYS_CTRL_EXIT_SUSPEND_WAIT_XTAL26M (1 << 2)
#define SYS_CTRL_USB20_VBUS_VALID_SW (1 << 3)
#define SYS_CTRL_USB20_VBUS_VALID_SEL (1 << 4)
#define SYS_CTRL_USB20_IDDIG (1 << 5)
#define SYS_CTRL_USB20_CON_TESTMODE (1 << 6)
#define SYS_CTRL_USB20_UTMI_WIDTH_SEL (1 << 7)
#define SYS_CTRL_AUD_SCLK_O_PN_SEL (1 << 8)
#define SYS_CTRL_APLL_REF_EN (1 << 9)
#define SYS_CTRL_MPLL_REF_EN (1 << 10)
#define SYS_CTRL_IIS_PLL_REF_EN (1 << 11)
#define SYS_CTRL_PMIC_26M_EN (1 << 12)
#define SYS_CTRL_RF_IDLE_ENABLE (1 << 13)

// plls_sts
#define SYS_CTRL_APLL_STATE(n) (((n)&0x7) << 0)
#define SYS_CTRL_MPLL_STATE(n) (((n)&0x7) << 4)
#define SYS_CTRL_IISPLL_STATE(n) (((n)&0x7) << 8)

// cfg_aon_anti_hang
#define SYS_CTRL_AON_AHBMUX_ERR_RESP_EN (1 << 0)
#define SYS_CTRL_AON_APBMUX_ERR_RESP_EN (1 << 1)
#define SYS_CTRL_AONIFC_ERR_RESP_EN (1 << 2)
#define SYS_CTRL_LPSIFC_ERR_RESP_EN (1 << 3)
#define SYS_CTRL_AON2PUB_SLV_DISABLE_REQ_FORCE (1 << 4)
#define SYS_CTRL_AON2PUB_SLV_DISABLE_REQ_SEL (1 << 5)
#define SYS_CTRL_LTE_ERR_RESP_EN (1 << 6)
#define SYS_CTRL_AON2AP_ERR_RESP_EN (1 << 7)
#define SYS_CTRL_AON2CP_ERR_RESP_EN (1 << 8)
#define SYS_CTRL_AON2RF_ERR_RESP_EN (1 << 9)
#define SYS_CTRL_AON2AP_SLV_DISABLE_REQ_FORCE (1 << 10)
#define SYS_CTRL_AON2AP_SLV_DISABLE_REQ_SEL (1 << 11)
#define SYS_CTRL_AON2CP_SLV_DISABLE_REQ_FORCE (1 << 12)
#define SYS_CTRL_AON2CP_SLV_DISABLE_REQ_SEL (1 << 13)
#define SYS_CTRL_AON2RF_SLV_DISABLE_REQ_FORCE (1 << 14)
#define SYS_CTRL_AON2RF_SLV_DISABLE_REQ_SEL (1 << 15)

// cfg_aon_qos
#define SYS_CTRL_AWQOS_AON(n) (((n)&0xf) << 0)
#define SYS_CTRL_ARQOS_AON(n) (((n)&0xf) << 4)

// aon_ahb_mtx_slice_autogate_en
#define SYS_CTRL_AON_AHB_MTX_SLICE_S0_AUTO_GATE_EN (1 << 0)
#define SYS_CTRL_AON_AHB_MTX_SLICE_S1_AUTO_GATE_EN (1 << 1)
#define SYS_CTRL_AON_AHB_MTX_SLICE_S2_AUTO_GATE_EN (1 << 2)
#define SYS_CTRL_AON_AHB_MTX_SLICE_S3_AUTO_GATE_EN (1 << 3)
#define SYS_CTRL_AON_AHB_MTX_SLICE_S4_AUTO_GATE_EN (1 << 4)
#define SYS_CTRL_AON_AHB_MTX_SLICE_S5_AUTO_GATE_EN (1 << 5)
#define SYS_CTRL_AON_AHB_MTX_SLICE_M0_AUTO_GATE_EN (1 << 6)
#define SYS_CTRL_AON_AHB_MTX_SLICE_M1_AUTO_GATE_EN (1 << 7)
#define SYS_CTRL_AON_AHB_MTX_SLICE_M2_AUTO_GATE_EN (1 << 8)
#define SYS_CTRL_AON_AHB_MTX_SLICE_M3_AUTO_GATE_EN (1 << 9)
#define SYS_CTRL_AON_AHB_MTX_SLICE_M4_AUTO_GATE_EN (1 << 10)
#define SYS_CTRL_AON_AHB_MTX_SLICE_M5_AUTO_GATE_EN (1 << 11)

// dap_djtag_en_cfg
#define SYS_CTRL_DAP_DJTAG_EN (1 << 0)

// lte_ahb2ahb_sync_cfg
#define SYS_CTRL_DMA2PHY_WR_EARLY_RESP_EN (1 << 0)
#define SYS_CTRL_DMA2PHY_AUTO_GATING_EN (1 << 1)
#define SYS_CTRL_CPU2PHY_WR_EARLY_RESP_EN (1 << 2)
#define SYS_CTRL_CPU2PHY_AUTO_GATING_EN (1 << 3)
#endif

